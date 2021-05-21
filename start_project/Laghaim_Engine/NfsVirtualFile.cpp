#include "stdafx.h"
#include <fstream>
#include <fcntl.h>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include "NfsVirtualFile.h"

unsigned char cripttable[] = {
	103, 198, 105, 115,  81, 255,  74, 236,  41, 205, 186, 171, 242, 251, 227,  70, 124, 194,  84, 248,  27, 232, 231, 141, \
	118,  90,  46,  99,  51, 159, 201, 154, 102,  50,  13, 183,  49,  88, 163,  90,  37,  93,   5,  23,  88, 233,  94, 212, \
	171, 178, 205, 198, 155, 180,  84,  17,  14, 130, 116,  65,  33,  61, 220, 135, 112, 233,  62, 161,  65, 225, 252, 103, \
	 62,   1, 126, 151, 234, 220, 107, 150, 143,  56,  92,  42, 236, 176,  59, 251,  50, 175,  60,  84, 236,  24, 219,  92, \
	  2,  26, 254,  67, 251, 250, 170,  58, 251,  41, 209, 230,   5,  60, 124, 148, 117, 216, 190,  97, 137, 249,  92, 187, \
	168, 153,  15, 149, 177, 235, 241, 179,   5, 239, 247,   0, 233, 161,  58, 229, 202,  11, 203, 208,  72,  71, 100, 189, \
	 31,  35,  30, 168,  28, 123, 100, 197,  20, 115,  90, 197,  94,  75, 121,  99,  59, 112, 100,  36,  17, 158,   9, 220, \
	170, 212, 172, 242,  27,  16, 175,  59,  51, 205, 227,  80,  72,  71,  21,  92, 187, 111,  34,  25, 186, 155, 125, 245, \
	 11, 225,  26,  28, 127,  35, 248,  41, 248, 164,  27,  19, 181, 202,  78, 232, 152,  50,  56, 224, 121,  77,  61,  52, \
	188,  95,  78, 119, 250, 203, 108,   5, 172, 134,  33,  43, 170,  26,  85, 162, 190, 112, 181, 115,  59,   4,  92, 211, \
	 54, 148, 179, 175, 226, 240, 228, 158,  79,  50,  21,  73, 253, 130,  78, 169};

NfsVirtualFile::NfsVirtualFile(void)
{
	m_dwNfsfileSize = 0;
	m_nfsFileOpen = false;
	
	ZeroMemory(m_szPath, sizeof(m_szPath));
	ZeroMemory(&m_NfsgrobFolder, sizeof( nfs_glob_t ));
	ZeroMemory(&m_NfsgrobFile, sizeof( nfs_glob_t ));

	strcpy(m_szDataSuff, nfs_DATA_SUFFIX);
	strcpy(m_szIIDSuff, nfs_IIO_SUFFIX);
	
	m_NfsBuffer = new char[FILE_COPY_BUFFER_SIZE];
	m_pTempBuff = new char[FILE_COPY_BUFFER_SIZE];

	_mergeStart	= NULL;
	_merging = NULL;
	_mergeEnd = NULL;
}


NfsVirtualFile::~NfsVirtualFile(void)
{
	delete m_NfsBuffer;
	delete m_pTempBuff;
}


bool NfsVirtualFile::fileopen(const char* l_pfile, bool bWrite)
{
	m_dwNfsfileSize = 0;

	if( bWrite == true )
	{
		m_Nfs = nfs_start(const_cast<char *>(l_pfile), FS_RW);
	}
	else
	{
		m_Nfs = nfs_start(const_cast<char *>(l_pfile), FS_READ);
	}

	strcpy(m_szPath, l_pfile);	

	m_nfsFileOpen = (m_Nfs == NULL) ? false : true;

	return m_nfsFileOpen;
}

void NfsVirtualFile::fileclose()
{	
	m_nfsFileOpen = false;
	nfs_end(m_Nfs, 0);
}

