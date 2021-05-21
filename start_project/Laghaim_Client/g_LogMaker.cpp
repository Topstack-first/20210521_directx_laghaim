#include "stdafx.h"
//----------------------------------------------------------------------------
//-- Error Log System
//----------------------------------------------------------------------------
//-- 2004.11
//-- Lyul
//-- Debug �� �� ���� Ư�� Error �� ��쿡 ����ϴ� System
//-- Error Log �� ����� ��ƾ�̴�. ������ Client ��ġ�� ���Ե��� �ʵ��� ��� �Ŀ��� ��� ��ƾ�� ���� �Ѵ�.
//-- Ư�� �˸� �κп��� CreateFile �� ȣ���Ͽ� ������ �˸���
//-- �ʿ� �κп��� Addlog �� ȣ���Ͽ� log �� ��� �� �� �ִ�.

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