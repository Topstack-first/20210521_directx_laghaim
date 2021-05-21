#include "stdafx.h"
// FxSummonFire.cpp: implementation of the CFxSummonFire class.
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
#include "DecoObj.h"
#include "FxSummonFire.h"


// nate 2004 - 7 글로벌 ShadowRect사용
extern float g_fDSDistance;

extern int g_EffectSound[];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSummonFire::CFxSummonFire()
{
	m_fScale = 0.0f;
	m_pLolo = NULL;
	m_pLolo2 = NULL;
	m_nTotalFrame = FX_SUMMONFIRE_FRAMECOUNT;

	m_rect_magic = NULL;
	m_rect_magic = new CRectPlane;
	if( m_rect_magic )
		m_rect_magic->Create( 30.0f, 30.0f, FALSE );
}

CFxSummonFire::~CFxSummonFire()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(m_pLolo2);
	SAFE_DELETE(m_rect_magic);
}

void CFxSummonFire::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/eff_summon_fire_start.LOL");
	}

	if (!m_pLolo2)
	{
		m_pLolo2 = new CLolos;
		m_pLolo2->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo2->Read("data/effect/lolos/eff_summon_fire_attack.LOL");
	}



}

BOOL CFxSummonFire::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	//-- 사용 캐릭이 사라질 경우
	if(!effect_sort.pNkChaFrom)
		return TRUE;

	float maxScale = 3.0f;

	m_fScale = (maxScale * effect_sort.nCurFrame) / m_nTotalFrame;

	BOOL bFinished = FALSE;
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

	D3DVECTOR	vDrawVec, vtestVec;
	if( effect_sort.pNkChaFrom )
	{
		vDrawVec = D3DVECTOR(effect_sort.pNkChaFrom->m_wx,
							 effect_sort.pNkChaFrom->m_wy,
							 effect_sort.pNkChaFrom->m_wz);
	}

	vtestVec.dvX=0;
	vtestVec.dvY =-10;
	vtestVec.dvZ = 0;

	if( effect_sort.nCurFrame > 15 && effect_sort.nCurFrame < 50 )
	{
		if( m_rect_magic )
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SCROLLRING ] ) );

			m_rect_magic->SetBillBoard(vtestVec, FALSE, 0.0f);
			m_rect_magic->SetColor(1.0f, 0.5f, 0.2f, 1.0f);
			m_rect_magic->SetTranslationXYZ(vDrawVec.x, vDrawVec.y + 22.0f, vDrawVec.z);
			m_rect_magic->RotateZ( m_fScale );
			m_rect_magic->Render();
		}
	}

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DMATRIX matTrans, matView,matRotate, matWorld,matWorld2,matTemp;
	D3DMATRIX matRx, matRy;

	int nFrame;
	nFrame = effect_sort.nCurFrame;

	D3DMATRIX matRot;
	D3DVECTOR vTmp;


	D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pNkChaFrom->m_dir);
	D3DUtil_SetTranslateMatrix( matTrans, effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy, effect_sort.pNkChaFrom->m_wz);
	D3DMath_MatrixMultiply(matWorld, matRotate, matTrans);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	if( m_pLolo )
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

	if( effect_sort.nCurFrame > 5 && effect_sort.nCurFrame < 17 )
	{
		InsertFire( effect_sort, D3DVECTOR(effect_sort.pNkChaFrom->m_wx, effect_sort.pNkChaFrom->m_wy + 35.0f,
										   effect_sort.pNkChaFrom->m_wz) );
	}

	if( effect_sort.nCurFrame > 49 )
	{
		// 방향벡터를 구한다.
		D3DVECTOR vDir, vPosFrom, vPosTo;
		D3DVECTOR vDrawPos;

		// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
		vPosFrom = vDrawVec;
		vPosFrom.y += 3.0f;

		// 타겟 좌표를 구한다.
		if (effect_sort.pMobTo)
			vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
		else if (effect_sort.pNkChaTo)
			vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
		else
			vPosTo = effect_sort.vTargetPos;

		// 방향.
		get_normal_vector(vDir, vPosFrom, vPosTo);

		// 프레임당 스피드.
		vDir *= 10.0f;

		int end_frame;
		end_frame = nFrame;

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		vDrawPos.x = vPosFrom.x + vDir.x * (end_frame - 49);
		vDrawPos.y = vPosFrom.y + vDir.y * (end_frame - 49);
		vDrawPos.z = vPosFrom.z + vDir.z * (end_frame - 49);

		float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) -
					  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

		if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			vPosTo.y += 10.0f;

			ef_sort.nType = FX_SUMMONFIREBOOM;
			ef_sort.vPos = vPosFrom;
			ef_sort.vTargetPos = vPosTo;
			//ef_sort.vTargetPos = vDrawPos;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		}
		else
		{
			float fDir = atan2f(vDir.z, vDir.x);
			Nk3DUtil_SetRotateCCWYMatrix(matRotate, fDir);
			D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);
			D3DMath_MatrixMultiply( matWorld2, matRotate, matTrans );

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld2 );

			if( m_pLolo2 )
				m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

			InsertFire( effect_sort, D3DVECTOR(vDrawPos.x, vDrawPos.y + 30.0f, vDrawPos.z ) );
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	return bFinished;
}

void CFxSummonFire::InsertFire( EffectSort &effect_sort, D3DVECTOR vDrawVec ) // 이펙트넣는게 좀 길어서 함수로 분리.
{
	Deco_Particle *pFire_Particle = NULL;

	pFire_Particle = new Deco_Particle[MAX_FIRE_PARTICLE]; // 이 숫자를 멋대로 늘리면 안된다. // 이 녀석의 메모리 해제 타이밍은 CFxFireParticle 의 Render에서 제거시다.

	if( !pFire_Particle )
		return;

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ ) // 초기화. 특별히 값을 복잡하게 셋팅할 필요는 없다.
	{
		pFire_Particle[i].start_time = 0;
		pFire_Particle[i].life_time = 0;
		pFire_Particle[i].cur_time = 0;
		pFire_Particle[i].scale = 1.0f; // 이 인자는 디폴트 값이 다른 기존 CFxFireParticle 과 다르다. 타입이 생성->소멸 타입이기 때문에.
		pFire_Particle[i].fade = 1.0f;

		pFire_Particle[i].red = 1.0f;
		pFire_Particle[i].green = 1.0f;
		pFire_Particle[i].blue = 1.0f;

		pFire_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

		pFire_Particle[i].texIndex = 0;
		pFire_Particle[i].texFrame = 0;
	}

	// 실제 이펙트 추가
	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	ef_sort.nType = FX_FIREPARTICLE;
	ef_sort.effects = pFire_Particle;
	ef_sort.num = REDFIRE_ONETIME; // 타입
	ef_sort.vPos = vDrawVec; // 위치
	ef_sort.Index = 20; // 불꽃 크기.
	ef_sort.dwColor = 1; // 현재 라이프 초기화. (1부터 시작하도록)
	ef_sort.subNum = 25; // 라이프(프레임단위)
	ef_sort.vSortPos.x = 2.0f; // 초반 커지는 속도
	ef_sort.vSortPos.y = 1.0f; // 후반 꺼지는 속도

	pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
}
