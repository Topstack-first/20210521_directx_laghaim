#include "stdafx.h"


#include "main.h"
#include "basicbutton.h"
#include "spinbox.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "MatMakeRoom.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "UIMgr.h"





extern HINSTANCE  g_hDllInst;
extern BOOL g_bMatrixMatch;

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(50, 50, 50 );
static DWORD gray_color = RGB(155, 155, 155);

#define MAT_LIST_TITLE_X	100
#define MAT_LIST_TITLE_Y	75
#define MATMAKEROOM_TITLE_WIDTH 319
#define MATMAKEROOM_PASSWORD_WIDTH 64
#define MATMAKEROOM_PAY_WIDTH 129
#define MATMAKEROOM_MINLEVEL_WIDTH 64
#define MATMAKEROOM_MAXLEVEL_WIDTH 64

typedef enum MATMAKEROOM_TEXTBOX_TYPE
{
	TITLE = 0,
	PASSWORD,
	PAY,
	MINLEVEL,
	MAXLEVEL
} MATMAKEROOM_TEXTBOX_TYPE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatMakeRoom::CMatMakeRoom()
	: m_nRoomType(0)
	, m_nMaxCount(0)
	, m_nPay(0)
{
	memset(m_strTitle, 0, sizeof(m_strTitle));
	memset(m_strPassWord, 0, sizeof(m_strPassWord));
	memset(m_strPay, 0, sizeof(m_strPay));

	if (!g_pDisplay)
		return;

	int i = 0;

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
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/matlist/back_make_room.bmp");
	if( !m_pBack )
		return;
	m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
	m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
	int X = m_iMainX = m_pBack->Xpos;
	int Y = m_iMainY =m_pBack->Ypos;

	//------------------------------------------------------------
	// Spin Box
	m_NumBox.Init(X+105, Y+97, 131, 22, 12, RGB(255, 255, 255));
	m_NumBox.SetSize(8);
	m_NumBox.SetString(0, "2");
	m_NumBox.SetString(1, "4");
	m_NumBox.SetString(2, "6");
	m_NumBox.SetString(3, "8");
	m_NumBox.SetString(4, "10");
	m_NumBox.SetString(5, "12");
	m_NumBox.SetString(6, "14");
	m_NumBox.SetString(7, "16");
	m_NumBox.LoadRes("matlist/rspinbtn", "matlist/lspinbtn");

	if(g_bMatrixMatch)
	{
		m_NumBox2.Init(X+105, Y+97, 131, 22, 12, RGB(255, 255, 255));
		m_NumBox2.SetSize(4);
		m_NumBox2.SetString(0, "2");
		m_NumBox2.SetString(1, "4");
		m_NumBox2.SetString(2, "6");
		m_NumBox2.SetString(3, "8");
		m_NumBox2.LoadRes("matlist/rspinbtn", "matlist/lspinbtn");
	}

	m_TypeBox.Init(X+105, Y+72, 131, 22, 12, RGB(255, 255, 255));
	m_TypeBox.SetSize(5);
	m_TypeBox.SetString(0, "팀배틀 전투");
	m_TypeBox.SetString(1, "서바이벌 전투");
	m_TypeBox.SetString(2, "몬스터 서바이벌");
	m_TypeBox.SetString(3, "일반대화방");
	m_TypeBox.SetString(4, "데쓰매치");
	m_TypeBox.LoadRes( "matlist/rspinbtn", "matlist/lspinbtn");

	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_Make = NULL;
	m_pBtn_Close = NULL;

	m_pBtn_Make = new CBasicButton();
	m_pBtn_Close = new CBasicButton();

	if( m_pBtn_Make )	m_pBtn_Make->SetFileName("matlist/make_room");
	if( m_pBtn_Close )	m_pBtn_Close->SetFileName("matlist/close");

	if( m_pBtn_Make )	m_pBtn_Make->LoadRes();
	if( m_pBtn_Close )	m_pBtn_Close->LoadRes();

	if( m_pBtn_Make )	m_pBtn_Make->SetPosition(X+71, Y+237);
	if( m_pBtn_Close )	m_pBtn_Close->SetPosition(X+235, Y+237);

	m_bLevelNoLimit = false;
	m_LimitBtn[0] = new CBasicButton();
	m_LimitBtn[1] = new CBasicButton();

	if( m_LimitBtn[0] )	m_LimitBtn[0]->SetFileName("matlist/checkinlev");
	if( m_LimitBtn[1] )	m_LimitBtn[1]->SetFileName("matlist/checkoutlev");

	if( m_LimitBtn[0] )	m_LimitBtn[0]->LoadRes();
	if( m_LimitBtn[1] )	m_LimitBtn[1]->LoadRes();

	if( m_LimitBtn[0] )	m_LimitBtn[0]->SetPosition(X+229, Y+167);
	if( m_LimitBtn[1] )	m_LimitBtn[1]->SetPosition(X+229, Y+167);

	//------------------------------------------------------------
	//	TEXTIN
	for(i = 0; i < MAX_MATMAKEROOM_TEXTBOX; i++)
	{
		m_pTextIn[i] = NULL;
		m_pTextIn[i] = new CTextBox();
		if( m_pTextIn[i] )
		{
			m_pTextIn[i]->Init(0, 0, 40, back_color, RGB(255, 255, 255 ));
			m_pTextIn[i]->SetfontSize(10);
			m_pTextIn[i]->SetEnable(false);
		}
	}

	// TITLE, PASSWORD,	PAY, MINLEVEL, MAXLEVEL
	if( m_pTextIn[TITLE] )		m_pTextIn[TITLE]->SetStringLength(30);
	if( m_pTextIn[PASSWORD] )	m_pTextIn[PASSWORD]->SetStringLength(4);
	if( m_pTextIn[PAY] )		m_pTextIn[PAY]->SetStringLength(6);
	if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetStringLength(3);
	if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->SetStringLength(3);

	if( m_pTextIn[TITLE] )		m_pTextIn[TITLE]->SetText("매트릭스_ROOM");
	if( m_pTextIn[PASSWORD] )	m_pTextIn[PASSWORD]->SetText('\0');
	if( m_pTextIn[PAY] )		m_pTextIn[PAY]->SetText("0");

	m_nMinLevel = g_pRoh->m_Level - 1;
	if( m_nMinLevel < 1 ) m_nMinLevel = 1;
	m_nMinLevel = ( m_nMinLevel / 30 ) * 30 + 1;
	m_nMaxLevel = m_nMinLevel + 29;
	if(g_bMatrixMatch && m_nMaxLevel < 90) m_nMaxLevel = 90;

	char strLevel[5];
	sprintf( strLevel, "%d", m_nMaxLevel );
	m_pTextIn[MAXLEVEL]->SetText( strLevel );

	if (!g_bMatrixMatch)  	// 게임대회 서버가 아니면 ( 1~30, 31~60, 61~90 ... )
	{
		sprintf( strLevel, "%d", m_nMinLevel );
		if( m_pTextIn[MINLEVEL] )
			m_pTextIn[MINLEVEL]->SetText( strLevel );
	}
	else  				// 게임대회 서버이면 ( 1~90, 1~120, 1~150 ... )
	{
		if( m_pTextIn[MINLEVEL] )
			m_pTextIn[MINLEVEL]->SetText("1");
	}

	if( m_pTextIn[TITLE] )		m_pTextIn[TITLE]->SetPos(((MATMAKEROOM_TITLE_WIDTH - m_pTextIn[TITLE]->m_Width)/2) + X+65, Y+51);
	if( m_pTextIn[PASSWORD] )	m_pTextIn[PASSWORD]->SetPos(((MATMAKEROOM_PASSWORD_WIDTH - m_pTextIn[PASSWORD]->m_Width)/2) + X+137, Y+200);
	if( m_pTextIn[PAY] )		m_pTextIn[PAY]->SetPos(((MATMAKEROOM_PAY_WIDTH - m_pTextIn[PAY]->m_Width)/2) + X+113, Y+125);
	if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetPos(((MATMAKEROOM_MINLEVEL_WIDTH - m_pTextIn[MINLEVEL]->m_Width)/2) + X+137, Y+151);
	if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->SetPos(((MATMAKEROOM_MAXLEVEL_WIDTH - m_pTextIn[MAXLEVEL]->m_Width)/2) + X+137, Y+175);

	m_pMsgPopUp = NULL;
	m_bActive = TRUE;
}

