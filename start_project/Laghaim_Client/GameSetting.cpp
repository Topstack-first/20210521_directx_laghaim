#include "stdafx.h"
// system include
#include <windows.h>
#include <stdio.h>

// source include
#include "GameSetting.h"
#include "ControlGroup.h"
#include "CheckButton.h"
#include "Slider.h"
#include "NkCharacter.h"

#include "d3dutil.h"
#include "headers.h"

// extern Include
#include "main.h"
#include "Nk2DFrame.h"
#include "DirtSnd.h"

// debugging use include

extern CMyD3DApplication	*pCMyApp;
extern CDisplay				*g_pDisplay;
extern CNk2DFrame			*g_pNk2DFrame;
extern CDirectSound			*g_pDSound;
extern BOOL					g_bUse3DSound;
extern CNkCharacter			*g_pRoh;

extern int  g_MatrixSvr;
extern DWORD g_dwBaseTime;
extern BOOL	 g_bFFullMode;


// static member function /////////
CGamesetting* CGamesetting::Create()
{
	return new CGamesetting;
}

// general member /////////////////
CGamesetting::CGamesetting()
{
	m_ScreenHeight = 0;
	m_ScreenWidth  = 0;

	m_nPosX	= 0;
	m_nPosY = 0;

	m_pBackSurface = NULL;

	m_GShadow	=	NULL; // 그림자 표현 ---> Checkbutton Count : 2(4)
	m_GWeather	=	NULL; // 날씨 효과   ---> Checkbutton Count : 2(4)
	m_GSMode	=	NULL; // 화면 모드   ---> Checkbutton Count : 2(4)
	m_GCBit		=	NULL; // 컬러 비트   ---> Checkbutton Count : 2(4)
	m_GResol	=	NULL; // 해상도	     ---> Checkbutton Count : 3(6)

	m_p3DSound	=	NULL; // Use 3D Sound				---> Checkbutton Count : 1(2)
	m_pEListen	=	NULL; // Effect Sound Listen		---> Checkbutton Count : 1(2)
	m_pBListen	=	NULL; // Background sound Listen	---> Checkbutton Count : 1(2)
	m_pZListen  =	NULL;

	m_pESlider	=	NULL;
	m_pBSlider	=	NULL;


	if(!CreateInstance())	return;

	Init();
}

CGamesetting::~CGamesetting()
{
	DeleteResource();

	SAFE_DELETE(m_GShadow);
	SAFE_DELETE(m_GWeather);
	SAFE_DELETE(m_GSMode);
	SAFE_DELETE(m_GCBit);
	SAFE_DELETE(m_GResol);

	SAFE_DELETE_ARRAY(m_p3DSound);
	SAFE_DELETE_ARRAY(m_pEListen);
	SAFE_DELETE_ARRAY(m_pBListen);
	SAFE_DELETE_ARRAY(m_pZListen);
	SAFE_DELETE_ARRAY(m_pESlider );
	SAFE_DELETE_ARRAY(m_pBSlider );

}

BOOL CGamesetting::IsInstance()
{

	if(!m_GShadow || !m_GWeather || !m_GSMode   || !m_GCBit ||
			!m_GResol  || !m_p3DSound || !m_pEListen || !m_pBListen || !m_pZListen || !m_pESlider ||
			!m_pBSlider)
		return FALSE;

	return TRUE;
}

BOOL CGamesetting::CreateInstance()
{
	m_GShadow  = CCheckGroup::Create(2, "shadow" );
	m_GWeather = CCheckGroup::Create(2, "weather");
	m_GSMode   = CCheckGroup::Create(2, "screen" );
	m_GCBit    = CCheckGroup::Create(2, "bit"    );
	m_GResol   = CCheckGroup::Create(3, "resolution");

	m_pESlider  = CSliderButton::Create(1);
	m_pBSlider  = CSliderButton::Create(1);

	m_p3DSound = CCheckbutton::Create(1);
	m_pEListen = CCheckbutton::Create(1);
	m_pBListen = CCheckbutton::Create(1);
	m_pZListen = CCheckbutton::Create(1);

	return IsInstance();
}