unsigned int NfsVirtualFile::read(const char* l_pfile)
{	
	int ReadCount = 0;
	unsigned int ifilesize =0 ;
	int ifMode = nfs_file_open(m_Nfs, l_pfile, O_RDWR);

	if( ifMode >= 0 )
	{
		int criptcounter = 0;
		for(;;)
		{
			ReadCount = nfs_file_read(m_Nfs, ifMode, m_NfsBuffer, FILE_COPY_BUFFER_SIZE);
			m_dwNfsfileSize += ReadCount;
			ifilesize += ReadCount;

			for(int i = 0; i < ReadCount; i++)
			{
				m_NfsBuffer[i] = m_NfsBuffer[i] ^ cripttable[criptcounter];
				criptcounter++;
				criptcounter = criptcounter % 256;
			}

			if( ReadCount < FILE_COPY_BUFFER_SIZE )
			{
				break;
			}
		}

		nfs_file_close(m_Nfs, ifMode);
	}

	return ifilesize;
}

bool NfsVirtualFile::Nfsfileopen(const char* l_pfile)
{
	m_ifMode = nfs_file_open(m_Nfs, l_pfile, O_RDWR);
	
	if( m_ifMode < 0 )
	{
		return false;
	}

	return true;
}

unsigned int NfsVirtualFile::getNfs_file_read()
{
	int ReadCount = 0;
	
	if( m_ifMode >= 0 )
	{
		ReadCount = nfs_file_read(m_Nfs, m_ifMode, m_NfsBuffer, FILE_COPY_BUFFER_SIZE);
	}

	return ReadCount;
}

unsigned int NfsVirtualFile::write(const char* l_pfile, const char* l_pSourcefile)
{
	if( nfs_file_exists(m_Nfs, const_cast<char*>(l_pSourcefile)) )
	{	
		return -1;
	}

	int ReadCount = 0;
	int WriteCount = 0;
	unsigned int ifilesize =0 ;	
	int ifMode = nfs_file_create(m_Nfs, l_pfile, 0);

	if( ifMode >= 0 )
	{
		ifstream ifSearhfile(l_pSourcefile, std::ios_base::out | std::ios_base::binary);

		if( ifSearhfile.bad() == false )
		{
			for(;;)
			{
				ifSearhfile.read(m_NfsBuffer, FILE_COPY_BUFFER_SIZE);
				ReadCount = ifSearhfile.gcount();
				
				if(0 == ReadCount)
				{
					break;
				}
				
				WriteCount = nfs_file_write(m_Nfs, ifMode, m_NfsBuffer, ReadCount);
				m_dwNfsfileSize += WriteCount;
				ifilesize += ReadCount;
			}
		}

		ifSearhfile.close();
	}

	nfs_file_close(m_Nfs , ifMode);
	
	return ifilesize;
}


unsigned int NfsVirtualFile::write(const char* l_pfile, const char* l_pBuffer, DWORD l_size)
{
	if( nfs_file_exists(m_Nfs, const_cast<char*>(l_pfile)) )
	{	
		return -1;
	}

	int WriteCount = 0;	
	int ifMode = nfs_file_create(m_Nfs, l_pfile, O_APPEND);

	if( ifMode >= 0 )
	{
		WriteCount = nfs_file_write(m_Nfs , ifMode , l_pBuffer , l_size);
	}

	nfs_file_close(m_Nfs , ifMode);

	return WriteCount;
}

bool NfsVirtualFile::Nfsfileclose()
{
	nfs_file_close(m_Nfs, m_ifMode);
	m_ifMode = -1;

	return true;
}

void NfsVirtualFile::globFolder(const char* l_path)
{
	nfs_glob(m_Nfs, l_path, GLOB_NOSORT, NULL, &m_NfsgrobFolder);	
}

void NfsVirtualFile::globFolderclose()
{
	nfs_glob_free(m_Nfs, &m_NfsgrobFolder);
}

