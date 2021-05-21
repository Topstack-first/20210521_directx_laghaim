#pragma once

#include "nfs.h"
//#include "Nfs/nfs.h"


//#include <fcntl.h>
//#include <fstream>
#include <windows.h>

#include <list>
#include <string>

using	namespace	std;
typedef list<string>				StringList;
typedef list<string>::iterator		StringListItr;


class NfsVirtualFile
{
public : 
	//enum { FILE_COPY_BUFFER_SIZE = 20971520 } ;
	enum { FILE_COPY_BUFFER_SIZE = 10485760 } ;

public:
	NfsVirtualFile(void);
	~NfsVirtualFile(void);

private :
	nfs_Handle* m_Nfs;
	int m_ifMode;
	nfs_glob_t  m_NfsgrobFolder;
	nfs_glob_t  m_NfsgrobFile;

	DWORD m_dwNfsfileSize;
	char *m_NfsBuffer;//[FILE_COPY_BUFFER_SIZE];
	char *m_pTempBuff;


	// miro/add:20070420
	bool					m_nfsFileOpen;
	char					m_szPath[MAX_PATH];			// 파일이름 저장
	// miro/end


	// miro/add:20070516
	void 					(*_mergeStart)		(int nMergeMaxSize);
	void 					(*_merging)			(int nMergingSize);
	void 					(*_mergeEnd)		();
	// miro/end

public:

	// miro/add:20070420
	char					m_szDataSuff[5];
	char					m_szIIDSuff[5];
	// miro/end


public :
	bool fileopen(const char* l_pfile , bool bWrite = true);
	void fileclose();
	
	unsigned int read(const char* l_pfile);
	unsigned int write(const char* l_pfile , const char* l_pSourcefile);
	
	unsigned int write(const char* l_pfile , const char* l_pBuffer , DWORD l_size);	


	void globFolder(const char* l_path);				// folder search in packing file
	void globFolderclose();								// close folder handle

	void globFile(const char* l_path);					// file search in packing file
	void globFileclose();								// close file handle

	DWORD getfileSize() { return m_dwNfsfileSize; }		// get packing file size
	char*	GetNfsBuffer() { return m_NfsBuffer; }		// get packing file data buffer pointer.
	
	nfs_glob_t* getNfsFolder_glob_t() { return &m_NfsgrobFolder; }
	nfs_glob_t* getNfsFile_glob_t() { return &m_NfsgrobFile; }
	
	bool Nfsfileopen(const char* l_pfile);
	bool Nfsfileclose();
	unsigned int getNfs_file_read();

	unsigned int getNfs_file_write(const char* l_pBuffer , DWORD l_size);


	// miro/add:20070420
	bool					isNfsFileOpen()		{ return m_nfsFileOpen; };
	bool					marge(const char* l_path);
	bool					update( const char* l_path );
	bool					updatefromfile( const char* l_path, const char* nfsPath );
	void					compareStringList( StringList& c1, StringList& c2, StringList& store );
	void					packfrompath( const char* l_path, const char* szCurDir, NfsVirtualFile* nfsDest );
	// miro/end


	// miro/add:20070516
	void 					setMergeStart(void (*mergeStart)(int nMergeMaxSize))		{ _mergeStart	= mergeStart; };
	void 					setMergeing(void (*merging)(int nMergingSize))				{ _merging		= merging; };
	void 					setMergeEnd(void (*mergeEnd)())								{ _mergeEnd		= mergeEnd; };
	// miro/end

};























