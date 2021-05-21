#include "stdafx.h"


#include "headers.h"
#include "main.h"
#include "TextOutBox.h"
#include "ControlMap.h"
#include "ddutil.h"
#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "3d_PosPoint.h"
#include "mouse.h"
#include "uimgr.h"
#include "Land.h"
#include "IndexedTexture.h"
#include "LoadEffectSound.h"
#include "controlparty.h"
#include "tcpipcon.h"
#include "g_stringmanager.h"
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"

#include "controlbottom.h" ///미니맵길드원찾콅E

#include "DmitronBattle.h"

extern int		gMapWidth;							// 맵의 x횁E넓이
extern int		gMapHeight;
extern int		gMapExtention;					// 스케일 팩터 - 정수?E





extern BOOL			g_bDungeon;
extern POINT		g_CursorPoint;
extern DWORD		g_dwLangType;
extern DWORD		g_dwClientCountry;
extern BOOL			g_bGhost;
// nate 2005-03-02 : Matrix
extern int			g_MatrixSvr;


// WORDL
#define WORLD_DECKARDI		0 // 구 필탛E(GetWordNum = 0)
#define WORLD_NAOROSMIA     1 // 구 던젼 (GetWordNum = 1)
#define WORLD_DECKADUM      2 // 신 던젼 (GetWordNum = 2)
#define WORLD_LAGRAMIA	    4 // 신 필탛E(스타트 필탛EGetWordNum = 4)
//wan:2004-11
//Whitehorn Dungeon
#define WORLD_DUNGEON4	    8
// VILLAGE IN DECKARDI
#define NEO_RHASA       0
#define LOST_REALM		1
#define SALANOS			2
#define VILLAGE_HUMAN		3
#define VILLAGE_NONE	   -1

// DECKADUM POSITION
#define DECKADUM_STAIRS_1   0  // 신던픸E1층
#define DECKADUM_STAIRS_2   1 //        2층
#define DECKADUM_STAIRS_3   2 //        3층 
#define DECKADUM_STAIRS_4   3 //        4층 
#define DECKADUM_NONE       -1 //

extern BOOL g_bUseMatrixCountry; // 매트릭스(뫼비퓖E?아레나)가 도입된 나라인가의 플래그.(도입 안된나라는 뫼비퓖E?데이터를 패치 안해주콅E때문에 이게 FALSE여야한다.) ///해외뫼비퓖E?
extern BOOL		g_bAdmin2;


CControlMap::CControlMap()
	: m_hWnd(NULL)
	, m_dwTimeIndication(0)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_curpostext_x(0)
	, m_curpostext_y(0)
	, m_minimap_x(0)
	, m_minimap_y(0)
	, m_iChaRoomPosX(0)
	, m_iChaRoomPosZ(0)
	, m_MyPosInVillage_x(0)
	, m_MyPosInVillage_y(0)
	, m_iSeaRoost(0)
{
	// nate 2004 - 5 : 크리스탈 소유자 위치 표시
	m_nCristalOwnerX = 0;
	m_nCristalOwnerZ = 0;
	m_pPosPoint		= NULL; // 케릭터 위치 포인터
	m_pFlag_A      = NULL;
	m_pFlag_B       = NULL;
	m_pMyPartys     = NULL;
	m_pMyGuilds		= NULL; ///미니맵길드원찾콅ED
	m_pBack			= NULL;	// 백그라웝?E이미햨E
	//==================================
	// nate 2005-03-21 : Matrix : indication
	m_pIndication_Point = NULL;
	m_bIndication_Point = 0;
	m_nCurClick_X = 0;
	m_nCurClick_Y = 0;
	//==================================
	m_pDeckardi			= NULL; // 구 필탛E
	m_pLagramia			= NULL;	// 신 필탛E
	m_pWhiteHorn		= NULL;
	m_pDeckadum_1		= NULL; // 신 던픸E
	m_pDeckadum_2		= NULL;
	m_pDeckadum_3		= NULL;
	m_pDeckadum_4		= NULL;
	m_pDeckaren_0		= NULL; ///인큱E
	m_pDeckaren_1		= NULL;
	m_pDeckaren_2		= NULL;
	m_pSeaRoost_first	= NULL; //1227
	m_pSeaRoost_second	= NULL;
	m_pSeaRoost_third	= NULL;
	m_pSeaRoost_forth	= NULL;
	m_pParos			= NULL;
	m_pShilon			= NULL; // 샤일론 필탛E
	m_pGWar				= NULL; // 길드픸E성
	m_pDmitron			= NULL;	// nate 2005-07-12 : Dmitron
	m_pBossRaidBulkan		= NULL;
	m_pBossRaidKailipton	= NULL;
	m_pBossRaidAidia		= NULL;
	m_pBossRaidHuman		= NULL;
	m_pBossRaidHybrider		= NULL;
	m_pBossRaidFinalboss	= NULL;
	m_pBossRaidFinalboss_Room	= NULL;

	m_pMoebius = NULL; // [2008/11/17 Theodoric] WORLD_MATRIX_2009
	m_pDisposal = NULL;
	m_pVidBline = NULL;

	m_zone_22 = NULL;
	m_zone_23 = NULL;
	m_zone_24 = NULL;
	m_Hightland = NULL;
	m_Tritian = NULL;
	m_Space = NULL;
	m_Royale = NULL;
	m_dun910 = NULL;
	m_zone930 = NULL;
//=========================================
	// nate 2004 - 9 : GuildHunt
	m_pShilonGuildHuntMap = NULL;
	m_pWhiteHornGuildhuntMap = NULL;
//=========================================
	m_pLagramia_detail = NULL;
	m_pTutorialIsland = NULL; ///튜토리푳E
	m_pShailon_detail = NULL;
	m_pLost_detail    = NULL;
	m_pInDoor = NULL;  // 신던젼 에서 입구 / 출구 표시
	m_pOutDoor = NULL;
	// wan:2004-11 : Whitehorn Dungeon
	m_pWhiteHornDungeon	= NULL;
	// nate 2005-03-21 : Matrix : Mini Map
	m_pMatrix = NULL;
//========================================================================
	// nate 2005-03-31 : 메트릭스 주둔지에 길탛E마크 출력
	m_pHawklaw = NULL;	// hawklaw
	m_pCoolrain = NULL;	// coolrain
	m_pShiver = NULL;	// shiver
	m_pArrow = NULL;
	m_dwAniTime = 0L;
	m_nAniframe = 0;
	ZeroMemory( m_nArmyPostX, sizeof( m_nArmyPostX ) );
	ZeroMemory( m_nArmyPostY, sizeof( m_nArmyPostY ) );
	// nate 2005-04-18
	m_pBrokenGate = NULL;
	ZeroMemory( m_nBrokenGatePosX, sizeof( m_nBrokenGatePosX ) );
	ZeroMemory( m_nBrokenGatePosY, sizeof( m_nBrokenGatePosY ) );
//========================================================================
	m_GuildPosVector.reserve( 50 ); // 현픸E최큱E50툈E깩痺만 보내주기로 했음. ///미니맵길드원찾콅E

	// [12/8/2008 Theodoric] WORLD_MATRIX_2009
	m_pTundmar	= NULL;
	m_pAngmar	= NULL;
	m_pOpenGate = NULL;
	Init();
}

CControlMap::~CControlMap()
{
	DeleteRes();
	SAFE_DELETE(m_pPosPoint);
	SAFE_DELETE(m_pFlag_A);
	SAFE_DELETE(m_pFlag_B);
	SAFE_DELETE(m_pMyPartys);
	SAFE_DELETE( m_pIndication_Point );

	SAFE_DELETE(m_pMyGuilds); ///미니맵길드원찾콅ED

	// 리스트 초기화.  ///미니맵길드원찾콅ED
	vector<sGuildPosInfo>::iterator prGuildPosVector;
	prGuildPosVector = m_GuildPosVector.begin();
	while( prGuildPosVector != m_GuildPosVector.end() )
	{
		prGuildPosVector = m_GuildPosVector.erase( prGuildPosVector );
	}
}

void CControlMap::Init()
{
	m_pPosPoint = new C3d_PosPoint();
	m_pFlag_A = new C3d_PosPoint();
	m_pFlag_B = new C3d_PosPoint();
	m_pMyPartys = new C3d_PosPoint();
	m_pMyGuilds = new C3d_PosPoint(); ///미니맵길드원찾콅ED
	m_pIndication_Point = new C3d_PosPoint();

	m_GWarBtn.SetFileName("map/btn_gwar");
	m_CloseBtn.SetFileName("common/btn_close_01");
	m_GuildFindBtn.SetFileName( "map/btn_search_guild" );
	m_PlayerFindBtn.SetFileName( "map/btn_search_ppl" );

	GuildFindTick = 0; // 효컖E연출퓖E틱 ///미니맵길드원찾콅E
	m_DrawGuildNameNum = 0; // 길원탛E이름을 그려야하는 갯수퓖E변펯E 이게 0이툈E안그려줘도 되는거다. ///미니맵길드원찾콅E


	m_iCurWorld     = WORLD_LAGRAMIA; // 최초 시작 월드는 라그라미아..필탛E
	m_iCurVillage   = VILLAGE_NONE;   // 구던픸E【??현제 마을은 디폴트 값으로
	m_iCurDeckadum  = DECKADUM_NONE; // 최초 신던픸E【?뫈署에 존재하는강
	m_iCurDeckaLen  = -1;
	m_bShowGWarMap	= FALSE;
	m_eState		= MOUSE_NONE;

	m_iDmitronPopup = -1;
	SetRect(&m_rcDmitronPopup, 0, 0, 0, 0 );
}

void CControlMap::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	SAFE_DELETE(m_pBack);
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/map/map.BMP") ) )
		return;

	m_pBack->SetColorKey( TRANS_COLOR );

	m_CloseBtn.LoadRes();
	m_GWarBtn.LoadRes();
	m_GuildFindBtn.LoadRes();
	m_PlayerFindBtn.LoadRes();
	m_GuildPosNameBox.LoadRes(m_ClientRc.right, m_ClientRc.bottom ); // 길드퓖E이름을 혖E蝴獵?반투툈E박스 ///미니맵길드원찾콅E

	if( m_pPosPoint )			m_pPosPoint->LoadRes();
	if( m_pFlag_A )				m_pFlag_A->LoadRes(6, 6);
	if( m_pFlag_B )				m_pFlag_B->LoadRes(6, 6);
	if( m_pMyPartys )			m_pMyPartys->LoadRes(3, 3);
	if( m_pMyGuilds )			m_pMyGuilds->LoadRes(3, 3); ///미니맵길드원찾콅ED
	if( m_pIndication_Point )	m_pIndication_Point->LoadRes(3, 3);

	// 필탛E, 던젼
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckardi, "interface/map/mini_map.bmp"); // 구 필탛E
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckadum_1, "interface/map/new_dg_1.bmp"); // 신 던젼
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckadum_2, "interface/map/new_dg_2.bmp"); // 신 던젼
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckadum_3, "interface/map/new_dg_3.bmp"); // 신 던젼
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckadum_4, "interface/map/new_dg_4.bmp"); // 신 던젼

	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckaren_0, "interface/map/deckaren0.bmp"); ///인큱E
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckaren_1, "interface/map/deckaren1.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pDeckaren_2, "interface/map/deckaren2.bmp");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pLagramia, "interface/map/start_field.bmp"); // 신 필탛E
	g_pDisplay->CreateSurfaceFromBitmap(&m_pWhiteHorn, "interface/map/whitehorn.bmp"); // Great white horn
	g_pDisplay->CreateSurfaceFromBitmap(&m_pShilon, "interface/map/shilon.bmp");// 샤일론 필탛E
