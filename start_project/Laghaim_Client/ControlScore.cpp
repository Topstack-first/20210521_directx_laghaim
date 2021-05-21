#include "stdafx.h"
#include "main.h"
#include "headers.h"
#include "sheet.h"
#include "ControlInven.h"
#include "ControlScore.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"
#include "tcpipcon.h"
#include "textoutbox.h"
#include "ItemProto.h"
#include "textbox.h"
#include "main.h"
#include "questdata.h"
#include "LoadEffectSound.h"
#include "g_stringmanager.h"
#include "LHExcelReader.h"
#include "Skill.h"
#include "Land.h"
#include "DmitronBattle.h"
#include "NkMyPet.h"
#include "DefenseValMgr.h"
#include "CommonConfig.h"

extern CPetStatus g_MyPetStatus;
extern BOOL	g_bAdmin2;
extern char g_strFontName[];

extern HINSTANCE g_hDllInst;
extern BOOL g_bUseCashShop; ///유료상점
extern int	g_MatrixSvr;
extern DWORD		g_dwClientCountry;

//----------------------------------------------------------------------
//		DEFINE
//----------------------------------------------------------------------
#define CLOSE_WINDOW	91
#define SHOW_WINDOW		92

#define LEVELUP_VITAL		0
#define LEVELUP_MANA		1
#define LEVELUP_STAMINA	2
#define LEVELUP_EPOWER	3
#define LEVELUP_STR		4
#define LEVELUP_INT		5
#define LEVELUP_DEX		6
#define LEVELUP_CON		7
#define LEVELUP_CHA		8

#define _800			44
#define _1024			55

#define _MAIN_XPOS_800   596
#define _MAIN_XPOS_1024  822

const int TextOut_Font__Size = 12;

extern BOOL CheckCanNpcUpgradeItem( CItem *pItem ); // 강화가 가능한 아이템인지의 체크용. ///신방어구강화시스템
extern BOOL g_bUseMatrixCountry; // 매트릭스(뫼비우스 아레나)가 도입된 나라인가의 플래그.(도입 안된나라는 뫼비우스 데이터를 패치 안해주기 때문에 이게 FALSE여야한다.) ///해외뫼비우스

#define IS_2011_ITEM_ARMOR(vnum) ((2295<=vnum && vnum<=2316) || (2942<=vnum && vnum<=2953) || (3152<=vnum && vnum<=3155) || (3227<=vnum && vnum<=3230) ? TRUE : FALSE )
#define IS_2011_ITEM_WEAPON(vnum) (/*(2743<=vnum && vnum<=2749) ||*/ (2939<=vnum && vnum<=2941) || (3151==vnum)/* || (3231==vnum) */? TRUE : FALSE )

#define IS_G_LEVEL_ITEM(vnum) ( (2173<=vnum && vnum<=2179) || (2743<=vnum && vnum<=2749)|| (3231==vnum) )

CControlScore::CControlScore()
	: m_FontWidth(0)
	, m_FontHeight(0)
	, m_pNkCha(NULL)
	, m_LevelUpPoint(0)
{
	memset(m_Tmp, 0, sizeof(m_Tmp));

	m_Font	   = NULL;

	m_pCashMarkSur = NULL;
	m_pCashTextSur = NULL;

	m_pBuffTexture[0][0] = 0;
	m_pBuffTexture[0][1] = 0;
	m_pBuffTexture[1][0] = 0;
	m_pBuffTexture[1][1] = 0;
	m_pBuffTexture[2][0] = 0;
	m_pBuffTexture[2][1] = 0;
	m_pBuffTexture[3][0] = 0;
	m_pBuffTexture[3][1] = 0;

	m_pIsBuffPopup = FALSE;
	SetRect(&m_rcDmitronPopup, 0, 0, 0, 0);

	for(int i=0; i<4; i++)
		m_SetItemFlag2[i] = 0;

	m_bFaceLoaded = false;
	m_bHPTableLoaded = false;
}


CControlScore::~CControlScore()
{
	DeleteRes();
}

HRESULT CControlScore::RestoreSurfaces()
{
	if (g_pDisplay)
	{
		if (g_pNk2DFrame && g_pNk2DFrame->GetSheet() == NULL)
		{
			SetAllScore();
		}

		if( CommonConfig::Instance()->GetRenewerCharInfo() == true )
		{
			m_bFaceLoaded = false;
			LoadRes_Char();
		}
	}

	return S_OK;
}

void CControlScore::Init(int FontSize)
{
	HDC hDC = GetDC(NULL);
	int ptHeight = -MulDiv(FontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	SetFont(FontSize/2, FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	m_FontWidth  = sizeFont.cx;
	m_FontHeight = sizeFont.cy;
}

void CControlScore::LoadRes()
{
	if( g_pDisplay == NULL )
		return;

	if( CommonConfig::Instance()->GetRenewerCharInfo() == true )
	{
		LoadRes_NEW();
	}
	else
	{
		LoadRes_OLD();
	}	

	Init(9);

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[0][0], "interface/GuildSiege/skill_attack_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[0][1], "interface/GuildSiege/skill_attack_unable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[1][0], "interface/GuildSiege/skill_defense_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[1][1], "interface/GuildSiege/skill_defense_unable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[2][0], "interface/GuildSiege/skill_life_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[2][1], "interface/GuildSiege/skill_life_unable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[3][0], "interface/GuildSiege/skill_item_normal.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pBuffTexture[3][1], "interface/GuildSiege/skill_item_unable.bmp");

	m_BuffTextPopup.Init( 12, RGB(255,255,255), 0, 0 );

	for ( int i = 0; i < 4; ++i )
	{
		m_pBuffTexture[i][0]->Xpos = m_background.m_x + 28 + 26 * i;
		m_pBuffTexture[i][1]->Xpos = m_background.m_x + 28 + 26 * i;
		m_pBuffTexture[i][0]->Ypos = m_background.m_y + 395;
		m_pBuffTexture[i][1]->Ypos = m_background.m_y + 395;
	}

	if( g_bUseCashShop ) ///유료상점 이 도입된 나라면..
	{
		if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pCashMarkSur, "interface/CashMark_Status.bmp") ) )
			return;

		m_pCashMarkSur->Xpos = m_background.m_x+21;
		m_pCashMarkSur->Ypos = m_background.m_y+365;
	}

	if( g_pRoh )
	{
		SetAllScore();
	}
}

