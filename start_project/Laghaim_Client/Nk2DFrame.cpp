#include "stdafx.h"
#define D3D_OVERLOADS

#include <ddraw.h>
#include <tchar.h>
#include <process.h>
#include <boost/format.hpp>
#include "Nk2DFrame.h"
#include "ddutil.h"
#include "dxutil.h"
#include "d3dmath.h"
#include "main.h"
#include "D3DEnum.h"
#include "MsgPopUp.h"
#include "country.h"
#include "TextOutBox.h"
#include "ControlLetter.h"
#include "Sprite.h"
#include "ControlUpgrade.h"
#include "WebCtrl.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "tcpipcon.h"
#include "Sheet.h"
#include "ControlTop.h"
#include "ControlStatus.h"
#include "ControlBottom.h"
#include "ControlInven.h"
#include "ControlSocial.h"
#include "ControlGate.h"
#include "ControlMap.h"
#include "ControlSkill.h"
#include "ControlShop.h"
#include "ControlRoyale.h"
#include "ItemProto.h"
#include "ItemRender.h"
#include "ControlScore.h"
#include "WarTax.h"
#include "matlist.h"
#include "Help.h"
#include "TextOutBox.h"
#include "scroll.h"
#include "AlphaPopup.h"
#include "BasicMenu.h"
#include "skill.h"
#include "controlhelp.h"
#include "controlexchange.h"
#include "interchange.h"
#include "controlparty.h"
#include "mouse.h"
#include "gamble.h"

#include "LockSmith.h"

#include "refine.h"
#include "Land.h"
#include "uimgr.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "RShiftMenu.h"
#include "LShiftMenu.h"
#include "controlwarehouse.h"
#include "controlusershop.h" ///개인상점
#include "ControlCashShop.h" ///유료상점
#include "controlpetstatus.h"
#include "controlpeiceshop.h"
#include "product.h"
#include "PetProduct.h"
#include "Event_Ancient.h"
#include "QuestData.h"
#include "Pet_Level.h"
#include "platinum.h"
#include "SummerEvent.h"
#include "GuildPK.h"
#include "AutoMouse.h"
#include "g_stringmanager.h"
#include "Process.h"
#include "ControlDocument.h"
#include "WorldCupStart.h"
#include "ControlHelper.h"
#include "SpiritMode.h"
#include "headers.h"
#include "AutoNotice.h"	// 브라질 요청 : 10분 마다 공지 메시지 출력
#include "PremiumExchange.h"
#include "ControlGuildSHouse.h"
#include "s_guild_manager.h"
#include "ControlGuildT.h"
#include "ControlGuildM.h"
#include "window_resource.h"
#include "WindowMgr.h"
#include "guild_data.h"
#include "guild_battle.h"

#include "ControlNotice.h" ///공지팝업창
#include "ControlHelpPet.h" ///펫도움말

#include "WindowMgr.h"

#include "PVPWidget.h"

#include "DmitronBattle.h"
#include "DmitronBattleTax.h"
#include "DmitronBattleStatus.h"

#include "ControlBossRaidInfo.h"

#include "ControlHeroFinalRank.h"

#include "ControlHeroList.h"
#include "ControlHeroPrivilege.h"

#include "ControlPremiumServiceChaMove.h"

#include "ControlYutboard.h"
#include "ControlFishing.h"
#include "ControlFishingReward.h"

#include "AutoPlay_test.h"
#include "GuildHunt.h"


#include "ControlSelectMenu.h"

#include "LHUI_OpEvent.h"

#include "LHUI_TakeBox.h"

#include "LHUI_Gate.h"

#include "LHUI_Define.h"

#include "UIFriend.h"

#include "UI_BuffItem.h"
#include "BuffItemMgr.h"
#include "DevTestGmCommand.h"

#include "CommonConfig.h"

#include "ControlDecardiBattle.h"
#include "ControlLotto.h"

#include "PacketSend.h"

#include "ShoutChatMgr.h"

#include "ControlBingo.h"
#include "ControlLetterWindow.h"
#include "ControlLetterSend.h"
#include "ControlLetterRecv.h"
#include "ControlBaseball.h"
#include "ControlBaseballReward.h"
#include "ControlLottery.h"
#include "ControlDiceGame.h"
#include "ControlDiceGameReward.h"
#include "ItemInfoScaleMgr.h"

CNk2DFrame * g_pNk2DFrame = NULL;

extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )
extern BOOL	   g_bAdmin;



extern float g_fTimeKey;
extern BOOL g_bRestart;

extern DWORD g_dwClientCountry;
extern DWORD g_dwLangType;
extern BOOL g_bUseCashShop; ///유료상점
extern BOOL g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.

extern void LoadSkills();
extern void DeleteSkills();

extern void SendAddFriend(const int& order, const std::string& name);
extern bool MobData_IsShop(int vnum);

char commOutBuf[512];
POINT g_CursorPoint;
//=============================
// nate 2004 - 4 : CastleWar
CSurface *g_pImgKing1 = NULL;
CSurface *g_pImgKing2 = NULL;
CSurface *g_pImgKing3 = NULL;
CSurface *g_pImgKing4 = NULL;
//=============================

CSurface *g_pPK = NULL;
CSurface *g_pNonPK = NULL;
CSurface *g_pMatrixKor = NULL; ///국가대항전 시의 국가 명 이미지

CSurface *g_pMatrixJpn = NULL; ///국가대항전 시의 국가 명 이미지
CSurface *g_pMatrixBra = NULL;
CSurface *g_pMatrixGer = NULL;
CSurface *g_pMatrixChi = NULL;
CSurface *g_pMatrixInd = NULL;
CSurface *g_pMatrixTha = NULL;

CSurface *g_pNvNWin = NULL; ///국가전승리 마크

// nate : 대만 명예 훈장


CSurface *g_pMedal = NULL;
CSurface *g_pGuildpk = NULL;

#define MSGPOPUP_NOT_EXIST  0
#define MSGPOPUP_EXIST  1

extern void SendEventMadCharName(const std::string& charName);
extern void SendMasterDelegation(const bool confirm);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//=============================================================================
//	Name : CNk2DFrame
//=============================================================================
CNk2DFrame::CNk2DFrame()
	: m_bLShift(FALSE)
	, m_bStatus(FALSE)
	, m_InfoTime(0)
	, m_InfoFrame(0)
	, m_InfoTime2(0)
	, m_InfoFrame2(0)
	, m_Slot_Num(0)
	, m_bLoadingNow(false)
	, m_bLoadingGoWorldNow(false)
{
	memset(m_strCheckWareHouse, 0, sizeof(m_strCheckWareHouse));
	memset(m_strRShiftName, 0, sizeof(m_strRShiftName));

	m_bRoyale = FALSE;
	m_pControlRoyale = NULL;

	m_pControlMenu = NULL;
	m_pControlBottom = NULL;
	m_pControlTop = NULL;
	m_pControlStatus = NULL;
	m_pRShiftMenu = NULL;
	m_pLShiftMenu = NULL;
	m_pAutoMouse = NULL;
	m_pControlInven = NULL;
	m_bAllLoaded = FALSE;

	m_pControlSkill = NULL;
	m_pControlShop = NULL;
	m_pControlExchange = NULL;
	m_pControlWareHouse = NULL;
	m_pControlGStoreHouse	= NULL;
	m_pGuildManager			= NULL;
	m_pControlGuild			= NULL;
	m_pControlGuildManager	= NULL;
	m_pControlGuildBattle	= NULL;
	m_pGuildRankWarBattleCount = NULL;
	m_pControlUserShop = NULL; ///개인상점
	m_pControlCashShop = NULL; ///유료상점
	m_pCashShopIntro = NULL; ///유료상점
	m_pInterchange = NULL;
	m_pProduct = NULL;
	m_pPetProduct = NULL;
	m_pControlHelp = NULL;
	m_pControlParty = NULL;
	m_pControlPetStatus = NULL;
	m_pControlPeiceShop = NULL;
	m_pGamble = NULL;

	m_pLockSmith = NULL;
	m_pRefine = NULL;
	m_pUpgrade = NULL; ///신방어구강화시스템
	m_pControlNotice = NULL; ///공지팝업창
	m_pUserShopSearch = NULL; ///개인상점검색

	m_pControlHelper = NULL;	//후견인
	m_pControlHelpPet = NULL;	///펫도움말

	m_pEventAncient = NULL;
	m_pPlatinum = NULL;
	m_pSummerEvent = NULL;
	m_pPet_Level = NULL;
	m_pGuildPK   = NULL;
	m_pStartSheet = NULL;
	m_pProcess = NULL;		//가공

	m_pSpiritMode = NULL;

	m_pWorldCup = NULL;


	m_ui_takebox = NULL;
	m_dwTakeBox_Time = 0;

	m_ui_gate = NULL;

	m_pAutoNotice = NULL;	// 브라질 요청 : 10분 마다 공지 메시지 출력

	m_bLetter = FALSE;
	m_GraphicMode = NKGRAPHIC2D;
	m_bMenu = FALSE;
	m_bShop = FALSE;
	m_bShowExchange = FALSE;
	m_bInterchange = FALSE;
	m_bProduct = FALSE;
	m_bPetProduct = FALSE;
	m_bInven = FALSE;
	m_bPetStatus = FALSE;
	m_bSkill = FALSE;
	m_bHelp = FALSE;
	m_bParty = FALSE;
	m_bViewTop = TRUE;
	m_bRShift = FALSE;
	m_bShowWareHouse = FALSE;

	m_bShowGStoreHouse = FALSE;
	m_bShowGuildManager = FALSE;
	m_bShowControlGuild = FALSE;
	m_bShowControlGuildManage = FALSE;

	m_bShowUserShop = FALSE; ///개인상점
	m_bPeiceShop = FALSE;
	m_bAutoMouse = FALSE;
	m_bPetLevel = FALSE;
	m_bZone      = FALSE;
	m_bWorldCup = FALSE;
	m_bHelper = FALSE;
	m_bHelpPet = FALSE; ///펫도움말
	m_bSpiritStart = FALSE;

	m_dpChaCapsyong = NULL;
	m_pInfoBox = NULL;
	m_pInfoBox2 = NULL;
	m_PartyReadyIndex = -1;
	m_pNkCha = NULL;
	m_bShowGamble = FALSE;

	m_bLockSmith = FALSE;

	m_bShowRefine = FALSE;
	m_bShowUpgrade = FALSE;	///신방어구강화시스템
	m_bShowNotice = FALSE;	///공지팝업창
	m_bShowUserShopSearch = FALSE; ///개인상점검색

	m_bShowCashShop = FALSE; ///유료상점
	m_bShowCashShopIntro = FALSE; ///유료상점
	m_bShowEventAncient	= FALSE;
	m_bShowPlatinum = FALSE;
	m_bShowSummerEvent = FALSE;
	m_bShowProcess = FALSE;
	m_Popup = NULL;
	m_iQuicSlotNum = -1;
	m_nDocument = 0;

	// [3/5/2007 Theodoric] 서버명 저장된 변수 초기화
	m_strConnectServerName = NULL;

	m_pUIWindow		= NULL;
	m_pWindowRes	= NULL;

	m_pControlMenu = NULL;
	m_pControlTop = NULL;
	m_pControlStatus = NULL;
	m_pRShiftMenu = NULL;
	m_pLShiftMenu = NULL;
	m_pControlBottom = NULL;
	m_pControlShop = NULL;
	m_pControlPeiceShop = NULL;
	m_pControlExchange = NULL;
	m_pControlWareHouse = NULL;
	m_pControlGStoreHouse = NULL;
	m_pGuildManager = NULL;

	m_pControlUserShop = NULL;
	m_pControlCashShop = NULL;
	m_pCashShopIntro = NULL;

	m_pInterchange = NULL;
	m_pProduct = NULL;
	m_pAutoMouse = NULL;
	m_pPetProduct = NULL;
	m_pControlInven = NULL;
	m_pControlSkill = NULL;
	m_pControlHelp = NULL;
	m_pControlParty = NULL;
	m_pControlPetStatus = NULL;
	m_pControlLetter = NULL;
	m_pPet_Level = NULL;
	m_pItemRender = NULL;
	m_pWorldCup = NULL;
	m_pControlHelper = NULL;
	m_pControlGuild = NULL;
	m_pControlGuildManager = NULL;
	m_pControlGuildBattle = NULL;
	m_pGuildRankWarBattleCount = NULL;
	m_pSpiritMode = NULL;
	m_pAutoNotice = NULL;

	m_pUIWindow = NULL;
	m_pWindowRes = NULL;
	m_pTgaUIWindow	= NULL;

	m_bShowPVPWidget = FALSE;
	m_pPVPWidget = NULL;

	m_bShowDmitronBattleStatus = TRUE;
	m_pDmitronBattleStatus = NULL;

	m_pBossraidInfo = NULL;
	m_bBossraidInfo = FALSE;

	m_pHeroFinalRank = NULL;
	m_bHeroFinalRank = FALSE;

	m_pHeroPrivilege = NULL;
	m_bHeroPrivilege = FALSE;
	m_pHeroRankList = NULL;
	m_bHeroRankList = FALSE;

	m_pPremiumExchange = NULL;
	m_bShowPremiumExchange = FALSE;


	LoadSkills();

	m_bProcLock = FALSE;
	m_pPremiumChaMove = NULL;
	m_bShowPremiumChaMove = FALSE;

	m_pControlSelMenu = NULL;
	m_bShowControlSelMenu = FALSE;

	m_ui_op_event = NULL;

	m_ui_takebox = NULL;
	m_dwTakeBox_Time = 0;

	m_pInterface = new CBaseInterface*[MAX_NUM];
	m_bInterface = new bool[MAX_NUM];
	for( int i = 0 ; i < MAX_NUM ; i ++ )
	{
		m_pInterface[ i ] = NULL;
		m_bInterface[ i ] = false;
	}

	m_ui_friend = NULL;
	m_ui_buffitem = NULL;
	m_ui_decardibattle = NULL;

	m_pStartSheet = new CSheet();
	m_pControlMenu = new CControlMenu();
	m_pControlTop = new CControlTop();
	m_pControlStatus = new CControlStatus();
	m_pRShiftMenu = new CRShiftMenu();
	m_pLShiftMenu = new CLShiftMenu();
	m_pControlBottom = new CControlBottom();
	m_pControlShop = new CControlShop();
	m_pControlPeiceShop = new CControlPeiceShop();
	m_pControlExchange = new CControlExchange();
	m_pControlWareHouse = new CControlWareHouse();
	m_pControlGStoreHouse = new CControlGuildSHouse();
	m_pGuildManager		  = CGuildSystemManager::Create();
	m_pControlUserShop = new CControlUserShop();  ///개인상점
	if( g_bUseCashShop )
	{
		m_pControlCashShop = new CControlCashShop();  ///유료상점
		m_pCashShopIntro = new CCashShopIntro();  ///유료상점
	}

	m_pInterchange = new CInterchange();
	m_pProduct = new CProduct();
	m_pAutoMouse  = new CAutoMouse();
	m_pPetProduct = new CPetProduct();
	m_pControlInven = new CControlInven();
	m_pControlRoyale = new CControlRoyale();
	m_pInterface[SCORE] = new CControlScore();
	m_pInterface[GATE] = new CControlGate();
	m_pInterface[SOCIAL] = new CControlSocial();
	m_pInterface[QUEST] = new CQuestData();
	m_pInterface[MAP] = new CControlMap();
	m_pInterface[DOCUMENT] = new CControlDocument();
	m_pInterface[YUTBOARD] = new ControlYutboard();
	m_pInterface[FISHING] = new ControlFishing();
	m_pInterface[FISHING_REWARD] = new ControlFishingReward();
	m_pInterface[BASEBALL] = new ControlBaseball();
	m_pInterface[BASEBALL_REWARD] = new ControlBaseballReward();
	m_pInterface[LOTTO] = new ControlLotto();
	m_pInterface[BINGO] = new ControlBingo();
	m_pInterface[LETTER_WINDOW] = new ControlLetterWindow();
	m_pInterface[LETTER_SEND] = new ControlLetterSend();
	m_pInterface[LETTER_RECV] = new ControlLetterRecv();
	m_pInterface[LOTTERY] = new ControlLottery();
	m_pInterface[DICEGAME] = new ControlDiceGame();
	m_pInterface[DICEGAME_REWARD] = new ControlDiceGameReward();

	m_pControlSkill = new CControlSkill();
	m_pControlHelp  = new CControlHelp();
	m_pControlParty = new CControlParty();
	m_pControlPetStatus = new CControlPetStatus();
	m_pPet_Level = new CPetLevel();
	m_pItemRender = new CItemRender();
	m_pWorldCup = new CWorldCupStart();
	m_pControlHelper = new CControlHelper();
	m_pControlGuild = new CControlGuild();
	m_pControlGuildManager = new CControlGuildManage();
	m_pControlGuildBattle = new CControlGuildBattle();
	m_pGuildRankWarBattleCount = new CGuildRankWarBattleCount();
	m_pSpiritMode = new CSpiritMode();
	m_pUIWindow = new CWindowMgr;
	m_pWindowRes = CWINDOWUI::Create();
	m_pTgaUIWindow = new CWindowTgaMgr;
	m_pPVPWidget = new CPVPWidget;
	m_pDmitronBattleStatus = new CDmitronBattleStatus;
	m_pBossraidInfo = new CControlBossraidInfo;
	m_pHeroFinalRank = new CControlHeroFinalRank;
	m_pHeroPrivilege = new CControlHeroPrivilege;
	m_pHeroRankList = new CControlHeroRankList;
	m_pPremiumExchange = new CPremiumExchange;
	
	LoadSkills();
	
	m_pPremiumChaMove = new CControlPremiumChaMove;
	m_pControlSelMenu = new CControlSelectMenu;

	m_ui_op_event = NULL;
	m_ui_takebox = NULL;
	m_dwTakeBox_Time = 0;

	m_exp_init = false;
	InitExpTable();

	m_force_exit = false;
	m_force_exit_tick = 0;
}

//=============================================================================
//	Name : ~CNk2DFrame
//=============================================================================
CNk2DFrame::~CNk2DFrame()
{
	if (GetControlExchange() && GetControlExchange()->IsExch())
	{
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("exch cancel\n", TRUE);
	}

	if (GetRefine() && GetRefine()->IsRefineConnection())
	{
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("ms reject\n", TRUE);
	}

	SAFE_DELETE(m_pPremiumExchange);


	SAFE_DELETE(m_pDmitronBattleStatus);

	SAFE_DELETE(m_pPremiumChaMove);

	SAFE_DELETE(m_pControlMenu);
	SAFE_DELETE(m_pStartSheet);
	SAFE_DELETE(m_pControlTop);
	SAFE_DELETE(m_pControlStatus);
	SAFE_DELETE(m_pRShiftMenu);
	SAFE_DELETE(m_pLShiftMenu);
	SAFE_DELETE(m_pControlBottom);
	SAFE_DELETE(m_pControlShop);
	SAFE_DELETE(m_pControlPeiceShop);
	SAFE_DELETE(m_pControlExchange);
	SAFE_DELETE(m_pControlWareHouse);
	SAFE_DELETE(m_pControlGStoreHouse);
	SAFE_DELETE(m_pGuildManager);
	SAFE_DELETE(m_pControlGuild);
	SAFE_DELETE(m_pControlGuildManager);
	SAFE_DELETE(m_pControlGuildBattle);

	SAFE_DELETE(m_pUIWindow);
	SAFE_DELETE(m_pWindowRes);

	SAFE_DELETE(m_pGuildRankWarBattleCount);

	SAFE_DELETE(m_pTgaUIWindow);

	SAFE_DELETE(m_pPVPWidget);
	SAFE_DELETE(m_pControlRoyale);
	SAFE_DELETE(m_pControlUserShop); ///개인상점
	SAFE_DELETE(m_pControlCashShop); ///유료상점
	SAFE_DELETE(m_pCashShopIntro); ///유료상점
	SAFE_DELETE(m_pInterchange);
	SAFE_DELETE(m_pProduct);
	SAFE_DELETE(m_pAutoMouse);
	SAFE_DELETE(m_pPetProduct);
	SAFE_DELETE(m_pControlInven);
	SAFE_DELETE(m_pInterface[SCORE]);
	SAFE_DELETE(m_pInterface[GATE]);
	SAFE_DELETE(m_pInterface[SOCIAL]);
	SAFE_DELETE(m_pInterface[QUEST]);
	SAFE_DELETE(m_pInterface[MAP]);
	SAFE_DELETE(m_pInterface[DOCUMENT]);
	SAFE_DELETE(m_pInterface[YUTBOARD]);
	SAFE_DELETE(m_pInterface[FISHING]);
	SAFE_DELETE(m_pInterface[FISHING_REWARD]);
	SAFE_DELETE(m_pInterface[BASEBALL]);
	SAFE_DELETE(m_pInterface[BASEBALL_REWARD]);
	SAFE_DELETE(m_pInterface[LOTTO]);
	SAFE_DELETE(m_pInterface[BINGO]);
	SAFE_DELETE(m_pInterface[LETTER_WINDOW]);
	SAFE_DELETE(m_pInterface[LETTER_SEND]);
	SAFE_DELETE(m_pInterface[LETTER_RECV]);
	SAFE_DELETE(m_pInterface[LOTTERY]);
	SAFE_DELETE(m_pInterface[DICEGAME]);
	SAFE_DELETE(m_pInterface[DICEGAME_REWARD]);
	SAFE_DELETE( m_ui_gate );
	SAFE_DELETE(m_pControlSkill);
	SAFE_DELETE(m_pControlHelp);
	SAFE_DELETE(m_pControlParty);
	SAFE_DELETE(m_pControlPetStatus);
	SAFE_DELETE(m_pWorldCup);
	SAFE_DELETE(m_pControlHelper);
	SAFE_DELETE(m_pSpiritMode);


	SAFE_DELETE(m_ui_takebox);

	DeleteObjects();

	// [12/18/2008 D.K ] : find memory leak .
	SAFE_DELETE_ARRAY( m_pInterface );
	SAFE_DELETE(m_pControlLetter);
	SAFE_DELETE(m_pItemRender);
	SAFE_DELETE(m_pInfoBox);
	SAFE_DELETE(m_pInfoBox2);

	DeleteSkills();

	if(m_pGamble)
	{
		delete m_pGamble;
		m_pGamble = NULL;
		m_bShowGamble = false;
	}

	if(m_pLockSmith)
	{
		delete m_pLockSmith;
		m_pLockSmith = NULL;
		m_bLockSmith = false;
	}

	if(m_pEventAncient)
	{
		delete	m_pEventAncient;
		m_pEventAncient = NULL;
		m_bShowEventAncient	= FALSE;
	}
	if(m_pPlatinum)
	{
		delete	m_pPlatinum;
		m_pPlatinum = NULL;
		m_bShowPlatinum	= FALSE;
	}
	if(m_pSummerEvent)
	{
		delete m_pSummerEvent;
		m_pSummerEvent = NULL;
		m_bShowSummerEvent = FALSE;
	}
	if(m_pRefine)
	{
		delete m_pRefine;
		m_pRefine = NULL;
		m_bShowRefine = false;
	}
	if( m_pUpgrade )	 ///신방어구강화시스템
	{
		delete m_pUpgrade;
		m_pUpgrade = NULL;
		m_bShowUpgrade = false;
	}
	if( m_pControlNotice ) ///공지팝업창
	{
		delete m_pControlNotice;
		m_pControlNotice = NULL;
		m_bShowNotice = false;
	}
	if( m_pUserShopSearch ) ///개인상점검색


	{
		delete m_pUserShopSearch;
		m_pUserShopSearch = NULL;
		m_bShowUserShopSearch = false;
	}
	if(m_pProcess)	//가공


	{
		delete m_pProcess;
		m_pProcess = NULL;
		m_bShowProcess = false;
	}

	// [3/5/2007 Theodoric] 서버명 저장된 변수 메모리 해제
	SAFE_DELETE_ARRAY(m_strConnectServerName);

	SAFE_DELETE(m_pGuildPK); // 길드전 관련
	SAFE_DELETE(m_pPet_Level);

	SAFE_DELETE(m_pBossraidInfo);
	m_bBossraidInfo = FALSE;

	SAFE_DELETE(m_pHeroFinalRank);
	m_bHeroFinalRank = FALSE;

	SAFE_DELETE( m_pHeroPrivilege );
	m_bHeroPrivilege = FALSE;
	SAFE_DELETE( m_pHeroRankList );
	m_bHeroRankList = FALSE;

	SAFE_DELETE_ARRAY( m_bInterface );

	SAFE_DELETE(m_pControlSelMenu);

	SAFE_DELETE( m_ui_op_event );

	SAFE_DELETE(m_ui_friend);


	SAFE_DELETE(m_ui_buffitem);
	SAFE_DELETE(m_ui_decardibattle);

	SAFE_DELETE(m_ui_takebox);

	SAFE_DELETE( m_ui_gate );

	DeleteAllPopup();
}

