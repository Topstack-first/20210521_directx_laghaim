#include "stdafx.h"
#include "ControlPremiumServiceChaMove.h"

#include "Basicbutton.h"
#include "headers.h"
#include "ddutil.h"
#include "main.h"
#include "2dresmgr.h"
#include "textbox.h"
#include "WindowMgr.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "ItemProto.h"
#include "ItemRender.h"
#include "SpecialItem.h"
#include "window_resource.h"
#include "g_stringmanager.h"

// by evilkiki 2008.08.04
// header include 미포함 ^^;
#include "Item.h"



extern char					g_strFontName[];
extern DWORD				g_dwLangType;

extern HHOOK				g_hHook;//주의?

extern int					g_EffectTextr[];
extern BOOL					g_bFFullMode;
extern HFONT G_GetFont(int h, int w,const char  * const fontname);

CControlPremiumChaMove::CControlPremiumChaMove()
	: m_Race(0)
{
	memset(m_EquipSlot, 0, sizeof(m_EquipSlot));
	memset(m_strNameFrom, 0, sizeof(m_strNameFrom));
	memset(m_strNameTo, 0, sizeof(m_strNameTo));

	m_Whoau = WHOAU_NONE;
	m_Step	= 0;
	m_bAgree= FALSE;
	m_Level	= 0;

	m_pSurfAgreeBg		= NULL;
	m_pSurfWearingBg	= NULL;
	m_pSurfAgreeYes		= NULL;
	m_pSurfAgreeNo		= NULL;
	m_pSurfChaName		= NULL;
	m_pSurfChaLevel		= NULL;
	m_pSurfChaRace		= NULL;
	m_AgreeMultiText	= NULL;

	for(int i=0; i<WEARING_NUM; i++)
		m_Wearing[i] = NULL;

	m_BtnOk.SetFileName("premium/ChaMove/ok");			 // "확인" 버튼
	m_BtnCancle.SetFileName("premium/ChaMove/cancel");	 // "취소" 버튼
	m_BtnReceive.SetFileName("premium/ChaMove/receive");

	SetFont(5, 12, g_strFontName);

	m_slot = -1;
}


CControlPremiumChaMove::~CControlPremiumChaMove()
{
	DeleteRes();

	m_pSurfAgreeBg;
	m_pSurfAgreeYes;
	m_pSurfAgreeNo;
	m_pSurfWearingBg;

	m_pSurfChaName;
	m_pSurfChaLevel;
	m_pSurfChaRace;
}


