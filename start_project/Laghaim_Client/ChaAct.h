#ifndef _CHA_ACT_H_
#define _CHA_ACT_H_

#include "Config.h"

#include "NkCharacter.h"

#include <string>

#define MAX_CHA_ACT_NUM		1000
#define CHA_ACT_INDEX_NUM	174
#define CHA_ACT_RACE_NUM	10

class CBiped;

class CChaAct
{
public:
	std::string m_bpd_name;

public:
	CChaAct();
	virtual ~CChaAct();

	CBiped *m_pBiped;
	int m_SoundNum;
	int m_SoundFrame;
	int m_AttackFrame;

};
extern CChaAct g_ChaBpdList[MAX_CHA_ACT_NUM];
extern int g_ChaBpdIndex[NUM_RACE*NUM_SEX][ CHA_ACT_INDEX_NUM ];
#endif // _CHA_ACT_H_
