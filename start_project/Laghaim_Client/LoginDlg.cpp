#include "stdafx.h"

#include "LoginDlg.h"
#include "resource.h"

CLoginDlg *g_pLoginDlg;

BOOL DlgProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if( g_pLoginDlg )
		return g_pLoginDlg->LoginDlgProc(hWnd, nMsg, wParam, lParam);
	else
		return false;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLoginDlg::CLoginDlg()
	: m_hwndName(NULL)
{
	m_pzName = new char[100];
	m_pzPass = new char[20];

	m_pzName[0] = NULL;
	m_pzPass[0] = NULL;

	// 다이얼로그 프로시저를 위해서.
	g_pLoginDlg = this;
}

CLoginDlg::~CLoginDlg()
{
	SAFE_DELETE(m_pzName);
	SAFE_DELETE(m_pzPass);
}

BOOL CLoginDlg::DoModal(HWND hWnd, HINSTANCE hInstance)
{
	BOOL bRet;
	bRet = ::DialogBox(hInstance, MAKEINTRESOURCE(IDD_LOGIN_DLG), hWnd, (DLGPROC) DlgProc);
	return bRet;
}

BOOL CLoginDlg::LoginDlgProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			m_hwndName = GetDlgItem(hWnd, IDC_EDIT_ID);
			m_hwndPass = GetDlgItem(hWnd, IDC_EDIT_PASS);
			SendMessage(m_hwndName, EM_GETLINE, (WPARAM) 0, (LPARAM) (LPCSTR) m_pzName);
			SendMessage(m_hwndPass, EM_GETLINE, (WPARAM) 0, (LPARAM) (LPCSTR) m_pzPass);
			EndDialog(hWnd, TRUE);
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, FALSE);
			return TRUE;
		}
	}

	return FALSE;
}
