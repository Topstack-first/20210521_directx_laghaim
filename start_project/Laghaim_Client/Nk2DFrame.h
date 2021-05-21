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
class CControlUpgrade;	///�Ź���ȭ�ý���
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
class CControlUserShop; ///���λ���
class CControlUShopSearch; ///���λ����˻�
class CProcess;	//����
class CWorldCupStart;
class CControlCashShop; ///�������
class CCashShopIntro; ///�������.
class CControlHelper; //�İ���
class CControlHelpPet; ///�굵��
class CSpiritMode;
class CAutoNotice;					// ����� ��û : 10�� ���� ���� �޽��� ���
class CControlStatus;


class CControlGuildSHouse;
class CGuildSystemManager;
class CControlGuild;
class CControlGuildManage;
class CWindowMgr;
class CWINDOWUI;
class CControlGuildBattle;
class CGuildRankWarBattleCount;

class CControlNotice; ///�����˾�â

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
#define CONTROLMAIN_Y_MARGIN	42				// ���� �޴������� ����κ�
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

// 2D �������� 3D�� �������Ѵ�.   // ����..
class CNk2DFrame
{
protected:
	CSheet					*m_pStartSheet;
	// ĳ���Ϳ� ��� ��.
	CCapsyong				**m_dpChaCapsyong;
	NKGRAPHIC_MODE			m_GraphicMode;
	POINT					m_ptCliSize;

	CControlMenu			*m_pControlMenu; // �������̽� ���� �޴�.
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
	CControlPetStatus		*m_pControlPetStatus; // �ֿϵ��� ����â
	CPetLevel				*m_pPet_Level;
	CGamble					*m_pGamble;

	CLockSmith				*m_pLockSmith;

	CEventAncient			*m_pEventAncient;
	Cplatinum               *m_pPlatinum;
	CSummerEvent            *m_pSummerEvent;
//	CQuestData				*m_pQuestData;
	CRefine					*m_pRefine;
	CControlUpgrade			*m_pUpgrade;		// ��ȭ �������̽�    ///�Ź���ȭ�ý���
	CControlNotice          *m_pControlNotice; ///�����˾�â
	CControlUShopSearch     *m_pUserShopSearch; ///���λ����˻�
	CGuildPK				*m_pGuildPK;		// ����� ����
	CAutoMouse				*m_pAutoMouse;		// AutoMouse(���)���� ����
	CControlWareHouse       *m_pControlWareHouse;
	CControlGuildSHouse		*m_pControlGStoreHouse;
	CGuildSystemManager		*m_pGuildManager;
	CControlGuild			*m_pControlGuild;
	CControlRoyale			*m_pControlRoyale;
	CControlGuildManage		*m_pControlGuildManager;	// ��� ���� ��ư Ŭ����
	CControlGuildBattle		*m_pControlGuildBattle;
	CGuildRankWarBattleCount * m_pGuildRankWarBattleCount;
	CControlUserShop        *m_pControlUserShop; ///���λ���
	CControlCashShop        *m_pControlCashShop; ///�������
	CCashShopIntro          *m_pCashShopIntro; ///�������
	CBaseInterface			**m_pInterface;
	bool					*m_bInterface;
	CProcess				*m_pProcess;
	CWorldCupStart			*m_pWorldCup;
	CControlHelper			*m_pControlHelper;	//�İ���
	CControlHelpPet			*m_pControlHelpPet;	///�굵��
	CSpiritMode				*m_pSpiritMode;     //mungmae-2006/08/30 ���Ǹ� ��� ����

	CAutoNotice				*m_pAutoNotice;		// ����� ��û : 10�� ���� ���� �޽��� ���

	CControlStatus			*m_pControlStatus;

	CControlBossraidInfo	*m_pBossraidInfo;

	CControlHeroFinalRank	*m_pHeroFinalRank;

	CControlHeroPrivilege	*m_pHeroPrivilege;
	CControlHeroRankList	*m_pHeroRankList;

	CPVPWidget				*m_pPVPWidget;	// 1:1 ���� ���

