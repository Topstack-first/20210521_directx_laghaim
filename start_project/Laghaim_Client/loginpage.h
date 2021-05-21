#ifndef _LOGINPAGE_H_
#define _LOGINPAGE_H_

#include "Config.h"


#include "headers.h"
#include "BasicButton.h"
#include "listbox.h"
#include "TextBox.h"
#include "MsgPopUp.h"
#include "textoutbox.h"
#include "BasicStatic.h"




#define LOGIN_BTN_X			446
#define LOGIN_BTN_Y			200
#define LOGIN_BTN_WIDTH		96
#define LOGIN_BTN_HEIGHT	39
#define LOGIN_BTN_GAP		60

#define LOGIN_EDIT_X		45
#define LOGIN_EDIT_Y		13
#define LOGIN_PW_EDIT_X		45
#define LOGIN_PW_EDIT_Y		37
#define LOGIN_EDIT_GAP		48
#define LOGIN_EDIT_SIZE		11  // ���̴� ���� ���̼�(��ũ�� �Ǵ� ������ �� �Էµȴ�.)(2����� �Էµ�.) ///2007�ε����� �� �н����嵵 ��ũ���� �ǰ� �����Ͽ���.
//ä�� ����Ʈ ����
#define LOGIN_NAME_LENGTH	31  // ���� �Է� ���� ���ڼ��� �ƴϴ���...LastId�� ���� ����.
#define LOGIN_EDIT_FCOLOR	RGB(255, 179, 0)

#define LOGIN_LIST_X		24
#define LOGIN_LIST_Y		150
#define LOGIN_LIST_WIDTH	160
#define LOGIN_LIST_HEIGHT	163
#define LOGIN_LIST_FCOLOR	RGB(148, 134, 99)


#define LOING_BACK_X 7 // �������̽� ������ġ
#define LOING_BACK_Y 220 // �������̽� ������ġ
#define LOING_BTN_ANI_START_X -200 // �������̽� �ִϸ��̼��� ���۵Ǵ� ��ġ.
#define LOING_BTN_ANI_TIME    1000 // �������̽� �ִϸ��̼��� �÷��̵Ǵ� �ð�.


#	define KOREA_IP_2012	"101.79.53."
#	define KOREA_IP			"119.205.215."

#define KOREA_INTERNAL_IP	"61.104.44."
#define KOREA_INTERNAL_IP2	"10.1.90." // by evilkiki 2010.03.15. ȸ������ : ���� CŬ����
#define KOREA_INTERNAL_IP3	"10.1.50." // by Sol9 [2012.08.31] �ڸ��̵�, ip����
#define KOREA_INTERNAL_IP4	"10.1.110." // by evilkiki 2011.02.17. ����� �׽�Ʈ
#define KOREA_INTERNAL_IP5	"10.1.30." // by evilkiki 2011.02.17. ����� �׽�Ʈ


