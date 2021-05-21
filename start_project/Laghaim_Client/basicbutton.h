#ifndef _BASIC_BUTTON_H_
#define _BASIC_BUTTON_H_

#include "Config.h"

#include <vector> ///������� �� �߰�
#include "ddutil.h"
using namespace std; ///������� �� �߰�

#define TAB_ALIGN_HORIZON     0 // ���� ���� ��� ///������� �� �߰�
#define TAB_ALIGN_VERTICAL    1 // ���� ���� ��� ///������� �� �߰�

class CBasicButton;
class CDirectSound;

class CBasicButton
{
public:
public:
	CSurface* m_surface;	// ��������Ʈ �̹���

	char m_bmpfile[80];

	RECT m_rc_button_flag;

public:
	int PosX;			// ȭ�鿡 ��ġ�� x ��ǥ
	int PosY;			// ȭ�鿡 ��ġ�� y ��ǥ
	int Width;			// ��ư �̹����� ���α���
	int Height;			// ��ư �̹����� ���α���

	int m_iBtnFlg;			// ��ư�� �������� ������ �ε���
	bool m_bDisable;		// ��ư �׸��� enable ���� disable ����
	bool bDown;				// ��ư�� ���ȴ��� �� ���ȴ���
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

class CTabControl ///������� �� �߰�
{
public:

	CTabControl();
	~CTabControl();

	void SetPosition( int x, int y, int w, int h  );
	void SetTabStyle( int Align, int DrawPosX, int DrawPosY, int Space ); // Align�� ����, ���� ���� ����.
	void SetIncDecBtn( char* DecFileName, int DecX, int DecY,  char* IncFileName, int IncX, int IncY );
	void Clear(); // �ǹ�ư ����Ʈ�� �� �����.
	BOOL AddTab( char* BtnFileName, char* ExString = NULL ); // ExString�� �� ��ư�� ����� ���۵��� �ϱ����� ��Ʈ���� �����صд�. ��Ʈ������ ��� �������� �ڿ�.
	char *GetExtraString( int Index ); // �����ص� ����Ʈ�� ��Ʈ���� �ҷ��������� �༮.
	void DeleteRes();
	void RestoreSurfaces();
	void Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	bool IsInside( int x, int y );
	int GetMouseOnIndex( int x, int y );

	int NowDrawStartIndex; // �׷����� �����ϴ� ��ȣ.(��ũ��ó�� �߰����� �׸����� �ִ�.)
	int NowSelectedIndex; // ���� ���õǾ��� ��.

	int m_TapAlign; // �������� �������� ����.(0�̸� ����, 1�̸� ����)
	int m_TabDrawPosX; // �� �׷����� �����ϴ� ��ǥ.(�� ��Ʈ���� ������ �ְ� ������ ��밡��)
	int m_TabDrawPosY; // �� �׷����� �����ϴ� ��ǥ.(�� ��Ʈ���� ������ �ְ� ������ ��밡��)
	int m_TabSpace; // �ǰ� �ǻ����� ���� ����.

	int m_TabCount; // ��ϵǾ��� ���� ����.

	int m_PosX;
	int m_PosY;
	int m_TabControlWidth;
	int m_TabControlHeight;
	CBasicButton m_DecBtn;
	CBasicButton m_IncBtn;
	vector<CBasicButton *> m_TabBtnList;
	vector<char *> m_ListExtraString;  // �� ��ư�� ����� ���۵��� �ϱ����� ��Ʈ���� �����صд�. ��Ʈ������ ��� �������� �ڿ�.

	void SetDisplay( CDisplay *pDis )
	{
		m_pDis = pDis;
	}
	CDisplay* m_pDis;
};

#endif // _BASIC_BUTTON_H_