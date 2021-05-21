#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include "main.h"
#include "DecoObj.h"
#include "GUtil.h"
#include "ClrAlloc.h"
#include "D3DMath.h"
#include "Land.h"
#include "IndexedTexture.h"
#include "FxSet.h"
#include "NKObjQuadNode.h"
// nate 2004 - 4 : CastleWar
#include "UIMgr.h"
#include "Mouse.h"
#include "headers.h"
#include <list>
#include <iterator>
#include "DecardiBattleManager.h"
using namespace std;

// 전역 함수와 전역 변수 : 포팅할때 증요함.
CDecoObjSet	*g_pDecoObjSet;
CDecoObj	*g_pGuildGateClosed = NULL, *g_pGuildGate2Opened = NULL;				// 성문 - 특별히 따로 관리.
CDecoObj	*g_pOutsideDoorClosed = NULL, *g_pCastleGateClosed = NULL;				// 성문 - 특별히 따로 관리.
CDecoObj	*g_pBossGateClosed = NULL;	//wan:2004-12 :Whitehonr Dungeon - 화이트혼 던젼 보스 문

extern CClrAlloc		 *g_ClrAlloc;
extern float g_fTimeKey;
// 맵을 셀로 나눈 것.
extern int g_nMapCellWidth;
extern int g_nMapCellHeight;
extern int g_nMapCellWidthCount;
extern int g_nMapCellHeightCount;

// 2004
extern BOOL	g_bNewTerrain;


#define DECO_FLY_FRAME	40

// nate 2004 - 4 : CastleWar
BYTE g_byEndCastleWar = 0;
float g_DecoFlyHeight[DECO_FLY_FRAME];

list<CDecoObj *> g_listDecoObj[MAP_CELL_HEIGHT_COUNT][MAP_CELL_WIDTH_COUNT];

BOOL InsertDecoObjToMaplist(CDecoObj *pDecoObj) ;
void DeleteDecoObjData();

BOOL IsBrightObj( int World, int ObjId ) // 게이트가 아닌 종류중에 마우스 오버하면 밝아져야하는 종류의 오브젝트를 지정...
{
	if( World == WORLD_START )
	{
		if( ObjId == 143 ) // 출석체크 피파컵   OLD OBJ LAGLAMIA
			return TRUE;
	}

	return FALSE;
}


BOOL IsHeroStone( int World, int ObjId )
{
	if( g_SvrType == ST_ADULT_ONLY)
		return FALSE;

	if( World == WORLD_START )
	{
		switch( ObjId )
		{
		case 150:
		case 151:
		case 152:
		case 153:
		case 154:
			return TRUE;
		}
	}

	return FALSE;
}

