#include "stdafx.h"
/////////////////////////////////////////
//
//        다이렉트 사운드 클래스
//
/////////////////////////////////////////

//#include "stdafx.h"

#include <direct.h>
#include <dxerr9.h>

#include <mmsystem.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>

#include "DMusicMgr.h"

extern BOOL g_bUse3DSound;

#define MUSIC_VOLUME_RANGE      ( 0-(DMUS_VOLUME_MIN/4) )


//////////////////////////////////////////////////////////////////////////
// local functions.
HRESULT		ConvertAnsiStringToWideCch( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar )
{
	if( wstrDestination == NULL || strSource == NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	cchDestChar = cchDestChar / sizeof(WCHAR);

	int nResult = MultiByteToWideChar( CP_ACP, 0, strSource, -1, wstrDestination, cchDestChar );
	wstrDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDMusicMgr::CDMusicMgr(HWND hWindow, LPDIRECTSOUND lpDS)
	: dm_perf(NULL)
	, dm_loader(NULL)
{
	m_hWnd = hWindow;
	m_bMusicInitSuccess = FALSE;
	m_pDMusic = NULL;
	m_pSWSynthPort = NULL;

	m_lpds = lpDS;
	My3DSound.lpds3db = NULL;
	My3DSound.lpdsb = NULL;

	m_bRepeat		=	FALSE;
	m_nMidiIndex	=	-1;
}

CDMusicMgr::~CDMusicMgr()
{
	DirectMusicShutdown();
}

BOOL CDMusicMgr::InitDirectMusic(LPDIRECTSOUND lpDS)
{
	if (FAILED(CoInitialize(NULL)))
	{
		return FALSE;
	}

	if( FAILED( CoCreateInstance( CLSID_DirectMusic, NULL,
								  CLSCTX_INPROC_SERVER, IID_IDirectMusic,
								  (VOID**)&m_pDMusic ) ) )
	{
		//MessageBox( hWnd, "Could not create DirectMusic!",
		//            "Initialization Error", MB_ICONERROR|MB_OK );
		return E_FAIL;
	}

	if( FAILED( m_pDMusic->SetDirectSound( lpDS, m_hWnd ) ) )
	{
		//MessageBox( hWnd, "Could not set DSound to the DMusic object!",
		//            "Initialization Error", MB_ICONERROR|MB_OK );
		return E_FAIL;
	}

	// 마스터 인터페이스를 생성한다.
	if (SUCCEEDED(CoCreateInstance(CLSID_DirectMusicPerformance,
								   NULL,
								   CLSCTX_INPROC,
								   IID_IDirectMusicPerformance,
								   (void**)&dm_perf)))
	{
		if( SUCCEEDED(dm_perf->Init( &m_pDMusic, NULL, NULL ) ) )
		{
			// Set autodownloading to be on
			BOOL fAutoDownload = TRUE;
			if( FAILED( dm_perf->SetGlobalParam( GUID_PerfAutoDownload,
												 &fAutoDownload,
												 sizeof(fAutoDownload) ) ) )
			{
				dm_perf->Release();
				dm_perf = NULL;
			}
		}

	}
	else
	{
		// return null
		dm_perf = NULL;
		m_bMusicInitSuccess = FALSE;
		return FALSE;
	}

	m_pSWSynthPort = CreateSWSynthPort();

	m_bMusicInitSuccess = TRUE;		// 임시.

	DirectMusicLoader();

	if( SUCCEEDED( dm_perf->AddPort( m_pSWSynthPort ) ) )
	{
		if( FAILED( dm_perf->AssignPChannelBlock( 0, m_pSWSynthPort, 1 ) ) )
		{
			m_bMusicInitSuccess = FALSE;
			return m_bMusicInitSuccess;
		}
	}

	// wave output buffer에 연결하기.
	if (g_bUse3DSound)
	{
		My3DSound.lpdsb = DMLoad3DSoundBuffer();
		if ( My3DSound.lpdsb )
		{
			My3DSound.lpdsb->QueryInterface( IID_IDirectSound3DBuffer,
											 ( LPVOID * )&My3DSound.lpds3db );
		}
		if ( ( !My3DSound.lpdsb ) | ( !My3DSound.lpds3db ) )
		{
			m_bMusicInitSuccess = FALSE;
			return FALSE;
		}
	}
	else
	{
		m_pSWSynthPort->SetDirectSound(lpDS, NULL);
	}
	m_pSWSynthPort->Activate( TRUE );

	// 구조체 초기화
	DirectMusicDelAll();


	return m_bMusicInitSuccess;
}

LPDIRECTSOUNDBUFFER CDMusicMgr::DMLoad3DSoundBuffer()
{
	IDirectSoundBuffer* pDSB = NULL;
	DSBUFFERDESC        dsBD = {0};
	HRESULT             hr   = E_FAIL;

	if( !m_lpds || !m_pSWSynthPort )
	{
		return NULL;
	}

	WAVEFORMATEX _WaveFormat;
	ZeroMemory( &_WaveFormat, sizeof(_WaveFormat) );

	ZeroMemory( &dsBD, sizeof(dsBD) );
	dsBD.dwSize      = sizeof(dsBD);
	dsBD.dwFlags     = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME |
					   DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2 |
					   DSBCAPS_STICKYFOCUS;
	dsBD.lpwfxFormat = &_WaveFormat;

	DWORD dwWFXSize = sizeof(_WaveFormat);
	if( FAILED( hr = m_pSWSynthPort->GetFormat( &_WaveFormat, &dwWFXSize,
					 &dsBD.dwBufferBytes ) ) )
	{
		_WaveFormat.wFormatTag      = WAVE_FORMAT_PCM;
		_WaveFormat.nChannels       = 2;
		//_WaveFormat.nSamplesPerSec  = 22050;
		_WaveFormat.nSamplesPerSec  = 44100;
		_WaveFormat.wBitsPerSample  = 16;
		_WaveFormat.nBlockAlign     = _WaveFormat.nChannels *
									  ( _WaveFormat.wBitsPerSample / 8 );
		_WaveFormat.nAvgBytesPerSec = _WaveFormat.nSamplesPerSec * _WaveFormat.nBlockAlign;
		_WaveFormat.cbSize          = sizeof(_WaveFormat);

		dsBD.dwBufferBytes = _WaveFormat.nAvgBytesPerSec;
	}

	if( SUCCEEDED( hr = IDirectSound_CreateSoundBuffer( m_lpds, &dsBD, &pDSB,
						NULL ) ) )
	{
		hr = m_pSWSynthPort->SetDirectSound( m_lpds, pDSB );
	}

	if( pDSB && FAILED(hr) )
	{
		pDSB->Release();
		pDSB = NULL;
	}

	return pDSB;
}

//-----------------------------------------------------------------------------
// Name: CreateSWSynthPort()
// Desc: Create a software synth port object
//-----------------------------------------------------------------------------
IDirectMusicPort* CDMusicMgr::CreateSWSynthPort()
{
	if( !m_pDMusic )
		return NULL;

	IDirectMusicPort* pPort = NULL;
	HRESULT           hr;
	GUID              guidPortGUID;
	DMUS_PORTPARAMS   dmos;
	DMUS_PORTCAPS     dmpc;

	// See if the default port suits our needs
	if( SUCCEEDED( m_pDMusic->GetDefaultPort( &guidPortGUID ) ) )
	{
		ZeroMemory( &dmos, sizeof(dmos) );
		dmos.dwSize          = sizeof(DMUS_PORTPARAMS);
		dmos.dwChannelGroups = 1;
		dmos.dwValidParams   = DMUS_PORTPARAMS_CHANNELGROUPS;

		if( SUCCEEDED( m_pDMusic->CreatePort( guidPortGUID, &dmos, &pPort,
											  NULL ) ) )
		{
			ZeroMemory( &dmpc, sizeof(dmpc) );
			dmpc.dwSize = sizeof(DMUS_PORTCAPS);

			if( SUCCEEDED( pPort->GetCaps( &dmpc ) ) )
			{
				if( ( dmpc.dwClass != DMUS_PC_OUTPUTCLASS ) ||
						!( dmpc.dwFlags & DMUS_PC_DLS ) ||
						!( dmpc.dwFlags & DMUS_PC_DIRECTSOUND ) )
				{
					pPort->Release();
					pPort = NULL;
				}
			}
			else
			{
				pPort->Release();
				pPort = NULL;
			}
		}
	}

	if( NULL == pPort )
	{
		// Find a software synth output port.
		for( DWORD index = 0; ; index++ )
		{
			ZeroMemory( &dmpc, sizeof(dmpc) );
			dmpc.dwSize = sizeof(DMUS_PORTCAPS);

			hr = m_pDMusic->EnumPort( index, &dmpc );

			if( SUCCEEDED(hr) && hr != S_FALSE )
			{
				if( ( dmpc.dwClass == DMUS_PC_OUTPUTCLASS ) &&
						( dmpc.dwFlags & DMUS_PC_DLS ) &&
						( dmpc.dwFlags & DMUS_PC_DIRECTSOUND ) )
				{
					CopyMemory( &guidPortGUID, &dmpc.guidPort, sizeof(GUID) );

					ZeroMemory( &dmos, sizeof(dmos) );
					dmos.dwSize          = sizeof(DMUS_PORTPARAMS);
					dmos.dwChannelGroups = 1;
					dmos.dwValidParams   = DMUS_PORTPARAMS_CHANNELGROUPS;

					if( FAILED( m_pDMusic->CreatePort( guidPortGUID, &dmos,
													   &pPort, NULL ) ) )
					{
						pPort = NULL;
					}
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	return pPort;
}

BOOL CDMusicMgr::DirectMusicInit(LPDIRECTSOUND lpDS)
{
	if (FAILED(CoInitialize(NULL)))
	{
		return FALSE;
	}

	// 마스터 인터페이스를 생성한다.
	if (FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicPerformance,
								(void**)&dm_perf)))
	{
		// return null
		dm_perf = NULL;
		m_bMusicInitSuccess = FALSE;
		return FALSE;
	} // end if

	// 연주를 초기화 한다.
	//if( FAILED(dm_perf->Init(NULL, lpDS, NULL)))
	if( FAILED(dm_perf->Init(NULL, lpDS, m_hWnd)))
		//if( FAILED(dm_perf->Init(NULL, NULL, m_hWnd)))
	{
		m_bMusicInitSuccess = FALSE;
		return FALSE;
	}

	BOOL fAutoDownload = TRUE;
	if( FAILED( dm_perf->SetGlobalParam( GUID_PerfAutoDownload,
										 &fAutoDownload,
										 sizeof(fAutoDownload) ) ) )
	{
		dm_perf->Release();
		dm_perf = NULL;
	}

	m_bMusicInitSuccess = TRUE;

	DirectMusicLoader();

	// 연주에 포트를 추가한다.
	if( FAILED(dm_perf->AddPort(NULL)))
	{
		m_bMusicInitSuccess = FALSE;
		return FALSE;
	}

	// 구조체 초기화
	DirectMusicDelAll();


	return m_bMusicInitSuccess;
}

BOOL CDMusicMgr::DirectMusicLoader()
{

	if (!m_bMusicInitSuccess)
	{
		return FALSE;
	}

	// MIDI 파일을 로더할 수 있도록 IDirectMusicLoader를 만든다.
	if( FAILED(CoCreateInstance(CLSID_DirectMusicLoader,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicLoader,
								(void**)&dm_loader)))
	{
		dm_loader = NULL;
		m_bMusicInitSuccess = FALSE;
		return m_bMusicInitSuccess;
	}

	m_bMusicInitSuccess = TRUE;

	return TRUE;

}

void CDMusicMgr::PlayMusic_current(bool bRepeat)
{
	if (!m_bMusicInitSuccess)
		return;

	if (m_nMidiIndex >= DMUSIC_NUM)
		return;

	if(m_nMidiIndex == -1)
	{
		m_nMidiIndex = 4;
		m_bRepeat = bRepeat;
	}

	dm_perf->Stop(NULL, NULL, 0, 0);

	{
		if( dm_midi[m_nMidiIndex].state == MIDI_LOADED )
		{
			// midi 를 반복적으로 연주할지 결정
			if( m_bRepeat == TRUE )
				dm_midi[m_nMidiIndex].dm_segment->SetRepeats( 0xFFFFFFFF );
			else
				dm_midi[m_nMidiIndex].dm_segment->SetRepeats( 0 );

			dm_perf->PlaySegment(dm_midi[m_nMidiIndex].dm_segment, 0, 0, &dm_midi[m_nMidiIndex].dm_segstate);
		}
	}

}

// midi 연주
void CDMusicMgr::DirectMusicPlay(int index, bool repeat)
{

	if (!m_bMusicInitSuccess)
		return;

	if (index >= DMUSIC_NUM)
		return;

	dm_perf->Stop(NULL, NULL, 0, 0);

	m_nMidiIndex = index;
	m_bRepeat = repeat;

	//if ( m_pSWSynthPort && SUCCEEDED( m_pSWSynthPort->Activate( TRUE ) ) )
	{
		if( dm_midi[index].state == MIDI_LOADED )
		{
			// midi 를 반복적으로 연주할지 결정
			if( repeat == TRUE )
				dm_midi[index].dm_segment->SetRepeats( 0xFFFFFFFF );
			else
				dm_midi[index].dm_segment->SetRepeats( 0 );

			dm_perf->PlaySegment(dm_midi[index].dm_segment, 0, 0, &dm_midi[index].dm_segstate);
		}
	}
}



// Music 볼륨을 조절하는 함수다
// 최고 보륨은 0 || 최소 볼륨은 -5000이다
// 실제로 사용시에는 100분률로 표현한다 (최소볼륨0, 최대볼륨100)
// 그러나 실제 실험결과..-3000정도의 볼륨에서 거의 들리지 않았다.
// 해서 설정치를 (-300 ~ 0)으로 재 설정 했다
void CDMusicMgr::SetVolumeLevelAll(long level)
{
	long chang_level = (-3000) +(30*level);

	if(level < 10)
		chang_level = -10000;


	// Adjust the slider position to match GUID_PerfMasterTempo range
	dm_perf->SetGlobalParam( GUID_PerfMasterVolume, (void*)&chang_level, sizeof(long) );
}


// 선택적 연주 정지
void CDMusicMgr::DirectMusicStop(int index)
{
	if (!m_bMusicInitSuccess)
		return;

	dm_perf->Stop(dm_midi[index].dm_segment, NULL, 0, 0);
}

// 모든 연주 정지
void CDMusicMgr::DirectMusicStop()
{
	dm_perf->Stop(NULL, NULL, 0, 0);
}

// 현재 연주 중인지 검사
BOOL CDMusicMgr::DirectMusicIsPlaying(int index)
{
	if (!m_bMusicInitSuccess)
		return FALSE;

	if( dm_perf->IsPlaying(dm_midi[index].dm_segment, NULL) == S_OK)
		return TRUE;
	else
		return FALSE;
}

void CDMusicMgr::DirectMusicRelease(int index)
{
	dm_midi[index].dm_segment->SetParam(GUID_Unload, -1, 0, 0, (void*)dm_perf);

	dm_midi[index].dm_segment->Release();
	dm_midi[index].dm_segment = NULL;
}

void CDMusicMgr::DirectMusicShutdown()
{
	// 플레이중인 음악을 중지한다.
	if( dm_perf )
		dm_perf->Stop(NULL, NULL, 0, 0);

	if (m_pSWSynthPort)
	{
		m_pSWSynthPort->Activate( FALSE );
		if (My3DSound.lpdsb != NULL)
		{
			m_pSWSynthPort->SetDirectSound(NULL, NULL);
		}
		m_pSWSynthPort->Release();
	}

	if (My3DSound.lpds3db)
	{
		My3DSound.lpds3db->Release();
	}

	if (My3DSound.lpdsb)
	{
		My3DSound.lpdsb->Release();
	}

	// Midi 세그먼트 해제
	for( int index = 0; index < DMUSIC_NUM; index++)
	{
		if( dm_midi[index].state != MIDI_NULL )
		{
			DirectMusicRelease(index);
		}
	} // end for index

	if( dm_perf )
	{
		dm_perf->CloseDown();
		dm_perf->Release();
	}

	if( dm_loader )
		dm_loader->Release();

	if (m_pDMusic)
		m_pDMusic->Release();

	// Release COM.
	CoUninitialize();

}


// 새 MIDI 파일을 로드하기 위해 dm_midi[] 배열에서
// 하나의 개방 슬롯을 찾아 검색 경로를 설정하고
// 그 세그먼트를 만들며 세그먼트를 로드한후 해제하고..
// ..배열색인을 반환...
int CDMusicMgr::DirectMusicLoadMIDI(char *filename)
{

	DMUS_OBJECTDESC ObjDesc;
	HRESULT hr;
	IDirectMusicSegment* pSegment = NULL;

	int index; // Loop Val

	int id = -1;

	if (!m_bMusicInitSuccess)
		return (-1);

	for( index = 0; index < DMUSIC_NUM; index++)
	{
		if( dm_midi[index].state == MIDI_NULL )
		{
			id = index;
			break;
		}
	} // end for index

	if( id == -1 )
	{
		DirectMusicError("id를 얻지 못했습니다.");
		return(-1);
	}

	// 현재 작업중인 디렉토리..
	char szDir[_MAX_PATH];
	WCHAR wszDir[_MAX_PATH];

	if( _getcwd( szDir, _MAX_PATH ) == NULL )
	{
		DirectMusicError("현재 디렉토리 실패");
		return(-1);
	}

	MULTI_TO_WIDE(wszDir, szDir);

	// 파일을 열어보자
	hr = dm_loader->SetSearchDirectory( GUID_DirectMusicAllTypes,
										wszDir, FALSE);

	if( FAILED(hr))
	{
		DirectMusicError("미디파일 열기 실패");
		return(-1);
	}

	//  filename 을 wide string 으로 변환
	WCHAR wfilename[_MAX_PATH];
	MULTI_TO_WIDE(wfilename, filename);

	// setup object description
	DD_INIT_STRUCT(ObjDesc);
	ObjDesc.guidClass = CLSID_DirectMusicSegment;
	ObjDesc.dwSize      = sizeof(DMUS_OBJECTDESC);
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;

	wcscpy( ObjDesc.wszFileName, wfilename);

	hr = dm_loader->GetObject(&ObjDesc, IID_IDirectMusicSegment,
							  (void**) &pSegment);

	if( FAILED(hr))
	{
		DirectMusicError("[DirectMusicLoadMIDI] GetObject 실패");
		return(-1);
	}


	/*hr = pSegment->SetParam(GUID_StandardMIDIFile, -1, 0, 0, (void*)dm_perf);

	if( FAILED(hr))
	{
		DirectMusicError("[DirectMusicLoadMIDI] SetParam 실패");
		return(-1);
	}


	hr = pSegment->SetParam(GUID_Download, -1, 0, 0, (void*)dm_perf);

	if( FAILED(hr))
	{
		DirectMusicError("[DirectMusicLoadMIDI] SetParam (2) 실패");
		return(-1);
	}*/

	dm_midi[id].dm_segment	= pSegment;
	dm_midi[id].dm_segstate = NULL;
	dm_midi[id].state		= MIDI_LOADED;

	return(id);

}

// 미디 구조체의 index 번째를 초기화.
void CDMusicMgr::DirectMusicDelMidi(int index)
{
	dm_midi[index].state = MIDI_NULL;
}

// 미디 구조체 전체를 초기화
void CDMusicMgr::DirectMusicDelAll()
{
	// dm_midi 초기화..
	for( int i=0; i < DMUSIC_NUM; i++)
	{
		dm_midi[i].state = MIDI_NULL;
	}
}



void CDMusicMgr::DirectMusicError(char* Message)
{
	//if (m_lpDS) DSObjectRelease();
}


//////////////////////////////////////////////////////////////////////////
// CMusicManager class.
CMusicManager::CMusicManager() :
	m_hWnd( NULL ),
	m_lpDS( NULL ),
	m_pLoader( NULL ),
	m_pPerformance( NULL ),
	m_pDSListener( NULL )
{
	ZeroMemory( &m_dsListenerParams, sizeof( DS3DLISTENER ) );
	m_dsListenerParams.dwSize = sizeof( DS3DLISTENER );
}

CMusicManager::~CMusicManager()
{
	if( !m_vecMSegments.empty() )
	{
		for( std::vector<CMusicSegment*>::iterator iter = m_vecMSegments.begin(); iter != m_vecMSegments.end(); ++iter )
		{
			(*iter)->Stop();
			delete (*iter);
		}

		m_vecMSegments.clear();
	}

	SAFE_RELEASE( m_pLoader );
	SAFE_RELEASE( m_pDSListener );

	if( m_pPerformance )
	{
		// If there is any music playing, stop it.
		m_pPerformance->Stop( NULL, NULL, 0, 0 );
		m_pPerformance->CloseDown();

		SAFE_RELEASE( m_pPerformance );
	}

	CoUninitialize();
}

// Initialize function.
HRESULT				CMusicManager::Initialize(HWND hWnd, DWORD dwPChannels /* = 128 */, DWORD dwDefaultPathType /* = DMUS_APATH_DYNAMIC_STEREO */, LPDIRECTSOUND pDS /* = NULL  */)
{
	HRESULT			hr;
	if( FAILED( hr = CoInitialize( NULL ) ) )
		return DXTRACE_ERR_MSGBOX( TEXT("CoInitialize"), hr );

	m_lpDS = pDS;
	m_hWnd = hWnd;

	// Create loader object.
	if( FAILED( hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC,
									   IID_IDirectMusicLoader8, (void**)&m_pLoader ) ) )
		return DXTRACE_ERR_MSGBOX( TEXT( "CoCreateInstance : MusicLoader" ), hr );

	// Create performance object.
	if( FAILED( hr = CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC,
									   IID_IDirectMusicPerformance8, (void**)&m_pPerformance ) ) )
		return DXTRACE_ERR_MSGBOX( TEXT( "CoCreateInstance : Performance" ), hr );

	// Initialize the performance with the standard audio path.
	// This initializes both DirectMusic and DirectSound and
	// sets up the synthesizer, Typcially its easist to use an
	// audio path for playing music and sound effects.
	if( FAILED( hr = m_pPerformance->InitAudio( NULL, &m_lpDS, m_hWnd, dwDefaultPathType,
					 dwPChannels, DMUS_AUDIOF_ALL, NULL ) ) )
	{
		if( hr == DSERR_NODRIVER )
		{
			DXTRACE_ERR( TEXT("Warning : No sound ard found\n"), hr );
			return hr;
		}

		return DXTRACE_ERR_MSGBOX( TEXT("InitAudio" ), hr );
	}

	// Get the listener from the in the default audio path.
	IDirectMusicAudioPath8* pAudioPath = GetDefaultAudioPath();
	if( pAudioPath )	// might be NULL if dwDefaultPathType == 0
	{
		if( SUCCEEDED( hr = pAudioPath->GetObjectInPath( 0, DMUS_PATH_PRIMARY_BUFFER, 0,
							GUID_NULL, 0, IID_IDirectSound3DListener,
							(LPVOID*)&m_pDSListener ) ) )
		{
			// Get listener parameters.
			m_dsListenerParams.dwSize = sizeof( DS3DLISTENER );
			m_pDSListener->GetAllParameters( &m_dsListenerParams );
		}
	}

	m_pPerformance->AddNotificationType( GUID_NOTIFICATION_SEGMENT );

	return S_OK;
}

IDirectMusicAudioPath8*		CMusicManager::GetDefaultAudioPath()
{
	IDirectMusicAudioPath8* pAudioPath = NULL;
	if( NULL == m_pPerformance )
		return NULL;

	m_pPerformance->GetDefaultAudioPath( &pAudioPath );

	return pAudioPath;
}

// setting search directory.
HRESULT				CMusicManager::SetSearchDirectory(const char* strMediaPath )
{
	if( NULL == m_pLoader )
		return E_UNEXPECTED;
	if( NULL == strMediaPath )
		return E_INVALIDARG;

	// DMusic only takes wide strings.
	WCHAR wstrMediaPath[MAX_PATH] = { 0, };

	ConvertAnsiStringToWideCch( wstrMediaPath, strMediaPath, sizeof( WCHAR ) * MAX_PATH );

	return m_pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, wstrMediaPath, FALSE );
}

// all stop segment.
VOID				CMusicManager::StopAll()
{
	if( m_pPerformance )
		m_pPerformance->Stop( NULL, NULL, 0, 0 );
}

// called garbage collectors.
VOID				CMusicManager::CollectGarbage()
{
	if( m_pLoader )
		m_pLoader->CollectGarbage();
}

// create segment from file.
HRESULT				CMusicManager::CreateSegmentFromFile(CMusicSegment** ppSegment, char* strFileName, BOOL bDownloadNow /* = TRUE */, BOOL bIsMidiFile /* = FALSE  */)
{
	HRESULT			hr;
	IDirectMusicSegment8*	pSegment = NULL;

	// DMusic only takes wide strings.
	WCHAR wstrFileName[MAX_PATH] = {0,};

	ConvertAnsiStringToWideCch( wstrFileName, strFileName, sizeof(WCHAR)*MAX_PATH );

	if( FAILED( hr = m_pLoader->LoadObjectFromFile( CLSID_DirectMusicSegment,
					 IID_IDirectMusicSegment8,
					 wstrFileName,
					 (LPVOID*)&pSegment ) ) )
	{
		if( hr == DMUS_E_LOADER_FAILEDOPEN )
			return hr;
		return DXTRACE_ERR_MSGBOX( TEXT("LoadObjectFromFile"), hr );
	}

	*ppSegment = new CMusicSegment( m_pPerformance, m_pLoader, pSegment );
	if( !*ppSegment )
		return E_OUTOFMEMORY;

	if( bIsMidiFile )
	{
		if( FAILED( hr = pSegment->SetParam( GUID_StandardMIDIFile,
											 0XFFFFFFFF, 0, 0, NULL ) ) )
			return DXTRACE_ERR_MSGBOX( TEXT("SetParam: MIDI"), hr );
	}

	if( bDownloadNow )
	{
		if( FAILED( hr = (*ppSegment)->Download() ) )
			return DXTRACE_ERR_MSGBOX( TEXT("Download"), hr );
	}

	return S_OK;
}

HRESULT				CMusicManager::CreateSegmentFromFile(CMusicSegment** ppSegment, char* strFileName, BOOL bDownloadNow /* = TRUE  */)
{
	char szDrive[_MAX_DRIVE]	= {0,};
	char szPathDir[_MAX_DIR]	= {0,};
	char szFileName[_MAX_FNAME] = {0,};
	char szFileExt[_MAX_EXT]	= {0,};

	_splitpath( strFileName, szDrive, szPathDir, szFileName, szFileExt );

	if( !strcmpi( szFileExt, "mp3" ) )
	{
		// decompression mpeg codec. [10/19/2009 rumist]
		return CreateSegmentFromFile( ppSegment, strFileName, bDownloadNow, FALSE );
	}
	else if( !strcmpi( szFileExt, "midi" ) || !strcmpi( szFileExt, "mid" ) )
	{
		return CreateSegmentFromFile( ppSegment, strFileName, bDownloadNow, TRUE );
	}
	else	// suppose wave.
	{
		return CreateSegmentFromFile( ppSegment, strFileName, bDownloadNow, FALSE );
	}
}

HRESULT				CMusicManager::LoadSegmentFromFile(int& nSlotNo, char* strFileName, BOOL bDownloadNow /* = TRUE */, BOOL bIsMidiFile /* = FALSE  */)
{
	HRESULT			hr;
	IDirectMusicSegment8*	pSegment = NULL;

	// DMusic only takes wide strings.
	WCHAR wstrFileName[MAX_PATH] = {0,};

	ConvertAnsiStringToWideCch( wstrFileName, strFileName, sizeof(WCHAR)*MAX_PATH );

	if( FAILED( hr = m_pLoader->LoadObjectFromFile( CLSID_DirectMusicSegment,
					 IID_IDirectMusicSegment8,
					 wstrFileName,
					 (LPVOID*)&pSegment ) ) )
	{
		if( hr == DMUS_E_LOADER_FAILEDOPEN )
			return hr;
		return DXTRACE_ERR_MSGBOX( TEXT("LoadObjectFromFile"), hr );
	}

	CMusicSegment* ppSegment = new CMusicSegment( m_pPerformance, m_pLoader, pSegment );
	if( !ppSegment )
		return E_OUTOFMEMORY;

	if( bIsMidiFile )
	{
		if( FAILED( hr = pSegment->SetParam( GUID_StandardMIDIFile,
											 0XFFFFFFFF, 0, 0, NULL ) ) )
			return DXTRACE_ERR_MSGBOX( TEXT("SetParam: MIDI"), hr );
	}

	m_vecMSegments.push_back( ppSegment );
	nSlotNo = m_vecMSegments.size() - 1;

	if( bDownloadNow )
	{
		if( FAILED( hr = (ppSegment)->Download() ) )
			return DXTRACE_ERR_MSGBOX( TEXT("Download"), hr );
	}

	return S_OK;
}

HRESULT			CMusicManager::LoadSegmentFromFile(int& nSlotNo, char* strFileName, BOOL bDownloadNow /* = TRUE  */)
{
	char szDrive[_MAX_DRIVE]	= {0,};
	char szPathDir[_MAX_DIR]	= {0,};
	char szFileName[_MAX_FNAME] = {0,};
	char szFileExt[_MAX_EXT]	= {0,};

	_splitpath( strFileName, szDrive, szPathDir, szFileName, szFileExt );

	if( !strcmpi( szFileExt, "mp3" ) )
	{
		// decompression mpeg codec. [10/19/2009 rumist]
		return LoadSegmentFromFile( nSlotNo, strFileName, bDownloadNow, FALSE );
	}
	else if( !strcmpi( szFileExt, "midi" ) || !strcmpi( szFileExt, "mid" ) )
	{
		return LoadSegmentFromFile( nSlotNo, strFileName, bDownloadNow, TRUE );
	}
	else	// suppose wave.
	{
		return LoadSegmentFromFile( nSlotNo, strFileName, bDownloadNow, FALSE );
	}
}

// create segment from memory storage.
HRESULT				CMusicManager::CreateSegmentFromMemory(CMusicSegment** ppSegment, char* buf, int nBufSize, BOOL bDownloadNow /* = TRUE */, BOOL bIsMidiFile /* = FALSE  */)
{
	HRESULT					hr;
	IDirectMusicSegment8*	pSegment	= NULL;
	DMUS_OBJECTDESC			objdesc;

	ZeroMemory( &objdesc, sizeof( DMUS_OBJECTDESC ) );
	objdesc.dwSize		= sizeof( DMUS_OBJECTDESC );
	objdesc.dwValidData	= DMUS_OBJ_MEMORY | DMUS_OBJ_CLASS;
	objdesc.guidClass	= CLSID_DirectMusicSegment;
	objdesc.llMemLength	= (LONGLONG)nBufSize;
	objdesc.pbMemData	= (BYTE*)buf;

	if( FAILED( hr = m_pLoader->GetObject( &objdesc, IID_IDirectMusicSegment8,
										   (void**)&pSegment ) ) )
	{
		if( hr == DMUS_E_LOADER_FAILEDOPEN )
			return hr;
		return DXTRACE_ERR_MSGBOX( TEXT("LoadObjectFromFile"), hr );
	}

	*ppSegment = new CMusicSegment( m_pPerformance, m_pLoader, pSegment );
	if( NULL == *ppSegment )
		return E_OUTOFMEMORY;

	if( bIsMidiFile )
	{
		// Do this to make sure that the default General MIDI set
		// is connected appropriately to the MIDI file and
		// all instruments sound correct.
		if( FAILED( hr = pSegment->SetParam( GUID_StandardMIDIFile,
											 0xFFFFFFFF, 0, 0, NULL ) ) )
			return DXTRACE_ERR_MSGBOX( TEXT("SetParam"), hr );
	}

	if( bDownloadNow )
	{
		// The segment needs to be download first before playing.
		// However, some applications may want to wait before calling this
		// to because the download allocates memory for the
		// instruments. The more instruments currently downloaded,
		// the more memory is in use by the synthesizer.
		if( FAILED( hr = (*ppSegment)->Download() ) )
			return DXTRACE_ERR_MSGBOX( TEXT("Download"), hr );
	}

	return S_OK;
}

HRESULT				CMusicManager::CreateSegmentFromMemory(CMusicSegment** ppSegment, char* buf, int nBufSize, char* szFileExt, BOOL bDownloadNow /* = TRUE  */)
{
	if( !strcmpi( szFileExt, "mp3" ) )
	{
		// decompressed mp3 [10/19/2009 rumist]
		return CreateSegmentFromMemory( ppSegment, buf, nBufSize, bDownloadNow, FALSE );
	}
	else if( !strcmpi( szFileExt, "midi" ) || !strcmpi( szFileExt, "mid" ) )
	{
		return CreateSegmentFromMemory( ppSegment, buf, nBufSize, bDownloadNow, TRUE );
	}
	else // suppose wave file.
		return CreateSegmentFromMemory( ppSegment, buf, nBufSize, bDownloadNow, FALSE );
}

HRESULT				CMusicManager::LoadSegmentFromMemory(int& nSlot, char* buf, int nBufSize, BOOL bDownloadNow /* = TRUE */, BOOL bIsMidiFile /* = FALSE  */)
{
	HRESULT					hr;
	IDirectMusicSegment8*	pSegment	= NULL;
	DMUS_OBJECTDESC			objdesc;

	ZeroMemory( &objdesc, sizeof( DMUS_OBJECTDESC ) );
	objdesc.dwSize		= sizeof( DMUS_OBJECTDESC );
	objdesc.dwValidData	= DMUS_OBJ_MEMORY | DMUS_OBJ_CLASS;
	objdesc.guidClass	= CLSID_DirectMusicSegment;
	objdesc.llMemLength	= (LONGLONG)nBufSize;
	objdesc.pbMemData	= (BYTE*)buf;

	if( FAILED( hr = m_pLoader->GetObject( &objdesc, IID_IDirectMusicSegment8,
										   (void**)&pSegment ) ) )
	{
		if( hr == DMUS_E_LOADER_FAILEDOPEN )
			return hr;
		return DXTRACE_ERR_MSGBOX( TEXT("LoadObjectFromFile"), hr );
	}

	CMusicSegment* ppSegment = new CMusicSegment( m_pPerformance, m_pLoader, pSegment );
	if( NULL == ppSegment )
		return E_OUTOFMEMORY;

	if( bIsMidiFile )
	{
		// Do this to make sure that the default General MIDI set
		// is connected appropriately to the MIDI file and
		// all instruments sound correct.
		if( FAILED( hr = pSegment->SetParam( GUID_StandardMIDIFile,
											 0xFFFFFFFF, 0, 0, NULL ) ) )
			return DXTRACE_ERR_MSGBOX( TEXT("SetParam"), hr );
	}

	m_vecMSegments.push_back( ppSegment );
	nSlot = m_vecMSegments.size() - 1;

	if( bDownloadNow )
	{
		// The segment needs to be download first before playing.
		// However, some applications may want to wait before calling this
		// to because the download allocates memory for the
		// instruments. The more instruments currently downloaded,
		// the more memory is in use by the synthesizer.
		if( FAILED( hr = (ppSegment)->Download() ) )
			return DXTRACE_ERR_MSGBOX( TEXT("Download"), hr );
	}

	return E_FAIL;
}

HRESULT				CMusicManager::LoadSegmentFromMemory(int& nSlot, char* buf, int nBufSize, char* szFileExt, BOOL bDownloadNow /* = TRUE  */)
{
	if( !strcmpi( szFileExt, "mp3" ) )
	{
		// decompressed mp3 [10/19/2009 rumist]
		return LoadSegmentFromMemory( nSlot, buf, nBufSize, bDownloadNow, FALSE );
	}
	else if( !strcmpi( szFileExt, "mid" ) || !strcmpi( szFileExt, "midi" ) )
		return LoadSegmentFromMemory( nSlot, buf, nBufSize, bDownloadNow, TRUE );
	else // suppose wave file.
		return LoadSegmentFromMemory( nSlot, buf, nBufSize, bDownloadNow, FALSE );
}

HRESULT				CMusicManager::DecompressedMp3(unsigned char* outBuffer, unsigned char* inBuffer, size_t out_size, size_t in_size )
{
	return E_FAIL;

}

// set 3d parameter in listener.
VOID				CMusicManager::Set3DParameters(FLOAT fDistanceFactor, FLOAT fDopplerFactor, FLOAT fRolloffFactor )
{
	m_dsListenerParams.flDistanceFactor = fDistanceFactor;
	m_dsListenerParams.flDopplerFactor	= fDopplerFactor;
	m_dsListenerParams.flRolloffFactor	= fRolloffFactor;

	if( m_pDSListener )
		m_pDSListener->SetAllParameters( &m_dsListenerParams, DS3D_IMMEDIATE );
}

VOID				CMusicManager::SetVolumeLevelAll(long level )
{
	long chang_level = (-3000) +(30*level);

	if(level < 10)
		chang_level = -10000;


	// Adjust the slider position to match GUID_PerfMasterTempo range
	m_pPerformance->SetGlobalParam( GUID_PerfMasterVolume, (void*)&chang_level, sizeof(long) );
}

HRESULT				CMusicManager::Play(int nIndex, bool bRepeats )
{
	if( m_pPerformance == NULL || m_pLoader == NULL )
		return CO_E_NOTINITIALIZED;
	if( m_vecMSegments.empty() )
		return CO_E_NOTINITIALIZED;

	if( m_vecMSegments.size() <= nIndex )
		return E_OUTOFMEMORY;


	m_vecMSegments[nIndex]->SetRepeats( bRepeats ? DMUS_SEG_REPEAT_INFINITE : 0 );
	return m_vecMSegments[nIndex]->Play( DMUS_SEGF_BEAT );
}

HRESULT				CMusicManager::Stop(int nIndex )
{
	if( m_pPerformance == NULL || m_pLoader == NULL )
		return CO_E_NOTINITIALIZED;
	if( m_vecMSegments.empty() )
		return CO_E_NOTINITIALIZED;

	if( m_vecMSegments.size() <= nIndex )
		return E_OUTOFMEMORY;

	return m_vecMSegments[nIndex]->Stop(0);
}


//////////////////////////////////////////////////////////////////////////
// CMusicSegment class.
CMusicSegment::CMusicSegment(IDirectMusicPerformance8* pPerformance, IDirectMusicLoader8* pLoader, IDirectMusicSegment8* psegment ) :
	m_pLoader( NULL ),
	m_pPerformance( NULL ),
	m_pSegment( NULL ),
	m_pEmbeddedAudioPath( NULL ),
	m_bDownloaded( FALSE )
{
	m_pPerformance = pPerformance;
	m_pLoader = pLoader;
	m_pSegment = psegment;
	// Try to pull out an audio path from the segment itself if there is one.
	// This embedded audio path will be used instead of the default
	// audio path if the application doesn't wish to use an overriding audio path.
	IUnknown* pConfig = NULL;
	if( SUCCEEDED( m_pSegment->GetAudioPathConfig( &pConfig ) ) )
	{
		m_pPerformance->CreateAudioPath( pConfig, TRUE, &m_pEmbeddedAudioPath );
		SAFE_RELEASE( pConfig );
	}
}

CMusicSegment::~CMusicSegment()
{
	if( m_pSegment )
	{
		// Tell the loader that this object should now be released.
		if( m_pLoader )
			m_pLoader->ReleaseObjectByUnknown( m_pSegment );

		if( m_bDownloaded )
		{
			if( m_pEmbeddedAudioPath )
				m_pSegment->Unload( m_pEmbeddedAudioPath );
			else
				m_pSegment->Unload( m_pPerformance );
		}

		SAFE_RELEASE( m_pEmbeddedAudioPath );
		SAFE_RELEASE( m_pSegment );
	}

	m_pPerformance = NULL;
}

// set play mode.
HRESULT				CMusicSegment::SetRepeats(DWORD dwRepeats )
{
	if( m_pSegment == NULL )
		return CO_E_NOTINITIALIZED;

	return m_pSegment->SetRepeats( dwRepeats );
}

// play segment.
HRESULT				CMusicSegment::Play(DWORD dwFlags /* = DMUS_SEGF_SECONDARY */, IDirectMusicAudioPath8* pAudioPath /* = NULL  */)
{
	if( m_pSegment == NULL || m_pPerformance == NULL )
		return CO_E_NOTINITIALIZED;

	if( !m_bDownloaded )
		return E_FAIL;

	// If an audio path was passed in then use it, otherwise
	// use the embedded audio path if there was one.
	if( pAudioPath == NULL && m_pEmbeddedAudioPath != NULL )
		pAudioPath = m_pEmbeddedAudioPath;

	// If pAudioPath is NULL then this plays on the default audio path.
	return m_pPerformance->PlaySegmentEx( m_pSegment, 0, NULL, dwFlags, 0, 0, NULL, pAudioPath );
}

// stop segment.
HRESULT				CMusicSegment::Stop(DWORD dwFlags /* = 0  */)
{
	if( m_pSegment == NULL || m_pPerformance == NULL )
		return CO_E_NOTINITIALIZED;

	return m_pPerformance->Stop( m_pSegment, NULL, 0, dwFlags );
}

// download segment from other storage.
HRESULT				CMusicSegment::Download(IDirectMusicAudioPath8* pAudioPath /* = NULL  */)
{
	HRESULT		hr;
	if( m_pSegment == NULL )
		return CO_E_NOTINITIALIZED;

	// If no audio path was passed in, then download
	// to the embedded audio path if it exists
	// else download to the performance.
	if( pAudioPath == NULL )
	{
		if( m_pEmbeddedAudioPath )
			hr = m_pSegment->Download( m_pEmbeddedAudioPath );
		else
			hr = m_pSegment->Download( m_pPerformance );
	}
	else
		hr = m_pSegment->Download( pAudioPath );

	if( SUCCEEDED( hr ) )
		m_bDownloaded = TRUE;

	return hr;
}

HRESULT				CMusicSegment::Unload(IDirectMusicAudioPath8* pAudioPath /* = NULL  */)
{
	HRESULT		hr;
	if( m_pSegment == NULL )
		return CO_E_NOTINITIALIZED;

	// If no audio path was passed in, then unload
	// from the embedded audio path if it exists
	// else unload from the performance.
	if( pAudioPath == NULL )
	{
		if( m_pEmbeddedAudioPath )
			hr = m_pSegment->Unload( m_pEmbeddedAudioPath );
		else
			hr = m_pSegment->Unload( m_pPerformance );
	}
	else
		hr = m_pSegment->Unload( pAudioPath );

	if( SUCCEEDED( hr ) )
		m_bDownloaded = FALSE;

	return hr;
}

// query to playing.
BOOL				CMusicSegment::IsPlaying()
{
	if( m_pSegment == NULL || m_pPerformance == NULL )
		return FALSE;

	return ( m_pPerformance->IsPlaying( m_pSegment, NULL ) == S_OK );
}