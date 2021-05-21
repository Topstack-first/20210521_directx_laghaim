#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include "FxSet.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "GUtil.h"
#include "FxHellDown_Spark.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHellDown_Spark::CFxHellDown_Spark()
{
	m_nTotalFrame = FX_HELLDOWN_FRAMECOUNT;
	m_fRadius = 3.0f;
	m_fCycleRadius = 5.5f;
	m_fHeight = m_fRadius;

	m_pMesh = new D3DLVERTEX[4*FX_SHOCKSPEARRINGNUM];
	m_pIndices = new WORD[6*FX_SHOCKSPEARRINGNUM];

	m_nStartRingSize[0] = 20;
	m_nStartRingSize[1] = 30;
	m_nStartRingSize[2] = 47;
	m_nStartRingSize[3] = 50;
	m_nStartRingSize[4] = 50;
	m_nStartRingSize[5] = 45;
	m_nStartRingSize[6] = 35;
	m_nStartRingSize[7] = 20;
	m_nStartRingSize[8] = 5;
	m_nStartRingSize[9] = 0;

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(m_fRadius, -m_fRadius, 0), 0x00ffffff, 0, 1.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fRadius, -m_fRadius, 0), 0x00ffffff, 0, 0.0f, 1.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR(-m_fRadius, m_fRadius, 0), 0x00ffffff, 0, 0.0f, 0.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fRadius, m_fRadius, 0), 0x00ffffff, 0, 1.0f, 0.0f );

	float x, z;
	int i;

	for (i=0; i < FX_SHOCKSPEARRINGNUM; i++)
	{
		x = m_fCycleRadius*cosf(i*1.0471f);		// 1.0471f == 60도.
		z = m_fCycleRadius*sinf(i*1.0471f);

		m_ShowupRing[i].position.x = x;
		m_ShowupRing[i].position.y = 0.0f;
		m_ShowupRing[i].position.z = z;
		m_ShowupRing[i].color = D3DVECTOR(rnd(), rnd(), rnd());
	}

	for (i=0; i < FX_SHOCKSPEARRINGNUM; i++)
	{
		m_pIndices[i*6+0] = 4*i + 0;
		m_pIndices[i*6+1] = 4*i + 1;
		m_pIndices[i*6+2] = 4*i + 2;
		m_pIndices[i*6+3] = 4*i + 0;
		m_pIndices[i*6+4] = 4*i + 2;
		m_pIndices[i*6+5] = 4*i + 3;
	}

	D3DVECTOR vColor = D3DVECTOR(rnd()/2+0.5f, rnd()/2+0.5f, rnd()/2+0.5f);
	m_lverRect[0].color = D3DRGB(vColor.x, vColor.y, vColor.z);
	m_lverRect[1].color = m_lverRect[2].color = m_lverRect[3].color = m_lverRect[0].color;

	memcpy(m_lverRot, m_lverRect, sizeof(m_lverRect));

	light1				= NULL;
	light2				= NULL;
	light3				= NULL;
	m_numTLight			= 0;

	Create();
}

CFxHellDown_Spark::~CFxHellDown_Spark()
{
	DeleteRes();

	SAFE_DELETE_ARRAY( m_pMesh );
	SAFE_DELETE_ARRAY( m_pIndices );
	SAFE_DELETE( light1 );
	SAFE_DELETE( light2 );
	SAFE_DELETE( light3 );
}

void CFxHellDown_Spark::Create()
{
	SAFE_DELETE( light1 );
	SAFE_DELETE( light2 );
	SAFE_DELETE( light3 );

	light1 = new CRectPlane;
	if( light1 )
		light1->Create( 10.0f, 10.0f, TRUE );
	light2 = new CRectPlane;
	if( light2 )
		light2->Create( 10.0f, 10.0f, TRUE );
	light3 = new CRectPlane;
	if( light3 )
		light3->Create( 10.0f, 10.0f, TRUE );
}

void CFxHellDown_Spark::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{
}

void CFxHellDown_Spark::DeleteRes()
{
}

