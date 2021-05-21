#ifndef _NK2D_FRAME_H_
#define _NK2D_FRAME_H_

#include "Config.h"

#include "main.h"
#include "ControlGuildBattle.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

class CBaseInterface;
class CSurface;
class CLolos;
class IndexedTexture;
class CTcpIpConnection;
class CSheet;
class CControlMenu;
class CControlTop;
class CControlBottom;
class CControlInven;
class CNkCharacter;
class CItemRender;
//class CItemRender;
class CControlLetter;
class CControlScore;
class CControlSocial;
class CControlRoyale;
class CControlMap;
class CControlSkill;
class CControlShop;
class CControlExchange;
class CControlHelp;
class CHelp;
class CTextOutBox;
class CControlParty;
class CControlGate;
class CInterchange;
class CControlWareHouse;
class CGamble;

class CLockSmith;

class CRefine;
class CControlUpgrade;	///신방어구강화시스템
class CRShiftMenu;
class CServMsgPopUp;
class CControlPetStatus;
class CLShiftMenu;
class CControlPeiceShop;
class CZoneInfo;
class CProduct;
class CPetProduct;
class CEventAncient;
class CQuestData;
class Cplatinum;
class CSummerEvent;
class CGuildPK;
class CAutoMouse;
class CControlUserShop; ///개인상점
class CControlUShopSearch; ///개인상점검색
class CProcess;	//가공
class CWorldCupStart;
class CControlCashShop; ///유료상점
class CCashShopIntro; ///유료상점.
class CControlHelper; //후견인
class CControlHelpPet; ///펫도움말
class CSpiritMode;
class CAutoNotice;					// 브라질 요청 : 10분 마다 공지 메시지 출력
class CControlStatus;


class CControlGuildSHouse;
class CGuildSystemManager;
class CControlGuild;
class CControlGuildManage;
class CWindowMgr;
class CWINDOWUI;
class CControlGuildBattle;
class CGuildRankWarBattleCount;

class CControlNotice; ///공지팝업창

class CWindowTgaMgr;

class CPVPWidget;

class CDmitronInfo;
class CDmitronBattle;
class CDmitronBattleStatus;

class CControlBossraidInfo;

class CControlHeroFinalRank;

class CPremiumExchange;

class CControlHeroPrivilege;
class CControlHeroRankList;

class CPetLevel;

class CControlPremiumChaMove;

class CControlSelectMenu;

class LHUI_OpEvent;

class LHUI_TakeBox;

class LHUI_Gate;

class CUIFriend;

class UI_BuffItem;

class ControlDecardiBattle;

#define CONTROLMAIN_X			640
#define CONTROLMAIN_Y			122
#define CONTROLMAIN_RIGHT_Y		358
#define CONTROLMAIN_Y_MARGIN	42				// 메인 메뉴에서의 공백부분
#define CONTROLMAIN_EQUIP_X		214
#define CONTROLMAIN_EQUIP_Y		371

enum
{
	SCORE,
	GATE,
	SOCIAL,
	QUEST,
	MAP,
	DOCUMENT,
	YUTBOARD,
	FISHING,
	FISHING_REWARD,
	LOTTO,
	BINGO,
	LETTER_WINDOW,
	LETTER_SEND,
	LETTER_RECV,
	BASEBALL,
	BASEBALL_REWARD,
	LOTTERY,
	DICEGAME,
	DICEGAME_REWARD,
	MAX_NUM
};

#define MAX_MB_NUM 10

// 2D 내에서도 3D를 렌더링한다.   // 지랄..
class CNk2DFrame
{
protected:
	CSheet					*m_pStartSheet;
	// 캐릭터와 장비 셋.
	CCapsyong				**m_dpChaCapsyong;
	NKGRAPHIC_MODE			m_GraphicMode;
	POINT					m_ptCliSize;

	CControlMenu			*m_pControlMenu; // 인터페이스 선택 메뉴.
	CControlLetter			*m_pControlLetter;
	CControlTop				*m_pControlTop;
	CRShiftMenu				*m_pRShiftMenu;
	CLShiftMenu				*m_pLShiftMenu;
	CControlBottom			*m_pControlBottom;
	CControlInven			*m_pControlInven;
	CControlSkill			*m_pControlSkill;
	CControlShop			*m_pControlShop;
	CControlPeiceShop       *m_pControlPeiceShop;
	CControlExchange		*m_pControlExchange;
	CInterchange			*m_pInterchange;
	CProduct				*m_pProduct;
	CPetProduct				*m_pPetProduct;
	CControlHelp			*m_pControlHelp;
	CControlParty			*m_pControlParty;
	CControlPetStatus		*m_pControlPetStatus; // 애완동물 정보창
	CPetLevel				*m_pPet_Level;
	CGamble					*m_pGamble;

	CLockSmith				*m_pLockSmith;

