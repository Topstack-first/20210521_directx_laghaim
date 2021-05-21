#include "stdafx.h"
#include "WindowMgr.h"
#include "headers.h"
#include "main.h"
#include "window_class.h"
#include "Nk2DFrame.h"
#include <assert.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




CWindowMgr::CWindowMgr()
{
	Clear();
	Init();
}

CWindowMgr::~CWindowMgr()
{
	Destroy();
	Clear();
}

void CWindowMgr::Init()
{
	if(!InitWindow())
	{
		OutputDebugString("Window Object Factory Fail\n");
	}
}

// 윈도우클래스를 객체팩토리에 등록
bool CWindowMgr::InitWindow()
{
	if(!m_WindowFactory.Register(WINDOW_GUILD_MEMBER			      , new Creator<CGuildMemberPage, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_NONMEMBER			      , new Creator<CGuildNoneMemberPage, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_SEARCH			      , new Creator<CGuildSearch, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_CREATE			      , new Creator<CGuildCreate, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_MIX				      , new Creator<CGuildMix, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_PROMOTION			      , new Creator<CGuildPromotion, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_COMBINATION		      , new Creator<CGuildCombination, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_KIND		      , new Creator<CBattleSelect, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_RANKING	      , new Creator<CBattleRankSelect, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_RANKING_MENU	  , new Creator<CBattleRankMenu, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_1VS1		      , new Creator<CBattleRank, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_ACCEPTMEMBER     , new Creator<CBattleAcceptMember, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_INVASION	      , new Creator<CBattleInvasionAttack, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_INVASION_INFO    , new Creator<CBattleInvasionInfo, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_BATTLE_INVASION_REQUEST , new Creator<CBattleInvasion, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_GUILD_MEMBER_NOTICE			  , new Creator<CMemberNotice, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_DMITRON_BATTLE				  , new Creator<CDmitronBattle, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_DMITRON_BATTLE_INFO			  , new Creator<CDmitronBattleInfo, CUIBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_DMITRON_BATTLE_TAX            , new Creator<CDmitronBattleTax, CUIBaseWindow>))
	{
		return false;
	}

	return true;
}

void CWindowMgr::Clear()
{
	m_PrvWidnow = 0;
	m_bActiveWindow = false;
	m_WindowList.clear();
	m_WinTypeList.clear();
	m_WinIDList.clear();
}

void CWindowMgr::Destroy()
{
	AllCloseWindow();
}

void CWindowMgr::DeleteRes()
{
	if(m_WindowList.empty())
	{
		return ;
	}

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		m_WindowList[i]->DeleteRes();
	}
}

void CWindowMgr::RestoreSurface()
{
	if(m_WindowList.empty())
	{
		return ;
	}

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		m_WindowList[i]->RestoreSurfaces();
	}
}

LRESULT CWindowMgr::WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(m_WindowList.empty())
	{
		return false;
	}

	bool bInputIn = false;

	int nTopWinNum = (m_WindowList.size()-1);

	if(UpdateActiveState(msg, LOWORD(lParam), HIWORD(lParam)))
	{
		bInputIn = true;
	}
	// 각 윈도우의 입력 프로세스의 상태를 받는다.
	INT nWindowState = m_WindowList[nTopWinNum]->MsgProc(hWnd, msg, wParam, lParam);

	// 상태에 따른 처리를 한다.
	switch(nWindowState)
	{
	case IM_INPUT:
		bInputIn = true;
		break;
	case IM_CLOSE:
		CloseWindow(nTopWinNum);
		break;
	}

	return bInputIn;
}


/************************************************************************/
/*
	bool CWindowMgr::UpdateActiveState()

	현재 사용하고 있는 창을 업데이트 시켜준다.
	                                                                    */
/************************************************************************/
bool CWindowMgr::UpdateActiveState(UINT msg, int x, int y)
{
	if(msg != WM_LBUTTONDOWN) return false;

	for(int i = (m_WindowList.size()-1); i >= 0; i--)
	{
		// 만약 i번째 윈도우를 클릭했다면 최상위로 올려주고 루프를 나온다.
		if(m_WindowList[i]->IsInside(x, y))
		{
			ClickWindow(i);
			return true;
		}
		// 마지막윈도우까지도 클릭한 것이 없다면 윈도우가 열려는 있되 활성이 안된 상태를 만든다.
		if(i == 0)
		{
			ClickOutside();
		}
	}

	return false;
}

void CWindowMgr::Draw()
{
	if(m_WindowList.empty())
	{
		return ;
	}

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		m_WindowList[i]->Draw();
	}
}

void CWindowMgr::OnOpenPrvWindow()
{
	if( m_PrvWidnow >= 0 )
	{
		CloseTopWindow();
		OnOpenWindow(m_PrvWidnow);
	}

}


