#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "FxRunSmoke.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxRunSmoke::CFxRunSmoke()
{
	smoke					= NULL;

	Create();
}

CFxRunSmoke::~CFxRunSmoke()
{
	SAFE_DELETE(smoke);
}

void CFxRunSmoke::Create()
{
	SAFE_DELETE(smoke);

	smoke = new CRectPlane;
	if( smoke )
		smoke->Create( 20.0f, 20.0f, FALSE );
}

void CFxRunSmoke::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxRunSmoke::DeleteRes()
{}

BOOL CFxRunSmoke::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !smoke )
		return true;

	if(effect_sort.nCurFrame >= FX_RUNSMOKE_FRAMECOUNT)
		return TRUE;

	float fScale = (3.0f * effect_sort.nCurFrame) / FX_RUNSMOKE_FRAMECOUNT;
	float fFade = 1.0f - (fScale * 0.3f);

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
	vDrawVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_RUNSMOKE ] ) );

	float red, green, blue;
	red = 0.75f;// - (0.4f * fScale * 0.3f);
	green = 0.75f;// - (0.4f * fScale * 0.3f);
	blue = 0.5f;// - (0.95f * fScale * 0.3f);

	smoke->SetColor(red, green, blue, fFade);

//	smoke->SetBillBoard( vViewVec, TRUE, 0.0f );
	smoke->SetBillBoard( vViewVec, FALSE, 0.0f );	// y축 빌보드로 할 이유가있는가? 위에서 보면 이상해 보여서 수정(07-03-06 원석)
	smoke->Scale( fScale, fScale, 1.0f );
	smoke->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + (fScale*0.15f) + 5.5f, vDrawVec.z);
	smoke->Render();

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

