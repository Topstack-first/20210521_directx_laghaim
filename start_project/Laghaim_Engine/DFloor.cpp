#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>

#include "Land.h"
#include "DFloor.h"
#include "IndexedTexture.h"

extern int		gMapWidth;					// 맵의 x축 넓이
extern int		gMapHeight;
extern int		gHFWidth;
extern int		gHFHeight;
extern int		gMapExtention;				// 스케일 팩터 - 정수형
extern float	gfMapExtention;				// 스케일 팩터 - 실수형
extern int		gMapYExtention;
extern float	gfMapYExtention;
extern int		gHeightScale;

extern DWORD	RGB_BITMASK_A;
extern DWORD	RGB_BITMASK_R;
extern DWORD	RGB_BITMASK_G;
extern DWORD	RGB_BITMASK_B;

CLand* CDFloor::m_pLand;						// 전역 정보를 유지하기 위해서.
D3DLVERTEX CDFloor::BaseTri[4];					// 텍스쳐 좌표를 위한 기본 삼각형.

extern BOOL g_bDviewBottom;
extern BOOL g_bDviewUp;

CDFloor::CDFloor()
{	
}

CDFloor::~CDFloor()
{	
}

void CDFloor::Init(float x, float z)
{
	m_fCenterX = x;
	m_fCenterZ = z;
}

void CDFloor::Render()
{
	LPDIRECTDRAWSURFACE7 lpDDsurface=NULL, lpSurfaceDetail=NULL;

	int map_center_x, map_center_z;
	int sw, sh, ew, eh, w, h;
	int h_mul_gHFWidth, h1_mul_gHFWidth;
	float fH_mul_gMap, fW_mul_gfMap;
	float dist;

	// 중심점의 맵 좌표를 찾는다.
	map_center_x = (int) (m_fCenterX / gfMapExtention);
	map_center_z = (int) (m_fCenterZ / gfMapExtention);

	sw = map_center_x - MAP_RENDER_RANGE;
	ew = map_center_x + MAP_RENDER_RANGE;

	sh = map_center_z - MAP_RENDER_RANGE;
	eh = map_center_z + MAP_RENDER_RANGE;

	// 렌더링 영역 범위 제한.
	if (sw < 0)
		sw = 0;
	if (ew > gMapWidth)
		ew = gMapWidth;
	if (sh < 0)
		sh = 0;
	if (eh > gMapHeight)
		eh = gMapHeight;

	// 중심점을 기준으로 일정 영역의 사각형을 그린다.
	m_pLand->m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE , FALSE);

	// 기본 바닥 렌더링.
	lpDDsurface = m_pLand->m_pIndexedTexture->GetSurfaceByIdx(m_pLand->m_nDBottomIdx);
	m_pLand->m_pd3dDevice->SetTexture(0, lpDDsurface);

	// 기본 바닥 텍스처 좌표 초기화.
	BaseTri[0].tu = 0.01f;
	BaseTri[0].tv = 0.01f;

	BaseTri[1].tu = 0.99f;
	BaseTri[1].tv = 0.01f;

	BaseTri[2].tu = 0.01f;
	BaseTri[2].tv = 0.99f;

	BaseTri[3].tu = 0.99f;
	BaseTri[3].tv = 0.99f;

	int mesh_count = 0;
	if (g_bDviewBottom != TRUE)
		goto haha;
	
	for (h = sh; h < eh; h++)
	{
		fH_mul_gMap = (float) (h * gfMapExtention);
		BaseTri[0].z = fH_mul_gMap + gMapExtention;
		BaseTri[1].z = fH_mul_gMap + gMapExtention;
		BaseTri[2].z = fH_mul_gMap;
		BaseTri[3].z = fH_mul_gMap;

		for (w = sw; w < ew; w++)
		{
			fW_mul_gfMap = w * gfMapExtention;
			BaseTri[0].x = fW_mul_gfMap;
			BaseTri[1].x = fW_mul_gfMap + gfMapExtention;
			BaseTri[2].x = fW_mul_gfMap;
			BaseTri[3].x = fW_mul_gfMap + gfMapExtention;

			h_mul_gHFWidth = h*gHFWidth;
			h1_mul_gHFWidth = (h+1)*gHFWidth;
			BaseTri[0].y = (float) m_pLand->m_pHeightMap[h1_mul_gHFWidth+w] * gHeightScale;
			BaseTri[1].y = (float) m_pLand->m_pHeightMap[h1_mul_gHFWidth+w+1] * gHeightScale;
			BaseTri[2].y = (float) m_pLand->m_pHeightMap[h_mul_gHFWidth+w] * gHeightScale;
			BaseTri[3].y = (float) m_pLand->m_pHeightMap[h_mul_gHFWidth+w+1] * gHeightScale;

			BaseTri[0].color = m_pLand->m_pdwColor[h1_mul_gHFWidth+w];
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[0].x)*(m_fCenterX-BaseTri[0].x)+
								   (m_fCenterZ-BaseTri[0].z)*(m_fCenterZ-BaseTri[0].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[0].color = AddColor(BaseTri[0].color, D3DRGBA(dist, dist, dist, 0));
				}
			}
			BaseTri[1].color = m_pLand->m_pdwColor[h1_mul_gHFWidth+w+1];
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[1].x)*(m_fCenterX-BaseTri[1].x)+
								   (m_fCenterZ-BaseTri[1].z)*(m_fCenterZ-BaseTri[1].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[1].color = AddColor(BaseTri[1].color, D3DRGBA(dist, dist, dist, 0));
				}
			}
			BaseTri[2].color = m_pLand->m_pdwColor[h_mul_gHFWidth+w];
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[2].x)*(m_fCenterX-BaseTri[2].x)+
								   (m_fCenterZ-BaseTri[2].z)*(m_fCenterZ-BaseTri[2].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[2].color = AddColor(BaseTri[2].color, D3DRGBA(dist, dist, dist, 0));
				}
			}
			BaseTri[3].color = m_pLand->m_pdwColor[h_mul_gHFWidth+w+1];
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[3].x)*(m_fCenterX-BaseTri[3].x)+
								   (m_fCenterZ-BaseTri[3].z)*(m_fCenterZ-BaseTri[3].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[3].color = AddColor(BaseTri[3].color, D3DRGBA(dist, dist, dist, 0));
				}
			}

			m_pLand->m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
		}
	}
