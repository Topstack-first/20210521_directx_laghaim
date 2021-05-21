#include "stdafx.h"
#include "main.h"
#include "headers.h"
#include "optionpg.h"
#include "TcpipCon.h"
#include "g_stringmanager.h"

#define DEFAULT_MUSIC_LEVEL			50
#define DEFAULT_MUSICEFF_LEVEL		50
#define OPTION_PAGE_BACK			"interface/Option/back_option.bmp"
// nate 2004 - 12 : interface - 좌표값 수정
#define _SPINBOX_CX   355
#define _SPINBOX_CY   35
#define _SPIN_FSIZE	  14
#define _SPIN_FCOLOR  (RGB(140, 204, 138))





COptionPg::COptionPg()
	: m_ArrowPos(0)
	, m_CurResolution(0)
	, m_CurShadow(0)
	, m_CurWeather(0)
	, m_CurMusicLevel(0)
	, m_CurMusicEffLevel(0)
	, m_CurMode(0)
{
	m_pSheet = NULL;
	m_pBack = NULL;
	Init();
}

COptionPg::COptionPg( CSheet* pSheet )
	: m_ArrowPos(0)
	, m_CurResolution(0)
	, m_CurShadow(0)
	, m_CurWeather(0)
	, m_CurMusicLevel(0)
	, m_CurMusicEffLevel(0)
{
	m_pSheet = NULL;
	m_pSheet = pSheet;
	m_pBack = NULL;
	m_CurMode = 0;

	Init();
}

COptionPg::~COptionPg()
{
	DeleteRes();
}

void COptionPg::Init()
{
	m_pEffVolume	 = NULL;  //	효과음 막대기 이미지
	m_pMusicVolume	 = NULL;  //	배경음악 막대기 이미지
	m_pDragVolume1	 = NULL;  //	드레그 바 이미지(효과음)
	m_pDragVolume2   = NULL;  //	드레그 바 이미지(배경음악)
	m_pDragVolume1_C = NULL;  //	드레그 바 클릭 이미지(효과음)
	m_pDragVolume2_C = NULL;  //    드레그 바 클릭 이미지 (배경음악)
	m_pGrayVolume1	 = NULL;  //	그레이처리된 볼륨막대기 이미지(효과음)
	m_pGrayVolume2	 = NULL;  //    그레이처리된 볼륨막대기 이미지(배경음악)

	m_bClickVB1   = FALSE; // (효과음)  볼륨바가 클릭된 상태인가??
	m_bClickVB2   = FALSE; // (배경음악)볼륨바가 클릭된 상태인가??
	m_bMusicOn    = FALSE; // (배경음악)"사용"버튼이 클릭된 상태인가??
	m_bMusicEffOn = FALSE; // (효과음)  "사용"버튼이 클릭된 상태인가??

	//enum SHADOW_OPTION {SHADOW_NONE, SHADOW_ALONE, SHADOW_PLAYER, SHADOW_MONSTER} ;
	int aa = 41;
	//==============================================================
	// nate 2004 - 12 : nterface - 좌표값 수정
	m_Shadow.Init(213, 142, _SPINBOX_CX, _SPINBOX_CY, _SPIN_FSIZE, _SPIN_FCOLOR);
	m_Wether.Init(213, 210, _SPINBOX_CX, _SPINBOX_CY, _SPIN_FSIZE, _SPIN_FCOLOR);
	m_Mode.Init(213, 278, _SPINBOX_CX, _SPINBOX_CY, _SPIN_FSIZE, _SPIN_FCOLOR);
	m_Res.Init(213, 346, _SPINBOX_CX, _SPINBOX_CY, _SPIN_FSIZE, _SPIN_FCOLOR);



	m_Shadow.SetSize(2);
	m_Wether.SetSize(2);
	m_Mode.SetSize(2);
	m_Res.SetSize(6);

	m_Shadow.SetString(0, (char*)G_STRING(IDS_OPTION_NONE_EXPRESSION));
	m_Shadow.SetString(1, (char*)G_STRING(IDS_OPTION_PLAYER));

	// 날씨효과 옵션 수정, 런쳐랑 게임 옵션이 반대로 되어있슴
	// 여기서 수정하여 런쳐랑 맞춰줌 따라서 둘다 반대로 되어있슴
	//-- IDS_OPTION_USE : 사        용
	m_Wether.SetString(0, (char*)G_STRING(IDS_OPTION_USE));
	//-- IDS_OPTION_NOT_USE : 사용않함
	m_Wether.SetString(1, (char*)G_STRING(IDS_OPTION_NOT_USE));


	//-- IDS_OPTION_FULL_MODE : 전체화면 모드
	m_Mode.SetString(0, (char*)G_STRING(IDS_OPTION_FULL_MODE));
	//-- IDS_OPTION_WINDOWS_MODE : 창화면 모드
	m_Mode.SetString(1, (char*)G_STRING(IDS_OPTION_WINDOWS_MODE));
		
	m_Res.SetString(0, "800 x 600");
	m_Res.SetString(1, "1024 x 768");
	m_Res.SetString(2, "1280 x 960");
	m_Res.SetString(3, "1280 x 720");
	m_Res.SetString(4, "1600 x 900");
	m_Res.SetString(5, "1920 x 1080");
}


