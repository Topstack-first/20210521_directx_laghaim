#include "stdafx.h"
#include "Nk2DFrame.h"
#include "sfxUtil.h"
#include "g_stringmanager.h"
#include "ControlLottery.h"

extern void SendLottery();

ControlLottery::ControlLottery()
	: m_curState(eLotteryState_Idle)
	, m_timeAccum(0)
	, m_result(-1)
	, m_surScratch(NULL)
	, m_coinSpeed(500)
	, m_coinDir(true)
	, m_coinPosX(0)
	, m_coinPosY(0)
	, m_frmScratch(0)
{
	m_time = GetTickCount();
}

ControlLottery::~ControlLottery()
{
	DeleteRes();
}

HRESULT ControlLottery::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlLottery::LoadRes()
{
	m_background.Init("LOTTERY/laglottery_board");
	m_btnStart.Init("LOTTERY/LL_start_button_01");
	m_btnClose.Init("common/btn_close_01");
	
	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnStart.SetPosition(bgx + 487, bgy + 209);
	m_btnClose.SetPosition(bgx + 613, bgy + 6);

	m_text_ItemCount.Init(12, RGB(255, 255, 255), bgx + 528, bgy + 155, 80, 21, TRUE);
	m_text_ItemCount.SetString((UINT64)0);

	g_pDisplay->CreateSurfaceFromBitmap(&m_surScratch, "INTERFACE/LOTTERY/LL_foil_seq.bmp");
	if( m_surScratch )
	{
		m_surScratch->SetColorKey(RGB(0, 0, 0));
	}

	m_imgCoin.Init("LOTTERY/laglottery_coin");
	m_imgCoin.SetPos(bgx + 265, bgy + 87);

	m_imgResult[0].Init("LOTTERY/LL_fail");
	m_imgResult[0].SetPos(bgx + 175, bgy + 62);		
	char buf[MAX_PATH] = {0};
	for(int i = 1; i < MAX_LOTTERY_GRADE; i++)
	{
		sprintf(buf, "LOTTERY/LL_%dst", i);
		m_imgResult[i].Init(buf);
		m_imgResult[i].SetPos(bgx + 175, bgy + 62);		
	}
	m_imgResult[1].SetPos(bgx + 120, bgy + 17);

	for(int i = 0; i < MAX_LOTTERY_SCRATCH_FRAME; i++)
	{
		m_rcScratch[i].left = 0;
		m_rcScratch[i].top = i * 63;
		m_rcScratch[i].right = 117;
		m_rcScratch[i].bottom = (i * 63) + 63 - 1;
	}
}

void ControlLottery::DeleteRes()
{
	m_background.DeleteRes();
	m_btnStart.DeleteRes();
	m_btnClose.DeleteRes();
	SAFE_DELETE(m_surScratch);
}

void ControlLottery::Init()
{
	m_btnStart.SetDisable2(false);

	SetState(eLotteryState_Idle);
}

void ControlLottery::Draw()
{
	if( !g_pDisplay )
		return;

	m_background.Draw();
	m_btnStart.Draw();
	m_btnClose.Draw();

	m_text_ItemCount.Draw();

	switch(m_curState)
	{
	case eLotteryState_Idle:
		DrawIdle();
		break;
	case eLotteryState_Scratch:
		DrawScratch();
		break;	
	case eLotteryState_WaitResult:
		DrawWaitResult();
		break;
	case eLotteryState_ShowResult:
		DrawShowResult();
		break;
	default:
		break;
	}
}

float ControlLottery::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	UpdateText(fElapsed);

	switch(m_curState)
	{
	case eLotteryState_Idle:
		UpdateIdle(fElapsed);
		break;
	case eLotteryState_Scratch:
		UpdateScratch(fElapsed);
		break;
	case eLotteryState_WaitResult:
		UpdateWaitResult(fElapsed);
		break;
	case eLotteryState_ShowResult:
		UpdateShowResult(fElapsed);
		break;
	default:
		break;
	}

	return fElapsed;
}

void ControlLottery::UpdateText(float elapsedTime)
{
	const int itemVnum = 3859;
	int count = g_pRoh->GetCountInPack(itemVnum);
	m_text_ItemCount.SetString(count);
}

void ControlLottery::UpdateIdle(float elapsedTime)
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_frmScratch = 0;

	m_coinPosX = 0;
	m_coinPosY = 0;
	m_coinDir = true;

	m_imgCoin.SetPos(bgx + 265, bgy + 87);
}

