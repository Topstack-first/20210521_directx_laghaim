#ifndef _TEXT_OUT_BOX_H_
#define _TEXT_OUT_BOX_H_

#include "Config.h"

#include "AlphaPopup.h"

class CSurface;

class CTextOutBox
{
public:
	CTextOutBox();
	virtual ~CTextOutBox();

	void DeleteRes();


	//
	void Init(int font_size, COLORREF font_color, int pos_x, int pos_y, int width = -1, int height = -1, BOOL bCenter = FALSE);
	void Restore();

	void SetFont (int f_w, int f_h, char *fontname);
	void SetPos_Center(int x, int y);

	void SetPos(int x, int y);
	void SetSize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}
	void SetString(char *string, COLORREF font_color = RGB(0, 0, 0));
	void SetString(char *string, COLORREF font_color, COLORREF background);
	void SetString(UINT64 value, BOOL bcomma = FALSE);
	void SetString_Center_Arrange(char *string, COLORREF font_color = RGB(0,0,0), COLORREF font_colorover = RGB(0,0,0), BOOL bCenter = FALSE);
	void SetString_withOutLine( char *string, COLORREF font_color, COLORREF outline_color ); // 글자에 외곽선을 넣어서 출력해주는 함수.(참고로 많이쓰면 느려짐의 원인이 될수있다.) (by 원석)

	void SetVisible(BOOL bVisible)
	{
		m_bVisible = bVisible;
	}
	int	 GetBoxHeight();
	int	 GetBoxWidth();
	char* GetString()
	{
		return m_str;
	}

	inline BOOL IsSurface()
	{
		if(m_pSurf) return TRUE;
		else		return FALSE;
	}

	int  m_AreaWidth;
	int  m_AreaHeight;

	void ChangeColor(COLORREF color);

	void Draw();
	void Draw(int x, int y);
	void Draw(int x, int y, int dest_cx);  // 글자 가운데정렬을위해서 새로만든 함수
	void Draw(RECT rc, int x, int y);
	void Draw(int dest_cx);
	void DrawText(int y, BOOL bOver = FALSE);
	BOOL DrawClipText(int y, int nMinArea, int nMaxArea, int nScrollPos, BOOL bOver = FALSE);

	void SetOver(BOOL bOver)
	{
		m_bOver = bOver;
	}

	CSurface *m_pSurf;

	CSurface *m_pOverSurf;
	BOOL	  m_bOver;

	char m_str[255];

	int m_PosX, m_PosY;
	int m_Width, m_Height;
	int m_StringPosX, m_StringPosY;
	HFONT m_Font;
	int m_FontWidth, m_FontHeight;
	int m_FontSize;
	COLORREF m_FontColor;
	BOOL m_bCenter;
	BOOL m_bVisible;
	int  m_Type;



	// 스트링 포멧 변경 관련 변수 및 함수
	char  m_sMoney[20];
	char* TransComma(char* money) ;
	char* delchr(char *str) ;
	char* iedt(char *str, char *fmt) ;

};

// 드미트론 점령전 - 다중 텍스트 출력
class CTextOutMultiBox
{
public:
	CTextOutMultiBox();
	virtual ~CTextOutMultiBox();

	void DeleteRes();


	//
	void Init(int font_size, COLORREF font_color, int pos_x, int pos_y, int width = -1, int height = -1, BOOL bCenter = FALSE);
	void Restore();

	void SetFont (int f_w, int f_h, char *fontname);
	void SetPos_Center(int x, int y);

	void SetPos(int x, int y);
	void SetSize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}

	void SetString(CMultiText* multi_text, COLORREF font_color = RGB(0, 0, 0));
	void SetString(CMultiText* multi_text, COLORREF font_color, COLORREF background);

	void SetString_Center_Arrange(CMultiText* multi_text, COLORREF font_color = RGB(0,0,0), COLORREF font_colorover = RGB(0,0,0), BOOL bCenter = FALSE);
	void SetString_withOutLine( CMultiText* multi_text, COLORREF font_color, COLORREF outline_color ); // 글자에 외곽선을 넣어서 출력해주는 함수.(참고로 많이쓰면 느려짐의 원인이 될수있다.) (by 원석)

	void SetVisible(BOOL bVisible)
	{
		m_bVisible = bVisible;
	}
	int	 GetBoxHeight();
	int	 GetBoxWidth();
	//char* GetString(int n){return m_str[n];}

	inline BOOL IsSurface()
	{
		if(m_pSurf) return TRUE;
		else		return FALSE;
	}

	int  m_AreaWidth;
	int  m_AreaHeight;

	void ChangeColor(COLORREF color);

	void Draw();
	void Draw(int x, int y);
	void Draw(int x, int y, int dest_cx);  // 글자 가운데정렬을위해서 새로만든 함수
	void Draw(RECT rc, int x, int y);
	void DrawText(int y, BOOL bOver = FALSE);
	BOOL DrawClipText(int y, int nMinArea, int nMaxArea, int nScrollPos, BOOL bOver = FALSE);

	void SetOver(BOOL bOver)
	{
		m_bOver = bOver;
	}

	CSurface *m_pSurf;

	CSurface *m_pOverSurf;
	BOOL	  m_bOver;

	CMultiText m_MultiText;

	int m_PosX, m_PosY;
	int m_Width, m_Height;
	int m_StringPosX, m_StringPosY;
	HFONT m_Font;
	int m_FontWidth, m_FontHeight;
	int m_FontSize;
	COLORREF m_FontColor;
	BOOL m_bCenter;
	BOOL m_bVisible;
	int  m_Type;

};

#endif // _TEXT_OUT_BOX_H_
