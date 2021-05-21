#ifndef _WAR_REQUEST_H_
#define _WAR_REQUEST_H_

#include "Config.h"

class CSurface;
class CTextOutBox;
class CBasicButton;
class CMsgPopUp;
class CTcpIpConnection;

class CWarRequest
{
public:
	CWarRequest();
	virtual ~CWarRequest();

	void        GetMsg(char* , int, int);
	void		Draw();
	LRESULT		MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL		IsInside(int x, int y);


public:
	CSurface	 * m_pBack;
	CBasicButton * m_pBtn_Request;
	CBasicButton * m_pBtn_Time;
	CBasicButton * m_pBtn_ArrowUp;
	CBasicButton * m_pBtn_ArrowDown;
	CBasicButton * m_pBtn_Close;
	CTextOutBox  * m_pText_Day; 		// ����
	CTextOutBox  * m_pText_Time;     	// �ð�
	CTextOutBox  * m_pText_Info;		// ����
	CTextOutBox  * m_pText_Info2;		// ����
	char        m_MsgBuf[1024];
	BOOL	    m_bActive;
	int			m_ScreenCx, m_ScreenCy, m_iMainX, m_iMainY;
	BOOL        m_bInfoTwoLine, m_iType, m_iTime;
	// ���� Ŭ���̾�Ʈ �޼��� ����.
	void SendMsgOpen();
	void SendMsgReq();
	void SendMsgTime(int time);

};

#endif // _WAR_REQUEST_H_



