#ifndef _CONTROL_BOTTOM_H_
#define _CONTROL_BOTTOM_H_

#include "Config.h"

#include "BasicButton.h"
#include "BasicStatic.h"
#include "TextBox.h"
#include "TextOutBox.h"
#include "NkCharacter.h"
#include "R_SkillBox.h" // 오른쪽 스킬  클레스
#include "L_SkillBox.h"
#include "Skill.h"
#include "msgpopup.h"

#include "ExtraButton.h"

enum GageView_Type
{
	GageView_Type__Vital = 0,
	GageView_Type__EVital,

	GageView_Type__Stamina = 0,
	GageView_Type__Mana ,
	GageView_Type__EPower,
};

#define QUICK_SLOT_X_SIZE	(34)
#define QUICK_SLOT_Y_SIZE	(33)
#define	QUICK_SLOT_WIDTH	(QUICK_SLOT_X_SIZE * QUICK_SLOT_X_NUM)
#define QUICK_SLOT_HEIGHT	(QUICK_SLOT_Y_SIZE * QUICK_SLOT_Y_NUM)

#define HELTH_BAR_X		(370 )//* m_ScreenWidth / BASE_X_SIZE)
#define MANA_BAR_X		(385)// * m_ScreenWidth / BASE_X_SIZE)
#define STAMINA_BAR_X	(610)// * m_ScreenWidth / BASE_X_SIZE)
#define EPOWER_BAR_X	(625)// * m_ScreenWidth / BASE_X_SIZE)

#define BAR_Y			(402)// * m_ScreenHeight / BASE_Y_SIZE)
#define BAR_WIDTH		(15 )//* m_ScreenWidth / BASE_X_SIZE)
#define BAR_HEIGHT		(73)// * m_ScreenHeight / BASE_Y_SIZE)

#define MAX_NUM_HISTORYBUF 10

class CQlistBox;
class CTcpIpConnection;
class CItemRender;
class CNkCharacter;
class CR_SkillBox;
class CL_SkillBox;
class CAlphaPopUp;
class CControlHelp;
class CScrollPopUp;
class CNoWhisper;
class CMsgPopUp;

#define IDS_CTR_BOTTOM_CNT		7
#define CTR_HELP_DLL_RC_NUM     23


#define MAX_LEVEL				250

#define INFO_FILE_NUM			3


// 스킬 단축키에 대한 구조체
typedef struct 
{
	int type;
	int skill;
} QUICK_SKILL;


class CControlBottom
{
	typedef void (CControlBottom::*CMD_CHAT_PROC)(char*);
	typedef std::map<std::string, CMD_CHAT_PROC> MAP_CHAT_PROC;
public:

	int m_iYellowCard;//채팅 에디트 도배경고 횟수..
	HWND  m_hWnd;			// 현재 클라이언트 영역이 얼만한가 알아보기 위한..
	RECT m_ClientRc;
	float m_fStretchX, m_fStretchY;
	int	  m_CurClientMode;

	CSurface*	m_pBottom;
	CR_SkillBox		m_R_Skill;
	CL_SkillBox		m_L_Skill;

	int m_CB_Xpos, m_CB_Ypos;
	int m_IB_Xpos, m_IB_Ypos;

	CSurface* m_pVitalEg;			// 생명력 에너지 이미지
	CSurface* m_pEVitalEg;			// 누적 데미지 이미지 
	CSurface* m_pManaEg;			// 마법력 에너지 이미지
	CSurface* m_pStaminaEg;			// 지구력 에너지 이미지
	CSurface* m_pEpowerEg;			// 전기력 에너지 이미지


	CBasicStatic	m_expbar;
	CBasicStatic	m_hpbar;
	CBasicStatic	m_mpbar;
	CBasicStatic	m_elecbar;
	CBasicStatic	m_staminabar;

	CBasicStatic	m_combat_state;


