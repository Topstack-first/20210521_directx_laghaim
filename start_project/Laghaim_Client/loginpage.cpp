#include "stdafx.h"
#define D3D_OVERLOADS
#include <assert.h>
#include <mmsystem.h>
#include <direct.h>
#include "LoginPage.h"
#include "headers.h"
#include "Sheet.h"
#include "MsgPopUp.h"
#include "TextBox.h"
#include "Tcpipcon.h"
#include "ItemProto.h"
#include "MakeAccPage.h"
#include "selectcharacterpage.h"
#include "LH_selectcharacterpage.h"
#include "main.h"
#include "IndexedTexture.h"
#include "DirtSnd.h"
#include "Country.h"
#include "textoutbox.h"
#include "g_stringmanager.h"
#include "WebWorld.h"
#include "CommonConfig.h"


#pragma warning( disable : 4244 )
#pragma warning( disable : 4018 )

#define GOSELCHAR	1
#define GOMAKEACNT	2
#define GOOPTION	3
#define GOSELCHAR_MATRIX  4
#define GOLOGIN     5

#define BACKGROUND_LOGINPAGE	"interface/login/back_loginpage.bmp"
#define SCROLLBTN_UP			"interface/login/cb_arrowup_normal.bmp"
#define SCROLLBTN_DOWN			"interface/login/cb_arrowdown_normal.bmp"
#define SCROLL_BAR				"interface/login/ScrollBar.bmp"

#	define TEST_SERVER_IP			"121.254.248.153"

bool strinc(char *str, const char *inc);

extern BOOL			g_bTestServer;
extern void			RefreshSkills();
extern DWORD		g_dwLangType;
extern DWORD		g_dwClientCountry;
extern int			g_MatrixSvr;
extern BOOL			g_bMatrixMatch;
extern int			g_nServerIndex;


extern int	g_EffectTextr[];
extern BOOL	g_bUseMatrixCountry;
extern BOOL	g_bAdmin;
extern BOOL g_bGlobalMatrix;

extern void ReadLine(FILE *fp, char *str);
extern void PReadLine( PFILE *fp, char *str );

// 각 페이즈가 끝나는 틱.
// (다음 페이즈의 시작 틱이기도하다.)
// (모두 종료되는 틱까지 있어야함에 유의. ///2007로딩변경

DWORD g_PhaseEndTick[4] = { 0, 2000, 1000, 1000 };


int CMatrixServ::IsInside()
{
	POINT point;
	GetCursorPos(&point);
	if( g_pDisplay )
		ScreenToClient(g_pDisplay->GetHWnd(), &point);


	if(pBtn && pBtn->IsInside(point.x, point.y))
	{
		if(bInside) return 1;
		if( pTextOut )
			pTextOut->ChangeColor(RGB(255, 142, 103));
		pBtn->m_iBtnFlg = BTN_ON;
		bInside = TRUE;
	}
	else
	{
		if(!bInside)
			return 1;
		if( pTextOut )
			pTextOut->ChangeColor(LOGIN_LIST_FCOLOR);
		if( pBtn )
			pBtn->m_iBtnFlg = BTN_NORMAL;
		bInside = FALSE;
	}
	return 1;
}


int  CLoginPage::AddMatrixList(char * ServName, BOOL bCancel)
{
	CMatrixServ * p = new CMatrixServ();

	if( !p || ServName == NULL || !g_pDisplay )
		return 0;

	strcpy(p->name, ServName);
	p->idx = m_iCurMatrixList++;
	p->pTextOut = new CTextOutBox();
	p->pTextOut->Init(12, LOGIN_LIST_FCOLOR,0, 0);
	p->pTextOut->SetString(p->name);
	p->pBtn = new CBasicButton();
	p->pBtn->SetFileName("login/btn_List");
	p->pBtn->LoadRes();
	p->bCancelOption = bCancel;

	if(!m_pMatrixList)
		m_pMatrixList = p;
	else
		m_pMatrixList->Add(p);

	return 1;
}

int CLoginPage::DrawMatrixList(int start_x, int start_y)
{
	int x, y;
	CMatrixServ * p = m_pMatrixList;

	if(!p)
		return 0;

	while(p)
	{
		x = start_x;
		y = start_y + (p->idx * 26);

		p->pBtn->SetPosition(x, y);
		p->pTextOut->SetPos(x, y);
		p->IsInside();
		p->pBtn->Draw();
		p->pTextOut->Draw(x, y+6, 122);
		p = p->pNext;
	}

	return 1;
}

int CLoginPage::RestoreMatrixList()
{
	CMatrixServ * p = m_pMatrixList;
	if( !p )
		return 0;

	while(p)
	{
		p->Restore();
		p->bInside = FALSE;
		p = p->pNext;
	}
	m_iCurMatrixList = 0;
	m_bShowMatrixList = FALSE;

	return 1;
}










#include "Filtering.h"
//-------------------------------------
//----------------------------------------

CLoginPage::CLoginPage()
	: m_pMatrixList(NULL)
	, m_bSpace(FALSE)
	, m_bShowMatrixList(FALSE)
	, m_iCurMatrixList(0)
	, m_nOpeningPhase(0)
	, m_dwPhaseTick(0)
{
	memset(m_AddRes, 0, sizeof(m_AddRes));
	memset(m_strUserInfo, 0, sizeof(m_strUserInfo));

	m_pSheet		= NULL;
	m_ServerList	= NULL;
	m_dpServerAddr	= NULL;
	m_nSvrPort		= NULL;
	m_pMatrixSvr	= NULL;
	m_pbMatrixMatch = NULL;
	m_nSvrType		= NULL; ///신규서버리스트
	m_pCheck_on     = NULL;
	m_pCheck_out    = NULL;
	//---------------------------------------------
	//넷마블 유저 체크
	m_pNetCheck_on     = NULL;
	m_pNetCheck_out    = NULL;
	m_bNetCheck = FALSE;
	//---------------------------------------------

	m_pLoginBack = NULL;
	
	m_bCheck = TRUE;


	m_bIsAniEnd = TRUE;
	m_bAnimateBtn = TRUE;

	m_pGlobalMatBack = NULL;  ///국가대항전 배경용.

	m_bConfirm = false;

	Init();
}

CLoginPage::CLoginPage( CSheet* pSheet )
	: m_bSpace(FALSE)
	, m_iCurMatrixList(0)
	, m_nOpeningPhase(0)
	, m_dwPhaseTick(0)
	, m_bConfirm(false)
{
	memset(m_AddRes, 0, sizeof(m_AddRes));
	memset(m_strUserInfo, 0, sizeof(m_strUserInfo));

	m_pSheet = NULL;
	SetSheetPtr( pSheet );
	m_ServerList	= NULL;
	m_dpServerAddr  = NULL;
	m_nSvrPort	    = NULL;
	m_pCheck_on     = NULL;
	m_pCheck_out    = NULL;

	//---------------------------------------------
	//넷마블 유저 체크
	m_pNetCheck_on     = NULL;
	m_pNetCheck_out    = NULL;
	//---------------------------------------------

	m_pLoginBack = NULL;	
	m_bCheck = TRUE;

	m_bIsAniEnd = FALSE;
	m_bAnimateBtn = TRUE;


	m_pMatrixList = NULL;
	m_pGlobalMatBack = NULL;  ///국가대항전 배경용.
	m_bShowMatrixList = FALSE;

	m_bNetCheck = FALSE;
	Init();
}

CLoginPage::~CLoginPage()
{
	DeleteRes();

	if (m_ServerList)
	{
		delete m_ServerList;
		m_ServerList = NULL;
	}

	if (m_dpServerAddr)
	{
		for (int i=0; i < m_nSvrAddrNum; i++)
			delete [] m_dpServerAddr[i];

		delete [] m_dpServerAddr;
		m_dpServerAddr = NULL;
	}

	if(m_nSvrPort)
	{
		for(int ii=0; ii< m_nSvrAddrNum; ii++)
			delete [] m_nSvrPort[ii];

		delete [] m_nSvrPort;
		m_nSvrPort = NULL;
	}

	if (m_pMatrixSvr)
	{
		delete [] m_pMatrixSvr;
		m_pMatrixSvr = NULL;
	}

	if (m_pbMatrixMatch)
	{
		delete [] m_pbMatrixMatch;
		m_pbMatrixMatch = NULL;
	}

	if (m_nSvrType) ///신규서버리스트
	{
		delete [] m_nSvrType;
		m_nSvrType = NULL;
	}

	SAFE_DELETE_ARRAY(m_iSvrMode);

	if(m_pMatrixList)
	{
		delete m_pMatrixList;
		m_pMatrixList = NULL;
	}

}

