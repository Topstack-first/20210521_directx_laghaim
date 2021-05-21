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

// �� ����� ������ ƽ.
// (���� �������� ���� ƽ�̱⵵�ϴ�.)
// (��� ����Ǵ� ƽ���� �־���Կ� ����. ///2007�ε�����

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
	m_nSvrType		= NULL; ///�űԼ�������Ʈ
	m_pCheck_on     = NULL;
	m_pCheck_out    = NULL;
	//---------------------------------------------
	//�ݸ��� ���� üũ
	m_pNetCheck_on     = NULL;
	m_pNetCheck_out    = NULL;
	m_bNetCheck = FALSE;
	//---------------------------------------------

	m_pLoginBack = NULL;
	
	m_bCheck = TRUE;


	m_bIsAniEnd = TRUE;
	m_bAnimateBtn = TRUE;

	m_pGlobalMatBack = NULL;  ///���������� ����.

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
	//�ݸ��� ���� üũ
	m_pNetCheck_on     = NULL;
	m_pNetCheck_out    = NULL;
	//---------------------------------------------

	m_pLoginBack = NULL;	
	m_bCheck = TRUE;

	m_bIsAniEnd = FALSE;
	m_bAnimateBtn = TRUE;


	m_pMatrixList = NULL;
	m_pGlobalMatBack = NULL;  ///���������� ����.
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

	if (m_nSvrType) ///�űԼ�������Ʈ
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


	// �������̽� ��ǥ �Է�.
	m_ptLoginBackPos.x = LOING_BACK_X;
	m_ptLoginBackPos.y = LOING_BACK_Y;

	// Server List Box
	m_ServerList = new CListBox(" ", m_ptLoginBackPos.x+LOGIN_LIST_X, m_ptLoginBackPos.y+LOGIN_LIST_Y, LOGIN_LIST_WIDTH, LOGIN_LIST_HEIGHT);



	m_ServerList->SetImage( "common/scroll_dn_t01", "common/scroll_up_t01", "interface/login/serverlist_scbar.bmp" );
	if( g_dwClientCountry == CTRY_BRA ) // ������� ù��° ������ ����Ʈ�� ���õǾ��ְ� �ش޷���. ///07-01-31
	{
		m_ServerList->SetSelectedLine(0);
		m_ServerList->SetTextColor(RGB(255, 142, 103)); // ���� ���� ����Ʈ�� ������ �ٲܻ� ���� ���� ���� �ٲ����� �ʾƼ� ������� �� ����Ʈ�� ���� ���û����� �ٲ۴�. �ϴ� ������� ������ �ϳ��̱� ������...
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
	m_GoHomePage.Init(TYPE_NOR_OKCANCLE); ///�븸ű��� Ȯ�ο����� ���.(07-03-08 ����)

	m_ExitPopup.Init(TYPE_NOR_OK);
	
	m_bIsLoadingEnd  = FALSE;
	m_bOneTry        = FALSE;
	m_iCurFrame      = 0;
	m_dwStartTime    = 0;	

	m_nMatrix = -1;

	m_bKickLogin = FALSE; ///�븸ű���
}

BOOL CLoginPage::ReadServerList(char *filename)
{
	if( filename == NULL )
		return FALSE;

	FILE *fp = NULL;

	char buf[256];

	// ���� �̸�  , �ּ�
	char nick[100], addr[100], port[100];

	char *sentence=NULL;
	int i;

	fp = fopen(filename, "rt");
	assert(fp != NULL);

	sentence = GetSentence(buf, sizeof(buf), fp);

	sscanf(sentence, "%d", &m_nSvrAddrNum);

	// �޸� �Ҵ�

	// ���� m_nSvrAddrNum = '3'
	m_dpServerAddr = new char *[m_nSvrAddrNum];
	m_nSvrPort	   = new char *[m_nSvrAddrNum];
	m_pMatrixSvr  = new int [m_nSvrAddrNum];
	m_pbMatrixMatch= new BOOL [m_nSvrAddrNum];
	m_nSvrType	   = new int  [m_nSvrAddrNum]; ///�űԼ�������Ʈ


	for (i = 0; i < m_nSvrAddrNum; i++)
	{
		// ù��° �������� sentence�� �ִ´�
		sentence = GetSentence(buf, sizeof(buf), fp);
		/////0123 ��������Ʈ ��ȣȭ ����� ����...
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
		m_nSvrType[i] = 0; ///�űԼ�������Ʈ

		m_ServerList->AddString(nick);
	}

	fclose(fp);

	return TRUE;
}

