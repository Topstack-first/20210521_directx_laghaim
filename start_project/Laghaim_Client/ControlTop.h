#ifndef _CONTROL_TOP_H_
#define _CONTROL_TOP_H_

//#include "LHUI_Base.h"

#include "BasicButton.h"

class CSurface;
class CAlphaPopUp;

class CControlTop //  : LHUI_Base
{
public:
	CAlphaPopUp	* m_pEtcInfo;
	CNkCharacter *m_pNkCha;

	RECT m_rcThis;

	CBasicButton m_IconInvenBtn;
	CBasicButton m_IconScoreBtn;
	CBasicButton m_IconSkillBtn;
	CBasicButton m_IconPetBtn;
	CBasicButton m_IconMapBtn;
	CBasicButton m_IconHelpBtn;
	CBasicButton m_IconOptionBtn;
	CBasicButton m_IconExitBtn;
	CBasicButton m_IconPartyBtn;
	CBasicButton m_IconCashShopBtn;
	CBasicButton m_IconHelperBtn;
	CBasicButton m_IconGuildBtn;
	CBasicButton m_IconAutoBtn;
	CBasicButton m_iConAutoOnBtn;








public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	BOOL IsInside(int x, int y);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



public:
	int GetWidth()
	{
		return (m_rcThis.right-m_rcThis.left);
	}
	int GetHeight()
	{
		return (m_rcThis.bottom-m_rcThis.top);
	}

public:
	CControlTop();
	virtual ~CControlTop();

};

#endif // _CONTROL_TOP_H_
