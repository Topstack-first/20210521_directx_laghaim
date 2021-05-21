#ifndef _OPTION_PG_H_
#define _OPTION_PG_H_

#include "Config.h"

#include "basicbutton.h"
#include "sheet.h"
#include "spinbox.h"


#define SURFACE_CNT		8
#define RES_BTN_CNT		3
#define SHADOW_BTN_CNT	4
#define CHECK_BTN_CNT	2
#define ON_BTN_CNT		7



class CBasicButton;
class CSheet;


class COptionPg
{
public:
	CSheet*			m_pSheet;

	CSpinBox		m_Shadow;  // 그림자
	CSpinBox		m_Wether;  // 날씨효과
	CSpinBox		m_Mode;    // 화면모드
	CSpinBox		m_Res;     // 해상도


	CSurface*		m_pBack;
	CSurface*		m_pEffVolume;
	CSurface*		m_pMusicVolume;
	CSurface*		m_pGrayVolume1;
	CSurface*		m_pGrayVolume2;
	CSurface*		m_pDragVolume1;
	CSurface*		m_pDragVolume2;
	CSurface*		m_pDragVolume1_C;
	CSurface*		m_pDragVolume2_C;



	CBasicButton	m_EffCheckBtn[CHECK_BTN_CNT];
	CBasicButton	m_MusicCheckBtn[CHECK_BTN_CNT];
	CBasicButton	m_MainBtn;


	int	m_ArrowPos;

	BOOL m_bClickVB1;
	BOOL m_bClickVB2;
	BOOL m_bMusicOn;
	BOOL m_bMusicEffOn;

	int m_CurResolution;
	int m_CurShadow;
	int m_CurWeather;
	int m_CurMusicLevel;
	int m_CurMusicEffLevel;
	int m_CurMode;

	COptionPg();
	COptionPg( CSheet* pSheet );
	~COptionPg();

	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	void DrawEffVolume();
	void DrawMusicVolume();
	void DrawVD();
	void DrawGrayVolume();
	void DrawOnBtn();
	void MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	BOOL IsInside(CSurface* surface, int x, int y) ;
	void ReadConfigFile(char *file_name);
	void WriteConfigFile(char *file_name);
	char *GetSentence(char *dest, int n, FILE *fp) ;
	char *AnyTwoArg(char *argument, char *first_arg, char *second_arg);
	void SkipSpaces(char **string);
	char *AnyOneArg(char *argument, char *first_arg);

	HRESULT Restore();
};

#endif // _OPTION_PG_H_
