#ifndef _FX_MUSICAL_NOTE_H_
#define _FX_MUSICAL_NOTE_H_

#include "Config.h"

class CFxMusicalNote
{
	CRectPlane*				musical_note;

	// nate 2004 - 8 ImageManager
//	DWORD					m_lifeTexId;

	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

public:
	CFxMusicalNote();
	virtual ~CFxMusicalNote();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);
};


#endif // _FX_MUSICAL_NOTE_H_

