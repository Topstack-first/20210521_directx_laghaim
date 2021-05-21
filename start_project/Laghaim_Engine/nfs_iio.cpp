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
// File:    nfs_iio.c
// Date:    07/08/02
// Content: Interlaced I/O module (IIO)
//
//----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

#if defined(WIN32) && !defined(CYGWIN) && !defined(CYGNUS)
#include <windows.h> /* for GetSystemInfo() */
#include <io.h>      /* for chsize() */
#else
#include <unistd.h>  /* for getpagesize() */
#endif

#include "nfs_iio.h"
//----------------------------------------------------------------------------
#pragma warning( disable : 4996)
//----------------------------------------------------------------------------
//  performance tracking
//----------------------------------------------------------------------------
#ifdef INSTRUMENT_SYSTEM

static int  cache_misses       = 0;  /* number of cache misses */
static int  cache_hits         = 0;  /* number of cache hits   */
static int  cache_flushes      = 0;  /* nr. of times cache was flushed */
static int  iio_read_requests  = 0;  /* nr. of read requests   */
static int  iio_write_requests = 0;  /* nr. of write requests  */
static int  iio_seek_requests  = 0;  /* nr. of seek requests   */
static int  partial_reads      = 0;  /* nr of partial channel reads */
static int  partial_writes     = 0;  /* nr of partial channel writes */
static int  full_reads         = 0;  /* nr of full channel reads */
static int  full_writes        = 0;  /* nr of full channel writes */
static int  abs_block_reads    = 0;  /* nr of absolute reads (physical reads) */
static int  abs_block_writes   = 0;  /* nr of absolute writes (physical writes) */
static int  page_switches      = 0;  /* nr. of page switches */
static char output_file_name[256];

static int fexists(char* s)
{
	FILE* f = fopen(s, "rb");
	if (f != NULL)
	{
		fclose(f);
		return 1;
	}
	return 0;
}

static void instrument_init()
{

	int i, ok;

	iio_read_requests = 0;
	iio_write_requests = 0;
	cache_misses = 0;
	cache_hits = 0;
	iio_seek_requests = 0;
	partial_reads = 0;
	partial_writes = 0;
	full_reads = 0;
	full_writes = 0;
	cache_flushes = 0;
	abs_block_reads = 0;
	abs_block_writes = 0;
	page_switches = 0;

	i = 0;
	ok = 0;
	while (!ok)
	{
		sprintf(output_file_name, "nfs_iio.instrument.%d.log%c", i++, 0);
		ok = !fexists(output_file_name);
	}

}

static void instrument_done()
{

	FILE* f = fopen(output_file_name, "wt");
	if (f == NULL)
		return;

	fprintf(f, "\n\n--------------------------------------------------\n");
	fprintf(f, "Overall statistics\n");
	fprintf(f, "--------------------------------------------------\n");

	fprintf(f, "IIO requests\n");
	fprintf(f, "  IIO Read requests (nfs_iio_read):     %d\n", iio_read_requests);
	fprintf(f, "  IIO Write requests (nfs_iio_write):   %d\n", iio_write_requests);
	fprintf(f, "  IIO Seek requests (nfs_iio_seek):     %d\n", iio_seek_requests);

	fprintf(f, "Block requests\n");
	fprintf(f, "  Partial block reads:                   %d\n", partial_reads);
	fprintf(f, "  Partial block writes:                  %d\n", partial_writes);
	fprintf(f, "  Full block reads:                      %d\n", full_reads);
	fprintf(f, "  Full block writes:                     %d\n", full_writes);
	fprintf(f, "  Absolute block reads (physical):       %d\n", abs_block_reads);
	fprintf(f, "  Absolute block writes (physical):      %d\n", abs_block_writes);

	fprintf(f, "Cache\n");
	fprintf(f, "  Cache misses:                          %d\n", cache_misses);
	fprintf(f, "  Cache hits:                            %d\n", cache_hits);
	fprintf(f, "  Cache flushes:                         %d\n", cache_flushes);
	fprintf(f, "  Page switches:                         %d\n", page_switches);
	fprintf(f, "  Cache efficiency:                      %lf%%\n", 100.0 * ((double)cache_hits / (double)(cache_hits + cache_misses)));

	fclose(f);

}

#endif

/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */
/*  typedefs and other definitions                             */
/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */


#define nfs_iio_MAGIC ('A' | ('B' << 8) | ('C' << 16) | ('D' << 24))

typedef struct
{
	int magic;
	int channels;
} nfs_iio_File_Header;

typedef struct
{
	int blocks;
	int size;
} nfs_iio_Channel_Header;



/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */
/*  internal                                                   */
/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */


int nfs_iio_IOMODE = 1;
int nfs_iio_BLOCK_SIZEv;

#if (nfs_iio_CC_POLICY == ___LRU___)
int nfs_iio_CLOCK = 0;
#endif

/* get the size of the vmem page */
static int get_page_size()
{

#if defined(WIN32) && !defined(CYGWIN) && !defined(CYGNUS)
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return (int)si.dwPageSize;
#else
	return getpagesize();
#endif

}

/* ----------------------------------------------------------- */
/* converts the block nr. within channel to the absolute       */
/* block nr.                                                   */
/* ----------------------------------------------------------- */

static int channel_block_to_absolute_block(nfs_iio_File* file, int channel, int cb)
{

	int chunk_no;
	int block_within_chunk;
	int bpc;
	int i;

	if (file == NULL)
		return -1;

	/* chunk number (where the given block resides) */
	chunk_no = cb / file->chn[channel]->blocks;

	/* compute the block index within the chunk */
	block_within_chunk = cb % file->chn[channel]->blocks;
	for (i = 0; i < channel; i++)
		block_within_chunk += file->chn[i]->blocks;

	/* how many blocks per chunk? */
	bpc = nfs_iio_blocks_per_chunk(file);

	/* compute and return absolute block */
	return (chunk_no * bpc + block_within_chunk);
}

