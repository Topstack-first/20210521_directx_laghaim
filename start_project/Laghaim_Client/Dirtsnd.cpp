#include "stdafx.h"
/////////////////////////////////////////
//        ���̷�Ʈ ���� Ŭ����
/////////////////////////////////////////

#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include <assert.h>
#include <d3d.h>
#include <math.h>
#include <dxerr9.h>
#include "D3DMath.h"
#include "D3DUtil.h"
#include "DirtSnd.h"
#include <VirtualFile.h>

#pragma comment( lib, "dxerr9.lib" )

#define D3D_OVERLOADS

CDirectSound * g_pDSound = NULL;

extern BOOL g_bUse3DSound;


// definition wave file class body. [10/6/2009 rumist]
CWaveFile::CWaveFile()
	:	m_pwfx( NULL )
	,	m_hmmio( NULL )
	,	m_presourceBuffer( NULL )
	,	m_dwSize( 0 )
	,	m_bIsReadingFromMemory( FALSE )
	, m_dwFlags(0)
	, m_pbData(NULL)
	, m_pbDataCur(NULL)
	, m_ulDataSize(0)
{
	return;
}

CWaveFile::~CWaveFile()
{
	Close();
	if( !m_bIsReadingFromMemory )
		SAFE_DELETE_ARRAY( m_pwfx );
	return;
}

HRESULT			CWaveFile::ReadMMIO()
{
	MMCKINFO			ckIn;
	PCMWAVEFORMAT		pcmWaveFormat;

	ZeroMemory( &ckIn, sizeof( MMCKINFO ) );
	m_pwfx = NULL;

	if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) ) )
		return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );

	// Check to make sure this is a valid wave file.
	if( m_ckRiff.ckid != FOURCC_RIFF || ( m_ckRiff.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) )
		return DXTRACE_ERR( TEXT("mmioFOURCC"), E_FAIL );

	// Search the input file for the 'fmt' chunk.
	ckIn.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
	if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
		return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );

	if( ckIn.cksize < (LONG) sizeof( PCMWAVEFORMAT ) )
		return DXTRACE_ERR( TEXT("sizeof( PCMWAVEFORMAT )"), E_FAIL );

	// Read the 'fmt' chunk into <pcmWaveFormat>
	if( mmioRead( m_hmmio, (HPSTR) &pcmWaveFormat, sizeof( pcmWaveFormat) ) != sizeof( pcmWaveFormat ) )
		return DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );

	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
		if( NULL == m_pwfx )
			return DXTRACE_ERR( TEXT("m_pwfx"), E_FAIL );

		// Copy the bytes from the pcm structure to the waveformatex structure.
		memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		m_pwfx->cbSize = 0;
	}
	else
	{
		// Read in length of extra bytes.
		WORD cbExtraBytes = 0L;
		if( mmioRead( m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD) ) != sizeof(WORD) )
			return DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );

		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];
		if( NULL == m_pwfx )
			return DXTRACE_ERR( TEXT("new"), E_FAIL );

		// Copy the bytes from the pcm structure to the waveformatex structure.
		memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		m_pwfx->cbSize = cbExtraBytes;

		// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
		if( mmioRead( m_hmmio, (CHAR*)( ((CHAR*)&(m_pwfx->cbSize))+sizeof(WORD) ), cbExtraBytes ) != cbExtraBytes )
		{
			SAFE_DELETE( m_pwfx );
			return DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );
		}
	}

	// Ascend the input file out of the 'fmt' chunk.
	if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
	{
		SAFE_DELETE( m_pwfx );
		return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
	}

	return S_OK;
}

HRESULT			CWaveFile::WriteMMIO(WAVEFORMATEX* pwfxDest )
{
	DWORD    dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
	MMCKINFO ckOut1;

	dwFactChunk = (DWORD)-1;

	// Create the output file RIFF chunk of form type 'WAVE'.
	m_ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	m_ckRiff.cksize = 0;

	if( 0 != mmioCreateChunk( m_hmmio, &m_ckRiff, MMIO_CREATERIFF ) )
		return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );

	// We are now descended into the 'RIFF' chunk we just created.
	// Now create the 'fmt ' chunk. Since we know the size of this chunk,
	// specify it in the MMCKINFO structure so MMIO doesn't have to seek
	// back and set the chunk size after ascending from the chunk.
	m_ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
	m_ck.cksize = sizeof(PCMWAVEFORMAT);

	if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
		return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );

	// Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type.
	if( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
	{
		if( mmioWrite( m_hmmio, (HPSTR) pwfxDest,
					   sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
			return DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
	}
	else
	{
		// Write the variable length size.
		if( (UINT)mmioWrite( m_hmmio, (HPSTR) pwfxDest,
							 sizeof(*pwfxDest) + pwfxDest->cbSize ) !=
				( sizeof(*pwfxDest) + pwfxDest->cbSize ) )
			return DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
	}

	// Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
	if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
		return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );

	// Now create the fact chunk, not required for PCM but nice to have.  This is filled
	// in when the close routine is called.
	ckOut1.ckid = mmioFOURCC('f', 'a', 'c', 't');
	ckOut1.cksize = 0;

	if( 0 != mmioCreateChunk( m_hmmio, &ckOut1, 0 ) )
		return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );

	if( mmioWrite( m_hmmio, (HPSTR)&dwFactChunk, sizeof(dwFactChunk)) !=
			sizeof(dwFactChunk) )
		return DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );

	// Now ascend out of the fact chunk...
	if( 0 != mmioAscend( m_hmmio, &ckOut1, 0 ) )
		return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );

	return S_OK;
}

