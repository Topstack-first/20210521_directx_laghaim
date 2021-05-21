#ifndef _SHADOW_RECT_H_
#define _SHADOW_RECT_H_

#include "Config.h"

#include <d3d.h>

class CLand;
class IndexedTexture;

class CShadowRect
{
	D3DLVERTEX	m_TileRectVert[36];
	unsigned short	m_TileIndex[54];
	CLand		*m_pLand;								// 지형에 대한 정보를 얻기 위한 포인터. 지우지 말것.
	D3DVECTOR	m_vSelSpot;								// 선택된 지점의 월드 좌표
	int			m_tileIdx;								// 선택 지점의 타일 인덱스
	D3DMATRIX	m_matWorld;
	int			m_TileNum;
	int			m_ShadowXSize;
	int			m_ShadowZSize;
	int			m_Sign;

protected:
	void		SetupTileRect();

public:
	CShadowRect();
	virtual ~CShadowRect();

	void		Init(CLand *pLand );

	void		SetTileCoord(D3DVECTOR &vecWorld, int size_or_hour, BOOL bIsHour);
	void		SetTileCoord(float wx, float wy, float wz, int size_or_hour, BOOL bIsHour);

	HRESULT		Render(LPDIRECTDRAWSURFACE7 lpDDsurface = NULL, BOOL bReal=FALSE);
};

extern CShadowRect * g_pShadowRect;

#endif // _SHADOW_RECT_H_
