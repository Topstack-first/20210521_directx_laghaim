#include "stdafx.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "ImageDefine.h"
#include "Fx_BossRaid_bulkan3_02.h"
#include "Skill.h"






CFxBossRaidBulkan3_02::CFxBossRaidBulkan3_02()
{
	for( int i = 0; i < 3; i++ )
	{
		m_bStartFrame[i] = false;
		m_nStartTime[i] = 0;
	}
}

CFxBossRaidBulkan3_02::~CFxBossRaidBulkan3_02() {}

void CFxBossRaidBulkan3_02::LoadRes() {}

void CFxBossRaidBulkan3_02::DeleteRes() {}

bool CFxBossRaidBulkan3_02::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() || !g_pCapsyongTexture )
		return true;

	D3DVECTOR Eye;
	pCMyApp->GetEyePos( Eye );
	D3DVECTOR At;
	pCMyApp->GetEyeAtPos( At );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,	true);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	if( effect_sort.Index == EFFECT_BOSSRAID_BULKAN3_01 )
	{
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

		int nCenterX = g_pNk2DFrame->GetClientWidth()/2;
		int nCenterY = g_pNk2DFrame->GetClientHeight()/2;
		int nX = g_pNk2DFrame->GetClientWidth()/2, nY = g_pNk2DFrame->GetClientWidth()/2;
		int move_Pos = 50;

		m_TLVec[0] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY-nY, 0), 1, 0xffffffff, 0, 0, 0);
		m_TLVec[1] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY-nY, 0), 1, 0xffffffff, 0, 1, 0);
		m_TLVec[2] = D3DTLVERTEX(D3DVECTOR(nCenterX-nX, nCenterY+nY, 0), 1, 0xffffffff, 0, 0, 1);
		m_TLVec[3] = D3DTLVERTEX(D3DVECTOR(nCenterX+nX, nCenterY+nY, 0), 1, 0xffffffff, 0, 1, 1);

		switch( effect_sort.nCurFrame )
		{
		case _FX_BULKANBOSS3_02_1_START:
		case _FX_BULKANBOSS3_02_1_START + 1:
		case _FX_BULKANBOSS3_02_1_START + 2:
			m_bStartFrame[0] = true;
			m_nStartTime[0] = timeGetTime();
			break;

		case _FX_BULKANBOSS3_02_2_START:
		case _FX_BULKANBOSS3_02_2_START + 1:
		case _FX_BULKANBOSS3_02_2_START + 2:
			m_bStartFrame[1] = true;
			m_nStartTime[1] = timeGetTime();
			break;

		case _FX_BULKANBOSS3_02_3_START:
		case _FX_BULKANBOSS3_02_3_START + 1:
		case _FX_BULKANBOSS3_02_3_START + 2:
			m_bStartFrame[2] = true;
			m_nStartTime[2] = timeGetTime();
			break;
		}

		if( m_bStartFrame[0] )
		{
			m_TLVec[0].sx = nCenterX-nX + move_Pos;
			m_TLVec[1].sx = nCenterX+nX + move_Pos;
			m_TLVec[2].sx = nCenterX-nX + move_Pos;
			m_TLVec[3].sx = nCenterX+nX + move_Pos;

			m_TLVec[0].sy = nCenterY-nY;
			m_TLVec[1].sy = nCenterY-nY;
			m_TLVec[2].sy = nCenterY+nY;
			m_TLVec[3].sy = nCenterY+nY;

			SetAlpha( m_TLVec, effect_sort, _FX_BULKANBOSS3_EFFECT_ALPHATIME + 10 );

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_BOSSRAID_BULKAN3_01_01]));
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

			SetRenderFrame( 0 );

			CameraShakeFrame( effect_sort.nCurFrame, _FX_BULKANBOSS3_02_1_START + 1, _FX_BULKANBOSS3_02_1_START + 2 );
		}

		if( m_bStartFrame[1] )
		{
			m_TLVec[0].sx = nCenterX-nX;
			m_TLVec[1].sx = nCenterX+nX;
			m_TLVec[2].sx = nCenterX-nX;
			m_TLVec[3].sx = nCenterX+nX;

			m_TLVec[0].sy = nCenterY-nY + move_Pos;
			m_TLVec[1].sy = nCenterY-nY + move_Pos;
			m_TLVec[2].sy = nCenterY+nY + move_Pos;
			m_TLVec[3].sy = nCenterY+nY + move_Pos;

			SetAlpha( m_TLVec, effect_sort, _FX_BULKANBOSS3_EFFECT_ALPHATIME + 20 );

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_BOSSRAID_BULKAN3_01_02]));
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

			SetRenderFrame( 1 );

			CameraShakeFrame( effect_sort.nCurFrame, _FX_BULKANBOSS3_02_2_START + 1, _FX_BULKANBOSS3_02_2_START + 2 );
		}

		if( m_bStartFrame[2] )
		{
			m_TLVec[0].sx = nCenterX-nX - ( move_Pos * 2 );
			m_TLVec[1].sx = nCenterX+nX - ( move_Pos * 2 );
			m_TLVec[2].sx = nCenterX-nX - ( move_Pos * 2 );
			m_TLVec[3].sx = nCenterX+nX - ( move_Pos * 2 );

			m_TLVec[0].sy = nCenterY-nY;
			m_TLVec[1].sy = nCenterY-nY;
			m_TLVec[2].sy = nCenterY+nY;
			m_TLVec[3].sy = nCenterY+nY;

			SetAlpha( m_TLVec, effect_sort, _FX_BULKANBOSS3_EFFECT_ALPHATIME + 30 );

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_BOSSRAID_BULKAN3_01_03]));
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

			SetRenderFrame( 2 );

			CameraShakeFrame( effect_sort.nCurFrame, _FX_BULKANBOSS3_02_3_START + 1, _FX_BULKANBOSS3_02_3_START + 2 );
		}

		if( effect_sort.nCurFrame >= ( _FX_BULKANBOSS3_02_3_START + 10 ) )
		{
			pCMyApp->SetCameraInitPos( Eye, At );
		}

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	}

	return false;
}

