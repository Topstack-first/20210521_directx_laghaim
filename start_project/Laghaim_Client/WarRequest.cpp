#include "stdafx.h"
// WarRequest.cpp: implementation of the CWarRequest class.
//
//////////////////////////////////////////////////////////////////////


#include "main.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "headers.h"
#include "nkcharacter.h"
#include "tcpipcon.h"
#include "WarRequest.h"
#include "Country.h"
#include "AlphaPopUp.h"
#include "g_stringmanager.h"

extern DWORD				g_dwClientCountry;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarRequest::CWarRequest()
	: m_iType(FALSE)
	, m_iTime(FALSE)
{
	m_bActive = FALSE;
	m_bInfoTwoLine = FALSE;

	if(!g_pDisplay)
		return;

	// open the door
	SendMsgOpen();

	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenCx = rc.right;
	m_ScreenCy = rc.bottom;

	m_pBack			= NULL;
	m_pBtn_Request	= NULL;
	m_pBtn_Time		= NULL;
	m_pBtn_Close	= NULL;
	m_pBtn_ArrowUp  = NULL;
	m_pBtn_ArrowDown  = NULL;
	m_pText_Day		= NULL; 		// 일자
	m_pText_Time	= NULL;     	// 시간
	m_pText_Info	= NULL;			// 정보
	m_pText_Info2   = NULL;
	//-------------------------------------------------------------------
	// BACKGROUND
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/war_request/War_Request.bmp");
	m_pBack->SetColorKey(TRANS_COLOR);
	m_iMainX = m_pBack->Xpos = m_ScreenCx/2  - m_pBack->GetWidth()/2;
	m_iMainY = m_pBack->Ypos = m_ScreenCy/2 - m_pBack->GetHeight()/2;


	//-------------------------------------------------------------------
	// BUTTON
	m_pBtn_Request	= new CBasicButton();
	m_pBtn_Time		= new CBasicButton();
	m_pBtn_Close	= new CBasicButton();
	m_pBtn_ArrowUp = new CBasicButton();
	m_pBtn_ArrowDown = new CBasicButton();

	m_pBtn_Request->SetFileName("war_request/btn_request");
	m_pBtn_Time->SetFileName("war_request/btn_time");
	m_pBtn_Close->SetFileName("common/btn_close_01");
	m_pBtn_ArrowUp->SetFileName("common/arrowup");
	m_pBtn_ArrowDown->SetFileName("common/arrowdown");

	m_pBtn_Request->LoadRes();
	m_pBtn_Time->LoadRes();
	m_pBtn_Close->LoadRes();
	m_pBtn_ArrowUp->LoadRes();
	m_pBtn_ArrowDown->LoadRes();


	m_pBtn_Request->SetPosition(m_pBack->Xpos+112,m_pBack->Ypos+202);
	m_pBtn_Time->SetPosition(m_pBack->Xpos+112,m_pBack->Ypos+202);
	m_pBtn_Close->SetPosition(m_pBack->Xpos+296,m_pBack->Ypos+5);
	m_pBtn_ArrowUp->SetPosition(m_pBack->Xpos+258,m_pBack->Ypos+59);
	m_pBtn_ArrowDown->SetPosition(m_pBack->Xpos+258,m_pBack->Ypos+73);

	//-------------------------------------------------------------------
	// TEXTOUTBOX
	m_pText_Day  = new CTextOutBox();
	m_pText_Time = new CTextOutBox();
	m_pText_Info = new CTextOutBox();
	m_pText_Info2 = new CTextOutBox();

	m_pText_Day->Init(12, RGB(255, 210, 0),0, 0);
	m_pText_Time->Init(12, RGB(255, 210, 0),0, 0);
	m_pText_Info->Init(12, RGB(255, 210, 0),0, 0);
	m_pText_Info2->Init(12, RGB(255, 210, 0),0, 0);
}

CWarRequest::~CWarRequest()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBtn_Request);
	SAFE_DELETE(m_pBtn_Time);
	SAFE_DELETE(m_pBtn_Close);
	SAFE_DELETE(m_pBtn_ArrowUp);
	SAFE_DELETE(m_pBtn_ArrowDown);
	SAFE_DELETE(m_pText_Day);
	SAFE_DELETE(m_pText_Time);
	SAFE_DELETE(m_pText_Info);
	if(m_bInfoTwoLine)
		SAFE_DELETE(m_pText_Info2);
}



