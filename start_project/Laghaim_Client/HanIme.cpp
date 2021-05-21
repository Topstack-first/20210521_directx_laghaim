#include "stdafx.h"
#include "HanIme.h"

#include <wchar.h>
#include <math.h>

#define DEFAULT_CAND_NUM_PER_PAGE 5

extern DWORD g_dwLangType;
extern BOOL g_ThaiClient;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHanIme::CHanIme()
	: m_iLetterCursor_x(0)
	, m_iLetterCursor_y(0)
	, m_cFChar(0)
{
	memset(m_iWhatChar, 0, sizeof(m_iWhatChar));
	memset(m_iCharIndex, 0, sizeof(m_iCharIndex));
	memset(m_LetterTable, 0, sizeof(m_LetterTable));
	memset(m_sKeyBuf, 0, sizeof(m_sKeyBuf));
	memset(m_convertedPos, 0, sizeof(m_convertedPos));

//////// 채팅에디트  ///////////////////////////////////////////
	m_strInputView = new char[IME_MAX_STRING];
	m_iCursPoint=0;			//커서 위치
	m_InputViewSP=0;		//채팅 입력창 보이는 첫글자 위치

	m_iBlockSP=0;			//블록이 잡힐때 첫위치
	m_iBlockLP=0;			//끝위치

	m_bIsComp=TRUE;			//조합중인가
	m_bIsBlock = FALSE;		//블록이 잡혔나
	m_bLetterFlag =FALSE;
////////////////////////////////////////////////////////////////
	m_hWnd		 = NULL;
	m_hIMC       = NULL;
	m_pszStr	 = new char[IME_MAX_STRING];			// 실제적으로 보여줄 텍스트를 위한 공간
	ZeroMemory(m_pszStr, IME_MAX_STRING);

	m_strIsHan = new char[IME_MAX_STRING];
	ZeroMemory(m_strIsHan, IME_MAX_STRING);

	m_CopyBuff = new char[IME_MAX_STRING];
	ZeroMemory(m_CopyBuff, IME_MAX_STRING);

	m_CopyIsHan= new char[IME_MAX_STRING];
	ZeroMemory(m_CopyIsHan, IME_MAX_STRING);

	m_nCharCount = 0;
	m_bIsPassword = FALSE;

	// taiwan
	m_pInputString = m_pszStr;
	m_bSkip = FALSE;
	m_nTotalBytes = m_nCurrentBytes = 0;
	// taiwan

	SetMaxLen(IME_MAX_STRING-1);
	Clear();

	m_iStringPoint = 0;
	m_iFepCurPoint = 0;
	m_bDraw = false;
	m_pCandidateList = NULL;
	m_iCandidateIndex = 0;

}

CHanIme::~CHanIme()
{
	Destroy();
}

void CHanIme::SetMaxLen(int maxLen)
{
	if (maxLen > IME_MAX_STRING-1)
		m_nMaxLen = IME_MAX_STRING-1;
	else if (maxLen < 0)
		m_nMaxLen = 0;
	else
		m_nMaxLen = maxLen;
}

void CHanIme::SetLenght(int len)
{
	m_nCharCount = len;
	// for chinese taiwan
	m_nCurrentBytes = m_nTotalBytes = len;
}
void CHanIme::Clear()
{
//////// 채팅에디트  ///////////////////////////////////////////
	m_iCursPoint=0;
	m_strInputView[0]= '\0';
	ZeroMemory(m_strInputView, IME_MAX_STRING);

	m_InputViewSP=0;
	m_iBlockLP=0;
	m_iBlockSP=0;
	m_bIsComp=TRUE;//조합끝
	m_bIsBlock=FALSE;

	m_strIsHan[0] = '\0';
	ZeroMemory(m_strIsHan, IME_MAX_STRING);
///////////////////////////////////////////////////////////////////

	m_nCharCount = 0;

	m_pszStr[0] = '\0';
	ZeroMemory(m_pszStr, IME_MAX_STRING);

	// for chinese taiwan
	m_nCurrentBytes = m_nTotalBytes = 0;

	// for Japanease taiwan
	clearString();
}

void CHanIme::Destroy()
{
	if (m_pszStr != NULL)
	{
		delete[] m_pszStr;
		m_pszStr = NULL;
	}

	if (m_strInputView != NULL)
	{
		delete[] m_strInputView;
		m_strInputView = NULL;
	}

	if (m_CopyIsHan != NULL)
	{
		delete[] m_CopyIsHan;
		m_CopyIsHan = NULL;
	}

	if (m_CopyBuff != NULL)
	{
		delete[] m_CopyBuff;
		m_CopyBuff = NULL;
	}

	if ( m_strIsHan != NULL )
	{
		delete[] m_strIsHan;
		m_strIsHan = NULL;
	}
}



LRESULT CHanIme::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	m_hWnd = hWnd;

	BOOL bJapan = FALSE;

	if (g_dwLangType == 1)  					// g_dwLangType = 1 : Japanese
	{
		return OnJapanMsg(hWnd, uMsg, wParam, lParam);
	}
	else if (g_dwLangType == 3 || g_dwLangType == 4)  						// Chinese Taiwan
	{
		return OnTaiwanMsg(hWnd, uMsg, wParam, lParam);
	}
	else
	{
		switch ( uMsg)
		{
		case WM_IME_COMPOSITION:
			if(strlen(m_pszStr)<(m_nMaxLen*2)-2 || m_bIsComp==FALSE)  	//출력버퍼가 다 안 찼을때만
			{
				OnIMEComposition(wParam, lParam);
				DrawText();
			}
			break;

		case WM_IME_ENDCOMPOSITION:
			break;

		case WM_CHAR:
			if(!m_bLetterFlag)
			{
				OnChar(wParam);
				DrawText();
			}
			break;

		case WM_KEYDOWN:
			if(!m_bLetterFlag)
			{
				OnKeyDown(wParam);	//커서이동
				DrawText();
			}
			break;
		}
	}
	return 0;
}

