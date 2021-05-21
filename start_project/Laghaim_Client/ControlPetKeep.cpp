#include "stdafx.h"
#define D3D_OVERLOADS



#include <math.h>
#include <d3d.h>
#include "d3dutil.h"
#include "d3dutil.h"
#include "main.h"
#include "IndexedTexture.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "nkmob.h"
#include "nkpet.h"
#include "nkmypet.h"
#include "controlPetKeep.h"
#include "g_stringmanager.h"
#include "Mouse.h"

#define PET_PUTMODE 0
#define PET_PICKUPMODE 1







CControlPetKeep::CControlPetKeep()
{
	m_pBack			= NULL;
	m_bActive		= TRUE;
	m_iMainX		= 0;
	m_iMainY		= 0;
	m_ScreenWidth	= 0;
	m_ScreenHeight	= 0;
	Pet_Mode		= PET_PUTMODE;
	m_PetKeepPay	= 0;
	m_PetKeepTime	= 0;
	m_dwLastOpenTime = timeGetTime();
	m_IsOpenWindow	= FALSE;
}

CControlPetKeep::~CControlPetKeep()
{
	SAFE_DELETE(m_pBack);
	m_CloseBtn.DeleteRes();
	m_PutBtn.DeleteRes();
	m_PickUpBtn.DeleteRes();
	m_pPetName.DeleteRes();
	m_pPetLevel.DeleteRes();
	m_pPetHealth.DeleteRes();
	m_pPetLoyalty.DeleteRes();
	m_pPetSatiety.DeleteRes();
	m_pPetDelayTime.DeleteRes();
	m_pCur_money.DeleteRes();
	m_pPay_money.DeleteRes();
}
void CControlPetKeep::LoadRes()
{
	if(!g_pDisplay)
		return;

	RECT rc;
	HWND hWnd = g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);

	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;

	int X = 0, Y = 0;
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/petkeep/pet_keep.BMP");
	if( m_pBack )
	{
		m_pBack->SetColorKey(RGB(0, 0, 0));
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY = m_pBack->Ypos;
	}

	m_CloseBtn.SetFileName("common/btn_close_01");
	m_PutBtn.SetFileName("petkeep/btn_leave");
	m_PickUpBtn.SetFileName("petkeep/btn_take");
	m_CloseBtn.LoadRes();
	m_PutBtn.LoadRes();
	m_PickUpBtn.LoadRes();

	m_CloseBtn.PosX		= X+445;
	m_CloseBtn.PosY		= Y+5;
	m_PutBtn.PosX		= X+294+24;
	m_PutBtn.PosY		= Y+170;
	m_PickUpBtn.PosX	= X+294+24;
	m_PickUpBtn.PosY	= Y+170;

	m_pPetName.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pPetLevel.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pPetHealth.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pPetLoyalty.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pPetSatiety.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pPetDelayTime.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pCur_money.Init(12, _FCOLOR_YELLOW,0, 0);
	m_pPay_money.Init(12, _FCOLOR_YELLOW,0, 0);
}
void CControlPetKeep::SetKeepInfo(char *Name, int KeepMode, int level, int health, int loyalty, int hunger, int pay, int time)
{
	char tmp[255] = {0,};

	m_pPetName.SetString(Name);
	m_pPetLevel.SetString(level, true);
	m_pPetHealth.SetString(health,true);
	m_pPetLoyalty.SetString(loyalty, true);
	m_pPetSatiety.SetString(hunger,true);
	int min = 0, sec = 0;
	min = time / 60;
	sec = time - (min*60);
	//-- IDS_PETKEEP_TIME : %d분 %d초
	sprintf(tmp, G_STRING(IDS_PETKEEP_TIME), min, sec);
	m_pPetDelayTime.SetString(tmp);
	m_pCur_money.SetString(g_pRoh->m_Money , true);
	m_pPay_money.SetString(pay, true);

	int bgx = m_iMainX;
	int bgy = m_iMainY;

	m_pPetName.SetPos(bgx+214, bgy+63);
	m_pPetLevel.SetPos(bgx+214, bgy+92);
	m_pPetHealth.SetPos(bgx+214, bgy+136);
	m_pPetLoyalty.SetPos(bgx+214, bgy+167);
	m_pPetSatiety.SetPos(bgx+214, bgy+198);

	m_pPetDelayTime.SetPos(bgx+351, bgy+63);
	m_pCur_money.SetPos(bgx+351, bgy+136);
	m_pPay_money.SetPos(bgx+351, bgy+167);

	m_PetKeepPay = pay;
	m_PetKeepTime = time;

	if( KeepMode == 1 )
		Pet_Mode = PET_PICKUPMODE;
	else if( KeepMode == 0 )
		Pet_Mode = PET_PUTMODE;

	return;
}
BOOL CControlPetKeep::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y );
}
void CControlPetKeep::SendChMsg(int PetMode)
{
	char tmp[100];

	if( PetMode == PET_PUTMODE )
		sprintf(tmp, "p_keep in\n");
	else if( PetMode == PET_PICKUPMODE )
		sprintf(tmp, "p_keep out\n");
	else
		return;

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);
}
void CControlPetKeep::Draw()
{
	if(!m_bActive || !m_pBack)
		return;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	m_CloseBtn.Draw();

	if( Pet_Mode == PET_PUTMODE )
		m_PutBtn.Draw(m_pBack->Xpos+349, m_pBack->Ypos+199);
	else if( Pet_Mode == PET_PICKUPMODE )
		m_PickUpBtn.Draw(m_pBack->Xpos+349, m_pBack->Ypos+199);

	m_pPetName.Draw(m_pBack->Xpos+214,m_pBack->Ypos+68,79);
	m_pPetLevel.Draw(m_pBack->Xpos+214, m_pBack->Ypos+97,79);
	m_pPetHealth.Draw(m_pBack->Xpos+214, m_pBack->Ypos+141,79);
	m_pPetLoyalty.Draw(m_pBack->Xpos+214, m_pBack->Ypos+172,79);
	m_pPetSatiety.Draw(m_pBack->Xpos+214, m_pBack->Ypos+203,79);
	m_pPetDelayTime.Draw(m_pBack->Xpos+351, m_pBack->Ypos+68,79);
	m_pCur_money.Draw(m_pBack->Xpos+351, m_pBack->Ypos+141,79);
	m_pPay_money.Draw(m_pBack->Xpos+351, m_pBack->Ypos+172,79);

	if(	m_IsOpenWindow == FALSE )
	{
		if ( (timeGetTime() - m_dwLastOpenTime) > 300 )
		{
			m_IsOpenWindow = TRUE;
			m_dwLastOpenTime = timeGetTime();
		}
	}
}