HRESULT CNk2DFrame::RestoreSurfaces()
{
	if (m_GraphicMode == NKGRAPHIC3D) // 이거 검사안해줘서 로그인화면등에서 화면전환하면 튕겼다.(06-01-05 원석)
	{
		if( IsExchangeVisible() )
			ShowExchangeWindow(FALSE, FALSE);

		if( m_ui_friend )			m_ui_friend->RestoreSurfaces();
		if( m_pControlInven	)		m_pControlInven->RestoreSurfaces();
		if( m_pControlStatus )		m_pControlStatus->RestoreSurfaces();
		if (m_pControlMenu)			m_pControlMenu->RestoreSurfaces();
		if (m_pControlBottom)		m_pControlBottom->RestoreSurfaces();
		if (m_pInterface[SCORE])	((CControlScore*)m_pInterface[SCORE])->RestoreSurfaces();
		if (m_pControlSkill)		m_pControlSkill->RestoreSurfaces();
		if (m_pControlPetStatus)	m_pControlPetStatus->RestoreSurfaces();
		if (m_pControlParty)		m_pControlParty->RestoreSurfaces();	///파티개편.
		if (m_pInterface[QUEST])	((CQuestData*)m_pInterface[QUEST])->RestoreSurfaces();
		if (m_pInterface[YUTBOARD]) ((ControlYutboard*)m_pInterface[YUTBOARD])->RestoreSurfaces();
		if (m_pInterface[FISHING])  ((ControlFishing*)m_pInterface[FISHING])->RestoreSurfaces();
		if (m_pInterface[FISHING_REWARD])	((ControlFishingReward*)m_pInterface[FISHING_REWARD])->RestoreSurfaces();
		if (m_pInterface[BASEBALL])	((ControlBaseball*)m_pInterface[BASEBALL])->RestoreSurfaces();
		if (m_pInterface[BASEBALL_REWARD])	((ControlBaseballReward*)m_pInterface[BASEBALL_REWARD])->RestoreSurfaces();
		if (m_pInterface[LOTTO])	((ControlLotto*)m_pInterface[LOTTO])->RestoreSurfaces();
		if (m_pInterface[BINGO])	((ControlBingo*)m_pInterface[BINGO])->RestoreSurfaces();
		if (m_pInterface[LETTER_WINDOW]) ((ControlLetterWindow*)m_pInterface[LETTER_WINDOW])->RestoreSurfaces();
		if (m_pInterface[LETTER_SEND]) ((ControlLetterSend*)m_pInterface[LETTER_SEND])->RestoreSurfaces();
		if (m_pInterface[LETTER_RECV]) ((ControlLetterRecv*)m_pInterface[LETTER_RECV])->RestoreSurfaces();
		if (m_pInterface[LOTTERY]) ((ControlLottery*)m_pInterface[LOTTERY])->RestoreSurfaces();
		if (m_pInterface[DICEGAME]) ((ControlDiceGame*)m_pInterface[DICEGAME])->RestoreSurfaces();
		if (m_pInterface[DICEGAME_REWARD]) ((ControlDiceGameReward*)m_pInterface[DICEGAME_REWARD])->RestoreSurfaces();
		if( m_pControlUserShop && m_pControlUserShop->m_bResLoad ) ///개인상점 // 개인상점은 한번 열었을때 리소스를 로딩해둔다. 그러므로 플래그를 확인하고 플래그를 초기화 해주는 루틴이 필요하다.
		{
			// 로딩을 했었었다면....
			m_pControlUserShop->m_bResLoad = FALSE; // 이 플래그를 지우면 다시 로딩할 수 있다.
			m_pControlUserShop->RestoreSurfaces();
		}
		if( m_pControlCashShop && m_pControlCashShop->m_bResLoad ) ///유료상점 // 유료상점도 한번 열었을때 리소스를 로딩해둔다. 그러므로 플래그를 확인하고 플래그를 초기화 해주는 루틴이 필요하다.
		{
			// 로딩을 했었었다면....
			m_pControlCashShop->m_bResLoad = FALSE; // 이 플래그를 지우면 다시 로딩할 수 있다.
			m_pControlCashShop->RestoreSurfaces();
		}
		if (m_pCashShopIntro)
			m_pCashShopIntro->RestoreSurfaces();	///유료상점

		{
			if(m_pControlGuild)
				m_pControlGuild->Restore();

			if(m_pControlGuildManager)
				m_pControlGuildManager->Restore();

			if(m_pUIWindow)
				m_pUIWindow->RestoreSurface();
		}

		if( g_SvrType == ST_ADULT_ONLY)
		{
			if(m_pTgaUIWindow)
				m_pTgaUIWindow->RestoreSurface();
		}

		if( m_pItemRender )
		{
			m_pItemRender->Restore();
		}

		if( m_pPet_Level )
		{
			m_pPet_Level->Restore();
		}

		RestorePopups();
	}
	else if( m_GraphicMode == NKGRAPHIC2D )
	{
		if( m_pStartSheet )
			m_pStartSheet->Restore();
	}

	return S_OK;
}

void CNk2DFrame::Init(int right, int bottom)
{
	if( g_pDisplay )
	{
		g_pDisplay->SetDirectDraw(GET_DD7());
		g_pDisplay->SetBackBuffer(GET_DDSRT());
		g_pDisplay->Clear();
	}
	m_ptCliSize.x = right;
	m_ptCliSize.y = bottom;
}

void CNk2DFrame::SetChaSet(CCapsyong **dpChaCapsyong, IndexedTexture *pChaTexture)
{
	m_dpChaCapsyong = dpChaCapsyong;

	if (m_pStartSheet)
		m_pStartSheet->SetChaSet(m_dpChaCapsyong, g_pCapsyongTexture);
}

void CNk2DFrame::SetEventType(int nType, int flag) ///이벤트버프표시
{
	m_ui_op_event->SetEvent( nType , flag );
}

void CNk2DFrame::SetHwnd(HWND hwnd)
{
	if( g_pDisplay )
		g_pDisplay->SetHwnd(hwnd);
}

void CNk2DFrame::SetFocus()
{
	if(m_pAutoMouse && m_bAutoMouse)
	{
		m_pAutoMouse->SetTextInFocus();
		return;
	}
	else if( GetUIWindow()->IsOpenWindow(WINDOW_GUILD_BATTLE_INVASION) )
	{
		CBattleInvasionAttack * wnd = (CBattleInvasionAttack*)GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_INVASION);
		if( wnd && wnd->IsTextBoxFocus() )
		{
			if(m_pControlBottom && m_pControlBottom)
				m_pControlBottom->LostFocus();
			return;
		}
	}

	if(m_pControlBottom && m_pControlBottom)
		m_pControlBottom->SetFocus();
}

BOOL CNk2DFrame::Load2DResources()
{
	// 게임 실행전 메뉴화면.
	if (m_pStartSheet && m_GraphicMode == NKGRAPHIC2D)
		m_pStartSheet->LoadRes();


	if (m_GraphicMode == NKGRAPHIC2D && g_bRestart == FALSE)
		pCMyApp->InitProgress(2);

	if (m_GraphicMode == NKGRAPHIC3D)
		Load2DResourcesInGame();

	m_bAllLoaded = TRUE;
	return TRUE;
}

BOOL CNk2DFrame::Load2DResourcesInGame()
{
	if (m_GraphicMode == NKGRAPHIC3D)
	{
		if(m_pItemRender)			m_pItemRender->Init();
		if(m_pControlBottom)		m_pControlBottom->LoadRes();
		if(m_pControlTop)			m_pControlTop->LoadRes();
		if(m_pControlStatus)		m_pControlStatus->LoadRes();
		if(m_pPremiumExchange)		m_pPremiumExchange->LoadRes();
		if(m_pPremiumChaMove)		m_pPremiumChaMove->LoadRes();
		if(m_pRShiftMenu)			m_pRShiftMenu->LoadRes();
		if(m_pLShiftMenu)			m_pLShiftMenu->LoadRes();
		if(m_pControlShop)			m_pControlShop->LoadRes();
		if(m_pControlPeiceShop)		m_pControlPeiceShop->LoadRes();
		if(m_pControlExchange)		m_pControlExchange->LoadRes();
		if(m_pControlWareHouse)		m_pControlWareHouse->LoadRes();
		if(m_pControlGStoreHouse)	m_pControlGStoreHouse->LoadRes();
		if(m_pControlGuild)			m_pControlGuild->LoadRes();
		if(m_pControlGuildManager)	m_pControlGuildManager->LoadRes();
		if(m_pWindowRes)			m_pWindowRes->LoadResource();
		if(m_pInterchange)			m_pInterchange->LoadRes();
		if(m_pProduct)				m_pProduct->LoadRes();
		if(m_pPetProduct)			m_pPetProduct->LoadRes();
		if(m_pControlRoyale)		m_pControlRoyale->LoadRes();
		if(m_pControlInven)			m_pControlInven->LoadRes();
		if(m_pControlSkill)			m_pControlSkill->LoadRes();
		if(m_pControlHelp)			m_pControlHelp->LoadRes();
		if(m_pControlParty)			m_pControlParty->LoadRes();
		if(m_pControlPetStatus)		m_pControlPetStatus->LoadRes();
		if(m_pPet_Level)			m_pPet_Level->LoadRes();
		if(m_pControlLetter)		m_pControlLetter->LoadRes();
		if(m_pCashShopIntro)		m_pCashShopIntro->LoadRes(); ///유료상점
		if(m_pWorldCup)				m_pWorldCup->LoadRes();
		if(m_pControlHelper)		m_pControlHelper->LoadRes();
		if(m_pSpiritMode)			m_pSpiritMode->LoadRes();		
		if(m_pInterface[MAP])		m_pInterface[MAP]->LoadRes();
		if(m_pInterface[SCORE] )	m_pInterface[SCORE]->LoadRes();
		if(m_pInterface[GATE])		m_pInterface[GATE]->LoadRes();
		if(m_pInterface[SOCIAL])	m_pInterface[SOCIAL]->LoadRes();
		if(m_pInterface[QUEST])		m_pInterface[QUEST]->LoadRes();
		if(m_pInterface[DOCUMENT])		m_pInterface[DOCUMENT]->LoadRes();
		if(m_pInterface[YUTBOARD])	m_pInterface[YUTBOARD]->LoadRes();
		if(m_pInterface[FISHING])	m_pInterface[FISHING]->LoadRes();		
		if(m_pInterface[FISHING_REWARD])	m_pInterface[FISHING_REWARD]->LoadRes();
		if(m_pInterface[BASEBALL])	m_pInterface[BASEBALL]->LoadRes();
		if(m_pInterface[BASEBALL_REWARD])	m_pInterface[BASEBALL_REWARD]->LoadRes();
		if(m_pInterface[LOTTO])		m_pInterface[LOTTO]->LoadRes();
		if(m_pInterface[BINGO])		m_pInterface[BINGO]->LoadRes();
		if(m_pInterface[LETTER_WINDOW])	m_pInterface[LETTER_WINDOW]->LoadRes();
		if(m_pInterface[LETTER_SEND])	m_pInterface[LETTER_SEND]->LoadRes();
		if(m_pInterface[LETTER_RECV])	m_pInterface[LETTER_RECV]->LoadRes();
		if(m_pInterface[LOTTERY])	m_pInterface[LOTTERY]->LoadRes();
		if(m_pInterface[DICEGAME])	m_pInterface[DICEGAME]->LoadRes();
		if(m_pInterface[DICEGAME_REWARD])	m_pInterface[DICEGAME_REWARD]->LoadRes();
		if(m_pPVPWidget)			m_pPVPWidget->LoadRes();
		if(m_pDmitronBattleStatus)	m_pDmitronBattleStatus->LoadRes();
		if( m_pBossraidInfo )	m_pBossraidInfo->LoadRes();
		if( m_pHeroFinalRank)	m_pHeroFinalRank->LoadRes();
		if(m_pHeroPrivilege)	m_pHeroPrivilege->LoadRes();
		if(m_pHeroRankList)		m_pHeroRankList->LoadRes();
		if( m_pControlSelMenu)	m_pControlSelMenu->LoadRes();

		if(!m_pGamble)
			m_pGamble = new CGamble();

		if(!m_pLockSmith)
			m_pLockSmith = new CLockSmith();
		if(!m_pRefine)
			m_pRefine = new CRefine();
		if( !m_pUpgrade )	///신방어구강화시스템
			m_pUpgrade = new CControlUpgrade(); // 이 인터페이스는 메모리를 창을 열때 생성하고 끌때 제거하는 방식으로 해봄.(상주 메모리를 줄이기 위해.)
		if( !m_pUserShopSearch )	///개인상점검색


			m_pUserShopSearch = new CControlUShopSearch(); // 이 인터페이스는 메모리를 창을 열때 생성하고 끌때 제거하는 방식으로 해봄.(상주 메모리를 줄이기 위해.)
		if(!m_pEventAncient)
			m_pEventAncient = new CEventAncient();
		if(!m_pPlatinum)
			m_pPlatinum = new Cplatinum();
		if(!m_pSummerEvent)
			m_pSummerEvent = new CSummerEvent();
		if(!m_pProcess)
			m_pProcess = new CProcess();

		//==========================================
		// nate 2004-4 : CastleWar - 성주 왕관 셋팅
		SAFE_DELETE( g_pImgKing1 );
		SAFE_DELETE( g_pImgKing2 );
		SAFE_DELETE( g_pImgKing3 );
		SAFE_DELETE( g_pImgKing4 );
		g_pDisplay->CreateSurfaceFromBitmap( &g_pImgKing1, "interface/war_tax/king.bmp" );
		if( g_pImgKing1 )
			g_pImgKing1->SetColorKey( RGB( 0, 0, 0 ) );
		g_pDisplay->CreateSurfaceFromBitmap( &g_pImgKing2, "interface/war_tax/king2.bmp" );
		if( g_pImgKing2 )
			g_pImgKing2->SetColorKey( RGB( 0, 0, 0 ) );
		g_pDisplay->CreateSurfaceFromBitmap( &g_pImgKing3, "interface/war_tax/king3.bmp" );
		if( g_pImgKing3 )
			g_pImgKing3->SetColorKey( RGB( 0, 0, 0 ) );
		g_pDisplay->CreateSurfaceFromBitmap( &g_pImgKing4, "interface/war_tax/d_king.bmp" );
		if( g_pImgKing4 )
			g_pImgKing4->SetColorKey( RGB( 0, 0, 0 ) );
		//==========================================
		SAFE_DELETE(g_pPK);
		g_pDisplay->CreateSurfaceFromBitmap (&g_pPK, "interface/war_tax/pk.bmp");
		if (g_pPK)
			g_pPK->SetColorKey(RGB(0, 0, 0));

		SAFE_DELETE(g_pNonPK);
		g_pDisplay->CreateSurfaceFromBitmap (&g_pNonPK, "interface/war_tax/peace.bmp");
		if (g_pNonPK)
			g_pNonPK->SetColorKey(RGB(0, 0, 0));

		SAFE_DELETE(g_pGuildpk);
		g_pDisplay->CreateSurfaceFromBitmap(&g_pGuildpk,"inderface/war_tax/guildpk.bmp");
		if(g_pGuildpk)
			g_pGuildpk->SetColorKey(RGB(0,0,0));

		// nate : 대만 매달 관련
		if( g_dwClientCountry == CTRY_TWN )
		{
			SAFE_DELETE(g_pMedal);
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMedal, "interface/top/hi_shield.bmp");
			if (g_pMedal)
				g_pMedal->SetColorKey(RGB(0, 0, 0));
		}


		//if( g_bGlobalMatrix )
	//	{
			SAFE_DELETE( g_pMatrixKor );
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMatrixKor, "interface/war_tax/nation_KR.bmp");			

			SAFE_DELETE( g_pMatrixJpn );
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMatrixJpn, "interface/war_tax/nation_JP.bmp");			

			SAFE_DELETE( g_pMatrixBra );
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMatrixBra, "interface/war_tax/nation_BR.bmp");	

			SAFE_DELETE( g_pMatrixGer );
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMatrixGer, "interface/war_tax/nation_GE.bmp");	

			SAFE_DELETE( g_pMatrixChi );
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMatrixChi, "interface/war_tax/nation_CH.bmp");	

			SAFE_DELETE( g_pMatrixInd );
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMatrixInd, "interface/war_tax/nation_IN.bmp");	

			SAFE_DELETE( g_pMatrixTha );
			g_pDisplay->CreateSurfaceFromBitmap (&g_pMatrixTha, "interface/war_tax/nation_TH.bmp");	
		//}

		SAFE_DELETE(g_pNvNWin);
		g_pDisplay->CreateSurfaceFromBitmap (&g_pNvNWin, "interface/war_tax/mobius_medal.bmp");
		if (g_pNvNWin)
			g_pNvNWin->SetColorKey(RGB(0, 0, 0));


		m_ui_op_event = new LHUI_OpEvent;

		if( CommonConfig::Instance()->GetFriendSystem() == true )
		{
			m_ui_friend = new CUIFriend;
		}
		
		m_ui_buffitem = new UI_BuffItem;
		m_ui_decardibattle = new ControlDecardiBattle;

		return TRUE;
	}
	return FALSE;
}

void CNk2DFrame::DeleteObjects()
{
	m_bAllLoaded = FALSE;

	SAFE_DELETE_RES(m_pPremiumChaMove);
	SAFE_DELETE_RES(m_pPremiumExchange);
	SAFE_DELETE_RES(m_pControlMenu);
	SAFE_DELETE_RES(m_pControlTop);
	SAFE_DELETE_RES(m_pControlStatus );
	SAFE_DELETE_RES(m_pUpgrade);
	SAFE_DELETE_RES(m_pRShiftMenu);
	SAFE_DELETE_RES(m_pLShiftMenu);
	SAFE_DELETE_RES(m_pItemRender);
	SAFE_DELETE_RES(m_pControlBottom);
	SAFE_DELETE_RES(m_pControlShop);
	SAFE_DELETE_RES(m_pControlPeiceShop);
	SAFE_DELETE_RES(m_pControlExchange);
	SAFE_DELETE_RES(m_pControlWareHouse);

	SAFE_DELETE_RES(m_pControlGStoreHouse);
	SAFE_DELETE_RES(m_pControlGuild);
	SAFE_DELETE_RES(m_pControlGuildManager);
	SAFE_DELETE_RES(m_pUIWindow);
	SAFE_DELETE_RES(m_pWindowRes);
	SAFE_DELETE_RES(m_pGuildRankWarBattleCount);
	SAFE_DELETE_RES(m_pControlRoyale);
	if( g_SvrType == ST_ADULT_ONLY )
	{
		SAFE_DELETE_RES(m_pTgaUIWindow);
	}

	SAFE_DELETE_RES(m_pPVPWidget);
	SAFE_DELETE_RES(m_pDmitronBattleStatus);
	SAFE_DELETE_RES(m_pBossraidInfo );

	SAFE_DELETE_RES(m_pHeroFinalRank);

	SAFE_DELETE_RES(m_pHeroPrivilege);
	SAFE_DELETE_RES(m_pHeroRankList);

	SAFE_DELETE_RES(m_pControlUserShop);
	SAFE_DELETE_RES(m_pControlCashShop);
	SAFE_DELETE_RES(m_pCashShopIntro);
	SAFE_DELETE_RES(m_pInterchange);
	SAFE_DELETE_RES(m_pProduct);
	SAFE_DELETE_RES(m_pAutoMouse);
	SAFE_DELETE_RES(m_pPetProduct);
	SAFE_DELETE_RES(m_pControlInven);
	SAFE_DELETE_RES(m_pControlSkill);
	SAFE_DELETE_RES(m_pControlHelp);
	SAFE_DELETE_RES(m_pControlParty);
	SAFE_DELETE_RES(m_pControlPetStatus);
	SAFE_DELETE_RES(m_pControlLetter);
	SAFE_DELETE_RES(m_pPet_Level);
	SAFE_DELETE_RES(m_pWorldCup);
	SAFE_DELETE_RES(m_pControlHelper);
	SAFE_DELETE_RES(m_pSpiritMode );
	SAFE_DELETE_RES(m_pInterface[QUEST]);
	SAFE_DELETE_RES(m_pInterface[SCORE]);
	SAFE_DELETE_RES(m_pInterface[GATE]);
	SAFE_DELETE_RES(m_pInterface[SOCIAL]);
	SAFE_DELETE_RES(m_pInterface[MAP]);
	SAFE_DELETE_RES(m_pInterface[DOCUMENT]);
	SAFE_DELETE_RES(m_pInterface[YUTBOARD]);
	SAFE_DELETE_RES(m_pInterface[FISHING]);
	SAFE_DELETE_RES(m_pInterface[FISHING_REWARD]);
	SAFE_DELETE_RES(m_pInterface[BASEBALL]);
	SAFE_DELETE_RES(m_pInterface[BASEBALL_REWARD]);
	SAFE_DELETE_RES(m_pInterface[LOTTO]);
	SAFE_DELETE_RES(m_pInterface[BINGO]);
	SAFE_DELETE_RES(m_pInterface[LETTER_WINDOW]);
	SAFE_DELETE_RES(m_pInterface[LETTER_SEND]);
	SAFE_DELETE_RES(m_pInterface[LETTER_RECV]);
	SAFE_DELETE_RES(m_pInterface[LOTTERY]);
	SAFE_DELETE_RES(m_pInterface[DICEGAME]);
	SAFE_DELETE_RES(m_pInterface[DICEGAME_REWARD]);

	if(m_pGamble)
	{
		delete m_pGamble;
		m_pGamble = NULL;
		if(IsGambleVisible())
			ShowGambleWindow(false);
	}

	if(m_pLockSmith)
	{
		delete m_pLockSmith;
		m_pLockSmith = NULL;
		if(IsLockSmith())
			ShowLockSmithWindow(false);
	}

	if(m_pEventAncient)
	{
		delete m_pEventAncient;
		m_pEventAncient = NULL;
		if(IsEventAncientVisible())
			ShowEventAncientWindow(false);
	}
	if(m_pPlatinum)
	{
		delete m_pPlatinum;
		m_pPlatinum = NULL;
		if(IsPlatinumVisible())
			ShowPlatinumWindow(false);
	}
	if(m_pSummerEvent)
	{
		delete m_pSummerEvent;
		m_pSummerEvent = NULL;
		if(IsSummerEventVisible())
			ShowSummerEventWindow(false);
	}
	if(m_pRefine)
	{
		delete m_pRefine;
		m_pRefine = NULL;
		if(IsRefineVisible())
			ShowRefineWindow(false);
	}
	if( m_pUpgrade ) ///신방어구강화시스템
	{
		delete m_pUpgrade;
		m_pUpgrade = NULL;
		if( IsUpgradeVisible() )
			ShowUpgradeWindow( false );	// 이건 굳이 Toggle로 안꺼줘도 된다. 포인터를 지우면 리소스 해제 하니까.
	}
	if( m_pControlNotice ) ///공지팝업창
	{
		delete m_pControlNotice;
		m_pControlNotice = NULL;
		m_bShowNotice = false;
	}
	if( m_pUserShopSearch ) ///개인상점검색


	{
		delete m_pUserShopSearch;
		m_pUserShopSearch = NULL;
		if( IsUserShopSearchVisible() )
			ShowUserShopSearchWindow( false );	// 이건 굳이 Toggle로 안꺼줘도 된다. 포인터를 지우면 리소스 해제 하니까.
	}
	if(m_pProcess)
	{
		delete m_pProcess;
		m_pProcess = NULL;
		if(IsProcessVisible())
			ShowProcessWindow(false);
	}
	// 스타트 화면


	SAFE_DELETE_RES(m_pStartSheet);

//	else
	{
		SAFE_DELETE( g_pNvNWin ); ///국가전승리 마크

//==============================================
		// nate 2004 - 4 : CastleWar
		SAFE_DELETE( g_pImgKing1 );
		SAFE_DELETE( g_pImgKing2 );
		SAFE_DELETE( g_pImgKing3 );
		SAFE_DELETE( g_pImgKing4 );
//==============================================
		SAFE_DELETE(g_pPK);
		SAFE_DELETE(g_pNonPK);
		SAFE_DELETE(g_pGuildpk);

	//	if( g_bGlobalMatrix ) ///국가대항전

	//	{
			SAFE_DELETE( g_pMatrixKor );
			SAFE_DELETE( g_pMatrixJpn );
			SAFE_DELETE( g_pMatrixBra );
			SAFE_DELETE( g_pMatrixGer );
			SAFE_DELETE( g_pMatrixChi );
			SAFE_DELETE( g_pMatrixInd );
			SAFE_DELETE( g_pMatrixTha );
		//}

		// nate : 대만 매달관련
		if( g_dwClientCountry == CTRY_TWN )
			SAFE_DELETE( g_pMedal );
	}

	SAFE_DELETE(m_pInfoBox);
	SAFE_DELETE(m_pInfoBox2);

	SAFE_DELETE_RES(m_pControlSelMenu);

}

void CNk2DFrame::DeleteStartSheet()
{
	SAFE_DELETE(m_pStartSheet);
}

