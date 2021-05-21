#ifndef _MAT_MAKE_ROOM_H_
#define _MAT_MAKE_ROOM_H_

#include "Config.h"

class CSurface;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;


#define MAX_MATMAKEROOM_TEXTBOX 5

class CMatMakeRoom
{
public:
	char m_strTitle[100], m_strPassWord[4], m_strPay[6];
	int  m_nRoomType, m_nMaxCount, m_nMinLevel, m_nMaxLevel, m_nPay;
	bool m_bLevelNoLimit;				// 레벨 무제한인지 체크

	CSurface*	m_pBack;

	CSpinBox	m_NumBox;
	CSpinBox    m_NumBox2;
	CSpinBox	m_TypeBox;

	CBasicButton*	m_pBtn_Make;
	CBasicButton*	m_pBtn_Close;
	CBasicButton*	m_LimitBtn[2];		// 레벨 무제한 버튼

	// TITLE, PASSWORD,	PAY, MINLEVEL, MAXLEVEL
	CTextBox*	m_pTextIn[MAX_MATMAKEROOM_TEXTBOX];

	CMsgPopUp*	m_pMsgPopUp;


	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;

public:
	CMatMakeRoom();
	virtual ~CMatMakeRoom();

	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);

	BOOL	SendMakeRoom();
};

#endif // _MAT_MAKE_ROOM_H_