	CEventAncient			*m_pEventAncient;
	Cplatinum               *m_pPlatinum;
	CSummerEvent            *m_pSummerEvent;
//	CQuestData				*m_pQuestData;
	CRefine					*m_pRefine;
	CControlUpgrade			*m_pUpgrade;		// 강화 인터페이스    ///신방어구강화시스템
	CControlNotice          *m_pControlNotice; ///공지팝업창
	CControlUShopSearch     *m_pUserShopSearch; ///개인상점검색
	CGuildPK				*m_pGuildPK;		// 길드전 관련
	CAutoMouse				*m_pAutoMouse;		// AutoMouse(사냥)제한 관련
	CControlWareHouse       *m_pControlWareHouse;
	CControlGuildSHouse		*m_pControlGStoreHouse;
	CGuildSystemManager		*m_pGuildManager;
	CControlGuild			*m_pControlGuild;
	CControlRoyale			*m_pControlRoyale;
	CControlGuildManage		*m_pControlGuildManager;	// 길드 관리 버튼 클릭시
	CControlGuildBattle		*m_pControlGuildBattle;
	CGuildRankWarBattleCount * m_pGuildRankWarBattleCount;
	CControlUserShop        *m_pControlUserShop; ///개인상점
	CControlCashShop        *m_pControlCashShop; ///유료상점
	CCashShopIntro          *m_pCashShopIntro; ///유료상점
	CBaseInterface			**m_pInterface;
	bool					*m_bInterface;
	CProcess				*m_pProcess;
	CWorldCupStart			*m_pWorldCup;
	CControlHelper			*m_pControlHelper;	//후견인
	CControlHelpPet			*m_pControlHelpPet;	///펫도움말
	CSpiritMode				*m_pSpiritMode;     //mungmae-2006/08/30 스피릿 모드 관련

	CAutoNotice				*m_pAutoNotice;		// 브라질 요청 : 10분 마다 공지 메시지 출력

	CControlStatus			*m_pControlStatus;

	CControlBossraidInfo	*m_pBossraidInfo;

	CControlHeroFinalRank	*m_pHeroFinalRank;

	CControlHeroPrivilege	*m_pHeroPrivilege;
	CControlHeroRankList	*m_pHeroRankList;

	CPVPWidget				*m_pPVPWidget;	// 1:1 자유 대련

	CDmitronBattleStatus	*m_pDmitronBattleStatus; // 드미트론 점령전 - 점령 현황

	CPremiumExchange*		m_pPremiumExchange;

	CControlPremiumChaMove*	m_pPremiumChaMove;

	CControlSelectMenu *	m_pControlSelMenu;

	LHUI_OpEvent *			m_ui_op_event;

	LHUI_TakeBox *			m_ui_takebox;

	LHUI_Gate * m_ui_gate;

	CUIFriend * m_ui_friend;

	UI_BuffItem * m_ui_buffitem;

	ControlDecardiBattle * m_ui_decardibattle;

	// Item 창...
	CItemRender				*m_pItemRender;
	// 창...
	BOOL					m_bMenu; // 인터페이스 선택 메뉴.(좀 포괄적으로 쓰일수있는 메뉴이다.)
	BOOL					m_bInven;
	BOOL					m_bLetter;
	BOOL					m_bShop;
	BOOL					m_bPeiceShop;
	BOOL					m_bShowExchange;
	BOOL					m_bInterchange;
	BOOL					m_bProduct;
	BOOL                    m_bPetProduct;
	BOOL					m_bShowGamble;

	BOOL					m_bLockSmith;

	BOOL					m_bShowRefine;
	BOOL					m_bShowUpgrade;  ///신방어구강화시스템
	BOOL					m_bShowNotice;  ///공지팝업창
	BOOL					m_bPetStatus;
	BOOL					m_bHelp;
	BOOL					m_bSkill;
	BOOL					m_bRoyale;
	BOOL					m_bViewTop;
	BOOL					m_bRShift;
	BOOL					m_bLShift;
	BOOL					m_bAutoMouse;
	BOOL					m_bParty;
	BOOL					m_bShowWareHouse;
	BOOL					m_bShowGStoreHouse;
	BOOL					m_bShowGuildManager;
	BOOL					m_bShowControlGuild;
	BOOL					m_bShowControlGuildManage;
	BOOL					m_bShowUserShop; ///개인상점
	BOOL					m_bShowUserShopSearch; ///개인상점검색
	BOOL					m_bShowCashShop; ///유료상점
	BOOL					m_bShowCashShopIntro; ///유료상점
	BOOL					m_bZone;
	BOOL					m_bPetLevel;
	BOOL					m_bShowEventAncient;
	BOOL                    m_bShowPlatinum;	// 플래티늄 관련 변수
	BOOL                    m_bShowSummerEvent;	// 여름 사냥 이벤트 관련 변수
	BOOL					m_bAllLoaded;		// 메시지 침범을 막기 위한 리소스 방어 변수
//	BOOL					m_bQuest;
	BOOL					m_bShowProcess;
	BOOL					m_bHelper;			//후견인
	BOOL					m_bHelpPet;			///펫도움말
	BOOL					m_bStatus;
	BOOL					m_bBossraidInfo;

