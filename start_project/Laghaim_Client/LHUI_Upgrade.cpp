#include "stdafx.h"
#include ".\lhui_upgrade.h"
#include "BasicStatic.h"
#include "basicbutton.h"

//=============================================================================
LHUI_Upgrade_Strategy_Base::LHUI_Upgrade_Strategy_Base()
	: m_desc_info(NULL)
	, m_btn_execute(NULL)
{

}

LHUI_Upgrade_Strategy_Base::~LHUI_Upgrade_Strategy_Base()
{
	SAFE_DELETE(m_desc_info);
	SAFE_DELETE(m_btn_execute);
}


//=============================================================================
void LHUI_Upgrade_Strategy_Upgrade::Init()
{

}

void LHUI_Upgrade_Strategy_Upgrade::LoadRes()
{

}

void LHUI_Upgrade_Strategy_Upgrade::DeleteRes()
{

}

void LHUI_Upgrade_Strategy_Upgrade::Draw()
{
	if( m_desc_info )	m_desc_info->Draw();
	if( m_btn_execute ) m_btn_execute->Draw();
}

//=============================================================================
void LHUI_Upgrade_Strategy_Conversion::Init()
{

}

void LHUI_Upgrade_Strategy_Conversion::LoadRes()
{

}

void LHUI_Upgrade_Strategy_Conversion::DeleteRes()
{

}

void LHUI_Upgrade_Strategy_Conversion::Draw()
{

}

LRESULT LHUI_Upgrade_Strategy_Conversion::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}


//=============================================================================
void LHUI_Upgrade_Strategy_Confusion::Init()
{

}

void LHUI_Upgrade_Strategy_Confusion::LoadRes()
{

}

void LHUI_Upgrade_Strategy_Confusion::DeleteRes()
{

}

void LHUI_Upgrade_Strategy_Confusion::Draw()
{

}

LRESULT LHUI_Upgrade_Strategy_Confusion::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}




//=============================================================================
LHUI_Upgrade::LHUI_Upgrade(void)
	: m_bg(NULL)
	, m_btn_close(NULL)
	, m_btn_execute(NULL)
	, m_strategy(NULL)
{
}

LHUI_Upgrade::~LHUI_Upgrade(void)
{
}

void LHUI_Upgrade::Init()
{

}

void LHUI_Upgrade::LoadRes()
{

}

void LHUI_Upgrade::LoadRes(eUpgrade_Type etype)
{

}

void LHUI_Upgrade::DeleteRes()
{

}

void LHUI_Upgrade::Draw()
{

}

void LHUI_Upgrade::SetPos(int x, int y)
{

}

LRESULT LHUI_Upgrade::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

