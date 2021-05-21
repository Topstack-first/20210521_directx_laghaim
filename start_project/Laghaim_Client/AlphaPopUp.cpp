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
// �Է¹��� ���ڿ��� ���� ����� ���ο� �����ϰ� ��������
{
	if ( m_LineCount >= MAX_TEXT_LINE || !string )
		return FALSE;

	m_Text[ m_LineCount ] = new char[ strlen(string) + 1 ];	// ��µ� ���ڿ�
	strcpy( m_Text[ m_LineCount ], string );	// ���ڿ� ����
	m_TextColor[ m_LineCount ] = color;			// ���� �� ����
	m_TextBackColor[ m_LineCount ] = back_color;	// ���� ����
	m_LineCount++;	// ���� ī��Ʈ 1 ����

	return TRUE;
}

// nate
//////////////////////////////////////////////////////////////////////////
// �ܾ� ������ �� �ٲ�. [2/6/2007 Theodoric]
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


BOOL CMultiText::AddStringEx(char *string, DWORD color, DWORD back_color) // ���� ���ڸ� �ν��ϴ� �༮. ///�����ۼ����Է�
// �Է¹��� ���ڿ��� ���� ����� ���ο� �����ϰ� ��������
{
	// �켱 ��Ʈ������ ���๮�ڰ� ���ö����� �д´�.
	char strTemp[256];
	int read_position = 0; // �а��ִ� ��ġ.
	int new_line_position = 0; // ���� ������ ��ġ.
	int string_len = strlen( string );
	int LineLen = 0;

	for( int i = 0 ; i < string_len ; ++i )
	{
		if( string[read_position] == '(' && read_position != string_len-2 )
		{
			if( string[read_position+1] == 'n' && string[read_position+2] == ')' ) // \n �̸�..
			{
				// ��������� ���ٷ� �Է�.
				LineLen = read_position-new_line_position; // �̹��� �Է��� ���� ����.
				if( LineLen > 255 ) // �ʹ� ��� ĵ��
					return FALSE;

				strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// ���ڿ� ����
				strTemp[LineLen] = '\0'; // �ι��� ����.

				AddString(strTemp, color, back_color);

				read_position += 2; // ���๮�� �ǳʶٱ�. ���� +3�ؾ������� ���� ������ ++�ϴϱ� 2��...
				new_line_position = read_position+1; // ���� ������ ��ġ�� ���...
			}
		}
		++read_position;
	}

	// �������� �Է�(������ ������ �̰� ó������ ��������)
	LineLen = read_position-new_line_position; // �̹��� �Է��� ���� ����.
	if( LineLen > 255 ) // �ʹ� ��� ĵ��
		return FALSE;

	strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// ���ڿ� ����
	strTemp[LineLen] = '\0'; // �ι��� ����.

	AddString(strTemp, color, back_color);

	return TRUE;
}

