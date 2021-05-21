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
// File:    nfs_nt.h
// Date:    07/08/02
// Content: Node Table (NT)
//
//----------------------------------------------------------------------------

#ifndef _nfs_nt_
#define _nfs_nt_

#include "nfs_iio.h"

/* Definitions */

#define nfs_nt_DEF_CHANNEL_SIZE    1 /* default channel size, in blocks */

/* Type definitions */

typedef struct _nfs_nt_Node {
    int       refcount;                 /* ref-count, used for hard links */
    int       size;                     /* data size */
    int       chain;                    /* index of first node in FAT chain */
    int       unused;                   /* unused (padding for alignment) */
} nfs_nt_Node;

typedef struct _nfs_nt_NT {
    nfs_iio_File*  file;                /* IIO file */
    int            channel;             /* channel within the IIO file */
    int            unallocated;         /* index of first node in free chain */
} nfs_nt_NT;

/* Prototypes */

/* Create a new NT in the given IIO file, using the given nr. of blocks per channel */
nfs_nt_NT*    nfs_nt_create(nfs_iio_File* file, int blocks_per_channel);

/* Open the NT stored in channel 'channel' of the given IIO file */
nfs_nt_NT*    nfs_nt_open(nfs_iio_File* file, int channel);

/* Close the NT */
void          nfs_nt_close(nfs_nt_NT* nt);

/* Destroy the NT */
void          nfs_nt_destroy(nfs_nt_NT* nt);

/* Allocate a new node, return the index */
int           nfs_nt_allocate_node(nfs_nt_NT* nt);

/* Get the information in node 'node' */
int           nfs_nt_get_node(nfs_nt_NT* nt, int node, nfs_nt_Node* node_ptr);

/* Set the given node's information using the given nfs_nt_Node */
int           nfs_nt_set_node(nfs_nt_NT* nt, int node, nfs_nt_Node* node_info);

/* Decrement refcount, if refcount=0 delete node and ret true, otherwise return false */
int           nfs_nt_refcount_decr(nfs_nt_NT* nt, int node);

/* Increment refcount */
void          nfs_nt_refcount_incr(nfs_nt_NT* nt, int node);

/* Get the size of the data associated with a node */
int           nfs_nt_node_get_size(nfs_nt_NT* nt, int node);

/* Set the new size of data associated with a node */
void          nfs_nt_node_set_size(nfs_nt_NT* nt, int node, int size);

/* Get the chain associated with a node */
int           nfs_nt_node_get_chain(nfs_nt_NT* nt, int node);

/* Set the chain associated with a node */
void          nfs_nt_node_set_chain(nfs_nt_NT* nt, int node, int chain);

#endif