//==========================================================================================
	// nate 2004 - 9 : GuildHunt
	g_pDisplay->CreateSurfaceFromBitmap( &m_pShilonGuildHuntMap, "interface/map/ShilonGuildHunt.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pWhiteHornGuildhuntMap, "interface/map/WhiteHornGuildHunt.bmp" );
//==========================================================================================
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSeaRoost_first, "interface/map/searoost1.bmp"); // 1227 시루스트
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSeaRoost_second, "interface/map/searoost2.bmp"); // 1227 시루스트
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSeaRoost_third, "interface/map/searoost3.bmp"); // 1227 시루스트
	g_pDisplay->CreateSurfaceFromBitmap( &m_pSeaRoost_forth, "interface/map/searoost4.bmp" );
	if (m_pSeaRoost_first)
		m_pSeaRoost_first->SetColorKey(RGB(0, 0, 0));
	g_pDisplay->CreateSurfaceFromBitmap(&m_pLagramia_detail, "interface/map/neo_rhasa.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pTutorialIsland, "interface/map/TutorialIsland.bmp");     ///튜토리푳E
	g_pDisplay->CreateSurfaceFromBitmap(&m_pShailon_detail, "interface/map/salanos.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pLost_detail, "interface/map/lost_realm.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pParos, "interface/map/paros.bmp");
	// 샤일론에서 길드픸E성
	g_pDisplay->CreateSurfaceFromBitmap(&m_pGWar, "interface/map/gwar.bmp");    // 퍊E款갋?항구
	// 신 던젼에서의 횁E입구 표시
	g_pDisplay->CreateSurfaceFromBitmap(&m_pInDoor, "interface/map/entrance_door.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pOutDoor,"interface/map/exit_door.bmp");
	// wan:2004-11 : Whitehorn Dungeon
	g_pDisplay->CreateSurfaceFromBitmap(&m_pWhiteHornDungeon, "interface/map/dungeon4.bmp");
	// nate 2005-07-12 : Dmitron
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDmitron, "interface/map/Dmitron.bmp" );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBossRaidBulkan,		"interface/map/bossraid_bulkan.bmp"		);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBossRaidKailipton,		"interface/map/bossraid_Kailipton.bmp"	);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBossRaidAidia,			"interface/map/bossraid_Aida.bmp"		);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBossRaidHuman,			"interface/map/bossraid_Human.bmp"		);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBossRaidHybrider,		"interface/map/bossraid_Hybrider.bmp"	);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBossRaidFinalboss,		"interface/map/bossraid_Finalboss.bmp"	);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBossRaidFinalboss_Room,"interface/map/bossraid_Finalboss_room.bmp"	);

	g_pDisplay->CreateSurfaceFromBitmap( &m_pMoebius,"interface/map/moebius.bmp"); // [2008/11/17 Theodoric] WORLD_MATRIX_2009

	g_pDisplay->CreateSurfaceFromBitmap( &m_pDisposal,"interface/map/forlorn_top.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pVidBline,"interface/map/vidbline.bmp");

	g_pDisplay->CreateSurfaceFromBitmap( &m_zone_22,"interface/map/ForlornRuinS2.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_zone_23,"interface/map/ForlornRuinS3.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_zone_24,"interface/map/ForlornRuinS4.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_Hightland,"interface/map/Highland.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_Tritian,"interface/map/Tritian.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_Space,"interface/map/Space.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_Royale,"interface/map/Royale.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_dun910,"interface/map/Kubera.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_zone930,"interface/map/Fantasy.bmp");

	if( g_bUseMatrixCountry ) ///해외뫼비퓖E?
	{
		// nate 2005-03-21 : Matrix
		g_pDisplay->CreateSurfaceFromBitmap(&m_pMatrix, "interface/map/matrix.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pArrow, "interface/map/Indication.bmp" );
		if( m_pArrow )
			m_pArrow->SetColorKey( RGB( 0, 0, 0 ) );
		//========================================================================
		// nate 2005-03-31 : 메트릭스 주둔지에 길탛E마크 출력
		g_pDisplay->CreateSurfaceFromBitmap(&m_pHawklaw, "interface/map/Hawklaw.bmp");
		if( m_pHawklaw )
			m_pHawklaw->SetColorKey( RGB( 0, 0, 0 ) );
		g_pDisplay->CreateSurfaceFromBitmap(&m_pCoolrain, "interface/map/Coolrain.bmp");
		if( m_pCoolrain )
			m_pCoolrain->SetColorKey( RGB( 0, 0, 0 ) );
		g_pDisplay->CreateSurfaceFromBitmap(&m_pShiver, "interface/map/Shiver.bmp");
		if( m_pShiver )
			m_pShiver->SetColorKey( RGB( 0, 0, 0 ) );
		// nate 2005-04-18
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBrokenGate, "interface/map/BrokenGate.bmp");
		if( m_pBrokenGate )
			m_pBrokenGate->SetColorKey( RGB( 0, 0, 0 ) );
		//========================================================================
	}
	g_pDisplay->CreateSurfaceFromBitmap(&m_pTundmar, "interface/map/tundmar.bmp");
	if( m_pTundmar )
		m_pTundmar->SetColorKey( RGB( 0, 0, 0 ) );
	g_pDisplay->CreateSurfaceFromBitmap(&m_pAngmar, "interface/map/Angmar.bmp");
	if( m_pAngmar )
		m_pAngmar->SetColorKey( RGB( 0, 0, 0 ) );
	g_pDisplay->CreateSurfaceFromBitmap(&m_pOpenGate, "interface/map/BrokenGate_2009.bmp");
	if( m_pOpenGate )
		m_pOpenGate->SetColorKey( RGB( 0, 0, 0 ) );

	m_MatrixStoneInfoPopup.LoadRes(m_ClientRc.right, m_ClientRc.bottom);
	m_MatrixStoneInfoPopup.Init(12);


	m_pBack->Xpos = m_ClientRc.right - m_pBack->GetWidth();
	m_pBack->Ypos = 0;
	m_PosBox.Init(PLAYERPOS_FONTSIZE, PLAYERPOS_FONTCOLOR, 0, 0, -1, -1, TRUE);
	m_LocBox.Init(PLAYERPOS_FONTSIZE, PLAYERPOS_FONTCOLOR, 0, 0, -1, -1, TRUE);
	m_PosBox.SetVisible(TRUE);
	m_LocBox.SetVisible(TRUE);

	m_curpostext_x = m_pBack->Xpos+116; // 현픸E나의 위치
	m_curpostext_y = m_pBack->Ypos+384;
	m_minimap_x    = m_pBack->Xpos+16;
	m_minimap_y    = m_pBack->Ypos+93;


	m_PlayerFindBtn.SetPosition( m_pBack->Xpos+114, m_pBack->Ypos+355 );
	m_GuildFindBtn.SetPosition( m_pBack->Xpos+114, m_pBack->Ypos+415 ); ///미니맵길드원찾콅E
	m_CloseBtn.SetPosition( m_pBack->Xpos + 257 , m_pBack->Ypos + 6 );

	m_GuildPosNameBox.Init(12); // 길드퓖E이름을 혖E蝴獵?반투툈E박스 ///미니맵길드원찾콅E




	//	구 필탛E
	if( m_pDeckardi )
	{
		m_pDeckardi->Xpos = m_minimap_x;
		m_pDeckardi->Ypos = m_minimap_y;
	}
	// 신 필탛E
	if( m_pLagramia )
	{
		m_pLagramia->Xpos = m_minimap_x;
		m_pLagramia->Ypos = m_minimap_y;
	}
	// Greate white horn
	if( m_pWhiteHorn )
	{
		m_pWhiteHorn->Xpos = m_minimap_x;
		m_pWhiteHorn->Ypos = m_minimap_y;
	}
	// 샤일론 필탛E
	if( m_pShilon )
	{
		m_pShilon->Xpos = m_minimap_x;
		m_pShilon->Ypos = m_minimap_y;
	}
	// nate 2004 - 9 : GuildHunt - 샤일론 길탛E사냥
	if( m_pShilonGuildHuntMap )
	{
		m_pShilonGuildHuntMap->Xpos = m_minimap_x;
		m_pShilonGuildHuntMap->Ypos = m_minimap_y;
	}
	// 화이트혼 길탛E사냥
	if( m_pWhiteHornGuildhuntMap )
	{
		m_pWhiteHornGuildhuntMap->Xpos = m_minimap_x;
		m_pWhiteHornGuildhuntMap->Ypos = m_minimap_y;
	}
	if( m_pDeckadum_1 && m_pDeckadum_2 && m_pDeckadum_3 && m_pDeckadum_4 )
	{
		m_pDeckadum_1->Xpos = m_pDeckadum_2->Xpos = m_pDeckadum_3->Xpos = m_pDeckadum_4->Xpos = m_minimap_x;//+((MINI_MAP_WIDTH - NEW_DG_MAP_WIDTH)/2);
		m_pDeckadum_1->Ypos = m_pDeckadum_2->Ypos = m_pDeckadum_3->Ypos = m_pDeckadum_4->Ypos = m_minimap_y;//+((MINI_MAP_HEIGHT - NEW_DG_MAP_HEIGHT)/2);
	}
	if( m_pDeckaren_0 && m_pDeckaren_1 && m_pDeckaren_2 ) ///인큱E 때 수정.
	{
		m_pDeckaren_0->Xpos = m_pDeckaren_1->Xpos = m_pDeckaren_2->Xpos = m_minimap_x;
		m_pDeckaren_0->Ypos = m_pDeckaren_1->Ypos = m_pDeckaren_2->Ypos = m_minimap_y;
	}

	//시루스트 던픸E242 256
	if( m_pSeaRoost_first )
	{
		m_pSeaRoost_first->Xpos = m_minimap_x;
		m_pSeaRoost_first->Ypos = m_minimap_y;
	}
	if( m_pSeaRoost_second )
	{
		m_pSeaRoost_second->Xpos = m_minimap_x+49;//53;
		m_pSeaRoost_second->Ypos = m_minimap_y;
	}
	if( m_pSeaRoost_third )
	{
		m_pSeaRoost_third->Xpos = m_minimap_x+93;//93;
		m_pSeaRoost_third->Ypos = m_minimap_y+73;
	}
	if( m_pSeaRoost_forth )
	{
		m_pSeaRoost_forth->Xpos = m_minimap_x+103;//93;
		m_pSeaRoost_forth->Ypos = m_minimap_y+148;
	}
	if( m_pParos )
	{
		m_pParos->Xpos = m_minimap_x;
		m_pParos->Ypos = m_minimap_y;
	}
	if( m_pLagramia_detail )
	{
		m_pLagramia_detail->Xpos = m_minimap_x;
		m_pLagramia_detail->Ypos = m_minimap_y;
	}
	if( m_pTutorialIsland ) ///튜토리푳E
	{
		m_pTutorialIsland->Xpos = m_minimap_x;
		m_pTutorialIsland->Ypos = m_minimap_y;
	}
	if( m_pShailon_detail )
	{
		m_pShailon_detail->Xpos = m_minimap_x;
		m_pShailon_detail->Ypos = m_minimap_y;
	}
	if( m_pLost_detail )
	{
		m_pLost_detail->Xpos = m_minimap_x;
		m_pLost_detail->Ypos = m_minimap_y;
	}
	if( m_pGWar )
	{
		m_pGWar->Xpos = m_minimap_x;
		m_pGWar->Ypos = m_minimap_y;
	}
	// wan:2004-11 : whitehorn Dungeon
	if( m_pWhiteHornDungeon )
	{
		m_pWhiteHornDungeon->Xpos = m_minimap_x;
		m_pWhiteHornDungeon->Ypos = m_minimap_y;
	}
	// nate 2005-03-21 : Matrix
	if( m_pMatrix ) ///해외뫼비퓖E?
	{
		m_pMatrix->Xpos = m_minimap_x;
		m_pMatrix->Ypos = m_minimap_y;
	}
	// nate 2005-07-12 : Dmitron
	if( m_pDmitron )
	{
		m_pDmitron->Xpos = m_minimap_x;
		m_pDmitron->Ypos = m_minimap_y;

		m_DmitronPopupText.Init( 12, RGB(255,255,255), 0, 0 );
	}

	if( m_pBossRaidBulkan )
	{
		m_pBossRaidBulkan->Xpos = m_minimap_x;
		m_pBossRaidBulkan->Ypos = m_minimap_y;
	}
	if( m_pBossRaidKailipton )
	{
		m_pBossRaidKailipton->Xpos = m_minimap_x;
		m_pBossRaidKailipton->Ypos = m_minimap_y;
	}
	if( m_pBossRaidAidia)
	{
		m_pBossRaidAidia->Xpos = m_minimap_x;
		m_pBossRaidAidia->Ypos = m_minimap_y;
	}
	if( m_pBossRaidHuman )
	{
		m_pBossRaidHuman->Xpos = m_minimap_x;
		m_pBossRaidHuman->Ypos = m_minimap_y;
	}
	if( m_pBossRaidHybrider)
	{
		m_pBossRaidHybrider->Xpos = m_minimap_x;
		m_pBossRaidHybrider->Ypos = m_minimap_y;
	}
	if( m_pBossRaidFinalboss)
	{
		m_pBossRaidFinalboss->Xpos = m_minimap_x;
		m_pBossRaidFinalboss->Ypos = m_minimap_y;
	}

	if( m_pBossRaidFinalboss_Room)
	{
		m_pBossRaidFinalboss_Room->Xpos = m_minimap_x;
		m_pBossRaidFinalboss_Room->Ypos = m_minimap_y;
	}

	if( m_pMoebius ) // [2008/11/17 Theodoric] WORLD_MATRIX_2009
	{
		m_pMoebius->Xpos = m_minimap_x;
		m_pMoebius->Ypos = m_minimap_y;
	}

	if( m_pDisposal )
	{
		m_pDisposal->Xpos = m_minimap_x;
		m_pDisposal->Ypos = m_minimap_y;
	}

	if( m_pVidBline )
	{
		m_pVidBline->Xpos = m_minimap_x;
		m_pVidBline->Ypos = m_minimap_y;
	}

	if( m_zone_22 )
	{
		m_zone_22->Xpos = m_minimap_x;
		m_zone_22->Ypos = m_minimap_y;
	}

	if( m_zone_23 )
	{
		m_zone_23->Xpos = m_minimap_x;
		m_zone_23->Ypos = m_minimap_y;
	}

	if( m_zone_24 )
	{
		m_zone_24->Xpos = m_minimap_x;
		m_zone_24->Ypos = m_minimap_y;
	}

	if( m_Hightland)
	{
		m_Hightland->Xpos = m_minimap_x;
		m_Hightland->Ypos = m_minimap_y;
	}

	if( m_Tritian)
	{
		m_Tritian->Xpos = m_minimap_x;
		m_Tritian->Ypos = m_minimap_y;
	}

	if( m_Space)
	{
		m_Space->Xpos = m_minimap_x;
		m_Space->Ypos = m_minimap_y;
	}

	if( m_Royale)
	{
		m_Royale->Xpos = m_minimap_x;
		m_Royale->Ypos = m_minimap_y;
	}

	if( m_dun910)
	{
		m_dun910->Xpos = m_minimap_x;
		m_dun910->Ypos = m_minimap_y;
	}

	if( m_zone930)
	{
		m_zone930->Xpos = m_minimap_x;
		m_zone930->Ypos = m_minimap_y;
	}

	//========================================================================
	// nate 2005-03-31
	// 메트릭스 주둔지에 길탛E마크 출력
	m_nArmyPostX[ 2 ] = m_minimap_x + 200;	// 1 주둔햨E
	m_nArmyPostY[ 2 ] = m_minimap_y + 135;
	m_nArmyPostX[ 1 ] = m_minimap_x + 15;	// 2 주둔햨E
	m_nArmyPostY[ 1 ] = m_minimap_y + 135;
	m_nArmyPostX[ 0 ] = m_minimap_x + 172;	// 3 주둔햨E
	m_nArmyPostY[ 0 ] = m_minimap_y + 35;
	// nate 2005-04-18
	int nar_TempX[] = { 127, 101, 79, 93, 137, 150 };
	int nar_TempY[] = { 158, 158, 114, 92, 92, 118 };

	for( int i = 0 ; i < 6 ; i ++ )
	{
		m_nBrokenGatePosX[ i ] = m_minimap_x + nar_TempX[ i ];
		m_nBrokenGatePosY[ i ] = m_minimap_y + nar_TempY[ i ];
	}
//========================================================================
	m_GWarBtn.SetPosition(m_minimap_x+133, m_minimap_y+193);
}

void CControlMap::DeleteRes()
{
	//int i;
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pDeckardi);
	SAFE_DELETE(m_pShilon);
	// nate 2004 - 9 : GuildHunt
	SAFE_DELETE( m_pShilonGuildHuntMap );
	SAFE_DELETE( m_pWhiteHornGuildhuntMap );
	SAFE_DELETE(m_pLagramia);
	SAFE_DELETE(m_pGWar);
	SAFE_DELETE( m_pWhiteHorn );
	// nate 2005-03-21 : Matrix
	SAFE_DELETE( m_pMatrix );
	// nate 2005-03-31 : 메트릭스 주둔지에 길탛E마크 출력
	SAFE_DELETE( m_pHawklaw );
	SAFE_DELETE( m_pCoolrain );
	SAFE_DELETE( m_pShiver );
	// nate 2005-04-18
	SAFE_DELETE( m_pBrokenGate );
	SAFE_DELETE(m_pDeckadum_1);
	SAFE_DELETE(m_pDeckadum_2);
	SAFE_DELETE(m_pDeckadum_3);
	SAFE_DELETE(m_pDeckadum_4);
	SAFE_DELETE( m_pDeckaren_0 ); ///인큱E
	SAFE_DELETE( m_pDeckaren_1 );
	SAFE_DELETE( m_pDeckaren_2 );
	SAFE_DELETE(m_pInDoor);
	SAFE_DELETE(m_pOutDoor);
	SAFE_DELETE(m_pSeaRoost_first);
	SAFE_DELETE(m_pSeaRoost_second);
	SAFE_DELETE(m_pSeaRoost_third);
	SAFE_DELETE( m_pSeaRoost_forth );
	SAFE_DELETE(m_pTutorialIsland); ///튜토리푳E
	SAFE_DELETE(m_pLagramia_detail);
	SAFE_DELETE(m_pShailon_detail);
	SAFE_DELETE(m_pLost_detail);
	SAFE_DELETE(m_pParos);
	SAFE_DELETE( m_pDmitron );
	// nate 2005-03-19 : Matrix : Indication
	SAFE_DELETE( m_pArrow );

	SAFE_DELETE( m_pBossRaidBulkan );
	SAFE_DELETE( m_pBossRaidKailipton );
	SAFE_DELETE( m_pBossRaidAidia );
	SAFE_DELETE( m_pBossRaidHuman );
	SAFE_DELETE( m_pBossRaidHybrider );
	SAFE_DELETE( m_pBossRaidFinalboss );
	SAFE_DELETE( m_pBossRaidFinalboss_Room );

	SAFE_DELETE( m_pMoebius); // [2008/11/17 Theodoric] WORLD_MATRIX_2009
	SAFE_DELETE(m_pDisposal);

	SAFE_DELETE(m_pVidBline);

	SAFE_DELETE(m_zone_22);
	SAFE_DELETE(m_zone_23);
	SAFE_DELETE(m_zone_24);
	SAFE_DELETE(m_Hightland);
	SAFE_DELETE(m_Tritian);
	SAFE_DELETE(m_Space);
	SAFE_DELETE(m_Royale);
	SAFE_DELETE(m_dun910);
	SAFE_DELETE(m_zone930);

	m_pPosPoint->DeleteRes();
	m_pFlag_A->DeleteRes();
	m_pFlag_B->DeleteRes();
	m_pMyPartys->DeleteRes();
	m_pIndication_Point->DeleteRes();
	m_CloseBtn.DeleteRes();
	m_PosBox.DeleteRes();
	m_LocBox.DeleteRes();
	m_GWarBtn.DeleteRes();

	SAFE_DELETE( m_pTundmar );
	SAFE_DELETE( m_pAngmar );
	SAFE_DELETE( m_pOpenGate );
	SAFE_DELETE( m_pWhiteHornDungeon );
	m_MatrixStoneInfoPopup.DeleteRes();

	m_DmitronPopupText.DeleteRes();

	///미니맵길드원찾콅E

	m_pMyGuilds->DeleteRes(); ///미니맵길드원찾콅ED

	m_PlayerFindBtn.DeleteRes();
	m_GuildFindBtn.DeleteRes(); // 길드퓖E찾콅E버튼.
	m_GuildPosNameBox.DeleteRes(); // 길드퓖E이름을 혖E蝴獵?반투툈E박스
}

void CControlMap::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return;
	// 배경이미햨E
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	// 현제 월탛E인덱스를 얻는다
	int cur_world = pCMyApp->GetCurWorld();

	if(m_iCurWorld != cur_world)
		m_iCurWorld = cur_world;

	// 구 필탛E 신 필탛E, 신던픸E
	Show_Map(m_iCurWorld);

	/////////////////////////////////////////////////////////////////////////////
	// 미니맵 길드퓖E찾콅E기능 (05-10-18 원석)  ///미니맵길드원찾콅E
	if( pCMyApp->m_nCurWorldIdx != 10 || pCMyApp->m_nCurWorldIdx != 19 ||  g_bAdmin2 ) // 매트릭스에선 길원찾콅E안됨. // 웝滂자는뫼비퓖E봇【??되도록
	{
		m_GuildFindBtn.Draw(); // 길드퓖E찾콅E버튼.
		m_PlayerFindBtn.Draw();


		ShowGuildMember(); // 길드퓖E위치 출력 퓖E(안에서 표시할햨E말지를 체크해서 혖E?
	}
	/////////////////////////////////////////////////////////////////////////////

	m_CloseBtn.Draw();
}



