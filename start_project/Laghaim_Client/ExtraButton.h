#ifndef _EXTRA_BUTTON_H_
#define _EXTRA_BUTTON_H_

#include "Config.h"

#include "basicbutton.h"

#define MAX_LOOP_BUTTON 10   // ���� Ÿ�� ��ư���� ���� �Ѱ�ġ.

// ������ �ȴ��� ���·� ��ȭ�ϴ� ��ư.
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


	BOOL ToggleState; // FALSE �̸� �� ����. TRUE�̸� ����.
};

// ���� �̹����� ������ ���鼭 ������ ��ư.
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

	int NumOfState; // ��ϵ� ��ư�� ����.
	int LoopState; // ���� ��ư�� ��ȣ.

	char m_arr_bmpfile[MAX_LOOP_BUTTON][80];
	CSurface* m_arr_surface[MAX_LOOP_BUTTON]; // �⺻ �̹���
};

#endif // _EXTRA_BUTTON_H_