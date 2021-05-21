#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "sfx.h"
#include "FxHorn.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "ChaAct.h"
#include "ItemProto.h"
#include "Skill.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "CsyManager.h"



//extern CChaAct	g_ChaBpdList[MAX_CHA_ACT_NUM];
//extern int g_ChaBpdIndex[ NUM_RACE*NUM_SEX ][ CHA_ACT_INDEX_NUM ];
extern float g_fDSDistance;


CFxHorn::CFxHorn()
{
	m_pLolo = NULL;
}

CFxHorn::~CFxHorn()
{
}

void CFxHorn::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/a_shape01.lol");
	}



}

void CFxHorn::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

void CFxHorn::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 10)
		{
			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();
		}

		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);

		if (effect_sort.nCurFrame > 6)
			m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame-6, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}

CFxSilverFly::CFxSilverFly()
{
	m_pLolo = NULL;
}

CFxSilverFly::~CFxSilverFly()
{
}

void CFxSilverFly::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/silverfly.LOL");
	}



}

void CFxSilverFly::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

void CFxSilverFly::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 10)
		{
			if (effect_sort.pMobTo)
			{
				effect_sort.pMobTo->Attacked();
			}
			else if (effect_sort.pNkChaTo)
			{
				effect_sort.pNkChaTo->Attacked();
			}
		}
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}

CFxRaven::CFxRaven()
{
	m_pLolo = NULL;
}

CFxRaven::~CFxRaven()
{
}

// nate 2004 - 8 ImageManager
void CFxRaven::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/a_raven.LOL");
	}



}

void CFxRaven::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

void CFxRaven::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if (effect_sort.nCurFrame >= 10 && effect_sort.nCurFrame < 12)
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_HORNBOOM;
			ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y + 10.0f, effect_sort.vPos.z );
			ef_sort.Index = HIT_RAVEN;

			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();

			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}

//////////////////////////////////////////////////////////////////////
// Ű�׽� �� ����.
//////////////////////////////////////////////////////////////////////
CFxKines::CFxKines()
{
	m_pLolo = NULL;
}

CFxKines::~CFxKines()
{
}

// nate 2004 - 8 ImageManager
void CFxKines::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/a_kynes.LOL");
	}



}

void CFxKines::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

void CFxKines::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if (effect_sort.nCurFrame >= 19 && effect_sort.nCurFrame < 21)
		{

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_HORNBOOM;
			ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y + 10.0f, effect_sort.vPos.z );
			ef_sort.Index = HIT_KINES;

			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();

			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}

//////////////////////////////////////////////////////////////////////
// �׿�������ũ �� ����.
//////////////////////////////////////////////////////////////////////
CFxNeo::CFxNeo()
{
	m_pLolo = NULL;
}

CFxNeo::~CFxNeo()
{
}

// nate 2004 - 8 ImageManager
void CFxNeo::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/a_neo.LOL");
	}



}

void CFxNeo::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

void CFxNeo::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if (effect_sort.nCurFrame >= 12 && effect_sort.nCurFrame < 14)
		{

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_HORNBOOM;
			ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y + 12.0f, effect_sort.vPos.z );
			ef_sort.Index = HIT_NEO;

			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();

			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}

		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}

//////////////////////////////////////////////////////////////////////
// ī���� �� ����.
//////////////////////////////////////////////////////////////////////
CFxKaisen::CFxKaisen()
{
	m_pLolo = NULL;
}

CFxKaisen::~CFxKaisen()
{
}

// nate 2004 - 8 ImageManager
void CFxKaisen::LoadRes()
{
	PFILE *fpLolo = NULL;
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/bat.LOL");
	}



}

void CFxKaisen::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

void CFxKaisen::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 8)
		{

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_HORNBOOM;
			ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y + 12.0f, effect_sort.vPos.z );
			ef_sort.Index = HIT_KAISEN;

			if (effect_sort.pMobTo)
				effect_sort.pMobTo->Attacked();
			else if (effect_sort.pNkChaTo)
				effect_sort.pNkChaTo->Attacked();

			if( g_pRoh )
				g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);
}


CFxPhenix::CFxPhenix()
{
	m_pLolo = NULL;
	ring = new CCylinderPlane;
	if( ring )
		ring->Create( 5.0f, 2.0f, 12.0f, 8.0f, 0.0f, TRUE);
}

