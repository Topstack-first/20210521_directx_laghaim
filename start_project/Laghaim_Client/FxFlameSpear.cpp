#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>
#include <assert.h>
#include <time.h>
#include "FxSet.h"
#include "ddutil.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "GUtil.h"
#include "SFXBaseShape.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "land.h"
#include "Skill.h"

#include "FxFireParticle.h"
#include "FxFlameSpear.h"

// 진행 페이즈
#define PHASE_SUMMONING 0 // 소환되는 단계
#define PHASE_THROWING	1 // 날라가는 단계
#define PHASE_EXPLOSION 2 // 폭발하는 단계







extern CLolos	*g_pFairyProtect; // 방어막 이펙트 롤로 ///페어리

CFxFlameSpear::CFxFlameSpear()
	: m_pMagicCircle2(NULL)
{
	m_pMagicCircle1 = NULL;
	m_pSpear = NULL;
	m_pSpearTail = NULL;
//	m_pSpearAura = NULL;
	m_pRollingEff = NULL;
}

CFxFlameSpear::~CFxFlameSpear()
{
	DeleteRes();
}

void CFxFlameSpear::DeleteRes()
{
	SAFE_DELETE( m_pRollingEff );
	SAFE_DELETE( m_pSpearTail );
	SAFE_DELETE( m_pSpear );
	SAFE_DELETE( m_pMagicCircle1 );
}

void CFxFlameSpear::LoadRes()
{
	DeleteRes();

	m_pMagicCircle1 = new CRectPlane;
	if( m_pMagicCircle1 )
		m_pMagicCircle1->Create( 25.0f, 25.0f, FALSE );

	m_pRollingEff = new CCylinderPlane;
	if( m_pRollingEff )
		m_pRollingEff->Create( 2.0f, 3.0f, 50.0f, 30.0f, 0.0f, TRUE, 2.0f);


	if (!m_pSpear)
	{
		m_pSpear = new CLolos;
		m_pSpear->SetIndexedTexture( g_pCapsyongTexture );
		m_pSpear->Read("data/effect/lolos/FlameSpear.LOL");
	}

	if (!m_pSpearTail)
	{
		m_pSpearTail = new CLolos;
		m_pSpearTail->SetIndexedTexture( g_pCapsyongTexture );
		m_pSpearTail->Read("data/effect/lolos/FlameSpearTail.LOL");
	}


}