void CHanIme::AddJString()
{
	HIMC hIMC;

	DWORD dwLen;
	LPSTR lpResultStr;

	hIMC = ImmGetContext(m_hWnd);

	if (!hIMC)
		return;

	dwLen = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0L);

	if (dwLen)
	{
		lpResultStr = (LPSTR) GlobalAlloc(GPTR, dwLen+1);

		if (lpResultStr)
		{
			ImmGetCompositionString(hIMC, GCS_RESULTSTR, lpResultStr, dwLen+1);
			DrawCompString(lpResultStr);
			GlobalFree((HANDLE) lpResultStr);
		}
	}

	ImmReleaseContext(m_hWnd, hIMC);
}


//--------------------------------------------------------------------------------------------
//	WM_IME_COMPOSITION 메세지 처리 (한글조합중 또는 한글조합완료..)
//--------------------------------------------------------------------------------------------
LONG CHanIme::OnIMEComposition(WPARAM dwCommand, LPARAM dwData)
{
	if (dwData & GCS_RESULTSTR)
		GetResultString(); // Result String
	else if (dwData & GCS_COMPSTR)
		GetCompString(dwCommand, dwData); // Composition string
	else if (dwData & 0x01BF)
		GetResultString2();

	return 0;
}

// 한글 조합 완료

BOOL CHanIme::GetResultString()
{
	int	        len;					// Storage for length of result str.
	char        str[4];					// Pointer to result string.

	if (Enter())
	{

		// IMM_ERROR_NODATA  (-1)
		// IMM_ERROR_GENERAL (-2)
		if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > -1)
		{
			//len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0);
			if (len != 1)
			{
				ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, str, len);
				str[2] = NULL;
//채팅 에디트 수정////////////////////////////////////////
				DrawResultString(str);
				if(m_iCursPoint<(m_nMaxLen*2)-1)
					m_iCursPoint+=2;
				m_nCharCount=strlen(m_pszStr);
				m_bIsComp=true;//조합끝
///////////////////////////////////////////////////////////
			}
		}
		Leave();
	}
	return TRUE;
}

// 일본어를 위한 임시..
BOOL CHanIme::GetResultString2()
{
	int	        len;					// Storage for length of result str.
	char        str[4];					// Pointer to result string.

	if (Enter())
	{

		// IMM_ERROR_NODATA  (-1)
		// IMM_ERROR_GENERAL (-2)
		if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > -1)
		{
			if (len != 1)
			{
				ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, str, len);
				str[2] = NULL;
				DrawCompString(str);
			}
		}
		Leave();
	}

	return TRUE;
}

// 한글 조합중..
BOOL CHanIme::GetCompString(WPARAM dwCommand, LONG flag)
{
	char       str[8];              // Pointer to composition str.

	if (!Enter())
		return FALSE;

	// by evilkiki 2009.06.27 ime 관련 수정 ( 이전소스는 소스세잎에서 )
	DWORD dwBufLen = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
	if( dwBufLen == 0 )
	{
		_asm nop;
		DrawResultString(NULL);//조합중일때..

	}
	else
	{
		ImmGetCompositionString( m_hIMC , GCS_COMPSTR, str , dwBufLen );
		str[dwBufLen] = 0;
		DrawResultString(str);//조합중일때..
	}

	m_nCharCount=strlen(m_pszStr);

	Leave();

	return TRUE;
}


//--------------------------------------------------------------------------------------------
//	WM_CHAR 메세지 처리 (1바이트 문자열 처리 또는 BACK_SPACE)
//--------------------------------------------------------------------------------------------

