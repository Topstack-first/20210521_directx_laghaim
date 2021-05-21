#if !defined(_BASE_)
#define _BASE_

#include "Config.h"

enum WND_INPUT_MSG
{
	IM_NONE	= 0,
	IM_INPUT,
	IM_CLOSE,
	IM_BACK,
};

typedef unsigned long WINDOW_ID;

class CSurface;
class CBasicButton;

class CUIBaseWindow
{
protected:
	CSurface*	m_pBackScene;

	int			m_nPosX;
	int			m_nPosY;

	int			m_nPosFX;
	int			m_nPosFY;

	bool		m_bMouseDown;

	BOOL		m_bMove;

	CBasicButton* m_pBtExit;

	WINDOW_ID	m_WndId;

	UINT  		m_eResolution;

public:
	void SetWndID(WINDOW_ID id)
	{
		m_WndId = id;
	}
	WINDOW_ID GetWndID()
	{
		return m_WndId;
	}

public:
	void SetBackground(int nResNum);
	void MakeCloseBtn(char *szName);

public:
	virtual int	 Draw();

	virtual BOOL InitMode()
	{
		return TRUE;
	}
	virtual void LoadRes() {  };

	virtual void DeleteRes();
	virtual void RestoreSurfaces() {};
	virtual void SetPosition() {};
	void		 SetExitBtnPostion(int nX, int nY);

	virtual BOOL MoveWindow(UINT msg, int x, int y);
	virtual BOOL IsInside(int x, int y);
	virtual BOOL IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CUIBaseWindow();
	virtual ~CUIBaseWindow();
};
#endif // _BASE_