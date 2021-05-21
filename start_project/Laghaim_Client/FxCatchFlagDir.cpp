#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "uimgr.h"
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxCatchFlagDir::CFxCatchFlagDir()
{
	catch_flag				= NULL;
	Create();
}

CFxCatchFlagDir::~CFxCatchFlagDir()
{
	SAFE_DELETE(catch_flag);
}

void CFxCatchFlagDir::Create()
{
	SAFE_DELETE(catch_flag);

	catch_flag = new CRectPlane;
	if( catch_flag )
		catch_flag->Create( 10.0f, 10.0f, FALSE );
}

void CFxCatchFlagDir::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxCatchFlagDir::DeleteRes()
{}

BOOL CFxCatchFlagDir::Render(EffectSort &effect_sort)
{
	if( !effect_sort.pNkChaTo || !GET_D3DDEVICE() || !catch_flag)
		return TRUE;

	if( pCMyApp->m_pUIMgr->m_bOnBattle &&
			(effect_sort.pNkChaTo->m_bFlagOwner[0] ||
			 effect_sort.pNkChaTo->m_bFlagOwner[1] ||
			 effect_sort.pNkChaTo->m_bFlagOwner[2] ||
			 effect_sort.pNkChaTo->m_bFlagOwner[3]) )
	{
		if( effect_sort.nCurFrame >= FX_CATCHFLAGDIR_FRAMECOUNT )
			effect_sort.nCurFrame = 0;
	}
	else
	{
		effect_sort.pNkChaTo->m_bCatchFlagShow = FALSE;
		effect_sort.nCurFrame = FX_CATCHFLAGDIR_FRAMECOUNT;
		return TRUE;
	}

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

	D3DVECTOR	vDrawVec;
	if( effect_sort.pNkChaTo )
		vDrawVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wz), effect_sort.pNkChaTo->m_wz);

	// 0 불칸, 1 카이립톤, 2 에이디아, 3 휴먼, 4 하이
	// pCha->m_Sex, pCha->m_Race;

	float Add_Height[NUM_RACE][NUM_SEX] = {	29.0f, 26.5f,
											27.5f, 26.5f,
											22.0f, 22.0f,
											27.5f, 26.5f,
											30.0f, 26.5f,
											30.0f, 26.5f
										  };	// nate 2005-06-28 : 하이브리더 화이트혼 외성 깃발 높이 설정

//===================================================
	// nate 2004 - 7 : Image Manager
	for( int i = 0 ; i < 4 ; i++ )
	{
		if( effect_sort.pNkChaTo->m_bFlagOwner[i] )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ i + EFF_DIR_FLAG1_TEX ] ));
			break;
		}
	}
//===================================================

	catch_flag->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	catch_flag->SetBillBoard( vViewVec, FALSE, 0.0f );
	catch_flag->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 5.0f + Add_Height[effect_sort.pNkChaTo->m_Race][effect_sort.pNkChaTo->m_Sex], vDrawVec.z);
	catch_flag->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