void NfsVirtualFile::globFile(const char* l_path)
{
	nfs_glob(m_Nfs, l_path, GLOB_NOSORT, NULL, &m_NfsgrobFile);	
}

void NfsVirtualFile::globFileclose()
{
	nfs_glob_free(m_Nfs , &m_NfsgrobFile);
}

unsigned int NfsVirtualFile::getNfs_file_write(const char* l_pBuffer, DWORD l_size)
{
	int WriteCount = 0;

	if( m_ifMode >= 0 )
	{
		WriteCount = nfs_file_write(m_Nfs, m_ifMode, l_pBuffer, l_size);
	}

	return WriteCount;
}

bool NfsVirtualFile::marge(const char* l_path)
{
	NfsVirtualFile nfsSource;

	nfsSource.fileopen(l_path, false);
	nfsSource.globFolder("*.*");

	nfs_glob_t*	pnfs_globFolder_t = nfsSource.getNfsFolder_glob_t();
	int	ifileSize = 0;

	if( _mergeStart )
	{
		(*_mergeStart)(pnfs_globFolder_t->gl_pathc * 1024);
	}

	for( int iCnt = 0; iCnt < pnfs_globFolder_t->gl_pathc; ++iCnt )
	{
		int	ifMode = nfs_file_create(m_Nfs , pnfs_globFolder_t->gl_pathv[iCnt] , 0);

		if( ifMode < 0 )
		{
			continue;
		}

		nfsSource.Nfsfileopen(pnfs_globFolder_t->gl_pathv[iCnt]);

		while( 0 < (ifileSize = nfsSource.getNfs_file_read()) )
		{
			nfs_file_write(m_Nfs , ifMode , nfsSource.GetNfsBuffer(), ifileSize);
		}

		if( _merging )
		{
			(*_merging)(1024);
		}

		nfs_file_close(m_Nfs , ifMode);
		nfsSource.Nfsfileclose();
	}

	nfsSource.globFolderclose();
	nfsSource.fileclose();
	
	if( _mergeEnd )
	{
		(*_mergeEnd)();
	}

	return true;
}

bool NfsVirtualFile::update(const char* l_path)
{
	NfsVirtualFile*		nfsSource;
	NfsVirtualFile*		nfsDest;
	list<string>		sourceList;
	list<string>		sourceUsedList;
	list<string>		sourceDifference;

	char szTemp[256] = {0,};
	strcpy( szTemp, l_path );
	char* p = strrchr(szTemp, '_');
	szTemp[p - szTemp] = '\0';
	strcat(szTemp, "_pack");

	nfsSource = new NfsVirtualFile();
	nfsDest = new NfsVirtualFile();
	nfsSource->fileopen(l_path, false);
	nfsDest->fileopen(szTemp);

	globFolder("*.*");
	nfsSource->globFolder("*.*");

	nfs_glob_t*	pnfs_globFolder_t = NULL;
	int	ifileSize = 0;
	pnfs_globFolder_t = nfsSource->getNfsFolder_glob_t();

	for( int i = 0; i < pnfs_globFolder_t->gl_pathc; ++i )
	{
		sourceList.push_back(std::string(pnfs_globFolder_t->gl_pathv[i]));
	}

	nfsSource->globFolderclose();

	for( int iCnt = 0; iCnt < m_NfsgrobFolder.gl_pathc; ++iCnt )
	{
		nfsSource->globFolder(m_NfsgrobFolder.gl_pathv[iCnt]);
		pnfs_globFolder_t = nfsSource->getNfsFolder_glob_t();
		
		if( pnfs_globFolder_t->gl_pathc )
		{
			nfsSource->Nfsfileopen(pnfs_globFolder_t->gl_pathv[0]);

			int fileSize = nfsSource->getNfs_file_read();
			nfsDest->write(pnfs_globFolder_t->gl_pathv[0], nfsSource->GetNfsBuffer(), fileSize);
			sourceUsedList.push_back(std::string(pnfs_globFolder_t->gl_pathv[0]));

			nfsSource->Nfsfileclose();
		}
		else
		{
			Nfsfileopen(m_NfsgrobFolder.gl_pathv[iCnt]);
			
			int iFileSize = getNfs_file_read();
			nfsDest->write(m_NfsgrobFolder.gl_pathv[iCnt], GetNfsBuffer(), iFileSize);
			
			Nfsfileclose();
		}

		nfsSource->globFolderclose();
	}

	compareStringList( sourceList, sourceUsedList, sourceDifference );
	
	for( StringListItr itr = sourceDifference.begin(); itr != sourceDifference.end(); ++itr )
	{
		nfsSource->Nfsfileopen((*itr).c_str());
		
		int iFileSize = nfsSource->getNfs_file_read();
		nfsDest->write((*itr).c_str(), nfsSource->GetNfsBuffer(), iFileSize);
		
		nfsSource->Nfsfileclose();
	}

	nfsSource->fileclose();
	nfsDest->fileclose();

	globFolderclose();

	delete nfsSource;
	delete nfsDest;

	return true;
}

