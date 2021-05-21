#ifndef _PLATINUM_H_
#define _PLATINUM_H_

#include "Config.h"


#include "ddutil.h"
#include "NkCharacter.h"

class CSurface;
class CBasicButton;
class CItemRender;
class CMsgPopUp;
class CAlphaPopUp;

#define NUM_PANNEL 6


class Cplatinum
{
public:
	Cplatinum();
	virtual ~Cplatinum();
	HWND			m_hWnd;
	RECT			m_ClientRc;
	CSurface	*   m_pBack;
	CBasicButton*	m_pBtnCancel;	 // "cancle" 버튼
	CBasicButton*	m_pBtnExchange;	 // "교환" 버튼
	CSurface*       m_pTextInfo;
	CAlphaPopUp*    m_pAlphaPopUp;
	CBasicButton*	m_pBtnExchangePaper;	 // "문서감정" 버튼
	CBasicButton*	m_pBtnExchangeRing;	 // "반지회수" 버튼
	int				m_iType; // 0=고대문서 , 1=어버이날 이벤트


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

	void Exchange(int result, int type, int vnum_money, int index, int gem_num);
	void ExchangeTree(int result, int type, int vnum, int index);

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
	LRESULT parentsDayMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void InterCancel(BOOL bSend);
	void SendMsg();
	int  CheckMyslot_Exg();
	int  Check2Myslot_Exg();
	int  CheckMyslot_ParentsDay();
	BOOL IsPlatinumItem(int vnum);
	void ChangeTo(int vnum, int svr_idx, int plus);

	void QuestExchange(int money);//0409 역사학자가 교환해주는것
	void GetMsgParentsDay(BOOL bsuccess, int vnum, int sver_idx); // 어버이날 이벤트

	BOOL m_bQuestWindow;
	DWORD m_dwMouseClickTickTime;
};

#endif // _PLATINUM_H_