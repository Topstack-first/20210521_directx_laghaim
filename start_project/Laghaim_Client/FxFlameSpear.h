#ifndef _FX_FLAMESPEAR_H_
#define _FX_FLAMESPEAR_H_

#include "Config.h"

class CFxFireParticle; // 오브젝트등에 쓰이고 있는 불꽃 이펙트.

class CFxFlameSpear // 카이립톤 300레벨 스킬 이펠트
{
public:
	CFxFlameSpear();
	~CFxFlameSpear();

	void DeleteRes();
	void LoadRes();

	BOOL Render( EffectSort &effect_sort ); // 왜곡된 평면을 드로우.(평면이 2D용 평면임에 유의. 만들당시부터 3D가 아니라 2D좌표다.)

	void InsertFire( EffectSort &effect_sort, D3DVECTOR vDrawVec ); // 불꽃 이펙트넣는게 좀 길어서 함수로 분리.
	void InsertFire2( EffectSort &effect_sort, D3DVECTOR vDrawVec ); // 불꽃 이펙트넣는게 좀 길어서 함수로 분리.

	CRectPlane* m_pMagicCircle1;    // 생성시의 마법진
	CRectPlane* m_pMagicCircle2;    // 생성시의 마법진
	CLolos    * m_pSpear;           // 창 본체.
	CLolos    * m_pSpearTail;       // 창 날라갈때 이펙트
//    CRectPlane* m_pSpearAura;       // 창 본체를 감싸는 빛을 표현하기 위한 녀석.
	CCylinderPlane *m_pRollingEff;  // 날아갈때 창 주위를 휘감는 불꽃.
};

#endif // _FX_FLAMESPEAR_H_