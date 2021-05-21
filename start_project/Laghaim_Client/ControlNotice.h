#ifndef _CONTROL_NOTICE_H_ ///�����˾�â
#define _CONTROL_NOTICE_H_

#include "Config.h"

#include <dsound.h>

class CSurface;
class CTextOutBox;
class CBasicButton;
class CScroll;
class CMultiText;

#define POPUP_NOTICE_TEXTSUR_H_SPACE 45 // �ؽ�Ʈ ���ǽ��� ���Ʒ� ������ ��.
#define POPUP_NOTICE_TEXTLINE_HEIGHT 14 // �ؽ�Ʈ ���� ������ ����.(��Ʈ ����)

#define POPUP_NOTICE_TITLEBAR_SIZE_Y 28 // Ÿ��Ʋ ���� ����.(����)


#define POPUP_NOTICE_MINIMIZE_SIZE_X 65
#define POPUP_NOTICE_MINIMIZE_SIZE_Y 30

#define POPUP_NOTICE_SIZECONTROL_SIZE_X 20
#define POPUP_NOTICE_SIZECONTROL_SIZE_Y 20

#define MAX_CONTANTS_LINE 100 // ���� ���ִ� �ټ�.

enum { NOTICE_TYPE_NORMAL = 0, NOTICE_TYPE_MOBIUS };
enum { NOTICE_STATE_FULL = 0, NOTICE_STATE_MINI };

// ��� ���� �˾�â. ������ �ܼ� �ؽ�Ʈ ������ �ƴ϶� �������̽��� �ߴ� �����̴�.
class CControlNotice // 2D�� �������̽��� �ƴ� 3D�� �������̽��� ����.(���� ��â�غ������� �׳� ���ǽ� �ƴ� ���ý��� �Ѵٴ� ��;;)
{
public:
	CControlNotice();
	virtual ~CControlNotice();

	void	Init();

	void	DeleteRes();
	void	LoadRes();
	HRESULT RestoreSurfaces();

	void	SetType( int Type ); // Ÿ��Ʋ ����.
	void	SetContents( char *String ); // ��Ʈ�� ����.
	void	MakeTextSurface(); // CMultiText �� ���ǽ��� ��ȯ���ֱ� ���� �༮.

	void    Draw(); // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
	void    RenderOj(int xpos, int ypos, int cx, int cy); // 3D�� �׸���.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInside( int x, int y );
	BOOL	IsTitleBarInside( int x, int y ); // ������ ���� Ÿ��Ʋ ��.
	BOOL	IsSizeControlInside( int x, int y ); // ������ ������ ���� �����ڸ��� Ŭ���ߴ°�.

	void WindowChange( int x, int y, int size_right, int size_bottom ); // â ��ġ�� ����� �ٲ� ������ϴ� �͵�


	HWND			m_hWnd;
	RECT			m_ClientRc;

	int m_nType; // ���� �����ΰ�.(�̰Ϳ� ���� Ÿ��Ʋ ����� �ٲ��.)
	int m_PosX; // â�� ��ġ
	int m_PosY; // â�� ��ġ
	int m_nState; // �������̽��� ���� ( 0 : ���� ����, 1 : �ּ�ȭ ���� )
	BOOL m_bMoveDrag; // ��ġ�̵� �巡�� ���ΰ�.
	BOOL m_bSizeDrag; // ������ ���� �巡�� ���ΰ�.
	BOOL m_bNewMessage; // ���ο� �޼����� ���ִ���. (��������)
	RECT m_SizeRect; // ������ ���� �����ϹǷ� �̰ɷ� ���.
	int m_ViewLineNum; // ���� ������� �ѹ��� �������� �ؽ�Ʈ �� ��.
	int m_TotalLineNum; // ���� ������� �������� �ؽ�Ʈ �� �� ��.

	D3DTLVERTEX			m_TLVec[4]; // â ǥ�ø� ���� ���ý�.

	CSurface *m_pTitleBar;     // â Ÿ��Ʋ �κ��� ���� ���.(���ý� ������ ������ ������ �� �������ֵ���...
	CSurface *m_pTitleBarLight;     // â Ÿ��Ʋ �κ��� ���� ���.(���ý� ������ ������ ������ �� �������ֵ���...
	CSurface *m_pTitle;     // â Ÿ��Ʋ �κ��� ���� ���.(���ý� ������ ������ ������ �� �������ֵ���...

	CMultiText2 m_Contents; // ǥ�� ����.

	CSurface *m_pContentsSurface; // ǥ�� ������ �ؽ�Ʈ�� ���ǽ��� ��ȯ�س��� ��. ������ �����ø��� �ٽ� ����� ����Ѵ�.

	CScroll m_Scroll; // �ؽ�Ʈ ��ũ�� ��.

	CBasicButton	m_MiniBtn;		// �ּ�ȭ ��ư
	CBasicButton	m_MaxBtn;		// �ּ�ȭ ���¿��� �ٽ� ���� ��ư
	CBasicButton	m_CloseBtn;     // �ݱ� ��ư

	CSurface *m_pSizeControl;     // ������ ������ ���� Ŭ���� �̹���


	LPDIRECTSOUNDBUFFER m_pNewMessage; // �� �޼������� ȿ����.
};

#endif // _CONTROL_NOTICE_H_