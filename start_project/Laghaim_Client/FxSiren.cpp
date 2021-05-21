#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "CapSyong.h"

extern int					g_EffectSound[];
extern float				g_fDSDistance;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSiren::CFxSiren()
{
	m_pLolo	= NULL;

	m_dwStartime = 0;
	m_curframe   = 0;

	m_effectAniframe = 0;
	m_effectframe	 = 0;

	m_fEffectInterval = 0;
	m_fSirenInterval  = 0;

	m_pEffectLight = NULL;

	m_color = 0;	// red default
	m_TextrIndexSiren = -1;
}

CFxSiren::~CFxSiren()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE_ARRAY(m_pEffectLight);

	m_color = 0;	// red default
	m_TextrIndexSiren = -1;
}

void CFxSiren::Create(int effect_frame, int nRace, int color)
{
	switch(nRace)
	{
	case RACE_BULKAN:
		{
			m_fSirenInterval  = 3.f;
			m_fEffectInterval = 25.5f;
		}
		break;
	case RACE_KAILIPTON:
		{
			m_fSirenInterval  = -1.f;
			m_fEffectInterval = 21.5f;
		}
		break;
	case RACE_AIDIA:
		{
			m_fSirenInterval  = -4.f;
			m_fEffectInterval = 18.5f;
		}
		break;
	case RACE_HUMAN:
		{
			m_fSirenInterval  = 0.f;
			m_fEffectInterval = 22.5f;
		}
		break;
	case RACE_FREAK:
		{
			m_fSirenInterval  = 4.f;
			m_fEffectInterval = 26.5f;
		}
		break;
	case RACE_PEROM:
		{
			m_fSirenInterval  = 4.f;
			m_fEffectInterval = 26.5f;
		}
		break;
	}

	m_color = color;
	m_effectframe = effect_frame;

	m_pEffectLight = new CRectPlane[effect_frame];

	for(int i = 0; i < effect_frame; i++)
	{
		m_pEffectLight[i].Create(i, 4.0f, 4.0f, 1.f/(float)effect_frame);
	}
}

void CFxSiren::LoadRes()
{
	SAFE_DELETE(m_pLolo);

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		g_pCapsyongTexture->SetTexturePath("data/effect/textr/");
		m_pLolo->Read("data/effect/lolos/siren.LOL", TRUE);

		g_pCapsyongTexture->RestoreByIdx(m_pLolo->m_LoloObjs->m_TextureNum, pCMyApp->GetDevice());

		m_dwStartime = timeGetTime();

		g_pCapsyongTexture->SetTexturePath("data/effect/textr/");

		// 색상만 바꾸자.
		switch( m_color )
		{
		case 1: // blue color
			m_TextrIndexSiren = g_pCapsyongTexture->CreateTextureFromFile("ap_siren_blue.tga");
			break;
		case 2: // Green color
			m_TextrIndexSiren = g_pCapsyongTexture->CreateTextureFromFile("ap_siren_green.tga");
			break;
		case 3: // Yellow color
			m_TextrIndexSiren = g_pCapsyongTexture->CreateTextureFromFile("ap_siren_yellow.tga");
			break;
		}

		g_pCapsyongTexture->RestoreByIdx(m_TextrIndexSiren, pCMyApp->GetDevice());
	}



}

void CFxSiren::DeleteRes()
{

}

BOOL CFxSiren::Render(EffectSort &effect_sort, CNkCharacter* pChar)
{
	if( !GET_D3DDEVICE() )
		return FALSE;

	if(m_curframe >= m_pLolo->m_AnimCount)
	{
		m_curframe = 0;
	}

	D3DMATRIX	matWorld;
	D3DMATRIX	matTemp;

	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_SetIdentityMatrix(matTemp);

	D3DUtil_SetRotateYMatrix(matWorld,pChar->m_dir);

	matWorld._41 = pChar->m_wx;
	matWorld._42 = pChar->m_wy;
	matWorld._43 = pChar->m_wz;

	matTemp._41 = 0;
	matTemp._42 = pChar->GetBiped()->m_AniMat[1].m_Mats[1]._42;
	matTemp._43 = 0;

	matWorld._42 += m_fSirenInterval;

	DWORD dwZwri = 0, dwLight = 0;

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &dwZwri );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	if(timeGetTime() - m_dwStartime > 100)
	{
		m_dwStartime = timeGetTime();

		m_curframe++;

		m_effectAniframe++;
	}

	switch( m_color )
	{
	case 0: // red
		m_pLolo->Render(GET_D3DDEVICE(), m_curframe, FALSE, FALSE, FALSE, &matWorld);
		break;
	case 1: // blue color
	case 2:	// Green color
	case 3: // Yellow
		m_pLolo->Render2(GET_D3DDEVICE(), m_TextrIndexSiren, m_curframe, FALSE, FALSE, FALSE, &matWorld);
		break;
	}

	if(m_effectAniframe >= m_effectframe)
		m_effectAniframe = 0;

	RenderEffect(pChar);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING    , dwLight);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZwri);

	return TRUE;
}

BOOL CFxSiren::RenderEffect(CNkCharacter* pChar)
{
	DWORD dwZWrite		= 0;
	DWORD dwZEnable		= 0;
	DWORD dWAlphaEnabel = 0;
	DWORD dWAlphaTest   = 0;
	DWORD dwLight		= 0;
	DWORD dwBlendOne    = 0;
	DWORD dwInvcolor    = 0;
	DWORD dwColor[3];

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE	   , &dwZWrite);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZENABLE		   , &dwZEnable);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dWAlphaEnabel);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE , &dWAlphaTest);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING		   , &dwLight);

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND , &dwBlendOne);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwInvcolor);

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_COLOROP  , &dwColor[0]);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColor[1]);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_COLORARG2, &dwColor[2]);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCCOLOR );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP  , D3DTOP_MODULATE2X);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_WANTED]));

	D3DVECTOR vTemp;
	D3DXMATRIX matTemp;

	D3DUtil_SetIdentityMatrix(matTemp);

	vTemp = pCMyApp->GetEyeDirection();

	switch(m_color)
	{
	case 0: // red
		m_pEffectLight[m_effectAniframe].SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 1:	// blue
		m_pEffectLight[m_effectAniframe].SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case 2:	// green
		m_pEffectLight[m_effectAniframe].SetColor(0.0f, 0.6f, 0.0f, 1.0f);
		break;
	case 3: // yellow
		m_pEffectLight[m_effectAniframe].SetColor(0.4f, 0.4f, 0.2f, 1.0f);
		break;
	}

	m_pEffectLight[m_effectAniframe].SetBillBoard( vTemp, FALSE, 0.f );

	switch(m_color)
	{
	case 0: // red
	case 1:	// blue
		m_pEffectLight[m_effectAniframe].Scale(1.0f, 1.0f, 1.0f);
		break;
	case 2:	// green
	case 3: // yellow
		m_pEffectLight[m_effectAniframe].Scale(0.8f, 0.8f, 0.8f);
		break;
	}

	m_pEffectLight[m_effectAniframe].SetTranslationXYZ(pChar->m_wx, pChar->m_wy+m_fEffectInterval, pChar->m_wz);
	m_pEffectLight[m_effectAniframe].Render();

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND , dwBlendOne);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwInvcolor);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, dwZEnable);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dWAlphaEnabel);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE , dWAlphaTest);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwLight);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP  , dwColor[0]);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColor[1]);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG2, dwColor[2]);

	GET_D3DDEVICE()->SetTexture(0, NULL);

	return TRUE;
}