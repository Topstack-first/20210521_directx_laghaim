#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "DecoObj.h"

typedef struct Deco_Particle Deco_Particle;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxTurnParticle::CFxTurnParticle()
{
	turn				= NULL;

	Create();
}

CFxTurnParticle::~CFxTurnParticle()
{
	SAFE_DELETE(turn);
}

void CFxTurnParticle::Create()
{
	SAFE_DELETE(turn);

	turn = new CRectPlane;
	if( turn )
		turn->Create( 7.0f, 7.0f, FALSE );
}


void CFxTurnParticle::Update_Turn_Particle(EffectSort &effect_sort)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	for( int i = 0; i < MAX_TURN_PARTICLE; i++ )
	{
		deco_particle[i].start_time -= effect_sort.nCurFrame;
		if( deco_particle[i].start_time < 0 ) deco_particle[i].start_time = 0;

		if( deco_particle[i].start_time == 0 )
		{
			deco_particle[i].cur_time -= effect_sort.nCurFrame;
			if( deco_particle[i].cur_time < 0 ) deco_particle[i].cur_time = 0;

			if( deco_particle[i].cur_time == 0 )
			{
				deco_particle[i].life_time = RandomNum( 100, 100 ) ;

				deco_particle[i].cur_time = deco_particle[i].life_time;
				deco_particle[i].scale = 0.0f;
				deco_particle[i].fade = 0.3f;

				deco_particle[i].red = 0.0f;
				deco_particle[i].green = 0.0f;
				deco_particle[i].blue = 0.0f;

				if( i == 0 )
				{
					if( deco_particle[MAX_TURN_PARTICLE-1].first_Pos.x == 0 )   // 맨처음에만...
					{
						float rnd_xPos = RandomNum(-2.5f, 2.5f);
						float rnd_zPos = RandomNum(-2.5f, 2.5f);

						deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x + rnd_xPos,
																vDrawPos.y,
																vDrawPos.z + rnd_zPos );

						deco_particle[i].direction = D3DVECTOR( RandomNum(-1.5f, 1.5f), 0.0f, RandomNum(-1.5f, 1.5f) );

					}
					else     // 맨처음빼고 쭉~
					{
						float old_xPos = deco_particle[MAX_TURN_PARTICLE-1].first_Pos.x - vDrawPos.x;
						float old_zPos = deco_particle[MAX_TURN_PARTICLE-1].first_Pos.z - vDrawPos.z;

						deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x + old_xPos,
																vDrawPos.y,
																vDrawPos.z + old_zPos );

						if( old_xPos >= 2.5f )
							deco_particle[i].direction.x = RandomNum(-1.5f, 0.0f);
						else if( old_xPos <= -2.5f )
							deco_particle[i].direction.x = RandomNum(0.0f, 1.5f);

						if( old_zPos >= 2.5f )
							deco_particle[i].direction.z = RandomNum(-1.5f, 0.0f);
						else if( old_zPos <= -2.5f )
							deco_particle[i].direction.z = RandomNum(0.0f, 1.5f);
					}
				}
				else
				{
					deco_particle[i].first_Pos = D3DVECTOR( deco_particle[0].first_Pos.x + (deco_particle[0].direction.x * i) + RandomNum(-0.5f, 0.5f),
															deco_particle[0].first_Pos.y,
															deco_particle[0].first_Pos.z + (deco_particle[0].direction.z * i) + RandomNum(-0.5f, 0.5f) );

					deco_particle[i].direction = D3DVECTOR( RandomNum(-1.5f, 1.5f), 0.0f, RandomNum(-1.5f, 1.5f) );
				}

				deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
				deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.001f, 0.0f );
			}
			else
			{
				deco_particle[i].fade = 1.0f - ( ((float)deco_particle[i].life_time - (float)deco_particle[i].cur_time) / (float)deco_particle[i].life_time);
				if( deco_particle[i].fade > 0.3f )
					deco_particle[i].fade = 0.3f;

				deco_particle[i].red = deco_particle[i].fade;
				deco_particle[i].green = deco_particle[i].fade;
				deco_particle[i].blue = deco_particle[i].fade;

				deco_particle[i].scale = ( ( (float)deco_particle[i].life_time - (float)deco_particle[i].cur_time ) * 0.015f );

				if( effect_sort.nCurFrame != 0 )
					deco_particle[i].cur_Pos += deco_particle[i].gravity;
			}
		}
	}
}


void CFxTurnParticle::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxTurnParticle::DeleteRes()
{}

BOOL CFxTurnParticle::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !turn )
		return true;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

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

	Update_Turn_Particle(effect_sort);
	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	for( int i = 0; i < MAX_TURN_PARTICLE; i++ )
	{
		if( deco_particle[i].start_time == 0 )
		{
			turn->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
			turn->SetRotationX( g_PI * 0.5f );
			turn->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
			turn->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
			turn->Render();
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	effect_sort.nCurFrame = 0;

	return FALSE;
}


