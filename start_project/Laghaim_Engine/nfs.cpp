#include "stdafx.h"
//----------------------------------------------------------------------------
//
// nfs File System
//
//   Released on February 23, 2005 by:
//
//      Radu Gruian
//      web:   http://www.gruian.com
//      email: gruian@cs.rutgers.edu
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to:
//
//      Free Software Foundation, Inc.
//      59 Temple Place, Suite 330
//      Boston, MA 02111-1307
//      USA
//
//----------------------------------------------------------------------------
//
// File:    nfs.c
// Date:    07/08/02
// Content: Main module
//
//----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>             /* O_xxx defines */
#include "nfs_iio.h"           /* Interlaced I/O */
#include "nfs_pmatch.h"        /* Pattern matching */
#include "nfs_dt.h"            /* Directory Table */
#include "nfs_nt.h"            /* Node Table */
#include "nfs_fat.h"           /* File Allocation Table */
#include "nfs_data.h"          /* Data Block I/O (virtual disk) */
#include "nfs.h"               /* nfs */
//----------------------------------------------------------------------------
#pragma warning( disable : 4996)
/* --- INTERNAL -------------------------------------------------------------- */

int nfs_errno;

/* true if the file exists, false otherwise */
static int file_exists(char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
		return 0;
	fclose(f);
	return 1;
}

/* truncate a file */
static void file_truncate(nfs_Handle* fs, nfs_FH* fh, int size)
{

	int node_no, node;

	/* truncate the chain */
	node_no = size / nfs_data_BLOCK_SIZE;
	node    = nfs_fat_chain_get_nth(fs->fat, fh->chain, node_no);
	nfs_fat_chain_truncate(fs->fat, node);

	/* update the size of data (field in the node in NT) */
	nfs_nt_node_set_size(fs->nt, fh->nt_index, size);

}

/* allocate a new (empty) file descriptor and update the
file system handle to reflect the new state */
static int allocate_file_descriptor(nfs_Handle* fs)
{
	int i = 0;
	nfs_FH* fh = (nfs_FH*)malloc(sizeof(nfs_FH));
	if (fh == NULL)
		return -1;
	fh->dt_index = 0;
	fh->nt_index = 0;
	fh->chain    = 0;
	fh->fp       = 0;

	/* check if h-table is already open, and look for an available slot */
	if (fs->handles != NULL)
	{
		while ( (i < fs->ht_size) && (fs->handles[i] != NULL) )
			i++;
		if (i >= fs->ht_size)
		{
			/* has to be expanded */
			fs->handles = (nfs_FH**)realloc(fs->handles, sizeof(nfs_FH*) * (fs->ht_size * 2));
			if (fs->handles == NULL)
				return -1;
			for (i = fs->ht_size; i < (fs->ht_size * 2); fs->handles[i++] = NULL);
			i = fs->ht_size; /* .. which is obviously empty */
			fs->ht_size <<= 1; /* double it! */
			fs->handles[i] = fh;
			return i;
		}
		else
		{
			/* empty slot */
			fs->handles[i] = fh;
			return i;
		}
	}

	/* first time a file is open */
	fs->ht_size = nfs_INITIAL_HT_SIZE;
	fs->handles = (nfs_FH**)malloc(sizeof(nfs_FH*) * fs->ht_size);
	if (fs->handles == NULL)
		return -1;
	for (i = 0; i < fs->ht_size; fs->handles[i++] = NULL);
	fs->handles[0] = fh;
	return 0; /* first index chosen */

}

static int deallocate_file_descriptor(nfs_Handle* fs, int filedes)
{

	/* dumb-proof */

	if (fs == NULL)
		return nfs_ERR_INVALID_FS;

	if (fs->handles[filedes] == NULL)
		return nfs_ERR_ALREADY_CLSD;

	/* deallocate the descriptor */
	free(fs->handles[filedes]);
	fs->handles[filedes] = NULL;

	return 0;

}

/* --- EXPORTED -------------------------------------------------- */

/* Returns nonzero (true) if the given file system is available, false (0) if not */
int nfs_exists(char* filename)
{
	char iio_name[nfs_NAME_LEN];
	char data_name[nfs_NAME_LEN];
	/* form the appropriate names for iio/data */
	strcpy(iio_name, filename);
	strcpy(&iio_name[strlen(filename)], nfs_IIO_SUFFIX);
	strcpy(data_name, filename);
	strcpy(&data_name[strlen(filename)], nfs_DATA_SUFFIX);
	/* do these files exist? */
	return (file_exists(iio_name) && file_exists(data_name));
}

