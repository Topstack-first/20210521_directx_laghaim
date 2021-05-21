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

	메인탭
	- 캐시샵, 거래정보, 선물함


	서브탭(캐시샵)
	- 캐릭터, 아이템, 라비용, 패키지



	캐릭터
	- 구분 1, 2, ...
	- 찾기


	- 컨택스트 메뉴
	입어보기, 찜하기, 구매하기, 선물하기

	페이징...



	// 캐릭터 미리보기
	망토,무기1, 무기2, 방어, 머리, 상의 ,하의,신발
	캐릭터, 기본/장비해제



	// 전체화면에서는 충전 페이지 오픈금지!
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