void DecoObjInitialLoading( char* strFilePath, char* strFileName )
{
	PFILE *fp = NULL;
	int listNum = 0;
	CDecoObj *pDecoObj = NULL;
	char *strFilePathName = NULL;

	// 기존에 로딩되어 있는 것들을 모두 지운다.
	DeleteDecoObjData();

	// [12/18/2008 D.K ] : memory leak detected.
	strFilePathName = new char[255];

	// 현재 건물들의 텍스처는 모두 한 곳에.
	if( pCMyApp->m_pDecoTexture )
		pCMyApp->m_pDecoTexture->SetTexturePath("data/building/textr/");

	SAFE_DELETE(g_pDecoObjSet);
	g_pDecoObjSet = new CDecoObjSet(pCMyApp->m_pDecoTexture);

	// 롤로 디렉토리 설정.
	strcpy(strFilePathName, strFilePath);
	strcat(strFilePathName, "lolos/");
	if( g_pDecoObjSet )
		g_pDecoObjSet->SetLolosDir(strFilePathName);

	// bld 화일 설정.
	if( g_bNewTerrain )
	{
		strcpy( strFilePathName, strFilePath );
		strcat( strFilePathName, strFileName );
		strcat( strFilePathName, ".bld" );
		if( g_pDecoObjSet )
			g_pDecoObjSet->ReadDataFile( strFilePathName, TRUE );

		if( !CNKObjQuadNode::LoadObj( strFilePath, strFileName, pCMyApp->m_pDecoTexture ) )
		{
			SAFE_DELETE_ARRAY( strFilePathName );
			return ;
		}
	}
	else
	{
		strcpy(strFilePathName, strFilePath);
		strcat(strFilePathName, "bld.bld");
		if( g_pDecoObjSet )
			g_pDecoObjSet->ReadDataFile(strFilePathName);

		// 데코오브젝트 셋을 읽었으면 월드상에 위치한 정보를 읽는다.
		strcpy(strFilePathName, strFilePath);
		strcat(strFilePathName, "DecoObjList.lst");
		fp = pfopen(strFilePathName, "rt");

		if (fp == NULL)
		{
			SAFE_DELETE_ARRAY( strFilePathName );
			return;
		}

		// 목록의 갯수를 읽어온다.
		pfscanf(fp, "%d", &listNum);

		for (int i=0; i < listNum; i++)
		{
			pDecoObj = new CDecoObj(g_ClrAlloc);
			if( !pDecoObj )
				continue;

			// id, x, z, y, dir
			pfscanf(fp, "%d %f %f %f %f", &(pDecoObj->m_nLoloId), &(pDecoObj->m_wx), &(pDecoObj->m_wz), \
					&(pDecoObj->m_wy), &(pDecoObj->m_dir));
			pDecoObj->m_pLolo = &(g_pDecoObjSet->m_pLoloSet[pDecoObj->m_nLoloId]);
			if( !pDecoObj->m_pLolo )
				continue;

			if( pDecoObj->m_pLolo->m_LineVector0
					&& ( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_FIRE
						 || pDecoObj->m_pLolo->m_LineType == LINE_TYPE_BLUEFIRE ))
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_SMOKE
					 && pDecoObj->m_pLolo->m_LineVector0)
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_VOLCANO && pDecoObj->m_pLolo->m_LineVector0)
			{
				pDecoObj->Spray_Particle = new Deco_Particle[MAX_SPRAY_PARTICLE];

				int spray_particle_start_time[MAX_SPRAY_PARTICLE] = { 0, };//0, 10, 15, 20, 25, 30, 35, 40, 45 };

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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_POISON && pDecoObj->m_pLolo->m_LineVector0)
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_GAS && pDecoObj->m_pLolo->m_LineVector0)
			{
				pDecoObj->Light_Particle = new Deco_Particle[MAX_LIGHT_PARTICLE];

				int light_particle_start_time[MAX_LIGHT_PARTICLE] = { 0, 6, 12, 18, 24, 30, 36, 42, 48, 54 };

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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_ELECTRIC && pDecoObj->m_pLolo->m_LineVector0)
			{
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_RED && pDecoObj->m_pLolo->m_LineVector0)
			{
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINEB && pDecoObj->m_pLolo->m_LineVector0)
			{
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINES && pDecoObj->m_pLolo->m_LineVector0)
			{
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_CYON && pDecoObj->m_pLolo->m_LineVector0)
			{
				pDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

				for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
				{
					pDecoObj->ClystalGlow[i].start_time= 0;
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
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_WATERFALLD && pDecoObj->m_pLolo->m_LineVector0)
			{
				pDecoObj->Waterfall = new Deco_Particle[MAX_WATERFALL];

				for( int i = 0; i < MAX_WATERFALL; i++ )
				{
					pDecoObj->Waterfall[i].start_time	= 0;
					pDecoObj->Waterfall[i].life_time	= 0;
					pDecoObj->Waterfall[i].cur_time		= 0;
					pDecoObj->Waterfall[i].scale		= 0;
					pDecoObj->Waterfall[i].fade			= 0.5f;

					pDecoObj->Waterfall[i].red			= 1.0f;
					pDecoObj->Waterfall[i].green		= 1.0f;
					pDecoObj->Waterfall[i].blue			= 1.0f;

					pDecoObj->Waterfall[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Waterfall[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Waterfall[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
					pDecoObj->Waterfall[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

					pDecoObj->Waterfall[i].texIndex		= 0;
					pDecoObj->Waterfall[i].texFrame		= 0;
				}
			}
			else if( pDecoObj->m_pLolo->m_LineType == LINE_TYPE_WATERFALL && pDecoObj->m_pLolo->m_LineVector0)
			{
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
				pDecoObj->PortalBossraid = new Deco_Particle[MAX_PORTAL_BOSSRAID_PARTICLE];

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
			InsertDecoObjToMaplist(pDecoObj);
			pDecoObj->CalBoundBox();
		}
		pfclose(fp);
	}

	SAFE_DELETE_ARRAY( strFilePathName );

	// 공성전을 위해서 무조건 로딩한다.
	if (pCMyApp->GetCurWorld() == WORLD_GUILD)
	{
		SAFE_DELETE(g_pGuildGateClosed);
		SAFE_DELETE(g_pGuildGate2Opened);
		g_pGuildGateClosed = new CDecoObj(g_ClrAlloc);
		g_pGuildGate2Opened = new CDecoObj(g_ClrAlloc);
		if( g_pGuildGateClosed && g_pDecoObjSet )
		{
			g_pGuildGateClosed->m_nLoloId = 221;
			g_pGuildGateClosed->m_pLolo = &(g_pDecoObjSet->m_pLoloSet[g_pGuildGateClosed->m_nLoloId]);
		}
		if( g_pGuildGate2Opened && g_pDecoObjSet )
		{
			g_pGuildGate2Opened->m_nLoloId = 222;
			g_pGuildGate2Opened->m_pLolo = &(g_pDecoObjSet->m_pLoloSet[g_pGuildGate2Opened->m_nLoloId]);
		}
	}
	if (pCMyApp->GetCurWorld() == WORLD_WHITEHORN)
	{
		SAFE_DELETE(g_pOutsideDoorClosed);
		SAFE_DELETE(g_pCastleGateClosed);
		g_pOutsideDoorClosed = new CDecoObj(g_ClrAlloc);
		g_pCastleGateClosed = new CDecoObj(g_ClrAlloc);
		if( g_pOutsideDoorClosed && g_pDecoObjSet )
		{
			g_pOutsideDoorClosed->m_nLoloId = LOLO_OUTSIDE_DOOR_CLOSED; // 화이트혼 외성문 닫힌 모양
			g_pOutsideDoorClosed->m_pLolo = &(g_pDecoObjSet->m_pLoloSet[g_pOutsideDoorClosed->m_nLoloId]);
		}
		if( g_pCastleGateClosed && g_pDecoObjSet )
		{
			g_pCastleGateClosed->m_nLoloId = LOLO_CASTLE_GATE_CLOSED; // 화이트혼 주성문 닫힌모양
			g_pCastleGateClosed->m_pLolo = &(g_pDecoObjSet->m_pLoloSet[g_pCastleGateClosed->m_nLoloId]);
		}
	}
	//wan:2004-12 :Whitehorn Dungeon
	if (pCMyApp->GetCurWorld() == WORLD_DUNGEON4)
	{
		SAFE_DELETE(g_pBossGateClosed);
		g_pBossGateClosed = new CDecoObj(g_ClrAlloc);
		if( g_pBossGateClosed && g_pDecoObjSet )
		{
			g_pBossGateClosed->m_nLoloId = LOLO_BOSS_GATE_CLOSED; // 화이트혼 던젼 보스방 닫힌모양
			g_pBossGateClosed->m_pLolo = &(g_pDecoObjSet->m_pLoloSet[g_pBossGateClosed->m_nLoloId]);
		}
	}

	// 하늘에서 붕붕거리는 높이를 미리 계산해놓자.
	for( int i = 0; i < DECO_FLY_FRAME; i++ )
		g_DecoFlyHeight[i] = sinf(g_2_PI * i / DECO_FLY_FRAME) * 10 + 10;
}

//////////////////////////////////////////////////////////////////////////////////////
// Name : InsertDecoObjToMaplist()
// Desc : Decoration Object들을 맵 리스트에 집어 넣는다.
//////////////////////////////////////////////////////////////////////////////////////
BOOL InsertDecoObjToMaplist(CDecoObj *pDecoObj)
{
	int w_pos = 0, h_pos = 0;

	w_pos = (int) (pDecoObj->m_wx);
	h_pos = (int) (pDecoObj->m_wz);

	w_pos /= g_nMapCellWidth;
	h_pos /= g_nMapCellHeight;

	if (w_pos < 0 || h_pos < 0)
		return FALSE;

	if ((w_pos < g_nMapCellWidthCount) && (h_pos < g_nMapCellHeightCount))
	{
		list<CDecoObj *>::iterator pr = g_listDecoObj[h_pos][w_pos].begin();
		g_listDecoObj[h_pos][w_pos].insert(pr, pDecoObj);
	}

	return TRUE;
}

void DeleteDecoObjData()
{
	CNKObjQuadNode::Destroy();

	CDecoObj *pDecoObj = NULL;
	list<CDecoObj *>::iterator it;

	for (int h = 0; h < MAP_CELL_HEIGHT_COUNT; h++)
	{
		for (int w = 0; w < MAP_CELL_WIDTH_COUNT; w++)
		{
			list<CDecoObj *>::iterator itEnd = g_listDecoObj[h][w].end();
			for (it = g_listDecoObj[h][w].begin(); it != itEnd; it++)
			{
				pDecoObj = *it;
				SAFE_DELETE( pDecoObj );
			}

			g_listDecoObj[h][w].clear();
		}
	}

	if (pCMyApp->GetCurWorld() == WORLD_GUILD)
	{
		SAFE_DELETE(g_pGuildGateClosed);
		SAFE_DELETE(g_pGuildGate2Opened);
	}

	SAFE_DELETE( g_pDecoObjSet );
}

//////////////////////////////////////////////////////////////////////
// CDecoObj Construction/Destruction
//////////////////////////////////////////////////////////////////////
D3DMATRIX CDecoObj::m_matWorld;
D3DMATRIX CDecoObj::m_matTrans;
D3DMATRIX CDecoObj::m_matRotate;
float	  CDecoObj::m_fAniRate = 0.1f;

CDecoObj::CDecoObj(CClrAlloc *pClrAlloc)
	: m_nLoloId(0)
	, m_wx(0)
	, m_wy(0)
	, m_wz(0)
	, m_dir(0)
	, PortalBossraid(NULL)
	, m_farFactor(0)
{
	m_pLolo = NULL;
	m_pClrAlloc = pClrAlloc;
	m_wColorIdx = 0;
	m_dwColorPick = 0;

	if (m_pClrAlloc)
		m_pClrAlloc->ColorAlloc(&m_dwColorPick, &m_wColorIdx);
	else
	{
		m_wColorIdx = 0;
		m_dwColorPick = 0x00ffffff;
	}

	m_nCurFrame = 0;
	m_fTimeElapsed = 0.0f;
	m_bOpenedRoof = FALSE;
	m_WalkPlaneVect = NULL;

	m_LineEffectsLoad = FALSE;

	Fire_Particle = NULL;
	Smoke_Particle = NULL;
	Spray_Particle = NULL;
	Turn_Particle = NULL;
	Light_Particle = NULL;
	Electric_Particle = NULL;
	ClystalGlow = NULL;
	Waterfall = NULL;
	Waterfall_Particle = NULL;
	Firesmoke_Particle = NULL;

	m_nFlyFrame = rand() % DECO_FLY_FRAME;
}


CDecoObj::~CDecoObj()
{
	if (m_pClrAlloc)	// 할당받은 컬라값을 해제한다.
		m_pClrAlloc->ColorFree(m_wColorIdx);

	if( m_LineEffectsLoad )
		DeleteEffectNum();

	SAFE_DELETE_ARRAY( Fire_Particle ); // 배열 딜리트로 바꿈.(배열로 new 하고 그냥 delete만 하고있었다. 이렇게 하면 메모리 누수 나지않는가?) (06-11-20 원석)
	SAFE_DELETE_ARRAY(Smoke_Particle);
	SAFE_DELETE_ARRAY(Spray_Particle);
	SAFE_DELETE_ARRAY(Turn_Particle);
	SAFE_DELETE_ARRAY(Light_Particle);
	SAFE_DELETE_ARRAY(m_WalkPlaneVect);
	SAFE_DELETE_ARRAY(Electric_Particle);
	SAFE_DELETE_ARRAY(ClystalGlow);
	SAFE_DELETE_ARRAY(Waterfall);
	SAFE_DELETE_ARRAY(Waterfall_Particle);
	SAFE_DELETE_ARRAY(Firesmoke_Particle);
}

void CDecoObj::InsertEffectNum(int num)
{
	m_listEffect.insert(m_listEffect.begin(), num);
}
void CDecoObj::DeleteEffectNum()
{
	// 관련된 효과를 없앤다.
	list<int>::iterator it;
	it = m_listEffect.begin();
	int idx = 0;
	while (it != m_listEffect.end())
	{
		idx = *it;

		if( pCMyApp->m_pFxSet )
			pCMyApp->m_pFxSet->DeleteEffect(idx);

		it = m_listEffect.erase(it);
		m_LineEffectsLoad = FALSE;
	}
}

void CDecoObj::CalBoundBox()
{
	D3DUtil_SetTranslateMatrix(m_matTrans, m_wx, m_wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix(m_matRotate, m_dir);
	D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matTrans );

	for (int i=0; i < 8; i++)
		D3DMath_VectorMatrixMultiply(m_vBoundBox[i], m_pLolo->m_BoundVect[i] , m_matWorld);

	if( m_pLolo && m_pLolo->m_WalkPlaneCount > 0 )
	{
		if( m_WalkPlaneVect == NULL )
			m_WalkPlaneVect = new D3DVECTOR[m_pLolo->m_WalkPlaneCount * 4];

		for( int i = 0; i < m_pLolo->m_WalkPlaneCount * 4; i++ )
			D3DMath_VectorMatrixMultiply( m_WalkPlaneVect[i], m_pLolo->m_WalkPlaneVect[i], m_matWorld );
	}
}

// ----------------------------------------------------------------
// Desc : 월드 좌표 x, z가 오브젝트 내에 포함되어 있는지 확인한다.
// ----------------------------------------------------------------
BOOL CDecoObj::IsInside(float wx, float wz)
{
	int bCCW[4];
	// (AB) 와 (AC) 벡터의 외적을 구하여 방향을 세 점의 위치 관계를 판다.
	bCCW[0] = Nk3DMath_CCW(m_vBoundBox[0].x, m_vBoundBox[0].z, m_vBoundBox[1].x, m_vBoundBox[1].z,  wx,  wz);
	bCCW[1] = Nk3DMath_CCW(m_vBoundBox[1].x, m_vBoundBox[1].z, m_vBoundBox[2].x, m_vBoundBox[2].z,  wx,  wz);
	bCCW[2] = Nk3DMath_CCW(m_vBoundBox[2].x, m_vBoundBox[2].z, m_vBoundBox[3].x, m_vBoundBox[3].z,  wx,  wz);
	bCCW[3] = Nk3DMath_CCW(m_vBoundBox[3].x, m_vBoundBox[3].z, m_vBoundBox[0].x, m_vBoundBox[0].z,  wx,  wz);

	if ( abs(bCCW[0] = (bCCW[0] + bCCW[1] + bCCW[2] + bCCW[3])) == 4)
		return TRUE;
	else
		return FALSE;
}

HRESULT CDecoObj::Render( BOOL bNoRoof, int whichAlpha)
{
	if( !GET_D3DDEVICE() || !m_pLolo )
		return S_FALSE;

	//메트릭스 롤러 관려(mungmae-03/22)
	if (pCMyApp->GetCurWorld() == WORLD_MATRIX)
	{
		if( RenderMatrixLolo() == S_OK )
			return S_OK;
	}
	else if( pCMyApp->GetCurWorld() == WORLD_MATRIX_2009 )
	{
		if( RenderMatrix2009Lolo() == S_OK )
			return S_OK;
	}
//=============================================
	// nate 2004 - 4
	// CastleWar
	// 이동게이트 이동..ㅋㅋㅋ
	// 로스트렐룸 성
	if( pCMyApp->m_pUIMgr && pCMyApp->m_pUIMgr->m_bOnBattle ||
		(DecardiBattleManager::GetInstance()->GetGate() == false) )
	{
		if( ( m_nLoloId == 68 && ( int )m_wx >= 6400 && ( int )m_wx <= 6800 )
				|| ( m_nLoloId == 93 && ( int )m_wx >= 6400 && ( int )m_wx <= 6800 ) )
		{
			DeleteEffectNum();
			return S_OK;
		}
//=============================================
		// nate 2004 - 5
		// 술집 문 닫기 추가
		if( m_nLoloId == 278 )
			return S_OK;
	}
	else if( m_nLoloId == 279 )
		return S_OK;
//=============================================

	// 간이마을
	if( pCMyApp->m_pUIMgr && pCMyApp->m_pUIMgr->m_byEndDecardiCastleWar == 0 )
	{
		if( ( m_nLoloId == 68 && ( int )m_wx >= 8900 && ( int )m_wx <= 9100 )
				|| ( m_nLoloId == 93 && ( int )m_wx >= 8900 && ( int )m_wx <= 9100 ) )
		{
			DeleteEffectNum();
			return S_OK;
		}
	}
//=============================================
//=============================================
	// nate 2004 - 10
	// GuildHunt
	// 길드 사냥터 문 제어
	if( pCMyApp->IsGuildHuntArea( m_wx, m_wz ) )
	{
		// 문이 닫혔을경우
		if( pCMyApp->m_pUIMgr->m_byDoorOpen == 0 )
		{
			// 열린문은 그리지 말라
			if( ( pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN && m_nLoloId == 184 )
					|| ( pCMyApp->m_nCurWorldIdx == WORLD_GUILD && m_nLoloId == 0 ) )
				return S_OK;
		}
		else     // 문이 열렸을 경우
		{
			// 닫힌문은 그리지 말라
			if( ( pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN && m_nLoloId == 183 )
					|| ( pCMyApp->m_nCurWorldIdx == WORLD_GUILD && m_nLoloId == 101 ) )
				return S_OK;
		}
	}
//=============================================
	m_fTimeElapsed += g_fTimeKey;

	while (m_fTimeElapsed >= m_fAniRate)
	{
		m_fTimeElapsed -= m_fAniRate;
		m_nCurFrame++;
		m_nFlyFrame++;
	}

	m_nFlyFrame %= DECO_FLY_FRAME;

	static DWORD alphaTest, alphaBlend;

	float wy = m_wy;
	if (g_pDecoObjSet->m_pnObjType[m_nLoloId] == BLD_FLY)
	{
		if (pCMyApp->GetCurWorld() == WORLD_SKY)
			wy += g_DecoFlyHeight[m_nFlyFrame];
		else
			wy += g_DecoFlyHeight[m_nFlyFrame] / 4.0f;
	}

	D3DUtil_SetTranslateMatrix( m_matTrans, m_wx, wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix( m_matRotate, m_dir);
	D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matTrans );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );

	// View Matrix 도 가져와보자.
	D3DMATRIX matView;
	pCMyApp->GetDevice()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// 알파 테스팅

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000030);
	//GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x0000001);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);

	if (g_pDecoObjSet->m_pnObjType[m_nLoloId] != BLD_STAR
			&& ( whichAlpha == DECO_ALPHA_SEE || whichAlpha == DECO_ALPHA_FAR )
			&& !( m_nLoloId >= 378 && m_nLoloId <= 381 ))
	{
		DWORD alpha;
		if (whichAlpha == DECO_ALPHA_FAR)
			alpha = (DWORD)(0xff * m_farFactor) << 24;
		else
			alpha = 0x55000000;

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);

		DWORD alphaArg1, alphaArg2, alphaRef, alphaOp;
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG1, &alphaArg1);
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG2, &alphaArg2);
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,  &alphaOp);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000001);

		m_pLolo->Render(GET_D3DDEVICE(), m_nCurFrame, FALSE, FALSE, FALSE, &m_matWorld, &matView, FALSE, FALSE, alpha);

		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, alphaArg1);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, alphaArg2);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAOP, alphaOp);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF  , alphaRef);
	}
	else
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);

		if (g_pDecoObjSet->m_pnObjType[m_nLoloId] == BLD_EVENT_1 ||
				g_pDecoObjSet->m_pnObjType[m_nLoloId] == BLD_EVENT_2)
		{

			m_pLolo->Render(GET_D3DDEVICE(), m_nCurFrame, FALSE, bNoRoof, FALSE, &m_matWorld, &matView);

			if( (pCMyApp->m_pMouse->GetMouseType() == M_DOOR || (m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() && IsBrightObj(pCMyApp->m_nCurWorldIdx,m_nLoloId)) )
					|| (pCMyApp->m_pMouse->GetMouseType() == M_HOLD || (m_wColorIdx == pCMyApp->m_pUIMgr->GetPickColor() && IsHeroStone(pCMyApp->m_nCurWorldIdx,m_nLoloId)) )
			  )
			{
				DWORD dwAlpha, dwSrcRenState, dwDestRenState;//, dwLighting, dwZEnable, dwZWrite;
				DWORD dwAmbient;

				GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00646464);
				GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlpha );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
				GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcRenState );
				GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestRenState );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

				m_pLolo->Render(GET_D3DDEVICE(), m_nCurFrame, FALSE, bNoRoof, FALSE, &m_matWorld, &matView);

				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcRenState );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestRenState );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, dwAlpha );
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbient);
			}
		}
		else
			m_pLolo->Render(GET_D3DDEVICE(), m_nCurFrame, FALSE, bNoRoof, FALSE, &m_matWorld, &matView);
	}


	if ( m_pLolo->m_LineVector0 && ( (m_pLolo->m_LineType == LINE_TYPE_FIRE) || (m_pLolo->m_LineType == LINE_TYPE_BLUEFIRE) ))
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_FIREPARTICLE;
			ef_sort.effects = Fire_Particle;
			if( m_pLolo->m_LineType == LINE_TYPE_FIRE )
				ef_sort.num = REDFIRE;
			else if( m_pLolo->m_LineType == LINE_TYPE_BLUEFIRE )
				ef_sort.num = BLUEFIRE;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			int index = 0;
			index = (int) fabs( (m_pLolo->m_LineVector0[1].y - m_pLolo->m_LineVector0[0].y) );

			if( pCMyApp->GetCurWorldID() >= 13 )
			{
				if( index >= 1 && index <= 8 )		// 불 기존 크기 효과
					ef_sort.Index = 1;
				else if( index >= 9 && index <= 12 )		// 2배효과
					ef_sort.Index = 10;
				else if( index >= 13 && index <= 17)		// 3배효과
					ef_sort.Index = 15;
				else if( index >= 18 && index <= 22 )	// 4배효과
					ef_sort.Index = 20;
				else if( index >= 23 && index <= 27 )	// 6배효과
					ef_sort.Index = 25;
				else if( index >= 28 && index <= 32 )	// 8배효과
					ef_sort.Index = 30;
				else if( index >= 33 && index <= 37 )	// 10배효과
					ef_sort.Index = 35;
			}
			else
			{
				if( index >= 9 && index <= 11 )
					ef_sort.Index = 10;
				else if( index >= 19 && index <= 21 )
					ef_sort.Index = 20;
				else
					ef_sort.Index = index;
			}

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_SMOKE && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_SMOKEPARTICLE;		// FX_SMOKE_PARTICLE
			ef_sort.effects = Smoke_Particle;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_VOLCANO && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_SPRAYPARTICLE;
			ef_sort.effects = Spray_Particle;
			ef_sort.num = m_pLolo->m_LineTextureNum;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_POISON && m_pLolo->m_LineVector0 )
	{

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_TURNPARTICLE;
			ef_sort.effects = Turn_Particle;
			ef_sort.num = m_pLolo->m_LineTextureNum;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_GAS && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_LIGHTPARTICLE;
			ef_sort.effects = Light_Particle;
			ef_sort.num = m_pLolo->m_LineTextureNum;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_ELECTRIC && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_ELECTRICPARTICLE;
			ef_sort.effects = Electric_Particle;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
				swap(ef_sort.vTargetPos, ef_sort.vPos );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_RED && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.nTargetType = CLYSTAL_TYPE_RED;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
				swap(ef_sort.vTargetPos, ef_sort.vPos );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINEB && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.nTargetType = CLYSTAL_TYPE_WINEB;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
				swap(ef_sort.vTargetPos, ef_sort.vPos );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINES && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.nTargetType = CLYSTAL_TYPE_WINES;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
				swap(ef_sort.vTargetPos, ef_sort.vPos );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_CYON && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.nTargetType = CLYSTAL_TYPE_CYON;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
				swap(ef_sort.vTargetPos, ef_sort.vPos );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_WATERFALLD && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_WATERFALL;
			//ef_sort.effects = Waterfall;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			//if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
			//	swap(ef_sort.vTargetPos, ef_sort.vPos );

			//vDrawVec = ef_sort.vTargetPos - ef_sort.vPos;
			//vDrawVec = Normalize(vDrawVec);
			ef_sort.vSortPos.x = m_dir; //(float)atan2(vDrawVec.z, vDrawVec.x);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_WATERFALL && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_WATERFALLPARTICLE;
			ef_sort.effects = Waterfall_Particle;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
				swap(ef_sort.vTargetPos, ef_sort.vPos );

			vDrawVec = ef_sort.vTargetPos - ef_sort.vPos;
			ef_sort.vSortPos  = Normalize(vDrawVec);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_PORTAL && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_PORTAL_BOSSRAID;
			ef_sort.effects = PortalBossraid;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			//if( ef_sort.vTargetPos.y < ef_sort.vPos.y )//
			//	swap(ef_sort.vTargetPos, ef_sort.vPos );

			ef_sort.num = DistPlToPlAbs(ef_sort.vTargetPos.x, ef_sort.vTargetPos.z, ef_sort.vPos.x, ef_sort.vPos.z);

			vDrawVec = ef_sort.vTargetPos - ef_sort.vPos;
			ef_sort.vSortPos  = Normalize(vDrawVec);

			vDrawVec = ef_sort.vSortPos * ((ef_sort.num/2.0f) - 4.0f);

			ef_sort.vCurPos = ef_sort.vPos;
			ef_sort.vCurPos.x += vDrawVec.x;
			ef_sort.vCurPos.z += vDrawVec.z;

			ef_sort.vSortPos.x = 10;
			ef_sort.vSortPos.y = 15;
			ef_sort.vSortPos.z = 17;

			//포탈에 빨려 들어가는 먼지 같은 느낌...파티클들을 셋팅하자
			for( int i=0; i<MAX_PORTAL_BOSSRAID_PARTICLE; i++ )
			{
				// 시작위치
				PortalBossraid[i].cur_Pos = ef_sort.vCurPos;
				vDrawVec.x = (rand()%ef_sort.num) * ((rand()%2 == 0)?-1:1);
				vDrawVec.y = (rand()%ef_sort.num) * ((rand()%2 == 0)?-1:1);
				vDrawVec.z = (rand()%ef_sort.num) * ((rand()%2 == 0)?-1:1);
				PortalBossraid[i].cur_Pos += vDrawVec; // 시작 위치

				// 진행 방향 찾기
				vDrawVec = ef_sort.vCurPos - PortalBossraid[i].cur_Pos;
				PortalBossraid[i].direction  = Normalize(vDrawVec);

				PortalBossraid[i].blue	= ((float)(rand()%10)+1) / 10.0f + ef_sort.num * 0.004f;	// 진행 속도
				PortalBossraid[i].scale = ef_sort.num * 0.025f;	// 크기
				PortalBossraid[i].fade	= 1.0f;	// 페이드
			}

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_FIRESMOKE && m_pLolo->m_LineVector0 )
	{
		// 2. InsertFxEffect 한다. 오브젝트가 보이기 시작하는 시점..

		// 확실치 않아서 주석 잡아놨다.

		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_FIRESMOKE_PARTICLE;
			ef_sort.effects = Firesmoke_Particle;

			D3DMATRIX	matTrans, matRotate, matWorld;
			D3DVECTOR	vDrawVec;

			vDrawVec.x = m_pLolo->m_LineVector0[0].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[0].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[0].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vPos, vDrawVec, matWorld);

			vDrawVec.x = m_pLolo->m_LineVector0[1].x; // * (float)sin(m_dir);
			vDrawVec.z = m_pLolo->m_LineVector0[1].z; // * (float)cos(m_dir);
			vDrawVec.y = m_pLolo->m_LineVector0[1].y;

			D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			D3DMath_VectorMatrixMultiply(ef_sort.vTargetPos, vDrawVec, matWorld);

			if( ef_sort.vTargetPos.y < ef_sort.vPos.y )
				swap(ef_sort.vTargetPos, ef_sort.vPos );

			vDrawVec = ef_sort.vTargetPos - ef_sort.vPos;
			ef_sort.vSortPos  = Normalize(vDrawVec);

			//- 파티클에 필요한 데이터를 지금 샛팅해주자~
			for( int i=0; i<MAX_FIRESMOKE_PARTICLE; i++)
			{
				Firesmoke_Particle[i].cur_Pos		= ef_sort.vPos;	// 시작 위치 현재 위치
				Firesmoke_Particle[i].start_time	= rand()%10000; // 몇초뒤에 이펙트 를 긜지 결정 하자
				Firesmoke_Particle[i].life_time		= rand()%8000 + 10000; // 이펙트를 그리는 시간
				Firesmoke_Particle[i].scale			= (float(rand()%10)+5) / 5.0f;	// 시작 크기
				Firesmoke_Particle[i].direction.x	= float(rand()%50)+0.5f;  // 회전 시작 각도
				Firesmoke_Particle[i].first_Pos.x	= (float(rand()%5)+2) / 10.0f; // 스케일 속도
				Firesmoke_Particle[i].first_Pos.y	= (float(rand()%4)+4) / 10.0f; // 진행 속도
				Firesmoke_Particle[i].first_Pos.z	= (float(rand()%30)+2) / 1000.0f; // 회전 속도
				Firesmoke_Particle[i].cur_time		= timeGetTime();
				Firesmoke_Particle[i].texIndex		= 1;	// 상태값 [0:초기화, 1:계산]
			}

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}

	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, alphaTest);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend);

	// 픽킹 버퍼 쓰기.
	if( GET_D3DPICK() )
	{
		if ( g_pDecoObjSet->m_pnObjType[m_nLoloId] == BLD_EVENT_1
				|| g_pDecoObjSet->m_pnObjType[m_nLoloId] == BLD_EVENT_2)
		{
			GET_D3DPICK()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
			GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick);
			m_pLolo->Render(GET_D3DPICK(), m_nCurFrame, TRUE, bNoRoof);
			GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		}
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CDecoObjSet Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDecoObjSet::CDecoObjSet(IndexedTexture *pTexture)
	: m_nLoloCount(0)
{
	assert(pTexture != NULL);
	m_pIndexedTexture = pTexture;
	m_pLoloSet = NULL;
	m_pnObjIdx = NULL;
	m_pnObjType = NULL;
	m_pLolosDir = NULL;
	m_FileName = NULL;
	m_NickName = NULL;
}

