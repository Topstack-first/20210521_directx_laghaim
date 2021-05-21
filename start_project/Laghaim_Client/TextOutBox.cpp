#include "stdafx.h"
// TextOutBox.cpp: implementation of the CTextOutBox class.
//
//////////////////////////////////////////////////////////////////////

#include "TextOutBox.h"

#include "ddutil.h"
//#include "DIme.h"
#include "textbox.h"
#include "main.h"
#include "LoadEffectSound.h"

#include "Nk2DFrame.h"
// [5/7/2008 반재승] 스트링의 해상도에 따른 위치 변경
extern DWORD	g_dwClientCountry;		   // [5/7/2008 반재승] 스트링의 해상도에 따른 위치 변경





extern char g_strFontName[];
extern void itocola(char *buf, int num);

// 현제 10억 단위 까지...
// 100조까지?
#define FMT1 "zzz,zzz,zzz,zzz,zzz,zzz,zz9"
#define FMT2 "----,----,----,----,----,---,--9"

//#define FMT1 "zzz,zzz,zz9"
//#define FMT2 "----,---,--9"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextOutBox::CTextOutBox()
	: m_PosX(0)
	, m_PosY(0)
	, m_Height(0)
	, m_StringPosX(0)
	, m_StringPosY(0)
	, m_FontWidth(0)
	, m_FontSize(0)
	, m_FontColor(0)
	, m_Type(0)
{
	memset(m_sMoney, 0, sizeof(m_sMoney));

	m_pSurf = NULL;

	m_Font = NULL;
	m_bVisible = TRUE;
	m_Width = 0;
	memset(m_str, 0, 255);
	m_bCenter = false;

	m_AreaWidth  = 0;
	m_AreaHeight = 0;
	m_pOverSurf = NULL;
	m_bOver		= FALSE;
}
//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
//-- Destructor

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CTextOutBox::~CTextOutBox()
{
	DeleteRes();
}

void CTextOutBox::DeleteRes()
{
	if (m_pSurf)
		delete m_pSurf;
	m_pSurf = NULL;

	if(m_pOverSurf)
		delete m_pOverSurf;

	m_pOverSurf = NULL;
}

void CTextOutBox::Restore()
{
	if (strlen(m_str) > 0)
		SetString(m_str);
}

void CTextOutBox::Init(int font_size, COLORREF font_color, int pos_x, int pos_y, int width, int height, BOOL bCenter)
{
	m_bCenter = bCenter;

	SetSize(width, height);
	SetPos(pos_x, pos_y);


	{
		m_AreaWidth  = width ;
		m_AreaHeight = height;
	}

	SetFont(font_size/2, font_size, g_strFontName);
	m_FontSize = font_size;
	m_FontColor = font_color;


}

// nate
// TextOutBox의 문자 셋팅함수
void CTextOutBox::SetString(char *string, COLORREF font_color)
{
	DeleteRes();

	int len = strlen(string);
	if (len <= 0)
	{
		return;
	}

	if (!g_pDisplay || !m_Font)
		return;

	strcpy(m_str, string);		// 스트링을 멤버에 추가

	if (font_color == RGB(0, 0, 0))
		font_color = m_FontColor;		// 컬러 설정
	// Surface생성
	g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, string, RGB(0, 0, 0), font_color);
	if( m_pSurf == NULL)
		return;

	m_pSurf->SetColorKey(RGB(0, 0, 0));

	g_pDisplay->CreateSurfaceFromText(&m_pOverSurf, m_Font, string, RGB(0, 0, 0), RGB(255,255,255));
	if( m_pOverSurf == NULL)
		return;

	m_pOverSurf->SetColorKey(RGB(0, 0, 0));

	// 현재 생성된 서피스의 크기를 멤버에 셋팅
	m_Width = m_pSurf->GetWidth();
	m_Height = m_pSurf->GetHeight();

	// 중심 맞추기
	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX - x_size/2;
		m_StringPosY = m_PosY;
	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}
}


