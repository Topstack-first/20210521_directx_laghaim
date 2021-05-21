#ifndef _UI_MGR_H_
#define _UI_MGR_H_

#include "Config.h"
#include "LHAutoPlay_Define.h"

#include <list>
#include <stack>
#include <iterator>
#include <queue>
using namespace std;

#include "TextOutBox.h"
#include "Item.h"
#include "FxSet.h"
#include "Process.h"
#include "EventLegKnockAppear.h" // ��ī�� ���� ����� �̺�Ʈ. ///�δ�1 �� �߰�

class CLand;
class CTcpIpConnection;
class CNkCharacter;
class CNkMob;
class CUserShopNpc; ///��������
class CNkPet;
class CItem;
class CFrustum;
class CTextOutBox;
class CNkKeyboard;
class CDecoObj;
class CCmd;
class CMsgPopUp;
class CSurface;
class CScrollBox;
class CWarInfo;
class CChangeHair;
class CWarTax;
class CGuardTower;
class CWarRequest;
class CControlPetShop2;
class CControlPetKeep;
class CGuildWar;
class CMatList;
class CMatMakeRoom;
class CMatBattleUI;
class CMatSurvivalUI;
class CControlTeleportItem;
class CControlLowGuildMaster;
//=====================================================================================
// nate 2004 - 9
// GuildHunt
class CGuildHunt;
class CReservationWindow;
//=====================================================================================
// nate 2005-03-21
// Matrix
class CRectPlane;
class CEventScriptActor; ///Ʃ�丮��
class CAutoPlay;
class CMatrixWarMessage;
class UI_GmMacro;

// �ٸ��콺 ó���� ���ؼ�.
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#define PLAYERNAME_FONTSIZE				12
#define PLAYERNAME_FONTCOLOR			RGB(255, 255, 0)
#define NAME_ACTIVECOLOR				RGB(0, 255, 255)
#define MUSIC_NUM						1
// ���� ����.
#define ATTACK_RANGE					(15.0f)
#define TOWER_RANGE						(20.0f)
#define HELL_ATTACK_RANGE				(60.0f)
#define GROUND_RANGE					(70.0f)
#define GP_RANGE						(50.0f)
#define GATE_RANGE						(60.0f)
#define CHAKRAM_ATTACK_RANGE			(130.0f)
// UI ���� ����

#define VITAL_BACK_WIDTH_C				128
#define VITAL_BACK_HEIGHT_C				9
#define VITAL_FRONT_WIDTH_C				114
#define VITAL_FRONT_HEIGHT_C			7

#define VITAL_BACK_WIDTH				75
#define VITAL_BACK_HEIGHT				5
#define VITAL_FRONT_WIDTH				70
#define VITAL_FRONT_HEIGHT				3


#define VITAL_BACK_Y					10 // [3/30/2007 Theodoric] ����� �� ǥ�� :: ��ġ ����  
#define VITAL_FRONT_Y					11 // [3/30/2007 Theodoric] ����� �� ǥ�� :: ��ġ ����

#define MAX_PRESENT_BOX					2
#define MAX_ANGEL_CNT					14
#define  _VERTEX_CNT					4

#define _DELIV_OPEN						0
#define _DELIV_SUCCEED					1
#define _DELIV_FAIL						2

#define MAX_LETTER_MSG_BOX				6
#define TIME_LETTER_FRAME				80
#define BUTTON_LEFT						0
#define BUTTON_RIGHT					1
#define BUTTON_WHEEL					2
//////////////////////////////////////////////////////////////////////////
// ����ü.
struct ACTION
{
	int nActionKind;
	char strCmd[128];
} ;

class CItemInfo
{
public:
	CItem m_Item;
	CTextOutBox m_TextOutBox;
	CTextOutBox m_TextOutBox_Active;
	BOOL		m_bActive;
};

typedef struct _node
{
	char    *str;
	struct _node* prev;
	struct _node* next;
} dnode;

#include "InterFileConnection.h"
class CGuildMark;
class CHttpConnection;
typedef list<CGuildMark*> list_guildmark;
typedef list<unsigned int> list_guildtemp;

#include <map>

typedef struct _LHEntityInfo
{
	int		index;
	int		entity_type;

	_LHEntityInfo()
	{
		index = -1;
		entity_type = -1;
	}
	_LHEntityInfo(int _index, int _entity_type)
	{
		index = _index;
		entity_type = _entity_type;
	}
} LHEntityInfo , *PLHEntityInfo;

typedef map<int, CNkCharacter*>			LHCharacterMap;
typedef LHCharacterMap::iterator		LHCharacterMap_it;
typedef LHCharacterMap::value_type		LHCharacterMap_vt;

typedef map<int, CNkMob*>				LHMobMap;
typedef LHMobMap::iterator				LHMobMap_it;
typedef LHMobMap::value_type			LHMobMap_vt;

typedef map<int, CUserShopNpc*>			LHUserShopNpcMap;
typedef LHUserShopNpcMap::iterator		LHUserShopNpcMap_it;
typedef LHUserShopNpcMap::value_type	LHUserShopNpcMap_vt;

typedef map<int, CNkPet*>				LHPetMap;
typedef LHPetMap::iterator				LHPetMap_it;
typedef LHPetMap::value_type			LHPetMap_vt;

typedef map<int, CItemInfo*>			LHItemInfoMap;
typedef LHItemInfoMap::iterator			LHItemInfoMap_it;
typedef LHItemInfoMap::value_type		LHItemInfoMap_vt;

typedef map<WORD, LHEntityInfo*>		LHColorKeyMap;
typedef LHColorKeyMap::iterator			LHColorKeyMap_it;
typedef LHColorKeyMap::value_type		LHColorKeyMap_vt;

typedef map<int, BOOL>					LHMobVisibleMap;

class CUIMgr
{
public:
	enum Use_QuickItem_Type
	{
		Use_QuickItem__NotDefine = -1 ,
		Use_QuickItem__GemStone,
		Use_QuickItem__Material,
		Use_QuickItem__Recovery,
		Use_QuickItem__Gem,
		Use_QuickItem__Present,
		Use_QuickItem__ForQuick,
		Use_QuickItem__Etc,
		Use_QuickItem__Chip,
		Use_QuickItem__MagicStone,

		Use_QuickItem__EndOfEnum
	};

	enum Move_To_Item_Type
	{
		Move_To_Item__NotDefine = -1,
		Move_To_Item__Quick,
		Move_To_Item__Apple,

		Move_To_Item_EndOfEnum
	};


private:
	HWND							m_hWnd;
protected:
	CFrustum			*			m_pFrustum;
	CDecoObj			*			m_pOpenDeco;
	CNkKeyboard			*			m_pNkKeyboard;
	int								m_DeviceId;

	char							m_commIn[2048], m_commInTmp[1024], m_commOut[512], m_oneSentence[512];
	WORD							m_wColor;				// �ȹ��ۿ� ���̴� Į�� ����.

