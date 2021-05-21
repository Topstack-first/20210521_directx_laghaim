#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "sfx.h"
#include "FxChain.h"
#include "main.h"
#include "UIMgr.h"
#include "D3DMath.h"
#include "D3DUtil.h"
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
#include "ItemProto.h"


#define FX_CHAIN_NOISE		20
#define FX_CHAIN_WIDTH		(4.0f)
#define FX_CHAIN_TEXTR_COUNT	4

CFxChain::CFxChain()
	: m_fScale(0)
	, m_fFade(0)
{
	for (int line = 0; line < FX_CHAIN_LINE; line++)
	{
		for (int div = 0; div < FX_CHAIN_DIV; div++)
		{
			int add = (line*(FX_CHAIN_DIV*2+2)) + div*2;
			m_indexRect[(line*FX_CHAIN_DIV+div)*6+0] = 0+add;
			m_indexRect[(line*FX_CHAIN_DIV+div)*6+1] = 2+add;
			m_indexRect[(line*FX_CHAIN_DIV+div)*6+2] = 1+add;
			m_indexRect[(line*FX_CHAIN_DIV+div)*6+3] = 1+add;
			m_indexRect[(line*FX_CHAIN_DIV+div)*6+4] = 2+add;
			m_indexRect[(line*FX_CHAIN_DIV+div)*6+5] = 3+add;
		}
	}

	light = new CRectPlane;
	if( light )
		light->Create( 5.0f, 5.0f, FALSE );
}

CFxChain::~CFxChain()
{
	SAFE_DELETE(light);
	DeleteRes();
}

void CFxChain::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxChain::DeleteRes()
{}

