#include "stdafx.h"
//-----------------------------------------------------------------------------
// File : NKMath.cpp
// Desc : 2004 addition
//-----------------------------------------------------------------------------


#include "NKMath.h"


//-----------------------------------------------------------------------------
// Name : NKIntersectRayAABB()
// Desc : Test a intersection between a ray & a AABB
//        http://www.codercorner.com/RayAABB.cpp
//-----------------------------------------------------------------------------
bool NKIntersectRayAABB( const D3DVECTOR& rOrigin, const D3DVECTOR& rDir,
						 const D3DVECTOR& rMin, const D3DVECTOR& rMax )
{
	bool		bInside = true;
	float		fMaxT[3];
	D3DVECTOR	vCoord;

	fMaxT[0] = fMaxT[1] = fMaxT[2] = -1.0f;

	// Find a candidate plane
	if( rOrigin.x < rMin.x )
	{
		bInside = false;
		vCoord.x = rMin.x;
		if( rDir.x != 0 ) fMaxT[0] = ( rMin.x - rOrigin.x ) / rDir.x;
	}
	else if( rOrigin.x > rMax.x )
	{
		bInside = false;
		vCoord.x = rMax.x;
		if( rDir.x != 0 ) fMaxT[0] = ( rMax.x - rOrigin.x ) / rDir.x;
	}

	if( rOrigin.y < rMin.y )
	{
		bInside = false;
		vCoord.y = rMin.y;
		if( rDir.y != 0 ) fMaxT[1] = ( rMin.y - rOrigin.y ) / rDir.y;
	}
	else if( rOrigin.y > rMax.y )
	{
		bInside = false;
		vCoord.y = rMax.y;
		if( rDir.y != 0 ) fMaxT[1] = ( rMax.y - rOrigin.y ) / rDir.y;
	}

	if( rOrigin.z < rMin.z )
	{
		bInside = false;
		vCoord.z = rMin.z;
		if( rDir.z != 0 ) fMaxT[2] = ( rMin.z - rOrigin.z ) / rDir.z;
	}
	else if( rOrigin.z > rMax.z )
	{
		bInside = false;
		vCoord.z = rMax.z;
		if( rDir.z != 0 ) fMaxT[2] = ( rMax.z - rOrigin.z ) / rDir.z;
	}

	// If origin is inside AABB
	if( bInside )
		return true;

	// Get a largest value among MaxT
	int	nWhichPlane = 0;
	if( fMaxT[1] > fMaxT[nWhichPlane] ) nWhichPlane = 1;
	if( fMaxT[2] > fMaxT[nWhichPlane] ) nWhichPlane = 2;

	if( fMaxT[nWhichPlane] < 0 )
		return false;

	if( nWhichPlane != 0 )
	{
		vCoord.x = rOrigin.x + fMaxT[nWhichPlane] * rDir.x;
		if( vCoord.x < rMin.x - NK_EPSILON4 || vCoord.x > rMax.x + NK_EPSILON4 )
			return false;
	}

	if( nWhichPlane != 1 )
	{
		vCoord.y = rOrigin.y + fMaxT[nWhichPlane] * rDir.y;
		if( vCoord.y < rMin.y - NK_EPSILON4 || vCoord.y > rMax.y + NK_EPSILON4 )
			return false;
	}

	if( nWhichPlane != 2 )
	{
		vCoord.z = rOrigin.z + fMaxT[nWhichPlane] * rDir.z;
		if( vCoord.z < rMin.z - NK_EPSILON4 || vCoord.z > rMax.z + NK_EPSILON4 )
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name : NKIntersectTriangle()
// Desc : Test a intersection & find a intersection point
//        between a ray & a triangle
//-----------------------------------------------------------------------------
bool NKIntersectRayTri( const D3DVECTOR& rOrig, const D3DVECTOR& rDir, D3DVECTOR& rP0,
						D3DVECTOR& rP1, D3DVECTOR& rP2, float* fT )
{
	float	fU, fV;
	// Find vectors for two edges sharing vert0
	D3DVECTOR	vEdge1 = rP1 - rP0;
	D3DVECTOR	vEdge2 = rP2 - rP0;

	// Begin calculating determinant - also used to calculate U parameter
	D3DVECTOR	vP = CrossProduct( rDir, vEdge2 );

	// If determinant is near zero, ray lies in plane of a triangle
	float	fDet = DotProduct( vEdge1, vP );

	D3DVECTOR	vT;
	if( fDet > 0 )
	{
		vT = rOrig - rP0;
	}
	else
	{
		vT = rP0 - rOrig;
		fDet = -fDet;
	}

	if( fDet < NK_EPSILON4 )
		return false;

	// Calculate U parameter and test bounds
	fU = DotProduct( vT, vP );
	if( fU < 0.0f || fU > fDet )
		return false;

	// Prepare to test V parameter
	D3DVECTOR	vQ = CrossProduct( vT, vEdge1 );

	// Calculate V parameter and test bounds
	fV = DotProduct( rDir, vQ );
	if( fV < 0.0f || fU + fV > fDet )
		return false;

	// Calculate t, a ray intersects a triangle
	*fT = DotProduct( vEdge2, vQ )/ fDet;

	return true;
}