/* ----------------------------------------------------------- */
/* converts the channel pos to absolute block number           */
/* ----------------------------------------------------------- */

static int channel_pos_to_absolute_block(nfs_iio_File* file, int channel, int pos)
{
	int cb;

	/* block within channel */
	cb = pos / nfs_iio_BLOCK_SIZEv;

	/* convert block-within-channel to absolute-block */
	return channel_block_to_absolute_block(file, channel, cb);
}

/* ----------------------------------------------------------- */
/* ret the size of the header, the file structure does not     */
/* necessarily have to be initialized                          */
/* ----------------------------------------------------------- */

static int header_size(nfs_iio_File* file)
{
	int k = sizeof(nfs_iio_File_Header) + file->channels * sizeof(nfs_iio_Channel_Header);
	if (k > nfs_iio_BLOCK_SIZEv)
	{
		printf("internal error: iio overflow. Too many iio channels.");
		exit(-1);
	}
	return nfs_iio_BLOCK_SIZEv;
}

/* ----------------------------------------------------------- */
/* reads absolute block nr. 'block_no'                         */
/* ----------------------------------------------------------- */
#ifdef UNUSED
static int read_absolute_block(nfs_iio_File* file, int block_no, void* block)
{

	int read_ = 0;

#ifdef INSTRUMENT_SYSTEM
	abs_block_reads++;
#endif

	if (file->file == NULL)
		return -1; /* handle not valid */

	if (block == NULL)
		return -1; /* invalid address */

	/* go to that position and read the block */
	fseek(file->file, header_size(file) + (block_no * nfs_iio_BLOCK_SIZEv), 0);
	read_ = fread(block, 1, nfs_iio_BLOCK_SIZEv, file->file);

	/* if an incomplete block was read in, complete it with additional zeroes */
	if (read_ < nfs_iio_BLOCK_SIZEv)
		ZeroMemory((char*)block + read_, nfs_iio_BLOCK_SIZEv - read_);

	return nfs_iio_BLOCK_SIZEv;
}
#endif /* UNUSED */

/* ----------------------------------------------------------- */
/* reads N absolute blocks starting at nr. 'start'             */
/* ----------------------------------------------------------- */

static int read_absolute_block_n(nfs_iio_File* file, int start, int N, void* buf)
{

	int read_ = 0;

#ifdef INSTRUMENT_SYSTEM
	abs_block_reads++;
#endif

	if (file->file == NULL)
		return -1; /* handle not valid */

	if (buf == NULL)
		return -1; /* invalid address */

	/* go to that position and read the block */
	fseek(file->file, header_size(file) + (start * nfs_iio_BLOCK_SIZEv), 0);
	read_ = fread(buf, 1, nfs_iio_BLOCK_SIZEv * N, file->file);

	/* if an incomplete block was read in, complete it with additional zeroes */
	if (read_ < nfs_iio_BLOCK_SIZEv * N)
		ZeroMemory((char*)buf + read_, nfs_iio_BLOCK_SIZEv * N - read_);

	return nfs_iio_BLOCK_SIZEv * N;
}

/* ----------------------------------------------------------- */
/* writes absolute block nr. 'block_no'                        */
/* ----------------------------------------------------------- */

#ifdef UNUSED
static int write_absolute_block(nfs_iio_File* file, int block_no, void* block)
{

#ifdef INSTRUMENT_SYSTEM
	abs_block_writes++;
#endif

	if (file->file == NULL)
		return -1; /* handle not valid */

	if (block == NULL)
		return -1; /* invalid address */

	/* go to that position and write the block */
	fseek(file->file, header_size(file) + (block_no * nfs_iio_BLOCK_SIZEv), 0);
	fwrite(block, 1, nfs_iio_BLOCK_SIZEv, file->file);

	return nfs_iio_BLOCK_SIZEv;
}
#endif /* UNUSED */

/* ----------------------------------------------------------- */
/* writes N contiguous absolute blocks starting at 'start'     */
/* ----------------------------------------------------------- */

static int write_absolute_block_n(nfs_iio_File* file, int start, int N, void* buf)
{

#ifdef INSTRUMENT_SYSTEM
	abs_block_writes++;
#endif

	if (file->file == NULL)
		return -1; /* handle not valid */

	if (buf == NULL)
		return -1; /* invalid address */

	/* go to that position and write the block */
	fseek(file->file, header_size(file) + (start * nfs_iio_BLOCK_SIZEv), 0);
	fwrite(buf, 1, nfs_iio_BLOCK_SIZEv * N, file->file);

	return nfs_iio_BLOCK_SIZEv * N;
}

/* get/set */
static int cache_page_get_sync(nfs_iio_Cache_Page* c)
{
	if (c == NULL)
		return 0;
#pragma warning(push)
#pragma warning(disable: 4311)
	return (int)c->buffer & nfs_iio_CC_SYNC_MASK;
#pragma warning(pop)
}

static void cache_page_set_sync(nfs_iio_Cache_Page* c, int s)
{
	if (c == NULL)
		return;
#pragma warning(push)
#pragma warning(disable: 4311)
	c->buffer = (char*)((int)c->buffer & nfs_iio_CC_BUF_MASK);
	c->buffer = (char*)((int)c->buffer | (s & nfs_iio_CC_SYNC_MASK));
#pragma warning(pop)
}

