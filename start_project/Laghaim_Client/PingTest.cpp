#include "stdafx.h"

#include "PingTest.h"
#include "ddutil.h"
#include "tcpipcon.h"

#include "Nk2DFrame.h"






CPingTest::CPingTest()
{
	init();
}

CPingTest::~CPingTest() {}

void CPingTest::init()
{
	m_beforTime = 0;
	m_nCount	= 0;
	m_avgTime	= 0;
}

void CPingTest::start()
{
	if( m_nCount > 0)
		return ;

	init();
	m_beforTime = timeGetTime();

	g_pTcpIp->SendNetMessage("callback m pingtest 0 \n");
	m_nCount++;
}

void CPingTest::check()
{
	if( m_nCount == 11 )
	{
		draw();
		init();
		return;
	}

	DWORD avgTime = 0;

	avgTime = timeGetTime() - m_beforTime;
	m_beforTime = timeGetTime();
	m_avgTime += avgTime;

	char temp[64];
	sprintf( temp, "[Ping Test %d]%6d (Milli-seconds)",m_nCount-1, (int)avgTime);
	g_pNk2DFrame->AddChatStr(temp ,-1);

	g_pTcpIp->SendNetMessage("callback m pingtest 0 \n");
	m_nCount ++;
}




void CPingTest::draw()
{
	char temp[65];

	sprintf( temp, "[Ping AVG]   %6d (Milli-seconds)", m_avgTime / 10);
	g_pNk2DFrame->AddChatStr("----------------------------------------" ,-1);
	g_pNk2DFrame->AddChatStr(temp ,-1);
}