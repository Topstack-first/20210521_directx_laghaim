#ifndef _WEB_WORLD_H_
#define _WEB_WORLD_H_

#include "Config.h"

#include "wininet.h"

#define MAX_URLDATA_LEN 3000 // m_UrlData �� ũ��

class CWebWorld
{
public:
	CWebWorld();
	virtual ~CWebWorld();

	BOOL GetWebPage(const char* Url);

	char m_UrlData[MAX_URLDATA_LEN]; // �̰� �����ͷ� ��Ƽ� �Ҵ��ϴϱ� ������ ������.�׷��� �׳� ū �迭�� ����.
	HINTERNET m_Session;
};

#endif // _WEB_WORLD_H_
