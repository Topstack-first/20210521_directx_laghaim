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
// File:    nfs_fat.c
// Date:    07/08/02
// Content: File Allocation Table (FAT)
//
//----------------------------------------------------------------------------

#include <stdlib.h>
#include "nfs_iio.h"
#include "nfs_fat.h"


/* ---- INTERNAL ------------------------------------------------------ */


/* sets the value of the node given by 'node_idx' */
static void node_set_value(nfs_fat_FAT* fat, int node_idx, int value)
{
	int v = value;
	nfs_iio_seek(fat->file, fat->channel, node_idx * sizeof(int));
	nfs_iio_write(fat->file, fat->channel, &v, sizeof(int));
}

/* gets the value of the node given by 'node_idx' */
static int node_get_value(nfs_fat_FAT* fat, int node_idx)
{
	int value;
	nfs_iio_seek(fat->file, fat->channel, node_idx * sizeof(int));
	nfs_iio_read(fat->file, fat->channel, &value, sizeof(int));
	return value;
}

/* recovers the given node (makes it available) */
static int node_recover(nfs_fat_FAT* fat, int node_idx)
{
	/* set node to 0 */
	if (node_idx > 0)
		node_set_value(fat, node_idx, 0);
	/* update the 'unallocated' pointer. Notice that
	node #0 is never used (thus recovered), since
	an index of 0 has a special meaning. */
	if ((node_idx < fat->unallocated) && (node_idx > 0))
		fat->unallocated = node_idx;
	return 1; /* must return 1 (true) */
}

/* returns the index of the first free node, starting from 'start'. */
static int next_free(nfs_fat_FAT* fat, int start)
{

	int node = start;
	/* find the first non-zero */
	while ((node <= 0) || (node_get_value(fat, node) != 0))
		node++;

	/* this is the non-allocated cell */
	return node;

}

/* returns the index of the last node in the chain (value = -1) */
static int find_last_in_chain(nfs_fat_FAT* fat, int start)
{

	int node      = start;
	int last_node;

	/* iterate until the end of the chain */
	do
	{
		last_node = node;
		node = node_get_value(fat, node);
	}
	while (node != nfs_fat_EOC);

	/* return last non-EOC node found */
	return last_node;

}



/* ---- EXPORTED ----------------------------------------------------- */



/* Creates a FAT using a new channel in the given IIO file */
nfs_fat_FAT* nfs_fat_create(nfs_iio_File* file, int blocks_per_channel)
{

	int           chn;
	nfs_fat_FAT* fat;

	if (file == NULL)
		return NULL;

	if (blocks_per_channel == 0)
		blocks_per_channel = nfs_fat_DEF_CHANNEL_SIZE;

	/* allocate new channel */
	chn = nfs_iio_allocate_channel(file, blocks_per_channel);

	/* allocate new fat handle and initialize it */
	fat = (nfs_fat_FAT*)malloc(sizeof(nfs_fat_FAT));
	fat->file        = file;
	fat->channel     = chn;
	fat->unallocated = 1;

	/* return the handle */
	return fat;

}

/* Opens an existing FAT and initializes it with the information found in the given channel */
nfs_fat_FAT* nfs_fat_open(nfs_iio_File* file, int channel_no)
{

	nfs_fat_FAT* fat;

	if (file == NULL)
		return NULL;

	/* create and initialize a new fat handle */
	fat = (nfs_fat_FAT*)malloc(sizeof(nfs_fat_FAT));
	fat->channel     = channel_no;
	fat->file        = file;
	fat->unallocated = next_free(fat, 0);

	/* return the handle */
	return fat;

}

/* Closes the given FAT */
int nfs_fat_close (nfs_fat_FAT* fat)
{
	if (fat != NULL)
		free(fat);
	return 0;
}

/* Allocates a new 1-node chain and returns the index of the first (only) node in the chain */
int nfs_fat_create_chain(nfs_fat_FAT* fat)
{

	int node;

	if ((fat == NULL) || (fat->file == NULL))
		return -1; /* oops */

	/* initializes the node with the index of the available node
	(fat->unallocated), and update fat->unallocated */
	node = fat->unallocated;
	node_set_value(fat, node, nfs_fat_EOC);
	fat->unallocated = next_free(fat, fat->unallocated);

	/* return the node */
	return node;

}

/* Destroys the chain given by the index of the first node in the chain */
int nfs_fat_destroy_chain(nfs_fat_FAT* fat, int first_node_index)
{
	if ((fat == NULL) || (fat->file == NULL) || (first_node_index <= 0))
		return -1; /* oops */
	nfs_fat_chain_for_each(fat, first_node_index, node_recover);
	return 0;
}

