#ifndef _FX_POISON_FOG_H_
#define _FX_POISON_FOG_H_

class CFxPoisonFog // 데카렌 보스방 등에서 쓰이는 독안개 이펙트. 화면을 전체 덥는 빌보드 형 이펙트이다. ///인던1
{
public:
	CFxPoisonFog();
	~CFxPoisonFog();

	BOOL Render( EffectSort &effect_sort ); // 왜곡된 평면을 드로우.(평면이 2D용 평면임에 유의. 만들당시부터 3D가 아니라 2D좌표다.)
};

#endif // _FX_POISON_FOG_H_