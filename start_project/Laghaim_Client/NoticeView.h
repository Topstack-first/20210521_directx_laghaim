#ifndef _NOTICE_VIEW_H_
#define _NOTICE_VIEW_H_

#include "Config.h"

class CControlLetter;
class CLetterBox;

class CNoticeView
{
public:
	CControlLetter *Owner_CL;

	CSurface * m_pBack;

	CTextOutBox	*m_pTitleText;
	CLetterBox	*m_pNoticeViewText;


	CBasicButton * m_pBtn_List;

	CBasicButton * m_pBtn_Prev;
	CBasicButton * m_pBtn_Next;
	CBasicButton * m_pBtn_Up;
	CBasicButton * m_pBtn_Down;

	CTextOutBox  * m_pTextOut;

	int		m_nPageNum, m_nCurLineNum;
	BOOL    m_bActive;
	int		m_iMainX, m_iMainY;
	int     m_ScreenWidth, m_ScreenHeight;
	BOOL	m_bIsViewPrev;
	BOOL	m_bIsViewNext;
	int	m_nNoticeIndex;
	char	*m_pNoticeIndex;

	BOOL		m_IsAllView;

public:
	CNoticeView(CControlLetter *Owner);
	virtual ~CNoticeView();
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();
};

#endif // _NOTICE_VIEW_H_