bool CWindowMgr::OnOpenWindow(int nWindowType)
{
	if(IsOpenWindow(nWindowType))
	{
		return false;
	}

	m_PrvWidnow = GetTopWindow();

	AllCloseWindow();

	CUIBaseWindow * pWindow = m_WindowFactory.Create(nWindowType);
	assert(pWindow);

	{
		pWindow->InitMode();
		pWindow->LoadRes();

		pWindow->SetWndID(GetNewWndID());

		m_WinIDList.push_back(pWindow->GetWndID());
		m_WinTypeList.push_back(nWindowType);
		m_WindowList.push_back(pWindow);

		m_bActiveWindow = true;

		return true;
	}

	SAFE_DELETE(pWindow);

	return false;
}

bool CWindowMgr::OnCloseWindow(int nWindowType)
{
	if(!IsOpenWindow(nWindowType))
	{
		return false;
	}

	m_WinIt	= m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(nWindowType == GWIN_TYPE)
		{
			SAFE_DELETE(GWIN_OBJ);
			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);
			return true;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
	}

	if(m_WindowList.empty())
	{
		m_bActiveWindow = false;
	}

	return false;
}

bool CWindowMgr::OnCloseWindow(WINDOW_ID WndID)
{
	if(!IsOpenWindow(WndID))
	{
		return false;
	}

	m_WinIt	= m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(WndID == GWIN_ID)
		{
			SAFE_DELETE(GWIN_OBJ);
			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);

			if(m_WindowList.empty())
			{
				m_bActiveWindow = false;
			}

			return true;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
	}

	return false;
}

void CWindowMgr::AllCloseWindow()
{
	if(!m_WindowList.empty() && !m_WinTypeList.empty() && !m_WinIDList.empty())
	{
		int nWinMaxNum = m_WindowList.size();

		for(int i = 0; i < nWinMaxNum; i++)
		{
			SAFE_DELETE(m_WindowList[i]);
		}

		m_WindowList.clear();
		m_WinTypeList.clear();
		m_WinIDList.clear();
	}
}

int CWindowMgr::GetTopWindow()
{
	if(m_WinTypeList.empty())
	{
		return -1;
	}

	return (m_WinTypeList[m_WinTypeList.size()-1]);
}

bool CWindowMgr::IsOpenWindow(int nWindowType)
{
	if( m_WindowList.empty() )
		return false;

	m_WtIt = find(m_WinTypeList.begin(), m_WinTypeList.end(), nWindowType);

	if(m_WinTypeList.end() == m_WtIt)
	{
		return false;
	}

	return true;
}

bool CWindowMgr::IsOpenWindow(WINDOW_ID WndID)
{
	m_IDIt = find(m_WinIDList.begin(), m_WinIDList.end(), WndID);

	if(m_WinIDList.end() == m_IDIt)
	{
		return false;
	}

	return true;
}

bool CWindowMgr::IsOpenWindow()
{
	if(m_WindowList.empty()) return false;

	return true;
}

// 여기서 받는 인자는 윈도우 타입이 아니라 몇번째 열린 윈도인가이다.
void CWindowMgr::ClickWindow(int nWindowNum)
{
	WindowSelectTop(nWindowNum);
	m_bActiveWindow = true;
}

void CWindowMgr::ClickOutside()
{
	m_bActiveWindow = false;
}

void CWindowMgr::CloseWindow(int nWindowNum)
{
	WindowSelectPop(nWindowNum);
	m_bActiveWindow = false;
}

void CWindowMgr::WindowSelectTop(int nWindowNum)
{
	int nWinCount = 0;
	m_WinIt = m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(nWinCount == nWindowNum)
		{
			CUIBaseWindow * pWindow	= GWIN_OBJ;
			int nIdx				= GWIN_TYPE;
			WINDOW_ID WndID			= GWIN_ID;

			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);

			m_WinIDList.push_back(WndID);
			m_WindowList.push_back(pWindow);
			m_WinTypeList.push_back(nIdx);
			return ;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
		nWinCount++;
	}
}

void CWindowMgr::WindowSelectPop(int nWindowNum)
{
	int nWinCount = 0;
	m_WinIt = m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(nWinCount == nWindowNum)
		{
			SAFE_DELETE(GWIN_OBJ);
			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);
			return ;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
		nWinCount++;
	}
}

VOID * CWindowMgr::GetWindow(int nWindowType)
{
	if(m_WindowList.empty())
	{
		return NULL;
	}

	if(!IsOpenWindow(nWindowType))
	{
		return NULL;
	}

	m_WtIt = find(m_WinTypeList.begin(), m_WinTypeList.end(), nWindowType);

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		if(nWindowType == m_WinTypeList[i])
		{
			return m_WindowList[i];
		}
	}

	return NULL;
}

WINDOW_ID CWindowMgr::GetNewWndID()
{
	static WINDOW_ID WndID = 0;

	WndID++;

	return WndID;
}


