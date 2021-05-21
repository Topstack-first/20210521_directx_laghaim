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
	float	m_fDir;					// �����󿡼� ī�޶� �ٶ󺸴� ��.
	D3DMATRIX	m_matRotateY;		// ī�޶� ���� ������ �ҽ��� ȸ����ų ��Ʈ����.
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
	long m_CurVolume;  /*"config.ini"������ �о ���� ���������� �����ϰ� �����߿� �ε��Ǵ�
					   wav���ϵ��� ������ m_CurVolume������ �����Ѵ�*/
	BOOL m_bIsUse;

	// 3D ���� ����.
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

	// �ܺ� ���� ���۸� ����ϴ� �ӽ� ȿ���� �� �Լ���( by ���� )
	LPDIRECTSOUNDBUFFER LoadWavToOutside(const char* wav); // ���� ���۸� ���θ���� �߰����� �ʰ� �ܺ��� ���� ���ۿ� �ε��ϰ� ������ ����ϴ� �Լ�.(����) ///�Ź���ȭ�ý��� �۾��� �߰�.
	void PlayToOutside( LPDIRECTSOUNDBUFFER SrcSB/*, float x, float y, float z, BOOL bDup*/ ); // �ܺ� ���� ���۸� �̿��Ͽ� �÷��� �����ִ� �Լ�.3D�� �������ϰ� ���ø�����Ʈ�� �������Ѵ�.(�ּ�ó��)(����) ///�Ź���ȭ�ý��� �۾��� �߰�.

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
	int m_iBufCount;                      // ������ ���� (WAV)
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
  	Volume    -10000 ~ 0      (�۴� ~ ũ��, -2000 ~ 0)
	Pan       -10000 ~ 10000  ( ��  ~  �� )
	Frequency    100 ~ 100000 (���� ~ ����, ���� �ӵ��� �� 11000)
	*�ʱⰪ�� 0
*/

