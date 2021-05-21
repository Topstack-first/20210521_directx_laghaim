#ifndef _LHParam_GMagicStone_H_
#define _LHParam_GMagicStone_H_

#include "ItemProto.h"

class LHParam_GMagicStone
{
public:
	LHParam_GMagicStone(void);
	~LHParam_GMagicStone(void);

public:
	bool CheckNeedItem( __int64 & rnNeedLaim );
};
#endif // _LHParam_GMagicStone_H_