#include "stdafx.h"
// Happy New Year Event

#include "D3DApp.h"
#include "NKCharacter.h"
#include "PVPWidget.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"
#include "IndexedTexture.h"
#include "BattleStart.h"
#include "main.h"
#include "LoadEffectSound.h"
#include "DirtSnd.h"
#include "TextOutBox.h"
#include "g_stringmanager.h"

extern float g_fTimeKey;

extern int		g_nCurWorldIdx;				// Current World Index ///인던1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPVPWidget::CPVPWidget()
	: bActive(false)
	, m_EnemyLosing(0)
	, m_EnemyWinning(0)
	, m_PlayerLosing(0)
	, m_PlayerWinning(0)
	, m_CurFrame(0)
	, m_fSaveTime(0)
{
	memset(m_FireTexId, 0, sizeof(m_FireTexId));
	memset(m_szEnemyName, 0, sizeof(m_szEnemyName));
	memset(m_szPlayerName, 0, sizeof(m_szPlayerName));

	m_fAlpha = 0.0f;
	m_fDelta1 = 1.0f;
	m_fDelta2 = 1.0f;
	m_EnemyHP = 100;
	m_EnemyHPOld = 100;
	m_PlayerHP = 100;
	m_PlayerHPOld = 100;
	m_ViewWidth = 800;
	m_ViewHeight = 538;


	ZeroMemory(m_szEnemyRecord, sizeof(m_szEnemyRecord));
	ZeroMemory(m_szPlayerRecord, sizeof(m_szPlayerRecord));

	m_rectBack = new D3DTLVERTEX[4];

	m_rectBack[0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectBack[1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectBack[2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectBack[3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectEnemyHP[0] = new D3DTLVERTEX[4];
	m_rectEnemyHP[1] = new D3DTLVERTEX[4];
	m_rectEnemyHP[2] = new D3DTLVERTEX[4];

	m_rectEnemyHP[0][0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectEnemyHP[0][1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectEnemyHP[0][2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectEnemyHP[0][3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectEnemyHP[1][0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectEnemyHP[1][1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectEnemyHP[1][2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectEnemyHP[1][3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectEnemyHP[2][0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectEnemyHP[2][1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectEnemyHP[2][2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectEnemyHP[2][3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectPlayerHP[0] = new D3DTLVERTEX[4];
	m_rectPlayerHP[1] = new D3DTLVERTEX[4];
	m_rectPlayerHP[2] = new D3DTLVERTEX[4];

	m_rectPlayerHP[0][0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectPlayerHP[0][1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectPlayerHP[0][2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectPlayerHP[0][3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectPlayerHP[1][0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectPlayerHP[1][1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectPlayerHP[1][2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectPlayerHP[1][3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_rectPlayerHP[2][0] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rectPlayerHP[2][1] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rectPlayerHP[2][2] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rectPlayerHP[2][3] = D3DTLVERTEX( D3DVECTOR( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	m_pEnemyName = new CTextOutBox();
	m_pPlayerName = new CTextOutBox();

	m_pEnemyRecord = new CTextOutBox();
	m_pPlayerRecord = new CTextOutBox();

	m_BackTexId = 0;

	m_BarSize = 290;
	m_NameWidth = 223-75;
	m_RecordWidth = 363 - 226;

	m_Bar1X = 76, m_Bar2X = 360+76;
	m_Name1X = 75, m_Name2X = 577;
	m_Record1X = 226, m_Record2X = 437;

	m_BarTexId[0] = 0;
	m_BarTexId[1] = 0;


	m_pIndexedTexture = new IndexedTexture( 3 );
	m_pTextureAnimation = NULL;

	if( m_pIndexedTexture )
		m_pIndexedTexture->SetTexturePath("interface/pvp/");
}

CPVPWidget::~CPVPWidget()
{
	DeleteRes();

	SAFE_DELETE(m_pIndexedTexture);
	SAFE_DELETE(m_pTextureAnimation);

	SAFE_DELETE_ARRAY(m_rectBack);
	for(int i=0; i<3; i++)
	{
		SAFE_DELETE_ARRAY(m_rectEnemyHP[i]);
		SAFE_DELETE_ARRAY(m_rectPlayerHP[i]);
	}

	SAFE_DELETE(m_pEnemyName);
	SAFE_DELETE(m_pPlayerName);

	SAFE_DELETE(m_pEnemyRecord);
	SAFE_DELETE(m_pPlayerRecord);

	SAFE_DELETE(m_pIndexedTexture);
}

bool CPVPWidget::IsActive()
{
	return bActive;
}

void CPVPWidget::Activate(bool status)
{
	bActive = status;
}

int  CPVPWidget::Lerp(int x0, int x1, float s)
{
	int k = int((float)x0 * (1.0f-s) + (float)x1 * (s));
	return k;
}

void CPVPWidget::SetPVPStatus(int PlayerHP, int EnemyHP)
{
	if (m_PlayerHP != PlayerHP) //자기자신 에너지 변화
	{
		m_PlayerHPOld = m_PlayerHP;
		m_PlayerHP = PlayerHP;
		m_fDelta1 = 1.0f;
	}

	if (m_EnemyHP != EnemyHP) //적 에너지 변화
	{
		m_EnemyHPOld = m_EnemyHP;
		m_EnemyHP = EnemyHP;
		m_fDelta2 = 1.0f;
	}
}

void CPVPWidget::SetPVPInfo(CNkCharacter *pPlayer, CNkCharacter *pEnemy)
{
	strcpy(m_szPlayerName, pPlayer->m_pstrName);
	strcpy(m_szEnemyName, pEnemy->m_pstrName);
}

void CPVPWidget::SetPVPRecord(int PlayerWin, int PlayerLose,
							  int EnemyWin, int EnemyLose)
{
	sprintf(m_szPlayerRecord, (char*)G_STRING(IDS_LHSTRING1878) , PlayerWin, PlayerLose);
	sprintf(m_szEnemyRecord, (char*)G_STRING(IDS_LHSTRING1878) , EnemyWin, EnemyLose);
}

void CPVPWidget::InitVariables()
{
	D3DVIEWPORT7 vp;

	pCMyApp->GetDevice()->GetViewport( &vp );

	m_fSaveTime = 0;
	m_fAlpha = 0.0f;
	m_fDelta1 = 1.0f;
	m_fDelta2 = 1.0f;
	m_EnemyHP = 100;
	m_EnemyHPOld = 100;
	m_PlayerHP = 100;
	m_PlayerHPOld = 100;
	m_ViewWidth = 800;
	m_ViewHeight = 538;

	AdjustVariables(vp.dwWidth, vp.dwHeight);
}

void CPVPWidget::LoadRes( )
{
	char strTexName[100] = {0,};

	if( !GET_D3DDEVICE() || !m_pIndexedTexture )
		return;

	if( m_pIndexedTexture )
	{
		m_BackTexId = m_pIndexedTexture->CreateTextureFromFile( "back.sek" );
		m_BarTexId[0] = m_pIndexedTexture->CreateTextureFromFile( "bar22.sek" );
		m_BarTexId[1] = m_pIndexedTexture->CreateTextureFromFile( "bar11.sek" );

		m_pIndexedTexture->RestoreAllTextures( GET_D3DDEVICE() );
	}
}

void CPVPWidget::DeleteRes()
{
	m_pIndexedTexture->InvalidateAllTextures();
	m_pEnemyName->DeleteRes();
	m_pPlayerName->DeleteRes();
	m_pEnemyRecord->DeleteRes();
	m_pPlayerRecord->DeleteRes();
}

void CPVPWidget::AdjustVariables(int width, int height)
{
	m_ViewWidth = width;
	m_ViewHeight = height;

	if (m_ViewWidth == 800)
	{
		//Reinitialize
		m_BarSize = 290;
		m_NameWidth = 223-75;
		m_RecordWidth = 363 - 226;

		m_Bar1X = 76, m_Bar2X = 360+76;
		m_Name1X = 75, m_Name2X = 577;
		m_Record1X = 226, m_Record2X = 437;
	}
	else
	{
		float coeff = (float)m_ViewWidth / 800.f;
		m_BarSize = 290*coeff;
		m_NameWidth = (223-75)*coeff;
		m_RecordWidth = (363 - 226)*coeff;

		m_Bar1X = 76*coeff, m_Bar2X = (360+76)*coeff;
		m_Name1X = 75*coeff, m_Name2X = 577*coeff;
		m_Record1X = 226*coeff, m_Record2X = 437*coeff;
	}

	//텍스트 표면 재생성
	m_pEnemyName->Init(14, RGB(0, 0, 0), m_Name2X, 36, m_NameWidth, 51, TRUE);
	m_pPlayerName->Init(14, RGB(0, 0, 0), 75, 36, m_NameWidth, 51, TRUE);

	m_pEnemyName->SetString_Center_Arrange(m_szEnemyName, RGB(152, 212, 4), RGB( 97, 249, 254), TRUE);
	m_pPlayerName->SetString_Center_Arrange(m_szPlayerName, RGB(152, 212, 4), RGB( 97, 249, 254), TRUE);

	m_pEnemyRecord->Init(14, RGB(0, 0, 0), m_Record2X, 36, m_RecordWidth, 51, TRUE);
	m_pPlayerRecord->Init(14, RGB(0, 0, 0), m_Record1X, 36, m_RecordWidth, 51, TRUE);

	m_pEnemyRecord->SetString_Center_Arrange(m_szEnemyRecord, RGB(152, 212, 4), RGB( 97, 249, 254), TRUE);
	m_pPlayerRecord->SetString_Center_Arrange(m_szPlayerRecord, RGB(152, 212, 4), RGB( 97, 249, 254), TRUE);

}

void CPVPWidget::Draw()
{
	int i = 0;
	if( !GET_D3DDEVICE() )
		return;

	float alpha = 0.0f;

	m_fSaveTime += g_fTimeKey;

	if (m_fSaveTime > 0.1f)
	{
		if (m_fAlpha + 0.08 < 1.0f)
		{
			m_fAlpha += 0.08f;
		}
		else m_fAlpha = 1.0f;

		if (m_fDelta1 - 0.1f > 0)
		{
			m_fDelta1 -= 0.1f;
		}
		else
		{
			m_fDelta1 = 0.0f;
			m_PlayerHPOld = m_PlayerHP;
		}

		if (m_fDelta2 - 0.1f > 0)
		{
			m_fDelta2 -= 0.1f;
		}
		else
		{
			m_fDelta2 = 0.0f;
			m_EnemyHPOld = m_EnemyHP;
		}

		m_fSaveTime = 0;
	}

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

//=============================================================

	m_rectBack[0].sx = 0;
	m_rectBack[1].sx = 0;
	m_rectBack[2].sx = m_ViewWidth;
	m_rectBack[3].sx = m_ViewWidth;

	m_rectBack[1].sy = 0;
	m_rectBack[3].sy = 0;
	m_rectBack[0].sy = 80;
	m_rectBack[2].sy = 80;

	/*
	m_rectEnemyHP[0][0].sx = m_Bar2X;
	m_rectEnemyHP[0][1].sx = m_Bar2X;
	m_rectEnemyHP[0][2].sx = m_Bar2X + m_BarSize;
	m_rectEnemyHP[0][3].sx = m_Bar2X + m_BarSize;
	*/
	m_rectEnemyHP[0][0].sx = m_Bar2X + Lerp((m_BarSize*(100-m_EnemyHPOld)/100),
											(m_BarSize*(100-m_EnemyHP)/100), 1-m_fDelta2);

	m_rectEnemyHP[0][1].sx = m_Bar2X + Lerp((m_BarSize*(100-m_EnemyHPOld)/100),
											(m_BarSize*(100-m_EnemyHP)/100), 1-m_fDelta2);

	m_rectEnemyHP[0][2].sx = m_Bar2X + m_BarSize;
	m_rectEnemyHP[0][3].sx = m_Bar2X + m_BarSize;

	m_rectEnemyHP[0][1].sy = 10;
	m_rectEnemyHP[0][3].sy = 10;
	m_rectEnemyHP[0][0].sy = 27;
	m_rectEnemyHP[0][2].sy = 27;

	m_rectEnemyHP[1][0].sx = m_Bar2X + (m_BarSize*(100-m_EnemyHP)/100);
	m_rectEnemyHP[1][1].sx = m_Bar2X + (m_BarSize*(100-m_EnemyHP)/100);
	m_rectEnemyHP[1][2].sx = m_Bar2X + m_BarSize;
	m_rectEnemyHP[1][3].sx = m_Bar2X + m_BarSize;

	m_rectEnemyHP[1][1].sy = 10;
	m_rectEnemyHP[1][3].sy = 10;
	m_rectEnemyHP[1][0].sy = 27;
	m_rectEnemyHP[1][2].sy = 27;

	/*
	m_rectPlayerHP[0][0].sx = m_Bar1X;
	m_rectPlayerHP[0][1].sx = m_Bar1X;
	m_rectPlayerHP[0][2].sx = m_Bar1X + m_BarSize;
	m_rectPlayerHP[0][3].sx = m_Bar1X + m_BarSize;
	*/

	m_rectPlayerHP[0][0].sx = m_Bar1X;
	m_rectPlayerHP[0][1].sx = m_Bar1X;
	m_rectPlayerHP[0][2].sx = m_Bar1X + Lerp( (m_BarSize * m_PlayerHP / 100),
							  (m_BarSize * m_PlayerHPOld / 100), m_fDelta1);
	m_rectPlayerHP[0][3].sx = m_Bar1X + Lerp( (m_BarSize * m_PlayerHP / 100),
							  (m_BarSize * m_PlayerHPOld / 100), m_fDelta1);

	m_rectPlayerHP[0][1].sy = 10;
	m_rectPlayerHP[0][3].sy = 10;
	m_rectPlayerHP[0][0].sy = 27;
	m_rectPlayerHP[0][2].sy = 27;

	m_rectPlayerHP[1][0].sx = m_Bar1X;
	m_rectPlayerHP[1][1].sx = m_Bar1X;
	m_rectPlayerHP[1][2].sx = m_Bar1X + (m_BarSize * m_PlayerHP / 100);
	m_rectPlayerHP[1][3].sx = m_Bar1X + (m_BarSize * m_PlayerHP / 100);

	m_rectPlayerHP[1][1].sy = 10;
	m_rectPlayerHP[1][3].sy = 10;
	m_rectPlayerHP[1][0].sy = 27;
	m_rectPlayerHP[1][2].sy = 27;

	m_rectBack[0].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectBack[1].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectBack[2].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectBack[3].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);

	m_rectEnemyHP[0][0].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectEnemyHP[0][1].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectEnemyHP[0][2].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectEnemyHP[0][3].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);

	m_rectEnemyHP[1][0].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectEnemyHP[1][1].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectEnemyHP[1][2].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectEnemyHP[1][3].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);

	m_rectPlayerHP[0][0].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectPlayerHP[0][1].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectPlayerHP[0][2].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectPlayerHP[0][3].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);

	m_rectPlayerHP[1][0].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectPlayerHP[1][1].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectPlayerHP[1][2].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);
	m_rectPlayerHP[1][3].color = D3DRGBA(1.0f, 1.0f, 1.0f, m_fAlpha);

	GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_BackTexId));
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectBack, 4, 0 );

	GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_BarTexId[0]));
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectEnemyHP[0], 4, 0 );

	GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_BarTexId[1]));
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectEnemyHP[1], 4, 0 );

	GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_BarTexId[0]));
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectPlayerHP[0], 4, 0 );

	GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_BarTexId[1]));
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rectPlayerHP[1], 4, 0 );

	//=============================================================

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

	m_pEnemyName->Draw();//DrawText(3);
	m_pPlayerName->Draw();//DrawText(3);

	m_pEnemyRecord->Draw();//DrawText(3);
	m_pPlayerRecord->Draw();//DrawText(3);

	return;
}