static char* cache_page_get_buffer(nfs_iio_Cache_Page* c)
{
	if (c == NULL)
		return NULL;
#pragma warning(push)
#pragma warning(disable: 4311)
	return (char*)((int)c->buffer & nfs_iio_CC_BUF_MASK);
#pragma warning(pop)
}

#ifdef UNUSED
static void cache_page_set_buffer(nfs_iio_Cache_Page* c, char* buf)
{
	if (c == NULL)
		return;
	c->buffer = (char*)((int)c->buffer & nfs_iio_CC_SYNC_MASK);
	c->buffer = (char*)((int)c->buffer | ((int)buf & nfs_iio_CC_BUF_MASK));
}
#endif /* UNUSED */

/* expands the cache and makes it encapsulate the given page */
static int cache_expand(nfs_iio_File* file, int channel, int page)
{
	int oldpgs, i;
	int pgs;

	if (file->chn[channel]->cache != NULL)
		pgs = file->chn[channel]->cache->nr_pages;
	else
		pgs = 0;
	oldpgs = pgs;
	/* double until page is in cache */
	if (pgs == 0) pgs++;
	while ((pgs <= page) || (pgs < nfs_iio_CC_DEF_TABSIZE)) pgs <<= 1;
	/* create cache if nonexistent */
	if (file->chn[channel]->cache == NULL)
	{
		file->chn[channel]->cache = (nfs_iio_Cache*)malloc(sizeof(nfs_iio_Cache));
		file->chn[channel]->cache->pages = NULL;
		file->chn[channel]->cache->nr_pages = 0;
		file->chn[channel]->cache->active_pages = 0;
	}
	/* update the page table */
	file->chn[channel]->cache->nr_pages = pgs;
	file->chn[channel]->cache->pages = (nfs_iio_Cache_Page**)realloc(file->chn[channel]->cache->pages,
									   pgs * sizeof(nfs_iio_Cache_Page*));
	for (i = oldpgs; i < pgs; i++)
		file->chn[channel]->cache->pages[i] = NULL;

	return pgs;
}

/* chooses the best page to reuse, avoiding the specified page */
static int cache_page_choose_best_to_reuse(nfs_iio_File* file, int channel, int page)
{

	int i, k;
	int rpage = 0;

#if (nfs_iio_CC_POLICY == ___LRU___)
	/* Policy: Least Recently Used */

	i = 0x7fffffff;
	for (k = 0; k < file->chn[channel]->cache->nr_pages; k++)
		if ((file->chn[channel]->cache->pages[k] != NULL) &&
				(file->chn[channel]->cache->pages[k]->timestamp < i) &&
				(k != page))
		{
			i = file->chn[channel]->cache->pages[k]->timestamp;
			rpage = k;
		}

#endif

#if (nfs_iio_CC_POLICY == ___LFU___)
	/* Policy: Least Frequently Used */

	i = 0x7fffffff;
	for (k = 0; k < file->chn[channel]->cache->nr_pages; k++)
		if ((file->chn[channel]->cache->pages[k] != NULL) &&
				(file->chn[channel]->cache->pages[k]->hits < i) &&
				(k != page))
		{
			i = file->chn[channel]->cache->pages[k]->hits;
			rpage = k;
		}

#endif

#if (nfs_iio_CC_POLICY == ___NLN___)
	/* Policy: Next Lower Neighbor */

	rpage = page - 1;
	while (file->chn[channel]->cache->pages[rpage] == NULL)
		if (rpage == 0)
			rpage = file->chn[channel]->cache->nr_pages - 1;
		else
			rpage--;

#endif

	return rpage;

}

static void cache_page_flush(nfs_iio_File* file, int channel, int page);

/* creates the given cache page */
static void cache_page_create(nfs_iio_File* file, int channel, int page)
{

	if (file->chn[channel]->cache->pages[page] == NULL)
	{

		/* if the number of active pages is not larger than
		the maximum number allowed, increase it and allocate
		    another page */

#ifdef nfs_iio_CC_RESTRICT_GROWTH
		if (file->chn[channel]->cache->active_pages < nfs_iio_CC_MAX_ACTIVE_PGS)
		{
#endif

			/* increase the number of active pages */
			file->chn[channel]->cache->active_pages++;

			/* allocate page entry */
			file->chn[channel]->cache->pages[page] = (nfs_iio_Cache_Page*)malloc(sizeof(nfs_iio_Cache_Page));

			/* allocate page buffer */
			file->chn[channel]->cache->pages[page]->buffer = (char*)malloc(nfs_iio_BLOCK_SIZEv *
					file->chn[channel]->blocks);
			/* allocation errors? */
			if (file->chn[channel]->cache->pages[page]->buffer == NULL)
			{
				fprintf(stderr, "Internal error: not enough memory for cache page. Aborting...\n");
				exit(-1);
			}

			/* initialize the page */
			file->chn[channel]->cache->pages[page]->position = 0;

#ifndef nfs_iio_CC_MEMSET_OPT
			memset(file->chn[channel]->cache->pages[page]->buffer,
				   0,
				   nfs_iio_BLOCK_SIZEv * file->chn[channel]->blocks);
#endif

			/* force the sync flag */
			cache_page_set_sync(file->chn[channel]->cache->pages[page], 1);

#ifdef nfs_iio_CC_RESTRICT_GROWTH
		}
		else
		{

			int rpage;

			/* The maximum amount of active pages has been reached, so
			an existing page has to be reused. Choose the best page to
			reuse and "rewire" it to this entry */

#ifdef INSTRUMENT_SYSTEM
			page_switches++;
#endif

			/* choose the best one */
			rpage = cache_page_choose_best_to_reuse(file, channel, page);

#ifdef __DEBUG__
			printf("[debug] page switch: %d's buffer rewired to %d\n", rpage, page);
#endif

			/* flush the old page if necessary */
			cache_page_flush(file, channel, rpage);

			/* rewire it */
			file->chn[channel]->cache->pages[page]  = file->chn[channel]->cache->pages[rpage];
			file->chn[channel]->cache->pages[rpage] = NULL;

			/* reconfigure the page */
			file->chn[channel]->cache->pages[page]->position = 0;
#ifndef nfs_iio_CC_MEMSET_OPT
			memset(cache_page_get_buffer(file->chn[channel]->cache->pages[page]),
				   0,
				   nfs_iio_BLOCK_SIZEv * file->chn[channel]->blocks);
#endif
#if (nfs_iio_CC_POLICY == ___LRU___)
			file->chn[channel]->cache->pages[page]->timestamp = nfs_iio_CLOCK;
#endif

#if (nfs_iio_CC_POLICY == ___LFU___)
			file->chn[channel]->cache->pages[page]->hits = 0;
#endif

			/* force the sync flag */
			cache_page_set_sync(file->chn[channel]->cache->pages[page], 1);

		}
#endif /* nfs_iio_CC_RESTRICT_GROWTH */

	}

}

