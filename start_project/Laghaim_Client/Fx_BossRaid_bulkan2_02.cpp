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
#include "Fx_BossRaid_bulkan2_02.h"




int CFxBossRaidBulkan2_02::m_Event[FX_BOSSRAID_BULKAN2_02_MAX_EVENT] =
{
	6, 8, 9, 10, 12, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23, 24
};

CFxBossRaidBulkan2_02::CFxBossRaidBulkan2_02()
{
	m_pLolo = 0;
	m_CirclePlane[0] = 0;
	m_CirclePlane[1] = 0;
}


CFxBossRaidBulkan2_02::~CFxBossRaidBulkan2_02()
{
	DeleteRes();
}

void CFxBossRaidBulkan2_02::LoadRes()
{
	m_CirclePlane[0] = new CRectPlane;
	if( m_CirclePlane[0] )
		m_CirclePlane[0]->Create( 2.0f, 2.0f, FALSE );

	m_CirclePlane[1] = new CRectPlane;
	if( m_CirclePlane[1] )
		m_CirclePlane[1]->Create( 2.0f, 2.0f, FALSE );

	if (!m_pLolo) // 하늘에서 떨어지는 기 형태의 검
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/boss_bul02_sword.LOL", TRUE);
	}



}


void CFxBossRaidBulkan2_02::DeleteRes()
{
	SAFE_DELETE( m_pLolo );
	SAFE_DELETE( m_CirclePlane[0] );
	SAFE_DELETE( m_CirclePlane[1] );
}

