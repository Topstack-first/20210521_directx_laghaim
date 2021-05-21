#ifndef _PET_LEVEL_H_
#define _PET_LEVEL_H_

#include "Config.h"

class CSurface;
class CBasicButton;
class CTextOutBox;
class CComboBox;

class CPetLevel // 임시로 2를 붙여서 새로 만든다.
{
public:
	CPetLevel();
	virtual ~CPetLevel();

	CSurface	*m_pSurfaceBack;

	CComboBox m_Combo1; // 교육, 진화, 스킬 등을 선택하기 위한 콤보 박스
	CComboBox m_Combo2; // 공격형,방어형,스킬 지정 등을 위한 서브 콤보 박스.

	CBasicButton *m_pBtn1; // 승인.
	CBasicButton *m_pBtn2; // 닫기.
	CTextOutBox  *m_pMoneyText;
	char		  m_UserName[100];
	__int64 m_money;   //현재 보유금액
	CTextOutBox  *m_pLevelText;
	CTextOutBox  *m_pExpText;
	CTextOutBox  *m_pNeedExpText;
	CTextOutBox  *m_pNeedMoneyText;

	int			GetWidth();
	int			GetHeight();
	void		SetXPos(int x);
	void		SetYPos(int y);
	void        SetMoney(__int64 money);
	void        CloseMenu();

	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	HRESULT Restore();

	BOOL IsInside(int x, int y);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HRESULT PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
};

#endif // _PET_LEVEL_H_
