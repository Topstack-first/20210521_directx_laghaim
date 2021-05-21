#ifndef _LHFX_Perom_Field_H_
#define _LHFX_Perom_Field_H_

#pragma once

class CLolos;

enum FxPerom_EffectType
{
	FxPerom_EffectType__S134,
	FxPerom_EffectType__S139,
	FxPerom_EffectType__S140,
	FxPerom_EffectType__S141,
	FxPerom_EffectType__S148,
	FxPerom_EffectType__S149,
	FxPerom_EffectType__S150,

	FxPerom_EffectType__EndOfEnum
};

//
class LHFX_Perom_Field
{
private:
	// CRectPlane*	m_plane;
	// CSphere*		m_sphere;

	// D3DLVERTEX	m_lsword[6];
	CLolos * m_lolo;

	int m_effect_texture[FxPerom_EffectType__EndOfEnum];

public:
	void LoadRes();
	void DeleteRes();
	bool Render(EffectSort &effect_sort);

	int GetTypeToTextureIndex(int type);


public:
	LHFX_Perom_Field(void);
	~LHFX_Perom_Field(void);
};
#endif // _LHFX_Perom_Field_H_