#ifndef _BASIC_MENU_H_
#define _BASIC_MENU_H_

#include "Config.h"
#include "LHUI_Base.h"
extern float locationX, locationZ;
#define MENU_FILE_NAME_LEN 256


// ��ϵ� �޴� Ÿ�� ����Ʈ(�޴��� ���µɶ� ���п�.)
#define MENU_TYPE_USERSHOP			1 // ���λ��� 
#define MENU_TYPE_NPCUPGRADE		2 // ��ȭ,����,������ ��...
#define MENU_TYPE_BPSHOP			3 // BP ����
#define MENU_TYPE_ENCHANT			4 // [2/27/2007 Theodoric] ��þƮ : �÷�Ƽ�� ����
#define MENU_TYPE_LPSHOP			5 // LP ����
#define MENU_TYPE_NEW_NPCUPGRADE	6
#define MENU_TYEP_ITEMMAKE			7 // ������ ���� ����
#define MENU_TYPE_BUFFMAKE			8

// �׼� ����Ʈ.(��ư���� ��ϵǴ� ���� �ൿ ��ȣ�̴�.)
#define MENU_ACTION_CLOSE -1            // ����
#define MENU_ACTION_USERSHOP_SEARCH			1 // MENU_TYPE_USERSHOP
#define MENU_ACTION_USERSHOP_RECEIVE		2 // MENU_TYPE_USERSHOP
#define MENU_ACTION_NPCUPGRADE_UPGRADE		3 // MENU_TYPE_NPCUPGRADE
#define MENU_ACTION_NPCUPGRADE_CONVERSION	4 // MENU_TYPE_NPCUPGRADE
#define MENU_ACTION_NPCUPGRADE_CONFUSION	5 // MENU_TYPE_NPCUPGRADE
#define MENU_ACTION_NPCUPGRADE_REFORM		6 // MENU_TYPE_NPCUPGRADE

#define MENU_ACTION_ENCHANT_UPGRADE			7 // [2/27/2007 Theodoric] ��þƮ : MENU_TYPE_ENCHANT ��ȭ
#define MENU_ACTION_ENCHANT_MAKE 			8 // [2/27/2007 Theodoric] ��þƮ : MENU_TYPE_ENCHANT �÷�Ƽ�� ����
#define MENU_ACTION_ENCHANT_INIT			9 // [2/27/2007 Theodoric] ��þƮ : MENU_TYPE_ENCHANT �ʱ�ȭ

#define MENU_ACTION_NEW_NPCUPGRADE_UPGRADE		10	// [2008/11/28 Theodoric] 300���� �̻��� ���� ��ȭ
#define MENU_ACTION_NEW_NPCUPGRADE_CONVERSION	11	// [2008/11/28 Theodoric] 300���� �̻��� ���� ������
#define MENU_ACTION_NEW_NPCUPGRADE_EXCHANGE		12	// [2008/11/28 Theodoric] 300���� �̻��� ���� ��ȯ

#define MENU_ACTION_ITEMMAKE					13
#define MENU_ACTION_ITEMSEPARATE				14
#define MENU_ACTION_GMAGICSTONE					15
#define MENU_ACTION_GODWEAPON_UP				16

#define MENU_ACTION_NPCUPGRADE_ARMOR400			17	// 400���� �̻��� �� ��ȭ

#define MENU_ACTION_BUFFMAKE_MAKE				18
#define MENU_ACTION_BUFFMAKE_GATCHA				19
#define MENU_ACTION_BUFFMAKE_MEDAL				20

struct sMenuSurface
{
	char FileName[MENU_FILE_NAME_LEN]; // �������� ���ϸ�.

	int x, y; // ��� ��ǥ.
	int Width,Height; // ����.

	CSurface *m_pSurface;     // ���� �̹���.
};

struct sMenuButton // �̰� ��� ��ǥ���� �ʿ�.
{
	int ActionIndex; // �������� �۵��� �׼� �ε���.
	int x, y; // ��� ��ǥ.

