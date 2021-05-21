#ifndef _CONTROL_TELEPORT_ITEM_H_
#define _CONTROL_TELEPORT_ITEM_H_

#include "Config.h"

class CSurface;
class IndexedTexture;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;

#define MAX_TELEPORT_COUNT	10

class CControlTeleportItem
{
public:
	CControlTeleportItem();
	virtual ~CControlTeleportItem();

	void	LoadRes(int QuickSlotNum);
	void	DeleteRes();
	void	Draw();
	void	SetDrawPos();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL	IsInside(int x, int y);
	void	SendSavePosMsg(int slotnum);
	int		SendLoadPosMsg(int slotnum);
	void	SetZonePosString(int index, int zonenum, int xpos, int ypos);
	bool	IsBossRoom();

	void	SetSlot(int slot_index)
	{
		m_slot_index = slot_index;
	}

private:
	CSurface		*m_pBack;

	int				m_iCurZoneNum[MAX_TELEPORT_COUNT];
	CBasicButton	m_SavePosBtn[MAX_TELEPORT_COUNT];
	CBasicButton	m_LoadPosBtn[MAX_TELEPORT_COUNT];
	CTextOutBox		m_pCur_ZonePos[MAX_TELEPORT_COUNT];
	CBasicButton    m_CloseBtn;

	BOOL			m_bActive;
	DWORD			m_dwLastOpenTime;
	bool			m_IsOpenWindow;
	bool			m_IsVisible;

protected:
	int m_slot_index;

public:
	int				m_iQuickSlotNum;
};

#endif // _CONTROL_TELEPORT_ITEM_H_

