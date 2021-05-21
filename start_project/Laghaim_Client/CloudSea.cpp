#include "stdafx.h"
#define D3D_OVERLOADS


#include <d3d.h>

#include "Land.h"
#include "Frustum.h"
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"
#include "CloudSea.h"
#include "IndexedTexture.h"
#include "Nk3DMipMap.h"
#include "main.h"

#define CLOUD_X_NUM		256
#define CLOUD_Z_NUM		256
#define CLOUD_WIDTH		800
#define CLOUD_WIDTH_F	(800.0f)
#define CLOUD_HEIGHT	800
#define CLOUD_HEIGHT_F	(800.0f)
#define CLOUD_POS_Y		(-200.0f)

#define CLOUDSEA_SKY_TIME	50000
#define CLOUDSEA_CLOUD_TIME	10000

extern BOOL		g_bMipmap;
extern BOOL		g_bDetailMipmap;

extern int		g_nCurWorldIdx;				// Current World Index

extern DWORD	g_dwBaseTime;

D3DLVERTEX CCloudSea::BaseTri[4];					// �ؽ��� ��ǥ�� ���� �⺻ �ﰢ��.
float CCloudSea::m_center_x;
float CCloudSea::m_center_z;
BOOL CCloudSea::m_bCanDoModulate2X;
DWORD CCloudSea::m_SkyFrame;
DWORD CCloudSea::m_CloudFrame;
float CCloudSea::m_SNoise[];
float CCloudSea::m_CNoise[];

CCloudSea::CCloudSea()
	: m_bVisibleState(0)
	, m_nLevel(0)
{
	memset(fmin, 0, sizeof(fmin));
	memset(fmax, 0, sizeof(fmax));

	m_pLeftTop = m_pLeftBottom = m_pRightTop = m_pRightBottom = NULL;
	m_bCanDoModulate2X = FALSE;
}

CCloudSea::~CCloudSea()
{
	if (m_pLeftTop != NULL)
	{
		// quad tree�ϱ� �ϳ��� ������ Ȯ���ϸ� �ȴ�.
		delete m_pLeftTop;
		delete m_pLeftBottom;
		delete m_pRightTop;
		delete m_pRightBottom;
	}
}

// MakeInitStruct(int nLevel, int nDepth)
// Desc : ���� Ʈ���� ������ ���� �� ����ü�� �����.
void CCloudSea::MakeInitStruct(int nLevel, int nDepth)
{
	if( this )
		this->m_nLevel = nLevel;

	if (nLevel >= nDepth)
		return;

	m_pLeftTop = new CCloudSea;
	m_pLeftBottom = new CCloudSea;
	m_pRightTop = new CCloudSea;
	m_pRightBottom = new CCloudSea;

	if( m_pLeftTop )
		m_pLeftTop->MakeInitStruct(m_nLevel+1, nDepth);
	if(	m_pLeftBottom )
		m_pLeftBottom->MakeInitStruct(m_nLevel+1, nDepth);
	if( m_pRightTop )
		m_pRightTop->MakeInitStruct(m_nLevel+1, nDepth);
	if( m_pRightBottom )
		m_pRightBottom->MakeInitStruct(m_nLevel+1, nDepth);

	// CLand ���� ������ �κ�...
	BaseTri[0] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,256.0f), 0x00000000, 0, 0.05f, 0.05f);
	BaseTri[1] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,256.0f),0x00000000, 0, 0.95f, 0.05f);
	BaseTri[2] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,0.0f), 0x00000000, 0, 0.05f, 0.95f);
	BaseTri[3] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,0.0f), 0x00000000, 0, 0.95f, 0.95f);
}

void CCloudSea::MakeNoise()
{
	for (int i = 0; i < CLOUDSEA_NOISE_NUM; i++)
	{
		m_SNoise[i] = (rand() % 101) / 101.0f * 0.317f;
		m_CNoise[i] = (rand() % 213) / 213.0f * 0.317f;
		for (int j = 0; j < i; j++)
		{
			if (m_SNoise[j] == m_SNoise[i] || m_CNoise[j] == m_CNoise[i])
			{
				i--;
				continue;
			}
		}
	}
}

