#include "stdafx.h"
#include "headers.h"
#include "Nk2DFrame.h"
#include "RShiftMenu.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "tcpipcon.h"
#include "NkCharacter.h"
#include "controlbottom.h"
#include "ControlParty.h"
#include "UIMgr.h"
#include "land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "ControlBottom.h"
#include "AlphaPopUp.h"
#include "Country.h"
#include "CommonConfig.h"

extern DWORD g_dwClientCountry;
extern int g_MatrixSvr;
extern BOOL g_bDungeon;
extern BOOL g_bNewTerrainDungeon;
extern BOOL	g_bFreeUser;

extern void SendAddFriend(const int& order, const std::string& name);

CRShiftMenu::CRShiftMenu()
	: m_pSurfaceBack(NULL)
	, m_pNameText(NULL)
	, m_nRace(-1)
{
	for (int i = eExchange; i < eBtnEnd; ++i)
		m_pBtn[i] = NULL;

	memset(m_UserName, 0, sizeof(m_UserName));
}

CRShiftMenu::~CRShiftMenu()
{
	DeleteRes();
}

void CRShiftMenu::LoadRes()
{
	if( !g_pDisplay )
		return;

	DeleteRes();

	SVR_TYPE SvrType = g_SvrType;
	if( CommonConfig::Instance()->GetSparring() == true )
	{
		SvrType = ST_ADULT_ONLY;
	}

	for (int i = eExchange; i < eBtnEnd; ++i)
		m_pBtn[i] = new CBasicButton();

	m_pBtn[eExchange]->SetFileName("rshiftmenu/btn_excha");
	m_pBtn[eParty]->SetFileName("rshiftmenu/btn_party");	// 파티
	m_pBtn[eRefine]->SetFileName("rshiftmenu/btn_refine");	// 정제
	m_pBtn[eWhisper]->SetFileName("rshiftmenu/btn_whisper");	// 귓말
	m_pBtn[eCustodian]->SetFileName("rshiftmenu/btn_helper");	// 후견인
	if( CommonConfig::Instance()->GetFriendSystem() == true )
	{
		m_pBtn[eFriend]->SetFileName("rshiftmenu/BTN_FRIEND");
	}

	if( SvrType == ST_ADULT_ONLY )
		m_pBtn[ePvP]->SetFileName("rshiftmenu/btn_pvp");	///자유대련

	for (int i = eExchange; i < eBtnEnd; ++i)
		m_pBtn[i]->LoadRes();

	m_pNameText = new CTextOutBox();
	m_pNameText->Init(12, RGB(255, 210, 0),0, 0);

	std::string strMenu1 = "interface/rshiftmenu/rshiftmenu5.bmp";
	std::string strMenu2 = "interface/rshiftmenu/rshiftmenu.bmp";

	if( CommonConfig::Instance()->GetFriendSystem() == true )
	{
		strMenu1 = "interface/rshiftmenu/rshiftmenu.bmp";
		strMenu2 = "interface/rshiftmenu/RSHIFTMENU7.BMP";
	}

	char szMenu1[MAX_PATH] = {0,};
	char szMenu2[MAX_PATH] = {0,};
	sprintf(szMenu1, "%s", strMenu1.c_str());
	sprintf(szMenu2, "%s", strMenu2.c_str());
	
	if( SvrType != ST_ADULT_ONLY )
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, szMenu1);
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, szMenu2);
	}

	m_pSurfaceBack->SetColorKey( TRANS_COLOR );

	CloseMenu();
}

void CRShiftMenu::DeleteRes()
{
	for (int i = eExchange; i < eBtnEnd; ++i)
		SAFE_DELETE(m_pBtn[i]);
	SAFE_DELETE(m_pNameText);
	SAFE_DELETE(m_pSurfaceBack);
}

int CRShiftMenu::GetWidth()
{
	if (m_pSurfaceBack)
		return m_pSurfaceBack->GetWidth();

	return 0;
}

int CRShiftMenu::GetHeight()
{
	if (m_pSurfaceBack)
		return m_pSurfaceBack->GetHeight();

	return 0;
}

void CRShiftMenu::SetXPos(int x)
{
	if (m_pSurfaceBack)
		m_pSurfaceBack->Xpos = x;
}

void CRShiftMenu::SetYPos(int y)
{
	if (m_pSurfaceBack)
		m_pSurfaceBack->Ypos = y;
}