void CControlPremiumChaMove::LoadRes()
{
	HWND	hWnd;
	RECT	ClientRc;

	hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfAgreeBg,	"interface/premium/chaMove/ag_bg.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfAgreeYes,	"interface/premium/chaMove/ag_box_yes.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfAgreeNo,	"interface/premium/chaMove/ag_box_no.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfWearingBg,	"interface/premium/chaMove/charac_bg.bmp");

	if( m_pSurfAgreeBg )
	{
		m_pSurfAgreeBg->Xpos = ((ClientRc.right/2) - (m_pSurfAgreeBg->GetWidth()/2));
		m_pSurfAgreeBg->Ypos = ((ClientRc.bottom/2) - (m_pSurfAgreeBg->GetHeight()/2));
		m_pSurfAgreeBg->SetColorKey(RGB(0, 0, 0));
	}

	if( m_pSurfWearingBg )
	{
		m_pSurfWearingBg->Xpos = ((ClientRc.right/2) - (m_pSurfWearingBg->GetWidth()/2));
		m_pSurfWearingBg->Ypos = ((ClientRc.bottom/2) - (m_pSurfWearingBg->GetHeight()/2));
		m_pSurfWearingBg->SetColorKey(RGB(0, 0, 0));
	}

	if( m_pSurfAgreeYes && m_pSurfAgreeNo )
	{
		m_pSurfAgreeYes->Xpos = m_pSurfAgreeNo->Xpos = m_pSurfAgreeBg->Xpos + 23;
		m_pSurfAgreeYes->Ypos = m_pSurfAgreeNo->Ypos = m_pSurfAgreeBg->Ypos + 321;
	}

	m_BtnOk.LoadRes();
	m_BtnCancle.LoadRes();
	m_BtnReceive.LoadRes();

	m_BtnOk.PosX = m_pSurfAgreeBg->Xpos + 52;
	m_BtnOk.PosY = m_pSurfAgreeBg->Ypos + 342;

	m_BtnReceive.PosX = m_pSurfWearingBg->Xpos + 53;
	m_BtnReceive.PosY = m_pSurfWearingBg->Ypos + 330;

	SetEquipArea();

	int left	= m_pSurfAgreeBg->Xpos + 30;
	int top		= m_pSurfAgreeBg->Ypos + 45;
	int right	= left + 237;
	int bottom	= top  + 172;

	int posX	= m_pSurfAgreeBg->Xpos + 14 + 245 - 20;
	int posY	= m_pSurfAgreeBg->Ypos + 44;
	int height	= 273;

	m_AgreeMultiText = new CMultiScrollText2;
	m_AgreeMultiText->LoadString("data/menu/Premiumcmagree.txt" );
	m_AgreeMultiText->SetScrollImage( "common/arrowup", "common/arrowdown", "interface/common/cute_Bar2.bmp" );
	m_AgreeMultiText->LoadRes(left,top,right,bottom);
	m_AgreeMultiText->MakeSurface();
	m_AgreeMultiText->SetScrollPos( posX, posY, height);
}


void CControlPremiumChaMove::DeleteRes()
{
	SAFE_DELETE(m_pSurfAgreeYes);
	SAFE_DELETE(m_pSurfAgreeNo);
	SAFE_DELETE(m_pSurfAgreeBg);
	SAFE_DELETE(m_pSurfWearingBg);

	m_BtnOk.DeleteRes();
	m_BtnCancle.DeleteRes();
	m_BtnReceive.DeleteRes();

	ClearChaInfo();

	if( m_AgreeMultiText )
		m_AgreeMultiText->DeleteRes();

	SAFE_DELETE(m_AgreeMultiText);
	WebClose();
}


void CControlPremiumChaMove::ClearChaInfo()
{
	for(int i=0; i<WEARING_NUM; i++)
	{
		if( m_Wearing[i] )
			SAFE_DELETE(m_Wearing[i]);
	}

	SAFE_DELETE(m_pSurfChaName);
	SAFE_DELETE(m_pSurfChaLevel);
	SAFE_DELETE(m_pSurfChaRace);
}

void CControlPremiumChaMove::SetStep(WHOAU whoau, int step)
{
	m_Whoau = whoau;
	m_Step	= step;

	if( whoau == WHOAU_NONE)
	{
		m_bAgree = FALSE;
		ClearChaInfo();
	}

	switch(step)
	{
	case 1: // 동의 창
		m_BtnCancle.PosX = m_pSurfAgreeBg->Xpos + 150;
		m_BtnCancle.PosY = m_pSurfAgreeBg->Ypos + 342;
		WebOpen();
		break;

	case 2: // 웨어링 창
		m_BtnReceive.SetPosition(m_pSurfWearingBg->Xpos + 52,m_pSurfWearingBg->Ypos + 342+90);
		m_BtnCancle.SetPosition(m_pSurfWearingBg->Xpos + 149,m_pSurfWearingBg->Ypos + 342+90);
		WebClose();
		break;
	}
}
CItem* CControlPremiumChaMove::Draw()
{
	switch(m_Whoau)
	{
	case FROM:
		switch(m_Step)
		{
		case 1:
			DrawAgree();
		}
		break;

	case TO:
		switch(m_Step)
		{
		case 1:
			DrawAgree();
			break;
		case 2:
			return DrawInven();
		}
		break;
	}
	return NULL;
}