BOOL CLoginPage::ReadServerListData(char *filename) //0215
{
	if( g_dwClientCountry == CTRY_CN ) // ����� �߱��� �� ��������Ʈ�� ����.
	{
		CWebWorld	webworld;

		char address[128] = {0,};

		switch( g_dwClientCountry )
		{
		case CTRY_KOR:
			sprintf(address, "http://laghaim.online/update/"); // �׽�Ʈ��.
			break;
		case CTRY_CN:
			sprintf(address, "http://laghaim.online/update/");
			break;
		default:
			return FALSE;
		}

		FILE *TestFile = NULL; // "config.ini2" ���Ϸ� üũ�ؼ� �ش� ������ ������ �ٸ� ��������Ʈ ������ �д´�.��ڿ� ��������Ʈ.

		TestFile = fopen( "config.ini3", "rb" );

		if( TestFile )
		{
			strcat( address, "SvrList.dtat" ); // ��ڿ� ��������Ʈ�� �д´�.
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
			if( !webworld.m_UrlData || strlen(webworld.m_UrlData) <= 0 ) // �� ���� ���� �߸��� ��.
			{
				return FALSE;
			}

			char buf[256];

			// ���� �̸�  , �ּ�
			char nick[100], addr[100], port[100], matrix[10];

			int i,j;
			unsigned char key;
			int iKey, iKeyTemp;

			// �� 6����Ʈ�� ����.
			j = 6;
			memcpy( &iKey, &webworld.m_UrlData[j], sizeof(int) ); // ��ȣȭ Ű.
			j += sizeof(int);
			memcpy( &key, &webworld.m_UrlData[j], sizeof(BYTE) ); // ��ȣȭ Ű.
			j += sizeof(BYTE);
			memcpy( &m_nSvrAddrNum, &webworld.m_UrlData[j], sizeof(int) ); // ��������Ʈ�� ����.
			j += sizeof(int);

			iKeyTemp = m_nSvrAddrNum;
			m_nSvrAddrNum -= iKey;
			iKey = iKeyTemp;

			// �޸� �Ҵ�

			m_dpServerAddr = new char *[m_nSvrAddrNum];
			m_nSvrPort	   = new char *[m_nSvrAddrNum];
			m_pMatrixSvr  = new int [m_nSvrAddrNum];
			m_pbMatrixMatch= new BOOL [m_nSvrAddrNum];
			m_nSvrType	   = new int [m_nSvrAddrNum]; ///�űԼ�������Ʈ

			for (i = 0; i < m_nSvrAddrNum; i++)
			{
				// ù��° �������� buf�� �ִ´�.
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

								// ���Ӵ�ȸ �������� Ȯ���Ѵ�.
								if (m_nMatrix == 2)
									m_pbMatrixMatch[i] = TRUE;
								else
									m_pbMatrixMatch[i] = FALSE;

				  */

				// ���� ��Ʈ ������ ���� ���� Ÿ���� �����صд�. (�� 1,2 Ÿ���� �׳� ���� ��ƾ �״�� �д�.)
				m_nSvrType[i] = m_nMatrix; ///�űԼ�������Ʈ
			}
		}
	}
	else
	{
		FILE *fp = NULL;

		char buf[256] = {0, };

		// ���� �̸�  , �ּ�
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

		// �޸� �Ҵ�


		// ���� m_nSvrAddrNum = '3'
		m_dpServerAddr = new char *[m_nSvrAddrNum];
		m_nSvrPort	   = new char *[m_nSvrAddrNum];
		m_pMatrixSvr   = new int [m_nSvrAddrNum];
		m_pbMatrixMatch= new BOOL [m_nSvrAddrNum];
		m_nSvrType	   = new int [m_nSvrAddrNum]; ///�űԼ�������Ʈ

		m_iSvrMode	   = new int [m_nSvrAddrNum];

		for (i = 0; i < m_nSvrAddrNum; i++)
		{
			// ù��° �������� buf�� �ִ´�.
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
// by evilkiki 2009.11.05 ��巩ŷ�� ( ip port value : value = 5 ������)
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

						// ���Ӵ�ȸ �������� Ȯ���Ѵ�.
						if (m_nMatrix == 2)
							m_pbMatrixMatch[i] = TRUE;
						else
							m_pbMatrixMatch[i] = FALSE;
			*/
			// ���� ��Ʈ ������ ���� ���� Ÿ���� �����صд�. (�� 1,2 Ÿ���� �׳� ���� ��ƾ �״�� �д�.)
			m_nSvrType[i] = m_nMatrix; ///�űԼ�������Ʈ
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

	m_LoginBtn.SetPosition( m_pLoginBack->Xpos+310, m_pLoginBack->Ypos+520 );     // ����
	m_OptionBtn.SetPosition( m_pLoginBack->Xpos+680, m_pLoginBack->Ypos+520 );	    // ����
	m_ExitBtn.SetPosition( m_pLoginBack->Xpos+680, m_pLoginBack->Ypos+550 );       // ����



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

	// ��Ʈ������ ���ӽ� �����ϴ� ��������Ʈ�� ��Ʈ���� txt���� �о������ ����(05-08-10 by ����)
	if( g_bUseMatrixCountry ) // ����콺�� ���Ե� ���� ó��...
	{
		PFILE *fp;
		int MatSvrNum = 0;
		char TempString[255];

		fp = pfopen( "data/mat_svr.txt", "rt" );

		if( fp )
		{
			pfscanf( fp, "%d \n", &MatSvrNum ); // ������ ����.

			for( int i = 0 ; i < MatSvrNum ; ++i )
			{
				PReadLine( fp, TempString ); // �׳� fgets�� ����ϸ� �ѱ��� �ڿ� ���� ���� ���� ������ �־ ��ų.dta���� ���̴� �����б� �Լ��� ������ ���.

				if( i != MatSvrNum-1 ) // ������ �༮�� �ƴϸ�...
					AddMatrixList( TempString );
				else // ������ �༮�� "���"��.
					AddMatrixList( TempString, TRUE );	// ��Ҵ� 3��°���ڸ� TRUE�� �ִ���...
			}

			pfclose( fp );
		}
		else // ������ ���� ���.
			MessageBox( NULL, "[mat_svr.txt] File Not Found!", "Error", MB_OK );

		///���������� �� ��� �ε�

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
	// ���� �κ�
	m_IdBox.DeleteRes();
	m_PasswdBox.DeleteRes();
	//

	if (m_ServerList)
	{
		m_ServerList->DeleteRes();
	}

	m_MsgPopUp.DeleteRes();
	m_GoHomePage.DeleteRes(); ///�븸ű��� Ȯ�ο����� ���.(07-03-08 ����)
	m_ExitPopup.DeleteRes();
	SAFE_DELETE(m_pGlobalMatBack);  ///���������� ����.

	SAFE_DELETE(m_pCheck_on);
	SAFE_DELETE(m_pCheck_out);
	//---------------------------------------------
	//�ݸ��� ���� üũ
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
			m_nOpeningPhase = 0; // ���̵� �ƿ� & ȸ�� �ΰ� ���̵���(����)
		else if( m_dwPhaseTick >= g_PhaseEndTick[1] && m_dwPhaseTick <= g_PhaseEndTick[2] )
			m_nOpeningPhase = 1; // ȸ�� �ΰ� ���̵�ƿ� & ���̵���
		else if( m_dwPhaseTick >= g_PhaseEndTick[2] && m_dwPhaseTick <= g_PhaseEndTick[3] )
			m_nOpeningPhase = 2; // ���������� & ���̵� �ƿ�
		else
		{
			m_bIsAniEnd = TRUE;
			m_bAnimateBtn = TRUE;

			m_dwStartTime = timeGetTime();			// ��ư �ִϸ��̼��� ���� ���۽ð� ���.
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

		m_LoginBtn.Draw( m_pLoginBack->Xpos+313, m_pLoginBack->Ypos+515 );     // ����
		m_OptionBtn.Draw( m_pLoginBack->Xpos+655, m_pLoginBack->Ypos+515 );	    // ����
		m_ExitBtn.Draw( m_pLoginBack->Xpos+655, m_pLoginBack->Ypos+545 );       // ����
				
		if(m_bCheck)
			g_pDisplay->Blt( m_pCheck_on->Xpos, m_pCheck_on->Ypos, m_pCheck_on);
		else
			g_pDisplay->Blt( m_pCheck_out->Xpos, m_pCheck_out->Ypos, m_pCheck_out);
		
		m_ServerList->CalculateBarPos2( 18 ); // ��ũ�ѹ��� ��ġ�� ������ ����.(���� ���� ��)
		m_ServerList->Draw();		
		
		m_IdBox.Draw(m_pLoginBack->Xpos+345, m_pLoginBack->Ypos+392 , 106 );
		m_PasswdBox.Draw(m_pLoginBack->Xpos+345, m_pLoginBack->Ypos+448, 106 );
		
		m_MsgPopUp.Draw();


		m_GoHomePage.Draw(); ///�븸ű��� Ȯ�ο����� ���.(07-03-08 ����)

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


	m_ServerList->GetMessage(hWnd, msg, wParam, lParam); // �α������������� �̳༮�� ���ϰ��� �˻��� �ʿ䰡����.

	switch ( msg )
	{
	case WM_RBUTTONUP: // ���콺 ������ ��ư���� ��� ���� ��������Ʈ ���÷��� ��� ����( 05-10-29 ����)

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( g_bAdmin ) // ��� �Ǵ��� �޴��ٷθ� �Ѵ�.(���������� �翬�� �ȵ���;;)
		{
			DeleteRes(); // ���ǽ��� ���� �������ϴ� �켱�����.

			if( m_ServerList ) // Init()���� new�ϴ� �̸� ��������Ѵ�.
			{
				delete m_ServerList;
				m_ServerList = NULL;
			}

			Init(); // ���⼭ �ε��ϴ� �̰� ��������Ѵ�.
			LoadRes(); // �̰������ ����Ʈ ���ǽ��� ���� ����.
		}

		break;

	case WM_LBUTTONDOWN:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_bIsLoadingEnd && !m_bIsAniEnd ) // ���� �����׽��� ���� ���̾��ٸ�... ///2007�ε���
			{
				// ������ �� ��ŵ ó��.
				m_bIsAniEnd = TRUE;
				m_bAnimateBtn = TRUE;
				m_dwStartTime = timeGetTime();			// ��ư �ִϸ��̼��� ���� ���۽ð� ���.

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
	///////// ���̵� �н����� �Է¿� ���� ó�� ////////////
	if (strlen(m_IdBox.GetString()) == 0)
	{
		//-- IDS_LOGIN_INPUT_ID : ID�� �Է��ϼ���

		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_INPUT_ID));
		return 0;
	}
	else if(strlen(m_PasswdBox.GetString()) == 0)
	{
		//-- IDS_LOGIN_INPUT_PASS : PASSWORD�� �Է��ϼ���

		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_INPUT_PASS));
		return 0;
	}
	else if(CheckSpace(m_IdBox.GetString()) == TRUE || CheckSpace(m_PasswdBox.GetString()) == TRUE )
	{
		//-- IDS_LOGIN_UNABLE_BLANK_STR : ID/PASSWORD �� ������ ������� �ʽ��ϴ�
		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_UNABLE_BLANK_STR));
		return 0;
	}
	else if(strinc(m_IdBox.GetString(), "'") || strinc(m_PasswdBox.GetString(), "'"))
	{
		//-- IDS_LOGIN_UNABLE_USE_STR : ID/PASSWORD �� ' �� ������� �ʽ��ϴ�
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
		//-- IDS_LOGIN_ID_LIMIT : ID�� 30�� �����Դϴ�
		sprintf(strTemp, G_STRING(IDS_LOGIN_ID_LIMIT), nMaxCount_ID );
		m_MsgPopUp.PutString(strTemp);
		return 0;
	}
	else if(strlen(m_PasswdBox.GetString()) < nMinCount_PWD
			|| strlen(m_PasswdBox.GetString()) > nMaxCount_PWD)
	{
		//-- IDS_LOGIN_PASSWORD_LIMIT : PASSWORD�� %d�� �̻�  %d�� �����Դϴ�.
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
			// ������ ���̵� ������ ������ �ʴ´�. ������ �����.
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

		/////////   �������ῡ ����ó��  ////////////
		idx = m_ServerList->GetSelectedLine();

		if (idx < 0)
		{
			//-- IDS_LOGIN_SELECT_SERVER : �����Ͻ� ������ �������ּ���.
			m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_SELECT_SERVER));
			return;
		}

		if( m_pMatrixSvr[idx]  > 0)
		{
			m_bShowMatrixList = TRUE;
			return;
		}

		switch( m_nSvrType[idx] ) ///�űԼ�������Ʈ
		{
		case 0: // �Ϲ� ����.
			g_SvrType = ST_12_OVER;
			break;
		case 1: // ��Ʈ���� ����
			g_SvrType = ST_ADULT_ONLY;
			break;
		case 2: // ��Ʈ���� ��ȸ ����.
			g_SvrType = ST_ADULT_ONLY;
			break;
		case 3: // PK ���μ���.
			g_SvrType = ST_ADULT_ONLY;
			break;
// by evilkiki 2009.11.05 ��巩ŷ��
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
		//-- ȸ������ �ּ�ó��.		m_pSheet->m_pMakeAccPg->FindPartners();

		// �ѱ���� �κ�.
		m_pSheet->GoPage( MAKEACCPAGE);
		return;
	case GOLOGIN:
		// ��Ʈ���� �������� ������ ���� �� �����Ƿ� �÷��׸� FALSE�� ���´�.
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
		m_MsgPopUp.PutString((char*)G_STRING(IDS_LOGIN_CONNECT_FAIL));	// �ڵ������� �ƴϸ� �޽��� �ڽ��� ������� �ʴ´�.
		m_PasswdBox.ClearText();

		return FALSE;
	}

	char	aServerNames[8][9] =
	{
		"������",		// ������	1
		"�ȵ��",		// �ȵ��	2
		"������",		// ������	3
		"��ť��",		// ��1		4
		"��ʽ�",		// ��2		5
		"������",		// ��3		6
		"���ɷ�",		// ���ɷ�	7
		"�츣�޽�"		// �츣�޽� 8 // �������յ� �ż� (������. ��ʽ�, ������)
	};

	if( m_ServerList->GetString(nSvrIdx) ) // �������� �޾ƿü� �־�� �Ѵ�.
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

	// ���� ��ġ�����ϸ� ����...
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

	// �ɸ��� ������������ �̵��ϱ����� "���� ������� �ɸ��Ͱ� �����ϴ��� üũ�ϰ�"
	// ���� �������� �ɸ��Ͱ� �ƹ��͵� ���ٸ� "���ɸ��� �����"��ư�� Ȱ��ȭ ��Ų��
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
				sprintf(buf,"�I�������T?�o?�ɂ́A�g�p�ł���L�����N??�����݂��܂���B"); // IDS_String �߰� ���
				if(g_bMatrixMatch)
					sprintf(buf,"���ؐ��p�A�J�E���g�ƃp�X��?�h�Őڑ����Ă��������B"); // IDS_String �߰�
			}
			else
			{
				sprintf(buf,G_STRING(IDS_LHSTRING1777)); // IDS_String �߰� ���
				if(g_bMatrixMatch)
					sprintf(buf,G_STRING(IDS_LHSTRING1778)); // IDS_String �߰� ���
			}

			m_MsgPopUp.PutString(buf);
			
			Action(GOLOGIN);
			return TRUE;
		}		
	}

	m_pSheet->GoPage( SELCHARPAGE );
	strcpy(m_pSheet->m_sUserPW, strUserPw); // �α��� �����ߴٸ� �佺���带 �����Ѵ�
	WriteLastId("Last.id");

	if (g_dwClientCountry != CTRY_MENG)
	{
		m_PasswdBox.SetEnable(TRUE);
		m_PasswdBox.ClearText();
	}

	// DummyMsg�� ������ ���� ������ �Ѵ�.
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
		return;   // ���� ������ �������� �ʴ´ٸ� �׳� ���..

	//[2009/4/15/ D.K ] : last ID  ���� ���� �߻�.
	fgets( id, LOGIN_NAME_LENGTH, fp );
	//fscanf(fp, "%s", id);		      // ���Ͽ��� "id"�� �̾� ����

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
	//[2009/4/15/ D.K ] : last ID ����� �����Ⱚ ����.
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

		if(cnt < 2) //�����Ǹ� �ϳ��� ������ �ִٸ�...
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