void CHanIme::OnChar(UINT nChar)
{	
	if( m_pszStr == NULL )
		return;

	char temp[IME_MAX_STRING]= {0,};
	m_nCharCount=strlen(m_pszStr);
	
	if( nChar == 3 && m_bIsBlock==TRUE)//clrl+c이면 copybuff로 복사

	{
		ZeroMemory(m_CopyBuff, IME_MAX_STRING);
		ZeroMemory(m_CopyIsHan, IME_MAX_STRING);
		m_CopyBuff[0] = '\0';
		m_CopyIsHan[0] = '\0';

		strncpy(m_CopyBuff,m_pszStr+min(m_iBlockSP,m_iBlockLP),abs(m_iBlockSP-m_iBlockLP));
		strncpy(m_CopyIsHan,m_strIsHan+min(m_iBlockSP,m_iBlockLP),abs(m_iBlockSP-m_iBlockLP));
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

		strcpy(temp,m_strIsHan+m_iCursPoint);
		strcpy(m_strIsHan+m_iCursPoint,m_CopyIsHan);
		strcpy(m_strIsHan+(m_iCursPoint+strlen(m_CopyIsHan)),temp);

		m_iCursPoint += strlen(m_CopyBuff);
		if(m_iCursPoint > m_nMaxLen-1)
			m_InputViewSP = m_iCursPoint- (m_nMaxLen -2);
	}
	else if((nChar == 8 )|| (nChar == 27))
	{
		if(!m_nCharCount|| !m_iCursPoint)//커서포인트가 0일때도 리턴
			return;

		if(m_bIsBlock==TRUE) 		//블럭이 지정되 있으면 블럭만 지움
		{

			strcpy(temp,m_pszStr+max(m_iBlockSP,m_iBlockLP));
			strcpy(m_pszStr+min(m_iBlockSP,m_iBlockLP),temp);

			strcpy(temp,m_strIsHan+max(m_iBlockSP,m_iBlockLP));
			strcpy(m_strIsHan+min(m_iBlockSP,m_iBlockLP),temp);

			m_iCursPoint=min(m_iBlockSP,m_iBlockLP);
			m_bIsBlock=FALSE;
		}
		else
		{
			if (m_strIsHan[m_iCursPoint-1] =='2')  		// 한글이면 2바이트를 지운다
			{
				if(m_iCursPoint<2)
				{
					ClearCopyBuff();
					return;
				}
				strcpy(temp,m_pszStr+m_iCursPoint);
				strcpy(m_pszStr+(m_iCursPoint-2),temp);

				strcpy(temp,m_strIsHan+m_iCursPoint);
				strcpy(m_strIsHan+(m_iCursPoint-2),temp);

				m_iCursPoint-=2;
			}
			else
			{
				if(m_iCursPoint<1)
				{
					ClearCopyBuff();
					return;
				}
				strcpy(temp,m_pszStr+m_iCursPoint);
				strcpy(m_pszStr+(m_iCursPoint-1),temp);

				strcpy(temp,m_strIsHan+m_iCursPoint);
				strcpy(m_strIsHan+(m_iCursPoint-1),temp);

				m_iCursPoint-=1;
			}
		}
		if(m_iCursPoint<m_InputViewSP || strlen(m_pszStr)<m_nMaxLen )
			m_InputViewSP=0;

	}
	// 영문일때에만..처리한다
	// 이유는 한글 조합이 완료되면 m_nCharCount는 이미증가한다
	//DrawCompString()에서 len만큼 증가시킨다
	else if ((g_ThaiClient && !iscntrl(nChar)))
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
	else if ( !iscntrl(nChar) && isascii(nChar) )
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
	
	else if ((g_dwLangType == 6 && !iscntrl(nChar)))
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
	else if ((g_dwLangType == 8 && !iscntrl(nChar)))
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
	else if ((g_dwLangType == 9 && !iscntrl(nChar)))     // 06-09-21 브라질 특수문자를 위한 예외처리 추가.
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}

	m_nCharCount=strlen(m_pszStr);
	m_bIsBlock=FALSE;
}

#include <winnls.h>
void CHanIme::OnJChar(UINT nChar)
{
	int i, len, z;
	bool bTwobyte = false;

	if((nChar == 8 )|| (nChar == 27))
	{
		if(!m_nCharCount)
			return;

		len = strlen(m_pszStr);
		for(i=0; i<len; i++)
		{
			// ASCII ..
			if((unsigned char)m_pszStr[i] >= 0x20 && (unsigned char)m_pszStr[i] <=0x7E)
			{
				bTwobyte = false;
			}
			// 2BYTE STRING (HANJA first byte / kata(JUN GAK) / hira)
			else if((unsigned char)m_pszStr[i] >= 0x81 && (unsigned char)m_pszStr[i] <=0x9F)
			{
				bTwobyte = true;
				i ++;
			}
			// 2BYTE STING (HANJA first byte)
			else if((unsigned char)m_pszStr[i] >= 0xE0 && (unsigned char)m_pszStr[i] <=0xEF)
			{
				bTwobyte = true;
				i ++;
			}
			// 1BYTE STRING
			else
				bTwobyte = false;
		}

		bTwobyte ? z = 2 : z = 1;

		if((unsigned char)m_pszStr[m_nCharCount-z] >= 0x81 && (unsigned char)m_pszStr[m_nCharCount-z] <=0x9F)
		{
			m_nCharCount -=2;
			m_pszStr[m_nCharCount] = NULL;
		}
		else
		{
			if((unsigned char)m_pszStr[m_nCharCount-z] >= 0xE0 && (unsigned char)m_pszStr[m_nCharCount-z] <=0xEF)
			{
				m_nCharCount -=2;
				m_pszStr[m_nCharCount] = NULL;
			}
			else
				m_pszStr[--m_nCharCount] = NULL;
		}
	}

	else if ( !iscntrl(nChar) && isascii(nChar) )
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		DrawCompString(str);
	}
}


void CHanIme::DrawCompString(LPSTR str)
{
	char temp[IME_MAX_STRING]= {0,};
	int len = strlen(str);

//	if((len+m_nCharCount) > m_nMaxLen*2 ||(m_bIsPassword && (len+m_nCharCount) > m_nMaxLen) )	 ///패스워드_스크롤
	if((len+m_nCharCount) > m_nMaxLen*2 )
		return;

	strncpy(temp,m_pszStr,m_iCursPoint);
	strncpy(temp + m_iCursPoint,str,strlen(str));
	strncpy(temp +strlen(temp),&(m_pszStr[m_iCursPoint]),strlen(m_pszStr)-m_iCursPoint);
	strcpy(m_pszStr,temp);


	strcpy(temp,m_strIsHan+m_iCursPoint);
	strcpy(m_strIsHan+m_iCursPoint,"0");
	strcpy(m_strIsHan+(m_iCursPoint+1),temp);

	m_nCharCount = strlen(m_pszStr);
	m_iCursPoint += strlen(str);
}

