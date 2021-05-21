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
// File:    nfs.h
// Date:    07/08/02
// Content: Main module
//
//----------------------------------------------------------------------------

#ifndef _nfs_
#define _nfs_


#include <string.h>
#include "nfs_iio.h"                 /* Interlaced I/O */
#include "nfs_pmatch.h"              /* Pattern matching */
#include "nfs_dt.h"                  /* Directory Table */
#include "nfs_nt.h"                  /* Node Table */
#include "nfs_fat.h"                 /* File Allocation Table */
#include "nfs_data.h"                /* Data Block I/O (virtual disk) */

extern int nfs_errno;

/* Definitions */

#define nfs_NULL               NULL
#define nfs_NAME_LEN           256
#define nfs_IIO_SUFFIX         ".lrs"
#define nfs_DATA_SUFFIX        ".lrf"
#define nfs_LOCK_SUFFIX        ".lock"
#define nfs_CHANNEL_DT1        0      /* channel for Directory Table (Trie Nodes) */
#define nfs_CHANNEL_DT2        1      /* channel for Directory Table (Key Nodes) */
#define nfs_CHANNEL_NT         2      /* channel for Node Table */
#define nfs_CHANNEL_FAT        3      /* channel for File Allocation Table */

#define nfs_CHANNEL_DT1_SIZE   4      /* 8k blocks/dt_channel(trie) */
#define nfs_CHANNEL_DT2_SIZE   8      /* 8k blocks/dt_channel(keys) */
#define nfs_CHANNEL_NT_SIZE    1      /* 8k blocks/nt_channel */
#define nfs_CHANNEL_FAT_SIZE   4      /* 8k blocks/fat_channel */

#define nfs_INITIAL_HT_SIZE    32     /* 32 slots available by default, will auto-increase as needed */

#define FS_READ                1
#define FS_WRITE               2
#define FS_RW                  (FS_READ | FS_WRITE)

#ifndef GLOB_ERR

#define	GLOB_ERR	       (1 << 0)  /* Return on read errors.  */
#define	GLOB_MARK	       (1 << 1)  /* Append a slash to each name.  */
#define	GLOB_NOSORT	       (1 << 2)  /* Don't sort the names.  */
#define	GLOB_DOOFFS	       (1 << 3)  /* Insert PGLOB->gl_offs NULLs.  */
#define	GLOB_DOOFS	       GLOB_DOOFFS
#define	GLOB_NOCHECK       (1 << 4)  /* If nothing matches, return the pattern.  */
#define	GLOB_APPEND	       (1 << 5)  /* Append to results of a previous call.  */
#define	GLOB_NOESCAPE      (1 << 6)  /* Backslashes don't quote metacharacters.  */
#define	GLOB_PERIOD	       (1 << 7)  /* Leading `.' can be matched by metachars.  */
#define GLOB_MAGCHAR       (1 << 8)  /* Set in gl_flags if any metachars seen.  */
#define GLOB_ALTDIRFUNC    (1 << 9)  /* Use gl_opendir et al functions.  */
#define GLOB_BRACE         (1 << 10) /* Expand "{a,b}" to "a" "b".  */
#define GLOB_NOMAGIC       (1 << 11) /* If no magic chars, return the pattern.  */
#define GLOB_TILDE	       (1 << 12) /* Expand ~user and ~ to home directories. */
#define GLOB_ONLYDIR       (1 << 13) /* Match only directories.  */
#define GLOB_TILDE_CHECK   (1 << 14) /* Like GLOB_TILDE but return an error
if the user name is not available.  */
#define GLOB_FIRSTONLY     (1 << 15) /* Stop after the first match. */

#define __GLOB_FLAGS       (GLOB_ERR|GLOB_MARK|GLOB_NOSORT|GLOB_DOOFFS| \
    GLOB_NOESCAPE|GLOB_NOCHECK|GLOB_APPEND|     \
    GLOB_PERIOD|GLOB_ALTDIRFUNC|GLOB_BRACE|     \
GLOB_NOMAGIC|GLOB_TILDE|GLOB_ONLYDIR|GLOB_TILDE_CHECK)

/* Error returns from `glob'.  */
#define	GLOB_NOSPACE       1	 /* Ran out of memory.  */
#define	GLOB_ABORTED       2	 /* Read error.  */
#define	GLOB_NOMATCH       3	 /* No matches found.  */
#define GLOB_NOSYS         4	 /* Not implemented.  */

/* GNU compatibility */
#define GLOB_ABEND         GLOB_ABORTED

#endif

/* Errors */

