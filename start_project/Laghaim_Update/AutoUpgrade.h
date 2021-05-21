// AutoUpgrade.h: interface for the CAutoUpgrade class.
//
//////////////////////////////////////////////////////////////////////

#include <afxinet.h>

#if !defined(AFX_AUTOUPGRADE_H__029B395E_127F_4986_941C_9B24C5CFB37F__INCLUDED_)
#define AFX_AUTOUPGRADE_H__029B395E_127F_4986_941C_9B24C5CFB37F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAutoUpgrade  
{
public:
	CAutoUpgrade();
	virtual ~CAutoUpgrade();
	void DownLoad( char *, char *);
	//char* ExtractDir();

//private:
	int Connection(char *, char *, int ver_idx, int ver_diff);

private:
	//CInternetSession	*session;
	//CStdioFile			*pFile;
	//CFile				*dFile;


public:
	char add[2048];	
	char* Lead(const char *url, const char*fName);
	BOOL ConnectTest(const char *url, const char*fName);
	BYTE  m_TotalByte;
	char* GetAdd() { return add;}
	int   progresspos;
	

};

#endif // !defined(AFX_AUTOUPGRADE_H__029B395E_127F_4986_941C_9B24C5CFB37F__INCLUDED_)
