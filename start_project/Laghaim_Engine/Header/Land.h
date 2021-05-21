#ifndef _LAND_H_
#define _LAND_H_

#include "Config.h"

#include <d3d.h>
#include "Capsyong.h"
#include "NKVertexType.h"

#define LAND_VISIBLE_DEPTH	5
#define LAND_VISIBLE_WIDTH	64
#define LAND_VISIBLE_HEIGHT	64
	    
#define WORLD_FIELD					0
#define WORLD_DUNGEON1				1
#define WORLD_DUNGEON2				2
#define WORLD_GUILD					3
#define WORLD_START					4
#define WORLD_DUNGEON_S				5
#define WORLD_SKY					6
#define WORLD_WHITEHORN				7
#define WORLD_DUNGEON4				8
#define WORLD_DMITRON				9	
#define WORLD_MATRIX				10	    
#define WORLD_GUILD_WAR				11
#define WORLD_DMITRON_BATTLE		12	    
#define WORLD_BOSSRAID_BULKAN		13
#define WORLD_BOSSRAID_KAILIPTON	14
#define WORLD_BOSSRAID_AIDIA		15
#define WORLD_BOSSRAID_HUMAN		16
#define WORLD_BOSSRAID_HYBRIDER		17
#define WORLD_BOSSRAID_FINALBOSS	18	    
#define WORLD_MATRIX_2009			19
#define	WORLD_DISPOSAL				20
#define	WORLD_VIDBLINE				21
#define WORLD_FORLORN_S2			22
#define WORLD_FORLORN_S3			23
#define WORLD_FORLORN_S4			24
#define WORLD_HIGHTLAND				25
#define WORLD_TRITIAN				26
#define WORLD_SPACE					27
#define WORLD_ROYALE				28
#define WORLD_DUN910				29
#define WORLD_MAP930				30


#define	MAX_VISIBLE_NODE	(LAND_VISIBLE_WIDTH*LAND_VISIBLE_HEIGHT+LAND_VISIBLE_HEIGHT)

#define MAP_RENDER_RANGE 30	// 13		// 던전 맵 렌더링 범위.

#define	MAP_CELL_WIDTH_COUNT	256
#define MAP_CELL_HEIGHT_COUNT	256

#define WATER_NOISE_NUM		11

struct MapAttr
{
	char	sTileFileName[256];
	int		nMapWidthNum, nMapHeightNum;					// 맵의 가로 세로 갯수
	int		nTileWidthNum, nTileHeightNum;					// 타일 비트맵의 가로 세로
	int		nTilePixelWidth, nTilePixelHeight;
};

class CQuadTreeNode;
class CDFloor;
class TileTextures;
class IndexedTexture;
class CWaterObj;
class CNk3DMipMap;
class CFrustum;
class CNKTerQuadNode;

DWORD AddColor(DWORD dwArg1, DWORD dwArg2);			// Used in CQuadTreeNode and CDFloor

class CLand
{
protected:
	CQuadTreeNode	*m_pQuadTreeNode;
	int				*m_pnVisibleIndex;				// 렌더링되는 타일에 대한 인덱스
	float			*m_pfFarWaterFactor;
	static int		m_nVisibleCount;				// 렌더링 타일 인덱스 카운트
	static int		m_nMaxVisibleCount;
	int				m_selPtIdx[9];					// 지형 제작시 선택되는 점에 대한 인덱스
	int				m_selPtCount;

	// Dungeon
	CDFloor			*m_pDFloor;

	// 타일 텍스처에 대한 인덱스
	MapAttr			m_MapAttr;

	// 지형에 사용될 물 객체
	CWaterObj		*m_pWaterObj;	
	static float	m_fWaterAltitude;

	BOOL	m_bCanDoModulate, m_bCanDoModulate2X;
	BOOL	m_bCanHighQuality;

public:
	CLand();
	virtual ~CLand();

	LPDIRECT3DDEVICE7 m_pd3dDevice;

