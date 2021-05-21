#include "stdafx.h"
#define D3D_OVERLOADS
#include <limits.h>
#include <stdlib.h>
#include <tchar.h>
#include <malloc.h>
#include <memory.h>
#include <io.h>
#include <atlstr.h>

#include "nfs.h"
#include "NfsVirtualFile.h"
#include "VirtualFile.h"

#if defined(_MSC_VER)
#if _MSC_VER <= 1400
extern "C"
int __cdecl _input(FILE *, const unsigned char *, va_list);
#else
extern "C"
int __cdecl _input_l(FILE*, unsigned char*, _locale_t, va_list);
#endif
#endif

CVirtualFile::CVirtualFile()
{
	m_pVF_Base = NULL;
	m_pVF_Ex = NULL;
	m_pVF_Patch = NULL;
	m_pNfsGlob_Base = NULL;
	m_pNfsGlob_Ex	= NULL;
	m_pNfsGlob_Patch = NULL;
	m_bIsBaseOpen	= false;
	m_bIsExpansionOpen = false;
	m_bIsPatchOpen = false;

	Init();
}

CVirtualFile::~CVirtualFile()
{
	SAFE_DELETE( m_pVF_Base );
	SAFE_DELETE( m_pVF_Ex );
	SAFE_DELETE( m_pVF_Patch );
}

BOOL CVirtualFile::Init()
{
	if( m_pVF_Base == NULL )
		m_pVF_Base = new NfsVirtualFile();

	if( m_pVF_Ex == NULL )
		m_pVF_Ex = new NfsVirtualFile();

	if( m_pVF_Patch == NULL )
		m_pVF_Patch = new NfsVirtualFile();

	if( m_pVF_Base == NULL )
		return FALSE;

	if( m_pVF_Ex == NULL )
		return FALSE;

	if( m_pVF_Patch == NULL )
		return FALSE;

	return TRUE;
}

