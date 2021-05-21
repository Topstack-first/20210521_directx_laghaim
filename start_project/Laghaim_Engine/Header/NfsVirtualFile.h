#pragma once

#include <list>
#include <string>
#include "nfs.h"

using namespace	std;
typedef list<string>			StringList;
typedef list<string>::iterator	StringListItr;

class NfsVirtualFile
{
public :	
	enum { FILE_COPY_BUFFER_SIZE = 10485760 };

public:
	NfsVirtualFile(void);
	~NfsVirtualFile(void);

public :
	bool fileopen(const char* l_pfile , bool bWrite = true);
	void fileclose();

	unsigned int read(const char* l_pfile);
	unsigned int write(const char* l_pfile, const char* l_pSourcefile);
	unsigned int write(const char* l_pfile, const char* l_pBuffer, DWORD l_size);

	void globFolder(const char* l_path);
	void globFolderclose();

	void globFile(const char* l_path);
	void globFileclose();

	DWORD getfileSize()
	{
		return m_dwNfsfileSize;
	}
	char*	GetNfsBuffer()
	{
		return m_NfsBuffer;
	}

	nfs_glob_t* getNfsFolder_glob_t()
	{
		return &m_NfsgrobFolder;
	}
	nfs_glob_t* getNfsFile_glob_t()
	{
		return &m_NfsgrobFile;
	}

	bool Nfsfileopen(const char* l_pfile);
	bool Nfsfileclose();
	unsigned int getNfs_file_read();
	unsigned int getNfs_file_write(const char* l_pBuffer, DWORD l_size);
	
	bool marge(const char* l_path);
	bool update(const char* l_path);
	bool updatefromfile(const char* l_path, const char* nfsPath);
	void compareStringList(StringList& c1, StringList& c2, StringList& store);
	void packfrompath(const char* l_path, NfsVirtualFile* nfsDest);

	bool isNfsFileOpen()
	{
		return m_nfsFileOpen;
	}
	
	void setMergeStart(void (*mergeStart)(int nMergeMaxSize))
	{
		_mergeStart	= mergeStart;
	}
	void setMergeing(void (*merging)(int nMergingSize))
	{
		_merging = merging;
	}
	void setMergeEnd(void (*mergeEnd)())
	{
		_mergeEnd = mergeEnd;
	}

public:
	char		m_szDataSuff[5];
	char		m_szIIDSuff[5];	

private:
	nfs_Handle*	m_Nfs;
	int			m_ifMode;
	nfs_glob_t	m_NfsgrobFolder;
	nfs_glob_t	m_NfsgrobFile;

	DWORD		m_dwNfsfileSize;
	char*		m_NfsBuffer;
	char*		m_pTempBuff;

	bool		m_nfsFileOpen;
	char		m_szPath[MAX_PATH];

	void 		(*_mergeStart)(int nMergeMaxSize);
	void 		(*_merging)(int nMergingSize);
	void 		(*_mergeEnd)();
};























