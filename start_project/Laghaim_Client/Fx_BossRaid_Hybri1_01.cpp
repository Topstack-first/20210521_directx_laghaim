#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxAssault.h"
#include "GUtil.h"
#include "IndexedTexture.h"
#include "NkMob.h"
#include "NkCharacter.h"
#include "FX_BOSSRAID_HYBRI1_01.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidHybri1_01::CFxBossRaidHybri1_01()
	: mb_sound(false)
{
	m_board = NULL;
	m_board =  new CRectPlane;
	m_board->Create( 10.0f, 10.0f, FALSE );
}

CFxBossRaidHybri1_01::~CFxBossRaidHybri1_01()
{
	SAFE_DELETE(m_board);
}

void CFxBossRaidHybri1_01::LoadRes()
{
}

void CFxBossRaidHybri1_01::DeleteRes()
{
}

//-- num 1 : assault
//--     2 : D Assault
bool CFxBossRaidHybri1_01::Render(EffectSort &effect_sort)
{
	//-- 사용 캐릭이 사라질 경우
	if(!GET_D3DDEVICE() || !m_board)
		return true;

	DWORD dwTime = timeGetTime() - effect_sort.dwColor;
	float fTime = (float)dwTime / 1000.0f;

	DWORD  dwTextureState[3];
	DWORD  dwSrc, dwDest;

	float dx[4][2] = { { 0, 60 }, { 0, 60 }, { 0, 60 }, { 0, 60 } };
	float dy[4][2] = { { -10, 60 }, { -10, 60 }, { -10, 85 }, { -10, 85 } };
	float dz[4][2] = { { -20, 20 }, { -40, 5 }, { 10, 10 }, { -10, -10 } };

	D3DVECTOR vEyeVec = pCMyApp->GetEyeDirection();

	D3DMATRIX matR, matT, matWorld;
	D3DVECTOR vDrawPos;

	vDrawPos =  effect_sort.vTargetPos;
	vDrawPos.y += 14.0f;

	//--------------------------------------------------------------------------
	//-- Set Render State
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//--------------------------------------------------------------------------
	//-- Virframe
	//-- Dassault
	//----------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//-- Set Texture
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0,g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_ASSAULT ] ));

	//--------------------------------------------------------------------------
	//-- Effect 의 좌표를 갱신한다.
	//x = 28.0f;
	//z = 0.0f;

	//--------------------------------------------------------------------------
	//-- World Matrix
	// 수정자 : 이원석
	// 수정일 : 05-03-24
	// 수정내용 : pNkChaFrom에 널이 아닌 깨진 포인터가 참조되어 튕기는 버그가 있었음.
	// 기존의 어설트 스킬의 행렬을 계산시 방향을 참조 포인터에서 얻어오는 루틴을 그냥 From과 To로
	// 부터 계산해서 쓰는 방식으로 바꾸었음. ( InsertMagic에서 넣어줄때 TargetPos와 Pos를 기억하도록 변경 )
	D3DVECTOR vDir;
	get_normal_vector(vDir, effect_sort.vCurPos, effect_sort.vTargetPos);

	float fDir = 0.0f;
	fDir = atan2f(vDir.z, vDir.x);

	Nk3DUtil_SetRotateCCWYMatrix( matR, fDir );
	D3DUtil_SetTranslateMatrix( matT, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
	D3DMath_MatrixMultiply( matWorld, matR, matT );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	//--------------------------------------------------------------------------
	//-- Render
	//--------------------------------------------------------------------------

	//if(effect_sort.nCurFrame >= 16 && effect_sort.nCurFrame < 21)
	if ( fTime >= 0.6f && fTime < 1.0)
	{
		float fFrame = (fTime-0.6f) * 28;
		RenderFire(dx[0], dy[0], dz[0], fFrame);
		RenderFire(dx[1], dy[1], dz[1], fFrame);
		/*
		if (effect_sort.nCurFrame >= 16 && effect_sort.nCurFrame < 21)
		{
			RenderExplosion(GET_D3DDEVICE(), vEyeVec, vDrawPos, 0.5f, 2);
		}
		*/
	}

	else if ( fTime >= 1.4f && fTime < 1.8)
	{
		float fFrame = (fTime-1.4f) * 28;
		RenderFire(dx[2], dy[2], dz[2], fFrame);
		RenderFire(dx[3], dy[3], dz[3], fFrame);
		/*
		if (effect_sort.nCurFrame >= 32 && effect_sort.nCurFrame < 40)
		{
			RenderExplosion(GET_D3DDEVICE(), vEyeVec, vDrawPos, 0.8f, 3);
		}
		*/
	}

	//--------------------------------------------------------------------------
	//--Restore Render State
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );


	return FALSE;
}

