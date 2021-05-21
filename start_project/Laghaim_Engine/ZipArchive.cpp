#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <atlcoll.h>
#include <atlstr.h>
#include "zip.h"
#include "unzip.h"
#include ".\ziparchive.h"

bool		any_errors = false;
bool p_abort = false;
void		msg( const TCHAR* s )
{
	//if( s[0] == '*' )	any_errors = true;
	//_tprintf( _T("%s\n"), s );
}


ZipArchive::ZipArchive(void)
{
	m_szFileName = "";
	m_szPassword = "";
	m_Mode = Write;
	m_hZip = 0;
	m_hMemZip = 0;
	m_dwFileSize = 0;
	m_bIsOpen = false;
	m_bIsCreateMem = false;
	m_nFileCountInZip = 0;
	m_mapFileList.RemoveAll();
	m_mapFileList.InitHashTable(30000);
}

ZipArchive::ZipArchive(const TCHAR* szName, AccessMode _mode /* = Write  */)
{
	m_szFileName = szName;
	m_szPassword = "0";
	m_Mode = _mode;
	m_dwFileSize = 0;
	m_hMemZip = 0;
	m_bIsOpen = false;
	m_bIsCreateMem = false;
	m_nFileCountInZip = 0;
	m_mapFileList.RemoveAll();
	m_mapFileList.InitHashTable(30000);
	OpenArchive( szName, 0, _mode );
}

ZipArchive::ZipArchive(const TCHAR* szName, const TCHAR* szPassword, AccessMode _mode /* = Write  */)
{
	m_szFileName = szName;
	m_szPassword = szPassword;
	m_Mode = _mode;
	m_dwFileSize = 0;
	m_hMemZip = 0;
	m_bIsOpen = false;
	m_bIsCreateMem = false;
	m_nFileCountInZip = 0;
	m_mapFileList.RemoveAll();
	m_mapFileList.InitHashTable(30000);
	OpenArchive( szName, szPassword, _mode );
}

ZipArchive::~ZipArchive(void)
{
	Destroy();
}

void			ZipArchive::Destroy()
{
	CloseArchive();
	_closeMemArchive();

	m_mapFileList.RemoveAll();
}

// ----------------------------------------------------------------------------

ZRESULT			ZipArchive::OpenArchive(const TCHAR* szName, const TCHAR* szPassword, AccessMode _mode /* = Write  */)
{
	m_szFileName = szName;
	m_szPassword = szPassword;
	m_Mode = _mode;
	if( _mode == Read )
		return _openArchive( szName, szPassword );
	else
		return _createArchive( szName, szPassword );
}

ZRESULT			ZipArchive::CloseArchive()
{
	if( !m_bIsOpen )
		return ZR_OK;

	ZRESULT zr = CloseZip(m_hZip);

	m_szFileName.Empty();
	m_szPassword.Empty();

	if( zr != ZR_OK )
		msg( _T("* Failed to close file." ) );

	m_mapFileList.RemoveAll();
	m_bIsOpen = false;

	return zr;
}

ZRESULT			ZipArchive::WriteFileToZip(const TCHAR* szDestName, const TCHAR* szSrc )
{
	ZRESULT zr;
	if( !m_bIsOpen )
	{
		msg( _T("* zip file is not open." ) );
		return ZR_FAILED;
	}
	if( m_hZip == 0 )
	{
		msg( _T("* Invalid zip file handle. " ) );
		return ZR_FAILED;
	}
	if( szDestName[0] == NULL || szDestName[0] == '\n' )
	{
		msg( _T("* Invalid dest file name.") );
		return ZR_FAILED;
	}
	if( szSrc[0] == NULL || szSrc[0] == '\n' )
	{
		msg( _T("* Invalid source file name. " ) );
		return ZR_FAILED;
	}

	zr = ZipAdd( m_hZip, szDestName, szSrc );
	if( zr != ZR_OK ) msg( _T("* Failed to add file" ) );

	return zr;
}

ZRESULT			ZipArchive::WriteFileToZip(const TCHAR* szDestName, void* src, unsigned int len )
{
	ZRESULT zr;
	if( !m_bIsOpen )
	{
		msg( _T("* zip file is not open." ) );
		return ZR_FAILED;
	}
	if( m_hZip == 0 )
	{
		msg( _T("* Invalid zip file handle. " ) );
		return ZR_FAILED;
	}
	if( szDestName[0] == NULL || szDestName[0] == '\n' )
	{
		msg( _T("* Invalid dest file name.") );
		return ZR_FAILED;
	}
	if( src == NULL )
	{
		msg( _T("* Invalid source buffer.") );
		return ZR_FAILED;
	}

	zr = ZipAdd( m_hZip, szDestName, src, len );
	if( zr != ZR_OK ) msg( _T("* Failed to add file" ) );

	return zr;
}

