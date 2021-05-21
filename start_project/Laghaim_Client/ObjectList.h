#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_

#include "Config.h"

class CObjectList
{
protected:
	HWND	m_hwndParent;
	HINSTANCE m_hInst;
	HWND	m_hwndDlg;

public:
	// child controls
	HWND	m_hwndObjTab;
	HWND	m_hwndListObj1;
	HWND	m_hwndListObj2;

public:
	CObjectList();
	virtual ~CObjectList();

	void	AddObjList1Contents();
	void	AddObjList2Contents();
	void	LocateInitialPos();

	void	OnNotify( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	int		InsertTabItem(HWND hwnd, int num, LPSTR text, int iImage);
	void	InitDialog(HWND hwndParent, HINSTANCE hInstance);

	void	SetHwnd(HWND hwnd)
	{
		m_hwndDlg = hwnd;
	};
	HWND	GetHwnd()
	{
		return m_hwndDlg;
	};
	HINSTANCE GetHInstance()
	{
		return m_hInst;
	};
};

#endif // _OBJECT_LIST_H_
