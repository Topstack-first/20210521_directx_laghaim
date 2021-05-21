#ifndef _DMUSICMGR_H_
#define _DMUSICMGR_H_

#include "Config.h"

#include <dmusici.h>
#include <vector>

#define DMUSIC_NUM		10	// 미디의 수
#define MIDI_NULL		0	// 미디 객체 없음
#define MIDI_LOADED		1	// 미디 객체 로드
#define MIDI_PALYING	2	// 미디 객체 음악 연주중
#define MIDI_STOPED		3	// 미디 객체 정지

#define MULTI_TO_WIDE( x, y) MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, y, -1, x, _MAX_PATH)

// initializes a direct draw struct
#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

typedef struct DMUSIC_MIDI_TYPE
{
	IDirectMusicSegment			*dm_segment;
	IDirectMusicSegmentState	*dm_segstate;
	int		id;
	int		state;

	DMUSIC_MIDI_TYPE() : dm_segment( NULL ), dm_segstate( NULL ), id( -1 ), state( -1 ) {}
	~DMUSIC_MIDI_TYPE()
	{
		SAFE_RELEASE( dm_segment );
		SAFE_RELEASE( dm_segstate );
	}
} DMUSIC_MIDI, *DMUSIC_MIDI_PTR;

typedef struct struct3DSOUND
{
	LPDIRECTSOUNDBUFFER     lpdsb;
	LPDIRECTSOUND3DBUFFER   lpds3db;
	struct3DSOUND() : lpdsb( NULL ), lpds3db( NULL )	{}
	~struct3DSOUND()
	{
		SAFE_RELEASE( lpdsb );
		SAFE_RELEASE( lpds3db );
	}
} _3DSOUND;

class CDMusicMgr
{
public:
	BOOL DirectMusicInit(LPDIRECTSOUND lpDS = NULL);
	BOOL DirectMusicLoader();

	int DirectMusicLoadMIDI(char *filename);
	void DirectMusicDelMidi(int index);
	void DirectMusicDelAll();
	void DirectMusicError(char* Message);
	void DirectMusicPlay(int index, bool);
	void DirectMusicStop(int index);
	void DirectMusicStop();
	BOOL DirectMusicIsPlaying(int index);
	void DirectMusicRelease(int index);
	void DirectMusicShutdown();
	CDMusicMgr(HWND, LPDIRECTSOUND);
	virtual ~CDMusicMgr();
	void SetVolumeLevelAll(long level);

	void PlayMusic_current(bool bRepeat);

	// new functions
	BOOL InitDirectMusic(LPDIRECTSOUND lpDS = NULL);
	IDirectMusicPort*	CreateSWSynthPort();
	IDirectMusicPort*	GetSWSynthPort()
	{
		return m_pSWSynthPort;
	};

	// 3D 사운드와 연결하기 위해서.
	LPDIRECTSOUND	m_lpds;
	_3DSOUND	My3DSound;
	LPDIRECTSOUNDBUFFER DMLoad3DSoundBuffer();


private:
	HWND m_hWnd;
	IDirectMusicPerformance *dm_perf;
	IDirectMusicLoader		*dm_loader;
	DMUSIC_MIDI				dm_midi[DMUSIC_NUM];

	IDirectMusic*            m_pDMusic;
	IDirectMusicPort*        m_pSWSynthPort;

	BOOL	m_bMusicInitSuccess;

	BOOL	m_bRepeat;
	int		m_nMidiIndex;
};

class CMusicSegment;

//////////////////////////////////////////////////////////////////////////
class CMusicManager
{
public:
	CMusicManager();
	virtual ~CMusicManager();
public:
	// get music manager properties.
	inline	IDirectMusicLoader8*			GetLoader()			const
	{
		return m_pLoader;
	}
	inline	IDirectMusicPerformance8*		GetPerformance()	const
	{
		return m_pPerformance;
	}
	inline	IDirectSound3DListener*			GetLinstener()		const
	{
		return m_pDSListener;
	}
	inline	IDirectMusicAudioPath8*			GetDefaultAudioPath();

