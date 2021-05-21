#ifndef _LHFX_Perom_Blader_H_
#define _LHFX_Perom_Blader_H_

#pragma once

class CLolos;

class LHFX_Perom_Blader
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
	LHFX_Perom_Blader(void);
	~LHFX_Perom_Blader(void);
};
#endif // _LHFX_Perom_Blader_H_