void CTextOutBox::SetString_Center_Arrange(char *string, COLORREF font_color, COLORREF font_colorover, BOOL bCenter )
{
	DeleteRes();

	int len = strlen(string);
	if (len <= 0)
	{
		return;
	}

	if (!g_pDisplay || !m_Font)
		return;

	strcpy(m_str, string);		// 스트링을 멤버에 추가

	if (font_color == RGB(0, 0, 0))
		font_color = m_FontColor;		// 컬러 설정
	// Surface생성
	g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, string, RGB(0, 0, 0), font_color);
	if( m_pSurf == NULL)
		return;

	m_pSurf->SetColorKey(RGB(0, 0, 0));

	g_pDisplay->CreateSurfaceFromText(&m_pOverSurf, m_Font, string, RGB(0, 0, 0), font_colorover);
	if( m_pOverSurf == NULL)
		return;
	// 서피스 컬러키 설정
	m_pSurf->SetColorKey(RGB(0, 0, 0));

	// 현재 생성된 서피스의 크기를 멤버에 셋팅
	m_Width = m_pSurf->GetWidth();
	m_Height = m_pSurf->GetHeight();

	// 중심 맞추기
	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX + (m_AreaWidth/2 - x_size/2);

		// [5/7/2008 반재승] 스트링의 해상도에 따른 위치 변경( y값만 )
		//connie-test
		if( g_dwClientCountry == CTRY_BRA)
		{
			if( g_pNk2DFrame->GetClientWidth() == 1024 )
			{
				m_StringPosY = m_PosY;
			}

			else
			{
				//m_StringPosY = m_PosY + 30;
				m_StringPosY = m_PosY;
			}
		}

		else
		{
			m_StringPosY = m_PosY;
		}

	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}
}

void CTextOutBox::SetString(char *string, COLORREF font_color, COLORREF background)
{
	DeleteRes();

	int len = strlen(string);
	if (len <= 0)
	{
		return;
	}

	if (!g_pDisplay || !m_Font)
		return;

	strcpy(m_str, string);

	if (font_color == RGB(0, 0, 0))
		font_color = m_FontColor;

	g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, string, background, font_color);
	if( m_pSurf == NULL)
		return;

	m_Width = m_pSurf->GetWidth();
	m_Height = m_pSurf->GetHeight();

	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX - x_size/2;
		m_StringPosY = m_PosY;
	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}
}

// 글자에 테두리를 넣어서 출력하고 싶을때 쓰는 함수. (너무 많이 쓰면 느려짐의 원인이 될 수 있으므로 주의1) (by 원석)
void CTextOutBox::SetString_withOutLine( char *string, COLORREF font_color, COLORREF outline_color )
{
	DeleteRes();

	int len = strlen(string);
	if (len <= 0)
	{
		return;
	}

	if (!g_pDisplay || !m_Font)
		return;

	strcpy(m_str, string);

	if (font_color == RGB(0, 0, 0))
		font_color = m_FontColor;
	g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, string, RGB(255, 0, 255), font_color, FALSE); // 여기서 생성되는 이미지는 실제론 무의미.(아래쪽에서 지우고 갱신해버린다.)
	if( m_pSurf == NULL)
		return;

	m_pSurf->SetColorKey(RGB(255, 0, 255));

//	m_pSurf->Clear( RGB(255, 0, 255) ); // 한번 투명색으로 깔끔하게 지운다. // 이거 16비트 컬러 해상도에서 배경색이 안지워지는 버그가 있어서 막음. 굳이 clear안해도 문제없더라.

	m_Width = m_pSurf->GetWidth();
	m_Height = m_pSurf->GetHeight();

	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX - x_size/2;
		m_StringPosY = m_PosY;
	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}

	// 서피스의 dc를 얻어와서 이미지를 바꾼다.
	HDC     hDC = NULL;
	HRESULT hr;
	HFONT	oldFont;

	if( m_pSurf->GetDDrawSurface() == NULL || string == NULL )
		return;

	// Make sure this surface is restored.
	if( FAILED( hr = m_pSurf->GetDDrawSurface()->Restore() ) )
		return;

	if( FAILED( hr = m_pSurf->GetDDrawSurface()->GetDC( &hDC ) ) )
		return;

	//-- Font 설정
	if( m_Font )
	{
		oldFont = (HFONT) SelectObject( hDC, m_Font );
	}

	SetBkMode(hDC, TRANSPARENT);

	// 외곽선 찍기.
	SetTextColor( hDC, outline_color );

	// 외곽선을 8번찍으면 좀더 빈틈없이 외곽선을 칠수있지만, 너무 딱딱한 느낌과 속도문제로 주변 4픽셀만 움직여서 찍어준다.
