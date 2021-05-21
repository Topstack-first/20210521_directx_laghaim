#ifndef _WINDOWMGR_H_
#define _WINDOWMGR_H_

#include "Config.h"

#include "baseTgaWindow.h"
#include "FactoryPattern.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

/************************************************************************/
/* include                                                                      */
/************************************************************************/
#include <vector>
#include <algorithm>
#include "FactoryPattern.h"
#include "basewindow.h"
#include "baseTgaWindow.h"

using namespace std;
/************************************************************************/
/* struct & define                                                                     */
/************************************************************************/
//class CEntityFactory;
//class CUIBaseWindow;

enum UI_WINDOW
{
	///// ��� UI /////////////////////////////
	WINDOW_GUILD_MEMBER,
	WINDOW_GUILD_NONMEMBER,
	WINDOW_GUILD_SEARCH,
	WINDOW_GUILD_CREATE,			// ��� ����
	WINDOW_GUILD_MIX,				// ���� ����
	WINDOW_GUILD_PROMOTION,			// ��� �±�
	WINDOW_GUILD_COMBINATION,		// ��� �±� ������ ����
	WINDOW_GUILD_BATTLE_KIND,
	WINDOW_GUILD_BATTLE_RANKING,
	WINDOW_GUILD_BATTLE_RANKING_MENU,
	WINDOW_GUILD_BATTLE_INVASION,
	//WINDOW_GUILD_BATTLE_RANK_SYSTEM,
	WINDOW_GUILD_BATTLE_1VS1,
	WINDOW_GUILD_BATTLE_FLAG,
	WINDOW_GUILD_BATTLE_OBSTACLE,
	WINDOW_GUILD_BATTLE_ACCEPTMEMBER,
	WINDOW_GUILD_BATTLE_INVASION_INFO,
	WINDOW_GUILD_BATTLE_INVASION_REQUEST,
	WINDOW_GUILD_MEMBER_NOTICE,
	WINDOW_DMITRON_BATTLE,			// ���Ʈ�� ������ - ��û
	WINDOW_DMITRON_BATTLE_INFO,		// ���Ʈ�� ������ - ��û ����
	WINDOW_DMITRON_BATTLE_TAX,		// ���Ʈ�� ������ - ����

	WINDOW_NUM
};

typedef std::vector<CUIBaseWindow * > WINDOW_LIST;
typedef std::vector<int>			  WINDOW_TYPE_LIST;
typedef std::vector<WINDOW_ID>		  WINDOW_ID_LIST;
#define GWIN_OBJ	(*m_WinIt)
#define GWIN_TYPE	(*m_WtIt)
#define GWIN_ID		(*m_IDIt)
/************************************************************************/
/* CWindowMgr                                                                     */
/************************************************************************/
class CWindowMgr
{
private:

	WINDOW_LIST				m_WindowList;
	WINDOW_LIST::iterator	m_WinIt;

	CEntityFactory<CUIBaseWindow>	m_WindowFactory;

	WINDOW_TYPE_LIST			m_WinTypeList;
	WINDOW_TYPE_LIST::iterator	m_WtIt;

	WINDOW_ID_LIST				m_WinIDList;
	WINDOW_ID_LIST::iterator	m_IDIt;

	int		m_PrvWidnow;

	bool	m_bActiveWindow;

public:
	CWindowMgr();
	virtual ~CWindowMgr();

	// ���� �Ҹ� ����
	void Init();
	bool InitWindow();

	void Clear();
	void Destroy();

	void	Draw();
	void	DeleteRes();
	void	RestoreSurface();
	LRESULT WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// �ܺ� ��Ʈ�� �Լ�
	bool OnOpenWindow(int nWindowType);

	void CloseTopWindow()
	{
		OnCloseWindow( GetTopWindow() );	   // �� ���� �ִ� â�� �ݴ´�.
	}
	void OnOpenPrvWindow();

