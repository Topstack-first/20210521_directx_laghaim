#ifndef _FIRE_FLY_H_
#define _FIRE_FLY_H_

#include "Config.h"

#include <d3d.h>

#define FF_XZ_MAX	50
#define FF_H_MIN	10
#define FF_H_MAX	30
#define FF_WIDTH	(0.5f)
#define FF_SPEED	(0.5f)
#define FF_LIFE		50	// 반딧불이 살아남는 프레임수..

#define FF_RATE		(0.1f)
class CFfObj
{
public:
	CFfObj(float x, float z, float h, float r);
	void Move();

	D3DVECTOR m_FfCenter;
	float m_dx;
	float m_dz;
	//float m_Dir;
	int m_Frame;
	float m_Width;
	DWORD m_Color;

	CFfObj *m_Next;
};

class CFireFly
{
public:
	CFireFly();
	virtual ~CFireFly();

	void SetDevice() {};

	void AddFf();
	CFfObj* DelFf(CFfObj *pPrevFfObj, CFfObj *pFfObj);
	void RemoveAllFf();
	void Render();

	D3DMATRIX m_matWorld, m_matView, m_matId;

	CFfObj *m_FfList;
	int m_LastTime;
};

#endif // _FIRE_FLY_H_
