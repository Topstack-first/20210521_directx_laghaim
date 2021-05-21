#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "nkMob.h"
#include "Land.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxWarGiantDie::CFxWarGiantDie()
{
	corona				= NULL;
	round				= NULL;
	round_light			= NULL;
	m_fScale			= 0.0f;
	m_fFade				= 1.0f;
	m_nTotalFrame		= FX_WARGIANTDIE_FRAMECOUNT;

	Create();
}

CFxWarGiantDie::~CFxWarGiantDie()
{
	SAFE_DELETE(corona);
	SAFE_DELETE(round);
	SAFE_DELETE(round_light);
}

void CFxWarGiantDie::Create()
{
	SAFE_DELETE(corona);
	SAFE_DELETE(round);
	SAFE_DELETE(round_light);

	corona = new CRectPlane;
	if( corona )
		corona->Create( 10.0f, 10.0f, FALSE );

	round = new CCylinderPlane;
	if( round )
		round->Create( 2.5f, 2.5f, 1.0f, 5, 0.0f, TRUE, 5.0f );

	round_light = new CCylinderPlane;
	if( round_light )
		round_light->Create( 10.0f, 10.0f, 1.0f, 20, 0.0f, TRUE, 20.0f );
}


void CFxWarGiantDie::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxWarGiantDie::DeleteRes()
{}
void CFxWarGiantDie::SetTextureNum()
{}

