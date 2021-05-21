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
#define BOX_GAP		5		// 알파블렌딩된 바탕배경에 글자가 가득차면 어색하다..해서..약간의 공간을 확보한다

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
	BOOL AddStringEx(char *string, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR); // 개행 문자를 인식하는 녀석. ///아이템설명입력

	int m_LineCount;
	char *m_Text[MAX_TEXT_LINE];
	DWORD m_TextColor[MAX_TEXT_LINE];
	DWORD m_TextBackColor[MAX_TEXT_LINE];
};

class CMultiText2 // 줄 수 더많은 버젼(상속쓰면 좀 꼬여서 그냥 배껴만듬.) ///공지팝업창
{
public:
	CMultiText2();
	~CMultiText2();

	void Reset();
	BOOL AddString(char *string, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR);
	void AddStringMulti(char *string, int len, BOOL flag, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR);
	BOOL AddStringEx(char *string, DWORD color = TEXT_COLOR, DWORD back_color = TEXT_BACK_COLOR); // 개행 문자를 인식하는 녀석. ///아이템설명입력

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

	// 아이템이나.. 딴 그런것들을 위한.. Index!
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
	// 대만 훈장관련
	void Draw( int nX, int nY, BYTE MedalFlag = 0 );

	void DrawImgGem(CSurface *m_pImgGem, int which_gem);
	void RenderOj( int CustomTextr = 0 );
	void PutString(CMultiText *pMultiText, BOOL bShadow = FALSE);

	void SetFont (int f_w, int f_h, char *fontname );
	HFONT GetFont()
	{
		return m_Font;    ///국가대항전 시 추가.
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
	// 대만 훈장 관련
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
		return m_pTextSur;    // 퀴즈 이벤트 작업과 관련해서 필요해서 추가. 070828
	}

//========================================================
	// nate 2004 - 4
	// Crash_Miss
	void Draw( int nX, int nY, LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime );
	void RenderCrash_Miss( LPDIRECTDRAWSURFACE7 lpSurface = NULL, int nShowTime = 0 );
	void Draw2( int nX, int nY, LPDIRECTDRAWSURFACE7 lpSurface, int nShowTime ); ///자유대련 승자,패자 출력용. 판때기 시간이나 움직임이 다르다.
	void RenderWinLose( LPDIRECTDRAWSURFACE7 lpSurface = NULL, int nShowTime = 0 ); ///자유대련 승자,패자 출력용. 판때기 시간이나 움직임이 다르다.
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