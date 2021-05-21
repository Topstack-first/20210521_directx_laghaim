#ifndef _PET_LEVEL_H_
#define _PET_LEVEL_H_

#include "Config.h"

class CSurface;
class CBasicButton;
class CTextOutBox;
class CComboBox;

class CPetLevel // �ӽ÷� 2�� �ٿ��� ���� �����.
{
public:
	CPetLevel();
	virtual ~CPetLevel();

	CSurface	*m_pSurfaceBack;

	CComboBox m_Combo1; // ����, ��ȭ, ��ų ���� �����ϱ� ���� �޺� �ڽ�
	CComboBox m_Combo2; // ������,�����,��ų ���� ���� ���� ���� �޺� �ڽ�.

	CBasicButton *m_pBtn1; // ����.
	CBasicButton *m_pBtn2; // �ݱ�.
	CTextOutBox  *m_pMoneyText;
	char		  m_UserName[100];
	__int64 m_money;   //���� �����ݾ�
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