CDecoObjSet::~CDecoObjSet()
{
	SAFE_DELETE_ARRAY(m_pLoloSet);

	if (m_FileName != NULL)
	{
		for (int i=0; i < m_nLoloCount; i++)
			if (m_FileName[i] != NULL)
				SAFE_DELETE_ARRAY( m_FileName[i] );
		SAFE_DELETE_ARRAY( m_FileName );
	}

	if (m_NickName != NULL)
	{
		for (int i=0; i < m_nLoloCount; i++)
			SAFE_DELETE_ARRAY(m_NickName[i]);
		SAFE_DELETE_ARRAY( m_NickName );
	}

	SAFE_DELETE_ARRAY(m_pnObjIdx);
	SAFE_DELETE_ARRAY(m_pnObjType);
	SAFE_DELETE_ARRAY(m_pLolosDir);
	m_pIndexedTexture = NULL;
}

void CDecoObjSet::SetLolosDir(char *dir_name)
{
	SAFE_DELETE_ARRAY(m_pLolosDir);
	m_pLolosDir = new char[strlen(dir_name)+1];
	strcpy(m_pLolosDir, dir_name);
}

void CDecoObjSet::ReadDataFile( char *file_name, BOOL bLoadWalkPlane )
{
	PFILE *fp = NULL;
	PFILE *fpLolo = NULL;
	char path_name[256] = "";
	int i = 0, len = 0;

	fp = pfopen(file_name, "rb");
	assert(fp != NULL);

	pfread(&m_nLoloCount, sizeof(int), 1, fp);
	assert(m_nLoloCount > 0);

	// 레코드 수만큼 메모리를 할당받는다.
	m_pLoloSet = new CLolos[m_nLoloCount];		// 로로
	m_pnObjIdx = new int[m_nLoloCount];
	m_pnObjType = new int[m_nLoloCount];
	m_FileName = new char *[m_nLoloCount];
	m_NickName = new char *[m_nLoloCount];





	for (i=0; i < m_nLoloCount; i++)
	{


		m_pLoloSet[i].SetIndexedTexture(m_pIndexedTexture);
		pfread(&m_pnObjIdx[i], sizeof(int), 1, fp);
		pfread(&m_pnObjType[i], sizeof(int), 1, fp);
		pfread(&len, sizeof(int), 1, fp);

		m_NickName[i] = new char[len+1];
		pfread(m_NickName[i], len, 1, fp);
		m_NickName[i][len] = NULL;

		pfread(&len, sizeof(int), 1, fp);
		m_FileName[i] = new char[len+1];
		pfread(m_FileName[i], len, 1, fp);
		m_FileName[i][len] = NULL;
		// 실제 로로를 읽는 부분.
		strcpy(path_name, m_pLolosDir);
		strcat(path_name, m_FileName[i]);

		PFILE* fptemp = new PFILE_NFS();
		bool decode = true;
	
		if(!fptemp->Open(path_name, "rb"))
		{
			decode = false;
		}
		else
			fptemp->Close();

		fpLolo = pfopen(path_name, "rb");
		if (fpLolo == NULL)
		{
			char temp[256] = "";
			sprintf(temp, "Missing %s..", path_name);
			MessageBox(NULL, temp, temp, MB_OK);
		}
		assert(fpLolo != NULL);
		m_pLoloSet[i].Read( fpLolo, FALSE, bLoadWalkPlane, decode );
		pfclose(fpLolo);
	}
	pfclose(fp);
}

