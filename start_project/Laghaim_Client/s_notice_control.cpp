#include "stdafx.h"
#include "headers.h"
#include "s_notice_control.h"
#include "Country.h"
#include "2DResMgr.h"
#include "Tcpipcon.h"
#include "LetterBox.h"
#include "g_stringmanager.h"



extern DWORD		g_dwClientCountry;




#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

CNoticeControl* CNoticeControl::Create(int nBaseX, int nBaseY)
{
	return new CNoticeControl(nBaseX, nBaseY);
}

CNoticeControl::CNoticeControl(int nBaseX, int nBaseY)
{
	// 변수초기화
	m_hWnd = NULL;

	m_BaseX = nBaseX;
	m_BaseY = nBaseY;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	m_pTitle = NULL;
	m_pNoticeBack = NULL;

	m_pNoticeString = NULL;

	Init();
}

CNoticeControl::~CNoticeControl()
{
	DeleteRes();

	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pNoticeBack);
	SAFE_DELETE(m_pNoticeString);
}

void CNoticeControl::Init()
{
	m_BtSave.SetFileName("guild/save");
}

void CNoticeControl::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);


	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pNoticeBack);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTitle, "interface/guild/guild_notice/admin_notice.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pNoticeBack, "interface/guild/guild_notice/admin_notice_bk.bmp");

	m_BtSave.LoadRes();

	m_pNoticeString = new CLetterBox();

	if( m_pNoticeString )
	{
		m_pNoticeString->Init_MEBox(false, 9, 30);
		m_pNoticeString->Init(0, 0,30,0x000000,_FCOLOR_YELLOW);
		m_pNoticeString->SetPos(m_BaseX+33, m_BaseY+200);
		m_pNoticeString->DrawText();
		m_pNoticeString->SetMaxLen(127);
		m_pNoticeString->SetFontSize(12);
		m_pNoticeString->SetEnable(false);
	}
	SetPosition();
}

void CNoticeControl::Restore()
{

}

void CNoticeControl::SetPosition()
{
	m_pNoticeBack->Xpos = m_BaseX + 21;
	m_pNoticeBack->Ypos = m_BaseY + 185;

	m_pTitle->Xpos = m_pNoticeBack->Xpos + (m_pNoticeBack->GetWidth() - m_pTitle->GetWidth())*0.5;
	m_pTitle->Ypos = m_BaseY + 142;

	m_BtSave.SetPosition(m_BaseX + 109, m_BaseY + 455);

}

void CNoticeControl::Draw()
{
	if( !g_pDisplay || (m_BaseX == 0 && m_BaseY))
		return;

	g_pDisplay->Blt(m_pTitle->Xpos, m_pTitle->Ypos, m_pTitle);
	g_pDisplay->Blt(m_pNoticeBack->Xpos, m_pNoticeBack->Ypos, m_pNoticeBack);

	m_BtSave.Draw();

	if( m_pNoticeString )
		m_pNoticeString->Draw();

}

void CNoticeControl::DeleteRes()
{
	m_BtSave.DeleteRes();

	m_pNoticeString->DeleteRes();
}

void CNoticeControl::RevMsg(char* psz)
{
	char tete[512*3] = "";
	char szNameTemp[ 256 ] = "";

	char* contents = psz;

	int len = strlen(contents);
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if(contents[i] == '%')
		{
			tete[j++] = '\n';
		}
		else
		{
			tete[j++] = contents[i];
		}
	}
	tete[j] = '\0';

	cleartext();

	if( len == 0 )
		return;

	m_pNoticeString->m_MEBox.Set_CoreString(tete);
	m_pNoticeString->m_MEBox.Gen_cursor_table();

}

void CNoticeControl::SendMsg()
{
	char tete[256*3] = "";
	char szNameTemp[ 256 ] = "";

	char* contents = m_pNoticeString->m_MEBox.Get_CoreString();

	int len = strlen(contents);
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (contents[i] == '\n')
		{
			tete[j++] = '%';
		}
		else
		{
			tete[j++] = contents[i];
		}
	}

	tete[j] = '\0';

	sprintf(m_MsgBuf, "mg notice %s\n", tete);

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);

}

