#ifndef _FX_WATERFALL_H_
#define _FX_WATERFALL_H_
// �������̵� �޸ռ� �߾ӿ� ���� ���� ����Ʈ

#include "Config.h"

// �޸� ������ �߾� ��� ������Ʈ ����Ʈ
class CFxWaterfall
{
	CLolos*	m_pLolo;

public:
	CFxWaterfall();
	virtual ~CFxWaterfall();

	void LoadRes();
	void DeleteRes();
	BOOL Render(EffectSort &ef_sort);
};

#endif