	BOOL					m_bHeroFinalRank;

	BOOL					m_bHeroPrivilege;
	BOOL					m_bHeroRankList;

	BOOL					m_bShowPVPWidget;

	BOOL					m_bShowDmitronBattleStatus;  // 드미트론 점령전 - 점령 현황

	BOOL					m_bShowPremiumExchange;

	BOOL					m_bShowPremiumChaMove;

	BOOL					m_bShowControlSelMenu;

	CTextOutBox				*m_pInfoBox;		// 서버에서 온 정보를 보여주는 창
	DWORD					m_InfoTime;
	int						m_InfoFrame;
	CTextOutBox				*m_pInfoBox2;
	DWORD					m_InfoTime2;
	int						m_InfoFrame2;
	std::string				m_InfoNextString;
	std::string				m_InfoNextString2;

	// [3/5/2007 Theodoric] 접속중인 서버명 저장
	char	*m_strConnectServerName;
	char    m_strCheckWareHouse[1024]; //  [1/15/2008 반재승] : 일본 버그 요청사항-기간제 아이템일때 띄우는 메세지
public:
	CNk2DFrame();
	virtual ~CNk2DFrame();

	HRESULT RestoreSurfaces();

	// [3/5/2007 Theodoric] 접속중인 서버명 리턴
	char*	GetConnectServerName()
	{
		return m_strConnectServerName;
	}
	void	SetConnectServerName(char *str);

	// 초기화 및 상태관련
	void	Init(int right, int bottom);
	void	SetHwnd(HWND hwnd);
	void	SetWindowRect(RECT &rcWindow);
	void	SetChaSet(CCapsyong **dpChaCapsyong, IndexedTexture *pChaTexture);

	int		GetClientWidth()
	{
		return m_ptCliSize.x;
	}
	int		GetClientHeight()
	{
		return m_ptCliSize.y;
	}
	POINT	GetClientSize()
	{
		return m_ptCliSize;
	}
//	LPDIRECT3DDEVICE7 GetDevice(){return m_pd3dDevice;}
	CItemRender*	GetItemRender()
	{
		return m_pItemRender;
	}

	LHUI_TakeBox * GetTakeOutBox()
	{
		return m_ui_takebox;
	}

	LHUI_Gate * GetGate()
	{
		return m_ui_gate;
	}

	CUIFriend * GetFriend()
	{
		return m_ui_friend;
	}

	CControlMenu		*GetControlMenu()
	{
		return m_pControlMenu;
	}
	CControlBottom		*GetControlBottom()
	{
		return m_pControlBottom;
	}
	CControlShop		*GetControlShop()
	{
		return m_pControlShop;
	}
	CControlPeiceShop	*GetControlPeiceShop()
	{
		return m_pControlPeiceShop;
	}
	CControlExchange	*GetControlExchange()
	{
		return m_pControlExchange;
	}
	CControlWareHouse	*GetControlWareHouse()
	{
		return m_pControlWareHouse;
	}
	CControlGuildSHouse *GetControlGStorehouse()
	{
		return m_pControlGStoreHouse;
	}
	CGuildSystemManager *GetGuildSystemManager()
	{
		return m_pGuildManager;
	}
	CControlGuild		*GetControlGuild()
	{
		return m_pControlGuild;
	}
	CControlGuildManage *GetControlGuildManage()
	{
		return m_pControlGuildManager;
	}
	CControlGuildBattle *GetControlGuildBattle()
	{
		return m_pControlGuildBattle;
	}
	CGuildRankWarBattleCount *GetGuildRankWarBattleCount()
	{
		return m_pGuildRankWarBattleCount;
	}
	CControlUserShop	*GetControlUserShop()
	{
		return m_pControlUserShop;    ///개인상점
	}
	CInterchange		*GetInterchange()
	{
		return m_pInterchange;
	}
	CProduct			*GetProduct()
	{
		return m_pProduct;
	}
	CPetProduct			*GetPetProduct()
	{
		return m_pPetProduct;
	}
	CControlLetter		*GetControlLetter()
	{
		return m_pControlLetter;
	}
	CControlInven		*GetControlInven()
	{
		return m_pControlInven;
	}
	CControlPetStatus	*GetControlPetStatus()
	{
		return m_pControlPetStatus;
	}
	CControlSkill		*GetControlSkill()
	{
		return m_pControlSkill;
	}
	CControlHelp		*GetControlHelp()
	{
		return m_pControlHelp;
	};
	CControlParty		*GetControlParty()
	{
		return m_pControlParty;
	}
	CGamble				*GetGamble()
	{
		return m_pGamble;
	}
	LHUI_OpEvent		*GetNewEvent()
	{
		return m_ui_op_event;
	}

	CLockSmith			*GetLockSmith()
	{
		return m_pLockSmith;
	}

