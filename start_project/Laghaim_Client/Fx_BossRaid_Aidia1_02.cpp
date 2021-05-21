#include "stdafx.h"
// FxTornadoHammer.cpp: implementation of the CFxBossRaidKai3_01 class.
//
//////////////////////////////////////////////////////////////////////

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
#include "FX_BOSSRAID_AIDIA1_02.h"
#include "Land.h"
#include "UIMgr.h"

extern float				g_fDSDistance;



extern int					g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define FIRE_SCALE_SMALL_SIZE	1.0f
#define FIRE_SCALE_BIG_SIZE		40.0f
#define LITHIN_MAX_SIZE			85.0f

CFxBossRaidAidia1_02::CFxBossRaidAidia1_02()
{
	m_Plane = NULL;
	Create();
}

CFxBossRaidAidia1_02::~CFxBossRaidAidia1_02()
{
	SAFE_DELETE(m_Plane);
}

void CFxBossRaidAidia1_02::Create()
{
	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}

void CFxBossRaidAidia1_02::LoadRes()
{

}

void CFxBossRaidAidia1_02::DeleteRes()
{}


void CFxBossRaidAidia1_02::FrameMove( EffectSort &eff, int nAddFrame )
{
	int WhileCount = FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1 + FX_BOSSRAID_AIDIA1_02_MAXCOUNT_2;
	Deco_Particle *pDPar = (Deco_Particle*)eff.pParticle;
	CNkMob *pMob = NULL;
	pMob = eff.pMobFrom;

	if(pMob == NULL)
		return;

	// 지팡이 끝 위치를 계속 갱신해준다.
	D3DMATRIX matRot;
	D3DVECTOR vTmp;

	D3DUtil_SetIdentityMatrix(matRot);
	Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
	D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[0], matRot);
	eff.vPos = vTmp;
	eff.vPos.x += pMob->m_wx;
	eff.vPos.y += pMob->m_wy;
	eff.vPos.z += pMob->m_wz;

	switch( eff.num )
	{
	case 0: // 파티클 관련 정보를 셋팅한다.
		{
			for( int i=0; i<WhileCount; i++ )
			{
				if( i < FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1 )
				{
					// 기가 모이는 이펙트
					pDPar[i].scale	= rand()%55 + LITHIN_MAX_SIZE/2;; // 크기
					pDPar[i].direction.x = 3.0f + ((float)(rand()%7)); // 스케일 속도
					pDPar[i].red	= (float)(rand()%10) + (float)(rand()%10)/10.0f; // 회전
					pDPar[i].cur_time = EFF_FX_BOSSRAID_AIDIA1_02_LIGHTIN;
				}
				else
				{
					// 모여 있는 기 이펙트
					pDPar[i].scale	= FIRE_SCALE_SMALL_SIZE + rand()%3;	// 크기
					pDPar[i].cur_time = EFF_FX_BOSSRAID_AIDIA1_02_STICKFIRE_00 + rand()%8; // 텍스쳐 에니메이션 프레임
					pDPar[i].life_time = 1; // 스케일 크기 변환 flag
					pDPar[i].red	= (float)(rand()%360); // 회전

					pDPar[i].cur_Pos.x = (float)(rand()%4) * ((float)((rand()%2)%2 == 0)?1:-1);
					pDPar[i].cur_Pos.y = (float)(rand()%4) * ((float)((rand()%2)%2 == 0)?1:-1);
					pDPar[i].cur_Pos.z = (float)(rand()%4) * ((float)((rand()%2)%2 == 0)?1:-1);
				}
			}

			eff.num = 1;
		}
		break;

	case 1: // 파티클을 실시간 계산한다.
		{
			for( int i=0; i<WhileCount; i++ )
			{
				if( i < FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1 )
				{
					// 기가 모이는 이펙트

					pDPar[i].red += 0.01f; //
					if(pDPar[i].red > 360.0f)
						pDPar[i].red  -= 360.0f;

					pDPar[i].scale	-= pDPar[i].direction.x; // 크기
					if( pDPar[i].scale < 0.5f )
					{
						pDPar[i].scale	= rand()%35 + LITHIN_MAX_SIZE; // 크기
						pDPar[i].direction.x = 3.0f + ((float)(rand()%6)); // 스케일 속도
						pDPar[i].red	= (float)(rand()%10) + (float)(rand()%10)/10.0f; // 회전
					}
				}
				else
				{
					// 모여 있는 기 이펙트
					if( pDPar[i].scale < FIRE_SCALE_BIG_SIZE )
						pDPar[i].scale += (float)((rand()%2))+0.3f;///10.0f;	// 크기 확대

					pDPar[i].cur_time++;// 텍스쳐 에니메이션 프레임

					if( pDPar[i].cur_time > EFF_FX_BOSSRAID_AIDIA1_02_STICKFIRE_07 )
						pDPar[i].cur_time = EFF_FX_BOSSRAID_AIDIA1_02_STICKFIRE_00;

					pDPar[i].red += 0.1f; //
					if(pDPar[i].red > 360.0f)
						pDPar[i].red  -= 360.0f;
				}
			}

			// 쇼크웨이브 이펙트를 호출한다.
			if( pMob->GetCurFrame() >= 58 )
			{
				EffectSort efSort;
				efSort.nType = FX_BOSSRAID_AIDIA1_02_SHOCKWAVE;

				efSort.vPos.x = eff.vPos.x;
				efSort.vPos.z = eff.vPos.z;
				efSort.vPos.y = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(eff.vPos.x, eff.vPos.z);

				pCMyApp->m_pFxSet->InsertFxEffect(efSort);

				eff.num = 2;
			}
		}
		break;

	case 2:	// 모앗던 기를 크게 스케일 하고 사라지게 한다.
		{
			if( pDPar[FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1+1].scale > FIRE_SCALE_BIG_SIZE * 8)
				eff.num = 3;

			for( int i=FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1; i<WhileCount; i++ )
			{
				pDPar[i].cur_time++;// 텍스쳐 에니메이션 프레임

				if( pDPar[i].cur_time > EFF_FX_BOSSRAID_AIDIA1_02_STICKFIRE_07 )
					pDPar[i].cur_time = EFF_FX_BOSSRAID_AIDIA1_02_STICKFIRE_00;

				pDPar[i].scale += 30.0f;
			}

		}
		break;

	case 3:	// 이펙트를 종료한다.
		{


		}
		break;
	}
}

BOOL CFxBossRaidAidia1_02::Render(EffectSort &eff)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	if( eff.num == 3)
		return TRUE;

	if( eff.pMobFrom->GetMotionState() != MOB_EMPTY1 )
		return TRUE;

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

	D3DVECTOR	vViewVec, vCurPos;
	vViewVec = pCMyApp->GetEyeDirection();

	Deco_Particle* pDpar = (Deco_Particle*)eff.pParticle;

	if( m_Plane )
	{
		int start = 0;

		if( eff.num == 2 )
			start = FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1;

		for(int i=start; i<FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1+FX_BOSSRAID_AIDIA1_02_MAXCOUNT_2; i++)
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ pDpar[i].cur_time ] ) ); // 텍스쳐 지정

			vCurPos = eff.vPos;

			if(i >=  FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1)
				vCurPos += pDpar[i].cur_Pos;

			m_Plane->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_Plane->Scale( pDpar[i].scale, pDpar[i].scale, pDpar[i].scale );
			m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_Plane->SetTranslationXYZ( vCurPos.x, vCurPos.y, vCurPos.z);
			m_Plane->RotateZ( pDpar[i].red );
			m_Plane->Render();
		}
	}


	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}

