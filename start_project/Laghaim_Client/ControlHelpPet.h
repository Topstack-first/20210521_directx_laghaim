#ifndef _CONTROL_HELPPET_ ///�굵��
#define _CONTROL_HELPPET_

#include "HelpScript.h"

#define HELP_USE_FONT_SIZE 12 // ��Ʈ ũ��
#define HELP_USE_LINE_HEIGHT 16 // ���� ũ��

#define HELPPET_TALKBOX_VIEWTERM 1000 // ��ǳ�� ���� �� �������µ����� ��޷��ִ� �ð�.
#define HELPPET_TALKBOX_CLOSETIME 5000 // ��ǳ�� �ڵ� ������ �ð�.

#define HELP_TALKBOX_PLUS_POSX -50 // ��ǳ���� ���� ������ ��ġ.
#define HELP_TALKBOX_PLUS_POSY -10 // ��ǳ���� ���� ������ ��ġ.

#define HELP_TALKBOX_XPLUS_SIZE 30 // ��ǳ���� ����� ���� ũ��.(���� ��ģ ũ��.)
#define HELP_TALKBOX_YPLUS_SIZE 20 // ��ǳ���� ����� ���� ũ��.(���� ��ģ ũ��.)

#define HELPPET_LIKE_HEIGHT 150 // ���� ���� �������� �ϴ� ����. �ٴڿ����� �Ÿ��ӿ� ����.

#define HELPPET_SEARCHRESULT_WIDTHSPACE 65 // �˻� ��� ǥ�ÿ��� ���ο� �ʿ��� ���� ũ��.
#define HELPPET_SEARCHRESULT_HEIGHTSPACE 55 // �˻� ��� ǥ�ÿ��� ���ο� �ʿ��� ���� ũ��.

#define HELPPET_SEARCHRESULT_HEIGHT 200 // �˻� ��� ǥ�� â ũ��. �׳� �Ƚ�.
#define HELPPET_SEARCHRESULT_CONTROLPOSY 140 // / ���� ������ ��ư �� ������ �� ǥ�� �� ��ġ��.

#define HELPPET_SEARCHRESULT_SEL_START_Y 40 // �������� ���۵Ǵ� ��ġ.
#define HELPPET_SEARCHRESULT_SEL_LINE_HEIGHT 20 // ������ �� ���� ũ��.


enum { HELPPET_TYPE_RABIYONG = 0, MAX_HELPPET_TYPE };
enum { HELPPET_ANI_NORMAL = 0, HELPPET_ANI_FIND, HELPPET_ANI_THROW, HELPPET_ANI_ANGRY, HELPPET_ANI_SLEEPY, HELPPET_ANI_SEARCH, MAX_HELPPET_ANI }; ///�굵��Ȯ��
#define HELPPET_FRAME_WIDTH 4 // �ִϸ��̼��� �� �̹����� �� ���ο� �̹��� ����.(�� ������ �޶����� �̰Ͱ� �ִϸ��̼� �� ������ �� ������ ���Ͻ�Ű��.)
//enum { HELPPET_STATE_NORMAL = 0, HELPPET_STATE_FIND, HELPPET_STATE_TALKING, HELPPET_STATE_STUDYTIME, MAX_HELPPET_STATE };


enum { HELPPET_INTERFACE_TALKBOX = 0, HELPPET_INTERFACE_CONTENTSBOX, HELPPET_INTERFACE_SEARCHBOX, HELPPET_INTERFACE_SEARCHMENT, MAX_HELPPET_INTERFACE }; // �������̽�.

#define HELPPET_SEARCHRESULT_VIEWINPAGE 5 // �ѹ��� ǥ�õǴ� �˻� ��� ��.


class IndexedTexture;
//struct sHelpData;
//class CHelpDataManager;

class CControlHelpPet // 2D�� �������̽��� �ƴ� 3D�� �������̽��� ����.(���� ��â�غ������� �׳� ���ǽ� �ƴ� ���ý��� �Ѵٴ� ��;;)
{
public:
	CControlHelpPet();
	virtual ~CControlHelpPet();

	void	Init();

	void	DeleteRes();
	void	LoadRes( int Type );
	HRESULT RestoreSurfaces();

//	void	SetPetState( int State ); // ���� ����.
	void	SetPetAnimation( int AniNum ); // �ִϸ��̼� ����
	void	MakeTextSurface( int HelpNumber ); // CMultiText �� ���ǽ��� ��ȯ���ֱ� ���� �༮.

	void    FrameMove();
	void    Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	BOOL	IsInsidePet( int x, int y );
	BOOL	IsInsideTalkBox( int x, int y );
	BOOL	IsInsideContents( int x, int y );
	int		IsInsideSearchBox( int x, int y ); // �������� ���õǾ��� ��� �������� ��ȣ�� ����. 0�̸� ���� ��, -1�̸� ������ �̿� ���� ��.

	int		CheckHelpCondition( int ConType, int Arg, BOOL bForced = FALSE ); // ��ϵ� �������� ������ ���鼭 �����ϴ� �༮�� �ִ��� üũ�Ѵ�.
	BOOL    ExcuteHelpTalk( int HelpIndex ); // ��縦 �ߵ� ��Ų��. �ε����� �˻��ؼ� �����ϴ� ����.
	BOOL    ExcuteHelpTalk( sHelpData *TempHelp ); // ��縦 �ߵ� ��Ų��. �����ͷ� �����ϴ� ����.
	BOOL	ExcuteHelpTalk( const char *strTalk ); // ���� ��縦 �Է��ϴ� ���...
	void    TerminateTalkBox(); // ��ǳ���� �����.
	BOOL    ExcuteHelpContents( int HelpIndex ); // �ڼ��� ���� �����ֱ⸦ �ߵ� ��Ų��.
	void	TerminateContents(); // �����ִ� ������ �κ��� �ʱ�ȭ ���ִ� �༮.