void CLoginPage::SetSheetPtr( CSheet* pSheet )
{
	m_pSheet = pSheet;
}

void CLoginPage::Init()
{
	// Buttons
	m_LoginBtn.SetFileName("login/btn_login");
	m_LoginBtn.SetDisable( false );

	m_OptionBtn.SetFileName("login/btn_setting");
	m_OptionBtn.SetDisable( false );

	m_ExitBtn.SetFileName("login/btn_login_exit");
	m_ExitBtn.SetDisable( false );


	// 인터페이스 좌표 입력.
	m_ptLoginBackPos.x = LOING_BACK_X;
	m_ptLoginBackPos.y = LOING_BACK_Y;

	// Server List Box
	m_ServerList = new CListBox(" ", m_ptLoginBackPos.x+LOGIN_LIST_X, m_ptLoginBackPos.y+LOGIN_LIST_Y, LOGIN_LIST_WIDTH, LOGIN_LIST_HEIGHT);



	m_ServerList->SetImage( "common/scroll_dn_t01", "common/scroll_up_t01", "interface/login/serverlist_scbar.bmp" );
	if( g_dwClientCountry == CTRY_BRA ) // 브라질은 첫번째 서버가 디폴트로 선택되어있게 해달랜다. ///07-01-31
	{
		m_ServerList->SetSelectedLine(0);
		m_ServerList->SetTextColor(RGB(255, 142, 103)); // 실제 위의 셀렉트는 변수만 바꿀뿐 실제 글자 색을 바꿔주지 않아서 편법으로 전 리스트의 색을 선택색으로 바꾼다. 일단 브라질은 서버가 하나이기 때문에...
	}
	else
	{
		m_ServerList->SetSelectedLine(-1);
		m_ServerList->SetTextColor(LOGIN_LIST_FCOLOR);
	}

	ReadServerListData("SvrList.dta");	
	//ReadServerListData("SvrListm.dta");  //ENABLE FOR MATRIX WAR ONLY  PABLO 02/10/2019
	
	m_IdBox.Init(m_ptLoginBackPos.x+LOGIN_EDIT_X, m_ptLoginBackPos.y+LOGIN_EDIT_Y, LOGIN_EDIT_SIZE, RGB(255, 255, 255), LOGIN_EDIT_FCOLOR, FALSE, LOGIN_PAGE);
	m_PasswdBox.Init( m_ptLoginBackPos.x+LOGIN_PW_EDIT_X, m_ptLoginBackPos.y+LOGIN_PW_EDIT_Y, LOGIN_EDIT_SIZE, RGB(255, 255, 255), LOGIN_EDIT_FCOLOR, TRUE);

	m_MsgPopUp.Init(TYPE_NOR_OK);
	m_GoHomePage.Init(TYPE_NOR_OKCANCLE); ///대만킥기능 확인용으로 사용.(07-03-08 원석)

	m_ExitPopup.Init(TYPE_NOR_OK);
	
	m_bIsLoadingEnd  = FALSE;
	m_bOneTry        = FALSE;
	m_iCurFrame      = 0;
	m_dwStartTime    = 0;	

	m_nMatrix = -1;

	m_bKickLogin = FALSE; ///대만킥기능
}

BOOL CLoginPage::ReadServerList(char *filename)
{
	if( filename == NULL )
		return FALSE;

	FILE *fp = NULL;

	char buf[256];

	// 서버 이름  , 주소
	char nick[100], addr[100], port[100];

	char *sentence=NULL;
	int i;

	fp = fopen(filename, "rt");
	assert(fp != NULL);

	sentence = GetSentence(buf, sizeof(buf), fp);

	sscanf(sentence, "%d", &m_nSvrAddrNum);

	// 메모리 할당

	// 현재 m_nSvrAddrNum = '3'
	m_dpServerAddr = new char *[m_nSvrAddrNum];
	m_nSvrPort	   = new char *[m_nSvrAddrNum];
	m_pMatrixSvr  = new int [m_nSvrAddrNum];
	m_pbMatrixMatch= new BOOL [m_nSvrAddrNum];
	m_nSvrType	   = new int  [m_nSvrAddrNum]; ///신규서버리스트


	for (i = 0; i < m_nSvrAddrNum; i++)
	{
		// 첫번째 읽은줄을 sentence에 넣는다
		sentence = GetSentence(buf, sizeof(buf), fp);
		/////0123 서버리스트 암호화 적용시 쓸꺼...
		/*

		int size = 0;
		while(buf[size]!='\n'){
		size++;
		}

		for (int idx = 0; idx < size; idx++) {
		buf[idx] = (buf[idx]) ^ 1;
		}

		sscanf(buf, "%s %s %s", nick, addr, port);
		*/
		sscanf(sentence, "%s %s %s", nick, addr, port);
		////////////
		m_dpServerAddr[i] = new char[strlen(addr)+1];
		m_nSvrPort[i]	  = new char[strlen(port)+1];

		strcpy(m_dpServerAddr[i], addr);
		strcpy(m_nSvrPort[i], port);
		m_pMatrixSvr[i] = 0;
		m_pbMatrixMatch[i] = FALSE;
		m_nSvrType[i] = 0; ///신규서버리스트

		m_ServerList->AddString(nick);
	}

	fclose(fp);

	return TRUE;
}