// nate
void CRShiftMenu::Draw()
{
	int x = m_pSurfaceBack->Xpos;
	int y = m_pSurfaceBack->Ypos;

	if( m_pSurfaceBack->Xpos < 0 )
		m_pSurfaceBack->Xpos = 0;

	if( m_pSurfaceBack->Ypos < 0 )
		m_pSurfaceBack->Ypos = 0;

	if( m_pSurfaceBack->Xpos > g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth() )
		m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth();

	if( m_pSurfaceBack->Ypos > g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
		m_pSurfaceBack->Ypos = g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();


	if (m_pSurfaceBack)
		g_pDisplay->Blt(x, y, m_pSurfaceBack);

	SVR_TYPE SvrType = g_SvrType;
	if( CommonConfig::Instance()->GetSparring() == true )
	{
		SvrType = ST_ADULT_ONLY;
	}

	// by evilkiki 2009.12.08 5칸으로 조정
	if( SvrType != ST_ADULT_ONLY )
		m_pNameText->Draw( x + 6, y + 9 , 225 );
	else
		m_pNameText->Draw( x + 6, y + 9 , 270 );

	for (int i = eExchange; i < eBtnEnd; ++i)
		m_pBtn[i]->Draw(x + 9 + (45 * i), y + 28);

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() && g_pNk2DFrame->GetControlBottom()->m_pEtcInfo )
	{
		CMultiText multiText;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		if( m_pBtn[eExchange]->IsInside( point.x , point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -116)
			{
				// IDS_BOTTOM_EXCH : 교환
				multiText.AddString((char *)G_STRING(IDS_BOTTOM_EXCH), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-116);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn[eParty]->IsInside( point.x , point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -117)
			{
				// IDS_BOTTOM_INFO_PARTY : 파티
				multiText.AddString((char *)G_STRING(IDS_BOTTOM_INFO_PARTY), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-117);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn[eRefine]->IsInside( point.x , point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -118)
			{
				// IDS_BOTTOM_INFO_REFINE : 정제
				multiText.AddString((char *)G_STRING(IDS_BOTTOM_INFO_REFINE), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-118);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn[eWhisper]->IsInside( point.x , point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -119)
			{
				// IDS_BOTTOM_WHISPER : 귓속말
				multiText.AddString((char *) G_STRING(IDS_BOTTOM_WHISPER), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-119);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn[eCustodian]->IsInside( point.x , point.y ) )
		{
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -121)
			{
				// IDS_BOTTOM_HELPER : 후견인
				multiText.AddString( (char*)G_STRING(IDS_BOTTOM_HELPER), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-121);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn[eFriend]->IsInside( point.x , point.y ) )
		{	
			if (g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->GetIndex() != -122)
			{				
				multiText.AddString( (char*)G_STRING(IDS_FRIEND_SHORTCUT), _FCOLOR_YELLOW);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->PutString(&multiText);
				g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->SetIndex(-122);
			}
			g_pNk2DFrame->GetControlBottom()->m_pEtcInfo->Draw(point.x, point.y);			
		}
	}
}

BOOL CRShiftMenu::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn(x, y);
}

LRESULT CRShiftMenu::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmp[200];

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		if(!IsInside(x, y))
		{
			CloseMenu();
			return 1;
		}

		for (int i = eExchange; i < eBtnEnd; ++i)
			if (m_pBtn[i] && m_pBtn[i]->GetState() != BTN_DISABLE)
				m_pBtn[i]->MsgProc(hWnd, msg, wParam, lParam);
		return 1;
		break;

	case WM_LBUTTONUP:
		///튜토리얼 예외처리
		if( pCMyApp->m_pUIMgr->IsEventScenePlaying() == 5 )
		{
			return 0;
		}

		for (int i = eExchange; i < eBtnEnd; ++i)
			if (m_pBtn[i])
				m_pBtn[i]->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn[eExchange]->GetState() == BTN_ACTION)
		{
			sprintf(tmp, "exchange %s\n", m_UserName);
			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(tmp);

			m_pBtn[eExchange]->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}
		else if(m_pBtn[eParty]->GetState() == BTN_ACTION)
		{
			// 잠재적 에러 가능성.
			if (g_pRoh->m_nTeamMemGrade == TEAM_BATTLE_AHEAD
					|| g_pRoh->m_nTeamMemGrade == TEAM_BATTLE_BHEAD)
			{
				sprintf(tmp, "g_war char %s\n", m_UserName);
			}
			else
			{
				//sprintf(tmp, "gwar char %s\n", m_UserName);

				int ExpRule = g_pNk2DFrame->GetControlParty()->GetExpRule();

				if( ExpRule == 0 ) //  0일땐 기본설정값으로 셋팅.
					ExpRule = 1;

				int ItemRule = g_pNk2DFrame->GetControlParty()->GetItemRule();

				if( ItemRule == 0 ) //  0일땐 기본설정값으로 셋팅.
					ItemRule = 1;

				sprintf(tmp, "party request %s %d %d\n", m_UserName, ExpRule, ItemRule ); ///파티개편.
			}

			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(tmp);

			m_pBtn[eParty]->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}
		else if(m_pBtn[eRefine]->GetState() == BTN_ACTION)
		{
			if( m_nRace == 2 ) //에이디아
				sprintf(tmp, "refine %s\n", m_UserName);
			else if( m_nRace == 3 ) //휴먼
				sprintf(tmp, "process req %s\n", m_UserName);

			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(tmp);
			m_pBtn[eRefine]->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}
		else if(m_pBtn[eWhisper]->GetState() == BTN_ACTION)
		{

			if (g_SvrMode == eSVR_MASTER)
				return 0;

			// 귓말 체팅창으로
			if(g_pNk2DFrame->GetControlBottom())
			{
				sprintf(tmp,"/%s ", m_UserName);
				g_pNk2DFrame->GetControlBottom()->SetFocus();
				g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
				g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText(tmp);
				m_pBtn[eWhisper]->SetState(BTN_NORMAL);
				CloseMenu();
				return 1;
			}
		}
		else if(m_pBtn[eCustodian]->GetState() == BTN_ACTION)
		{
			sprintf(tmp, "support req %s\n", m_UserName);
			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(tmp);

			m_pBtn[eCustodian]->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}
		else if( m_pBtn[ePvP] &&m_pBtn[ePvP]->GetState() == BTN_ACTION) ///자유대련
		{
			if( g_pRoh->m_Vital != g_pRoh->m_MaxVital ) // HP가 만땅일때만 신청가능
			{
				// IDS_F_PVP_FAIL_HEALTH "HP가 꽉 찾을때만 신청하실 수 있습니다."
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_F_PVP_FAIL_HEALTH), TYPE_NOR_OK, 1 ); // 다른 메세지 왔을때 가리고 있기때문에 일단 보류.
				CloseMenu();
				return 1;
			}

			sprintf(tmp, "pvp 1 %s\n", m_UserName);
			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(tmp);

			char strTemp[256] = "";
			// IDS_F_PVP_PLZ_WAIT "%s 님께 겨루기 신청 메시지를 보내고, 수락을 기다리는 중입니다."
			sprintf( strTemp, (char*)G_STRING(IDS_F_PVP_PLZ_WAIT), m_UserName );
			g_pNk2DFrame->AddChatStr( strTemp, -1); // 채팅창에 남겨준다.

			m_pBtn[ePvP]->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}
		else if( m_pBtn[eFriend] && m_pBtn[eFriend]->GetState() == BTN_ACTION )
		{
			ProcAddFriend();
			return 1;
		}
		break;

	case WM_MOUSEMOVE:
		for (int i = eExchange; i < eBtnEnd; ++i)
			if (m_pBtn[i] && m_pBtn[i]->GetState() != BTN_DISABLE)
				m_pBtn[i]->MsgProc(hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}