	unsigned char	*m_pHeightMap;
	unsigned char	*m_pAttrMap;				// 오브젝트가 있다 없다에 대한 속성 맵.
	unsigned char	*m_pAttrMapDummy;			// 유저가 속성맵 교환을 할 수 없도록.
	unsigned char	*m_pTileDrawMap;
	WORD			*m_pMapIdx;					// 맵의 텍스처 인덱스
	unsigned char	*m_pDungeonIdx;				// 던전 맵 인덱스.
	int				*m_pTileTexIdx;				// 타일 텍스처에 대한 인덱스 <== 이건 없을 수도 있다.
	TileTextures	*m_pTileTextures;
	int				m_nDetailIdx, m_nDetailIdxLow;
	int				m_nNoneDetailIdx;
	int				m_nDBottomIdx;
	IndexedTexture  *m_pIndexedTexture;
	CNk3DMipMap		*m_pNk3DMipMap;
	DWORD			*m_pdwColor;
	unsigned char	*m_puAlpha;					// 알파값 테이블.
	LAGVERTEX		m_WaterRect[4];
	float			m_WaterNoise[WATER_NOISE_NUM];
	float			m_WaveNoise[WATER_NOISE_NUM];

	// 속성맵의 가로 세로 길이.
	int				m_nAttrMapWidth, m_nAttrMapByteWidth, m_nAttrMapHeight;

	// 땅의 밝기를 더 해주는 요소.
	DWORD			m_dwDayLight;				// 삭제될 예정.
	float			m_fLightFactor;				// 지면의 LVertex의 Color 감산 요소.
	void			IncreaseDayLight();
	void			DecreaseDayLight();
	DWORD			GetDayLightColor()
	{
		return m_dwDayLight;
	};
	void			SetLightFactor(float fLightFactor)
	{
		m_fLightFactor = fLightFactor;
	};

	// 땅의 하이라이트.
	float			m_fLandDistRange, m_fLandSpotFactor;
	float			m_fDunDistRange, m_fDunSpotFactor;
	void			SetHourLight(BOOL bLight)
	{
		m_bHourLight = bLight;
	};
	void			SetHourLightLand(float fRange, float fFactor);
	void			SetHourLightDungeon(float fRange, float fFactor);

	int				m_nCurHour;
	void			SetCurHour(int nHour)
	{
		m_nCurHour = nHour;
	};

	float			m_fCenterX, m_fCenterZ;		// 던젼의 중심.
	BOOL			m_bHourLight;
	// Operations
public:
	void		SetDevice(LPDIRECT3DDEVICE7 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
	}
	void		AddVisibleList(int idx, float farWaterFactor = 0.0f);

	void		QuadInit(CFrustum *pFrustum, float x, float z);					// 중심 점..
	void		DungeonInit(float x, float z);
	BOOL		Init(char *strFilePath, IndexedTexture *pIndexedTexture, int nMapNum);
	void		Render(float x, float z);
	void		RenderWaterArea(LPDIRECTDRAWSURFACE7 lpTexture = NULL, LPDIRECTDRAWSURFACE7 lpTexture2 = NULL);
	void		RenderWaterRect(float x, float z, DWORD alpha, LPDIRECTDRAWSURFACE7 lpTexture, LPDIRECTDRAWSURFACE7 lpTexture2);

	// 타일의 높이 조절을 위한 함수들.
	void		DrawSelectedPoints();
	void		AdjustHeight(int oldy, int y);
	BOOL		FindNearPoint(long mx, long my, int count);		// 가장 근접한 점을 찾는다.
	BOOL		PickPoint(D3DVECTOR &reVec, long mx, long my);	// PickPoint
	BOOL		PickPoint(D3DVECTOR &reVec, long mx, long my, int &tileIdx);	// PickPoint
	void		GetRay(D3DVECTOR &org, D3DVECTOR &dir, long sx, long sy);
	void		GetPipelineMatrix(D3DMATRIX &matPipe);
	float		GetHFHeight(float x, float z);					// 좌표 (x, z) 위체의 높이.
	int			GetMapIndex(D3DVECTOR vCoord);					// 광간좌표 => 맵 인덱스
	int			GetHF(int x, int z);							// (x, z) 위치의 height field값.
	BOOL		IsInsideMap(D3DVECTOR *vecOrg, D3DMATRIX &matPipe, BOOL &bLeft, long mx, long my);

	// 맵의 속성을 알아보는 부분.
	BOOL		IsMovableArea(float x, float z);

	// 안그려주는 타일 알아보는 부분.
	BOOL		IsDrawTile(float x, float z);

	void		OnSaveHeightMap();

