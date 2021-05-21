#include "stdafx.h"
#define D3D_OVERLOADS


#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "UIMgr.h"
#include "Land.h"
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
#include "Fx_BossRaid_human2_02.h"



CFxBossRaidHuman2_02::CFxBossRaidHuman2_02()
{
	vPos[0] = D3DVECTOR(0, 0, 0);
	vPos[1] = D3DVECTOR(0, 0, 0);
	vPos[2] = D3DVECTOR(0, 0, 0);
	vPos[3] = D3DVECTOR(0, 0, 0);

	m_pLolo[0] = NULL; //날아가는 구체
	m_pLolo[1] = NULL; //바닥
}

CFxBossRaidHuman2_02::~CFxBossRaidHuman2_02()
{
	DeleteRes();
}

void CFxBossRaidHuman2_02::LoadRes()
{
	if (!m_pLolo[0])
	{
		m_pLolo[0] = new CLolos;
		m_pLolo[0]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[0]->Read("data/effect/lolos/spider_Fire.LOL");
	}
	if (!m_pLolo[1])
	{
		m_pLolo[1] = new CLolos;
		m_pLolo[1]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[1]->Read("data/effect/lolos/spider_wire.LOL");
	}



}

void CFxBossRaidHuman2_02::DeleteRes()
{
	SAFE_DELETE(m_pLolo[0]);
	SAFE_DELETE(m_pLolo[1]);
}

bool CFxBossRaidHuman2_02::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return FALSE;

	//-- 사용 캐릭이 사라질 경우
	if(!effect_sort.pMobFrom)
		return FALSE;

	DWORD dwLighting, dwSrc, dwDest;
	DWORD dwTextureState;

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING,	&dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND,   &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND,  &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,	FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView,matRotate, matWorld,matTemp;
	D3DVECTOR vTmp;

	Deco_Particle* pObj = effect_sort.pParticle;

	D3DVECTOR vDiff = effect_sort.vTargetPos - effect_sort.vCurPos;
	float theta;
	theta = atan2(vDiff.z, vDiff.x) + g_PI * 0.5f;

	if (effect_sort.nCurFrame > 30 &&
			effect_sort.nCurFrame <= 38)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);

		Nk3DUtil_SetRotateCCWYMatrix(matRotate, theta );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos);

		D3DMath_MatrixMultiply(matWorld, matRotate, matTrans);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_pLolo[0]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame - 30, FALSE, FALSE);
	}
	if (effect_sort.nCurFrame > 36 )
	{
		DWORD dwTime = timeGetTime() - effect_sort.dwColor;
		float fTime = (float)dwTime / 1000.0f;
		float radius = 1.0f;
		DWORD a, w;
		a = 0;
		if (fTime < 3.0f)
		{
			a = 0xFF * 1.00f;
		}
		else
		{
			if (fTime > 4.0f) fTime = 4.0f;
			a = 0xFF * (4.0f - fTime);
		}
		w = (a << 24) | (a << 16) | (a << 8) | (a);

		D3DUtil_SetIdentityMatrix(matTemp);
		D3DUtil_SetIdentityMatrix(matWorld);
		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);

		D3DVECTOR v1(0, 0, -90), v2;
		Nk3DUtil_SetRotateCCWYMatrix(matRotate, theta );
		D3DMath_VectorMatrixMultiply( v2, v1, matRotate );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos + v2 );

		D3DMath_MatrixMultiply(matWorld, matRotate, matTrans);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		matTrans._11 = matTrans._22 = matTrans._33 = 10.0f;

		m_pLolo[1]->RenderRaw( GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE, FALSE, &matTrans, &matView, FALSE, FALSE, w);
	}

	//상태 복원
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  dwDest );

	return TRUE;
}