//------------------------------------------------------------------------
// name : Show_Map
// desc : 현제 월드에 따라서 분콅E
//------------------------------------------------------------------------
void CControlMap::Show_Map(int index)
{
	if( !g_pDisplay	|| !g_pRoh )
		return;

	int bgx = m_pBack->Xpos;
	int bgy = m_pBack->Ypos;


	int mx = bgx + 16;
	int my = bgy + 93;

	//int i;
	int xpos = 0, ypos = 0;
	int aa = 0, bb = 0;
	char strLoc[50] = "";

	// 이거 초기화 안해주툈E맵이름 안혖E?맵의 경퓖E깨진글이 나오는 경퓖E?있다.
	ZeroMemory( strLoc, sizeof(strLoc) );
	strLoc[0] = NULL;

	switch(index)
	{
	// 구 필탛E(GetWordNum = 0)
	case WORLD_DECKARDI:
		if(m_iCurVillage == 3)	// 로스트렐름
		{
			if( m_pLost_detail )
				g_pDisplay->Blt(mx, my, m_pLost_detail);
			Show_PosPoint_InVillage();

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );
			//-- IDS_GATE_LOST_REALM : 로스트렐름 성
			sprintf(strLoc, G_STRING(IDS_GATE_LOST_REALM));//1223
			m_LocBox.SetString(strLoc,RGB(231, 203, 102),RGB(0,0,0));
			Show_Pos_Text(m_curpostext_x, m_curpostext_y);
			return;
		}

		if( m_pDeckardi )
			g_pDisplay->Blt(mx, my, m_pDeckardi);
		xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
		ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );
		if( m_pPosPoint )
			m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		Show_PosPoint_Partys();

		//-- IDS_GATE_LOST_REALM : 로스트렐름 성
		sprintf(strLoc, G_STRING(IDS_GATE_LOST_REALM));//1223
		break;

	// nate 2005-07-12 : Dmitron
	case WORLD_DMITRON:
		if( m_pDmitron )
			g_pDisplay->Blt( mx, my, m_pDmitron );
		xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
		ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );
		if( !pCMyApp->IsAcropolis() )
		{
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		}
		Show_PosPoint_Partys();

			Show_Dmitron_Battle_Status();
		// IDS_MAP_DMITRON	: 드미트론
		sprintf( strLoc, G_STRING(IDS_MAP_DMITRON));
		break;

	case WORLD_DMITRON_BATTLE:
		if( m_pDmitron )
			g_pDisplay->Blt( mx, my, m_pDmitron );
		xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
		ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );
		if( !pCMyApp->IsAcropolis() )
		{
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		}
		Show_PosPoint_Partys();
			Show_Dmitron_Battle_Status();
		// IDS_MAP_DMITRON_BATTLE : 드미트론 주둔햨E
		sprintf( strLoc, G_STRING(IDS_DMITRON_BATTLE_ZONE6));
		break;

	// 구 던젼 (GetWordNum = 1)
	case WORLD_NAOROSMIA:
		///인큱E
		if( !g_pRoh )
			return;

		RECT	rect;

		if( pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 7300,2200,8500,1300) ) // 큱E蒐퓻?있는햨E검퍊E
			m_iCurDeckaLen = 0;
		else if( pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 150, 5110,6600,596) ) // 1층에 있는햨E검퍊E
			m_iCurDeckaLen = 1;
		else if( pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 300,9700,6800,5100) ) // 2층에 있는햨E검퍊E
			m_iCurDeckaLen = 2;
		else
			m_iCurDeckaLen = -1;

		switch( m_iCurDeckaLen )
		{
		case 0: // 큱E蒐?
			if( m_pDeckaren_0 )
				g_pDisplay->Blt( mx, my, m_pDeckaren_0 );
			/*
			if( m_pInDoor ) // 1층가는 오틒E㎷?위치표시
				g_pDisplay->Blt(m_minimap_x+0, m_minimap_y+0, m_pInDoor);
			if( m_pOutDoor ) // 데카르디로 나가는 오틒E㎷?위치표시.
				g_pDisplay->Blt(m_minimap_x+0, m_minimap_y+0, m_pOutDoor);
			*/

			SetRect(&rect, 7250,2200,8500,1300);
			CalPos(rect, m_minimap_x, m_minimap_y+45, 235, 165);

			//-- IDS_WAITROOM : 큱E繡갋
			sprintf(strLoc, G_STRING(IDS_WAITROOM), m_iCurDeckaLen);

			break;

		case 1: // 1층
			if( m_pDeckaren_1 )
				g_pDisplay->Blt( mx, my, m_pDeckaren_1 );

//			if( m_pOutDoor ) // 2층가는 오틒E㎷?위치표시
//				g_pDisplay->Blt(m_minimap_x+0, m_minimap_y+0, m_pOutDoor);

			SetRect(&rect,150, 5110,6600,596);
			CalPos(rect, m_minimap_x, m_minimap_y+45, 245, 165);

			//-- IDS_MAP_DEKALEN : 데카렌%d층
			sprintf(strLoc, G_STRING(IDS_MAP_DEKALEN), m_iCurDeckaLen);

			break;

		case 2: // 2층
			if( m_pDeckaren_2 )
				g_pDisplay->Blt( mx, my, m_pDeckaren_2 );

			SetRect(&rect, 300,9700,6800,5100);
			CalPos(rect, m_minimap_x, m_minimap_y+45, 236, 165);

			//-- IDS_MAP_DEKALEN : 데카렌%d층
			sprintf(strLoc, G_STRING(IDS_MAP_DEKALEN), m_iCurDeckaLen);

			break;

		default: // 지정된 영역이 아닌 경퓖E..
			aa = (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth;
			bb = ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			xpos = m_minimap_x + aa;
			ypos = m_minimap_y + bb;

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			break;
		}
		/*
				aa = (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth;
				bb = ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

				if( aa >= 125 )
				{
					m_iCurDeckaLen = 2;
					if( m_pDeckaren_2 )
						g_pDisplay->Blt( m_pDeckaren_2->Xpos, m_pDeckaren_2->Ypos, m_pDeckaren_2 );
				}
				else
				{
					m_iCurDeckaLen = 1;
					if( m_pDeckaren_1 )
						g_pDisplay->Blt( m_pDeckaren_1->Xpos, m_pDeckaren_1->Ypos, m_pDeckaren_1 );
				}

				if( m_iCurDeckaLen == 2 )
				{
					if( m_pInDoor )
						g_pDisplay->Blt(m_minimap_x+130, m_minimap_y+MINI_MAP_HEIGHT-22, m_pInDoor);
				}
				else
				{
					if( m_pInDoor )
						g_pDisplay->Blt(m_minimap_x+53, m_minimap_y+MINI_MAP_HEIGHT-22, m_pInDoor);
					if( m_pOutDoor )
						g_pDisplay->Blt(m_minimap_x+110, m_minimap_y+18/*MINI_MAP_HEIGHT-20/, m_pOutDoor);
				}
		*/
		/*		// 픸E?맵에서 엉鸞햨E알컖E시픙툈E이 주석 안 껄로...
				xpos = m_minimap_x + aa;
				ypos = m_minimap_y + bb;

				if( m_pPosPoint )
					m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
				Show_PosPoint_Partys();

				//-- IDS_MAP_DEKALEN : 데카렌%d층
				sprintf(strLoc, G_STRING(IDS_MAP_DEKALEN), m_iCurDeckaLen);//1223
		*/

		break;

	// 신 던젼 (GetWordNum = 2)
	case WORLD_DECKADUM:
		if(m_iCurDeckadum == 0 && m_pDeckadum_1)
			g_pDisplay->Blt(mx, my, m_pDeckadum_1);
		else if(m_iCurDeckadum == 1 && m_pDeckadum_2)
			g_pDisplay->Blt(mx, my, m_pDeckadum_2);
		else if(m_iCurDeckadum == 2 && m_pDeckadum_3)
			g_pDisplay->Blt(mx, my, m_pDeckadum_3);
		else if(m_iCurDeckadum == 3 && m_pDeckadum_4)
			g_pDisplay->Blt(mx, my, m_pDeckadum_4);

		if( m_pInDoor )
			g_pDisplay->Blt(m_pInDoor->Xpos, m_pInDoor->Ypos, m_pInDoor);

		if(m_pOutDoor && m_iCurDeckadum !=3) // 4층에는 문이 하나다.!!
			g_pDisplay->Blt(m_pOutDoor->Xpos, m_pOutDoor->Ypos, m_pOutDoor);

		Show_PosPoint_InDeckadum();
		//-- IDS_MAP_DEKADUNE : 데카둔%d층
		sprintf(strLoc, G_STRING(IDS_MAP_DEKADUNE), m_iCurDeckadum+1);//1223
		break;

	// 신 필탛E(스타트 필탛EGetWordNum = 4)
	case WORLD_LAGRAMIA:

		if(	pCMyApp->IsAcropolis() == 1 ) ///튜토리푳E섬지역 미니맵 예외처리.
		{
			if( m_pTutorialIsland )
				g_pDisplay->Blt(mx, my, m_pTutorialIsland);

			SetRect(&rect,500, 12200,3200 ,9300);
			CalPos(rect, m_minimap_x-25, m_minimap_y-25, 315, 315);

			break;
		}

		if(m_iCurVillage != VILLAGE_NONE)
		{
			if( m_pLagramia_detail )
				g_pDisplay->Blt(mx, my, m_pLagramia_detail);
			Show_PosPoint_InVillage();
		}
		else
		{
			if( m_pLagramia )
				g_pDisplay->Blt(mx, my, m_pLagramia);

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();
		}
		//-- IDS_MAP_LAGRAMIA : 라그라미아
		sprintf(strLoc, G_STRING(IDS_MAP_LAGRAMIA));//1223
		break;

		case WORLD_ROYALE:
		{
			if( m_Royale )
				g_pDisplay->Blt(mx, my, m_Royale);

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();
		}
			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING4558));
			break;

	// 샤일론 필탛E
	case WORLD_GUILD:
		//========================================================================
		// nate 2004 - 9
		// GuildHunt
		if( pCMyApp->IsGuildHuntArea( g_pRoh->m_wx, g_pRoh->m_wz ) )
		{
			if( m_pShilonGuildHuntMap )
				g_pDisplay->Blt(mx, my, m_pShilonGuildHuntMap);
			Show_PosPoint_InGuildHuntArea( GUILDHUNT_SHILON );
			break;
		}
		//========================================================================
		if(m_iCurVillage == 2)
		{
			if( m_pShailon_detail )
				g_pDisplay->Blt(mx, my, m_pShailon_detail);
			Show_PosPoint_InVillage();
			return;
		}
		if(!m_bShowGWarMap)
		{
			if( m_pShilon )
				g_pDisplay->Blt(mx, my, m_pShilon);
			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			m_GWarBtn.Draw();
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();
		}
		else
		{
			if( m_pGWar )
				g_pDisplay->Blt(mx, my, m_pGWar);
			Show_PosPoint_InGWar();
		}

		//-- IDS_GATE_SHAILON : 샤일론 필탛E
		sprintf(strLoc, G_STRING(IDS_GATE_SHAILON));//1223
		break;

	case WORLD_DUNGEON_S:
		if(m_iSeaRoost == 0)
		{
			if( m_pSeaRoost_first )
				g_pDisplay->Blt(m_pSeaRoost_first->Xpos, m_pSeaRoost_first->Ypos, m_pSeaRoost_first);
			//-- IDS_MAP_ZOO : 시루스트(프로그매엉洑 동물퓖E
			sprintf(strLoc, G_STRING(IDS_MAP_ZOO));//1223
		}
		else if(m_iSeaRoost == 1)
		{
			if( m_pSeaRoost_second)
				g_pDisplay->Blt(m_pSeaRoost_second->Xpos, m_pSeaRoost_second->Ypos, m_pSeaRoost_second);
			//-- IDS_MAP_VILL : 시루스트(언큱E랜탛E틒E
			sprintf(strLoc, G_STRING(IDS_MAP_VILL));//1223
		}
		else if(m_iSeaRoost == 2)
		{
			if( m_pSeaRoost_third )
				g_pDisplay->Blt(m_pSeaRoost_third->Xpos, m_pSeaRoost_third->Ypos, m_pSeaRoost_third);
			//-- IDS_MAP_ZONE : 시루스트(타임리프트의 존)
			sprintf(strLoc, G_STRING(IDS_MAP_ZONE));//1223
		}
		else if(m_iSeaRoost == 3)
		{
			if( m_pSeaRoost_forth )
				g_pDisplay->Blt(m_pSeaRoost_forth->Xpos, m_pSeaRoost_forth->Ypos, m_pSeaRoost_forth);
			//-- IDS_MAP_SEAROOST4 : 시루스트(자이언트 배럭)
			sprintf(strLoc, G_STRING(IDS_MAP_SEAROOST4));//1223
		}

		if( m_iSeaRoost != 3 )
		{
			if( m_pInDoor )		g_pDisplay->Blt(m_pInDoor->Xpos, m_pInDoor->Ypos, m_pInDoor);
			if( m_pOutDoor )	g_pDisplay->Blt(m_pOutDoor->Xpos, m_pOutDoor->Ypos, m_pOutDoor);
		}

		Show_PosPoint_InSeaRoost();

		break;

	case WORLD_SKY:

		if( !m_pParos )
			break;

		g_pDisplay->Blt(mx, my, m_pParos);
		RECT	rc;
		SetRect(&rc,0, 12750, 12750, 0);
		CalPos(rc, mx, my+10, 242, 256-10);

		//-- IDS_MAP_PHAROS : 천공의성
		sprintf(strLoc, G_STRING(IDS_MAP_PHAROS));//1223
		break;

	// 신 필탛E(스타트 필탛EGetWordNum = 4)
	case WORLD_WHITEHORN:
		//========================================================================
		// nate 2004 - 9
		// GuildHunt
		if( g_pRoh && pCMyApp->IsGuildHuntArea( g_pRoh->m_wx, g_pRoh->m_wz ) )
		{
			if( m_pWhiteHornGuildhuntMap )
				g_pDisplay->Blt(mx, my, m_pWhiteHornGuildhuntMap);
			Show_PosPoint_InGuildHuntArea( GUILDHUNT_WHITEHORN );
			break;
		}
		//========================================================================
		if( m_pWhiteHorn )	g_pDisplay->Blt(mx, my, m_pWhiteHorn );

		xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
		ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( m_pPosPoint )	m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		Show_PosPoint_Partys();

		//-- IDS_GATE_WHITEHORN : 그레이트 화이트 혼
		sprintf(strLoc, G_STRING(IDS_GATE_WHITEHORN));//1223
		break;
	//wan:2004-11
	//Whitehorn Dungeon
	case WORLD_DUNGEON4:
		if( m_pWhiteHornDungeon )
			g_pDisplay->Blt( mx, my, m_pWhiteHornDungeon );
		xpos = m_minimap_x + (int)( (g_pRoh->m_wx / gMapExtention-1) * MINI_MAP_WIDTH / gMapWidth *1.65 + 8);
		ypos = m_minimap_y + (int)( (gMapHeight - (g_pRoh->m_wz / gMapExtention - 1)*1.6) * MINI_MAP_HEIGHT / gMapHeight -7);

		if( m_pPosPoint )
			m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		Show_PosPoint_Partys();
		//-- IDS_DUNGEON4 : 제누스 레버너티
		sprintf(strLoc, G_STRING( IDS_DUNGEON4 ));
		break;
	// nate 2005-03-21
	// Matrix : Indication
	case WORLD_MATRIX:
		if( m_pMatrix )
			g_pDisplay->Blt( mx, my, m_pMatrix );
		// nate 2005-03-31
		// 메트릭스 주둔지에 길탛E마크 출력
		Show_PosPOint_StoneState();

		xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
		ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( m_pPosPoint )
			m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);

		Show_PosPoint_Partys();

		// IDS_MATRIX : 메트릭스
		sprintf(strLoc, G_STRING( IDS_MATRIX ));
		break;

	case WORLD_BOSSRAID_BULKAN:
		if( m_pBossRaidBulkan )
			g_pDisplay->Blt( mx, my, m_pBossRaidBulkan );

		xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
		ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( !pCMyApp->IsAcropolis() )
		{
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		}
		Show_PosPoint_Partys();

		// IDS_WORLD_13	: 듀턿E
		sprintf( strLoc, G_STRING(IDS_WORLD_13));
		break;

	case WORLD_BOSSRAID_KAILIPTON:
		if( m_pBossRaidKailipton )
			g_pDisplay->Blt( mx, my, m_pBossRaidKailipton );

		xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
		ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( !pCMyApp->IsAcropolis() )
		{
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		}
		Show_PosPoint_Partys();

		// IDS_WORLD_14	: 발카리야
		sprintf( strLoc, G_STRING(IDS_WORLD_14));
		break;

	case WORLD_BOSSRAID_AIDIA:
		if( m_pBossRaidAidia)
			g_pDisplay->Blt( mx, my, m_pBossRaidAidia );

		xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
		ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( !pCMyApp->IsAcropolis() )
		{
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		}
		Show_PosPoint_Partys();

		// IDS_WORLD_15	: 바르샤
		sprintf( strLoc, G_STRING(IDS_WORLD_15));
		break;

	case WORLD_BOSSRAID_HUMAN:
		if( m_pBossRaidHuman )
			g_pDisplay->Blt( mx, my, m_pBossRaidHuman );

		xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
		ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( !pCMyApp->IsAcropolis() )
		{
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		}
		Show_PosPoint_Partys();

		// IDS_WORLD_16	: 퀘이햨E
		sprintf( strLoc, G_STRING(IDS_WORLD_16));
		break;

	case WORLD_BOSSRAID_HYBRIDER:
		if( m_pBossRaidHybrider )
			g_pDisplay->Blt( mx, my, m_pBossRaidHybrider );

		xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
		ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( !pCMyApp->IsAcropolis() )
		{
			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
		}
		Show_PosPoint_Partys();

		// IDS_WORLD_17	: 베엉斛리탛E
		sprintf( strLoc, G_STRING(IDS_WORLD_17));
		break;

	case WORLD_BOSSRAID_FINALBOSS:

		if( pCMyApp->IsFianlBossRoomArea(0,0) )
		{
			if( m_pBossRaidFinalboss_Room )
				g_pDisplay->Blt( mx, my, m_pBossRaidFinalboss_Room );

			RECT rect;
			SetRect(&rect, 9600, 6540, 11400 ,4760);
			CalPos(rect, m_minimap_x-27, m_minimap_y-30, 295, 295);

			// IDS_WORLD_18_CASTLE : 마르보덴 성 입구
			sprintf( strLoc, G_STRING(IDS_WORLD_18_CASTLE));
		}
		else
		{
			if( m_pBossRaidFinalboss )
				g_pDisplay->Blt( mx, my, m_pBossRaidFinalboss );

			xpos = m_minimap_x + ((int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth ) + 17;
			ypos = m_minimap_y + ((gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight ) + 10;

			if( !pCMyApp->IsAcropolis() )
			{
				if( m_pPosPoint )
					m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			}

			Show_PosPoint_Partys();

			// IDS_WORLD_18	: 마르보덴
			sprintf( strLoc, G_STRING(IDS_WORLD_18)); //G_STRING(IDS_MAP_DMITRON));
		}
		break;

	case WORLD_MATRIX_2009:
		if( m_pMoebius )
			g_pDisplay->Blt( mx, my, m_pMoebius );
		// nate 2005-03-31
		// 메트릭스 주둔지에 길탛E마크 출력
		Show_PosPOint_StoneState_2009();
		DrawMatrixStoneInfo();

		xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
		ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

		if( m_pPosPoint )
			m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);

		Show_PosPoint_Partys();

		// IDS_Mobius : 메트릭스
		sprintf(strLoc, G_STRING( IDS_WORLD_10 ));
		break;

	case WORLD_DISPOSAL:
		{
			if( m_pDisposal )
				g_pDisplay->Blt( mx, my, m_pDisposal );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING1829));
		}
		break;

	case WORLD_VIDBLINE:
		{
			if( m_pVidBline )
				g_pDisplay->Blt( mx, my, m_pVidBline );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			strcpy( strLoc, (char*)G_STRING(IDS_LHSTRING1830));

		}
		break;

	case WORLD_FORLORN_S2:
		{		
			if( m_zone_22 )
				g_pDisplay->Blt( mx, my, m_zone_22 );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth) * 2;
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight ) * 2;

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING1892));
		}
		break;

	case WORLD_FORLORN_S3:
		{
			if( m_zone_23 )
				g_pDisplay->Blt( mx, my, m_zone_23 );

			xpos = m_minimap_x + ( ((int)g_pRoh->m_wx - (12800 / 2))/ gMapExtention * MINI_MAP_WIDTH / gMapWidth) * 2;
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight ) * 2;

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING1896));
		}
		break;

	case WORLD_FORLORN_S4:    //arnold update
		{		
			if( m_zone_24 )
				g_pDisplay->Blt( mx, my, m_zone_24 );
			
			xpos = m_minimap_x + ( ((int)g_pRoh->m_wx - (12560 / 2))/ gMapExtention * MINI_MAP_WIDTH / gMapWidth) * 2;
			ypos = m_minimap_y + ( ((123) - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight ) * 2;

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING1897)); 
		}
		
		break;

	case WORLD_HIGHTLAND:
		{
			if( m_Hightland )
				g_pDisplay->Blt( mx, my, m_Hightland );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			strcpy( strLoc, (char*)G_STRING(IDS_LHSTRING4555));

		}
		break;

	case WORLD_TRITIAN:
		{
			if( m_Tritian )
				g_pDisplay->Blt( mx, my, m_Tritian );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING4556));
		}
		break;

	case WORLD_SPACE:
		{
			if( m_Space )
				g_pDisplay->Blt( mx, my, m_Space );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING4557));
		}
		break;
	case WORLD_DUN910:
		{
			if( m_dun910 )
				g_pDisplay->Blt( mx, my, m_dun910 );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING4662));
		}
		break;
	case WORLD_MAP930:
		{
			if( m_zone930 )
				g_pDisplay->Blt( mx, my, m_zone930 );

			xpos = m_minimap_x + ( (int)g_pRoh->m_wx / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
			ypos = m_minimap_y + ( (gMapHeight - ((int)g_pRoh->m_wz / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

			if( m_pPosPoint )
				m_pPosPoint->RenderOj(xpos, ypos, g_pRoh->m_dir);
			Show_PosPoint_Partys();

			sprintf(strLoc, (char*)G_STRING(IDS_LHSTRING4663));
		}
		break;

	}
	m_LocBox.SetString(strLoc,RGB(231, 203, 102),RGB(0,0,0));
	Show_Pos_Text(m_curpostext_x, m_curpostext_y);
}

void CControlMap::Show_Dmitron_Battle_Status()
{
	if( !g_pNk2DFrame )
		return;

	if(m_iDmitronPopup == -1)
		return;

	D3DVIEWPORT7 vpOld, vpNew;
	// 현재의 뷰포트를 저장하컖E새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);


	DWORD dwState[8];

	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwState[0] );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwState[1]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[2]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[3]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[4]);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwState[5] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwState[6] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwState[7] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	float m_fAlpha = 0.5f;
	D3DTLVERTEX vertex[4];

	vertex[0] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.left, m_rcDmitronPopup.top, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	vertex[1] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.right, m_rcDmitronPopup.top, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	vertex[2] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.left, m_rcDmitronPopup.bottom, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	vertex[3] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.right, m_rcDmitronPopup.bottom, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	vertex[0].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);
	vertex[1].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);
	vertex[2].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);
	vertex[3].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);

	GET_D3DDEVICE()->SetTexture(0,NULL);
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vertex, 4, 0 );

	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwState[0] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwState[1]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[2]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[3]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[4]);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwState[5] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwState[6] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwState[7] );

	m_DmitronPopupText.SetPos(m_rcDmitronPopup.left+4, m_rcDmitronPopup.top+4);
	m_DmitronPopupText.Draw();
}