void COptionPg::LoadRes()
{
	if( !g_pDisplay )
		return;

	ReadConfigFile("config.ini");

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBack, OPTION_PAGE_BACK);		// 해상도

	m_Shadow.LoadRes("common/btn_toright", "common/btn_toleft");
	m_Wether.LoadRes("common/btn_toright", "common/btn_toleft");
	m_Mode.LoadRes("common/btn_toright", "common/btn_toleft");
	m_Res.LoadRes("common/btn_toright", "common/btn_toleft");

	g_pDisplay->CreateSurfaceFromBitmap( &m_pEffVolume, "interface/Option/pro_volume.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pMusicVolume, "interface/Option/pro_volume.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pGrayVolume1, "interface/Option/pro_volume_gray.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pGrayVolume2, "interface/Option/pro_volume_gray.bmp");

	g_pDisplay->CreateSurfaceFromBitmap( &m_pDragVolume1, "interface/Option/dragbtn_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDragVolume1_C, "interface/Option/dragbtn_click.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDragVolume2, "interface/Option/dragbtn_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDragVolume2_C, "interface/Option/dragbtn_click.bmp");
	if( m_pEffVolume )		m_pEffVolume->SetColorKey(RGB(0, 0, 0));
	if( m_pMusicVolume )	m_pMusicVolume->SetColorKey(RGB(0, 0, 0));
	if( m_pDragVolume1 )	m_pDragVolume1->SetColorKey(RGB(0, 0, 0));
	if( m_pDragVolume2 )	m_pDragVolume2->SetColorKey(RGB(0, 0, 0));
	if( m_pDragVolume1_C )	m_pDragVolume1_C->SetColorKey(RGB(0, 0, 0));
	if( m_pDragVolume2_C )	m_pDragVolume2_C->SetColorKey(RGB(0, 0, 0));

	if( m_pEffVolume )
	{
		m_pEffVolume->Xpos = 187;
		m_pEffVolume->Ypos = 457;
	}

	if( m_pGrayVolume1 )
	{
		m_pGrayVolume1->Xpos = 187;
		m_pGrayVolume1->Ypos = 457;
	}

	if( m_pMusicVolume )
	{
		m_pMusicVolume->Xpos = 529;
		m_pMusicVolume->Ypos = 457;
	}

	if( m_pGrayVolume2 )
	{
		m_pGrayVolume2->Xpos = 529;
		m_pGrayVolume2->Ypos = 457;
	}

	if( m_pDragVolume1 && m_pEffVolume )
	{
		m_pDragVolume1->Xpos = m_pEffVolume->Xpos+ (m_CurMusicEffLevel * (m_pEffVolume->GetWidth())) / (100) - m_pDragVolume1->GetWidth();
		m_pDragVolume1->Ypos = m_pEffVolume->Ypos+(m_pEffVolume->GetHeight() / 2) -(m_pDragVolume1->GetHeight()/2) +1;
	}
	if( m_pDragVolume2 && m_pMusicVolume )
	{
		m_pDragVolume2->Xpos = m_pMusicVolume->Xpos+(m_CurMusicLevel * m_pMusicVolume->GetWidth()) / (100) -m_pDragVolume1->GetWidth();;
		m_pDragVolume2->Ypos = m_pMusicVolume->Ypos+(m_pMusicVolume->GetHeight() / 2) -(m_pDragVolume2->GetHeight()/2)+2;
	}

	m_EffCheckBtn[0].SetFileName("Option/available");
	m_EffCheckBtn[1].SetFileName("Option/unavailable");
	m_MusicCheckBtn[0].SetFileName("Option/available");
	m_MusicCheckBtn[1].SetFileName("Option/unavailable");

	for(int i =0; i<CHECK_BTN_CNT; i++)
	{
		m_EffCheckBtn[i].SetDisable(false);
		m_EffCheckBtn[i].LoadRes();
		m_MusicCheckBtn[i].SetDisable(false);
		m_MusicCheckBtn[i].LoadRes();

		m_EffCheckBtn[i].PosX = 79;
		m_EffCheckBtn[i].PosY = 451;
		m_MusicCheckBtn[i].PosX = 436;
		m_MusicCheckBtn[i].PosY = 451;
	}

	m_MainBtn.SetFileName("common/btn_toleft2");
	m_MainBtn.LoadRes();
	m_MainBtn.PosX = 336;
	m_MainBtn.PosY = 563;
}


void COptionPg::DeleteRes()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pEffVolume);
	SAFE_DELETE(m_pMusicVolume);
	SAFE_DELETE(m_pGrayVolume1);
	SAFE_DELETE(m_pGrayVolume2);
	SAFE_DELETE(m_pDragVolume1);
	SAFE_DELETE(m_pDragVolume2);
	SAFE_DELETE(m_pDragVolume1_C);
	SAFE_DELETE(m_pDragVolume2_C);
	m_Shadow.DeleteRes();
	m_Wether.DeleteRes();
	m_Mode.DeleteRes();
	m_Res.DeleteRes();
	m_MainBtn.DeleteRes();

	for(int i =0; i<CHECK_BTN_CNT; i++)
	{
		m_EffCheckBtn[i].DeleteRes();
		m_MusicCheckBtn[i].DeleteRes();
	}
}

