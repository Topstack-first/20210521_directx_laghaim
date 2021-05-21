#include "stdafx.h"


#include <list>
#include "main.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "MatList.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "UIMgr.h"
#include "g_stringmanager.h"

#define MATLIST_ROOMNUM_WIDTH 28
#define MATLIST_TITLE_WIDTH 290
#define MATLIST_ROOMTYPE_WIDTH 150
#define MATLIST_CURMAXCNT_WIDTH 51
#define MATLIST_MASTER_WIDTH 139
#define MATLIST_STATE_WIDTH 52
#define MATLIST_MINMAXLEVEL_WIDTH 62
#define MATLIST_PAY_WIDTH 80







static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);

list<CMatRoomList *> g_listMatRoomList;

char g_RoomType[5][20] = { "팀배틀", "서바이벌", "몬스터 서바이벌", "일반대화방", "데쓰매치" };
char g_StateType[5][20] = { "WAIT", "WAIT", "WAIT", "PLAY", "PLAY" };
int	 g_LastTime = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatList::CMatList()
	: m_pTextOut(NULL)
{
	if (!g_pDisplay)
		return;

	m_bActive = TRUE;
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
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/matlist/back_list_room.bmp");
	if( !m_pBack )
		return;

	m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
	m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
	int X = m_iMainX = m_pBack->Xpos;
	int Y = m_iMainY =m_pBack->Ypos;

	m_pSecret = NULL;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSecret, "interface/matlist/OPEN.bmp");
	if( m_pSecret )
	{
		m_pSecret->Xpos = 0;
		m_pSecret->Ypos = 0;
	}

	m_pNumText = NULL;
	m_pTitleText = NULL;
	m_pTypeText = NULL;
	m_pCur_MaxCountText = NULL;
	m_pMasterText = NULL;
	m_pStateText = NULL;
	m_nMin_MaxLevelText = NULL;
	m_pPayText = NULL;
	m_pMsgText = NULL;

	m_pNumText = new CTextOutBox();
	m_pTitleText = new CTextOutBox();
	m_pTypeText = new CTextOutBox();
	m_pCur_MaxCountText = new CTextOutBox();
	m_pMasterText = new CTextOutBox();
	m_pStateText = new CTextOutBox();
	m_nMin_MaxLevelText = new CTextOutBox();
	m_pPayText = new CTextOutBox();
	m_pMsgText = new CTextOutBox();

	if( m_pNumText )			m_pNumText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pTitleText )			m_pTitleText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pTypeText )			m_pTypeText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pCur_MaxCountText )	m_pCur_MaxCountText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pMasterText )			m_pMasterText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pStateText )			m_pStateText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_nMin_MaxLevelText )	m_nMin_MaxLevelText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pPayText )			m_pPayText->Init(13, RGB(255, 255, 255),0, 0);
	if( m_pMsgText )			m_pMsgText->Init(13, RGB(255, 255, 255),0, 0);

	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_Enter1 = NULL;
	m_pBtn_Enter2 = NULL;
	m_pBtn_Enter3 = NULL;
	m_pBtn_Enter4 = NULL;
	m_pBtn_Make = NULL;
	m_pBtn_Prev = NULL;
	m_pBtn_Next = NULL;
	m_pBtn_Refresh = NULL;
	m_pBtn_WaitView = NULL;
	m_pBtn_AllView = NULL;
	m_pBtn_Close = NULL;

	m_pBtn_Enter1 = new CBasicButton();
	m_pBtn_Enter2 = new CBasicButton();
	m_pBtn_Enter3 = new CBasicButton();
	m_pBtn_Enter4 = new CBasicButton();
	m_pBtn_Make = new CBasicButton();
	m_pBtn_Prev = new CBasicButton();
	m_pBtn_Next = new CBasicButton();
	m_pBtn_Refresh = new CBasicButton();
	m_pBtn_WaitView = new CBasicButton();
	m_pBtn_AllView = new CBasicButton();
	m_pBtn_Close = new CBasicButton();

	if( m_pBtn_Enter1 )		m_pBtn_Enter1->SetFileName("matlist/list");
	if( m_pBtn_Enter2 )		m_pBtn_Enter2->SetFileName("matlist/list");
	if( m_pBtn_Enter3 )		m_pBtn_Enter3->SetFileName("matlist/list");
	if( m_pBtn_Enter4 )		m_pBtn_Enter4->SetFileName("matlist/list");
	if( m_pBtn_Make )		m_pBtn_Make->SetFileName("matlist/make_room");
	if( m_pBtn_Prev )		m_pBtn_Prev->SetFileName("matlist/prev_room");
	if( m_pBtn_Next )		m_pBtn_Next->SetFileName("matlist/next_room");
	if( m_pBtn_Refresh )	m_pBtn_Refresh->SetFileName("matlist/refresh");
	if( m_pBtn_WaitView )	m_pBtn_WaitView->SetFileName("matlist/waitroom");
	if( m_pBtn_AllView )	m_pBtn_AllView->SetFileName("matlist/allroom");
	if( m_pBtn_Close )		m_pBtn_Close->SetFileName("matlist/close");

	if( m_pBtn_Enter1 )		m_pBtn_Enter1->LoadRes();
	if( m_pBtn_Enter2 )		m_pBtn_Enter2->LoadRes();
	if( m_pBtn_Enter3 )		m_pBtn_Enter3->LoadRes();
	if( m_pBtn_Enter4 )		m_pBtn_Enter4->LoadRes();
	if( m_pBtn_Make )		m_pBtn_Make->LoadRes();
	if( m_pBtn_Prev )		m_pBtn_Prev->LoadRes();
	if( m_pBtn_Next )		m_pBtn_Next->LoadRes();
	if( m_pBtn_Refresh )	m_pBtn_Refresh->LoadRes();
	if( m_pBtn_WaitView )	m_pBtn_WaitView->LoadRes();
	if( m_pBtn_AllView )	m_pBtn_AllView->LoadRes();
	if( m_pBtn_Close )		m_pBtn_Close->LoadRes();

	if( m_pBtn_Enter1 )		m_pBtn_Enter1->SetPosition(X+14, Y+50);
	if( m_pBtn_Enter2 )		m_pBtn_Enter2->SetPosition(X+14, Y+100);
	if( m_pBtn_Enter3 )		m_pBtn_Enter3->SetPosition(X+14, Y+150);
	if( m_pBtn_Enter4 )		m_pBtn_Enter4->SetPosition(X+14, Y+200);
	if( m_pBtn_Make )		m_pBtn_Make->SetPosition(X+26, Y+246);
	if( m_pBtn_Prev )		m_pBtn_Prev->SetPosition(X+120, Y+246);
	if( m_pBtn_Next )		m_pBtn_Next->SetPosition(X+211, Y+246);
	if( m_pBtn_Refresh )	m_pBtn_Refresh->SetPosition(X+305, Y+246);
	if( m_pBtn_WaitView )	m_pBtn_WaitView->SetPosition(X+397, Y+246);
	if( m_pBtn_AllView )	m_pBtn_AllView->SetPosition(X+397, Y+246);
	if( m_pBtn_Close )		m_pBtn_Close->SetPosition(X+492, Y+246);

	ClearAllRoomList();

	g_LastTime = m_nCurRoomNum = m_nPageNum = 0;
	m_IsAllView = TRUE;

}

