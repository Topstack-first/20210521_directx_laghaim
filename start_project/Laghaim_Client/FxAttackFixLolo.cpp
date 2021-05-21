#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "FxAttackFixLolo.h"





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAttackFixLolo::CFxAttackFixLolo()
{
	for( int i = 0 ; i < EFF_NUM ; i ++ )
		m_pLolo[ i ] = NULL;

	light = new CRectPlane;
	if( light )
		light->Create( 10.0f, 10.0f, TRUE );
	m_numTLight = 0;
	m_fScale = 0.0f;
	m_nOldFrame = 0;
}

CFxAttackFixLolo::~CFxAttackFixLolo()
{
	DeleteRes();

	SAFE_DELETE(light);
}

void CFxAttackFixLolo::LoadRes()
{

	if( !m_pLolo[SPORE] )
	{
		m_pLolo[SPORE]			= new CLolos;
		m_pLolo[SPORE]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[SPORE]->Read("data/effect/lolos/spore0001.LOL");
	}
	if( !m_pLolo[ELECTRICSHOT] )
	{
		m_pLolo[ELECTRICSHOT]	= new CLolos;
		m_pLolo[ELECTRICSHOT]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[ELECTRICSHOT]->Read("data/effect/lolos/electricshot001.LOL");
	}
	if( !m_pLolo[SNAKEBOSS] )
	{
		m_pLolo[SNAKEBOSS]		= new CLolos;
		m_pLolo[SNAKEBOSS]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[SNAKEBOSS]->Read("data/effect/lolos/SerpentQueen_effect.LOL");
	}
	if( !m_pLolo[SNAKEBOSS_MUL] )
	{
		m_pLolo[SNAKEBOSS_MUL]	= new CLolos;
		m_pLolo[SNAKEBOSS_MUL]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[SNAKEBOSS_MUL]->Read("data/effect/lolos/snakemonster_01.LOL");
	}
	if( !m_pLolo[FROZEN_SHOT] )
	{
		m_pLolo[FROZEN_SHOT]	= new CLolos;
		m_pLolo[FROZEN_SHOT]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[FROZEN_SHOT]->Read("data/effect/lolos/FrozenShot.LOL");
	}
	if( !m_pLolo[LEGKNOCK] )
	{
		m_pLolo[LEGKNOCK]		= new CLolos;
		m_pLolo[LEGKNOCK]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[LEGKNOCK]->Read("data/effect/lolos/legknock_att00-eff.LOL");
	}
	if( !m_pLolo[LEGKNOCK_MUL] )
	{
		m_pLolo[LEGKNOCK_MUL]	= new CLolos;
		m_pLolo[LEGKNOCK_MUL]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[LEGKNOCK_MUL]->Read("data/effect/lolos/legknock_att01-eff.LOL");
	}

}

void CFxAttackFixLolo::DeleteRes()
{
	for( int i = 0 ; i < EFF_NUM ; i ++ )
	{
		SAFE_DELETE(m_pLolo[ i ]);
	}
}

