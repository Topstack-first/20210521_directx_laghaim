#include "stdafx.h"
#include "ControlDecardiBattle.h"
#include "DecardiBattleManager.h"
#include "Nk2DFrame.h"
#include "main.h"
#include "UIMgr.h"

ControlDecardiBattle::ControlDecardiBattle()
	: m_bVisible(false)
	, m_nWinLoseDraw(0)
	, m_dwEndTime(0)	
{
	LoadRes();
}

ControlDecardiBattle::~ControlDecardiBattle()
{
	DeleteRes();
}

void ControlDecardiBattle::LoadRes()
{
	m_background.Init("WARINFO/DCB_SCORE");
	m_background.Align(1, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());
	m_background.m_y += 38;

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_imgWin.Init("WARINFO/DCB_WIN");
	m_imgWin.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());
	m_imgLose.Init("WARINFO/DCB_LOSE");
	m_imgLose.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());
	m_imgDraw.Init("WARINFO/DCB_DRAW");
	m_imgDraw.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());
	
	m_text_RedTeam.Init(12, RGB(246, 88, 88), bgx + 41, bgy + 9, 20, 21, TRUE);
	m_text_RedTeam.SetString("¶ó±×ÆÀ");
	m_text_BlueTeam.Init(12, RGB(48, 158, 243), bgx + 92, bgy + 9, 20, 21, TRUE);
	m_text_BlueTeam.SetString("ÇÏÀÓÆÀ");
	
	m_text_RedPoint.Init(12, RGB(242, 238, 113), bgx + 41, bgy + 26, 20, 21, TRUE);
	m_text_RedPoint.SetString("0");
	m_text_BluePoint.Init(12, RGB(242, 238, 113), bgx + 92, bgy + 26, 20, 21, TRUE);
	m_text_BluePoint.SetString("0");
	m_text_Time.Init(12, RGB(255, 255, 255), bgx + 65, bgy + 45, 34, 21, TRUE);
	m_text_Time.SetString("30:00");
}

void ControlDecardiBattle::DeleteRes()
{
	m_background.DeleteRes();
	m_imgWin.DeleteRes();
	m_imgLose.DeleteRes();
	m_imgDraw.DeleteRes();
}

float ControlDecardiBattle::Update()
{
	if( m_dwEndTime == 0 )
		return 0;

	DWORD dwCurTime = GetTickCount();
	DWORD dwElapsed = dwCurTime - m_dwEndTime;

	if( dwElapsed < 10000 )
		return 0;

	m_bVisible = false;
	m_dwEndTime = 0;

	return 0;
}

void ControlDecardiBattle::Draw()
{
	if( m_bVisible == false )
		return;

	drawPoint();
	drawResult();
}

void ControlDecardiBattle::drawPoint()
{
	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	int redPoint = dcbMgr->GetPoint(0);
	int bluePoint = dcbMgr->GetPoint(1);
	int min = 0;
	int sec = 0;
	dcbMgr->GetTime(min, sec);
	char buf[MAX_PATH] = {0};
	
	sprintf(buf, "%d", redPoint);
	m_text_RedPoint.SetString(buf);
	sprintf(buf, "%d", bluePoint);
	m_text_BluePoint.SetString(buf);
	sprintf(buf, "%02d:%02d", min, sec);
	m_text_Time.SetString(buf);

	m_background.Draw();
	m_text_RedTeam.Draw();
	m_text_BlueTeam.Draw();
	m_text_RedPoint.Draw();
	m_text_BluePoint.Draw();
	m_text_Time.Draw();
}

void ControlDecardiBattle::drawResult()
{
	if( m_dwEndTime == 0 )
		return;

	if( m_nWinLoseDraw == 0 )
	{
		m_imgWin.Draw();
	}
	else if( m_nWinLoseDraw == 1 )
	{
		m_imgLose.Draw();
	}
	else if( m_nWinLoseDraw == 2 )
	{
		m_imgDraw.Draw();
	}
}

void ControlDecardiBattle::Start()
{
	m_bVisible = true;

	if( pCMyApp && pCMyApp->m_pUIMgr )
	{
		pCMyApp->m_pUIMgr->ProcessOneSentence("namehide 1\n");
	}
}

void ControlDecardiBattle::End(const int result)
{
	m_dwEndTime = GetTickCount();

	m_nWinLoseDraw = result;	

	if( pCMyApp && pCMyApp->m_pUIMgr )
	{
		pCMyApp->m_pUIMgr->ProcessOneSentence("namehide 0\n");
	}
}

LRESULT ControlDecardiBattle::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}