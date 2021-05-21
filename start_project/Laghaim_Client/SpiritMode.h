#ifndef _SPIRIT_MODE_H_
#define _SPIRIT_MODE_H_

#include "Config.h"

class CSurface;
class CSprite; // 2D 스프라이트 애니메이션 클래스.

class CSpiritMode
{
	LPD3DTLVERTEX		m_rectGetScore1000;
	LPD3DTLVERTEX		m_rectGetScore100;
	LPD3DTLVERTEX		m_rectGetScore10;
	LPD3DTLVERTEX		m_rectGetScore1;
	LPD3DTLVERTEX		m_rectSlash;
	LPD3DTLVERTEX		m_rectTagetScore100;
	LPD3DTLVERTEX		m_rectTagetScore10;
	LPD3DTLVERTEX		m_rectTagetScore1;

	DWORD				m_dwLastTickTime; //애니 프레임 갱신용.

	CSurface			*m_pSurfaceBack;
	CSprite				m_SpriteFireAni;

	int					m_nGetPoint1000;
	int					m_nGetPoint100;
	int					m_nGetPoint10;
	int					m_nGetPoint1;
	int					m_nTagetPoint100;
	int					m_nTagetPoint10;
	int					m_nTagetPoint1;
	int					m_nCurFrame;
	int					m_nSpiritTime;

public:
	CSpiritMode();
	virtual ~CSpiritMode();
	void LoadRes();
	void DeleteRes();
	void Draw();
	void DrawPoint();
	void SpiritSetPoint( int nGet_Thousand, int nGet_Hundred, int nGet_Ten, int nGet_One, int nTaget_Hundred, int nTaget_Ten, int nTaget_One);
	void SpiritEnd( BOOL bEnd );
	void SetFrame( int nCurFrame );
	void SetSpiritTime( int nRemainTime );
	BOOL IsInside(int x, int y);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // _SPIRIT_MODE_H_
