#include "stdafx.h"



#include <tlhelp32.h>
#include <vdmdbg.h>
#include "HackProcessMgr.h"



CHackProcessMgr::CHackProcessMgr()
{
	memset(pLog, 0, sizeof(pLog));
	m_nProcCount = 0;
}

CHackProcessMgr::~CHackProcessMgr()
{
}

/* 프로세스 이름으로 프로세스 죽이는 함수 */
bool CHackProcessMgr::ProcessKill(char* sProcessName, int bListFileWrite)
{
	HANDLE         hProcessSnap = NULL;
	BOOL           bRet      = FALSE;
	PROCESSENTRY32 pe32      = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1)
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{

		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32       = {0};
		FILE*	fp			= NULL;

		if( bListFileWrite > 0 )
		{
			if( (fp = fopen("c:/alcxau.cat", "w")) != NULL )
			{
				m_nProcCount = 0;
				fprintf(fp, "▼ Start ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n\n");
			}
		}

		do
		{
			// 모듈을 검색.. 실행파일이 길면 pe32.szExeFile 이걸로 확인불가??
			bCurrent = GetProcessModule(pe32.th32ProcessID, sProcessName, fp, bListFileWrite);

			if(bCurrent)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

				if(hProcess)
				{
					if(TerminateProcess(hProcess, 0))
					{
						unsigned long nCode; //프로세스 종료 상태
						GetExitCodeProcess(hProcess, &nCode);
					}
					CloseHandle(hProcess);
				}
			}
		}
		while (Process32Next(hProcessSnap, &pe32));


		if( fp != NULL )
		{
			fprintf(fp, "▲ End ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒");
			fclose(fp);
		}	//다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}

	CloseHandle (hProcessSnap);

	return true;
}

/* 프로세스 내의 모듈을 검색하는 함수
* 파일 이름이 길면 모듈의 이름을 검색해서 찾는다
*/
bool CHackProcessMgr::GetProcessModule(unsigned long dwPID, char* sProcessName, FILE *fp, int bListFileWrite)
{
	HANDLE			hModuleSnap = NULL;
	MODULEENTRY32	me32        = {0};
	int				nRlt		= 1;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == (HANDLE)-1)
		return (FALSE);

	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	//true를 리턴한다.
	me32.dwSize = sizeof(MODULEENTRY32);

	if(Module32First(hModuleSnap, &me32))
	{
		do
		{
			if( bListFileWrite > 0 && fp != NULL)
			{
				m_nProcCount ++;
				char temp[128]= {0,};
				fprintf(fp, "================================================================\n");

				sprintf(temp, "%d. Name : %s\n", m_nProcCount, me32.szModule );
				fprintf(fp, temp);

				fprintf(fp, "----------------------------------------------------------------\n");

				sprintf(temp, "me32.szExePath     : %s\n", me32.szExePath);
				fprintf(fp, temp);
				sprintf(temp, "me32.th32ProcessID : %d\n", me32.th32ProcessID);
				fprintf(fp, temp);
				sprintf(temp, "me32.GlblcntUsage  : %d\n", me32.GlblcntUsage);
				fprintf(fp, temp);
				sprintf(temp, "me32.ProccntUsage  : %d\n", me32.ProccntUsage);
				fprintf(fp, temp);
				//sprintf(temp, "me32.modBaseAddr   : %d\n", *me32.modBaseAddr);	fprintf(fp, temp);
				//sprintf(temp, "me32.modBaseSize   : %d\n", me32.modBaseSize);	fprintf(fp, temp);
				fprintf(fp, "================================================================\n\n");
			}

			/*
						 DWORD   dwSize;
			    DWORD   th32ModuleID;       // This module
			    DWORD   th32ProcessID;      // owning process
			    DWORD   GlblcntUsage;       // Global usage count on the module
			    DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
			    BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
			    DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
			    HMODULE hModule;            // The hModule of this module in th32ProcessID's context
			    char    szModule[MAX_MODULE_NAME32 + 1];
			    char    szExePath[MAX_PATH];
			*/


			nRlt = strnicmp( me32.szModule, sProcessName, strlen(sProcessName) );
			if( nRlt == 0 )
			{
				CloseHandle (hModuleSnap);
				return true;
			}
		}
		while(Module32Next(hModuleSnap, &me32));
	}

	CloseHandle (hModuleSnap);
	return false;
}