void CHanIme::DrawResultString(LPSTR str)
{
	int len = 0;

	if( str )
		len = strlen(str);


	char temp[IME_MAX_STRING]= {0,};

//	if((len+m_nCharCount) > m_nMaxLen*2 ||(m_bIsPassword && (len+m_nCharCount) > m_nMaxLen) )	///패스워드_스크롤
	if((len+m_nCharCount) > m_nMaxLen*2 )
		return;

	if(m_bIsComp==TRUE) //조합시작 삽입
	{
		strncpy(temp,m_pszStr,m_iCursPoint);
		strncpy(temp + m_iCursPoint,str,2);
		strncpy(temp +strlen(temp),&(m_pszStr[m_iCursPoint]),strlen(m_pszStr)-m_iCursPoint);
		strcpy(m_pszStr,temp);

		strcpy(temp,m_strIsHan+m_iCursPoint);
		strcpy(m_strIsHan+m_iCursPoint,"12");
		strcpy(m_strIsHan+(m_iCursPoint+2),temp);

		m_bIsComp=FALSE;
	}
	else 	//조합중 덮어씌우기
	{

		strncpy(temp,m_pszStr,m_iCursPoint);
		if( str )
			strncpy(temp + m_iCursPoint,str,2);

		// 채팅중 튕기던 버그 수정.(by 원석)
		int len = strlen(m_pszStr); // 그냥 strlen(m_pszStr) 을 밑에 조건문에서 쓰면 문제시점에서 제대로 안되더라. 그래서 변수에 담은 후에 사용.
		if( len-(m_iCursPoint+2) >= 0 ) // 이게 음수로 되서 튕기는 버그가 있었다.(근본적인 원인은 여기가 아닌듯하지만 일단 여기서 땜빵식으로 막는다.)
		{
			strncpy(temp+strlen(temp),&(m_pszStr[m_iCursPoint+2]),strlen(m_pszStr)-(m_iCursPoint+2)); // 여기가 채팅튕김
		}

		strcpy(m_pszStr,temp);
	}
	m_nCharCount=strlen(m_pszStr);
	if(m_nCharCount==m_iCursPoint||len==0) //조합중에 지워지면....
	{
		if( m_iCursPoint - 2 >= 0 )
		{
			strcpy(temp,m_strIsHan+m_iCursPoint);
			strcpy(m_strIsHan+(m_iCursPoint-2),temp);
		}
		
		m_bIsComp=TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Chinese Taiwan
LRESULT CHanIme::OnTaiwanMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		switch( wParam  )
		{
		case VK_BACK:
			return OnTaiwanBackSpace();
		}
		break;

	case WM_CHAR:
		//if( wParam == VK_BACK )
		//{	return OnTaiwanBackSpace();	}

		if (m_pInputString)
		{
			char *pTemp = (char *)&wParam;

			if (m_bSkip)
			{
				if (m_nTotalBytes < m_nMaxLen)
				{
					if (m_nCurrentBytes < m_nTotalBytes)
					{
						for (short i=m_nTotalBytes-1; i>=m_nCurrentBytes; i--)
						{
							m_pszStr[i+1] = m_pInputString[i];
						}
					}
					m_pInputString[m_nCurrentBytes] = m_cFChar;
					m_nCurrentBytes++;
					m_nTotalBytes++;
					m_pInputString[m_nCurrentBytes]=pTemp[0];
					m_nCurrentBytes++;
					m_nTotalBytes++;
					DrawText();
				}
				m_bSkip = FALSE;
			}
			else
			{
				if (pTemp[0] < 1)
				{
					m_cFChar=pTemp[0];
					m_bSkip = TRUE;
				}
				else if (isprint(pTemp[0]))
				{
					if (m_nTotalBytes < m_nMaxLen)
					{
						if (m_nCurrentBytes < m_nTotalBytes)
						{
							for ( short i=m_nTotalBytes-1; i>=m_nCurrentBytes; i--)
							{
								m_pInputString[i+1] = m_pInputString[i];
							}
						}
						m_pInputString[m_nCurrentBytes]=pTemp[0];
						m_nCurrentBytes++;
						m_nTotalBytes++;
					}
					DrawText();
				}
			}
			break;
		}
		break;

	default:
		break;
	}
	return 0;
}


