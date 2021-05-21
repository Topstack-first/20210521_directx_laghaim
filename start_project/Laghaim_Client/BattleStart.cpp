#include "stdafx.h"
// Happy New Year Event

#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"
#include "IndexedTexture.h"
#include "BattleStart.h"
#include "main.h"
#include "LoadEffectSound.h"
#include "DirtSnd.h"



extern int g_EffectSound[];
extern float g_fTimeKey;

extern int		g_nCurWorldIdx;				// Current World Index ///인던1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleStart::CBattleStart()
{
	memset(m_CountDownId, 0, sizeof(m_CountDownId));

	int i=0;
	m_rectBattle = NULL;
	m_rectBattle = new D3DTLVERTEX[4];

	m_rectBattle[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectBattle[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectBattle[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectBattle[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectStart = NULL;
	m_rectStart = new D3DTLVERTEX[4];

	m_rectStart[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectStart[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectStart[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectStart[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectCountDown = NULL;
	m_rectCountDown = new D3DTLVERTEX[4];
	m_rectCountDown[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectCountDown[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectCountDown[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectCountDown[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectWinLose = NULL;
	m_rectWinLose = new D3DTLVERTEX[4];

	m_rectWinLose[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectWinLose[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectWinLose[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectWinLose[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_StartTexId =0;
	m_BattleTexId=0;

	m_WinTexId =0;
	m_LoseTexId=0;

	m_pIndexedTexture = NULL;

	m_CurFrame = 0;
	m_TexNum = 0;

	m_pIndexedTexture = new IndexedTexture( 18 );
	if( m_pIndexedTexture )
		m_pIndexedTexture->SetTexturePath("interface/BattleStart/");
}

CBattleStart::~CBattleStart()
{
	DeleteRes();
}

void CBattleStart::LoadRes()
{
	int	i;
	char strTexName[100] = {0,};

	if( !GET_D3DDEVICE() || !m_pIndexedTexture )
		return;

	if( m_pIndexedTexture )
	{
		m_BattleTexId = m_pIndexedTexture->CreateTextureFromFile( "Battle.tga" );
		m_StartTexId = m_pIndexedTexture->CreateTextureFromFile( "Start.tga" );

		for(i = 0; i < 10; i++)
		{
			sprintf( strTexName, "CountDown%d.tga", 10-i);
			m_CountDownId[i] = m_pIndexedTexture->CreateTextureFromFile( strTexName );
		}
		m_WinTexId = m_pIndexedTexture->CreateTextureFromFile( "Win.tga" );
		m_LoseTexId = m_pIndexedTexture->CreateTextureFromFile( "Lose.tga" );

		m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );
	}
}

void CBattleStart::DeleteRes()
{
	SAFE_DELETE( m_pIndexedTexture ); ///펫도움말 작업시에 이 녀석 메모리 해제 안하는걸 발견하여 수정.
	SAFE_DELETE_ARRAY(m_rectBattle);
	SAFE_DELETE_ARRAY(m_rectStart);
	SAFE_DELETE_ARRAY(m_rectCountDown);
	SAFE_DELETE_ARRAY(m_rectWinLose);
}

void CBattleStart::Render(int RendNum)
{
	int i = 0;
	if( !GET_D3DDEVICE() )
		return;

	static float fSaveTime = 0.0f;
	int nAddFrame = 0;
	int drawTotalFrame = 0;

	if(RendNum == 2 )
		drawTotalFrame = 234;
	else if( RendNum == 3 ) ///자유대련 시작 카운트
		drawTotalFrame = 117;
	else
		drawTotalFrame = 35;

	fSaveTime += g_fTimeKey;

	while (fSaveTime > 0.05f)
	{
		nAddFrame++;
		fSaveTime -= 0.05f;
	}

	m_CurFrame += nAddFrame;
	m_TexNum += nAddFrame;

	if( m_CurFrame > drawTotalFrame )
	{
		m_CurFrame = 0;
		m_TexNum = 0;
		pCMyApp->DrawBattleStart = FALSE;
		return;
	}

	if( m_TexNum >= 6 )
		m_TexNum = 0;
//=============================================================
	DWORD dwlingting, dwAlphaTest, dwAlphaBlend, dwRenState[2];
	DWORD dwTextureState[3];

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwlingting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest);

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVIEWPORT7 vp;
	GET_D3DDEVICE()->GetViewport( &vp );
//=============================================================
	//battle start count
	if( RendNum == 2 ) ///자유대련 시 수정.
	{
		if(m_CurFrame < 200 )
		{
			i = m_CurFrame/20;
			float alpha = 0.9f;
			m_rectCountDown[0].sx = (FLOAT)vp.dwWidth/2 - 60 ;
			m_rectCountDown[1].sx = (FLOAT)vp.dwWidth/2 - 60 ;
			m_rectCountDown[2].sx = (FLOAT)vp.dwWidth/2 + 60 ;
			m_rectCountDown[3].sx = (FLOAT)vp.dwWidth/2 + 60 ;

			m_rectCountDown[1].sy = (FLOAT)vp.dwHeight/2 - 120;
			m_rectCountDown[3].sy = (FLOAT)vp.dwHeight/2 - 120;
			m_rectCountDown[0].sy = (FLOAT)vp.dwHeight/2 +0;
			m_rectCountDown[2].sy = (FLOAT)vp.dwHeight/2 +0;

			alpha -= (float)(m_CurFrame%20)/15.0f;
			//		if(!i) alpha=0;
			if(alpha<0)
				alpha=0;
			m_rectCountDown[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectCountDown[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectCountDown[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectCountDown[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

			GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_CountDownId[i]));

			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectCountDown, 4, 0 );
			if (g_pDSound && m_CurFrame%20<1)
			{
				if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_COUNT]))
					g_pDSound->Play(g_EffectSound[EFF_SND_COUNT]);
			}
		}
		//=============================================================
		//battle start
		if( m_CurFrame>200 )
		{
			int m_CurFrame1 = m_CurFrame- 200;

			if(m_CurFrame1 < 5)
			{
				m_rectBattle[0].sx = (FLOAT)vp.dwWidth/2 - 500+ m_CurFrame1*60;
				m_rectBattle[1].sx = (FLOAT)vp.dwWidth/2 - 500+ m_CurFrame1*60;
				m_rectBattle[2].sx = (FLOAT)vp.dwWidth/2 - 100+ m_CurFrame1*60;
				m_rectBattle[3].sx = (FLOAT)vp.dwWidth/2 - 100+ m_CurFrame1*60;

				if (g_pDSound&&m_CurFrame1<4)
				{
					if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_BATTLESTART]))
						g_pDSound->Play(g_EffectSound[EFF_SND_BATTLESTART]);
				}

			}
			else
			{
				m_rectBattle[0].sx = (FLOAT)vp.dwWidth/2 - 500+ 300;
				m_rectBattle[1].sx = (FLOAT)vp.dwWidth/2 - 500+ 300;
				m_rectBattle[2].sx = (FLOAT)vp.dwWidth/2 - 100+ 300;
				m_rectBattle[3].sx = (FLOAT)vp.dwWidth/2 - 100+ 300;
			}
			m_rectBattle[1].sy = (FLOAT)vp.dwHeight/2 - 200;
			m_rectBattle[3].sy = (FLOAT)vp.dwHeight/2 - 200;
			m_rectBattle[0].sy = (FLOAT)vp.dwHeight/2 - 50;
			m_rectBattle[2].sy = (FLOAT)vp.dwHeight/2 - 50;

			float alpha = 1.0f;

			if( m_CurFrame1 < 20 )
				alpha = 1.0f;
			else
			{
				int curFrame = 35 - m_CurFrame1;
				alpha = ((float)curFrame / 15);
			}
			m_rectBattle[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectBattle[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectBattle[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectBattle[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

			GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_BattleTexId));

			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX,
											m_rectBattle, 4, 0 );
			//=============================================================
			if(m_CurFrame1>4)
			{
				if(m_CurFrame1<10)
				{
					m_rectBattle[0].sx = (FLOAT)vp.dwWidth/2 + 100 - (m_CurFrame1-5)*60;
					m_rectBattle[1].sx = (FLOAT)vp.dwWidth/2 + 100 - (m_CurFrame1-5)*60;
					m_rectBattle[2].sx = (FLOAT)vp.dwWidth/2 + 500 - (m_CurFrame1-5)*60;
					m_rectBattle[3].sx = (FLOAT)vp.dwWidth/2 + 500 - (m_CurFrame1-5)*60;
				}
				else
				{
					m_rectBattle[0].sx = (FLOAT)vp.dwWidth/2  + 100 - 300;
					m_rectBattle[1].sx = (FLOAT)vp.dwWidth/2  + 100 - 300;
					m_rectBattle[2].sx = (FLOAT)vp.dwWidth/2  + 500 - 300;
					m_rectBattle[3].sx = (FLOAT)vp.dwWidth/2  + 500 - 300;
				}
				m_rectBattle[1].sy = (FLOAT)vp.dwHeight/2 + 0;
				m_rectBattle[3].sy = (FLOAT)vp.dwHeight/2 + 0;
				m_rectBattle[0].sy = (FLOAT)vp.dwHeight/2 + 150;
				m_rectBattle[2].sy = (FLOAT)vp.dwHeight/2 + 150;

				float alpha = 1.0f;

				if( m_CurFrame1 < 20 )
				{
					alpha = 1.0f;
				}
				else
				{
					int curFrame = 35 - m_CurFrame1;
					alpha = ((float)curFrame / 15);
				}
				m_rectBattle[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
				m_rectBattle[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
				m_rectBattle[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
				m_rectBattle[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

				GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_StartTexId));

				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX,
												m_rectBattle, 4, 0 );
			}
		}
	}
	else if( RendNum == 3 ) ///자유대련 시작 카운트.
	{
		if(m_CurFrame < 100 )
		{
			i = m_CurFrame/20+5;
			float alpha = 0.9f;
			m_rectCountDown[0].sx = (FLOAT)vp.dwWidth/2 - 60 ;
			m_rectCountDown[1].sx = (FLOAT)vp.dwWidth/2 - 60 ;
			m_rectCountDown[2].sx = (FLOAT)vp.dwWidth/2 + 60 ;
			m_rectCountDown[3].sx = (FLOAT)vp.dwWidth/2 + 60 ;

			m_rectCountDown[1].sy = (FLOAT)vp.dwHeight/2 - 120;
			m_rectCountDown[3].sy = (FLOAT)vp.dwHeight/2 - 120;
			m_rectCountDown[0].sy = (FLOAT)vp.dwHeight/2 + 0;
			m_rectCountDown[2].sy = (FLOAT)vp.dwHeight/2 + 0;

			alpha -= (float)(m_CurFrame%20)/15.0f;
			//		if(!i) alpha=0;
			if(alpha<0)
				alpha=0;
			m_rectCountDown[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectCountDown[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectCountDown[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
			m_rectCountDown[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

			GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_CountDownId[i]));

			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectCountDown, 4, 0 );
			if (g_pDSound && m_CurFrame%20<1)
			{
				if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_COUNT]))
					g_pDSound->Play(g_EffectSound[EFF_SND_COUNT]);
			}
		}
		//=============================================================
		//battle start
		if( m_CurFrame>100 && g_nCurWorldIdx != 1 ) // 데카렌 카운트는 Battle Start 안나오게... ///인던1 ///자유대련
		{
			int m_CurFrame1 = m_CurFrame- 100;
//			if(m_CurFrame1>4)
			{
				if(m_CurFrame1<10)
				{
					m_rectBattle[0].sx = (FLOAT)vp.dwWidth/2 + 100 - (m_CurFrame1-5)*60;
					m_rectBattle[1].sx = (FLOAT)vp.dwWidth/2 + 100 - (m_CurFrame1-5)*60;
					m_rectBattle[2].sx = (FLOAT)vp.dwWidth/2 + 500 - (m_CurFrame1-5)*60;
					m_rectBattle[3].sx = (FLOAT)vp.dwWidth/2 + 500 - (m_CurFrame1-5)*60;
				}
				else
				{
					m_rectBattle[0].sx = (FLOAT)vp.dwWidth/2  + 100 - 300;
					m_rectBattle[1].sx = (FLOAT)vp.dwWidth/2  + 100 - 300;
					m_rectBattle[2].sx = (FLOAT)vp.dwWidth/2  + 500 - 300;
					m_rectBattle[3].sx = (FLOAT)vp.dwWidth/2  + 500 - 300;
				}
				m_rectBattle[1].sy = (FLOAT)vp.dwHeight/2 - 120;
				m_rectBattle[3].sy = (FLOAT)vp.dwHeight/2 - 120;
				m_rectBattle[0].sy = (FLOAT)vp.dwHeight/2 + 0;
				m_rectBattle[2].sy = (FLOAT)vp.dwHeight/2 + 0;

				float alpha = 1.0f;

				if( m_CurFrame1 < 20 )
				{
					alpha = 1.0f;
				}
				else
				{
					int curFrame = 35 - m_CurFrame1;
					alpha = ((float)curFrame / 15);
				}
				m_rectBattle[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
				m_rectBattle[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
				m_rectBattle[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
				m_rectBattle[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

				GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_StartTexId));

				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX,
												m_rectBattle, 4, 0 );
			}
		}
	}

//=============================================================
	//win lose
	if(RendNum ==0||RendNum ==1)
	{
		int curFrame =m_CurFrame;

		float alpha = 1.0f;
		if(curFrame>200)
			curFrame=200;

		m_rectWinLose[0].sx = (FLOAT)vp.dwWidth/2 - curFrame*8;
		m_rectWinLose[1].sx = (FLOAT)vp.dwWidth/2 - curFrame*8;
		m_rectWinLose[2].sx = (FLOAT)vp.dwWidth/2 + curFrame*8;
		m_rectWinLose[3].sx = (FLOAT)vp.dwWidth/2 + curFrame*8;

		m_rectWinLose[1].sy = (FLOAT)vp.dwHeight/2 - curFrame*6;
		m_rectWinLose[3].sy = (FLOAT)vp.dwHeight/2 - curFrame*6;
		m_rectWinLose[0].sy = (FLOAT)vp.dwHeight/2 + curFrame*6;
		m_rectWinLose[2].sy = (FLOAT)vp.dwHeight/2 + curFrame*6;

		alpha = (float)(curFrame)/0.09f;

		if( alpha > 1.0f )
			alpha = 1.0f;

		m_rectWinLose[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectWinLose[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectWinLose[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		m_rectWinLose[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
		if(RendNum==1)
		{
			// win
			GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_WinTexId));
			if (g_pDSound)
			{
				if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_WIN]))
					g_pDSound->Play(g_EffectSound[EFF_SND_WIN]);
			}
		}
		if(RendNum==0)
		{
			// lose
			GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_LoseTexId));
			if (g_pDSound)
			{
				if(!g_pDSound->IsPlaying(g_EffectSound[EFF_SND_WIN]))
					g_pDSound->Play(g_EffectSound[EFF_SND_LOSE]);
			}

		}

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectWinLose, 4, 0 );

	}
//=============================================================
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

	return;
}

