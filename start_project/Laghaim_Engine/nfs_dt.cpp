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
// File:    nfs_dt.c
// Date:    07/08/02
// Content: Directory Table (DT)
//
//----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include "nfs_iio.h"
#include "nfs_pmatch.h"
#include "nfs_dt.h"
//----------------------------------------------------------------------------
#pragma warning( disable : 4996)
//---- INTERNAL --------------------------------------------------------------
typedef struct _nfs_dt_TrieNode
{
	unsigned short nt_index;               /* index in NT */
	short          bindex;                 /* bit index (for Patricia) */
	int            kindex;                 /* index in Key table (channel 1), and avail bit (msb) */
	int            left;                   /* index of left child */
	int            right;                  /* index of right child */
} nfs_dt_TrieNode;

#define nfs_dt_MAX_SKN_SIZE       60

#ifdef  nfs_dt_MAX_SUBNAME_SIZE
#undef  nfs_dt_MAX_SUBNAME_SIZE
#define nfs_dt_MAX_SUBNAME_SIZE   4096
#endif

typedef struct _nfs_dt_KeyNode
{
	int  next;                             /* next plus availability bit (msb) */
	char name [nfs_dt_MAX_SKN_SIZE];       /* (partial) name */
} nfs_dt_KeyNode;

/* ----------------------------------------------------------------------- */
/* the left child or the right child (indices in the DT) are >= 0. An
index of 0 means the first node in the table. Therefore we cannot
have "null" pointers (missing children). This is actually
great, since the Patricia trie does not use null pointers, either. */

static char* find_prefix(char* s, char* buffer)
{
	int idx = 0;
	while ( (s[idx] != 0)   &&
			(s[idx] != '*') &&
			(s[idx] != '?') &&
			(s[idx] != '[') )
	{
		buffer[idx] = s[idx];
		idx++;
	}
	buffer[idx] = 0; /* asciiz */
	return buffer;
}

/* ----------------------------------------------------------------------- */
/* get a Trie node */
static int trienode_get(nfs_dt_DT* dt, int node, nfs_dt_TrieNode* n)
{
	nfs_iio_seek(dt->file, dt->channel1, node * sizeof(nfs_dt_TrieNode));
	return nfs_iio_read(dt->file, dt->channel1, n, sizeof(nfs_dt_TrieNode));
}

/* ----------------------------------------------------------------------- */
/* get a key node (one node might contain only a partial key!) */
static int keynode_get(nfs_dt_DT* dt, int node, nfs_dt_KeyNode* n)
{
	nfs_iio_seek(dt->file, dt->channel2, node * sizeof(nfs_dt_KeyNode));
	return nfs_iio_read(dt->file, dt->channel2, n, sizeof(nfs_dt_KeyNode));
}

/* ----------------------------------------------------------------------- */

/* get the 'nt_index' field of the given Trie node */
static int trienode_get_nt(nfs_dt_DT* dt, int node)
{
	int r;
	unsigned short kk;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode));
	r = nfs_iio_read(dt->file, dt->channel1, &kk, sizeof(unsigned short));
	return ((r < sizeof(unsigned short)) ? 0 : (int)kk);
}

/* ----------------------------------------------------------------------- */
/* get the 'left' field of the given Trie node */
static int trienode_get_left(nfs_dt_DT* dt, int node)
{
	int kk, r;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset  of 'left' in the structure */
				 (2 * sizeof(int)) /* nt_idx, bindex, kindex */
				);
	r = nfs_iio_read(dt->file, dt->channel1, &kk, sizeof(int));
	return ((r < sizeof(int)) ? 0 : kk);
}

/* ----------------------------------------------------------------------- */
/* get the 'right' field of the given Trie node */
static int trienode_get_right(nfs_dt_DT* dt, int node)
{
	int kk, r;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset  of 'right' in the structure */
				 (3 * sizeof(int))       /* nt_index + bindex + kindex + left */
				);
	r = nfs_iio_read(dt->file, dt->channel1, &kk, sizeof(int));
	return ((r < sizeof(int)) ? 0 : kk);
}

