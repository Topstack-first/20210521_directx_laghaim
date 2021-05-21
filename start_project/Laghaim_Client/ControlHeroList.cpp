#include "stdafx.h"


#include "headers.h"
#include "BaseInterface.h"
#include "main.h"
#include "ControlSocial.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "TcpUtil.h"
#include "ListBox.h"
#include "ControlHeroList.h"
#include "Land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "GUtil.h"
#include "Mouse.h"




extern char					g_strFontName[];

extern HFONT G_GetFont(int h, int w,const char  * const fontname);

CControlHeroRankList::CControlHeroRankList()
{
	m_pBack = NULL;

	m_pSurPage = NULL;

	m_Page = -1;
	m_Race = 0;
	m_TotalPage = -1;

	for( int i=0; i<MAX_HERO_LIST_PAGE; i++ )
	{
		m_RankList[i].bEnable = FALSE;
		m_RankList[i].pSurRank =  m_RankList[i].pSurName = NULL;
	}

	for( int i=0; i<NUM_RACE; i++ )
		m_pSurRace[i] = NULL;

	SetFont(5, 12, g_strFontName);

	m_btnClose.SetFileName("Bossraid/HeroRankList/btn_close");
	m_btnArrowR.SetFileName("Bossraid/HeroRankList/btn_ArrowR");
	m_btnArrowL.SetFileName("Bossraid/HeroRankList/btn_ArrowL");
}

CControlHeroRankList::~CControlHeroRankList()
{
	DeleteRes();
}

void CControlHeroRankList::SetFont (int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h, 0, fontname);
}

void CControlHeroRankList::LoadRes()
{
	HWND	  hWnd;

	hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, "interface/Bossraid/HeroRankList/bg.bmp");

	m_pBack->Xpos = (m_ClientRc.right	- m_pBack->GetWidth())	/ 2;
	m_pBack->Ypos = (m_ClientRc.bottom	- m_pBack->GetHeight())	/ 2 - 29;

	char strTemp[256] = {0,};
	for(int i=0; i<NUM_RACE; i++ )
	{
		sprintf(strTemp, "interface/Bossraid/HeroRankList/title_race_%d.bmp", i);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pSurRace[i], strTemp );

		m_pSurRace[i]->Xpos = m_pBack->Xpos + 79;
		m_pSurRace[i]->Ypos = m_pBack->Ypos + 50;
	}

	m_btnClose.LoadRes();
	m_btnArrowR.LoadRes();
	m_btnArrowL.LoadRes();

	m_btnClose.SetPosition( m_pBack->Xpos + m_pBack->GetWidth() - 6 - 16, m_pBack->Ypos + 6 );
	m_btnArrowR.SetPosition( m_pBack->Xpos + 250 + 30, m_pBack->Ypos + 421 );
	m_btnArrowL.SetPosition( m_pBack->Xpos + 177 - 30, m_pBack->Ypos + 421 );
}


