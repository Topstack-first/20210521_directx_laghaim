#ifndef _FX_REFLECTION_H_
#define _FX_REFLECTION_H_

#include "Config.h"

class CFxReflection
{
	CRectPlane*				m_rect_Shiled; //리플렉션 보호막
	CCylinderPlane*			m_cyli_Shiled; //리플렉션 지렁이3마리
	float					m_fScale;
	float					m_fFade;

	float					fMinScale; //보호막의 최소 크기
	float					fMaxScale;//보호막의 최대 크기
	float					fFlag;

	int						nSpeed;
	int						m_nTotalFrame;

public:
	CFxReflection();
	virtual ~CFxReflection();

	void Create();

	void LoadRes(IndexedTexture *pIndexedTexture);
	void DeleteRes();

	BOOL Render(EffectSort &effect_sort);

};

#endif // _FX_REFLECTION_H_
