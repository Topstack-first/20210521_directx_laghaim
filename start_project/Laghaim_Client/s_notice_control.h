#ifndef _NOTICE_CHANGE_H_
#define _NOTICE_CHANGE_H_

#include "Config.h"

#include "Nk2dframe.h"
#include "BasicButton.h"
#include "letterBox.h"

class CSurface;
class CLetterBox;

class CNoticeControl			// 길드 공지 관리
{
public:
	enum
	{
		BT_SAVE = 0,
		BT_CHANGE,
		BT_CANCEL,
		BT_TOTAL
	};
public:
	HWND	m_hWnd;
	RECT    m_ClientRc;

	CSurface*  m_pTitle;
	CSurface*  m_pNoticeBack;

	CBasicButton m_BtSave;

	int		m_BaseX;
	int		m_BaseY;

	char	m_MsgBuf[1024];

	CLetterBox *m_pNoticeString;
public:

	static CNoticeControl* Create(int nBaseX = 0, int nBaseY = 0);
	~CNoticeControl();

	BOOL IsFocus()
	{
		return (BOOL)m_pNoticeString->GetEnable();
	}
	void Init();
	void LoadRes();
	void Draw();
	void DeleteRes();
	BOOL IsInside(int, int);

	void SetPosition();
	void Restore();
	void SendMsg();
	void RevMsg(char* psz);

	void SetNotice(char* szNotice);
	void cleartext();

	BOOL IsInside_Prompt(int x, int y);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	CNoticeControl(int nBaseX = 0, int nBaseY = 0);
};
#endif