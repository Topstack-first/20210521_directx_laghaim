#ifndef _NK_OBJ_QUAD_NODE_H_
#define _NK_OBJ_QUAD_NODE_H_

#include "Config.h"

#include <list>
#include "NKVertexType.h"
#include <Def.h>

using namespace std;


class IndexedTexture;
class CDecoObj;


//-----------------------------------------------------------------------------
// Name : class CNKObjQuadNode
// Desc : The node of a quadtree for object set
//-----------------------------------------------------------------------------
class CNKObjQuadNode
{
protected:
	static LPDIRECT3D7				ms_pD3D;						// The main D3D object
	static LPDIRECT3DDEVICE7		ms_pd3dDevice;					// The D3D rendering device
	static TCHAR*					ms_szWaterFilename;				// Filename of object data to restore water vertex buffer

	// Quad node info
	CNKObjQuadNode*					m_pLLeft;						// Lower left node
	CNKObjQuadNode*					m_pLRight;						// Lower right node
	CNKObjQuadNode*					m_pULeft;						// Upper left node
	CNKObjQuadNode*					m_pURight;						// Upper right node
	D3DVECTOR						m_vMin;							// Min & max vertices for bounding box
	D3DVECTOR						m_vMax;
	static int						ms_nCellSize;					// Width & height of a cell
	static int						ms_nTreeWidth;					// Width & height of object quadtree

	// DecoObj info
	list<CDecoObj*>					m_lstDecoObj;					// DecoObj list
	static CDecoObj*				ms_pOpenDeco;					// DecObj opened
	static D3DVECTOR				ms_vCameraAt;					// At position of the camera
	static D3DVECTOR				ms_vCameraEye;					// Eye position of the camera
	static float					ms_fRenderDist;					// Distance to render
	static float					ms_fBlendDist;					// Distance to blend

	// Water info
	unsigned char					m_nBaseWaterSegment;			// Segment of the base water
	unsigned char					m_nWaveWater1Segment;			// Segment of the wave water 1
	unsigned char					m_nWaveWater2Segment;			// Segment of the wave water 2
	WORD							m_wBaseVBIndex;					// Vertex buffer index of the base water
	WORD							m_wWaveVBIndex;					// Vertex buffer index of the wave water
	static LPDIRECT3DVERTEXBUFFER7	ms_pBaseWaterVB;				// Vertex buffer of the base water
	static LPDIRECT3DVERTEXBUFFER7	ms_pWaveWaterVB;				// Vertex buffer of the wave water
	static DWORD					ms_dwBaseWaterVBSize;			// Size of vertex buffer of the base water
	static DWORD					ms_dwWaveWaterVBSize;			// Size of vertex buffer of the wave water
	static WORD*					ms_pBaseWaterIB;				// Index buffer of the base water
	static WORD*					ms_pWaveWaterIB;				// Index buffer of the wave water
	static DWORD					ms_dwBaseIndexCount;			// Count of vertex indices of the base water
	static DWORD					ms_dwWaveIndexCount;			// Count of vertex indices of the wave water
	static IndexedTexture*			ms_pIndexedTexture;				// Indexed texture for the water
	static int						ms_nBaseWaterTexID;				// Texture index of the base water
	static int						ms_nWaveWater1TexID;			// Texture index of the wave water 1
	static int						ms_nWaveWater2TexID;			// Texture index of the wave water 2
	static float*					ms_pWaterHeights;				// Water height of each node
	static int						ms_nWaterHeightWidth;			// Width of the water heights
	static float					ms_fBaseWaterMove;				// Translation value of a base water texture
	static float					ms_fWaveWater1Move;				// Translation value of a wave water 1 texture
	static float					ms_fWaveWater2Move;				// Translation value of a wave water 2 texture
	static D3DMATRIX				ms_matTexTrans;					// Transform matrix for a translation of the water texture
	static float					ms_fBaseWaterSpeed;				// Speed of the base water
	static float					ms_fWaveWater1Speed;			// Speed of the wave water 1
	static float					ms_fWaveWater2Speed;			// Speed of the wave water 2
	static int						ms_nBaseWaterSize;				// Width of the base water


protected:
	static bool	InitWaterVertexBuffer();
	static bool InitWaterVertexBufferInSysMem( NKVERTEXLT* pTempBaseVB, NKVERTEXLT* pTempWaveVB );

public:
	CNKObjQuadNode();
	CNKObjQuadNode( int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ, PFILE* fp );
	virtual ~CNKObjQuadNode();

