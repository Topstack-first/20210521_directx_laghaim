#ifndef _CLR_ALLOC_H_
#define _CLR_ALLOC_H_

#include "Config.h"
#include <deque>

#define MAX_COLOR_ALLOC 32767 // 2^15 - 1

typedef struct
{
	WORD B:5;
	WORD G:5;
	WORD R:5;
	WORD N:1;
} RGB555;

typedef struct
{
	WORD B:5;
	WORD G:6;
	WORD R:5;
} RGB565;

typedef union
{
	RGB555 Bit555;
	RGB565 Bit565;
	WORD  Color;
} HICOLOR;

typedef struct
{
	BYTE B;
	BYTE G;
	BYTE R;
	BYTE A;
} ARGB8888;

typedef union
{
	ARGB8888 BYTE8888;
	DWORD dwColor;
} TRUECOLOR;


// �� Ŭ������ �ϴ� ���� ������ �Ҵ����ִ� �� ���̴�.
// ������ ������ ������ 16bit Į���� �Ѱ��ָ� �ȴ�.
// RGB565���� ������ �ʿ䰡 ����.

typedef std::deque<WORD>						WordDequeColorkey;
typedef	WordDequeColorkey::iterator				WordDequeColorkey_it;

class CClrAlloc
{
	WordDequeColorkey	m_ColorKeyDeque;
	bool				m_bUse[MAX_COLOR_ALLOC];
	BOOL				m_bRGB565;
public:
	CClrAlloc(BOOL bRGB565 = TRUE);
	virtual ~CClrAlloc();

public:	
	WORD			m_wCurIdx;
	unsigned short	m_unMaxColorValue;				// �������� �����Ҷ� �ִ밪.
	unsigned short	m_unMinColorValue;				// �������� �����Ҷ� �ּҰ�

	// �Ҵ�� ����.
	void ColorAlloc(DWORD *dwColor, WORD *wColor);
	void ColorFree(WORD wColor);
	void ReAlloc();

	WORD ConvertDWordColorToWord(DWORD dwColor);
	WORD ConvertRGB24ToWord(BYTE R, BYTE G, BYTE B);
};

#endif // _CLR_ALLOC_H_
