#include "stdafx.h"
//=============================================================================
//	Name : LHExcelReader
//=============================================================================
#include "LHExcelReader.h"

const char * SKIP_LINE = "end of skip";
const int MAX_LINE_LENGTH = 4096;

LHExcelReader::LHExcelReader(void)
	:	m_opened(false)
{
}

LHExcelReader::~LHExcelReader(void)
{
	m_opened = false;
	Release();
}

void LHExcelReader::Initial()
{
	m_opened = false;
	Release();
}

void LHExcelReader::Release()
{
	m_opened = false;
	m_title.clear();
	m_sheet.clear();
}

bool LHExcelReader::Open(const TCHAR * filename, LHFileMode mode)
{
	LHFile f;
	if( !f.Open( filename , LHFileMode__ReadText ) )
		return false;

	_ReadTitle(f);
	_SkipLine(f);
	_ReadData(f);

	m_opened = true;


	int col_count = m_title.size();
	int row_count = m_sheet.size();

	return true;
}

void LHExcelReader::Close()
{
}

void LHExcelReader::_ReadTitle(LHFile& f)
{
	char line_buffer[MAX_LINE_LENGTH];
	int line_length = 0;

	char c;
	char buffer[MAX_LINE_LENGTH];
	int buffer_size = 0;

	f.gets( line_buffer , MAX_LINE_LENGTH );

	line_length = (int)strlen( line_buffer );

	for( int i=0 ; i<line_length ; i++ )
	{
		c = line_buffer[i];

		if( c=='\t' || c=='\r' || c=='\n' )
		{
			buffer[buffer_size] = 0;
			buffer_size = 0;
			m_title.push_back( buffer );
		}
		else
		{
			buffer[buffer_size++] = c;
		}
	}
}

void LHExcelReader::_SkipLine(LHFile& f)
{
	char line_buffer[MAX_LINE_LENGTH];
	int line_length = 0;

	char c;
	char buffer[MAX_LINE_LENGTH];
	int buffer_size = 0;
	bool skipline = false;

	while(true)
	{
		if( f.gets( line_buffer , MAX_LINE_LENGTH ) == NULL )
			break;

		line_length = (int)strlen( line_buffer );

		for( int i=0 ; i<line_length ; i++ )
		{
			c = line_buffer[i];
			if( c=='\t' || c=='\r' || c=='\n' )
			{
				buffer[buffer_size] = 0;
				buffer_size = 0;

				if( !stricmp( buffer , SKIP_LINE ) )
					return;
			}
			else
			{
				buffer[buffer_size++] = c;
			}
		}
	}
}

void LHExcelReader::_ReadData(LHFile& f)
{
	char line_buffer[MAX_LINE_LENGTH];
	int line_length = 0;

	char c;
	char buffer[MAX_LINE_LENGTH];
	int buffer_size = 0;

	rowVec row;

	while(true)
	{
		if( f.gets( line_buffer , MAX_LINE_LENGTH ) == NULL )
			break;

		line_length = (int)strlen( line_buffer );

		row.clear();
		row.reserve( m_title.size() );

		for( int i=0 ; i<line_length ; i++ )
		{
			c = line_buffer[i];

			if( c=='\t' || c=='\r' || c=='\n' )
			{
				buffer[buffer_size] = 0;
				buffer_size = 0;
				row.push_back(buffer);
			}
			else
			{
				buffer[buffer_size++] = c;
			}
		}

		m_sheet.push_back( row );
	}
}

const char * LHExcelReader::GetData(int row, int col)
{
	if( row<0 )
		return NULL;
	if( row>=GetRowSize() )
		return NULL;

	if( col<0 )
		return NULL;
	if( col>=GetColSize() )
		return NULL;

	return m_sheet[row][col].c_str();
}

bool LHExcelReader::GetBool(int row, int col)
{
	int b = GetInt( row, col );
	if( b )
		return true;
	return false;
}

int LHExcelReader::GetInt(int row, int col)
{
	const char * data = GetData(row, col);
	if( !data )
		return -1;
	if( strcmp( data, "" ) == 0 )
		return -1;

	return atoi(data);
}

float LHExcelReader::GetFloat(int row, int col)
{
	const char * data = GetData(row, col);
	if( !data )
		return 0.0f;
	if( strcmp( data, "" ) == 0 )
		return 0.0f;

	return (float)atof(data);
}

DWORD LHExcelReader::GetHex(int row, int col)
{
	std::string data = GetData(row, col);
	std::stringstream str;
	str << data;
	DWORD dwHex;
	str >> std::hex >> dwHex;

	return dwHex;
}
