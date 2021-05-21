#ifndef _FX_REFLECTION_H_
#define _FX_REFLECTION_H_

#include "Config.h"

class CFxReflection
{
	CRectPlane*				m_rect_Shiled; //���÷��� ��ȣ��
	CCylinderPlane*			m_cyli_Shiled; //���÷��� ������3����
	float					m_fScale;
	float					m_fFade;

	float					fMinScale; //��ȣ���� �ּ� ũ��
	float					fMaxScale;//��ȣ���� �ִ� ũ��
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