HRESULT CControlPremiumChaMove::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(m_Step)
	{
	case 1:
		return MsgProcAgree(hWnd, msg, wParam, lParam);
	case 2:
		return MsgProcInven(hWnd, msg, wParam, lParam);
	}

	return S_OK;
}



HRESULT CControlPremiumChaMove::MsgProcAgree( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
		if( m_bAgree)
		{
			m_BtnOk.MsgProc(hWnd, msg, wParam, lParam);

			if( m_BtnOk.GetState() == BTN_ACTION)
			{
				m_BtnOk.SetState(BTN_NORMAL);

				switch( m_Whoau )
				{
				case FROM:
					g_pNk2DFrame->InsertPopup("  ",TYPE_PREMIUM_CHA_MOVE_NAME_INPUT, POPUP_PREMIUM_CHA_MOVE_NAME_INPUT);
					WebClose();
					g_pNk2DFrame->ShowPremiumChaMove(FALSE)	;
					break;
				case TO:
					SetStep(TO,2);
					WebClose();
					break;
				}
				return S_OK;
			}
		}

		m_BtnCancle.MsgProc(hWnd, msg, wParam, lParam);

		if( m_BtnCancle.GetState() == BTN_ACTION )
		{
			m_BtnCancle.SetState(BTN_NORMAL);
			OnCancleChaMove();
			WebClose();
			g_pNk2DFrame->ShowPremiumChaMove(FALSE);
			return S_OK;
		}

		break;

	case WM_LBUTTONDOWN:
		if( m_bAgree)
		{
			m_BtnOk.MsgProc(hWnd, msg, wParam, lParam);
			if( m_BtnOk.GetState() == BTN_DOWN)
				return S_OK;
		}

		m_BtnCancle.MsgProc(hWnd, msg, wParam, lParam);
		if( m_BtnCancle.GetState() == BTN_DOWN)
			return S_OK;

		if(IsInsideAgreeBtn(x, y))
			ToggleAgreeBtn();

		break;
	}

	m_AgreeMultiText->MsgProc(hWnd, msg, wParam, lParam);

	return S_OK;
}



HRESULT CControlPremiumChaMove::MsgProcInven( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:

		m_BtnReceive.MsgProc(hWnd, msg, wParam, lParam);
		m_BtnCancle.MsgProc(hWnd, msg, wParam, lParam);

		if( m_BtnReceive.GetState() == BTN_ACTION)
		{
			m_BtnReceive.SetState(BTN_NORMAL);

			char str[128] = {0,};
			sprintf( str, "premium chmove_req_step 3 %s %s\n", m_strNameFrom, m_strNameTo);
			g_pTcpIp->SendNetMessage(str);
			WebClose();
			g_pNk2DFrame->ShowPremiumChaMove(FALSE)	;
			return S_OK;
		}

		if( m_BtnCancle.GetState() == BTN_ACTION )
		{
			m_BtnCancle.SetState(BTN_NORMAL);
			OnCancleChaMove();
			WebClose();
			g_pNk2DFrame->ShowPremiumChaMove(FALSE)	;
			return S_OK;
		}

		break;

	case WM_LBUTTONDOWN:

		m_BtnReceive.MsgProc(hWnd, msg, wParam, lParam);
		m_BtnCancle.MsgProc(hWnd, msg, wParam, lParam);

		if( m_BtnReceive.GetState() == BTN_DOWN)
			return S_OK;

		if( m_BtnCancle.GetState() == BTN_DOWN)
			return S_OK;

		break;
	}

	return S_OK;
}




void CControlPremiumChaMove::DrawAgree()
{
	if( !m_pSurfAgreeBg )
		return;

	g_pDisplay->Blt( m_pSurfAgreeBg->Xpos, m_pSurfAgreeBg->Ypos, m_pSurfAgreeBg );

	if( m_bAgree )
		g_pDisplay->Blt( m_pSurfAgreeYes->Xpos, m_pSurfAgreeYes->Ypos, m_pSurfAgreeYes );
	else
		g_pDisplay->Blt( m_pSurfAgreeNo->Xpos, m_pSurfAgreeNo->Ypos, m_pSurfAgreeNo );

	if( m_bAgree ) // 확인 일때만 보여주자.
		m_BtnOk.Draw();

	m_BtnCancle.Draw();
	m_WebAgree.Draw();

	m_AgreeMultiText->Draw();
}



