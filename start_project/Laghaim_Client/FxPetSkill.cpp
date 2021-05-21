#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>
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

#include "FxPetSkill.h"






CFxPetSkill::CFxPetSkill()
{
	m_pBillBoard = NULL;
	m_pPoison = NULL;
	m_pParticleInfoArray = NULL;
}

CFxPetSkill::~CFxPetSkill()
{
	DeleteRes();
}

void CFxPetSkill::DeleteRes()
{
	SAFE_DELETE( m_pBillBoard );
	SAFE_DELETE( m_pPoison );
}

void CFxPetSkill::LoadRes()
{
	DeleteRes();

	m_pBillBoard = new CRectPlane;
	if( m_pBillBoard )
		m_pBillBoard->Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)

	// 방어막 이펙트 롤로 ///페어리

	g_pCapsyongTexture->SetTexturePath( "data/effect/textr/" );

	CLolos *pLolo;
	pLolo = new CLolos(g_pCapsyongTexture);
	pLolo->Read("data/effect/lolos/pet_poison.LOL");
	m_pPoison = pLolo;




}

void CFxPetSkill::FrameMove( EffectSort &effect_sort, int nAddFrame ) // 페어리의 움직임을 구현하자.
{
	int TotalFrame = 0; // 각 타입,애니메이션 별 총 프레임.
	float speed = 0.0f; // 움직이는 속도.
	D3DVECTOR vTempVec;
	D3DVECTOR vTempVec2;
	D3DVECTOR vTempVec3;
	float fTempFloat = 0.0f;
	float MasterDist = 0.0f; // 마스터와의 거리 값.
	D3DMATRIX TempMatrix;

	D3DVECTOR	vViewVec, vRightVec;
	vViewVec = pCMyApp->GetEyeDirection();
	vRightVec = CrossProduct( D3DVECTOR(0.0f,1.0f,0.0f),vViewVec ); // 뷰로 부터 라이트를 구해놓는다.
	vRightVec = Normalize( vRightVec );

	// 프레임 갱신
	effect_sort.nCurFrame += nAddFrame;

	// 타입별로 다른 AI를 지정한다.
	switch( effect_sort.Index ) // effect_sort 의 Index를 타입으로 사용한다.
	{
	case PET_SKILL_EFFECT_TYPE1: // 대상에게 조그만 광자가 날라가서 부딪치는 이펙트.

		if( effect_sort.nCurFrame < 0 ) // -프레임일땐 무시
			return;

		if( effect_sort.subNum == 0 ) // 날아가는 상태
		{
			TotalFrame = PET_SKILL_EFFECT_TYPE1_FRAME;

			// 이동방향을 구한다. 타겟이 있으면 타겟에 맞춰서...
			if( effect_sort.pNkChaTo )
				vTempVec2 = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+13.0f, effect_sort.pNkChaTo->m_wz );
			else if( effect_sort.pMobTo )
				vTempVec2 = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+13.0f, effect_sort.pMobTo->m_wz );
			else
				vTempVec2 = effect_sort.vTargetPos;

			vTempVec = vTempVec2 - effect_sort.vCurPos;
			fTempFloat = Magnitude( vTempVec ); // 남은 거리를 구해서.
			effect_sort.vSortPos.y = fTempFloat / (PET_SKILL_EFFECT_TYPE1_FRAME-effect_sort.nCurFrame); // 일정 프레임안에 이동하도록 속도를 바꿔준다.
			vTempVec = Normalize(vTempVec); // 일단 단위벡터로 뽑아서...

			// 이동.
//			effect_sort.vCurPos = effect_sort.vPos + (vTempVec*(float)effect_sort.nCurFrame*effect_sort.vSortPos.y); // 일정 프레임안에 이동하도록...
			effect_sort.vCurPos = effect_sort.vCurPos + (vTempVec*(float)nAddFrame*effect_sort.vSortPos.y); // 일정 프레임안에 이동하도록...
			/*
						BOOL bFinish = FALSE;

						vTempVec3 = effect_sort.vTargetPos - effect_sort.vPos;
						vTempVec3 = Normalize(vTempVec); // 일단 단위벡터로 뽑아서...
			*/
			if( effect_sort.nCurFrame >= TotalFrame )
//			if( bFinish )
			{
				effect_sort.subNum = 1; // 상태를 폭발로 바꿔준다.
				effect_sort.nCurFrame = 0;

				if( m_pParticleInfoArray )
				{
					for( int i = 0 ; i < MAX_FXPETSKILL_PARTICLE ; ++i )
					{
						m_pParticleInfoArray[i].life_time = 5; // curTailPosNum을 라이프로 사용.
					}
				}
			}
		}
		else if( effect_sort.subNum == 1 ) // 폭발하는(커지는) 상태
		{
			TotalFrame = 20;

			// 커지는 상태라도 위치는 따라가도록.
			if( effect_sort.pNkChaTo )
				effect_sort.vCurPos = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+13.0f, effect_sort.pNkChaTo->m_wz );
			else if( effect_sort.pMobTo )
				effect_sort.vCurPos = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+13.0f, effect_sort.pMobTo->m_wz );
			else
				effect_sort.vCurPos = effect_sort.vTargetPos;


			if( effect_sort.nCurFrame >= TotalFrame )
			{
				effect_sort.subNum = 2; // 없애기...
			}
		}

		// 파티클 갱신.
		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

		if( m_pParticleInfoArray )
		{
			for( int i = 0 ; i < MAX_FXPETSKILL_PARTICLE ; ++i )
			{
				m_pParticleInfoArray[i].life_time += nAddFrame%3; // curTailPosNum을 라이프로 사용.

				if( m_pParticleInfoArray[i].life_time > 2+(int)RandomNum(-2.0f,4.0f) )
				{
					// 수명이 다한 녀석이 있으면 새로 생성
					m_pParticleInfoArray[i].life_time = 0;
					m_pParticleInfoArray[i].cur_Pos.x = effect_sort.vCurPos.x+RandomNum(-1.7f,1.7f);
					m_pParticleInfoArray[i].cur_Pos.y = effect_sort.vCurPos.y+RandomNum(-1.7f,1.7f);
					m_pParticleInfoArray[i].cur_Pos.z = effect_sort.vCurPos.z+RandomNum(-1.7f,1.7f);

					m_pParticleInfoArray[i].fade = 1.0f; // 투명도. 점점 줄어든다.
					m_pParticleInfoArray[i].scale = RandomNum( 0.5f, 1.0f ); // 입자크기를 랜덤.
				}

				m_pParticleInfoArray[i].fade -= 0.04f*(float)nAddFrame; // 투명도. 점점 줄어든다.
				if( m_pParticleInfoArray[i].fade < 0.0f )
					m_pParticleInfoArray[i].fade = 0.0f;

				if( effect_sort.subNum == 1 )
				{
					m_pParticleInfoArray[i].cur_Pos.x += RandomNum( -2.1f, 2.1f ); // 약간 흔들리게...
					m_pParticleInfoArray[i].cur_Pos.y -= (float)nAddFrame*5.2f; // 일정 속도로 떨어지게.
					m_pParticleInfoArray[i].cur_Pos.z += RandomNum( -2.1f, 2.1f ); // 약간 흔들리게...
				}
			}
		}

		break;

	case PET_SKILL_EFFECT_TYPE2: // 포이즌 이펙트. 라비용이 구형 액체를 발사해서 독구름이 터진다.

		if( !effect_sort.pMobFrom )
			return;

		if( effect_sort.nCurFrame >= 22 && effect_sort.subNum == 0 ) // 후반부에 한번만 가스 이펙트를 넣어준다.
		{
			// 여기에 가스 이펙트 넣어줘야한다.
			EffectSort ef_sort;

			ef_sort.nType = FX_POISON;
			ef_sort.Index = 10082;	// 차후 같은 형식의 스킬 구분
			ef_sort.nCurFrame = 0; // 이렇게 하면 연기가 바로 나온다.

			float fX = cos(effect_sort.pMobFrom->m_dir) * 20.0f; // 일정 방향 앞으로...
			float fZ = sin(effect_sort.pMobFrom->m_dir) * 20.0f;
			ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x + fX, effect_sort.vPos.y, effect_sort.vPos.z + fZ);

			ef_sort.vTargetPos = ef_sort.vPos;

			effect_sort.pMobFrom->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

			effect_sort.subNum = 1; // 한번만 넣게..
		}

		break;
	}
}

