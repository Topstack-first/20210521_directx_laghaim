#ifndef _CHANGE_HAIR_H_
#define _CHANGE_HAIR_H_

#include "Config.h"

#include "spinbox.h"
#include "BasicStatic.h"

class CSurface;
class CSpinBox;
class IndexedTexture;
class CBasicButton;
class CTextOutBox;

#define  NUM_HAIR_ICON  10


class CChangeHair
{
public:
	CChangeHair();
	virtual ~CChangeHair();

	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);

public:
	CBasicStatic m_bg;
	CSurface * m_pHairIcon[NUM_HAIR_ICON];
	CSpinBox * m_pHairBox;
	CBasicButton * m_pBtn1;
	CBasicButton * m_pBtn2;
	CTextOutBox		* m_pText;

	BOOL       m_bActive;
	int		   m_iCurIcon;
	int        m_ScreenWidth, m_ScreenHeight;
	int        m_iMainX, m_iMainY, m_iMainCx, m_iMainCy;
};

#endif // _CHANGE_HAIR_H_
