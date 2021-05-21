#ifndef _WANTED_START_PAGE_H_
#define _WANTED_START_PAGE_H_

#include "Config.h"

#include "baseTgaWindow.h"


class CWantedStartPage : public CUITgaBaseWindow
{
public:
	CWantedStartPage();
	virtual ~CWantedStartPage();

public:
	/*********************************************************************/
	int		Render();
	void	Init();
	void	RestoreSurfaces() {};
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

	void	SendMsg();
private :
	CBasicButton	m_btWantedAdd;	// 수배등록
	CBasicButton	m_btMyMenu;		// 나의메뉴
	CBasicButton	m_btWantedlist; // 수배자 리스트
	CBasicButton	m_btPrizeMoney;	// 상금 지급

	char			m_MsgBuf[1024];

};

#endif // _WANTED_START_PAGE_H_