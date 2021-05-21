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

	BOOL m_bMacroEditing; // 매크로 모드 편집중인가 플래그. 편집중엔 매크로 모드 창만 보여준다.
	CControlPetMacro *m_pControlMacro; // 매크로 모드 인터페이스. CLShiftMenu가 멤버로 가지고 관리하자.

	CSurface	*m_pSurfaceBack;
	CSurface	*m_pApple;
	CBasicButton *m_pBtn1;
	CBasicButton *m_pBtn2;
	CBasicButton *m_pBtn3;
	CBasicButton *m_pBtn4;
	CBasicButton *m_pBtn5; // 닫기
	CBasicButton *m_pBtn6; // 생산모드?
	CBasicButton *m_pBtn7; // 원거리 계열 해제 모드
	CBasicButton *m_pBtn8; // 경계모드(진화 라비용만 사용가능)
	CBasicButton *m_pBtn9; // 매크로 모드(진화 라비용만 사용가능)
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

class CControlPetMacro // 펫 행동모드 매크로 창. CLShiftMenu의 멤버로 사용되는 클래스.
{
public:
	CControlPetMacro();
	virtual ~CControlPetMacro();

	CSurface	*m_pSurfaceBack;
	CBasicButton m_RunBtn; // 닫기 버튼
	CBasicButton m_CheckBtn; // 체크 버튼(이미지는 하나만 로드해서 쓴다.)
	BOOL m_bNowInput; // 글자 입력중인가 여부. 이걸로 퀵슬롯 사용안되게 체크한다.

	int m_NowUse; // 현재 사용되는 매크로 번호.
	DWORD m_dwPrevChangeTick; // 이전에 매크로가 진행된 시간.

	BOOL m_bUseCheck[PET_MACRO_MAX]; // 사용 유무 체크.
	CComboBox m_ModeComboBox[PET_MACRO_MAX]; // 모드 선택.
	CTextBox m_DelayInputText[PET_MACRO_MAX]; // 딜레이 입력창.

	void Init();
	void LoadRes();
	void DeleteRes();

	void SaveMacro(); // 간단한 파일 저장
	void LoadMacro(); // 기억해둔 이전 매크로를 로드.

	void StartMacroMode(); // 처음 실행할 매크로를 찾아서 패킷 보내주고 매크로 모드 셋팅해주는 녀석.
	int CheckMacroTime(); // 매크로 모드 바꿔주기 체크.
	void Draw();

	void		SetPos(int x, int y);
	BOOL IsInside(int x, int y);
	int IsInsideContents(int x, int y); // 인터페이스 상에 작동하는 컨텐츠 위치인가 체크.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

#endif // _L_SHIFT_MENU_H_
