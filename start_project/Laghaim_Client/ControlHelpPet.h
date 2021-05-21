#ifndef _CONTROL_HELPPET_ ///펫도움말
#define _CONTROL_HELPPET_

#include "HelpScript.h"

#define HELP_USE_FONT_SIZE 12 // 폰트 크기
#define HELP_USE_LINE_HEIGHT 16 // 한줄 크기

#define HELPPET_TALKBOX_VIEWTERM 1000 // 말풍선 띄우기 후 보여지는데까지 기달려주는 시간.
#define HELPPET_TALKBOX_CLOSETIME 5000 // 말풍선 자동 꺼지는 시간.

#define HELP_TALKBOX_PLUS_POSX -50 // 말풍선과 펫의 떨어진 위치.
#define HELP_TALKBOX_PLUS_POSY -10 // 말풍선과 펫의 떨어진 위치.

#define HELP_TALKBOX_XPLUS_SIZE 30 // 말풍선의 내용외 여백 크기.(양쪽 합친 크기.)
#define HELP_TALKBOX_YPLUS_SIZE 20 // 말풍선의 내용외 여백 크기.(양쪽 합친 크기.)

#define HELPPET_LIKE_HEIGHT 150 // 헬프 펫이 있으려고 하는 높이. 바닥에서의 거리임에 유의.

#define HELPPET_SEARCHRESULT_WIDTHSPACE 65 // 검색 결과 표시에서 가로에 필요한 여백 크기.
#define HELPPET_SEARCHRESULT_HEIGHTSPACE 55 // 검색 결과 표시에서 세로에 필요한 여백 크기.

#define HELPPET_SEARCHRESULT_HEIGHT 200 // 검색 결과 표시 창 크기. 그냥 픽스.
#define HELPPET_SEARCHRESULT_CONTROLPOSY 140 // / 다음 페이지 버튼 및 페이지 수 표시 의 위치용.

#define HELPPET_SEARCHRESULT_SEL_START_Y 40 // 선택지가 시작되는 위치.
#define HELPPET_SEARCHRESULT_SEL_LINE_HEIGHT 20 // 선택지 각 라인 크기.


enum { HELPPET_TYPE_RABIYONG = 0, MAX_HELPPET_TYPE };
enum { HELPPET_ANI_NORMAL = 0, HELPPET_ANI_FIND, HELPPET_ANI_THROW, HELPPET_ANI_ANGRY, HELPPET_ANI_SLEEPY, HELPPET_ANI_SEARCH, MAX_HELPPET_ANI }; ///펫도움말확장
#define HELPPET_FRAME_WIDTH 4 // 애니메이션의 한 이미지당 한 가로에 이미지 갯수.(펫 종류가 달라져도 이것과 애니메이션 별 프레임 총 갯수는 통일시키자.)
//enum { HELPPET_STATE_NORMAL = 0, HELPPET_STATE_FIND, HELPPET_STATE_TALKING, HELPPET_STATE_STUDYTIME, MAX_HELPPET_STATE };


enum { HELPPET_INTERFACE_TALKBOX = 0, HELPPET_INTERFACE_CONTENTSBOX, HELPPET_INTERFACE_SEARCHBOX, HELPPET_INTERFACE_SEARCHMENT, MAX_HELPPET_INTERFACE }; // 인터페이스.

#define HELPPET_SEARCHRESULT_VIEWINPAGE 5 // 한번에 표시되는 검색 결과 수.


class IndexedTexture;
//struct sHelpData;
//class CHelpDataManager;

class CControlHelpPet // 2D형 인터페이스가 아닌 3D형 인터페이스로 구현.(존니 거창해보이지만 그냥 서피스 아닌 버택스로 한다는 뜻;;)
{
public:
	CControlHelpPet();
	virtual ~CControlHelpPet();

	void	Init();

	void	DeleteRes();
	void	LoadRes( int Type );
	HRESULT RestoreSurfaces();

//	void	SetPetState( int State ); // 상태 변경.
	void	SetPetAnimation( int AniNum ); // 애니메이션 변경
	void	MakeTextSurface( int HelpNumber ); // CMultiText 를 서피스로 변환해주기 위한 녀석.

	void    FrameMove();
	void    Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInsidePet( int x, int y );
	BOOL	IsInsideTalkBox( int x, int y );
	BOOL	IsInsideContents( int x, int y );
	int		IsInsideSearchBox( int x, int y ); // 선택지가 선택되었을 경우 선택지의 번호를 리턴. 0이면 영역 밖, -1이면 선택지 이외 영역 안.

	int		CheckHelpCondition( int ConType, int Arg, BOOL bForced = FALSE ); // 등록된 헬프들의 조건을 돌면서 만족하는 녀석이 있는지 체크한다.
	BOOL    ExcuteHelpTalk( int HelpIndex ); // 대사를 발동 시킨다. 인덱스로 검사해서 실행하는 종류.
	BOOL    ExcuteHelpTalk( sHelpData *TempHelp ); // 대사를 발동 시킨다. 포인터로 실행하는 종류.
	BOOL	ExcuteHelpTalk( const char *strTalk ); // 직접 대사를 입력하는 경우...
	void    TerminateTalkBox(); // 말풍선을 지운다.
	BOOL    ExcuteHelpContents( int HelpIndex ); // 자세한 내용 보여주기를 발동 시킨다.
	void	TerminateContents(); // 보고있던 컨텐츠 부분을 초기화 해주는 녀석.

