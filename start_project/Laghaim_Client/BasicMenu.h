#ifndef _BASIC_MENU_H_
#define _BASIC_MENU_H_

#include "Config.h"
#include "LHUI_Base.h"
extern float locationX, locationZ;
#define MENU_FILE_NAME_LEN 256


// 등록된 메뉴 타입 리스트(메뉴가 오픈될때 구분용.)
#define MENU_TYPE_USERSHOP			1 // 개인상점 
#define MENU_TYPE_NPCUPGRADE		2 // 강화,가공,컨버젼 등...
#define MENU_TYPE_BPSHOP			3 // BP 상점
#define MENU_TYPE_ENCHANT			4 // [2/27/2007 Theodoric] 인첸트 : 플래티늄 상인
#define MENU_TYPE_LPSHOP			5 // LP 상점
#define MENU_TYPE_NEW_NPCUPGRADE	6
#define MENU_TYEP_ITEMMAKE			7 // 아이템 분해 제작
#define MENU_TYPE_BUFFMAKE			8

// 액션 리스트.(버튼마다 등록되는 고유 행동 번호이다.)
#define MENU_ACTION_CLOSE -1            // 공통
#define MENU_ACTION_USERSHOP_SEARCH			1 // MENU_TYPE_USERSHOP
#define MENU_ACTION_USERSHOP_RECEIVE		2 // MENU_TYPE_USERSHOP
#define MENU_ACTION_NPCUPGRADE_UPGRADE		3 // MENU_TYPE_NPCUPGRADE
#define MENU_ACTION_NPCUPGRADE_CONVERSION	4 // MENU_TYPE_NPCUPGRADE
#define MENU_ACTION_NPCUPGRADE_CONFUSION	5 // MENU_TYPE_NPCUPGRADE
#define MENU_ACTION_NPCUPGRADE_REFORM		6 // MENU_TYPE_NPCUPGRADE

#define MENU_ACTION_ENCHANT_UPGRADE			7 // [2/27/2007 Theodoric] 인첸트 : MENU_TYPE_ENCHANT 강화
#define MENU_ACTION_ENCHANT_MAKE 			8 // [2/27/2007 Theodoric] 인첸트 : MENU_TYPE_ENCHANT 플레티늄 제작
#define MENU_ACTION_ENCHANT_INIT			9 // [2/27/2007 Theodoric] 인첸트 : MENU_TYPE_ENCHANT 초기화

#define MENU_ACTION_NEW_NPCUPGRADE_UPGRADE		10	// [2008/11/28 Theodoric] 300레벨 이상의 무기 강화
#define MENU_ACTION_NEW_NPCUPGRADE_CONVERSION	11	// [2008/11/28 Theodoric] 300레벨 이상의 무기 컨버젼
#define MENU_ACTION_NEW_NPCUPGRADE_EXCHANGE		12	// [2008/11/28 Theodoric] 300레벨 이상의 무기 교환

#define MENU_ACTION_ITEMMAKE					13
#define MENU_ACTION_ITEMSEPARATE				14
#define MENU_ACTION_GMAGICSTONE					15
#define MENU_ACTION_GODWEAPON_UP				16

#define MENU_ACTION_NPCUPGRADE_ARMOR400			17	// 400레벨 이상의 방어구 강화

#define MENU_ACTION_BUFFMAKE_MAKE				18
#define MENU_ACTION_BUFFMAKE_GATCHA				19
#define MENU_ACTION_BUFFMAKE_MEDAL				20

struct sMenuSurface
{
	char FileName[MENU_FILE_NAME_LEN]; // 리스토어용 파일명.

	int x, y; // 출력 좌표.
	int Width,Height; // 기억용.

	CSurface *m_pSurface;     // 실제 이미지.
};

struct sMenuButton // 이건 출력 좌표땜에 필요.
{
	int ActionIndex; // 눌렸을때 작동할 액션 인덱스.
	int x, y; // 출력 좌표.

	CBasicButton m_Button;     // 실제 버튼.
};

struct sMenuString
{
	int x, y; // 출력 좌표.

	char String[MENU_FILE_NAME_LEN]; // 일단 256까지만...
};

class CMultiScrollText // 멀티텍스트에 스크롤 기능을 달아 출력시켜주는 클래스.
{
public:
	CMultiText m_MultiText;
	CSurface*		m_pTextSur; // 출력할 서피스.

	CScroll *m_pScroll;

	RECT m_ViewRect; // 출력할 크기.
	int m_ViewHeightSize;  // 출력할 크기.(보여지는 세로 길이)

	HFONT			m_Font;

	int				m_FontWidth, m_FontHeight;
	int				m_Xpos,		 m_Ypos;
	int				m_TextSurWidth, m_TextSurHeight;

	char m_DecBtnName[MENU_FILE_NAME_LEN]; // 스크롤 업 버튼 경로 // 자체 리스토어 용.
	char m_IncBtnName[MENU_FILE_NAME_LEN]; // 스크롤 다운 버튼 경로
	char m_BarName[MENU_FILE_NAME_LEN]; // 스크롤 바 경로

	CMultiScrollText();
	~CMultiScrollText();
	void Init( int FontSize = 12 );
	void DeleteRes();
	void LoadRes( int l, int t, int r,int b );

	void SetViewRect( int l, int t, int r,int b );
	void SetFont (int f_w, int f_h, char *fontname ); // 폰트를 바꾸고 싶을때 사용.
	void SetScrollImage( char *DecBtn = NULL, char *IncBtn = NULL, char *ScrollBar = NULL ); // 스크롤을  변경하고 싶을때 사용.
	void SetScrollPos( int x = -1, int y = -1, int size = -1 ); // 스크롤 위치및 크기를 변경하고 싶을때 사용.

