#ifndef _CONTROL_BOTTOM_H_
#define _CONTROL_BOTTOM_H_

#include "Config.h"

#include "BasicButton.h"
#include "BasicStatic.h"
#include "TextBox.h"
#include "TextOutBox.h"
#include "NkCharacter.h"
#include "R_SkillBox.h" // ������ ��ų  Ŭ����
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


// ��ų ����Ű�� ���� ����ü
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

	int m_iYellowCard;//ä�� ����Ʈ ������ Ƚ��..
	HWND  m_hWnd;			// ���� Ŭ���̾�Ʈ ������ ���Ѱ� �˾ƺ��� ����..
	RECT m_ClientRc;
	float m_fStretchX, m_fStretchY;
	int	  m_CurClientMode;

	CSurface*	m_pBottom;
	CR_SkillBox		m_R_Skill;
	CL_SkillBox		m_L_Skill;

	int m_CB_Xpos, m_CB_Ypos;
	int m_IB_Xpos, m_IB_Ypos;

	CSurface* m_pVitalEg;			// ����� ������ �̹���
	CSurface* m_pEVitalEg;			// ���� ������ �̹��� 
	CSurface* m_pManaEg;			// ������ ������ �̹���
	CSurface* m_pStaminaEg;			// ������ ������ �̹���
	CSurface* m_pEpowerEg;			// ����� ������ �̹���


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

	int m_Activebox_X, m_Activebox_Y;   // ü�� ����Ʈ �ڽ� (��ü����) ��ǥ		
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
	// ä�� �ƿ�ǲ �ɼ� ��
	float ChatOutOpt_PlusY; // ä�þƿ� �ɼ� ����� y��ǥ ������. �� ���������� �����ؾ��ؼ� float������ ��.
	CLoopButton m_ChatOutOpt_View; // ä�� �ƿ�ǲ �ɼ��� ���̴��� �������� ����.

	CSurface*     m_pChatOutOpt_Back; // �� ��� �Ƕ���.
	CToggleButton m_ChatOutOpt_All;   // ��ü ä�� ���̱�
	CToggleButton m_ChatOutOpt_Normal; // �Ϲ� ä�� ���̱�
	CToggleButton m_ChatOutOpt_Party; // ��Ƽ ä�� ���̱�
	CToggleButton m_ChatOutOpt_Guild; // ��� ä�� ���̱�
	CToggleButton m_ChatOutOpt_Shout; // ��ħ ä�� ���̱�
	CToggleButton m_ChatOutOpt_MasterMsg; // ���� ä�� ���̱�
	CToggleButton m_ChatOutOpt_BalloonOnOff; // �ڽ� �Ӹ����� �ߴ� ǳ���� On/Off ��� ��ư.
	///////////////////////////////////////////////////////
	// ä�� ��ǲ �ɼǿ�
	CLoopButton m_ChatInOpt_LoopBtn; // ä�� �Է½� �ڵ����� ��Ƽ, ��� ���� �������ִ� ��ư.
	///////////////////////////////////////////////////////
	BOOL		  m_bSalesView;
	int			  m_nRepSales;
	CBasicButton  m_SocialBtn;
	CSurface*     m_pChaMode_S; // �ɸ��� ��ȭ���
	CSurface*     m_pChaMode_C; // �ɸ��� �������
	CSurface*     m_pNeedExpBar; // ���������� ���� ����Ʈ bar
	//----------------- KEYBOARD INPUT BOX
	CTextBox m_InputBox;
	//-----------------LIST BOX
	CQlistBox* m_QlistBox;
	CQlistBox* m_QlistBox_Shout;

	char m_TmpBuf[128];			// Ű���� �Է� ��Ʈ���� �����ؼ� ��Ƶд�
	BOOL m_bAct;				// ����Ʈ �ڽ��� ��ũ�� ��뿩���� ����
	BOOL m_bMoadChange;			// ���� ��尡 ����Ǿ��°�??

	char			m_HistoryBuf[MAX_NUM_HISTORYBUF][128];	// �Ӹ� �����丮
	int				history_cnt;
	int				historydraw_cnt;

	CNkCharacter	*m_pNkCha;

	int				m_ScreenWidth;
	int				m_ScreenHeight;

	CAlphaPopUp		*m_pEtcInfo;
	char			m_StrTemp[128];

	CTextOutBox		m_QuickCount[QUICK_SLOT_NUM];
	CTextOutBox		m_QuickCount2[QUICK_SLOT_NUM]; // Ƚ�� ���� ������ ���� Ƚ�� ǥ�ÿ�.
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
	void DibStr(char *str, int len, DWORD color, CQlistBox* pListBox); // �ؽ�Ʈ �ٳѱ��

	void RandomHelp();
	void SetFocus();
	BOOL HasFocus() 
	{ 
		return m_InputBox.GetEnable(); 
	};	// ��Ʈ�� ������ ��Ŀ���� ������ �ִ°��� Ȯ���Ѵ�.
	void DrawText(char*);
	void CreatEnergyImage();	//--------------------ENERGY	

	BOOL	IsInside(int x, int y);		// ä�ùڽ��� ������ üũ�Ѵ�	
	BOOL    IsInside_Prompt(int x, int y) ; // text �Է� �ڽ������� üũ�Ѵ�
	BOOL    IsInside_Tmp(CSurface* surface, int x, int y) ;
	BOOL	IsChatWide();				// ä�ø���Ʈ�� ���� ȭ�鿡 ���� �ִ°�?
	BOOL	IsInsideQuickSlot(int x, int y);			// ������ ���� üũ
	int		CheckQuickSlotNumber(int x, int y);			// �����Թ�ȣ�� ���� by.Sol9

	void SetNkCha(CNkCharacter *pNkCha);

	void DrawChatBox(int xpos, int ypos);

	void DrawAllBtn(CBasicButton* surface, int xpos, int ypos);
	void DrawAllEg(CSurface* surface, int xpos, int ypos, int cur_energy, int max_energy);
	void DrawInputBox(CTextBox* surface, int xpos, int ypos);
	void DrawQlistBox(CQlistBox* surface, int xpos, int ypos);



	void DrawQuickItem(POINT &pt, CItem **ppOverItem);
	void DrawFunctionList();
	void DrawEtcInfo(POINT &pt);

	// ��ġ ���� ����. - ����.
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

	// ��ų ����Ű������ �Լ�
	int CheckQuickKeyState(int); // ������ ���� �Ǵ��Ѵ�	
	void ActionQuickKey(int);     // m_QuickKeyStat�� �������� ����� �����Ѵ� 
	void QuickKeyLock();          // ����Ű �����ϱ� �Լ�
	void QuickKeyUnLock();        // ������� ����Ű �����ϱ�
	void QuickKeyCall();          // ������� ����Ű ȣ���ϱ�	
	void DrawFunctionKey();       // �ؽ�Ʈ �׷��ֱ�

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

	void SetFramePos( int Case ); // ������ ���� ��ǥ�� �ٲ��� �� ��� ���⼭ ó��������.( by ���� )
	void SetChangeVal(int);       // ä�ð� ���õ� ��ġ���� ����.(�ٲٱ⸦ �����ø� ����ǰ� ���� by ����)
	//------------�弳 ���͸� ����	
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

	// �Ӹ��ź� ��ƾ
	CNoWhisper	*m_pNoWhisper;
	BOOL  CheckNoWhisper(char *msg);
	CNoWhisper  *GetNoWhisper() 
	{ 
		return m_pNoWhisper; 
	};

	// ��� ��ɾ� ��ƾ.
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
	BOOL m_bViewFunctionList; //����Ű ����Ʈ ���̴��� ������...vŰ�� ���
	BOOL m_bDrawQuick; //������ ǳ���� �׸����� ������...

	char m_szSenderLIstBuf[MAX_NUM_HISTORYBUF][64];	// nate 2006-03-10 : �Ӹ� �۽��� ���
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