#define JAPAN_IP	"219.105.50."
#define JAPAN_IP_2	"122.208.74."	// [1/29/2008 �����] �Ϻ� IP �߰�
#define TAIWAN_IP	"210.208.90."
#define HONGKONG_IP	"202.123.175."
#define CHINA_BJ_IP_1 "211.151.1."
#define CHINA_BJ_IP_2 "211.151.0."
#define CHINA_SH_IP_1 "61.172.244."
#define CHINA_SH_IP_2 "61.129.255."
#define CHINA_SH_IP_3 "61.152.168."
#define CHINA_CD_IP_1 "218.6.175."
#define CHINA_CD_IP_2 "218.6.170."
#define CHINA_BJ_TEST "219.238.187."
#define CHINA_BJ_IP_NEW "221.236.25."//�ϰ� IDC����
#define CHINA_BJ_IP_NEW1 "218.7.132."//�ϰ� ��������
#define CHINA_BJ_IP_NEW2 "211.151.239."//�ϰ� IDC�������� ���� �߰� // 05-08-20 ���� ///�߱�IP�߰�
#define CHINA_BJ_IP_NEW3 "255.255.255."//�ϰ� IDC�������� ���� �߰� // 05-08-20 ���� ///�߱�IP�߰�
#define CHINA_BJ_IP_NEW4 "61.182.161."//�ϰ� IDC���� // 06-03-02 ���� 
#define CHINA_BJ_IP_NEW5 "222.66.84."//�ϰ� IDC���� // 06-03-23 ���� 
#define INI_IP		  "202.10.33."
#define DEU_IP		  "194.9.11." // 06-08-22 ���� ���� �����.
#define USA_IP		  "66.129.108."
#define USA_IP_NEW  "216.27.13."//�̱� ���� IDC����
#define FRANCE_IP		"62.26.131."
#define BRASIL_IP_TEST	"201.77.211." // 07-08-31 ����� ���� IDC�������� ���Ͽ� ������.
#define CHINA_BJ_IP_NEW6 "124.42.77."
#define CHINA_BJ_IP_NEW7 "211.101.182."
#define GSP_TEST_IP				"93.90.190."

class CLoginPage;
class CSheet;
class CMatrixServ;
class CTextOutBox;



class CMatrixServ
{
public:
	CMatrixServ * pNext;
	CTextOutBox * pTextOut;
	char name[80];
	int  idx;
	BOOL bInside;
	BOOL bSelect;
	BOOL bCancelOption;

	CBasicButton * pBtn;


	CMatrixServ()
	{
		memset(name, 0, sizeof(name));

		pNext = NULL;
		pTextOut = NULL;
		idx = -1;
		bInside = false;
		bSelect = false;
		bCancelOption = false;
		pBtn = NULL;
	}
	~CMatrixServ()
	{
		if(pNext)
		{
			delete pNext;
			pNext = NULL;
		}
		if(pTextOut)
		{
			delete pTextOut;
			pTextOut = NULL;
		}
		if(pBtn)
		{
			delete pBtn;
			pBtn = NULL;
		}
	}
	int Restore()
	{
		if(pTextOut)
		{
			pTextOut->DeleteRes();
		}
		if(pBtn)
		{
			pBtn->DeleteRes();
		}
		return 1;
	}
	int Add(CMatrixServ * p)
	{
		if(pNext)
			pNext->Add(p);
		else
		{
			pNext = p;
		}
		return 1;
	}
	int IsInside();

};



class CLoginPage
{
public:
	CSheet* m_pSheet;

	CBasicStatic m_bg;


	CListBox *		m_ServerList;
	CMatrixServ *	m_pMatrixList;

	CBasicButton	m_LoginBtn;     // �α�
	CBasicButton	m_OptionBtn;    // ����
	CBasicButton	m_ExitBtn;

	CMsgPopUp		m_MsgPopUp;     // ����
	CMsgPopUp		m_GoHomePage;	// ����������� �˾�â
	CMsgPopUp		m_ExitPopup;	// ����������� �˾�â
	BOOL			m_bKickLogin;	// ������ ���̵��϶� ���� ������ ű��Ű�� ���� ���� ������. �븸�� ���. ///�븸ű���

	CSurface *		m_pCheck_on;
	CSurface *		m_pCheck_out;

	CSurface *		m_pLoginBack;
	POINT			m_ptLoginBackPos; // m_pLoginBack �� ��µ� ��ġ.
	BOOL			m_bCheck;

	//---------------------------------------------
	//�ݸ��� ���� üũ
	CSurface	*m_pNetCheck_on,*m_pNetCheck_out;
	BOOL		 m_bNetCheck;
	//---------------------------------------------

	// �պκ� �ε��ÿ� ���� ������.
	int			 m_iCurFrame;
	DWORD		 m_dwStartTime;
	BOOL         m_bIsLoadingEnd, m_bOneTry;
	BOOL		 m_bIsAniEnd;

