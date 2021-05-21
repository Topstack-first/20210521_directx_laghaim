#ifndef _CONTROL_YUTBOARD_H_
#define _CONTROL_YUTBOARD_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "YutManager.h"
#include <queue>

#define MAX_UNIT_TYPE	6
#define MAX_YUT_FRAME	13

class ControlYutboard : public CBaseInterface
{
public:
	ControlYutboard();
	~ControlYutboard();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void SetAniYutStart(int result);
	void SetAniUnitMoveStart();
	void SetCurPos(int pos);

	void SetEnableStartBtn(bool bEnable);

	void SetCompleteCount(int completeCount);

private:
	void updateAniYut();
	void updateAniUnitMove();
	void updateText();

private:
	CBasicStatic	m_background;
	CBasicButton	m_btn_start;
	CBasicButton	m_btn_close;
	CTextOutBox		m_text_CompleteCount;
	CTextOutBox		m_text_ItemCount;

	CBasicStatic	m_img_unit[MAX_UNIT_TYPE];

	RECT			m_rcYut[MAX_YUT_FRAME];
	RECT			m_rcResult[eYutResult_MAX];

	CSurface*		m_surYut;
	CSurface*		m_surResult;

	int	m_result;
	bool m_aniYut;
	DWORD m_timeAniYutStart;

	bool m_aniUnitMove;
	int m_curPos;
	DWORD m_timeAniUnitMoveStart;

	int m_CompleteCount;

	std::queue<int> m_queRoute;
};

#endif // _CONTROL_YUTBOARD_H_