/* create a lock file, using the given mode (read/write),
and initialize the lock's refcount to 1 */
static int lock_create(char* filename, int mode)
{
	int refcount;
	FILE* f;
	char lock_name[nfs_NAME_LEN];

	/* open lock */
	strcpy(lock_name, filename);
	strcpy(&lock_name[strlen(filename)], nfs_LOCK_SUFFIX);
	f = fopen(lock_name, "wb");
	if (f == NULL)
		return -1; /* could not create lock */
	fwrite(&mode, 1, sizeof(int), f);
	refcount = 1;
	fwrite(&refcount, 1, sizeof(int), f);
	fclose(f);
	return 0;
}

/* "enters" the pool of processes that are allowed to
use the locked file system at the same time. The refcount
is updated to reflect the new number of processes
accessing the lock simultaneously */
static int lock_enter(char* filename, int mode)
{
	int refcount, r;
	FILE* f;
	char lock_name[nfs_NAME_LEN];
	strcpy(lock_name, filename);
	strcpy(&lock_name[strlen(filename)], nfs_LOCK_SUFFIX);
	f = fopen(lock_name, "r+b");
	if (f == NULL)
		return lock_create(filename, mode);

	/* write access mode */
	fseek(f, 0, 0);
	fwrite(&mode, 1, sizeof(int), f);
	fflush(f);

	/* read refcount */
	fseek(f, sizeof(int), 0);
	r = fread(&refcount, 1, sizeof(int), f);
	if (r != sizeof(int))
		refcount = 0;

	/* increment */
	refcount++;

	/* write new refcount */
	fflush(f);
	fseek(f, sizeof(int), 0);
	fwrite(&refcount, 1, sizeof(int), f);

	fclose(f);
	return 0;
}

/* returns true (1) if the lock does not exist or
if accessing the FS is allowed. Returns false (0)
otherwise (if the system is locked and this process
cannot "enter" it) */
static int lock_check(char* filename, int mode)
{
	FILE* f;
	int mm, ret;
	char lock_name[nfs_NAME_LEN];

	strcpy(lock_name, filename);
	strcpy(&lock_name[strlen(filename)], nfs_LOCK_SUFFIX);
	f = fopen(lock_name, "rb");
	if (f == NULL)
		return 1; /* lock not found; access granted */

	/* read mode */
	fseek(f, 0, 0);
	fread(&mm, 1, sizeof(int), f);

	if (mm & FS_WRITE)
		ret = 0; /* no other processes are allowed when FS locked for writing */
	else if (mm & FS_READ)
		ret = (mode == FS_READ); /* only readers are allowed */
	else
		ret = 0; /* invalid mode */
	fclose(f);
	return ret;
}

/* removes a lock file */
static int lock_remove(char* filename)
{
	char lock_name[nfs_NAME_LEN];
	strcpy(lock_name, filename);
	strcpy(&lock_name[strlen(filename)], nfs_LOCK_SUFFIX);
	return remove(lock_name);
}

/* leave a locked system, and update the lock's refcount
to reflect the new number of processes accessing the
locked system simultaneously */
static int lock_leave(char* filename)
{
	FILE* f;
	int   mode, refcount = 0;
	char lock_name[nfs_NAME_LEN];

	strcpy(lock_name, filename);
	strcpy(&lock_name[strlen(filename)], nfs_LOCK_SUFFIX);

	/* open */
	f = fopen(lock_name, "r+b");
	if (f == NULL)
		return -1;

	/* read mode and refcount */
	fseek(f, 0, 0);
	fread(&mode, 1, sizeof(int), f);
	fread(&refcount, 1, sizeof(int), f);

	/* decrement */
	refcount--;

	/* rewrite refcount */
	fflush(f);
	fseek(f, sizeof(int), 0);
	fwrite(&refcount, 1, sizeof(int), f);

	/* close file */
	fclose(f);

	/* refcount <= 0 ? then remove lock */
	if (refcount <= 0)
		return lock_remove(filename);

	return 0;

}

