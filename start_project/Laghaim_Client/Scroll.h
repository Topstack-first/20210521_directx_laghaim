#ifndef SCROLL_H
#define SCROLL_H

#include "Config.h"

#include "basicbutton.h"

#include "d3dutil.h"

///////////////////////////////////////////////////////////////////////////////////////
// �ۼ��� : �̿���
// ��  �� : �����ϰ� ��ũ�Ѹ� �������ִ� Ŭ����.
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
		DrawPosX = 0; // ���� ������ ��ǥ.
		DrawPosY = 0;
		DrawWidth = 0; // �о�� �׸��� ���� �����ʿ� ����
		DrawHeight = 0; // ���� ����. ���� ���ð���.

		BarHeight = 0;

		NowPos = 0; // ���� ����Ű�� ��ũ�� ����Ʈ������ ��ġ.
		RangeMax = 1; // �ִ�ġ.

		MoveGap = 12;

		m_bDraging = FALSE;

		IsUseWheelArea = false;
		memset(&WheelArea, 0, sizeof(WheelArea));

		SAFE_DELETE( m_DecBtn );
		SAFE_DELETE( m_IncBtn );
		SAFE_DELETE( ScrollBarSurface );
	}

	void LoadRes( char *DecBtn = NULL, char *IncBtn = NULL, char *ScrollBar = NULL );
	void DeleteRes(); ///��Ƽ���� �� �߰�.

	void SetRange( int Max ) // 0���� �����ϴ� ������ �����Ѵ�. (��, 200�� ���� ������ (200-1)�� �Է��ؾ��Կ� ����!)
	{
		RangeMax = Max;
	}

	void SetDrawPos( int x, int y, int Height ) // ���� ���̴� �׸��� �ڵ����� ������.
	{
		DrawPosX = x; // ���� ������ ��ǥ.
		DrawPosY = y;

		DrawHeight = Height; // ���� ����. ���� ���ð���.
	}

	int GetPos()
	{
		return NowPos;    // ���� ��ġ�� �˾Ƴ���.
	}
	int GetInvPos()
	{
		return RangeMax - NowPos;    // �Ųٷ� ���� ��ũ���� �̳༮�� ����.
	}
	void SetPos( int Pos )
	{
		if( Pos > RangeMax -1 )
		{
			Pos = RangeMax -1;
		}

		NowPos = Pos;
	} // ���� ��ġ�� ����.

	void IncPos();
	void DecPos();

	int CalculateBarYPos(); // Scroll Bar�� ��ġ�� ����Ѵ�.
	int CalcPosFromY( int y ); // y ������ ���� Pos���� �����Ѵ�.( y�� ���� ��ǥ )

	bool IsInside( int, int ); // ���� ��ü üũ��.
	bool IsInsideScrollArea( int, int ); // ���� üũ��
	bool IsInsideScrollBar( int, int ); // �巡�� üũ��.

	void Draw();

	void SetWheelArea(int x, int y, int width, int height);
	void SetUseWheelArea(bool isUse);
	bool IsInsideWheelArea(int x, int y);

	// Message ó��
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	CSurface	*ScrollBarSurface;
	CBasicButton	*m_DecBtn;
	CBasicButton	*m_IncBtn;

	int DrawPosX; // ���� ������ ��ǥ.
	int DrawPosY;
	int DrawWidth; // �о�� �׸��� ���� �����ʿ� ����
	int DrawHeight; // ���� ����. ���� ���ð���.

	int BarHeight; // ��ȭ�Ǵ� ���� ����.

	int NowPos; // ���� ����Ű�� ��ũ�� ����Ʈ������ ��ġ.
	int RangeMax; // �ִ�ġ.

	BOOL m_bDraging; // �巡�� ���ΰ��� üũ.

	int MoveGap;

	RECT WheelArea;
	bool IsUseWheelArea;
};

#endif // SCROLL_H