#include "stdafx.h"
#include "main.h"

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
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
#include "FX_BossRaid_Aidia2_03.h"




CFxBossRaidAidia2_03::CFxBossRaidAidia2_03()
{
	m_laser					= NULL;
	light					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_pLolo					= NULL;

	m_laser = new D3DLVERTEX[ FX_BOSSRAID_ADIA2_03_MAX_LASER_VERTEXS ];

	light = new CRectPlane;
	if( light )
		light->Create( 5.0f, 5.0f, FALSE );

	if (!m_pLolo) // 하늘에서 떨어지는 기 형태의 검
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/arrow.LOL", TRUE);
	}


}

CFxBossRaidAidia2_03::~CFxBossRaidAidia2_03()
{
	SAFE_DELETE( m_pLolo );
	SAFE_DELETE(light);
	SAFE_DELETE_ARRAY(m_laser);
}

void CFxBossRaidAidia2_03::LoadRes()
{

}

void CFxBossRaidAidia2_03::DeleteRes()
{
}

bool CFxBossRaidAidia2_03::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	float tu[4];
	DWORD dwRenderState[3];
	DWORD dwTextureState[3];

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	float red = 0.4f - (0.4f * m_fScale * 0.15f);
	float green = 0.5f - (0.5f * m_fScale * 0.15f);
	float blue = 0.95f - (0.95f * m_fScale * 0.15f);

	Deco_Particle* deco_particle = effect_sort.pParticle;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING,  &dwRenderState[0]);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND,  &dwRenderState[1]);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenderState[2]);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,   FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	if (effect_sort.nCurFrame >= 50 && effect_sort.nCurFrame < 80)
	{
		int nFrame = effect_sort.nCurFrame - 50;
		int nEvent = 6; //default

		if (nFrame > 0  && nFrame <= 3)  nEvent = 8;
		if (nFrame > 3  && nFrame <= 7)  nEvent = 18;
		if (nFrame > 7  && nFrame <= 9)	 nEvent = 24;
		if (nFrame > 9  && nFrame <= 12) nEvent = 32;
		if (nFrame > 12 && nFrame <= 15) nEvent = 35;

		D3DVECTOR vMobVec, vChaVec;

		for(int index = 0; index < nEvent; ++index)
		{
			D3DVECTOR vMobVec = effect_sort.vTargetPos + deco_particle[index].cur_Pos+D3DVECTOR(0, 150,0);
			vChaVec = effect_sort.vTargetPos + deco_particle[index].cur_Pos;

			D3DVECTOR	vDirVec;
			vDirVec = vMobVec - vChaVec;
			vDirVec = Normalize(vDirVec);

			D3DVECTOR	vRight = CrossProduct( vViewVec, vDirVec );
			vRight = Normalize(vRight);
			vRight *= 2.0f;

			m_fScale = (3.0f * nFrame) / 15;
			m_fFade = 1.0f - (m_fScale * 0.3f);

			tu[0] = 1.0f;
			tu[1] = 1.0f;
			tu[2] = 0.0f;
			tu[3] = 0.0f;

			// 옆으로 향하는 텍스쳐로 빌보드된 사각형
			m_laser[0] = D3DLVERTEX( vChaVec - vRight, D3DRGBA(red, green, blue, m_fFade),0, tu[2], 0.0f );
			m_laser[1] = D3DLVERTEX( vChaVec + vRight, D3DRGBA(red, green, blue, m_fFade), 0, tu[3], 1.0f );
			m_laser[2] = D3DLVERTEX( vMobVec - vRight, 0x00000000, 0, tu[0], 0.0f );
			m_laser[3] = D3DLVERTEX( vMobVec + vRight, 0x00000000, 0, tu[1], 1.0f );

			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MAGICLASER ] ) );

			D3DMATRIX	matWorld;
			D3DUtil_SetIdentityMatrix(matWorld);
			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_laser,
											FX_BOSSRAID_ADIA2_03_MAX_LASER_VERTEXS, 0 );

			// nate 2004 - 7 - image
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MAGICLASER_LIGHT ] ) );

			if( light )
			{
				light->SetColor(red, green, blue, m_fFade);
				light->SetBillBoard( vViewVec, FALSE, 0.0f );
				light->Scale( RandomNum(5.0f, 9.0f), RandomNum(5.0f, 9.0f), RandomNum(5.0f, 9.0f) );
				light->SetTranslationXYZ(vChaVec.x, vChaVec.y, vChaVec.z);
				light->Render();
			}
		}
	}

	if(effect_sort.nCurFrame >= 36 && effect_sort.nCurFrame < 52)
	{
		int nFrame = effect_sort.nCurFrame - 32;

		D3DVECTOR vMobVec, vChaVec;

		vMobVec = effect_sort.vCurPos;
		vChaVec = effect_sort.vTargetPos;

		D3DVECTOR	vDirVec;
		vDirVec = vMobVec - vChaVec;
		vDirVec = Normalize(vDirVec);

		D3DVECTOR	vRight = CrossProduct( D3DVECTOR(0, 1,0), vDirVec );
		vRight = Normalize(vRight);
		vRight *= 2.0f;

		vMobVec = effect_sort.vCurPos + D3DVECTOR(0, 85,0);
		vChaVec = vMobVec + D3DVECTOR(0, 40,0);

		D3DVECTOR	vMobHand1, vMobHand2;
		vMobHand1 = effect_sort.pMobFrom->m_pCapSyong->m_LineVector[1];
		vMobHand1.x *= -1;
		vMobHand1.z *= -1;
		vMobHand2 = vMobHand1 + D3DVECTOR(0.0f, 30.0f, 0.0f);

		m_fScale = (3.0f * nFrame) / 15;
		m_fFade = 1.0f - (m_fScale * 0.3f);

		D3DMATRIX	matWorld, matRotate, matTrans;
		D3DUtil_SetIdentityMatrix(matWorld);
		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);

		int nEvent = 12; //default
		for( int index = 0; index < nEvent; ++index)
		{
			float theta = 0;

			D3DVECTOR vHeight = D3DVECTOR(0, 100 + nFrame*25.f + index * -50.f, 0);
			if (vHeight.y < 100) continue;

			Nk3DUtil_SetRotateCCWYMatrix(matRotate, theta );
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos+vHeight);

			D3DMath_MatrixMultiply(matWorld, matRotate, matTrans);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			m_pLolo->Render(GET_D3DDEVICE(), nFrame, FALSE, FALSE);
		}
	}

	if(effect_sort.nCurFrame >= 12 && effect_sort.nCurFrame < 32)
	{
		int nFrame = effect_sort.nCurFrame - 12;

		D3DVECTOR vMobVec, vChaVec;

		vMobVec = effect_sort.vCurPos;
		vChaVec = effect_sort.vTargetPos;

		D3DVECTOR	vDirVec;
		vDirVec = vMobVec - vChaVec;
		vDirVec = Normalize(vDirVec);

		D3DVECTOR	vRight = CrossProduct( D3DVECTOR(0, 1,0), vDirVec );
		vRight = Normalize(vRight);
		vRight *= 2.0f;

		vMobVec = effect_sort.vCurPos + D3DVECTOR(0, 85,0);
		vChaVec = vMobVec + D3DVECTOR(0, 40,0);

		D3DVECTOR	vMobHand1, vMobHand2;
		D3DMATRIX matRotate;
		D3DUtil_SetIdentityMatrix(matRotate);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.pMobFrom->m_dir );
		D3DMath_VectorMatrixMultiply( vMobHand1, effect_sort.pMobFrom->m_pCapSyong->m_LineVector[1], matRotate );
		vMobHand2 = vMobHand1 + D3DVECTOR(0.0f, 30.0f, 0.0f);

		m_fScale = (3.0f * nFrame) / 15;
		m_fFade = 1.0f - (m_fScale * 0.3f);

		int nEvent = 1; //default
		if (nFrame > 0  && nFrame <= 3)  nEvent = 8;
		if (nFrame > 3  && nFrame <= 7)  nEvent = 18;
		if (nFrame > 7  && nFrame <= 9)	 nEvent = 24;
		if (nFrame > 9  && nFrame <= 12) nEvent = 32;
		if (nFrame > 12 && nFrame <= 15) nEvent = 35;
		if (nFrame > 12 && nFrame <= 20) nEvent = 35;

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MAGICLASER_LIGHT ] ) );

		if( light )
		{
			int index;
			for( index = 0; index < nEvent; ++index)
			{
				light->SetColor(red, green, blue, m_fFade);
				light->SetBillBoard( vViewVec, FALSE, 0.0f );
				light->Scale( RandomNum(nFrame*0.45f, nFrame*0.70f),
							  RandomNum(nFrame*0.45f, nFrame*0.70f),
							  RandomNum(nFrame*0.45f, nFrame*0.70f) );
				light->SetTranslationXYZ(effect_sort.pMobFrom->m_wx + vMobHand1.x,
										 effect_sort.pMobFrom->m_wy + vMobHand1.y,
										 effect_sort.pMobFrom->m_wz + vMobHand1.z);
				light->Render();
			}
		}
	}
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,   dwRenderState[0]);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   dwRenderState[1]);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  dwRenderState[2]);

	return FALSE;
}
