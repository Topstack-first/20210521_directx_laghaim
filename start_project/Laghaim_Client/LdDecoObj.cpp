#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <assert.h>
#include "main.h"
#include "ClrAlloc.h"
#include "DecoObj.h"
#include "D3DMath.h"
#include "Land.h"
#include "IndexedTexture.h"
#include "FxSet.h"
#include <list>
#include <iterator>
using namespace std;

#include "LdDecoObj.h"

////////////////////////////////////////////////////////////////////////////////
// 전역 함수와 전역 변수 : 포팅할때 증요함.
CLdDecoObjSet	*g_pLdDecoObjSet;
CLdDecoObjSet	*g_pLdDecoObjSet2; // 이 녀석 login zone 과 select zone 이 따로 있어야한다. ///2007로딩변경
CLdDecoObj	*g_pGuildGateClosed, *g_pGuildGate2Opened;				// 성문 - 특별히 따로 관리.
extern IndexedTexture *g_pLdDecoTexture;
extern float g_fTimeKey;
////////////////////////////////////////////////////////////////////////////////
// 맵을 셀로 나눈 것.
extern int g_nLdMapCellWidth;
extern int g_nLdMapCellHeight;
extern int g_nLdMapCellWidthCount;
extern int g_nLdMapCellHeightCount;


list<CLdDecoObj *> g_listLdDecoObj[MAP_CELL_HEIGHT_COUNT][MAP_CELL_WIDTH_COUNT];
list<CLdDecoObj *> g_listLdDecoObj2[MAP_CELL_HEIGHT_COUNT][MAP_CELL_WIDTH_COUNT];

BOOL InsertLdDecoObjToMaplist(CLdDecoObj *pLdDecoObj) ;
BOOL InsertLdDecoObjToMaplist2(CLdDecoObj *pLdDecoObj) ;
void DeleteLdDecoObjSet();
void DeleteLdDecoObjData(int nStage);

void LdDecoObjSet(char *strFilePath)
{
	FILE *fp = NULL;
	//int listNum;
	CLdDecoObj *pLdDecoObj = NULL;
	char *strFilePathName = NULL;

	// 기존에 로딩되어 있는 것들을 모두 지운다.
	DeleteLdDecoObjSet();

	strFilePathName = new char[255];

	// 현재 건물들의 텍스처는 모두 한 곳에.
	if( g_pLdDecoTexture )
		g_pLdDecoTexture->SetTexturePath("data//back//textr//");

	SAFE_DELETE(g_pLdDecoObjSet);
	g_pLdDecoObjSet = new CLdDecoObjSet(g_pLdDecoTexture);
	SAFE_DELETE(g_pLdDecoObjSet2);
	g_pLdDecoObjSet2 = new CLdDecoObjSet(g_pLdDecoTexture); ///2007로딩변경


	// 현재 모든 롤로는 한곳에
	strcpy(strFilePathName, "data//back//lolo/");
	if( g_pLdDecoObjSet )
		g_pLdDecoObjSet->SetLolosDir(strFilePathName);
	if( g_pLdDecoObjSet2 ) ///2007로딩변경
		g_pLdDecoObjSet2->SetLolosDir(strFilePathName);
	SAFE_DELETE_ARRAY( strFilePathName );
}