	// member access
	unsigned char*	GetHeightMap()
	{
		return m_pHeightMap;
	};
	int			GetMapWidthNum()
	{
		return m_MapAttr.nMapWidthNum;
	};
	int			GetMapHeightNum()
	{
		return m_MapAttr.nMapHeightNum;
	};
	BOOL		TranslateTileToWorld(D3DVECTOR &reVec, int w, int h);
	BOOL		IsValidWorldCoord(D3DVECTOR &vec);

	// textures
	void		InvalidateAllTextures();
	void		RestoreAllTextures();

	void		DoModulate(BOOL bCanDoModulate)
	{
		m_bCanDoModulate = bCanDoModulate;
	};
	BOOL		CanDoModulate()
	{
		return m_bCanDoModulate;
	};

	void		DoModulate2X(BOOL bCanDoModulate2X)
	{
		m_bCanDoModulate2X = bCanDoModulate2X;
	};
	BOOL		CanDoModulate2X()
	{
		return m_bCanDoModulate2X;
	};

	void		DoHighQuality( BOOL bHighQuality )
	{
		m_bCanHighQuality = bHighQuality;
	}

	// ---------------------------------------------------------
	// specific to world (zone)
	// ---------------------------------------------------------
	BOOL		SetFieldAttr(float fMinX, float fMinZ, float fMaxX, float fMaxZ, BOOL bPass);
	void		CalcWaterNoise();
	
protected:
	LPDIRECT3D7					m_pD3D;
	CNKTerQuadNode*				m_aRootNode[4];				// Root nodes of quad tree
	LPDIRECT3DVERTEXBUFFER7		m_aVB[4];					// Terrain Vertex buffers
	DWORD						m_dwVBSize;					// Vertex count of vertex buffer
	int							m_nVBWidth;					// Width of vertex buffer
	LPDIRECTDRAWSURFACE7		m_aClrMapTexL2[4];			// Color(alpha & light) map texture ( layer 2 )
	LPDIRECTDRAWSURFACE7		m_aClrMapTexL3[4];			// Color(alpha & light) map texture ( layer 3 )
	unsigned char*				m_pLightMapsR[6];			// Light maps ( r channel )
	unsigned char*				m_pLightMapsG[6];			// Light maps ( g channel )
	unsigned char*				m_pLightMapsB[6];			// Light maps ( b channel )
	D3DVECTOR					m_aDirectLightDir[6];		// Directions of directional lights
	int							m_nTimeID;					// World time ID ( 0 ~ 5 )
	int							m_nTexCount;				// Texture count

public:
	int							m_nQuadTreeDepth;			// Depth of quad tree
	unsigned char*				m_pAlphaMapL2;				// Alpha map ( layer 2 )
	unsigned char*				m_pAlphaMapL3;				// Alpha map ( layer 3 )
	WORD**						m_ppTerIBL1;				// Terrain index buffer - layer 1
	WORD**						m_ppTerIBL2;				// Terrain index buffer - layer 2
	WORD**						m_ppTerIBL3;				// Terrain index buffer - layer 3
	DWORD*						m_pTerIBCountL1;			// Index count of the terrain index buffer - layer 1
	DWORD*						m_pTerIBCountL2;			// Index count of the terrain index buffer - layer 2
	DWORD*						m_pTerIBCountL3;			// Index count of the terrain index buffer - layer 3

protected:
	BOOL	InitQuadTree( unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID );
	BOOL	InitVertexBufferInSysMem( NKVERTEXT2* pVB );
	void	RenderTerrainByLow( CFrustum* pFrustum );
	HRESULT	CreateClrMapTexture();
	HRESULT	UpdateClrMapTexture( int nLayer, int nIndex );

public:
	BOOL	LoadMap( const TCHAR* szFilePath, const TCHAR* szFileName );
	void	Destroy();
	void	RenderTerrain( CFrustum* pFrustum );
	void	SetWorldTime( int nWorldTime );
	void	GetLightDirection( float& rX,float& rY, float& rZ );
	HRESULT	InvalidateDeviceObjects();
	HRESULT	RestoreDeviceObjects( LPDIRECT3D7 pD3D, LPDIRECT3DDEVICE7 pd3dDevice );
	void	SetD3D(LPDIRECT3D7 pD3D)
	{
		m_pD3D = pD3D;
	}
	HRESULT	InvalidateVB();
	BOOL	InitVertexBuffer();
};

#endif // _LAND_H_