void CControlHeroRankList::SetHeroRankLsit(char* buf)
{
	// ���� ���� ��Ŷ
	// hr_list [������ȣ] [��������ȣ] [�ѿ���ȸ��] [ĳ����1] [ĳ����2] .. [ĳ����10]
	// (s->c) hr_list 0 2 18 ��ĭ11�� -1 ��ĭ13�� ��ĭ14�� -1 ��ĭ16�� ��ĭ17�� -1 -1 -1

	char strTemp1[128] = {0,};
	char strTemp2[128] = {0,};

	// �������� �����Ѵ�.
	buf = AnyOneArg(buf, strTemp1);
	m_Race = atoi(strTemp1); // ���� ��ȣ

	buf = AnyOneArg(buf, strTemp1);
	m_Page = atoi(strTemp1); // ������

	buf = AnyOneArg(buf, strTemp1);
	int num = atoi(strTemp1) % MAX_HERO_LIST_PAGE ;
	m_TotalPage =  (atoi(strTemp1) / MAX_HERO_LIST_PAGE ) + 1; // ��Ż ������

	if( atoi(strTemp1) > MAX_HERO_LIST_PAGE * m_TotalPage)
		m_TotalPage ++;

	if( m_Page != -1 )
	{
		SAFE_DELETE(m_pSurPage);
		char temp[64] = {0,};
		sprintf(temp, "%d / %d", m_Page, m_TotalPage);

		g_pDisplay->CreateSurfaceFromText(&m_pSurPage, m_Font, temp, RGB(255,0,255), RGB(255,255,255));
		if( m_pSurPage )
		{
			m_pSurPage->SetColorKey( RGB(255,0,255) );
			m_pSurPage->Xpos = m_pBack->Xpos + ( m_pBack->GetWidth() / 2 ) - (m_pSurPage->GetWidth() / 2  );
			m_pSurPage->Ypos = m_pBack->Ypos + 428;
		}
	}

	for(int i=0; i<MAX_HERO_LIST_PAGE; i++)
	{
		m_RankList[i].bEnable = FALSE;
		SAFE_DELETE( m_RankList[i].pSurRank );
		SAFE_DELETE( m_RankList[i].pSurName );

		buf = AnyOneArg(buf, strTemp1);

//		if( atoi(strTemp1) != -1 )
		{
			m_RankList[i].bEnable = TRUE;

			// ��ȸ ����

			//const char * szOrder = "%d ȸ";

			sprintf( strTemp2, (char*)G_STRING(IDS_LHSTRING1905), i + ((m_Page-1)*MAX_HERO_LIST_PAGE) +1);
			g_pDisplay->CreateSurfaceFromText( &m_RankList[i].pSurRank, m_Font, strTemp2, RGB(0, 0, 0), RGB(255,200,1000) );

			if( m_RankList[i].pSurRank )
				m_RankList[i].pSurRank->SetColorKey(RGB(0, 0, 0));

			// ������ �̸�
			if( atoi(strTemp1) == -1 )
			{
				if( m_TotalPage > m_Page || (m_TotalPage == m_Page && num > 0 && num > i) )
					g_pDisplay->CreateSurfaceFromText( &m_RankList[i].pSurName, m_Font, "------------", RGB(0, 0, 0), RGB(255,200,1000) );
				else
					g_pDisplay->CreateSurfaceFromText( &m_RankList[i].pSurName, m_Font, " ", RGB(0, 0, 0), RGB(255,200,1000) );

			}
			else
				g_pDisplay->CreateSurfaceFromText( &m_RankList[i].pSurName, m_Font, strTemp1, RGB(0, 0, 0), RGB(255,200,1000) );

			if( m_RankList[i].pSurName )
				m_RankList[i].pSurName->SetColorKey(RGB(0, 0, 0));

			// ��ġ�� �������.
			m_RankList[i].pSurRank->Xpos = m_pBack->Xpos + 190 - m_RankList[i].pSurRank->GetWidth();
			m_RankList[i].pSurRank->Ypos = m_pBack->Ypos + 164 + (i * 25) ;

			m_RankList[i].pSurName->Xpos = m_pBack->Xpos + 225;
			m_RankList[i].pSurName->Ypos = m_RankList[i].pSurRank->Ypos;
		}
//#endif
	}

}

void CControlHeroRankList::DeleteRes()
{
	SAFE_DELETE(m_pBack);

	for(int i=0; i<MAX_HERO_LIST_PAGE; i++)
	{
		m_RankList[i].bEnable = FALSE;
		SAFE_DELETE( m_RankList[i].pSurRank );
		SAFE_DELETE( m_RankList[i].pSurName );
	}

	for( int i=0; i<NUM_RACE; i++ )
		SAFE_DELETE( m_pSurRace[i] );

	SAFE_DELETE(m_pSurPage);

	m_btnClose.DeleteRes();
	m_btnArrowR.DeleteRes();
	m_btnArrowL.DeleteRes();
}


