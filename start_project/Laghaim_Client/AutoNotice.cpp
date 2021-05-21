#include "stdafx.h"
#include "AutoNotice.h"
#include "Nk2DFrame.h"

// 브라질 요청 : 10분 마다 공지 메시지 출력

extern	CNk2DFrame*		g_pNk2DFrame;

CAutoNotice::CAutoNotice()
{
	sprintf(m_strNotice[0], "N? divida sua conta com ningu?, este ato ?proibido e pass?el de Ban" );
	sprintf(m_strNotice[1], "Nunca baixe programas suspeitos, eles podem conter Keyloger" );
	sprintf(m_strNotice[2], "Denuncie jogadores suspeitos ou que se passem por GM" );
	sprintf(m_strNotice[3], "GM jamais pedir?seu Login, Senha e E-mail no jogo" );
	sprintf(m_strNotice[4], "O suporte LastWar ?disponibilizado apenas pelo HelpDesk do site oficial" );
	sprintf(m_strNotice[5], "Tenha sempre seu anti-v?us atualizado juntamente com anti-spy" );

	m_Count			= 0;
	m_bActive		= TRUE;
	m_dwBeforeTime	= timeGetTime();
	SetTime(10);
}


CAutoNotice::~CAutoNotice()
{
}


HRESULT	CAutoNotice::MsgProc()
{
	if( timeGetTime() - m_dwBeforeTime >= m_DelayTime )
	{
		m_dwBeforeTime = timeGetTime();
		g_pNk2DFrame->SetInfoBox(m_strNotice[m_Count]);
		g_pNk2DFrame->AddChatStr(m_strNotice[m_Count], 4);

		m_Count ++;
		if( m_Count >= AUTONOTICE_MAX_COUNT )
			m_Count = 0;
	}

	SetTime(10);
	return S_OK;
}