CMatMakeRoom::~CMatMakeRoom()
{
	SAFE_DELETE(m_pBack);

	m_NumBox.DeleteRes();
	m_NumBox2.DeleteRes();
	m_TypeBox.DeleteRes();

	SAFE_DELETE(m_pBtn_Make);
	SAFE_DELETE(m_pBtn_Close);
	SAFE_DELETE(m_LimitBtn[0]);
	SAFE_DELETE(m_LimitBtn[1]);

	for(int i = 0; i < MAX_MATMAKEROOM_TEXTBOX; i++)
		SAFE_DELETE(m_pTextIn[i]);

	SAFE_DELETE(m_pMsgPopUp);
}

void CMatMakeRoom::Draw()
{
	if (!m_bActive || !g_pDisplay || !m_pBack)
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);


	m_TypeBox.Draw();

	if(m_TypeBox.m_CurSel == 0 && g_bMatrixMatch)
		m_NumBox2.Draw();
	else
		m_NumBox.Draw();

	if( m_pBtn_Make )	m_pBtn_Make->Draw();
	if( m_pBtn_Close )	m_pBtn_Close->Draw();

	if(m_TypeBox.m_CurSel != 3)
		m_LimitBtn[m_bLevelNoLimit]->Draw();

	for(int i = 0; i < MAX_MATMAKEROOM_TEXTBOX; i++)
	{
		if( m_pTextIn[i] )
			m_pTextIn[i]->Draw();
	}
}

