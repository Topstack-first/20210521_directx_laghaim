#include "stdafx.h"
#include "LetterBox.h"
#include "ddutil.h"
#include "main.h"
#include "LoadEffectSound.h"
#include "CommonConfig.h"

extern DWORD g_dwLangType;
extern char g_strFontName[];


extern BOOL g_bFixedWidthFont;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLetterBox::CLetterBox()
	: m_X(0)
	, m_X2(0)
	, m_Y(0)
	, m_Width(0)
	, m_Height(0)
	, m_FontSize(0)
	, m_FontWidth(0)
	, m_FontHeight(0)
	, m_BackColor(0)
	, m_ForeColor(0)
	, m_nCandidatePos(LETTER)
{
	m_pSurf = NULL;
	m_pPromptSurf = NULL;
	m_pCandidateSurf = NULL;
	m_pBack_CandidateSurf = NULL;
	m_pStatusSurf = NULL;
	m_Font = NULL;
	m_CandyFont = NULL;
	m_SpaceStr = NULL;
	m_read_only = false;
	m_row=5;
	m_column=5 ;
	//SetPos(0, 0);
	m_LastTime = timeGetTime();
	m_bShowPrompt = TRUE;
	m_bEnabled = FALSE;
	m_bIsPassword = FALSE;
	m_bDImeEnable = FALSE;
	m_iCandidateIndex = 0;
	m_iCandidateStart = 0;
	m_iCandidateEnd = 0;
	m_heightCandidate = 0;
	//reset candidate
	for( int i = 0; i < 9; i++ )
	{
		for( int j = 0; j < 512; j++ )
		{
			m_cBuff[i][j] = '\0';
		}
	}

	LetterTableReset();
}

CLetterBox::~CLetterBox()
{
	DeleteRes();
	SAFE_DELETE_ARRAY(m_SpaceStr);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


HRESULT CLetterBox::RestoreSurfaces()
{
	DrawText();				// m_pszStr에 있는 내용을 다시 그린다.

	// 프롬프트 글짜는 다시 쓴다.
	SAFE_DELETE(m_pPromptSurf);

	if( g_pDisplay )
		g_pDisplay->CreateSurfaceFromText(&m_pPromptSurf, m_Font, PROMPT_STRING, m_BackColor, m_ForeColor);
	if( m_pPromptSurf )
		m_pPromptSurf->SetColorKey(m_BackColor);

	return S_OK;
}

void CLetterBox::Init_MEBox(bool read_only, int row, int column)
{
	m_read_only = read_only;
	m_row = row;
	m_column=column;

}
void CLetterBox::Init(int x, int y, int len, COLORREF backColor, COLORREF foreColor,
					  BOOL bPassword, n_CandidatePos1 n_candyPos)
{
	SetPos(x, y);
	SetStringLength(len);
	m_SpaceStr = new char[m_column +1];
	for (int i = 0; i < m_column +1; i++)
		m_SpaceStr[i] = ' ';

	m_SpaceStr[m_column] = 0;
	SetColor(backColor, foreColor);
	m_bIsPassword = bPassword;
	m_nCandidatePos = n_candyPos;
}

void CLetterBox::SetEnable(BOOL bEnable)
{
	m_bEnabled = bEnable;
	HIMC hIMC;

	if (bEnable)
	{
		hIMC = ImmGetContext(pCMyApp->Get_hWnd());
		m_MEBox.FocusOn();
		if( g_dwLangType == 1 )
			ImmSetConversionStatus( hIMC,
									IME_CMODE_JAPANESE | IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE,
									IME_SMODE_PHRASEPREDICT );

	}
	else
		m_MEBox.FocusOff();
}

void CLetterBox::SetFontSize(int fontSize)
{
	if( !g_pDisplay )
		return;

	m_FontSize = fontSize;
	m_FontWidth  = 8;
	m_FontHeight = 16;

	if( CommonConfig::Instance()->GetThaFlag() == true )
	{
		m_Font = G_GetFont(m_FontHeight,m_FontWidth,"MS Sans Serif");
	}
	else
	{
		m_Font = G_GetFont(m_FontHeight,m_FontWidth,"굴림체");
	}	

	g_pDisplay->CreateSurface(&m_pSurf, m_FontWidth*m_column, m_FontHeight*m_row);
	if( m_pSurf )
		m_pSurf->SetColorKey(m_BackColor);

	for(int i=0; i<m_row; i++)
	{
		if( m_pSurf )
			m_pSurf->DrawText(m_Font, m_SpaceStr, 0, m_FontHeight*i, m_BackColor, m_ForeColor);
	}

	g_pDisplay->CreateSurfaceFromText(&m_pPromptSurf, m_Font, PROMPT_STRING, m_BackColor, m_ForeColor);
	if( m_pPromptSurf )
		m_pPromptSurf->SetColorKey(m_BackColor);

	g_pDisplay->CreateSurface(&m_pStatusSurf, m_FontWidth*m_column, m_FontHeight*m_row);
	if( m_pStatusSurf )
	{
		m_pStatusSurf->SetColorKey(m_BackColor);
		m_pStatusSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);
	}

	//--------------------------------------------------------------------------
	//-- Multi Line Edit Set
	m_MEBox.Set_RowColm(m_row, m_column);

	//-- 보내기만 256
	if(!m_read_only)
		m_MEBox.Init(256,m_pSurf,m_pPromptSurf, m_read_only);
	else
		m_MEBox.Init(2048,m_pSurf,m_pPromptSurf, m_read_only);
}