/* Open if possible (otherwise create) the given file system, return handle */
nfs_Handle* nfs_start(char* filename, int flags)
{

	char iio_name[nfs_NAME_LEN];
	char data_name[nfs_NAME_LEN];
	nfs_Handle* fs = NULL;

	/* form the appropriate file names */
	strcpy(iio_name, filename);
	strcpy(&iio_name[strlen(filename)], nfs_IIO_SUFFIX);
	strcpy(data_name, filename);
	strcpy(&data_name[strlen(filename)], nfs_DATA_SUFFIX);

	/* allocate new file system handle */
	fs = (nfs_Handle*)malloc(sizeof(nfs_Handle));
	fs->ht_size  = 0;
	fs->handles  = 0;
	nfs_errno   = 0;
	fs->fls_open = 0;

	/* set the internal access mode flags */
	nfs_iio_IOMODE  = flags;
	nfs_data_IOMODE = flags;

	if (!lock_check(filename, flags))
	{
		/* access denied - FS locked */
		nfs_errno = nfs_ERR_LOCKED;
		free(fs);
		return nfs_NULL;
	}

	/* if the files exist, open them, otherwise create them */
	if (file_exists(iio_name) && file_exists(data_name))
	{
		/* the file system was previously created, now simply open it */

		/* open the IIO file */
		fs->file = nfs_iio_open(iio_name);
		if (fs->file == NULL)
		{
			nfs_errno = nfs_ERR_IIO_INIT;
			free(fs);
			return nfs_NULL;
		}

		/* open the data file */
		fs->data = nfs_data_open(data_name);
		if (fs->data == NULL)
		{
			nfs_errno = nfs_ERR_NO_DATA;
			free(fs);
			return nfs_NULL;
		}

		/* Initialize the DT */
		fs->dt = nfs_dt_open(fs->file, nfs_CHANNEL_DT1, nfs_CHANNEL_DT2);
		if (fs->dt == NULL)
		{
			nfs_errno = nfs_ERR_NO_DT;
			free(fs);
			return nfs_NULL;
		}

		/* Initialize the NT */
		fs->nt = nfs_nt_open(fs->file, nfs_CHANNEL_NT);
		if (fs->nt == NULL)
		{
			nfs_errno = nfs_ERR_NO_NT;
			free(fs);
			return nfs_NULL;
		}

		/* Initialize the FAT */
		fs->fat = nfs_fat_open(fs->file, nfs_CHANNEL_FAT);
		if (fs->fat == NULL)
		{
			nfs_errno = nfs_ERR_NO_FAT;
			free(fs);
			return nfs_NULL;
		}

	}
	else
	{
		/* The file system does not exist, create it */

		/* if the file system does not exist and it's supposed
		to be open for reading => error, the file(s) do(es)
		not exist. */
		if (flags == FS_READ)
		{
			nfs_errno = nfs_ERR_IIO_INIT;
			free(fs);
			return nfs_NULL;
		}

		/* create a new IIO file */
		fs->file = nfs_iio_create(iio_name);
		if (fs->file == NULL)
		{
			nfs_errno = nfs_ERR_IIO_INIT;
			free(fs);
			return nfs_NULL;
		}

		/* create a new data file */
		fs->data = nfs_data_create(data_name);
		if (fs->data == NULL)
		{
			nfs_errno = nfs_ERR_NO_DATA;
			free(fs);
			return nfs_NULL;
		}

		/* Create the DT */
		fs->dt = nfs_dt_create(fs->file, nfs_CHANNEL_DT1_SIZE, nfs_CHANNEL_DT2_SIZE);
		if (fs->dt == NULL)
		{
			nfs_errno = nfs_ERR_A_DT;
			free(fs);
			return nfs_NULL;
		}

		/* Create the NT */
		fs->nt = nfs_nt_create(fs->file, nfs_CHANNEL_NT_SIZE);
		if (fs->nt == NULL)
		{
			nfs_errno = nfs_ERR_A_NT;
			free(fs);
			return nfs_NULL;
		}

		/* Create the FAT */
		fs->fat = nfs_fat_create(fs->file, nfs_CHANNEL_FAT_SIZE);
		if (fs->fat == NULL)
		{
			nfs_errno = nfs_ERR_A_FAT;
			free(fs);
			return nfs_NULL;
		}

	}

	fs->filename = (char*)malloc(128);
	strcpy(fs->filename, filename);

	/* create or update the lock */
	//lock_enter(filename, flags);  //lock 파일을 생성한다..

	/* return the file system handle */
	return fs;
}

/* Close the given file system, remove (physically) if 'remove_fs' is true (1) */
void nfs_end(nfs_Handle* fs, int remove_fs)
{

	int i;

	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return;
	}

	if (remove_fs)
	{
		/* the file system has to be closed and removed physically */
		nfs_dt_destroy(fs->dt);
		nfs_nt_destroy(fs->nt);
		nfs_fat_close(fs->fat);
		nfs_data_destroy(fs->data);
		nfs_iio_destroy(fs->file);
	}
	else
	{
		/* just close it */
		nfs_dt_close(fs->dt);
		nfs_nt_close(fs->nt);
		nfs_fat_close(fs->fat);
		nfs_data_close(fs->data);
		nfs_iio_close(fs->file);
	}

	/* deallocate all file descriptors */
	if (fs->handles != NULL)
	{
		for (i = 0; i < fs->ht_size; i++)
			deallocate_file_descriptor(fs, i);
		free(fs->handles);
	}

	/* leaves the pool of processes accessing the lock simultaneously */
	lock_leave(fs->filename);

	if (fs->filename != NULL)
		free(fs->filename);

	/* deallocate the file system handle - invalid from this point on */
	free(fs);

}

/* Returns nonzero if the given file exists. Faster than opening the file for reading. */
int nfs_file_exists(nfs_Handle* fs, char* path)
{

	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return 0;
	}

	/* look for the file name in the DT */
	return (nfs_dt_filename_lookup(fs->dt, (char*)path) >= 0);

}