// 파티원들의 위치를 보여준다.
void CControlMap::Show_PosPoint_Partys()
{
	if( !g_pNk2DFrame )
		return;

	int x, z;
	int xpos, ypos;

	if(!GET_D3DDEVICE())
		return;

	D3DVIEWPORT7 vpOld, vpNew;
	// 현재의 뷰포트를 저장하컖E새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);


	DWORD dwState[8];

	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwState[0] );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwState[1]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[2]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[3]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[4]);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwState[5] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwState[6] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwState[7] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{

		if(g_pRoh && g_pRoh->m_bIsParty
				&& g_pNk2DFrame->GetControlParty()
				&& g_pNk2DFrame->GetControlParty()->m_MemberCount >1
				&& g_pRoh->m_bRecivedPartysInfo)
		{
			for(int i=0; i<g_pRoh->m_iMaxPartysNum; i++)
			{
				if(g_pRoh->m_iPartysListId[i] <0)
					goto _END;

				x = g_pRoh->m_iPartysListXpos[i];
				z = g_pRoh->m_iPartysListYpos[i];

				if( pCMyApp->IsAcropolis(x, z) )
					goto _END;

				int aa = ( (int)x / gMapExtention * MINI_MAP_WIDTH / gMapWidth);
				int bb = ( (gMapHeight - ((int)z/ gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

				if(m_iCurWorld == WORLD_NAOROSMIA)
				{
					// 구던젼

					aa = (int)x / gMapExtention * MINI_MAP_WIDTH / gMapWidth;
					bb = ( (gMapHeight - ((int)z / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

					int cur_floor;
					if( aa >= 125 )
						cur_floor = 2;
					else
						cur_floor = 1;

					if(m_iCurDeckaLen != cur_floor) // 같은 층에 있햨E않는 파티원은 보이햨E않는다.
						goto _END;

				}
				else if(m_iCurWorld == WORLD_DUNGEON4)
				{
					//wan:2005-01 화이트혼 던젼
					aa = (int)( ((int)x / gMapExtention-1) * MINI_MAP_WIDTH / gMapWidth *1.65 + 8);
					bb = (int)( (gMapHeight - ((int)z / gMapExtention - 1)*1.6) * MINI_MAP_HEIGHT / gMapHeight -7);
					xpos = m_minimap_x + aa	;
					ypos = m_minimap_y + bb;
					m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_MYPARTS2);
				}
				else if( m_iCurWorld == WORLD_BOSSRAID_FINALBOSS )
				{
					if( FALSE == pCMyApp->IsFianlBossRoomArea(0,0) &&
							pCMyApp->IsFianlBossRoomArea(x,z) )
					{
						int start_x = m_minimap_x-27;
						int start_y = m_minimap_y-30;

						xpos = start_x + 154;
						ypos = start_y + 83;

						m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_MYPARTYS);
					}
					else
					{
						xpos = m_minimap_x + aa + 18;
						ypos = m_minimap_y + bb + 11;
						m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_MYPARTYS);
					}
				}
				else if( m_iCurWorld == WORLD_FORLORN_S2 )
				{
					xpos = m_minimap_x + (aa<<1);
					ypos = m_minimap_y + (bb<<1);
					m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_MYPARTYS);
				}
				else if( m_iCurWorld == WORLD_FORLORN_S3 )
				{					
					aa = ( ((int)x - (12800 / 2)) / gMapExtention * MINI_MAP_WIDTH / gMapWidth) * 2;
					bb = ( (gMapHeight - ((int)z/ gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight ) * 2;

					xpos = m_minimap_x + aa;
					ypos = m_minimap_y + bb;
					m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_MYPARTYS);
				}
				else if( m_iCurWorld == WORLD_FORLORN_S4 )
				{					
					xpos = m_minimap_x + (aa<<1);
					ypos = m_minimap_y + (bb<<1);
					m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_MYPARTYS);
				}
				else
				{
					xpos = m_minimap_x + aa;
					ypos = m_minimap_y + bb;
					m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_MYPARTYS);
				}
			}
		}
//=====================================================================================
		// nate 2004 - 5
		// 크리스탈 소유자 좌표 표시
		if( pCMyApp->m_pUIMgr
				&& pCMyApp->m_pUIMgr->m_bOnBattle
				&& pCMyApp->GetCurWorld() == WORLD_FIELD  )
		{
			if( m_nCristalOwnerX != 0 && m_nCristalOwnerZ != 0 )
			{
				int aa = ( (int)m_nCristalOwnerX / gMapExtention * MINI_MAP_WIDTH / gMapWidth );
				int bb = ( (gMapHeight - (( int )m_nCristalOwnerZ / gMapExtention - 1 ) ) * MINI_MAP_HEIGHT / gMapHeight );

				// 구던젼
				if( m_iCurWorld == WORLD_NAOROSMIA )
				{
					aa = (int)m_nCristalOwnerX / gMapExtention * MINI_MAP_WIDTH / gMapWidth;
					bb = ( (gMapHeight - ((int)m_nCristalOwnerZ / gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );

					int cur_floor;
					if( aa >= 125 )
						cur_floor = 2;
					else
						cur_floor = 1;

					if(m_iCurDeckaLen != cur_floor) // 같은 층에 있햨E않는 파티원은 보이햨E않는다.
						goto _END;

				}
				xpos = m_minimap_x + aa	;
				ypos = m_minimap_y + bb;


				m_pMyPartys->RenderOj_Fade(xpos, ypos, 0, EFF_CRISTAL_OWNER );
			}
		}

		// nate 2005-03-21
		// Matrix : Indication
		if( m_bIndication_Point )
		{
			if( m_bIndication_Point == 2 )
			{
				DWORD dwCurTime = timeGetTime();

				if( dwCurTime > m_dwTimeIndication + 60000 )
					m_bIndication_Point = 0;
			}

			int nX = m_nCurClick_X - m_minimap_x;
			int nY = m_nCurClick_Y - m_minimap_y;

			if( m_pArrow && nX > 0 && nY > 0 && nX < 240 && nY < 250 )
			{
				m_nAniframe += ((timeGetTime() - m_dwAniTime)/ 15);

				if( m_nAniframe > 20 )
					m_nAniframe  = 0;

				g_pDisplay->Blt( m_nCurClick_X - 10
								 , m_nCurClick_Y - 40 + m_nAniframe
								 , m_nCurClick_X - 10 + m_pArrow->GetWidth()
								 , m_nCurClick_Y - 20 + m_nAniframe, m_pArrow );

				m_dwAniTime = timeGetTime();
			}

			Show_PosPoint_Indication();
		}
//=====================================================================================
_END:
		GET_D3DDEVICE()->EndScene();
	}
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwState[0] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwState[1]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[2]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[3]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[4]);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwState[5] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwState[6] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwState[7] );
}


//------------------------------------------------------------------------
// name : Show_Pos_Text
// desc : 케릭터의 좌프를 텍스트로 보여준다
//------------------------------------------------------------------------
void CControlMap::Show_Pos_Text(int pos_x, int pos_z)
{
	float x = 0.0f, z = 0.0f;
	char strPos[20] = "";

	x = g_pRoh->m_wx;
	z = g_pRoh->m_wz;

	int aa = (int) (x/gMapExtention);
	int bb = (int) (gMapHeight-z/gMapExtention);

	if(aa >=133 && pCMyApp->GetCurWorld() == 1)//0401 aa>133
		aa = aa -133;

	if(pCMyApp->GetCurWorld() == 5)  //샤일론 던픸E
	{
		if(m_iSeaRoost==1/*aa > 55 && bb <=72*/) //2층
			aa = aa - 55;
		else if(m_iSeaRoost==2/*aa >98 && bb >=73*/)  //3층
		{
			aa = aa - 98;
			bb = bb - 73;
		}
	}

	if(pCMyApp->GetCurWorld() == 2)  //데카둔
	{
		if(m_iCurDeckadum==0)
			bb = bb - 128;
		else if(m_iCurDeckadum==2)
		{
			aa = aa - 128;
			bb = bb - 128;
		}
		else if(m_iCurDeckadum==3)
			aa = aa -(128+64);
	}

	int CurMapPosX, CurMapPosZ;

	if( pCMyApp->GetCurWorld() == 1 || pCMyApp->GetCurWorld() == 2 || pCMyApp->GetCurWorld() == 5 )
	{
		CurMapPosX = (x/(gMapExtention/4));
		CurMapPosZ = (int)(gMapHeight*4-(z/(gMapExtention/4)));
	}
	else
	{
		CurMapPosX = (x/(gMapExtention/5));
		CurMapPosZ = (int)(gMapHeight*5-(z/(gMapExtention/5)));
	}
	m_iChaRoomPosX = aa;
	m_iChaRoomPosZ = bb;
	if( pCMyApp->GetCurWorld() == 1 || pCMyApp->GetCurWorld() == 2 || pCMyApp->GetCurWorld() == 5 )
	{
		sprintf(strPos, "[%d, %d]", CurMapPosX, CurMapPosZ);
	}
	else
	{
		sprintf(strPos, "[%d, %d]", CurMapPosX, CurMapPosZ);
	}

	m_PosBox.SetString(strPos);
// 	m_PosBox.SetPos(m_pBack->Xpos+116, m_pBack->Ypos+390);
// 	m_LocBox.SetPos(m_pBack->Xpos+75, m_pBack->Ypos+66);

	m_PosBox.Draw(m_pBack->Xpos+116, m_pBack->Ypos+390 , 127);
	m_LocBox.Draw(m_pBack->Xpos+75, m_pBack->Ypos+66 , 127);
}

//------------------------------------------------------------------------
// name : Show_PosPoint_InGWar
// desc : 샤일론 필드의 길드픸E봉構嚥갋처리
//------------------------------------------------------------------------
void CControlMap::Show_PosPoint_InGWar()
{
	if( !g_pRoh || !m_pGWar )
		return;
	RECT	rc;
	int x = g_pRoh->m_wx;
	int y = g_pRoh->m_wz;

	if(x > 7432 && x < 8014 && y > 2280 && y< 3060)
	{
		SetRect(&rc,7432, 3060, 8014,2280 );
		CalPos(rc, m_pGWar->Xpos+57, m_pGWar->Ypos+20, 149, 214); // <--- 맵 그림상의 시작되는 영역 start  x, y , w, h
		CalPos_For_Flag(rc, m_pGWar->Xpos+57, m_pGWar->Ypos+20, 149, 214);
	}
}

//------------------------------------------------------------------------
// name : Show_PosPoint_InDeckadum
// desc : 신던픸E【? 4개(1~4층)의 맵 좌표영역을 확인하컖E현제 플레이엉瞼 서있는 위치로
//        뫈署에 서있는햨E판단한다
//------------------------------------------------------------------------
void CControlMap::Show_PosPoint_InDeckadum()
{
	if( !g_pRoh )
		return;

	RECT	rc;
	if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 80, 2480, 2500, 80))
	{
		SetRect(&rc,80, 2480, 2500, 80);
		if( m_pDeckadum_1 )		CalPos(rc, m_pDeckadum_1->Xpos, m_pDeckadum_1->Ypos, 242, 256);
		Get_Door_Pos(rc, 202, 580, 1801,2282);
		m_iCurDeckadum = 0;
	}
	else if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 60,5080,2520,2660))
	{
		SetRect(&rc, 60,5080,2520,2660);
		if( m_pDeckadum_2 )		CalPos(rc, m_pDeckadum_2->Xpos, m_pDeckadum_2->Ypos, 242, 256);
		Get_Door_Pos(rc, 1829, 4953, 1032, 3347);
		m_iCurDeckadum = 1;
	}
	else if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 2720, 2480,5040, 60))
	{
		SetRect(&rc, 2720, 2480,5040, 60);
		if( m_pDeckadum_3 )		CalPos(rc, m_pDeckadum_3->Xpos, m_pDeckadum_3->Ypos, 242, 256);
		Get_Door_Pos(rc, 3116, 1751, 4905, 233);
		m_iCurDeckadum = 2;
	}
	else if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 3780, 5080, 5080, 3780))
	{
		SetRect(&rc, 3780, 5080, 5080, 3780);
		if( m_pDeckadum_4 )		CalPos(rc, m_pDeckadum_4->Xpos, m_pDeckadum_4->Ypos, 242, 256);
		Get_Door_Pos(rc, 4767, 3907, -1, -1);
		m_iCurDeckadum = 3;
	}
	else
		m_iCurDeckadum = -1;
}

//1226 시루스트 던픸E?좌표 표시...
void CControlMap::Show_PosPoint_InSeaRoost()
{
	if( !g_pRoh )
		return;

	RECT	rc;

	if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 20, 3660,1960,20) || pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 20, 5120,1100,3660))
	{
		SetRect(&rc,20, 5120,1960,20);
		if( m_pSeaRoost_first )
			CalPos(rc, m_pSeaRoost_first->Xpos, m_pSeaRoost_first->Ypos, 93, 256);
		m_iSeaRoost = 0;
		Get_Door_Pos_SeaRoost(rc, 272,106,1000,4890);
	}
	else if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 1100,5120,5120,3660))
	{
		SetRect(&rc, 1100,5120,5120,3660);
		if( m_pSeaRoost_second )
			CalPos(rc, m_pSeaRoost_second->Xpos, m_pSeaRoost_second->Ypos, 194, 69);
		m_iSeaRoost = 1;
		Get_Door_Pos_SeaRoost(rc,1150, 5100, 2190, 3662);

	}
	else if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 1960,3660,4040,2220))
	{
		SetRect(&rc, 1960,3660,4040,2220);
		if( m_pSeaRoost_third )
			CalPos(rc, m_pSeaRoost_third->Xpos, m_pSeaRoost_third->Ypos, 102, 73);
		m_iSeaRoost = 2;
		Get_Door_Pos_SeaRoost(rc, 2081, 3655, 3352, 3170);

	}
	else if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 2050,2150,4100,850))
	{
		SetRect(&rc, 2050,2150,4100,850);
		if( m_pSeaRoost_forth )
			CalPos(rc, m_pSeaRoost_forth->Xpos, m_pSeaRoost_forth->Ypos, 94, 59);
		m_iSeaRoost = 3;
	}
	else
		m_iSeaRoost = -1;
}

//---------------------------------------------------------------------
// name : Show_PosPoint_InVillage
// desc : 마을안에서의 케릭위치 커서를 보여준다(구 필탛E
//---------------------------------------------------------------------
void CControlMap::Show_PosPoint_InVillage()
{
	RECT villageRc;

	RECT	rc;
	int x = g_pRoh->m_wx;
	int y = g_pRoh->m_wz;

	switch(m_iCurVillage)
	{
	case 1:
		if(x > 6099 && x < 8564 && y > 3608 && y< 57495)
		{
			SetRect(&rc,6099, 5749, 8564,3608 );
			if( m_pLagramia_detail )
				CalPos(rc, m_pLagramia_detail->Xpos+8, m_pLagramia_detail->Ypos+44, 225, 195); // <--- 맵 그림상의 시작되는 영역 start  x, y , w, h
		}
		break;

	case 2:// 샤일론 마을 상세지도
		if(x > 6907 && x < 7903 && y > 11262 && y< 11948)
		{
			SetRect(&rc,6907, 11948,7903, 11262 );
			if( m_pShailon_detail )
				CalPos(rc, m_pShailon_detail->Xpos+52, m_pShailon_detail->Ypos+82, 147, 101); // <--- 맵 그림상의 시작되는 영역 start  x, y , w, h
		}
		break;

	case 3:// 로스트레름
		if (pCMyApp->IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 5558, 7143, 7574, 5464))
		{
			SetRect(&villageRc, 5558, 7143, 7574, 5464);
			CalPos(villageRc, m_minimap_x+37, m_minimap_y+46, 168, 139);
		}
		break;

	default:
		break;
	}
}

void CControlMap::CalPos_For_Flag(RECT rc, int start_x, int start_y, int width, int height)
{
	int cx = rc.right - rc.left;
	int cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..

	CNkCharacter *pCha;
	LHCharacterMap_it pr;
	for (pr = pCMyApp->m_pUIMgr->m_character_map.begin(); pr != pCMyApp->m_pUIMgr->m_character_map.end(); pr++)
	{
		pCha = (*pr).second;
		if (pCha->m_nTeamMemGrade == TEAM_BATTLE_AFLAG)
		{
			int x = ((int)pCha->m_wx - rc.left);
			int y = (rc.top - (int)pCha->m_wz);

			m_MyPosInVillage_x = start_x+(x * width) / cx;
			m_MyPosInVillage_y = start_y+(y * height) / cy;

			m_pFlag_A->RenderOj(m_MyPosInVillage_x, m_MyPosInVillage_y, pCha->m_dir, EFF_GWAR_FLAG_A);
			break;
		}

		if (pCha->m_nTeamMemGrade == TEAM_BATTLE_BFLAG)
		{
			int x = ((int)pCha->m_wx - rc.left);
			int y = (rc.top - (int)pCha->m_wz);

			m_MyPosInVillage_x = start_x+(x * width) / cx;
			m_MyPosInVillage_y = start_y+(y * height) / cy;
			m_pFlag_B->RenderOj(m_MyPosInVillage_x, m_MyPosInVillage_y, pCha->m_dir, EFF_GWAR_FLAG_B);
			break;
		}
	}
}