void CGamesetting::Init()
{
	if(!IsInstance())
		return;

	// circle
	m_GShadow->SetFileName("test/check");
	m_GWeather->SetFileName("test/check");
	m_GSMode->SetFileName("test/check");
	m_GCBit->SetFileName("test/check");
	m_GResol->SetFileName("test/check");

	// slider
	m_pESlider->SetFileName("test/vol_con");
	m_pBSlider->SetFileName("test/vol_con");

	// rectangle
	m_p3DSound->SetFileName("test/check");
	m_pEListen->SetFileName("test/check");
	m_pBListen->SetFileName("test/check");
	m_pZListen->SetFileName("test/check");

	// button
	m_btCancel.SetFileName("test/btn_e_cancle");
	m_btRefresh.SetFileName("test/btn_e_save");
	m_btClose.SetFileName("test/G_n_btn");

}

void CGamesetting::GameSetLoad()
{
	m_GShadow->SetBtSet((int)pCMyApp->m_ProOption.shadow, true);
	m_GWeather->SetBtSet((int)pCMyApp->m_ProOption.bWeather, true);
	m_GSMode->SetBtSet((int)pCMyApp->m_ProOption.bWindowed, true);

	int nTemp = 0;
	if(pCMyApp->m_ProOption.nBackBufferBit == 32) // 32비트
		nTemp = 0;
	else
		nTemp = 1;

	m_GCBit->SetBtSet(nTemp, true);
	m_GResol->SetBtSet((int)pCMyApp->m_ProOption.resolution, true);

	m_pESlider->SetStatus(false);
	m_pBSlider->SetStatus(false);

	m_pESlider->SetNowLevelPer(pCMyApp->m_ProOption.nMusicEffLevel);
	m_pBSlider->SetNowLevelPer(pCMyApp->m_ProOption.nMusicLevel);

	m_p3DSound->SetStatus(g_bUse3DSound);
	m_pEListen->SetStatus(pCMyApp->m_ProOption.bMusicEffOn);
	m_pBListen->SetStatus(pCMyApp->m_ProOption.bMusicOn);
	m_pZListen->SetStatus(pCMyApp->m_ProOption.bUseMusicForEachZone);
}

void CGamesetting::GameSetSave()
{

	pCMyApp->m_ProOption.resolution = (enum RESOLUTION)m_GResol->GetBtSet();
	pCMyApp->m_ProOption.shadow = (enum SHADOW_OPTION)m_GShadow->GetBtSet();
	pCMyApp->m_ProOption.bWeather = (BOOL)m_GWeather->GetBtSet();
	pCMyApp->m_ProOption.bMusicOn = m_pBListen->GetEnable();
	pCMyApp->m_ProOption.bMusicEffOn = m_pEListen->GetEnable();
	pCMyApp->m_ProOption.nMusicEffLevel =  m_pESlider->GetNowLevel();
	pCMyApp->m_ProOption.nMusicLevel = m_pBSlider->GetNowLevel();

	pCMyApp->m_ProOption.bWindowed = m_GSMode->GetBtSet();


	if(pCMyApp->m_ProOption.bWindowed)
		g_bFFullMode = FALSE;
	else
		g_bFFullMode = TRUE;

	if(m_GCBit->GetBtSet() == 0)
		pCMyApp->m_ProOption.nBackBufferBit = 32;
	else
		pCMyApp->m_ProOption.nBackBufferBit = 16;

	pCMyApp->m_ProOption.bUseMusicForEachZone = m_pZListen->GetEnable();

	FILE *fp = fopen("config.ini", "wt");
	if (fp)
	{
		fprintf(fp, "# 건들면 책임 못 집니다..\n");
		fprintf(fp, "# 만약 건드려서 잘 안된다 싶으면, 이 화일을 지우세요.\n");
		fprintf(fp, "Resolution %d\n", m_GResol->GetBtSet());
		fprintf(fp, "Shadow %d\n", m_GShadow->GetBtSet());
		fprintf(fp, "Weather %d\n", m_GWeather->GetBtSet());
		fprintf(fp, "MusicOn %d\n", (int)m_pBListen->GetEnable());
		fprintf(fp, "MusicEffectOn %d\n", m_pEListen->GetEnable());
		fprintf(fp, "MusicLevel %d\n", m_pBSlider->GetNowLevel());
		fprintf(fp, "MusicEffectLevel %d\n", m_pESlider->GetNowLevel());
		fprintf(fp, "Windowed %d\n", (int)m_GSMode->GetBtSet());
		fprintf(fp, "ColorBit %d\n", m_GCBit->GetBtSet());
		fprintf(fp, "ZoneMusic %d\n", (int)m_pZListen->GetEnable());
		fclose(fp);
	}

	fp = fopen("3dsound.ini", "wt");
	if (fp)
	{
		if (m_p3DSound->GetEnable())
		{
			fprintf(fp, "1\n");
		}
		else
		{
			fprintf(fp, "0\n");
		}
		fprintf(fp, "0\n");
		fclose(fp);
	}
}

