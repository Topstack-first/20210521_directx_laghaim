#ifndef _CONTROL_PET_SHOP_H_
#define _CONTROL_PET_SHOP_H_

#include "Config.h"
#include "NkCharacter.h"

class CSurface;
class IndexedTexture;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;


class CControlPetShop
{
public:
	CControlPetShop();
	virtual ~CControlPetShop();

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
	int				m_iMainX, m_iMainY;
	int				m_ScreenWidth, m_ScreenHeight;
	int				m_iPetEgg_race;
	DWORD			m_dwLastOpenTime;
	bool			m_IsOpenWindow;

	// 서버 클라이언트 메세지 관련.
public:
	// "새로운 애완동물 키우기 창" 은 여기서 부터..
	CSurface*		m_pBack2;
	CBasicButton    m_Btn_Ok;
	CBasicButton    m_Btn_Cancel;
	BOOL			m_bIsOpenWindow2;
};

#endif // _CONTROL_PET_SHOP_H_


