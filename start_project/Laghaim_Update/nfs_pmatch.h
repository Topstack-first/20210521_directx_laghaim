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
// File:    nfs_pmatch.h
// Date:    07/08/02
// Content: Pattern matching for file names
//
//----------------------------------------------------------------------------

#ifndef _nfs_pmatch_
#define _nfs_pmatch_

/* Bits set in the FLAGS argument to `fnmatch'.  */
#ifdef  FNM_PATHNAME
#undef  FNM_PATHNAME
#endif
#define	FNM_PATHNAME	(0 << 0) /* Wildcard can match `/' */

#ifdef  FNM_NOESCAPE
#undef  FNM_NOESCAPE
#endif
#define	FNM_NOESCAPE	(1 << 1) /* Backslashes don't quote special chars */

#ifdef  FNM_PERIOD
#undef  FNM_PERIOD
#endif
#define	FNM_PERIOD	(1 << 2) /* Leading `.' is matched only explicitly */

#ifdef  __FNM_FLAGS
#undef  __FNM_FLAGS
#endif
#define	__FNM_FLAGS	(FNM_PATHNAME | FNM_NOESCAPE | FNM_PERIOD)

/* Value returned by 'fnmatch' if STRING does not match PATTERN.  */
#ifdef  FNM_NOMATCH
#undef  FNM_NOMATCH
#endif
#define	FNM_NOMATCH	1

/* Match STRING against the filename pattern PATTERN,
   returning zero if it matches, FNM_NOMATCH if not.  */
int nfs_pmatch(char *pattern, char *string, int flags);

#endif

