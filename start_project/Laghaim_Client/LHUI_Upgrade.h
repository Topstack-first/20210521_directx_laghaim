#pragma once

#include "LHUI_Base.h"

class CBasicStatic;
class CBasicButton;


enum eUpgrade_Type
{
	eUpgrade__Weapon,
	eUpgrade__Armor,
	eUpgrade__Conversion,
	eUpgrade__Confusion,
	eUpgrade__Process,

	eUpgrade__EndOfEnum
};


class LHUI_Upgrade_Strategy_Base
{
public:
	CBasicStatic * m_desc_info;
	CBasicButton * m_btn_execute;


public:
	virtual void Init() = 0;
	virtual void LoadRes() = 0;
	virtual void DeleteRes() = 0;
	virtual void Draw() = 0;

	virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

public:
	LHUI_Upgrade_Strategy_Base();
	virtual ~LHUI_Upgrade_Strategy_Base();
};

class LHUI_Upgrade_Strategy_Upgrade : public LHUI_Upgrade_Strategy_Base
{
public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	LHUI_Upgrade_Strategy_Upgrade() {}
	virtual ~LHUI_Upgrade_Strategy_Upgrade() {}
};


class LHUI_Upgrade_Strategy_Conversion : public LHUI_Upgrade_Strategy_Base
{
public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	LHUI_Upgrade_Strategy_Conversion() {}
	virtual ~LHUI_Upgrade_Strategy_Conversion() {}
};


class LHUI_Upgrade_Strategy_Confusion : public LHUI_Upgrade_Strategy_Base
{
public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:
	LHUI_Upgrade_Strategy_Confusion() {}
	virtual ~LHUI_Upgrade_Strategy_Confusion() {}
};

class LHUI_Upgrade : public LHUI_Base
{
public:
	CBasicStatic * m_bg;
	CBasicButton * m_btn_close;
	CBasicButton * m_btn_execute;



public:
	void Init();
	void LoadRes();
	void LoadRes(eUpgrade_Type etype);
	void DeleteRes();
	void Draw();

	void SetPos(int x, int y);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:
	LHUI_Upgrade_Strategy_Base * m_strategy;

	void SetType(eUpgrade_Type etype);


public:
	LHUI_Upgrade(void);
	virtual ~LHUI_Upgrade(void);
};