void CLetterBox::ChangeColor(DWORD color)
{
	if( m_pSurf )
		m_pSurf->DrawText(m_Font, m_pszStr, 0, 0, m_BackColor, color);
}


void CLetterBox::DeleteRes()
{
	SAFE_DELETE(m_pSurf);
	SAFE_DELETE(m_pPromptSurf);
	SAFE_DELETE(m_pCandidateSurf);
	SAFE_DELETE(m_pStatusSurf);
	SAFE_DELETE(m_pBack_CandidateSurf);
}

void CLetterBox::SetStringLength(int len)
{
	SetMaxLen(len);
	m_Width = 6*(len+2);
	m_Height = 12;
}

void CLetterBox::SetPos(int x, int y)
{
	m_X = x;
	m_Y = y;
}

#define E_GAP	4
BOOL CLetterBox::IsInside(int x, int y)
{
	if (x >= m_X-E_GAP && x < m_X+m_Width+E_GAP && y >= m_Y-E_GAP && y < m_Y+m_Height+E_GAP)
		return TRUE;

	return FALSE;
}

void CLetterBox::SetColor(COLORREF backColor, COLORREF foreColor)
{
	m_BackColor = backColor;
	m_ForeColor = foreColor;
}

char *CLetterBox::_StrDup()
{
	char *str = new char[strlen(m_pszStr) + 1];
	strcpy(str, m_pszStr);

	return str;
}
//////// 채팅에디트  ///////////////////////////////////////////
void CLetterBox::DrawText()
{
	int blockSP_x=0;
	int blockSP_y=0;
	int blockLP_x=0;
	int blockLP_y=0;

	if (!m_Font || !m_pSurf || !m_pPromptSurf || !m_SpaceStr)
		return;

	int len = strlen(m_pszStr);

	LetterTableUpdate();

	for(int j=0; j<LETTER_MAX_Y; j++)
		m_pSurf->DrawText(m_Font, m_SpaceStr, 0, j*m_FontHeight, m_BackColor, m_ForeColor);

	for(int i=0; i<LETTER_MAX_Y; i++)
		m_pSurf->DrawText(m_Font, m_LetterTable[i], 0, i*m_FontHeight, m_BackColor, m_ForeColor);

	if(m_bIsBlock==TRUE)
	{
		//블록이 잡히면
		GetLetterCursor(min(m_iBlockSP,m_iBlockLP));
		blockSP_x=m_iLetterCursor_x;
		blockSP_y=m_iLetterCursor_y;

		GetLetterCursor(max(m_iBlockSP,m_iBlockLP));
		blockLP_x=m_iLetterCursor_x;
		blockLP_y=m_iLetterCursor_y;

		for(int z=blockSP_y; z<blockLP_y+1; z++)
		{
			char temp[IME_MAX_STRING]= {0,};
			if(z==blockSP_y)
			{
				if(blockSP_y==blockLP_y)
					strncpy(temp,m_LetterTable[z]+blockSP_x,blockLP_x-blockSP_x);
				else
					strcpy(temp,m_LetterTable[z]+blockSP_x);
				m_pSurf->DrawText(m_Font, temp, blockSP_x*m_FontWidth, z*m_FontHeight, m_ForeColor,m_BackColor);
			}
			else if(z==blockLP_y)
			{
				strncpy(temp,m_LetterTable[z],blockLP_x);
				m_pSurf->DrawText(m_Font, temp, 0, z*m_FontHeight, m_ForeColor,m_BackColor);
			}
			else
			{
				m_pSurf->DrawText(m_Font, m_LetterTable[z], 0, z*m_FontHeight, m_ForeColor,m_BackColor);
			}
		}
	}
}

