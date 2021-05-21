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
#include "FxPortalBOSSRAID.h"
#include "Land.h"
#include "UIMgr.h"

extern float				g_fDSDistance;



extern int					g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFxPortalBossraid::CFxPortalBossraid()
{
	m_Plane = NULL;
	Create();
}

CFxPortalBossraid::~CFxPortalBossraid()
{
	SAFE_DELETE(m_Plane);
}

void CFxPortalBossraid::Create()
{
	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}

void CFxPortalBossraid::LoadRes()
{

}

void CFxPortalBossraid::DeleteRes()
{}


void CFxPortalBossraid::FrameMove( EffectSort &eff, int nAddFrame )
{
	Deco_Particle* dp = (Deco_Particle*)eff.effects;
	D3DVECTOR vDrawVec;

	for( int i=0; i<MAX_PORTAL_BOSSRAID_PARTICLE; i++)
	{
		float fDist = DistVecToVec(dp[i].cur_Pos, eff.vCurPos );

		if( fDist <= eff.num * 0.1f )
			dp[i].texIndex = 0;// 초기화

		if( fDist >= eff.num * 3)
			dp[i].texIndex = 0;// 초기화

		switch( dp[i].texIndex )
		{
		case 0: // 초기화
			// 시작위치
			dp[i].cur_Pos = eff.vCurPos;
//			vDrawVec.x = (rand()%(int)(eff.num*0.4f)+(eff.num*0.3f)) * ((rand()%2 == 0)?-1:1);
//			vDrawVec.y = (rand()%(int)(eff.num*0.4f)+(eff.num*0.3f)) * ((rand()%2 == 0)?-1:1);
//			vDrawVec.z = (rand()%(int)(eff.num*0.4f)+(eff.num*0.3f)) * ((rand()%2 == 0)?-1:1);
			vDrawVec.x = (rand()%eff.num) * ((rand()%2 == 0)?-1:1);
			vDrawVec.y = (rand()%eff.num) * ((rand()%2 == 0)?-1:1);
			vDrawVec.z = (rand()%eff.num) * ((rand()%2 == 0)?-1:1);
			dp[i].cur_Pos += vDrawVec; // 시작 위치

			// 진행 방향 찾기
			vDrawVec =  eff.vCurPos - dp[i].cur_Pos ;
			dp[i].direction  = Normalize(vDrawVec);

			dp[i].blue	= ((float)(rand()%10)+1) / 10.0f + eff.num * 0.0150f;	// 진행 속도
			dp[i].scale = eff.num * 0.05f;	// 크기
			dp[i].fade	= 1.0f;	// 페이드
			dp[i].gravity.x = 0;
			dp[i].texIndex = 1; // 상태값 [0:초기화, 1프레임 계산]
			break;

		case 1: // 프레임 계산

			vDrawVec = dp[i].direction * dp[i].blue;
			dp[i].cur_Pos += vDrawVec;
			dp[i].scale -= eff.num * 0.0022f;
			dp[i].gravity.x += 0.02f;
			if( dp[i].scale <= 0.0f )
				dp[i].texIndex = 0;// 초기화
			break;
		}
	}

}

BOOL CFxPortalBossraid::Render(EffectSort &eff)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	FrameMove(eff, 0);

	Deco_Particle* dp = (Deco_Particle*)eff.effects;
	D3DVECTOR vDrawVec;

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	if( m_Plane )
	{
		for(int i=0; i<3; i++)
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FORTAL_BOSSRAID ]) ); // 텍스쳐 지정

			float angle = 0;
			float scale = ((float)(eff.num));
			float color;

			switch(i)
			{
			case 0:
				angle = eff.vSortPos.x += 0.002f;
				break;

			case 1:
				angle = eff.vSortPos.y += 0.007f;
				break;

			case 2:
				angle = eff.vSortPos.z += 0.013f;
				break;
			}

			D3DVECTOR	vPos, vTempPos, vEye;

			vTempPos = eff.vTargetPos - eff.vPos;
			vTempPos = Normalize(vTempPos);

			vTempPos = vTempPos * ((i+1) * 2.0f);
			vEye = vPos = eff.vCurPos;
			vPos.x += vTempPos.x;
			vPos.z += vTempPos.z;

			vTempPos = vTempPos * 70.0f;
			vEye.x += vTempPos.x;
			vEye.z += vTempPos.z;

			color = 0.65f + (i*0.15f);

			m_Plane->SetBillBoard(vTempPos, FALSE, 0.0f);
			m_Plane->Scale( scale, scale, scale);
			m_Plane->SetColor(color, color, color, 0.5f);
			m_Plane->SetTranslationXYZ( vPos.x, vPos.y, vPos.z);
			m_Plane->RotateZ( angle );
			m_Plane->Render();
		}

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		for(int i=0; i<MAX_PORTAL_BOSSRAID_PARTICLE; i++)
		{
			if( dp[i].texIndex == 1)
			{
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_RUNSMOKE ]) ); // 텍스쳐 지정 EFF_DSTONE_EFF_SMOKE

				m_Plane->SetBillBoard(vViewVec, FALSE, 0.0f);
				m_Plane->Scale( dp[i].scale, dp[i].scale, dp[i].scale);
				m_Plane->SetColor(1.0f, 0.7f, 0.8f, 1.0f);
				m_Plane->SetTranslationXYZ( dp[i].cur_Pos.x, dp[i].cur_Pos.y, dp[i].cur_Pos.z);
				m_Plane->RotateZ( dp[i].gravity.x  );
				m_Plane->Render();
			}
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

	return FALSE;
}

