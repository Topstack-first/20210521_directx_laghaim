#ifndef _HELP_H_
#define _HELP_H_

#include "Config.h"

#define HELP_TIMER		1000
#define HELP_TIMER_NUM	22

#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

//#define HELP_X		(170 * m_ScreenWidth / BASE_X_SIZE)
//#define HELP_Y		(100 * m_ScreenHeight / BASE_Y_SIZE)
#define HELP_X		40
#define HELP_Y		5
#define HELP_WIDTH	(300 * m_ScreenWidth / BASE_X_SIZE)
#define HELP_HEIGHT	(200 * m_ScreenHeight / BASE_Y_SIZE)

class CSurface;

void LoadDLLStringTable();

class CHelp
{
public:
	CHelp();
	virtual ~CHelp();

	void LoadRes();
	void DeleteRes();
	void Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	CSurface *m_pSurfaceBack;
	int m_ScreenWidth;
	int m_ScreenHeight;
};

#endif // _HELP_H_
