#ifndef _CONTROL_POPUP_MENU_H_
#define _CONTROL_POPUP_MENU_H_

#include <Windows.h>
#include <vector>
#include "basicbutton.h"

class ControlPopupMenu
{
public:
	typedef std::vector<CBasicButton*> VEC_BTNS;

	ControlPopupMenu();
	virtual ~ControlPopupMenu();

public:
	virtual void LoadRes();
	virtual void Draw();
	virtual void DeleteRes();
	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

public:
	void SetVisible(const bool& visible)
	{
		m_bVisible = visible;
	}
	const bool& GetVisible()
	{
		return m_bVisible;
	}

	virtual void SetPosition(const int& posX, const int& posY, const bool& bDown);
	const int& GetPosX()
	{
		return m_iPosX;
	}
	const int& GetPosY()
	{
		return m_iPosY;
	}
	void SetGap(const int& gapX, const int& gapY)
	{
		m_iGapX = gapX;
		m_iGapY = gapY;
	}

	void SetIndex(const int& index)
	{
		m_index = index;
	}

	bool isInside(const int& pointX, const int& pointY);

protected:
	VEC_BTNS m_vecBtns;
	bool m_bVisible;

	int	m_iPosX;
	int m_iPosY;

	int m_iGapX;
	int m_iGapY;

	int m_index;

	CSurface* m_pSurpBack;
};

#endif