void COptionPg::Draw()
{
	if( !g_pDisplay || !m_pBack )
		return;

	g_pDisplay->Blt(0, 0, m_pBack);

	m_Shadow.Draw();
	m_Wether.Draw();
	m_Mode.Draw();
	m_Res.Draw();

	m_EffCheckBtn[m_bMusicEffOn].Draw();
	m_MusicCheckBtn[m_bMusicOn].Draw();

	if(	m_pDragVolume2->Xpos > m_pMusicVolume->Xpos)
		DrawMusicVolume();

	if(	m_pDragVolume1->Xpos > m_pEffVolume->Xpos)
		DrawEffVolume();

	DrawVD();
	m_MainBtn.Draw();
}



void COptionPg::DrawVD()
{
	if( !g_pDisplay )
		return;

	if(m_bClickVB1 == TRUE)
		g_pDisplay->Blt(m_pDragVolume1->Xpos,m_pDragVolume1->Ypos-2, m_pDragVolume1_C);
	else
		g_pDisplay->Blt(m_pDragVolume1->Xpos,m_pDragVolume1->Ypos-2, m_pDragVolume1);

	if(m_bClickVB2 == TRUE)
		g_pDisplay->Blt(m_pDragVolume2->Xpos,m_pDragVolume2->Ypos-2, m_pDragVolume2_C);
	else
		g_pDisplay->Blt(m_pDragVolume2->Xpos,m_pDragVolume2->Ypos-2, m_pDragVolume2);
}


