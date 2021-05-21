#ifndef _MAT_LIST_H_
#define _MAT_LIST_H_

#include "Config.h"

class CSurface;
class CBasicButton;
class CTextOutBox;
class CTcpIpConnection;
class CMsgPopUp;

class CMatRoomList
{
public:
	CMatRoomList();
	~CMatRoomList();

	char m_strTitle[100], m_strMaster[35], m_strPassWord[4];
	int  m_nRoomNum, m_nRoomType, m_nCurCount, m_nMaxCount, m_nStatus, m_nMinLevel, m_nMaxLevel, m_nPay;

	void AddData(int nRoomNum, int nRoomType, char *strRoomName, int nCount, int nMaxCount, char *strMaster, int nState, int nMinLevel, int nMaxLevel, int nFee, char *strPasswd);
};


class CMatList
{
public:
	CSurface * m_pBack;
	CSurface * m_pSecret;

	CTextOutBox	*m_pNumText;
	CTextOutBox *m_pTitleText;
	CTextOutBox *m_pTypeText;
	CTextOutBox *m_pCur_MaxCountText;
	CTextOutBox *m_pMasterText;
	CTextOutBox *m_pStateText;
	CTextOutBox *m_nMin_MaxLevelText;
	CTextOutBox *m_pPayText;

	CTextOutBox *m_pMsgText;

	CBasicButton * m_pBtn_Enter1;
	CBasicButton * m_pBtn_Enter2;
	CBasicButton * m_pBtn_Enter3;
	CBasicButton * m_pBtn_Enter4;

	CBasicButton * m_pBtn_Make;
	CBasicButton * m_pBtn_Prev;
	CBasicButton * m_pBtn_Next;
	CBasicButton * m_pBtn_Refresh;
	CBasicButton * m_pBtn_WaitView;
	CBasicButton * m_pBtn_AllView;
	CBasicButton * m_pBtn_Close;

	CMsgPopUp		m_MsgPopUp;

	CTextOutBox  * m_pTextOut;

	int		   m_nPageNum, m_nCurRoomNum;
	BOOL       m_bActive;
	int		   m_iMainX, m_iMainY;
	int        m_ScreenWidth, m_ScreenHeight;
	BOOL		m_IsAllView;

public:
	CMatList();
	virtual ~CMatList();

	void	AddRoomData(int nRoomNum, int nRoomType, char *strRoomName, int nCount, int nMaxCount, char *strMaster, int nState, int nMinLevel, int nMaxLevel, int nFee, char *strPasswd);
	void	ClearAllRoomList();

	void	GetRoomList(BOOL IsAllView, int PageNum);
	int		SelectRoom(int cur_room);
	BOOL	EnterRoom(int cur_sel, char* password = NULL);

	void	Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
};

#endif // _MAT_LIST_H_