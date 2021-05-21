#ifndef _STDAFX_H_
#define _STDAFX_H_

#define POINTER_64
#define _ASSERT_OK

#pragma warning(disable: 4996)
#pragma warning(disable: 4200)

#include <WinSock.h>
#include <Windows.h>
#include <stdio.h>
#include <yvals.h>

#include <list>
#include <vector>
#include <string>
#include <map>

#include "Config.h"

extern void LogBugTrap(std::string strLog);

#endif//_STDAFX_H_