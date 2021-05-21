#include "stdafx.h"
#include "ControlBaseball.h"
#include "Nk2DFrame.h"
#include "sfxUtil.h"
#include "g_stringmanager.h"
#include "BaseballManager.h"

extern void SendBaseballHit(const int type, const bool success);
extern void SendBaseballRewardListReq();

ControlBaseball::ControlBaseball()
	: m_curState(eBaseballState_Idle)
	, m_timeAccum(0)
	, m_ballSpeed(1.0f)
	, m_ballDir(false)
	, m_ballPosX(0.0f)
	, m_bSuccess(false)
	, m_typeBall(0)
	, m_ballResult(-1)
	, m_surPitcher(NULL)
	, m_frmPitcher(0)
	, m_frmHitter(0)
{
	m_time = GetTickCount();
}

ControlBaseball::~ControlBaseball()
{
	DeleteRes();
}

HRESULT ControlBaseball::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlBaseball::LoadRes()
{
	if( BaseballManager::GetInstance()->GetBaseballOn() == false )
	{
		return;
	}

	m_background.Init("baseball/baseball_board");
	m_btnStart.Init("baseball/BTN_GO");
	m_btnStop.Init("baseball/BTN_STOP");
	m_btnClose.Init("common/btn_close_01");
	m_btnBall1.Init("baseball/BTN_ball");
	m_btnBall2.Init("baseball/BTN_goldball");
	m_btnReward.Init("baseball/BTN_REWARD");
	m_ball.Init("baseball/baseball_medal", 37, 37);
	m_ballCursor.Init("baseball/ball_cursor", 33, 33);

	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnStart.SetPosition(bgx + 606, bgy + 382);
	m_btnStop.SetPosition(bgx + 606, bgy + 382);
	m_btnClose.SetPosition(bgx + 753, bgy + 5);
	m_btnBall1.SetPosition(bgx + 601, bgy + 318);
	m_btnBall2.SetPosition(bgx + 601, bgy + 273);
	m_btnReward.SetPosition(bgx + 254, bgy + 382);
	m_ball.SetPos(bgx + 61, bgy + 73);
	m_ballCursor.SetPos(bgx + 596, bgx + 268);

	char buf[MAX_PATH] = {0,};
	DWORD textColor = RGB(255, 255, 255);
	for(int i = 0; i < MAX_BASEBALL_GRADE; i++)
	{	
		m_text_BallCount[i].Init(12, textColor, bgx + 684, bgy + 211 - (26 * i), 32, 21, TRUE);		
		m_text_BallCount[i].SetString((UINT64)9999);
		
		sprintf(buf, "baseball/img_hit0%d", i);
		m_imgResult[i].Init(buf);
		m_imgResult[i].SetPos(bgx + 155, bgy + 145);
	}

	m_text_TypeCount[0].Init(12, textColor, bgx + 684, bgy + 324, 80, 21, TRUE);
	m_text_TypeCount[1].Init(12, textColor, bgx + 684, bgy + 280, 80, 21, TRUE);
	m_text_TypeCount[0].SetString((UINT64)0);
	m_text_TypeCount[1].SetString((UINT64)0);

	g_pDisplay->CreateSurfaceFromBitmap(&m_surPitcher, "INTERFACE/BASEBALL/pitcher_batter.BMP");
	if( m_surPitcher )
	{
		m_surPitcher->SetColorKey(RGB(0, 0, 0));
	}

	for( int i = 0; i < MAX_PICHER_MOTION_FRAME; i++)
	{
		m_rcPitcher[i].left = i * 81;
		m_rcPitcher[i].top = 0;
		m_rcPitcher[i].right = m_rcPitcher[i].left + 80;
		m_rcPitcher[i].bottom = 95;

		m_rcHitter[i].left = i * 106;
		m_rcHitter[i].top = 96;
		m_rcHitter[i].right = m_rcHitter[i].left + 105;
		m_rcHitter[i].bottom = 206;
	}
	

	m_btnStart.SetDisable2(false);
	m_btnStart.SetVisible(TRUE);
	m_btnStop.SetDisable2(true);
	m_btnStop.SetVisible(FALSE);

	SetBallType(0);

	m_ballSpeed = BaseballManager::GetInstance()->GetBallSpeed();
}