bool NfsVirtualFile::updatefromfile(const char* l_path, const char* nfsPath)
{
	NfsVirtualFile* nfsPatched = NULL;
	char szTemp[256] = {0,};
	strcpy(szTemp, nfsPath);
	char* p = strrchr(szTemp, '_');
	char szCurDir[MAX_PATH] = {0,};
	szTemp[p - szTemp] = '\0';
	strcat(szTemp, "_pack");

	nfsPatched = new NfsVirtualFile();
	nfsPatched->fileopen( szTemp );

	GetCurrentDirectory(MAX_PATH, szCurDir);
	SetCurrentDirectory(l_path);

	packfrompath("*.*", nfsPatched);
	globFolder("*.*");

	for( int i = 0; i < m_NfsgrobFolder.gl_pathc; ++i )
	{
		nfsPatched->globFolder(m_NfsgrobFolder.gl_pathv[i]);
		nfs_glob_t* pGlob = nfsPatched->getNfsFolder_glob_t();
		if( pGlob->gl_pathc )
		{
			continue;
		}

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

void NfsVirtualFile::packfrompath(const char* l_path, NfsVirtualFile* nfsDest )
{
	HANDLE h = NULL;
	WIN32_FIND_DATA wfd;
	char fname[MAX_PATH] = {0,};
	BOOL bResult = TRUE;
	char drive[_MAX_DRIVE] = {0,};
	char dir[MAX_PATH] = {0,};
	char newpath[MAX_PATH] = {0,};
	ZeroMemory(&wfd, sizeof( WIN32_FIND_DATA ));

	h = FindFirstFile(l_path, &wfd);

	while( bResult )
	{
		_splitpath(l_path, drive, dir, NULL, NULL);

		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( wfd.cFileName[0] != '.' )
			{	
				sprintf(newpath, "%s%s%s/*.*", drive, dir, wfd.cFileName);
				packfrompath(newpath, nfsDest);
			}
		}
		else
		{
			sprintf(fname, "%s%s%s", drive, dir, wfd.cFileName);
			_strupr(fname);
			nfsDest->write(fname, fname);
		}

		bResult = FindNextFile(h, &wfd);
	}

	FindClose(h);
}

void NfsVirtualFile::compareStringList(StringList& c1, StringList& c2, StringList& store)
{
	bool hit = false;

	for( StringListItr c1itr = c1.begin(); c1itr != c1.end(); ++c1itr )
	{
		hit = false;

		for( StringListItr c2itr = c2.begin(); c2itr != c2.end(); ++c2itr )
		{
			if( (*c1itr) == (*c2itr) )
			{
				hit = true;
				break;
			}
		}

		if( hit == false )
		{
			store.push_back((*c1itr));
		}
	}
}