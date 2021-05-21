#ifndef _FX_CASTING_H_
#define _FX_CASTING_H_

#include "Config.h"

class CFxCasting
{
	LPD3DTLVERTEX		m_rectCasting;
	DWORD				m_CastingTexId[5];
	float				m_CurFrame;
	int					m_nTexNum;

public:
	CFxCasting();
	virtual ~CFxCasting();
	void DeleteRes();
	void Render( DWORD dwCurtime );
	int GetTexNum()
	{
		return m_nTexNum;
	}
	void SetTexNum( int nTexNum )
	{
		m_nTexNum = nTexNum;
	}
};

#endif // _FX_CASTING_H_
