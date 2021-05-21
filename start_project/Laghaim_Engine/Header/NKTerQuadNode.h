#ifndef _NKTERQUADNODE_H_
#define _NKTERQUADNODE_H_

#include "NKVertexType.h"

class CLand;
class CFrustum;
class CNKTerQuadNode
{
public:
	CNKTerQuadNode();
	CNKTerQuadNode( int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ,
					unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID );
	virtual ~CNKTerQuadNode();

	void	InitVertexBuffer( int nLeftX, int nLeftZ, int nRightX, int nRightZ,
							  int nOffsetX, int nOffsetZ, NKVERTEXT2* pVB );
	void	Render( CFrustum *pFrustum, int nCullingState );
	void	GetPickedPoint( const D3DVECTOR& rOrigin, const D3DVECTOR& rDir, float& rDist );

public:
	static CLand*		ms_pTerrain;						// Static variable to access index buffer of CLand
	static float		ms_fdt;								// Offset of texture coordinate
	static int			ms_nVBIdxCnt;						// Vertex buffer index count

protected:
	CNKTerQuadNode*		m_pLLeft;							// Lower left node
	CNKTerQuadNode*		m_pLRight;							// Lower right node
	CNKTerQuadNode*		m_pULeft;							// Upper left node
	CNKTerQuadNode*		m_pURight;							// Upper right node
	D3DVECTOR			m_vMin;								// Min & max vertices for bounding box
	D3DVECTOR			m_vMax;
	WORD				m_wVBIndex;							// Vertex buffer index
	unsigned char		m_nL1TexID;							// Texture ID of layer 1
	unsigned char		m_nL2TexID;							// Texture ID of layer 2
	unsigned char		m_nL3TexID;							// Texture ID of layer 3
};

#endif // _NKTERQUADNODE_H_