void CControlScore::DeleteRes()
{
	m_background.DeleteRes();
	m_btn_close.DeleteRes();
	m_btn_quest.DeleteRes();

	m_btn_pet.DeleteRes();
	m_bFaceLoaded = false;
	m_face.DeleteRes();
	//m_defense.DeleteRes();
	//m_status_box.DeleteRes();
	m_tableHP.clear();

	int i;
	
	for(i=0; i<LEVELBTN_CNT; i++)
	{
		m_btn_pointup[i].DeleteRes();
		m_btn_pointdown[i].DeleteRes();
	}

	for(i=0; i< TEXTSURFACE_CNT; i++)
		m_textout[i].DeleteRes();

	SAFE_DELETE( m_pCashMarkSur ); // 캐쉬 표시용 마크 ///유료상점
	SAFE_DELETE( m_pCashTextSur ); // 캐쉬 액수 표시용 서피스(나라별로 출력 되고 말고 함으로 그냥 따로 가지고있는다.) ///유료상점

	for ( i = 0; i < 4; i++)
	{
		SAFE_DELETE( m_pBuffTexture[i][0] );
		SAFE_DELETE( m_pBuffTexture[i][1] );
	}
	m_BuffTextPopup.DeleteRes();
}

//----------------------------------------------------------------------
//		DRAW
//----------------------------------------------------------------------
void CControlScore::DrawDmitronEffect()
{
	if( !g_pNk2DFrame )
		return;

	if( !GET_D3DDEVICE() )
		return;

	D3DVIEWPORT7 vpOld, vpNew;
	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = g_pNk2DFrame->GetClientWidth();
	vpNew.dwHeight = g_pNk2DFrame->GetClientHeight();
	GET_D3DDEVICE()->SetViewport(&vpNew);


	DWORD dwState[8];

	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwState[0] );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwState[1]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[2]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[3]);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[4]);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwState[5] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwState[6] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwState[7] );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	float m_fAlpha = 0.5f;
	D3DTLVERTEX vertex[4];

	vertex[0] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.left, m_rcDmitronPopup.top, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	vertex[1] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.right, m_rcDmitronPopup.top, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	vertex[2] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.left, m_rcDmitronPopup.bottom, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	vertex[3] = D3DTLVERTEX( D3DVECTOR( m_rcDmitronPopup.right, m_rcDmitronPopup.bottom, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	vertex[0].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);
	vertex[1].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);
	vertex[2].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);
	vertex[3].color = D3DRGBA(0.0f, 0.0f, 0.0f, m_fAlpha);

	GET_D3DDEVICE()->SetTexture(0,NULL);
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vertex, 4, 0 );

	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwState[0] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwState[1]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[2]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[3]);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[4]);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwState[5] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwState[6] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwState[7] );

	m_BuffTextPopup.Draw();
}

void CControlScore::Draw()
{
	if( g_pDisplay == NULL )
		return;

	if( CommonConfig::Instance()->GetRenewerCharInfo() == true )
	{
		Draw_NEW();
	}
	else
	{
		Draw_OLD();
	}	

	if( g_bUseCashShop ) ///유료상점 이 도입된 나라면..
	{
		if( m_pCashMarkSur )
			g_pDisplay->Blt( m_pCashMarkSur->Xpos, m_pCashMarkSur->Ypos, m_pCashMarkSur );

		if( m_pCashTextSur )
			g_pDisplay->Blt( m_pCashTextSur->Xpos, m_pCashTextSur->Ypos, m_pCashTextSur );
	}

	if( pCMyApp->m_nCurWorldIdx == WORLD_DMITRON )
	{
		for( int i = 0; i < 4; ++i)
		{
			if (CDmitronInfo::GetBuff(i) == TRUE)
			{
				if (m_pBuffTexture[i][0])
					g_pDisplay->Blt(m_pBuffTexture[i][0]->Xpos, m_pBuffTexture[i][0]->Ypos, m_pBuffTexture[i][0]);
			}
			else
			{
				if (m_pBuffTexture[i][1])
					g_pDisplay->Blt(m_pBuffTexture[i][1]->Xpos, m_pBuffTexture[i][1]->Ypos, m_pBuffTexture[i][1]);
			}
		}
		if (m_pIsBuffPopup)
		{
			DrawDmitronEffect();
		}
	}
}


void CControlScore::DrawPointUpBtn()
{
	if( g_pRoh && g_pRoh->m_LevelupPoint > 0 )
	{
		for(int i=0; i<LEVELBTN_CNT; i++)
		{
			m_btn_pointup[i].Draw();
		}
	}
	for(int i=0; i<LEVELBTN_CNT; i++)
	m_btn_pointdown[i].Draw();
}

void CControlScore::PutString(int idx, char* String, int x, int y, DWORD Color )
{
}

void CControlScore::SetFont (int f_w, int f_h, char *fontname)
{
	HDC hDC = GetDC(NULL);

	int ptHeight = -MulDiv(f_h, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);
	
	m_Font = G_GetFont(ptHeight,0,fontname);
}

