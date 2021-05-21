#ifndef _WEB_WORLD_H_
#define _WEB_WORLD_H_

#include "Config.h"

#include "wininet.h"

#define MAX_URLDATA_LEN 3000 // m_UrlData 의 크기

class CWebWorld
{
public:
	CWebWorld();
	virtual ~CWebWorld();

	BOOL GetWebPage(const char* Url);

	char m_UrlData[MAX_URLDATA_LEN]; // 이걸 포인터로 잡아서 할당하니까 문제가 많더라.그래서 그냥 큰 배열로 잡음.
	HINTERNET m_Session;
};

#endif // _WEB_WORLD_H_
