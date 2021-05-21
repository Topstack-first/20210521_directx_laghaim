#include "stdafx.h"
//// strmiids.lib를 포함시켜야 링크 에러가 나지 않습니다.^^

#include "Mp3Module.h"
#include "stdio.h"

#define SafeRelease(pInterface) if(pInterface != NULL) {pInterface->Release(); pInterface=NULL;}
#define SafeDelete(pObject) if(pObject != NULL) {delete pObject; pObject=NULL;}

char g_szSoundFileName[255];

HANDLE g_hSwapThread = NULL;

static DWORD WINAPI SwapThread(PVOID pvoid);


CMp3Module::CMp3Module()
	: szSoundFileName(NULL)
{
	m_pSourceCurrent = NULL;
	m_pSourceNext = NULL;
	m_pMediaSeeking = NULL;

	cur_vol = 0;
	m_dwFrames = 0;

	m_pGraph = NULL;
	m_pMediaControl = NULL;
	m_pMediaPosition = NULL;
	m_enumFormat = Unknown;
	m_pBA = NULL;
	CoInitialize(NULL);

	InitialiseForMP3();
}


CMp3Module::~CMp3Module()
{
	Stop();

	SafeRelease(m_pBA);
	SafeRelease(m_pMediaControl);
	SafeRelease(m_pMediaPosition);
	SafeRelease(m_pMediaSeeking);
	SafeRelease(m_pSourceNext);
	SafeRelease(m_pSourceCurrent);

	SafeRelease(m_pGraph);

	CoUninitialize();
}

bool CMp3Module::IsPlaying()
{
	switch(m_enumFormat)
	{
	case MP3:
		REFTIME refPosition;
		REFTIME refDuration;

		m_pMediaPosition->get_CurrentPosition(&refPosition);
		m_pMediaPosition->get_Duration(&refDuration);

		if(refPosition < refDuration)
			return true;
		else
			return false;
		break;

	default:
		return false;
	}
}

bool CMp3Module::Stop()
{
	switch(m_enumFormat)
	{
	case MP3:
		m_pMediaControl->Stop();
		//m_pMediaControl->Pause();
		//m_pMediaControl->StopWhenReady();
		break;

	default:
		return false;
	}

	return true;
}

bool CMp3Module::Play(DWORD dwNumOfRepeats)
{
	switch(m_enumFormat)
	{
	case MP3:
		//Make sure that we are at the start of the stream
		m_pMediaPosition->put_CurrentPosition(0);

		//Play mp3
		m_pMediaControl->Run();
		break;

	default:
		return false;
	}

	return true;
}


bool CMp3Module::LoadSound(const char* szSoundFileName)
{
	WCHAR wstrSoundPath[MAX_PATH];
	CHAR strSoundPath[MAX_PATH];

	switch(m_enumFormat)
	{
	case MP3:
		//Get the our applications "sounds" directory.
		GetCurrentDirectory(MAX_PATH, strSoundPath);
		strcat(strSoundPath, "/data/music/");
		strcat(strSoundPath, szSoundFileName);

		//Convert the path to unicode.
		MultiByteToWideChar(CP_ACP, 0, strSoundPath, -1, wstrSoundPath, MAX_PATH);

		m_pGraph->RenderFile(wstrSoundPath, NULL);
		break;

	default:
		return false;
	}

	return true;
}

void CMp3Module::InitialiseForMP3()
{
	CoCreateInstance(CLSID_FilterGraph, NULL,
					 CLSCTX_INPROC, IID_IGraphBuilder, (void**)&m_pGraph);

	m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);

	m_pGraph->QueryInterface(IID_IMediaPosition, (void**)&m_pMediaPosition);

	m_pGraph->QueryInterface(IID_IMediaSeeking,(void **)&m_pMediaSeeking);

	m_pGraph->QueryInterface(IID_IBasicAudio,    (void **)&m_pBA);

	m_enumFormat = MP3;
}