	CBasicButton m_Button;     // ���� ��ư.
};

struct sMenuString
{
	int x, y; // ��� ��ǥ.

	char String[MENU_FILE_NAME_LEN]; // �ϴ� 256������...
};

class CMultiScrollText // ��Ƽ�ؽ�Ʈ�� ��ũ�� ����� �޾� ��½����ִ� Ŭ����.
{
public:
	CMultiText m_MultiText;
	CSurface*		m_pTextSur; // ����� ���ǽ�.

	CScroll *m_pScroll;

	RECT m_ViewRect; // ����� ũ��.
	int m_ViewHeightSize;  // ����� ũ��.(�������� ���� ����)

	HFONT			m_Font;

	int				m_FontWidth, m_FontHeight;
	int				m_Xpos,		 m_Ypos;
	int				m_TextSurWidth, m_TextSurHeight;

	char m_DecBtnName[MENU_FILE_NAME_LEN]; // ��ũ�� �� ��ư ��� // ��ü ������� ��.
	char m_IncBtnName[MENU_FILE_NAME_LEN]; // ��ũ�� �ٿ� ��ư ���
	char m_BarName[MENU_FILE_NAME_LEN]; // ��ũ�� �� ���

	CMultiScrollText();
	~CMultiScrollText();
	void Init( int FontSize = 12 );
	void DeleteRes();
	void LoadRes( int l, int t, int r,int b );

	void SetViewRect( int l, int t, int r,int b );
	void SetFont (int f_w, int f_h, char *fontname ); // ��Ʈ�� �ٲٰ� ������ ���.
	void SetScrollImage( char *DecBtn = NULL, char *IncBtn = NULL, char *ScrollBar = NULL ); // ��ũ����  �����ϰ� ������ ���.
	void SetScrollPos( int x = -1, int y = -1, int size = -1 ); // ��ũ�� ��ġ�� ũ�⸦ �����ϰ� ������ ���.

	void AddString( char *String, int LineLen = -1, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR  ); // ��Ʈ�� ���� �߰�. �׳� ��Ƽ�ؽ�Ʈ�� �ֱ� ���� ���ֱ� ��.

	void LoadString( char *FileName ); // �ؽ�Ʈ ���Ϸ� ���� ��Ʈ���� �о���� �Լ�.

	void MakeSurface(); // ��Ƽ �ؽ�Ʈ�� ������ ���ǽ��� ������ִ� �༮.

	void Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // ��ũ�ѿ�.
};



struct sMenuMultiScrollText
{
	RECT OutRect; // ��� ��ǥ.

	int ScrollPosX, ScrollPosY, ScrollHeight;

	char DecBtnName[MENU_FILE_NAME_LEN]; // ��ũ�� �� ��ư ���
	char IncBtnName[MENU_FILE_NAME_LEN]; // ��ũ�� �ٿ� ��ư ���
	char BarName[MENU_FILE_NAME_LEN]; // ��ũ�� �� ���

//	int FontWidth, int FontHeight, char FontName[MENU_FILE_NAME_LEN]; // ��Ʈ ����. // ��Ʈ ������ ���� �����ϵ��� ����.

	CMultiScrollText ScrollText;
};


// ���,��Ʈ��,��ư���� ���Ƿ� ������ �� �ִ� ����� ���� �޴��� ���� �������̽�.
class CControlMenu : public LHUI_Base
{
public:

	CControlMenu();
	~CControlMenu();

	void	Init();
	void	DeleteRes();
	HRESULT RestoreSurfaces();
	void	LoadRes(); // �� ������� Add�� ����� �Ŀ� ȣ��Ǿ�� �ҵ�.
	void ClearList();

