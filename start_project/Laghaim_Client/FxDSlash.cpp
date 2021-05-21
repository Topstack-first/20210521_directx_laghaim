#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxDslash.h"
#include "NkMob.h"
#include "NkCharacter.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDSlash::CFxDSlash()
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

CFxDSlash::~CFxDSlash()
{
	SAFE_DELETE(hit);
	SAFE_DELETE(hit2);
	SAFE_DELETE(hit3);
	SAFE_DELETE(hit4);
}


//----------------------------------------------------------------------------
//-- 더블 슬래쉬
//----------------------------------------------------------------------------
//-- 2004.12.06
BOOL CFxDSlash::Render(EffectSort &effect_sort)
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
	if(effect_sort.num)
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA);
	else
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
	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vTargetPos;
	vDrawPos.y += 14.0f;
	D3DVECTOR vAdj = Normalize(effect_sort.vTargetPos);
	//----------------------------------------------------------------------
	//-- 유효 Frame
	if(effect_sort.nCurFrame > 16 && effect_sort.nCurFrame <22)
	{
		float fade;
		if(effect_sort.nCurFrame > 17)
			fade = 0.8f - float(effect_sort.nCurFrame -18) / 5.0f;
		else
			fade =0.8f;
		//----------------------------------------------------------------------
		//-- Board 1
		m_fScale = float(effect_sort.nCurFrame - 15) *2.1f;

		if(effect_sort.num) m_fScale*= 1.3f; //-- 몹이 쓴 Dslash

		//-- Set Texture
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKILL_DSLASH_1 ] ) );
		//-- Dev1
		if( hit2 )
		{
			hit2->SetBillBoard(vtestVec, false, 0.0f);       //-- Make Billboard
			hit2->Scale(m_fScale, m_fScale, 0.0f);           //-- Scale

			if(effect_sort.num)
				hit2->SetColor(1.0f, 0.0f, 0.0f, fade);          //-- color
			else
				hit2->SetColor(1.0f, 1.0f, 1.0f, fade);          //-- color

			hit2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit2->Render();
		}

		//----------------------------------------------------------------------
		//-- Board 2
		if( hit )
		{
			hit->SetBillBoard(vViewVec, false, 0.0f);	 //-- Make Billboard
			hit->Scale(m_fScale, m_fScale, 0.0f);	//-- Scale

			if(effect_sort.num)
				hit->SetColor(1.0f, 0.0f, 0.0f, fade);          //-- color
			else
				hit->SetColor(1.0f, 1.0f, 1.0f, fade);          //-- color

			hit->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			hit->Render();
		}
	}
	//--Restore Render State
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

