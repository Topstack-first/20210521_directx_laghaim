#include "stdafx.h"
#define D3D_OVERLOADS
#include <math.h>
#include <d3d.h>
#include "d3dutil.h"
#include "AlphaPopUp.h"
#include "IndexedTexture.h"
#include "LoadEffectSound.h"
#include "textbox.h"
#include "main.h"
#include "Country.h"
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"



extern BOOL g_bFixedWidthFont;
extern DWORD g_dwLangType;

extern int g_EffectTextr[];
extern char g_strFontName[];
//----------------------------------------------------------------------
//		DEFINE
//----------------------------------------------------------------------

CMultiText::CMultiText()
	: m_LineCount(0)
{
	memset(m_TextColor, 0, sizeof(m_TextColor));
	memset(m_TextBackColor, 0, sizeof(m_TextBackColor));
	memset(m_Text, 0, sizeof(m_Text));
}

void CMultiText::Clone(CMultiText& multiText)
{
	for(int i = 0; i < multiText.m_LineCount; ++i)
	{
		AddString(multiText.m_Text[i], multiText.m_TextColor[i], multiText.m_TextBackColor[i]);
	}
}

CMultiText::~CMultiText()
{
	Reset();
}

void CMultiText::Reset()
{
	for (int i = 0; i < m_LineCount; i++)
	{
		SAFE_DELETE_ARRAY(m_Text[i]);
		m_TextColor[i] = 0;
		m_TextBackColor[i] = 0;
	}

	m_LineCount = 0;
}

// nate
BOOL CMultiText::AddString(char *string, DWORD color, DWORD back_color)
// 입력받은 문자열을 현재 출력할 라인에 복사하고 라인증가
{
	if ( m_LineCount >= MAX_TEXT_LINE || !string )
		return FALSE;

	m_Text[ m_LineCount ] = new char[ strlen(string) + 1 ];	// 출력될 문자열
	strcpy( m_Text[ m_LineCount ], string );	// 문자열 복사
	m_TextColor[ m_LineCount ] = color;			// 문자 색 설정
	m_TextBackColor[ m_LineCount ] = back_color;	// 배경색 설정
	m_LineCount++;	// 라인 카운트 1 증가

	return TRUE;
}

