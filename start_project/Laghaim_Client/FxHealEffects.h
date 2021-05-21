#ifndef _FX_HEAL_EFFECTS_H_
#define _FX_HEAL_EFFECTS_H_

#include "Config.h"

#define MAX_HEAL_PARTICLE 10

typedef struct Heal_Particle
{
	int				start_time;
	int 			life_time;
	int 			cur_time;
	float			scale;
	float			fade;

	float			red;
	float			green;
	float			blue;

	D3DVECTOR		cur_Pos;
	D3DVECTOR		direction;
//	D3DVECTOR		inverse_direction;
	D3DVECTOR		gravity;

} Heal_Particle;


class CFxHealEffects
{
	CCylinderPlane*			guard;
	CCylinderPlane*			guard2;

	CRectPlane*				light;

	CSphere*				explosion;

//	Heal_Particle*			heal_particle;
//	CRectPlane*				heal;

	CRectPlane*				heal_round1;
	CRectPlane*				heal_round2;
	CRectPlane*				heal_round3;

	// nate 2004 - 8 ImageManager
//	DWORD					m_guardTexId;
//	DWORD					m_lightTexId;
//	DWORD					m_healroundTexId;

//	DWORD					m_explosionTexId;

//	DWORD					m_healTexId;


	float					m_fScale;
	float					m_fFade;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;
	int						m_nTotalFrame;

public:
	CFxHealEffects();
	virtual ~CFxHealEffects();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

//	void Update_Heal_Particle(EffectSort &effect_sort);

	BOOL Render(EffectSort &effect_sort);
};

#endif // _FX_HEAL_EFFECTS_H_
