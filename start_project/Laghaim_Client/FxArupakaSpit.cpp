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
#include "fxarupakaspit.h"

extern float g_fDSDistance;

#define FX_SOULDUST_MAX_VERTEXS 4

FxArupakaSpit::FxArupakaSpit(void)
{
	m_souldust				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_SOULDUST_FRAMECOUNT;

	Create();
}

FxArupakaSpit::~FxArupakaSpit(void)
{
	SAFE_DELETE(m_souldust);
}

void FxArupakaSpit::Create()
{
	m_souldust = new D3DLVERTEX[ FX_SOULDUST_MAX_VERTEXS ];
}

void FxArupakaSpit::LoadRes(IndexedTexture *pIndexedTexture)
{}
void FxArupakaSpit::DeleteRes()
{}

BOOL FxArupakaSpit::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	float maxScale = 3.0f;
	DWORD alphaTest, alphaBlend;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCCOLOR );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_SRCALPHA );

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
	vChaVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y + 7.5f, effect_sort.vPos.z);

	// 반드시 몹말고 캐릭터도 체크해야함....꼭...
	if( effect_sort.pMobTo )
		vMobVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if( effect_sort.pNkChaTo )
		vMobVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);

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
	vDirVec *= 2.0f; // Speed

	D3DVECTOR	vRight = CrossProduct( vViewVec, vDirVec );
	vRight = Normalize(vRight);
	vRight *= 0.5f;

	D3DVECTOR	vTargetVec, vFromVec;

	vTargetVec = vChaVec + (vDirVec * (effect_sort.nCurFrame + 1));
	vFromVec = vChaVec + (vDirVec * effect_sort.nCurFrame);

	float red, green, blue;

	red = 1.0f;
	green = 1.0f;
	blue = 1.0f;
		
	m_souldust[0] = D3DLVERTEX( vFromVec - vRight, D3DRGBA(red, green, blue, m_fFade), 0, 0.0f, 0.0f );
	m_souldust[1] = D3DLVERTEX( vFromVec + vRight, D3DRGBA(red, green, blue, m_fFade), 0, 0.0f, 1.0f );
	m_souldust[2] = D3DLVERTEX( vTargetVec - vRight, D3DRGBA(red, green, blue, m_fFade), 0, 1.0f, 0.0f );
	m_souldust[3] = D3DLVERTEX( vTargetVec + vRight, D3DRGBA(red, green, blue, m_fFade), 0, 1.0f, 1.0f );
		
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ARUPAKA ] ) );

	D3DMATRIX				matWorld;
	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_souldust,									
									FX_SOULDUST_MAX_VERTEXS,
									0 );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	// 마지막 프레임이 목적지에 도착했는가 확인? - 거리를 측정해서 한다.
	float fDist = DistPlToPlAbs(vMobVec.x, vMobVec.z, vChaVec.x, vChaVec.z) -
				  DistPlToPlAbs(vTargetVec.x + vDirVec.x, vTargetVec.z + vDirVec.z, vChaVec.x, vChaVec.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 1.0f)
	{
		effect_sort.nCurFrame = m_nTotalFrame;
		m_fScale = 0.0f;
		m_fFade = 1.0f;		
	}

	return FALSE;
}