/* ----------------------------------------------------------------------- */
/* get the 'bindex' field of the given Trie node */
static int trienode_get_bindex(nfs_dt_DT* dt, int node)
{
	short kk, r;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset  of 'bindex' in the structure */
				 (sizeof(short)               /* nt_index */
				 ));
	r = nfs_iio_read(dt->file, dt->channel1, &kk, sizeof(short));
	return ((r < sizeof(short)) ? 0 : ((int)kk));
}

/* ----------------------------------------------------------------------- */
/* get the 'kindex' field of the given Trie node */
static int trienode_get_kindex(nfs_dt_DT* dt, int node)
{
	int kk, r;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset of 'kindex' in the structure */
				 (sizeof(int)               /* nt_index + bindex */
				 ));
	r = nfs_iio_read(dt->file, dt->channel1, &kk, sizeof(int));
	return ((r < sizeof(int)) ? 0 : (kk & 0x7fffffff));
}

/* ----------------------------------------------------------------------- */
/* return 1 if the Trie node is free, 0 otherwise */
static int trienode_is_free(nfs_dt_DT* dt, int node)
{
	nfs_dt_TrieNode ni;
	int k;
	if (node == 0)
		return 0; /* no! #0 is reserved! */
	k = trienode_get(dt, node, &ni);
	if (k < 0)
		return 0;
	return ((ni.kindex & 0x80000000) == 0);
}

/* ----------------------------------------------------------------------- */
/* return 1 if the given Key node is free, 0 otherwise */
static int keynode_is_free(nfs_dt_DT* dt, int node)
{
	nfs_dt_KeyNode ni;
	int k;

	if (node == 0)
		return 0; /* no! #0 is reserved! */

	k = keynode_get(dt, node, &ni);
	if (k < 0)
		return 0;
	return ((ni.next & 0x80000000) == 0);

}

/* ----------------------------------------------------------------------- */
/* set the value of the given Trie node */
static int trienode_set(nfs_dt_DT* dt, int node, nfs_dt_TrieNode* n)
{
	nfs_iio_seek(dt->file, dt->channel1, node * sizeof(nfs_dt_TrieNode));
	return nfs_iio_write(dt->file, dt->channel1, n, sizeof(nfs_dt_TrieNode));
}

/* ----------------------------------------------------------------------- */
/* set the value of the given key node */
static int keynode_set(nfs_dt_DT* dt, int node, nfs_dt_KeyNode* n)
{
	nfs_iio_seek(dt->file, dt->channel2, node * sizeof(nfs_dt_KeyNode));
	return nfs_iio_write(dt->file, dt->channel2, n, sizeof(nfs_dt_KeyNode));
}

/* ----------------------------------------------------------------------- */
/* set the 'nt_index' field of the given Trie node */
static int trienode_set_nt(nfs_dt_DT* dt, int node, int nt)
{
	unsigned short kk = (unsigned short)nt;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode));
	return nfs_iio_write(dt->file, dt->channel1, &kk, sizeof(unsigned short));
}

/* ----------------------------------------------------------------------- */
/* set the 'left' field of the given Trie node */
static int trienode_set_left(nfs_dt_DT* dt, int node, int left)
{
	int kk = left;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset  of 'left' in the structure */
				 (sizeof(int) +              /* nt_index (+bindex) */
				  sizeof(int)                /* bindex or kindex */
				 ));
	return nfs_iio_write(dt->file, dt->channel1, &kk, sizeof(int));
}

/* ----------------------------------------------------------------------- */
/* set the 'right' field of the given Trie node */
static int trienode_set_right(nfs_dt_DT* dt, int node, int right)
{
	int kk = right;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset  of 'right' in the structure */
				 (sizeof(int) +              /* nt_index (+bindex) */
				  sizeof(int) +              /* bindex or kindex */
				  sizeof(int)                /* left */
				 ));
	return nfs_iio_write(dt->file, dt->channel1, &kk, sizeof(int));
}

