#ifndef _CONTROL_LETTER_SEND_H_
#define _CONTROL_LETTER_SEND_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "LetterBox.h"

class ControlLetterSend : public CBaseInterface
{
public:
	ControlLetterSend();
	~ControlLetterSend();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();

	bool IsFocus();

	void ClearText();
	void SetName(const std::string& name);

	bool IsInside_Prompt(int x, int y);

	void SetLock(bool bLock) { m_isLock = bLock; }

private:
	void ActionClose();
	void ActionSend();

private:
	CBasicStatic	m_background;
	CBasicButton	m_btnClose;
	CBasicButton	m_btnSend;

	CTextBox		m_tbTitle;
	CTextBox		m_tbName;

	CLetterBox*		m_pLetterBox;

	DWORD			m_time;
	float			m_timeAccum;

	bool			m_isLock;
};

#endif // _CONTROL_LETTER_SEND_H_
