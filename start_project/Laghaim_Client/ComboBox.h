#ifndef _COMBO_BOX_H_
#define _COMBO_BOX_H_

#include "Config.h"

#include "basicbutton.h" // 내부적으로 기본 버튼을 필요로 하므로 사용.

#define MAX_COMBO_LIST 10 // 리스트 맥스치. 부족하면 늘려라.

#define COMBO_STATE_DISABLE -1 // 비활성.
#define COMBO_STATE_NORMAL 0 // 일반.
#define COMBO_STATE_SELECT 1 // 선택중.
#define COMBO_STATE_ANIMATE 2 // 애니메이션 중.(전개)

// 콤보박스
class CComboBox // 제한적인 갯수의 스트링 또는 단일 이미지 용 콤보박스이다. 스크롤이라던지 롤오버 버튼 등을 넣고 싶으면 상속해서 만들자!
{
public:
	CComboBox()
	{
//		m_pDisplay = NULL; // 참조 포인터.
		m_pSelectedBackGround = NULL;
		m_pListBackGround = NULL;
		m_pListViewBtn = NULL;

		for( int i = 0 ; i < MAX_COMBO_LIST ; ++i )
		{
			m_pListItem[i] = NULL;
			m_pListOverItem[i] = NULL;
		}

		Init();
	}

	CComboBox( int Width, int Height, int X_Plus = 0, int Y_Plus = 0 ) // 사이즈를 미리 받는 생성자. 일반적으로 이쪽으로 생성하자.(사이즈를 셋팅하는걸 까먹지 않도록...사이즈 셋팅은 이미지 로드 전에 이루어져야한다.)
	{
//		m_pDisplay = NULL; // 참조 포인터.
		m_pSelectedBackGround = NULL;
		m_pListBackGround = NULL;
		m_pListViewBtn = NULL;

		for( int i = 0 ; i < MAX_COMBO_LIST ; ++i )
		{
			m_pListItem[i] = NULL;
			m_pListOverItem[i] = NULL;
		}

		Init();

		SetItemSize( Width, Height, X_Plus, Y_Plus );
	}

	~CComboBox()
	{
		Init();
	}

	void Init()
	{
		int i = 0;

		m_State = 0; // 0이면 보통 상태, -1이면 비활성 상태, 1이면 선택가능 상태, 2이면 전개중 상태.
		m_NowSelectIndex = 0; // 현재 선택되어있는 리스트의 인덱스.
		m_NowOverIndex = 0; // 마우스가 올라가 있는 아이템의 인덱스.
		m_InsertedCount = 0;

		m_ItemWidth = 0; // 리스트의 아이템 가로 길이.(전 리스트는 통일되어 있어야한다.)
		m_ItemHeight = 0; // 리스트의 아이템 세로 길이.(전 리스트는 통일되어 있어야한다.)
		m_X_Draw_Plus = 0; // 리스트 출력시 x좌표 보정치
		m_Y_Draw_Plus = 0; // 리스트 출력시 y좌표 보정치

		m_DrawPosX = 0; // 실제 찍히는 좌표.
		m_DrawPosY = 0;

		m_AnimateHeight = 0.0f; // 전개 애니메이션시에 쓰이는 전개 세로 길이.
		m_dwElapsedTick = 0;

		m_nBtnPosX = 0;
		m_nBtnPosY = 0;

		SAFE_DELETE( m_pSelectedBackGround );
		SAFE_DELETE( m_pListBackGround );
		SAFE_DELETE( m_pListViewBtn );

		for( i = 0 ; i < MAX_COMBO_LIST ; ++i )
		{
			SAFE_DELETE( m_pListItem[i] );
			SAFE_DELETE( m_pListOverItem[i] );
		}
	}

	void LoadRes( char *ListViewBtnName = NULL, char *SelectedBackGroundName = NULL, char *ListBackGroundName = NULL ); // 사이즈 셋팅은 이미지 로드 전에 이루어져야한다.
	void DeleteRes();

	int InsertItem( char* ItemText, int FontSize = 12, DWORD TextColor = RGB(200, 200, 200), DWORD OverColor = RGB(50, 50, 200), DWORD BackColor = RGB(0, 0, 0) );
	int InsertItemByImage( char* ImageFileName ); // 이미지로 삽입.

	void DeleteAllItem(); // 현재는 전부 지우는 함수만 지원. 개별로 지우는 것까진 만들 시간이 없다! 나중에 필요할때 추가하자.

