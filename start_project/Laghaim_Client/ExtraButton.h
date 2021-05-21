#ifndef _EXTRA_BUTTON_H_
#define _EXTRA_BUTTON_H_

#include "Config.h"

#include "basicbutton.h"

#define MAX_LOOP_BUTTON 10   // 루프 타입 버튼에서 종류 한계치.

// 눌림과 안눌림 상태로 변화하는 버튼.
class CToggleButton:public CBasicButton
{
public:
	CToggleButton()
	{
		ToggleState = FALSE;
	}

	virtual ~CToggleButton()
	{
		DeleteRes();
	}

	BOOL GetToggleState()
	{
		return ToggleState;
	}

	void SetToggleState( BOOL State )
	{
		ToggleState = State;
	}

	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);

	void Draw();
	void DrawToBlt( BOOL bChecked = FALSE );


	BOOL ToggleState; // FALSE 이면 안 눌림. TRUE이면 눌림.
};

// 여러 이미지가 번갈아 가면서 나오는 버튼.
class CLoopButton:public CBasicButton
{
public:
	CLoopButton()
	{
		NumOfState = 0;
		LoopState = 0;

		ZeroMemory( m_arr_bmpfile, sizeof(m_arr_bmpfile) );

		for( int i = 0 ; i < MAX_LOOP_BUTTON ; ++i )
			m_arr_surface[i] = NULL;
	}

	~CLoopButton()
	{
	}

	int GetLoopState()
	{
		return LoopState;
	}

	void SetLoopState( int State )
	{
		LoopState = State;

		if( LoopState < 0 )
		{
			LoopState = 0;
		}
		if( LoopState > NumOfState-1 )
		{
			LoopState = NumOfState-1;
		}

		m_surface = m_arr_surface[LoopState];
	}

	void SetFileName( char* );
	void DeleteRes();
	void LoadRes( int w = -1, int h = -1 );

	int MsgProc( HWND, UINT, WPARAM, LPARAM );

	int NumOfState; // 등록된 버튼의 갯수.
	int LoopState; // 현재 버튼의 번호.

	char m_arr_bmpfile[MAX_LOOP_BUTTON][80];
	CSurface* m_arr_surface[MAX_LOOP_BUTTON]; // 기본 이미지
};

#endif // _EXTRA_BUTTON_H_