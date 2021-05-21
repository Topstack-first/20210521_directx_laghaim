#include "stdafx.h"
#include "guild_member_notice.h"
#include "d3dutil.h"
#include "headers.h"
#include "Nk2DFrame.h"
#include "2DResMgr.h"
#include "main.h"
#include "window_resource.h"
#include "WindowMgr.h"
#include "Tcpipcon.h"
#include "NkCharacter.h"
#include "s_guild_manager.h"
#include "LetterBox.h"
#include "guild_data.h"








CMemberNotice::CMemberNotice() : CUIBaseWindow()
{
	m_pNoticeString = NULL;
}

CMemberNotice::~CMemberNotice()
{
	DeleteRes();

	SAFE_DELETE(m_pNoticeString);

}

BOOL CMemberNotice::InitMode()
{
	return TRUE;
}

void CMemberNotice::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;

	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_MEMBERNOTICE);
	MakeCloseBtn("guild/msg_ok");

	if(m_pBackScene)
	{
		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);

		m_nPosX = n_x - 50;
		m_nPosY = n_y;

		m_pNoticeString = new CLetterBox();

		if( m_pNoticeString )
		{
			m_pNoticeString->Init_MEBox(false, 9, 30);
			m_pNoticeString->Init(0,0,30,0x000000,_FCOLOR_YELLOW);

			m_pNoticeString->SetPos(m_nPosX+29, m_nPosY+47);
			m_pNoticeString->DrawText();
			m_pNoticeString->SetMaxLen(127);
			m_pNoticeString->SetFontSize(12);
			m_pNoticeString->SetEnable(false);

			CGuildData* pData = g_pRoh->GetMyGuildData();

			if( pData )
				SetNotice(pData->GetMyNoticeInfo().s_guildnotic);
		}
		SetPosition();
	}
}

void CMemberNotice::DeleteRes()
{
	m_pNoticeString->DeleteRes();
}

void CMemberNotice::RestoreSurfaces()
{
}

int CMemberNotice::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			if( m_pNoticeString )
				m_pNoticeString->Draw();
		}
	}
	return -1;
}

void CMemberNotice::SetNotice(char* szNotice)
{
	RevMsg(szNotice);
}

void CMemberNotice::RevMsg(char* psz)
{
	char tete[256*3] = "";
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

	if( tete )
	{
		m_pNoticeString->m_MEBox.Set_CoreString(tete);
		m_pNoticeString->m_MEBox.Gen_cursor_table();
	}
}

void CMemberNotice::cleartext()
{
	m_pNoticeString->ClearText();
	m_pNoticeString->m_MEBox.Reset();
}

BOOL CMemberNotice::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

void CMemberNotice::SetPosition()
{
	SetExitBtnPostion(195, 156);
	m_pNoticeString->SetPos(m_nPosX+29, m_nPosY+47);
}

BOOL CMemberNotice::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}

LRESULT CMemberNotice::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return IM_NONE;

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}
