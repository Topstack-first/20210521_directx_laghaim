#include "stdafx.h"

#include <commctrl.h>
#include "ObjectList.h"
#include "resource.h"

BOOL CALLBACK ObjectListProc( HWND, UINT, WPARAM, LPARAM );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectList::CObjectList()
	: m_hwndParent(NULL)
	, m_hInst(NULL)
	, m_hwndObjTab(NULL)
	, m_hwndListObj1(NULL)
	, m_hwndListObj2(NULL)
{
	m_hwndDlg = NULL;
}

CObjectList::~CObjectList()
{}

void CObjectList::InitDialog(HWND hwndParent, HINSTANCE hInstance)
{
	m_hwndParent = hwndParent;
	m_hInst = hInstance;
	m_hwndDlg = ::CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_OBJECT), hwndParent, ObjectListProc, (LPARAM) this);
}

void CObjectList::LocateInitialPos()
{
	HWND hwndDesk;
	RECT rcDlg, rcParent, rcDesk;
	int x, y, width, height;

	hwndDesk = GetDesktopWindow();
	GetWindowRect(hwndDesk, &rcDesk);
	GetWindowRect(m_hwndDlg, &rcDlg);
	GetWindowRect(m_hwndParent, &rcParent);

	x = rcDesk.right - (rcDlg.right - rcDlg.left);
	y = rcParent.top+20;
	width = rcDlg.right - rcDlg.left;
	height = rcDlg.bottom - rcDlg.top;

	::MoveWindow(m_hwndDlg, x, y, width, height, FALSE);
}
void CObjectList::OnNotify(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR pnmh;
	int		pos;

	if (wParam == IDC_OBJ_TAB)
	{
		pnmh = (LPNMHDR) lParam;
		switch(pnmh->code)
		{
		case TCN_SELCHANGE:
			pos = TabCtrl_GetCurFocus(m_hwndObjTab);

			if (pos == 0)
			{
				ShowWindow(m_hwndListObj1, SW_SHOW);
				ShowWindow(m_hwndListObj2, SW_HIDE);
			}
			if (pos == 1)
			{
				ShowWindow(m_hwndListObj1, SW_HIDE);
				ShowWindow(m_hwndListObj2, SW_SHOW);
			}

			break;
		}
	}
}

int	CObjectList::InsertTabItem(HWND hwnd, int num, LPSTR text, int iImage)
{
	TC_ITEM item;

	ZeroMemory(&item, sizeof(TC_ITEM));
	item.mask = TCIF_TEXT;
	item.pszText = text;
	item.cchTextMax = strlen(text)+5;
	item.iImage = iImage;

	return TabCtrl_InsertItem(hwnd, num, &item);
}

// List1의 Content를 넣는다.
void CObjectList::AddObjList1Contents()
{
	int iCount = 0;
	char szTemp[20];

	for ( ; iCount < 100; iCount++)
	{
		_itoa(iCount, szTemp, 10);
		SendMessage(m_hwndListObj1, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) szTemp);
	}

	// 맨처음 항목을 선택해놓는다.
	SendMessage(m_hwndListObj1, LB_SETCURSEL, (WPARAM) 0, 0);
}

// List2의 Content를 넣는다.
void CObjectList::AddObjList2Contents()
{
	int iCount = 0;

	SendMessage(m_hwndListObj2, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) "두번째 리스트 박스");
	// 맨처음 항목을 선택해놓는다.
	SendMessage(m_hwndListObj2, LB_SETCURSEL, (WPARAM) 0, 0);
}

BOOL CALLBACK ObjectListProc( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	static CObjectList *pObjectList = NULL;
	static HINSTANCE hInst;

	switch (nMsg)
	{
	case WM_INITDIALOG:
		char *pszTemp;
		pszTemp = new char[255];

		// 다이얼로그 생성시 다이얼로그 클래스를 가져온다.
		pObjectList = (CObjectList *) lParam;
		// 차일드 컨트롤들의 윈도우 핸들을 가져온다.
		if( pObjectList )
		{
			pObjectList->SetHwnd(hWnd);
			pObjectList->m_hwndObjTab = GetDlgItem(hWnd, IDC_OBJ_TAB);
			pObjectList->m_hwndListObj1 = GetDlgItem(hWnd, IDC_LIST_OBJ1);
			pObjectList->m_hwndListObj2 = GetDlgItem(hWnd, IDC_LIST_OBJ2);

			// 탭컨트롤의 탭에 문자 넣기
			hInst = pObjectList->GetHInstance();
			// 첫번째 탭
			LoadString(hInst, IDS_OBJTAB1, pszTemp, 255);
			pObjectList->InsertTabItem(pObjectList->m_hwndObjTab, 0, pszTemp, 0);
			// 두번째 탭
			LoadString(hInst, IDS_OBJTAB2, pszTemp, 255);
			pObjectList->InsertTabItem(pObjectList->m_hwndObjTab, 1, pszTemp, 0);

			// 실제 컨텐트 넣기
			pObjectList->AddObjList1Contents();
			pObjectList->AddObjList2Contents();

			ShowWindow(pObjectList->m_hwndListObj1, SW_SHOW);
			ShowWindow(pObjectList->m_hwndListObj2, SW_HIDE);

			pObjectList->LocateInitialPos();
		}

		delete [] pszTemp;
		break;

	case WM_NOTIFY:
		if( pObjectList )
			pObjectList->OnNotify(hWnd, nMsg, wParam, lParam);
		break;

	default:
		break;
	}

	return FALSE;
}