	CRefine				*GetRefine()
	{
		return m_pRefine;
	}
	CControlUpgrade		*GetUpgrade()
	{
		return m_pUpgrade;    ///신방어구강화시스템
	}
	CControlNotice		*GetNotice()
	{
		return m_pControlNotice;    ///공지팝업창
	}
	CControlUShopSearch *GetUserShopSearch()
	{
		return m_pUserShopSearch;    ///개인상점검색
	}
	CControlCashShop	*GetControlCashShop()
	{
		return m_pControlCashShop;    ///유료상점
	}
	CCashShopIntro		*GetCashShopIntro()
	{
		return m_pCashShopIntro;    ///유료상점
	}
	CRShiftMenu			*GetRShiftMenu()
	{
		return m_pRShiftMenu;
	}
	CLShiftMenu			*GetLShiftMenu()
	{
		return m_pLShiftMenu;
	}
	CEventAncient		*GetEventAncient()
	{
		return m_pEventAncient;
	}
	Cplatinum			*GetPlatinum()
	{
		return m_pPlatinum;    //플래티늄
	}
	CSummerEvent		*GetSummerEvent()
	{
		return m_pSummerEvent;   // 여름 사냥
	}
	CGuildPK			*GetGuildPK()
	{
		return m_pGuildPK;
	}
	CAutoMouse			*GetAutoMouse()
	{
		return m_pAutoMouse;
	};
//	CQuestData			*GetQuestData() { return m_pQuestData; }
	CPetLevel			*GetPetLevel()
	{
		return m_pPet_Level;
	}

	CProcess			*GetProcess()
	{
		return m_pProcess;
	}
	CWorldCupStart		*GetWorldCup()
	{
		return m_pWorldCup;
	}
	CControlHelper		*GetControlHelper()
	{
		return m_pControlHelper;
	}
	CControlHelpPet		*GetControlHelpPet()
	{
		return m_pControlHelpPet;    ///펫도움말
	}
	CSpiritMode			*GetSpiritMode()
	{
		return m_pSpiritMode;
	}
	//==========================================================================
	CBaseInterface		*GetInterface(int nNum)
	{
		return m_pInterface[nNum];
	}
	//==========================================================================
	CControlStatus * GetControlStatus()
	{
		return m_pControlStatus;
	}

	CControlSelectMenu *GetControlSelMenu()
	{
		return m_pControlSelMenu;
	}
	BOOL IsControlSelMenuVisible()
	{
		return m_bShowControlSelMenu;
	}
	void ShowControlSelMenu(BOOL bShow );
	void ToggleControlSelMenu()
	{
		ShowControlSelMenu( !m_bShowControlSelMenu );
	}

	BOOL ControlSelmenuSetMetuList(int type, char * arg = NULL);
	void ControlSelmenuAddStart();
	void ControlSelmenuAddPush(int vnum);
	void ControlSelmenuAddEnd();

	CControlBossraidInfo *GetBossraidInfo()
	{
		return m_pBossraidInfo;
	}
	BOOL IsBossraidInfoVisible()
	{
		return m_bBossraidInfo;
	};
	void ShowBossraidInfoWindow(BOOL bShow);
	void ToggleBossraidInfoWindow()
	{
		ShowBossraidInfoWindow(!m_bBossraidInfo);
	}
	void SetBossRaidInfo(int BossZoneNum, int bossNum, BOOL KillFlag);
	void InitBossRaidKillCount();

	CControlHeroFinalRank *GetHeroFinalRank()
	{
		return m_pHeroFinalRank;
	}
	BOOL IsHeroFinalRankVisible()
	{
		return m_bHeroFinalRank;
	}
	void ShowHeroFinalRankWindow(BOOL bShow);
	void ToggleHeroFinalRankWindow()
	{
		ShowHeroFinalRankWindow(!m_bHeroFinalRank);
	}
	void SetHeroFinalRank(char* buf);



	CControlHeroPrivilege *GetHeroPrivilege()
	{
		return m_pHeroPrivilege;
	}
	BOOL IsHeroPrivilegeVisable()
	{
		return m_bHeroPrivilege;
	}
	void ShowHeroPrivilegeWindow( BOOL bShow );
	void ToggleHeroPrivilegeWindow()
	{
		ShowHeroPrivilegeWindow(!m_bHeroPrivilege);
	}
	void SetHeroPrivilege(int heroPrivilege);

	CControlHeroRankList *GetHeroRankList()
	{
		return m_pHeroRankList;
	}
	BOOL IsHeroRankListeVisable()
	{
		return m_bHeroRankList;
	}
	void ShowHeroRankListWindow( BOOL bShow, int race = -1);
	void ToggleHeroRankListWindow(int race = -1 )
	{
		ShowHeroRankListWindow(!m_bHeroRankList, race);
	}
	void SetHeroRankList(char* buf);

	CControlPremiumChaMove* GetPremmiumChaMove()
	{
		return m_pPremiumChaMove;
	}
	BOOL IsPremiumChaMove()
	{
		return m_bShowPremiumChaMove;
	};
	void ShowPremiumChaMove(BOOL bShow, int slot=-1);
	void TogglePremiumChaMoveWindow()
	{
		ShowPremiumChaMove(!m_bShowPremiumChaMove);
	}

