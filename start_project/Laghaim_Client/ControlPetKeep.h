#ifndef _CONTROL_PET_KEEP_H_
#define _CONTROL_PET_KEEP_H_

#include "Config.h"

class CSurface;
class IndexedTexture;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;


class CControlPetKeep
{
public:

	CSurface		*m_pBack;

	CBasicButton	m_CloseBtn;
	CBasicButton	m_PutBtn;
	CBasicButton	m_PickUpBtn;

	CTextOutBox		m_pPetName;
	CTextOutBox		m_pPetLevel;
	CTextOutBox		m_pPetHealth;
	CTextOutBox		m_pPetLoyalty;
	CTextOutBox		m_pPetSatiety;
	CTextOutBox		m_pPetDelayTime;

	CTextOutBox		m_pCur_money;
	CTextOutBox		m_pPay_money;

	BOOL			m_bActive;
	int				m_iMainX, m_iMainY;
	int				m_ScreenWidth, m_ScreenHeight;

	int				Pet_Mode;
	__int64				m_PetKeepPay;
	int				m_PetKeepTime;

	DWORD			m_dwLastOpenTime;
	bool			m_IsOpenWindow;

public:
	CControlPetKeep();
	virtual ~CControlPetKeep();

	void	LoadRes();
	void	SetKeepInfo(char *Name, int KeepMode, int level, int health, int loyalty, int hunger, int pay, int time);
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL	IsInside(int x, int y);
	void	SendChMsg(int PetMode);
};

#endif // _CONTROL_PET_KEEP_H_