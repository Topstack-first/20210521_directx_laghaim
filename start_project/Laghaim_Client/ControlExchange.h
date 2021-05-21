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

#define SIZE_OF_SKILLMINICON 18 // �絵�� ǥ�õǴ� ���� ��ų �������� ������.

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

	// �ŷ�â�� ���̴� ���.
	void SetVisibleMode(BOOL bExch);		// �ŷ����ΰ� �ƴϸ� �Ϲ� ����ΰ�.

	// ������ ����.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	CSurface *m_pSurfaceBack;
	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;
	CSurface *m_pPetInfo; //���� ���� surface

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

	//----���ο� ������
	int m_opp_slotstart_x, m_opp_slotstart_y;
	int m_my_slotstart_x, m_my_slotstart_y;

	// �ŷ� ������� �ӽ� �κ�.
	CItem	*m_ExgOppInven;
	int		m_ExgOppSlot[EXG_SLOT_X_NUM][EXG_SLOT_Y_NUM];

	void	ExchInvenTo(int x, int y, int index, int vnum, int gem_num, int special, int special2,
						int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur ); // ����. ///�Ź���ȭ�ý���
	int		GetExgOppSlotIndex(int x, int y);
	int		GetItemIndexInOppSlot(int x, int y, int w, int h);
	CItem*	PickItemInOppSlot(int index, BOOL isVnum=FALSE);
	void	ExchInvenFrom(int x, int y, int w = 1, int h = 1);						// ����.

	void	RemoveAllOppInven();

	// �ŷ��� ���Ǵ� ������
	char  m_strPetName[30];// �ֿϵ��� �̸�
	char  m_PetLevel[15] ;  // ����
	char  m_PetHP[15];     // �����
	char  m_PetAttack[15] ; // ���ݷ�
	char  m_PetDefense[15]; // ����

	BOOL	m_bExch;
	BOOL	m_bDisable;
	BOOL	m_bTrade;
	BOOL	m_bOppTrade;
	bool    m_bMyPetExchNote; // �� ���Կ� ���� �絵 ���� ���� ����
	int     m_cntItem; // �� ���Կ� �ִ� ������ ����
	bool    m_bOppPetExchNote; // ����� ���Կ� ���� ��ȯ���� ���翩��
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

	int         m_PetClass; // ���� ��ȭ Ÿ��.
	CTextOutBox *m_pPetClassInfoText; // ������ ��ġ�Ⱑ ���� ���̰��� �׳� ���� ��.
	int         m_nPetClassTextX,m_nPetClassTextY;
	int         m_PetSkillLevelInfo[4]; // ���� ��ų ���� ����.
	CBasicButton *m_pSkillIcon[12]; // ��ų�� ��������� ������. ��ų ���� ��ŭ ������ �ִ´�. // �絵 ���� �� ��ų�� �߰��ǵ� �ڵ����� ó���� ���ߴ�. �� ��ų�� �߰��Ǹ� �밡�ٷ� �÷�����.
	CAlphaPopUp		*m_pSkillIconName; // ��ų ���� ǥ�����ֱ� ���� �˾�.


	// �ŷ��� ���õǴ� �Լ���.
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
	void    ExchPetOppInfoDraw(); // ���� ���� ����Լ�

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