BOOL CNk2DFrame::UpdateFrame()
{
	static int nSleep = 0;

	if (m_GraphicMode == NKGRAPHIC2D)
	{
		if( g_pDisplay )
			g_pDisplay->Clear();
		if (m_pStartSheet)
			m_pStartSheet->Draw();
	}
	else if (m_GraphicMode == NKGRAPHIC3D)
	{
		/*if( !pCMyApp->m_bWaitingAt
			&& !pCMyApp->m_bBeforeTrans
			&& !pCMyApp->m_bBeforeGoWorld )
		{

			
			static DWORD lastTickTime2 = 0;
			static time_t bBefore = 0;
			DWORD curTickTime = timeGetTime();

			int ttime=0;
			if(bBefore > 0)
			{
				ttime = time(0) - bBefore;

				if(ttime >=3 )
				{
					if(curTickTime - lastTickTime2 > 3500)
					{
						if(g_pTcpIp)
					//	g_pTcpIp->CloseSocket();
						extern HWND	g_hWnd;
					//	PostMessage(g_hWnd,0x555,0,0);
					}
					bBefore = time(0);
					lastTickTime2 = curTickTime;
				}


			}
			else
			{
				lastTickTime2 = curTickTime;
				bBefore = time(0);
			}

		}*/

		if( !pCMyApp->m_bWaitingAt
				&& !pCMyApp->m_bBeforeTrans
				&& !pCMyApp->m_bBeforeGoWorld )
			SendTickTime();

		GetCursorPos(&g_CursorPoint);
		ScreenToClient(g_pDisplay->GetHWnd(), &g_CursorPoint);


		if( m_ui_op_event->IsActivate() )
			m_ui_op_event->Draw();

		if (m_ui_friend && m_ui_friend->IsVisible())
			m_ui_friend->Draw();

		if( m_ui_buffitem )
			m_ui_buffitem->Draw();

		if( m_ui_decardibattle )
		{
			m_ui_decardibattle->Update();
			m_ui_decardibattle->Draw();
		}


		int over_skill = SKILL_UNSELECTED;
		int over_skill2 = SKILL_UNSELECTED;
		CItem *pInvenOverItem = NULL;
		CItem *pExchangeOverItem = NULL;
		CItem *pWareOverItem = NULL;
		CItem *pUserShopOverItem = NULL; ///개인상점
		CItem *pBottomOverItem = NULL;
		CItem *pShopOverItem = NULL;
		CItem *pInterchangeOverItem = NULL;
		CItem *pProductOverItem = NULL;
		CItem *pPetProductOverItem = NULL;
		CItem *pGambleOverItem = NULL;

		CItem *pLockSmithOverItem = NULL;

		CItem *pRefineOverItem = NULL;
		CItem *pUpgradeOverItem = NULL;	///신방어구강화시스템
		CItem *pPieceShopOverItem = NULL;
		CItem *pEventAncientOverItem = NULL;
		CItem *pPlatinumOverItem = NULL;
		CItem *pSummerEventOverItem = NULL;
		CItem *pProcessOverItem = NULL;
		CItem *pGuildStoreItem = NULL;
		CItem *pPremiumChaMoveItem = NULL;
		CItem *pPetEquipOverItem = NULL;

		if (m_pPremiumExchange && m_bShowPremiumExchange)
			m_pPremiumExchange->Draw();

		if(g_pRoh->GetMotionState() == CHA_DEAD && g_pRoh->m_Vital > 0 && pCMyApp->GetCurWorldID() == 4)
		{
			g_pRoh->SetMotionState(0);
		}

		if( m_pUIWindow )
			m_pUIWindow->Draw();

		if(m_pTgaUIWindow && g_SvrType == ST_ADULT_ONLY) m_pTgaUIWindow->Draw();


		if( !CheckFullScreenInterface() && 
				( m_bViewTop || m_bRoyale ||		m_bInven ||			m_bInterface[SCORE]	||		m_bInterface[MAP] ||
				  m_bSkill ||			m_bHelp ||			m_bParty ||					m_bPetStatus ||
				  m_bInterface[QUEST] || m_bInterface[DOCUMENT] || m_bHelper
				  || m_bShowControlGuild 	|| m_bShowControlGuildManage
				  || m_bHeroFinalRank
				))
		{
			if( m_pControlTop )
				m_pControlTop->Draw();

			if( m_pControlStatus )
				m_pControlStatus->Draw();

			if( g_bAdmin )
			{
				if( m_pControlBottom )
				{
					over_skill2 = m_pControlBottom->Draw(&pBottomOverItem);
					m_pControlBottom->Draw_Chatting();
				}
			}
		}

		if( !g_bAdmin )
		{
			if( !CheckFullScreenInterface() ) // 풀스크린 인터페이스가떠있으면 bottom도 안그린다. ///유료상점.
			{
				if( m_pControlBottom )
				{
					over_skill2 = m_pControlBottom->Draw(&pBottomOverItem);
					m_pControlBottom->Draw_Chatting();
				}
			}
		}
		if( m_pControlSelMenu && m_bShowControlSelMenu )
			m_pControlSelMenu->Draw();

		if( m_bShowDmitronBattleStatus && m_pDmitronBattleStatus)
		{
			m_pDmitronBattleStatus->Draw();
		}
		/***************************************************************************************/
		//- 탑 메뉴 관련 윈도우



		if (m_bMenu)
			m_pControlMenu->Draw();

		// 유료상점
		if( m_bShowCashShop )
			m_pControlCashShop->Draw();

		if (m_bShop)
			pShopOverItem = m_pControlShop->Draw();

		if (m_bPeiceShop)
			pPieceShopOverItem = m_pControlPeiceShop->Draw();

		// 창고

		if (m_bShowWareHouse)
			pWareOverItem = m_pControlWareHouse->Draw();

		// 개인상점
		if( m_bShowUserShop)
			pUserShopOverItem = m_pControlUserShop->Draw();


		if(m_bRoyale)
			m_pControlRoyale->Draw();


		// 인벤
		if( m_bInven)
			pInvenOverItem = m_pControlInven->Draw();

		// 팻
		if( m_bPetStatus)
			pPetEquipOverItem = m_pControlPetStatus->Draw();

		// 도움말
		if( m_bHelp)
			m_pControlHelp->Draw();

		// 스킬
		if( m_bSkill)
			over_skill = m_pControlSkill->Draw();

		// 파티
		if (m_bParty)
			m_pControlParty->Draw();

		// 후견인
		if (m_bHelper&&m_pControlHelper)
			m_pControlHelper->Draw();



		if( m_bInterface[SOCIAL])
			m_pInterface[SOCIAL]->Draw();

		if( m_bInterface[GATE])
			m_pInterface[GATE]->Draw();

		if( GetGate() )
			GetGate()->Draw();


		if( m_bInterface[SCORE])
			m_pInterface[SCORE]->Draw();

		// 지도
		if( m_bInterface[MAP])
			m_pInterface[MAP]->Draw();

		if( m_bInterface[DOCUMENT])
			m_pInterface[DOCUMENT]->Draw();

		// 퀘스트
		if (m_bInterface[QUEST])
			m_pInterface[QUEST]->Draw();
// 		else
// 			((CQuestData*)m_pInterface[QUEST])->CloseRes();

		if( m_bInterface[YUTBOARD] )
			m_pInterface[YUTBOARD]->Draw();

		if( m_pInterface[FISHING] )
			m_pInterface[FISHING]->Update();
		if( m_bInterface[FISHING] )
			m_pInterface[FISHING]->Draw();

		if( m_bInterface[FISHING_REWARD] )
			m_pInterface[FISHING_REWARD]->Draw();

		if( m_pInterface[BASEBALL] )
			m_pInterface[BASEBALL]->Update();
		if( m_bInterface[BASEBALL] )
			m_pInterface[BASEBALL]->Draw();
						 
		if( m_bInterface[BASEBALL_REWARD] )
			m_pInterface[BASEBALL_REWARD]->Draw();

		if( m_pInterface[LOTTO] )
			m_pInterface[LOTTO]->Update();
		if( m_bInterface[LOTTO] )
			m_pInterface[LOTTO]->Draw();

		if( m_pInterface[BINGO] )
			m_pInterface[BINGO]->Update();
		if( m_bInterface[BINGO] )
			m_pInterface[BINGO]->Draw();

		if( m_pInterface[LETTER_WINDOW] )
			m_pInterface[LETTER_WINDOW]->Update();
		if( m_bInterface[LETTER_WINDOW] )
			m_pInterface[LETTER_WINDOW]->Draw();

		if( m_pInterface[LETTER_SEND] )
			m_pInterface[LETTER_SEND]->Update();
		if( m_bInterface[LETTER_SEND] )
			m_pInterface[LETTER_SEND]->Draw();

		if( m_pInterface[LETTER_RECV] )
			m_pInterface[LETTER_RECV]->Update();
		if( m_bInterface[LETTER_RECV] )
			m_pInterface[LETTER_RECV]->Draw();

		if( m_pInterface[LOTTERY] )
			m_pInterface[LOTTERY]->Update();
		if( m_bInterface[LOTTERY] )
			m_pInterface[LOTTERY]->Draw();

		if( m_pInterface[DICEGAME] )
			m_pInterface[DICEGAME]->Update();
		if( m_bInterface[DICEGAME] )
			m_pInterface[DICEGAME]->Draw();
						 
		if( m_bInterface[DICEGAME_REWARD] )
			m_pInterface[DICEGAME_REWARD]->Draw();

		// 플레티늄
		if(m_bShowPlatinum)
			pPlatinumOverItem = m_pPlatinum->Draw();

		//
		if(m_bShowRefine)
			pRefineOverItem = m_pRefine->Draw();

		//
		if( m_bShowUpgrade )	///신방어구강화시스템
			pUpgradeOverItem = m_pUpgrade->Draw();

		// 유저 상점 검색

		if( m_bShowUserShopSearch )	///개인상점검색

			m_pUserShopSearch->Draw();



		{
			// 시규서버 길드 창고

			if(m_bShowGStoreHouse )
				pGuildStoreItem = m_pControlGStoreHouse->Draw();

			if (m_bShowControlGuild && m_pControlGuild)
				m_pControlGuild->Draw();		// 길드


			if(m_bShowControlGuildManage && m_pControlGuildManager)
				m_pControlGuildManager->Draw();

			if( m_bHeroFinalRank && m_pHeroFinalRank )
				m_pHeroFinalRank->Draw();

		}
		/***************************************************************************************/

		if( m_bShowPVPWidget && m_pPVPWidget )
			m_pPVPWidget->Draw();

		if (m_bLetter)
			m_pControlLetter->Draw();

		if (m_bShowExchange)
			pExchangeOverItem = m_pControlExchange->Draw();
		if (m_bInterchange)
			pInterchangeOverItem = m_pInterchange->Draw();
		if (m_bProduct)
			pProductOverItem = m_pProduct->Draw();
		if(m_bPetProduct)
			pPetProductOverItem = m_pPetProduct->Draw();
		if(m_bShowGamble)
			pGambleOverItem = m_pGamble->Draw();

		if(m_bLockSmith)
			pLockSmithOverItem = m_pLockSmith->Draw();


		if(m_bShowEventAncient)
			pEventAncientOverItem = m_pEventAncient->Draw();

		if(m_bShowSummerEvent)
			pSummerEventOverItem = m_pSummerEvent->Draw();

		if( m_bShowProcess )	//가공
			pProcessOverItem = m_pProcess->Draw();


		if( m_ui_takebox )
			m_ui_takebox->Draw();


		if (m_bRShift)
			m_pRShiftMenu->Draw();
		if (m_bLShift && m_pLShiftMenu)
			m_pLShiftMenu->Draw();
		if (m_bPetLevel && m_pPet_Level)
			m_pPet_Level->Draw();
		if(m_pControlInven && m_pControlInven->m_plus5Message.m_bActive)
			m_pControlInven->m_plus5Message.Draw();

		if (m_bWorldCup)
		{
			m_pWorldCup->Draw();
			m_pWorldCup->DrawScore();
		}

		// [5/6/2008 반재승] 브라질 요청. 채팅 글씨가 유료상점 창 밑으로 그려지게.
		if( m_bShowCashShop ) ///유료상점
			m_pControlCashShop->Draw();
		if( m_bShowCashShopIntro ) ///유료상점
			m_pCashShopIntro->Draw();

//========================================================================
// 아이템 정보 출력
		if( m_pControlCashShop && m_pControlCashShop->m_pInvenOverItem ) // 인벤창의 DrawItemInfo 를 여기서 해줘야 제대로 나오더라. ///유료상점
			m_pControlInven->DrawItemInfo(m_pControlCashShop->m_pInvenOverItem, ITEM_INFO_INVEN);
		else if (pInvenOverItem)
			m_pControlInven->DrawItemInfo(pInvenOverItem, ITEM_INFO_INVEN);
		else if (pPetEquipOverItem)
			m_pControlInven->DrawItemInfo(pPetEquipOverItem, ITEM_INFO_INVEN);
		else if (pExchangeOverItem)
			m_pControlInven->DrawItemInfo(pExchangeOverItem, ITEM_INFO_EXCHANGE);
		else if (pInterchangeOverItem)
			m_pControlInven->DrawItemInfo(pInterchangeOverItem, ITEM_INFO_INTERCHANGE);
		else if (pProductOverItem)
			m_pControlInven->DrawItemInfo(pProductOverItem, ITEM_INFO_PRODUCT);
		else if(pPetProductOverItem)
			m_pControlInven->DrawItemInfo(pPetProductOverItem, ITEM_INFO_PETPRODUCT);
		else if (pBottomOverItem)
			m_pControlInven->DrawItemInfo(pBottomOverItem, ITEM_INFO_BOTTOM);
		else if (pShopOverItem)
			m_pControlInven->DrawItemInfo(pShopOverItem, ITEM_INFO_SHOP);
		else if (pWareOverItem)
			m_pControlInven->DrawItemInfo(pWareOverItem, ITEM_INFO_WAREHOUSE);
		else if (pUserShopOverItem) ///개인상점
			m_pControlInven->DrawItemInfo(pUserShopOverItem, ITEM_INFO_USERSHOP);
		else if(pGambleOverItem)
			m_pControlInven->DrawItemInfo(pGambleOverItem, ITEM_INFO_INTERCHANGE);

		else if(pLockSmithOverItem)
			m_pControlInven->DrawItemInfo(pLockSmithOverItem, ITEM_INFO_INTERCHANGE);
		else if(pEventAncientOverItem)
			m_pControlInven->DrawItemInfo(pEventAncientOverItem, ITEM_INFO_INTERCHANGE);
		else if(pPlatinumOverItem)
			m_pControlInven->DrawItemInfo(pPlatinumOverItem,ITEM_INFO_INTERCHANGE);	/// ITEM_INFO_PLATINUM 으로 변경


		else if(pSummerEventOverItem)
			m_pControlInven->DrawItemInfo(pSummerEventOverItem,ITEM_INFO_INTERCHANGE);
		else if(pRefineOverItem)
			m_pControlInven->DrawItemInfo(pRefineOverItem, ITEM_INFO_INTERCHANGE);
		else if(pUpgradeOverItem) ///신방어구강화시스템
			m_pControlInven->DrawItemInfo(pUpgradeOverItem, ITEM_INFO_INTERCHANGE);
		else if (pPieceShopOverItem)
			m_pControlInven->DrawItemInfo(pPieceShopOverItem, ITEM_INFO_PIECESHOP);
		else if(pProcessOverItem)
			m_pControlInven->DrawItemInfo(pProcessOverItem, ITEM_INFO_INTERCHANGE);

		if( m_pSpiritMode && m_bSpiritStart ) ///펫도움말 시에 위치옮김.
			m_pSpiritMode->Draw();

		if( m_bHelpPet && m_pControlHelpPet ) ///펫도움말
		{
			m_pControlHelpPet->FrameMove();		// 위치 순위에 주의!
			m_pControlHelpPet->Draw();		// 위치 순위에 주의!
		}

		if( m_bAutoMouse )//(m_bAutoMouse && m_bAutoMouse) ///07-04-12 숫자확인 시스템 타 인터페이스와 함께 쓸 수 있게 수정.
		{
//			m_pControlBottom->m_CurTextView = 13;//TEXTVIEW_EXP(챗팅창 버튼 플래그)
//			m_pControlBottom->m_bAct = TRUE; // 챗팅창 크기 플래그
			m_pAutoMouse->Draw();
		}

		if( m_pControlInven && !m_Popup )
			m_pControlInven->DrawExtra();

		if( !CheckFullScreenInterface() )
		{
			if( m_pControlSkill )
			{
				if (over_skill != SKILL_UNSELECTED)
					m_pControlSkill->DrawSkillInfo(over_skill);
				else if (over_skill2 != SKILL_UNSELECTED)
					m_pControlSkill->DrawSkillInfo(over_skill2);
			}
		}

		/// 퀵슬롯 풍선말을 그리는지 유무.DrawItemInfo을 하고 있다면 그리지 않는다.
		if( m_pControlBottom )
		{
			if (pBottomOverItem)
				m_pControlBottom->m_bDrawQuick = FALSE;
			else
				m_pControlBottom->m_bDrawQuick = TRUE;
		}

		if( m_bBossraidInfo && m_pBossraidInfo )
			m_pBossraidInfo->Draw();

		if( m_bHeroPrivilege && m_pHeroPrivilege )
			m_pHeroPrivilege->Draw();

		if( m_bHeroRankList && m_pHeroRankList )
			m_pHeroRankList->Draw();

		if( m_bShowNotice )	///공지팝업창
			m_pControlNotice->Draw();

		if( m_bShowPremiumChaMove )
			pPremiumChaMoveItem = m_pPremiumChaMove->Draw();

		if(pPremiumChaMoveItem)
			m_pControlInven->DrawItemInfo(pPremiumChaMoveItem, ITEM_INFO_INVEN);

//========================================================================
		if( m_Popup )		// nate 2006-02-28 : 가장 상위의 팝업만 그린다.
			m_Popup->pMsg_Popup.Draw();



		// 여기다 exchange item 정보를 보여주는 함수를 호출.
		if (m_pInfoBox && pCMyApp->m_pUIMgr->IsEventScenePlaying() != 1 ) // 이벤트 신 플레이 중엔 공지메세지 안뜨도록... ///인던1시 추가 ///튜토리얼시 조건 변경

		{
			DWORD curTime = timeGetTime();
			if (curTime > m_InfoTime + 500)
			{
				m_InfoTime = curTime;
				m_InfoFrame++;
			}
			if( m_InfoFrame % 3 )
				m_pInfoBox->Draw();

			if (m_InfoFrame > 20)
			{
				SAFE_DELETE(m_pInfoBox);
				if( m_InfoNextString.length() > 0)
				{
					AddChatStr((char*)m_InfoNextString.c_str(), 4);
					SetInfoBox((char*)m_InfoNextString.c_str(), false);
					m_InfoNextString = "";
				}
			}
		}
		if (m_pInfoBox2 && pCMyApp->m_pUIMgr->IsEventScenePlaying() != 1 ) // 이벤트 신 플레이 중엔 공지메세지 안뜨도록... ///인던1시 추가 ///튜토리얼시 조건 변경

		{
			DWORD curTime = timeGetTime();
			if (curTime > m_InfoTime2 + 500)
			{
				m_InfoTime2 = curTime;
				m_InfoFrame2++;
			}
			if( m_InfoFrame2 % 3 )
				m_pInfoBox2->Draw();

			if (m_InfoFrame2 > 20)
			{
				SAFE_DELETE(m_pInfoBox2);

				if( m_InfoNextString2.length() > 0)
				{
					AddChatStr((char*)m_InfoNextString2.c_str(), 4);
					SetInfoBox2((char*)m_InfoNextString2.c_str(), RGB(0, 0, 0), RGB(100, 100, 250));					
					m_InfoNextString2 = "";
				}
			}
		}
	}
	else if (m_GraphicMode == NKGRAPHICLOADING)
	{	
		if ( nSleep < 100 )
		{
			if( g_pDisplay )
				g_pDisplay->Clear(0);
			nSleep++;
		}
		else
		{
			if (m_pStartSheet->IsStarting() == START_GAME)
				pCMyApp->StartGame();			
		}
	}	

	BuffItemMgr::GetInstance()->Update();

	return TRUE;
}

void CNk2DFrame::SetInfoBox(char *msg, bool removeInfo2)
{
	int font_size = 16;
	int len;

	SAFE_DELETE(m_pInfoBox);
	if( removeInfo2 == true )
	{
		SAFE_DELETE(m_pInfoBox2);
	}	

	if ((len = strlen(msg)) <= 0)
		return;

	m_pInfoBox = new CTextOutBox;
	if( m_pInfoBox )
	{
		m_pInfoBox->Init(font_size, RGB(0, 0, 0), (GetClientWidth()-len*(font_size/2)) / 2, 100);
		m_pInfoBox->SetString_withOutLine( msg, RGB(255, 255, 255), RGB(100, 100, 250) ); // 인자 : 글, 글자색, 외곽선 색
	}
	m_InfoTime = timeGetTime();
	m_InfoFrame = 0;

	m_InfoNextString = "";
}

void CNk2DFrame::SetInfoBox2(char *msg, COLORREF fontColor, COLORREF outlineColor) // 매트릭스 지휘관 입장 출력용. (운영자 공지와는 위치와 색상이 다르다.) (by 원석)
{
	int font_size = 16;
	int len;

	SAFE_DELETE(m_pInfoBox2);	

	if ((len = strlen(msg)) <= 0)
		return;

	m_pInfoBox2 = new CTextOutBox;
	if( m_pInfoBox2 )
	{
		m_pInfoBox2->Init(font_size, fontColor, (GetClientWidth()-len*(font_size/2)) / 2, 120);
		m_pInfoBox2->SetString_withOutLine( msg, RGB(255, 255, 255), outlineColor ); // 인자 : 글, 글자색, 외곽선 색
	}
	m_InfoTime2 = timeGetTime();
	m_InfoFrame2 = 0;

	m_InfoNextString2 = "";
}

void CNk2DFrame::SetInfoBox_NextString(const std::string& strMsg)
{
	m_InfoNextString = strMsg;
}

void CNk2DFrame::SetInfoBox_NextString2(const std::string& strMsg)
{
	m_InfoNextString2 = strMsg;
}

//---------------------------------------------------------------------
// AddChatStr()
//---------------------------------------------------------------------
void CNk2DFrame::AddChatStr(char *str, int say_type)
{
	if( m_pControlBottom )
		m_pControlBottom->AddStrQlist(str, say_type);
}

void CNk2DFrame::AddChatStr(int index, int say_type)
{
	// index 비교!!!
	AddChatStr((char*)G_STRING(index), say_type);
}

void CNk2DFrame::AddChatStr_ExtraInfo(char* str, int say_type, int infoType, int infoVnum)
{
	if( m_pControlBottom )
		m_pControlBottom->AddStrQlist_ExtraInfo(str, say_type, infoType, infoVnum);
}

void CNk2DFrame::AddChatShoutStr(char* str)
{
	if( m_pControlBottom )
		m_pControlBottom->AddStrShoutList(str);
}

void CNk2DFrame::SetNkCha(CNkCharacter *pNkCha)
{
	if( m_pControlBottom )		m_pControlBottom->SetNkCha(pNkCha);
	if( m_pControlShop )		m_pControlShop->SetNkCha(pNkCha);
	if( m_pControlExchange )	m_pControlExchange->SetNkCha(pNkCha);
	if( m_pControlWareHouse )	m_pControlWareHouse->SetNkCha(pNkCha);
	if( m_pControlUserShop )	m_pControlUserShop->SetNkCha(pNkCha); ///개인상점
	if( m_pInterchange )		m_pInterchange->SetNkCha(pNkCha);
	if( m_pProduct )			m_pProduct->SetNkCha(pNkCha);
	if( m_pPetProduct )			m_pPetProduct->SetNkCha(pNkCha);
	if( m_pControlInven )		m_pControlInven->SetNkCha(pNkCha);
	if( m_pInterface[SCORE] )	g_pRoh->SetScoreWnd((CControlScore*)m_pInterface[SCORE]);
	if( m_pInterface[SCORE] )	((CControlScore*)m_pInterface[SCORE])->SetAllScore();
	if( m_pControlSkill )		m_pControlSkill->SetNkCha(pNkCha);
	if( m_pControlPeiceShop )	m_pControlPeiceShop->SetNkCha(pNkCha);

	{
		if( m_pControlGStoreHouse)	m_pControlGStoreHouse->SetNkCha(pNkCha);
	}

	m_pNkCha = pNkCha;

}



void CNk2DFrame::ShowControlSelMenu(BOOL bShow )
{
	if( !CloseAllWindow(FALSE) )
		return;

	if( m_pControlSelMenu )
		m_bShowControlSelMenu = bShow;

	if( bShow)
		pCMyApp->StopHero();
}

BOOL CNk2DFrame::ControlSelmenuSetMetuList(int type, char * arg )
{
	return m_pControlSelMenu->SetMenuList(type, arg);
}
void CNk2DFrame::ControlSelmenuAddStart()
{
	m_pControlSelMenu->AddMenuStart();
}

void CNk2DFrame::ControlSelmenuAddPush(int vnum)
{
	m_pControlSelMenu->AddMenuPush(vnum);
}

void CNk2DFrame::ControlSelmenuAddEnd()
{
	m_pControlSelMenu->AddMenuEnd();
}


void CNk2DFrame::ShowLetterWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( m_pControlLetter )
	{
		m_pControlLetter->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,1);
		m_pControlLetter->PutList();
	}

	if( bShow )
	{
		m_bLetter = bShow; // 필요한 것만 킨다.

		if( m_pControlInven )
			m_pControlInven->ResetItemInfo();

		if( m_pControlLetter )
			m_pControlLetter->m_bActive = true;
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}

void CNk2DFrame::ShowShopWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( GetControlShop() )
		GetControlShop()->SetLpShopFlag(FALSE);

	if( bShow )
	{
		m_bInven = TRUE;
		m_bShop = bShow;

		if( m_pControlInven )
			m_pControlInven->ResetItemInfo();

		pCMyApp->StopHero();
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}


void CNk2DFrame::ShowPeiceShopWindow (BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		if ( m_pControlPeiceShop )
			m_pControlPeiceShop->LoadShopItems(); // 진화 작업을 위해 아이템 등록 타이밍을 인터페이스 열때로 옮김.

		m_bPetStatus = TRUE;
		m_bPeiceShop = bShow;

		if( m_pControlInven )
			m_pControlInven->ResetItemInfo();

		pCMyApp->StopHero();
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}


void CNk2DFrame::ShowWareHouseWindow(BOOL bShow)  ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	// 실제  창고 물품 리스트를 다 보여준다음에 창고 이미지를 보여준다.
	//m_bShowWareHouse = bShow; // (이거 인터페이스 클로즈 처리 함수화 중에 주석단거 아님.)
	if (bShow)
	{
		m_bInven = TRUE;
		m_pControlWareHouse->ActivateWareHouse(TRUE);

		pCMyApp->StopHero();
	}
	else
	{
	}
}

void CNk2DFrame::ShowGStoreHouseWindow(BOOL bShow)
{
	if( !CloseAllWindow(FALSE) )
		return;

	if (bShow)
	{
		m_bInven = TRUE;
		m_pControlGStoreHouse->ActivateGStoreHouse(TRUE, g_pRoh->GetGuildLevel());
		pCMyApp->StopHero();
	}
	else
	{
	}
}

void CNk2DFrame::ToggleGambleWindow(int type) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if (m_pGamble && m_pGamble->IsOnGamble() == TRUE)
		return;
	if(Check_All_MsgPopUp())
		return;

	ShowGambleWindow(!m_bShowGamble);
	if(IsGambleVisible())
	{
		SAFE_DELETE_RES(m_pGamble);
		ShowGambleWindow(false);

		m_pGamble->LoadRes(type, m_pItemRender);
		m_pGamble->SetNkCha(m_pNkCha);
		ShowGambleWindow(true);
		m_pGamble->m_nMoney = g_pRoh->m_Money;

		pCMyApp->StopHero();
	}
	else
	{
		if(m_pGamble)
			if(m_pGamble->m_bReceiveMsg)
				m_pGamble->ForcingCloseGamble();
	}
}

void CNk2DFrame::ToggleLockSmithWindow(void) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if (m_pLockSmith && IsLockSmith() )
		return;

	if(Check_All_MsgPopUp())
		return;

	ShowLockSmithWindow(!m_bLockSmith);
	if( IsLockSmith() )
	{
		SAFE_DELETE_RES(m_pLockSmith);
		ShowLockSmithWindow(false);

		m_pLockSmith->LoadRes( m_pItemRender);
		ShowLockSmithWindow(true);

		pCMyApp->StopHero();
	}
}

void CNk2DFrame::ShowLockSmithWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
// 갬블 윈도우를 보여준다.
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bLockSmith = bShow;
	}
	else
	{
		m_bInven = FALSE;
		m_bLockSmith = bShow;
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}


//0120
void CNk2DFrame::ToggleEventAncientWindow(int type)
{
	if (m_pGamble && m_pGamble->IsOnGamble() == TRUE)
		return;

	if(Check_All_MsgPopUp())
		return;

	ShowEventAncientWindow(!m_bShowEventAncient);
	if(IsEventAncientVisible())
	{
		SAFE_DELETE_RES(m_pEventAncient);
		ShowEventAncientWindow(false);

		m_pEventAncient->LoadRes(type, m_pItemRender);
		m_pEventAncient->SetNkCha(m_pNkCha);
		ShowEventAncientWindow(true);

		pCMyApp->StopHero();
	}
}

void CNk2DFrame::ToggleRefineWindow(int type)
{
	if (m_pGamble && m_pGamble->IsOnGamble() == TRUE)
		return;

	if(Check_All_MsgPopUp())
		return;

	ShowRefineWindow(!m_bShowRefine);
	if(IsRefineVisible())
	{
		SAFE_DELETE_RES(m_pRefine);
		ShowRefineWindow(false);
		m_pRefine->LoadRes(type, m_pItemRender);
		m_pRefine->SetNkCha(m_pNkCha);
		ShowRefineWindow(true);

		pCMyApp->StopHero();
	}
	else
	{
		if(m_pRefine)
		{
			if (m_pRefine->IsRefineConnection())
				m_pRefine->CloseConnection();
		}
	}
}

//  nate
void CNk2DFrame::ShowGambleWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
// 갬블 윈도우를 보여준다.
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bShowGamble = bShow;

		pCMyApp->StopHero();
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}

