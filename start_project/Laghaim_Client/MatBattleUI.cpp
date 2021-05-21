#include "stdafx.h"


#include "dxutil.h"
#include "main.h"
#include "basicbutton.h"
#include "spinbox.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "ListBox.h"
#include "NewListBox.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "MatBattleUI.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "ControlShop.h"
#include "uimgr.h"






extern HINSTANCE			g_hDllInst;

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(50, 50, 50 );
static DWORD gray_color = RGB(155, 155, 155);

#define MAT_LIST_TITLE_X	100
#define MAT_LIST_TITLE_Y	75

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatBattleUI::CMatBattleUI()
{
	memset(m_commOut, 0, sizeof(m_commOut));
	memset(m_MasterName, 0, sizeof(m_MasterName));
	memset(m_MyName, 0, sizeof(m_MyName));

	if (!g_pDisplay)
		return;

	// ----------------------------------------------------------
	// GET SCREEN SIZE
	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;

	//------------------------------------------------------------
	//	BACKGROUND
	m_pBack		=  NULL;
	m_pSurNonMaster = NULL;
	//------------------------------------------------------------
	//	BUTTON
	m_pBtnOut = new CBasicButton();
	m_pBtnBeA = new CBasicButton();
	m_pBtnBeB = new CBasicButton();
	m_pBtnWait = new CBasicButton();
	m_pBtnStart = new CBasicButton();
	m_pBtnShop = new CBasicButton();
	m_pBtnExit = new CBasicButton();

	m_pBtnOut->SetFileName("battle/btn_room_out");
	m_pBtnBeA->SetFileName("battle/btn_sel_a");
	m_pBtnBeB->SetFileName("battle/btn_sel_b");
	m_pBtnWait->SetFileName("battle/btn_wait");
	m_pBtnStart->SetFileName("battle/btn_start");
	m_pBtnShop->SetFileName("battle/btn_shop");
	m_pBtnExit->SetFileName("battle/btn_exit");

	m_bRoomOwner = FALSE;
	m_pMsgPopUp = NULL;

	m_WaitList = NULL;
	m_WaitList = new CNewListBox(" ", 0,0,0,0/*X+26+6, Y+50-28,	150, 56*/);
	if( m_WaitList )
	{
		m_WaitList->SetSelectedLine(-1);
		m_WaitList->SetTextColor(RGB(255, 255, 0));
		m_WaitList->SetFont(0, 8, "굴림체");
	}

	m_BlueTeamList = NULL; // A팀은 원래 홍팀이다.
	m_BlueTeamList = new CNewListBox(" ", 0,0,0,0);
	if( m_BlueTeamList )
	{
		m_BlueTeamList->SetSelectedLine(-1);
		m_BlueTeamList->SetTextColor(RGB(255, 100, 100));
		m_BlueTeamList->SetFont(0, 8, "굴림체");
	}

	m_RedTeamList = NULL;
	m_RedTeamList = new CNewListBox(" ", 0,0,0,0);
	if( m_RedTeamList )
	{
		m_RedTeamList->SetSelectedLine(-1);
		m_RedTeamList->SetTextColor(RGB(100, 100, 255));
		m_RedTeamList->SetFont(0, 8, "굴림체");
	}

	m_WaitSelectLst = -1;
	m_BlueSelectLst = -1;
	m_RedSelectLst = -1;

	m_bMaster = FALSE;
	m_bReady = FALSE;
	m_bBlueTeamSelected = FALSE;
	m_bRedTeamSelected = FALSE;

	m_pWaitNumText = NULL;
	m_nBlueNumText = NULL;
	m_pRedNumText = NULL;
	m_pWaitNumText = new CTextOutBox();
	m_nBlueNumText = new CTextOutBox();
	m_pRedNumText = new CTextOutBox();
	if( m_pWaitNumText )
		m_pWaitNumText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_nBlueNumText )
		m_nBlueNumText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pRedNumText )
		m_pRedNumText->Init(13, RGB(255, 255, 255),0, 0);
	m_bActive = true;

	LoadRes();
}

