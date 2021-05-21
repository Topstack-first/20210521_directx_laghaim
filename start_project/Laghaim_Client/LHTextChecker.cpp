#include "stdafx.h"
#include ".\lhtextchecker.h"
#include <tchar.h>


bool LHTextChecker::CheckDigit(const char * text)
{
	for ( unsigned char *lpszCheck = ( unsigned char*)text; *lpszCheck; ++lpszCheck)
	{
		if ( 1 == _mbclen( lpszCheck))
		{
			// �� ����Ʈ
			int chr = *lpszCheck;

			if (chr >= 0x30 && chr <= 0x39)/*0 to 9*/
				continue;
			else
				return false;
		}
		else
		{
			// �� ����Ʈ
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
			// �� ����Ʈ
			//if ( *lpszCheck < 0x21 || *lpszCheck > 0x7E)
			if ( *lpszCheck < 48 || ( 58 <= *lpszCheck && *lpszCheck < 65) ||
					( 91 <= *lpszCheck && *lpszCheck < 97) || *lpszCheck > 122)
			{
				return ( true);
			}
		}
		else
		{
			// �� ����Ʈ
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( 0x81 <= *lpszCheck && *lpszCheck <= 0xC8)
			{
				// �ѱ�
				if ( ( 0x41 <= *lpszTrail && *lpszTrail <= 0x5A) ||
						( 0x61 <= *lpszTrail && *lpszTrail <= 0x7A) ||
						( 0x81 <= *lpszTrail && *lpszTrail <= 0xFE))
				{
					// ������� ���κ�
					// �ȵǴ� Ư�����ڿ�����
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
			// �� ����Ʈ
			if ( *lpszCheck < 0x21 || ( *lpszCheck > 0x7E && *lpszCheck < 0xA1) ||
					*lpszCheck > 0xDF)
			{
				return ( true);
			}
		}
		else
		{
			// �� ����Ʈ
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( ( 0x88 <= *lpszCheck && *lpszCheck <= 0x9F) ||
					( 0xE0 <= *lpszCheck && *lpszCheck <= 0xEA) ||
					( 0xED <= *lpszCheck && *lpszCheck <= 0xEE) ||
					( 0xFA <= *lpszCheck && *lpszCheck <= 0xFC))
			{
				// ����
				if ( 0x40 <= *lpszTrail && *lpszTrail <= 0xFC)
				{
					// ���Ǵ� ����
					if ( *lpszTrail == 0x7F)
					{
						// ����
						return ( true);
					}
					if ( *lpszCheck == 0x88 && *lpszTrail <= 0x9E)
					{
						// ����
						return ( true);
					}
					if ( *lpszCheck == 0x98 && ( 0x73 <= *lpszTrail && *lpszTrail <= 0x9E))
					{
						// ����
						return ( true);
					}
					if ( *lpszCheck == 0xEA && 0xA5 <= *lpszTrail)
					{
						// ����
						return ( true);
					}
					if ( *lpszCheck == 0xEE && 0xED <= *lpszTrail)
					{
						// Ư�� ���� �� ����
						return ( true);
					}
					if ( *lpszCheck == 0xFC && 0x4C <= *lpszTrail)
					{
						// ����
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
					// �Ϻ� ����/���󰡳��� ������ ����
					return ( true);
				}
			}
			else if ( *lpszCheck == 0x83)
			{
				if ( *lpszTrail < 0x40 || 0x97 <= *lpszTrail)
				{
					// ���󰡳� ���� ���� �� Ư������
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