	BOOL IsLevelMenu()
	{
		return m_bPetLevel;
	}
	CControlTop *GetControlTop()
	{
		return m_pControlTop;
	}
	BOOL GetbViewTop()
	{
		return m_bViewTop;
	}
	BOOL GetbRShift()
	{
		return m_bRShift;
	}
	int m_Slot_Num;  // 이동장치가 있던 슬롯번호
	// 현재의 모드
	void	SetGraphicMode(NKGRAPHIC_MODE mode)
	{
		m_GraphicMode = mode;
	};
	void	SetRenderingSize(POINT	&pt)
	{
		m_ptCliSize = pt;
	};
	void	SetFocus();
	// 리소스 로딩
	CSheet* GetSheet()
	{
		if (m_pStartSheet) return m_pStartSheet;
		else return m_pStartSheet;
	};
	BOOL	Load2DResources();
	BOOL	Load2DResourcesInGame();
	void	DeleteObjects();
	void	DeleteStartSheet();
	// Drawing
	BOOL UpdateFrame();



	void ShowUI(int ui_type, int sub_type=-1);
	void CloseUI(int ui_type);



	BOOL IsControlMenuVisible()
	{
		return m_bMenu;
	};
	void ShowControlMenuWindow( BOOL bShow, int Type = -1, const char *NpcName = NULL ); // 타입은 어떤 경우의 메뉴인가...(이녀석은 토글 함수가 필요없다.)

	BOOL IsAutoMouseVisible()
	{
		return m_bAutoMouse;
	};

	//--------------
	// 쪽지창
	BOOL IsLetterVisible()
	{
		return m_bLetter;
	};
	void ShowLetterWindow(BOOL bShow);
	void ToggleLetterWindow()
	{
		ShowLetterWindow(!m_bShop);
	}

	BOOL IsShopVisible()
	{
		return m_bShop;
	};
	void ShowShopWindow(BOOL bShow);
	void ToggleShopWindow()
	{
		ShowShopWindow(!m_bShop);
	}

	BOOL IsPeiceShopVisible()
	{
		return m_bPeiceShop;
	};
	void ShowPeiceShopWindow(BOOL bShow);
	void TogglePeiceShopWindow()
	{
		ShowPeiceShopWindow(!m_bPeiceShop);
	}

	BOOL IsExchangeVisible()
	{
		return m_bShowExchange;
	};
	void ShowExchangeWindow(BOOL bShow, BOOL bExch=FALSE);
	void ToggleExchangeWindow(BOOL bExch=FALSE)
	{
		ShowExchangeWindow(!m_bShowExchange, bExch);
	}

	BOOL IsInterchangeVisible()
	{
		return m_bInterchange;
	};
	void ShowInterchangeWindow(BOOL bShow, BOOL bInter=FALSE);
	void ToggleInterchangeWindow(BOOL bInter=FALSE)
	{
		ShowInterchangeWindow(!m_bInterchange, bInter);
	}

	BOOL IsInvenVisible()
	{
		return m_bInven;
	};
	void ShowInvenWindow(BOOL bShow);
	void ToggleInvenWindow()
	{
		ShowInvenWindow(!m_bInven);
	}	

	bool IsInterfaceVisible( int nNum )
	{
		return m_bInterface[ nNum ];
	}
	void ShowInterfaceWindow( bool bShow, int nNum, int nCurWorldNum = 0, int nSlot_num = -1 );
	void ToggleInterfaceWindow( int nNum )
	{
		ShowInterfaceWindow(!m_bInterface[nNum], nNum);
	}
	void SetVisibleInterface(bool bVisible, int nNum);

	void ShowGateWindow(bool show , int gate_value , bool direct=false);

	BOOL IsPetStatusVisible()
	{
		return m_bPetStatus;
	}
	void ShowPetStatusWindow(BOOL bShow);
	void TogglePetStatusWindow()
	{
		ShowPetStatusWindow(!m_bPetStatus);
	}

	BOOL IsSkillVisible()
	{
		return m_bSkill;
	}
	void ShowRoyaleWindow(BOOL bShow);
	void ShowSkillWindow(BOOL bShow);
	void ToggleSkillWindow()
	{
		ShowSkillWindow(!m_bSkill);
	}
	void ToggleRoyaleWindow()
	{
		ShowRoyaleWindow(!m_bRoyale);
	}
	BOOL IsHelpVisible()
	{
		return m_bHelp;
	}
	void ShowHelpWindow(BOOL bShow);
	void ToggleHelpWindow()
	{
		ShowHelpWindow(!m_bHelp);
	}

	BOOL IsPartyVisible()
	{
		return m_bParty;
	}
	void ShowPartyWindow(BOOL bShow);
	void TogglePartyWindow()
	{
		ShowPartyWindow(!m_bParty);
	}

	BOOL IsProductVisible()
	{
		return m_bProduct;
	};
	void ShowProductWindow(BOOL bShow, BOOL bExch = FALSE);
	void ToggleProductWindow(BOOL bAct = FALSE)
	{
		ShowProductWindow(!m_bProduct, bAct);
	}

