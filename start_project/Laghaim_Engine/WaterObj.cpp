#include "stdafx.h"
#include <d3d.h>
#include "WaterObj.h"
#include "IndexedTexture.h"
#include "CapSyong.h"
#include "D3DUtil.h"

CWaterObj::CWaterObj(IndexedTexture *pTexture) : CLolosBase(pTexture)
{
	m_wAnimIdx = 0;
	m_dwBaseTime = timeGetTime();
	m_AniRate = 0.2f;
}

CWaterObj::~CWaterObj()
{

}

HRESULT CWaterObj::Render(LPDIRECT3DDEVICE7 lpd3dDevice, float wx, float wy, float wz, DWORD alpha)
{
	D3DUtil_SetTranslateMatrix( m_matTrans, wx, wy, wz);
	lpd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matTrans);

	CLolosBase::Render(lpd3dDevice, m_wAnimIdx, FALSE, FALSE, FALSE, NULL, NULL,
				   FALSE, FALSE, alpha);

	return S_OK;
}

void CWaterObj::AdjustAnimFrame()
{
	DWORD dwCurtime;

	dwCurtime = timeGetTime();

	if ( ((dwCurtime - m_dwBaseTime) * 0.001f) >= m_AniRate )
	{
		m_wAnimIdx++;

		if (m_wAnimIdx >= m_AnimCount)
		{
			m_wAnimIdx = 0;
		}

		m_dwBaseTime = dwCurtime;
	}
}