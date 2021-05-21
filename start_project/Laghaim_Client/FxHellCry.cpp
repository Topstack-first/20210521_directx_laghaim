#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "FxHellCry.h"


// nate 2004 - 7 글로벌 ShadowRect사용

extern float g_fDSDistance;

extern int g_EffectSound[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxHellCry::CFxHellCry()
{
	m_pLolo = NULL;
	smoke = new CRectPlane;
	if( smoke )
		smoke->Create(15.0f, 15.0f, FALSE);
}

CFxHellCry::~CFxHellCry()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(smoke);
}

// nate 2004 - 8 ImageManager
void CFxHellCry::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/firewave.lol");
	}



}

void CFxHellCry::DeleteRes()
{}

void CFxHellCry::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView,matRotate, matWorld,matWorld2,matTemp;
	DWORD dwLighting, dwZWrite, dwSrc, dwDest;
	D3DMATRIX matRx, matRy;
//==========================================
	// nate2004 - 6
	// RenderState 버그 수정
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
//==========================================

	int nFrame;
	nFrame = effect_sort.nCurFrame-12;

	D3DVECTOR vDrawPos;
	D3DVECTOR vDir, vPosFrom, vPosTo;
	// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 60.0f;
	// 타겟 좌표를 구한다.
	vPosTo = effect_sort.vPos;
	// 방향.
	get_normal_vector(vDir, vPosFrom, vPosTo);
	// 프레임당 스피드.
	vDir *= 6.0f;
	// 현재 프레임에서 8프레임 전까지 그린다.
	int end_frame;

	if(nFrame<0)
		return;

	end_frame = nFrame;

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld2, matRx, matRy);

//--------------------------------------------------------------------------------------
	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+3);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+3);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+3);
//--------------------------------------------------------------------------------------
	// 검기 쏘기
	// 텍스처 택하기.
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_SWORD ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	m_lsword[0] = D3DLVERTEX( D3DVECTOR(-10.5f, -10.5f, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lsword[1] = D3DLVERTEX( D3DVECTOR(-10.5f, 10.5f, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lsword[2] = D3DLVERTEX( D3DVECTOR( 10.5f, -10.5f, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lsword[3] = D3DLVERTEX( D3DVECTOR( 10.5f, 10.5f, 0), 0xffffffff, 0, 1.0f, 0.0f );
	m_lsword[4] = D3DLVERTEX( D3DVECTOR( -10.5f, -10.5f, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lsword[5] = D3DLVERTEX( D3DVECTOR( -10.5f, 10.5f, 0), 0xffffffff, 0, 0.0f, 0.0f );

	float fDir = atan2f(vDir.z, vDir.x);
	Nk3DUtil_SetRotateCCWYMatrix(matRotate, fDir);
	D3DUtil_SetRotateZMatrix(matTemp, nFrame * 10.0f);
	D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);

	D3DMath_MatrixMultiply( matRotate, matTemp, matRotate );
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lsword, 6, 0 );
//----------------------------------------------------------------------
	if (m_pLolo)
	{
		if (effect_sort.nCurFrame < 19)
			return;

		D3DUtil_SetTranslateMatrix( matWorld2, vPosTo.x-3.0f, vPosTo.y, vPosTo.z-3.0f);
		pCMyApp->SetViewMatrix(matView);
//--------------------------------------------------------------------------------------
		//스케일 조절
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix(matS, 1.5f,1.5f,1.5f);
		D3DMath_MatrixMultiply(matWorld2, matS, matWorld2);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld2 );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
//----------------------------------------------------------------------
		//갈라지는 바닥
		if( g_pShadowRect )
		{
//===========================================================
			// nate2004 - 6
			// RenderState는 밖에서
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);
//===========================================================
			if( g_pCapsyongTexture )
			{
				if(effect_sort.nCurFrame < 22)
					lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_BASE1 ] ) ;
				else if(effect_sort.nCurFrame < 23)
					lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_BASE2 ] ) ;
				else if(effect_sort.nCurFrame < 26)
					lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_BASE3 ] ) ;
				else
					lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_BASE4 ] ) ;
			}

			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo,150 , FALSE);// + (effect_sort.nCurFrame * 0.1f), FALSE);		// 부울값은 시간인가 아닌가
			g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
		}

		if(effect_sort.nCurFrame > 27 )//&& effect_sort.nCurFrame < 30)
		{
			if(effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if(effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();
			//솟아오르는 불꽃
			// nate2004 - 6
			// 불필요 RenderState 제거
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );
			if( m_pLolo )
				m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
		}
//===========================================================
		// nate2004 - 6
		// TextureStateState도 쓰고나서 돌려주기
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

		DWORD  dwTextureState[3];

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_SMOKE ] ) ;
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();
//----------------------------------------------------------------------
		//지면에서 올라오는 연기
		float up_height = effect_sort.nCurFrame * 0.5f + 4.0f;
		float smoke_fade = 1.0f - (effect_sort.nCurFrame / (float)FX_HELLCRY_FRAMECOUNT);

		if( smoke )
		{
			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->Scale(3.0f, 3.0f, 3.0f);
			smoke->SetTranslationXYZ(effect_sort.vPos.x , effect_sort.vPos.y + up_height* 1.5f, effect_sort.vPos.z);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->Scale(2.0f, 2.0f, 2.0f);
			smoke->SetTranslationXYZ(effect_sort.vPos.x + 15.0f, effect_sort.vPos.y + up_height, effect_sort.vPos.z);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->Scale(2.0f, 2.0f, 2.0f);
			smoke->SetTranslationXYZ(effect_sort.vPos.x, effect_sort.vPos.y + up_height, effect_sort.vPos.z + 15.0f);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->Scale(2.0f, 2.0f, 2.0f);
			smoke->SetTranslationXYZ(effect_sort.vPos.x - 15.0f, effect_sort.vPos.y + up_height, effect_sort.vPos.z);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->Scale(2.0f, 2.0f, 2.0f);
			smoke->SetTranslationXYZ(effect_sort.vPos.x, effect_sort.vPos.y + up_height, effect_sort.vPos.z - 15.0f);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->Scale(2.0f, 2.0f, 2.0f);
			smoke->SetTranslationXYZ(effect_sort.vPos.x + 15.0f, effect_sort.vPos.y + up_height, effect_sort.vPos.z + 15.0f);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->Scale(2.0f, 2.0f, 2.0f);
			smoke->SetTranslationXYZ(effect_sort.vPos.x - 15.0f, effect_sort.vPos.y + up_height, effect_sort.vPos.z - 15.0f);
			smoke->Render();
		}

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	}
//==========================================
	// nate2004 - 6
	// RenderState 되돌리기
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );
//==========================================

}