LRESULT CControlPetKeep::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	if(!m_bActive)
		return -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_RBUTTONDOWN:
		if(IsInside(x, y))
			return 1;
		else
			return 0;

		break;

	case WM_LBUTTONDOWN:

		if(IsInside(x, y))
		{
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( Pet_Mode == PET_PUTMODE )
				m_PutBtn.MsgProc(hWnd, msg, wParam, lParam);
			else if( Pet_Mode == PET_PICKUPMODE )
				m_PickUpBtn.MsgProc(hWnd, msg, wParam, lParam);
			return 1;
			break;
		}
		else
			return 2;

		break;

	case WM_LBUTTONUP:

		if(IsInside(x, y))
		{
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( Pet_Mode == PET_PUTMODE )
				m_PutBtn.MsgProc(hWnd, msg, wParam, lParam);
			else if( Pet_Mode == PET_PICKUPMODE )
				m_PickUpBtn.MsgProc(hWnd, msg, wParam, lParam);

			if(m_CloseBtn.m_iBtnFlg == BTN_ACTION)
				return 2;
			else if(m_PutBtn.m_iBtnFlg == BTN_ACTION && Pet_Mode == PET_PUTMODE)
			{
				SendChMsg(PET_PUTMODE);
				return 2;
			}
			else if(m_PickUpBtn.m_iBtnFlg == BTN_ACTION && Pet_Mode == PET_PICKUPMODE)
			{
				if(g_pRoh->m_Money < m_PetKeepPay)
				{
					//-- IDS_PETKEEP_NOTENOUGH_MONEY : 보유금액이 부족합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETKEEP_NOTENOUGH_MONEY), TYPE_NOR_OK, 1 );
					return 1;
				}
				else
				{
					SendChMsg(PET_PICKUPMODE);
					return 2;
				}
			}

			m_CloseBtn.m_iBtnFlg = BTN_NORMAL;

			if( Pet_Mode == PET_PUTMODE )
				m_PutBtn.m_iBtnFlg = BTN_NORMAL;
			else if( Pet_Mode == PET_PICKUPMODE )
				m_PickUpBtn.m_iBtnFlg = BTN_NORMAL;

			break;
		}
		else
		{
			if( m_IsOpenWindow )
				return 2;
			else
				return 1;
		}
		break;

	case WM_MOUSEMOVE:

		if(IsInside(x, y))
		{
			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( Pet_Mode == PET_PUTMODE )
				m_PutBtn.MsgProc(hWnd, msg, wParam, lParam);
			else if( Pet_Mode == PET_PICKUPMODE )
				m_PickUpBtn.MsgProc(hWnd, msg, wParam, lParam);
			return 1;
		}
		else
			return 0;

		break;
	}

	return 0;
}