void CControlMap::CalPos(RECT rc, int start_x, int start_y, int width, int height)
{
	int cx = rc.right - rc.left;
	int cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
	int x = ((int)g_pRoh->m_wx - rc.left);
	int y = (rc.top - (int)g_pRoh->m_wz);

	m_MyPosInVillage_x = start_x+(x * width) / cx;
	m_MyPosInVillage_y = start_y+(y * height) / cy;

	if( m_pPosPoint )
		m_pPosPoint->RenderOj(m_MyPosInVillage_x/*+7*/, m_MyPosInVillage_y, g_pRoh->m_dir);//1227 +7은 오차보정.

	if( m_iCurWorld == 1 && m_iCurDeckaLen != 0 ) // 데카렌 일때만...///인큱E 시 추가
		RenderLimitSight( m_MyPosInVillage_x, m_MyPosInVillage_y ); // 자콅E주변 미니맵만 보이게 해주는 효컖E

	CalPos_Partys(rc, start_x, start_y, width, height);
}


void CControlMap::CalPos_Partys(RECT rc, int start_x, int start_y, int width, int height)
{
	if( !g_pNk2DFrame )
		return;

	int cha_x, cha_z;
	int out_x, out_z;
	int icur_floor = -1;
//==============================================================================
	// nate 2004 - 5
	// 크리스탈 소유자 좌표 출력
	int cx = rc.right - rc.left;
	int cy = rc.top - rc.bottom;
//==============================================================================

	if(!GET_D3DDEVICE())
		return;

	D3DVIEWPORT7 vpOld, vpNew;
	// 현재의 뷰포트를 저장하컖E새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	DWORD dwState[8];
	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwState[0] );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwState[1]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[2]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[3]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[4]);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwState[5] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwState[6] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwState[7] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		if(g_pRoh && g_pRoh->m_bIsParty
				&& g_pNk2DFrame->GetControlParty() && g_pNk2DFrame->GetControlParty()->m_MemberCount >1
				&& g_pRoh->m_bRecivedPartysInfo)
		{
			for(int i=0; i<g_pRoh->m_iMaxPartysNum; i++)
			{
				if(g_pRoh->m_iPartysListId[i] <0)
					goto _END;

				cha_x = g_pRoh->m_iPartysListXpos[i];
				cha_z = g_pRoh->m_iPartysListYpos[i];

				if(m_iCurWorld==WORLD_DECKARDI || m_iCurWorld==WORLD_LAGRAMIA || m_iCurWorld==WORLD_GUILD)
				{
					// 상세지도 켰을때도 보인다.
					if(m_iCurVillage == 1)
					{
						if(!pCMyApp->IsInsideRect(cha_x, cha_z, 6099, 57495, 8564, 3608))
							goto _END;
					}
					else if(m_iCurVillage == 2)
					{
						if(!pCMyApp->IsInsideRect(cha_x, cha_z, 6907, 11948, 7903, 11262))
							goto _END;
					}
					else if(m_iCurVillage == 3)
					{
						if (!pCMyApp->IsInsideRect(cha_x, cha_z, 5558, 7143, 7574, 5464))
							goto _END;
					}
				}

				else if(m_iCurWorld ==WORLD_DECKADUM)// 신던젼
				{
					if (pCMyApp->IsInsideRect(cha_x, cha_z, 80, 2480, 2500, 80))
						icur_floor = 0;
					else if (pCMyApp->IsInsideRect(cha_x, cha_z, 60,5080,2520,2660))
						icur_floor = 1;
					else if (pCMyApp->IsInsideRect(cha_x, cha_z, 2720, 2480,5040, 60))
						icur_floor = 2;
					else if (pCMyApp->IsInsideRect(cha_x, cha_z, 3780, 5080, 5080, 3780))
						icur_floor = 3;

					if(m_iCurDeckadum != icur_floor) // 던젼에서 다른층의 파티원은 보일필퓖E없다.
						goto _END;
				}

				else if(m_iCurWorld ==WORLD_DUNGEON_S) // 시루스트
				{
					if (pCMyApp->IsInsideRect(cha_x, cha_z, 20, 3660,1960,20) || pCMyApp->IsInsideRect(cha_x, cha_z, 20, 5120,1100,3660))
						icur_floor = 0;
					else if (pCMyApp->IsInsideRect(cha_x, cha_z, 1100,5120,5120,3660))
						icur_floor = 1;
					else if (pCMyApp->IsInsideRect(cha_x, cha_z, 1960,3660,4040,2220))
						icur_floor = 2;
					else if (pCMyApp->IsInsideRect(cha_x, cha_z, 2050,2150,4100,850))
						icur_floor = 3;

					if(m_iSeaRoost != icur_floor)
						goto _END;
				}
				else if( m_iCurWorld == WORLD_BOSSRAID_FINALBOSS )
				{
					if( pCMyApp->IsFianlBossRoomArea(0,0) )
					{
						if( pCMyApp->IsFianlBossRoomArea(cha_x, cha_z) )
						{
							RECT rect;
							SetRect(&rect, 9600, 6540, 11400 ,4760);

							int start_x = m_minimap_x-27;
							int start_y = m_minimap_y-30;
							int width	= 295;
							int height	= 295;

							int cx = rect.right - rect.left;
							int cy = rect.top - rect.bottom ; // (주의!!) 좌표가 꺼꾸로..
							int nx = ((int)cha_x - rect.left);
							int ny = (rect.top - (int)cha_z);

							out_x = start_x+(nx * width) / cx;
							out_z = start_y+(ny * height) / cy;

							if( m_pMyPartys )
								m_pMyPartys->RenderOj_Fade(out_x, out_z, 0, EFF_MYPARTYS);
							continue;
						}
						else
						{
							continue;
						}
					}
					else
					{
						if( pCMyApp->IsFianlBossRoomArea(cha_x, cha_z) )
						{
							out_x = m_minimap_x + 127;
							out_z = m_minimap_y + 50;

							if( m_pMyPartys )
								m_pMyPartys->RenderOj_Fade(out_x, out_z, 0, EFF_MYPARTYS);
							continue;
						}
					}
				}

				int x = ((int)cha_x - rc.left);
				int y = (rc.top - (int)cha_z);

				out_x = start_x+(x * width) / cx;
				out_z = start_y+(y * height) / cy;

				if( m_pMyPartys )
					m_pMyPartys->RenderOj_Fade(out_x, out_z, 0, EFF_MYPARTYS);
			}
		}
_END:
//==============================================================================
		// nate 2004 - 5
		// 크리스탈 소유자 좌표 출력
		if( pCMyApp->m_pUIMgr && pCMyApp->m_pUIMgr->m_bOnBattle
				&& pCMyApp->GetCurWorld() == WORLD_FIELD  )
		{
			if( m_nCristalOwnerX != 0 && m_nCristalOwnerZ != 0 )
			{
				int nCristalOwnerX = ( m_nCristalOwnerX - rc.left);
				int nCristalOwnerY = ( rc.top - m_nCristalOwnerZ );
				nCristalOwnerX = start_x + ( nCristalOwnerX * width ) / cx;
				nCristalOwnerY = start_y + ( nCristalOwnerY * height ) / cy;

				m_pMyPartys->RenderOj_Fade(nCristalOwnerX, nCristalOwnerY, 0, EFF_CRISTAL_OWNER );
			}
		}
//==============================================================================
		GET_D3DDEVICE()->EndScene();
	}
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwState[0] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwState[1]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[2]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[3]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[4]);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwState[5] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwState[6] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwState[7] );
}


// 입구 x,y 출구 x,y 순서큱E?입력한다
void CControlMap::Get_Door_Pos(RECT rc, int door_x1, int door_y1, int door_x2, int door_y2)
{
	if( !m_pDeckadum_1 )
		return;

	int cx = rc.right - rc.left;
	int cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
	int x = ((int)door_x1 - rc.left);
	int y = (rc.top - (int)door_y1);
	int x2 = ((int)door_x2 - rc.left);
	int y2 = (rc.top - (int)door_y2);

	if( m_pInDoor )
	{
		// 입,출구 아이콘 이미지의 중앙에 위치하도록 ..
		m_pInDoor->Xpos = m_pDeckadum_1->Xpos-(m_pInDoor->GetWidth()/2)+(x * /*NEW_DG_MAP_WIDTH*/MINI_MAP_WIDTH) / cx;
		m_pInDoor->Ypos = m_pDeckadum_1->Ypos-(m_pInDoor->GetHeight()/2)+(y * /*NEW_DG_MAP_HEIGHT*/MINI_MAP_HEIGHT) / cy;

		if(m_pInDoor->Xpos < m_pDeckadum_1->Xpos)
			m_pInDoor->Xpos = m_pDeckadum_1->Xpos;
		if(m_pInDoor->Xpos+m_pInDoor->GetWidth() > m_pDeckadum_1->Xpos+m_pDeckadum_1->GetWidth())
			m_pInDoor->Xpos = m_pDeckadum_1->Xpos+m_pDeckadum_1->GetWidth()-m_pInDoor->GetWidth();
		if(m_pInDoor->Ypos < m_pDeckadum_1->Ypos)
			m_pInDoor->Ypos = m_pDeckadum_1->Ypos;
		if(m_pInDoor->Ypos+m_pInDoor->GetHeight() > m_pDeckadum_1->Ypos+m_pDeckadum_1->GetHeight())
			m_pInDoor->Ypos = m_pDeckadum_1->Ypos+m_pDeckadum_1->GetHeight()-m_pInDoor->GetHeight();
	}

	if( m_pOutDoor )
	{
		m_pOutDoor->Xpos = m_pDeckadum_1->Xpos-(m_pOutDoor->GetWidth()/2)+(x2 * /*NEW_DG_MAP_WIDTH*/MINI_MAP_WIDTH) / cx;
		m_pOutDoor->Ypos = m_pDeckadum_1->Ypos-(m_pOutDoor->GetWidth()/2)+(y2 * /*NEW_DG_MAP_WIDTH*/MINI_MAP_HEIGHT) / cy;

		if(m_pOutDoor->Xpos < m_pDeckadum_1->Xpos)
			m_pOutDoor->Xpos = m_pDeckadum_1->Xpos;
		if(m_pOutDoor->Xpos+m_pOutDoor->GetWidth() > m_pDeckadum_1->Xpos+m_pDeckadum_1->GetWidth())
			m_pOutDoor->Xpos = m_pDeckadum_1->Xpos+m_pDeckadum_1->GetWidth()-m_pOutDoor->GetWidth();
		if(m_pOutDoor->Ypos < m_pDeckadum_1->Ypos)
			m_pOutDoor->Ypos = m_pDeckadum_1->Ypos;
		if(m_pOutDoor->Ypos+m_pOutDoor->GetHeight() > m_pDeckadum_1->Ypos+m_pDeckadum_1->GetHeight())
			m_pOutDoor->Ypos = m_pDeckadum_1->Ypos+m_pDeckadum_1->GetHeight()-m_pOutDoor->GetHeight();
	}
}
//1227
void CControlMap::Get_Door_Pos_SeaRoost(RECT rc, int door_x1, int door_y1, int door_x2, int door_y2)
{
	int cx = rc.right - rc.left;
	int cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
	int x = ((int)door_x1 - rc.left);
	int y = (rc.top - (int)door_y1);
	int x2 = ((int)door_x2 - rc.left);
	int y2 = (rc.top - (int)door_y2);

	if(m_pSeaRoost_first && m_iSeaRoost==0)
	{
		if( m_pInDoor )
		{
			// 입,출구 아이콘 이미지의 중앙에 위치하도록 ..
			m_pInDoor->Xpos = m_pSeaRoost_first->Xpos-(m_pInDoor->GetWidth()/2)+(x * 87/*NEW_DG_MAP_WIDTH*/) / cx;
			m_pInDoor->Ypos = m_pSeaRoost_first->Ypos-(m_pInDoor->GetHeight()/2)+(y * 256/*NEW_DG_MAP_HEIGHT*/) / cy;

			if(m_pInDoor->Xpos < m_pSeaRoost_first->Xpos)
				m_pInDoor->Xpos = m_pSeaRoost_first->Xpos;
			if(m_pInDoor->Xpos+m_pInDoor->GetWidth() > m_pSeaRoost_first->Xpos+m_pSeaRoost_first->GetWidth())
				m_pInDoor->Xpos = m_pSeaRoost_first->Xpos+m_pSeaRoost_first->GetWidth()-m_pInDoor->GetWidth();
			if(m_pInDoor->Ypos < m_pSeaRoost_first->Ypos)
				m_pInDoor->Ypos = m_pSeaRoost_first->Ypos;
			if(m_pInDoor->Ypos+m_pInDoor->GetHeight() > m_pSeaRoost_first->Ypos+m_pSeaRoost_first->GetHeight())
				m_pInDoor->Ypos = m_pSeaRoost_first->Ypos+m_pSeaRoost_first->GetHeight()-m_pInDoor->GetHeight();
		}

		if( m_pOutDoor )
		{
			m_pOutDoor->Xpos = m_pSeaRoost_first->Xpos-(m_pOutDoor->GetWidth()/2)+(x2 * 87/*NEW_DG_MAP_WIDTH*/) / cx;
			m_pOutDoor->Ypos = m_pSeaRoost_first->Ypos-(m_pOutDoor->GetWidth()/2)+(y2 * 256/*NEW_DG_MAP_WIDTH*/) / cy;

			if(m_pOutDoor->Xpos < m_pSeaRoost_first->Xpos)
				m_pOutDoor->Xpos = m_pSeaRoost_first->Xpos;
			if(m_pOutDoor->Xpos+m_pOutDoor->GetWidth() > m_pSeaRoost_first->Xpos+m_pSeaRoost_first->GetWidth())
				m_pOutDoor->Xpos = m_pSeaRoost_first->Xpos+m_pSeaRoost_first->GetWidth()-m_pOutDoor->GetWidth();
			if(m_pOutDoor->Ypos < m_pSeaRoost_first->Ypos)
				m_pOutDoor->Ypos = m_pSeaRoost_first->Ypos;
			if(m_pOutDoor->Ypos+m_pOutDoor->GetHeight() > m_pSeaRoost_first->Ypos+m_pSeaRoost_first->GetHeight())
				m_pOutDoor->Ypos = m_pSeaRoost_first->Ypos+m_pSeaRoost_first->GetHeight()-m_pOutDoor->GetHeight();
		}
	}
	else if(m_pSeaRoost_second && m_iSeaRoost==1)
	{
		if( m_pInDoor )
		{
			// 입,출구 아이콘 이미지의 중앙에 위치하도록 ..
			m_pInDoor->Xpos = m_pSeaRoost_second->Xpos-(m_pInDoor->GetWidth()/2)+(x * NEW_DG_MAP_WIDTH) / cx;
			m_pInDoor->Ypos = m_pSeaRoost_second->Ypos-(m_pInDoor->GetHeight()/2)+(y * NEW_DG_MAP_HEIGHT) / cy;

			if(m_pInDoor->Xpos < m_pSeaRoost_second->Xpos)
				m_pInDoor->Xpos = m_pSeaRoost_second->Xpos;
			if(m_pInDoor->Xpos+m_pInDoor->GetWidth() > m_pSeaRoost_second->Xpos+m_pSeaRoost_second->GetWidth())
				m_pInDoor->Xpos = m_pSeaRoost_second->Xpos+m_pSeaRoost_second->GetWidth()-m_pInDoor->GetWidth();
			if(m_pInDoor->Ypos < m_pSeaRoost_second->Ypos)
				m_pInDoor->Ypos = m_pSeaRoost_second->Ypos;
			if(m_pInDoor->Ypos+m_pInDoor->GetHeight() > m_pSeaRoost_second->Ypos+m_pSeaRoost_second->GetHeight())
				m_pInDoor->Ypos = m_pSeaRoost_second->Ypos+m_pSeaRoost_second->GetHeight()-m_pInDoor->GetHeight();
		}

		if( m_pOutDoor )
		{
			m_pOutDoor->Xpos = m_pSeaRoost_second->Xpos-(m_pOutDoor->GetWidth()/2)+(x2 * NEW_DG_MAP_WIDTH) / cx;
			m_pOutDoor->Ypos = m_pSeaRoost_second->Ypos-(m_pOutDoor->GetWidth()/2)+(y2 * NEW_DG_MAP_WIDTH) / cx;

			if(m_pOutDoor->Xpos < m_pSeaRoost_second->Xpos)
				m_pOutDoor->Xpos = m_pSeaRoost_second->Xpos;
			if(m_pOutDoor->Xpos+m_pOutDoor->GetWidth() > m_pSeaRoost_second->Xpos+m_pSeaRoost_second->GetWidth())
				m_pOutDoor->Xpos = m_pSeaRoost_second->Xpos+m_pSeaRoost_second->GetWidth()-m_pOutDoor->GetWidth();
			if(m_pOutDoor->Ypos < m_pSeaRoost_second->Ypos)
				m_pOutDoor->Ypos = m_pSeaRoost_second->Ypos;
			if(m_pOutDoor->Ypos+m_pOutDoor->GetHeight() > m_pSeaRoost_second->Ypos+m_pSeaRoost_second->GetHeight())
				m_pOutDoor->Ypos = m_pSeaRoost_second->Ypos+m_pSeaRoost_second->GetHeight()-m_pOutDoor->GetHeight();
		}
	}
	else if(m_pSeaRoost_third && m_iSeaRoost==2)
	{
		if( m_pInDoor )
		{
			// 입,출구 아이콘 이미지의 중앙에 위치하도록 ..
			m_pInDoor->Xpos = m_pSeaRoost_third->Xpos-(m_pInDoor->GetWidth()/2)+(x * NEW_DG_MAP_WIDTH) / cx;
			m_pInDoor->Ypos = m_pSeaRoost_third->Ypos-(m_pInDoor->GetHeight()/2)+(y * NEW_DG_MAP_HEIGHT) / cy;

			if(m_pInDoor->Xpos < m_pSeaRoost_third->Xpos)
				m_pInDoor->Xpos = m_pSeaRoost_third->Xpos;
			if(m_pInDoor->Xpos+m_pInDoor->GetWidth() > m_pSeaRoost_third->Xpos+m_pSeaRoost_third->GetWidth())
				m_pInDoor->Xpos = m_pSeaRoost_third->Xpos+m_pSeaRoost_third->GetWidth()-m_pInDoor->GetWidth();
			if(m_pInDoor->Ypos < m_pSeaRoost_third->Ypos)
				m_pInDoor->Ypos = m_pSeaRoost_third->Ypos;
			if(m_pInDoor->Ypos+m_pInDoor->GetHeight() > m_pSeaRoost_third->Ypos+m_pSeaRoost_third->GetHeight())
				m_pInDoor->Ypos = m_pSeaRoost_third->Ypos+m_pSeaRoost_third->GetHeight()-m_pInDoor->GetHeight();
		}

		if( m_pOutDoor )
		{
			m_pOutDoor->Xpos = m_pSeaRoost_third->Xpos-(m_pOutDoor->GetWidth()/2)+(x2 * NEW_DG_MAP_WIDTH) / cx;
			m_pOutDoor->Ypos = m_pSeaRoost_third->Ypos-(m_pOutDoor->GetWidth()/2)+(y2 * NEW_DG_MAP_WIDTH) / cx;

			if(m_pOutDoor->Xpos < m_pSeaRoost_third->Xpos)
				m_pOutDoor->Xpos = m_pSeaRoost_third->Xpos;
			if(m_pOutDoor->Xpos+m_pOutDoor->GetWidth() > m_pSeaRoost_third->Xpos+m_pSeaRoost_third->GetWidth())
				m_pOutDoor->Xpos = m_pSeaRoost_third->Xpos+m_pSeaRoost_third->GetWidth()-m_pOutDoor->GetWidth();
			if(m_pOutDoor->Ypos < m_pSeaRoost_third->Ypos)
				m_pOutDoor->Ypos = m_pSeaRoost_third->Ypos;
			if(m_pOutDoor->Ypos+m_pOutDoor->GetHeight() > m_pSeaRoost_third->Ypos+m_pSeaRoost_third->GetHeight())
				m_pOutDoor->Ypos = m_pSeaRoost_third->Ypos+m_pSeaRoost_third->GetHeight()-m_pOutDoor->GetHeight();
		}
	}
	else if(m_pSeaRoost_forth && m_iSeaRoost==3)
	{
		if( m_pInDoor )
		{
			// 입,출구 아이콘 이미지의 중앙에 위치하도록 ..
			m_pInDoor->Xpos = m_pSeaRoost_forth->Xpos-(m_pInDoor->GetWidth()/2)+(x * NEW_DG_MAP_WIDTH) / cx;
			m_pInDoor->Ypos = m_pSeaRoost_forth->Ypos-(m_pInDoor->GetHeight()/2)+(y * NEW_DG_MAP_HEIGHT) / cy;

			if(m_pInDoor->Xpos < m_pSeaRoost_forth->Xpos)
				m_pInDoor->Xpos = m_pSeaRoost_forth->Xpos;
			if(m_pInDoor->Xpos+m_pInDoor->GetWidth() > m_pSeaRoost_forth->Xpos+m_pSeaRoost_forth->GetWidth())
				m_pInDoor->Xpos = m_pSeaRoost_forth->Xpos+m_pSeaRoost_forth->GetWidth()-m_pInDoor->GetWidth();
			if(m_pInDoor->Ypos < m_pSeaRoost_forth->Ypos)
				m_pInDoor->Ypos = m_pSeaRoost_forth->Ypos;
			if(m_pInDoor->Ypos+m_pInDoor->GetHeight() > m_pSeaRoost_forth->Ypos+m_pSeaRoost_forth->GetHeight())
				m_pInDoor->Ypos = m_pSeaRoost_forth->Ypos+m_pSeaRoost_forth->GetHeight()-m_pInDoor->GetHeight();
		}

		if( m_pOutDoor )
		{
			m_pOutDoor->Xpos = m_pSeaRoost_forth->Xpos-(m_pOutDoor->GetWidth()/2)+(x2 * NEW_DG_MAP_WIDTH) / cx;
			m_pOutDoor->Ypos = m_pSeaRoost_forth->Ypos-(m_pOutDoor->GetWidth()/2)+(y2 * NEW_DG_MAP_WIDTH) / cx;

			if(m_pOutDoor->Xpos < m_pSeaRoost_forth->Xpos)
				m_pOutDoor->Xpos = m_pSeaRoost_forth->Xpos;
			if(m_pOutDoor->Xpos+m_pOutDoor->GetWidth() > m_pSeaRoost_forth->Xpos+m_pSeaRoost_forth->GetWidth())
				m_pOutDoor->Xpos = m_pSeaRoost_forth->Xpos+m_pSeaRoost_forth->GetWidth()-m_pOutDoor->GetWidth();
			if(m_pOutDoor->Ypos < m_pSeaRoost_forth->Ypos)
				m_pOutDoor->Ypos = m_pSeaRoost_forth->Ypos;
			if(m_pOutDoor->Ypos+m_pOutDoor->GetHeight() > m_pSeaRoost_forth->Ypos+m_pSeaRoost_forth->GetHeight())
				m_pOutDoor->Ypos = m_pSeaRoost_forth->Ypos+m_pSeaRoost_forth->GetHeight()-m_pOutDoor->GetHeight();
		}
	}
}


