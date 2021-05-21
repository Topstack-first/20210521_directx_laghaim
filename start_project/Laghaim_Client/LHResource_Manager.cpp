#include "stdafx.h"
#include "LHResource_Manager.h"
#include <windows.h>

#include "Def.h"

#pragma comment(lib, "tinyxml.lib" );

#include "tinyxml.h"
#include "tinystr.h"

#include "config.h"


// std::wstring utf8str_to_wstring(const std::string& utf8str, int len)
// {
// 	std::wstring wout;
//
// 	int i = 0;
// 	while (i < len)
// 	{
// 		wchar_t unichar;
// 		unsigned char cur_char = utf8str[i];
//
// 		if (cur_char < 0x80)
// 		{
// 			// Ascii character, just add it
// 			unichar = cur_char;
// 		}
// 		else
// 		{
// 			int cont_bytes = 0;
// 			if ((cur_char >> 5) == 0x6)			// Two byte UTF8 -> 1 UTF32
// 			{
// 				unichar = (0x1F&cur_char);
// 				cont_bytes = 1;
// 			}
// 			else if ((cur_char >> 4) == 0xe)	// Three byte UTF8 -> 1 UTF32
// 			{
// 				unichar = (0x0F&cur_char);
// 				cont_bytes = 2;
// 			}
// 			else if ((cur_char >> 3) == 0x1e)	// Four byte UTF8 -> 1 UTF32
// 			{
// 				unichar = (0x07&cur_char);
// 				cont_bytes = 3;
// 			}
// 			else if ((cur_char >> 2) == 0x3e)	// Five byte UTF8 -> 1 UTF32
// 			{
// 				unichar = (0x03&cur_char);
// 				cont_bytes = 4;
// 			}
// 			else if ((cur_char >> 1) == 0x7e)	// Six byte UTF8 -> 1 UTF32
// 			{
// 				unichar = (0x01&cur_char);
// 				cont_bytes = 5;
// 			}
// 			else
// 			{
// 				wout += '?';
// 				++i;
// 				continue;
// 			}
//
// 			// Check that this character doesn't go past the end of the string
// 			int end = (len < (i + cont_bytes)) ? len : (i + cont_bytes);
// 			do
// 			{
// 				++i;
//
// 				cur_char = utf8str[i];
// 				if ( (cur_char >> 6) == 0x2 )
// 				{
// 					unichar <<= 6;
// 					unichar += (0x3F&cur_char);
// 				}
// 				else
// 				{
// 					// Malformed sequence - roll back to look at this as a new char
// 					unichar = '?';
// 					--i;
// 					break;
// 				}
// 			} while(i < end);
//
// 			// Handle overlong characters and NULL characters
// 			if ( ((cont_bytes == 1) && (unichar < 0x80))
// 				|| ((cont_bytes == 2) && (unichar < 0x800))
// 				|| ((cont_bytes == 3) && (unichar < 0x10000))
// 				|| ((cont_bytes == 4) && (unichar < 0x200000))
// 				|| ((cont_bytes == 5) && (unichar < 0x4000000)) )
// 			{
// 				unichar = '?';
// 			}
// 		}
//
// 		wout += unichar;
// 		++i;
// 	}
//
// 	return wout;
// }
//
//
// std::wstring wstring_to_utf16str(const std::wstring &utf32str, int len)
// {
// 	std::wstring out;
//
// 	int i = 0;
// 	while (i < len)
// 	{
// 		int cur_char = utf32str[i];
// 		if (cur_char > 0xFFFF)
// 		{
// 			out += (0xD7C0 + (cur_char >> 10));
// 			out += (0xDC00 | (cur_char & 0x3FF));
// 		}
// 		else
// 		{
// 			out += cur_char;
// 		}
// 		i++;
// 	}
// 	return out;
// }
//
//
//
//
//
// std::string _utf16str_to_ansistr( const std::wstring& utf16str, int len )
// {
// #define BUF_SIZE 4096
// 	char cbuf[BUF_SIZE] = {0,};
// 	WideCharToMultiByte( CP_ACP, 0, utf16str.c_str(), -1, cbuf, len==-1 ? BUF_SIZE : min( len, BUF_SIZE ), 0, 0 );
// 	return cbuf;
// #undef BUF_SIZE
// }
//
// std::string _utf16str_to_ansistr( const std::wstring& utf16str )
// {
// 	return _utf16str_to_ansistr( utf16str, -1 );
// }
//
//
// std::wstring utf8str_to_wstring(const std::string& utf8str)
// {
// 	const int len = (int)utf8str.length();
// 	return utf8str_to_wstring(utf8str, len);
// }
//
//
// std::wstring wstring_to_utf16str(const std::wstring &utf32str)
// {
// 	const int len = (int)utf32str.length();
// 	return wstring_to_utf16str(utf32str, len);
// }
//
// std::wstring _utf8str_to_utf16str( const std::string& utf8str )
// {
// 	std::wstring wstr = utf8str_to_wstring( utf8str );
// 	return wstring_to_utf16str( wstr );
// }
//
//
// std::string _utf8str_to_ansistr( const std::string& utf8str)
// {
// 	return _utf16str_to_ansistr( _utf8str_to_utf16str( utf8str) );
// }

