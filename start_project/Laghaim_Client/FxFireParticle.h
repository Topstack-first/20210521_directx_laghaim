#ifndef _FX_FIRE_PARTICLE_H_
#define _FX_FIRE_PARTICLE_H_

#include "Config.h"

class CFxFireParticle
{
	CRectPlane*				fire1;
	CRectPlane*				fire2;
	CRectPlane*				fire3;
	CRectPlane*				fire4;
	CRectPlane*				fire5;
	CRectPlane*				fire6;
	CRectPlane*				fire7;
	CRectPlane*				fire8;
	CRectPlane*				fire9;
	CRectPlane*				fire10;

	// nate 2004 - 8 ImageManager
	/*	DWORD					m_fireTexId1[6];
		DWORD					m_fireTexId2[7];
		DWORD					m_fireTexId3[8];
		DWORD					m_fireTexId4[10];
		DWORD					m_fireTexId5[8];
		DWORD					m_fireTexId6[10];

		DWORD					m_bfireTexId1[6];
		DWORD					m_bfireTexId2[7];
		DWORD					m_bfireTexId3[8];
		DWORD					m_bfireTexId4[10];
		DWORD					m_bfireTexId5[8];
		DWORD					m_bfireTexId6[10];

		int						m_numTFire1;
		int						m_numTFire2;
		int						m_numTFire3;
		int						m_numTFire4;
		int						m_numTFire5;
		int						m_numTFire6;

		int						m_texFrame;
	*/

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxFireParticle();
	virtual ~CFxFireParticle();

	void Create();
	void Update_Fire_Particle(EffectSort &effect_sort);

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_FIRE_PARTICLE_H_
