#ifndef _CONTROL_DICEGAME_REWARD_H_
#define _CONTROL_DICEGAME_REWARD_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "DiceGameRewardManager.h"

class ControlDiceGameReward : public CBaseInterface
{
public:
	ControlDiceGameReward();
	~ControlDiceGameReward();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	

	void Refresh();

private:
	void ReqReward(int index);

private:
	CBasicStatic	m_background;
	CBasicButton	m_btnClose;
	CBasicButton	m_btnOK;
	CBasicButton	m_btnReward[MAX_DICEGAME_REWARD_COUNT];
	CTextOutBox		m_textFishCount[MAX_DICEGAME_REWARD_COUNT];
	CTextOutBox		m_textReward[MAX_DICEGAME_REWARD_COUNT];
	CTextOutBox		m_textAlready[MAX_DICEGAME_REWARD_COUNT];
};

#endif // _CONTROL_DICEGAME_REWARD_H_