	// �ִϸ��̼� �Լ�
	BOOL		m_bAnimateBtn;
	void		AnimateBtn();

	CSurface *m_pGlobalMatBack; ///���������� ����.

	// ���� �ּ� ���.
	char	**m_dpServerAddr;
	char    **m_nSvrPort;
	int		*m_pMatrixSvr;
	BOOL	*m_pbMatrixMatch;
	int     *m_nSvrType; // ��Ʈ ���Ŀ� ���� ���� Ÿ�� ����. 1 ��Ʈ����, 2 ��Ʈ���� ��ȸ, 3 PK���μ���.(1,2,�� ���� �κ����� ó��) ///�űԼ�������Ʈ
	int		m_nSvrAddrNum;

	int*	m_iSvrMode;

	BOOL	ReadServerList(char *filename);
	BOOL	ReadServerListData(char *filename); //0215
	int		m_nMatrix;
	char	*GetSentence(char *dest, int n, FILE *fp);
	char	*PGetSentence(char *dest, int n, PFILE *fp);

	CTextBox m_IdBox;
	CTextBox m_PasswdBox;

	CLoginPage();
	CLoginPage( CSheet* pSheet );
	~CLoginPage();
	void SetSheetPtr( CSheet* pSheet );
	void Init();
	void SetBackground();
	void LoadRes();
	void AniLoadRes();
	void DeleteRes();
	void Draw();

	void Draw3D();
	void Draw2D();


	void MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Action( int i );
	int  CheckID_PW();
	int  ConnectServ(int nSvrNum);

	BOOL m_bSpace;
	BOOL CheckSpace(char* String);		// ���� �Էµ� ��Ʈ���߿� "���鹮��" �� �ִ°��� üũ�ϰ� �����޼����� ����
	void ReadLastId(char *file_name);   // "Last.id"������ �о id�� ��´�
	void WriteLastId(char *file_name); // �������� ������ id�� ���Ͽ� ����


	BOOL m_bShowMatrixList;
	int  m_iCurMatrixList;
	int  AddMatrixList(char *, BOOL bCancel = FALSE);
	int  DrawMatrixList(int, int);
	int  RestoreMatrixList();
	int  GetSelectedLine_Matrix()
	{
		CMatrixServ * p = m_pMatrixList;

		while(p)
		{
			if(p->bSelect)
				return p->idx;
			p = p->pNext;
		}
		return -1;
	}


	// �α� ���μ���
	BOOL LoginProcess(int nSvrIdx, char *strUserId, char *strUserPw, BOOL bAuto=FALSE);

	// Ȩ�������� �̵� ó��
	char m_AddRes[256];		// Ȩ������ URL �ּҸ� ������ ����
	void Read_URL_Add(char *filename);
		
	BOOL IsInside(int x, int y);

//---------------------------------------------
	//�ݸ��� ���� üũ
	BOOL IsNetInside(int x, int y);
//---------------------------------------------

	BOOL IsValidIP(int nSvrIdx );
	BOOL IsValidIP(char* IpAddr );
	BOOL GetMyIP();

	int		m_nOpeningPhase; // ���� �ܰ�.
	DWORD	m_dwPhaseTick; // �ܰ谡 ����� ƽ.
	void RunCameraWork( DWORD dwPhaseElapsedTick, DWORD dwPhaseTotalTick ); // ��ŸƮ���� ī�޶� ��ũ�� �Լ�ȭ. ������ TRUE ����.

	//////////////////////////////////////////////////////////////////////////
	//// [2008/1/9 Theodoric] weblogin
	char m_strUserInfo[512];

	BOOL	PackageDecoding(char* strPack, char* strID, char* strPW);
	int		Hex2Dec(char *hex);
	//////////////////////////////////////////////////////////////////////////

	HRESULT	Restore();
	
	bool	m_bConfirm;
};

#endif // _LOGINPAGE_H_