#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include "Land.h"
#include "Frustum.h"
#include "QuadTreeNode.h"
#include "IndexedTexture.h"
#include "Nk3DMipMap.h"

extern int		gMapWidth;					// 맵의 x축 넓이
extern int		gMapHeight;
extern int		gHFWidth;
extern int		gHFHeight;
extern int		gMapExtention;					// 스케일 팩터 - 정수형
extern float	gfMapExtention;				// 스케일 팩터 - 실수형
extern int		gMapYExtention;
extern float	gfMapYExtention;
extern int		gHeightScale;

extern BOOL		g_bMipmap;
extern BOOL		g_bDetailMipmap;
extern BOOL		g_bDrawDetailGround;

extern int		g_nCurWorldIdx;				// Current World Index


extern DWORD	RGB_BITMASK_A;
extern DWORD	RGB_BITMASK_R;
extern DWORD	RGB_BITMASK_G;
extern DWORD	RGB_BITMASK_B;

CLand *CQuadTreeNode::m_pLand;							// 전역 정보를 유지하기 위해서.
D3DLVERTEX CQuadTreeNode::BaseTri[4];					// 텍스쳐 좌표를 위한 기본 삼각형.
float CQuadTreeNode::m_center_x;
float CQuadTreeNode::m_center_z;
LPDIRECT3DDEVICE7 CQuadTreeNode::m_pd3dDevice;

CQuadTreeNode::CQuadTreeNode()
{
	m_pLeftTop = m_pLeftBottom = m_pRightTop = m_pRightBottom = NULL;
	m_pd3dDevice = NULL;
}

CQuadTreeNode::~CQuadTreeNode()
{
	if (m_pLeftTop != NULL)  		// quad tree니까 하나의 리프만 확인하면 된다.
	{
		delete m_pLeftTop;
		delete m_pLeftBottom;
		delete m_pRightTop;
		delete m_pRightBottom;
	}
}

void CQuadTreeNode::MakeInitStruct(int nLevel, int nDepth)
{
	this->m_nLevel = nLevel;

	if (nLevel >= nDepth)
		return;

	m_pLeftTop = new CQuadTreeNode;
	m_pLeftBottom = new CQuadTreeNode;
	m_pRightTop = new CQuadTreeNode;
	m_pRightBottom = new CQuadTreeNode;

	m_pLeftTop->MakeInitStruct(m_nLevel+1, nDepth);
	m_pLeftBottom->MakeInitStruct(m_nLevel+1, nDepth);
	m_pRightTop->MakeInitStruct(m_nLevel+1, nDepth);
	m_pRightBottom->MakeInitStruct(m_nLevel+1, nDepth);
}

void CQuadTreeNode::Init(int LeftX, int LeftZ, int RightX, int RightZ, int nLevel)
{
	int w, h;
	unsigned int minY, maxY, temp;
	float fminY, fmaxY;
	int CenterX, CenterZ;

	// 최하위 노드 부터 바운딩 박스를 만들어 나간다. upward
	if (nLevel >= LAND_VISIBLE_DEPTH)
	{
		minY = maxY = m_pLand->m_pHeightMap[LeftZ*gHFWidth+LeftX] * gHeightScale;
		for (h=LeftZ; h <= RightZ; h++)
		{
			if (h >= gMapHeight)
				continue;
			for (w = LeftX; w <= RightX; w++)
			{
				if ( w >= gMapWidth)
					continue;
				temp = m_pLand->m_pHeightMap[h*gHFWidth+w] * gHeightScale;
				if (minY > temp)
					minY = temp;
				if (maxY < temp)
					maxY = temp;
			}
		}

		//바운딩 박스의 기록
		fmin[0] = (float) (LeftX * gfMapExtention);
		fmin[1] = (float) (minY);
		fmin[2] = (float) (LeftZ * gfMapExtention);

		fmax[0] = (float) (RightX * gfMapExtention);
		fmax[1] = (float) (maxY);
		fmax[2] = (float) (RightZ * gfMapExtention);

		return;
	}


	// 재귀적으로 분할해 나간다.
	// 좌표계는 x, z, 증가
	// LeftX < RightX, LeftZ < RightZ
	CenterX = (LeftX + RightX) >> 1;
	CenterZ = (LeftZ + RightZ) >> 1;
	nLevel++;

	m_pLeftTop->Init(LeftX, CenterZ, CenterX, RightZ, nLevel);
	m_pLeftBottom->Init(LeftX, LeftZ, CenterX, CenterZ, nLevel);
	m_pRightTop->Init(CenterX, CenterZ, RightX, RightZ, nLevel);
	m_pRightBottom->Init(CenterX, LeftZ, RightX, CenterZ, nLevel);

	// 상위 단계의 바운딩 박스 정보 넣기
	fminY = m_pLeftTop->fmin[1];
	fmaxY = m_pLeftTop->fmax[1];

	// 현재 수준의 바운딩 박스 결정
	if (fminY > m_pLeftBottom->fmin[1]) fminY = m_pLeftBottom->fmin[1];
	if (fminY > m_pRightTop->fmin[1]) fminY = m_pRightTop->fmin[1];
	if (fminY > m_pRightBottom->fmin[1]) fminY = m_pRightBottom->fmin[1];

	if (fmaxY < m_pLeftBottom->fmax[1]) fmaxY = m_pLeftBottom->fmax[1];
	if (fmaxY < m_pRightTop->fmax[1]) fmaxY = m_pRightTop->fmax[1];
	if (fmaxY < m_pRightBottom->fmax[1]) fmaxY = m_pRightBottom->fmax[1];

	fmin[0] = (float) (LeftX * gMapExtention);
	fmin[1] = fminY;
	fmin[2] = (float) (LeftZ * gMapExtention);

	fmax[0] = (float) (RightX * gMapExtention);
	fmax[1] = fmaxY;
	fmax[2] = (float) (RightZ * gMapExtention);
}

