#include "stdafx.h"
#include "sfx.h"
#include <assert.h>
#include "main.h"
#include "DecoObj.h"
#include "ImageDefine.h"
#include "GUtil.h"
#include "NkCharacter.h"


typedef struct Deco_Particle Deco_Particle;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxClystalGlow::CFxClystalGlow()
{
	m_Plane = NULL;
	m_pClystalRed	= NULL;
	m_pClystalWineB = NULL;
	m_pClystalWineS = NULL;
	m_pClystalCyon	= NULL;
	Create();
}

CFxClystalGlow::~CFxClystalGlow()
{
	SAFE_DELETE(m_Plane);
}

void CFxClystalGlow::Create()
{
	SAFE_DELETE(m_Plane);

	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}


void CFxClystalGlow::LoadRes()
{
	if (!m_pClystalRed)
	{
		m_pClystalRed = new CLolos;
		m_pClystalRed->SetIndexedTexture( g_pCapsyongTexture );
		m_pClystalRed->Read("data/effect/lolos/crystal_red.LOL");
	}

	if (!m_pClystalWineB)
	{
		m_pClystalWineB = new CLolos;
		m_pClystalWineB->SetIndexedTexture( g_pCapsyongTexture );
		m_pClystalWineB->Read("data/effect/lolos/crystal_pink_b.LOL");
	}

	if (!m_pClystalWineS)
	{
		m_pClystalWineS = new CLolos;
		m_pClystalWineS->SetIndexedTexture( g_pCapsyongTexture );
		m_pClystalWineS->Read("data/effect/lolos/crystal_pink_s.LOL");
	}

	if (!m_pClystalCyon)
	{
		m_pClystalCyon = new CLolos;
		m_pClystalCyon->SetIndexedTexture( g_pCapsyongTexture );
		m_pClystalCyon->Read("data/effect/lolos/crystal_blue.LOL");
	}




}


void CFxClystalGlow::DeleteRes()
{
	SAFE_DELETE(m_pClystalRed);
	SAFE_DELETE(m_pClystalWineB);
	SAFE_DELETE(m_pClystalWineS);
	SAFE_DELETE(m_pClystalCyon);
}


void CFxClystalGlow::Update_Particle(EffectSort &ef_sort)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( ef_sort.vPos.x, ef_sort.vPos.y, ef_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) ef_sort.effects;

	ef_sort.vSortPos.x = DistVecToVec( ef_sort.vTargetPos, ef_sort.vPos );

	ef_sort.vCurPos = ef_sort.vPos;

	switch(ef_sort.nTargetType )
	{
	case CLYSTAL_TYPE_WINEB:
	case CLYSTAL_TYPE_WINES:
		ef_sort.vCurPos.y += (ef_sort.vTargetPos.y - ef_sort.vPos.y) / 2.0f;
		break;
	}

}


BOOL CFxClystalGlow::Render(EffectSort &ef_sort)
{

	ClystalRender(ef_sort);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	// nate 2004 - 7 - image


	Update_Particle(ef_sort);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) ef_sort.effects;
	if( m_Plane )
	{
		switch( ef_sort.nTargetType )
		{
		case CLYSTAL_TYPE_RED:
			{
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CLYSTALGLOWLIGHT_RED ] ));
				float fScale = 500.0f + (((float)((rand()%1)+14)) * ((rand()%2 == 0? -1.0f:1.0f)));
				//m_Plane->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
				m_Plane->Scale( fScale, fScale, 0.0f );
				m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_Plane->SetTranslationXYZ(ef_sort.vCurPos.x, ef_sort.vCurPos.y+30, ef_sort.vCurPos.z);
			}
			break;
		case CLYSTAL_TYPE_WINEB:
			{
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CLYSTALGLOWLIGHT_WINE ] ));
				float fScale = 300.0f + (((float)((rand()%1)+4)) * ((rand()%2 == 0? -1.0f:1.0f)));
				//m_Plane->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
				m_Plane->Scale( fScale, fScale, 0.0f );
				m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_Plane->SetTranslationXYZ(ef_sort.vCurPos.x, ef_sort.vCurPos.y, ef_sort.vCurPos.z);
			}
			break;
		case CLYSTAL_TYPE_WINES:
			{
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CLYSTALGLOWLIGHT_WINE ] ));
				float fScale = 150.0f + (((float)((rand()%1)+4)) * ((rand()%2 == 0? -1.0f:1.0f)));
				//m_Plane->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
				m_Plane->Scale( fScale, fScale, 0.0f );
				m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_Plane->SetTranslationXYZ(ef_sort.vCurPos.x, ef_sort.vCurPos.y, ef_sort.vCurPos.z);
			}
			break;
		case CLYSTAL_TYPE_CYON:
			{
				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CLYSTALGLOWLIGHT_CYON ] ));
				float fScale = 500.0f + (((float)((rand()%1)+14)) * ((rand()%2 == 0? -1.0f:1.0f)));
				//m_Plane->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
				m_Plane->Scale( fScale, fScale, 0.0f );
				m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_Plane->SetTranslationXYZ(ef_sort.vCurPos.x, ef_sort.vCurPos.y+30, ef_sort.vCurPos.z);
			}
			break;
		}
		m_Plane->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}


BOOL CFxClystalGlow::ClystalRender(EffectSort &ef_sort)
{
	DWORD  dwTextureState[3];
	D3DMATRIX matTrans, matRotate, matWorld, matS;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	switch( ef_sort.nTargetType)
	{
	case CLYSTAL_TYPE_RED:
		D3DUtil_SetRotateYMatrix(matRotate, 3.12f);
		break;
	case CLYSTAL_TYPE_CYON:
		D3DUtil_SetRotateYMatrix(matRotate, 0.0f);
		break;
	default:
		D3DUtil_SetRotateYMatrix(matRotate, 1);
		break;
	}

	//D3DUtil_SetRotateYMatrix(matRotate, 1);
	D3DUtil_SetTranslateMatrix( matTrans, ef_sort.vCurPos.x, ef_sort.vCurPos.y, ef_sort.vCurPos.z);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	switch( ef_sort.nTargetType)
	{
	case CLYSTAL_TYPE_RED:
		m_pClystalRed-> Render(GET_D3DDEVICE(), 0, FALSE, FALSE);
		break;
	case CLYSTAL_TYPE_WINEB:
		m_pClystalWineB->Render(GET_D3DDEVICE(), ef_sort.nCurFrame, FALSE, FALSE);
		break;
	case CLYSTAL_TYPE_WINES:
		m_pClystalWineS->Render(GET_D3DDEVICE(), ef_sort.nCurFrame, FALSE, FALSE);
		break;
	case CLYSTAL_TYPE_CYON:
		m_pClystalCyon-> Render(GET_D3DDEVICE(), 0, FALSE, FALSE);
		break;
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return TRUE;
}
