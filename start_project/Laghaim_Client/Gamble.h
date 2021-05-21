#ifndef _GAMBLE_H_
#define _GAMBLE_H_

#include "Config.h"


#include "ddutil.h"
#include "NkCharacter.h"

class CSurface;
class CBasicButton;
class CItemRender;
class CMsgPopUp;
class CAlphaPopUp;

#define NUM_PANNEL 6

typedef struct JewelExg_Info
{
	int vnum;
	int count;
	int money;
	int platinum;
} JewelExg_Info;

enum RESULTIMG { RSIMG_BLANK, RSIMG_REEVENT, RSIMG_UP1, RSIMG_DOWN1, RSIMG_DOWN2, RSIMG_2, RSIMG_3, RSIMG_4, RSIMG_5, RSIMG_END };

class CGamble
{

public:
	HWND			m_hWnd;
	RECT			m_ClientRc;
	CSurface	*   m_pBack;
	CSurface	*   m_pBack_Title;
	CBasicButton*	m_pBtnCancel;	 // "cancle" 버튼
	CBasicButton*	m_pBtnExchange;	 // "교환" 버튼
	CBasicButton*	m_pBtnInfo;		 // "성명보기" 버튼
	CSurface*       m_pTextInfo;
	CSurface*		m_pRect_Click[NUM_PANNEL];
	CSurface*       m_pRect_Move[NUM_PANNEL];
	CSurface*       m_pBinggo;
	CSurface*       m_pFail;
	CAlphaPopUp*    m_pAlphaPopUp;
	CSurface*       m_pBack_Yut;



	BOOL    m_bMainActive;
	BOOL	m_bType;				 // 교환용인가? 도박용인가?
	BOOL    m_bClick, m_bMove, m_bMoveEnd;
	BOOL	m_bReceiveMsg;
	BOOL    m_bCooSuk;

	int     m_iResult;
	int     m_iGamble_vnum, m_iGamble_idx, m_iBinggo_num;
	int	    m_iClick_Slot, m_iLoop_Slot;
	int		m_iNewBinggo_num;		// 행운의 번호를 미리 알 수 있는 버그 방지용

	RECT   m_rtArray[NUM_PANNEL];
	int    m_iOrder[NUM_PANNEL];

	float  m_fStartTime, m_fEndTime;
	float  m_iOverTime;
	BOOL   m_bExchange;

	int*	m_BackupItemIndex;
	int		m_BackupItemCount;

	DWORD	m_dwMouseClickTickTime;

public:
	CGamble();
	~CGamble();

	void LoadRes(int Type, CItemRender *pItemRender);
	void DeleteRes();
	CItem* Draw();
	LRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	void LoadExchange();
	void LoadGamble();
	void LoadRefine();

	void DeleteExchange();
	void DeleteGamble();

	void DrawExchange();
	void DrawRefine();
	void DrawGamble();
	void DrawGamble_CooSuk();
	void DrawCommon();
	void DrawMySlot();
	int CheckPannel(int x, int y);
	void ForcingCloseGamble();
	void ChipExchangeMulti(int type, BOOL bResult, int vnum, int count, int binggo_num, char * arg = NULL);
	void ChipExchange(int type, BOOL bResult, int vnum, int svr_idx, int binggo_num);

public:
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
	LRESULT GamMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT YutMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void InterCancel(BOOL bSend);
	void SendMsg();
	int  CheckMyslot_Exg();
	int  CheckMyslot_Gam();
	void ChangeToMulti();
	int CheckMyslot_Gam2();
	LRESULT GamMessage2( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void ChangeTo(int vnum, int svr_idx);
	void RemoveAllMyInven();

	BOOL IsOnGamble()
	{
		return (m_bMove == TRUE && m_bMoveEnd == FALSE);
	}

	BOOL m_bGambleStart;

	int		m_nMoney;

	int	CheckMyslot_JewelExg();
	void JewelExchange(BOOL bResult, int vnum, int svr_idx);
	LRESULT JewelExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	CSurface*	m_psExecution[2];
	CSurface*	m_psResult[RSIMG_END];

	int			m_nCnt;

	DWORD		m_dwStartTime;
	DWORD		m_dwCoolTime;

	bool		m_bImgRender;

	char		m_szArg[128];

	void LoadRullet(void);
	void DeleteRullet(void);
	void DrawRullet(void);
	LRESULT RulletMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void RulletExchangeMulti( int bResult, int vnum, int count, char * arg);
	void RulletResult(int result );
};

#endif // _GAMBLE_H_