BOOL CLoginPage::ReadServerListData(char *filename) //0215
{
	if( g_dwClientCountry == CTRY_CN ) // 현재는 중국만 웹 서버리스트를 도입.
	{
		CWebWorld	webworld;

		char address[128] = {0,};

		switch( g_dwClientCountry )
		{
		case CTRY_KOR:
			sprintf(address, "http://laghaim.online/update/"); // 테스트용.
			break;
		case CTRY_CN:
			sprintf(address, "http://laghaim.online/update/");
			break;
		default:
			return FALSE;
		}

		FILE *TestFile = NULL; // "config.ini2" 파일로 체크해서 해당 파일이 있으면 다른 서버리스트 파일을 읽는다.운영자용 서버리스트.

		TestFile = fopen( "config.ini3", "rb" );

		if( TestFile )
		{
			strcat( address, "SvrList.dtat" ); // 운영자용 서버리스트를 읽는다.
			fclose( TestFile );
			g_dev_client = true;
		}
		else
		{
			strcat( address, "SvrList.dta" );
			//strcat( address, "SvrListm.dta" );  //ENABLE FOR MATRIX WAR ONLY  PABLO 02/10/2019
		}

		if( webworld.GetWebPage(address) )
		{
			if( !webworld.m_UrlData || strlen(webworld.m_UrlData) <= 0 ) // 이 경우는 뭔가 잘못된 듯.
			{
				return FALSE;
			}

			char buf[256];

			// 서버 이름  , 주소
			char nick[100], addr[100], port[100], matrix[10];

			int i,j;
			unsigned char key;
			int iKey, iKeyTemp;

			// 앞 6바이트는 더미.
			j = 6;
			memcpy( &iKey, &webworld.m_UrlData[j], sizeof(int) ); // 암호화 키.
			j += sizeof(int);
			memcpy( &key, &webworld.m_UrlData[j], sizeof(BYTE) ); // 암호화 키.
			j += sizeof(BYTE);
			memcpy( &m_nSvrAddrNum, &webworld.m_UrlData[j], sizeof(int) ); // 서버리스트의 갯수.
			j += sizeof(int);

			iKeyTemp = m_nSvrAddrNum;
			m_nSvrAddrNum -= iKey;
			iKey = iKeyTemp;

			// 메모리 할당

			m_dpServerAddr = new char *[m_nSvrAddrNum];
			m_nSvrPort	   = new char *[m_nSvrAddrNum];
			m_pMatrixSvr  = new int [m_nSvrAddrNum];
			m_pbMatrixMatch= new BOOL [m_nSvrAddrNum];
			m_nSvrType	   = new int [m_nSvrAddrNum]; ///신규서버리스트

			for (i = 0; i < m_nSvrAddrNum; i++)
			{
				// 첫번째 읽은줄을 buf에 넣는다.
				int count;
				unsigned char data;

				memcpy( &count, &webworld.m_UrlData[j], sizeof(int) );
				j += sizeof(int);

				iKeyTemp = count;
				count -= iKey;
				iKey = iKeyTemp;

				for (int idx = 0; idx < count; idx++)
				{
					memcpy( &data, &webworld.m_UrlData[j], sizeof(BYTE) );
					j += sizeof(BYTE);
					buf[idx] = data - key;
					key = data;
				}
				buf[count] = '\0';

				matrix[0] = '\0';
				sscanf(buf, "%s %s %s %s", nick, addr, port, matrix);
				m_dpServerAddr[i] = new char[strlen(addr)+1];
				m_nSvrPort[i]	  = new char[strlen(port)+1];

				strcpy(m_dpServerAddr[i], addr);
				strcpy(m_nSvrPort[i], port);

				m_ServerList->AddString(nick);

				m_nMatrix = atoi(matrix);

				switch( m_nMatrix )
				{
				case 0:
					m_pMatrixSvr[i] = 0;
					break;
				case 1:
					m_pMatrixSvr[i] = 1;
					break;
				case 4:
					m_pMatrixSvr[i] = 2;
					break;
				}

				switch( m_nMatrix )
				{
				case 2:
					m_pbMatrixMatch[i] = TRUE;
					break;
				default:
					m_pbMatrixMatch[i] = FALSE;
					break;
				}
				/*
								if (m_nMatrix > 0 && m_nMatrix < 3)
									m_pbMatrixSvr[i] = TRUE;
								else
									m_pbMatrixSvr[i] = FALSE;

								// 게임대회 서버인지 확인한다.
								if (m_nMatrix == 2)
									m_pbMatrixMatch[i] = TRUE;
								else
									m_pbMatrixMatch[i] = FALSE;

				  */

				// 서버 포트 다음에 오는 서버 타입을 지정해둔다. (위 1,2 타입은 그냥 위에 루틴 그대로 둔다.)
				m_nSvrType[i] = m_nMatrix; ///신규서버리스트
			}
		}
	}
	else
	{
		FILE *fp = NULL;

		char buf[256] = {0, };

		// 서버 이름  , 주소
		char nick[100] = {0, }, addr[100] = {0, }, port[100] = {0, }, matrix[10] = {0, };

		int i;
		unsigned char key;
		int iKey, iKeyTemp;

		fp = fopen(filename, "rb");
		assert(fp != NULL);

		for (i = 0; i < 6; i++)
		{
			fread(&key, 1, 1, fp);
		}
		fread(&iKey, sizeof(int), 1, fp);
		fread(&key, 1, 1, fp);

		fread(&m_nSvrAddrNum, sizeof(int), 1, fp);

		iKeyTemp = m_nSvrAddrNum;
		m_nSvrAddrNum -= iKey;
		iKey = iKeyTemp;

		// 메모리 할당


		// 현재 m_nSvrAddrNum = '3'
		m_dpServerAddr = new char *[m_nSvrAddrNum];
		m_nSvrPort	   = new char *[m_nSvrAddrNum];
		m_pMatrixSvr   = new int [m_nSvrAddrNum];
		m_pbMatrixMatch= new BOOL [m_nSvrAddrNum];
		m_nSvrType	   = new int [m_nSvrAddrNum]; ///신규서버리스트

		m_iSvrMode	   = new int [m_nSvrAddrNum];

		for (i = 0; i < m_nSvrAddrNum; i++)
		{
			// 첫번째 읽은줄을 buf에 넣는다.
			int count;
			unsigned char data;
			fread(&count, sizeof(int), 1, fp);

			iKeyTemp = count;
			count -= iKey;
			iKey = iKeyTemp;

			for (int idx = 0; idx < count; idx++)
			{
				fread(&data, 1, 1, fp);
				buf[idx] = data - key;
				key = data;
			}
			buf[count] = '\0';

			matrix[0] = '\0';
			sscanf(buf, "%s %s %s %s", nick, addr, port, matrix);
			m_dpServerAddr[i] = new char[strlen(addr)+1];
			m_nSvrPort[i]	  = new char[strlen(port)+1];

			if(!strcmp(addr, TEST_SERVER_IP))
				g_bTestServer = TRUE;

			strcpy(m_dpServerAddr[i], addr);
			strcpy(m_nSvrPort[i], port);

			m_ServerList->AddString(nick);

			m_nMatrix = atoi(matrix);

			switch( m_nMatrix )
			{
			case 3:
			case 0:
				m_pMatrixSvr[i] = 0;
				break;
			case 1:
				m_pMatrixSvr[i] = 1;
				break;
			case 4:
				m_pMatrixSvr[i] = 2;
				break;
// by evilkiki 2009.11.05 길드랭킹전 ( ip port value : value = 5 여야함)
			case 5:
				m_pMatrixSvr[i] = 0;
				break;
			default:
				m_pMatrixSvr[i] = 0;
				break;
			}

			switch( m_nMatrix )
			{
			case 2:
				m_pbMatrixMatch[i] = TRUE;
				break;
			default:
				m_pbMatrixMatch[i] = FALSE;
				break;
			}

			switch( m_nMatrix )
			{
			case 6:
				m_iSvrMode[i] = eSVR_MASTER;
				break;
			default:
				m_iSvrMode[i] = eSVR_NORMAL;
				break;
			}

			/*

						if (m_nMatrix > 0 && m_nMatrix < 3)
							m_pbMatrixSvr[i] = TRUE;
						else
							m_pbMatrixSvr[i] = FALSE;

						// 게임대회 서버인지 확인한다.
						if (m_nMatrix == 2)
							m_pbMatrixMatch[i] = TRUE;
						else
							m_pbMatrixMatch[i] = FALSE;
			*/
			// 서버 포트 다음에 오는 서버 타입을 지정해둔다. (위 1,2 타입은 그냥 위에 루틴 그대로 둔다.)
			m_nSvrType[i] = m_nMatrix; ///신규서버리스트
		}
		fclose(fp);
	}

	return TRUE;
}


char *CLoginPage::GetSentence(char *dest, int n, FILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = fgets(dest, n, fp)) == NULL)
			return tmp;

		//while ((*tmp == 0x20 || (*tmp >= 0x09 && *tmp <= 0x0D)) && *tmp != 0)
		while (iswspace(*tmp) && *tmp != 0)
			tmp++;
	}
	while (*tmp == '#' || *tmp == 0);

	return tmp;
}

void CLoginPage::SetBackground()
{
}