void CLetterBox::Draw()
{
	int i = 0;

	if( m_pSurf )
		m_pSurf->Clear();

	for( i=0; i<m_row; i++)
		m_pSurf->DrawText(m_Font, m_SpaceStr, 0, m_FontHeight*i, m_BackColor, m_ForeColor);

	m_MEBox.DrawMultiText();
	g_pDisplay->Blt(m_X, m_Y + pCMyApp->Font_AddPosY, m_pSurf);
	return;
	/** nate 2005-10-07 : 위에서 리턴하는 루틴이 있어서 주석처리 함
		DWORD curTime = timeGetTime();
		if (curTime - m_LastTime >= PROMPT_TIME/2) {
			m_bShowPrompt = !m_bShowPrompt;
			m_LastTime = curTime;
		}

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

			if( m_bShowPrompt ) {
				if (g_dwLangType == 1)
				{
					if( m_iFepCurPoint + m_iStringPoint > m_nMaxLen )
						g_pDisplay->Blt(m_X+(m_nMaxLen*m_FontWidth)-3, m_Y, m_pPromptSurf);
					else
					{
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
					GetLetterCursor(m_iCursPoint);
					LetterTableUpdate();
					int len = strlen(m_pszStr);

					if( g_bFixedWidthFont ) {
						g_pDisplay->Blt(m_X+((m_iLetterCursor_x)*m_FontWidth)-(m_FontWidth/2)  , m_Y + pCMyApp->Font_AddPosY + (m_iLetterCursor_y*m_FontHeight), m_pPromptSurf);
					} else {
						HDC hdc;
						hdc = GetDC(pCMyApp->Get_hWnd());
						HFONT hFont = (HFONT) SelectObject(hdc, m_Font);

						SIZE size;

						if( m_bIsPassword ) {
							GetTextExtentPoint32(hdc, "*", 1, &size);
							g_pDisplay->Blt(m_X+(len*size.cx), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
						} else {
							GetTextExtentPoint32(hdc, m_pszStr, len, &size);
							g_pDisplay->Blt(m_X+size.cx, m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);
						}
						SelectObject(hdc, hFont);
						ReleaseDC(pCMyApp->Get_hWnd(), hdc);
					}

				}
			}
		}
	*/
}
///////////////////////////////////////////////////////////////////

void CLetterBox::Draw(int x, int y, int dest_cx)//안씀
{

//L_MultiEditBox::DrawMultiText(
	if (!g_pDisplay || !m_pSurf || !m_pPromptSurf || !m_pCandidateSurf || !m_pStatusSurf || !m_pBack_CandidateSurf)
		return;

	DWORD curTime = timeGetTime();
	if (curTime - m_LastTime >= PROMPT_TIME/2)
	{
		m_bShowPrompt = !m_bShowPrompt;
		m_LastTime = curTime;
	}

	m_X = x = x + (dest_cx/2) - (m_Width/2);
	m_X2 = x + (dest_cx/2)-m_Width;
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
				int len = strlen(m_pszStr);
				if( g_bFixedWidthFont )
				{
					g_pDisplay->Blt(m_X+(len*m_FontWidth), m_Y + pCMyApp->Font_AddPosY, m_pPromptSurf);

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

void CLetterBox::SetFont (int f_w, int f_h, char *fontname)
{
	// 이미 폰트를 만들었으면....  제거하궁...
	if (m_Font != NULL)
		DeleteObject(m_Font);
	if( m_CandyFont )
		DeleteObject(m_CandyFont);

	HDC hDC = GetDC(NULL);

	int ptHeight = -MulDiv(f_h, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	int ptHeight_candy = -MulDiv(CANDYFONT_SIZE, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	ReleaseDC(NULL, hDC);

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
							  fontname);
}

LRESULT CLetterBox::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return m_MEBox.MsgProc(hWnd,  uMsg, wParam, lParam);
}


//.............................................. 수정 부분
void CLetterBox::ClearText()
{
	if(m_pszStr != NULL)
	{
		if (m_pSurf)
		{
			CHanIme::Clear();
			m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);
		}
	}
}
//...........................................................

void CLetterBox::CloseIMC()
{
	SetCloseIMC();
	return;
}

void CLetterBox::SetText(char *str)
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
			LetterTableUpdate();
			m_strBuff.insert(0, str);
			m_iStringPoint = strlen(m_strBuff.c_str());
			m_iCursPoint = m_iStringPoint;//wan
			DrawText();
			CHanIme::SetLenght(strlen(str));
		}
	}
}

