#ifndef _SUMMER_EVENT_H_
#define _SUMMER_EVENT_H_

#include "Config.h"


#include "ddutil.h"
#include "NkCharacter.h"

class CSurface;
class CBasicButton;
class CItemRender;
class CMsgPopUp;
class CAlphaPopUp;

#define NUM_PANNEL 6


class CSummerEvent
{
public:
	CSummerEvent();
	virtual ~CSummerEvent();

	HWND			m_hWnd;
	RECT			m_ClientRc;
	CSurface	*   m_pBack;
	CSurface	*   m_pBack_Title;
	CBasicButton*	m_pBtnCancel;	 // "cancle" 버튼
	CBasicButton*	m_pBtnExchange;	 // "교환" 버튼
	CSurface*       m_pTextInfo;
// 	CBasicButton*	m_pBtnExchangePaper;   // "문서감정" 버튼
// 	CBasicButton*	m_pBtnExchangeRing;	   // "반지회수" 버튼


	BOOL    m_bMainActive;
	BOOL    m_bClick, m_bMove, m_bMoveEnd;
	BOOL    m_bExchange;


	void LoadRes(int Type, CItemRender *pItemRender);
	void DeleteRes();
	CItem* Draw();
	LRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	void LoadExchange();
	void DeleteExchange();
	void DrawExchange();
	void DrawMySlot();

	void Exchange(int bsucceed, int type, int vnum_money, int index);

	void RemoveAllMyInven();

	CNkCharacter *m_pNkCha;
	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;
	CItemRender		*m_pItemRender;
	CItem			*m_pOverItem;

	int m_my_slotstart_x, m_my_slotstart_y;
	char m_MsgBuf[256];

	void SetNkCha(CNkCharacter *pNkCha)
	{
		m_pNkCha = pNkCha;
	}
	LRESULT ExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);

	void InterCancel(BOOL bSend);
	void SendMsg();
	int  CheckMyslot_Exg();

	BOOL IsSummerEventItem(int vnum);
	void LaimTo(int Laim);
	void ChangeTo(int vnum, int svr_idx);

	BOOL m_bQuestWindow;
	DWORD m_dwMouseClickTickTime;

};
#endif // _SUMMER_EVENT_H_


