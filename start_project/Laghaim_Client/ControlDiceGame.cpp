#include "stdafx.h"
#include "Nk2DFrame.h"
#include "sfxUtil.h"
#include "g_stringmanager.h"
#include "DiceGameManager.h"
#include "PacketSend.h"
#include "ControlDiceGame.h"

POINT ptBoard[36] = 
{
	{ 62,369}, {123,369}, {184,369}, {245,369}, {306,369}, {367,369},
	{367,308}, {306,308}, {245,308}, {184,308}, {123,308}, { 62,308},
	{ 62,247}, {123,247}, {184,247}, {245,247}, {306,247}, {367,247},
	{367,186}, {306,186}, {245,186}, {184,186}, {123,186}, { 62,186},
	{ 62,125}, {123,125}, {184,125}, {245,125}, {306,125}, {367,125},
	{367, 64}, {306, 64}, {245, 64}, {184, 64}, {123, 64}, { 62, 64}
};

ControlDiceGame::ControlDiceGame()
	: m_curState(eDiceGameState_Idle)
	, m_timeAccum(0)
	, m_frmDice(0)
	, m_curPos(-1)
	, m_curDice(0)
	, m_cntComplete(0)
{
	for(int i = 0; i < 6; ++i)
	{
		m_surDice[i] = NULL;
	}

	m_rcDiceSeq[0].left = 0;
	m_rcDiceSeq[0].top = 0;
	m_rcDiceSeq[0].right = 270;
	m_rcDiceSeq[0].bottom = 255;
	m_rcDiceSeq[1].left = 270;
	m_rcDiceSeq[1].top = 0;
	m_rcDiceSeq[1].right = 540;
	m_rcDiceSeq[1].bottom = 255;
	m_rcDiceSeq[2].left = 540;
	m_rcDiceSeq[2].top = 0;
	m_rcDiceSeq[2].right = 810;
	m_rcDiceSeq[2].bottom = 255;

	m_rcDiceSeq[3].left = 0;
	m_rcDiceSeq[3].top = 255;
	m_rcDiceSeq[3].right = 270;
	m_rcDiceSeq[3].bottom = 510;
	m_rcDiceSeq[4].left = 270;
	m_rcDiceSeq[4].top = 255;
	m_rcDiceSeq[4].right = 540;
	m_rcDiceSeq[4].bottom = 510;
	m_rcDiceSeq[5].left = 540;
	m_rcDiceSeq[5].top = 255;
	m_rcDiceSeq[5].right = 810;
	m_rcDiceSeq[5].bottom = 510;

	m_rcDiceSeq[6].left = 0;
	m_rcDiceSeq[6].top = 510;
	m_rcDiceSeq[6].right = 270;
	m_rcDiceSeq[6].bottom = 765;
	m_rcDiceSeq[7].left = 270;
	m_rcDiceSeq[7].top = 510;
	m_rcDiceSeq[7].right = 540;
	m_rcDiceSeq[7].bottom = 765;
	m_rcDiceSeq[8].left = 540;
	m_rcDiceSeq[8].top = 510;
	m_rcDiceSeq[8].right = 810;
	m_rcDiceSeq[8].bottom = 765;

	m_rcDiceSeq[9].left = 0;
	m_rcDiceSeq[9].top = 765;
	m_rcDiceSeq[9].right = 270;
	m_rcDiceSeq[9].bottom = 1020;
}

ControlDiceGame::~ControlDiceGame()
{
	DeleteRes();
}

HRESULT ControlDiceGame::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlDiceGame::LoadRes()
{
	if( DiceGameManager::GetInstance()->GetDiceGameOn() == false )
	{
		return;
	}

	m_background.Init("dicegame/dice_board");
	m_btnStart.Init("dicegame/dice_board_start_button");
	m_btnClose.Init("common/btn_close_01");
	m_btnReward.Init("dicegame/dice_board_product_button");

	m_imgPlayer.Init("dicegame/player01");
	m_imgComplete.Init("dicegame/complete");

	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnStart.SetPosition(bgx + 617, bgy + 388);
	m_btnClose.SetPosition(bgx + 753, bgy + 5);
	m_btnReward.SetPosition(bgx + 481, bgy + 398);

	m_imgPlayer.SetPos(-58, -58);
	m_imgComplete.SetPos(bgx + 49, bgy + 88);

	DWORD textColor = RGB(255, 255, 255);
	m_text_CompleteCount.Init(12, textColor, bgx + 516, bgy + 343, 10, TRUE);
	m_text_DiceCount.Init(12, textColor, bgx + 650, bgy + 343, 10, TRUE);
	m_text_CompleteCount.SetString((UINT64)0);
	m_text_DiceCount.SetString((UINT64)0);

	char buf[MAX_PATH] = {0};
	for(int i = 0; i < 6; ++i)
	{
		sprintf(buf, "INTERFACE/DICEGAME/DICE_SEQ_0%d.BMP", i + 1);
		g_pDisplay->CreateSurfaceFromBitmap(&m_surDice[i], buf);
		if( m_surDice[i] )
		{
			m_surDice[i]->SetColorKey(RGB(0, 0, 0));
		}
	}

	m_btnStart.SetDisable2(false);
	m_btnStart.SetVisible(TRUE);
}

