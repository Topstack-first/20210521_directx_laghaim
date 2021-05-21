#ifndef _LETTER_LIST_H_
#define _LETTER_LIST_H_

#include "Config.h"

#include "TextOutBox.h"

class CControlLetter;

/*class CLetterLineList
{
public:
	CLetterLineList();
	~CLetterLineList();

	int m_nNewCount, m_nKeepCount, m_nNoticeCount, m_nLineNum, m_nStateType;
	char m_strChName[20], m_strDate[20];

	void AddData(int nNewCount, int nKeepCount, int nNoticeCount, int nLineNum, char *strChName, char *strDate, int nStateType);

};
*/
class CLetterList
{
public:
	CControlLetter *Owner_CL;
	CSurface * m_pBack;
	CSurface * m_pSecret;

	CTextOutBox	*m_pNewText;
	CTextOutBox	*m_pKeepText;
	CTextOutBox	*m_pNoticeText;

	CTextOutBox	*m_pLineNumText[7];
	CTextOutBox *m_pChNameText[7];
	CTextOutBox *m_pDateText[7];
	CTextOutBox *m_pStateText[7];

	CBasicButton * m_pBtn_Prev;
	CBasicButton * m_pBtn_Next;


	char*	m_pLineIndex[7];
//	char	m_pLineIndex[7][10];
	int		m_ListNum[7];

	int		m_iLineCount;
	int		m_nPageNum, m_nCurLineNum;
	BOOL	m_bActive;
	int		m_iMainX, m_iMainY;
	int     m_ScreenWidth, m_ScreenHeight;
	BOOL	m_IsAllView;
	BOOL	m_bIsViewPrev;
	BOOL	m_bIsViewNext;
public:
	CLetterList(CControlLetter *Owner);
	virtual ~CLetterList();
	/*
		void	AddLineData(int nNewCount, int nKeepCount, int nNoticeCount, int nLineNum,char *strChName, char *strDate, int nStateType);
		void	ClearAllLineList();

		void	GetLineList(BOOL IsAllView, int PageNum);
		int		SelectLine(int cur_room);
		BOOL	EnterLine(int cur_sel, char* password = NULL);
	*/
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	int		IsLineInside(int x, int y);
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();
};

#endif // _LETTER_LIST_H_
