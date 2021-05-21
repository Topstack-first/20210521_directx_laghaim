#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxDstone.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDstone::CFxDstone()
{
	hit					= NULL;
	hit2				= NULL;
	hit3				= NULL;
	hit4				= NULL;

	m_fScale			= 0.0f;
	m_fFade				= 1.0f;


	hit = new CRectPlane;
	hit2 = new CRectPlane;
	hit3 = new CRectPlane;
	hit4 = new CRectPlane;

	if( hit )	hit->Create( 10.0f, 10.0f, FALSE );
	if( hit2 )	hit2->Create( 10.0f, 10.0f, FALSE );
	if( hit3 )	hit3->Create( 10.0f, 10.0f, FALSE );
	if( hit4 )	hit4->Create( 10.0f, 10.0f, FALSE );
}

CFxDstone::~CFxDstone()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit2);
	SAFE_DELETE(hit3);
	SAFE_DELETE(hit4);
}


//----------------------------------------------------------------------------
//-- 디바인 스톤의 푝발 빌보드 효과
//----------------------------------------------------------------------------
//-- 2004.12.06
//-- Lyul
BOOL CFxDstone::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;
	//-- 현재 프래임이 17보다 작으면( 땅에 치기 전이라면)미안하다 하드코딩

	if(effect_sort.nCurFrame >= FX_DSTONE_FRAMECOUNT)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 2.0f;

	m_fScale = (effect_sort.nCurFrame - 16) * 3;
	m_fFade = 1.0f - (m_fScale * 0.15f);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//-- Vector
	D3DVECTOR	vViewVec;
	D3DVECTOR	vtestVec;

	vtestVec.dvX=0;
	vtestVec.dvY =-10;
	vtestVec.dvZ = 0;
	vViewVec = pCMyApp->GetEyeDirection();

	// 거리를 떨어뜨린다.
	D3DVECTOR	vAdj;
	vAdj = effect_sort.vTargetPos - effect_sort.vPos;
	vAdj = Normalize(vAdj);

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vTargetPos.x, effect_sort.vTargetPos.y + 4.0f, effect_sort.vTargetPos.z  );

	if(effect_sort.nCurFrame > 16 && effect_sort.nCurFrame <25)
	{
		m_fScale = (effect_sort.nCurFrame - 14);
		//-- Set Texture
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKILL_DSTONE_1 ] ) );
		//-- Dev1
		if( hit2 )
		{
			hit2->SetBillBoard(vtestVec, FALSE, 0.0f);	 //-- Make Billboard
			hit2->Scale(m_fScale, m_fScale, 0.0f);	//-- Scale
			hit2->SetColor(1.0f, 1.0f, 1.0f, 0.4f);	//-- color
			hit2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit2->Render();
		}
	}

	if(effect_sort.nCurFrame > 16 && effect_sort.nCurFrame <20)
	{
		m_fScale = (effect_sort.nCurFrame - 15) * 2;
		//-- Hitting 효과(Billboard)
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKILL_DSTONE_2 ] ) );
		if( hit )
		{
			hit->SetBillBoard(vViewVec, FALSE, 0.0f);	 //-- Make Billboard
			hit->Scale(m_fScale, m_fScale, 0.0f);	//-- Scale
			hit->SetColor(1.0f, 1.0f, 1.0f, 0.4f);	//-- color
			hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit->Render();
		}
	}

	//--Restore Render State
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

