#ifndef _WATER_OBJ_H_
#define _WATER_OBJ_H_

#include "CapSyong.h"

class IndexedTexture;
class CWaterObj : public CLolosBase
{
public:
	CWaterObj(IndexedTexture *pTexture);
	~CWaterObj();

	void AdjustAnimFrame();
	HRESULT Render(LPDIRECT3DDEVICE7 lpd3dDevice, float wx, float wy, float wz, DWORD alpha);

private:
	WORD		m_wAnimIdx;
	D3DMATRIX	m_matTrans;
	DWORD		m_dwBaseTime;
	float		m_AniRate;
};

#endif // _WATER_OBJ_H_
