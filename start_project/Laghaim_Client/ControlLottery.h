#ifndef _CONTROL_LOTTERY_H_
#define _CONTROL_LOTTERY_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"

#define MAX_LOTTERY_SCRATCH_FRAME	4
#define MAX_LOTTERY_GRADE	11

enum eLotteryState
{
	eLotteryState_Idle = 0,
	eLotteryState_Scratch,
	eLotteryState_WaitResult,
	eLotteryState_ShowResult,

	eLotteryState_MAX,
};

class ControlLottery : public CBaseInterface
{
public:
	ControlLottery();
	~ControlLottery();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();
	void SetState(int state);

	void SetResult(int result);

	void Init();

public:
	void UpdateText(float elapsedTime);
	void UpdateIdle(float elapsedTime);
	void UpdateScratch(float elapsedTime);	
	void UpdateWaitResult(float elapsedTime);
	void UpdateShowResult(float elapsedTime);

	void DrawIdle();
	void DrawScratch();	
	void DrawWaitResult();
	void DrawShowResult();

	void Action();
	void ActionIdle();	
	void ActionScratch();
	void ActionWaitResult();
	void ActionShowResult();

	void DrawResult();
	bool CheckItemCount();

private:
	CBasicStatic	m_background;
	CBasicStatic	m_imgResult[MAX_LOTTERY_GRADE];
	CBasicStatic	m_imgCoin;
	CBasicButton	m_btnStart;
	CBasicButton	m_btnClose;
	CTextOutBox		m_text_ItemCount;

	RECT			m_rcScratch[MAX_LOTTERY_SCRATCH_FRAME];
	CSurface*		m_surScratch;

	eLotteryState	m_curState;

	DWORD			m_time;
	float			m_timeAccum;
	float			m_coinSpeed;
	bool			m_coinDir;
	float			m_coinPosX;
	float			m_coinPosY;
	int				m_result;
	int				m_frmScratch;
};

#endif // _CONTROL_LOTTERY_H_
