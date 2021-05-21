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
#include "DirtSnd.h"
#include "LoadEffectSound.h"

#include "Fx_BossRaid_Kai3_03.h"







#define FADE_TIME				100
#define POISONSMOKE_TIME		20
#define POISONFOG_TIME			48
#define POISON_POSITION			5.0f
#define FOG_SIZE_X				2.0f
#define FOG_SIZE_Y				0.5f
#define POISON_RING_SIZE		75.0f
#define POISON_RING_SCALESIZE	2.0f


#define random(num) (rand()%(num))

CFxBossRaidKai3_03::CFxBossRaidKai3_03()
{
	for( int i = 0; i < SMOKE_NUM; i++ )
	{
		m_poison_Smoke[i] = NULL;

		m_poison_Smoke[i] = new CRectPlane;

		if( m_poison_Smoke[i] )
		{
			if( i < ( SMOKE_NUM / 2 ) )
			{
				m_poison_Smoke[i]->Create(( i + 1 ) * FOG_SIZE_X, ( i + 1 ) * FOG_SIZE_Y  , FALSE);
			}

			else
			{
				m_poison_Smoke[i]->Create(( ( ( SMOKE_NUM / 2 ) + 1 ) - i ) * FOG_SIZE_X, ( ( ( SMOKE_NUM / 2 ) + 1 ) - i ) * FOG_SIZE_Y  , FALSE);
			}
		}
	}

	m_Alpha = 0;
	m_fadeValue = 0.0f;
	ground_ring			= NULL;

	Create();
}

CFxBossRaidKai3_03::~CFxBossRaidKai3_03()
{
	SAFE_DELETE(ground_ring);
}

void CFxBossRaidKai3_03::Create()
{
	ground_ring = new CRectPlane;

	if( ground_ring )
		ground_ring->Create( POISON_RING_SIZE, POISON_RING_SIZE, FALSE );
}

void CFxBossRaidKai3_03::LoadRes()
{
}

void CFxBossRaidKai3_03::DeleteRes()
{
	for( int i = 0; i < SMOKE_NUM; i++ )
	{
		if( m_poison_Smoke[i] )
			SAFE_DELETE(m_poison_Smoke[i]);
	}
}

