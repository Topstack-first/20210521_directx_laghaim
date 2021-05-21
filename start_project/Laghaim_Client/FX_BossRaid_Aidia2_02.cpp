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
#include "FX_BossRaid_Aidia2_02.h"




CFxBossRaidAidia2_02::CFxBossRaidAidia2_02()
{
	m_laser					= NULL;
	light					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;

	m_laser = new D3DLVERTEX[ FX_BOSSRAID_ADIA2_02_MAX_LASER_VERTEXS ];

	light = new CRectPlane;
	if( light )
		light->Create( 5.0f, 5.0f, FALSE );
}

CFxBossRaidAidia2_02::~CFxBossRaidAidia2_02()
{
//	DeleteRes();
}

void CFxBossRaidAidia2_02::LoadRes()
{

}

void CFxBossRaidAidia2_02::DeleteRes()
{
	SAFE_DELETE(m_laser);
	SAFE_DELETE(light);
}

bool CFxBossRaidAidia2_02::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	float tu[4];
	D3DVECTOR vViewVec;
	D3DVECTOR vChaVec, vMobVec;

	float red = 0.95f - (0.95f * m_fScale * 0.15f);
	float green = 0.2f - (0.2f * m_fScale * 0.15f);
	float blue = 0.4f - (0.4f * m_fScale * 0.15f);

	DWORD dwRenderState[3];
	DWORD dwTextureState[3];

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

	if (effect_sort.nCurFrame >= 38 && effect_sort.nCurFrame < 45)
	{
		vViewVec = pCMyApp->GetEyeDirection();

		float fHeight = 0.0f;
		vMobVec = effect_sort.vCurPos;
		vChaVec = effect_sort.vTargetPos;

		D3DVECTOR	vDirVec;
		vDirVec = vMobVec - vChaVec;
		vDirVec = Normalize(vDirVec);

		D3DVECTOR	vRight = CrossProduct( vViewVec, vDirVec );
		vRight = Normalize(vRight);
		vRight *= 2.0f;

		int nFrame = effect_sort.nCurFrame - 38;

		m_fScale = (3.0f * nFrame) / 15;
		m_fFade = 1.0f - (m_fScale * 0.3f);

		tu[0] = 1.0f;
		tu[1] = 1.0f;
		tu[2] = 0.0f;
		tu[3] = 0.0f;

		for( int i = 0; i < 4; i++ )
		{
			if( tu[i] > 1.0f )
				tu[i] = 1.0f - tu[i];
		}

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
										// nate 2004 - 8 ImageManager
										FX_BOSSRAID_ADIA2_02_MAX_LASER_VERTEXS, 0 );

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MAGICLASER_LIGHT ] ) );

		if( light )
		{
			light->SetColor(red, green, blue, m_fFade);
			light->SetBillBoard( vViewVec, FALSE, 0.0f );
			light->Scale( RandomNum(12.0f, 19.0f), RandomNum(12.0f, 19.0f), RandomNum(12.0f, 19.0f) );
			light->SetTranslationXYZ(vChaVec.x, vChaVec.y, vChaVec.z);
			light->Render();
		}
	}
	else if (effect_sort.nCurFrame >= 25 && effect_sort.nCurFrame < 36)
	{

		int nFrame = effect_sort.nCurFrame - 25;

		effect_sort.vTargetPos = D3DVECTOR(effect_sort.pNkChaTo->m_wx,
										   effect_sort.pNkChaTo->m_wy,
										   effect_sort.pNkChaTo->m_wz );

		vViewVec = pCMyApp->GetEyeDirection();

		float fHeight = 0.0f;
		vMobVec = D3DVECTOR(effect_sort.pMobFrom->m_wx,
							effect_sort.pMobFrom->m_wy,
							effect_sort.pMobFrom->m_wz);
		vChaVec = effect_sort.vTargetPos;

		D3DVECTOR	vDirVec;
		vDirVec = vMobVec - vChaVec;
		vDirVec = Normalize(vDirVec);

		D3DVECTOR	vRight = CrossProduct( vViewVec, vDirVec );
		vRight = Normalize(vRight);
		vRight *= 2.0f;

		D3DVECTOR vLine;
		D3DMATRIX matRotate;
		D3DUtil_SetIdentityMatrix(matRotate);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.pMobFrom->m_dir );
		D3DMath_VectorMatrixMultiply( vLine, effect_sort.pMobFrom->m_pCapSyong->m_LineVector[1], matRotate );
		vMobVec += vLine;

		effect_sort.vCurPos = vMobVec;

		m_fScale = (3.0f * nFrame) / 15;
		m_fFade = 1.0f - (m_fScale * 0.3f);

		// nate 2004 - 7 - image
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MAGICLASER_LIGHT ] ) );

		if( light )
		{
			light->SetColor(red, green, blue, m_fFade);
			light->SetBillBoard( vViewVec, FALSE, 0.0f );
			light->Scale( RandomNum(3.0f, 8.0f), RandomNum(3.0f, 8.0f), RandomNum(3.0f, 8.0f) );
			light->SetTranslationXYZ(vMobVec.x, vMobVec.y, vMobVec.z);

			light->Render();
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
