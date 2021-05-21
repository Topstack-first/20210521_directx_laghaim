#ifndef _CONTROL_PET_SHOP2_H_
#define _CONTROL_PET_SHOP2_H_

#include "Config.h"

class CSurface;
class IndexedTexture;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;


enum PetShop_Status
{
	PetShop_Status__None,
	PetShop_Status__Rabi,
	PetShop_Status__Pera,
};

class CControlPetShop2
{
public:
	CControlPetShop2();
	~CControlPetShop2();

	void	LoadRes();
	void	DeleteRes();
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	void	ShowCurPrice();
	void	SendChMsg();
	void	SelectPetEgg();
	int		GetCurPrice_PetEgg();

public:
	CSurface		*m_pBack;

	CSurface        *m_pSurPetEgg[NUM_RACE];
	CBasicButton	m_CloseBtn;
	CBasicButton	m_BuyBtn;
	CTextOutBox		m_pCur_money;
	CTextOutBox		m_pPay_money;
	BOOL			m_bActive;
	int				m_iPetEgg_race;
	DWORD			m_dwLastOpenTime;
	bool			m_IsOpenWindow;

public:
	// "새로운 애완동물 키우기 창" 은 여기서 부터..
	CBasicButton    m_Btn_Ok;
	CBasicButton    m_Btn_Cancel;
	BOOL			m_bIsOpenWindow2;


	PetShop_Status	m_petshop_status;
};

#endif // _CONTROL_PET_SHOP2_H_