void COptionPg::DrawEffVolume()
{
	if( !g_pDisplay || !m_pBack )
		return;

	RECT rc;
	SetRect(&rc, 0, 0, m_pDragVolume1->Xpos - m_pEffVolume->Xpos+m_pDragVolume1->GetWidth(), m_pEffVolume->GetHeight());

	if(m_pDragVolume1->Xpos != m_pEffVolume->Xpos && m_bMusicEffOn == TRUE )
		g_pDisplay->Blt(m_pEffVolume->Xpos , m_pEffVolume->Ypos, m_pEffVolume, &rc);
	else
		g_pDisplay->Blt(m_pEffVolume->Xpos , m_pEffVolume->Ypos, m_pGrayVolume1, &rc);
}

void COptionPg::DrawMusicVolume()
{
	if( !g_pDisplay || !m_pBack )
		return;

	RECT rc;
	SetRect(&rc, 0, 0, m_pDragVolume2->Xpos - m_pMusicVolume->Xpos+m_pDragVolume1->GetWidth(), m_pMusicVolume->GetHeight());

	if(m_pDragVolume2->Xpos != m_pMusicVolume->Xpos && m_bMusicOn == TRUE )
		g_pDisplay->Blt(m_pMusicVolume->Xpos , m_pMusicVolume->Ypos, m_pMusicVolume, &rc);
	else
		g_pDisplay->Blt(m_pMusicVolume->Xpos , m_pMusicVolume->Ypos, m_pGrayVolume2, &rc);
}


