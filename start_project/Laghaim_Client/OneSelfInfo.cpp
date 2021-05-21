#include "stdafx.h"
//----------------------------------------------------------------------
//		INCLUDE
//----------------------------------------------------------------------
#include "headers.h"
#include "sheet.h"
#include "OneSelfInfo.h"
#include "Nk2DFrame.h"
#include "textbox.h"
#include "main.h"
#include "LoadEffectSound.h"




extern char g_strFontName[];

//----------------------------------------------------------------------
//		DEFINE
//----------------------------------------------------------------------
#define CLOSE_WINDOW	91
#define SHOW_WINDOW		92
//----------------------------------------------------------------------
//		CONSTRUCTOR / DESTRUCTOR
//----------------------------------------------------------------------
COneSelfInfo::COneSelfInfo()
	: m_hWnd(NULL)
	, m_fStretchX(0)
	, m_fStretchY(0)
	, m_FontWidth(0)
	, m_FontHeight(0)
	, m_TextColor(0)
	, m_TextBackColor(0)
{
	memset(m_Tmp, 0, sizeof(m_Tmp));

	m_pSheet   = NULL;
	m_pBackSur = NULL;
	m_Font	   = NULL;

	for(int i=0; i<TEXTSURFACE_CNT; i++)
		m_pTextSur[i] = NULL;

	for (int i = 0; i < TEXTSURFACE_CNT; i++)
		m_SaveBuf[i][0] = '\0';
}

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
COneSelfInfo::~COneSelfInfo()
{
	DeleteRes();
}

