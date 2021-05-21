#ifndef _HAPPY_NEW_YEAR_H_
#define _HAPPY_NEW_YEAR_H_

#include "Config.h"

class IndexedTexture;

class CHappyNewYear
{
	LPD3DTLVERTEX		m_rect;
	DWORD				m_eventTexId[8];
	IndexedTexture*		m_pIndexedTexture;
	int					m_CurFrame;
	int					m_TexNum;

public:
	CHappyNewYear();
	virtual ~CHappyNewYear();

	void LoadRes();
	void DeleteRes();

	void Render();

	// [12/24/2007 �����] �޸� ������ ���� �Ǵ� ����
	bool GetRenderState()
	{
		return m_bRender;
	}

protected:
	bool m_bRender;

};

#endif // _HAPPY_NEW_YEAR_H_