void CQuadTreeNode::Culling(CFrustum *pFrustum)
{
	static D3DVECTOR vecArray[8];
	
	// 바운딩 박스의 아랫쪽 네 점
	vecArray[0] = D3DVECTOR(fmin[0], fmin[1], fmin[2]);
	vecArray[1] = D3DVECTOR(fmin[0], fmin[1], fmax[2]);
	vecArray[2] = D3DVECTOR(fmax[0], fmin[1], fmax[2]);
	vecArray[3] = D3DVECTOR(fmax[0], fmin[1], fmin[2]);
	// 바운딩 박스의 윗쪽 네점
	vecArray[4] = D3DVECTOR(fmin[0], fmax[1], fmin[2]);
	vecArray[5] = D3DVECTOR(fmin[0], fmax[1], fmax[2]);
	vecArray[6] = D3DVECTOR(fmax[0], fmax[1], fmax[2]);
	vecArray[7] = D3DVECTOR(fmax[0], fmax[1], fmin[2]);

	int nState = pFrustum->FrustumCullLand(vecArray, 8);

	if (nState == VFCULL_OUTSIDE)
	{
		m_bVisibleState = VFCULL_OUTSIDE;
		return;
	}
	else if (nState == VFCULL_INSIDE)
	{
		PropagateInsideFlag(this);
		return;
	}
	else
	{
		m_bVisibleState = VFCULL_OVERLAP;
		if (m_pLeftTop != NULL)
		{
			m_pLeftTop->Culling(pFrustum);
			m_pLeftBottom->Culling(pFrustum);
			m_pRightTop->Culling(pFrustum);
			m_pRightBottom->Culling(pFrustum);
		}
	}

	return;
}

void CQuadTreeNode::PropagateInsideFlag(CQuadTreeNode *pQuadTreeNode)
{
	if (pQuadTreeNode == NULL)
		return;

	pQuadTreeNode->m_bVisibleState = VFCULL_INSIDE;
	PropagateInsideFlag(pQuadTreeNode->m_pLeftTop);
	PropagateInsideFlag(pQuadTreeNode->m_pLeftBottom);
	PropagateInsideFlag(pQuadTreeNode->m_pRightTop);
	PropagateInsideFlag(pQuadTreeNode->m_pRightBottom);
}

#define SKY_LAND_HEIGHT 49

