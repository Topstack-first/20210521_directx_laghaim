#ifndef _FX_PET_SKILL_H_
#define _FX_PET_SKILL_H_

#include "Config.h"

#define MAX_FXPETSKILL_PARTICLE 15

#define PET_SKILL_EFFECT_TYPE1_FRAME 7.0f // 광자가 날아가는 프레임.

#define PET_SKILL_EFFECT_TYPE1 0 // 대상에게 조그만 광자가 날라가서 부딪치는 이펙트. 
#define PET_SKILL_EFFECT_TYPE2 1 // 포이즌 이펙트. 라비용이 구형 액체를 발사해서 독구름이 터진다.

// 진화형 펫이 쓰는 스킬 이펙트. 여러정류의 이펙트를 타입을 나눠서 한 클래스에서 처리한다.
class CFxPetSkill
{
public:
	CFxPetSkill();
	~CFxPetSkill();

	void DeleteRes();
	void LoadRes();

	void FrameMove( EffectSort &effect_sort, int nAddFrame );
	BOOL Render( EffectSort &effect_sort );

	CRectPlane *m_pBillBoard; // 빌보드로 쓸 수도 있는 클래스.
	CLolos     *m_pPoison; // 독성 구체가 날라가는 롤로.

	Deco_Particle *m_pParticleInfoArray; // Tail_Info 클래스를 파티클 정보 클래스로 쓴다.(참조포인터임에 유의)
};


#endif // _FX_PET_SKILL_H_