//	TextOut( hDC, -1, -1, string, strlen(string) );
	TextOut( hDC, -1, 0, string, strlen(string) );
//	TextOut( hDC, -1, 1, string, strlen(string) );
//	TextOut( hDC, 1, -1, string, strlen(string) );
	TextOut( hDC, 1, 0, string, strlen(string) );
//	TextOut( hDC, 1, 1, string, strlen(string) );
	TextOut( hDC, 0, -1, string, strlen(string) );
	TextOut( hDC, 0, 1, string, strlen(string) );

	// 본래 글자 찍기.
	SetTextColor( hDC, font_color );
	TextOut( hDC, 0, 0, string, strlen(string) );

	if (m_Font)
	{
		SelectObject( hDC, oldFont);
	}

	// 다쓴 서피스 dc를 풀어준다.
	if( FAILED( hr = m_pSurf->GetDDrawSurface()->ReleaseDC( hDC ) ) )
		return;
}

void CTextOutBox::ChangeColor(COLORREF color)
{

	int len = strlen(m_str);
	if(len<1)
		return;

	if(m_pSurf)
	{
		delete m_pSurf;
		m_pSurf = NULL;
	}
	g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_str, RGB(0, 0, 1), color);
	if( m_pSurf == NULL)
		return;

	m_pSurf->SetColorKey(RGB(0, 0, 0));
}

void CTextOutBox::SetPos_Center(int x, int y)
{
	m_PosX = x;
	m_PosY = y;

	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX + (m_AreaWidth/2 - x_size/2);
		m_StringPosY = m_PosY;
	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}


}

void CTextOutBox::SetPos(int x, int y)
{
	m_PosX = x;
	m_PosY = y;

	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX - x_size/2;
		m_StringPosY = m_PosY;
	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}
}




int CTextOutBox::GetBoxHeight()
{
	if (m_pSurf)
	{
		return m_pSurf->GetHeight();
	}
	else
	{
		return -1;
	}
}

int CTextOutBox::GetBoxWidth()
{
	if (m_pSurf)
	{
		return m_pSurf->GetWidth();
	}
	else
	{
		return -1;
	}
}
/*
//.............................................. 수정 부분
void CTextBox::CrearText()
{
	if(m_pszStr != NULL)
	{
		ZeroMemory(m_pszStr, strlen(m_pszStr));
		m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);
		m_nCharCount = 0;
	}
}
//...........................................................
*/
void CTextOutBox::SetString(UINT64 value, BOOL bcomma)
{
	DeleteRes();

	char string[32];	

	sprintf(string, "%I64d", value);

	if(bcomma)
	{
		if( value == 0 )
		{
			SetString("0");
		}
		else
		{
			SetString(TransComma(string));
		}
		
		return;
	}

	SetString(string);
}

void CTextOutBox::Draw()
{
	Draw(m_StringPosX, m_StringPosY);
}

void CTextOutBox::Draw(int x, int y)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, m_pSurf);
}


void CTextOutBox::DrawText(int y, BOOL bOver)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	m_StringPosY = y;

	if(!bOver)
		g_pDisplay->Blt(m_StringPosX, y + pCMyApp->Font_AddPosY, m_pSurf);
	else
		g_pDisplay->Blt(m_StringPosX, y + pCMyApp->Font_AddPosY, m_pOverSurf);
}

void CTextOutBox::Draw(int dest_cx)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	int x = m_StringPosX + (dest_cx/2) - (m_Width/2);
	int y = m_StringPosY;

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, m_pSurf);
}

