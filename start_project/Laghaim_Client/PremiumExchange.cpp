#include "stdafx.h"
#include "headers.h"
#include "Nk2DFrame.h"
#include "main.h"
#include "UIMgr.h"
#include "NKCharacter.h"
#include "BasicButton.h"
#include "TcpIpCon.h"
#include "Land.h"
#include "ControlBottom.h"
#include "g_stringmanager.h"
#include "PremiumExchange.h"

#include "createcharacterpage.h"
#include "LHTextChecker.h"

extern LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam);
extern HHOOK	g_hHook;
extern DWORD	g_dwClientCountry;

CPremiumExchange::CPremiumExchange()
	: m_nSlot(0)
{
	m_nType = 0;
	m_pBack = NULL;
	m_nScreenWidth = NULL;
	m_nScreenHeight = NULL;

	m_pBackSource[0] = NULL;
	m_pBackSource[1] = NULL;

	m_pszProperList = NULL;
	m_nProperListCount = 0;
}

CPremiumExchange::~CPremiumExchange()
{
	DeleteRes();

	for(int i = 0 ; i < m_nProperListCount ; i ++ )
		SAFE_DELETE_ARRAY(m_pszProperList[i]);
	SAFE_DELETE_ARRAY(m_pszProperList);

	m_pBack = NULL;
}

void CPremiumExchange::SetInfo(int nType, int nSlot)
{
	m_nType = nType;
	m_nSlot = nSlot;
	m_pBack = m_pBackSource[nType];
	m_boxName.ClearText();
}

int  CPremiumExchange::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return FALSE;

	SetDrawPos();

	// ���� �׸���
	g_pDisplay->Blt( m_pBack->Xpos, m_pBack->Ypos, m_pBack );

	m_btOk.Draw();
	m_btCancel.Draw();

	m_boxName.Draw();

	return FALSE;
}

BOOL CPremiumExchange::CheckGuildName(char* string)
{
	// ���ں� �˻�
	int len = strlen(string);

	if ( len > 10*2 || len < 2*2 )
	{
		// IDS_GUILD_CREATE_DONT_STRLEN ������ �ѱ� 2~10��, ���� 4~20�ڷ� �Է����ּ���."
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_DONT_STRLEN), TYPE_NOR_OK );
		return FALSE;
	}

	for(int i=0; i<len; i++)
	{
		if(string[i] == *" ")
		{
			// IDS_CANNOT_SPACE "���鹮�ڴ� ������� �ʽ��ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CANNOT_SPACE), TYPE_NOR_OK, 1);
			return FALSE;
		}
	}

	//'%s' ���� ���͸�
	for(int i = 0; i < (len-1); ++i)
	{
		if( string[i] == *"%" && (string[i] == *"s" || string[i] == *"S" ))
		{
			// IDS_GUILD_CREATE_DONT_WORD "���� %'%s%'���ڸ� ������� �ʽ��ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_CREATE_DONT_WORD), TYPE_NOR_OK, 1);
			return FALSE;
		}
	}


	{
		//Ư�� ���� ���͸�
		char* found_sp = NULL;
		char msgStr[100] = "";

		if((found_sp = ExamSpCharOne(string)) != NULL)
		{
			// IDS_PREMIUM_MSG_30 ���� Ư�� ����, ���ڸ� ������� �ʽ��ϴ�.
			// sprintf(msgStr, "���� Ư�� ���ڸ� ������� �ʽ��ϴ�", found_sp);
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_30), TYPE_NOR_OK, 1);
			return FALSE;
		}

		if((found_sp = ExamSpChar(string)) != NULL)
		{
			// IDS_PREMIUM_MSG_30 ���� Ư�� ����, ���ڸ� ������� �ʽ��ϴ�.
			// sprintf(msgStr, "���� Ư�� ���ڸ� ������� �ʽ��ϴ�", found_sp);
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_30), TYPE_NOR_OK, 1);
			return FALSE;
		}

		if(ExamNumOnly(string) == true)
		{
			// IDS_PREMIUM_MSG_30 ���� Ư�� ����, ���ڸ� ������� �ʽ��ϴ�.
			// sprintf(msgStr, "���� ���ڸ� ������� �ʽ��ϴ�.", found_sp);
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_30), TYPE_NOR_OK, 1);
			return FALSE;
		}

		for ( int i = 0; i < m_nProperListCount; i++)
		{
			strcpy(msgStr, string);
			strlwr(msgStr);	// �ҹ��ڷ� ��ȯ

			if( Str2Str( msgStr, m_pszProperList[i] ) )
				//		if(strstr(msgStr,m_pszProperList[i]))
			{
				Sleep(700);
				// IDS_CRECHA_UNABLE_STRING_02 : ĳ���� �̸��� "%s" �� ������� �ʽ��ϴ�
				sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), m_pszProperList[i]);
				g_pNk2DFrame->InsertPopup(msgStr, TYPE_NOR_OK, 1);
				return FALSE;
			}
		}
	}

	//���� ��� ���͸� - ������ �ʿ����
	return TRUE;
}


