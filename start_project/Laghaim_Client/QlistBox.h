#ifndef _QLIST_BOX_H_
#define _QLIST_BOX_H_

#include "Config.h"



#include "headers.h"

#include "Scroll.h"

class CSurface;

#define MAX_CHAT_STRING_LENGTH 300 // 채팅 글자수 제한.(원석이 추가.)
#define MAX_Q	300   // 실제 저장되는 버퍼의 갯수.(스트링을 기억하는 줄 수)
//#define MAX_Q	30   // 실제 저장되는 버퍼의 갯수.(스트링을 기억하는 줄 수)
#define MAX_DRAW_Q 25 // 출력하는 서피스 버퍼의 한계.(출력하는 줄 수 최대치라고 보면 된다.)
#define MAX_TEXT_LEN	70
//#define MAX_LINE  20

struct QlistBox_ExtraInfo
{
	int nType;		// 0 : none, 1 : item, 2 : npc
	int nVnum;		// item 또는 npc 의 Vnum
	int nStrLen;	// 스트링의 길이
};

class CQlistBox
{

public :

	int		m_PosX, m_PosY;		// Interface 객체가 놓여질 위치
	int		m_Width, m_Height;	// Interface 객체의 가로/세로 길이
	int		m_ImgWidth, m_BtnHeight, m_SbarHeight;

	int		m_Head;
	int		m_ViewCount;
	int		m_Tem;  // 각 리스트박스간의 간격을 조절한다(

	CSurface	*m_DecBtnSurf;			// Decrement Button Surface
	CSurface	*m_IncBtnSurf;			// Increment Button Surface
	CSurface	*m_SbarSurf;		// Scroll Bar Surface

	char		m_TextQ[MAX_Q][MAX_CHAT_STRING_LENGTH];		// ListBox content Text
	COLORREF	m_TextColor[MAX_Q];
	char		m_OutputText[MAX_Q][MAX_CHAT_STRING_LENGTH]; // 출력용 버퍼.(옵션에따른 필터링때문에 필요.)(줄수는 출력 줄수가 아닌 버퍼 줄수임에 유의!)
	COLORREF	m_OutputTextColor[MAX_Q];
	CSurface	*m_TextSurf[MAX_DRAW_Q];
	QlistBox_ExtraInfo m_ExtraInfo[MAX_Q];
	QlistBox_ExtraInfo m_OutputExtraInfo[MAX_Q];
	QlistBox_ExtraInfo m_DrawExtraInfo[MAX_DRAW_Q];
	//COLORREF	m_TextBkColor;
	HFONT		m_Font;						// Font..
	int m_FontSize;

	BOOL m_bScrollFlag; // 스크롤 바 출력 플래그.

	CQlistBox();
	CQlistBox (int x, int y, int w, int h, int view_count, int font_size);
	virtual ~CQlistBox ();			// 소멸자

	HRESULT		RestoreSurfaces();

	void DeleteRes();
	void DeleteImg();
	void DeleteTextSurf();

	// Message 처리
	LRESULT	MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void SetFont (int f_w, int f_h, char *fontname);
	int AddString (char *str, COLORREF textColor = RGB(255, 255, 255));		// Line 추가
	int AddString_ExtraInfo(char* str, COLORREF textColor = RGB(255, 255, 255), int infoType = 0, int infoVnum = 0);
	void DrawText(int idx);
	void MakeFillterdStr(); // 옵션에 따라서 필터링 된 스트링을 아웃풋 버퍼에 넣어준다.
	void UpdateSurface(); // 출력할 서피스 셋트를 작성한다. 스크롤 기능이 추가되면서 이걸 쓴다.(05-03-04 by 이원석)
	void Draw();		// Drawing
	void SetImage(int m_FontSize);// char *DecBtn, char *IncBtn, char *ScrollBar, int img_width, int btn_height, int sbar_height);
	void SetValue(int x, int y, int w, int h, int view_count);
	void  SetTem(int distance)
	{
		m_Tem = distance;
	};

	HRESULT Clear(CSurface *surf);

	int m_StartViewCount;


	// 필터링 옵션 플래그.
	BOOL m_bViewAll;
	BOOL m_bViewNormal;
	BOOL m_bViewParty;
	BOOL m_bViewGuild;
	BOOL m_bViewShout;
	BOOL m_bViewMasterMsg;


	// 스크롤 처리용
	CScroll m_Scroll;

};

#endif // _QLIST_BOX_H_
