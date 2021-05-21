#ifndef _COMPAS_H_
#define _COMPAS_H_


#include "TextOutBox.h"
#include "BasicStatic.h"

class IndexedTexture;
class CSurface;

#define COMPAS_HALF_WIDTH_BASE	84
#define COMPAS_HALF_HEIGHT_BASE	56
#define COMPAS_HALF_WIDTH_TURN	50
#define COMPAS_HALF_HEIGHT_TURN	50

#define PLAYERPOS_FONTSIZE 12
//#define PLAYERPOS_FONTCOLOR RGB(255, 255, 0)
//#define PLAYERPOS_FONTCOLOR RGB(242, 166, 76)

class CCompas
{
protected:
	int					m_ScreenWidth, m_ScreenHeight;
	int					m_nHalfWidth, m_nHalfHeight;
	float				m_fDir;
	float				m_fMobDir;
	D3DMATRIX			m_matRotateZ;
	float					m_Mag;
	float				m_Scale;
	CSurface			*m_pCross;


	D3DTLVERTEX			m_SrcCompasTurn[4], m_OutCompas[4];
	WORD				m_wIdx[6];
	CTextOutBox	 m_PosBox;
	int m_iCurDeckadum;


	CBasicStatic m_bg;

public:
	int		GetX()
	{
		return m_bg.m_x;
	}
	int		GetY()
	{
		return m_bg.m_y;
	}


public:
	int					m_CurMapPosX;
	int					m_CurMapPosZ;

public:
	CCompas();
	virtual ~CCompas();

	void		LoadRes();

	void		SetDevice() {};
	void		DeleteRes();
	void		CalDirection(D3DVECTOR &vCam, D3DVECTOR &vAt);
	void		Render(int sx, int sy);
	void		Init(int width, int height);
	int			GetHalfWidth()
	{
		return m_nHalfWidth;
	}
	int			GetHalfHeight()
	{
		return m_nHalfHeight;
	}
	void		Scale(float scale);
	BOOL		IsInside(int x, int y);
	void        Show_Pos_Text();

};

#endif // _COMPAS_H_
