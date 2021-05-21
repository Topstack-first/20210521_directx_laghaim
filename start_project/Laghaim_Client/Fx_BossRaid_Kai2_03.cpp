#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "DecoObj.h"
#include "ImageDefine.h"
#include "GUtil.h"
#include "ChaSta.h"
#include "NkMob.h"
#include "Land.h"
#include "UIMgr.h"
#include "Fx_BossRaid_Kai2_03.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
typedef struct Deco_Particle Deco_Particle;

extern float				g_fDSDistance;



extern int					g_EffectSound[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidKai2_03::CFxBossRaidKai2_03()
{
	m_Plane = NULL;
	Create();
}

CFxBossRaidKai2_03::~CFxBossRaidKai2_03()
{
	SAFE_DELETE(m_Plane);
}

void CFxBossRaidKai2_03::Create()
{
	SAFE_DELETE(m_Plane);

	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}

void CFxBossRaidKai2_03::LoadRes()
{}

void CFxBossRaidKai2_03::DeleteRes()
{}


void CFxBossRaidKai2_03::Update_Particle(EffectSort &ef_sort)
{
	if( !ef_sort.pMobFrom )
		return;

	CNkMob* pMob = ef_sort.pMobFrom;

	switch(ef_sort.num)
	{
	case 0:
		{
			if( pMob->GetMotionState() == MOB_EMPTY2 && pMob->GetCurFrame() >= 18 )
			{
				//Ä®³¡ÀÇ À§¸®¯H °è¼Ó °»½ÅÇØÁØ´Ù.
				D3DMATRIX matRot;
				D3DVECTOR vTmp;

				D3DUtil_SetIdentityMatrix(matRot);
				Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[1], matRot);
				ef_sort.vCurPos = vTmp;
				ef_sort.vCurPos.x += pMob->m_wx;
				ef_sort.vCurPos.y += pMob->m_wy;
				ef_sort.vCurPos.z += pMob->m_wz;

				ef_sort.vSortPos.x += 0.7f;

				if( pMob->GetCurFrame() >= 67)
				{
					ef_sort.nTargetType = 0;
					ef_sort.num = 1;
				}
			}
		}
		break;

	case 1:
		{
			if( ef_sort.nTargetType % 2 == 0)
			{
				for( int i=0; i<2; i++)
				{
					EffectSort efSort;
					efSort.nType = FX_LIGHTNINGEX;

					efSort.vPos.x = ef_sort.vTargetPos.x + 5.0f + ( (float)(rand()%70) * (rand()%2 == 0? -1.0f:1.0f) );
					efSort.vPos.z = ef_sort.vTargetPos.z + 5.0f + ( (float)(rand()%70) * (rand()%2 == 0? -1.0f:1.0f) );
					efSort.vPos.y = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(efSort.vPos.x, efSort.vPos.z);

					pCMyApp->m_pFxSet->InsertFxEffect(efSort);

					if (g_pDSound)
					{
						float x, y, z;
						x = (ef_sort.vPos.x - g_pRoh->m_wx) / g_fDSDistance;
						y = (ef_sort.vPos.y - g_pRoh->m_wy) / g_fDSDistance;
						z = (ef_sort.vPos.z - g_pRoh->m_wz) / g_fDSDistance;

						g_pDSound->Play(g_EffectSound[EFF_SND_LIGHTNING], x, y, z);
					}
				}
			}
			ef_sort.nTargetType ++;

			if( ef_sort.nTargetType >= 2 * 10)
				ef_sort.num = 2;
		}
		break;

	case 4:
		{
			CNkMob* pMob = ef_sort.pMobFrom;

			if (g_pDSound)
			{
				float x, y, z;
				x = (pMob->m_wx - g_pRoh->m_wx) / g_fDSDistance;
				y = (pMob->m_wy - g_pRoh->m_wy) / g_fDSDistance;
				z = (pMob->m_wz - g_pRoh->m_wz) / g_fDSDistance;

				g_pDSound->Play(g_EffectSound[EFF_SND_LIGHTNING], x, y, z);
			}

			ef_sort.num = 0;
		}
		break;
	}

}

BOOL CFxBossRaidKai2_03::Render(EffectSort &ef_sort)
{
	if( ef_sort.num == 2)
		return TRUE;

	Update_Particle(ef_sort);

	switch(ef_sort.num)
	{
	case 0:
		RenderLightningBall(ef_sort);
		break;
	case 1:
		RenderMagicCycle(ef_sort);
		break;
	}

	return FALSE;
}



BOOL CFxBossRaidKai2_03::RenderLightningBall(EffectSort &ef_sort)
{
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

	Update_Particle(ef_sort);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) ef_sort.effects;

	if( m_Plane )
	{
		float fScale = 10.0f+ ef_sort.vSortPos.x +( (float)(rand()%5)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f) );
		int TextureRand =  (rand()%60)%3;
		switch( TextureRand )
		{
		case 0:
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] ) );
			break;
		case 1:
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLTAIL] ) );
			break;
		case 2:
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLBASE] ) );
			break;
		}

		m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
		m_Plane->Scale( fScale, fScale, fScale );
		m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Plane->SetTranslationXYZ( ef_sort.vCurPos.x, ef_sort.vCurPos.y, ef_sort.vCurPos.z);
		m_Plane->Render();

		fScale = 20.0f + ef_sort.vSortPos.x +( (float)(rand()%5)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f) );
		TextureRand =  EFF_FX_SHOCKSPEARLIGHT + (rand()%40)%4;
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ TextureRand ] ) );

		m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
		m_Plane->Scale( fScale, fScale, fScale );
		m_Plane->SetColor(0.3f, 0.2f, 1.0f, 1.0f);
		m_Plane->SetTranslationXYZ( ef_sort.vCurPos.x, ef_sort.vCurPos.y, ef_sort.vCurPos.z);
		m_Plane->Render();

		fScale = 20.0f + ef_sort.vSortPos.x +( (float)(rand()%5)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f) );
		TextureRand =  EFF_FX_SHOCKSPEARLIGHT + (rand()%40)%4;
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ TextureRand ] ) );

		m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
		m_Plane->Scale( fScale, fScale, fScale );
		m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Plane->SetTranslationXYZ( ef_sort.vCurPos.x, ef_sort.vCurPos.y, ef_sort.vCurPos.z);
		m_Plane->Render();
	}


	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return TRUE;
}

BOOL CFxBossRaidKai2_03::RenderMagicCycle(EffectSort &ef_sort)
{

	return TRUE;
}