void CFxChain::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !light )
		return;
	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return;

	// nate 2005-05-24 : 홀리스테프 이펙트 랜더링
	int nFrame = effect_sort.nCurFrame;
	if(  effect_sort.num == 2	// 홀리 스테프
			&&nFrame < 13 )			// 13프레임부터 랜더링
		return;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	D3DMATRIX matWorld;
	// 방향벡터를 구한다.
	D3DVECTOR vPosFrom, vPosTo;

	// 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+10.0f, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	if (effect_sort.pMobTo)
	{
		switch (effect_sort.pMobTo->m_Height)
		{
		case MOB_HEIGHT_HIGH_NUM:
			vPosTo.y += MOB_HEIGHT_HIGH_VAL;
			break;
		case MOB_HEIGHT_LOW_NUM:
			vPosTo.y += MOB_HEIGHT_LOW_VAL;
			break;
		default: // MOB_HEIGHT_MIDDLE_NUM:
			vPosTo.y += MOB_HEIGHT_MIDDLE_VAL;
			break;
		}
	}

	D3DVECTOR vDir;
	get_normal_vector(vDir, vPosFrom, vPosTo);

	vPosFrom += (vDir * 3.0f);
	vPosFrom.y += 15.0f;

	MakeChain(vPosFrom, vPosTo);

	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	//-- 체인 라이트닝과 죽음의 물결 완드 구분
	if( g_pCapsyongTexture )
	{
		if(effect_sort.num == 1)
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CHAIN ] ) ;
		// nate 2005-05-20 : 홀리부대
		else if( effect_sort.num == 2 )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HOLLEY ] );
		else
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WAVE_OF_DEATH ] ) ;
	}

	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX,
										   m_lverRect, FX_CHAIN_LINE*(FX_CHAIN_DIV*2+2),
										   m_indexRect, FX_CHAIN_LINE*FX_CHAIN_DIV*6, 0 );
	float maxScale = 3.0f;
	m_fScale = (maxScale * effect_sort.nCurFrame) / FX_WAND_CHAIN_FRAMECOUNT;
	m_fFade = 1.0f - (m_fScale * 0.3f);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CHAIN_LIGHT ] ) );

	light->SetColor(0.75f, 0.75f, 1.0f, m_fFade);
	light->SetBillBoard( vViewVec, FALSE, 0.0f );
	light->Scale( RandomNum(1.0f, 3.0f), RandomNum(1.0f, 3.0f), 1.0f );
	light->SetTranslationXYZ(vPosTo.x, vPosTo.y, vPosTo.z);
	light->Render();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxChain::MakeChain(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo)
{
	if( !GET_D3DDEVICE() )
		return;

	int textr_off = rand() % FX_CHAIN_TEXTR_COUNT;
	float u1 = 1.0f / FX_CHAIN_TEXTR_COUNT * textr_off;
	float u2 = 1.0f / FX_CHAIN_TEXTR_COUNT * (textr_off + 1);

	D3DMATRIX matView, matInv;
	D3DVECTOR vFromView, vToView, vRotView, vRotViewN, vNoise;

	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &matView );
	D3DMath_MatrixInvert(matInv, matView);
	D3DMath_VectorMatrixMultiply(vFromView, vPosFrom, matView);
	D3DMath_VectorMatrixMultiply(vToView, vPosTo, matView);

	// line에 직각인 벡터를 구한다.
	D3DVECTOR lineInv;
	lineInv = vToView;
	lineInv -= vFromView;

	FLOAT x_temp = lineInv.x;
	lineInv.x = -lineInv.y;
	lineInv.y = x_temp;
	lineInv.z = 0;
	lineInv = Normalize(lineInv);
	lineInv *= FX_CHAIN_WIDTH;

	vFromView += lineInv;

	D3DMath_VectorMatrixMultiply(vFromView, vFromView, matInv);
	vRotView = vFromView - vPosFrom;
	vRotViewN = Normalize(vRotView);

	float dist = DistPlToPlAbs(vPosFrom.x, vPosFrom.z, vPosTo.x, vPosTo.z);
	int noise_base = FX_CHAIN_NOISE * dist / 150.0f;
	if (noise_base < 2)
		noise_base = 2;
	D3DVECTOR vAdd = (vPosTo - vPosFrom) / FX_CHAIN_DIV;
	for (int line = 0; line < FX_CHAIN_LINE; line++)
	{
		m_lverRect[line*(FX_CHAIN_DIV*2+2)+0] =
			D3DLVERTEX( D3DVECTOR(vPosFrom.x+vRotView.x, vPosFrom.y+vRotView.y, vPosFrom.z+vRotView.z), 0xffffffff, 0, u1, 0.0f );
		m_lverRect[line*(FX_CHAIN_DIV*2+2)+1] =
			D3DLVERTEX( D3DVECTOR(vPosFrom.x-vRotView.x, vPosFrom.y-vRotView.y, vPosFrom.z-vRotView.z), 0xffffffff, 0, u2, 0.0f );
		for (int div = 1; div < FX_CHAIN_DIV; div++)
		{
			int noise = rand() % noise_base - (noise_base / 2);
			vNoise = vRotViewN * noise;
			vNoise.x += RandomNum(-5.0f, 5.0f);
			vNoise.y += RandomNum(-5.0f, 5.0f);
			vNoise.z += RandomNum(-5.0f, 5.0f);
			D3DVECTOR vPosMid = vPosFrom + (vAdd * div);
			m_lverRect[line*(FX_CHAIN_DIV*2+2)+div*2] =
				D3DLVERTEX( D3DVECTOR(vPosMid.x+vRotView.x+vNoise.x, vPosMid.y+vRotView.y+vNoise.y, vPosMid.z+vRotView.z+vNoise.z), 0xffffffff, 0, u1, (div % 2) ? 1.0f : 0.0f);
			m_lverRect[line*(FX_CHAIN_DIV*2+2)+div*2+1] =
				D3DLVERTEX( D3DVECTOR(vPosMid.x-vRotView.x+vNoise.x, vPosMid.y-vRotView.y+vNoise.y, vPosMid.z-vRotView.z+vNoise.z), 0xffffffff, 0, u2, (div % 2) ? 1.0f : 0.0f);
		}
		m_lverRect[line*(FX_CHAIN_DIV*2+2)+FX_CHAIN_DIV*2] =
			D3DLVERTEX( D3DVECTOR(vPosTo.x+vRotView.x, vPosTo.y+vRotView.y, vPosTo.z+vRotView.z), 0xffffffff, 0, u1, (FX_CHAIN_DIV % 2) ? 1.0f : 0.0f);
		m_lverRect[line*(FX_CHAIN_DIV*2+2)+FX_CHAIN_DIV*2+1] =
			D3DLVERTEX( D3DVECTOR(vPosTo.x-vRotView.x, vPosTo.y-vRotView.y, vPosTo.z-vRotView.z), 0xffffffff, 0, u2, (FX_CHAIN_DIV % 2) ? 1.0f : 0.0f);
	}
}