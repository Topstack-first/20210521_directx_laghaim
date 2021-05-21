#include "stdafx.h"
#include "FileManager.h"
#include "Config.h"

#if defined(_MSC_VER)
#if _MSC_VER <= 1400
extern "C"
int __cdecl _input(FILE *, const unsigned char *, va_list);
#else
extern "C"
int __cdecl _input_l(FILE*, unsigned char*, _locale_t, va_list);
#endif
#endif

PFILE_STD::PFILE_STD()
	: m_fp(NULL)
{

}

PFILE_STD::~PFILE_STD()
{
	Close();
}

int	PFILE_STD::Scanf(const char* format, va_list arglist)
{
#if defined(_MSC_VER)
#if _MSC_VER <= 1400
	return (_input(m_fp, reinterpret_cast<unsigned char*>(const_cast<char *>(format) ), arglist) );
#else
	return (_input_l(m_fp, reinterpret_cast<unsigned char*>(const_cast<char *>(format) ), NULL, arglist ) );
#endif
#endif
}

char* PFILE_STD::Gets(char* string, int count)
{
	return fgets(string, count, m_fp);
}

int PFILE_STD::Getc()
{
	return ::getc(m_fp);
}

int	PFILE_STD::Seek(long int offset, int whence)
{
	return fseek(m_fp, offset, whence);
}

PFILE* PFILE_STD::Open(const char* filename, const char* mode)
{
	m_fp = fopen(filename, mode);

	if( m_fp == NULL )
	{
		delete this;
		return NULL;
	}

	return this;
}

void PFILE_STD::Close()
{
	if( m_fp != NULL )
		fclose(m_fp);

	m_fp = NULL;
}

size_t PFILE_STD::Read(void* ptr, size_t size, size_t n)
{
	return fread(ptr, size, n, m_fp);
}

int	PFILE_STD::Tell()
{
	return ftell(m_fp);
}
//////////////////////////////////////////////////////////////////////////
PFILE_NFS::PFILE_NFS()
	: m_vfp(NULL)
{

}

PFILE_NFS::~PFILE_NFS()
{
	Close();
}

int PFILE_NFS::Scanf(const char* format, va_list arglist)
{
	return vfscanf(m_vfp, format, arglist);
}

char* PFILE_NFS::Gets(char* string, int count)
{
	return vfgets(string, count, m_vfp);
}

int	PFILE_NFS::Getc()
{
	return vgetc(m_vfp);
}

int	PFILE_NFS::Seek(long int offset, int whence)
{
	return vfseek(m_vfp, offset, whence);
}

PFILE* PFILE_NFS::Open(const char* filename, const char* mode)
{
	m_vfp = vfopen(filename, mode);

	if( m_vfp == NULL )
	{
		delete this;
		return NULL;
	}

	return this;
}

void PFILE_NFS::Close()
{
	if( m_vfp != NULL )
		vfclose(m_vfp);

	m_vfp = NULL;
}

size_t PFILE_NFS::Read(void* ptr, size_t size, size_t n)
{
	return vfread(ptr, size, n, m_vfp);
}

int PFILE_NFS::Tell()
{
	return vftell(m_vfp);
}

//////////////////////////////////////////////////////////////////////////
FileManager::FileManager()
	: m_eFileType(eFILE_TYPE_NFS)
{
	/*FILE* fp = fopen("pack.cfg", "rt");
	if( fp != NULL )
	{
		int nUnPack = 0;
		fscanf(fp, "%d", &nUnPack);

		if( nUnPack == 1 )
			m_eFileType = eFILE_TYPE_STD;

		fclose(fp);
	}*/
	m_eFileType = eFILE_TYPE_NFS;
}

FileManager::~FileManager()
{

}

int	FileManager::Scanf(PFILE* stream, const char* format, ...)
{
	int ret = 0;
	va_list arglist;
	va_start(arglist, format);
	ret = stream->Scanf(format, arglist);
	va_end(arglist);

	return ret;
}

char* FileManager::Gets(char* string, int count, PFILE* stream)
{
	return stream->Gets(string, count);
}

int FileManager::Getc(PFILE* stream)
{
	return stream->Getc();
}

int FileManager::Seek(PFILE* stream, long int offset, int whence)
{
	return stream->Seek(offset, whence);
}

PFILE* FileManager::Open(const char* filename, const char* mode, bool forceStd)
{
	PFILE* fp = NULL;
	PFILE* fptemp = new PFILE_NFS();
	if( m_eFileType == eFILE_TYPE_NFS && forceStd == false)
	{
		if(!fptemp->Open(filename, mode))
		{
			forceStd = true;
		}
		else
			fptemp->Close();
	}
	
	
	if( m_eFileType == eFILE_TYPE_STD || (forceStd == true) )
		fp = new PFILE_STD();
	else if( m_eFileType == eFILE_TYPE_NFS )
		fp = new PFILE_NFS();

	return fp->Open(filename, mode);
}

void FileManager::Close(PFILE* fp)
{
	if( fp != NULL )
		fp->Close();

	SAFE_DELETE(fp);
}

size_t FileManager::Read(void* ptr, size_t size, size_t n, PFILE* stream)
{
	return stream->Read(ptr, size, n);
}

int FileManager::Tell(PFILE* stream)
{
	return stream->Tell();
}

void FileManager::PackInit()
{
	if( m_eFileType == eFILE_TYPE_NFS )
		vPackInitialize();
}

void FileManager::PackUninit()
{
	if( m_eFileType == eFILE_TYPE_NFS )
		vPackUninitialize();
}
unsigned char cripttable_[] = {
	105, 127, 112, 185,  63, 255,  74, 236,  41, 205, 186, 121, 243, 241, 237,  90, 154, 198,  74, 238,  28, 242, 211, 131, \
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
void FileManager::Decode(unsigned char* stream, int filesize)
{
	int criptcounter = 0;
	for(int j = 0; j < filesize; j++)
	{
		stream[j] = stream[j] ^ cripttable_[criptcounter];
		criptcounter++;
		criptcounter = criptcounter % 256;
	}
}