void CLetterBox::drawIMEString()
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

	m_pSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);

	if (m_bIsPassword)
	{
		int len = strlen(m_pszStr);
		int i;
		for (i = 0; i < len; i++)
			str_temp[i] = '*';

		str_temp[i] = '\0';
		m_pSurf->DrawText(m_Font, str_temp, 0, 0, m_BackColor, m_ForeColor);
	}
	else
	{
		strFw = m_strBuff.substr(0,m_iStringPoint);
		strNx = m_strBuff.substr(m_iStringPoint);

		INT len = strlen(m_strFep.c_str());

		INT Fw_convertedPos = 0;
		INT Cw_convertedPos = 0;

		Fw_convertedPos = m_convertedPos[0];
		Cw_convertedPos = m_convertedPos[1]+1;

		if( Fw_convertedPos == 0 && Cw_convertedPos == 1 )
			strFep_Fw = m_strFep.c_str();

		if( Fw_convertedPos > 0 )
			strFep_Fw = m_strFep.substr(0, Fw_convertedPos);

		if( Cw_convertedPos > 1 )
		{
			strFep_Cw = m_strFep.substr(Fw_convertedPos, Cw_convertedPos - Fw_convertedPos);
			strFep_Ew = m_strFep.substr(Cw_convertedPos);
		}

		getFontSize(&sizeFw, m_Font, (LPCTSTR)strFw.c_str(), -1);
		getFontSize(&sizeFep, m_Font, (LPCTSTR)m_strFep.c_str(), -1);
		getFontSize(&sizeFep2, m_Font, (LPCTSTR)m_strFep.c_str(), m_iFepCurPoint);

		getFontSize(&sizeFep_Fw, m_Font, (LPCTSTR)strFep_Fw.c_str(), -1);
		getFontSize(&sizeFep_Cw, m_Font, (LPCTSTR)strFep_Cw.c_str(), -1);
		getFontSize(&sizeFep_Ew, m_Font, (LPCTSTR)strFep_Ew.c_str(), -1);

		m_pSurf->DrawText(m_Font, (char *)strFw.c_str(), 0, 0, m_BackColor, m_ForeColor);
		m_pSurf->DrawText(m_Font, (char *)strFep_Fw.c_str(), sizeFw.cx, 0, RGB(0,30,0), RGB(150,255,150));
		m_pSurf->DrawText(m_Font, (char *)strFep_Cw.c_str(), sizeFw.cx + sizeFep_Fw.cx, 0, RGB(0,100,0), RGB(150,255,150));
		m_pSurf->DrawText(m_Font, (char *)strFep_Ew.c_str(), sizeFw.cx + sizeFep_Fw.cx + sizeFep_Cw.cx, 0, RGB(0,30,0), RGB(150,255,150));
		m_pSurf->DrawText(m_Font, (char *)strNx.c_str(), sizeFw.cx+sizeFep.cx, 0, m_BackColor, m_ForeColor);
	}

	return ;
}


