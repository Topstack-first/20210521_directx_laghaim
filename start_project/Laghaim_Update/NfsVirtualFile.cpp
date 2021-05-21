

//#ifdef _USING_TOOL_LAUNCHER
	//#include "stdafx.h"
//#endif

#include "StdAfx.h"
#include <fstream>
#include <fcntl.h>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include "NfsVirtualFile.h"


NfsVirtualFile::NfsVirtualFile(void)
{
	m_dwNfsfileSize = 0;

	// miro/add:20070420
	memset(m_szPath, 0, sizeof(m_szPath));

	strcpy(m_szDataSuff, nfs_DATA_SUFFIX);
	strcpy(m_szIIDSuff, nfs_IIO_SUFFIX);
	
	m_nfsFileOpen			= false;
	// miro/end

	_mergeStart				= NULL;
	_merging				= NULL;
	_mergeEnd				= NULL;
	ZeroMemory( &m_NfsgrobFolder, sizeof( nfs_glob_t ) );
	ZeroMemory( &m_NfsgrobFile, sizeof( nfs_glob_t ) );

	m_dwNfsfileSize =  0L;
	//ZeroMemory( m_NfsBuffer, FILE_COPY_BUFFER_SIZE );
	m_NfsBuffer = new char[FILE_COPY_BUFFER_SIZE];
	if( !m_NfsBuffer )
		printf("Error Code : m_NfsBuffer!!\n");
	m_pTempBuff = new char[FILE_COPY_BUFFER_SIZE];
	if( !m_pTempBuff )
		printf("Error Code : m_pTempBuff!!\n");
}


NfsVirtualFile::~NfsVirtualFile(void)
{
	delete m_NfsBuffer;
	delete m_pTempBuff;
}


bool NfsVirtualFile::fileopen(const char* l_pfile , bool bWrite)
{
	m_dwNfsfileSize = 0; 
	if(bWrite == true)
		m_Nfs = nfs_start(const_cast<char *>(l_pfile) , FS_RW);
	else
		m_Nfs = nfs_start(const_cast<char *>(l_pfile) , FS_READ);


	// miro/modify:20070420
	//////if(!m_Nfs) 
	//////	return false;

	//////return true;

	strcpy(m_szPath, l_pfile);

	return m_nfsFileOpen	= (!m_Nfs) ? false : true;
	// miro/end

}

void NfsVirtualFile::fileclose()
{
	// miro/add:20070420
	m_nfsFileOpen			= false;
	// miro/end

	nfs_end(m_Nfs, 0);
}

unsigned int NfsVirtualFile::read(const char* l_pfile)
{
	int ifMode;
	int ReadCount = 0;
	unsigned int ifilesize =0 ; 
	ifMode = nfs_file_open(m_Nfs, l_pfile , O_RDWR);
	
	if(ifMode >= 0)
	{
		for(;;)
		{
			ReadCount = nfs_file_read(m_Nfs , ifMode , m_NfsBuffer , FILE_COPY_BUFFER_SIZE) ;
			m_dwNfsfileSize += ReadCount; 
			ifilesize += ReadCount;
			if(ReadCount < FILE_COPY_BUFFER_SIZE) break;
		}
		nfs_file_close(m_Nfs , ifMode);
	}
	
	return ifilesize;
}


// "./dfd/sd/f.jpg"
bool NfsVirtualFile::Nfsfileopen(const char* l_pfile)
{
	m_ifMode = nfs_file_open(m_Nfs, l_pfile , O_RDWR);
	if(m_ifMode < 0) return false;
	return true;
}


unsigned int NfsVirtualFile::getNfs_file_read()
{
	int ReadCount = 0;
	if(m_ifMode >= 0)
	{
		ReadCount = nfs_file_read(m_Nfs , m_ifMode , m_NfsBuffer , FILE_COPY_BUFFER_SIZE) ;
	}
	return ReadCount;
}

