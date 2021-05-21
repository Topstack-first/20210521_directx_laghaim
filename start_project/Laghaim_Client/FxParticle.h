#ifndef _FX_PARTICLE_H_
#define _FX_PARTICLE_H_

#include "Config.h"

#define MAX_PARTICLE_FOR_HEARTLOAD 15 // ��Ʈ �ε� ����Ʈ���� ����� ��ƼŬ ��.
#define MAX_PARTICLE_FOR_LUCKYPIG  20 // ���� ���� ȸ�� ����Ʈ���� ����� ��ƼŬ ��.

#define FX_PARTICLE_TYPE_HEARTLORD 0 // �Ϻ� �߷�Ÿ�ε��� ������ ��Ʈ�ε�,���귯��ε� �� ����Ʈ. ��Ʈ���� �ȶ� �ȶ�~ ///��Ʈ�ε�
#define FX_PARTICLE_TYPE_LUCKYPIG 1 // ���� ���� ȸ�� ����Ʈ

#define FX_PARTICLE_TYPE_LIP	  2 // 2007.03.14 ȭ��Ʈ
#define FX_PARTICLE_TYPE_L		  3
#define FX_PARTICLE_TYPE_O		  4
#define FX_PARTICLE_TYPE_V		  5
#define FX_PARTICLE_TYPE_E		  6


// ���� �߰��ϴ� ��ƼŬ Ÿ�� ����Ʈ �����ϱ� ���� Ŭ���� (07-01-18 ����)
class CFxTypeParticle // Ÿ�Կ� ���� ���� ��ƼŬ
{
public:
	CFxTypeParticle();
	~CFxTypeParticle();

	void DeleteRes();
	void LoadRes();

	void FrameMove( EffectSort &effect_sort, int nAddFrame );
	BOOL Render( EffectSort &effect_sort );

	CRectPlane* m_pRectPlane; // ������ ��µ��� ���� �Ƕ���.
	Deco_Particle *m_pParticleInfoArray; // Tail_Info Ŭ������ ��ƼŬ ���� Ŭ������ ����.
};

#endif // _FX_PARTICLE_H_