BOOL CHanIme::OnTaiwanBackSpace()
{
	if (m_pInputString)
	{
		if (m_nCurrentBytes > 0)
		{
			short i, j;

			for (j=0; j < m_nCurrentBytes; j++)
			{
				if (m_pInputString[j] < 0)
				{
					if ( j+2 == m_nCurrentBytes)
						break;
					else
						j++;
				}
			}

			if (j+2 == m_nCurrentBytes)
			{
				m_nCurrentBytes -= 2;
				for (i=m_nCurrentBytes; i <m_nTotalBytes; i++)
				{
					m_pInputString[i] = m_pInputString[i+2];
				}
				m_nTotalBytes -= 2;
				m_pInputString[m_nTotalBytes] = m_pInputString[m_nTotalBytes+1] = '\0';
			}
			else
			{
				m_nCurrentBytes--;
				for (i=m_nCurrentBytes; i < m_nTotalBytes; i++)
				{
					m_pInputString[i] = m_pInputString[i+1];
				}
				m_nTotalBytes--;
				m_pInputString[m_nTotalBytes]= '\0';
			}
			DrawText();
		}
	}
	return 1;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// Japanese Font
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
LRESULT CHanIme::OnJapanMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HIMC	hImc;
	HWND	hIMEwnd;

	// 기본 IME 윈도우 없애기
	hIMEwnd = ImmGetDefaultIMEWnd(m_hWnd);
	SendMessage(hIMEwnd, WM_CLOSE, 0, 0);

	// 직접 접근 하기 위해서 핸들값 얻어오기
	hImc = ImmGetContext(m_hWnd);
	m_hIMC = hImc;

	switch(uMsg)
	{
	case WM_IME_SETCONTEXT:			// 위에서 창을 닫았기 땜에 필요없음
		return 0;
	case WM_CHAR:
		OnJapanChar(wParam);		// 일본어 입력 String 조합
		break;
	case WM_KEYDOWN:
		OnJKeyDown(wParam);			// 커서 포지션 이동시 처리 ( 05-06-27 일본 전용함수로 따로 처리. by 원석 )
		break;
	default:
		break;
	}

	OnJapanDrawText();				// 실제적으로 변화되고 있거나 완료된 글자 그리기
//	drawCandidate();				// TextBox 에 있는 현재 변환 가능한 폰트 나열

	return 1;
}

void CHanIme::clearString()			// 재입력을 위한 초기화
{
	m_strBuff.erase();
	m_iStringPoint = 0;
	m_bDraw = true;
}

bool CHanIme::OnJapanChar(CHAR cChar)
{
	static bool lb_char_MBCS= false;
	static bool lb_char_MBCS_Denied = false;

	if((int)cChar == VK_RETURN)
		m_bDraw = true;
	else if((int)cChar == VK_TAB)
		m_bDraw = true;
	else if((int)cChar == VK_ESCAPE)
		m_bDraw = true;
	else if((int)cChar == VK_BACK)
		backChar();
	else
	{
		//------------------------------------------------------------------------
		//-- 1 Byte Character
		if(((unsigned char)cChar >= 0x81
				&& (unsigned char)cChar < 0xa0)
				|| ((unsigned char)cChar >= 0xe0
					&& (unsigned char)cChar < 0xeb))
		{
			lb_char_MBCS  = !lb_char_MBCS;

			int len = 0;
			len = strlen(m_strBuff.c_str());

			//if( len < m_nMaxLen-1-(int)lb_char_MBCS )
			if(m_nCharCount+strlen(m_CopyBuff ) < m_nMaxLen*2 -1) // 문자열 길이 두배로 [2/21/2007 Theodoric]
			{
				if(lb_char_MBCS_Denied)
				{
					lb_char_MBCS_Denied = false;
					return true;
				}
				m_strBuff.insert(m_iStringPoint,1,cChar);
				m_iStringPoint++;			// 현재 커서의 위치
			}
			else
				lb_char_MBCS_Denied = true;

		}
		//------------------------------------------------------------------------
		//-- 1 Byte Character
		else if(lb_char_MBCS  )
		{
			lb_char_MBCS   =false;

			int len = 0;
			len = strlen(m_strBuff.c_str());

			//if( len < m_nMaxLen-1-(int)lb_char_MBCS )
			if(m_nCharCount+strlen(m_CopyBuff ) < m_nMaxLen*2 -1) // 문자열 길이 두배로 [2/21/2007 Theodoric]
			{
				if(lb_char_MBCS_Denied)
				{
					lb_char_MBCS_Denied = false;
					return true;
				}
				m_strBuff.insert(m_iStringPoint,1,cChar);
				m_iStringPoint++;			// 현재 커서의 위치
			}
		}
		//------------------------------------------------------------------------
		//-- 1 Byte Character
		else
		{
			int len = 0;
			len = strlen(m_strBuff.c_str());

			//if( len < m_nMaxLen-1 )
			if(m_nCharCount+strlen(m_CopyBuff ) <m_nMaxLen*2 -1) // 문자열 길이 두배로 [2/21/2007 Theodoric]
			{
				m_strBuff.insert(m_iStringPoint,1,cChar);
				m_iStringPoint++;			// 현재 커서의 위치
			}
		}
	}

	strcpy(m_pszStr, m_strBuff.c_str());	// TextBox 에서 m_pszStr 처리 위해
	m_bDraw = true;							// 바뀐 글자가 있으면 다시 그림

	return true;
}

bool CHanIme::backChar()
{
	if(m_iStringPoint == 0)
		return false;

	moveCount(-1);			// BackSpace Key 시 m_iStringPoint 처리

	LPCSTR pString = m_strBuff.c_str();
	if(isSJIS(pString[m_iStringPoint]))
		m_strBuff.erase(m_iStringPoint,2);	// 현재 커서 위치에서 한 글자 삭제
	else
		m_strBuff.erase(m_iStringPoint,1);

	return true;
}

INT CHanIme::moveCount(INT iCount)
{
	INT iStringCount = getStringCount(m_iStringPoint) + iCount;
	INT iStringPoint = getStringPoint(iStringCount);
	m_iStringPoint = iStringPoint;

	return iStringCount;
}

INT CHanIme::getStringCount(INT iStringPoint)
{
	INT i;
	INT iCount;

	if(iStringPoint <= 0)
		return 0;

	LPCSTR pString = m_strBuff.c_str();

	for(i=0,iCount=0; i!=iStringPoint && pString[i]; iCount++,i++)
	{
		if(isSJIS(pString[i]))		// 조합중인 일본어 일때 2바이트씩 글자수를 셈
			i++;
	}

	return iCount;					// 커서 위치에 따른 글자 수
}

