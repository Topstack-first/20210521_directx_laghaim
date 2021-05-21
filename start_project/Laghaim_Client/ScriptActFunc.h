#ifndef _SCRIPT_ACTION_FUNC_ ///Ʃ�丮�� �� �߰�
#define _SCRIPT_ACTION_FUNC_

class NkMob;

// �������̽� Ŭ����.
class IScriptActFunc
{
public:
	IScriptActFunc() {}
	virtual ~IScriptActFunc() {}

	virtual bool Render() = NULL;
	virtual int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) = NULL;
	virtual bool EndCheck() = NULL;
};

// �Ŵ��� Ŭ����
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


// Ʃ�丮�� ���� ��
class CTutorialStartAct : public IScriptActFunc
{
public:
	CTutorialStartAct();
	~CTutorialStartAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.
};

// Ʃ�丮�� �̵� ü��.
class CTutorialMoveAct : public IScriptActFunc
{
public:
	CTutorialMoveAct();
	~CTutorialMoveAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	int m_nPointPhase; // ���� ���° ��ġ ǥ��������..
	CRectPlane m_Pointing1;
	CRectPlane m_Pointing2;

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	CNkMob* m_pTeacher; // ���� ������.

	BOOL m_bEndFlag; // ���� üũ��.
};

// Ʃ�丮�� ���� ���� ü��.
class CTutorialCamAct : public IScriptActFunc
{
public:
	CTutorialCamAct();
	~CTutorialCamAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	int m_nPointPhase; // ���� ���° ��ġ ǥ��������..

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

	int m_Data1; // ������ üũ��.

//	CNkMob* m_pTeacher; // ���� ������.

	BOOL m_bEndFlag; // ���� üũ��.
};


// Ʃ�丮�� ��� ü��.
class CTutorialHuntAct : public IScriptActFunc
{
public:
	CTutorialHuntAct();
	~CTutorialHuntAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.


	BOOL m_bEndFlag; // ���� üũ��.
};


// Ʃ�丮�� ����Ʈ �� ü��.
class CTutorialPointAct : public IScriptActFunc
{
public:
	CTutorialPointAct();
	~CTutorialPointAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.


	BOOL m_bEndFlag; // ���� üũ��.
};

// Ʃ�丮�� ������ ü��.
class CTutorialPosionBuyAct : public IScriptActFunc
{
public:
	CTutorialPosionBuyAct();
	~CTutorialPosionBuyAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	DWORD m_dwPrevClickTick; // ���� Ŭ�� üũ��. ���� Ŭ�� ƽ.

	BOOL m_bEndFlag; // ���� üũ��.
};

// Ʃ�丮�� ������ ü��.
class CTutorialPosionUseAct : public IScriptActFunc
{
public:
	CTutorialPosionUseAct();
	~CTutorialPosionUseAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	CNkMob* m_pTeacher; // ���� ������.

	BOOL m_bEndFlag; // ���� üũ��.
};

// Ʃ�丮�� ����� ü��.
class CTutorialEquipBuyAct : public IScriptActFunc
{
public:
	CTutorialEquipBuyAct();
	~CTutorialEquipBuyAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.


	BOOL m_bEndFlag; // ���� üũ��.
};

// Ʃ�丮�� ��� ����
class CTutorialEquipUseAct : public IScriptActFunc
{
public:
	CTutorialEquipUseAct();
	~CTutorialEquipUseAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	CNkMob* m_pTeacher; // ���� ������.

	BOOL m_bEndFlag; // ���� üũ��.
};


// Ʃ�丮�� �޴��� ü��
class CTutorialTopMenuAct : public IScriptActFunc
{
public:
	CTutorialTopMenuAct();
	~CTutorialTopMenuAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	CNkMob* m_pTeacher; // ���� ������.
	int m_nPointPhase; // ���� ���° ��ġ ǥ��������..

	BOOL m_bEndFlag; // ���� üũ��.
};


// Ʃ�丮�� ��Ƽ��û ü��
class CTutorialPartyAct : public IScriptActFunc
{
public:
	CTutorialPartyAct();
	~CTutorialPartyAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	CNkMob* m_pTeacher; // ���� ������.
	int m_nPointPhase; // ���� ���൵
	DWORD m_dwEndFlagTick; // �̴ϸ� ���� ī��Ʈ�ؼ� �ణ �����ְ� ����ǵ���.

	BOOL m_bEndFlag; // ���� üũ��.
};


// Ʃ�丮�� ��ȯ ü��
class CTutorialExchangeAct : public IScriptActFunc
{
public:
	CTutorialExchangeAct();
	~CTutorialExchangeAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	CNkMob* m_pTeacher; // ���� ������.
	int m_nPointPhase; // ���� ���൵

	BOOL m_bEndFlag; // ���� üũ��.
};

// Ʃ�丮�� �������/��ȭ��� ü��
class CTutorialPeaceModeAct : public IScriptActFunc
{
public:
	CTutorialPeaceModeAct();
	~CTutorialPeaceModeAct();

	bool Render();
	int  MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	bool EndCheck();

	DWORD m_dwStartTick;
	DWORD m_dwOldElapsed; // Ÿ�̹� üũ��.

//	CNkMob* m_pTeacher; // ���� ������.
	int m_nPointPhase; // ���� ���൵
	BOOL m_bPrevMode; // ���� ��� ����.

	BOOL m_bEndFlag; // ���� üũ��.
};

// Ʃ�丮�� ����. (��Ŷ����������...)
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
