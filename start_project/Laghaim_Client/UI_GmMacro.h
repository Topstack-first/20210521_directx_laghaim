#ifndef _UI_GMMACRO_H_
#define _UI_GMMACRO_H_

#include "Config.h"
#include "ddutil.h"
#include "basicbutton.h"
#include "TextBox.h"

#define MAX_GMMACRO_COUNT 10

class UI_GmMacro
{
public:
	UI_GmMacro();
	~UI_GmMacro();

	void	LoadRes();
	void	DeleteRes();
	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void	SetVisible(bool visible);
	bool	IsVisible();

private:
	void	sendChatMacro(int index);
	void	loadMacro();
	void	saveMacro();

private:
	CSurface*		m_pBack;

	CBasicButton	m_SendChatBtn[MAX_GMMACRO_COUNT];
	CTextBox		m_MacroText[MAX_GMMACRO_COUNT];
	CBasicButton    m_CloseBtn;

	bool			m_IsVisible;
};

#endif