LRESULT CMatMakeRoom::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = 0, y = 0, i = 0;

	if (!m_bActive || !g_pDisplay || !m_pBack)
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
			if( m_pBtn_Make )	m_pBtn_Make->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			m_NumBox.MsgProc(hWnd, msg, wParam, lParam);
			if(g_bMatrixMatch)
				m_NumBox2.MsgProc(hWnd, msg, wParam, lParam);
			m_TypeBox.MsgProc(hWnd, msg, wParam, lParam);
			m_LimitBtn[m_bLevelNoLimit]->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pTextIn[TITLE] )		m_pTextIn[TITLE]->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pTextIn[PAY] )		m_pTextIn[PAY]->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pTextIn[PASSWORD] )	m_pTextIn[PASSWORD]->MsgProc(hWnd, msg, wParam, lParam);
			return 4;
		}
		else
			return 2;

		break;

	case WM_LBUTTONUP:
		if(m_bActive)
		{
			m_NumBox.MsgProc(hWnd, msg, wParam, lParam);
			if(g_bMatrixMatch)
				m_NumBox2.MsgProc(hWnd, msg, wParam, lParam) ;
			if (m_TypeBox.MsgProc(hWnd, msg, wParam, lParam) >= 0)
			{
				if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->ClearText();
				if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->ClearText();
				if(m_TypeBox.m_CurSel != 3 && !m_bLevelNoLimit)
				{
					char strLevel[5];
					sprintf( strLevel, "%d", m_nMaxLevel );
					m_pTextIn[MAXLEVEL]->SetText( strLevel );

					if (!g_bMatrixMatch)
					{
						sprintf( strLevel, "%d", m_nMinLevel );
						if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetText( strLevel );
					}
					else
					{
						if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetText( "1" );
					}
				}
				else
				{
					if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetText( "1" );
					if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->SetText( "300" );
				}

			}

			if( m_pBtn_Make )	m_pBtn_Make->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			if( m_LimitBtn[m_bLevelNoLimit] )	m_LimitBtn[m_bLevelNoLimit]->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtn_Make && m_pBtn_Make->m_iBtnFlg == BTN_ACTION)
			{
				SendMakeRoom();
				return 2;
			}
			else if(m_pBtn_Close && m_pBtn_Close->m_iBtnFlg == BTN_ACTION)
			{
				return 2;
			}
			else if(m_TypeBox.m_CurSel != 3 && m_LimitBtn[m_bLevelNoLimit]->GetState() == BTN_ACTION)
			{
				if( m_LimitBtn[m_bLevelNoLimit] )	m_LimitBtn[m_bLevelNoLimit]->m_iBtnFlg = BTN_NORMAL;

				if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->ClearText();
				if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->ClearText();
				if(m_bLevelNoLimit ==  FALSE)
				{
					m_bLevelNoLimit = TRUE;
					if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetText( "1" );
					if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->SetText( "300" );
				}
				else
				{
					m_bLevelNoLimit = FALSE;

					char strLevel[5];
					sprintf( strLevel, "%d", m_nMaxLevel );
					if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->SetText( strLevel );

					if (!g_bMatrixMatch)
					{
						sprintf( strLevel, "%d", m_nMinLevel );
						if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetText( strLevel );
					}
					else
					{
						if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->SetText( "1" );
					}
				}
			}

			if( m_pBtn_Make )	m_pBtn_Make->m_iBtnFlg = BTN_NORMAL;
			if( m_pBtn_Close )	m_pBtn_Close->m_iBtnFlg = BTN_NORMAL;

		}
		return 1;
	case WM_MOUSEMOVE:
		if(IsInside(x, y))
		{
			if( m_pBtn_Make )	m_pBtn_Make->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtn_Close )	m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		break;

	case WM_KEYDOWN:
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
		{
			for(i = 0; i < MAX_MATMAKEROOM_TEXTBOX; i++)
			{
				if(m_pTextIn[i] && m_pTextIn[i]->GetEnable())
				{
					m_pTextIn[i]->SetEnable(false);
					break;
				}
			}

			return 1;
		}
		break;


	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if( m_pTextIn[TITLE] )	m_pTextIn[TITLE]->MsgProc(hWnd, msg, wParam, lParam);
		if (!g_bMatrixMatch)
		{
			if( m_pTextIn[PAY] )	m_pTextIn[PAY]->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pTextIn[PASSWORD] )	m_pTextIn[PASSWORD]->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pTextIn[MINLEVEL] )	m_pTextIn[MINLEVEL]->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pTextIn[MAXLEVEL] )	m_pTextIn[MAXLEVEL]->MsgProc(hWnd, msg, wParam, lParam);
		}
		return 1;
		break;
	}
	return 0;
}


