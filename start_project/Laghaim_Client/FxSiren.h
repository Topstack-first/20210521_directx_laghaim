#ifndef _FX_SIREN_
#define _FX_SIREN_

#include "Config.h"

#pragma  once

class CLolos;
class CRectPlane;

class CFxSiren
{
	CLolos* m_pLolo;						// object ÇÏ³ª.
	DWORD   m_dwStartime;

	int m_curframe;
	int	m_effectAniframe;
	int m_effectframe;

	float m_fSirenInterval;
	float m_fEffectInterval;

	CRectPlane* m_pEffectLight;

	int	m_color;
	int	m_TextrIndexSiren;

public:
	CFxSiren();
	virtual ~CFxSiren();

	void		Create(int effect_frame, int nRace, int color = 0);
	void		LoadRes();

	void		DeleteRes();
	BOOL		FrameMove();

	BOOL		Render(EffectSort &effect_sort, CNkCharacter* pChar);
	BOOL		RenderEffect(CNkCharacter* pChar);
};

#endif