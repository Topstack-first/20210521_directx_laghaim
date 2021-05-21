#include "stdafx.h"
// SpiritMode.cpp: implementation of the CSpiritMode class.
//
//////////////////////////////////////////////////////////////////////

#include "D3DApp.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"

#include "IndexedTexture.h"
#include "main.h"
#include "DirtSnd.h"
#include "ImageDefine.h"




extern float g_fTimeKey;

extern BOOL g_bGhost; ///070723 일본버그리포트 수정 고스트 모드땐 스피릿 안 보이게...


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpiritMode::CSpiritMode()
	: m_nSpiritTime(0)
{
	m_pSurfaceBack = NULL;

	m_rectGetScore1000 = NULL;
	m_rectGetScore100 = NULL;
	m_rectGetScore10 = NULL;
	m_rectGetScore1 = NULL;
	m_rectSlash = NULL;
	m_rectTagetScore100 = NULL;
	m_rectTagetScore10 = NULL;
	m_rectTagetScore1 = NULL;

	m_nCurFrame = 0;

	m_nGetPoint1000 = 0;
	m_nGetPoint100 = 0;
	m_nGetPoint10 = 0;
	m_nGetPoint1 = 0;
	m_nTagetPoint100 = 0;
	m_nTagetPoint10 = 0;
	m_nTagetPoint1 = 0;

	m_dwLastTickTime = 0;
}

CSpiritMode::~CSpiritMode()
{
	DeleteRes();
}