CMatList::~CMatList()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pSecret);
	SAFE_DELETE(m_pNumText);
	SAFE_DELETE(m_pTitleText);
	SAFE_DELETE(m_pTypeText);
	SAFE_DELETE(m_pCur_MaxCountText);
	SAFE_DELETE(m_pMasterText);
	SAFE_DELETE(m_pStateText);
	SAFE_DELETE(m_nMin_MaxLevelText);
	SAFE_DELETE(m_pPayText);
	SAFE_DELETE(m_pMsgText);
	SAFE_DELETE(m_pBtn_Enter1);
	SAFE_DELETE(m_pBtn_Enter2);
	SAFE_DELETE(m_pBtn_Enter3);
	SAFE_DELETE(m_pBtn_Enter4);
	SAFE_DELETE(m_pBtn_Make);
	SAFE_DELETE(m_pBtn_Prev);
	SAFE_DELETE(m_pBtn_Next);
	SAFE_DELETE(m_pBtn_Refresh);
	SAFE_DELETE(m_pBtn_WaitView);
	SAFE_DELETE(m_pBtn_AllView);
	SAFE_DELETE(m_pBtn_Close);
}

void CMatList::Draw()
{
	if (!m_bActive || !g_pDisplay || !m_pBack)
		return;

	list<CMatRoomList *>::iterator prList;
	CMatRoomList *pMatRoomList = NULL;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	if( m_pBtn_Enter1 )	m_pBtn_Enter1->Draw();
	if( m_pBtn_Enter2 )	m_pBtn_Enter2->Draw();
	if( m_pBtn_Enter3 )	m_pBtn_Enter3->Draw();
	if( m_pBtn_Enter4 )	m_pBtn_Enter4->Draw();
	if( m_pBtn_Make )	m_pBtn_Make->Draw();
	if( m_pBtn_Prev )	m_pBtn_Prev->Draw();
	if( m_pBtn_Next )	m_pBtn_Next->Draw();
	if( m_pBtn_Refresh )	m_pBtn_Refresh->Draw();
	if( m_pBtn_WaitView )
	{
		if( m_IsAllView )
			m_pBtn_WaitView->Draw();
		else
			m_pBtn_AllView->Draw();
	}
	if( m_pBtn_Close )	m_pBtn_Close->Draw();

	// 현재 페이지를 찾아라.
	int nListCount = 0;
	int nViewCount = 0;

	for (prList = g_listMatRoomList.begin(); prList != g_listMatRoomList.end(); prList++)
	{
		if( nViewCount >= 4 || *prList == NULL )
			break;

		pMatRoomList = *prList;
		nViewCount++;

		if( m_pNumText )	m_pNumText->SetString(pMatRoomList->m_nRoomNum);
		if( m_pTitleText )	m_pTitleText->SetString(pMatRoomList->m_strTitle);
		if( m_pTypeText )	m_pTypeText->SetString(g_RoomType[pMatRoomList->m_nRoomType-1]);

		char strTemp[100] = "";
		if( pMatRoomList )
		{
			sprintf(strTemp, "%d/%d", pMatRoomList->m_nCurCount, pMatRoomList->m_nMaxCount);
			if( m_pCur_MaxCountText )	m_pCur_MaxCountText->SetString(strTemp);
			if( m_pMasterText )	m_pMasterText->SetString(pMatRoomList->m_strMaster);
			if( m_pStateText )	m_pStateText->SetString(g_StateType[pMatRoomList->m_nStatus]);
			sprintf(strTemp, "%d/%d", pMatRoomList->m_nMinLevel, pMatRoomList->m_nMaxLevel);
			if( m_nMin_MaxLevelText )	m_nMin_MaxLevelText->SetString(strTemp);
			if( m_pPayText )	m_pPayText->SetString(pMatRoomList->m_nPay);
		}

		int start_posY = 48;

		if( m_pNumText )
			m_pNumText->Draw( ((MATLIST_ROOMNUM_WIDTH - m_pNumText->GetBoxWidth())/2) + m_iMainX + 20,
							  m_iMainY + nViewCount*50 + (52-start_posY));
		if( m_pTitleText )
			m_pTitleText->Draw( ((MATLIST_TITLE_WIDTH - m_pTitleText->GetBoxWidth())/2) + m_iMainX + 76,
								m_iMainY + nViewCount*50 + (52-start_posY));
		if( m_pTypeText )
			m_pTypeText->Draw( ((MATLIST_ROOMTYPE_WIDTH - m_pTypeText->GetBoxWidth())/2) + m_iMainX + 397,
							   m_iMainY + nViewCount*50 + (52-start_posY));
		if( m_pCur_MaxCountText )
			m_pCur_MaxCountText->Draw( ((MATLIST_CURMAXCNT_WIDTH - m_pCur_MaxCountText->GetBoxWidth())/2) + m_iMainX + 44,
									   m_iMainY + nViewCount*50 + (71-start_posY));
		if( m_pMasterText )
			m_pMasterText->Draw( ((MATLIST_MASTER_WIDTH - m_pMasterText->GetBoxWidth())/2) + m_iMainX + 125,
								 m_iMainY + nViewCount*50 + (71-start_posY));
		if( m_pStateText )
			m_pStateText->Draw( ((MATLIST_STATE_WIDTH - m_pStateText->GetBoxWidth())/2) + m_iMainX + 293,
								m_iMainY + nViewCount*50 + (71-start_posY));
		if( m_nMin_MaxLevelText )
			m_nMin_MaxLevelText->Draw( ((MATLIST_MINMAXLEVEL_WIDTH - m_nMin_MaxLevelText->GetBoxWidth())/2) + m_iMainX + 398,
									   m_iMainY + nViewCount*50 + (71-start_posY));
		if( m_pPayText )
			m_pPayText->Draw( ((MATLIST_PAY_WIDTH - m_pPayText->GetBoxWidth())/2) + m_iMainX + 501,
							  m_iMainY + nViewCount*50 + (71-start_posY));

		if( pMatRoomList && (*pMatRoomList->m_strPassWord != '\0') )
			g_pDisplay->Blt(m_iMainX + 548, m_iMainY + nViewCount*50 + (50-start_posY), m_pSecret);
	}
}