CItem* CControlPremiumChaMove::DrawInven()
{
	if( !m_pSurfWearingBg )
		return NULL;

	g_pDisplay->Blt( m_pSurfWearingBg->Xpos, m_pSurfWearingBg->Ypos, m_pSurfWearingBg );
	g_pDisplay->Blt( m_pSurfChaName->Xpos, m_pSurfChaName->Ypos, m_pSurfChaName );
	g_pDisplay->Blt( m_pSurfChaRace->Xpos, m_pSurfChaRace->Ypos, m_pSurfChaRace );
	g_pDisplay->Blt( m_pSurfChaLevel->Xpos, m_pSurfChaLevel->Ypos, m_pSurfChaLevel );

	m_BtnReceive.Draw();
	m_BtnCancle.Draw();


	return DrawWearing();
}

CItem* CControlPremiumChaMove::DrawWearing()
{
	CItem* pOverItem = NULL;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	g_pNk2DFrame->GetItemRender()->BeginRenderLolo();

	for(int i=0; i<WEARING_NUM; i++)
	{
		if (m_Wearing[i])
		{
			CLolos *pLolo = GET_LOLO(m_Wearing[i]);
			if (pLolo)
			{
				if (point.x >= m_EquipSlot[i].m_X
						&& point.x < m_EquipSlot[i].m_X + m_EquipSlot[i].m_W
						&& point.y >= m_EquipSlot[i].m_Y
						&& point.y < m_EquipSlot[i].m_Y + m_EquipSlot[i].m_H)
				{
					g_pNk2DFrame->GetItemRender()->RenderLolo(m_EquipSlot[i].m_X + (m_EquipSlot[i].m_W - ITEM_RENDER_WIDTH) / 2
							, m_EquipSlot[i].m_Y + (m_EquipSlot[i].m_H - ITEM_RENDER_HEIGHT) / 2
							, pLolo, GET_LOLOTEXTR_NUM(m_Wearing[i]), 0x00ffffff, FALSE, -1, -1, TRUE, -1, -1, TRUE);
					pOverItem = m_Wearing[i];
				}
				else
				{
					DWORD ambient = 0x00555555;

					if( m_Wearing[i]->m_Vnum == 1286		// 미국 성조기 망토
							|| ( m_Wearing[i]->m_Vnum >= 1598 && m_Wearing[i]->m_Vnum <= 1695 ) // 브라질 요청 남미국기망토 종류도 성조기에서 응용이므로 예외처리.
							|| m_Wearing[i]->m_Vnum == 1287 ) // 성조기 날개도 어둡게 보여서 클라이언트에서 직접 밝게 처리.(by 원석)
						ambient = 0x00cccccc;

					if (m_Wearing[i]->m_PlusNum > 0)
						ambient = UPGRADE_ITEM_COLOR;

					g_pNk2DFrame->GetItemRender()->RenderLolo(m_EquipSlot[i].m_X + (m_EquipSlot[i].m_W - ITEM_RENDER_WIDTH) / 2
							, m_EquipSlot[i].m_Y + (m_EquipSlot[i].m_H - ITEM_RENDER_HEIGHT) / 2
							, pLolo, GET_LOLOTEXTR_NUM(m_Wearing[i]), ambient, FALSE, -1, -1, TRUE, -1, -1, TRUE);
				}
			}
		}
	}

	g_pNk2DFrame->GetItemRender()->EndRenderLolo();

	return pOverItem;
}

