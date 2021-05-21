#ifndef _CONTROL_LOW_GUILD_MASTER_H_
#define _CONTROL_LOW_GUILD_MASTER_H_

#include "Config.h"

#define BLUE_GUILD 0
#define RED_GUILD 1
#define OUT_GUILD 2

class CSurface;
class IndexedTexture;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;

class CControlLowGuildMaster
{
public:
	CControlLowGuildMaster();
	virtual ~CControlLowGuildMaster();

	void	LoadRes();
	void	DeleteRes();
	void	Draw();
	void	SetDrawPos();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	void	SendGuildMsg(int index);
	void	SetSiegeDateString(char *SiegeDate);

public:

	CSurface		*m_pBack;
//=======================================
	// nate 2004 - 4 : CastleWar
	CSurface		*m_pBack2;
	BYTE			m_byRandomNum;
//=======================================
	CBasicButton	m_BlueBtn;
	CBasicButton	m_RedBtn;
	CBasicButton	m_OutBtn;
	CBasicButton    m_CloseBtn;
	CTextOutBox		m_pSiegeDate;
	BOOL			m_bActive;
	int				m_iMainX, m_iMainY;
	DWORD			m_dwLastOpenTime;
	bool			m_IsOpenWindow;
	bool			m_IsVisible;
};

#endif // _CONTROL_LOW_GUILD_MASTER_H_