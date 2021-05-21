#ifndef _LHFx_Perom_DSystem_H_
#define _LHFx_Perom_DSystem_H_

#pragma once

class LHFx_Perom_DSystem
{
private:
	CLolos * m_lolo;

public:
	void LoadRes();
	void DeleteRes();
	bool Render(EffectSort &effect_sort);

public:
	LHFx_Perom_DSystem(void);
	~LHFx_Perom_DSystem(void);
};
#endif // _LHFx_Perom_DSystem_H_