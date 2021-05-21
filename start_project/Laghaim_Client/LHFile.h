#pragma once

enum LHFileMode
{
	LHFileMode__ReadText = 0,
	LHFileMode__ReadBin,
	LHFileMode__WriteText,
	LHFileMode__WriteBin,
	LHFileMode__AppendText,
	LHFileMode__AppendBin,
};



#include <tchar.h>

#include "Def.h"

class LHFile
{
private:
	PFILE * m_fp;
	unsigned char* stream;// = new unsigned char[fileSize];

	char	m_filename[MAX_PATH];
	UINT	m_filesize;


public:
	bool Open(const TCHAR * filename, LHFileMode mode);
	void Close();
	const TCHAR * GetModeText(LHFileMode mode);



public:
	const char* GetFileName()
	{
		return m_filename;
	}
	UINT GetFileSize()
	{
		return m_filesize;
	}


public:
	size_t Read(void* data, int datalen);

	void ReadText(TCHAR* text);

	void puts(const TCHAR* buf);
	TCHAR* gets(TCHAR* buf, int buf_size);

	void Seek(int offset, int orgin);
	int Tell();
	// int Feof();


public:
	template<typename T>
	size_t Read(T* data)
	{
		return Read( (void*)data, sizeof(T) );
	}

	template<typename T>
	size_t operator >> (T& data)
	{
		return Read(&data);
	}


public:
	LHFile(void);
	~LHFile(void);
};

#define FCLOSE(fp)		{ if(fp) pfclose(fp); fp=NULL; }