void CLetterBox::drawCandidate()
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

		if( g_pDisplay )
			g_pDisplay->Blt(x_pos - 20, y_pos - 20, m_pBack_CandidateSurf);

		for(i = 0; (m_iCandidateStart + i) < m_iCandidateEnd; i++)
		{
			if( m_pCandidateSurf )
				m_pCandidateSurf->DrawText(m_CandyFont, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);

			if( (m_iCandidateStart + i) == m_iCandidateIndex )
			{
				if( m_pCandidateSurf )
					m_pCandidateSurf->DrawText(m_CandyFont, m_cBuff[i], 0, 0, m_BackColor, RGB(100,255,255));
				if( g_pDisplay )
					g_pDisplay->Blt(x_pos, y_pos + iCandidateHeight, m_pCandidateSurf);
			}
			else
			{
				if( m_pCandidateSurf )
					m_pCandidateSurf->DrawText(m_CandyFont, m_cBuff[i], 0, 0, m_BackColor , RGB(100,100,255));
				if( g_pDisplay )
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
void CLetterBox::SetCandidatePos(int* x_pos, int* y_pos)
{
	*x_pos = *y_pos = 0;
	return;
}

void CLetterBox::drawConversionStatus()
{
	getConversionStatus();

	if( m_pStatusSurf )
		m_pStatusSurf->DrawText(m_Font, m_SpaceStr, 0, 0, m_BackColor, m_ForeColor);

	if( m_pStatusSurf )
		m_pStatusSurf->DrawText(m_Font, (char *)m_strStat.c_str(), 0, 0, RGB(0,50,0), RGB(150,200,150));
	if( g_pDisplay )
		g_pDisplay->Blt(m_X, m_Y - 25, m_pStatusSurf);

	return;
}


bool CLetterBox::getFontSize(LPSIZE pSize,HFONT hFont,LPCTSTR pText,INT iLength)
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

void CLetterBox::OnChar(UINT nChar)
{
	char temp[IME_MAX_STRING]= {0,};

	if( nChar == 3 && m_bIsBlock==TRUE)//clrl+c이면 copybuff로 복사
	{
		strncpy(m_CopyBuff,m_pszStr+min(m_iBlockSP,m_iBlockLP),abs(m_iBlockSP-m_iBlockLP));
		m_bIsBlock=FALSE;
	}
	else if(nChar == 22)//clrl+v이면 copybuff를 m_pszStr로 삽입
	{
		//copybuff + m_pszStr가 m_nMaxLen를 못넘게...
		if(m_nCharCount+strlen(m_CopyBuff ) >m_nMaxLen*2)//
			return;
		strcpy(temp,m_pszStr+m_iCursPoint);
		strcpy(m_pszStr+m_iCursPoint,m_CopyBuff);
		strcpy(m_pszStr+(m_iCursPoint+strlen(m_CopyBuff)),temp);
		m_iCursPoint += strlen(m_CopyBuff);
		if(m_iCursPoint > m_nMaxLen-1)
			m_InputViewSP = m_iCursPoint- (m_nMaxLen -2);
	}
	else if((nChar == 8 )|| (nChar == 27))
	{
		GetLetterCursor(m_iCursPoint-1);
		if(m_iCursPoint==0)
			return;
		if(m_bIsBlock==TRUE) 		//블럭이 지정되 있으면 블럭만 지움
		{
			strcpy(temp,m_pszStr+max(m_iBlockSP,m_iBlockLP));
			strcpy(m_pszStr+min(m_iBlockSP,m_iBlockLP),temp);
			m_iCursPoint=min(m_iBlockSP,m_iBlockLP);
			m_bIsBlock=FALSE;
		}
		else
		{
			if (m_iWhatChar[m_iLetterCursor_y][m_iLetterCursor_x] ==3)  		// 영,숫자이면 1바이트
			{
				strcpy(temp,m_pszStr+m_iCursPoint);
				strcpy(m_pszStr+(m_iCursPoint-1),temp);
				m_iCursPoint-=1;
			}
			else
			{
				strcpy(temp,m_pszStr+m_iCursPoint);
				strcpy(m_pszStr+(m_iCursPoint-2),temp);
				m_iCursPoint-=2;
			}
		}
		if(m_iCursPoint<m_InputViewSP || strlen(m_pszStr)<m_nMaxLen )
			m_InputViewSP=0;
	}
	// 영문일때에만..처리한다
	// 이유는 한글 조합이 완료되면 m_nCharCount는 이미증가한다
	//DrawCompString()에서 len만큼 증가시킨다
	else if ( !iscntrl(nChar) && isascii(nChar) )
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
	else if ((g_dwLangType == 7 && !iscntrl(nChar)))
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
	else if (((g_dwLangType == 8||g_dwLangType == 6) && !iscntrl(nChar)))
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
	else if(isascii(nChar))   //엔터값이면..
	{
		char str[3];
		str[0] = nChar & 0xff;
		str[1] = 10 & 0xff;
		str[2] = 0;
		DrawCompString(str);
	}
	m_nCharCount=strlen(m_pszStr);
	m_bIsBlock=FALSE;
}

bool CLetterBox::OnKeyDown(CHAR cChar)
{
	INT iState;
	iState=GetAsyncKeyState(VK_SHIFT);

	switch(cChar)
	{
	case VK_DELETE:
		delChar();					// 현재 커서 위치 아래 있는 글자 지우기
		m_bDraw = true;
		break;
	case VK_LEFT:					// 커서 위치 변환
		if(!m_iCursPoint)
			break;
		CursLMove(iState);
		m_bDraw = true;
		break;
	case VK_RIGHT:					// 커서 위치 변환
		if(m_iCursPoint==m_nCharCount)
			break;
		CursRMove(iState);
		m_bDraw = true;
		break;
	case VK_UP:					// 커서 위치 변환
		if(!m_iCursPoint)
			break;
		CursUMove(iState);
		m_bDraw = true;
		break;
	case VK_DOWN:					// 커서 위치 변환
		if(m_iCursPoint==strlen(m_pszStr))
			break;
		CursDMove(iState);
		m_bDraw = true;
		break;
	case VK_HOME:
		m_iCursPoint = 0;
		m_InputViewSP=0;
		break;
	case VK_END:
		m_iCursPoint =strlen(m_pszStr);
		if(m_iCursPoint>m_nMaxLen-1)
		{
			m_InputViewSP = m_iCursPoint - m_nMaxLen  ;
		}
		break;
	}

	return true;
}
bool CLetterBox::delChar()
{
	char temp[IME_MAX_STRING]= {0,};
	if(m_bIsBlock==TRUE) 		//블럭이 지정되 있으면 블럭만 지움
	{
		strcpy(temp,m_pszStr+max(m_iBlockSP,m_iBlockLP));
		strcpy(m_pszStr+min(m_iBlockSP,m_iBlockLP),temp);
		m_iCursPoint = min(m_iBlockSP,m_iBlockLP);
		m_bIsBlock=FALSE;
	}
	else if (m_iCursPoint < strlen(m_pszStr))
	{
		GetLetterCursor(m_iCursPoint);
		if (m_iWhatChar[m_iLetterCursor_y][m_iLetterCursor_x] ==3)  		// 영,숫자이면 1바이트
		{
			strcpy(temp,m_pszStr+(m_iCursPoint+1));
			strcpy(m_pszStr+m_iCursPoint,temp);
			m_nCharCount=strlen(m_pszStr);
		}
		else
		{
			strcpy(temp,m_pszStr+(m_iCursPoint+2));
			strcpy(m_pszStr+m_iCursPoint,temp);
			m_nCharCount=strlen(m_pszStr);
		}
	}
	if(m_nCharCount<m_nMaxLen)
		m_InputViewSP=0;
	return true;
}
void CLetterBox::CursLMove(INT iState)
{
	if(m_iCursPoint==0)
		return;
	GetLetterCursor(m_iCursPoint-1);
	if(iState>=0) //SHIFT가 눌리지 않았을때
	{
		if (m_iWhatChar[m_iLetterCursor_y][m_iLetterCursor_x] ==3)  		// 영,숫자이면 1바이트
		{
			m_iCursPoint--;//wan
		}
		else 	//한글,엔터이면
		{
			m_iCursPoint=-2;//wan
		}
		m_bIsBlock=FALSE;
	}
	else 					//SHIFT가 눌렸을때
	{
		if(m_bIsBlock==FALSE) 		//블럭이 잡혔는가
		{
			m_iBlockLP=m_iCursPoint;
			m_iBlockSP=m_iCursPoint;
		}
		if (m_iWhatChar[m_iLetterCursor_y][m_iLetterCursor_x] ==3)  		// 영,숫자이면 1바이트
		{
			m_iCursPoint--;//wan
		}
		else
		{
			m_iCursPoint -= 2;//wan
		}
		m_iBlockLP =m_iCursPoint;
		m_bIsBlock=TRUE;
	}
	if(m_iCursPoint<m_InputViewSP)
		m_InputViewSP=0;

}
void CLetterBox::CursRMove(INT iState)
{
	if(m_iCursPoint==strlen(m_pszStr))
		return;
	GetLetterCursor(m_iCursPoint);
	if(iState>=0) //SHIFT가 눌리지 않았을때
	{
		if (m_iWhatChar[m_iLetterCursor_y][m_iLetterCursor_x] ==3)  		// 영,숫자이면 1바이트
		{
			m_iCursPoint++;
		}
		else
		{
			m_iCursPoint += 2;
		}
		m_bIsBlock=FALSE;
	}
	else 					//SHIFT가 눌렸을때
	{
		if(m_bIsBlock==FALSE) 		//블럭이 잡혔는가
		{
			m_iBlockLP=m_iCursPoint;
			m_iBlockSP=m_iCursPoint;
		}
		if (m_iWhatChar[m_iLetterCursor_y][m_iLetterCursor_x] ==3)  		// 영,숫자이면 1바이트
		{
			m_iCursPoint++;//wan
		}
		else
		{
			m_iCursPoint += 2;//wan
		}
		m_iBlockLP =m_iCursPoint;
		m_bIsBlock=TRUE;
	}
}
void CLetterBox::CursUMove(INT iState)
{
	int iTemp=-1;
	GetLetterCursor(m_iCursPoint);

	if(m_iLetterCursor_y==0)
		return;

	if(iState>=0) //SHIFT가 눌리지 않았을때
	{
		if (m_iWhatChar[m_iLetterCursor_y-1][m_iLetterCursor_x] == 2)
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y-1][m_iLetterCursor_x-1];
		}
		else if(m_iWhatChar[m_iLetterCursor_y-1][m_iLetterCursor_x] == 0)
		{
			int i = 0;
			for(i=0; i<LETTER_MAX_X; i++)
			{
				if(m_iWhatChar[m_iLetterCursor_y-1][i]==4)
				{
					iTemp=i;
					i=LETTER_MAX_X;
				}
			}
			if(i>-1)
				m_iCursPoint=m_iCharIndex[m_iLetterCursor_y-1][iTemp];
		}
		else
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y-1][m_iLetterCursor_x];
		}
	}
	else
	{
		if(m_bIsBlock==FALSE) 		//블럭이 잡혔는가
		{
			m_iBlockLP=m_iCursPoint;
			m_iBlockSP=m_iCursPoint;
		}
		if (m_iWhatChar[m_iLetterCursor_y-1][m_iLetterCursor_x] == 2)
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y-1][m_iLetterCursor_x-1];
		}
		else if(m_iWhatChar[m_iLetterCursor_y-1][m_iLetterCursor_x] == 0)
		{
			int i = 0;
			for(i=0; i<LETTER_MAX_X; i++)
			{
				if(m_iWhatChar[m_iLetterCursor_y-1][i]==4)
				{
					iTemp=i;
					i=LETTER_MAX_X;
				}
			}
			if(i>-1)
				m_iCursPoint=m_iCharIndex[m_iLetterCursor_y-1][iTemp];
		}
		else
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y-1][m_iLetterCursor_x];
		}
		m_iBlockLP=m_iCursPoint;
		m_bIsBlock=TRUE;
	}
}
void CLetterBox::CursDMove(INT iState)
{
	int iTemp=-1;
	GetLetterCursor(m_iCursPoint);

	if(iState>=0) //SHIFT가 눌리지 않았을때
	{
		if (m_iWhatChar[m_iLetterCursor_y+1][m_iLetterCursor_x] == 2)
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][m_iLetterCursor_x-1];
		}
		else if(m_iWhatChar[m_iLetterCursor_y+1][m_iLetterCursor_x] == 0)
		{
			for(int i=0; i<LETTER_MAX_X; i++)
			{
				if(m_iWhatChar[m_iLetterCursor_y+1][i]==4)
				{
					iTemp=i;
					i=LETTER_MAX_X;
				}
			}
			if(iTemp>-1)
			{
				m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][iTemp];
			}
			else
			{
				for(int i=0; i<LETTER_MAX_X; i++)
				{
					if(m_iWhatChar[m_iLetterCursor_y+1][i]==0)
					{
						iTemp=i;
						i=LETTER_MAX_X;
					}
				}
				if(iTemp==0)
					m_iCursPoint=m_iCharIndex[m_iLetterCursor_y][m_iLetterCursor_x];
				else
					m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][iTemp];
			}
		}
		else
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][m_iLetterCursor_x];
		}
	}
	else
	{
		if(m_bIsBlock==FALSE) 		//블럭이 잡혔는가
		{
			m_iBlockLP=m_iCursPoint;
			m_iBlockSP=m_iCursPoint;
		}
		if (m_iWhatChar[m_iLetterCursor_y+1][m_iLetterCursor_x] == 2)
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][m_iLetterCursor_x-1];
		}
		else if(m_iWhatChar[m_iLetterCursor_y+1][m_iLetterCursor_x] == 0)
		{
			for(int i=0; i<LETTER_MAX_X; i++)
			{
				if(m_iWhatChar[m_iLetterCursor_y+1][i]==4)
				{
					iTemp=i;
					i=LETTER_MAX_X;
				}
			}
			if(iTemp>-1)
			{
				m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][iTemp];
			}
			else
			{
				for(int i=0; i<LETTER_MAX_X; i++)
				{
					if(m_iWhatChar[m_iLetterCursor_y+1][i]==0)
					{
						iTemp=i;
						i=LETTER_MAX_X;
					}
				}
				if(iTemp==0)
					m_iCursPoint=m_iCharIndex[m_iLetterCursor_y][m_iLetterCursor_x];
				else
					m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][iTemp];
			}
		}
		else
		{
			m_iCursPoint=m_iCharIndex[m_iLetterCursor_y+1][m_iLetterCursor_x];
		}
		m_iBlockLP=m_iCursPoint;
		m_bIsBlock=TRUE;
	}

}
void CLetterBox::LetterTableUpdate()
{
	LetterTableReset();
	int count=0;
	m_iCharIndex[0][0]=count;
	for(int i=0; i<LETTER_MAX_Y; i++)
	{
		for(int j=0; j<LETTER_MAX_X; j++)
		{
			if(m_pszStr[count]==13)//엔터이면
			{
				m_iWhatChar[i][j]=4;
				m_iCharIndex[i][j]=count;
				m_iCharIndex[i][j+1]=count+1;
				m_iCharIndex[i+1][0]=count+2;

				//count++;
				count+=2;
				j=LETTER_MAX_X;
			}
			else if(m_pszStr[count]<0x00 )//한글이면
			{
				if(j<LETTER_MAX_X-2)
				{
					m_LetterTable[i][j]=m_pszStr[count];
					m_LetterTable[i][j+1]=m_pszStr[count+1];
					m_iWhatChar[i][j]=1;
					m_iWhatChar[i][j+1]=2;
					m_iCharIndex[i][j]=count;
					m_iCharIndex[i][j+1]=count+1;
					m_iCharIndex[i][j+2]=count+2;
					count+=2;
					j++;
				}
				else if(j==LETTER_MAX_X-2)
				{
					m_LetterTable[i][j]=m_pszStr[count];
					m_LetterTable[i][j+1]=m_pszStr[count+1];
					m_iWhatChar[i][j]=1;
					m_iWhatChar[i][j+1]=2;
					m_iCharIndex[i][j]=count;
					m_iCharIndex[i][j+1]=count+1;
					m_iCharIndex[i+1][0]=count+2;
					count+=2;
					j++;
				}
				else
				{
					j++;
				}
			}
			else if(m_pszStr[count]==0x00)	//문자끝이면
			{
				j=LETTER_MAX_X;
				i=LETTER_MAX_Y;
			}
			else			//영어, 숫자이면
			{
				if(j<LETTER_MAX_X-1)
				{
					m_LetterTable[i][j]=m_pszStr[count];
					m_iWhatChar[i][j]=3;
					m_iCharIndex[i][j]=count;
					m_iCharIndex[i][j+1]=count+1;
					count++;
				}
				else
				{
					m_LetterTable[i][j]=m_pszStr[count];
					m_iWhatChar[i][j]=3;
					m_iCharIndex[i][j]=count;
					m_iCharIndex[i+1][0]=count+1;
					count++;
				}
			}
		}
	}
}
void CLetterBox::GetLetterCursor(int cursor)
{
	LetterTableUpdate();
	m_iLetterCursor_x=0;
	m_iLetterCursor_y=0;
	for(int i=0; i<LETTER_MAX_Y; i++)
	{
		for(int j=0; j<LETTER_MAX_X; j++)
		{
			if(m_iCharIndex[i][j]==cursor)
			{
				m_iLetterCursor_x=j;
				m_iLetterCursor_y=i;
				i=LETTER_MAX_Y;
				j=LETTER_MAX_X;
			}

		}
	}
}

void CLetterBox::LetterTableReset()
{
	ZeroMemory(m_iWhatChar,LETTER_MAX_X*LETTER_MAX_Y*sizeof(int));
	ZeroMemory(m_iCharIndex,LETTER_MAX_X*LETTER_MAX_Y*sizeof(int));
	ZeroMemory(m_LetterTable,LETTER_MAX_X*LETTER_MAX_Y*sizeof(char));
}

void CLetterBox::SetLetterFlag(bool flag)
{
	m_bLetterFlag = flag;
}




