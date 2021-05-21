#ifndef _CONTROL_LETTER_WINDOW_H_
#define _CONTROL_LETTER_WINDOW_H_

#include "BaseInterface.h"
#include "BasicStatic.h"
#include "basicbutton.h"
#include "multilistbox.h"
#include "TextOutBox.h"

#define MAX_LETTER_LIST	10

class ControlLetterWindow : public CBaseInterface
{
public:
	ControlLetterWindow();
	~ControlLetterWindow();

	HRESULT RestoreSurfaces();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	float Update();

	void Refresh();
	void ReqList(bool byDel);

	void SetLock(bool bLock) { m_isLock = bLock; }
	void Open();

	void SetCurType(int type);
	int GetCurType() { return m_curType; }

private:
	void ActionTab(int type);
	void ActionWrite();
	void ActionPage(int type);
	void ActionRead(int index);

private:
	CBasicStatic	m_background;
	CBasicStatic	m_background2;
	CBasicButton	m_btnClose;
	CTextOutBox		m_textPage;

	CBasicButton	m_btnTabSend;
	CBasicButton	m_btnTabRecv;
	CBasicButton	m_btnTabGM;

	CBasicButton	m_btnWrite;

	CBasicButton	m_btnPageFirst;
	CBasicButton	m_btnPagePrev;
	CBasicButton	m_btnPageNext;
	CBasicButton	m_btnPageLast;

	CMutiListBox*	m_lstLetter;
	CBasicStatic	m_signNew[MAX_LETTER_LIST];

	int				m_curType;	
	DWORD			m_time;
	float			m_timeAccum;

	bool			m_isLock;
};

#endif // _CONTROL_LETTER_WINDOW_H_