void CGamesetting::IsScreenModeChange(unsigned char* mode)
{
	unsigned int nbit = 1;
	unsigned int nBBit = 0;

	// 0000 0000 : 변화없음
	// 0000 0001 : 화면모드
	// 0000 0010 : 컬러비트
	// 0000 0100 : 해상도
	if(pCMyApp->m_ProOption.bWindowed != m_GSMode->GetBtSet())
	{
		*mode |= 0x01;	// fullscreen <---> window
		//g_bFFullMode = FALSE;
	}

	if(m_GCBit->GetBtSet() == 0)
		nBBit = 32;
	else if(m_GCBit->GetBtSet() == 1)
		nBBit = 16;

	if(pCMyApp->m_ProOption.nBackBufferBit != nBBit)
	{
		*mode |= 0x02;	// 백버퍼 비트 변경
	}

	if(pCMyApp->m_ProOption.resolution != (enum RESOLUTION)m_GResol->GetBtSet())
	{
		*mode |= 0x04;	// 해상도 변경
	}

	if(pCMyApp->m_ProOption.shadow != (enum SHADOW_OPTION)m_GShadow->GetBtSet())
	{
		pCMyApp->m_ProOption.shadow = (enum SHADOW_OPTION)m_GShadow->GetBtSet();

		if(pCMyApp->m_ProOption.shadow == SHADOW_NONE)
			g_pRoh->SetShadow(FALSE);
		else
			g_pRoh->SetShadow(TRUE);
	}

	if(pCMyApp->m_ProOption.bWeather != (BOOL)m_GWeather->GetBtSet())
	{
		pCMyApp->m_ProOption.bWeather = (BOOL)m_GWeather->GetBtSet();
	}

	if(pCMyApp->m_ProOption.bUseMusicForEachZone != m_pZListen->GetEnable())
	{
		pCMyApp->m_ProOption.bMusicOn = m_pBListen->GetEnable();
		pCMyApp->m_ProOption.nMusicLevel = m_pBSlider->GetNowLevel();
		pCMyApp->m_ProOption.bUseMusicForEachZone = m_pZListen->GetEnable();

		pCMyApp->SetZoneMusic_music();
	}

	if((pCMyApp->m_ProOption.bMusicEffOn != m_pEListen->GetEnable()))
	{
		g_pDSound->GetVolFromConfigFile(m_pEListen->GetEnable(), m_pESlider->GetNowLevel());
		g_pDSound->SetAllVolume();
	}

	if(pCMyApp->m_ProOption.nMusicEffLevel != m_pESlider->GetNowLevel())
	{
		g_pDSound->GetVolFromConfigFile(m_pEListen->GetEnable(), m_pESlider->GetNowLevel());
		g_pDSound->SetAllVolume();
	}

	if(pCMyApp->m_ProOption.nMusicLevel != m_pBSlider->GetNowLevel())
	{
		pCMyApp->SetBackMp3_music(m_pBListen->GetEnable(), m_pBSlider->GetNowLevel());
	}

	if(pCMyApp->m_ProOption.bMusicOn != m_pBListen->GetEnable())
	{
		pCMyApp->m_ProOption.bMusicOn = m_pBListen->GetEnable();

		pCMyApp->SetBackMp3_music(pCMyApp->m_ProOption.bMusicOn, m_pBSlider->GetNowLevel());
	}
}