// Init()
// Desc : �簢������ ���ʾƷ��� ���� ������ ���� ���� �޾� �鿩 ����Ʈ�� �����
//     ����Ÿ�� ä���.
// LeftX, LeftZ, RightX, RightZ �� ����Ʈ�� �ε���
void CCloudSea::Init(int LeftX, int LeftZ, int RightX, int RightZ, int nLevel)
{
	float fminY, fmaxY;
	int CenterX, CenterZ;

	// ������ ��� ���� �ٿ�� �ڽ��� ����� ������. upward
	if (nLevel >= LAND_VISIBLE_DEPTH)
	{
		//�ٿ�� �ڽ��� ���
		fmin[0] = (float) (LeftX * CLOUD_WIDTH_F);
		fmin[1] = CLOUD_POS_Y; //(float) (minY);
		fmin[2] = (float) (LeftZ * CLOUD_WIDTH_F);

		fmax[0] = (float) (RightX * CLOUD_WIDTH_F);
		fmax[1] = CLOUD_POS_Y; //(float) (maxY);
		fmax[2] = (float) (RightZ * CLOUD_WIDTH_F);

		return;
	}

	// ��������� ������ ������.
	// ��ǥ��� x, z, ����
	// LeftX < RightX, LeftZ < RightZ
	CenterX = (LeftX + RightX) >> 1;
	CenterZ = (LeftZ + RightZ) >> 1;
	nLevel++;

	if( m_pLeftTop )
		m_pLeftTop->Init(LeftX, CenterZ, CenterX, RightZ, nLevel);
	if( m_pLeftBottom )
		m_pLeftBottom->Init(LeftX, LeftZ, CenterX, CenterZ, nLevel);
	if( m_pRightTop )
		m_pRightTop->Init(CenterX, CenterZ, RightX, RightZ, nLevel);
	if( m_pRightBottom )
		m_pRightBottom->Init(CenterX, LeftZ, RightX, CenterZ, nLevel);

	// ���� �ܰ��� �ٿ�� �ڽ� ���� �ֱ�
	fminY = m_pLeftTop->fmin[1];
	fmaxY = m_pLeftTop->fmax[1];

	fmin[0] = (float) (LeftX * CLOUD_WIDTH);
	fmin[1] = fminY;
	fmin[2] = (float) (LeftZ * CLOUD_WIDTH);

	fmax[0] = (float) (RightX * CLOUD_WIDTH);
	fmax[1] = fmaxY;
	fmax[2] = (float) (RightZ * CLOUD_WIDTH);
}

void CCloudSea::Culling(CFrustum *pFrustum)
{
	static D3DVECTOR vecArray[8];
	// �ٿ�� �ڽ��� �Ʒ��� �� ��
	vecArray[0] = D3DVECTOR(fmin[0], fmin[1], fmin[2]);
	vecArray[1] = D3DVECTOR(fmin[0], fmin[1], fmax[2]);
	vecArray[2] = D3DVECTOR(fmax[0], fmin[1], fmax[2]);
	vecArray[3] = D3DVECTOR(fmax[0], fmin[1], fmin[2]);
	// �ٿ�� �ڽ��� ���� ����
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
		if( m_pLeftTop )
			m_pLeftTop->Culling(pFrustum);
		if( m_pLeftBottom )
			m_pLeftBottom->Culling(pFrustum);
		if( m_pRightTop )
			m_pRightTop->Culling(pFrustum);
		if( m_pRightBottom )
			m_pRightBottom->Culling(pFrustum);
	}
	return;
}

// All In �̸� ������嵵 üũ�� �Ѵ�.
void CCloudSea::PropagateInsideFlag(CCloudSea *pQuadTreeNode)
{
	if (pQuadTreeNode == NULL)
		return;

	pQuadTreeNode->m_bVisibleState = VFCULL_INSIDE;
	PropagateInsideFlag(pQuadTreeNode->m_pLeftTop);
	PropagateInsideFlag(pQuadTreeNode->m_pLeftBottom);
	PropagateInsideFlag(pQuadTreeNode->m_pRightTop);
	PropagateInsideFlag(pQuadTreeNode->m_pRightBottom);
}