void COptionPg::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !g_pDisplay || !m_pBack )
		return;

	int mouse_x = LOWORD (lParam);
	int mouse_y = HIWORD (lParam);


	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		m_EffCheckBtn[m_bMusicEffOn].MsgProc(hWnd, msg, wParam, lParam);
		m_MusicCheckBtn[m_bMusicOn].MsgProc(hWnd, msg, wParam, lParam);
		m_Shadow.MsgProc( hWnd, msg, wParam, lParam );
		m_Wether.MsgProc( hWnd, msg, wParam, lParam );
		m_Mode.MsgProc( hWnd, msg, wParam, lParam );
		m_Res.MsgProc( hWnd, msg, wParam, lParam );
		m_MainBtn.MsgProc(hWnd, msg, wParam, lParam);

		if(IsInside(m_pEffVolume, LOWORD (lParam), HIWORD (lParam)) == TRUE && m_bMusicEffOn ==TRUE)
		{
			if(m_pDragVolume1->Xpos+m_pDragVolume1->GetWidth() < mouse_x)
				m_pDragVolume1->Xpos  = mouse_x - m_pDragVolume1->GetWidth();
			if(m_pDragVolume1->Xpos > mouse_x)
				m_pDragVolume1->Xpos  = mouse_x;
		}

		if(IsInside(m_pMusicVolume, LOWORD (lParam), HIWORD (lParam)) == TRUE && m_bMusicOn ==TRUE)
		{
			if(m_pDragVolume2->Xpos+m_pDragVolume1->GetWidth() < mouse_x)
				m_pDragVolume2->Xpos  = mouse_x - m_pDragVolume2->GetWidth();
			if(m_pDragVolume2->Xpos > mouse_x)
				m_pDragVolume2->Xpos  = mouse_x;
		}

		if(IsInside(m_pDragVolume1, LOWORD (lParam), HIWORD (lParam)) == TRUE)
		{
			m_bClickVB1 = TRUE;
		}

		if(IsInside(m_pDragVolume2, LOWORD (lParam), HIWORD (lParam)) == TRUE)
		{
			m_bClickVB2 = TRUE;
		}
		break;

	case WM_LBUTTONUP:
		m_EffCheckBtn[m_bMusicEffOn].MsgProc(hWnd, msg, wParam, lParam);
		m_MusicCheckBtn[m_bMusicOn].MsgProc(hWnd, msg, wParam, lParam);
		m_MainBtn.MsgProc(hWnd, msg, wParam, lParam);


		if (m_Shadow.MsgProc( hWnd, msg, wParam, lParam ) >= 0)
			m_CurShadow = m_Shadow.m_CurSel;
		if (m_Wether.MsgProc( hWnd, msg, wParam, lParam ) >= 0)
			m_CurWeather = m_Wether.m_CurSel;
		if (m_Mode.MsgProc( hWnd, msg, wParam, lParam ) >= 0)
			m_CurMode = m_Mode.m_CurSel;
		if (m_Res.MsgProc( hWnd, msg, wParam, lParam ) >= 0)
			m_CurResolution = m_Res.m_CurSel+1;

		if(m_bClickVB1 == TRUE)
			m_bClickVB1 = FALSE;
		else if(m_bClickVB2 == TRUE)
			m_bClickVB2 = FALSE;

		else if(m_EffCheckBtn[m_bMusicEffOn].GetState() ==BTN_ACTION)
		{
			m_EffCheckBtn[m_bMusicEffOn].m_iBtnFlg = BTN_NORMAL;

			if(m_bMusicEffOn == FALSE)
				m_bMusicEffOn = TRUE;
			else
				m_bMusicEffOn = FALSE;
		}

		else if(m_MusicCheckBtn[m_bMusicOn].GetState() ==BTN_ACTION)
		{
			m_MusicCheckBtn[m_bMusicOn].m_iBtnFlg = BTN_NORMAL;

			if(m_bMusicOn == FALSE)
				m_bMusicOn = TRUE;
			else
				m_bMusicOn = FALSE;
		}

		if(m_MainBtn.GetState() == BTN_ACTION)
		{
			WriteConfigFile("config.ini");
			if( m_pSheet )
				m_pSheet->GoPage(LOGINPAGE);
			m_MainBtn.m_iBtnFlg = BTN_NORMAL;
		}
		break;

	case WM_MOUSEMOVE:

		m_MainBtn.MsgProc(hWnd, msg, wParam, lParam);

		if(m_bClickVB1 == TRUE && m_bMusicEffOn== TRUE)
		{
			m_pDragVolume1->Xpos    = mouse_x - m_pDragVolume1->GetWidth()/2;
			if(m_pDragVolume1->Xpos+m_pDragVolume1->GetWidth() > m_pEffVolume->Xpos+m_pEffVolume->GetWidth())
				m_pDragVolume1->Xpos = m_pEffVolume->Xpos+m_pEffVolume->GetWidth()-m_pDragVolume1->GetWidth();
			if(m_pDragVolume1->Xpos < m_pEffVolume->Xpos)
				m_pDragVolume1->Xpos  = m_pEffVolume->Xpos;
		}


		if(m_bClickVB2 == TRUE && m_bMusicOn== TRUE)
		{
			m_pDragVolume2->Xpos    = mouse_x - m_pDragVolume2->GetWidth()/2;
			if(m_pDragVolume2->Xpos+m_pDragVolume1->GetWidth() > m_pMusicVolume->Xpos+m_pMusicVolume->GetWidth())
				m_pDragVolume2->Xpos = m_pMusicVolume->Xpos+m_pMusicVolume->GetWidth()-m_pDragVolume2->GetWidth();
			if(m_pDragVolume2->Xpos < m_pMusicVolume->Xpos)
				m_pDragVolume2->Xpos  = m_pMusicVolume->Xpos;
		}
		if(m_bMusicEffOn == FALSE)
			m_CurMusicEffLevel = 7;
		if(m_bMusicOn ==FALSE)
			m_CurMusicLevel = 7;
		else
		{
			m_CurMusicEffLevel = (((m_pDragVolume1->Xpos+m_pDragVolume1->GetWidth()) - m_pEffVolume->Xpos )*100) / (m_pEffVolume->GetWidth());
			m_CurMusicLevel = (((m_pDragVolume2->Xpos+m_pDragVolume2->GetWidth()) - m_pMusicVolume->Xpos )*100) / (m_pMusicVolume->GetWidth());
		}

		break;
	}
}

BOOL COptionPg::IsInside(CSurface* surface, int x, int y)
{
	if( !surface )
		return FALSE;

	if ( ( ( x > surface->Xpos ) && ( x < (surface->Xpos + surface->GetWidth())))
			&& ( ( y > surface->Ypos ) && ( y < (surface->Ypos + surface->GetHeight()))))
		return true;

	else
		return false;
}

