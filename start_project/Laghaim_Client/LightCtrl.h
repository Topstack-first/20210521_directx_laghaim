#ifndef _LIGHT_CTRL_H_
#define _LIGHT_CTRL_H_

#include "Config.h"

#define NKLIGHT_DIRECTIONAL 0
#define NKLIGHT_POINT		1
#define NKLIGHT_NUM			4

// ���ӳ� �ǽð� ����Ʈ�� �����ϱ� ���� ���� Ŭ����
// Ư���� ����� ����.

// ������ directional light�� �Ѹ���
// �㿡�� point light�� �Ѹ���.

class CLightCtrl
{
public:
	CLightCtrl();
	virtual ~CLightCtrl();

	D3DLIGHT7 m_d3dLight[NKLIGHT_NUM];
	DWORD	m_dwAmbiColor;

	void	InitLight(int light_num, D3DLIGHTTYPE ltType, FLOAT x=0.0f, FLOAT y=0.0f, FLOAT z=0.0f );
	void	InitLight(int light_num, D3DLIGHTTYPE ltType, FLOAT x=0.0f, FLOAT y=0.0f, FLOAT z=0.0f,
					  FLOAT r=1.0f, FLOAT g=1.0f, FLOAT b=1.0f );
	void	LightingEnalbe(BOOL bEnable);
	void	LightEnable(int light_num, BOOL bEnable);
	void	LightAllEnalbe(BOOL bEnable);
	void	SetLight(int light_num);
	BOOL	IsEnabled(int light_num);

	void	SetAmbientLight(DWORD dwColor);
	void	SetCurLandLight();
	void	IncAmbientLight();
	void	DecAmbientLight();
	void	SetDayLight();
	void	SetNightLight();
	void    SetPointLightRange(float range)
	{
		m_fCurRange = range;
	}
	float   GetPointLightRange()
	{
		return m_fCurRange;
	}
	float     m_fCurRange;
};

#endif // _LIGHT_CTRL_H_
