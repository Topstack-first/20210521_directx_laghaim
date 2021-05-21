#ifndef _TGA_BASE_WINDOW_H_
#define _TGA_BASE_WINDOW_H_

#include "Config.h"

#include "d3dutil.h"
#include "basicbutton.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

enum WND_INPUT_TGA_MSG
{
	IM_TGA_WIN_NONE	= 0,
	IM_TGA_WIN_INPUT,
	IM_TGA_WIN_CLOSE,
};

typedef unsigned long WINDOW_TGA_ID;

class CSurface;
class IndexedTexture;

class CUITgaBaseWindow
{
protected:
	int			m_nPosX;
	int			m_nPosY;
	int			m_nPosFX;
	int			m_nPosFY;

	RECT		m_MoveBarRange;
	BOOL		m_bMove;

	CBasicButton* m_pBtExit;

	WINDOW_TGA_ID	m_WndId;

	UINT  		m_eResolution;

	int			m_nWindowResNum;

	bool		m_bMouseDown;

	IndexedTexture *m_pIndexedTexture;

	int			m_nWidth;
	int			m_nHeight;

	D3DTLVERTEX TLVec[4];
public:
	void SetWndID(WINDOW_TGA_ID id)
	{
		m_WndId = id;
	}
	WINDOW_TGA_ID GetWndID()
	{
		return m_WndId;
	}

public:
	void SetBackground(int nResNum);
	void MakeCloseBtn(char *szName);
	void SetIndex_ptr(IndexedTexture* pIndex)
	{
		m_pIndexedTexture = pIndex;
	}
	void SetWidth(int nWidth)
	{
		m_nWidth = nWidth;
	}
	void SetHeight(int nHeight)
	{
		m_nHeight = nHeight;
	}
	void SetWindomMoveBarRange( int left, int right, int top, int bottom);

	int  GetWidth() const
	{
		return m_nWidth;
	}
	int  GetHeight() const
	{
		return m_nHeight;
	}

public:
	virtual int	 Render();

	virtual BOOL InitMode()
	{
		return TRUE;
	}
	virtual void LoadRes() {  };


	virtual void DeleteRes();
	virtual void RestoreSurfaces() {};
	virtual void SetPosition() {};
	void SetExitBtnPostion(int nX, int nY);

	virtual BOOL MoveWindow(UINT msg, int x, int y);
	virtual BOOL IsInside(int x, int y);
	virtual BOOL IsMoveBarInSide(int x, int y);
	virtual BOOL IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CUITgaBaseWindow();
	virtual ~CUITgaBaseWindow();
};
#endif // _TGA_BASE_WINDOW_