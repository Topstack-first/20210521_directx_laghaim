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
// File:    nfs_nt.c
// Date:    07/08/02
// Content: Node Table (NT)
//
//----------------------------------------------------------------------------


#include <stdlib.h>
#include "nfs_iio.h"
#include "nfs_nt.h"

/* --- INTERNAL ---------------------------------------------------------- */

static int node_get(nfs_nt_NT* nt, int node, nfs_nt_Node* node_info)
{
	nfs_iio_seek(nt->file, nt->channel, node * sizeof(nfs_nt_Node));
	return nfs_iio_read(nt->file, nt->channel, node_info, sizeof(nfs_nt_Node));
}

static int node_set(nfs_nt_NT* nt, int node, nfs_nt_Node* node_info)
{
	nfs_iio_seek(nt->file, nt->channel, node * sizeof(nfs_nt_Node));
	return nfs_iio_write(nt->file, nt->channel, node_info, sizeof(nfs_nt_Node));
}

static int node_recover(nfs_nt_NT* nt, int node)
{
	nfs_nt_Node ni;

#ifdef PURIFY
	/* Initialize ni to avoid UMR/UMC in purify. */
	memset(&ni, 0, sizeof(ni));
#endif /* PURIFY */

	/* init an empty node */
	ni.refcount = 0;
	ni.size     = 0;
	ni.chain    = 0;

	/* delete node */
	node_set(nt, node, &ni);

	/* update chain */
	if (node < nt->unallocated)
		nt->unallocated = node;

	return 0;

}

static int find_first_free(nfs_nt_NT* nt, int start)
{

	nfs_nt_Node ni;
	int node = start;

	/* look for the first free one */
	node_get(nt, node, &ni);
	while (ni.refcount > 0)
		node_get(nt, ++node, &ni);

	/* return the node */
	return node;

}

/* --- EXPORTED ---------------------------------------------------------- */


/* Create a new NT in the given IIO file, using the given nr. of blocks per channel */
nfs_nt_NT* nfs_nt_create(nfs_iio_File* file, int blocks_per_channel)
{

	nfs_nt_NT* nt;
	int         chn;

	if (file == NULL)
		return NULL;

	if (blocks_per_channel == 0)
		blocks_per_channel = nfs_nt_DEF_CHANNEL_SIZE;

	/* allocate new channel */
	chn = nfs_iio_allocate_channel(file, blocks_per_channel);

	/* allocate new NT handle and initialize it */
	nt = (nfs_nt_NT*)malloc(sizeof(nfs_nt_NT));
	nt->file        = file;
	nt->channel     = chn;
	nt->unallocated = 0;

	/* return the handle */
	return nt;

}

/* Open the NT stored in channel 'channel' of the given IIO file */
nfs_nt_NT* nfs_nt_open(nfs_iio_File* file, int channel)
{

	nfs_nt_NT* nt;

	/* allocate new NT handle and initialize it */
	nt = (nfs_nt_NT*)malloc(sizeof(nfs_nt_NT));
	nt->file        = file;
	nt->channel     = channel;
	nt->unallocated = find_first_free(nt, 0);

	/* return the handle */
	return nt;

}

/* Close the NT */
void nfs_nt_close(nfs_nt_NT* nt)
{
	if (nt != NULL)
		free(nt);
}

/* Destroy the NT */
void nfs_nt_destroy(nfs_nt_NT* nt)
{
	nfs_nt_close(nt);
	/* remove ... */
	/* ... */
}

/* Get the information in node 'node' */
int nfs_nt_get_node(nfs_nt_NT* nt, int node, nfs_nt_Node* node_ptr)
{
	if (nt == NULL)
		return -1;
	return node_get(nt, node, node_ptr);
}

/* Allocate a new node, return the index */
int nfs_nt_allocate_node(nfs_nt_NT* nt)
{

	int          node;
	nfs_nt_Node ni;

	if (nt == NULL)
		return -1;

	/* get head of free chain */
	node = nt->unallocated;

	/* initialize the node */
	nfs_nt_get_node(nt, node, &ni);
	ni.refcount = 1;
	ni.size     = 0;
	ni.chain    = 0;
	nfs_nt_set_node(nt, node, &ni);

	/* update the free chain */
	nt->unallocated = find_first_free(nt, nt->unallocated);

	/* return the node index */
	return node;
}

/* Decrement refcount, if refcount=0 delete node and ret true, otherwise return false */
int nfs_nt_refcount_decr(nfs_nt_NT* nt, int node)
{

	nfs_nt_Node ni;
	int          must_delete = 0;

	/* decrement refcount */
	nfs_nt_get_node(nt, node, &ni);
	ni.refcount--;
	if (ni.refcount <= 0)
		must_delete = 1;
	nfs_nt_set_node(nt, node, &ni);

	/* delete node if necessary */
	if (must_delete)
		node_recover(nt, node);

	/* pass to caller */
	return must_delete;

}

/* Increment refcount */
void nfs_nt_refcount_incr(nfs_nt_NT* nt, int node)
{
	nfs_nt_Node ni;
	nfs_nt_get_node(nt, node, &ni);
	ni.refcount++;
	nfs_nt_set_node(nt, node, &ni);
}

/* Update the given node with the information from the given nfs_nt_Node */
int nfs_nt_set_node(nfs_nt_NT* nt, int node, nfs_nt_Node* node_info)
{
	return node_set(nt, node, node_info);
}

/* Get the size of the data associated with a node */
int nfs_nt_node_get_size(nfs_nt_NT* nt, int node)
{
	nfs_nt_Node ni;
	nfs_nt_get_node(nt, node, &ni);
	return ni.size;
}

/* Set the new size of data associated with a node */
void nfs_nt_node_set_size(nfs_nt_NT* nt, int node, int size)
{
	nfs_nt_Node ni;
	nfs_nt_get_node(nt, node, &ni);
	ni.size = size;
	nfs_nt_set_node(nt, node, &ni);
}

/* Get the chain associated with a node */
int nfs_nt_node_get_chain(nfs_nt_NT* nt, int node)
{
	nfs_nt_Node ni;
	nfs_nt_get_node(nt, node, &ni);
	return ni.chain;
}

/* Set the chain associated with a node */
void nfs_nt_node_set_chain(nfs_nt_NT* nt, int node, int chain)
{
	nfs_nt_Node ni;
	nfs_nt_get_node(nt, node, &ni);
	ni.chain = chain;
	nfs_nt_set_node(nt, node, &ni);
}