BOOL CFxAttackFixLolo::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matWorld, matRotate, matTrans;
	D3DVECTOR vDrawPos;


	// 타겟 좌표를 구한다.
	vDrawPos = effect_sort.vPos;
	if( effect_sort.Index == 83 )
		vDrawPos.y += 9.0f;

	int nFrame = effect_sort.nCurFrame;

	// 프로즌샷 프레임 강제 조절..
	if( effect_sort.Index == 135 && nFrame >= 11 && timeGetTime()-effect_sort.dwColor <= 3000 ) // 3초간은 깨지지 않도록..
	{
		effect_sort.nCurFrame = nFrame = 22; // 20 프레임에서 멈춘다.
	}

	if( effect_sort.pMobFrom && effect_sort.Index == 4010 ) ///인던1
	{
		if( effect_sort.pMobFrom->GetMotionState() != MOB_ATTACK ) // 애니메이션이 바뀌었다면
			return TRUE; // 지워준다.

		effect_sort.nCurFrame = effect_sort.pMobFrom->GetCurFrame(); // 몹의 프레임에 맞춰준다.(타이밍상 어긋나서...)
		vDrawPos = D3DVECTOR( effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz ); // 최신좌표로 갱신
	}


	if( effect_sort.pMobFrom
			&& ( effect_sort.Index == 4007 || effect_sort.Index == 4010 || effect_sort.Index == 4011 ) ///인던1
	  )	// 방향값 계산
	{
		Nk3DUtil_SetRotateCCWYMatrix(matRotate, effect_sort.pMobFrom->m_dir);
		D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

		if( effect_sort.Index == 4010 ) // 레그넉 크기를 임의로 키웠기 땜에 공격 이펙트도 키워준다.
		{
			float ExceptionScale = GetMobExceptioncale( effect_sort.pMobFrom ); // 예외적인 클라이언트 스케일이 적용되는 경우 스케일 값을 리턴해주는 함수.

			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, ExceptionScale, ExceptionScale, ExceptionScale);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
	}
	else if( effect_sort.Index == 135 ) // 프로즌샷 부위별 붙은 위치 계산.
	{
		if( effect_sort.pNkChaTo )
		{
			D3DMATRIX		matTemp;
			D3DMATRIX		matTemp2;
			D3DUtil_SetIdentityMatrix( matTemp );
			D3DUtil_SetIdentityMatrix( matTemp2 );

			CBiped *pBiped = effect_sort.pNkChaTo->GetBiped();
			int CurFrame = effect_sort.pNkChaTo->Get_m_CurFrame();

			if( pBiped->m_bHalfFrame )
			{
				if( CurFrame % 2 )
				{
					g_pRoh->GetHalfMat( matTemp, pBiped, effect_sort.subNum, CurFrame / 2 ); // 함수만 빌려쓴다.
				}
				else
				{
					matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame / 2];
				}
			}
			else
			{
				matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame];
			}

			vDrawPos = D3DVECTOR( 0.0f,0.0f,0.0f ); // 초기화하고.
			D3DUtil_SetTranslateMatrix(matWorld, vDrawPos);
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);

			Nk3DUtil_SetRotateCCWYMatrix( matTemp2, effect_sort.pNkChaTo->m_dir );
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);

			D3DUtil_SetTranslateMatrix( matTemp2
										, D3DVECTOR(effect_sort.pNkChaTo->m_wx,effect_sort.pNkChaTo->m_wy,effect_sort.pNkChaTo->m_wz) ); // 월드상에서의 위치로 이동시키고...
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);
		}
		else if( effect_sort.pMobTo )
		{
			D3DMATRIX		matTemp;
			D3DMATRIX		matTemp2;
			D3DUtil_SetIdentityMatrix( matTemp );
			D3DUtil_SetIdentityMatrix( matTemp2 );

			CBiped *pBiped = effect_sort.pMobTo->GetBiped( effect_sort.pMobTo->GetMotionState() );
			int CurFrame = effect_sort.pMobTo->GetCurFrame();

			if( pBiped->m_bHalfFrame )
			{
				if( CurFrame % 2 )
				{
					g_pRoh->GetHalfMat( matTemp, pBiped, effect_sort.subNum, CurFrame / 2 ); // 함수만 빌려쓴다.
				}
				else
				{
					matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame / 2];
				}
			}
			else
			{
				matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame];
			}

			vDrawPos = D3DVECTOR( 0.0f,0.0f,0.0f ); // 초기화하고.
			D3DUtil_SetTranslateMatrix(matWorld, vDrawPos);

			// 클라이언트에서 스케일 되는 종류의 몹은 여기서도 스케일 처리를 해줘야한다.
			float ExceptionScale = GetMobExceptioncale( effect_sort.pMobTo ); // 예외적인 클라이언트 스케일이 적용되는 경우 스케일 값을 리턴해주는 함수.
			if( ExceptionScale != 1.0f ) // 1.0f이면 굳이 해줄 필요없으니 스킵.
			{
				// 스케일 메트릭스를 곱하지 말고 이동만을 수동으로 이동시켜준다.
				D3DVECTOR vDrawPos2 = D3DVECTOR( matTemp._41,matTemp._42,matTemp._43 );
				vDrawPos2 *= ExceptionScale;
				matTemp._41 = vDrawPos2.x;
				matTemp._42 = vDrawPos2.y;
				matTemp._43 = vDrawPos2.z;
			}

			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);


			Nk3DUtil_SetRotateCCWYMatrix( matTemp2, effect_sort.pMobTo->m_dir );
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);

			D3DUtil_SetTranslateMatrix( matTemp2
										, D3DVECTOR(effect_sort.pMobTo->m_wx,effect_sort.pMobTo->m_wy,effect_sort.pMobTo->m_wz) ); // 월드상에서의 위치로 이동시키고...
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);
		}
		else
			return TRUE; // 만약 붙어있을 대상이 없어졌으면 바로 지운다.

		//스케일 조절
		D3DMATRIX		matS;
		if( nFrame >= 35 ) // 터질때 스케일 더 크게 조정.(그래픽에서 안해줘서...)
			D3DUtil_SetScaleMatrix(matS, 6.0f, 6.0f, 6.0f);
		else
			D3DUtil_SetScaleMatrix(matS, 3.0f, 3.0f, 3.0f);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}
	else
		D3DUtil_SetTranslateMatrix(matWorld, vDrawPos);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	DWORD dwLighting, dwAmbient, dwZWrite;
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

	if( effect_sort.Index == 4006 && m_pLolo[SPORE] )
		m_pLolo[SPORE]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 83 && m_pLolo[ELECTRICSHOT] )
		m_pLolo[ELECTRICSHOT]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4007 && m_pLolo[SNAKEBOSS] )
		m_pLolo[SNAKEBOSS]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4009 && m_pLolo[SNAKEBOSS_MUL] )
		m_pLolo[SNAKEBOSS_MUL]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 135 && m_pLolo[FROZEN_SHOT] ) // 휴먼 스킬 프로즌 샷
		m_pLolo[FROZEN_SHOT]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4010 && m_pLolo[LEGKNOCK] && effect_sort.pMobFrom ) ///인던1
		m_pLolo[LEGKNOCK]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4011 && m_pLolo[LEGKNOCK_MUL] ) ///인던1
	{
		// 이 공격 발동중엔 카메라를 위아래로 흔들어서 지진 느낌을 준다.
		if( effect_sort.nCurFrame > 30 ) // 땅을 친 후부터 흔들리게...
		{

			D3DVECTOR Eye;
			pCMyApp->GetEyePos( Eye );
			D3DVECTOR At;
			pCMyApp->GetEyeAtPos( At );
			float Adjust = RandomNum( -3.0f, 3.0f );

			Eye.y += Adjust;
			At.y += Adjust;

			pCMyApp->SetCameraInitPos( Eye, At ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.

			g_pRoh->m_wy += Adjust; // 이동중에 흔들리려면 이것도 같이 해줘야한다.(HeroMoving 함수에서 위 카메라 값들을 주인공 wy에 상대적인 좌표로 바꾸어버린다.)(이렇게 바꿔도 wy 는 자동 매프레임 되돌려줌)
		}


		m_pLolo[LEGKNOCK_MUL]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}


	if( effect_sort.Index == 83 )
		RenderLight(effect_sort);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );

	if(  effect_sort.Index == 83 )	// 일렉트릭 샷
	{
		if( nFrame > 12 )
		{
			EffectSort ef_sort;
			ZeroMemory( &ef_sort, sizeof( EffectSort ) );
			ef_sort.nType = FX_MAGICLASER;
			ef_sort.Index = effect_sort.Index;
			ef_sort.vPos = vDrawPos;
			ef_sort.vTargetPos = effect_sort.vTargetPos;
			effect_sort.pNkChaFrom->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			bFinished = true;
		}
	}
	else if( effect_sort.Index == 4006 )	// 스포어
	{
		if( nFrame > 20 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4007 )
	{
		if( nFrame > 8 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4009 )
	{
		if( nFrame > 32 )
			bFinished = true;
	}
	else if( effect_sort.Index == 135 ) // 휴먼 스킬 프로즌 샷
	{
		if( nFrame > 40 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4010 ) // 데카렌 인던 보스 레그녹 일반공격 이펙트. ///인던1
	{
		if( nFrame > 56 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4011 ) // 데카렌 인던 보스 레그녹 다중공격 이펙트. ///인던1
	{
		if( nFrame > 54 )
		{
			// 카메라 화면 y 값을 지진 나기 전껄로 바꿔준다.
			D3DVECTOR Eye;
			pCMyApp->GetEyePos( Eye );
			D3DVECTOR At;
			pCMyApp->GetEyeAtPos( At );
			Eye.y = effect_sort.vSortPos.x;
			At.y = effect_sort.vSortPos.y;
			pCMyApp->SetCameraInitPos( Eye, At ); // 직접 셋팅하기 귀찮아서 pCMyApp 껄 직접 고쳐버림. 씬이 끝나면 기억해둔 이전 카메라 좌표로 다시 바꿔줘야한다.

			bFinished = true;
		}
	}

	return bFinished;
}


void CFxAttackFixLolo::RenderLight(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	BOOL bFinished = FALSE;
	D3DMATRIX matWorld;
	D3DVECTOR vDrawPos;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,	FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	vDrawPos = effect_sort.vTargetPos;

	if( ++m_numTLight >= 4 )
		m_numTLight = 0;

	m_nOldFrame = effect_sort.nCurFrame % 8;
	float fScale = (3.0f * m_nOldFrame) / 7;
	m_fScale = 3.0f - fScale;

	int nFrame = effect_sort.nCurFrame;
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOCKSPEARLIGHT + m_numTLight ] ));

	if( light )
	{
		light->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
		light->SetRotationY( g_PI * 0.5f );
		light->RotateZ( -nFrame * 0.3f );
		light->Scale(m_fScale, 0.0f, m_fScale);
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 9.0f, vDrawPos.z);
		light->Render();

		light->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
		light->SetRotationX( g_PI * 0.5f );
		light->RotateZ( -nFrame * 0.3f );
		light->Scale(m_fScale, 0.0f, m_fScale);
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 9.0f, vDrawPos.z);
		light->Render();

		light->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
		light->SetRotationY( g_PI * 0.5f );
		light->RotateX( -nFrame * 0.3f );
		light->Scale(m_fScale, 0.0f, m_fScale);
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 9.0f, vDrawPos.z);
		light->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}