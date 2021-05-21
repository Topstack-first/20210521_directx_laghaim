#ifndef _CONTROL_LETTER_RECV_H_
#define _CONTROL_LETTER_RECV_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "TextOutBox.h"
#include "LetterBox.h"

class ControlLetterRecv : public CBaseInterface
{
public:
	ControlLetterRecv();
	~ControlLetterRecv();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();

	void Refresh(int index, int type);

	void SetLock(bool bLock) { m_isLock = bLock; }

private:
	void ActionClose();
	void ActionOK();
	void ActionDel();
	void ActionRe();

private:
	CBasicStatic	m_background;
	CBasicButton	m_btnClose;
	CBasicButton	m_btnOK;
	CBasicButton	m_btnDel;
	CBasicButton	m_btnRe;

	CTextBox		m_tbTitle;
	CTextBox		m_tbName;

	CLetterBox*		m_pLetterBox;

	int				m_index;
	int				m_type;

	DWORD			m_time;
	float			m_timeAccum;

	bool			m_isLock;
};

#endif // _CONTROL_LETTER_RECV_H_
