#include "stdafx.h"
#include "ClrAlloc.h"
#include "main.h"

CClrAlloc::CClrAlloc(BOOL bRGB565)
{
	m_bRGB565 = bRGB565;
	m_unMaxColorValue = MAX_COLOR_ALLOC;
	m_unMinColorValue = 1;
	m_wCurIdx = m_unMinColorValue;
	
	ZeroMemory(m_bUse, sizeof(bool)*MAX_COLOR_ALLOC);
	for( int i = 0; i < MAX_COLOR_ALLOC; i++ )
	{	
		m_ColorKeyDeque.push_back(i+1);
	}
}

CClrAlloc::~CClrAlloc()
{
	m_ColorKeyDeque.clear();
}

void CClrAlloc::ReAlloc()
{
	if( !m_ColorKeyDeque.empty() )
		return;

	m_ColorKeyDeque.clear();

	ZeroMemory(m_bUse, sizeof(bool)*MAX_COLOR_ALLOC);
	for( int i = 0; i < MAX_COLOR_ALLOC; i++ )
	{
		m_ColorKeyDeque.push_back(i+1);
	}
}

void CClrAlloc::ColorAlloc(DWORD *dwColor, WORD *wColor)
{
	HICOLOR hicolor;
	unsigned char r24, g24, b24;

	WordDequeColorkey_it itor = m_ColorKeyDeque.begin();

	if( !m_ColorKeyDeque.empty() )
	{
		hicolor.Color = *wColor = (*itor);
		m_ColorKeyDeque.pop_front();
	}

	// 변환.
	if (m_bRGB565)
	{
		r24 = hicolor.Bit565.R << 3;
		g24 = hicolor.Bit565.G << 2;
		b24 = hicolor.Bit565.B << 3;
	}
	else
	{
		r24 = hicolor.Bit555.R << 3;
		g24 = hicolor.Bit555.G << 3;
		b24 = hicolor.Bit555.B << 3;
	}

	if (m_bRGB565 && pCMyApp->GetCurResolution() == 16)
	{
		if( pCMyApp->m_DrvType == INTEL_DRV )
			*dwColor = ( ((r24+4) << 16) | ((g24+2) << 8) | ((b24+4) << 0) );
		else
			*dwColor = ( (r24 << 16) | (g24 << 8) | (b24 << 0) );
	}
	else if(m_bRGB565 && pCMyApp->GetCurResolution() == 32)
	{
		if( pCMyApp->m_DrvType == XABRE_DRV )
			*dwColor = ( ((r24+1) << 16) | ((g24+1) << 8) | ((b24+1) << 0) );
		else
			*dwColor = ( (r24 << 16) | (g24 << 8) | (b24 << 0) );
	}
	else
	{
		*dwColor = 0;
		*dwColor |= r24;
		*dwColor = *dwColor << 8;
		*dwColor |= g24;
		*dwColor = *dwColor << 8;
		*dwColor |= b24;
	}
}

void CClrAlloc::ColorFree(WORD wColor)
{
	if (wColor >= MAX_COLOR_ALLOC)
	{
		wColor = wColor & 0x7FFF;
		OutputDebugString("wColor >= MAX_COLOR_ALLOC\n");
	}

	m_ColorKeyDeque.push_back(wColor);
}

// ConvertDWordColorToWord()
// Desc : 32bit 해상도에서 16bit 인덱스로 바꾸는 루틴.
WORD CClrAlloc::ConvertDWordColorToWord(DWORD dwColor)
{
	HICOLOR hicolor;
	TRUECOLOR truecolor;
	BYTE R, G, B;

	truecolor.dwColor = dwColor;
	hicolor.Color = 0;
	B = truecolor.BYTE8888.B;
	G = truecolor.BYTE8888.G;
	R = truecolor.BYTE8888.R;

	if (m_bRGB565)
	{
		hicolor.Bit565.B = B >> 3;
		hicolor.Bit565.G = G >> 2;
		hicolor.Bit565.R = R >> 3;
	}
	else
	{
		hicolor.Bit555.B = B >> 3;
		hicolor.Bit555.G = G >> 3;
		hicolor.Bit555.R = R >> 3;
	}

	return hicolor.Color;
}

// ConvertRGB24ToWord()
// Desc : 24bit 해상도에서 16bit 인덱스로 바꾸는 루틴.
WORD CClrAlloc::ConvertRGB24ToWord(BYTE R, BYTE G, BYTE B)
{
	HICOLOR hicolor;

	hicolor.Color = 0;

	if (m_bRGB565)
	{
		hicolor.Bit565.B = B >> 3;
		hicolor.Bit565.G = G >> 2;
		hicolor.Bit565.R = R >> 3;
	}
	else
	{
		hicolor.Bit555.B = B >> 3;
		hicolor.Bit555.G = G >> 3;
		hicolor.Bit555.R = R >> 3;
	}

	return hicolor.Color;
}