bool CWindowMgr::IsMouseUpperWindow(int x, int y)
{
	if(m_WindowList.empty())
	{
		return false;
	}

	if( m_WindowList[m_WindowList.size()-1]->IsInside( x, y) )
		return true;

	return false;
}
//#endif

/*********************************************************************************************************************************/
CWindowTgaMgr::CWindowTgaMgr()
{
	Clear();
	Init();
}

CWindowTgaMgr::~CWindowTgaMgr()
{
	Destroy();
	Clear();
}

void CWindowTgaMgr::Init()
{
	if(!InitWindow())
	{
		OutputDebugString("Window Object Factory Fail\n");
	}
}

// 윈도우클래스를 객체팩토리에 등록
bool CWindowTgaMgr::InitWindow()
{
	if(!m_WindowFactory.Register(WINDOW_TGA_WANTED_PAGE	   , new Creator<CWantedStartPage, CUITgaBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_TGA_WANTED_REGISTER, new Creator<CWantedRegister , CUITgaBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_TGA_WANTED_MYMENU  , new Creator<CWantedMyMenu   , CUITgaBaseWindow>))
	{
		return false;
	}
	if(!m_WindowFactory.Register(WINDOW_TGA_WANTED_ALLLIST , new Creator<CWantedList     , CUITgaBaseWindow>))
	{
		return false;
	}

	return true;
}

void CWindowTgaMgr::Clear()
{
	m_bActiveWindow = false;
	m_WindowList.clear();
	m_WinTypeList.clear();
	m_WinIDList.clear();
}

void CWindowTgaMgr::Destroy()
{
	AllCloseWindow();
}

void CWindowTgaMgr::DeleteRes()
{
	if(m_WindowList.empty())
	{
		return ;
	}

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		m_WindowList[i]->DeleteRes();
	}
}

void CWindowTgaMgr::RestoreSurface()
{
	if(m_WindowList.empty())
	{
		return ;
	}

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		m_WindowList[i]->RestoreSurfaces();
	}
}

LRESULT CWindowTgaMgr::WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(m_WindowList.empty())
	{
		return false;
	}

	bool bInputIn = false;

	int nTopWinNum = (m_WindowList.size()-1);

	if(UpdateActiveState(msg, LOWORD(lParam), HIWORD(lParam)))
	{
		bInputIn = true;
	}
	// 각 윈도우의 입력 프로세스의 상태를 받는다.
	INT nWindowState = m_WindowList[nTopWinNum]->MsgProc(hWnd, msg, wParam, lParam);

	// 상태에 따른 처리를 한다.
	switch(nWindowState)
	{
	case IM_TGA_WIN_INPUT:
		bInputIn = true;
		break;
	case IM_TGA_WIN_CLOSE:
		CloseWindow(nTopWinNum);
		break;
	}

	return bInputIn;
}


/************************************************************************/
/*
	bool CWindowMgr::UpdateActiveState()

	현재 사용하고 있는 창을 업데이트 시켜준다.
	                                                                    */
/************************************************************************/
bool CWindowTgaMgr::UpdateActiveState(UINT msg, int x, int y)
{
	if(msg != WM_LBUTTONDOWN) return false;

	for(int i = (m_WindowList.size()-1); i >= 0; i--)
	{
		// 만약 i번째 윈도우를 클릭했다면 최상위로 올려주고 루프를 나온다.
		if(m_WindowList[i]->IsInside(x, y))
		{
			ClickWindow(i);
			return true;
		}
		// 마지막윈도우까지도 클릭한 것이 없다면 윈도우가 열려는 있되 활성이 안된 상태를 만든다.
		if(i == 0)
		{
			ClickOutside();
		}
	}

	return false;
}

void CWindowTgaMgr::Draw()
{
	if(m_WindowList.empty())
	{
		return ;
	}

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		m_WindowList[i]->Render();
	}
}

bool CWindowTgaMgr::OnOpenWindow(int nWindowType)
{
	if(IsOpenWindow(nWindowType))
	{
		return false;
	}

	AllCloseWindow();

	CUITgaBaseWindow * pWindow = m_WindowFactory.Create(nWindowType);
	assert(pWindow);

	{
		pWindow->InitMode();
		pWindow->LoadRes();

		pWindow->SetWndID(GetNewWndID());

		m_WinIDList.push_back(pWindow->GetWndID());
		m_WinTypeList.push_back(nWindowType);
		m_WindowList.push_back(pWindow);

		m_bActiveWindow = true;

		return true;
	}

	SAFE_DELETE(pWindow);

	return false;
}

bool CWindowTgaMgr::OnCloseWindow(int nWindowType)
{
	if(!IsOpenWindow(nWindowType))
	{
		return false;
	}

	m_WinIt	= m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(nWindowType == TWIN_TYPE)
		{
			SAFE_DELETE(TWIN_OBJ);
			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);
			return true;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
	}

	if(m_WindowList.empty())
	{
		m_bActiveWindow = false;
	}

	return false;
}

