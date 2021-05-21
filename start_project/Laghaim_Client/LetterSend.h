#ifndef _LETTER_SEND_H_
#define _LETTER_SEND_H_

#include "Config.h"

#include "ListBox.h"
#include "TextBox.h"
#include "LetterBox.h"

class CControlLetter;

class CLetterSend
{
public:
//나중에 친구 선택 버튼과 CLetterBox,CInputBox해주자...
	CControlLetter *Owner_CL;
	CSurface * m_pBack;
	CSurface * m_pListBack;
	CSurface * m_pSecret;

	CTextOutBox	*m_pCharNameText;
	CTextBox *m_pRecNameText;
	CLetterBox *m_pSendLetterText;
//	CListBox *m_pFriendList;
	CTextOutBox *m_pViewFriendList[8];


	CBasicButton * m_pBtn_Send;
	CBasicButton * m_pBtn_List;
	CBasicButton * m_pBtn_Up;
	CBasicButton * m_pBtn_Down;

	CTextOutBox  * m_pTextOut;

	int		   m_nPageNum, m_nCurLineNum;
	BOOL		m_bViewList;
	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	BOOL		m_IsAllView;

	int		m_nFriendLineCount;
	int		m_nSelectLine;
	int		m_nStartLine;
	int		m_nAllFriendNum;
	char	*m_pFriendList[50];

public:
	CLetterSend(CControlLetter *Owner);
	virtual ~CLetterSend();
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	BOOL	IsInside_Letter_Prompt(int x, int y);
	BOOL	IsInside_RecName_Prompt(int x, int y);
	int		IsLineInside(int x, int y);
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();
};

#endif // _LETTER_SEND_H_
