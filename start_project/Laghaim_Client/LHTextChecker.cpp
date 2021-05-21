#include "stdafx.h"
#include ".\lhtextchecker.h"
#include <tchar.h>


bool LHTextChecker::CheckDigit(const char * text)
{
	for ( unsigned char *lpszCheck = ( unsigned char*)text; *lpszCheck; ++lpszCheck)
	{
		if ( 1 == _mbclen( lpszCheck))
		{
			// 한 바이트
			int chr = *lpszCheck;

			if (chr >= 0x30 && chr <= 0x39)/*0 to 9*/
				continue;
			else
				return false;
		}
		else
		{
			// 두 바이트
			return false;
		}
	}
	return true;
}

bool LHTextChecker::CheckInvalidText(const char * text)
{
	for ( unsigned char *lpszCheck = ( unsigned char*)text; *lpszCheck; ++lpszCheck)
	{
		if ( 1 == _mbclen( lpszCheck))
		{
			// 한 바이트
			//if ( *lpszCheck < 0x21 || *lpszCheck > 0x7E)
			if ( *lpszCheck < 48 || ( 58 <= *lpszCheck && *lpszCheck < 65) ||
					( 91 <= *lpszCheck && *lpszCheck < 97) || *lpszCheck > 122)
			{
				return ( true);
			}
		}
		else
		{
			// 두 바이트
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( 0x81 <= *lpszCheck && *lpszCheck <= 0xC8)
			{
				// 한글
				if ( ( 0x41 <= *lpszTrail && *lpszTrail <= 0x5A) ||
						( 0x61 <= *lpszTrail && *lpszTrail <= 0x7A) ||
						( 0x81 <= *lpszTrail && *lpszTrail <= 0xFE))
				{
					// 투명글자 뺀부분
					// 안되는 특수문자영역들
					if ( 0xA1 <= *lpszCheck && *lpszCheck <= 0xAF && 0xA1 <= *lpszTrail)
					{
						return ( true);
					}
					else if ( *lpszCheck == 0xC6 && 0x53 <= *lpszTrail && *lpszTrail <= 0xA0)
					{
						return ( true);
					}
					else if ( 0xC7 <= *lpszCheck && *lpszCheck <= 0xC8 && *lpszTrail <= 0xA0)
					{
						return ( true);
					}
				}
				else
				{
					return ( true);
				}
			}
			else
			{
				return ( true);
			}

			++lpszCheck;
		}
	}
	return false;
}

bool LHTextChecker::CheckSpecailText(const char * text)
{
	return false;
	for ( unsigned char *lpszCheck = ( unsigned char*)text; *lpszCheck; ++lpszCheck)
	{
		if ( 1 == _mbclen( lpszCheck))
		{
			// 한 바이트
			if ( *lpszCheck < 0x21 || ( *lpszCheck > 0x7E && *lpszCheck < 0xA1) ||
					*lpszCheck > 0xDF)
			{
				return ( true);
			}
		}
		else
		{
			// 두 바이트
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( ( 0x88 <= *lpszCheck && *lpszCheck <= 0x9F) ||
					( 0xE0 <= *lpszCheck && *lpszCheck <= 0xEA) ||
					( 0xED <= *lpszCheck && *lpszCheck <= 0xEE) ||
					( 0xFA <= *lpszCheck && *lpszCheck <= 0xFC))
			{
				// 한자
				if ( 0x40 <= *lpszTrail && *lpszTrail <= 0xFC)
				{
					// 사용되는 범위
					if ( *lpszTrail == 0x7F)
					{
						// 공백
						return ( true);
					}
					if ( *lpszCheck == 0x88 && *lpszTrail <= 0x9E)
					{
						// 공백
						return ( true);
					}
					if ( *lpszCheck == 0x98 && ( 0x73 <= *lpszTrail && *lpszTrail <= 0x9E))
					{
						// 공백
						return ( true);
					}
					if ( *lpszCheck == 0xEA && 0xA5 <= *lpszTrail)
					{
						// 공백
						return ( true);
					}
					if ( *lpszCheck == 0xEE && 0xED <= *lpszTrail)
					{
						// 특수 문자 와 공백
						return ( true);
					}
					if ( *lpszCheck == 0xFC && 0x4C <= *lpszTrail)
					{
						// 공백
						return ( true);
					}
				}
				else
				{
					return ( true);
				}
			}
			else if ( *lpszCheck == 0x81)
			{
				if ( *lpszTrail != 0x58 && *lpszTrail != 0x5B)
				{
					return ( true);
				}
			}
			else if ( *lpszCheck == 0x82)
			{
				if ( *lpszTrail <= 0x4E || ( 0x59 <= *lpszTrail && *lpszTrail <= 0x5F) ||
						( 0x7A <= *lpszTrail && *lpszTrail <= 0x80) ||
						( 0x9B <= *lpszTrail && *lpszTrail <= 0x9E) ||
						( 0xF2 <= *lpszTrail))
				{
					// 일부 전각/히라가나를 제외한 공백
					return ( true);
				}
			}
			else if ( *lpszCheck == 0x83)
			{
				if ( *lpszTrail < 0x40 || 0x97 <= *lpszTrail)
				{
					// 히라가나 외의 공백 및 특수문자
					return ( true);
				}
			}
			else
			{
				return ( true);
			}
			++lpszCheck;
		}
	}

	return false;
}
