#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "Config.h"

#define MOUSE_KIND_CNT 15 // [6/7/2007 Theodoric] 내구도 수리 

class CDisplay;
class CSurface;

class CMouse
{


	RECT			m_MouseRc;

	int		m_nKind;
	int		m_nCurFrame;
	int		m_FrameCnt;
	DWORD	m_dwDelay;
	DWORD   m_dwBaseTime;
	POINT	m_ptCliSize;

public:
	CMouse();
	virtual ~CMouse();

	CDisplay	*m_pDisplay;
	CSurface	*m_pMouseSurf[MOUSE_KIND_CNT];

	void		SetKind(int nKind)
	{
		m_nKind = nKind;
	};
	void		SetDelay(DWORD dwDelay)
	{
		m_dwDelay = dwDelay;
	};

	void		LoadRes(int client_width, int client_height);
	void		DeleteRes();
	void		Draw(POINT point);
	void	    SetMouseType(int type);
	int 		GetMouseType()
	{
		return m_nKind;
	}

};

#endif // _MOUSE_H_
