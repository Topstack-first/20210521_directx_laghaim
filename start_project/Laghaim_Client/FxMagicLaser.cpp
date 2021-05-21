#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Land.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"



extern float g_fDSDistance;

// nate 2004 - 8 ImageManager
#define FX_MAGICLASER_MAX_LASER_VERTEXS 4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxMagicLaser::CFxMagicLaser()
{
	m_laser					= NULL;
	light					= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_MAGICLASER_FRAMECOUNT;

	Create();
}

CFxMagicLaser::~CFxMagicLaser()
{
	DeleteRes();
}

void CFxMagicLaser::Create()
{
	m_laser = new D3DLVERTEX[ FX_MAGICLASER_MAX_LASER_VERTEXS ];

	light = new CRectPlane;
	if( light )
		light->Create( 5.0f, 5.0f, FALSE );
}

void CFxMagicLaser::DeleteRes()
{
	SAFE_DELETE(m_laser);
	SAFE_DELETE(light);
}

BOOL CFxMagicLaser::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(effect_sort.nCurFrame >= m_nTotalFrame)
	{
		m_fScale = 0.0f;
		m_fFade = 1.0f;
		return TRUE;
	}

	float maxScale = 3.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.3f);

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

	D3DVECTOR	vChaVec, vMobVec;
	float fHeight = 0.0f;
	if( effect_sort.Index == 1 )
	{
		fHeight = 12.0f;
		if( effect_sort.pNkChaFrom )
			vChaVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy+fHeight, effect_sort.pNkChaFrom->m_wz);
		else if( effect_sort.pMobFrom )
			vChaVec = D3DVECTOR(effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy+fHeight, effect_sort.pMobFrom->m_wz);
	}
	else if( effect_sort.Index == 83 )
		vChaVec = effect_sort.vPos;

	if( effect_sort.Index == 1 )
	{
		if( effect_sort.pMobTo )
			vMobVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
		else if( effect_sort.pNkChaTo )
			vMobVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + fHeight, effect_sort.pNkChaTo->m_wz);
	}
	else if( effect_sort.Index == 83 )
		vMobVec = effect_sort.vTargetPos;

	if (effect_sort.pMobTo)
	{
		switch (effect_sort.pMobTo->m_Height)
		{
		case MOB_HEIGHT_HIGH_NUM:
			vMobVec.y += MOB_HEIGHT_HIGH_VAL;
			break;
		case MOB_HEIGHT_LOW_NUM:
			vMobVec.y += MOB_HEIGHT_LOW_VAL;
			break;
		default: // MOB_HEIGHT_MIDDLE_NUM:
			vMobVec.y += MOB_HEIGHT_MIDDLE_VAL;
			break;
		}
	}

	D3DVECTOR	vDirVec;
	vDirVec = vMobVec - vChaVec;
	vDirVec = Normalize(vDirVec);

	D3DVECTOR	vRight = CrossProduct( vViewVec, vDirVec );
	vRight = Normalize(vRight);
	vRight *= 2.0f;

	float red = 0.0f, green = 0.0f, blue = 0.0f;
	if( effect_sort.Index == 1 )
	{
		red = 0.4f - (0.4f * m_fScale * 0.15f);
		green = 0.5f - (0.5f * m_fScale * 0.15f);
		blue = 0.95f - (0.95f * m_fScale * 0.15f);
	}
	else if( effect_sort.Index == 83 )
	{
		red = 0.95f - (0.95f * m_fScale * 0.15f);
		green = 0.2f - (0.2f * m_fScale * 0.15f);
		blue = 0.4f - (0.4f * m_fScale * 0.15f);
	}

	float tu[4];

	tu[0] = 0.0f + effect_sort.nCurFrame * 0.35f;
	tu[1] = 0.0f + effect_sort.nCurFrame * 0.35f;
	tu[2] = 1.0f + effect_sort.nCurFrame * 0.35f;
	tu[3] = 1.0f + effect_sort.nCurFrame * 0.35f;

	for( int i = 0; i < 4; i++ )
	{
		if( tu[i] > 1.0f )
			tu[i] = 1.0f - tu[i];
	}

	// 옆으로 향하는 텍스쳐로 빌보드된 사각형
	m_laser[0] = D3DLVERTEX( vChaVec - vRight, 0x00000000, 0, tu[2], 0.0f );
	m_laser[1] = D3DLVERTEX( vChaVec + vRight, 0x00000000, 0, tu[3], 1.0f );
	m_laser[2] = D3DLVERTEX( vMobVec - vRight, D3DRGBA(red, green, blue, m_fFade), 0, tu[0], 0.0f );
	m_laser[3] = D3DLVERTEX( vMobVec + vRight, D3DRGBA(red, green, blue, m_fFade), 0, tu[1], 1.0f );

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MAGICLASER ] ) );

	D3DMATRIX	matWorld;
	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_laser,
									// nate 2004 - 8 ImageManager
									FX_MAGICLASER_MAX_LASER_VERTEXS,
									0 );

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_MAGICLASER_LIGHT ] ) );

	if( light )
	{
		light->SetColor(red, green, blue, m_fFade);
		light->SetBillBoard( vViewVec, FALSE, 0.0f );
		light->Scale( RandomNum(5.0f, 9.0f), RandomNum(5.0f, 9.0f), RandomNum(5.0f, 9.0f) );
		light->SetTranslationXYZ(vMobVec.x, vMobVec.y, vMobVec.z);
		light->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}
