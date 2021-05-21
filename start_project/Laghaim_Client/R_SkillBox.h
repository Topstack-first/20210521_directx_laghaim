#ifndef _RSKILL_BOX_H_
#define _RSKILL_BOX_H_

#include "Config.h"

//---------------------------------------------------------
//		INCLUDE
//---------------------------------------------------------
//#include "dxutil.h"
#include "ddutil.h"
#include "headers.h"
#include "sheet.h"
#include "basicbutton.h"


//---------------------------------------------------------
//		DEFINE
//---------------------------------------------------------
class CR_SkillBox
{
public:

	HWND			m_hWnd;
	RECT			m_ClientRc;

	CSurface*		m_pAttNormal;
	CSurface*		m_pAttDisable;
	CSurface*		m_pRotateNormal;
	CSurface*		m_pRotateDisable;


	//-- Time Delay shadow
	D3DTLVERTEX		m_flame[5][3]; //-- Lyul skill

	int m_CurRace;								// 선택된 종족은??
	int m_CurSkill;								// 현재 기술을 습득한 스킬은 어떤 것인가??
	int m_SkillIconStartXpos;
	int m_SkillIconStartYpos;

	int m_SkillBox_Xpos, m_SkillBox_Ypos;		// 선택된 스킬을 보여주는 위치
	int m_SkillBox_Width, m_SkillBox_Height;    // 선택된 스킬을 보여주는 크기
	BOOL		m_bShowSkillBox;				// 스킬 박스를 출력해도 줗은가??
	float m_fStretchX, m_fStretchY;

	char m_commOut[512];

public:
	CR_SkillBox();
	~CR_SkillBox();

	void	LoadRes();
	void	DeleteRes();

	int		Draw();
	int		DrawAllSkillIcon();			 // 현재 사용가능한 스킬아이콘들을 보여준다
	int		DrawSeletedSkillIcon();      // 선택된 스킬아이콘을 보여준다
	void    DrawNorSkillIcon();			 // 아이콘에 포함되지않은 기본스킬아이콘을 직접생성하고 드로우한다

	void    Con_RightSkillPos(int xpos, int ypos, int width, int height);
	BOOL	IsInside(int x, int y) ;
	BOOL    IsInside2(int x, int y, int index) ;
	BOOL    CheckNorSkillIcon(CSurface* surface, int x, int y) ;  // 현제까지 두개의 기본적으로 제공되는 스킬이아콘의 영역을 체크할수 있다
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	int GetCurSkill()
	{
		return m_CurSkill;
	}
	void SetCurSkill(int skill)
	{
		m_CurSkill = skill;
	}
	BOOL GetShowSkillBox()
	{
		return m_bShowSkillBox;
	}

	void SetPos(int x, int y);
};

#endif // _RSKILL_BOX_H_