HRESULT			CWaveFile::Open(LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags )
{
	HRESULT			hr;
	m_dwFlags = dwFlags;
	m_bIsReadingFromMemory = FALSE;

	if( m_dwFlags == WAVEFILE_READ )
	{
		if( strFileName == NULL )
			return E_INVALIDARG;

		SAFE_DELETE_ARRAY( m_pwfx );

		// open wave file header.
		m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );

		// open resource file.
		if( NULL == m_hmmio )
		{
			HRSRC		hResInfo;
			HGLOBAL		hResData;
			DWORD		dwSize;
			VOID*		pvRes;

			// Loading it as a file failed, so try it as a resource.
			if( NULL == ( hResInfo = FindResource( NULL, strFileName, TEXT("WAVE") ) ) )
			{
				if( NULL == (hResInfo = FindResource( NULL, strFileName, TEXT("WAV") ) ) )
					return DXTRACE_ERR( TEXT("FindResource"), E_FAIL );
			}

			if( NULL == ( hResData = LoadResource( NULL, hResInfo ) ) )
				return DXTRACE_ERR( TEXT( "LoadResurce" ), E_FAIL );

			if( NULL == ( dwSize = SizeofResource( NULL, hResInfo ) ) )
				return DXTRACE_ERR( TEXT("SizeofResource"), E_FAIL );

			if( NULL == ( pvRes = LockResource( hResData ) ) )
				return DXTRACE_ERR( TEXT("LockResource"), E_FAIL );

			m_presourceBuffer = new CHAR[dwSize];

			memcpy( m_presourceBuffer, pvRes, dwSize );

			MMIOINFO mmioInfo;
			ZeroMemory( &mmioInfo, sizeof( MMIOINFO ) );
			mmioInfo.fccIOProc = FOURCC_MEM;
			mmioInfo.cchBuffer = dwSize;
			mmioInfo.pchBuffer = (CHAR*) m_presourceBuffer;

			m_hmmio = mmioOpen( NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
		}

		if( FAILED( hr = ReadMMIO() ) )
		{
			// ReadMMIO will fail if it's an not a wave file.
			mmioClose( m_hmmio, 0 );
			return DXTRACE_ERR( TEXT("ReadMMIO"), hr );
		}

		if( FAILED( hr = ResetFile() ) )
			return DXTRACE_ERR( TEXT("ResetFile"), hr );

		// After the reset, the size of the wave file is m_ck.cksize so store it now.
		m_dwSize = m_ck.cksize;
	}
	else
	{
		m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF | MMIO_READWRITE | MMIO_CREATE );
		if( NULL == m_hmmio )
			return DXTRACE_ERR( TEXT("mmioOpen"), E_FAIL );

		if( FAILED( hr =  WriteMMIO( pwfx ) ) )
		{
			mmioClose( m_hmmio, 0 );
			return DXTRACE_ERR( TEXT("WriteMMIO"), hr );
		}
		if( FAILED( hr = ResetFile() ) )
			return DXTRACE_ERR( TEXT("ResetFile"), hr );
	}

	return hr;
}

HRESULT			CWaveFile::OpenFromVFile(LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags )
{
	HRESULT			hr;
	m_dwFlags = dwFlags;
	m_bIsReadingFromMemory = FALSE;
	VFILE* fp = NULL;

	if( m_dwFlags == WAVEFILE_READ )
	{
		if( strFileName == NULL )
			return E_INVALIDARG;

		SAFE_DELETE_ARRAY( m_pwfx );

		// open wave file header.
		m_hmmio = NULL;//mmioOpen( strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );

		// open resource file.
		if( NULL == m_hmmio )
		{
			DWORD		dwSize;

			fp = vfopen( strFileName, "rb" );

			if( NULL == fp )
				return DXTRACE_ERR( TEXT("vfopen"), E_FAIL );

			if( 0 == fp->_basesize )
				return DXTRACE_ERR( TEXT("file size is zero."), E_FAIL );

			dwSize = fp->_basesize;

			m_presourceBuffer = new CHAR[fp->_basesize];
			if( NULL == m_presourceBuffer )
				return DXTRACE_ERR( TEXT("OutOfMemory"), E_FAIL );

			memcpy( m_presourceBuffer, fp->_base, fp->_basesize );

			// close virtual files.
			vfclose( fp );

			MMIOINFO mmioInfo;
			ZeroMemory( &mmioInfo, sizeof( MMIOINFO ) );
			mmioInfo.fccIOProc = FOURCC_MEM;
			mmioInfo.cchBuffer = dwSize;
			mmioInfo.pchBuffer = (CHAR*) m_presourceBuffer;

			m_hmmio = mmioOpen( NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
		}

		if( FAILED( hr = ReadMMIO() ) )
		{
			// ReadMMIO will fail if it's an not a wave file.
			mmioClose( m_hmmio, 0 );
			return DXTRACE_ERR( TEXT("ReadMMIO"), hr );
		}

		if( FAILED( hr = ResetFile() ) )
			return DXTRACE_ERR( TEXT("ResetFile"), hr );

		// After the reset, the size of the wave file is m_ck.cksize so store it now.
		m_dwSize = m_ck.cksize;
	}
	else
	{
		m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF | MMIO_READWRITE | MMIO_CREATE );
		if( NULL == m_hmmio )
			return DXTRACE_ERR( TEXT("mmioOpen"), E_FAIL );

		if( FAILED( hr =  WriteMMIO( pwfx ) ) )
		{
			mmioClose( m_hmmio, 0 );
			return DXTRACE_ERR( TEXT("WriteMMIO"), hr );
		}
		if( FAILED( hr = ResetFile() ) )
			return DXTRACE_ERR( TEXT("ResetFile"), hr );
	}

	return hr;
}

HRESULT			CWaveFile::OpenFromMemory(BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags )
{
	m_pwfx			= pwfx;
	m_ulDataSize	= ulDataSize;
	m_pbData		= pbData;
	m_pbDataCur		= m_pbData;
	m_bIsReadingFromMemory = TRUE;

	if( dwFlags != WAVEFILE_READ )
		return E_NOTIMPL;

	return S_OK;
}

HRESULT			CWaveFile::Close()
{
	if( m_dwFlags == WAVEFILE_READ )
	{
		mmioClose( m_hmmio, 0 );
		m_hmmio = NULL;
		SAFE_DELETE_ARRAY( m_presourceBuffer );
	}
	else
	{
		m_mmioinfoOut.dwFlags |= MMIO_DIRTY;

		if( m_hmmio == NULL )
			return CO_E_NOTINITIALIZED;

		if( 0 != mmioSetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
			return DXTRACE_ERR( TEXT("mmioSetInfo"), E_FAIL );

		if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
			return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );

		if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
			return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );

		mmioSeek( m_hmmio, 0, SEEK_SET );

		if( 0 != (INT)mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) )
			return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );

		m_ck.ckid = mmioFOURCC( 'f', 'a', 'c', 't' );
		if( 0 == mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
		{
			DWORD dwSamples = 0;
			mmioWrite( m_hmmio, (HPSTR)&dwSamples, sizeof(DWORD) );
			mmioAscend( m_hmmio, &m_ck, 0 );
		}

		if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
			return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );

		mmioClose( m_hmmio, 0 );
		m_hmmio = NULL;
	}

	return S_OK;
}

HRESULT			CWaveFile::Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
	if( m_bIsReadingFromMemory )
	{
		if( m_pbDataCur == NULL )
			return CO_E_NOTINITIALIZED;

		if( pdwSizeRead != NULL )
			*pdwSizeRead = 0;

		if( (BYTE*)(m_pbDataCur + dwSizeToRead ) > (BYTE*)( m_pbData + m_ulDataSize ) )
		{
			dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
		}

		CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );

		if( pdwSizeRead != NULL )
			*pdwSizeRead = dwSizeToRead;

		return S_OK;
	}
	else
	{
		MMIOINFO mmioinfoIn;		// current status of m_hmmio

		if( m_hmmio == NULL )
			return CO_E_NOTINITIALIZED;

		if( pBuffer == NULL || pdwSizeRead == NULL )
			return E_INVALIDARG;

		if( pdwSizeRead != NULL )
			*pdwSizeRead = 0;

		if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
			return DXTRACE_ERR( TEXT("mmioGetInfo"), E_FAIL );

		UINT cbDataIn = dwSizeToRead;
		if( cbDataIn > m_ck.cksize )
			cbDataIn = m_ck.cksize;

		m_ck.cksize -= cbDataIn;

		for( DWORD cT = 0; cT < cbDataIn; ++cT )
		{
			// Copy the bytes from the io to the buffer.
			if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
			{
				if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
					return DXTRACE_ERR( TEXT("mmioAdvance"), E_FAIL );

				if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
					return DXTRACE_ERR( TEXT("mmioinfoIn.pchNext"), E_FAIL );
			}

			// actual copy.
			*((BYTE*)pBuffer+cT) = *((BYTE*)mmioinfoIn.pchNext);
			mmioinfoIn.pchNext++;
		}

		if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
			return DXTRACE_ERR( TEXT("mmioSetInfo"), E_FAIL );

		if( pdwSizeRead != NULL )
			*pdwSizeRead = cbDataIn;

		return S_OK;
	}
}

