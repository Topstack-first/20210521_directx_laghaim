#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "FxSumPart.h"


extern float g_fDSDistance;
extern float g_SummonScale;

extern int g_EffectSound[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSumPart::CFxSumPart()
{
	m_pLolo = NULL;
	bottem = NULL;
	bottem = new CRectPlane;
	if( bottem )
		bottem->Create(15.0f, 15.0f, FALSE);
}

CFxSumPart::~CFxSumPart()
{
	SAFE_DELETE( m_pLolo );
	SAFE_DELETE( bottem );
}

// nate 2004 - 8 ImageManager
void CFxSumPart::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/a_summon.LOL");
	}



}

void CFxSumPart::DeleteRes()
{}

void CFxSumPart::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( !effect_sort.pMobTo )
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matWorld,matTrans, matView;
	D3DMATRIX matRx, matRy;
	DWORD dwLighting, dwZWrite;

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vTargetPos;
	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	if(effect_sort.nCurFrame < 25)
	{
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

		g_SummonScale =(float)effect_sort.nCurFrame/20;

		float red, green, blue;
		float m_fScale, m_fFade;

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SUMPART_BOTTEM ] ) );

		if( effect_sort.Index >= 50 )
			effect_sort.Index = 0;

		m_fScale = ((float)25.0f-effect_sort.nCurFrame);//.Index / 50);

		m_fFade = 0.5f;// - (m_fScale/25.0f);
		red = 0.9f - (0.9f * m_fScale * 0.5f);
		green = 0.9f - (0.9f * m_fScale * 0.5f);
		blue = 0.484f - (0.484f * m_fScale * 0.5f);

		if( bottem )
		{
			bottem->SetColor(red, green, blue, m_fFade);
			bottem->SetRotationX( g_PI * 0.5f );
			bottem->RotateZ( m_fScale * 3.0f );
			bottem->Scale( 1.0f + (m_fScale*0.1f), 1.0f + (m_fScale*0.1f), 1.0f + (m_fScale*0.1f));
			bottem->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 2.0f, vDrawPos.z);
			bottem->Render();
		}

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
	}
	else
	{
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );

		g_SummonScale = 0.0f;
		effect_sort.pMobTo->SetFissionType(0);

		if (m_pLolo)
		{
			D3DUtil_SetTranslateMatrix( matWorld, effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy + 5.0f, effect_sort.pMobTo->m_wz);

			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 0.7f,0.7f, 0.7f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);

			pCMyApp->SetViewMatrix(matView);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
			m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		}
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
	}
}