void CFxBossRaidBulkan3_02::SetAlpha(D3DTLVERTEX *in_TLVec, EffectSort &in_effect_sort, float in_Time)
{
	int nAlpha = (int)(255 - (255 * (in_effect_sort.nCurFrame/(float)in_Time)));

	if( nAlpha < 0 )
		nAlpha = 0;

	if( g_SvrType != ST_ADULT_ONLY )
	{
		in_TLVec[0].color = RGBA_MAKE(_FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, nAlpha);
		in_TLVec[1].color = RGBA_MAKE(_FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, nAlpha);
		in_TLVec[2].color = RGBA_MAKE(_FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, nAlpha);
		in_TLVec[3].color = RGBA_MAKE(_FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, _FX_BULKANBOSS3_VERTEX_COLOR, nAlpha);
	}
	else // 성인 서버는 리얼하게..
	{
		in_TLVec[0].color = RGBA_MAKE(100, 100, 100, nAlpha);
		in_TLVec[1].color = RGBA_MAKE(100, 100, 100, nAlpha);
		in_TLVec[2].color = RGBA_MAKE(100, 100, 100, nAlpha);
		in_TLVec[3].color = RGBA_MAKE(100, 100, 100, nAlpha);
	}
}

void CFxBossRaidBulkan3_02::CameraShakeFrame( int currentFrame, int startFrame, int endFrame  )
{
	if( currentFrame == startFrame )
	{
		pCMyApp->GoRight(14.0f);
	}

	if( currentFrame == endFrame )
	{
		pCMyApp->GoLeft(14.0f);
	}
}

void CFxBossRaidBulkan3_02::SetRenderFrame( int num )
{
	if( m_bStartFrame[num] )
	{
		DWORD  curTime = timeGetTime();

		switch(num)
		{
		case 0:
			if( ( curTime - m_nStartTime[0] ) > _FX_BULKANBOSS3_EFFECT_ENDTIME + 500 )
			{
				m_bStartFrame[0] = false;
				m_nStartTime[0] = 0;
			}
			break;

		case 1:
			if( ( curTime - m_nStartTime[1] ) > ( _FX_BULKANBOSS3_EFFECT_ENDTIME + 800 ) )
			{
				m_bStartFrame[1] = false;
				m_nStartTime[1] = 0;
			}
			break;

		case 2:
			if( ( curTime - m_nStartTime[2] ) > ( _FX_BULKANBOSS3_EFFECT_ENDTIME + 1100 ) )
			{
				m_bStartFrame[2] = false;
				m_nStartTime[2] = 0;
			}
			break;
		}
	}
}