void CLoginPage::AniLoadRes()
{
}
void CLoginPage::LoadRes()
{
	m_LoginBtn.LoadRes();
	m_OptionBtn.LoadRes();
	m_ExitBtn.LoadRes();

	m_ServerList->LoadRes(  );

	m_IdBox.SetfontSize(12);
	m_PasswdBox.SetfontSize(12);

	m_MsgPopUp.LoadRes();
	m_GoHomePage.LoadRes();
	m_ExitPopup.LoadRes();

	//g_pDisplay->CreateSurfaceFromBitmap(&m_pLoginBack, "interface/login/login_serverlist_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pLoginBack, "interface/login/Bg.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pCheck_on, "interface/common/check.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pCheck_out, "interface/common/check_none.bmp");

	m_pLoginBack->Xpos = 0;
	m_pLoginBack->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pLoginBack->GetHeight())>>1;

	m_pCheck_on->Xpos = m_pLoginBack->Xpos+333;
	m_pCheck_on->Ypos = m_pLoginBack->Ypos+480;

	m_pCheck_out->Xpos = m_pLoginBack->Xpos+333;
	m_pCheck_out->Ypos = m_pLoginBack->Ypos+480;

	m_ServerList->SetPosDim(m_pLoginBack->Xpos + 320, m_pLoginBack->Ypos + 276, 213, 198);

	m_LoginBtn.SetPosition( m_pLoginBack->Xpos+310, m_pLoginBack->Ypos+520 );     // 접속
	m_OptionBtn.SetPosition( m_pLoginBack->Xpos+680, m_pLoginBack->Ypos+520 );	    // 설정
	m_ExitBtn.SetPosition( m_pLoginBack->Xpos+680, m_pLoginBack->Ypos+550 );       // 종료



	m_pLoginBack->SetColorKey(RGB(0, 0, 0));
	m_pCheck_on->SetColorKey(RGB(0, 0, 0));
	m_pCheck_out->SetColorKey(RGB(0, 0, 0));


	if (g_dwClientCountry == CTRY_MENG)
	{
		m_IdBox.SetText(pCMyApp->m_strUserId);
		m_PasswdBox.SetText(pCMyApp->m_strUserPw);
	}
	else
		ReadLastId("Last.id");

	Read_URL_Add("explorer.txt");

	m_iCurMatrixList = 0;

	// 매트릭스에 접속시 선택하는 서버리스트의 스트링을 txt에서 읽어오도록 변경(05-08-10 by 원석)
	if( g_bUseMatrixCountry ) // 뫼비우스가 도입된 나라만 처리...
	{
		PFILE *fp;
		int MatSvrNum = 0;
		char TempString[255];

		fp = pfopen( "data/mat_svr.txt", "rt" );

		if( fp )
		{
			pfscanf( fp, "%d \n", &MatSvrNum ); // 서버의 갯수.

			for( int i = 0 ; i < MatSvrNum ; ++i )
			{
				PReadLine( fp, TempString ); // 그냥 fgets를 사용하면 한글은 뒤에 깨진 글자 들어가는 문제가 있어서 스킬.dta에서 쓰이던 한줄읽기 함수를 가져다 썼다.

				if( i != MatSvrNum-1 ) // 마지막 녀석이 아니면...
					AddMatrixList( TempString );
				else // 마지막 녀석은 "취소"다.
					AddMatrixList( TempString, TRUE );	// 취소는 3번째인자를 TRUE로 주더라...
			}

			pfclose( fp );
		}
		else // 파일이 없을 경우.
			MessageBox( NULL, "[mat_svr.txt] File Not Found!", "Error", MB_OK );

		///국가대항전 용 배경 로딩

		if( g_bGlobalMatrix )
			g_pDisplay->CreateSurfaceFromBitmap(&m_pGlobalMatBack, "interface/loading/mobiusarena.bmp");
	}

	m_bg.Init("login/bg");

}

void CLoginPage::DeleteRes()
{
	m_LoginBtn.DeleteRes();
	m_OptionBtn.DeleteRes();
	m_ExitBtn.DeleteRes();
	// 수정 부분
	m_IdBox.DeleteRes();
	m_PasswdBox.DeleteRes();
	//

	if (m_ServerList)
	{
		m_ServerList->DeleteRes();
	}

	m_MsgPopUp.DeleteRes();
	m_GoHomePage.DeleteRes(); ///대만킥기능 확인용으로 사용.(07-03-08 원석)
	m_ExitPopup.DeleteRes();
	SAFE_DELETE(m_pGlobalMatBack);  ///국가대항전 배경용.

	SAFE_DELETE(m_pCheck_on);
	SAFE_DELETE(m_pCheck_out);
	//---------------------------------------------
	//넷마블 유저 체크
	SAFE_DELETE(m_pNetCheck_on);
	SAFE_DELETE(m_pNetCheck_out);
	//---------------------------------------------

	SAFE_DELETE(m_pLoginBack);


	RestoreMatrixList();

}

void CLoginPage::Draw3D()
{
	static int start_ani = 0;


	if(!m_bIsLoadingEnd)
	{
		DWORD curTime = timeGetTime();
		RECT rc;
		SetRect(&rc, 0, 0, 800, 600);
		if(!m_bOneTry)
		{
			m_dwStartTime = curTime;
			m_bOneTry = TRUE;
		}
		if((curTime - m_dwStartTime) < 100)
		{
		}
		else
		{
			m_iCurFrame++;
			m_dwStartTime = curTime;
//			if(m_iCurFrame == 3)
			{
				m_bIsLoadingEnd = TRUE;
				return;
			}
		}
		return;
	}

	RECT rc = {0, 0, 800, 494};

	if (m_bIsLoadingEnd && !m_bIsAniEnd)
	{
		if (!start_ani)
		{
			m_dwStartTime = timeGetTime();
			start_ani = 1;
		}

		m_dwPhaseTick = timeGetTime() - m_dwStartTime;

		if( m_dwPhaseTick >= g_PhaseEndTick[0] && m_dwPhaseTick <= g_PhaseEndTick[1] )
			m_nOpeningPhase = 0; // 페이드 아웃 & 회사 로고 페이드인(유지)
		else if( m_dwPhaseTick >= g_PhaseEndTick[1] && m_dwPhaseTick <= g_PhaseEndTick[2] )
			m_nOpeningPhase = 1; // 회사 로고 페이드아웃 & 페이드인
		else if( m_dwPhaseTick >= g_PhaseEndTick[2] && m_dwPhaseTick <= g_PhaseEndTick[3] )
			m_nOpeningPhase = 2; // 문열림시작 & 페이드 아웃
		else
		{
			m_bIsAniEnd = TRUE;
			m_bAnimateBtn = TRUE;

			m_dwStartTime = timeGetTime();			// 버튼 애니메이션을 위해 시작시간 기록.
		}
	}
	//=======================================================================

	m_pSheet->TestSceneRender(0);

}

void CLoginPage::Draw2D()
{
	if(m_bShowMatrixList)
	{
		if( g_bGlobalMatrix && m_pGlobalMatBack )
		{
			g_pDisplay->Blt( 0, 0, m_pGlobalMatBack, NULL );
		}

		DrawMatrixList((800/2 - 122/2), 245);
		m_MsgPopUp.Draw();
		return;
	}

	if (m_bAnimateBtn && m_bIsAniEnd)
	{
		AnimateBtn();
	}


	if (m_bIsAniEnd && m_bAnimateBtn == FALSE)
	{
		m_bg.Draw();

		g_pDisplay->Blt( m_pLoginBack->Xpos, m_pLoginBack->Ypos, m_pLoginBack);

		m_LoginBtn.Draw( m_pLoginBack->Xpos+313, m_pLoginBack->Ypos+515 );     // 접속
		m_OptionBtn.Draw( m_pLoginBack->Xpos+655, m_pLoginBack->Ypos+515 );	    // 설정
		m_ExitBtn.Draw( m_pLoginBack->Xpos+655, m_pLoginBack->Ypos+545 );       // 종료
				
		if(m_bCheck)
			g_pDisplay->Blt( m_pCheck_on->Xpos, m_pCheck_on->Ypos, m_pCheck_on);
		else
			g_pDisplay->Blt( m_pCheck_out->Xpos, m_pCheck_out->Ypos, m_pCheck_out);
		
		m_ServerList->CalculateBarPos2( 18 ); // 스크롤바의 위치를 강제로 구함.(졸라 구린 루)
		m_ServerList->Draw();		
		
		m_IdBox.Draw(m_pLoginBack->Xpos+345, m_pLoginBack->Ypos+392 , 106 );
		m_PasswdBox.Draw(m_pLoginBack->Xpos+345, m_pLoginBack->Ypos+448, 106 );
		
		m_MsgPopUp.Draw();


		m_GoHomePage.Draw(); ///대만킥기능 확인용으로 사용.(07-03-08 원석)

		m_ExitPopup.Draw();
	}

	Sleep(1);
}

void CLoginPage::Draw()
{
	Draw3D();
	Draw2D();
}

#define LEFT_IDCHECK_WIDTH 109
#define LEFT_ID_WIDTH 132
#define LEFT_PW_WIDTH 150

#define RIGHT_LOGIN_WIDTH 126
#define RIGHT_OPTION_WIDTH 144
#define RIGHT_ACC_WIDTH 162
#define RIGHT_EXIT_WIDTH 180

void CLoginPage::AnimateBtn()
{
	float ElapedTick = timeGetTime() - m_dwStartTime;

	if( ElapedTick > LOING_BTN_ANI_TIME )
	{
		m_ptLoginBackPos.x = LOING_BACK_X;
		m_bAnimateBtn = FALSE;		
	}
}