// nate
//////////////////////////////////////////////////////////////////////////
// 단어 단위로 줄 바꿈. [2/6/2007 Theodoric]
void CMultiText::AddStringMulti(char *string, int len, BOOL flag, DWORD color, DWORD back_color)// ???? ???? ??? ???? ??
{
	if (!string)
		return;

	int descr_len = strlen(string);

	if (descr_len <= 0)
		return;

	if (len > 100)
		len = 100;	// ?? ?? ??

	int		pos = 0;
	int		idx = 0;
	int		idx_tmp = 0;
	int		CheckPoint = 0;
	BOOL	is_han = FALSE;
	char	descr_buf[102] = {0,};
	char	print_buf[102] = {0,};

	while (pos < descr_len)// ?? ?? ?? ??
	{
		idx = 0;
		is_han = FALSE;
		CheckPoint = 0;

		ZeroMemory(descr_buf, sizeof(descr_buf));
		ZeroMemory(print_buf, sizeof(print_buf));

		while(1)// ?? ??? ??? ?? ????.
		{
			BOOL bBreakTime = FALSE;

			switch( g_dwLangType )
			{
			case LANG_JPN:	// ??
				if( string[pos] == 0x20)  // ?? ????.
				{
					pos++;
				}
				else if( string[pos] == -127 && string[pos+1] == 0x40)  // ?? ????
				{
					pos += 2;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;

			case LANG_TWN:	// ??
				if( string[pos] == 0x20)  // ?? ????.
				{
					pos++;
				}
				else if( string[pos] == -95 && string[pos+1] == 0x40)  // ?? ????
				{
					pos += 2;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;

			case LANG_CN:	// ??
				if( string[pos] == 0x20)  // ?? ????.
				{
					pos++;
				}
				else if( string[pos] == -95 && string[pos+1] == -95)  // ?? ????
				{
					pos += 2;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;

			default:
				if( string[pos] == 0x20)
				{
					pos++;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;
			}
			if(bBreakTime)
			{
				break;
			}
		}

		while( (idx < len || is_han) && pos < descr_len ) // ??? ?? ?? ?? ? ? ??.
		{
			// ?? ?? ??? ?? ??.
			switch( g_dwLangType)
			{
			case LANG_JPN:	// ??
				if((string[pos-1] != 0x20  && string[pos] == 0x20)		// ????.
						|| ((string[pos-2] != -127 && string[pos-1] != 0x40)	// ????.
							&& (string[pos] == -127 && string[pos+1] == 0x40)))
				{
					CheckPoint = idx;
				}
				break;

			case LANG_TWN:	// ??
				if((string[pos-1] != 0x20  && string[pos] == 0x20)		// ????.
						|| ((string[pos-2] != -95 && string[pos-1] != 0x40)		// ????.
							&& (string[pos] == -95 && string[pos+1] == 0x40)))
				{
					CheckPoint = idx;
				}
				break;

			case LANG_CN:  // ??
				if((string[pos-1] != 0x20  && string[pos] == 0x20)		// ????.
						|| (string[pos-1] != -95 && string[pos] == -95))		// ????.
				{
					CheckPoint = idx;
				}
				break;

			default:
				if(string[pos-1] != 0x20  && string[pos] == 0x20)
				{
					CheckPoint = idx;
				}
				break;
			}

			if( string[pos] == 0x0D && string[pos+1] == 0x0A )
			{
				pos++;
				CheckPoint = idx;
				break;
			}

			if (string[pos] < 0 || string[pos] > 127)
			{
				if (is_han) // ???? ??
				{
					is_han = FALSE;
				}
				else
				{
					is_han = TRUE;
				}
			}
			else
			{
				is_han = FALSE;
			}

			// ??? ??, ??? ??
			descr_buf[idx++] = string[pos++];
		}

		if( CheckPoint > 0 && pos < descr_len )  // ? ??? ??? ??? ?? ??? ?? ??.
		{
			memcpy(print_buf, descr_buf, CheckPoint);
			pos = pos - (idx - CheckPoint) +1;
			switch( g_dwLangType )
			{
			case LANG_JPN:
				if( string[pos-1] == -127 )
				{
					pos ++;
				}
				break;	// ??
			case LANG_TWN:
				if( string[pos-1] == -95  )
				{
					pos ++;
				}
				break;	// ??
			case LANG_CN:
				if( string[pos-1] == -95  )
				{
					pos ++;
				}
				break;	// ??
			}
			idx = CheckPoint;
		}
		else
		{
			memcpy(print_buf, descr_buf, idx);
		}

		print_buf[strlen(print_buf)+1] = '\0';
		AddString(print_buf, color);
	}
}
//////////////////////////////////////////////////////////////////////////


BOOL CMultiText::AddStringEx(char *string, DWORD color, DWORD back_color) // 개행 문자를 인식하는 녀석. ///아이템설명입력
// 입력받은 문자열을 현재 출력할 라인에 복사하고 라인증가
{
	// 우선 스트링에서 개행문자가 나올때까지 읽는다.
	char strTemp[256];
	int read_position = 0; // 읽고있는 위치.
	int new_line_position = 0; // 이전 개행한 위치.
	int string_len = strlen( string );
	int LineLen = 0;

	for( int i = 0 ; i < string_len ; ++i )
	{
		if( string[read_position] == '(' && read_position != string_len-2 )
		{
			if( string[read_position+1] == 'n' && string[read_position+2] == ')' ) // \n 이면..
			{
				// 여기까지를 한줄로 입력.
				LineLen = read_position-new_line_position; // 이번에 입력할 줄의 길이.
				if( LineLen > 255 ) // 너무 길면 캔슬
					return FALSE;

				strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// 문자열 복사
				strTemp[LineLen] = '\0'; // 널문자 삽입.

				AddString(strTemp, color, back_color);

				read_position += 2; // 개행문자 건너뛰기. 원래 +3해야하지만 여길 지난후 ++하니까 2만...
				new_line_position = read_position+1; // 이전 개행한 위치를 기억...
			}
		}
		++read_position;
	}

	// 마지막줄 입력(개행이 없으면 이게 처음이자 마지막줄)
	LineLen = read_position-new_line_position; // 이번에 입력할 줄의 길이.
	if( LineLen > 255 ) // 너무 길면 캔슬
		return FALSE;

	strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// 문자열 복사
	strTemp[LineLen] = '\0'; // 널문자 삽입.

	AddString(strTemp, color, back_color);

	return TRUE;
}

CMultiText2::CMultiText2() ///공지팝업창
	: m_LineCount(0)
{
	memset(m_Text, 0, sizeof(m_Text));
	memset(m_TextColor, 0, sizeof(m_TextColor));
	memset(m_TextBackColor, 0, sizeof(m_TextBackColor));
}


CMultiText2::~CMultiText2()
{
	Reset();
}

void CMultiText2::Reset()
{
	for (int i = 0; i < m_LineCount; i++)
	{
		SAFE_DELETE_ARRAY(m_Text[i]);
		m_TextColor[i] = 0;
		m_TextBackColor[i] = 0;
	}

	m_LineCount = 0;
}

BOOL CMultiText2::AddString(char *string, DWORD color, DWORD back_color)
// 입력받은 문자열을 현재 출력할 라인에 복사하고 라인증가
{
	if ( m_LineCount >= MAX_TEXT_LINE2 || !string )
		return FALSE;

	m_Text[ m_LineCount ] = new char[ strlen(string) + 1 ];	// 출력될 문자열
	strcpy( m_Text[ m_LineCount ], string );	// 문자열 복사
	m_TextColor[ m_LineCount ] = color;			// 문자 색 설정
	m_TextBackColor[ m_LineCount ] = back_color;	// 배경색 설정
	m_LineCount++;	// 라인 카운트 1 증가

	return TRUE;
}

void CMultiText2::AddStringMulti(char *string, int len, BOOL flag, DWORD color, DWORD back_color)// ???? ???? ??? ???? ??
{
	if (!string)
		return;

	int descr_len = strlen(string);

	if (descr_len <= 0)
		return;

	if (len > 100)
		len = 100;	// ?? ?? ??

	int		pos = 0;
	int		idx = 0;
	int		idx_tmp = 0;
	int		CheckPoint = 0;
	BOOL	is_han = FALSE;
	char	descr_buf[102] = {0,};
	char	print_buf[102] = {0,};

	while (pos < descr_len)// ?? ?? ?? ??
	{
		idx = 0;
		is_han = FALSE;
		CheckPoint = 0;

		ZeroMemory(descr_buf, sizeof(descr_buf));
		ZeroMemory(print_buf, sizeof(print_buf));

		while(1)// ?? ??? ??? ?? ????.
		{
			BOOL bBreakTime = FALSE;

			switch( g_dwLangType )
			{
			case LANG_JPN:	// ??
				if( string[pos] == 0x20)  // ?? ????.
				{
					pos++;
				}
				else if( string[pos] == -127 && string[pos+1] == 0x40)  // ?? ????
				{
					pos += 2;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;

			case LANG_TWN:	// ??
				if( string[pos] == 0x20)  // ?? ????.
				{
					pos++;
				}
				else if( string[pos] == -95 && string[pos+1] == 0x40)  // ?? ????
				{
					pos += 2;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;

			case LANG_CN:	// ??
				if( string[pos] == 0x20)  // ?? ????.
				{
					pos++;
				}
				else if( string[pos] == -95 && string[pos+1] == -95)  // ?? ????
				{
					pos += 2;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;

			default:
				if( string[pos] == 0x20)
				{
					pos++;
				}
				else
				{
					bBreakTime = TRUE;
				}
				break;
			}
			if(bBreakTime)
			{
				break;
			}
		}

		while( (idx < len || is_han) && pos < descr_len ) // ??? ?? ?? ?? ? ? ??.
		{
			// ?? ?? ??? ?? ??.
			switch( g_dwLangType)
			{
			case LANG_JPN:	// ??
				if((string[pos-1] != 0x20  && string[pos] == 0x20)		// ????.
						|| ((string[pos-2] != -127 && string[pos-1] != 0x40)	// ????.
							&& (string[pos] == -127 && string[pos+1] == 0x40)))
				{
					CheckPoint = idx;
				}
				break;

			case LANG_TWN:	// ??
				if((string[pos-1] != 0x20  && string[pos] == 0x20)		// ????.
						|| ((string[pos-2] != -95 && string[pos-1] != 0x40)		// ????.
							&& (string[pos] == -95 && string[pos+1] == 0x40)))
				{
					CheckPoint = idx;
				}
				break;

			case LANG_CN:  // ??
				if((string[pos-1] != 0x20  && string[pos] == 0x20)		// ????.
						|| (string[pos-1] != -95 && string[pos] == -95))		// ????.
				{
					CheckPoint = idx;
				}
				break;

			default:
				if(string[pos-1] != 0x20  && string[pos] == 0x20)
				{
					CheckPoint = idx;
				}
				break;
			}

			if (string[pos] < 0 || string[pos] > 127)
			{
				if (is_han) // ???? ??
				{
					is_han = FALSE;
				}
				else
				{
					is_han = TRUE;
				}
			}
			else
			{
				is_han = FALSE;
			}

			// ??? ??, ??? ??
			descr_buf[idx++] = string[pos++];
		}

		if( CheckPoint > 0 && pos < descr_len )  // ? ??? ??? ??? ?? ??? ?? ??.
		{
			memcpy(print_buf, descr_buf, CheckPoint);
			pos = pos - (idx - CheckPoint) +1;
			switch( g_dwLangType )
			{
			case LANG_JPN:
				if( string[pos-1] == -127 )
				{
					pos ++;
				}
				break;	// ??
			case LANG_TWN:
				if( string[pos-1] == -95  )
				{
					pos ++;
				}
				break;	// ??
			case LANG_CN:
				if( string[pos-1] == -95  )
				{
					pos ++;
				}
				break;	// ??
			}
			idx = CheckPoint;
		}
		else
		{
			memcpy(print_buf, descr_buf, idx);
		}

		print_buf[strlen(print_buf)+1] = '\0';
		AddString(print_buf, color);
	}
}

BOOL CMultiText2::AddStringEx(char *string, DWORD color, DWORD back_color) // 개행 문자를 인식하는 녀석. ///아이템설명입력
// 입력받은 문자열을 현재 출력할 라인에 복사하고 라인증가
{
	// 우선 스트링에서 개행문자가 나올때까지 읽는다.
	char strTemp[256];
	int read_position = 0; // 읽고있는 위치.
	int new_line_position = 0; // 이전 개행한 위치.
	int string_len = strlen( string );
	int LineLen = 0;

	for( int i = 0 ; i < string_len ; ++i )
	{
		if( string[read_position] == '(' && read_position != string_len-2 )
		{
			if( string[read_position+1] == 'n' && string[read_position+2] == ')' ) // \n 이면..
			{
				// 여기까지를 한줄로 입력.
				LineLen = read_position-new_line_position; // 이번에 입력할 줄의 길이.
				if( LineLen > 255 ) // 너무 길면 캔슬
					return FALSE;

				strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// 문자열 복사
				strTemp[LineLen] = '\0'; // 널문자 삽입.

				AddString(strTemp, color, back_color);

				read_position += 2; // 개행문자 건너뛰기. 원래 +3해야하지만 여길 지난후 ++하니까 2만...
				new_line_position = read_position+1; // 이전 개행한 위치를 기억...
			}
		}
		++read_position;
	}

	// 마지막줄 입력(개행이 없으면 이게 처음이자 마지막줄)
	LineLen = read_position-new_line_position; // 이번에 입력할 줄의 길이.
	if( LineLen > 255 ) // 너무 길면 캔슬
		return FALSE;

	strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// 문자열 복사
	strTemp[LineLen] = '\0'; // 널문자 삽입.

	AddString(strTemp, color, back_color);

	return TRUE;
}


//----------------------------------------------------------------------
//		CONSTRUCTOR / DESTRUCTOR
//----------------------------------------------------------------------
CAlphaPopUp::CAlphaPopUp()
	: m_pTextSur(NULL)
	, m_Font(NULL)
	, m_Index(-1)
	, m_ScreenWidth(800)
	, m_ScreenHeight(600)
	, m_bRectSoft(FALSE)
	, m_GemNum(0)
	, m_alpha_type(AlphaPopUp_Type__Default)
	, m_FontWidth(0)
	, m_FontHeight(0)
	, m_Xpos(0)
	, m_Ypos(0)
	, m_TextSurWidth(0)
	, m_TextSurHeight(0)
	, m_MainActive(false)
	, m_TextColor(0)
	, m_TextBackColor(0)
	, m_nGuildNameXPos(0)
	, m_nGuildNameYPos(0)
{
}

//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
//-- Destructor

//-- 2004.03.12 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CAlphaPopUp::~CAlphaPopUp()
{
	DeleteRes();
	SAFE_DELETE( m_pTextSur );
}

//----------------------------------------------------------------------
//		INITIALIZE
//----------------------------------------------------------------------
void CAlphaPopUp::Init(int FontSize, AlphaPopUp_Type type)
{
	SetFont(FontSize/2, FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	if( pCMyApp->Font_AddPosY == 0 )
	{
		m_FontWidth  = sizeFont.cx;
		m_FontHeight = sizeFont.cy+4;
	}
	else
	{
		m_FontWidth  = sizeFont.cx;
		m_FontHeight = sizeFont.cy+1;
	}

	m_MainActive = FALSE;
	m_alpha_type = type;
}



//----------------------------------------------------------------------
//		LOAD RESOURCE
//----------------------------------------------------------------------
void CAlphaPopUp::LoadRes(int screenWidth, int screenHeight)
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
}



//----------------------------------------------------------------------
//		DELETE
//----------------------------------------------------------------------
void CAlphaPopUp::DeleteRes()
{
	SAFE_SURFACE_DESTROY(m_pTextSur);
	m_Index = -1;
}



//----------------------------------------------------------------------
//		DRAW
//----------------------------------------------------------------------
// nate
void CAlphaPopUp::Draw( int Xpos, int Ypos, BYTE byMedalFlag )
// 팝업창을 그린다.
// 팝업창은 캐릭이 멀리있어도 같은 크기로 보인다.
// byMedalflag =>	0: 팝업창 위치조정 + 박스 + 텍스트
//					1: 텍스트
//					2: 위치조정 + 박스
{
	if(m_MainActive == TRUE && m_pTextSur)
	{
		if( byMedalFlag == 3 ) // 개인상점용 판때기 배경.
		{
			CalcPos(Xpos, Ypos);
			RenderOj( 3 );
		}
		else if( byMedalFlag == 4 )
		{
			CalcPos(Xpos, Ypos);
			RenderOj( 4 );
		}
		else if( byMedalFlag != 1 )
		{
			CalcPos(Xpos, Ypos);
			RenderOj();
		}

		if( byMedalFlag != 2 )
		{
			m_pTextSur->GetWidth();
			m_pTextSur->GetHeight();
			g_pDisplay->Blt( m_Xpos, m_Ypos + pCMyApp->Font_AddPosY, m_pTextSur);
		}
	}
}


// nate
void CAlphaPopUp::CalcPos(int x, int y)
// 팝업창의 맵버 x, y 셋팅
{
	// BOX_GAP : 채팅창에 글씨가 꽉차면 보기않좋기 때문에 갭을 준다.
	if (x + (m_TextSurWidth / 2) + BOX_GAP >= m_ScreenWidth)
		m_Xpos = m_ScreenWidth - m_TextSurWidth - BOX_GAP;
	else
		m_Xpos = x - m_TextSurWidth / 2;

	if (y + m_TextSurHeight + BOX_GAP >= m_ScreenHeight)
		m_Ypos = m_ScreenHeight - m_TextSurHeight - BOX_GAP;
	else if (y - m_TextSurHeight - BOX_GAP <= 0)
		m_Ypos = y;
	else
		m_Ypos = y - m_TextSurHeight;

	if (m_Xpos < 0)
		m_Xpos = 0;
	if (m_Ypos < 0)
		m_Ypos = 0;

	if( m_alpha_type == AlphaPopUp_Type__Char )
		return;

	if( x - m_TextSurWidth < 0 )
	{
		m_Xpos = 0+5;
	}
	else
	{
		m_Xpos = x - m_TextSurWidth;
	}

	if( m_Xpos + m_TextSurWidth > m_ScreenWidth )
	{
		m_Xpos = m_ScreenWidth - m_TextSurWidth-5;
	}


	if( y - m_TextSurHeight - 5 < 0 )
	{
		m_Ypos = 0+5;
	}
	else
	{
		m_Ypos = y - m_TextSurHeight;
	}
	if( m_Ypos + m_TextSurHeight >= m_ScreenHeight )
	{
		m_Ypos = m_ScreenHeight - m_TextSurHeight-10;
	}
}


// nate
void CAlphaPopUp::RenderOj( int CustomTextr )
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	// 프로젝션메트릭스 다시 셋팅
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// 텍스트의 크기만큼 크기설정

	int offsetX = 0;
	if( CustomTextr == 4 )
	{
		offsetX = 54;
	}

	TLVec[0] = D3DTLVERTEX(D3DVECTOR(m_Xpos-BOX_GAP-offsetX,    m_Ypos-BOX_GAP,     0), 1, 0x00ffffff, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR(m_Xpos+m_TextSurWidth+BOX_GAP, m_Ypos-BOX_GAP,     0), 1, 0x00ffffff, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR(m_Xpos-BOX_GAP-offsetX,    m_Ypos+m_TextSurHeight+BOX_GAP,  0), 1, 0x00ffffff, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR(m_Xpos+m_TextSurWidth+BOX_GAP, m_Ypos+m_TextSurHeight+BOX_GAP,  0), 1, 0x00ffffff, 0, 1, 1);


	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00ffffff);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
//	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

	//----------------그리기 시작 / 끝 --------------
	if (m_bRectSoft || SUCCEEDED( GET_D3DDEVICE()->BeginScene()))
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		// 알파테스트 안함
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		if( CustomTextr == 3 ) // 개인상점용 판때기 배경.
		{
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_USHOP_BOARD]);
		}
		else
		{
			if (m_bRectSoft)	//  CNkChacter가 초가화 될때 TRUE 셋팅
				lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_BLACK2]);
			else
				lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_BLACK]);
		}

		GET_D3DDEVICE()->SetTexture(0, lpTexture);

		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		if (!m_bRectSoft)
			GET_D3DDEVICE()->EndScene();
	}

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}

