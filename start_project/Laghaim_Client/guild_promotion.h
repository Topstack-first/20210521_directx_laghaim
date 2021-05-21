#ifndef _GUILD_PROMOTION_H_
#define _GUILD_PROMOTION_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include "basicbutton.h"
#include "TextBox.h"
#include "Scroll.h"
#include "basewindow.h"

class CItem;
class CTextBox;

enum
{
	FIRST_BETA = 0,
	SECOND_DELTA,
	THIRD_ZETA,
	FOURTH_THETA,
	FIFTH_IOTA,
	SIXTH_PI,
	SEVENTH_PHI,
	EIGHTTH_KAI,
	NINETH_XI,
	BT_PROMOTION_NUM
};

class CGuildCreate : public CUIBaseWindow
{
public:
	CGuildCreate();
	virtual ~CGuildCreate();

	/*********************************************************************/
	int		Draw();
	void	DrawMySlot();

	void	HookClose();
	void	DeleteRes();
	void	RestoreSurfaces() {};
	BOOL    IsInside(int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*********************************************************************/
	BOOL	InitMode();
	void	LoadRes();
	void	guilds_SendMsg();
	/*********************************************************************/
	BOOL	MoveWindow(UINT msg, int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}
	void	SetPosition();
	/*********************************************************************/
	bool	CheckGuldName();
	BOOL	CheckSpace(char* String);
	void	ReadProperList(char* szFileName);

	int			m_nProperListCount;
	char**		m_pszProperList;
	/*********************************************************************/

	CBasicButton	m_btCreate;
	char			m_MsgBuf[1024];

	//CSurface*		m_pBackground;

	CSurface*		m_pExistSlot;
	CSurface*		m_pEnableSlot;
	CSurface*		m_pDisableSlot;
	CItem*			m_pOverItem;

	CTextBox		m_GuildName;
};

class CGuildPromotion : public CUIBaseWindow
{
public:
	CGuildPromotion();
	virtual ~CGuildPromotion();

	/*********************************************************************/
	int		Draw();

	void	DeleteRes();
	void	RestoreSurfaces() {};
	BOOL    IsInside(int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*********************************************************************/
	BOOL	InitMode();
	void	LoadRes();
	/*********************************************************************/
	BOOL	MoveWindow(UINT msg, int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}
	void	SetPosition();
	/*********************************************************************/

	void	guilds_Bt_Status(int nGLevel);
	void	guilds_SendMsg();

private:

	CBasicButton	m_btPromotion[BT_PROMOTION_NUM];
	CBasicButton	m_btCancel;
	char			m_MsgBuf[1024];

	CSurface*		m_pBackground;
	CSurface*       m_pTextInfo;
};

class CGuildCombination : public CUIBaseWindow
{
public:
	CGuildCombination();
	virtual ~CGuildCombination();

	/*********************************************************************/
	int		Draw();
	void	DrawMySlot();

	void	DeleteRes();
	void	RestoreSurfaces() {};
	BOOL    IsInside(int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*********************************************************************/
	BOOL	InitMode();
	void	LoadRes();
	void	guilds_SendMsg();
	/*********************************************************************/
	BOOL	MoveWindow(UINT msg, int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}
	void	SetPosition();
	/*********************************************************************/

	CBasicButton	m_btPromote;
	CBasicButton	m_btCancel;

	char			m_MsgBuf[1024];

//	CSurface*		m_pBackground;

	CSurface*		m_pExistSlot;
	CSurface*		m_pEnableSlot;
	CSurface*		m_pDisableSlot;
	CItem*			m_pOverItem;
};

class CGuildMix : public CUIBaseWindow
{
public:
	CGuildMix();
	virtual ~CGuildMix();

	/*********************************************************************/
	int		Draw();
	void	DrawMySlot();

	void	DeleteRes();
	void	RestoreSurfaces() {};
	BOOL    IsInside(int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*********************************************************************/
	BOOL	InitMode();
	void	LoadRes();
	void	guilds_SendMsg();
	/*********************************************************************/
	BOOL	MoveWindow(UINT msg, int x, int y);
	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}
	void	SetPosition();
	/*********************************************************************/

	CBasicButton	m_btMix;
	CBasicButton	m_btCancel;
	char			m_MsgBuf[1024];

//	CSurface*		m_pBackground;

	CSurface*		m_pExistSlot;
	CSurface*		m_pEnableSlot;
	CSurface*		m_pDisableSlot;
	CItem*			m_pOverItem;
};
#endif // _GUILD_PROMOTION_H_