BOOL CControlPremiumChaMove::IsInside(int x, int y)
{
	int x1, x2, y1, y2;

	switch( m_Step )
	{
	case 1:
		x1 = m_pSurfAgreeBg->Xpos;
		x2 = x1 + m_pSurfAgreeBg->GetWidth();
		y1 = m_pSurfAgreeBg->Ypos;
		y2 = y1 + m_pSurfAgreeBg->GetHeight();
		break;

	case 2:
		x1 = m_pSurfWearingBg->Xpos;
		x2 = x1 + m_pSurfWearingBg->GetWidth();
		y1 = m_pSurfWearingBg->Ypos;
		y2 = y1 + m_pSurfWearingBg->GetHeight();
		break;

	default:
		return FALSE;
	}

	if( x1 <= x && x2 <= x && y1 <= y && y2 >= y )
		return TRUE;

	return FALSE;
}


BOOL CControlPremiumChaMove::IsInsideAgreeBtn(int x, int y)
{
	if( !m_pSurfAgreeYes )
		return FALSE;

	int x1 = m_pSurfAgreeYes->Xpos;
	int x2 = x1 + m_pSurfAgreeYes->GetWidth();
	int y1 = m_pSurfAgreeYes->Ypos;
	int y2 = y1 + m_pSurfAgreeYes->GetHeight();

	if( x1 <= x && x2 >= x && y1 <= y && y2 >= y )
		return TRUE;

	return FALSE;
}

BOOL CControlPremiumChaMove::ToggleAgreeBtn()
{
	m_bAgree = !m_bAgree;

	if( m_bAgree )
	{
		m_BtnOk.SetDisable(FALSE);
		m_BtnOk.SetState(BTN_DISABLE);
	}
	else
	{
		m_BtnOk.SetDisable(TRUE);
		m_BtnOk.SetState(BTN_NORMAL);
	}

	return TRUE;
}

void CControlPremiumChaMove::OnCancleChaMove()
{
	g_pTcpIp->SendNetMessage("premium chmove_req_stop\n");
	g_pNk2DFrame->SetProcLock(FALSE);
	WebClose();
}



void CControlPremiumChaMove::SetFont(int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h,0,fontname);
}


void CControlPremiumChaMove::SetChaInfo(char* nameFrom, char* nameTo, int level, int race)
{
	strcpy(m_strNameFrom,	nameFrom);
	strcpy(m_strNameTo,		nameTo);
	m_Race	= race;
	m_Level = level;

	char temp[256]= {0,};

	sprintf(temp,G_STRING(IDS_LHSTRING1820), m_strNameFrom);
	g_pDisplay->CreateSurfaceFromText( &m_pSurfChaName, m_Font, temp, RGB(0, 0, 0), RGB(255,200,1000) );
	m_pSurfChaName->SetColorKey(RGB(0, 0, 0));

	sprintf(temp,G_STRING(IDS_LHSTRING1821), g_pNk2DFrame->GetWindowRes()->GetRaceName(m_Race));
	g_pDisplay->CreateSurfaceFromText( &m_pSurfChaRace, m_Font, temp, RGB(0, 0, 0), RGB(255,200,1000) );
	m_pSurfChaRace->SetColorKey(RGB(0, 0, 0));

	sprintf(temp,G_STRING(IDS_LHSTRING1822), m_Level);
	g_pDisplay->CreateSurfaceFromText( &m_pSurfChaLevel, m_Font, temp, RGB(0, 0, 0), RGB(255,200,1000) );
	m_pSurfChaLevel->SetColorKey(RGB(0, 0, 0));

	if( m_pSurfChaName && m_pSurfChaRace && m_pSurfChaLevel )
	{
		m_pSurfChaName->Xpos = m_pSurfWearingBg->Xpos + 24+4;
		m_pSurfChaName->Ypos = m_pSurfWearingBg->Ypos + 293;

		m_pSurfChaRace->Xpos = m_pSurfChaName->Xpos;
		m_pSurfChaRace->Ypos = m_pSurfChaName->Ypos + 20;

		m_pSurfChaLevel->Xpos = m_pSurfChaName->Xpos;
		m_pSurfChaLevel->Ypos = m_pSurfChaRace->Ypos + 20;
	}
}


