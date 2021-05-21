#ifndef _LHUI_CashShop_H_
#define _LHUI_CashShop_H_

#pragma once

#include "LHUI_Base.h"


#include "MsgPopUp.h"
#include "BasicStatic.h"
#include "BasicButton.h"

// #include "ComboBox.h"
// #include "TextOutBox.h"
// #include "TextBox.h"
// #include "ListBox.h"

enum Cash_MainMenu
{
	Cash_MainMenu__Shop,
	Cash_MainMenu__History,
	Cash_MainMenu__Present,

	Cash_MainMenu__EndOfEnum
};

class LHUI_CashShop : public LHUI_Base
{
public:
	CMsgPopUp		m_MsgPopUp;

	CBasicStatic	m_bg;
	CBasicButton	m_close;




	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;

	/*

	������
	- ĳ�ü�, �ŷ�����, ������


	������(ĳ�ü�)
	- ĳ����, ������, ����, ��Ű��



	ĳ����
	- ���� 1, 2, ...
	- ã��


	- ���ý�Ʈ �޴�
	�Ծ��, ���ϱ�, �����ϱ�, �����ϱ�

	����¡...



	// ĳ���� �̸�����
	����,����1, ����2, ���, �Ӹ�, ���� ,����,�Ź�
	ĳ����, �⺻/�������



	// ��üȭ�鿡���� ���� ������ ���±���!
	*/



public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();

	void SetPos(int x, int y);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	UINT MsgProc_Mouse( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:
	LHUI_CashShop(void);
	~LHUI_CashShop(void);
};
#endif // _LHUI_CashShop_H_