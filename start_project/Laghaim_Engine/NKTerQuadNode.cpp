#include "stdafx.h"
#define D3D_OVERLOADS
#include "NKTerQuadNode.h"
#include "Land.h"
#include "Frustum.h"
#include "NKMath.h"

// External global variables
extern int		gMapWidth;
extern int		gMapHeight;
extern int		gHFWidth;
extern int		gHFHeight;
extern int		gMapExtention;
extern float	gfMapExtention;
extern int		gHeightScale;

// Static member variables
int		CNKTerQuadNode::ms_nVBIdxCnt = 0;
CLand*	CNKTerQuadNode::ms_pTerrain = NULL;
float	CNKTerQuadNode::ms_fdt = 0.0f;


CNKTerQuadNode::CNKTerQuadNode()
{
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
	m_nL1TexID = 0;
	m_nL2TexID = 0;
	m_nL3TexID = 0;
}

CNKTerQuadNode::CNKTerQuadNode( int nLevel, int nLeftX, int nLeftZ,	int nRightX, int nRightZ,
								unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID )
{
	int		nCenterX, nCenterZ;
	float	fMinY, fMaxY;

	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;

	if( nLevel < 1 )			// Lowest level node
	{
		int	nX, nZ, nMinY, nMaxY, nTemp;
		// Calculate min & max heights
		nMinY = nMaxY = ms_pTerrain->m_pHeightMap[nLeftX + nLeftZ * gHFWidth] * gHeightScale;
		for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
		{
			for( nX = nLeftX; nX <= nRightX; nX++ )
			{
				nTemp = ms_pTerrain->m_pHeightMap[nX + nZ * gHFWidth] * gHeightScale;
				if( nMinY > nTemp ) nMinY = nTemp;
				if( nMaxY < nTemp ) nMaxY = nTemp;
			}
		}

		// Set a bounding box of lowest level node
		m_vMin = D3DVECTOR( (float)( nLeftX * gfMapExtention ), (float)nMinY, (float)( nLeftZ * gfMapExtention ) );
		m_vMax = D3DVECTOR( (float)( nRightX * gfMapExtention ), (float)nMaxY, (float)( nRightZ * gfMapExtention ) );

		// Set a texture ID
		m_nL1TexID = pL1TexID[nLeftX + nLeftZ * gMapWidth];
		m_nL2TexID = pL2TexID[nLeftX + nLeftZ * gMapWidth];
		m_nL3TexID = pL3TexID[nLeftX + nLeftZ * gMapWidth];

		return ;
	}

	// Prepare for child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKTerQuadNode( nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ, pL1TexID, pL2TexID, pL3TexID );
	m_pLRight = new CNKTerQuadNode( nLevel, nCenterX, nLeftZ, nRightX, nCenterZ, pL1TexID, pL2TexID, pL3TexID );
	m_pULeft = new CNKTerQuadNode( nLevel, nLeftX, nCenterZ, nCenterX, nRightZ, pL1TexID, pL2TexID, pL3TexID );
	m_pURight = new CNKTerQuadNode( nLevel, nCenterX, nCenterZ, nRightX, nRightZ, pL1TexID, pL2TexID, pL3TexID );

	// Get min & max heights of lower level node
	fMinY = m_pLLeft->m_vMin.y;
	fMaxY = m_pLLeft->m_vMax.y;

	// Calculate min & max heights of current level node
	if( fMinY > m_pLRight->m_vMin.y ) fMinY = m_pLRight->m_vMin.y;
	if( fMinY > m_pULeft->m_vMin.y ) fMinY = m_pULeft->m_vMin.y;
	if( fMinY > m_pURight->m_vMin.y ) fMinY = m_pURight->m_vMin.y;

	if( fMaxY < m_pLRight->m_vMax.y ) fMaxY = m_pLRight->m_vMax.y;
	if( fMaxY < m_pULeft->m_vMax.y ) fMaxY = m_pULeft->m_vMax.y;
	if( fMaxY < m_pURight->m_vMax.y ) fMaxY = m_pURight->m_vMax.y;

	// Set a bounding box of current level node
	m_vMin = D3DVECTOR( (float)( nLeftX * gMapExtention ), fMinY, (float)( nLeftZ * gMapExtention ) );
	m_vMax = D3DVECTOR( (float)( nRightX * gMapExtention ), fMaxY, (float)( nRightZ * gMapExtention ) );
}

