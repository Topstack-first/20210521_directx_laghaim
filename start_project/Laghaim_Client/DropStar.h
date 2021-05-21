#ifndef _DROP_STAR_H_
#define _DROP_STAR_H_

#include "Config.h"

#define DROPSTAR_COUNT	40	// ÃÖ´ë º° °¹¼ö

class CDropStar
{
public:
	CDropStar();
	virtual ~CDropStar();

	DWORD m_lastTime;
	float	m_StarX[DROPSTAR_COUNT];
	float	m_StarY0[DROPSTAR_COUNT];
	float	m_StarY[DROPSTAR_COUNT];
	float	m_StarZ[DROPSTAR_COUNT];
	D3DLVERTEX	m_StarVertex[DROPSTAR_COUNT*4];

	void		Render(float x, float y, float z);

};

#endif // _DROP_STAR_H_