void ControlDiceGame::DeleteRes()
{
	if( DiceGameManager::GetInstance()->GetDiceGameOn() == false )
	{
		return;
	}

	m_background.DeleteRes();
	m_btnStart.DeleteRes();
	m_btnClose.DeleteRes();
	m_btnReward.DeleteRes();
	m_imgPlayer.DeleteRes();
	m_imgComplete.DeleteRes();

	for(int i = 0; i < 6; ++i)
	{
		SAFE_DELETE(m_surDice[i]);
	}
}

void ControlDiceGame::Init()
{
	m_frmDice = 0;
	m_curPos = -1;
	m_curDice = 0;
	SetState(eDiceGameState_Idle);
}

void ControlDiceGame::Draw()
{
	if( !g_pDisplay )
		return;

	m_background.Draw();
	m_btnStart.Draw();
	m_btnClose.Draw();
	m_btnReward.Draw();

	m_text_DiceCount.Draw();
	m_text_CompleteCount.Draw();

	m_imgPlayer.Draw();

	switch(m_curState)
	{
	case eDiceGameState_Idle:
		DrawIdle();
		break;
	case eDiceGameState_WaitDice:
		DrawWaitDice();
		break;
	case eDiceGameState_ShowDice:
		DrawShowDice();
		break;
	case eDiceGameState_MovePlayer:
		DrawMovePlayer();
		break;
	case eDiceGameState_Complete:
		DrawComplete();
		break;
	default:
		break;
	}
}

float ControlDiceGame::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	UpdateText(fElapsed);

	switch(m_curState)
	{
	case eDiceGameState_Idle:
		UpdateIdle(fElapsed);
		break;
	case eDiceGameState_WaitDice:
		UpdateWaitDice(fElapsed);
		break;
	case eDiceGameState_ShowDice:
		UpdateShowDice(fElapsed);
		break;
	case eDiceGameState_MovePlayer:
		UpdateMovePlayer(fElapsed);
		break;
	case eDiceGameState_Complete:
		UpdateComplete(fElapsed);
		break;
	default:
		break;
	}

	return fElapsed;
}

void ControlDiceGame::UpdateText(float elapsedTime)
{
	const int diceItemVnum = 3879;
	int count = g_pRoh->GetCountInPack(diceItemVnum);
	m_text_DiceCount.SetString(count);	
	m_text_CompleteCount.SetString(m_cntComplete);
}

void ControlDiceGame::UpdateIdle(float elapsedTime)
{
	m_frmDice = 0;
}

void ControlDiceGame::UpdateWaitDice(float elapsedTime)
{
}

void ControlDiceGame::UpdateShowDice(float elapsedTime)
{
	float tmp = m_timeAccum / 0.07f;
	if( tmp > 9 )
	{
		m_frmDice = 9;
	}
	else
	{
		m_frmDice = (int)tmp;
	}

	if( m_frmDice < 9 )
		return;

	SetState(eDiceGameState_MovePlayer);	
}

void ControlDiceGame::UpdateMovePlayer(float elapsedTime)
{
	float ftmp = m_timeAccum / 0.2f;
	int tmp = (int)ftmp;
	int count = m_path.size();
	if( tmp < 1 )
	{
		return;
	}
	
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;
	if( tmp <= count )
	{
		m_curPos = m_path[tmp - 1];
		m_imgPlayer.SetPos(bgx + ptBoard[m_curPos].x + 1, bgy + ptBoard[m_curPos].y + 1);
	}
	else
	{
		m_curPos = m_path[count - 1];
		m_imgPlayer.SetPos(bgx + ptBoard[m_curPos].x + 1, bgy + ptBoard[m_curPos].y + 1);

		if( m_curPos == 35 )
		{
			SetState(eDiceGameState_Complete);			
		}
		else
		{
			SetState(eDiceGameState_Idle);
			m_btnStart.SetDisable2(false);
		}
	}
}

