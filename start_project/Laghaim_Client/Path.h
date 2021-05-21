#ifndef _PATH_H_
#define _PATH_H_

#include "Config.h"

#define PATHMAP_WIDTH	30
#define PATHMAP_HEIGHT	30

class CLand;
class CNkCharacter;

class CPath
{
public:
	CPath();
	virtual ~CPath();

	char *m_PathMap;
	CLand *m_pLand;

	int m_tWidth, m_tHeight;

	void Init(CLand *pLand);
	void ClearAll();
	void SetMapPoint(float x, float z, unsigned char uFlag);
	void SetMapMovable(float x, float z, BOOL bMove);
	BOOL IsMoveArea(float x, float z);
};

#endif // _PATH_H_