bool CFxBossRaidBulkan2_02::Render(EffectSort &effect_sort)
{
	int m_Event[FX_BOSSRAID_BULKAN2_02_MAX_EVENT] =
	{
		0, 0,  6,  8,  9,
		10, 12, 13, 17, 21,
		25, 27, 30, 31, 36,
		37, 49, 31, 49, 49,
		49, 49, 49, 49, 49,
		49, 49, 49, 49, 49,
		49, 49, 49, 49, 49
	};

	if( !GET_D3DDEVICE() )
		return FALSE;

	if(!effect_sort.pMobFrom)
		return FALSE;

	D3DVECTOR Eye;
	pCMyApp->GetEyePos( Eye );
	D3DVECTOR At;
	pCMyApp->GetEyeAtPos( At );


	D3DVECTOR vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR vCirclePos;
	vCirclePos = D3DVECTOR( effect_sort.vCurPos.x, effect_sort.vCurPos.y+1.0f, effect_sort.vCurPos.z);


	//변수 설정
	DWORD srcBlendOld, destBlendOld;
	DWORD alphaBlend, dwLighting, dwAmbient, dwFogEnable;

	if (effect_sort.dwColor == 0 && effect_sort.nCurFrame >= 44)
	{
		effect_sort.dwColor = 1;

		if (g_pDSound)
		{
			float x, y, z;
			x = 0;//(ef_sort.vPos.x - g_pRoh->m_wx) / g_fDSDistance;
			y = 0;//(ef_sort.vPos.y - g_pRoh->m_wy) / g_fDSDistance;
			z = 0;//(ef_sort.vPos.z - g_pRoh->m_wz) / g_fDSDistance;
			g_pDSound->Play(g_EffectSound[EFF_SND_FROZEN_SHOT], x, y, z);
		}
	}

	if (effect_sort.dwColor == 1 && effect_sort.nCurFrame >= 44)
	{
		effect_sort.dwColor = 2;

		if (g_pDSound)
		{
			float x, y, z;
			x = 0;//(ef_sort.vPos.x - g_pRoh->m_wx) / g_fDSDistance;
			y = 0;//(ef_sort.vPos.y - g_pRoh->m_wy) / g_fDSDistance;
			z = 0;//(ef_sort.vPos.z - g_pRoh->m_wz) / g_fDSDistance;
			g_pDSound->Play(g_EffectSound[EFF_SND_STONE_BREAK], x, y, z);
		}
	}

	int nFrame = effect_sort.nCurFrame - 48;
	int nFrame2 = effect_sort.nCurFrame - 30;
	float radius0, radius1;

	if( effect_sort.nCurFrame >= 44 && effect_sort.nCurFrame < 60 )
	{
		float delta = effect_sort.nCurFrame - 43;
		float theta = delta * g_PI * 0.25f;
		float power = 8 / sqrt(delta);
		Eye.y += power * sin(theta);
		pCMyApp->SetCameraInitPos(Eye, At);
	}

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffffff );

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );

	//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	if (nFrame2 > 0)
	{
		radius1 = 120.0f * (float)nFrame2 * 16 / FX_BOSSRAID_BULKAN2_02_FRAMECOUNT;
		if (radius1 > 150.0f) radius1 = 150.0f;

		vCirclePos = D3DVECTOR( effect_sort.vCurPos.x, effect_sort.vCurPos.y+2.0f, effect_sort.vCurPos.z);

		//마법원 그리기
		if( m_CirclePlane[1] )
		{
			float a;
			if (nFrame2 < 40)
			{
				a = 0.60f;
			}
			else if (nFrame2 >= 40 && nFrame2 < 100)
			{
				float t = ((float)80- nFrame2) / 40;
				a = t * 0.60f + (1.0f-t) * 0.2f;
			}
			else
			{
				a = 0.2f;
			}
			m_CirclePlane[1]->SetColor(a, a, a, a);
			m_CirclePlane[1]->SetRotationX( g_PI * 0.5f );
			m_CirclePlane[1]->RotateZ( nFrame2 * nFrame2 * g_PI*0.0003f );
			m_CirclePlane[1]->Scale(radius1, radius1, 10.0f);

			m_CirclePlane[1]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y, vCirclePos.z);
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BOSSRAID_BULKAN2_2_MAGIC ] ));
			m_CirclePlane[1]->Render();
		}
	}


	if (nFrame > 0)
	{
		Deco_Particle* deco_particle = effect_sort.pParticle;

		radius0 = 150.0f * (float)sqrtf(nFrame) * 48 / FX_BOSSRAID_BULKAN2_02_FRAMECOUNT;

		//충격파 그리기
		if( m_CirclePlane[0] )
		{
			float a;
			if (nFrame < 8)
			{
				a = 0.60f;
			}
			else if (nFrame >= 8 && nFrame < 15)
			{
				float t = ((float)15 - nFrame) / 7;
				a = t * 0.60f + (1.0f-t) * 0.0f;
			}
			else
			{
				a = 0.0f;
			}
			m_CirclePlane[0]->SetColor(a, a, a, a);
			m_CirclePlane[0]->SetRotationX( g_PI * 0.5f );
			m_CirclePlane[0]->RotateZ( effect_sort.nCurFrame * g_PI*0.00006f );
			m_CirclePlane[0]->Scale(radius0, radius0, 10.0f);

			m_CirclePlane[0]->SetTranslationXYZ(vCirclePos.x, vCirclePos.y,  vCirclePos.z);
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BOSSRAID_BULKAN2_2_SHOCK ] ));
			m_CirclePlane[0]->Render();
		}

		D3DVECTOR vPos;
		D3DMATRIX matTrans, matView;

		DWORD a, w;
		if (nFrame < 60)
		{
			a = 0xFF * 1.00f;
		}
		else if (nFrame >= 60 && nFrame < 80)
		{
			float t = ((float)80- nFrame) / 20;
			a = 0xFF * t * 1.00f + 0xFF * (1.0f-t) * 0.0f;
		}
		else
		{
			a = 0;
		}
		w = (a << 24) | (a << 16) | (a << 8) | (a);

		int nEvent = nFrame / 4;
		if (nEvent > FX_BOSSRAID_BULKAN2_02_MAX_EVENT)
			nEvent = FX_BOSSRAID_BULKAN2_02_MAX_EVENT;
		for( int i = 0; i < m_Event[nEvent]; ++i)
		{
			int nInterval = nFrame - deco_particle[i].cur_time;
			if (deco_particle[i].cur_Pos.y > -15 )
			{
				deco_particle[i].cur_Pos.y -= 6.0f*nInterval*nInterval + 4.5f * nInterval;
			}
			else if (vPos.y >= -30)
			{
				deco_particle[i].cur_Pos.y -= 0.2f;//(rand()%2);
			}
			else
			{
				deco_particle[i].cur_Pos.y = -30;
			}

			vPos = effect_sort.vCurPos + deco_particle[i].cur_Pos;

			D3DUtil_SetIdentityMatrix( matTrans );
			D3DUtil_SetTranslateMatrix( matTrans, vPos.x, vPos.y, vPos.z);
			pCMyApp->SetViewMatrix(matView);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD,	&matTrans );
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW,	&matView);
			//m_pLolo->Render(GET_D3DDEVICE(), nFrame, FALSE, FALSE, FALSE, &matTrans, &matView, FALSE, FALSE, 0);
			m_pLolo->RenderRaw(GET_D3DDEVICE(), nFrame, FALSE, FALSE, FALSE, &matTrans, &matView, FALSE, FALSE, w);
		}

		for(int  i = 0; i < FX_BOSSRAID_BULKAN2_02_MAX_EVENT; ++i)
		{
			deco_particle[i].cur_time = nFrame;
		}

	}

	//상태 복원
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);

	return TRUE;
}