void ControlBaseball::DeleteRes()
{
	if( BaseballManager::GetInstance()->GetBaseballOn() == false )
	{
		return;
	}

	m_background.DeleteRes();
	m_btnStart.DeleteRes();
	m_btnStop.DeleteRes();
	m_btnClose.DeleteRes();
	m_btnBall1.DeleteRes();
	m_btnBall2.DeleteRes();
	m_btnReward.DeleteRes();
}

void ControlBaseball::Init()
{
 	m_btnStart.SetDisable2(false);
 	m_btnStart.SetVisible(TRUE);
	m_btnStop.SetDisable2(true);
	m_btnStop.SetVisible(FALSE);

	SetState(eBaseballState_Idle);
}

void ControlBaseball::Draw()
{
	if( !g_pDisplay )
		return;

 	m_background.Draw();
 	m_btnStart.Draw();
	m_btnStop.Draw();
 	m_btnClose.Draw();
 	m_btnReward.Draw();
 
 	if( m_typeBall == 0 )
 	{
 		m_btnBall1.SetState(BTN_ON);
 	}
 	else
 	{
 		m_btnBall2.SetState(BTN_ON);
 	}
 
 	m_btnBall1.Draw();
 	m_btnBall2.Draw();
	m_ballCursor.Draw();
 
 	for(int i = 0; i < MAX_BASEBALL_GRADE; i++)
 	{	
 		m_text_BallCount[i].Draw();
 	}
 
 	m_text_TypeCount[0].Draw();
 	m_text_TypeCount[1].Draw();

	switch(m_curState)
	{
	case eBaseballState_Idle:
		DrawIdle();
		break;
	case eBaseballState_Pitching:
		DrawPitching();
		break;
	case eBaseballState_Hitting:
		DrawHitting();
		break;
	case eBaseballState_WaitResult:
		DrawWaitResult();
		break;
	case eBaseballState_ShowResult:
		DrawShowResult();
		break;
	default:
		break;
	}
}

float ControlBaseball::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	UpdateText(fElapsed);

	switch(m_curState)
	{
	case eBaseballState_Idle:
		UpdateIdle(fElapsed);
		break;
	case eBaseballState_Pitching:
		UpdatePitching(fElapsed);
		break;
	case eBaseballState_Hitting:
		UpdateHitting(fElapsed);
		break;
	case eBaseballState_WaitResult:
		UpdateWaitResult(fElapsed);
		break;
	case eBaseballState_ShowResult:
		UpdateShowResult(fElapsed);
		break;
	default:
		break;
	}

	return fElapsed;
}

void ControlBaseball::UpdateText(float elapsedTime)
{
	const int wormItemVnum1 = 3823;
	const int wormItemVnum2 = 3824;
	int count1 = g_pRoh->GetCountInPack(wormItemVnum1);
	int count2 = g_pRoh->GetCountInPack(wormItemVnum2);
	m_text_TypeCount[0].SetString(count1);
	m_text_TypeCount[1].SetString(count2);

	BaseballManager* ballMgr = BaseballManager::GetInstance();
	int ballCount = 0;
	for(int i = 0; i < MAX_BASEBALL_GRADE; i++)
	{
		ballCount = ballMgr->GetBallCount(i);
		m_text_BallCount[i].SetString(ballCount);
	}
}

void ControlBaseball::UpdateIdle(float elapsedTime)
{
	m_frmPitcher = 0;
	m_frmHitter = 0;
}

void ControlBaseball::UpdatePitching(float elapsedTime)
{
	if( m_timeAccum > 0.3f && m_timeAccum < 0.6f )
		m_frmPitcher = 1;

	if( m_timeAccum > 0.6f )
		m_frmPitcher = 2;

	if( m_timeAccum < 1.0f )
		return;	

	m_ballPosX = RandomNum(0.0f, 421.0f);
	SetState(eBaseballState_Hitting);
	UpdateHitting(elapsedTime);
}