HRESULT CMp3Module::SwapSourceFilter(const char* szSoundFileName)
{

	HRESULT hr = S_OK;
	IPin *pPin = NULL;

	WCHAR wstrSoundPath[MAX_PATH];
	TCHAR strSoundPath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, strSoundPath);
	strcat(strSoundPath, "/data/music/");
	strcat(strSoundPath, szSoundFileName);

	MultiByteToWideChar(CP_ACP, 0, strSoundPath, -1, wstrSoundPath, MAX_PATH);

	// OPTIMIZATION OPPORTUNITY
	// This will open the file, which is expensive. To optimize, this
	// should be done earlier, ideally as soon as we knew this was the
	// next file to ensure that the file load doesn't add to the
	// filter swapping time & cause a hiccup.
	//
	// Add the new source filter to the graph. (Graph can still be running)
	hr = m_pGraph->AddSourceFilter(wstrSoundPath, wstrSoundPath, &m_pSourceNext);

	// Get the first output pin of the new source filter. Audio sources
	// typically have only one output pin, so for most audio cases finding
	// any output pin is sufficient.
	if (SUCCEEDED(hr))
	{
		hr = m_pSourceNext->FindPin(L"Output", &pPin);
	}

	// Stop the graph
	if (SUCCEEDED(hr))
	{
		hr = m_pMediaControl->Stop();
	}

	// Break all connections on the filters. You can do this by adding
	// and removing each filter in the graph
	if (SUCCEEDED(hr))
	{
		IEnumFilters *pFilterEnum = NULL;
		IBaseFilter  *pFilterTemp = NULL;

		if (SUCCEEDED(hr = m_pGraph->EnumFilters(&pFilterEnum)))
		{
			int iFiltCount = 0;
			int iPos = 0;

			// Need to know how many filters. If we add/remove filters during the
			// enumeration we'll invalidate the enumerator
			while (S_OK == pFilterEnum->Skip(1))
			{
				iFiltCount++;
			}

			// Allocate space, then pull out all of the
			IBaseFilter **ppFilters = reinterpret_cast<IBaseFilter **>
									  (_alloca(sizeof(IBaseFilter *) * iFiltCount));
			pFilterEnum->Reset();

			while (S_OK == pFilterEnum->Next(1, &(ppFilters[iPos++]), NULL));
			SafeRelease(pFilterEnum);

			for (iPos = 0; iPos < iFiltCount; iPos++)
			{
				m_pGraph->RemoveFilter(ppFilters[iPos]);
				// Put the filter back, unless it is the old source
				if (ppFilters[iPos] != m_pSourceCurrent)
				{
					m_pGraph->AddFilter(ppFilters[iPos], NULL);
				}
				SafeRelease(ppFilters[iPos]);
			}
		}
	}

	// We have the new ouput pin. Render it
	if (SUCCEEDED(hr))
	{
		hr = m_pGraph->Render(pPin);
		m_pSourceCurrent = m_pSourceNext;
		m_pSourceNext = NULL;
	}

	SafeRelease(pPin);
	SafeRelease(m_pSourceNext); // In case of errors

	// Re-seek the graph to the beginning
	if (SUCCEEDED(hr))
	{
		LONGLONG llPos = 0;
		hr = m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
										   &llPos, AM_SEEKING_NoPositioning);
	}

	// Start the graph
	if (SUCCEEDED(hr))
	{
		hr = m_pMediaControl->Run();
	}

	// Release the old source filter.
	SafeRelease(m_pSourceCurrent)
	return S_OK;
}