CNKTerQuadNode::~CNKTerQuadNode()
{
	if( m_pLLeft != NULL )
	{
		delete m_pLLeft;
		delete m_pLRight;
		delete m_pULeft;
		delete m_pURight;
	}
}

void CNKTerQuadNode::InitVertexBuffer( int nLeftX, int nLeftZ, int nRightX, int nRightZ,
									   int nOffsetX, int nOffsetZ, NKVERTEXT2* pVB )
{
	int	nCenterX, nCenterZ;

	// Prepare for child node
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	if( m_pLLeft->m_pLLeft == NULL )		// If this node is lowest level node
	{
		int	nX, nZ;

		// Vertex buffer data
		pVB[ms_nVBIdxCnt].x = pVB[ms_nVBIdxCnt+3].x = pVB[ms_nVBIdxCnt+6].x =
								  (float)( nLeftX * gMapExtention );
		pVB[ms_nVBIdxCnt+1].x = pVB[ms_nVBIdxCnt+4].x = pVB[ms_nVBIdxCnt+7].x =
									(float)( nCenterX * gMapExtention );
		pVB[ms_nVBIdxCnt+2].x = pVB[ms_nVBIdxCnt+5].x = pVB[ms_nVBIdxCnt+8].x =
									(float)( nRightX * gMapExtention );

		pVB[ms_nVBIdxCnt].z = pVB[ms_nVBIdxCnt+1].z = pVB[ms_nVBIdxCnt+2].z =
								  (float)( nLeftZ * gMapExtention );
		pVB[ms_nVBIdxCnt+3].z = pVB[ms_nVBIdxCnt+4].z = pVB[ms_nVBIdxCnt+5].z =
									(float)( nCenterZ * gMapExtention );
		pVB[ms_nVBIdxCnt+6].z = pVB[ms_nVBIdxCnt+7].z = pVB[ms_nVBIdxCnt+8].z =
									(float)( nRightZ * gMapExtention );

		pVB[ms_nVBIdxCnt].u1 = pVB[ms_nVBIdxCnt+3].u1 = pVB[ms_nVBIdxCnt+6].u1 = 0.0f;
		pVB[ms_nVBIdxCnt+1].u1 = pVB[ms_nVBIdxCnt+4].u1 = pVB[ms_nVBIdxCnt+7].u1 = 0.5f;
		pVB[ms_nVBIdxCnt+2].u1 = pVB[ms_nVBIdxCnt+5].u1 = pVB[ms_nVBIdxCnt+8].u1 = 1.0f;
		pVB[ms_nVBIdxCnt].v1 = pVB[ms_nVBIdxCnt+1].v1 = pVB[ms_nVBIdxCnt+2].v1 = 1.0f;
		pVB[ms_nVBIdxCnt+3].v1 = pVB[ms_nVBIdxCnt+4].v1 = pVB[ms_nVBIdxCnt+5].v1 = 0.5f;
		pVB[ms_nVBIdxCnt+6].v1 = pVB[ms_nVBIdxCnt+7].v1 = pVB[ms_nVBIdxCnt+8].v1 = 0.0f;

		int nWidth = 1 << ms_pTerrain->m_nQuadTreeDepth;
		pVB[ms_nVBIdxCnt].u2 = pVB[ms_nVBIdxCnt+3].u2 = pVB[ms_nVBIdxCnt+6].u2 =
								   (float)( ( ( nLeftX + nWidth * nOffsetX ) << 1 ) ) * ms_fdt;
		pVB[ms_nVBIdxCnt+1].u2 = pVB[ms_nVBIdxCnt+4].u2 = pVB[ms_nVBIdxCnt+7].u2 =
									 (float)( ( ( nCenterX + nWidth * nOffsetX ) << 1 ) ) * ms_fdt;
		pVB[ms_nVBIdxCnt+2].u2 = pVB[ms_nVBIdxCnt+5].u2 = pVB[ms_nVBIdxCnt+8].u2 =
									 (float)( ( ( nRightX + nWidth * nOffsetX ) << 1 ) ) * ms_fdt;
		pVB[ms_nVBIdxCnt].v2 = pVB[ms_nVBIdxCnt+1].v2 = pVB[ms_nVBIdxCnt+2].v2 =
								   (float)( ( ( nLeftZ + nWidth * nOffsetZ ) << 1 ) ) * ms_fdt;
		pVB[ms_nVBIdxCnt+3].v2 = pVB[ms_nVBIdxCnt+4].v2 = pVB[ms_nVBIdxCnt+5].v2 =
									 (float)( ( ( nCenterZ + nWidth * nOffsetZ ) << 1 ) ) * ms_fdt;
		pVB[ms_nVBIdxCnt+6].v2 = pVB[ms_nVBIdxCnt+7].v2 = pVB[ms_nVBIdxCnt+8].v2 =
									 (float)( ( ( nRightZ + nWidth* nOffsetZ ) << 1 ) ) * ms_fdt;

		int nCount = 0;
		for( nZ = nLeftZ; nZ <= nRightZ; nZ++ )
		{
			for( nX = nLeftX; nX <= nRightX; nX++)
			{
				pVB[ms_nVBIdxCnt + nCount].y = (float)( ms_pTerrain->m_pHeightMap[nX + nZ * gHFWidth] * gHeightScale );
				nCount++;
			}
		}

		// Vertex buffer index
		m_pLLeft->m_wVBIndex = ms_nVBIdxCnt;
		m_pLRight->m_wVBIndex = ms_nVBIdxCnt + 1;
		m_pULeft->m_wVBIndex = ms_nVBIdxCnt + 3;
		m_pURight->m_wVBIndex = ms_nVBIdxCnt + 4;
		ms_nVBIdxCnt += 9;

		return ;
	}

	// Recursive call
	m_pLLeft->InitVertexBuffer( nLeftX, nLeftZ, nCenterX, nCenterZ, nOffsetX, nOffsetZ, pVB );
	m_pLRight->InitVertexBuffer( nCenterX, nLeftZ, nRightX, nCenterZ, nOffsetX, nOffsetZ, pVB );
	m_pULeft->InitVertexBuffer( nLeftX, nCenterZ, nCenterX, nRightZ, nOffsetX, nOffsetZ, pVB );
	m_pURight->InitVertexBuffer( nCenterX, nCenterZ, nRightX, nRightZ, nOffsetX, nOffsetZ, pVB );
}