BOOL CControlScore::IsInside(int x, int y)
{
	return m_background.IsIn(x, y);
}

LRESULT CControlScore::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;

	switch ( msg )
	{
	case WM_CHAR:
		if(g_bAdmin2)
		{
			switch((int)wParam)
			{
			case '<':
				PointUpBtnAct(4);
				break;
			case '>':
				PointUpBtnAct(5);
				break;
			case '?':
				PointUpBtnAct(6);
				break;
			case ':':
				PointUpBtnAct(7);
				break;
			case '"':
				PointUpBtnAct(8);
				break;
			}
		}


	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
			return 1;

		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 마우스가 CControlScore 패널 위에 놓여 있지 않으면
		// 메시지를 처리하지 않는다.
		if (!IsInside(x, y))
			return 0;

		m_btn_close.MsgProc(hWnd, msg, wParam, lParam);
		m_btn_quest.MsgProc(hWnd, msg, wParam, lParam);

		if(g_pRoh->m_LevelupPoint >0 )
		{
			for(int i=0; i<LEVELBTN_CNT; i++)
			{
				m_btn_pointup[i].MsgProc(hWnd, msg, wParam, lParam);

				if(m_btn_pointup[i].GetState() == BTN_ACTION)
				{
					m_btn_pointup[i].m_iBtnFlg = BTN_NORMAL;
					PointUpBtnAct(i+4);
				}
			}
		}

		for(int i=0; i<LEVELBTN_CNT; i++)
			{
				m_btn_pointdown[i].MsgProc(hWnd, msg, wParam, lParam);

				if(m_btn_pointdown[i].GetState() == BTN_ACTION)
				{
					m_btn_pointdown[i].m_iBtnFlg = BTN_NORMAL;
					PointDownBtnAct(i+4);
				}
			}



		if(m_btn_close.GetState() == BTN_ACTION)
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterfaceWindow(FALSE, SCORE);
			m_btn_close.m_iBtnFlg = BTN_NORMAL;
		}

		if(m_btn_quest.GetState() == BTN_ACTION)
		{
			m_btn_quest.m_iBtnFlg = BTN_NORMAL;
			if( g_pNk2DFrame )
			{	
				g_pNk2DFrame->ToggleInterfaceWindow(SCORE);
				((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->QuestMessage();
				g_pNk2DFrame->ToggleInterfaceWindow(QUEST);
			}
		}

		if( g_MatrixSvr == 0 )
		{
			m_btn_pet.MsgProc(hWnd, msg, wParam, lParam);

			if (m_btn_pet.GetState() == BTN_ACTION)
			{
				switch (g_MyPetStatus.mode)
				{
				case MYPET_MODE_NONE:
					//-- IDS_TOP_PET_NOTEXIST : 당신은 애완동물을 기르고 있지 않습니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_TOP_PET_NOTEXIST),TYPE_NOR_OK, 1);
					break;
				case MYPET_MODE_EGG:
					//-- IDS_TOP_PET_NOTYET : 당신의 애완동물은 아직 부화되지 않습니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_TOP_PET_NOTYET),TYPE_NOR_OK, 1);
					break;
				case MYPET_MODE_KEEP:
					//-- IDS_TOP_PET_KEEPING : 당신의 애완동물은 현재 보관상태입니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_TOP_PET_KEEPING),TYPE_NOR_OK, 1);
					break;
				default:
					g_pNk2DFrame->ToggleInterfaceWindow(SCORE);
					g_pNk2DFrame->TogglePetStatusWindow();
					break;
				}
			}
		}

		// 드미트론 버프 효과  표시
		if (pCMyApp->m_nCurWorldIdx == WORLD_DMITRON ||
				pCMyApp->m_nCurWorldIdx == WORLD_DMITRON_BATTLE )
			CalcPopupIndex(x, y);

		return 1;
	}
	return 0;
}


void CControlScore::PointUpBtnAct(int Idx)
{
	char buf[128];
	if (Idx >= 0 && Idx < 4+LEVELBTN_CNT)
	{
		if (g_pTcpIp)
		{
			sprintf(buf, "levelup %d\n", Idx);
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(buf);
		}
	}
}
void CControlScore::PointDownBtnAct(int Idx)
{
	char buf[128];
	if (Idx >= 0 && Idx < 4+LEVELBTN_CNT)
	{
		if (g_pTcpIp)
		{
			sprintf(buf, "leveldown %d\n", Idx);
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(buf);
		}
	}
}

void CControlScore::SetName()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Name].SetString(g_pRoh->m_pstrName);
}

void CControlScore::SetRace()
{
	if( g_pRoh == NULL )
		return;

	char race_name[50] = {0,};
	int x = 0, y = 0;

	if( g_pRoh )
	{
		//-- 종족 설정.
		switch (g_pRoh->m_Race)
		{
		case RACE_BULKAN://-- IDS_SCORE_BULKAN : 불        칸
			strcpy(race_name, G_STRING(IDS_SCORE_BULKAN));
			break;
		case RACE_KAILIPTON://-- IDS_SCORE_KAI : 카이립톤
			strcpy(race_name, G_STRING(IDS_SCORE_KAI));
			break;
		case RACE_AIDIA://-- IDS_SCORE_ADI : 에이디아
			strcpy(race_name, G_STRING(IDS_SCORE_ADI));
			break;
		case RACE_HUMAN://-- IDS_SCORE_HUMAN : 휴       먼
			strcpy(race_name, G_STRING(IDS_SCORE_HUMAN));
			break;
		case RACE_FREAK://-- IDS_SCORE_FREEK : 하이브리더 ( 프리크 )
			strcpy(race_name, G_STRING(IDS_SCORE_FREEK));
			break;
		case RACE_PEROM:
			strcpy(race_name, G_STRING(IDS_LHSTRING1893));
			break;
		}
	}

	m_textout[ScoreTextOut_Type__Race].SetString(race_name);
}

