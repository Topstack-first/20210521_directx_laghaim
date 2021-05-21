#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "DecoObj.h"
#include "ImageDefine.h"

typedef struct Deco_Particle Deco_Particle;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSmokeParticle::CFxSmokeParticle()
{
	smoke				= NULL;
	Create();
}

CFxSmokeParticle::~CFxSmokeParticle()
{
	SAFE_DELETE(smoke);
}

void CFxSmokeParticle::Create()
{
	SAFE_DELETE(smoke);

	smoke = new CRectPlane;
	if( smoke )
		smoke->Create( 7.0f, 7.0f, FALSE );
}


void CFxSmokeParticle::Update_Smoke_Particle(EffectSort &effect_sort)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	for( int i = 0; i < MAX_SMOKE_PARTICLE; i++ )
	{
		deco_particle[i].start_time -= effect_sort.nCurFrame;
		if( deco_particle[i].start_time < 0 ) deco_particle[i].start_time = 0;

		if( deco_particle[i].start_time == 0 )
		{
			deco_particle[i].cur_time -= effect_sort.nCurFrame;
			if( deco_particle[i].cur_time < 0 ) deco_particle[i].cur_time = 0;

			if( deco_particle[i].cur_time == 0 )
			{
				deco_particle[i].life_time = RandomNum( 50, 50 );

				if( i != 0 )
				{
					if( deco_particle[i-1].cur_time < 5 )
						deco_particle[i].start_time = (5 - deco_particle[i-1].cur_time );
					if( deco_particle[i-1].cur_time > 5 )
						deco_particle[i].life_time -= (deco_particle[i-1].cur_time - 5);
				}

				deco_particle[i].cur_time = deco_particle[i].life_time;
				deco_particle[i].scale = 1.0f;
				deco_particle[i].fade = 0.5f;

				deco_particle[i].red = 1.0f;
				deco_particle[i].green = 1.0f;
				deco_particle[i].blue = 1.0f;

				deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x + RandomNum(-2.0f, 2.0f),
														vDrawPos.y, //andomNum(-3.0f, 3.0f),
														//vDrawPos.y + (i*0.3f),
														vDrawPos.z + RandomNum(-2.0f, 2.0f) );

				deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );

				if( i < 8 )
					deco_particle[i].direction = D3DVECTOR( RandomNum(-0.05f, 0.05f), RandomNum(0.25f, 0.30f), RandomNum(-0.05f, 0.05f) );
				else
					deco_particle[i].direction = D3DVECTOR( RandomNum(-0.05f, 0.05f), RandomNum(0.30f, 0.35f), RandomNum(-0.05f, 0.05f) );

				deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
			}
			else
			{
				deco_particle[i].fade = 1.0f - ( ((float)deco_particle[i].life_time - (float)deco_particle[i].cur_time) / (float)deco_particle[i].life_time);
				if( deco_particle[i].fade > 0.5f )
					deco_particle[i].fade = 0.5f;
				deco_particle[i].scale = 1.0f + ( ( (float)deco_particle[i].life_time - (float)deco_particle[i].cur_time ) * 0.005f );

				if( effect_sort.nCurFrame != 0 )
					deco_particle[i].cur_Pos += ( (deco_particle[i].direction * effect_sort.nCurFrame )
												  + (deco_particle[i].gravity * effect_sort.nCurFrame) );
			}
		}
	}
}

void CFxSmokeParticle::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxSmokeParticle::DeleteRes()
{}

BOOL CFxSmokeParticle::Render(EffectSort &effect_sort)
{
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SMOKEPARTICLE ] ));

	Update_Smoke_Particle(effect_sort);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	if( smoke )
	{
		for( int i = 0; i < MAX_SMOKE_PARTICLE; i++ ) // max °íÃÄ¾ß µÊ...
		{
			if( deco_particle[i].start_time == 0 )
			{
				smoke->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
				smoke->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				smoke->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				smoke->Render();
			}
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	effect_sort.nCurFrame = 0;

	return FALSE;
}