	bool OnCloseWindow(int nWindowType);
	bool OnCloseWindow(WINDOW_ID WndID);
	void AllCloseWindow();

	int  GetTopWindow();
	bool IsMouseUpperWindow(int x, int y);
	bool IsOpenWindow(int nWindowType);
	bool IsOpenWindow(WINDOW_ID WndID);
	bool IsOpenWindow();
	VOID *GetWindow(int nWindowType);
	bool IsHaveActiveWindow()
	{
		return m_bActiveWindow;
	}

private:
	bool UpdateActiveState(UINT msg, int x, int y);
	void ClickWindow(int nWindowNum);
	void ClickOutside();
	void CloseWindow(int nWindowNum);
	void WindowSelectTop(int nWindowNum);
	void WindowSelectPop(int nWindowNum);

	WINDOW_ID GetNewWndID()	;
};

//#endif

/***************************************** TGA WINDOW **************************************************************/
enum UI_TGA_WINDOW
{
	///// ��� UI /////////////////////////////
	WINDOW_TGA_WANTED_PAGE = 0,
	WINDOW_TGA_WANTED_REGISTER,
	WINDOW_TGA_WANTED_MYMENU,
	WINDOW_TGA_WANTED_ALLLIST,
	WINDOW_TGA_NUM
};

typedef std::vector<CUITgaBaseWindow* > WINDOW_LIST_TGA;
typedef std::vector<int>			    WINDOW_TYPE_LIST_TGA;
typedef std::vector<WINDOW_TGA_ID>	    WINDOW_ID_LIST_TGA;
#define TWIN_OBJ	(*m_WinIt)
#define TWIN_TYPE	(*m_WtIt)
#define TWIN_ID		(*m_IDIt)
/************************************************************************/
/* CWindowMgr                                                                     */
/************************************************************************/
class CWindowTgaMgr
{
private:

	WINDOW_LIST_TGA				m_WindowList;
	WINDOW_LIST_TGA::iterator	m_WinIt;

	CEntityFactory<CUITgaBaseWindow>	m_WindowFactory;

	WINDOW_TYPE_LIST_TGA			m_WinTypeList;
	WINDOW_TYPE_LIST_TGA::iterator	m_WtIt;

	WINDOW_ID_LIST_TGA				m_WinIDList;
	WINDOW_ID_LIST_TGA::iterator	m_IDIt;

	bool	m_bActiveWindow;

public:
	CWindowTgaMgr();
	virtual ~CWindowTgaMgr();

	// ���� �Ҹ� ����
	void Init();
	bool InitWindow();

	void Clear();
	void Destroy();

	void	Draw();
	void	DeleteRes();
	void	RestoreSurface();
	LRESULT WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	// �ܺ� ��Ʈ�� �Լ�
	bool OnOpenWindow(int nWindowType);

	void CloseTopWindow()
	{
		OnCloseWindow( GetTopWindow());
	}
	bool OnCloseWindow(int nWindowType);
	bool OnCloseWindow(WINDOW_TGA_ID WndID);
	void AllCloseWindow();

	int  GetTopWindow();
	bool IsMouseUpperWindow(int x, int y);
	bool IsMouseUpperWindow();
	bool IsOpenWindow(int nWindowType);
	bool IsOpenWindow(WINDOW_TGA_ID WndID);
	bool IsOpenWindow();
	VOID * GetWindow(int nWindowType);
	bool IsHaveActiveWindow()
	{
		return m_bActiveWindow;
	}

private:
	bool UpdateActiveState(UINT msg, int x, int y);
	void ClickWindow(int nWindowNum);
	void ClickOutside();
	void CloseWindow(int nWindowNum);
	void WindowSelectTop(int nWindowNum);
	void WindowSelectPop(int nWindowNum);

	WINDOW_TGA_ID GetNewWndID();
};
#endif // !defined(_WINDOWMGR_H_)