void ControlBaseball::UpdateHitting(float elapsedTime)
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	float fDir = 1.0f;
	if( m_ballDir == true )
	{
		fDir = -1.0f;
	}

	m_ballPosX += fDir * (elapsedTime * m_ballSpeed);
	int nPosX = (int)m_ballPosX;

	if( nPosX > 421 )
	{
		nPosX = 421;
		m_ballPosX = 421;
		m_ballDir = true;
	}
	else if( nPosX < 0 )
	{
		nPosX = 0;
		m_ballPosX = 0;
		m_ballDir = false;
	}

	m_ball.SetPos(bgx + 61 + nPosX, m_ball.m_y);
}

void ControlBaseball::UpdateWaitResult(float elapsedTime)
{
	if( m_timeAccum > 0.3f && m_timeAccum < 0.6f )
		m_frmHitter = 1;

	if( m_timeAccum > 0.6f )
		m_frmHitter = 2;

	if( m_timeAccum < 1.0f )
		return;

	if( m_ballResult != -1 )
	{
		SetState(eBaseballState_ShowResult);
	}
}

void ControlBaseball::UpdateShowResult(float elapsedTime)
{
	if( m_timeAccum < 1.0f )
		return;

	m_btnStart.SetDisable2(false);
	m_btnStart.SetVisible(TRUE);
	m_btnStop.SetDisable2(true);
	m_btnStop.SetVisible(FALSE);
	SetResult(-1);
	SetState(eBaseballState_Idle);
}

void ControlBaseball::DrawIdle()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;
	
	g_pDisplay->Blt(bgx + 50, bgy + 132, m_surPitcher, &m_rcPitcher[m_frmPitcher]);
	g_pDisplay->Blt(bgx + 445, bgy + 118, m_surPitcher, &m_rcHitter[m_frmHitter]);
}

void ControlBaseball::DrawPitching()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;	

	g_pDisplay->Blt(bgx + 50, bgy + 132, m_surPitcher, &m_rcPitcher[m_frmPitcher]);
	g_pDisplay->Blt(bgx + 445, bgy + 118, m_surPitcher, &m_rcHitter[m_frmHitter]);
}

void ControlBaseball::DrawHitting()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;	

	g_pDisplay->Blt(bgx + 50, bgy + 132, m_surPitcher, &m_rcPitcher[m_frmPitcher]);
	g_pDisplay->Blt(bgx + 445, bgy + 118, m_surPitcher, &m_rcHitter[m_frmHitter]);	

	m_ball.Draw();
}

void ControlBaseball::DrawWaitResult()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;	

	g_pDisplay->Blt(bgx + 50, bgy + 132, m_surPitcher, &m_rcPitcher[m_frmPitcher]);
	g_pDisplay->Blt(bgx + 445, bgy + 118, m_surPitcher, &m_rcHitter[m_frmHitter]);	

	m_ball.Draw();
}

void ControlBaseball::DrawShowResult()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;	

	g_pDisplay->Blt(bgx + 50, bgy + 132, m_surPitcher, &m_rcPitcher[m_frmPitcher]);
	g_pDisplay->Blt(bgx + 445, bgy + 118, m_surPitcher, &m_rcHitter[m_frmHitter]);

	m_ball.Draw();

	DrawResult();
}

void ControlBaseball::DrawResult()
{
	if( m_ballResult < 0 || m_ballResult >= MAX_BASEBALL_GRADE )
		return;

	m_imgResult[m_ballResult].Draw();
}

void ControlBaseball::Action()
{
	switch(m_curState)
	{
	case eBaseballState_Idle:
		ActionIdle();
		break;
	case eBaseballState_Pitching:
		ActionPitching();
		break;
	case eBaseballState_Hitting:
		ActionHitting();
		break;
	case eBaseballState_WaitResult:
		ActionWaitResult();
		break;
	case eBaseballState_ShowResult:
		ActionShowResult();
		break;
	default:
		break;
	}
}