unsigned int NfsVirtualFile::write(const char* l_pfile , const char* l_pSourcefile)
{
	int ifMode;
	//char buffer[FILE_COPY_BUFFER_SIZE];
	int ReadCount = 0;
	int WriteCount = 0;
	unsigned int ifilesize =0 ; 
	//buffer = new char[FILE_COPY_BUFFER_SIZE];

	if( nfs_file_exists( m_Nfs, const_cast<char*>(l_pSourcefile) ) )
	{	
		printf("File %s is Exists!!!\n", l_pSourcefile);
		return -1;
	}
	ifMode = nfs_file_create(m_Nfs , l_pfile , 0);
	if(ifMode >= 0)
	{
		ifstream  ifSearhfile(l_pSourcefile , std::ios_base::out | std::ios_base::binary);
		if(!ifSearhfile.bad())
		{
			for(;;)
			{
				ifSearhfile.read( m_NfsBuffer , FILE_COPY_BUFFER_SIZE);
				ReadCount = ifSearhfile.gcount();
				if(0 == ReadCount) break;
				//WriteCount = nfs_file_write(m_Nfs , ifMode , buffer , ReadCount);
				WriteCount = nfs_file_write(m_Nfs , ifMode , m_NfsBuffer , ReadCount);
				m_dwNfsfileSize += WriteCount; 
				ifilesize += ReadCount;
			}
		}
		ifSearhfile.close();
	}
	nfs_file_close(m_Nfs , ifMode);
	//delete buffer;
	return ifilesize;
}


unsigned int NfsVirtualFile::write(const char* l_pfile , const char* l_pBuffer , DWORD l_size)
{
	int ifMode;
	//char buffer[FILE_COPY_BUFFER_SIZE];;
	//int ReadCount = 0;
	int WriteCount = 0;
	unsigned int ifilesize =0 ; 

	if( nfs_file_exists( m_Nfs, const_cast<char*>(l_pfile) ) )
	{
		printf("file %s already exists\n", l_pfile );
		return -1;
	}

	ifMode = nfs_file_create(m_Nfs , l_pfile , O_APPEND);
	if(ifMode >= 0)
	{
		WriteCount = nfs_file_write(m_Nfs , ifMode , l_pBuffer , l_size);
	}
	nfs_file_close(m_Nfs , ifMode);
	return WriteCount;
}	


bool NfsVirtualFile::Nfsfileclose()
{
	nfs_file_close(m_Nfs , m_ifMode);
	m_ifMode = -1;
	return true;
}


void NfsVirtualFile::globFolder(const char* l_path)
{
	
	nfs_glob(m_Nfs, l_path, GLOB_NOSORT, NULL, &m_NfsgrobFolder);
	//nfs_glob_free(m_Nfs , &m_Nfsgrob);
}

void NfsVirtualFile::globFolderclose()
{
	nfs_glob_free(m_Nfs , &m_NfsgrobFolder);
}

void NfsVirtualFile::globFile(const char* l_path)
{
	
	nfs_glob(m_Nfs, l_path, GLOB_NOSORT, NULL, &m_NfsgrobFile);
	//nfs_glob_free(m_Nfs , &m_Nfsgrob);
}

void NfsVirtualFile::globFileclose()
{
	nfs_glob_free(m_Nfs , &m_NfsgrobFile);
}

unsigned int NfsVirtualFile::getNfs_file_write(const char* l_pBuffer , DWORD l_size)
{
	int WriteCount = 0;
	if(m_ifMode >= 0)
	{
		WriteCount = nfs_file_write(m_Nfs , m_ifMode , l_pBuffer , l_size) ;
	}
	return WriteCount;
}