INT CHanIme::getStringPoint(INT iStringCount)
{
	INT i;
	INT iCount;

	if(iStringCount <= 0)
		return 0;

	LPCSTR pString = m_strBuff.c_str();

	for(i=0,iCount=0; iCount!=iStringCount && pString[i]; iCount++,i++)
	{
		if(isSJIS(pString[i]))
			i++;
	}

	return i;						// 조합중인 글자 수에 따른 커서 위치
}

bool CHanIme::OnJKeyDown(CHAR cChar) // 일본용 KeyDown 메세지.(원석이 추가)
{
	INT iStringCount;
	INT iStringPoint;
	m_nCharCount=strlen(m_pszStr);
	INT iState;
	iState=GetAsyncKeyState(VK_SHIFT); // 일본은 아직 실질적으로 블럭 복사 처리가 안되어있다.(하지만 추후 들어갈것을 생각해 내비둠.)

	switch(cChar)
	{
	case VK_DELETE:

		delJChar();					// 현재 커서 위치 아래 있는 글자 지우기
		m_bDraw = true;
		break;

	case VK_LEFT:					// 커서 위치 변환

		if( m_iStringPoint <= 0 ) // 0이면 더이상 왼쪽으로 못감.
			break;

		iStringCount = getStringCount(m_iStringPoint);
		iStringPoint = getStringPoint(iStringCount-1);
		m_iStringPoint = iStringPoint;

		m_bDraw = true;
		break;

	case VK_RIGHT:

		// 커서 위치 변환
		if(m_iStringPoint>=m_nCharCount) // 글자들 끝에선 더이상 오른쪽으로 못감.
			break;

		iStringCount = getStringCount(m_iStringPoint);
		iStringPoint = getStringPoint(iStringCount+1);
		m_iStringPoint = iStringPoint;

		m_bDraw = true;
		break;

	case VK_HOME:
		m_iCursPoint = 0;
		m_InputViewSP=0;
		break;

	case VK_END:
		m_iCursPoint =strlen(m_pszStr);

		if(m_iCursPoint>m_nMaxLen-1)
			m_InputViewSP = m_iCursPoint - m_nMaxLen  ;

		break;
	}

	return true;
}


bool CHanIme::OnKeyDown(CHAR cChar)
{
	m_nCharCount=strlen(m_pszStr);
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

	case VK_HOME:
		m_iCursPoint = 0;
		m_InputViewSP=0;
		break;

	case VK_END:
		m_iCursPoint =strlen(m_pszStr);

		if(m_iCursPoint>m_nMaxLen-1)
			m_InputViewSP = m_iCursPoint - m_nMaxLen  ;

		break;
	}

	return true;
}

bool CHanIme::delJChar() // 일본어용 Del 키 처리.
{
	LPCSTR pString = m_strBuff.c_str();

	if( isSJIS(pString[m_iStringPoint]) || pString[m_iStringPoint] < 0 ) // 0보다 작으면 멀티바이트(2바이트문자) 다. 일본어 뿐이아니라 멀티바이트도 체크해준다.(일본어 이외의 멀티바이트를 이용해 글자를 깨서 악용하는 경우가 있다고 함)
		m_strBuff.erase(m_iStringPoint,2);
	else
		m_strBuff.erase(m_iStringPoint,1);

	strcpy( m_pszStr, m_strBuff.c_str() );	// TextBox 에서 m_pszStr 처리 위해

	return true;
}

bool CHanIme::delChar()
{
	char temp[IME_MAX_STRING]= {0,};

	if(m_bIsBlock==TRUE)
	{
		//블럭이 지정되 있으면 블럭만 지움
		strcpy(temp,m_pszStr+max(m_iBlockSP,m_iBlockLP));
		strcpy(m_pszStr+min(m_iBlockSP,m_iBlockLP),temp);

		strcpy(temp,m_strIsHan+max(m_iBlockSP,m_iBlockLP));
		strcpy(m_strIsHan+min(m_iBlockSP,m_iBlockLP),temp);

		m_iCursPoint = min(m_iBlockSP,m_iBlockLP);
		m_bIsBlock=FALSE;
	}
	else if (m_iCursPoint < strlen(m_pszStr))
	{
		if (m_strIsHan[m_iCursPoint] =='1')  		// 한글이면 2바이트를 지운다
		{
			strcpy(temp,m_pszStr+(m_iCursPoint+2));
			strcpy(m_pszStr+m_iCursPoint,temp);

			strcpy(temp,m_strIsHan+(m_iCursPoint+2));
			strcpy(m_strIsHan+m_iCursPoint,temp);

			m_nCharCount=strlen(m_pszStr);
		}
		else
		{
			strcpy(temp,m_pszStr+(m_iCursPoint+1));
			strcpy(m_pszStr+m_iCursPoint,temp);

			strcpy(temp,m_strIsHan+(m_iCursPoint+1));
			strcpy(m_strIsHan+m_iCursPoint,temp);
		}
	}
	if(strlen(m_pszStr)<m_nMaxLen)
		m_InputViewSP=0;
	return true;
}