	void AddBackGround( const char *FileName, int PosX, int PosY, int Width, int Height ); // ��� ����. ��ġ�� -1�̸� �ڵ� �߾� ��ġ��.
	void AddSurface( const char *FileName, int PosX, int PosY, int Width = -1, int Height = -1 ); // �̳༮�� �ʿ信 ���� ���� ���θ� �Է��صξ���Ѵ�.
	void AddButton( const char *FileName, int PosX, int PosY, int ActionIndex );
	void AddString( const char *String, int PosX, int PosY, BOOL bCenter = FALSE );
	CMultiScrollText* AddScrollText( int PosX, int PosY, int Width, int Height, int ScrollPosX = -1, int ScrollPosY = -1, int ScrollHeight = -1, const char *DecBtn = NULL, const char *IncBtn = NULL, const char *Bar = NULL /*, int f_w = 6, int f_h = 12, char *fontname = NULL*/ );

	void	Draw(); // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void BtnAction( int ActionIndex ); // ��ư�� �������� ������ �ڵ�.

	BOOL	IsInside( int x, int y );


	HWND			m_hWnd;
	RECT			m_ClientRc;

	CTextOutBox m_TextOut; // �ؽ�Ʈ ��¿�.(�ϳ��� ������ ��½ÿ� ������)(���� ���ϸ� ��Ʈ�� �ϳ��ϳ��� �ƴ϶� ����� ���� ���۰��� ����)

	sMenuSurface m_BackSur; // BackGround �̹���.
	list<sMenuSurface *> m_SurfaceList; // �̹��� ����Ʈ.
	list<sMenuButton *> m_BtnList; // ��ư ����Ʈ.
	list<sMenuString *> m_StringList; // ��Ʈ��.
	vector<sMenuMultiScrollText *> m_ScrollTextList; // ��ũ�� ��Ʈ��. // �̳༮�� list���� ������ ���� ���� vector�� �ٲ���.
};


//////////////////////////////////////////////////////////////////////////


class CMultiScrollText2 // ��Ƽ�ؽ�Ʈ�� ��ũ�� ����� �޾� ��½����ִ� Ŭ����.
{
public:
	CMultiText2		m_MultiText;
	CSurface*		m_pTextSur; // ����� ���ǽ�.

	CScroll *m_pScroll;

	RECT m_ViewRect; // ����� ũ��.
	int m_ViewHeightSize;  // ����� ũ��.(�������� ���� ����)

	HFONT			m_Font;

	int				m_FontWidth, m_FontHeight;
	int				m_Xpos,		 m_Ypos;
	int				m_TextSurWidth, m_TextSurHeight;

	char m_DecBtnName[MENU_FILE_NAME_LEN]; // ��ũ�� �� ��ư ��� // ��ü ������� ��.
	char m_IncBtnName[MENU_FILE_NAME_LEN]; // ��ũ�� �ٿ� ��ư ���
	char m_BarName[MENU_FILE_NAME_LEN]; // ��ũ�� �� ���

	CMultiScrollText2();
	~CMultiScrollText2();
	void Init( int FontSize = 12 );
	void DeleteRes();
	void LoadRes( int l, int t, int r,int b );

	void SetViewRect( int l, int t, int r,int b );
	void SetFont (int f_w, int f_h, char *fontname ); // ��Ʈ�� �ٲٰ� ������ ���.
	void SetScrollImage( char *DecBtn = NULL, char *IncBtn = NULL, char *ScrollBar = NULL ); // ��ũ����  �����ϰ� ������ ���.
	void SetScrollPos( int x = -1, int y = -1, int size = -1 ); // ��ũ�� ��ġ�� ũ�⸦ �����ϰ� ������ ���.

	void AddString( char *String, int LineLen = -1, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR  ); // ��Ʈ�� ���� �߰�. �׳� ��Ƽ�ؽ�Ʈ�� �ֱ� ���� ���ֱ� ��.

	void LoadString( char *FileName ); // �ؽ�Ʈ ���Ϸ� ���� ��Ʈ���� �о���� �Լ�.

	void MakeSurface(); // ��Ƽ �ؽ�Ʈ�� ������ ���ǽ��� ������ִ� �༮.

	void Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ); // ��ũ�ѿ�.
};

//////////////////////////////////////////////////////////////////////////


#endif // _BASIC_MENU_H_