	static bool	LoadObj( const TCHAR* szFilepath, const TCHAR* szFilename, IndexedTexture* pIndexedTexture );
	static bool	LoadWat( const TCHAR* szFilename );
	static void	Destroy();
	static void	PrepareRender();
	void	Render( int nCullingState );
	static void	RenderWater();
	static void	SetD3DDevice();
	static HRESULT	RestoreDeviceObjects();
	static HRESULT	InvalidateDeviceObjects();

	static void	SetOpenDeco( CDecoObj* pDecoObj );
	static CDecoObj*	GetOpenDeco();
	static void	SetRenderDist( float fRenderDist );
	static void	SetBlendDist( float fBlendDist );
	static float	GetRenderDist();
	static void SetCameraAtPos( D3DVECTOR& rAt );
	static void SetCameraEyePos( D3DVECTOR& rEye );
	static void	SetBaseWaterSpeed( float fSpeed );
	static void	SetWaveWater1Speed( float fSpeed );
	static void	SetWaveWater2Speed( float fSpeed );
	static float	GetHeightOnWater( float fZ, float fX );
	static float	GetHeightOnObject( float fZ, float fX, float fY );
};


// Inline functions
//-----------------------------------------------------------------------------
// Name : SetOpenDeco()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetOpenDeco( CDecoObj* pOpenDeco )
{
	ms_pOpenDeco = pOpenDeco;
}


//-----------------------------------------------------------------------------
// Name : GetOpenDeco()
// Desc :
//-----------------------------------------------------------------------------
inline CDecoObj* CNKObjQuadNode::GetOpenDeco()
{
	return ms_pOpenDeco;
}


//-----------------------------------------------------------------------------
// Name : SetBlendDist()
// Desc :
//-----------------------------------------------------------------------------
inline void CNKObjQuadNode::SetBlendDist( float fBlendDist )
{
	ms_fBlendDist = fBlendDist;
}


//-----------------------------------------------------------------------------
// Name : GetRenderDist()
// Desc :
//-----------------------------------------------------------------------------
inline float CNKObjQuadNode::GetRenderDist()
{
	return ms_fRenderDist;
}


//-----------------------------------------------------------------------------
// Name : SetBaseWaterSpeed()
// Desc :
//-----------------------------------------------------------------------------
inline void	CNKObjQuadNode::SetBaseWaterSpeed( float fSpeed )
{
	ms_fBaseWaterSpeed = fSpeed;
}


//-----------------------------------------------------------------------------
// Name : SetWaveWater1Speed()
// Desc :
//-----------------------------------------------------------------------------
inline void	CNKObjQuadNode::SetWaveWater1Speed( float fSpeed )
{
	ms_fWaveWater1Speed = fSpeed;
}


//-----------------------------------------------------------------------------
// Name : SetWaveWater2Speed()
// Desc :
//-----------------------------------------------------------------------------
inline void	CNKObjQuadNode::SetWaveWater2Speed( float fSpeed )
{
	ms_fWaveWater2Speed = fSpeed;
}


//-----------------------------------------------------------------------------
// Name : SetCameraAtPos()
// Desc :
//-----------------------------------------------------------------------------
inline void	CNKObjQuadNode::SetCameraAtPos( D3DVECTOR& rAt )
{
	ms_vCameraAt = rAt;
}


//-----------------------------------------------------------------------------
// Name : SetCameraEyePos()
// Desc :
//-----------------------------------------------------------------------------
inline void	CNKObjQuadNode::SetCameraEyePos( D3DVECTOR& rEye )
{
	ms_vCameraEye = rEye;
}


#endif // _NK_OBJ_QUAD_NODE_H_