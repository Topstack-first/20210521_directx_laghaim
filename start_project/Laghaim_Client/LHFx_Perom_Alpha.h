#ifndef _LHFx_Perom_Alpha_H_
#define _LHFx_Perom_Alpha_H_

#pragma once

class CLolos;

class LHFx_Perom_Alpha
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
	LHFx_Perom_Alpha(void);
	~LHFx_Perom_Alpha(void);
};
#endif // _LHFx_Perom_Alpha_H_