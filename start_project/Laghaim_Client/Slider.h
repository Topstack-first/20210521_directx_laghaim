#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "Config.h"

#include "ddutil.h"
#include "basicbutton.h"

enum
{
	BUTTON_UNCHECK1   = 0,
	BUTTON_CHECK1    ,
	BUTTON_NUM1
};

#define MAX_VOLUME	100.f

class CSliderButton
{
public:
	static CSliderButton* Create(int nNum = 2);

	~CSliderButton();

public:
	// get function
	int  GetState();

	BOOL GetEnable()
	{
		return m_bDisable;
	}
	float GetNowPer() const
	{
		return m_fPer;
	}

	int	GetNowLevel() const
	{
		return m_fPer*MAX_VOLUME;
	}
	// set function

	void SetNowLevelPer(float fLevel)
	{
		m_fPer = fLevel/MAX_VOLUME;
	}
	void SetPosition( int x, int y )
	{
		m_nPosX = x, m_nPosY = y;
	}

	void SetPositionSlider( int x, int y )
	{
		m_fBackBasisX = x;
		m_fBackBasisY = y;
	}

	// 임시 급 추가..^^
	void SetPositionPM(int px, int py, int mx, int my)
	{
		m_btPlus.SetPosition(px, py);
		m_btMinus.SetPosition(mx, my);
	}

	void SetFileName( char* );
	void SetState(int state)
	{
		m_iBtnFlg = state;
	}
	void SetVisible(BOOL bVisible)
	{
		m_bVisible = bVisible;
	}
	void SetStatus(BOOL bable);
	void SetPercentage(float fPer)
	{
		m_fPer = fPer;
	}
	// Logic function
	bool IsInsidebt( int, int );
	bool IsInside( int, int );
	// resource
	void LoadRes( int w = -1, int h = -1 );
	void DeleteRes();
	// render
	void Draw();
	void Draw( int x, int y, BOOL bChecked = FALSE );
	void DrawToBlt(BOOL bChecked = FALSE);
	// message
	void MsgProc( HWND, UINT, WPARAM, LPARAM );

private:

	unsigned int	m_uiwidth ;	// 슬라이더 단추
	unsigned int	m_uiheight;

	unsigned int	m_uibwidth ; // 슬라이더 백그라운드
	unsigned int	m_uibheight;

	int	m_nUsingCnt;
	int	m_nPosX;
	int	m_nPosY;
	int	m_iBtnFlg;

	float m_fBackBasisX;
	float m_fBackBasisY;

	BOOL m_bDisable;
	BOOL m_bVisible;
	BOOL m_bDown   ;

	BOOL m_bbtDown;

	CSurface* m_pUnCheckSurface;
	CSurface* m_pCheckSurface  ;

	char m_szuncheckfile[80];	// uncheck
	char m_szcheckfile[80];		// check

	RECT m_BtnRc;

	float m_fPer;	   // 현재 수치 ( 0 ~ 1 )
	float m_fMaxWidth; // 슬라이더 최대 길이

	CBasicButton	m_btPlus;
	CBasicButton	m_btMinus;

	CSliderButton();
};

#endif // _SLIDER_H_