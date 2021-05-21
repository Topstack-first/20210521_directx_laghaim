#ifndef SCROLL_H
#define SCROLL_H

#include "Config.h"

#include "basicbutton.h"

#include "d3dutil.h"

///////////////////////////////////////////////////////////////////////////////////////
// 작성자 : 이원석
// 내  용 : 순수하게 스크롤만 관리해주는 클래스.
class CScroll
{
public:

	CScroll()
	{
		ScrollBarSurface = NULL;
		m_DecBtn = NULL;
		m_IncBtn = NULL;

		Init();
	}

	~CScroll()
	{
		Init();
	}

	void Init()
	{
		DrawPosX = 0; // 실제 찍히는 좌표.
		DrawPosY = 0;
		DrawWidth = 0; // 읽어온 그림에 의해 결정됨에 유의
		DrawHeight = 0; // 세로 길이. 직접 셋팅가능.

		BarHeight = 0;

		NowPos = 0; // 현재 가리키는 스크롤 리스트에서의 위치.
		RangeMax = 1; // 최대치.

		MoveGap = 12;

		m_bDraging = FALSE;

		IsUseWheelArea = false;
		memset(&WheelArea, 0, sizeof(WheelArea));

		SAFE_DELETE( m_DecBtn );
		SAFE_DELETE( m_IncBtn );
		SAFE_DELETE( ScrollBarSurface );
	}

	void LoadRes( char *DecBtn = NULL, char *IncBtn = NULL, char *ScrollBar = NULL );
	void DeleteRes(); ///파티개편 시 추가.

	void SetRange( int Max ) // 0부터 시작하는 범위를 셋팅한다. (즉, 200개 쓰고 싶으면 (200-1)을 입력해야함에 유의!)
	{
		RangeMax = Max;
	}

	void SetDrawPos( int x, int y, int Height ) // 가로 길이는 그림에 자동으로 맞추자.
	{
		DrawPosX = x; // 실제 찍히는 좌표.
		DrawPosY = y;

		DrawHeight = Height; // 세로 길이. 직접 셋팅가능.
	}

	int GetPos()
	{
		return NowPos;    // 현재 위치를 알아낸다.
	}
	int GetInvPos()
	{
		return RangeMax - NowPos;    // 거꾸로 세는 스크롤은 이녀석을 쓴다.
	}
	void SetPos( int Pos )
	{
		if( Pos > RangeMax -1 )
		{
			Pos = RangeMax -1;
		}

		NowPos = Pos;
	} // 현재 위치를 조정.

	void IncPos();
	void DecPos();

	int CalculateBarYPos(); // Scroll Bar의 위치를 계산한다.
	int CalcPosFromY( int y ); // y 값으로 부터 Pos값을 산출한다.( y는 절대 좌표 )

	bool IsInside( int, int ); // 영역 전체 체크용.
	bool IsInsideScrollArea( int, int ); // 점프 체크용
	bool IsInsideScrollBar( int, int ); // 드래그 체크용.

	void Draw();

	void SetWheelArea(int x, int y, int width, int height);
	void SetUseWheelArea(bool isUse);
	bool IsInsideWheelArea(int x, int y);

	// Message 처리
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	CSurface	*ScrollBarSurface;
	CBasicButton	*m_DecBtn;
	CBasicButton	*m_IncBtn;

	int DrawPosX; // 실제 찍히는 좌표.
	int DrawPosY;
	int DrawWidth; // 읽어온 그림에 의해 결정됨에 유의
	int DrawHeight; // 세로 길이. 직접 셋팅가능.

	int BarHeight; // 변화되는 바의 길이.

	int NowPos; // 현재 가리키는 스크롤 리스트에서의 위치.
	int RangeMax; // 최대치.

	BOOL m_bDraging; // 드래그 중인가의 체크.

	int MoveGap;

	RECT WheelArea;
	bool IsUseWheelArea;
};

#endif // SCROLL_H