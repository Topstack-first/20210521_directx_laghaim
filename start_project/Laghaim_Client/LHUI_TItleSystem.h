#ifndef _LHUI_TItleSystem_H_
#define _LHUI_TItleSystem_H_

#pragma once

#include "LHUI_Base.h"

class CBasicStatic;
class CBasicButton;
class CListBox;

enum TitleSystem_Type
{
	TitleSystem_Regist,
	TitleSystem_Delete,

	TitleSystem_EndOfEnum
};

const int MAX_TITLE = 10;

class LHUI_TItleSystem // : public LHUI_Base
{
public:
	CBasicStatic * m_background;
	CBasicButton * m_btn_close;
	CBasicStatic * m_text;
	CBasicButton * m_btn_execute;

	CListBox * m_lst_title;


public:
	TitleSystem_Type m_type;

	int m_current_title;

	int m_my_title[MAX_TITLE];


public:
	virtual void Init();
	virtual void LoadRes();
	virtual void DeleteRes();
	virtual void Draw();

	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


public:
	LHUI_TItleSystem(TitleSystem_Type type);
	virtual ~LHUI_TItleSystem(void);
};
#endif // _LHUI_TItleSystem_H_