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

#include "FxParticle.h"





CFxTypeParticle::CFxTypeParticle()
{
	m_pRectPlane = NULL;
	m_pParticleInfoArray = NULL;
}

CFxTypeParticle::~CFxTypeParticle()
{
	DeleteRes();
}

void CFxTypeParticle::DeleteRes()
{
	SAFE_DELETE( m_pRectPlane );
}

void CFxTypeParticle::LoadRes()
{
	DeleteRes();

	m_pRectPlane = new CRectPlane;
	if( m_pRectPlane )
		m_pRectPlane->Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)
}


void CFxTypeParticle::FrameMove( EffectSort &effect_sort, int nAddFrame )
{
	int i = 0;
	DWORD dwElapsedTick = 0;
	DWORD ParticleDieCount = 0;

	// 타입별로 다른 AI를 지정한다.
	switch( effect_sort.Index ) // effect_sort 의 Index를 타입으로 사용한다.
	{
	case FX_PARTICLE_TYPE_LIP : // 2007.03.14 화이트
	case FX_PARTICLE_TYPE_L	  :
	case FX_PARTICLE_TYPE_O	  :
	case FX_PARTICLE_TYPE_V	  :
	case FX_PARTICLE_TYPE_E	  :
	case FX_PARTICLE_TYPE_HEARTLORD: // 일본 발렌타인데이 아이템 하트로드,러브러브로드 의 이펙트. 하트들이 팔랑 팔랑~ ///하트로드

		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

		for( i = 0 ; i < MAX_PARTICLE_FOR_HEARTLOAD ; ++i )
		{
			dwElapsedTick = timeGetTime() - m_pParticleInfoArray[i].cur_time; // 이전에서 지난시간.
			m_pParticleInfoArray[i].cur_time = timeGetTime() - m_pParticleInfoArray[i].start_time; // 생성후 경과 시간.

			if( m_pParticleInfoArray[i].cur_time < 0 ) // 이 경우는 아직 생성안된 녀석.
				continue;

			if( m_pParticleInfoArray[i].cur_time > m_pParticleInfoArray[i].life_time ) // 수명이 다했으면...
			{
				++ParticleDieCount; // 파티클 수명이 다한 숫자를 세둔다.
				continue;
			}


			// gravity 의 방향으로 direction 의 속도 만큼 이동.
//			float v = m_pParticleInfoArray[i].direction.x + (m_pParticleInfoArray[i].direction.y*(float)m_pParticleInfoArray[i].cur_time); // 속도를 구함.
			float v = m_pParticleInfoArray[i].direction.x; // 속도를 구함.
			/*
						if( v > m_pParticleInfoArray[i].direction.z ) // 속도 제한
							v = m_pParticleInfoArray[i].direction.z;
			*/
			/*
						m_pParticleInfoArray[i].direction.y +=

						float v = m_pParticleInfoArray[i].direction.x + m_pParticleInfoArray[i].direction.y; // 속도를 구함.

						v = v * (float)m_pParticleInfoArray[i].cur_time;

						if( v > m_pParticleInfoArray[i].direction.z ) // 속도 제한
							v = m_pParticleInfoArray[i].direction.z;
			*/
			m_pParticleInfoArray[i].cur_Pos.x = m_pParticleInfoArray[i].first_Pos.x + ( m_pParticleInfoArray[i].gravity.x * v * (float)m_pParticleInfoArray[i].cur_time );
			m_pParticleInfoArray[i].cur_Pos.y = m_pParticleInfoArray[i].first_Pos.y + ( m_pParticleInfoArray[i].gravity.y * v * (float)m_pParticleInfoArray[i].cur_time );
			m_pParticleInfoArray[i].cur_Pos.z = m_pParticleInfoArray[i].first_Pos.z + ( m_pParticleInfoArray[i].gravity.z * v * (float)m_pParticleInfoArray[i].cur_time );
			/* // 페이드 조절이 잘 안되서 그냥 뺌.
			//			m_pParticleInfoArray[i].fade = 255 - (m_pParticleInfoArray[i].cur_time * 0.1f);

						if( m_pParticleInfoArray[i].fade < 0 )
							m_pParticleInfoArray[i].fade = 0;
						if( m_pParticleInfoArray[i].fade > 255 )
							m_pParticleInfoArray[i].fade = 255;
			*/
			// 타입별 움직임 처리
			if( m_pParticleInfoArray[i].texFrame == 1 ) // 좌우로 갸우뚱 회전
			{
				if( m_pParticleInfoArray[i].direction.y == 1.0f ) // 오른쪽 회전 중
				{
					m_pParticleInfoArray[i].direction.z += 0.2f*nAddFrame;
					// 일정량 이상은 안 기울어지게.
					if( m_pParticleInfoArray[i].direction.z > 0.5f )
					{
						m_pParticleInfoArray[i].direction.z = 0.5f;
						m_pParticleInfoArray[i].direction.y = 0.0f;
					}
				}
				else // 왼쪽회전중.
				{
					m_pParticleInfoArray[i].direction.z -= 0.2f*nAddFrame;
					// 일정량 이상은 안 기울어지게.
					if( m_pParticleInfoArray[i].direction.z < -0.5f )
					{
						m_pParticleInfoArray[i].direction.z = -0.5f;
						m_pParticleInfoArray[i].direction.y = 1.0f;
					}
				}
			}
			else if( m_pParticleInfoArray[i].texFrame == 2 ) // 커졌다 작아졌다.
			{
				if( m_pParticleInfoArray[i].direction.y == 1.0f ) // 커지는 중
				{
					m_pParticleInfoArray[i].direction.z += 0.2f*nAddFrame;
					// 일정량 이상은 안 기울어지게.
					if( m_pParticleInfoArray[i].direction.z > 0.5f )
					{
						m_pParticleInfoArray[i].direction.z = 0.5f;
						m_pParticleInfoArray[i].direction.y = 0.0f;
					}
				}
				else // 작아지는중
				{
					m_pParticleInfoArray[i].direction.z -= 0.2f*nAddFrame;
					// 일정량 이상은 안 기울어지게.
					if( m_pParticleInfoArray[i].direction.z < -0.5f )
					{
						m_pParticleInfoArray[i].direction.z = -0.5f;
						m_pParticleInfoArray[i].direction.y = 1.0f;
					}
				}
			}

		}

		if( ParticleDieCount == MAX_PARTICLE_FOR_HEARTLOAD ) // 파티클들이 전부 수명을 다했으면...
		{
			effect_sort.nCurFrame = pCMyApp->m_pFxSet->m_pFrameCount[FX_TYPE_PARTICLE]; // 이게 널이면 렌더에서 TRUE를 리턴해 이펙트를 지우게 했다.
		}


		break;

	case FX_PARTICLE_TYPE_LUCKYPIG: // 설날 돼지 회복 이펙트

		if( !effect_sort.pNkChaTo )
			return;

		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

		for( i = 0 ; i < MAX_PARTICLE_FOR_LUCKYPIG ; ++i )
		{
			dwElapsedTick = timeGetTime() - m_pParticleInfoArray[i].cur_time; // 이전에서 지난시간.
			m_pParticleInfoArray[i].cur_time = timeGetTime() - m_pParticleInfoArray[i].start_time; // 생성후 경과 시간.

			if( m_pParticleInfoArray[i].cur_time < 0 ) // 이 경우는 아직 생성안된 녀석.
				continue;

			if( m_pParticleInfoArray[i].cur_time > m_pParticleInfoArray[i].life_time ) // 수명이 다했으면...
			{
				++ParticleDieCount; // 파티클 수명이 다한 숫자를 세둔다.
				continue;
			}

			float v = m_pParticleInfoArray[i].direction.x; // 속도를 구함.

			if( (m_pParticleInfoArray[i].cur_time)%1000 > 500 )
			{
				m_pParticleInfoArray[i].cur_Pos.x = effect_sort.pNkChaTo->m_wx + m_pParticleInfoArray[i].first_Pos.x - ( m_pParticleInfoArray[i].gravity.x * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
				m_pParticleInfoArray[i].cur_Pos.z = effect_sort.pNkChaTo->m_wz + m_pParticleInfoArray[i].first_Pos.z - ( m_pParticleInfoArray[i].gravity.z * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
			}
			else
			{
				m_pParticleInfoArray[i].cur_Pos.x = effect_sort.pNkChaTo->m_wx + m_pParticleInfoArray[i].first_Pos.x + ( m_pParticleInfoArray[i].gravity.x * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
				m_pParticleInfoArray[i].cur_Pos.z = effect_sort.pNkChaTo->m_wz + m_pParticleInfoArray[i].first_Pos.z + ( m_pParticleInfoArray[i].gravity.z * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
			}

			m_pParticleInfoArray[i].cur_Pos.y = effect_sort.pNkChaTo->m_wy + m_pParticleInfoArray[i].first_Pos.y + ( m_pParticleInfoArray[i].gravity.y * v * (float)m_pParticleInfoArray[i].cur_time ); // 밑으로 떨어지게.

			// 페이드 조절이 잘 안되서 그냥 뺌.
//			m_pParticleInfoArray[i].fade = m_pParticleInfoArray[i].fade - ((float)nAddFrame*0.1f);

			if( m_pParticleInfoArray[i].fade < 0 )
				m_pParticleInfoArray[i].fade = 0;
			if( m_pParticleInfoArray[i].fade > 255 )
				m_pParticleInfoArray[i].fade = 255;

		}

		if( ParticleDieCount == MAX_PARTICLE_FOR_LUCKYPIG ) // 파티클들이 전부 수명을 다했으면...
		{
			effect_sort.nCurFrame = pCMyApp->m_pFxSet->m_pFrameCount[FX_TYPE_PARTICLE]; // 이게 널이면 렌더에서 TRUE를 리턴해 이펙트를 지우게 했다.
		}


		break;
	}
}

BOOL CFxTypeParticle::Render( EffectSort &effect_sort )
{
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DVECTOR vViewAt = pCMyApp->GetEyeAtPos();
	D3DVECTOR vViewFrom;
	pCMyApp->GetEyePos( vViewFrom );
	float fViewDist = Magnitude( vViewFrom-vViewAt );

	DWORD dwLighting, dwSrc, dwDest, dwAlphaBlend;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;

	float rectwidth = 0.0f; // vSortPos를 사용안하고 있는것 같으니 그 x값을 크기값으로 사용한다.
	float tu = 0.0f;
	float tv = 0.0f;

	int i = 0, k = 0;

	switch( effect_sort.Index ) // effect_sort 의 Index를 타입으로 사용한다.
	{
	case FX_PARTICLE_TYPE_L    : // 2007.03.14 화이트
	case FX_PARTICLE_TYPE_O    :
	case FX_PARTICLE_TYPE_V    :
	case FX_PARTICLE_TYPE_E    :
	case FX_PARTICLE_TYPE_LIP  :
	case FX_PARTICLE_TYPE_HEARTLORD: // 일본 발렌타인데이 아이템 하트로드,러브러브로드 의 이펙트. 하트들이 팔랑 팔랑~
	case FX_PARTICLE_TYPE_LUCKYPIG: // 설날 돼지 회복 이펙트. 하트로드랑 같은 루틴을 타되 텍스쳐만 바꿔줘도 된다.

		// 주변 이펙트 출력
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		// 파티클 출력.
		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

		if( !m_pParticleInfoArray ) // 파티클들 수명이 다하면 이게 NULL이 된다.
			return FALSE; // 이펙트를 지우라고 알려준다.

		if( effect_sort.Index == FX_PARTICLE_TYPE_HEARTLORD )
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HEART_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_LUCKYPIG )
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LUCKYPIG_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_LUCKYPIG;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_LIP) // 2007.03.14 화이트
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIP_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_L)   // 2007.03.14 화이트
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_L_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_O)   // 2007.03.14 화이트
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_O_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_V)   // 2007.03.14 화이트
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_V_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_E)   // 2007.03.14 화이트
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_E_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}

		for( i = 0 ; i < k ; ++i )
		{
			if( m_pParticleInfoArray[i].cur_time < 0 ) // 이 경우는 아직 생성안된 녀석.
				continue;
			if( m_pParticleInfoArray[i].cur_time > m_pParticleInfoArray[i].life_time ) // 수명이 다했으면...
				continue;


			rectwidth = m_pParticleInfoArray[i].scale * 1.0f;

			if( m_pParticleInfoArray[i].texFrame == 2 ) // 커졌다 작아졌다.
				rectwidth += m_pParticleInfoArray[i].direction.z;



			if( m_pRectPlane )
			{
				m_pRectPlane->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 0.0f, 1.0f )  );
				m_pRectPlane->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 0.0f, 0.0f )  );
				m_pRectPlane->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 1.0f, 1.0f )  );
				m_pRectPlane->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 1.0f, 0.0f )  );
			}

			m_pRectPlane->SetBillBoard( vViewVec, FALSE, 0.0f );

			if( m_pParticleInfoArray[i].texFrame == 1 ) // 좌우로 갸우뚱 회전
				m_pRectPlane->RotateZ( m_pParticleInfoArray[i].direction.z );


			m_pRectPlane->SetTranslationXYZ( m_pParticleInfoArray[i].cur_Pos.x
											 , m_pParticleInfoArray[i].cur_Pos.y, m_pParticleInfoArray[i].cur_Pos.z );

			m_pRectPlane->Render();
		}

		break;
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return FALSE;
}