//0120
void CNk2DFrame::ShowEventAncientWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bShowEventAncient = bShow;

		pCMyApp->StopHero();
	}
	else // 닫을때 07-01-15 추가
		pCMyApp->ConvertImmStatus(); // 이걸해줘야 조합중 닫았을때 풀어주는 듯.(이거 안해주면 일본 전각문자 입력중등시에 문제가 생김)

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}

// nate
void CNk2DFrame::ShowRefineWindow(BOOL bShow, int type) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
// 정재창 뛰우기


{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	// 정제창이 떠있는지 확인
	if( bShow )
	{
		if( m_pRefine )
		{
			m_pRefine->LoadRes(type, m_pItemRender);
			m_pRefine->SetNkCha(m_pNkCha);

			pCMyApp->StopHero();
		}

		m_bInven = TRUE;
		m_bShowRefine = bShow;

	}
	else
	{
		if(m_pRefine)
		{
			if (m_pRefine->IsRefineConnection())
				m_pRefine->CloseConnection();

			SAFE_DELETE_RES(m_pRefine);
		}
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}

void CNk2DFrame::SetShowWareHouseFlag(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bShowWareHouse = bShow;
		m_bInven = TRUE;

		pCMyApp->StopHero();
	}
}


void CNk2DFrame::SetShowGStoreHouseFlag(BOOL bShow)
{
	if( !CloseAllWindow(FALSE) )
		return;

	if( bShow )
	{
		m_bShowGStoreHouse = bShow;
		m_bInven = TRUE;

		pCMyApp->StopHero();
	}
}

void CNk2DFrame::ShowExchangeWindow(BOOL bShow, BOOL bExch) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;				// shop과 inven의 순서가 중요하다.
		m_bShowExchange = bShow;
	}

	if (m_bShowExchange && bExch == TRUE)
		m_pControlExchange->ActivateExch(bExch);
	else if (m_bShowExchange == FALSE && bExch == FALSE)
		m_pControlExchange->ActivateExch(FALSE);
	else if (m_bShowExchange == FALSE && bExch == TRUE)
		return;
	else
		m_pControlExchange->ActivateExch(FALSE);
}

void CNk2DFrame::ShowInterchangeWindow(BOOL bShow, BOOL bInter) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;				// shop과 inven의 순서가 중요하다.
		m_bInterchange = bShow;

		pCMyApp->StopHero();
	}

	if (m_bInterchange && bInter)
		m_pInterchange->ActivateExch(TRUE);
	else
		m_pInterchange->ActivateExch(FALSE);
}


void CNk2DFrame::ShowProductWindow(BOOL bShow, BOOL bAct) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;				// shop과 inven의 순서가 중요하다.
		m_bProduct = bShow;

		pCMyApp->StopHero();
	}

	if (m_bProduct && bAct)
		m_pProduct->ActivateExch(TRUE);
	else
		m_pProduct->ActivateExch(FALSE);
}

void CNk2DFrame::ShowInvenWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = bShow;
	}
}


void CNk2DFrame::ShowInterfaceWindow(bool bShow, int nNum, int nCurWorldNum, int nSlot_num) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
// by evilkiki 2009.11.05 길드랭킹전 : 게이트 포탈 안열리도록
	if( g_SvrType == ST_GUILD_RANK && nNum == GATE )
		return;

	if( !CloseAllWindow(FALSE, (nNum==GATE?true:false)) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		if( nNum == GATE )
		{
			((CControlGate*)m_pInterface[GATE])->SetCurWorld(nCurWorldNum);
		}
		if( nNum == DOCUMENT )
		{
			((CControlDocument*)m_pInterface[DOCUMENT])->SetDocument(nCurWorldNum);
			m_iQuicSlotNum = nSlot_num;
		}

		if( nNum == MAP || nNum == QUEST || nNum == SCORE || nNum == YUTBOARD || nNum == FISHING || nNum == FISHING_REWARD ||
			nNum == LOTTO || nNum == BINGO || nNum == LETTER_WINDOW || nNum == LETTER_SEND || nNum == LETTER_RECV || 
			nNum == BASEBALL || nNum == BASEBALL_REWARD || nNum == LOTTERY ||
			nNum == DICEGAME || nNum == DICEGAME_REWARD)
		{
		}
		else
		{
			pCMyApp->StopHero();
		}
	}
	m_bInterface[nNum] = bShow;
}

void CNk2DFrame::SetVisibleInterface(bool bVisible, int nNum)
{
	m_bInterface[nNum] = bVisible;
}

void CNk2DFrame::ShowPetStatusWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bPetStatus = bShow;
	}
}


void CNk2DFrame::ShowSkillWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bSkill = bShow;
	}
}

void CNk2DFrame::ShowRoyaleWindow(BOOL bShow) ///ภฮลอฦไภฬฝบ ลฌทฮม๎ รณธฎ วิผ๖ศญ(05-09-30 ฟ๘ผฎ)
{
	if( !CloseAllWindow(FALSE) ) // ลฌทฮม๎ ฝวฦะฝรฟฃ ฟญธฎม๖ พสฐิ รณธฎ. (05-11-19 ฟ๘ผฎ) // ภฯดภบ ภบฮ ดภบ ศฤ. (บไฦ๗ฦฎ ฐปฝล วรทกฑืธฆ FALSEทฮ มฟก ภฏภว. บไฦ๗ฦฎ ฐปฝลภบ ภฬ วิผ๖ ธถม๖ธทฟก วุมึพ๎พ฿วัดู.)
		return;

	if( bShow )
	{
		m_pControlRoyale->curReward = 0;
		m_bRoyale = bShow;
	}
}

void CNk2DFrame::ShowHelpWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bHelp = bShow;
	}
	else
	{
		m_pControlHelp->SelectDeleteRes();
	}
}

void CNk2DFrame::ShowPartyWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bParty = bShow;
	}
}

static char * spChar_jpn = "`ⅰ∀ㄵ├ŁかЛ?갚껙뉘떱많본섹씩읒쩠컵판훔墾勁卦今錄桐虜妹膊覆?@";

static bool IsTowByte(char str)
{
	bool bTwobyte = false;

	// ASCII ..
	if((unsigned char)str  >= 0x20 && (unsigned char)str  <=0x7E)
	{
		bTwobyte = false;
	}
	// 2BYTE STRING (HANJA first byte / kata(JUN GAK) / hira)
	else if((unsigned char)str  >= 0x81 && (unsigned char)str  <=0x9F)
	{
		bTwobyte = true;
	}
	// 2BYTE STING (HANJA first byte)
	else if((unsigned char)str  >= 0xE0 && (unsigned char)str  <=0xEF)
	{
		bTwobyte = true;
	}
	// 1BYTE STRING
	else
		bTwobyte = false;

	return bTwobyte;
}


static char *ExamSpChar_for_jpn(char *str)
{
	static char found_sp[3];
	found_sp[0] = '\0';

	int len = strlen(str);
	int splen;

	splen = strlen(spChar_jpn);

	bool input_str_byte = false;
	bool list_str_byte = false;
	int z;
	for (int pos = 0; pos < len; pos++)
	{
		input_str_byte = IsTowByte(str[pos]);

		for (int sp_pos = 0; sp_pos < splen; sp_pos++)
		{
			list_str_byte = IsTowByte(spChar_jpn[sp_pos]);

			if(input_str_byte == list_str_byte)
			{

				list_str_byte ? z = 2 : z = 1;

				if (!strncmp(str+pos, spChar_jpn+sp_pos, z))
				{

					if(!list_str_byte)
					{
						found_sp[0] = spChar_jpn[sp_pos];
						found_sp[1] = '\0';
					}
					else
					{
						found_sp[0] = spChar_jpn[sp_pos];
						found_sp[1] = spChar_jpn[sp_pos+1];
						found_sp[2] = '\0';
					}

					return found_sp;
				}
			}
			if(list_str_byte)
				sp_pos++;

		}
		if(input_str_byte)
			pos++;
	}
	return NULL;

}


