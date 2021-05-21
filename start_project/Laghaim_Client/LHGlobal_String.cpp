#include "stdafx.h"
#include "LHGlobal_String.h"
#include "Country.h"
#include "LHExcelReader.h"

const char * MSG_TABLE_FILE = "data/language/msg_table.txt";
const char * ITEM_TABLE_FILE = "data/language/item_table.txt";
const char * NPC_TABLE_FILE = "data/language/npc_table.txt";
const char * SYSMSG_TABLE_FILE = "data/sysmsg_table.txt";

extern DWORD g_dwClientCountry;

LHGlobal_String theGlobalString;

//#define _TEST_GS_

int UTF8ToUnicode(wchar_t* wcstr, const char * utfstr, size_t count)
{
	if( count == 0 && wcstr != NULL )
		return 0;

	int result = MultiByteToWideChar(CP_UTF8 , 0 , utfstr , -1 , wcstr , (int)count);
	if( result > 0 )
		wcstr[result-1] = 0;

	return result;
	return 0;
}

int UniToMulti(char * mbstr, const wchar_t*wcstr , size_t count)
{
	if( count == 0 && mbstr != NULL )
		return 0;

	int result = ::WideCharToMultiByte( CP_ACP , 0 , wcstr , -1 , mbstr , (int)count , NULL, NULL );
	if( result > 0 )
		mbstr[result-1] = 0;
	return result;
}


LHGlobal_String::LHGlobal_String(void)
	:	m_msg_string(new LHSTRMAP)
	,	m_item_string(new LHSTRMAP)
	,	m_npc_string(new LHSTRMAP)
	,	m_current_language(LH_LANG_TYPE__KOR)
{
	if(PRIMARYLANGID(GetSystemDefaultLangID())==LANG_KOREAN)
	{
		m_current_language = LH_LANG_TYPE__KOR;
	}
	else if(PRIMARYLANGID(GetSystemDefaultLangID())==LANG_ENGLISH)
	{
		m_current_language = LH_LANG_TYPE__ENG;
	}
	else if(PRIMARYLANGID(GetSystemDefaultLangID())==LANG_JAPANESE)
	{
		m_current_language = LH_LANG_TYPE__JPN;
	}
	else if(PRIMARYLANGID(GetSystemDefaultLangID())==LANG_THAI)
	{
		m_current_language = LH_LANG_TYPE__THAI;
	}
	else
	{
		m_current_language = LH_LANG_TYPE__ENG;
	}



}

LHGlobal_String::~LHGlobal_String(void)
{
	m_msg_string->clear();
	delete m_msg_string;
	m_msg_string = NULL;


	m_item_string->clear();
	delete m_item_string;
	m_item_string = NULL;

	m_npc_string->clear();
	delete m_npc_string;
	m_npc_string = NULL;


}


void LHGlobal_String::Open_All()
{
	Open_MsgTable();
	Open_ItemTable();
	Open_NpcTable();
}

void LHGlobal_String::Open_MsgTable()
{
	LHExcelReader er;
	if( !er.Open( MSG_TABLE_FILE ) )
	{
		OutputDebugString("File Not Found!!![MSG TABLE]\n");
	}

	char multibyte[1024];
	wchar_t unicode[1024];

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		int col = 0;
		int index = er.GetInt(row,col++);
		int lcol = 0;

		switch(g_dwClientCountry)
		{
		case CTRY_KOR:
			lcol = 1;
			break;
		case CTRY_JPN:
			lcol = 2;
			break;
		case CTRY_ENG:
			lcol = 3;
			break;
		case CTRY_TH:
			lcol = 4;
			break;
		}

		const char * text = er.GetData(row, lcol);

		multibyte[0] = 0;
		unicode[0] = 0;

		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );

		m_msg_string->insert(LHSTRMAP_vt(index,multibyte));
	}
}

void LHGlobal_String::Open_ItemTable()
{
	LHExcelReader er;
	if( !er.Open( ITEM_TABLE_FILE ) )
	{
		OutputDebugString("File Not Found!!![ITEM TABLE]\n");
	}

	char multibyte[1024];
	wchar_t unicode[1024];

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		int col = 0;
		int index = er.GetInt(row,col++);
		int lcol = 0;

		switch(g_dwClientCountry)
		{
		case CTRY_KOR:
			lcol = 2;
			break;
		case CTRY_JPN:
			lcol = 3;
			break;
		case CTRY_ENG:
			lcol = 4;
			break;
		case CTRY_TH:
			lcol = 5;
			break;
		}

		const char * text = er.GetData(row, lcol);

		multibyte[0] = 0;
		unicode[0] = 0;

		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );

		m_item_string->insert(LHSTRMAP_vt(index,multibyte));
	}
}

void LHGlobal_String::Open_NpcTable()
{
	LHExcelReader er;
	if( !er.Open( NPC_TABLE_FILE ) )
	{
		OutputDebugString("File Not Found!!![NPC TABLE]\n");
	}

	char multibyte[1024];
	wchar_t unicode[1024];

	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		int col = 0;
		int index = er.GetInt(row,col++);
		int lcol = 0;

		switch(g_dwClientCountry)
		{
		case CTRY_KOR:
			lcol = 1;
			break;
		case CTRY_JPN:
			lcol = 2;
			break;
		case CTRY_ENG:
			lcol = 3;
			break;
		case CTRY_TH:
			lcol = 4;
			break;
		}

		const char * text = er.GetData(row, lcol);

		multibyte[0] = 0;
		unicode[0] = 0;

		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );

		m_npc_string->insert(LHSTRMAP_vt(index,multibyte));
	}
}


const char * LHGlobal_String::GetMsg(int index)
{
	if( m_msg_string->empty() )
		Open_MsgTable();

	LHSTRMAP_it it = m_msg_string->find(index);

	if( it!=m_msg_string->end() )
		return (*it).second.c_str();

	return "Not Found!!";
}

const char * LHGlobal_String::GetItemName(int index)
{
	if( m_item_string->empty() )
		Open_MsgTable();

	LHSTRMAP_it it = m_item_string->find(index);

	if( it!=m_item_string->end() )
		return (*it).second.c_str();

	return "Not Found!!";
}

const char * LHGlobal_String::GetNpcName(int index)
{
	if( m_npc_string->empty() )
		Open_MsgTable();

	LHSTRMAP_it it = m_npc_string->find(index);

	if( it!=m_npc_string->end() )
		return (*it).second.c_str();

	return "Not Found!!";
}


