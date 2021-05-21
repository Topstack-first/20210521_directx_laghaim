#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "Fx_BossRaid_bulkan3_03.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "NkMob.h"



extern float g_fTimeKey;
extern float g_fDSDistance;

extern int g_EffectSound[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidBulkan3_03::CFxBossRaidBulkan3_03()
{
	m_pLolo = NULL;
	m_fSpeed = 5.0f;
	smoke = NULL;
	smoke = new CRectPlane;
	if( smoke )
		smoke->Create(15.0f, 15.0f, FALSE);
}

CFxBossRaidBulkan3_03::~CFxBossRaidBulkan3_03()
{
	DeleteRes();
}

void CFxBossRaidBulkan3_03::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(smoke);
}

// nate 2004 - 8 ImageManager
void CFxBossRaidBulkan3_03::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/k_s_hurricane.LOL");
	}



}


BOOL CFxBossRaidBulkan3_03::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matTrans, matView;
	D3DMATRIX matRx, matRy, matWorld;
	DWORD dwLighting, dwSrc, dwDest;
	int	  m_Alpha = 255;
	float m_scaleRate = 2.0f;;

//===================================================================
	// nate2004 - 6
	// RenderState 되돌리기 대 작전
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND,   &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND,  &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );
//===================================================================
	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= m_fSpeed;

	if( effect_sort.nCurFrame < (float)(FX_BOSSRAID_BULKAN3_02_FRAMELIMITE - 16) )
	{
		vDrawPos.x = vPosFrom.x;
		vDrawPos.y = vPosFrom.y;
		vDrawPos.z = vPosFrom.z;
	}
	else if( (effect_sort.nCurFrame >= (float)(FX_BOSSRAID_BULKAN3_02_FRAMELIMITE - 16))
			 && (effect_sort.nCurFrame < (float)(FX_BOSSRAID_BULKAN3_02_FRAMELIMITE - 1)) )
	{
		vDrawPos.x = vPosFrom.x + 4*(vPosTo.x-vPosFrom.x) * ((float)(effect_sort.nCurFrame-44) / (float)FX_BOSSRAID_BULKAN3_02_FRAMELIMITE);
		vDrawPos.y = vPosFrom.y + 4*(vPosTo.y-vPosFrom.y) * ((float)(effect_sort.nCurFrame-44) / (float)FX_BOSSRAID_BULKAN3_02_FRAMELIMITE);
		vDrawPos.z = vPosFrom.z + 4*(vPosTo.z-vPosFrom.z) * ((float)(effect_sort.nCurFrame-44) / (float)FX_BOSSRAID_BULKAN3_02_FRAMELIMITE);
	}
	else
	{
		vDrawPos.x = vPosTo.x;
		vDrawPos.y = vPosTo.y;
		vDrawPos.z = vPosTo.z;
	}


	if( (effect_sort.nCurFrame >= (float)FX_BOSSRAID_BULKAN3_02_FRAMELIMITE)
			&& (effect_sort.nCurFrame < (float)FX_BOSSRAID_BULKAN3_02_FRAMECOUNT) )
	{
		m_scaleRate = (m_scaleRate * (effect_sort.nCurFrame/(float)(FX_BOSSRAID_BULKAN3_02_FRAMECOUNT-FX_BOSSRAID_BULKAN3_02_FRAMELIMITE)));
		m_Alpha = (int)(255 - (255 * (effect_sort.nCurFrame/(float)(FX_BOSSRAID_BULKAN3_02_FRAMECOUNT-FX_BOSSRAID_BULKAN3_02_FRAMELIMITE))));
	}

	// 스케일 값 설정
	float	m_Scale = m_scaleRate * effect_sort.nCurFrame / (float)( FX_BOSSRAID_BULKAN3_02_FRAMELIMITE );

	D3DMATRIX		matS;

	if( m_pLolo )
	{
		//lolo 스케일 조절
		D3DUtil_SetTranslateMatrix( matWorld, vDrawPos.x, vDrawPos.y, vDrawPos.z);
		D3DUtil_SetScaleMatrix(matS, 0.5f + m_Scale, 0.5f + m_Scale, 0.5f + m_Scale);

		D3DMath_MatrixMultiply(matWorld, matS, matWorld);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE, FALSE, NULL, NULL, FALSE, FALSE, m_Alpha);
	}

	if (g_pDSound)
	{
		float x, y, z;
		x = (vDrawPos.x - g_pRoh->m_wx) / g_fDSDistance;
		y = (vDrawPos.y - g_pRoh->m_wy) / g_fDSDistance;
		z = (vDrawPos.z - g_pRoh->m_wz) / g_fDSDistance;
		g_pDSound->Play(g_EffectSound[EFF_SND_TONADO], x, y, z);
	}

	// 3차원 공간상에서 거리를 재야 한다.
	float fDist = Dist3PTo3P(vPosTo.x, vPosTo.y, vPosTo.z, vPosFrom.x, vPosFrom.y, vPosFrom.z) - \
				  Dist3PTo3P(vDrawPos.x, vDrawPos.y, vDrawPos.z, vPosFrom.x, vPosFrom.y, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		// 맞은 놈한테 너 맞았다고 알려준다.
		// 그럼으로써 맞은 놈이 가지고 있는 effect_num이 삭제된다.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		vDrawPos = vPosTo;
		bFinished = TRUE;
	}


	DWORD  dwTextureState;

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	if( smoke )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_TORNADORUIN_SMOKE ] ) ;
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		vViewVec = pCMyApp->GetEyeDirection();
		//지면에서 올라오는 연기
		//float up_height =10.0f;// effect_sort.nCurFrame * 0.5f + 7.0f;
		float up_height =effect_sort.nCurFrame * 0.7f + 5.0f;
		float smoke_fade = 0.0f + ((effect_sort.nCurFrame*0.5f) / (float)FX_HELLCRY_FRAMECOUNT);

		smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		smoke->Scale(2.0f+smoke_fade*10.0f, 2.0f+smoke_fade*15.0f, 2.0f+smoke_fade*10.0f);
		smoke->SetTranslationXYZ(vDrawPos.x , vDrawPos.y + up_height, vDrawPos.z);
		smoke->Render();
	}

//========================================================
	// nate2004 - 6
	// RenderState 되돌리기 대 작전 성공
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  dwDest );
//========================================================

	return bFinished;
}