CFxPhenix::~CFxPhenix()
{
	SAFE_DELETE(ring);
}

void CFxPhenix::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/firebird.LOL");
	}



}

void CFxPhenix::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

BOOL CFxPhenix::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	float maxScale, m_fScale, m_fFade;
	maxScale = 3.0f;
	m_fScale = (maxScale * effect_sort.nCurFrame) / FX_PHENIX_FRAMECOUNT;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vPos;

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PHENIX_RING ] ) );

	if( ring )
	{
		ring->SetColor(0.6f, 0.15f, 0.15f, 0.15f);
		ring->SetRotationY( g_PI );
		ring->RotateY( m_fScale * 50.0f );
		ring->Scale( 3.5f, 10.0f, 3.5f );
		ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		ring->Render();
	}

	if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 8)
	{
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return bFinished;
}

CFxGryphon::CFxGryphon()
{
	m_pLolo = NULL;
	ring = NULL;
	ring = new CCylinderPlane;
	if( ring )
		ring->Create( 2.5f, 6.0f, 0.0f, 20, FALSE );

	ground_corona = NULL;
	ground_corona = new CRectPlane;
	if( ground_corona )
		ground_corona->Create( 8.0f, 8.0f, FALSE );

	eng_sphere = NULL;
	eng_sphere = new CSphere;
	if( eng_sphere )
		eng_sphere->Create();
}

CFxGryphon::~CFxGryphon()
{
	SAFE_DELETE(ground_corona);
	SAFE_DELETE(eng_sphere);
	SAFE_DELETE(ring);
}

void CFxGryphon::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/ring_gr_fx.LOL");
	}



}

void CFxGryphon::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

BOOL CFxGryphon::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 8)
	{

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_HORNBOOM;
		ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y + 12.0f, effect_sort.vPos.z );
		ef_sort.Index = HIT_GRYPHON;

		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

	}
	else if( effect_sort.nCurFrame > 8 )
	{

		float maxScale, m_fScale, m_fFade;
		maxScale = 3.0f;
		m_fScale = (maxScale * effect_sort.nCurFrame) / FX_DRAGON_FRAMECOUNT;
		m_fFade = 1.0f - (m_fScale * 0.3f);

		D3DVECTOR vDrawPos;
		vDrawPos = effect_sort.vPos;

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

		float red, green, blue;
		red = green = blue = 0.0f;

		red = 0.7f - (0.7f * m_fScale * 0.15f);
		green = 0.7f - (0.7f * m_fScale * 0.15f);
		blue = 0.15f - (0.15f * m_fScale * 0.15f);


		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_GRIPHON_RING ] ) );
		if( ring )
		{
			ring->SetColor(red, green, blue, m_fFade);
			ring->SetRotationY( g_PI * 0.5f );
			ring->RotateY( m_fScale * 5.0f );
			ring->Scale( m_fScale + 1.0f, 1.0f, m_fScale + 1.0f );
			ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			ring->Render();

			ring->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
			ring->SetRotationY( g_PI * 0.5f );
			ring->Scale( m_fScale*1.5f + 1.0f, 1.0f, m_fScale*1.5f + 1.0f );
			ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			ring->Render();
		}

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_GRIPHON_SHIELD ] ) );
		if( ground_corona )
		{
			ground_corona->SetColor(red*0.3f, green*0.3f, blue*0.3f, m_fFade*0.3f);
			ground_corona->SetRotationX( g_PI * 0.5f );
			ground_corona->Scale( m_fScale, m_fScale, m_fScale );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale*3.5f, vDrawPos.z);
			ground_corona->Render();

			ground_corona->Scale( 2.0f, 2.0f, 2.0f );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale*2.5f, vDrawPos.z);
			ground_corona->Render();
		}

		red = 0.95f - (0.95f * m_fScale * 0.15f);
		green = 0.95f - (0.95f * m_fScale * 0.15f);
		blue = 0.7f - (0.7f * m_fScale * 0.15f);

		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_GRIPHON_RING ] ) );
		if( eng_sphere )
		{
			eng_sphere->SetColor(red, green, blue, m_fFade);
			eng_sphere->SetScaling(m_fScale*7.0f, m_fScale*7.0f, m_fScale*7.0f);
			eng_sphere->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			eng_sphere->Render();
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return bFinished;
}