BOOL CPremiumExchange::CheckCharName(char* string)
{
	// ���ں� �˻�
	int len = strlen(string);

	if(len < 4 || len > 12)
	{
		// IDS_PREMIUM_MSG_31 ĳ���� �̸��� �ѱ� 2~6��, ���� 4~12�ڷ� �Է����ּ���.
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_31), TYPE_NOR_OK, 1);
		return FALSE;
	}

	for(int i=0; i<len; i++)
	{
		if(string[i] == *" ")
		{
			// IDS_CANNOT_SPACE "���鹮�ڴ� ������� �ʽ��ϴ�."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CANNOT_SPACE), TYPE_NOR_OK, 1);
			return FALSE;
		}
	}

	//'%s' ���� ���͸�
	for( int i = 0; i < (len-1); ++i)
	{
		if( string[i] == *"%" && (string[i] == *"s" || string[i] == *"S" ))
		{
			// IDS_PREMIUM_MSG_33 ����� �� ���� ���ڰ�  ���ԵǾ� �ֽ��ϴ�.
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_33), TYPE_NOR_OK, 1);
			return FALSE;
		}
	}



	//Ư�� ���� ���͸�
	{
		char* found_sp = NULL;
		char msgStr[100] = "";

		if((found_sp = ExamSpCharOne(string)) != NULL)
		{
			// IDS_PREMIUM_MSG_32 ĳ���� �̸��� Ư�� ����, ���ڸ� ������� �ʽ��ϴ�.
			//sprintf(msgStr, "ĳ���� �̸��� Ư�� ���ڸ� ������� �ʽ��ϴ�", found_sp);
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_32), TYPE_NOR_OK, 1);
			return FALSE;
		}

		if((found_sp = ExamSpChar(string)) != NULL)
		{
			// IDS_PREMIUM_MSG_32 ĳ���� �̸��� Ư�� ����, ���ڸ� ������� �ʽ��ϴ�..
			//sprintf(msgStr, "ĳ���� �̸��� Ư�� ���ڸ� ������� �ʽ��ϴ�", found_sp);
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_32), TYPE_NOR_OK, 1);
			return FALSE;
		}

		if(ExamNumOnly(string) == true)
		{
			// IDS_PREMIUM_MSG_32 ĳ���� �̸��� Ư�� ����, ���ڸ� ������� �ʽ��ϴ�.
			//sprintf(msgStr, "���ڸ� ���յ� ĳ���� �̸��� ������� �ʽ��ϴ�.", found_sp);
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PREMIUM_MSG_32), TYPE_NOR_OK, 1);
			return FALSE;
		}

		for ( int i = 0; i < m_nProperListCount; i++)
		{
			strcpy(msgStr, string);
			strlwr(msgStr);	// �ҹ��ڷ� ��ȯ

			if( Str2Str( msgStr, m_pszProperList[i] ) )
				//		if(strstr(msgStr,m_pszProperList[i]))
			{
				Sleep(700);
				// IDS_CRECHA_UNABLE_STRING_02 : ĳ���� �̸��� "%s" �� ������� �ʽ��ϴ�
				sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), m_pszProperList[i]);
				g_pNk2DFrame->InsertPopup(msgStr, TYPE_NOR_OK, 1);
				return FALSE;
			}
		}
	}

	//���� ��� ���͸� - ������ �ʿ����
	return TRUE;
}

