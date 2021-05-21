#pragma once

#include <atlstr.h>
#include <atlcoll.h>
#include "zip.h"
#include "unzip.h"

class ZipArchive
{
	enum { FILE_COPY_BUFFER_SIZE = 10485760, };
public:
	enum AccessMode
	{
		Read = 0,
		Write,
		ReadWrite,
	};
public:
	ZipArchive(void);
	ZipArchive( const TCHAR* szName, AccessMode _mode = Write );
	ZipArchive( const TCHAR* szName, const TCHAR* szPassword, AccessMode _mode = Write );
	virtual ~ZipArchive(void);
	ZRESULT		OpenArchive( const TCHAR* szName, const TCHAR* szPassword, AccessMode _mode = Write );
	ZRESULT		CloseArchive();

	// this function activation that write only.
	ZRESULT		WriteFileToZip( const TCHAR* szDestName, const TCHAR* szSrc );
	ZRESULT		WriteFileToZip( const TCHAR* szDestName, void* src, unsigned int len );


	// this function activation that read only.
	ZRESULT		ReadFileFromZip( const TCHAR* szName, const TCHAR* szDestName );
	long		ReadFileFromZip( const TCHAR* szName, char* &src, long& len );
	int			ReadFileFromZip( const TCHAR* szName, char* &src, int& len );
	int			SearchFIleFromZip( const TCHAR* szName );

	// add a file to a zip, possibly replacing what was there before.
	ZRESULT		AppendFileToZip( const TCHAR* szDestName, const TCHAR* szSrc );
	ZRESULT		AppendFileToZip( const TCHAR* szDestName, void* src, unsigned int len );

	// delete a file to zip, possibly replacing what was where before.
	ZRESULT		RemoveFileInZip( const TCHAR* szDFName );

	// this merge with other zip.
	ZRESULT		MergeZipToZip( const TCHAR* szZipName );
	ZRESULT		MergeZipToZip( ZipArchive& other );


	const DWORD	GetZipFileSize() const;
protected:
	ZRESULT		_openArchive( const TCHAR* szName, const TCHAR* szPassword );
	ZRESULT		_createArchive( const TCHAR* szName, const TCHAR* szPassword );
	// memory func.
	ZRESULT		_createMemArchive( const int memSize );
	// write a memory buffer only.
	ZRESULT		_writeFileToMem( const TCHAR* zefn, const TCHAR* szSrc );
	ZRESULT		_writeFileToMem( const TCHAR* zefn, void* src, unsigned int len );
	ZRESULT		_writeFolderToMem( const TCHAR* szDestFolder );

	ZRESULT		_appendAllFileToZip( HZIP& zip );
	// remove file in zip.
	ZRESULT		_removeFileInZip( const TCHAR* zefn );

	// rebuild zip file. if remove some files in zip, must call this func.
	ZRESULT		_reBuildInZip();

	// export mem zip to real file.
	ZRESULT		_memCopyToFIle( const TCHAR* szDestName );

	ZRESULT		_closeMemArchive();
	ZRESULT		_closeArchive();
	ZRESULT		_openArchive();

	// remove value of matched key in map.
	void		_makeUniqueKey( const CAtlMap<CString, ZIPENTRY>* pmap );
	bool		_removeKey( const CString _target );

	// get hash map pointer.
	const CAtlMap<CString, ZIPENTRY>*		_getHashmapPointer() const;


	void		Destroy();
protected:
	CString				m_szFileName;
	CString				m_szPassword;
	AccessMode			m_Mode;
	HZIP				m_hZip;
	HZIP				m_hMemZip;
	DWORD				m_dwFileSize;
	bool				m_bIsOpen;
	bool				m_bIsCreateMem;
	int					m_nFileCountInZip;
	CAtlMap<CString, ZIPENTRY>				m_mapFileList;
public:

};