//////////////////////////////////////////////////////////////////////
// ����ũ�� �� ����.
//////////////////////////////////////////////////////////////////////
CFxSphinx::CFxSphinx()
{
	m_pLolo = NULL;

	ring = NULL;
	ring = new CCylinderPlane;
	if( ring )
		ring->Create( 2.5f, 6.0f, 0.0f, 20, FALSE );

	ground_corona = NULL;
	ground_corona = new CRectPlane;
	if( ground_corona )
		ground_corona->Create( 8.0f, 8.0f, FALSE );

	eng_sphere = NULL;
	eng_sphere = new CSphere;
	if( eng_sphere )
		eng_sphere->Create();
}

CFxSphinx::~CFxSphinx()
{
	SAFE_DELETE(eng_sphere);
	SAFE_DELETE(ground_corona);
	SAFE_DELETE(ring);
}

void CFxSphinx::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/parao_fx.LOL");
	}



}

void CFxSphinx::DeleteRes()
{
	SAFE_DELETE(m_pLolo);
}

BOOL CFxSphinx::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if (m_pLolo)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );

		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
		m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 8)
	{
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_HORNBOOM;
		ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y + 12.0f, effect_sort.vPos.z );
		ef_sort.Index = HIT_GRYPHON;

		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		if( pCMyApp->m_pFxSet )
			//Add the effect to the players screen?
			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

	}
	else if( effect_sort.nCurFrame > 8 )
	{

		float maxScale, m_fScale, m_fFade;
		maxScale = 3.0f;
		m_fScale = (maxScale * effect_sort.nCurFrame) / FX_DRAGON_FRAMECOUNT;
		m_fFade = 1.0f - (m_fScale * 0.3f);

		D3DVECTOR vDrawPos;
		vDrawPos = effect_sort.vPos;

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

		float red, green, blue;
		red = green = blue = 0.0f;

		red = 0.5f - (0.5f * m_fScale * 0.15f);
		green = 0.5f - (0.5f * m_fScale * 0.15f);
		blue = 1.0f - (1.0f * m_fScale * 0.15f);


		//The ring (duh)
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPHINX_SHIELD ] ) );
		if( ring )
		{
			ring->SetColor(red, green, blue, m_fFade);
			ring->SetRotationY( g_PI * 0.5f );
			ring->RotateY( m_fScale * 5.0f );
			ring->Scale( m_fScale + 1.0f, 1.0f, m_fScale + 1.0f );
			ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			ring->Render();

			ring->SetColor(red * 0.5f, green * 0.5f, blue * 0.5f, m_fFade * 0.5f);
			ring->SetRotationY( g_PI * 0.5f );
			ring->Scale( m_fScale*1.5f + 1.0f, 1.0f, m_fScale*1.5f + 1.0f );
			ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 5.5f, vDrawPos.z);
			ring->Render();
		}

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();
		//Give it a texture on the floor
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPHINX_SHIELD ] ) );
		if( ground_corona )
		{
			ground_corona->SetColor(red*0.3f, green*0.3f, blue*0.3f, m_fFade*0.3f);
			ground_corona->SetRotationX( g_PI * 0.5f );
			ground_corona->Scale( m_fScale, m_fScale, m_fScale );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale*3.5f, vDrawPos.z);
			ground_corona->Render();

			ground_corona->Scale( 2.0f, 2.0f, 2.0f );
			ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale*2.5f, vDrawPos.z);
			ground_corona->Render();
		}

		red = 0.55f - (0.55f * m_fScale * 0.15f);
		green = 0.55f - (0.55f * m_fScale * 0.15f);
		blue = 0.2f - (0.2f * m_fScale * 0.15f);

		// Possibly the sphinx image it's self
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPHINX_SHIELD ] ) );
		if( eng_sphere )
		{
			eng_sphere->SetColor(red, green, blue, m_fFade);
			eng_sphere->SetScaling(m_fScale*12.0f, m_fScale*7.5f, m_fScale*12.0f);
			eng_sphere->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			eng_sphere->Render();
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return bFinished;
}


//////////////////////////////////////////////////////////////////////
// �巹�� �� ����.
//////////////////////////////////////////////////////////////////////
CFxDragon::CFxDragon()
{
	m_pLolo = NULL;
	m_pLolo2 = NULL;
	ring = NULL;
	ring = new CCylinderPlane;
	if( ring )
		ring->Create( 8.0f, 8.0f, 15.0f, 20.0f, 0.0f, TRUE, 2.0f);

	ground_corona = NULL;
	ground_corona = new CRectPlane;
	if( ground_corona )
		ground_corona->Create( 10.0f, 10.0f, FALSE );
}