void ControlBaseball::ActionIdle()
{
	if( CheckItemCount() == false )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_FISHING_NOTENOUGH), TYPE_NOR_OK, 1);
		return;
	}

	SetState(eBaseballState_Pitching);

	m_btnStart.SetDisable2(true);
	m_btnStart.SetVisible(FALSE);
	m_btnStop.SetDisable2(false);
	m_btnStop.SetVisible(TRUE);
}

void ControlBaseball::ActionPitching()
{

}

void ControlBaseball::ActionHitting()
{
	m_bSuccess = IsInSuccessArea();
	SendBaseballHit(m_typeBall, m_bSuccess);

	m_btnStop.SetDisable2(true);
	SetState(eBaseballState_WaitResult);
}

void ControlBaseball::ActionWaitResult()
{

}

void ControlBaseball::ActionShowResult()
{

}

LRESULT ControlBaseball::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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
			m_btnStop.MsgProc(hWnd, msg, wParam, lParam);
			m_btnClose.MsgProc(hWnd, msg, wParam, lParam);
			m_btnReward.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnClose.GetState() == BTN_ACTION )
			{				
				m_btnClose.SetState(BTN_NORMAL);
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, BASEBALL_REWARD);
			}

			if( m_btnStart.GetState() == BTN_ACTION )
			{
				m_btnStart.SetState(BTN_NORMAL);
				Action();
			}
			if( m_btnStop.GetState() == BTN_ACTION )
			{
				m_btnStop.SetState(BTN_NORMAL);
				Action();
			}
			if( m_btnReward.GetState() == BTN_ACTION )
			{
				SendBaseballRewardListReq();
				m_btnReward.SetState(BTN_NORMAL);
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(TRUE, BASEBALL_REWARD);
			}

			if( m_curState == eBaseballState_Idle )
			{
				m_btnBall1.MsgProc(hWnd, msg, wParam, lParam);
				m_btnBall2.MsgProc(hWnd, msg, wParam, lParam);

				if( m_btnBall1.GetState() == BTN_ACTION )
				{
					SetBallType(0);
				}
				if( m_btnBall2.GetState() == BTN_ACTION )
				{
					SetBallType(1);
				}
			}

			return 1;
		}
	}

	return 0;
}

bool ControlBaseball::IsInSuccessArea()
{
	if( m_ballPosX > 385 && m_ballPosX < 422 )
		return true;

	return false;
}

void ControlBaseball::SetState(int state)
{
 	m_curState = (eBaseballState)state;
 	m_timeAccum = 0;
}

void ControlBaseball::SetResult(int result)
{
	m_ballResult = result;
}

void ControlBaseball::SetBallType(int typeBall)
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_typeBall = typeBall;
	if( m_typeBall == 0 )
	{
		m_btnBall1.SetState(BTN_ON);
		m_btnBall2.SetState(BTN_NORMAL);
		m_ballCursor.SetPos(bgx + 596, bgy + 312);
	}
	else
	{
		m_btnBall1.SetState(BTN_NORMAL);
		m_btnBall2.SetState(BTN_ON);
		m_ballCursor.SetPos(bgx + 596, bgy + 268);
	}
}

void ControlBaseball::SetBallSpeed(int speed)
{
	m_ballSpeed = (float)speed;
}

bool ControlBaseball::CheckItemCount()
{
	const int batItemVnum = 3825;
	const int ballItemVnum1 = 3823;
	const int ballItemVnum2 = 3824;
	int count0 = g_pRoh->GetCountInPack(batItemVnum);

	if( count0 <= 0 )
		return false;

	int count1 = g_pRoh->GetCountInPack(ballItemVnum1);
	int count2 = g_pRoh->GetCountInPack(ballItemVnum2);

	if( m_typeBall == 0 )
	{
		if( count1 > 0 )
			return true;
	}
	else
	{
		if( count2 > 0 )
			return true;
	}

	return false;
}