// 처음로딩할때..Config.ini파일을 읽는다
void COptionPg::ReadConfigFile(char *file_name)
{
	FILE *fp = NULL;
	char *sentence = NULL;
	char buf[256] = {0, }, first_arg[20] = {0, }, second_arg[10] = {0, };
	int value;

	if ( (fp = fopen(file_name, "rt")) == NULL)
	{
		// 디폴트값 설정.
		m_CurResolution = R800;				// 현재 해상도
		m_CurShadow     = SHADOW_ALONE;		// 현재 그림자
		m_CurWeather    = TRUE;				// 기후변화 사용유무
		m_bMusicEffOn	= TRUE;				// 효과음악 사용유무
		m_bMusicOn		= TRUE;				// 배경음악 사용유무
		m_CurMusicLevel = 50;				// 배경음악 볼륨상태
		m_CurMusicEffLevel = 50;			// 효과음악 볼륨상태
		m_CurMode = 0;						// 전체화면 모드.

		m_Shadow.m_CurSel = m_CurShadow;
		m_Wether.m_CurSel = m_CurWeather;
		m_Mode.m_CurSel	  = m_CurMode;
		m_Res.m_CurSel    = m_CurResolution-1;
		return;
	};

	// 윈도우 모드의 변수는 임시로 초기화
	pCMyApp->m_ProOption.bWindowed = FALSE;
	pCMyApp->m_ProOption.nBackBufferBit = 32;
	pCMyApp->m_ProOption.bUseMusicForEachZone = FALSE;

	sentence = GetSentence(buf, sizeof(buf), fp);

	while (sentence != NULL)
	{
		//AnyOneArg(buf, first_arg);
		AnyTwoArg(buf, first_arg, second_arg);
		value = atoi(second_arg);

		if (!stricmp("Resolution", first_arg))
		{
			pCMyApp->m_ProOption.resolution = (enum RESOLUTION) value;
		}
		else if (!stricmp("Shadow", first_arg))
		{
			pCMyApp->m_ProOption.shadow = (enum SHADOW_OPTION) value;

		}
		else if (!stricmp("Weather", first_arg))
		{
			pCMyApp->m_ProOption.bWeather = value;

		}
		else if (!stricmp("MusicOn", first_arg))
		{
			pCMyApp->m_ProOption.bMusicOn = value;

		}
		else if (!stricmp("MusicEffectOn", first_arg))
		{
			pCMyApp->m_ProOption.bMusicEffOn = value;

		}
		else if (!stricmp("MusicEffectLevel", first_arg))
		{
			pCMyApp->m_ProOption.nMusicEffLevel =  value;

		}
		else if (!stricmp("MusicLevel", first_arg))
		{
			pCMyApp->m_ProOption.nMusicLevel = value;
		}
		else if (!stricmp("Windowed", first_arg))
		{
			if (value == 1)
				pCMyApp->m_ProOption.bWindowed = 1;
			else
				pCMyApp->m_ProOption.bWindowed = 0;
		}
		else if( !stricmp( "ColorBit", first_arg ) )
		{
// 			if( value == 0 )
			pCMyApp->m_ProOption.nBackBufferBit = 32;
// 			else
// 				pCMyApp->m_ProOption.nBackBufferBit = 16;
		}
		else if( !stricmp( "ZoneMusic", first_arg ) )
		{
			if( value == 1 )
				pCMyApp->m_ProOption.bUseMusicForEachZone = TRUE;
			else
				pCMyApp->m_ProOption.bUseMusicForEachZone = FALSE;
		}
		sentence = GetSentence(buf, sizeof(buf), fp);
	}

	m_CurResolution = pCMyApp->m_ProOption.resolution;   // 현재 해상도
	m_CurShadow     = pCMyApp->m_ProOption.shadow;		// 현재 그림자
	m_CurWeather    = pCMyApp->m_ProOption.bWeather;	// 기후변화 사용유무
	m_bMusicEffOn	= pCMyApp->m_ProOption.bMusicEffOn; // 효과음악 사용유무
	m_bMusicOn		= pCMyApp->m_ProOption.bMusicOn;	// 배경음악 사용유무
	m_CurMusicLevel = pCMyApp->m_ProOption.nMusicLevel; // 배경음악 볼륨상태
	m_CurMusicEffLevel = pCMyApp->m_ProOption.nMusicEffLevel; // 효과음악 볼륨상태
	m_CurMode = pCMyApp->m_ProOption.bWindowed;

	m_Shadow.m_CurSel = m_CurShadow;
	m_Wether.m_CurSel = m_CurWeather;
	m_Mode.m_CurSel	  = m_CurMode;
	m_Res.m_CurSel    = m_CurResolution-1;

	fclose(fp);
}


