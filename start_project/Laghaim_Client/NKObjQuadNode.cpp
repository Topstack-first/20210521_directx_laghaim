#include "stdafx.h"
//-----------------------------------------------------------------------------
// File : NKObjQuadNode.cpp
// Desc : 2004 addition
//-----------------------------------------------------------------------------
#define D3D_OVERLOADS
#include <tchar.h>
#include "NKObjQuadNode.h"
#include "main.h"
#include "Land.h"
#include "Frustum.h"
#include "DecoObj.h"
#include "ClrAlloc.h"
#include "NKMath.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "WeatherMgr.h"
#include "UIMgr.h"
#include "DecardiBattleManager.h"

// Global variables


extern int					gMapWidth;
extern int					gMapExtention;
extern float				g_fTimeKey;
extern CDecoObjSet*			g_pDecoObjSet;
extern CClrAlloc*			g_ClrAlloc;
extern list<WORD>			g_listRenderedDecoColor;
extern list<CDecoObj*>		g_listRenderedDeco;
extern list<CDecoObj*>		g_listAlphaDeco;
extern list<CDecoObj*>		g_listLastDeco;
extern list<CDecoObj*>		g_listFarDeco;
extern list<CDecoObj*>		g_listDecoObj[MAP_CELL_HEIGHT_COUNT][MAP_CELL_WIDTH_COUNT];
CNKObjQuadNode*				g_pObjTree = NULL;

// 공성 특별 오브젝트.
extern CDecoObj	*g_pGuildGateClosed;				// 성문 - 특별히 따로 관리.
extern CDecoObj *g_pGuildGate2Opened;

//-----------------------------------------------------------------------------
// Static member variables
//-----------------------------------------------------------------------------
int						CNKObjQuadNode::ms_nCellSize = 0;
int						CNKObjQuadNode::ms_nTreeWidth = 0;
CDecoObj*				CNKObjQuadNode::ms_pOpenDeco = NULL;
TCHAR*					CNKObjQuadNode::ms_szWaterFilename = NULL;
float					CNKObjQuadNode::ms_fRenderDist = 1500.0f;
float					CNKObjQuadNode::ms_fBlendDist = 1100.0f;
IndexedTexture*			CNKObjQuadNode::ms_pIndexedTexture = NULL;
int						CNKObjQuadNode::ms_nBaseWaterTexID = -1;
int						CNKObjQuadNode::ms_nWaveWater1TexID = -1;
int						CNKObjQuadNode::ms_nWaveWater2TexID = -1;
LPDIRECT3DVERTEXBUFFER7	CNKObjQuadNode::ms_pBaseWaterVB = NULL;
LPDIRECT3DVERTEXBUFFER7	CNKObjQuadNode::ms_pWaveWaterVB = NULL;
DWORD					CNKObjQuadNode::ms_dwBaseWaterVBSize = 0;
DWORD					CNKObjQuadNode::ms_dwWaveWaterVBSize = 0;
WORD*					CNKObjQuadNode::ms_pBaseWaterIB = NULL;
WORD*					CNKObjQuadNode::ms_pWaveWaterIB = NULL;
DWORD					CNKObjQuadNode::ms_dwBaseIndexCount = 0;
DWORD					CNKObjQuadNode::ms_dwWaveIndexCount = 0;
float					CNKObjQuadNode::ms_fBaseWaterMove = 0.0f;
float					CNKObjQuadNode::ms_fWaveWater1Move = 0.0f;
float					CNKObjQuadNode::ms_fWaveWater2Move = 0.0f;
float					CNKObjQuadNode::ms_fBaseWaterSpeed = 0.07f;
float					CNKObjQuadNode::ms_fWaveWater1Speed = 0.17f;
float					CNKObjQuadNode::ms_fWaveWater2Speed = 0.12f;
D3DMATRIX				CNKObjQuadNode::ms_matTexTrans;
float*					CNKObjQuadNode::ms_pWaterHeights = NULL;
int						CNKObjQuadNode::ms_nWaterHeightWidth = 0;
int						CNKObjQuadNode::ms_nBaseWaterSize = 0;
D3DVECTOR				CNKObjQuadNode::ms_vCameraAt;
D3DVECTOR				CNKObjQuadNode::ms_vCameraEye;

float g_OutsideDoorX[8] = {	10118.0f,	11763.0f,	9986.0f,	11490.0f,
							10329.0f,	11413.0f,	10330.0f,	11543.0f
						  };
float g_OutsideDoorZ[8] = {	7109.0f,	6906.0f,	4847.0f,	4667.0f,
							6715.0f,	6722.0f,	5041.0f,	5050.0f
						  };
// nate 2004 - 11
// 수호탑 주변속성 변경
float g_fGuardTowerX[ 4 ] = { 3900, 3500, 2900, 2500 };
float g_fGuardTowerZ[ 4 ] = { 10100, 10500, 10500, 10100 };
// nate 2005-03-23
// Matrix : G.P.Stone 공격할수 잇게 좌표따기
float g_fGPStoneX[ 12 ] = { 6296.87f, 9106.94f, 3459.45f, 6997.37f, 5600.22f, 4488.54f, 5221.25f, 7360.5f, 8126.77f, 6315.8f, 3608.04f, 8984.74f };
float g_fGPStoneZ[ 12 ] = { 9753.8f, 4669.47f, 4644.1f, 4619.08f, 4607.0f, 6577.62f, 7849.56f, 7856.04f, 6606.22f, 3011.41f, 8062.2f, 8111.78f };

// [2008/11/10 Theodoric] WORLD_MATRIX_2009 :: Stone 공격할수 잇게 좌표따기
float g_fGPStone2009X1[10] = {  1910.0f, 6990.0f, 3020.0f, 3270.0f, 6770.0f, 5670.0f, 10160.0f, 9770.0f, 5920.0f, 11000.0f};
float g_fGPStone2009X2[10] = {  1980.0f, 7070.0f, 3100.0f, 3330.0f, 6850.0f, 5740.0f, 10240.0f, 9860.0f, 5990.0f, 11060.0f};
float g_fGPStone2009Z1[10] = { 11040.0f, 6890.0f, 7020.0f, 4220.0f, 3360.0f, 9310.0f,  8270.0f, 5720.0f, 6130.0f,  1630.0f};
float g_fGPStone2009Z2[10] = { 11110.0f, 6970.0f, 7090.0f, 4300.0f, 3440.0f, 9380.0f,  8370.0f, 5800.0f, 6210.0f,  1690.0f};