// miro/add:20070420
bool NfsVirtualFile::marge(const char* l_path)
{

	NfsVirtualFile			nfsSource;

	nfsSource.fileopen(l_path, false);
	nfsSource.globFolder("*.*");
	
	nfs_glob_t*				pnfs_globFolder_t	= nfsSource.getNfsFolder_glob_t();
	int						ifileSize			= 0 ;


	if(_mergeStart)
	{
		(*_mergeStart)(pnfs_globFolder_t->gl_pathc * 1024);
	}


	for(int iCnt = 0; iCnt < pnfs_globFolder_t->gl_pathc ;iCnt++)
	{
		int					ifMode			= nfs_file_create(m_Nfs , pnfs_globFolder_t->gl_pathv[iCnt] , 0);

		if(!(ifMode >= 0))
			continue;

		nfsSource.Nfsfileopen(pnfs_globFolder_t->gl_pathv[iCnt]);

		while(0 < (ifileSize = nfsSource.getNfs_file_read()))
		{
			nfs_file_write(m_Nfs , ifMode , nfsSource.GetNfsBuffer(), ifileSize);

			//printf("fileSize : %d  %s\n", ifileSize, pnfs_globFolder_t->gl_pathv[iCnt]);
			//fprintf(fp, "fileSize :\t %10d \t %s\n", ifileSize, pnfs_globFolder_t->gl_pathv[iCnt]);
		}

		if(_merging)
		{
			(*_merging)(1024);
		}

		nfs_file_close(m_Nfs , ifMode);
		nfsSource.Nfsfileclose();
	}

	nfsSource.globFolderclose();
	nfsSource.fileclose();


	if(_mergeEnd)
	{
		(*_mergeEnd)();
	}


	return true;
}
// miro/end

// [2/23/2009 D.K ] : update func.
bool NfsVirtualFile::update( const char* l_path )
{
	NfsVirtualFile*				nfsSource;
	NfsVirtualFile*				nfsDest;
	list<string>		sourceList;
	list<string>		sourceUsedList;
	list<string>		sourceDifference;
	
	char szTemp[256] = {0,};
	strcpy( szTemp, l_path );
	char* p = strrchr( szTemp, '_' );
	szTemp[ p - szTemp ] = '\0';
	strcat( szTemp, "_pack" );

	nfsSource = new NfsVirtualFile();
	nfsDest = new NfsVirtualFile();
	nfsSource->fileopen( l_path, false );
	nfsDest->fileopen( szTemp );
	globFolder( "*.*" );
	nfsSource->globFolder( "*.*" );
	

	nfs_glob_t*				pnfs_globFolder_t	= NULL;
	int						ifileSize			= 0;
	pnfs_globFolder_t = nfsSource->getNfsFolder_glob_t();

	for( int i = 0; i < pnfs_globFolder_t->gl_pathc; i++ )
		sourceList.push_back( string( pnfs_globFolder_t->gl_pathv[i] ) );

	nfsSource->globFolderclose();

	for( int iCnt = 0; iCnt < m_NfsgrobFolder.gl_pathc; iCnt++ )
	{
		nfsSource->globFolder( m_NfsgrobFolder.gl_pathv[iCnt] );
		pnfs_globFolder_t = nfsSource->getNfsFolder_glob_t();
		// patch 파일에 같은 파일이 있다면 패치파일에서 정보를 읽고 아니라면 
		// expansion에서 읽는다.
		if( pnfs_globFolder_t->gl_pathc )
		{
			nfsSource->Nfsfileopen( pnfs_globFolder_t->gl_pathv[0] );
			int fileSize = nfsSource->getNfs_file_read();
			nfsDest->write( pnfs_globFolder_t->gl_pathv[0], nfsSource->GetNfsBuffer(), fileSize );
			sourceUsedList.push_back( string( pnfs_globFolder_t->gl_pathv[0] ) );
			nfsSource->Nfsfileclose();
		}
		else
		{
			Nfsfileopen( m_NfsgrobFolder.gl_pathv[iCnt] );
			int iFileSize = getNfs_file_read();
			nfsDest->write( m_NfsgrobFolder.gl_pathv[iCnt], GetNfsBuffer(), iFileSize );
			Nfsfileclose();
		}

		nfsSource->globFolderclose();
	}

	compareStringList( sourceList, sourceUsedList, sourceDifference );

	// [2/23/2009 D.K ] : append file
	for( StringListItr itr = sourceDifference.begin(); itr != sourceDifference.end(); itr++ )
	{
		nfsSource->Nfsfileopen( (*itr).c_str() );
		int iFileSize = nfsSource->getNfs_file_read();
		nfsDest->write( (*itr).c_str(), nfsSource->GetNfsBuffer(), iFileSize );
	}

	nfsSource->fileclose();
	nfsDest->fileclose();

	globFolderclose();

	delete nfsSource;
	delete nfsDest;

	return true;
}