/* refreshes the cache page from the given position */
static int cache_page_refresh(nfs_iio_File* file, int channel, int pos)
{

	int page, ablock;

	/* compute the page number, expand cache if necessary */
	page = pos / (nfs_iio_BLOCK_SIZEv * file->chn[channel]->blocks);
	if ((file->chn[channel]->cache == NULL) ||
			(page >= file->chn[channel]->cache->nr_pages))
		cache_expand(file, channel, page);

	/* is the page already allocated? if not, allocate */
	if (file->chn[channel]->cache->pages[page] == NULL)
		cache_page_create(file, channel, page);

	/* refresh the page */
	ablock = channel_pos_to_absolute_block(file, channel, pos);
	read_absolute_block_n(file,
						  ablock,
						  file->chn[channel]->blocks,
						  cache_page_get_buffer(file->chn[channel]->cache->pages[page]));

	/* cache page is in sync */
	cache_page_set_sync(file->chn[channel]->cache->pages[page], 1);
	file->chn[channel]->cache->pages[page]->position = pos;

	return 0;

}

/* dumps the given cache page immediately in the given channel */
static int cache_page_dump(nfs_iio_File* file, int channel, int page)
{

	int pos, ablock;

#ifdef INSTRUMENT_SYSTEM
	cache_flushes++;
#endif

	/* invalid page? */
	if (file->chn[channel]->cache->pages[page] == NULL)
		return 0;

	pos = file->chn[channel]->cache->pages[page]->position;
	ablock = channel_pos_to_absolute_block(file, channel, pos);
	write_absolute_block_n(file,
						   ablock,
						   file->chn[channel]->blocks,
						   cache_page_get_buffer(file->chn[channel]->cache->pages[page]));

	/* cache page is in sync */
	cache_page_set_sync(file->chn[channel]->cache->pages[page], 1);

	return 0;

}

/* flushes the given cache page */
static void cache_page_flush(nfs_iio_File* file, int channel, int i)
{
	/* flush if out-of-sync */
	if (cache_page_get_sync(file->chn[channel]->cache->pages[i]) == 0)
		cache_page_dump(file, channel, i);
}

/* flushes the entire cache if necessary */
static void cache_flush(nfs_iio_File* file, int channel)
{
	int i;
	/* flush all pages (if necessary) */
	for (i = 0; i < file->chn[channel]->cache->nr_pages; i++)
		cache_page_flush(file, channel, i);
}

/* updates the channel cache to include the given position */
static void cache_update(nfs_iio_File* file, int channel, int pos)
{

	int page, pgsize;

	/* update RG 05/16/01 (!W@#$%!^&*)
	First, the position has to be a multiple of cache page size, otherwise
	the hell breaks loose from this point on */
	pgsize = nfs_iio_BLOCK_SIZEv * file->chn[channel]->blocks;
	if ((pos % pgsize) != 0)
		pos -= (pos % pgsize);

	/* compute the page number, expand cache if necessary */
	page = pos / pgsize;
	if ((file->chn[channel]->cache == NULL) ||
			(page >= file->chn[channel]->cache->nr_pages))
		cache_expand(file, channel, page);

	/* is the page already allocated? if not, allocate */
	if (file->chn[channel]->cache->pages[page] == NULL)
		cache_page_create(file, channel, page);

	/* if out-of-sync, dump */
	cache_page_flush(file, channel, page); /* necessary? */

	/* refresh the page */
	cache_page_refresh(file, channel, pos);

}

/* returns non-zero (true) if the given channe block is in the cache */
static int is_in_cache(nfs_iio_File* file, int channel, int block)
{
	int page = block / file->chn[channel]->blocks;
	return (file->chn[channel]->cache->pages[page] != NULL);
}

/* creates a channel cache */
static int cache_create(nfs_iio_File* file, int channel)
{

	/* errors? */
	if (file == NULL)
		return -1;

	/* read in the initial data */
	cache_page_refresh(file, channel, 0);

	/* success! */
	return 0;

}

static int cache_destroy(nfs_iio_Cache* c)
{

	int i;

	/* errors? */
	if (c == NULL)
		return -1;

	if (c->pages == NULL)
		return -1;

	/* free pages */
	for (i = 0; i < c->nr_pages; i++)
		if (c->pages[i] != NULL)
		{
#pragma warning(push)
#pragma warning(disable: 4311)
			free((char*)((int)c->pages[i]->buffer & ~1));
#pragma warning(pop)
			free(c->pages[i]);
		}

	/* free entry table */
	free(c->pages);

	/* free cache structure */
	free(c);

	return 0;

}

