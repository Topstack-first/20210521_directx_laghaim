#ifndef _FxLv400Effect_H_
#define _FxLv400Effect_H_

#pragma once

// #include "Config.h"

const int MAX_EFFECT_PLANE = 3;

class CRectPlane;

class CFxLv400Effect
{
protected:
	CRectPlane*	m_Plane;

public:
	void		LoadRes();
	void		DeleteRes();
	void		Create();
	void		FrameMove( EffectSort &eff, int nAddFrame );
	BOOL		Render(EffectSort &eff);

public:
	int			GetSkillTexture(int type, int frame);

public:
	CFxLv400Effect(void);
	virtual ~CFxLv400Effect(void);
};
#endif // _FxLv400Effect_H_