#include "stdafx.h"
#include "InterFileConnection.h"

#define MAX_BMP_FILESIZE	5120

CHttpConnection* CHttpConnection::Create()
{
	return new CHttpConnection;
}

CHttpConnection::CHttpConnection()
{

}

CHttpConnection::~CHttpConnection()
{

}

BOOL CHttpConnection::ConnectDown(char* openInternet, char* openFileUrl)
{
	HINTERNET	hInternet = NULL;
	HINTERNET	hHttp = NULL;
	DWORD		dwSize = 0;
	DWORD		dwRead = 0;
	DWORD		dwWritten = 0;
	DWORD		dwInfoSize = 10000;

	BYTE		buf[5120];
	char		szStatus[10000];

	ZeroMemory(buf, sizeof(buf));
	ZeroMemory(szStatus, sizeof(szStatus));

	if((hInternet = InternetOpen("httpfile", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL,  0)) == NULL)
	{
		OutputDebugString("InternetOpen Fail\n");
		InternetCloseHandle(hInternet);
		return FALSE;
	}


	if((hHttp = InternetOpenUrl(hInternet, openInternet, NULL, 0, INTERNET_FLAG_RELOAD, 0)) == NULL)
	{
		OutputDebugString("InternetConnect fail\n");
		InternetCloseHandle(hHttp);
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	::HttpQueryInfo(hHttp, HTTP_QUERY_STATUS_CODE, szStatus, &dwInfoSize, NULL);

	long nStatusCode = atol(szStatus);

	if(nStatusCode != HTTP_STATUS_OK)
	{
		OutputDebugString("HTTP_STATUS_NOT_FOUND\n");
		InternetCloseHandle(hHttp);
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	bool bFileSizeOk = false;
	DWORD dwFilesize = 0;
	dwInfoSize = 32;
	if( ::HttpQueryInfo(hHttp, HTTP_QUERY_CONTENT_LENGTH, szStatus, &dwInfoSize, NULL) == TRUE )
	{
		dwFilesize = (DWORD) atol(szStatus);

		if( dwFilesize < MAX_BMP_FILESIZE )
			bFileSizeOk = true;
	}

	if( bFileSizeOk == false )
	{
		InternetCloseHandle(hHttp);
		InternetCloseHandle(hInternet);

		return FALSE;
	}

	HANDLE hFile;

	hFile = CreateFile(openFileUrl, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		char a = 'a';

		WriteFile(hFile, &a, 1, &dwWritten, NULL);
		WriteFile(hFile, &a, 1, &dwWritten, NULL);
		WriteFile(hFile, &a, 1, &dwWritten, NULL);
		WriteFile(hFile, &a, 1, &dwWritten, NULL);
		WriteFile(hFile, &a, 1, &dwWritten, NULL);
		WriteFile(hFile, &a, 1, &dwWritten, NULL);

		do
		{
			InternetQueryDataAvailable(hHttp, &dwSize, 0, 0);
			InternetReadFile(hHttp, buf, dwSize, &dwRead);
			WriteFile(hFile, buf, dwRead, &dwWritten, NULL);
			buf[dwRead] = 0;
		}
		while(dwRead != 0);

		CloseHandle(hFile);
	}

	InternetCloseHandle(hHttp);
	InternetCloseHandle(hInternet);

	return TRUE;
}