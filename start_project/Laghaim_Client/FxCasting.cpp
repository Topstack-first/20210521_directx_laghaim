#include "stdafx.h"
#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "main.h"
#include "DirtSnd.h"
#include "FxCasting.h"
#include "skill.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxCasting::CFxCasting()
{
	m_rectCasting = NULL;
	m_rectCasting = new D3DTLVERTEX[4];

	m_rectCasting[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectCasting[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectCasting[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectCasting[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	for( int i = 0 ; i < 5 ; i ++ )
		m_CastingTexId[i] = 0;

	m_CurFrame = 1;
	m_nTexNum = 0;
}

CFxCasting::~CFxCasting()
{
	DeleteRes();
}

void CFxCasting::DeleteRes()
{
	SAFE_DELETE(m_rectCasting);
}

void CFxCasting::Render( DWORD dwCurtime)
{
	if(!g_pCapsyongTexture || !GET_D3DDEVICE() )
		return;

	m_rectCasting[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectCasting[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.1f );
	m_rectCasting[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectCasting[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.1f );
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

	float alpha = 1.0f;
	int nLevelGrade	= g_ChaSkill.m_SkillLevel[26];
	int nHellMas_Level = g_ChaSkill.m_SkillLevel[25];	//헬 마스터리 레벨
	int nGrade_Hellmas1 = 3000 - ( 30 * nHellMas_Level * 2.5);	//마스터리가 적용된 시간
	int nGrade_Hellmas2 = 6000 - ( 60 * nHellMas_Level * 2.5);
	int nGrade_Hellmas3 = 8000 - ( 80 * nHellMas_Level * 2.5);
	int nGrade_Hellmas4 = 10000 - ( 100 * nHellMas_Level * 2.5);
	int nGrade_Hellmas5 = 12000 - ( 120 * nHellMas_Level * 2.5);

	m_rectCasting[0].sx = (FLOAT)vp.dwWidth/2 - 82;
	m_rectCasting[1].sx = (FLOAT)vp.dwWidth/2 - 82;

	int dwDrawtime = 0;
	if( dwCurtime > 0
			&& dwCurtime <= nGrade_Hellmas1 )
	{
		dwDrawtime = dwCurtime;
		m_rectCasting[2].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( nGrade_Hellmas1 / 100 );
		m_rectCasting[3].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( nGrade_Hellmas1 / 100 );
	}
	else if( dwCurtime > nGrade_Hellmas1
			 && dwCurtime <= nGrade_Hellmas2
			 && nLevelGrade > 6 )
	{
		dwDrawtime = dwCurtime - nGrade_Hellmas1;
		m_rectCasting[2].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas2 - nGrade_Hellmas1 ) / 100 );
		m_rectCasting[3].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas2 - nGrade_Hellmas1 ) / 100 );
	}
	else if( dwCurtime > nGrade_Hellmas2
			 && dwCurtime <= nGrade_Hellmas3
			 && nLevelGrade > 12 )
	{
		dwDrawtime = dwCurtime - nGrade_Hellmas2;
		m_rectCasting[2].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas3 - nGrade_Hellmas2 ) / 100 );
		m_rectCasting[3].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas3 - nGrade_Hellmas2 ) / 100 );
	}
	else if( dwCurtime > nGrade_Hellmas3
			 && dwCurtime <= nGrade_Hellmas4
			 && nLevelGrade > 18 )
	{
		dwDrawtime = dwCurtime - nGrade_Hellmas3;
		m_rectCasting[2].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas4 - nGrade_Hellmas3 ) / 100 );
		m_rectCasting[3].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas4 - nGrade_Hellmas3 ) / 100 );
	}
	else if( dwCurtime > nGrade_Hellmas4
			 && dwCurtime <= nGrade_Hellmas5
			 && nLevelGrade > 24 )
	{
		dwDrawtime = dwCurtime - nGrade_Hellmas4;
		m_rectCasting[2].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas5 - nGrade_Hellmas4 ) / 100 );
		m_rectCasting[3].sx = (FLOAT)vp.dwWidth/2 - 108
							  + ( dwDrawtime * 2 ) / ( ( nGrade_Hellmas5 - nGrade_Hellmas4 ) / 100 );
	}

	m_rectCasting[1].sy = (FLOAT)vp.dwHeight - 100;
	m_rectCasting[3].sy = (FLOAT)vp.dwHeight - 100;
	m_rectCasting[0].sy = (FLOAT)vp.dwHeight - 93;
	m_rectCasting[2].sy = (FLOAT)vp.dwHeight - 93;

	m_rectCasting[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectCasting[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectCasting[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectCasting[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLDOWN + m_nTexNum ] ));
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectCasting, 4, 0 );

	m_rectCasting[0].sx = (FLOAT)vp.dwWidth/2 - 120;
	m_rectCasting[1].sx = (FLOAT)vp.dwWidth/2 - 120;
	m_rectCasting[2].sx = (FLOAT)vp.dwWidth/2 + 130;
	m_rectCasting[3].sx = (FLOAT)vp.dwWidth/2 + 130;

	m_rectCasting[1].sy = (FLOAT)vp.dwHeight - 140;
	m_rectCasting[3].sy = (FLOAT)vp.dwHeight - 140;
	m_rectCasting[0].sy = (FLOAT)vp.dwHeight - 88;
	m_rectCasting[2].sy = (FLOAT)vp.dwHeight - 88;

	m_rectCasting[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectCasting[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectCasting[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);
	m_rectCasting[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, alpha);

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_CASTINGBOX ] ) );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectCasting, 4, 0 );
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