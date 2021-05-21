#include "stdafx.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "ImageDefine.h"
#include "FxTLVertex.h"
#include "Skill.h"




CFxTLVertex::CFxTLVertex() {}

CFxTLVertex::~CFxTLVertex() {}

void CFxTLVertex::LoadRes()
{
	/*
		int nCenterX = g_pNk2DFrame->GetClientWidth()/2;
		int nCenterY = g_pNk2DFrame->GetClientHeight()/2;
		int nX = 250, nY = 150;

		m_TLVec[0] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY-nY, 0), 1, 0xffffffff, 0, 0, 0);
		m_TLVec[1] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY-nY, 0), 1, 0xffffffff, 0, 1, 0);
		m_TLVec[2] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY+nY, 0), 1, 0xffffffff, 0, 0, 1);
		m_TLVec[3] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY+nY, 0), 1, 0xffffffff, 0, 1, 1);
	*/
}

void CFxTLVertex::DeleteRes() {}

bool CFxTLVertex::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !g_pCapsyongTexture )
		return true;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,	true);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	if( effect_sort.Index == EFFECT_SKILL_MOB_SNAKEBOSS2 )
	{
		if( effect_sort.nCurFrame > 25 ) // 이펙트 제거 처리
			return TRUE;

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

		int nCenterX = g_pNk2DFrame->GetClientWidth()/2;
		int nCenterY = g_pNk2DFrame->GetClientHeight()/2;
		int nX = 250, nY = 150;

		m_TLVec[0] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY-nY, 0), 1, 0xffffffff, 0, 0, 0);
		m_TLVec[1] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY-nY, 0), 1, 0xffffffff, 0, 1, 0);
		m_TLVec[2] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY+nY, 0), 1, 0xffffffff, 0, 0, 1);
		m_TLVec[3] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY+nY, 0), 1, 0xffffffff, 0, 1, 1);

		int nAlpha = (int)(255 - (255 * (effect_sort.nCurFrame/(float)25))); // 이 이펙트는 25프레임 짜리.

		m_TLVec[0].color = RGBA_MAKE(255, 255, 255, nAlpha);
		m_TLVec[1].color = RGBA_MAKE(255, 255, 255, nAlpha);
		m_TLVec[2].color = RGBA_MAKE(255, 255, 255, nAlpha);
		m_TLVec[3].color = RGBA_MAKE(255, 255, 255, nAlpha);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_SCRATCH]));
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);
	}
	else if( effect_sort.Index == EFFECT_DECKAREN_CUT_IN )
	{
		DWORD ElapsedTick = timeGetTime() - effect_sort.dwColor;

		if( ElapsedTick > 3000 )
			return TRUE;

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		D3DVIEWPORT7 vpNew;
		GET_D3DDEVICE()->GetViewport(&vpNew);
		int nCenterX = vpNew.dwWidth/2;
		int nCenterY = vpNew.dwHeight/2;
		int nX = 256, nY = 256;

		if( effect_sort.subNum == 2 )
		{
			nX = 200;
			nY = 200;
		}

		int nAlpha = 500 - ( (float)ElapsedTick * 0.2f );

		if( effect_sort.subNum == 0 || effect_sort.subNum == 1 ) // 몬스터 등장 컷인
			nCenterY += (int)RandomNum(-5.0f,5.0f); // 위 아래로 흔들어 준다.
//		else if( effect_sort.subNum == 2 && ElapsedTick > 1000 )
//			effect_sort.subNum = 3;


		if( nAlpha < 0 )
			nAlpha = 0;
		if( nAlpha > 255 )
			nAlpha = 255;


		m_TLVec[0] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY-nY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 0, 0);
		m_TLVec[1] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY-nY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 1, 0);
		m_TLVec[2] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY+nY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 0, 1);
		m_TLVec[3] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY+nY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 1, 1);

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_DEKAREN_CUT_IN+effect_sort.subNum]));	// subNum에 컷인 번호.
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);
	}
	else if( effect_sort.Index == EFFECT_KOREA_SERVICE_LEVEL_MARK )
	{
		DWORD ElapsedTick = timeGetTime() - effect_sort.dwColor;

		if( ElapsedTick > 3000 )
			return TRUE;

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		D3DVIEWPORT7 vpNew;
		GET_D3DDEVICE()->GetViewport(&vpNew);
		float nSX = (float)vpNew.dwWidth-20; // 우측 X
		float nSY = 30+10; // 상단 Y
		float nWX = 220.5f, nHY = 85.5f; // 이미지 Width,Height

		int nAlpha = 500 - ( (float)ElapsedTick * 0.2f );

		if( nAlpha < 0 )
			nAlpha = 0;
		if( nAlpha > 255 )
			nAlpha = 255;


		m_TLVec[0] = D3DTLVERTEX(D3DVECTOR(nSX-nWX, nSY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 0, 0);
		m_TLVec[1] = D3DTLVERTEX(D3DVECTOR(nSX, nSY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 1, 0);
		m_TLVec[2] = D3DTLVERTEX(D3DVECTOR(nSX-nWX, nSY+nHY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 0, 1);
		m_TLVec[3] = D3DTLVERTEX(D3DVECTOR(nSX, nSY+nHY, 0), 1, RGBA_MAKE(255, 255, 255, nAlpha), 0, 1, 1);

		if( g_SvrType == ST_ADULT_ONLY ) // 18세이상
			GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_KOREA_SERVICE_LEVEL_MARK2]) );	// subNum에 컷인 번호.
		else
			GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_KOREA_SERVICE_LEVEL_MARK]) );	// subNum에 컷인 번호.

		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	return false;
}