/* reads an entire channel block (cached) */
static int cache_read_channel_block(nfs_iio_File* file, int channel, int block_no, void* block)
{

	int page;

#ifdef INSTRUMENT_SYSTEM
	full_reads++;
#endif

	/* update the cache if necessary */
	if (!is_in_cache(file, channel, block_no))
#ifdef INSTRUMENT_SYSTEM
	{
		cache_misses++;
#endif
		cache_update(file, channel, block_no * nfs_iio_BLOCK_SIZEv);
#ifdef INSTRUMENT_SYSTEM
	}
	else
		cache_hits++;
#endif

	/* compute the index of the page where this block resides */
	page = block_no / file->chn[channel]->blocks;

	/* copy block from cache */
	memcpy(block,
		   cache_page_get_buffer(file->chn[channel]->cache->pages[page]) +
		   (block_no * nfs_iio_BLOCK_SIZEv - file->chn[channel]->cache->pages[page]->position),
		   nfs_iio_BLOCK_SIZEv);

#if (nfs_iio_CC_POLICY == ___LRU___)
	file->chn[channel]->cache->pages[page]->timestamp = nfs_iio_CLOCK;
#endif

#if (nfs_iio_CC_POLICY == ___LFU___)
	file->chn[channel]->cache->pages[page]->hits++;
#endif

	/* return the number of bytes read */
	return nfs_iio_BLOCK_SIZEv;

}

#ifdef __DEBUG__
static int __stats(nfs_iio_File* file, int channel)
{

	int i;

	printf("Statistics\n");
	printf("  Cache pages:  %d\n", file->chn[channel]->cache->nr_pages);
	printf("  Active pages: %d\n", file->chn[channel]->cache->active_pages);

	for (i = 0; i < file->chn[channel]->cache->active_pages; i++)
		if (file->chn[channel]->cache->pages[i] != NULL)
			printf("    %d: Position = %d\n", i, file->chn[channel]->cache->pages[i]->position);
		else
			printf("    %d: NULL\n");

	return 0;

}
#endif

/* writes an entire channel block (cached) */
static int cache_write_channel_block(nfs_iio_File* file, int channel, int block_no, void* block)
{

	int page;

#ifdef INSTRUMENT_SYSTEM
	full_writes++;
#endif

	/* update the cache if necessary */
	if (!is_in_cache(file, channel, block_no))
#ifdef INSTRUMENT_SYSTEM
	{
		cache_misses++;
#endif
		cache_update(file, channel, block_no * nfs_iio_BLOCK_SIZEv);
#ifdef INSTRUMENT_SYSTEM
	}
	else
		cache_hits++;
#endif

	/* compute the page where this block resides */
	page = block_no / file->chn[channel]->blocks;

	/* copy block to cache */
	memcpy(cache_page_get_buffer(file->chn[channel]->cache->pages[page]) +
		   (block_no * nfs_iio_BLOCK_SIZEv - file->chn[channel]->cache->pages[page]->position),
		   block,
		   nfs_iio_BLOCK_SIZEv);

	/* cache page is out-of-sync */
	cache_page_set_sync(file->chn[channel]->cache->pages[page], 0);

#if (nfs_iio_CC_POLICY == ___LRU___)
	file->chn[channel]->cache->pages[page]->timestamp = nfs_iio_CLOCK;
#endif

#if (nfs_iio_CC_POLICY == ___LFU___)
	file->chn[channel]->cache->pages[page]->hits++;
#endif

	/* return the number of bytes written */
	return nfs_iio_BLOCK_SIZEv;
}

/* reads a partial channel block (cached) */
static int cache_read_partial_channel_block(nfs_iio_File* file, int channel, int block_no, int start, int end, void* block)
{

	int page;

#ifdef INSTRUMENT_SYSTEM
	partial_reads++;
#endif

	/* update the cache if necessary */
	if (!is_in_cache(file, channel, block_no))
#ifdef INSTRUMENT_SYSTEM
	{
		cache_misses++;
#endif
		cache_update(file, channel, block_no * nfs_iio_BLOCK_SIZEv);
#ifdef INSTRUMENT_SYSTEM
	}
	else
		cache_hits++;
#endif

	/* compute the page where this block resides */
	page = block_no / file->chn[channel]->blocks;

	/* copy partial block from cache */
	memcpy(block,
		   cache_page_get_buffer(file->chn[channel]->cache->pages[page]) +
		   (block_no * nfs_iio_BLOCK_SIZEv - file->chn[channel]->cache->pages[page]->position) +
		   start,
		   (end - start + 1));

#if (nfs_iio_CC_POLICY == ___LRU___)
	file->chn[channel]->cache->pages[page]->timestamp = nfs_iio_CLOCK;
#endif

#if (nfs_iio_CC_POLICY == ___LFU___)
	file->chn[channel]->cache->pages[page]->hits++;
#endif

	/* return the number of bytes read */
	return (end-start+1);

}