void CMatBattleUI::LoadRes()
{
	if( !g_pDisplay )
		return;

	if(pCMyApp->m_pUIMgr->m_bIsMatBattle)
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/battle/back_battle.bmp");
	else if(pCMyApp->m_pUIMgr->m_bIsMatDeath)
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/battle/back_death.bmp");

	if( !m_pBack )
		return;

	m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth();
	m_pBack->Ypos = 0;

	int X = m_iMainX = m_pBack->Xpos;
	int Y = m_iMainY =m_pBack->Ypos;

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurNonMaster, "interface/battle/room_out_unable.bmp");
	if( m_pSurNonMaster )
	{
		m_pSurNonMaster->Xpos = X+195-3;
		m_pSurNonMaster->Ypos = Y+30+18;
	}

	if( m_pBtnOut )		m_pBtnOut->LoadRes();
	if( m_pBtnBeA )		m_pBtnBeA->LoadRes();
	if( m_pBtnBeB )		m_pBtnBeB->LoadRes();
	if( m_pBtnWait )	m_pBtnWait->LoadRes();
	if( m_pBtnStart )	m_pBtnStart->LoadRes();
	if( m_pBtnShop )	m_pBtnShop->LoadRes();
	if( m_pBtnExit )	m_pBtnExit->LoadRes();
	if (m_pMsgPopUp)	m_pMsgPopUp->LoadRes();

	if( m_WaitList )		m_WaitList->SetImage("common/arrowdown", "common/arrowup", "interface/common/cute_Bar2.bmp");
	if( m_BlueTeamList )	m_BlueTeamList->SetImage("common/arrowdown", "common/arrowup", "interface/common/cute_Bar2.bmp");
	if( m_RedTeamList )		m_RedTeamList->SetImage("common/arrowdown", "common/arrowup", "interface/common/cute_Bar2.bmp");
	if( m_WaitList )		m_WaitList->LoadRes();
	if( m_BlueTeamList )	m_BlueTeamList->LoadRes();
	if( m_RedTeamList )		m_RedTeamList->LoadRes();

	if( m_pWaitNumText )	m_pWaitNumText->Restore();
	if( m_nBlueNumText )	m_nBlueNumText->Restore();
	if( m_pRedNumText )		m_pRedNumText->Restore();

	if( m_pBtnOut )		m_pBtnOut->SetPosition(X+195-6, Y+30+18);
	if( m_pBtnBeA )		m_pBtnBeA->SetPosition(X+198-9, Y+125+5);
	if( m_pBtnBeB )		m_pBtnBeB->SetPosition(X+199-10, Y+251-24);
	if( m_pBtnWait )	m_pBtnWait->SetPosition(X+19-5, Y+341-31);
	if( m_pBtnStart )	m_pBtnStart->SetPosition(X+19-5, Y+341-31);
	if( m_pBtnShop )	m_pBtnShop->SetPosition(X+110-13, Y+340-30);
	if( m_pBtnExit )	m_pBtnExit->SetPosition(X+184-5, Y+341-31);

	if( m_WaitList )		m_WaitList->SetPosDim(X+26+6, Y+50-28,	150/*127*/, 56+33);
	if( m_BlueTeamList )	m_BlueTeamList->SetPosDim(X+26+6, Y+50-28+67,	150/*127*/, 85+33);
	if( m_RedTeamList )		m_RedTeamList->SetPosDim(X+26+6, Y+50-28+67+96,	150/*127*/, 85+33);
}

CMatBattleUI::~CMatBattleUI()
{
	DeleteRes();
	SAFE_DELETE(m_pBtnOut);
	SAFE_DELETE(m_pBtnBeA);
	SAFE_DELETE(m_pBtnBeB);
	SAFE_DELETE(m_pBtnWait);
	SAFE_DELETE(m_pBtnStart);
	SAFE_DELETE(m_pBtnShop);
	SAFE_DELETE(m_pBtnExit);
	SAFE_DELETE(m_pMsgPopUp);
	SAFE_DELETE(m_WaitList);
	SAFE_DELETE(m_BlueTeamList);
	SAFE_DELETE(m_RedTeamList);
	SAFE_DELETE(m_pWaitNumText);
	SAFE_DELETE(m_nBlueNumText);
	SAFE_DELETE(m_pRedNumText);
}


void CMatBattleUI::DeleteRes()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pSurNonMaster);

	if( m_pBtnOut )		m_pBtnOut->DeleteRes();
	if( m_pBtnBeA )		m_pBtnBeA->DeleteRes();
	if( m_pBtnBeB )		m_pBtnBeB->DeleteRes();
	if( m_pBtnWait )	m_pBtnWait->DeleteRes();
	if( m_pBtnStart )	m_pBtnStart->DeleteRes();
	if( m_pBtnShop )	m_pBtnShop->DeleteRes();
	if( m_pBtnExit )	m_pBtnExit->DeleteRes();
	if (m_pMsgPopUp)	m_pMsgPopUp->DeleteRes();

	if( m_WaitList )	m_WaitList->DeleteRes();
	if( m_BlueTeamList )m_BlueTeamList->DeleteRes();
	if( m_RedTeamList )	m_RedTeamList->DeleteRes();

	if( m_pWaitNumText )	m_pWaitNumText->DeleteRes();
	if( m_nBlueNumText )	m_nBlueNumText->DeleteRes();
	if( m_pRedNumText )		m_pRedNumText->DeleteRes();
}