bool CHanIme::OnJapanDrawText()
{
	if(!m_hWnd)
		return false;

	std::string strFep;
	getString(strFep);						// 현재 변환 중인 글자 얻기
	getAttrPos();
	static INT iconvertedPos = 0;

	INT iFepCount = 0;
	if(m_strFep.c_str()[0])
	{
		iFepCount = getCursorCount();		// 현재 커서 위치 반환

		if( iFepCount + m_iStringPoint > m_nMaxLen )
			iFepCount = m_nMaxLen - m_iStringPoint;
	}

	// 변환된 글자가 있거나 커서의 위치가 변하면 업데이트 하고 그려줌
	if(strFep != m_strFep || m_iFepCurPoint != iFepCount || m_convertedPos[1] != iconvertedPos)
	{
		iconvertedPos = m_convertedPos[1];
		m_iFepCurPoint = iFepCount;
		m_strFep = strFep.c_str();
		m_bDraw = true;
	}

	if(m_bDraw)
	{
		m_bDraw = false;
		drawIMEString();			// TextBox 의 drawIMEString()
	}

	return 1;
}

bool CHanIme::getString(std::string& strBuff) const
{
	LONG lByte;
	lByte = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
	if( lByte > m_nMaxLen )
		lByte = m_nMaxLen;

	LPSTR pBuff = new CHAR[lByte+1];
	ImmGetCompositionString(m_hIMC, GCS_COMPSTR, pBuff, lByte);
	pBuff[lByte] = 0;
	strBuff = pBuff;				// 현재 변환중인 String 얻기
	delete [] pBuff;

	return true;
}

bool CHanIme::getAttrPos()
{
	LONG lByte;
	lByte = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);

	std::string strFw;

	char *limit = NULL;

	strFw = m_strFep.substr(0,m_nMaxLen);

	if( lByte > m_nMaxLen )
		lByte = m_nMaxLen;

	LPSTR pBuff = new CHAR[lByte+1];
	ImmGetCompositionString(m_hIMC, GCS_COMPATTR, pBuff, lByte);
	pBuff[lByte] = 0;

	int test[255] = {0,};
	bool isFirstOK = FALSE;

	m_convertedPos[0] = 0;
	m_convertedPos[1] = 0;

	for( int n = 0; n < lByte; n++ )
	{
		if ( pBuff[n] == ATTR_INPUT )
			test[n] = ATTR_INPUT;
		else if( pBuff[n] == ATTR_TARGET_CONVERTED )
			test[n] = ATTR_TARGET_CONVERTED;
		else if( pBuff[n] == ATTR_CONVERTED )
			test[n] = ATTR_CONVERTED;
		else if( pBuff[n] == ATTR_TARGET_NOTCONVERTED )
			test[n] = ATTR_TARGET_NOTCONVERTED;
		else if( pBuff[n] == ATTR_INPUT_ERROR )
			test[n] = ATTR_INPUT_ERROR;
		else if( pBuff[n] == ATTR_FIXEDCONVERTED )
			test[n] = ATTR_FIXEDCONVERTED;
		else
			test[n] = 0;

		if( test[n] == ATTR_TARGET_CONVERTED )
		{
			if( !isFirstOK )
			{
				m_convertedPos[0] = n;
				isFirstOK = TRUE;
			}
			else if( n >= m_convertedPos[0] )
				m_convertedPos[1] = n;
		}
		else if( test[n] == ATTR_TARGET_NOTCONVERTED )
		{
			if( !isFirstOK )
			{
				m_convertedPos[0] = n;
				isFirstOK = TRUE;
			}
			else if( n >= m_convertedPos[0] )
				m_convertedPos[1] = n;
		}

	}

	delete [] pBuff;

	return true;
}

INT CHanIme::getConvertPos() const
{
	DWORD dwBuff[2];
	if(ImmGetCompositionString(m_hIMC, GCS_COMPCLAUSE, dwBuff, sizeof(DWORD)*2) == 0)
		return 0;
	return dwBuff[1];
}

INT CHanIme::getCursorCount() const
{
	return ImmGetCompositionString(m_hIMC, GCS_CURSORPOS, NULL, 0);
}

INT CHanIme::getCountCandidate()
{
	if(m_pCandidateList)
	{
		delete m_pCandidateList;
		m_pCandidateList = NULL;
	}

	if( m_iStringPoint > m_nMaxLen - 1 )
		return 0;

	// 일본어 변환 폰트 리스트를 나열
	INT iCandidateSize = ImmGetCandidateList(m_hIMC, 0, NULL, 0);
	if(!iCandidateSize)
		return 0;

	m_pCandidateList = (LPCANDIDATELIST)new BYTE[iCandidateSize];
	if( !m_pCandidateList )
		return NULL;

	ImmGetCandidateList(m_hIMC, 0, m_pCandidateList, iCandidateSize);

	return m_pCandidateList->dwCount;
}

INT CHanIme::getCandidateIndex() const
{
	// 현재 선택 되어 있는 폰트 인덱스 값
	if( !m_pCandidateList )
		return NULL;

	return m_pCandidateList->dwSelection;
}

INT CHanIme::getCandidateStart(INT iPageCount) const
{
	// 첫번째 String의 인덱스
	return (iPageCount * 9);// + 1;
}

INT CHanIme::getCandidateEnd(INT iPageCount) const
{
	// 한 페이지에 보여지는 폰트의 최대 개수
	int iCount = 0;

	iCount = (iPageCount * 9) + 9;

	if(m_pCandidateList && iCount > (int)m_pCandidateList->dwCount)
		iCount = m_pCandidateList->dwCount;

	return iCount;
}

