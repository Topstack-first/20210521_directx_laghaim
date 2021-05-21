#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include "singleton.h"
#include "VirtualFile.h"

class PFILE
{
public:
	PFILE() {}
	~PFILE() {}

public:
	virtual int		Scanf(const char* format, va_list arglist) = 0;
	virtual char*	Gets(char* string, int count) = 0;
	virtual int		Getc() = 0;
	virtual int		Seek(long int offset, int whence) = 0;
	virtual PFILE*	Open(const char* filename, const char* mode) = 0;
	virtual void	Close() = 0;
	virtual size_t	Read(void* ptr, size_t size, size_t n) = 0;
	virtual int		Tell() = 0;
};

class PFILE_STD : public PFILE
{
public:
	PFILE_STD();
	~PFILE_STD();

public:
	int		Scanf(const char* format, va_list arglist);
	char*	Gets(char* string, int count);
	int		Getc();
	int		Seek(long int offset, int whence);
	PFILE*	Open(const char* filename, const char* mode);
	void	Close();
	size_t	Read(void* ptr, size_t size, size_t n);
	int		Tell();

private:
	FILE*	m_fp;
};

class PFILE_NFS : public PFILE
{
public:
	PFILE_NFS();
	~PFILE_NFS();

public:
	int		Scanf(const char* format, va_list arglist);
	char*	Gets(char* string, int count);
	int		Getc();
	int		Seek(long int offset, int whence);
	PFILE*	Open(const char* filename, const char* mode);
	void	Close();
	size_t	Read(void* ptr, size_t size, size_t n);
	int		Tell();

private:
	VFILE*	m_vfp;
};

class FileManager : public CSingleton<FileManager>
{
	enum eFILE_TYPE
	{
		eFILE_TYPE_STD = 0,
		eFILE_TYPE_NFS,
	};

public:
	FileManager();
	~FileManager();

public:
	int			Scanf(PFILE* stream, const char* format, ...);
	char*		Gets(char* string, int count, PFILE* stream);
	int			Getc(PFILE* stream);
	int			Seek(PFILE* steram, long int offset, int whence);
	PFILE*		Open(const char* filename, const char* mode, bool forceStd = false);
	void		Close(PFILE* fp);
	size_t		Read(void* ptr, size_t size, size_t n, PFILE* stream);
	int			Tell(PFILE* stream);
	void		PackInit();
	void		PackUninit();
	void		Decode(unsigned char* stream, int filesize);

private:
	eFILE_TYPE	m_eFileType;
};

#endif