haha:

	DWORD dwAlphaOp, dwAlphaArg1, dwAlphaArg2;
	DWORD dwAlpha;

	// 2차 렌더링.
	if (g_bDviewBottom && g_bDviewUp)
	{
		m_pLand->m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE , TRUE);
		m_pLand->m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pLand->m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pLand->m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp );
		m_pLand->m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg1 );
		m_pLand->m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwAlphaArg2 );

		m_pLand->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		m_pLand->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		m_pLand->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	}

	if (g_bDviewUp != TRUE)
		return;

	for (h = sh; h < eh; h++)
	{
		fH_mul_gMap = (float) (h * gfMapExtention);
		BaseTri[0].z = fH_mul_gMap + gMapExtention;
		BaseTri[1].z = fH_mul_gMap + gMapExtention;
		BaseTri[2].z = fH_mul_gMap;
		BaseTri[3].z = fH_mul_gMap;

		for (w = sw; w < ew; w++)
		{
			fW_mul_gfMap = w * gfMapExtention;
			BaseTri[0].x = fW_mul_gfMap;
			BaseTri[1].x = fW_mul_gfMap + gfMapExtention;
			BaseTri[2].x = fW_mul_gfMap;
			BaseTri[3].x = fW_mul_gfMap + gfMapExtention;

			h_mul_gHFWidth = h*gHFWidth;
			h1_mul_gHFWidth = (h+1)*gHFWidth;
			BaseTri[0].y = (float) m_pLand->m_pHeightMap[h1_mul_gHFWidth+w] * gHeightScale;
			BaseTri[1].y = (float) m_pLand->m_pHeightMap[h1_mul_gHFWidth+w+1] * gHeightScale;
			BaseTri[2].y = (float) m_pLand->m_pHeightMap[h_mul_gHFWidth+w] * gHeightScale;
			BaseTri[3].y = (float) m_pLand->m_pHeightMap[h_mul_gHFWidth+w+1] * gHeightScale;

			// 알파값
			dwAlpha = m_pLand->m_puAlpha[h1_mul_gHFWidth+w];
			BaseTri[0].color = m_pLand->m_pdwColor[h1_mul_gHFWidth+w];
			BaseTri[0].color = BaseTri[0].color & 0x00FFFFFF;
			dwAlpha = dwAlpha << 24;
			BaseTri[0].color = BaseTri[0].color | dwAlpha;
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[0].x)*(m_fCenterX-BaseTri[0].x)+
								   (m_fCenterZ-BaseTri[0].z)*(m_fCenterZ-BaseTri[0].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[0].color = AddColor(BaseTri[0].color, D3DRGBA(dist, dist, dist, 0));
				}
			}

			dwAlpha = m_pLand->m_puAlpha[h1_mul_gHFWidth+w+1];
			BaseTri[1].color = m_pLand->m_pdwColor[h1_mul_gHFWidth+w+1];
			BaseTri[1].color = BaseTri[1].color & 0x00FFFFFF;
			dwAlpha = dwAlpha << 24;
			BaseTri[1].color = BaseTri[1].color | dwAlpha;
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[1].x)*(m_fCenterX-BaseTri[1].x)+
								   (m_fCenterZ-BaseTri[1].z)*(m_fCenterZ-BaseTri[1].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[1].color = AddColor(BaseTri[1].color, D3DRGBA(dist, dist, dist, 0));
				}
			}

			dwAlpha = m_pLand->m_puAlpha[h_mul_gHFWidth+w];
			BaseTri[2].color = m_pLand->m_pdwColor[h_mul_gHFWidth+w];
			BaseTri[2].color = BaseTri[2].color & 0x00FFFFFF;
			dwAlpha = dwAlpha << 24;
			BaseTri[2].color = BaseTri[2].color | dwAlpha;
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[2].x)*(m_fCenterX-BaseTri[2].x)+
								   (m_fCenterZ-BaseTri[2].z)*(m_fCenterZ-BaseTri[2].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[2].color = AddColor(BaseTri[2].color, D3DRGBA(dist, dist, dist, 0));
				}
			}

			dwAlpha = m_pLand->m_puAlpha[h_mul_gHFWidth+w+1];
			BaseTri[3].color = m_pLand->m_pdwColor[h_mul_gHFWidth+w+1];
			BaseTri[3].color = BaseTri[3].color & 0x00FFFFFF;
			dwAlpha = dwAlpha << 24;
			BaseTri[3].color = BaseTri[3].color | dwAlpha;
			if (m_pLand->m_bHourLight)
			{
				dist = (float)sqrt((m_fCenterX-BaseTri[3].x)*(m_fCenterX-BaseTri[3].x)+
								   (m_fCenterZ-BaseTri[3].z)*(m_fCenterZ-BaseTri[3].z));
				dist = (m_pLand->m_fDunDistRange-dist) / m_pLand->m_fDunDistRange;
				if (dist > 0.0f)
				{
					dist *= m_pLand->m_fDunSpotFactor;
					BaseTri[3].color = AddColor(BaseTri[3].color, D3DRGBA(dist, dist, dist, 0));
				}
			}

			// 텍스처 좌표.
			if ( w & 0x1 )
			{
				BaseTri[0].tu = 0.5f;
				BaseTri[1].tu = 0.99f;
				BaseTri[2].tu = 0.5f;
				BaseTri[3].tu = 0.99f;
			}
			else
			{
				BaseTri[0].tu = 0.01f;
				BaseTri[1].tu = 0.5f;
				BaseTri[2].tu = 0.01f;
				BaseTri[3].tu = 0.5f;
			}

			if ( h & 0x1 )
			{
				BaseTri[0].tv = 0.01f;
				BaseTri[1].tv = 0.01f;
				BaseTri[2].tv = 0.5f;
				BaseTri[3].tv = 0.5f;
			}
			else
			{
				BaseTri[0].tv = 0.5f;
				BaseTri[1].tv = 0.5f;
				BaseTri[2].tv = 0.99f;
				BaseTri[3].tv = 0.99f;
			}

			int idx = h*gMapWidth + w;
			unsigned char tidx = m_pLand->m_pDungeonIdx[idx];

			m_pLand->AddVisibleList(idx);

			if (tidx == 0)
				tidx = 0;
			else if (tidx > 62 && tidx < 68)
				tidx = 1;
			else if (tidx > 124 && tidx < 132)
				tidx = 2;
			else if (tidx > 190 && tidx < 194 )	// 192
				tidx = 3;
			else if (tidx > 253 )	// 255
				tidx = 4;
			else
				tidx = 0;

			lpDDsurface = m_pLand->m_pTileTextures->GetSurfaceByIdx(tidx);

			m_pLand->m_pd3dDevice->SetTexture(0, lpDDsurface);
			m_pLand->m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
		}
	}

	if (g_bDviewBottom && g_bDviewUp)
	{
		m_pLand->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwAlphaOp);
		m_pLand->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg1 );
		m_pLand->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwAlphaArg2 );
		m_pLand->m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE , FALSE);
	}
}