#define GEM_IMG_WIDTH		24
#define GEM_IMG_Y			14
void CAlphaPopUp::DrawImgGem(CSurface* m_pImgGem, int which_gem)
{
	if( !g_pDisplay )
		return;

	int x = m_Xpos + (m_TextSurWidth - (GEM_IMG_WIDTH * m_GemNum)) / 2;
	RECT rcRect;
	rcRect.left = which_gem * GEM_IMG_WIDTH;
	rcRect.right = rcRect.left + GEM_IMG_WIDTH;
	rcRect.top = 0;
	rcRect.bottom = GEM_IMG_WIDTH;

	for (int count = 0; count < m_GemNum; count++)
		g_pDisplay->Blt( x + (count * GEM_IMG_WIDTH), m_Ypos + GEM_IMG_Y, m_pImgGem, &rcRect);
}


// nate
//----------------------------------------------------------------------------------
//		만약 메세지를 받았다면 출력해라!!!
//----------------------------------------------------------------------------------
//void CAlphaPopUp::PutString(int LineCnt, char**String)
void CAlphaPopUp::PutString(CMultiText *pMultiText, BOOL bShadow)
{
	if (!g_pDisplay || !m_Font )	// m_Font 는 핸들이다
		return;

	int GreatestCount=0; // 가장큰 문자열의 개수를 얻는다
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// 가장큰 문자열의 개수를 얻는다
	for(int i = 0; i < pMultiText->m_LineCount; i++)
	{
		// 입력받은 텍스트를 루프를 돌면서 가장긴 문자열 체크
		int len = strlen(pMultiText->m_Text[i]);
		// 문자열의 폭이 고정되었는가?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// 폰트 사이즈 반환
			GetTextExtentPoint32(hdc, pMultiText->m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextSurWidth = size.cx;
		};
	}

	// by evilkiki 2009.12.03
	SAFE_DELETE(m_pTextSur);

	if (g_bFixedWidthFont)
	{
		// 가장긴 문자열과 라인수에 의해서 서피스 생성
		m_TextSurWidth = GreatestCount*m_FontWidth;
		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;
	}
	else
	{
		m_TextSurWidth = TextSurWidth;
		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;

	}

	if( !m_TextSurWidth || !m_TextSurHeight ) // 만약 사이즈 한쪽이 0이면 캔슬. 07-09-05 수정.
		return;

	// 계산된 크기로 서피스 생성
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor를 이용한 textsurface 클리어하기
	m_pTextSur->Clear();

	// 들어온 문자열 가운데로 정렬해서 DrawText
	m_Ypos = 0;
	for( int i=0; i<pMultiText->m_LineCount; i++)
	{
		if (g_bFixedWidthFont)
			m_Xpos =  ((m_TextSurWidth/2) - ((strlen(pMultiText->m_Text[i])*m_FontWidth) /2));
		else
		{
			GetTextExtentPoint32(hdc, pMultiText->m_Text[i], strlen(pMultiText->m_Text[i]), &size);
			m_Xpos =  m_TextSurWidth/2 - size.cx/2;
		}

		if (bShadow)
		{
			//-- Lyul 수정 -- Shadow : 1
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 1 ,RGB(10, 10, 10));
		}
		else
		{
			//-- Lyul 수정 -- Normal : 0
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 0);
		}
		m_Ypos+=m_FontHeight;
	}
	m_pTextSur->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	m_MainActive = TRUE; // 화면에 출력해도 좋다 !!!
}

