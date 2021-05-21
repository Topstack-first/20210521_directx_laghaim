#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "DecoObj.h"
#include "IndexedTexture.h"

typedef struct Deco_Particle Deco_Particle;




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSprayParticle::CFxSprayParticle()
{
	spray				= NULL;

	Create();
}

CFxSprayParticle::~CFxSprayParticle()
{
	SAFE_DELETE(spray);
}

void CFxSprayParticle::Create()
{
	SAFE_DELETE(spray);

	spray = new CRectPlane;
	if( spray )
		spray->Create( 7.0f, 7.0f, FALSE );
}


void CFxSprayParticle::Update_Spray_Particle(EffectSort &effect_sort)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	int rnd_start_time = RandomNum( 100, 130 );

	for( int i = 0; i < MAX_SPRAY_PARTICLE; i++ )
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
				deco_particle[i].life_time = RandomNum( 35, 35 );

				deco_particle[i].cur_time = deco_particle[i].life_time;
				deco_particle[i].scale = 1.0f;
				deco_particle[i].fade = 0.5f;

				deco_particle[i].red = 1.0f;
				deco_particle[i].green = 1.0f;
				deco_particle[i].blue = 1.0f;

				deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x + RandomNum(-2.0f, 2.0f),
														vDrawPos.y, //andomNum(-3.0f, 3.0f),
														vDrawPos.z + RandomNum(-2.0f, 2.0f) );

				deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );

				// 분수 같이 떨어지는 효과
				deco_particle[i].direction = D3DVECTOR( RandomNum(-0.15f, 0.15f), RandomNum(1.5f, 1.5f), RandomNum(-0.15f, 0.15f) );
				deco_particle[i].gravity = D3DVECTOR( 0.0f, -0.085f, 0.0f );
			}
			else
			{
				deco_particle[i].fade = 1.0f - ( ((float)deco_particle[i].life_time - (float)deco_particle[i].cur_time) / (float)deco_particle[i].life_time);
				if( deco_particle[i].fade > 0.5f )
					deco_particle[i].fade = 0.5f;
				deco_particle[i].scale = 1.0f + ( ( (float)deco_particle[i].life_time - (float)deco_particle[i].cur_time ) * 0.005f );

				if( effect_sort.nCurFrame != 0 )
				{
					deco_particle[i].direction += (deco_particle[i].gravity * effect_sort.nCurFrame);
					deco_particle[i].cur_Pos += (deco_particle[i].direction * effect_sort.nCurFrame);
				}
			}
		}
	}
}

void CFxSprayParticle::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxSprayParticle::DeleteRes()
{}

BOOL CFxSprayParticle::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
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

	GET_D3DDEVICE()->SetTexture(0, pCMyApp->m_pDecoTexture->GetSurfaceByIdx(effect_sort.num));

	Update_Spray_Particle(effect_sort);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	if( spray )
	{
		for( int i = 0; i < MAX_SPRAY_PARTICLE; i++ ) // max 고쳐야 됨...
		{
			if( deco_particle[i].start_time == 0 )
			{
				spray->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				spray->SetBillBoard(vViewVec, FALSE, 0.0f);
				spray->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				spray->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				spray->Render();
			}
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	effect_sort.nCurFrame = 0;

	return FALSE;
}


