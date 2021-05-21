#include "stdafx.h"
#include "guild_search.h"

#include "d3dutil.h"
#include "headers.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "multilistbox.h"
#include "NkCharacter.h"
#include "window_resource.h"
#include "WindowMgr.h"
#include "ControlBottom.h"
#include "g_stringmanager.h"




#define MAX_PAGE_SIZE 10
#define MAX_PAGE_LINE 10
#define MAX_PAGE	  50	// 총 페이지수 * 라인개수 (ex. 5 x MAX_PAGE_LINE)








/*************************************************************
#. 초기 설정시 함수 호출 순서
   1. CTextBoard::Init
   2. CTextBoard::AddLine
   3. CTextBoard::SetBasisPos
**************************************************************/

CGuildSearch::CGuildSearch() : CUIBaseWindow()
{
	m_listguild.clear();
	m_listPage.clear();

	m_pMultiList	= NULL;
	m_bStart		= FALSE;
	m_dwAniTime		= 0;
	m_nAniCnt		= 0;

	clear();
	InitMode();
}

CGuildSearch::~CGuildSearch()
{

	DeleteRes();

	all_Reset();

	SAFE_DELETE(m_pMultiList);

	for(int i = 0; i < 5; i++)
		SAFE_DELETE(m_pIngSurface[i]);

	m_tboxsearch.CloseIMC();
}

void CGuildSearch::clear()
{
	m_nTotalPage   = 0;
	m_nCurrentPage = 0;
	m_nMaxLine     = 0;

	for(int i = 0; i < 5 ; i++)
		m_pIngSurface[i] = NULL;
}

void CGuildSearch::all_Reset()
{
	set_start(FALSE);

	m_nTotalPage = 0;
	m_nCurrentPage = 0;
	m_nMaxLine = 0;

	m_listguild.clear();
	m_listPage.clear();

	m_pMultiList->all_Reset();
}

void CGuildSearch::guilds_insertlist(g_listInfo& g_list)
{
	list_guild::iterator _iter = m_listguild.end();

	m_listguild.insert(_iter, g_list);

	int nPageNum = (m_listguild.size()/MAX_PAGE_SIZE);

	nPageNum = 0;

	m_listPage[nPageNum].push_back(g_list);

	guilds_insertpagelist(g_list);

}

void CGuildSearch::guilds_insertpagelist(g_listInfo& g_list)
{
	if(!m_pMultiList)	return;

	char szTmp[256] = {0,};

	// 길드 이름
	sprintf(szTmp, "%s", g_list.guild_name);

	m_pMultiList->AddString(m_nMaxLine, CW_GUILD_NAME, szTmp);

	ZeroMemory(szTmp, sizeof(szTmp));

	// 길드 마스터 이름
	sprintf(szTmp, "%s", g_list.guild_master_name);

	m_pMultiList->AddString(m_nMaxLine, CW_GUILD_MASTER, szTmp);

	ZeroMemory(szTmp, sizeof(szTmp));

	// 길드 랭킹
	sprintf(szTmp, "%d", g_list.guild_rank);

	m_pMultiList->AddString(m_nMaxLine, CW_GUILD_RANK, szTmp);

	ZeroMemory(szTmp, sizeof(szTmp));

	// 길드 레벨
	sprintf(szTmp, "%s", g_pNk2DFrame->GetWindowRes()->GetGuildLevel(g_list.guild_glevel-1));

	m_pMultiList->AddString(m_nMaxLine, CW_GUILD_LEVEL, szTmp);


	ZeroMemory(szTmp, sizeof(szTmp));

	// 길드 점수
	sprintf(szTmp, "%d", g_list.guild_point);

	m_pMultiList->AddString(m_nMaxLine, CW_GUILD_POINT, szTmp);

	m_pMultiList->SetIncLine();

	m_nMaxLine++;
}

BOOL CGuildSearch::InitMode()
{
	// 1. 스트링
	// 2. X
	// 3. Y
	// 4. Height
	// 5. linenum
	// 6. columnnum
	m_pMultiList = new CMutiListBox(" ",100, 30, 20, 200, 5, 12);

	m_pMultiList->AddLine(CW_GUILD_NAME  , 158.f);
	m_pMultiList->AddLine(CW_GUILD_MASTER, 140.f);
	m_pMultiList->AddLine(CW_GUILD_RANK  , 66.f );
	m_pMultiList->AddLine(CW_GUILD_LEVEL , 56.f );
	m_pMultiList->AddLine(CW_GUILD_POINT , 100.f);


	m_pMultiList->SetImage("guild/scr_down", "guild/scr_up", "interface/guild/scr_bar.bmp");
	m_pMultiList->SetSelectedLine(-1);

	m_btSearch.SetFileName("guild/search_bt");
	m_btRequest.SetFileName("guild/request");
	m_btBack.SetFileName("guild/back");

	return TRUE;
}