CDecoObj* g_OutsideDoorPoint[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
float g_CastleFlagX[4] = {	10209.0f, 11518.0f, 10232.0f, 11515.0f};
float g_CastleFlagZ[4] = {	6905.0f, 6915.0f, 4840.0f, 4856.0f};
CDecoObj* g_CastleFlagPoint[4] = {NULL, NULL, NULL, NULL};
extern BOOL g_OutsideDoorOpen[];

CDecoObj* g_CrystalTowerPoint = NULL;
extern CDecoObj	*g_pOutsideDoorClosed, *g_pCastleGateClosed;

//wan:2004-12
//Whitehorn Dungeon
extern CDecoObj	*g_pBossGateClosed;
//-----------------------------------------------------------------------------
// Name : CNKObjQuadNode()
// Desc : Constructor
//-----------------------------------------------------------------------------
CNKObjQuadNode::CNKObjQuadNode()
	: m_nBaseWaterSegment(0)
	, m_nWaveWater1Segment(0)
	, m_nWaveWater2Segment(0)
	, m_wBaseVBIndex(0)
	, m_wWaveVBIndex(0)
{
	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
}

CNKObjQuadNode::CNKObjQuadNode( int nLevel, int nLeftX, int nLeftZ, int nRightX, int nRightZ, PFILE* fp )
{
	int	nCenterX, nCenterZ;

	m_pLLeft = NULL;
	m_pLRight = NULL;
	m_pULeft = NULL;
	m_pURight = NULL;
	m_nBaseWaterSegment = 0;
	m_nWaveWater1Segment = 0;
	m_nWaveWater2Segment = 0;
	m_wBaseVBIndex = 0;
	m_wWaveVBIndex = 0;

	if( nLevel < 1 )			// Lowest level node
	{
		//Bounding box
		pfread( &(m_vMin.x), sizeof(float), 1, fp );
		pfread( &(m_vMin.y), sizeof(float), 1, fp );
		pfread( &(m_vMin.z), sizeof(float), 1, fp );
		pfread( &(m_vMax.x), sizeof(float), 1, fp );
		pfread( &(m_vMax.y), sizeof(float), 1, fp );
		pfread( &(m_vMax.z), sizeof(float), 1, fp );

		// Read DecoObj list
		// Count
		DWORD	dwCount;
		pfread( &dwCount, sizeof(DWORD), 1, fp );

		// DecoObj
		CDecoObj*	pDecoObj = NULL;
		CDecoObj*	pDecoObj2 = NULL;
		for( DWORD i = 0; i < dwCount; i++ )
		{
			pDecoObj = new CDecoObj( g_ClrAlloc );
			if( !pDecoObj )
			{
				pfseek( fp, 20L, SEEK_SET);
				continue;
			}
			pfread( &(pDecoObj->m_nLoloId), sizeof(int), 1, fp );
			pfread( &(pDecoObj->m_wx), sizeof(float), 1, fp );
			pfread( &(pDecoObj->m_wz), sizeof(float), 1, fp );
			pfread( &(pDecoObj->m_wy), sizeof(float), 1, fp );
			pfread( &(pDecoObj->m_dir), sizeof(float), 1, fp );
			pDecoObj->m_pLolo = &( g_pDecoObjSet->m_pLoloSet[pDecoObj->m_nLoloId] );

			// 화이트혼 외성문을 먼저 붙잡아두자.
			if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN)
			{
				if (pDecoObj->m_nLoloId == LOLO_OUTSIDE_DOOR)
				{
					for (int k = 0; k < 8; k++)
					{
						if (pDecoObj->m_wx >= g_OutsideDoorX[k] - 50.0f && pDecoObj->m_wx <= g_OutsideDoorX[k] + 50.0f &&
								pDecoObj->m_wz >= g_OutsideDoorZ[k] - 50.0f && pDecoObj->m_wz <= g_OutsideDoorZ[k] + 50.0f)
						{
							g_OutsideDoorPoint[k] = pDecoObj;
							break;
						}
					}
				}
				// 크리스탈 타워도...
				if (pDecoObj->m_nLoloId == LOLO_CRYSTAL_TOWER)
				{
					g_CrystalTowerPoint = pDecoObj;
				}
				// 깃발도...
				if (pDecoObj->m_nLoloId == LOLO_CASTLE_FLAG)
				{
					for (int k = 0; k < 4; k++)
					{
						if (pDecoObj->m_wx >= g_CastleFlagX[k] - 50.0f && pDecoObj->m_wx <= g_CastleFlagX[k] + 50.0f &&
								pDecoObj->m_wz >= g_CastleFlagZ[k] - 50.0f && pDecoObj->m_wz <= g_CastleFlagZ[k] + 50.0f)
						{
							g_CastleFlagPoint[k] = pDecoObj;
							break;
						}
					}
				}
			}

			if( ( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_FIRE || pDecoObj->m_pLolo->m_LineType == LINE_TYPE_BLUEFIRE ) &&
					pDecoObj->m_pLolo->m_LineVector0 )
			{
				pDecoObj->Fire_Particle = new Deco_Particle[MAX_FIRE_PARTICLE];

				for( int i = 0; i < MAX_FIRE_PARTICLE; i++ )
				{
					pDecoObj->Fire_Particle[i].start_time = 0;
					pDecoObj->Fire_Particle[i].life_time = 0;
					pDecoObj->Fire_Particle[i].cur_time = 0;
					pDecoObj->Fire_Particle[i].scale = 1.0f;
					pDecoObj->Fire_Particle[i].fade = 1.0f;

					pDecoObj->Fire_Particle[i].red = 1.0f;
					pDecoObj->Fire_Particle[i].green = 1.0f;
					pDecoObj->Fire_Particle[i].blue = 1.0f;

					pDecoObj->Fire_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Fire_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Fire_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Fire_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Fire_Particle[i].texIndex = 0;
					pDecoObj->Fire_Particle[i].texFrame = 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_SMOKE && pDecoObj->m_pLolo->m_LineVector0 )
			{
				pDecoObj->Smoke_Particle = new Deco_Particle[MAX_SMOKE_PARTICLE];

				int smoke_particle_start_time[MAX_SMOKE_PARTICLE] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45 };

				for( int i = 0; i < MAX_SMOKE_PARTICLE; i++ )
				{
					pDecoObj->Smoke_Particle[i].start_time = smoke_particle_start_time[i];
					pDecoObj->Smoke_Particle[i].life_time = 0;
					pDecoObj->Smoke_Particle[i].cur_time = 0;
					pDecoObj->Smoke_Particle[i].scale = 1.0f;
					pDecoObj->Smoke_Particle[i].fade = 0.5f;

					pDecoObj->Smoke_Particle[i].red = 1.0f;
					pDecoObj->Smoke_Particle[i].green = 1.0f;
					pDecoObj->Smoke_Particle[i].blue = 1.0f;

					pDecoObj->Smoke_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Smoke_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Smoke_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Smoke_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Smoke_Particle[i].texIndex = 0;
					pDecoObj->Smoke_Particle[i].texFrame = 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_VOLCANO && pDecoObj->m_pLolo->m_LineVector0 )
			{
				pDecoObj->Spray_Particle = new Deco_Particle[MAX_SPRAY_PARTICLE];

				int	spray_particle_start_time[MAX_SPRAY_PARTICLE] = { 0, };//0, 10, 15, 20, 25, 30, 35, 40, 45

				for( int i = 0; i < MAX_SPRAY_PARTICLE; i++ )
				{
					pDecoObj->Spray_Particle[i].start_time = spray_particle_start_time[i];
					pDecoObj->Spray_Particle[i].life_time = 0;
					pDecoObj->Spray_Particle[i].cur_time = 0;
					pDecoObj->Spray_Particle[i].scale = 1.0f;
					pDecoObj->Spray_Particle[i].fade = 0.5f;

					pDecoObj->Spray_Particle[i].red = 1.0f;
					pDecoObj->Spray_Particle[i].green = 1.0f;
					pDecoObj->Spray_Particle[i].blue = 1.0f;

					pDecoObj->Spray_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Spray_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Spray_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Spray_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Spray_Particle[i].texIndex = 0;
					pDecoObj->Spray_Particle[i].texFrame = 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_POISON && pDecoObj->m_pLolo->m_LineVector0 )
			{
				pDecoObj->Turn_Particle = new Deco_Particle[MAX_TURN_PARTICLE];

				int turn_particle_start_time[MAX_TURN_PARTICLE] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };

				for( int i = 0; i < MAX_TURN_PARTICLE; i++ )
				{
					pDecoObj->Turn_Particle[i].start_time = turn_particle_start_time[i];
					pDecoObj->Turn_Particle[i].life_time = 0;
					pDecoObj->Turn_Particle[i].cur_time = 0;
					pDecoObj->Turn_Particle[i].scale = 1.0f;
					pDecoObj->Turn_Particle[i].fade = 0.5f;

					pDecoObj->Turn_Particle[i].red = 1.0f;
					pDecoObj->Turn_Particle[i].green = 1.0f;
					pDecoObj->Turn_Particle[i].blue = 1.0f;

					pDecoObj->Turn_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Turn_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Turn_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Turn_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Turn_Particle[i].texIndex = 0;
					pDecoObj->Turn_Particle[i].texFrame = 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_GAS && pDecoObj->m_pLolo->m_LineVector0 )
			{
				pDecoObj->Light_Particle = new Deco_Particle[MAX_LIGHT_PARTICLE];

				int	light_particle_start_time[MAX_LIGHT_PARTICLE] = { 0,};

				for( int i = 0; i < MAX_LIGHT_PARTICLE; i++ )
				{
					pDecoObj->Light_Particle[i].start_time = light_particle_start_time[i];
					pDecoObj->Light_Particle[i].life_time = 0;
					pDecoObj->Light_Particle[i].cur_time = 0;
					pDecoObj->Light_Particle[i].scale = 1.0f;
					pDecoObj->Light_Particle[i].fade = 0.5f;

					pDecoObj->Light_Particle[i].red = 1.0f;
					pDecoObj->Light_Particle[i].green = 1.0f;
					pDecoObj->Light_Particle[i].blue = 1.0f;

					pDecoObj->Light_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Light_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Light_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Light_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Light_Particle[i].texIndex = 0;
					pDecoObj->Light_Particle[i].texFrame = 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_ELECTRIC && pDecoObj->m_pLolo->m_LineVector0 )
			{
				// 1. 오브젝트 이펙트 설정 후에 넣어준다.
				pDecoObj->Electric_Particle = new Deco_Particle[MAX_ELCTRIC_PARTICLE];

				for( int i = 0; i < MAX_ELCTRIC_PARTICLE; i++ )
				{
					pDecoObj->Electric_Particle[i].start_time	= 0;
					pDecoObj->Electric_Particle[i].life_time	= 0;
					pDecoObj->Electric_Particle[i].cur_time		= 0;
					pDecoObj->Electric_Particle[i].scale		= 0;
					pDecoObj->Electric_Particle[i].fade			= 0.5f;

					pDecoObj->Electric_Particle[i].red			= 1.0f;
					pDecoObj->Electric_Particle[i].green		= 1.0f;
					pDecoObj->Electric_Particle[i].blue			= 1.0f;

					pDecoObj->Electric_Particle[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Electric_Particle[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Electric_Particle[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Electric_Particle[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Electric_Particle[i].texIndex		= 0;
					pDecoObj->Electric_Particle[i].texFrame		= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_RED && pDecoObj->m_pLolo->m_LineVector0 )
			{
				// 1. 오브젝트 이펙트 설정 후에 넣어준다.
				pDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

				for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
				{
					pDecoObj->ClystalGlow[i].start_time	= 0;
					pDecoObj->ClystalGlow[i].life_time	= 0;
					pDecoObj->ClystalGlow[i].cur_time	= 0;
					pDecoObj->ClystalGlow[i].scale		= 0;
					pDecoObj->ClystalGlow[i].fade		= 0.5f;

					pDecoObj->ClystalGlow[i].red		= 1.0f;
					pDecoObj->ClystalGlow[i].green		= 1.0f;
					pDecoObj->ClystalGlow[i].blue		= 1.0f;

					pDecoObj->ClystalGlow[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].cur_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].gravity	= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->ClystalGlow[i].texIndex	= 0;
					pDecoObj->ClystalGlow[i].texFrame	= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINEB && pDecoObj->m_pLolo->m_LineVector0 )
			{
				// 1. 오브젝트 이펙트 설정 후에 넣어준다.
				pDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

				for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
				{
					pDecoObj->ClystalGlow[i].start_time		= 0;
					pDecoObj->ClystalGlow[i].life_time		= 0;
					pDecoObj->ClystalGlow[i].cur_time		= 0;
					pDecoObj->ClystalGlow[i].scale			= 0;
					pDecoObj->ClystalGlow[i].fade			= 0.5f;

					pDecoObj->ClystalGlow[i].red			= 1.0f;
					pDecoObj->ClystalGlow[i].green			= 1.0f;
					pDecoObj->ClystalGlow[i].blue			= 1.0f;

					pDecoObj->ClystalGlow[i].first_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].direction		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->ClystalGlow[i].texIndex		= 0;
					pDecoObj->ClystalGlow[i].texFrame		= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINES && pDecoObj->m_pLolo->m_LineVector0 )
			{
				// 1. 오브젝트 이펙트 설정 후에 넣어준다.
				pDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

				for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
				{
					pDecoObj->ClystalGlow[i].start_time		= 0;
					pDecoObj->ClystalGlow[i].life_time		= 0;
					pDecoObj->ClystalGlow[i].cur_time		= 0;
					pDecoObj->ClystalGlow[i].scale			= 0;
					pDecoObj->ClystalGlow[i].fade			= 0.5f;

					pDecoObj->ClystalGlow[i].red			= 1.0f;
					pDecoObj->ClystalGlow[i].green			= 1.0f;
					pDecoObj->ClystalGlow[i].blue			= 1.0f;

					pDecoObj->ClystalGlow[i].first_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].direction		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->ClystalGlow[i].texIndex		= 0;
					pDecoObj->ClystalGlow[i].texFrame		= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_CYON && pDecoObj->m_pLolo->m_LineVector0 )
			{
				// 1. 오브젝트 이펙트 설정 후에 넣어준다.
				pDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

				for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
				{
					pDecoObj->ClystalGlow[i].start_time		= 0;
					pDecoObj->ClystalGlow[i].life_time		= 0;
					pDecoObj->ClystalGlow[i].cur_time		= 0;
					pDecoObj->ClystalGlow[i].scale			= 0;
					pDecoObj->ClystalGlow[i].fade			= 0.5f;

					pDecoObj->ClystalGlow[i].red			= 1.0f;
					pDecoObj->ClystalGlow[i].green			= 1.0f;
					pDecoObj->ClystalGlow[i].blue			= 1.0f;

					pDecoObj->ClystalGlow[i].first_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].direction		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->ClystalGlow[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->ClystalGlow[i].texIndex		= 0;
					pDecoObj->ClystalGlow[i].texFrame		= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_WATERFALL && pDecoObj->m_pLolo->m_LineVector0 )
			{
				// 1. 오브젝트 이펙트 설정 후에 넣어준다.
				pDecoObj->Waterfall_Particle = new Deco_Particle[MAX_WATERFALL_PARTICLE];

				for( int i = 0; i < MAX_WATERFALL_PARTICLE; i++ )
				{
					pDecoObj->Waterfall_Particle[i].start_time	= 0;
					pDecoObj->Waterfall_Particle[i].life_time	= 0;
					pDecoObj->Waterfall_Particle[i].cur_time	= 0;
					pDecoObj->Waterfall_Particle[i].scale		= 0;
					pDecoObj->Waterfall_Particle[i].fade		= 0.5f;

					pDecoObj->Waterfall_Particle[i].red			= 1.0f;
					pDecoObj->Waterfall_Particle[i].green		= 1.0f;
					pDecoObj->Waterfall_Particle[i].blue		= 1.0f;

					pDecoObj->Waterfall_Particle[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Waterfall_Particle[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Waterfall_Particle[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Waterfall_Particle[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Waterfall_Particle[i].texIndex	= 0;
					pDecoObj->Waterfall_Particle[i].texFrame	= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_PORTAL && pDecoObj->m_pLolo->m_LineVector0)
			{
				pDecoObj->PortalBossraid= new Deco_Particle[MAX_PORTAL_BOSSRAID_PARTICLE];

				for( int i = 0; i < MAX_PORTAL_BOSSRAID_PARTICLE; i++ )
				{
					pDecoObj->PortalBossraid[i].start_time	= 0;
					pDecoObj->PortalBossraid[i].life_time	= 0;
					pDecoObj->PortalBossraid[i].cur_time	= 0;
					pDecoObj->PortalBossraid[i].scale		= 0;
					pDecoObj->PortalBossraid[i].fade		= 0.5f;

					pDecoObj->PortalBossraid[i].red			= 1.0f;
					pDecoObj->PortalBossraid[i].green		= 1.0f;
					pDecoObj->PortalBossraid[i].blue		= 1.0f;

					pDecoObj->PortalBossraid[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->PortalBossraid[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->PortalBossraid[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->PortalBossraid[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->PortalBossraid[i].texIndex	= 0;
					pDecoObj->PortalBossraid[i].texFrame	= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_FIRESMOKE && pDecoObj->m_pLolo->m_LineVector0)
			{
				pDecoObj->Firesmoke_Particle = new Deco_Particle[MAX_FIRESMOKE_PARTICLE];

				for( int i = 0; i < MAX_FIRESMOKE_PARTICLE; i++ )
				{
					pDecoObj->Firesmoke_Particle[i].start_time	= 0;
					pDecoObj->Firesmoke_Particle[i].life_time	= 0;
					pDecoObj->Firesmoke_Particle[i].cur_time	= 0;
					pDecoObj->Firesmoke_Particle[i].scale		= 0;
					pDecoObj->Firesmoke_Particle[i].fade		= 0.5f;

					pDecoObj->Firesmoke_Particle[i].red			= 1.0f;
					pDecoObj->Firesmoke_Particle[i].green		= 1.0f;
					pDecoObj->Firesmoke_Particle[i].blue		= 1.0f;

					pDecoObj->Firesmoke_Particle[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Firesmoke_Particle[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Firesmoke_Particle[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Firesmoke_Particle[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Firesmoke_Particle[i].texIndex	= 0;
					pDecoObj->Firesmoke_Particle[i].texFrame	= 0;
				}
			}

			pDecoObj->CalBoundBox();
			m_lstDecoObj.push_back( pDecoObj );

			if( pDecoObj->m_pLolo->m_WalkPlaneCount > 0 )
			{
				pDecoObj2 = new CDecoObj( g_ClrAlloc );
				if( pDecoObj2 )
				{
					pDecoObj2->m_nLoloId = pDecoObj->m_nLoloId;
					pDecoObj2->m_wx = pDecoObj->m_wx;
					pDecoObj2->m_wy = pDecoObj->m_wy;
					pDecoObj2->m_wz = pDecoObj->m_wz;
					pDecoObj2->m_dir = pDecoObj->m_dir;
					pDecoObj2->m_pLolo = pDecoObj->m_pLolo;
					pDecoObj2->CalBoundBox();
					g_listDecoObj[nLeftZ][nLeftX].push_back( pDecoObj2 );
				}
			}
			pDecoObj = NULL;
			pDecoObj2 = NULL;
		}


		// Read water info
		pfread( &m_nBaseWaterSegment, sizeof(unsigned char), 1, fp );
		pfread( &m_nWaveWater1Segment, sizeof(unsigned char), 1, fp );
		pfread( &m_nWaveWater2Segment, sizeof(unsigned char), 1, fp );
		pfread( &m_wBaseVBIndex, sizeof(WORD), 1, fp );
		pfread( &m_wWaveVBIndex, sizeof(WORD), 1, fp );

		return ;
	}

	// Prepare for a child node
	nLevel--;
	nCenterX = ( nLeftX + nRightX ) >> 1;
	nCenterZ = ( nLeftZ + nRightZ ) >> 1;

	// Recursive call
	m_pLLeft = new CNKObjQuadNode( nLevel, nLeftX, nLeftZ, nCenterX, nCenterZ, fp );
	m_pLRight = new CNKObjQuadNode( nLevel, nCenterX, nLeftZ, nRightX, nCenterZ, fp );
	m_pULeft = new CNKObjQuadNode( nLevel, nLeftX, nCenterZ, nCenterX, nRightZ, fp );
	m_pURight = new CNKObjQuadNode( nLevel, nCenterX, nCenterZ, nRightX, nRightZ, fp );

	// Get min & max vertices of lower level node
	D3DVECTOR	vMin = m_pLLeft->m_vMin;
	D3DVECTOR	vMax = m_pLLeft->m_vMax;

	// Calculate min & max vertices of current level node
	if( vMin.x > m_pLRight->m_vMin.x ) vMin.x = m_pLRight->m_vMin.x;
	if( vMin.x > m_pULeft->m_vMin.x ) vMin.x = m_pULeft->m_vMin.x;
	if( vMin.x > m_pURight->m_vMin.x ) vMin.x = m_pURight->m_vMin.x;
	if( vMax.x < m_pLRight->m_vMax.x ) vMax.x = m_pLRight->m_vMax.x;
	if( vMax.x < m_pULeft->m_vMax.x ) vMax.x = m_pULeft->m_vMax.x;
	if( vMax.x < m_pURight->m_vMax.x ) vMax.x = m_pURight->m_vMax.x;

	if( vMin.y > m_pLRight->m_vMin.y ) vMin.y = m_pLRight->m_vMin.y;
	if( vMin.y > m_pULeft->m_vMin.y ) vMin.y = m_pULeft->m_vMin.y;
	if( vMin.y > m_pURight->m_vMin.y ) vMin.y = m_pURight->m_vMin.y;
	if( vMax.y < m_pLRight->m_vMax.y ) vMax.y = m_pLRight->m_vMax.y;
	if( vMax.y < m_pULeft->m_vMax.y ) vMax.y = m_pULeft->m_vMax.y;
	if( vMax.y < m_pURight->m_vMax.y ) vMax.y = m_pURight->m_vMax.y;

	if( vMin.z > m_pLRight->m_vMin.z ) vMin.z = m_pLRight->m_vMin.z;
	if( vMin.z > m_pULeft->m_vMin.z ) vMin.z = m_pULeft->m_vMin.z;
	if( vMin.z > m_pURight->m_vMin.z ) vMin.z = m_pURight->m_vMin.z;
	if( vMax.z < m_pLRight->m_vMax.z ) vMax.z = m_pLRight->m_vMax.z;
	if( vMax.z < m_pULeft->m_vMax.z ) vMax.z = m_pULeft->m_vMax.z;
	if( vMax.z < m_pURight->m_vMax.z ) vMax.z = m_pURight->m_vMax.z;

	// Set a bounding box of current level node
	m_vMin = vMin;
	m_vMax = vMax;
}


//-----------------------------------------------------------------------------
// Name : ~CNKObjQuadNode()
// Desc : Destructor
//-----------------------------------------------------------------------------
CNKObjQuadNode::~CNKObjQuadNode()
{
	CDecoObj*					pDecoObj = NULL;
	list<CDecoObj*>::iterator	iterDeco;

	for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); )
	{
		pDecoObj = *iterDeco;
		delete pDecoObj;
		iterDeco = m_lstDecoObj.erase( iterDeco );
	}

	SAFE_DELETE( m_pLLeft );
	SAFE_DELETE( m_pLRight );
	SAFE_DELETE( m_pULeft );
	SAFE_DELETE( m_pURight );
}


//-----------------------------------------------------------------------------
// Name : RestoreDeviceObjects()
// Desc :
//-----------------------------------------------------------------------------
HRESULT CNKObjQuadNode::RestoreDeviceObjects()
{
	// Create water vertex buffer
	if( !InitWaterVertexBuffer() )
		return E_FAIL;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : InvalidateDeviceObjects()
// Desc :
//-----------------------------------------------------------------------------
HRESULT CNKObjQuadNode::InvalidateDeviceObjects()
{
	SAFE_RELEASE( ms_pBaseWaterVB );
	SAFE_RELEASE( ms_pWaveWaterVB );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name : InitWaterVertexBuffer()
// Desc :
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::InitWaterVertexBuffer()
{
	PFILE*	fp = NULL;
	fp = pfopen( ms_szWaterFilename, "rb" );
	if( fp == NULL )
		return false;

	DWORD	dwMagic;
	pfread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'W' << 24 ) | ( 'A' << 16 ) | ( 'T' << 8 ) ) + 10 )
		return false;

	// Data of vertex buffer of base water
	pfread( &ms_dwBaseWaterVBSize, sizeof(DWORD), 1, fp );
	NKVERTEXLT*	pTempBaseVB = new NKVERTEXLT[ms_dwBaseWaterVBSize];
	pfread( pTempBaseVB, sizeof(NKVERTEXLT), ms_dwBaseWaterVBSize, fp );

	// Data of vertex buffer of wave water
	pfread( &ms_dwWaveWaterVBSize, sizeof(DWORD), 1, fp );
	NKVERTEXLT*	pTempWaveVB = new NKVERTEXLT[ms_dwWaveWaterVBSize];
	pfread( pTempWaveVB, sizeof(NKVERTEXLT), ms_dwWaveWaterVBSize, fp );

	pfclose( fp );

	// Create vertex buffer
	if( pCMyApp->CanHighQuality() )
	{
		D3DVERTEXBUFFERDESC	VBDesc;
		VBDesc.dwSize = sizeof( VBDesc );
		VBDesc.dwCaps = D3DVBCAPS_WRITEONLY;
		VBDesc.dwFVF = NKVERTEXLT_FVF;
		VBDesc.dwNumVertices = ms_dwBaseWaterVBSize;

		//=====================================================
		// nate 2004 - 12
		// 샤일론 개편
		NKVERTEXLT*	pVert;
		if( ms_dwBaseWaterVBSize )
		{
			if( FAILED( GET_D3D()->CreateVertexBuffer( &VBDesc, &ms_pBaseWaterVB, 0 ) ) )
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );

			if( FAILED( ms_pBaseWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
											   (LPVOID*)(&pVert), NULL ) ) )
			{
				SAFE_RELEASE( ms_pBaseWaterVB );
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
			}
			memcpy( pVert, pTempBaseVB, ms_dwBaseWaterVBSize * sizeof(NKVERTEXLT) );
			ms_pBaseWaterVB->Unlock();
		}

		VBDesc.dwNumVertices = ms_dwWaveWaterVBSize;

		if( ms_dwWaveWaterVBSize )
		{
			if( FAILED( GET_D3D()->CreateVertexBuffer( &VBDesc, &ms_pWaveWaterVB, 0 ) ) )
			{
				SAFE_RELEASE( ms_pBaseWaterVB );
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
			}
			if( FAILED( ms_pWaveWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
											   (LPVOID*)(&pVert), NULL ) ) )
			{
				SAFE_RELEASE( ms_pBaseWaterVB );
				SAFE_RELEASE( ms_pWaveWaterVB );
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
			}

			memcpy( pVert, pTempWaveVB, ms_dwWaveWaterVBSize * sizeof(NKVERTEXLT) );
			ms_pWaveWaterVB->Unlock();
		}
		//=====================================================
		SAFE_DELETE_ARRAY( pTempBaseVB );
		SAFE_DELETE_ARRAY( pTempWaveVB );

		return true;
	}
	else
		return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
}

//-- Low Quality 의 컴퓨터에서 Water을 형성한다.
//-- 2005.01.20 / Lyul
//-- vertex buffer 에 사용될 vertex 개수가 0일 경우에도 강제 생성하다 실패해서
//-- 저 사양들의 화이트 혼 팅을 발생시키는 버그를 제거
bool CNKObjQuadNode::InitWaterVertexBufferInSysMem( NKVERTEXLT* pTempBaseVB, NKVERTEXLT* pTempWaveVB )
{
	// Create vertex buffer
	D3DVERTEXBUFFERDESC	VBDesc;
	VBDesc.dwSize = sizeof( VBDesc );
	VBDesc.dwCaps = D3DVBCAPS_SYSTEMMEMORY;
	VBDesc.dwFVF = NKVERTEXLT_FVF;
	VBDesc.dwNumVertices = ms_dwBaseWaterVBSize;
	NKVERTEXLT*	pVert;

	//---------------------------------------------------------------------------
	//-- 생성해야 될 Water가 있을 경우
	if( ms_dwBaseWaterVBSize > 0)
	{
		if( FAILED( GET_D3D()->CreateVertexBuffer( &VBDesc, &ms_pBaseWaterVB, 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			return false;
		}

		if( FAILED( ms_pBaseWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
										   (LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			SAFE_RELEASE( ms_pBaseWaterVB );
			return false;
		}

		memcpy( pVert, pTempBaseVB, ms_dwBaseWaterVBSize * sizeof(NKVERTEXLT) );
		ms_pBaseWaterVB->Unlock();
	}

	//---------------------------------------------------------------------------
	//-- 생성해야 될 Wave가 있을 경우
	if(ms_dwWaveWaterVBSize > 0)
	{
		VBDesc.dwNumVertices = ms_dwWaveWaterVBSize;
		if( FAILED( GET_D3D()->CreateVertexBuffer( &VBDesc, &ms_pWaveWaterVB, 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			SAFE_RELEASE( ms_pBaseWaterVB );
			return false;
		}

		if( FAILED( ms_pWaveWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
										   (LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pTempBaseVB );
			SAFE_DELETE_ARRAY( pTempWaveVB );
			SAFE_RELEASE( ms_pBaseWaterVB );
			SAFE_RELEASE( ms_pWaveWaterVB );
			return false;
		}

		memcpy( pVert, pTempWaveVB, ms_dwWaveWaterVBSize * sizeof(NKVERTEXLT) );
		ms_pWaveWaterVB->Unlock();
	}

	SAFE_DELETE_ARRAY( pTempBaseVB );
	SAFE_DELETE_ARRAY( pTempWaveVB );

	return true;
}


//-----------------------------------------------------------------------------
// Name : LoadObj()
// Desc :
//-----------------------------------------------------------------------------
bool CNKObjQuadNode::LoadObj( const TCHAR* szFilepath, const TCHAR* szFilename, IndexedTexture* pIndexedTexture )
{
	if( szFilename == NULL || pIndexedTexture == NULL )
		return FALSE;

	ms_pIndexedTexture = pIndexedTexture;
	ms_nBaseWaterSize = gMapExtention * 2;
	ms_nCellSize = gMapExtention * 4;
	ms_nTreeWidth = gMapWidth / 4;
	ms_nWaterHeightWidth = gMapWidth / 2;

	// Get a quadtree depth
	DWORD	dwTreeDepth = 0;
	DWORD	dwWidth = gMapWidth / 4;
	int		nRight = dwWidth;
	while( dwWidth >>= 1 )
		dwTreeDepth++;

	TCHAR	szFilePathName[512];

	// Load wat
	_tcscpy( szFilePathName, szFilepath );
	_tcscat( szFilePathName, szFilename );
	_tcscat( szFilePathName, ".wat" );
	if( !CNKObjQuadNode::LoadWat( szFilePathName ) )
		return false;

	// Load obj
	_tcscpy( szFilePathName, szFilepath);
	_tcscat( szFilePathName, szFilename );
	//_tcscat( szFilePathName, ".obj" ); // [2008/3/31 Theodoric] 인스톨 쉴드에서 확장자가 obj 가 있으면 에러가 남. 그래서 oba로 바꿈
	_tcscat( szFilePathName, ".oba" );


	PFILE*	fp;
	fp = pfopen( szFilePathName, "rb" );
	if( fp == NULL)
		return false;

	// Magic( OBJ + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic;
	pfread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'O' << 24 ) | ( 'B' << 16 ) | ( 'J' << 8 ) ) + 10 )
		return false;

	// Object tree
	g_pObjTree = new CNKObjQuadNode( dwTreeDepth, 0, 0, nRight, nRight, fp );

	pfclose( fp );

	return true;
}

//-- 물 파일을 Load 한다.
//-- 2005.01.21 / Lyul
//-- 저사양 루틴의 경우 물이 없을 경우 버그를 피하지 못한다. 수정
bool CNKObjQuadNode::LoadWat( const TCHAR* szFilename )
{
	SAFE_DELETE_ARRAY( ms_szWaterFilename );
	ms_szWaterFilename = new TCHAR[_tcslen( szFilename ) + 1];
	_tcscpy( ms_szWaterFilename, szFilename );
	ms_szWaterFilename[_tcslen( szFilename )] = NULL;

	PFILE*	fp;
	fp = pfopen( szFilename, "rb" );

	if( fp == NULL )
		return false;

	// Magic( WAT + Version ) - Version: 0.1 -> 1, 1.1 -> 11
	DWORD	dwMagic;
	pfread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'W' << 24 ) | ( 'A' << 16 ) | ( 'T' << 8 ) ) + 10 )
		return false;

	// Data of vertex buffer of base water
	pfread( &ms_dwBaseWaterVBSize, sizeof(DWORD), 1, fp );

	NKVERTEXLT*	pTempBaseVB = new NKVERTEXLT[ms_dwBaseWaterVBSize];
	pfread( pTempBaseVB, sizeof(NKVERTEXLT), ms_dwBaseWaterVBSize, fp );

	// Data of vertex buffer of wave water
	pfread( &ms_dwWaveWaterVBSize, sizeof(DWORD), 1, fp );
	NKVERTEXLT*	pTempWaveVB = new NKVERTEXLT[ms_dwWaveWaterVBSize];
	pfread( pTempWaveVB, sizeof(NKVERTEXLT), ms_dwWaveWaterVBSize, fp );

	// Water heights of each nodes
	ms_pWaterHeights = new float[ms_nWaterHeightWidth * ms_nWaterHeightWidth];
	pfread( ms_pWaterHeights, sizeof(float), ms_nWaterHeightWidth * ms_nWaterHeightWidth, fp );

	// Water textures
	int	nLength;
	TCHAR	szBaseName[512], szWave1Name[512], szWave2Name[512];
	pfread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		pfread( szBaseName, sizeof(TCHAR), nLength, fp );
	szBaseName[nLength] = NULL;
	pfread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		pfread( szWave1Name, sizeof(TCHAR), nLength, fp );
	szWave1Name[nLength] = NULL;
	pfread( &nLength, sizeof(int), 1, fp );
	if( nLength > 0 )
		pfread( szWave2Name, sizeof(TCHAR), nLength, fp );
	szWave2Name[nLength] = NULL;

	pfclose( fp );

	// Create textures
	ms_nBaseWaterTexID = ms_pIndexedTexture->CreateTextureFromFile( szBaseName );
	ms_nWaveWater1TexID = ms_pIndexedTexture->CreateTextureFromFile( szWave1Name );
	ms_nWaveWater2TexID = ms_pIndexedTexture->CreateTextureFromFile( szWave2Name );

	// Create index buffer
	SAFE_DELETE_ARRAY( ms_pBaseWaterIB );
	SAFE_DELETE_ARRAY( ms_pWaveWaterIB );
	DWORD	dwSize = pCMyApp->GetFrustum()->m_Far / ms_nBaseWaterSize;
	ms_pBaseWaterIB = new WORD[dwSize * dwSize * 6];
	dwSize = pCMyApp->GetFrustum()->m_Far / ( ms_nBaseWaterSize * 2 );
	ms_pWaveWaterIB = new WORD[dwSize * dwSize * 96];

	// Initialize texture matrix
	D3DUtil_SetIdentityMatrix( ms_matTexTrans );

	//--------------------------------------------------------------------------
	//-- High Quality - Create vertex buffer
	if( pCMyApp->CanHighQuality())
	{
		D3DVERTEXBUFFERDESC	VBDesc;
		VBDesc.dwSize = sizeof( VBDesc );
		VBDesc.dwCaps = D3DVBCAPS_WRITEONLY;
		VBDesc.dwFVF = NKVERTEXLT_FVF;
		VBDesc.dwNumVertices = ms_dwBaseWaterVBSize;

		//==============================================
		// nate 2004 - 12 : 샤일론 개편
		NKVERTEXLT*	pVert;

		if( ms_dwBaseWaterVBSize )
		{
			if( FAILED( GET_D3D()->CreateVertexBuffer( &VBDesc, &ms_pBaseWaterVB, 0 ) ) )
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );

			if( FAILED( ms_pBaseWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
											   (LPVOID*)(&pVert), NULL ) ) )
			{
				SAFE_RELEASE( ms_pBaseWaterVB );
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
			}

			memcpy( pVert, pTempBaseVB, ms_dwBaseWaterVBSize * sizeof(NKVERTEXLT) );
			ms_pBaseWaterVB->Unlock();
		}

		VBDesc.dwNumVertices = ms_dwWaveWaterVBSize;
		if( ms_dwWaveWaterVBSize )
		{
			if( FAILED( GET_D3D()->CreateVertexBuffer( &VBDesc, &ms_pWaveWaterVB, 0 ) ) )
			{
				SAFE_RELEASE( ms_pBaseWaterVB );
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
			}

			if( FAILED( ms_pWaveWaterVB->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
											   (LPVOID*)(&pVert), NULL ) ) )
			{
				SAFE_RELEASE( ms_pBaseWaterVB );
				SAFE_RELEASE( ms_pWaveWaterVB );
				return InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
			}

			memcpy( pVert, pTempWaveVB, ms_dwWaveWaterVBSize * sizeof(NKVERTEXLT) );
			ms_pWaveWaterVB->Unlock();
		}
		//==============================================
		SAFE_DELETE_ARRAY( pTempBaseVB );
		SAFE_DELETE_ARRAY( pTempWaveVB );
		return true;
	}
	//--------------------------------------------------------------------------
	//-- Low Quality - Create vertex buffer
	else
		return 	InitWaterVertexBufferInSysMem( pTempBaseVB, pTempWaveVB );
}


//-----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::Destroy()
{
	InvalidateDeviceObjects();
	SAFE_DELETE_ARRAY( ms_szWaterFilename );
	SAFE_DELETE_ARRAY( ms_pWaterHeights );
	SAFE_DELETE_ARRAY( ms_pBaseWaterIB );
	SAFE_DELETE_ARRAY( ms_pWaveWaterIB );
	SAFE_DELETE( g_pObjTree );
	ms_pOpenDeco = NULL;
}


//-----------------------------------------------------------------------------
// Name : PrepareRender()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::PrepareRender()
{
	static float	fBaseWaterTime = 0.0f;

	fBaseWaterTime += g_fTimeKey * ms_fBaseWaterSpeed;
	if( fBaseWaterTime > 1.0f )
		fBaseWaterTime -= 1.0f;

	ms_fWaveWater1Move += g_fTimeKey * ms_fWaveWater1Speed;
	if( ms_fWaveWater1Move > 1.0f )
		ms_fWaveWater1Move -= 1.0f;

	ms_fWaveWater2Move += g_fTimeKey * ms_fWaveWater2Speed;
	if( ms_fWaveWater2Move > 1.0f )
		ms_fWaveWater2Move -= 1.0f;

	ms_fBaseWaterMove = sinf( fBaseWaterTime * NK_TWOPI ) * 0.1f;
	ms_dwBaseIndexCount = 0;
	ms_dwWaveIndexCount = 0;
}


//-----------------------------------------------------------------------------
// Name : Render()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::Render( int nCullingState )
{
	if( nCullingState != VFCULL_INSIDE )
	{
		nCullingState = pCMyApp->GetFrustum()->CullAABBObj( m_vMin, m_vMax );
		if( nCullingState == VFCULL_OUTSIDE )
			return;
	}

	// Render if this node is lowest level node
	if( m_pLLeft == NULL )
	{
		BOOL						bAlpha = FALSE;
		BOOL						bOpenRoof = FALSE;
		CDecoObj*					pDecoObj = NULL;
		list<CDecoObj*>::iterator	iterDeco;


// 		if( m_lstDecoObj.empty() )
// 		{
// 			OutputDebugString( "m_lstDecoObj.empty()\n" );
// 			return;
// 		}

		for( iterDeco = m_lstDecoObj.begin(); iterDeco != m_lstDecoObj.end(); iterDeco++ )
		{
			pDecoObj = *iterDeco;
			if( !pDecoObj )
				continue;

//======================================
			// nate 2004 - 4
			// CastleWar
			// 비석, 나무 제거
			if( pCMyApp->m_nCurWorldIdx == WORLD_FIELD && 
				(pCMyApp->m_pUIMgr->m_bOnBattle || (DecardiBattleManager::GetInstance()->GetGate() == false)) )
			{
				if( pDecoObj->m_nLoloId == 246 )
				{
					if( ( int )pDecoObj->m_wx >= 5800 && ( int )pDecoObj->m_wx <= 6100 )
						continue;
				}
				else if( pDecoObj->m_nLoloId > 239 && pDecoObj->m_nLoloId < 247 )
					continue;
			}
//======================================

			// 화이트 혼 공성전때는 외성문과 크리스탈 탑을 적당히 보여줘야 한다.
			BOOL bClosedRender = FALSE;
			BOOL bFlagRender = TRUE;

			if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN && pCMyApp->m_pUIMgr->m_bOnBattle)
			{
				if (pDecoObj == g_CrystalTowerPoint)
					continue;

				for (int k = 0; k < 8; k++)
				{
					if (pDecoObj == g_OutsideDoorPoint[k] && !g_OutsideDoorOpen[k])
					{
						bClosedRender = TRUE;
						break;
					}
				}

				for ( int k = 0; k < 4; k++)
				{
					if (pDecoObj == g_CastleFlagPoint[k])
					{
						bFlagRender = FALSE;
						break;
					}
				}

				if (!bFlagRender)
					continue;
			}
			// Distance culling
			float	fDecoDist = DistPlToPlAbs( g_pRoh->m_wx, g_pRoh->m_wz,
											   pDecoObj->m_wx, pDecoObj->m_wz );

			const float BLD_FLOOR_Fist = 3.5f; // [2007/11/5 theodoric] 바닥 속성 추가

			// [2007/11/5 theodoric] 바닥 속성 추가.
			if( fDecoDist <= ms_fRenderDist ||
					( g_pDecoObjSet->m_pnObjType[pDecoObj->m_nLoloId] == BLD_FLOOR && fDecoDist <= ms_fRenderDist * BLD_FLOOR_Fist))
			{
				// View frustum culling
				if( nCullingState == VFCULL_INSIDE ||
						pCMyApp->GetFrustum()->FrustumCull( pDecoObj->m_vBoundBox, 8 ) != VFCULL_OUTSIDE )
				{
					// Check if the roof of a decoobj is opened
					if( g_pDecoObjSet->m_pnObjType[pDecoObj->m_nLoloId] <= BLD_HOUSE )
					{
						bOpenRoof = pDecoObj->IsInside( ms_vCameraAt.x, ms_vCameraAt.z );

						if( bOpenRoof && pDecoObj->m_pLolo->m_WalkPlaneCount == 0 )
						{
							ms_pOpenDeco = pDecoObj;
							pCMyApp->m_pWeatherMgr->StopWeatherSystem();
						}
					}
					else
					{
						// [2007/11/5 theodoric] 바닥 속성 추가
						if( g_pDecoObjSet->m_pnObjType[pDecoObj->m_nLoloId] != BLD_FLOOR )
							bOpenRoof = FALSE;
					}

					// Check if a character is hidden by a decoobj
					if( g_pDecoObjSet->m_pnObjType[pDecoObj->m_nLoloId] <= BLD_TREE )
					{
						for( int i = 6; i > 1; i-- )
						{
							bAlpha = pDecoObj->IsInside( ms_vCameraAt.x + ( ms_vCameraEye.x - ms_vCameraAt.x ) * i / 6,
														 ms_vCameraAt.z + ( ms_vCameraEye.z - ms_vCameraAt.z ) * i / 6 );
							if( bAlpha )
								break;
						}
					}
					else if ( pCMyApp->m_nCurWorldIdx == WORLD_DUNGEON1 && pDecoObj->m_nLoloId == 37 ) // 인던 통로의 해골문 오브젝트는 타입이 ETC다 그래서 위의 루틴을 안타서 반투명이 안됨.(BLD 로 바꿀까도 생각했지만 맵툴상에 '못들어감' 이라고 써있어서 그냥 예외처리함.\) ///인던1
					{
						for( int i = 6; i > 1; i-- )
						{
							bAlpha = pDecoObj->IsInside( ms_vCameraAt.x + ( ms_vCameraEye.x - ms_vCameraAt.x ) * i / 6,
														 ms_vCameraAt.z + ( ms_vCameraEye.z - ms_vCameraAt.z ) * i / 6 );
							if( bAlpha )
								break;
						}
					}
					else
						bAlpha = FALSE;

					if( bOpenRoof && bAlpha )
						bAlpha = FALSE;

					// [2007/11/5 theodoric] 바닥 속성 추가
					if( g_pDecoObjSet->m_pnObjType[pDecoObj->m_nLoloId] == BLD_FLOOR && fDecoDist > ms_fBlendDist*BLD_FLOOR_Fist)
					{
						pDecoObj->m_farFactor = ( ms_fRenderDist*BLD_FLOOR_Fist - fDecoDist ) / ( ms_fRenderDist*BLD_FLOOR_Fist - (ms_fBlendDist)*BLD_FLOOR_Fist );
						g_listFarDeco.push_back( pDecoObj );

						if( pDecoObj->m_LineEffectsLoad )
							pDecoObj->DeleteEffectNum();

					}
					else if( g_pDecoObjSet->m_pnObjType[pDecoObj->m_nLoloId] != BLD_FLOOR && fDecoDist > ms_fBlendDist )
					{
						pDecoObj->m_farFactor = ( ms_fRenderDist - fDecoDist ) / ( ms_fRenderDist - ms_fBlendDist );
						g_listFarDeco.push_back( pDecoObj );

						if( pDecoObj->m_LineEffectsLoad )
							pDecoObj->DeleteEffectNum();
					}
					else if( bAlpha )
						g_listAlphaDeco.push_back( pDecoObj );
					else if( pDecoObj->MustRenderLast() )
						g_listLastDeco.push_back( pDecoObj );
					else
					{
						// 공성중 주성문은 닫힌상태로 보여주자.
						if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN && pCMyApp->m_pUIMgr->m_bOnBattle)
						{
							if (pDecoObj->m_nLoloId == LOLO_CASTLE_GATE_OPEND)
							{
								g_pCastleGateClosed->m_dir = pDecoObj->m_dir;
								g_pCastleGateClosed->m_wx = pDecoObj->m_wx;
								g_pCastleGateClosed->m_wy = pDecoObj->m_wy;
								g_pCastleGateClosed->m_wz = pDecoObj->m_wz;
								g_pCastleGateClosed->Render(bOpenRoof, FALSE);
							}
							else if (bClosedRender)
							{
								g_pOutsideDoorClosed->m_dir = pDecoObj->m_dir;
								g_pOutsideDoorClosed->m_wx = pDecoObj->m_wx;
								g_pOutsideDoorClosed->m_wy = pDecoObj->m_wy;
								g_pOutsideDoorClosed->m_wz = pDecoObj->m_wz;
								g_pOutsideDoorClosed->Render(bOpenRoof, FALSE);
							}
							else	// 걍 아래 그려주는 루틴하고 같다.
								pDecoObj->Render(bOpenRoof );
						}
						//wan:2004-12
						//Whitehorn Dungeon
						else if (pCMyApp->m_nCurWorldIdx == WORLD_DUNGEON4)
						{
							if (pDecoObj->m_nLoloId == LOLO_BOSS_GATE_CLOSED)  //보스방 닫힌 롤로 그려줌
							{
								if(pCMyApp->m_pUIMgr->m_bBossGateClosedRender) //닫혔을때
								{
									g_pBossGateClosed->m_dir = pDecoObj->m_dir;
									g_pBossGateClosed->m_wx = pDecoObj->m_wx;
									g_pBossGateClosed->m_wy = pDecoObj->m_wy;
									g_pBossGateClosed->m_wz = pDecoObj->m_wz;
									g_pBossGateClosed->Render(bOpenRoof, FALSE);
								}//열렸을때는 안그려줌
							}
							else
								pDecoObj->Render( bOpenRoof );
						}
						////////////////////////////////////////////////////////////////////////////////////
						// 샤일론도 신맵툴로 개편되어 여기로 처리를 옮김.(by 원석)
						else if (pCMyApp->GetCurWorld() == WORLD_GUILD && pDecoObj->m_nLoloId == 0) // 샤일론에서 앞문이면..
						{
							if (pCMyApp->m_pUIMgr->m_nCastleDoorState == 0 || pCMyApp->m_pUIMgr->m_nCastleDoorState == 2) // 닫힌 상태일때는 다른 녀석으로 대체해서 그려준다.
							{
								// 닫힌 문 그리기.
								g_pGuildGateClosed->m_dir = pDecoObj->m_dir;
								g_pGuildGateClosed->m_wx = pDecoObj->m_wx;
								g_pGuildGateClosed->m_wy = pDecoObj->m_wy;
								g_pGuildGateClosed->m_wz = pDecoObj->m_wz;
								g_pGuildGateClosed->Render(bOpenRoof, FALSE);
							}
							else
								pDecoObj->Render(bOpenRoof);
						}
						else if (pCMyApp->GetCurWorld() == WORLD_GUILD && pDecoObj->m_nLoloId == 101) // 샤일론에서 뒷문이면..(평상시는 닫혀 있음.)
						{
							if (pCMyApp->m_pUIMgr->m_nCastleDoorState == 2 || pCMyApp->m_pUIMgr->m_nCastleDoorState == 3) // 열린 상태일때는 다른 녀석으로 대체해서 그려준다.
							{
								// 열린 문 그리기. (평상시는 닫혀 있음.)
								g_pGuildGate2Opened->m_dir = pDecoObj->m_dir+3.14f;
								g_pGuildGate2Opened->m_wx = pDecoObj->m_wx;
								g_pGuildGate2Opened->m_wy = pDecoObj->m_wy;
								g_pGuildGate2Opened->m_wz = pDecoObj->m_wz;

								g_pGuildGate2Opened->Render(bOpenRoof, FALSE);
							}
							else
								pDecoObj->Render(bOpenRoof);
						}
						else
							pDecoObj->Render(bOpenRoof );
					}

					g_listRenderedDecoColor.push_back( pDecoObj->m_wColorIdx );
					g_listRenderedDeco.push_back( pDecoObj );
				}
				else
				{
					if( pDecoObj->m_LineEffectsLoad )
						pDecoObj->DeleteEffectNum();
				}
			}
			else
			{
				if( pDecoObj->m_LineEffectsLoad )
					pDecoObj->DeleteEffectNum();
			}
			pDecoObj = NULL;
		}

		if( m_nBaseWaterSegment > 0 )
		{
			WORD	wVBIndex = m_wBaseVBIndex;
			for( int i = 0; i < m_nBaseWaterSegment; i++ )
			{
				ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 2;
				ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 3;
				ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex;
				ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex;
				ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 3;
				ms_pBaseWaterIB[ms_dwBaseIndexCount++] = wVBIndex + 1;
				wVBIndex += 4;
			}

			if( m_nWaveWater1Segment > 0 )
			{
				wVBIndex = m_wWaveVBIndex;
				for( int i = 0; i < m_nWaveWater1Segment; i++ )
				{
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
					ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
					wVBIndex += 3;
				}

				if( m_nWaveWater2Segment > 0 )
				{
					wVBIndex += 3;
					for( int i = 0; i < m_nWaveWater2Segment; i++ )
					{
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 3;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 1;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 4;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 2;
						ms_pWaveWaterIB[ms_dwWaveIndexCount++] = wVBIndex + 5;
						wVBIndex += 3;
					}
				}
			}
		}
		return ;
	}

	// Recursive call
	if( m_pLLeft ) m_pLLeft->Render( nCullingState );
	if( m_pLRight ) m_pLRight->Render( nCullingState );
	if( m_pULeft ) m_pULeft->Render( nCullingState );
	if( m_pURight ) m_pURight->Render( nCullingState );
}

//-- 물과 웨이브(하이퀄리티) 를 구현한다.
//-- 2005.01.21 / Lyul
//-- 화이트 혼 던젼에서는 물을 표현할 필요가 없다.
//-- 따라서 바로 return 하여 속도를 올리자.
void CNKObjQuadNode::RenderWater()
{
	//-- 물을 표현할 필요가 없다면
	if(!ms_dwBaseIndexCount || !GET_D3DDEVICE())
		return;

	// Get texture stage state
	DWORD	dwColorOp, dwColorArg1, dwColorArg2, dwAlphaOp, dwAlphaArg1;
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg1 );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_COLORARG2, &dwColorArg2 );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg1 );

	// Get render state
	DWORD	dwSrcBlend, dwDestBlend;
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestBlend );

	// Set texture stage state
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

	// Set render state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, FALSE );

	// Set world transform
	D3DMATRIX	matWorld;
	D3DUtil_SetIdentityMatrix( matWorld );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	//--------------------------------------------------------------------------
	// Base water
	if( ms_dwBaseIndexCount > 0 )
	{
		// Set texture stage state
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

		// Set render state
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

		GET_D3DDEVICE()->SetTexture( 0, ms_pIndexedTexture->GetSurfaceByIdx( ms_nBaseWaterTexID ) );
		ms_matTexTrans._31 = ms_fBaseWaterMove;
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &ms_matTexTrans );
		GET_D3DDEVICE()->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, ms_pBaseWaterVB, 0, ms_dwBaseWaterVBSize,
				ms_pBaseWaterIB, ms_dwBaseIndexCount, 0 );
	}

	//--------------------------------------------------------------------------
	// Wave water
	if( pCMyApp->CanHighQuality() && ms_dwWaveIndexCount > 0 )
	{
		// Set texture stage state
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Set render state
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		ms_matTexTrans._31 = ms_fWaveWater1Move;
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &ms_matTexTrans );
		GET_D3DDEVICE()->SetTexture( 0, ms_pIndexedTexture->GetSurfaceByIdx( ms_nWaveWater1TexID ) );
		GET_D3DDEVICE()->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, ms_pWaveWaterVB, 0, ms_dwWaveWaterVBSize,
				ms_pWaveWaterIB, ms_dwWaveIndexCount, 0 );

		ms_matTexTrans._31 = ms_fWaveWater2Move;
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &ms_matTexTrans );
		GET_D3DDEVICE()->SetTexture( 0, ms_pIndexedTexture->GetSurfaceByIdx( ms_nWaveWater2TexID ) );
		GET_D3DDEVICE()->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, ms_pWaveWaterVB, 0, ms_dwWaveWaterVBSize,
				ms_pWaveWaterIB, ms_dwWaveIndexCount, 0 );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	}

	ms_matTexTrans._31 = 0.0f;
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_TEXTURE0, &ms_matTexTrans );

	// Restore render state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestBlend );

	// Restore texture stage state
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg1 );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG2, dwColorArg2 );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwAlphaOp );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg1 );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
}