HRESULT			CWaveFile::Write(UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote )
{
	UINT cT;

	if( m_bIsReadingFromMemory )
		return E_NOTIMPL;
	if( m_hmmio == NULL )
		return CO_E_NOTINITIALIZED;
	if( pnSizeWrote == NULL || pbSrcData == NULL )
		return E_INVALIDARG;

	*pnSizeWrote = 0;

	for( cT = 0; cT < nSizeToWrite; cT++ )
	{
		if( m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite )
		{
			m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
			if( 0 != mmioAdvance( m_hmmio, &m_mmioinfoOut, MMIO_WRITE ) )
				return DXTRACE_ERR( TEXT("mmioAdvance"), E_FAIL );
		}

		*((BYTE*)m_mmioinfoOut.pchNext) = *((BYTE*)pbSrcData+cT);
		(BYTE*)m_mmioinfoOut.pchNext++;

		(*pnSizeWrote)++;
	}

	return S_OK;
}

DWORD			CWaveFile::GetSize()
{
	return m_dwSize;
}

HRESULT			CWaveFile::ResetFile()
{
	if( m_bIsReadingFromMemory )
	{
		m_pbDataCur = m_pbData;
	}
	else
	{
		if( m_hmmio == NULL )
			return CO_E_NOTINITIALIZED;

		if( m_dwFlags == WAVEFILE_READ )
		{
			// Seek to the data.
			if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof( FOURCC ), SEEK_SET ) )
				return DXTRACE_ERR( TEXT("mmioSeek"), E_FAIL );

			// Search the input file for the 'data' chunk.
			m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
			if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
				return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );
		}
		else
		{
			// Create the 'data' chunk that holds the waveform samples.
			m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
			m_ck.cksize = 0;

			if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
				return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );

			if( 0 != mmioGetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
				return DXTRACE_ERR( TEXT("mmioGetInfo"), E_FAIL );
		}
	}

	return S_OK;
}

CDirectSound::CDirectSound(HWND hWindow)
	: m_fDir(0)
	, m_CurVolume(0)
	, m_WavFile(NULL)
	, m_iRefCount(0)
{
	memset(m_WaveFilePath, 0, sizeof(m_WaveFilePath));

	m_iBufCount= 0;
	m_iCurBufIdx = 0;
	m_hWnd       = hWindow;
	m_pDS3DListener = NULL;
	ClearMapSC();

	m_fMinDistance = 2.0f;
	m_fMaxDistance = 150.0f;
	m_bIsUse = TRUE;
}

CDirectSound::~CDirectSound()
{
	if (m_lpDS)
		DSObjectRelease();
}

//new functions
BOOL CDirectSound::InitDirectSound()
{
	DSBUFFERDESC        dsBD;
	static LPDIRECTSOUNDBUFFER  pDirectSoundPrimaryBuffer = NULL;

	// ���̷�Ʈ ���� ��ü����
	HRESULT dsrval = DirectSoundCreate8(NULL, &m_lpDS, NULL);

	if (dsrval != DS_OK)
	{
		m_lpDS = NULL;
		return FALSE;
	}
	// ���̷�Ʈ ���� ��� ȯ���� �����Ѵ�.
	dsrval = m_lpDS->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY );
	if (dsrval != DS_OK)
		DirectSoundError("��� ���� ����");

	ZeroMemory( &dsBD, sizeof(DSBUFFERDESC));
	dsBD.dwSize = sizeof(DSBUFFERDESC);
	//dsBD.dwBufferBytes = 0;  //must be zero for primary buffer..
	if (g_bUse3DSound)
		dsBD.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	else
		dsBD.dwFlags = DSBCAPS_PRIMARYBUFFER;

	if (m_lpDS->CreateSoundBuffer(&dsBD, &pDirectSoundPrimaryBuffer, NULL) != DS_OK)
	{
		DirectSoundError("1�� ���� ���� ���� ����");
		return FALSE;
	}

	// Set primary buffer format to 22kHz and 16-bit output.
	WAVEFORMATEX wfx;
	ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
	wfx.wFormatTag      = WAVE_FORMAT_PCM;
	wfx.nChannels       = 2;
	wfx.nSamplesPerSec  = 44100;

	wfx.wBitsPerSample  = 16;
	wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if( FAILED( pDirectSoundPrimaryBuffer->SetFormat(&wfx) ) )
		return FALSE;

	// û���� �������̽� ���
	if (g_bUse3DSound)
	{
		if(FAILED( pDirectSoundPrimaryBuffer->QueryInterface(
					   IID_IDirectSound3DListener, (void**)&m_pDS3DListener)) )
		{
			DirectSoundError("1�� ���۷κ��� 3DListener �������̽� ��� ����");
			return FALSE;
		}

		// û���� �������̽��� ����Ʈ �� ��������.
		m_ListenerParams.dwSize = sizeof(DS3DLISTENER);
		m_pDS3DListener->GetAllParameters (&m_ListenerParams);
	}

	if(pDirectSoundPrimaryBuffer)
	{
		pDirectSoundPrimaryBuffer->Release();
		pDirectSoundPrimaryBuffer = NULL;
	}

	return TRUE;

}
//end new functions