void CCloudSea::RenderMain()
{
	if( !GET_D3DDEVICE() )
		return;

	static DWORD dwDayLight;
	LPDIRECTDRAWSURFACE7 lpSurfaceSky, lpSurfaceCloud;

	if (m_bVisibleState == VFCULL_OUTSIDE)
		return;

	if (m_pLeftTop == NULL)
	{
		int w, h, sw, ew, sh, eh;
		float fH_mul_gMap, fW_mul_gfMap;

		ew = (int) (fmax[0]/CLOUD_WIDTH_F);
		sw = (int) (fmin[0] / CLOUD_WIDTH_F) ;
		eh = (int) (fmax[2]/ CLOUD_WIDTH_F);
		sh = (int) (fmin[2]/ CLOUD_WIDTH_F) ;

		dwDayLight = 0x00ffffff;

		// �̺κп��� ����ȭ�ϴ� ���
		// �׷����� �͵��� ����Ʈ�� ���� ���߿� �׸���.
		float fRenderDist;
		fRenderDist = (LAND_VISIBLE_WIDTH / 2 ) * CLOUD_WIDTH_F;
		float cAdd = 1.0f * m_CloudFrame / CLOUDSEA_CLOUD_TIME;
		float sAdd = 1.0f * m_SkyFrame / CLOUDSEA_SKY_TIME;
		for (h = sh; h < eh; h++)
		{

			fH_mul_gMap = (float) (h * CLOUD_WIDTH);
			BaseTri[0].z = fH_mul_gMap + CLOUD_WIDTH;
			BaseTri[1].z = fH_mul_gMap + CLOUD_WIDTH;
			BaseTri[2].z = fH_mul_gMap;
			BaseTri[3].z = fH_mul_gMap;

			for (w = sw; w < ew; w++)
			{
				fW_mul_gfMap = w * CLOUD_WIDTH_F;
				BaseTri[0].x = fW_mul_gfMap;
				BaseTri[1].x = fW_mul_gfMap + CLOUD_WIDTH_F;
				BaseTri[2].x = fW_mul_gfMap;
				BaseTri[3].x = fW_mul_gfMap + CLOUD_WIDTH_F;

				BaseTri[0].y = CLOUD_POS_Y;
				BaseTri[1].y = CLOUD_POS_Y;
				BaseTri[2].y = CLOUD_POS_Y;
				BaseTri[3].y = CLOUD_POS_Y;

				float dist;

				BaseTri[0].color = 0x00ffffff;
				dist = (float) sqrt((m_center_x-BaseTri[0].x)*(m_center_x-BaseTri[0].x)+
									(m_center_z-BaseTri[0].z)*(m_center_z-BaseTri[0].z));

				if (dist > fRenderDist)
					continue;

				BaseTri[1].color = 0x00ffffff;
				dist = (float) sqrt((m_center_x-BaseTri[1].x)*(m_center_x-BaseTri[1].x)+
									(m_center_z-BaseTri[1].z)*(m_center_z-BaseTri[1].z));

				if (dist > fRenderDist)
					continue;

				BaseTri[2].color = 0x00ffffff;
				dist = (float) sqrt((m_center_x-BaseTri[2].x)*(m_center_x-BaseTri[2].x)+
									(m_center_z-BaseTri[2].z)*(m_center_z-BaseTri[2].z));

				if (dist > fRenderDist)
					continue;

				BaseTri[3].color = 0x00ffffff;
				dist = (float) sqrt((m_center_x-BaseTri[3].x)*(m_center_x-BaseTri[3].x)+
									(m_center_z-BaseTri[3].z)*(m_center_z-BaseTri[3].z));
				if (dist > fRenderDist)
					continue;
				// nate 2004 - 7 : Image Manager
				if( g_pCapsyongTexture )
				{
					lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_CLOUDSEA_SKY ] );
					lpSurfaceCloud = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_CLOUDSEA_CLOUD ] );
				}

				BaseTri[0].tu = 0.0f+sAdd+m_SNoise[(w+(h*2)+2)%CLOUDSEA_NOISE_NUM];
				BaseTri[0].tv = 0.0f+m_SNoise[(h+(w*3)-3)%CLOUDSEA_NOISE_NUM];
				BaseTri[1].tu = 1.0f+sAdd+m_SNoise[(w+(h*2)+3)%CLOUDSEA_NOISE_NUM];
				BaseTri[1].tv = 0.0f+m_SNoise[(h+(w*3)-0)%CLOUDSEA_NOISE_NUM];
				BaseTri[2].tu = 0.0f+sAdd+m_SNoise[(w+(h*2)+0)%CLOUDSEA_NOISE_NUM];
				BaseTri[2].tv = 1.0f+m_SNoise[(h+(w*3)-4)%CLOUDSEA_NOISE_NUM];
				BaseTri[3].tu = 1.0f+sAdd+m_SNoise[(w+(h*2)+1)%CLOUDSEA_NOISE_NUM];
				BaseTri[3].tv = 1.0f+m_SNoise[(h+(w*3)-1)%CLOUDSEA_NOISE_NUM];

				bool bDrawDetail = true;
				// ��Ƽ�ؽ�ó ���.
				if (FALSE)
				{
					GET_D3DDEVICE()->SetTexture(0, lpSurfaceSky);
					if (bDrawDetail)
					{
						GET_D3DDEVICE()->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
						GET_D3DDEVICE()->SetTexture(1, lpSurfaceCloud);
						GET_D3DDEVICE()->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_ADD);
					}
					else
						GET_D3DDEVICE()->SetTextureStageState (1, D3DTSS_COLOROP, D3DTOP_DISABLE );

					// Draw
					GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);


				}
				else
				{
					// two pass, blended
					GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
					GET_D3DDEVICE()->SetTexture(0, lpSurfaceSky);
					GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);

					if (bDrawDetail)
					{
						BaseTri[0].tu = 0.0f+cAdd+m_CNoise[(w-(h*2)+0)%CLOUDSEA_NOISE_NUM];
						BaseTri[0].tv = 0.0f+m_CNoise[(h-(w*3)-0)%CLOUDSEA_NOISE_NUM];
						BaseTri[1].tu = 1.0f+cAdd+m_CNoise[(w-(h*2)+1)%CLOUDSEA_NOISE_NUM];
						BaseTri[1].tv = 0.0f+m_CNoise[(h-(w*3)-3)%CLOUDSEA_NOISE_NUM];
						BaseTri[2].tu = 0.0f+cAdd+m_CNoise[(w-(h*2)+2)%CLOUDSEA_NOISE_NUM];
						BaseTri[2].tv = 1.0f+m_CNoise[(h-(w*3)-1)%CLOUDSEA_NOISE_NUM];
						BaseTri[3].tu = 1.0f+cAdd+m_CNoise[(w-(h*2)+3)%CLOUDSEA_NOISE_NUM];
						BaseTri[3].tv = 1.0f+m_CNoise[(h-(w*3)-4)%CLOUDSEA_NOISE_NUM];

						GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
						GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
						GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
						GET_D3DDEVICE()->SetTexture( 0, lpSurfaceCloud);

						GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);

						GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
					}
				}
				GET_D3DDEVICE()->SetTextureStageState (1, D3DTSS_COLOROP, D3DTOP_DISABLE );
			}
		}
	}
	else
	{
		if( m_pLeftTop )
			m_pLeftTop->RenderMain();
		if( m_pLeftBottom )
			m_pLeftBottom->RenderMain();
		if( m_pRightTop )
			m_pRightTop->RenderMain();
		if( m_pRightBottom )
			m_pRightBottom->RenderMain();
	}
}