	CTextOutBox						m_NameBox;				// ��ü�� �̸��� �����ֱ� ���� Ŭ����.
	BOOL							m_bShowName;			// ��ü�� �̸�.
	int								m_nShowNameType;		// �÷��̾��ΰ� �����ΰ� NPC�ΰ�, �������ΰ�. 0, 1, 2, 3
	int								m_ShowVitalId;
	WORD							m_wPrevName;			// ������ ���õ� Į���.
	int								m_Vital;

	BOOL							m_bAdjustingCamera;		// ī�޶� ����ġ ��Ű��.
	CCmd				*			m_pCmd;					// ��ɾ� ó��

	BOOL							m_bPocketPapa_Click;	// [5/7/2008 �����] �±� ���� ���� �̺�Ʈ �Ҿƹ��� Ŭ�� üũ
	float							m_papaClickTime;
	std::string						m_strGuildMarkURL;
public:
	// ���� UI�� surface
	CSurface			*			m_pVitalBack;
	CSurface			*			m_pVitalBack_E;
	CSurface			*			m_pPetVitalFront;
	CSurface			*			m_pVitalFront;
	CSurface			*			m_pManaFront;
	CSurface			*			m_pStamFront;
	CSurface			*			m_pEpowerFront;

	//  [8/13/2009 ppmmjj83] ������ npc ������ �̹��� ����
	CSurface			*			m_pVitalBack_Center;
	CSurface			*			m_pVitalFront_Center;

	bool							m_bMouseDown;
	DWORD							m_dwElapsedTime;
	BOOL							m_bRMouseDown;
	DWORD							m_dwRbuttonElapsedTime;
	// ���� ������ ���� UI
	BOOL							m_bNextAction;
	ACTION							m_Action;
	BOOL							m_bPickLock;			// pick �� ���� ���ÿ� �κ� ������ ������ �κ� ������ ������� ���� ������ �÷���. ///pick_bug
	BOOL							m_bMsg;
	
	CLand				*			m_pLand;
	int								m_nChangeWorldIdx;		// ���� �̵�.
	int								m_nChangeLocalIdx;		// ���� �̵�.
	int								m_nCurFloor;			// ����Ʈ �������� �̵�
	// �ŷ����� ��� ����.
	BOOL							m_bExch;
	char							m_strExchPlayerName[30];
	int								m_nExchCharId;			// nate 2005-06-07 : ��ȯ�� �ĺ�

	char							m_strMsPlayerName[30];	// ���� ���� ��� ����
	BOOL							m_bModal;
	// "ALT" Ű ��� ����
	BOOL							m_bAlt;
	int								m_cal_item_x;
	int								m_cal_item_y;
	// ���콺 Ŭ�� ���� // Mouse click limit
	DWORD							m_lastMoveTime;			// Ŭ���ؼ� �����̴� �� 0.5 �ʸ��� �ѹ����� ����...
	DWORD							m_dwUseQuickItemTime;	// nate 2005-08-23 : ���� ����� 0.5�ʸ��� �ѹ��� ��� ����
	// nate 2005-08-29 : ���� ����� ��ư Ŭ�� �ѹ��� �ѹ�����(�������� �ڵ��Է±���� skip�� ����)


	// by evilkiki 2009.07.29
	DWORD							m_dwUseFireworkTick;

