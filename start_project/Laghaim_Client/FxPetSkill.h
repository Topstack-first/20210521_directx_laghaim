#ifndef _FX_PET_SKILL_H_
#define _FX_PET_SKILL_H_

#include "Config.h"

#define MAX_FXPETSKILL_PARTICLE 15

#define PET_SKILL_EFFECT_TYPE1_FRAME 7.0f // ���ڰ� ���ư��� ������.

#define PET_SKILL_EFFECT_TYPE1 0 // ��󿡰� ���׸� ���ڰ� ���󰡼� �ε�ġ�� ����Ʈ. 
#define PET_SKILL_EFFECT_TYPE2 1 // ������ ����Ʈ. ������ ���� ��ü�� �߻��ؼ� �������� ������.

// ��ȭ�� ���� ���� ��ų ����Ʈ. ���������� ����Ʈ�� Ÿ���� ������ �� Ŭ�������� ó���Ѵ�.
class CFxPetSkill
{
public:
	CFxPetSkill();
	~CFxPetSkill();

	void DeleteRes();
	void LoadRes();

	void FrameMove( EffectSort &effect_sort, int nAddFrame );
	BOOL Render( EffectSort &effect_sort );

	CRectPlane *m_pBillBoard; // ������� �� ���� �ִ� Ŭ����.
	CLolos     *m_pPoison; // ���� ��ü�� ���󰡴� �ѷ�.

	Deco_Particle *m_pParticleInfoArray; // Tail_Info Ŭ������ ��ƼŬ ���� Ŭ������ ����.(�����������ӿ� ����)
};


#endif // _FX_PET_SKILL_H_