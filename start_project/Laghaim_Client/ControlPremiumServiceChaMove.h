#ifndef _PREMIUM_SERVICE_CHA_MOVE_HEADER_
#define _PREMIUM_SERVICE_CHA_MOVE_HEADER_

#include "Config.h"
#include "BasicButton.h"
#include "TextBox.h"
#include "NkCharacter.h"
#include "ControlInven.h"
#include "WebCtrl.h"
#include "Scroll.h"
#include "BasicMenu.h"


class CSurface;
class CTextBox;
class CItem;

typedef enum WHOAU
{
	WHOAU_NONE = -1,
	FROM,
	TO
};


class CControlPremiumChaMove
{
protected:

	WHOAU	m_Whoau;
	int		m_Step;

	BOOL	m_bAgree;

	CSurface *m_pSurfAgreeBg;
	CSurface *m_pSurfAgreeYes;
	CSurface *m_pSurfAgreeNo;
	CSurface *m_pSurfWearingBg;

	CSurface *m_pSurfChaName;
	CSurface *m_pSurfChaLevel;
	CSurface *m_pSurfChaRace;

	CBasicButton	m_BtnOk;		 // "확인" 버튼
	CBasicButton	m_BtnCancle;	 // "취소" 버튼
	CBasicButton	m_BtnReceive;	 // "받기" 버튼

	CItem		*m_Wearing[WEARING_NUM];	// 보낼 장비를 보여준다.
	CEquipSlot	m_EquipSlot[WEARING_NUM];

	char	m_strNameFrom[32];
	char	m_strNameTo[32];
	int		m_Race;
	int		m_Level;

	HFONT		m_Font;

	CWebWnd m_WebAgree;

	CMultiScrollText2* m_AgreeMultiText;

public:

	CControlPremiumChaMove();
	~CControlPremiumChaMove();

	void	LoadRes();
	void	DeleteRes();
	void	ClearChaInfo();

	void	SetStep(WHOAU whoau = WHOAU_NONE, int step = 0);
	void	SetFont (int f_w, int f_h, char *fontname);
	CItem*	Draw();

	void	DrawAgree();
	CItem*	DrawInven();
	CItem*	DrawWearing();
	void	SetEquipArea();

	HRESULT	MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HRESULT	MsgProcAgree(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	// 동의 창
	HRESULT	MsgProcInven(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	// 안밴 청

	BOOL	OnCheckFromInfo();
	BOOL	IsInside(int x, int y);
	BOOL	ToggleAgreeBtn();
	BOOL	IsInsideAgreeBtn(int x, int y);

	void	OnCancleChaMove();
	void	SetChaInfo(char* nameFrom, char* nameTo, int level, int race);
	void	SetWearing(int wear, CItem* pItem);

	void	Wear(int where, int index, int vnum, int gem_num, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur);
	void	DeleteWear();

	void	WebOpen();
	void	WebClose();


	int m_slot;
	void SetSlot(int slot);
};
#endif