BOOL CFxPetSkill::Render( EffectSort &effect_sort ) // 왜곡된 평면을 드로우.(평면이 2D용 평면임에 유의. 만들당시부터 3D가 아니라 2D좌표다.)
{
	int i;
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DVECTOR vViewAt = pCMyApp->GetEyeAtPos();
	D3DVECTOR vViewFrom;
	pCMyApp->GetEyePos( vViewFrom );
	float fViewDist = Magnitude( vViewFrom-vViewAt );

	DWORD dwLighting, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	float rectwidth = 0.0f;
	float tu = 0.0f;
	float tv = 0.0f;

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;

	switch( effect_sort.Index ) // effect_sort 의 Index를 타입으로 사용한다.
	{
	case PET_SKILL_EFFECT_TYPE1: // 대상에게 조그만 광자가 날라가서 부딪치는 이펙트.

		if( effect_sort.nCurFrame < 0 ) // -프레임일땐 무시
			return FALSE;

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

		if( effect_sort.subNum == 0 ) // 날아가는 상태
		{
			// 깜빡이는 이펙트
			rectwidth += RandomNum(4.25f,4.7f);

			if( m_pBillBoard )
			{
				m_pBillBoard->SetBillBoard( vViewVec, FALSE, 0.0f );
				m_pBillBoard->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );

				m_pBillBoard->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 0.0f )  );
				m_pBillBoard->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 0.0f )  );

				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_LIGHT ] ) );
				m_pBillBoard->Render();
			}
		}
		else if( effect_sort.subNum == 1 ) // 확산되어 퍼지는 상태.
		{
			// 깜빡이는 이펙트
			rectwidth = 12.5f + (effect_sort.nCurFrame-10)*1.0f;

			if( m_pBillBoard )
			{
				m_pBillBoard->SetBillBoard( vViewVec, FALSE, 0.0f );
				m_pBillBoard->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );

				m_pBillBoard->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 0.0f )  );
				m_pBillBoard->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 0.0f )  );

				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_LIGHT ] ) );
				m_pBillBoard->Render();
			}

		}
		else // 없는 상태면 지워준다.
		{
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

			return TRUE;
		}

		// 파티클 출력.
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

		for( i = 0 ; i < MAX_FXPETSKILL_PARTICLE ; ++i )
		{
			if( fViewDist > 50.0f && fViewDist < 500.0f ) // 카메라와 일정거리 이상 떨어져있다면... 너무 멀어도 보정 안한다.
				rectwidth = m_pParticleInfoArray[i].scale * (fViewDist*0.02f) * 0.5f;
			else // 카메라와 가까울땐 그냥 출력
				rectwidth = m_pParticleInfoArray[i].scale * 0.5f;

			if( m_pBillBoard )
			{
				m_pBillBoard->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 0.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 0.0f, 0.0f )  );
				m_pBillBoard->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 1.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 1.0f, 0.0f )  );

				m_pBillBoard->SetBillBoard( vViewVec, FALSE, 0.0f );
				m_pBillBoard->SetTranslationXYZ( m_pParticleInfoArray[i].cur_Pos.x
												 , m_pParticleInfoArray[i].cur_Pos.y, m_pParticleInfoArray[i].cur_Pos.z );

				m_pBillBoard->Render();
			}
		}

		break;

	case PET_SKILL_EFFECT_TYPE2: // 포이즌 이펙트. 라비용이 구형 액체를 발사해서 독구름이 터진다.

		if( !effect_sort.pMobFrom )
			return FALSE;

		if( effect_sort.nCurFrame >= 52 ) // 프레임이 끝났다면...
		{
			return TRUE;  // 이펙트를 제거한다.
		}

		// 방향.
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.pMobFrom->m_dir );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

		D3DMath_MatrixMultiply( matTrans, matRotate, matTrans );

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matTrans );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		m_pPoison->Render( GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE );

		break;

	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return FALSE;
}