void CMatBattleUI::Draw()
{
	char tmp[10] = "";

	if (!m_bActive || !g_pDisplay || !m_pBack)
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	if(m_bMaster && m_pBtnOut)
		m_pBtnOut->Draw();
	else if( m_pSurNonMaster )
		g_pDisplay->Blt(m_pSurNonMaster->Xpos, m_pSurNonMaster->Ypos, m_pSurNonMaster);

	if( m_pBtnBeA )	m_pBtnBeA->Draw();
	if( m_pBtnBeB )	m_pBtnBeB->Draw();

	if(m_bMaster && m_pBtnStart)
		m_pBtnStart->Draw();
	else if( m_pBtnWait )
		m_pBtnWait->Draw();

	if( m_pBtnShop )	m_pBtnShop->Draw();
	if( m_pBtnExit )	m_pBtnExit->Draw();
	if( m_WaitList )	m_WaitList->Draw();
	if( m_BlueTeamList )m_BlueTeamList->Draw();
	if( m_RedTeamList )	m_RedTeamList->Draw();

	if( m_pWaitNumText )	m_pWaitNumText->SetString(itoa(m_WaitList->TotalLine,tmp,10));
	if( m_nBlueNumText )	m_nBlueNumText->SetString(itoa(m_BlueTeamList->TotalLine,tmp,10));
	if( m_pRedNumText )		m_pRedNumText->SetString(itoa(m_RedTeamList->TotalLine,tmp,10));

	if( m_pWaitNumText )	m_pWaitNumText->Draw(m_pBack->Xpos+217,m_pBack->Ypos+93);
	if( m_nBlueNumText )	m_nBlueNumText->Draw(m_pBack->Xpos+217,m_pBack->Ypos+176);
	if( m_pRedNumText )		m_pRedNumText->Draw(m_pBack->Xpos+217,m_pBack->Ypos+272);
}

