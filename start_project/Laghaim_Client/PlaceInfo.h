#ifndef _PLACE_INFO_H_
#define _PLACE_INFO_H_

#include "Config.h"

class CPlaceInfo
{
	LPD3DTLVERTEX		m_rectPlaceInfo;
	DWORD				m_PlaceInfoTexId;
	IndexedTexture*		m_pIndexedTexture;
	int					m_CurFrame;
	int					m_TexNum;

public:
	CPlaceInfo();
	virtual ~CPlaceInfo();
	void LoadRes(int texNum);
	void DeleteRes();
	void Render();

};

#endif // _PLACE_INFO_H_
