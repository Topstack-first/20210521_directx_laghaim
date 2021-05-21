#ifndef _LETTER_VIEW_H_
#define _LETTER_VIEW_H_

#include "Config.h"

class CControlLetter;
class CLetterBox;

class CLetterView
{
public:
	CControlLetter *Owner_CL;
	CSurface * m_pBack;
	CSurface * m_pSecret;

	CTextOutBox	*m_pSenderText;
//	CTextOutBox	*m_pLetterViewText;
	CLetterBox	*m_pLetterViewText;

	CBasicButton * m_pBtn_List;
	CBasicButton * m_pBtn_Report;
	CBasicButton * m_pBtn_Reject;
	CBasicButton * m_pBtn_friend;
	CBasicButton * m_pBtn_Reply;
	CBasicButton * m_pBtn_Keep;
	CBasicButton * m_pBtn_Del;

	CBasicButton * m_pBtn_Up;
	CBasicButton * m_pBtn_Down;

	char *m_pLetterIndex;

	int		   m_nPageNum, m_nCurLineNum;
	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	BOOL		m_IsAllView;
	BOOL		m_bLetterIsNew;

public:
	CLetterView(CControlLetter *Owner);
	virtual ~CLetterView();
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();
};

#endif // _LETTER_VIEW_H_