BOOL CFxFlameSpear::Render( EffectSort &effect_sort )
{
	if( !effect_sort.pNkChaFrom || (!effect_sort.pNkChaTo&&!effect_sort.pMobTo) ) // 혹시 타겟중 하나가 없어졌다면...
		return TRUE;

	DWORD dwLighting, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );



	D3DVECTOR TempVec, TempVec2, TempVec3;
	D3DMATRIX matWorld;
	D3DMATRIX matTrans;
	D3DMATRIX matRotate, matRotate2;

	D3DXQUATERNION DestQuat; // 대상을 향하는 회전.
	D3DXQUATERNION TempQuat;

	float fTargetDist = 0.0f;

	// 향하는 방향을 타겟을 항상 따라가게 해준다.
	if( effect_sort.num != PHASE_EXPLOSION && effect_sort.nCurFrame <= 35 ) // 이 프레임 이후엔 방향을 계산하지 않는다. 최종 각도록 고정이다.
	{
		if( effect_sort.pNkChaTo )
		{
			// 타겟과 이동 방향을 갱신.
			effect_sort.vTargetPos = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+0.0f, effect_sort.pNkChaTo->m_wz ); // 최종 목표가 되는 위치.(여기를 기준으로 타겟을 산정한다.) // 약간 위로 타겟팅한다.
			effect_sort.vSortPos = effect_sort.vTargetPos - effect_sort.vCurPos; // 실제 타겟 위치가 아니라 창이 향하는 방향 벡터로 사용.
			fTargetDist = Magnitude( effect_sort.vSortPos ); // 타겟까지의 남은 거리를 계산.
			effect_sort.vSortPos = Normalize( effect_sort.vSortPos ); // 실제 타겟 위치가 아니라 창이 향하는 방향 벡터로 사용.
		}
		else if( effect_sort.pMobTo )
		{
			// 타겟과 이동 방향을 갱신.
			effect_sort.vTargetPos = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+0.0f, effect_sort.pMobTo->m_wz ); // 최종 목표가 되는 위치.(여기를 기준으로 타겟을 산정한다.) // 약간 위로 타겟팅한다.
			effect_sort.vSortPos = effect_sort.vTargetPos - effect_sort.vCurPos; // 실제 타겟 위치가 아니라 창이 향하는 방향 벡터로 사용.
			fTargetDist = Magnitude( effect_sort.vSortPos ); // 타겟까지의 남은 거리를 계산.
			effect_sort.vSortPos = Normalize( effect_sort.vSortPos ); // 실제 타겟 위치가 아니라 창이 향하는 방향 벡터로 사용.
		}
	}


	float fLoloRotateY = atan2f(effect_sort.vSortPos.z, effect_sort.vSortPos.x); // y 로테이트 라디언 값.
	float fLoloRotateX = acos( DotProduct( D3DVECTOR(effect_sort.vSortPos.x,effect_sort.vSortPos.y,effect_sort.vSortPos.z), D3DVECTOR(effect_sort.vSortPos.x,0.0f,effect_sort.vSortPos.z) ) ); // x 로테이트 라디언 값.
	float fLoloRotateZ = (float)(effect_sort.nCurFrame)*0.5f; // z 로테이트 라디언 값. 날아갈때만 돌아간다.

	float rectwidth;

	switch( effect_sort.num ) // Phase 로 쓰인다.
	{
	case PHASE_SUMMONING: // 소환되는 단계

		if( effect_sort.nCurFrame <= 25 ) // 마법진이 이동하는 단계
		{
			// 임시로 크기조절.
			rectwidth = (float)effect_sort.nCurFrame*0.3f;
			m_pMagicCircle1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 0.0f, 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, 0.0f, 0.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 1.0f , 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, 1.0f , 0.0f )  );

			// 마법진 회전은 여기서 셋팅.
			m_pMagicCircle1->SetRotationY( 0.0f ); // 초기화를 위해 해줌.
			m_pMagicCircle1->RotateAxis( D3DVECTOR(0.0f,1.0f,0.0f), fLoloRotateY+1.57f ); // 90도 돌려서..
			m_pMagicCircle1->RotateAxis( D3DVECTOR(0.0f,0.0f,1.0f), (float)effect_sort.nCurFrame*0.7f ); // 이 페이즈에선 마법진을 회전시킨다.

			// 순수하게 x,z평면의 방향벡터를 만들어서 마법진 및 불꽃의 방향을 정해준다.( vSortPos로 부터 얻어오면 거리에따라 길이가 달라져 버린다.
			TempVec3 = Normalize( D3DVECTOR(effect_sort.vSortPos.x,0.0f,effect_sort.vSortPos.z) );

			TempVec = effect_sort.vCurPos + ( TempVec3 * ((float)effect_sort.nCurFrame*0.85f) );
			TempVec2 = effect_sort.vCurPos + ( TempVec3 * ((float)effect_sort.nCurFrame*(-0.85f)) ); // 위 꺼랑 반대방향으로.

			if( effect_sort.nCurFrame <= 22 ) // 불꽃은 약간 일찍 그만나오게..
			{
				if( effect_sort.subNum != effect_sort.nCurFrame ) // 1프레임에 중복되진 않도록..
				{
					// 첫번째 마법진 불꽃 이펙트 추가.
					InsertFire( effect_sort, TempVec ); // 위치는 마법진 위치에...
					// 두번째 마법진 불꽃 이펙트 추가.
					InsertFire( effect_sort, TempVec2 ); // 위치는 마법진 위치에...
				}
			}
		}
		else // 마법진이 이동하진 않는단계..
		{
			rectwidth = 9.0f;
			m_pMagicCircle1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 0.0f, 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, 0.0f, 0.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 1.0f , 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, 1.0f , 0.0f )  );

			// 마법진 회전은 여기서 셋팅.
			m_pMagicCircle1->SetRotationY( 0.0f ); // 초기화를 위해 해줌.
			m_pMagicCircle1->RotateAxis( D3DVECTOR(0.0f,1.0f,0.0f), fLoloRotateY+1.57f ); // 90도 돌려서..

			// 순수하게 x,z평면의 방향벡터를 만들어서 마법진 및 불꽃의 방향을 정해준다.( vSortPos로 부터 얻어오면 거리에따라 길이가 달라져 버린다.
			TempVec3 = Normalize( D3DVECTOR(effect_sort.vSortPos.x,0.0f,effect_sort.vSortPos.z) );

			TempVec = effect_sort.vCurPos + ( TempVec3 * (21.25f) );
			TempVec2 = effect_sort.vCurPos + ( TempVec3 * (-21.25f) ); // 위 꺼랑 반대방향으로.
		}

		// 마법진 출력. 소환되는 단계에선 내내 마버진을 그려준다.
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

		// 마법진은 두개 그린다.
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FLAMESPEAR_MC ] ) ); // 텍스쳐는 공통사용.

		// 첫번째 마법진...
		m_pMagicCircle1->SetTranslationXYZ( TempVec.x, TempVec.y, TempVec.z );
		m_pMagicCircle1->Render();

		// 두번째 마법진...
		m_pMagicCircle1->SetTranslationXYZ( TempVec2.x, TempVec2.y, TempVec2.z );
		m_pMagicCircle1->Render();

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);


		if( effect_sort.nCurFrame > 20 ) // 일정 프레임 이후부터만 창을 출력
		{
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

			D3DMath_QuaternionFromAngles( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
										  , 0.0f, fLoloRotateY, 0.0f );

			if( effect_sort.nCurFrame > 25 ) // 이 이후부터는 창이 목표를 향해 서서히 회전하게 해준다.
			{
				D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
											  , fLoloRotateX, fLoloRotateY, 0.0f );

				D3DMath_QuaternionSlerp( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
										 , TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
										 , DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
										 , (float)(effect_sort.nCurFrame-25)*0.07f ); // 쿼터니온을 프레임에 맞춰 보간해간다.
				/*
								// 이 단계부터 Roll 회전을 먹인다.
								D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
									, 0.0f, 0.0f, fLoloRotateZ ); // Roll을 위에다 먹이면 원하던 결과가 안나오더라. 따로 Roll 회전을 만들어서 쿼터니온끼리 합성시킨다.

								D3DMath_QuaternionMultiply( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
									, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
									, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w );
				*/
				D3DMath_MatrixFromQuaternion( matRotate, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w );
			}
			else
			{
				D3DMath_MatrixFromQuaternion( matRotate, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w );
			}

			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos );
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

			m_pSpear->Render(GET_D3DDEVICE(), 0, FALSE, FALSE); // 애니메이트 안 시킴에 유의
		}

		if( effect_sort.nCurFrame > 39 )
			effect_sort.num = PHASE_THROWING; // 다음 페이즈로 진행

		break;

	case PHASE_THROWING: // 날라가는 단계

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// 이 단계까지는 Roll 회전을 먹인다.
		D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
									  , fLoloRotateX, fLoloRotateY, 0.0f );
		/*
				D3DMath_QuaternionFromAngles( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
					, 0.0f, 0.0f, fLoloRotateZ ); // Roll을 위에다 먹이면 원하던 결과가 안나오더라. 따로 Roll 회전을 만들어서 쿼터니온끼리 합성시킨다.

				D3DMath_QuaternionMultiply( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
					, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
					, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w );
		*/
		D3DMath_MatrixFromQuaternion( matRotate, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w );

		fTargetDist = (float)(effect_sort.nCurFrame-35) * 2.5f; // 그냥 가속도.

		effect_sort.vCurPos += effect_sort.vSortPos * fTargetDist; // 좌표를 이동.

		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos );
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_pSpear->Render( GET_D3DDEVICE(), 0 );


		// 날라갈때 뒤에 효과선(꼬리) 이펙트를 출력.
		m_pSpearTail->Render( GET_D3DDEVICE(), /*(effect_sort.nCurFrame-35)*/5 ); // 이녀석은 애니메이트 시킨다.


		/*
				// 날라갈때의 이펙트를 추가.
				TempVec = m_pSpear->m_LineVector0[0];
				D3DMath_VectorMatrixMultiply( TempVec, TempVec, matWorld );
				InsertFire2( effect_sort, TempVec ); // 위치는 마법진 위치에...
		*/
		/*
				if( g_pCapsyongTexture )
					GET_D3DDEVICE()->SetTexture(0, /*g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FLAME_TAIL ] )/ NULL );
		//			GET_D3DDEVICE()->SetTexture(0, NULL );

				if( m_pRollingEff )
				{
					TempVec = effect_sort.vCurPos - ( effect_sort.vSortPos*10.0f );

					m_pRollingEff->SetColor(1.0f, 0.5f, 0.5f, 1.0f);
		//			m_pRollingEff->SetRotationY( g_PI );
		//			m_pRollingEff->RotateY( m_fRotation * 0.1f );
		//			m_pRollingEff->Scale( 1.0f, m_fScale * 300.0f, 1.0f );

		//			m_pRollingEff->ReCreate( 2.0f, 3.0f, 150.0f, 20.0f, 0.0f, 2.0f);
					m_pRollingEff->SetRotationX( fLoloRotateX );
					m_pRollingEff->SetRotationY( fLoloRotateY );
		//			m_pRollingEff->SetRotation( matRotate );
		//			m_pRollingEff->RotateY( 1.57f );
					m_pRollingEff->SetTranslationXYZ( effect_sort.vCurPos.x,effect_sort.vCurPos.y, effect_sort.vCurPos.z );
					m_pRollingEff->Render( GET_D3DDEVICE() );
				}
		*/



		// 페이즈 진행 검사.
		if( effect_sort.vCurPos.y < pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x,effect_sort.vCurPos.z) ) // 창이 땅에 충돌하면 폭발시킨다.
		{
			effect_sort.nCurFrame = 0; // 폭발 프레임을 제어하기 위해 초기화.
			effect_sort.num = PHASE_EXPLOSION; // 다음 페이즈로 진행

			// 만약 자기 자신의 플레임 스피어라면 타겟 패킷을 여기서 보낸다.
			if( effect_sort.pNkChaFrom == g_pRoh )
			{
				if( effect_sort.pNkChaTo )
					pCMyApp->SetHeroAttGoCha( effect_sort.pNkChaTo );
				else if( effect_sort.pMobTo )
					pCMyApp->SetHeroAttGoMob( effect_sort.pMobTo );

				pCMyApp->AttackMsgSend( SKILL_K_FLAMESPEAR_TARGET );
			}
		}

		break;

	case PHASE_EXPLOSION: // 폭발하는 단계
		/*
				if( effect_sort.nCurFrame < 10 ) // 일정시간은 창이 보이도록
				{
					GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

					D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
						, fLoloRotateX, fLoloRotateY, 0.0f );

					D3DMath_MatrixFromQuaternion( matRotate, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w );

					D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos );
					D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

					m_pSpear->Render( GET_D3DDEVICE(), 0 ); // 애니메이트 시킨다.
				}

				// 폭파 효과를 호출한다.
				if( effect_sort.pNkChaTo )
				{
					// 타겟과 이동 방향을 갱신.
					TempVec = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+3.0f, effect_sort.pNkChaTo->m_wz ); // 최종 목표가 되는 위치.(여기를 기준으로 타겟을 산정한다.) // 약간 위로 타겟팅한다.
				}
				else if( effect_sort.pMobTo )
				{
					// 타겟과 이동 방향을 갱신.
					TempVec = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+3.0f, effect_sort.pMobTo->m_wz ); // 최종 목표가 되는 위치.(여기를 기준으로 타겟을 산정한다.) // 약간 위로 타겟팅한다.
				}
		*/

		if( effect_sort.nCurFrame > 0 && effect_sort.subNum != effect_sort.nCurFrame ) // 1프레임에 중복되진 않도록..
		{
			EffectSort ef_sort;
			ef_sort.nType = FX_METEORITEBOOM;
			ef_sort.Index = 1; // 바닥 갈라지는거 안표시하도록...

			ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-2.0f, 2.0f)*(float)effect_sort.nCurFrame, effect_sort.vCurPos.y+RandomNum(0.0f, 3.5f)*(float)effect_sort.nCurFrame, effect_sort.vCurPos.z+RandomNum(-2.0f, 2.0f)*(float)effect_sort.nCurFrame );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
