#pragma once
#include "LHSingleton.h"
#include "Config.h"


#include <map>
#include <string>

typedef std::map<int, std::string>		LHSTRMAP;
typedef LHSTRMAP::iterator				LHSTRMAP_it;
typedef LHSTRMAP::value_type			LHSTRMAP_vt;


enum LH_LANG_TYPE
{
	LH_LANG_TYPE__KOR,
	LH_LANG_TYPE__JPN,
	LH_LANG_TYPE__ENG,
	LH_LANG_TYPE__THAI,

	LH_LANG_TYPE__EndOfEnum
};


class LHGlobal_String : public LHSingleton<LHGlobal_String>
{
public:
	int m_current_language;

public:
	LHSTRMAP * m_msg_string;
	LHSTRMAP * m_item_string;
	LHSTRMAP * m_npc_string;


public:
	void Open_All();

public:
	void Open_MsgTable();
	void Open_ItemTable();
	void Open_NpcTable();



public:
	const char * GetMsg(int index);
	const char * GetItemName(int index);
	const char * GetNpcName(int index);



public:
	LHGlobal_String(void);
	~LHGlobal_String(void);
};

extern LHGlobal_String theGlobalString;

#define LHGET_MSG(idx)		LHGlobal_String::GetSingletonPtr()->GetMsg(idx)
#define LHGET_ITEM_NAME(idx)	LHGlobal_String::GetSingletonPtr()->GetItemName(idx)
#define LHGET_NPC_NAME(idx)	LHGlobal_String::GetSingletonPtr()->GetNpcName(idx)
#define LHGET_SYS_MSG(idx)	LHGlobal_String::GetSingletonPtr()->GetSysMsg(idx)

