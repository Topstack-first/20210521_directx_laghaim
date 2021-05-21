#ifndef _QLIST_BOX_H_
#define _QLIST_BOX_H_

#include "Config.h"



#include "headers.h"

#include "Scroll.h"

class CSurface;

#define MAX_CHAT_STRING_LENGTH 300 // ä�� ���ڼ� ����.(������ �߰�.)
#define MAX_Q	300   // ���� ����Ǵ� ������ ����.(��Ʈ���� ����ϴ� �� ��)
//#define MAX_Q	30   // ���� ����Ǵ� ������ ����.(��Ʈ���� ����ϴ� �� ��)
#define MAX_DRAW_Q 25 // ����ϴ� ���ǽ� ������ �Ѱ�.(����ϴ� �� �� �ִ�ġ��� ���� �ȴ�.)
#define MAX_TEXT_LEN	70
//#define MAX_LINE  20

struct QlistBox_ExtraInfo
{
	int nType;		// 0 : none, 1 : item, 2 : npc
	int nVnum;		// item �Ǵ� npc �� Vnum
	int nStrLen;	// ��Ʈ���� ����
};

class CQlistBox
{

public :

	int		m_PosX, m_PosY;		// Interface ��ü�� ������ ��ġ
	int		m_Width, m_Height;	// Interface ��ü�� ����/���� ����
	int		m_ImgWidth, m_BtnHeight, m_SbarHeight;

	int		m_Head;
	int		m_ViewCount;
	int		m_Tem;  // �� ����Ʈ�ڽ����� ������ �����Ѵ�(

	CSurface	*m_DecBtnSurf;			// Decrement Button Surface
	CSurface	*m_IncBtnSurf;			// Increment Button Surface
	CSurface	*m_SbarSurf;		// Scroll Bar Surface

	char		m_TextQ[MAX_Q][MAX_CHAT_STRING_LENGTH];		// ListBox content Text
	COLORREF	m_TextColor[MAX_Q];
	char		m_OutputText[MAX_Q][MAX_CHAT_STRING_LENGTH]; // ��¿� ����.(�ɼǿ����� ���͸������� �ʿ�.)(�ټ��� ��� �ټ��� �ƴ� ���� �ټ��ӿ� ����!)
	COLORREF	m_OutputTextColor[MAX_Q];
	CSurface	*m_TextSurf[MAX_DRAW_Q];
	QlistBox_ExtraInfo m_ExtraInfo[MAX_Q];
	QlistBox_ExtraInfo m_OutputExtraInfo[MAX_Q];
	QlistBox_ExtraInfo m_DrawExtraInfo[MAX_DRAW_Q];
	//COLORREF	m_TextBkColor;
	HFONT		m_Font;						// Font..
	int m_FontSize;

	BOOL m_bScrollFlag; // ��ũ�� �� ��� �÷���.

	CQlistBox();
	CQlistBox (int x, int y, int w, int h, int view_count, int font_size);
	virtual ~CQlistBox ();			// �Ҹ���

	HRESULT		RestoreSurfaces();

	void DeleteRes();
	void DeleteImg();
	void DeleteTextSurf();

	// Message ó��
	LRESULT	MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void SetFont (int f_w, int f_h, char *fontname);
	int AddString (char *str, COLORREF textColor = RGB(255, 255, 255));		// Line �߰�
	int AddString_ExtraInfo(char* str, COLORREF textColor = RGB(255, 255, 255), int infoType = 0, int infoVnum = 0);
	void DrawText(int idx);
	void MakeFillterdStr(); // �ɼǿ� ���� ���͸� �� ��Ʈ���� �ƿ�ǲ ���ۿ� �־��ش�.
	void UpdateSurface(); // ����� ���ǽ� ��Ʈ�� �ۼ��Ѵ�. ��ũ�� ����� �߰��Ǹ鼭 �̰� ����.(05-03-04 by �̿���)
	void Draw();		// Drawing
	void SetImage(int m_FontSize);// char *DecBtn, char *IncBtn, char *ScrollBar, int img_width, int btn_height, int sbar_height);
	void SetValue(int x, int y, int w, int h, int view_count);
	void  SetTem(int distance)
	{
		m_Tem = distance;
	};

	HRESULT Clear(CSurface *surf);

	int m_StartViewCount;


	// ���͸� �ɼ� �÷���.
	BOOL m_bViewAll;
	BOOL m_bViewNormal;
	BOOL m_bViewParty;
	BOOL m_bViewGuild;
	BOOL m_bViewShout;
	BOOL m_bViewMasterMsg;


	// ��ũ�� ó����
	CScroll m_Scroll;

};

#endif // _QLIST_BOX_H_
