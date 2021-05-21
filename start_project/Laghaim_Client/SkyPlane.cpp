#include "stdafx.h"
// SkyPlane.cpp: implementation of the CSkyPlane class.
//
//////////////////////////////////////////////////////////////////////

#define D3D_OVERLOADS


#include <d3d.h>

#include "IndexedTexture.h"
#include "SkyPlane.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "Main.h"
#include "Land.h"
//===========================================
// nate 2004 - 7
// Image Image Manager
#include "ImageDefine.h"
extern int g_EffectTextr[];
//===========================================



extern BOOL		g_bMipmap;

extern DWORD	g_dwBaseTime;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkyPlane::CSkyPlane()
{
	memset(m_Indices, 0, sizeof(m_Indices));
	memset(m_tU, 0, sizeof(m_tU));

	m_PlanetRadius = 0.0f;
	m_AtmosphereRadius = 0.0f;
	m_HTileFactor = 1.0f;
	m_VTileFactor = 1.0f;

//===========================================
	// nate 2004 - 7
	// Image Image Manager
	/*
		m_pIndexedTexture = g_pCapsyongTexture;

		m_textrSky[0] = m_pIndexedTexture->CreateTextureFromFile("sky/sebuk_sky.bmp");
		m_textrSky[1] = m_pIndexedTexture->CreateTextureFromFile("sky/day_sky.bmp");
		m_textrSky[2] = m_pIndexedTexture->CreateTextureFromFile("sky/noul_sky.bmp");
		m_textrSky[3] = m_pIndexedTexture->CreateTextureFromFile("sky/night_sky.bmp");
		m_textrSky[4] = m_pIndexedTexture->CreateTextureFromFile("sky/flow_cloud3.bmp");
		m_textrSky[5] = m_pIndexedTexture->CreateTextureFromFile("sky/mat_sky.bmp");	// matrix
	*/
//===========================================
}

CSkyPlane::CSkyPlane(IndexedTexture *pIndexedTexture)
{
	memset(m_Indices, 0, sizeof(m_Indices));
	memset(m_tU, 0, sizeof(m_tU));

	m_PlanetRadius = 0.0f;
	m_AtmosphereRadius = 0.0f;
	m_HTileFactor = 1.0f;
	m_VTileFactor = 1.0f;

//===========================================
	// nate 2004 - 7
	// Image Image Manager
//	m_pIndexedTexture = pIndexedTexture;

//	m_textrSky[0] = m_pIndexedTexture->CreateTextureFromFile("sky/flow_cloud3.bmp");
//===========================================
}

CSkyPlane::~CSkyPlane()
{
}

// Initialize
BOOL CSkyPlane::Init(float planetRadius,
					 float atmosphereRadius,
					 float hTilefactor,
					 float vTilefactor,
					 float scale)
{
	//atmosphereRadius = 295.0f;
	m_PlanetRadius = planetRadius;
	m_AtmosphereRadius = atmosphereRadius;
	m_HTileFactor = hTilefactor;
	m_VTileFactor = vTilefactor;

	if (!SetupVertices(scale))
	{
		OutputDebugString("unable to set up verts");
		return FALSE;
	}

	if (!SetupIndices())
	{
		OutputDebugString("unable to set up indices");
		return FALSE;
	}

	return TRUE;
}

// Generate vertices that describe sphere geometry
BOOL CSkyPlane::SetupVertices(float scale)
{
	float plane_size = 2.0f * (float)sqrt((m_AtmosphereRadius*m_AtmosphereRadius) -
										  (m_PlanetRadius*m_PlanetRadius));

	float delta = plane_size/(float)SKYPLANE_DIV;
	float tex_delta = 2.0f/(float)SKYPLANE_DIV;
	float x_dist   = 0.0f;
	float z_dist   = 0.0f;
	float x_height = 0.0f;
	float z_height = 0.0f;
	float height = 0.0f;

	D3DMATRIX matScale;
	D3DUtil_SetScaleMatrix(matScale, scale, scale, scale);

	for (int i=0; i<=SKYPLANE_DIV; i++)
	{
		for (int j=0; j<=SKYPLANE_DIV; j++)
		{
			x_dist = (-0.5f * plane_size) + ((float)j*delta);
			z_dist = (-0.5f * plane_size) + ((float)i*delta);

			x_height = (x_dist*x_dist) / m_AtmosphereRadius;
			z_height = (z_dist*z_dist) / m_AtmosphereRadius;
			height = x_height + z_height;

			D3DVECTOR v = D3DVECTOR( x_dist, 0.0f - height, z_dist);
			D3DMath_VectorMatrixMultiply(v, v, matScale);
			m_tU[i*(SKYPLANE_DIV+1)+j] = m_HTileFactor*((float)j * tex_delta*0.5f);
			m_Vertices[i*(SKYPLANE_DIV+1)+j] =
				D3DLVERTEX(v, 0x00FFFFFF, 0,
						   m_tU[i*(SKYPLANE_DIV+1)+j], m_VTileFactor*(1.0f - (float)i * tex_delta*0.5f));
		}
	}

	return TRUE;
}


