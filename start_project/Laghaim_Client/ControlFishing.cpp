#include "stdafx.h"
#include "ControlFishing.h"
#include "Nk2DFrame.h"
#include "sfxUtil.h"
#include "g_stringmanager.h"
#include "FishingManager.h"

extern void SendFishThrow(const int& wormType, const bool& success);
extern void SendFishRewardListReq();

ControlFishing::ControlFishing()
	: m_surFishingRod(NULL)
	, m_curState(eFishingState_Idle)
	, m_timeAccum(0)
	, m_ballSpeed(1.0f)
	, m_ballDir(false)
	, m_ballPosX(0.0f)
	, m_throwDivide(1.0f)
	, m_bSuccess(false)
	, m_typeWorm(0)
	, m_fishResult(-1)
{
	m_time = GetTickCount();
}

ControlFishing::~ControlFishing()
{
	DeleteRes();
}

HRESULT ControlFishing::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlFishing::LoadRes()
{
	if( FishingManager::GetInstance()->GetFishingOn() == false )
	{
		return;
	}

	m_background.Init("fishing/fishing_board");
	m_btnStart.Init("fishing/BTN_GO");
	m_btnStop.Init("fishing/BTN_STOP");
	m_btnClose.Init("common/btn_close_01");
	m_btnWorm1.Init("fishing/BTN_earthworm");
	m_btnWorm2.Init("fishing/BTN_lugworm");
	m_btnReward.Init("fishing/BTN_REWARD");
	m_ball.Init("fishing/fishing_medal", 37, 37);
	m_imgGo.Init("fishing/TEXT_GO");
	m_imgSuccess.Init("fishing/TEXT_SUCCESS");
	m_imgFail.Init("fishing/TEXT_FAIL");

	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnStart.SetPosition(bgx + 432, bgy + 399);
	m_btnStop.SetPosition(bgx + 432, bgy + 399);
	m_btnClose.SetPosition(bgx + 753, bgy + 5);
	m_btnWorm1.SetPosition(bgx + 53, bgy + 370);
	m_btnWorm2.SetPosition(bgx + 53, bgy + 406);
	m_btnReward.SetPosition(bgx + 193, bgy + 417);
	m_ball.SetPos(bgx + 140, bgy + 40);
	m_imgGo.SetPos(bgx + 301, bgy + 128);
	m_imgSuccess.SetPos(bgx + 256, bgy + 86);
	m_imgFail.SetPos(bgx + 360, bgy + 105);

	FishingManager* fishMgr = FishingManager::GetInstance();
	char buf[MAX_PATH] = {0,};
	std::string filename;
	POINT fishOffset;
	DWORD textColor = RGB(255, 255, 255);
	for(int i = 0; i < MAX_FISHTYPE; i++)
	{
		m_text_FishType[i].Init(12, textColor, bgx + 92, bgy + 82 + (22 * i), 86, 21, TRUE);
		m_text_FishCount[i].Init(12, textColor, bgx + 156, bgy + 82 + (22 * i), 32, 21, TRUE);

		m_text_FishType[i].SetString((char*)G_STRING(IDS_FISHTYPE_1 + i), textColor);
		m_text_FishCount[i].SetString((UINT64)9999);

		filename = fishMgr->GetFileName(i);
		sprintf(buf, "fishing/%s", filename.c_str());
		m_imgResult[i].Init(buf);

		fishOffset = fishMgr->GetOffset(i);
		m_imgResult[i].SetPos(bgx + 419 - fishOffset.x, bgy + 310 - fishOffset.y);
	}

	m_text_WormCount[0].Init(12, textColor, bgx + 129, bgy + 376, 80, 21, TRUE);
	m_text_WormCount[1].Init(12, textColor, bgx + 129, bgy + 412, 80, 21, TRUE);
	m_text_WormCount[0].SetString((UINT64)0);
	m_text_WormCount[1].SetString((UINT64)0);

	m_text_Result.Init(12, textColor, bgx + 476, bgy + 323, 84, 21, TRUE);
	m_text_Result.SetString("");

	g_pDisplay->CreateSurfaceFromBitmap(&m_surFishingRod, "INTERFACE/FISHING/float_seq.BMP");
	if( m_surFishingRod )
	{
		m_surFishingRod->SetColorKey(RGB(0, 0, 0));
	}

	int index = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			index = (i * 8) + j;
			if( index >= MAX_FISHING_THROW_FRAME )
				break;

			m_rcFishingRod[index].left = j * 64;
			m_rcFishingRod[index].top = i * 64;
			m_rcFishingRod[index].right = j * 64 + 64;
			m_rcFishingRod[index].bottom = i * 64 + 64;
		}
	}

	m_btnStart.SetDisable2(false);
	m_btnStart.SetVisible(TRUE);
	m_btnStop.SetDisable2(true);
	m_btnStop.SetVisible(FALSE);

	SetWormType(0);

	m_ballSpeed = fishMgr->GetBallSpeed();
}

