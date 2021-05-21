#ifndef _DEVTESTGMCOMMAND_H_
#define _DEVTESTGMCOMMAND_H_

#include "singleton.h"
#include <string>
#include <map>

#define DECLARE_CMD(funcname) \
	bool funcname(char*);

#define REGIST_CMD(funcname) \
	m_mapFuncs.insert(std::make_pair(#funcname, &DevTestGmCommand::funcname));

#define REGIST_CMD_NICK(funcname, nick) \
	m_mapFuncNicks.insert(std::make_pair(#nick, #funcname));

class DevTestGmCommand : public CSingleton<DevTestGmCommand>
{
	typedef bool (DevTestGmCommand::*CMD_FUNC_PTR)(char*);
	typedef std::map<std::string, CMD_FUNC_PTR> MAP_FUNC;
	typedef std::map<std::string, std::string> MAP_FUNC_NICK;
public:
	DevTestGmCommand();
	~DevTestGmCommand();

public:
	bool run(const char* buf);

	DECLARE_CMD(res);
	DECLARE_CMD(fullscr);
	DECLARE_CMD(msrl);
	DECLARE_CMD(item);
	DECLARE_CMD(addbuff);
	DECLARE_CMD(delbuff);
	DECLARE_CMD(crash);
	DECLARE_CMD(lock_upgrade);
	DECLARE_CMD(open_friend);
	DECLARE_CMD(spack);
	DECLARE_CMD(thy);
	DECLARE_CMD(yut);
	DECLARE_CMD(gmark);
	DECLARE_CMD(skillupall);
	DECLARE_CMD(nocool);
	DECLARE_CMD(index);
	DECLARE_CMD(vnum);
	DECLARE_CMD(moblist);
	DECLARE_CMD(gomob);
	DECLARE_CMD(gonpc);
	DECLARE_CMD(moblistall);
	DECLARE_CMD(wfr);
	DECLARE_CMD(fish);
	DECLARE_CMD(fishball);
	DECLARE_CMD(rlcfg);
	DECLARE_CMD(pr);
	DECLARE_CMD(testpopup);
	DECLARE_CMD(ac);
	DECLARE_CMD(shout_test);
	DECLARE_CMD(bl);
	DECLARE_CMD(cboss);
	DECLARE_CMD(cbe);
	DECLARE_CMD(invpack)
	DECLARE_CMD(invpack2)

private:
	bool registCmd();

private:
	MAP_FUNC m_mapFuncs;
	MAP_FUNC_NICK m_mapFuncNicks;
};

#endif