LHResource_Manager::LHResource_Manager(void)
{
	Init();
}

LHResource_Manager::~LHResource_Manager(void)
{
	Release();
}




int LHResource_Manager::UTF8ToUnicode(wchar_t* wcstr, const char * utfstr, size_t count)
{
	if( count == 0 && wcstr != NULL )
		return 0;

	int result = MultiByteToWideChar(CP_UTF8 , 0 , utfstr , -1 , wcstr , (int)count);
	if( result > 0 )
		wcstr[result-1] = 0;

	return result;
	return 0;
}

int LHResource_Manager::UniToMulti(char * mbstr, const wchar_t*wcstr , size_t count)
{
	if( count == 0 && mbstr != NULL )
		return 0;

	int result = ::WideCharToMultiByte( CP_ACP , 0 , wcstr , -1 , mbstr , (int)count , NULL, NULL );
	if( result > 0 )
		mbstr[result-1] = 0;
	return result;
}

void LHResource_Manager::Init()
{

// 	ff = pfopen("./data/jpn.xml" , "rb");
// #if defined(_JPN_CLIENT_) && defined(_DLL_TO_XML_)
// 	TiXmlDocument doc("./data/jpn.xml");
// #endif //
//
// #if defined(_KOR_CLIENT_) && defined(_DLL_TO_XML_)
// 	TiXmlDocument doc("./data/kor.xml");
// #endif //
//
// #if defined(_GSP_CLIENT_) && defined(_DLL_TO_XML_)
// 	TiXmlDocument doc("./data/eng.xml");
// #endif //

	TiXmlDocument doc;

#if defined(_DLL_TO_XML_)
// 	if( !doc.LoadFile(ff) )
// 		return;
//
// 	TiXmlHandle hDoc(&doc);
// 	TiXmlElement * pElem;
// 	TiXmlHandle hRoot(0);
//
//
// 	pElem=hDoc.FirstChildElement().Element();
// 	if (!pElem)
// 		return;
// 	const char * name =pElem->Value();
//
// 	hRoot=TiXmlHandle(pElem);
//
// 	pElem = hRoot.FirstChild( "Messages" ).FirstChild().Element();
//
//
// 	LHSTRING_MAP	string_map;
// 	for( pElem ; pElem ; pElem = pElem->NextSiblingElement() )
// 	{
// 		int idx;
// 		pElem->QueryIntAttribute( "id" , &idx );
// 		const char *pText=pElem->GetText();
//
// 		wchar_t uni[512] = {0,};
// 		char multi[512] = {0,};
//
//  		UTF8ToUnicode( uni , pText , 512 );
//  		UniToMulti(multi , uni, 512);
// //		std::string text = _utf8str_to_ansistr(pText);
//
// 		if( idx && pText )
// 			m_string_map.insert(std::map<int, std::string>::value_type(idx,multi) );
// 	}
#endif // defined(_DLL_TO_XML_)
}

void LHResource_Manager::Release()
{
	m_string_map.clear();
}

const char * LHResource_Manager::GetString(int index)
{
	if( m_string_map.empty() )
		return "Empty";

	LHSTRING_MAP_it it = m_string_map.find(index);

	if( it != m_string_map.end() )
		return (*it).second.c_str();

// 	char err[64] = {0, };
// 	sprintf( err , "Not Found : %04d" , index );
// 	return err;

	return "error";
}


const char * LHResource_Manager::GetItemName(int index)
{
// 	char err[64] = {0, };
// 	sprintf( err , "Not Found : %04d" , index );
// 	return err;
	return "error";
}

const char * LHResource_Manager::GetNpcName(int index)
{
// 	char err[64] = {0, };
// 	sprintf( err , "Not Found : %04d" , index );
// 	return err;
	return "error";
}

