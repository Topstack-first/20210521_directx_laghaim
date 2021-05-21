#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "DecoObj.h"

typedef struct Deco_Particle Deco_Particle;



// nate 2004 - 8 ImageManager
#define FX_LIGHTPARTICLE_MAX_LIGHT_VERTEXS 4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxLightParticle::CFxLightParticle()
{
	m_light	= NULL;
	Create();
}

CFxLightParticle::~CFxLightParticle()
{
	SAFE_DELETE(m_light);
}

void CFxLightParticle::Create()
{
	m_light = new D3DLVERTEX[ FX_LIGHTPARTICLE_MAX_LIGHT_VERTEXS ];
}


void CFxLightParticle::Update_Light_Particle(EffectSort &effect_sort)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	for( int i = 0; i < MAX_LIGHT_PARTICLE; i++ )
	{
		deco_particle[i].start_time -= effect_sort.nCurFrame;
		if( deco_particle[i].start_time < 0 ) deco_particle[i].start_time = 0;

		if( deco_particle[i].start_time == 0 )
		{
			deco_particle[i].cur_time -= effect_sort.nCurFrame;
			if( deco_particle[i].cur_time < 0 ) deco_particle[i].cur_time = 0;

			if( deco_particle[i].cur_time == 0 )
			{
				deco_particle[i].life_time = RandomNum( 30, 30 ) ;

				if( i != 0 )
				{
					if( deco_particle[i-1].cur_time < 6 )
						deco_particle[i].start_time = (6 - deco_particle[i-1].cur_time );
					if( deco_particle[i-1].cur_time > 6 )
						deco_particle[i].life_time -= (deco_particle[i-1].cur_time - 6);
				}

				deco_particle[i].cur_time = deco_particle[i].life_time;
				deco_particle[i].scale = 3.0f;
				deco_particle[i].fade = 1.0f;

				deco_particle[i].red = 0.3f;
				deco_particle[i].green = 1.0f;
				deco_particle[i].blue = 0.3f;
				deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x + RandomNum(-7.0f, 7.0f),
														vDrawPos.y,
														vDrawPos.z + RandomNum(-7.0f, 7.0f) );
				deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
				deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.275f, 0.280f), 0.0f );
				deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.05f, 0.0f );
			}
			else
			{
				deco_particle[i].fade = 1.0f - ( ((float)deco_particle[i].life_time - (float)deco_particle[i].cur_time) / (float)deco_particle[i].life_time );
				deco_particle[i].red = (0.3f - ( 0.3f * ( ((float)deco_particle[i].life_time - (float)deco_particle[i].cur_time) / (float)deco_particle[i].life_time) ));// * 0.5f;
				deco_particle[i].green = (1.0f - ( 1.0f * ( ((float)deco_particle[i].life_time - (float)deco_particle[i].cur_time) / (float)deco_particle[i].life_time) ));// * 0.5f;
				deco_particle[i].blue = (0.3f - ( 0.3f * ( ((float)deco_particle[i].life_time - (float)deco_particle[i].cur_time) / (float)deco_particle[i].life_time) ));// * 0.5f;

				if( effect_sort.nCurFrame != 0 )
				{
					deco_particle[i].direction += (deco_particle[i].gravity * effect_sort.nCurFrame);
					deco_particle[i].cur_Pos += (deco_particle[i].direction * effect_sort.nCurFrame);
				}

			}
		}

	}
}

void CFxLightParticle::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxLightParticle::DeleteRes()
{}

BOOL CFxLightParticle::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
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

	Update_Light_Particle(effect_sort);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	for( int i = 0; i < MAX_LIGHT_PARTICLE; i++ )
	{
		if( deco_particle[i].start_time == 0 )
		{
			D3DVECTOR	vDirVec;
			vDirVec = deco_particle[i].direction;
			vDirVec = Normalize(vDirVec);

			D3DVECTOR	vRight = CrossProduct( vViewVec, vDirVec );
			vRight = Normalize(vRight);

			vDirVec *= (deco_particle[i].scale * 5.0f);
			vRight *= (deco_particle[i].scale * 2.0f);

			D3DVECTOR	vTargetVec, vFromVec;
			vTargetVec = deco_particle[i].cur_Pos + vDirVec;
			vFromVec = deco_particle[i].cur_Pos - vDirVec;

			m_light[0] = D3DLVERTEX( vFromVec - vRight, D3DRGBA(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade), 0, 0.0f, 0.0f );
			m_light[1] = D3DLVERTEX( vFromVec + vRight, D3DRGBA(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade), 0, 0.0f, 1.0f );
			m_light[2] = D3DLVERTEX( vTargetVec - vRight, D3DRGBA(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade), 0, 1.0f, 0.0f );
			m_light[3] = D3DLVERTEX( vTargetVec + vRight, D3DRGBA(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade), 0, 1.0f, 1.0f );

			D3DMATRIX				matWorld;
			D3DUtil_SetIdentityMatrix(matWorld);
			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
											m_light,
											// nate 2004 - 8 ImageManager
											FX_LIGHTPARTICLE_MAX_LIGHT_VERTEXS,
											0 );
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	effect_sort.nCurFrame = 0;

	return FALSE;
}


