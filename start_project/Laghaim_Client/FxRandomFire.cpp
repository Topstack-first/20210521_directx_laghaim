#include "stdafx.h"
// FxRandomFire.cpp: implementation of the CFxRandomFire class.
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
#include "skill.h"
#include "DecoObj.h"
#include "FxRandomFire.h"




// nate 2004 - 7 글로벌 ShadowRect사용
extern float g_fDSDistance;

extern int g_EffectSound[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxRandomFire::CFxRandomFire()
{
	m_fScale = 0.0f;
	m_pLolo = NULL;
	m_pLolo2 = NULL;
	m_pLolo3 = NULL;
	m_pLolo4 = NULL;
	m_nTotalFrame = FX_RANDOMFIRE_FRAMECOUNT;

	m_rect_magic = NULL;
	m_rect_magic = new CRectPlane;
	if( m_rect_magic )
		m_rect_magic->Create( 30.0f, 30.0f, FALSE );
}

CFxRandomFire::~CFxRandomFire()
{
	SAFE_DELETE(m_pLolo);
	SAFE_DELETE(m_pLolo2);
	SAFE_DELETE(m_pLolo3);
	SAFE_DELETE(m_pLolo4);
	SAFE_DELETE(m_rect_magic);
}

void CFxRandomFire::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/h_random_fire2.LOL");
	}

	if (!m_pLolo2)
	{
		m_pLolo2 = new CLolos;
		m_pLolo2->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo2->Read("data/effect/lolos/h_random_fire3.LOL");
	}

	if (!m_pLolo3)
	{
		m_pLolo3 = new CLolos;
		m_pLolo3->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo3->Read("data/effect/lolos/h_random_fire4.LOL");
	}

	if (!m_pLolo4)
	{
		m_pLolo4 = new CLolos;
		m_pLolo4->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo4->Read("data/effect/lolos/h_random_fire1.LOL");
	}


}

void CFxRandomFire::DeleteRes()
{
}

BOOL CFxRandomFire::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	//-- 사용 캐릭이 사라질 경우
	if(!effect_sort.pNkChaFrom)
		return TRUE;


	BOOL bFinished = FALSE;

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

	if( effect_sort.pNkChaFrom == g_pRoh )
	{
		int nSkillLevel = effect_sort.num;
		if( m_pLolo && nSkillLevel <= 10 )
		{
			m_pLolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
			if ( nFrame >= 88 )
				return TRUE;
		}
		else if ( m_pLolo2 && nSkillLevel <= 20 )
		{
			m_pLolo2->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
			if ( nFrame >= 109 )
				return TRUE;
		}
		else if ( m_pLolo3 && nSkillLevel <= 35 )
		{
			m_pLolo3->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
		}
	}
	else
	{
		if( nFrame >= 21 )
			return TRUE;

		if( m_pLolo4 )
			m_pLolo4->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}

	return bFinished;
}