LRESULT CMatBattleUI::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActive || !g_pDisplay || !m_pBack)
		return 0;

	int x = 0, y = 0, sel = -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	if(m_pMsgPopUp && m_pMsgPopUp->CheckMsgExist() == TRUE)
	{
		if(msg == WM_MOVE)
			return 0;

		m_pMsgPopUp->MsgProc(hWnd, msg, wParam, lParam);
		return 1;
	}

	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		if(IsInside(x, y))
		{
			if( m_pBtnOut )	m_pBtnOut->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBeA )	m_pBtnBeA->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBeB )	m_pBtnBeB->MsgProc(hWnd, msg, wParam, lParam);
			if(m_bMaster && m_pBtnStart)
				m_pBtnStart->MsgProc(hWnd, msg, wParam, lParam);
			else if( m_pBtnWait )
				m_pBtnWait->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnShop )	m_pBtnShop->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExit )	m_pBtnExit->MsgProc(hWnd, msg, wParam, lParam);

			if( m_WaitList )
				sel = m_WaitList->GetMessage(hWnd, msg, wParam, lParam);
			if(sel != -1)
			{
				m_WaitSelectLst = sel;
				m_BlueSelectLst = -1;
				m_RedSelectLst = -1;
				m_BlueTeamList->UpdateList();
				m_RedTeamList->UpdateList();
			}
			if( m_BlueTeamList )
				sel = m_BlueTeamList->GetMessage(hWnd, msg, wParam, lParam);
			if(sel != -1)
			{
				m_BlueSelectLst = sel;
				m_WaitSelectLst = -1;
				m_RedSelectLst = -1;
				m_WaitList->UpdateList();
				m_RedTeamList->UpdateList();
			}
			if( m_RedTeamList )
				sel = m_RedTeamList->GetMessage(hWnd, msg, wParam, lParam);
			if(sel != -1)
			{
				m_RedSelectLst = sel;
				m_BlueSelectLst = -1;
				m_WaitSelectLst = -1;
				m_BlueTeamList->UpdateList();
				m_WaitList->UpdateList();
			}

			return 4;
		}
		else
			return 2;

		break;

	case WM_LBUTTONUP:

		if(m_bActive)
		{
			if( m_pBtnOut )	m_pBtnOut->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBeA )	m_pBtnBeA->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBeB )	m_pBtnBeB->MsgProc(hWnd, msg, wParam, lParam);
			if(m_bMaster && m_pBtnStart)
				m_pBtnStart->MsgProc(hWnd, msg, wParam, lParam);
			else if( m_pBtnWait )
				m_pBtnWait->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnShop )	m_pBtnShop->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExit )	m_pBtnExit->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtnOut && m_pBtnOut->m_iBtnFlg == BTN_ACTION)
			{

				if(	m_RedTeamList && m_RedSelectLst != -1)
				{
					if(strcmp(m_MyName,m_RedTeamList->LineText[m_RedSelectLst]))
					{
						sprintf(m_commOut, "troom kick %s\n", m_RedTeamList->LineText[m_RedSelectLst]);
						if(g_pTcpIp)
							g_pTcpIp->SendNetMessage(m_commOut);
					}
					return 1;
				}
				if(m_BlueTeamList && m_BlueSelectLst != -1)
				{
					if(strcmp(m_MyName,m_BlueTeamList->LineText[m_BlueSelectLst]))
					{
						sprintf(m_commOut, "troom kick %s\n", m_BlueTeamList->LineText[m_BlueSelectLst]);
						if(g_pTcpIp)
							g_pTcpIp->SendNetMessage(m_commOut);
					}
					return 1;
				}
				if(m_WaitList && m_WaitSelectLst != -1)
				{
					if(strcmp(m_MyName,m_WaitList->LineText[m_WaitSelectLst]))
					{
						sprintf(m_commOut, "troom kick %s\n", m_WaitList->LineText[m_WaitSelectLst]);
						if(g_pTcpIp)
							g_pTcpIp->SendNetMessage(m_commOut);
					}
					return 1;
				}

				return 1;
			}
			else if(m_pBtnBeA && m_pBtnBeA->m_iBtnFlg == BTN_ACTION)
			{
				if(!m_bBlueTeamSelected && !m_bReady)
				{
					m_bBlueTeamSelected = TRUE;
					m_bRedTeamSelected = FALSE;
					sprintf(m_commOut, "troom be_a\n");
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage(m_commOut);
				}

				return 1;
			}
			else if (m_pBtnBeB && m_pBtnBeB->m_iBtnFlg == BTN_ACTION)
			{
				if(!m_bRedTeamSelected  && !m_bReady)
				{
					m_bRedTeamSelected = TRUE;
					m_bBlueTeamSelected = FALSE;
					sprintf(m_commOut, "troom be_b\n");
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage(m_commOut);
				}
				return 1;
			}
			else if (m_pBtnWait && m_pBtnWait->m_iBtnFlg == BTN_ACTION)
			{
				if(m_bBlueTeamSelected || m_bRedTeamSelected)
				{
					if(!m_bReady)
					{
						m_bReady = TRUE;
						sprintf(m_commOut, "troom ready\n");
					}
					else
					{
						m_bReady = FALSE;
						sprintf(m_commOut, "troom wait\n");
					}
					if(m_bReady)
						m_pBtnWait->m_iBtnFlg = BTN_DOWN;
					else
						m_pBtnWait->m_iBtnFlg = BTN_NORMAL;
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage(m_commOut);
				}
				return 1;

			}
			else if (m_pBtnStart && m_pBtnStart->m_iBtnFlg == BTN_ACTION)  	// Start BTN
			{
				if (pCMyApp->m_pUIMgr->m_bIsMatBattle || pCMyApp->m_pUIMgr->m_bIsMatDeath)
				{
					sprintf(m_commOut, "troom start\n");
					if(g_pTcpIp)
						g_pTcpIp->SendNetMessage(m_commOut);
				}
				return 1;

			}
			else if (m_pBtnShop && m_pBtnShop->m_iBtnFlg == BTN_ACTION)
			{
				if(m_bReady)
					return 1;
				m_pBtnShop->m_iBtnFlg = BTN_NORMAL;
				g_pNk2DFrame->ToggleShopWindow();
				if (g_pNk2DFrame->IsShopVisible())
				{
					g_pNk2DFrame->GetControlShop()->SetKeeper(-1);
				}
				if (g_pRoh->m_Race == RACE_BULKAN)  					// 불칸
				{
					sprintf(m_commOut, "shopidx 159\n");
					g_pNk2DFrame->GetControlShop()->SetShopIdx(159);
				}
				else if (g_pRoh->m_Race == RACE_KAILIPTON)  			// 카이
				{
					sprintf(m_commOut, "shopidx 166\n");			// 157도 가능할 지 모름.
					g_pNk2DFrame->GetControlShop()->SetShopIdx(166);
				}
				else if (g_pRoh->m_Race == RACE_AIDIA)  			// 에이디아
				{
					sprintf(m_commOut, "shopidx 160\n");
					g_pNk2DFrame->GetControlShop()->SetShopIdx(160);
				}
				else  											// 휴먼
				{
					sprintf(m_commOut, "shopidx 156\n");
					g_pNk2DFrame->GetControlShop()->SetShopIdx(156);
				}
				if(g_pTcpIp)
					g_pTcpIp->SendNetMessage(m_commOut);

				return 1;
			}
			else if (m_pBtnExit && m_pBtnExit->m_iBtnFlg == BTN_ACTION)
			{
				sprintf(m_commOut, "leave_room\n");
				if(g_pTcpIp)
					g_pTcpIp->SendNetMessage(m_commOut);
				if(!m_bMaster && m_bReady)
				{
					return 1;
				}
				else
				{
					g_pRoh->m_nTeamMemGrade = TEAM_BATTLE_NONE;
					return 1;
				}
			}

			if( m_WaitList )	m_WaitList->GetMessage(hWnd, msg, wParam, lParam );
			if( m_BlueTeamList )m_BlueTeamList->GetMessage(hWnd, msg, wParam, lParam);
			if( m_RedTeamList )	m_RedTeamList->GetMessage(hWnd, msg, wParam, lParam);

			if( m_pBtnOut )		m_pBtnOut->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnBeA )		m_pBtnBeA->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnBeB )		m_pBtnBeB->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnStart )	m_pBtnStart->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnShop )	m_pBtnShop->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnExit )	m_pBtnExit->m_iBtnFlg = BTN_NORMAL;
		}
		return 1;

	case WM_MOUSEMOVE:

		if(IsInside(x, y))
		{
			if( m_pBtnOut )	m_pBtnOut->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBeA )	m_pBtnBeA->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnBeB )	m_pBtnBeB->MsgProc(hWnd, msg, wParam, lParam);
			if(m_bMaster && m_pBtnStart)
				m_pBtnStart->MsgProc(hWnd, msg, wParam, lParam);
			else if( m_pBtnWait )
			{
				if(m_bReady)
					m_pBtnWait->m_iBtnFlg = BTN_DOWN;
				else
					m_pBtnWait->m_iBtnFlg = BTN_NORMAL;
			}
			if( m_pBtnShop )	m_pBtnShop->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExit )	m_pBtnExit->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;

	case WM_KEYDOWN:
	case WM_CHAR:
		return 0;

	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		return 1;
	}
	return 0;
}