// QuadInit()
// Desc : ���� Ʈ���� ����Ÿ�� �ʱ�ȭ �Ѵ�.
// float x, y : ���� ��ǥ
void CCloudSea::QuadInit(CFrustum *pFrustum, float x, float z)
{
	int dx, dz;
	int max_x, max_z, min_x, min_z;			// �������� ���� ��ǥ.

	dx = (int) (x / CLOUD_WIDTH_F);
	dz = (int) (z / CLOUD_WIDTH_F);

	max_x = dx + LAND_VISIBLE_WIDTH/2;
	min_x = dx - LAND_VISIBLE_WIDTH/2;

	max_z = dz + LAND_VISIBLE_WIDTH/2;
	min_z = dz - LAND_VISIBLE_WIDTH/2;

	// �� ��ǥ�� �Ѱ� �ش�.
	Init(min_x, min_z, max_x, max_z, 0);
	Culling(pFrustum);
}

void CCloudSea::Render(float x, float z, BOOL bFog)
{
	if( !GET_D3DDEVICE() )
		return;

	BOOL bPrevState = FALSE;
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING , FALSE);

	if (g_bMipmap)
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );

	if (g_bDetailMipmap)
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );

	m_SkyFrame = g_dwBaseTime % CLOUDSEA_SKY_TIME;
	m_CloudFrame = g_dwBaseTime % CLOUDSEA_CLOUD_TIME;

	SetCenterPos(x, z);
	SetFog(bFog);
	RenderMain();
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);

	if (g_bMipmap)
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );

	if (g_bDetailMipmap)
		GET_D3DDEVICE()->SetTextureStageState(1, D3DTSS_MIPFILTER, D3DTFP_NONE );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING , TRUE);
}

extern int g_cR, g_cG, g_cB;
void CCloudSea::SetFog(BOOL bFog)
{
	if( !GET_D3DDEVICE() )
		return;

	if (bFog)
	{
		BOOL bUsingTableFog = FALSE;		// pixel fog�� table �����̴�.
		BOOL bRangeBasedFog = FALSE;		// �����ϴ� �ϵ��� ����. ���� vertexfog���� cpu�� ��´�.

		float fFogStart = (0.5f * (6000.0f-20.0f)) + 20.0f; // ( fStart*(m_zFar-m_zNear) ) + m_zNear
		float fFogEnd = (1.3f * (6000.0f-20.0f)) + 20.0f; // ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
		float fFogDensity = 0.0f;
		DWORD dwFogColor = RGBA_MAKE ( 48, 64, 110, 255 );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwFogColor);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&fFogStart)));
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&fFogEnd)));
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&fFogDensity)));

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  D3DFOG_LINEAR );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, bRangeBasedFog );

	}
	else
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
}