void CControlScore::SetLevel()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Level].SetString(g_pRoh->m_Level);
}

void CControlScore::SetSex()
{
	if( g_pRoh == NULL )
		return;

	char sex_char[64]= {0,};
	int x = 0, y = 0;

	switch (g_pRoh->m_Sex)
	{
	case 0://-- IDS_SCORE_MALE : 남
		strcpy(sex_char, G_STRING(IDS_SCORE_MALE));
		break;
	case 1://-- IDS_SCORE_FEMALE : 여
		strcpy(sex_char, G_STRING(IDS_SCORE_FEMALE));
		break;
	default:
		strcpy(sex_char, "--");
		break;
	}

	m_textout[ScoreTextOut_Type__Sex].SetString(sex_char);
}

void CControlScore::Set_GameTime(int time)
{
	m_textout[4].SetString(time);
}

void CControlScore::SetVital()
{
	if( g_pRoh == NULL )
		return;

	if( CommonConfig::Instance()->GetRenewerCharInfo() == true )
	{
		if( m_bHPTableLoaded == false )
		{
			LoadRes_HPTable();
		}

		int addHP = 0;
		addHP = m_tableHP[g_pRoh->m_Level];

		sprintf(m_Tmp, "%d/%d (%d)", (int)g_pRoh->m_Vital, (int)g_pRoh->m_MaxVital, addHP);
	}
	else
	{
		sprintf(m_Tmp, "%d/%d", (int)g_pRoh->m_Vital, (int)g_pRoh->m_MaxVital);
	}
	
	m_textout[ScoreTextOut_Type__Vital].SetString(m_Tmp);
}

void CControlScore::SetMana()
{
	if( g_pRoh == NULL )
		return;

	sprintf(m_Tmp, "%d/%d", (int)g_pRoh->m_Mana, (int)g_pRoh->m_MaxMana);
	m_textout[ScoreTextOut_Type__Mana].SetString(m_Tmp);
}

void CControlScore::SetStamina()
{
	if( g_pRoh == NULL )
		return;

	sprintf(m_Tmp, "%d/%d", (int)g_pRoh->m_Stamina, (int)g_pRoh->m_MaxStamina);
	m_textout[ScoreTextOut_Type__Stamina].SetString(m_Tmp);
}

void CControlScore::SetEpower()
{
	if( g_pRoh == NULL )
		return;

	sprintf(m_Tmp, "%d/%d", (int)g_pRoh->m_Epower, (int)g_pRoh->m_MaxEpower);
	m_textout[ScoreTextOut_Type__EErg].SetString(m_Tmp);
}

void CControlScore::SetPenalty()
{
	if( g_pRoh == NULL )
		return;

	sprintf(m_Tmp, "%d", g_pRoh->m_Chaotic_Num/10);
	m_textout[ScoreTextOut_Type__Penalty].SetString(m_Tmp);
}

void CControlScore::SetDefense()
{
	if( g_pRoh == NULL )
		return;

	int tot_defense = 0;
	int tot_eventdp = 0;

	for(int i=0; i<=8; i++)
	{
		tot_defense += GetItemDefense( g_pRoh->m_Wearing[i] );
		tot_eventdp += GetItemDefense( g_pRoh->m_EventWearing[i] );
	}
	
	float up_ratio = 1.0f;

	if( g_pRoh->m_Race == RACE_PEROM )
	{
		if( g_pRoh->IsActiveDSystem() )
		{
			int over_skill = -1;
			for( int i=0 ; i<26 ; i++ )
			{
				if( g_ChaSkill.m_SkillIndex[i] == PEROM_SKILL_S146 )
				{
					float ratio = g_Skills[PEROM_SKILL_S146].GetApp0(g_ChaSkill.m_SkillLevel[i]) * 0.01;
					up_ratio += ratio;
					break;
				}
			}
		}
	}

	sprintf(m_Tmp, "%d(%d)", (int)(tot_defense*up_ratio), (int)(tot_eventdp*up_ratio) );

	m_textout[ScoreTextOut_Type__Def].SetString(m_Tmp);
}

// BP
void CControlScore::SetBP()
{
	if( CommonConfig::Instance()->GetThaFlag() == true )
		return;

	if( g_pRoh == NULL )
		return;	

	// BP와 GHP는 몫과 나머지로 나눠서 출력해줘야한다.
	int arg1 = g_pRoh->m_nBattlePoint / 100; // 몫
	int arg2 = g_pRoh->m_nBattlePoint % 100; // 나머지

	_itoa( arg1, m_Tmp, 10 );

	m_textout[ScoreTextOut_Type__BP].SetString(m_Tmp);
}

// GHP
void CControlScore::SetGHP()
{
	if( CommonConfig::Instance()->GetThaFlag() == true )
		return;

	if( g_pRoh == NULL )
		return;

	// BP와 GHP는 몫과 나머지로 나눠서 출력해줘야한다.
	int arg1 = g_pRoh->m_nGHPoint / 100; // 몫
	int arg2 = g_pRoh->m_nGHPoint % 100; // 나머지

	_itoa( arg1, m_Tmp, 10 );

	m_textout[ScoreTextOut_Type__GHP].SetString(m_Tmp);
}


const int Ability_Pos_X = 734;
const int Ability_Pos_Y = 197;
const int Ability_Offset = 16;

void CControlScore::SetStr()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Strength].SetString(g_pRoh->m_Str);
}

