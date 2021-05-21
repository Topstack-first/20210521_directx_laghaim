#ifndef _CONTROL_NOTICE_H_ ///공지팝업창
#define _CONTROL_NOTICE_H_

#include "Config.h"

#include <dsound.h>

class CSurface;
class CTextOutBox;
class CBasicButton;
class CScroll;
class CMultiText;

#define POPUP_NOTICE_TEXTSUR_H_SPACE 45 // 텍스트 서피스의 위아래 여백의 합.
#define POPUP_NOTICE_TEXTLINE_HEIGHT 14 // 텍스트 라인 한줄의 높이.(폰트 높이)

#define POPUP_NOTICE_TITLEBAR_SIZE_Y 28 // 타이틀 바의 높이.(고정)


#define POPUP_NOTICE_MINIMIZE_SIZE_X 65
#define POPUP_NOTICE_MINIMIZE_SIZE_Y 30

#define POPUP_NOTICE_SIZECONTROL_SIZE_X 20
#define POPUP_NOTICE_SIZECONTROL_SIZE_Y 20

#define MAX_CONTANTS_LINE 100 // 받을 수있는 줄수.

enum { NOTICE_TYPE_NORMAL = 0, NOTICE_TYPE_MOBIUS };
enum { NOTICE_STATE_FULL = 0, NOTICE_STATE_MINI };

// 운영자 공지 팝업창. 기존의 단순 텍스트 공지가 아니라 인터페이스로 뜨는 공지이다.
class CControlNotice // 2D형 인터페이스가 아닌 3D형 인터페이스로 구현.(존니 거창해보이지만 그냥 서피스 아닌 버택스로 한다는 뜻;;)
{
public:
	CControlNotice();
	virtual ~CControlNotice();

	void	Init();

	void	DeleteRes();
	void	LoadRes();
	HRESULT RestoreSurfaces();

	void	SetType( int Type ); // 타이틀 변경.
	void	SetContents( char *String ); // 스트링 삽입.
	void	MakeTextSurface(); // CMultiText 를 서피스로 변환해주기 위한 녀석.

	void    Draw(); // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
	void    RenderOj(int xpos, int ypos, int cx, int cy); // 3D쪽 그리기.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );
	BOOL	IsTitleBarInside( int x, int y ); // 무빙을 위한 타이틀 바.
	BOOL	IsSizeControlInside( int x, int y ); // 사이즈 조절을 위한 가장자리를 클릭했는가.

	void WindowChange( int x, int y, int size_right, int size_bottom ); // 창 위치나 사이즈가 바뀔때 해줘야하는 것들


	HWND			m_hWnd;
	RECT			m_ClientRc;

	int m_nType; // 무슨 공지인가.(이것에 따라 타이틀 출력이 바뀐다.)
	int m_PosX; // 창의 위치
	int m_PosY; // 창의 위치
	int m_nState; // 인터페이스의 상태 ( 0 : 전개 상태, 1 : 최소화 상태 )
	BOOL m_bMoveDrag; // 위치이동 드래그 중인가.
	BOOL m_bSizeDrag; // 사이즈 조절 드래그 중인가.
	BOOL m_bNewMessage; // 새로운 메세지가 와있는지. (깜빡이자)
	RECT m_SizeRect; // 사이즈 조절 가능하므로 이걸로 기억.
	int m_ViewLineNum; // 현재 사이즈에서 한번에 보여지는 텍스트 줄 수.
	int m_TotalLineNum; // 현재 사이즈에서 보여지는 텍스트 총 줄 수.

	D3DTLVERTEX			m_TLVec[4]; // 창 표시를 위한 버택스.

	CSurface *m_pTitleBar;     // 창 타이틀 부분의 글자 배경.(버택스 사이즈 조절에 영향을 안 받을수있도록...
	CSurface *m_pTitleBarLight;     // 창 타이틀 부분의 글자 배경.(버택스 사이즈 조절에 영향을 안 받을수있도록...
	CSurface *m_pTitle;     // 창 타이틀 부분의 글자 배경.(버택스 사이즈 조절에 영향을 안 받을수있도록...

	CMultiText2 m_Contents; // 표시 내용.

	CSurface *m_pContentsSurface; // 표시 내용의 텍스트를 서피스로 변환해놓은 것. 사이즈 조절시마다 다시 만들어 줘야한다.

	CScroll m_Scroll; // 텍스트 스크롤 용.

	CBasicButton	m_MiniBtn;		// 최소화 버튼
	CBasicButton	m_MaxBtn;		// 최소화 상태에서 다시 전개 버튼
	CBasicButton	m_CloseBtn;     // 닫기 버튼

	CSurface *m_pSizeControl;     // 사이즈 조절을 위한 클릭용 이미지


	LPDIRECTSOUNDBUFFER m_pNewMessage; // 뉴 메세지시의 효과음.
};

#endif // _CONTROL_NOTICE_H_