ZRESULT			ZipArchive::ReadFileFromZip(const TCHAR* szName, const TCHAR* szDestName )
{
	CString szTemp(szName);
	ZIPENTRY ze;
	int index;
	if( !m_bIsOpen )
	{
		msg( _T("* zip file is not open." ) );
		return ZR_FAILED;
	}
	if( m_hZip == 0 )
	{
		msg( _T("* Invalid zip file handle. " ) );
		return ZR_FAILED;
	}

	szTemp.Replace( "\\", "/" );
	index = SearchFIleFromZip( szTemp );
	if( index == -1 )
	{
		msg( _T("is not contained value." ) );
		return ZR_FAILED;
	}

	GetZipItem( m_hZip, index, &ze );

	UnzipItem( m_hZip, index, szDestName );

	return ZR_OK;
}

int				ZipArchive::ReadFileFromZip(const TCHAR* szName, char* &src, int& len )
{
	long length = 0;

	ReadFileFromZip( szName, src, length );
	len = length;
	return len;
}

long			ZipArchive::ReadFileFromZip(const TCHAR* szName, char* &src, long& len )
{
	CString szTemp( szName );
	ZIPENTRY ze;
	int index;
	len = -1;
	if( !m_bIsOpen )
	{
		msg( _T("* zip file is not open." ) );
		return len;
	}
	if( m_hZip == 0 )
	{
		msg( _T("* Invalid zip file handle. " ) );
		return len;
	}

	szTemp.Replace( "\\", "/" );

	if( ( index = SearchFIleFromZip( szTemp ) ) == -1 )
	{
		msg( _T("is not contained value." ) );
		return len;
	}

	GetZipItem( m_hZip, index, &ze );

	src = new char[ze.unc_size];
	UnzipItem( m_hZip, index, src, ze.unc_size );

	len = ze.unc_size;
	return len;
}

int				ZipArchive::SearchFIleFromZip(const TCHAR* szName )
{

	if( m_mapFileList.IsEmpty() )
	{
		msg( _T("* map container is empty." ) );
		return -1;
	}

	CString temp = szName;

	temp.Replace( "\\", "/" );
	temp.Replace( "//", "/" );
	temp.MakeUpper();
	CAtlMap<CString, ZIPENTRY>::CPair* pPair = NULL;
	pPair = m_mapFileList.Lookup( temp );

	if( pPair == NULL )
	{
		msg( _T("* map is not contain that this key." ) );
		return -1;
	}

	return pPair->m_value.index;
}

ZRESULT			ZipArchive::RemoveFileInZip(const TCHAR* szDFName )
{
	ZRESULT zr;

	zr = _removeFileInZip( szDFName );
	if( zr != ZR_OK )
	{
		msg( _T("Have not delete file in zip" ) );
		return zr;
	}

	zr = _createMemArchive( m_dwFileSize );
	if( zr != ZR_OK ) return zr;

	zr = _reBuildInZip();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _closeArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _memCopyToFIle( m_szFileName );
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _openArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	return zr;
}