	BOOL							m_bCanUsePower[ QUICK_SLOT_NUM ];
	DWORD							m_dwRClickDelay;		// mungmae 2005/10/8 : ��ٿ� ������ Ŭ�� ������ ����
	// Display CutePopUp......
	CScrollBox			*			m_pScrollBox;
	CScrollBox			*			m_pDelivBox;
	CScrollBox			*			m_pChSexBox;			// change sex
	BOOL							m_bIsScrollBox;
	BOOL							m_bIsDelivBox;
	BOOL							m_bIsChSexBox;			// change sex
	BOOL							m_bIsChangeHair;		// change hair
	BOOL							m_bIsWarTax;
	BOOL							m_bIsTower;
	BOOL							m_bIsWarReq;
	BOOL							m_bIsPetShop;
	BOOL							m_bIsPetKeep;
	BOOL							m_bIsGuildWar;
	BOOL							m_bIsMatList, m_bIsMatMakeRoom, m_bIsMatBattle, m_bIsMatDeath;
	BOOL							m_bIsMatSurvival, m_bIsMatSurvivalMon, m_bIsMatChat;
	BOOL							m_bIsTeleportItem;
	BOOL							m_bIsLowGuildMaster;
	//=====================================================================================
	// nate 2004 - 9 : GuildHunt
	BOOL							m_bIsGuildHunt;
	BOOL							m_bIsReservationWindow;
	//=====================================================================================
	int								m_iQuickSlotNum;
	// Display present  Info Box..
	D3DTLVERTEX						TLVec[_VERTEX_CNT];
	CSurface			*			m_pPresentBox[MAX_PRESENT_BOX];
	CSurface			*			m_pAngel[MAX_ANGEL_CNT];
	CTextOutBox			*			m_pPresentText;
	BOOL							m_bShowPresentBox;
	DWORD							m_MainTime, m_BoxTime, m_TextTime, m_AngelTime;
	int								m_BoxFrameCnt,  m_AngelFrameCnt;
	int								m_iTextFFrameCnt, m_iTextEFrameCnt;
	int								m_ScreenWidth, m_ScreenHeight;
	int								m_iMainXpos, m_iMainYpos;
	int								m_iStartX, m_iStartY;
	int								m_iCnt;
	BOOL							m_bAngelSta, m_bAngelAniActive;
	BOOL							m_bShowAngel;
	BOOL							m_bF11;
	int								m_iCurMode;
	//--------------------------------------------------
	// ���� �˸� ���
	CSurface			*			m_pLetterMsgBox[MAX_LETTER_MSG_BOX];
	BOOL							m_bShowLetterMsgBox;
	int								m_LetterTime,m_LetterFrameCnt,m_iMove;
	BOOL							isFirstNewLetter;
	//--------------------------------------------------
	// �޼��� ť ���� ..
	dnode				*			head;
	dnode				*			tail;
	// ������ ����.
	BOOL							m_nCastleDoorState;
	BOOL							m_bOnBattle;
	//============================================
	// nate 2004 - 4 : CastleWar - ����.
	BYTE							m_byEndDecardiCastleWar;
	BYTE							m_byOnBattleDecardi;
	char				*			m_szDecardiGuildGamma;
	char				*			m_szDecardiGuildDelta;
	char							m_MasterGuild1[100];
	char							m_MasterGuild2[100];
	char							m_MasterGuild3[100];
	int								m_MasterZone1;
	int								m_MasterZone2;
	int								m_MasterZone3;
	char							m_SubGuild[4][100];	
	CWarInfo			*			m_pWarInfo;
	BOOL							m_bWarInfo;
	int								m_iCurDelivState;
	int								m_whichShift;			// 0: ����, 1: ������ // ������ ����Ʈ�� ���ȳ� �ƴϸ�.. ���� ����Ʈ�� ���ȳ�...?
	// ���� ���� UI ����.
	CChangeHair			*			m_pChangeHair;
	CWarTax				*			m_pWarTax;
	CGuardTower			*			m_pTower;
	CWarRequest			*			m_pWarReq;
	CControlPetShop2	*			m_pPetShop;
	CControlPetKeep		*			m_pPetKeep;
	CGuildWar			*			m_pGuildWar;
	CMatMakeRoom		*			m_pMatMakeRoom;
	CMatList			*			m_pMatList;
	CMatBattleUI		*			m_pBattleUI;
	CMatSurvivalUI		*			m_pSurvivalUI;
	CControlTeleportItem	*		m_pTeleportItem;
	UI_GmMacro*						m_pGmMacro;
	CControlLowGuildMaster	*		m_pLowGuildMaster;
	CGuildHunt				*		m_pGuildHunt;			// nate 2004-9 : GuildHunt - ����� ���� ���� ������
	CReservationWindow		*		m_pReservationWindow;	// nate 2004-9 : GuildHunt - ����� ���� ���� ������
	BYTE							m_byDoorOpen;			// nate 2004-9 : GuildHunt - ��� ������� ������ ���Թ� ����
	// ����� score ..
	CSurface				*		m_pBack_Score;
	CTextOutBox				*		m_pTextScoreA;
	CTextOutBox				*		m_pTextScoreB;
	CTextOutBox				*		m_pTextTeamNameA;
	CTextOutBox				*		m_pTextTeamNameB;
	BOOL							m_bOnTeamBattle;
	BOOL							m_bShowScore;
	BOOL							m_bSummonLock;
	DWORD							m_timeSummonLock;
	BOOL							m_bSubAttack;
	BOOL							m_b_Attack;              //-- ���� ���� ���ΰ�.
	BOOL							m_b_MotionForAttack;     //-- ���� ���� ������ΰ�.
	BOOL							m_b_MovingForAttack;     //-- ���� ������ ���� �����̰� �ִ°�
	BOOL							m_b_SecAttack;           //-- ���� SecAttack �� ����Ǿ� �ִ°�.
	BOOL							m_b_QuickAttack;         //-- ���� QuickSlot Sec �� ����Ǿ� �ִ°�.
	BOOL							bControlKey;             //-- ��Ʈ��Ű ���� �Ǵ�.
	BOOL							m_bChakram;              //-- ��ũ���� �����Կ� ������ ���Թ�ȣ�� �����°�?
	BOOL							m_bChakramAtt;           //-- ���ʸ��콺�� ���� ����� Ŭ���ߴ°�?
	int								m_ChakramSlotNum;        //-- ���õ� Quick Slot ��ȣ
	BOOL							m_bBookSpell;            //-- ��ũ���� �ִ� �� ������ �����°�
	BOOL							m_bBookSpellAtt;         //-- ���� ����� Ŭ���ߴ°�.
	int								m_BookSlotNum;           //-- ���õ� Quick Slot ��ȣ
	//wan:2004-12 : whitehorn dungeon - ������ ����Ʈ �÷��� TRUE:����,FALSE:����
	BOOL							m_bBossGateClosedRender;
	BOOL							m_SummonMobControl;		 //��ȯ�� ��Ʈ��Ű.1029
	BOOL							m_F1_Help;				 //f1Ű�� help�� ������ ����Ű�� ������...
	BOOL							m_QuickKeyUnlock;		//f1Ű ����Ű �����Ҷ� help�ߴ°� ��������...
	BOOL							m_bTeleport;			//1126 �ڷ���Ʈ�� �������� ����.
	BOOL							m_bMoveKit;				//1217 �̵���ġ�� �� �̵�����Ʈ�ΰ�...
	BOOL							m_bParosKit;			//�ķν� �̵���ġ
	BOOL							m_bDmitron;				//���Ʈ�� �����
	BOOL							m_bMarboden;			// [2008/6/23 Theodoric] �������� �̵���ġ
	BOOL							m_bMultiAttack;			//0110 �����̾�Ʈ�� ��Ƽ�����ΰ�...
	DWORD							m_dwVk1KeyTime;			// automouse üũ
	BOOL							isFirstViewGuildMsg;
	// nate 2004 - 4 : quest
	BYTE							m_byQuest2Start;
	int								m_nWarpGateIndex;		// nate 2005-03-10 : Matrix - ���� ����Ʈ ���ؽ� ����
	int								m_nStoneState[ 12 ];	// ��Ʈ���� ���� ���� ����
	int								m_nStoneState2[ 12 ];	// WORLD_MATRIX_2009 ��Ʈ���� ����Ʈ ���� �� ����
	CRectPlane			*			m_pMatrixMark;			// nate 2005-03-21 : Matrix - �ٴ� ��ũ �׸���
	int								m_nClickTimeCheck;		//mungmae-2005/04/11 - ����: ��ĭ�޺�(CNKCharacter�� m_bComboReady����)�Ҷ� ���콺 �ѹ��� ������ �ϱ� ����.
	//wan:2005-04	���̺긮�� �뽬��ų�� ���� �����̵� ������ üũ �÷���
	BOOL							m_bNextGoto;			//�����̵��߿� goto�� �ޱ� ������ FALSE
	BYTE							m_bRClickDown;			//mungmae 2005-07-04: ���콺 ���� üũ(���� ���³�? �� ���³�?)
	BOOL							m_bSendMessage;
	BYTE							m_byCheckHellDown;		// ��ٿ��� ��������� üũ 0 : Useing HellDown
	DWORD							m_dwRevivalAccTime;		// mungmea-2005/06/23 : ����콺 ���� ���� Ÿ��
	int								m_nGuildPoint[3];		// mungmae-2005/06/23 : ����콺 �� ������ ������ ����
	CRectPlane			*			m_pNamPlane;			// nate 2004 - 6 : CastleWar �� ���� �ٴ�ǥ��
	CNkMob				*			m_pSummonMob[2];		// nate 2005-11-28 : ���̵�� ��ȯ�������� 0: Normal Summon, 1 : Sun flower Summon
	DWORD							dwDelayTime;
	char							m_strProcessPlayerName[30];	// ���� ���� ��� ����
	DWORD							m_dwUseQuickTick;		// ���������� �������� (����Ű��)����� �ð��� ���. ����� �ٷ� �����Կ��� �������� ���� ���� ���´�.(06-02-07 ����)
	BOOL							m_bWorldCupDash;		// mungmae-2006/06/13 : ������ ���ǵ� ��
	BOOL							m_bWorldCupDashReady;	// mungmae-2006/06/13 : ������ ���ǵ� �� �غ� ����
	DWORD							m_dwDashUseTime;
	BOOL							m_bOneTime;				// mungmae-2006/06/13 : �ѹ� �� �����Ű�� ����
	// �̺�Ʈ �� ����.
	int								m_nNowPlayingEvent;		// ���� �������� �̺�Ʈ ���� �ε����� ���.(0�̸� �̺�Ʈ ���� �ƴ�.)
	DWORD							m_dwEventStartTime;		// ���� �ð��� ���.
	CEventLegKnockAppear*			m_pEvent_LegKnock_Appear; // ��ī�� ���� ���׳��� ����� �̺�Ʈ.
	CEventScriptActor	*			m_pEventScript;			// �̺�Ʈ ��ũ��Ʈ ///Ʃ�丮�� �� �߰�
	BOOL							m_bAutoHunt;			// ��� �ڵ� ��� ��� �߰�.