///////////////////////////////////////////////////////////////////////////
// Name : GetSentence()
// Desc : DecoObjSet.set 화일을 읽기 위한 것.
//////////////////////////////////////////////////////////////////////////
char * CDecoObjSet::GetSentence(char *dest, int n, FILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = fgets(dest, n, fp)) == NULL)
			return tmp;

		while (iswspace(*tmp) && *tmp != 0)
			tmp++;
	}
	while (*tmp == '#' || *tmp == 0);

	return tmp;
}

char * CDecoObjSet::PGetSentence(char *dest, int n, PFILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = pfgets(dest, n, fp)) == NULL)
			return tmp;

		while (iswspace(*tmp) && *tmp != 0)
			tmp++;
	}
	while (*tmp == '#' || *tmp == 0);

	return tmp;
}

//메트릭스 관련 롤러를 그리자
HRESULT	CDecoObj::RenderMatrixLolo()
{
//=============================================
	// nate 2005-03-22 : Matrix - G.P.Stone 마크 랜더링 선택
	if( m_nLoloId >= 112 && m_nLoloId <= 120 )
	{
		for( int i = 9 ; i < 12 ; i ++ )
		{
			switch( pCMyApp->m_pUIMgr->m_nStoneState[ i ] )
			{
			case 0:
				if( m_nLoloId == i + 103 || m_nLoloId == i + 106 || m_nLoloId == i + 109 )
					return S_OK;
				break;
			case 1:
				if( m_nLoloId == i + 103 || m_nLoloId == i + 106 )
					return S_OK;
				break;
			case 2:
				if( m_nLoloId == i + 103 || m_nLoloId == i + 109 )
					return S_OK;
				break;
			case 3:
				if( m_nLoloId == i + 106 || m_nLoloId == i + 109 )
					return S_OK;
				break;
			}
		}
	}
	// 스톤 상태에 따라 몹랜더링과 오브젝트 렌더링 구별
	else if( m_nLoloId >= 160 && m_nLoloId <= 162 )
	{
		if( !pCMyApp->m_pUIMgr->m_nStoneState[ m_nLoloId - 151 ] )
			return S_OK;
	}
	else if( m_nLoloId >= 102 && m_nLoloId <= 111 || m_nLoloId == 76 || m_nLoloId == 77 )
	{
//=============================================
		//메트릭스 게이트 스톤에 따른 성문 열고 닫힘을 보여주자.(mungmae-3/21)
		for(int count = 0; count < 6; count++)
		{
			if(pCMyApp->m_pUIMgr->m_nStoneState[count] == 0 )
			{
				switch(count)
				{
				case 0:
					if(m_nLoloId == 105)
						return S_OK;
					break;

				case 1:
					if(m_nLoloId == 107)
						return S_OK;
					break;

				case 2:
					if(m_nLoloId == 109)
						return S_OK;
					break;

				case 3:
					if(m_nLoloId == 111)
						return S_OK;
					break;

				case 4:
					if(m_nLoloId == 77)
						return S_OK;
					break;

				case 5:
					if(m_nLoloId == 103)
						return S_OK;
					break;
				}
			}
			else if( pCMyApp->m_pUIMgr->m_nStoneState[count] == 1 )
			{
				switch(count)
				{
				case 0:
					if(m_nLoloId == 104)
						return S_OK;
					break;

				case 1:
					if(m_nLoloId == 106)
						return S_OK;
					break;

				case 2:
					if(m_nLoloId == 108)
						return S_OK;
					break;

				case 3:
					if(m_nLoloId == 110)
						return S_OK;
					break;

				case 4:
					if(m_nLoloId == 76)
						return S_OK;
					break;

				case 5:
					if(m_nLoloId == 102)
						return S_OK;
					break;

				}
			}
		}
	}
	return E_FAIL;
}

