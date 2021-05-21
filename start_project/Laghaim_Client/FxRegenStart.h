#ifndef _FX_REGEN_START_H_
#define _FX_REGEN_START_H_

#include "Config.h"

class CFxRegenStart
{
	LPD3DTLVERTEX		m_rectRegen;
	float				m_CurFrame;
public:
	CFxRegenStart();
	virtual ~CFxRegenStart();
	void LoadRes();
	void DeleteRes();
	void Render();
};

#endif // _FX_REGEN_START_H_
