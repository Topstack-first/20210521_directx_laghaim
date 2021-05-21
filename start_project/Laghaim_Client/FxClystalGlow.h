#ifndef _FX_CLYSTALGLOW_H_
#define _FX_CLYSTALGLOW_H_
// �������̵� �޸ռ� �߾ӿ� ���� ���� ����Ʈ

#include "Config.h"
class CLolos;

#define CLYSTAL_TYPE_RED	1
#define CLYSTAL_TYPE_WINEB	2
#define CLYSTAL_TYPE_WINES	3
#define CLYSTAL_TYPE_CYON	4

// �޸� ������ �߾� ��� ������Ʈ ����Ʈ
class CFxClystalGlow
{
	CRectPlane*	m_Plane;

	CLolos* m_pClystalRed;
	CLolos* m_pClystalWineB;
	CLolos* m_pClystalWineS;
	CLolos* m_pClystalCyon;

public:
	CFxClystalGlow();
	virtual ~CFxClystalGlow();

	void Create();
	void Update_Particle(EffectSort &ef_sort);

	void LoadRes();
	void DeleteRes();

	BOOL Render(EffectSort &ef_sort);
	BOOL ClystalRender(EffectSort &ef_sort);
};

#endif