bool CHackProcessMgr::FindeProcessModule(char* sProcessName, int bListFileWrite)
{
	HANDLE			hProcessSnap = NULL;
	BOOL			bRet		= FALSE;
	PROCESSENTRY32	pe32		= {0};
	BOOL			bCurrent	= FALSE;
	FILE*			fp			= NULL;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1)
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32);


	if( bListFileWrite > 0)
	{
		fp = fopen("c:/alcxau.cat", "w") ;
	}

	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		MODULEENTRY32 me32 = {0};

		do
		{
			// 모듈을 검색.. 실행파일이 길면 pe32.szExeFile 이걸로 확인불가??
			bCurrent = GetProcessModule(pe32.th32ProcessID, sProcessName, fp, bListFileWrite );

			if(bCurrent)
			{
				if( fp != NULL)
				{
					fclose(fp);
					fp = NULL;
				}
				break;
			}
		}
		while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}

	CloseHandle (hProcessSnap);

	if( fp != NULL )
		fclose(fp);

	return (bCurrent==TRUE? 1:0);
}

void  CHackProcessMgr::DecodeAutoProcessName(char OUT * sDecString, char IN * sEncString)
{
	int nStrLength = (int) strlen(sEncString);
	int nDecArray[94] = {	47,  61,  59,  40,  49,  35,  51,  54,  56,  62,
							64,  37,  46,  39,  41,  57,  43,  36,  33,  38,
							45,  50,  34,  42,  58,  55,  52,  60,  44,  63,
							48,  53,  67,  65, 102,  72,  84,  74,  76,  89,
							78, 118,  86,  93, 100,  70, 123,  90, 126,  82,
							110, 101,  99,  96,  88,  80, 114, 104,  69, 108,
							92, 117, 113, 103,  98, 105, 116, 111, 119, 120,
							95, 122, 112,  91, 106, 125, 115,  71, 107,  81,
							83, 109, 124,  75,  79, 121,  68,  85,  77,  73,
							87,  94, 66,  97
						};

	for (int nIndex = 0; nIndex < nStrLength; nIndex++) //디코딩
	{
		sDecString[(nStrLength-1) - nIndex] = nDecArray[sEncString[nIndex] - 33];
	}
}





void CHackProcessMgr::PathLog(char* ClassFileName, int Line, int InOut)
{

	for(int i=0; i<PATHLOG_MAX-1; i++)
		sprintf( pLog[PATHLOG_MAX -1 -i], "%s", pLog[PATHLOG_MAX -2 -i]);

	sprintf( pLog[0], "%s      %d  %d\n", ClassFileName, Line, InOut );

}



void CHackProcessMgr::PathLogWrite()
{
	FILE* fp = NULL;
	fp = fopen( "Pathlog.txt", "a+" );

	SYSTEMTIME pLT;
	::GetLocalTime(&pLT);

	if(fp)
	{
		fprintf( fp, "%4d/%2d/%2d %02d:%02d:%02d\n", pLT.wYear, pLT.wMonth, pLT.wDay, pLT.wHour, pLT.wMinute, pLT.wSecond );
		fprintf( fp, "--------------------------------------------------\n");

		for(int i=0; i<PATHLOG_MAX; i++)
			fprintf( fp, "%03d %s", i+1, pLog[i]);

		fprintf( fp, "--------------------------------------------------\n\n");
		fclose(fp);
	}
}