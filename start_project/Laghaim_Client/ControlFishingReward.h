#ifndef _CONTROL_FISHING_REWARD_H_
#define _CONTROL_FISHING_REWARD_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "FishingRewardManager.h"

class ControlFishingReward : public CBaseInterface
{
public:
	ControlFishingReward();
	~ControlFishingReward();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	

	void Refresh();

private:
	void ChangeTab(int grade);
	void ReqReward(int index);

private:
	CBasicStatic	m_background;
	CBasicButton	m_btnClose;
	CBasicButton	m_btnOK;
	CBasicButton	m_btnGradeTab[MAX_FISHING_GRADE];
	CBasicButton	m_btnReward[MAX_FISHING_REWARD_COUNT];
	CTextOutBox		m_textFishCount[MAX_FISHING_REWARD_COUNT];
	CTextOutBox		m_textReward[MAX_FISHING_REWARD_COUNT];
	CTextOutBox		m_textAlready[MAX_FISHING_REWARD_COUNT];

	int				m_curGrade;
};

#endif // _CONTROL_FISHING_REWARD_H_
