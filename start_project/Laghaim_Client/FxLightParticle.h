#ifndef _FX_LIGHT_PARTICLE_H_
#define _FX_LIGHT_PARTICLE_H_

#include "Config.h"

class CFxLightParticle
{
	LPD3DLVERTEX			m_light;
	// nate 2004 - 8 ImageManager
//	int						max_light_vertexs;
//	CRectPlane*				light;
//	DWORD					m_lightTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxLightParticle();
	virtual ~CFxLightParticle();

	void Create();
	void Update_Light_Particle(EffectSort &effect_sort);

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_LIGHT_PARTICLE_H_