void CGuildSearch::LoadRes()
{
	int nScreenWidth  = 0;
	int nScreenHeight = 0;
	if( g_pNk2DFrame )
	{
		nScreenWidth  = g_pNk2DFrame->GetClientWidth();
		nScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SetBackground(WBK_SEARCH);
	MakeCloseBtn("guild/exit_new");

	char szTmp[256] = {0,};

	for(int i = 0; i < 5; i++)
	{
		SAFE_DELETE(m_pIngSurface[i]);
		sprintf(szTmp, "interface/guild/guild_search/search_ing%d.bmp", i+1);

		g_pDisplay->CreateSurfaceFromBitmap(&m_pIngSurface[i], szTmp);
	}

	if(m_pBackScene)
	{
		int n_x = (nScreenWidth  / 2) - (m_pBackScene->GetWidth()  / 2);
		int n_y = (nScreenHeight / 2) - (m_pBackScene->GetHeight() / 2);


		m_nPosX = n_x;
		m_nPosY = n_y;

		m_pMultiList->LoadRes();

		m_btRequest.LoadRes();
		m_btSearch.LoadRes();
		m_btBack.LoadRes();

		m_ComboBox.SetItemSize(70, 21, 4, 3);
		m_ComboBox.LoadRes("guild/down", "interface/guild/scr_box.bmp", "interface/guild/guild_search/search_tap.bmp");
		m_ComboBox.DeleteAllItem();

		m_ComboBox.InsertItemByImage("interface/guild/guild_search/search_tap1");
		m_ComboBox.InsertItemByImage("interface/guild/guild_search/search_tap2");

		m_tboxsearch.Init(m_nPosX, m_nPosY, 20, RGB(255,255,255), RGB(255, 179, 0), FALSE, MSG_POPUP);
		m_tboxsearch.SetfontSize();

		SetPosition();
	}

}


void CGuildSearch::DeleteRes()
{
	CUIBaseWindow::DeleteRes();

	m_tboxsearch.DeleteRes();
	m_ComboBox.DeleteRes();
	m_btBack.DeleteRes();

	m_pMultiList->DeleteRes();

}

void CGuildSearch::RestoreSurfaces()
{
	m_pMultiList->Restore();
}

void CGuildSearch::set_start(BOOL bStart)
{
	m_bStart = bStart;

	if(m_bStart)
		set_start_ani();
	else
		set_end_ani();
}

void CGuildSearch::set_start_ani()
{
	m_dwAniTime = timeGetTime();

	m_tboxsearch.SetEnable(FALSE);
}

void CGuildSearch::set_end_ani()
{
	m_dwAniTime = 0;
	m_nAniCnt   = 0;

	m_tboxsearch.SetEnable(TRUE);
}

int CGuildSearch::Draw()
{
	if( g_pDisplay && m_pBackScene)
	{
		if(CUIBaseWindow::Draw())
		{
			m_btRequest.Draw();
			m_btSearch.Draw();
			m_btBack.Draw();

			m_tboxsearch.Draw();
			m_ComboBox.DrawImage();

			m_pMultiList->Draw();
		}
	}

	if(m_bStart)
	{
		DWORD tmpTime = timeGetTime() - m_dwAniTime;

		if(tmpTime > 100)
		{
			m_dwAniTime = timeGetTime();

			if(m_nAniCnt >= 4)
				m_nAniCnt = 0;
			else
				m_nAniCnt++;
		}

		if(g_pDisplay->Blt(m_nPosX + 120, m_nPosY + 282, m_pIngSurface[m_nAniCnt]) == DD_OK)
		{
		}

	}

	return -1;
}

char* CGuildSearch::Get_SelectGuildName()
{
	return m_pMultiList->GetString(0);
}

void CGuildSearch::SetPosition()
{
	m_btRequest.SetPosition(m_nPosX + 379 , m_nPosY + 282);
	m_btSearch.SetPosition(m_nPosX + 277, m_nPosY + 282);
	m_btBack.SetPosition( m_nPosX + 328, m_nPosY + 282);
	m_pBtExit->SetPosition( m_nPosX + 513, m_nPosY + 282);
	m_ComboBox.SetPosition(m_nPosX + 19, m_nPosY + 284);
	m_ComboBox.BtnSetPosition(m_nPosX + 19 + 78, m_nPosY + 284 + 3);
	m_tboxsearch.SetPos(m_nPosX + 130, m_nPosY + 287);
	m_pMultiList->SetPosDim(m_nPosX + 15, m_nPosY + 55, 537, 205);
}

BOOL CGuildSearch::MoveWindow(UINT msg, int x, int y)
{
	return CUIBaseWindow::MoveWindow(msg, x, y);
}

BOOL CGuildSearch::IsInside(int x, int y)
{
	return CUIBaseWindow::IsInside(x, y);
}


LRESULT CGuildSearch::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( m_tboxsearch.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_tboxsearch.SetEnable( FALSE );
		pCMyApp->ConvertImmStatus();
	}

	m_ComboBox.MsgProcImage(hWnd, msg, wParam, lParam);

	m_btRequest.MsgProc(hWnd, msg, wParam, lParam);
	m_btSearch.MsgProc(hWnd, msg, wParam, lParam);
	m_btBack.MsgProc(hWnd, msg, wParam, lParam);

	if( m_pMultiList )
	{
		if( E_FAIL != m_pMultiList->GetMessage(hWnd, msg, wParam, lParam) )
			return IM_INPUT;
	}

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
				return IM_NONE;

			int bInputEnable = m_tboxsearch.GetEnable();

			if( E_FAIL != m_tboxsearch.MsgProc( hWnd, msg, wParam, lParam ) )
				return IM_INPUT;

			if( bInputEnable == FALSE && m_tboxsearch.GetEnable() )
				pCMyApp->RestoreImmStatus();

			if( m_btRequest.GetState() == BTN_DOWN )
				return IM_INPUT;

			if( m_btSearch.GetState() == BTN_DOWN )
				return IM_INPUT;

			if( m_btBack.GetState() == BTN_DOWN )
				return IM_INPUT;

		}
		break;

	case WM_LBUTTONUP:

		if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
			return IM_NONE;

		m_tboxsearch.MsgProc( hWnd, msg, wParam, lParam );

		if(m_btRequest.GetState() == BTN_ACTION)
		{
			if(g_pRoh->GetMyGuildData())
			{
				// IDS_GUILD_JOIN_DONT_GMEMBER "길드 가입자는 요청할 수 없습니다."
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_DONT_GMEMBER), TYPE_GUILD_CONFIRM, 42);
				return IM_INPUT;
			}

			if(Get_SelectGuildName())	// IDS_GUILD_JOIN_SEND_MSG "길드 가입을 요청 합니다."
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOIN_SEND_MSG), TYPE_GUILD_CONFIRM, POPUP_GUILD_JOIN);
			else	// IDS_GUILD_NEED_CHOISE_GUILD "선택된 길드가 없습니다."
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_NEED_CHOISE_GUILD), TYPE_GUILD_CONFIRM, 42);

			return IM_INPUT;
		}

		if(m_btSearch.GetState() == BTN_ACTION)
		{
			char szTmp[1024] = {0,};

			char* szString = m_tboxsearch.GetStrBuf();

			if(m_ComboBox.GetSelect() == 0)
			{
				sprintf(szTmp, "mg search g %s\n", szString); // 길드명
			}
			else if(m_ComboBox.GetSelect() == 1)
			{
				sprintf(szTmp, "mg search m %s\n", szString); // 길드마스터명
			}

			g_pTcpIp->SendNetMessage(szTmp);

			return IM_INPUT;
		}

		if( m_btBack.GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->GetUIWindow()->OnOpenPrvWindow();
			return IM_INPUT;
		}

		break;

	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:

		if( m_tboxsearch.GetEnable() )
			m_tboxsearch.MsgProc( hWnd, msg, wParam, lParam );

		if( m_tboxsearch.GetEnable() )
			return IM_INPUT;

		break;
	}

//	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
//		return IM_NONE;

	return CUIBaseWindow::MsgProc(hWnd, msg, wParam, lParam);
}
