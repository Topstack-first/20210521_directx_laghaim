#ifndef _COMBO_BOX_H_
#define _COMBO_BOX_H_

#include "Config.h"

#include "basicbutton.h" // ���������� �⺻ ��ư�� �ʿ�� �ϹǷ� ���.

#define MAX_COMBO_LIST 10 // ����Ʈ �ƽ�ġ. �����ϸ� �÷���.

#define COMBO_STATE_DISABLE -1 // ��Ȱ��.
#define COMBO_STATE_NORMAL 0 // �Ϲ�.
#define COMBO_STATE_SELECT 1 // ������.
#define COMBO_STATE_ANIMATE 2 // �ִϸ��̼� ��.(����)

// �޺��ڽ�
class CComboBox // �������� ������ ��Ʈ�� �Ǵ� ���� �̹��� �� �޺��ڽ��̴�. ��ũ���̶���� �ѿ��� ��ư ���� �ְ� ������ ����ؼ� ������!
{
public:
	CComboBox()
	{
//		m_pDisplay = NULL; // ���� ������.
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

	CComboBox( int Width, int Height, int X_Plus = 0, int Y_Plus = 0 ) // ����� �̸� �޴� ������. �Ϲ������� �������� ��������.(����� �����ϴ°� ����� �ʵ���...������ ������ �̹��� �ε� ���� �̷�������Ѵ�.)
	{
//		m_pDisplay = NULL; // ���� ������.
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

		m_State = 0; // 0�̸� ���� ����, -1�̸� ��Ȱ�� ����, 1�̸� ���ð��� ����, 2�̸� ������ ����.
		m_NowSelectIndex = 0; // ���� ���õǾ��ִ� ����Ʈ�� �ε���.
		m_NowOverIndex = 0; // ���콺�� �ö� �ִ� �������� �ε���.
		m_InsertedCount = 0;

		m_ItemWidth = 0; // ����Ʈ�� ������ ���� ����.(�� ����Ʈ�� ���ϵǾ� �־���Ѵ�.)
		m_ItemHeight = 0; // ����Ʈ�� ������ ���� ����.(�� ����Ʈ�� ���ϵǾ� �־���Ѵ�.)
		m_X_Draw_Plus = 0; // ����Ʈ ��½� x��ǥ ����ġ
		m_Y_Draw_Plus = 0; // ����Ʈ ��½� y��ǥ ����ġ

		m_DrawPosX = 0; // ���� ������ ��ǥ.
		m_DrawPosY = 0;

		m_AnimateHeight = 0.0f; // ���� �ִϸ��̼ǽÿ� ���̴� ���� ���� ����.
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

	void LoadRes( char *ListViewBtnName = NULL, char *SelectedBackGroundName = NULL, char *ListBackGroundName = NULL ); // ������ ������ �̹��� �ε� ���� �̷�������Ѵ�.
	void DeleteRes();

	int InsertItem( char* ItemText, int FontSize = 12, DWORD TextColor = RGB(200, 200, 200), DWORD OverColor = RGB(50, 50, 200), DWORD BackColor = RGB(0, 0, 0) );
	int InsertItemByImage( char* ImageFileName ); // �̹����� ����.

	void DeleteAllItem(); // ����� ���� ����� �Լ��� ����. ������ ����� �ͱ��� ���� �ð��� ����! ���߿� �ʿ��Ҷ� �߰�����.

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

	void SetItemSize( int Width, int Height, int X_Plus = 0, int Y_Plus = 0 ) // ������ �ϳ��� ����,����, ��½� x��ǥ ����ġ, ��½� y��ǥ ����ġ. // ������ ������ �̹��� �ε� ���� �̷�������Ѵ�.
	{
		m_ItemWidth = Width; // ����Ʈ�� ������ ���� ����.(�� ����Ʈ�� ���ϵǾ� �־���Ѵ�.)
		m_ItemHeight = Height; // ����Ʈ�� ������ ���� ����.(�� ����Ʈ�� ���ϵǾ� �־���Ѵ�.)
		m_X_Draw_Plus = X_Plus; // ����Ʈ ��½� x��ǥ ����ġ
		m_Y_Draw_Plus = Y_Plus; // ����Ʈ ��½� y��ǥ ����ġ
	}

	void SetPosition( int x, int y )
	{
		m_DrawPosX = x;
		m_DrawPosY = y;
	}

	int IsInsideList( int, int ); // ����Ʈ Ŭ����...����� Ŭ���ߴ����� ����(0�̼��ý� 0����, �迭�󿡼� 0�� ����Ʈ �������̹Ƿ� ����Ʈ���� �������� �ʴ´�.)

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


	CSurface	*m_pSelectedBackGround; // ���õǾ��ִ� �༮�� �����ִ� ��ġ�� ���.
	CSurface	*m_pListBackGround; // ����Ʈ���� ���. �׳� ������ ������ �ڽ��� ���� ����.
	CBasicButton	*m_pListViewBtn; // ����Ʈ�� �����ֱ� ���� ��ư. ��Ȱ�� �����϶� �� ������.
	CSurface	*m_pListItem[MAX_COMBO_LIST]; // ����Ʈ ��� (��Ʈ�� �Ǵ� �̹����� ����� �� �ִ�.) // 0���� ����Ʈ ���������� ���� ���۽� ����� ��Ʈ��(�Ǵ� �̹���)�ӿ� ����.
	CSurface	*m_pListOverItem[MAX_COMBO_LIST]; // ���콺 �����ÿ� �������� ���ǽ�.               // �������� ����Ʈ �������� 1������ ���۵ȴٰ� ����ȴ�.

	int m_State; // 0�̸� ���� ����, -1�̸� ��Ȱ�� ����, 1�̸� ���ð��� ����, 2�̸� ������ ����.
	int m_NowSelectIndex; // ���� ���õǾ��ִ� ����Ʈ�� �ε���.
	int m_NowOverIndex; // ���콺�� �ö� �ִ� �������� �ε���.
	int m_InsertedCount; // �Էµ� ����Ʈ ������ ����.

	int m_DrawPosX; // ���� ������ ��ǥ.
	int m_DrawPosY;

	int m_ItemWidth; // ����Ʈ�� ������ ���� ����.(�� ����Ʈ�� ���ϵǾ� �־���Ѵ�.)
	int m_ItemHeight; // ����Ʈ�� ������ ���� ����.(�� ����Ʈ�� ���ϵǾ� �־���Ѵ�.)
	int m_X_Draw_Plus; // ����Ʈ ��½� x��ǥ ����ġ(����Ʈ�� ����ġ����. ����Ʈ ����� ����ġ�� �ƴԿ� ����! ������ �׵θ� ũ���� �����ϸ� �� ��.)
	int m_Y_Draw_Plus; // ����Ʈ ��½� y��ǥ ����ġ(����Ʈ�� ����ġ����. ����Ʈ ����� ����ġ�� �ƴԿ� ����! ������ �׵θ� ũ���� �����ϸ� �� ��.)

	float m_AnimateHeight; // ���� �ִϸ��̼ǽÿ� ���̴� ���� ���� ����.
	DWORD m_dwElapsedTick; // ���� �ִϸ��̼ǽÿ� ���̴� �ð�����. �޺��ڽ����� �ϳ��� �ʿ��ѵ�.(�Լ����� static���� �ΰ��� ���� �۵��� �� �ȵǴ���.)

	int m_nBtnPosX;
	int m_nBtnPosY;
};

#endif // _COMBO_BOX_H_