void CLoginPage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int ret;
	int x, y;

	if(m_GoHomePage.CheckMsgExist() == TRUE)
	{
		if ((ret = m_GoHomePage.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{			
			if( CommonConfig::Instance()->GetLoginProcess() == true )
			{
				ShellExecute(hWnd, NULL , "iexplore", "https://discordapp.com/invite/3SzqxJZ", NULL, SW_SHOWNORMAL);
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				return;
			}
			else
			{
				m_bKickLogin = TRUE;
				Action( GOSELCHAR );
			}
		}
		else
		{
			if( CommonConfig::Instance()->GetLoginProcess() == true )
			{
				if(ret == -1)
				{
					m_bConfirm = false;
					Action( GOSELCHAR );					
				}
			}
			else
			{
				m_PasswdBox.ClearText();
			}
		}
		return;
	}

	if( m_ExitPopup.CheckMsgExist() == TRUE )
	{
		if ((ret = m_ExitPopup.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return;
		}
	}

	if(m_MsgPopUp.CheckMsgExist() == TRUE)
	{
		if ((ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{	
			if (g_pDSound)
			{
				g_pDSound->Play(g_SheetSound[SHEET_SND_CLICK]);
			}
		}
		m_PasswdBox.ClearText();
		return ;
	}


	m_ServerList->GetMessage(hWnd, msg, wParam, lParam); // 로그인페이지에선 이녀석의 리턴값을 검사할 필요가없다.

	switch ( msg )
	{
	case WM_RBUTTONUP: // 마우스 오른쪽 버튼으로 운영자 전용 서버리스트 리플레쉬 기능 구현( 05-10-29 원석)

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( g_bAdmin ) // 운영자 판단은 메뉴바로만 한다.(서버인증은 당연히 안되지;;)
		{
			DeleteRes(); // 서피스를 새로 만들어야하니 우선지우기.

			if( m_ServerList ) // Init()에서 new하니 미리 지워줘야한다.
			{
				delete m_ServerList;
				m_ServerList = NULL;
			}

			Init(); // 여기서 로드하니 이걸 꼭해줘야한다.
			LoadRes(); // 이걸해줘야 리스트 서피스를 새로 만듬.
		}

		break;

	case WM_LBUTTONDOWN:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_bIsLoadingEnd && !m_bIsAniEnd ) // 만약 오프닝신을 연출 중이었다면... ///2007로딩변
			{
				// 오프닝 신 스킵 처리.
				m_bIsAniEnd = TRUE;
				m_bAnimateBtn = TRUE;
				m_dwStartTime = timeGetTime();			// 버튼 애니메이션을 위해 시작시간 기록.

				m_pSheet->m_vEye    = D3DVECTOR( 1036.18f, 390.0f, 1610.20f );
				m_pSheet->m_vLookAt = D3DVECTOR( 1029.54f, 392.934f, 1617.07f );

				return;
			}

			if(m_bShowMatrixList)
			{
				CMatrixServ * p = m_pMatrixList;
				while(p)
				{
					if(p->bInside)
					{
						p->bSelect = TRUE;
						p->pTextOut->ChangeColor(RGB(255, 255, 0));
						p->pBtn->m_iBtnFlg = BTN_DOWN;
						return;
					}
					else
					{
						p->bSelect = FALSE;
					}
					p = p->pNext;
				}
				return;
			}

			m_IdBox.MsgProc( hWnd, msg, wParam, lParam );
			m_PasswdBox.MsgProc( hWnd, msg, wParam, lParam );			

			if(IsInside(x, y) )
				m_bCheck == TRUE ? m_bCheck = FALSE : m_bCheck = TRUE;
		}

	case WM_MOUSEMOVE:
		m_LoginBtn.MsgProc( hWnd, msg, wParam, lParam);
		m_OptionBtn.MsgProc( hWnd, msg, wParam, lParam);
		m_ExitBtn.MsgProc(hWnd, msg, wParam, lParam);

		break;

	case WM_LBUTTONUP:
		if(m_bShowMatrixList)
		{
			CMatrixServ * p = m_pMatrixList;
			while(p)
			{
				if(p->bInside && p->bSelect)
				{

					if(p->bCancelOption)
					{
						m_bShowMatrixList = FALSE;
						pCMyApp->ReleaseGameMutex();
						return;
					}

					Action(GOSELCHAR_MATRIX);
					p->pBtn->m_iBtnFlg = BTN_NORMAL;
					return;
				}
				p = p->pNext;
			}
			return;
		}


		m_LoginBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_OptionBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_ExitBtn.MsgProc( hWnd, msg, wParam, lParam );

		if ( m_LoginBtn.GetState() == BTN_ACTION )
		{
			m_bConfirm = true;

			m_LoginBtn.m_iBtnFlg = BTN_NORMAL;
			m_IdBox.CloseIMC();
			m_PasswdBox.CloseIMC();

			Action( GOSELCHAR );
		}

		if ( m_OptionBtn.GetState() == BTN_ACTION )
		{
			m_OptionBtn.m_iBtnFlg = BTN_NORMAL;
			Action( GOOPTION );
		}
		if ( m_ExitBtn.GetState() == BTN_ACTION )
		{
			m_ExitBtn.m_iBtnFlg = BTN_NORMAL;
			PostMessage( hWnd, WM_CLOSE, 0, 0);
		}
		break;
			
	case WM_KEYDOWN:
		if(m_bShowMatrixList)
		{
			return;	
		}
		
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
		{
			if(m_IdBox.GetEnable() == TRUE)
			{
				m_IdBox.SetEnable(FALSE);
				m_PasswdBox.SetEnable(TRUE);
			}
			else if(wParam == VK_RETURN)
			{
				if (g_dwLangType == 1)
				{
					m_IdBox.CloseIMC();
					m_PasswdBox.CloseIMC();
				}
				Action(GOSELCHAR);
			}
			else
			{
				m_IdBox.SetEnable(TRUE);
				m_PasswdBox.SetEnable(FALSE);
			}
		}	

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if(m_bShowMatrixList)
		{
			return;
		}		

		m_IdBox.MsgProc( hWnd, msg, wParam, lParam );
		m_PasswdBox.MsgProc( hWnd, msg, wParam, lParam );
		break;
	}

}


int CLoginPage:: CheckID_PW()
{
	///////// 아이디 패스워드 입력에 관한 처리 ////////////
	if (strlen(m_IdBox.GetString()) == 0)
	{
		//-- IDS_LOGIN_INPUT_ID : ID를 입력하세요

		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_INPUT_ID));
		return 0;
	}
	else if(strlen(m_PasswdBox.GetString()) == 0)
	{
		//-- IDS_LOGIN_INPUT_PASS : PASSWORD를 입력하세요

		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_INPUT_PASS));
		return 0;
	}
	else if(CheckSpace(m_IdBox.GetString()) == TRUE || CheckSpace(m_PasswdBox.GetString()) == TRUE )
	{
		//-- IDS_LOGIN_UNABLE_BLANK_STR : ID/PASSWORD 는 공백을 허용하지 않습니다
		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_UNABLE_BLANK_STR));
		return 0;
	}
	else if(strinc(m_IdBox.GetString(), "'") || strinc(m_PasswdBox.GetString(), "'"))
	{
		//-- IDS_LOGIN_UNABLE_USE_STR : ID/PASSWORD 는 ' 를 허용하지 않습니다
		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_UNABLE_USE_STR));
		return 0;
	}

	int nMinCount_ID = 4, nMinCount_PWD = 4;
	int nMaxCount_ID = 20, nMaxCount_PWD = 20;
	if( g_dwLangType == CTRY_KOR )	// kor
	{
		nMinCount_ID = 3;
		nMaxCount_ID = 30;
		nMinCount_PWD = 4;
		nMaxCount_PWD = 20;
	}

	char strTemp[ 100 ] = "";
	if (strlen(m_IdBox.GetString()) < nMinCount_ID
			|| strlen(m_IdBox.GetString()) > nMaxCount_ID)
	{
		//-- IDS_LOGIN_ID_LIMIT : ID는 30자 이하입니다
		sprintf(strTemp, G_STRING(IDS_LOGIN_ID_LIMIT), nMaxCount_ID );
		m_MsgPopUp.PutString(strTemp);
		return 0;
	}
	else if(strlen(m_PasswdBox.GetString()) < nMinCount_PWD
			|| strlen(m_PasswdBox.GetString()) > nMaxCount_PWD)
	{
		//-- IDS_LOGIN_PASSWORD_LIMIT : PASSWORD는 %d자 이상  %d자 이하입니다.
		sprintf(strTemp, G_STRING(IDS_LOGIN_PASSWORD_LIMIT), nMinCount_PWD, nMaxCount_PWD );
		m_MsgPopUp.PutString(strTemp);
		return 0;
	}

	return 1;
}