/* ----------------------------------------------------------------------- */
/* set the 'bindex' field of the given Trie node */
#ifdef UNUSED
static int trienode_set_bindex(nfs_dt_DT* dt, int node, int bindex)
{
	short kk = (short)bindex;
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset  of 'bindex' in the structure */
				 (sizeof(unsigned short)                /* nt_index */
				 ));
	return nfs_iio_write(dt->file, dt->channel1, &kk, sizeof(short));
}
#endif /* UNUSED */

/* ----------------------------------------------------------------------- */
/* set the 'kindex' field of the given Trie node */
#ifdef UNUSED
static int trienode_set_kindex(nfs_dt_DT* dt, int node, int kindex)
{
	int kk = kindex | 0x80000000; /* automatically allocated */
	nfs_iio_seek(dt->file, dt->channel1,
				 node * sizeof(nfs_dt_TrieNode) +
				 /* offset  of 'bindex' in the structure */
				 (sizeof(int)                /* nt_index + bindex */
				 ));
	return nfs_iio_write(dt->file, dt->channel1, &kk, sizeof(int));
}
#endif /* UNUSED */

/* ----------------------------------------------------------------------- */
/* clear the node */
static int trienode_clear(nfs_dt_DT* dt, int node)
{
	nfs_dt_TrieNode ni;
	ni.nt_index = 0;
	ni.bindex   = 0;
	ni.kindex   = 0;
	ni.left     = node;
	ni.right    = node;
	return trienode_set(dt, node, &ni);
}

/* ----------------------------------------------------------------------- */
static int keynode_clear(nfs_dt_DT* dt, int node)
{
	nfs_dt_KeyNode ni;
	memset(&ni, 0, sizeof(ni));
	ni.next     = 0;
	ni.name[0]  = 0;
	return keynode_set(dt, node, &ni);
}

/* ----------------------------------------------------------------------- */
/* deallocates the chain of keynodes given by the keynode index "start" */
static int fnode_free(nfs_dt_DT* dt, int start)
{
	int             next = start;
	int             k;
	nfs_dt_KeyNode kn;

	while (next != 0)
	{
		/* get the next block in chain */
		k = next;
		keynode_get(dt, k, &kn);
		next = kn.next & 0x7fffffff;
		/* clear this one */
		keynode_clear(dt, k);
		if (k < dt->unallocated2)
			dt->unallocated2 = k;
	}

	return 0;

}

/* ----------------------------------------------------------------------- */

static int keynode_find_first_free(nfs_dt_DT* dt, int start);

/* allocates a new chain of keynodes using the given key.
Returns the index of the first keynode in chain. */
static int fnode_allocate(nfs_dt_DT* dt, char* key)
{

	int keynodes[1024]; /* 60*1024 = 61240 bytes / filename (max) */
	int nr_keynodes;
	int klen, i;
	nfs_dt_KeyNode kn;

	klen = strlen(key);
	nr_keynodes = 1 + (klen / nfs_dt_MAX_SKN_SIZE);

	/* preallocate */
	i = 0;
	while (i < nr_keynodes)
	{
		keynodes[i] = dt->unallocated2;
		dt->unallocated2 = keynode_find_first_free(dt, dt->unallocated2 + 1);
		i++;
	}

	/* fill in the actual info */
	for (i = 0; i < nr_keynodes; i++)
	{
		/* wire properly */
		if (i == (nr_keynodes - 1))
			kn.next = 0x80000000;
		else
			kn.next = 0x80000000 | keynodes[i+1];
		/* partial key */
		strncpy(kn.name, (char*)key + i * nfs_dt_MAX_SKN_SIZE, nfs_dt_MAX_SKN_SIZE);
		/* write keynode */
		keynode_set(dt, keynodes[i], &kn);
	}

	/* return first in chain */
	return keynodes[0];

}

