#ifndef _FX_FLAMESPEAR_H_
#define _FX_FLAMESPEAR_H_

#include "Config.h"

class CFxFireParticle; // ������Ʈ� ���̰� �ִ� �Ҳ� ����Ʈ.

class CFxFlameSpear // ī�̸��� 300���� ��ų ����Ʈ
{
public:
	CFxFlameSpear();
	~CFxFlameSpear();

	void DeleteRes();
	void LoadRes();

	BOOL Render( EffectSort &effect_sort ); // �ְ�� ����� ��ο�.(����� 2D�� ����ӿ� ����. �����ú��� 3D�� �ƴ϶� 2D��ǥ��.)

	void InsertFire( EffectSort &effect_sort, D3DVECTOR vDrawVec ); // �Ҳ� ����Ʈ�ִ°� �� �� �Լ��� �и�.
	void InsertFire2( EffectSort &effect_sort, D3DVECTOR vDrawVec ); // �Ҳ� ����Ʈ�ִ°� �� �� �Լ��� �и�.

	CRectPlane* m_pMagicCircle1;    // �������� ������
	CRectPlane* m_pMagicCircle2;    // �������� ������
	CLolos    * m_pSpear;           // â ��ü.
	CLolos    * m_pSpearTail;       // â ���󰥶� ����Ʈ
//    CRectPlane* m_pSpearAura;       // â ��ü�� ���δ� ���� ǥ���ϱ� ���� �༮.
	CCylinderPlane *m_pRollingEff;  // ���ư��� â ������ �ְ��� �Ҳ�.
};

#endif // _FX_FLAMESPEAR_H_