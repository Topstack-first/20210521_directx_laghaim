#ifndef __theodoric__200706261812__hackprocess_mgr_kill__
#define __theodoric__200706261812__hackprocess_mgr_kill__

#include"singleton.h"

#define PATHLOG_MAX 100

class CHackProcessMgr : public CSingleton<CHackProcessMgr>
{
protected:
	char pLog[PATHLOG_MAX][128];

public:

	CHackProcessMgr();
	~CHackProcessMgr();

	int m_nProcCount;

	bool ProcessKill(char* sProcessName, int bListFileWrite = 0 );
	bool GetProcessModule(unsigned long dwPID, char* sProcessName, FILE* fp, int bListFileWrite = 0);
	bool FindeProcessModule(char* sProcessName, int bListFileWrite = 0);
	void DecodeAutoProcessName(char* sDecString, char* sEncString);
	void PathLog(char* ClassFileName, int Line, int InOut = 1);
	void PathLogWrite();
};



#endif