#include "stdafx.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "ImageDefine.h"
#include "Fx_BossRaid_Human3_02.h"
#include "Skill.h"
#include "NkMob.h"






CFxBossRaidHuman3_02::CFxBossRaidHuman3_02()
{
	m_CirclePlane[0] = NULL;
	m_CirclePlane[1] = NULL;
	m_bStartFrame = false;
	m_nAlpha = 1.0f;
}

CFxBossRaidHuman3_02::~CFxBossRaidHuman3_02() {}

void CFxBossRaidHuman3_02::LoadRes()
{
	m_CirclePlane[0] = new CRectPlane;
	if( m_CirclePlane[0] )
		m_CirclePlane[0]->Create( 1.0f, 1.0f, FALSE );

	m_CirclePlane[1] = new CRectPlane;
	if( m_CirclePlane[1] )
		m_CirclePlane[1]->Create( 2.0f, 2.0f, FALSE );
}

void CFxBossRaidHuman3_02::DeleteRes()
{
	SAFE_DELETE( m_CirclePlane[0] );
	SAFE_DELETE( m_CirclePlane[1] );
}

bool CFxBossRaidHuman3_02::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !g_pCapsyongTexture )
		return true;

	//변수 설정
	DWORD srcBlendOld, destBlendOld;
	DWORD alphaTest, alphaBlend;
	DWORD dwLighting, dwAmbient, dwFogEnable;

	float radius0, radius1;

	if( effect_sort.Index == EFFECT_BOSSRAID_HUMAN3_02 )
	{
		if( effect_sort.nCurFrame == 30 )
		{
			pCMyApp->GetEyePos( m_oldEye );
			pCMyApp->GetEyeAtPos( m_oldAt );
		}

		if( effect_sort.nCurFrame > FX_BOSSRAID_HUMAN3_02_FRAMECOUNT ) // 이펙트 제거 처리
		{
			m_bStartFrame = false;

			return TRUE;
		}

		radius0 = 120.0f;

		// 상태값 보존.
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld);
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffffff );

		DWORD  dwTextureState[3];

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		D3DVECTOR vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		if( effect_sort.nCurFrame > 30 )
		{
			D3DVECTOR vTemp, vDesc, vAdj;
			D3DMATRIX matRot;

			if( effect_sort.nCurFrame == 31 )
			{
				vAdj = D3DVECTOR( effect_sort.pMobFrom->GetBiped(effect_sort.pMobFrom->GetMotionState())->m_AniMat[24].m_Mats[effect_sort.pMobFrom->GetCurFrame()]._41,
								  effect_sort.pMobFrom->GetBiped(effect_sort.pMobFrom->GetMotionState())->m_AniMat[24].m_Mats[effect_sort.pMobFrom->GetCurFrame()]._42,
								  effect_sort.pMobFrom->GetBiped(effect_sort.pMobFrom->GetMotionState())->m_AniMat[24].m_Mats[effect_sort.pMobFrom->GetCurFrame()]._43 );

				vTemp = D3DVECTOR( effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz );

				D3DUtil_SetIdentityMatrix(matRot);
				D3DUtil_SetTranslateMatrix( matRot, vAdj );
				Nk3DUtil_SetRotateCCWYMatrix( matRot, effect_sort.pMobFrom->m_dir );
				D3DMath_VectorMatrixMultiply(vDesc, vAdj, matRot);

				effect_sort.vPos = D3DVECTOR( vTemp.x + vDesc.x, vTemp.y + 1.0f, vTemp.z + vDesc.z );
			}

			radius1 = 2.0f * (float)( effect_sort.nCurFrame - 29.0f ) * 70;

			if( effect_sort.nCurFrame < 70 )
			{
				m_bStartFrame = false;
				m_nAlpha = 1.0f;
			}
			else if( effect_sort.nCurFrame > 70 && effect_sort.nCurFrame <= FX_BOSSRAID_HUMAN3_02_FRAMECOUNT )
			{
				m_bStartFrame = true;
				SetAlpha( m_bStartFrame, effect_sort, m_nAlpha, 3 );
			}

			//바닥 평면 그리기(충격파)
			if( m_CirclePlane[0] )
			{
				float colorValue = 1.0f - ( ( effect_sort.nCurFrame - 31 ) * 0.1f );

				if( colorValue < 0 )
				{
					colorValue = 0.0f;
				}

				m_CirclePlane[0]->SetColor(colorValue, colorValue, colorValue, 1.0f);
				m_CirclePlane[0]->SetRotationX( g_PI * 0.5f );
				m_CirclePlane[0]->Scale(radius1, radius1, 10.0f);

				m_CirclePlane[0]->SetTranslationXYZ(effect_sort.vPos.x, effect_sort.vPos.y,  effect_sort.vPos.z);
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BOSSRAID_HUMAN3_02_SHOCK ] ));
				m_CirclePlane[0]->Render();
			}

			//바닥 꺼지는 효과
			if( m_CirclePlane[1] )
			{
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

				m_CirclePlane[1]->SetColor(1.0f, 1.0f, 1.0f, m_nAlpha);
				m_CirclePlane[1]->SetRotationX( g_PI * 0.5f );
				m_CirclePlane[1]->Scale(radius0, radius0, 1.0f);

				m_CirclePlane[1]->SetTranslationXYZ(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[EFF_FX_BOSSRAID_HUMAN3_02] ));
				m_CirclePlane[1]->Render();
			}

			// [12/13/2007 반재승] 카메라 쉐이킹 해주기.
			if( effect_sort.nCurFrame <= 41 )
			{
				CameraShakeFrame( effect_sort.nCurFrame );
			}

			else if( effect_sort.nCurFrame > 41 )
			{
				D3DVECTOR Eye;
				pCMyApp->GetEyePos( Eye );
				D3DVECTOR At;
				pCMyApp->GetEyeAtPos( At );

				Eye.y = m_oldEye.y;
				At.y = m_oldAt.y;

				pCMyApp->SetCameraInitPos( Eye, At );
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

void CFxBossRaidHuman3_02::CameraShakeFrame( int currentFrame )
{
	D3DVECTOR Eye;
	pCMyApp->GetEyePos( Eye );
	D3DVECTOR At;
	pCMyApp->GetEyeAtPos( At );

	if( currentFrame > 30 ) // 땅을 친 후부터 흔들리게...
	{
		float Adjust = RandomNum( -3.0f, 3.0f );

		Eye.y += Adjust;
		At.y += Adjust;

		pCMyApp->SetCameraInitPos( Eye, At );

		g_pRoh->m_wy += Adjust; // 이동중에 흔들리려면 이것도 같이 해줘야한다.(HeroMoving 함수에서 위 카메라 값들을 주인공 wy에 상대적인 좌표로 바꾸어버린다.)(이렇게 바꿔도 wy 는 자동 매프레임 되돌려줌)
	}
}


void CFxBossRaidHuman3_02::SetAlpha( bool startFrame, EffectSort &ef_sort, float &nAlpha, int time )
{
	if( startFrame )
	{
		static DWORD startTime = timeGetTime();
		DWORD  curTime = timeGetTime();

		if( startFrame )
		{
			if( ( curTime - startTime ) > time )
			{
				nAlpha -= (float)( 0.0002f * ef_sort.nCurFrame );

				startTime = curTime;
			}
		}
	}
}
