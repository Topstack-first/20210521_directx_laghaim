#ifndef _SCRIPT_ACTION_FUNC_ ///튜토리얼 시 추가
#define _SCRIPT_ACTION_FUNC_

class NkMob;

// 인터페이스 클래스.
class IScriptActFunc
{
public:
	IScriptActFunc() {}
	virtual ~IScriptActFunc() {}

	virtual bool Render() = NULL;
	virtual int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) = NULL;
	virtual bool EndCheck() = NULL;
};

// 매니져 클래스
class CActFuncManager
{
public:
	CActFuncManager()
	{
		m_pFunc = NULL;
	};

	BOOL ReadyAction( int ActionNum );
	void ReleaseAction( int ActionNum );

	IScriptActFunc *m_pFunc;
};


// 튜토리얼 시작 부
class CTutorialStartAct : public IScriptActFunc
{
public:
	CTutorialStartAct();
	~CTutorialStartAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.
};

// 튜토리얼 이동 체험.
class CTutorialMoveAct : public IScriptActFunc
{
public:
	CTutorialMoveAct();
	~CTutorialMoveAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	int m_nPointPhase; // 현재 몇번째 위치 표시중인지..
	CRectPlane m_Pointing1;
	CRectPlane m_Pointing2;

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.

	BOOL m_bEndFlag; // 종료 체크용.
};

// 튜토리얼 시점 변경 체험.
class CTutorialCamAct : public IScriptActFunc
{
public:
	CTutorialCamAct();
	~CTutorialCamAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	int m_nPointPhase; // 현재 몇번째 위치 표시중인지..

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

	int m_Data1; // 누적량 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.

	BOOL m_bEndFlag; // 종료 체크용.
};


// 튜토리얼 사냥 체험.
class CTutorialHuntAct : public IScriptActFunc
{
public:
	CTutorialHuntAct();
	~CTutorialHuntAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.


	BOOL m_bEndFlag; // 종료 체크용.
};


// 튜토리얼 포인트 업 체험.
class CTutorialPointAct : public IScriptActFunc
{
public:
	CTutorialPointAct();
	~CTutorialPointAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.


	BOOL m_bEndFlag; // 종료 체크용.
};

// 튜토리얼 물약사기 체험.
class CTutorialPosionBuyAct : public IScriptActFunc
{
public:
	CTutorialPosionBuyAct();
	~CTutorialPosionBuyAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	DWORD m_dwPrevClickTick; // 더블 클릭 체크용. 이전 클릭 틱.

	BOOL m_bEndFlag; // 종료 체크용.
};

// 튜토리얼 물약사용 체험.
class CTutorialPosionUseAct : public IScriptActFunc
{
public:
	CTutorialPosionUseAct();
	~CTutorialPosionUseAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.

	BOOL m_bEndFlag; // 종료 체크용.
};

// 튜토리얼 장비구입 체험.
class CTutorialEquipBuyAct : public IScriptActFunc
{
public:
	CTutorialEquipBuyAct();
	~CTutorialEquipBuyAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.


	BOOL m_bEndFlag; // 종료 체크용.
};

// 튜토리얼 장비 착용
class CTutorialEquipUseAct : public IScriptActFunc
{
public:
	CTutorialEquipUseAct();
	~CTutorialEquipUseAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.

	BOOL m_bEndFlag; // 종료 체크용.
};


// 튜토리얼 메뉴바 체험
class CTutorialTopMenuAct : public IScriptActFunc
{
public:
	CTutorialTopMenuAct();
	~CTutorialTopMenuAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.
	int m_nPointPhase; // 현재 몇번째 위치 표시중인지..

	BOOL m_bEndFlag; // 종료 체크용.
};


// 튜토리얼 파티신청 체험
class CTutorialPartyAct : public IScriptActFunc
{
public:
	CTutorialPartyAct();
	~CTutorialPartyAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.
	int m_nPointPhase; // 현재 진행도
	DWORD m_dwEndFlagTick; // 미니맵 열면 카운트해서 약간 보여주고 종료되도록.

	BOOL m_bEndFlag; // 종료 체크용.
};


// 튜토리얼 교환 체험
class CTutorialExchangeAct : public IScriptActFunc
{
public:
	CTutorialExchangeAct();
	~CTutorialExchangeAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.
	int m_nPointPhase; // 현재 진행도

	BOOL m_bEndFlag; // 종료 체크용.
};

// 튜토리얼 전투모드/평화모드 체험
class CTutorialPeaceModeAct : public IScriptActFunc
{
public:
	CTutorialPeaceModeAct();
	~CTutorialPeaceModeAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // 타이밍 체크용.

//	CNkMob* m_pTeacher; // 교관 포인터.
	int m_nPointPhase; // 현재 진행도
	BOOL m_bPrevMode; // 이전 모드 기억용.

	BOOL m_bEndFlag; // 종료 체크용.
};

// 튜토리얼 종료. (패킷보내기위해...)
class CTutorialEndAct : public IScriptActFunc
{
public:
	CTutorialEndAct();
	~CTutorialEndAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();
};

#endif