//			ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-5.0f, 5.0f), effect_sort.vCurPos.y+RandomNum(0.0f, 5.0f), effect_sort.vCurPos.z+RandomNum(-5.0f, 5.0f) );
//			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);


			// 한 루프당 두개씩 추가시킨다.(타겟 좌표 근처에 하나랑 좀더 떨어지게 하나.
//			ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-5.0f, 5.0f), effect_sort.vCurPos.y+RandomNum(0.0f, 5.0f), effect_sort.vCurPos.z+RandomNum(-5.0f, 5.0f) );
//			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

			if( effect_sort.nCurFrame > 10 )
			{
				ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-35.0f, 35.0f), effect_sort.vCurPos.y+RandomNum(0.0f, 20.0f), effect_sort.vCurPos.z+RandomNum(-35.0f, 35.0f) );
				pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			}

		}

		if( effect_sort.nCurFrame > 15 )
			return TRUE;

		break;
	}



	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	effect_sort.subNum = effect_sort.nCurFrame; // subNum 을 PrevFrame으로 사용.

	return FALSE;
}


void CFxFlameSpear::InsertFire( EffectSort &effect_sort, D3DVECTOR vDrawVec ) // 이펙트넣는게 좀 길어서 함수로 분리.
{
	Deco_Particle *pFire_Particle = NULL;

	pFire_Particle = new Deco_Particle[MAX_FIRE_PARTICLE]; // 이 숫자를 멋대로 늘리면 안된다. // 이 녀석의 메모리 해제 타이밍은 CFxFireParticle 의 Render에서 제거시다.

	if( !pFire_Particle )
		return;

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ ) // 초기화. 특별히 값을 복잡하게 셋팅할 필요는 없다.
	{
		pFire_Particle[i].start_time = 0;
		pFire_Particle[i].life_time = 0;
		pFire_Particle[i].cur_time = 0;
		pFire_Particle[i].scale = 0.0f; // 이 인자는 디폴트 값이 다른 기존 CFxFireParticle 과 다르다. 타입이 생성->소멸 타입이기 때문에.
		pFire_Particle[i].fade = 1.0f;

		pFire_Particle[i].red = 1.0f;
		pFire_Particle[i].green = 1.0f;
		pFire_Particle[i].blue = 1.0f;

		pFire_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

		pFire_Particle[i].texIndex = 0;
		pFire_Particle[i].texFrame = 0;
	}

	// 실제 이펙트 추가
	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	ef_sort.nType = FX_FIREPARTICLE;
	ef_sort.effects = pFire_Particle;
	ef_sort.num = REDFIRE_ONETIME; // 타입
	ef_sort.vPos = vDrawVec; // 위치
	ef_sort.Index = 5; // 불꽃 크기.
	ef_sort.dwColor = 1; // 현재 라이프 초기화. (1부터 시작하도록)
	ef_sort.subNum = 25; // 라이프(프레임단위)
	ef_sort.vSortPos.x = 0.3f; // 초반 커지는 속도
	ef_sort.vSortPos.y = 0.1f; // 후반 꺼지는 속도

	pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
}