int CLoginPage::ConnectServ(int idx)
{
	LoginProcess(idx, m_IdBox.GetString(), m_PasswdBox.GetString(), FALSE);

	if(m_pSheet)
	{

		m_pSheet->m_bEmptySloat = TRUE;
		for(int i=0; i<5; i++)
		{
			if(m_pSheet->m_Slot[i])
			{
				m_pSheet->m_bEmptySloat = FALSE;
			}
		}

		if( m_pSheet->m_bEmptySloat)
		{
			m_pSheet->m_pSelCharPg2->m_btn_start.SetDisable( TRUE );

			m_pSheet->m_pSelCharPg2->m_btn_new.SetDisable( FALSE );
			m_pSheet->m_pSelCharPg2->m_btn_delete.SetDisable(TRUE);
			m_pSheet->m_pSelCharPg2->m_btn_delete.m_iBtnFlg = BTN_DISABLE;

		}
		else
		{
			m_pSheet->m_pSelCharPg2->m_btn_start.SetDisable( TRUE );
			m_pSheet->m_pSelCharPg2->m_btn_new.SetDisable( TRUE );
			m_pSheet->m_pSelCharPg2->m_btn_new.SetState( BTN_DISABLE );
			m_pSheet->m_pSelCharPg2->m_btn_delete.SetDisable(TRUE);
			m_pSheet->m_pSelCharPg2->m_btn_start.m_iBtnFlg = BTN_DISABLE;
			m_pSheet->m_pSelCharPg2->m_btn_delete.m_iBtnFlg = BTN_DISABLE;
		}
		if(m_pSheet->m_pSelCharPg2)
		{
			m_pSheet->m_pSelCharPg2->m_selected_slot = -1;
			m_pSheet->m_pSelCharPg2->m_btn_start.m_iBtnFlg = BTN_DISABLE;
			m_pSheet->m_pSelCharPg2->m_btn_delete.m_iBtnFlg = BTN_DISABLE;
		}
	}

	if(1)
	{
		BOOL bResult = FALSE;
		FILE * fp = NULL;
		if((fp = fopen("Last.id", "rt")))
			bResult = TRUE;
		if(fp)
			fclose(fp);

		if(!m_bCheck && bResult)
		{
			// 유저가 아이디 저장을 원하지 않는다. 파일을 지운다.
			_unlink("Last.id");
		}
	}
	return 1;
}


void CLoginPage::Action( int i )
{
	int idx;
	char *str1, *str2;

	str1 = m_IdBox.GetString();
	str2 = m_PasswdBox.GetString();

	if (g_pDSound)
	{
		g_pDSound->Play(g_SheetSound[SHEET_SND_CLICK]);
	}
	switch ( i )
	{
	case GOSELCHAR:

		if(!CheckID_PW())
			return;

		/////////   서버연결에 관한처리  ////////////
		idx = m_ServerList->GetSelectedLine();

		if (idx < 0)
		{
			//-- IDS_LOGIN_SELECT_SERVER : 접속하실 서버를 선택해주세요.
			m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_SELECT_SERVER));
			return;
		}

		if( m_pMatrixSvr[idx]  > 0)
		{
			m_bShowMatrixList = TRUE;
			return;
		}

		switch( m_nSvrType[idx] ) ///신규서버리스트
		{
		case 0: // 일반 서버.
			g_SvrType = ST_12_OVER;
			break;
		case 1: // 매트릭스 서버
			g_SvrType = ST_ADULT_ONLY;
			break;
		case 2: // 매트릭스 대회 서버.
			g_SvrType = ST_ADULT_ONLY;
			break;
		case 3: // PK 성인서버.
			g_SvrType = ST_ADULT_ONLY;
			break;
// by evilkiki 2009.11.05 길드랭킹전
		case 5:
			g_SvrType = ST_GUILD_RANK;
			break;
		}

		if(!ConnectServ(idx))
			return;

		switch(m_iSvrMode[idx])
		{
		case eSVR_MASTER:
			g_SvrMode = eSVR_MASTER;
			break;
		case eSVR_NORMAL:
		default:
			g_SvrMode = eSVR_NORMAL;
			break;
		}

				
		break;

	case GOSELCHAR_MATRIX:
		{
			m_pSheet->m_nMatrixChaSvrIdx = GetSelectedLine_Matrix();
			if (m_pSheet->m_nMatrixChaSvrIdx < 0)
			{
				return;
			}		

			idx = m_ServerList->GetSelectedLine();

			if (idx < 0)
			{
				return;
			}

			ConnectServ(idx);	

			return;
		}
		break;

	case GOOPTION:
		m_pSheet->GoPage( OPTIONPAGE );
		return ;

	case GOMAKEACNT:
		//-- Lyul
		//-- 회원가입 주석처리.		m_pSheet->m_pMakeAccPg->FindPartners();

		// 한국통신 부분.
		m_pSheet->GoPage( MAKEACCPAGE);
		return;
	case GOLOGIN:
		// 매트릭스 서버로의 접속이 있을 수 있으므로 플래그를 FALSE로 놓는다.
		g_pTcpIp->SetTerminate();
		m_pSheet->m_pLoginPg->m_bShowMatrixList = FALSE;

		m_pSheet->GoPage( LOGINPAGE );
		return ;
	}
}

