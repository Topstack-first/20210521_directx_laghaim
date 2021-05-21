#ifndef _LHUI_Base_H_
#define _LHUI_Base_H_

#pragma once



class LHUI_Base
{
public:
	bool m_visible;
	bool m_disable;

	int m_x;
	int m_y;
	int m_w;
	int m_h;

	RECT m_rcThis;


public:
	bool PtInUI(POINT& pt)
	{
		return PtInRect(&m_rcThis,pt)?true:false;
	}
	bool PtInUI(int x, int y)
	{
		POINT pt = {x,y};
		return PtInUI(pt);
	}

	bool _GetVisible()
	{
		return m_visible;
	}
	bool _GetDisalbe()
	{
		return m_disable;
	}

	void _SetVisible(bool visible)
	{
		m_visible = visible;
	}
	void _SetDisable(bool disable)
	{
		m_disable = disable;
	}
	void _SetPos(int x, int y)
	{
		m_x = x;
		m_y = y;
		_UpdateTransform();
	}
	void _SetMove(int w, int h)
	{
		m_w = w;
		m_h = h;
		_UpdateTransform();
	}
	void _UpdateTransform()
	{
		SetRect(&m_rcThis, m_x, m_y , m_x+m_w , m_y+m_h);
	}


public:
	virtual void Init() = 0;
	virtual void LoadRes() = 0;
	virtual void DeleteRes() = 0;
	virtual void Draw() = 0;

	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) = 0;

public:
	LHUI_Base(void);
	virtual ~LHUI_Base(void);
};
#endif // _LHUI_Base_H_