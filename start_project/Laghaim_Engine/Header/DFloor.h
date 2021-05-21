#ifndef _DFLOOR_H_
#define _DFLOOR_H_

class CLand;
class CDFloor
{
protected:
	float		m_fCenterX, m_fCenterZ;

public:
	CDFloor();
	virtual ~CDFloor();

	static CLand *m_pLand;							// 전역 정보를 유지하기 위해서.
	static D3DLVERTEX BaseTri[4];

	D3DLVERTEX	*m_pBaseMesh;
	WORD		*m_pwBaseIdx;

	void		Init(float x, float z);
	void		Render();	
};

#endif // _DFLOOR_H_
