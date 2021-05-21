#ifndef _NOTICE_LIST_H_
#define _NOTICE_LIST_H_

#include "Config.h"

class CControlLetter;

class CNoticeLineList
{
public:
	CNoticeLineList();
	~CNoticeLineList();

	int m_nNoticeCount, m_nLineNum;
	char m_strTitle[20], m_strDate[20];

	void AddData(int nNoticeCount, int nLineNum, char *strTitle, char *strDate);

};

class CNoticeList
{

public:
	CControlLetter *Owner_CL;

	CSurface * m_pBack;
	CSurface * m_pSecret;

	CTextOutBox	*m_pNoticeText;

	CTextOutBox	*m_pLineNumText[7];
	CTextOutBox *m_pTitleText[7];
	CTextOutBox *m_pDateText[7];
	char *m_pNoticeLineIndex[7];
	BOOL	m_bIsViewPrev;
	BOOL	m_bIsViewNext;


	CBasicButton * m_pBtn_Prev;
	CBasicButton * m_pBtn_Next;

	CTextOutBox  * m_pTextOut;
	char	*m_strPageNum;
	int m_nPageNum;

	int		m_iLineCount;

	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	BOOL		m_IsAllView;

public:
	CNoticeList(CControlLetter *Owner);
	virtual ~CNoticeList();

	void	AddRoomData(int nRoomNum, int nRoomType, char *strRoomName, int nCount, int nMaxCount, char *strMaster, int nState, int nMinLevel, int nMaxLevel, int nFee, char *strPasswd);
	void	ClearAllRoomList();

	void	GetRoomList(BOOL IsAllView, int PageNum);
	int		SelectRoom(int cur_room);
	BOOL	EnterRoom(int cur_sel, char* password = NULL);

	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);

	int		IsNoticeLineInside(int x,int y);
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();

};

#endif // _NOTICE_LIST_H_