#ifndef _GUARD_TOWER_H_
#define _GUARD_TOWER_H_

#include "Config.h"

class CSurface;
class IndexedTexture;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;

#define _NUM_ATT_MODE	3
#define _NUM_TOP		3
#define _NUM_BODY		3
#define _NUM_SHIELD		5

class CGuardTower
{
public:
	CGuardTower(int, int, int);
	virtual ~CGuardTower();

	void	Draw();
	void	RenderOj(int xpos, int ypos, int cx, int cy);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	void	ChangeTop(int right_left);
	void	ChangeBody(int right_left);
	void	ChangeShield(int right_left);
	void	SendChMsg();

	int GetCurPrice_Body();
	int GetCurPrice_Shield();
	int GetCurPrice_Top();

public:
	D3DTLVERTEX				TLVec[4];

	CSurface		* m_pBack;
	CSurface        * m_pSurTop[3];
	CSurface        * m_pSurBody[3];
	CSurface        * m_pSurShield[5];

	CBasicButton	* m_pBtn1;
	CBasicButton	* m_pBtn2;
	CBasicButton    * m_pBtnRight[3];
	CBasicButton    * m_pBtnLeft[3];

	CTextOutBox		* m_pTop_mode;
	CTextOutBox		* m_pTop_level;
	CTextOutBox		* m_pTop_name;

	CTextOutBox		* m_pBody_name;
	CTextOutBox		* m_pBody_level;
	CTextOutBox		* m_pShield;
	CTextOutBox		* m_pCur_money;
	CTextOutBox		* m_pPay_money;

	POINT price_top_fire[4];
	POINT price_top_light[4];
	POINT price_top_stone[4];
	POINT price_body[3];
	POINT price_shield[5];

	int m_iprice_top[3];
	int m_iprice_body[3];
	int m_iprice_shield[5];



	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;

	int		   m_iAtt_mode, m_iTop_level;
	int        m_iBody_level, m_iBody_mode;
	int        m_iShield_level;
	int		   m_iMobIdx;
	int		   m_iOld_Att_mode, m_iOld_Body_mode, m_iOld_Shield_level;
	int        m_iPrice_Top, m_iPrice_Body, m_iPrice_Shield;

	void ShowCurPrice();
};

#endif // _GUARD_TOWER_H_