	HANDLE							m_hWaitObject;
	std::queue<int>					m_GuildMarkStack;
	list_guildmark					m_GuildMarkList;
	list_guildtemp					m_GuildMarkTemp;

	HANDLE							m_hThread;				// thread handle
	unsigned						m_ThreadID;				// thread id
	BOOL							m_bIsThread;			// if true, i used thread.
	int								m_nServerIndex;			// server index number

	CHttpConnection		*			m_phttpConnection;
	CLOCK							m_GuildMarkMutex;

	DWORD							m_AutoItemTakeDelayTime;
	CAutoPlay			*			m_AutoPlay;
	BOOL							m_bPaymentLagrush;
	//=====================================================================================================
	// protect function
protected:
	int								ShowName(int &target_idx);
	void							CalNamePos(float wx, float wy, float wz, int which);
	void							CalNamePos(CItemInfo *pItemInfo, float wx, float wy, float wz, int which);
	BOOL							DontShowNameMob( CNkMob *pMob );						// ����� �������� ǥ������ ���� ���� ����Ʈ
	DWORD							SetShowName( CTextOutBox *pNamebox, CNkMob *pMob ); 	// [3/30/2007 Theodoric] ����� �� ǥ�� :: �̸�ǥ�� �κ��� ���� �Լ��� �������.
	void							Except_All_False(int val);
	void							LoadGuildMarkURL();
	//=====================================================================================================
	// public function
public:
	// [5/14/2009 D.K ] : modified by D.K.
	BOOL							IsInside_MouseDown_Itm_Namebox(int x, int y);
	BOOL							IsInside_MouseMove_Itm_Namebox(int x, int y);
	// constructor
	CUIMgr();
	virtual ~CUIMgr();
	//============================================================================================
	// Initialized UI Manager and management of UI Manager.
	// Test �� ����
	void							AddPlayerTest();
	void							DeletePlayerTest();
	void							AttackTest();
	void							TestEffect(int type);
	// render.
	void							Render();													// main render function
	void							RenderDecoObjs();											// render decoration
	void							RenderPlayers();											// render character and other player
	void							RenderMobs();												// render monsters
	void							RenderUserShopNpc();										// render market and shop
	void							RenderPets();												// render pets.
	void							RenderItems();												// render inventory item and drop item.
	void							RenderAlphaObjs();											// render alpha object.
	void							DrawChatPopups();											// render chatting popup view.
	void							InitChatPopups();				// initialize chatting popup view.
	//	void							RenderEffects();										// nate 2004 - 7 notused
	// connect object pointer ��ü ����.
	void							ChangeEnvironment();										// change game environment.
	void							DeleteRes();											// destory to device
	void							Init(HWND hwnd, CLand *pLand);								// initialize UI Manager.
	void							SetHwnd(HWND hwnd)
	{
		m_hWnd = hwnd;    // set main window handle to UI Manager.
	}
	void							SetLandPtr(CLand *pLand)
	{
		m_pLand = pLand;    // Set world map pointer to UI Manager.
	}
	void							SetEnergyBar();												// set energy bar draw position
	void							SetFrustum(CFrustum *pFrustum)
	{
		m_pFrustum = pFrustum;    // set frustum culling
	}
	// ��� ����
	BOOL							InitSocket();												// initialize Socket process.
	BOOL							ConnectToServer();											// connect to server. if connect fail to server, return false.
	// �̵� ����
	void							MusicStop();												// stoped music(MIDI)
	void							MusicPlay(char* fileName);									// play music from this file.
	// ������ ���μ��� ó�� �Լ�
	void							ProcessFrame();	
	void							ReadMessages();
	void							ProcessOneSentence(char *msg);
	// �޽��� ó�� ��ƾ
	int								Message(UINT uMsg, WPARAM wParam, LPARAM lParam);			// called from MAIN Thread.
	// ī�޶� �̵�ó��.
	BOOL							IsAdjustingCam()
	{
		return m_bAdjustingCamera;
	}
	BOOL							FixCameraPos();
	BOOL							Save_Screen_Shot(HWND hWnd);
	//========================================================================================================
	// process command and game input. ��ɾ� ó�� �� ���� �Է� ����.
	// [6/7/2007 Theodoric] ���� �Է½� �����԰� ����� �Է� ����ó�� �Լ���
	// ��... �ϳ��� �����ص� �ɰ� ������...���� �鿩�� ����.
	bool							IsCheckKeyUsed();
	bool							IsQuickSlotUsed();
	void							CheckKeyEvent();											// �񵿱� Ű���� �̺�Ʈ�� ���콺 ������ ã��.
	void							TraceMouseState();											// ĳ���� �̵��� ���õ� ���콺 ó���� �Ѵ�.
	void							TraceRMouseState();
	// ��ɾ� ó���� ���콺 ����.
	void							CmdAttack(char *msg);
	void							CmdMultiAttack(char *arg);
	int 							CmdClick(int which_button, WPARAM wParam, int mx, int my);
	int								CmdLShift();
	int								CmdRShift();
	int								MouseDown(int x, int y, WPARAM wParam, int which_button);	// nouse move
	void							ShowLShiftMenu();
	void							ShowRShiftMenu(CNkCharacter *pCha);
	WORD							WhatColorOnPick(int x, int y);								// pick buffer���� �÷� ��������.
	// �������� �����Ǵ� �����쿡���� �˻��� �ǽ��Ѵ�.
	BOOL							Check_All_RealtimeBox();
	void							SetModal(BOOL bModal)
	{
		m_bModal = bModal;
	};	// ��� ����.
	void							CmdErrorMsg(char *Name);
	void							CmdLoadZonePos();
	// auto mouse üũ.
	BOOL							IsVk1Pressed();

	//********************************************************************************************
	//* message ����.
	void							init_queue() ;
	void							put(char* msg);
	BOOL							get();
	void							clear_queue();
	int								check_queue();
	char							*GetFirstMsg();
	void							delete_queue_ptr();
	BOOL							HelpMessage(int num);//1203 help �޽���
	BOOL							GuildMessage(int guildindex, bool isnetmable); // ��� ���� �޼���
	BOOL							MatrixMessage(int num);// Matrix �޽���

	//========================================================================================================
	// function
	void							Siren_flag(int switch_flag, int nCharIdx, int color = 0);	// wanted(�������)
	WORD							GetPickColor()
	{
		return m_wColor;    // ������ ��ŷ �÷��� ���������� �Լ�.( 06-06-13 ���� �߰� )
	}
	void							SetDecardiGuildName( char* GuildName, BYTE byGammaDelta );
	void							GuildHuntLandAttr( BOOL bPass );							// nate 2004 - 10 guildhunt system.
	void							SetShowVital(int mob_idx, int vital);						// ���� ü���� �ٲ�� ���̴� �͵� �ٲ�����...

	//********************************************************************************************
	//* ������ ã��. ���̳� user�� �� ��ġ�� id���� ã�Ƽ� ������ ��´�.
	CNkMob				*			FindMobById(int id);
	CUserShopNpc		*			FindUShopNpcById( int id ); ///��������
	CNkPet				*			FindPetById(int id, BOOL with_my = TRUE);
	CNkCharacter		*			FindChaById(int id, BOOL with_me = TRUE);
	CDecoObj			*			FindDecoObjByColorIdx(WORD colorIdx);
	CNkCharacter		*			GetChaFromPickColor( DWORD PickColorIndex); // ��ŷ Į��� ���õ� ĳ���͸� ã�´�.

