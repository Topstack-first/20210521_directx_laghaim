#ifndef _FX_REGEN_BAR_H_
#define _FX_REGEN_BAR_H_

#include "Config.h"

#define POINT_DANCE_SPACE 0    // 꼭지점 댄스 스페이스바 표시.

class CFxRegenBar
{
	LPD3DTLVERTEX		m_rectRegenBar;
public:
	CFxRegenBar();
	virtual ~CFxRegenBar();
	void Render(DWORD dwCurtime, int nRegenIndex);
	void RenderAnother( int Type ); // 캐릭터에 귀속되어있는 CFxRegenBar 객체를 이용해서 화면에 그려지는 여러가지 이펙트를 그려준다.(꼭지점 댄스시의 스페이스바 푸쉬 이펙트를 그려준다.)
};

#endif // _FX_REGEN_BAR_H_