void LdDecoObjInitialLoading(char *strFilePath, int nStage)
{
	PFILE *fp = NULL;
	int listNum;
	CLdDecoObj *pLdDecoObj = NULL;
	char *strFilePathName = NULL;
	CLdDecoObjSet *pLdDecoObjSet = NULL; ///2007로딩변경

	// 기존에 로딩되어 있는 것들을 모두 지운다.
	DeleteLdDecoObjData(nStage);

	strFilePathName = new char[255];

	// 현재 건물들의 텍스처는 모두 한 곳에.
	if( g_pLdDecoTexture )
		g_pLdDecoTexture->SetTexturePath("data//back//textr//");

	// bld 화일 설정. // bld 를 여기서 읽도록 변경 ///2007로딩변경
	strcpy(strFilePathName, strFilePath);
	strcat(strFilePathName, "bld.bld");

	if( nStage == 0 )
	{
		if( g_pLdDecoObjSet ) // 로그인존과 다른 오브젝트 셋트를 사용.
		{
			g_pLdDecoObjSet->ReadDataFile(strFilePathName);
			pLdDecoObjSet = g_pLdDecoObjSet; // 참조 포인터를 연결
		}
	}
	else if( nStage == 1 )// 셀렉트 존 이면.
	{
		if( g_pLdDecoObjSet2 ) // 로그인존과 다른 오브젝트 셋트를 사용.
		{
			g_pLdDecoObjSet2->ReadDataFile(strFilePathName);
			pLdDecoObjSet = g_pLdDecoObjSet2; // 참조 포인터를 연결
		}
	}

	if( !pLdDecoObjSet ) ///2007로딩변경
	{
		SAFE_DELETE_ARRAY( strFilePathName );
		return;
	}

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

	//MessageBox(NULL, "read list", "read list", MB_OK);

	for (int i=0; i < listNum; i++)
	{
		pLdDecoObj = new CLdDecoObj();
		if( !pLdDecoObj )
			continue;

		// id, x, z, y, dir
		pfscanf(fp, "%d %f %f %f %f", &(pLdDecoObj->m_nLoloId), &(pLdDecoObj->m_wx), &(pLdDecoObj->m_wz), \
				&(pLdDecoObj->m_wy), &(pLdDecoObj->m_dir));
		pLdDecoObj->m_pLolo = &(pLdDecoObjSet->m_pLoloSet[pLdDecoObj->m_nLoloId]);

		if( ( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_FIRE || pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_BLUEFIRE ) &&
				pLdDecoObj->m_pLolo->m_LineVector0 )
		{
			pLdDecoObj->Fire_Particle = new Deco_Particle[MAX_FIRE_PARTICLE];

			for( int i = 0; i < MAX_FIRE_PARTICLE; i++ )
			{
				pLdDecoObj->Fire_Particle[i].start_time = 0;
				pLdDecoObj->Fire_Particle[i].life_time = 0;
				pLdDecoObj->Fire_Particle[i].cur_time = 0;
				pLdDecoObj->Fire_Particle[i].scale = 1.0f;
				pLdDecoObj->Fire_Particle[i].fade = 1.0f;

				pLdDecoObj->Fire_Particle[i].red = 1.0f;
				pLdDecoObj->Fire_Particle[i].green = 1.0f;
				pLdDecoObj->Fire_Particle[i].blue = 1.0f;

				pLdDecoObj->Fire_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Fire_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Fire_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Fire_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->Fire_Particle[i].texIndex = 0;
				pLdDecoObj->Fire_Particle[i].texFrame = 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_SMOKE && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->Smoke_Particle = new Deco_Particle[MAX_SMOKE_PARTICLE];

			int smoke_particle_start_time[MAX_SMOKE_PARTICLE] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45 };

			for( int i = 0; i < MAX_SMOKE_PARTICLE; i++ )
			{
				pLdDecoObj->Smoke_Particle[i].start_time = smoke_particle_start_time[i];
				pLdDecoObj->Smoke_Particle[i].life_time = 0;
				pLdDecoObj->Smoke_Particle[i].cur_time = 0;
				pLdDecoObj->Smoke_Particle[i].scale = 1.0f;
				pLdDecoObj->Smoke_Particle[i].fade = 0.5f;

				pLdDecoObj->Smoke_Particle[i].red = 1.0f;
				pLdDecoObj->Smoke_Particle[i].green = 1.0f;
				pLdDecoObj->Smoke_Particle[i].blue = 1.0f;

				pLdDecoObj->Smoke_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Smoke_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Smoke_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Smoke_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->Smoke_Particle[i].texIndex = 0;
				pLdDecoObj->Smoke_Particle[i].texFrame = 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_ELECTRIC && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->Electric_Particle = new Deco_Particle[MAX_ELCTRIC_PARTICLE];

			for( int i = 0; i < MAX_ELCTRIC_PARTICLE; i++ )
			{
				pLdDecoObj->Electric_Particle[i].start_time		= 0;
				pLdDecoObj->Electric_Particle[i].life_time		= 0;
				pLdDecoObj->Electric_Particle[i].cur_time		= 0;
				pLdDecoObj->Electric_Particle[i].scale			= 0;
				pLdDecoObj->Electric_Particle[i].fade			= 0.5f;

				pLdDecoObj->Electric_Particle[i].red			= 1.0f;
				pLdDecoObj->Electric_Particle[i].green			= 1.0f;
				pLdDecoObj->Electric_Particle[i].blue			= 1.0f;

				pLdDecoObj->Electric_Particle[i].first_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Electric_Particle[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Electric_Particle[i].direction		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Electric_Particle[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->Electric_Particle[i].texIndex		= 0;
				pLdDecoObj->Electric_Particle[i].texFrame		= 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_RED && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

			for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
			{
				pLdDecoObj->ClystalGlow[i].start_time	= 0;
				pLdDecoObj->ClystalGlow[i].life_time	= 0;
				pLdDecoObj->ClystalGlow[i].cur_time		= 0;
				pLdDecoObj->ClystalGlow[i].scale		= 0;
				pLdDecoObj->ClystalGlow[i].fade			= 0.5f;

				pLdDecoObj->ClystalGlow[i].red			= 1.0f;
				pLdDecoObj->ClystalGlow[i].green		= 1.0f;
				pLdDecoObj->ClystalGlow[i].blue			= 1.0f;

				pLdDecoObj->ClystalGlow[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->ClystalGlow[i].texIndex		= 0;
				pLdDecoObj->ClystalGlow[i].texFrame		= 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINEB && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

			for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
			{
				pLdDecoObj->ClystalGlow[i].start_time	= 0;
				pLdDecoObj->ClystalGlow[i].life_time	= 0;
				pLdDecoObj->ClystalGlow[i].cur_time	= 0;
				pLdDecoObj->ClystalGlow[i].scale		= 0;
				pLdDecoObj->ClystalGlow[i].fade		= 0.5f;

				pLdDecoObj->ClystalGlow[i].red		= 1.0f;
				pLdDecoObj->ClystalGlow[i].green		= 1.0f;
				pLdDecoObj->ClystalGlow[i].blue		= 1.0f;

				pLdDecoObj->ClystalGlow[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].cur_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].gravity	= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->ClystalGlow[i].texIndex	= 0;
				pLdDecoObj->ClystalGlow[i].texFrame	= 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINES && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

			for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
			{
				pLdDecoObj->ClystalGlow[i].start_time	= 0;
				pLdDecoObj->ClystalGlow[i].life_time	= 0;
				pLdDecoObj->ClystalGlow[i].cur_time	= 0;
				pLdDecoObj->ClystalGlow[i].scale		= 0;
				pLdDecoObj->ClystalGlow[i].fade		= 0.5f;

				pLdDecoObj->ClystalGlow[i].red		= 1.0f;
				pLdDecoObj->ClystalGlow[i].green		= 1.0f;
				pLdDecoObj->ClystalGlow[i].blue		= 1.0f;

				pLdDecoObj->ClystalGlow[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].cur_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].gravity	= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->ClystalGlow[i].texIndex	= 0;
				pLdDecoObj->ClystalGlow[i].texFrame	= 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_CYON && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->ClystalGlow = new Deco_Particle[MAX_CLYSTALGLOW];

			for( int i = 0; i < MAX_CLYSTALGLOW; i++ )
			{
				pLdDecoObj->ClystalGlow[i].start_time	= 0;
				pLdDecoObj->ClystalGlow[i].life_time	= 0;
				pLdDecoObj->ClystalGlow[i].cur_time		= 0;
				pLdDecoObj->ClystalGlow[i].scale		= 0;
				pLdDecoObj->ClystalGlow[i].fade			= 0.5f;

				pLdDecoObj->ClystalGlow[i].red			= 1.0f;
				pLdDecoObj->ClystalGlow[i].green		= 1.0f;
				pLdDecoObj->ClystalGlow[i].blue			= 1.0f;

				pLdDecoObj->ClystalGlow[i].first_Pos	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].direction	= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->ClystalGlow[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->ClystalGlow[i].texIndex		= 0;
				pLdDecoObj->ClystalGlow[i].texFrame		= 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_WATERFALL && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->Waterfall_Particle = new Deco_Particle[MAX_WATERFALL_PARTICLE];

			for( int i = 0; i < MAX_WATERFALL_PARTICLE; i++ )
			{
				pLdDecoObj->Waterfall_Particle[i].start_time	= 0;
				pLdDecoObj->Waterfall_Particle[i].life_time		= 0;
				pLdDecoObj->Waterfall_Particle[i].cur_time		= 0;
				pLdDecoObj->Waterfall_Particle[i].scale			= 0;
				pLdDecoObj->Waterfall_Particle[i].fade			= 0.5f;

				pLdDecoObj->Waterfall_Particle[i].red			= 1.0f;
				pLdDecoObj->Waterfall_Particle[i].green			= 1.0f;
				pLdDecoObj->Waterfall_Particle[i].blue			= 1.0f;

				pLdDecoObj->Waterfall_Particle[i].first_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Waterfall_Particle[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Waterfall_Particle[i].direction		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Waterfall_Particle[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->Waterfall_Particle[i].texIndex		= 0;
				pLdDecoObj->Waterfall_Particle[i].texFrame		= 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_PORTAL && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->PortalBossraid = new Deco_Particle[MAX_PORTAL_BOSSRAID_PARTICLE];

			for( int i = 0; i < MAX_PORTAL_BOSSRAID_PARTICLE; i++ )
			{
				pLdDecoObj->PortalBossraid[i].start_time	= 0;
				pLdDecoObj->PortalBossraid[i].life_time		= 0;
				pLdDecoObj->PortalBossraid[i].cur_time		= 0;
				pLdDecoObj->PortalBossraid[i].scale			= 0;
				pLdDecoObj->PortalBossraid[i].fade			= 0.5f;

				pLdDecoObj->PortalBossraid[i].red			= 1.0f;
				pLdDecoObj->PortalBossraid[i].green			= 1.0f;
				pLdDecoObj->PortalBossraid[i].blue			= 1.0f;

				pLdDecoObj->PortalBossraid[i].first_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->PortalBossraid[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->PortalBossraid[i].direction		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->PortalBossraid[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->PortalBossraid[i].texIndex		= 0;
				pLdDecoObj->PortalBossraid[i].texFrame		= 0;
			}
		}
		else if( pLdDecoObj->m_pLolo->m_LineType == LINE_TYPE_FIRESMOKE && pLdDecoObj->m_pLolo->m_LineVector0)
		{
			pLdDecoObj->Firesmoke_Particle = new Deco_Particle[MAX_FIRESMOKE_PARTICLE];

			for( int i = 0; i < MAX_FIRESMOKE_PARTICLE; i++ )
			{
				pLdDecoObj->Firesmoke_Particle[i].start_time	= 0;
				pLdDecoObj->Firesmoke_Particle[i].life_time		= 0;
				pLdDecoObj->Firesmoke_Particle[i].cur_time		= 0;
				pLdDecoObj->Firesmoke_Particle[i].scale			= 0;
				pLdDecoObj->Firesmoke_Particle[i].fade			= 0.5f;

				pLdDecoObj->Firesmoke_Particle[i].red			= 1.0f;
				pLdDecoObj->Firesmoke_Particle[i].green			= 1.0f;
				pLdDecoObj->Firesmoke_Particle[i].blue			= 1.0f;

				pLdDecoObj->Firesmoke_Particle[i].first_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Firesmoke_Particle[i].cur_Pos		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Firesmoke_Particle[i].direction		= D3DVECTOR( 0.0f, 0.0f, 0.0f );
				pLdDecoObj->Firesmoke_Particle[i].gravity		= D3DVECTOR( 0.0f, 0.0f, 0.0f );

				pLdDecoObj->Firesmoke_Particle[i].texIndex		= 0;
				pLdDecoObj->Firesmoke_Particle[i].texFrame		= 0;
			}
		}

		if (nStage == 0)
			InsertLdDecoObjToMaplist(pLdDecoObj);
		else
			InsertLdDecoObjToMaplist2(pLdDecoObj);

		pLdDecoObj->CalBoundBox();
	}

	pfclose(fp);
	SAFE_DELETE_ARRAY( strFilePathName );
}

//////////////////////////////////////////////////////////////////////////////////////
// Name : InsertLdDecoObjToMaplist()
// Desc : Decoration Object들을 맵 리스트에 집어 넣는다.
//////////////////////////////////////////////////////////////////////////////////////
BOOL InsertLdDecoObjToMaplist(CLdDecoObj *pLdDecoObj)
{
	/*
		int w_pos, h_pos;

		w_pos = (int) (pLdDecoObj->m_wx);
		h_pos = (int) (pLdDecoObj->m_wz);

		w_pos /= g_nLdMapCellWidth;
		h_pos /= g_nLdMapCellHeight;

		if ((w_pos < g_nLdMapCellWidthCount) && (h_pos < g_nLdMapCellHeightCount)) {
			list<CLdDecoObj *>::iterator pr = g_listLdDecoObj[h_pos][w_pos].begin();
			g_listLdDecoObj[h_pos][w_pos].insert(pr, pLdDecoObj);
		}
	*/
	///2007로딩변경
	// 로그인 존 크기가 얼마나된다고 구역을 나눠서 컬링하나? 그냥 무식하게 다찍게 바꿈.
	g_listLdDecoObj[0][0].push_back(pLdDecoObj);

	return TRUE;
}

BOOL InsertLdDecoObjToMaplist2(CLdDecoObj *pLdDecoObj)
{
	int w_pos, h_pos;

	w_pos = (int) (pLdDecoObj->m_wx);
	h_pos = (int) (pLdDecoObj->m_wz);

	w_pos /= g_nLdMapCellWidth;
	h_pos /= g_nLdMapCellHeight;

	if ((w_pos < g_nLdMapCellWidthCount) && (h_pos < g_nLdMapCellHeightCount))
	{
		list<CLdDecoObj *>::iterator pr = g_listLdDecoObj2[h_pos][w_pos].begin();
		g_listLdDecoObj2[h_pos][w_pos].insert(pr, pLdDecoObj);
	}

	return TRUE;
}

void DeleteLdDecoObjData(int nStage)
{
	CLdDecoObj *pLdDecoObj;
	list<CLdDecoObj *>::iterator pr;

	if (nStage == 0)
	{
		for (int h = 0; h < g_nLdMapCellHeightCount; h++)
		{
			for (int w = 0; w < g_nLdMapCellWidthCount; w++)
			{
				for (pr = g_listLdDecoObj[h][w].begin(); pr != g_listLdDecoObj[h][w].end();)
				{
					pLdDecoObj = *pr;
					delete pLdDecoObj;
					pr = g_listLdDecoObj[h][w].erase(pr);
					if (pr == g_listLdDecoObj[h][w].end())
						break;
				}
			}
		}
	}
	else
	{
		for (int h = 0; h < g_nLdMapCellHeightCount; h++)
		{
			for (int w = 0; w < g_nLdMapCellWidthCount; w++)
			{
				for (pr = g_listLdDecoObj2[h][w].begin(); pr != g_listLdDecoObj2[h][w].end();)
				{
					pLdDecoObj = *pr;
					delete pLdDecoObj;
					pr = g_listLdDecoObj2[h][w].erase(pr);
					if (pr == g_listLdDecoObj2[h][w].end())
						break;
				}
			}
		}
	}
}

void DeleteLdDecoObjSet()
{
	SAFE_DELETE(g_pLdDecoObjSet2); ///2007로딩변경
	SAFE_DELETE(g_pLdDecoObjSet);
}

//////////////////////////////////////////////////////////////////////
// CLdDecoObj Construction/Destruction
//////////////////////////////////////////////////////////////////////
D3DMATRIX CLdDecoObj::m_matWorld;
D3DMATRIX CLdDecoObj::m_matTrans;
D3DMATRIX CLdDecoObj::m_matRotate;
float	  CLdDecoObj::m_fAniRate = 0.1f;

CLdDecoObj::CLdDecoObj()
	: m_nLoloId(0)
	, m_wx(0)
	, m_wy(0)
	, m_wz(0)
	, m_dir(0)
	, m_dwAdjustTime(0)
	, m_wColorIdx(0)
	, PortalBossraid(NULL)
	, m_farFactor(0)
{
	m_pLolo = NULL;
	m_nCurFrame = 0;
	m_fTimeElapsed = 0.0f;
	m_bOpenedRoof = FALSE;
	m_LineEffectsLoad = FALSE;
	Fire_Particle = NULL;
	Smoke_Particle = NULL;
	m_fAdjustY = 0.0f;
	m_fDownAdjustY = 0.0f;
	m_bDoAni = FALSE;
	m_bUpAni = FALSE;
	Electric_Particle = NULL;
	ClystalGlow = NULL;
	Waterfall = NULL;
	Waterfall_Particle = NULL;
	Firesmoke_Particle = NULL;
}

CLdDecoObj::~CLdDecoObj()
{
	if( m_LineEffectsLoad )
		DeleteEffectNum();

	SAFE_DELETE(Fire_Particle);
	SAFE_DELETE(Smoke_Particle);
	SAFE_DELETE(Electric_Particle);
	SAFE_DELETE(ClystalGlow);
	SAFE_DELETE(Waterfall);
	SAFE_DELETE(Waterfall_Particle);
	SAFE_DELETE(Firesmoke_Particle);
}

void CLdDecoObj::InsertEffectNum(int num)
{
	m_listEffect.insert(m_listEffect.begin(), num);
}

void CLdDecoObj::DeleteEffectNum()
{
	// 관련된 효과를 없앤다.
	list<int>::iterator it;
	it = m_listEffect.begin();
	int idx;

	while (it != m_listEffect.end())
	{
		idx = *it;
		if( pCMyApp->m_pFxSet )
			pCMyApp->m_pFxSet->DeleteEffect(idx);
		it = m_listEffect.erase(it);
		m_LineEffectsLoad = FALSE;
	}
}

void CLdDecoObj::CalBoundBox()
{
	D3DUtil_SetTranslateMatrix(m_matTrans, m_wx, m_wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix(m_matRotate, m_dir);
	D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matTrans );

	if( m_pLolo )
	{
		for (int i=0; i < 8; i++)
			D3DMath_VectorMatrixMultiply(m_vBoundBox[i], m_pLolo->m_BoundVect[i] , m_matWorld);
	}
}

// ----------------------------------------------------------------
// Name :
// Desc : 월드 좌표 x, z가 오브젝트 내에 포함되어 있는지 확인한다.
// ----------------------------------------------------------------
BOOL CLdDecoObj::IsInside(float wx, float wz)
{
	int bCCW[4];
	// (AB) 와 (AC) 벡터의 외적을 구하여 방향을 세 점의 위치 관계를 판다.
	bCCW[0] = Nk3DMath_CCW(m_vBoundBox[0].x, m_vBoundBox[0].z, m_vBoundBox[1].x, m_vBoundBox[1].z, wx, wz);
	bCCW[1] = Nk3DMath_CCW(m_vBoundBox[1].x, m_vBoundBox[1].z, m_vBoundBox[2].x, m_vBoundBox[2].z,  wx,  wz);
	bCCW[2] = Nk3DMath_CCW(m_vBoundBox[2].x, m_vBoundBox[2].z, m_vBoundBox[3].x, m_vBoundBox[3].z,  wx,  wz);
	bCCW[3] = Nk3DMath_CCW(m_vBoundBox[3].x, m_vBoundBox[3].z, m_vBoundBox[0].x, m_vBoundBox[0].z,  wx,  wz);

	if ( abs(bCCW[0] = (bCCW[0] + bCCW[1] + bCCW[2] + bCCW[3])) == 4)
		return TRUE;
	else
		return FALSE;
}

void CLdDecoObj::AdjustYPos(float fAdjust, BOOL bUpAni, BOOL bDoAni)
{
	m_dwAdjustTime = timeGetTime();

	m_bDoAni = bDoAni;
	m_bUpAni = bUpAni;

	if (m_bUpAni)
	{
		m_fAdjustY = fAdjust;
	}
	else
	{
		m_fAdjustY = 0.0f;
		m_fDownAdjustY = fAdjust;
	}

}

HRESULT CLdDecoObj::Render(BOOL bNoRoof, int whichAlpha, BOOL bAnimate ) ///2007로딩변경 때 애니메이트 여부 플래그 추가.
{
	if( !GET_D3DDEVICE() )
		return S_FALSE;

	static DWORD alphaTest, alphaBlend;
	float fCurTime, fTimeElapsed;

	// 오브젝트 위치 조정을 확인한다.
	if (m_fAdjustY <= 0.0f && m_bDoAni && m_bUpAni)
	{
		fCurTime = timeGetTime() / 1000.0f;
		fTimeElapsed = fCurTime - m_dwAdjustTime/1000.0f;

		while (fTimeElapsed > 0.05f)
		{
			m_fAdjustY += 1.0f;
			fTimeElapsed -= 0.05f;
		}

		if (m_fAdjustY >= 0.0f)
		{
			m_fAdjustY = 0.0f;
			m_bDoAni = FALSE;
		}
	}
	else if (m_fAdjustY >= m_fDownAdjustY && m_bDoAni && !m_bUpAni)
	{
		fCurTime = timeGetTime() / 1000.0f;
		fTimeElapsed = fCurTime - m_dwAdjustTime/1000.0f;

		while (fTimeElapsed > 0.05f)
		{
			m_fAdjustY -= 1.0f;
			fTimeElapsed -= 0.05f;
		}

		if (m_fAdjustY <= m_fDownAdjustY)
		{
			m_fAdjustY = m_fDownAdjustY;
			m_bDoAni = FALSE;
		}
	}

	D3DUtil_SetTranslateMatrix( m_matTrans, m_wx, m_wy + m_fAdjustY, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix( m_matRotate, m_dir);
	D3DMath_MatrixMultiply( m_matWorld, m_matRotate, m_matTrans );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );

	if (m_pLolo->m_AnimCount > 1 && bAnimate )   ///2007로딩변경 때 애니메이트 여부 플래그 추가.
	{
		m_fTimeElapsed += g_fTimeKey;
		while (m_fTimeElapsed >= m_fAniRate)
		{
			m_fTimeElapsed -= m_fAniRate;
			m_nCurFrame++;
		}

		while (m_nCurFrame >= m_pLolo->m_AnimCount )
			m_nCurFrame -= m_pLolo->m_AnimCount;
	}

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	// 알파 테스팅
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000030);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);

	if (whichAlpha == DECO_ALPHA_SEE || whichAlpha == DECO_ALPHA_FAR)
	{
		DWORD alpha;
		if (whichAlpha == DECO_ALPHA_FAR)
			alpha = (DWORD)(0xff * m_farFactor) << 24;
		else
			alpha = 0x55000000;

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

		DWORD alphaArg1, alphaArg2, alphaRef, alphaOp;
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG1, &alphaArg1);
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG2, &alphaArg2);
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,  &alphaOp);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000010);

		m_pLolo->Render(GET_D3DDEVICE(), m_nCurFrame, FALSE, FALSE, FALSE, NULL, NULL,
						FALSE, FALSE, alpha);

		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, alphaArg1);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, alphaArg2);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAOP, alphaOp);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF  , alphaRef);
	}
	else
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
		m_pLolo->Render(GET_D3DDEVICE(), m_nCurFrame, FALSE, bNoRoof);
	}


	if ( ( (m_pLolo->m_LineType == LINE_TYPE_FIRE) || (m_pLolo->m_LineType == LINE_TYPE_BLUEFIRE) )
			&& m_pLolo->m_LineVector0 )
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
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[0].x + m_wx,
										m_pLolo->m_LineVector0[0].y + m_wy,
										m_pLolo->m_LineVector0[0].z + m_wz );


			int index = 0;
			index = (int) fabs( (m_pLolo->m_LineVector0[1].y - m_pLolo->m_LineVector0[0].y) );

			if( index >= 9 && index <= 11 )
				ef_sort.Index = 10;
			else if( index >= 19 && index <= 21 )
				ef_sort.Index = 20;
			else
				ef_sort.Index = index;

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
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[0].x + m_wx,
										m_pLolo->m_LineVector0[0].y + m_wy,
										m_pLolo->m_LineVector0[0].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_ELECTRIC && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_ELECTRICPARTICLE;		// FX_SMOKE_PARTICLE
			ef_sort.effects = Electric_Particle;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_RED && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINEB && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_WINES && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_CLYSTAL_CYON && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_CLYSTALGLOW;
			ef_sort.effects = ClystalGlow;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_WATERFALLD && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_WATERFALL;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_WATERFALL && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_WATERFALLPARTICLE;
			ef_sort.effects = Waterfall_Particle;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

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
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}
	else if ( m_pLolo->m_LineType == LINE_TYPE_FIRESMOKE && m_pLolo->m_LineVector0 )
	{
		if( ! m_LineEffectsLoad )
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_FIRESMOKE_PARTICLE;
			ef_sort.effects = Firesmoke_Particle;
			ef_sort.vPos = D3DVECTOR(	m_pLolo->m_LineVector0[1].x + m_wx,
										m_pLolo->m_LineVector0[1].y + m_wy,
										m_pLolo->m_LineVector0[1].z + m_wz );

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			m_LineEffectsLoad = TRUE;
		}
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, alphaTest);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CLdDecoObjSet Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLdDecoObjSet::CLdDecoObjSet(IndexedTexture *pTexture)
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

