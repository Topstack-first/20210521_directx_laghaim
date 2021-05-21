#ifndef _CONTROL_FISHING_H_
#define _CONTROL_FISHING_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "FishingManager.h"

#define MAX_FISHING_THROW_FRAME	26

enum eFishingState
{
	eFishingState_Idle = 0,
	eFishingState_Throw,
	eFishingState_Fishing,
	eFishingState_WaitResult,
	eFishingState_ShowResult,

	eFishingState_MAX,
};

class ControlFishing : public CBaseInterface
{
public:
	ControlFishing();
	~ControlFishing();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();
	void SetState(int state);

	void SetResult(int result);

	void Init();
	void SetWormType(int typeWorm);
	void SetBallSpeed(int speed);

private:
	void UpdateText(float elapsedTime);
	void UpdateIdle(float elapsedTime);
	void UpdateThrow(float elapsedTime);
	void UpdateFishing(float elapsedTime);
	void UpdateWaitResult(float elapsedTime);
	void UpdateShowResult(float elapsedTime);

	void DrawIdle();
	void DrawThrow();
	void DrawFishing();
	void DrawWaitResult();
	void DrawShowResult();

	void Action();
	void ActionIdle();
	void ActionThrow();
	void ActionFishing();
	void ActionWaitResult();
	void ActionShowResult();


	bool IsInSuccessArea();
	void DrawResult();
	bool CheckItemCount();

private:
	CBasicStatic	m_background;
	CBasicStatic	m_ball;
	CBasicStatic	m_imgGo;
	CBasicStatic	m_imgSuccess;
	CBasicStatic	m_imgFail;
	CBasicStatic	m_imgResult[MAX_FISHTYPE];
	CBasicButton	m_btnStart;
	CBasicButton	m_btnStop;
	CBasicButton	m_btnClose;
	CBasicButton	m_btnWorm1;
	CBasicButton	m_btnWorm2;
	CBasicButton	m_btnReward;
	CTextOutBox		m_text_FishType[MAX_FISHTYPE];
	CTextOutBox		m_text_FishCount[MAX_FISHTYPE];
	CTextOutBox		m_text_WormCount[2];
	CTextOutBox		m_text_Result;


	RECT			m_rcFishingRod[MAX_FISHING_THROW_FRAME];
	CSurface*		m_surFishingRod;

	eFishingState	m_curState;

	DWORD			m_time;
	float			m_timeAccum;
	float			m_ballSpeed;
	bool			m_ballDir;
	float			m_ballPosX;
	float			m_throwDivide;
	bool			m_bSuccess;
	int				m_typeWorm;
	int				m_fishResult;
};

#endif // _CONTROL_FISHING_H_
