#ifndef _FX_CATCH_FLAG_H_
#define _FX_CATCH_FLAG_H_

#include "Config.h"

class CFxCatchFlag
{
	CRectPlane*				catch_flag;

	int						m_nTotalFrame;


public:
	CFxCatchFlag();
	virtual ~CFxCatchFlag();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_CATCH_FLAG_H_