void CControlHeroRankList::Draw()
{
	if( m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	m_btnClose.Draw();

	if( m_Page != -1 )
		g_pDisplay->Blt(m_pSurPage->Xpos, m_pSurPage->Ypos, m_pSurPage );

	if( m_Race != -1 )
		g_pDisplay->Blt(m_pSurRace[m_Race]->Xpos, m_pSurRace[m_Race]->Ypos, m_pSurRace[m_Race]);

	for(int i=0; i<MAX_HERO_LIST_PAGE; i++)
	{
		if( m_RankList[i].bEnable )
		{
			g_pDisplay->Blt(m_RankList[i].pSurRank->Xpos, m_RankList[i].pSurRank->Ypos, m_RankList[i].pSurRank);
			g_pDisplay->Blt(m_RankList[i].pSurName->Xpos, m_RankList[i].pSurName->Ypos, m_RankList[i].pSurName);
		}
	}

	if( m_Page > 1)
		m_btnArrowL.Draw();
	else
		m_btnArrowL.SetDisable(TRUE);

	if( m_Page < m_TotalPage)
		m_btnArrowR.Draw();
	else
		m_btnArrowR.SetDisable(TRUE);
}



int CControlHeroRankList::IsInside(int x, int y)
{
	// ���콺�� bg�ȿ� �ִ��� �˻�
	int left, right, top, bottom;
	left	= m_pBack->Xpos;
	top		= m_pBack->Ypos;
	right	= left + m_pBack->GetWidth();
	bottom	= top  + m_pBack->GetHeight();

	if( left < x && right > x && top < y && bottom > y )
		return 1;

	return 0;
}


LRESULT CControlHeroRankList::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	if( m_Page > 1)
		m_btnArrowL.SetDisable(FALSE);
	else
		m_btnArrowL.SetDisable(TRUE);

	if( m_Page < m_TotalPage)
		m_btnArrowR.SetDisable(FALSE);
	else
		m_btnArrowR.SetDisable(TRUE);

	switch ( msg )
	{
	case WM_MOUSEMOVE:
		if(IsInside(x, y) == 1)
		{
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
			return 1;
		}
		break;

	case WM_LBUTTONDOWN:

		if( IsInside(x, y) != 1)
		{
			g_pNk2DFrame->ShowHeroRankListWindow(FALSE);
			return 1;
		}

		m_btnClose.MsgProc(hWnd, msg, wParam, lParam );
		m_btnArrowR.MsgProc(hWnd, msg, wParam, lParam );
		m_btnArrowL.MsgProc(hWnd, msg, wParam, lParam );
		return 1;

	case WM_LBUTTONUP:

		if(IsInside(x, y) != 1)
			return 1;

		m_btnClose.MsgProc(hWnd, msg, wParam, lParam );
		m_btnArrowR.MsgProc(hWnd, msg, wParam, lParam );
		m_btnArrowL.MsgProc(hWnd, msg, wParam, lParam );

		if(m_btnClose.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowHeroRankListWindow(FALSE);
			m_btnClose.SetState(BTN_NORMAL);
			return 1;
		}
		else
			m_btnClose.SetState(BTN_NORMAL);

		if(m_btnArrowR.GetState() == BTN_ACTION)
		{
			SendMsgPage(m_Page+1);
			m_btnArrowR.SetState(BTN_NORMAL);
			return 1;
		}
		else
			m_btnArrowR.SetState(BTN_NORMAL);


		if(m_btnArrowL.GetState() == BTN_ACTION)
		{
			SendMsgPage(m_Page-1);
			m_btnArrowL.SetState(BTN_NORMAL);
			return 1;
		}
		else
			m_btnArrowL.SetState(BTN_NORMAL);
		return 1;
	}
	return 0;
}


void CControlHeroRankList::SendMsgPage(int page)
{
	// Ŭ�� ��û ��Ŷ
	// hr_list req [������ȣ] [��������ȣ]
	// (c->s) hr_list req 0 2

	char msg[64] = {0,};

	if( m_Race != -1)
	{
		sprintf(msg, "hr_list req %d %d\n", m_Race, page);
		g_pTcpIp->SendNetMessage(msg);
	}

}


void CControlHeroRankList::SetRace(int race /* = -1 */)
{
	m_Race			= race;
	m_Page			= -1;
	m_TotalPage	= -1;

	// �ʱ�ȭ ������.
	for(int i=0; i<MAX_HERO_LIST_PAGE; i++)
	{
		m_RankList[i].bEnable = FALSE;
		SAFE_DELETE( m_RankList[i].pSurRank );
		SAFE_DELETE( m_RankList[i].pSurName );
	}


}