	BOOL HelpSearch( char *SearchWord ); // ���� �˻� ���.


	HWND			m_hWnd;
	RECT			m_ClientRc;
	HFONT m_Font;

	// �� �κ�
	int m_nType; // ���� ����. (���� ���� ������ ���� ó���� �� �ȵǾ������� ����.Ȥ�� ���߿� �߰��Ϸ��� �ϸ� ���� �����Ѵ�.)
	int m_nAnimation; // ���� �ִϸ��̼� ��ȣ.
	DWORD m_dwPetAniChageTick; // �� ���۰� ���õ� �κ��� ��� ƽ.
//	int m_nState; // ���� ���� ( �ִϸ��̼ǰ��� ����. �˻� ��, ���� �ߵ�, ������ ǥ����.��. )
//	DWORD m_dwStateChangeTick; // ���� ������ �� ƽ.
	int m_nDir; // ���� ����. 0�̸� ����, 1�̸� ������.
	int m_nMoveFlag; // �¿��̵� ������ �����ִ��� �÷���.
	int m_nSleepCount; // ���� ������ ������

	BOOL m_bPetDrag; // �� �����.
	int m_DragDistX; // �巡�׿� ����� ��ġ���� ��� ����.
	int m_DragDistY; // �巡�׿� ����� ��ġ���� ��� ����.
	int m_ClickX; // Ŭ���� ��ġ ���.
	int m_ClickY; // �����⿡ ����Ѵ�. ������ ���Ŀ� ��������� ���.
	int m_ThrowPower; // �����⿡ ���� ��.(������ ���� - �ȴ�.)

	DWORD m_dwPetDragTick; // �巡������ Ŭ������ üũ��.

	IndexedTexture*		m_pIndexedTexture; // �� �ִϸ��̼��� �����صα� ���� �༮.(�� �༮�� �� ���� �ڱ��ڽŲ��� ���̹Ƿ� ���϶��� �ε��ؼ� �����Ѵ�.)
	int m_AniIndex[MAX_HELPPET_ANI]; // �ִϸ��̼��� �ؽ��� �ε����� ����صд�.
	RECT		    	m_PetRect; // ���� ��ġ�� ũ��.
	RECT		    	m_TalksRect;
	int					m_TalkIndex; // ���� ������� ����� ���� �ε���.
	D3DTLVERTEX			m_TLVec[4]; // 3D �׷��� ǥ�ø� ���� ���ý�.

	int m_nNowFrame; // ���� �ִϸ��̼� ������.
	DWORD m_dwFrameRate; // �� ������ �Ѿ�� �� �ʿ��� ������ ����Ʈ. ƽ �����ӿ� ����.

	DWORD m_dwOldConCheckTick; // ���� ���� ���� �ߵ��� �˻��ߴ� ƽ.

	DWORD m_dwOldFrameTick; // ���� ƽ.

	CMultiText m_TalksText; // �̹��� ����ؾ��ϴ� ��� ����.

	// ���� ���� �κ�.
	CHelpDataManager m_HelpManager; // ���� ���� ����������
	CSurface *m_pTextSurface; // ��ǳ�� ��� ��¿�.

	DWORD m_dwTalkStartTick; // ��ǳ�� �ߵ� ƽ.
	int m_InterfaceTexIndex[MAX_HELPPET_INTERFACE]; // �ε��� �ؽ��Ŀ����� �ε���.
	CBasicButton	m_CloseBtn;     // �ݱ� ��ư



	// ���� ���� �������� �κ�.
	BOOL m_bContents;
	RECT m_ContentsRect;
	vector<CSurface *> m_ListContentsSurface;   // ���� ���� ���� �׸���. ������ ���, JPEG �� �ε��Ѵ�.
	int m_nNowContentsPage; // ���� �����ִ� ������ ������ ��ȣ.
	CBasicButton	m_PrevBtn2;		// �ּ�ȭ ��ư
	CBasicButton	m_NextBtn2;		// �ּ�ȭ ���¿��� �ٽ� ���� ��ư
	CBasicButton	m_CloseBtn2;     // �ݱ� ��ư
	BOOL m_bContentsDrag; // ��ġ�̵� �巡�� ���ΰ�.

	CTextOutBox     m_TextOutBox; // ���� ��¿�.

	int m_BoundCount; // ������ �� ƨ�� Ƚ�� ����. ///�굵��Ȯ��

	int m_Searching; // �˻� ������ ����. 0�̸� �˻��� �ƴ�, 1�̸� �˻��� �Է���, 2�̸� �˻� ��� ����޴� ///�굵��Ȯ��
	RECT		    	m_SearchRect;
	CTextBox m_SearchInput; // �˻��� �Է¿�.
	CBasicButton	m_SearchBtn;		// �˻� ��ư
	CMultiText2 m_SearchResultText; // 200 ���α��� �������ִ� �༮.
	vector<int> m_SearchResultIndex;
	int m_NowViewResult; // ���� �����ִ� �˻���� ��ġ. (������ ���� ������ �ʿ�)
	int m_TotalResult; // �˻� ��� ����.
	CBasicButton	m_PrevBtn;		// �ּ�ȭ ��ư
	CBasicButton	m_NextBtn;		// �ּ�ȭ ���¿��� �ٽ� ���� ��ư
	int m_NowSelResult; // ���� ���õ� �������� ��ȣ.(1���� ����)

//	LPDIRECTSOUNDBUFFER m_pNewMessage; // �� �޼������� ȿ����.
};


#endif