#define nfs_ERR_IIO_INIT      1      /* Could not open/create the IIO file  */
#define nfs_ERR_NO_DATA       2      /* Could not open/create the data file */
#define nfs_ERR_NO_FAT        3      /* Could not initialize the FAT channel */
#define nfs_ERR_NO_NT         4      /* Could not initialize the NT channel */
#define nfs_ERR_NO_DT         5      /* Could not initialize the DT channel */
#define nfs_ERR_A_FAT         6      /* Could not allocate the FAT channel */
#define nfs_ERR_A_NT          7      /* Could not allocate the NT channel */
#define nfs_ERR_A_DT          8      /* Could not allocate the DT channel */
#define nfs_ERR_INVALID_FS    9      /* Invalid file system handle */
#define nfs_ERR_ALREADY_CLSD  10     /* File already closed */
#define nfs_ERR_NOT_FOUND     11     /* File not found */
#define nfs_ERR_NO_OPEN       12     /* No files open */
#define nfs_ERR_INVALID_FH    13     /* Invalid file descriptor */
#define nfs_ERR_INVALID_NT    14     /* Invalid node in NT */
#define nfs_ERR_NOMEM_OPEN    15     /* No memory available for opening files */
#define nfs_ERR_LOCKED        16     /* File system is locked by another process */
#define nfs_ERR_DUPLICATEF    17     /* File already exists */
#define nfs_ERR_DT_INTERNAL   18     /* Internal error in DT */
#define nfs_ERR_INVALID_PARMS 19     /* Invalid parameters */

/* Typedefs */

/* Internal file handle */
typedef struct _nfs_FH
{
	int dt_index;                /* index in Directory Table */
	int nt_index;                /* index in Node Table */
	int chain;                   /* index in FAT (chain start) */
	int fp;                      /* current file pointer */
	int oflags;                  /* current oflags (specified when the file is open) */
} nfs_FH;

/* File system handle */
typedef struct _nfs_Handle
{
	nfs_iio_File*      file;     /* interlaced i/o file */
	nfs_dt_DT*         dt;       /* directory table */
	nfs_nt_NT*         nt;       /* node table */
	nfs_fat_FAT*       fat;      /* file allocation table */
	nfs_data_Data*     data;     /* virtual disk */
	nfs_FH**           handles;  /* handles for open files */
	int                ht_size;  /* size of handle table (in entries) */
	int                fls_open; /* nr of open files */
	int                mode;     /* mode - FS_READ, FS_WRITE, or both */
	char*              filename;
} nfs_Handle;

typedef struct _nfs_glob_t
{
	int    gl_pathc;    /* Count of paths matched so far  */
	char** gl_pathv;    /* List of matched pathnames.  */
	int    gl_offs;     /* Slots to reserve in `gl_pathv'.  */
} nfs_glob_t;

/* Exported - prototypes */

/* Returns nonzero (true) if the given file system is available, false (0) if not */
int               nfs_exists(char* filename);

/* Open if possible (otherwise create) the given file system, return handle */
nfs_Handle*       nfs_start(char* filename, int flags);

/* Close the given file system, remove (physically) if 'remove_fs' is true (1) */
void              nfs_end(nfs_Handle* fs, int remove_fs);

/* Returns nonzero if the given file exists. Faster than opening the file for reading. */
int               nfs_file_exists(nfs_Handle* fs, char* path);

/* Open the given file. Ignores any O_CREAT flag in 'oflag'. */
int               nfs_file_open(nfs_Handle* fs, const char* path, int oflag);

/* Create a file with the given name. Disregards the value of 'mode'. */
int               nfs_file_create(nfs_Handle* fs, const char* path, int mode);

/* Close the file given by its file descriptor */
int               nfs_file_close(nfs_Handle* fs, int filedes);

/* Position the file pointer */
int               nfs_file_lseek(nfs_Handle* fs, int filedes, int offset, int whence);

/* Read data from the given file */
int               nfs_file_read(nfs_Handle* fs, int filedes, void* buf, int nbyte);

/* Write data to the given file */
int               nfs_file_write(nfs_Handle* fs, int filedes, const void* buf, int nbyte);

/* Create a hard link */
int               nfs_file_link(nfs_Handle* fs, char* existing, const char* New);

/* Destroy a hard link or remove a file */
int               nfs_file_unlink(nfs_Handle* fs, const char* path);

/* Increment the given file's refcount */
int               nfs_file_inc_refcount(nfs_Handle* fs, char* path);

/* Decrement the given file's refcount, remove file if refcount drops below 1 */
int               nfs_file_dec_refcount(nfs_Handle* fs, const char* path);

/* Search for file names matching the given pattern */
int               nfs_glob(nfs_Handle* fs,
						   const char* pattern,
						   int flags,
						   int (*errfunc)(const char*, int),
						   nfs_glob_t* pglob);

/* Frees the memory allocated by a previous nfs_glob */
void              nfs_glob_free(nfs_Handle* fs, nfs_glob_t* pglob);

/* Writes the textual error given by (fs->errno) to the standard output */
void              nfs_perror(nfs_Handle* fs, const char* s);

#endif


