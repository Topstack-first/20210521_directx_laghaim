#if !defined(_MEMBER_NOTICE_)
#define _MEMBER_NOTICE_

#include "Config.h"

#include "basewindow.h"
#include "basicbutton.h"

class CLetterBox;

class CMemberNotice	: public CUIBaseWindow
{
public:
	CMemberNotice();
	virtual ~CMemberNotice();

	/*******************************************************************/
	int	 Draw();
	void DeleteRes();
	void RestoreSurfaces();

	BOOL IsInside(int x, int y);
	BOOL IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}

	BOOL	InitMode();
	void LoadRes();

	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*******************************************************************/

	void SetNotice(char* szNotice);
	void RevMsg(char* psz);
	void cleartext();
private:
	CLetterBox *m_pNoticeString;

};
#endif