	CDmitronBattleStatus	*m_pDmitronBattleStatus; // ���Ʈ�� ������ - ���� ��Ȳ

	CPremiumExchange*		m_pPremiumExchange;

	CControlPremiumChaMove*	m_pPremiumChaMove;

	CControlSelectMenu *	m_pControlSelMenu;

	LHUI_OpEvent *			m_ui_op_event;

	LHUI_TakeBox *			m_ui_takebox;

	LHUI_Gate * m_ui_gate;

	CUIFriend * m_ui_friend;

	UI_BuffItem * m_ui_buffitem;

	ControlDecardiBattle * m_ui_decardibattle;

	// Item â...
	CItemRender				*m_pItemRender;
	// â...
	BOOL					m_bMenu; // �������̽� ���� �޴�.(�� ���������� ���ϼ��ִ� �޴��̴�.)
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
	BOOL					m_bShowUpgrade;  ///�Ź���ȭ�ý���
	BOOL					m_bShowNotice;  ///�����˾�â
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
	BOOL					m_bShowUserShop; ///���λ���
	BOOL					m_bShowUserShopSearch; ///���λ����˻�
	BOOL					m_bShowCashShop; ///�������
	BOOL					m_bShowCashShopIntro; ///�������
	BOOL					m_bZone;
	BOOL					m_bPetLevel;
	BOOL					m_bShowEventAncient;
	BOOL                    m_bShowPlatinum;	// �÷�Ƽ�� ���� ����
	BOOL                    m_bShowSummerEvent;	// ���� ��� �̺�Ʈ ���� ����
	BOOL					m_bAllLoaded;		// �޽��� ħ���� ���� ���� ���ҽ� ��� ����
//	BOOL					m_bQuest;
	BOOL					m_bShowProcess;
	BOOL					m_bHelper;			//�İ���
	BOOL					m_bHelpPet;			///�굵��
	BOOL					m_bStatus;
	BOOL					m_bBossraidInfo;

	BOOL					m_bHeroFinalRank;

	BOOL					m_bHeroPrivilege;
	BOOL					m_bHeroRankList;

	BOOL					m_bShowPVPWidget;

	BOOL					m_bShowDmitronBattleStatus;  // ���Ʈ�� ������ - ���� ��Ȳ

	BOOL					m_bShowPremiumExchange;

	BOOL					m_bShowPremiumChaMove;

	BOOL					m_bShowControlSelMenu;

	CTextOutBox				*m_pInfoBox;		// �������� �� ������ �����ִ� â
	DWORD					m_InfoTime;
	int						m_InfoFrame;
	CTextOutBox				*m_pInfoBox2;
	DWORD					m_InfoTime2;
	int						m_InfoFrame2;
	std::string				m_InfoNextString;
	std::string				m_InfoNextString2;

	// [3/5/2007 Theodoric] �������� ������ ����
	char	*m_strConnectServerName;
	char    m_strCheckWareHouse[1024]; //  [1/15/2008 �����] : �Ϻ� ���� ��û����-�Ⱓ�� �������϶� ���� �޼���
public:
	CNk2DFrame();
	virtual ~CNk2DFrame();

	HRESULT RestoreSurfaces();

	// [3/5/2007 Theodoric] �������� ������ ����
	char*	GetConnectServerName()
	{
		return m_strConnectServerName;
	}
	void	SetConnectServerName(char *str);