CMultiText2::CMultiText2() ///�����˾�â
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
// �Է¹��� ���ڿ��� ���� ����� ���ο� �����ϰ� ��������
{
	if ( m_LineCount >= MAX_TEXT_LINE2 || !string )
		return FALSE;

	m_Text[ m_LineCount ] = new char[ strlen(string) + 1 ];	// ��µ� ���ڿ�
	strcpy( m_Text[ m_LineCount ], string );	// ���ڿ� ����
	m_TextColor[ m_LineCount ] = color;			// ���� �� ����
	m_TextBackColor[ m_LineCount ] = back_color;	// ���� ����
	m_LineCount++;	// ���� ī��Ʈ 1 ����

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

BOOL CMultiText2::AddStringEx(char *string, DWORD color, DWORD back_color) // ���� ���ڸ� �ν��ϴ� �༮. ///�����ۼ����Է�
// �Է¹��� ���ڿ��� ���� ����� ���ο� �����ϰ� ��������
{
	// �켱 ��Ʈ������ ���๮�ڰ� ���ö����� �д´�.
	char strTemp[256];
	int read_position = 0; // �а��ִ� ��ġ.
	int new_line_position = 0; // ���� ������ ��ġ.
	int string_len = strlen( string );
	int LineLen = 0;

	for( int i = 0 ; i < string_len ; ++i )
	{
		if( string[read_position] == '(' && read_position != string_len-2 )
		{
			if( string[read_position+1] == 'n' && string[read_position+2] == ')' ) // \n �̸�..
			{
				// ��������� ���ٷ� �Է�.
				LineLen = read_position-new_line_position; // �̹��� �Է��� ���� ����.
				if( LineLen > 255 ) // �ʹ� ��� ĵ��
					return FALSE;

				strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// ���ڿ� ����
				strTemp[LineLen] = '\0'; // �ι��� ����.

				AddString(strTemp, color, back_color);

				read_position += 2; // ���๮�� �ǳʶٱ�. ���� +3�ؾ������� ���� ������ ++�ϴϱ� 2��...
				new_line_position = read_position+1; // ���� ������ ��ġ�� ���...
			}
		}
		++read_position;
	}

	// �������� �Է�(������ ������ �̰� ó������ ��������)
	LineLen = read_position-new_line_position; // �̹��� �Է��� ���� ����.
	if( LineLen > 255 ) // �ʹ� ��� ĵ��
		return FALSE;

	strncpy( strTemp, &string[new_line_position], sizeof(char)*LineLen );	// ���ڿ� ����
	strTemp[LineLen] = '\0'; // �ι��� ����.

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

//-- 2004.03.12 - �����Ϸ�
//-- Lyul
//-- m_Font �� handle �� �����´�. ���� �ı��� �ʿ䰡 ����.
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
// �˾�â�� �׸���.
// �˾�â�� ĳ���� �ָ��־ ���� ũ��� ���δ�.
// byMedalflag =>	0: �˾�â ��ġ���� + �ڽ� + �ؽ�Ʈ
//					1: �ؽ�Ʈ
//					2: ��ġ���� + �ڽ�
{
	if(m_MainActive == TRUE && m_pTextSur)
	{
		if( byMedalFlag == 3 ) // ���λ����� �Ƕ��� ���.
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
// �˾�â�� �ʹ� x, y ����
{
	// BOX_GAP : ä��â�� �۾��� ������ ��������� ������ ���� �ش�.
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

	// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// �������� ��Ʈ���� ����.
	// �������Ǹ�Ʈ���� �ٽ� ����
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// �ؽ�Ʈ�� ũ�⸸ŭ ũ�⼳��

	int offsetX = 0;
	if( CustomTextr == 4 )
	{
		offsetX = 54;
	}

	TLVec[0] = D3DTLVERTEX(D3DVECTOR(m_Xpos-BOX_GAP-offsetX,    m_Ypos-BOX_GAP,     0), 1, 0x00ffffff, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR(m_Xpos+m_TextSurWidth+BOX_GAP, m_Ypos-BOX_GAP,     0), 1, 0x00ffffff, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR(m_Xpos-BOX_GAP-offsetX,    m_Ypos+m_TextSurHeight+BOX_GAP,  0), 1, 0x00ffffff, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR(m_Xpos+m_TextSurWidth+BOX_GAP, m_Ypos+m_TextSurHeight+BOX_GAP,  0), 1, 0x00ffffff, 0, 1, 1);


	// ������
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00ffffff);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
//	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

	//----------------�׸��� ���� / �� --------------
	if (m_bRectSoft || SUCCEEDED( GET_D3DDEVICE()->BeginScene()))
	{
		//-----------------------TLVERTEX �� �̿��� ������ ���(ȭ���� ���� ��ǥ�� �Է��Ѵ�)

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		// �����׽�Ʈ ����
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		if( CustomTextr == 3 ) // ���λ����� �Ƕ��� ���.
		{
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_USHOP_BOARD]);
		}
		else
		{
			if (m_bRectSoft)	//  CNkChacter�� �ʰ�ȭ �ɶ� TRUE ����
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

	// ���󺹱�.
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
//		���� �޼����� �޾Ҵٸ� ����ض�!!!
//----------------------------------------------------------------------------------
//void CAlphaPopUp::PutString(int LineCnt, char**String)
void CAlphaPopUp::PutString(CMultiText *pMultiText, BOOL bShadow)
{
	if (!g_pDisplay || !m_Font )	// m_Font �� �ڵ��̴�
		return;

	int GreatestCount=0; // ����ū ���ڿ��� ������ ��´�
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// ����ū ���ڿ��� ������ ��´�
	for(int i = 0; i < pMultiText->m_LineCount; i++)
	{
		// �Է¹��� �ؽ�Ʈ�� ������ ���鼭 ����� ���ڿ� üũ
		int len = strlen(pMultiText->m_Text[i]);
		// ���ڿ��� ���� �����Ǿ��°�?
		if (g_bFixedWidthFont)
		{
			if(GreatestCount < len)
				GreatestCount = len;
		}
		else
		{
			// ��Ʈ ������ ��ȯ
			GetTextExtentPoint32(hdc, pMultiText->m_Text[i], len, &size);
			if (size.cx > TextSurWidth)
				TextSurWidth = size.cx;
		};
	}

	// by evilkiki 2009.12.03
	SAFE_DELETE(m_pTextSur);

	if (g_bFixedWidthFont)
	{
		// ����� ���ڿ��� ���μ��� ���ؼ� ���ǽ� ����
		m_TextSurWidth = GreatestCount*m_FontWidth;
		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;
	}
	else
	{
		m_TextSurWidth = TextSurWidth;
		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;

	}

	if( !m_TextSurWidth || !m_TextSurHeight ) // ���� ������ ������ 0�̸� ĵ��. 07-09-05 ����.
		return;

	// ���� ũ��� ���ǽ� ����
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
	m_pTextSur->Clear();

	// ���� ���ڿ� ����� �����ؼ� DrawText
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
			//-- Lyul ���� -- Shadow : 1
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 1 ,RGB(10, 10, 10));
		}
		else
		{
			//-- Lyul ���� -- Normal : 0
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 0);
		}
		m_Ypos+=m_FontHeight;
	}
	m_pTextSur->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	m_MainActive = TRUE; // ȭ�鿡 ����ص� ���� !!!
}