BOOL CTextOutBox::DrawClipText(int y, int nMinArea, int nMaxArea, int nScrollPos, BOOL bOver /* = FALSE */)
{
	if(m_pSurf == NULL)
		return FALSE;

	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = m_pSurf->GetWidth();
	src.bottom = m_pSurf->GetHeight();

	dest.left   = m_StringPosX;
	dest.top    = y + pCMyApp->Font_AddPosY;
	dest.right  = m_StringPosX + m_pSurf->GetWidth();
	dest.bottom = y + pCMyApp->Font_AddPosY + m_pSurf->GetHeight();

	if(dest.top - nScrollPos <= nMinArea &&
			dest.bottom - nScrollPos <= nMinArea)	// 모두 밖에 있음..
	{
		return FALSE;
	}
	else if(dest.top - nScrollPos <= nMinArea &&		// 윗 영역에 걸쳤음.
			dest.bottom - nScrollPos >= nMinArea)
	{
		src.top  = abs((dest.top - nScrollPos) - nMinArea );
		dest.top = nMinArea;
		dest.bottom = (dest.bottom - nScrollPos);
	}
	else if(dest.top - nScrollPos >= nMinArea &&		// 모두 영역 안에 있음.
			dest.bottom - nScrollPos <= nMaxArea/*BOUNDARY_LINE_POSY_TOP*/)
	{
		dest.left   = m_StringPosX;
		dest.top    = y + pCMyApp->Font_AddPosY - nScrollPos;
		dest.right  = m_StringPosX + m_pSurf->GetWidth();
		dest.bottom = dest.top + m_pSurf->GetHeight();
	}
	else if(dest.top - nScrollPos <= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( nMaxArea - (dest.top - nScrollPos) );
		dest.top    = (dest.top- nScrollPos);
		dest.bottom = nMaxArea;
	}
	else if(dest.top - nScrollPos >= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}

	g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, m_pSurf, &src);

	return TRUE;
}


void CTextOutBox::Draw(RECT rc, int x, int y)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, m_pSurf, &rc);
}

void CTextOutBox::Draw(int x, int y, int dest_cx)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	x = x + (dest_cx/2) - (m_Width/2);

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, m_pSurf);
}

//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font 중 하나를 받아온다.

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경

void CTextOutBox::SetFont (int f_w, int f_h, char *fontname)
{
	m_FontWidth = f_w;
	m_FontHeight = f_h;

	// Font 생성
	m_Font = G_GetFont(f_h,0,fontname);
}


char* CTextOutBox::TransComma(char* money)
{
	char fmt[32];
	//char Buf[20];

	gets (money);
	if (*money != '\0')  	// 널이 아니면
	{
		if (*money == '+' || *money == '-')		// 현금의 추가인지 제거인지 체크
			strcpy(fmt, FMT2);
		else strcpy(fmt, FMT1);
	}

	strcpy(m_sMoney, iedt(money, fmt));


	return m_sMoney;
}
char* CTextOutBox::delchr(char *str)
{
	char *cp;

	for (cp = str; *cp != '\0'; cp++) *cp = *(cp + 1);
	return(str);
}
char* CTextOutBox::iedt(char *str, char *fmt)
{
	char *ptr, fil;
	int ix,ox,l1,l2,sw=0;
	long val = atol(str);

	printf("%ld\n",val);
	if (*str == '+' || *str == '-') delchr(str);
	while (*str == '0') delchr(str); //leading zero suppressing
	l1 = strlen(str);
	l2 = strlen(fmt);
	fil= *fmt;
	for(ix=l1-1,ox=l2-1,ptr=fmt; ox>=0; ox--,ix--)
	{
		if (ix < 0)
		{
			switch (fil)
			{
			case 'z':
				fil = ' ';
				break;
			case '9':
				fil = '0';
				break;
			case '+':
				if (sw==0)
				{
					if (val < 0) fil = '-';
					else fil = '+';
					sw = 1;
				}
				else fil = ' ';
				break;
			case '-':
				if (sw==0)
				{
					if (val < 0) fil = '-';
					else fil = ' ';
					sw = 1;
				}
				else fil = ' ';
				break;
			case '*':
				fil = '*';
				break;
			default:
				fil = ' ';
				break;
			}
			*(fmt+ox) = fil;
			continue;
		}
		else
		{
			if (*(fmt+ox) == ',')
				if (fil != '*')ox--;
			if (*(fmt+ox) == ':') ox--;
			if (*(fmt+ox) == '/') ox--;
			*(fmt+ox) = *(str+ix);
		}
	}

	while(*ptr == 0x20) ptr++;

	return(ptr);
}

extern BOOL g_bFixedWidthFont;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextOutMultiBox::CTextOutMultiBox()
	: m_PosX(0)
	, m_PosY(0)
	, m_Height(0)
	, m_StringPosX(0)
	, m_StringPosY(0)
	, m_FontWidth(0)
	, m_FontHeight(0)
	, m_FontSize(0)
	, m_FontColor(0)
	, m_Type(0)
{
	m_pSurf = NULL;

	m_Font = NULL;
	m_bVisible = TRUE;
	m_Width = 0;
	m_bCenter = false;


	m_AreaWidth  = 0;
	m_AreaHeight = 0;
	m_pOverSurf = NULL;
	m_bOver		= FALSE;
}
//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
//-- Destructor

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CTextOutMultiBox::~CTextOutMultiBox()
{
	DeleteRes();
}

