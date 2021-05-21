#ifndef _LETTER_REJECT_H_
#define _LETTER_REJECT_H_

#include "Config.h"

class CControlLetter;

class CLetterReject
{
public:
	CControlLetter *Owner_CL;
	CSurface * m_pBack;

	CTextBox	*m_pAddRejectText;
	CTextOutBox	*m_pDelRejectText[5];

	CBasicButton * m_pBtn_Add;
	CBasicButton * m_pBtn_Del;

	CBasicButton * m_pBtn_Up;
	CBasicButton * m_pBtn_Down;

	char	*m_pBlockList[50];
	int		m_nSelectLine;
	int			m_nRejectLineCount;
	int		   m_nPageNum, m_nCurLineNum;
	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	BOOL		m_IsAllView;
	int			m_nStartLine;
	int		m_nAllRejectNum;

public:
	CLetterReject(CControlLetter *Owner);
	virtual ~CLetterReject();
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	BOOL	IsInside_Prompt(int x, int y);
	int		IsLineInside(int x, int y);
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();
};

#endif // _LETTER_REJECT_H_
