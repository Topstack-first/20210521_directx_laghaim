#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxWeaponEffects::CFxWeaponEffects()
{}

CFxWeaponEffects::~CFxWeaponEffects()
{}

void CFxWeaponEffects::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxWeaponEffects::DeleteRes()
{}

void CFxWeaponEffects::Update_Weapon_Particle(EffectSort &effect_sort, int linetype)
{

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.pNkChaTo->m_NkCha_Particle;

	D3DVECTOR	vDrawPos[2];
	vDrawPos[0] = D3DVECTOR( effect_sort.pNkChaTo->m_TraceLine[0].x,
							 effect_sort.pNkChaTo->m_TraceLine[0].y,
							 effect_sort.pNkChaTo->m_TraceLine[0].z );
	vDrawPos[1] = D3DVECTOR( effect_sort.pNkChaTo->m_TraceLine[1].x,
							 effect_sort.pNkChaTo->m_TraceLine[1].y,
							 effect_sort.pNkChaTo->m_TraceLine[1].z );

	if(	linetype == TRACELINE_NORMAL
			|| linetype == TRACELINE_ATTACK
			|| linetype == TRACELINE_JUMPATTACK
			|| linetype == TRACELINE_ROTATEATTACK )
	{

		int rnd_start_time = 0;

		for( int i = 0; i < MAX_NKCHA_PARTICLE; i++ )
		{
			deco_particle[i].start_time -= effect_sort.nCurFrame;

			if( deco_particle[i].start_time < 0 )
				deco_particle[i].start_time = 0;

			if( deco_particle[i].start_time == 0 )
			{
				deco_particle[i].cur_time -= effect_sort.nCurFrame;

				if( deco_particle[i].cur_time < 0 )
					deco_particle[i].cur_time = 0;

				if( deco_particle[i].cur_time == 0 )
				{
					deco_particle[i].start_time = rnd_start_time;
					deco_particle[i].life_time = 0;

					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 0.7f;
					deco_particle[i].fade = 0.5f;

					deco_particle[i].red = 0.7f;
					deco_particle[i].green = 0.2f;
					deco_particle[i].blue = 0.2f;

					D3DVECTOR	vDirVec;
					vDirVec = vDrawPos[0] - vDrawPos[1];

					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos[0].x - (vDirVec.x*i*0.1f) + RandomNum(-1.0f, 1.0f),
															vDrawPos[0].y - (vDirVec.y*i*0.1f) + RandomNum(-1.0f, 1.0f),
															vDrawPos[0].z - (vDirVec.z*i*0.1f) + RandomNum(-1.0f, 1.0f) ) ;

					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );

					// 맞았을때 파편이 퍼지는 효과
					deco_particle[i].direction = D3DVECTOR( RandomNum(-0.25f, 0.25f), RandomNum(-0.25f, 0.25f), RandomNum(-0.25f, 0.25f) );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.05f, 0.0f );
				}
				else
				{
					deco_particle[i].scale = 1.0f + ( ( (float)deco_particle[i].life_time - (float)deco_particle[i].cur_time ) * 0.05f );

					// 렌더링 ONE_ONE
					deco_particle[i].red = (0.7f - ( 0.7f * deco_particle[i].fade * 0.75f));
					deco_particle[i].green = (0.2f - ( 0.2f * deco_particle[i].fade * 0.75f));
					deco_particle[i].blue = (0.2f - ( 0.2f * deco_particle[i].fade * 0.75f));

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

//-- 검의 궤적을 그린다.
//-- 2005.01.13
//-- 프리크의 Bleed 전용 붉은 색 Texture 를 사용하는 궤적을 추가한다.
BOOL CFxWeaponEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pNkChaTo )
		return TRUE;

	if( effect_sort.pNkChaTo->m_DrawWeaponEffects == NONE )
	{
		effect_sort.nCurFrame = 0;
		effect_sort.Index = 0;
		return FALSE;
	}

	DWORD dwlingting, dwRenState[2];
	DWORD dwTextureState[3];

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwlingting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	D3DMATRIX oldmatWorld,matWorld;
	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &oldmatWorld );

	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	//	int effect_type = 0;
	//--------------------------------------------------------------------------
	//-- 각 형식에 따른 궤적을 그린다.
	switch( effect_sort.pNkChaTo->m_DrawWeaponEffects )
	{
	//-- Bleed 전용 궤적
	case TRACELINE_ATTACK_BLEED:
	case TRACELINE_BURNINGCRASH:	// nate 2005-06-16 : Burning Crash - 궤적 그리기
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WEAPON_TRACE2 ] ) );
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,&effect_sort.pNkChaTo->m_TraceLine, effect_sort.pNkChaTo->m_drawLineNum, 0 );
		break;
	//-- 일반적인 검의 궤적
	case TRACELINE_ATTACK:
	case TRACELINE_JUMPATTACK:
	case TRACELINE_ROTATEATTACK:
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WEAPON_TRACE ] ) );
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,&effect_sort.pNkChaTo->m_TraceLine, effect_sort.pNkChaTo->m_drawLineNum, 0 );
		break;
	case TRACELINE_SHOOTATTACK:
		D3DUtil_SetIdentityMatrix(matWorld);
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&effect_sort.pNkChaTo->m_TraceLine, effect_sort.pNkChaTo->m_drawLineNum, 0 );
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										&effect_sort.pNkChaTo->m_TraceLine, effect_sort.pNkChaTo->m_drawLineNum, 0 );
		break;

	case TRACELINE_DASHSLASH: ///하이콤보
	case TRACELINE_SLASHSHADOW:
		// 좌우가 절반을 기준으로 나누어져있다.
		if( effect_sort.pNkChaTo && effect_sort.pNkChaTo->m_bBleed )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WEAPON_TRACE2 ] ) );
		else
			GET_D3DDEVICE()->SetTexture(0, NULL );
//				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WEAPON_TRACE ] ) );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &effect_sort.pNkChaTo->m_TraceLine, effect_sort.pNkChaTo->m_drawLineNum, 0 );  // 좌측검의 궤적
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &(effect_sort.pNkChaTo->m_TraceLine[((MAX_TRACELINE_FRAME*4)+4)/2]), effect_sort.pNkChaTo->m_drawLineNum, 0 ); // 우측 검의 궤적.
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

		break;

	default:
		break;
	}

	effect_sort.nCurFrame = 0;
	effect_sort.Index = 0;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &oldmatWorld );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );

	return FALSE;
}