	QUICK_SKILL  m_QuickSkill[FUNCTION_NUM];
	CTextOutBox  m_FunctionKey[FUNCTION_NUM];
	CTextOutBox  m_FunctionList[FUNCTION_NUM];

	int m_VitalEg_Xpos,		m_VitalEg_Ypos ;
	int m_ManaEg_Xpos,		m_ManaEg_Ypos;
	int m_StaminaEg_Xpos,	m_StaminaEg_Ypos;
	int m_EpowerEg_Xpos,	m_EpowerEg_Ypos;

	int m_Activebox_X, m_Activebox_Y;   // 체팅 리스트 박스 (전체보기) 좌표		
	int	m_ModeState;
	BOOL	m_bViewCntChange;
	int _StartCnt, _LastCnt;
	int m_CurTextView;

	CBasicButton  m_SalesOn;
	CBasicButton  m_btn_Letter;
	CBasicButton  m_btn_NewLetter;
	CBasicButton  m_btn_Yutnori;
	CBasicButton  m_btn_Fishing;
	CBasicButton  m_btn_Lotto;
	CBasicButton  m_btn_Bingo;
	CBasicButton  m_btn_Baseball;
	CBasicButton  m_btn_Lottery;
	CBasicButton  m_btn_DiceGame;

	CBasicStatic  m_imgLetterNew;

	//////////////////////////////////////////////////
	// 채팅 아웃풋 옵션 용
	float ChatOutOpt_PlusY; // 채팅아웃 옵션 숨기기 y좌표 조정용. 좀 세부적으로 조정해야해서 float형으로 함.
	CLoopButton m_ChatOutOpt_View; // 채팅 아웃풋 옵션을 보이느냐 마느냐의 여부.

	CSurface*     m_pChatOutOpt_Back; // 뒤 배경 판때기.
	CToggleButton m_ChatOutOpt_All;   // 전체 채팅 보이기
	CToggleButton m_ChatOutOpt_Normal; // 일반 채팅 보이기
	CToggleButton m_ChatOutOpt_Party; // 파티 채팅 보이기
	CToggleButton m_ChatOutOpt_Guild; // 길드 채팅 보이기
	CToggleButton m_ChatOutOpt_Shout; // 외침 채팅 보이기
	CToggleButton m_ChatOutOpt_MasterMsg; // 공지 채팅 보이기
	CToggleButton m_ChatOutOpt_BalloonOnOff; // 자신 머리위에 뜨는 풍선말 On/Off 토글 버튼.
	///////////////////////////////////////////////////////
	// 채팅 인풋 옵션용
	CLoopButton m_ChatInOpt_LoopBtn; // 채팅 입력시 자동으로 파티, 길드 등을 셋팅해주는 버튼.
	///////////////////////////////////////////////////////
	BOOL		  m_bSalesView;
	int			  m_nRepSales;
	CBasicButton  m_SocialBtn;
	CSurface*     m_pChaMode_S; // 케릭터 평화모드
	CSurface*     m_pChaMode_C; // 케릭터 전투모드
	CSurface*     m_pNeedExpBar; // 레벨업까지 남은 포인트 bar
	//----------------- KEYBOARD INPUT BOX
	CTextBox m_InputBox;
	//-----------------LIST BOX
	CQlistBox* m_QlistBox;
	CQlistBox* m_QlistBox_Shout;

	char m_TmpBuf[128];			// 키보드 입력 스트링을 복사해서 담아둔다
	BOOL m_bAct;				// 리스트 박스의 스크롤 사용여부의 제한
	BOOL m_bMoadChange;			// 현재 모드가 변경되었는가??

	char			m_HistoryBuf[MAX_NUM_HISTORYBUF][128];	// 귓말 히스토리
	int				history_cnt;
	int				historydraw_cnt;

	CNkCharacter	*m_pNkCha;

	int				m_ScreenWidth;
	int				m_ScreenHeight;

	CAlphaPopUp		*m_pEtcInfo;
	char			m_StrTemp[128];

