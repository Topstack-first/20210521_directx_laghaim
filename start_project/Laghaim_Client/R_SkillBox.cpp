#include "stdafx.h"
//-------------------------------------------------------------
//		INCLUDE
//-------------------------------------------------------------

#include "r_skillbox.h"
#include "skill.h"
#include "main.h"
#include "UIMgr.h"
#include "Tcpipcon.h"
#include "Itemproto.h"
#include "Country.h"
#include "string_res.h"
#include "g_stringmanager.h"
#include "PacketSend.h"

extern POINT g_CursorPoint;
extern DWORD g_dwClientCountry;

//-------------------------------------------------------------
//		DEFINE
//-------------------------------------------------------------

#define R_START_XPOS  438			// ��ų ������ ������ X ������ġ
#define R_START_YPOS  350			// ��ų ������ ������ Y ������ġ
#define R_CUR_SKILLBOX_XPOS 438		// ���ص� �������� ������ X ��ġ
#define R_CUR_SKILLBOX_YPOS 423		// ���ص� �������� ������ Y ��ġ
#define R_SOKET_CX  -6				// ������ ���� ����
#define R_SOKET_CY 5				// ������ ���� ����
#define R_ICON_WIDTH  32			// �������� ����ũ��
#define R_ICON_HEIGHT 32			// �������� ����ũ��



//-------------------------------------------------------------
//		CONSTRUCTOR / DESTRUCTOR
//-------------------------------------------------------------
CR_SkillBox::CR_SkillBox()
	: m_hWnd(NULL)
	, m_CurRace(0)
	, m_SkillIconStartXpos(0)
	, m_SkillIconStartYpos(0)
	, m_SkillBox_Xpos(0)
	, m_SkillBox_Ypos(0)
	, m_SkillBox_Width(0)
	, m_SkillBox_Height(0)
	, m_fStretchX(0)
	, m_fStretchY(0)
{
	memset(m_commOut, 0, sizeof(m_commOut));

	int i,j;

	m_bShowSkillBox = FALSE;
	m_CurSkill = SELECTED_SKILL_ROTATE;
	m_pAttNormal  = NULL;				// �⺻ ��ų(����)�������� ���� �׷����Ѵ�..
	m_pAttDisable = NULL;
	m_pRotateNormal  = NULL;				// �⺻ ��ų(����)�������� ���� �׷����Ѵ�..
	m_pRotateDisable = NULL;

	//-- Skill Delay Shadow Box
	for(i=0; i<5; i++)
	{
		for(j=0; j<3; j++)
		{
			m_flame[i][j].sz = 0.0f;
			m_flame[i][j].rhw = 1;
			m_flame[i][j].color = 0x88000000;
			if(j==1)m_flame[i][j].color = 0xee000000;
		}
	}

}


CR_SkillBox::~CR_SkillBox()
{
	DeleteRes();
}