ZRESULT			ZipArchive::AppendFileToZip(const TCHAR* zefn/*zip entry file name*/, const TCHAR* szSrc )
{
	DWORD size = 0;
	if( GetFileAttributes( szSrc ) == 0xFFFFFFFF )
		return ZR_NOFILE;

	size = m_dwFileSize;

	HANDLE handle = CreateFile( szSrc, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	if( handle == INVALID_HANDLE_VALUE ) return ZR_NOFILE;
	// get extend file size and add to base file size.
	size += GetFileSize( handle, 0 );
	CloseHandle( handle );

	size *= 2; // just to be on the safe side.

	// var.
	ZRESULT zr;
	char* buf = 0;
	unsigned int bufsize = 0;
	int numItems = 0;
	CAtlMap<CString, ZIPENTRY>::CPair* ppair = NULL;
	// 중복제거.
	_removeKey( zefn );

	if( _createMemArchive(size) != ZR_OK ) return ZR_FAILED;


	POSITION pos = m_mapFileList.GetStartPosition();

	while( pos )
	{
		ppair = m_mapFileList.GetNext( pos );

		if( ((ZIPENTRY)(ppair->m_value)).attr & FILE_ATTRIBUTE_DIRECTORY )
		{
			zr = _writeFolderToMem( ((ZIPENTRY)ppair->m_value).name);
			if( zr != ZR_OK )
			{
				_closeMemArchive();
				return zr;
			}
			continue;
		}

		if( ((ZIPENTRY)ppair->m_value).unc_size > (long)bufsize )
		{
			if( buf != 0 )
				delete [] buf;

			bufsize = ((ZIPENTRY)ppair->m_value).unc_size * 2;
			buf = new char[bufsize];
		}

		zr = UnzipItem( m_hZip, ((ZIPENTRY)ppair->m_value).index, buf, bufsize );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
		zr = _writeFileToMem( ((ZIPENTRY)ppair->m_value).name, buf, ((ZIPENTRY)ppair->m_value).unc_size );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
	}

	delete [] buf;

	zr = _writeFileToMem( zefn, szSrc );
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _closeArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _memCopyToFIle( m_szFileName );
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _openArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	return zr;
}

ZRESULT			ZipArchive::AppendFileToZip(const TCHAR* zefn, void* src, unsigned int len )
{
	DWORD size = 0;
	if( src == NULL )
		return ZR_NOFILE;

	size = m_dwFileSize;

	size += len;

	size *= 2; // just to be on the safe side.

	// var.
	ZRESULT zr;
	char* buf = 0;
	unsigned int bufsize = 0;
	int numItems = 0;
	CAtlMap<CString, ZIPENTRY>::CPair* ppair = NULL;
	// 중복제거.
	_removeKey( zefn );

	if( _createMemArchive(size) != ZR_OK ) return ZR_FAILED;

	POSITION pos = m_mapFileList.GetStartPosition();

	while( pos )
	{
		ppair = m_mapFileList.GetNext( pos );

		if( ((ZIPENTRY)(ppair->m_value)).attr & FILE_ATTRIBUTE_DIRECTORY )
		{
			zr = _writeFolderToMem( ((ZIPENTRY)ppair->m_value).name);
			if( zr != ZR_OK )
			{
				_closeMemArchive();
				return zr;
			}
			continue;
		}

		if( ((ZIPENTRY)ppair->m_value).unc_size > (long)bufsize )
		{
			if( buf != 0 )
				delete [] buf;

			bufsize = ((ZIPENTRY)ppair->m_value).unc_size * 2;
			buf = new char[bufsize];
		}

		zr = UnzipItem( m_hZip, ((ZIPENTRY)ppair->m_value).index, buf, bufsize );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
		zr = _writeFileToMem( ((ZIPENTRY)ppair->m_value).name, buf, ((ZIPENTRY)ppair->m_value).unc_size );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
	}

	delete [] buf;

	zr = _writeFileToMem( zefn, src, len );
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _closeArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _memCopyToFIle( m_szFileName );
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _openArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	return zr;
}

ZRESULT			ZipArchive::_appendAllFileToZip( HZIP& zip )
{
	if( zip == 0 )
	{
		msg(_T("Invalid zip handle." ) );
		return ZR_FAILED;
	}
	ZRESULT zr;
	char* buf = 0;
	unsigned int bufsize = 0;
	int numItems = 0;
	CAtlMap<CString, ZIPENTRY>::CPair* ppair = NULL;

	POSITION pos = m_mapFileList.GetStartPosition();

	while( pos )
	{
		ppair = m_mapFileList.GetNext( pos );

		if( ((ZIPENTRY)(ppair->m_value)).attr & FILE_ATTRIBUTE_DIRECTORY )
		{
			zr = ZipAddFolder( zip, ((ZIPENTRY)ppair->m_value).name);
			if( zr != ZR_OK )
			{
				return zr;
			}
			continue;
		}

		if( ((ZIPENTRY)ppair->m_value).unc_size > (long)bufsize )
		{
			if( buf != 0 )
				delete [] buf;

			bufsize = ((ZIPENTRY)ppair->m_value).unc_size * 2;
			buf = new char[bufsize];
		}

		zr = UnzipItem( m_hZip, ((ZIPENTRY)ppair->m_value).index, buf, bufsize );
		if( zr != ZR_OK )
		{
			return zr;
		}
		zr = ZipAdd( zip, ((ZIPENTRY)ppair->m_value).name, buf, ((ZIPENTRY)ppair->m_value).unc_size );
		if( zr != ZR_OK )
		{
			return zr;
		}
	}

	delete [] buf;

	return ZR_OK;
}

ZRESULT			ZipArchive::MergeZipToZip(const TCHAR* szZipName )
{
	ZipArchive other( szZipName, 0, Read );
	// open other zip file.
	ZRESULT zr = MergeZipToZip( other );

	other.CloseArchive();

	return zr;
}

ZRESULT			ZipArchive::MergeZipToZip(ZipArchive& other )
{
	if( !m_bIsOpen )
		return ZR_FAILED;

	_makeUniqueKey( other._getHashmapPointer() );
	// base file size.
	int size = GetZipFileSize();
	// add to other file size.
	size += other.GetZipFileSize();
	size *= 2;	// just to be on the safe side.

	if( ZR_OK != _createMemArchive( size ) )	ZR_FAILED;

	// var.
	ZRESULT zr;
	char* buf = 0;
	unsigned int bufsize = 0;
	int numItems = 0;
	CAtlMap<CString, ZIPENTRY>::CPair* ppair = NULL;

	POSITION pos = m_mapFileList.GetStartPosition();

	while( pos )
	{
		ppair = m_mapFileList.GetNext( pos );

		if( ((ZIPENTRY)(ppair->m_value)).attr & FILE_ATTRIBUTE_DIRECTORY )
		{
			zr = _writeFolderToMem( ((ZIPENTRY)ppair->m_value).name);
			if( zr != ZR_OK )
			{
				_closeMemArchive();
				return zr;
			}
			continue;
		}

		if( ((ZIPENTRY)ppair->m_value).unc_size > (long)bufsize )
		{
			if( buf != 0 )
				delete [] buf;

			bufsize = ((ZIPENTRY)ppair->m_value).unc_size * 2;
			buf = new char[bufsize];
		}

		zr = UnzipItem( m_hZip, ((ZIPENTRY)ppair->m_value).index, buf, bufsize );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
		zr = _writeFileToMem( ((ZIPENTRY)ppair->m_value).name, buf, ((ZIPENTRY)ppair->m_value).unc_size );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
	}

	delete [] buf;

	// 나 자신을 주고 거기에 나머지를 써 넣는다.
	other._appendAllFileToZip( m_hMemZip );

	zr = _closeArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _memCopyToFIle( m_szFileName );
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}

	zr = _openArchive();
	if( zr != ZR_OK )
	{
		_closeMemArchive();
		return zr;
	}


	return ZR_OK;
}