BOOL CSkyPlane::SetupIndices()
{
	int index = 0;

	for (int i=0; i<SKYPLANE_DIV; i++)
	{
		for (int j=0; j<SKYPLANE_DIV; j++)
		{
			int startvert = (i*(SKYPLANE_DIV+1) + j);

			// tri 1
			m_Indices[index++] = startvert;
			m_Indices[index++] = startvert+1;
			m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+1;

			// tri 2
			m_Indices[index++] = startvert+1;
			m_Indices[index++] = startvert+SKYPLANE_DIV+2;
			m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+SKYPLANE_DIV+1;
			//m_Indices[index++] = startvert+SKYPLANE_DIV+2;
		}
	}

	return TRUE;
}

void CSkyPlane::Render(float x, float y, float z, BOOL bFog)
{
	D3DMATRIX matWorld;

	float yTemp = y;

	if(pCMyApp->GetCurWorld() == WORLD_BOSSRAID_AIDIA || pCMyApp->GetCurWorld() == WORLD_BOSSRAID_FINALBOSS )
		yTemp += 1500;

	D3DUtil_SetTranslateMatrix( matWorld, x, yTemp, z);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	// 텍스춰 좌표를 이동시켜주자!
	for (int i=0; i<=SKYPLANE_DIV; i++)
	{
		for (int j=0; j<=SKYPLANE_DIV; j++)
		{
			m_Vertices[i*(SKYPLANE_DIV+1)+j].tu = m_tU[i*(SKYPLANE_DIV+1)+j] +
												  g_dwBaseTime % SKYPLANE_TIME * 1.0f / SKYPLANE_TIME;
		}
	}
	// 설정하고 그리자!
	SetFog(bFog);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING , FALSE);

	if (g_bMipmap)
	{
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
	}

	LPDIRECTDRAWSURFACE7 lpSurfaceSky;

	int world_num;
	world_num = pCMyApp->GetCurWorld();
	/*	if (pCMyApp->GetCurWorld() == WORLD_SKY) {
			lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[4]);	// 천공의 성.
		} else if (world_num == 7) {
			lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[5]);	// matrix
		} else {
			if(pCMyApp->m_nCurHour >= 2 && pCMyApp->m_nCurHour <= 6) { // 새벽
				lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[0]);
			} else if(pCMyApp->m_nCurHour >= 7 && pCMyApp->m_nCurHour <= 21) { // 낮
				lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[1]);
			} else if(pCMyApp->m_nCurHour >= 22 && pCMyApp->m_nCurHour <= 27) { // 저녁
				lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[2]);
			} else {	// 밤
				lpSurfaceSky = m_pIndexedTexture->GetSurfaceByIdx(m_textrSky[3]);
			}
		}
	*/
//===========================================
	// nate 2004 - 7
	// Image Image Manager
	if (pCMyApp->GetCurWorld() == WORLD_SKY)
	{
		lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY04 ] );	// 천공의 성.
	}
	else if (world_num == 7)
	{
		lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY05 ] );	// matrix
	}
	else
	{
		if( world_num == WORLD_BOSSRAID_BULKAN && (pCMyApp->m_nCurHour == 2 || pCMyApp->m_nCurHour == 3))
		{
			// 새벽
			lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY03 ] );
		}
		else if (world_num == WORLD_BOSSRAID_HUMAN || world_num == WORLD_BOSSRAID_FINALBOSS )
		{
			lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY03 ] );
		}
		else if(pCMyApp->m_nCurHour >= 2 && pCMyApp->m_nCurHour <= 6)
		{
			// 새벽
			lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY00 ] );
		}
		else if(pCMyApp->m_nCurHour >= 7 && pCMyApp->m_nCurHour <= 21)
		{
			// 낮
			lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY01 ] );
		}
		else if(pCMyApp->m_nCurHour >= 22 && pCMyApp->m_nCurHour <= 27)
		{
			// 저녁
			lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY02 ] );
		}
		else
		{
			// 밤
			lpSurfaceSky = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_TEXTRSKY03 ] );
		}
	}
