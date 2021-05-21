#include "stdafx.h"
#include "Nk2DFrame.h"
#include "CommonConfig.h"
#include "DiceGameManager.h"
#include "PacketSend.h"
#include "ControlDiceGameReward.h"

ControlDiceGameReward::ControlDiceGameReward()
{
}

ControlDiceGameReward::~ControlDiceGameReward()
{
	DeleteRes();
}

HRESULT ControlDiceGameReward::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlDiceGameReward::LoadRes()
{
	if( DiceGameManager::GetInstance()->GetDiceGameOn() == false )
	{
		return;
	}

	m_background.Init("dicegame/dice_board_list");
	m_btnClose.Init("common/btn_close_01");
	m_btnOK.Init("dicegame/dice_board_list_btn_01");

	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	DWORD textColor = RGB(255, 255, 255);
	for(int i = 0; i < MAX_DICEGAME_REWARD_COUNT; i++)
	{
		m_textReward[i].Init(12, textColor, bgx + 425, bgy + 94 + (i * 21), 100, 21, TRUE);
		m_textReward[i].SetString("Reward Test", textColor);
		m_textFishCount[i].Init(12, textColor, bgx + 119, bgy + 94 + (i * 21), 144, 21, TRUE);
		m_textFishCount[i].SetString("Count Test", textColor);
		m_btnReward[i].Init("dicegame/dice_board_list_btn_02");
		m_btnReward[i].SetPosition(bgx + 664, bgy + 89 + (i * 21));
		m_textAlready[i].Init(12, textColor, bgx + 699, bgy + 94 + (i * 21), 65, 21, TRUE);
		m_textAlready[i].SetString_withOutLine("지급 완료", RGB(178, 180, 181), RGB(0,0,0));
	}

	m_btnClose.SetPosition(bgx + 753, bgy + 5);
	m_btnOK.SetPosition(bgx + 342, bgy + 418);

	Refresh();
}

void ControlDiceGameReward::DeleteRes()
{
	if( DiceGameManager::GetInstance()->GetDiceGameOn() == false )
	{
		return;
	}

	m_background.DeleteRes();

	for(int i = 0; i < MAX_DICEGAME_REWARD_COUNT; i++)
	{
		m_textReward[i].DeleteRes();
		m_textFishCount[i].DeleteRes();
		m_btnReward[i].DeleteRes();
		m_textAlready[i].DeleteRes();
	}

	m_btnClose.DeleteRes();
	m_btnOK.DeleteRes();
}

void ControlDiceGameReward::Draw()
{
	if( g_pDisplay == NULL )
		return;

	m_background.Draw();

	for(int i = 0; i < MAX_DICEGAME_REWARD_COUNT; i++)
	{
		m_textReward[i].Draw();
		m_textFishCount[i].Draw();
		m_btnReward[i].Draw();
		m_textAlready[i].Draw();
	}

	m_btnClose.Draw();
	m_btnOK.Draw();
}

LRESULT ControlDiceGameReward::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
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

			m_btnClose.MsgProc(hWnd, msg, wParam, lParam);
			m_btnOK.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnClose.GetState() == BTN_ACTION ||
				m_btnOK.GetState() == BTN_ACTION )
			{
				m_btnClose.SetState(BTN_NORMAL);
				m_btnOK.SetState(BTN_NORMAL);
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(TRUE, DICEGAME);
			}			

			for(int i = 0; i < MAX_DICEGAME_REWARD_COUNT; i++)
			{	
				m_btnReward[i].MsgProc(hWnd, msg, wParam, lParam);

				if( m_btnReward[i].GetState() == BTN_ACTION )
				{
					m_btnReward[i].SetState(BTN_NORMAL);
					ReqReward(i);
				}
			}

			return 1;
		}
	}

	return 0;
}

void ControlDiceGameReward::Refresh()
{
	DiceGameRewardManager* rewardMgr = DiceGameRewardManager::GetInstance();

	std::vector<std::string> vecRewardList = rewardMgr->GetRewardList();
	int cntRewardList = vecRewardList.size();
	BOOL visible = FALSE;
	std::string rewardName;
	UINT64 succCount = 0;
	CommonConfig* config = CommonConfig::Instance();
	for(int i = 0; i < MAX_DICEGAME_REWARD_COUNT; i++)
	{
		visible = FALSE;
		if( i < cntRewardList )
		{
			visible = TRUE;
			rewardName = vecRewardList[i];
			succCount = config->GetDiceGameRewardSuccCount(i);
			m_textReward[i].SetString(const_cast<char*>(rewardName.c_str()));
			m_textFishCount[i].SetString((UINT64)succCount);
		}

		m_textReward[i].SetVisible(visible);
		m_textFishCount[i].SetVisible(visible);
		m_btnReward[i].SetVisible(visible);
		m_btnReward[i].SetDisable2(true);
		m_textAlready[i].SetVisible(FALSE);
	}

	std::vector<int> vecCanGetList = rewardMgr->GetCanGetList();
	int cntCanGetList = vecCanGetList.size();
	int idxCanGet = 0;
	for(int i = 0; i < cntCanGetList; i++)
	{
		idxCanGet = vecCanGetList[i];
		if( idxCanGet < 0 || idxCanGet >= MAX_DICEGAME_REWARD_COUNT )
			continue;

		m_btnReward[idxCanGet].SetVisible(TRUE);
		m_btnReward[idxCanGet].SetDisable2(false);
	}

	std::vector<int> vecAlreadyList = rewardMgr->GetAlreadyList();
	int cntAlready = vecAlreadyList.size();
	int idxAlready = 0;
	for(int i = 0; i < cntAlready; i++)
	{
		idxAlready = vecAlreadyList[i];
		if( idxAlready < 0 || idxAlready >= MAX_DICEGAME_REWARD_COUNT )
			continue;

		m_btnReward[idxAlready].SetVisible(FALSE);
		m_btnReward[idxAlready].SetDisable2(true);
		m_textAlready[idxAlready].SetVisible(TRUE);
	}
}

void ControlDiceGameReward::ReqReward(int index)
{
	m_btnReward[index].SetDisable(true);

	int succCount = CommonConfig::Instance()->GetDiceGameRewardSuccCount(index);
	if( succCount == -1 )
		return;

	SendSnakeDiceGetPresent(succCount);
}