	//********************************************************************************************
	//* Add Something ĳ����, ����, ��, ������, ��NPC�� ���� �߰��Լ���. ���� ���.
	// nate : �븸 ���� ���� ���� - ĳ������ ���� �߰�
	void							AddPlayer(int id, const char *name,float fLevel,_int64 fame, int nShowMark, float x, float z, 
			float dir, int nRace=0, int nSex=0, int nHair=0, int *wearing_vnum = NULL,
			int nCombatState = 0, int condition=0, int skill=0, int extra=0,
			int chaotic_grade = 0, int free_pk=0, int fly_level=0, char *guild_name = NULL,
			char *guild_grade_name=NULL, int guild_index=0, int guild_type=0
					, int guild_idx = 0
						);
	void							AddMob(int mob_num, int id, float x, float z, float dir, int vital, int mutant = 0,
										   int attribute=0, int mobclass=0, int nLevel = 0, int RegenType = 0 ); // ���� �÷��� �߰� ///�δ�1
	void							AddPet(int id, char *name, int pet_class, float x, float z, float dir, int vital,
										   int horn, int head, int body, int wings, int legs, int tail, BOOL bMeta);
	void							AddItem(int item_index, int vnum, float x, float z, float dir, int gem_num,
											int special, int special2, BOOL bTimeItem, BOOL bDrop = FALSE); ///�Ź���ȭ�ý���
	void							AddUserShopNpc( int MasterIndex, int Vnum, int NpcIndex, float x, float z, float dir,
			const char *name, const char *ment );		// ��������

	//********************************************************************************************
	//* Delete Something ����� �Լ���. ���������� �����.
	void							DeletePlayer(int id);

	// DelMob�Լ��� ��ü
	//void							DeleteMob(int id);

	void							DeletePet(int id);
	void							DeleteItem(int item_index);
	void							DeleteUserShopNpc( int NpcIndex );							// ��������
	void							EraseAllList();												// all delete.

	//********************************************************************************************
	//* Insert Effect. ����Ʈ �����ֱ�. ��Ȯ�ϰԴ� Add�� ����� ������.
	void							InsertPetHitEffects(CNkPet *pet_from, CNkMob *mob_from, CNkCharacter *ch_to,
			CNkPet *pet_to, CNkMob *mob_to, int skill_idx);
	void							InsertHitEffects(CNkCharacter *ch_from, CNkMob *mob_from, CNkCharacter *ch_to,
			CNkMob *mob_to, Hit_Index hit_index);
	void							InsertMagic(CNkCharacter *ch_from, CNkMob *mob_from, CNkCharacter *ch_to, CNkMob *mob_to, int skill_idx);

	//********************************************************************************************
	// Move world or floor. �̵����� �Լ���. �ȴ� ���� ���ܵǾ� ����. ����� ���� �� �̵���.
	// ���� �̵� ����.
	int								GetChangeWorld()
	{
		return m_nChangeWorldIdx;
	}
	int								IsChangeWorld(D3DVECTOR &vLoc);
	void							PrepareChangeWorld(BOOL bUseGate, int nWorldIdx, int LocalIdx, int sub = 0);
	void							GoWorld(char *strIpAddr, int nPort, int nWorldNum = 0, int nAreaNum = 0);
	void							GoWorldFail();
	// ����Ʈ�������� �̵�.
	int								CanTransFloor(int nWorldNum);
	void							TransFloor(int nWorldNum, int gate_idx);
	int								GetFloor()
	{
		return m_nCurFloor;
	};
	void							SetFloor(int nFloor)
	{
		m_nCurFloor = nFloor;
	};
	// ��ġ�� �ȵ�!!
	BOOL							IsNotBlocked(float x1, float z1, float x2, float z2, BOOL bIgnore = FALSE);
	// �̵�����Ʈ �� ����.
	BOOL							IsValidGate(int world_idx, int gate_vnum, float x, float z);

	//********************************************************************************************
	//* Update Something. ����� �����۸�. ���߿� ������Ʈ�� �ϴ� �Լ����� ���� ��������..
	// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void							UpdateItem( int Index, int vnum, int plus, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int Maxendur );	// ������ ���� ���ſ� ���� �Լ�(05-10-21). ///�Ź���ȭ�ý���
	// �������� �Լ�
	void							SuccessWork(int slot_num, int gemstone_index, int gem_index);
	void							FailedWork(int slot_num);
	// Upgrade ���� �Լ�.
	void							Upgrade(int nSuccess, int where, int item_index, int gem_num);

	//********************************************************************************************
	//* ���� �ŷ� �� ������ �ŷ�����.
	// �ŷ����� �Լ���.
	void							AskExchange(char *player_name);
	void							ExchStart(char *player_name, int nPlayer_level);
	void							ExchMoney(char *strMoney);
	void							ExchInvenTo(int x, int y, int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur);
	void							ExchInvenFrom(int x, int y);
	void							Exch(char *cmd);
	// ������ȯ �Ǵ� Ĩ�׺�
	void							ChipExchangeMulti(int type, BOOL bResult, int vnum, int count, int binggo_num, char* arg = NULL);
	void							ChipExchange(int type, BOOL bResult, int vnum, int svr_idx, int binggo_num);
	void							EventAncientExchange(int result, int type, int vnum_money, int index, int gem_num);
	void							EventTreeExchange(int result, int type, int vnum, int index);
	void 							EventMultiExchange( int NumItem, char* Packet ); // �̺�Ʈ �������̽� �󿡼� ������ ������ ��ȯ ������ ������ �Լ�. (by ����) ///���߱�ȯ
	//�÷�Ƽ�� ���� �Լ�
	void							PlatimumExch(int result, int type, int vnum_money, int index, int gem_num);
	void							ExchPutTarget( int nItemIndex, int nVnum, int nPlusNum, int nSpecialNum, int special2,
			int UpEndur, int MaxUpdEndur, int nX, int nY, int Endur, int MaxEndur );
	void							ExchPutInven( int nInvenPackNum, int nX, int nY );
	void							ExchPutExch( int nItemIndex, int nVnum,  int nPlusNum, int nSpecialNum, int special2,
			int UpEndur, int MaxUpdEndur, int nX, int nY, int Endur, int MaxEndur );
	void							ExchGetTarget( int nX, int nY );
	void							ExchGetInven( int nInvenPackNum, int nItemIndex, int nVnum, int nPlusNum, int nSpecialNum, int special2,
			int UpEndur, int MaxUpdEndur, int nX, int nY, int Endur, int MaxEndur );
	void							ExchGetExch( int nX, int nY );

	//********************************************************************************************
	//* ������ ���� ����.
	void							ShowDroppedItemName(CItemInfo *pItemInfo);
	bool							CanUseQuickItem(CItem* pUseQuickItem);			// nate 2006-03-23 : �� ������ ��� �������� üũ
	void							SetItemTimeLimit( int item_index, int time );	// �������� �ð��� ���������� �����Ѵ�.(by ����) ///�ð���������
	void							SetItemExPrice( int item_index, int PriceType, int Price ); // Ư�������� �����Ѵ�. ///BP����.

