#include "stdafx.h"
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
#include "Land.h"

#include "FX_BOSSRAID_LastBoss_04.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidLastBoss_04::CFxBossRaidLastBoss_04()
	: end_idx(0)
{
	m_Plane = 0;
	m_pLolo = 0;
}


CFxBossRaidLastBoss_04::~CFxBossRaidLastBoss_04()
{
	DeleteRes();
}

void CFxBossRaidLastBoss_04::LoadRes()
{
	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 2.0f, 2.0f, FALSE );

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/finalboss_fire.LOL");
	}





}

void CFxBossRaidLastBoss_04::DeleteRes()
{
	SAFE_DELETE( m_Plane );
	SAFE_DELETE( m_pLolo );
}

int CFxBossRaidLastBoss_04::GetMovingRange(int nFrame)
{
	static int limit[FX_BOSSRAID_LASTBOSS_04_FLAME_FRAME] =
	{
		37, 38, 39, 40, 41,
		42, 43, 44, 45, 46,
		47, 48, 49, 50, 51,
		52, 53, 54, 55, 56,
		57, 59, 60, 61, 62,
		63, 65, 66, 67, 69,
	};

	int idx = 0;
	int sum = 0;
	for(; idx < FX_BOSSRAID_LASTBOSS_04_FLAME_FRAME; ++idx)
	{
		if ( nFrame < limit[idx])
			return sum;
		else
			++sum;
	}
	return sum;
}

Deco_Particle* CFxBossRaidLastBoss_04::MakeParticle(float dir)
{
	Deco_Particle* deco_particle = new Deco_Particle[FX_BOSSRAID_LASTBOSS_04_FLAME_COUNT];
	memset(deco_particle, FX_BOSSRAID_LASTBOSS_04_FLAME_COUNT, FX_BOSSRAID_LASTBOSS_04_FLAME_COUNT*sizeof(Deco_Particle));

	for(int i = 0; i < FX_BOSSRAID_LASTBOSS_04_FLAME; ++i)
	{
		for(int j = 0; j < FX_BOSSRAID_LASTBOSS_04_FLAME_FRAME; ++j)
		{
			D3DMATRIX matRotate;
			D3DVECTOR vFirePos0(m_pLolo->m_LoloObjs[i].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][0].x,
								m_pLolo->m_LoloObjs[i].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][0].y,
								m_pLolo->m_LoloObjs[i].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][0].z);

			D3DVECTOR vFirePos1(m_pLolo->m_LoloObjs[i].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][1].x,
								m_pLolo->m_LoloObjs[i].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][1].y,
								m_pLolo->m_LoloObjs[i].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][1].z);

			D3DUtil_SetRotateYMatrix(matRotate, -dir);
			D3DMath_VectorMatrixMultiply( vFirePos0, vFirePos0, matRotate );
			D3DMath_VectorMatrixMultiply( vFirePos1, vFirePos1, matRotate );

			D3DVECTOR vFireDir;
			get_normal_vector(vFireDir, vFirePos0, vFirePos1);

			deco_particle[ ParticleOffset(i, j) ].fade = 1.0f;
			deco_particle[ ParticleOffset(i, j) ].scale = 10.0f;
			deco_particle[ ParticleOffset(i, j) ].cur_Pos = vFirePos0;
			deco_particle[ ParticleOffset(i, j) ].direction = vFireDir * 6.0f;
			deco_particle[ ParticleOffset(i, j) ].cur_time = 0;
			deco_particle[ ParticleOffset(i, j) ].start_time = FX_BOSSRAID_LASTBOSS_04_FRAME1;
		}
	}

	return deco_particle;
}

