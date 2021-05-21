#ifndef _LETTER_FRIEND_H_
#define _LETTER_FRIEND_H_

#include "Config.h"

class CControlLetter;

#define MAX_FRIENDLIST 50

class CLetterFriend
{
public:
	CControlLetter *Owner_CL;
	CSurface * m_pBack;

	CTextOutBox	*m_pDelFriendText[5];

	CTextBox *m_pAddFriendText;

	CBasicButton * m_pBtn_Add;
	CBasicButton * m_pBtn_Del;
	CBasicButton * m_pBtn_Up;
	CBasicButton * m_pBtn_Down;

	char	*m_pFriendList[MAX_FRIENDLIST];
	int		   m_nPageNum, m_nCurLineNum;
	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	BOOL		m_IsAllView;
	int		m_nFriendLineCount;
	int		m_nSelectLine;
	int		m_nStartLine;
	int		m_nAllFriendNum;

public:

	CLetterFriend(CControlLetter *Owner);
	virtual ~CLetterFriend();
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	BOOL	IsInside_Prompt(int x, int y);
	int		IsLineInside(int x, int y);
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();
};

#endif // _LETTER_FRIEND_H_
