#include "stdafx.h"
#define D3D_OVERLOADS


#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "UIMgr.h"
#include "Land.h"
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
#include "Fx_BossRaid_human2_03.h"



CFxBossRaidHuman2_03::CFxBossRaidHuman2_03()
	: m_pLolo(NULL)
	, m_pLolo1(NULL)
	, m_pLolo2(NULL)
{
	vControl[0] = D3DVECTOR(0, 0, 0);
	vControl[1] = D3DVECTOR(0, 0, 0);
	vControl[2] = D3DVECTOR(0, 0, 0);
	vControl[3] = D3DVECTOR(0, 0, 0);

	m_CirclePlane = 0;
}

CFxBossRaidHuman2_03::~CFxBossRaidHuman2_03()
{
	DeleteRes();
}

void CFxBossRaidHuman2_03::LoadRes()
{
	m_CirclePlane = new CRectPlane;
	if( m_CirclePlane )
		m_CirclePlane->Create( 2.0f, 2.0f, FALSE );
}

void CFxBossRaidHuman2_03::DeleteRes()
{
	SAFE_DELETE(m_CirclePlane);
}

float BiClamp(float alpha, float beta, float delta)
{
	float gamma;
	if (delta > 0)
	{
		gamma = alpha + delta;
		return (gamma > beta) ? beta : gamma;
	}
	else
	{
		gamma = alpha + delta;
		return (gamma < beta) ? beta : gamma;
	}
}

void CFxBossRaidHuman2_03::RenderWave(D3DVECTOR& pos, D3DVECTOR& scale, float alpha)
{
	if( m_CirclePlane )
	{
		m_CirclePlane->SetColor(alpha, alpha, alpha, alpha);
		m_CirclePlane->SetRotationX( g_PI * 0.5f );
		m_CirclePlane->Scale(scale.x, scale.y, scale.z);
		m_CirclePlane->SetTranslationXYZ(pos.x, pos.y+1.0f, pos.z);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BOSSRAID_BULKAN2_2_SHOCK ] ));
		m_CirclePlane->Render();
	}
}

void CFxBossRaidHuman2_03::RenderLaser(D3DVECTOR& view, D3DVECTOR& pos, D3DVECTOR& scale, float alpha, float rotate)
{
	if( m_CirclePlane )
	{
		m_CirclePlane->SetColor(alpha, alpha, alpha, alpha);
		m_CirclePlane->SetRotationX( g_PI * 0.5f );
		m_CirclePlane->SetBillBoard( view, FALSE, 0.0f);
		m_CirclePlane->RotateZ( rotate );
		m_CirclePlane->Scale(scale.x, scale.y, scale.z);
		m_CirclePlane->SetTranslationXYZ(pos.x, pos.y, pos.z );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BOSSRAID_HUMAN2_03_LASER ] ));
		m_CirclePlane->Render();
	}
}