BOOL CFxBossRaidKai3_03::Render( EffectSort &effect_sort )
{
	if( effect_sort.nCurFrame > FX_BOSSRAID_KAI3_03_FRAMECOUNT ) // 이펙트 제거 처리
	{
		m_Alpha = 0;
		m_fadeValue = 0.0f;
		return TRUE;
	}

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;

	// 상태값 보존.
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);
	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	if(  effect_sort.nCurFrame >= POISONSMOKE_TIME && effect_sort.nCurFrame < POISONFOG_TIME + 5 )
	{
		RenderRing( effect_sort);

		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BOSSRAID_KAI3_03_SMOKE ] ) ;
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		vViewVec = pCMyApp->GetEyeDirection();
		float smoke_fade = 0.3f + ((effect_sort.nCurFrame - POISONSMOKE_TIME * 0.5f) / (float)(FX_BOSSRAID_KAI3_03_FRAMECOUNT - POISONSMOKE_TIME) );
		float smoke_scale = 0.0f + ((effect_sort.nCurFrame - POISONSMOKE_TIME) * 0.5f);
		float smoke_X = 0.0f;
		float smoke_Z = 0.0f;

		for( int i = 0; i < SMOKE_NUM; i++ )
		{
			if( i < ( SMOKE_NUM / 2 ) )
			{
				smoke_X = ( ( ( SMOKE_NUM / 2 ) - 1 ) - i ) * POISON_POSITION;
				smoke_Z = ( ( ( SMOKE_NUM / 2 ) - 1 ) - i ) * POISON_POSITION;
			}

			else
			{
				smoke_X = -( ( i - ( SMOKE_NUM / 2 ) ) * POISON_POSITION );
				smoke_Z = -( ( i - ( SMOKE_NUM / 2 ) ) * POISON_POSITION );
			}

			m_poison_Smoke[i]->SetColor(0.6f, 0.1f, 0.5f, smoke_fade);
			m_poison_Smoke[i]->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_poison_Smoke[i]->Scale(1.5f+smoke_scale, 1.5f+smoke_scale, 1.5f+smoke_scale);
			m_poison_Smoke[i]->SetTranslationXYZ(effect_sort.pMobFrom->m_wx + smoke_X , effect_sort.pMobFrom->m_wy + 70.0f, effect_sort.pMobFrom->m_wz );
			m_poison_Smoke[i]->Render();
		}


		if (g_pDSound)
		{
			if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_FISSION]))
				g_pDSound->Play(g_EffectSound[EFF_SND_FISSION]);
		}
	}

	if( effect_sort.nCurFrame >= POISONFOG_TIME )
	{
		DWORD dwElapsedTick = (effect_sort.nCurFrame - POISONFOG_TIME) / 2 ; // 프레임을 직접 계산한다. (프레임 레이트를 다르게 적용하기 위해서...)

		if( dwElapsedTick >= 20 )
			dwElapsedTick %= 20; // 루프시키기.

		// 알파 조정.
		if( effect_sort.nCurFrame < FADE_TIME ) // 안보이다가 점점 보이게...
		{
			m_Alpha += (float)effect_sort.nCurFrame*0.1f;

			if( m_Alpha > 255 )
				m_Alpha = 255;
		}

		if( effect_sort.nCurFrame > FADE_TIME )  // 점점 사라지듯이
		{
			m_Alpha -= (float)effect_sort.nCurFrame*0.01f;

			if( m_Alpha > 255 )
				m_Alpha = 0;
		}

		m_TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,g_pNk2DFrame->GetClientHeight(),0), 1, RGBA_MAKE(255,255,255,m_Alpha), 0, 0, 1.0f );
		m_TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0), 1, RGBA_MAKE(255,255,255,m_Alpha), 0, 0, 0 );
		m_TLVec[2] = D3DTLVERTEX( D3DVECTOR(g_pNk2DFrame->GetClientWidth(),0,0), 1, RGBA_MAKE(255,255,255,m_Alpha), 0, 1.0f, 0 );
		m_TLVec[3] = D3DTLVERTEX( D3DVECTOR(g_pNk2DFrame->GetClientWidth(),g_pNk2DFrame->GetClientHeight(),0), 1, RGBA_MAKE(255,255,255,m_Alpha), 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_POISON_FOG + dwElapsedTick ] ) );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

		if (g_pDSound && effect_sort.nCurFrame >= POISONFOG_TIME + 10 && effect_sort.nCurFrame <= POISONFOG_TIME + 25 )
		{
			if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_M_ASSAULT2]))
				g_pDSound->Play(g_EffectSound[EFF_SND_M_ASSAULT2]);
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}


BOOL CFxBossRaidKai3_03::RenderRing(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( effect_sort.nCurFrame < POISONSMOKE_TIME + 5 )
	{
		m_fadeValue += (float)effect_sort.nCurFrame*0.1f;

		if( m_fadeValue > 1.0f )
			m_fadeValue = 1.0f;
	}

	if( effect_sort.nCurFrame > POISONFOG_TIME )
	{
		m_fadeValue -= (float)effect_sort.nCurFrame*0.0008f;

		if( m_fadeValue < 0.0f )
			m_fadeValue = 0.0f;
	}

	float scaleRate = 1.0f + ( ( effect_sort.nCurFrame - POISONSMOKE_TIME ) * 0.3f );

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR(effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz);

	float red, green, blue;
	red = green = blue = 0.0f;

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BLACKHOLE_EARTH + 1 ] ) );

	if( ground_ring )
	{
		red = 0.8f;
		green = 0.3f;
		blue = 0.6f;

		ground_ring->SetColor(red, green, blue, m_fadeValue);
		ground_ring->SetRotationX( g_PI * 0.5f );
		ground_ring->RotateZ( scaleRate * 2.0f );

		if( scaleRate <= POISON_RING_SCALESIZE )
		{
			ground_ring->Scale( scaleRate, scaleRate, scaleRate );
		}

		else
		{
			ground_ring->Scale( POISON_RING_SCALESIZE, POISON_RING_SCALESIZE, POISON_RING_SCALESIZE );
		}

		ground_ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 0.75f, vDrawPos.z);
		ground_ring->Render();
	}

	return FALSE;
}