void CFxBossRaidHybri1_01::RenderFire(float dx[2], float dy[2], float dz[2],
									  float fFrame)
{
	int i, j;

	float fade = 1.0f;
	float scale = 1.0f;

	int st_inframe = 0;
	int st_outframe = 0;
	int ed_inframe = 0;
	int ed_outframe = 0;

	m_flame[0].tv = 1.0f;
	m_flame[2].tv = 1.0f;
	m_flame[3].tv = 0.0f;
	m_flame[1].tv = 0.0f;

	//--------------------------------------------------------------------------
	//-- Two Side
	for(j=0; j<2; j++)
	{
		if(j)
			//--cw
		{
			m_flame[2].x = dx[0];
			m_flame[2].y = dy[0];
			m_flame[2].z = dz[0];
			m_flame[3].x = dx[0];
			m_flame[3].y = dy[1];
			m_flame[3].z = dz[1];
			m_flame[0].x = dx[1];
			m_flame[0].y = dy[0];
			m_flame[0].z = dz[0];
			m_flame[1].x = dx[1];
			m_flame[1].y = dy[1];
			m_flame[1].z = dz[1];
		}
		//-- ccw
		else
		{
			m_flame[0].x = dx[0];
			m_flame[0].y = dy[0];
			m_flame[0].z = dz[0];
			m_flame[1].x = dx[0];
			m_flame[1].y = dy[1];
			m_flame[1].z = dz[1];
			m_flame[2].x = dx[1];
			m_flame[2].y = dy[0];
			m_flame[2].z = dz[0];
			m_flame[3].x = dx[1];
			m_flame[3].y = dy[1];
			m_flame[3].z = dz[1];
		}

		//-- Texture animation
		for(i =0 ; i< 4 ; i++)
		{
			fade = 1.0f;
			scale = 30.0f;

			st_inframe = i*1;
			ed_inframe = st_inframe + 2;
			st_outframe = i*1 + 2;
			ed_outframe = st_outframe + 2;


			//-- 행동 방식 결정
			if(fFrame >=st_inframe && fFrame <ed_inframe )
				fade = (fFrame - st_inframe) /2.0f;
			else if(fFrame >=st_outframe && fFrame <ed_outframe )
				fade = (ed_outframe-fFrame) /2.0f;
			else fade =0.0f;

			if(fFrame > 18)
				fade = 0.0f;

			//-- Texture Animation 지정.
			if(j)
			{
				m_flame[2].tu = i*0.25;
				m_flame[3].tu = i*0.25;
				m_flame[1].tu = (i+1)*0.25;
				m_flame[0].tu = (i+1)*0.25;
			}
			else
			{
				m_flame[0].tu = i*0.25;
				m_flame[1].tu = i*0.25;
				m_flame[2].tu = (i+1)*0.25;
				m_flame[3].tu = (i+1)*0.25;
			}

			//-- Color 지정
			for(int t=0; t<4; t++)m_flame[t].color = D3DRGBA(1.0f,1.0f,1.0f,fade);

			//-- Render
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
		}
	}
}

void CFxBossRaidHybri1_01::RenderExplosion(D3DVECTOR vEyeVec,
		D3DVECTOR vPos,
		float fIntesity,
		float fScale)
{
	if( m_board )
	{
		if(g_pCapsyongTexture)
			GET_D3DDEVICE()->SetTexture(0,g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_ASSAULT_BOOM ] ));
		//-- Dev1
		m_board->SetBillBoard(vEyeVec, false, 0.0f);			//-- Set Billboard
		m_board->Scale(fScale*2, fScale*2, 0.0f);				//-- Scale
		m_board->SetColor(1.0f, 1.0f, 1.0f, fIntesity);    //-- color
		m_board->SetTranslationXYZ(vPos.x, vPos.y, vPos.z);
		m_board->Render();
	}
}
