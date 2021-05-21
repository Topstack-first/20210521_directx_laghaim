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
// File:    nfs_data.c
// Date:    07/08/02
// Content: Data Block Infrastructure with 1-level Sliding-Window Cache
//          (simulates the behavior of a physical multi-sector disk)
//          All the I/O accesses are made via an internal cache (configurable).
//
//----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include "nfs_data.h"
//----------------------------------------------------------------------------
#pragma warning( disable : 4996)
//----------------------------------------------------------------------------
int nfs_data_IOMODE = 1;

/* create cache */
static int cache_create(nfs_data_Data* data)
{

	int size = nfs_data_DEF_CACHE_SIZE;

	if (data == NULL)
		return -1; /* invalid argument */

	/* avoid arbitrarily sized cache - round up to the nearest
	multiple of nfs_data_BLOCK_SIZE */
	if ((size % nfs_data_BLOCK_SIZE) != 0)
		size += (nfs_data_BLOCK_SIZE - (size % nfs_data_BLOCK_SIZE));

	/* allocate */
	data->cache = (nfs_data_Cache*)malloc(sizeof(nfs_data_Cache));
	if (data->cache == NULL)
		return -1; /* not enough memory */
	data->cache->buffer = (char*)malloc(size);
	if (data->cache->buffer == NULL)
		return -1; /* not enough memory */

	/* initialize */
	data->cache->size     = size;
	data->cache->sync     = nfs_data_SYNC;
	data->cache->position = 0;

	/* read a block of size 'size' */
	fseek(data->file, data->cache->position, 0);
	fread(data->cache->buffer, 1, data->cache->size, data->file);

	return 0;

}

/* resize the cache */
static int cache_resize(nfs_data_Data* data, int size)
{

	if (data == NULL)
		return -1; /* invalid argument */

	if (data->cache->sync != nfs_data_SYNC)
		return -1; /* cannot resize no-sync cache */

	/* avoid arbitrarily sized cache - round up to the nearest
	multiple of nfs_data_BLOCK_SIZE */
	if ((size % nfs_data_BLOCK_SIZE) != 0)
		size += (nfs_data_BLOCK_SIZE - (size % nfs_data_BLOCK_SIZE));
	if (size <= 0)
		size = nfs_data_BLOCK_SIZE; /* last resort */

	data->cache->buffer = (char*)realloc(data->cache->buffer, size);
	if (data->cache->buffer == NULL)
		return -1; /* not enough memory */

	/* initialize */
	data->cache->size     = size;
	data->cache->sync     = nfs_data_SYNC;

	/* read a block of size 'size' */
	fseek(data->file, data->cache->position, 0);
	fread(data->cache->buffer, 1, data->cache->size, data->file);

	return 0;

}

/* flush the cache */
static int cache_flush(nfs_data_Data* data)
{

	/* tests */
	if (data->cache == NULL)
		return -1; /* no cache */
	if (data->cache->sync == nfs_data_SYNC)
		return 0;  /* flush unnecessary */

	/* data->cache->sync is 0, which means that a process
	wrote some stuff in the cache and the cache hasn't
	been flushed */
	fseek(data->file, data->cache->position, 0);
	fwrite(data->cache->buffer, 1, data->cache->size, data->file);

	/* mark the cache as being in sync */
	data->cache->sync = nfs_data_SYNC;

	return 0;
}

/* destroys the cache */
static int cache_destroy(nfs_data_Data* data)
{

	/* flush the cache */
	cache_flush(data);

	/* deallocate it */
	if (data->cache->buffer != NULL)
		free(data->cache->buffer);
	if (data->cache != NULL)
		free(data->cache);
	data->cache = NULL; /* mark it */

	return 0;

}

/* slides the cache window */
static int cache_slide(nfs_data_Data* data, int new_position)
{

	/* is the cache is out of sync, flush it first */
	if (data->cache->sync != nfs_data_SYNC)
		cache_flush(data);

	/* adjust cache positions that are not block-aligned */
	if ((new_position % nfs_data_BLOCK_SIZE) != 0)
		new_position = (int)(new_position / nfs_data_BLOCK_SIZE) * nfs_data_BLOCK_SIZE;

	/* update cache based on the new absolute position */
	data->cache->position = new_position;
	fseek(data->file, data->cache->position, 0);
	fread(data->cache->buffer, 1, data->cache->size, data->file);

	return 0;

}

/* return nonzero (true) if a block is currently in cache */
static int is_in_cache(nfs_data_Data* data, int block)
{
	if ((data == NULL) || (data->cache == NULL))
		return 0;
	if ((block >= (data->cache->position / nfs_data_BLOCK_SIZE)) &&
			(block <  ((data->cache->position + data->cache->size) / nfs_data_BLOCK_SIZE)))
		return 1; /* yes! */
	return 0; /* gotta refresh... :( */
}

/* get 'how_much' bytes from the cached disk, write bytes to address referenced by 'to' */
static int cache_get(nfs_data_Data* data, int abspos, int how_much, void* to)
{
	int i;

	for (i = 0; i < how_much; i += nfs_data_BLOCK_SIZE)
	{
		/* refresh cache only if necessary */
		if (!is_in_cache(data, ((abspos + i) / nfs_data_BLOCK_SIZE)))
			cache_slide(data, (int)((abspos + i) / nfs_data_BLOCK_SIZE) * nfs_data_BLOCK_SIZE);
		/* get one block of data from cache */
		memcpy((char*)to + i,
			   (void*)((char*)data->cache->buffer + (abspos - data->cache->position + i)),
			   nfs_data_BLOCK_SIZE);
	}

	return 0;

}