/* ----------------------------------------------------------------------- */
/* copy the key and the nt_index from one node to another */
static int node_copy_key(nfs_dt_DT* dt, int from, int to)
{

	nfs_dt_TrieNode tn1, tn2;
	int old_kn2;

	trienode_get (dt, from, &tn1);
	trienode_get (dt, to,   &tn2);
	old_kn2 = tn2.kindex & 0x7fffffff;

	/* rewire from's key to 'to' */
	tn2.nt_index = tn1.nt_index;
	tn2.kindex   = tn1.kindex; /* this is the actual rewiring */

	/* save "to" */
	trienode_set(dt, to, &tn2);

	/* the old key can now be deleted */
	fnode_free(dt, old_kn2);

	return 0;
}

/* ----------------------------------------------------------------------- */
/* extract the key from the given fnode */
static int fnode_extract_key(nfs_dt_DT* dt, int node, char* key)
{

	register int i, j;
	int k, next;
	nfs_dt_KeyNode kn;

	i = 0;
	next = node;
	while (next > 0)
	{
		/* get keynode */
		k = next;
		keynode_get(dt, k, &kn);
		/* copy contents to key - hope this will be
		properly optimized by the compiler... */
		j = 0;
		while ((j < nfs_dt_MAX_SKN_SIZE) && (kn.name[j] != 0))
			key[i++] = kn.name[j++];
		/* next node */
		next = (kn.next & 0x7fffffff);
	}

	/* asciiz */
	key[i] = 0;

	return 0;
}

/* ----------------------------------------------------------------------- */
static int trienode_find_first_free(nfs_dt_DT* dt, int start)
{
	int index = (start - 1);
	do
	{
		++index;
	}
	while (!trienode_is_free(dt, index) || (index <= 0));
	return index;
}

/* ----------------------------------------------------------------------- */
static int keynode_find_first_free(nfs_dt_DT* dt, int start)
{
	int index = (start - 1);
	do
	{
		++index;
	}
	while (!keynode_is_free(dt, index) || (index <= 0));
	return index;
}

/* ----------------------------------------------------------------------- */
/* recover a node, update the chain */
#ifdef UNUSED
static int node_recover(nfs_dt_DT* dt, int node_idx)
{

	int kidx;

	/* get kindex from the given trie node */
	kidx = trienode_get_kindex(dt, node_idx);

	/* clear */
	trienode_clear(dt, node_idx);
	keynode_clear(dt, kidx);

	/* update chains */
	if (node_idx < dt->unallocated)
		dt->unallocated = node_idx;

	if (kidx < dt->unallocated2)
		dt->unallocated2 = kidx;

	return 0;
}
#endif /* UNUSED */

/* ----------------------------------------------------------------------- */
/* recover a trienode */
static int trienode_recover(nfs_dt_DT* dt, int node_idx)
{

	/* clear */
	trienode_clear(dt, node_idx);

	/* update chains */
	if (node_idx < dt->unallocated)
		dt->unallocated = node_idx;

	return 0;
}

/* ----------------------------------------------------------------------- */
/* returns the index of the trie node, not the key node! */
static int node_allocate(nfs_dt_DT* dt, char* filename, int nt_index, int bindex)
{

	int keynode, trienode;
	nfs_dt_TrieNode tn;

	/* first, allocate (and fill in) the keynode chain (fnode) */
	keynode = fnode_allocate(dt, filename);

	/* second, allocate (and fill in) a new trienode */
	trienode = dt->unallocated;
	tn.nt_index = nt_index;
	tn.bindex   = bindex;
	tn.kindex   = keynode | 0x80000000;
	tn.left     = 0;
	tn.right    = 0;
	trienode_set(dt, trienode, &tn);

	/* next in chain */
	dt->unallocated = trienode_find_first_free(dt, dt->unallocated);

	/* return the trie node */
	return trienode;

}

/* ----------------------------------------------------------------------- */
/* ----- BIT STREAMS (all streams end with '\0') */
/* bit with index (-1) is always "2", for all streams */

static int bit_get(char* bit_stream, int n)
{
	int k = (n & 0x7);
	if (n < 0) return 2;
	return ( (*(bit_stream + (n >> 3))) >> k) & 0x1;
}