	CTextOutBox		m_QuickCount[QUICK_SLOT_NUM];
	CTextOutBox		m_QuickCount2[QUICK_SLOT_NUM]; // 횟수 제한 물약의 남은 횟수 표시용.
	char			m_commOut[512];

public:

	CControlBottom();
	virtual ~CControlBottom();

	HRESULT RestoreSurfaces();
	void Init();
	void LoadRes();
	void DivLoadRes();
	void DeleteRes();
	int Draw(CItem **ppOverItem);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void AddStrQlist(char *str, int say_type = 0);
	void AddStrShoutList(char* str);
	void AddStrQlist_ExtraInfo(char* str, int say_type = 0,  int infoType = 0, int infoVnum = 0);
	void DibStr(char *str, int len, DWORD color, CQlistBox* pListBox); // 텍스트 줄넘기기

	void RandomHelp();
	void SetFocus();
	BOOL HasFocus() 
	{ 
		return m_InputBox.GetEnable(); 
	};	// 컨트롤 바텀이 포커스를 가지고 있는가를 확인한다.
	void DrawText(char*);
	void CreatEnergyImage();	//--------------------ENERGY	

	BOOL	IsInside(int x, int y);		// 채팅박스의 영역을 체크한다	
	BOOL    IsInside_Prompt(int x, int y) ; // text 입력 박스영역을 체크한다
	BOOL    IsInside_Tmp(CSurface* surface, int x, int y) ;
	BOOL	IsChatWide();				// 채팅리스트가 게임 화면에 나와 있는가?
	BOOL	IsInsideQuickSlot(int x, int y);			// 퀵슬롯 영역 체크
	int		CheckQuickSlotNumber(int x, int y);			// 퀵슬롯번호를 리턴 by.Sol9

	void SetNkCha(CNkCharacter *pNkCha);

	void DrawChatBox(int xpos, int ypos);

	void DrawAllBtn(CBasicButton* surface, int xpos, int ypos);
	void DrawAllEg(CSurface* surface, int xpos, int ypos, int cur_energy, int max_energy);
	void DrawInputBox(CTextBox* surface, int xpos, int ypos);
	void DrawQlistBox(CQlistBox* surface, int xpos, int ypos);



	void DrawQuickItem(POINT &pt, CItem **ppOverItem);
	void DrawFunctionList();
	void DrawEtcInfo(POINT &pt);

	// 위치 정보 리턴. - 정원.
	DWORD GetRefHeight();

	void ResetInfo(int info_num);

	CR_SkillBox* GetRSkill() 
	{ 
		return &m_R_Skill; 
	}
	CL_SkillBox* GetLSkill() 
	{ 
		return &m_L_Skill; 
	}
	BOOL SelectedRotate() 
	{ 
		return (m_R_Skill.GetCurSkill() == SELECTED_SKILL_ROTATE); 
	}

	// 스킬 단축키에대한 함수
	int CheckQuickKeyState(int); // 어떤기능을 할지 판단한다	
	void ActionQuickKey(int);     // m_QuickKeyStat의 값에따라서 기능을 수행한다 
	void QuickKeyLock();          // 단축키 지정하기 함수
	void QuickKeyUnLock();        // 지정됬던 단축키 해제하기
	void QuickKeyCall();          // 지정됬던 단축키 호출하기	
	void DrawFunctionKey();       // 텍스트 그려주기

	QUICK_SKILL* GetQuickSkill() 
	{
		return m_QuickSkill;
	}
	CTextOutBox* GetKeyText() 
	{ 
		return m_FunctionKey;
	}

	int  m_DownKey;
	int  m_CallRskillNum;
	int  m_CallLskillNum;

	void SetQuickCount(int count);
	void SetQuickNoCount(int count);//1001

