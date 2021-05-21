#include "stdafx.h"
// TextBox.cpp: implementation of the CTextBox class.
//
//////////////////////////////////////////////////////////////////////

#include "TextBox.h"
#include "ddutil.h"
#include "main.h"
#include "LoadEffectSound.h"

extern DWORD g_dwLangType;
extern char g_strFontName[];

extern BOOL g_bFixedWidthFont;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextBox::CTextBox()
	: m_X(0)
	, m_Y(0)
	, m_Width(0)
	, m_Height(0)
	, m_FontSize(0)
	, m_FontWidth(0)
	, m_FontHeight(0)
	, m_BackColor(0)
	, m_ForeColor(0)
	, m_nCandidatePos(LOGIN_PAGE)
{
	m_pSurf = NULL;
	m_pPromptSurf = NULL;
	m_pCandidateSurf = NULL;
	m_pBack_CandidateSurf = NULL;
	m_pStatusSurf = NULL;
	m_Font = NULL;
	m_CandyFont = NULL;

	m_SpaceStr = NULL;

	//SetPos(0, 0);
	m_LastTime = timeGetTime();
	m_bShowPrompt = TRUE;
	m_bEnabled = FALSE;

	m_bIsPassword = FALSE;
	m_bDImeEnable = FALSE;

	//reset candidate
	for( int i = 0; i < 9; i++ )
		for( int j = 0; j < 512; j++ )
			m_cBuff[i][j] = '\0';

	m_iCandidateIndex = 0;
	m_iCandidateStart = 0;
	m_iCandidateEnd = 0;
	m_heightCandidate = 0;
}
//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
//-- Destructor

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CTextBox::~CTextBox()
{
	DeleteObject(m_CandyFont);

	// [12/22/2008 D.K ] : memory leak.
	// 만들어 놓고 호출 안해뒀음..;; 대체 누구냐..=_=
	DeleteRes();

	if (m_SpaceStr)
		delete[] m_SpaceStr;
}

HRESULT CTextBox::RestoreSurfaces()
{
	DrawText();				// m_pszStr에 있는 내용을 다시 그린다.

	// 프롬프트 글자는 다시 쓴다.
	if (m_pPromptSurf)
	{
		delete m_pPromptSurf;
		m_pPromptSurf = NULL;
	}
	g_pDisplay->CreateSurfaceFromText(&m_pPromptSurf, m_Font, PROMPT_STRING, m_BackColor, m_ForeColor);
	if( m_pPromptSurf )
		m_pPromptSurf->SetColorKey(m_BackColor);

	return S_OK;
}

void CTextBox::Init(int x, int y, int len, COLORREF backColor, COLORREF foreColor,
					BOOL bPassword, n_CandidatePos n_candyPos)
{
	SetPos(x, y);
	SetStringLength(len);
	m_SpaceStr = new char[500+1];
	for (int i = 0; i < 500; i++)
	{
		m_SpaceStr[i] = ' ';
	}
	m_SpaceStr[500] = '\0';

	SetColor(backColor, foreColor);

	m_bIsPassword = bPassword;
	m_nCandidatePos = n_candyPos;
}

// nate
void CTextBox::SetEnable(BOOL bEnable)
{
	m_bEnabled = bEnable;
	HIMC hIMC;

	if (bEnable)
	{
		// Get Input Context Handle
		hIMC = ImmGetContext(pCMyApp->Get_hWnd());
		//if (hIMC) {
		//	ImmSetConversionStatus(hIMC,d3dApp.m_dwImmConv,d3dApp.m_dwImmSent);
		//	ImmReleaseContext(pCMyApp->Get_hWnd(),hIMC);
		//}
		if( g_dwLangType == 1 )
			ImmSetConversionStatus( hIMC,
									IME_CMODE_JAPANESE | IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE,
									IME_SMODE_PHRASEPREDICT );
	}
	else // 일본 반-전자 버그 수정 [2/20/2007 Theodoric]
	{
		ImmReleaseContext(pCMyApp->Get_hWnd(), ImmGetContext(pCMyApp->Get_hWnd()) );
//		m_strFep.erase();
	}
}

void CTextBox::SetfontSize(int fontSize)
{
	m_FontSize = fontSize;

	HDC hDC = GetDC(NULL);
	int ptHeight = -MulDiv(fontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	//int ptWidth = -MulDiv(fontSize, GetDeviceCaps(hDC, LOGPIXELSX), 72);
	int ptHeight_candy = MulDiv(CANDYFONT_SIZE, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

//	m_FontWidth  = 0-(ptHeight-1)/2;
//	m_FontHeight = 0-ptHeight;

	SetFont (m_FontSize/2, m_FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	m_FontWidth  = sizeFont.cx;
	m_FontHeight = sizeFont.cy;

	g_pDisplay->CreateSurface(&m_pSurf, m_FontWidth*m_nMaxLen, m_FontHeight);
	m_pSurf->SetColorKey(m_BackColor);

	//-- Lyul --수정
	//m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);
	m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor, 0);
	g_pDisplay->CreateSurfaceFromText(&m_pPromptSurf, m_Font, PROMPT_STRING, m_BackColor, m_ForeColor);
	m_pPromptSurf->SetColorKey(m_BackColor);
	g_pDisplay->CreateSurface(&m_pStatusSurf, m_FontWidth*m_nMaxLen, m_FontHeight);
	m_pStatusSurf->SetColorKey(m_BackColor);
//-- Lyul 수정
//m_pStatusSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);
	m_pStatusSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor,0);

	int CandyWidth = (ptHeight_candy-1)/2;
	int CandyHeight = ptHeight_candy;

	g_pDisplay->CreateSurface(&m_pCandidateSurf, CandyWidth*m_nMaxLen, CandyHeight);
	m_pCandidateSurf->SetColorKey(m_BackColor);
	//-- Lyul
	//m_pCandidateSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);
	m_pCandidateSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor ,0);
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack_CandidateSurf, "interface/common/Back_Candy.bmp");
	m_pBack_CandidateSurf->SetColorKey(RGB(0, 0, 0));
}

