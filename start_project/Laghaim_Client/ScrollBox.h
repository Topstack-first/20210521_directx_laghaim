#ifndef _SCROLL_BOX_H_
#define _SCROLL_BOX_H_

#include "Config.h"



#include <stdlib.h>
#include <Def.h>
//#include "dxutil.h"
#include "ddutil.h"
//#include "header.h"
#include "BasicStatic.h"

class CSurface;
class CTextOutBox;
class CBasicButton;
class IndexedTexture;
class CTextBox;

#define  _VERTEX_CNT	4


class CScrollBox
{

public:
	D3DTLVERTEX				TLVec[_VERTEX_CNT];
	HWND			m_hWnd;

	CBasicStatic	m_background;

	CTextOutBox*    m_pTitleSur;
	CTextOutBox**   m_pListSur;	 // List Normal Img
	CTextOutBox**	m_pOverSur;  // List Over Img
	CTextOutBox**	m_pClickSur; // List Click Img

	CBasicButton*   m_pArrowUp;
	CBasicButton*   m_pArrowDown;
	CSurface	*   m_pBarSur;

	CBasicButton*	m_pBtn1;	 // "ok" 버튼
	CBasicButton*	m_pBtn2;	 // "cancle" 버튼
	CBasicButton*	m_pBtn3;	 // "back" 버튼
	CBasicButton*	m_pBtn4;	 // "next" 버튼

	CTextBox*		m_pInBox;	 // 포인트 재분배 입력란
	CSurface*		m_pBox;      // 포인트 재분배 입력란 바닥 그림

	char *  m_sTitleStr;
	char ** m_sListStr;
	char    m_sPreVal[100];

	BOOL    m_bMainActive;
	BOOL	m_bTitle;
	BOOL    m_bScroll;
	BOOL    m_bLink;
	BOOL	m_bDown;		// 마우스버튼이 클릭됬는가?


	int		m_iCurType;
	int		m_RenderXpos, m_RenderYpos;  // 백그라운드의 출력좌표
	int		m_RenderWidth, m_RenderHeight;
	int		m_ScreenWidth, m_ScreenHeight;
	int		m_iMaxListCnt;
	int     m_iCurListNum;
	int     m_iListBox_X, m_iListBox_Y;
	int     m_iListBox_Cx, m_iListBox_Cy;

	int     m_iStartViewCnt;
	int     m_iEndViewCnt;
	int     m_iViewCnt;

	enum	STATE	{ NONE, SCROLLBAR_POLL, SCROLLBAR_FLAT};

	STATE	m_eState;

	BOOL	m_IsVisible;

public:
	CScrollBox(int Type, BOOL bLink = false);
	~CScrollBox();

	void Init(int MaxListCnt, int ViewCnt);
	void InitFromFile(char*fname, int ViewCnt);
	void SetMaxListCnt(int cnt);

	void InitBtn(int type);
	BOOL SetAddStr(char* str);
	void AddStringMulti(char *string, int len, BOOL flag = TRUE, BOOL Enable_Enter = FALSE);
	BOOL SetTitleStr(char* title);
	void LoadRes();
	void DeleteRes();
	void CreTitleSurface();
	void CreListSurface();

	void Draw();
	void RenderOj(int xpos, int ypos, int cx, int cy);
	void Show_Title();
	void Show_List();
	void Show_ArrowBtn();
	void SetBarPosToGo (int y);
	void CalculateBarPos();
	void GoHome();
	void GoEnd();
	void List_UpScroll();
	void List_DownScroll();

	BOOL  ReadInfoFromTxt(char *filename);
	int  GetFileLineCnt(char* fname);
	char* GetSentence(char *dest, int n, FILE *fp);
	char* PGetSentence(char *dest, int n, PFILE *fp);


	BOOL IsInside_BackGround(int x, int y);
	BOOL IsInside_Text(int index, int x, int y);
	int IsInside_ScrollArea(int x, int y);
	LRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void PreSetValForInBox(int Str, int Int, int Dex, int Con, int Cha);

	void SetDrawPosition();
};
#endif // _SCROLL_BOX_H_