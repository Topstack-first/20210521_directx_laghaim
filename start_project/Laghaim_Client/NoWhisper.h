#ifndef _NO_WHISPER_H_
#define _NO_WHISPER_H_

#include "Config.h"

#include <list>
#include <iterator>
using namespace std;

class CNoWhisper
{
public:
	CNoWhisper();
	virtual ~CNoWhisper();

	list<char *> m_listName;

	void	AddToList(char *player_name);
	void	DeleteFromList(char *player_name);

	BOOL	IsIntheList(char *player_name);
};

#endif // _NO_WHISPER_H_