static int bit_first_different(char* bit_stream1, char* bit_stream2)
{
	int n = 0;
	int d = 0;
	while ( (bit_stream1[n] == bit_stream2[n]) &&
			(bit_stream1[n] != 0) &&
			(bit_stream2[n] != 0) )
		n++;
	while (bit_get(&bit_stream1[n], d) == bit_get(&bit_stream2[n], d))
		d++;
	return ((n << 3) + d);
}

/* ----------------------------------------------------------------------- */
/* ----- PATRICIA STRUCTURE */
/* the first node in the channel (#0) will always be reserved for internal
information (the root of the Patricia structure). It cannot be allocated
or deallocated. */

static int p_init_head(nfs_dt_DT* dt)
{

	nfs_dt_TrieNode tn;
	nfs_dt_KeyNode  kn;

	/* init keynode */
	kn.next     = 0x80000000;
	kn.name[0]  = '.';
	kn.name[1]  = 0;

	/* init trienode */
	tn.nt_index = 0;
	tn.bindex   = -1;
	tn.kindex   = 0x80000000; /* root */
	tn.left     = 0;
	tn.right    = 0;

	keynode_set(dt, 0, &kn);
	return trienode_set(dt, 0, &tn);
}

/* ----------------------------------------------------------------------- */
static int p_get_head(nfs_dt_DT* dt)
{
	return 0; /* node at index 0 is always the head (root) of the Patricia tree */
}

/* ----------------------------------------------------------------------- */
static int p_compare_keys(nfs_dt_DT* dt, char* key, int node)
{
	nfs_dt_TrieNode tn;
	nfs_dt_KeyNode kn;
	int i, j, s;

	/* key length */
	s = strlen(key);

	/* get trie node */
	trienode_get(dt, node, &tn);

	/* compare the keys (in 60-byte chunks) */
	i = 0;
	j = (tn.kindex & 0x7fffffff);
	while (i < s)
	{
		keynode_get(dt, j, &kn);
		if (strncmp((char*)key + i, kn.name, nfs_dt_MAX_SKN_SIZE) != 0)
			return 0; /* not equal */
		j = kn.next & 0x7fffffff;
		i += nfs_dt_MAX_SKN_SIZE;
	}

	return 1;

}

/* ----------------------------------------------------------------------- */
static int p_find_first_different_bit(nfs_dt_DT* dt, char* key, int node)
{
	nfs_dt_TrieNode tn;
	char tempname[4096];

	/* get the trie node */
	trienode_get(dt, node, &tn);

	/* extract the full key from the fnode */
	fnode_extract_key(dt, tn.kindex & 0x7fffffff, tempname);

	return bit_first_different(key, tempname);
}

/* ----------------------------------------------------------------------- */
/* inserts the given key in the tree, and return the index of the trie node */
static int p_insert_key(nfs_dt_DT* dt, char* key, int nt_index)
{
	int p, t, x, px, pt;
	int i = 0;

	/* start at the root */
	p = p_get_head(dt);
	t = trienode_get_right(dt, p);

	/* navigate down the tree and look for the key */
	pt = trienode_get_bindex(dt, t);
	while (trienode_get_bindex(dt, p) < pt)
	{
		p = t;
		t = bit_get(key, pt) ? trienode_get_right(dt, t) : trienode_get_left(dt, t);
		pt = trienode_get_bindex(dt, t);
	}

	/* is the key already in the tree? */
	if (p_compare_keys(dt, key, t))
		return -1; /* already in the tree! */

	/* find first bit that does not match */
	i = p_find_first_different_bit(dt, key, t);

	/* find the appropriate place in the tree where
	the node has to be inserted */
	p  = p_get_head(dt);
	x  = trienode_get_right(dt, p);
	px = trienode_get_bindex(dt, x);
	while ( (trienode_get_bindex(dt, p) < px) && (px < i) )
	{
		p = x;
		x = bit_get(key, px) ? trienode_get_right(dt, x) : trienode_get_left(dt, x);
		px = trienode_get_bindex(dt, x);
	}

	/* allocate a new node and initialize it */
	t = node_allocate(dt, key, nt_index, i);
	trienode_set_left(dt, t, (bit_get(key, i) ? x : t));
	trienode_set_right(dt, t, (bit_get(key, i) ? t : x));

	/* rewire */
	if (bit_get(key, trienode_get_bindex(dt, p)))
		trienode_set_right(dt, p, t);
	else
		trienode_set_left(dt, p, t);

	return t;

}