//----------------------------------------------------------------------
//		INITIALIZE
//----------------------------------------------------------------------
void COneSelfInfo::Init(int FontSize)
{
	SetFont(FontSize/2, FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	m_FontWidth  = sizeFont.cx;
	m_FontHeight = sizeFont.cy;
}

//----------------------------------------------------------------------
//		LOAD RESOURCE
//----------------------------------------------------------------------
void COneSelfInfo::LoadRes()
{
	if( !g_pDisplay )
		return;
	/////////////////////////////

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_fStretchX = (m_ClientRc.right-m_ClientRc.left) / 640.0f ;
	m_fStretchY = (m_ClientRc.bottom-m_ClientRc.top) / 480.0f;


	SAFE_DELETE(m_pBackSur);
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/oneselfinfo.bmp");
	if( !m_pBackSur )
		return;

	m_pBackSur->SetColorKey(RGB(0, 0, 0));
	m_pBackSur->Xpos = 640 - m_pBackSur->GetWidth();
	m_pBackSur->Ypos = 36;

	m_CloseBtn.SetFileName("btn_close");
	m_CloseBtn.SetDisable(false);
	m_CloseBtn.LoadRes();
//	m_CloseBtn.SetStretchRate(m_fStretchX, m_fStretchY);
	m_CloseBtn.PosX = m_pBackSur->Xpos+236;
	m_CloseBtn.PosY = m_pBackSur->Ypos+4;

	if(/*m_ClientRc.right>0 && */m_ClientRc.right<=640)
		Init(12);
	else if(/*m_ClientRc.right>640 && */m_ClientRc.right<=800)
		Init(16);
	else// if(m_ClientRc.right>800/* && m_ClientRc.right<1157*/)
		Init(20);

	Con_DrawText(0, m_pBackSur->Xpos+51,	m_pBackSur->Ypos+13);
	Con_DrawText(1, m_pBackSur->Xpos+51,	m_pBackSur->Ypos+27);
	Con_DrawText(2, m_pBackSur->Xpos+158, m_pBackSur->Ypos+12);
	Con_DrawText(3, m_pBackSur->Xpos+158, m_pBackSur->Ypos+29);
	Con_DrawText(4 ,m_pBackSur->Xpos+88,	m_pBackSur->Ypos+44);
	Con_DrawText(5,m_pBackSur->Xpos+88,	m_pBackSur->Ypos+67);
	Con_DrawText(6, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+85);
	Con_DrawText(7, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+100);
	Con_DrawText(8, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+114);
	Con_DrawText(9, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+140);
	Con_DrawText(10, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+155);
	Con_DrawText(11, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+170);
	Con_DrawText(12, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+186);
	Con_DrawText(13, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+202);
	Con_DrawText(14, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+228);
	Con_DrawText(15, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+244);
	Con_DrawText(16, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+259);
	Con_DrawText(17, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+275);
	Con_DrawText(18, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+299);
	Con_DrawText(19, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+315);
	Con_DrawText(21, m_pBackSur->Xpos+111, m_pBackSur->Ypos+345);
	RestoreAll();
}

//----------------------------------------------------------------------
//		DELETE
//----------------------------------------------------------------------
void COneSelfInfo::DeleteRes()
{
	for(int i=0; i<TEXTSURFACE_CNT; i++)
		SAFE_DELETE(m_pTextSur[i]);

	SAFE_DELETE(m_pBackSur);
	m_CloseBtn.DeleteRes();
}

//----------------------------------------------------------------------
//		DRAW
//----------------------------------------------------------------------
void COneSelfInfo::Draw()
{
	if( !m_pBackSur || !g_pDisplay )
		return;

	DrawBack(m_pBackSur->Xpos, m_pBackSur->Ypos);
	m_CloseBtn.DrawToBlt();

	for(int i = 0 ; i< TEXTSURFACE_CNT; i++)
	{
		if (m_pTextSur[i])
			g_pDisplay->Blt(m_pTextSur[i]->Xpos, m_pTextSur[i]->Ypos + pCMyApp->Font_AddPosY, m_pTextSur[i]);
	}
}

//----------------------------------------------------------------------------------
//		텍스트를 그린다!!!
//----------------------------------------------------------------------------------
void COneSelfInfo::PutString(int index, char* String)
{
	if(index > TEXTSURFACE_CNT)
		return;

	if ( !g_pDisplay || !m_Font || String == NULL )
		return;

	int let = strlen(String);

	if(m_pTextSur[index])
		SAFE_DELETE(m_pTextSur[index]);

	strcpy(m_SaveBuf[index], String);
	g_pDisplay->CreateSurface( &m_pTextSur[index], let*m_FontWidth,  m_FontHeight);

	if( m_pTextSur[index] )
	{
		m_pTextSur[index]->DrawText(m_Font, m_SaveBuf[index], 0, 0, RGB(0, 0, 0), RGB(255, 228, 132), 0);
		m_pTextSur[index]->SetColorKey(RGB(0, 0, 0));
	}
}

void COneSelfInfo::Con_DrawText(int idx, int x, int y)
{
	int GetPercent_Width;
	int GetPercent_Height;
	int Image_Xpos;
	int Image_Ypos;

	for(int i =0; i<TEXTSURFACE_CNT; i++)
	{
		if(m_pTextSur[i] !=  NULL)
		{
			GetPercent_Width = ((m_ClientRc.right-m_ClientRc.left)* 100) / 640 ;
			Image_Xpos	 = ((x)*(GetPercent_Width)) / (100);

			GetPercent_Height= ((m_ClientRc.bottom-m_ClientRc.top)*100) / (480);
			Image_Ypos	 = ((y)*(GetPercent_Height)) / (100);

			m_pTextSur[idx]->Xpos = Image_Xpos ;
			m_pTextSur[idx]->Ypos = Image_Ypos ;
		}
	}
}


void COneSelfInfo::RestoreAll()
{
	if( !m_pBackSur || !g_pDisplay )
		return;

	for(int i = 0; i<TEXTSURFACE_CNT; i++)
	{
		int let = strlen(m_SaveBuf[i]);

		if (let > 0)
		{
			g_pDisplay->CreateSurface( &m_pTextSur[i], let*m_FontWidth,  m_FontHeight);

			if( m_pTextSur[i] )
			{
				m_pTextSur[i]->DrawText(m_Font, m_SaveBuf[i], 0, 0, RGB(0, 0, 0), RGB(255, 210, 255), 0);
				m_pTextSur[i]->SetColorKey(RGB(0, 0, 0));
			}
		}
	}

	Con_DrawText(0, m_pBackSur->Xpos+51,	m_pBackSur->Ypos+13);
	Con_DrawText(1, m_pBackSur->Xpos+51,	m_pBackSur->Ypos+27);
	Con_DrawText(2, m_pBackSur->Xpos+158, m_pBackSur->Ypos+12);
	Con_DrawText(3, m_pBackSur->Xpos+158, m_pBackSur->Ypos+29);
	Con_DrawText(4 ,m_pBackSur->Xpos+88,	m_pBackSur->Ypos+44);
	Con_DrawText(5,m_pBackSur->Xpos+88,	m_pBackSur->Ypos+67);
	Con_DrawText(6, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+85);
	Con_DrawText(7, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+100);
	Con_DrawText(8, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+114);
	Con_DrawText(9, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+140);
	Con_DrawText(10, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+155);
	Con_DrawText(11, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+170);
	Con_DrawText(12, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+186);
	Con_DrawText(13, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+202);
	Con_DrawText(14, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+228);
	Con_DrawText(15, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+244);
	Con_DrawText(16, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+259);
	Con_DrawText(17, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+275);
	Con_DrawText(18, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+299);
	Con_DrawText(19, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+315);
	Con_DrawText(21, m_pBackSur->Xpos+111, m_pBackSur->Ypos+345);
}

//-- Global Font 중 하나를 받아온다.
//-- 2004.03.15 - 수정완료
//-- Lyul : 자체 생성에서 Global 사용으로 변경
void COneSelfInfo::SetFont (int f_w, int f_h, char *fontname)
{
	if( fontname == NULL )
		return;

	// Font 생성
	m_Font = G_GetFont(f_h,0,fontname);
}

//-----------------------------------------------------------------------------
//		 두가지 컨트롤 체팅박스 / 아이템박스 계산해서 그리기
//-----------------------------------------------------------------------------
void COneSelfInfo::DrawBack(int xpos, int ypos)
{
	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	RECT	DestRc;

	int GetPercent_Width;
	int GetPercent_Height;
	int Image_Width;
	int Image_Height;
	int Image_Xpos;
	int Image_Ypos;

	GetPercent_Width = ((m_ClientRc.right-m_ClientRc.left)* 100) / 640 ;
	Image_Width  = ((m_pBackSur->GetWidth()*(GetPercent_Width)) / 100);
	Image_Xpos	 = ((xpos)*(GetPercent_Width)) / (100);

	GetPercent_Height= ((m_ClientRc.bottom-m_ClientRc.top)*100) / (480);
	Image_Height = ((m_pBackSur->GetHeight()*(GetPercent_Height)) / (100));
	Image_Ypos	 = ((ypos)*(GetPercent_Height)) / (100);

	DestRc.left   = Image_Xpos;
	DestRc.right  = Image_Xpos+Image_Width;
	DestRc.top    = Image_Ypos;
	DestRc.bottom = Image_Ypos+Image_Height;

	if( g_pDisplay && m_pBackSur)
		g_pDisplay->GetBackBuffer()->Blt(&DestRc, m_pBackSur->GetDDrawSurface(), NULL, DDBLT_KEYSRC |DDBLT_WAIT, &ddbltfx);
}

BOOL COneSelfInfo::IsInside(int x, int y)
{
	if( m_pBackSur )
	{
		// 아이텀 박스의 영역또한 마우스체크를 체크했다..
		if ( ( ( x > m_pBackSur->Xpos*m_fStretchX ) && ( x < (m_pBackSur->Xpos + m_pBackSur->GetWidth())*m_fStretchX) )
				&& ( ( y > m_pBackSur->Ypos*m_fStretchY ) && ( y < (m_pBackSur->Ypos + m_pBackSur->GetHeight())*m_fStretchY ) ) )
			return true;
	}
	return false;
}

LRESULT COneSelfInfo::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if(m_CloseBtn.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowInterfaceWindow(SCORE, FALSE);
			m_CloseBtn.m_iBtnFlg = BTN_NORMAL;
		}
		break;
	}
	return 0;
}