void CGamesetting::LoadRes()
{
	if(!IsInstance() || !g_pDisplay)
		return;

	if( g_pNk2DFrame )
	{
		m_ScreenWidth  = g_pNk2DFrame->GetClientWidth();
		m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SAFE_DELETE(m_pBackSurface);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pBackSurface, "interface/test/option_bk.bmp");

	if(m_pBackSurface)
	{
		m_pBackSurface->SetWidth( 300 );
		m_pBackSurface->SetHeight( 450 );

		int n_x = (m_ScreenWidth  / 2) - (m_pBackSurface->GetWidth()  / 2);
		int n_y = (m_ScreenHeight / 2) - (m_pBackSurface->GetHeight() / 2);

		m_pBackSurface->Xpos = n_x;
		m_pBackSurface->Ypos = n_y;

		m_nPosX = n_x;
		m_nPosY = n_y;

		// circle
		m_GShadow->LoadRes();
		m_GWeather->LoadRes();
		m_GSMode->LoadRes();
		m_GCBit->LoadRes();
		m_GResol->LoadRes();

		// slider
		m_pESlider->LoadRes();
		m_pBSlider->LoadRes();

		// rectangle
		m_p3DSound->LoadRes();
		m_pEListen->LoadRes();
		m_pBListen->LoadRes();
		m_pZListen->LoadRes();

		// button
		m_btRefresh.LoadRes();
		m_btCancel.LoadRes();
		m_btClose.LoadRes();

		SetBtnPosition();


//		GameSetLoad();
	}
}

void CGamesetting::SetBtnPosition()
{
	BTPS b_shadow_p[2]  = { BTPS(m_nPosX + 109, m_nPosY + 112), BTPS(m_nPosX + 191, m_nPosY + 112 )};
	BTPS b_weather_p[2] = { BTPS(m_nPosX + 109, m_nPosY + 140), BTPS(m_nPosX + 191, m_nPosY + 140 )};
	BTPS b_screen_p[2]  = { BTPS(m_nPosX + 109, m_nPosY + 168), BTPS(m_nPosX + 191, m_nPosY + 168 )};
	BTPS b_bit_p[2]		= { BTPS(m_nPosX + 109, m_nPosY + 196), BTPS(m_nPosX + 191, m_nPosY + 196 )};
	BTPS b_resol_p[3]	= { BTPS(m_nPosX + 21 , m_nPosY + 254), BTPS(m_nPosX + 109, m_nPosY + 254), BTPS(m_nPosX + 195, m_nPosY + 254)};

	BTPS b_sound_p = BTPS(m_nPosX + 232, m_nPosY + 308);
	BTPS b_backsound_p = BTPS(m_nPosX + 232, m_nPosY + 362);

	m_GShadow->SetbuttonPos(b_shadow_p  , m_nPosX +100, m_nPosY + 108 , 180, 24);
	m_GWeather->SetbuttonPos(b_weather_p, m_nPosX +100, m_nPosY + 138 , 180, 24);
	m_GSMode->SetbuttonPos(b_screen_p   , m_nPosX +100, m_nPosY + 166 , 180, 24);
	m_GCBit->SetbuttonPos(b_bit_p       , m_nPosX +100, m_nPosY + 194 , 180, 24);
	m_GResol->SetbuttonPos(b_resol_p    , m_nPosX +15 , m_nPosY + 250 , 266, 23);

	m_pEListen->SetPosition(m_nPosX + 232, m_nPosY + 308);
	m_pBListen->SetPosition(m_nPosX + 232, m_nPosY + 362);

	m_p3DSound->SetPosition(m_nPosX + 34 , m_nPosY + 383);
	m_pZListen->SetPosition(m_nPosX + 168, m_nPosY + 383);

	m_pESlider->SetPosition(m_nPosX + 72, m_nPosY + 310); // 364
	m_pESlider->SetPositionSlider(m_nPosX + 71, m_nPosY + 312);
	m_pESlider->SetPositionPM(m_nPosX + 213, m_nPosY + 311, m_nPosX + 59, m_nPosY + 311);
	// 슬라이더 버튼이 아닌 슬라이더는 배경 박혀있는 관계로
	// 좌표두 소스에 박혀있다.
	// 모듈화 시킬라믄 다시 약간의 수정이 필요함.

	m_pBSlider->SetPosition(m_nPosX + 72, m_nPosY + 364); // 364
	m_pBSlider->SetPositionSlider(m_nPosX + 71, m_nPosY + 366);
	m_pBSlider->SetPositionPM(m_nPosX + 213, m_nPosY + 365, m_nPosX + 59, m_nPosY + 365);
	// 슬라이더 버튼이 아닌 슬라이더는 배경 박혀있는 관계로
	// 좌표두 소스에 박혀있다.

	m_btCancel.SetPosition(m_nPosX  + 165, m_nPosY + 413);
	m_btClose.SetPosition(m_nPosX   + 276, m_nPosY + 12);
	m_btRefresh.SetPosition(m_nPosX + 34 , m_nPosY + 413);


}

