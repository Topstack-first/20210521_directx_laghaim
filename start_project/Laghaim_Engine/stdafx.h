#ifndef _STDAFX_H_
#define _STDAFX_H_

#define POINTER_64

#pragma warning(disable: 4996)

#include <WinSock.h>
#include <Windows.h>
#include <stdio.h>

#include <list>
#include <vector>
#include <string>
#include <map>

#include "FileManager.h"
#include "Def.h"

extern long getFileSize(PFILE* fp);
extern void readData(void* ptr, size_t size, size_t n, unsigned char** stream);

#endif //_STDAFX_H_