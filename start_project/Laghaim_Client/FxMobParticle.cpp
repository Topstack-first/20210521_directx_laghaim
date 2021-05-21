#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkMob.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"

extern int g_EffectTextr[];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxMobParticle::CFxMobParticle()
{
	light = new CRectPlane;
	if( light )
		light->Create( 1.0f, 1.0f, FALSE );
}

CFxMobParticle::~CFxMobParticle()
{
	SAFE_DELETE(light);
}

void CFxMobParticle::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxMobParticle::DeleteRes()
{}

void CFxMobParticle::Update_Mob_Particle(EffectSort &effect_sort)
{
	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.pMobTo->m_NkMob_Particle;

	if( effect_sort.num == 256 )
	{

		int rnd_start_time = 0;

		for( int i = 0; i < MAX_NKMOB_PARTICLE; i++ )
		{
			deco_particle[i].start_time -= effect_sort.nCurFrame;
			if( deco_particle[i].start_time < 0 ) deco_particle[i].start_time = 0;

			if( deco_particle[i].start_time == 0 )
			{
				deco_particle[i].cur_time -= effect_sort.nCurFrame;
				if( deco_particle[i].cur_time < 0 ) deco_particle[i].cur_time = 0;

				if( deco_particle[i].cur_time == 0 )
				{
					deco_particle[i].start_time = rnd_start_time;
					deco_particle[i].life_time = RandomNum( 10, 15 );

					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 0.2f;
					deco_particle[i].fade = 0.5f;

					deco_particle[i].red = 0.9f;
					deco_particle[i].green = 0.9f;
					deco_particle[i].blue = 0.29f;

					float rnd_pos = RandomNum(-0.75f, 0.75f);

					deco_particle[i].first_Pos = D3DVECTOR( effect_sort.pMobTo->m_wx + rnd_pos,
															effect_sort.pMobTo->m_wy + rnd_pos + 5.0f,
															effect_sort.pMobTo->m_wz + rnd_pos);

					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );

					// 맞았을때 파편이 퍼지는 효과
					deco_particle[i].direction = D3DVECTOR( RandomNum(-0.5f, 0.5f), RandomNum(-0.5f, 0.5f), RandomNum(-0.5f, 0.5f) );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, -0.075f, 0.0f );
				}
				else
				{
					deco_particle[i].scale = 0.5f;

					// 렌더링 ONE_ONE
					deco_particle[i].red = (0.9f - ( 0.9f * deco_particle[i].fade * 0.75f));
					deco_particle[i].green = (0.9f - ( 0.9f * deco_particle[i].fade * 0.75f));
					deco_particle[i].blue = (0.29f - ( 0.29f * deco_particle[i].fade * 0.75f));

					if( effect_sort.nCurFrame != 0 )
					{
						deco_particle[i].direction += (deco_particle[i].gravity * effect_sort.nCurFrame);
						deco_particle[i].cur_Pos += (deco_particle[i].direction * effect_sort.nCurFrame);
					}
				}
			}
		}
	}

	return;
}


BOOL CFxMobParticle::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !light )
		return true;

	if( effect_sort.pMobTo->m_bDrawEffects == FALSE )
	{
		effect_sort.nCurFrame = 0;
		effect_sort.Index = 0;
		return FALSE;
	}

	DWORD dwlingting, dwRenState[2];
	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwlingting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );
	if( effect_sort.num == 256 )
	{
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );
	}
	else
	{
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );
	}

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	Update_Mob_Particle(effect_sort);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.pMobTo->m_NkMob_Particle;

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MOBPARTICLE_LIGHT ] ) );

	for( int i = 0; i < MAX_NKMOB_PARTICLE; i++ ) // max 고쳐야 됨...
	{
		if( deco_particle[i].start_time == 0 )
		{
			light->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
			light->SetBillBoard(vViewVec, FALSE, 0.0f);
			light->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
			light->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
			light->Render();
		}
	}

	effect_sort.nCurFrame = 0;
	effect_sort.Index = 0;

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );

	return FALSE;
}

