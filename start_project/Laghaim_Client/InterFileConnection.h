#ifndef _INTERNET_FILE_CONNECTION_H_
#define _INTERNET_FILE_CONNECTION_H_

#include "Config.h"

#pragma comment(lib, "wininet.lib")


#include <wininet.h>

class CHttpConnection
{
public :
	static CHttpConnection* Create();
	~CHttpConnection();

	BOOL ConnectDown(char* openInternet, char* openFileUrl);

private:
	CHttpConnection();
};

class CLOCK
{
public:
	CLOCK() : m_hMutex(NULL)
	{
		m_hMutex = ::CreateMutex( NULL, false, NULL);
	}

	virtual ~CLOCK()
	{
		::CloseHandle( m_hMutex );
	}
	bool lock()
	{
		if( m_hMutex == NULL ) return false;

		WaitForSingleObject( m_hMutex, INFINITE );

		return true;
	}

	void unlock()
	{
		ReleaseMutex(m_hMutex);
	}
private:
	HANDLE m_hMutex;
};
#endif // _INTERNET_FILE_CONNECTION_H_