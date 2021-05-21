#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkMob.h"
#include "NkCharacter.h"
#include "ImageDefine.h"
#include "skill.h"
#include "FxLifeExpansion.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxLifeExpansion::CFxLifeExpansion()
{
	memset(m_fPlaneFade, 0, sizeof(m_fPlaneFade));

	m_pPlane = new CRectPlane;
	if( m_pPlane )
		m_pPlane->Create( 2.5f, 2.5f, FALSE );

	for( int i = 0 ; i < 8 ; i ++ )
		m_fCurHeight[i] = 0.0f;
	m_nOldFrame = 0;
}

CFxLifeExpansion::~CFxLifeExpansion()
{
	DeleteRes();
}

void CFxLifeExpansion::DeleteRes()
{
	SAFE_DELETE( m_pPlane );
}

void CFxLifeExpansion::LoadRes()
{}

BOOL CFxLifeExpansion::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !effect_sort.pNkChaTo )
		return TRUE;

	if( effect_sort.pNkChaTo->m_byShowLIfeExpantion == 0 )
	{
		effect_sort.pNkChaTo->m_ShowSkillEffect = FALSE;
		effect_sort.nCurFrame = FX_LIFEEXPENSION_FRAMECOUNT;
		return TRUE;
	}
	else
	{
		if( effect_sort.nCurFrame >= 50 )
			effect_sort.nCurFrame = 0;
	}

	if( !GET_D3DDEVICE()
			&& !m_pPlane )
		return TRUE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	D3DVECTOR vDrawPos;
	if( effect_sort.pNkChaTo )
		vDrawPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);;

	/*  ///07-04-10 원석 수정. 라이프 익스펜션이 Attacked() 를 해주는 이유는 뭔가? 헬다운시 피뽕받으면 취소되는 것 때문에 막아버림. 혹시 문제생길수도 있음.
		if (effect_sort.nCurFrame >= 6
			&& effect_sort.nCurFrame < 10)
		{
			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();
		}
	*/

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIFEEXPANSION ] ) );

	D3DVECTOR	vViewVec = pCMyApp->GetEyeDirection();
	float scale = 10.0f + effect_sort.nCurFrame * 2.0f;

	for( int i = 0 ; i < 8 ; i ++ )
	{
		m_fCurHeight[ i ] += ( effect_sort.Index + i * 0.2f ) * 0.15f;
		if( m_fCurHeight[ i ] > 23.0f )
			m_fCurHeight[ i ] = 7.0f;
		m_fPlaneFade[ i ] = 1.0f - ((m_fCurHeight[ i ] - 7.0f) / (float)15);
	}

	if( m_pPlane )
	{
		m_pPlane->SetColor(0.8f, 0.8f, 0.8f, m_fPlaneFade[0]);
		m_pPlane->Scale( m_fPlaneFade[0], m_fPlaneFade[0], m_fPlaneFade[0] );
		m_pPlane->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_pPlane->RotateZ( effect_sort.nCurFrame );

		m_pPlane->SetTranslationXYZ(vDrawPos.x + 4.0f, vDrawPos.y + m_fCurHeight[0], vDrawPos.z);
		m_pPlane->RotateY( effect_sort.nCurFrame / 5 );
		m_pPlane->Render();

		m_pPlane->SetTranslationXYZ(vDrawPos.x - 4.0f, vDrawPos.y + m_fCurHeight[7], vDrawPos.z);
		m_pPlane->Render();

		m_pPlane->SetColor(1.0f, 0.7f, 0.7f, m_fPlaneFade[1]);

		m_pPlane->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fCurHeight[2], vDrawPos.z + 4.0f);
		m_pPlane->RotateY( effect_sort.nCurFrame );
		m_pPlane->Render();

		m_pPlane->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fCurHeight[5], vDrawPos.z - 4.0f);
		m_pPlane->Render();

		m_pPlane->SetColor(0.7f, 0.7f, 1.0f, m_fPlaneFade[2]);

		m_pPlane->SetTranslationXYZ(vDrawPos.x + 3.0f, vDrawPos.y + m_fCurHeight[3], vDrawPos.z + 3.0f);
		m_pPlane->RotateY( effect_sort.nCurFrame );
		m_pPlane->Render();

		m_pPlane->SetTranslationXYZ(vDrawPos.x - 3.0f, vDrawPos.y + m_fCurHeight[6], vDrawPos.z - 3.0f);
		m_pPlane->Render();

		m_pPlane->SetColor(0.7f, 1.0f, 0.7f, m_fPlaneFade[3]);

		m_pPlane->SetTranslationXYZ(vDrawPos.x - 3.0f, vDrawPos.y + m_fCurHeight[1], vDrawPos.z + 3.0f);
		m_pPlane->RotateY( effect_sort.nCurFrame );
		m_pPlane->Render();

		m_pPlane->SetTranslationXYZ(vDrawPos.x + 3.0f, vDrawPos.y + m_fCurHeight[4], vDrawPos.z - 3.0f);
		m_pPlane->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);

	return FALSE;
}
