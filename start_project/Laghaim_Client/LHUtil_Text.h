#ifndef _LHUtil_Text_H_
#define _LHUtil_Text_H_
#pragma once

#include <string>
#include <iostream>
#include <locale>
#include <algorithm>

class LHUtil_Text
{
public:
	static int UTF8ToUnicode(wchar_t* wcstr, const char * utfstr, size_t count)
	{
		if( count == 0 && wcstr != NULL )
			return 0;

		int result = MultiByteToWideChar(CP_UTF8 , 0 , utfstr , -1 , wcstr , (int)count);
		if( result > 0 )
			wcstr[result-1] = 0;

		return result;
	}

	static int UniToMulti(char * mbstr, const wchar_t*wcstr , size_t count)
	{
		if( count == 0 && mbstr != NULL )
			return 0;

		int result = ::WideCharToMultiByte( CP_ACP , 0 , wcstr , -1 , mbstr , (int)count , NULL, NULL );
		if( result > 0 )
			mbstr[result-1] = 0;
		return result;
	}

	static bool IsDigit(std::string intString)
	{
		bool isDigit = std::all_of( intString.begin(), intString.end(),
									[](const std::string::value_type& c) -> bool
		{
			return std::isdigit(c, std::locale());
		});

		return isDigit;
	}
};
#endif // _LHUtil_Text_H_