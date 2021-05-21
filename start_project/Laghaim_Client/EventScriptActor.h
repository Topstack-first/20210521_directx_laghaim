#ifndef _EVENT_SCRIPT_ACTOR_
#define _EVENT_SCRIPT_ACTOR_

#include "ScriptActFunc.h"
#include "EventScript.h"

#define MAX_SCRIPT_TEXTLINE 10 // 한 씬에서 쓰일수있는 텍스트 최대라인수.

#define SCRIPT_USE_FONT_SIZE 12 // 폰트 크기
#define SCRIPT_USE_LINE_HEIGHT 16 // 한줄 크기
#define SCRIPT_TEXT_SPACE_LEFT 5 // 텍스트 출력시 왼쪽 여백.
#define SCRIPT_TEXT_SPACE_TOP 5 // 텍스트 출력시 위쪽 여백.

extern HFONT G_GetFont(int h, int w,const char  * const fontname);

enum { EVENT_SCRIPT_STATE_RUN = 0 /*실행 중*/, EVENT_SCRIPT_STATE_WAITACTION/*이벤트 연출 대기중*/, EVENT_SCRIPT_STATE_PAGEWAIT
	   , EVENT_SCRIPT_STATE_WAITCLICK/*선택 대기중*/, EVENT_SCRIPT_STATE_FADING/*씬 페이드 인 아웃중*/
	 }; // 스크립트의 상태 정의

enum { EVENT_ACTION_TUTORIAL_START = 1, EVENT_ACTION_TUTORIAL_MOVE, EVENT_ACTION_TUTORIAL_CAM, EVENT_ACTION_TUTORIAL_HUNT, EVENT_ACTION_TUTORIAL_POINTUP // 5
	   , EVENT_ACTION_TUTORIAL_POSIONBUY, EVENT_ACTION_TUTORIAL_POSIONUSE, EVENT_ACTION_TUTORIAL_EQUIPBUY, EVENT_ACTION_TUTORIAL_EQUIPUSE, EVENT_ACTION_TUTORIAL_TOPMENU // 10
	   , EVENT_ACTION_TUTORIAL_PARTY, EVENT_ACTION_TUTORIAL_EXCHANGE, EVENT_ACTION_TUTORIAL_PEACEMODE, EVENT_ACTION_TUTORIAL_END // 14
	   , MAX_EVENT_ACTION_FUNC
	 };

class CEventScriptActor // 스크립트 실행자
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

		m_pSelection1Surf = NULL; // 선택지는 개행을 지원안한다.
		m_pSelection2Surf = NULL; // 선택지는 밑에서 위치를 재서 출력한다.
		m_pSelection3Surf = NULL;
		m_pSelectionOn1Surf = NULL; // 선택지는 개행을 지원안한다.
		m_pSelectionOn2Surf = NULL; // 선택지는 밑에서 위치를 재서 출력한다.
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

		m_OnePageLine = 0; // 대사 출력시 한 페이지당 표시가능 라인 수.
		m_NowViewLine = 0; // 현재 보여지는 라인 시작 위치.
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

		m_pNowScene = NULL; // 참조포인터니까 여기서 초기화하자.

		m_Font = G_GetFont(SCRIPT_USE_FONT_SIZE,0,NULL);

		ZeroMemory( &m_BoxArea, sizeof(RECT) );
		ZeroMemory( &m_TextArea, sizeof(RECT) );
		m_NowPrintLine = 0;
		m_NowPrintChar = 0;
		m_NowMouseOnSel = 0;
	}

	BOOL LoadScript( const char* FileName ); // 스크립트 및 텍스쳐를 로드.
	void SceneChange( int SceneIndex, BOOL bNotCheckFade = FALSE ); // 씬 전환 처리. 대사를 서피스로 만들고 웨이트 타임이나 웨이트 액션등을 처리해준다.
	void RunScript(); // 스크립트를 구동. 출력도 여기서 담당한다.
	void RenderScript(); // 스크립트 출력.
	BOOL MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // 메세지 프로시져.
	BOOL IsInsideTalkBox(int x, int y); // 대사창 영역 채크.
	int IsInsideSelection(int x, int y); // 선택지 영역 채크.

	CActFuncManager m_Action;

	int m_TextureNum; // 텍스쳐 갯수.
	IndexedTexture *m_pIndexedTexture; // 텍스쳐 관리.

	// 텍스트 출력용 정보.
	int m_OnePageLine; // 대사 출력시 한 페이지당 표시가능 라인 수.
	int m_NowViewLine; // 현재 보여지는 라인 시작 위치.
//	int m_StopLine; // 넘어가서 멈춰있는 라인.
	DWORD m_dwStopTick; // 멈춘 틱.

	int m_ContantsLineCount; // 컨텐츠 스트링의 라인 갯수.
	char *m_ContantsText[MAX_SCRIPT_TEXTLINE]; // 한 씬당 문장들을 개행해서 끊어놓은 녀석.
	CSurface *m_pContantsOutSurf[MAX_SCRIPT_TEXTLINE];

	CSurface *m_pClickSurf; // 클릭은 별도로...
	CSurface *m_pClickOnSurf; // 클릭은 별도로...

	int m_SelectionLineCount;
	CSurface *m_pSelection1Surf; // 선택지는 개행을 지원안한다.
	CSurface *m_pSelection2Surf; // 선택지는 밑에서 위치를 재서 출력한다.
	CSurface *m_pSelection3Surf;
	CSurface *m_pSelectionOn1Surf; // 선택지는 개행을 지원안한다.
	CSurface *m_pSelectionOn2Surf; // 선택지는 밑에서 위치를 재서 출력한다.
	CSurface *m_pSelectionOn3Surf;


	int m_ScriptState; // 스크립트의 상태
	DWORD m_Waiting; // 시간 대기가 있을 경우의 멈춤이 풀릴 시간.(틱단위.)
	int m_Fading; // 현재 씬 페이딩 중인지 여부와 남은 시간.(틱단위)(-면 페이드인,+면 페이드아웃.)
	int m_ReserveNextScent; // 페이드 아웃시에 효과 종료후 이동할 씬을 기억해두기 위한 변수.

	CScriptManager m_ScriptMgr; // 데이터 관리자.
	sSceneData *m_pNowScene; // 현재 씬의 포인터.

	HFONT m_Font;

	// 출력 계산용.
	RECT m_BoxArea;
	RECT m_TextArea;
	int m_NowPrintLine; // 이번 출력 라인
	int m_NowPrintChar; // 이번 출력 글자.(해당 라인에서의 위치)
	int m_NowMouseOnSel; // 현재 마우스가 선택한 선택지 번호. 0 이하는 선택안됨.

};


#endif