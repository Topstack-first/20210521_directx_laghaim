#ifndef _CONTROL_SKILL_H_
#define _CONTROL_SKILL_H_

#include "Config.h"

#include "NkCharacter.h"
#include "BasicButton.h"
#include "TextOutBox.h"

class CSurface;
class CNkCharacter;
class CTcpIpConnection;
class CAlphaPopUp;
class CMultiText;


#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

#define MENU_WIDTH		(273)// * m_ScreenWidth / BASE_X_SIZE)
#define MENU_HEIGHT		(388)// * m_ScreenHeight / BASE_Y_SIZE)
#define MENU_START_X	(366)// * m_ScreenWidth / BASE_X_SIZE)
#define MENU_START_Y	(29)// * m_ScreenHeight / BASE_Y_SIZE)

#define SKILL_ICON_WIDTH	(32)// * m_ScreenWidth / BASE_X_SIZE)
#define SKILL_ICON_HEIGHT	(32)// * m_ScreenHeight / BASE_Y_SIZE)
#define SKILL_ICON_START_X	(411)// * m_ScreenWidth / BASE_X_SIZE)
#define SKILL_ICON_START_Y	(144)// * m_ScreenHeight / BASE_Y_SIZE)
#define SKILL_ICON_X_GAP	(52)// * m_ScreenWidth / BASE_X_SIZE)
#define SKILL_ICON_Y_GAP	(66)// * m_ScreenHeight / BASE_Y_SIZE)
#define SKILL_ICON_X_NUM	4
#define SKILL_ICON_Y_NUM	4
#define SKILL_LEVEL_START_X	(418)// * m_ScreenWidth / BASE_X_SIZE)
#define SKILL_LEVEL_START_Y	(125)// * m_ScreenHeight / BASE_Y_SIZE)

#define LEVELUP_X		(565)// * m_ScreenWidth / BASE_X_SIZE)
#define LEVELUP_Y		(370)// * m_ScreenHeight / BASE_Y_SIZE)

#define SKILL_CLOSE_BTN_X		(SKILL_ICON_START_X+166)// * m_ScreenWidth / BASE_X_SIZE)
#define SKILL_CLOSE_BTN_Y		(SKILL_ICON_START_Y+357)// * m_ScreenHeight / BASE_Y_SIZE)
//#define CLOSE_BTN_W		(24 * m_ScreenWidth / BASE_X_SIZE)
//#define CLOSE_BTN_H		(40 * m_ScreenHeight / BASE_Y_SIZE)

#define IDS_CTR_SKILL_CNT		12

class CControlSkill
{
public:
	HWND	m_hWnd;
	RECT	m_ClientRc;

	CControlSkill();
	virtual ~CControlSkill();

	HRESULT	RestoreSurfaces();

	void Init();
	void LoadRes();
	void DeleteRes();
	int Draw();
	void DrawSkillInfo(int over_skill);
	void SetNkCha(CNkCharacter *pNkCha = NULL);
	void SetLevelupPoint();
	void SetSkillEnable();
	//void AddDescr(CMultiText *pMultiText, char *descr, int len);
	BOOL IsInside(int x, int y);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	CSurface *m_pSurfaceBack;
	CNkCharacter *m_pNkCha;

	CBasicButton m_CloseBtn;
////2003 스킬창 플립에 필요한 버튼 2개.
	CBasicButton m_BtnWindow_1;
	CBasicButton m_BtnWindow_2;

	int m_ScreenWidth;
	int m_ScreenHeight;

	CTextOutBox m_LevelupPoint;

	CAlphaPopUp *m_pSkillInfo;

	int m_skilliconstart_x, m_skilliconstart_y;
	int m_levelfontstart_x, m_levelfontstart_y;
	void SetSkillInfo( CMultiText &multiText, int nSkill_Index, int nSkill_Level, int nCount );
	bool DrawDiableSkill( int nSkill_Index );

	bool IsTabSkill(int tab, int skill_index);
	void SetSkillInfo_Perom(CMultiText &multiText, int nSkill_Index, int nSkill_Level, int nCount);

	int GetSkillEffectValue_Int(int skill_index, int skill_level);
	float GetSkillEffectValue_Float(int skill_index, int skill_level);
};

#endif // _CONTROL_SKILL_H_