/* Open the given file. O_CREAT is ignored. */
int nfs_file_open(nfs_Handle* fs, const char* path, int oflag)
{

	int          filedes;
	nfs_FH*     fh;
	int          dt_idx;
	nfs_nt_Node ni;

	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}

	/* look for the file name in the DT, error if not found */
	dt_idx = nfs_dt_filename_lookup(fs->dt, (char*)path);
	if (dt_idx < 0)
	{
		nfs_errno = nfs_ERR_NOT_FOUND;
		return -1;
	}

	/* allocate a new (and empty) file descriptor */
	filedes = allocate_file_descriptor(fs);
	if (filedes < 0)
	{
		nfs_errno = nfs_ERR_NOMEM_OPEN;
		return -1;
	}

	/* increment the number of files open */
	fs->fls_open++;

	/* initialize descriptor */
	fh = fs->handles[filedes];
	fh->oflags   = oflag;   /* oflags */
	fh->dt_index = dt_idx;  /* index in DT */
	fh->nt_index = nfs_dt_filename_get_nt_index(fs->dt, dt_idx); /* index in NT */
	nfs_nt_get_node(fs->nt, fh->nt_index, &ni);
	fh->chain    = ni.chain; /* index in FAT */
	fh->fp       = 0;

	/* truncate file if requested */
	if ( ((oflag & O_RDWR) || (oflag & O_WRONLY)) &&
			(oflag & O_TRUNC) )
	{
		file_truncate(fs, fh, 0);
	}

	/* return the descriptor */
	return filedes;

}

/* Create a file with the given name. Disregards the value of 'mode'. */
int nfs_file_create(nfs_Handle* fs, const char* path, int)
{

	int          filedes, oflag;
	nfs_FH*     fh;
	int          dt_idx;
	nfs_nt_Node ni;
	int          already = 1;

	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}

	/* look for the file name in the DT, create a new one if not found */
	dt_idx = nfs_dt_filename_lookup(fs->dt, (char*)path);
	if (dt_idx < 0)
	{
		already = 0;
		dt_idx = nfs_dt_filename_add(fs->dt, (char*)path);
		if (dt_idx < 0)
		{
			nfs_errno = nfs_ERR_DT_INTERNAL;
			return -1;
		}
	}

	/* allocate a new (and empty) file descriptor */
	filedes = allocate_file_descriptor(fs);
	if (filedes < 0)
	{
		nfs_errno = nfs_ERR_NOMEM_OPEN;
		return -1;
	}

	/* increment the number of files open */
	fs->fls_open++;

	/* initialize descriptor */
	fh = fs->handles[filedes];
	fh->oflags   = O_CREAT | O_RDWR | O_TRUNC; /* default oflags for "creat" */
	oflag = fh->oflags;
	fh->dt_index = dt_idx;  /* index in DT */
	if (already)
		/* index of existing node in NT */
		fh->nt_index = nfs_dt_filename_get_nt_index(fs->dt, dt_idx);
	else
	{
		/* index of new node in NT */
		fh->nt_index = nfs_nt_allocate_node(fs->nt);
		nfs_dt_filename_set_nt_index(fs->dt, dt_idx, fh->nt_index); /* link */
	}
	nfs_nt_get_node(fs->nt, fh->nt_index, &ni);
	if (!already)
	{
		/* index of new chain head in FAT */
		ni.chain     = nfs_fat_create_chain(fs->fat);
		nfs_nt_set_node(fs->nt, fh->nt_index, &ni);
	}
	fh->chain    = ni.chain; /* index in FAT */
	fh->fp       = 0;

	/* truncate file if requested */
	if ( ((oflag & O_RDWR) || (oflag & O_WRONLY)) &&
			(oflag & O_TRUNC) &&
			already )
	{
		file_truncate(fs, fh, 0);
	}

	/* return the descriptor */
	return filedes;

}

/* Close the file given by its file descriptor */
int nfs_file_close(nfs_Handle* fs, int filedes)
{

	int ret;

	if (fs == nfs_NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}

	/* deallocate the file descriptor - this is enough */
	ret = deallocate_file_descriptor(fs, filedes);

	/* decrement the number of files open */
	if (ret == 0)
		fs->fls_open--;

	/* simulate the behavior of "close()" */
	if (ret == 0)
		return 0;

	return -1;

}

static int nblocks(int size)
{
	if (size == 0)
		return 1;
	return (((size - 1) / nfs_data_BLOCK_SIZE) + 1);
}

static int blockno(int fp)
{
	return (fp / nfs_data_BLOCK_SIZE);
}

