#ifndef _CONTROL_LOTTO_H_
#define _CONTROL_LOTTO_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "LottoManager.h"

enum eLottoState
{
	eLottoState_Idle = 0,
	eLottoState_Auto,
	eLottoState_Regist,
	eLottoState_Refresh,

	eLottoState_MAX,
};

class ControlLotto : public CBaseInterface
{
public:
	ControlLotto();
	~ControlLotto();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();
	void SetState(int state);

	void ButtonEnable(bool enable);
	bool IsFocus();

	void RefreshListReq(const int page = -1);
	void RefreshList();
	void SetCurPage(const int curPage);
	void SetMaxPage(const int maxPage);

private:
	void UpdateIdle(float elapsedTime);
	void UpdateAuto(float elapsedTime);
	void UpdateRegist(float elapsedTime);

	void DrawIdle();
	void DrawAuto();
	void DrawRegist();

	void ActionRegist();
	void ActionAuto();
	void ActionListLeft();
	void ActionListRight();

	void ClearNums();
	int	MakeRandom(const int step);
	void AlignInputs();

	bool CheckAndSend();

private:
	CBasicStatic	m_background;
	CBasicButton	m_btnRegist;
	CBasicButton	m_btnAuto;
	CBasicButton	m_btnPageLeft;
	CBasicButton	m_btnPageRight;	
	CTextOutBox		m_textRegistPage;
	CTextOutBox		m_textItemNeedCount;
	CTextOutBox		m_textItemHaveCount;
	CTextOutBox		m_textRegistList[MAX_LOTTO_REGIST_LIST];
	CTextBox		m_tbInput[MAX_LOTTO_INPUT];

	eLottoState		m_curState;

	DWORD			m_time;
	float			m_timeAccum;
	int				m_curPage;
	int				m_maxPage;
	int				m_stepRandom;
};

#endif // _CONTROL_LOTTO_H_