bool CWindowTgaMgr::OnCloseWindow(WINDOW_TGA_ID WndID)
{
	if(!IsOpenWindow(WndID))
	{
		return false;
	}

	m_WinIt	= m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(WndID == TWIN_ID)
		{
			SAFE_DELETE(TWIN_OBJ);
			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);

			if(m_WindowList.empty())
			{
				m_bActiveWindow = false;
			}

			return true;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
	}

	return false;
}

void CWindowTgaMgr::AllCloseWindow()
{
	if(!m_WindowList.empty() && !m_WinTypeList.empty() && !m_WinIDList.empty())
	{
		int nWinMaxNum = m_WindowList.size();

		for(int i = 0; i < nWinMaxNum; i++)
		{
			SAFE_DELETE(m_WindowList[i]);
		}

		m_WindowList.clear();
		m_WinTypeList.clear();
		m_WinIDList.clear();
	}
}

int CWindowTgaMgr::GetTopWindow()
{
	if(m_WinTypeList.empty())
	{
		return -1;
	}

	return (m_WinTypeList[m_WinTypeList.size()-1]);
}

bool CWindowTgaMgr::IsOpenWindow(int nWindowType)
{
	m_WtIt = find(m_WinTypeList.begin(), m_WinTypeList.end(), nWindowType);

	if(m_WinTypeList.end() == m_WtIt)
	{
		return false;
	}

	return true;
}

bool CWindowTgaMgr::IsOpenWindow(WINDOW_TGA_ID WndID)
{
	m_IDIt = find(m_WinIDList.begin(), m_WinIDList.end(), WndID);

	if(m_WinIDList.end() == m_IDIt)
	{
		return false;
	}

	return true;
}

bool CWindowTgaMgr::IsOpenWindow()
{
	if(m_WindowList.empty()) return false;

	return true;
}

// 여기서 받는 인자는 윈도우 타입이 아니라 몇번째 열린 윈도인가이다.
void CWindowTgaMgr::ClickWindow(int nWindowNum)
{
	WindowSelectTop(nWindowNum);
	m_bActiveWindow = true;
}

void CWindowTgaMgr::ClickOutside()
{
	m_bActiveWindow = false;
}

void CWindowTgaMgr::CloseWindow(int nWindowNum)
{
	WindowSelectPop(nWindowNum);
	m_bActiveWindow = false;
}


void CWindowTgaMgr::WindowSelectTop(int nWindowNum)
{
	int nWinCount = 0;
	m_WinIt = m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(nWinCount == nWindowNum)
		{
			CUITgaBaseWindow * pWindow	= TWIN_OBJ;
			int nIdx					= TWIN_TYPE;
			WINDOW_TGA_ID WndID			= TWIN_ID;

			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);

			m_WinIDList.push_back(WndID);
			m_WindowList.push_back(pWindow);
			m_WinTypeList.push_back(nIdx);
			return ;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
		nWinCount++;
	}
}

void CWindowTgaMgr::WindowSelectPop(int nWindowNum)
{
	int nWinCount = 0;
	m_WinIt = m_WindowList.begin();
	m_WtIt	= m_WinTypeList.begin();
	m_IDIt	= m_WinIDList.begin();

	while(m_WinIt != m_WindowList.end())
	{
		if(nWinCount == nWindowNum)
		{
			SAFE_DELETE(TWIN_OBJ);
			m_WindowList.erase(m_WinIt);
			m_WinTypeList.erase(m_WtIt);
			m_WinIDList.erase(m_IDIt);
			return ;
		}

		m_WinIt++;
		m_WtIt++;
		m_IDIt++;
		nWinCount++;
	}
}

VOID * CWindowTgaMgr::GetWindow(int nWindowType)
{
	if(m_WindowList.empty())
	{
		return NULL;
	}

	if(!IsOpenWindow(nWindowType))
	{
		return NULL;
	}

	m_WtIt = find(m_WinTypeList.begin(), m_WinTypeList.end(), nWindowType);

	for(int i = 0; i < m_WindowList.size(); i++)
	{
		if(nWindowType == m_WinTypeList[i])
		{
			return m_WindowList[i];
		}
	}

	return NULL;
}

WINDOW_TGA_ID CWindowTgaMgr::GetNewWndID()
{
	static WINDOW_TGA_ID WndID = 0;

	WndID++;

	return WndID;
}

bool CWindowTgaMgr::IsMouseUpperWindow(int x, int y)
{
	if(m_WindowList.empty())
	{
		return false;
	}

	if( m_WindowList[m_WindowList.size()-1]->IsInside( x, y) )
		return true;

	return false;
}