/* Position the file pointer */
int nfs_file_lseek(nfs_Handle* fs, int filedes, int offset, int whence)
{

	int fp, size;

	/* dumb-proof */
	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}
	if (fs->handles == NULL)
	{
		nfs_errno = nfs_ERR_NO_OPEN;
		return -1;
	}
	if (fs->handles[filedes] == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FH;
		return -1;
	}

	/* get file size */
	size = nfs_nt_node_get_size(fs->nt, fs->handles[filedes]->nt_index);

	/* whence? */
	if (whence == (int)SEEK_SET)
	{
		/* relative to the beginning */
		fp = offset;
	}
	else if (whence == (int)SEEK_CUR)
	{
		/* relative to the current position */
		fp = fs->handles[filedes]->fp + offset;
	}
	else if (whence == (int)SEEK_END)
	{
		/* relative to the end of the file */
		fp = size + offset;
	}
	else
	{
		/* To avoid compiler warnings. If whence doesn't have a valid
		    value, use SEEK_SET. */
		fp = offset;
	}

	/* change the file pointer */
	fs->handles[filedes]->fp = fp;

	/* update the FAT chain and size (in NT) if necessary */
	if (blockno(fp) >= nblocks(size))
	{
		int node = fs->handles[filedes]->chain;
		int sz   = size;
		while (blockno(fp) >= nblocks(sz))
		{
			node = nfs_fat_chain_extend(fs->fat, node);
			sz  += nfs_data_BLOCK_SIZE;
		}
	}

	/* update the size (in NT), if necessary */
	if (size < fp)
		nfs_nt_node_set_size(fs->nt, fs->handles[filedes]->nt_index, (fp + 1));

	return 0;

}

/* Read data from the given file */
int nfs_file_read(nfs_Handle* fs, int filedes, void* buf, int nbyte)
{

	int fp, size;
	int startb, endb;
	int start_in_block, end_in_block;
	int node, idx;
	int sidx, eidx, kk, pp;

	/* dumb-proof */
	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}
	if (fs->handles == NULL)
	{
		nfs_errno = nfs_ERR_NO_OPEN;
		return -1;
	}
	if (fs->handles[filedes] == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FH;
		return -1;
	}

	if (nbyte == 0)
		return 0;

	/* get file size and pointer */
	fp   = fs->handles[filedes]->fp;
	size = nfs_nt_node_get_size(fs->nt, fs->handles[filedes]->nt_index);

	/* adjust nbyte if necessary - it may not be possible to read
	that many bytes, if the file pointer is approaching the
	end of file */
	if (fp + nbyte > size)
		nbyte = size - fp;

	if (nbyte <= 0)
		return 0;

	/* compute the range of nodes needed */
	startb         = (fp / nfs_data_BLOCK_SIZE);
	endb           = ((fp + nbyte - 1) / nfs_data_BLOCK_SIZE);
	start_in_block = (fp % nfs_data_BLOCK_SIZE);
	end_in_block   = ((fp + nbyte - 1) % nfs_data_BLOCK_SIZE);

	/* loop */
	node = nfs_fat_chain_get_nth(fs->fat, fs->handles[filedes]->chain, startb);
	idx  = startb;
	kk   = 0;
	while (idx <= endb)
	{
		/* sidx, eidx */
		if (idx == startb)
			sidx = start_in_block;
		else
			sidx = 0;
		if (idx == endb)
			eidx = end_in_block;
		else
			eidx = (nfs_data_BLOCK_SIZE - 1);
		/* read (conditional split for efficiency) */
		if ((sidx == 0) && (eidx == (nfs_data_BLOCK_SIZE - 1)))
			/* one full block, fast */
			pp = nfs_data_read(fs->data, node, (char*)buf + kk);
		else
		{
			char tmpbuf[nfs_data_BLOCK_SIZE];
			/* a portion of a block (slower) */
			pp = nfs_data_read(fs->data, node, tmpbuf);
			memcpy((char*)buf + kk, &tmpbuf[sidx], (eidx - sidx + 1));
		}
		/* next node in chain */
		node = nfs_fat_chain_get_nth(fs->fat, node, 1);
		idx++;
		if (pp == 0)
			kk += (eidx - sidx + 1);
	}

	/* update the file pointer with the number of bytes actually read */
	fs->handles[filedes]->fp = fp + nbyte;

	/* return the amount of data that was actually read */
	return nbyte;
}

