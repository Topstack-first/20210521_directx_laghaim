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
		case 0: // �ʱ�ȭ
			// ������ġ
			dp[i].cur_Pos		= eff.vPos;	// ���� ��ġ ���� ��ġ
			dp[i].start_time	= rand()%1000 ; // ���ʵڿ� ����Ʈ �� �Z�� ���� ����
			dp[i].life_time		= rand()%5000 + 15000; // ����Ʈ�� �׸��� �ð�
			dp[i].scale			= (float(rand()%30)+5) / 3.0f;	// ���� ũ��
			dp[i].direction.x	= float(rand()%50)+0.5f; // ȸ�� ���� ����
			dp[i].first_Pos.x	= (float(rand()%5)+5) / 10.0f; // ������ �ӵ�
			dp[i].first_Pos.y	= (float(rand()%4)+6) / 10.0f; // ���� �ӵ�
			dp[i].first_Pos.z	= (float(rand()%10)+2) / 1000.0f; // ȸ�� �ӵ�
			dp[i].cur_time		= timeGetTime();
			dp[i].texIndex		= 1;	// ���°� [0:�ʱ�ȭ, 1:���]
			break;

		case 1: // ������ ���
			if( dp[i].start_time <= timeGetTime() - dp[i].cur_time )
			{
				dp[i].texIndex = 2;
				dp[i].cur_time = timeGetTime();
			}
			break;
		case 2:
			if(  dp[i].life_time <= timeGetTime() - dp[i].cur_time )
				dp[i].texIndex = 0;

			dp[i].cur_Pos.y		+= dp[i].first_Pos.y;	// ����
			dp[i].scale			+= dp[i].first_Pos.x;	// ������
			dp[i].direction.x	+= dp[i].first_Pos.z;	// ȸ��

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
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_RUNSMOKE ]) ); // �ؽ��� ����

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