int CControlMap::IsInside(int x, int y, int event)
{
	//int i;
	int xpos, ypos, width, height;

	// 배경이미햨E영역 안인가??
	if( x >= m_pBack->Xpos
			&& x <= m_pBack->Xpos+ m_pBack->GetWidth()
			&& y >= m_pBack->Ypos
			&& y < m_pBack->Ypos + m_pBack->GetHeight() )
	{
		if(m_iCurWorld == WORLD_DECKARDI)
		{
			if(m_iCurVillage == VILLAGE_NONE)
			{
				xpos = m_minimap_x+100;
				ypos = m_minimap_y+106;
				width = 49;
				height = 45;
				if (x >= xpos  && x<=xpos+width && y >= ypos && y < ypos+height)
				{
					if(event == MOUSE_CLICK)
					{
						m_iCurVillage = 3;
						return 5;
					}
					else if (event == MOUSE_MOVE)
						return 4;
				}
				return 2;
			}
			else
			{
				xpos = m_minimap_x;
				ypos = m_minimap_y;
				width = 242;
				height = 256;

				if (x >= xpos  && x<=xpos+width && y >= ypos && y < ypos+height)
				{
					if(event == MOUSE_MOVE)
						return 3;
					else if(event ==MOUSE_CLICK)
					{
						m_iCurVillage = -1;
						return 6;
					}
				}
			}
		}
		else if(m_iCurWorld  == WORLD_LAGRAMIA)
		{
			///튜토리푳E예외처리
			if( pCMyApp->m_pUIMgr->IsEventScenePlaying() == 5 ) // 튜토리얼중엔 커서를 바꿔주는 처리를 안한다.
			{
				return 0;
			}

			if(m_iCurVillage == VILLAGE_NONE)
			{
				xpos = m_minimap_x+110;
				ypos = m_minimap_y+120;
				width = 43;
				height = 43;
				if (x >= xpos  && x<=xpos+width && y >= ypos && y < ypos+height)
				{

					if(event == MOUSE_CLICK)
					{
						m_iCurVillage = 1; // 상세툈E오픈
						return 5;
					}
					else if (event == MOUSE_MOVE)
						return 4;
				}
				return 2;
			}
			else
			{
				xpos = m_minimap_x;
				ypos = m_minimap_y;
				width = 242;
				height = 256;

				if (x >= xpos  && x<=xpos+width && y >= ypos && y < ypos+height)
				{
					if(event == MOUSE_MOVE)
						return 3;
					else if(event ==MOUSE_CLICK)
					{
						m_iCurVillage = -1;
						return 6;
					}
				}
			}
		}
		else if(m_iCurWorld == WORLD_GUILD)
		{
			if(m_iCurVillage == VILLAGE_NONE)
			{
				int xpos, ypos, width, height;
				xpos = m_minimap_x+128;
				ypos = m_minimap_y+11;
				width = 28;
				height = 28;

				if (x >= xpos  && x<=xpos+width && y >= ypos && y < ypos+height)
				{
					if(event == MOUSE_CLICK)
					{
						m_iCurVillage = 2; // 샤일론 상세맵
						return 5;
					}
					else if (event == MOUSE_MOVE)
						return 4;
				}

				if(m_bShowGWarMap && m_pGWar)
				{
					int a = m_pGWar->GetWidth();

					if (x >= m_pGWar->Xpos
							&& x <= m_pGWar->Xpos + m_pGWar->GetWidth()
							&& y >= m_pGWar->Ypos
							&& y < m_pGWar->Ypos + m_pGWar->GetHeight() )
					{
						if(event == MOUSE_MOVE)
							return 3;

						// 마을 맵에서의 버튼이 클릭상태툈E다시 구필탛E맵으로 픸E?.
						else if(event ==MOUSE_CLICK)
						{
							m_bShowGWarMap = FALSE;
							return 6;
						}
					}

				}
				else
				{
					if (x >= m_GWarBtn.PosX
							&& x<= m_GWarBtn.PosX + m_GWarBtn.Width
							&& y >= m_GWarBtn.PosY
							&& y < m_GWarBtn.PosY + m_GWarBtn.Width)
					{
						// 마을 버튼이 클릭이 되었다툈E맵을 마을 멥으로
						if(event == MOUSE_CLICK)
						{
							m_bShowGWarMap = TRUE;
							return 5;
						}
						// 마을버튼 위에 마퓖E별?오버된 상태이툈E마퓖E봬옘??바꾼다
						else if (event == MOUSE_MOVE)
							return 4;
					}
				}
				return 2;
			}
			else if(m_iCurVillage == 2)
			{
				int xpos, ypos, width, height;
				xpos = m_minimap_x;
				ypos = m_minimap_y;
				width = 242;
				height = 256;

				if (x >= xpos  && x<=xpos+width && y >= ypos && y < ypos+height)
				{
					if(event == MOUSE_MOVE)
						return 3;
					else if(event ==MOUSE_CLICK)
					{
						m_iCurVillage = -1;
						return 6;
					}
				}
			}
		}
		return 1;
	}
	else
		return 0;

}


LRESULT CControlMap::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !g_pRoh )
		return 0;

	int x, y;
	int ret;
	int i = 0;
	int nX = m_nCurClick_X - m_minimap_x;
	int nY = m_nCurClick_Y - m_minimap_y;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	m_GuildFindBtn.MsgProc( hWnd, msg, wParam, lParam ); ///미니맵길드원찾콅E
	m_PlayerFindBtn.MsgProc( hWnd, msg, wParam, lParam );


	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		m_eState = MOUSE_CLICK;

		ret = IsInside(x, y, m_eState);
		//====================================
		// nate 2005-03-21
		// Matrix : Indication
		// 렌더링에 들엉怯 부분
		if(
			ret == 1
			&& (pCMyApp->m_nCurWorldIdx == 10
				|| pCMyApp->m_nCurWorldIdx == 19
			   )
			&& g_pRoh->m_GuildLevel == 10	// 10 : 지휘자 권한
			&& !m_bIndication_Point )
		{
			m_nCurClick_X = x;	// 현픸E클랙된 마퓖E?좌표 저픸E
			m_nCurClick_Y = y;
			m_bIndication_Point = 1;
		}

		if(pCMyApp->m_pMouse && ret ==  5)
			pCMyApp->m_pMouse->SetMouseType(M_ZOOM_OUT);

		if(pCMyApp->m_pMouse && ret ==  6)
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

		if(ret != 0)
			return 1;
		else
			return 0;

		break;

	case WM_LBUTTONUP:

		// nate 2005-03-21
		// Matrix : Indication
		m_eState		= MOUSE_NONE; // 이걸해줘야 세부맵들엉瞼는게 오작동 안한다. ///미니맵길드원찾콅E시에 버그를 발견하여 수정

		ret = IsInside(x, y, m_eState);

		// 종료버튼
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if (m_CloseBtn.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, MAP);
			m_CloseBtn.SetState(BTN_NORMAL);
			return 1;
		}

		// 길드퓖E찾콅E버튼. ///미니맵길드원찾콅E
		if( pCMyApp->m_nCurWorldIdx != 10 || g_bAdmin2 ) // 매트릭스에선 길원찾콅E안됨. // 웝滂자는뫼비퓖E봇【??되도록.
		{
			m_PlayerFindBtn.MsgProc( hWnd, msg, wParam, lParam );
			m_GuildFindBtn.MsgProc( hWnd, msg, wParam, lParam );
			if( m_GuildFindBtn.GetState() == BTN_ACTION )
			{
				if( g_pRoh->m_GuildLevel == 0 ) // 길드가 없으툈E.
				{
					if( pCMyApp->m_pUIMgr ) // 미니맵 클래스에서 기존에 쓰큱E메세햨E팝업이 없으므로 UIMgr껄 쓴다.
						//-- IDS_NOGUILD : 소속된 길드가 없습니다..
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NOGUILD), TYPE_NOR_OK, 1 );

					return 1;
				}

				if( timeGetTime()-GuildFindTick > 11000 ) // 한퉩E누른후 11초간은 다시 못누르게 처리...
				{
					// 리스트 초기화가 끝나컖E보내는 것이 좋을탛E
					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage( "g_pos req\n" );

				}

				return 1;
			}


			if( m_PlayerFindBtn.GetState() == BTN_ACTION )
			{
				

				if( timeGetTime()-GuildFindTick > 11000 ) // วันEดฉธฅศฤ 11รสฐฃภบ ดูฝร ธ๘ดฉธฃฐิ รณธฎ...
				{
					// ธฎฝบฦฎ รสฑโศญฐก ณกณชฐEบธณปดย ฐอภฬ มมภปตE
					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage( "g_pos req_player\n" );

				}

				return 1;
			}


			// 길드퓖E포인트 클릭시 귓말로 가게 처리... ///미니맵길드원찾콅E
			if( m_DrawGuildNameNum ) // 이게 있으툈E길드퓖E표지션에다 더빚細릭한걸로 인식
			{
				if( pCMyApp->m_pUIMgr && g_pNk2DFrame
						&& g_pNk2DFrame->GetControlBottom() )
				{
					char strTemp[256];

					strcpy( strTemp, g_pNk2DFrame->GetControlBottom()->m_InputBox.GetStrBuf() );

					int SetNameNum = 0; // 귓말에 넣을 녀석의 인덱스.

					if( strncmp(strTemp,"/",1) == 0 ) // 채팅창에 셋팅된 스트링이 귓말 모드인딪菩 판별된다툈E..
					{
						for( i = 0 ; i < m_DrawGuildNameNum ; ++i )
						{
							if( m_GuildPosMultiText.m_Text[i] )
							{
								if( strncmp( &strTemp[1], m_GuildPosMultiText.m_Text[i], strlen(m_GuildPosMultiText.m_Text[i]) ) == 0 ) // 지금 가리킨 길드퓖E포인트 중에서 이름이 같은 녀석을 찾는다. // 이녀석도 strncmp를 써주는 이유는 귓말 채팅에 공백이 있기때문.
								{
									SetNameNum = ++i; // 찾은 녀석 다음 녀석으로 셋팅 해준다.

									break; // 찾았으툈E중단.
								}
							}
						}
					}

					if( SetNameNum >= m_DrawGuildNameNum ) // 넘엉怯경퓖E다시 0으로 돌려준다.
						SetNameNum = 0;

					if( m_GuildPosMultiText.m_Text[SetNameNum] ) // 안픸E?위해 체크.
					{
						sprintf( strTemp,"/%s ", m_GuildPosMultiText.m_Text[SetNameNum] );
						g_pNk2DFrame->GetControlBottom()->SetFocus();
						g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
						g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText( strTemp );
					}
				}
			}
		}

		if( ret == 1
				&& (pCMyApp->m_nCurWorldIdx == 10
					|| pCMyApp->m_nCurWorldIdx == 19
				   )
				&& g_pRoh->m_GuildLevel == 10	// 10 : 지휘자 권한
				&& m_bIndication_Point == 1
				&& nX > 0 && nY > 0 && nX < 240 && nY < 250)
		{
			m_nCurClick_X = x;	// 현픸E클랙된 마퓖E?좌표 저픸E
			m_nCurClick_Y = y;
			m_dwTimeIndication = timeGetTime();
			char szBuf[ 100 ] = {0,};
			sprintf( szBuf, "mini %d %d\n", m_nCurClick_X - m_minimap_x, m_nCurClick_Y - m_minimap_y );

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( szBuf );

			m_bIndication_Point = 2;
			return 1;
		}

		break;

	case WM_RBUTTONDOWN:
		if (IsInside(x, y, m_eState))
			return 1;
		break;

	case WM_MOUSEMOVE:
		m_eState = MOUSE_MOVE;

		ret = IsInside(x, y, m_eState);

		/////////////////////////////////////////////////////////////////////////////
		// 미니맵 길드퓖E찾콅E기능 (05-10-18 원석)  ///미니맵길드원찾콅E
		m_DrawGuildNameNum = 0; // 퓖E?영으로 초기화. 커서가 미니맵 밖에 있을때도 이 녀석을 초기화해서 더이퍊E출력안하게 해야한다.

		// 드미트론 픸E갋현황 표시
		if (pCMyApp->m_nCurWorldIdx == WORLD_DMITRON ||
				pCMyApp->m_nCurWorldIdx == WORLD_DMITRON_BATTLE )
			CalcPopupIndex(x, y);

		if( ret != 0 ) // 영역안에서 움직였다툈E..
		{
			m_GuildPosMultiText.Reset();

			// 입력된 길원수만큼 돌면서 검퍊E
			vector<sGuildPosInfo>::iterator prGuildPosVector;
			sGuildPosInfo *pGuildPosInfo;

			RECT temp_rect;
			POINT ScreenPos;

			for( prGuildPosVector = m_GuildPosVector.begin() ; prGuildPosVector != m_GuildPosVector.end(); ++prGuildPosVector )
			{
				pGuildPosInfo = &(*prGuildPosVector);

				ScreenPos = GetWorldToMiniMap( pGuildPosInfo->m_PosX, pGuildPosInfo->m_PosY ); // 월드좌표를 미니맵 화툈E좌표로 변환한다.

				if( pGuildPosInfo->m_Grade >= 9 ) // 9: 부길마 , 10 길마
				{
					temp_rect.left = ScreenPos.x-7;
					temp_rect.right = ScreenPos.x+7;
					temp_rect.top = ScreenPos.y-7;
					temp_rect.bottom = ScreenPos.y+7;
				}
				else // 길퓖E
				{
					temp_rect.left = ScreenPos.x-4;
					temp_rect.right = ScreenPos.x+4;
					temp_rect.top = ScreenPos.y-4;
					temp_rect.bottom = ScreenPos.y+4;
				}

				if( x > temp_rect.left && x < temp_rect.right && y > temp_rect.top && y < temp_rect.bottom ) // 마퓖E?포인터가 점 영역 근처에 있으툈E.
				{
					++m_DrawGuildNameNum; // 인원펯E증가시키컖E 인원수는 맥스가 차큱E말큱E무조건 증가시켜야한다. 그래야 표시 한계를 넘엉檄는지가 체크된다.

					if( m_DrawGuildNameNum < MAX_DRAW_GUILD_POS_NAME-1 )
					{
						m_GuildPosMultiText.AddString( pGuildPosInfo->m_strName, RGB(255,255,255) );

						m_DrawGuildNamePos.x = x; // 나중에 보정처리해야한다.
						m_DrawGuildNamePos.y = y; // 화면의 오른쪽 끝일땐 왼쪽으로 출력되게 탛E..
					}
				}
			}
		}
		/////////////////////////////////////////////////////////////////////////////

		// nate 2005-03-21
		// Matrix : Indication
		if( ret == 1
				&& g_pRoh->m_GuildLevel == 10
				&& m_bIndication_Point )
		{
			// 10 : 지휘자 권한
			if( m_bIndication_Point == 1 )
			{
				m_nCurClick_X = x;	// 현픸E클랙된 마퓖E?좌표 저픸E
				m_nCurClick_Y = y;
			}
		}

		if( pCMyApp->m_pMouse && ret == 2 )
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

		if( pCMyApp->m_pMouse && ret == 3 )
			pCMyApp->m_pMouse->SetMouseType(M_ZOOM_OUT);

		if( pCMyApp->m_pMouse && ret == 4 )
			pCMyApp->m_pMouse->SetMouseType(M_ZOOM_IN);


		if(ret != 0)
			return 1;
		else
			return 0;
		break;
	}
	return 0;
}

