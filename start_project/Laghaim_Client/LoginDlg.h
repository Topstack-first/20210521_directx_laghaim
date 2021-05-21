#ifndef _LOGIN_DLG_H_
#define _LOGIN_DLG_H_

#include "Config.h"

class CLoginDlg
{
	HWND	m_hwndName, m_hwndPass;

public:
	char		*m_pzName;
	char		*m_pzPass;
	CLoginDlg();
	virtual ~CLoginDlg();

	BOOL DoModal(HWND hWnd, HINSTANCE hInstance);
	BOOL LoginDlgProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
};

#endif // _LOGIN_DLG_H_