LRESULT CNk2DFrame::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL bDblClk)
{
	if (!m_bAllLoaded)
		return 0;
	
	if (m_GraphicMode == NKGRAPHIC2D && m_pStartSheet)
	{
		m_pStartSheet->MsgProc(hWnd, msg, wParam, lParam);
		if (m_pStartSheet->IsStarting() == START_GAME)
		{
			m_GraphicMode = NKGRAPHICLOADING;
		}
	}
	else if (m_GraphicMode == NKGRAPHIC3D )
	{
		if( g_dwClientCountry == CTRY_BRA && m_pAutoNotice ) // 브라질 요청 : 10분 마다 공지 메시지 출력
			m_pAutoNotice->MsgProc();

		if( m_Popup )
		{
			if( m_force_exit )
			{
				DWORD tick = GetTickCount();
				if( m_force_exit_tick == 0 )
					m_force_exit_tick = tick;

				if( m_force_exit_tick + 10000 < tick )
				{
					exit(1);
					return 1;
				}
			}
			return PopupMsgProc(hWnd, msg, wParam, lParam);
		}

		if( m_pPremiumChaMove && m_bShowPremiumChaMove )
			return m_pPremiumChaMove->MsgProc( hWnd, msg, wParam, lParam );

		if (m_bHelpPet && m_pControlHelpPet )	///펫도움말 우선순위가 높다.
		{
			if (bDblClk)
				msg = WM_LBUTTONDBLCLK;
			int ret = m_pControlHelpPet->MsgProc(hWnd, msg, wParam, lParam);
			if(bDblClk)
				msg = WM_LBUTTONDOWN;
			if(ret)
				return 1;
		}

		if (m_pPremiumExchange && m_bShowPremiumExchange)
		{
			if (m_pPremiumExchange->MsgProc(hWnd, msg, wParam, lParam))
				return 1;
		}


		if( m_bShowNotice && m_pControlNotice ) ///공지팝업창
		{
			if( m_pControlNotice->MsgProc( hWnd, msg, wParam, lParam ) )
				return 1;
		}

		if ( m_bShowCashShop && m_pControlCashShop ) ///유료상점 은 인터페이스 독점모드 방식으로 출력되야한다.
			return m_pControlCashShop->MsgProc(hWnd, msg, wParam, lParam); // 유료상점만 키고 끊어버린다.우선 순위에 주의할것. ControlBottom  qhekeh dntjsehldigksek.

		if ( m_bShowCashShopIntro && m_pCashShopIntro ) ///유료상점 은 인터페이스 독점모드 방식으로 출력되야한다.
			return m_pCashShopIntro->MsgProc(hWnd, msg, wParam, lParam); // 유료상점만 키고 끊어버린다.우선 순위에 주의할것. ControlBottom  qhekeh dntjsehldigksek.

		if (m_bInterface[GATE] && m_pInterface[GATE] && m_pInterface[GATE]->MsgProc(hWnd, msg, wParam, lParam))
		{
			if( g_dwClientCountry == CTRY_JPN )
			{
				return 0;
			}
			else
			{
				// tick 을 넣자!
				return 1;
			}
		}

		if( GetGate() && GetGate()->IsActivate() )
		{
			if( 2 == GetGate()->MsgProc(hWnd , msg, wParam ,lParam) )
			{
				ShowGateWindow(false,-1,false);
			}
			return 1;
		}


		{
			if (m_bShowControlGuild && m_pControlGuild && m_pControlGuild->MsgProc(hWnd, msg, wParam, lParam))
				return 1;

			if (m_bShowControlGuildManage && m_pControlGuildManager && m_pControlGuildManager->MsgProc(hWnd, msg, wParam, lParam))
				return 1;
		}

		if ( (m_bViewTop || m_bInven || m_bInterface[SCORE]
				|| m_bInterface[MAP] || m_bSkill || m_bParty
				|| m_bHelp
				|| m_bRoyale
				|| m_bPetStatus
				|| m_bInterface[QUEST]
				|| m_bInterface[DOCUMENT]
				|| m_bHelper
				|| m_bShowControlGuild
				|| m_bShowControlGuildManage
				|| m_bHeroFinalRank
			 )
				&& !(m_bShowPVPWidget)
		   )
		{
			// 게임내에 사용하는 메세지창중 하나라도 있는경우 "탑메뉴"는 사용할 수 없다
			if(Check_All_MsgPopUp() == MSGPOPUP_NOT_EXIST && pCMyApp->m_pUIMgr && !pCMyApp->m_pUIMgr->m_bIsDelivBox)
			{
				if (m_pControlTop && m_pControlTop->MsgProc(hWnd, msg, wParam, lParam))
				{
					return 1;
				}
			}
		}

		if( m_pControlSelMenu && m_bShowControlSelMenu )
		{
			if( m_pControlSelMenu->MsgProc(hWnd, msg, wParam, lParam ) )
				return 1;
			else
				return 0;
		}


		if( m_pBossraidInfo && m_bBossraidInfo )
		{
			if( m_pBossraidInfo->MsgProc(hWnd, msg, wParam, lParam ) )
				return 1;
		}

		if( m_pHeroPrivilege && m_bHeroPrivilege )
		{
			if( m_pHeroPrivilege->MsgProc(hWnd, msg, wParam, lParam ) )
				return 1;
		}

		if( m_pHeroRankList && m_bHeroRankList )
		{
			if( m_pHeroRankList->MsgProc(hWnd, msg, wParam, lParam ) )
				return 1;
		}

		if (m_bLetter && m_pControlLetter->MsgProc(hWnd, msg, wParam, lParam))
			return 1;

		if (m_bMenu && m_pControlMenu)
		{
			if (m_pControlMenu->MsgProc(hWnd, msg, wParam,lParam))
				return 1;
		}
		if (m_bRShift && m_pRShiftMenu)
		{
			if (m_pRShiftMenu->MsgProc(hWnd, msg, wParam,lParam))
				return 1;
		}
		if (m_bLShift && m_pLShiftMenu)
		{
			if(m_pLShiftMenu->MsgProc(hWnd, msg, wParam, lParam))
				return 1;
		}
		if(m_bAutoMouse && m_pAutoMouse)
		{
			if(m_pAutoMouse->MsgProc(hWnd,msg,wParam,lParam))
				return 1;
		}
		if (m_bPetLevel && m_pPet_Level)
		{
			if (m_pPet_Level->MsgProc(hWnd, msg, wParam,lParam))
				return 1;
		}
		if ( m_bShowCashShop && m_pControlCashShop && m_pControlCashShop->MsgProc(hWnd, msg, wParam, lParam) ) ///유료상점
			return 1;
		if ( m_bShowCashShopIntro && m_pCashShopIntro && m_pCashShopIntro->MsgProc(hWnd, msg, wParam, lParam) ) ///유료상점
			return 1;
		if (m_bShop && m_pControlShop)
		{
			if (bDblClk)
				msg = WM_LBUTTONDBLCLK;
			int ret = m_pControlShop->MsgProc(hWnd, msg, wParam, lParam);
			if (bDblClk)
				msg = WM_LBUTTONDOWN;
			if (ret)
				return 1;
		}
		if (m_bPeiceShop && m_pControlPeiceShop)
		{
			if (bDblClk)
				msg = WM_LBUTTONDBLCLK;

			int ret = m_pControlPeiceShop->MsgProc(hWnd, msg, wParam, lParam);

			if (bDblClk)
				msg = WM_LBUTTONDOWN;

			if(ret)
				return 1;
		}
		if (m_bProduct && m_pProduct)
		{
			if (bDblClk)
				msg = WM_LBUTTONDBLCLK;

			int ret = m_pProduct->MsgProc(hWnd, msg, wParam, lParam);

			if (bDblClk)
				msg = WM_LBUTTONDOWN;

			if (ret)
				return 1;
		}
		if(m_bPetProduct && m_pPetProduct)
		{
			if(bDblClk)
				msg = WM_LBUTTONDBLCLK;
			int ret = m_pPetProduct->MsgProc(hWnd, msg, wParam, lParam);
			if(bDblClk)
				msg = WM_LBUTTONDOWN;
			if(ret)
				return 1;
		}
		if(m_bShowGamble && m_pGamble)
		{
			int ret = m_pGamble->MsgProc(hWnd, msg, wParam, lParam);
			if (ret)
				return 1;
		}

		if(m_bLockSmith && m_pLockSmith)
		{
			if( m_pLockSmith->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}
		if(m_bShowEventAncient && m_pEventAncient)
		{
			if( m_pEventAncient->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}
		if(m_bShowPlatinum && m_pPlatinum)
		{
			if( m_pPlatinum->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}
		if(m_bShowSummerEvent && m_pSummerEvent)
		{
			if( m_pSummerEvent->MsgProc(hWnd,msg,wParam,lParam) )
				return 1;
		}
		if (m_bInterface[QUEST] && m_pInterface[QUEST] && m_pInterface[QUEST]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bPetStatus && m_pControlPetStatus && m_pControlPetStatus->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if(m_bShowRefine && m_pRefine)
		{
			if( m_pRefine->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}
		if( m_bShowUpgrade && m_pUpgrade ) ///신방어구강화시스템
		{
			if( m_pUpgrade->MsgProc( hWnd, msg, wParam, lParam ) )
				return 1;
		}
		if( m_bShowUserShopSearch && m_pUserShopSearch ) ///개인상점검색


		{
			if( m_pUserShopSearch->MsgProc( hWnd, msg, wParam, lParam ) )
				return 1;
		}
		if(m_bShowProcess && m_pProcess)	//가공


		{
			if( m_pProcess->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}
		if (m_bShowExchange && m_pControlInven && (!m_pControlInven->m_plus5Message.m_bActive))
		{
			if( m_pControlExchange->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}
		if (m_bShowWareHouse && m_pControlInven && (!m_pControlInven->m_plus5Message.m_bActive))
		{

			if( m_pControlWareHouse->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}

		if (m_bShowGStoreHouse && m_pControlInven && (!m_pControlInven->m_plus5Message.m_bActive))
		{
			if( m_pControlGStoreHouse->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}

		if( m_pHeroFinalRank && m_bHeroFinalRank )
		{
			if( m_pHeroFinalRank->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}

		if (m_bShowUserShop && m_pControlInven /*&& (!m_pControlInven->m_plus5Message.m_bActive && !m_pControlInven->m_MsgPopUp.m_bActive)*/) ///개인상점
		{
			if (bDblClk)
				msg = WM_LBUTTONDBLCLK;
			int ret = m_pControlUserShop->MsgProc(hWnd, msg, wParam, lParam);
			if (bDblClk)
				msg = WM_LBUTTONDOWN;
			if (ret)
				return 1;
		}
		if (m_bInterchange && m_pControlInven)
		{
			if (bDblClk)
				msg = WM_LBUTTONDBLCLK;


			if(!m_pControlInven->m_plus5Message.m_bActive)
			{
				int ret = m_pInterchange->MsgProc(hWnd, msg, wParam, lParam);
				if (bDblClk)
					msg = WM_LBUTTONDOWN;
				if (ret)
					return 1;
			}
		}
				
		
		if(m_pUIWindow)
		{
			if( m_pUIWindow->WindowMsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}		

		if( g_SvrType == ST_ADULT_ONLY )
		{
			if(m_pTgaUIWindow && m_pTgaUIWindow->WindowMsgProc(hWnd, msg, wParam, lParam))
				return 1;
		}


		if (m_pControlInven && m_pControlInven->MsgProc(hWnd, msg, wParam, lParam, m_bInven
				, m_bInven || m_bInterface[SCORE] || m_bRoyale || m_bInterface[MAP] || m_bSkill || m_bParty || m_bHelp || m_bPetStatus || m_bInterface[QUEST] || m_bInterface[DOCUMENT] || m_bHelper
				|| m_bShowControlGuild
				|| m_bShowControlGuildManage
													   ))
			return 1;

		if(m_bRoyale && m_pControlRoyale->MsgProc(hWnd, msg, wParam, lParam))
			return 1;

		if (m_bInterface[SCORE] && m_pInterface[SCORE] && m_pInterface[SCORE]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bPetStatus && m_pControlPetStatus && m_pControlPetStatus->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[SOCIAL] && m_pInterface[SOCIAL] && m_pInterface[SOCIAL]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;

		if (m_bInterface[MAP]&& m_pInterface[MAP] && m_pInterface[MAP]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[DOCUMENT] && m_pInterface[DOCUMENT] && m_pInterface[DOCUMENT]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[YUTBOARD] && m_pInterface[YUTBOARD] && m_pInterface[YUTBOARD]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[FISHING] && m_pInterface[FISHING] && m_pInterface[FISHING]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[FISHING_REWARD] && m_pInterface[FISHING_REWARD] && m_pInterface[FISHING_REWARD]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[BASEBALL] && m_pInterface[BASEBALL] && m_pInterface[BASEBALL]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[BASEBALL_REWARD] && m_pInterface[BASEBALL_REWARD] && m_pInterface[BASEBALL_REWARD]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[LOTTO] && m_pInterface[LOTTO] && m_pInterface[LOTTO]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[BINGO] && m_pInterface[BINGO] && m_pInterface[BINGO]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;		
		if (m_bInterface[LETTER_SEND] && m_pInterface[LETTER_SEND] && m_pInterface[LETTER_SEND]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[LETTER_RECV] && m_pInterface[LETTER_RECV] && m_pInterface[LETTER_RECV]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[LOTTERY] && m_pInterface[LOTTERY] && m_pInterface[LOTTERY]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[DICEGAME] && m_pInterface[DICEGAME] && m_pInterface[DICEGAME]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[DICEGAME_REWARD] && m_pInterface[DICEGAME_REWARD] && m_pInterface[DICEGAME_REWARD]->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bInterface[LETTER_WINDOW] && m_pInterface[LETTER_WINDOW] )
		{
			if(bDblClk)
				msg = WM_LBUTTONDBLCLK;
			int ret = m_pInterface[LETTER_WINDOW]->MsgProc(hWnd, msg, wParam, lParam);
			if(bDblClk)
				msg = WM_LBUTTONDOWN;
			if( ret )
				return 1;
		}
		if (m_bSkill && m_pControlSkill && m_pControlSkill->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bHelp && m_pControlHelp && m_pControlHelp->MsgProc(hWnd, msg, wParam, lParam))
			return 1;
		if (m_bParty && m_pControlParty && m_pControlParty->MsgProc(hWnd, msg, wParam, lParam))
			return 1;

		if (m_bHelper && m_pControlHelper )	//후견인
		{
			if (bDblClk)
				msg = WM_LBUTTONDBLCLK;
			int ret = m_pControlHelper->MsgProc(hWnd, msg, wParam, lParam);
			if(bDblClk)
				msg = WM_LBUTTONDOWN;
			if(ret)
				return 1;
		}

		if( m_pSpiritMode && m_bSpiritStart )
		{
			if( m_pSpiritMode->MsgProc(hWnd,msg,wParam,lParam) )
				return 1;
		}
//		if (m_bQuest && m_pQuestData && m_pQuestData->MsgProc(hWnd, msg, wParam, lParam))
//			return 1;


		if (m_pControlBottom && m_pControlBottom->MsgProc(hWnd, msg, wParam, lParam))
			return 1;

		if (m_ui_takebox )
		{
			int rv = m_ui_takebox->MsgProc(hWnd, msg, wParam, lParam);
			if( rv )
			{
				if( rv == 2 )
				{
					ShowTakeboxWindow(false,0);
					return 1;
				}
				else
				{
					return 1;
				}
			}

			return 0;
		}

		if (m_ui_friend && m_ui_friend->IsVisible())
		{
			if( m_ui_friend->MsgProc(hWnd, msg, wParam, lParam) )
				return 1;
		}

		if( m_ui_buffitem &&
				m_ui_buffitem->MsgProc(hWnd, msg, wParam, lParam) )
		{
			return 1;
		}

	}

	return 0L;
}

#include "resource.h"

void CNk2DFrame::SendTickTime()
{
	static DWORD lastTickTime = 0;
//	static DWORD lastTickTime2 = 0; //desativado
	DWORD curTickTime = timeGetTime();
/*	if (curTickTime >= lastTickTime2 + 10000) //desativado
	{
		// 1บะฟก วัน๘พฟ บธณปบฮภฺ!!
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage("vfy\n");

		lastTickTime2 = curTickTime;
	}*/
	if (curTickTime >= lastTickTime + 60000)
	{
		// 1분에 한번씩 보내부자!!
		if (g_pRoh)
		{
			if (g_pRoh->m_Wearing[WEARING_WEAPON])
			{
				CItem *pItem = g_pRoh->m_Wearing[WEARING_WEAPON];
				sprintf(commOutBuf, "pulse %d, %d, %d, %d, %d\n", curTickTime/1000, pItem->m_Vnum, pItem->m_Index, pItem->m_Speed, GET_ATT_RANGE(pItem));
			}
			else
				sprintf(commOutBuf, "pulse %d, -1, 0, 0, 0\n", curTickTime/1000);
		}
		else
			sprintf(commOutBuf, "pulse %d, -1, 0, 0, 0\n", curTickTime/1000);

		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(commOutBuf);

		if( g_pTcpIp && 
			pCMyApp->m_pUIMgr->m_AutoPlay &&
			pCMyApp->m_pUIMgr->m_bAutoHunt )
		{
			g_pTcpIp->SendNetMessage( "lr check\n" );
		}

		lastTickTime = curTickTime;
	}
}

void CNk2DFrame::DeleteVoiceChat()
{
}

// nate
BOOL CNk2DFrame::Check_All_MsgPopUp()
// 현재 셋팅되어있는 메세지 박스가 있는지 확인
{
	if( m_Popup )
		return MSGPOPUP_EXIST;

	// 메세지 창이 떠 있을 경우

	if( m_pControlInven && m_pControlLetter )
	{
		if(	m_pControlInven->m_plus5Message.m_bActive ||
				m_pControlLetter->m_bActive ||
				pCMyApp->m_MsgPopUp.m_bActive )
			return MSGPOPUP_EXIST;
	}

	return MSGPOPUP_NOT_EXIST;
}


BOOL CNk2DFrame::All_Kill_MsgPopUp()
{
	if( m_pControlInven )
	{
		if(m_pControlInven->m_plus5Message.m_bActive)
			m_pControlInven->m_plus5Message.m_bActive = FALSE;
		if(pCMyApp->m_MsgPopUp.m_bActive)
			pCMyApp->m_MsgPopUp.m_bActive = FALSE;
	}

	DeleteAllPopup();
	return TRUE;
}



void CNk2DFrame::ShowRShiftMenu(CNkCharacter *pCha)
{
//	if(m_bAutoMouse) ///07-04-12 숫자확인 시스템 타 인터페이스와 함께 쓸 수 있게 수정.
//		return;
	HRESULT hr;
	D3DVIEWPORT7 vp;

	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;

	if (pCha != NULL)
	{
		m_bRShift = TRUE;

		vWorld = D3DVECTOR(pCha->m_wx, pCha->m_wy, pCha->m_wz);

		// 화면 좌표 계산.
		pCMyApp->GetLandPtr()->GetPipelineMatrix(matPipe);
		D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

		if (m_pRShiftMenu)
		{
			if (FAILED(hr = pCMyApp->GetDevice()->GetViewport( &vp ))) return ;

			// viewport의 크기 확인
			int cx, cy;
			cx =230, cy = 67; // 06-04-19 cx의 크기를 185에서 230으로 증가. 버튼이 한칸 추가되었음.
			vScr.x = vScr.x - 230/2;

			if((vScr.x+cx) > (vp.dwX+vp.dwWidth))
				vScr.x = vp.dwX+vp.dwWidth-cx;
			if(vScr.x <0)
				vScr.x = 0;
			if(vScr.y < 0)
				vScr.y = 0;
			if((vScr.y+cy) > (vp.dwY+vp.dwHeight))
				vScr.y = vp.dwY+vp.dwHeight-cy;

			m_pRShiftMenu->SetXPos(vScr.x);
			m_pRShiftMenu->SetYPos(vScr.y);
			m_pRShiftMenu->SetCurRace(pCha->m_Race);
			m_pRShiftMenu->SetUserName(pCha->m_pstrName);
		}
	}
}


void CNk2DFrame::ShowLShiftMenu()
{
//	if(m_bAutoMouse) ///07-04-12 숫자확인 시스템 타 인터페이스와 함께 쓸 수 있게 수정.
//		return;
	HRESULT hr;
	D3DVIEWPORT7 vp;

	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;

	CNkMyPet *pPet = pCMyApp->m_pMyPet;

	if (pPet != NULL)
	{
		m_bLShift = TRUE;

		vWorld = D3DVECTOR(pPet->m_wx, pPet->m_wy, pPet->m_wz);

		// 화면 좌표 계산.
		pCMyApp->GetLandPtr()->GetPipelineMatrix(matPipe);
		D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

		if (m_pLShiftMenu)
		{
			if (FAILED(hr = pCMyApp->GetDevice()->GetViewport( &vp ))) return ;

			// viewport의 크기 확인
			//pCMyApp->
			int cx, cy;
			cx = 279, cy = 122;  //팝업 메뉴가 클릭한 캐릭터 하단중앙에 위치하게 함.
			vScr.x = vScr.x - 230/2;

			if((vScr.x+cx) > (vp.dwX+vp.dwWidth))
				vScr.x = vp.dwX+vp.dwWidth-cx;
			if(vScr.x <0)
				vScr.x = 0;
			if(vScr.y < 0)
				vScr.y = 0;
			if((vScr.y+cy) > (vp.dwY+vp.dwHeight))
				vScr.y = vp.dwY+vp.dwHeight-cy;

			m_pLShiftMenu->SetPos(vScr.x, vScr.y);
			m_pLShiftMenu->SetCurRace(0);
			m_pLShiftMenu->SetUserName(pPet->m_strName);
			m_pLShiftMenu->m_bMacroEditing = FALSE; // 새로 켜질때 이건 항상 초기화 되어야한다.
		}
	}
}

void CNk2DFrame::ShowPetLevelMenu()
{
//	if(m_bAutoMouse) ///07-04-12 숫자확인 시스템 타 인터페이스와 함께 쓸 수 있게 수정.
//		return;

	HRESULT hr;
	D3DVIEWPORT7 vp;

	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;

	HWND m_hWnd;
	RECT ClientRc;
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &ClientRc);

	m_bPetLevel = TRUE;
	if (m_pPet_Level)
	{
		if (FAILED(hr = pCMyApp->GetDevice()->GetViewport( &vp ))) return ;

		vScr.x=ClientRc.right/2-200;  //찍는 위치.
		vScr.y=ClientRc.bottom/2-129;

		m_pPet_Level->Init();
		m_pPet_Level->SetXPos(vScr.x);
		m_pPet_Level->SetYPos(vScr.y);
		m_pPet_Level->SetMoney(g_pRoh->m_Money);
	}
}

BOOL CNk2DFrame::CloseAllWindow( BOOL bAdjustVP, bool bTap ) // 뷰포트 갱신을 하느냐 마느냐를 인자로 받음. 이 함수가 다른 인터페이스의 Show계열 함수에서 사용될때는 뷰포트 갱신을 안하기 위해 사용.(05-09-30 원석) ///신방어구강화시스템
{
	// 이 함수의 특징은 함부로 끄면 안되는 경우를 고려해야 한다.
	// 그 예가 바로 겜블중의 겜블윈도우나 오토마우스 창이다.
	// 이런녀석들은 함부로 여기에 넣으면 안된에 유의! (원석)

	/////////////////////////////////////////////////////
	// 인터페이스 오픈 실패 체크.
	// 특정 인터페이스들이 작동중일땐 다른 인터페이스의 활성화를 막는다.
	// 이것들도 우선순위를 잘 생각해야한다.
	if( m_pGamble && m_pGamble->IsOnGamble() == TRUE )
		return FALSE;

	if( m_pLockSmith && m_pLockSmith->IsProcess() == true )
		return FALSE;

	if( IsGambleVisible() && GetGamble() && GetGamble()->m_bExchange ) return FALSE;

	if( m_pControlUserShop && m_pControlUserShop->m_States != -1 && m_pControlUserShop->m_States != 2 ) // 개인상점 열려있는 중엔 함부로 못닫게 처리.(하이드 중엔 상관없음.) ///개인상점
		return FALSE;


	m_bLockSmith = FALSE;

	if (m_bShop)
	{
		m_bShop = FALSE;
		if( m_pControlShop )
			m_pControlShop->RemoveAllItems();
	}

	if (m_bShowExchange)
	{
		m_bShowExchange = FALSE;
		if( m_pControlExchange && m_pControlExchange->IsExch() )
			m_pControlExchange->ActivateExch(FALSE);	// 네트웍 메시지를 보낸다.
	}
	if (m_bShowWareHouse)
	{
		m_bShowWareHouse = FALSE;
		if( m_pControlWareHouse && m_pControlWareHouse->IsActivate() )
			m_pControlWareHouse->ActivateWareHouse(FALSE);
	}

	if( m_bShowGStoreHouse )
	{
		m_bShowGStoreHouse = FALSE;
		if( m_pControlGStoreHouse && m_pControlGStoreHouse->IsActivate() )
			m_pControlGStoreHouse->ActivateGStoreHouse(FALSE);
	}

	if (m_bInterchange)
	{
		m_bInterchange = FALSE;
	}

	m_bInven = FALSE;
	if( m_pControlInven && m_pControlInven->m_bAct_Masuk )
	{
		m_pControlInven->Close_Masuk();
	}

	if( m_bMenu ) // 메뉴인터페이스가 켜져있었다면...
	{
		m_bMenu = FALSE;
		SAFE_DELETE_RES( m_pControlMenu );
	}

	if( !bTap )
	{
		if(pCMyApp->m_pUIMgr)
			pCMyApp->m_pUIMgr->m_bMoveKit=FALSE; // 존 이동장치의 사용 여부. ShowGateWindow에서는 예외적으로 값을 기록해두었다 복구한다.
	}
	else
	{
	}
	m_bInterface[SCORE] = false;
	m_bInterface[GATE] = false;
	m_bInterface[SOCIAL] = false;
	m_bInterface[QUEST] = false;
	m_bInterface[MAP] = false;
	m_bInterface[DOCUMENT] = false;
	m_bInterface[YUTBOARD] = false;
	m_bInterface[FISHING] = false;
	m_bInterface[FISHING_REWARD] = false;
	m_bInterface[BASEBALL] = false;
	m_bInterface[BASEBALL_REWARD] = false;
	m_bInterface[LOTTO] = false;
	m_bInterface[BINGO] = false;
	m_bInterface[LETTER_WINDOW] = false;
	m_bInterface[LETTER_SEND] = false;
	m_bInterface[LETTER_RECV] = false;
	m_bInterface[LOTTERY] = false;
	m_bInterface[DICEGAME] = false;
	m_bInterface[DICEGAME_REWARD] = false;
	m_bParty = FALSE;
	m_bHelper = FALSE;	//후견인
	m_bRoyale = false;

	{

		m_bHeroFinalRank = FALSE;
		m_bShowControlGuild = FALSE;
		m_bShowControlGuildManage = FALSE;
		m_bShowGuildManager = FALSE;
		//m_pUIWindow->AllCloseWindow();
	}

	m_bSkill = FALSE;
	m_bHelp = FALSE;
	m_bShowGamble = FALSE;
	m_bShowEventAncient = FALSE;
	m_bShowPlatinum = FALSE;
	m_bShowSummerEvent = FALSE;
	m_bRShift = FALSE;
//kwon
	m_bPetLevel = FALSE;

	m_bPetStatus = FALSE;

	m_bPeiceShop = FALSE; // 이거 기존에 없던거 "///신방어구강화시스템" 넣으면서 추가 했음. 문제 생길여지가 있으니 요주의 할 것.(05-10-04 원석)
	m_bProduct = FALSE; // 이거 기존에 없던거 "///신방어구강화시스템" 넣으면서 추가 했음. 문제 생길여지가 있으니 요주의 할 것.(05-10-04 원석)
	m_bPetProduct = FALSE; // 05-11-09 추가

	if( m_bShowUpgrade ) // 강화 계열 인터페이스가 켜져있었다면...
	{
		m_bShowUpgrade = FALSE; ///신방어구강화시스템
		SAFE_DELETE_RES( m_pUpgrade );

	}

	if( m_bShowUserShopSearch ) // 텍스트 입력 받는 인터페이스는 끌때 ConvertImmStatus() 를 해줘야한다.
	{
		m_bShowUserShopSearch = FALSE; ///개인상점검색

		pCMyApp->ConvertImmStatus(); // 이걸해줘야 조합중 닫았을때 풀어주는 듯.(이거 안해주면 일본 전각문자 입력중등시에 문제가 생김)
	}

	if( m_bShowCashShop ) ///유료상점
	{
		m_bShowCashShop = FALSE;

		if( m_pControlCashShop )
			m_pControlCashShop->ClearDta(); // Dta는 열때 마다 새로 로딩.(메모리 절약을 위해)
	}

	if( m_bShowCashShopIntro ) ///유료상점
	{
		m_bShowCashShopIntro = FALSE;

		if( m_pCashShopIntro )
		{
			m_pCashShopIntro->Close(); // 웹 페이지 윈도우를 숨겨준다.
		}
	}


	m_bShowRefine = FALSE; // 기존에는 밑의 것처럼 함수를 호출해서 껐는데 인터페이스 끄기 일관화 작업중에 저 함수로 끄면 문제가 발생했다. 그래서 플래그만 끔.(관련하여 다른 문제가 생길수있음에 유의.)(05-11-02 원석)
	m_bShowProcess = FALSE;
	m_bWorldCup = FALSE;
	m_bHelper = FALSE;		//후견인

	if( bAdjustVP ) ///신방어구강화시스템
	{
	}

	if( pCMyApp->m_pMouse->GetMouseType() == M_REPAIR )
	{
		pCMyApp->m_pMouse->SetMouseType(M_REPAIR);
	}

	if( m_bBossraidInfo )
	{
		m_bBossraidInfo = FALSE;
	}

	if( m_bHeroPrivilege )
	{
		m_bHeroPrivilege = FALSE;
	}

	if( m_bHeroRankList )
	{
		m_bHeroRankList = FALSE;
	}

	if(m_pPremiumChaMove)
	{
		m_bShowPremiumChaMove = FALSE;
	}

	pCMyApp->m_pUIMgr->m_bIsTeleportItem = FALSE;

	if( m_pControlSelMenu )
		m_bShowControlSelMenu = FALSE;

	if( GetTakeOutBox() )
	{
		SAFE_DELETE(m_ui_takebox);
	}

	return TRUE;
}

void CNk2DFrame::UpdateDrawInfo()
{
	if( m_pControlInven == NULL )
		return;

	m_pControlInven->m_bDrawInfoUpdate = TRUE;
}

void CNk2DFrame::ShowUI(int ui_type, int sub_type)
{
	if (m_pGamble && m_pGamble->IsOnGamble() == TRUE)
		return;

	switch(ui_type)
	{
	case LHUI_Type__Letter:
		break;
	case LHUI_Type__Shop:
		break;
	case LHUI_Type__PieceShop:
		break;
	case LHUI_Type__Exchange:
		break;
	case LHUI_Type__Interchange:
		break;
	case LHUI_Type__Inven:
		break;
	case LHUI_Type__Gate:
		break;
	case LHUI_Type__PetStatus:
		break;
	case LHUI_Type__Skill:
		break;
	case LHUI_Type__Help:
		break;
	case LHUI_Type__Party:
		break;
	case LHUI_Type__Product:
		break;
	case LHUI_Type__PetProduct:
		break;
	case LHUI_Type__Gamble:
		break;
	case LHUI_Type__LockSmith:
		break;
	case LHUI_Type__EventAncient:
		break;
	case LHUI_Type__Platinum:
		break;
	case LHUI_Type__SummerEvent:
		break;
	case LHUI_Type__Refine:
		break;
	case LHUI_Type__Upgrade:
		break;
	case LHUI_Type__TitleSystem:
		break;
	case LHUI_Type__Notice:
		break;
	case LHUI_Type__Warehouse:
		break;
	case LHUI_Type__GStoreHouse:
		break;
	case LHUI_Type__Guild_Battle:
		break;
	case LHUI_Type__Guild:
		break;
	case LHUI_Type__GuildManager:
		break;
	case LHUI_Type__UserShop:
		break;
	case LHUI_Type__UserShop_Search:
		break;
	case LHUI_Type__CashShop:
		break;
	case LHUI_Type__CashShop_Intro:
		break;
	case LHUI_Type__Process:
		break;
	case LHUI_Type__Helper:
		break;
	case LHUI_Type__PetHelp:
		break;
	case LHUI_Type__RShift:
		break;
	case LHUI_Type__LShift:
		break;
	case LHUI_Type__PetLevel:
		break;
	case LHUI_Type__PVP:
		break;
	case LHUI_Type__Dmitron_Status:
		break;
	case LHUI_Type__PremiumExchange:
		break;
	case LHUI_Type__Password_Check:
		break;
	case LHUI_Type__TakeBox:
		break;
	default: // LHUI_Type__EndOfEnum
		break;
	}
}

void CNk2DFrame::CloseUI(int ui_type)
{
	switch(ui_type)
	{
	case LHUI_Type__Letter:
		break;
	case LHUI_Type__Shop:
		break;
	case LHUI_Type__PieceShop:
		break;
	case LHUI_Type__Exchange:
		break;
	case LHUI_Type__Interchange:
		break;
	case LHUI_Type__Inven:
		break;
	case LHUI_Type__Gate:
		break;
	case LHUI_Type__PetStatus:
		break;
	case LHUI_Type__Skill:
		break;
	case LHUI_Type__Help:
		break;
	case LHUI_Type__Party:
		break;
	case LHUI_Type__Product:
		break;
	case LHUI_Type__PetProduct:
		break;
	case LHUI_Type__Gamble:
		break;
	case LHUI_Type__LockSmith:
		break;
	case LHUI_Type__EventAncient:
		break;
	case LHUI_Type__Platinum:
		break;
	case LHUI_Type__SummerEvent:
		break;
	case LHUI_Type__Refine:
		break;
	case LHUI_Type__Upgrade:
		break;
	case LHUI_Type__TitleSystem:
		break;
	case LHUI_Type__Notice:
		break;
	case LHUI_Type__Warehouse:
		break;
	case LHUI_Type__GStoreHouse:
		break;
	case LHUI_Type__Guild_Battle:
		break;
	case LHUI_Type__Guild:
		break;
	case LHUI_Type__GuildManager:
		break;
	case LHUI_Type__UserShop:
		break;
	case LHUI_Type__UserShop_Search:
		break;
	case LHUI_Type__CashShop:
		break;
	case LHUI_Type__CashShop_Intro:
		break;
	case LHUI_Type__Process:
		break;
	case LHUI_Type__Helper:
		break;
	case LHUI_Type__PetHelp:
		break;
	case LHUI_Type__RShift:
		break;
	case LHUI_Type__LShift:
		break;
	case LHUI_Type__PetLevel:
		break;
	case LHUI_Type__PVP:
		break;
	case LHUI_Type__Dmitron_Status:
		break;
	case LHUI_Type__PremiumExchange:
		break;
	case LHUI_Type__Password_Check:
		break;
	case LHUI_Type__TakeBox:
		break;
	default: // LHUI_Type__EndOfEnum
		break;
	}
}

void CNk2DFrame::ShowBossraidInfoWindow(BOOL bShow)
{
	if( !CloseAllWindow(FALSE) )
		return;

	m_bBossraidInfo = bShow;

	if(bShow)
		pCMyApp->StopHero();
}

void CNk2DFrame::SetBossRaidInfo(int BossZoneNum, int bossNum, BOOL KillFlag)
{
	m_pBossraidInfo->SetBossraidKillFlag(BossZoneNum, bossNum, KillFlag);
}

void CNk2DFrame::InitBossRaidKillCount()
{
	m_pBossraidInfo->InitKillCount();
}

void CNk2DFrame::ShowHeroPrivilegeWindow(BOOL bShow)
{
	m_bHeroRankList = bShow;

	if( !CloseAllWindow(FALSE))
		return;

	if( bShow )
		pCMyApp->StopHero();

	m_bHeroPrivilege = bShow;
}

void CNk2DFrame::SetHeroPrivilege(int heroPrivilege)
{
	m_pHeroPrivilege->SetHeroPrivilege( heroPrivilege );
}


void CNk2DFrame::ShowHeroRankListWindow(BOOL bShow, int race )
{
	m_bHeroRankList = bShow;

	if( !CloseAllWindow(FALSE))
		return;

	m_bHeroRankList = bShow;

	if(m_pHeroRankList && bShow )
	{
		m_pHeroRankList->SetRace( race );
		m_pHeroRankList->SendMsgPage();
	}


}

void CNk2DFrame::SetHeroRankList(char* buf)
{
	if( m_pHeroRankList )
		m_pHeroRankList->SetHeroRankLsit( buf );
}



void CNk2DFrame::ShowHeroFinalRankWindow(BOOL bShow)
{
	if( !CloseAllWindow(FALSE) )
		return;

	m_bHeroFinalRank = bShow;
}

void CNk2DFrame::SetHeroFinalRank(char* buf)
{
	m_pHeroFinalRank->SetHeroRank(buf);
}


void CNk2DFrame::ShowPetProductWindow(BOOL bShow, BOOL bAct) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bPetProduct = bShow;

		pCMyApp->StopHero();
	}

	if (m_bPetProduct && bAct)
		m_pPetProduct->ActivateExch(TRUE);
	else
		m_pPetProduct->ActivateExch(FALSE);
}

void CNk2DFrame::TogglePlatinumExchWindow(int type)
{
	if(Check_All_MsgPopUp())
		return;

	ShowPlatinumWindow(!m_bShowPlatinum);
	if(IsPlatinumVisible())
	{
		pCMyApp->StopHero();

		SAFE_DELETE_RES(m_pPlatinum);
		ShowPlatinumWindow(false);

		m_pPlatinum->LoadRes(type, m_pItemRender);
		m_pPlatinum->SetNkCha(m_pNkCha);

		ShowPlatinumWindow(true);
	}
}

// 플래티늄 관련 함수


void CNk2DFrame::ShowPlatinumWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bShowPlatinum = bShow;
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}

void CNk2DFrame::ShowSummerEventWindow(BOOL bShow) ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bShowSummerEvent = bShow;
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}



void CNk2DFrame::ToggleSummerEventWindow(int type)
{
	if(Check_All_MsgPopUp())
		return;

	ShowSummerEventWindow(!m_bShowSummerEvent);
	if(IsSummerEventVisible())
	{
		pCMyApp->StopHero();

		SAFE_DELETE_RES(m_pSummerEvent);
		ShowSummerEventWindow(false);
		m_pSummerEvent->LoadRes(type, m_pItemRender);
		m_pSummerEvent->SetNkCha(m_pNkCha);
		ShowSummerEventWindow(true);
	}
}

void CNk2DFrame::ShowPremiumChaMove(BOOL bShow, int slot)
{
	if( !CloseAllWindow(TRUE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	m_bShowPremiumChaMove = bShow;
	if( m_bShowPremiumChaMove == FALSE )
	{
		m_pPremiumChaMove->SetStep();
	}

	m_pPremiumChaMove->SetSlot(slot);
}

void CNk2DFrame::ShowAutoMouseWindow(BOOL bShow,int Type) ///QnA ///인터페이스 클로즈 처리 함수화(05-09-30 원석)
{
//	CloseAllWindow(FALSE); // 이 녀석은 예외적으로 클로즈 살패해도 처리해준다.(왜냐면 이녀석이 실패 조건 중 하나니까 -_-;) (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)

	if( !m_pAutoMouse )
		return;

	m_bAutoMouse = bShow; // 문자입력시스템은 예외적으로 여기서 직접 닫아줘야한다.(함부로 닫히면 안되니까 위의 CloseAllWindow에서 하지 않는다.)
	SAFE_DELETE_RES(m_pAutoMouse);

	if( bShow ) // 열때
		m_pAutoMouse->LoadRes(Type ); // 리소스를 다시 로드.
}

void CNk2DFrame::GuildPK(int sub_type, char *GuildName1, char *GuildName2, int PKstate)
{
	SAFE_DELETE(m_pGuildPK);
	m_pGuildPK = new CGuildPK();

	switch(sub_type)
	{
	case 0: // 길드전 요청(request)
		{
			m_pGuildPK->AskGPK(GuildName1);
		}
		break;
	case 1: // 항복(surrender)
		{
			m_pGuildPK->Surrender(GuildName1,GuildName2);
		}
		break;
	default:
		break;
	}
}

void CNk2DFrame::ShowUpgradeWindow( BOOL bShow ) ///신방어구강화시스템
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bShowUpgrade = bShow;
	}
	else // (06-01-18 결과 받은후 창을 닫을 경우 메세지 박스 출력되도록.
	{
		if( m_pUpgrade == NULL )
			return;

		if( m_pUpgrade->m_Result != 0 ) // 결과가 들어와있다면...메세지 박스로 결과를 표시해준다.
		{
			if( m_pUpgrade->m_iType == INTERFACE_TYPE_UPARMOR || m_pUpgrade->m_iType == INTERFACE_TYPE_UPWEAPON ) // 타입따라 메세지가 바뀌어야 한다.
			{
				switch( m_pUpgrade->m_Result )
				{
				case UPGRADE_ANI_SUCCESS:
					//-- IDS_UIMGR_SUCCEED_UPGRADE : ◈ 장비 업그레이드에 성공했습니다
					pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE) );
					break;

				case UPGRADE_ANI_FAIL:
					//-- IDS_UIMGR_FAIL_UPGRADE : ◈ 장비 업그레이드에 실패했습니다.
					pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE) );
					break;

				case UPGRADE_ANI_BROKEN:
					//-- IDS_UIMGR_REMOVE_UPGRADE : ◈ 장비가 소멸되었습니다.
					pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE) );

					break;
				}
			}
			else if( m_pUpgrade->m_iType == INTERFACE_TYPE_CONVERSION ) // 타입따라 메세지가 바뀌어야 한다.
			{
				switch( m_pUpgrade->m_Result )
				{
				case UPGRADE_ANI_SUCCESS:
					//-- IDS_CONVERSION_SUCC : 컨버젼에 성공하였습니다.
					pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_CONVERSION_SUCC) );
					break;

				case UPGRADE_ANI_FAIL:
					//-- IDS_CONVERSION_FAIL : 컨버젼에 실패하였습니다.
					pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_CONVERSION_FAIL) );
					break;
				}
			}
			else if( m_pUpgrade->m_iType == INTERFACE_TYPE_CONFUSION ) // 랜덤 다시 돌리기
			{
				// 이녀석은 무조건 성공이다.
				//-- IDS_EQUIP_STATUS_RESET : 장비의 수치가 새로이 적용되었습니다.
				pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_EQUIP_STATUS_RESET) );
			}

		}
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}

void CNk2DFrame::ToggleUpgradeWindow( int type, BOOL ForceClose ) ///신방어구강화시스템 //타입은 후에 가공을 고려. 현재 안쓰임.
{
	if( Check_All_MsgPopUp() )
		return;

	if( ForceClose ) // 강제로 끄기위한 녀석. ShowUpgradeWindow( FALSE )를 안쓰는 이유는 이 녀석은 닫힐때 메모리를 해제하고 싶어서..(열때마다 다시 로딩시킨다. 상주 메모리를 줄일수있을 듯.)
	{
		m_bShowUpgrade = TRUE;
	}

	ShowUpgradeWindow( !m_bShowUpgrade );
	if( IsUpgradeVisible() )
	{
		m_pUpgrade->LoadRes(type, m_pItemRender); // 열었을때 로드시킨다.(열 당시 느릴지는 몰라도 끌때 메모리 해제를 해주므로 상주메모리를 줄일수있을 것이다.)
		m_pUpgrade->SetNkCha(m_pNkCha);
	}
}

void CNk2DFrame::ShowControlNoticeWindow( BOOL bShow, int Type, char *String ) // 인터페이스형 공지 창 ///공지팝업창
{
	// 이 녀석은 조금 처리가 특이하다. 3D형 인터페이스라 다른 인터페이스를 끄지 않는다.
	if( !bShow ) // 끄는 경우..
	{
		SAFE_DELETE( m_pControlNotice );
		m_bShowNotice = false;
	}
	else // 키는 경우. 타입별로 갈라야한다.
	{
		// 킬 때는 기존에 안켜져잇던 상태에서 새로 켜지는 경우와 기존에 켜져있던 상태에서 스트링이 추가되는 경우를 나누어야 한다.
		if( m_pControlNotice ) // 기존에 창이 떠있는 경우.
		{
			m_pControlNotice->SetContents( String ); // 내용은 무조건 추가.

			if( m_pControlNotice->m_nState == NOTICE_STATE_MINI ) // 최소화 상태일때는
				m_pControlNotice->m_bNewMessage = TRUE; // 새로 메세지가 왔을을 알려줘야한다.
		}
		else // 새로 창을 띄우는 경우.
		{
			m_pControlNotice = new CControlNotice;
			if( !m_pControlNotice )
				return;
			m_pControlNotice->LoadRes();

			// 타입에 따라 타이틀을 자동으로 붙여주자.
			m_pControlNotice->SetType( Type );

			m_pControlNotice->SetContents( String ); // 내용을 추가.

			m_pControlNotice->m_bNewMessage = TRUE; // 새로 메세지가 왔을을 알려줘야한다.
		}

		m_bShowNotice = true;
	}
}
void CNk2DFrame::ShowUserShopWindow( BOOL bShow ) ///개인상점
{
	CloseAllWindow(FALSE); // 이녀석도 예외적으로 실패해도 처리가 되어야한다.. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)

	m_bShowUserShop = bShow; // CloseAllWindow() 실패시 처리를 안 막는 예외적인 녀석들은 끌때도 이걸 셋팅해줘야한다.

	if( bShow )
	{
		m_bInven = TRUE;
		m_pControlUserShop->m_States = USHOP_STATES_NORMAL; // 열렸을 시는 노말 상태.
	}
	else
	{
		m_pControlUserShop->m_States = USHOP_STATES_NOTOPEN; // 닫힌 상태임을 이걸로도 표시해주자.
		pCMyApp->ConvertImmStatus(); // 이걸해줘야 조합중 닫았을때 풀어주는 듯.(이거 안해주면 일본 전각문자 입력중등시에 문제가 생김)
	}
}

