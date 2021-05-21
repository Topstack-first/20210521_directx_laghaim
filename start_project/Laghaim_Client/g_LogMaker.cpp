#include "stdafx.h"
//----------------------------------------------------------------------------
//-- Error Log System
//----------------------------------------------------------------------------
//-- 2004.11
//-- Lyul
//-- Debug 할 수 없는 특정 Error 의 경우에 사용하는 System
//-- Error Log 를 남기는 루틴이다. 유저용 Client 패치에 포함되지 않도록 사용 후에는 모든 루틴을 제거 한다.
//-- 특정 알림 부분에서 CreateFile 을 호출하여 시작을 알리고
//-- 필요 부분에서 Addlog 를 호출하여 log 를 기록 할 수 있다.

#include "g_LogMaker.h"
#include <fstream>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


void G_LOG_CreateFile()
{
	char title1[] = "-------------------------------------------------------------------------\n";
	char title2[] = "Error log ver 0.9 Final!!!!!! \n";


	std::fstream createfile;
	createfile.open("Error.log", std::ios::trunc );

	createfile.write(title1,strlen(title1));
	createfile.write(title2,strlen(title2));


	createfile.close();
}

void G_LOG_AddLog(char* string, int address , int val2)
{
	char temp[500] = {0,};

	std::ofstream outfile;
	outfile.open("Error.log", std::ios::app );
	sprintf(temp,"%s  -  address: %x /value: %d\n",string, address, val2);
	outfile.write(temp,strlen(temp));

	outfile.close();
}