BOOL CDirectSound::DirectSoundInit()
{
	DSBUFFERDESC dsBD = {0};
	LPDIRECTSOUNDBUFFER  pDirectSoundPrimaryBuffer = NULL;

	// ���̷�Ʈ ���� ��ü����
	HRESULT dsrval = ::DirectSoundCreate8(NULL, &m_lpDS, NULL);

	if (dsrval != DS_OK)
	{
		m_lpDS = NULL;
		return FALSE;
	}
	// ���̷�Ʈ ���� ��� ȯ���� �����Ѵ�.
	dsrval = m_lpDS->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY );
	if (dsrval != DS_OK)
		DirectSoundError("��� ���� ����");


	// 1�� ���� ���� ����
	ZeroMemory( &dsBD, sizeof(DSBUFFERDESC));
	dsBD.dwSize = sizeof(DSBUFFERDESC);
	dsBD.dwBufferBytes = 0;  //must be zero for primary buffer..
	if (g_bUse3DSound)
		dsBD.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	else
		dsBD.dwFlags = DSBCAPS_PRIMARYBUFFER;

	if (m_lpDS->CreateSoundBuffer(&dsBD, &pDirectSoundPrimaryBuffer, NULL) != DS_OK)
	{
		DirectSoundError("1�� ���� ���� ���� ����");
		return FALSE;
	}

	// Set primary buffer format to 22kHz and 16-bit output.
	WAVEFORMATEX wfx;
	ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
	wfx.wFormatTag      = WAVE_FORMAT_PCM;
	wfx.nChannels       = 2;
	wfx.nSamplesPerSec  = 44100;
	wfx.wBitsPerSample  = 16;
	wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if( FAILED( pDirectSoundPrimaryBuffer->SetFormat(&wfx) ) )
		return FALSE;

	// û���� �������̽� ���
	if (g_bUse3DSound)
	{
		if(DS_OK != pDirectSoundPrimaryBuffer->QueryInterface(
					IID_IDirectSound3DListener, (void**)&m_pDS3DListener))
		{
			DirectSoundError("1�� ���۷κ��� 3DListener �������̽� ��� ����");
			return FALSE;
		}

		// û���� �������̽��� ����Ʈ �� ��������.
		m_ListenerParams.dwSize = sizeof(DS3DLISTENER);
		m_pDS3DListener->GetAllParameters (&m_ListenerParams);
	}

	if(pDirectSoundPrimaryBuffer)
	{
		pDirectSoundPrimaryBuffer->Release();
		pDirectSoundPrimaryBuffer = NULL;
	}

	return TRUE;
}

void CDirectSound::DirectSoundError(char* Message)
{
	if (m_lpDS)
		DSObjectRelease();

}

void CDirectSound::DSObjectRelease()
{
	StopAll();

	for( mapSC::iterator itr = m_mapSC.begin(); itr != m_mapSC.end(); ++itr )
	{
		SAFE_RELEASE( (*itr).second->lpDSBuf );
		SAFE_RELEASE( (*itr).second->lpDS3DBuf );
		SAFE_RELEASE( (*itr).second->lpDSBufDup );
		SAFE_RELEASE( (*itr).second->lpDS3DBufDup );
		SAFE_DELETE( (*itr).second );
	}
	ClearMapSC();

	if(m_pDS3DListener)
	{
		m_pDS3DListener->Release();
		m_pDS3DListener = NULL;
	}

	if (m_lpDS)
		m_lpDS->Release();
}

// ���� �Ҹ� ȭ���� ������� �����Ѵ�.
void CDirectSound::SetWavSize(int num)
{
	if(num<0)
	{
		m_iBufCount = 0;
		return;
	}

	m_iBufCount = num;
}

// ���̺� ȭ���� �ִ� ���丮 ��θ� �����Ѵ�. �����丮.
void CDirectSound::SetWaveFilePath(char *path)
{
//	strcpy(m_WaveFilePath, path);
	m_szWaveFilePath = path;
}


int CDirectSound::LoadWAV(const char* wav)
{
	return LoadWav(wav);
}

