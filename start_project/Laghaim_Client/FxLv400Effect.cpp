#include "stdafx.h"
#define D3D_OVERLOADS
#include "main.h"
#include "ImageDefine.h"
#include "CapSyong.h"
#include "FxSet.h"
#include "UIMgr.h"
#include "FxLv400Effect.h"
#include "Skill.h"

CFxLv400Effect::CFxLv400Effect(void)
	:	m_Plane(new CRectPlane)
{
	Create();
}

CFxLv400Effect::~CFxLv400Effect(void)
{
	SAFE_DELETE(m_Plane);
}

void CFxLv400Effect::LoadRes()
{

}

void CFxLv400Effect::DeleteRes()
{

}

void CFxLv400Effect::Create()
{
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}

void CFxLv400Effect::FrameMove( EffectSort &eff, int nAddFrame )
{
	Deco_Particle* dp = (Deco_Particle*)eff.effects;

	D3DMATRIX matTrans;
	D3DVECTOR vScr, vTemp;

	int actNum = 4;
	CBiped* pBpd =  eff.pNkChaTo->GetBiped();
	int CurFrame = eff.pNkChaTo->Get_m_CurFrame();

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
	case 0:
		eff.vPos.x = 0.0f;
		eff.vPos.y = 0.0f;
		eff.vPos.z = 0.0f;

		switch( eff.pNkChaTo->m_Race )
		{
		case RACE_AIDIA:
			eff.vSortPos.x = 38.0f;
			eff.vSortPos.y = 38.0f;
			eff.vSortPos.z = 18.0f;
			break;
		default:
			eff.vSortPos.x = 43.0f;
			eff.vSortPos.y = 43.0f;
			eff.vSortPos.z = 23.0f;
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

BOOL CFxLv400Effect::Render(EffectSort &eff)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	FrameMove(eff, 0);

	DWORD  dwTextureState[3] = {0,0,0};

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	pCMyApp->GetTSS_Alpha(0,dwTextureState[0],dwTextureState[1],dwTextureState[2]);
	pCMyApp->SetTSS_Alpha(0,D3DTA_TEXTURE,D3DTA_TEXTURE,D3DTOP_MODULATE);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();
	Deco_Particle* dp = (Deco_Particle*)eff.effects;
	if( m_Plane )
	{
		for(int i=0; i<MAX_EFFECT_PLANE; i++)
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

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(GetSkillTexture(eff.nType, i)));

			m_Plane->SetBillBoard(vViewVec, FALSE, 0.0f);
			m_Plane->Scale( size, size, size);
			m_Plane->SetTranslationXYZ( eff.vCurPos.x, eff.vCurPos.y, eff.vCurPos.z);
			m_Plane->RotateZ( rotate );

			m_Plane->Render();
		}
	}

	pCMyApp->SetTSS_Alpha(0,dwTextureState[0],dwTextureState[1],dwTextureState[2]);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

	return FALSE;
}

int	CFxLv400Effect::GetSkillTexture(int type, int frame)
{
	switch(type)
	{
	case FX_LV400_EFFECT_GREEN:
		return  g_EffectTextr[ EFF_FX_LV400_EFFECT_G01+frame ];
	case FX_LV450_EFFECT_BLUE:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_B01+frame ];
	case FX_LV500_EFFECT_RED:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_R01+frame ];
	case FX_LV550_EFFECT_YELLOW:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_Y01+frame ];
	case FX_LV600_EFFECT_PURPLE:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_P01+frame ];
	case FX_LV650_EFFECT_RAINBOW1:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_F01+frame ];
	case FX_LV700_EFFECT_RAINBOW2:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_M01+frame ];
	case FX_LV750_EFFECT_WHITE:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_W01+frame ];
	case FX_LV800_EFFECT_BLACK:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_BL01+frame ];
	case FX_LV850_EFFECT_ORANGE:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_N01+frame ];
	case FX_LV900_EFFECT_RAINBOW3:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_X01+frame ];
	case FX_LV950_EFFECT_RAINBOW4:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_Z01+frame ];
	case FX_LV1000_EFFECT_PINK:
		return g_EffectTextr[ EFF_FX_LV400_EFFECT_PI01+frame ];
	}

	return 0;
}