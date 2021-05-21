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

	int m_CurRace;								// ���õ� ������??
	int m_CurSkill;								// ���� ����� ������ ��ų�� � ���ΰ�??
	int m_SkillIconStartXpos;
	int m_SkillIconStartYpos;

	int m_SkillBox_Xpos, m_SkillBox_Ypos;		// ���õ� ��ų�� �����ִ� ��ġ
	int m_SkillBox_Width, m_SkillBox_Height;    // ���õ� ��ų�� �����ִ� ũ��
	BOOL		m_bShowSkillBox;				// ��ų �ڽ��� ����ص� �G����??
	float m_fStretchX, m_fStretchY;

	char m_commOut[512];

public:
	CR_SkillBox();
	~CR_SkillBox();

	void	LoadRes();
	void	DeleteRes();

	int		Draw();
	int		DrawAllSkillIcon();			 // ���� ��밡���� ��ų�����ܵ��� �����ش�
	int		DrawSeletedSkillIcon();      // ���õ� ��ų�������� �����ش�
	void    DrawNorSkillIcon();			 // �����ܿ� ���Ե������� �⺻��ų�������� ���������ϰ� ��ο��Ѵ�

	void    Con_RightSkillPos(int xpos, int ypos, int width, int height);
	BOOL	IsInside(int x, int y) ;
	BOOL    IsInside2(int x, int y, int index) ;
	BOOL    CheckNorSkillIcon(CSurface* surface, int x, int y) ;  // �������� �ΰ��� �⺻������ �����Ǵ� ��ų�̾����� ������ üũ�Ҽ� �ִ�
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