void CNk2DFrame::ToggleUserShopWindow( int index, int type, int grade ) // type은 점포상점인지 개인상점인지, grade는 팩의 갯수다. ///개인상점
{
//	if( Check_All_MsgPopUp() )
//		return;
//	if( m_bAutoMouse )
//		return;

	if( !m_pControlUserShop ) // 혹시라도 없을경우


		return;

	ShowUserShopWindow( !m_bShowUserShop );
	if( IsUserShopVisible() )
	{
		char strTemp1[100]; // 이름 저장용.
		char strTemp2[256]; // 멘트 저장용.

		if( !m_pControlUserShop->m_bResLoad ) // 리소스 로드가 아직 안되었다면...
		{
			m_pControlUserShop->LoadRes( m_pItemRender); // 처음 열었을때만 로드


			m_pControlUserShop->SetNkCha(m_pNkCha);
//			m_pControlUserShop->SetTcpIp(g_pTcpIp);
		}

		if( type == 0 ) // 개인 상점이면...
		{
			//-- IDS_WHOS_USERSHOP : %s의 개인상점
			sprintf( strTemp2, "%s", (char*)G_STRING(IDS_WHOS_USERSHOP) );

			if( index == -1 ) // 자기 자신의 상점을 새로 개설하려고 오픈하는 경우.
			{
				sprintf( strTemp1, strTemp2, g_pRoh->m_pstrName ); // 자기 이름.
				ZeroMemory( strTemp2, sizeof(strTemp2) ); // 멘트 부는 비워둔다.

				type = 0; // type 변수를 구매자,판매자,관리자 모드를 표시해주도록 변환한다.(여기서 인자로 들어오는 개인,점포 여부는 위의 스트링 셋팅에서만 필요하기때문에.)
			}
			else // 다른 사람의 상점을 개설 목적으로 연 경우.
			{
				///점포상점 시 작업.
				CNkCharacter *pCha = NULL;

				pCha = pCMyApp->m_pUIMgr->FindChaById( index, FALSE );
				if( pCha )
				{
					sprintf( strTemp1, strTemp2, pCha->m_pstrName ); // 자기 이름.
					sprintf( strTemp2, "%s", pCha->m_strUserShopMent ); // 멘트
				}
				else
				{
					ShowUserShopWindow( FALSE ); // 디시 닫아줘야한다.

					return; // 뭔가 이상한 경우다. 오픈하지 말자.
				}

				type = 1; // type 변수를 구매자,판매자,관리자 모드를 표시해주도록 변환한다.(여기서 인자로 들어오는 개인,점포 여부는 위의 스트링 셋팅에서만 필요하기때문에.)
			}

		}
		else if( type == 1 ) // 점포 상점이면...
		{
			//-- IDS_WHOS_USHOP2 : %s의 점포상점
			sprintf( strTemp2, "%s", (char*)G_STRING(IDS_WHOS_USHOP2) );

			if( index == -1 ) // 자기 자신의 상점을 새로 개설하려고 오픈하는 경우.
			{
				sprintf( strTemp1, strTemp2, g_pRoh->m_pstrName ); // 자기 이름.
				ZeroMemory( strTemp2, sizeof(strTemp2) ); // 멘트 부는 비워둔다.

				type = 3; // 점포 상점인데 이 경우는 중간에 수정을 위해서 연게 아니라 처음 개설 오픈이다.
			}
			else
			{
				CUserShopNpc *TempUShopNpc = pCMyApp->m_pUIMgr->FindUShopNpcById( index );

				if( TempUShopNpc )
				{
					sprintf( strTemp1, strTemp2, TempUShopNpc->m_strMasterName ); // 주인 이름.
					sprintf( strTemp2, "%s", TempUShopNpc->m_strUserShopMent ); // 멘트

					if( TempUShopNpc->m_MasterIndex == g_pRoh->m_nCharIndex ) // 자기 자신 점포상점이면...
					{
						type = 2; // type 변수를 구매자,판매자,관리자 모드를 표시해주도록 변환한다.(여기서 인자로 들어오는 개인,점포 여부는 위의 스트링 셋팅에서만 필요하기때문에.)
					}
					else
					{
						type = 1; // type 변수를 구매자,판매자,관리자 모드를 표시해주도록 변환한다.(여기서 인자로 들어오는 개인,점포 여부는 위의 스트링 셋팅에서만 필요하기때문에.)
					}
				}
				else
				{
					ShowUserShopWindow( FALSE ); // 디시 닫아줘야한다.

					return; // 뭔가 이상한 경우다. 오픈하지 말자.
				}
			}
		}

		m_pControlUserShop->Open( type, grade, strTemp1, strTemp2 ); // 멘트와 이름은 추후 개인상점 리스트를 만들어서 검색하여 넣어주자.
	}
}

void CNk2DFrame::ShowUserShopSearchWindow( BOOL bShow ) ///개인상점검색
{
	if( !CloseAllWindow(FALSE) ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( bShow )
	{
		m_bInven = TRUE;
		m_bShowUserShopSearch = bShow;
	}
	else // 닫을때 07-01-15 추가
		pCMyApp->ConvertImmStatus(); // 이걸해줘야 조합중 닫았을때 풀어주는 듯.(이거 안해주면 일본 전각문자 입력중등시에 문제가 생김)
}

void CNk2DFrame::ToggleUserShopSearchWindow( BOOL ForceClose ) ///개인상점검색
{
	if( ForceClose ) // 강제로 끄기위한 녀석. 닫힐때 메모리를 해제하고 싶어서..(열때마다 다시 로딩시킨다. 상주 메모리를 줄일수있을 듯.)
		m_bShowUserShopSearch = TRUE;

	ShowUserShopSearchWindow( !m_bShowUserShopSearch );
	if( IsUserShopSearchVisible() )
		m_pUserShopSearch->LoadRes( m_pItemRender ); // 열었을때 로드시킨다.(열 당시 느릴지는 몰라도 끌때 메모리 해제를 해주므로 상주메모리를 줄일수있을 것이다.)
}

void CNk2DFrame::ShowControlMenuWindow( BOOL bShow, int Type, const char *NpcName ) // 타입은 어떤 경우의 메뉴인가...(이녀석은 토글 함수가 필요없다.) ///메뉴인터페이스
{
	if( !CloseAllWindow() ) // 클로즈 실패시엔 열리지 않게 처리. (05-11-19 원석) // 일단은 전부 닫은 후. (뷰포트 갱신 플래그를 FALSE로 줌에 유의. 뷰포트 갱신은 이 함수 마지막에 해주어야한다.)
		return;

	if( !bShow ) // 끄는 경우..
	{
		if( m_pControlMenu )
			SAFE_DELETE_RES(m_pControlMenu);
	}
	else // 키는 경우. 타입별로 갈라야한다.
	{
		pCMyApp->StopHero();

		CMultiScrollText *TempScrollText = NULL;

		switch( Type )
		{
		case MENU_TYPE_USERSHOP: // 개인상점 회수,검색.

			// 필요한 멤버들을 등록 배치한 후.
			m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 ); // 배경 지정. 위치가 -1이면 자동 중앙 위치다.
			if( NpcName && NpcName[0] ) // Npc명이 제대로 들어왔다면...
				m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀

			m_pControlMenu->AddButton( "Menu/btn_search", 57, 230, MENU_ACTION_USERSHOP_SEARCH ); // 검색

			m_pControlMenu->AddButton( "Menu/btn_regain", 114, 230, MENU_ACTION_USERSHOP_RECEIVE ); // 회수

			m_pControlMenu->AddButton( "Menu/btn_cancel", 171, 230, MENU_ACTION_CLOSE ); // 닫기



			TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)
			// by evilkiki 2009.09.03 이동
			TempScrollText->LoadString( "data/menu/String1.txt" ); // 텍스트 파일로 부터 읽어옴.

			m_pControlMenu->LoadRes( ); // 필요한 리소스를 로드한다.

			break;

		case MENU_TYPE_NPCUPGRADE:

			// 필요한 멤버들을 등록 배치한 후.
			m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 ); // 배경 지정. 위치가 -1이면 자동 중앙 위치다.
			if( NpcName && NpcName[0] ) // Npc명이 제대로 들어왔다면...
				m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀

			if( g_dwClientCountry == CTRY_JPN )
			{
				m_pControlMenu->AddButton( "Menu/btn_upgrade", 20, 225, MENU_ACTION_NPCUPGRADE_UPGRADE ); // 강화
				m_pControlMenu->AddButton( "Menu/btn_process", 75, 225, MENU_ACTION_NPCUPGRADE_REFORM ); // 정련
				m_pControlMenu->AddButton( "Menu/btn_conv", 130, 225, MENU_ACTION_NPCUPGRADE_CONVERSION ); // 컨버젼
				m_pControlMenu->AddButton( "Menu/btn_conf", 200, 225, MENU_ACTION_NPCUPGRADE_CONFUSION ); // 컨퓨젼
				m_pControlMenu->AddButton( "Menu/btn_cancel", 113, 250, MENU_ACTION_CLOSE ); // 닫기

			}
			else
			{
				m_pControlMenu->AddButton( "Menu/btn_upgrade", 20, 225, MENU_ACTION_NPCUPGRADE_UPGRADE ); // 강화
				m_pControlMenu->AddButton( "Menu/btn_armor400", 70, 225, MENU_ACTION_NPCUPGRADE_ARMOR400 ); // 방어구강화400lv
				m_pControlMenu->AddButton( "Menu/btn_process", 120, 225, MENU_ACTION_NPCUPGRADE_REFORM ); // 정련
				m_pControlMenu->AddButton( "Menu/btn_conv", 170, 225, MENU_ACTION_NPCUPGRADE_CONVERSION ); // 컨버젼
				m_pControlMenu->AddButton( "Menu/btn_conf", 220, 225, MENU_ACTION_NPCUPGRADE_CONFUSION ); // 컨퓨젼
				m_pControlMenu->AddButton( "Menu/btn_cancel", 113, 250, MENU_ACTION_CLOSE ); // 닫기
			}


			TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)

			TempScrollText->LoadString( "data/menu/String2.txt" ); // 텍스트 파일로 부터 읽어옴.

			m_pControlMenu->LoadRes(); // 필요한 리소스를 로드한다.



			break;

		case MENU_TYPE_BPSHOP: // 이 녀석은 특이하게 여기서 AddButton()과 LoadRes()를 하지 않는다. 이 녀석을 호출한 CmdBpShopList() 에서 해준다.

			// 필요한 멤버들을 등록 배치한 후.
			m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 ); // 배경 지정. 위치가 -1이면 자동 중앙 위치다.
			if( NpcName && NpcName[0] ) // Npc명이 제대로 들어왔다면...
				m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀
			TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)
			TempScrollText->LoadString( "data/menu/String3.txt" ); // 텍스트 파일로 부터 읽어옴.
			break;

		case MENU_TYPE_ENCHANT:
			if( g_SvrType == ST_ADULT_ONLY ) // [2/27/2007 Theodoric] 인첸트 : 선택 팝업창 띄우기

			{
				// 필요한 멤버들을 등록 배치한 후.
				m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 ); // 배경 지정. 위치가 -1이면 자동 중앙 위치다.
				if( NpcName && NpcName[0] ) // Npc명이 제대로 들어왔다면...
					m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀

				m_pControlMenu->AddButton( "Menu/btn_upgrade", 20, 230, MENU_ACTION_ENCHANT_UPGRADE ); // 강화
				m_pControlMenu->AddButton( "Menu/btn_make", 79, 230, MENU_ACTION_ENCHANT_MAKE ); //  플래티늄 제작
				m_pControlMenu->AddButton( "Menu/btn_init", 139, 230, MENU_ACTION_ENCHANT_INIT ); // 초기화
				m_pControlMenu->AddButton( "Menu/btn_cancel", 193, 230, MENU_ACTION_CLOSE ); // 닫기


				TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)

				TempScrollText->LoadString( "data/menu/String4.txt" ); // 텍스트 파일로 부터 읽어옴.
				m_pControlMenu->LoadRes( ); // 필요한 리소스를 로드한다.
			}
			break;

		case MENU_TYPE_LPSHOP:
			// 이 녀석은 특이하게 여기서 AddButton()과 LoadRes()를 하지 않는다. 이 녀석을 호출한 CmdLPShopList() 에서 해준다.

			// 필요한 멤버들을 등록 배치한 후.
			m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 ); // 배경 지정. 위치가 -1이면 자동 중앙 위치다.

			if( NpcName && NpcName[0] ) // Npc명이 제대로 들어왔다면...
				m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀


			TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)

			TempScrollText->LoadString( "data/menu/String5.txt" ); // 텍스트 파일로 부터 읽어옴.
			break;

		case MENU_TYPE_NEW_NPCUPGRADE: 	// [2008/12/19 Theodoric] 300레벨 이상 무기 강화
			m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 ); // 배경 지정. 위치가 -1이면 자동 중앙 위치다.

			if( NpcName && NpcName[0] ) // Npc명이 제대로 들어왔다면...
				m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀

			if( CommonConfig::Instance()->GetGodWeapon() == true )
			{
				m_pControlMenu->AddButton( "Menu/btn_godup", 20, 225, MENU_ACTION_GODWEAPON_UP ); // 신의무기강화
				m_pControlMenu->AddButton( "Menu/btn_conv2", 80, 225, MENU_ACTION_NEW_NPCUPGRADE_CONVERSION ); // 컨버젼
				m_pControlMenu->AddButton( "Menu/btn_upgrade", 140, 225, MENU_ACTION_NEW_NPCUPGRADE_UPGRADE ); // 업그레이드
				m_pControlMenu->AddButton( "Menu/btn_godpow", 200, 225, MENU_ACTION_NEW_NPCUPGRADE_EXCHANGE ); // 교환
				m_pControlMenu->AddButton( "Menu/btn_cancel", 113, 250, MENU_ACTION_CLOSE ); // 닫기
			}
			else
			{
				m_pControlMenu->AddButton( "Menu/btn_conv2", 50, 230, MENU_ACTION_NEW_NPCUPGRADE_CONVERSION ); // 컨버젼
				m_pControlMenu->AddButton( "Menu/btn_upgrade", 100, 230, MENU_ACTION_NEW_NPCUPGRADE_UPGRADE ); // 업그레이드
				m_pControlMenu->AddButton( "Menu/btn_godpow", 150, 230, MENU_ACTION_NEW_NPCUPGRADE_EXCHANGE ); // 교환
				m_pControlMenu->AddButton( "Menu/btn_cancel", 200, 230, MENU_ACTION_CLOSE ); // 닫기
			}

			TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)
			TempScrollText->LoadString( "data/menu/String6.txt" ); // 텍스트 파일로 부터 읽어옴.

			m_pControlMenu->LoadRes( ); // 필요한 리소스를 로드한다.
			break;

		case MENU_TYEP_ITEMMAKE:
			m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 );
			m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀

			//제작 분해 취소
			m_pControlMenu->AddButton( "Menu/btn_production", 57, 230, MENU_ACTION_ITEMMAKE ); // 제작
			m_pControlMenu->AddButton( "Menu/btn_disassemble", 114, 230, MENU_ACTION_ITEMSEPARATE ); // 분해
			m_pControlMenu->AddButton( "Menu/btn_cancel", 171, 230, MENU_ACTION_CLOSE ); // 닫기

			TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)

			TempScrollText->LoadString( "data/menu/String7.txt" ); // 텍스트 파일로 부터 읽어옴.
			m_pControlMenu->LoadRes( ); // 필요한 리소스를 로드한다.
			break;

		case MENU_TYPE_BUFFMAKE:
			{
				m_pControlMenu->AddBackGround( "interface/Menu/MenuBack1.bmp", -1, -1, 300, 300 );
				m_pControlMenu->AddString( NpcName, 137, 12, TRUE ); // 타이틀
				
				m_pControlMenu->AddButton( "Menu/btn_production", 50, 230, MENU_ACTION_BUFFMAKE_MAKE ); // 제작
				m_pControlMenu->AddButton( "Menu/btn_gatcha", 100, 230, MENU_ACTION_BUFFMAKE_GATCHA ); // 갓챠
				m_pControlMenu->AddButton( "Menu/btn_medal", 150, 230, MENU_ACTION_BUFFMAKE_MEDAL ); // 메달
				m_pControlMenu->AddButton( "Menu/btn_cancel", 200, 230, MENU_ACTION_CLOSE ); // 닫기

				TempScrollText = m_pControlMenu->AddScrollText( 25, 50, 256, 162, 256-8, 50, 162 , "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // 내용. (x,y,width,height)

				TempScrollText->LoadString( "data/menu/String8.txt" ); // 텍스트 파일로 부터 읽어옴.
				m_pControlMenu->LoadRes( ); // 필요한 리소스를 로드한다.
			}
			break;

		default: // 등록 안된 타입은 인터페이스를 키지 않는다.
			return;
		}

		m_bMenu = TRUE;
	}

}

void CNk2DFrame::ShowProcessWindow(BOOL bShow, int type) // 가공창 뛰우기
{
	if( !CloseAllWindow(FALSE) )
		return;

	if( bShow )
	{
		if( m_pProcess )
		{
			m_pProcess->LoadRes(type, m_pItemRender);
			m_pProcess->SetNkCha(m_pNkCha);
		}

		m_bInven = TRUE;
		m_bShowProcess = bShow;

	}
	else
	{
		if(m_pProcess)
		{
			if (m_pProcess->IsProcessConnection())
				m_pProcess->CloseConnection();

			SAFE_DELETE_RES(m_pProcess);
		}
	}

	if( m_pControlInven )
		m_pControlInven->ResetItemInfo();
}

void CNk2DFrame::ToggleProcessWindow(int type)
{
	if (m_pGamble && m_pGamble->IsOnGamble() == TRUE)
		return;

	if(Check_All_MsgPopUp())
		return;

	ShowProcessWindow(!m_bShowProcess);
	if(IsProcessVisible())
	{
		SAFE_DELETE_RES(m_pProcess);
		ShowProcessWindow(false);
		m_pProcess->LoadRes(type, m_pItemRender);
		m_pProcess->SetNkCha(m_pNkCha);
		m_pProcess->SetTcpIp(g_pTcpIp);
		ShowProcessWindow(true);
	}
	else
	{
		if(m_pProcess)
		{
			if (m_pProcess->IsProcessConnection())
				m_pProcess->CloseConnection();
		}
	}
}

// nate 2006-02-24 : 모드 팝업 제거
// 스텍에 쌓인 모든 팝업을 순차적으로 제거한다.
void CNk2DFrame::DeleteAllPopup()
{
	if( !m_Popup )
		return;

	POPUP* pTail = NULL;
	do
	{
		pTail = m_Popup->pNext;		// 꼬리 저장


		delete m_Popup;				// 몸뚱이 삭제
		m_Popup = pTail;			// 꼬리를 몸뚱이로 변경


	}
	while( m_Popup );
	m_Popup = NULL;
}

// nate 2006-02-24 : 가장 상위 팝업 제거
// 스텍구조이므로 상위부터 제거됨
void CNk2DFrame::DeletePopup()
{
	if( !m_Popup )
		return;

	POPUP* pTemp = m_Popup->pNext;	// 가장 앞쪽의 팝업 제거
	delete m_Popup;
	m_Popup = pTemp;
}

// nate 2006-02-24 : 스텍에 팝업 샇기


// 팝업을 스텍알고리즘으로 관리한다.
void CNk2DFrame::InsertPopup( char* arg, int nType, int nProcess, int nLimit, int nDocument, void* pArgs )
{
	POPUP** pTemp = &m_Popup;
	while( *pTemp )					// 마지막 팝업을 찾는다
		pTemp = &((*pTemp)->pNext);

	(*pTemp) = new POPUP;

	if( !(*pTemp) ) // 07-01-24 예외처리 추가
		return;

	(*pTemp)->nProcess = nProcess;				// 메세지 처리 분류
	(*pTemp)->pArgs = pArgs;				// 드미트론 공성전
	(*pTemp)->pMsg_Popup.Init( nType );		// 팝업 종류 결정
	(*pTemp)->pMsg_Popup.LoadRes();
	(*pTemp)->pMsg_Popup.m_StrLimit = nLimit;
	(*pTemp)->pMsg_Popup.PutString( arg );		// 내용 입력
	m_nDocument = nDocument;
	if( pCMyApp->m_pMouse )
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
}

// nate 2006-02-28 : 팝업 검색


CMsgPopUp* CNk2DFrame::FindPopup( int nProcess )
{
	POPUP** pTemp = &m_Popup;
	while( *pTemp )					// 마지막 팝업을 찾는다
	{
		if( (*pTemp)->nProcess == nProcess )
			return &((*pTemp)->pMsg_Popup);

		pTemp = &((*pTemp)->pNext);
	}

	return NULL;
}

HRESULT CNk2DFrame::RestorePopups()
{
	POPUP** pTemp = &m_Popup;
	while( *pTemp )					// 마지막 팝업을 찾는다
	{
		((*pTemp)->pMsg_Popup).RestoreAll();

		pTemp = &((*pTemp)->pNext);
	}

	return S_OK;
}