void CTextOutMultiBox::DeleteRes()
{
	m_MultiText.Reset();

	if (m_pSurf)
		delete m_pSurf;
	m_pSurf = NULL;

	if(m_pOverSurf)
		delete m_pOverSurf;

	m_pOverSurf = NULL;
}

void CTextOutMultiBox::Restore()
{
	if (m_MultiText.m_LineCount)
	{
		SetString(&m_MultiText, m_FontColor);
	}
}

void CTextOutMultiBox::Init(int font_size, COLORREF font_color, int pos_x, int pos_y, int width, int height, BOOL bCenter)
{
	m_bCenter = bCenter;

	SetSize(width, height);
	SetPos(pos_x, pos_y);
	if( g_SvrType == ST_ADULT_ONLY )
	{
		m_AreaWidth  = width ;
		m_AreaHeight = height;
	}

	SetFont(font_size/2, font_size, g_strFontName);
	m_FontSize = font_size;
	m_FontColor = font_color;
}

#define LINE_HEIGHT 15;

// nate
// TextOutBox의 문자 셋팅함수
void CTextOutMultiBox::SetString(CMultiText* multiText, COLORREF font_color)
{
	DeleteRes();

	m_MultiText.Clone(*multiText);

//	DDSURFACEDESC2 ddsd;
	HDC hDC = GetDC( pCMyApp->Get_hWnd() );
	HFONT hFont = (HFONT) SelectObject( hDC, m_Font );

	SIZE sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);
	int FontWidth  = sizeFont.cx;

	// 가장큰 문자열의 개수를 얻는다
	int TextWidth=0, TextHeight=0;
	for(int i = 0; i < m_MultiText.m_LineCount; i++)
	{
		// 입력받은 텍스트를 루프를 돌면서 가장긴 문자열 체크
		int len = strlen(m_MultiText.m_Text[i]);
		// 문자열의 폭이 고정되었는가?
		if (g_bFixedWidthFont)
		{
			TextWidth = MAX(TextWidth, len * FontWidth);
		}
		else
		{
			// 폰트 사이즈 반환
			GetTextExtentPoint32(hDC, m_MultiText.m_Text[i], len, &sizeFont);
			TextWidth = MAX(TextWidth, sizeFont.cx);
		};
	}

	TextHeight = m_MultiText.m_LineCount*LINE_HEIGHT;

	// 계산된 크기로 서피스 생성
	g_pDisplay->CreateSurface( &m_pSurf, TextWidth, TextHeight );
	if( !m_pSurf ) return ;

	// fillcolor를 이용한 textsurface 클리어하기
	m_pSurf->Clear();

	// 들어온 문자열 가운데로 정렬해서 DrawText
	int Xpos = 0;
	int Ypos = 0;

	Ypos = 0;
	for( int i = 0 ; i < m_MultiText.m_LineCount ; ++i )
	{
		if (g_bFixedWidthFont)
			Xpos = 0;
		else
		{
			GetTextExtentPoint32(hDC, m_MultiText.m_Text[i], strlen(m_MultiText.m_Text[i]), &sizeFont);
			Xpos = 0;
		}

		m_pSurf->DrawText( m_Font, m_MultiText.m_Text[i], Xpos, Ypos,
						   m_MultiText.m_TextBackColor[i], m_MultiText.m_TextColor[i], 0);

		Ypos += LINE_HEIGHT;
	}
	m_pSurf->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hDC, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hDC);

	m_Width = TextWidth;
	m_Height = TextHeight;
}


void CTextOutMultiBox::SetString_Center_Arrange(CMultiText* multiText, COLORREF font_color, COLORREF font_colorover, BOOL bCenter )
{
	DeleteRes();
}

void CTextOutMultiBox::SetString(CMultiText* multiText, COLORREF font_color, COLORREF background)
{
	DeleteRes();
}

// 글자에 테두리를 넣어서 출력하고 싶을때 쓰는 함수. (너무 많이 쓰면 느려짐의 원인이 될 수 있으므로 주의1) (by 원석)
void CTextOutMultiBox::SetString_withOutLine( CMultiText* multiText, COLORREF font_color, COLORREF outline_color )
{
	DeleteRes();
}

