#ifndef _SPEED_CHECK_H_
#define _SPEED_CHECK_H_

#include <boost/chrono.hpp>
#include "singleton.h"

class SpeedChecker : public CSingleton<SpeedChecker>
{
public:
	SpeedChecker();
	~SpeedChecker();

public:
	void StartCheck();
	void Reset();
	void FrameMove();

private:	
	void sendPacket();

private:
	bool	m_isChecking;
	boost::chrono::steady_clock::time_point m_timeStart;
	boost::chrono::steady_clock::time_point m_timeCur;
};

#endif // _SPEED_CHECK_H_
