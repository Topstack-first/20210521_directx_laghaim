#ifndef _LOLOMANAGER_H_
#define _LOLOMANAGER_H_

#include "singleton.h"

class CLolos;
class LoloManager : public CSingleton<LoloManager>
{
public:
	typedef std::map<std::string, CLolos*> MAP_LOLO;

	LoloManager();
	~LoloManager();

public:
	CLolos* GetLolo(std::string filename);
	int GetLoloTextureNum(std::string textrFileName);
	void Clear();

private:
	MAP_LOLO m_mapLolo;
};

#endif