void CPremiumExchange::SetDrawPos()
{
	if( !g_pNk2DFrame || !m_pBack )
		return;

	int i = 0;
	// �������̽� �����찡 ���� �������� üũ
	if( g_pNk2DFrame->IsHelpVisible()			|| g_pNk2DFrame->IsInvenVisible()
			|| g_pNk2DFrame->IsPartyVisible()		|| g_pNk2DFrame->IsInterfaceVisible(SCORE)
			|| g_pNk2DFrame->IsSkillVisible()		|| g_pNk2DFrame->IsInterfaceVisible(MAP)
			|| g_pNk2DFrame->IsPetStatusVisible()	|| g_pNk2DFrame->IsInterfaceVisible(QUEST) )
		m_pBack->Xpos = abs( m_nScreenWidth - (int)m_pBack->GetWidth() - 274 )  / 2;	// 274�� �������̽� ũ��
	else
		m_pBack->Xpos = abs( m_nScreenWidth - (int)m_pBack->GetWidth() ) / 2;

	m_pBack->Ypos = abs( m_nScreenHeight - (int)m_pBack->GetHeight() ) / 2 - 15;

	// ���� ���� ��� ��ǥ ����
	SetStringPos();
	// ��ư ��ǥ ����
	SetButtonPos();
}

void CPremiumExchange::SetStringPos()
// �ؽ�Ʈ ��� �ڽ� ��ǥ ����
{
	if( !m_pBack )
		return;
}

void CPremiumExchange::SetButtonPos()
// �� ��ư ��ǥ ����
{
	// Ȯ�� ��ư
	m_btOk.SetPosition( m_pBack->Xpos + 56, m_pBack->Ypos + 136 );
	// ���� ��ư
	m_btCancel.SetPosition( m_pBack->Xpos + 154, m_pBack->Ypos + 136 );

	m_boxName.SetPos(m_pBack->Xpos + 125, m_pBack->Ypos + 94);
}

// ���콺�� ������ �ȿ� ��� �Դ��� üũ
BOOL CPremiumExchange::IsInside( int x, int y )
{
	if( !m_pBack )
		return false;

	return m_pBack->IsIn(x, y);
}

void CPremiumExchange::LoadRes()
{
	switch( g_dwClientCountry )
	{
	case 0: // CRTY_KOR
	case 7: // CTRY_TH
		ReadProperList("proper.nam");
	}

	int i = 0;

	if( !g_pDisplay)
		return;

	RECT rc;
	HWND hWnd = g_pDisplay->GetHWnd();
	GetClientRect( hWnd, &rc );

	m_nScreenWidth = rc.right;
	m_nScreenHeight = rc.bottom;

	SetDrawPos();

	// �����̹��� �ε�
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSource[1], "interface/premium/charname_exchage.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSource[0], "interface/premium/guildname_exchage.bmp");
	m_pBackSource[1]->SetColorKey(TRANS_COLOR);
	m_pBackSource[0]->SetColorKey(TRANS_COLOR);

	m_pBack = m_pBackSource[0];

	m_btOk.SetFileName("premium/ChaMove/ok");
	m_btOk.LoadRes();

	m_btCancel.SetFileName("premium/ChaMove/cancel");;
	m_btCancel.LoadRes();

	m_boxName.Init(m_pBack->Xpos + 112, m_pBack->Ypos + 84, 20, RGB(255,255,255), RGB(255, 179, 0), FALSE, MSG_POPUP);
	m_boxName.SetfontSize();

	m_boxName.SetEnable(FALSE);
}

void CPremiumExchange::DeleteRes()
{
	m_boxName.CloseIMC();

	SAFE_DELETE(m_pBackSource[0]);
	SAFE_DELETE(m_pBackSource[1]);

//	SAFE_DELETE(m_pBack);

	m_btOk.DeleteRes();
	m_btCancel.DeleteRes();
	m_boxName.DeleteRes();
}

