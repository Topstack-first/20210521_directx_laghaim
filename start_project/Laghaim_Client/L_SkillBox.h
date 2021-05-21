#ifndef _L_SKILLBOX_H_
#define _L_SKILLBOX_H_

#include "Config.h"

//---------------------------------------------------------
//		INCLUDE
//---------------------------------------------------------

#include "headers.h"
#include "sheet.h"
#include "basicbutton.h"



class CL_SkillBox
{
public:

	HWND			m_hWnd;
	RECT			m_ClientRc;

	CSurface*		m_pSur;						// 기본어텍 스킬 버튼 normal 이미지
	CSurface*		m_pSur2;					// 기본어텍 스킬 버튼 disable 이미지

	//-- Time Delay shadow
	D3DTLVERTEX		m_flame[5][3]; //-- Lyul skill

	int m_CurRace;								// 선택된 종족은??
	int m_CurSkill;								// 현재 기술을 습득한 스킬은 어떤 것인가??
	int m_SkillIconStartXpos;
	int m_SkillIconStartYpos;

	int m_SkillBox_Xpos, m_SkillBox_Ypos;		// 선택된 스킬을 보여주는 위치
	int m_SkillBox_Width, m_SkillBox_Height;    // 선택된 스킬을 보여주는 크기
	BOOL m_bShowSkillBox;						// 스킬 박스를 출력해도 줗은가??
	float m_fStretchX, m_fStretchY;

public:
	CL_SkillBox();
	~CL_SkillBox();


	void	LoadRes();
	void	DeleteRes();

	int		Draw();
	int		DrawAllSkillIcon();			 // 현재 사용가능한 스킬아이콘들을 보여준다
	int		DrawSeletedSkillIcon();      // 선택된 스킬아이콘을 보여준다
	void    DrawNorSkillIcon();			 // 아이콘에 포함되지않은 기본스킬아이콘을 직접생성하고 드로우한다

	void    Con_LeftSkillPos(int xpos, int ypos, int width, int height);
	BOOL	IsInside(int x, int y) ;
	BOOL    IsInside2(int x, int y, int index) ;
	BOOL    IsInside3(int x, int y) ;
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

#endif // _L_SKILLBOX_H_