LRESULT CMatList::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bActive || !g_pDisplay || !m_pBack)
		return 0;

	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	int elapsetime = 0, curtime = 0;

	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		if(IsInside(x, y))
		{
			if( m_pBtn_Enter1 )	m_pBtn_Enter1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter2 )	m_pBtn_Enter2->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter3 )	m_pBtn_Enter3->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter4 )	m_pBtn_Enter4->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Make )	m_pBtn_Make->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Prev )	m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )	m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Refresh )	m_pBtn_Refresh->MsgProc(hWnd, msg, wParam, lParam);

			if( m_IsAllView )
				m_pBtn_AllView->MsgProc(hWnd, msg, wParam, lParam);
			else
				m_pBtn_WaitView->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 4;
		}
		else
			return 2;

		break;
	case WM_LBUTTONUP:
		if(m_bActive)
		{
			if( m_pBtn_Enter1 )	m_pBtn_Enter1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter2 )	m_pBtn_Enter2->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter3 )	m_pBtn_Enter3->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter4 )	m_pBtn_Enter4->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Make )	m_pBtn_Make->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Prev )	m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )	m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Refresh )	m_pBtn_Refresh->MsgProc(hWnd, msg, wParam, lParam);

			if( m_IsAllView )
				m_pBtn_AllView->MsgProc(hWnd, msg, wParam, lParam);
			else
				m_pBtn_WaitView->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtn_Enter1 && m_pBtn_Enter1->m_iBtnFlg == BTN_ACTION)
			{
				return SelectRoom(0);
			}
			else if(m_pBtn_Enter2 && m_pBtn_Enter2->m_iBtnFlg == BTN_ACTION)
			{
				return SelectRoom(1);
			}
			else if(m_pBtn_Enter3 && m_pBtn_Enter3->m_iBtnFlg == BTN_ACTION)
			{
				return SelectRoom(2);
			}
			else if(m_pBtn_Enter4 && m_pBtn_Enter4->m_iBtnFlg == BTN_ACTION)
			{
				return SelectRoom(3);
			}
			else if(m_pBtn_Make && m_pBtn_Make->m_iBtnFlg == BTN_ACTION)
			{
				pCMyApp->m_pUIMgr->MakeGameRoom();
				return 2;
			}
			else if(m_pBtn_Prev && m_pBtn_Prev->m_iBtnFlg == BTN_ACTION)
			{
				curtime = timeGetTime();
				elapsetime = curtime - g_LastTime;
				g_LastTime = curtime;

				if( elapsetime <= 500 )
					return 1;

				if (m_nPageNum > 0)
				{
					m_nPageNum--;
					ClearAllRoomList();
					GetRoomList(m_IsAllView, m_nPageNum);
				}
				return 1;
			}
			else if(m_pBtn_Next && m_pBtn_Next->m_iBtnFlg == BTN_ACTION)
			{
				curtime = timeGetTime();
				elapsetime = curtime - g_LastTime;
				g_LastTime = curtime;

				if( elapsetime <= 500 )
					return 1;

				m_nPageNum++;
				ClearAllRoomList();
				GetRoomList(m_IsAllView, m_nPageNum);
				return 1;
			}
			else if(m_pBtn_Refresh && m_pBtn_Refresh->m_iBtnFlg == BTN_ACTION)
			{
				curtime = timeGetTime();
				elapsetime = curtime - g_LastTime;
				g_LastTime = curtime;

				if( elapsetime <= 1000 )
					return 1;

				m_nPageNum = 0;
				ClearAllRoomList();
				GetRoomList(m_IsAllView, m_nPageNum);
				return 1;
			}
			else if(m_pBtn_WaitView && m_pBtn_WaitView->m_iBtnFlg == BTN_ACTION && !m_IsAllView )
			{

				m_IsAllView = !m_IsAllView;

				curtime = timeGetTime();
				elapsetime = curtime - g_LastTime;
				g_LastTime = curtime;

				if( elapsetime <= 1000 )
					return 1;

				m_nPageNum = 0;
				ClearAllRoomList();
				// 대기방 보는 메세지 날리기
				GetRoomList(m_IsAllView, m_nPageNum);
				return 1;
			}
			else if(m_pBtn_AllView && m_pBtn_AllView->m_iBtnFlg == BTN_ACTION && m_IsAllView )
			{
				m_IsAllView = !m_IsAllView;

				curtime = timeGetTime();
				elapsetime = curtime - g_LastTime;
				g_LastTime = curtime;

				if( elapsetime <= 1000 )
					return 1;

				m_nPageNum = 0;
				ClearAllRoomList();
				// 대기방 보는 메세지 날리기
				GetRoomList(m_IsAllView, m_nPageNum);
				return 1;
			}
			else if(m_pBtn_Close && m_pBtn_Close->m_iBtnFlg == BTN_ACTION)
			{
				ClearAllRoomList();
				return 2;
			}
		}
		break;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if( m_pBtn_Enter1 )	m_pBtn_Enter1->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter2 )	m_pBtn_Enter2->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter3 )	m_pBtn_Enter3->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Enter4 )	m_pBtn_Enter4->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Make )	m_pBtn_Make->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Prev )	m_pBtn_Prev->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Next )	m_pBtn_Next->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Refresh )	m_pBtn_Refresh->MsgProc(hWnd, msg, wParam, lParam);

			if( m_IsAllView )
				m_pBtn_AllView->MsgProc(hWnd, msg, wParam, lParam);
			else
				m_pBtn_WaitView->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;

	case WM_KEYDOWN:
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
			return 1;
		break;


	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		return 1;
		break;
	}
	return 0;
}