LRESULT CNoticeControl::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_BtSave.MsgProc(hWnd, msg, wParam, lParam);

	switch(msg)
	{
	case WM_LBUTTONDOWN :

		if( m_pNoticeString->IsInside(LOWORD(lParam), HIWORD(lParam)))
			m_pNoticeString->MsgProc(hWnd, msg, wParam, lParam);
		else
			m_pNoticeString->SetEnable(FALSE);

		break;
	case WM_LBUTTONUP :
		{
			if( m_BtSave.GetState() == BTN_ACTION )
			{
				if( strlen( m_pNoticeString->m_MEBox.Get_CoreString() ) > 0  &&
						strlen(m_pNoticeString->m_MEBox.Get_CoreString()) < 512 )
				{
					sprintf(m_MsgBuf, "mg notice %s\n", m_pNoticeString->m_MEBox.Get_CoreString());
					SendMsg();
				}
				else if( strlen( m_pNoticeString->m_MEBox.Get_CoreString() ) <= 0 )
				{
					// IDS_GUILD_NOTICE_PLZ_WRITE "공지 내용을 넣어주세요."
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_NOTICE_PLZ_WRITE), TYPE_GUILD_CONFIRM);
				}
				else if( strlen( m_pNoticeString->m_MEBox.Get_CoreString() ) > 512 )
				{
					// IDS_GUILD_NOTICE_STRLEN_OVER "512byte를 초과 하였습니다."
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_NOTICE_PLZ_WRITE), TYPE_GUILD_CONFIRM, 42 );
				}
			}


			m_pNoticeString->MsgProc(hWnd, msg, wParam, lParam);

			if (IsInside(LOWORD (lParam), HIWORD (lParam)) == 0)
			{
				m_pNoticeString->SetEnable(FALSE);
				pCMyApp->ConvertImmStatus();
				return 1;
			}
			else
			{
				if(IsInside_Prompt(LOWORD (lParam), HIWORD (lParam)) == 1)
				{
					m_pNoticeString->SetEnable(TRUE);
					return 3;
				}
			}
		}
		break;
	case WM_KEYDOWN:

		if( (int)wParam == VK_ESCAPE)
			break;

		if(m_pNoticeString->GetEnable())
		{
			m_pNoticeString->MsgProc( hWnd, msg, wParam, lParam );
			return 3;
		}


	case WM_CHAR:
		if(m_pNoticeString->GetEnable())
		{
			m_pNoticeString->MsgProc( hWnd, msg, wParam, lParam );
			return 3;
		}
		break;

	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:

		if(m_pNoticeString->GetEnable())
		{
			m_pNoticeString->MsgProc( hWnd, msg, wParam, lParam );
			return 3;
		}
		break;
	}

	return 1;
}

void CNoticeControl::SetNotice(char* szNotice)
{
	RevMsg(szNotice);
}

void CNoticeControl::cleartext()
{
	m_pNoticeString->ClearText();
	m_pNoticeString->m_MEBox.Reset();
}

BOOL CNoticeControl::IsInside(int x, int y)
{

	if (x >= (int)(m_BaseX)  && x < (int)(m_BaseX + 283) &&
			y >= (int)(m_BaseY) && y < (int)(m_BaseY + 533) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CNoticeControl::IsInside_Prompt(int x, int y)
{
	if( !m_pNoticeBack )
		return false;

	int start_x, start_y;
	int check_cx, check_cy;

	start_y = m_BaseY+200;
	check_cy = 200;

	start_x = m_BaseX+33;
	check_cx = 170;

	if ( ( ( x > start_x) && ( x < (start_x + check_cx)) )
		&& ( ( y > start_y ) && ( y < (start_y + check_cy) ) ) )
		return true;
	else
		return false;
}