	HRESULT						Initialize( HWND hWnd, DWORD dwPChannels = 128, DWORD dwDefaultPathType = DMUS_APATH_DYNAMIC_STEREO, LPDIRECTSOUND pDS = NULL );
	HRESULT						SetSearchDirectory( const char* strMediaPath );
	VOID						CollectGarbage();			// garbage collector.
	// stop all segments.
	VOID						StopAll();

	HRESULT						Play( int nIndex, bool bRepeats );
	HRESULT						Stop( int nIndex );

	HRESULT						CreateSegmentFromFile( CMusicSegment** ppSegment, char* strFileName, BOOL bDownloadNow = TRUE, BOOL bIsMidiFile = FALSE );
	HRESULT						CreateSegmentFromFile( CMusicSegment** ppSegment, char* strFileName, BOOL bDownloadNow = TRUE );
	HRESULT						LoadSegmentFromFile( int& nSlotNo, char* strFileName, BOOL bDownloadNow = TRUE, BOOL bIsMidiFile = FALSE );
	HRESULT						LoadSegmentFromFile( int& nSlotNo, char* strFileName, BOOL bDownloadNow = TRUE );

	HRESULT						CreateSegmentFromMemory( CMusicSegment** ppSegment, char* buf, int nBufSize, BOOL bDownloadNow = TRUE, BOOL bIsMidiFile = FALSE );
	HRESULT						CreateSegmentFromMemory( CMusicSegment** ppSegment, char* buf, int nBufSize, char* szFileExt, BOOL bDownloadNow = TRUE );
	HRESULT						LoadSegmentFromMemory( int& nSlot, char* buf, int nBufSize, BOOL bDownloadNow = TRUE, BOOL bIsMidiFile = FALSE );
	HRESULT						LoadSegmentFromMemory( int& nSlot, char* buf, int nBufSize, char* szFileExt, BOOL bDownloadNow = TRUE );

	VOID						Set3DParameters( FLOAT fDistanceFactor, FLOAT fDopplerFactor, FLOAT fRolloffFactor );

	VOID						SetVolumeLevelAll( long level );
protected:
	HRESULT						DecompressedMp3( unsigned char* outBuffer, unsigned char* inBuffer, size_t out_size, size_t in_size );

protected:
	HWND						m_hWnd;
	LPDIRECTSOUND				m_lpDS;
	IDirectMusicLoader8*		m_pLoader;
	IDirectMusicPerformance8*	m_pPerformance;
	IDirectSound3DListener*		m_pDSListener;
	DS3DLISTENER				m_dsListenerParams;				// Listener Properties.
	std::vector<CMusicSegment*>	m_vecMSegments;
};


class CMusicSegment
{
public:
	CMusicSegment( IDirectMusicPerformance8* pPerformance, IDirectMusicLoader8* pLoader,
				   IDirectMusicSegment8* psegment );
	virtual ~CMusicSegment();

	inline	IDirectMusicSegment8*	GetSegment() const
	{
		return m_pSegment;
	}
	HRESULT							SetRepeats( DWORD dwRepeats );
	virtual HRESULT					Play( DWORD dwFlags = DMUS_SEGF_SECONDARY, IDirectMusicAudioPath8* pAudioPath = NULL );
	HRESULT							Stop( DWORD dwFlags = 0 );
	HRESULT							Download( IDirectMusicAudioPath8* pAudioPath = NULL );
	HRESULT							Unload( IDirectMusicAudioPath8*	pAudioPath = NULL );

	BOOL							IsPlaying();

protected:
	IDirectMusicPerformance8*		m_pPerformance;
	IDirectMusicLoader8*			m_pLoader;
	IDirectMusicSegment8*			m_pSegment;
	IDirectMusicAudioPath8*			m_pEmbeddedAudioPath;
	BOOL							m_bDownloaded;
};

#endif // _DMUSICMGR_H_
