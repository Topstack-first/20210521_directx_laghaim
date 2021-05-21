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
#include "EventLegKnockAppear.h" // 데카렌 보스 등장씬 이벤트. ///인던1 때 추가

class CLand;
class CTcpIpConnection;
class CNkCharacter;
class CNkMob;
class CUserShopNpc; ///점포상점
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
class CEventScriptActor; ///튜토리얼
class CAutoPlay;
class CMatrixWarMessage;
class UI_GmMacro;

// 휠마우스 처리를 위해서.
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#define PLAYERNAME_FONTSIZE				12
#define PLAYERNAME_FONTCOLOR			RGB(255, 255, 0)
#define NAME_ACTIVECOLOR				RGB(0, 255, 255)
#define MUSIC_NUM						1
// 범위 지정.
#define ATTACK_RANGE					(15.0f)
#define TOWER_RANGE						(20.0f)
#define HELL_ATTACK_RANGE				(60.0f)
#define GROUND_RANGE					(70.0f)
#define GP_RANGE						(50.0f)
#define GATE_RANGE						(60.0f)
#define CHAKRAM_ATTACK_RANGE			(130.0f)
// UI 범위 지정

#define VITAL_BACK_WIDTH_C				128
#define VITAL_BACK_HEIGHT_C				9
#define VITAL_FRONT_WIDTH_C				114
#define VITAL_FRONT_HEIGHT_C			7

#define VITAL_BACK_WIDTH				75
#define VITAL_BACK_HEIGHT				5
#define VITAL_FRONT_WIDTH				70
#define VITAL_FRONT_HEIGHT				3


#define VITAL_BACK_Y					10 // [3/30/2007 Theodoric] 생명력 바 표시 :: 수치 수정  
#define VITAL_FRONT_Y					11 // [3/30/2007 Theodoric] 생명력 바 표시 :: 수치 수정

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
// 구조체.
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
	WORD							m_wColor;				// 픽버퍼에 쓰이는 칼러 변수.

	CTextOutBox						m_NameBox;				// 객체의 이름을 보여주기 위한 클래스.
	BOOL							m_bShowName;			// 객체의 이름.
	int								m_nShowNameType;		// 플레이어인가 몬스터인가 NPC인가, 아이템인가. 0, 1, 2, 3
	int								m_ShowVitalId;
	WORD							m_wPrevName;			// 이전에 선택된 칼라색.
	int								m_Vital;

	BOOL							m_bAdjustingCamera;		// 카메라 정위치 시키기.
	CCmd				*			m_pCmd;					// 명령어 처리

	BOOL							m_bPocketPapa_Click;	// [5/7/2008 반재승] 태국 국왕 생신 이벤트 할아버지 클릭 체크
	float							m_papaClickTime;
	std::string						m_strGuildMarkURL;