/* ----------------------------------------------------------------------- */
/* lookup a given key, return its index in the file (channel). If the
key is not found, return -1 */
/* Note: this is actually a Patricia tree item search (compacted
binary trie search) */
static int p_lookup_key(nfs_dt_DT* dt, char* key)
{
	int p, x, px;

	/* start at the root */
	p = p_get_head(dt);
	x = trienode_get_right(dt, p);

	/* navigate down the tree until an upward
	link is encountered */
	px = trienode_get_bindex(dt, x);
	while (trienode_get_bindex(dt, p) < px)
	{
		p = x;
		x = bit_get(key, px) ? trienode_get_right(dt, x) : trienode_get_left(dt, x);
		px = trienode_get_bindex(dt, x);
	}

	/* full string comparison */
	if (!p_compare_keys(dt, key, x))
		return -1; /* not found! */

	/* return the node index */
	return x;

}

/* ----------------------------------------------------------------------- */
/* lookup a given key (or prefix), return its index in the file (channel). If the
key is not found, return -1. Restrict the search to the first 'n' bits.
No final string comparison is performed. */
static int p_lookup_key_n(nfs_dt_DT* dt, char* key, int n)
{
	int p, x, px;

	/* start at the root */
	p = p_get_head(dt);
	x = trienode_get_right(dt, p);

	/* navigate down the tree until an upward
	link is encountered */
	px = trienode_get_bindex(dt, x);
	while ( (trienode_get_bindex(dt, p) < px) && (px < n) )
	{
		p = x;
		x = bit_get(key, px) ? trienode_get_right(dt, x) : trienode_get_left(dt, x);
		px = trienode_get_bindex(dt, x);
	}

	/* return the last node that's not discriminating on a bit with
	index greater than n */
	return p;

}

/* ----------------------------------------------------------------------- */
/* remove a key from the Patricia structure */
static int p_remove_key(nfs_dt_DT* dt, char* k)
{

	int p, t, x, pp, lp, pt, px;
	int bp, bl, br;
	char key[nfs_dt_MAX_SUBNAME_SIZE + 1];

	/* start at the root */
	pp = -1;
	p  = p_get_head(dt);
	t  = trienode_get_right(dt, p);

	/* navigate down the tree and look for the key */
	pt = trienode_get_bindex(dt, t);
	while (trienode_get_bindex(dt, p) < pt)
	{
		pp = p;
		p  = t;
		t  = bit_get(k, pt) ? trienode_get_right(dt, t) : trienode_get_left(dt, t);
		pt = trienode_get_bindex(dt, t);
	}

	/* is the key in the tree? If not, get out! */
	if (!p_compare_keys(dt, k, t))
		return -1; /* key could not be found! */

	/* copy p's key to t */
	if (t != p)
		node_copy_key(dt, p, t);

	/* is p a leaf? */
	bp = trienode_get_bindex(dt, p);
	bl = trienode_get_bindex(dt, trienode_get_left(dt, p));
	br = trienode_get_bindex(dt, trienode_get_right(dt, p));

	if ((bl > bp) || (br > bp))
	{
		/* at least one downwards edge */

		if (p != t)
		{
			/* look for new (intermediary) key */
			fnode_extract_key(dt, trienode_get_kindex(dt, p), key);
			lp = p;
			x  = bit_get(key, trienode_get_bindex(dt, p)) ? trienode_get_right(dt, p) : trienode_get_left(dt, p);
			px = trienode_get_bindex(dt, x);
			while (trienode_get_bindex(dt, lp) < px)
			{
				lp = x;
				x  = bit_get(key, px) ? trienode_get_right(dt, x) : trienode_get_left(dt, x);
				px = trienode_get_bindex(dt, x);
			}
			/* If the intermediary key was not found, we have a problem.. */
			if (!p_compare_keys(dt, key, x))
				return -1; /* key could not be found! */
			/* rewire leaf (lp) to point to t */
			if (bit_get(key, trienode_get_bindex(dt, lp)))
				trienode_set_right(dt, lp, t);
			else
				trienode_set_left(dt, lp, t);
		}
		/* rewire parent to point to the real child of p */
		if (pp != p)
		{
			int ch;
			ch = bit_get(k, trienode_get_bindex(dt, p)) ? trienode_get_left(dt, p) : trienode_get_right(dt, p); /*!*/
			if (bit_get(k, trienode_get_bindex(dt, pp)))
				trienode_set_right(dt, pp, ch);
			else
				trienode_set_left(dt, pp, ch);
		}

	}
	else
	{
		/* both links pointing upwards or to the node (self-edges) */

		/* rewire the parent */
		if (pp != p)
		{
			bl = trienode_get_left(dt, p);
			br = trienode_get_right(dt, p);
			if (bit_get(k, trienode_get_bindex(dt, pp)))
				trienode_set_right(dt, pp, (((bl == br) && (bl == p)) ? pp : ((bl==p)?br:bl)));
			else
				trienode_set_left(dt, pp, (((bl == br) && (bl == p)) ? pp : ((bl==p)?br:bl)));
		}

	}

	/* deallocate p - no longer needed */
	trienode_recover(dt, p); /* keynodes were removed earlier, see above */

	return 0;

}