void ControlFishing::DeleteRes()
{
	if( FishingManager::GetInstance()->GetFishingOn() == false )
	{
		return;
	}

	m_background.DeleteRes();
	m_btnStart.DeleteRes();
	m_btnStop.DeleteRes();
	m_btnClose.DeleteRes();
	m_btnWorm1.DeleteRes();
	m_btnWorm2.DeleteRes();
	m_btnReward.DeleteRes();

	SAFE_DELETE(m_surFishingRod);
}

void ControlFishing::Init()
{
	m_btnStart.SetDisable2(false);
	m_btnStart.SetVisible(TRUE);
	m_btnStop.SetDisable2(true);
	m_btnStop.SetVisible(FALSE);

	SetState(eFishingState_Idle);
}

void ControlFishing::Draw()
{
	if( !g_pDisplay )
		return;

	m_background.Draw();
	m_btnStart.Draw();
	m_btnStop.Draw();
	m_btnClose.Draw();
	m_btnReward.Draw();

	if( m_typeWorm == 0 )
	{
		m_btnWorm1.SetState(BTN_ON);
	}
	else
	{
		m_btnWorm2.SetState(BTN_ON);
	}

	m_btnWorm1.Draw();
	m_btnWorm2.Draw();

	for(int i = 0; i < MAX_FISHTYPE; i++)
	{
		m_text_FishType[i].Draw();
		m_text_FishCount[i].Draw();
	}

	m_text_WormCount[0].Draw();
	m_text_WormCount[1].Draw();

	switch(m_curState)
	{
	case eFishingState_Idle:
		DrawIdle();
		break;
	case eFishingState_Throw:
		DrawThrow();
		break;
	case eFishingState_Fishing:
		DrawFishing();
		break;
	case eFishingState_WaitResult:
		DrawWaitResult();
		break;
	case eFishingState_ShowResult:
		DrawShowResult();
		break;
	default:
		break;
	}
}

float ControlFishing::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	UpdateText(fElapsed);

	switch(m_curState)
	{
	case eFishingState_Idle:
		UpdateIdle(fElapsed);
		break;
	case eFishingState_Throw:
		UpdateThrow(fElapsed);
		break;
	case eFishingState_Fishing:
		UpdateFishing(fElapsed);
		break;
	case eFishingState_WaitResult:
		UpdateWaitResult(fElapsed);
		break;
	case eFishingState_ShowResult:
		UpdateShowResult(fElapsed);
		break;
	default:
		break;
	}

	return fElapsed;
}

void ControlFishing::UpdateText(float elapsedTime)
{
	const int wormItemVnum1 = 3756;
	const int wormItemVnum2 = 3757;
	int count1 = g_pRoh->GetCountInPack(wormItemVnum1);
	int count2 = g_pRoh->GetCountInPack(wormItemVnum2);
	m_text_WormCount[0].SetString(count1);
	m_text_WormCount[1].SetString(count2);

	FishingManager* fishMgr = FishingManager::GetInstance();
	int fishCount = 0;
	for(int i = 0; i < MAX_FISHTYPE; i++)
	{
		fishCount = fishMgr->GetFishCount(i);
		m_text_FishCount[i].SetString(fishCount);
	}
}

void ControlFishing::UpdateIdle(float elapsedTime)
{

}

void ControlFishing::UpdateThrow(float elapsedTime)
{
	if( m_timeAccum < 1.0f )
		return;

	m_ballPosX = RandomNum(0.0f, 347.0f);
	SetState(eFishingState_Fishing);
	UpdateFishing(elapsedTime);
}

void ControlFishing::UpdateFishing(float elapsedTime)
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

	if( nPosX > 347 )
	{
		nPosX = 347;
		m_ballPosX = 347;
		m_ballDir = true;
	}
	else if( nPosX < 0 )
	{
		nPosX = 0;
		m_ballPosX = 0;
		m_ballDir = false;
	}

	m_ball.SetPos(bgx + 280 + nPosX, m_ball.m_y);
}

void ControlFishing::UpdateWaitResult(float elapsedTime)
{
	if( m_timeAccum < 1.0f )
		return;

	if( m_fishResult != -1 )
	{
		SetState(eFishingState_ShowResult);
	}
}

void ControlFishing::UpdateShowResult(float elapsedTime)
{
	if( m_timeAccum < 3.0f )
		return;

	m_btnStart.SetDisable2(false);
	m_btnStart.SetVisible(TRUE);
	m_btnStop.SetDisable2(true);
	m_btnStop.SetVisible(FALSE);
	SetResult(-1);
	SetState(eFishingState_Idle);
}

void ControlFishing::DrawIdle()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;
	int curFrame = 0;

	g_pDisplay->Blt(bgx + 445, bgy + 232, m_surFishingRod, &m_rcFishingRod[curFrame]);
}

void ControlFishing::DrawThrow()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;
	int curFrame = 0;

	g_pDisplay->Blt(bgx + 445, bgy + 232, m_surFishingRod, &m_rcFishingRod[curFrame]);

	m_imgGo.Draw();
}

