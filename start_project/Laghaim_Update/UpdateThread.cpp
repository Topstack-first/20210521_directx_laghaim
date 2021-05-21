#include "stdafx.h"
#include "UpdateThread.h"
#include "AutoUpgrade.h"
//#include "CapExtract.h"
#include "unzip.h"
#include <time.h>
//#include "global.h"
#include "NfsVirtualFile.h"
#include <TlHelp32.h>
#include "UrlFile.h"
#define SAFE_RELEASE( p )	if( p ) delete p


extern int do_extract(unzFile uf, const char *target_dir, int opt_extract_without_path,
					  int opt_overwrite, int ver_idx, int ver_diff);



// nate 2006-03-30 : 파일 패치 - 파일 다운로드와 압축 해제
bool PatchFile(char* szConnectIp, CAutoUpgrade* pUpdate, int ver_idx, int ver_diff, char* szFile)
{
	int fail_count = 0;
	BOOL bSuccess = TRUE;
	unzFile uf=NULL;

	while (fail_count < 3)
	{
		int ret = 0;

		if ((ret = pUpdate->Connection(szConnectIp , szFile, ver_idx, ver_diff)) == FALSE)
		{
			return true;
		}
		else if (ret < 0)
		{
			fail_count++;
		}
		else
			fail_count = 3;
	}

	uf = unzOpen(szFile);

	if (uf==NULL)
	{
		bSuccess = FALSE;
	}
	else if (!do_extract(uf, "", 0, 1, ver_idx, ver_diff))
	{
		unzClose(uf);
		bSuccess = FALSE;
	}
	else
		unzClose(uf);

	TRY
	{
		CFile::Remove(szFile);
	}
	CATCH_ALL(error)
	{
	}
	END_CATCH_ALL;

	FILE* fp = fopen("Data_patch.lrf", "r" );

	if( fp )
	{
		fclose(fp);
		NfsVirtualFile * pVFile = new NfsVirtualFile();
		if( !pVFile->fileopen( "Data", true ) )
		{
			printf("BaseData File Open failed!!\n");
			SAFE_RELEASE( pVFile );
			exit(1);
		}

		pVFile->marge("Data_patch");
		pVFile->fileclose();
	}

	fp = fopen("Data_patch.lrf", "r" );
	if( fp )
	{
		fclose(fp);
		remove("Data_patch.lrf");
	}
	fp = fopen("Data_patch.lrs", "r" );
	if( fp )
	{
		fclose(fp);
		remove("Data_patch.lrs");
	}
	
	if (!bSuccess)
		return false;

	return true;
}

bool PatchProc(bool repatch)
{
	char ConnectIp[500] = {0,};	
	CAutoUpgrade upgrade;

	if (!ConnectPatchServer(ConnectIp, &upgrade))
	{
		return false;
	}

//	int newVersion = VersionCheck(ConnectIp, &upgrade, VERSION_FILE);
	char* arg = upgrade.Lead(ConnectIp, VERSION_FILE);
	if (!arg)
	{
		return false;
	}
	
	int newVersion = atoi(arg);

	if (newVersion < 0)
		return false;

	if( repatch == true )
	{
		GetLocalVersion();	
		m_LocalVersion--;
		SetLocalVersion();
	}

	if(m_LocalVersion < newVersion)
	{
		int ver_diff = newVersion - m_LocalVersion;
		int ver_idx = 0;
		char filename[256] = "";
		for(int i = m_LocalVersion+1; i <= newVersion; i++, ver_idx++ )
		{
			sprintf(filename,"%d.zip", i );

			PatchFile(ConnectIp, &upgrade, ver_idx, ver_diff, filename);
//				return false;

			SetLocalVersion();
		}
		m_LocalVersion = newVersion;
		SetLocalVersion();
	}

	return true;
}

void UpdateThread()
{
	srand( (unsigned)time( NULL ) );
	GetLocalVersion();
	if( PatchProc(false) == false )
		return;
}

bool ConnectPatchServer(char* szConnectIp, CAutoUpgrade* pUpdate)
{
	if (!pUpdate || !pUpdate)
		return false;

	CString strUpdateUrl;
	strUpdateUrl = LH_CONNECT_URL;

	
	char buf[1024] = {0,};
	char *arg = pUpdate->Lead(strUpdateUrl.GetBuffer(), IP_FILE);

	if (!arg) 
		return false;

	strcpy(szConnectIp, arg);
	arg = GetOneLine(arg, buf);

	int server_count = atoi(buf);
	if (server_count > 10000)
	{
		server_count %= 10000;
	}
	else if (server_count <= 0) 
	{
		return false;
	}

	int server_num = rand() % server_count;
	
	for (int i = 0; i < server_num+1; i++)	
	{
		arg = GetOneLine(arg, buf);
	}

	strcpy(szConnectIp, buf);
	return true;
}

int VersionCheck(char* szConnectIp, CAutoUpgrade* pUpdate, char* file)
{
	if (!pUpdate || !pUpdate)
		return -1;

	char* arg = pUpdate->Lead(szConnectIp, file);
	if (!arg)
	{
		return -1;
	}
	
	int newVersion = atoi(arg);

	if( newVersion == 0 )
	{
		return -1;
	}
	else if( newVersion >= 2000 )
	{
		if( m_LocalVersion < 2000 )
		{
			return -1;
		}
	}
	else
	{
		if( newVersion == 1999 )
		{
			return -1;
		}
	}

	return newVersion;
}

char* GetOneLine(char *arg, char *buf)
{
	while (*arg == ' ' || *arg == '\r' || *arg == '\n')
		arg++;
	while (*arg != '\r' && *arg != '\n' && *arg != '\0')
		*(buf++) = *(arg++);
	*buf = '\0';

	return arg;
}

BOOL GetLocalVersion()
{	
	CString fileName = LOCAL_VERSION_FILE;
	int encr_version;
	FILE *fp;
	fp = fopen(fileName, "rb");
	if (!fp) 
	{		
		fp = fopen(fileName, "wb");
		if (!fp)
		{
			return FALSE;
		}
		encr_version = (LOCAL_VERSION * 3) + 27;
		fwrite(&encr_version, sizeof(int), 1, fp);
		fclose(fp);
		m_LocalVersion = LOCAL_VERSION;
	} 
	else 
	{
		fread(&encr_version, sizeof(int), 1, fp);
		m_LocalVersion = (encr_version - 27) / 3;
		fclose(fp);
	}

	return TRUE;
}

void SetLocalVersion()
{
	int encr_version = 0;
	FILE *fp;
	fp = fopen(LOCAL_VERSION_FILE, "r+b");
	if( fp ) 
	{
		encr_version = (m_LocalVersion * 3) + 27;
		fwrite(&encr_version, sizeof(int), 1, fp);
		fclose(fp);
	}
}