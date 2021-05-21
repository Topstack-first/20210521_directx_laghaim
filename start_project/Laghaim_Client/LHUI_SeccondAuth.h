#ifndef _LHUI_SeccondAuth_H_
#define _LHUI_SeccondAuth_H_

#pragma once
#include "LHUI_Base.h"

#include "BasicStatic.h"
#include "BasicButton.h"
#include "TextBox.h"
#include "TextOutBox.h"
#include "MsgPopUp.h"

#include <vector>

class CSheet;

enum
{
	MAX_CAPTCHA_BUTTON = 10,
	MIN_SECAUTH = 6,
	MAX_SECAUTH = 8,
};

typedef std::vector<int>		ArrangeNum;
typedef ArrangeNum::iterator	ArrangeNum_it;

class LHUI_SeccondAuth : public LHUI_Base
{
private:
	int				m_captcha_number;
	int				m_input_number[MAX_SECAUTH];

	int				m_current;

	ArrangeNum		m_arrange_num;

	CSheet * m_sheet;


public:
	CMsgPopUp		m_MsgPopUp;

	CBasicStatic	m_bg;
	CBasicButton	m_btn_confirm;
	CBasicButton	m_btn_cancel;
	CBasicButton	m_btn_back;
	CBasicButton	m_btn_refresh;
	CBasicButton	m_btn_captcha[MAX_CAPTCHA_BUTTON];

	CTextOutBox		m_captcha_output;
	CTextOutBox		m_captcha_output_test;



public:
	void _generate();
	void _reset();
	bool _check(const int iv);
	void _relocate_button();


public:



public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();

	void SetPos(int x, int y);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	UINT MsgProc_Mouse( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:
	LHUI_SeccondAuth(CSheet* sheet);
	~LHUI_SeccondAuth(void);
};
#endif // _LHUI_SeccondAuth_H_