//-- Global Font 중 하나를 받아온다.
//-- 2004.03.15 - 수정완료
//-- 자체 생성에서 Global 사용으로 변경
void CAlphaPopUp::SetFont (int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h,f_w,fontname);
}

// nate 2004 - 4 : Crash_Miss
void CAlphaPopUp::Draw( int nX, int nY, LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime )
{
	CalcPos_Crash_Miss( nX, nY );
	RenderCrash_Miss( lpSurface, nShowTime );
}

void CAlphaPopUp::Draw2( int nX, int nY, LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime ) ///자유대련 승자,패자 출력용. 판때기 시간이나 움직임이 다르다.
{
	CalcPos_Crash_Miss( nX, nY );
	RenderWinLose( lpSurface, nShowTime );
}

void CAlphaPopUp::RenderCrash_Miss( LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime )
{
	if( !GET_D3DDEVICE() )
		return;
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;
	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);
	int ShowSize = 0;

	if( nShowTime < 60 ) // 까지는 안늘어나게...
		ShowSize = 0;
	else
		ShowSize = nShowTime - 60;

	float nI = (float)nShowTime * 0.01f;
//================================
//	1 ================== 2
//	||				*	||
//	||			*		||
//	||		*			||
//	||	*				||
//	3 ================== 4
//================================

	TLVec[0] = D3DTLVERTEX( D3DVECTOR( m_Xpos - BOX_GAP - ShowSize,
									   m_Ypos - BOX_GAP - ShowSize - ShowSize,
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 0, 0);

	TLVec[1] = D3DTLVERTEX( D3DVECTOR( m_Xpos + CRASH_MISS_WIDTH + BOX_GAP + ShowSize,
									   m_Ypos -  BOX_GAP - ShowSize - ShowSize,
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 1, 0);

	TLVec[2] = D3DTLVERTEX( D3DVECTOR( m_Xpos - BOX_GAP - ShowSize,
									   m_Ypos + CRASH_MISS_HEIGHT + BOX_GAP,// + nShowTime
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 0, 1);

	TLVec[3] = D3DTLVERTEX( D3DVECTOR( m_Xpos + CRASH_MISS_WIDTH + BOX_GAP + ShowSize,
									   m_Ypos + CRASH_MISS_HEIGHT + BOX_GAP,// + nShowTime
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 1, 1);

	DWORD dwFog, dwLight;
	// 라이팅
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLight );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFog );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 알파테스트 안함
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

	DWORD	dwAlphaOp, dwAlphaArg1, dwAlphaArg2;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg1 );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwAlphaArg2 );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwAlphaOp );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg1 );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwAlphaArg2 );

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwLight);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFog );
}