void CTextBox::ChangeColor(DWORD color)
{
	//-- Lyul
	if( !m_pSurf )
		return;

	m_pSurf->DrawText(m_Font, m_pszStr, 0, 0, m_BackColor, color);
}


void CTextBox::DeleteRes()
{
	SAFE_DELETE(m_pSurf);
	SAFE_DELETE(m_pPromptSurf);
	SAFE_DELETE(m_pCandidateSurf);
	SAFE_DELETE(m_pStatusSurf);
	SAFE_DELETE(m_pBack_CandidateSurf);

	clearString();
}

void CTextBox::SetStringLength(int len)
{
	SetMaxLen(len);
	m_Width = 6*(len+2);
	m_Height = 12;
}

void CTextBox::SetPos(int x, int y)
{
	m_X = x;
	m_Y = y;
}

#define E_GAP	4
BOOL CTextBox::IsInside(int x, int y)
{
	if (x >= m_X-E_GAP && x < m_X+m_Width+E_GAP && y >= m_Y-E_GAP && y < m_Y+m_Height+E_GAP)
		return TRUE;

	return FALSE;
}

void CTextBox::SetColor(COLORREF backColor, COLORREF foreColor)
{
	m_BackColor = backColor;
	m_ForeColor = foreColor;
}

char *CTextBox::_StrDup()
{
	char *str = new char[strlen(m_pszStr) + 1];
	strcpy(str, m_pszStr);

	return str;
}

void CTextBox::DrawText()
{
	if (!m_Font || !m_pSurf || !m_pPromptSurf || !m_SpaceStr)
		return;

	char str_temp[IME_MAX_STRING];
	char str_temp2[IME_MAX_STRING];
	std::string	m_strDragTemp;
	std::string strInputBoxTemp;
	std::string strInputBoxTemp2;
	m_strDragTemp.clear();
	strInputBoxTemp.clear();
	int len = strlen(m_pszStr);

	//-- Lyul
	m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);

	if(m_iCursPoint<m_nMaxLen || len < m_nMaxLen-1 )
		m_InputViewSP=0;
	if(m_bIsBlock==TRUE)
	{
		m_strDragTemp = m_pszStr;
		m_strDragTemp = m_strDragTemp.substr(min(m_iBlockLP,m_iBlockSP) ,abs(m_iBlockLP-m_iBlockSP));
		strcpy(str_temp2,m_strDragTemp.c_str());
	}
	if(m_InputViewSP==0)
	{
		strcpy(m_strInputView,m_pszStr);
	}


	/* ///패스워드_스크롤
		if (m_bIsPassword)
		{
			for (int i = 0; i < len; i++)
			{
				str_temp[i] = '*';
			}
			str_temp[i] = '\0';
	  //-- Lyul
			m_pSurf->DrawText(m_Font, str_temp, 0, 0, m_BackColor, m_ForeColor);
		}
		else
	*/
	{
//		m_pSurf->DrawText(m_Font, m_pszStr, 0, 0, m_BackColor, m_ForeColor);
		if(len - m_InputViewSP > m_nMaxLen -1 && len>strlen(strInputBoxTemp2.c_str())  && m_iCursPoint >= m_nMaxLen-1)
		{

			strInputBoxTemp2 = m_pszStr;

//			if(m_strInputView[0]< 0x00 ){//지워질문자가 2바이트
			if(m_strIsHan[m_InputViewSP]=='1' ) //지워질문자가 한글의 초성이면
			{
				m_InputViewSP +=2;
				strInputBoxTemp=strInputBoxTemp2.substr(m_InputViewSP , strlen(strInputBoxTemp2.c_str())-m_InputViewSP);
				strcpy(m_strInputView ,strInputBoxTemp.c_str());
			}
			else
			{
				m_InputViewSP +=1;
				strInputBoxTemp=strInputBoxTemp2.substr(m_InputViewSP , strlen(strInputBoxTemp2.c_str())-m_InputViewSP);
				strcpy(m_strInputView ,strInputBoxTemp.c_str());
			}
		}
		else
		{
			strInputBoxTemp2 = m_pszStr;
			strInputBoxTemp=strInputBoxTemp2.substr(m_InputViewSP , strlen(strInputBoxTemp2.c_str())-m_InputViewSP);
			strcpy(m_strInputView ,strInputBoxTemp.c_str());

		}
		strInputBoxTemp.clear();
		strInputBoxTemp2.clear();

		strcpy(str_temp,m_strInputView);

		if (m_bIsPassword) ///패스워드_스크롤 패스워드는 이쪽으로만 오는중
		{
			int i;
			for (i = 0; i < strlen(str_temp); i++)
			{
				str_temp[i] = '*';
			}
			str_temp[i] = '\0';
		}
		//-- Lyul
		m_pSurf->DrawText(m_Font, str_temp, 0, 0, m_BackColor, m_ForeColor);
	}

	if(m_bIsBlock==TRUE && !m_bIsPassword)//드래그된 문자는 반전시켜 보여준다
		//-- Lyul
		m_pSurf->DrawText(m_Font, str_temp2,(min(m_iBlockLP,m_iBlockSP)-m_InputViewSP) *m_FontWidth ,0, m_ForeColor ,m_BackColor);//드래그 반전

}