//-- Global Font �� �ϳ��� �޾ƿ´�.
//-- 2004.03.15 - �����Ϸ�
//-- ��ü �������� Global ������� ����
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

void CAlphaPopUp::Draw2( int nX, int nY, LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime ) ///������� ����,���� ��¿�. �Ƕ��� �ð��̳� �������� �ٸ���.
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
	// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);
	int ShowSize = 0;

	if( nShowTime < 60 ) // ������ �ȴþ��...
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
	// ������
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLight );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFog );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// �����׽�Ʈ ����
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

	// ���󺹱�.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwLight);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFog );
}

void CAlphaPopUp::RenderWinLose( LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime ) ///������� ����,���� ��¿�. �Ƕ��� �ð��̳� �������� �ٸ���.
{
	if( !GET_D3DDEVICE() )
		return;
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;
	// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);
	int ShowSize = 0;

	// ũ�� ����
	int factor = 50;

	if( (nShowTime/factor)%2 ) // ������ ������ ������ ����
		ShowSize = nShowTime%factor / 5;
	else
		ShowSize = -nShowTime%factor / 5;

//	float nI = (float)nShowTime * 0.002f; // ���� ����
	float nI = 0.0f; // ���� ����
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
	// ������
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLight );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFog );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// �����׽�Ʈ ����
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

	// ���󺹱�.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwLight);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFog );
}

void CAlphaPopUp::CalcPos_Crash_Miss(int x, int y)
// �˾�â�� �ʹ� x, y ����
{
	y -= 40;
	// BOX_GAP : ä��â�� �۾��� ������ ��������� ������ ���� �ش�.
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
	if (!g_pDisplay || !m_Font )	// m_Font �� �ڵ��̴�
		return;

	int GreatestCount=0; // ����ū ���ڿ��� ������ ��´�
	int TextSurWidth = 0;
	SIZE size;

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hFont = (HFONT) SelectObject( hdc, m_Font );

	// ����ū ���ڿ��� ������ ��´�
	for(int i = 0; i < pMultiText->m_LineCount; i++)
	{
		// �Է¹��� �ؽ�Ʈ�� ������ ���鼭 ����� ���ڿ� üũ
		int len = strlen(pMultiText->m_Text[i]);
		// ���ڿ��� ���� �����Ǿ��°�?
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
			// ��Ʈ ������ ��ȯ
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
		// ����� ���ڿ��� ���μ��� ���ؼ� ���ǽ� ����
		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;

	}
	else
	{

		m_TextSurHeight = pMultiText->m_LineCount*m_FontHeight;
	}


	// ���� ũ��� ���ǽ� ����
	g_pDisplay->CreateSurface( &m_pTextSur, m_TextSurWidth, m_TextSurHeight );
	if( !m_pTextSur )
		return;
	// fillcolor�� �̿��� textsurface Ŭ�����ϱ�
	m_pTextSur->Clear();

	// ���� ���ڿ� ����� �����ؼ� DrawText
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
			//-- Lyul ���� -- Shadow : 1
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 1 ,RGB(10, 10, 10));
		}
		else
		{
			//-- Lyul ���� -- Normal : 0
			m_pTextSur->DrawText(m_Font, pMultiText->m_Text[i], m_Xpos, m_Ypos,
								 pMultiText->m_TextBackColor[i], pMultiText->m_TextColor[i], 0);
		}

		m_Ypos+=m_FontHeight;
	}
	m_pTextSur->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	m_MainActive = TRUE; // ȭ�鿡 ����ص� ���� !!!
}
