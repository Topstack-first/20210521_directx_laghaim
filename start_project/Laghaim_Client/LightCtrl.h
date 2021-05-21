#ifndef _LIGHT_CTRL_H_
#define _LIGHT_CTRL_H_

#include "Config.h"

#define NKLIGHT_DIRECTIONAL 0
#define NKLIGHT_POINT		1
#define NKLIGHT_NUM			4

// 게임내 실시간 라이트를 조절하기 위한 랩퍼 클래스
// 특별한 기능은 없다.

// 낮에는 directional light를 뿌리고
// 밤에는 point light를 뿌린다.

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
