#ifndef _TEXT_BOX_H_
#define _TEXT_BOX_H_

#include "Config.h"

#include "HanIme.h"

#define PROMPT_STRING	"|"
#define PROMPT_TIME		800

#define CANDYFONT_SIZE 11

class CSurface;
class CHanIme;

typedef enum n_CandidatePos
{
	LOGIN_PAGE = 0,
	NORMAL,
	MSG_POPUP,
	NEW_CHARNAME

} n_CandidatePos;


class CTextBox : public CHanIme
{
public:
	CTextBox();
	virtual ~CTextBox();

	HRESULT	RestoreSurfaces();

	void Init(int x, int y, int len, COLORREF backColor, COLORREF foreColor,
			  BOOL nPassword = FALSE, n_CandidatePos n_candyPos = NORMAL);
	void SetfontSize(int fontSize = 9);
	void DeleteRes();
	void Draw();
	void Draw(int, int, int);
	BOOL DrawToClip(int nMinArea, int nMaxArea, int nScrollPos);
	BOOL IsInsideClip(int x, int y, int nMinArea, int nMaxArea, int nScrollPos);
	LRESULT MsgProcClip( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScrollPos, int nMin, int nMax);

	void SetFont (int f_w, int f_h, char *fontname);
	void DrawText();
	void SetPos(int x, int y);
	void SetSize(int width, int height);
	void SetColor(COLORREF backColor, COLORREF foreColor);
	void SetStringLength(int len);
	BOOL IsInside(int x, int y);
	void SetEnable(BOOL bEnable);
	void ChangeColor(DWORD color);



	//...........................수정부분
	int GetEnable()
	{
		return m_bEnabled;
	}
	void ClearText();
	char* GetStrBuf()
	{
		return m_pszStr;
	}
	//...................................
	void SetText(char *str);
	void SetPassword_Flg(bool flg)
	{
		m_bIsPassword = flg;
	}

	char* _StrDup();

	// 메시지 처리 루틴
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	CSurface *m_pSurf;
	CSurface *m_pPromptSurf;

	// For Candidate
	CSurface *m_pCandidateSurf;
	CSurface *m_pBack_CandidateSurf;

	CHAR m_cBuff[9][512];
	INT m_iCandidateIndex;
	INT	m_iCandidateStart;
	INT	m_iCandidateEnd;
	INT	m_heightCandidate;

	CSurface *m_pStatusSurf;


	char *m_SpaceStr;

	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

	HFONT m_Font;
	HFONT m_CandyFont;
	int m_FontSize;
	int m_FontWidth;
	int m_FontHeight;

	COLORREF m_BackColor;
	COLORREF m_ForeColor;

	DWORD m_LastTime;
	BOOL m_bShowPrompt;
	BOOL m_bEnabled;

	BOOL m_bDImeEnable;
	void SetDImeEnable(BOOL bDImeEnable)
	{
		m_bDImeEnable = bDImeEnable;
	};
	BOOL GetDImeEnable()
	{
		return m_bDImeEnable;
	};

	// DIme
	POINT m_po_ime, m_po_edit;

	// For Japanese
	void drawIMEString();
//	bool getFontSize(LPSIZE pSize, HFONT hFont, LPCTSTR pText,INT iLength);
	void drawCandidate();

	n_CandidatePos		m_nCandidatePos;

	void SetCandidatePos(int* x_pos, int* y_pos);

	void drawConversionStatus();

	void CloseIMC();
};

bool getFontSize(LPSIZE pSize,HFONT hFont,LPCTSTR pText,INT iLength);


#endif // _TEXT_BOX_H_
