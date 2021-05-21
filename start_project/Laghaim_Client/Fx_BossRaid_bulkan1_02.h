#ifndef _FX_BOSSRAID_BULKAN1_02
#define _FX_BOSSRAID_BULKAN1_02

#include "Config.h"

#define FX_BOSSRAID_BULKAN1_02_MAX_COUNT	3
#define FX_BOSSRAID_BULKAN1_02_MAX_PARTICLE 40

class CLolos;
class CRectPlane;

class CFxBossRaidBulkan1_02
{
protected:

	CLolos*	m_pLolo;	// ������ ���ư��� �������� ����Ʈ
	int		m_LoloTextrIndex[FX_BOSSRAID_BULKAN1_02_MAX_COUNT-1];
	//CLolos*		m_pLolo2;	// nate 2005-06-16 : Burning Crash - ���� Ƣ�� �ѷ�
	D3DVECTOR	m_vCurPos;
	float		m_fScale;
	CRectPlane*	m_Smoke;

public:
	CFxBossRaidBulkan1_02();
	virtual ~CFxBossRaidBulkan1_02();

	void	LoadRes();
	void	DeleteRes();

	void	FrameMove( EffectSort &ef_sort, int nAddFrame );
	void	LoloFrameMove( EffectSort &ef_sort, int nAddFrame);
	void	ParticleFRameMove( EffectSort &ef_sort, int nAddFrame );

	void	Render(EffectSort &ef_sort);
	void	SmokeRender(EffectSort &ef_sort);
	void	EarthCrackRender(EffectSort &ef_sort);
};


#endif // _FX_BOSSRAID_BULKAN1_02


