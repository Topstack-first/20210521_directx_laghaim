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

	CSurface*		m_pSur;						// �⺻���� ��ų ��ư normal �̹���
	CSurface*		m_pSur2;					// �⺻���� ��ų ��ư disable �̹���

	//-- Time Delay shadow
	D3DTLVERTEX		m_flame[5][3]; //-- Lyul skill

	int m_CurRace;								// ���õ� ������??
	int m_CurSkill;								// ���� ����� ������ ��ų�� � ���ΰ�??
	int m_SkillIconStartXpos;
	int m_SkillIconStartYpos;

	int m_SkillBox_Xpos, m_SkillBox_Ypos;		// ���õ� ��ų�� �����ִ� ��ġ
	int m_SkillBox_Width, m_SkillBox_Height;    // ���õ� ��ų�� �����ִ� ũ��
	BOOL m_bShowSkillBox;						// ��ų �ڽ��� ����ص� �G����??
	float m_fStretchX, m_fStretchY;

public:
	CL_SkillBox();
	~CL_SkillBox();


	void	LoadRes();
	void	DeleteRes();

	int		Draw();
	int		DrawAllSkillIcon();			 // ���� ��밡���� ��ų�����ܵ��� �����ش�
	int		DrawSeletedSkillIcon();      // ���õ� ��ų�������� �����ش�
	void    DrawNorSkillIcon();			 // �����ܿ� ���Ե������� �⺻��ų�������� ���������ϰ� ��ο��Ѵ�

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

