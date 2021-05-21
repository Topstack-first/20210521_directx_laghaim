#include "stdafx.h"
#include "LHFile.h"

LHFile::LHFile(void)
	:	m_fp(0)
	,	m_filesize(0)
{
	ZeroMemory( m_filename , sizeof(m_filename) );
}

LHFile::~LHFile(void)
{
	Close();
}

bool LHFile::Open(const TCHAR * filename, LHFileMode mode)
{
	Close();

	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(filename, "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();
	// m_fp = _tfopen( filename , GetModeText(mode) );
	m_fp = pfopen(filename , GetModeText(mode) );

	if( !m_fp )
		return false;
	

	pfseek( m_fp , 0 , SEEK_END);
	m_filesize = pftell(m_fp);
	pfseek( m_fp , 0 , SEEK_SET );

	stream = new unsigned char[m_filesize];

	pfread(stream, m_filesize, 1, m_fp);
	if(decode)
		pDecode(stream, m_filesize);

	pfseek( m_fp , 0 , SEEK_SET );

	return true;
}

void LHFile::Close()
{
	FCLOSE(m_fp);

	m_filesize = 0;
	ZeroMemory( m_filename , sizeof(m_filename) );
}

const TCHAR * LHFile::GetModeText(LHFileMode mode)
{
	TCHAR * rv;
	switch( mode )
	{
	case LHFileMode__ReadText:
		rv = _TEXT("rt");
		break;
	case LHFileMode__ReadBin:
		rv = _TEXT("rb");
		break;
	case LHFileMode__WriteText:
		rv = _TEXT("wt");
		break;
	case LHFileMode__WriteBin:
		rv = _TEXT("rb");
		break;
	case LHFileMode__AppendText:
		rv = _TEXT("at");
		break;
	case LHFileMode__AppendBin:
		rv = _TEXT("ab");
		break;
	default:
		return _TEXT("rt");
	}

	return rv;
}


size_t LHFile::Read(void* data, int datalen)
{
	return pfread( data, 1 , datalen , m_fp );
}

void LHFile::ReadText(TCHAR* text)
{
	int len = 0;
	Read(&len);
	Read(text , len+1);
}

TCHAR* LHFile::gets(TCHAR* buf, int buf_size)
{
	return pfgets(buf, buf_size , m_fp);
}

void LHFile::Seek(int offset, int orgin)
{
	pfseek(m_fp , offset , orgin);
}

int LHFile::Tell()
{
	return pftell(m_fp);
}

// int LHFile::Feof()
// {
// 	return feof(m_fp);
// }