/* ----------------------------------------------------------------------- */
static char nfs_glob_key[nfs_dt_MAX_SUBNAME_SIZE];

static int p_node_iterate(nfs_dt_DT* dt, int root, int from_bidx, char* pattern, int flags,
						  int (*p)(nfs_dt_DT* dt,
								   char*       filename,
								   int         node,
								   void*       tmp),
						  void* tmp)
{

	int bidx, tl, tr;

	bidx = trienode_get_bindex(dt, root);

	if (bidx <= from_bidx)
	{
		/* The root of the Patricia structure cannot
		be inspected. Continue the search, but
		    cancel the branch */
		if (root <= 0)
			return 1;

		/* process the node: does the key match the pattern? If it does,
		call the user function. */
		fnode_extract_key(dt, trienode_get_kindex(dt, root), nfs_glob_key);
		if (nfs_pmatch(pattern, nfs_glob_key, flags) != FNM_NOMATCH)
			if (p != 0)
				if (!p(dt, nfs_glob_key, root, tmp))
					return 0;

		/* after an upward link: stop */
		return 1;
	}

	/* recursively traverse the left and right subtrees */
	tl = trienode_get_left(dt, root);
	tr = trienode_get_right(dt, root);
	if (!p_node_iterate(dt, tl, bidx, pattern, flags, p, tmp))
		return 0;
	if (!p_node_iterate(dt, tr, bidx, pattern, flags, p, tmp))
		return 0;

	return 1;

}


/* --- EXPORTED ---------------------------------------------------------- */

/* Create a new directory table in the given iio file & channel */
nfs_dt_DT* nfs_dt_create(nfs_iio_File* file, int bpc1, int bpc2)
{

	nfs_dt_DT*  dt;
	int          chn1, chn2;

	if (file == NULL)
		return NULL;

	/* init with defaults if necessary */
	if (bpc1 == 0)
		bpc1 = nfs_dt_DEF_CHANNEL1_SIZE;

	if (bpc2 == 0)
		bpc2 = nfs_dt_DEF_CHANNEL2_SIZE;

	/* allocate two new channels */
	chn1 = nfs_iio_allocate_channel(file, bpc1);
	chn2 = nfs_iio_allocate_channel(file, bpc2);

	/* allocate new DT handle and initialize it */
	dt = (nfs_dt_DT*)malloc(sizeof(nfs_dt_DT));
	dt->file        = file;
	dt->channel1    = chn1;
	dt->channel2    = chn2;
	dt->unallocated = 1;
	dt->unallocated2 = 1;

	/* initialize the first node in the channel (Trie&Key nodes #0) */
	p_init_head(dt); /* this node (0) will always be reserved for the root of the tree */

	/* return the handle */
	return dt;

}