	BOOL IsPetProductVisible()
	{
		return m_bPetProduct;
	};
	void ShowPetProductWindow(BOOL bShow, BOOL bExch = FALSE);
	void TogglePetProductWindow(BOOL bAct = FALSE)
	{
		ShowPetProductWindow(!m_bPetProduct, bAct);
	}

	BOOL IsGambleVisible()
	{
		return m_bShowGamble;
	}
	void ShowGambleWindow(BOOL bShow);
	void ToggleGambleWindow(int type);

	BOOL IsLockSmith()
	{
		return m_bLockSmith;
	}
	void ShowLockSmithWindow(BOOL bShow);
	void ToggleLockSmithWindow(void);

	BOOL IsEventAncientVisible()
	{
		return m_bShowEventAncient;
	}
	void ShowEventAncientWindow(BOOL bShow);
	void ToggleEventAncientWindow(int type);
	// 플래티늄 관련 함수
	BOOL IsPlatinumVisible()
	{
		return m_bShowPlatinum;
	}
	void ShowPlatinumWindow(BOOL bShow);
	void TogglePlatinumExchWindow(int type);
	// 여름 사냥 이벤트 관련 함수
	BOOL IsSummerEventVisible()
	{
		return m_bShowSummerEvent;
	}
	void ShowSummerEventWindow(BOOL bShow);
	void ToggleSummerEventWindow(int type);

	BOOL IsRefineVisible()
	{
		return m_bShowRefine;
	}
	void ShowRefineWindow(BOOL bShow, int type = 0);
	void ToggleRefineWindow(int type);
	///신방어구강화시스템
	BOOL IsUpgradeVisible()
	{
		return m_bShowUpgrade;
	}
	void ShowUpgradeWindow( BOOL bShow );
	void ToggleUpgradeWindow( int type, BOOL ForceClose = FALSE ); // 타입은 후에 가공을 고려.


	///공지팝업창
	BOOL IsControlNoticeVisible()
	{
		return m_bShowNotice;
	}
	void ShowControlNoticeWindow( BOOL bShow, int Type = 0, char *String = NULL ); // 이 녀석은 좀 특이해서 토글방식이 아님에 유의.

	BOOL IsWareHouseVisible()
	{
		return m_bShowWareHouse;
	};
	void SetShowWareHouseFlag(BOOL bShow);
	void ShowWareHouseWindow(BOOL bShow);
	void ToggleWareHouseWindow()
	{
		ShowWareHouseWindow(!m_bShowWareHouse);
	}

	BOOL IsGStoreHouseVisible()
	{
		return m_bShowGStoreHouse;
	}
	void SetShowGStoreHouseFlag(BOOL bShow);
	void ShowGStoreHouseWindow(BOOL bShow);
	void ToggleGStoreHouseWindow()
	{
		ShowGStoreHouseWindow(!m_bShowGStoreHouse);
	}

	BOOL IsControlGuildBattle()
	{
		return (BOOL)GetControlGuildBattle()->IsActive();
	}
	void ShowControlGuildBattle(BOOL bShow );
	void ToggleControlGuildBattle()
	{
		ShowControlGuildBattle( (BOOL)!GetControlGuildBattle()->IsActive() );
	}

	BOOL IsControlGuild()
	{
		return m_bShowControlGuild;
	}
	void SetShowControlGuild(BOOL bShow);
	void ShowControlGuild(BOOL bShow);
	void ToggleControlGuildWindow()
	{
		ShowControlGuild(!m_bShowControlGuild);
	}

	BOOL IsControlGuildManage()
	{
		return m_bShowControlGuildManage;
	}
	void SetShowControlGuildManage(BOOL bShow);
	void ShowControlGuildManage(BOOL bShow);
	void ToggleControlGuildManageWindow()
	{
		ShowControlGuildManage(!m_bShowControlGuildManage);
	}

	///개인상점
	BOOL IsUserShopVisible()
	{
		return m_bShowUserShop;
	}
	void ShowUserShopWindow( BOOL bShow );
	void ToggleUserShopWindow( int index, int type, int grade ); // type은 점포상점인지 개인상점인지, grade는 팩의 갯수다. ///개인상점

	BOOL IsUserShopSearchVisible()
	{
		return m_bShowUserShopSearch;    ///개인상점검색
	}
	void ShowUserShopSearchWindow( BOOL bShow );
	void ToggleUserShopSearchWindow( BOOL ForceClose );

	///유료상점
	BOOL IsCashShopVisible()
	{
		return m_bShowCashShop;
	}
	void ShowCashShopWindow( BOOL bShow );
	void ToggleCashShopWindow( int type ); // type으로 다양한 상점에서 사용할수있도록....

	///유료상점
	BOOL IsCashShopIntroVisible()
	{
		return m_bShowCashShopIntro;
	}
	void ShowCashShopIntroWindow( BOOL bShow );
	void ToggleCashShopIntroWindow( int type ); // type으로 다양한 상점에서 사용할수있도록....