CLdDecoObjSet::~CLdDecoObjSet()
{
	int i = 0;

	SAFE_DELETE_ARRAY(m_pLoloSet);
	SAFE_DELETE_ARRAY(m_pLolosDir);
	if (m_FileName != NULL)
	{
		for (i=0; i < m_nLoloCount; i++)
			SAFE_DELETE_ARRAY(m_FileName[i]);

		delete [] m_FileName;
		m_FileName = NULL;
	}

	if (m_NickName != NULL)
	{
		for (i=0; i < m_nLoloCount; i++)
			SAFE_DELETE_ARRAY(m_NickName[i]);
		SAFE_DELETE_ARRAY( m_NickName );
	}

	SAFE_DELETE_ARRAY(m_pnObjIdx);
	SAFE_DELETE_ARRAY(m_pnObjType);
}

void CLdDecoObjSet::SetLolosDir(char *dir_name)
{
	SAFE_DELETE_ARRAY(m_pLolosDir);
	m_pLolosDir = new char[strlen(dir_name)+1];
	strcpy(m_pLolosDir, dir_name);
}

void CLdDecoObjSet::ReadDataFile(char *file_name)
{
	PFILE *fp = NULL;
	PFILE *fpLolo = NULL;
	char path_name[256];
	int i, len;

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
			char temp[256];
			sprintf(temp, "Missing %s..", path_name);
			MessageBox(NULL, temp, temp, MB_OK);
		}
		assert(fpLolo != NULL);
		m_pLoloSet[i].Read(fpLolo, TRUE, FALSE, decode);
		pfclose(fpLolo);
	}

	pfclose(fp);
}

///////////////////////////////////////////////////////////////////////////
// Name : GetSentence()
// Desc : LdDecoObjSet.set 화일을 읽기 위한 것.
//////////////////////////////////////////////////////////////////////////
char * CLdDecoObjSet::GetSentence(char *dest, int n, FILE *fp)
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

char * CLdDecoObjSet::PGetSentence(char *dest, int n, PFILE *fp)
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