//===========================================
	GET_D3DDEVICE()->SetTexture(0, lpSurfaceSky);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX,
										   m_Vertices, (SKYPLANE_DIV+1)*(SKYPLANE_DIV+1),
										   m_Indices, SKYPLANE_DIV*SKYPLANE_DIV*6, 0 );

	if (g_bMipmap)
	{
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );
	}
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
}

extern float g_cS, g_cE, g_cD;

DWORD g_skyFogColor[30] =
{
	RGBA_MAKE(25, 53, 115, 255), RGBA_MAKE(25, 53, 115, 255),	// 0, 1
	RGBA_MAKE(135, 198, 255, 255), RGBA_MAKE(135, 198, 255, 255),	// 새벽 2, 3
	RGBA_MAKE(135, 198, 255, 255), RGBA_MAKE(135, 198, 255, 255),	// 4, 5
	RGBA_MAKE(135, 198, 255, 255), RGBA_MAKE(134, 177, 210, 255),	// 6, 낮 7
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 8, 9
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 10, 11
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 12, 13
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 14, 15
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 16, 17
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 18, 19
	RGBA_MAKE(134, 177, 210, 255), RGBA_MAKE(134, 177, 210, 255),	// 20, 21
	RGBA_MAKE(255, 185, 140, 255), RGBA_MAKE(255, 185, 140, 255),	// 저녁 22, 23
	RGBA_MAKE(255, 185, 140, 255), RGBA_MAKE(255, 185, 140, 255),	// 24, 25
	RGBA_MAKE(255, 185, 140, 255), RGBA_MAKE(255, 185, 140, 255),	// 26, 27
	RGBA_MAKE(25, 53, 115, 255), RGBA_MAKE(25, 53, 115, 255),	// 밤 28, 29
};

void CSkyPlane::SetFog(BOOL bFog)
{
	if (bFog)
	{
		BOOL bUsingTableFog = FALSE;		// pixel fog가 table 포그이다.
		BOOL bRangeBasedFog = FALSE;		// 지원하는 하드웨어가 없다. 오직 vertexfog에서 cpu를 잡는다.

		float fFogStart = (1.16f * (6000.0f-20.0f)) + 20.0f; // ( fStart*(m_zFar-m_zNear) ) + m_zNear
		float fFogEnd = (1.4f * (6000.0f-20.0f)) + 20.0f; // ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
		//float fFogStart = (g_cS * (6000.0f-20.0f)) + 20.0f; // ( fStart*(m_zFar-m_zNear) ) + m_zNear
		//float fFogEnd = (g_cE * (6000.0f-20.0f)) + 20.0f; // ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
		float fFogDensity = 0.0f;
		//float fFogDensity = g_cD;
		//DWORD dwFogColor = RGBA_MAKE ( 163, 189, 255, 255 );
		DWORD dwFogColor;
		if (pCMyApp->GetCurWorld() == WORLD_SKY)
		{
			dwFogColor = RGBA_MAKE ( 57, 81, 156, 255 );
		}
		else if( pCMyApp->GetCurWorldID() == WORLD_BOSSRAID_BULKAN )
		{
			if( pCMyApp->m_nCurHour == 2 || pCMyApp->m_nCurHour == 3 )
				dwFogColor = g_skyFogColor[0];
			else
				dwFogColor = g_skyFogColor[pCMyApp->m_nCurHour];
		}
		else if( pCMyApp->GetCurWorldID() == WORLD_BOSSRAID_HUMAN )
			dwFogColor = g_skyFogColor[0];
		else if( pCMyApp->GetCurWorldID() == WORLD_BOSSRAID_FINALBOSS)
			dwFogColor = g_skyFogColor[0];
		else
			dwFogColor = g_skyFogColor[pCMyApp->m_nCurHour];

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
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
	}
}