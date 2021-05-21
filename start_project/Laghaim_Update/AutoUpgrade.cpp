// 자동 업그레이드
//
//		* 현재 디렉토리 구하기.
//		* 서버로부터 정보 받아오기.
//
//////////////////////////////////////////////////////////////////////

//#include <windows.h>
#include "stdafx.h"
#include <wininet.h>

//#include "resource.h"
#include "AutoUpgrade.h"
//#include "Language.h"

extern BOOL g_bWebLogin;
extern const int g_Language;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAutoUpgrade::CAutoUpgrade()
{

}
CAutoUpgrade::~CAutoUpgrade()
{

}

// 다운로드 받을 서버을 선택한다
char* CAutoUpgrade::Lead(const char *url, const char*fName)
{		
	//session = new CInternetSession();
	CInternetSession session;
	//pFile   = new CStdioFile();
	CStdioFile *pFile;
	
		
	char tmp[500];
	strcpy(tmp, url);
	strcat(tmp, fName);
	
	
	// 서버에 접속한다
	TRY{
		pFile = session.OpenURL(tmp, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
	}
	CATCH_ALL(error)
	{
		session.Close();
		// 실행 종료 코드 삽입!!!
		return NULL;
	}
	END_CATCH_ALL;
	
	// 읽는다
	try{
		//int len = pFile->GetLength();
		int len = 0;
	    int read;
		char temp[1000];
		add[0] = '\0';
		while ((read = pFile->Read(temp, 999)) > 0) {
			strcat(add, temp);
			len += read;
		}
		add[len] = '\0';
	}
	catch(CFileException *e){
		e->Delete();
		pFile->Close();
		session.Close();
		return NULL;
	}
	pFile->Close();
	session.Close();

	return add;
}


// 접속이 되나 파일 하나 받아본다.
BOOL CAutoUpgrade::ConnectTest(const char *url, const char*fName)
{		
	CInternetSession session;
	CStdioFile *pFile;
	
	char tmp[500];
	strcpy(tmp, url);
	strcat(tmp, fName);

	BOOL bTest = session.SetOption( INTERNET_OPTION_CONNECT_TIMEOUT, 500 ); // 이걸 해줘야 접속 대기시간이 짧아진다.

	// 서버에 접속한다
	TRY {
		pFile = session.OpenURL(tmp, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
	}
	CATCH_ALL(error)
	{
		//e->Delete();
		session.Close();
		// 실행 종료 코드 삽입!!!
		return FALSE;
	}
	END_CATCH_ALL;

	
	pFile->Close();
	session.Close();

	return TRUE;
}


// 다운로드 요청
void CAutoUpgrade::DownLoad(char *url, char *filename)
{
	//if( Connection(url, filename) == FALSE)			
	//	AfxMessageBox("다운로드 실패");
}



int CAutoUpgrade::Connection(char *url, char *filename, int ver_idx, int ver_diff)
{
	char tmp[500];
	strcpy(tmp, url);
	strcat(tmp, filename);

	// 세션열기.	
	// CInternetSession <- 이클레스는 접속할 서버에 대한 정보를 관리한다
	CInternetSession session;
	CString fname;
	unsigned char szBuff[1025];
	UINT bytesRead;
	CStdioFile *pFile = NULL;


	long 	nFileSize = 0;
	char	szSizeBuffer[32];
	DWORD	dwLengthSizeBuffer = 32;
	int		file_read = 0;

	// 서버에 접속한다
	TRY
	{		
		pFile = session.OpenURL(tmp, 1, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
		//Retreive file length
		HINTERNET	hFileInfo;
		hFileInfo = InternetOpenUrl(session, tmp, NULL, 0, 0, 0);


		if(::HttpQueryInfo(hFileInfo, HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer, &dwLengthSizeBuffer, NULL))
			nFileSize = atol(szSizeBuffer);
		else
			nFileSize = -1;

		InternetCloseHandle(hFileInfo);
	}
	CATCH_ALL(error)
	{
		session.Close();
		TCHAR   szCause[255];
		error->GetErrorMessage(szCause,254,NULL);

		return FALSE;
	}
	END_CATCH_ALL;

	// 다운로드 하여 작성할 파일 지정
	CFile saveFile( filename, CFile::modeCreate|CFile::modeWrite);
	// 다운로드 시작..
	try{
		while( (bytesRead = pFile->Read(szBuff, 1024)) > 0 )
		{
			saveFile.Write(szBuff, bytesRead);
			file_read += bytesRead;
		}
	}
	catch(CFileException *e)
	{
		e->Delete();
		pFile->Close();
		saveFile.Close();
		session.Close();
		return FALSE;
	}

	pFile->Close();
	saveFile.Close();
	session.Close();

	if (nFileSize > 0 && file_read < nFileSize)
		return -1;

	return TRUE;
}