void CControlScore::SetInt()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Wisdom].SetString(g_pRoh->m_Int);
}

void CControlScore::SetDex()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Dex].SetString(g_pRoh->m_Dex, TRUE);
}

void CControlScore::SetCon()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Strain].SetString(g_pRoh->m_Con , TRUE);
}

void CControlScore::SetCha()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Charisma].SetString(g_pRoh->m_Cha , TRUE);
}

void CControlScore::Set_AAtt_Val(int aatt_s)
{
}

void CControlScore::Set_Att_S(int att_s)
{
}

void CControlScore::Set_Def(int def)
{
	if( g_pRoh == NULL )
		return;

	int tot_defense = 0;
	int tot_eventdp = 0;

	for(int i=0; i<=8; i++)
	{
		tot_defense += GetItemDefense( g_pRoh->m_Wearing[i] );
		tot_eventdp += GetItemDefense( g_pRoh->m_EventWearing[i] );
	}

	if( g_dwLangType == LANG_KOR )
		sprintf(m_Tmp, "%d/(%d)", tot_defense, tot_eventdp );
	else
		sprintf(m_Tmp, "%d/", tot_defense);

	m_textout[ScoreTextOut_Type__Def].SetString(m_Tmp);
}

void CControlScore::Set_Move_S(int move_s)
{
}

void CControlScore::SetMoney()
{
	if( g_pRoh == NULL )
		return;

	_ltoa_s( g_pRoh->m_Money, m_Tmp, 10 );
	m_textout[ScoreTextOut_Type__Stash].SetString(g_pRoh->m_Money,true);
}


void CControlScore::SetExp()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__TotalExp].SetString(g_pNk2DFrame->GetExpAccumulate() , true);
}

void CControlScore::Set_NeedExp()
{
	if( g_pRoh == NULL )
		return;
	if(g_pRoh->m_NeedExp < 0)return;
	sprintf( m_Tmp, "%I64d", g_pRoh->m_NeedExp );
	m_textout[ScoreTextOut_Type__ExpLevel].SetString(g_pNk2DFrame->GetExpRemain(),true);
}

void CControlScore::SetLevelupPoint()
{
	m_textout[ScoreTextOut_Type__LevelUpPoint].SetString(g_pRoh->m_LevelupPoint);
}

void CControlScore::SetFame()
{
	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__Fame].SetString(g_pRoh->m_Fame , TRUE);
}

void CControlScore::SetCash()
{
	if ( !g_pDisplay || !m_Font )
		return;

	char strTemp[256];

	_itoa( g_pRoh->m_HaveCash, m_Tmp, 10 );

	int let = strlen(m_Tmp);

	if( m_pCashTextSur )
		SAFE_DELETE( m_pCashTextSur );

	strcpy( strTemp, m_Tmp);
	g_pDisplay->CreateSurface( &m_pCashTextSur, let*m_FontWidth,  m_FontHeight);

	if( m_pCashTextSur )
	{
		m_pCashTextSur->Clear();
		m_pCashTextSur->DrawText(m_Font, strTemp, 0, 0, 0, RGB(231, 203, 102), 0);
		m_pCashTextSur->SetColorKey(0);
		
		m_pCashTextSur->Xpos = m_background.m_x+80;
		m_pCashTextSur->Ypos = m_background.m_y+295-5;
	}
}

void CControlScore::SetLP()
{
	if( CommonConfig::Instance()->GetThaFlag() == true )
		return; 

	if( g_pRoh == NULL )
		return;

	m_textout[ScoreTextOut_Type__LP].SetString(g_pRoh->m_nLaghaimPoint, TRUE);
}

int CControlScore::GetItemDefense(CItem * pItem)
{
	int defense = 0;
	float f_defense = 0;

	if( pItem && GET_TYPE(pItem) != ITYPE_SUB )
	{
		if( pItem->m_MaxUpgradeEndurance )
		{
			if( pItem->m_UpgradeEndurance <= 0 )
				return 0;
		}
				
		f_defense = pItem->m_Defense;
		defense = f_defense;

		if( IS_2011_ITEM_ARMOR(pItem->m_Vnum) )
		{
			switch(pItem->m_PlusNum)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				defense += ((defense * pItem->m_PlusNum * 10) / 100);
				break;
			case 10:
				defense += ((defense * pItem->m_PlusNum * 15) / 100);
				break;
			case 11:
				defense += ((defense * pItem->m_PlusNum * 20) / 100);
				break;
			case 12:
				defense += ((defense * pItem->m_PlusNum * 25) / 100);
				break;
			case 13:
				defense += ((defense * pItem->m_PlusNum * 30) / 100);
				break;
			case 14:
				defense += ((defense * pItem->m_PlusNum * 35) / 100);
				break;
			case 15:
				defense += ((defense * pItem->m_PlusNum * 40) / 100);
				break;
			default:
				defense += ((defense * pItem->m_PlusNum * 10) / 100);
				break;
			}
		}
		else if(GET_ITEM_LEVEL(pItem)>=181 )
		{
			float add_rate;
			add_rate = defense * (pItem->m_PlusNum *10)/100;
			defense += add_rate;
		}
		//-- 다이아 급이 아닐 경우
		else
		{
			if (pItem->m_PlusNum >= 5)
			{
				//  [5/22/2008 ppmmjj83] 아이템 방어력의 설명과 케릭터 정보 방어력수치 버그 수정
				defense *= 2;
			}
			else if (pItem->m_PlusNum > 0)
			{
				int plus_percent[10] = {0, 12, 26, 44, 68, 100, 100, 100, 100, 100};
				defense += f_defense * float(plus_percent[pItem->m_PlusNum]) / 100.0f + 0.5f;

			}
		}

		int defenseLV400 = DefenseValMgr::GetInstance()->GetDefense(pItem);
		if( defenseLV400 != 0 )
		{
			defense = defenseLV400;
		}

		switch(pItem->m_Vnum)
		{
		case 875:
		case 1027:
		case 1234:
		case 1235:
			if( g_pRoh->m_Wearing[WEARING_WEAPON] && GET_RENDER_FLAG(g_pRoh->m_Wearing[WEARING_WEAPON]) == 0 )
				defense += 70;
			break;
		}

		// 옵션2에 있는 방어구 랜덤 보정의 값이 있으면 계산에 추가한다.
		//if( (pItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION) ) // 방어구 보정치가 있는지의 체크. (이 마스크는 부호비트를 뺀 4비트를 얻어온다.)
		//{
		//	if( (pItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION_SIGN) ) // 부호 비트가 있으면...
		//		defense -= GET_DEFENSE(pItem)*(pItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // -퍼센트. 기본 방어력을 기준으로 계산해서 더해준다.
		//	else
		//		defense += GET_DEFENSE(pItem)*(pItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // +퍼센트. 기본 방어력을 기준으로 계산해서 더해준다.
		//}
	}

	return defense;
}


