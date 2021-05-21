#if !defined(_FAMILY_EVENT_)
#define _FAMILY_EVENT_

///이벤트버프표시

#include "d3dutil.h"

#pragma once

enum EVENT_TYPE
{
	TYPE_DROP_PER	= 0,
	TYPE_ATTACK_PER,
	TYPE_DEFENSE_PER,
	TYPE_STRENGTH_PER,
	TYPE_EXP_PER,
	TYPE_STATUS_TOTAL
};

enum EVENT_FAMILY_IMAGE_FRAME
{
	EF_IMAGE_FRAME_1,
	EF_IMAGE_FRAME_2,
	EF_IMAGE_FRAME_3,
	EF_IMAGE_FRAME_4,
	EF_IMAGE_FRAME_5,
	EF_IMAGE_FRAME_6,
	EF_IMAGE_FRAME_TOTAL
};

const unsigned int FLAG_DROP_PER		= 1;
const unsigned int FLAG_ATTACK_PER		= 1<<1;
const unsigned int FLAG_DEFENSE_PER		= 1<<2;
const unsigned int FLAG_STRENGTH_PER	= 1<<3;
const unsigned int FLAG_EXP_PER			= 1<<4;


class IndexedTexture;

class CFamilyEvent
{
public:
	static CFamilyEvent* CreateInstance(int nType);
	~CFamilyEvent();

private:

	EVENT_TYPE m_eType;

	int	 m_nTgaWindowNum[EF_IMAGE_FRAME_TOTAL*TYPE_STATUS_TOTAL];
	int	 m_nNowRender;
	int  m_nFade;

	int  m_Alpha;

	DWORD m_dwStartTime;

	IndexedTexture* m_pIScene;

	//	D3DTLVERTEX TLVec[4];

	float m_fWidth;
	float m_fHeight;

	float m_fPosX;
	float m_fPosY;


	unsigned int m_event_flagset;

public:
	void	LoadTgaResource();
	void	DeleteRes();
	int		Render();
	int		FrameMove();

	void	SetEventType(int nType)
	{
		m_eType = (EVENT_TYPE)nType;
	}
	int		GetEventType() const
	{
		return (int)m_eType;
	}
	void	SetEventType(int type, int flag);

	void	SetFadeIn()
	{
		m_nFade = 1;
	}
	void	SetFadeOut()
	{
		m_nFade = -1;
	}


private:
	CFamilyEvent(int nType);

};


#endif // _FAMILY_EVENT_