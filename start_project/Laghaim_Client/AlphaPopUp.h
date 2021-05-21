#ifndef _ALPHA_POPUP_H_
#define _ALPHA_POPUP_H_

#include "Config.h"
#include "dxutil.h"
#include "ddutil.h"
#include "sheet.h"

//==========================================
// nate 2004 - 4
// Crash_miss
#define CRASH_MISS_WIDTH	50
#define CRASH_MISS_HEIGHT	40
//==========================================

#define VERTEX_CNT	4
#define TEXT_COLOR			RGB(255, 255, 255)
#define TEXT_BACK_COLOR		0L
#define BOX_GAP		5		// ���ĺ����� ������濡 ���ڰ� �������� ����ϴ�..�ؼ�..�ణ�� ������ Ȯ���Ѵ�

class CSheet;
class IndexedTexture;

#define MAX_TEXT_LINE		32
#define MAX_TEXT_LINE2		200

class CMultiText
{
public:
	CMultiText();
	~CMultiText();

	void Clone(CMultiText& multiText);

	void Reset();
	BOOL AddString(char *string, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR);
	void AddStringMulti(char *string, int len, BOOL flag, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR);
	BOOL AddStringEx(char *string, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR); // ���� ���ڸ� �ν��ϴ� �༮. ///�����ۼ����Է�

	int m_LineCount;
	char *m_Text[MAX_TEXT_LINE];
	DWORD m_TextColor[MAX_TEXT_LINE];
	DWORD m_TextBackColor[MAX_TEXT_LINE];
};

class CMultiText2 // �� �� ������ ����(��Ӿ��� �� ������ �׳� �貸����.) ///�����˾�â
{
public:
	CMultiText2();
	~CMultiText2();

	void Reset();
	BOOL AddString(char *string, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR);
	void AddStringMulti(char *string, int len, BOOL flag, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR);
	BOOL AddStringEx(char *string, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR); // ���� ���ڸ� �ν��ϴ� �༮. ///�����ۼ����Է�

	int m_LineCount;
	char *m_Text[MAX_TEXT_LINE2];
	DWORD m_TextColor[MAX_TEXT_LINE2];
	DWORD m_TextBackColor[MAX_TEXT_LINE2];
};

enum AlphaPopUp_Type
{
	AlphaPopUp_Type__Default,
	AlphaPopUp_Type__Char,

	AlphaPopUp_Type__EndOfEnum
};

class CAlphaPopUp
{
protected:
	D3DTLVERTEX				TLVec[VERTEX_CNT];

	CSurface*		m_pTextSur;

	HFONT			m_Font;

	int				m_FontWidth, m_FontHeight;
	int				m_Xpos,		 m_Ypos;
	int				m_TextSurWidth, m_TextSurHeight;

	bool			m_MainActive;

	DWORD			m_TextColor;
	DWORD			m_TextBackColor;

	// �������̳�.. �� �׷��͵��� ����.. Index!
	int				m_Index;

	int				m_ScreenWidth, m_ScreenHeight;

	BOOL			m_bRectSoft;
	int				m_GemNum;



	AlphaPopUp_Type m_alpha_type;


public:
	CAlphaPopUp();
	~CAlphaPopUp();
	void Init(int FontSize,AlphaPopUp_Type type=AlphaPopUp_Type__Default);
	void DeleteRes();
	void LoadRes(int screenWidth, int screenHeight);

	// nate
	// �븸 �������
	void Draw( int nX, int nY, BYTE MedalFlag = 0 );

	void DrawImgGem(CSurface *m_pImgGem, int which_gem);
	void RenderOj( int CustomTextr = 0 );
	void PutString(CMultiText *pMultiText, BOOL bShadow = FALSE);

	void SetFont (int f_w, int f_h, char *fontname );
	HFONT GetFont()
	{
		return m_Font;    ///���������� �� �߰�.
	}
	void SetIndex (int index)
	{
		m_Index = index;
	}
	int  GetIndex()
	{
		return m_Index;
	}
	BOOL GetMainActive()
	{
		return m_MainActive;
	}
	void SetMainActive(bool Act)
	{
		m_MainActive = Act;
	}
	void CalcPos(int x, int y);
	int  GetYpos()
	{
		return m_Ypos;
	}
	int  GetXpos()
	{
		return m_Xpos;
	}

	void SetRectSoft(BOOL bRectSoft)
	{
		m_bRectSoft = bRectSoft;
	}
	void SetGemNum(int gem_num)
	{
		m_GemNum = gem_num;
	}
	int GetGemNum()
	{
		return m_GemNum;
	}

	// nate
	// �븸 ���� ����
	int GetSurWidth()
	{
		return m_TextSurWidth;
	}
	int GetSurHeight()
	{
		return m_TextSurHeight;
	}
	CSurface* GetSurface()
	{
		return m_pTextSur;    // ���� �̺�Ʈ �۾��� �����ؼ� �ʿ��ؼ� �߰�. 070828
	}

//========================================================
	// nate 2004 - 4
	// Crash_Miss
	void Draw( int nX, int nY, LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime );
	void RenderCrash_Miss( LPDIRECTDRAWSURFACE7 lpSurface = NULL, int nShowTime = 0 );
	void Draw2( int nX, int nY, LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime ); ///������� ����,���� ��¿�. �Ƕ��� �ð��̳� �������� �ٸ���.
	void RenderWinLose( LPDIRECTDRAWSURFACE7 lpSurface = NULL, int nShowTime = 0 ); ///������� ����,���� ��¿�. �Ƕ��� �ð��̳� �������� �ٸ���.
	void CalcPos_Crash_Miss( int x, int y );
//========================================================

	int m_nGuildNameXPos;
	int m_nGuildNameYPos;
	int GetGuildNameXPos()
	{
		return m_nGuildNameXPos;
	}
	int GetGuildNameYPos()
	{
		return m_nGuildNameYPos;
	}
	void MakeChatPopup(CMultiText *pMultiText, BOOL bShadow, int NameLine = 0);

	void SetAlphaPopup_Type(AlphaPopUp_Type alpha_type)
	{
		m_alpha_type = alpha_type;
	}
};

#endif // _ALPHA_POPUP_H_