void CControlScore::SetAbil()
{
	SetStr();
	SetInt();
	SetDex();
	SetCon();
	SetCha();
}

void CControlScore::SetAllScore()
{
	SetName();
	SetSex();
	SetRace();
	SetLevel();
	SetVital();
	SetMana();
	SetStamina();
	SetEpower();
	SetAbil();
	SetExp();
	Set_NeedExp();
	SetLevelupPoint();
	SetMoney();
	SetPenalty();
	SetBP();
	SetLP();
	SetGHP();
	SetFame();

	SetDefense();
}

void CControlScore::CalcPopupIndex(int x, int y)
{
	for(int i = 0; i < 4; ++i)
	{
		if (x >= m_background.m_x + 28 + 26 * i &&
				x <  m_background.m_x + 28 + 26 * (i+1) &&
				y >= m_background.m_y + 395 &&
				y <  m_background.m_y + 395 + 36 )
		{
			m_BuffTextPopup.SetPos( m_background.m_x + 28 + 26 * i,
									m_background.m_y + 395 - 20);
			m_BuffTextPopup.SetString(CDmitronString::Effect(i));

			SetRect(&m_rcDmitronPopup,
					m_background.m_x + 28 + 26 * i - 10,
					m_background.m_y + 395 - 20 - 5,
					m_background.m_x + 28 + 26 * i + 100,
					m_background.m_y + 395 - 20 + 15);

			m_pIsBuffPopup = TRUE;
			return;
		}
	}

	m_pIsBuffPopup = FALSE;
}


void CControlScore::SetSetitemFlag(int where, int flag)
{
	if( where < 0 || where > 3 )
		return ;

	m_SetItemFlag2[where] = flag;

	switch( where )
	{
	case 0:
		SetVital();
		break;	// 생명력
	case 1:
		SetMana();
		break;	// 마나
	case 2:
		SetStamina();
		break;	// 스테미너
	case 3:
		SetEpower();
		break;	// 전기력
	}
}

void CControlScore::LoadRes_Char(bool bForce)
{
	if( bForce == true )
		m_bFaceLoaded = false;

	if (m_bFaceLoaded || !g_pRoh)
		return;

	// 캐릭터 얼굴 이미지
	char* facename[] = { "oneselfinfo_new/bm_face" , "oneselfinfo_new/bw_face"
						 ,"oneselfinfo_new/km_face" , "oneselfinfo_new/kw_face"
						 ,"oneselfinfo_new/am_face" , "oneselfinfo_new/aw_face"
						 ,"oneselfinfo_new/hm_face" , "oneselfinfo_new/hw_face"
						 ,"oneselfinfo_new/fm_face" , "oneselfinfo_new/fw_face"
						 ,"oneselfinfo_new/pm_face" , "oneselfinfo_new/pw_face"
					   };

	int sid = 0;
	switch( g_pRoh->m_Race )
	{
	case RACE_BULKAN: // 불칸
		break;
	case RACE_KAILIPTON: // 카이립톤
		sid = 2;
		break;
	case RACE_AIDIA: // 에이디아
		sid = 4;
		break;
	case RACE_HUMAN: // 휴먼
		sid = 6;
		break;
	case RACE_FREAK: // 하이브리더
		sid = 8;
		break;
	case RACE_PEROM:
		sid = 10;
		break;
	}

	if( g_pRoh->m_Sex == 1 )
		sid++;

	m_face.Init(facename[sid]);
	m_bFaceLoaded = true;
}