void CNKTerQuadNode::Render( CFrustum* pFrustum, int nCullingState )
{
	if( nCullingState != VFCULL_INSIDE )
	{
		nCullingState = pFrustum->CullAABB( m_vMin, m_vMax );
		if( nCullingState == VFCULL_OUTSIDE )
			return ;
	}

	// Render if this node is lowest level node
	if( m_pLLeft == NULL )
	{
		DWORD dwCount;

		dwCount = ms_pTerrain->m_pTerIBCountL1[m_nL1TexID];
		ms_pTerrain->m_ppTerIBL1[m_nL1TexID][dwCount++] = m_wVBIndex + 3;
		ms_pTerrain->m_ppTerIBL1[m_nL1TexID][dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_ppTerIBL1[m_nL1TexID][dwCount++] = m_wVBIndex;
		ms_pTerrain->m_ppTerIBL1[m_nL1TexID][dwCount++] = m_wVBIndex;
		ms_pTerrain->m_ppTerIBL1[m_nL1TexID][dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_ppTerIBL1[m_nL1TexID][dwCount++] = m_wVBIndex + 1;
		ms_pTerrain->m_pTerIBCountL1[m_nL1TexID] = dwCount;

		dwCount = ms_pTerrain->m_pTerIBCountL2[m_nL2TexID];
		ms_pTerrain->m_ppTerIBL2[m_nL2TexID][dwCount++] = m_wVBIndex + 3;
		ms_pTerrain->m_ppTerIBL2[m_nL2TexID][dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_ppTerIBL2[m_nL2TexID][dwCount++] = m_wVBIndex;
		ms_pTerrain->m_ppTerIBL2[m_nL2TexID][dwCount++] = m_wVBIndex;
		ms_pTerrain->m_ppTerIBL2[m_nL2TexID][dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_ppTerIBL2[m_nL2TexID][dwCount++] = m_wVBIndex + 1;
		ms_pTerrain->m_pTerIBCountL2[m_nL2TexID] = dwCount;

		dwCount = ms_pTerrain->m_pTerIBCountL3[m_nL3TexID];
		ms_pTerrain->m_ppTerIBL3[m_nL3TexID][dwCount++] = m_wVBIndex + 3;
		ms_pTerrain->m_ppTerIBL3[m_nL3TexID][dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_ppTerIBL3[m_nL3TexID][dwCount++] = m_wVBIndex;
		ms_pTerrain->m_ppTerIBL3[m_nL3TexID][dwCount++] = m_wVBIndex;
		ms_pTerrain->m_ppTerIBL3[m_nL3TexID][dwCount++] = m_wVBIndex + 4;
		ms_pTerrain->m_ppTerIBL3[m_nL3TexID][dwCount++] = m_wVBIndex + 1;
		ms_pTerrain->m_pTerIBCountL3[m_nL3TexID] = dwCount;

		return ;
	}

	// Recursive call
	m_pLLeft->Render( pFrustum, nCullingState );
	m_pLRight->Render( pFrustum, nCullingState );
	m_pULeft->Render( pFrustum, nCullingState );
	m_pURight->Render( pFrustum, nCullingState );
}

void CNKTerQuadNode::GetPickedPoint( const D3DVECTOR& rOrigin, const D3DVECTOR& rDir, float& rDist )
{
	if( !NKIntersectRayAABB( rOrigin, rDir, m_vMin, m_vMax ) )
		return ;

	// Test a intersection between a ray and a triangle if this node is lowest level node
	if( m_pLLeft == NULL )
	{
		D3DVECTOR	vP0, vP1, vP2;
		float		fDist2;
		int			nMinX, nMinZ, nMaxX, nMaxZ;

		nMinX = (int)( m_vMin.x / gfMapExtention );
		nMinZ = (int)( m_vMin.z / gfMapExtention );
		nMaxX = nMinX + 1;
		nMaxZ = nMinZ + 1;

		// Upper left triangle ( vP0, vP1, vP2 )
		vP0.x = m_vMin.x;
		vP0.z = m_vMax.z;
		vP0.y = (float)( ms_pTerrain->m_pHeightMap[nMinX + nMaxZ * gHFWidth] * gHeightScale );
		vP1.x = m_vMax.x;
		vP1.z = m_vMax.z;
		vP1.y = (float)( ms_pTerrain->m_pHeightMap[nMaxX + nMaxZ * gHFWidth] * gHeightScale );
		vP2.x = m_vMin.x;
		vP2.z = m_vMin.z;
		vP2.y = (float)( ms_pTerrain->m_pHeightMap[nMinX + nMinZ * gHFWidth] * gHeightScale );
		if( NKIntersectRayTri( rOrigin, rDir, vP0, vP1, vP2, &fDist2 ) )
		{
			if( fDist2 < rDist )
				rDist = fDist2;
		}

		// Lower right triangle ( vP2, vP1, vP0 )
		vP0.x = m_vMax.x;
		vP0.z = m_vMin.z;
		vP0.y = (float)( ms_pTerrain->m_pHeightMap[nMaxX + nMinZ * gHFWidth] * gHeightScale );
		if( NKIntersectRayTri( rOrigin, rDir, vP2, vP1, vP0, &fDist2 ) )
		{
			if( fDist2 < rDist )
				rDist = fDist2;
		}

		return ;
	}

	// Recursive call
	m_pLLeft->GetPickedPoint( rOrigin, rDir, rDist );
	m_pLRight->GetPickedPoint( rOrigin, rDir, rDist );
	m_pULeft->GetPickedPoint( rOrigin, rDir, rDist );
	m_pURight->GetPickedPoint( rOrigin, rDir, rDist );
}