BOOL CFxWarGiantDie::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.num)
	{
		if(effect_sort.nCurFrame >= m_nTotalFrame)
		{
			m_fScale = 0.0f;
			m_fFade = 1.0f;
			return TRUE;
		}

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

		DWORD  dwTextureState[3];

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		D3DVECTOR	vDrawPos;
		vDrawPos = effect_sort.vPos;

		float maxScale = 2.0f;
		m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
		m_fFade = 1.0f - (m_fScale * 0.45f);

		float red, green, blue;
		red = green = blue = 1.0f;

		if( corona )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_CORONA ] ) );

			red = 0.75f - (0.75f * m_fScale * 0.45f);
			green = 1.0f - (1.0f * m_fScale * 0.45f);
			blue = 0.8f - (0.8f * m_fScale * 0.45f);

			corona->SetColor(red * 0.15f, green * 0.15f, blue * 0.15f, m_fFade * 0.15f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 5.0f );
			corona->Scale( 5.0f, 5.0f, 5.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			corona->Render();
		}

		///////////////////////////////////////////////////////////////////////////////////////
		// 밑에 마법진 나타났다 사라지는 효과

		float ground_height = 1.0f;
		float life_scale = 1.0f;
		float round_light_scale = 1.0f;

		if( effect_sort.nCurFrame <= 20 )
		{
			m_fScale = ((float)effect_sort.nCurFrame / 20);
			m_fFade = 1.0f - m_fScale;

			red = 0.7f * m_fScale;
			green = 1.0f * m_fScale;
			blue = 0.85f * m_fScale;

			ground_height = m_fScale * 5.0f;
			life_scale = effect_sort.nCurFrame * 10.0f;
			round_light_scale = 0.0f;
		}
		else if( effect_sort.nCurFrame > 20 && effect_sort.nCurFrame <= 110 )
		{
			m_fScale = ((float)(effect_sort.nCurFrame-20) / 90);
			m_fFade = 1.0f - m_fScale;

			red = 0.7f;
			green = 1.0f;
			blue = 0.85f;

			ground_height = 5.0f;
			life_scale = 200.0f;

			if( effect_sort.nCurFrame > 20 && effect_sort.nCurFrame <= 50 )
				round_light_scale = 2.5f * ((float)(effect_sort.nCurFrame - 20) / 30);
			else
				round_light_scale = 2.5f;

		}
		else
		{
			m_fScale = ((float)(effect_sort.nCurFrame - 110) / 20);
			m_fFade = 1.0f - m_fScale;

			red = 0.7f - (0.7f * m_fScale);
			green = 1.0f - (1.0f * m_fScale);
			blue = 0.85f - (0.85f * m_fScale);

			ground_height = m_fFade * 5.0f;
			life_scale = 200.0f - ((effect_sort.nCurFrame - 110) * 10.0f);
			round_light_scale = 2.5f;
		}

		if( corona )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_GROUND2 ] ) );

			corona->SetColor(red, green, blue, m_fFade);
			corona->SetRotationX( g_PI * 0.5f );
			corona->Scale( 5.0f, 5.0f, 5.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			corona->Render();

			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_GROUND ] ) );

			corona->SetColor(red * 0.15f, green * 0.15f, blue * 0.15f, m_fFade * 0.15f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 2.0f );
			corona->Scale( 5.0f, 5.0f, 5.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + ground_height, vDrawPos.z);
			corona->Render();

			corona->SetColor(red * 0.25f, green * 0.25f, blue * 0.25f, m_fFade * 0.25f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 3.0f );
			corona->Scale( 3.0f, 3.0f, 3.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + ground_height*3.5f, vDrawPos.z);
			corona->Render();

			corona->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 4.0f );
			corona->Scale( 1.0f, 1.0f, 1.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + ground_height*5.0f, vDrawPos.z);
			corona->Render();
		}

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_ROUND ] ) );

		if( round )
		{
			round->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
			round->SetRotationY( g_PI );
			round->RotateY( m_fScale * 3.0f );
			round->Scale( 1.0f, life_scale, 1.0f );
			round->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			round->Render();
		}

		if( round_light )
		{
			round_light->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
			round_light->SetRotationY( g_PI );
			round_light->Scale( round_light_scale, life_scale, round_light_scale );
			round_light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			round_light->Render();
		}

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		return FALSE;
	}
	//wan:2004-12
	//뮤엘 죽을때 효과
	else
	{
		if(effect_sort.nCurFrame >= m_nTotalFrame)
		{
			m_fScale = 0.0f;
			m_fFade = 1.0f;
			return TRUE;
		}

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

		DWORD  dwTextureState[3];

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		D3DVECTOR	vDrawPos;
		vDrawPos = effect_sort.vPos;

		float maxScale = 2.0f;
		m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
		m_fFade = 1.0f - (m_fScale * 0.45f);

		float red, green, blue;
		red = green = blue = 1.0f;

		if( corona )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_CORONA ] ) );

			red = 0.4f - (0.4f * m_fScale * 0.45f);
			green = 0.1f - (0.1f * m_fScale * 0.45f);
			blue = 0.4f - (0.4f * m_fScale * 0.45f);

			corona->SetColor(red * 0.15f, green * 0.15f, blue * 0.15f, m_fFade * 0.15f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 5.0f );
			corona->Scale( 5.0f, 5.0f, 5.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			corona->Render();
		}

		///////////////////////////////////////////////////////////////////////////////////////
		// 밑에 마법진 나타났다 사라지는 효과

		float ground_height = 1.0f;
		float life_scale = 1.0f;
		float round_light_scale = 1.0f;

		if( effect_sort.nCurFrame <= 20 )
		{
			m_fScale = ((float)effect_sort.nCurFrame / 20);
			m_fFade = 1.0f - m_fScale;

			red = 0.4f * m_fScale;
			green = 0.1f * m_fScale;
			blue = 0.4f * m_fScale;

			ground_height = m_fScale * 5.0f;
			life_scale = effect_sort.nCurFrame * 10.0f;
			round_light_scale = 0.0f;
		}
		else if( effect_sort.nCurFrame > 20 && effect_sort.nCurFrame <= 110 )
		{
			m_fScale = ((float)(effect_sort.nCurFrame-20) / 90);
			m_fFade = 1.0f - m_fScale;

			red = 0.4f;
			green = 0.1f;
			blue = 0.4f;

			ground_height = 5.0f;
			life_scale = 200.0f;

			if( effect_sort.nCurFrame > 20 && effect_sort.nCurFrame <= 50 )
				round_light_scale = 2.5f * ((float)(effect_sort.nCurFrame - 20) / 30);
			else
				round_light_scale = 2.5f;

		}
		else
		{
			m_fScale = ((float)(effect_sort.nCurFrame - 110) / 20);
			m_fFade = 1.0f - m_fScale;

			red = 0.4f - (0.4f * m_fScale);
			green = 0.1f - (0.1f * m_fScale);
			blue = 0.4f - (0.4f * m_fScale);

			ground_height = m_fFade * 5.0f;
			life_scale = 200.0f - ((effect_sort.nCurFrame - 110) * 10.0f);
			round_light_scale = 2.5f;
		}

		if( corona )
		{
			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_GROUND2 ] ) );

			corona->SetColor(red, green, blue, m_fFade);
			corona->SetRotationX( g_PI * 0.5f );
			corona->Scale( 5.0f, 5.0f, 5.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			corona->Render();

			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_GROUND ] ) );

			corona->SetColor(red * 0.15f, green * 0.15f, blue * 0.15f, m_fFade * 0.15f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 2.0f );
			corona->Scale( 5.0f, 5.0f, 5.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + ground_height, vDrawPos.z);
			corona->Render();

			corona->SetColor(red * 0.25f, green * 0.25f, blue * 0.25f, m_fFade * 0.25f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 3.0f );
			corona->Scale( 3.0f, 3.0f, 3.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + ground_height*3.5f, vDrawPos.z);
			corona->Render();

			corona->SetColor(red * 0.35f, green * 0.35f, blue * 0.35f, m_fFade * 0.35f);
			corona->SetRotationX( g_PI * 0.5f );
			corona->RotateZ( m_fScale * 4.0f );
			corona->Scale( 1.0f, 1.0f, 1.0f );
			corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f + ground_height*5.0f, vDrawPos.z);
			corona->Render();
		}

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WARGIANTDIE_ROUND ] ) );
		if( round )
		{
			//맨 가운데 원 기둥
			round->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
			round->SetRotationY( g_PI );
			round->RotateY( m_fScale * 3.0f );
			round->Scale( 1.0f, life_scale, 1.0f );
			round->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			round->Render();
		}
		if( round_light )
		{
			//바깥 기둥
			round_light->SetColor(red * 0.3f, green * 0.3f, blue * 0.5f, m_fFade * 0.35f);
			round_light->SetRotationY( g_PI );
			round_light->Scale( round_light_scale, life_scale, round_light_scale );
			round_light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			round_light->Render();
		}

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		return FALSE;
	}
}
