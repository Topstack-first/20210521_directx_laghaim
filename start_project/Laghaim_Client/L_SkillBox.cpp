#include "stdafx.h"
//-------------------------------------------------------------
//		INCLUDE
//-------------------------------------------------------------

#include "dxutil.h"
#include "l_skillbox.h"
#include "skill.h"
#include "main.h"


extern POINT g_CursorPoint;



//-------------------------------------------------------------
//		DEFINE
//-------------------------------------------------------------
#define L_START_XPOS  173			// ��ų ������ ������ X ������ġ
#define L_START_YPOS  350			// ��ų ������ ������ Y ������ġ
#define L_CUR_SKILLBOX_XPOS 173		// ���ص� �������� ������ X ��ġ
#define L_CUR_SKILLBOX_YPOS 423		// ���ص� �������� ������ Y ��ġ
#define L_SOKET_CX  6               // ������ ���� ����
#define L_SOKET_CY  5				// ������ ���� ����
#define L_ICON_WIDTH 32				// �������� ����ũ��
#define L_ICON_HEIGHT 32            // �������� ����ũ��

//-------------------------------------------------------------
//		CONSTRUCTOR / DESTRUCTOR
//-------------------------------------------------------------
CL_SkillBox::CL_SkillBox()
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
	m_bShowSkillBox = FALSE;
	m_CurSkill = SELECTED_SKILL_ATTACT;
	m_pSur = NULL; // �⺻ ��ų(����)�������� ���� �׷����Ѵ�..
	m_pSur2 = NULL;

	//-- Skill Delay Shadow Box
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			m_flame[i][j].sz = 0.0f;
			m_flame[i][j].rhw = 1;
			m_flame[i][j].color = 0x88000000;
			if(j==1)m_flame[i][j].color = 0xee000000;
		}
	}
}

CL_SkillBox::~CL_SkillBox()
{
	DeleteRes();
}

//-------------------------------------------------------------
//		LOAD RESOURCE
//-------------------------------------------------------------
void CL_SkillBox::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_fStretchX = (m_ClientRc.right-m_ClientRc.left) / 640.0f ;
	m_fStretchY = (m_ClientRc.bottom-m_ClientRc.top) / 480.0f;

	if(m_ClientRc.right==640 && m_ClientRc.bottom==480)
	{
		m_SkillIconStartXpos = (173);	// ��ų ������ ������ X ������ġ
		m_SkillIconStartYpos = (350);
		m_SkillBox_Xpos = 173;			// ���õ� ��ų�� �����ش�
		m_SkillBox_Ypos = 423;
	}
	else if(m_ClientRc.right==800 && m_ClientRc.bottom==600)
	{
		m_SkillIconStartXpos = (253);	// ��ų ������ ������ X ������ġ
		m_SkillIconStartYpos = (491);
		m_SkillBox_Xpos = 253;			// ���õ� ��ų�� �����ش�
		m_SkillBox_Ypos = 542;
	}
	else if(m_ClientRc.right==1024 && m_ClientRc.bottom==768)
	{
		m_SkillIconStartXpos = (366);	// ��ų ������ ������ X ������ġ
		m_SkillIconStartYpos = (660);
		m_SkillBox_Xpos = 366;			// ���õ� ��ų�� �����ش�
		m_SkillBox_Ypos = 710;
	}

	m_SkillBox_Width = 32;
	m_SkillBox_Height = 32;

	// ��� ���� �̹����� ���� �ػ󵵿� ���� ũ�⸦ �����ؾ��Ѵ�
	int CX, CY;
	int GetPercent_Width = ((m_ClientRc.right)* 100) / 640 ;
	CX  = (((L_ICON_WIDTH) * (GetPercent_Width)) / 100);

	int GetPercent_Height= ((m_ClientRc.bottom)*100) / (480);
	CY  = (((L_ICON_HEIGHT) * (GetPercent_Height)) / 100);

	// �⺻���� ��ų�������̹����� �����Ѵ�..������� �϶� �׷���
	// normal �̹����� unable�̹����� �����Ѵ�
	// ������ �߱�..���°� ������� �Ѵٱ� �׷���
	g_pDisplay->CreateSurfaceFromBitmap( &m_pSur, "interface/skill/skill_0_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pSur2, "interface/skill/skill_0_unable.bmp");
	if( m_pSur )
		m_pSur->SetColorKey(RGB(0, 0, 0));
	if( m_pSur2 )
		m_pSur2->SetColorKey(RGB(0, 0, 0));
}


//-------------------------------------------------------------
//		DELETE
//-------------------------------------------------------------
void CL_SkillBox::DeleteRes()
{
	SAFE_DELETE(m_pSur);
	SAFE_DELETE(m_pSur2);
}



