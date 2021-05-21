// ClrAlloc.h: interface for the CClrAlloc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLRALLOC_H__7DD768F7_259F_46FB_83C5_14F5A65E52E7__INCLUDED_)
#define AFX_CLRALLOC_H__7DD768F7_259F_46FB_83C5_14F5A65E52E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_COLOR_ALLOC 32767 // 2^15 - 1

typedef struct _RGB555
{
	WORD B:5;
	WORD G:5;
	WORD R:5;
	WORD N:1;

	_RGB555()
		:	B(0)
		,	G(0)
		,	R(0)
		,	N(0)
	{
	}


} RGB555;

typedef struct _RGB565
{
	WORD B:5;
	WORD G:6;
	WORD R:5;

	_RGB565()
		:	B(0)
		,	G(0)
		,	R(0)
	{
	}
} RGB565;

typedef union
{
	RGB555 Bit555;
	RGB565 Bit565;
	WORD  Color;
} HICOLOR;

typedef struct _ARGB8888
{
	BYTE B;
	BYTE G;
	BYTE R;
	BYTE A;

	ARGB8888()
		:	B(0)
		,	G(0)
		,	R(0)
		,	A(0)
	{
	}
} ARGB8888;

typedef union
{
	ARGB8888 BYTE8888;
	DWORD dwColor;
} TRUECOLOR;


// �� Ŭ������ �ϴ� ���� ������ �Ҵ����ִ� �� ���̴�.
// ������ ������ ������ 16bit Į���� �Ѱ��ָ� �ȴ�.
// RGB565���� ������ �ʿ䰡 ����.

class CClrAlloc
{
	BOOL			m_bRGB565;
public:
	CClrAlloc(BOOL bRGB565 = TRUE);
	virtual ~CClrAlloc();

public:
	unsigned char	*m_pcColorUsed;
	WORD			m_wCurIdx;
	unsigned short	m_unMaxColorValue;				// �������� �����Ҷ� �ִ밪.
	unsigned short	m_unMinColorValue;				// �������� �����Ҷ� �ּҰ�

	// �Ҵ�� ����.
	void ColorAlloc(DWORD *dwColor, WORD *wColor);
	void ColorFree(WORD wColor);

	WORD ConvertDWordColorToWord(DWORD dwColor);
	WORD ConvertRGB24ToWord(BYTE R, BYTE G, BYTE B);
};

#endif // !defined(AFX_CLRALLOC_H__7DD768F7_259F_46FB_83C5_14F5A65E52E7__INCLUDED_)