/* writes a partial channel block (cached) */
static int cache_write_partial_channel_block(nfs_iio_File* file, int channel, int block_no, int start, int end, void* block)
{

	int page;

#ifdef INSTRUMENT_SYSTEM
	partial_writes++;
#endif

	/* update the cache if necessary */
	if (!is_in_cache(file, channel, block_no))
#ifdef INSTRUMENT_SYSTEM
	{
		cache_misses++;
#endif
		cache_update(file, channel, block_no * nfs_iio_BLOCK_SIZEv);
#ifdef INSTRUMENT_SYSTEM
	}
	else
		cache_hits++;
#endif

	/* compute the page where this block resides */
	page = block_no / file->chn[channel]->blocks;

	/* copy partial block to cache */
	memcpy(cache_page_get_buffer(file->chn[channel]->cache->pages[page]) +
		   (block_no * nfs_iio_BLOCK_SIZEv - file->chn[channel]->cache->pages[page]->position) +
		   start,
		   block,
		   (end - start + 1));

	/* cache page is out-of-sync! */
	cache_page_set_sync(file->chn[channel]->cache->pages[page], 0);

#if (nfs_iio_CC_POLICY == ___LRU___)
	file->chn[channel]->cache->pages[page]->timestamp = nfs_iio_CLOCK;
#endif

#if (nfs_iio_CC_POLICY == ___LFU___)
	file->chn[channel]->cache->pages[page]->hits++;
#endif

	/* return the number of bytes written */
	return (end-start+1);

}

/* ----------------------------------------------------------- */
/* read IIO header from file; file is already open             */
/* ----------------------------------------------------------- */

static int read_header(nfs_iio_File* file)
{

	static nfs_iio_File_Header    fh;
	static nfs_iio_Channel_Header ch;
	int i, k;

	/* errors? */
	if (file->file == NULL)
		return -1;

	/* read the file header, check magic */
	fseek(file->file, 0, 0);
	fread(&fh, sizeof(fh), 1, file->file);
	if (fh.magic != nfs_iio_MAGIC)
		return -1; /* invalid IIO file */

	/* init channels */
	file->channels = fh.channels;
	file->chn = (nfs_iio_Channel**)malloc( fh.channels * sizeof(nfs_iio_Channel*) );

	/* reopen the appropriate channels */
	for (i = 0; i < fh.channels; i++)
	{
		file->chn[i] = (nfs_iio_Channel*)malloc(sizeof(nfs_iio_Channel));
		/* read channel header */
		fseek(file->file, sizeof(fh) + (i * sizeof(ch)), 0);
		k = fread(&ch, 1, sizeof(ch), file->file);
		file->chn[i]->size   = ch.size;
		file->chn[i]->blocks = ch.blocks;
		file->chn[i]->cp     = 0;
		file->chn[i]->cache  = NULL;
	}

	/* initialize the cache buffers */
	for (i = 0; i < fh.channels; i++)
		cache_create(file, i);

	return 0;

}

/* ----------------------------------------------------------- */
/* write IIO header to file; file is already open              */
/* ----------------------------------------------------------- */

static int write_header(nfs_iio_File* file)
{

	nfs_iio_File_Header    fh;
	nfs_iio_Channel_Header ch;
	int i;

	/* errors? */
	if (file->file == NULL)
		return -1;

	/* initialize the file header */
	fh.magic    = nfs_iio_MAGIC;
	fh.channels = file->channels;

	/* write file header */
	fseek(file->file, 0, 0);
	fwrite(&fh, 1, sizeof(fh), file->file);

	/* initialize the channel headers and dump them in the file */
	for (i = 0; i < file->channels; i++)
	{
		/* initialize channel header for channel 'i' */
		ch.blocks = file->chn[i]->blocks;
		ch.size   = file->chn[i]->size;
		/* write channel header */
		fwrite(&ch, 1, sizeof(ch), file->file);
	}

	fflush(file->file);

	return 0;

}

/* ----------------------------------------------------------- */
/* Auto-truncate the IIO file if necessary                     */
/* ----------------------------------------------------------- */

void auto_truncate(nfs_iio_File* file)
{
	int i, nc, ns;
	int max_chunks  = 0; /* max chunks in a channel */
	int bps         = 0; /* total blocks per slice */

	for (i = 0; i < (int)file->channels; i++)
	{
		nc = 1 + ((1 + (file->chn[i]->size / nfs_iio_BLOCK_SIZEv)) / file->chn[i]->blocks);
		if (nc > max_chunks)
		{
			max_chunks = nc;
		}
		bps += file->chn[i]->blocks;
	}

	if (max_chunks > 0)
	{
		ns = max_chunks * bps;
		/* ns is now the number of blocks actually used */
		fseek(file->file, header_size(file) + ns * nfs_iio_BLOCK_SIZEv, 0);
#if defined(WIN32) && !defined(CYGWIN) && !defined(CYGNUS)
		chsize(fileno(file->file), header_size(file) + ns * nfs_iio_BLOCK_SIZEv);
#else
		ftruncate(fileno(file->file), header_size(file) + ns * nfs_iio_BLOCK_SIZEv);
#endif

	}

}

/* ----------------------------------------------------------- */
/* flushes all the data                                        */
/* ----------------------------------------------------------- */

static void flush_data(nfs_iio_File* file)
{

	int i;

	/* flush the channel cache buffers */
	for (i = 0; i < file->channels; i++)
		cache_flush(file, i);

	/* i/o flush */
	if (file->file != NULL)
		fflush(file->file);

}



/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */
/*  Exported                                                   */
/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */




/* ----------------------------------------------------------- */
/* Positions the channel pointer to a certain position in the  */
/* given channel                                               */
/* ----------------------------------------------------------- */

int nfs_iio_seek(nfs_iio_File* file, int channel, int where)
{

#ifdef INSTRUMENT_SYSTEM
	iio_seek_requests++;
#endif

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	/* check for illegal arguments */
	if (file == NULL)
		return -1;

	if ((channel < 0) || (channel >= (int)file->channels))
		return -1;

	/* position the channel pointer */
	file->chn[channel]->cp = where;

	return where;
}

/* ----------------------------------------------------------- */
/* Create an Interlaced I/O file                               */
/* ----------------------------------------------------------- */