CFxDragon::~CFxDragon()
{
	SAFE_DELETE(ground_corona);
	SAFE_DELETE(ring);
}

// nate 2004 - 8 ImageManager
void CFxDragon::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/ring_dr_fx.LOL");
	}

	if (!m_pLolo2)
	{
		m_pLolo2 = new CLolos;
		m_pLolo2->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo2->Read("data/effect/lolos/ring_dr2_fx.LOL");
	}

}

void CFxDragon::DeleteRes()
{
	SAFE_DELETE(m_pLolo2);
	SAFE_DELETE(m_pLolo);
}

BOOL CFxDragon::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView;
	DWORD dwLighting, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );


	if (m_pLolo && m_pLolo2)
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

		if( effect_sort.num == 1 ) // �� �巡�︵�̸�.. ///����������
			m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
		else
			m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	float maxScale, m_fScale, m_fFade;
	maxScale = 3.0f;
	m_fScale = (maxScale * effect_sort.nCurFrame) / FX_DRAGON_FRAMECOUNT;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	D3DVECTOR vDrawPos;
	vDrawPos = effect_sort.vPos;

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DRAGON_RING ] ) );

	float red, green, blue;
	red = green = blue = 0.0f;

	red = 0.15f - (0.15f * m_fScale * 0.15f);
	green = 0.15f - (0.15f * m_fScale * 0.15f);
	blue = 0.4f - (0.4f * m_fScale * 0.15f);


	if( ring )
	{
		ring->SetColor(red, green, blue, m_fFade);
		ring->SetRotationY( g_PI );
		ring->RotateY( m_fScale * 20.0f );
		ring->Scale( 3.5f, 10.0f, 3.5f );
		ring->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		ring->Render();
	}

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	red = 0.25f - (0.25f * m_fScale * 0.15f);
	green = 0.25f - (0.25f * m_fScale * 0.15f);
	blue = 0.75f - (0.75f * m_fScale * 0.15f);

	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DRAGON_SHIELD ] ) );
	if( ground_corona )
	{
		ground_corona->SetColor(red*0.5f, green*0.5f, blue*0.5f, m_fFade*0.5f);
		ground_corona->SetRotationX( g_PI * 0.5f );
		ground_corona->Scale( m_fScale*1.5f, m_fScale*1.5f, m_fScale*1.5f );
		ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale*3.0f, vDrawPos.z);
		ground_corona->Render();
	}

	// nate 2004 - 7 - image
	if( ground_corona )
	{
		ground_corona->Scale( 1.5f, 1.5f, 1.5f );
		ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + m_fScale*2.5f, vDrawPos.z);
		ground_corona->Render();
	}

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DRAGON_CORONA ] ) );
	if( ground_corona )
	{
		ground_corona->SetColor(red, green, blue, m_fFade);
		ground_corona->ReCreate(m_fScale * 0.25f);
		ground_corona->SetBillBoard(vViewVec, FALSE, 0.0f);
		ground_corona->Scale(m_fScale, m_fScale, 0.0f);
		ground_corona->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 12.0f, vDrawPos.z);
		ground_corona->Render();
	}

	if (effect_sort.nCurFrame >= 6 && effect_sort.nCurFrame < 8)
	{

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_HORNBOOM;
		ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y + 12.0f, effect_sort.vPos.z );

		if( effect_sort.num == 1 ) // �� �巡�︵�̸�.. ///����������
			ef_sort.Index = HIT_KAISEN; // ������
		else
			ef_sort.Index = HIT_DRAGON;


		if (effect_sort.pMobTo)
		{
			effect_sort.pMobTo->Attacked();
		}
		else if (effect_sort.pNkChaTo)
		{
			effect_sort.pNkChaTo->Attacked();
		}

		if( pCMyApp->m_pFxSet )
			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return bFinished;
}


CFxSummonRing::CFxSummonRing()
{
}

CFxSummonRing::~CFxSummonRing()
{
}

