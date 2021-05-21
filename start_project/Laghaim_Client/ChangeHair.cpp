#include "stdafx.h"
#define D3D_OVERLOADS



#include <math.h>
#include <d3d.h>
#include "d3dutil.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "ChangeHair.h"
#include "NkCharacter.h"
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "textoutbox.h"
#include "g_stringmanager.h"





extern int					g_EffectTextr[];
extern BOOL g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChangeHair::CChangeHair()
{
	m_bActive = FALSE;
	if(!g_pDisplay)
	{
		m_bActive = FALSE;
		return;
	}

	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);

	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;

	m_iMainCx  = 305;
	m_iMainCy  = 314;

	m_bg.Init( "change_hair/back" );

	// 	m_iMainX = (rc.right/2)  - (m_iMainCx/2);
	// 	m_iMainY = (rc.bottom/2) - (m_iMainCy/2);

	m_bg.SetPos(( m_ScreenWidth - m_bg.m_w )>>1 , ( m_ScreenHeight - m_bg.m_h )>>1 );

	m_iMainX = m_bg.m_x;
	m_iMainY = m_bg.m_y;

	int sex = g_pRoh->m_Sex;
	int race = g_pRoh->m_Race;

	//==========================================================
	// nate 2005-01-27
	// 하이브리더 헤어 추가
	char tmp[80];
	for(int i=0; i<NUM_HAIR_ICON; i++)
	{
		if(sex == 0)
		{
			// 남성일경우
			if(race == RACE_BULKAN) // 불칸
				sprintf(tmp, "interface/change_hair/bm_%d.bmp", i+1);
			else if(race == RACE_KAILIPTON) // 카이립톤
				sprintf(tmp, "interface/change_hair/km_%d.bmp", i+1);
			else if(race == RACE_AIDIA) // 에이디아
				sprintf(tmp, "interface/change_hair/am_%d.bmp", i+1);
			else if(race == RACE_HUMAN) // 휴먼
				sprintf(tmp, "interface/change_hair/hm_%d.bmp", i+1);
			else if( race == RACE_FREAK) // 하이브리더
				sprintf( tmp, "interface/change_hair/fm_%d.bmp", i + 1 );
			else if( race == RACE_PEROM )
				sprintf( tmp, "interface/change_hair/pm_%d.bmp", i + 1 );
		}
		else if(sex == 1)
		{
			// 여성일경우
			if(race == RACE_BULKAN) // 불칸
				sprintf(tmp, "interface/change_hair/bw_%d.bmp", i+1);
			else if(race == RACE_KAILIPTON) // 카이
				sprintf(tmp, "interface/change_hair/kw_%d.bmp", i+1);
			else if(race == RACE_AIDIA) // 에이디아
				sprintf(tmp, "interface/change_hair/aw_%d.bmp", i+1);
			else if(race == RACE_HUMAN) // 휴먼
				sprintf(tmp, "interface/change_hair/hw_%d.bmp", i+1);
			else if(race == RACE_FREAK) // 하이브리더
				sprintf(tmp, "interface/change_hair/fw_%d.bmp", i+1);
			else if(race == RACE_PEROM)
				sprintf(tmp, "interface/change_hair/pw_%d.bmp", i+1);
		}

		m_pHairIcon[i] = NULL;
		g_pDisplay->CreateSurfaceFromBitmap(&m_pHairIcon[i], tmp);
		if( m_pHairIcon[i] )
		{
			m_pHairIcon[i]->Xpos = m_iMainX + 92;
			m_pHairIcon[i]->Ypos = m_iMainY + 97;
		}
	}
	//==========================================================
	m_pHairBox = NULL;
	m_pHairBox = new CSpinBox();
	if( m_pHairBox )
		m_pHairBox->Init(m_iMainX+80, m_iMainY+225, 140, 22, 14, RGB(255, 255, 255));

	char buf[10][80];
	for( int i = 0 ; i < 10 ; i++ )
		//-- IDS_CRECHA_HAIR_01 : Style %d
		sprintf(buf[i], G_STRING(IDS_CRECHA_HAIR_01), i+1);

	if( m_pHairBox )
	{
		m_pHairBox->SetSize(10);
		m_pHairBox->SetString(0, buf[0]);
		m_pHairBox->SetString(1, buf[1]);
		m_pHairBox->SetString(2, buf[2]);
		m_pHairBox->SetString(3, buf[3]);
		m_pHairBox->SetString(4, buf[4]);
		m_pHairBox->SetString(5, buf[5]);
		m_pHairBox->SetString(6, buf[6]);
		m_pHairBox->SetString(7, buf[7]);
		m_pHairBox->SetString(8, buf[8]);
		m_pHairBox->SetString(9, buf[9]);
		m_pHairBox->LoadRes("common/btn_toright2", "common/btn_toleft2");
	}


	m_pBtn1 = NULL;
	m_pBtn2 = NULL;
	m_pBtn1 = new CBasicButton();
	m_pBtn2 = new CBasicButton();

	if(	m_pBtn1 )
	{
		m_pBtn1->SetFileName("common/btn_ok");
		m_pBtn1->SetDisable( false );
		m_pBtn1->LoadRes();
		m_pBtn1->PosX = m_iMainX+89;
		m_pBtn1->PosY = m_iMainY+264;
	}

	if( m_pBtn2 )
	{
		m_pBtn2->SetFileName("common/btn_cancel");
		m_pBtn2->SetDisable( false );
		m_pBtn2->LoadRes();
		m_pBtn2->PosX = m_iMainX+170;
		m_pBtn2->PosY = m_iMainY+264;
	}

	char a[80] = {0,};
	int  ren;
	int  str_cx;
	m_pText	= NULL;
	m_pText = new CTextOutBox();
	if( m_pText )
	{
		m_pText->Init(12, RGB(255, 210, 0),0, 0);
		//-- IDS_CHANGHAIR_PRICE : 비용 : 100,000 라임
		strcpy(a, G_STRING(IDS_CHANGHAIR_PRICE));
		m_pText->SetString(a);
		m_pText->SetPos(m_iMainX+100, m_iMainY+248);
	}

	ren = strlen(a);
	str_cx = ren*6;
	m_iCurIcon = 0;
	m_bActive = TRUE;
}

