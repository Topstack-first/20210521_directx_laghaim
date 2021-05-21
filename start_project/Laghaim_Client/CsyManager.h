#ifndef _CSYMANAGER_H_
#define _CSYMANAGER_H_

#include "singleton.h"

class CCapsyong;
class CsyManager : public CSingleton<CsyManager>
{
public:
	typedef std::map<std::string, CCapsyong*> MAP_CAPSYONG;

	CsyManager();
	~CsyManager();

public:
	CCapsyong* GetCsy(std::string filename);
	int GetLoloTextureNum(std::string textrFileName);
	void Clear();

private:
	MAP_CAPSYONG m_mapCapsyong;
};

#endif