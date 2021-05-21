#ifndef _WANTED_DATA_H_
#define _WANTED_DATA_H_

#include "Config.h"
#include "headers.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include <list>
#include <algorithm>

using namespace std;

typedef struct _stwInfo1
{
	char	wanted_name[128];
	int		wanted_laim;
	int		wanted_race;
	int		wanted_zone;
	t_Info	wanted_remaintime;

	_stwInfo1()
	{
		ZeroMemory(wanted_name, sizeof(wanted_name));
		wanted_laim		  =  0;
		wanted_race		  =  0;
		wanted_zone		  = -1;	// -1이면 비접속중
		wanted_remaintime = t_Info(-1, -1, -1, -1, -1, -1);
	}

	_stwInfo1& operator=(const _stwInfo1 &info)
	{
		strcpy(wanted_name, info.wanted_name);
		wanted_laim		  = info.wanted_laim;
		wanted_race		  = info.wanted_race;
		wanted_zone		  = info.wanted_zone;
		wanted_remaintime = info.wanted_remaintime;

		return (*this);
	}
} wanted_info, *wanted_info_ptr;

typedef list<wanted_info> list_wanted;

#endif // _WANTED_DATA_H_