// nate 2006-02-24 : 팝업 메세지 처리
LRESULT CNk2DFrame::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bStartExchange = FALSE;
	static BOOL bStartParty = FALSE;
	static BOOL bStartRefine = FALSE;
	static BOOL bStartProcess = FALSE;
	static BOOL bStartAddFriend = FALSE;
	static DWORD lastTickTime = timeGetTime();

	// [11/19/2008 D.K ] : 오토 관련 죽었을때 부활이랑 기타 등등 옵션 설정에 필요한 부분.
	int nMsg;

	if( m_Popup->nProcess == POPUP_RESURRECTION )
	{
		if( pCMyApp->m_pUIMgr->m_AutoPlay &&
			pCMyApp->m_pUIMgr->m_AutoPlay->IsRun() &&
			pCMyApp->m_pUIMgr->m_AutoPlay->m_dwGameMode == GAMEMODE_FULL_PACKAGE )
			return 1;

	}
	
	//This you can set pop-ups to close with ESC instead of having to fucking click them haha
	if( (m_Popup->nProcess == POPUP_RESURRECTION || m_Popup->nProcess == POPUP_RECYCLE_ITEM)
			&& ( msg == WM_KEYDOWN || msg == WM_CHAR || msg == WM_IME_STARTCOMPOSITION
				 || msg == WM_IME_COMPOSITION || msg == WM_IME_ENDCOMPOSITION || msg == WM_IME_CHAR
				 || msg == WM_IME_NOTIFY
			   )
	  )
	{
		m_pControlBottom->MsgProc(hWnd, msg, wParam, lParam); // 임의로 채팅을 가능하도록 프로시져 예외처리...

		if( msg == WM_KEYDOWN && (int)wParam == VK_RETURN ) // 엔터면 포커스 처리.(직접 클릭은 복잡해서 처리안한다.엔터로만 포커스..)
		{
			SetFocus(); // 안에서 알아서 꺼져있을때만 처리해줌.
		}

		return 1; // 팝업쪽에선 무시하도록.
	}
	else
	{
		nMsg = m_Popup->pMsg_Popup.MsgProc(hWnd, msg, wParam, lParam);	// 액션 값 얻어오기

	}

	if(m_Popup->nProcess == POPUP_EVOLUTIONBALL_USE)
	{
		if( nMsg != 0)
		{
			int Page = atoi(m_Popup->pMsg_Popup.m_InputValue1.GetStrBuf());
			int Time = atoi(m_Popup->pMsg_Popup.m_InputValue2.GetStrBuf());
			char str[128] = {0,};

			if( nMsg == 1)
			{
				//인벤에  에볼류션 볼이 있는지 찾자
				CItem *pItem = g_pRoh->FindItemByVnumFromAll(2225);
				if( pItem )
				{
					// 클라에서 에볼류션 볼을 삭제하고 서버에서도 삭제를 위해 인덱스 값을 보내준다.
					sprintf( str, "gh req %d 1 1 %d %d\n", Page, pItem->m_Index, Time );
					g_pRoh->RemoveByIndex(pItem->m_Index);
					pItem = NULL;
				}
				else
					sprintf( str, "gh req %d 1 0 0 %d\n", Page, Time );
			}
			else if (nMsg == -1 )
				sprintf( str, "gh req %d 1 0 0 %d\n",Page, Time );

			g_pTcpIp->SendNetMessage(str);
		}
	}

	if( m_Popup->nProcess == POPUP_CHANGE_CHAR_NAME )
	{
		if( nMsg == 1 )
		{
			if( GetPremiumExchange() )
			{
				GetPremiumExchange()->SendChangeCharName();
			}
		}
	}

	switch( m_Popup->nProcess)
	{
	case POPUP_LAGRUSH_START:
		if( nMsg == 1)
		{
			pCMyApp->m_pUIMgr->m_AutoPlay->StartProc(GAMEMODE_AUTO);
			GetControlTop()->m_iConAutoOnBtn.SetState( BTN_DISABLE );
			DeletePopup();
			return 1;
		}
		break;
	case POPUP_PREMIUM_CHA_MOVE_FROM:
		if( nMsg == 1)
		{
			GetPremmiumChaMove()->SetStep(FROM, 1);
			if( !IsPremiumChaMove() )
				ShowPremiumChaMove(TRUE);
			DeletePopup();
			SetProcLock(TRUE);
			return 1;
		}
		break;

	case POPUP_PREMIUM_CHA_MOVE_TO:
		if( nMsg == 1)
		{
			GetPremmiumChaMove()->SetStep(TO, 1);
			if( !IsPremiumChaMove() )
				ShowPremiumChaMove(TRUE);

			DeletePopup();
			return 1;
		}
		else if( nMsg == -1 )
		{
			g_pTcpIp->SendNetMessage("premium chmove_req_stop\n");
			// IDS_PREMIUM_MSG_21 캐릭터 이동이 종료되었습니다.
			InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_21), TYPE_NOR_OK );
			DeletePopup();
			SetProcLock(FALSE);
			return 1;
		}

		break;

	case POPUP_PREMIUM_CHA_MOVE_NAME_INPUT:
		if( nMsg == 1)
		{
			if( !strncmp( m_Popup->pMsg_Popup.m_InputLine.GetStrBuf(), g_pRoh->m_pstrName ,  strlen(g_pRoh->m_pstrName) )  )
			{
				DeletePopup();
				// IDS_PREMIUM_MSG_22 본인 계정으로 이동할 수 없습니다.
				InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_22), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				// g_pNk2DFrame->ShowPremiumChaMove(FALSE)	;
				OnClosePremiumChaMove();
				return 1;
			}
			else if( strlen(m_Popup->pMsg_Popup.m_InputLine.GetStrBuf() ) == 0	)
			{
				DeletePopup();
				// InsertPopup("  ",TYPE_PREMIUM_CHA_MOVE_NAME_INPUT, POPUP_PREMIUM_CHA_MOVE_NAME_INPUT);
				InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_04), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				OnClosePremiumChaMove();
				return 1;
			}

			char str[128] = {0,};
			sprintf(str,"premium chmove_req_step 1 %s\n", m_Popup->pMsg_Popup.m_InputLine.GetStrBuf() );
			g_pTcpIp->SendNetMessage(str);
			DeletePopup();
			return 1;
		}
		else if( nMsg == -1)
		{
			OnClosePremiumChaMove();
			SetProcLock(FALSE);
			DeletePopup();
			return 1;
		}

		break;

	case POPUP_PREMIUM_CHA_MOVE_FAIL:
		if( nMsg == 1)
		{
			OnClosePremiumChaMove();
			SetProcLock(FALSE);
			DeletePopup();
			return 1;
		}

		break;
	case POPUP_PREMIUM_CHA_MOVE_WAIT:
		return 1;
		break;
	case POPUP_PREMIUM_CHA_MOVE_SUC_EXIT:
		if( nMsg == 1 )
		{
			OnClosePremiumChaMove();
			SetProcLock(FALSE);
			DeletePopup();
			exit(1);
			return 1;
		}
		break;
	}

	//==========================================================================
	// 시간이 지나면 자동으로 사라지는 팝업 처리
	if( m_Popup->nProcess == POPUP_EXCHANGE )
	{
		if(!bStartExchange)
		{
			lastTickTime = timeGetTime();
			bStartExchange = TRUE;
		}
		else
		{
			if (timeGetTime() >= lastTickTime + 30000) // 30초
				nMsg = -1;
		}
	}
	else if( m_Popup->nProcess == POPUP_FRIEND_ADD_REQ )
	{
		if(!bStartAddFriend)
		{
			lastTickTime = timeGetTime();
			bStartAddFriend = TRUE;
		}
		else
		{
			if(timeGetTime() >= lastTickTime + 30000)
				nMsg = -1;
		}
	}
	else if( m_Popup->nProcess == POPUP_PARTY )
	{
		if(!bStartParty)
		{
			lastTickTime = timeGetTime();
			bStartParty = TRUE;
		}
		else
		{
			if (timeGetTime() >= lastTickTime + 30000)
				nMsg = -1;
		}
	}
	else if( m_Popup->nProcess == POPUP_REFINE )
	{
		if(!bStartRefine)
		{
			lastTickTime = timeGetTime();
			bStartRefine = TRUE;
		}
		else
		{
			if (timeGetTime()>= lastTickTime + 30000)
				nMsg = -1;
		}
	}
	else if( m_Popup->nProcess == POPUP_PROCESS )
	{
		if(!bStartProcess)
		{
			lastTickTime = timeGetTime();
			bStartProcess = TRUE;
		}
		else
		{
			if (timeGetTime() >= lastTickTime + 30000)
				nMsg = -1;
		}
	}
	//==========================================================================

	//==========================================================================
	// 각 팝업의 메세지 처리
	if( nMsg != 0 )	// 팝업의 확인 또는 취소버튼을 누른 경우

	{
		{
			switch( m_Popup->nProcess )
			{
			case POPUP_MONEYGUILDSTOREHOUSE:
				m_pControlGStoreHouse->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
				break;

			case POPUP_GUILD_CREATE:
				break;

			case POPUP_GUILD_NAMEINPUT:
			case POPUP_GUILD_CONFIRM_REQUEST:
				m_pGuildManager->guild_popMsgproc(hWnd, msg, wParam, lParam, nMsg, m_Popup->nProcess, m_Popup->pMsg_Popup.GetCheckPW());
				break;

			case POPUP_GUILD_MONEYEXCHANGE:
				GetControlGStorehouse()->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
				break;

			case POPUP_GUILD_INVASION_FINISH_RES_OK:
				GetUIWindow()->OnOpenWindow(WINDOW_GUILD_BATTLE_INVASION_INFO);
				break;

			case POPUP_GUILD_RANKWAR_INVITE:
				if( nMsg == 1)
					pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_GUILD_WAR, 0);

				if( nMsg == -1 )
					return 0;

				break;
			default:

				if(m_pGuildManager)
					m_pGuildManager->guild_popMsgproc(hWnd, msg, wParam, lParam, nMsg, m_Popup->nProcess,  m_Popup->pMsg_Popup.GetCheckPW());
				break;
			}
		}


		if( m_Popup->nProcess == 1 )	// 확인 팝업

		{}
		else if( m_Popup->nProcess == POPUP_GUILDOUT )	// 확인,취소 팝업

		{
			if( nMsg == 1 && g_pTcpIp )
				g_pTcpIp->SendNetMessage("mg leave\n");	// 길드 탈퇴 선택
		}
		else if( m_Popup->nProcess == POPUP_WINDOW_CLOSE )
		{
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			return 1;
			//	return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		else if( m_Popup->nProcess == POPUP_SHOP && m_pControlShop )
		{
			if( nMsg == 1 )
				m_pControlShop->WarningPopup(hWnd, msg, wParam, lParam);
			else if( nMsg == -1 )
				pCMyApp->m_bInverseReturn = FALSE;
		}
		else if( m_Popup->nProcess == POPUP_USESHOP_YESNO )
			m_pControlUserShop->PopupMsg(hWnd, msg, wParam, lParam, nMsg, &m_Popup->pMsg_Popup);
		else if( m_Popup->nProcess == POPUP_EVENTANCIENT )
			m_pEventAncient->PopupMsg(hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_GUILDPK )
			m_pGuildPK->MsgProc( hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_INTERCHANGE )
			m_pInterchange->PopupMsg( hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_EXCHANGE )
		{
			m_pControlExchange->PopupMsgProc( hWnd, msg, wParam, lParam, nMsg);
			bStartExchange = FALSE;
		}
		else if( m_Popup->nProcess == POPUP_PARTY )
		{
			m_pControlParty->PopupMsgProc( hWnd, msg, wParam, lParam, nMsg);
			bStartParty = FALSE;
		}
		else if( m_Popup->nProcess == POPUP_REFINE )
		{
			if (nMsg == 1 && m_bShowRefine)
				return 1;

			m_pRefine->PopupMsgProc( hWnd, msg, wParam, lParam, nMsg);
			bStartRefine = FALSE;
		}
		else if( m_Popup->nProcess == POPUP_PROCESS )
		{
			if (nMsg == 1 && m_bShowProcess)
				return 1;

			m_pProcess->PopupMsgProc( hWnd, msg, wParam, lParam, nMsg);
			bStartProcess = FALSE;
		}
		else if( m_Popup->nProcess == POPUP_QUEST )
			((CQuestData*)m_pInterface[QUEST])->PopupMsgProc( hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_GATE )
		{
			((CControlGate*)m_pInterface[GATE])->PopupMsgProc( hWnd, msg, wParam, lParam, nMsg);
			pCMyApp->m_pUIMgr->m_bNextGoto = FALSE;
		}
		else if( m_Popup->nProcess == POPUP_MONEYEXCHANGE )
			m_pControlExchange->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
		else if( m_Popup->nProcess == POPUP_REQUESTPARTY )
		{
			m_pControlParty->ReQuestPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
			if (g_dwLangType == 1)
				m_Popup->pMsg_Popup.m_InputLine.CloseIMC();
		}
		else if( m_Popup->nProcess == POPUP_MONEYUSERSHOP )
			m_pControlUserShop->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, &m_Popup->pMsg_Popup);
		else if( m_Popup->nProcess == POPUP_MONEYWAREHOUSE )
			m_pControlWareHouse->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
		else if( m_Popup->nProcess == POPUP_MONEYPROCESS )
			m_pProcess->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
		else if( m_Popup->nProcess == POPUP_MONEYREFINE )
			m_pRefine->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
		else if( m_Popup->nProcess == POPUP_PETNAME )
		{
			m_pControlPetStatus->NamePopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
			if( g_dwLangType == 1 )
			{
				char *found_sp = NULL;
				if((found_sp = ExamSpChar_for_jpn(m_Popup->pMsg_Popup.GetCheckPW())) != NULL)
				{
					char msgStr[100] = "";
					//-- IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
					sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), found_sp);
					InsertPopup( msgStr, TYPE_EXCHANGE, 22 );
				}
			}
		}
		else if( m_Popup->nProcess == POPUP_MONEYWARTEX )	// 인터페이스는 ui가 아니라 nk2dframe에 있어야 한다..누가좀 옯겨 주세요


		{
			if( pCMyApp->m_pUIMgr->m_pWarTax )
				pCMyApp->m_pUIMgr->m_pWarTax->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
		}
		else if( m_Popup->nProcess == POPUP_PASSMATLIST )	// 인터페이스는 ui가 아니라 nk2dframe에 있어야 한다..누가좀 옯겨 주세요


		{
			if( pCMyApp->m_pUIMgr->m_pMatList )
				pCMyApp->m_pUIMgr->m_pMatList->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
		}
		else if( m_Popup->nProcess == POPUP_WARNINGINVEN )
			m_pControlInven->WarningPopupMsg(hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_DOCUMENT )	// 확인,취소 팝업


			((CControlDocument*)m_pInterface[DOCUMENT])->PopupMsgProc(hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_TIMEITEM )	// 시간제 아이템 버릴시 확인,취소 팝업

		{
			if( nMsg == 1 && g_pTcpIp )
			{
				g_pTcpIp->SendNetMessage("drop 1\n");
				SAFE_DELETE(g_pRoh->m_ExtraSlot);
			}
		}
		else if( m_Popup->nProcess == POPUP_CASHSHOP_BUY ) ///유료상점
			m_pControlCashShop->BuyPopupMsg(hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_NPCUPGRADE_BROKEN ) // 강화시 깨짐 경고.
			m_pUpgrade->BrokenPopupMsg(hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_PET_UPGRADE ) // 펫 진화,스킬업 확인 창.
			m_pPet_Level->PopupMsg(hWnd, msg, wParam, lParam, nMsg);
		else if( m_Popup->nProcess == POPUP_NPCUPGRADE_BROKEN_CHECK ) // 강화시 깨짐 경고.(서버 체크)
		{
			if( nMsg != 1 ) // 확인이 아니면
			{
				g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.
				g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
				g_pNk2DFrame->GetUpgrade()->m_Result = 0; // 결과값 초기화...
			}
			else
			{
				g_pTcpIp->SendNetMessage("npc_up armor_check\n");
			}
		}
		else if( m_Popup->nProcess == POPUP_NPCUPGRADE_RESULT ) // 강화시 결과 창. 확인버튼만 있는 타입이지만 키보드로 꺼지지 않게 하기위해 별도 처리.
		{
			if( nMsg != 1 ) // 확인이 눌린게 아니면

				return 1; // 팝업 제거를 거치지 않고 함수를 빠져나간다.
			else
				pCMyApp->m_bInverseReturn = FALSE; // 리턴을 막기위해 셋팅했던 변수를 정상으로 돌려준다.
		}
		else if( m_Popup->nProcess == POPUP_HELPER && m_pControlHelper )  //후견인
		{
			m_pControlHelper->PopupMsgProc( hWnd, msg, wParam, lParam, nMsg);
			m_bHelper = FALSE;
		}
		else if( m_Popup->nProcess == POPUP_TRAINEEGIVEUP )  //후견인
		{
			if( nMsg == 1 && g_pTcpIp )
				g_pTcpIp->SendNetMessage( "support disband\n" );
		}
		else if( m_Popup->nProcess == POPUP_HELPER_REJECT && m_pControlHelper )  //후견인 관계 포기

		{
			m_pControlHelper->PopupMsgProc2( hWnd, msg, wParam, lParam, nMsg);
		}

		else if( m_Popup->nProcess == POPUP_RESURRECTION )
		{
			if( nMsg == 1 && g_pTcpIp )
			{
				g_pTcpIp->SendNetMessage( "revival\n" );
				g_pRoh->m_DeadFrame = -1;
			}
			else if( nMsg == -1 ) // 부활 확인은 취소가 없다.(ESC를 누르면 이 경우로 들어올 수 있지만 이때 팝업을 지우면 안된다. // 07-03-26 원석 추가.
				return 1; // 팝업 제거를 거치지 않고 함수를 빠져나간다.
		}
		else if( m_Popup->nProcess == POPUP_910_ENTER )	// 데카렌1층 갈꺼닞 물어보는것.
		{
			if( nMsg == 1 && g_pTcpIp )
			{
				pCMyApp->SetTransTime();
				g_pTcpIp->SendNetMessage( "kroom make\n" ); // 1층으로 가겠다고 서버에게 알림.
			}
		}
		else if( m_Popup->nProcess == POPUP_910_OUT )	// 데카렌1층 갈꺼닞 물어보는것.
		{
			if( nMsg == 1 && g_pTcpIp )
			{
				pCMyApp->SetTransTime();
				g_pTcpIp->SendNetMessage( "kroom tout\n" ); // 1층으로 가겠다고 서버에게 알림.
			}
		}
		else if( m_Popup->nProcess == POPUP_GO_DEKAREN1 )	// 데카렌1층 갈꺼닞 물어보는것.
		{
			if( nMsg == 1 && g_pTcpIp )
			{
				pCMyApp->SetTransTime();
				g_pTcpIp->SendNetMessage( "go_inst in 1\n" ); // 1층으로 가겠다고 서버에게 알림.
			}
		}
		else if( m_Popup->nProcess == POPUP_GO_DEKAREN2 )	// 데카렌2층 갈꺼냐고 물어보는것.
		{
			if( nMsg == 1 && g_pTcpIp )
			{
				pCMyApp->SetTransTime();
				g_pTcpIp->SendNetMessage( "go_inst in 2\n" ); // 2층으로 가겠다고 서버에게 알림.
			}
		}
		else if( m_Popup->nProcess == POPUP_GO_DEKAREN3 )	// 1층에서 대기실가기

		{
			if( nMsg == 1 && g_pTcpIp )
			{
				pCMyApp->SetTransTime();
				g_pTcpIp->SendNetMessage( "go_inst out\n" ); // 대기실으로 가겠다고 서버에게 알림.
			}
		}
		else if( m_Popup->nProcess == POPUP_LETS_PVP )	///자유대련
		{
			if( g_pTcpIp )
			{
				if( nMsg == 1 )
					g_pTcpIp->SendNetMessage( "pvp 2 1\n" ); // 승낙
				else
					g_pTcpIp->SendNetMessage( "pvp 2 0\n" ); // 거절
			}
		}
		else if( m_Popup->nProcess == POPUP_MULTI_SALE )	///다중판매
		{
			if( nMsg == 1 && g_pTcpIp )
			{
				g_pTcpIp->SendNetMessage( m_pControlInven->m_MultiSaleString ); // 패킷 자체를 스트링에 기억하고 있자.
			}
		}
		else if( m_Popup->nProcess == POPUP_MULTI_SALE_WARRING ) // 다중 판매인데 스페셜 아이템 등이라서 경고 띄워야하는 경우.	///다중판매
		{
			if( nMsg == 1 && g_pTcpIp ) // 경고에 대한 확인이므로 다시 다중 판매 메세지를 물어야한다.
			{
				// 아이템을 저장해둔 메세지로 부터 찾아낸다.
				CItem *pItem = NULL;
				int SlotX, SlotY;
				int keeper, pack, Count;
				char strTemp[256] = "";

				sscanf( m_pControlInven->m_MultiSaleString, "%s %d %d %d %d %d", strTemp, &keeper, &pack, &Count, &SlotX, &SlotY ); // 저장해둔 패킷으로부터 제일 처음 슬롯 x,y를 얻어온다.

				if( pack == g_pRoh->m_CurPackNum ) // 혹시 모르니 체크.
				{
					pItem = g_pRoh->FindItemByIndexFromAll( g_pRoh->GetSlotIndex(SlotX, SlotY) ); // 슬롯으로 부터 아이템을 찾는다.
				}

				if( pItem )
				{
					std::string str = boost::str(boost::format(G_STRING(IDS_MULTI_SALE)) % GET_ITEM_NAME_VNUM(pItem->m_Vnum) % Count);
					sprintf(strTemp, "%s", str.c_str());

					InsertPopup( strTemp,TYPE_NOR_OKCANCLE, POPUP_MULTI_SALE );
				}
			}
		}
		else if( m_Popup->nProcess == POPUP_WAREHOUSE_CHECK ) //  [11/19/2007 반재승] : 일본 버그 요청사항-기간제 아이템일때 띄우는 메세지

		{
			if( nMsg == 1 )
			{
				if( strcmp( m_strCheckWareHouse, "" ) != 0 )
				{
					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage( m_strCheckWareHouse );
				}
			}
		}

		else if( m_Popup->nProcess == POPUP_DMITRON_BATTLE_REQUEST )
		{
			if( nMsg == 1 && g_pTcpIp )
			{
				char Buf[256];
				CDmitronInfo::SelectZone( (int)m_Popup->pArgs );
				sprintf(Buf, "occp bt req %d\n", (int)m_Popup->pArgs);
				g_pTcpIp->SendNetMessage(Buf);
			}

		}
		else if( m_Popup->nProcess == POPUP_DMITRON_TAX_PAYMENT )
		{
			CDmitronBattleTax* pTax = (CDmitronBattleTax*)GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE_TAX);
			if( pTax ) pTax->MoneyPopupMsgProc( hWnd, msg, wParam, lParam, nMsg, m_Popup->pMsg_Popup.GetCheckPW());
		}
		else if (m_Popup->nProcess == POPUP_STASH_FROM_WAREHOUSE_PROC)
		{
			m_pControlWareHouse->PopupMsgProc(hWnd, msg, wParam, lParam, nMsg);
		}
		else if (m_Popup->nProcess == POPUP_STASH_FROM_INVEN_PROC)
		{
			m_pControlInven->PopupMsgProc(hWnd, msg, wParam, lParam, nMsg);
		}
		else if( m_Popup->nProcess == POPUP_FRIEND_ADD_REQ )
		{
			bStartAddFriend = FALSE;
			if( nMsg == 1 )
			{
				SendAddFriend(2, (char*)m_Popup->pArgs);
				SAFE_DELETE_ARRAY(m_Popup->pArgs);
			}
			else if( nMsg == -1 )
			{
				SendAddFriend(3, (char*)m_Popup->pArgs);
				SAFE_DELETE_ARRAY(m_Popup->pArgs);
			}
		}

		else if( m_Popup->nProcess == POPUP_RECYCLE_ITEM )
		{
			int itemIndex = (int)m_Popup->pArgs;
			if( nMsg == 1 )
			{
				g_pTcpIp->SendNetMessage("r_mov\n");
			}
		}

		else if( m_Popup->nProcess == POPUP_GMCOMMAND_LOADITEM )
		{
			if( nMsg == 1 )
			{
				char buf[MAX_PATH] = {0,};
				int itemIndex = (int)m_Popup->pArgs;
				sprintf(buf, "load %d\n", itemIndex);
				g_pTcpIp->SendNetMessage(buf);
			}
		}
		else if( m_Popup->nProcess == POPUP_GMCOMMAND_LOADITEM_10)
		{
			if( nMsg == 1 )
			{
				char buf[MAX_PATH] = {0,};
				int itemIndex = (int)m_Popup->pArgs;
				sprintf(buf, "load %d 10\n", itemIndex);
				g_pTcpIp->SendNetMessage(buf);
			}
		}
		else if( m_Popup->nProcess == POPUP_GMCOMMAND_GOMOB )
		{
			if( nMsg == 1 )
			{
				char buf[MAX_PATH] = {0,};
				int mobVnum = (int)m_Popup->pArgs;
				sprintf(buf, "gomob %d", mobVnum);
				DevTestGmCommand::GetInstance()->run(buf);
			}
		}
		else if( m_Popup->nProcess == POPUP_GMCOMMAND_GONPC )
		{
			if( nMsg == 1 )
			{
				char buf[MAX_PATH] = {0,};
				int mobVnum = (int)m_Popup->pArgs;
				sprintf(buf, "gonpc %d", mobVnum);
				DevTestGmCommand::GetInstance()->run(buf);
			}
		}
		else if( m_Popup->nProcess == POPUP_CHECK_SUMMON_NPC_NOTIFY )
		{
			if( nMsg == 1 )
			{
				int itemIndex = (int)m_Popup->pArgs;
				CItem* pItem = g_pRoh->FindItemByIndexFromAll(itemIndex);
				if( pItem != NULL )
				{
					pCMyApp->m_pUIMgr->Send_UseQuickItem(pItem);
				}
			}
		}
		else if( m_Popup->nProcess == POPUP_EVENT_MAD_INPUT )
		{	
			if( nMsg == 1 )
			{
				char* name = m_Popup->pMsg_Popup.m_InputLine.GetStrBuf();
				char* temp = new char[MAX_PATH];
				memset(temp, 0, MAX_PATH);
				strcpy(temp, name);

				char buf[MAX_PATH] = {0};
				sprintf(buf, (char*)G_STRING(IDS_EVENT_MAD_SURE), name);

				InsertPopup(buf, TYPE_NOR_OKCANCLE, POPUP_EVENT_MAD_SURE, 30, 0, temp);
			}
		}
		else if( m_Popup->nProcess == POPUP_EVENT_MAD_SURE )
		{
			if( nMsg == -1 )
			{
				InsertPopup((char*)G_STRING(IDS_EVENT_MAD_INPUT), TYPE_EXCHANGE, POPUP_EVENT_MAD_INPUT);
			}
			else if( nMsg == 1 )
			{
				if( m_Popup->pArgs )
				{
					std::string charName = (char*)m_Popup->pArgs;
					SendEventMadCharName(charName);
				}
			}

			SAFE_DELETE_ARRAY(m_Popup->pArgs);
		}
		else if( m_Popup->nProcess == POPUP_EVENT_MAD_ERROR )
		{
			if( nMsg == 1 )
			{
				InsertPopup((char*)G_STRING(IDS_EVENT_MAD_INPUT), TYPE_EXCHANGE, POPUP_EVENT_MAD_INPUT);
			}
		}
		else if( m_Popup->nProcess == POPUP_MASTER_DELEGATION )
		{
			bool bConfirm = false;			
			if( nMsg == 1 )
			{
				bConfirm = true;
			}

			SendMasterDelegation(bConfirm);
		}
		else if( m_Popup->nProcess == POPUP_CHAT_SHOUT )
		{
			if( nMsg == 1 )
			{
				char* msg_shout = m_Popup->pMsg_Popup.m_InputLine.GetStrBuf();
				if( strlen(msg_shout) > 100 )
				{
					InsertPopup((char*)G_STRING(IDS_SHOUT_OVERLENGTH), TYPE_NOR_OK);
					DeletePopup();
					return 1;
				}
				else if( strlen(msg_shout) <= 0 )
				{
					InsertPopup((char*)G_STRING(IDS_SHOUT_UNDERLENGTH), TYPE_NOR_OK);
					DeletePopup();
					return 1;
				}

				int slot_num = (int)m_Popup->pArgs;
				if( slot_num < 0 || slot_num >= QUICK_SLOT_NUM*QUICK_SLOT_PAGE )
				{					
					InsertPopup((char*)G_STRING(IDS_SHOUT_NOITEM), TYPE_NOR_OK);
					DeletePopup();
					return 1;
				}
				CItem* pItem = g_pRoh->m_QuickSlot[slot_num];
				if( pItem == NULL )
				{
					InsertPopup((char*)G_STRING(IDS_SHOUT_NOITEM), TYPE_NOR_OK);
					DeletePopup();
					return 1;
				}

				int type = GET_SHAPE(pItem);				
				int index = pItem->m_Index;				

				ShoutChatMgr* shoutMgr = ShoutChatMgr::GetInstance();
				shoutMgr->Clear();
				shoutMgr->SetSlotNum(slot_num);
				shoutMgr->SetItemType(type);
				shoutMgr->SetItemIndex(index);
				shoutMgr->SetMessage(msg_shout);

				InsertPopup((char*)G_STRING(IDS_SHOUT_CONFIRM), TYPE_NOR_OKCANCLE, POPUP_CHAT_SHOUT_CONFIRM);
			}
		}
		else if( m_Popup->nProcess == POPUP_CHAT_SHOUT_CONFIRM )
		{
			if( nMsg == 1 )
			{
				ShoutChatMgr* shoutMgr = ShoutChatMgr::GetInstance();
				int slotNum = shoutMgr->GetSlotNum();
				int itemType = shoutMgr->GetItemType();
				int itemIndex = shoutMgr->GetItemIndex();
				std::string strMessage = shoutMgr->GetMessage();

				SendShout(itemType, slotNum, itemIndex, strMessage);
			}
		}		
		else if( m_Popup->nProcess == POPUP_BATTLELEAGUE )
		{
			if( nMsg == 1 )
			{
				SendPromotion();
			}
		}
		else if( m_Popup->nProcess == POPUP_BINGO_TEN_TIME )
		{
			if( nMsg == 1 )
			{
				ControlBingo* pBingo = (ControlBingo*)m_pInterface[BINGO];				
				pBingo->ActionRegist(eBingoRegType_Many);
			}
		}
		else if( m_Popup->nProcess == POPUP_GIFTTIME )
		{
			if( nMsg == 1 )
			{
				SendGiftTime();
			}
		}
		else if( m_Popup->nProcess == POPUP_PET_EQUIP )
		{
			if( nMsg == 1 )
			{
				int pack = -1;
				CItem* pItem = g_pRoh->FindItemAndPackByIndexFromAll((int)m_Popup->pArgs, pack);
				if(pItem != NULL && pack != -1)
				{
					SendPetEquip(pack, pItem->m_SlotX, pItem->m_SlotY);
				}				
			}
		}

		DeletePopup();	// 확인이든 취소든 누르면 팝업 제거
		return 1;
	}
	else
	{
		{
			if(m_Popup->nProcess == POPUP_GUILD_RECOMMAND)
				return 1;

			if(m_Popup->nProcess == POPUP_GUILD_NAMEINPUT)
				return 1;
			if(m_Popup->nProcess == POPUP_REQUESTPARTY)
				return 1;
			if(m_Popup->nProcess == POPUP_GUILD_RECOM_MEMBER)
				return 1;
		}
	}
	//==========================================================================

	return 0; // 06-10-02 디폴트 리턴값 1에서 0으로 바꿈.(원석) , 이게 1이면 뒤에 메세지들이 무시되서 창이동등의 메세지가 정상적으로 처리안되는 문제가 있었다. 그러나 이걸 0으로 바꿈으러서 메세지 팝업이 오작동할 가능성도 있으니 주의!
}


