#include "stdafx.h"
#include "LHUI_CashShop.h"

LHUI_CashShop::LHUI_CashShop(void)
	: m_pEnableSlot(NULL)
	, m_pDisableSlot(NULL)
	, m_pExistSlot(NULL)
{
}

LHUI_CashShop::~LHUI_CashShop(void)
{
}

void LHUI_CashShop::Init()
{

}

void LHUI_CashShop::LoadRes()
{

}

void LHUI_CashShop::DeleteRes()
{

}

void LHUI_CashShop::Draw()
{

}


void LHUI_CashShop::SetPos(int x, int y)
{

}


LRESULT LHUI_CashShop::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !_GetVisible() && _GetDisalbe())
		return 0;

	if(m_MsgPopUp.m_bActive == TRUE)
	{
		int ret = -1;
		if ((ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{
		}
		else if (ret == -1)
		{
		}
		return 0;
	}

	switch( msg )
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		return MsgProc_Mouse(hWnd, msg, wParam , lParam);
	}

	return 0;
}


//
UINT LHUI_CashShop::MsgProc_Mouse( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	m_close.MsgProc(hWnd, msg, wParam, lParam);

	switch( msg )
	{
	case WM_MOUSEMOVE:
		{
			// 잡고는 이아이템 있나요?
		}
		break;
	case WM_LBUTTONUP:
		{
			// 여기선 몰할까요?
		}
		break;
	case WM_LBUTTONDOWN:
		{
			// 일단 선택?
		}
		break;

	case WM_RBUTTONUP:
		{
			//
		}
		break;
	case WM_RBUTTONDOWN:
		{
			// 선택된 아이템이 있는가?
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			// 구매할테요?
		}
		break;
	case WM_RBUTTONDBLCLK:
		{
			// 없을듯!!!
		}
		break;
	}

	return 0;

//	g_pRoh->m_ExtraSlot;
}