BOOL CVirtualFile::Open(char *fileName)
{
	// nfs file open.
	CString szBaseFile, szPatchFile, szExpansionFile;

	szBaseFile = fileName;
	szPatchFile = fileName;
	szExpansionFile = fileName;

	szExpansionFile += "_ex";
	szPatchFile += "_patch";

	if( m_pVF_Base->fileopen( szBaseFile, false ) )
	{
		m_bIsBaseOpen = true;
		if( m_pVF_Ex->fileopen( szExpansionFile, false ) )
		{
			m_bIsExpansionOpen = true;
		}
		if( m_pVF_Patch->fileopen( szPatchFile, false ) )
		{
			m_bIsPatchOpen = true;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CVirtualFile::SearchFile( const char* fileName )
{
	m_pVF_Patch->globFileclose();
	m_pVF_Ex->globFileclose();
	m_pVF_Base->globFileclose();
	
	// search in patch file.
	m_pVF_Patch->globFile( fileName );
	if( m_pVF_Patch->getNfsFile_glob_t()->gl_pathc )
		return m_pVF_Patch->getNfsFile_glob_t()->gl_pathc;

	// search in ex file.
	m_pVF_Ex->globFile( fileName );
	if( m_pVF_Ex->getNfsFile_glob_t()->gl_pathc )
		return m_pVF_Ex->getNfsFile_glob_t()->gl_pathc;

	// search in base file.
	m_pVF_Base->globFile( fileName );

	return m_pVF_Base->getNfsFile_glob_t()->gl_pathc;
}

int	CVirtualFile::HowManyMatchFile( const char* desc, FILETYPE& _type )
{
	m_pVF_Patch->globFileclose();
	m_pVF_Ex->globFileclose();
	m_pVF_Base->globFileclose();

	m_pNfsGlob_Patch = NULL;
	m_pNfsGlob_Base = NULL;
	m_pNfsGlob_Ex	= NULL;

	char szName[MAX_PATH] = {0,};
	char temp[MAX_PATH] = {0,};

	strcpy( szName, changeDeleminate( const_cast<char *>(desc), temp ) );
	strupr( szName );
	
	if( m_bIsPatchOpen )
	{
		m_pVF_Patch->globFile( szName );
		if( m_pVF_Patch->getNfsFile_glob_t()->gl_pathc )
		{
			m_pNfsGlob_Patch = m_pVF_Patch->getNfsFile_glob_t();
			_type = PATCH;
			return m_pNfsGlob_Patch->gl_pathc;
		}
	}
	
	if( m_bIsExpansionOpen )
	{
		m_pVF_Ex->globFile( szName );
		if( m_pVF_Ex->getNfsFile_glob_t()->gl_pathc )
		{
			m_pNfsGlob_Ex = m_pVF_Ex->getNfsFile_glob_t();
			_type = EX;
			return m_pNfsGlob_Ex->gl_pathc;
		}
	}
	
	m_pVF_Base->globFile( szName );

	m_pNfsGlob_Base = m_pVF_Base->getNfsFile_glob_t();

	_type = BASE;

	return m_pNfsGlob_Base->gl_pathc;
}

BOOL CVirtualFile::OpenFile( const char *fileName)
{
	FILETYPE _type;
	
	int filecount = HowManyMatchFile( fileName, _type );

	if( filecount > 0 )
	{
		// what is opened?
		if( _type == PATCH )
		{
			m_iCurPos = m_pNfsGlob_Patch->gl_offs;
		}
		else if( _type == EX )
		{
			m_iCurPos = m_pNfsGlob_Ex->gl_offs;
		}
		else if( _type == BASE )
		{
			m_iCurPos = m_pNfsGlob_Base->gl_offs;
		}
	}

	return filecount;
}

void CVirtualFile::CloseFile()
{
	m_pVF_Patch->globFileclose();
	m_pVF_Base->globFileclose();
	m_pVF_Ex->globFileclose();
}

void CVirtualFile::Read( char** buf, long& size )
{
	*buf = NULL;
	size = 0;

	// if all file is not opened, It's Error!!!
	if( !m_bIsBaseOpen && !m_bIsExpansionOpen && !m_bIsPatchOpen )
		return;

	if( !m_pNfsGlob_Base && !m_pNfsGlob_Ex && !m_pNfsGlob_Patch )
		return;

	if( m_pNfsGlob_Patch )
	{
		m_pVF_Patch->Nfsfileopen( m_pNfsGlob_Patch->gl_pathv[m_iCurPos++] );
		size = m_pVF_Patch->getNfs_file_read();
		*buf = new char[size];
		ZeroMemory( *buf, size );
		memcpy( *buf, m_pVF_Patch->GetNfsBuffer(), size );
		m_pVF_Patch->Nfsfileclose();
	}
	else if( m_pNfsGlob_Ex )
	{
		m_pVF_Ex->Nfsfileopen( m_pNfsGlob_Ex->gl_pathv[m_iCurPos++] );
		size = m_pVF_Ex->getNfs_file_read();
		*buf = new char[size];
		ZeroMemory( *buf, size );
		memcpy( *buf, m_pVF_Ex->GetNfsBuffer(), size );
		m_pVF_Ex->Nfsfileclose();
	}
	else if( m_pNfsGlob_Base )
	{
		m_pVF_Base->Nfsfileopen( m_pNfsGlob_Base->gl_pathv[m_iCurPos++] );
		size = m_pVF_Base->getNfs_file_read();
		*buf = new char[size];
		ZeroMemory( *buf, size );
		memcpy( *buf, m_pVF_Base->GetNfsBuffer(), size );
		m_pVF_Base->Nfsfileclose();
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

char* changeDeleminate( char* szSrc, char* szDst )
{
	char* tempPtr = szSrc;
	char szTemp[128] = {0,};
	char* destPtr = szTemp;

	// ./ 또는 .// 또는 ../ 또는 ..// 를 없애기 위해서.
	// 최초 시작에는 필요가 없다.
	for( int i = 0; i < 4; i++ )
		if( *tempPtr == '.' || *tempPtr == '/' || *tempPtr == '\\' )
			tempPtr++;

	while( *tempPtr )
	{
		if( *tempPtr == '/' || *tempPtr == '\\' )
		{
			if( *(destPtr-1) == '/' )
			{
				tempPtr++;
				continue;
			}
			else
			{
				*destPtr++ = '/';
				*tempPtr++;
			}
		}
		else
			*destPtr++ = *tempPtr++;
	}

	szDst = szTemp;

	return szDst;
}

int	vPackInitialize()
{
	return (CVirtualFile::GetInstance())->Open( "Data" );
}

void vPackUninitialize()
{
	/* ocarina */
	(CVirtualFile::GetInstance())->CloseFile();
	(CVirtualFile::GetInstance())->m_pVF_Patch->fileclose();
	(CVirtualFile::GetInstance())->m_pVF_Base->fileclose();
	(CVirtualFile::GetInstance())->m_pVF_Ex->fileclose();
}

VFILE* vfopen( const char* filename, const char* mode )
{
	CVirtualFile* pVF = CVirtualFile::GetInstance();
	char szFileName[MAX_PATH]= {0,};

	strcpy( szFileName, filename );
	strupr( szFileName );

	__try
	{
		printf("calling File Name : %s\n", filename );
		fflush( stdout );
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		system( "pause" );
		int testbreak = 0;
	};

	if( !pVF->OpenFile( szFileName ) )
		return NULL;
	
	VFILE* temp = new VFILE();
	ZeroMemory( temp, sizeof( VFILE ) );
	
	pVF->Read( &temp->_base, temp->_basesize );
	pVF->CloseFile();

	temp->_ptr = temp->_base;
	temp->_cnt = 0;

	return temp;
}

void vfclose( VFILE* fp )
{
	SAFE_DELETE_ARRAY( fp->_base );
	SAFE_DELETE( fp );
}

size_t vfread( void* ptr, size_t size, size_t n, VFILE* stream  )
{
	char* pTemp = (char*)ptr;
	size_t cnt = 0;
	while( stream->_base + stream->_basesize > stream->_ptr && cnt < n )
	{
		if( stream->_ptr + size > stream->_base + stream->_basesize )
			size = ( stream->_base + stream->_basesize ) - stream->_ptr;
		memcpy( pTemp, stream->_ptr, size );
		cnt ++;
		stream->_ptr += size;
		pTemp += size;
	}

	return cnt;
}

char* vfgets( char* string, int count, VFILE* str )
{
	VFILE * stream;
	char* pointer = string;
	char* retval = string;
	int ch;

	if( count == 0 )
		return NULL;

	/* The C Standard states the input buffer should remain
	unchanged if EOF is encountered immediately. Hence we
	do not blank out the input buffer here */

	/* init stream pointer */
	stream = str;

	int size = stream->_basesize;
	__try
	{
		if( retval != NULL )
		{
			while( --count )
			{
				long ptrSize = stream->_ptr - stream->_base;
				// EOF check.
				if( stream->_basesize <= ptrSize || (ch = *stream->_ptr) == EOF )
				{
					if( pointer == string )
					{
						retval = NULL;
						goto done;
					}
					// 끝을 만나면 종료.
					break;
				}
				if( (*pointer++ = *stream->_ptr++) == '\n' )
				{
					if( *(pointer-2) == '\r' )
						--pointer;
					break;
				}
			}
			*pointer = '\0';
		}
done:
		;
	}
	__finally
	{
		;
	}

	return retval;
}


int __cdecl vfscanf( VFILE* stream, const char *format, ... )
{
	va_list arglist;
	va_start( arglist, format );

	FILE str;
	FILE* infile = &str;
	int retval;

	int curPos = vftell(stream);
	size_t count = stream->_basesize - curPos;

	_ASSERTE(stream->_ptr != NULL);
	_ASSERTE(format != NULL);


	infile->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
	infile->_ptr = infile->_base = stream->_ptr;

	if( count > ( INT_MAX / sizeof( _TCHAR ) ) )
	{
		infile->_cnt = INT_MAX;
	}
	else
		infile->_cnt = (int)count*sizeof(_TCHAR);

	// VS 80부터는 _input_l이다. 아규먼트도 4개.
	// 게다가 unicode지원시에는 _tinput_l로 해야함.
#if defined(_MSC_VER)
#if _MSC_VER <= 1400
	retval = (_input(infile, reinterpret_cast<unsigned char*>(const_cast<char *>(format) ), arglist ) );
#else
	retval = (_input_l(infile, reinterpret_cast<unsigned char*>(const_cast<char *>(format) ), NULL, arglist ) );
#endif
#endif

	va_end( arglist );

	stream->_ptr = infile->_ptr;

	return retval;
}

int vfscanf( VFILE* stream, const char* format, va_list arglist )
{
	FILE str;
	FILE* infile = &str;
	int retval;

	int curPos = vftell(stream);
	size_t count = stream->_basesize - curPos;

	_ASSERTE(stream->_ptr != NULL);
	_ASSERTE(format != NULL);


	infile->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
	infile->_ptr = infile->_base = stream->_ptr;

	if( count > ( INT_MAX / sizeof( _TCHAR ) ) )
	{
		infile->_cnt = INT_MAX;
	}
	else
		infile->_cnt = (int)count*sizeof(_TCHAR);

	// VS 80부터는 _input_l이다. 아규먼트도 4개.
	// 게다가 unicode지원시에는 _tinput_l로 해야함.
#if defined(_MSC_VER)
#if _MSC_VER <= 1400
	retval = (_input(infile, reinterpret_cast<unsigned char*>(const_cast<char *>(format) ), arglist ) );
#else
	retval = (_input_l(infile, reinterpret_cast<unsigned char*>(const_cast<char *>(format) ), NULL, arglist ) );
#endif
#endif


	stream->_ptr = infile->_ptr;

	return retval;
}

int vfseek( VFILE *stream, long int offset, int whence )
{
	if( !stream )
		return -1;

	switch( whence )
	{
	case SEEK_SET:
		stream->_ptr = stream->_base;
		stream->_ptr += offset;
		break;
	case SEEK_CUR:
		if( stream->_ptr + offset > stream->_base + stream->_basesize )
			return -1;
		if( stream->_ptr + offset < stream->_base )
			return -1;
		stream->_ptr += offset;
		break;
	case SEEK_END:
		if( offset > 0 )
			return -1;
		stream->_ptr += stream->_basesize;
		if( stream->_basesize < offset )
			stream->_ptr = stream->_base;
		else
			stream->_ptr -= offset;

		break;
	}
	return 0;
}

bool vfexist( char* szFileName )
{
	CVirtualFile* pVF = CVirtualFile::GetInstance();
	if( !pVF->OpenFile(szFileName ) )
		return false;
	pVF->CloseFile();
	return true;
}

int	vftell( VFILE* stream )
{
	long len = (long)(stream->_ptr - stream->_base);

	return len;
}
