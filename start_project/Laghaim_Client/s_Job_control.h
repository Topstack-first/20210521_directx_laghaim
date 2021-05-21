#ifndef _JOB_CHANGE_H_
#define _JOB_CHANGE_H_

#include "Config.h"

#include "Nk2dframe.h"
#include "BasicButton.h"
#include "TextBox.h"
#include "Scroll.h"

class CSurface;

class CJobChangeControl
{
public:
	enum
	{
		MASTER = 0,
		ASSIST,
		VULCAN,
		HUMAN,
		AIDA,
		KALIP,
		HYBRID,
		PEROM,
		TOTAL
	};

public:
	HWND	m_hWnd;
	RECT    m_ClientRc;

	CBasicButton	m_BtSave;
	CBasicButton	m_BtCancel;
	CBasicButton	m_BtAppoint[TOTAL];

	CTextBox		m_Text[TOTAL];

	CScroll			m_Scroll;

	CSurface*		m_pTitle;
	CSurface*		m_pName[TOTAL];

	char			m_MsgBuf[1024];

	int				m_nBaseX;
	int				m_nBaseY;

public:
	static CJobChangeControl* Create(int nBaseX = 0, int nBaseY = 0);

	~CJobChangeControl();

	BOOL IsFocus();

	void Init();
	void LoadRes();
	void Draw();
	void DeleteRes();

	void SetPosition();
	void SendMsg();

	void Restore();
	void Reset();

	void Insert_list();
	void Change_list(int g_level);

	BOOL DrawToBlt(CSurface* pSurface);
	BOOL IsInside(int, int);


	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	CJobChangeControl(int nBaseX = 0, int nBaseY = 0);
};
#endif