/* Truncates the chain starting at the given node */
int nfs_fat_chain_truncate(nfs_fat_FAT* fat, int first)
{

	int following;

	if ((fat == NULL) || (fat->file == NULL))
		return -1;

	/* get the following node */
	following = node_get_value(fat, first);

	/* truncate at this point */
	node_set_value(fat, first, nfs_fat_EOC);

	if (following != nfs_fat_EOC)
		/* destroy the chain that starts at 'following' */
		nfs_fat_destroy_chain(fat, following);

	return 0;

}

/* Allocates a new node for the given chain and appends it at the end. Returns the node index. */
int nfs_fat_chain_extend(nfs_fat_FAT* fat, int first_node_index)
{

	int node, last_node;

	if ((fat == NULL) || (fat->file == NULL))
		return -1; /* oops */

	/* get a free node */
	node = fat->unallocated;
	node_set_value(fat, node, nfs_fat_EOC);
	fat->unallocated = next_free(fat, fat->unallocated);

	/* find last node and link it to this one */
	last_node = find_last_in_chain(fat, first_node_index);
	node_set_value(fat, last_node, node);

	/* return the last allocated node */
	return node;

}

/* Removes the last 'last_n' nodes at the end of the chain */
int nfs_fat_chain_shrink(nfs_fat_FAT* fat, int first_node_index, int last_n)
{

	int node      = first_node_index;
	int prev_node = nfs_fat_EOC;
	int nth   = 0;
	int nth2  = 0;

	if ((fat == NULL) || (fat->file == NULL))
		return -1; /* oops */

	/* iterate until EOC is encountered, count the number of nodes */
	while (node != nfs_fat_EOC)
	{
		node = node_get_value(fat, node);
		nth++;
	}

	/* find the index of node (nth - last_n) */
	node = first_node_index;
	nth2 = 0;
	while (nth2 < (nth - last_n))
	{
		prev_node = node;
		node = node_get_value(fat, node);
		nth2++;
	}

	/* destroy the subchain starting at 'node' */
	nfs_fat_destroy_chain(fat, node);

	/* make the last remaining node the end of the chain */
	if (prev_node != nfs_fat_EOC)
		node_set_value(fat, prev_node, nfs_fat_EOC);

	return 0;

}

/* Gets all the nodes following a given node (specified by its index), including the node itself */
void nfs_fat_chain_get(nfs_fat_FAT* fat, int first_node_index, void* buf)
{

	int node = first_node_index;
	int nth  = 0;

	if ((fat == NULL) || (fat->file == NULL))
		return; /* oops */

	/* iterate until EOC is encountered */
	while (node != nfs_fat_EOC)
	{
		*((int*)buf + nth) = node;
		node = node_get_value(fat, node);
		nth++;
	}

}

/* Gets the following 'n'-1 nodes following the given start node (including the start = n) */
void nfs_fat_chain_get_first_n(nfs_fat_FAT* fat, int first_node_index, int n, void* buf)
{

	int node = first_node_index;
	int nth  = 0;

	if ((fat == NULL) || (fat->file == NULL))
		return; /* oops */

	/* iterate until n-th is found or EOC is encountered */
	while ( (nth <= (n-1)) && (node != nfs_fat_EOC) )
	{
		*((int*)buf + nth) = node;
		node = node_get_value(fat, node);
		nth++;
	}

}

/* Get the index of the 'n'-th node in the chain, given the start node */
/* the "0th" node is always the start node */
int nfs_fat_chain_get_nth(nfs_fat_FAT* fat, int first_node_index, int n)
{

	int node = first_node_index;
	int nth  = 0;

	if ((fat == NULL) || (fat->file == NULL))
		return -1; /* oops */

	/* iterate until n-th is found or EOC is encountered */
	while ( (nth < n) && (node != nfs_fat_EOC) )
	{
		node = node_get_value(fat, node);
		nth++;
	}

	/* return the node */
	return node;

}

/* Calls a user function for each node in the chain */
void nfs_fat_chain_for_each(nfs_fat_FAT* fat, int first_node_index, int (*p)(nfs_fat_FAT*, int))
{

	int node  = first_node_index;
	int node2 = node;
	int quit  = 0;

	if ((fat == NULL) || (fat->file == NULL))
		return; /* oops */

	/* iterate until end of chain (-1) is encountered */
	do
	{
		node2 = node_get_value(fat, node);
		if (!p(fat, node))
			quit = 1;
		else
		{
			if (node <= 0)
				quit = 1;
			node = node2;
		}
	}
	while ( !quit && (node != nfs_fat_EOC) );

}