void ControlDiceGame::UpdateComplete(float elapsedTime)
{
	if( m_timeAccum < 1.0f )
		return;

	SetState(eDiceGameState_Idle);
	m_btnStart.SetDisable2(false);
		
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;
	m_curPos = 0;
	m_imgPlayer.SetPos(bgx + ptBoard[m_curPos].x + 1, bgy + ptBoard[m_curPos].y + 1);
}

void ControlDiceGame::DrawIdle()
{
}

void ControlDiceGame::DrawWaitDice()
{
}

void ControlDiceGame::DrawShowDice()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	if( m_curDice <= 0 || m_curDice > 6 )
		return;

	g_pDisplay->Blt(bgx + 456, bgy + 49, m_surDice[m_curDice - 1], &m_rcDiceSeq[m_frmDice]);
}

void ControlDiceGame::DrawMovePlayer()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	if( m_curDice <= 0 || m_curDice > 6 )
		return;

	g_pDisplay->Blt(bgx + 456, bgy + 49, m_surDice[m_curDice - 1], &m_rcDiceSeq[MAX_DICE_FRAME - 1]);
}

void ControlDiceGame::DrawComplete()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_imgComplete.Draw();

	if( m_curDice <= 0 || m_curDice > 6 )
		return;

	g_pDisplay->Blt(bgx + 456, bgy + 49, m_surDice[m_curDice - 1], &m_rcDiceSeq[MAX_DICE_FRAME - 1]);
}

void ControlDiceGame::Action()
{
	switch(m_curState)
	{
	case eDiceGameState_Idle:
		ActionIdle();
		break;
	case eDiceGameState_WaitDice:
		ActionWaitDice();
		break;
	case eDiceGameState_ShowDice:
		ActionShowDice();
		break;
	case eDiceGameState_MovePlayer:
		ActionMovePlayer();
		break;
	case eDiceGameState_Complete:
		ActionComplete();
		break;
	default:
		break;
	}
}

void ControlDiceGame::ActionIdle()
{
	if( CheckItemCount() == false )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_FISHING_NOTENOUGH), TYPE_NOR_OK, 1);
		return;
	}

	m_btnStart.SetDisable2(true);

	SendSnakeDiceThrow();
	SetState(eDiceGameState_WaitDice);
}

void ControlDiceGame::ActionWaitDice()
{
}

void ControlDiceGame::ActionShowDice()
{
}

void ControlDiceGame::ActionMovePlayer()
{
}

void ControlDiceGame::ActionComplete()
{
}

LRESULT ControlDiceGame::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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
			m_btnReward.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnClose.GetState() == BTN_ACTION )
			{				
				m_btnClose.SetState(BTN_NORMAL);
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, DICEGAME);
			}
			if( m_btnStart.GetState() == BTN_ACTION )
			{
				m_btnStart.SetState(BTN_NORMAL);
				Action();
			}
			if( m_btnReward.GetState() == BTN_ACTION )
			{
				SendSnakeDicePresentList();
				m_btnReward.SetState(BTN_NORMAL);
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(TRUE, DICEGAME_REWARD);
			}

			return 1;
		}		
	}

	return 0;
}

void ControlDiceGame::SetState(int state)
{
	m_curState = (eDiceGameState)state;
	m_timeAccum = 0;
}

void ControlDiceGame::SetPath(int curDice, std::vector<int>& path)
{
	m_curDice = curDice;
	m_path.assign(path.begin(), path.end());
}

bool ControlDiceGame::CheckItemCount()
{
	const int diceItemVnum = 3879;
	int count = g_pRoh->GetCountInPack(diceItemVnum);
	if( count <= 0 )
		return false;

	return true;
}

void ControlDiceGame::SetPlayerPos(int pos)
{
	m_cntComplete = DiceGameManager::GetInstance()->GetSuccessCount();

	if( m_curState != eDiceGameState_Idle )
		return;

	if( pos == -1 )
	{
		m_imgPlayer.SetPos(-58, -58);
	}
	else
	{
		int bgx = m_background.m_x;
		int bgy = m_background.m_y;
		m_curPos = pos;
		m_imgPlayer.SetPos(bgx + ptBoard[m_curPos].x + 1, bgy + ptBoard[m_curPos].y + 1);		
	}
}