void CRShiftMenu::SetCurRace(int race)
{
	m_nRace = race;

	switch(race)
	{
	case 0: // BULKAN (교환, 파티)
	case 1: // KAILPTON(교환 , 파티)
	case 4: // FREAK (교환, 파티)
	case 5: // (교환, 파티)
		m_pBtn[eRefine]->SetState(BTN_DISABLE);
		break;
	case 2: // AIDIA(교환, 파티, 정제)
	case 3: // HUMAN(교환, 파티, 세공)
		break;
	}

// 	if( g_pRoh->m_Level >= 100 || g_MatrixSvr != 0 )
// 	{
// 		m_pBtn5->SetState(BTN_DISABLE);
// 	}

	// connie [2009/2/26] - 일본(0803 Ver.)일때 레벨에 관계없이 서포트 시스템 disable 시킨다.
	if (g_dwClientCountry == CTRY_JPN)
	{
		if( g_pRoh->m_Level >= 0 || g_MatrixSvr != 0 )
			m_pBtn[eCustodian]->SetState(BTN_DISABLE);
	}
	else
	{
		if( g_pRoh->m_Level > 200 || g_MatrixSvr != 0 )
		{
			m_pBtn[eCustodian]->SetState(BTN_DISABLE);
		}
	}

	if( g_MatrixSvr != 0 || g_bDungeon || g_bNewTerrainDungeon
			|| pCMyApp->m_pUIMgr->m_bOnBattle || (pCMyApp->m_nCurWorldIdx==WORLD_DMITRON&&pCMyApp->IsAcropolis()) ) ///자유대련 불가 조건 (공성전,던젼,보스존,매트릭스 등)
	{
		if( m_pBtn[ePvP] )
			m_pBtn[ePvP]->SetState(BTN_DISABLE);
	}
}

void CRShiftMenu::CloseMenu()
{
	g_pNk2DFrame->SetShowRShiftFlag(false);

	for (int i = eExchange; i < eBtnEnd; ++i)
		if (m_pBtn[i])
			m_pBtn[i]->SetState(BTN_NORMAL);
}

void CRShiftMenu::SetUserName(char* name)
{
	if(!m_pNameText)
		return;

	if (g_SvrMode == eSVR_MASTER)
		return;

	m_pNameText->SetString(name);
	strcpy(m_UserName, name);
}

void CRShiftMenu::ProcAddFriend()
{
	if (g_SvrMode == eSVR_MASTER)
		return;

	SendAddFriend(1, m_UserName);

	m_pBtn[eFriend]->SetState(BTN_NORMAL);

	CloseMenu();
}