void CControlScore::LoadRes_NEW()
{
	m_background.Init("oneselfinfo_new/back_oneselfinfo");
	m_btn_close.Init("common/btn_close_01");
	m_btn_quest.Init("oneselfinfo_new/btn_quest");
	m_btn_pet.Init("oneselfinfo_new/btn_pet");
	//m_defense.Init("oneselfinfo_new/normal_defense");
	//m_status_box.Init("oneselfinfo_new/status_box");

	for (int i = 0; i < LEVELBTN_CNT; ++i)
	{
		m_btn_pointup[i].Init("oneselfinfo_new/btn_up");
		m_btn_pointdown[i].Init("oneselfinfo_new/btn_down");
	}
	m_background.Align( 3 , g_pNk2DFrame->GetClientWidth() , g_pNk2DFrame->GetClientHeight() );

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_face.SetPos(bgx + 28, bgy + 88);
	m_btn_close.SetPosition( bgx + 267 , bgy + 5 );
	m_btn_quest.SetPosition( bgx + 215 , bgy + 320 );
	m_btn_pet.SetPosition(bgx + 215, bgy + 350);
	//m_defense.SetPos(bgx + 28, bgy + 347);
	//m_status_box.SetPos(bgx + 86, bgy + 341);

	int gap_y = 21;
	for(int i=0 ; i<LEVELBTN_CNT ; i++ )
	{
		m_btn_pointup[i].SetPosition( bgx + 171 , bgy + 275 + gap_y*i );
		m_btn_pointdown[i].SetPosition( bgx + 191 , bgy + 275 + gap_y*i );
	}
	
	int yy = -5;

	DWORD text_color = RGB(255, 255, 255);

	m_textout[ScoreTextOut_Type__Name].Init(TextOut_Font__Size, text_color, bgx + 205, bgy + 80, 123, 17, TRUE);
	m_textout[ScoreTextOut_Type__Race].Init(TextOut_Font__Size, text_color, bgx + 205, bgy + 101, 123, 17, TRUE);
	m_textout[ScoreTextOut_Type__Level].Init(TextOut_Font__Size, text_color, bgx + 205, bgy + 122, 123, 17, TRUE);
	m_textout[ScoreTextOut_Type__Penalty].Init(TextOut_Font__Size, text_color, bgx + 205, bgy + 143, 123, 17, TRUE);

	m_textout[ScoreTextOut_Type__Vital].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 180, 176, 17, TRUE);
	m_textout[ScoreTextOut_Type__Mana].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 201, 176, 17, TRUE);
	m_textout[ScoreTextOut_Type__Stamina].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 222, 176, 17, TRUE);
	m_textout[ScoreTextOut_Type__EErg].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 243, 176, 17, TRUE);

	m_textout[ScoreTextOut_Type__Strength].Init(TextOut_Font__Size, text_color, bgx + 129, bgy + 278, 77, 17, TRUE);
	m_textout[ScoreTextOut_Type__Wisdom].Init(TextOut_Font__Size, text_color, bgx + 129, bgy + 298, 77, 17, TRUE);
	m_textout[ScoreTextOut_Type__Dex].Init(TextOut_Font__Size, text_color, bgx + 129, bgy + 320, 77, 17, TRUE);
	m_textout[ScoreTextOut_Type__Strain].Init(TextOut_Font__Size, text_color, bgx + 129, bgy + 341, 77, 17, TRUE);
	m_textout[ScoreTextOut_Type__Charisma].Init(TextOut_Font__Size, text_color, bgx + 129, bgy + 362, 77, 17, TRUE);

	m_textout[ScoreTextOut_Type__LevelUpPoint].Init(TextOut_Font__Size, text_color, bgx + 234, bgy + 299, 65, 17, TRUE);

	m_textout[ScoreTextOut_Type__Def].Init(TextOut_Font__Size, text_color, bgx + 146, bgy + 395, 112, 17, TRUE);

	m_textout[ScoreTextOut_Type__TotalExp].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 426, 176, 17, TRUE);
	m_textout[ScoreTextOut_Type__ExpLevel].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 447, 176, 17, TRUE);

	m_textout[ScoreTextOut_Type__BP].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 479, 176, 17, TRUE);
	m_textout[ScoreTextOut_Type__Fame].Init(TextOut_Font__Size, text_color, bgx + 178, bgy + 501, 176, 17, TRUE);
}

void CControlScore::Draw_NEW()
{
	if (!m_bFaceLoaded)
		LoadRes_Char();

	m_background.Draw();
	m_face.Draw();
	m_btn_close.Draw();
	m_btn_quest.Draw();
	m_btn_pet.Draw();
	//m_defense.Draw();
	//m_status_box.Draw();

	DrawPointUpBtn();

	{
		int cx_01 = 212;
		int cx_02 = 106;
		int cx_03 = 58;
		int cx_04 = 104;

		for (int i = 0; i < TEXTSURFACE_CNT; ++i)
		{
			m_textout[i].Draw();
		}
	}
}

void CControlScore::LoadRes_HPTable()
{
	if (m_bHPTableLoaded || !g_pRoh)
		return;

	LHExcelReader er;
	if (!er.Open("data/level_table.txt"))
	{
		OutputDebugString("File not Found! [HP TABLE]\n");
		return;
	}

	int nRowSize = er.GetRowSize();
	for (int row = 0; row < nRowSize; ++row)
	{
		int col = 1;
		int level = er.GetInt(row, col);
		int race = g_pRoh->m_Race + 2;

		int hp = er.GetInt(row, race);
		m_tableHP[level] = hp;
	}

	m_bHPTableLoaded = true;
}

