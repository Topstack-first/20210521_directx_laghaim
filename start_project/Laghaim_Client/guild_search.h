#ifndef _GUILD_SEARCH_H_
#define _GUILD_SEARCH_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include <list>
#include <map>

#include "basewindow.h"
#include "basicbutton.h"
#include "TextBox.h"
#include "Scroll.h"
#include "ComboBox.h"

using namespace std;


typedef struct _stsearch
{
	char guild_name[256];
	char guild_master_name[256];
	int	 guild_rank;
	int  guild_glevel;
	int	 guild_point;
	int	 guild_pageNum;

	_stsearch()
	{
		ZeroMemory(guild_name, sizeof(guild_name));
		ZeroMemory(guild_master_name, sizeof(guild_master_name));
		guild_rank    = 0;
		guild_glevel  = 0;
		guild_point   = 0;
		guild_pageNum = 0;
	}

	_stsearch& operator=(const _stsearch &info)
	{
		strcpy(guild_name, info.guild_name);
		strcpy(guild_master_name, info.guild_master_name);
		guild_rank    = info.guild_rank ;
		guild_glevel  = info.guild_glevel;
		guild_point   = info.guild_point;
		guild_pageNum = info.guild_pageNum;

		return (*this);
	}
} g_listInfo, *g_plistInfo;

typedef list<g_listInfo>	list_guild;

typedef map< int, list_guild > list_guild_page;

class CTextBoard;
class CMutiListBox;

class CGuildSearch : public CUIBaseWindow
{
	enum
	{
		CW_GUILD_NAME  = 0,
		CW_GUILD_MASTER,
		CW_GUILD_RANK,
		CW_GUILD_LEVEL,
		CW_GUILD_POINT,
		CW_GUILD_BOARD_TOTAL
	};

public:
	CGuildSearch();
	virtual ~CGuildSearch();

	void	clear();
	/*********************************************************************/
	int		Draw();
	void	RestoreSurfaces();
	void	DeleteRes();
	BOOL    IsInside(int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*********************************************************************/
	BOOL	InitMode();
	void	LoadRes();

	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}
	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();
	/*********************************************************************/
	void	guilds_insertlist(g_listInfo& g_list);
	void	guilds_insertpagelist(g_listInfo& g_list);
	/*********************************************************************/
	int	    guilds_GetTotalpage() const
	{
		return m_nTotalPage;
	}

	void	set_start(BOOL bStart);
	BOOL	get_start() const
	{
		return m_bStart;
	}
	char*	Get_SelectGuildName();

	void	set_start_ani();
	void	set_end_ani();

	void	all_Reset();

	BOOL	IsFocus()
	{
		return m_tboxsearch.GetEnable();
	}

private:
	list_guild		m_listguild;
	list_guild_page	m_listPage;

	int				m_nTotalPage;
	int				m_nCurrentPage;
	int				m_nMaxLine;
	int				m_nAniCnt;

	BOOL			m_bStart; // 검색 시작, 끝

	CMutiListBox*	m_pMultiList;
	CTextBox		m_tboxsearch;

	CBasicButton	m_btRequest;
	CBasicButton	m_btSearch;
	CBasicButton	m_btBack;

	CComboBox		m_ComboBox;

	DWORD			m_dwAniTime;

	CSurface*		m_pIngSurface[5];
};
#endif // _GUILD_SEARCH_H_