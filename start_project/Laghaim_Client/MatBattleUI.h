#ifndef _MAT_BATTLE_UI_H_
#define _MAT_BATTLE_UI_H_

#include "Config.h"

class CListbox;
class CNewListbox;
class CSurface;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;


class CMatBattleUI
{
public:
	CSurface * m_pBack;
	CSurface * m_pSurNonMaster;

	CNewListBox		*m_WaitList;
	CNewListBox		*m_BlueTeamList;
	CNewListBox		*m_RedTeamList;

	CBasicButton *m_pBtnOut, *m_pBtnBeA, *m_pBtnBeB, *m_pBtnWait;
	CBasicButton *m_pBtnStart, *m_pBtnShop, *m_pBtnExit;

	CMsgPopUp    * m_pMsgPopUp;

	CTextOutBox *m_pWaitNumText;
	CTextOutBox *m_nBlueNumText;
	CTextOutBox *m_pRedNumText;


	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;

	// 방장용 UI를 위한 변수.
	BOOL	   m_bRoomOwner;
	void	   SetUserType(BOOL bRoomOwner)
	{
		m_bRoomOwner = bRoomOwner;
	};

	void	   TRoom(char *cmd, char *strName);
	void	   AddMember(char *strName, int which_loc, BOOL bWaiting, BOOL bOwner);

	void RoomUpdate(int what_cmd, char *strName);
	void DeleteCha(char *strName);
	int m_WaitSelectLst;
	int m_RedSelectLst;
	int m_BlueSelectLst;
	BOOL m_bMaster;
	BOOL m_bReady;
	void FindCha(char *strName, BOOL bReady, BOOL bMater);

	char m_commOut[512];
	char m_MasterName[50];
	char m_MyName[50];

	BOOL m_bBlueTeamSelected;
	BOOL m_bRedTeamSelected;


public:
	CMatBattleUI();
	virtual ~CMatBattleUI();

	void	LoadRes();
	void	DeleteRes();

	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
};

#endif // _MAT_BATTLE_UI_H_