#ifndef _DIRTSND_H_
#define _DIRTSND_H_

#include "Config.h"

#include <mmsystem.h>
#include <dsound.h>
#define DUP_NUM	10

#include <d3d.h>
#include <map>
#include <string>


typedef std::map<int, LPDIRECTSOUNDBUFFER>		mapDSBuf;
typedef std::map<int, LPDIRECTSOUND3DBUFFER>	mapDS3DBuf;
typedef std::string								strWaveFileName;

typedef struct _SOUNDCHUNK
{
public:
	LPDIRECTSOUNDBUFFER		lpDSBuf;
	LPDIRECTSOUND3DBUFFER	lpDS3DBuf;
	LPDIRECTSOUNDBUFFER		lpDSBufDup;
	LPDIRECTSOUND3DBUFFER	lpDS3DBufDup;
	std::string				szWaveFileName;
public:
	_SOUNDCHUNK() : lpDSBuf( NULL ), lpDS3DBuf( NULL ), lpDSBufDup( NULL ), lpDS3DBufDup( NULL ) {}
	~_SOUNDCHUNK()
	{
		SAFE_RELEASE( lpDSBuf );
		SAFE_RELEASE( lpDS3DBuf );
		SAFE_RELEASE( lpDSBufDup );
		SAFE_RELEASE( lpDS3DBufDup );
	}
} SOUNDCHUNK, *LPSOUNDCHUNK;

typedef	std::map< int, LPSOUNDCHUNK >			mapSC;
typedef std::map< std::string, int >			mapSC_Indexer;

#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2

class CWaveFile
{
public:
	WAVEFORMATEX*				m_pwfx;
	HMMIO						m_hmmio;
	MMCKINFO					m_ck;
	MMCKINFO					m_ckRiff;
	DWORD						m_dwSize;
	MMIOINFO					m_mmioinfoOut;
	DWORD						m_dwFlags;
	BOOL						m_bIsReadingFromMemory;
	BYTE*						m_pbData;
	BYTE*						m_pbDataCur;
	ULONG						m_ulDataSize;
	CHAR*						m_presourceBuffer;
protected:
	HRESULT						ReadMMIO();
	HRESULT						WriteMMIO( WAVEFORMATEX* pwfxDest );		// not used
public:

	CWaveFile();
	~CWaveFile();

	HRESULT						Open( LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags );
	HRESULT						OpenFromVFile( LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags );
	HRESULT						OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags );
	HRESULT						Close();

	HRESULT						Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
	HRESULT						Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );		// not used.

	DWORD						GetSize();
	HRESULT						ResetFile();
	inline WAVEFORMATEX*		GetFormat()
	{
		return m_pwfx;
	}
};



class CDirectSound
{
protected:
	float	m_fDir;					// 공간상에서 카메라가 바라보는 각.
	D3DMATRIX	m_matRotateY;		// 카메라에 따라 사운드의 소스를 회전시킬 매트릭스.
	float	m_fMinDistance, m_fMaxDistance;

public:
	CDirectSound(HWND);
	~CDirectSound();

	void SetAllVolume();
	void Play(int index, float x = 0, float y = 0, float z = 0, BOOL bDup = TRUE);
	void PlaySound(int index, D3DVERTEX pos, BOOL bDup = TRUE);
	void PlayDup(int index, float x, float y, float z);
	void Loop(int index);
	void Stop(int index);
	void StopAll();
	BOOL IsPlaying(int index);
	BOOL IsLooping(int index);
	void SetPan(int index,long pan);
	void GetPan(int index,long& pan);
	void SetVol(int index,long vol);
	void GetVol(int index,long& vol);
	void SetFreq(int index,DWORD freq);
	void GetFreq(int index,DWORD& freq);
	void SetWavSize(int num);
	void SetWaveFilePath(char *path);

	int LoadWAV(const char* wav);
	int LoadWav(const char* wav);
	int	LoadWavFromVFile( const char* wav );
	void GetVolFromConfigFile(bool bUse, long level);
	int GetDSBufCnt()
	{
		return m_iBufCount;
	};
	DWORD GetPosition(int index);
	long m_CurVolume;  /*"config.ini"파일을 읽어서 얻어온 볼륨갑값을 저장하고 게임중에 로딩되는
					   wav파일들의 볼륨을 m_CurVolume값으로 셋팅한다*/
	BOOL m_bIsUse;

	// 3D 사운드 관련.
	void _Set3DBufferPos(float x, float y, float z);
	void _SetListenerPos(float x, float y, float z);
	void SetDistanceFactor(D3DVALUE fDistanceFactor, DWORD dwApply = DS3D_IMMEDIATE);
	void SetRolloffFactor(float nMultiple);
	void AdjustSoundSourceDirMatrix(D3DVECTOR &vCam, D3DVECTOR &vAt);


	LPDIRECTSOUND GetDSound()
	{
		if (m_lpDS) return m_lpDS;
		else return NULL;
	};

	// 외부 사운드 버퍼를 사용하는 임시 효과음 용 함수들( by 원석 )
	LPDIRECTSOUNDBUFFER LoadWavToOutside(const char* wav); // 사운드 버퍼를 내부멤버에 추가하지 않고 외부의 사운드 버퍼에 로드하고 싶을때 사용하는 함수.(원석) ///신방어구강화시스템 작업때 추가.
	void PlayToOutside( LPDIRECTSOUNDBUFFER SrcSB/*, float x, float y, float z, BOOL bDup*/ ); // 외부 사운드 버퍼를 이용하여 플레이 시켜주는 함수.3D를 지원안하고 듀플리케이트도 지원안한다.(주석처리)(원석) ///신방어구강화시스템 작업때 추가.

public:
	BOOL IsBufIndex(int index);
	BOOL DirectSoundInit();
	void DirectSoundError(char* Message);
	void DSObjectRelease(void);

	// new functions
	BOOL InitDirectSound();

private:
	void InsertMapSC(int index, LPSOUNDCHUNK pChunk, const char* filename);
	void ClearMapSC();

private:
	HWND m_hWnd;
	LPDIRECTSOUND8       m_lpDS;
	char m_WaveFilePath[100];
	std::string			m_szWaveFilePath;
	char **m_WavFile;
	int m_iBufCount;                      // 버퍼의 갯수 (WAV)
	int m_iCurBufIdx;
	int m_iRefCount;
	mapSC				m_mapSC;
	mapSC_Indexer		m_mapSC_Indexer;

	LPDIRECTSOUND3DLISTENER m_pDS3DListener;


	DS3DBUFFER              m_BufferParams;           // 3D buffer properties
	DS3DLISTENER            m_ListenerParams;         // Listener properties
};

extern CDirectSound * g_pDSound;

#endif // _DIRTSND_H_

/*
  	Volume    -10000 ~ 0      (작다 ~ 크다, -2000 ~ 0)
	Pan       -10000 ~ 10000  ( 좌  ~  우 )
	Frequency    100 ~ 100000 (느림 ~ 빠름, 보통 속도는 약 11000)
	*초기값은 0
*/