void CControlPremiumChaMove::SetEquipArea()
{
	m_EquipSlot[0].m_X = m_pSurfWearingBg->Xpos+118; // 헬멧
	m_EquipSlot[0].m_Y = m_pSurfWearingBg->Ypos+12;
	m_EquipSlot[0].m_W = HELMET_SLOT_W;
	m_EquipSlot[0].m_H = HELMET_SLOT_H;

	m_EquipSlot[1].m_X = m_pSurfWearingBg->Xpos+118; // 갑옷
	m_EquipSlot[1].m_Y = m_pSurfWearingBg->Ypos+69;
	m_EquipSlot[1].m_W = ARMOR_SLOT_W;
	m_EquipSlot[1].m_H = ARMOR_SLOT_H;

	m_EquipSlot[2].m_X = m_pSurfWearingBg->Xpos+118; // 바지
	m_EquipSlot[2].m_Y = m_pSurfWearingBg->Ypos+134;
	m_EquipSlot[2].m_W = PANTS_SLOT_W;
	m_EquipSlot[2].m_H = PANTS_SLOT_H;

	m_EquipSlot[3].m_X = m_pSurfWearingBg->Xpos+221; // 망토
	m_EquipSlot[3].m_Y = m_pSurfWearingBg->Ypos+136;
	m_EquipSlot[3].m_W = CLOAK_SLOT_W;
	m_EquipSlot[3].m_H = CLOAK_SLOT_H;

	m_EquipSlot[4].m_X = m_pSurfWearingBg->Xpos+118;  // 신발
	m_EquipSlot[4].m_Y = m_pSurfWearingBg->Ypos+209;
	m_EquipSlot[4].m_W = SHOES_SLOT_W;
	m_EquipSlot[4].m_H = SHOES_SLOT_H;

	m_EquipSlot[5].m_X = m_pSurfWearingBg->Xpos+14; // 방패
	m_EquipSlot[5].m_Y = m_pSurfWearingBg->Ypos+115;
	m_EquipSlot[5].m_W = SHIELD_SLOT_W;
	m_EquipSlot[5].m_H = SHIELD_SLOT_H;

	m_EquipSlot[6].m_X = m_pSurfWearingBg->Xpos+73;  // 목걸이
	m_EquipSlot[6].m_Y = m_pSurfWearingBg->Ypos+32;
	m_EquipSlot[6].m_W = NECKLACE_SLOT_W;
	m_EquipSlot[6].m_H = NECKLACE_SLOT_H;

	m_EquipSlot[7].m_X = m_pSurfWearingBg->Xpos+186;  // 반지
	m_EquipSlot[7].m_Y = m_pSurfWearingBg->Ypos+117;
	m_EquipSlot[7].m_W = RING_SLOT_W;
	m_EquipSlot[7].m_H = RING_SLOT_H;

	m_EquipSlot[8].m_X = m_pSurfWearingBg->Xpos+60;  // 팔찌
	m_EquipSlot[8].m_Y = m_pSurfWearingBg->Ypos+103;
	m_EquipSlot[8].m_W = GLOVES_SLOT_W;
	m_EquipSlot[8].m_H = GLOVES_SLOT_H;

	m_EquipSlot[9].m_X = m_pSurfWearingBg->Xpos+221; // 무기
	m_EquipSlot[9].m_Y = m_pSurfWearingBg->Ypos+77;
	m_EquipSlot[9].m_W = WEAPON_SLOT_W;
	m_EquipSlot[9].m_H = WEAPON_SLOT_H;

	m_EquipSlot[10].m_X = m_pSurfWearingBg->Xpos+15-11; // 무기
	m_EquipSlot[10].m_Y = m_pSurfWearingBg->Ypos+210-14;
	m_EquipSlot[10].m_W = WEAPON_SLOT_W;
	m_EquipSlot[10].m_H = WEAPON_SLOT_H;

	m_EquipSlot[11].m_X = m_pSurfWearingBg->Xpos+15+PET_SLOT_W+22-11; // 무기
	m_EquipSlot[11].m_Y = m_pSurfWearingBg->Ypos+204-14;
	m_EquipSlot[11].m_W = WEAPON_SLOT_W;
	m_EquipSlot[11].m_H = WEAPON_SLOT_H;

	m_EquipSlot[12].m_X = m_pSurfWearingBg->Xpos+15+PET_SLOT_W+152-11; // 무기
	m_EquipSlot[12].m_Y = m_pSurfWearingBg->Ypos+210-14;
	m_EquipSlot[12].m_W = WEAPON_SLOT_W;
	m_EquipSlot[12].m_H = WEAPON_SLOT_H;

	m_EquipSlot[13].m_X = m_pSurfWearingBg->Xpos+15+PET_SLOT_W+198-11; // 무기
	m_EquipSlot[13].m_Y = m_pSurfWearingBg->Ypos+210-14;
	m_EquipSlot[13].m_W = WEAPON_SLOT_W;
	m_EquipSlot[13].m_H = WEAPON_SLOT_H;
}