//-------------------------------------------------------------
//		DRAW
//-------------------------------------------------------------
int CL_SkillBox::Draw( void )
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
int CL_SkillBox::DrawAllSkillIcon()
{
	// ������ ��ų������ ��ġ�� �����ʺ��� �������� ü�����Ѵ�
	int x =1;  // �⺻��ų�������� ���� ó���� ������ �����ϹǷ� �� ������ ��ų �����ܵ��� �� ������ġ���� �׷��ֱ� �����Ѵ�
	int y =0;
	int over_skill = SKILL_UNSELECTED;

	if(m_bShowSkillBox == TRUE)
	{
		DrawNorSkillIcon();

		if ( g_CursorPoint.x > m_SkillIconStartXpos
				&& g_CursorPoint.x <= m_SkillIconStartXpos + m_pSur->GetWidth()
				&& g_CursorPoint.y > m_SkillIconStartYpos
				&& g_CursorPoint.y <= m_SkillIconStartYpos + m_pSur->GetHeight() )
			over_skill = SELECTED_SKILL_ATTACT;

		for (int index=0; index< g_ChaSkill.m_SkillNum; index++)
		{
			if (g_ChaSkill.m_SkillLevel[index] > 0 && g_ChaSkill.m_bLeft[index]== TRUE)
			{
				int icon_x = m_SkillIconStartXpos + (x * g_ChaSkill.m_SkillIcon[0].Width);
				int icon_y = m_SkillIconStartYpos + (y * g_ChaSkill.m_SkillIcon[0].Height);

				if (g_CursorPoint.x >= icon_x
						&& g_CursorPoint.x < icon_x + m_pSur->GetWidth()
						&& g_CursorPoint.y >= icon_y
						&& g_CursorPoint.y < icon_y + m_pSur->GetHeight())
					over_skill = index;

				g_ChaSkill.m_SkillIcon[index].SetState( BTN_NORMAL );
				g_ChaSkill.m_SkillIcon[index].Draw(icon_x, icon_y);
				g_ChaSkill.m_SkillIcon[index].PosX = icon_x;
				g_ChaSkill.m_SkillIcon[index].PosY = icon_y;

				g_ChaSkill.UpdateLskill( index, icon_x , icon_y );

				x++;
				if (x >= L_SOKET_CX)
				{
					x = 0 ;
					y--;
				}
			}
		}
	}
	return over_skill;
}

void CL_SkillBox::DrawNorSkillIcon()
{
	if( !g_pDisplay )
		return;

	// �� �⺻ ��ų �������� ��ġ�� �׻� ������ �����߿� ���� ó�� ��ġ�� ���ߴ�
	g_pDisplay->Blt(m_SkillIconStartXpos, m_SkillIconStartYpos, m_pSur, NULL);
	if( m_pSur )
	{
		m_pSur->Xpos = m_SkillIconStartXpos;
		m_pSur->Ypos = m_SkillIconStartYpos;
	}
}

//----------------------------------------------------------------------------
//-- ���õ� ��ų�������� �׸���
//----------------------------------------------------------------------------
//-- ���� �Ǵ��� ���� main �� conditioincheck �Լ��� �����Ѵ�.
int CL_SkillBox::DrawSeletedSkillIcon()
{
	static float alpha = 0;
	DWORD dwstate;

	if(m_CurSkill == SELECTED_SKILL_ATTACT)
		g_pDisplay->Blt(m_SkillBox_Xpos, m_SkillBox_Ypos, m_pSur, NULL);

	if(m_CurSkill>=0)
	{
		if(pCMyApp->SkillConditionCheck(m_CurSkill))
			g_ChaSkill.m_SkillIcon[m_CurSkill].SetState( BTN_NORMAL );
		else
			g_ChaSkill.m_SkillIcon[m_CurSkill].SetState( BTN_DISABLE );

		g_ChaSkill.m_SkillIcon[m_CurSkill].Draw(m_SkillBox_Xpos ,m_SkillBox_Ypos );
	}

	G_SK_RotateShadow();

	//-- Shadow set true
	if(m_CurSkill >= 0 && G_SK_Getflag( g_ChaSkill.m_SkillIndex[m_CurSkill] ))
	{
		alpha = G_SK_GetShdangle( g_ChaSkill.m_SkillIndex[m_CurSkill], g_ChaSkill.m_SkillLevel[m_CurSkill]);
		D3DVIEWPORT7 m_vpOld, m_vpNew;

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
	}

	if (g_CursorPoint.x > m_SkillBox_Xpos
			&& g_CursorPoint.x <= m_SkillBox_Xpos + m_pSur->GetWidth()
			&& g_CursorPoint.y > m_SkillBox_Ypos
			&& g_CursorPoint.y <= m_SkillBox_Ypos + m_pSur->GetHeight() )
		return m_CurSkill;

	return SKILL_UNSELECTED;
}

