#ifndef _WANTED_FUNC_H_
#define _WANTED_FUNC_H_

#include "Config.h"
#include "Cmd.h"

class CUIMgr;
class CWantedCmd
{
private:
	typedef void (*cmd_f)(char*, CUIMgr*);
	typedef std::map<std::string, cmd_f> MAP_FUNC;

	CWantedCmd(CUIMgr *pMgr);

public:
	static CWantedCmd* Create(CUIMgr *pMgr);

	~CWantedCmd();

public:
	BOOL wanted_addcmd(char *cmd_str, void cmd_p(char *, CUIMgr *));
	void wanted_run(char* cmd, char *arg);

private:
	CUIMgr* m_pUIMgr;
	MAP_FUNC m_mapFunc;
};

#endif // _WANTED_FUNC_H_