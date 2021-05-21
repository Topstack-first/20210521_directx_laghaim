#ifndef _FX_ASSAULT_H_
#define _FX_ASSAULT_H_

#include "Config.h"

class CLolos;
class CFxAssault
{
public:
	CFxAssault();
	virtual ~CFxAssault();

	D3DLVERTEX		m_flame[4];                 //-- Effect Billboard
	D3DVECTOR			vPosFrom;                   //-- Effect ÀÇ ÁÂÇ¥
	CRectPlane*		m_board;										//-- ºôº¸µå


	bool mb_sound;
	BOOL		Render(EffectSort &effect_sort);

};

#endif // _FX_ASSAULT_H_
