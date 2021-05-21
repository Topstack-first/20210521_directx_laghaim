#ifndef _EVENT_SCRIPT_ACTOR_
#define _EVENT_SCRIPT_ACTOR_

#include "ScriptActFunc.h"
#include "EventScript.h"

#define MAX_SCRIPT_TEXTLINE 10 // �� ������ ���ϼ��ִ� �ؽ�Ʈ �ִ���μ�.

#define SCRIPT_USE_FONT_SIZE 12 // ��Ʈ ũ��
#define SCRIPT_USE_LINE_HEIGHT 16 // ���� ũ��
#define SCRIPT_TEXT_SPACE_LEFT 5 // �ؽ�Ʈ ��½� ���� ����.
#define SCRIPT_TEXT_SPACE_TOP 5 // �ؽ�Ʈ ��½� ���� ����.

extern HFONT G_GetFont(int h, int w,const char  * const fontname);

enum { EVENT_SCRIPT_STATE_RUN = 0 /*���� ��*/, EVENT_SCRIPT_STATE_WAITACTION/*�̺�Ʈ ���� �����*/, EVENT_SCRIPT_STATE_PAGEWAIT
	   , EVENT_SCRIPT_STATE_WAITCLICK/*���� �����*/, EVENT_SCRIPT_STATE_FADING/*�� ���̵� �� �ƿ���*/
	 }; // ��ũ��Ʈ�� ���� ����

enum { EVENT_ACTION_TUTORIAL_START = 1, EVENT_ACTION_TUTORIAL_MOVE, EVENT_ACTION_TUTORIAL_CAM, EVENT_ACTION_TUTORIAL_HUNT, EVENT_ACTION_TUTORIAL_POINTUP // 5
	   , EVENT_ACTION_TUTORIAL_POSIONBUY, EVENT_ACTION_TUTORIAL_POSIONUSE, EVENT_ACTION_TUTORIAL_EQUIPBUY, EVENT_ACTION_TUTORIAL_EQUIPUSE, EVENT_ACTION_TUTORIAL_TOPMENU // 10
	   , EVENT_ACTION_TUTORIAL_PARTY, EVENT_ACTION_TUTORIAL_EXCHANGE, EVENT_ACTION_TUTORIAL_PEACEMODE, EVENT_ACTION_TUTORIAL_END // 14
	   , MAX_EVENT_ACTION_FUNC
	 };

class CEventScriptActor // ��ũ��Ʈ ������
{
public:
	CEventScriptActor()
	{
		int i = 0;
		m_pIndexedTexture = NULL;

		for( i = 0 ; i < MAX_SCRIPT_TEXTLINE ; i++ )
		{
			m_ContantsText[i] = NULL;
			m_pContantsOutSurf[i] = NULL;
		}

		m_pSelection1Surf = NULL; // �������� ������ �������Ѵ�.
		m_pSelection2Surf = NULL; // �������� �ؿ��� ��ġ�� �缭 ����Ѵ�.
		m_pSelection3Surf = NULL;
		m_pSelectionOn1Surf = NULL; // �������� ������ �������Ѵ�.
		m_pSelectionOn2Surf = NULL; // �������� �ؿ��� ��ġ�� �缭 ����Ѵ�.
		m_pSelectionOn3Surf = NULL;
		m_pClickSurf = NULL;
		m_pClickOnSurf = NULL;

		Init();
	}

	~CEventScriptActor()
	{
		Init();
	}