CChangeHair::~CChangeHair()
{
	for(int i=0; i<NUM_HAIR_ICON; i++)
		SAFE_DELETE(m_pHairIcon[i]);

	if(m_pHairBox)
		SAFE_DELETE(m_pHairBox);

	SAFE_DELETE(m_pBtn1);
	SAFE_DELETE(m_pBtn2);
	SAFE_DELETE(m_pText);
}


void CChangeHair::Draw()
{
	if(!m_bActive || !g_pDisplay)
		return;

	m_bg.Draw();

	g_pDisplay->Blt(m_pHairIcon[m_iCurIcon]->Xpos, m_pHairIcon[m_iCurIcon]->Ypos, m_pHairIcon[m_iCurIcon]);

	if(m_pHairBox)
		m_pHairBox->Draw();

	if( m_pBtn1 )
		m_pBtn1->Draw();
	if( m_pBtn2 )
		m_pBtn2->Draw();
	if( m_pText && !g_bGlobalMatrix ) ///국가대항전 중엔 라임 안 뺀다.
		m_pText->Draw();
}



LRESULT CChangeHair::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	if(!m_bActive)
		return -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			if( m_pHairBox )
				m_pHairBox->MsgProc( hWnd, msg, wParam, lParam );
			if( m_pBtn1 )
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn2 )
				m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			return 4;
		}
		break;
	case WM_LBUTTONUP:
		if(IsInside(x, y))
		{
			if (m_pHairBox
					&& m_pHairBox->MsgProc( hWnd, msg, wParam, lParam ) >= 0)
				m_iCurIcon = m_pHairBox->m_CurSel;

			if( m_pBtn1 )
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn2 )
				m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn1 && m_pBtn1->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn1->m_iBtnFlg = BTN_NORMAL;
				return 1;
			}
			if(m_pBtn2 && m_pBtn2->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn2->m_iBtnFlg = BTN_NORMAL;
				return 2;
			}
		}
		break;
	case WM_MOUSEMOVE:
		if( m_pBtn1 )
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtn2 )
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

BOOL CChangeHair::IsInside(int x, int y)
{
	return m_bg.IsIn( x, y);
}