LPCSTR CHanIme::getCandidateString(INT iIndex) const
{
	if(!m_pCandidateList)
		return NULL;

	// 폰트 리스트 중 인덱스에 맞는 폰트를 반환
	return (LPCSTR)((LPBYTE)m_pCandidateList+m_pCandidateList->dwOffset[iIndex]);
}

VOID CHanIme::setCandidatePage(INT iStartCount)
{
	if( m_pCandidateList )
		m_pCandidateList->dwPageStart = iStartCount;
	return;
}

INT CHanIme::getPageCount(INT iIndex)
{
	int iPageCount = 0;

	if( iIndex >= 1 && iIndex < 9 )
		iPageCount = 0;
	if( iIndex >= 9 && iIndex < 18 )
		iPageCount = 1;
	if( iIndex >= 18 && iIndex < 27 )
		iPageCount = 2;
	if( iIndex >= 27 && iIndex < 36 )
		iPageCount = 3;
	if( iIndex >= 36 && iIndex < 45 )
		iPageCount = 4;
	if( iIndex >= 45 && iIndex < 54 )
		iPageCount = 5;
	if( iIndex >= 54 && iIndex < 63 )
		iPageCount = 6;
	if( iIndex >= 63 && iIndex < 72 )
		iPageCount = 7;
	if( iIndex >= 72 && iIndex < 81 )
		iPageCount = 8;
	if( iIndex >= 81 && iIndex < 90 )
		iPageCount = 9;
	if( iIndex >= 90 && iIndex < 99 )
		iPageCount = 10;

	return iPageCount;
}

void CHanIme::getConversionStatus()
{
	DWORD dwConversion;
	DWORD dwSentence;

	ImmGetConversionStatus(m_hIMC, &dwConversion, &dwSentence);

	m_strStat.erase();

	if( !(ImmGetOpenStatus(m_hIMC)) || dwConversion & IME_CMODE_NOCONVERSION )
		m_strStat.insert(0, "뎟");
	else
	{
		if(dwConversion & IME_CMODE_FULLSHAPE)
			m_strStat.insert(0, "멣");
		else
			m_strStat.insert(0, "뵾");
	}
}

void CHanIme::SetCloseIMC()
{
	if( ImmGetOpenStatus(m_hIMC) == TRUE )
	{
		ImmSetOpenStatus(m_hIMC, FALSE);
		m_iFepCurPoint = 0;
		m_iStringPoint = 0;
	}
	return;
}

void CHanIme::ClearCopyBuff()
{
	m_CopyBuff[0] = '\0';
	ZeroMemory(m_CopyBuff, IME_MAX_STRING);
	m_CopyIsHan[0] = '\0';
	ZeroMemory(m_CopyIsHan, IME_MAX_STRING);
}//복사버퍼 비움

//////// 채팅에디트  ///////////////////////////////////////////
void CHanIme::CursLMove(INT iState)
{
	if(m_iCursPoint==0)
		return;

	if(iState>=0) //SHIFT가 눌리지 않았을때
	{
		if (m_strIsHan[m_iCursPoint-1] =='2')		// 한글이면 2바이트를 지운다
			m_iCursPoint -= 2;
		else
			m_iCursPoint--;

		m_bIsBlock=FALSE;
	}
	else 					//SHIFT가 눌렸을때
	{
		if(m_bIsBlock==FALSE) 		//블럭이 잡혔는가
		{
			m_iBlockLP=m_iCursPoint;
			m_iBlockSP=m_iCursPoint;
		}
		if (m_strIsHan[m_iCursPoint-1] =='2')  		// 한글이면 2바이트를 지운다
		{
			m_iCursPoint -= 2;
			m_iBlockLP -=2;
		}
		else
		{
			m_iCursPoint--;
			m_iBlockLP --;
		}
		m_bIsBlock=TRUE;
	}
	if(m_iCursPoint<m_InputViewSP)
		m_InputViewSP=0;

}
void CHanIme::CursRMove(INT iState)
{
	if(m_iCursPoint==strlen(m_pszStr))
		return;

	if(iState>=0) //SHIFT가 눌리지 않았을때
	{
		if (m_strIsHan[m_iCursPoint] =='1')  		// 한글이면 2바이트를 지운다
		{
			m_iCursPoint += 2;
		}
		else
		{
			m_iCursPoint++;
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
		if (m_strIsHan[m_iCursPoint] =='1')  		// 한글이면 2바이트를 지운다
		{
			m_iCursPoint += 2;//wan
			m_iBlockLP +=2;
		}
		else
		{
			m_iCursPoint++;//wan
			m_iBlockLP ++;
		}
		m_bIsBlock=TRUE;
	}
}

//------------------------------------
//넷마블 유저 체크
void CHanIme::ConvertNetId(BOOL check)
{
	char temp[IME_MAX_STRING]= {0,};

	if(check)
	{
		if(strlen(m_pszStr)+4>(m_nMaxLen*2))
			return;

		strcpy(temp,m_pszStr);
		Clear();
		strcpy(m_pszStr,"nm_");
		strcpy(m_pszStr+3,temp);
	}
	else
	{
		strncpy(temp,m_pszStr,3);
		if(!strcmp(temp,"nm_"))
		{
			char temp2[IME_MAX_STRING]= {0,};
			strcpy(temp2,m_pszStr+3);
			Clear();
			strcpy(m_pszStr,temp2);
		}
	}
	m_iCursPoint = strlen(m_pszStr);
	DrawText();
}
//------------------------------------