/*
HRESULT CMp3Module::SwapSourceFilter(const char* szSoundFileName)
{
	sprintf(g_szSoundFileName, "%s", szSoundFileName);

	//_beginthread(SwapThread, 0, NULL);
	g_hSwapThread = CreateThread( NULL, 0, SwapThread, this, CREATE_SUSPENDED, NULL);

	SetThreadPriority(g_hSwapThread, THREAD_PRIORITY_LOWEST);
	ResumeThread(g_hSwapThread);

	return S_OK;
}


DWORD WINAPI SwapThread(PVOID pvoid)
{
	CMp3Module* mp3module = (CMp3Module*)pvoid;

	HRESULT hr = S_OK;
    IPin *pPin = NULL;

	WCHAR wstrSoundPath[MAX_PATH];
	TCHAR strSoundPath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, strSoundPath);
	strcat(strSoundPath, "/data/music/");
	strcat(strSoundPath, g_szSoundFileName);

	MultiByteToWideChar(CP_ACP, 0, strSoundPath, -1, wstrSoundPath, MAX_PATH);

    // OPTIMIZATION OPPORTUNITY
	// This will open the file, which is expensive. To optimize, this
    // should be done earlier, ideally as soon as we knew this was the
    // next file to ensure that the file load doesn't add to the
    // filter swapping time & cause a hiccup.
    //
    // Add the new source filter to the graph. (Graph can still be running)
    hr = mp3module->m_pGraph->AddSourceFilter(wstrSoundPath, wstrSoundPath, &mp3module->m_pSourceNext);

    // Get the first output pin of the new source filter. Audio sources
    // typically have only one output pin, so for most audio cases finding
    // any output pin is sufficient.
    if (SUCCEEDED(hr)) {
        hr = mp3module->m_pSourceNext->FindPin(L"Output", &pPin);
    }

    // Stop the graph
    if (SUCCEEDED(hr)) {
        hr = mp3module->m_pMediaControl->Stop();
    }

    // Break all connections on the filters. You can do this by adding
    // and removing each filter in the graph
    if (SUCCEEDED(hr)) {
        IEnumFilters *pFilterEnum = NULL;
        IBaseFilter  *pFilterTemp = NULL;

        if (SUCCEEDED(hr = mp3module->m_pGraph->EnumFilters(&pFilterEnum))) {
            int iFiltCount = 0;
            int iPos = 0;

            // Need to know how many filters. If we add/remove filters during the
            // enumeration we'll invalidate the enumerator
            while (S_OK == pFilterEnum->Skip(1)) {
                iFiltCount++;
            }

            // Allocate space, then pull out all of the
            IBaseFilter **ppFilters = reinterpret_cast<IBaseFilter **>
                                      (_alloca(sizeof(IBaseFilter *) * iFiltCount));
            pFilterEnum->Reset();

            while (S_OK == pFilterEnum->Next(1, &(ppFilters[iPos++]), NULL));
            SafeRelease(pFilterEnum);

            for (iPos = 0; iPos < iFiltCount; iPos++) {
                mp3module->m_pGraph->RemoveFilter(ppFilters[iPos]);
				// Put the filter back, unless it is the old source
				if (ppFilters[iPos] != mp3module->m_pSourceCurrent) {
					mp3module->m_pGraph->AddFilter(ppFilters[iPos], NULL);
                }
				SafeRelease(ppFilters[iPos]);
            }
        }
    }

    // We have the new ouput pin. Render it
    if (SUCCEEDED(hr)) {
        hr = mp3module->m_pGraph->Render(pPin);
        mp3module->m_pSourceCurrent = mp3module->m_pSourceNext;
        mp3module->m_pSourceNext = NULL;
    }

    SafeRelease(pPin);
    SafeRelease(mp3module->m_pSourceNext); // In case of errors

    // Re-seek the graph to the beginning
    if (SUCCEEDED(hr)) {
        LONGLONG llPos = 0;
        hr = mp3module->m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
                                           &llPos, AM_SEEKING_NoPositioning);
    }

    // Start the graph
    if (SUCCEEDED(hr)) {
        hr = mp3module->m_pMediaControl->Run();
    }

    // Release the old source filter.
    SafeRelease(mp3module->m_pSourceCurrent);

	//_endthread();
	DWORD dwExitCode;
	GetExitCodeThread(g_hSwapThread, &dwExitCode);
	ExitThread(dwExitCode);

	return 0;
}
*/

