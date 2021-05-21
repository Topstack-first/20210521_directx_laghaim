#ifndef _VIRTUALFILE_H_
#define _VIRTUALFILE_H_

enum VF_ERROR_CODE
{
	VF_ERR_NONE = 0,
	VF_ERR_NULL_STRING = -1,
	VF_ERR_NULL_PT	= -2,
};

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_READ_BUFFER( dest, src, size )			{ memcpy( dest, src, size ); src += size; }
#define vgetc(_stream)     ( 0xff & *(_stream)->_ptr++ )

#include <memory.h>

#include "nfs.h"
#include "singleton.h"

typedef struct _VFiobuf
{
	char* _ptr;
	char* _base;
	long  _cnt;
	long  _basesize;
	bool  _expansion;
} VFILE;

class NfsVirtualFile;
class CVirtualFile : public CSingleton<CVirtualFile>
{
	enum FILETYPE
	{
		BASE = 0,
		EX,
		PATCH,
	};

public:
	CVirtualFile();
	virtual ~CVirtualFile();

	BOOL Init();
	BOOL Open(char *fileName);
	BOOL OpenFile( const char *fileName);
	void CloseFile();						// close glob file.

	BOOL SearchFile( const char* fileName );		// it's can folder name search.
	int	 HowManyMatchFile( const char* desc, FILETYPE &_type );	// it's can part of name search. return to file count.
	void Read( char** buf, long& size );

	NfsVirtualFile* m_pVF_Base;
	NfsVirtualFile*	m_pVF_Ex;
	NfsVirtualFile* m_pVF_Patch;
	nfs_glob_t*		m_pNfsGlob_Base;
	nfs_glob_t*		m_pNfsGlob_Ex;
	nfs_glob_t*		m_pNfsGlob_Patch;
	bool			m_bIsBaseOpen;
	bool			m_bIsExpansionOpen;
	bool			m_bIsPatchOpen;
	int				m_iCurPos;
	int				m_iCurPos_Ex;
	int				m_iCurPos_Patch;
};

char*			changeDeleminate( char* szSrc, char* szDst );
int				vPackInitialize();
void			vPackUninitialize();
VFILE*			vfopen( const char* filename, const char* mode );
void			vfclose( VFILE* fp );
size_t			vfread( void* ptr, size_t size, size_t n, VFILE* stream  );
char*			vfgets( char* string, int count, VFILE* str );
int				__cdecl vfscanf( VFILE* stream, const char *format, ... );
int				vfscanf( VFILE* stream, const char* format, va_list arglist );
int				vfseek( VFILE *stream, long int offset, int whence );
bool			vfexist( char* szFileName );
int				vftell( VFILE* stream );

#endif