nfs_iio_File* nfs_iio_create(char* filename)
{

	nfs_iio_File* handle;

#ifdef INSTRUMENT_SYSTEM
	instrument_init();
#endif

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	/* first, figure out what the vm page size is, and set the block
	size equal to the page size */
	nfs_iio_BLOCK_SIZEv = get_page_size();
	if (nfs_iio_BLOCK_SIZEv < nfs_iio_BLOCK_SIZE)
		nfs_iio_BLOCK_SIZEv = nfs_iio_BLOCK_SIZE;

	/* create a new file handle and initialize it */
	handle = (nfs_iio_File*)malloc(sizeof(nfs_iio_File));
	if (handle == NULL)
		return NULL; /* not enough memory */
	handle->channels = 0;
	handle->chn      = NULL;
	handle->filename = (char*)malloc(strlen(filename) + 8);
	if (handle->filename == NULL)
		return NULL; /* out of memory */
	strcpy(handle->filename, filename);
	handle->file     = fopen(filename, "w+b");
	if (handle->file == NULL)
		return NULL; /* cannot create file - permission problem */

	/* write the header */
	write_header(handle);

	/* return the handle */
	return handle;
}

/* ----------------------------------------------------------- */
/* Close and remove an IIO file                                */
/* ----------------------------------------------------------- */

void nfs_iio_destroy(nfs_iio_File* file)
{
	char* tempfn = (char*)malloc(strlen(file->filename) + 8);
	if (tempfn)
		strcpy(tempfn, file->filename);
	/* close the file */
	nfs_iio_close(file);
	/* remove the file! */
#ifdef WIN32
	if (tempfn)
		remove(tempfn);
#else
	if (tempfn)
		unlink(tempfn);
#endif
	/* deallocate memory */
	if (tempfn)
		free(tempfn);
}

/* ----------------------------------------------------------- */
/* Open existing IIO file                                      */
/* ----------------------------------------------------------- */

nfs_iio_File* nfs_iio_open(char* filename)
{

	nfs_iio_File* handle;

#ifdef INSTRUMENT_SYSTEM
	instrument_init();
#endif

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	/* first, figure out what the vm page size is, and set the block
	size equal to the page size */
	nfs_iio_BLOCK_SIZEv = get_page_size();
	if (nfs_iio_BLOCK_SIZEv < nfs_iio_BLOCK_SIZE)
		nfs_iio_BLOCK_SIZEv = nfs_iio_BLOCK_SIZE;

	/* create a new file handle and initialize it */
	handle = (nfs_iio_File*)malloc(sizeof(nfs_iio_File));
	if (handle == NULL)
		return NULL; /* not enough memory */
	handle->channels = 0;
	handle->chn      = NULL;

	handle->filename = (char*)malloc(strlen(filename) + 8);
	if (handle->filename == NULL)
		return NULL; /* out of memory */
	strcpy(handle->filename, filename);

	if (nfs_iio_IOMODE & 2) /* write */
		handle->file     = fopen(filename, "r+b");
	else
		handle->file     = fopen(filename, "rb");

	if (handle->file == NULL)
		return NULL; /* file not found */

	/* read the file header */
	if (read_header(handle) < 0)
		return NULL; /* error reading the IIO file header */

	/* return the handle */
	return handle;
}

/* ----------------------------------------------------------- */
/* Close an IIO file, do not remove it                         */
/* ----------------------------------------------------------- */

void nfs_iio_close(nfs_iio_File* file)
{

	int i;

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	/* flush the remaining data */
	flush_data(file);

	/* write the header */
	write_header(file);

	/* auto-truncate file if necessary */
	if (nfs_iio_IOMODE & 2)
		auto_truncate(file);

	/* close the file, mark the handle as being closed */
	fclose(file->file);
	file->file = NULL; /* handle no longer valid */

	/* deallocate channels */
	for (i = 0; i < file->channels; i++)
		if (file->chn[i] != NULL)
		{
			if (file->chn[i]->cache != NULL)
				cache_destroy(file->chn[i]->cache);
			free(file->chn[i]);
		}

	/* deallocate the channel list */
	free(file->chn);

	/* deallocate the file name */
	if (file->filename)
		free(file->filename);

	/* deallocate the handle */
	free(file);

#ifdef INSTRUMENT_SYSTEM
	instrument_done();
#endif

}

/* ----------------------------------------------------------- */
/* Allocate a new channel for the given IIO file, with a       */
/* certain number of blocks per channel                        */
/* ----------------------------------------------------------- */

int nfs_iio_allocate_channel(nfs_iio_File* file, int blocks)
{
	int               chn_no;
	nfs_iio_Channel* chn_ptr;

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	/* check for illegal arguments */
	if (file == NULL)
		return -1;

	/* create new channel & initialize it */
	chn_ptr = (nfs_iio_Channel*)malloc(sizeof(nfs_iio_Channel));
	chn_ptr->blocks = blocks;
	chn_ptr->size   = 0;
	chn_ptr->cp     = 0;
	chn_ptr->cache  = NULL;

	/* allocate new channel in the given IIO file */
	chn_no  = (int)file->channels++;
	file->chn = (nfs_iio_Channel**)realloc(file->chn, sizeof(nfs_iio_Channel*) * (file->channels));

	/* error test */
	if (file->chn == NULL)
		return -1; /* not enough memory */

	/* place new channel in its allocated slot, and return the index */
	file->chn[chn_no] = chn_ptr;

	/* create a cache for this channel */
	cache_create(file, chn_no);

	return chn_no;
}

/* ----------------------------------------------------------- */
/* Write 'size' bytes to the given channel from 'data', return */
/* nr. of bytes written                                        */
/* ----------------------------------------------------------- */