/* ----------------------------------------------------------------------- */
/* Open a directory table */
nfs_dt_DT* nfs_dt_open(nfs_iio_File* file, int channel1, int channel2)
{

	nfs_dt_DT* dt;

	/* allocate new DT handle and initialize it */
	dt = (nfs_dt_DT*)malloc(sizeof(nfs_dt_DT));
	dt->file        = file;
	dt->channel1    = channel1;
	dt->channel2    = channel2;
	dt->unallocated  = trienode_find_first_free(dt, 1);
	dt->unallocated2 = keynode_find_first_free(dt, 1);

	/* return the handle */
	return dt;

}

/* ----------------------------------------------------------------------- */
/* Close the directory table */
int nfs_dt_close(nfs_dt_DT* dt)
{
	if (dt != NULL)
		free(dt);
	return 0;
}

/* ----------------------------------------------------------------------- */
/* Destroy the directory table */
int nfs_dt_destroy(nfs_dt_DT* dt)
{
	/* close */
	nfs_dt_close(dt);
	/* remove */
	/* ... */
	return 0;
}

/* ----------------------------------------------------------------------- */
/* Add new file name, return index in channel */
int nfs_dt_filename_add(nfs_dt_DT* dt, char* filename)
{
	if (filename == NULL)
		return -1;
	return p_insert_key(dt, filename, 0);
}

/* ----------------------------------------------------------------------- */
/* Delete file name given by its index in table */
int nfs_dt_filename_delete(nfs_dt_DT* dt, char* filename)
{
	if (filename == NULL)
		return -1;
	return p_remove_key(dt, filename);
}

/* ----------------------------------------------------------------------- */
/* Lookup file name in DT, return index in DT */
int nfs_dt_filename_lookup(nfs_dt_DT* dt, char* filename)
{
	if (filename == NULL)
		return -1;
	return p_lookup_key(dt, filename);
}

/* ----------------------------------------------------------------------- */
/* Globbing (pattern matching): lookup those names that match the given pattern and
for each of those names call the given callback */
int nfs_dt_filename_glob(nfs_dt_DT* dt,
						 char* pattern,
						 int flags,
						 int (*p)(nfs_dt_DT* dt,
								  char* filename,
								  int filename_idx,
								  void* tmp),
						 void* tmp)
{

	char prefix[256];
	int  start;

	/* find commmon prefix (longest possible prefix in the
	given pattern that does not contain wildcards) */
	find_prefix(pattern, prefix);

	/* find the node corresponding to the longest
	substring of the computed prefix */
	start = p_lookup_key_n(dt, prefix, strlen(prefix) * 8);

	/* all the keys in the subtree starting at the node
	found above is a potential match. Get all these
	keys, and perform a pattern matching on each one. */
	p_node_iterate(dt, start, trienode_get_bindex(dt, start) - 1, pattern, flags, p, tmp);

	return 0;
}

/* ----------------------------------------------------------------------- */
/* Get the NT index associated with a given filename (given by its index in the DT) */
int nfs_dt_filename_get_nt_index(nfs_dt_DT* dt, int filename_idx)
{
	return trienode_get_nt(dt, filename_idx);
}

/* ----------------------------------------------------------------------- */
/* Set the NT index associated with a given filename (given by its index in the DT) */
int nfs_dt_filename_set_nt_index(nfs_dt_DT* dt, int filename_idx, int nt_idx)
{
	return trienode_set_nt(dt, filename_idx, nt_idx);
}

/* ----------------------------------------------------------------------- */
/* Get the filename(char*) associated with a given filename (given by its index in the DT) */
int nfs_dt_filename_get_name(nfs_dt_DT* dt, int filename_idx, char* buffer)
{
	fnode_extract_key(dt, trienode_get_kindex(dt, filename_idx), buffer);
	return 0;
}