void CAlphaPopUp::RenderWinLose( LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime ) ///자유대련 승자,패자 출력용. 판때기 시간이나 움직임이 다르다.
{
	if( !GET_D3DDEVICE() )
		return;
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;
	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);
	int ShowSize = 0;

	// 크기 조절
	int factor = 50;

	if( (nShowTime/factor)%2 ) // 오르고 내리는 페이즈 조절
		ShowSize = nShowTime%factor / 5;
	else
		ShowSize = -nShowTime%factor / 5;

//	float nI = (float)nShowTime * 0.002f; // 알파 조절
	float nI = 0.0f; // 알파 조절
//================================
//	1 ================== 2
//	||				*	||
//	||			*		||
//	||		*			||
//	||	*				||
//	3 ================== 4
//================================

	TLVec[0] = D3DTLVERTEX( D3DVECTOR( m_Xpos - BOX_GAP,
									   m_Ypos - BOX_GAP - ShowSize ,
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 0, 0);

	TLVec[1] = D3DTLVERTEX( D3DVECTOR( m_Xpos + 70 + BOX_GAP,
									   m_Ypos -  BOX_GAP - ShowSize ,
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 1, 0);

	TLVec[2] = D3DTLVERTEX( D3DVECTOR( m_Xpos - BOX_GAP,
									   m_Ypos + 50 + BOX_GAP - ShowSize,// + nShowTime
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 0, 1);

	TLVec[3] = D3DTLVERTEX( D3DVECTOR( m_Xpos + 70 + BOX_GAP,
									   m_Ypos + 50 + BOX_GAP - ShowSize,// + nShowTime
									   0 ),
							1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f-nI ),
							0, 1, 1);

	DWORD dwFog, dwLight;
	// 라이팅
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLight );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFog );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 알파테스트 안함
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

	DWORD	dwAlphaOp, dwAlphaArg1, dwAlphaArg2;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg1 );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwAlphaArg2 );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwAlphaOp );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg1 );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwAlphaArg2 );

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwLight);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFog );
}