//
void CWarRequest::GetMsg(char* date, int time, int type)
{
	char tmp[300];
	char tmp2[300];

	m_iType = type;
	m_iTime = time;
	//-- IDS_WAR_NOT_DECIDE : 미정
	int res = strcmp(date, G_STRING(IDS_WAR_NOT_DECIDE));//"미정"

	if(type ==0)
	{
		// 날자true 시간false(기간여부)
		//중국, 일본, 대만에서는 200만 라임으로 공성전 신청 금액 수정
		//-- IDS_WAR_REQUEST_LIME : 공성전 신청을 하기 위해서는 1000만라임이
		strcpy(tmp, G_STRING(IDS_LHSTRING1889));
		//-- IDS_WAR_NEED_REQUEST : 필요합니다. 신청 하시겠습니까?
		strcpy(tmp2, G_STRING(IDS_WAR_NEED_REQUEST));
		m_pBtn_Request->m_iBtnFlg = BTN_NORMAL;
	}
	else if(type ==1)
	{
		//-- IDS_WAR_ALREADY_REQUEST : 이미 공성전이 신청되었습니다.
		strcpy(tmp, G_STRING(IDS_WAR_ALREADY_REQUEST));
		//-- IDS_WAR_NOT_TIME_YET : 시간이 아직 정해지지 않았습니다.
		strcpy(tmp2, G_STRING(IDS_WAR_NOT_TIME_YET));
		m_pBtn_Request->m_iBtnFlg = BTN_DISABLE;
	}
	else if(type ==2)
	{
		//-- IDS_WAR_WHEN : 공성전 시간을 언제로 하시겠습니까?
		strcpy(tmp, G_STRING(IDS_WAR_WHEN));
		m_pBtn_Request->m_iBtnFlg = BTN_NORMAL;
	}
	else if(type ==3)
	{
		//-- IDS_WAR_DECIDE_DATE_TIME : 공성전 일자와 시간이 정해졌습니다.
		strcpy(tmp, G_STRING(IDS_WAR_DECIDE_DATE_TIME));
		m_pBtn_Request->m_iBtnFlg = BTN_DISABLE;
	}
	else if(type ==4)
	{
		//-- IDS_WAR_NOT_REQUEST_YET : 아직 공성전을 신청 하실 수 없습니다.
		strcpy(tmp, G_STRING(IDS_WAR_NOT_REQUEST_YET));
		m_pBtn_Request->m_iBtnFlg = BTN_DISABLE;
	}

	int a = time;
	if(!strcmp(date, "none"))
		m_pText_Day->SetString(" ");
	else
		m_pText_Day->SetString(date);

	m_pText_Info->SetString(tmp);
	//-- IDS_WAR_TERM : %d시 ~ %d시
	sprintf(tmp, G_STRING(IDS_WAR_TERM), a, a+1);

	m_pText_Time->SetString(tmp);

	m_pText_Day->SetPos(m_pBack->Xpos+144, m_pBack->Ypos+67);
	m_pText_Time->SetPos(m_pBack->Xpos+144, m_pBack->Ypos+102);
	m_pText_Info->SetPos(m_pBack->Xpos+29, m_pBack->Ypos+142);

	if(type == 0 || type == 1)
	{
		m_bInfoTwoLine = TRUE;
	}
	if(m_bInfoTwoLine)
	{
		m_pText_Info2->SetString(tmp2);
		m_pText_Info->SetPos(m_pBack->Xpos+29, m_pBack->Ypos+142);
		m_pText_Info2->SetPos(m_pBack->Xpos+29, m_pBack->Ypos+162);

	}

	if(type == 2)
	{
		m_pBtn_Request->m_bVisible = FALSE;
		m_pBtn_Time->m_bVisible = TRUE;
	}
	else
	{
		m_pBtn_Request->m_bVisible = TRUE;
		m_pBtn_Time->m_bVisible = FALSE;
	}

	m_bActive = TRUE;
}