void CGamesetting::DeleteResource()
{
	if(m_GShadow)  m_GShadow->DeleteRes();
	if(m_GWeather) m_GWeather->DeleteRes();
	if(m_GSMode)   m_GSMode->DeleteRes();
	if(m_GCBit)	   m_GCBit->DeleteRes();
	if(m_GResol)   m_GResol->DeleteRes();

	if(m_pESlider)	m_pESlider->DeleteRes();
	if(m_pBSlider)	m_pBSlider->DeleteRes();

	if(m_p3DSound) m_p3DSound->DeleteRes();
	if(m_pEListen) m_pEListen->DeleteRes();
	if(m_pBListen) m_pBListen->DeleteRes();
	if(m_pZListen) m_pZListen->DeleteRes();

	m_btClose.DeleteRes();
	m_btCancel.DeleteRes();
	m_btRefresh.DeleteRes();

	SAFE_DELETE(m_pBackSurface);
}

void CGamesetting::ButtonDraw()
{
	m_GShadow->DrawToBlt(true);
	m_GWeather->DrawToBlt(true);
	m_GSMode->DrawToBlt(true);
	m_GCBit->DrawToBlt(true);
	m_GResol->DrawToBlt(true);

	if(m_pEListen->GetEnable())
		m_pESlider->DrawToBlt(true);

	if(m_pBListen->GetEnable())
		m_pBSlider->DrawToBlt(true);

	m_p3DSound->DrawToBlt(true);
	m_pEListen->DrawToBlt(true);
	m_pBListen->DrawToBlt(true);
	m_pZListen->DrawToBlt(true);

	m_btCancel.DrawToBlt();
	m_btClose.DrawToBlt();
	m_btRefresh.DrawToBlt();
}

int CGamesetting::Draw()
{
	if (g_pDisplay && m_pBackSurface)
	{
		if(!IsInstance()) return -1;

		if(g_pDisplay->Blt(m_pBackSurface->Xpos, m_pBackSurface->Ypos, m_pBackSurface) == DD_OK)
		{
			ButtonDraw();
		}
	}
	return -1;
}

BOOL CGamesetting::IsInside(int x, int y)
{
	if (x >= m_pBackSurface->Xpos
			&& x < m_pBackSurface->Xpos + m_pBackSurface->GetWidth()
			&& y >= m_pBackSurface->Ypos
			&& y < m_pBackSurface->Ypos + m_pBackSurface->GetHeight())
		return TRUE;

	return false;
}

BOOL bMove = FALSE;