	void SetFramePos( int Case ); // 프레임 별로 좌표가 바뀌어야 할 경우 여기서 처리해주자.( by 원석 )
	void SetChangeVal(int);       // 채팅과 관련된 수치들을 셋팅.(바꾸기를 했을시만 실행되게 변경 by 원석)
	//------------욕설 필터링 관련	
	void SendChat();

	int m_QuicSlot_X;
	int m_QuicSlot_Y;

	int m_bar_y;
	int m_helth_bar_x, m_mana_bar_x, m_stamina_bar_x, m_epower_bar_x;

	// display !! Exp progress bar 
	BOOL  m_bLevelUp;
	int   m_OldNeedExp;
	int   m_ExpBar_Cx, m_ExpBar_Cy;
	__int64   m_NeedTotalExp;
	int   a_LevelupExp[MAX_LEVEL];
	void  InitExp();
	int   GetLevelupExp(int level);
	void  DrawNeedExpBar();

	int		m_iCurQuickSlotNum;
	int     GetCurQuickSlotNum()
	{
		return m_iCurQuickSlotNum;
	}

	// 귓말거부 루틴
	CNoWhisper	*m_pNoWhisper;
	BOOL  CheckNoWhisper(char *msg);
	CNoWhisper  *GetNoWhisper() 
	{ 
		return m_pNoWhisper; 
	};

	// 장사 명령어 루틴.
	char  m_strSalesCmd[256];
	BOOL	m_bRepSalesCmd;
	int	  CheckSalesCmd(char *strCmd);
	void  SendSalesCmd();
		
	BOOL m_bF1Pressed;
	BOOL m_bF2Pressed;
	BOOL m_bF3Pressed;
	BOOL m_bF4Pressed;
	BOOL m_bF5Pressed;
	BOOL m_bF6Pressed;
	BOOL m_bF7Pressed;
	BOOL m_bF8Pressed;
	BOOL m_bViewFunctionList; //단축키 리스트 보이느냐 마느냐...v키로 토글
	BOOL m_bDrawQuick; //퀵슬롯 풍선말 그리느냐 마느냐...

	char m_szSenderLIstBuf[MAX_NUM_HISTORYBUF][64];	// nate 2006-03-10 : 귓말 송신자 목록
	void InsertSender( char* szSender );



public:
	int m_lcurrent_view;
	int m_rcurrent_view;

	DWORD m_hide_tick;
	bool m_chat_hidden_list;
	bool m_chat_hidden_bottom;

public:
	void LoadRes_Bottom();
	void DeleteRes_Bottom();
	void Draw_Bottom();

	void LoadRes_Chatting();
	void DeleteRes_Chatting();
	void Draw_Chatting();

	void LostFocus();

	void SetPosAll();

	void NewLetter(bool visible);

public:
	int m_chat_view_value;

	CBasicButton m_chat_size_big;
	CBasicButton m_chat_size_small;

private:
	bool CheckAndMsg(char * buf, const char * const msg, const char * const check);
	bool CheckAndMsg(char * buf, const char * const msg, const int num, const char ** check);

	void RegistChatProc();
	void ProcChat(char* chat);

	void ProcChat_Point(char* chat);
	void ProcChat_Exchange(char* chat);
	void ProcChat_NoChat(char* chat);
	void ProcChat_Sales(char* chat);
	void ProcChat_FreePk(char* chat);
	void ProcChat_Refine(char* chat);
	void ProcChat_Peace(char* chat);
	void ProcChat_Watching(char* chat);
	void ProcChat_AddFriend(char* chat);
	void ProcChat_DelFriend(char* chat);
	void ProcChat_MultiPoint(char* chat);
	void ProcChat_ControlTower(char* chat);
	void ProcChat_SelfRefine(char* chat);
	void ProcChat_CurrentTime(char* chat);
	void ProcChat_ServerName(char* chat);
	void ProcChat_SkyMap(char* chat);
	void ProcChat_GoldConvert(char* chat);

private:
	MAP_CHAT_PROC m_mapChatProc;
};

#endif // _CONTROL_BOTTOM_H_