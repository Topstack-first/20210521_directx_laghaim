#include "stdafx.h"
#include "WebWorld.h"

CWebWorld::CWebWorld()
{
	memset(m_UrlData, 0, sizeof(m_UrlData));

	DWORD dwError;

	// Initialize the Win32 Internet functions
	m_Session = ::InternetOpen("Guild Info Browser 1.0",
							   INTERNET_OPEN_TYPE_PRECONFIG, // Use registry settings.
							   NULL, // Proxy name. NULL indicates use default.
							   NULL, // List of local servers. NULL indicates default.
							   0) ;

	dwError = GetLastError();
}

CWebWorld::~CWebWorld()
{
	// Closing the session
	::InternetCloseHandle(m_Session);
}

BOOL CWebWorld::GetWebPage(const char* Url)
{
	HINTERNET hHttpFile;
	char szSizeBuffer[32];
	DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
	DWORD dwFileSize;
	DWORD dwBytesRead;
	BOOL bSuccessful = FALSE;


	// Opening the Url and getting a Handle for HTTP file
	hHttpFile = InternetOpenUrl(m_Session, (const char *) Url, NULL, 0, 0, 0);

	if (hHttpFile)
	{
		// Getting the size of HTTP Files
		BOOL bQuery = ::HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer, &dwLengthSizeBuffer, NULL) ;

		if(bQuery==TRUE)
		{
			// Allocating the memory space for HTTP file contents
			dwFileSize=atol(szSizeBuffer);			
			ZeroMemory( m_UrlData, sizeof(char)*MAX_URLDATA_LEN );

			// Read the HTTP file
			BOOL bRead = ::InternetReadFile(hHttpFile, m_UrlData, dwFileSize, &dwBytesRead);

			if (bRead)
			{
				bSuccessful = TRUE;
				m_UrlData[dwBytesRead] = '\0';
			}

			::InternetCloseHandle(hHttpFile); // Close the connection.
		}

	}
	else
	{
		// Connection failed.
		bSuccessful = FALSE;
	}

	return bSuccessful;
}