void CQuadTreeNode::Render()
{
	static DWORD dwDayLight;
	LPDIRECTDRAWSURFACE7 lpDDsurface, lpSurfaceDetail;

	if (m_bVisibleState == VFCULL_OUTSIDE)
		return;

	if (m_pLeftTop == NULL)
	{
		int w, h, sw, ew, sh, eh, idx;
		float fH_mul_gMap, fW_mul_gfMap;
		int h_mul_gHFWidth, h1_mul_gHFWidth;


		ew = (int) (fmax[0]/gfMapExtention);
		sw = (int) (fmin[0] / gfMapExtention) ;
		eh = (int) (fmax[2]/ gfMapExtention);
		sh = (int) (fmin[2]/ gfMapExtention) ;

		dwDayLight = m_pLand->GetDayLightColor();

		// 이부분에서 최적화하는 방법
		// 그려야할 것들의 리스트를 만들어서 나중에 그린다.
		for (h = sh; h < eh; h++)
		{
			float fRenderDist, fFarWaterDist;

			fRenderDist = (LAND_VISIBLE_WIDTH / 2 ) * gfMapExtention;
			fFarWaterDist = 5 * gfMapExtention;

			fH_mul_gMap = (float) (h * gMapExtention);
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

				float dist;
				float farWaterFactor;

				farWaterFactor = 0.0f;

				BaseTri[0].color = CalColorFactor(m_pLand->m_pdwColor[h1_mul_gHFWidth+w], m_pLand->m_fLightFactor);
				dist = (float)sqrt((m_center_x-BaseTri[0].x)*(m_center_x-BaseTri[0].x)+
								   (m_center_z-BaseTri[0].z)*(m_center_z-BaseTri[0].z));

				if (dist > fRenderDist)
					continue;

				if (dist > fFarWaterDist)
				{
					farWaterFactor = (dist - fFarWaterDist) / (fRenderDist - fFarWaterDist) * 2.0f;
				}

				if (m_pLand->m_bHourLight)
				{
					dist = (m_pLand->m_fLandDistRange-dist) / m_pLand->m_fLandDistRange;
					if (dist > 0.0f)
					{
						dist *= m_pLand->m_fLandSpotFactor;
						BaseTri[0].color = AddColor(BaseTri[0].color, D3DRGBA(dist, dist, dist, 0));
					}
				}

				BaseTri[1].color = CalColorFactor(m_pLand->m_pdwColor[h1_mul_gHFWidth+w+1], m_pLand->m_fLightFactor);
				dist = (float)sqrt((m_center_x-BaseTri[1].x)*(m_center_x-BaseTri[1].x)+
								   (m_center_z-BaseTri[1].z)*(m_center_z-BaseTri[1].z));

				if (dist > fRenderDist)
					continue;
				if (dist > fFarWaterDist)
				{
					farWaterFactor = (dist - fFarWaterDist) / (fRenderDist - fFarWaterDist) * 2.0f;
				}
				if (m_pLand->m_bHourLight)
				{


					dist = (m_pLand->m_fLandDistRange-dist) / m_pLand->m_fLandDistRange;
					if (dist > 0.0f)
					{
						dist *= m_pLand->m_fLandSpotFactor;
						BaseTri[1].color = AddColor(BaseTri[1].color, D3DRGBA(dist, dist, dist, 0));
					}
				}

				BaseTri[2].color = CalColorFactor(m_pLand->m_pdwColor[h_mul_gHFWidth+w], m_pLand->m_fLightFactor);
				dist = (float)sqrt((m_center_x-BaseTri[2].x)*(m_center_x-BaseTri[2].x)+
								   (m_center_z-BaseTri[2].z)*(m_center_z-BaseTri[2].z));

				if (dist > fRenderDist)
					continue;
				if (dist > fFarWaterDist)
				{
					farWaterFactor = (dist - fFarWaterDist) / (fRenderDist - fFarWaterDist) * 2.0f;
				}
				if (m_pLand->m_bHourLight)
				{
					dist = (m_pLand->m_fLandDistRange-dist) / m_pLand->m_fLandDistRange;
					if (dist > 0.0f)
					{
						dist *= m_pLand->m_fLandSpotFactor;
						BaseTri[2].color = AddColor(BaseTri[2].color, D3DRGBA(dist, dist, dist, 0));
					}
				}

				BaseTri[3].color = CalColorFactor(m_pLand->m_pdwColor[h_mul_gHFWidth+w+1], m_pLand->m_fLightFactor);
				dist = (float)sqrt((m_center_x-BaseTri[3].x)*(m_center_x-BaseTri[3].x)+
								   (m_center_z-BaseTri[3].z)*(m_center_z-BaseTri[3].z));
				if (dist > fRenderDist)
					continue;
				if (dist > fFarWaterDist)
				{
					farWaterFactor = (dist - fFarWaterDist) / (fRenderDist - fFarWaterDist) * 2.0f;
				}
				if (m_pLand->m_bHourLight)
				{
					dist = (m_pLand->m_fLandDistRange-dist) / m_pLand->m_fLandDistRange;
					if (dist > 0.0f)
					{
						dist *= m_pLand->m_fLandSpotFactor;
						BaseTri[3].color = AddColor(BaseTri[3].color, D3DRGBA(dist, dist, dist, 0));
					}
				}			

				idx = h*gMapWidth+w;
				m_pLand->AddVisibleList(idx, farWaterFactor);

				WORD tidx = m_pLand->m_pMapIdx[idx];
				lpDDsurface = m_pLand->m_pTileTextures->GetSurfaceByIdx(tidx);

				BOOL bAdjustTexCoord = FALSE;
				if (g_bDetailMipmap)
				{					
					if (g_nCurWorldIdx == WORLD_GUILD)
					{
						bAdjustTexCoord = TRUE;
						lpSurfaceDetail = m_pLand->m_pNk3DMipMap->GetSurface();
					}
					else
					{
						lpSurfaceDetail = m_pLand->m_pNk3DMipMap->GetSurface();
					}
				}
				else
				{
					// 플레이어와의 거리를 측정하여 어떤 해상도의 상세맵을 사용할 것인가를 결정한다.
					int hero_h, hero_w, dist;
					hero_w = (int) (m_center_x/gfMapExtention);
					hero_h = (int) (m_center_z/gfMapExtention);

					dist = (hero_w - w)*(hero_w - w) + (hero_h-h)*(hero_h-h);
					dist = (int)sqrt((float)dist);
					if(dist <= 3)
						lpSurfaceDetail = m_pLand->m_pIndexedTexture->GetSurfaceByIdx(m_pLand->m_nDetailIdx);
					else
						lpSurfaceDetail = m_pLand->m_pIndexedTexture->GetSurfaceByIdx(m_pLand->m_nDetailIdxLow);
				}

				if (bAdjustTexCoord)
				{
					BaseTri[0].tu = 0.01f;
					BaseTri[0].tv = 0.01f;
					BaseTri[1].tu = 0.99f;
					BaseTri[1].tv = 0.01f;
					BaseTri[2].tu = 0.01f;
					BaseTri[2].tv = 0.99f;
					BaseTri[3].tu = 0.99f;
					BaseTri[3].tv = 0.99f;
				}
				else
				{
					BaseTri[0].tu = 0.05f;
					BaseTri[0].tv = 0.05f;
					BaseTri[1].tu = 0.95f;
					BaseTri[1].tv = 0.05f;
					BaseTri[2].tu = 0.05f;
					BaseTri[2].tv = 0.95f;
					BaseTri[3].tu = 0.95f;
					BaseTri[3].tv = 0.95f;
				}

				// 멀티텍스처 방법.
				if (m_pLand->CanDoModulate2X())
				{
					m_pd3dDevice->SetTexture(0, lpDDsurface);
					if (g_bDrawDetailGround)
					{
						m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
						m_pd3dDevice->SetTexture(1, lpSurfaceDetail);
						m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE2X );
					}
					else
					{
						m_pd3dDevice->SetTextureStageState (1, D3DTSS_COLOROP, D3DTOP_DISABLE );
					}
					// Draw
					if (m_pLand->m_pTileDrawMap)
					{
						if (m_pLand->m_pTileDrawMap[h*gMapWidth+w] < 128)
							m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
					}
					else
					{
						m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
					}


				}
				else
				{
					// two pass, blended
					m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
					m_pd3dDevice->SetTexture(0, lpDDsurface);
					if (m_pLand->m_pTileDrawMap && m_pLand->m_pTileDrawMap[h*gMapWidth+w] < 128)
					{
						m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
					}
					else
					{
						m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
					}

					if (g_bDrawDetailGround)
					{
						m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
						m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR );
						m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_DESTCOLOR );
						m_pd3dDevice->SetTexture( 0, lpSurfaceDetail);
						if (m_pLand->m_pTileDrawMap)
						{
							if (m_pLand->m_pTileDrawMap[h*gMapWidth+w] < 128)
								m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
						}
						else
						{
							m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
						}

						m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
					}
				}
				m_pd3dDevice->SetTextureStageState (1, D3DTSS_COLOROP, D3DTOP_DISABLE );
			}
		}
	}
	else
	{
		m_pLeftTop->Render();
		m_pLeftBottom->Render();
		m_pRightTop->Render();
		m_pRightBottom->Render();
	}
}

DWORD CQuadTreeNode::CalColorFactor(DWORD dwColor, float fColorFactor)
{
	DWORD dwR, dwG, dwB;
	DWORD dwResColor;

	dwR = (dwColor & RGB_BITMASK_R) >> 16;
	dwG = (dwColor & RGB_BITMASK_G) >> 8;
	dwB = (dwColor & RGB_BITMASK_B);

	dwR = (DWORD) (dwR * fColorFactor);
	dwG = (DWORD) (dwG * fColorFactor);
	dwB = (DWORD) (dwB * fColorFactor);

	dwResColor = 0;
	dwResColor |= dwR;
	dwResColor = dwResColor << 8;
	dwResColor |= dwG;
	dwResColor = dwResColor << 8;
	dwResColor |= dwB;

	return dwResColor;
}