//-------------------------------------------------------------
//		LOAD RESOURCE
//-------------------------------------------------------------
void CR_SkillBox::LoadRes()
{
//	 = pDis;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_fStretchX = (m_ClientRc.right-m_ClientRc.left) / 640.0f ;
	m_fStretchY = (m_ClientRc.bottom-m_ClientRc.top) / 480.0f;

	if(m_ClientRc.right==640 && m_ClientRc.bottom==480)
	{

		m_SkillIconStartXpos = (438);	// ��ų ������ ������ X ������ġ
		m_SkillIconStartYpos = (350);
		m_SkillBox_Xpos = 438;			// ���õ� ��ų�� �����ش�
		m_SkillBox_Ypos = 423;
	}
	else if(m_ClientRc.right==800 && m_ClientRc.bottom==600)
	{
		m_SkillIconStartXpos = (516);
		m_SkillIconStartYpos = (491);
		m_SkillBox_Xpos = 516;
		m_SkillBox_Ypos = 542;
	}
	else if(m_ClientRc.right==1024 && m_ClientRc.bottom==768)
	{
		m_SkillIconStartXpos = (630);	// ��ų ������ ������ X ������ġ
		m_SkillIconStartYpos = (660);
		m_SkillBox_Xpos = 630;			// ���õ� ��ų�� �����ش�
		m_SkillBox_Ypos = 710;
	}
	m_SkillBox_Width = 32;
	m_SkillBox_Height = 32;

	// ��� ���� �̹����� ���� �ػ󵵿� ���� ũ�⸦ �����ؾ��Ѵ�
	int CX, CY;
	int GetPercent_Width = ((m_ClientRc.right)* 100) / 640 ;
	CX  = (((R_ICON_WIDTH) * (GetPercent_Width)) / 100);

	int GetPercent_Height= ((m_ClientRc.bottom)*100) / (480);
	CY  = (((R_ICON_HEIGHT) * (GetPercent_Height)) / 100);

	// �⺻���� ��ų�������̹����� �����Ѵ�..������� �϶� �׷���
	// normal �̹����� unable�̹����� �����Ѵ�
	// ������ �߱�..���°� ������� �Ѵٱ� �׷���
	g_pDisplay->CreateSurfaceFromBitmap( &m_pAttNormal, "interface/skill/skill_0_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pAttDisable, "interface/skill/skill_0_unable.bmp");
	m_pAttNormal->SetColorKey(RGB(0, 0, 0));
	m_pAttDisable->SetColorKey(RGB(0, 0, 0));
	g_pDisplay->CreateSurfaceFromBitmap( &m_pRotateNormal, "interface/skill/rotate_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pRotateDisable, "interface/skill/rotate_unable.bmp");
	m_pRotateNormal->SetColorKey(RGB(0, 0, 0));
	m_pRotateDisable->SetColorKey(RGB(0, 0, 0));

	// �ΰ��� �⺻������ �����Ǵ� ��ų�������� �ִ�.
	// �� �⺻ ��ų �������� ��ġ�� �׻� ������ �����߿� ���� ó�� �׸��� �״��� ��ġ�� ���ߴ�
	m_pRotateNormal->Xpos = m_SkillIconStartXpos;
	m_pRotateNormal->Ypos = m_SkillIconStartYpos;
	m_pAttNormal->Xpos = m_SkillIconStartXpos - m_pAttNormal->GetWidth();
	m_pAttNormal->Ypos = m_SkillIconStartYpos;
}


//-------------------------------------------------------------
//		DELETE
//-------------------------------------------------------------
void CR_SkillBox::DeleteRes()
{
	if(m_pAttNormal)
		SAFE_DELETE(m_pAttNormal);
	if(m_pAttDisable)
		SAFE_DELETE(m_pAttDisable);
	if(m_pRotateNormal)
		SAFE_DELETE(m_pRotateNormal);
	if(m_pRotateDisable)
		SAFE_DELETE(m_pRotateDisable);
}



//-------------------------------------------------------------
//		DRAW
//-------------------------------------------------------------
int CR_SkillBox::Draw( void )
{
	int over_skill1 = DrawAllSkillIcon();
	int over_skill2 = DrawSeletedSkillIcon();

	if (over_skill1 != SKILL_UNSELECTED)
		return over_skill1;
	return over_skill2;
}

//-------------------------------------------------------------
//		�⺻��ų �ڽ� (��ư)���� ȭ��ü ����ϱ�
//-------------------------------------------------------------
int CR_SkillBox::DrawAllSkillIcon()
{
	// ������ ��ų������ ��ġ�� �����ʺ��� �������� ü�����Ѵ�
	int x =-2;  /* �⺻��ų������ (���� 2��)�� ���� ó���� ����(ù��° , �ι�°)�� �����ϹǷ� �� ������ ��ų �����ܵ��� �� ������ġ���� �׷��ֱ� �����Ѵ�
	               */
	int y =0;
	int over_skill = SKILL_UNSELECTED;
	if(m_bShowSkillBox == TRUE)
	{
		DrawNorSkillIcon();
		if (CheckNorSkillIcon(m_pAttNormal, g_CursorPoint.x, g_CursorPoint.y))
			over_skill = SELECTED_SKILL_ATTACT;
		else if (CheckNorSkillIcon(m_pRotateNormal, g_CursorPoint.x, g_CursorPoint.y))
			over_skill = SELECTED_SKILL_ROTATE;

		for (int index=0; index< g_ChaSkill.m_SkillNum; index++)
		{
			if (g_ChaSkill.m_SkillLevel[index] > 0 && g_ChaSkill.m_bRight[index]== TRUE)
			{
				int icon_x = m_SkillIconStartXpos + (x * g_ChaSkill.m_SkillIcon[0].Width);
				int icon_y = m_SkillIconStartYpos + (y * g_ChaSkill.m_SkillIcon[0].Height);

				if (g_CursorPoint.x >= icon_x && g_CursorPoint.x < icon_x + m_pAttNormal->GetWidth()
						&& g_CursorPoint.y >= icon_y && g_CursorPoint.y < icon_y + m_pAttNormal->GetHeight() )
					over_skill = index;

				g_ChaSkill.m_SkillIcon[index].SetState( BTN_NORMAL );
				g_ChaSkill.m_SkillIcon[index].Draw(icon_x, icon_y);
				g_ChaSkill.m_SkillIcon[index].PosX = icon_x;
				g_ChaSkill.m_SkillIcon[index].PosY = icon_y;

				g_ChaSkill.UpdateRskill( index, icon_x , icon_y );

				x --;
				if(x <= R_SOKET_CX)
				{
					x = -1;
					y --;
				}
			}
		}
	}
	return over_skill;
}

void CR_SkillBox::DrawNorSkillIcon()
{
	g_pDisplay->Blt( m_pAttNormal->Xpos, m_pAttNormal->Ypos, m_pAttNormal, NULL);
	g_pDisplay->Blt( m_pRotateNormal->Xpos, m_pRotateNormal->Ypos, m_pRotateNormal, NULL);
}

//----------------------------------------------------------------------------
//-- ���õ� ��ų�������� �׸���
//----------------------------------------------------------------------------
//-- 2004.11.20
//-- Lyul / Skill Delay Shadow �߰�.
//-- ���� �Ǵ��� ���� main �� conditioincheck �Լ��� �����Ѵ�.

int CR_SkillBox::DrawSeletedSkillIcon()
{
	static float alpha = 0;
	DWORD dwstate;

	if(m_CurSkill == SELECTED_SKILL_ATTACT)
		g_pDisplay->Blt(m_SkillBox_Xpos, m_SkillBox_Ypos, m_pAttNormal, NULL);
	else if(m_CurSkill == SELECTED_SKILL_ROTATE)
		g_pDisplay->Blt(m_SkillBox_Xpos, m_SkillBox_Ypos, m_pRotateNormal, NULL);

	if(m_CurSkill>=0)
	{
		if(pCMyApp->SkillConditionCheck(m_CurSkill))
			g_ChaSkill.m_SkillIcon[m_CurSkill].SetState( BTN_NORMAL );
		else
			g_ChaSkill.m_SkillIcon[m_CurSkill].SetState( BTN_DISABLE );

		g_ChaSkill.m_SkillIcon[m_CurSkill].Draw(m_SkillBox_Xpos ,m_SkillBox_Ypos );
	}
	//--------------------------------------------------------------------------
	//-- Lyul Skill Test

	G_SK_RotateShadow();

	//-- Shadow set true
	if(m_CurSkill >= 0 && G_SK_Getflag( g_ChaSkill.m_SkillIndex[m_CurSkill] ))
	{
		alpha = G_SK_GetShdangle( g_ChaSkill.m_SkillIndex[m_CurSkill], g_ChaSkill.m_SkillLevel[m_CurSkill]);
		D3DVIEWPORT7			m_vpOld, m_vpNew;


		DWORD src_blend;
		DWORD dest_blend;
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND,&src_blend);//D3DBLEND_DESTCOLOR);// D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dest_blend);//D3DBLEND_SRCCOLOR

		//-- Alpha Enable
		GET_D3DDEVICE()->SetTexture(0,NULL);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwstate );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,   TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,D3DBLEND_SRCALPHA);//D3DBLEND_DESTCOLOR);// D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_DESTCOLOR);//D3DBLEND_SRCCOLOR

		// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
		GET_D3DDEVICE()->GetViewport(&m_vpOld);
		m_vpNew = m_vpOld;
		m_vpNew.dwX = m_SkillBox_Xpos;//m_SkillBox_Xpos;
		m_vpNew.dwY = m_SkillBox_Ypos;
		m_vpNew.dwWidth = 32;
		m_vpNew.dwHeight = 32;
		GET_D3DDEVICE()->SetViewport(&m_vpNew);

		//------------------------------------------------------------------------
		//-- Skill Time Shadow
		//-- Lyul skill
		//-- Color Setting : �����Ǹ� �����ڿ� �ڵ带 �д�.
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<3; j++)
			{
				m_flame[i][j].color = 0xaaffffff;//m_flame[i][j].color = 0x88000000;
				if(j==1)
					m_flame[i][j].color = 0x88666666;
			}
		}
		//-- �߽���.
		m_flame[0][1].sx = m_SkillBox_Xpos + 16;
		m_flame[0][1].sy = m_SkillBox_Ypos + 16;
		m_flame[1][1].sx = m_SkillBox_Xpos + 16;
		m_flame[1][1].sy = m_SkillBox_Ypos + 16;
		m_flame[2][1].sx = m_SkillBox_Xpos + 16;
		m_flame[2][1].sy = m_SkillBox_Ypos + 16;
		m_flame[3][1].sx = m_SkillBox_Xpos + 16;
		m_flame[3][1].sy = m_SkillBox_Ypos + 16;
		m_flame[4][1].sx = m_SkillBox_Xpos + 16;
		m_flame[4][1].sy = m_SkillBox_Ypos + 16;

		//-- ������ ���� ��.
		m_flame[0][0].sx = m_SkillBox_Xpos + 32;
		m_flame[0][0].sy = m_SkillBox_Ypos;
		m_flame[1][0].sx = m_SkillBox_Xpos + 32;
		m_flame[1][0].sy = m_SkillBox_Ypos +32;
		m_flame[2][0].sx = m_SkillBox_Xpos;
		m_flame[2][0].sy = m_SkillBox_Ypos +32;
		m_flame[3][0].sx = m_SkillBox_Xpos;
		m_flame[3][0].sy = m_SkillBox_Ypos;
		m_flame[4][0].sx = m_SkillBox_Xpos + 16;
		m_flame[4][0].sy = m_SkillBox_Ypos;

		//-- ��ȭ ��
		m_flame[0][2].sx = m_SkillBox_Xpos + 16;
		m_flame[0][2].sy = m_SkillBox_Ypos;
		m_flame[1][2].sx = m_SkillBox_Xpos + 32;
		m_flame[1][2].sy = m_SkillBox_Ypos;
		m_flame[2][2].sx = m_SkillBox_Xpos + 32;
		m_flame[2][2].sy = m_SkillBox_Ypos + 32;
		m_flame[3][2].sx = m_SkillBox_Xpos     ;
		m_flame[3][2].sy = m_SkillBox_Ypos + 32;
		m_flame[4][2].sx = m_SkillBox_Xpos     ;
		m_flame[4][2].sy = m_SkillBox_Ypos;

		int tmp_ang = (int(alpha + 45 ) % 90) * 32 / 90;
		float tmp_tc = (int(alpha + 45) % 90) / 90.0f;
		int tmp_zone = int(alpha + 45) / 90;

		//-- �ش� Zone �� Shadow �� ����.
		switch(tmp_zone)
		{
		case 0:
			m_flame[0][2].sx = m_SkillBox_Xpos + tmp_ang;
			m_flame[0][2].sy = m_SkillBox_Ypos;
			break;
		case 1:
			m_flame[1][2].sx = m_SkillBox_Xpos + 32;
			m_flame[1][2].sy = m_SkillBox_Ypos + tmp_ang;
			break;
		case 2:
			m_flame[2][2].sx = m_SkillBox_Xpos + 32 - tmp_ang;
			m_flame[2][2].sy = m_SkillBox_Ypos + 32;
			break;
		case 3:
			m_flame[3][2].sx = m_SkillBox_Xpos ;
			m_flame[3][2].sy = m_SkillBox_Ypos + 32 - tmp_ang;
			break;
		case 4:
			m_flame[4][2].sx = m_SkillBox_Xpos + tmp_ang ;
			m_flame[4][2].sy = m_SkillBox_Ypos;
			break;
		}

		//-- Render
		switch(tmp_zone)
		{
		case 0:
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX ,m_flame[0], 3, 0 );
		case 1:
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX ,m_flame[1], 3, 0 );
		case 2:
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX ,m_flame[2], 3, 0 );
		case 3:
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX ,m_flame[3], 3, 0 );
		case 4:
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX ,m_flame[4], 3, 0 );
		}


		//-- Restore Render State
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,   dwstate );
		GET_D3DDEVICE()->SetViewport(&m_vpOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,src_blend);//D3DBLEND_DESTCOLOR);// D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dest_blend);//D3DBLEND_SRCCOLOR
	}

	if (g_CursorPoint.x > m_SkillBox_Xpos && g_CursorPoint.x <= m_SkillBox_Xpos + m_pAttNormal->GetWidth()
			&& g_CursorPoint.y > m_SkillBox_Ypos && g_CursorPoint.y <= m_SkillBox_Ypos + m_pAttNormal->GetHeight() )
		return m_CurSkill;

	return SKILL_UNSELECTED;
}




