#ifndef _CMD_H_
#define _CMD_H_

#include "Config.h"

class CUIMgr;
class CCmd
{
private:
	typedef void (*cmd_f)(char*, CUIMgr*);
	typedef std::map<std::string, cmd_f> MAP_FUNC;

public:
	CCmd(CUIMgr *pUIMgr);
	virtual ~CCmd();

	BOOL AddCmd(char *cmd_str, void cmd_p(char *, CUIMgr *));
	void Run(char* cmd, char *arg);

private:
	CUIMgr *m_pUIMgr;
	MAP_FUNC m_mapFunc;
};

#endif // _CMD_H_
