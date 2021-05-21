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
// File:    nfs_pmatch.c
// Date:    07/08/02
// Content: Pattern matching for file names
//
//----------------------------------------------------------------------------

#include "nfs_pmatch.h"

#if defined (__CYGWIN32__) || defined (WIN32)
#define FOLD_FN_CHAR(c) ( ((c < 'A') || (c > 'Z')) ? (c) : (c - 'A' + 'a') )
#else
#define FOLD_FN_CHAR(c) (c)
#endif

/* Match STRING against the filename pattern PATTERN, returning zero if
it matches, nonzero if not.  */

int nfs_pmatch(char *pattern, char *string, int flags)
{

	register char *p = pattern, *n = string;
	register char c;

	if ((flags & ~__FNM_FLAGS) != 0)
	{
		/*
		nfs_errno = nfs_EINVAL;
		    */
		return -1;
	}

	while ((c = *p++) != '\0')
	{

		switch (c)
		{

		case '?':
			if (*n == '\0')
				return FNM_NOMATCH;
			else if ((flags & FNM_PATHNAME) && *n == '/')
				return FNM_NOMATCH;
			else if ((flags & FNM_PERIOD) && *n == '.' &&
					 (n == string || ((flags & FNM_PATHNAME) && n[-1] == '/')))
				return FNM_NOMATCH;
			break;

		case '\\':
			if (!(flags & FNM_NOESCAPE))
				c = *p++;
			if (FOLD_FN_CHAR (*n) != FOLD_FN_CHAR (c))
				return FNM_NOMATCH;
			break;

		case '*':
			if ((flags & FNM_PERIOD) && *n == '.' &&
					(n == string || ((flags & FNM_PATHNAME) && n[-1] == '/')))
				return FNM_NOMATCH;

			for (c = *p++; c == '?' || c == '*'; c = *p++, ++n)
				if (((flags & FNM_PATHNAME) && *n == '/') ||
						(c == '?' && *n == '\0'))
					return FNM_NOMATCH;

			if (c == '\0')
				return 0;

			{
				char c1 = (!(flags & FNM_NOESCAPE) && c == '\\') ? *p : c;
				for (--p; *n != '\0'; ++n)
					if ((c == '[' || FOLD_FN_CHAR (*n) == FOLD_FN_CHAR (c1)) &&
							nfs_pmatch(p, n, flags & ~FNM_PERIOD) == 0)
						return 0;
				return FNM_NOMATCH;
			}

		case '[':
			{
				/* Nonzero if the sense of the character class is inverted.  */
				register int not;

				if (*n == '\0')
					return FNM_NOMATCH;

				if ((flags & FNM_PERIOD) && *n == '.' &&
						(n == string || ((flags & FNM_PATHNAME) && n[-1] == '/')))
					return FNM_NOMATCH;

				not = (*p == '!' || *p == '^');
				if (not)
					++p;

				c = *p++;
				for (;;)
				{
					register char cstart = c, cend = c;

					if (!(flags & FNM_NOESCAPE) && c == '\\')
						cstart = cend = *p++;

					if (c == '\0')
						/* [ (unterminated) loses.  */
						return FNM_NOMATCH;

					c = *p++;

					if ((flags & FNM_PATHNAME) && c == '/')
						/* [/] can never match.  */
						return FNM_NOMATCH;

					if (c == '-' && *p != ']')
					{
						cend = *p++;
						if (!(flags & FNM_NOESCAPE) && cend == '\\')
							cend = *p++;
						if (cend == '\0')
							return FNM_NOMATCH;
						c = *p++;
					}

					if (*n >= cstart && *n <= cend)
						goto matched;

					if (c == ']')
						break;
				}
				if (!not)
					return FNM_NOMATCH;
				break;

matched:
				;
				/* Skip the rest of the [...] that already matched.  */
				while (c != ']')
				{
					if (c == '\0')
						/* [... (unterminated) loses.  */
						return FNM_NOMATCH;

					c = *p++;
					if (!(flags & FNM_NOESCAPE) && c == '\\')
						/* 1003.2d11 is unclear if this is right.  %%% */
						++p;
				}
				if (not)
					return FNM_NOMATCH;
			}
			break;

		default:
			if (FOLD_FN_CHAR (c) != FOLD_FN_CHAR (*n))
				return FNM_NOMATCH;
		}

		++n;
	}

	if (*n == '\0')
		return 0;

	return FNM_NOMATCH;
}