LRESULT CR_SkillBox::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int i;

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if (IsInside(LOWORD (lParam), HIWORD (lParam)))
		{
			return 1;
		}
		else
		{
			if (m_bShowSkillBox)
			{
				for (i=0; i< g_ChaSkill.m_SkillNum; i++)
				{
					IsInside2(LOWORD (lParam), HIWORD (lParam), i);
					CheckNorSkillIcon(m_pAttNormal, LOWORD (lParam), HIWORD (lParam));
					CheckNorSkillIcon(m_pRotateNormal, LOWORD (lParam), HIWORD (lParam));
				}
				return 1;
			}
		}
		break;


	case WM_MOUSEMOVE:
		if (IsInside(LOWORD (lParam), HIWORD (lParam)) == TRUE)
			return 0;
		for (i=0; i< g_ChaSkill.m_SkillNum; i++)
		{
			IsInside2(LOWORD (lParam), HIWORD (lParam), i);
			CheckNorSkillIcon(m_pAttNormal, LOWORD (lParam), HIWORD (lParam));
			CheckNorSkillIcon(m_pRotateNormal, LOWORD (lParam), HIWORD (lParam));
		}

		break;

	case WM_LBUTTONUP:
		if( pCMyApp->m_pUIMgr->m_bRClickDown == 1 //mungmae-2006/03/30 ��ٿ� �߿� ���콺�� ��ų ���� �Ұ�
				&& pCMyApp->DrawCasting )
			return 0;
		if (IsInside(LOWORD (lParam), HIWORD (lParam)) == TRUE)
		{
			if (m_bShowSkillBox)
				m_bShowSkillBox = FALSE;
			else
				m_bShowSkillBox = TRUE;
		}
		else
		{
			if (m_bShowSkillBox)
			{

				for (i=0; i< g_ChaSkill.m_SkillNum; i++)
				{
					if (g_ChaSkill.m_SkillLevel[i] > 0 && g_ChaSkill.m_bRight[i]== TRUE)
					{

						if(IsInside2(LOWORD (lParam), HIWORD (lParam), i)== TRUE)
						{
							////////////////////1004

							//-- ��� ��ų
							//-- skill �ڽ����� �����ϴ� �� ������ Skill �� �ߵ��ȴ� .Bleed �߰�.
							if (i >= 0 && (g_Skills[g_ChaSkill.m_SkillIndex[i]].m_Kind == SKILL_KIND_ME) &&
									(g_ChaSkill.m_SkillIndex[i]==SKILL_PEACEFUL
									 || g_ChaSkill.m_SkillIndex[i]==SKILL_MELODY_OF_GUD
									 || g_ChaSkill.m_SkillIndex[i]==SKILL_THOUGHT_STORM ))
							{
								//���
								if (g_Skills[g_ChaSkill.m_SkillIndex[i]].m_WhichEnergy == SKILL_USE_MANA)
								{
									if (g_pRoh->m_Mana >= g_Skills[g_ChaSkill.m_SkillIndex[i]].GetEnerge0(g_ChaSkill.m_SkillLevel[i]) ||
											g_ChaSkill.m_SkillIndex[i]==SKILL_PEACEFUL )
									{	
										SendCharUseSkill(g_ChaSkill.m_SkillIndex[i]);
									}
								}
							}
							else if (g_Skills[g_ChaSkill.m_SkillIndex[i]].m_WhichEnergy == SKILL_USE_STAMINA
									 && (g_ChaSkill.m_SkillIndex[i]==SKILL_IRON_MUSCLE ||
										 g_ChaSkill.m_SkillIndex[i]==SKILL_FURY_EXPLOSION ||
										 g_ChaSkill.m_SkillIndex[i]==SKILLF_E_BLEED))
							{
								if (g_pRoh->m_Stamina >= g_Skills[g_ChaSkill.m_SkillIndex[i]].GetEnerge0(g_ChaSkill.m_SkillLevel[i]))
								{
									//  [4/30/2009 ppmmjj83] ���� ��ų�� ��հ˸� �����ϵ���
									if( g_dwClientCountry == CTRY_JPN )
									{
										if( g_pRoh->m_Wearing[WEARING_WEAPON] )
										{
											if( g_ChaSkill.m_SkillIndex[i]==SKILLF_E_BLEED
													&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_DUAL_SWORD )
											{
												// IDS_SKILL_BLEED_LIMIT : �ش� ��ų�� ��հ� ����� �����մϴ�.
												g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_SKILL_BLEED_LIMIT), -1);
												return 1;
											}
										}
										else // ������ü�� ���ٸ�
										{
											// IDS_SKILL_BLEED_LIMIT : �ش� ��ų�� ��հ� ����� �����մϴ�.
											g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_SKILL_BLEED_LIMIT), -1);
											return 1;
										}
									}									
									SendCharUseSkill(g_ChaSkill.m_SkillIndex[i]);

								}
							}
							else if (g_Skills[g_ChaSkill.m_SkillIndex[i]].m_WhichEnergy == SKILL_USE_EPOWER && g_ChaSkill.m_SkillIndex[i]==SKILL_PIERCING)
							{
								if (g_pRoh->m_Epower >= g_Skills[g_ChaSkill.m_SkillIndex[i]].GetEnerge0(g_ChaSkill.m_SkillLevel[i]))
								{	
									SendCharUseSkill(g_ChaSkill.m_SkillIndex[i]);
								}
							}
							else

								////////////////////
								m_CurSkill = i;
							m_bShowSkillBox = FALSE;
						}
					}
				}
				//  ��� ���ؾ�������  ���콺�� Ŭ���Ǿ��ٸ� m_CurSkill�� "��� ����"����
				if(CheckNorSkillIcon(m_pAttNormal, LOWORD (lParam), HIWORD (lParam)) == TRUE)
				{
					m_CurSkill = SELECTED_SKILL_ATTACT;
					m_bShowSkillBox = FALSE;
				}
				//  ������Ʈ�������� ���콺�� Ŭ���Ǿ��ٸ� m_CurSkill�� "������Ʈ"��
				if(CheckNorSkillIcon(m_pRotateNormal, LOWORD(lParam), HIWORD (lParam)) == TRUE)
				{
					m_CurSkill = SELECTED_SKILL_ROTATE;
					m_bShowSkillBox = FALSE;
				}
				m_bShowSkillBox = FALSE;
				return 1;
			}
		}

		break;
	}
	return 0;
}





