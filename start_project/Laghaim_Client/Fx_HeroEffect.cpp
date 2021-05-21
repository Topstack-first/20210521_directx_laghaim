#include "stdafx.h"
// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트
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
#include "Fx_HeroEffect.h"
#include "Land.h"
#include "UIMgr.h"

extern float				g_fDSDistance;



extern int					g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFxHeroEffect::CFxHeroEffect()
{
	m_Plane = NULL;
	Create();
}

CFxHeroEffect::~CFxHeroEffect()
{
	SAFE_DELETE(m_Plane);
}

void CFxHeroEffect::Create()
{
	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}

void CFxHeroEffect::LoadRes()
{

}

void CFxHeroEffect::DeleteRes()
{}


void CFxHeroEffect::FrameMove( EffectSort &eff, int nAddFrame )
{
	Deco_Particle* dp = (Deco_Particle*)eff.effects;

	// 캐릭터 상체 바디 중앙에 그린다. 위치 값을 BPD에서 가져오장

//	g_PetBiped[m_ActNum].m_Bpd.m_AniMat[10].m_Mats[m_CurFrame]._41


	D3DMATRIX matTrans;
	D3DVECTOR vScr, vTemp;

	int actNum = 4;
	CBiped* pBpd =  eff.pNkChaTo->GetBiped();
	int CurFrame = eff.pNkChaTo->Get_m_CurFrame();

	// [2008/8/14 Theodoric] 여자 캐릭터의 스텐드 모션에서 피직 좌표에 문제가 있는 듯 아래 방식으로 해결 봄 ㅡㅡ;;
	if( eff.pNkChaTo->GetMotionState() == CHA_STAND && eff.pNkChaTo->m_Sex != 0 )
		CurFrame = eff.pNkChaTo->Get_m_CurFrame()/2;

	vTemp.x = pBpd->m_AniMat[actNum].m_Mats[CurFrame]._41;
	vTemp.y = pBpd->m_AniMat[actNum].m_Mats[CurFrame]._42;
	vTemp.z = pBpd->m_AniMat[actNum].m_Mats[CurFrame]._43;

	D3DUtil_SetIdentityMatrix(matTrans);
	D3DUtil_SetTranslateMatrix( matTrans, vTemp );
	Nk3DUtil_SetRotateCCWYMatrix( matTrans, eff.pNkChaTo->m_dir );
	D3DMath_VectorMatrixMultiply(vScr, vTemp, matTrans);

	eff.vCurPos.x = eff.pNkChaTo->m_wx + vScr.x;
	eff.vCurPos.y = eff.pNkChaTo->m_wy + vScr.y;
	eff.vCurPos.z = eff.pNkChaTo->m_wz + vScr.z;

	switch( eff.num )
	{
	case 0: // 필요한 내용을 셋팅하자.

		eff.vPos.x = 0.0f;
		eff.vPos.y = 0.0f;
		eff.vPos.z = 0.0f;

		switch( eff.pNkChaTo->m_Race )
		{
		case RACE_BULKAN:
		case RACE_KAILIPTON:
		case RACE_HUMAN:
		case RACE_FREAK:
		case RACE_PEROM:
			eff.vSortPos.x = 43.0f;
			eff.vSortPos.y = 43.0f;
			eff.vSortPos.z = 23.0f;
			break;
		case RACE_AIDIA:
			eff.vSortPos.x = 38.0f;
			eff.vSortPos.y = 38.0f;
			eff.vSortPos.z = 18.0f;
			break;
		}

		eff.num  = 1;
		break;

	case 1:

		eff.vPos.x += 0.003f;
		eff.vPos.y -= 0.005f;
		eff.vPos.z -= 0.005f;

		break;
	}
}

BOOL CFxHeroEffect::Render(EffectSort &eff)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	FrameMove(eff, 0);

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();
	Deco_Particle* dp = (Deco_Particle*)eff.effects;

	if( m_Plane )
	{
		for(int i=0; i<MAX_HEROEFFECT_PLANE; i++)
		{
			//	if( dp[i].texIndex == 2)
			{
				float rotate	= 0.0f;
				float size		= 0.0f;

				switch(i)
				{
				case 0:
					rotate	= eff.vPos.x + (((float)(rand()%3)*0.005f) * ((rand()%2 == 0)?-1:1));
					size	= eff.vSortPos.x + ((rand()%8) * ((rand()%2 == 0)?-1:1));
					break;
				case 1:
					rotate	= eff.vPos.y + (((float)(rand()%3)*0.005f) * ((rand()%2 == 0)?-1:1));
					size	= eff.vSortPos.y + ((rand()%8) * ((rand()%2 == 0)?-1:1));
					break;
				case 2:
					rotate	= eff.vPos.z;
					size	= eff.vSortPos.z;
					break;
				}


				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HERO_EFFECT01+i ]) ); // 텍스쳐 지정

				m_Plane->SetBillBoard(vViewVec, FALSE, 0.0f);
				m_Plane->Scale( size, size, size);
				m_Plane->SetTranslationXYZ( eff.vCurPos.x, eff.vCurPos.y, eff.vCurPos.z);
				m_Plane->RotateZ( rotate );

				m_Plane->Render();
			}
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

	return FALSE;
}