#define WORLD_SIZE (256)

BOOL CControlMap::GetMapMousePos(int &x, int &y)
{
	if (!g_pNk2DFrame || !g_pNk2DFrame->IsInterfaceVisible(MAP))
		return FALSE;

	x = (g_CursorPoint.x - m_minimap_x) * WORLD_SIZE / 242;
	y = (g_CursorPoint.y - m_minimap_y) * WORLD_SIZE / 256;

	if (x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_SIZE)
		return FALSE;

	return TRUE;
}

//==========================================================
// nate 2004 - 10
// GuildHunt
void CControlMap::Show_PosPoint_InGuildHuntArea( BYTE byZone )
{
	RECT rcGuildHuntMiniMapRect;
	if( byZone == GUILDHUNT_SHILON )
	{
		SetRect( &rcGuildHuntMiniMapRect, 3470, 3190, 4950, 1910 );
		if( m_pShilonGuildHuntMap )
			CalPos( rcGuildHuntMiniMapRect
					,m_pShilonGuildHuntMap->Xpos + 30
					,m_pShilonGuildHuntMap->Ypos + 40
					, 180, 156 );
	}
	else if( byZone == GUILDHUNT_WHITEHORN )
	{
		SetRect( &rcGuildHuntMiniMapRect, 7570, 1830, 11420, 520 );
		if( m_pWhiteHornGuildhuntMap )
			CalPos( rcGuildHuntMiniMapRect
					,m_pWhiteHornGuildhuntMap->Xpos + 30
					,m_pWhiteHornGuildhuntMap->Ypos + 99
					, 187, 65 );
	}
}

//==========================================================
//==========================================================
// nate 2005-03-21
// Matrix
// 출력 좌표 계퍊E및 출력
void CControlMap::Show_PosPoint_Indication()
{
	if( !m_pIndication_Point )
		return;

	int nX = m_nCurClick_X - m_minimap_x;
	int nY = m_nCurClick_Y - m_minimap_y;
	if( nX > 0 && nY > 0 && nX < 240 && nY < 250 )
		m_pIndication_Point->RenderOj_Fade( m_nCurClick_X, m_nCurClick_Y, 0, EFF_INDICATION_POINT );
}

// nate 2005-03-31
// 메트릭스 주둔지에 길탛E마크 출력
void CControlMap::Show_PosPOint_StoneState()
{
	if( !pCMyApp->m_pUIMgr )
		return;

	for( int i = 9 ; i < 12 ; i ++ )
	{
		// 미니맵에 마크출력
		switch( pCMyApp->m_pUIMgr->m_nStoneState[ i ] )
		{
		case 1:		// 호크로퓖E
			if( m_pHawklaw )
			{
				m_pHawklaw->Xpos = m_nArmyPostX[ 11 - i ];
				m_pHawklaw->Ypos = m_nArmyPostY[ 11 - i ];
				g_pDisplay->Blt( m_pHawklaw->Xpos, m_pHawklaw->Ypos, m_pHawklaw );
			}
			break;
		case 2:		// 쿨레인
			if( m_pCoolrain )
			{
				m_pCoolrain->Xpos = m_nArmyPostX[ 11 - i ];
				m_pCoolrain->Ypos = m_nArmyPostY[ 11 - i ];
				g_pDisplay->Blt( m_pCoolrain->Xpos, m_pCoolrain->Ypos, m_pCoolrain );
			}
			break;
		case 3:		// 쉬퉩E
			if( m_pShiver )
			{
				m_pShiver->Xpos = m_nArmyPostX[ 11 - i ];
				m_pShiver->Ypos = m_nArmyPostY[ 11 - i ];
				g_pDisplay->Blt( m_pShiver->Xpos, m_pShiver->Ypos, m_pShiver );
			}
			break;
		}
	}

	// nate 2005-04-18
	for( int i = 0 ; i < 6 ; i ++ )
	{
		if( !pCMyApp->m_pUIMgr->m_nStoneState[ i ] )
		{
			if( m_pBrokenGate )
			{
				m_pBrokenGate->Xpos = m_nBrokenGatePosX[ i ] + 1;
				m_pBrokenGate->Ypos = m_nBrokenGatePosY[ i ] + 1;
				g_pDisplay->Blt( m_pBrokenGate->Xpos, m_pBrokenGate->Ypos, m_pBrokenGate );
			}
		}
	}
}

void CControlMap::Show_PosPOint_StoneState_2009() // [12/8/2008 Theodoric] WORLD_MATRIX_2009
{
	if( !pCMyApp->m_pUIMgr )
		return;

	//if(!pCMyApp->m_pUIMgr->m_bOnBattle) // 뫼비퓖E별?진행상태가 아니툈E.
	//	return;

	int GstoneMapPosX[5] = {101,  48, 125, 178, 120};
	int GstoneMapPosY[5] = { 62, 110, 109, 133, 183};

	for( int i = 6 ; i < 11 ; i ++ ) // 각 스톤위치에 마크를 보여주자
	{
		// 미니맵에 마크출력
		switch( pCMyApp->m_pUIMgr->m_nStoneState[ i ] )
		{
		case 1:		// 툰드마르
			if( m_pTundmar )
			{
				m_pTundmar->Xpos = m_minimap_x + GstoneMapPosX[i-6];
				m_pTundmar->Ypos = m_minimap_y + GstoneMapPosY[i-6];
				g_pDisplay->Blt( m_pTundmar->Xpos, m_pTundmar->Ypos, m_pTundmar );
			}
			break;
		case 2:		// 안그마르
			if( m_pAngmar)
			{
				m_pAngmar->Xpos = m_minimap_x + GstoneMapPosX[i-6];
				m_pAngmar->Ypos = m_minimap_y + GstoneMapPosY[i-6];
				g_pDisplay->Blt( m_pAngmar->Xpos, m_pAngmar->Ypos, m_pAngmar );
			}
			break;
		}
	}

	int GPstoneMapPosX[3] = {185, 106,  55};
	int GPstoneMapPosY[3] = { 84, 126, 165};
	int GateMapPosX[4] = {37, 103, 122, 201};
	int GateMapPosY[4] = {33,  88, 146, 215};


	// 게이트 및 GP스흟E열림 상태
	for( int i = 0 ; i < 4 ; i ++ )
	{
		if( !pCMyApp->m_pUIMgr->m_nStoneState[ i ] )
		{
			if( m_pOpenGate )
			{
				m_pOpenGate->Xpos = m_minimap_x + GateMapPosX[ i ];
				m_pOpenGate->Ypos = m_minimap_y + GateMapPosY[ i ];
				g_pDisplay->Blt( m_pOpenGate->Xpos, m_pOpenGate->Ypos, m_pOpenGate );

				switch( i )
				{
				case 0:
				case 3:// 툰드마르 게이트 혹은 가드게이트 가 열렸다툈E미미르 GP가 깨햨E것
					m_pOpenGate->Xpos = m_minimap_x + GPstoneMapPosX[ 1 ];
					m_pOpenGate->Ypos = m_minimap_y + GPstoneMapPosY[ 1 ];
					break;

				case 1:
					m_pOpenGate->Xpos = m_minimap_x + GPstoneMapPosX[ 0 ];
					m_pOpenGate->Ypos = m_minimap_y + GPstoneMapPosY[ 0 ];
					break;

				case 2:
					m_pOpenGate->Xpos = m_minimap_x + GPstoneMapPosX[ 2 ];
					m_pOpenGate->Ypos = m_minimap_y + GPstoneMapPosY[ 2 ];
					break;
				}
				g_pDisplay->Blt( m_pOpenGate->Xpos, m_pOpenGate->Ypos, m_pOpenGate );
			}
		}
	}
}
//==========================================================

void CControlMap::ShowGuildMember() ///미니맵길드원찾콅E
{
	if( GuildFindTick != 0 && timeGetTime()-GuildFindTick < 10000 ) // 0이툈E안누른 상태컖E 눌린상태라툈E10초 동안만 표시한다.
	{
		POINT DrawPos;
		float fEffAni_Val = 0.0f;
		int i = 0;
		DWORD TempTick = 0;

		if(!GET_D3DDEVICE())
			return;

		D3DVIEWPORT7 vpOld, vpNew;
		// 현재의 뷰포트를 저장하컖E새롭게 뷰포트를 설정한다.
		GET_D3DDEVICE()->GetViewport(&vpOld);
		vpNew = vpOld;
		vpNew.dwWidth  = m_ClientRc.right;
		vpNew.dwHeight = m_ClientRc.bottom;
		GET_D3DDEVICE()->SetViewport(&vpNew);


		DWORD dwState[8];

		// 라이팅
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwState[0] );
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwState[1]);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[2]);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[3]);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[4]);
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwState[5] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwState[6] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwState[7] );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


		if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
		{
			// 입력된 길원수만큼 돌면서 출력.
			vector<sGuildPosInfo>::iterator prGuildPosVector;
			sGuildPosInfo *pGuildPosInfo;

			for( prGuildPosVector = m_GuildPosVector.begin() ; prGuildPosVector != m_GuildPosVector.end(); ++prGuildPosVector )
			{
				pGuildPosInfo = &(*prGuildPosVector);


				DrawPos = GetWorldToMiniMap( pGuildPosInfo->m_PosX, pGuildPosInfo->m_PosY ); // 월드좌표를 미니맵 화툈E좌표로 변환한다.

				// 퉩E㎞講갋 출력위치가 미니맵을 넘엉瞼툈E안혖E쨈?
				if( DrawPos.x > m_minimap_x && DrawPos.x < m_minimap_x+242 && DrawPos.y > m_minimap_y && DrawPos.y < m_minimap_y+256 )
				{
					if(	timeGetTime()-GuildFindTick > i*50 ) // 따遝된후 일정시간 후부터 출력.
					{
						TempTick = ( timeGetTime()-( GuildFindTick+(i*50) ) ) % 1300; // 2초 주기로 반복되게..
						fEffAni_Val = (float)(TempTick*0.0013f); // 0.0f~1.0f 사이로 변환.

						if( pGuildPosInfo->m_Grade >= 9 ) // 9: 부길마 , 10 길마
						{
							m_pMyGuilds->RenderGuildPos( DrawPos.x, DrawPos.y, fEffAni_Val, EFF_GUILDMASTER); ///미니맵길드원찾콅ED
						}
						else // 길퓖E
						{
							m_pMyGuilds->RenderGuildPos( DrawPos.x, DrawPos.y, fEffAni_Val, EFF_GUILDMEMBER); ///미니맵길드원찾콅ED
						}
					}
					++i; // 뫈礫째 녀석인지를 센다.(혖E갋녀석만 센다. 미니맵 퉩E㎏?넘엉瞼는 녀석은 안센다.)
				}
			}
			GET_D3DDEVICE()->EndScene();
		}
		GET_D3DDEVICE()->SetViewport(&vpOld);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwState[0] );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwState[1]);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[2]);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[3]);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[4]);
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwState[5] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwState[6] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwState[7] );


		if( m_DrawGuildNameNum ) // 이름을 그려야한다툈E..
		{
			if( m_DrawGuildNameNum >= MAX_DRAW_GUILD_POS_NAME ) // 표시해야 하는 인원을 넘었으툈E..
			{
				m_GuildPosMultiText.AddString( "......", RGB(255,255,255) );
				m_DrawGuildNameNum = MAX_DRAW_GUILD_POS_NAME-1; // 이걸 이렇게 셋팅해서 "......"을 늘리햨E않는다. 안하툈E"......."이 많아진다.
			}

			m_GuildPosNameBox.PutString( &m_GuildPosMultiText );
			m_GuildPosNameBox.Draw( m_DrawGuildNamePos.x, m_DrawGuildNamePos.y );
		}
	}
}