void COptionPg::WriteConfigFile(char *file_name)
{
	FILE *fp = NULL;
	if ( (fp = fopen(file_name, "w")) == NULL)
		return;

	fprintf(fp, "# 건들면 책임 못 집니다..\n");
	fprintf(fp, "# 만약 건드려서 잘 안된다 싶으면, 이 화일을 지우세요.\n");
	fprintf(fp, "Resolution %d\n", m_CurResolution);
	fprintf(fp, "Shadow %d\n", m_CurShadow);
	fprintf(fp, "Weather %d\n", m_CurWeather);
	fprintf(fp, "MusicOn %d\n", m_bMusicOn);
	fprintf(fp, "MusicEffectOn %d\n", m_bMusicEffOn);
	fprintf(fp, "MusicLevel %d\n", m_CurMusicLevel);
	fprintf(fp, "MusicEffectLevel %d\n", m_CurMusicEffLevel);
	fprintf(fp, "Windowed %d\n", m_CurMode);
	fprintf(fp, "ColorBit %d\n", pCMyApp->m_ProOption.nBackBufferBit == 32 ? 0 : 1 );
	fprintf(fp, "ZoneMusic %d\n", pCMyApp->m_ProOption.bUseMusicForEachZone );

	fclose(fp);

	pCMyApp->m_ProOption.resolution = (enum RESOLUTION) m_CurResolution;   // 현재 해상도
	pCMyApp->m_ProOption.shadow = (enum SHADOW_OPTION) m_CurShadow;		// 현재 그림자
	pCMyApp->m_ProOption.bWeather = m_CurWeather;	// 기후변화 사용유무
	pCMyApp->m_ProOption.bMusicEffOn = m_bMusicEffOn; // 효과음악 사용유무
	pCMyApp->m_ProOption.bMusicOn = m_bMusicOn;	// 배경음악 사용유무
	pCMyApp->m_ProOption.nMusicLevel = m_CurMusicLevel; // 배경음악 볼륨상태
	pCMyApp->m_ProOption.nMusicEffLevel = m_CurMusicEffLevel; // 효과음악 볼륨상태
	pCMyApp->m_ProOption.bWindowed = m_CurMode;
}


char *COptionPg::GetSentence(char *dest, int n, FILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = fgets(dest, n, fp)) == NULL)
			return tmp;

		while (iswspace(*tmp) && *tmp != 0)
			tmp++;
	}
	while (*tmp == '#' || *tmp == 0);

	return tmp;
}

char *COptionPg::AnyTwoArg(char *argument, char *first_arg, char *second_arg)
{
	return (AnyOneArg(AnyOneArg(argument, first_arg), second_arg)); // :-)
}


#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

void COptionPg::SkipSpaces(char **string)
{
	for (; **string && is_space(**string); (*string)++);
}

char *COptionPg::AnyOneArg(char *argument, char *first_arg)
{
	if( argument == NULL || first_arg == NULL )
		return NULL;

	SkipSpaces(&argument);

	while (*argument && !is_space(*argument))
	{
		*(first_arg++) = *argument;
		argument++;
	}

	*first_arg = '\0';

	return (argument);
}

HRESULT COptionPg::Restore()
{
	HRESULT hr = S_OK;

	return hr;
}
