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
#define MAX_CANNON_VERTEXS_COUNT 4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxChargeCannon::CFxChargeCannon()
{
	m_cannon				= NULL;
	m_fScale				= 0.0f;
	m_fFade					= 0.0f;
	m_nTotalFrame			= FX_CHARGECANNON_FRAMECOUNT;

	Create();
}

CFxChargeCannon::~CFxChargeCannon()
{
	SAFE_DELETE(m_cannon);
}

void CFxChargeCannon::Create()
{
	m_cannon = new D3DLVERTEX[ MAX_CANNON_VERTEXS_COUNT ];
}

void CFxChargeCannon::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxChargeCannon::DeleteRes()
{}


BOOL CFxChargeCannon::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

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
	vChaVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y + 15.0f, effect_sort.vPos.z);

	// 반드시 몹말고 캐릭터도 체크해야함....꼭...
	if( effect_sort.pMobTo )
		vMobVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if( effect_sort.pNkChaTo )
		vMobVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + 10.0f, effect_sort.pNkChaTo->m_wz);

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
	vDirVec *= 7.5f; // Speed
	vRight *= 2.5f;

	D3DVECTOR	vTargetVec, vFromVec;
	vTargetVec = vChaVec + (vDirVec * (effect_sort.nCurFrame + 5));
	vFromVec = vChaVec + (vDirVec * effect_sort.nCurFrame);

	// 마지막 프레임이 목적지에 도착했는가 확인? - 거리를 측정해서 한다.
	float fDist = DistPlToPlAbs(vMobVec.x, vMobVec.z, vChaVec.x, vChaVec.z) -
				  DistPlToPlAbs(vTargetVec.x + vDirVec.x, vTargetVec.z + vDirVec.z, vChaVec.x, vChaVec.z);

	if ( fDist <= 0.0f )
	{
		effect_sort.nCurFrame = m_nTotalFrame;
		m_fScale = 0.0f;
		m_fFade = 1.0f;

		// 폭파 효과를 호출한다.
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_LIGHTBALLBOOMNEW;

		if (effect_sort.pMobTo)
		{
			ef_sort.pMobTo = effect_sort.pMobTo;
			effect_sort.pMobTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		else if (effect_sort.pNkChaTo)
		{
			ef_sort.pNkChaTo = effect_sort.pNkChaTo;
			effect_sort.pNkChaTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		if (g_pDSound)
		{
			float x, y, z;
			x = (vMobVec.x - g_pRoh->m_wx) / g_fDSDistance;
			y = (vMobVec.y - g_pRoh->m_wy) / g_fDSDistance;
			z = (vMobVec.z - g_pRoh->m_wz) / g_fDSDistance;
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM2], x, y, z);
		}

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		return TRUE;
	}

	float red, green, blue;
	red = 0.4f;// - (0.4f * m_fScale * 0.3f);
	green = 0.39f;// - (0.39f * m_fScale * 0.3f);
	blue = 0.95f;// - (0.95f * m_fScale * 0.3f);

	// 테스트
	m_cannon[0] = D3DLVERTEX( vFromVec - vRight, 0x00000000, 0, 1.0f, 0.0f );
	m_cannon[1] = D3DLVERTEX( vFromVec + vRight, 0x00000000, 0, 1.0f, 1.0f );
	m_cannon[2] = D3DLVERTEX( vTargetVec - vRight, D3DRGBA(red, green, blue, m_fFade), 0, 0.0f, 0.0f );
	m_cannon[3] = D3DLVERTEX( vTargetVec + vRight, D3DRGBA(red, green, blue, m_fFade), 0, 0.0f, 1.0f );

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CHARGECANNON ] ) );

	D3DMATRIX				matWorld;
	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_cannon,
									MAX_CANNON_VERTEXS_COUNT,
									// nate 2004 - 8 ImageManager
//										 max_cannon_vertexs,
									0 );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}