void CTextOutMultiBox::ChangeColor(COLORREF color)
{
}

void CTextOutMultiBox::SetPos_Center(int x, int y)
{
	m_PosX = x;
	m_PosY = y;

	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX + (m_AreaWidth/2 - x_size/2);
		m_StringPosY = m_PosY;
	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}
}


void CTextOutMultiBox::SetPos(int x, int y)
{
	m_PosX = x;
	m_PosY = y;

	if (m_bCenter)
	{
		int x_size = m_Width;
		m_StringPosX = m_PosX - x_size/2;
		m_StringPosY = m_PosY;
	}
	else
	{
		m_StringPosX = m_PosX;
		m_StringPosY = m_PosY;
	}
}


int CTextOutMultiBox::GetBoxHeight()
{
	if (m_pSurf)
	{
		return m_pSurf->GetHeight();
	}
	else
	{
		return -1;
	}
}

int CTextOutMultiBox::GetBoxWidth()
{
	if (m_pSurf)
	{
		return m_pSurf->GetWidth();
	}
	else
	{
		return -1;
	}
}

void CTextOutMultiBox::Draw()
{
	Draw(m_StringPosX, m_StringPosY);
}

void CTextOutMultiBox::Draw(int x, int y)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, m_pSurf);
}


void CTextOutMultiBox::DrawText(int y, BOOL bOver)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	m_StringPosY = y;

	if(!bOver)
		g_pDisplay->Blt(m_StringPosX, y + pCMyApp->Font_AddPosY, m_pSurf);
	else
		g_pDisplay->Blt(m_StringPosX, y + pCMyApp->Font_AddPosY, m_pOverSurf);
}

BOOL CTextOutMultiBox::DrawClipText(int y, int nMinArea, int nMaxArea, int nScrollPos, BOOL bOver /* = FALSE */)
{
	if(m_pSurf == NULL)
		return FALSE;

	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = m_pSurf->GetWidth();
	src.bottom = m_pSurf->GetHeight();

	dest.left   = m_StringPosX;
	dest.top    = y + pCMyApp->Font_AddPosY;
	dest.right  = m_StringPosX + m_pSurf->GetWidth();
	dest.bottom = y + pCMyApp->Font_AddPosY + m_pSurf->GetHeight();

	if(dest.top - nScrollPos <= nMinArea &&
			dest.bottom - nScrollPos <= nMinArea)	// 모두 밖에 있음..
	{
		return FALSE;
	}
	else if(dest.top - nScrollPos <= nMinArea &&		// 윗 영역에 걸쳤음.
			dest.bottom - nScrollPos >= nMinArea)
	{
		src.top  = abs((dest.top - nScrollPos) - nMinArea );
		dest.top = nMinArea;
		dest.bottom = (dest.bottom - nScrollPos);
	}
	else if(dest.top - nScrollPos >= nMinArea &&		// 모두 영역 안에 있음.
			dest.bottom - nScrollPos <= nMaxArea/*BOUNDARY_LINE_POSY_TOP*/)
	{
		dest.left   = m_StringPosX;
		dest.top    = y + pCMyApp->Font_AddPosY - nScrollPos;
		dest.right  = m_StringPosX + m_pSurf->GetWidth();
		dest.bottom = dest.top + m_pSurf->GetHeight();
	}
	else if(dest.top - nScrollPos <= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( nMaxArea - (dest.top - nScrollPos) );
		dest.top    = (dest.top- nScrollPos);
		dest.bottom = nMaxArea;
	}
	else if(dest.top - nScrollPos >= nMaxArea &&
			dest.bottom - nScrollPos >= nMaxArea) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}

	g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, m_pSurf, &src);

	return TRUE;
}


void CTextOutMultiBox::Draw(RECT rc, int x, int y)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, m_pSurf, &rc);
}

void CTextOutMultiBox::Draw(int x, int y, int dest_cx)
{
	if (!m_bVisible || !g_pDisplay || !m_pSurf)
		return;

	x = x + (dest_cx/2) - (m_Width/2);

	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, m_pSurf);
}

//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font 중 하나를 받아온다.

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경

void CTextOutMultiBox::SetFont (int f_w, int f_h, char *fontname)
{
	m_FontWidth = f_w;
	m_FontHeight = f_h;

	// Font 생성
	m_Font = G_GetFont(f_h,0,fontname);
}