	void AddString( char *String, int LineLen = -1, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR  ); // 스트링 라인 추가. 그냥 멀티텍스트에 넣기 쉽게 해주기 용.

	void LoadString( char *FileName ); // 텍스트 파일로 부터 스트링을 읽어오는 함수.

	void MakeSurface(); // 멀티 텍스트의 내용을 서피스로 만들어주는 녀석.

	void Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // 스크롤용.
};



struct sMenuMultiScrollText
{
	RECT OutRect; // 출력 좌표.

	int ScrollPosX, ScrollPosY, ScrollHeight;

	char DecBtnName[MENU_FILE_NAME_LEN]; // 스크롤 업 버튼 경로
	char IncBtnName[MENU_FILE_NAME_LEN]; // 스크롤 다운 버튼 경로
	char BarName[MENU_FILE_NAME_LEN]; // 스크롤 바 경로

//	int FontWidth, int FontHeight, char FontName[MENU_FILE_NAME_LEN]; // 폰트 정보. // 폰트 정보는 직접 셋팅하도록 변경.

	CMultiScrollText ScrollText;
};


// 배경,스트링,버튼들을 임의로 지정할 수 있는 기능을 가진 메뉴의 유저 인터페이스.
class CControlMenu : public LHUI_Base
{
public:

	CControlMenu();
	~CControlMenu();

	void	Init();
	void	DeleteRes();
	HRESULT RestoreSurfaces();
	void	LoadRes(); // 각 멤버들의 Add가 실행된 후에 호출되어야 할듯.
	void ClearList();

	void AddBackGround( const char *FileName, int PosX, int PosY, int Width, int Height ); // 배경 지정. 위치가 -1이면 자동 중앙 위치다.
	void AddSurface( const char *FileName, int PosX, int PosY, int Width = -1, int Height = -1 ); // 이녀석은 필요에 따라 가로 세로를 입력해두어야한다.
	void AddButton( const char *FileName, int PosX, int PosY, int ActionIndex );
	void AddString( const char *String, int PosX, int PosY, BOOL bCenter = FALSE );
	CMultiScrollText* AddScrollText( int PosX, int PosY, int Width, int Height, int ScrollPosX = -1, int ScrollPosY = -1, int ScrollHeight = -1, const char *DecBtn = NULL, const char *IncBtn = NULL, const char *Bar = NULL /*, int f_w = 6, int f_h = 12, char *fontname = NULL*/ );

	void	Draw(); // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void BtnAction( int ActionIndex ); // 버튼을 눌렀을때 동작할 코드.

	BOOL	IsInside( int x, int y );


	HWND			m_hWnd;
	RECT			m_ClientRc;

	CTextOutBox m_TextOut; // 텍스트 출력용.(하나를 가지고 출력시에 돌려씀)(쉽게 말하면 스트링 하나하나가 아니라 출력을 위한 버퍼같은 것임)

	sMenuSurface m_BackSur; // BackGround 이미지.
	list<sMenuSurface *> m_SurfaceList; // 이미지 리스트.
	list<sMenuButton *> m_BtnList; // 버튼 리스트.
	list<sMenuString *> m_StringList; // 스트링.
	vector<sMenuMultiScrollText *> m_ScrollTextList; // 스크롤 스트링. // 이녀석은 list쓰면 워링이 절라 떠서 vector로 바꿨음.
};


//////////////////////////////////////////////////////////////////////////


class CMultiScrollText2 // 멀티텍스트에 스크롤 기능을 달아 출력시켜주는 클래스.
{
public:
	CMultiText2		m_MultiText;
	CSurface*		m_pTextSur; // 출력할 서피스.

	CScroll *m_pScroll;

	RECT m_ViewRect; // 출력할 크기.
	int m_ViewHeightSize;  // 출력할 크기.(보여지는 세로 길이)

	HFONT			m_Font;

	int				m_FontWidth, m_FontHeight;
	int				m_Xpos,		 m_Ypos;
	int				m_TextSurWidth, m_TextSurHeight;

	char m_DecBtnName[MENU_FILE_NAME_LEN]; // 스크롤 업 버튼 경로 // 자체 리스토어 용.
	char m_IncBtnName[MENU_FILE_NAME_LEN]; // 스크롤 다운 버튼 경로
	char m_BarName[MENU_FILE_NAME_LEN]; // 스크롤 바 경로

	CMultiScrollText2();
	~CMultiScrollText2();
	void Init( int FontSize = 12 );
	void DeleteRes();
	void LoadRes( int l, int t, int r,int b );

	void SetViewRect( int l, int t, int r,int b );
	void SetFont (int f_w, int f_h, char *fontname ); // 폰트를 바꾸고 싶을때 사용.
	void SetScrollImage( char *DecBtn = NULL, char *IncBtn = NULL, char *ScrollBar = NULL ); // 스크롤을  변경하고 싶을때 사용.
	void SetScrollPos( int x = -1, int y = -1, int size = -1 ); // 스크롤 위치및 크기를 변경하고 싶을때 사용.

	void AddString( char *String, int LineLen = -1, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR  ); // 스트링 라인 추가. 그냥 멀티텍스트에 넣기 쉽게 해주기 용.

	void LoadString( char *FileName ); // 텍스트 파일로 부터 스트링을 읽어오는 함수.

	void MakeSurface(); // 멀티 텍스트의 내용을 서피스로 만들어주는 녀석.

	void Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // 스크롤용.
};

//////////////////////////////////////////////////////////////////////////


#endif // _BASIC_MENU_H_