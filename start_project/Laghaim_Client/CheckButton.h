#if !defined(_CHECK_BUTTON_)
#define _CHECK_BUTTON_

#include "Config.h"

#include "ddutil.h"


enum
{
	BUTTON_UNCHECK   = 0,
	BUTTON_CHECK    ,
	BUTTON_NUM
};


class CCheckbutton
{
public:
	static CCheckbutton* Create(int nNum = 2);

	~CCheckbutton();

public:
	// get function
	int  GetState();

	BOOL GetEnable()
	{
		return m_bDisable;
	}
	// set function
	void SetPosition( int x, int y )
	{
		m_nPosX = x, m_nPosY = y;
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
	// Logic function
	bool IsInside( int, int );
	// resource
	void LoadRes( int w = -1, int h = -1 );
	void DeleteRes();
	// render
	void Draw();
	void Draw( int x, int y, BOOL bChecked = FALSE );
	void DrawToBlt(BOOL bChecked = FALSE);


	BOOL DrawClip(int nMinArea, int nMaxArea, int nScrollPos, BOOL bCheck);
	BOOL IsInsideClip(int x, int y, int nMinArea, int nMaxArea, int nScrollPos);
	void MsgProcClip( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScrollPos, int nMin, int nMax);
	// message
	bool MsgProcGroup( HWND, UINT, WPARAM, LPARAM );
	void MsgProc( HWND, UINT, WPARAM, LPARAM );

public:
	int	m_nPosX;
	int	m_nPosY;
private:

	unsigned int	m_uiwidth ;
	unsigned int	m_uiheight;

	int	m_nUsingCnt;

	int	m_iBtnFlg;

	BOOL m_bDisable;
	BOOL m_bVisible;
	BOOL m_bDown   ;

	CSurface* m_pUnCheckSurface;
	CSurface* m_pCheckSurface  ;

	char m_szuncheckfile[80];	// uncheck
	char m_szcheckfile[80];		// check

	RECT m_BtnRc;

	CCheckbutton();

};

#endif
//#endif //_CHECK_BUTTON_