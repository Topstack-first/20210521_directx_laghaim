#ifndef _L_SHIFT_MENU_H_
#define _L_SHIFT_MENU_H_

#include "Config.h"

#define PET_MACRO_MAX 5

class CSurface;
class CBasicButton;
class CTextOutBox;
class CComboBox;

class CControlPetMacro;

class CLShiftMenu
{
public:
	CLShiftMenu();
	virtual ~CLShiftMenu();

	BOOL m_bMacroEditing; // ��ũ�� ��� �������ΰ� �÷���. �����߿� ��ũ�� ��� â�� �����ش�.
	CControlPetMacro *m_pControlMacro; // ��ũ�� ��� �������̽�. CLShiftMenu�� ����� ������ ��������.

	CSurface	*m_pSurfaceBack;
	CSurface	*m_pApple;
	CBasicButton *m_pBtn1;
	CBasicButton *m_pBtn2;
	CBasicButton *m_pBtn3;
	CBasicButton *m_pBtn4;
	CBasicButton *m_pBtn5; // �ݱ�
	CBasicButton *m_pBtn6; // ������?
	CBasicButton *m_pBtn7; // ���Ÿ� �迭 ���� ���
	CBasicButton *m_pBtn8; // �����(��ȭ ���븸 ��밡��)
	CBasicButton *m_pBtn9; // ��ũ�� ���(��ȭ ���븸 ��밡��)
	CTextOutBox  *m_pNameText;
	char		  m_UserName[100];

	CAlphaPopUp		*m_pMenuInfo;
	char			m_StrTemp[128];
	char			m_StrTempSupp[128];
	char            m_StrTempPk[128];

	int			GetWidth();
	int			GetHeight();
	void		SetXPos(int x);
	void		SetYPos(int y);
	void		SetPos(int x, int y);
	void		SetCurRace(int race);
	void        SetUserName(char* name);
	void        CloseMenu();


	void Init();
	void LoadRes();
	void DeleteRes();

	void Draw();

	BOOL IsInside(int x, int y);
	BOOL IsFoodSlot(int x, int y);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

class CControlPetMacro // �� �ൿ��� ��ũ�� â. CLShiftMenu�� ����� ���Ǵ� Ŭ����.
{
public:
	CControlPetMacro();
	virtual ~CControlPetMacro();

	CSurface	*m_pSurfaceBack;
	CBasicButton m_RunBtn; // �ݱ� ��ư
	CBasicButton m_CheckBtn; // üũ ��ư(�̹����� �ϳ��� �ε��ؼ� ����.)
	BOOL m_bNowInput; // ���� �Է����ΰ� ����. �̰ɷ� ������ ���ȵǰ� üũ�Ѵ�.

	int m_NowUse; // ���� ���Ǵ� ��ũ�� ��ȣ.
	DWORD m_dwPrevChangeTick; // ������ ��ũ�ΰ� ����� �ð�.

	BOOL m_bUseCheck[PET_MACRO_MAX]; // ��� ���� üũ.
	CComboBox m_ModeComboBox[PET_MACRO_MAX]; // ��� ����.
	CTextBox m_DelayInputText[PET_MACRO_MAX]; // ������ �Է�â.

	void Init();
	void LoadRes();
	void DeleteRes();

	void SaveMacro(); // ������ ���� ����
	void LoadMacro(); // ����ص� ���� ��ũ�θ� �ε�.

	void StartMacroMode(); // ó�� ������ ��ũ�θ� ã�Ƽ� ��Ŷ �����ְ� ��ũ�� ��� �������ִ� �༮.
	int CheckMacroTime(); // ��ũ�� ��� �ٲ��ֱ� üũ.
	void Draw();

	void		SetPos(int x, int y);
	BOOL IsInside(int x, int y);
	int IsInsideContents(int x, int y); // �������̽� �� �۵��ϴ� ������ ��ġ�ΰ� üũ.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

#endif // _L_SHIFT_MENU_H_