/* Write data to the given file. If the file was open with
O_APPEND, any previous lseek() is ignored and the block
is appended to the end of the file */
int nfs_file_write(nfs_Handle* fs, int filedes, const void* buf, int nbyte)
{

	int fp, size;
	int startb, endb;
	int start_in_block, end_in_block;
	int node, idx;
	int sidx, eidx, kk;

	/* dumb-proof */
	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}
	if (fs->handles == NULL)
	{
		nfs_errno = nfs_ERR_NO_OPEN;
		return -1;
	}
	if (fs->handles[filedes] == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FH;
		return -1;
	}

	/* if nbyte is zero, "write()" returns immediately - so do we. */
	if (nbyte <= 0)
		return 0;

	/* get file size and pointer */
	fp   = fs->handles[filedes]->fp;
	size = nfs_nt_node_get_size(fs->nt, fs->handles[filedes]->nt_index);

	/* if the file was open for appending (O_APPEND), write() will
	ignore any previous lseek() - so do we */
	if ( (fs->handles[filedes]->oflags & O_APPEND) &&
			(fp < size) )
	{
		nfs_file_lseek(fs, filedes, size, (int)SEEK_SET);
		size = nfs_nt_node_get_size(fs->nt, fs->handles[filedes]->nt_index);
		fp   = fs->handles[filedes]->fp;
	}

	/* compute the range of nodes needed */
	startb         = (fp / nfs_data_BLOCK_SIZE);
	endb           = ((fp + nbyte - 1) / nfs_data_BLOCK_SIZE);
	start_in_block = (fp % nfs_data_BLOCK_SIZE);
	end_in_block   = ((fp + nbyte - 1) % nfs_data_BLOCK_SIZE);

	/* extend the FAT chain beforehand, as needed (faster this way) */
	nfs_file_lseek(fs, filedes, fp + nbyte - 1, (int)SEEK_SET);
	nfs_file_lseek(fs, filedes, fp,             (int)SEEK_SET);

	/* loop */
	node = nfs_fat_chain_get_nth(fs->fat, fs->handles[filedes]->chain, startb);
	idx  = startb;
	kk   = 0;
	while (idx <= endb)
	{
		/* sidx, eidx */
		if (idx == startb)
			sidx = start_in_block;
		else
			sidx = 0;
		if (idx == endb)
			eidx = end_in_block;
		else
			eidx = (nfs_data_BLOCK_SIZE - 1);
		/* write (conditional split for efficiency) */
		if ((sidx == 0) && (eidx == (nfs_data_BLOCK_SIZE - 1)))
			/* one full block, fast */
			nfs_data_write(fs->data, node, (char*)buf + kk);
		else
		{
			char tmpbuf[nfs_data_BLOCK_SIZE];
			/* a portion of a block (slower) */
			nfs_data_read(fs->data, node, tmpbuf);
			memcpy(&tmpbuf[sidx], (char*)buf + kk, (eidx - sidx + 1));
			nfs_data_write(fs->data, node, tmpbuf);
		}
		/* next node in chain */
		node = nfs_fat_chain_get_nth(fs->fat, node, 1);
		idx++;
		kk += (eidx - sidx + 1);
	}

	/* update the file pointer with the number of bytes actually written */
	nfs_file_lseek(fs, filedes, fp + nbyte - 1, (int)SEEK_SET);
	fs->handles[filedes]->fp = fp + nbyte;

	/* return the amount of data that was actually written */
	return nbyte;

}

/* Increment the given file's refcount */
int nfs_file_inc_refcount(nfs_Handle* fs, char* path)
{

	int dt_idx, nt_idx;

	/* dumb-proof */
	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}
	if (path == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_PARMS;
		return -1;
	}

	/* look for the file name in the DT, error if not found */
	dt_idx = nfs_dt_filename_lookup(fs->dt, (char*)path);
	if (dt_idx < 0)
	{
		nfs_errno = nfs_ERR_NOT_FOUND;
		return -1;
	}

	/* find index in NT */
	nt_idx = nfs_dt_filename_get_nt_index(fs->dt, dt_idx);
	if (nt_idx < 0)
	{
		nfs_errno = nfs_ERR_INVALID_NT;
		return -1;
	}

	/* increment refcount by 1 */
	nfs_nt_refcount_incr(fs->nt, nt_idx);

	/* success */
	return 0;

}

/* Decrement the given file's refcount, remove file if refcount drops below 1 */
int nfs_file_dec_refcount(nfs_Handle* fs, const char* path)
{

	int dt_idx, nt_idx, chain;
	int must_go, ret = 0;

	/* dumb-proof */
	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}
	if (path == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_PARMS;
		return -1;
	}

	/* look for the file name in the DT, error if not found */
	dt_idx = nfs_dt_filename_lookup(fs->dt, (char*)path);
	if (dt_idx < 0)
	{
		nfs_errno = nfs_ERR_NOT_FOUND;
		return -1;
	}

	/* find index in NT */
	nt_idx = nfs_dt_filename_get_nt_index(fs->dt, dt_idx);
	if (nt_idx < 0)
	{
		nfs_errno = nfs_ERR_INVALID_NT;
		return -1;
	}

	/* get the chain */
	chain = nfs_nt_node_get_chain(fs->nt, nt_idx);
	if (chain <= 0) /* node #0 is reserved */
		return -1;

	/* decrement refcount by 1, record result in 'must_go' */
	must_go = nfs_nt_refcount_decr(fs->nt, nt_idx);

	if (must_go)
	{
		/* the filename goes away */
		ret = nfs_dt_filename_delete(fs->dt, (char*)path);
		if (ret < 0)
			return ret;
	}

	/* if 'must_go' is true, the FAT chain has to be erased, too */
	if (must_go)
		ret = nfs_fat_destroy_chain(fs->fat, chain);

	return ret;

}

