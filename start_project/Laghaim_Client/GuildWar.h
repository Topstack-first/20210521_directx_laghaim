#ifndef _GUILD_WAR_H_
#define _GUILD_WAR_H_

#include "Config.h"

class CSurface;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;

class CGuildWar
{
public:
	CSurface * m_pBack;


	CBasicButton * m_pBtn_Req;
	CBasicButton * m_pBtn_Close;
	CBasicButton * m_pBtn_Cancel;
	CBasicButton * m_pBtn_Agree;
	CBasicButton * m_pBtn_reject;
	CBasicButton * m_pBtn_Start;

	CBasicButton * m_pBtn_ArrowUp;
	CBasicButton * m_pBtn_ArrowDown;


	CMsgPopUp    * m_pMsgPopUp;
	CTextOutBox  * m_pTextOut;
	CTextBox	 * m_pTextIn[3];


	int  	   m_iMode;
	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	int		   m_iPersonNum;
	char	   m_strA[30];
	char       m_strB[30];
	char       m_strMoney[30];




public:
	CGuildWar(int mode = 0);
	virtual ~CGuildWar();

	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	BOOL	SendMsg_Open();
	BOOL	SendMsg_Req();
	BOOL	SendMsg_Accept();
	BOOL	SendMsg_Cancel();
	void	GetMsg(int, char*, char*, char*, char*);

	BOOL	CheckAllItem();
	BOOL	CheckSpace(char* String);
};

#endif // GUILD_WAR_H