void ControlLottery::UpdateScratch(float elapsedTime)
{	
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	if( m_timeAccum >= 0.0f && m_timeAccum < 0.3f )
	{	
		m_frmScratch = 0;
		
		m_coinDir = true;
		m_coinPosX += elapsedTime * 70;
		m_coinPosY += elapsedTime * 150;
	}

	if( m_timeAccum >= 0.3f && m_timeAccum < 0.6f )
	{
		m_frmScratch = 1;

		m_coinDir = false;
		m_coinPosX += elapsedTime * 70;
		m_coinPosY -= elapsedTime * 150;
	}
	else if( m_timeAccum >= 0.6 && m_timeAccum < 0.9f )
	{
		m_frmScratch = 2;

		m_coinDir = true;
		m_coinPosX += elapsedTime * 70;
		m_coinPosY += elapsedTime * 150;
	}
	else if( m_timeAccum >= 0.9f && m_timeAccum < 1.2f )
	{
		m_frmScratch = 3;

		m_coinDir = false;
		m_coinPosX += elapsedTime * 70;
		m_coinPosY -= elapsedTime * 150;
	}
	
	m_imgCoin.SetPos(bgx + 265 + m_coinPosX, bgy + 136 + m_coinPosY);

	if( m_timeAccum < 1.2f )
	{
		return;
	}	

	SetState(eLotteryState_WaitResult);
}

void ControlLottery::UpdateWaitResult(float elapsedTime)
{
	if( m_result != -1 )
	{
		SetState(eLotteryState_ShowResult);
	}
}

void ControlLottery::UpdateShowResult(float elapsedTime)
{
	if( m_timeAccum < 1.0f )
		return;

	m_btnStart.SetDisable2(false);
	SetResult(-1);
	SetState(eLotteryState_Idle);
}

void ControlLottery::DrawIdle()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	g_pDisplay->Blt(bgx + 305, bgy + 154, m_surScratch, &m_rcScratch[m_frmScratch]);
}

void ControlLottery::DrawScratch()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	g_pDisplay->Blt(bgx + 305, bgy + 154, m_surScratch, &m_rcScratch[m_frmScratch]);

	m_imgCoin.Draw();
}

void ControlLottery::DrawWaitResult()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	g_pDisplay->Blt(bgx + 305, bgy + 154, m_surScratch, &m_rcScratch[m_frmScratch]);

	m_imgCoin.Draw();
}

void ControlLottery::DrawShowResult()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	g_pDisplay->Blt(bgx + 305, bgy + 154, m_surScratch, &m_rcScratch[m_frmScratch]);

	DrawResult();
}

void ControlLottery::DrawResult()
{
	if( m_result < 0 || m_result >= MAX_LOTTERY_GRADE )
		return;

	m_imgResult[m_result].Draw();
}

void ControlLottery::Action()
{
	switch(m_curState)
	{
	case eLotteryState_Idle:
		ActionIdle();
		break;
	case eLotteryState_Scratch:
		ActionScratch();
		break;
	case eLotteryState_WaitResult:
		ActionWaitResult();
		break;
	case eLotteryState_ShowResult:
		ActionShowResult();
		break;
	default:
		break;
	}
}

void ControlLottery::ActionIdle()
{
	if( CheckItemCount() == false )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LOTTERY_NOTIEM), TYPE_NOR_OK, 1);
		return;
	}

	SendLottery();
	SetState(eLotteryState_Scratch);

	m_btnStart.SetDisable2(true);
}

void ControlLottery::ActionScratch()
{
}

void ControlLottery::ActionWaitResult()
{
}

void ControlLottery::ActionShowResult()
{
}

LRESULT ControlLottery::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_background.IsIn(x, y) == false )
				return 0;

			m_btnStart.MsgProc(hWnd, msg, wParam, lParam);			
			m_btnClose.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnClose.GetState() == BTN_ACTION )
			{				
				m_btnClose.SetState(BTN_NORMAL);
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, LOTTERY);
			}

			if( m_btnStart.GetState() == BTN_ACTION )
			{
				m_btnStart.SetState(BTN_NORMAL);				
				Action();
			}

			return 1;
		}
		break;
	}
	return 0;
}

void ControlLottery::SetState(int state)
{
	m_curState = (eLotteryState)state;
	m_timeAccum = 0;
}

void ControlLottery::SetResult(int result)
{
	m_result = result;
}

bool ControlLottery::CheckItemCount()
{
	const int itemVnum = 3859;
	int count = g_pRoh->GetCountInPack(itemVnum);
	if( count <= 0 )
		return false;

	return true;
}
