#include "stdafx.h"


#include "main.h"
#include "basicbutton.h"
#include "spinbox.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "NewListBox.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "MatSurvivalUI.h"
#include "nk2dframe.h"
#include "controlshop.h"
#include "controlbottom.h"
#include "uimgr.h"






extern HINSTANCE  g_hDllInst;

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(50, 50, 50 );
static DWORD gray_color = RGB(155, 155, 155);

#define MAT_LIST_TITLE_X	100
#define MAT_LIST_TITLE_Y	75

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatSurvivalUI::CMatSurvivalUI()
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
	m_pBackTitle	=  NULL;
	m_pSurNonMaster = NULL;

	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/battle/back_survival.bmp");
	if( !m_pBack )
		return;

	m_pBack->Xpos = m_ScreenWidth - m_pBack->GetWidth();
	m_pBack->Ypos = 35-6;
	int X = m_iMainX = m_pBack->Xpos;
	int Y = m_iMainY =m_pBack->Ypos;

	if(pCMyApp->m_pUIMgr->m_bIsMatSurvival)
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBackTitle, "interface/battle/title_Survival.bmp");
	else if(pCMyApp->m_pUIMgr->m_bIsMatSurvivalMon)
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBackTitle, "interface/battle/title_MonSurvival.bmp");
	else if(pCMyApp->m_pUIMgr->m_bIsMatChat)
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBackTitle, "interface/battle/title_Chat.bmp");

	if( m_pBackTitle )
	{
		m_pBackTitle->Xpos = X + 22;
		m_pBackTitle->Ypos = Y + 25;
	}

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurNonMaster, "interface/battle/room_out_unable.bmp");
	if( m_pSurNonMaster )
	{
		m_pSurNonMaster->Xpos = X+195-3;
		m_pSurNonMaster->Ypos = Y+30+18;
	}

	//------------------------------------------------------------
	//	BUTTON
	m_pBtnOut = new CBasicButton();
	m_pBtnWait = new CBasicButton();
	m_pBtnStart = new CBasicButton();
	m_pBtnShop = new CBasicButton();
	m_pBtnExit = new CBasicButton();

	if( m_pBtnOut )		m_pBtnOut->SetFileName("battle/room_out");
	if( m_pBtnWait )	m_pBtnWait->SetFileName("battle/wait");
	if( m_pBtnStart )	m_pBtnStart->SetFileName("battle/start");
	if( m_pBtnShop )	m_pBtnShop->SetFileName("battle/shop");
	if( m_pBtnExit )	m_pBtnExit->SetFileName("battle/exit");

	if( m_pBtnOut )		m_pBtnOut->LoadRes();
	if( m_pBtnWait )	m_pBtnWait->LoadRes();
	if( m_pBtnStart )	m_pBtnStart->LoadRes();
	if( m_pBtnShop )	m_pBtnShop->LoadRes();
	if( m_pBtnExit )	m_pBtnExit->LoadRes();

	if( m_pBtnOut )		m_pBtnOut->SetPosition(X+195-3, Y+30+18);
	if( m_pBtnWait )	m_pBtnWait->SetPosition(X+19-5, Y+341-31);
	if( m_pBtnStart )	m_pBtnStart->SetPosition(X+19-5, Y+341-31);
	if( m_pBtnShop )	m_pBtnShop->SetPosition(X+110-13, Y+340-30);
	if( m_pBtnExit )	m_pBtnExit->SetPosition(X+184-5, Y+341-31);

	m_bRoomOwner = FALSE;
	m_pMsgPopUp = NULL;

//////////

	m_WaitList = NULL;
	m_WaitList = new CNewListBox(" ", 0,0,0,0/*X+26+6, Y+50-28,	150, 56*/);
	if( m_WaitList )
	{
		m_WaitList->SetImage("battle/arrowdown", "battle/arrowup", "interface/battle/cute_Bar2.bmp");
		m_WaitList->SetSelectedLine(-1);
		m_WaitList->SetTextColor(RGB(255, 255, 0));
		m_WaitList->SetFont(0, 8, "굴림체");
		m_WaitList->SetPosDim(X+26+6, Y+50-28,	150/*127*/, 271+33-23);
		m_WaitList->LoadRes();
	}
	m_WaitSelectLst = -1;
	m_bMaster = FALSE;
	m_bReady = FALSE;
	m_pWaitNumText = NULL;
	m_pWaitNumText = new CTextOutBox();
	if( m_pWaitNumText )
		m_pWaitNumText->Init(13, RGB(255, 255, 255),0, 0);
	m_bActive = TRUE;
}

CMatSurvivalUI::~CMatSurvivalUI()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBackTitle);
	SAFE_DELETE(m_pSurNonMaster);
	SAFE_DELETE(m_pBtnOut);
	SAFE_DELETE(m_pBtnWait);
	SAFE_DELETE(m_pBtnStart);
	SAFE_DELETE(m_pBtnShop);
	SAFE_DELETE(m_pBtnExit);
	SAFE_DELETE(m_pMsgPopUp);
	SAFE_DELETE(m_WaitList);
	SAFE_DELETE(m_pWaitNumText);
}

void CMatSurvivalUI::LoadRes()
{}

void CMatSurvivalUI::DeleteRes()
{}