/* write 'how_much' bytes from 'from' to the cached disk, at absolute position 'abspos' */
static int cache_put(nfs_data_Data* data, int abspos, int how_much, void* from)
{
	int i;

	for (i = 0; i < how_much; i += nfs_data_BLOCK_SIZE)
	{
		/* refresh cache only if necessary */
		if (!is_in_cache(data, ((abspos + i) / nfs_data_BLOCK_SIZE)))
			cache_slide(data, (int)((abspos + i) / nfs_data_BLOCK_SIZE) * nfs_data_BLOCK_SIZE);
		/* get one block of data from cache */
		memcpy((void*)((char*)data->cache->buffer + (abspos - data->cache->position + i)),
			   (char*)from + i,
			   nfs_data_BLOCK_SIZE);
		/* something has been dumped into the cache, so set the unsync flag */
		data->cache->sync = nfs_data_NOT_SYNC; /* needs flushing */
	}

	return 0;

}







/* Create a new virtual disk */
nfs_data_Data* nfs_data_create(char* filename)
{
	nfs_data_Data*  data;

	/* allocate a new data handle corresponding to the virtual disk */
	data = (nfs_data_Data*)malloc(sizeof(nfs_data_Data));
	if (data == NULL)
		return NULL; /* not enough memory */

	/* create the file */
	data->filename = (char*)malloc(strlen(filename) + 8);
	if (!data->filename)
	{
		free(data);
		return NULL;
	}
	strcpy(data->filename, filename);
	data->file = fopen(filename, "w+b");
	if (data->file == NULL)
	{
		free(data);
		return NULL; /* could not create file - permission problem */
	}

	/* create the cache. data->file must be initialized. */
	if (cache_create(data) < 0)
	{
		free(data);
		return NULL; /* could not create cache */
	}

	/* return the handle */
	return data;
}

/* Open an existing virtual disk */
nfs_data_Data* nfs_data_open(char* filename)
{

	nfs_data_Data*  data;

	/* allocate a new data handle corresponding to the virtual disk */
	data = (nfs_data_Data*)malloc(sizeof(nfs_data_Data));
	if (data == NULL)
		return NULL; /* not enough memory */

	/* open the file */
	data->filename = (char*)malloc(strlen(filename) + 8);
	if (!data->filename)
	{
		free(data);
		return NULL;
	}
	strcpy(data->filename, filename);
	if (nfs_data_IOMODE & 2) /* write */
		data->file = fopen(filename, "r+b");
	else
		data->file = fopen(filename, "rb");

	if (data->file == NULL)
	{
		free(data);
		return NULL; /* could not open file for R/W */
	}

	/* create the cache. data->file must be initialized. */
	if (cache_create(data) < 0)
	{
		free(data);
		return NULL; /* could not create cache */
	}

	/* return the handle */
	return data;

}

/* Close and destroy (remove) the given virtual disk */
void nfs_data_destroy(nfs_data_Data* data)
{
	char* tempfn = (char*)malloc(strlen(data->filename) + 8);
	if (tempfn)
		strcpy(tempfn, data->filename);
	/* close and destroy */
	nfs_data_close(data);
	/* remove file */
#ifdef WIN32
	if (tempfn)
		remove(tempfn);
#else
	if (tempfn)
		unlink(tempfn);
#endif
	if (tempfn)
		free(tempfn);
}

/* Close (but do not destroy) the given virtual disk */
void nfs_data_close(nfs_data_Data* data)
{

	/* close the cache */
	cache_destroy(data);

	/* destroy the handle */
	if (data != NULL)
	{
		/* close the file */
		fflush(data->file);
		fclose(data->file);

		if(data->filename != NULL) // 메모리 할당된 파일 이름 삭제
		{
			free(data->filename);
			data->filename = NULL;
		}

		free(data); /* no longer valid */
	}
}

/* Set the size of the internal cache. The function fails if the cache is not in sync with the disk */
int nfs_data_set_cache_size(nfs_data_Data* data, int cache_size)
{
	nfs_data_flush_cache(data);
	return cache_resize(data, cache_size);
}

/* Flush the cache */
int nfs_data_flush_cache(nfs_data_Data* data)
{
	if (data->cache->sync != nfs_data_SYNC)
		cache_flush(data);
	return 0;
}

/* Write one block of data from 'from' to absolute block number 'block_no' */
int nfs_data_write(nfs_data_Data* data, int block_no, void* from)
{
	if (block_no < 0)
		return -1;
	return cache_put(data, block_no * nfs_data_BLOCK_SIZE, nfs_data_BLOCK_SIZE, from);
}

/* Read one block of data from block number 'block_no' into the memory referenced by 'to' */
int nfs_data_read(nfs_data_Data* data, int block_no, void* to)
{
	if (block_no < 0)
		return -1;
	return cache_get(data, block_no * nfs_data_BLOCK_SIZE, nfs_data_BLOCK_SIZE, to);
}

/* Write 'n' contiguous blocks starting at 'block_start' from memory at 'from' */
int nfs_data_write_contiguous(nfs_data_Data* data, int block_start, int n, void* from)
{
	if (block_start < 0)
		return -1;
	return cache_put(data, block_start * nfs_data_BLOCK_SIZE, n * nfs_data_BLOCK_SIZE, from);
}

/* Read 'n' contiguous blocks starting from 'block_start' into the memory referenced by 'to' */
int nfs_data_read_contiguous(nfs_data_Data* data, int block_start, int n, void* to)
{
	if (block_start < 0)
		return -1;
	return cache_get(data, block_start * nfs_data_BLOCK_SIZE, n * nfs_data_BLOCK_SIZE, to);
}