void ControlFishing::DrawFishing()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	int curFrame = (int)(MAX_FISHING_THROW_FRAME * m_timeAccum / 3.0);
	if( curFrame < 0 )
		curFrame = 0;

	curFrame %= MAX_FISHING_THROW_FRAME;

	if( curFrame >= MAX_FISHING_THROW_FRAME )
		curFrame = MAX_FISHING_THROW_FRAME - 1;

	g_pDisplay->Blt(bgx + 445, bgy + 232, m_surFishingRod, &m_rcFishingRod[curFrame]);

	m_ball.Draw();
}

void ControlFishing::DrawWaitResult()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;
	int curFrame = 0;

	g_pDisplay->Blt(bgx + 445, bgy + 232, m_surFishingRod, &m_rcFishingRod[curFrame]);

	m_ball.Draw();

	if( m_bSuccess == true )
	{
		m_imgSuccess.Draw();
	}
	else
	{
		m_imgFail.Draw();
	}
}

void ControlFishing::DrawShowResult()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;
	int curFrame = 0;

	g_pDisplay->Blt(bgx + 445, bgy + 232, m_surFishingRod, &m_rcFishingRod[curFrame]);

	m_ball.Draw();

	DrawResult();
}

void ControlFishing::DrawResult()
{
	if( m_fishResult < 0 || m_fishResult >= MAX_FISHTYPE )
		return;

	m_imgResult[m_fishResult].Draw();
	m_text_Result.Draw();
}

void ControlFishing::Action()
{
	switch(m_curState)
	{
	case eFishingState_Idle:
		ActionIdle();
		break;
	case eFishingState_Throw:
		ActionThrow();
		break;
	case eFishingState_Fishing:
		ActionFishing();
		break;
	case eFishingState_WaitResult:
		ActionWaitResult();
		break;
	case eFishingState_ShowResult:
		ActionShowResult();
		break;
	default:
		break;
	}
}

void ControlFishing::ActionIdle()
{
	if( CheckItemCount() == false )
	{
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_FISHING_NOTENOUGH), TYPE_NOR_OK, 1);
		return;
	}

	SetState(eFishingState_Throw);

	m_btnStart.SetDisable2(true);
	m_btnStart.SetVisible(FALSE);
	m_btnStop.SetDisable2(false);
	m_btnStop.SetVisible(TRUE);
}

void ControlFishing::ActionThrow()
{
}

void ControlFishing::ActionFishing()
{
	m_bSuccess = IsInSuccessArea();
	SendFishThrow(m_typeWorm, m_bSuccess);

	m_btnStop.SetDisable2(true);
	SetState(eFishingState_WaitResult);
}

void ControlFishing::ActionWaitResult()
{
}

void ControlFishing::ActionShowResult()
{
}

LRESULT ControlFishing::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, FISHING_REWARD);
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
				SendFishRewardListReq();
				m_btnReward.SetState(BTN_NORMAL);
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(TRUE, FISHING_REWARD);
			}

			if( m_curState == eFishingState_Idle )
			{
				m_btnWorm1.MsgProc(hWnd, msg, wParam, lParam);
				m_btnWorm2.MsgProc(hWnd, msg, wParam, lParam);

				if( m_btnWorm1.GetState() == BTN_ACTION )
				{
					SetWormType(0);
				}
				if( m_btnWorm2.GetState() == BTN_ACTION )
				{
					SetWormType(1);
				}
			}

			return 1;
		}
	}

	return 0;
}

bool ControlFishing::IsInSuccessArea()
{
	if( m_ballPosX > 105 && m_ballPosX < 250 )
		return true;

	return false;
}

void ControlFishing::SetState(int state)
{
	m_curState = (eFishingState)state;
	m_timeAccum = 0;
}

void ControlFishing::SetResult(int result)
{
	m_fishResult = result;

	if( result < 0 || result >= MAX_FISHTYPE )
	{
		m_text_Result.SetString("");
	}
	else
	{
		m_text_Result.SetString((char*)G_STRING(IDS_FISHTYPE_1 + result));
	}
}

void ControlFishing::SetWormType(int typeWorm)
{
	m_typeWorm = typeWorm;
	if( m_typeWorm == 0 )
	{
		m_btnWorm1.SetState(BTN_ON);
		m_btnWorm2.SetState(BTN_NORMAL);
	}
	else
	{
		m_btnWorm1.SetState(BTN_NORMAL);
		m_btnWorm2.SetState(BTN_ON);
	}
}

void ControlFishing::SetBallSpeed(int speed)
{
	m_ballSpeed = (float)speed;
}

bool ControlFishing::CheckItemCount()
{
	const int reelItemVnum = 3758;
	const int wormItemVnum1 = 3756;
	const int wormItemVnum2 = 3757;
	int count0 = g_pRoh->GetCountInPack(reelItemVnum);

	if( count0 <= 0 )
		return false;

	int count1 = g_pRoh->GetCountInPack(wormItemVnum1);
	int count2 = g_pRoh->GetCountInPack(wormItemVnum2);

	if( m_typeWorm == 0 )
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
