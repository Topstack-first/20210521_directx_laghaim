#include "stdafx.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "ImageDefine.h"
#include "Fx_BossRaid_Hybri3_01.h"
#include "Skill.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "Land.h"








CFxBossRaidHybri3_01::CFxBossRaidHybri3_01()
{
	m_CirclePlane = NULL;
	m_bStartFrame = false;
	m_nAlpha = 1.0f;
}

CFxBossRaidHybri3_01::~CFxBossRaidHybri3_01() {}

void CFxBossRaidHybri3_01::LoadRes()
{
	m_CirclePlane = new CRectPlane;
	if( m_CirclePlane )
		m_CirclePlane->Create( 1.0f, 1.0f, FALSE );
}

void CFxBossRaidHybri3_01::DeleteRes()
{
	SAFE_DELETE( m_CirclePlane );
}

bool CFxBossRaidHybri3_01::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !g_pCapsyongTexture )
		return true;

	//변수 설정
	DWORD srcBlendOld, destBlendOld;
	DWORD alphaTest, alphaBlend;
	DWORD dwLighting, dwAmbient, dwFogEnable;

	float radius0, radius1;

	if( effect_sort.Index == EFFECT_BOSSRAID_HYBRI3_01 )
	{
		if( effect_sort.nCurFrame > FX_BOSSRAID_HAYBRI3_01_FRAMECOUNT ) // 이펙트 제거 처리
		{
			m_bStartFrame = false;

			m_nAlpha = 1.0f;

			return TRUE;
		}

		radius0 = 60.0f;

		// 상태값 보존.
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);


		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffffff );

		DWORD  dwTextureState[3];

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

		D3DVECTOR vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		if( effect_sort.nCurFrame > FX_BOSSRAID_HAYBRI3_01_ANIFRAME - 1 )
		{
			D3DMATRIX matRot;
			D3DVECTOR vTmp;

			if( effect_sort.nCurFrame == FX_BOSSRAID_HAYBRI3_01_ANIFRAME )
			{
				D3DUtil_SetIdentityMatrix(matRot);
				Nk3DUtil_SetRotateCCWYMatrix(matRot,effect_sort.pMobFrom->m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, effect_sort.pMobFrom->m_pCapSyong->m_LineVector[0], matRot);

				effect_sort.vPos = D3DVECTOR( vTmp.x + effect_sort.pMobFrom->m_wx, 0.0f, vTmp.z + effect_sort.pMobFrom->m_wz );
				effect_sort.vPos.y = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(effect_sort.vPos.x, effect_sort.vPos.z) + 1.5f;

				radius1 = 30.0f;
			}

			else if( effect_sort.nCurFrame > FX_BOSSRAID_HAYBRI3_01_ANIFRAME && effect_sort.nCurFrame <= FX_BOSSRAID_HAYBRI3_01_ANIFRAME + 4 )
			{
				radius1 = (float)( effect_sort.nCurFrame - FX_BOSSRAID_HAYBRI3_01_ANIFRAME ) * 25;
			}

			else
			{
				radius1 = 0.0f;
			}

			if( effect_sort.nCurFrame <= FX_BOSSRAID_HAYBRI3_01_ANIFRAME )
			{
				m_bStartFrame = false;
				m_nAlpha = 1.0f;
			}
			else if( effect_sort.nCurFrame > FX_BOSSRAID_HAYBRI3_01_ANIFRAME && effect_sort.nCurFrame <= FX_BOSSRAID_HAYBRI3_01_FRAMECOUNT )
			{
				m_bStartFrame = true;
				SetAlpha( m_bStartFrame, effect_sort, m_nAlpha, 3 );
			}

			//바닥 평면 그리기(충격파)
			if( m_CirclePlane )
			{
				float colorValue = 1.0f - ( ( effect_sort.nCurFrame - FX_BOSSRAID_HAYBRI3_01_ANIFRAME ) * 0.1f );

				if( colorValue < 0 )
				{
					colorValue = 0.0f;
				}

				m_CirclePlane->SetColor(colorValue, colorValue, colorValue, 1.0);
				m_CirclePlane->SetRotationX( g_PI * 0.5f );
				m_CirclePlane->Scale(radius1, radius1, 1.0f);

				m_CirclePlane->SetTranslationXYZ(effect_sort.vPos.x, effect_sort.vPos.y,  effect_sort.vPos.z);
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BOSSRAID_HYBRI3_01 ] ));
				m_CirclePlane->Render();
			}
		}

		//상태 복원
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	}

	return false;
}

void CFxBossRaidHybri3_01::SetAlpha( bool startFrame, EffectSort &ef_sort, float &nAlpha, int time )
{
	if( startFrame )
	{
		static DWORD startTime = timeGetTime();
		DWORD  curTime = timeGetTime();

		if( startFrame )
		{
			if( ( curTime - startTime ) > time )
			{
				nAlpha -= (float)( 0.008f * ef_sort.nCurFrame );

				startTime = curTime;
			}
		}
	}
}