// �޼��� ó��
LRESULT CPremiumExchange::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	if( m_boxName.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_boxName.SetEnable( FALSE );
		pCMyApp->ConvertImmStatus();
	}

	m_btOk.MsgProc(hWnd, uMsg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, uMsg, wParam, lParam);

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		if( !IsInside( x, y ) )
		{
			g_pNk2DFrame->HidePremiumExchange();
			return TRUE; //����
		}
		else
		{
			int bInputEnable = m_boxName.GetEnable();

			if (E_FAIL != m_boxName.MsgProc( hWnd, uMsg, wParam, lParam ) )
				return TRUE;

			if( bInputEnable == FALSE && m_boxName.GetEnable() )
				pCMyApp->RestoreImmStatus();

			if (m_btCancel.GetState() == BTN_DOWN)
			{
				UnhookWindowsHookEx(g_hHook);
				g_pNk2DFrame->HidePremiumExchange();
				return TRUE;
			}

			if (m_btOk.GetState() == BTN_DOWN)
			{
				UnhookWindowsHookEx(g_hHook);

				if (m_nType == 1)
				{
					if (CheckCharName(m_boxName.GetString()))
					{
						char msg[MAX_PATH] = {0,};

						sprintf(msg, G_STRING(IDS_CHANGE_CHAR_NAME));
						g_pNk2DFrame->InsertPopup(msg, TYPE_NOR_OKCANCLE, POPUP_CHANGE_CHAR_NAME, 30);
					}
					else
					{
						g_pNk2DFrame->HidePremiumExchange();
					}

					return TRUE;
				}
				else if(m_nType == 0)
				{
					if(CheckGuildName(m_boxName.GetString()))
					{
						pCMyApp->m_pUIMgr->UsePremium( m_nSlot , m_boxName.GetString() );
					}

					g_pNk2DFrame->HidePremiumExchange();
					return TRUE;
				}

				//�˻����� ���ϸ�
				// IDS_PREMIUM_MSG_34 ĳ���� ������̳� ���� ������� �����ϴ�.
				else g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_34), TYPE_NOR_OK, 1);
				return TRUE;
			}
		}
		return IM_INPUT;

	case WM_LBUTTONUP:
		if( !IsInside( x, y ) )
		{
			if( !g_pNk2DFrame->IsVisiblePremiumExchange() )
				return TRUE;
		}
		else
		{
			g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, NULL, GetCurrentThreadId());
		}
		break;

	case WM_MOUSEMOVE:
		return IM_INPUT;

	case WM_KEYDOWN:
		if( int(wParam) == VK_ESCAPE )
		{
			g_pNk2DFrame->HidePremiumExchange();
			return TRUE; //����
		}

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if( m_boxName.GetEnable() )
			m_boxName.MsgProc( hWnd, uMsg, wParam, lParam );

		if( m_boxName.GetEnable() )
			return TRUE;

		break;
	}

	return FALSE;
}

void CPremiumExchange::SendChangeCharName()
{
	pCMyApp->m_pUIMgr->UsePremium( m_nSlot , m_boxName.GetString() );
	g_pNk2DFrame->HidePremiumExchange();
}

void CPremiumExchange::ReadProperList(char* szFileName)
{
	FILE* fp = NULL;
	int i = 0, j = 0;
	int nLen = 0;
	char szBuf[100] = "";

	fp = fopen(szFileName, "rt");
	if(!fp)	// ������ ������ ����
	{
		fp = fopen(szFileName, "rt");
		if(!fp)
		{
			MessageBox(NULL, szFileName, "Warning", MB_OK);
			exit(1);
		}
	}

	fscanf(fp, "%s", szBuf);
	nLen = strlen(szBuf);
	for(i=0; i<nLen; i++)
		szBuf[i] = szBuf[i]-1;

	m_nProperListCount = atoi(szBuf);

	// ����ü�� ����Ʈ ������ŭ �����Ѵ�
	m_pszProperList = new char*[m_nProperListCount];

	for( i = 0 ; i < m_nProperListCount ; i ++ )
	{
		m_pszProperList[i] = new char[16];

		fscanf(fp, "%s", szBuf);
		nLen = strlen(szBuf);

		for(j=0; j<nLen; j++)
			szBuf[j] = szBuf[j]-1;

		strcpy(m_pszProperList[i], szBuf);
	}
	fclose(fp);
}