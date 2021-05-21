#ifndef _LHFX_Perom_Meta_H_
#define _LHFX_Perom_Meta_H_

#pragma once

class CLolos;

class LHFX_Perom_Meta
{
private:
	// CRectPlane*	m_plane;
	// CSphere*		m_sphere;

	// D3DLVERTEX	m_lsword[6];
	CLolos * m_lolo;

public:
	void LoadRes();
	void DeleteRes();
	bool Render(EffectSort &effect_sort);


public:
	LHFX_Perom_Meta(void);
	~LHFX_Perom_Meta(void);
};
#endif // _LHFX_Perom_Meta_H_