	//********************************************************************************************
	//* using quick item..�� �������� ����!!
	BOOL							CanUsePower(CItem *pItem);
	void							UseQuickItem(int slot_num);
	void							UseQuickPower(int slot_num);
	void							UseQuickGemstone(int slot_num);
	void							UseQuickEtc(int slot_num);
	void							UseQuickFindPetItem(int slot_num);				// ���� ���ã�� ������
	bool							CanUseFindPetItem();							// ���̿� ���ã�� �������� ����� ���ִ°�?
	///2003 �������� ���� ������ ��������� ���� �Լ�
	void						    UseQuickSubItem(int slot_num);					//Ŭ���̾�Ʈ ó��
	void							UseQuickSubItem(int del_slot, int fill_slot,int plus_num, int special_num); //���� ó��.


	bool CanUseQuickSubItem(CItem *pUseQuickItem);

	bool UseGemStone(CItem *pUseQuickItem);
	bool UsePart(CItem *pUseQuickItem);
	bool UseRecovery(CItem *pUseQuickItem);
	bool UseGem(CItem *pUseQuickItem);
	bool UseGift(CItem *pUseQuickItem);
	bool UseForQuick(CItem *pUseQuickItem);
	bool UseEtc(CItem *pUseQuickItem);
	bool UseGemChip(CItem *pUseQuickItem);
	bool UseBuff(CItem *pUseQuickItem);

	bool UseQuest(CItem *pUseQuickItem);
	bool UseQuest(int slot, char * param);

	bool UseWarp(CItem *pUseQuickItem);
	bool UseWarp(int slot, char * param);

	bool UsePet(CItem *pUseQuickItem);

	bool UsePremium(CItem *pUseQuickItem);
	bool UsePremium(int slot , char * param);

	bool UseDocument(CItem *pUseQuickItem);
	bool UseDocument(int slot, char * param);

	bool UseUsing(CItem *pUseQuickItem);
	bool UseUsing(int slot, char * param);

	bool UseScroll(CItem * pUseQuickItem);
	bool UseSummon(CItem * pUseQuickItem);
	bool UseShell(CItem * pUseQuickItem);
	bool UseCharge(CItem * pUseQuickItem);
	bool UseMobSummon(CItem * pUseQuickItem);
	bool UseNpcSummon(CItem * pUseQuickItem);

	bool UseShout(CItem* pUseQuickItem);

	int  GetQuickIndex(CItem* pQuickSlotItem);

	int	GetNum2_ZoneCount(int value);

	bool IsMatrix();
	bool IsStartZone();

	//********************************************************************************************
	//* ��ǰ ������.
	void							Stash(int slot_num, int vnum, int plus, int special, int special2,
										  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur , int count, int time_limit);
	void							StashTo(int slot_num, int vnum, int plus, int special, int special2,
											int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag = 0);
	void							StashFrom(int slot_num, int index, int vnum, int plus, int special,
			int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count);
	void							StashEnd();
	void							guild_Stash(int slot_num, int vnum, int plus, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit );
	void							guild_StashTo(int slot_num, int vnum, int plus, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag = 0);
	void							guild_StashFrom(int slot_num, int index, int vnum, int plus, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count);
	void							guild_StashEnd();
	// ������ �ټ� �̵� ���� �Լ�
	void							guild_StashPut( int nItemNum, int mWarePackNum, int nInvenPackNum,
			int nWareSlotNum, int nInven_X, int nInven_Y );
	void							guild_StashGet( int nItemNum, int nInvenPackNum, int mWarePackNum,
			int nWareSlotNum, int nItemIndex, int nInven_X, int nInven_Y );
	void							StashPut( int nItemNum, int mWarePackNum, int nInvenPackNum,
			int nWareSlotNum, int nInven_X, int nInven_Y );
	void							StashGet( int nItemNum, int nInvenPackNum, int mWarePackNum,
			int nWareSlotNum, int nItemIndex, int nInven_X, int nInven_Y );

	//********************************************************************************************
	//* ���� & ���� ���� �Լ���.
	void							CmdMsRequest(char *player_name);
	void							CmdMsStart(int type, char *player_name, int nSection );
	void							MsInvenTo(int x, int y, int index, int vnum, int gem_num, int special, int nSection,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur );
	void							MsInvenFrom(int x, int y, int nSection );
	void							MsMoney(char *strMoney, int nSection );
	void							MsSuccess(int clear_flag, int vnum, int index, int x, int y, int nSection );
	void							MsFail( int nSection = 0 );
	void							CmdMsUpgrade(int nSuccess, int where, int item_index, int special, int special2);
	void							JewelExchange(BOOL bResult, int vnum, int svr_idx);
	// �������� ��ȯ// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
	void							InterchangeTo(int old_idx, int new_idx, int vnum, int plus_point, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur);
	void							ProcessInvenTo(int x, int y, int index, int vnum, int gem_num, int special,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur  );
	void							ProcessInvenFrom(int x, int y );
	void							ProcessMoney(char *strMoney );
	void							ProcessSuccess(int vnum, int index, int x, int y );
	void							ProcessFail();

	//********************************************************************************************
	//* ���.
	void							CmdExchPetClear();
	void							CmdPetExchOppInfo(char* PetName, char* Petlevel, char* PetHP, char* PetAttack,
			char* PetDefense, int PetClass=0, int Skill1_Level=0,
			int Skill2_Level=0, int Skill3_Level=0, int Skill4_Level=0 );// �̸� ���� ����� ���ݷ� ����
	void							CmdSetKeepInfo(char *Name, int KeepMode, int level, int health,
			int loyalty, int hunger, int pay, int time);
	// PetProduct ���� �Լ�
	void							PetAssemblySuccess(int vnum,int index, int slot_x, int slot_y);
	void							PetAssemblyFail(BOOL bfail);

	//********************************************************************************************
	//* �̺�Ʈ ����. �Ⱦ��� �̺�Ʈ�� �������� �̺�Ʈ.
	//���� ��� �̺�Ʈ ���� �Լ�
	void							SummerEvent(int bsucceed, int type, int vnum, int index);
	// ī���̼� ��ȯ â ����
	BOOL							CheckCarnationExist();
	void							CheckCarnation(int);
	// �̺�Ʈ ���� �����ϱ� ���� �����.(�̺�Ʈ ���� �߿� �������̰�, ī�޶� ���� �����ȴ�.)  ///�δ�1 �� �߰�
	BOOL							IsEventScenePlaying()
	{
		return m_nNowPlayingEvent;    // �÷��� ������ üũ.
	}
	void							InitEventScene();											// �̺�Ʈ �ŵ��� �ʱ�ȭ ///�δ�1 �� �߰�
	void							StartEventScene( int EventIndex, float PosX, float PosZ, float Dir ); // Ư�� ���� �̺�Ʈ �� ����
	void							PlayEventScene();											// Ư�� ���� �̺�Ʈ �� ��� ///�δ�1 �� �߰�
	BOOL							EventSceneMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // �̺�Ʈ ���� �޼��� ���ν���. ///Ʃ�丮�� �� �߰�
	void							EndEventScene();											// Ư�� ���� �̺�Ʈ �� ����. �ʱ�ȭ ///�δ�1 �� �߰�
	void							ReleaseEventScene();										// �̺�Ʈ �ŵ��� ���� ///�δ�1 �� �߰�
	//* ��ǰ �̺�Ʈ ����.
	void							CmdEvent(char *msg);
	void							CreAllFromMsg(char* msg);
	void							DeleteAllPresent();
	void							DrawAllPresent();
	void							ShowPresentText();
	void							ShowAngel();
	void							RenderOj(int xpos, int ypos, int cx, int cy);
	void							CmdEventtt(char *msg);									// Event Test
	// ������ ����.
	void							WorldCupTime(int time);
	void							WorldCupScore(int A_Team, int B_Team);
	void							WorldCupGuildName(char *ch_AGuild, char *ch_BGuild);

