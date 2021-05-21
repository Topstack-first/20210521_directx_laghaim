#ifndef _LETTER_BOX_H_
#define _LETTER_BOX_H_

#include "Config.h"

#include "L_MultiEditBox.h"
#include "HanIme.h"

#define PROMPT_STRING	"|"
#define PROMPT_TIME		800
#define CANDYFONT_SIZE 11

class CSurface;

typedef enum n_CandidatePos1
{
	LETTER=0

} n_CandidatePos1;

class CLetterBox :public CHanIme
{
public:
	CLetterBox();
	virtual ~CLetterBox();

	L_MultiEditBox m_MEBox;
	bool m_read_only ;
	int m_row;
	int m_column;

	void Init_MEBox(bool read_only, int row, int column);
	HRESULT	RestoreSurfaces();

	void SetLetterFlag(bool flag);
	void OnChar(UINT nChar);
	bool OnKeyDown(CHAR cChar);
	bool delChar();
	void CursLMove(INT iState);		//커서 왼쪽 이동
	void CursRMove(INT iState);		//커서 오른쪽이동
	void CursUMove(INT iState);		//커서 위로이동
	void CursDMove(INT iState);		//커서 아래이동
	void LetterTableUpdate();//문자 테이블 업데이트
	void GetLetterCursor(int cursor);//문자 테이블 커서위치
	void LetterTableReset();

	void Init(int x, int y, int len, COLORREF backColor, COLORREF foreColor,
			  BOOL nPassword = FALSE, n_CandidatePos1 n_candyPos = LETTER);
	void SetFontSize(int fontSize = 9);
	void DeleteRes();
	void Draw();
	void Draw(int, int, int);

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
//	char *m_MulLetterText[MaxRow];


	int m_X;
	int m_X2;
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

	void SetCandidatePos(int* x_pos, int* y_pos);

	n_CandidatePos1		m_nCandidatePos;

	void drawConversionStatus();

	void CloseIMC();

	bool getFontSize(LPSIZE pSize,HFONT hFont,LPCTSTR pText,INT iLength);
};


#endif // _LETTER_BOX_H_