ZRESULT			ZipArchive::_openArchive(const TCHAR* szName, const TCHAR* szPassword )
{
	char szTemp[128];

	if( GetFileAttributes( szName ) == 0xFFFFFFFF ) return ZR_NOFILE;
	HANDLE h = CreateFile( szName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	// zip file size.
	if( h == INVALID_HANDLE_VALUE )
		return ZR_NOFILE;

	m_dwFileSize = GetFileSize( h, 0 );
	CloseHandle( h );

	m_hZip = OpenZip( szName, szPassword );
	if( m_hZip == 0 )
	{
		msg( _T("* Failed to Open file." ) );
		return ZR_READ;
	}

	ZIPENTRY ze;
	GetZipItem( m_hZip, -1, &ze );
	m_nFileCountInZip = ze.index;

	// contained file list.
	for( int cnt = 0; cnt < m_nFileCountInZip; ++cnt )
	{
		GetZipItem( m_hZip, cnt, &ze );
		memset(szTemp, 0, sizeof(szTemp));
		strcpy(szTemp, ze.name);
		strupr(szTemp);
		m_mapFileList.SetAt( szTemp, ze );
	}

	m_bIsOpen = true;

	return ZR_OK;
}

ZRESULT			ZipArchive::_createArchive(const TCHAR* szName, const TCHAR* szPassword )
{
	m_dwFileSize = 0;
	m_hZip = CreateZip( szName, szPassword );
	if( m_hZip == 0 )
	{
		msg( _T("* Failed to Create file." ) );
		return ZR_WRITE;
	}

	m_bIsOpen = true;

	return ZR_OK;
}

ZRESULT			ZipArchive::_createMemArchive(const int memSize )
{
	m_hMemZip = CreateZip( 0, memSize, 0 );
	if( m_hMemZip == 0 )
	{
		msg( _T("* zip file create failed in memory." ) );
		return ZR_FAILED;
	}

	m_bIsCreateMem = true;
	return ZR_OK;
}

ZRESULT			ZipArchive::_openArchive()
{
	return OpenArchive( m_szFileName, m_szPassword, m_Mode );
}

ZRESULT			ZipArchive::_closeArchive()
{
	m_bIsOpen = false;
	m_mapFileList.RemoveAll();
	return CloseZip(m_hZip);
}

ZRESULT			ZipArchive::_closeMemArchive()
{
	if( !m_bIsCreateMem )
		return ZR_OK;

	ZRESULT zr;
	zr = CloseZip( m_hMemZip );
	if( zr != ZR_OK )
	{
		msg( _T("* failed close memory zip." ) );
	}

	m_hMemZip = 0;
	return zr;
}

ZRESULT			ZipArchive::_writeFileToMem(const TCHAR* szDestName, const TCHAR* szSrc )
{
	ZRESULT zr;
	zr = ZipAdd( m_hMemZip, szDestName, szSrc );

	if( zr != ZR_OK )
	{
		msg( _T("* failed write file in memory zip." ) );
	}

	return zr;
}

ZRESULT			ZipArchive::_writeFileToMem(const TCHAR* szDestName, void* src, unsigned int len )
{
	ZRESULT zr;
	zr = ZipAdd( m_hMemZip, szDestName, src, len );

	if( zr != ZR_OK )
	{
		msg( _T("* failed write file in memory zip." ) );
	}

	return zr;
}

ZRESULT			ZipArchive::_writeFolderToMem(const TCHAR* szDestFolder )
{
	ZRESULT zr = ZipAddFolder( m_hMemZip, szDestFolder );
	if( zr != ZR_OK )
	{
		msg( _T("* failed write directory in memory zip." ) );
	}

	return zr;
}

ZRESULT			ZipArchive::_removeFileInZip(const TCHAR* zefn )
{
	CString temp = zefn;
	temp.Replace( "\\", "/" );
	if( _removeKey( temp ) )
		return ZR_OK;
	return ZR_FAILED;
}

ZRESULT			ZipArchive::_reBuildInZip()
{
	// var.
	ZRESULT zr;
	char* buf = 0;
	unsigned int bufsize = 0;
	int numItems = 0;
	CAtlMap<CString, ZIPENTRY>::CPair* ppair = NULL;

	POSITION pos = m_mapFileList.GetStartPosition();

	while( pos )
	{
		ppair = m_mapFileList.GetNext( pos );

		if( ((ZIPENTRY)(ppair->m_value)).attr & FILE_ATTRIBUTE_DIRECTORY )
		{
			zr = _writeFolderToMem( ((ZIPENTRY)ppair->m_value).name);
			if( zr != ZR_OK )
			{
				_closeMemArchive();
				return zr;
			}
			continue;
		}

		if( ((ZIPENTRY)ppair->m_value).unc_size > (long)bufsize )
		{
			if( buf != 0 )
				delete [] buf;

			bufsize = ((ZIPENTRY)ppair->m_value).unc_size * 2;
			buf = new char[bufsize];
		}

		zr = UnzipItem( m_hZip, ((ZIPENTRY)ppair->m_value).index, buf, bufsize );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
		zr = _writeFileToMem( ((ZIPENTRY)ppair->m_value).name, buf, ((ZIPENTRY)ppair->m_value).unc_size );
		if( zr != ZR_OK )
		{
			_closeMemArchive();
			return zr;
		}
	}

	delete [] buf;

	return zr;
}

ZRESULT			ZipArchive::_memCopyToFIle(const TCHAR* szDestName )
{
	HANDLE handle;
	ZRESULT zr;
	char* buf = 0;
	DWORD bufsize = 0;

	zr = ZipGetMemory( m_hMemZip, (void**)&buf, &bufsize );

	if( zr != ZR_OK )
	{
		CloseZip(m_hMemZip);
		return zr;
	}

	handle = CreateFile( szDestName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if( handle == INVALID_HANDLE_VALUE )
	{
		CloseZip( m_hMemZip);
		return ZR_WRITE;
	}

	DWORD writesize;
	WriteFile( handle, buf, bufsize, &writesize, 0 );
	CloseHandle( handle );

	return ZR_OK;
}

void			ZipArchive::_makeUniqueKey(const CAtlMap<CString, ZIPENTRY>* pmap )
{
	POSITION pos = pmap->GetStartPosition();
	while ( pos )
	{
		CString key = pmap->GetNextKey( pos );
		_removeKey( key );
	}
}
bool			ZipArchive::_removeKey(const CString _target )
{
	CString temp = _target;
	temp.Replace( "\\", "/" );
	return m_mapFileList.RemoveKey( temp );
}

const CAtlMap<CString, ZIPENTRY>*	ZipArchive::_getHashmapPointer() const
{
	return &m_mapFileList;
}

const DWORD		ZipArchive::GetZipFileSize() const
{
	return m_dwFileSize;
}