HRESULT	CDecoObj::RenderMatrix2009Lolo() // [2008/11/3 Theodoric] WORLD_MATRIX_2009
{
	// 요기선 그리지 않을 거면 S)OK를 넘겨줘야한다. ㅡㅡ;;
	int i = 0;

	if( m_nLoloId >= 6 && m_nLoloId <= 25 )		// GP스톤
	{
		for(i=4; i<6; i++) // 크리스탈 밑의 마크
		{
			if(pCMyApp->m_pUIMgr->m_nStoneState[i] == 0  ) // 크리스탈이 없다면 깨진 것이므로 상대 진영 마크를 그려주자.
			{
				switch(i)
				{
				case 4: // 툰드마르 크리스탈이 없다면 안그마르 가드게이트를 그려주자. (기존 쉬버 마크)
					if( m_nLoloId == 6)
						return S_OK;
					break;
				case 5:
					if( m_nLoloId == 15)
						return S_OK;
					break;
				}
			}
			else // 체력이 남아 있다면 아무것도 그리지 말자
			{
				switch(i)
				{
				case 4:
					if( m_nLoloId == 6 || m_nLoloId == 7 )
						return S_OK;
					break;
				case 5:
					if( m_nLoloId == 14 || m_nLoloId == 15 )
						return S_OK;
					break;
				}

			}
		}

		for(i=6; i<11; i++)
		{

			switch(i)
			{
			case 6: // 안들랑그 북부 G 스톤
				switch(pCMyApp->m_pUIMgr->m_nStoneState[i] )
				{
				case 0:
					if( m_nLoloId == 9 || m_nLoloId == 8 )	return S_OK;
					break;
				case 1:
					if( m_nLoloId == 8 )	return S_OK;
					break;
				case 2:
					if( m_nLoloId == 9 )	return S_OK;
					break;
				}
				break;

			case 7: // 툰드마르 G 스톤
				switch(pCMyApp->m_pUIMgr->m_nStoneState[i] )
				{
				case 0:
					if( m_nLoloId == 20 || m_nLoloId == 21 )	return S_OK;
					break;
				case 1:
					if( m_nLoloId == 20 )	return S_OK;
					break;
				case 2:
					if( m_nLoloId == 21 )	return S_OK;
					break;
				}
				break;

			case 8: // 미미르 G 스톤
				switch(pCMyApp->m_pUIMgr->m_nStoneState[i] )
				{
				case 0:
					if( m_nLoloId == 22 || m_nLoloId == 23 )	return S_OK;
					break;
				case 1:
					if( m_nLoloId == 22 )	return S_OK;
					break;
				case 2:
					if( m_nLoloId == 23 )	return S_OK;
					break;
				}
				break;

			case 9: // 안그마르 북부 G 스톤
				switch(pCMyApp->m_pUIMgr->m_nStoneState[i] )
				{
				case 0:
					if( m_nLoloId == 12 || m_nLoloId == 13 )	return S_OK;
					break;
				case 1:
					if( m_nLoloId == 12 )	return S_OK;
					break;
				case 2:
					if( m_nLoloId == 13 )	return S_OK;
					break;
				}
				break;

			case 10: // 안들랑그 남 G 스톤
				switch(pCMyApp->m_pUIMgr->m_nStoneState[i] )
				{
				case 0:
					if( m_nLoloId == 16 || m_nLoloId == 17 )	return S_OK;
					break;
				case 1:
					if( m_nLoloId == 16 )	return S_OK;
					break;
				case 2:
					if( m_nLoloId == 17 )	return S_OK;
					break;
				}
				break;
			}
		}

	}
	if( m_nLoloId >= 26 && m_nLoloId <= 33 )	 //가드 게이트 열림/닫힘 상태
	{
		for( i = 0; i < 4; i++)
		{
			if( pCMyApp->m_pUIMgr->m_nStoneState[i] == 0 ) // 열림 상태
			{
				switch(i)
				{
				case 0:
					if( m_nLoloId == 26 )
						return S_OK;
					break;
				case 1:
					if( m_nLoloId == 28 )
						return S_OK;
					break;
				case 2:
					if( m_nLoloId == 30 )
						return S_OK;
					break;
				case 3:
					if( m_nLoloId == 32 )
						return S_OK;
					break;
				}
			}
			else if( pCMyApp->m_pUIMgr->m_nStoneState[i] == 1 ) // 닫힘 상태
			{
				switch(i)
				{
				case 0:
					if( m_nLoloId == 27 )
						return S_OK;
					break;
				case 1:
					if( m_nLoloId == 29 )
						return S_OK;
					break;
				case 2:
					if( m_nLoloId == 31 )
						return S_OK;
					break;
				case 3:
					if( m_nLoloId == 33 )
						return S_OK;
					break;
				}
			}
		}
	}

	return E_FAIL;

}