BOOL CMatBattleUI::IsInside(int x, int y)
{
	if( m_pBack )
	{
		if (x >= (int)(m_pBack->Xpos)
				&& x < (int)(m_pBack->Xpos + m_pBack->GetWidth())
				&& y >= (int)(m_pBack->Ypos)
				&& y < (int)(m_pBack->Ypos + m_pBack->GetHeight()))
			return TRUE;
	}
	return FALSE;
}

void CMatBattleUI::TRoom(char *cmd, char *strName)
{}

void CMatBattleUI::AddMember(char *strName, int which_loc, BOOL bWaiting, BOOL bOwner)
{
	if( strName == NULL )
		return;

	switch(which_loc)
	{
	case -1: //대기
		if( m_WaitList )
		{
			m_WaitList->AddString(strName);
			m_WaitList->UpdateList();
		}
		break;
	case 0://a팀
		if( m_BlueTeamList )
		{
			m_BlueTeamList->AddString(strName);
			m_BlueTeamList->UpdateList();
		}
		break;
	case 1://b팀
		if( m_RedTeamList )
		{
			m_RedTeamList->AddString(strName);
			m_RedTeamList->UpdateList();
		}
		break;
	}

	if(!bWaiting)//대기상태라면,
		FindCha(strName, FALSE, FALSE);
	else
		FindCha(strName, TRUE, FALSE);

	if(bOwner)
	{
		strcpy(m_MasterName,strName);
		FindCha(strName, FALSE, TRUE); //방장의 색깔
	}
}

