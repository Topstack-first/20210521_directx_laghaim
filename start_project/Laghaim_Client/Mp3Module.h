#ifndef _MP3_MODULE_H_
#define _MP3_MODULE_H_

#include "Config.h"

//#include "stdafx.h"
#include <strmif.h>
#include <dmusici.h>
#include <dshow.h>
#include <malloc.h>



class CMp3Module
{
protected:

public:
	CMp3Module();
	~CMp3Module();

public:


	bool IsPlaying();
	bool Stop();
	bool Play(DWORD dwNumOfRepeats = 0);
	bool LoadSound(const char* szSoundFilePath);
	void InitialiseForMP3();
	HRESULT SwapSourceFilter(const char* szSoundFileName);


	DWORD m_dwFrames;
	long cur_vol;

	enum Format {Unknown, MP3, WavMidi};

	IGraphBuilder* m_pGraph;
	IMediaControl* m_pMediaControl;
	IMediaPosition* m_pMediaPosition;
	Format m_enumFormat;

	IBasicAudio *m_pBA;

	IBaseFilter   *m_pSourceCurrent;
	IBaseFilter   *m_pSourceNext;
	IMediaSeeking *m_pMediaSeeking;

	char* szSoundFileName;
};

#endif // _MP3_MODULE_H_