/*
//=============================================
	// nate 2005-03-22 : Matrix - G.P.Stone 마크 랜더링 선택
	if( m_nLoloId >= 112 && m_nLoloId <= 120 ) {
		for( int i = 9 ; i < 12 ; i ++ ) {
			switch( pCMyApp->m_pUIMgr->m_nStoneState[ i ] )
			{
			case 0:
				if( m_nLoloId == i + 103 || m_nLoloId == i + 106 || m_nLoloId == i + 109 )
					return S_OK;
				break;
			case 1:
				if( m_nLoloId == i + 103 || m_nLoloId == i + 106 )
					return S_OK;
				break;
			case 2:
				if( m_nLoloId == i + 103 || m_nLoloId == i + 109 )
					return S_OK;
				break;
			case 3:
				if( m_nLoloId == i + 106 || m_nLoloId == i + 109 )
					return S_OK;
				break;
			}
		}
	}
	// 스톤 상태에 따라 몹랜더링과 오브젝트 렌더링 구별
	else if( m_nLoloId >= 160 && m_nLoloId <= 162 ) {
		if( !pCMyApp->m_pUIMgr->m_nStoneState[ m_nLoloId - 151 ] )
			return S_OK;
	}
	else if( m_nLoloId >= 102 && m_nLoloId <= 111 || m_nLoloId == 76 || m_nLoloId == 77 )
	{
//=============================================
		//메트릭스 게이트 스톤에 따른 성문 열고 닫힘을 보여주자.(mungmae-3/21)
		for(int count = 0; count < 6; count++)
		{
			if(pCMyApp->m_pUIMgr->m_nStoneState[count] == 0 )
			{
				switch(count)
				{
				case 0:
					if(m_nLoloId == 105)
						return S_OK;
					break;

				case 1:
					if(m_nLoloId == 107)
						return S_OK;
					break;

				case 2:
					if(m_nLoloId == 109)
						return S_OK;
					break;

				case 3:
					if(m_nLoloId == 111)
						return S_OK;
					break;

				case 4:
					if(m_nLoloId == 77)
						return S_OK;
					break;

				case 5:
					if(m_nLoloId == 103)
						return S_OK;
					break;
				}
			}
			else if( pCMyApp->m_pUIMgr->m_nStoneState[count] == 1 )
			{
				switch(count) {
				case 0:
					if(m_nLoloId == 104)
						return S_OK;
					break;

				case 1:
					if(m_nLoloId == 106)
						return S_OK;
					break;

				case 2:
					if(m_nLoloId == 108)
						return S_OK;
					break;

				case 3:
					if(m_nLoloId == 110)
						return S_OK;
					break;

				case 4:
					if(m_nLoloId == 76)
						return S_OK;
					break;

				case 5:
					if(m_nLoloId == 102)
						return S_OK;
					break;

				}
			}
		}
	}


	return E_FAIL;
}
 */