BOOL CLoginPage::LoginProcess(int nSvrIdx, char *strUserId, char *strUserPw, BOOL bAuto)
{
	m_pSheet->SetSvrAddr(m_dpServerAddr[nSvrIdx], atoi(m_nSvrPort[nSvrIdx]));

	g_MatrixSvr = m_pMatrixSvr[nSvrIdx];
	g_bMatrixMatch = m_pbMatrixMatch[nSvrIdx];
	
	bool bConfirm = m_bConfirm;
	if( CommonConfig::Instance()->GetLoginProcess() == false )
	{
		bConfirm = false;
	}

	if (m_pSheet->Connect(strUserId, strUserPw, bConfirm)==FALSE)
	{
		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_CONNECT_FAIL));	// 자동접속이 아니면 메시지 박스를 출력하지 않는다.
		m_PasswdBox.ClearText();

		return FALSE;
	}

	char	aServerNames[8][9] =
	{
		"갤럭시",		// 갤럭시	1
		"안드로",		// 안드로	2
		"오리온",		// 오리온	3
		"머큐리",		// 넷1		4
		"비너스",		// 넷2		5
		"쥬피터",		// 넷3		6
		"아케론",		// 아케론	7
		"헤르메스"		// 헤르메스 8 // 서버통합된 신섭 (오리온. 비너스, 쥬피터)
	};

	if( m_ServerList->GetString(nSvrIdx) ) // 서버명을 받아올수 있어야 한다.
	{
		if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[0], 6 ) == 0 )
			g_nServerIndex = 1;
		else if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[1], 6 ) == 0 )
			g_nServerIndex = 2;
		else if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[2], 8 ) == 0 )
			g_nServerIndex = 3;
		else if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[3], 6 ) == 0 )
			g_nServerIndex = 4;
		else if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[4], 6 ) == 0 )
			g_nServerIndex = 5;
		else if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[5], 6 ) == 0 )
			g_nServerIndex = 6;
		else if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[6], 6 ) == 0 )
			g_nServerIndex = 7;
		else if( strncmp( m_ServerList->GetString(nSvrIdx), aServerNames[7], 8 ) == 0 )
			g_nServerIndex = 8;

	}

	// 기존 위치에서하면 뻑남...
	g_pNk2DFrame->SetConnectServerName(m_ServerList->GetString(nSvrIdx));
	
	int ret = m_pSheet->UpdateChars();
	if( ret == -1 )
	{
		m_MsgPopUp.PutString((char*)G_STRING(IDS_UIMGR_BREAK_CONNECTION));
		return FALSE;
	}
	else if( ret == 0 )
	{
		if(m_pSheet->m_bIsChangeInfo)
		{
			m_GoHomePage.PutString("Discord Suport/News");
		}
		
		char* strTempBuf;
		char strTempBuf2[MAX_PATH] = {0,};
		strTempBuf = m_pSheet->AnyOneArg(m_pSheet->m_Buf, strTempBuf2);

		if( strncmp(strTempBuf2, "fail", 4) == 0 )
		{	
			if( strncmp(strTempBuf," check_join",strlen(" check_join")) == 0 )
			{			
				m_GoHomePage.PutString( (char*)G_STRING(IDS_WANT_LOGINKICK) );
			}
			else if( strncmp(strTempBuf," e0",strlen(" e0")) == 0 )
			{	
				m_ExitPopup.PutString( (char*)G_STRING(IDS_LHSTRING1890) );
			}
			else if( strncmp(strTempBuf," e1",strlen(" e1")) == 0 )
			{
				m_MsgPopUp.PutString((char*)G_STRING(IDS_LHSTRING1891));
			}
			else
			{	
				m_MsgPopUp.PutString(strTempBuf);
			}
		}

		if(g_MatrixSvr != 0)
		{
			Action(GOLOGIN);
			g_MatrixSvr = 0;
		}
		return FALSE;
	}

	// 케릭터 선택페이지로 이동하기전에 "현제 만들어진 케릭터가 존재하는지 체크하고"
	// 만약 만들어놓은 케릭터가 아무것도 없다면 "새케릭터 만들기"버튼을 활성화 시킨다
	if(m_pSheet->m_Slot[0] == NULL && m_pSheet->m_Slot[1] == NULL && m_pSheet->m_Slot[2] == NULL
			&& m_pSheet->m_Slot[3] == NULL && m_pSheet->m_Slot[4] == NULL)
	{
		m_pSheet->m_bEmptySloat = TRUE;
		m_pSheet->m_SlotNum = 2;		
		m_pSheet->ResetAllGate();
		m_pSheet->OpenNewGate(m_pSheet->m_SlotNum);
		

		if(g_MatrixSvr != 0)
		{
			char buf[256];
			if( g_dwClientCountry == CTRY_JPN)
			{
				sprintf(buf,"멗묖궢궫긖?긫?궸궼갂럊뾭궳궖귡긌긿깋긏??궕뫔띪궢귏궧귪갃"); // IDS_String 추가 요망
				if(g_bMatrixMatch)
					sprintf(buf,"볷듰먰먭뾭귺긇긂깛긣궴긬긚깗?긤궳먝뫏궢궲궘궬궠궋갃"); // IDS_String 추가
			}
			else
			{
				sprintf(buf,G_STRING(IDS_LHSTRING1777)); // IDS_String 추가 요망
				if(g_bMatrixMatch)
					sprintf(buf,G_STRING(IDS_LHSTRING1778)); // IDS_String 추가 요망
			}

			m_MsgPopUp.PutString(buf);
			
			Action(GOLOGIN);
			return TRUE;
		}		
	}

	m_pSheet->GoPage( SELCHARPAGE );
	strcpy(m_pSheet->m_sUserPW, strUserPw); // 로긴이 성공했다면 페스워드를 저장한다
	WriteLastId("Last.id");

	if (g_dwClientCountry != CTRY_MENG)
	{
		m_PasswdBox.SetEnable(TRUE);
		m_PasswdBox.ClearText();
	}

	// DummyMsg를 보내기 위한 세팅을 한다.
	m_pSheet->m_dwDummyBaseTime = timeGetTime();

	if( g_dev_client )
	{
		char buf[MAX_PATH] = {0,};
		sprintf(buf, "LaghaimOnline New [IP : %s][PORT : %d]", g_pTcpIp->GetIp().c_str(), g_pTcpIp->GetPort());		
		::SetWindowText(pCMyApp->Get_hWnd(), buf);
	}

	return TRUE;
}


BOOL CLoginPage::CheckSpace(char* String)
{
	if( String == NULL )
		return FALSE;

	int len = strlen(String);
	for(int i=0; i<len; i++)
	{
		if(is_space(String[i]) == TRUE)
			return TRUE;
	}
	return FALSE;
}



void CLoginPage::ReadLastId(char *file_name)
{
	FILE *fp = NULL;
	char id[LOGIN_NAME_LENGTH];

	if ( (fp = fopen(file_name, "rt")) == NULL)
		return;   // 읽을 파일이 존재하지 않는다면 그냥 통과..

	//[2009/4/15/ D.K ] : last ID  관련 버그 발생.
	fgets( id, LOGIN_NAME_LENGTH, fp );
	//fscanf(fp, "%s", id);		      // 파일에서 "id"를 뽑아 내고

	fclose(fp);
	m_IdBox.SetText(id);
}


void CLoginPage::WriteLastId(char *file_name)
{
	FILE *fp = NULL;

	if ( (fp = fopen(file_name, "w")) == NULL)
	{
		return;
	}
	//[2009/4/15/ D.K ] : last ID 저장시 쓰레기값 저장.
	fputs( m_IdBox.GetString(), fp );
	//fprintf(fp, "%s", m_IdBox.GetString());
	fclose(fp);
}




void CLoginPage::Read_URL_Add(char *filename)
{
	FILE *stream;
	char line[100] = {0,};

	if( (stream = fopen( filename, "rt" )) != NULL )
	{
		if( fgets( line, 100, stream ) == NULL)
			return;
		else
			printf( "%s", line);
		fclose( stream );
	}
	strcpy(m_AddRes, line);
}


BOOL CLoginPage::IsInside(int x, int y)
{
	if( !m_pCheck_on )
		return false;
	return m_pCheck_on->IsIn( x, y);
}
BOOL CLoginPage::IsNetInside(int x, int y)
{
	if (x >= m_ptLoginBackPos.x+133  && x < m_ptLoginBackPos.x+133+m_pNetCheck_on->GetWidth() &&
			y >= m_ptLoginBackPos.y+35 && y < m_ptLoginBackPos.y+35 + m_pNetCheck_on->GetHeight() )
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CLoginPage::IsValidIP(int nSvrIdx )
{
	char IpAddr[256];

	strcpy(IpAddr,m_dpServerAddr[nSvrIdx]);

	int descr_len = strlen(IpAddr)-1;

	while(IpAddr[descr_len] != '.')
	{
		IpAddr[descr_len] = '\0';
		descr_len--;
	}

	if(!strcmp(IpAddr, KOREA_IP)
			|| !strcmp(IpAddr, KOREA_IP_2012)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP2)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP3)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP4)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP5)
			|| !strcmp(IpAddr, JAPAN_IP)
			|| !strcmp(IpAddr, TAIWAN_IP)
			|| !strcmp(IpAddr, "219.238.187.")
	  )
		return	TRUE;

	if( g_dwClientCountry == CTRY_IDN
			|| g_dwClientCountry == CTRY_TH
			|| g_dwClientCountry == CTRY_ENG
			|| g_dwClientCountry == CTRY_TEN
			|| g_dwClientCountry == CTRY_JPN )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CLoginPage::IsValidIP(char* IpAddr)
{
	int descr_len = strlen(IpAddr)-1;

	while(IpAddr[descr_len] != '.')
	{
		IpAddr[descr_len] = '\0';
		descr_len--;
	}

	if (g_dwClientCountry >= CTRY_IDN)
		return TRUE;

	if( !strcmp(IpAddr, KOREA_IP)
			|| !strcmp(IpAddr, KOREA_IP_2012)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP2)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP3)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP4)
			|| !strcmp(IpAddr, KOREA_INTERNAL_IP5)
			|| !strcmp(IpAddr, JAPAN_IP)
			|| !strcmp(IpAddr, TAIWAN_IP)
			|| !strcmp(IpAddr, HONGKONG_IP)
			|| !strcmp(IpAddr, CHINA_BJ_IP_1)
			|| !strcmp(IpAddr, CHINA_BJ_IP_2)
			|| !strcmp(IpAddr, CHINA_SH_IP_1)
			|| !strcmp(IpAddr, CHINA_SH_IP_2)
			|| !strcmp(IpAddr, CHINA_SH_IP_3)
			|| !strcmp(IpAddr, CHINA_CD_IP_1)
			|| !strcmp(IpAddr, CHINA_CD_IP_2)
			|| !strcmp(IpAddr, INI_IP)
			|| !strcmp(IpAddr, CHINA_BJ_TEST)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW2)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW3)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW4)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW5)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW1)
			|| !strcmp(IpAddr, USA_IP_NEW)
			|| !strcmp(IpAddr, FRANCE_IP)
			|| !strcmp(IpAddr, BRASIL_IP_TEST)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW6)
			|| !strcmp(IpAddr, CHINA_BJ_IP_NEW7)
			|| !strcmp(IpAddr, GSP_TEST_IP)
	  )

		return	FALSE;

	return TRUE;
}