void CControlScore::LoadRes_OLD()
{	
	m_background.Init( "oneselfinfo/oneselfinfo" );
	m_btn_close.Init( "common/btn_close_01" );
	m_btn_quest.Init( "oneselfinfo/btn_quest" );

	int i;
	for(i=0; i<LEVELBTN_CNT; i++)
	{
		m_btn_pointup[i].Init("common/btn_rarrow");
		m_btn_pointdown[i].Init("common/btn_larrow");
	}
	m_background.Align( 3 , g_pNk2DFrame->GetClientWidth() , g_pNk2DFrame->GetClientHeight() );

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btn_close.SetPosition( bgx + 271 , bgy + 6 );
	m_btn_quest.SetPosition( bgx + 179 , bgy + 291 );

	int gap_y = 22;
	for( i=0 ; i<LEVELBTN_CNT ; i++ )
	{
		m_btn_pointup[i].SetPosition( bgx + 263 , bgy + 157 + gap_y*i );
		m_btn_pointdown[i].SetPosition( bgx + 283 , bgy + 157 + gap_y*i );
	}

	int yy = -5;

	DWORD text_color = RGB(255, 255, 255);

	m_textout[ScoreTextOut_Type__Name].Init(TextOut_Font__Size, text_color, bgx+58, bgy+85+yy);

	m_textout[ScoreTextOut_Type__Race].Init(TextOut_Font__Size, text_color, bgx+58, bgy+109+yy);
	m_textout[ScoreTextOut_Type__Level].Init(TextOut_Font__Size, text_color, bgx+212, bgy+109+yy);
	m_textout[ScoreTextOut_Type__Penalty].Init(TextOut_Font__Size, text_color, bgx+58, bgy+131+yy);
	m_textout[ScoreTextOut_Type__Sex].Init(TextOut_Font__Size, text_color, bgx+212, bgy+131+yy );

	m_textout[ScoreTextOut_Type__Vital].Init(TextOut_Font__Size, text_color, bgx+58, bgy+169+yy);
	m_textout[ScoreTextOut_Type__Strength].Init(TextOut_Font__Size, text_color, bgx+212, bgy+169+yy);
	m_textout[ScoreTextOut_Type__Mana].Init(TextOut_Font__Size, text_color, bgx+58, bgy+191+yy);
	m_textout[ScoreTextOut_Type__Wisdom].Init(TextOut_Font__Size, text_color, bgx+212, bgy+191+yy);
	m_textout[ScoreTextOut_Type__Stamina].Init(TextOut_Font__Size, text_color, bgx+58, bgy+213+yy);
	m_textout[ScoreTextOut_Type__Dex].Init(TextOut_Font__Size, text_color, bgx+212, bgy+213+yy);
	m_textout[ScoreTextOut_Type__EErg].Init(TextOut_Font__Size, text_color, bgx+58, bgy+235+yy);
	m_textout[ScoreTextOut_Type__Strain].Init(TextOut_Font__Size, text_color, bgx+212, bgy+235+yy);
	m_textout[ScoreTextOut_Type__Def].Init(TextOut_Font__Size, text_color, bgx+58, bgy+256+yy);
	m_textout[ScoreTextOut_Type__Charisma].Init(TextOut_Font__Size, text_color, bgx+212, bgy+256+yy);

	m_textout[ScoreTextOut_Type__BP].Init(TextOut_Font__Size, text_color, bgx+58, bgy+295+yy);
	m_textout[ScoreTextOut_Type__GHP].Init(TextOut_Font__Size, text_color, bgx+58, bgy+316+yy);

	m_textout[ScoreTextOut_Type__Fame].Init(TextOut_Font__Size, text_color, bgx+58, bgy+339+yy);
	m_textout[ScoreTextOut_Type__X2].Init(TextOut_Font__Size, text_color, bgx+58, bgy+361+yy);
	m_textout[ScoreTextOut_Type__Stash].Init(TextOut_Font__Size, text_color, bgx+58, bgy+399+yy);
	m_textout[ScoreTextOut_Type__TotalExp].Init(TextOut_Font__Size, text_color, bgx+58, bgy+421+yy);
	m_textout[ScoreTextOut_Type__ExpLevel].Init(TextOut_Font__Size, text_color, bgx+166, bgy+463+yy);
	m_textout[ScoreTextOut_Type__LevelUpPoint].Init(TextOut_Font__Size, text_color, bgx+166, bgy+485+yy);

	m_textout[23].Init(TextOut_Font__Size, text_color, bgx+166, bgy+475 );
	m_textout[24].Init(TextOut_Font__Size, text_color, bgx+166, bgy+475 );
	m_textout[25].Init(TextOut_Font__Size, text_color, bgx+166, bgy+475 );
	m_textout[26].Init(TextOut_Font__Size, text_color, bgx+166, bgy+475 );
	m_textout[27].Init(TextOut_Font__Size, text_color, bgx+166, bgy+475 );
}

void CControlScore::Draw_OLD()
{
	m_background.Draw();
	m_btn_close.Draw();
	m_btn_quest.Draw();

	DrawPointUpBtn();

	{
		int cx_01 = 212;
		int cx_02 = 106;
		int cx_03 = 58;
		int cx_04 = 104;

		m_textout[ScoreTextOut_Type__Name].Draw(cx_01);
		m_textout[ScoreTextOut_Type__Race].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Level].Draw(cx_03);
		m_textout[ScoreTextOut_Type__Penalty].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Sex].Draw(cx_03);
		m_textout[ScoreTextOut_Type__Vital].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Strength].Draw(cx_03);
		m_textout[ScoreTextOut_Type__Mana].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Wisdom].Draw(cx_03);
		m_textout[ScoreTextOut_Type__Stamina].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Dex].Draw(cx_03);
		m_textout[ScoreTextOut_Type__EErg].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Strain].Draw(cx_03);
		m_textout[ScoreTextOut_Type__Def].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Charisma].Draw(cx_03);

		m_textout[ScoreTextOut_Type__BP].Draw(cx_02);
		m_textout[ScoreTextOut_Type__GHP].Draw(cx_02);
		m_textout[ScoreTextOut_Type__Fame].Draw(cx_02);
		m_textout[ScoreTextOut_Type__X2].Draw(cx_02);

		m_textout[ScoreTextOut_Type__Stash].Draw(cx_02);
		m_textout[ScoreTextOut_Type__TotalExp].Draw(cx_02);
		m_textout[ScoreTextOut_Type__ExpLevel].Draw(cx_04);
		m_textout[ScoreTextOut_Type__LevelUpPoint].Draw(cx_04);

		m_textout[23].Draw();
		m_textout[24].Draw();
		m_textout[25].Draw();
		m_textout[26].Draw();
		m_textout[27].Draw();		
	}
}