//  이름
void COneSelfInfo::Set_Name(char* name)
{
	PutString(0,  name);
	if( m_pBackSur )
		Con_DrawText(0, m_pBackSur->Xpos+51,	m_pBackSur->Ypos+13);
}

// 종족
void COneSelfInfo::Set_Race(char* race)
{
	PutString(1, race);
	if( m_pBackSur )
		Con_DrawText(1, m_pBackSur->Xpos+51,	m_pBackSur->Ypos+27);
}

// 레벨
void COneSelfInfo::Set_Level(int level)
{
	_itoa( level, m_Tmp, 10 );

	PutString(2, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(2, m_pBackSur->Xpos+158, m_pBackSur->Ypos+12);
}

// 성별
void COneSelfInfo::Set_Sex(char* sex)
{
	PutString(3, sex);
	if( m_pBackSur )
		Con_DrawText(3, m_pBackSur->Xpos+158, m_pBackSur->Ypos+29);
}

// 게임시간
void COneSelfInfo::Set_GameTime(int time)
{
	_itoa( time, m_Tmp, 10 );

	PutString(4, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(4 ,m_pBackSur->Xpos+88,	m_pBackSur->Ypos+44);
}

// 생명력
void COneSelfInfo::Set_Vital(int vital )
{
	_itoa( vital, m_Tmp, 10 );

	PutString(5, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(5,m_pBackSur->Xpos+88,	m_pBackSur->Ypos+67);
}

// 마법력
void COneSelfInfo::Set_Mana(int mana)
{
	_itoa( mana, m_Tmp, 10 );

	PutString(6, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(6, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+85);
}

// 스테미너
void COneSelfInfo::Set_Stamina(int stamina)
{
	_itoa( stamina, m_Tmp, 10 );

	PutString(7, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(7, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+100);
}

// 전기력
void COneSelfInfo::Set_Epower(int epower)
{
	_itoa( epower, m_Tmp, 10 );

	PutString(8, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(8, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+114);
}


// 힘
void COneSelfInfo::Set_Str(int str)
{
	_itoa( str, m_Tmp, 10 );

	PutString(9, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(9, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+140);
}


// 지혜
void COneSelfInfo::Set_Int(int Int)
{
	_itoa( Int, m_Tmp, 10 );

	PutString(10, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(10, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+155);
}

// 기민성
void COneSelfInfo::Set_Dex(int dex)
{
	_itoa( dex, m_Tmp, 10 );

	PutString(11, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(11, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+170);
}

// 체질
void COneSelfInfo::Set_Con(int con)
{
	_itoa( con, m_Tmp, 10 );

	PutString(12, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(12, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+186);
}

// 통솔력
void COneSelfInfo::Set_Cha(int cha)
{
	_itoa( cha, m_Tmp, 10 );

	PutString(13, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(13, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+202);
}

// 평균타격속도
void COneSelfInfo::Set_AAtt_Val(int aatt_s)
{
	_itoa( aatt_s, m_Tmp, 10 );

	PutString(14, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(14, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+228);
}

// 공격속도
void COneSelfInfo::Set_Att_S(int att_s)
{
	_itoa( att_s, m_Tmp, 10 );

	PutString(15, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(15, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+244);
}

// 방어력
void COneSelfInfo::Set_Def(int def)
{
	_itoa( def, m_Tmp, 10 );

	PutString(16, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(16, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+259);
}

// 이동속도
void COneSelfInfo::Set_Move_S(int move_s)
{
	_itoa( move_s, m_Tmp, 10 );

	PutString(17, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(17, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+275);
}


// 보유금액
void COneSelfInfo::Set_CurMoney(_int64 curmoney)
{
	_ltoa_s( curmoney, m_Tmp, 10 );

	PutString(18, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(18, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+299);
}

// 총경험치
void COneSelfInfo::Set_CurExp(_int64 exp)
{
	_ltoa_s( exp, m_Tmp, 10 );

	PutString(19, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(19, m_pBackSur->Xpos+88,	m_pBackSur->Ypos+315);
}

// 레벨업 까지 남은 경험치
void COneSelfInfo::Set_NeedExp(_int64 needexp)
{
	_ltoa_s( needexp, m_Tmp, 10 );

	PutString(20, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(20, m_pBackSur->Xpos+165, m_pBackSur->Ypos+329);
}


// 레벨업 포인트
void COneSelfInfo::Set_LevelUpPoint(int levelpoint)
{
	_itoa( levelpoint, m_Tmp, 10 );

	PutString(21, m_Tmp);
	if( m_pBackSur )
		Con_DrawText(21, m_pBackSur->Xpos+111, m_pBackSur->Ypos+345);
}