	void Init()
	{
		int i = 0;

		m_ScriptState = EVENT_SCRIPT_STATE_RUN;
		m_Waiting = 0;
		m_Fading = 0;
		m_ReserveNextScent = -1;

		m_ScriptMgr.Init();

		m_OnePageLine = 0; // ��� ��½� �� �������� ǥ�ð��� ���� ��.
		m_NowViewLine = 0; // ���� �������� ���� ���� ��ġ.
//		m_StopLine = 0;
		m_dwStopTick = 0;


		for( i = 0 ; i < MAX_SCRIPT_TEXTLINE ; i++ )
		{
			SAFE_DELETE_ARRAY( m_ContantsText[i] );
			SAFE_DELETE( m_pContantsOutSurf[i] );
		}
		m_ContantsLineCount = 0;

		m_SelectionLineCount = 0;
		SAFE_DELETE( m_pSelectionOn3Surf );
		SAFE_DELETE( m_pSelectionOn2Surf );
		SAFE_DELETE( m_pSelectionOn1Surf );
		SAFE_DELETE( m_pSelection3Surf );
		SAFE_DELETE( m_pSelection2Surf );
		SAFE_DELETE( m_pSelection1Surf );

		SAFE_DELETE( m_pClickOnSurf );
		SAFE_DELETE( m_pClickSurf );



		m_TextureNum = 0;
		SAFE_DELETE( m_pIndexedTexture );

		m_pNowScene = NULL; // ���������ʹϱ� ���⼭ �ʱ�ȭ����.

		m_Font = G_GetFont(SCRIPT_USE_FONT_SIZE,0,NULL);

		ZeroMemory( &m_BoxArea, sizeof(RECT) );
		ZeroMemory( &m_TextArea, sizeof(RECT) );
		m_NowPrintLine = 0;
		m_NowPrintChar = 0;
		m_NowMouseOnSel = 0;
	}

	BOOL LoadScript( const char* FileName ); // ��ũ��Ʈ �� �ؽ��ĸ� �ε�.
	void SceneChange( int SceneIndex, BOOL bNotCheckFade = FALSE ); // �� ��ȯ ó��. ��縦 ���ǽ��� ����� ����Ʈ Ÿ���̳� ����Ʈ �׼ǵ��� ó�����ش�.
	void RunScript(); // ��ũ��Ʈ�� ����. ��µ� ���⼭ ����Ѵ�.
	void RenderScript(); // ��ũ��Ʈ ���.
	BOOL MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // �޼��� ���ν���.
	BOOL IsInsideTalkBox(int x, int y); // ���â ���� äũ.
	int IsInsideSelection(int x, int y); // ������ ���� äũ.

	CActFuncManager m_Action;

	int m_TextureNum; // �ؽ��� ����.
	IndexedTexture *m_pIndexedTexture; // �ؽ��� ����.

	// �ؽ�Ʈ ��¿� ����.
	int m_OnePageLine; // ��� ��½� �� �������� ǥ�ð��� ���� ��.
	int m_NowViewLine; // ���� �������� ���� ���� ��ġ.
//	int m_StopLine; // �Ѿ�� �����ִ� ����.
	DWORD m_dwStopTick; // ���� ƽ.

	int m_ContantsLineCount; // ������ ��Ʈ���� ���� ����.
	char *m_ContantsText[MAX_SCRIPT_TEXTLINE]; // �� ���� ������� �����ؼ� ������� �༮.
	CSurface *m_pContantsOutSurf[MAX_SCRIPT_TEXTLINE];

	CSurface *m_pClickSurf; // Ŭ���� ������...
	CSurface *m_pClickOnSurf; // Ŭ���� ������...

	int m_SelectionLineCount;
	CSurface *m_pSelection1Surf; // �������� ������ �������Ѵ�.
	CSurface *m_pSelection2Surf; // �������� �ؿ��� ��ġ�� �缭 ����Ѵ�.
	CSurface *m_pSelection3Surf;
	CSurface *m_pSelectionOn1Surf; // �������� ������ �������Ѵ�.
	CSurface *m_pSelectionOn2Surf; // �������� �ؿ��� ��ġ�� �缭 ����Ѵ�.
	CSurface *m_pSelectionOn3Surf;


	int m_ScriptState; // ��ũ��Ʈ�� ����
	DWORD m_Waiting; // �ð� ��Ⱑ ���� ����� ������ Ǯ�� �ð�.(ƽ����.)
	int m_Fading; // ���� �� ���̵� ������ ���ο� ���� �ð�.(ƽ����)(-�� ���̵���,+�� ���̵�ƿ�.)
	int m_ReserveNextScent; // ���̵� �ƿ��ÿ� ȿ�� ������ �̵��� ���� ����صα� ���� ����.

	CScriptManager m_ScriptMgr; // ������ ������.
	sSceneData *m_pNowScene; // ���� ���� ������.

	HFONT m_Font;

	// ��� ����.
	RECT m_BoxArea;
	RECT m_TextArea;
	int m_NowPrintLine; // �̹� ��� ����
	int m_NowPrintChar; // �̹� ��� ����.(�ش� ���ο����� ��ġ)
	int m_NowMouseOnSel; // ���� ���콺�� ������ ������ ��ȣ. 0 ���ϴ� ���þȵ�.

};


#endif