void CControlPremiumChaMove::Wear(int where, int index, int vnum, int gem_num, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur)
{
	if (vnum < 0)
		return;

	int proto_num = g_ItemList.FindItem(vnum);

	if (proto_num >= 0)
	{
		SAFE_DELETE(m_Wearing[where]);
		m_Wearing[where] = new CItem;
		if( !m_Wearing[where] )
			return;
		m_Wearing[where]->m_Index = index;
		m_Wearing[where]->m_Vnum = vnum;
		m_Wearing[where]->m_ProtoNum = proto_num;
		//2003 착용하고 있는 서브아이템을 플러스아이템으로 인식하지 않게 하기.
		if(GET_TYPE(m_Wearing[where])==ITYPE_SUB)
			m_Wearing[where]->m_PlusNum = gem_num;
		else if (gem_num > 0)
		{
			if(g_ItemList.m_ItemArray[proto_num].m_MinLevel>=181)
				m_Wearing[where]->SetDiaGemNum(gem_num);
			else
				m_Wearing[where]->SetGemNum(gem_num);
		}

		m_Wearing[where]->m_Special = special;

		///신방어구강화시스템
		m_Wearing[where]->m_Special2 = special2; // 추가 옵션
		m_Wearing[where]->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		m_Wearing[where]->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

		m_Wearing[where]->m_Endurance = Endur;
		m_Wearing[where]->m_MaxEndurance = MaxEndur;

		// 아이템 값을 입력해보자!
		ResetItemValue(m_Wearing[where]);
		SetSpecialValue(m_Wearing[where], GET_TYPE(m_Wearing[where]), GET_SHAPE(m_Wearing[where]), special, special2);
	}
}

void CControlPremiumChaMove::DeleteWear()
{
	for (int i = 0; i < WEARING_NUM; i++)
		SAFE_DELETE(m_Wearing[i]);
}


void CControlPremiumChaMove::WebOpen()
{
	return;
	m_WebAgree.LoadRes(1);

	if( g_bFFullMode )
		m_WebAgree.Open( "http://121.254.248.48/update2/agree.htm", m_pSurfAgreeBg->Xpos+14, m_pSurfAgreeBg->Ypos+44, 244, 272 );
	else
		m_WebAgree.Open( "http://121.254.248.48/update2/agree.htm", m_pSurfAgreeBg->Xpos+14+4, m_pSurfAgreeBg->Ypos+44+49, 244, 272 );

}

void CControlPremiumChaMove::WebClose()
{
	return;
	m_WebAgree.Close();
	m_WebAgree.DeleteRes();
}


void CControlPremiumChaMove::SetSlot(int slot)
{
	m_slot = slot;
}