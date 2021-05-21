#ifndef __FX_MURAMASA_GHOST__
#define __FX_MURAMASA_GHOST__

class CFxMuramasaGhost ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
{
public:
	CFxMuramasaGhost();
	~CFxMuramasaGhost();

	void DeleteRes();
	void LoadRes();

	void FrameMove( EffectSort &effect_sort, int nAddFrame ); // effect_sort는 각 페어리 셋트마다 생성된다. 이 effect_sort를 갱신해가면서 출력해주자.
	BOOL Render( EffectSort &effect_sort );

	CRectPlane* m_pMainObject;    // 빌보드로 쓸 수도 있는 클래스.
	CRectPlane* m_pEffect1;
	CLolos    * m_pLolo1;
};


#endif