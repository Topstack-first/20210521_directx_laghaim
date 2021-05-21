#ifndef _CONTROL_EXCHANGE_H_
#define _CONTROL_EXCHANGE_H_

#include "Config.h"

#include "BasicButton.h"
#include "TextOutBox.h"
#include "AlphaPopup.h"
#include "NkCharacter.h"

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CMsgPopUp;
class CTextOutBox;

#define SIZE_OF_SKILLMINICON 18 // 양도시 표시되는 작은 스킬 아이콘의 사이즈.

class CControlExchange//:public CTransStrFormat
{
public:

	void ExchPetEnd();
	void ExchangedPetClear();

	HWND	m_hWnd;
	RECT	m_ClientRc;

	CControlExchange();
	virtual ~CControlExchange();

	void Init();
	void LoadRes();
	void DeleteRes();
	CItem* Draw();
	void DrawMySlot();
	void DrawOppSlot();
	void DrawItemInfo();
	//void ResetItemInfo() { m_pItemInfo->SetIndex(-1); }
	void SetNkCha(CNkCharacter *pNkCha);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void SendMsg();
	BOOL CheckMoneyStr(char* szMoney);

	// 거래창이 보이는 모습.
	void SetVisibleMode(BOOL bExch);		// 거래중인가 아니면 일반 모드인가.

	// 아이템 관련.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	CSurface *m_pSurfaceBack;
	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;
	CSurface *m_pPetInfo; //라비용 정보 surface

	CNkCharacter *m_pNkCha;
	CItem *m_pOverItem;

	CBasicButton m_CancelBtn;
	CBasicButton m_ExgBtn1;
	CBasicButton m_DisableBtn1;
	CBasicButton m_MoneyBtn1;
	CBasicButton m_ExgBtn2;
	CBasicButton m_DisableBtn2;
	CBasicButton m_MoneyBtn2;

	__int64 m_ExchMoney;

	char m_MsgBuf[256];

	int m_ScreenWidth;
	int m_ScreenHeight;

	//CAlphaPopUp *m_pItemInfo;
	CTextOutBox m_MoneyText1, m_MoneyText2, m_MoneyTextKor1, m_MoneyTextKor2;
	CTextOutBox m_MyMoneyText;

	//----새로운 변수들
	int m_opp_slotstart_x, m_opp_slotstart_y;
	int m_my_slotstart_x, m_my_slotstart_y;

	// 거래 상대자의 임시 인벤.
	CItem	*m_ExgOppInven;
	int		m_ExgOppSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];

	void	ExchInvenTo(int x, int y, int index, int vnum, int gem_num, int special, int special2,
						int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur ); // 삽입. ///신방어구강화시스템
	int		GetExgOppSlotIndex(int x, int y);
	int		GetItemIndexInOppSlot(int x, int y, int w, int h);
	CItem*	PickItemInOppSlot(int index, BOOL isVnum=FALSE);
	void	ExchInvenFrom(int x, int y, int w = 1, int h = 1);						// 삭제.

	void	RemoveAllOppInven();

	// 거래에 사용되는 변수들
	char  m_strPetName[30];// 애완동물 이름
	char  m_PetLevel[15] ;  // 레벨
	char  m_PetHP[15];     // 생명력
	char  m_PetAttack[15] ; // 공격력
	char  m_PetDefense[15]; // 방어력

	BOOL	m_bExch;
	BOOL	m_bDisable;
	BOOL	m_bTrade;
	BOOL	m_bOppTrade;
	bool    m_bMyPetExchNote; // 내 슬롯에 라비용 양도 문서 존재 여부
	int     m_cntItem; // 내 슬롯에 있는 아템의 개수
	bool    m_bOppPetExchNote; // 상대의 슬롯에 라비용 교환문서 존재여부
	bool    m_bPetInfoDraw;

	char		m_strMoney1[12], m_strMoney2[12];
	char		m_strOppName[40];
	char		m_strOppLevel[40];
	CTextOutBox *m_pOppNameBox;
	CTextOutBox *m_pOppLevelBox;
	int			m_nOppNameX, m_nOppNameY;
	int			m_nOppLevelX, m_nOppLevelY;
	CTextOutBox *m_pPetInfoText[5];
	int         m_nPetTextX[5],m_nPetTextY[5];

	int         m_PetClass; // 펫의 진화 타입.
	CTextOutBox *m_pPetClassInfoText; // 위에다 합치기가 손이 많이가서 그냥 따로 씀.
	int         m_nPetClassTextX,m_nPetClassTextY;
	int         m_PetSkillLevelInfo[4]; // 펫의 스킬 습득 레벨.
	CBasicButton *m_pSkillIcon[12]; // 스킬을 배웠는지의 아이콘. 스킬 갯수 만큼 가지고 있는다. // 양도 쪽은 펫 스킬이 추가되도 자동으로 처리를 못했다. 펫 스킬이 추가되면 노가다로 늘려주자.
	CAlphaPopUp		*m_pSkillIconName; // 스킬 명을 표시해주기 위한 팝업.


	// 거래에 관련되는 함수들.
	BOOL	IsExch()
	{
		return m_bExch;
	};
	void	ActivateExch(BOOL bExch = FALSE);
	void	ExchDisable();
	void	ExchTrade();
	void	ExchChange();
	void	ExchCancel(BOOL bSend = FALSE);
	void	ExchMoney(char *strMoney);
	void	SetOppName(char *player_name);
	void	SetOppLevel(int nPlayer_level);
	void    SetExchPetInfo(char *PetName, char* PetLevel, char* PetHP, char* PetAttack, char* PetDefense, int PetClass=0, int Skill1_Level=0, int Skill2_Level=0, int Skill3_Level=0, int Skill4_Level=0 );
	void    ExchPetOppInfoDraw(); // 라비용 정보 출력함수

//====================================================
	// nate 2004 - 4 : exchange
	BYTE	m_byExchLock;	// 0 : false, 1 : true
	void	SetExchLock( BYTE byExchLock = 1 );
	DWORD	m_timeExchLock;
//====================================================
	void	AskExchange(char *player_name);
	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);

	HRESULT RestoreSurfaces();

	DWORD m_dwMouseClickTickTime;
};

#endif // _CONTROL_EXCHANGE_H_
