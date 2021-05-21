#ifndef _CONTROL_BASEBALL_H_
#define _CONTROL_BASEBALL_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "BaseballManager.h"

#define MAX_PICHER_MOTION_FRAME	3

enum eBaseballState
{
	eBaseballState_Idle = 0,
	eBaseballState_Pitching,
	eBaseballState_Hitting,
	eBaseballState_WaitResult,
	eBaseballState_ShowResult,

	eBaseballState_MAX,
};

class ControlBaseball : public CBaseInterface
{
public:
	ControlBaseball();
	~ControlBaseball();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();
	void SetState(int state);

	void SetResult(int result);

	void Init();
	void SetBallType(int typeBall);
	void SetBallSpeed(int speed);

public:
	void UpdateText(float elapsedTime);
	void UpdateIdle(float elapsedTime);
	void UpdatePitching(float elapsedTime);
	void UpdateHitting(float elapsedTime);
	void UpdateWaitResult(float elapsedTime);
	void UpdateShowResult(float elapsedTime);
		
	void DrawIdle();
	void DrawPitching();
	void DrawHitting();
	void DrawWaitResult();
	void DrawShowResult();

	void Action();
	void ActionIdle();
	void ActionPitching();
	void ActionHitting();
	void ActionWaitResult();
	void ActionShowResult();

	bool IsInSuccessArea();
	void DrawResult();
	bool CheckItemCount();

private:
	CBasicStatic	m_background;
	CBasicStatic	m_ball;
	CBasicStatic	m_ballCursor;
	CBasicStatic	m_imgResult[MAX_BASEBALL_GRADE];
	CBasicButton	m_btnStart;	
	CBasicButton	m_btnStop;
	CBasicButton	m_btnClose;
	CBasicButton	m_btnBall1;
	CBasicButton	m_btnBall2;
	CBasicButton	m_btnReward;	
	CTextOutBox		m_text_BallCount[MAX_BASEBALL_GRADE];
	CTextOutBox		m_text_TypeCount[2];	
	

	RECT			m_rcPitcher[MAX_PICHER_MOTION_FRAME];	
	RECT			m_rcHitter[MAX_PICHER_MOTION_FRAME];
	CSurface*		m_surPitcher;

	eBaseballState	m_curState;

	DWORD			m_time;
	float			m_timeAccum;
	float			m_ballSpeed;
	bool			m_ballDir;
	float			m_ballPosX;
	bool			m_bSuccess;
	int				m_typeBall;
	int				m_ballResult;
	int				m_frmPitcher;
	int				m_frmHitter;
};

#endif // _CONTROL_BASEBALL_H_