BOOL CMatList::IsInside(int x, int y)
{
	if( m_pBack )
	{
		if (x >= (int)(m_pBack->Xpos)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
				y >= (int)(m_pBack->Ypos) && y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
			return TRUE;
	}
	return FALSE;
}

void CMatList::AddRoomData(int nRoomNum, int nRoomType, char *strRoomName, int nCount, int nMaxCount, char *strMaster, int nState, int nMinLevel, int nMaxLevel, int nFee, char *strPasswd)
{
	if( strRoomName == NULL || strMaster == NULL || strPasswd == NULL )
		return;

	CMatRoomList *pMatRoomList = NULL;
	pMatRoomList = new CMatRoomList();
	if( pMatRoomList )
		pMatRoomList->AddData(nRoomNum, nRoomType, strRoomName, nCount, nMaxCount, strMaster, nState, nMinLevel, nMaxLevel, nFee, strPasswd);

	g_listMatRoomList.insert(g_listMatRoomList.end(), pMatRoomList);
}

void CMatList::ClearAllRoomList()
{
	list<CMatRoomList *>::iterator prList;
	CMatRoomList *pMatRoomList;

	for (prList = g_listMatRoomList.begin(); prList != g_listMatRoomList.end(); )
	{
		pMatRoomList = *prList;
		delete pMatRoomList;
		prList = g_listMatRoomList.erase(prList);
	}

	m_nCurRoomNum = 0;
}

void CMatList::GetRoomList(BOOL IsAllView, int PageNum)
{

	char buf[256];

	if( IsAllView )
		sprintf(buf, "list_room %d\n", PageNum);
	else
		sprintf(buf, "wait_room %d\n", PageNum);

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(buf);

	return;
}

int CMatList::SelectRoom(int cur_room)
{

	list<CMatRoomList *>::iterator prList;
	CMatRoomList *pMatRoomList = NULL;
	char buf[256] = {0,};
	int listcnt = 0;

	for (prList = g_listMatRoomList.begin(); prList != g_listMatRoomList.end(); prList++, listcnt++)
	{
		if( listcnt == cur_room )
		{
			pMatRoomList = *prList;
			break;
		}
	}

	if( pMatRoomList == NULL )
		return 1;

	if( *pMatRoomList->m_strPassWord == '\0' )
	{
		EnterRoom(cur_room);
		return 2;
	}
	else
	{
		m_nCurRoomNum = cur_room;
		//-- IDS_INPUT_PASSWORD : 비밀번호를 입력해주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INPUT_PASSWORD), TYPE_EXCHANGE, 25 );
		return 1;
	}

	return 1;
}


BOOL CMatList::EnterRoom(int cur_sel, char* password)
{
	if( password == NULL )
		return FALSE;

	list<CMatRoomList *>::iterator prList;
	CMatRoomList *pMatRoomList = NULL;
	char buf[256];
	int listcnt = 0;
	int CurRoomNum = 0;

	for (prList = g_listMatRoomList.begin(); prList != g_listMatRoomList.end(); prList++, listcnt++)
	{
		if( listcnt == cur_sel )
		{
			pMatRoomList = *prList;
			CurRoomNum = pMatRoomList->m_nRoomNum;
			break;
		}
	}

	if( password == NULL )
		sprintf(buf, "enter_room %d\n", CurRoomNum);
	else
		sprintf(buf, "enter_room %d %s\n", CurRoomNum, password);

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(buf);

	return TRUE;
}

LRESULT CMatList::MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	if(nMsg == 1) // "확인" 버튼
		EnterRoom(m_nCurRoomNum, szMoney);

	return 0L;
}