	BOOL HelpSearch( char *SearchWord ); // 헬프 검색 기능.


	HWND			m_hWnd;
	RECT			m_ClientRc;
	HFONT m_Font;

	// 펫 부분
	int m_nType; // 펫의 종류. (현재 펫의 종류에 따른 처리가 잘 안되어있음에 유의.혹시 나중에 추가하려고 하면 손좀 봐야한다.)
	int m_nAnimation; // 펫의 애니메이션 번호.
	DWORD m_dwPetAniChageTick; // 펫 동작과 관련된 부분의 기억 틱.
//	int m_nState; // 펫의 상태 ( 애니메이션과는 별도. 검색 중, 도움말 발동, 컨텐츠 표시중.등. )
//	DWORD m_dwStateChangeTick; // 상태 변경이 된 틱.
	int m_nDir; // 현재 방향. 0이면 왼쪽, 1이면 오른쪽.
	int m_nMoveFlag; // 좌우이동 중인지 멈춰있는지 플래그.
	int m_nSleepCount; // 졸기 프레임 증가용

	BOOL m_bPetDrag; // 펫 끌기용.
	int m_DragDistX; // 드래그에 사용할 위치차이 기억 변수.
	int m_DragDistY; // 드래그에 사용할 위치차이 기억 변수.
	int m_ClickX; // 클릭한 위치 기억.
	int m_ClickY; // 던지기에 사용한다. 던지고 난후에 방향용으로 사용.
	int m_ThrowPower; // 던지기에 사용된 힘.(던지면 점점 - 된다.)

	DWORD m_dwPetDragTick; // 드래그인지 클릭인지 체크용.

	IndexedTexture*		m_pIndexedTexture; // 펫 애니메이션을 저장해두기 위한 녀석.(이 녀석은 쓴 유저 자기자신껏만 보이므로 보일때만 로드해서 관리한다.)
	int m_AniIndex[MAX_HELPPET_ANI]; // 애니메이션의 텍스쳐 인덱스를 기억해둔다.
	RECT		    	m_PetRect; // 펫의 위치와 크기.
	RECT		    	m_TalksRect;
	int					m_TalkIndex; // 지금 출력중인 대사의 헬프 인덱스.
	D3DTLVERTEX			m_TLVec[4]; // 3D 그래픽 표시를 위한 버택스.

	int m_nNowFrame; // 현재 애니메이션 프레임.
	DWORD m_dwFrameRate; // 한 프레임 넘어가는 데 필요한 프레임 레이트. 틱 단위임에 유의.

	DWORD m_dwOldConCheckTick; // 이전 헬프 조건 발동을 검색했던 틱.

	DWORD m_dwOldFrameTick; // 이전 틱.

	CMultiText m_TalksText; // 이번에 출력해야하는 대사 내용.

	// 도움말 내용 부분.
	CHelpDataManager m_HelpManager; // 헬프 내용 정보관리자
	CSurface *m_pTextSurface; // 말풍선 대사 출력용.

	DWORD m_dwTalkStartTick; // 말풍선 발동 틱.
	int m_InterfaceTexIndex[MAX_HELPPET_INTERFACE]; // 인덱스 텍스쳐에서의 인덱스.
	CBasicButton	m_CloseBtn;     // 닫기 버튼



	// 상세한 도움말 페이지들 부분.
	BOOL m_bContents;
	RECT m_ContentsRect;
	vector<CSurface *> m_ListContentsSurface;   // 상세한 내용 설명 그림들. 페이지 방식, JPEG 로 로딩한다.
	int m_nNowContentsPage; // 현재 보고있는 내용의 페이지 번호.
	CBasicButton	m_PrevBtn2;		// 최소화 버튼
	CBasicButton	m_NextBtn2;		// 최소화 상태에서 다시 전개 버튼
	CBasicButton	m_CloseBtn2;     // 닫기 버튼
	BOOL m_bContentsDrag; // 위치이동 드래그 중인가.

	CTextOutBox     m_TextOutBox; // 글자 출력용.

	int m_BoundCount; // 던졌을 때 튕긴 횟수 기억용. ///펫도움말확장

	int m_Searching; // 검색 중인지 여부. 0이면 검색중 아님, 1이면 검색어 입력중, 2이면 검색 결과 보기메뉴 ///펫도움말확장
	RECT		    	m_SearchRect;
	CTextBox m_SearchInput; // 검색어 입력용.
	CBasicButton	m_SearchBtn;		// 검색 버튼
	CMultiText2 m_SearchResultText; // 200 라인까지 받을수있는 녀석.
	vector<int> m_SearchResultIndex;
	int m_NowViewResult; // 현재 보고있는 검색결과 위치. (페이지 관리 때문에 필요)
	int m_TotalResult; // 검색 결과 갯수.
	CBasicButton	m_PrevBtn;		// 최소화 버튼
	CBasicButton	m_NextBtn;		// 최소화 상태에서 다시 전개 버튼
	int m_NowSelResult; // 현재 선택된 선택지의 번호.(1부터 시작)

//	LPDIRECTSOUNDBUFFER m_pNewMessage; // 뉴 메세지시의 효과음.
};


#endif