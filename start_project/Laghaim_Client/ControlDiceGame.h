#ifndef _CONTROL_DICEGAME_H_
#define _CONTROL_DICEGAME_H_

#include <deque>
#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "DiceGameManager.h"

#define MAX_DICE_FRAME	10

enum eDiceGameState
{
	eDiceGameState_Idle = 0,	
	eDiceGameState_WaitDice,
	eDiceGameState_ShowDice,
	eDiceGameState_MovePlayer,
	eDiceGameState_Complete,
	
	eDiceGameState_MAX,
};

class ControlDiceGame : public CBaseInterface
{
public:
	ControlDiceGame();
	~ControlDiceGame();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();
	void SetState(int state);

	void SetPath(int curDice, std::vector<int>& path);

	void Init();

public:
	void UpdateText(float elapsedTime);
	void UpdateIdle(float elapsedTime);
	void UpdateWaitDice(float elapsedTime);
	void UpdateShowDice(float elapsedTime);
	void UpdateMovePlayer(float elapsedTime);
	void UpdateComplete(float elapsedTime);
		
	void DrawIdle();
	void DrawWaitDice();
	void DrawShowDice();
	void DrawMovePlayer();
	void DrawComplete();

	void Action();
	void ActionIdle();
	void ActionWaitDice();
	void ActionShowDice();
	void ActionMovePlayer();
	void ActionComplete();	
	
	bool CheckItemCount();

	void SetPlayerPos(int pos);	

private:
	CBasicStatic	m_background;
	CBasicButton	m_btnStart;	
	CBasicButton	m_btnClose;
	CBasicButton	m_btnReward;
	CTextOutBox		m_text_DiceCount;
	CTextOutBox		m_text_CompleteCount;

	CBasicStatic	m_imgPlayer;
	CBasicStatic	m_imgComplete;

	RECT			m_rcDiceSeq[MAX_DICE_FRAME];
	CSurface*		m_surDice[6];

	eDiceGameState	m_curState;

	DWORD			m_time;
	float			m_timeAccum;
	int				m_frmDice;
	int				m_curPos;
	int				m_curDice;
	int				m_cntComplete;

	std::vector<int> m_path;
};

#endif // _CONTROL_DICEGAME_H_