	// �ʱ�ȭ �� ���°���
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
		return m_pControlUserShop;    ///���λ���
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
		return m_pUpgrade;    ///�Ź���ȭ�ý���
	}
	CControlNotice		*GetNotice()
	{
		return m_pControlNotice;    ///�����˾�â
	}
	CControlUShopSearch *GetUserShopSearch()
	{
		return m_pUserShopSearch;    ///���λ����˻�
	}
	CControlCashShop	*GetControlCashShop()
	{
		return m_pControlCashShop;    ///�������
	}
	CCashShopIntro		*GetCashShopIntro()
	{
		return m_pCashShopIntro;    ///�������
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
		return m_pPlatinum;    //�÷�Ƽ��
	}
	CSummerEvent		*GetSummerEvent()
	{
		return m_pSummerEvent;   // ���� ���
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
		return m_pControlHelpPet;    ///�굵��
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
	int m_Slot_Num;  // �̵���ġ�� �ִ� ���Թ�ȣ
	// ������ ���
	void	SetGraphicMode(NKGRAPHIC_MODE mode)
	{
		m_GraphicMode = mode;
	};
	void	SetRenderingSize(POINT	&pt)
	{
		m_ptCliSize = pt;
	};
	void	SetFocus();
	// ���ҽ� �ε�
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
	void ShowControlMenuWindow( BOOL bShow, int Type = -1, const char *NpcName = NULL ); // Ÿ���� � ����� �޴��ΰ�...(�̳༮�� ��� �Լ��� �ʿ����.)

	BOOL IsAutoMouseVisible()
	{
		return m_bAutoMouse;
	};

	//--------------
	// ����â
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
	// �÷�Ƽ�� ���� �Լ�
	BOOL IsPlatinumVisible()
	{
		return m_bShowPlatinum;
	}
	void ShowPlatinumWindow(BOOL bShow);
	void TogglePlatinumExchWindow(int type);
	// ���� ��� �̺�Ʈ ���� �Լ�
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
	///�Ź���ȭ�ý���
	BOOL IsUpgradeVisible()
	{
		return m_bShowUpgrade;
	}
	void ShowUpgradeWindow( BOOL bShow );
	void ToggleUpgradeWindow( int type, BOOL ForceClose = FALSE ); // Ÿ���� �Ŀ� ������ ���.


	///�����˾�â
	BOOL IsControlNoticeVisible()
	{
		return m_bShowNotice;
	}
	void ShowControlNoticeWindow( BOOL bShow, int Type = 0, char *String = NULL ); // �� �༮�� �� Ư���ؼ� ��۹���� �ƴԿ� ����.

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

	///���λ���
	BOOL IsUserShopVisible()
	{
		return m_bShowUserShop;
	}
	void ShowUserShopWindow( BOOL bShow );
	void ToggleUserShopWindow( int index, int type, int grade ); // type�� ������������ ���λ�������, grade�� ���� ������. ///���λ���

	BOOL IsUserShopSearchVisible()
	{
		return m_bShowUserShopSearch;    ///���λ����˻�
	}
	void ShowUserShopSearchWindow( BOOL bShow );
	void ToggleUserShopSearchWindow( BOOL ForceClose );

	///�������
	BOOL IsCashShopVisible()
	{
		return m_bShowCashShop;
	}
	void ShowCashShopWindow( BOOL bShow );
	void ToggleCashShopWindow( int type ); // type���� �پ��� �������� ����Ҽ��ֵ���....

	///�������
	BOOL IsCashShopIntroVisible()
	{
		return m_bShowCashShopIntro;
	}
	void ShowCashShopIntroWindow( BOOL bShow );
	void ToggleCashShopIntroWindow( int type ); // type���� �پ��� �������� ����Ҽ��ֵ���....

	//mungmae 2006/01/23 �޸� ����
	BOOL IsProcessVisible()
	{
		return m_bShowProcess;
	}
	void ShowProcessWindow(BOOL bShow, int type = 0);
	void ToggleProcessWindow(int type);

	//�İ���
	BOOL IsHelperVisible()
	{
		return m_bHelper;
	}
	void ShowHelperWindow(BOOL bShow);
	void ToggleHelperWindow()
	{
		ShowHelperWindow(!m_bHelper);
	}

	///�굵��
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

	void SetEventType(int nType, int flag); ///�̺�Ʈ����ǥ��

	// ���丶�콺 ������� ����
	void ShowAutoMouseWindow(BOOL bShow,int Type = 0); ///QnA
	//GuildPK����
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

	// ���Ʈ�� ������ - ���� ��Ȳ
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

	// CControlBottom�� ���� ���
	void AddChatStr(char *str, int say_type = 0);
	void AddChatStr(int index, int say_type = 0);
	void AddChatStr_ExtraInfo(char* str, int say_type = 0, int infoType = 0, int infoVnum = 0);
	void AddChatShoutStr(char* str);

	// CControlInven�� ���� ���
	void SetNkCha(CNkCharacter *pNkCha);

	// Info Box�� ���� ����.
	void SetInfoBox(char *msg, bool removeInfo2 = true);
	void SetInfoBox2(char *msg, COLORREF fontColor = RGB(0, 200, 0), COLORREF outlineColor = RGB(220, 100, 135));
	void SetInfoBox_NextString(const std::string& strMsg);
	void SetInfoBox_NextString2(const std::string& strMsg);

	// ������ ���ν���
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL bDblClk);

	// 1�п� �ѹ��� �ð��� ������.
	void SendTickTime();

	// ���̽�ä���� �����.
	void DeleteVoiceChat();

	// ���ӳ����� ����ϴ� ��� �޼��� â�� �˻��Ѵ�.
	BOOL Check_All_MsgPopUp();

	// �޼��� â�� �ϳ��� ���ִٸ�...���δ�
	BOOL All_Kill_MsgPopUp();

	// �׺� ����
	CNkCharacter *m_pNkCha;
	POPUP	*m_Popup;	// �˾�â
	void DeleteAllPopup();	// �˾�â ��� �ݱ�
	void InitDmitronInfo(); // ���Ʈ�� ������ - ���� ��Ȳ
	void DisplayDmitronInfo(); // ���Ʈ�� ������ - ���� ��Ȳ
	void SetDmitronInfo1(int index, int status); // ���Ʈ�� ������ - ���� ��Ȳ
	void SetDmitronInfo2(int index, int status, char* myGuildName, int myGuildPoint,
						 char* topGuildName, int topGuildPoint, int lastTime, int elapsedTime); // ���Ʈ�� ������ - ���� ��Ȳ
	void InsertPopup( char* arg, int nType, int nProcess = 1, int nLinit = 30, int nDocument = 0, void* pArgs = 0); // �˾�â �߰�
	void DeletePopup();	// �˾�â �ݱ�
	CMsgPopUp* FindPopup( int nProcess );
	HRESULT RestorePopups();
	LRESULT PopupMsgProc(HWND, UINT, WPARAM, LPARAM);
	bool IsPopUpTyping(); // �˾� �Է��� �ް� �ִ����� ���� üũ�Լ�.
	void WareHouseStrCopy( char *str ); //  [1/15/2008 �����] : �Ϻ� ���� ��û����-�Ⱓ�� �������϶� ���� �޼���

	int		m_iQuicSlotNum;
	int		m_nDocument;

	BOOL	m_bWorldCup;

	BOOL CheckFullScreenInterface(); // ȭ���� ���������� ����ϴ� �������̽��� �����ִ��� üũ�ϴ� �Լ�. ///��������� �߰�
	BOOL IsItemMultiMoveEnable(); // �κ� ������ ����Ŭ���� �����̵� ����� ����ϴ� â�� ���ֳ� üũ���ִ� �Լ�. ���� �κ����� ���� ��� üũ�ѹ� ���ֵ��� �߰�(07-02-20 ����)

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
	BOOL	m_bProcLock;					// ĳ���� �̵� �߿��� �ƹ��͵� ���� ���ϵ��� ���μ����� ���� ������.
public:
	BOOL	IsProcLock()
	{
		return m_bProcLock;
	}
	void	SetProcLock( BOOL bProcLock)
	{
		m_bProcLock = bProcLock;
	}
	BOOL	CheckPremiumChaMoveInfo();		//  �� �Լ��� Ŭ�󿡼� äũ�ؾ� �� ���� üũ ����
	BOOL	CheckPremiumChamoveScroll();	// ������ �����۸� ���� �ִ��� üũ ��� �κ� üũ
	BOOL	CheckPremiumLetter();			// ������ �˻��Ѵ�.
	BOOL	OnClosePremiumChaMove();		// ĳ���� �̵��� ���õ� �˾��� �ݾ� �ش�.
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