//-----------------------------------------------------------------------------
// Name : SetD3DDevice()
// Desc :
//-----------------------------------------------------------------------------
void	CNKObjQuadNode::SetD3DDevice()
{
}


//-----------------------------------------------------------------------------
// Name : GetHeightOnWater()
// Desc :
//-----------------------------------------------------------------------------
float CNKObjQuadNode::GetHeightOnWater( float fZ, float fX )
{
	int	nX = fX / ms_nBaseWaterSize;
	int	nZ = fZ / ms_nBaseWaterSize;
	//wan:2004-11
	//Whitehorn Dungeon
	//물이 없으면 -1.0f로 리턴
	if( nX < 0 || nZ < 0
			|| nX >= ms_nWaterHeightWidth
			|| nZ >= ms_nWaterHeightWidth
			||ms_dwBaseWaterVBSize==0
			|| ms_pWaterHeights == NULL)
		return -1.0f;

	return ms_pWaterHeights[nX + nZ * ms_nWaterHeightWidth];
}

//-----------------------------------------------------------------------------
// Name : GetHeightOnObject()
// Desc :
//-----------------------------------------------------------------------------
float CNKObjQuadNode::GetHeightOnObject( float fZ, float fX, float fY )
{
	int	nX = fX / ms_nCellSize;
	int	nZ = fZ / ms_nCellSize;

	if( nX < 2 || nZ < 2 || nX >= ms_nTreeWidth - 2 || nZ >= ms_nTreeWidth - 2 )
		return -1.0f;

	int							x, z, i, nVert;
	float						fDist;//, fHeight;
	D3DVECTOR					vP0, vP1, vP2;
	D3DVECTOR					vOrig = D3DVECTOR( fX, fY, fZ );
	D3DVECTOR					vDir = D3DVECTOR( 0.0f, 1.0f, 0.0f );
	CDecoObj*					pDecoObj = NULL;
	list<CDecoObj *>::iterator	iterDeco;
	int	sx = nX - 2;
	int	sz = nZ - 2;
	int	ex = nX + 2;
	int	ez = nZ + 2;

	for( z = sz; z <= ez; z++ )
	{
		for( x = sx; x <= ex; x++ )
		{
			for( iterDeco = g_listDecoObj[z][x].begin(); iterDeco != g_listDecoObj[z][x].end(); iterDeco++ )
			{
				pDecoObj = *iterDeco;
				if( !pDecoObj )
					continue;
				nVert = pDecoObj->m_pLolo->m_WalkPlaneCount * 4;
				for( i = 0; i < nVert; i += 4 )
				{
					vP0 = pDecoObj->m_WalkPlaneVect[i];
					vP1 = pDecoObj->m_WalkPlaneVect[i + 1];
					vP2 = pDecoObj->m_WalkPlaneVect[i + 2];

					if( NKIntersectRayTri( vOrig, vDir, vP0, vP1, vP2, &fDist ) )
						return vDir.y * fDist + fY;

					vP0 = pDecoObj->m_WalkPlaneVect[i + 3];

					if( NKIntersectRayTri( vOrig, vDir, vP2, vP1, vP0, &fDist ) )
						return vDir.y * fDist + fY;
				}
				pDecoObj = NULL;
			}
		}
	}

	return -1.0f;
}


//-----------------------------------------------------------------------------
// Name : SetRenderDist()
// Desc :
//-----------------------------------------------------------------------------
void CNKObjQuadNode::SetRenderDist( float fRenderDist )
{
	ms_fRenderDist = fRenderDist;
	pCMyApp->GetFrustum()->InitObjFrustum( ms_fRenderDist );
}