// 클라  --> 서버  Message
// OPEN
void CWarRequest::SendMsgOpen()
{
	strcpy(m_MsgBuf, "bt open\n");
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 공성신청
void CWarRequest::SendMsgReq()
{
	sprintf(m_MsgBuf, "bt req\n");
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}
// 시간설정
void CWarRequest::SendMsgTime(int time)
{
	if(time <= 0)
		return;

	sprintf(m_MsgBuf, "bt time %d\n", time);
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}


void CWarRequest::Draw()
{
	if(!m_bActive)
		return ;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	m_pBtn_Time->Draw();
	m_pBtn_Request->Draw();
	m_pBtn_Close->Draw();
	m_pBtn_ArrowUp->Draw();
	m_pBtn_ArrowDown->Draw();

	m_pText_Day->Draw(112);
	m_pText_Time->Draw(112);
	m_pText_Info->Draw(253);
	if(m_bInfoTwoLine)
		m_pText_Info2->Draw(253);
}




LRESULT CWarRequest::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bActive)
		return -1;

	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_RBUTTONDOWN:
		if(IsInside(x, y))
		{
			return 1;
		}
		else
		{
			return 0;
		}
		break;
	case WM_LBUTTONDOWN:
		if(IsInside(x, y))
		{
			if(m_iType != 2)
			{
				if(m_pBtn_Request->m_iBtnFlg != BTN_DISABLE)
					m_pBtn_Request->MsgProc( hWnd, msg, wParam, lParam );
			}
			else
				m_pBtn_Time->MsgProc(hWnd, msg, wParam, lParam);

			m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 4;
		}
		break;
	case WM_LBUTTONUP:
		if(IsInside(x, y))
		{
			char tmp[80];

			m_pBtn_Request->MsgProc( hWnd, msg, wParam, lParam );
			m_pBtn_Time->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn_Request->m_iBtnFlg == BTN_ACTION)
			{
				// 공성신청제한?
				SendMsgReq();
				m_pBtn_Request->m_iBtnFlg	 = BTN_NORMAL;
				return 1;
			}
			else if(m_pBtn_Time->m_iBtnFlg == BTN_ACTION)
			{
				SendMsgTime(m_iTime);
				m_pBtn_Time->m_iBtnFlg		 = BTN_NORMAL;
				return 1;
			}
			else if(m_pBtn_ArrowUp->m_iBtnFlg == BTN_ACTION)
			{
				if (g_dwClientCountry == CTRY_TEN || g_dwClientCountry == CTRY_TH)
				{
					if(m_iTime < 20)
					{
						int a = ++m_iTime;
						//-- IDS_WAR_TERM : %d시 ~ %d시
						sprintf(tmp, G_STRING(IDS_WAR_TERM), a, a+1);
						m_pText_Time->SetString(tmp);
						m_pBtn_ArrowUp->m_iBtnFlg	 = BTN_NORMAL;
					}
				}
				else
				{
					if(m_iTime < 22)
					{
						int a = ++m_iTime;
						//-- IDS_WAR_TERM : %d시 ~ %d시
						sprintf(tmp, G_STRING(IDS_WAR_TERM), a, a+1);
						m_pText_Time->SetString(tmp);
						m_pBtn_ArrowUp->m_iBtnFlg	 = BTN_NORMAL;
					}
				}
				return 1;
			}
			else if(m_pBtn_ArrowDown->m_iBtnFlg == BTN_ACTION)
			{
				if (g_dwClientCountry == CTRY_TEN || g_dwClientCountry == CTRY_TH)
				{
					if(m_iTime > 18)
					{
						int a = --m_iTime;
						//-- IDS_WAR_TERM : %d시 ~ %d시
						sprintf(tmp, G_STRING(IDS_WAR_TERM), a, a+1);
						m_pText_Time->SetString(tmp);
						m_pBtn_ArrowDown->m_iBtnFlg	 = BTN_NORMAL;
					}
				}
				else
				{
					if(m_iTime > 19)
					{
						int a = --m_iTime;
						//-- IDS_WAR_TERM : %d시 ~ %d시
						sprintf(tmp, G_STRING(IDS_WAR_TERM), a, a+1);
						m_pText_Time->SetString(tmp);
						m_pBtn_ArrowDown->m_iBtnFlg	 = BTN_NORMAL;
					}
				}
				return 1;
			}
			else if(m_pBtn_Close->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn_Close->m_iBtnFlg		 = BTN_NORMAL;
				return 2;
			}
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if(m_iType != 2)
			{
				if(m_pBtn_Request->m_iBtnFlg != BTN_DISABLE)
					m_pBtn_Request->MsgProc( hWnd, msg, wParam, lParam );
			}
			else
				m_pBtn_Time->MsgProc(hWnd, msg, wParam, lParam);

			m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;
	}

	return 0;
}


BOOL CWarRequest::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	return m_pBack->IsIn(x, y);
}