// ȭ���� ����� üũ����ġ�� ũ�⸦ ���� ��ǥ /ũ�� ����
// �ε�� ����ȴ�
void CR_SkillBox::Con_RightSkillPos(int xpos, int ypos, int width, int height)
{
	m_SkillIconStartXpos = (R_START_XPOS);
	m_SkillIconStartYpos = (R_START_YPOS);

	m_SkillBox_Xpos = xpos;
	m_SkillBox_Ypos = ypos;
	m_SkillBox_Width = width;
	m_SkillBox_Height = height;
}






BOOL CR_SkillBox::IsInside(int x, int y)
{
	if ( ( ( x > m_SkillBox_Xpos) && ( x < (m_SkillBox_Xpos + m_SkillBox_Width)) )
			&& ( ( y > m_SkillBox_Ypos) && ( y < (m_SkillBox_Ypos + m_SkillBox_Height)) ) )
		return true;
	else
		return false;
}


BOOL CR_SkillBox::IsInside2(int x, int y, int index)
{
	return g_ChaSkill.IsInRskill( index, x, y );

	if ( ( ( x > g_ChaSkill.m_SkillIcon[index].PosX) && ( x < (g_ChaSkill.m_SkillIcon[index].PosX + g_ChaSkill.m_SkillIcon[0].Width)) )
			&& ( ( y > g_ChaSkill.m_SkillIcon[index].PosY) && ( y < (g_ChaSkill.m_SkillIcon[index].PosY + g_ChaSkill.m_SkillIcon[0].Height)) ) )
		return true;
	else
		return false;
}


BOOL CR_SkillBox::CheckNorSkillIcon(CSurface* surface, int x, int y)
{
	if ( ( ( x > surface->Xpos) && ( x < (surface->Xpos + surface->GetWidth())) )
			&& ( ( y > surface->Ypos) && ( y < (surface->Ypos + surface->GetHeight())) ) )
		return true;
	else
		return false;
}


void CR_SkillBox::SetPos(int x, int y)
{
	m_SkillBox_Xpos = x;
	m_SkillBox_Ypos = y;

	m_SkillIconStartXpos = x;
	m_SkillIconStartYpos = y-40;

	if( m_pRotateNormal )
	{
		m_pRotateNormal->Xpos = m_SkillIconStartXpos;
		m_pRotateNormal->Ypos = m_SkillIconStartYpos;
	}

	if( m_pAttNormal )
	{
		m_pAttNormal->Xpos = m_SkillIconStartXpos - m_pAttNormal->GetWidth();
		m_pAttNormal->Ypos = m_SkillIconStartYpos;
	}
}