// ȭ���� ����� üũ����ġ�� ũ�⸦ ���� ��ǥ /ũ�� ����
// �ε�� ����ȴ�
void CL_SkillBox::Con_LeftSkillPos(int xpos, int ypos, int width, int height)
{
	int GetPercent_Width;
	int GetPercent_Height;
	int Image_Width;
	int Image_Height;
	int Image_Xpos;
	int Image_Ypos;

	GetPercent_Width = ((m_ClientRc.right-m_ClientRc.left)* 100) / 640 ;
	Image_Width  = ((width*(GetPercent_Width)) / 100);
	Image_Xpos	 = ((xpos)*(GetPercent_Width)) / (100);
	m_SkillIconStartXpos = ((L_START_XPOS)*(GetPercent_Width)) /100;

	GetPercent_Height= ((m_ClientRc.bottom-m_ClientRc.top)*100) / (480);
	Image_Height = ((height*(GetPercent_Height)) / (100));
	Image_Ypos	 = ((ypos)*(GetPercent_Height)) / (100);
	m_SkillIconStartYpos = ((L_START_YPOS)*(GetPercent_Height)) / (100);

	m_SkillBox_Xpos = Image_Xpos;
	m_SkillBox_Ypos = Image_Ypos;
	m_SkillBox_Width = Image_Width;
	m_SkillBox_Height = Image_Height;
}

LRESULT CL_SkillBox::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int i;
	BOOL bKKK = FALSE;

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
					IsInside2(LOWORD (lParam), HIWORD (lParam), i);		// ������ ���� ����üũ
					IsInside3(LOWORD (lParam), HIWORD (lParam));        // �⺻ ��ų������(����)
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
			IsInside3(LOWORD (lParam), HIWORD (lParam));
		}
		break;

	case WM_LBUTTONUP:
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
					if (g_ChaSkill.m_SkillLevel[i] > 0 && g_ChaSkill.m_bLeft[i]== TRUE)
					{
						if(IsInside2(LOWORD (lParam), HIWORD (lParam), i)== TRUE)// �׿��� ��ų ������ ������ üũ�Ѵ�
						{
							m_CurSkill = i;
							m_bShowSkillBox = FALSE;
						}
					}
				}
				if(IsInside3(LOWORD (lParam), HIWORD (lParam)) == TRUE)// ��� ���ؽ�ų ������ üũ�Ѵ�
				{
					m_CurSkill = SELECTED_SKILL_ATTACT;
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


BOOL CL_SkillBox::IsInside(int x, int y)
{
	if ( ( ( x > m_SkillBox_Xpos) && ( x < (m_SkillBox_Xpos + m_SkillBox_Width)) )
			&& ( ( y > m_SkillBox_Ypos) && ( y < (m_SkillBox_Ypos + m_SkillBox_Height)) ) )
		return true;
	else
		return false;
}


BOOL CL_SkillBox::IsInside2(int x, int y, int index)  // ������ ���Ϲڽ��� ����üũ
{
	return g_ChaSkill.IsInLskill( index, x, y );

	if ( ( ( x > g_ChaSkill.m_SkillIcon[index].PosX) && ( x < (g_ChaSkill.m_SkillIcon[index].PosX + g_ChaSkill.m_SkillIcon[0].Width)) )
			&& ( ( y > g_ChaSkill.m_SkillIcon[index].PosY) && ( y < (g_ChaSkill.m_SkillIcon[index].PosY + g_ChaSkill.m_SkillIcon[0].Height)) ) )
		return true;
	else
		return false;
}

BOOL CL_SkillBox::IsInside3(int x, int y) // �⺻��ų ������(����) ����üũ
{
	if( !m_pSur )
		return false;

	if ( ( ( x > m_pSur->Xpos) && ( x < (m_pSur->Xpos + m_pSur->GetWidth())) )
			&& ( ( y > m_pSur->Ypos) && ( y < (m_pSur->Ypos + m_pSur->GetHeight())) ) )
		return true;
	else
		return false;
}

void CL_SkillBox::SetPos(int x, int y)
{
	m_SkillBox_Xpos = x;
	m_SkillBox_Ypos = y;

	m_SkillIconStartXpos = x;
	m_SkillIconStartYpos = y - 40;

	if( m_pSur )
	{
		m_pSur->Xpos = m_SkillIconStartXpos;
		m_pSur->Ypos = m_SkillIconStartYpos;
	}
}