D3DVECTOR CFxBossRaidLastBoss_04::GetDeltaVector(float rx, float ry, float dx, float dy)
{
	D3DMATRIX matRx, matRy, matWorld;
	D3DVECTOR vLight = D3DVECTOR(0, 0, 2);

	D3DUtil_SetRotateXMatrix(matRx, rx+dx);
	D3DUtil_SetRotateYMatrix(matRy, ry+dy);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);
	D3DMath_VectorMatrixMultiply( vLight, vLight, matWorld );
	return vLight;
}
void CFxBossRaidLastBoss_04::RenderFireBall(D3DVECTOR vViewVec, D3DVECTOR vPos,
		float alpha, float gamma, int time)
{
	if( m_Plane )
	{
		m_Plane->SetTranslationXYZ(vPos.x, vPos.y,  vPos.z);
		m_Plane->Render();
		m_Plane->SetColor(alpha, alpha, alpha, alpha);
		m_Plane->SetBillBoard( vViewVec, FALSE, 0.f );
		m_Plane->Scale(gamma, gamma, gamma);

		m_Plane->SetTranslationXYZ(vPos.x, vPos.y, vPos.z);
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE1 + time ] ));
		m_Plane->Render();
	}
}

bool CFxBossRaidLastBoss_04::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return FALSE;

	if(!effect_sort.pMobFrom)
		return FALSE;

	D3DVECTOR Eye;
	pCMyApp->GetEyePos( Eye );
	D3DVECTOR At;
	pCMyApp->GetEyeAtPos( At );

	D3DVECTOR vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

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

	int nLoloFrame = effect_sort.nCurFrame % 76;
	int nFireFrame = effect_sort.nCurFrame - FX_BOSSRAID_LASTBOSS_04_FRAME1;

	if (effect_sort.nCurFrame >= FX_BOSSRAID_LASTBOSS_04_FRAME0 &&
			effect_sort.nCurFrame < FX_BOSSRAID_LASTBOSS_04_FRAME1)
	{
		D3DMATRIX matRotate;

		Deco_Particle* deco_particle = effect_sort.pParticle;

		//불 그리기
		for(int idx = 0; idx < FX_BOSSRAID_LASTBOSS_04_FLAME; ++idx)
		{
			for( int sub_idx = 0; sub_idx < FX_BOSSRAID_LASTBOSS_04_FLAME_FRAME; ++sub_idx)
			{
				D3DVECTOR vFirePos0( m_pLolo->m_LoloObjs[idx].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][1].x,
									 m_pLolo->m_LoloObjs[idx].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][1].y,
									 m_pLolo->m_LoloObjs[idx].m_Vertices[FX_BOSSRAID_LASTBOSS_04_FRAME1][1].z);

				D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pMobFrom->m_dir);
				D3DMath_VectorMatrixMultiply( vFirePos0, vFirePos0, matRotate );
			}
		}
	}

	if (effect_sort.nCurFrame >= FX_BOSSRAID_LASTBOSS_04_FRAME1-1 &&
			effect_sort.nCurFrame < FX_BOSSRAID_LASTBOSS_04_FRAME3)
	{
		Deco_Particle* deco_particle = effect_sort.pParticle;
		for(int idx = 0; idx < FX_BOSSRAID_LASTBOSS_04_FLAME; ++idx)
		{
			for(int sub_idx = 0; sub_idx < FX_BOSSRAID_LASTBOSS_04_FLAME_FRAME; ++sub_idx)
			{
				int texIndex;
				float rx, ry, alpha, gamma;
				D3DVECTOR vFirePos0, vFirePos1;
				D3DVECTOR vFirePos, vFireDir, vFireDir2;

				if (sub_idx >= GetMovingRange(effect_sort.nCurFrame))
				{
					Deco_Particle& flame = deco_particle[ ParticleOffset(idx, sub_idx) ];

					D3DMATRIX matRotate;
					vFirePos0 = D3DVECTOR(m_pLolo->m_LoloObjs[idx].m_Vertices[nLoloFrame][0].x,
										  m_pLolo->m_LoloObjs[idx].m_Vertices[nLoloFrame][0].y,
										  m_pLolo->m_LoloObjs[idx].m_Vertices[nLoloFrame][0].z);

					vFirePos1 = D3DVECTOR(m_pLolo->m_LoloObjs[idx].m_Vertices[nLoloFrame][1].x,
										  m_pLolo->m_LoloObjs[idx].m_Vertices[nLoloFrame][1].y,
										  m_pLolo->m_LoloObjs[idx].m_Vertices[nLoloFrame][1].z);

					D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pMobFrom->m_dir);
					D3DMath_VectorMatrixMultiply( vFirePos0, vFirePos0, matRotate );
					D3DMath_VectorMatrixMultiply( vFirePos1, vFirePos1, matRotate );

					D3DVECTOR vFireDir;
					get_normal_vector(vFireDir, vFirePos0, vFirePos1);

					flame.fade = 1.0f;
					flame.scale = 4.0f;
					flame.cur_Pos = vFirePos0;
					flame.gravity = vFireDir;
					flame.direction = vFireDir * 6.0f;
					flame.cur_time = 0;
					flame.life_time = effect_sort.nCurFrame; //애니메이션의 시작 프레임 갱신
					flame.start_time = FX_BOSSRAID_LASTBOSS_04_FRAME1;
				}
				else
				{
					Deco_Particle& flame = deco_particle[ ParticleOffset(idx, sub_idx) ];

					vFireDir = flame.direction;
					vFirePos = vFireDir + effect_sort.vCurPos + flame.cur_Pos;

					ry = atan2f(vFireDir.x, vFireDir.z);
					rx = atan2f(vFireDir.x*sinf(ry)+vFireDir.z*cosf(ry), vFireDir.y) - (g_PI / 2.0f) ;

					//일반 상태 방정식 계산
					while( flame.life_time <= effect_sort.nCurFrame )
					{
						if (flame.scale < 30)
						{
							flame.fade *= 0.94f;
							flame.scale *= 1.11f;
							flame.direction *= 0.96f;
							flame.cur_Pos += vFireDir;
							flame.direction.y -= 0.06f;
						}
						else if (flame.scale < 80)
						{
							flame.fade *= 0.94f;
							flame.scale *= 1.03f;
							flame.direction *= 0.96f;
							flame.cur_Pos += vFireDir;
							flame.direction.y -= 0.06f;
						}
						else
						{
							flame.fade *= 0.72f;
							flame.scale *= 0.90f;
							flame.direction *= 0.96f;
							flame.cur_Pos += vFireDir;
							flame.direction.y -= 0.06f;
						}

						++flame.cur_time;
						++flame.life_time;
					}

					alpha = flame.fade;
					gamma = flame.scale;
					texIndex = flame.cur_time % 10;

					RenderFireBall(vViewVec, vFirePos, alpha, gamma, texIndex);

					++texIndex %= 10;

					alpha = flame.fade * 0.8;
					gamma = flame.scale * 0.4;
					vFireDir2 = GetDeltaVector(rx, ry, g_PI*0.5f, 0.0f);
					RenderFireBall(vViewVec, vFirePos+vFireDir2, alpha, gamma, texIndex);

					alpha = flame.fade * 0.8;
					gamma = flame.scale * 0.4;
					vFireDir2 = GetDeltaVector(rx, ry, g_PI*-0.5f, 0.0f);
					RenderFireBall(vViewVec, vFirePos+vFireDir2, alpha, gamma, texIndex);

					alpha = flame.fade * 0.8;
					gamma = flame.scale * 0.4;
					vFireDir2 = GetDeltaVector(rx, ry, 0.0f, g_PI*0.5f );
					RenderFireBall(vViewVec, vFirePos+vFireDir2, alpha, gamma, texIndex);

					alpha = flame.fade * 0.8;
					gamma = flame.scale;
					vFireDir2 = GetDeltaVector(rx, ry, 0.0f, g_PI*-0.5f );
					RenderFireBall(vViewVec, vFirePos+vFireDir2, alpha, gamma, texIndex);

					alpha = flame.fade * 0.1;
					gamma = flame.scale * 2.0f;
					vFireDir2 = GetDeltaVector(rx, ry, 0.0f, g_PI*-0.5f );
					RenderFireBall(vViewVec, vFirePos, alpha, gamma, texIndex);
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

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);

	return FALSE;
}
