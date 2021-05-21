#ifndef _ONE_SELF_INFO_H_
#define _ONE_SELF_INFO_H_

#include "Config.h"

#include "dxutil.h"
#include "ddutil.h"
#include "sheet.h"
#include "basicbutton.h"

#define TEXTSURFACE_CNT	22

class CSheet;

class COneSelfInfo
{
protected:

	CSheet*			m_pSheet;
	CSurface*		m_pBackSur;
	CSurface*		m_pTextSur[TEXTSURFACE_CNT];
	CBasicButton	m_CloseBtn;


	HWND			m_hWnd;
	RECT			m_ClientRc;

	HFONT			m_Font;
	float			m_fStretchX, m_fStretchY;
	int				m_FontWidth,     m_FontHeight;

	char			m_Tmp[21];
	char			m_SaveBuf[TEXTSURFACE_CNT][256];

	DWORD			m_TextColor;
	DWORD			m_TextBackColor;

public:
	COneSelfInfo();
	~COneSelfInfo();
	void Init(int FontSize);
	void DeleteRes();
	void LoadRes();
	void Draw();
	void SetSheetPtr( CSheet* pSheet );
	void PutString(int index, char* String);
	void SetFont (int f_w, int f_h, char *fontname);
	BOOL IsInside(int x, int y) ;
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Action(int);

	void DrawBack(int xpos, int ypos);
	void Con_DrawText(int, int, int);
	void RestoreAll();

	void Set_Name(char*);
	void Set_Race(char*);
	void Set_Level(int);
	void Set_Sex(char*);
	void Set_GameTime(int);
	void Set_Vital(int);
	void Set_Mana(int);
	void Set_Stamina(int);
	void Set_Epower(int);
	void Set_Str(int);
	void Set_Int(int);
	void Set_Dex(int);
	void Set_Con(int);
	void Set_Cha(int);
	void Set_AAtt_Val(int);
	void Set_Att_S(int);
	void Set_Def(int);
	void Set_Move_S(int);
	void Set_CurMoney(__int64);
	void Set_CurExp(__int64);
	void Set_NeedExp(__int64);
	void Set_LevelUpPoint(int);
};

#endif // _ONE_SELF_INFO_H_
