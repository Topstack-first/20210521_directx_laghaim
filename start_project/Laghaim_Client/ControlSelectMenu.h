#ifndef _CONTROL_SELECTMENU_H_
#define _CONTROL_SELECTMENU_H_

#include "Config.h"
#include <vector>



class CListBox;


// 후견인과 견습생 상품 지금 UI
// 다른 곳에서도 쓸 수 있도록 ControlGate에서 분리

class CControlSelectMenu : public CBaseInterface
{
public:

	CSurface		*m_pSurfaceBack;
	CBasicButton	m_CloseBtn;
	CListBox		*m_MenuList;
	int				m_ScreenWidth;
	int				m_ScreenHeight;
	int				m_MenuType;
	int				m_NumCount;

	std::vector<int> m_vecReturnMsg;

public:
	CControlSelectMenu();
	virtual ~CControlSelectMenu();

	void	Init();
	void	ResetMenuList();
	void	AddMenuList();
	void	LoadRes();
	void	DeleteRes();
	void	Draw();
	BOOL	IsInside(int x, int y);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL	SetMenuList(int type, char * arg = NULL);
	void	AddMenuStart();
	//void	AddMenuPush( char * strMenuList );
	void	AddMenuPush( int num  );
	void	AddMenuEnd();

};

#endif // _CONTROL_GATE_H_