bool NfsVirtualFile::updatefromfile( const char* l_path, const char* nfsPath )
{
	NfsVirtualFile*			nfsPatched = NULL;
	char szTemp[256] = {0,};
	strcpy( szTemp, nfsPath );
	char* p = strrchr( szTemp, '_' );
	char	szCurDir[MAX_PATH] = {0,};
	szTemp[ p - szTemp ] = '\0';
	strcat( szTemp, "_pack" );

	nfsPatched = new NfsVirtualFile();
	if( !nfsPatched )
		return false;

	nfsPatched->fileopen( szTemp );
	
	GetCurrentDirectory( MAX_PATH, szCurDir );
	SetCurrentDirectory( l_path );

	packfrompath( "*.*", szCurDir, nfsPatched );

	globFolder( "*.*" );
	for( int i = 0; i < m_NfsgrobFolder.gl_pathc; i++ )
	{
		nfsPatched->globFolder( m_NfsgrobFolder.gl_pathv[i] );
		nfs_glob_t* pGlob = nfsPatched->getNfsFolder_glob_t();
		if( pGlob->gl_pathc )
			continue;			// patch된 내용임. 건너뛴다.

		Nfsfileopen( m_NfsgrobFolder.gl_pathv[i] );
		int nSize = getNfs_file_read();
		nfsPatched->write( m_NfsgrobFolder.gl_pathv[i], GetNfsBuffer(), nSize );
		Nfsfileclose();
		nfsPatched->globFolderclose();
	}

	globFolderclose();
	nfsPatched->fileclose();
	SetCurrentDirectory( szCurDir );

	return true;
}

void NfsVirtualFile::packfrompath(const char* l_path, const char* szCurDir, NfsVirtualFile* nfsDest )
{
	HANDLE h = NULL;
	WIN32_FIND_DATA wfd;
	char fname[MAX_PATH] = {0,};
	BOOL bResult = TRUE;
	char drive[_MAX_DRIVE] = {0,};
	char dir[MAX_PATH] = {0,};
	char newpath[MAX_PATH] = {0,};
	ZeroMemory( &wfd, sizeof( WIN32_FIND_DATA ) );

	h = FindFirstFile( l_path , &wfd );
	while( bResult )
	{
		_splitpath( l_path, drive,  dir,  NULL, NULL );
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )	
		{
			if( wfd.cFileName[0] != '.' )
			{
				sprintf( newpath, "%s%s%s/*.*", drive, dir, wfd.cFileName );
				packfrompath( newpath, szCurDir, nfsDest );				// 재귀호출
			}
		}else
		{
			sprintf( fname, "%s%s%s", drive, dir, wfd.cFileName );
			_strupr( fname );
			if( strlen(dir) == 0 )
			{
				std::string szTemp;
				szTemp = szCurDir;
				szTemp += "\\";
				szTemp += wfd.cFileName;
				BOOL result = CopyFile( wfd.cFileName, szTemp.c_str(), FALSE );
			}
			else
				int size = nfsDest->write( fname, fname );
		}
		bResult = FindNextFile( h, &wfd );
	}
	FindClose( h );
}

void NfsVirtualFile::compareStringList( StringList& c1, StringList& c2, StringList& store )
{
	bool hit = false;
	for ( StringListItr c1itr = c1.begin(); c1itr != c1.end(); c1itr++ )
	{
		hit = false;
		for( StringListItr c2itr = c2.begin(); c2itr != c2.end(); c2itr++ )
		{
			if( (*c1itr) == (*c2itr) )
			{
				hit = true;
				break;
			}
		}
		if( !hit )
			store.push_back( (*c1itr ) );
	}
}