	//mungmae 2006/01/23 휴먼 가공
	BOOL IsProcessVisible()
	{
		return m_bShowProcess;
	}
	void ShowProcessWindow(BOOL bShow, int type = 0);
	void ToggleProcessWindow(int type);

	//후견인
	BOOL IsHelperVisible()
	{
		return m_bHelper;
	}
	void ShowHelperWindow(BOOL bShow);
	void ToggleHelperWindow()
	{
		ShowHelperWindow(!m_bHelper);
	}

	///펫도움말
	BOOL IsHelpPetVisible()
	{
		return m_bHelpPet;
	}
	void ShowHelpPetWindow( BOOL bShow, int Type );
	void ToggleHelpePetWindow( int Type )
	{
		ShowHelpPetWindow(!m_bHelpPet,Type);
	}

	BOOL IsFriendVisible();
	void ShowFriend(BOOL bShow);
	void ToggleFriend();

	ControlDecardiBattle* GetDecardiBattle()
	{
		return m_ui_decardibattle;
	}

	void SetEventType(int nType, int flag); ///이벤트버프표시

	// 오토마우스 사용제한 고나련
	void ShowAutoMouseWindow(BOOL bShow,int Type = 0); ///QnA
	//GuildPK관련
	void GuildPK(int sub_type, char *GuildName1, char *GuildName2, int PKstate);

	BOOL IsRShiftVisible()
	{
		return m_bRShift;
	};
	void SetShowRShiftFlag(BOOL bShow)
	{
		m_bRShift = bShow;
	};

	BOOL IsLShiftVisible()
	{
		return m_bLShift;
	};
	void SetShowLShiftFlag(BOOL bShow)
	{
		m_bLShift = bShow;
	};

	void SetShowPetLevelFlag(BOOL bShow)
	{
		m_bPetLevel = bShow;
	};

	BOOL IsPVPVisible()
	{
		return m_bShowPVPWidget;
	}
	void ShowPVPWidget( BOOL bShow );
	CPVPWidget* GetPVPWidget()
	{
		return m_pPVPWidget;
	}

	// 드미트론 점령전 - 점령 현황
	BOOL IsVisibleDmitronBattleStatus()
	{
		return m_bShowDmitronBattleStatus;
	}
	void ShowDmitronBattleStatus( BOOL bShow );
	CDmitronBattleStatus* GetDmitronBattleStatus()
	{
		return m_pDmitronBattleStatus;
	}

	BOOL IsVisiblePremiumExchange()
	{
		return m_bShowPremiumExchange;
	}
	void ShowPremiumExchange( int nType, int nSlot );
	void HidePremiumExchange();
	CPremiumExchange* GetPremiumExchange()
	{
		return m_pPremiumExchange;
	}


	BOOL IsVisibleTakebox();
	bool ShowTakeboxWindow( BOOL bShow , int type);

	BOOL CloseAllWindow( BOOL bAdjustVP = TRUE, bool bTap = false );
	void UpdateDrawInfo();

	int m_PartyReadyIndex;

	void ToggleTopWindow()
	{
		m_bViewTop = !m_bViewTop;
	}
	void SetVisibleTopWindow( BOOL bVisible = true )
	{
		m_bViewTop = bVisible;
	}

	char m_strRShiftName[40];
	void ShowRShiftMenu(CNkCharacter *pCha);
	void ShowLShiftMenu();

	void ShowPetLevelMenu();

	// CControlBottom에 대한 명령
	void AddChatStr(char *str, int say_type = 0);
	void AddChatStr(int index, int say_type = 0);
	void AddChatStr_ExtraInfo(char* str, int say_type = 0, int infoType = 0, int infoVnum = 0);
	void AddChatShoutStr(char* str);

	// CControlInven에 대한 명령
	void SetNkCha(CNkCharacter *pNkCha);

	// Info Box에 글을 쓴다.
	void SetInfoBox(char *msg, bool removeInfo2 = true);
	void SetInfoBox2(char *msg, COLORREF fontColor = RGB(0, 200, 0), COLORREF outlineColor = RGB(220, 100, 135));
	void SetInfoBox_NextString(const std::string& strMsg);
	void SetInfoBox_NextString2(const std::string& strMsg);

