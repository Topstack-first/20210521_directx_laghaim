#ifndef _PING_TEST_H_
#define _PING_TEST_H_

#include "Config.h"



#define PINGTEST_MAX 10
class CPingTest
{
public:

	DWORD	m_beforTime;
	DWORD	m_avgTime;
	int		m_nCount;

	CPingTest();
	~CPingTest();

	void init();
	void start();
	void check();
	void draw();
};



#endif