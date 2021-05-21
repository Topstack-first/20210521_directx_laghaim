//-----------------------------------------------------------------------------
// File : NKMath.h
// Desc : 2004 addition
//-----------------------------------------------------------------------------

#ifndef _NKMATH_H_
#define _NKMATH_H_

#define D3D_OVERLOADS


#include <d3d.h>
#include <math.h>


//-----------------------------------------------------------------------------
// Useful constants
//-----------------------------------------------------------------------------
// PI
const float	NK_PI = 4.0f * atanf( 1.0f );
const float	NK_TWOPI = 2.0f * NK_PI;
const float	NK_HALFPI = 0.5f * NK_PI;
const float	NK_QUATERPI = atanf( 1.0f );
const float	NK_DEGTORAD = NK_PI / 180.0f;
const float	NK_RADTODEG = 180.0f / NK_PI;

// Epsilon
const float NK_EPSILON1 = 1e-1f;
const float NK_EPSILON2 = 1e-2f;
const float NK_EPSILON3 = 1e-3f;
const float NK_EPSILON4 = 1e-4f;
const float NK_EPSILON5 = 1e-5f;
const float NK_EPSILON6 = 1e-6f;


//-----------------------------------------------------------------------------
// Intersection functions
//-----------------------------------------------------------------------------
bool	NKIntersectRayAABB( const D3DVECTOR& rOrigin, const D3DVECTOR& rDir,	// Test a intersection between a ray & a AABB
							const D3DVECTOR& rMin, const D3DVECTOR& rMax );

bool	NKIntersectRayTri( const D3DVECTOR& rOrig, const D3DVECTOR& rDir,		// Test a intersection & find a intersection point
						   D3DVECTOR& rP0, D3DVECTOR& rP1, D3DVECTOR& rP2,			// between a ray & a triangle
						   float* fT );


#endif // _NKMATH_H_