BOOL CFxSummonRing::Render(EffectSort &effect_sort)
{
	BOOL bFinished = FALSE; // ����Ʈ�� ������� ����.
	D3DMATRIX matTrans, matRotate;
	DWORD dwAlpha,dwLighting, dwZEnable, dwZWrite;

	CBiped *pBiped; // ���� �����Ϳ�.
	CCapsyong *pCapSyong[CAPSYONG_NUM]; // ���� �����Ϳ�.
	CCapsyong *pWearing[WEARING_NUM]; // ���� �����Ϳ�.

	int body_offset = 0, hair_offset = 0, nRace = 0, nSex = 0, nHair = 0, BpdRace = 0;
	int TempProtoNum = 0;
	BOOL TempOneOne = FALSE; // ���ĺ�����.

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZENABLE, &dwZEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlpha);

	switch( effect_sort.num ) // ����Ʈ ����
	{
	case EFFECT_SKILL_RING_BULKAN: // ��ĭ��. ���ڴ� �ӽ�.

		{
			nRace = 0;
			nSex = 0;
			nHair = 0;
			BpdRace = nRace*2+nSex;

			memset(pCapSyong, 0, sizeof(pCapSyong));

			CSYMAP_it it = pCMyApp->m_race_csy_map.begin();
			for (int i=0; i < NKCHA_HAIR; i++)
			{
				it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(i&0xFF)<<8);
				if( it != pCMyApp->m_race_csy_map.end() )
					pCapSyong[i] = (*it).second;
				else
				{
					_asm nop;
				}
			}

			it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(5&0xFF)<<8|(nHair&0xFF));
			if( it != pCMyApp->m_race_csy_map.end() )
				pCapSyong[NKCHA_HAIR] = (*it).second;
			else
			{
				_asm nop;
			}


			// ����
			TempProtoNum = g_ItemList.FindItem(472);
			pWearing[WEARING_WEAPON] = CsyManager::GetInstance()->GetCsy(g_ItemList.m_ItemArray[TempProtoNum].m_CsyFile[BpdRace]);

			// �ִϸ��̼�.
			pBiped = g_ChaBpdList[ g_ChaBpdIndex[BpdRace][60] ].m_pBiped;




			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);

			D3DUtil_SetIdentityMatrix( matTrans );
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );


			for( int i = 0 ; i < CAPSYONG_NUM ; ++i )
			{
				if( pCapSyong[i] == NULL )
					continue;

				pCapSyong[i]->Animate( pBiped, effect_sort.nCurFrame );

				TempOneOne = pCapSyong[i]->m_Csy->m_CsyEffect; // ���� ��� ���� �÷��� �� ���.
				pCapSyong[i]->m_Csy->m_CsyEffect = CSY_EFF_ONEONE; // OneOne ���� ���.
				pCapSyong[i]->Render( GET_D3DDEVICE(), -1, FALSE, TRUE/*bLow*/ ); // ���� ������ ���� ������ ���� ������ ���� ���.
				pCapSyong[i]->m_Csy->m_CsyEffect = TempOneOne; // ���� �����ش�.
			}

			pWearing[WEARING_WEAPON]->Animate( pBiped, effect_sort.nCurFrame );

			TempOneOne = pWearing[WEARING_WEAPON]->m_Csy->m_CsyEffect; // ���� ��� ���� �÷��� �� ���.
			pWearing[WEARING_WEAPON]->m_Csy->m_CsyEffect = CSY_EFF_ONEONE; // OneOne ���� ���.
			pWearing[WEARING_WEAPON]->Render( GET_D3DDEVICE(), -1, FALSE, TRUE/*bLow*/ ); // ���� ������ ���� ������ ���� ������ ���� ���.
			pWearing[WEARING_WEAPON]->m_Csy->m_CsyEffect = TempOneOne; // ���� �����ش�.

			if (effect_sort.nCurFrame >= 17 && effect_sort.nCurFrame < 20)
			{
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));

				ef_sort.nType = FX_HORNBOOM;
				ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x+20.0f, effect_sort.vPos.y + 12.0f, effect_sort.vPos.z );
				ef_sort.Index = HIT_KAISEN;

				g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}

			if(  effect_sort.nCurFrame >= pBiped->m_AnimCount-4  ) // ������ ��������... ������� ����(��ĭ���� �ణ ���� ������..) (�������� ������ ������ Ʋ���Ƿ� ���⼭ ���ش�.)
				bFinished = TRUE;
		}


		break;

	case EFFECT_SKILL_BASHREFLECTION: // ���̺긮�� �޺� ��ų ����转+���÷����� �н� ����Ʈ ///�����޺�

		if( effect_sort.pNkChaFrom )
		{
			// ĳ���� Ŭ������ ĸ���� protected ����� ����,����,�������� ���� �⺻ ��ü�� ���´�.
			nRace = 4;
			nSex = effect_sort.pNkChaFrom->m_Sex;
			nHair = effect_sort.pNkChaFrom->m_Hair;
			BpdRace = nRace*2+nSex;

			memset(pCapSyong, 0, sizeof(pCapSyong));

			CSYMAP_it it = pCMyApp->m_race_csy_map.begin();
			for (int i=0; i < NKCHA_HAIR; i++)
			{
				it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(i&0xFF)<<8);
				if( it != pCMyApp->m_race_csy_map.end() )
					pCapSyong[i] = (*it).second;
				else
				{
					_asm nop;
				}
			}

			it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(5&0xFF)<<8|(nHair&0xFF));
			if( it != pCMyApp->m_race_csy_map.end() )
				pCapSyong[NKCHA_HAIR] = (*it).second;
			else
			{
				_asm nop;
			}

			for( int i=0 ; i < WEARING_SHOW_NUM ; ++i ) // ���� ������ ĸ�� ����.
			{
				if( effect_sort.pNkChaFrom->m_Wearing[i] ) // �ش� ��ġ�� ��� �����ϰ��ִٸ�..
					pWearing[i] = GET_CSY( effect_sort.pNkChaFrom->m_Wearing[i], BpdRace );
				else // ������ϰ��ִٸ�.
					pWearing[i] = NULL;
			}

			if( effect_sort.pNkChaFrom->m_Wearing[WEARING_WEAPON] ) // �ش� ��ġ�� ��� �����ϰ��ִٸ�..
			{
				pWearing[WEARING_WEAPON] = GET_CSY( effect_sort.pNkChaFrom->m_Wearing[WEARING_WEAPON], BpdRace );
			}


			if( effect_sort.Index == 1 ) // �Ѽ� �ֵθ��� phase
			{
				if( effect_sort.pNkChaFrom->m_Sex == 0 ) // ���ڸ�..
					pBiped = g_ChaBpdList[ g_ChaBpdIndex[BpdRace][BPDIDX_ATT_1HAND+2] ].m_pBiped;
				else // ���ڸ�
					pBiped = g_ChaBpdList[ g_ChaBpdIndex[BpdRace][BPDIDX_ATT_1HAND] ].m_pBiped;

				// Ÿ�� ����Ʈ �߰�
				if (effect_sort.nCurFrame > 12 && effect_sort.subNum == 0 )
				{
					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));

					ef_sort.nType = FX_HORNBOOM;
					ef_sort.vPos = D3DVECTOR( effect_sort.vTargetPos.x, effect_sort.vTargetPos.y + 12.0f, effect_sort.vTargetPos.z );
					ef_sort.Index = HIT_GRYPHON;
					pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );

					effect_sort.subNum = 1; // �ѹ��� ������..
				}

			}
			else if( effect_sort.Index == 2 ) // �μ� �ֵθ��� phase
			{
				if( effect_sort.pNkChaFrom->m_Sex == 0 ) // ���ڸ�..
					pBiped = g_ChaBpdList[ g_ChaBpdIndex[BpdRace][BPDIDX_ATT_2AXE+1] ].m_pBiped;
				else // ���ڸ�
					pBiped = g_ChaBpdList[ g_ChaBpdIndex[BpdRace][BPDIDX_ATT_2AXE] ].m_pBiped;

				if (effect_sort.nCurFrame > 12 && effect_sort.subNum == 0 )
				{
					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));

					ef_sort.nType = FX_HORNBOOM;
					ef_sort.vPos = D3DVECTOR( effect_sort.vTargetPos.x, effect_sort.vTargetPos.y + 12.0f, effect_sort.vTargetPos.z );
					ef_sort.Index = HIT_GRYPHON;
					pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );

					effect_sort.subNum = 1; // �ѹ��� ������..
				}

			}
			else if( effect_sort.Index == 3 ) // ������� phase
			{
				if( effect_sort.pNkChaFrom->m_Sex == 0 ) // ���ڸ�..
					pBiped = g_ChaBpdList[ g_ChaBpdIndex[BpdRace][BPDIDX_SKILL_BASH] ].m_pBiped;
				else // ���ڸ�
					pBiped = g_ChaBpdList[ g_ChaBpdIndex[BpdRace][BPDIDX_ATT_2AXE+2] ].m_pBiped;

				if (effect_sort.nCurFrame > 12 && effect_sort.subNum == 0 )
				{
					if( effect_sort.pNkChaTo )
						pCMyApp->m_pUIMgr->InsertMagic(effect_sort.pNkChaFrom, NULL, effect_sort.pNkChaTo, NULL, EFFECT_SKILL_LIGHTNING);
					else if( effect_sort.pMobTo )
						pCMyApp->m_pUIMgr->InsertMagic(effect_sort.pNkChaFrom, NULL, NULL, effect_sort.pMobTo, EFFECT_SKILL_LIGHTNING);

					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));

					ef_sort.nType = FX_HORNBOOM;
					ef_sort.vPos = D3DVECTOR( effect_sort.vTargetPos.x, effect_sort.vTargetPos.y + 12.0f, effect_sort.vTargetPos.z );
					ef_sort.Index = HIT_GRYPHON;
					pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );

					effect_sort.subNum = 1; // �ѹ��� ������..
				}
			}
			else
				return TRUE; // Ȥ�� �� ��츦 ���ؼ�...

			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

			D3DUtil_SetIdentityMatrix( matTrans );
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.z );
			D3DMath_MatrixMultiply( matTrans, matRotate, matTrans );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );


			// �⺻ ������ �׸���...
			for (int i = 0; i < CAPSYONG_NUM; i++)
			{
				if( pCapSyong[i] )
				{
					// �⺻ �� ���� �ɼ� üũ.
					if ((i == NKCHA_HAIR && (effect_sort.pNkChaFrom->m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(effect_sort.pNkChaFrom->m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR)) ||
							(i == NKCHA_FACE && (effect_sort.pNkChaFrom->m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(effect_sort.pNkChaFrom->m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE)) ||
							(i == NKCHA_BREAST && (effect_sort.pNkChaFrom->m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(effect_sort.pNkChaFrom->m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY)) ||
							(i == NKCHA_ARMS && (effect_sort.pNkChaFrom->m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(effect_sort.pNkChaFrom->m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS)) ||
							(i == NKCHA_LEGS && (effect_sort.pNkChaFrom->m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(effect_sort.pNkChaFrom->m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS)) ||
							(i == NKCHA_FEET && (effect_sort.pNkChaFrom->m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(effect_sort.pNkChaFrom->m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET)) ||
							(i == NKCHA_FEET && ((effect_sort.pNkChaFrom->m_Wearing[WEARING_SHOES]  && GET_RENDER_FLAG(effect_sort.pNkChaFrom->m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET)
												 || (effect_sort.pNkChaFrom->m_Wearing[WEARING_PANTS] && (effect_sort.pNkChaFrom->m_Wearing[WEARING_PANTS]->m_Vnum == 384 || effect_sort.pNkChaFrom->m_Wearing[WEARING_PANTS]->m_Vnum == 303)))) )
					{
						// �׸��� ���ƶ�!!
					}
					else
					{
						pCapSyong[i]->Animate( pBiped, effect_sort.nCurFrame );

						TempOneOne = pCapSyong[i]->m_Csy->m_CsyEffect; // ���� ��� ���� �÷��� �� ���.
						pCapSyong[i]->m_Csy->m_CsyEffect = CSY_EFF_ONEONE; // OneOne ���� ���.
						pCapSyong[i]->Render( GET_D3DDEVICE(), -1, FALSE, TRUE );
						pCapSyong[i]->m_Csy->m_CsyEffect = TempOneOne; // ���� �����ش�.
					}
				}
			}

			for( int i = 0 ; i < WEARING_SHOW_NUM ; ++i )
			{
				if( pWearing[i] )
				{
					pWearing[i]->Animate( pBiped, effect_sort.nCurFrame );
					TempOneOne = pWearing[i]->m_Csy->m_CsyEffect; // ���� ��� ���� �÷��� �� ���.
					pWearing[i]->m_Csy->m_CsyEffect = CSY_EFF_ONEONE; // OneOne ���� ���.
					pWearing[i]->Render( GET_D3DDEVICE(), -1, FALSE, TRUE );
					pWearing[i]->m_Csy->m_CsyEffect = TempOneOne; // ���� �����ش�.
				}
			}

			if( pWearing[WEARING_WEAPON] )
			{
				pWearing[WEARING_WEAPON]->Animate( pBiped, effect_sort.nCurFrame );
				TempOneOne = pWearing[WEARING_WEAPON]->m_Csy->m_CsyEffect; // ���� ��� ���� �÷��� �� ���.
				pWearing[WEARING_WEAPON]->m_Csy->m_CsyEffect = CSY_EFF_ONEONE; // OneOne ���� ���.
				pWearing[WEARING_WEAPON]->Render( GET_D3DDEVICE(), -1, FALSE, TRUE );
				pWearing[WEARING_WEAPON]->m_Csy->m_CsyEffect = TempOneOne; // ���� �����ش�.
			}

			int EndFrame = 0;
			int NextStartFrame = 0;

			if( effect_sort.Index == 1 ) // �Ѽ� �ֵθ� phase
			{
				if( effect_sort.pNkChaFrom->m_Sex == 0 ) // ���ڸ�..
				{
					EndFrame = pBiped->m_AnimCount-12;
					NextStartFrame = 5;
				}
				else // ���ڸ�...
				{
					EndFrame = pBiped->m_AnimCount-10;
					NextStartFrame = 5;
				}


				if( effect_sort.nCurFrame >= EndFrame ) // ������ ��������...
				{
					effect_sort.nCurFrame = NextStartFrame;
					effect_sort.Index++; // ���� phase��....
					effect_sort.subNum = 0; // �ʱ�ȭ.

					if (g_pDSound)
					{
						// ������ �Ѹ� ���ǵǸ� �ȴ�.
						float x, y, z;
						x = (effect_sort.vTargetPos.x - effect_sort.pNkChaFrom->m_wx) / g_fDSDistance;
						y = (effect_sort.vTargetPos.y - effect_sort.pNkChaFrom->m_wy) / g_fDSDistance;
						z = (effect_sort.vTargetPos.z - effect_sort.pNkChaFrom->m_wz) / g_fDSDistance;

						g_pDSound->Play(g_ChaBpdList[g_ChaBpdIndex[BpdRace][BPDIDX_ATT_2AXE]].m_SoundNum, x, y, z );
					}
				}
			}
			else if( effect_sort.Index == 2 ) // �μ� �ֵθ��� phase
			{
				if( effect_sort.pNkChaFrom->m_Sex == 0 ) // ���ڸ�..
				{
					EndFrame = pBiped->m_AnimCount-5;
					NextStartFrame = 10;
				}
				else // ���ڸ�...
				{
					EndFrame = pBiped->m_AnimCount-5;
					NextStartFrame = 5;
				}

				if( effect_sort.nCurFrame >= EndFrame ) // ������ ��������...
				{
					effect_sort.nCurFrame = NextStartFrame;

					effect_sort.Index++; // ���� phase��....
					effect_sort.subNum = 0; // �ʱ�ȭ.

					if (g_pDSound)
					{
						// ������ �Ѹ� ���ǵǸ� �ȴ�.
						float x, y, z;
						x = (effect_sort.vTargetPos.x - effect_sort.pNkChaFrom->m_wx) / g_fDSDistance;
						y = (effect_sort.vTargetPos.y - effect_sort.pNkChaFrom->m_wy) / g_fDSDistance;
						z = (effect_sort.vTargetPos.z - effect_sort.pNkChaFrom->m_wz) / g_fDSDistance;

						g_pDSound->Play(g_ChaBpdList[g_ChaBpdIndex[BpdRace][BPDIDX_ATT_2AXE]].m_SoundNum, x, y, z );
					}
				}
			}
			else if( effect_sort.Index == 3 ) // ������� phase
			{
				if( effect_sort.pNkChaFrom->m_Sex == 0 ) // ���ڸ�..
					EndFrame = pBiped->m_AnimCount-5;
				else // ���ڸ�...
					EndFrame = pBiped->m_AnimCount-5;

				if( effect_sort.nCurFrame >= EndFrame ) // ������ ��������...
				{
					// ����.
					effect_sort.nCurFrame = 0;
					effect_sort.Index = 0;
					effect_sort.pNkChaFrom->m_nComboFrame = 0; // �̰͵� �ʱ�ȭ ������.
					bFinished = TRUE;
				}
			}
		}

		break;
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlpha);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZENABLE, dwZEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite);

	return bFinished;
}


