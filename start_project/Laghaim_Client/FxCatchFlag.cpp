#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxCatchFlag::CFxCatchFlag()
{
	catch_flag				= NULL;
	m_nTotalFrame			= FX_CATCHFLAG_FRAMECOUNT;

	Create();
}

CFxCatchFlag::~CFxCatchFlag()
{
	SAFE_DELETE(catch_flag);
}

void CFxCatchFlag::Create()
{
	SAFE_DELETE(catch_flag);

	catch_flag = new CRectPlane;
	if( catch_flag )
		catch_flag->Create( 12.0f, 12.0f, FALSE );
}

void CFxCatchFlag::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxCatchFlag::DeleteRes()
{}

BOOL CFxCatchFlag::Render(EffectSort &effect_sort)
{
	if( !effect_sort.pNkChaTo || !GET_D3DDEVICE() || !catch_flag)
		return TRUE;

	if( effect_sort.pNkChaTo->m_nTeamMemGrade != TEAM_BATTLE_AFLAG &&
			effect_sort.pNkChaTo->m_nTeamMemGrade != TEAM_BATTLE_BFLAG )
	{
		effect_sort.pNkChaTo->m_bCatchFlagShow = FALSE;
		effect_sort.nCurFrame = m_nTotalFrame;
		return TRUE;
	}
	else
	{
		if( effect_sort.nCurFrame >= m_nTotalFrame )
			effect_sort.nCurFrame = 0;
	}

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

	D3DVECTOR	vDrawVec;
	if( effect_sort.pNkChaTo )
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wz), effect_sort.pNkChaTo->m_wz);

	// 0 불칸, 1 카이립톤, 2 에이디아, 3 휴먼, 4 하이브리더
	float Add_Height[NUM_RACE][NUM_SEX] = {	34.0f, 31.5f,
											32.5f, 31.5f,
											27.0f, 27.0f,
											32.5f, 31.5f,
											35.0f, 31.5f,
											35.0f, 31.5f
										  };


	if( g_pCapsyongTexture )
	{
		if( effect_sort.pNkChaTo->m_nTeamMemGrade == TEAM_BATTLE_AFLAG )
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CATCHFLAG_FLAGA ] ) );
		else if( effect_sort.pNkChaTo->m_nTeamMemGrade == TEAM_BATTLE_BFLAG )
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CATCHFLAG_FLAGB ] ) );
	}

	catch_flag->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	catch_flag->SetBillBoard( vViewVec, FALSE, 0.0f );
	catch_flag->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 5.0f + Add_Height[effect_sort.pNkChaTo->m_Race][effect_sort.pNkChaTo->m_Sex], vDrawVec.z);
	catch_flag->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

