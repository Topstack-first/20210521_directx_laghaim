#include "stdafx.h"
// ProgressBox.cpp: implementation of the CProgressBox class.
//
//////////////////////////////////////////////////////////////////////


#include "ProgressBox.h"
#include "ddutil.h"
#include "main.h"
#include "LoadEffectSound.h"



extern char g_strFontName[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProgressBox::CProgressBox()
	: m_PosX(0)
	, m_PosY(0)
	, m_Width(0)
	, m_Height(0)
	, m_TextColor(0)
	, m_FontSize(0)
{
	m_pBackSur = NULL;
	m_pProgressSur = NULL;
	m_Font = NULL;

	SetMaxPos(100);
	SetCurPos(0);

	m_bVisible = TRUE;
}


//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
//-- Destructor

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CProgressBox::~CProgressBox()
{
	DeleteRes();
}

void CProgressBox::DeleteRes()
{
	if (m_pBackSur)
		delete m_pBackSur;
	if (m_pProgressSur)
		delete m_pProgressSur;

	m_pBackSur = NULL;
	m_pProgressSur = NULL;
}

void CProgressBox::LoadRes( int x, int y, int width, int height, char *backFile, char *progressFile )
{
	if( progressFile == NULL )
		return;

	DeleteRes();

	m_PosX = x;
	m_PosY = y;
	m_Width = width;
	m_Height = height;

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, backFile, width, height);//, width, height );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pProgressSur, progressFile, width, height);//, width, height );
}

void CProgressBox::Draw()
{
	if (!m_bVisible || !g_pDisplay || !m_pBackSur || !m_pProgressSur)
		return;

	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = m_pBackSur->GetWidth() * m_CurPos / m_MaxPos;
	rect.bottom = m_pBackSur->GetHeight();

	g_pDisplay->Blt( m_PosX, m_PosY, m_pProgressSur, &rect);

	rect.left = rect.right;
	rect.right = m_pBackSur->GetWidth();

	if(rect.left != rect.right)
	{
		g_pDisplay->Blt( m_PosX + rect.left, m_PosY, m_pBackSur, &rect);
	}

	if (!m_Font)
		return;

	CSurface *Surface;
	char str_temp[100];

	sprintf(str_temp, "%d/%d", m_CurPos, m_MaxPos);
	g_pDisplay->CreateSurfaceFromText(&Surface, m_Font, str_temp, RGB(0, 0, 1), m_TextColor);
	Surface->SetColorKey(RGB(0, 0, 1));

	int x = m_PosX + (m_Width - (strlen(str_temp) * m_FontSize / 2)) / 2;
	int y = m_PosY + (m_Height - m_FontSize) / 2;
	g_pDisplay->Blt(x, y + pCMyApp->Font_AddPosY, Surface);

	delete Surface;
}

void CProgressBox::SetValues(int maxPos, int curPos, COLORREF textColor, int fontSize)
{
	if (maxPos == 0)
		SetMaxPos(1);
	else
		SetMaxPos(maxPos);
	SetCurPos(curPos);
	m_TextColor = textColor;

	SetFont (fontSize/2, fontSize, g_strFontName);
}



//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font 중 하나를 받아온다.

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경
void CProgressBox::SetFont (int f_w, int f_h, char *fontname)
{
	if( fontname == NULL )
		return;
	// Font 생성
	m_Font = G_GetFont(f_h,f_w,fontname);
	/*m_Font = CreateFont (f_h, f_w,	// 세로, 가로 크기
						0, 0,		// 기울어짐 각도
						FW_NORMAL,	// 폰트의 굵기
						FALSE,		// 이탤릭 플래그
						FALSE,		// 언더라인 플래그
						FALSE,		// Strikeout 플래그
						DEFAULT_CHARSET,		// Char SET
						OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY,
						DEFAULT_PITCH,
						fontname);
	*/
	// Line Height 계산 요망..
	//LineHeight = f_h+2;		// 2 pixel 더...
	//MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

	// Bar Position 변경
	//CalculateBarPos ();
	m_FontSize = f_h;
}