void CMatSurvivalUI::Draw()
{
	char tmp[10] = "";

	if (!m_bActive || !m_pBack || !g_pDisplay)
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	g_pDisplay->Blt(m_pBackTitle->Xpos, m_pBackTitle->Ypos, m_pBackTitle);


	if(m_bMaster && m_pBtnOut)
		m_pBtnOut->Draw();
	else if( m_pSurNonMaster )
		g_pDisplay->Blt(m_pSurNonMaster->Xpos, m_pSurNonMaster->Ypos, m_pSurNonMaster);

	if(m_bMaster && m_pBtnStart)
		m_pBtnStart->Draw();
	else if( m_pBtnWait )
		m_pBtnWait->Draw();

	if( m_pBtnShop )	m_pBtnShop->Draw();
	if( m_pBtnExit )	m_pBtnExit->Draw();
	if( m_WaitList )	m_WaitList->Draw();
	if( m_pWaitNumText )	m_pWaitNumText->SetString(itoa(m_WaitList->TotalLine,tmp,10));
	if( m_pWaitNumText )	m_pWaitNumText->Draw(m_pBack->Xpos+217,m_pBack->Ypos+95);

}

LRESULT CMatSurvivalUI::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y, sel = -1;//, i;

	if (!m_bActive || !m_pBack || !g_pDisplay)
		return 0;

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

			if(m_pBtnStart && m_bMaster)
				m_pBtnStart->MsgProc(hWnd, msg, wParam, lParam);
			else if( m_pBtnWait )
				m_pBtnWait->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtnShop )	m_pBtnShop->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExit )	m_pBtnExit->MsgProc(hWnd, msg, wParam, lParam);

			if( m_WaitList )
				sel = m_WaitList->GetMessage(hWnd, msg, wParam, lParam);

			if(sel != -1)
				m_WaitSelectLst = sel;

			return 4;
		}
		else
			return 2;

		break;

	case WM_LBUTTONUP:

		if(m_bActive)
		{
			if( m_pBtnOut )	m_pBtnOut->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtnStart && m_bMaster)
				m_pBtnStart->MsgProc(hWnd, msg, wParam, lParam);
			else if( m_pBtnWait )
				m_pBtnWait->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtnShop )	m_pBtnShop->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnExit )	m_pBtnExit->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtnOut && m_pBtnOut->m_iBtnFlg == BTN_ACTION)
			{

				if(m_WaitSelectLst != -1)
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
			else if (m_pBtnWait && m_pBtnWait->m_iBtnFlg == BTN_ACTION)
			{
				if (pCMyApp->m_pUIMgr->m_bIsMatChat)				// 일반 대화방의 경우 기능 정지.
					return 1;

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

				return 1;

			}
			else if (m_pBtnStart && m_pBtnStart->m_iBtnFlg == BTN_ACTION)
			{
				if (!pCMyApp->m_pUIMgr->m_bIsMatChat)  				// 일반 대화방의 경우 스타트가 없다.
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
					sprintf(m_commOut, "shopidx 166\n");
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
			else if (m_pBtnExit->m_iBtnFlg == BTN_ACTION)
			{
				sprintf(m_commOut, "leave_room\n");
				if(g_pTcpIp)
					g_pTcpIp->SendNetMessage(m_commOut);
				if(!m_bMaster && m_bReady)
					return 1;
				else
				{
					g_pRoh->m_nTeamMemGrade = TEAM_BATTLE_NONE;
					return 1;
				}
			}

			if( m_WaitList )	m_WaitList->GetMessage(hWnd, msg, wParam, lParam );
			if( m_pBtnOut )		m_pBtnOut->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnStart )	m_pBtnStart->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnShop )	m_pBtnShop->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtnExit )	m_pBtnExit->m_iBtnFlg = BTN_NORMAL;
		}
		return 1;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if( m_pBtnOut )	m_pBtnOut->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtnStart && m_bMaster)
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

BOOL CMatSurvivalUI::IsInside(int x, int y)
{
	if( m_pBack )
	{
		if (x >= (int)(m_pBack->Xpos)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
				y >= (int)(m_pBack->Ypos) && y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
			return TRUE;
	}
	return FALSE;
}

void CMatSurvivalUI::TRoom(char *cmd, char *strName)
{}

void CMatSurvivalUI::AddMember(char *strName, int which_loc, BOOL bWaiting, BOOL bOwner)
{
	if( strName == NULL )
		return;

	if( m_WaitList )
	{
		switch(which_loc)
		{
		case -1: //대기
			m_WaitList->AddString(strName);
			m_WaitList->UpdateList();
			break;
		case 0://
			m_WaitList->AddString(strName);
			m_WaitList->UpdateList();
			break;
		case 1://
			break;
		}
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

void CMatSurvivalUI::RoomUpdate(int what_cmd, char *strName)
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
		break;
	case 6://B팀으로 이동
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

void CMatSurvivalUI::DeleteCha(char *strName)
{
	if( !m_WaitList || strName == NULL )
		return;

	for(int i=0; i<m_WaitList->TotalLine; i++)
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

void CMatSurvivalUI::FindCha(char *strName, BOOL bReady, BOOL bMater)
{
	if( !m_WaitList || strName == NULL )
		return;

	for(int i=0; i<m_WaitList->TotalLine; i++)
	{
		if(!strcmp(m_WaitList->LineText[i],strName))
		{
			if(bMater)
			{
				m_WaitList->SetColorString (i, 3);
				return;
			}
			if(bReady)
			{
				m_WaitList->SetColorString (i, 0);
				return;
			}
			else
			{
				m_WaitList->SetColorString (i, 4);
				return;
			}
		}
	}
}