void CGamesetting::move_window(UINT msg, int x, int y)
{
	static BOOL bFlag = FALSE;

	if(msg == WM_LBUTTONDOWN)
	{
		// 클릭 좌표
		int nx = x;
		int ny = y;
		if (nx > m_pBackSurface->Xpos
				&& nx < m_pBackSurface->Xpos + m_pBackSurface->GetWidth()
				&& ny > m_pBackSurface->Ypos
				&& ny < m_pBackSurface->Ypos + 30)
		{
			bMove = TRUE;
		}

	}


	if(msg == WM_LBUTTONUP)
	{
		bMove = FALSE;
		bFlag = FALSE;
		int nGapX = (int)m_nPosX%5;
		int nGapY = (int)m_nPosY%5;
		m_pBackSurface->Xpos -= (float)nGapX;
		m_pBackSurface->Ypos -= (float)nGapY;

		m_nPosX = m_pBackSurface->Xpos;
		m_nPosY = m_pBackSurface->Ypos;

		SetBtnPosition();
	}

	static int n_x = x;
	static int n_y = y;

	if(bMove && msg == WM_MOUSEMOVE)
	{
		if(!bFlag)
		{
			n_x = x;
			n_y = y;
			bFlag = TRUE;
		}

		int cx = x;
		int cy = y;

		m_pBackSurface->Xpos += cx - n_x;
		m_pBackSurface->Ypos += cy - n_y;

		int w = 0, h = 0;

		if(pCMyApp->m_ProOption.resolution == R640)
		{
			w = 640;
			h = 480;
		}
		else if(pCMyApp->m_ProOption.resolution == R800)
		{
			w = 800;
			h = 600;
		}
		else if(pCMyApp->m_ProOption.resolution == R1024)
		{
			w = 1024;
			h = 768;
		}

		if(m_pBackSurface->Xpos < 0)
			m_pBackSurface->Xpos = 0;
		if(m_pBackSurface->Xpos > (w - m_pBackSurface->GetWidth()))
			m_pBackSurface->Xpos = (w - m_pBackSurface->GetWidth());

		if(m_pBackSurface->Ypos < 0)
			m_pBackSurface->Ypos = 0;
		if(m_pBackSurface->Ypos > (h - m_pBackSurface->GetHeight()))
			m_pBackSurface->Ypos = (h - m_pBackSurface->GetHeight());

		m_nPosX = m_pBackSurface->Xpos;
		m_nPosY = m_pBackSurface->Ypos;

		n_x = cx;
		n_y = cy;

		SetBtnPosition();
	}
}

LRESULT CGamesetting::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	move_window(msg, LOWORD(lParam), HIWORD(lParam));

	if(!IsInside(LOWORD(lParam), HIWORD(lParam)) || !IsInstance())
		return 0;

	switch(msg)
	{
	case WM_LBUTTONDOWN :
		m_btClose.MsgProc(hWnd, msg, wParam, lParam);
		m_btRefresh.MsgProc(hWnd, msg, wParam, lParam);
		m_btCancel.MsgProc(hWnd, msg, wParam, lParam);
		break;
	case WM_LBUTTONUP   :
		m_btClose.MsgProc(hWnd, msg, wParam, lParam);
		if(m_btClose.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowGameSetWindow(false);
		}
		m_btRefresh.MsgProc(hWnd, msg, wParam, lParam);
		if(m_btRefresh.GetState() == BTN_ACTION)
		{
			int mode_change = -1;

			unsigned char bitflag = 0;

			IsScreenModeChange(&bitflag);

			GameSetSave();

			g_pNk2DFrame->ShowGameSetWindow(false);

			if( (unsigned char)bitflag & 0x0f )
			{
				SendMessage(hWnd, WM_MYSMODECHANGE,(WPARAM)bitflag, lParam);
			}
		}
		m_btCancel.MsgProc(hWnd, msg, wParam, lParam);
		if(m_btCancel.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowGameSetWindow(false);
		}

		break;
	case WM_MOUSEMOVE   :
		break;
	}


	m_GShadow->MsgProc(hWnd, msg, wParam, lParam);
	m_GWeather->MsgProc(hWnd, msg, wParam, lParam);
	m_GSMode->MsgProc(hWnd, msg, wParam, lParam);
	m_GCBit->MsgProc(hWnd, msg, wParam, lParam);
	m_GResol->MsgProc(hWnd, msg, wParam, lParam);

	if(m_pEListen->GetEnable())
		m_pESlider->MsgProc(hWnd, msg, wParam, lParam);

	if(m_pBListen->GetEnable())
		m_pBSlider->MsgProc(hWnd, msg, wParam, lParam);

	m_p3DSound->MsgProc(hWnd, msg, wParam, lParam);
	m_pEListen->MsgProc(hWnd, msg, wParam, lParam);
	m_pBListen->MsgProc(hWnd, msg, wParam, lParam);
	m_pZListen->MsgProc(hWnd, msg, wParam, lParam);

	m_btClose.MsgProc(hWnd, msg, wParam, lParam);
	m_btRefresh.MsgProc(hWnd, msg, wParam, lParam);
	m_btCancel.MsgProc(hWnd, msg, wParam, lParam);

	return -1;
}
