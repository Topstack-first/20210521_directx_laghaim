#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Land.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



extern float g_fDSDistance;

#define MAX_GATHER_VERTEXS_NUM 4


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxGatheringGun::CFxGatheringGun()
{
	m_gather		= NULL;
	m_fScale		= 0.0f;
	m_fFade			= 0.0f;
	m_nTotalFrame	= FX_GATHERINGGUN_FRAMECOUNT;

	Create();
}

CFxGatheringGun::~CFxGatheringGun()
{
	SAFE_DELETE(m_gather);
}

void CFxGatheringGun::Create()
{
	// nate 2004 - 8 ImageManager
	m_gather = new D3DLVERTEX[ MAX_GATHER_VERTEXS_NUM ];
}

void CFxGatheringGun::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxGatheringGun::DeleteRes()
{}

BOOL CFxGatheringGun::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pNkChaFrom
			&& !effect_sort.pMobFrom
			&& !effect_sort.pNkChaTo
			&& !effect_sort.pMobTo )
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
	// Set any appropiate state
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
	if( effect_sort.pNkChaFrom )
		vChaVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy+18.0f, effect_sort.pNkChaFrom->m_wz);
	else if( effect_sort.pMobFrom )
		vChaVec = D3DVECTOR(effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz);
	else
		return true;

	// 반드시 몹말고 캐릭터도 체크해야함....꼭...
	if( effect_sort.pMobTo )
		vMobVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if( effect_sort.pNkChaTo )
		vMobVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+15.0f, effect_sort.pNkChaTo->m_wz);
	else
		return true;

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
	vRight *= 1.0f;
	vChaVec += (vDirVec*3.0f) + vRight;

	float red, green, blue;
	red = 0.4f - (0.4f * m_fScale * 0.15f);
	green = 0.95f - (0.95f * m_fScale * 0.15f);
	blue = 0.5f - (0.5f * m_fScale * 0.15f);

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
	m_gather[0] = D3DLVERTEX( vChaVec - vRight, D3DRGBA(red, green, blue, m_fFade), 0, tu[0], 0.0f );
	m_gather[1] = D3DLVERTEX( vChaVec + vRight, D3DRGBA(red, green, blue, m_fFade), 0, tu[1], 1.0f );
	m_gather[2] = D3DLVERTEX( vMobVec - vRight, D3DRGBA(red, green, blue, m_fFade), 0, tu[2], 0.0f );
	m_gather[3] = D3DLVERTEX( vMobVec + vRight, D3DRGBA(red, green, blue, m_fFade), 0, tu[3], 1.0f );

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_GATHERINGGUN ] ) );

	D3DMATRIX				matWorld;
	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
									m_gather,
									MAX_GATHER_VERTEXS_NUM,
									0 );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}