/* Create a hard link */
int nfs_file_link(nfs_Handle* fs, char* existing, const char* new_)
{

	int dt_idx, nt_idx;
	int dt_idx2;

	/* dumb-proof */
	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}

	/* look for the file name in the DT, error if not found */
	dt_idx = nfs_dt_filename_lookup(fs->dt, (char*)existing);
	if (dt_idx < 0)
	{
		nfs_errno = nfs_ERR_NOT_FOUND;
		return -1;
	}

	/* find index in NT */
	nt_idx = nfs_dt_filename_get_nt_index(fs->dt, dt_idx);
	if (nt_idx < 0)
	{
		nfs_errno = nfs_ERR_INVALID_NT;
		return -1;
	}

	/* increment refcount by 1 */
	nfs_nt_refcount_incr(fs->nt, nt_idx);

	/* create a new directory entry that points to the same node in NT */
	dt_idx2 = nfs_dt_filename_add(fs->dt, (char*)new_);
	if (dt_idx2 < 0)
	{
		nfs_errno = nfs_ERR_DUPLICATEF;
		return -1;
	}
	nfs_dt_filename_set_nt_index(fs->dt, dt_idx2, nt_idx);

	/* success */
	return 0;

}

/* Destroy a hard link or remove a file */
int nfs_file_unlink(nfs_Handle* fs, const char* path)
{

	int dt_idx, nt_idx, chain;
	int must_go, ret;

	/* dumb-proof */
	if (fs == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}

	/* look for the file name in the DT, error if not found */
	dt_idx = nfs_dt_filename_lookup(fs->dt, (char*)path);
	if (dt_idx < 0)
	{
		nfs_errno = nfs_ERR_NOT_FOUND;
		return -1;
	}

	/* find index in NT */
	nt_idx = nfs_dt_filename_get_nt_index(fs->dt, dt_idx);
	if (nt_idx < 0)
	{
		nfs_errno = nfs_ERR_INVALID_NT;
		return -1;
	}

	/* get the chain */
	chain = nfs_nt_node_get_chain(fs->nt, nt_idx);
	if (chain <= 0) /* node #0 is reserved */
		return -1;

	/* decrement refcount by 1, record result in 'must_go' */
	must_go = nfs_nt_refcount_decr(fs->nt, nt_idx);

	/* the filename goes away */
	ret = nfs_dt_filename_delete(fs->dt, (char*)path);
	if (ret < 0)
		return ret;

	/* if 'must_go' is true, the FAT chain has to be erased, too */
	if (must_go)
		ret = nfs_fat_destroy_chain(fs->fat, chain);

	return ret;

}

typedef struct _nfs_tmp
{
	nfs_glob_t* pglob;
	char*        pattern;
	int          flags;
	int          (*e)(const char*, int);
	int          errnum;
} nfs_tmp;

static int __nfs_glob_cbk(nfs_dt_DT*, char* filename, int, void* t)
{

	int n;
	nfs_tmp* tmp = (nfs_tmp*)t;
	int must_extend = 0;

	/* check if the array needs to be extended */
	if (!(tmp->pglob->gl_pathc & 0x1F))
		must_extend = 1;

	/* found one more match! */
	tmp->pglob->gl_pathc++;

	/* how many slots? */
	n = tmp->pglob->gl_pathc;
	if (tmp->flags & GLOB_DOOFFS)
		n += tmp->pglob->gl_offs;

	/* reallocate if necessary */
	if (must_extend)
	{
		tmp->pglob->gl_pathv = (char**)realloc(tmp->pglob->gl_pathv, ((n-1)+40) * sizeof(char*));
		if (tmp->pglob->gl_pathv == NULL)
		{
			tmp->pglob->gl_pathc--;
			if (tmp->e)
				tmp->e(filename, GLOB_NOSPACE);
			tmp->errnum = GLOB_NOSPACE; /* set the error code */
			return 0; /* stop */
		}
	}

	/* create a new filename in the next available slot */
	tmp->pglob->gl_pathv[n-1] = (char*)malloc(strlen(filename) + 4);
	strcpy(tmp->pglob->gl_pathv[n-1], filename);

	/* the first pointer after the last pathname must be NULL */
	tmp->pglob->gl_pathv[n] = NULL;

	if (tmp->flags & GLOB_FIRSTONLY)
		return 0; /* stop after the first one */

	return 1;
}

