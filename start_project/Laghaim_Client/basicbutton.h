#ifndef _BASIC_BUTTON_H_
#define _BASIC_BUTTON_H_

#include "Config.h"

#include <vector> ///유료상점 시 추가
#include "ddutil.h"
using namespace std; ///유료상점 시 추가

#define TAB_ALIGN_HORIZON     0 // 가로 정렬 방식 ///유료상점 시 추가
#define TAB_ALIGN_VERTICAL    1 // 세로 정렬 방식 ///유료상점 시 추가

class CBasicButton;
class CDirectSound;

class CBasicButton
{
public:
public:
	CSurface* m_surface;	// 스프라이트 이미지

	char m_bmpfile[80];

	RECT m_rc_button_flag;

public:
	int PosX;			// 화면에 위치할 x 좌표
	int PosY;			// 화면에 위치할 y 좌표
	int Width;			// 버튼 이미지의 가로길이
	int Height;			// 버튼 이미지의 세로길이

	int m_iBtnFlg;			// 버튼이 보여지는 상태의 인덱스
	bool m_bDisable;		// 버튼 그릴때 enable 인지 disable 인지
	bool bDown;				// 버튼이 눌렸는지 안 눌렸는지
	BOOL m_bVisible;

	CBasicButton();
	CBasicButton(const char * filename,int w=0, int h=0);
	virtual ~CBasicButton();

	void SetSize( int, int);
	void SetPosition( int x, int y )
	{
		PosX = x, PosY = y;
	}
	void SetDisable( bool );
	void SetDisable2( bool );
	void SetFileName( const char* );
	bool IsInside( int, int );
	void LoadRes( int w = -1, int h = -1 );
	void DeleteRes();
	void Draw();
	void Draw( int x, int y, BOOL bChecked = FALSE );
	BOOL DrawClip(int nMinArea, int nMaxArea, int nScrollPos);
	BOOL IsInsideClip(int x, int y, int nMinArea, int nMaxArea, int nScrollPos);
	void MsgProcClip( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScrollPos, int nMin, int nMax);
	HRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);
	int  GetState()
	{
		return m_iBtnFlg;
	}
	void SetState(int state)
	{
		m_iBtnFlg = state;
	}
	void SetVisible(BOOL bVisible)
	{
		m_bVisible = bVisible;
	}

	BOOL IsDisable();
	void DrawToBlt(BOOL bChecked = FALSE);
	RECT m_BtnRc;

	void Init( const char * filename , int w=-1, int h=-1 );
};

class CTabControl ///유료상점 시 추가
{
public:

	CTabControl();
	~CTabControl();

	void SetPosition( int x, int y, int w, int h  );
	void SetTabStyle( int Align, int DrawPosX, int DrawPosY, int Space ); // Align은 가로, 세로 정렬 구분.
	void SetIncDecBtn( char* DecFileName, int DecX, int DecY,  char* IncFileName, int IncX, int IncY );
	void Clear(); // 탭버튼 리스트를 싹 지운다.
	BOOL AddTab( char* BtnFileName, char* ExString = NULL ); // ExString은 탭 버튼과 연결되 동작등을 하기위해 스트링을 연결해둔다. 스트리응ㄹ 어떻게 스던지는 자요.
	char *GetExtraString( int Index ); // 저장해둔 엑스트라 스트링을 불러오기위한 녀석.
	void DeleteRes();
	void RestoreSurfaces();
	void Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	bool IsInside( int x, int y );
	int GetMouseOnIndex( int x, int y );

	int NowDrawStartIndex; // 그려지기 시작하는 번호.(스크롤처럼 중간부터 그릴수도 있다.)
	int NowSelectedIndex; // 현재 선택되어진 탭.

	int m_TapAlign; // 가로인지 세로인지 여부.(0이면 가로, 1이면 세로)
	int m_TabDrawPosX; // 탭 그려지기 시작하는 좌표.(탭 컨트롤중 공백을 넣고 싶을때 사용가능)
	int m_TabDrawPosY; // 탭 그려지기 시작하는 좌표.(탭 컨트롤중 공백을 넣고 싶을때 사용가능)
	int m_TabSpace; // 탭과 탭사이의 여백 공간.

	int m_TabCount; // 등록되어진 탭의 갯수.

	int m_PosX;
	int m_PosY;
	int m_TabControlWidth;
	int m_TabControlHeight;
	CBasicButton m_DecBtn;
	CBasicButton m_IncBtn;
	vector<CBasicButton *> m_TabBtnList;
	vector<char *> m_ListExtraString;  // 탭 버튼과 연결되 동작등을 하기위해 스트링을 연결해둔다. 스트리응ㄹ 어떻게 스던지는 자요.

	void SetDisplay( CDisplay *pDis )
	{
		m_pDis = pDis;
	}
	CDisplay* m_pDis;
};

#endif // _BASIC_BUTTON_H_