#include "stdafx.h"
#include "headers.h"
#include "main.h"
#include "ControlSkill.h"
#include "ddutil.h"
#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "Skill.h"
#include "TextOutBox.h"
#include "TcpIpCon.h"
#include "AlphaPopUp.h"
#include "g_stringmanager.h"
#include "CommonConfig.h"

extern int				g_SkillsNum;
extern CRaceSkill		g_RaceSkill[];

extern char				commOutBuf[];
extern HINSTANCE		g_hDllInst;

extern int GetMultiShot(int skill_level);

CControlSkill::CControlSkill()
	: m_hWnd(NULL)
	, m_pNkCha(NULL)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_skilliconstart_x(0)
	, m_skilliconstart_y(0)
	, m_levelfontstart_x(0)
	, m_levelfontstart_y(0)
{
	m_pSurfaceBack = NULL;
	m_pSkillInfo = new CAlphaPopUp;
	if( m_pSkillInfo )
		m_pSkillInfo->SetIndex(-100);
	Init();
}

CControlSkill::~CControlSkill()
{
	DeleteRes();
	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE( m_pSkillInfo );

	m_pNkCha = NULL;
}

HRESULT CControlSkill::RestoreSurfaces()
{
	if(g_pDisplay == NULL)				// 제대로 된 에러처리 같지는 않다.
		return E_FAIL;

	for (int i = 0; i < g_ChaSkill.m_SkillNum; i++)
		g_ChaSkill.m_LevelText[i].SetString(g_ChaSkill.m_SkillLevel[i]);

	SetLevelupPoint();

	return S_OK;
}

void CControlSkill::Init()
{
	if( m_pSkillInfo )
		m_pSkillInfo->Init(14);
}

void CControlSkill::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);
	if( g_pNk2DFrame )
	{
		m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
		m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	if(g_pRoh)
		g_ChaSkill.LoadRes(SKILL_ICON_WIDTH, SKILL_ICON_HEIGHT);

	if( m_pSkillInfo )
		m_pSkillInfo->LoadRes(m_ScreenWidth, m_ScreenHeight);

	SAFE_DELETE(m_pSurfaceBack);

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/skill/menu_skill.bmp") ) )
		return;
	m_pSurfaceBack->SetColorKey( TRANS_COLOR );

	m_CloseBtn.Init("common/btn_close_01");

	m_BtnWindow_1.Init("common/btn_tab_01");
	m_BtnWindow_2.Init("common/btn_tab_02");
	m_BtnWindow_1.SetState(BTN_DOWN);

	m_pSurfaceBack->Xpos = m_ScreenWidth - m_pSurfaceBack->GetWidth();
	m_pSurfaceBack->Ypos = 0;

	m_LevelupPoint.Init(14, RGB(0, 255, 255), m_pSurfaceBack->Xpos+127, m_pSurfaceBack->Ypos+392+4, -1, -1, TRUE);
	m_CloseBtn.SetPosition(m_pSurfaceBack->Xpos+257, m_pSurfaceBack->Ypos+5);
	///////2003 스킬 플립버튼 위치
	m_BtnWindow_1.SetPosition(m_pSurfaceBack->Xpos+17, m_pSurfaceBack->Ypos+66);
	m_BtnWindow_2.SetPosition(m_pSurfaceBack->Xpos+96, m_pSurfaceBack->Ypos+66);


	m_skilliconstart_x = m_pSurfaceBack->Xpos+44;
	m_skilliconstart_y = m_pSurfaceBack->Ypos+137;
	m_levelfontstart_x = m_pSurfaceBack->Xpos+41;
	m_levelfontstart_y = m_pSurfaceBack->Ypos+117;


	if(g_pRoh)
		SetLevelupPoint();
}

void CControlSkill::DeleteRes()
{
	m_CloseBtn.DeleteRes();
	m_BtnWindow_1.DeleteRes();
	m_BtnWindow_2.DeleteRes();
	g_ChaSkill.DeleteRes();
	m_LevelupPoint.DeleteRes();

	SAFE_SURFACE_DESTROY(m_pSurfaceBack);
	SAFE_DELETE_RES( m_pSkillInfo );

	if( m_pSkillInfo )
		m_pSkillInfo->SetIndex(-100);
}