bool CNk2DFrame::IsPopUpTyping() // 팝업 입력을 받고 있는지의 여부 체크함수.
{
	if( !m_Popup ) // 팝업이 없으면 의미 없음.
		return FALSE;

	if( m_Popup->nProcess == POPUP_MONEYUSERSHOP
			|| m_Popup->nProcess == POPUP_MONEYEXCHANGE
			|| m_Popup->nProcess == POPUP_MONEYWAREHOUSE
			|| m_Popup->nProcess == POPUP_MONEYPROCESS
			|| m_Popup->nProcess == POPUP_MONEYREFINE
			|| m_Popup->nProcess == POPUP_PETNAME
			|| m_Popup->nProcess == POPUP_MONEYWARTEX
			|| m_Popup->nProcess == POPUP_GUILD_MONEYEXCHANGE
	  ) // 입력기능이 있는 팝업 종류인지..
	{
		if( m_Popup->pMsg_Popup.m_InputLine.GetEnable()
				|| m_Popup->pMsg_Popup.m_InputValue1.GetEnable()
				|| m_Popup->pMsg_Popup.m_InputValue2.GetEnable()
				|| m_Popup->pMsg_Popup.m_InputValue3.GetEnable()
				|| m_Popup->pMsg_Popup.m_InputValue4.GetEnable()
				|| m_Popup->pMsg_Popup.m_InputValue5.GetEnable()
		  )
			return TRUE;
	}

	// 입력기능이 없으면 상관없음.
	return FALSE;
}

void CNk2DFrame::ShowCashShopWindow( BOOL bShow ) ///유료상점
{
	if(Check_All_MsgPopUp()) // 2006 버젼 과는 처리가 약간 달라졌다.
		return;
//	if(m_bAutoMouse) ///07-04-12 숫자확인 시스템 타 인터페이스와 함께 쓸 수 있게 수정.
//		return;

	CloseAllWindow();

	m_bShowCashShop = bShow; // CloseAllWindow() 실패시 처리를 안 막는 예외적인 녀석들은 끌때도 이걸 셋팅해줘야한다.

	if( bShow )
	{
		if( m_bShowCashShopIntro ) ///유료상점
		{
			ShowCashShopIntroWindow( FALSE );
		}
	}
	else // 끌때..
	{
		if( m_pControlCashShop )
		{
			m_pControlCashShop->ClearDta(); // Dta는 열때 마다 새로 로딩.(메모리 절약을 위해)
		}
	}
}

void CNk2DFrame::ToggleCashShopWindow( int type ) // type으로 다양한 상점에서 사용할수있도록.... ///유료상점
{
	if( !m_pControlCashShop ) // 혹시라도 없을경우
		return;

	if( g_pRoh->m_HaveCash <= 0 )
	{	
		return;
	}

	ShowCashShopWindow( !m_bShowCashShop );

	if( IsCashShopVisible() )
	{
		if( !m_pControlCashShop->m_bResLoad )
		{
			m_pControlCashShop->LoadRes();

		}

		m_pControlCashShop->m_iShopKind = type;

		if( type == CASHSHOP_TYPE_NORMAL )
		{
			m_pControlCashShop->ClearDta();

			m_pControlCashShop->ReadDta( type, "./data/CashShop.dta" );

			if( !m_pControlCashShop->m_DtaData[type].m_bImageLoad )
			{
				m_pControlCashShop->LoadFromDta( type );
			}
		}

		m_pControlCashShop->OpenInit(); // 열릴때마다 초기화해줄 녀석들.
	}
	else
	{

	}
}

void CNk2DFrame::ShowCashShopIntroWindow( BOOL bShow ) ///유료상점
{
	if(Check_All_MsgPopUp()) // 2006 버젼 과는 처리가 약간 달라졌다.
		return;
//	if(m_bAutoMouse) ///07-04-12 숫자확인 시스템 타 인터페이스와 함께 쓸 수 있게 수정.
//		return;

	CloseAllWindow();

	m_bShowCashShopIntro = bShow; // CloseAllWindow() 실패시 처리를 안 막는 예외적인 녀석들은 끌때도 이걸 셋팅해줘야한다.

	if( bShow )
	{
		if( m_bShowCashShop ) ///유료상점
			ShowCashShopWindow( FALSE );
	}
	else // 끌때..
	{
		if( m_pCashShopIntro )
			m_pCashShopIntro->Close(); // 웹 페이지 윈도우를 숨겨준다.
	}
}

void CNk2DFrame::ToggleCashShopIntroWindow( int type ) // type으로 다양한 상점에서 사용할수있도록.... ///유료상점
{
	if( !m_pCashShopIntro ) // 혹시라도 없을경우

		return;

	if( GetClientWidth() == 640 ) // 640*480 모드에서는 일단 캐쉬샵이 안켜지도록...
	{
		//-- IDS_NOT_SURRPOT_DISPLAY : 현재 게임 해상도에서는 지원하지 않습니다
		pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_NOT_SURRPOT_DISPLAY) );
	}

	ShowCashShopIntroWindow( !m_bShowCashShopIntro );
	if( IsCashShopIntroVisible() )
	{
		m_pCashShopIntro->Open();

		sprintf(commOutBuf, "multi_shop cash\n" ); // 캐쉬 갱신 요청 패킷을 한번 보낸다.(태국 라카쪽 캐쉬사용때문에 필요..)
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(commOutBuf);
	}
}

BOOL CNk2DFrame::CheckFullScreenInterface() // 화면을 독점적으로 사용하는 인터페이스가 켜져있는지 체크하는 함수. ///유료상점시 추가
{
	// 해상도 별로 갈린다.
	switch( GetClientWidth() )
	{
	case 640:
		break;
	case 800:

		if( IsCashShopVisible() ) // 유료상점
			return TRUE;

		if( IsCashShopIntroVisible() ) // 유료상점 인트로
			return TRUE;

		break;
	case 1024:
		break;
	}

	return FALSE;
}

void CNk2DFrame::ShowHelperWindow(BOOL bShow) //후견인
{
	if( !CloseAllWindow(FALSE) )
		return;

	if( bShow )
	{
		m_bHelper = bShow;
	}
}

void CNk2DFrame::ShowHelpPetWindow( BOOL bShow, int Type ) ///펫도움말
{
	// 이 녀석은 조금 처리가 특이하다. 3D형 인터페이스라 다른 인터페이스를 끄지 않는다.
	if( !bShow ) // 끄는 경우..
	{
		// 그냥 메모리 해제해 버린다.
		if( m_pControlHelpPet )
		{
			SAFE_DELETE( m_pControlHelpPet );
		}

		m_bHelpPet = false;

		///펫도움말확장

		FILE *fp;
		fp = fopen( "data/etc/OnOff.txt", "wt" ); // 켰었는지 껏었는지 저장.
		fprintf( fp, "0\n" ); // 껏다.
		fclose( fp );
	}
	else // 키는 경우.
	{
		SAFE_DELETE( m_pControlHelpPet );

		m_pControlHelpPet = new CControlHelpPet;
		if( !m_pControlHelpPet )
			return;
		m_pControlHelpPet->LoadRes( Type );

		m_pControlHelpPet->SetPetAnimation( HELPPET_ANI_NORMAL ); // 초기화

		// 등장 대사.
		if( g_pRoh->m_Chaotic_Grade ) // 이게 0이 아니면 카오상태다.
			// IDS_PETHELP_GREET_CHAO : 나 좀 귀찮게 하지말고 알아서 하면 안되겠니?
			m_pControlHelpPet->ExcuteHelpTalk( (char *)G_STRING(IDS_PETHELP_GREET_CHAO) );  // 인사 대사를 넣어준다.
		else
			// IDS_PETHELP_GREET_PEACE : 라그하임 기획자들도 인정한 이 라그박사\\n 라비에게 어서 물어보란말이지

			m_pControlHelpPet->ExcuteHelpTalk( (char *)G_STRING(IDS_PETHELP_GREET_PEACE) ); // 인사 대사를 넣어준다.

		m_bHelpPet = true;

		///펫도움말확장

		// by evilkiki 2009.09.17
		FILE *fp;
		fp = fopen( "data/etc/OnOff.txt", "wt" ); // 켰었는지 껏었는지 저장.
		fprintf( fp, "1\n" ); // 켰다.
		fclose( fp );
	}
}


void CNk2DFrame::ShowControlGuild(BOOL bShow)
{

	if( !CloseAllWindow(FALSE) )
		return;

	if( bShow )
	{
		m_bShowControlGuild = bShow;

		if(g_pRoh->GetMyGuildData() != NULL)
		{
			m_pControlGuild->Set_btn_status(TRUE, g_pRoh->GetMyGuildLevel());

			m_pControlGuild->set_revision(g_pRoh->GetMyGuildRevisionTime());
		}
		else
			m_pControlGuild->Set_btn_status(FALSE, -1);
	}
	else
		m_pControlGuildManager->HookClose();
}

void CNk2DFrame::ShowControlGuildManage(BOOL bShow)
{
	if( !CloseAllWindow(FALSE) )
		return;

	if( bShow )
	{
		m_bShowControlGuildManage = bShow;

		if(g_pRoh->GetMyGuildData() != NULL)
		{
			if(g_pRoh->GetMyGuildLevel() == JOB_MASTER)	m_pControlGuildManager->set_btn_status(TRUE);

			if(g_pRoh->GetMyGuildLevel() == JOB_ASSISTANTMASTER) m_pControlGuildManager->set_btn_status(FALSE);
		}
	}
	else
		m_pControlGuildManager->HookClose();
}

void CNk2DFrame::ShowControlGuildBattle(BOOL bShow)
{

	if( bShow )
	{
		GetControlGuildBattle()->LoadResource();
		GetControlGuildBattle()->SetActive(true);
	}
	else
	{
		SAFE_DELETE_RES(GetControlGuildBattle());
		GetControlGuildBattle()->SetActive(false);
	}
}


void CNk2DFrame::ShowPVPWidget( BOOL bShow )
{
	m_bShowPVPWidget = bShow;

	if (m_pPVPWidget)
	{
		if (bShow == TRUE)
			m_pPVPWidget->InitVariables();
	}

}

BOOL CNk2DFrame::IsItemMultiMoveEnable() // 인벤 오른쪽 더블클릭시 다중이동 기능을 사용하는 창이 떠있나 체크해주는 함수. 물약 인벤에서 사용시 등에서 체크한번 해주도록 추가(07-02-20 원석)
{
	// 교환창. 개인 상점등이 열려있을땐 인벤 오른쪽 클릭으로 사용 못하게
	if( IsUserShopVisible() || GetControlWareHouse()->IsActivate()
			|| IsExchangeVisible() || IsUpgradeVisible() || IsEventAncientVisible()
			|| ( IsGambleVisible() && GetGamble()->m_bType == 0 ) ) // 오른쪽 더블클릭으로 다중이동시키는 창들이 떠잇으면...
		return TRUE;

	return FALSE;
}


// [3/5/2007 Theodoric] 접속중인 서버명 저장

void CNk2DFrame::SetConnectServerName(char *str)
{
	int strSize = strlen(str)+1;
	SAFE_DELETE_ARRAY(m_strConnectServerName);
	m_strConnectServerName = new char[strSize];
	ZeroMemory(m_strConnectServerName, strSize);
	memcpy(m_strConnectServerName, str, strSize );
}

void CNk2DFrame::InitDmitronInfo()
{
	m_pDmitronBattleStatus->Init();
}

void CNk2DFrame::SetDmitronInfo1(int index, int status)
{
	m_pDmitronBattleStatus->SetDmitronInfo1(index, status);
}

void CNk2DFrame::SetDmitronInfo2(int index, int status, char* myGuildName, int myGuildPoint,
								 char* topGuildName, int topGuildPoint, int lastTime, int elapsedTime)
{
	m_pDmitronBattleStatus->SetDmitronInfo2(index, status, myGuildName, myGuildPoint,
											topGuildName, topGuildPoint, lastTime, elapsedTime);
}

void CNk2DFrame::DisplayDmitronInfo()
{
	m_pDmitronBattleStatus->MakeDrawPopup();
}

void CNk2DFrame::ShowPremiumExchange(int type, int nSlot)
{
	m_bShowPremiumExchange = TRUE;
	m_pPremiumExchange->SetInfo(type, nSlot);
}

void CNk2DFrame::HidePremiumExchange()
{
	m_bShowPremiumExchange = FALSE;
}

BOOL CNk2DFrame::CheckPremiumChaMoveInfo()
{
	if( CheckPremiumChamoveScroll() == FALSE )
	{
		return FALSE;
	}


	return TRUE;
}


BOOL CNk2DFrame::CheckPremiumChamoveScroll()
{
	int i,j;//,p;

	if(!g_pRoh)
		return FALSE;

	// 이벤트 의상 검색
	for(i=0; i<WEARING_NUM; i++)
	{
		if( g_pRoh->m_EventWearing[i] != NULL)
		{
			// IDS_PREMIUM_MSG_23 이벤트 의상을 비워주세요.
			InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_23), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
			return FALSE;
		}
	}

	// 마우스에 잡고 있는지 검색

	if( g_pRoh->m_ExtraSlot != NULL)
	{
		// IDS_PREMIUM_MSG_24 마우스로 들고 있는 아이템이 없어야합니다.
		InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_24), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
		return FALSE;
	}

	if( m_pControlExchange )
	{
		for( i=0; i<EXG_SLOT_X_NUM; i++)
			for( j=0; j<EXG_SLOT_Y_NUM; j++)
			{
				if( m_pControlExchange->m_ExgOppSlot[i][j] != -1)
				{
					// IDS_PREMIUM_MSG_25 교환창을 비워주세요.
					InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_25), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
					return FALSE;
				}
			}
	}

	for(i=0; i<QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		if( g_pRoh->m_QuickSlot[i] )
		{
			if(g_pRoh->m_QuickSlot[i]->m_Vnum != 2101)
			{
				// IDS_PREMIUM_MSG_26 퀵슬롯에 캐릭터 이동 마법사를 제외한 다른 아이템은 모두 비워주세요.
				InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_26), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CNk2DFrame::CheckPremiumLetter()
{
	BOOL result;

	result = m_pControlLetter->LetterListMessage(g_nServerIndex,g_pRoh->m_pstrName,g_pRoh->m_nUserIndex,1);
	if( result == FALSE)
	{
		// IDS_PREMIUM_MSG_27 쪽지 목록 및 공지를 모두 비워주세요

		InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_27), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
		return FALSE;
	}

	result = m_pControlLetter->LetterFriendListMessage(g_nServerIndex,pCMyApp->m_strLetterUserId,g_pRoh->m_nUserIndex);
	if( result == FALSE)
	{
		// IDS_PREMIUM_MSG_28 쪽지 친구 리스트를 모두 비워주세요

		InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_28), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
		return FALSE;
	}

	result = m_pControlLetter->LetterBlockListMessage(g_nServerIndex,g_pRoh->m_pstrName);
	if( result == FALSE)
	{
		// IDS_PREMIUM_MSG_29 쪽지 거부 리스트를 모두 비워주세요.
		InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_29), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
		return FALSE;
	}


	return TRUE;
}

BOOL CNk2DFrame::OnClosePremiumChaMove()
{
	// 창이 열려 있다면 닫아주자.
	if( m_bShowPremiumChaMove )
		ShowPremiumChaMove(FALSE);

	SetProcLock( FALSE );

	return  ClossePremiumMsgBox();

}


BOOL CNk2DFrame::ClossePremiumMsgBox()
{
	if( m_Popup == NULL)
		return TRUE;

	// 웨이트 팝업이 떠 있다면 그냥 종료하자.
	POPUP *pPopup = m_Popup;

	switch( pPopup->nProcess )
	{
	case POPUP_PREMIUM_CHA_MOVE_FROM:
	case POPUP_PREMIUM_CHA_MOVE_TO:
	case POPUP_PREMIUM_CHA_MOVE_WAIT:
	case POPUP_PREMIUM_CHA_MOVE_NAME_INPUT:
		m_Popup = pPopup->pNext;
		pPopup->pNext = NULL;
		delete pPopup;
		break;
	}
	pPopup = NULL;

	return TRUE;

}

void CNk2DFrame::PremiumChaMoveWear(int where, int index, int vnum, int gem_num, int special, int special2,
									int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///신방어구강화시스템
{
	m_pPremiumChaMove->Wear(where, index, vnum, gem_num, special, special2, UpEndur, MaxUpdEndur, Endur, MaxEndur);
}

void CNk2DFrame::WareHouseStrCopy(char *str)
{
	if( strcmp( str, "" ) == 0 )
		return;

	memcpy(m_strCheckWareHouse, str, strlen(str));
}


bool CNk2DFrame::IsInPopup(int x, int y)
{
	// top , compas , chatting 클릭시 어떻게 할까요?
	if( !GetControlInven() || !GetControlBottom() )
		return false;

	if( GetControlInven()->IsInside( x, y ) || GetControlBottom()->IsInside( x, y ) )
		return true;

	return false;
}

void CNk2DFrame::SetMyMoney(__int64 money)
{
	if (m_bShop)
		GetControlShop()->SetMoney();

	if( m_bPetLevel )
		GetPetLevel()->SetMoney(money);
}

void CNk2DFrame::InitExpTable()
{	
	m_exp_require[0] = 150;
	m_exp_accumulate[0] = 150;

	for( int i=1 ; i<249 ; i++ ) // 2~249
	{
		if( CommonConfig::Instance()->GetKorFlag() == true )
		{
			m_exp_accumulate[i] = (((UINT64)i*i*i*i/2) + ((UINT64)i*i*i) + ((UINT64)250*i*i) - ((UINT64)3*i) + 352) / 25;
		}
		else if( CommonConfig::Instance()->GetThaFlag() == true )
		{
			m_exp_accumulate[i] = ( ((UINT64)i*i*i*i/2) + ((UINT64)i*i*i) + ((UINT64)250*i*i) - ((UINT64)3*i) + 352 ) / 25;
		} 
		else
		{
			m_exp_accumulate[i] = (((UINT64)i*i*i*i/2) + ((UINT64)i*i*i) + ((UINT64)250*i*i) - ((UINT64)3*i) + 352) / 25;
		}

		m_exp_require[i] = (UINT64)m_exp_accumulate[i] - (UINT64)m_exp_accumulate[i-1];
		_asm nop;
	}

	//
	for( int i=249 ; i< 259 ; i++ ) // 250~259
	{
		m_exp_require[i] = ((UINT64)100000000 * ( (UINT64)(i+1-250) / 10 + 1)) /25 ;
		m_exp_accumulate[i] = (UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}
	for( int i=259 ; i< 339 ; i++ ) // 260~339
	{
		m_exp_require[i] = ((UINT64)100000000 * ( ( (UINT64)(i+1-250) / 10 + 1) + ( (i+1-260) / 10 ) )) / 25;
		m_exp_accumulate[i] = (UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}

	for( int i=339 ; i< 749 ; i++ ) // 340~749   
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*400000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}
	for( int i=749 ; i< 849 ; i++ ) // 749-849i<LH_MAX_LEVEL
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*40000000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}
	for( int i=849 ; i<879 ; i++ ) // 849-879  i<LH_MAX_LEVEL
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*1000000000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}
	for( int i=879 ; i<889 ; i++ ) // 879-889  i<LH_MAX_LEVEL
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*20000000000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}
	for( int i=889 ; i<899 ; i++ ) // 879-889  i<LH_MAX_LEVEL
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*80000000000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}
	for( int i=899 ; i<909 ; i++ ) // 879-889  i<LH_MAX_LEVEL
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*500000000000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}
	for( int i=909 ; i<919 ; i++ ) // 879-889  i<LH_MAX_LEVEL
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*1000000000000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}

	for( int i=919 ; i<LH_MAX_LEVEL ; i++ ) // 889-1000  i<LH_MAX_LEVEL
	{
		m_exp_require[i] = ((UINT64)m_exp_require[i-1] + (UINT64)((UINT64)(i+1)*2000000000000));
		m_exp_accumulate[i] =(UINT64)m_exp_accumulate[i-1] + (UINT64)m_exp_require[i];
		_asm nop;
	}

	m_exp_init = true;
}


//
UINT64 CNk2DFrame::GetExpAccumulate()
{
	if( !m_exp_init || !g_pRoh )
		return 0;

	if( g_pRoh->m_Level < 250 )
		return g_pRoh->m_Exp;

	return m_exp_accumulate[g_pRoh->m_Level-1] + (UINT64)g_pRoh->m_Exp;
}


// 현재레벨에서 필요한 경험치
UINT64 CNk2DFrame::GetExpRequire()
{
	if( !m_exp_init || !g_pRoh )
		return 0;

	return (UINT64)m_exp_require[g_pRoh->m_Level-1];
}

// 현재레벨에서 남은 경험치
UINT64 CNk2DFrame::GetExpRemain()
{
	if( !m_exp_init || !g_pRoh )
		return 0;

	return (UINT64)g_pRoh->m_NeedExp;
}


UINT64 CNk2DFrame::GetExpGet()
{
	if( !m_exp_init || !g_pRoh )
		return 0;

	if( g_pRoh->m_Level == 1 )
		return g_pRoh->m_Exp;
	else if( g_pRoh->m_Level < 250 )
		return g_pRoh->m_Exp - m_exp_accumulate[g_pRoh->m_Level-2];

	return m_exp_require[g_pRoh->m_Level-1] - g_pRoh->m_NeedExp;
}

int CNk2DFrame::GetRSkill()
{
	if( !GetControlBottom() )
		return -1;

	return GetControlBottom()->GetRSkill()->GetCurSkill();
}

int CNk2DFrame::GetLSkill()
{
	if( !GetControlBottom() )
		return -1;

	return GetControlBottom()->GetLSkill()->GetCurSkill();
}

void CNk2DFrame::SetRSkill(int index)
{
	if( GetControlBottom() )
		GetControlBottom()->GetRSkill()->SetCurSkill(index);
}

void CNk2DFrame::SetLSkill(int index)
{
	if( GetControlBottom() )
		GetControlBottom()->GetLSkill()->SetCurSkill(index);
}

bool CNk2DFrame::CreateLHUI(int ui, int param)
{
	enum CreateUIType
	{
		CreateUIType__TakeOutBox = 0,

		CreateUIType__EndOfNum
	};

	switch( (CreateUIType)ui )
	{
	case CreateUIType__TakeOutBox:
		SAFE_DELETE(m_ui_takebox);
		m_ui_takebox = new LHUI_TakeBox((TakeBox_Type)param);
		break;

	default:
		return false;
	}
	return true;
}

void CNk2DFrame::ShowGateWindow(bool show , int gate_value , bool direct)
{
	CloseAllWindow();

	if( show )
	{
		if( GetGate() )
		{
			GetGate()->ResetAll();
			GetGate()->LoadRes();
		}
		else
			m_ui_gate = new LHUI_Gate();

		m_ui_gate->SetZoneList(gate_value , direct);
	}
	else
	{
		if( GetGate() )
			GetGate()->ResetAll();
	}
}


BOOL CNk2DFrame::IsVisibleTakebox()
{
	if( GetTakeOutBox() )
		return GetTakeOutBox()->_GetVisible();

	return false;
}

bool CNk2DFrame::ShowTakeboxWindow( BOOL bShow , int type)
{
	if( bShow )
	{
		pCMyApp->StopHero();

		if( m_dwTakeBox_Time != 0 && GetTickCount() - m_dwTakeBox_Time < 3000 )
		{
			AddChatStr( "Please wait." , -1 );
			return false;
		}

		if( GetTakeOutBox() )
			SAFE_DELETE(m_ui_takebox);

		m_ui_takebox = new LHUI_TakeBox((TakeBox_Type)type);
		m_ui_takebox->m_send_delay = m_dwTakeBox_Time;
		m_ui_takebox->Request_List();

		m_dwTakeBox_Time = GetTickCount();
	}
	else
	{
		SAFE_DELETE(m_ui_takebox);
	}

	return true;
}

bool CNk2DFrame::IsScroll(CItem* pItem)
{
	return (GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == IETC_SCROLL)?true:false;

}


void CNk2DFrame::RenderItemInUI(int x, int y, CItem* pItem, BOOL over, BOOL bMove,BOOL ignore_size, BOOL begin_end, float fScale)
{
	if( !GetItemRender() || !pItem )
		return;

	DWORD ambient = 0x00808080;
	ambient = 0x00cccccc;

	if( over )
	{
		ambient = 0x00ffffff;
	}
	else

	{
// 		if(  IsScroll(pItem)
// 			|| pItem->m_Vnum == 1251
// 			|| pItem->m_Vnum == 1308
// 			|| pItem->m_Vnum == 1286
// 			|| ( pItem->m_Vnum >= 1598 && pItem->m_Vnum <= 1695 )
// 			|| pItem->m_Vnum == 1287 )
// 			ambient = 0x00cccccc;
// 		else
		if (pItem->m_PlusNum > 0)
			ambient = UPGRADE_ITEM_COLOR;
	}

	if( ItemInfoScaleMgr::GetInstance()->IsNoScale(pItem->m_Vnum) == true )
	{
		if( ignore_size == TRUE )
		{
			fScale = 1.0f;
		}
		else
		{
			fScale = 0.5f;
		}
	}

	GetItemRender()->RenderLolo(x, y , GET_LOLO(pItem), GET_LOLOTEXTR_NUM(pItem), ambient, bMove, GET_ITEM_WIDTH_S(pItem), GET_ITEM_HEIGHT_S(pItem), begin_end, -1, -1, ignore_size, fScale);
}

void CNk2DFrame::RenderItemInUIWithSize(int vnum, int x, int y, int sizeX, int sizeY)
{
	int protoNum = g_ItemList.FindItem(vnum);

	if( !GetItemRender() || GET_LOLOC(protoNum) == NULL )
		return;

	DWORD ambient = 0x00cccccc;

	GetItemRender()->RenderLolo(x, y , GET_LOLOC(protoNum), -1, ambient, FALSE, -1, -1, TRUE, sizeX, sizeY, TRUE);
}

BOOL CNk2DFrame::IsFriendVisible()
{
	if( m_ui_friend == NULL )
		return false;

	return m_ui_friend->IsVisible();
}

void CNk2DFrame::ShowFriend( BOOL bShow )
{
	if( m_ui_friend )
		m_ui_friend->SetVisible(bShow);

	return;
}

void CNk2DFrame::ToggleFriend()
{
	if( m_ui_friend == NULL )
		return;

	ShowFriend( ! m_ui_friend->IsVisible() );
}

unsigned __stdcall CNk2DFrame::ThreadLoading(void* pParam )
{
	CMyD3DApplication* app = (CMyD3DApplication*)pParam;
	app->StartGame();

	return 1;
}

unsigned __stdcall CNk2DFrame::ThreadGoWorldLoading( void* pParam )
{
	CMyD3DApplication* app = (CMyD3DApplication*)pParam;
	app->GoWorldLoading();

	return 1;
}