void CMatBattleUI::RoomUpdate(int what_cmd, char *strName)
{
	if( strName == NULL )
		return;

	switch(what_cmd)
	{
	//case 0: //내가 나갔다.
	//	break;
	case 1: //사람이 나갔다.
		DeleteCha(strName);
		break;
	case 2: //레디 했다.
		FindCha(strName, TRUE, FALSE);
		break;
	case 3: // 대기
		FindCha(strName, FALSE, FALSE);
		break;
	case 4: //샾
		break;
	case 5: //A팀으로 이동
		DeleteCha(strName);
		m_BlueTeamList->AddString(strName);
		if(!strcmp(m_MasterName,strName)) //방장이라면
			FindCha(strName, FALSE, TRUE);
		else
			FindCha(strName, FALSE, FALSE);

		if( m_BlueTeamList )	m_BlueTeamList->UpdateList();

		break;
	case 6://B팀으로 이동
		DeleteCha(strName);
		m_RedTeamList->AddString(strName);
		if(!strcmp(m_MasterName,strName)) //방장이라면
			FindCha(strName, FALSE, TRUE);
		else
			FindCha(strName, FALSE, FALSE);

		if( m_RedTeamList )	m_RedTeamList->UpdateList();

		break;
	case 7://방장됐다.
		if(!strcmp(m_MyName, strName))
		{
			m_bMaster = TRUE;
			m_bReady = FALSE;
		}

		strcpy(m_MasterName,strName);
		FindCha(strName, FALSE, TRUE); //방장의 색깔
		break;
	}
}

void CMatBattleUI::DeleteCha(char *strName)
{
	if( strName == NULL )
		return;

	int i;

	if( m_WaitList )
	{
		for(i=0; i<m_WaitList->TotalLine; i++)
		{
			if(!strcmp(m_WaitList->LineText[i],strName))
			{
				m_WaitList->DelString (i);
				if(m_WaitSelectLst == i)//선택되어있던 캐릭이 나갔다면,
					m_WaitSelectLst = -1; //커서 초기화.
				m_WaitList->UpdateList();
				return;
			}
		}
	}
	if( m_BlueTeamList )
	{
		for(i=0; i<m_BlueTeamList->TotalLine; i++)
		{
			if(!strcmp(m_BlueTeamList->LineText[i],strName))
			{
				m_BlueTeamList->DelString (i);
				if(m_BlueSelectLst == i)
					m_BlueSelectLst = -1; //커서 초기화.
				m_BlueTeamList->UpdateList();
				return;
			}
		}
	}
	if( m_RedTeamList )
	{
		for(i=0; i<m_RedTeamList->TotalLine; i++)
		{
			if(!strcmp(m_RedTeamList->LineText[i],strName))
			{
				m_RedTeamList->DelString (i);
				if(m_RedSelectLst == i)
					m_RedSelectLst = -1; //커서 초기화.
				m_RedTeamList->UpdateList();
				return;
			}
		}
	}
}

void CMatBattleUI::FindCha(char *strName, BOOL bReady, BOOL bMater)
{
	if( strName == NULL )
		return;

	int i = 0;

	if( m_BlueTeamList )
	{
		for(i=0; i<m_BlueTeamList->TotalLine; i++)
		{
			if(!strcmp(m_BlueTeamList->LineText[i],strName))
			{
				if(bMater)  //방장
				{
					m_BlueTeamList->SetColorString (i, 3);
					return;
				}

				if(bReady)
					m_BlueTeamList->SetColorString (i, 0); //레디
				else
					m_BlueTeamList->SetColorString (i, 1);//wait블루색 복귀
				return;
			}
		}
	}
	if( m_RedTeamList )
	{
		for(i=0; i<m_RedTeamList->TotalLine; i++)
		{
			if(!strcmp(m_RedTeamList->LineText[i],strName))
			{
				if(bMater)
				{
					m_RedTeamList->SetColorString (i, 3);
					return;
				}

				if(bReady)
					m_RedTeamList->SetColorString (i, 0); //레디
				else
					m_RedTeamList->SetColorString (i, 2);//wait 레드색 복귀

				return;
			}
		}
	}
	if( m_WaitList )
	{
		for(i=0; i<m_WaitList->TotalLine; i++)
		{
			if(!strcmp(m_WaitList->LineText[i],strName))
			{
				if(bMater)
				{
					m_WaitList->SetColorString (i, 3);
					return;
				}
			}
		}
	}
}