int nfs_iio_write(nfs_iio_File* file, int channel, void* data, int size)
{

	int cblock, cp;
	int start, end, k;

#ifdef INSTRUMENT_SYSTEM
	iio_write_requests++;
#endif

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	if (file->file == NULL)
		return -1;

	k  = 0;
	cp = file->chn[channel]->cp;

	for (cblock = (cp / nfs_iio_BLOCK_SIZEv);
			cblock <= ((cp + size - 1) / nfs_iio_BLOCK_SIZEv);
			cblock++)
	{

#if (nfs_iio_CC_POLICY == ___LRU___)
		nfs_iio_CLOCK++;
#endif

		/* first block in sequence? */
		if (cblock == (cp / nfs_iio_BLOCK_SIZEv))
			start = cp % nfs_iio_BLOCK_SIZEv;
		else
			start = 0;

		/* last block in sequence? */
		if (cblock == ((cp + size - 1) / nfs_iio_BLOCK_SIZEv))
			end   = (cp + size - 1) % nfs_iio_BLOCK_SIZEv;
		else
			end   = nfs_iio_BLOCK_SIZEv - 1;

		/* write (cached) channel block */
		if ((start > 0) || (end < (nfs_iio_BLOCK_SIZEv - 1)))
			cache_write_partial_channel_block(file, channel, cblock, start, end, (char*)data + k);
		else
			cache_write_channel_block(file, channel, cblock, (char*)data + k);

		k += (end - start + 1);

	}

	/* update the channel pointer */
	/* at this point, 'k' should be equal to 'size' */
	file->chn[channel]->cp += k;

	/* update the size of the channel */
	if (file->chn[channel]->size <= file->chn[channel]->cp)
		file->chn[channel]->size = file->chn[channel]->cp;

	return k;
}

/* ----------------------------------------------------------- */
/* Read 'size' bytes from the given channel into 'data',       */
/* return nr. of bytes read                                    */
/* ----------------------------------------------------------- */

int nfs_iio_read(nfs_iio_File* file, int channel, void* data, int size)
{

	int cblock, cp;
	int start, end, k;

#ifdef INSTRUMENT_SYSTEM
	iio_read_requests++;
#endif

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	if (file->file == NULL)
		return -1;

	k = 0;
	cp = file->chn[channel]->cp;

	for (cblock = (cp / nfs_iio_BLOCK_SIZEv);
			cblock <= ((cp + size - 1) / nfs_iio_BLOCK_SIZEv);
			cblock++)
	{

#if (nfs_iio_CC_POLICY == ___LRU___)
		nfs_iio_CLOCK++;
#endif

		/* first block in sequence? */
		if (cblock == (cp / nfs_iio_BLOCK_SIZEv))
			start = cp % nfs_iio_BLOCK_SIZEv;
		else
			start = 0;

		/* last block in sequence? */
		if (cblock == ((cp + size - 1) / nfs_iio_BLOCK_SIZEv))
			end   = (cp + size - 1) % nfs_iio_BLOCK_SIZEv;
		else
			end   = nfs_iio_BLOCK_SIZEv - 1;

		/* read cached channel block */
		if ((start > 0) || (end < (nfs_iio_BLOCK_SIZEv - 1)))
			cache_read_partial_channel_block(file, channel, cblock, start, end, (char*)data + k);
		else
			cache_read_channel_block(file, channel, cblock, (char*)data + k);

		k += (end - start + 1);

	}

	/* update the channel pointer */
	/* at this point, 'k' should be equal to 'size' */
	file->chn[channel]->cp += k;

	return k;

}

/* ----------------------------------------------------------- */
/* Returns the size of a chunk (in blocks) for the given IIO   */
/* file */
/* ----------------------------------------------------------- */

int nfs_iio_blocks_per_chunk(nfs_iio_File* file)
{
	int i;
	int blocks = 0;

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	if (file == NULL)
		return -1;
	/* count the number of blocks for all channels */
	for (i = 0; i < file->channels; i++)
		blocks += file->chn[i]->blocks;
	/* return the number of blocks per chunk */
	return blocks;
}

/* ----------------------------------------------------------- */
/* Returns the channel number 'N' from the given IIO file      */
/* ----------------------------------------------------------- */

nfs_iio_Channel* nfs_iio_get_channel(nfs_iio_File* file, int channel)
{

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	if (file == NULL)
		return NULL;
	if ((channel < 0) || (channel >= (int)file->channels))
		return NULL;
	return file->chn[channel];
}

/* ----------------------------------------------------------- */
/* Returns the current size of a given channel (in bytes)      */
/* ----------------------------------------------------------- */

int nfs_iio_channel_size(nfs_iio_Channel* channel)
{

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	if (channel == NULL)
		return -1;
	return channel->size;
}

/* ----------------------------------------------------------- */
/* Returns the size (in blocks) of the given channel           */
/* ----------------------------------------------------------- */

int nfs_iio_channel_blocks(nfs_iio_Channel* channel)
{

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	if (channel == NULL)
		return -1;
	return channel->blocks;
}


/* ----------------------------------------------------------- */
/* Truncates a channel at the current channel position         */
/* ----------------------------------------------------------- */

int nfs_iio_channel_truncate(nfs_iio_File* file, int channel)
{

#if (nfs_iio_CC_POLICY == ___LRU___)
	nfs_iio_CLOCK++;
#endif

	if (file == NULL)
		return -1;

	if (channel >= file->channels)
		return -1;

	if (file->chn[channel] == NULL)
		return -1;

	file->chn[channel]->size = file->chn[channel]->cp;

	return 0;

}