	//********************************************************************************************
	//* ���� �˸� ���.
	void							CreLetterMsgBox();
	void							DeleteLetterMsgBox();
	void							DrawLetterMsgBox();
	void							isNewLetter();

	//********************************************************************************************
	//* ���� ����.
	void							DecardiCastleWarLandAttr( int Where, BOOL bPass );		//DEC_TOWER
	void							OnBattle(int nOnBattle);
	void							OpenCastle(int nState);
	void							CloseCastle();
	void							OpenDoor( int nZone, int* nState );						// nate 2004 - 11 : ���Ϸ��� ��ȣž, ȭ��Ʈȥ�� �ܼ����� ������ �����κ��� ȣ��
	void							BattleCount(int nSec);
	void							TimeCount(char *guild_name, char *owner_name, int nOwnerSec, char *first_guild_name,
			int nFirstSec, float owner_x, float owner_z, int point);
	void							MatTimeCount( int nHawklawPoint, int nCoolrainPoint,
			int nShiberPoint,int nBattleTime );		// nate 2005-03-11 : Matrix - ��Ʈ���� ���� �� ���ھ�, ���� �ð�
	void							MatTimeCount( int nGuild1, int nGuild2, int nBattleTime ); // WORLD_MATRIX_2009�� �Լ�
	void							MatBattlePoint( int nBattlePoint);						//��Ʋ ����Ʈ
	BOOL							IsMasterGuild();
	BOOL							IsSubGuild(int m_nFlag);								//�ܼ� ���� üũ
	// ������ ��Ȳ ����
	void							DrawWarCastleInfo();
	void							ToggleWarInfo()
	{
		m_bWarInfo = !m_bWarInfo;
	}
	void							RecalPath();
	// ����¡�� ���� �����޼��� �ޱ�..
	void							GetServMsg_WarTax(int, int);
	// ������ ��û�� ���� �޼��� �ޱ�
	void							GetServMsg_WarReq(char*, int, int);
	int								IsPortal(D3DVECTOR &vLoc);

	//********************************************************************************************
	//* �����, PVP, ��� ����.
	void							GuildPK(int sub_type, char* GuildName1,char* GuildName2,int PKstate);
	void							Dictionary(char* word1,char*word2);
	void							GuildWarStaus(int nStatus, char *strTeamName, char *strBossName, char *strMoney, char *strHead);
	void							GWarCancel(int nCause);
	void							GWarNotify(char *strNotify);
	void							GWarPoint(int a_point, int b_point,char* a_team, char* b_team, CNkCharacter *pCha);
	void							GWarHead(char *a_head, char *b_head);
	void							AddTeamMem(char *strName, int nGrade);
	void							CreateScore();
	void							DestroyScore();
	void							ShowScore();

	void							guildmark_clear();
	void							guildmark_create(int nIndex);
	bool							guildmark_exist(int gidx);
	void							guildmark_render(int gidx, float fx, float fy);
	BOOL							thread_create(int nServerIndex);
	void							thread_finish();
	void							isguildmark(int index);
	void							clearguildmark();
	int								ThreadMsg();
	static unsigned __stdcall		ThreadDataLoad( void* pParam );


	void							PostLoadGuildMark();
	int								GetServerIndex()
	{
		return m_nServerIndex;
	}

	void							FindFreePkGuild(char *guild_name, int on_off);
	bool							IsInvasionGuildChar(WORD PickColorIndex);		  // ��ŷ �������� ����� ���� ã��.

	//********************************************************************************************
	//* ��Ʈ���� ����.
	void							ShowRoomList(int nRoomNum, int nRoomType, char *strRoomName, int nCount,
			int nMaxCount, char *strMaster, int nState, int nMinLevel,
			int nMaxLevel, int nFee, char *strPasswd);
	void							DoRoomListStart();
	void							MakeGameRoom();
	void							WaitingRoom();
	void							EnterRoom(int nRoomType, char* MyName, int nUserType);
	void							TRoomIn(char *strName, int which_loc, BOOL bWaiting, BOOL bOwner);
	void							TRoom(char *cmd, char *strName);
	void							TRoomUpdate(int what_cmd, char *strName);
	void							TRoomClose(BOOL bOut = TRUE);
	void							OpenMatrixGate();												//��Ʈ���� ���� ����(mungmae-03/21)
	void							CreateMatrixMark();

	//********************************************************************************************
	//* ��ȯ����.
	void							SetSummonLock(BOOL bLock);
	BOOL							CheckSummonToInven( int nDelSlot );							// nate 2004 - 4 : ��ȯ�� ������ �ڵ� ä��� ���
	void							SetQuest2StartFlag( BYTE byQuest2Start = 1 )
	{
		m_byQuest2Start = byQuest2Start;
	}

	//********************************************************************************************
	//* Ȯ�� �� ��Ÿ ��� ����. Compute.
	int								ComputeCharClass( int nLevel );
	void							ComputeMiss( char*, int );									// nate 2004 - 4 : Crash_Miss
	void							ComputeBlock( char *BlockCharType, int BlockCharIndex,
			char *AttackCharType, int AttackCharIndex ); // Block ȿ��. ///��
	void							TimeCount_Flag(int flagType, char *owner_name, char *guild_name);
	void							CmdProcessRequest(char *player_name);
	void							CmdProcessAccept(int type, char *player_name);
	void							ComputeCombo( int nCharIndex, int nFrameIndex );			// �޺� ����ϱ�
	void							ComputeEventPoint( int nCharIndex );						// �̺�Ʈ����Ʈ(EP)ȹ��� ���� �޼����� ���� ��Ÿ��

