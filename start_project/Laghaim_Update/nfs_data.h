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
// File:    nfs_data.h
// Date:    07/08/02
// Content: Data Block Infrastructure with 1-level Sliding-Window Cache
//          (simulates the behavior of a physical multi-sector disk)
//          All the I/O accesses are made via an internal cache (configurable).
//
//----------------------------------------------------------------------------

#ifndef _nfs_data_
#define _nfs_data_

#include <stdio.h>
#include <string.h>

/* Constants */

#define nfs_data_BLOCK_SIZE      512                         /* Basic data block: 1K */
#define nfs_data_DEF_CACHE_SIZE  (128 * nfs_data_BLOCK_SIZE) /* Default cache size: 128 blocks = 64K */
#define nfs_data_SYNC            1                           /* Constant for cache's sync state */
#define nfs_data_NOT_SYNC        0                           /* Constant for cache's unsync state */

extern int nfs_data_IOMODE;

/* Data structures */

typedef struct _nfs_data_Cache {
    int   size;             /* cache size */
    char* buffer;           /* buffer, see size above */
    int   position;         /* start position in the file */
    int   sync;             /* in sync? 1=true, 0=false(needs flushing) */
} nfs_data_Cache;

typedef struct _nfs_data_Data {
    FILE*            file;     /* file where the data (virtual disk) is stored */
    char*            filename; /* name of data file */
    nfs_data_Cache*  cache;    /* cache for this virtual disk */
} nfs_data_Data;

/* Exported functions, argh I miss C++ */

/* Create a new virtual disk */
nfs_data_Data*  nfs_data_create(char* filename);

/* Open an existing virtual disk */
nfs_data_Data*  nfs_data_open  (char* filename);

/* Close and destroy (remove) the given virtual disk */
void            nfs_data_destroy(nfs_data_Data* data);

/* Close (but do not destroy) the given virtual disk */
void            nfs_data_close(nfs_data_Data* data);

/* Set the size of the internal cache. The function fails if the cache is not in sync with the disk */
int             nfs_data_set_cache_size(nfs_data_Data* data, int cache_size);

/* Flush the cache */
int             nfs_data_flush_cache(nfs_data_Data* data);

/* Write one block of data from 'from' to absolute block number 'block_no' */
int             nfs_data_write(nfs_data_Data* data, int block_no, void* from);

/* Read one block of data from block number 'block_no' into the memory referenced by 'to' */
int             nfs_data_read(nfs_data_Data* data, int block_no, void* to);

/* Write 'n' contiguous blocks starting at 'block_start' from memory at 'from' */
int             nfs_data_write_contiguous(nfs_data_Data* data, int block_start, int n, void* from);

/* Read 'n' contiguous blocks starting from 'block_start' into the memory referenced by 'to' */
int             nfs_data_read_contiguous(nfs_data_Data* data, int block_start, int n, void* to);

#endif

