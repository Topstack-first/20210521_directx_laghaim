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
#define LOGIN_EDIT_SIZE		11  // 보이는 글자 길이수(스크롤 되니 실제론 더 입력된다.)(2배까지 입력됨.) ///2007로딩변경 때 패스워드도 스크롤이 되게 수정하였음.
//채팅 에디트 수정
#define LOGIN_NAME_LENGTH	31  // 실제 입력 가능 글자수가 아니더라...LastId에 쓰는 길이.
#define LOGIN_EDIT_FCOLOR	RGB(255, 179, 0)

#define LOGIN_LIST_X		24
#define LOGIN_LIST_Y		150
#define LOGIN_LIST_WIDTH	160
#define LOGIN_LIST_HEIGHT	163
#define LOGIN_LIST_FCOLOR	RGB(148, 134, 99)


#define LOING_BACK_X 7 // 인터페이스 최종위치
#define LOING_BACK_Y 220 // 인터페이스 최종위치
#define LOING_BTN_ANI_START_X -200 // 인터페이스 애니메이션이 시작되는 위치.
#define LOING_BTN_ANI_TIME    1000 // 인터페이스 애니메이션이 플레이되는 시간.


#	define KOREA_IP_2012	"101.79.53."
#	define KOREA_IP			"119.205.215."

#define KOREA_INTERNAL_IP	"61.104.44."
#define KOREA_INTERNAL_IP2	"10.1.90." // by evilkiki 2010.03.15. 회사이전 : 개발 C클래스
#define KOREA_INTERNAL_IP3	"10.1.50." // by Sol9 [2012.08.31] 자리이동, ip변경
#define KOREA_INTERNAL_IP4	"10.1.110." // by evilkiki 2011.02.17. 신장비 테스트
#define KOREA_INTERNAL_IP5	"10.1.30." // by evilkiki 2011.02.17. 신장비 테스트


#define JAPAN_IP	"219.105.50."
#define JAPAN_IP_2	"122.208.74."	// [1/29/2008 반재승] 일본 IP 추가
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
#define CHINA_BJ_IP_NEW "221.236.25."//북경 IDC이전
#define CHINA_BJ_IP_NEW1 "218.7.132."//북경 본섭이전
#define CHINA_BJ_IP_NEW2 "211.151.239."//북경 IDC수정으로 인한 추가 // 05-08-20 원석 ///중국IP추가
#define CHINA_BJ_IP_NEW3 "255.255.255."//북경 IDC수정으로 인한 추가 // 05-08-20 원석 ///중국IP추가
#define CHINA_BJ_IP_NEW4 "61.182.161."//북경 IDC이전 // 06-03-02 원석 
#define CHINA_BJ_IP_NEW5 "222.66.84."//북경 IDC이전 // 06-03-23 원석 
#define INI_IP		  "202.10.33."
#define DEU_IP		  "194.9.11." // 06-08-22 독일 서비스 재시작.
#define USA_IP		  "66.129.108."
#define USA_IP_NEW  "216.27.13."//미국 서버 IDC이전
#define FRANCE_IP		"62.26.131."
#define BRASIL_IP_TEST	"201.77.211." // 07-08-31 브라질 서버 IDC이전으로 인하여 변경함.
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

	CBasicButton	m_LoginBtn;     // 로긴
	CBasicButton	m_OptionBtn;    // 설정
	CBasicButton	m_ExitBtn;

	CMsgPopUp		m_MsgPopUp;     // 종료
	CMsgPopUp		m_GoHomePage;	// 새계정만들기 팝업창
	CMsgPopUp		m_ExitPopup;	// 새계정만들기 팝업창
	BOOL			m_bKickLogin;	// 접속중 아이디일때 기존 접속을 킥시키고 들어가는 건지 프래그. 대만만 사용. ///대만킥기능

	CSurface *		m_pCheck_on;
	CSurface *		m_pCheck_out;

	CSurface *		m_pLoginBack;
	POINT			m_ptLoginBackPos; // m_pLoginBack 이 출력될 위치.
	BOOL			m_bCheck;

	//---------------------------------------------
	//넷마블 유저 체크
	CSurface	*m_pNetCheck_on,*m_pNetCheck_out;
	BOOL		 m_bNetCheck;
	//---------------------------------------------

	// 앞부분 로딩시에 관한 변수들.
	int			 m_iCurFrame;
	DWORD		 m_dwStartTime;
	BOOL         m_bIsLoadingEnd, m_bOneTry;
	BOOL		 m_bIsAniEnd;

	// 애니메이션 함수
	BOOL		m_bAnimateBtn;
	void		AnimateBtn();

	CSurface *m_pGlobalMatBack; ///국가대항전 배경용.

	// 서버 주소 목록.
	char	**m_dpServerAddr;
	char    **m_nSvrPort;
	int		*m_pMatrixSvr;
	BOOL	*m_pbMatrixMatch;
	int     *m_nSvrType; // 포트 이후에 오는 서버 타입 인자. 1 매트릭스, 2 매트릭스 대회, 3 PK성인서버.(1,2,는 위의 두변수로 처리) ///신규서버리스트
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
	BOOL CheckSpace(char* String);		// 현재 입력된 스트링중에 "공백문자" 가 있는가를 체크하고 에러메세지를 띄운다
	void ReadLastId(char *file_name);   // "Last.id"파일을 읽어서 id를 얻는다
	void WriteLastId(char *file_name); // 마지막에 생성된 id를 파일에 쓴다


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


	// 로긴 프로세스
	BOOL LoginProcess(int nSvrIdx, char *strUserId, char *strUserPw, BOOL bAuto=FALSE);

	// 홈페이지로 이동 처리
	char m_AddRes[256];		// 홈페이지 URL 주소를 저장할 공간
	void Read_URL_Add(char *filename);
		
	BOOL IsInside(int x, int y);

//---------------------------------------------
	//넷마블 유저 체크
	BOOL IsNetInside(int x, int y);
//---------------------------------------------

	BOOL IsValidIP(int nSvrIdx );
	BOOL IsValidIP(char* IpAddr );
	BOOL GetMyIP();

	int		m_nOpeningPhase; // 진행 단계.
	DWORD	m_dwPhaseTick; // 단계가 진행된 틱.
	void RunCameraWork( DWORD dwPhaseElapsedTick, DWORD dwPhaseTotalTick ); // 스타트시의 카메라 워크를 함수화. 끝나면 TRUE 리턴.

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