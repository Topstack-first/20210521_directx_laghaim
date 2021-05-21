#include "stdafx.h"
#include "SpeedCheck.h"
#include "Tcpipcon.h"

SpeedChecker::SpeedChecker()
	: m_isChecking(false)	
{
}

SpeedChecker::~SpeedChecker()
{
}

void SpeedChecker::StartCheck()
{
	m_isChecking = true;
	DWORD_PTR dOldMask = SetThreadAffinityMask(GetCurrentThread(), 1);
	m_timeStart = boost::chrono::steady_clock::now();
	SetThreadAffinityMask(GetCurrentThread(), dOldMask);
	m_timeCur = m_timeStart;
}

void SpeedChecker::Reset()
{
	m_isChecking = false;
}

void SpeedChecker::FrameMove()
{
	if( m_isChecking == false )
	{
		return;
	}
	
	DWORD_PTR dOldMask = SetThreadAffinityMask(GetCurrentThread(), 1);
	m_timeCur = boost::chrono::steady_clock::now();
	SetThreadAffinityMask(GetCurrentThread(), dOldMask);
	
	boost::chrono::milliseconds timeSub = boost::chrono::duration_cast<boost::chrono::milliseconds>(m_timeCur - m_timeStart);
	
	if( timeSub.count() > 10100 )
	{
		sendPacket();
		Reset();
	}	
}

void SpeedChecker::sendPacket()
{
	g_pTcpIp->SendNetMessage("speedcheck\n");
}