void CSpiritMode::LoadRes()
{
	RECT rcClient;
	GetClientRect(g_pDisplay->GetHWnd(), &rcClient);

	D3DVIEWPORT7 vp;
	GET_D3DDEVICE()->GetViewport( &vp );

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/spirit/Spirit_back.bmp", 195, 128);

	if( m_pSurfaceBack )
	{
		m_pSurfaceBack->SetColorKey(RGB(0, 0, 0));
		m_pSurfaceBack->Xpos = vp.dwWidth/2 - 80;
		m_pSurfaceBack->Ypos = 0;
	}

	m_rectGetScore1000 = new D3DTLVERTEX[4];
	m_rectGetScore100 = new D3DTLVERTEX[4];
	m_rectGetScore10 = new D3DTLVERTEX[4];
	m_rectGetScore1 = new D3DTLVERTEX[4];
	m_rectSlash = new D3DTLVERTEX[4];
	m_rectTagetScore100 = new D3DTLVERTEX[4];
	m_rectTagetScore10 = new D3DTLVERTEX[4];
	m_rectTagetScore1 = new D3DTLVERTEX[4];

	m_rectGetScore1000[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectGetScore1000[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectGetScore1000[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectGetScore1000[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectGetScore100[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectGetScore100[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectGetScore100[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectGetScore100[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectGetScore10[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectGetScore10[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectGetScore10[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectGetScore10[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectGetScore1[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectGetScore1[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectGetScore1[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectGetScore1[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectSlash[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectSlash[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectSlash[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectSlash[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectTagetScore100[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectTagetScore100[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectTagetScore100[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectTagetScore100[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectTagetScore10[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectTagetScore10[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectTagetScore10[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectTagetScore10[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectTagetScore1[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectTagetScore1[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectTagetScore1[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectTagetScore1[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_SpriteFireAni.LoadRes( "interface/spirit/fire", 3, 200, RGB(255,0,255) );
	m_SpriteFireAni.m_AniType = SPRITE_ANITYPE_LOOP; // 이 애니메이션은 한번 반복으로 셋팅.
	m_SpriteFireAni.SetStart(); // 애니메이션 시작.

}

void CSpiritMode::SpiritSetPoint( int nGet_Thousand, int nGet_Hundred, int nGet_Ten, int nGet_One, int nTaget_Hundred, int nTaget_Ten, int nTaget_One)
{
	m_nGetPoint1000 = nGet_Thousand;
	m_nGetPoint100 = nGet_Hundred;
	m_nGetPoint10 = nGet_Ten;
	m_nGetPoint1 = nGet_One;
	m_nTagetPoint100 = nTaget_Hundred;
	m_nTagetPoint10 = nTaget_Ten;
	m_nTagetPoint1 = nTaget_One;
}

void CSpiritMode::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);

	m_SpriteFireAni.DeleteRes();

	SAFE_DELETE_ARRAY(m_rectGetScore1000);
	SAFE_DELETE_ARRAY(m_rectGetScore100);
	SAFE_DELETE_ARRAY(m_rectGetScore10);
	SAFE_DELETE_ARRAY(m_rectGetScore1);
	SAFE_DELETE_ARRAY(m_rectSlash);
	SAFE_DELETE_ARRAY(m_rectTagetScore100);
	SAFE_DELETE_ARRAY(m_rectTagetScore10);
	SAFE_DELETE_ARRAY(m_rectTagetScore1);
}

void CSpiritMode::SpiritEnd( BOOL bEnd )
{

}

void CSpiritMode::Draw()
{
	if( g_bGhost ) ///070723 일본버그리포트 수정 고스트 모드땐 스피릿 안 보이게...
		return;

	D3DVIEWPORT7 vp;
	GET_D3DDEVICE()->GetViewport( &vp );

	// 게이지 화면에 뿌려주어야할 좌표
	int Xpos[10]		= { 63, 71, 82, 93, 104, 115, 126, 137, 148, 159};
	int Ypos[10]		= { 100, 92, 92, 92, 92,  92,  92,  92,  92,  92 };
	int Width			= 10;
	int Height			= 10;
	int TempDrawFrame	= 0;	// 각 불마다 그릴 프레임 번호만 따로 계산하기위한 임시 변수.

	m_SpriteFireAni.UpdateFast( timeGetTime()-m_dwLastTickTime );

	for( int i = 0; i < m_nSpiritTime; i++)
	{
		// 마지막만 깜빡임 처리
		if( m_nSpiritTime-1 == i )
		{
			TempDrawFrame = m_SpriteFireAni.m_NowFrame;

			if( TempDrawFrame >= m_SpriteFireAni.m_NumOfImage ) // 프레임이 맥스치를 넘어가면...
				TempDrawFrame %= m_SpriteFireAni.m_NumOfImage;
		}

		m_SpriteFireAni.DrawFrame( TempDrawFrame, Xpos[i], Ypos[i], Width, Height ); // 프레임을 약간씩 다르게 해서 출력.
	}

	m_dwLastTickTime = timeGetTime();


	DrawPoint();

	return;
}

void CSpiritMode::SetFrame( int nCurFrame )
{
	m_nCurFrame = nCurFrame;
}

void CSpiritMode::SetSpiritTime( int nRemainTime )
{
	m_nSpiritTime = nRemainTime;
}

void CSpiritMode::DrawPoint()
{
	if( !GET_D3DDEVICE() )
		return;

	D3DVIEWPORT7 vp;
	GET_D3DDEVICE()->GetViewport( &vp );

	static float fSaveTime = 0.0f;
	int nAddFrame = 0;
	int drawTotalFrame = 10;

	// 애니메이션 계산인듯...
	if( m_nCurFrame < drawTotalFrame ) // 작을때만 프레임 계산하면 좀 더 빠를듯.
	{
		fSaveTime += g_fTimeKey;

		while (fSaveTime > 0.05f)
		{
			nAddFrame++;
			fSaveTime -= 0.05f;
		}

		m_nCurFrame += nAddFrame;

		if( m_nCurFrame > drawTotalFrame )
			m_nCurFrame = drawTotalFrame;
	}


	// 실제 숫자를 표시하는 부분
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
	//=============================================================

	float alpha = 1.0f;

	alpha = (float)m_nCurFrame / 10;

	// 슬래쉬
	m_rectSlash[0].sx = 119;
	m_rectSlash[1].sx = 119;
	m_rectSlash[2].sx = 119+12;
	m_rectSlash[3].sx = 119+12;
	m_rectSlash[1].sy = 105;
	m_rectSlash[3].sy = 105;
	m_rectSlash[0].sy = 117;
	m_rectSlash[2].sy = 117;

	// 총포인트 백의자리
	m_rectTagetScore100[0].sx = 135;
	m_rectTagetScore100[1].sx = 135;
	m_rectTagetScore100[2].sx = 135+12;
	m_rectTagetScore100[3].sx = 135+12;
	m_rectTagetScore100[1].sy = 105;
	m_rectTagetScore100[3].sy = 105;
	m_rectTagetScore100[0].sy = 117;
	m_rectTagetScore100[2].sy = 117;

	m_rectTagetScore100[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore100[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore100[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore100[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);

	// 총포인트 십의자리
	m_rectTagetScore10[0].sx = 143;
	m_rectTagetScore10[1].sx = 143;
	m_rectTagetScore10[2].sx = 143+12;
	m_rectTagetScore10[3].sx = 143+12;
	m_rectTagetScore10[1].sy = 105;
	m_rectTagetScore10[3].sy = 105;
	m_rectTagetScore10[0].sy = 117;
	m_rectTagetScore10[2].sy = 117;

	m_rectTagetScore10[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore10[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore10[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore10[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);

	// 총포인트 일의자리
	m_rectTagetScore1[0].sx = 151;
	m_rectTagetScore1[1].sx = 151;
	m_rectTagetScore1[2].sx = 151+12;
	m_rectTagetScore1[3].sx = 151+12;
	m_rectTagetScore1[1].sy = 105;
	m_rectTagetScore1[3].sy = 105;
	m_rectTagetScore1[0].sy = 117;
	m_rectTagetScore1[2].sy = 117;

	m_rectTagetScore1[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore1[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore1[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);
	m_rectTagetScore1[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, 1.0f);


	m_rectGetScore1000[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore1000[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore1000[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore1000[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore100[0].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore100[1].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore100[2].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore100[3].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore10[0].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore10[1].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore10[2].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore10[3].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore1[0].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore1[1].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore1[2].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectGetScore1[3].color	= D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

	// 천의 자리의 값이 있다면 나머지도 있으니 그냥 출력
	if( m_nGetPoint1000 > 0 )
	{
		// 얻은 포인트 천의자리
		m_rectGetScore1000[0].sx = 79 - ( (10-m_nCurFrame) * 4 );
		m_rectGetScore1000[1].sx = 79 - ( (10-m_nCurFrame) * 4 );
		m_rectGetScore1000[2].sx = 91 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1000[3].sx = 91 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1000[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1000[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1000[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1000[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		// 얻은 포인트 백의자리
		m_rectGetScore100[0].sx = 87 - ( (10-m_nCurFrame) * 2 )	;
		m_rectGetScore100[1].sx = 87 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore100[2].sx = 87+12							;
		m_rectGetScore100[3].sx = 87+12;
		m_rectGetScore100[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore100[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore100[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore100[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		// 얻은 포인트 십의자리
		m_rectGetScore10[0].sx = 95;
		m_rectGetScore10[1].sx = 95;
		m_rectGetScore10[2].sx = 95+12 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[3].sx = 95+12 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		// 얻은 포인트 일의자리
		m_rectGetScore1[0].sx = 103 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[1].sx = 103 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[2].sx = 103+12 + ( (10-m_nCurFrame) * 4 );
		m_rectGetScore1[3].sx = 103+12 + ( (10-m_nCurFrame) * 4 );
		m_rectGetScore1[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint1000 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore1000, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint100 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore100, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint10 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore10, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint1 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore1, 4, 0 );
	}

	// 백의 자리 출력
	else if( m_nGetPoint1000 <= 0 && m_nGetPoint100 > 0 )
	{
		// 얻은 포인트 백의자리
		m_rectGetScore100[0].sx = 87 - ( (10-m_nCurFrame)  * 4)	;
		m_rectGetScore100[1].sx = 87 - ( (10-m_nCurFrame)  * 4);
		m_rectGetScore100[2].sx = 87+12	- ( (10-m_nCurFrame) * 2);
		m_rectGetScore100[3].sx = 87+12 - ( (10-m_nCurFrame) * 2);
		m_rectGetScore100[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore100[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore100[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore100[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		// 얻은 포인트 십의자리
		m_rectGetScore10[0].sx = 95 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[1].sx = 95 - ( (10-m_nCurFrame) * 2);
		m_rectGetScore10[2].sx = 95+12 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[3].sx = 95+12 + ( (10-m_nCurFrame) * 2);
		m_rectGetScore10[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		// 얻은 포인트 일의자리
		m_rectGetScore1[0].sx = 103 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[1].sx = 103 + ( (10-m_nCurFrame) * 2);
		m_rectGetScore1[2].sx = 103+12 + ( (10-m_nCurFrame) *  4);
		m_rectGetScore1[3].sx = 103+12 + ( (10-m_nCurFrame) * 4 );
		m_rectGetScore1[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint100 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore100, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint10 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore10, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint1 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore1, 4, 0 );
	}

	// 십의 자리 출력
	else if( m_nGetPoint1000 <= 0 && m_nGetPoint100 <= 0 && m_nGetPoint10 > 0 )
	{
		// 얻은 포인트 십의자리
		m_rectGetScore10[0].sx = 95 - ( (10-m_nCurFrame) * 4 );
		m_rectGetScore10[1].sx = 95 - ( (10-m_nCurFrame) * 4 );
		m_rectGetScore10[2].sx = 95+12;
		m_rectGetScore10[3].sx = 95+12;
		m_rectGetScore10[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore10[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		// 얻은 포인트 일의자리
		m_rectGetScore1[0].sx = 103;
		m_rectGetScore1[1].sx = 103;
		m_rectGetScore1[2].sx = 103+12 + ( (10-m_nCurFrame) * 4 );
		m_rectGetScore1[3].sx = 103+12 + ( (10-m_nCurFrame) * 4 );
		m_rectGetScore1[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint10 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore10, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint1 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore1, 4, 0 );
	}

	// 일의 자리
	else
	{
		// 얻은 포인트 일의자리
		m_rectGetScore1[0].sx = 103 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[1].sx = 103 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[2].sx = 103+12 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[3].sx = 103+12 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[1].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[3].sy = 105 - ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[0].sy = 117 + ( (10-m_nCurFrame) * 2 );
		m_rectGetScore1[2].sy = 117 + ( (10-m_nCurFrame) * 2 );

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT_R + m_nGetPoint1 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectGetScore1, 4, 0 );
	}

	// 슬래쉬
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITSLASH ] ));
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectSlash, 4, 0 );

	if( m_nTagetPoint100 > 0 )
	{
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT + m_nTagetPoint100 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectTagetScore100, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT + m_nTagetPoint10 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectTagetScore10, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT + m_nTagetPoint1 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectTagetScore1, 4, 0 );
	}
	else if( m_nTagetPoint100 <= 0 && m_nTagetPoint10 > 0 )
	{
		LPD3DTLVERTEX	rectTemp10, rectTemp1;

		rectTemp10	= m_rectTagetScore100;
		rectTemp1	= m_rectTagetScore10;

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT + m_nTagetPoint10 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, rectTemp10, 4, 0 );
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT + m_nTagetPoint1 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, rectTemp1, 4, 0 );
	}
	else
	{
		LPD3DTLVERTEX	rectTemp1;

		rectTemp1	= m_rectTagetScore100;

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SPIRITPOINT + m_nTagetPoint1 ] ));
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, rectTemp1, 4, 0 );
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
}

BOOL CSpiritMode::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return FALSE;

	if (x >= m_pSurfaceBack->Xpos
			&& x < m_pSurfaceBack->Xpos + m_pSurfaceBack->GetWidth()
			&& y >= m_pSurfaceBack->Ypos
			&& y < m_pSurfaceBack->Ypos + m_pSurfaceBack->GetHeight() )
		return TRUE;

	return FALSE;
}

LRESULT CSpiritMode::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = 0, y = 0;
	static BOOL bDrag = FALSE;
	static int DistX = 0, DistY = 0; // 드래그시 클릭한 곳과 마우스 좌표의 차이 수치.

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))//영역을 벗어남
			return 0;
		else
		{
			bDrag = TRUE;

			DistX = m_pSurfaceBack->Xpos - x;
			DistY = m_pSurfaceBack->Ypos - y;
		}
		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 어디서 땟든 드래그 플래그는 풀어준다.
		bDrag = FALSE;

		if(!IsInside(x, y))//영역을 벗어남
			return 0;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( bDrag ) // 드래그 중이면...
		{
			m_pSurfaceBack->Xpos = x + DistX;
			m_pSurfaceBack->Ypos = y + DistY;

			// 화면 넘어가면 보정.
			if( m_pSurfaceBack->Xpos < 0 )
				m_pSurfaceBack->Xpos = 0;
			if( m_pSurfaceBack->Ypos < 0 )
				m_pSurfaceBack->Ypos = 0;
			if( m_pSurfaceBack->Xpos > g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth() )
				m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth();
			if( m_pSurfaceBack->Ypos > g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
				m_pSurfaceBack->Ypos = g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();
		}
		break;
	}
	return 0;
}