void CFxHellDown_Spark::RenderRing(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	int i, j;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRyCycle, matRyRev, matWorld, matRot;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vPos;

	int nFrame = effect_sort.nCurFrame;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		return;

	// 원환을 그려주는 위치.
	m_fHeight = m_fRadius + 0.5f * nFrame;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_DESTALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_SRCALPHA );

	// 싸이클 회전각 계산과 빌보드 회전 보정각 계산.
	D3DUtil_SetRotateYMatrix(matRyCycle, g_2_PI * (float) nFrame / 10);
	D3DUtil_SetRotateYMatrix(matRyRev, -g_2_PI * (float) nFrame / 10);

	// 빌보드 회전.
	vViewVec = pCMyApp->GetEyeDirection();
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DMath_MatrixMultiply(matRx, matRx, matRyRev);		// 싸이클 회전에 의한 보정.
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matRot, matRx, matRy);

	// 기본 사각형을 카메라를 향하게 회전.
	NKMath_LVertexMatrixMultiply(m_lverRot[0], m_lverRect[0], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[1], m_lverRect[1], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[2], m_lverRect[2], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[3], m_lverRect[3], matRot);

	// 그릴 메쉬 데이타
	for (i=0; i < FX_SHOWUPRINGNUM; i++)
	{
		D3DCOLOR fColor = D3DRGB(RandomNum(0.0f, 1.0f), RandomNum(0.0f, 1.0f), RandomNum(0.0f, 1.0f));

		float x, z;
		x = (m_fCycleRadius/nFrame)*cosf(i*1.0471f);		// 1.0471f == 60도.
		z = (m_fCycleRadius/nFrame)*sinf(i*1.0471f);

		m_ShowupRing[i].position.x = x;
		m_ShowupRing[i].position.y = 0.0f;
		m_ShowupRing[i].position.z = z;

		for (j=0; j < 4; j++)
		{
			m_pMesh[i*4+j].x = m_ShowupRing[i].position.x + m_lverRot[j].x;
			m_pMesh[i*4+j].y = m_ShowupRing[i].position.y + m_lverRot[j].y;
			m_pMesh[i*4+j].z = m_ShowupRing[i].position.z + m_lverRot[j].z;

			m_pMesh[i*4+j].color = fColor;
			m_pMesh[i*4+j].specular = 0.0f;
			m_pMesh[i*4+j].tu = m_lverRot[j].tu;
			m_pMesh[i*4+j].tv = m_lverRot[j].tv;
		}
	}

	// 월드공간 이동 좌표.
	D3DUtil_SetIdentityMatrix(matWorld);
	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y+m_fHeight+1.0f;
	matWorld._43 = vPos.z;

	D3DMath_MatrixMultiply(matWorld, matRyCycle, matWorld);

	// 월드 트랜스폼.
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOCKSPEAR ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	if (nFrame < 8)
		GET_D3DDEVICE()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DFVF_LVERTEX,
											  (LPVOID)m_pMesh, FX_SHOWUPRINGNUM*4 , m_pIndices, FX_SHOWUPRINGNUM*6, 0);
}

void CFxHellDown_Spark::RenderBoom(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRyCycle, matRyRev, matWorld, matRot;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vPos;

	int nFrame = effect_sort.nCurFrame;

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		return;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	if (nFrame > 6)
	{
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y + 12.0f;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOCKSPEARSTART ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		int size = 0;
		size = nFrame - 6;

		if (size >= 0 && size < 9)
			MakeLVertexRect(m_lverBoom, m_nStartRingSize[size], m_nStartRingSize[size], TRUE);
	}
}

void CFxHellDown_Spark::RenderLight(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7	lpSurface = NULL;
	D3DVECTOR				vPos;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	if (effect_sort.pMobTo)
		vPos = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);

	if( ++m_numTLight >= 4 )
		m_numTLight = 0;

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	int nFrame = effect_sort.nCurFrame;

	if (nFrame > 6)
	{
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOCKSPEARLIGHT + m_numTLight ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		if( light1 )
		{
			light1->SetColor(1.0f, 0.2f, 0.0f, 1.0f);
			light1->SetRotationY( g_PI * 0.5f );
			light1->RotateZ( -nFrame * 0.3f );
			light1->Scale(3.0f, 0.0f, 3.0f);
			light1->SetTranslationXYZ(vPos.x, vPos.y + 30.0f, vPos.z);
			light1->Render();
		}

		if( light2 )
		{
			light2->SetColor(1.0f, 0.2f, 0.0f, 1.0f);
			light2->SetRotationX( g_PI * 0.5f );
			light2->RotateZ( -nFrame * 0.3f );
			light2->Scale(3.0f, 0.0f, 3.0f);
			light2->SetTranslationXYZ(vPos.x, vPos.y + 30.0f, vPos.z);
			light2->Render();
		}

		if( light3 )
		{
			light3->SetColor(1.0f, 0.2f, 0.0f, 1.0f);
			light3->SetRotationY( g_PI * 0.5f );
			light3->RotateX( -nFrame * 0.3f );
			light3->Scale(3.0f, 0.0f, 3.0f);
			light3->SetTranslationXYZ(vPos.x, vPos.y + 30.0f, vPos.z);
			light3->Render();
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}

void CFxHellDown_Spark::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	RenderRing(effect_sort);
	RenderBoom(effect_sort);
	RenderLight(effect_sort);
}
