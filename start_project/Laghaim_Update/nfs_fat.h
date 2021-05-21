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
// File:    nfs_fat.h
// Date:    07/08/02
// Content: File Allocation Table (FAT)
//
//----------------------------------------------------------------------------

#ifndef _nfs_fat_
#define _nfs_fat_

#include "nfs_iio.h"

/* Constants */
#define nfs_fat_DEF_CHANNEL_SIZE    4       /* Default channel size in blocks ("bandwidth") */
#define nfs_fat_EOC                 -1      /* End of chain marker */

/* Type definitions */

typedef struct _nfs_fat_FAT {
    nfs_iio_File*   file;                /* IIO file */
    int             channel;             /* alloted channel in IIO file */
    int             unallocated;         /* offs of first entry in the chain of unallocated entries */
} nfs_fat_FAT;

/* Exported functions */

/* Creates a FAT using a new channel in the given IIO file */
nfs_fat_FAT*  nfs_fat_create(nfs_iio_File* file, int blocks_per_channel);

/* Opens an existing FAT and initializes it with the information found in the given channel */
nfs_fat_FAT*  nfs_fat_open  (nfs_iio_File* file, int channel_no);

/* Closes the given FAT */
int           nfs_fat_close (nfs_fat_FAT* fat);

/* Allocates a new 1-node chain and returns the index of the first (only) node in the chain */ 
int           nfs_fat_create_chain(nfs_fat_FAT* fat);

/* Destroys the chain given by the index of the first node in the chain */
int           nfs_fat_destroy_chain(nfs_fat_FAT* fat, int first);

/* Allocates a new node for the given chain and appends it at the end. Returns the node index. */
int           nfs_fat_chain_extend(nfs_fat_FAT* fat, int first);

/* Removes the last 'last_n' nodes at the end of the chain */
int           nfs_fat_chain_shrink(nfs_fat_FAT* fat, int first, int last_n);

/* Truncates the chain starting at the given node */
int           nfs_fat_chain_truncate(nfs_fat_FAT* fat, int first);

/* Gets all the nodes following a given node (specified by its index), including the node itself */
void          nfs_fat_chain_get(nfs_fat_FAT* fat, int first, void* buf);

/* Gets the following 'n'-1 nodes following the given start node (including the start = n) */
void          nfs_fat_chain_get_first_n(nfs_fat_FAT* fat, int first, int n, void* buf);

/* Get the index of the 'n'-th node in the chain, given the start node */
int           nfs_fat_chain_get_nth(nfs_fat_FAT* fat, int first, int n);

/* Calls a user function for each node in the chain */
void          nfs_fat_chain_for_each(nfs_fat_FAT* fat, int first, int (*p)(nfs_fat_FAT* f, int s));

#endif

