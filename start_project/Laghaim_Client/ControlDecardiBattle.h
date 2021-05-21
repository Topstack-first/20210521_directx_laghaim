#ifndef _CONTROL_DECARDI_BATTLE_H_
#define _CONTROL_DECARDI_BATTLE_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "TextOutBox.h"

class CSurface;
class CTextOutBox;
class ControlDecardiBattle : public CBaseInterface
{
public:
	ControlDecardiBattle();
	~ControlDecardiBattle();

	void LoadRes();
	float Update();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

public:
	void Start();
	void End(const int result);

private:
	void drawPoint();
	void drawResult();

private:
	bool m_bVisible;
	int m_nWinLoseDraw;
	DWORD m_dwEndTime;

	CBasicStatic m_background;
	CBasicStatic m_imgWin;
	CBasicStatic m_imgLose;
	CBasicStatic m_imgDraw;
	CTextOutBox m_text_RedTeam;
	CTextOutBox m_text_BlueTeam;
	CTextOutBox m_text_RedPoint;
	CTextOutBox m_text_BluePoint;
	CTextOutBox m_text_Time;
};


#endif // _CONTROL_DECARDI_BATTLE_H_