BOOL CLoginPage::GetMyIP()
{

	char host[100],ipName[20];
	HOSTENT* hostent;
	int cnt =0;


	if(gethostname(host,100) != 0)
	{
		return TRUE;
	}

	if((hostent=gethostbyname(host)) != NULL)
	{

		while(hostent->h_addr_list[cnt]!=NULL)
		{
			cnt++;
		}

		if(cnt < 2) //아이피를 하나만 가지고 있다면...
			return TRUE;

		for (int i=0; hostent->h_addr_list[i]; i++)
		{
			memcpy(ipName,inet_ntoa(*(struct in_addr *)hostent->h_addr_list[i]),sizeof(ipName));

			if(!IsValidIP(ipName))
				return FALSE;
		}
		return TRUE;
	}


	return FALSE;

}

void CLoginPage::RunCameraWork( DWORD dwPhaseElapsedTick, DWORD dwPhaseTotalTick ) // 스타트시의 카메라 워크를 함수화. 끝나면 TRUE 리턴. ///2007로딩변경

{
	D3DVECTOR vTemp1, vTemp2, vTemp3, vTemp4, vTemp5, vTemp6;
	float fTemp1, fTemp2;

	switch( m_nOpeningPhase ) // 4,5,6 단계에서만 카메라 워킹이 있다.
	{
	case 0:

		vTemp1 = D3DVECTOR( 1877.80f, 360.0f, 739.631f ); // 시작점.
		vTemp2 = D3DVECTOR( 1530.27f,390.0f,1099.11f ); // 목적지(문 위치)
		vTemp3 = vTemp2 - vTemp1; // 이동할 방향

		fTemp1 = Magnitude(vTemp3); // 이동할 거리
		vTemp3 = Normalize(vTemp3); // 정규화
		fTemp2 = fTemp1 / ((float)dwPhaseTotalTick); // 틱당 이동할 거리.

		m_pSheet->m_vEye = vTemp1 + ( (vTemp3*fTemp2) * (dwPhaseElapsedTick) ); // 시작점에서 진행된 틱만큼 이동시킨 위치.


		vTemp4 = m_pSheet->m_vEye; // 시작점.
		vTemp5 = D3DVECTOR( 1036.18f,650.0f-(0.13f*(float)dwPhaseElapsedTick),1610.20f ); // 목적지(석상 앞)
		vTemp6 = vTemp5 - vTemp4; // 이동할 방향

		vTemp6 = Normalize(vTemp6); // 정규화
		m_pSheet->m_vLookAt = m_pSheet->m_vEye + (vTemp6*10.0f); // 달려가는 방향을 바라보게. (여기 바라보는 방향이 조금 이상한듯.)

		break;

	case 1: // 일직선 달리기


		vTemp1 = D3DVECTOR(1530.27f,390.0f,1099.11f); // 시작점.
		vTemp2 = D3DVECTOR(1036.18f,390.0f,1610.20f); // 목적지(석상 앞)
		vTemp3 = vTemp2 - vTemp1; // 이동할 방향

		fTemp1 = Magnitude(vTemp3); // 이동할 거리
		vTemp3 = Normalize(vTemp3); // 정규화
		fTemp2 = fTemp1 / ((float)dwPhaseTotalTick); // 틱당 이동할 거리.

		m_pSheet->m_vEye = vTemp1 + ( (vTemp3*fTemp2) * dwPhaseElapsedTick ); // 시작점에서 진행된 틱만큼 이동시킨 위치.
		m_pSheet->m_vLookAt = m_pSheet->m_vEye + (vTemp3*10.0f); // 달려가는 방향을 바라보게.

		break;
	case 2: // 석상 올려다보기


		vTemp1 = D3DVECTOR(1530.27f,390.0f,1099.11f); // 시작점.
		vTemp2 = D3DVECTOR(912.652f,390.0f+((float)dwPhaseElapsedTick*0.27f),1737.97f); // 목적지(석상 앞)
		vTemp3 = vTemp2 - vTemp1; // 이동할 방향

		fTemp1 = Magnitude(vTemp3); // 이동할 거리
		vTemp3 = Normalize(vTemp3); // 정규화

		m_pSheet->m_vEye    = D3DVECTOR(1036.18f,390.0f,1610.20f);
		m_pSheet->m_vLookAt = m_pSheet->m_vEye + (vTemp3*10.0f); // 달려가는 방향을 바라보게.

		break;
	}

}




BOOL CLoginPage::PackageDecoding(char* strPack, char* strID, char* strPW)
{
	char *sCode = "partnerTEnterNakuGmFThypieFT";
	//int i;
	int n;
	int flag = 0;
	char decodingStr[500];
	int codePos[] = {2, 5, 8, 10, 11, 18, 19, 21, 25, 30, 51, 105, 300, 500, 1000};	// 해킹방지값 위치
	//1,4, 7, 10, 11, 13, 19, 21, 25, 30, 51, 105, 300, 500, 1000}; // 해킹방지값 위치
	int tmpPos = 0;	// 배열 위치
	int tmpStr = 1;	// 스트링 위치
	int str_len = 0;
	char hex[10];

	int cmd_len = strlen(strPack);
	*decodingStr = '\0';

	for (int i = 0; i < cmd_len; i++)
	{
		char tempA = 8;
		int hex_pos = 0;
		while(strPack[i] != 8)
		{
			hex[hex_pos++] = strPack[i++];
		}

		hex[hex_pos] = '\0';
		n = Hex2Dec(hex);

		if (n > 255)  // 이건 2바이트 한글이당.
		{
			n ^= sCode[flag];
			decodingStr[str_len] = (char) (n / 256);
			decodingStr[++str_len] = (char) (n % 256);
		}
		else
		{
			decodingStr[str_len] = (char) n;
			decodingStr[str_len] ^= sCode[flag];
		}
		decodingStr[++str_len] = '\0';
		if (tmpStr == codePos[tmpPos])
		{
			tmpPos++;
			i += 2;
		}

		tmpStr++;
		flag++;

		if (flag >= 28)
			flag = 0;
	}

	for (int i = 0; i < str_len; i++)
	{
		if (!strncmp(decodingStr+i, "|lastgame_stringOk|", 19))
		{
			decodingStr[i] = '\0';
			memcpy( strID, decodingStr, strlen(decodingStr));
			strcat( strPW, &decodingStr[i + 19]);
			break;
		}
	}
	return TRUE;
}

int CLoginPage::Hex2Dec(char *hex)
{
	int len = strlen(hex);
	int dec = 0;

	for (int i = 0; i < len; i++)
	{
		dec *= 16;
		if (hex[i] >= 'a' && hex[i] <= 'z')
		{
			dec += hex[i] - 'a' + 10;
		}
		else if (hex[i] >= 'A' && hex[i] <= 'Z')
		{
			dec += hex[i] - 'A' + 10;
		}
		else if (hex[i] >= '0' && hex[i] <= '9')
		{
			dec += hex[i] - '0';
		}
		else
		{
			return dec/16;
		}
	}

	return dec;
}

HRESULT CLoginPage::Restore()
{
	HRESULT hr = S_OK;

	m_IdBox.RestoreSurfaces();
	m_PasswdBox.RestoreSurfaces();
	if( m_ServerList )
		m_ServerList->Restore();

	return hr;
}