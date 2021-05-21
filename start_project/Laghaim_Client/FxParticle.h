#ifndef _FX_PARTICLE_H_
#define _FX_PARTICLE_H_

#include "Config.h"

#define MAX_PARTICLE_FOR_HEARTLOAD 15 // 하트 로드 이펙트에서 사용할 파티클 수.
#define MAX_PARTICLE_FOR_LUCKYPIG  20 // 설날 돼지 회복 이펙트에서 사용할 파티클 수.

#define FX_PARTICLE_TYPE_HEARTLORD 0 // 일본 발렌타인데이 아이템 하트로드,러브러브로드 의 이펙트. 하트들이 팔랑 팔랑~ ///하트로드
#define FX_PARTICLE_TYPE_LUCKYPIG 1 // 설날 돼지 회복 이펙트

#define FX_PARTICLE_TYPE_LIP	  2 // 2007.03.14 화이트
#define FX_PARTICLE_TYPE_L		  3
#define FX_PARTICLE_TYPE_O		  4
#define FX_PARTICLE_TYPE_V		  5
#define FX_PARTICLE_TYPE_E		  6


// 새로 추가하는 파티클 타입 이펙트 관리하기 위한 클래스 (07-01-18 생성)
class CFxTypeParticle // 타입에 따른 동작 파티클
{
public:
	CFxTypeParticle();
	~CFxTypeParticle();

	void DeleteRes();
	void LoadRes();

	void FrameMove( EffectSort &effect_sort, int nAddFrame );
	BOOL Render( EffectSort &effect_sort );

	CRectPlane* m_pRectPlane; // 빌보드 출력등을 위한 판때기.
	Deco_Particle *m_pParticleInfoArray; // Tail_Info 클래스를 파티클 정보 클래스로 쓴다.
};

#endif // _FX_PARTICLE_H_