void CAlphaPopUp::CalcPos_Crash_Miss(int x, int y)
// 팝업창의 맵버 x, y 셋팅
{
	y -= 40;
	// BOX_GAP : 채팅창에 글씨가 꽉차면 보기않좋기 때문에 갭을 준다.
	if (x + ( CRASH_MISS_WIDTH / 2)  >= m_ScreenWidth)
		m_Xpos = m_ScreenWidth - CRASH_MISS_WIDTH /2;
	else
		m_Xpos = x - CRASH_MISS_WIDTH / 2;

	if (y + CRASH_MISS_HEIGHT + BOX_GAP >= m_ScreenHeight)
		m_Ypos = m_ScreenHeight - CRASH_MISS_HEIGHT - BOX_GAP;
	else if (y > 0)
		m_Ypos = y;
	else
		m_Ypos = y - CRASH_MISS_HEIGHT;

	if (m_Xpos < 0)
		m_Xpos = 0;
	if (m_Ypos < 0)
		m_Ypos = 0;
}

void CAlphaPopUp::MakeChatPopup(CMultiText *pMultiText, BOOL bShadow, int NameLine)
{
	if (!g_pDisplay || !m_Font )	// m_Font 는 핸들이다
		return;

	int GreatestCount=0; // 가장큰 문자열의 개수를 얻는다
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// 가장큰 문자열의 개수를 얻는다
	for(int i = 0; i < pMultiText->m_LineCount; i++)
	{
		// 입력받은 텍스트를 루프를 돌면서 가장긴 문자열 체크
		int len = strlen(pMultiText->m_Text[i]);
		// 문자열의 폭이 고정되었는가?
		if (g_bFixedWidthFont)
		{
			int ext_len = 0;
			if ( i == NameLine )
				ext_len = len * m_FontWidth + 18;
			else
				ext_len = len * m_FontWidth;

			if(TextSurWidth  < ext_len)
			{
				TextSurWidth = ext_len;
				m_TextSurWidth = ext_len;
			}
		}
		else
		{
			// 폰트 사이즈 반환
			GetTextExtentPoint32(hdc, pMultiText->m_Text[i], len, &size);

			if ( i == NameLine )
				size.cx = len * m_FontWidth + 18;
			else
				size.cx = len * m_FontWidth;

			if (size.cx > TextSurWidth)
			{
				TextSurWidth = size.cx;
				m_TextSurWidth = size.cx;
			}
		};
	}

	SAFE_DELETE(m_pTextSur);
	if (g_bFixedWidthFont)
	{
		// 가장긴 문자열과 라인수에 의해서 서피스 생성
		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;

	}
	else
	{

		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;
	}


	// 계산된 크기로 서피스 생성
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor를 이용한 textsurface 클리어하기
	m_pTextSur->Clear();

	// 들어온 문자열 가운데로 정렬해서 DrawText
	m_Ypos = 0;
	for(int i=0; i<pMultiText->m_LineCount; i++)
	{
		if (g_bFixedWidthFont)
		{
			if (NameLine != i)
			{
				m_Xpos =  ((m_TextSurWidth/2) - ((strlen(pMultiText->m_Text[i])*m_FontWidth) /2));
			}
			else
			{
				m_Xpos =  ((m_TextSurWidth/2) - ((strlen(pMultiText->m_Text[i])*m_FontWidth-18) /2));
				m_nGuildNameXPos = (m_TextSurWidth/2) - ((strlen(pMultiText->m_Text[i])*m_FontWidth+18)/2);
				m_nGuildNameYPos = m_Ypos - 2;
			}
		}
		else
		{
			if (NameLine != i)
			{
				GetTextExtentPoint32(hdc, pMultiText->m_Text[i], strlen(pMultiText->m_Text[i]), &size);
				m_Xpos =  m_TextSurWidth/2 - size.cx/2;
			}
			else
			{
				GetTextExtentPoint32(hdc, pMultiText->m_Text[i], strlen(pMultiText->m_Text[i]), &size);
				m_Xpos =  m_TextSurWidth/2 - ((size.cx-18)/2);
				m_nGuildNameXPos = m_TextSurWidth/2 - ((size.cx+18)/2);
				m_nGuildNameYPos = m_Ypos - 2;
			}
		}

		if (bShadow)
		{
			//-- Lyul 수정 -- Shadow : 1
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 1 ,RGB(10, 10, 10));
		}
		else
		{
			//-- Lyul 수정 -- Normal : 0
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 0);
		}

		m_Ypos+=m_FontHeight;
	}
	m_pTextSur->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	m_MainActive = TRUE; // 화면에 출력해도 좋다 !!!
}