/* cbk for quicksort */
int __nfs_glob_sort(const void* e1, const void* e2)
{
	return strcmp(*(char**)e1, *(char**)e2);
}

/* Search for file names matching the given pattern */
int nfs_glob(nfs_Handle* fs,
			 const char* pattern,
			 int flags,
			 int (*errfunc)(const char*, int),
			 nfs_glob_t* pglob)
{

	nfs_tmp tmp;
	int      lastc;

	if (fs == nfs_NULL)
	{
		nfs_errno = nfs_ERR_INVALID_FS;
		return -1;
	}

	if ((pattern == NULL) || (pglob == NULL))
	{
		nfs_errno = nfs_ERR_INVALID_PARMS;
		return -1;
	}

	/* init temp structure */
	tmp.pglob   = pglob;
	tmp.pattern = (char*)pattern;
	tmp.flags   = flags;
	tmp.e       = errfunc;

	/* if GLOB_APPEND is not specified, reinitialize glob structure */
	if (!(flags & GLOB_APPEND))
	{
		pglob->gl_pathv = NULL;
		pglob->gl_pathc = 0;
	}

	/* use offset? */
	if (!(flags & GLOB_DOOFFS))
		pglob->gl_offs  = 0;

	/* save counter */
	lastc = pglob->gl_pathc;

	/* do it */
	nfs_dt_filename_glob(fs->dt, (char*)pattern, __FNM_FLAGS, __nfs_glob_cbk, &tmp);

	/* GLOB_NOCHECK? then return the pattern if no match */
	if ((flags & GLOB_NOCHECK) && (pglob->gl_pathc == lastc))
		__nfs_glob_cbk(fs->dt, (char*)pattern, 0, &tmp);

	/* sort if necessary */
	if (!(flags & GLOB_NOSORT))
	{
		if (pglob->gl_pathc > 0)
			qsort(&(pglob->gl_pathv[pglob->gl_offs]), pglob->gl_pathc, sizeof(char*), __nfs_glob_sort);
	}

	return 0;

}

/* Frees the memory allocated by a previous nfs_glob */
void nfs_glob_free(nfs_Handle*, nfs_glob_t* pglob)
{

	int i;

	if (pglob == NULL)
	{
		nfs_errno = nfs_ERR_INVALID_PARMS;
		return;
	}

	/* free the space allocated to the filenames */
	if (pglob->gl_pathv != NULL)
	{
		for (i = pglob->gl_offs; i < (pglob->gl_offs + pglob->gl_pathc); i++)
		{
			if (pglob->gl_pathv[i] != NULL)
				free(pglob->gl_pathv[i]);
		}
		free(pglob->gl_pathv);
	}

	pglob->gl_pathc = 0;
	pglob->gl_pathv = NULL;

}

/* Writes the textual error given by (nfs_errno) to the standard output */
void nfs_perror(nfs_Handle*, const char* s)
{

	char* e = "Internal error";

	switch ((char)nfs_errno)
	{
	case nfs_ERR_IIO_INIT:
		e = "Could not open/create the IIO file";
		break;
	case nfs_ERR_NO_DATA:
		e = "Could not open/create the data file";
		break;
	case nfs_ERR_NO_FAT:
		e = "Could not initialize the FAT channel";
		break;
	case nfs_ERR_NO_NT:
		e = "Could not initialize the NT channel";
		break;
	case nfs_ERR_NO_DT:
		e = "Could not initialize the DT channel";
		break;
	case nfs_ERR_A_FAT:
		e = "Could not allocate the FAT channel";
		break;
	case nfs_ERR_A_NT:
		e = "Could not allocate the NT channel";
		break;
	case nfs_ERR_A_DT:
		e = "Could not allocate the DT channel";
		break;
	case nfs_ERR_INVALID_FS:
		e = "Invalid file system handle";
		break;
	case nfs_ERR_ALREADY_CLSD:
		e = "File already closed";
		break;
	case nfs_ERR_NOT_FOUND:
		e = "File not found";
		break;
	case nfs_ERR_NO_OPEN:
		e = "No files open";
		break;
	case nfs_ERR_INVALID_FH:
		e = "Invalid file descriptor";
		break;
	case nfs_ERR_INVALID_NT:
		e = "Invalid node in NT";
		break;
	case nfs_ERR_NOMEM_OPEN:
		e = "No memory available for opening files";
		break;
	case nfs_ERR_LOCKED:
		e = "File system is locked by another process";
		break;
	case nfs_ERR_DUPLICATEF:
		e = "The file you are trying to create already exists";
		break;
	case nfs_ERR_DT_INTERNAL:
		e = "Internal error in DT";
		break;
	case nfs_ERR_INVALID_PARMS:
		e = "Invalid parameters";
		break;
	default:
		e = "Internal error";
	}

	printf("%s: %s\n", s, e);

}