// wavȭ���� �о directsound buffer�� ���� �ִ´�.
int CDirectSound::LoadWav(const char* wav)
{
	// ���� �˻縦 �Ѵ�.
	mapSC_Indexer::iterator itFind = m_mapSC_Indexer.find(wav);
	if( itFind != m_mapSC_Indexer.end() )
	{
		return (*itFind).second;
	}

	DSBUFFERDESC    dsbd;

	if (m_lpDS == NULL)
	{
		// DirectSound�� �ʱ�ȭ ���� �ʾҴ�.
		return -1;
	}

	// ȭ���� ���� ���� ���̺�ȭ���� �ִ� �н��� �����Ų��.
	std::string path_name;
	path_name = m_szWaveFilePath;
	path_name += wav;

	CWaveFile* pWaveFile = new CWaveFile();

	if( pWaveFile == NULL )
	{
		return -1;
	}

	pWaveFile->Open( (LPSTR)(path_name.c_str()), NULL, WAVEFILE_READ );

	if( pWaveFile->GetSize() == 0 )
	{
		delete pWaveFile;

		return -1;
	}

	DWORD dwDSBufferSize = pWaveFile->GetSize();
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	if (g_bUse3DSound)
		dsbd.dwFlags =  DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME;
	else
		dsbd.dwFlags =  DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = dwDSBufferSize;
	dsbd.lpwfxFormat = pWaveFile->m_pwfx;

	// �˰��� ���úκ� ���� ��..
	dsbd.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	if (g_bUse3DSound)
		dsbd.guid3DAlgorithm = DS3DALG_HRTF_FULL ;

	// 2�� ���� ���۸� �����Ѵ�.
	// 3D sound �� ��� ���Ÿ�Ը� ����.
	SOUNDCHUNK* pChunk = new SOUNDCHUNK;

	if( pChunk == NULL )
	{
		DirectSoundError("���� ûũ ���� ����");
		delete pWaveFile;
		return -1;
	}

	HRESULT hr = 0;
//	LPDIRECTSOUNDBUFFER lpTempDSBuf;
	if (FAILED(hr = m_lpDS->CreateSoundBuffer(&dsbd, &pChunk->lpDSBuf,NULL) ))
	{
		DirectSoundError("������� ��������");
		delete pWaveFile;
		SAFE_DELETE( pChunk );
		return -1;
	}

	VOID* pDSLockedBuffer = NULL;
	DWORD	dwDSLockedBufferSize = 0;
	DWORD	dwWaveDataRead = 0;
	if( FAILED(hr = pChunk->lpDSBuf->Lock( 0, dwDSBufferSize, &pDSLockedBuffer, &dwDSLockedBufferSize, NULL, NULL, 0 ) ) )
	{
		DXTRACE_ERR( TEXT("LOCK"), hr );
		delete pWaveFile;
		SAFE_DELETE( pChunk );
		return -1;
	}

	pWaveFile->ResetFile();

	if( FAILED( hr = pWaveFile->Read( (BYTE*)pDSLockedBuffer, dwDSLockedBufferSize, &dwWaveDataRead ) ) )
	{
		DXTRACE_ERR( TEXT("Read" ), hr );
		delete pWaveFile;
		SAFE_DELETE( pChunk );
		return -1;
	}

	if( dwWaveDataRead == 0 )
		FillMemory( (BYTE*) pDSLockedBuffer, dwDSLockedBufferSize, (BYTE)(pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
	else if( dwWaveDataRead < dwDSLockedBufferSize )
		FillMemory( (BYTE*)pDSLockedBuffer + dwWaveDataRead, dwDSLockedBufferSize - dwWaveDataRead, (BYTE) ( pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );

	pChunk->lpDSBuf->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

	delete pWaveFile;

	// Get the 3D buffer from the secondary buffer

	LPDIRECTSOUND3DBUFFER lpTempDS3DBuf = NULL;
	if (g_bUse3DSound)
	{
		if( pChunk->lpDSBuf->QueryInterface( IID_IDirectSound3DBuffer, (VOID**)&pChunk->lpDS3DBuf ) !=DS_OK )
		{
			DirectSoundError("2�� ���� ���۷κ��� 3DBuffer ���� ����");
		}
		DS3DBUFFER Ds3DBuffer;

		ZeroMemory(&Ds3DBuffer, sizeof(DS3DBUFFER));
		Ds3DBuffer.dwSize = sizeof(DS3DBUFFER);
		pChunk->lpDS3DBuf->GetAllParameters(&Ds3DBuffer);
		Ds3DBuffer.dwMode = DS3DMODE_HEADRELATIVE;
		Ds3DBuffer.flMinDistance = m_fMinDistance;
		Ds3DBuffer.flMaxDistance = m_fMaxDistance;

		pChunk->lpDS3DBuf->SetAllParameters(&Ds3DBuffer, DS3D_IMMEDIATE);
	}

	pChunk->szWaveFileName = wav;
	// ������ ȭ�� ���� �ε����� �����Ѵ�.

	// �����߿� �ε��Ǵ� ������ wav������ ������ config.ini������ �������� ������ ������Ų��
	// ���� ����ũ��� 0 -> -10000ũ���̴�..
	// �׷��� ���� ������..-5000�������� ���ʹ� ���ǼҸ��� ���� �鸮�� �ʾҴ�..�ؼ�
	// ���� �������� (-5000 ~ 0)������ �� �����ߴ�.
	pChunk->lpDSBuf->SetVolume((-5000 )+ (50*m_CurVolume));
	if(m_CurVolume < 10)
	{
		pChunk->lpDSBuf->SetVolume(-500000 );
	}

	InsertMapSC(m_iCurBufIdx, pChunk, wav);

	return m_iCurBufIdx++;
}

// wavȭ���� �о directsound buffer�� ���� �ִ´�.
int CDirectSound::LoadWavFromVFile(const char* wav)
{
	// ���� �˻縦 �Ѵ�.
	mapSC_Indexer::iterator itFind = m_mapSC_Indexer.find(wav);
	if( itFind != m_mapSC_Indexer.end() )
	{
		return (*itFind).second;
	}

	DSBUFFERDESC    dsbd;

	if (m_lpDS == NULL)
	{
		// DirectSound�� �ʱ�ȭ ���� �ʾҴ�.
		return -1;
	}

	// ȭ���� ���� ���� ���̺�ȭ���� �ִ� �н��� �����Ų��.
	std::string path_name;
	path_name = m_szWaveFilePath;
	path_name += wav;

	CWaveFile* pWaveFile = new CWaveFile();

	if( pWaveFile == NULL )
	{
		return -1;
	}

	pWaveFile->OpenFromVFile( (LPSTR)(path_name.c_str()), NULL, WAVEFILE_READ );

	if( pWaveFile->GetSize() == 0 )
	{
		return -1;
	}

	DWORD dwDSBufferSize = pWaveFile->GetSize();
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	if (g_bUse3DSound)
		dsbd.dwFlags =  DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME;
	else
		dsbd.dwFlags =  DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = dwDSBufferSize;
	dsbd.lpwfxFormat = pWaveFile->m_pwfx;

	// �˰��� ���úκ� ���� ��..
	dsbd.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	if (g_bUse3DSound)
		dsbd.guid3DAlgorithm = DS3DALG_HRTF_FULL ;

	// 2�� ���� ���۸� �����Ѵ�.
	// 3D sound �� ��� ���Ÿ�Ը� ����.
	SOUNDCHUNK* pChunk = new SOUNDCHUNK;

	if( pChunk == NULL )
	{
		DirectSoundError("���� ûũ ���� ����");
		delete pWaveFile;
		return -1;
	}

	HRESULT hr = 0;
	//	LPDIRECTSOUNDBUFFER lpTempDSBuf;
	if (FAILED(hr = m_lpDS->CreateSoundBuffer(&dsbd, &pChunk->lpDSBuf,NULL) ))
	{
		DirectSoundError("������� ��������");
		delete pWaveFile;
		SAFE_DELETE( pChunk );
		return -1;
	}

	VOID* pDSLockedBuffer = NULL;
	DWORD	dwDSLockedBufferSize = 0;
	DWORD	dwWaveDataRead = 0;
	if( FAILED(hr = pChunk->lpDSBuf->Lock( 0, dwDSBufferSize, &pDSLockedBuffer, &dwDSLockedBufferSize, NULL, NULL, 0 ) ) )
	{
		DXTRACE_ERR( TEXT("LOCK"), hr );
		delete pWaveFile;
		SAFE_DELETE( pChunk );
		return -1;
	}

	pWaveFile->ResetFile();

	if( FAILED( hr = pWaveFile->Read( (BYTE*)pDSLockedBuffer, dwDSLockedBufferSize, &dwWaveDataRead ) ) )
	{
		DXTRACE_ERR( TEXT("Read" ), hr );
		delete pWaveFile;
		SAFE_DELETE( pChunk );
		return -1;
	}

	if( dwWaveDataRead == 0 )
		FillMemory( (BYTE*) pDSLockedBuffer, dwDSLockedBufferSize, (BYTE)(pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
	else if( dwWaveDataRead < dwDSLockedBufferSize )
		FillMemory( (BYTE*)pDSLockedBuffer + dwWaveDataRead, dwDSLockedBufferSize - dwWaveDataRead, (BYTE) ( pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );

	pChunk->lpDSBuf->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

	delete pWaveFile;

	// Get the 3D buffer from the secondary buffer

	LPDIRECTSOUND3DBUFFER lpTempDS3DBuf = NULL;
	if (g_bUse3DSound)
	{
		if( pChunk->lpDSBuf->QueryInterface( IID_IDirectSound3DBuffer, (VOID**)&pChunk->lpDS3DBuf ) !=DS_OK )
		{
			DirectSoundError("2�� ���� ���۷κ��� 3DBuffer ���� ����");
		}
		DS3DBUFFER Ds3DBuffer;

		ZeroMemory(&Ds3DBuffer, sizeof(DS3DBUFFER));
		Ds3DBuffer.dwSize = sizeof(DS3DBUFFER);
		pChunk->lpDS3DBuf->GetAllParameters(&Ds3DBuffer);
		Ds3DBuffer.dwMode = DS3DMODE_HEADRELATIVE;
		Ds3DBuffer.flMinDistance = m_fMinDistance;
		Ds3DBuffer.flMaxDistance = m_fMaxDistance;

		pChunk->lpDS3DBuf->SetAllParameters(&Ds3DBuffer, DS3D_IMMEDIATE);
	}

	pChunk->szWaveFileName = wav;
	// ������ ȭ�� ���� �ε����� �����Ѵ�.

	// �����߿� �ε��Ǵ� ������ wav������ ������ config.ini������ �������� ������ ������Ų��
	// ���� ����ũ��� 0 -> -10000ũ���̴�..
	// �׷��� ���� ������..-5000�������� ���ʹ� ���ǼҸ��� ���� �鸮�� �ʾҴ�..�ؼ�
	// ���� �������� (-5000 ~ 0)������ �� �����ߴ�.
	pChunk->lpDSBuf->SetVolume((-5000 )+ (50*m_CurVolume));
	if(m_CurVolume < 10)
	{
		pChunk->lpDSBuf->SetVolume(-500000 );
	}

	InsertMapSC(m_iCurBufIdx, pChunk, wav);

	return m_iCurBufIdx++;
}

void CDirectSound::SetAllVolume()
{
	mapSC::iterator itEnd = m_mapSC.end();
	for(int i = 0; i < m_iCurBufIdx; i++)
	{
		if( m_mapSC.find( i ) == itEnd )
			continue;

		m_mapSC[i]->lpDSBuf->SetVolume((-5000 )+ (50*m_CurVolume));

		if(m_CurVolume < 10)
		{
			m_mapSC[i]->lpDSBuf->SetVolume(-500000 );
		}
	}
}

void CDirectSound::GetVolFromConfigFile(bool bUse, long level)
{
	m_CurVolume = level;
	m_bIsUse    = bUse;
}
////////

BOOL CDirectSound::IsBufIndex(int index)
{
	if( m_mapSC.find(index) == m_mapSC.end() )
		return FALSE;

	if (index >= 0 && index < m_iCurBufIdx)
		return TRUE;
	else
		return FALSE;
}

void CDirectSound::Play(int index, float x, float y, float z, BOOL bDup)
{
	if (m_lpDS == NULL || ! m_bIsUse)
		return;

	if (IsBufIndex(index))
	{

		if (IsPlaying(index))
		{
			PlayDup(index, x, y, z);

		}
		else if (bDup)
		{
			D3DVECTOR vPos;

			vPos.x = x;
			vPos.y = y;
			vPos.z = z;
			D3DMath_VectorMatrixMultiply(vPos, vPos, m_matRotateY);

			if (g_bUse3DSound)
				m_mapSC[index]->lpDS3DBuf->SetPosition(vPos.x, vPos.y, vPos.z, DS3D_IMMEDIATE);

			m_mapSC[index]->lpDSBuf->SetCurrentPosition(0); // ���ϰ� ó�� ����
			if (m_mapSC[index]->lpDSBuf->Play(0, 0, 0) == DSERR_BUFFERLOST )
				m_mapSC[index]->lpDSBuf->Restore();		 //
		}
	}
}

void CDirectSound::PlayDup(int index, float x, float y, float z)
{
	DWORD Status;
	HRESULT rVal;
	int i = 0;

	mapSC::iterator it = m_mapSC.begin();

	for (i=0; i < DUP_NUM; i++)
	{
		it = m_mapSC.find( i );

		if( it == m_mapSC.end() )
			break;

		if (m_mapSC[i]->lpDSBufDup == NULL)
			break;
		else
		{
			if (m_mapSC[i]->lpDSBufDup->GetStatus(&Status) == DSERR_BUFFERLOST)
			{
				if (g_bUse3DSound)
				{
					if (m_mapSC[i]->lpDS3DBufDup != NULL)
					{
						m_mapSC[i]->lpDS3DBufDup->Release();
						m_mapSC[i]->lpDS3DBufDup = NULL;
					}
				}
				m_mapSC[i]->lpDSBufDup->Restore();
				m_mapSC[i]->lpDSBufDup->Release();
				m_mapSC[i]->lpDSBufDup = NULL;
				break;
			}
			if ( Status & DSBSTATUS_PLAYING)
				continue;

			if (g_bUse3DSound)
			{
				if (m_mapSC[i]->lpDS3DBufDup != NULL)
				{
					m_mapSC[i]->lpDS3DBufDup->Release();
					m_mapSC[i]->lpDS3DBufDup = NULL;
				}
			}
			m_mapSC[i]->lpDSBufDup->Release();
			m_mapSC[i]->lpDSBufDup = NULL;
			break;
		}
	}

	it = m_mapSC.find( i );
	if( it == m_mapSC.end() )
		return;

	if ( i < DUP_NUM )
	{
		D3DVECTOR vPos;
		vPos.x = x;
		vPos.y = y;
		vPos.z = z;
		D3DMath_VectorMatrixMultiply(vPos, vPos, m_matRotateY);

		if (m_lpDS->DuplicateSoundBuffer(m_mapSC[index]->lpDSBuf, &m_mapSC[i]->lpDSBufDup) == DSERR_OUTOFMEMORY)
		{
		}

		if (g_bUse3DSound)
		{
			if( m_mapSC[i]->lpDSBufDup->QueryInterface( IID_IDirectSound3DBuffer, (VOID**)&m_mapSC[i]->lpDS3DBufDup ) !=DS_OK )
			{
			}
			else
			{
				DS3DBUFFER Ds3DBuffer;

				Ds3DBuffer.dwSize = sizeof(DS3DBUFFER);
				m_mapSC[i]->lpDS3DBufDup->GetAllParameters(&Ds3DBuffer);
				Ds3DBuffer.dwMode = DS3DMODE_NORMAL;
				Ds3DBuffer.flMinDistance = m_fMinDistance;
				Ds3DBuffer.flMaxDistance = m_fMaxDistance;

				m_mapSC[i]->lpDS3DBufDup->SetAllParameters(&Ds3DBuffer, DS3D_IMMEDIATE);
			}

			m_mapSC[i]->lpDS3DBufDup->SetPosition(vPos.x, vPos.y, vPos.z, DS3D_IMMEDIATE);
		}

		m_mapSC[i]->lpDSBufDup->SetCurrentPosition(0);
		if ( (rVal = m_mapSC[i]->lpDSBufDup->Play(0, 0, 0)) != DS_OK)
		{
			if (rVal == DSERR_BUFFERLOST)
			{
			}
		}
	}
}

void CDirectSound::Loop(int index)
{

	if (m_lpDS == NULL || ! m_bIsUse)
		return;

	if (IsBufIndex(index))
	{
		m_mapSC[index]->lpDSBuf->SetCurrentPosition(0);
		m_mapSC[index]->lpDSBuf->Play(0, 0, DSBPLAY_LOOPING);
	}
}

void CDirectSound::Stop(int index)
{
	if (m_lpDS == NULL)
		return;

	if (IsBufIndex(index))
		m_mapSC[index]->lpDSBuf->Stop();
}

void CDirectSound::StopAll()
{
	int i;

	if (m_lpDS == NULL)
		return;

	mapSC::iterator itEnd = m_mapSC.end();
	for (i=0; i < m_iCurBufIdx; i++)
	{
		if( m_mapSC.find( i ) == itEnd )
			continue;
		if( m_mapSC[i]->lpDS3DBuf == NULL )
			continue;
		else
			m_mapSC[i]->lpDSBuf->Stop();
	}

	for (i=0; i < DUP_NUM; i++)
	{
		if( m_mapSC.find( i ) == itEnd )
			continue;
		if (m_mapSC[i]->lpDSBufDup == NULL)
			continue;
		else
			m_mapSC[i]->lpDSBufDup->Stop();
	}
}

BOOL CDirectSound::IsPlaying(int index)
{
	if (m_lpDS == NULL)
		return FALSE;

	if (IsBufIndex(index))
	{
		DWORD Status;
		if (m_mapSC[index]->lpDSBuf->GetStatus(&Status) == DSERR_BUFFERLOST)
		{
			m_mapSC[index]->lpDSBuf->Restore();
		};
		return Status & DSBSTATUS_PLAYING;
	}
	return FALSE;
}

BOOL CDirectSound::IsLooping(int index)
{
	if (m_lpDS == NULL)
		return FALSE;

	if (IsBufIndex(index))
	{
		DWORD Status;
		m_mapSC[index]->lpDSBuf->GetStatus(&Status);
		return Status & DSBSTATUS_LOOPING;
	}
	return FALSE;
}

DWORD CDirectSound::GetPosition(int index)
{
	if (m_lpDS == NULL)
		return 0;

	if (IsBufIndex(index))
	{
		DWORD pos;
		m_mapSC[index]->lpDSBuf->GetCurrentPosition(&pos, NULL);
		return pos;
	}
	else
	{
		return 0;
	}
}

void CDirectSound::SetPan(int index,long pan)
{
	if (m_lpDS == NULL)
		return;

	if (IsBufIndex(index)) m_mapSC[index]->lpDSBuf->SetPan(pan);
}

void CDirectSound::GetPan(int index,long& pan)
{
	if (m_lpDS == NULL)
		return;

	if (IsBufIndex(index)) m_mapSC[index]->lpDSBuf->GetPan(&pan);
}

void CDirectSound::SetVol(int index,long vol)
{
	if (m_lpDS == NULL)
		return;

	if (IsBufIndex(index)) m_mapSC[index]->lpDSBuf->SetVolume(vol);
}

void CDirectSound::GetVol(int index,long& vol)
{
	if (m_lpDS == NULL)
		return;

	if (IsBufIndex(index)) m_mapSC[index]->lpDSBuf->GetVolume(&vol);
}

void CDirectSound::SetFreq(int index,DWORD freq)
{
	if (m_lpDS == NULL)
		return;

	if (IsBufIndex(index)) m_mapSC[index]->lpDSBuf->SetFrequency(freq);
}

void CDirectSound::GetFreq(int index,DWORD& freq)
{
	if (m_lpDS == NULL)
		return;

	if (IsBufIndex(index)) m_mapSC[index]->lpDSBuf->GetFrequency(&freq);
}


void CDirectSound::_Set3DBufferPos(float x, float y, float z)
{
	if( IsBufIndex(0) == FALSE )
		return;

	if (g_bUse3DSound)
		m_mapSC[0]->lpDS3DBuf->SetPosition(x, y, z,DS3D_IMMEDIATE);
}

void CDirectSound::_SetListenerPos(float x, float y, float z)
{
	if (g_bUse3DSound)
		m_pDS3DListener->SetPosition(x, y, z,DS3D_IMMEDIATE);
}

void CDirectSound::SetDistanceFactor(D3DVALUE fDistanceFactor, DWORD dwApply)
{
	if (g_bUse3DSound)
		m_pDS3DListener->SetDistanceFactor(fDistanceFactor, dwApply);
}

void CDirectSound::SetRolloffFactor(float fMultiple)
{
	if (g_bUse3DSound)
		m_pDS3DListener->SetRolloffFactor(DS3D_DEFAULTROLLOFFFACTOR * fMultiple, DS3D_IMMEDIATE );
}

// ���� �ҽ��� ���Ϲ��⿡ �ִٰ� �Ҷ�.
// ī�޶� �ٶ󺸴� ���� 0���϶�(������ �ٶ󺻴�.) ���� �ҽ��� 90���� ��ġ (��������� ����).
// ī�޶� �ٶ󺸴� ���� 90���϶� ���� �ҽ��� ȸ���� �ʿ����.
// ī�޶� �ٶ󺸴� ���� 180���϶� ���� �ҽ��� �ð� �������� 90�� ȸ�� �ʿ�.
//
void CDirectSound::AdjustSoundSourceDirMatrix(D3DVECTOR &vCam, D3DVECTOR &vAt)
{
	D3DVECTOR vDir;

	vDir.x = vAt.x - vCam.x;
	vDir.y = vAt.y - vCam.y;
	vDir.z = vAt.z - vCam.z;

	// ī�޶�� �÷��̾��� ������ x�� z�� ������ �Ѵ�.
	m_fDir = atan2f(vDir.z, vDir.x);		// ī�޶󿡼� �÷��̾ �� ����.
	// ���� �ҽ��� ȸ���� ����.. ī�޶� ȸ���� �ݴ����.
	// �ð�������� 270�� ȸ�� �ʿ�.
	m_fDir = m_fDir + g_PI + +g_PI_DIV_2;

	if (m_fDir >= g_2_PI)
		m_fDir -= g_2_PI;

	D3DUtil_SetRotateYMatrix(m_matRotateY, m_fDir );
}

// ���� ���۸� ���θ���� �߰����� �ʰ� �ܺ��� ���� ���ۿ� �ε��ϰ� ������ ����ϴ� �Լ�.(����) ///�Ź���ȭ�ý��� �۾��� �߰�.
LPDIRECTSOUNDBUFFER CDirectSound::LoadWavToOutside(const char* wav)
{
	int  ret;
	WORD wExtraAlloc;
	UINT i, DataIn;

	HMMIO          hmmioIn;
	MMIOINFO       mmioinfoIn;
	MMCKINFO       ckInfo, ckInfoRIFF;
	LPWAVEFORMATEX lpWaveFormat=0;
	PCMWAVEFORMAT  pcmWaveFormat;

	LPBYTE  lpData;
	LPVOID  lpWrapPtr;
	DWORD   dwBSize, dwWavSize, dwDataSize, dwWrapBSize;

	DSBUFFERDESC    dsbd;

	LPDIRECTSOUNDBUFFER pResultDSB; // �����Ͽ� ��ȯ�ϴ� ��� ���� ����

	if (m_lpDS == NULL)
	{
		// DirectSound�� �ʱ�ȭ ���� �ʾҴ�.
		return NULL;
	}

	// ȭ���� ����
	hmmioIn=mmioOpen((char*)wav, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (!hmmioIn)
	{
		DirectSoundError("WAV ȭ���� ������ ����");
		return NULL;
	}

	// ����ȭ���� RIFF WAVEȭ������ Ȯ���Ѵ�
	ckInfoRIFF.fccType = mmioFOURCC('W','A','V','E');
	if (mmioDescend(hmmioIn, &ckInfoRIFF, NULL, MMIO_FINDRIFF) != 0)
	{
		DirectSoundError("RIFF WAVE ȭ���� �ƴմϴ�");
		return NULL;
	}

	// 'fmt' Chunk���� Ȯ���Ѵ�
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hmmioIn, &ckInfo,&ckInfoRIFF, MMIO_FINDCHUNK) != 0)
	{
		DirectSoundError("ȭ���� fmt Chunk�� �ƴմϴ�");
		return NULL;
	}

	// ����ȭ���� �д´�
	ret = mmioRead(hmmioIn, (HPSTR) &pcmWaveFormat, (long) sizeof(pcmWaveFormat));
	if (ret != (long) sizeof(pcmWaveFormat))
	{
		DirectSoundError("ȭ�� �б� ����");
		return NULL;
	}

	// ���� ȭ���� PCM�������� Ȯ���Ѵ�
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
		wExtraAlloc = 0;
	else
	{
		ret = mmioRead(hmmioIn, (LPSTR) &wExtraAlloc,(long) sizeof(wExtraAlloc));
		if (ret != (long) sizeof(wExtraAlloc))
		{
			DirectSoundError("ȭ�� �б� ����");
			return NULL;
		}
	}

	lpWaveFormat = (LPWAVEFORMATEX) malloc(sizeof(WAVEFORMATEX)+wExtraAlloc);
	memcpy(lpWaveFormat, &pcmWaveFormat, sizeof(pcmWaveFormat));
	lpWaveFormat->cbSize = wExtraAlloc;

	if (wExtraAlloc != 0)
	{
		ret = mmioRead(hmmioIn,
					   (LPSTR)(((BYTE*)&(lpWaveFormat->cbSize))+sizeof(wExtraAlloc)),
					   (long)(wExtraAlloc));
		if (ret != (long) sizeof(wExtraAlloc))
		{
			DirectSoundError("ȭ�� �б� ����");
			return NULL;
		}
	}

	if (mmioAscend(hmmioIn, &ckInfo, 0) != MMSYSERR_NOERROR)
	{
		DirectSoundError("����");
		return NULL;
	}


	if (mmioSeek(hmmioIn, ckInfoRIFF.dwDataOffset+sizeof(FOURCC),SEEK_SET) == -1)
	{
		DirectSoundError("����");
		return NULL;
	}

	// 'data' Chunk�� ã�´�
	MMCKINFO    pckIn;
	pckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hmmioIn,&pckIn,&ckInfoRIFF, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		DirectSoundError("Data Chunk�� �ƴմϴ�");
		return NULL;
	}

	dwDataSize = pckIn.cksize;
	lpData     = NULL;

	// ����ƽ 2�� ���۸� ����� ���� ����ü�� �����Ѵ�.
	// ���� ������ ����� ����, �¿� �뷱��, ���ļ� ������ �����ϵ��� �Ѵ�
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	if (g_bUse3DSound)
		dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME;
	else
		dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = dwDataSize;
	dwWavSize = lpWaveFormat->cbSize + sizeof(WAVEFORMATEX);

	dsbd.lpwfxFormat = (LPWAVEFORMATEX) malloc(dwWavSize);
	memcpy( dsbd.lpwfxFormat, lpWaveFormat, dwWavSize );
	free(lpWaveFormat);

	// �˰��� ���úκ� ���� ��..
	//dsbd.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	if (g_bUse3DSound)
		dsbd.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION ;

	// 2�� ���� ���۸� �����Ѵ�.
	if (m_lpDS->CreateSoundBuffer(&dsbd, &pResultDSB,NULL) != DS_OK)
	{
		DirectSoundError("������� ��������");
		return NULL;
	}

	free(dsbd.lpwfxFormat);

	// ������ ���ۿ� �����Ѵ�
	ret = pResultDSB->Lock(0, dwDataSize, (void **)&lpData,&dwBSize,  &lpWrapPtr, &dwWrapBSize, 0L);
	if (ret != DS_OK)
	{
		DirectSoundError("����");
		return NULL;
	}
	dwDataSize = dwBSize;

	// ����ȭ���� ����(�����Ȳ)�� �˾ƿ´�
	if (mmioGetInfo(hmmioIn, &mmioinfoIn, 0) != MMSYSERR_NOERROR)
	{
		DirectSoundError("����");
		return NULL;
	}

	DataIn = dwDataSize;
	if (DataIn > pckIn.cksize) DataIn = pckIn.cksize;
	pckIn.cksize -= DataIn;

	for (i = 0; i < DataIn; i++)
	{
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
		{
			ret = mmioAdvance(hmmioIn, &mmioinfoIn, MMIO_READ);
			if (ret != MMSYSERR_NOERROR) DirectSoundError("����");
		}
		*((BYTE*)lpData + i) = *((BYTE*)mmioinfoIn.pchNext++);
	}
	if (mmioSetInfo(hmmioIn, &mmioinfoIn, 0) != MMSYSERR_NOERROR)
	{
		DirectSoundError("����");
		return NULL;
	}

	// unlock
	ret = pResultDSB->Unlock(lpData, dwBSize, lpWrapPtr, dwWrapBSize);
	if (ret != DS_OK)
	{
		DirectSoundError("����");
		return NULL;
	}

	// �����߿� �ε��Ǵ� ������ wav������ ������ config.ini������ �������� ������ ������Ų��
	// ���� ����ũ��� 0 -> -10000ũ���̴�..
	// �׷��� ���� ������..-5000�������� ���ʹ� ���ǼҸ��� ���� �鸮�� �ʾҴ�..�ؼ�
	// ���� �������� (-5000 ~ 0)������ �缳���ߴ�.

	pResultDSB->SetVolume((-5000 )+ (50*m_CurVolume));
	if(m_CurVolume < 10)
	{
		pResultDSB->SetVolume(-500000 );
	}

	return pResultDSB;
}

// �ܺ� ���� ���۸� �̿��Ͽ� �÷��� �����ִ� �Լ�.3D�� �������ϰ� ���ø�����Ʈ�� �������Ѵ�.(����) ///�Ź���ȭ�ý��� �۾��� �߰�.
void CDirectSound::PlayToOutside( LPDIRECTSOUNDBUFFER SrcSB/*, float x, float y, float z, BOOL bDup*/ )
{
	if (m_lpDS == NULL || ! m_bIsUse)
		return;

	DWORD Status = 0;
	if( SrcSB->GetStatus(&Status) == DSERR_BUFFERLOST )
	{
		SrcSB->Restore();
	}

	if( !(Status&DSBSTATUS_PLAYING) ) // �÷��� ���� �ƴҶ��� �÷����Ѵ�. // ȥ�� ó�������� ���Ѵ�. ������ �ݺ� ȣ���ϸ�
	{
		SrcSB->SetCurrentPosition(0); // ���ϰ� ó�� ����
		if( SrcSB->Play(0, 0, 0) == DSERR_BUFFERLOST )
			SrcSB->Restore();
	}
}

void CDirectSound::InsertMapSC(int index, LPSOUNDCHUNK pChunk, const char* filename)
{
	m_mapSC.insert( std::make_pair( index, pChunk ) );
	m_mapSC_Indexer.insert( std::make_pair( filename, index ) );
}

void CDirectSound::ClearMapSC()
{
	m_mapSC.clear();
	m_mapSC_Indexer.clear();
}