void CLoginPage::RunCameraWork( DWORD dwPhaseElapsedTick, DWORD dwPhaseTotalTick ) // ��ŸƮ���� ī�޶� ��ũ�� �Լ�ȭ. ������ TRUE ����. ///2007�ε�����

{
	D3DVECTOR vTemp1, vTemp2, vTemp3, vTemp4, vTemp5, vTemp6;
	float fTemp1, fTemp2;

	switch( m_nOpeningPhase ) // 4,5,6 �ܰ迡���� ī�޶� ��ŷ�� �ִ�.
	{
	case 0:

		vTemp1 = D3DVECTOR( 1877.80f, 360.0f, 739.631f ); // ������.
		vTemp2 = D3DVECTOR( 1530.27f,390.0f,1099.11f ); // ������(�� ��ġ)
		vTemp3 = vTemp2 - vTemp1; // �̵��� ����

		fTemp1 = Magnitude(vTemp3); // �̵��� �Ÿ�
		vTemp3 = Normalize(vTemp3); // ����ȭ
		fTemp2 = fTemp1 / ((float)dwPhaseTotalTick); // ƽ�� �̵��� �Ÿ�.

		m_pSheet->m_vEye = vTemp1 + ( (vTemp3*fTemp2) * (dwPhaseElapsedTick) ); // ���������� ����� ƽ��ŭ �̵���Ų ��ġ.


		vTemp4 = m_pSheet->m_vEye; // ������.
		vTemp5 = D3DVECTOR( 1036.18f,650.0f-(0.13f*(float)dwPhaseElapsedTick),1610.20f ); // ������(���� ��)
		vTemp6 = vTemp5 - vTemp4; // �̵��� ����

		vTemp6 = Normalize(vTemp6); // ����ȭ
		m_pSheet->m_vLookAt = m_pSheet->m_vEye + (vTemp6*10.0f); // �޷����� ������ �ٶ󺸰�. (���� �ٶ󺸴� ������ ���� �̻��ѵ�.)

		break;

	case 1: // ������ �޸���


		vTemp1 = D3DVECTOR(1530.27f,390.0f,1099.11f); // ������.
		vTemp2 = D3DVECTOR(1036.18f,390.0f,1610.20f); // ������(���� ��)
		vTemp3 = vTemp2 - vTemp1; // �̵��� ����

		fTemp1 = Magnitude(vTemp3); // �̵��� �Ÿ�
		vTemp3 = Normalize(vTemp3); // ����ȭ
		fTemp2 = fTemp1 / ((float)dwPhaseTotalTick); // ƽ�� �̵��� �Ÿ�.

		m_pSheet->m_vEye = vTemp1 + ( (vTemp3*fTemp2) * dwPhaseElapsedTick ); // ���������� ����� ƽ��ŭ �̵���Ų ��ġ.
		m_pSheet->m_vLookAt = m_pSheet->m_vEye + (vTemp3*10.0f); // �޷����� ������ �ٶ󺸰�.

		break;
	case 2: // ���� �÷��ٺ���


		vTemp1 = D3DVECTOR(1530.27f,390.0f,1099.11f); // ������.
		vTemp2 = D3DVECTOR(912.652f,390.0f+((float)dwPhaseElapsedTick*0.27f),1737.97f); // ������(���� ��)
		vTemp3 = vTemp2 - vTemp1; // �̵��� ����

		fTemp1 = Magnitude(vTemp3); // �̵��� �Ÿ�
		vTemp3 = Normalize(vTemp3); // ����ȭ

		m_pSheet->m_vEye    = D3DVECTOR(1036.18f,390.0f,1610.20f);
		m_pSheet->m_vLookAt = m_pSheet->m_vEye + (vTemp3*10.0f); // �޷����� ������ �ٶ󺸰�.

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
	int codePos[] = {2, 5, 8, 10, 11, 18, 19, 21, 25, 30, 51, 105, 300, 500, 1000};	// ��ŷ������ ��ġ
	//1,4, 7, 10, 11, 13, 19, 21, 25, 30, 51, 105, 300, 500, 1000}; // ��ŷ������ ��ġ
	int tmpPos = 0;	// �迭 ��ġ
	int tmpStr = 1;	// ��Ʈ�� ��ġ
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

		if (n > 255)  // �̰� 2����Ʈ �ѱ��̴�.
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