void CFxFlameSpear::InsertFire2( EffectSort &effect_sort, D3DVECTOR vDrawVec ) // 이펙트넣는게 좀 길어서 함수로 분리.
{
	Deco_Particle *pFire_Particle = NULL;

	pFire_Particle = new Deco_Particle[MAX_FIRE_PARTICLE]; // 이 숫자를 멋대로 늘리면 안된다. // 이 녀석의 메모리 해제 타이밍은 CFxFireParticle 의 Render에서 제거시다.

	if( !pFire_Particle )
		return;

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ ) // 초기화. 특별히 값을 복잡하게 셋팅할 필요는 없다.
	{
		pFire_Particle[i].start_time = 0;
		pFire_Particle[i].life_time = 0;
		pFire_Particle[i].cur_time = 0;
		pFire_Particle[i].scale = 0.0f; // 이 인자는 디폴트 값이 다른 기존 CFxFireParticle 과 다르다. 타입이 생성->소멸 타입이기 때문에.
		pFire_Particle[i].fade = 1.0f;

		pFire_Particle[i].red = 1.0f;
		pFire_Particle[i].green = 1.0f;
		pFire_Particle[i].blue = 1.0f;

		pFire_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

		pFire_Particle[i].texIndex = 0;
		pFire_Particle[i].texFrame = 0;
	}

	// 실제 이펙트 추가
	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	ef_sort.nType = FX_FIREPARTICLE;
	ef_sort.effects = pFire_Particle;
	ef_sort.num = REDFIRE_ONETIME; // 타입
	ef_sort.vPos = vDrawVec; // 위치
	ef_sort.Index = 1; // 불꽃 크기.
	ef_sort.dwColor = 1; // 현재 라이프 초기화. (1부터 시작하도록)
	ef_sort.subNum = 5; // 라이프(프레임단위)
	ef_sort.vSortPos.x = 1.3f; // 초반 커지는 속도
	ef_sort.vSortPos.y = 1.1f; // 후반 꺼지는 속도

	pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
}