//------------------------------------------------------------------------------------
// CMatRoomList class
//------------------------------------------------------------------------------------
CMatRoomList::CMatRoomList()
{
	ZeroMemory( m_strTitle, sizeof(m_strTitle) );
	ZeroMemory( m_strMaster, sizeof(m_strMaster) );
	ZeroMemory( m_strPassWord, sizeof(m_strPassWord) );

	m_nRoomNum = 0;
	m_nRoomType = 0;
	m_nCurCount = 0;
	m_nMaxCount = 0;
	m_nStatus = 0;
	m_nMinLevel = 0;
	m_nMaxLevel = 0;
	m_nPay = 0;
}

CMatRoomList::~CMatRoomList()
{}

void CMatRoomList::AddData(int nRoomNum, int nRoomType, char *strRoomName, int nCount, int nMaxCount, char *strMaster, int nState, int nMinLevel, int nMaxLevel, int nFee, char *strPasswd)
{
	strcpy( m_strTitle, strRoomName );
	strcpy( m_strMaster, strMaster );
	strcpy( m_strPassWord, strPasswd );

	m_nRoomNum = nRoomNum;
	m_nRoomType = nRoomType;
	m_nCurCount = nCount;
	m_nMaxCount = nMaxCount;
	m_nStatus = nState;
	m_nMinLevel = nMinLevel;
	m_nMaxLevel = nMaxLevel;
	m_nPay = nFee;
}
