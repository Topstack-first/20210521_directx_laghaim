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
#include "FxFireSmokeParticle.h"
#include "Land.h"
#include "UIMgr.h"

extern float				g_fDSDistance;



extern int					g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFxFireSmokeParticle::CFxFireSmokeParticle()
{
	m_Plane = NULL;
	Create();
}

CFxFireSmokeParticle::~CFxFireSmokeParticle()
{
	SAFE_DELETE(m_Plane);
}

void CFxFireSmokeParticle::Create()
{
	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}

void CFxFireSmokeParticle::LoadRes()
{

}

void CFxFireSmokeParticle::DeleteRes()
{}


void CFxFireSmokeParticle::FrameMove( EffectSort &eff, int nAddFrame )
{
	Deco_Particle* dp = (Deco_Particle*)eff.effects;

	for( int i=0; i<MAX_FIRESMOKE_PARTICLE; i++)
	{
		switch( dp[i].texIndex )
		{
		case 0: // 초기화
			// 시작위치
			dp[i].cur_Pos		= eff.vPos;	// 시작 위치 현재 위치
			dp[i].start_time	= rand()%1000 ; // 몇초뒤에 이펙트 를 긜지 결정 하자
			dp[i].life_time		= rand()%5000 + 15000; // 이펙트를 그리는 시간
			dp[i].scale			= (float(rand()%30)+5) / 3.0f;	// 시작 크기
			dp[i].direction.x	= float(rand()%50)+0.5f; // 회전 시작 각도
			dp[i].first_Pos.x	= (float(rand()%5)+5) / 10.0f; // 스케일 속도
			dp[i].first_Pos.y	= (float(rand()%4)+6) / 10.0f; // 진행 속도
			dp[i].first_Pos.z	= (float(rand()%10)+2) / 1000.0f; // 회전 속도
			dp[i].cur_time		= timeGetTime();
			dp[i].texIndex		= 1;	// 상태값 [0:초기화, 1:계산]
			break;

		case 1: // 프레임 계산
			if( dp[i].start_time <= timeGetTime() - dp[i].cur_time )
			{
				dp[i].texIndex = 2;
				dp[i].cur_time = timeGetTime();
			}
			break;
		case 2:
			if(  dp[i].life_time <= timeGetTime() - dp[i].cur_time )
				dp[i].texIndex = 0;

			dp[i].cur_Pos.y		+= dp[i].first_Pos.y;	// 진행
			dp[i].scale			+= dp[i].first_Pos.x;	// 스케일
			dp[i].direction.x	+= dp[i].first_Pos.z;	// 회전

			break;
		}
	}

}

BOOL CFxFireSmokeParticle::Render(EffectSort &eff)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	FrameMove(eff, 0);

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();
	Deco_Particle* dp = (Deco_Particle*)eff.effects;

	if( m_Plane )
	{
		for(int i=0; i<MAX_FIRESMOKE_PARTICLE; i++)
		{
			if( dp[i].texIndex == 2)
			{
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_RUNSMOKE ]) ); // 텍스쳐 지정

				m_Plane->SetBillBoard(vViewVec, FALSE, 0.0f);
				m_Plane->Scale( dp[i].scale, dp[i].scale, dp[i].scale);
				m_Plane->SetColor(0.4f, 0.4f, 0.4f, 0.2f);
				m_Plane->SetTranslationXYZ( dp[i].cur_Pos.x, dp[i].cur_Pos.y, dp[i].cur_Pos.z);
				m_Plane->RotateZ( dp[i].direction.x );

				m_Plane->Render();
			}
		}
	}



	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}

