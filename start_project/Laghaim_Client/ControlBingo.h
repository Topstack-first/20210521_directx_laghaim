#ifndef _CONTROL_BINGO_H_
#define _CONTROL_BINGO_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "BingoManager.h"

#define BINGO_SEQ_MAX	41

enum eBingoState
{
	eBingoState_Idle = 0,
	eBingoState_Regist,
	eBingoState_Result,
	eBingoState_Bingo,
	eBingoState_Erase,
	eBingoState_ReqInfo,
	eBingoState_ReqReward,

	eBingoState_MAX,
};

class ControlBingo : public CBaseInterface
{
public:
	ControlBingo();
	~ControlBingo();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();
	void SetState(int state);
	eBingoState GetState();

	void SetLastNum(int num);

	void RefreshRewardText();

	void ActionRegist(int type);

private:
	void UpdateText();

	void UpdateIdle(float elapsedTime);
	void UpdateRegist(float elapsedTime);
	void UpdateResult(float elapsedTime);
	void UpdateBingo(float elapsedTime);
	void UpdateErase(float elapsedTime);

	void DrawCommon();
	void DrawLastNum();
	void DrawPreview();
	void DrawCircle();
	void DrawLine();
	void DrawText();

	void DrawIdle();
	void DrawRegist();
	void DrawResult();
	void DrawBingo();
	void DrawErase();

	void ActionReward(int index);	

private:
	CBasicStatic	m_background;
	CBasicButton	m_btnStart;
	CBasicButton	m_btnStartTen;
	CBasicButton	m_btnClose;

	CBasicStatic	m_imgBingo;
	CBasicStatic	m_imgIcon;

	CSurface*		m_surBigNum;
	CSurface*		m_surSmallNum;
	RECT			m_rcBigNum[BINGO_NUMS_MAX];
	RECT			m_rcSmallNum[BINGO_NUMS_MAX];

	CSurface*		m_surSeq;
	RECT			m_rcSeq[BINGO_SEQ_MAX];

	CBasicButton	m_btnReward[BINGO_PRESENT_MAX];	
	CTextOutBox		m_textReward[BINGO_PRESENT_MAX];
	CTextOutBox		m_textReward2[BINGO_PRESENT_MAX];

	CSurface*		m_surLine;
	RECT			m_rcLineWidth;
	RECT			m_rcLineHeight;
	RECT			m_rcLineSlash;
	RECT			m_rcLineBackSlash;
	RECT			m_rcCircle;
	RECT			m_rcEraser;

	CTextOutBox		m_textCompleteCount;
	CTextOutBox		m_textItemCount;

	int				m_numLast;
	bool			m_isReqRefresh;

	eBingoState		m_curState;

	DWORD			m_time;
	float			m_timeAccum;

	int				m_regType;
};

#endif // _CONTROL_BINGO_H_