bool CFxBossRaidHuman2_03::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return FALSE;

	//-- 사용 캐릭이 사라질 경우
	if(!effect_sort.pMobFrom)
		return FALSE;

	D3DVECTOR vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView, matRotate, matWorld, matTemp;

	//변수 설정
	DWORD srcBlendOld, destBlendOld;
	DWORD alphaBlend, dwLighting, dwAmbient, dwFogEnable;

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffffff );

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );

	//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	D3DVECTOR v1;

	//에너지 모으기
	if (effect_sort.nCurFrame >= 0 && effect_sort.nCurFrame < 14 )
	{
		D3DUtil_SetIdentityMatrix(matRotate);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.pMobFrom->m_dir );
		D3DMath_VectorMatrixMultiply( v1, effect_sort.pMobFrom->m_pCapSyong->m_LineVector[0], matRotate );
		v1.x += effect_sort.pMobFrom->m_wx;
		v1.y += effect_sort.pMobFrom->m_wy;
		v1.z += effect_sort.pMobFrom->m_wz;

		D3DVECTOR scale;
		float nDelta = effect_sort.nCurFrame;
		float fAlpha = (float)effect_sort.nCurFrame / 14;

		fAlpha = fAlpha * fAlpha;
		scale.x = FX_BOSSRAID_HUMAN2_03_LASER_SIZE1 / sqrtf(nDelta);
		scale.y = FX_BOSSRAID_HUMAN2_03_LASER_SIZE1 / sqrtf(nDelta);
		scale.z = FX_BOSSRAID_HUMAN2_03_LASER_SIZE1 / sqrtf(nDelta);

		RenderLaser(vViewVec, v1, scale, fAlpha, nDelta * 0.1f);
	}
	//에너지 발사하기
	else if(effect_sort.nCurFrame >= 14)
	{
		D3DUtil_SetIdentityMatrix(matRotate);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.pMobFrom->m_dir );
		D3DMath_VectorMatrixMultiply( v1, effect_sort.pMobFrom->m_pCapSyong->m_LineVector[0], matRotate );
		v1.x += effect_sort.pMobFrom->m_wx;
		v1.y += effect_sort.pMobFrom->m_wy;
		v1.z += effect_sort.pMobFrom->m_wz;

		int nFrame = effect_sort.nCurFrame - 14;

		int nCount = nFrame / (2+(rand()%2));
		if (nCount > FX_BOSSRAID_HUMAN2_03_MAX_LASER)
			nCount = FX_BOSSRAID_HUMAN2_03_MAX_LASER;

		for( int i = 0; i < nCount; ++i)
		{
			Deco_Particle* pObj = effect_sort.pParticle;

			//날아가지 않는 볼
			if (pObj[i].texFrame == 0)
			{
				pObj[i].texFrame = 1;
				pObj[i].first_Pos = v1;
				pObj[i].cur_Pos = v1;
				pObj[i].direction.x = effect_sort.vPos.x + (rand()%61-30);
				pObj[i].direction.z = effect_sort.vPos.z + (rand()%61-30);
				pObj[i].direction.y	= effect_sort.vPos.y + 6.0f;
				pObj[i].gravity = Normalize( pObj[i].direction - v1) * 60.0f;

				D3DVECTOR pos;
				pos.x = pObj[i].cur_Pos.x;
				pos.y = pObj[i].cur_Pos.y;
				pos.z = pObj[i].cur_Pos.z;

				D3DVECTOR scale;
				scale.x = FX_BOSSRAID_HUMAN2_03_LASER_SIZE2 * 1.25;
				scale.y = FX_BOSSRAID_HUMAN2_03_LASER_SIZE2 * 1.25;
				scale.z = FX_BOSSRAID_HUMAN2_03_LASER_SIZE2 * 1.25;

				RenderLaser(vViewVec, pos, scale, 1.0f);
			}
			else
			{
				pObj[i].cur_Pos.x = BiClamp( pObj[i].cur_Pos.x, pObj[i].direction.x, pObj[i].gravity.x );
				pObj[i].cur_Pos.y = BiClamp( pObj[i].cur_Pos.y, pObj[i].direction.y, pObj[i].gravity.y );
				pObj[i].cur_Pos.z = BiClamp( pObj[i].cur_Pos.z, pObj[i].direction.z, pObj[i].gravity.z );

				if (pObj[i].texFrame == 1 &&
						pObj[i].cur_Pos == pObj[i].direction )
				{
					pObj[i].texFrame = 2;
					pObj[i].start_time = effect_sort.nCurFrame;
				}

				//충격파
				if (pObj[i].texFrame == 2)
				{
					float nDelta = effect_sort.nCurFrame - pObj[i].start_time;
					float nScale = sqrtf(nDelta) * FX_BOSSRAID_HUMAN2_03_EXPLOSION;
					float fAlpha = 1.0f - nDelta/10.f;
					if (fAlpha < 0) fAlpha = 0;


					D3DVECTOR pos;
					pos.x = pObj[i].cur_Pos.x;
					pos.y = pObj[i].cur_Pos.y;
					pos.z = pObj[i].cur_Pos.z;

					D3DVECTOR scale;
					scale.x = nScale;
					scale.y = nScale;
					scale.z = nScale;

					RenderWave(pos, scale, fAlpha);
				}
				//날아가는 에너지
				else
				{
					D3DVECTOR pos;
					pos.x = pObj[i].cur_Pos.x;
					pos.y = pObj[i].cur_Pos.y;
					pos.z = pObj[i].cur_Pos.z;

					D3DVECTOR scale;
					scale.x = FX_BOSSRAID_HUMAN2_03_LASER_SIZE3;
					scale.y = FX_BOSSRAID_HUMAN2_03_LASER_SIZE3;
					scale.z = FX_BOSSRAID_HUMAN2_03_LASER_SIZE3;

					RenderLaser(vViewVec, pos, scale, 1.0f);

					scale.x = FX_BOSSRAID_HUMAN2_03_LASER_SIZE2;
					scale.y = FX_BOSSRAID_HUMAN2_03_LASER_SIZE2;
					scale.z = FX_BOSSRAID_HUMAN2_03_LASER_SIZE2;

					RenderLaser(vViewVec, v1, scale, 0.5f);
				}
			}
		}
	}
	//상태 복원
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld );

	return false;
}