BOOL CMatMakeRoom::IsInside(int x, int y)
{
	if( m_pBack )
	{
		if (x >= (int)(m_pBack->Xpos)  && x < (int)(m_pBack->Xpos + m_pBack->GetWidth()) &&
				y >= (int)(m_pBack->Ypos) && y < (int)(m_pBack->Ypos + m_pBack->GetHeight()) )
			return TRUE;
	}
	return FALSE;
}

BOOL CMatMakeRoom::SendMakeRoom()
{
	char Buf[255] = "";
	int nNum = 0, nType = 0;

	// 경기 타입.
	nType = m_TypeBox.m_CurSel+1;
	if(nType == 1 && g_bMatrixMatch)//하두리 게임 대회 에서 팀배틀인 경우
		nNum = (m_NumBox2.m_CurSel+1)*2;
	else
		nNum = (m_NumBox.m_CurSel+1)*2;

	if (m_pTextIn[PASSWORD] && (*m_pTextIn[PASSWORD]->GetString() != '\0'))
	{
		if(nType == 3)		// 일반 대화방은 레벨 제한 없음
		{
			sprintf(Buf, "make_room '%s' %d %d %s 1 300 %s\n", m_pTextIn[TITLE]->GetString(),
					nType,
					nNum,
					m_pTextIn[PAY]->GetString(),
					m_pTextIn[PASSWORD]->GetString());
		}
		else
		{
			sprintf(Buf, "make_room '%s' %d %d %s %s %s %s\n", m_pTextIn[TITLE]->GetString(),
					nType,
					nNum,
					m_pTextIn[MINLEVEL]->GetString(),
					m_pTextIn[MAXLEVEL]->GetString(),
					m_pTextIn[PAY]->GetString(),
					m_pTextIn[PASSWORD]->GetString());
		}
	}
	else
	{
		if(nType == 3)
		{
			sprintf(Buf, "make_room '%s' %d %d 1 300 %s \n", m_pTextIn[TITLE]->GetString(),
					nType,
					nNum,
					m_pTextIn[PAY]->GetString());
		}
		else
		{
			sprintf(Buf, "make_room '%s' %d %d %s %s %s\n", m_pTextIn[TITLE]->GetString(),
					nType,
					nNum,
					m_pTextIn[MINLEVEL]->GetString(),
					m_pTextIn[MAXLEVEL]->GetString(),
					m_pTextIn[PAY]->GetString());
		}
	}

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(Buf);

	return TRUE;
}
