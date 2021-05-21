#ifndef _FX_REGEN_BAR_H_
#define _FX_REGEN_BAR_H_

#include "Config.h"

#define POINT_DANCE_SPACE 0    // ������ �� �����̽��� ǥ��.

class CFxRegenBar
{
	LPD3DTLVERTEX		m_rectRegenBar;
public:
	CFxRegenBar();
	virtual ~CFxRegenBar();
	void Render(DWORD dwCurtime, int nRegenIndex);
	void RenderAnother( int Type ); // ĳ���Ϳ� �ͼӵǾ��ִ� CFxRegenBar ��ü�� �̿��ؼ� ȭ�鿡 �׷����� �������� ����Ʈ�� �׷��ش�.(������ ������ �����̽��� Ǫ�� ����Ʈ�� �׷��ش�.)
};

#endif // _FX_REGEN_BAR_H_
