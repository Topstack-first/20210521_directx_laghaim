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

#ifndef _nfs_dt_
#define _nfs_dt_

#include <stdio.h>
#include "nfs_iio.h"
#include "nfs_pmatch.h"

/* Definitions */

#define nfs_dt_MAX_SUBNAME_SIZE     256  /* Max size of component part */
#define nfs_dt_DEF_CHANNEL1_SIZE    1    /* Default channel size, in blocks */
#define nfs_dt_DEF_CHANNEL2_SIZE    2    /* Default channel size, in blocks */

/* Types */

typedef struct _nfs_dt_DT {
    nfs_iio_File*   file;                   /* IIO file */
    int             channel1;               /* IIO file channel #1 (Patricia nodes) */
    int             channel2;               /* IIO file channel #2 (keys) */
    int             unallocated;            /* head of free chain (internal) */
    int             unallocated2;           /* head of free chain for channel1 (keynodes) */
} nfs_dt_DT;

/* Exported functions - prototypes */

/* Create a new directory table in the given iio file & channel */
nfs_dt_DT*     nfs_dt_create (nfs_iio_File* file, int blocks_per_channel1, int blocks_per_channel2);

/* Open a directory table */
nfs_dt_DT*     nfs_dt_open   (nfs_iio_File* file, int channel1, int channel2);

/* Close the directory table */
int            nfs_dt_close  (nfs_dt_DT* dt);

/* Destroy the directory table */
int            nfs_dt_destroy(nfs_dt_DT* dt);

/* Add new file name, return index in table */
int            nfs_dt_filename_add(nfs_dt_DT* dt, char* filename);

/* Delete file name given by its index in table */
int            nfs_dt_filename_delete(nfs_dt_DT* dt, char* filename);

/* Lookup file name in DT, return index in DT */
int            nfs_dt_filename_lookup(nfs_dt_DT* dt, char* filename);

/* Globbing (pattern matching): lookup those names that match the given pattern and
for each of those names call the given callback */
int            nfs_dt_filename_glob(nfs_dt_DT* dt,
                                    char* pattern,
                                    int flags,
                                    int (*p)(nfs_dt_DT* dt,
                                    char* filename,
                                    int filename_idx,
                                    void* tmp),
                                    void* propagate);

/* Get the NT index associated with a given filename (given by its index in the DT) */
int            nfs_dt_filename_get_nt_index(nfs_dt_DT* dt, int filename_idx);

/* Set the NT index associated with a given filename (given by its index in the DT) */
int            nfs_dt_filename_set_nt_index(nfs_dt_DT* dt, int filename_idx, int nt_idx);

/* Get the filename(char*) associated with a given filename (given by its index in the DT) */
int            nfs_dt_filename_get_name(nfs_dt_DT* dt, int filename_idx, char* buffer);

#endif

