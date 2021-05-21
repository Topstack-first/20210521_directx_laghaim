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
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "FxDropCandy.h"
#include "Country.h"

extern DWORD g_dwClientCountry;

// nate 2004 - 7 글로벌 ShadowRect사용


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDropCandy::CFxDropCandy()
{
	m_pLolo = NULL;
	smoke = NULL;
	smoke = new CRectPlane;
	if( smoke )
		smoke->Create(15.0f, 15.0f, FALSE);
}

CFxDropCandy::~CFxDropCandy()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(smoke);
}

//-- Candy 의 Resource 를 Load 한다.
//-- 2004.03.29
//-- kind - 0 : Candy
//--        1 : Apple
// nate 2004 - 8 ImageManager
void CFxDropCandy::LoadRes( int kind )
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		if( m_pLolo )
		{
			m_pLolo->SetIndexedTexture( g_pCapsyongTexture );

			char filename[128] = {0,};
			if( kind ==0 )
			{
				if( g_dwClientCountry != CTRY_JPN)	strcpy( filename , "data/effect/lolos/gi_attack_candy.LOL" );
				else								strcpy( filename , "data/effect/lolos/Candy_color_RG.LOL" );
			}
			else if ( kind ==1 )
				strcpy( filename , "data/effect/lolos/apple_drop.LOL" );
			else if ( kind ==2 )
				strcpy( filename , "data/effect/lolos/Candy_color_GG.LOL" );
			else if ( kind ==3 )
				strcpy( filename , "data/effect/lolos/Candy_color_BG.LOL" );
			else if ( kind ==4 )
				strcpy( filename , "data/effect/lolos/apple_droppvp.LOL" );
			m_pLolo->Read(filename);
		}
	}

}

void CFxDropCandy::DeleteRes()
{}

void CFxDropCandy::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite, dwAlpha;
	DWORD dwAlphaTest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlpha);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest); //-- lyul

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE); //-- lyul


	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vPos;

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 10)
		{
			if (effect_sort.pMobTo)
			{
				effect_sort.pMobTo->Attacked();
			}
			else if (effect_sort.pNkChaTo)
			{
				effect_sort.pNkChaTo->Attacked();
			}
		}
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );


		// nate 2004 - 7 글로벌 ShadowRect사용
		if( g_pShadowRect )
		{
			// nate 2004 - 7 - image
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DROPCANDY_BASE ] ) ;
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 55 + (effect_sort.nCurFrame * 0.1f), FALSE);		// 부울값은 시간인가 아닌가
			g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
		}

		DWORD  dwTextureState[3];

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DROPCANDY_SMOKE ] ) ;
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		float up_height = effect_sort.nCurFrame * 0.5f + 7.0f;
		float smoke_fade = 1.0f - (effect_sort.nCurFrame / (float)FX_DROPCANDY_FRAMECOUNT);

		if( smoke )
		{
			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->SetTranslationXYZ(vDrawPos.x + 10.0f, vDrawPos.y + up_height, vDrawPos.z);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + up_height, vDrawPos.z + 10.0f);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->SetTranslationXYZ(vDrawPos.x - 10.0f, vDrawPos.y + up_height, vDrawPos.z);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + up_height, vDrawPos.z - 10.0f);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->SetTranslationXYZ(vDrawPos.x + 10.0f, vDrawPos.y + up_height, vDrawPos.z + 10.0f);
			smoke->Render();

			smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
			smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
			smoke->SetTranslationXYZ(vDrawPos.x - 10.0f, vDrawPos.y + up_height, vDrawPos.z - 10.0f);
			smoke->Render();
		}
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlpha);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);
}

CFxDropStarCandy::CFxDropStarCandy()
{
	m_pLolo = NULL;
}

CFxDropStarCandy::~CFxDropStarCandy()
{
	SAFE_DELETE(m_pLolo);
}

void CFxDropStarCandy::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		if( m_pLolo )
		{
			m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
			m_pLolo->Read("data/effect/lolos/star_candy.LOL");
		}
	}



}

void CFxDropStarCandy::DeleteRes()
{
}

BOOL CFxDropStarCandy::IsActive(int nIndex, int nFrame)
{
	//별사탕 발생 조건
	if (nFrame < 5)
	{
		return (nIndex < 13) ? TRUE : FALSE;
	}
	else if(nFrame < 12)
	{
		return (nIndex < 24) ? TRUE : FALSE;
	}
	else if(nFrame < 19)
	{
		return (nIndex < 37) ? TRUE : FALSE;
	}
	else return TRUE;
}

void CFxDropStarCandy::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite, dwAlpha;
	DWORD dwAlphaTest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlpha);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest); //-- lyul

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE); //-- lyul

	if (m_pLolo)
	{

		for(int i = 0; i < FX_DROP_STARCANDY_COUNT; ++i)
		{
			if (IsActive(i, effect_sort.nCurFrame))
			{
				if (effect_sort.pParticle[i].start_time == 0)
				{
					effect_sort.pParticle[i].start_time = effect_sort.nCurFrame;
				}

				D3DUtil_SetIdentityMatrix( matTrans );
				D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pParticle[i].first_Pos.x,
											effect_sort.pParticle[i].first_Pos.y,
											effect_sort.pParticle[i].first_Pos.z);
				pCMyApp->SetViewMatrix(matView);

				int n = effect_sort.nCurFrame - effect_sort.pParticle[i].start_time;
				int m = n * n;
				//초기속도 + 중력 가속도 + 공기 저항
				effect_sort.pParticle[i].first_Pos.y -= 6.0 + 0.06f * m - 0.3f * n ;
				GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
				GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
				m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
			}
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlpha);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);
}