	// 윈도우 프로시저
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL bDblClk);

	// 1분에 한번씩 시간을 보낸다.
	void SendTickTime();

	// 보이스채팅을 지운다.
	void DeleteVoiceChat();

	// 게임내에서 사용하는 모든 메세제 창을 검색한다.
	BOOL Check_All_MsgPopUp();

	// 메세지 창이 하나라도 떠있다면...죽인다
	BOOL All_Kill_MsgPopUp();

	// 겜블 관련
	CNkCharacter *m_pNkCha;
	POPUP	*m_Popup;	// 팝업창
	void DeleteAllPopup();	// 팝업창 모두 닫기
	void InitDmitronInfo(); // 드미트론 점령전 - 점령 현황
	void DisplayDmitronInfo(); // 드미트론 점령전 - 점령 현황
	void SetDmitronInfo1(int index, int status); // 드미트론 점령전 - 점령 현황
	void SetDmitronInfo2(int index, int status, char* myGuildName, int myGuildPoint,
						 char* topGuildName, int topGuildPoint, int lastTime, int elapsedTime); // 드미트론 점령전 - 점령 현황
	void InsertPopup( char* arg, int nType, int nProcess = 1, int nLinit = 30, int nDocument = 0, void* pArgs = 0); // 팝업창 추가
	void DeletePopup();	// 팝업창 닫기
	CMsgPopUp* FindPopup( int nProcess );
	HRESULT RestorePopups();
	LRESULT PopupMsgProc(HWND, UINT, WPARAM, LPARAM);
	bool IsPopUpTyping(); // 팝업 입력을 받고 있는지의 여부 체크함수.
	void WareHouseStrCopy( char *str ); //  [1/15/2008 반재승] : 일본 버그 요청사항-기간제 아이템일때 띄우는 메세지

	int		m_iQuicSlotNum;
	int		m_nDocument;

	BOOL	m_bWorldCup;

	BOOL CheckFullScreenInterface(); // 화면을 독점적으로 사용하는 인터페이스가 켜져있는지 체크하는 함수. ///유료상점시 추가
	BOOL IsItemMultiMoveEnable(); // 인벤 오른쪽 더블클릭시 다중이동 기능을 사용하는 창이 떠있나 체크해주는 함수. 물약 인벤에서 사용시 등에서 체크한번 해주도록 추가(07-02-20 원석)

	BOOL    m_bSpiritStart;


	CWindowMgr*	m_pUIWindow;
	CWINDOWUI*	m_pWindowRes;

	CWindowMgr* GetUIWindow() const
	{
		return m_pUIWindow;
	}
	CWINDOWUI*  GetWindowRes() const
	{
		return m_pWindowRes;
	}

	CWindowTgaMgr* m_pTgaUIWindow;
	CWindowTgaMgr* GetTgaUIWindow() const
	{
		return m_pTgaUIWindow;
	}

	bool IsInPopup(int x, int y);

	DWORD m_dwTakeBox_Time;


	bool CreateLHUI(int ui, int param);

protected:
	BOOL	m_bProcLock;					// 캐릭터 이동 중에는 아무것도 하지 못하도록 프로세스를 막아 버린다.
public:
	BOOL	IsProcLock()
	{
		return m_bProcLock;
	}
	void	SetProcLock( BOOL bProcLock)
	{
		m_bProcLock = bProcLock;
	}
	BOOL	CheckPremiumChaMoveInfo();		//  이 함수로 클라에서 채크해야 할 모든걸 체크 하자
	BOOL	CheckPremiumChamoveScroll();	// 지정된 아이템만 남아 있는지 체크 모든 인벤 체크
	BOOL	CheckPremiumLetter();			// 편지를 검사한다.
	BOOL	OnClosePremiumChaMove();		// 캐릭터 이동과 과련된 팝업을 닫아 준다.
	BOOL	ClossePremiumMsgBox();
	void	PremiumChaMoveWear(int where, int index, int vnum, int gem_num, int special, int special2,
							   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur);

	void SetMyMoney(__int64 money);
		
#define LH_MAX_LEVEL 1000
	bool	m_exp_init;
	UINT64	m_exp_require[LH_MAX_LEVEL];
	UINT64	m_exp_accumulate[LH_MAX_LEVEL];

	void InitExpTable();
	UINT64 GetExpAccumulate();
	UINT64 GetExpRequire();
	UINT64 GetExpRemain();
	UINT64 GetExpGet();


	int GetRSkill();
	int GetLSkill();
	void SetRSkill(int index);
	void SetLSkill(int index);

	bool IsScroll(CItem* pItem);

	void RenderItemInUI(int x, int y, CItem* pItem, BOOL over, BOOL bMove,BOOL ignore_size, BOOL begin_end=TRUE, float fScale = 1.0f);

	void RenderItemInUIWithSize(int vnum, int x, int y, int sizeX, int sizeY);




	bool m_force_exit;
	DWORD m_force_exit_tick;
	void SetForceExit()
	{
		m_force_exit = true;
		m_force_exit_tick = 0;
	}



public:
	void SetLoadingNow(bool bLoadingNow)	{ m_bLoadingNow = bLoadingNow; }
	const bool& GetLoadingNow()				{ return m_bLoadingNow; }
	static unsigned __stdcall ThreadLoading( void* pParam );

	void SetLoadingGoworldNow(bool bValue)	{ m_bLoadingGoWorldNow = bValue; }
	const bool& GetLoadingGoWorldNow()		{ return m_bLoadingGoWorldNow; }
	static unsigned __stdcall ThreadGoWorldLoading( void* pParam );

private:
	bool m_bLoadingNow;
	bool m_bLoadingGoWorldNow;
};

extern CNk2DFrame * g_pNk2DFrame;

#endif // _NK2D_FRAME_H_