POINT CControlMap::GetWorldToMiniMap( int x, int y ) // 월탛E좌표로 부터 미니맵에 표시될 위치를 구해온다. ///미니맵길드원찾콅E
{
	POINT Result;
	int x_add = 0, y_add = 0; // 최종적인 미니맵 기준 좌표. 미니맵 출력 위치에 이 녀석을 더하툈E최종 출력 위치다.
	int sx = m_minimap_x, sy = m_minimap_y; // 맵 그림이 시작되는 위치.(조각 맵의 경퓖E그 위치가 각콅E틀리다. 조각맵이 아니툈E미니맵 틀의 시작위치가 된다.
	RECT	rc;
	int cx = 0; // 조각 맵의 가로길이.
	int cy = 0; // 조각 맵의 세로 길이// (주의!!) 좌표가 꺼꾸로..
	int dx = 0; // 조각 맵에서의 위치.(존 좌표 기준. 화툈E좌표가 아님에 유의.)
	int dy = 0; // 조각 맵에서의 위치.(존 좌표 기준. 화툈E좌표가 아님에 유의.)
	int wx = 0, wy = 0; // 조각맵의 가로 세로 사이햨E


	x_add = ( x / gMapExtention * MINI_MAP_WIDTH / gMapWidth);	// 이 공식은 일반적인 경퓖E?공식이다.
	y_add = ( (gMapHeight - (y/ gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );		 // 특이하게 구해야하는 경퓖E밑의 조건문에서 바꿔준다.

	if( m_iCurWorld == WORLD_DECKARDI )		//	1zone
	{
		if( m_iCurVillage == 3 )
		{
			SetRect( &rc, 5558, 7143, 7574, 5464 );

			sx = m_minimap_x+37;
			sy = m_minimap_y+46;

			wx = 168;
			wy = 139;

			cx = rc.right - rc.left;
			cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
			dx = (x - rc.left);
			dy = (rc.top - y);

			x_add = (dx * wx) / cx;
			y_add = (dy * wy) / cy;
		}
	}
	else if( m_iCurWorld == WORLD_DECKADUM ) // 2zone
	{
		switch( m_iCurDeckadum )
		{
		case 0:
			SetRect( &rc,80, 2480, 2500, 80 );

			if( m_pDeckadum_1 )
			{
				sx = m_pDeckadum_1->Xpos; // 이 녀석은 조각맵이 이므로 맵 틀의 시작 위치를 바꿔준다.
				sy = m_pDeckadum_1->Ypos;
			}
			break;

		case 1:
			SetRect( &rc, 60,5080,2520,2660 );

			if( m_pDeckadum_2 )
			{
				sx = m_pDeckadum_2->Xpos;
				sy = m_pDeckadum_2->Ypos;
			}
			break;

		case 2:
			SetRect( &rc, 2720, 2480,5040, 60 );

			if( m_pDeckadum_3 )
			{
				sx = m_pDeckadum_3->Xpos;
				sy = m_pDeckadum_3->Ypos;
			}
			break;

		case 3:
			SetRect( &rc, 3780, 5080, 5080, 3780 );

			if( m_pDeckadum_4 )
			{
				sx = m_pDeckadum_4->Xpos;
				sy = m_pDeckadum_4->Ypos;
			}
			break;
		}
		wx = 242;
		wy = 256;

		cx = rc.right - rc.left;
		cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
		dx = (x - rc.left);
		dy = (rc.top - y);

		x_add = (dx * wx) / cx;
		y_add = (dy * wy) / cy;
	}
	else if( m_iCurWorld == WORLD_GUILD )			// 3zone
	{
		if( pCMyApp->IsGuildHuntArea( x, y ) )
		{
			SetRect( &rc, 3470, 3190, 4950, 1910 );

			if( m_pShilonGuildHuntMap  )
			{
				sx = m_pShilonGuildHuntMap->Xpos+30;
				sy = m_pShilonGuildHuntMap->Ypos+40;
			}
			wx = 180;
			wy = 156;

			cx = rc.right - rc.left;
			cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
			dx = (x - rc.left);
			dy = (rc.top - y);

			x_add = (dx * wx) / cx;
			y_add = (dy * wy) / cy;
		}

		if( m_iCurVillage == 2 )
		{
			SetRect( &rc, 6907, 11948,7903, 11262 );

			if( m_pShailon_detail  )
			{
				sx = m_pShailon_detail->Xpos+52;
				sy = m_pShailon_detail->Ypos+82;
			}
			wx = 147;
			wy = 101;

			cx = rc.right - rc.left;
			cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
			dx = (x - rc.left);
			dy = (rc.top - y);

			x_add = (dx * wx) / cx;
			y_add = (dy * wy) / cy;
		}

		if(m_bShowGWarMap)
		{
			SetRect( &rc, 7432, 3060, 8014,2280 );

			if( m_pGWar  )
			{
				sx = m_pGWar->Xpos+57;
				sy = m_pGWar->Ypos+20;
			}
			wx = 149;
			wy = 214;

			cx = rc.right - rc.left;
			cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
			dx = (x - rc.left);
			dy = (rc.top - y);

			x_add = (dx * wx) / cx;
			y_add = (dy * wy) / cy;
		}
	}
	else if( m_iCurWorld == WORLD_LAGRAMIA)		// 4zone
	{
		if( m_iCurVillage == 1 ) // 픸E섯跏릴璲?아닌 세부 맵보콅E모드이툈E..
		{
			SetRect( &rc,6099, 5749, 8564,3608 );

			if( m_pLagramia_detail  )
			{
				sx = m_pLagramia_detail->Xpos+8;
				sy = m_pLagramia_detail->Ypos+44;
			}
			wx = 225;
			wy = 195;

			cx = rc.right - rc.left;
			cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
			dx = (x - rc.left);
			dy = (rc.top - y);

			x_add = (dx * wx) / cx;
			y_add = (dy * wy) / cy;
		}
	}
	else if( m_iCurWorld == WORLD_DUNGEON_S) //시루스트		// 5zone
	{
		switch( m_iSeaRoost )
		{
		case 0:
			SetRect( &rc,20, 5120,1960,20 );

			if( m_pSeaRoost_first )
			{
				sx = m_pSeaRoost_first->Xpos; // 이 녀석은 조각맵이 이므로 맵 틀의 시작 위치를 바꿔준다.
				sy = m_pSeaRoost_first->Ypos;

				wx = 93;
				wy = 256;
			}
			break;

		case 1:
			SetRect( &rc, 1100,5120,5120,3660 );

			if( m_pSeaRoost_second )
			{
				sx = m_pSeaRoost_second->Xpos;
				sy = m_pSeaRoost_second->Ypos;

				wx = 194;
				wy = 69;
			}
			break;

		case 2:
			SetRect( &rc, 1960,3660,4040,2220 );

			if( m_pSeaRoost_third )
			{
				sx = m_pSeaRoost_third->Xpos;
				sy = m_pSeaRoost_third->Ypos;

				wx = 102;
				wy = 73;
			}
			break;

		case 3:
			SetRect( &rc, 2050,2150,4100,850 );

			if( m_pSeaRoost_forth )
			{
				sx = m_pSeaRoost_forth->Xpos;
				sy = m_pSeaRoost_forth->Ypos;

				wx = 94;
				wy = 59;
			}
			break;
		}

		cx = rc.right - rc.left;
		cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
		dx = (x - rc.left);
		dy = (rc.top - y);

		x_add = (dx * wx) / cx;
		y_add = (dy * wy) / cy;
	}
	else if( m_iCurWorld == WORLD_SKY )				// 6zone
	{
		/*		// 이 공식 엉蘭서 끌엉輩는지는 모르겠지만 잘 못되있다. 에볼중이라던가 그런때 코탛E같은데.... ///07-04-10
				if( m_pParos )
				{
					sx = m_pSeaRoost_forth->Xpos;
					sy = m_pSeaRoost_forth->Ypos;
				}

				SetRect(&rc,0, 12750, 12750, 0);

				wx = 242;
				wy = 256-10;

				cx = rc.right - rc.left;
				cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
				dx = (x - rc.left);
				dy = (rc.top - y);

				x_add = (dx * wx) / cx;
				y_add = (dy * wy) / cy;
		*/
	}
	else if( m_iCurWorld == WORLD_WHITEHORN )			// 7zone
	{
		/*		// 이 공식 엉蘭서 끌엉輩는지는 모르겠지만 잘 못되있다. 에볼중이라던가 그런때 코탛E같은데.... ///07-04-10
				SetRect( &rc, 7570, 1830, 11420, 520 );

				if( m_pWhiteHornGuildhuntMap  )
				{
					sx = m_pWhiteHornGuildhuntMap->Xpos+30;
					sy = m_pWhiteHornGuildhuntMap->Ypos+99;
				}
				wx = 187;
				wy = 65;

				cx = rc.right - rc.left;
				cy = rc.top - rc.bottom ; // (주의!!) 좌표가 꺼꾸로..
				dx = (x - rc.left);
				dy = (rc.top - y);

				x_add = (dx * wx) / cx;
				y_add = (dy * wy) / cy;
		*/
	}
	else if( m_iCurWorld == WORLD_DUNGEON4 )			// 8zone
	{
		x_add = (int)( ( x / gMapExtention-1) * MINI_MAP_WIDTH / gMapWidth *1.65 + 8);
		y_add = (int)( (gMapHeight - (y / gMapExtention - 1)*1.6) * MINI_MAP_HEIGHT / gMapHeight -7);
	}	
	else if (m_iCurWorld == WORLD_DMITRON)				// 9zone
	{
	}
	else if (m_iCurWorld == WORLD_BOSSRAID_BULKAN)		// 13zone
	{
	}
	else if (m_iCurWorld == WORLD_BOSSRAID_KAILIPTON)	// 14zone
	{
	}
	else if (m_iCurWorld == WORLD_BOSSRAID_AIDIA)		// 15zone
	{
	}
	else if (m_iCurWorld == WORLD_BOSSRAID_HUMAN)		// 16zone
	{
	}
	else if (m_iCurWorld == WORLD_BOSSRAID_HYBRIDER)	// 17zone
	{
	}
	else if( m_iCurWorld == WORLD_BOSSRAID_FINALBOSS )		// 18zone
	{
		if( pCMyApp->IsFianlBossRoomArea(0,0) )
		{
			if( pCMyApp->IsFianlBossRoomArea(x,y) )
			{

				RECT rect;
				SetRect(&rect, 9600, 6540, 11400 ,4760);

				int start_x = m_minimap_x-27;
				int start_y = m_minimap_y-30;
				int width	= 295;
				int height	= 295;

				int cx = rect.right - rect.left;
				int cy = rect.top - rect.bottom ; // (주의!!) 좌표가 꺼꾸로..
				int nx = ((int)x - rect.left);
				int ny = (rect.top - (int)y);

				Result.x = start_x+(nx * width) / cx;
				Result.y = start_y+(ny * height) / cy;
				return Result;
			}
			else
			{
				Result.x = - 1;
				Result.y = - 1;
				return Result;
			}
		}
		else
		{
			SetRect( &rc, 9600, 6540, 11400 ,4760 );
			if ( rc.left < x && x < rc.right &&
					rc.top > y && y > rc.bottom )
			{
				Result.x = sx + 127;
				Result.y = sy + 50;
				return Result;
			}
			x_add += 18;
			y_add += 11;
		}
	}
	else if (m_iCurWorld == WORLD_DISPOSAL )				// 20zone
	{
	}
	else if( m_iCurWorld == WORLD_FORLORN_S3 )
	{	
		x_add = ( (x - (12800 / 2)) / gMapExtention * MINI_MAP_WIDTH / gMapWidth) * 2;
		y_add = ( (gMapHeight - (y/ gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight ) * 2;
	}
	else if( m_iCurWorld == WORLD_FORLORN_S4 ) // Add for S4
	{	
		x = abs((x - 6390)) * 2;
		if(x>12800)x=12790;
		else if(x < 100)x=100;
		y = (y+100) *2;
		if(y<50)y=100;
		else if(y>12800)y=12790;
		x_add = ( x / gMapExtention * MINI_MAP_WIDTH / gMapWidth);	// ภฬ ฐ๘ฝฤภบ ภฯนภ๛ภฮ ฐๆฟE?ฐ๘ฝฤภฬดู.
		y_add = ( (gMapHeight - (y/ gMapExtention - 1)) * MINI_MAP_HEIGHT / gMapHeight );		 // ฦฏภฬวฯฐิ ฑธวุพ฿วฯดย ฐๆฟEนุภว มถฐวนฎฟกผญ นูฒใมุดู.


	}
	else if (m_iCurWorld == WORLD_VIDBLINE )				// 21zone
	{
	}
	else if (m_iCurWorld == WORLD_HIGHTLAND )				// 25zone
	{
	}
	else if (m_iCurWorld == WORLD_TRITIAN )				// 26zone
	{
	}
	else if (m_iCurWorld == WORLD_SPACE )				// 27zone
	{
	}
	else if (m_iCurWorld == WORLD_ROYALE )				// 28zone
	{
	}
	else if (m_iCurWorld == WORLD_DUN910 )				// 28zone
	{
	}
	else if (m_iCurWorld == WORLD_MAP930 )				// 28zone
	{
	}
	else
	{
		x_add<<=1;
		y_add<<=1;
	}

	Result.x = sx + x_add;
	Result.y = sy + y_add;

	return Result;
}

void CControlMap::RenderLimitSight( int HeroX, int HeroY ) // 자콅E주변 미니맵만 보이게 해주는 효컖E///인큱E 시 추가
{
	if(!GET_D3DDEVICE())
		return;

	D3DVIEWPORT7 vpOld, vpNew;
	// 현재의 뷰포트를 저장하컖E새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);


	DWORD dwState[8];

	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwState[0] );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwState[1]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[2]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[3]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[4]);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwState[5] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwState[6] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwState[7] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );


	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		D3DTLVERTEX TLVec[4];

		int size = 40; // 자콅E주위로 보일 반컖E
		RECT rect;

		// 자콅E중심을 제외한 4 영역을 깩毒 사각?E막?..
		GET_D3DDEVICE()->SetTexture(0, NULL);

		// 좌상단
		rect.left = m_minimap_x;
		rect.right = HeroX+size;
		rect.top = m_minimap_y;
		rect.bottom = HeroY-size;
		if( rect.left < m_minimap_x )
			rect.left = m_minimap_x;
		if( rect.right > m_minimap_x+MINI_MAP_WIDTH )
			rect.right = m_minimap_x+MINI_MAP_WIDTH;
		if( rect.top < m_minimap_y )
			rect.top = m_minimap_y;
		if( rect.bottom > m_minimap_y+MINI_MAP_HEIGHT )
			rect.bottom = m_minimap_y+MINI_MAP_HEIGHT;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 1.0f );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);


		// 퓖E遮?
		rect.left = HeroX+size;
		rect.right = m_minimap_x+MINI_MAP_WIDTH;
		rect.top = m_minimap_y;
		rect.bottom = HeroY+size;
		if( rect.left < m_minimap_x )
			rect.left = m_minimap_x;
		if( rect.right > m_minimap_x+MINI_MAP_WIDTH )
			rect.right = m_minimap_x+MINI_MAP_WIDTH;
		if( rect.top < m_minimap_y )
			rect.top = m_minimap_y;
		if( rect.bottom > m_minimap_y+MINI_MAP_HEIGHT )
			rect.bottom = m_minimap_y+MINI_MAP_HEIGHT;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 1.0f );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);


		// 퓖E求?
		rect.left = HeroX-size;
		rect.right = m_minimap_x+MINI_MAP_WIDTH;
		rect.top = HeroY+size;
		rect.bottom = m_minimap_y+MINI_MAP_HEIGHT;
		if( rect.left < m_minimap_x )
			rect.left = m_minimap_x;
		if( rect.right > m_minimap_x+MINI_MAP_WIDTH )
			rect.right = m_minimap_x+MINI_MAP_WIDTH;
		if( rect.top < m_minimap_y )
			rect.top = m_minimap_y;
		if( rect.bottom > m_minimap_y+MINI_MAP_HEIGHT )
			rect.bottom = m_minimap_y+MINI_MAP_HEIGHT;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 1.0f );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);


		// 좌하단
		rect.left = m_minimap_x;
		rect.right = HeroX-size;
		rect.top = HeroY-size;
		rect.bottom = m_minimap_y+MINI_MAP_HEIGHT;
		if( rect.left < m_minimap_x )
			rect.left = m_minimap_x;
		if( rect.right > m_minimap_x+MINI_MAP_WIDTH )
			rect.right = m_minimap_x+MINI_MAP_WIDTH;
		if( rect.top < m_minimap_y )
			rect.top = m_minimap_y;
		if( rect.bottom > m_minimap_y+MINI_MAP_HEIGHT )
			rect.bottom = m_minimap_y+MINI_MAP_HEIGHT;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.top,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.bottom,0), 1, RGBA_MAKE(0,0,0,255), 0, 1.0f, 1.0f );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

		// 자콅E주변
		rect.left = HeroX-size;
		rect.right = HeroX+size;
		rect.top = HeroY-size;
		rect.bottom = HeroY+size;
		if( rect.left < m_minimap_x )
			rect.left = m_minimap_x;
		if( rect.right > m_minimap_x+MINI_MAP_WIDTH )
			rect.right = m_minimap_x+MINI_MAP_WIDTH;
		if( rect.top < m_minimap_y )
			rect.top = m_minimap_y;
		if( rect.bottom > m_minimap_y+MINI_MAP_HEIGHT )
			rect.bottom = m_minimap_y+MINI_MAP_HEIGHT;

		DWORD Color = RGBA_MAKE(10,10,10,200);

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.bottom,0), 1, Color, 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(rect.left,rect.top,0), 1, Color, 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.top,0), 1, Color, 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(rect.right,rect.bottom,0), 1, Color, 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_INVSRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_METEORSPARK]) );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

		GET_D3DDEVICE()->EndScene();
	}
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwState[0] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwState[1]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[2]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[3]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[4]);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwState[5] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwState[6] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwState[7] );
}

void CControlMap::MakeDmitronString(int Zone)
{
	char buffer[256];
	CMultiText multiText;
	multiText.AddString(CDmitronString::Zone( Zone ));
	multiText.AddString(CDmitronInfo::GetZone( Zone ).Guild);

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_PREFIX2));
	strcat(buffer, CDmitronString::Status( CDmitronInfo::GetZone( Zone ).Status ) );
	multiText.AddString(buffer);

	strcpy(buffer, CDmitronInfo::GetZone(Zone).GetBuff1());
	multiText.AddString(buffer);

	strcpy(buffer, CDmitronInfo::GetZone(Zone).GetBuff2());
	multiText.AddString(buffer);

	m_DmitronPopupText.SetString(&multiText);
	m_iDmitronPopup = Zone;
}

void CControlMap::CalcPopupIndex(int x, int y)
{
	if (IsInside(x, y, MOUSE_MOVE) == 0)
	{
		m_iDmitronPopup = -1;
		return;
	}
	else if (x >= m_minimap_x + 170 && x < m_minimap_x + 230 &&
			 y >= m_minimap_y + 200 && y < m_minimap_y + 230 )
	{

		if (m_iDmitronPopup != 0)
			MakeDmitronString( 0 );

		SetRect(&m_rcDmitronPopup,
				m_minimap_x + 100,
				m_minimap_y + 120,
				m_minimap_x + 100 + m_DmitronPopupText.m_Width+8,
				m_minimap_y + 120 + m_DmitronPopupText.m_Height+8 );
	}
	else if (x >= m_minimap_x + 20 && x < m_minimap_x + 80 &&
			 y >= m_minimap_y + 170 && y < m_minimap_y + 200 )
	{

		if (m_iDmitronPopup != 1)
			MakeDmitronString( 1 );

		SetRect(&m_rcDmitronPopup,
				m_minimap_x + 10,
				m_minimap_y + 105,
				m_minimap_x + 10 + m_DmitronPopupText.m_Width+8,
				m_minimap_y + 105 + m_DmitronPopupText.m_Height+8 );
	}
	else if (x >= m_minimap_x + 115 && x < m_minimap_x + 195 &&
			 y >= m_minimap_y + 75 && y < m_minimap_y + 100 )
	{

		if (m_iDmitronPopup != 2)
			MakeDmitronString( 2 );

		SetRect(&m_rcDmitronPopup,
				m_minimap_x + 65,
				m_minimap_y + 90,
				m_minimap_x + 65 + m_DmitronPopupText.m_Width+8,
				m_minimap_y + 90 + m_DmitronPopupText.m_Height+8 );
	}
	else if (x >= m_minimap_x + 25 && x < m_minimap_x + 105 &&
			 y >= m_minimap_y + 67 && y < m_minimap_y + 90 )
	{

		if (m_iDmitronPopup != 3)
			MakeDmitronString( 3 );

		SetRect(&m_rcDmitronPopup,
				m_minimap_x + 15,
				m_minimap_y + 60,
				m_minimap_x + 15 + m_DmitronPopupText.m_Width+8,
				m_minimap_y + 60 + m_DmitronPopupText.m_Height+8 );
	}
	else if (x >= m_minimap_x + 15 && x < m_minimap_x + 100 &&
			 y >= m_minimap_y + 40 && y < m_minimap_y + 67 )
	{

		if (m_iDmitronPopup != 4)
			MakeDmitronString( 4 );

		SetRect(&m_rcDmitronPopup,
				m_minimap_x + 15,
				m_minimap_y + 30,
				m_minimap_x + 15 + m_DmitronPopupText.m_Width+8,
				m_minimap_y + 30 + m_DmitronPopupText.m_Height+8 );
	}
	else m_iDmitronPopup = -1;
}

int CControlMap::GetInsideStone(int x, int y)
{
	int x1[10] = { 24,206, 54,105,184,105, 52,129,182,125};
	int x2[10] = { 38,219, 68,119,198,113, 60,137,190,133};
	int y1[10] = { 26,223,165,126, 83, 68,115,114,138,188};
	int y2[10] = { 39,236,179,140, 98, 75,122,121,145,195};

	if(!( x >= m_pBack->Xpos && x <= m_pBack->Xpos+ m_pBack->GetWidth() &&
			y >= m_pBack->Ypos && y <= m_pBack->Ypos + m_pBack->GetHeight() ))
		return -1;

	for(int i=0; i<10; i++)
	{
		if( m_minimap_x+x1[i]-3 <= x && m_minimap_x+x2[i]-3 >= x &&
				m_minimap_y+y1[i]-3 <= y && m_minimap_y+y2[i]-3 >= y)
			return i;
	}
	return -1;
}



void CControlMap::DrawMatrixStoneInfo()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	int stoneNum = GetInsideStone(point.x, point.y);
	if( stoneNum == -1)
		return;

	m_MatrixStoneInfoMulti.Reset();
	m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_01+stoneNum), _FCOLOR_YELLOW);

	if( pCMyApp->m_pUIMgr )
	{
		switch( stoneNum )
		{
		case 0:
		case 1:
			switch ( pCMyApp->m_pUIMgr->m_nStoneState[stoneNum+4] )
			{
			case 1:
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_11), _FCOLOR_GREEN);
				break;
			case 2:
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_12), _FCOLOR_RED);
				break;
			}
			break;

		case 2:
			if( !pCMyApp->m_pUIMgr->m_nStoneState[2] )
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_13), _FCOLOR_GREEN);
			else
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_14), _FCOLOR_RED);
			break;

		case 3:
			if( !pCMyApp->m_pUIMgr->m_nStoneState[0] || !pCMyApp->m_pUIMgr->m_nStoneState[3] )
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_13), _FCOLOR_GREEN);
			else
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_14), _FCOLOR_RED);
			break;

		case 4:
			if( !pCMyApp->m_pUIMgr->m_nStoneState[1] )
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_13), _FCOLOR_GREEN);
			else
				m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_14), _FCOLOR_RED);
			break;

		default:
			{
				DWORD color = _FCOLOR_RED;
				if( pCMyApp->m_pUIMgr->m_nStoneState[stoneNum+1] == g_pRoh->m_GuildIndex )
					color = _FCOLOR_GREEN;

				switch( pCMyApp->m_pUIMgr->m_nStoneState[stoneNum+1] )
				{
				case 1:
					m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_15), color);
					break;
				case 2:
					m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_16), color);
					break;
				case 0:
					m_MatrixStoneInfoMulti.AddString( (char*)G_STRING(IDS_MATRIX_2009_17), _FCOLOR_RED);
					break;
				}
			}
			break;
		}
	}
	m_MatrixStoneInfoPopup.PutString( &m_MatrixStoneInfoMulti );
	m_MatrixStoneInfoPopup.Draw(point.x, point.y);
}