void CTextBox::Draw()
{

	if (!g_pDisplay || !m_pSurf || !m_pPromptSurf || !m_pCandidateSurf || !m_pStatusSurf || !m_pBack_CandidateSurf)
		return;

	DWORD curTime = timeGetTime();
	if (curTime - m_LastTime >= PROMPT_TIME/2)
	{
		m_bShowPrompt = !m_bShowPrompt;
		m_LastTime = curTime;
	}

	g_pDisplay->Blt(m_X, m_Y + pCMyApp->Font_AddPosY, m_pSurf);

	if( m_bEnabled )
	{
		if( !m_bIsPassword )
		{
			if (g_dwLangType == 1)
			{
				drawConversionStatus();
				drawCandidate();
			}
		}

		if( m_bShowPrompt )
		{
			if (g_dwLangType == 1)
			{
				if( m_iFepCurPoint + m_iStringPoint > m_nMaxLen )
					g_pDisplay->Blt(m_X+(m_nMaxLen*m_FontWidth)-3, m_Y, m_pPromptSurf);
				else
				{
//					g_pDisplay->Blt(m_X+((m_iFepCurPoint+m_iStringPoint)*m_FontWidth)-3, m_Y, m_pPromptSurf);

					SIZE sizePt, sizeFep;
					std::string strPt, strFep;
					strPt = m_strBuff.substr(0,m_iStringPoint);
					strFep = m_strFep.substr(0,m_iFepCurPoint);

					getFontSize(&sizePt, m_Font, (LPCTSTR)strPt.c_str(), -1);
					getFontSize(&sizeFep, m_Font, (LPCTSTR)strFep.c_str(), -1);

					g_pDisplay->Blt(m_X+(sizePt.cx)+(sizeFep.cx)-3, m_Y, m_pPromptSurf);
				}
			}
			else
			{
//				int len = strlen(m_pszStr);
				int len = m_iCursPoint - m_InputViewSP ;//스크롤땜에..

				if( g_bFixedWidthFont )
				{
//					g_pDisplay->Blt(m_X+(len*m_FontWidth), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
					if (g_dwLangType == 3 ||g_dwLangType == 4) //중국 대만 버전... 커서이동 없음
					{
						len = strlen(m_pszStr);
						g_pDisplay->Blt(m_X+(len*m_FontWidth), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
					}
					else
						//커서가 문자 중간에 위치하기 때문에 옮겨줌..
						g_pDisplay->Blt(m_X+(len*m_FontWidth)-(m_FontWidth/2), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
				}
				else
				{
					HDC hdc;
					hdc = GetDC(pCMyApp->Get_hWnd());
					HFONT hFont = (HFONT) SelectObject(hdc, m_Font);

					SIZE size;

					if( m_bIsPassword )
					{
						GetTextExtentPoint32(hdc, "*", 1, &size);
						g_pDisplay->Blt(m_X+(len*size.cx)-(m_FontWidth/2), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
					}
					else
					{
						GetTextExtentPoint32(hdc, m_pszStr, len, &size);
						g_pDisplay->Blt(m_X+size.cx-(m_FontWidth/2), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
					}
					SelectObject(hdc, hFont);
					ReleaseDC(pCMyApp->Get_hWnd(), hdc);
				}

			}
		}
	}
}

void CTextBox::Draw(int x, int y, int dest_cx)
{
	if (!g_pDisplay || !m_pSurf || !m_pPromptSurf || !m_pCandidateSurf || !m_pStatusSurf || !m_pBack_CandidateSurf)
		return;

	DWORD curTime = timeGetTime();
	if (curTime - m_LastTime >= PROMPT_TIME/2)
	{
		m_bShowPrompt = !m_bShowPrompt;
		m_LastTime = curTime;
	}

	m_X = x = x + (dest_cx/2) - (m_Width/2);
	m_Y = y;

	HRESULT hr;
	hr = g_pDisplay->Blt(m_X, m_Y + pCMyApp->Font_AddPosY, m_pSurf);

	if( m_bEnabled )
	{
		if( !m_bIsPassword )
		{
			if (g_dwLangType == 1)
			{
				drawConversionStatus();
				drawCandidate();
			}
		}

		if( m_bShowPrompt )
		{
			if (g_dwLangType == 1)
			{
				if( m_iFepCurPoint + m_iStringPoint > m_nMaxLen )
					g_pDisplay->Blt(m_X+(m_nMaxLen*m_FontWidth)-3, m_Y, m_pPromptSurf);
				else
				{
//					g_pDisplay->Blt(m_X+((m_iFepCurPoint+m_iStringPoint)*m_FontWidth)-3, m_Y, m_pPromptSurf);

					SIZE sizePt, sizeFep;
					std::string strPt, strFep;
					strPt = m_strBuff.substr(0,m_iStringPoint);
					strFep = m_strFep.substr(0,m_iFepCurPoint);

					getFontSize(&sizePt, m_Font, (LPCTSTR)strPt.c_str(), -1);
					getFontSize(&sizeFep, m_Font, (LPCTSTR)strFep.c_str(), -1);

					g_pDisplay->Blt(m_X+(sizePt.cx)+(sizeFep.cx)-3, m_Y, m_pPromptSurf);
				}
			}
			else
			{
//				int len = strlen(m_pszStr);
				int len =m_iCursPoint - m_InputViewSP;
				if( g_bFixedWidthFont )
				{
					if (g_dwLangType == 3 ||g_dwLangType == 4) //중국 대만 버전... 커서이동 없음 ( 캐릭터 생성의 이름 출력등이 여기로 들어오는데 중국버젼에서 커서이동안되서 추가 ) (by 원석)
					{
						len = strlen(m_pszStr);
						g_pDisplay->Blt(m_X+(len*m_FontWidth), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
					}
					else
//						g_pDisplay->Blt(m_X+(len*m_FontWidth), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
						g_pDisplay->Blt(m_X+(len*m_FontWidth)-(m_FontWidth/2), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
				}
				else
				{
					HDC hdc;
					hdc = GetDC(pCMyApp->Get_hWnd());
					HFONT hFont = (HFONT) SelectObject(hdc, m_Font);

					SIZE size;

					GetTextExtentPoint32(hdc, m_pszStr, len, &size);
					g_pDisplay->Blt(m_X+size.cx, m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);

					SelectObject(hdc, hFont);
				}

			}
		}
	}

}

BOOL CTextBox::DrawToClip(int nMinArea, int nMaxArea, int nScrollPos)
{
	if (!g_pDisplay || !m_pSurf || !m_pPromptSurf || !m_pCandidateSurf || !m_pStatusSurf || !m_pBack_CandidateSurf)
		return FALSE;

	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = m_Width;
	src.bottom = m_Height;

	dest.left   = m_X;
	dest.top    = m_Y;
	dest.right  = m_X + m_Width;
	dest.bottom = m_Y + m_Height;

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
		dest.left   = m_X;
		dest.top    = m_Y  - nScrollPos;
		dest.right  = m_X + m_Width;
		dest.bottom = dest.top + m_Height;
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

	DWORD curTime = timeGetTime();
	if (curTime - m_LastTime >= PROMPT_TIME/2)
	{
		m_bShowPrompt = !m_bShowPrompt;
		m_LastTime = curTime;
	}

	//g_pDisplay->Blt(m_X, m_Y + pCMyApp->Font_AddPosY, m_pSurf);

	g_pDisplay->Blt(dest.left, dest.top + pCMyApp->Font_AddPosY,  m_pSurf);

	if( m_bEnabled )
	{
		if( !m_bIsPassword )
		{
			if (g_dwLangType == 1)
			{
				drawConversionStatus();
				drawCandidate();
			}
		}

		if( m_bShowPrompt )
		{
			if (g_dwLangType == 1)
			{
				if( m_iFepCurPoint + m_iStringPoint > m_nMaxLen )
					//g_pDisplay->Blt(m_X+(m_nMaxLen*m_FontWidth)-3, m_Y, m_pPromptSurf);
					g_pDisplay->Blt(dest.left+(m_nMaxLen*m_FontWidth)-3,  dest.top, m_pPromptSurf);

				else
				{
//					g_pDisplay->Blt(m_X+((m_iFepCurPoint+m_iStringPoint)*m_FontWidth)-3, m_Y, m_pPromptSurf);

					SIZE sizePt, sizeFep;
					std::string strPt, strFep;
					strPt = m_strBuff.substr(0,m_iStringPoint);
					strFep = m_strFep.substr(0,m_iFepCurPoint);

					getFontSize(&sizePt, m_Font, (LPCTSTR)strPt.c_str(), -1);
					getFontSize(&sizeFep, m_Font, (LPCTSTR)strFep.c_str(), -1);

					//g_pDisplay->Blt(m_X+(sizePt.cx)+(sizeFep.cx)-3, m_Y, m_pPromptSurf);
					g_pDisplay->Blt(dest.left+(sizePt.cx)+(sizeFep.cx)-3, dest.top,  m_pPromptSurf);
				}
			}
			else
			{
//				int len = strlen(m_pszStr);
				int len = m_iCursPoint - m_InputViewSP ;//스크롤땜에..

				if( g_bFixedWidthFont )
				{
//					g_pDisplay->Blt(m_X+(len*m_FontWidth), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
					if (g_dwLangType == 3 ||g_dwLangType == 4) //중국 대만 버전... 커서이동 없음
					{
						len = strlen(m_pszStr);
						//g_pDisplay->Blt(m_X+(len*m_FontWidth), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
						g_pDisplay->Blt(dest.left+(len*m_FontWidth), dest.top + pCMyApp->Font_AddPosY, dest.right, dest.bottom, m_pPromptSurf);
					}
					else
						//커서가 문자 중간에 위치하기 때문에 옮겨줌..
						//g_pDisplay->Blt(m_X+(len*m_FontWidth)-(m_FontWidth/2), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
						g_pDisplay->Blt(dest.left+(len*m_FontWidth)-(m_FontWidth/2), dest.top + pCMyApp->Font_AddPosY,  m_pPromptSurf);
				}
				else
				{
					HDC hdc;
					hdc = GetDC(pCMyApp->Get_hWnd());
					HFONT hFont = (HFONT) SelectObject(hdc, m_Font);

					SIZE size;

					if( m_bIsPassword )
					{
						GetTextExtentPoint32(hdc, "*", 1, &size);
						//g_pDisplay->Blt(m_X+(len*size.cx)-(m_FontWidth/2), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
						g_pDisplay->Blt(dest.left+(len*size.cx)-(m_FontWidth/2), dest.top + pCMyApp->Font_AddPosY,  m_pPromptSurf);

					}
					else
					{
						GetTextExtentPoint32(hdc, m_pszStr, len, &size);
						//	g_pDisplay->Blt(m_X+size.cx-(m_FontWidth/2), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
						g_pDisplay->Blt(dest.left+size.cx-(m_FontWidth/2), dest.top + pCMyApp->Font_AddPosY,  m_pPromptSurf);
					}
					SelectObject(hdc, hFont);
					ReleaseDC(pCMyApp->Get_hWnd(), hdc);
				}

			}
		}
	}
	return TRUE;
}

//DrawText( HFONT hFont, TCHAR* strText, DWORD dwOriginX, DWORD dwOriginY,
//		COLORREF crBackground, COLORREF crForeground );
//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font 중 하나를 받아온다.

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경

void CTextBox::SetFont (int f_w, int f_h, char *fontname)
{

	HDC hDC = GetDC(NULL);

	int ptHeight = -MulDiv(f_h, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	int ptHeight_candy = -MulDiv(CANDYFONT_SIZE, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	ReleaseDC(NULL, hDC);

	m_Font = G_GetFont(ptHeight,0,fontname);
	m_CandyFont = G_GetFont(ptHeight_candy,0,fontname);
	/*
	m_Font = CreateFont (ptHeight, 0,	// 세로, 가로 크기


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
	*//*
m_CandyFont = CreateFont (ptHeight_candy, 0,	// 세로, 가로 크기


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
	fontname);*/
}
// Font 생성
/*
m_Font = CreateFont (f_h, f_w,	// 세로, 가로 크기

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

/*OSVERSIONINFO VerInfo;

VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
GetVersionEx(&VerInfo);

if (VerInfo.dwMajorVersion < 4) {
    NONCLIENTMETRICS ncm;

    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
    m_Font = CreateFontIndirect(&ncm.lfStatusFont);

    if (!m_Font)
        m_Font = (HFONT) GetStockObject(SYSTEM_FONT);
} else {
    m_Font = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
}*/

// Line Height 계산 요망..
//LineHeight = f_h+2;		// 2 pixel 더...
//MaxVisibleLine = (Height-BOARDER_OFF*2-TITLE_H-CONTROL_OFF) / LineHeight;

// Bar Position 변경

//CalculateBarPos ();

BOOL CTextBox::IsInsideClip(int x, int y, int nMinArea, int nMaxArea, int nScrollPos)
{
	if (!g_pDisplay || !m_pSurf || !m_pPromptSurf || !m_pCandidateSurf || !m_pStatusSurf || !m_pBack_CandidateSurf)
		return FALSE;

	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = m_Width;
	src.bottom = m_Height;

	dest.left   = m_X;
	dest.top    = m_Y;
	dest.right  = m_X + m_Width;
	dest.bottom = m_Y + m_Height;

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
		dest.left   = m_X;
		dest.top    = m_Y  - nScrollPos;
		dest.right  = m_X + m_Width;
		dest.bottom = dest.top + m_Height;
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

	if (x >= m_X-E_GAP && x < m_X+m_Width+E_GAP &&
			y >= dest.top-E_GAP && y < dest.top+m_Height+E_GAP)
		return TRUE;

	return FALSE;
}

LRESULT CTextBox::MsgProcClip( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScrollPos, int nMin, int nMax)
{
	switch ( msg)
	{
	case WM_LBUTTONDOWN:
		if ( (IsInsideClip(LOWORD(lParam), HIWORD(lParam), nMin, nMax, nScrollPos)))
		{
			//bDown = true;
			//m_iBtnFlg = BTN_DOWN;
			m_bEnabled = TRUE;
		}
		else
		{
			m_bEnabled = FALSE;
		}
		break;

	default:
		break;
	}


	if (m_bEnabled)
		return CHanIme::MsgProc(hWnd, msg, wParam, lParam);
	else
		return E_FAIL;
}

LRESULT CTextBox::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg)
	{
	case WM_LBUTTONDOWN:
		if ( (IsInside(LOWORD(lParam), HIWORD(lParam)))	/*&& (!m_bDisable)*/ )
		{
			//bDown = true;
			//m_iBtnFlg = BTN_DOWN;
			m_bEnabled = TRUE;
			return S_OK;
		}
		else
		{
			m_bEnabled = FALSE;
		}
		break;

	default:
		break;
	}


	if (m_bEnabled)
		return CHanIme::MsgProc(hWnd, uMsg, wParam, lParam);
	else
		return E_FAIL;
}


//.............................................. 수정 부분
void CTextBox::ClearText()
{
	if(m_pszStr != NULL)
	{
		if (m_pSurf)
		{
			//ZeroMemory(m_pszStr, strlen(m_pszStr));
			//*m_pszStr = '\0';
			CHanIme::Clear();
			//-- Lyul
			m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);
			//m_nCharCount = 0;
		}
	}
}
//...........................................................

void CTextBox::CloseIMC()
{
	SetCloseIMC();
	return;
}

void CTextBox::SetText(char *str)
{
	if(m_pszStr != NULL)
	{
		if (m_pSurf)
		{
			if( str == '\0' )
			{
				m_pszStr[0] = '\0';
				return;
			}

			strcpy(m_pszStr, str);
			m_strBuff.insert(0, str);
			m_iStringPoint = strlen(m_strBuff.c_str());

			m_iCursPoint = m_iStringPoint;
			int len = strlen(m_pszStr);
			for(int i=0; i<len; i++) 		//히스토리 가져올때 문자정보 입력...
			{
				if(m_pszStr[i] <0x00)
				{
					m_strIsHan[i]='1';
					i++;
					m_strIsHan[i]='2';
				}
				else
					m_strIsHan[i]='0';
			}

			//-- Lyul
			m_pSurf->DrawText(m_Font, m_pszStr, 0, 0, m_BackColor, m_ForeColor);
			CHanIme::SetLenght(strlen(str));
			//m_nCharCount = strlen(str);
		}
	}
}

void CTextBox::drawIMEString()
{
	if (!m_Font || !m_pSurf || !m_pPromptSurf || !m_SpaceStr)
		return;

	SIZE sizeFw,sizeFep,sizeFep2;
	SIZE sizeFep_Fw,sizeFep_Cw,sizeFep_Ew;
	std::string strFw;
	std::string strNx;
	std::string strFep_Fw;
	std::string strFep_Cw;
	std::string strFep_Ew;
	char str_temp[IME_MAX_STRING];

//-- Lyul
	m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);

	if (m_bIsPassword)
	{
		int len = strlen(m_pszStr);
		int i = 0;
		for (i = 0; i < len; i++)
		{
			str_temp[i] = '*';
		}
		str_temp[i] = '\0';
		//-- Lyul
		m_pSurf->DrawText(m_Font, str_temp, 0, 0, m_BackColor, m_ForeColor);
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 입력창 스크롤롤 [2/20/2007 Theodoric]

		// Made Compeleted Word Draw
		INT len = strlen(m_strFep.c_str());

		// will made word Draw
		INT Fw_convertedPos = 0;
		INT Cw_convertedPos = 0;

		Fw_convertedPos = m_convertedPos[0];
		Cw_convertedPos = m_convertedPos[1]+1;

		if( Fw_convertedPos == 0 && Cw_convertedPos == 1 )
		{
			strFep_Fw = m_strFep.c_str();
		}

		if( Fw_convertedPos > 0 )
		{
			strFep_Fw = m_strFep.substr(0, Fw_convertedPos);
		}

		if( Cw_convertedPos > 1 )
		{
			strFep_Cw = m_strFep.substr(Fw_convertedPos, Cw_convertedPos - Fw_convertedPos);
			strFep_Ew = m_strFep.substr(Cw_convertedPos);
		}

		getFontSize(&sizeFep, m_Font, (LPCTSTR)m_strFep.c_str(), -1);
		getFontSize(&sizeFep2, m_Font, (LPCTSTR)m_strFep.c_str(), m_iFepCurPoint);

		getFontSize(&sizeFep_Fw, m_Font, (LPCTSTR)strFep_Fw.c_str(), -1);
		getFontSize(&sizeFep_Cw, m_Font, (LPCTSTR)strFep_Cw.c_str(), -1);
		getFontSize(&sizeFep_Ew, m_Font, (LPCTSTR)strFep_Ew.c_str(), -1);

		int uncompeletewordsLen = strlen(m_strFep.c_str());

		if(  m_iStringPoint +  uncompeletewordsLen > m_nMaxLen   )
		{
			if( uncompeletewordsLen > m_nMaxLen  )
			{
				m_InputViewSP = m_iStringPoint;
				int fep_fw_End = strlen(m_strFep.c_str());
				int fep_fw_Start = fep_fw_End - m_nMaxLen;

				if( fep_fw_Start % 2 )
				{
					fep_fw_Start ++;
				}

				strFep_Fw = m_strFep.substr( fep_fw_Start, fep_fw_End );
				getFontSize(&sizeFep_Fw, m_Font, (LPCTSTR)strFep_Fw.c_str(), -1);
			}
			else if( uncompeletewordsLen >= ((m_nMaxLen * 2) - m_iStringPoint + 2))
			{
				uncompeletewordsLen = ((m_nMaxLen * 2) - m_iStringPoint + 2);
				m_InputViewSP = m_iStringPoint  - m_nMaxLen + uncompeletewordsLen;
			}
			else
			{
				m_InputViewSP = m_iStringPoint -  m_nMaxLen  + uncompeletewordsLen;
			}

			if( m_strBuff[m_InputViewSP] >0 && m_strBuff[m_InputViewSP] < 127 && m_InputViewSP > 0)
			{
				m_InputViewSP ++;
			}

			if( m_InputViewSP >= m_iStringPoint || m_InputViewSP < 0 )
			{
				m_InputViewSP = m_iStringPoint;
			}

			strFw = m_strBuff.substr(m_InputViewSP, m_iStringPoint);
		}
		else
		{
			strFw = m_strBuff.substr(0,m_iStringPoint);
		}

		strNx = m_strBuff.substr(m_iStringPoint);
		getFontSize(&sizeFw, m_Font, (LPCTSTR)strFw.c_str(), -1);

		//-- Lyul
		m_pSurf->DrawText(m_Font, (char *)strFw.c_str(), 0, 0, m_BackColor, m_ForeColor);


// 		 //////////////////////////////////////////////////////////////////////////
// 		// Made Compeleted Word Draw :: ????? ??? ??? ???.
// 		strFw = m_strBuff.substr(0,m_iStringPoint);
// 		strNx = m_strBuff.substr(m_iStringPoint);
//
// 		INT len = strlen(m_strFep.c_str());
//
// 		INT Fw_convertedPos = 0;
// 		INT Cw_convertedPos = 0;
//
// 		Fw_convertedPos = m_convertedPos[0];
// 		Cw_convertedPos = m_convertedPos[1]+1;
//
// 		if( Fw_convertedPos == 0 && Cw_convertedPos == 1 )
// 			strFep_Fw = m_strFep.c_str();
//
// 		if( Fw_convertedPos > 0 )
// 		{
// 			strFep_Fw = m_strFep.substr(0, Fw_convertedPos);
// 		}
// 		if( Cw_convertedPos > 1 )
// 		{
// 			strFep_Cw = m_strFep.substr(Fw_convertedPos, Cw_convertedPos - Fw_convertedPos);
// 			strFep_Ew = m_strFep.substr(Cw_convertedPos);
// 		}
//
// 		getFontSize(&sizeFw, m_Font, (LPCTSTR)strFw.c_str(), -1);
// 		getFontSize(&sizeFep, m_Font, (LPCTSTR)m_strFep.c_str(), -1);
// 		getFontSize(&sizeFep2, m_Font, (LPCTSTR)m_strFep.c_str(), m_iFepCurPoint);
//
// 		getFontSize(&sizeFep_Fw, m_Font, (LPCTSTR)strFep_Fw.c_str(), -1);
// 		getFontSize(&sizeFep_Cw, m_Font, (LPCTSTR)strFep_Cw.c_str(), -1);
// 		getFontSize(&sizeFep_Ew, m_Font, (LPCTSTR)strFep_Ew.c_str(), -1);
//
// 		//-- Lyul
// 		m_pSurf->DrawText(m_Font, (char *)strFw.c_str(), 0, 0, m_BackColor, m_ForeColor);

		switch(m_nCandidatePos )
		{
		case LOGIN_PAGE:
			//m_pSurf->DrawText(m_Font, (char *)m_strFep.c_str(), sizeFw.cx, 0, RGB(30,30,0), RGB(230, 230, 100));
			m_pSurf->DrawText(m_Font, (char *)strFep_Fw.c_str(), sizeFw.cx, 0, RGB(30,30,0), RGB(230, 230, 100));
			m_pSurf->DrawText(m_Font, (char *)strFep_Cw.c_str(), sizeFw.cx + sizeFep_Fw.cx, 0, RGB(100,100,0), RGB(230, 230, 100));
			m_pSurf->DrawText(m_Font, (char *)strFep_Ew.c_str(), sizeFw.cx + sizeFep_Fw.cx + sizeFep_Cw.cx, 0, RGB(30,30,0), RGB(230, 230, 100));
			break;

		case NORMAL:
			//m_pSurf->DrawText(m_Font, (char *)m_strFep.c_str(), sizeFw.cx, 0, RGB(0,50,0), RGB(150,255,150));
			m_pSurf->DrawText(m_Font, (char *)strFep_Fw.c_str(), sizeFw.cx, 0, RGB(0,120,0), RGB(150,255,150));
			m_pSurf->DrawText(m_Font, (char *)strFep_Cw.c_str(), sizeFw.cx + sizeFep_Fw.cx, 0, RGB(0,100,0), RGB(150,255,150));
			m_pSurf->DrawText(m_Font, (char *)strFep_Ew.c_str(), sizeFw.cx + sizeFep_Fw.cx + sizeFep_Cw.cx, 0, RGB(0,30,0), RGB(150,255,150));
			break;

		case MSG_POPUP:
			//m_pSurf->DrawText(m_Font, (char *)m_strFep.c_str(), sizeFw.cx, 0, RGB(30,30,0), RGB(230, 230, 100));
			m_pSurf->DrawText(m_Font, (char *)strFep_Fw.c_str(), sizeFw.cx, 0, RGB(30,30,0), RGB(230, 230, 100));
			m_pSurf->DrawText(m_Font, (char *)strFep_Cw.c_str(), sizeFw.cx + sizeFep_Fw.cx, 0, RGB(100,100,0), RGB(230, 230, 100));
			m_pSurf->DrawText(m_Font, (char *)strFep_Ew.c_str(), sizeFw.cx + sizeFep_Fw.cx + sizeFep_Cw.cx, 0, RGB(30,30,0), RGB(230, 230, 100));
			break;

		case NEW_CHARNAME:
			//m_pSurf->DrawText(m_Font, (char *)m_strFep.c_str(), sizeFw.cx, 0, RGB(0,50,0), RGB(150,255,150));
			m_pSurf->DrawText(m_Font, (char *)strFep_Fw.c_str(), sizeFw.cx, 0, RGB(0,30,0), RGB(150,255,150));
			m_pSurf->DrawText(m_Font, (char *)strFep_Cw.c_str(), sizeFw.cx + sizeFep_Fw.cx, 0, RGB(0,100,0), RGB(150,255,150));
			m_pSurf->DrawText(m_Font, (char *)strFep_Ew.c_str(), sizeFw.cx + sizeFep_Fw.cx + sizeFep_Cw.cx, 0, RGB(0,30,0), RGB(150,255,150));
			break;
		}
		m_pSurf->DrawText(m_Font, (char *)strNx.c_str(), sizeFw.cx+sizeFep.cx, 0, m_BackColor, m_ForeColor);
	}
	// end  [2/20/2007 Theodoric]
	//////////////////////////////////////////////////////////////////////////
	return ;
}


void CTextBox::drawCandidate()
{
	if(!m_hWnd)
		return;

	INT	i = 0, j = 0;
	INT iCondidateCount = getCountCandidate();	// 폰트 리스트 얻기

	if(iCondidateCount)
	{
		INT	iCandidateHeight = 0;
		INT	iIndex = getCandidateIndex();

		if(m_iCandidateIndex != iIndex)			// 다른 폰트 선택시 그려주기
		{
			INT		iPageCount = getPageCount(iIndex);
			INT		iStart = getCandidateStart(iPageCount);
			INT		iEnd = getCandidateEnd(iPageCount);
			SIZE	sizeCandidate;

			for( i = 0; i < 9; i++ )
				for( j = 0; j < 512; j++ )
					m_cBuff[i][j] = '\0';

			for(i=0; iStart+i < iEnd; i++)
			{
				sprintf(m_cBuff[i], "%d:%s", i+1, getCandidateString(iStart+i));
				getFontSize(&sizeCandidate, m_Font, m_cBuff[i], -1);
			}

			m_iCandidateIndex = iIndex;
			m_iCandidateStart = iStart;
			m_iCandidateEnd = iEnd;
			m_heightCandidate = sizeCandidate.cy;

			setCandidatePage(m_iCandidateStart);
		}

		int x_pos = 0, y_pos = 0;
		SetCandidatePos(&x_pos, &y_pos);

		g_pDisplay->Blt(x_pos - 20, y_pos - 20, m_pBack_CandidateSurf);

		for(i = 0; (m_iCandidateStart + i) < m_iCandidateEnd; i++)
		{
			m_pCandidateSurf->DrawText(m_CandyFont, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);

			if( (m_iCandidateStart + i) == m_iCandidateIndex )
			{
				m_pCandidateSurf->DrawText(m_CandyFont, m_cBuff[i], 0, 0, m_BackColor, RGB(100,255,255));
				g_pDisplay->Blt(x_pos, y_pos + iCandidateHeight, m_pCandidateSurf);
			}
			else
			{
				m_pCandidateSurf->DrawText(m_CandyFont, m_cBuff[i], 0, 0, m_BackColor , RGB(100,100,255));
				g_pDisplay->Blt(x_pos, y_pos + iCandidateHeight, m_pCandidateSurf);
			}

			iCandidateHeight += m_heightCandidate;
		}

	}
	else
	{
		//reset candidate
		for( i = 0; i < 9; i++ )
			for( j = 0; j < 512; j++ )
				m_cBuff[i][j] = '\0';

		m_iCandidateIndex = 0;
		m_iCandidateStart = 0;
		m_iCandidateEnd = 0;
		m_heightCandidate = 0;
	}

	return;
}
void CTextBox::SetCandidatePos(int* x_pos, int* y_pos)
{
	// Modify (else if => switch) [2/20/2007 Theodoric]
	switch( m_nCandidatePos )
	{
	case LOGIN_PAGE:
//		*x_pos = m_X - 100;
//		*y_pos = m_Y + 75;
		*x_pos = m_X + 180;
		*y_pos = m_Y - 170;
		break;
	case NORMAL:
		*x_pos = m_X;
		*y_pos = m_Y - 200;
		break;
	case MSG_POPUP:
		*x_pos = m_X;
		*y_pos = m_Y - 220;
		break;
	case NEW_CHARNAME:
		*x_pos = m_X - 100;
		*y_pos = m_Y + 150;
		break;
	default:
		*x_pos = *y_pos = 0;
		break;
	}
	return;
}

void CTextBox::drawConversionStatus()
{
	getConversionStatus();

	m_pStatusSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);

//	m_pStatusSurf->DrawText(m_Font, (char *)m_strStat.c_str(), 0, 0, m_BackColor, RGB(255,255,0));

	// Modify ( else if  => switch) [2/20/2007 Theodoric]
	switch(m_nCandidatePos )
	{
	case LOGIN_PAGE:
		m_pStatusSurf->DrawText(m_Font, (char *)m_strStat.c_str(), 0, 0, RGB(0,50,0), RGB(150,200,150));
//		g_pDisplay->Blt(m_X, m_Y - 20, m_pStatusSurf);
		g_pDisplay->Blt(m_X - 42, m_Y, m_pStatusSurf);
		break;

	case NORMAL:
		m_pStatusSurf->DrawText(m_Font, (char *)m_strStat.c_str(), 0, 0, RGB(0,50,0), RGB(150,200,150));
		g_pDisplay->Blt(m_X - 22, m_Y, m_pStatusSurf);
//		g_pDisplay->Blt(m_X + 265, m_Y, m_pStatusSurf);
		break;

	case MSG_POPUP:
		m_pStatusSurf->DrawText(m_Font, (char *)m_strStat.c_str(), 0, 0, m_BackColor, m_ForeColor);
		g_pDisplay->Blt(m_X - 40, m_Y, m_pStatusSurf);
//		g_pDisplay->Blt(m_X - 50, m_Y, m_pStatusSurf);
		break;

	case NEW_CHARNAME:
		m_pStatusSurf->DrawText(m_Font, (char *)m_strStat.c_str(), 0, 0, RGB(0,50,0), RGB(150,200,150));
//		m_pStatusSurf->DrawText(m_Font, (char *)m_strStat.c_str(), 0, 0, RGB(0,0,30), RGB(50, 200,200));
//		g_pDisplay->Blt(m_X - 20, m_Y, m_pStatusSurf);
		g_pDisplay->Blt(m_X, m_Y - 25, m_pStatusSurf);
		break;
	}
	return;
}


bool getFontSize(LPSIZE pSize,HFONT hFont,LPCTSTR pText,INT iLength)
{
	if(iLength < 0)
		iLength = strlen(pText);

	SIZE	sizeFont;
	HDC		hdmDC = CreateCompatibleDC(NULL);
	HFONT	holdFont = (HFONT)SelectObject(hdmDC,hFont);

	// 폰트 String 의 폭과 넓이 얻기
	GetTextExtentPoint(hdmDC,pText,iLength,&sizeFont);
	SelectObject(hdmDC,holdFont);
	DeleteDC(hdmDC);
	*pSize = sizeFont;

	return true;
}
