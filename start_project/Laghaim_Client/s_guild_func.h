#ifndef _GUILD_FUNC_H_
#define _GUILD_FUNC_H_

#include "Config.h"
#include "Cmd.h"

class CUIMgr;
class CGuildCmd
{
private:
	typedef void (*cmd_f)(char*, CUIMgr*);
	typedef std::map<std::string, cmd_f> MAP_FUNC;

	CGuildCmd(CUIMgr *pMgr);

public:
	static CGuildCmd* Create(CUIMgr *pMgr);

	~CGuildCmd();

public:
	BOOL guild_addcmd(char *cmd_str, void cmd_p(char *, CUIMgr *));
	void guild_run(char* cmd, char *arg);

private:
	CUIMgr* m_pUIMgr;
	MAP_FUNC m_mapFunc;
};
#endif // _GUILD_FUNC_H_