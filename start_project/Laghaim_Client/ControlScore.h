#ifndef _CONTROL_SCORE_H_
#define _CONTROL_SCORE_H_

#include "Config.h"

#include "dxutil.h"
#include "ddutil.h"
#include "sheet.h"
#include "basicbutton.h"
#include "BasicStatic.h"
#include "textoutbox.h"
#include "BaseInterface.h"

//#define TEXTSURFACE_CNT	24 //22에서 23으로 바꿈.다시 24로.. 1113
#define TEXTSURFACE_CNT	30 // [2008/10/28 Theodoric] 29 =>30 이벤트 방어구 추가.

#define LEVELBTN_CNT	5


//class CBaseInterface;
class CSheet;
class CNkCharacter;
class CTextOutBox;


enum ScoreTextOut_Type
{
	ScoreTextOut_Type__Name,
	ScoreTextOut_Type__Race,
	ScoreTextOut_Type__Level,
	ScoreTextOut_Type__Penalty,
	ScoreTextOut_Type__Sex,
	ScoreTextOut_Type__Vital,
	ScoreTextOut_Type__Strength,
	ScoreTextOut_Type__Mana,
	ScoreTextOut_Type__Wisdom,
	ScoreTextOut_Type__Stamina,
	ScoreTextOut_Type__Dex,
	ScoreTextOut_Type__EErg,
	ScoreTextOut_Type__Strain,
	ScoreTextOut_Type__Def,
	ScoreTextOut_Type__Charisma,
	ScoreTextOut_Type__BP,
	ScoreTextOut_Type__GHP,
	ScoreTextOut_Type__Fame,
	ScoreTextOut_Type__X2,
	ScoreTextOut_Type__Stash,
	ScoreTextOut_Type__TotalExp,
	ScoreTextOut_Type__ExpLevel,
	ScoreTextOut_Type__LevelUpPoint,

	ScoreTextOut_Type__EndOfEnum ,

	ScoreTextOut_Type__LP = ScoreTextOut_Type__BP
};


class CControlScore : public CBaseInterface
{
public:
	CBasicStatic	m_background;
	CBasicButton	m_btn_close;
	CBasicButton	m_btn_quest;

	CBasicButton	m_btn_pointup[LEVELBTN_CNT];
	CBasicButton	m_btn_pointdown[LEVELBTN_CNT];


	CTextOutBox		m_textout[TEXTSURFACE_CNT];

	HFONT			m_Font;
	int				m_FontWidth,     m_FontHeight;
	char			m_Tmp[100];

	CNkCharacter	*m_pNkCha;

	CSurface*		m_pCashMarkSur; // 캐쉬 표시용 마크 ///유료상점
	CSurface*       m_pCashTextSur; // 캐쉬 액수 표시용 서피스(나라별로 출력 되고 말고 함으로 그냥 따로 가지고있는다.) ///유료상점

	CSurface*		m_pBuffTexture[4][2]; // 드미트론 점령전 - 버프 효과
	RECT			m_rcDmitronPopup; // 드미트론 점령전 - 버프 효과
	CTextOutBox		m_BuffTextPopup; // 드미트론 점령전 - 버프 효과
	BOOL			m_pIsBuffPopup; // 드미트론 점령전 - 버프 효과

	int		m_SetItemFlag2[4];		// 능력치 색상 변화
	void	SetSetitemFlag(int where, int flag);

	CBasicButton	m_btn_pet;
	CBasicStatic	m_face;
	bool			m_bFaceLoaded;
	//CBasicStatic	m_defense;
	//CBasicStatic	m_status_box;
	std::map<int, int> m_tableHP;
	bool			m_bHPTableLoaded;

public:

	CControlScore();
	~CControlScore();

	HRESULT	RestoreSurfaces();

	void Init(int FontSize);
	void DeleteRes();
	void LoadRes();
	void Draw();
	void DrawDmitronEffect(); // 드미트론 점령전 - 버프 효과
	void SetSheetPtr( CSheet* pSheet );
	void PutString(int idx, char* String, int x, int y, DWORD Color = RGB(231, 203, 102) );
	void SetFont (int f_w, int f_h, char *fontname);
	BOOL IsInside(int x, int y) ;
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Action(int);

	void DrawBack(int xpos, int ypos);
	//void Con_DrawText(int, int, int);

	void SetName();
	void SetRace();
	void SetLevel();
	void SetSex();
	void Set_GameTime(int);
	void SetVital();
	void SetMana();
	void SetStamina();
	void SetEpower();
	void SetStr();
	void SetInt();
	void SetDex();
	void SetCon();
	void SetCha();
	void Set_AAtt_Val(int);
	void Set_Att_S(int);
	void Set_Def(int);
	void Set_Move_S(int);
	void SetMoney();
	void SetExp();
	void Set_NeedExp();
	void SetLevelupPoint();
	void SetFame();
	void SetAbil();
	void SetBP();
	void SetLP();
	void SetGHP();
	void SetCash(); // 캐쉬 값을 입력. TEXTSURFACE_CNT 에 포함되지 않음에 유의. ///유료상점
	void SetAllScore();

	void SetPenalty();
	void SetDefense();//1113 총방어력 표시
	void SetEventDefence(int len); // [2008/10/28 Theodoric] 아놔 짜증 나려해
	int	 GetItemDefense(CItem * pItem); // [2008/7/4 Theodoric] 방어도를 구하는 함수를 따로 빼자

	void SetNkCha(CNkCharacter *pNkCha);

	////////////////////
	void DrawPointUpBtn();
	void PointUpBtnAct(int Idx);
	void PointDownBtnAct(int Idx);
	int  m_LevelUpPoint; // 시험용변수

	void CalcPopupIndex(int x, int y); // 드미트론 점령전 - 점령 현황
	
	void LoadRes_NEW();
	void Draw_NEW();
	void LoadRes_Char(bool bForce = false);
	void LoadRes_HPTable();

	void Draw_OLD();
	void LoadRes_OLD();
};

#endif // _CONTROL_SCORE_H