public:
	// 게임 UI용 surface
	CSurface			*			m_pVitalBack;
	CSurface			*			m_pVitalBack_E;
	CSurface			*			m_pPetVitalFront;
	CSurface			*			m_pVitalFront;
	CSurface			*			m_pManaFront;
	CSurface			*			m_pStamFront;
	CSurface			*			m_pEpowerFront;

	//  [8/13/2009 ppmmjj83] 선택한 npc 게이지 이미지 변경
	CSurface			*			m_pVitalBack_Center;
	CSurface			*			m_pVitalFront_Center;

	bool							m_bMouseDown;
	DWORD							m_dwElapsedTime;
	BOOL							m_bRMouseDown;
	DWORD							m_dwRbuttonElapsedTime;
	// 연속 동작을 위한 UI
	BOOL							m_bNextAction;
	ACTION							m_Action;
	BOOL							m_bPickLock;			// pick 과 거의 동시에 인벤 아이템 집을시 인벤 아이템 사라지는 버그 방지용 플래그. ///pick_bug
	BOOL							m_bMsg;
	
	CLand				*			m_pLand;
	int								m_nChangeWorldIdx;		// 월드 이동.
	int								m_nChangeLocalIdx;		// 로컬 이동.
	int								m_nCurFloor;			// 퀘스트 존에서의 이동
	// 거래관련 멤버 변수.
	BOOL							m_bExch;
	char							m_strExchPlayerName[30];
	int								m_nExchCharId;			// nate 2005-06-07 : 교환자 식별

	char							m_strMsPlayerName[30];	// 정제 관련 멤버 변수
	BOOL							m_bModal;
	// "ALT" 키 기능 관련
	BOOL							m_bAlt;
	int								m_cal_item_x;
	int								m_cal_item_y;
	// 마우스 클릭 제한 // Mouse click limit
	DWORD							m_lastMoveTime;			// 클릭해서 움직이는 거 0.5 초마다 한번씩만 가능...
	DWORD							m_dwUseQuickItemTime;	// nate 2005-08-23 : 물약 사용은 0.5초마다 한번씩 사용 가능
	// nate 2005-08-29 : 물약 사용은 버튼 클릭 한번에 한번씩만(윈도우의 자동입력기능을 skip할 목적)


	// by evilkiki 2009.07.29
	DWORD							m_dwUseFireworkTick;

	BOOL							m_bCanUsePower[ QUICK_SLOT_NUM ];
	DWORD							m_dwRClickDelay;		// mungmae 2005/10/8 : 헬다운 오른쪽 클릭 딜레이 적용
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
	// 쪽지 알림 기능
	CSurface			*			m_pLetterMsgBox[MAX_LETTER_MSG_BOX];
	BOOL							m_bShowLetterMsgBox;
	int								m_LetterTime,m_LetterFrameCnt,m_iMove;
	BOOL							isFirstNewLetter;
	//--------------------------------------------------
	// 메세지 큐 관련 ..
	dnode				*			head;
	dnode				*			tail;
	// 공성전 관련.
	BOOL							m_nCastleDoorState;
	BOOL							m_bOnBattle;
	//============================================
	// nate 2004 - 4 : CastleWar - 공성.
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
	int								m_whichShift;			// 0: 왼쪽, 1: 오른쪽 // 오른쪽 시프트가 눌렸나 아니면.. 왼쪽 시프트가 눌렸나...?
	// 각종 게임 UI 정보.
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
	CGuildHunt				*		m_pGuildHunt;			// nate 2004-9 : GuildHunt - 길드사냥 예약 메인 윈도우
	CReservationWindow		*		m_pReservationWindow;	// nate 2004-9 : GuildHunt - 길드사냥 예약 서브 윈도우
	BYTE							m_byDoorOpen;			// nate 2004-9 : GuildHunt - 길드 사냥터의 보스몹 출입문 제어
	// 길드전 score ..
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
	BOOL							m_b_Attack;              //-- 현재 공격 중인가.
	BOOL							m_b_MotionForAttack;     //-- 현재 공격 모션중인가.
	BOOL							m_b_MovingForAttack;     //-- 현재 공격을 위해 움직이고 있는가
	BOOL							m_b_SecAttack;           //-- 현재 SecAttack 이 예약되어 있는가.
	BOOL							m_b_QuickAttack;         //-- 현재 QuickSlot Sec 가 예약되어 있는가.
	BOOL							bControlKey;             //-- 컨트롤키 누름 판단.
	BOOL							m_bChakram;              //-- 차크람이 퀵슬롯에 장착된 슬롯번호를 눌렀는가?
	BOOL							m_bChakramAtt;           //-- 왼쪽마우스로 공격 대상을 클릭했는가?
	int								m_ChakramSlotNum;        //-- 선택된 Quick Slot 번호
	BOOL							m_bBookSpell;            //-- 스크롤이 있는 퀵 슬롯을 눌렀는가
	BOOL							m_bBookSpellAtt;         //-- 공격 대상을 클릭했는가.
	int								m_BookSlotNum;           //-- 선택된 Quick Slot 번호
	//wan:2004-12 : whitehorn dungeon - 보스방 게이트 플래그 TRUE:닫힘,FALSE:열림
	BOOL							m_bBossGateClosedRender;
	BOOL							m_SummonMobControl;		 //소환몹 콘트롤키.1029
	BOOL							m_F1_Help;				 //f1키가 help로 쓰일지 단축키로 쓰일지...
	BOOL							m_QuickKeyUnlock;		//f1키 단축키 해제할때 help뜨는거 막기위한...
	BOOL							m_bTeleport;			//1126 텔레포트가 쓰였는지 여부.
	BOOL							m_bMoveKit;				//1217 이동장치로 뜬 이동리스트인가...
	BOOL							m_bParosKit;			//파로스 이동장치
	BOOL							m_bDmitron;				//드미트론 입장권
	BOOL							m_bMarboden;			// [2008/6/23 Theodoric] 마르보덴 이동장치
	BOOL							m_bMultiAttack;			//0110 워자이언트의 멀티어택인가...
	DWORD							m_dwVk1KeyTime;			// automouse 체크
	BOOL							isFirstViewGuildMsg;
	// nate 2004 - 4 : quest
	BYTE							m_byQuest2Start;
	int								m_nWarpGateIndex;		// nate 2005-03-10 : Matrix - 워프 게이트 인텍스 저장
	int								m_nStoneState[ 12 ];	// 메트릭스 스톤 상태 변수
	int								m_nStoneState2[ 12 ];	// WORLD_MATRIX_2009 메트릭스 게이트 상태 비교 변수
	CRectPlane			*			m_pMatrixMark;			// nate 2005-03-21 : Matrix - 바닦 마크 그리기
	int								m_nClickTimeCheck;		//mungmae-2005/04/11 - 설명: 불칸콤보(CNKCharacter에 m_bComboReady참조)할때 마우스 한번만 들어오게 하기 위해.
	//wan:2005-04	하이브리더 대쉬스킬을 쓸때 공간이동 중인지 체크 플래그
	BOOL							m_bNextGoto;			//공간이동중에 goto를 받기 전까진 FALSE
	BYTE							m_bRClickDown;			//mungmae 2005-07-04: 마우스 상태 체크(누른 상태냐? 띤 상태냐?)
	BOOL							m_bSendMessage;
	BYTE							m_byCheckHellDown;		// 헬다운이 사용중인지 체크 0 : Useing HellDown
	DWORD							m_dwRevivalAccTime;		// mungmea-2005/06/23 : 뫼비우스 리젠 누적 타임
	int								m_nGuildPoint[3];		// mungmae-2005/06/23 : 뫼비우스 팀 점수를 얻어오기 위함
	CRectPlane			*			m_pNamPlane;			// nate 2004 - 6 : CastleWar 팀 구분 바닥표시
	CNkMob				*			m_pSummonMob[2];		// nate 2005-11-28 : 에이디아 소환수포인터 0: Normal Summon, 1 : Sun flower Summon
	DWORD							dwDelayTime;
	char							m_strProcessPlayerName[30];	// 가공 관련 멤버 변수
	DWORD							m_dwUseQuickTick;		// 마지막으로 퀵슬롯을 (단축키로)사용한 시간을 기억. 사용후 바로 퀵슬롯에서 아이템을 빼는 것을 막는다.(06-02-07 원석)
	BOOL							m_bWorldCupDash;		// mungmae-2006/06/13 : 월드컵 스피드 업
	BOOL							m_bWorldCupDashReady;	// mungmae-2006/06/13 : 월드컵 스피드 업 준비 상태
	DWORD							m_dwDashUseTime;
	BOOL							m_bOneTime;				// mungmae-2006/06/13 : 한번 만 실행시키기 위해
	// 이벤트 신 정보.
	int								m_nNowPlayingEvent;		// 현재 진행중인 이벤트 신의 인덱스를 기억.(0이면 이벤트 중이 아님.)
	DWORD							m_dwEventStartTime;		// 시작 시간을 기억.
	CEventLegKnockAppear*			m_pEvent_LegKnock_Appear; // 데카렌 보스 레그녹의 등장씬 이벤트.
	CEventScriptActor	*			m_pEventScript;			// 이벤트 스크립트 ///튜토리얼 시 추가
	BOOL							m_bAutoHunt;			// 운영자 자동 사냥 기능 추가.

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
	BOOL							DontShowNameMob( CNkMob *pMob );						// 생명력 게이지를 표시하지 않을 몬스터 리스트
	DWORD							SetShowName( CTextOutBox *pNamebox, CNkMob *pMob ); 	// [3/30/2007 Theodoric] 생명력 바 표시 :: 이름표시 부분을 띄어내어 함수로 만들었다.
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
	// Test 용 렌더
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
	// connect object pointer 객체 연결.
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
	// 통신 관리
	BOOL							InitSocket();												// initialize Socket process.
	BOOL							ConnectToServer();											// connect to server. if connect fail to server, return false.
	// 미디 음암
	void							MusicStop();												// stoped music(MIDI)
	void							MusicPlay(char* fileName);									// play music from this file.
	// 프레임 프로세싱 처리 함수
	void							ProcessFrame();	
	void							ReadMessages();
	void							ProcessOneSentence(char *msg);
	// 메시지 처리 루틴
	int								Message(UINT uMsg, WPARAM wParam, LPARAM lParam);			// called from MAIN Thread.
	// 카메라 이동처리.
	BOOL							IsAdjustingCam()
	{
		return m_bAdjustingCamera;
	}
	BOOL							FixCameraPos();
	BOOL							Save_Screen_Shot(HWND hWnd);
	//========================================================================================================
	// process command and game input. 명령어 처리 및 게임 입력 관련.
	// [6/7/2007 Theodoric] 문자 입력시 퀵슬롯과 단축기 입력 예외처리 함수들
	// 음... 하나로 통일해도 될거 같은데...좀더 들여다 보자.
	bool							IsCheckKeyUsed();
	bool							IsQuickSlotUsed();
	void							CheckKeyEvent();											// 비동기 키보드 이벤트와 마우스 움직임 찾기.
	void							TraceMouseState();											// 캐릭터 이동과 관련된 마우스 처리만 한다.
	void							TraceRMouseState();
	// 명령어 처리와 마우스 상태.
	void							CmdAttack(char *msg);
	void							CmdMultiAttack(char *arg);
	int 							CmdClick(int which_button, WPARAM wParam, int mx, int my);
	int								CmdLShift();
	int								CmdRShift();
	int								MouseDown(int x, int y, WPARAM wParam, int which_button);	// nouse move
	void							ShowLShiftMenu();
	void							ShowRShiftMenu(CNkCharacter *pCha);
	WORD							WhatColorOnPick(int x, int y);								// pick buffer에서 컬러 가져오기.
	// 동적으루 생생되는 윈도우에대한 검색을 실시한다.
	BOOL							Check_All_RealtimeBox();
	void							SetModal(BOOL bModal)
	{
		m_bModal = bModal;
	};	// 모달 설정.
	void							CmdErrorMsg(char *Name);
	void							CmdLoadZonePos();
	// auto mouse 체크.
	BOOL							IsVk1Pressed();

	//********************************************************************************************
	//* message 관련.
	void							init_queue() ;
	void							put(char* msg);
	BOOL							get();
	void							clear_queue();
	int								check_queue();
	char							*GetFirstMsg();
	void							delete_queue_ptr();
	BOOL							HelpMessage(int num);//1203 help 메시지
	BOOL							GuildMessage(int guildindex, bool isnetmable); // 길드 공지 메세지
	BOOL							MatrixMessage(int num);// Matrix 메시지

	//========================================================================================================
	// function
	void							Siren_flag(int switch_flag, int nCharIdx, int color = 0);	// wanted(지명수배)
	WORD							GetPickColor()
	{
		return m_wColor;    // 현재의 픽킹 컬러를 얻어오기위한 함수.( 06-06-13 원석 추가 )
	}
	void							SetDecardiGuildName( char* GuildName, BYTE byGammaDelta );
	void							GuildHuntLandAttr( BOOL bPass );							// nate 2004 - 10 guildhunt system.
	void							SetShowVital(int mob_idx, int vital);						// 맙이 체력이 바뀌면 보이는 것도 바뀌어야쥐...

	//********************************************************************************************
	//* 데이터 찾기. 몹이나 user등 각 위치를 id별로 찾아서 정보를 얻는다.
	CNkMob				*			FindMobById(int id);
	CUserShopNpc		*			FindUShopNpcById( int id ); ///점포상점
	CNkPet				*			FindPetById(int id, BOOL with_my = TRUE);
	CNkCharacter		*			FindChaById(int id, BOOL with_me = TRUE);
	CDecoObj			*			FindDecoObjByColorIdx(WORD colorIdx);
	CNkCharacter		*			GetChaFromPickColor( DWORD PickColorIndex); // 피킹 칼라로 선택된 캐릭터를 찾는다.

	//********************************************************************************************
	//* Add Something 캐릭터, 몬스터, 펫, 아이템, 샵NPC등 정보 추가함수들. 제법 길다.
	// nate : 대만 훈장 관련 수정 - 캐릭터의 레벌 추가
	void							AddPlayer(int id, const char *name,float fLevel,_int64 fame, int nShowMark, float x, float z, 
			float dir, int nRace=0, int nSex=0, int nHair=0, int *wearing_vnum = NULL,
			int nCombatState = 0, int condition=0, int skill=0, int extra=0,
			int chaotic_grade = 0, int free_pk=0, int fly_level=0, char *guild_name = NULL,
			char *guild_grade_name=NULL, int guild_index=0, int guild_type=0
					, int guild_idx = 0
						);
	void							AddMob(int mob_num, int id, float x, float z, float dir, int vital, int mutant = 0,
										   int attribute=0, int mobclass=0, int nLevel = 0, int RegenType = 0 ); // 리젠 플레그 추가 ///인던1
	void							AddPet(int id, char *name, int pet_class, float x, float z, float dir, int vital,
										   int horn, int head, int body, int wings, int legs, int tail, BOOL bMeta);
	void							AddItem(int item_index, int vnum, float x, float z, float dir, int gem_num,
											int special, int special2, BOOL bTimeItem, BOOL bDrop = FALSE); ///신방어구강화시스템
	void							AddUserShopNpc( int MasterIndex, int Vnum, int NpcIndex, float x, float z, float dir,
			const char *name, const char *ment );		// 점포상점

	//********************************************************************************************
	//* Delete Something 지우기 함수들. 여러가지들 지운다.
	void							DeletePlayer(int id);

	// DelMob함수로 대체
	//void							DeleteMob(int id);

	void							DeletePet(int id);
	void							DeleteItem(int item_index);
	void							DeleteUserShopNpc( int NpcIndex );							// 점포상점
	void							EraseAllList();												// all delete.

	//********************************************************************************************
	//* Insert Effect. 이팩트 끼워넣기. 정확하게는 Add와 비슷한 구조다.
	void							InsertPetHitEffects(CNkPet *pet_from, CNkMob *mob_from, CNkCharacter *ch_to,
			CNkPet *pet_to, CNkMob *mob_to, int skill_idx);
	void							InsertHitEffects(CNkCharacter *ch_from, CNkMob *mob_from, CNkCharacter *ch_to,
			CNkMob *mob_to, Hit_Index hit_index);
	void							InsertMagic(CNkCharacter *ch_from, CNkMob *mob_from, CNkCharacter *ch_to, CNkMob *mob_to, int skill_idx);

	//********************************************************************************************
	// Move world or floor. 이동관련 함수들. 걷는 것은 제외되어 있음. 현재는 존과 층 이동만.
	// 월드 이동 관련.
	int								GetChangeWorld()
	{
		return m_nChangeWorldIdx;
	}
	int								IsChangeWorld(D3DVECTOR &vLoc);
	void							PrepareChangeWorld(BOOL bUseGate, int nWorldIdx, int LocalIdx, int sub = 0);
	void							GoWorld(char *strIpAddr, int nPort, int nWorldNum = 0, int nAreaNum = 0);
	void							GoWorldFail();
	// 퀘스트존에서의 이동.
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
	// 걸치기 안되!!
	BOOL							IsNotBlocked(float x1, float z1, float x2, float z2, BOOL bIgnore = FALSE);
	// 이동게이트 핵 막기.
	BOOL							IsValidGate(int world_idx, int gate_vnum, float x, float z);

	//********************************************************************************************
	//* Update Something. 현재는 아이템만. 나중에 업데이트를 하는 함수들은 전부 이쪽으로..
	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void							UpdateItem( int Index, int vnum, int plus, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int Maxendur );	// 아이템 정보 갱신용 범용 함수(05-10-21). ///신방어구강화시스템
	// 세공관련 함수
	void							SuccessWork(int slot_num, int gemstone_index, int gem_index);
	void							FailedWork(int slot_num);
	// Upgrade 관련 함수.
	void							Upgrade(int nSuccess, int where, int item_index, int gem_num);

	//********************************************************************************************
	//* 상점 거래 및 유저간 거래관련.
	// 거래관련 함수들.
	void							AskExchange(char *player_name);
	void							ExchStart(char *player_name, int nPlayer_level);
	void							ExchMoney(char *strMoney);
	void							ExchInvenTo(int x, int y, int index, int vnum, int gem_num, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur);
	void							ExchInvenFrom(int x, int y);
	void							Exch(char *cmd);
	// 보석교환 또는 칩겜블
	void							ChipExchangeMulti(int type, BOOL bResult, int vnum, int count, int binggo_num, char* arg = NULL);
	void							ChipExchange(int type, BOOL bResult, int vnum, int svr_idx, int binggo_num);
	void							EventAncientExchange(int result, int type, int vnum_money, int index, int gem_num);
	void							EventTreeExchange(int result, int type, int vnum, int index);
	void 							EventMultiExchange( int NumItem, char* Packet ); // 이벤트 인터페이스 상에서 여러개 아이템 교환 받을때 들어오는 함수. (by 원석) ///다중교환
	//플래티늄 관련 함수
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
	//* 아이템 정보 관련.
	void							ShowDroppedItemName(CItemInfo *pItemInfo);
	bool							CanUseQuickItem(CItem* pUseQuickItem);			// nate 2006-03-23 : 퀵 아이템 사용 가능한지 체크
	void							SetItemTimeLimit( int item_index, int time );	// 아이템을 시간제 아이템으로 셋팅한다.(by 원석) ///시간제아이템
	void							SetItemExPrice( int item_index, int PriceType, int Price ); // 특정가격을 지정한다. ///BP상점.

	//********************************************************************************************
	//* using quick item..퀵 아이템을 쓰자!!
	BOOL							CanUsePower(CItem *pItem);
	void							UseQuickItem(int slot_num);
	void							UseQuickPower(int slot_num);
	void							UseQuickGemstone(int slot_num);
	void							UseQuickEtc(int slot_num);
	void							UseQuickFindPetItem(int slot_num);				// 라비용 즉시찾기 아이템
	bool							CanUseFindPetItem();							// 라이용 즉시찾기 아이템을 사용할 수있는가?
	///2003 퀵슬롯의 서브 아이템 사용했을때 쓰는 함수
	void						    UseQuickSubItem(int slot_num);					//클라이언트 처리
	void							UseQuickSubItem(int del_slot, int fill_slot,int plus_num, int special_num); //서버 처리.


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
	//* 물품 보관함.
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
	// 아이템 다수 이동 관련 함수
	void							guild_StashPut( int nItemNum, int mWarePackNum, int nInvenPackNum,
			int nWareSlotNum, int nInven_X, int nInven_Y );
	void							guild_StashGet( int nItemNum, int nInvenPackNum, int mWarePackNum,
			int nWareSlotNum, int nItemIndex, int nInven_X, int nInven_Y );
	void							StashPut( int nItemNum, int mWarePackNum, int nInvenPackNum,
			int nWareSlotNum, int nInven_X, int nInven_Y );
	void							StashGet( int nItemNum, int nInvenPackNum, int mWarePackNum,
			int nWareSlotNum, int nItemIndex, int nInven_X, int nInven_Y );

	//********************************************************************************************
	//* 정제 & 가공 관련 함수들.
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
	// 서버와의 교환// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void							InterchangeTo(int old_idx, int new_idx, int vnum, int plus_point, int special, int special2,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur);
	void							ProcessInvenTo(int x, int y, int index, int vnum, int gem_num, int special,
			int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur  );
	void							ProcessInvenFrom(int x, int y );
	void							ProcessMoney(char *strMoney );
	void							ProcessSuccess(int vnum, int index, int x, int y );
	void							ProcessFail();

	//********************************************************************************************
	//* 라비뇽.
	void							CmdExchPetClear();
	void							CmdPetExchOppInfo(char* PetName, char* Petlevel, char* PetHP, char* PetAttack,
			char* PetDefense, int PetClass=0, int Skill1_Level=0,
			int Skill2_Level=0, int Skill3_Level=0, int Skill4_Level=0 );// 이름 레벨 생명력 공격력 방어력
	void							CmdSetKeepInfo(char *Name, int KeepMode, int level, int health,
			int loyalty, int hunger, int pay, int time);
	// PetProduct 관련 함수
	void							PetAssemblySuccess(int vnum,int index, int slot_x, int slot_y);
	void							PetAssemblyFail(BOOL bfail);

	//********************************************************************************************
	//* 이벤트 관련. 안쓰는 이벤트나 진행중인 이벤트.
	//여름 사냥 이벤트 관련 함수
	void							SummerEvent(int bsucceed, int type, int vnum, int index);
	// 카네이션 교환 창 관련
	BOOL							CheckCarnationExist();
	void							CheckCarnation(int);
	// 이벤트 신을 연출하기 위한 멤버들.(이벤트 연출 중엔 못움직이고, 카메라도 강제 조정된다.)  ///인던1 시 추가
	BOOL							IsEventScenePlaying()
	{
		return m_nNowPlayingEvent;    // 플레이 중인지 체크.
	}
	void							InitEventScene();											// 이벤트 신들의 초기화 ///인던1 시 추가
	void							StartEventScene( int EventIndex, float PosX, float PosZ, float Dir ); // 특수 연출 이벤트 신 시작
	void							PlayEventScene();											// 특수 연출 이벤트 신 재생 ///인던1 시 추가
	BOOL							EventSceneMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // 이벤트 신의 메세지 프로시져. ///튜토리얼 시 추가
	void							EndEventScene();											// 특수 연출 이벤트 신 종료. 초기화 ///인던1 시 추가
	void							ReleaseEventScene();										// 이벤트 신들의 제거 ///인던1 시 추가
	//* 경품 이벤트 관련.
	void							CmdEvent(char *msg);
	void							CreAllFromMsg(char* msg);
	void							DeleteAllPresent();
	void							DrawAllPresent();
	void							ShowPresentText();
	void							ShowAngel();
	void							RenderOj(int xpos, int ypos, int cx, int cy);
	void							CmdEventtt(char *msg);									// Event Test
	// 월드컵 관련.
	void							WorldCupTime(int time);
	void							WorldCupScore(int A_Team, int B_Team);
	void							WorldCupGuildName(char *ch_AGuild, char *ch_BGuild);

	//********************************************************************************************
	//* 쪽지 알림 기능.
	void							CreLetterMsgBox();
	void							DeleteLetterMsgBox();
	void							DrawLetterMsgBox();
	void							isNewLetter();

	//********************************************************************************************
	//* 공성 관련.
	void							DecardiCastleWarLandAttr( int Where, BOOL bPass );		//DEC_TOWER
	void							OnBattle(int nOnBattle);
	void							OpenCastle(int nState);
	void							CloseCastle();
	void							OpenDoor( int nZone, int* nState );						// nate 2004 - 11 : 샤일론의 수호탑, 화이트혼의 외성문이 깨질때 서버로부터 호출
	void							BattleCount(int nSec);
	void							TimeCount(char *guild_name, char *owner_name, int nOwnerSec, char *first_guild_name,
			int nFirstSec, float owner_x, float owner_z, int point);
	void							MatTimeCount( int nHawklawPoint, int nCoolrainPoint,
			int nShiberPoint,int nBattleTime );		// nate 2005-03-11 : Matrix - 메트릭스 서버 팀 스코어, 남은 시간
	void							MatTimeCount( int nGuild1, int nGuild2, int nBattleTime ); // WORLD_MATRIX_2009용 함수
	void							MatBattlePoint( int nBattlePoint);						//배틀 포인트
	BOOL							IsMasterGuild();
	BOOL							IsSubGuild(int m_nFlag);								//외성 성주 체크
	// 공성전 상황 정보
	void							DrawWarCastleInfo();
	void							ToggleWarInfo()
	{
		m_bWarInfo = !m_bWarInfo;
	}
	void							RecalPath();
	// 세금징수 관련 서버메세지 받기..
	void							GetServMsg_WarTax(int, int);
	// 공성전 신청에 관한 메세지 받기
	void							GetServMsg_WarReq(char*, int, int);
	int								IsPortal(D3DVECTOR &vLoc);

	//********************************************************************************************
	//* 길드전, PVP, 길드 관련.
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
	bool							IsInvasionGuildChar(WORD PickColorIndex);		  // 피킹 색상으로 적길드 인지 찾자.

	//********************************************************************************************
	//* 메트릭스 서버.
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
	void							OpenMatrixGate();												//메트릭스 성문 관련(mungmae-03/21)
	void							CreateMatrixMark();

	//********************************************************************************************
	//* 소환관련.
	void							SetSummonLock(BOOL bLock);
	BOOL							CheckSummonToInven( int nDelSlot );							// nate 2004 - 4 : 소환석 퀵슬롯 자동 채우기 기능
	void							SetQuest2StartFlag( BYTE byQuest2Start = 1 )
	{
		m_byQuest2Start = byQuest2Start;
	}

	//********************************************************************************************
	//* 확률 및 기타 계산 관련. Compute.
	int								ComputeCharClass( int nLevel );
	void							ComputeMiss( char*, int );									// nate 2004 - 4 : Crash_Miss
	void							ComputeBlock( char *BlockCharType, int BlockCharIndex,
			char *AttackCharType, int AttackCharIndex ); // Block 효과. ///페어리
	void							TimeCount_Flag(int flagType, char *owner_name, char *guild_name);
	void							CmdProcessRequest(char *player_name);
	void							CmdProcessAccept(int type, char *player_name);
	void							ComputeCombo( int nCharIndex, int nFrameIndex );			// 콤보 출력하기
	void							ComputeEventPoint( int nCharIndex );						// 이벤트포인트(EP)획득시 서버 메세지에 의해 나타남

	//********************************************************************************************
	//* 주 캐릭터 행동 지정 및 그 연관 함수.
	// 가까운 맙 찾기
	int								FindNearTarget(float base_x, float base_z, void *pBaseTarget, float distance,
			float area, bool bMob, int nTargetCount = 0, bool bStoneBash = false ); ///하이콤보
	int								FindLineTargetGuildWar( float LineStartX, float LineStartZ, float LineEndX, float LineEndZ,
			float LineThick, int nTargetCount, int GuildWarType ) ;// [2008/1/18 Theodoric]
	int								FindLineTarget( float LineStartX, float LineStartZ, float LineEndX, float LineEndZ,
			float LineThick, bool bMob, int nTargetCount ); // 두께를 지닌 선과의 충돌 체크 함수 ///하이콤보
	void							HeroMoving();
	// 스킬 상태.
	void							SkillOnOff(int skillTime, BOOL bChar, int index, int skill_index);
	///2003 서브 어택함수
	void							CmdSubAttack(char *msg);
	void							AssemblySuccess(int vnum,int index, int plus, int special);
	void							AssemblyFail(BOOL bfail);
	void							SendAttackMessage(char *Msg);
	bool							MouseClickEvent_NPC(CNkMob *pMob, int which_button);
	BOOL							IsRohSkill( int nSkill_Idx );										// nate 2005-06-17 : 주캐릭 스킬인지 판별
	BOOL							AutoItemTake( float itemTakeRange );								// [4/12/2007 Theodoric] 아이템 자동 집기
	void							SpeedUpEffect(CNkCharacter *pCha, BOOL bVisual);					// 일반 이펙트(스킬 이펙트아님)를 위해
	void							CmdHelperRequest(char *player_name); 								//후견인
	// 스피릿 모드.
	void							SpiritModeEffect(CNkCharacter *pCha, BOOL bVisual);					// 스피릿 모드 이펙트
	void							SpiritPoint(int nGet_Thousand, int nGet_Hundred, int nGet_Ten, int nGet_One,
			int nTaget_Hundred, int nTaget_Ten, int nTaget_One);
	void							SpiritTime(int nRemainTime);
	void							CmdSumPart(int index,int mobIndex);									//중고렙스킬 추가 서먼피션
	void							SetPickColor( WORD wcolor )
	{
		m_wColor = wcolor;
	}

	//********************************************************************************************
	//* 기타 함수.
	// 패키지 구입여부 처리 ( 대만 )
	void							TwPackage( int nDate );
	void							PKItemDrop(int nInvenNum,int nSlotX,int nSlotY,int nWidth,int nHeight);	//pk시 아이템 드롭(중국만 적용)
	void							SetBRFalg(CItem *pItem, int flag);
	void							RandomCameraRotate();


	BOOL IsTeleportArea(float wx, float wz, float roh_wx, float roh_wz); // 에이디아 테레포트 못하는 지역
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

	int  Check_Skill_Resurrection(int skill_Index, CNkCharacter *pCha ); // [5/29/2008 반재승]  에이디아 부활 스킬 관련 체킹 함수.

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
		printf("해당 몬스터를 찾지 못했습니다!!!\n");
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