	//********************************************************************************************
	//* �� ĳ���� �ൿ ���� �� �� ���� �Լ�.
	// ����� �� ã��
	int								FindNearTarget(float base_x, float base_z, void *pBaseTarget, float distance,
			float area, bool bMob, int nTargetCount = 0, bool bStoneBash = false ); ///�����޺�
	int								FindLineTargetGuildWar( float LineStartX, float LineStartZ, float LineEndX, float LineEndZ,
			float LineThick, int nTargetCount, int GuildWarType ) ;// [2008/1/18 Theodoric]
	int								FindLineTarget( float LineStartX, float LineStartZ, float LineEndX, float LineEndZ,
			float LineThick, bool bMob, int nTargetCount ); // �β��� ���� ������ �浹 üũ �Լ� ///�����޺�
	void							HeroMoving();
	// ��ų ����.
	void							SkillOnOff(int skillTime, BOOL bChar, int index, int skill_index);
	///2003 ���� �����Լ�
	void							CmdSubAttack(char *msg);
	void							AssemblySuccess(int vnum,int index, int plus, int special);
	void							AssemblyFail(BOOL bfail);
	void							SendAttackMessage(char *Msg);
	bool							MouseClickEvent_NPC(CNkMob *pMob, int which_button);
	BOOL							IsRohSkill( int nSkill_Idx );										// nate 2005-06-17 : ��ĳ�� ��ų���� �Ǻ�
	BOOL							AutoItemTake( float itemTakeRange );								// [4/12/2007 Theodoric] ������ �ڵ� ����
	void							SpeedUpEffect(CNkCharacter *pCha, BOOL bVisual);					// �Ϲ� ����Ʈ(��ų ����Ʈ�ƴ�)�� ����
	void							CmdHelperRequest(char *player_name); 								//�İ���
	// ���Ǹ� ���.
	void							SpiritModeEffect(CNkCharacter *pCha, BOOL bVisual);					// ���Ǹ� ��� ����Ʈ
	void							SpiritPoint(int nGet_Thousand, int nGet_Hundred, int nGet_Ten, int nGet_One,
			int nTaget_Hundred, int nTaget_Ten, int nTaget_One);
	void							SpiritTime(int nRemainTime);
	void							CmdSumPart(int index,int mobIndex);									//�߰���ų �߰� �����Ǽ�
	void							SetPickColor( WORD wcolor )
	{
		m_wColor = wcolor;
	}

	//********************************************************************************************
	//* ��Ÿ �Լ�.
	// ��Ű�� ���Կ��� ó�� ( �븸 )
	void							TwPackage( int nDate );
	void							PKItemDrop(int nInvenNum,int nSlotX,int nSlotY,int nWidth,int nHeight);	//pk�� ������ ���(�߱��� ����)
	void							SetBRFalg(CItem *pItem, int flag);
	void							RandomCameraRotate();


	BOOL IsTeleportArea(float wx, float wz, float roh_wx, float roh_wz); // ���̵�� �׷���Ʈ ���ϴ� ����
	void CloseTeleportItem();

	CMatrixWarMessage	*m_pMatrixWarMsg;

	void			SetLagrushPayment( BOOL	_pay )
	{
		m_bPaymentLagrush = _pay;
	}
	BOOL			GetLagrushPayment( ); //				{ return m_bPaymentLagrush;	}

	LHColorKeyMap		m_colorkey_map;

	LHCharacterMap		m_character_map;
	LHMobMap			m_mob_map;
	LHUserShopNpcMap	m_usershopnpc_map;
	LHPetMap			m_pet_map;
	LHItemInfoMap		m_iteminfo_map;
	LHMobVisibleMap		m_mob_visible_map;

	bool			AddColorKeyMap(WORD colorkey, int index, int type , void * data);

	bool			AddCharacter(CNkCharacter* target);
	bool			AddMob(CNkMob* target);
	bool			AddUserShopNpc(CUserShopNpc* target);
	bool			AddPet(CNkPet* target);
	bool			AddItemInfo(CItemInfo* target);


	void			DelColorKeyMap(WORD colorkey);

	void			DelCharacter(int id);
	void			DelMob(int id);
	void			DelUserShopNpc(int id);
	void			DelPet(int id);
	void			DelItemInfo(int id);

	void			ClearAll();

	int				GetEntityByColorKey(int key,int type=0);

	CNkCharacter *	GetCharacterByColorKey(WORD colorkey);
	CNkMob *		GetMobByColorKey(WORD colorkey);
	CUserShopNpc *	GetUserShopNpcByColorKey(WORD colorkey);
	CNkPet *		GetPetByColorKey(WORD colorkey);
	CItemInfo *		GetItemInfoByColorKey(WORD colorkey);

	CNkCharacter *	GetCharacterByID(int id);
	CNkMob *		GetMobByID(int id);
	CUserShopNpc *	GetUserShopNpcByID(int id);
	CNkPet *		GetPetByID(int id);
	CItemInfo *		GetItemInforByID(int id);

	int  Check_Skill_Resurrection(int skill_Index, CNkCharacter *pCha ); // [5/29/2008 �����]  ���̵�� ��Ȱ ��ų ���� üŷ �Լ�.

	bool IsInvenLock();

	void ConvertWorldPos(float x, float z, float& rx, float &fz);


public:
	bool m_edit_autopath;


	bool Send_UseQuickItem(int vnum, int type, int slot, int index, char * param=NULL, bool need_lock=false);
	bool Send_UseQuickItem(CItem* pItem, char * param=NULL, bool need_lock=false);
	bool Send_MoveToItem(int slot , int pack , int index);

	bool Send_InvenToStash(char * param=NULL);
	bool Send_StashToInven(char * param=NULL);

	bool Send_InvenToBag();

	void StashPuts(int vnum , int ware_slot , int inven_pack , int item_index);
	void StashGets(int vnum , int ware_slot , int inven_pack , int item_index , int inven_x , int inven_y);

	void SetAttackState(bool activate);

	bool SkipMonster();

	void SetVisibleMob(int vnum, BOOL bVisible);
	BOOL IsVisibleMob(int vnum);
};

#define safe_delete(p)			{if(p) delete p; p=NULL; }
#define safe_delete_array(p)	{if(p) delete [] p; p=NULL; }

template<typename Container>
void safe_delete_sequence(Container& container)
{
	if( container.empty() )
		return;

	Container::iterator it = container.begin();

	for( ; it!=container.end() ; ++it )
		if(*it) delete (*it);

	container.clear();
}

template<typename Container>
void safe_delete_associate(Container& container)
{
	if( container.empty() )
		return;

	Container::iterator it = container.begin();

	for( ; it!=container.end() ; ++it )
		if( (*it).second )	delete (*it).second;

	container.clear();
}
template<typename Container, typename Keytype>
CNkMob* safe_delete_associate_colorkey(Container& container, Keytype key)
{
	if( container.empty() )
		return NULL;

	Container::iterator it = container.find(key);
	if( it!=container.end() )
	{
		if( (*it).second )
		{
			return (*it).second;
		}
	}
	else
		printf("�ش� ���͸� ã�� ���߽��ϴ�!!!\n");
}

template<typename Container, typename Keytype>
void safe_delete_associate_key(Container& container, Keytype key)
{
	if( container.empty() )
		return;

	Container::iterator it = container.find(key);
	if( it!=container.end() )
	{
		if( (*it).second )
		{
			delete (*it).second;
			container.erase(it);
		}
	}
}

#define SAFE_DELETE_RES(p)		{ if(p) p->DeleteRes(); }
#define SAFE_LOAD_RES(p)		{ if(p) p->LoadRes(); }

#endif // _UI_MGR_H_