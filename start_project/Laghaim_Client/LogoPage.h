#ifndef _LOGO_PAGE_H_
#define _LOGO_PAGE_H_

#include "Config.h"

#include "headers.h"

class CSheet;
class CSurface;

class CLogoPage
{

public:
	CSheet*			m_pSheet;

	CSurface*		m_pBatangSur;

	CLogoPage();
	CLogoPage(CSheet *pSheet);
	virtual ~CLogoPage();

	void Init();
	void SetBackground();
	void LoadRes();
	void DeleteRes();
	void Draw();
	void MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Action( int i );


};

#endif // _LOGO_PAGE_H_
