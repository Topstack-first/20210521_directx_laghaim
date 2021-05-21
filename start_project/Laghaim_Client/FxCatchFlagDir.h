#ifndef _FX_CATCH_FLAG_DIR_H_
#define _FX_CATCH_FLAG_DIR_H_

#include "Config.h"

class CFxCatchFlagDir
{
	CRectPlane*				catch_flag;

public:
	CFxCatchFlagDir();
	virtual ~CFxCatchFlagDir();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_CATCH_FLAG_DIR_H_

