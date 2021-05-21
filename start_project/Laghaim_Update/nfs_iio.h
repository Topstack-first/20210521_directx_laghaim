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
// File:    nfs_iio.h
// Date:    07/08/02
// Content: Interlaced I/O module (IIO)
//
//----------------------------------------------------------------------------

#ifndef _nfs_iio_
#define _nfs_iio_

#include <stdio.h>

/* Constants */

#define nfs_iio_BLOCK_SIZE         256       /* default: size of a block */
#define nfs_iio_CC_DEF_TABSIZE     1024      /* default: initial size of the cache */
#define nfs_iio_CC_MAX_ACTIVE_PGS  8         /* maximum number of active pages */
#define nfs_iio_CC_SYNC_MASK       1         /* mask for sync flag */
#define nfs_iio_CC_BUF_MASK        (~nfs_iio_CC_SYNC_MASK) /* mask for actual page buffer */

/* optimization flags */
#define nfs_iio_CC_RESTRICT_GROWTH           /* if defined, will restrict the growth of the cache */
#define nfs_iio_CC_MEMSET_OPT                /* tries to avoid useless memsets */

/* cache page switching policies */
#define ___LRU___                   1         /* least recently used */
#define ___LFU___                   2         /* least frequently used */
#define ___NLN___                   3         /* next lower neighbor */
#define nfs_iio_CC_POLICY           ___LRU___ /* policy for page switching */

extern int nfs_iio_IOMODE;

/* Data structures */

typedef struct _nfs_iio_Cache_Page {
    char* buffer;                   /* lowest 1 bit: sync flag; rest: cache buffer for this page */
    int   position;                 /* position within channel */
#if (nfs_iio_CC_POLICY == ___LRU___)
    int   timestamp;
#endif
#if (nfs_iio_CC_POLICY == ___LFU___)
    int   hits;
#endif
} nfs_iio_Cache_Page;

typedef struct _nfs_iio_Cache {
    nfs_iio_Cache_Page** pages;    /* cache pages */
    int                   nr_pages; /* number of pages currently in the cache */
    int                   active_pages; /* number of active pages */
} nfs_iio_Cache;

typedef struct _nfs_iio_Channel {
    int blocks;                     /* number of blocks in this channel */
    int size;                       /* bytes currently in this channel */
    int cp;                         /* channel pointer for this channel */
    nfs_iio_Cache* cache;           /* channel cache */
} nfs_iio_Channel;

typedef struct _nfs_iio_File {
    FILE* file;                     /* interlaced I/O file ptr */
    char* filename;                 /* name of .iio file */
    short channels;                 /* number of channels */
    nfs_iio_Channel** chn;          /* array of channels */
} nfs_iio_File;

/* Prototypes */

/* Create an Interlaced I/O file */
nfs_iio_File*        nfs_iio_create(char* filename);

/* Close and remove an IIO file */
void                 nfs_iio_destroy(nfs_iio_File* file);

/* Open existing IIO file */
nfs_iio_File*        nfs_iio_open(char* filename);

/* Close an IIO file, do not remove it */
void                 nfs_iio_close(nfs_iio_File* file);

/* Allocate a new channel for the given IIO file, with a certain number of blocks per channel */
int                  nfs_iio_allocate_channel(nfs_iio_File* file, int blocks);

/* Write 'size' bytes to the given channel from 'data', return nr. of bytes written */
int                  nfs_iio_write(nfs_iio_File* file, int channel, void* data, int size);

/* Read 'size' bytes from the given channel into 'data', return nr. of bytes read */
int                  nfs_iio_read (nfs_iio_File* file, int channel, void* data, int size);

/* Positions the channel pointer to a certain position in the given channel */
int                  nfs_iio_seek(nfs_iio_File* file, int channel, int where);

/* Returns the size of a chunk (in blocks) for the given IIO file */
int                  nfs_iio_blocks_per_chunk(nfs_iio_File* file);

/* Returns the channel number 'N' from the given IIO file */
nfs_iio_Channel*     nfs_iio_get_channel(nfs_iio_File* file, int channel);

/* Returns the current size of a given channel (in bytes) */
int                  nfs_iio_channel_size(nfs_iio_Channel* channel);

/* Returns the size (in blocks) of the given channel */
int                  nfs_iio_channel_blocks(nfs_iio_Channel* channel);

/* Truncates a channel at the current channel position */
int                  nfs_iio_channel_truncate(nfs_iio_File* file, int channel);

#endif