int CControlSkill::Draw()
{
	if(g_pDisplay && m_pSurfaceBack)
	{
		if( m_pSurfaceBack )
			g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		int over_skill = SKILL_UNSELECTED;
		int plusOne = 0, icon_x = 0, icon_y = 0, level_x = 0, level_y = 0;

		//----------------------------------------------------------------------
		// 왼쪽 창
		if(m_BtnWindow_1.GetState() == BTN_DOWN)
		{
			for (int i = 0; i < g_ChaSkill.m_SkillNum; i++)
			{
				if(g_ChaSkill.m_SkillIndex[i] < 47
						|| ( g_ChaSkill.m_SkillIndex[i] > 52 && g_ChaSkill.m_SkillIndex[i] < 60)
						|| ( g_ChaSkill.m_SkillIndex[i] > 78 && g_ChaSkill.m_SkillIndex[i] < 89)
						|| ( g_ChaSkill.m_SkillIndex[i] == 111 || g_ChaSkill.m_SkillIndex[i] == 117 )
						|| ( g_ChaSkill.m_SkillIndex[i] >= 119 && g_ChaSkill.m_SkillIndex[i] <= 123 )
						|| IsTabSkill(0, g_ChaSkill.m_SkillIndex[i] )
				  )
				{
					if(g_ChaSkill.m_SkillIndex[i] == SKILL_STAR_FLARE)
						plusOne = 1;

					// 쉐도우 슬래쉬와 아드레날린(둘다 이도류)스킬은 왼쪽으로 스킬이 옮겨져야 한다.
					// 기존 코딩에서는 인덱스 89에서 BREAK시켜서 나중에 나온
					// 쉐도우 슬래쉬와 아드레 날린이 오른쪽으로가 있었던 걸 수정하였다.
					if( g_ChaSkill.m_SkillIndex[i] == 111 )
					{
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((10+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((10+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((10+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((10+plusOne) / 4);
					}
					else if( g_ChaSkill.m_SkillIndex[i] == 117 )
					{
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((11+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((11+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((11+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((11+plusOne) / 4);
					}
					else if( g_ChaSkill.m_SkillIndex[i] == 119 )
					{
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((11+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((11+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((11+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((11+plusOne) / 4);
					}
					else if( g_ChaSkill.m_SkillIndex[i] == 120 )
					{
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((10+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((10+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((10+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((10+plusOne) / 4);
					}
					else if( g_ChaSkill.m_SkillIndex[i] == 121 )
					{
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((11+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((11+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((11+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((11+plusOne) / 4);
					}
					else if( g_ChaSkill.m_SkillIndex[i] == 122 )
					{
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((12+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((12+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((12+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((12+plusOne) / 4);
					}
					else if( g_ChaSkill.m_SkillIndex[i] == 123 )
					{
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((12+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((12+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((12+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((12+plusOne) / 4);
					}
					else
					{
						//mungmae 2006/02/23: 쉐도우 슬레쉬, 아드레날린이외의 스킬은 일반적으로 처리
						icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * ((i+plusOne) % 4);
						icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * ((i+plusOne) / 4);
						level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * ((i+plusOne) % 4);
						level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * ((i+plusOne) / 4);
					}

					if( over_skill < 0
							&& point.x >= icon_x
							&& point.x < icon_x + SKILL_ICON_WIDTH
							&& point.y >= icon_y
							&& point.y < icon_y + SKILL_ICON_HEIGHT )
						over_skill = i;

					g_ChaSkill.m_SkillIcon[i].Draw(icon_x, icon_y);
					//g_ChaSkill.m_LevelText[i].SetPos(level_x-3, level_y+5);
					g_ChaSkill.m_LevelText[i].Draw(level_x-3, level_y+5 , 40);
				}
			}
		}
		//----------------------------------------------------------------------
		// 오른쪽 창
		else if(m_BtnWindow_2.GetState() == BTN_DOWN)
		{
			m_skilliconstart_x = m_pSurfaceBack->Xpos+44;
			m_skilliconstart_y = m_pSurfaceBack->Ypos+137;

			int skillcnt = 0;

			for (int i = 0; i < g_ChaSkill.m_SkillNum; i++)
			{
				if(( g_ChaSkill.m_SkillIndex[i] >= 47 && g_ChaSkill.m_SkillIndex[i] <= 52 )
						|| ( g_ChaSkill.m_SkillIndex[i] >= 60 && g_ChaSkill.m_SkillIndex[i] <= 78 )
						|| ( g_ChaSkill.m_SkillIndex[i] >= 89 && g_ChaSkill.m_SkillIndex[i] <= 110 )
						|| ( g_ChaSkill.m_SkillIndex[i] >= 112 && g_ChaSkill.m_SkillIndex[i] <= 116 )
						|| ( g_ChaSkill.m_SkillIndex[i] == 118 || g_ChaSkill.m_SkillIndex[i] == 124  )
						|| IsTabSkill(1, g_ChaSkill.m_SkillIndex[i] )
				  )
				{
					int icon_x = m_skilliconstart_x + SKILL_ICON_X_GAP * (skillcnt % 4);
					int icon_y = m_skilliconstart_y + SKILL_ICON_Y_GAP * (skillcnt / 4);

					g_ChaSkill.m_SkillIcon[i].Draw(icon_x, icon_y);
					if(over_skill < 0
							&& point.x >= icon_x
							&& point.x < icon_x + SKILL_ICON_WIDTH
							&& point.y >= icon_y
							&& point.y < icon_y + SKILL_ICON_HEIGHT)
						over_skill = i;

					int level_x = m_levelfontstart_x + SKILL_ICON_X_GAP * (skillcnt % 4);
					int level_y = m_levelfontstart_y + SKILL_ICON_Y_GAP * (skillcnt / 4);

					//g_ChaSkill.m_LevelText[i].SetPos(level_x-3, level_y+5);
					g_ChaSkill.m_LevelText[i].Draw(level_x-3, level_y+5 , 40);

					skillcnt++;
				}
			}
		}

		m_LevelupPoint.Draw(m_pSurfaceBack->Xpos+127, m_pSurfaceBack->Ypos+395 , 108);
		m_CloseBtn.Draw();

		m_BtnWindow_1.Draw();
		m_BtnWindow_2.Draw();

		return over_skill;
	}

	return SKILL_UNSELECTED;
}

void CControlSkill::DrawSkillInfo(int over_skill)
// nate 2005-11-16 : 스킬정보 출력부 수정
// 확인 결과 A4지 90매에 달하는 분량의 소스코딩으로 중복 코드가 많았음
// 정리1 : 스트링을 중심으로 스킬 정리
{
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( g_pDisplay->GetHWnd(), &point );

	if (over_skill < 0)
		over_skill--;

	if( over_skill != m_pSkillInfo->GetIndex() )	// 같은 스킬위에 있는동안은 갱신 안함
	{
		char strTemp[ 128 ] = "";
		CMultiText multiText;

		if( over_skill < 0 )
		{
			if(over_skill == SELECTED_SKILL_ATTACT - 1)
				// 일반 공격
				multiText.AddString((char*)G_STRING(IDS_SKILL_NOR_ATT), _FCOLOR_YELLOW);
			else if(over_skill == SELECTED_SKILL_ROTATE - 1)
				// 화면 회전
				multiText.AddString((char*)G_STRING(IDS_SKILL_SCREEN_ROTATE), _FCOLOR_YELLOW);
		}
		else
		{
			int nSkill_Index = g_ChaSkill.m_SkillIndex[over_skill];
			int nSkill_Level = g_ChaSkill.m_SkillLevel[over_skill];

			multiText.AddString((char*)g_Skills[nSkill_Index].GetName(), _FCOLOR_YELLOW);				// 스킬 이름
			multiText.AddStringMulti((char*)g_Skills[nSkill_Index].GetDesc(), 26, 1, _FCOLOR_WHITE);	// 스킬 설명


			if(DrawDiableSkill(nSkill_Index))	// 사용할수 없는 스킬
			{
				multiText.AddStringMulti((char*)G_STRING(IDS_USE_NOT_SKILL), 20, 1, _FCOLOR_RED);
				m_pSkillInfo->PutString(&multiText);
				m_pSkillInfo->SetIndex(over_skill);
				m_pSkillInfo->Draw(point.x, point.y);
				return;
			}

			switch( nSkill_Index )
			{
			case SKILLF_E_REFLECTION:
			case SKILLF_E_CLOAKING:
			case SKILL_B_BURNINGCRASH:
			case SKILL_B_BURNING_CONTROL:
			case SKILL_C_HELLDOWN:
			case SKILL_C_DOWN_CONTROL:
			case SKILL_A_SUMMONS_SQUAD:
			case SKILL_A_LIFE_EXPANSION:
			case SKILL_H_MINE_CONTROL:
			case SKILL_H_AUTOMINE:
			case SKILL_B_FLYINGCRASH:
			case SKILL_K_POISON:
			case SKILL_H_ELECTRICSHOT:
			case SKILL_A_SUMMON_FLOWER:
			case SKILL_F_SHADOWSLASH:
			case SKILL_F_ADRENALINE:
				if(g_Skills[nSkill_Index].m_MinLevel > g_pRoh->m_Level
						&& nSkill_Level < g_Skills[nSkill_Index].m_MaxLevel)
					// 스킬업요구레벨: %d
					sprintf(strTemp, G_STRING(IDS_SKILL_REQUILE_LEVEL_SKILL), g_Skills[nSkill_Index].m_MinLevel );
				break;
			default:
				if(g_Skills[nSkill_Index].m_MinLevel + nSkill_Level > g_pRoh->m_Level
						&& nSkill_Level < g_Skills[nSkill_Index].m_MaxLevel)
					// 스킬업요구레벨: %d
					sprintf(strTemp, G_STRING(IDS_SKILL_REQUILE_LEVEL_SKILL), g_Skills[nSkill_Index].m_MinLevel + nSkill_Level);
				break;
			}
			multiText.AddString(strTemp, _FCOLOR_RED);

			// 스킬레벨: %d
			sprintf(strTemp, G_STRING(IDS_SKILL_SKILL_LEVEL), nSkill_Level);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);

			for( int i = 0 ; i < 2 ; i ++ )	// 현재 스킬과 다음 스킬의 2번 출력
			{
				if( (!i && nSkill_Level > 0)										// 현재 스킬 정보
						|| (i && nSkill_Level < g_Skills[nSkill_Index].m_MaxLevel) )	// 다음 스킬 정보
				{

					if( g_pRoh->m_Race == RACE_PEROM )
						SetSkillInfo_Perom( multiText, nSkill_Index, nSkill_Level + i, i );
					else
						SetSkillInfo( multiText, nSkill_Index, nSkill_Level + i, i );
				}

				if( !i && nSkill_Level < g_Skills[nSkill_Index].m_MaxLevel )		// 다음 스킬 레벨
				{
					//if( nSkill_Index != SKILL_A_LIFE_EXPANSION || nSkill_Index != SKILL_A_SUMMONS_SQUAD)
					{
						multiText.AddString(" ", _FCOLOR_YELLOW);
						// IDS_SKILL_NEXT_SKILL_LEVEL : 다음 스킬레벨: %d
						sprintf(strTemp, G_STRING(IDS_SKILL_NEXT_SKILL_LEVEL), nSkill_Level+1);
						multiText.AddString(strTemp, _FCOLOR_YELLOW);
					}
				}
			}

			// 요구 조건이 필요한 Skill
			if (nSkill_Level < g_Skills[nSkill_Index].m_MaxLevel)
			{
				char strTemp[ 128 ] = "";
				multiText.AddString(" ", _FCOLOR_YELLOW);

				switch( nSkill_Index )
				{
				case SKILL_CHAIN_ATTACK:
					sprintf(strTemp, G_STRING(IDS_NEED_1));
					break;
				case SKILL_MELODY_OF_GUD:
				case SKILL_PASSION_OF_MELODY:
					sprintf(strTemp, G_STRING(IDS_NEED_2));
					break;
				case SKILL_PASSION_OF_FIGHT:
					sprintf(strTemp, G_STRING(IDS_NEED_3));
					break;
				case SKILL_VANPIRE_TOUCH:
					sprintf(strTemp, G_STRING(IDS_NEED_4));
					break;
				case SKILL_HELL_TOUCH:
					sprintf(strTemp, G_STRING(IDS_NEED_5));
					break;
				case SKILL_FORCE_CONTROL:
					sprintf(strTemp, G_STRING(IDS_NEED_6));
					break;
				case SKILLF_P_HASTE:
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),
							g_Skills[g_ChaSkill.m_SkillIndex[over_skill-1]].GetName(), 5);
					break;
				case SKILLF_P_MASERT_HEAVY:
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),
							g_Skills[g_ChaSkill.m_SkillIndex[over_skill-1]].GetName(), 5 );
					break;
				case SKILLF_A_BASH:
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),
							g_Skills[g_ChaSkill.m_SkillIndex[over_skill-2]].GetName(), 5 ); // 2 이전 스킬 필요 구문
					break;
				case SKILLF_A_DSLASH:
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),
							g_Skills[g_ChaSkill.m_SkillIndex[over_skill-2]].GetName(), 5 ); // 2 이전 스킬 필요 구문
					break;
				case SKILLF_P_EVASION:
				case SKILLF_A_DASH:
				case SKILLF_A_ASSAULT:
				case SKILLF_P_IRONSKIN:
				case SKILLF_P_STONRINC:
				case SKILLF_A_TIMECHARGE:
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL), g_Skills[g_ChaSkill.m_SkillIndex[over_skill-2]].GetName(), 5);
					break;
				case SKILLF_P_REC_UP:
				case SKILLF_E_BLEED:
				case SKILLF_C_DASSAULT:
				case SKILLF_E_CLOAKING:
				case SKILLF_P_VITAL_UP:
				case SKILLF_A_DSTONE:
				case SKILLF_C_STONEBRUST:
				case SKILLF_E_REFLECTION:
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL), g_Skills[g_ChaSkill.m_SkillIndex[over_skill-1]].GetName(), 5);
					break;
				case PEROM_SKILL_S129: // 128-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[128].GetName(), 10 );
					break;
				case PEROM_SKILL_S130: // 127-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[127].GetName(), 10 );
					break;
				case PEROM_SKILL_S132: // 135-15
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[135].GetName(), 15 );
					break;
				case PEROM_SKILL_S133: // 131-15,148-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[131].GetName(), 15 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[148].GetName(), 10 );
					break;
				case PEROM_SKILL_S134: // 141-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[126].GetName(), 15 );
					break;
				case PEROM_SKILL_S135: // 131-10,129-15
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[131].GetName(), 10 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[129].GetName(), 15 );
					break;
				case PEROM_SKILL_S136: // 133-10,130-15
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[133].GetName(), 10 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[130].GetName(), 15 );
					break;
				case PEROM_SKILL_S137: // 127-15,129-25
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[127].GetName(), 15 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[129].GetName(), 25 );
					break;
				case PEROM_SKILL_S138: // 137-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[137].GetName(), 10 );
					break;
				case PEROM_SKILL_S140: // 139-15 141-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[139].GetName(), 15 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[141].GetName(), 10 );
					break;
				case PEROM_SKILL_S144: // 140-10 142-5 143-5
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[140].GetName(), 10 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[142].GetName(), 5 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[143].GetName(), 5 );
					break;
				case PEROM_SKILL_S145: // 139-10 126-20
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[139].GetName(), 10 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[126].GetName(), 20 );
					break;
				case PEROM_SKILL_S146: // 139-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[139].GetName(), 10 );
					break;
				case PEROM_SKILL_S147: // 139-10 149-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[139].GetName(), 10 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[149].GetName(), 10 );
					break;
				case PEROM_SKILL_S150: // 134-10 139-10
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[134].GetName(), 10 );
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
					sprintf(strTemp, G_STRING(IDS_NEED_SKILL),g_Skills[139].GetName(), 10 );
					break;
				}

				if( strlen(strTemp) )
					multiText.AddString(strTemp, _FCOLOR_YELLOW);
				switch( nSkill_Index )
				{
				case SKILLF_P_HASTE:
					// %s 0 조건
					sprintf(strTemp, G_STRING(IDS_MUST_SKILL_1), g_Skills[SKILLF_P_MASERT_HEAVY].GetName() ); // 헤비마스터리 0 조건
					multiText.AddString(strTemp, _FCOLOR_RED);
					break;
				case SKILLF_P_MASERT_HEAVY:
					// %s 0 조건
					sprintf(strTemp, G_STRING(IDS_MUST_SKILL_1), g_Skills[SKILLF_P_HASTE].GetName() ); // 헤비마스터리 0 조건
					multiText.AddString(strTemp, _FCOLOR_RED);
					break;
				case SKILLF_A_BASH:
					// %s 0 조건
					sprintf(strTemp, G_STRING(IDS_MUST_SKILL_1), g_Skills[SKILLF_A_DSLASH].GetName() ); // 더블 슬래쉬 0 조건
					multiText.AddString(strTemp, _FCOLOR_RED);
					break;
				case SKILLF_A_DSLASH:
					// %s 0 조건
					sprintf(strTemp, G_STRING(IDS_MUST_SKILL_1), g_Skills[SKILLF_A_BASH].GetName() ); // 배쉬 0 조건
					multiText.AddString(strTemp, _FCOLOR_RED);
					break;
				}
				// 최고스킬레벨: %d
				sprintf(strTemp, G_STRING(IDS_SKILL_MAX_SKILL_LEVEL ), g_Skills[nSkill_Index].m_MaxLevel);
				multiText.AddString(strTemp, _FCOLOR_YELLOW);
			}

		}
		m_pSkillInfo->PutString(&multiText);
		m_pSkillInfo->SetIndex(over_skill);
	}
	m_pSkillInfo->Draw(point.x, point.y);
}

bool CControlSkill::DrawDiableSkill( int nSkill_Index )
{
	switch( nSkill_Index )
	{
	case SKILL_F_SHADOWSLASH:
	case SKILL_B_FLYINGCRASH:
	case SKILL_TELEPORT:
	case SKILLF_A_DASH:
		if( pCMyApp->IsAcropolis() == 1 )
			return true;
		break;
	case SKILL_PEACEFUL:
	case SKILL_SUMMON_PARTY:
	case SKILLF_E_CLOAKING:
		if( pCMyApp->IsAcropolis() )
			return true;
		break;
	}
	return false;
}

void CControlSkill::SetSkillInfo( CMultiText &multiText, int nSkill_Index, int nSkill_Level, int nCount )
{
	int nTemp = 0;
	char strTemp[ 128 ] = "";

	// 회피율 %d%%상승
	if( nSkill_Index == SKILLF_P_EVASION )
	{
		sprintf( strTemp, G_STRING(IDS_AVOID_UP), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}

	// 스테미너 %d%% 상승
	if(nSkill_Index == SKILL_B_BURNING_CONTROL)
	{
		sprintf(strTemp, G_STRING(IDS_EVRB_OPT_STA), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}

	// 스테미너 소모: %d
	if( nSkill_Index == SKILLF_E_CLOAKING
			|| nSkill_Index == SKILLF_E_BLEED
			|| nSkill_Index == SKILL_FORCE_ATTACK
			|| nSkill_Index == SKILL_B_BURNINGCRASH
			|| nSkill_Index == SKILL_HELL_CRY
			|| nSkill_Index == SKILL_DROP_ATTACK
			|| nSkill_Index == SKILL_ROTATE_ATTACK
			|| nSkill_Index == SKILLF_A_DSLASH
			|| nSkill_Index == SKILLF_A_ASSAULT
			|| nSkill_Index == SKILLF_C_DASSAULT
			|| nSkill_Index == SKILL_B_FLYINGCRASH   //mungmae-2005/11/24 플라잉 크래쉬
			|| nSkill_Index == SKILL_F_SHADOWSLASH //mungmae-2005/11/24 쉐도우 슬래쉬
			|| nSkill_Index == SKILL_B_WILDSMASH // 불칸 - 와일드 스매쉬
			|| nSkill_Index == SKILL_F_SPACESLASH	   // 하이 - 스페이스 슬래
			|| nSkill_Index == SKILL_B_PROTECTION_SHIELDS
			|| nSkill_Index == SKILL_F_MERCERNERY_SLASH )
	{
		sprintf( strTemp, G_STRING( IDS_INVEN_USE_STAMINA ), g_Skills[nSkill_Index].GetEnerge0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 마나소모: %d
	if(nSkill_Index == SKILLF_A_BASH
			|| nSkill_Index == SKILL_A_LIFE_EXPANSION
			|| nSkill_Index == SKILL_A_SUMMONS_SQUAD
			|| nSkill_Index == SKILL_TORNADO_RUIN
			|| nSkill_Index == SKILL_SUMMON_PARTITION
			|| nSkill_Index == SKILL_K_POISON
			|| nSkill_Index == SKILL_LIGHTNING
			|| nSkill_Index == SKILL_FIRE_PILLAR
			|| nSkill_Index == SKILL_FALL_STONE
			|| nSkill_Index == SKILL_STAR_FLARE
			|| nSkill_Index == SKILL_SHOCK_SPEAR
			|| nSkill_Index == SKILL_STONE_LANCE
			|| nSkill_Index == SKILL_C_HELLDOWN
			|| nSkill_Index == SKILL_PEBBLE_SHOT
			|| nSkill_Index == SKILL_LIGHTNING_BALL
			|| nSkill_Index == SKILL_FIRE_BALL
			|| nSkill_Index == SKILL_HEAL_SELF
			|| nSkill_Index == SKILL_MANA_SHIELD
			|| nSkill_Index == SKILL_F_ADRENALINE
			|| nSkill_Index == SKILLF_E_REFLECTION // 06-07-06 리플렉션 마나 소모 표시 추가.
			|| nSkill_Index == SKILL_K_POISON
			|| nSkill_Index == SKILL_K_FLAMESPEAR // 카이 - 프레임 스피어
			|| nSkill_Index == SKILL_A_SUMMONFIRE // 에이디아 - 서먼 파이어
			|| nSkill_Index == SKILL_K_ICEBOLT
			|| nSkill_Index == SKILL_A_RESURRECTION
			|| nSkill_Index == SKILL_F_ELECTRICHAMMER // 하이 - 일렉트로닉 해머
			|| nSkill_Index == SKILL_F_TORNADO_HAMMER )
	{
		sprintf(strTemp,G_STRING(IDS_INVEN_USE_MANA),g_Skills[nSkill_Index].GetEnerge0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// %d%% 마나소모 감소
	if( nSkill_Index == SKILL_CONCENTRATION)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_DEC_MANA_USE), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 전기력소모: %d
	if( nSkill_Index == SKILL_AIR_RAID
			|| nSkill_Index == SKILL_H_AUTOMINE
			|| nSkill_Index == SKILL_H_ELECTRICSHOT
			|| nSkill_Index == SKILL_H_RANDOMFIRE
			|| nSkill_Index == SKILL_H_FROZENSHOT )
	{
		sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), g_Skills[nSkill_Index].GetEnerge0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}

	// 전기력 소모 : +%d%%
	if(nSkill_Index == SKILL_SNIFING)
	{
		sprintf(strTemp, G_STRING(IDS_EPOWER_PERSENT), (g_Skills[nSkill_Index].GetApp0(nSkill_Level) - 10) * 10);
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 초기 전기력 소모: %d
	if(nSkill_Index == SKILL_HIDE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_WASTE_EPOWER), g_Skills[nSkill_Index].GetEnerge0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 10 초당 전기력 소모: %d
	if(nSkill_Index == SKILL_HIDE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_10SEC_WASTE_EPOWER), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 생명소모량 : %d%%
	if(nSkill_Index == SKILL_BLOODY_MANA)
	{
		sprintf(strTemp,G_STRING(IDS_OPTION_4), g_Skills[nSkill_Index].GetEnerge0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 생명력 흡수 : %d%%
	if(nSkill_Index == SKILL_VANPIRE_TOUCH)
	{
		sprintf(strTemp, G_STRING(IDS_OPTION_6), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 마나전환률 : %d%%
	if(nSkill_Index == SKILL_BLOODY_MANA)
	{
		sprintf(strTemp,G_STRING(IDS_OPTION_5), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 데미지 전환 : %d%%
	if(nSkill_Index == SKILL_HELL_TOUCH)
	{
		sprintf(strTemp, G_STRING(IDS_OPTION_7), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 데미지: +%d%%
	if(nSkill_Index == SKILLF_A_BASH
			|| nSkill_Index == SKILLF_C_STONEBRUST
			|| nSkill_Index == SKILL_HELL_CRY
			|| nSkill_Index == SKILL_TORNADO_RUIN
			|| nSkill_Index == SKILL_AIR_RAID
			|| nSkill_Index == SKILL_SUMMON_PARTITION
			|| nSkill_Index == SKILL_K_POISON
			|| nSkill_Index == SKILL_DROP_ATTACK
			|| nSkill_Index == SKILL_ROTATE_ATTACK
			|| nSkill_Index == SKILLF_A_DSLASH
			|| nSkill_Index == SKILLF_A_ASSAULT
			|| nSkill_Index == SKILLF_C_DASSAULT
			|| nSkill_Index == SKILL_PASSION_OF_FIGHT
			|| nSkill_Index == SKILLF_P_MASTER_DSWORD
			|| nSkill_Index == SKILLF_P_HASTE
			|| nSkill_Index == SKILLF_P_MASET_HAMMER
			|| nSkill_Index == SKILLF_P_MASERT_HEAVY
			|| nSkill_Index == SKILL_FIRE_MASTERY
			|| nSkill_Index == SKILL_LIGHTNING_MASTERY
			|| nSkill_Index == SKILL_STONE_MASTERY
			|| nSkill_Index == SKILL_TORNADO_OPERATION
			|| nSkill_Index == SKILL_SWORD_MASTERY
			|| nSkill_Index == SKILL_AXE_MASTERY
			|| nSkill_Index == SKILL_HELL_CRY_CONTROLL
			|| nSkill_Index == SKILL_GUN_MASTERY
			|| nSkill_Index == SKILL_AIR_RAID_CONCENT
			|| nSkill_Index == SKILL_ENERGYBALL
			|| nSkill_Index == SKILL_ANIMAL_SUMMON
			|| nSkill_Index == SKILL_PHENOMENON
			|| nSkill_Index == SKILL_SUMMON_CONTROL
			|| nSkill_Index == SKILL_A_SUMMON_FLOWER
			|| nSkill_Index == SKILL_K_POISON
			|| nSkill_Index == SKILL_H_ELECTRICSHOT
			|| nSkill_Index == SKILL_B_FLYINGCRASH   //mungmae-2005/11/24 플라잉 크래쉬
			|| nSkill_Index == SKILL_F_SHADOWSLASH   //mungmae-2005/11/24 쉐도우 슬래쉬
			|| nSkill_Index == SKILL_B_WILDSMASH
			|| nSkill_Index == SKILL_K_FLAMESPEAR
			|| nSkill_Index == SKILL_H_RANDOMFIRE
			|| nSkill_Index == SKILL_A_SUMMONFIRE
			|| nSkill_Index == SKILL_F_SPACESLASH
			|| nSkill_Index == SKILL_F_ELECTRICHAMMER
			|| nSkill_Index == SKILL_K_ICEBOLT
			|| nSkill_Index == SKILL_H_FROZENSHOT
			|| nSkill_Index == SKILL_F_MERCERNERY_SLASH
			|| nSkill_Index == SKILL_F_TORNADO_HAMMER
	  )
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_STA_AND_DAMAGE),g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 데미지:%d
	if( nSkill_Index == SKILL_PEBBLE_SHOT
			|| nSkill_Index == SKILL_LIGHTNING_BALL
			|| nSkill_Index == SKILL_FIRE_BALL)
	{
		sprintf(strTemp, G_STRING(IDS_DAMAGE),g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 데미지 흡수 %d%% 상승
	if( nSkill_Index == SKILL_MANA_SHIELD
			|| nSkill_Index == SKILL_ELECTRIC_SHIELD
			|| nSkill_Index == SKILL_IRON_SKIN
			|| nSkill_Index == SKILLF_P_IRONSKIN)
	{
		sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_ABS_DAMAGE),g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 물리데미지실드 +%d%%
	if( nSkill_Index == SKILL_MANA_SHIELD
			|| nSkill_Index == SKILL_ELECTRIC_SHIELD)
	{
		sprintf(strTemp, G_STRING(IDS_INVEN_PHYSCAL_DAM),g_Skills[nSkill_Index].GetApp1(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 데미지: %d + %d
	if( nSkill_Index == SKILL_LIGHTNING
			|| nSkill_Index == SKILL_FIRE_PILLAR
			|| nSkill_Index == SKILL_FALL_STONE
			|| nSkill_Index == SKILL_STAR_FLARE
			|| nSkill_Index == SKILL_SHOCK_SPEAR
			|| nSkill_Index == SKILL_STONE_LANCE)
	{
		switch (nSkill_Index)
		{
		case SKILL_LIGHTNING:
			nTemp = (g_Skills[SKILL_LIGHTNING_BALL].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_LIGHTNING_BALL)) * 60 / 100);
			break;
		case SKILL_FIRE_PILLAR:
			nTemp = (g_Skills[SKILL_FIRE_BALL].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_FIRE_BALL)) * 60 / 100);
			break;
		case SKILL_FALL_STONE:
			nTemp = (g_Skills[SKILL_PEBBLE_SHOT].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_PEBBLE_SHOT)) * 60 / 100);
			break;
		case SKILL_STAR_FLARE:
			nTemp = (g_Skills[SKILL_FIRE_PILLAR].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_FIRE_PILLAR)) * 50 / 100)
					+ (g_Skills[SKILL_FIRE_BALL].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_FIRE_BALL)) * 70 / 100);
			break;
		case SKILL_SHOCK_SPEAR:
			nTemp = (g_Skills[SKILL_LIGHTNING].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_LIGHTNING)) * 50 / 100)
					+ (g_Skills[SKILL_LIGHTNING_BALL].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_LIGHTNING_BALL)) * 70 / 100);
			break;
		case SKILL_STONE_LANCE:
			nTemp = (g_Skills[SKILL_FALL_STONE].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_FALL_STONE)) * 50 / 100)
					+ (g_Skills[SKILL_PEBBLE_SHOT].GetApp0(g_ChaSkill.GetLevelByIndex(SKILL_PEBBLE_SHOT)) * 70 / 100);
			break;
		}
		sprintf(strTemp, G_STRING(IDS_DAMAGE_PLUS), g_Skills[nSkill_Index].GetApp0(nSkill_Level), nTemp);
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// %d ~ %d%% 데미지 흡수
	if(nSkill_Index == SKILL_INSIGHT
			|| nSkill_Index == SKILL_ABSORB_MAGIC
			|| nSkill_Index == SKILL_DISTURBANCE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_SUCTION_DAM), g_Skills[nSkill_Index].GetApp0(nSkill_Level), g_Skills[nSkill_Index].GetApp1(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 데미지 배분 %d%%
	if(nSkill_Index == SKILL_JOINT_LIFE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_DAM_DIVISION), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 공격력 %d%% 상승
	if( nSkill_Index == SKILLF_E_BLEED
			|| nSkill_Index == SKILL_FORCE_ATTACK
			|| nSkill_Index == SKILL_BLESSING
			|| nSkill_Index == SKILL_B_BURNINGCRASH
			|| nSkill_Index == SKILL_C_DOWN_CONTROL
			|| nSkill_Index == SKILL_THROW
			|| nSkill_Index == SKILL_HANDLE
			|| nSkill_Index == SKILL_MAGIC_MOVE
			|| nSkill_Index == SKILL_C_HELLDOWN
			|| nSkill_Index == SKILL_H_AUTOMINE
			|| nSkill_Index == SKILL_PIERCING
			|| nSkill_Index == SKILL_SUBMISSION
			|| nSkill_Index == SKILL_AIMING
	  )
	{
		int nStep = 0;

		bool automine = false;
		switch( nSkill_Index )
		{
		case SKILL_B_BURNINGCRASH:
			{
				nStep = 5;
			}			
			break;

		case SKILL_C_HELLDOWN:
			{
				nStep = nSkill_Level;
			}			
			break;

		case SKILL_H_AUTOMINE:
			{
				nStep = 7;

				if( CommonConfig::Instance()->GetCalcFloatSkill() == true )
				{
					automine = true;
				}				
			}
			break;
		}

		sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT), g_Skills[nSkill_Index].GetApp0(nSkill_Level, nStep,automine));		

		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// %d ~ %d%% 공격력 상승
	if( nSkill_Index == SKILL_THOUGHT_STORM
			|| nSkill_Index == SKILL_FURY_EXPLOSION
	  )
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT2), g_Skills[nSkill_Index].GetApp0(nSkill_Level), g_Skills[nSkill_Index].GetApp1(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// (%d단계)
	if( nSkill_Index == SKILL_B_BURNINGCRASH
			|| nSkill_Index == SKILL_C_HELLDOWN
	  )
	{
		int nStep = 0;
		switch( nSkill_Index )
		{
		case SKILL_B_BURNINGCRASH:
			nStep = 5;
			break;
		case SKILL_C_HELLDOWN:
			nStep = 6;
			break;
		}
		nTemp = (nSkill_Level - 1 ) / nStep + 1;
		sprintf( strTemp, G_STRING(IDS_SKILL_INFO_PHASE), nTemp );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 시간감소: %d%
	if( nSkill_Index == SKILL_C_DOWN_CONTROL )
	{
		sprintf(strTemp, G_STRING(IDS_TIME_MINUS), (int)(nSkill_Level * 2.5) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 생명력 회복도: %d
	if( nSkill_Index == SKILL_HEAL_SELF )
	{
		sprintf(strTemp, G_STRING(IDS_INVEN_RECOVERY_VATAL), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 생명력 회복도: +%d%%
	if( nSkill_Index == SKILL_RECOVERY
			|| nSkill_Index == SKILLF_P_REC_UP)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_RECOVERY_VATAL), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 생명력 증가 %d
	if(nSkill_Index == SKILLF_P_VITAL_UP
			|| nSkill_Index == SKILL_A_LIFE_EXPANSION
			|| nSkill_Index == SKILL_SUBMISSION)
	{
		if( nSkill_Index == SKILL_SUBMISSION )
			nTemp = g_Skills[nSkill_Index].GetApp1(nSkill_Level);
		else
			nTemp = g_Skills[nSkill_Index].GetApp0(nSkill_Level);

		sprintf(strTemp, G_STRING(IDS_VITAL_UP), nTemp);
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 명중확률 : %d%%
	if( nSkill_Index == SKILL_FORCE_CONTROL
	  )
	{
		int nStep = 0;
		if( nSkill_Index == SKILL_H_AUTOMINE )
			nStep = 7;

		sprintf(strTemp, G_STRING(IDS_OPTION_11), g_Skills[nSkill_Index].GetApp0(nSkill_Level, nStep) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 전체저항력 %d%% 상승
	if( nSkill_Index == SKILL_BLESSING )
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_INC_RESIS), g_Skills[nSkill_Index].GetApp1(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 재사용 시간 %d
	if( nSkill_Index == SKILLF_E_CLOAKING
			|| nSkill_Index == SKILLF_A_DSTONE )
	{
//		sprintf(strTemp, G_STRING(IDS_TIME_UP),g_Skills[nSkill_Index].GetApp0(nSkill_Level));
//		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 지속 시간: %d
	if( nSkill_Index == SKILLF_E_CLOAKING
			|| nSkill_Index == SKILLF_A_TIMECHARGE
			|| nSkill_Index == SKILL_A_LIFE_EXPANSION
			|| nSkill_Index == SKILL_MANA_SHIELD
			|| nSkill_Index == SKILL_ELECTRIC_SHIELD
			|| nSkill_Index == SKILL_PEACEFUL
			|| nSkill_Index == SKILL_JOINT_LIFE)
	{
		if( nSkill_Index == SKILL_MANA_SHIELD || nSkill_Index == SKILL_ELECTRIC_SHIELD )
			nTemp = g_Skills[nSkill_Index].GetManaShieldTime(nSkill_Level);
		else
			nTemp = g_Skills[nSkill_Index].GetApp1(nSkill_Level);

		sprintf( strTemp, G_STRING( IDS_SKILL_KEEP_TIME ), nTemp );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 소환시간 %d%% 상승
	if(nSkill_Index == SKILL_SUBMISSION)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_INC_SUMMONS_TIME), g_Skills[nSkill_Index].GetEnerge0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 시간증가율 : %d%%
	if(nSkill_Index == SKILL_PASSION_OF_MELODY)
	{
		sprintf(strTemp, G_STRING(IDS_OPTION_12), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 최대누적시간: %d초
	if(nSkill_Index == SKILL_MANA_SHIELD
			|| nSkill_Index == SKILL_ELECTRIC_SHIELD)
	{
		sprintf(strTemp, G_STRING(IDS_ACCTIME),g_Skills[nSkill_Index].GetManaShieldAddTime(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 적용 범위 증가 %d
	if(nSkill_Index == SKILLF_P_STONRINC)
	{
		sprintf(strTemp, G_STRING(IDS_RANGE_UP),g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 적용 타겟 + %d
	if(nSkill_Index == SKILLF_P_STONRINC
			|| nSkill_Index == SKILLF_C_STONEBRUST)
	{
		if( nSkill_Index == SKILLF_P_STONRINC )
			nTemp = int(ceil(float(nSkill_Level) * 5.0f /30.0f));
		else if( nSkill_Index == SKILLF_C_STONEBRUST )
			nTemp = int(ceil(float(nSkill_Level) *10.0f /30.0f));

		sprintf(strTemp, G_STRING(IDS_REDDRAGON_TARGET_PLUS), nTemp);
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 반사데미지 %d%%
	if(nSkill_Index == SKILLF_E_REFLECTION)
	{
		sprintf(strTemp, G_STRING(IDS_EVRB_OPT_REF), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 방어력 %d 상승
	if( nSkill_Index == SKILLF_E_REFLECTION ||
		nSkill_Index == SKILL_MELODY_OF_DEF )
	{
		if( nSkill_Index == SKILLF_E_REFLECTION )
		{
			nTemp = g_Skills[nSkill_Index].GetApp1(nSkill_Level);
			sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF),nTemp);
		}
		else if( nSkill_Index == SKILL_MELODY_OF_DEF )
		{
			nTemp = g_Skills[nSkill_Index].GetApp0(nSkill_Level);
			sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF2),nTemp);
		}
		else if( nSkill_Index == SKILL_B_PROTECTION_SHIELDS )
		{
			nTemp = g_Skills[nSkill_Index].GetApp0(nSkill_Level);
			sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF2),nTemp);
		}
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 데미지 감소 : %d
	if(nSkill_Index == SKILL_B_PROTECTION_SHIELDS)
	{
		sprintf(strTemp, G_STRING(IDS_OPTION_14), g_Skills[nSkill_Index].GetProtectionShieldData(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// %d ~ %d%% 방어력 상승
	if(nSkill_Index == SKILL_IRON_MUSCLE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_INC_DEF), g_Skills[nSkill_Index].GetApp0(nSkill_Level), g_Skills[nSkill_Index].GetApp1(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 몬스터방어력 %d%% 하락
	if(nSkill_Index == SKILL_CRUSH)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_DEF_MON_DEF), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 공격력적용: %d%%
	if(nSkill_Index == SKILL_DEEP_ATTACK)
	{
		sprintf(strTemp, G_STRING(IDS_OPTION_2), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 딥어택 확률 : %d%%
	if(nSkill_Index == SKILL_CHAIN_ATTACK)
	{
		sprintf(strTemp,G_STRING(IDS_OPTION_3),	g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 상태이상 저지확률 : %d%%
	if(nSkill_Index == SKILL_MELODY_OF_GUD)
	{
		sprintf(strTemp, G_STRING(IDS_OPTION_10), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 상태해제 확률 %d%%
	if(nSkill_Index==SKILL_REMOVE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_RATE_REMOVE), g_Skills[nSkill_Index].GetApp0(nSkill_Level),g_Skills[nSkill_Index].GetApp1(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 퍼지계열 해제 %d%%
	if(nSkill_Index==SKILL_REMOVE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_REMOVE_PURGE), g_Skills[nSkill_Index].GetApp1(nSkill_Level),g_Skills[nSkill_Index].GetApp1(nSkill_Level));
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 성공율: %d%%
	if(nSkill_Index == SKILL_H_MINE_CONTROL
			|| nSkill_Index == SKILL_A_SUMMONS_SQUAD
			|| nSkill_Index == SKILL_RECALL
			|| nSkill_Index == SKILL_A_RESURRECTION )
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_SUCCESS_RATE), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 시야 거리: %d 미터
	if(nSkill_Index == SKILL_HIDE)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_VISION_DIST), g_Skills[nSkill_Index].GetApp1(nSkill_Level) / 10);
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 사격거리 증가: %.1f 미터
	if(nSkill_Index == SKILL_SNIFING)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_INC_RANGE), g_Skills[nSkill_Index].GetApp1(nSkill_Level) / 10.0f );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 공격거리: %d
	if(nSkill_Index == SKILL_PEBBLE_SHOT
			|| nSkill_Index == SKILL_LIGHTNING_BALL
			|| nSkill_Index == SKILL_FIRE_BALL
			|| nSkill_Index == SKILL_B_FLYINGCRASH   //mungmae-2005/11/24 플라잉 크래쉬
			|| nSkill_Index == SKILL_F_SHADOWSLASH  //mungmae-2005/11/24 쉐도우 슬래쉬
			|| nSkill_Index == SKILL_B_WILDSMASH
			|| nSkill_Index == SKILL_K_ICEBOLT
			|| nSkill_Index == SKILL_H_FROZENSHOT
			|| nSkill_Index == SKILL_A_RESURRECTION )
	{
		sprintf(strTemp, G_STRING(IDS_INVEN_DIST_ATT), (int)(g_Skills[nSkill_Index].GetApp1(nSkill_Level)/10.0f) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 보석세공 성공률: %d%%
	if(nSkill_Index == SKILL_WORK_GEM)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_SUCCESS_RATE_SEGONG), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 연사 성공률 %d%%
	if(nSkill_Index == SKILL_MULTI_SHOT)
	{
//		sprintf(strTemp, G_STRING(IDS_SKILL_DOUBLE_SHOOT), GetMultiShot(nSkill_Level) ); // 이건 하드코딩
		sprintf(strTemp, G_STRING(IDS_SKILL_DOUBLE_SHOOT), g_Skills[nSkill_Index].GetApp1(nSkill_Level) ); // DTA 에서 읽어오도록 수정 06-10-19 원석
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 멘탈계열 해제 %d초
	if(nSkill_Index == SKILL_LIFE_LIGHT)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_REMOVE_MENTAL), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 상성스킬 해제 %d초
	if(nSkill_Index == SKILL_LIFE_LIGHT)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_REMOVE_SKILL), g_Skills[nSkill_Index].GetApp1(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	// 하드배리어 해제확률 %d%%
	if(nSkill_Index == SKILL_CRUSH)
	{
		sprintf(strTemp, G_STRING(IDS_SKILL_RATE_HARDB), g_Skills[nSkill_Index].GetApp1(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	if(nSkill_Index == SKILL_F_ADRENALINE)
	{
		// 결정타 공격 %d%% 상승
		sprintf(strTemp, G_STRING(IDS_SPECIAL_HIT_ATT), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
		//	치명타 공격 %d%% 상승
		sprintf(strTemp, G_STRING(IDS_SPECIAL_FATAL_ATT), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
		multiText.AddString(strTemp, _FCOLOR_YELLOW);
	}
	if(nSkill_Index == SKILL_B_PASSIVE)
	{
		float val = GetSkillEffectValue_Float(nSkill_Index, nSkill_Level);
		if(val != 0)
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_EFFECT_151), val);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}		
	}
	if(nSkill_Index == SKILL_K_PASSIVE)
	{
		int val = GetSkillEffectValue_Int(nSkill_Index, nSkill_Level);
		if(val != 0)
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_EFFECT_152), val);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}		
	}
	if(nSkill_Index == SKILL_A_PASSIVE)
	{
		float val = GetSkillEffectValue_Float(nSkill_Index, nSkill_Level);
		if(val != 0)
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_EFFECT_153), val);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		
	}
	if(nSkill_Index == SKILL_H_PASSIVE)
	{
		int val = GetSkillEffectValue_Int(nSkill_Index, nSkill_Level);
		if(val != 0)
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_EFFECT_154), val);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
	}
	if(nSkill_Index == SKILL_F_PASSIVE)
	{
		int val = GetSkillEffectValue_Int(nSkill_Index, nSkill_Level);
		if(val != 0)
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_EFFECT_155), val);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
	}
	if(nSkill_Index == SKILL_P_PASSIVE)
	{
		int val = GetSkillEffectValue_Int(nSkill_Index, nSkill_Level);
		if(val != 0)
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_EFFECT_156), val);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
	}
	
	if( nCount )
	{
		// 요구라임 %d
		if( nSkill_Index == SKILLF_E_REFLECTION
				|| nSkill_Index == SKILL_B_BURNINGCRASH
				|| nSkill_Index == SKILL_B_BURNING_CONTROL
				|| nSkill_Index == SKILL_H_MINE_CONTROL
				|| nSkill_Index == SKILL_A_LIFE_EXPANSION
				|| nSkill_Index == SKILL_A_SUMMONS_SQUAD
				|| nSkill_Index == SKILL_C_DOWN_CONTROL
				|| nSkill_Index == SKILL_K_POISON
				|| nSkill_Index == SKILL_C_HELLDOWN
				|| nSkill_Index == SKILL_H_AUTOMINE
				|| nSkill_Index == SKILLF_E_CLOAKING
				|| nSkill_Index == SKILL_A_SUMMON_FLOWER
				|| nSkill_Index == SKILL_F_ADRENALINE
				|| nSkill_Index == SKILL_K_POISON
				|| nSkill_Index == SKILL_H_ELECTRICSHOT
				|| nSkill_Index == SKILL_B_FLYINGCRASH  //mungmae-2005/11/24 플라잉 크래쉬
				|| nSkill_Index == SKILL_F_SHADOWSLASH  //mungmae-2005/11/24 쉐도우 슬래쉬
				|| nSkill_Index == SKILL_B_WILDSMASH
				|| nSkill_Index == SKILL_K_FLAMESPEAR
				|| nSkill_Index == SKILL_H_RANDOMFIRE
				|| nSkill_Index == SKILL_A_SUMMONFIRE
				|| nSkill_Index == SKILL_F_SPACESLASH
				|| nSkill_Index == SKILL_F_ELECTRICHAMMER
				|| nSkill_Index == SKILL_B_PROTECTION_SHIELDS
				|| nSkill_Index == SKILL_K_ICEBOLT
				|| nSkill_Index == SKILL_H_FROZENSHOT
				|| nSkill_Index == SKILL_A_RESURRECTION
				|| nSkill_Index == SKILL_F_MERCERNERY_SLASH
				|| nSkill_Index == SKILL_F_TORNADO_HAMMER
				|| nSkill_Index == SKILL_B_PASSIVE
				|| nSkill_Index == SKILL_K_PASSIVE
				|| nSkill_Index == SKILL_A_PASSIVE
				|| nSkill_Index == SKILL_H_PASSIVE
				|| nSkill_Index == SKILL_F_PASSIVE
				|| nSkill_Index == SKILL_P_PASSIVE
		  )
		{
			sprintf(strTemp, G_STRING(IDS_NEED_LAIM), g_Skills[nSkill_Index].GetMoney(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		else if( nSkill_Index == SKILL_MAGIC_MOVE && nSkill_Level >= 5 )
		{
			sprintf(strTemp, G_STRING(IDS_NEED_LAIM), g_Skills[nSkill_Index].GetMoney(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
	}
}

int CControlSkill::GetSkillEffectValue_Int(int skill_index, int skill_level)
{
	if(skill_level <= 0)
		return 0;

	int val = 0;
	switch(skill_index)
	{
	case SKILL_K_PASSIVE:
		{
			val = 25 * skill_level;
		}
		break;
	case SKILL_H_PASSIVE:
		{
			val = 25 * skill_level;
		}
		break;
	case SKILL_F_PASSIVE:
		{
			val = skill_level;
		}
		break;
	case SKILL_P_PASSIVE:
		{
			val = 25 * skill_level;
		}
		break;
	}

	return val;
}

float CControlSkill::GetSkillEffectValue_Float(int skill_index, int skill_level)
{
	if(skill_level <= 0)
		return 0;

	float val = 0;
	switch(skill_index)
	{
	case SKILL_B_PASSIVE:
		{
			val = 0.5f * skill_level;
		}
		break;
	case SKILL_A_PASSIVE:
		{
			val = 0.5f * skill_level;
		}
		break;
	}

	return val;
}

void CControlSkill::SetSkillEnable()
{

	for (int i = 0; i < g_ChaSkill.m_SkillNum; i++)
	{
		// 신 스킬 체크.(라임 요구 스킬들..)
		if( g_ChaSkill.m_SkillIndex[i] == SKILLF_E_REFLECTION
				|| g_ChaSkill.m_SkillIndex[i] == SKILLF_E_CLOAKING
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_B_BURNINGCRASH
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_B_BURNING_CONTROL
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_C_HELLDOWN
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_C_DOWN_CONTROL
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_A_SUMMONS_SQUAD
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_A_LIFE_EXPANSION
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_H_MINE_CONTROL
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_H_AUTOMINE
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_A_SUMMON_FLOWER
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_F_ADRENALINE
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_K_POISON
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_H_ELECTRICSHOT
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_B_FLYINGCRASH   //mungmae-2005/11/24 플라잉 크래쉬
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_F_SHADOWSLASH  //mungmae-2005/11/24 쉐도우 슬래쉬
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_B_WILDSMASH
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_K_FLAMESPEAR
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_H_RANDOMFIRE
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_A_SUMMONFIRE
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_F_SPACESLASH
				|| g_ChaSkill.m_SkillIndex[i] == SKILL_F_ELECTRICHAMMER
		  )
		{
			// 라임요구 스킬들은 최소 스킬레벨만 체크한다.
			if(g_Skills[g_ChaSkill.m_SkillIndex[i]].m_MinLevel > g_pRoh->m_Level)
				g_ChaSkill.m_SkillIcon[i].SetDisable(TRUE);
			else
				g_ChaSkill.m_SkillIcon[i].SetDisable(FALSE);
		}
		else
		{
			if(g_Skills[g_ChaSkill.m_SkillIndex[i]].m_MinLevel+g_ChaSkill.m_SkillLevel[i] > g_pRoh->m_Level)
				g_ChaSkill.m_SkillIcon[i].SetDisable(TRUE);
			else
				g_ChaSkill.m_SkillIcon[i].SetDisable(FALSE);
		}
	}
}

void CControlSkill::SetLevelupPoint()
{
	if( g_pRoh )
		m_LevelupPoint.SetString(g_pRoh->m_LevelupPoint, true);
}

BOOL CControlSkill::IsInside(int x, int y)
{
	if(m_pSurfaceBack
			&& x >= m_pSurfaceBack->Xpos
			&& y >= m_pSurfaceBack->Ypos
			&& y < m_pSurfaceBack->Ypos + m_pSurfaceBack->GetHeight() )
		return TRUE;

	return FALSE;
}

LRESULT CControlSkill::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int i;

	switch ( msg )
	{
	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(IsInside(x, y))
			return 1;

		break;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))
			return 0;

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if(m_CloseBtn.GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowSkillWindow(FALSE);
			m_CloseBtn.SetState(BTN_NORMAL);
		}

		if(g_pRoh->m_LevelupPoint <= 0)
			return 1;

		for (i = 0; i < g_ChaSkill.m_SkillNum; i++)
		{
			g_ChaSkill.m_SkillIcon[i].MsgProc(hWnd, msg, wParam, lParam);
			if(g_ChaSkill.m_SkillIcon[i].GetState() == BTN_ACTION)
			{
				if(g_ChaSkill.m_SkillLevel[i] < g_Skills[g_ChaSkill.m_SkillIndex[i]].m_MaxLevel)
				{
					if( g_ChaSkill.m_SkillIndex[i] == SKILL_VANPIRE_TOUCH )
					{
						// 카이_뱀파이어터치
						if( g_ChaSkill.m_SkillLevel[i-1] >= 5 )  							// 카이_블러디마나
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					else if( g_ChaSkill.m_SkillIndex[i] == SKILL_HELL_TOUCH )
					{
						// 카이_헬터치
						if( g_ChaSkill.m_SkillLevel[i-1] >= 5 )  							// 카이_뱀파이어터치
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					else if( g_ChaSkill.m_SkillIndex[i] == SKILL_CHAIN_ATTACK )
					{
						// 휴먼_체인어택
						if( g_ChaSkill.m_SkillLevel[i-1] >= 5 )  							// 휴먼_딥어택
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					else if( g_ChaSkill.m_SkillIndex[i] == SKILL_MELODY_OF_GUD )
					{
						// 에이디아_수호의음계
						if( g_ChaSkill.m_SkillLevel[i-1] >= 10 )  							// 에이디아_보호의 음계
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					else if( g_ChaSkill.m_SkillIndex[i] == SKILL_PASSION_OF_MELODY )
					{
						// 에이디아_음악의열정
						if( g_ChaSkill.m_SkillLevel[i-2] >= 10 )  							// 에이디아_보호의 음계
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					else if( g_ChaSkill.m_SkillIndex[i] == SKILL_PASSION_OF_FIGHT )
					{
						// 에이디아_투지의음계
						if( g_ChaSkill.m_SkillLevel[i-1] >= 5 )  							// 에이디아_음악의열정
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					else if( g_ChaSkill.m_SkillIndex[i] == SKILL_FORCE_CONTROL )
					{
						// 불칸_포스컨트롤
						if( g_ChaSkill.m_SkillLevel[i-1] >= 5 )  							// 불칸_포스어텍
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					//--------------------------------------------------------------
					// 신 종족 Skill 요구 조건
					// 2005.1.4 / Lyul
					// define 상  1 이전의 스킬 마스터 필요
					// 회복력 증가 | 헤이스트 | 블레드 | 듀얼 어썰트 | 하이드
					// 생명력 증가 | 헤비마스터리 | 디바인스톤 | 스톤 버스트 | 리플렉션
					else if( g_ChaSkill.m_SkillIndex[i] == SKILLF_P_REC_UP ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_P_HASTE ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_E_BLEED ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_C_DASSAULT ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_E_CLOAKING ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_P_VITAL_UP ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_P_MASERT_HEAVY ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_A_DSTONE ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_C_STONEBRUST ||
							 g_ChaSkill.m_SkillIndex[i] == SKILLF_E_REFLECTION)
					{
						// 이전 Skill 5 필요
						// 이전 MAX : g_Skills[g_ChaSkill.m_SkillIndex[i-1]].m_MaxLevel
						if(g_ChaSkill.m_SkillLevel[i-1] >= 5 )
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)
								g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					// define 상 2 이전의 스킬 마스터 필요
					// 더블슬래쉬 | 이베이션 | 대쉬 | 어썰트 |
					// 배쉬 | 아이언 스킨 | 스톤인크리스 | 타임차지
					else if(g_ChaSkill.m_SkillIndex[i] == SKILLF_A_DSLASH||
							g_ChaSkill.m_SkillIndex[i] == SKILLF_P_EVASION ||
							g_ChaSkill.m_SkillIndex[i] == SKILLF_A_DASH ||
							g_ChaSkill.m_SkillIndex[i] == SKILLF_A_ASSAULT ||

							g_ChaSkill.m_SkillIndex[i] == SKILLF_A_BASH ||
							g_ChaSkill.m_SkillIndex[i] == SKILLF_P_IRONSKIN ||
							g_ChaSkill.m_SkillIndex[i] == SKILLF_P_STONRINC ||
							g_ChaSkill.m_SkillIndex[i] == SKILLF_A_TIMECHARGE)
					{
						// 이전 스킬 5 필요
						if(g_ChaSkill.m_SkillLevel[i-2] >= 5 )
						{
							sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
							if(g_pTcpIp)g_pTcpIp->SendNetMessage(commOutBuf);
						}
					}
					else
					{
						// 스킬업관련 퍼롬 선행스킬 체크!!!
						sprintf(commOutBuf, "skillup %d\n", g_ChaSkill.m_SkillIndex[i]);
						if(g_pTcpIp)
							g_pTcpIp->SendNetMessage(commOutBuf);
					}
				}
				g_ChaSkill.m_SkillIcon[i].SetState(BTN_NORMAL);
			}
		}
		return 1;

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(IsInside(x, y))
		{
			if(m_CloseBtn.IsInside(x, y))
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

			if(m_BtnWindow_1.IsInside( x, y ) )
			{
				// 1번창 버튼 클릭
				m_BtnWindow_1.SetState(BTN_DOWN);
				if(m_BtnWindow_2.GetState() != BTN_NORMAL)
					m_BtnWindow_2.SetState(BTN_NORMAL);
			}
			else if(m_BtnWindow_2.IsInside(x, y))
			{
				// 2번창 버튼 클릭
				m_BtnWindow_2.SetState(BTN_DOWN);
				if(m_BtnWindow_1.GetState() != BTN_NORMAL)
					m_BtnWindow_1.SetState(BTN_NORMAL);
			}

			if(g_pRoh->m_LevelupPoint <= 0)
				return 1;

			//------------------------------------------------------------------
			// 1번창일때.
			if(m_BtnWindow_1.GetState() == BTN_DOWN)
			{
				for (i = 0; i < g_ChaSkill.m_SkillNum; i++)
				{
					// 왼쪽 창의 스킬들
					if(g_ChaSkill.m_SkillIndex[i] < 47
							|| ( g_ChaSkill.m_SkillIndex[i] > 52 && g_ChaSkill.m_SkillIndex[i] < 60)
							|| ( g_ChaSkill.m_SkillIndex[i] > 78 && g_ChaSkill.m_SkillIndex[i] < 89)
							|| ( g_ChaSkill.m_SkillIndex[i] == 111 || g_ChaSkill.m_SkillIndex[i] == 117 )
							|| ( g_ChaSkill.m_SkillIndex[i] >= 119 && g_ChaSkill.m_SkillIndex[i] <= 123 )
							|| IsTabSkill(0, g_ChaSkill.m_SkillIndex[i] )
					  )
					{
						if(g_ChaSkill.m_SkillLevel[i] < g_Skills[g_ChaSkill.m_SkillIndex[i]].m_MaxLevel)
							g_ChaSkill.m_SkillIcon[i].MsgProc(hWnd, msg, wParam, lParam);
					}
				}
			}
			//------------------------------------------------------------------
			// 2번창일때.
			else if(m_BtnWindow_2.GetState() == BTN_DOWN)
			{
				for (i = 0; i < g_ChaSkill.m_SkillNum; i++)
				{
					if(( g_ChaSkill.m_SkillIndex[i] >= 47 && g_ChaSkill.m_SkillIndex[i] <= 52 )
							|| ( g_ChaSkill.m_SkillIndex[i] >= 60 && g_ChaSkill.m_SkillIndex[i] <= 78 )
							|| ( g_ChaSkill.m_SkillIndex[i] >= 89 && g_ChaSkill.m_SkillIndex[i] <= 110 )
							|| ( g_ChaSkill.m_SkillIndex[i] >= 112 && g_ChaSkill.m_SkillIndex[i] <= 116 )
							|| ( g_ChaSkill.m_SkillIndex[i] == 118 || g_ChaSkill.m_SkillIndex[i] == 124 )
							|| IsTabSkill(1, g_ChaSkill.m_SkillIndex[i] )
					  )
					{
						if(g_ChaSkill.m_SkillLevel[i] < g_Skills[g_ChaSkill.m_SkillIndex[i]].m_MaxLevel)
							g_ChaSkill.m_SkillIcon[i].MsgProc(hWnd, msg, wParam, lParam);
					}
				}
			}
		}
		else
			return 0;

		return 1;
	}

	return 0;
}

void CControlSkill::SetNkCha(CNkCharacter *pNkCha)
{
	CNkCharacter* pNkChaacter = pNkCha;
	if( !pNkChaacter )
		pNkChaacter = g_pRoh;

	g_ChaSkill.Init(pNkChaacter->m_Race);
	g_ChaSkill.LoadRes(SKILL_ICON_WIDTH, SKILL_ICON_HEIGHT);
	SetSkillEnable();
}



bool CControlSkill::IsTabSkill(int tab, int skill_index)
{
	if( tab == 0 )
	{
		if( skill_index == 125
				|| skill_index == 126
				|| skill_index == 127
				|| skill_index == 128
				|| skill_index == 134
				|| skill_index == 148
				|| skill_index == 149
				|| skill_index == 129
				|| skill_index == 130
				|| skill_index == 131
				|| skill_index == 135
				|| skill_index == 132
				|| skill_index == 133
				|| skill_index == 136
				|| skill_index == 137
				|| skill_index == 138
		  )
			return true;
	}
	else if( tab == 1 )
	{
		if( skill_index == 139
				|| skill_index == 140
				|| skill_index == 141
				|| skill_index == 150
				|| skill_index == 142
				|| skill_index == 143
				|| skill_index == 144
				|| skill_index == 145
				|| skill_index == 146
				|| skill_index == 147
				|| skill_index == 151
				|| skill_index == 152
				|| skill_index == 153
				|| skill_index == 154
				|| skill_index == 155
				|| skill_index == 156
		  )
			return true;
	}

	return false;
}

void CControlSkill::SetSkillInfo_Perom(CMultiText &multiText, int nSkill_Index, int nSkill_Level, int nCount)
{
	int nTemp = 0;
	char strTemp[ 128 ] = "";

	CSkill * current_skill = &g_Skills[nSkill_Index];

	if( !current_skill )
		return;

	switch(nSkill_Index)
	{
	case PEROM_SKILL_S125:
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S126:
		{
			sprintf(strTemp, G_STRING(IDS_OPTION_10), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;

	case PEROM_SKILL_S139:
		{
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1908) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1909) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1906) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;

	case PEROM_SKILL_S134:
		{
			sprintf(strTemp, G_STRING(IDS_LHSTRING1895), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, G_STRING( IDS_SKILL_KEEP_TIME ), 150);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;

	case PEROM_SKILL_S127:
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT3), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_LHSTRING1894) , (int)(current_skill->m_cool_time * 0.001));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S128:
	case PEROM_SKILL_S129:
	case PEROM_SKILL_S130:
	case PEROM_SKILL_S131:
	case PEROM_SKILL_S132:
	case PEROM_SKILL_S133:
	case PEROM_SKILL_S135:
	case PEROM_SKILL_S136:
	case PEROM_SKILL_S137:
	case PEROM_SKILL_S138:
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT3), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_LHSTRING1894) , (int)(current_skill->m_cool_time * 0.001));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1909) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S140:
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT3), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_LHSTRING1894) , (int)(current_skill->m_cool_time * 0.001));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1911) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;

	case PEROM_SKILL_S141:
		{
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_SKILL_RATE_REMOVE), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1911) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;

	case PEROM_SKILL_S142:
		{
			sprintf(strTemp, (char*)G_STRING(IDS_LHSTRING1912) , g_Skills[nSkill_Index].GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S143:
		{
			sprintf(strTemp, (char*)G_STRING(IDS_LHSTRING1913), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S144:
		{
			sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_AVOIDANCE), g_Skills[nSkill_Index].GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S145:
		{
			sprintf(strTemp, (char*)G_STRING(IDS_LHSTRING1914) , g_Skills[nSkill_Index].GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S146:
		{
			sprintf(strTemp, G_STRING(IDS_SPECIAL_INC_DEF2), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S147:
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT3), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S148:
		{
			sprintf(strTemp, G_STRING(IDS_LHSTRING1944), g_Skills[nSkill_Index].GetApp0(nSkill_Level) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);

			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);

			sprintf(strTemp, G_STRING(IDS_LHSTRING1894) , (int)(current_skill->m_cool_time * 0.001));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);

			sprintf( strTemp, G_STRING( IDS_SKILL_KEEP_TIME ), 300);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case PEROM_SKILL_S149:
		{
			sprintf(strTemp, G_STRING(IDS_SKILL_INC_ATT3), current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_LHSTRING1894) , (int)(current_skill->m_cool_time * 0.001));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, G_STRING( IDS_SKILL_KEEP_TIME ), 300);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;

	case PEROM_SKILL_S150:
		{
			sprintf(strTemp, (char*)G_STRING(IDS_LHSTRING1915) , current_skill->GetApp0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), current_skill->GetEnerge0(nSkill_Level));
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1910) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, (char*)G_STRING(IDS_LHSTRING1916) );
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
			sprintf( strTemp, G_STRING( IDS_SKILL_KEEP_TIME ), 150);
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		break;
	case SKILL_P_PASSIVE:
		{
			int val = GetSkillEffectValue_Int(nSkill_Index, nSkill_Level);
			if(val != 0)
			{
				sprintf(strTemp, G_STRING(IDS_SKILL_EFFECT_156), val);
				multiText.AddString(strTemp, _FCOLOR_YELLOW);
			}
		}
		break;
	}
	
	sprintf(strTemp, G_STRING(IDS_NEED_LAIM), g_Skills[nSkill_Index].GetMoney(nSkill_Level));
	multiText.AddString(strTemp, _FCOLOR_YELLOW);

}