	int GetState()
	{
		return m_State;
	}

	void SetState( int value )
	{
		m_State = value;
	}

	int GetSelect()
	{
		return m_NowSelectIndex;
	}

	void SetSelect( int value )
	{
		m_NowSelectIndex = value;
	}

	void SetItemSize( int Width, int Height, int X_Plus = 0, int Y_Plus = 0 ) // 아이템 하나당 가로,세로, 출력시 x좌표 보정치, 출력시 y좌표 보정치. // 사이즈 셋팅은 이미지 로드 전에 이루어져야한다.
	{
		m_ItemWidth = Width; // 리스트의 아이템 가로 길이.(전 리스트는 통일되어 있어야한다.)
		m_ItemHeight = Height; // 리스트의 아이템 세로 길이.(전 리스트는 통일되어 있어야한다.)
		m_X_Draw_Plus = X_Plus; // 리스트 출력시 x좌표 보정치
		m_Y_Draw_Plus = Y_Plus; // 리스트 출력시 y좌표 보정치
	}

	void SetPosition( int x, int y )
	{
		m_DrawPosX = x;
		m_DrawPosY = y;
	}

	int IsInsideList( int, int ); // 리스트 클릭시...몇번을 클릭했는지를 리턴(0미선택시 0리턴, 배열상에서 0은 디폴트 아이템이므로 리스트에선 보여지지 않는다.)

	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);

	void Draw();

	void BtnSetPosition(int nX, int nY)
	{
		m_nBtnPosX = nX;
		m_nBtnPosY = nY;

		m_pListViewBtn->SetPosition(m_nBtnPosX, m_nBtnPosY);
	}
	void	DrawImage();
	int		IsInsideListImage( int, int );
	LRESULT MsgProcImage( HWND, UINT, WPARAM, LPARAM );


	CSurface	*m_pSelectedBackGround; // 선택되어있는 녀석을 보여주는 위치의 배경.
	CSurface	*m_pListBackGround; // 리스트들의 배경. 그냥 검은색 반투명 박스로 쓰일 예정.
	CBasicButton	*m_pListViewBtn; // 리스트를 보여주기 위한 버튼. 비활성 상태일땐 안 눌린다.
	CSurface	*m_pListItem[MAX_COMBO_LIST]; // 리스트 목록 (스트링 또는 이미지를 사용할 수 있다.) // 0번은 디폴트 아이템으로 최초 시작시 설명용 스트링(또는 이미지)임에 유의.
	CSurface	*m_pListOverItem[MAX_COMBO_LIST]; // 마우스 오버시에 보여지는 서피스.               // 실직적인 리스트 아이템은 1번부터 시작된다고 보면된다.

	int m_State; // 0이면 보통 상태, -1이면 비활성 상태, 1이면 선택가능 상태, 2이면 전개중 상태.
	int m_NowSelectIndex; // 현재 선택되어있는 리스트의 인덱스.
	int m_NowOverIndex; // 마우스가 올라가 있는 아이템의 인덱스.
	int m_InsertedCount; // 입력된 리스트 아이템 갯수.

	int m_DrawPosX; // 실제 찍히는 좌표.
	int m_DrawPosY;

	int m_ItemWidth; // 리스트의 아이템 가로 길이.(전 리스트는 통일되어 있어야한다.)
	int m_ItemHeight; // 리스트의 아이템 세로 길이.(전 리스트는 통일되어 있어야한다.)
	int m_X_Draw_Plus; // 리스트 출력시 x좌표 보정치(리스트의 보정치이지. 리스트 배경의 보정치가 아님에 유의! 배경들의 테두리 크기라고 생각하면 될 듯.)
	int m_Y_Draw_Plus; // 리스트 출력시 y좌표 보정치(리스트의 보정치이지. 리스트 배경의 보정치가 아님에 유의! 배경들의 테두리 크기라고 생각하면 될 듯.)

	float m_AnimateHeight; // 전개 애니메이션시에 쓰이는 전개 세로 길이.
	DWORD m_dwElapsedTick; // 전개 애니메이션시에 쓰이는 시간기억용. 콤보박스마다 하나씩 필요한듯.(함수에서 static쓰면 두개가 같이 작동이 잘 안되더라.)

	int m_nBtnPosX;
	int m_nBtnPosY;
};

#endif // _COMBO_BOX_H_