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
#include "ControlTeleportItem.h"
#include "Nk2DFrame.h"
#include "Country.h"
#include "Compas.h"
#include "g_stringmanager.h"
#include "UiMgr.h"
#include "CommonConfig.h"

extern DWORD	g_dwLangType;
extern DWORD	g_dwClientCountry;
// �������ۿ��� ��ǥ�� ������� �Ʒ��� �ΰ� ������ ����.(by ����)
extern int		gMapHeight;
extern int		gMapExtention;		// ������ ���� - ������

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlTeleportItem::CControlTeleportItem()
	: m_slot_index(0)
{
	m_pBack			= NULL;

	m_iQuickSlotNum = 0;

	m_bActive		= TRUE;

	m_dwLastOpenTime	= timeGetTime();
	m_IsOpenWindow		= FALSE;
	m_IsVisible			= FALSE;

	for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
	{
		m_iCurZoneNum[i] = 9999;
	}
}

CControlTeleportItem::~CControlTeleportItem()
{
	DeleteRes();
}

void CControlTeleportItem::LoadRes(int QuickSlotNum)
{
	if(!g_pDisplay)
		return;

	m_iQuickSlotNum = QuickSlotNum;

	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/teleport/teleport.bmp");
	m_pBack->SetColorKey( TRANS_COLOR );
	m_pBack->Xpos = ( g_pNk2DFrame->GetClientWidth() - m_pBack->GetWidth() ) >> 1;
	m_pBack->Ypos = ( g_pNk2DFrame->GetClientHeight() - m_pBack->GetHeight() ) >> 1;

	int bgx = m_pBack->Xpos;
	int bgy = m_pBack->Ypos;

	for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
	{
		m_SavePosBtn[i].Init("teleport/btn_save");
		m_LoadPosBtn[i].Init("teleport/btn_send");
		m_SavePosBtn[i].SetPosition(bgx + 272, bgy + 62 + (28 * i));
		m_LoadPosBtn[i].SetPosition(bgx + 328, bgy + 62 + (28 * i));

		m_pCur_ZonePos[i].Init(12, _FCOLOR_YELLOW, 0, 0);
		//-- IDS_NOT_SET : �����ȵ�
		m_pCur_ZonePos[i].SetString((char*)G_STRING(IDS_NOT_SET));
		m_pCur_ZonePos[i].SetPos(bgx + 40, bgy + 68 + (28 * i));
	}

	m_CloseBtn.Init("common/btn_close_01");
	m_CloseBtn.SetPosition( bgx + 397 , bgy + 5 );
}

void CControlTeleportItem::DeleteRes()
{
	SAFE_DELETE(m_pBack);

	for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
	{
		m_SavePosBtn[i].DeleteRes();
		m_LoadPosBtn[i].DeleteRes();
		m_pCur_ZonePos[i].DeleteRes();
	}

	m_CloseBtn.DeleteRes();
}
BOOL CControlTeleportItem::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	return true;
}

bool isInsideRect( RECT &rc, POINT & pt )
{
	if( rc.left <= pt.x && rc.right > pt.x && rc.top <= pt.y && rc.bottom > pt.y )
		return true;
	return false;
}

void setRect( RECT &rc, int left, int top, int right, int bottom )
{
	rc.left = left;
	rc.top = top;
	rc.right = right;
	rc.bottom = bottom;
}

bool CControlTeleportItem::IsBossRoom()
{
	POINT pt;
	RECT turan, valkaria, barshya_druyd, barshya_naga, barshya_behemoth, queise, beargrid;

	pt.x = g_pRoh->m_wx;
	pt.y = g_pRoh->m_wz;
	setRect( turan, 8135, 12065, 12225, 8865 );
	setRect( valkaria, 8765, 4535, 12255, 1105 );
	setRect( barshya_druyd, 1195, 7575, 4055, 4515 );
	setRect( barshya_naga, 9025, 12385, 12515, 8955 );
	setRect( barshya_behemoth, 8455,4335,12335, 1155 );
	setRect( queise, 2005, 8275, 5705, 4875 );
	setRect( beargrid, 8005, 12195, 12385, 7235 );

	return isInsideRect( turan, pt ) || isInsideRect( valkaria, pt ) || isInsideRect( barshya_druyd, pt ) ||
		   isInsideRect( barshya_naga, pt ) || isInsideRect( barshya_behemoth, pt ) || isInsideRect( queise, pt ) ||
		   isInsideRect( beargrid, pt );
}

void CControlTeleportItem::SendSavePosMsg(int slotnum)
{
	int 	CurMapPosX = 0; // ��ǥ ���ϴµ� ���� ����.
	int		CurMapPosZ = 0;

	if( !g_pRoh || !pCMyApp->m_pUIMgr )
		return;

	if( !( g_pRoh->m_QuickSlot[ m_slot_index ] ) )
		return;

	if( !( g_pRoh->m_QuickSlot[ m_slot_index ]->m_Vnum == 1015
			&& ( pCMyApp->m_pUIMgr->m_bOnBattle
				 || pCMyApp->IsPrisonArea( g_pRoh->m_wx, g_pRoh->m_wz )
				 || pCMyApp->IsGuildHuntArea( g_pRoh->m_wx, g_pRoh->m_wz )
			   ) ) )
	{
		//////////////////////////////////////////////////////////////////////
		// ���� ������ ��ǥ�� �ǽð����� ���;� �Ѵ�. �������� ���۽� ��ǥ�� ���Դµ� �̷��� �ϸ� ���������� �߸��� ��ǥ�� ����. (by ����)
		if( pCMyApp->GetCurWorld() == 1 || pCMyApp->GetCurWorld() == 2 || pCMyApp->GetCurWorld() == 5 )
		{
			CurMapPosX = (g_pRoh->m_wx/(gMapExtention/4));
			CurMapPosZ = (int)(gMapHeight*4-(g_pRoh->m_wz/(gMapExtention/4)));
		}
		else
		{
			CurMapPosX = (g_pRoh->m_wx/(gMapExtention/5));
			CurMapPosZ = (int)(gMapHeight*5-(g_pRoh->m_wz/(gMapExtention/5)));
		}


		char param[64];
		sprintf( param , "1 %d %d %d" , slotnum , CurMapPosX , CurMapPosZ  );
		pCMyApp->m_pUIMgr->UseWarp( m_slot_index , param );
	}
//==============================================================
}


int CControlTeleportItem::SendLoadPosMsg(int slotnum)
{
	if( pCMyApp->GetCurWorld() != m_iCurZoneNum[slotnum] )
	{
		if( m_iCurZoneNum[slotnum] == 9999 )
			//-- IDS_NOTSET_PLACE : ���� ��ġ�� ���� �Ǿ� ���� �ʽ��ϴ�
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NOTSET_PLACE), TYPE_NOR_OK, 1 );
		else
			//-- IDS_CANNOT_TRANS : �ش� ������ �̵��� �� �����ϴ�
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANNOT_TRANS), TYPE_NOR_OK, 1 );
		return 1;
	}

	if( pCMyApp->GetCurWorld() > 10 && !pCMyApp->m_pUIMgr->m_bAutoHunt )
	{	
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PAYMENT_LR), TYPE_NOR_OK, 1 );
		return 1;
	}

	if( CommonConfig::Instance()->GetLagrush() == true )
	{
		if( IsBossRoom() )
		{
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANNOT_USEINZONE), TYPE_NOR_OK, 1 );
			return 1;
		}
	}

//==============================================================
	if( !g_pRoh || !pCMyApp->m_pUIMgr )
		return 2;
	// nate 2004 - 6
	// ������ ������� �̵���ġ ��� ����
	// �������� ��� �ȵ�

	if( !( g_pRoh->m_QuickSlot[ m_slot_index ] ) )
		return 1;

	if( !( g_pRoh->m_QuickSlot[ m_slot_index ]->m_Vnum == 1015
			&& ( pCMyApp->m_pUIMgr->m_bOnBattle
				 || pCMyApp->IsPrisonArea( g_pRoh->m_wx, g_pRoh->m_wz )
				 || pCMyApp->IsGuildHuntArea( g_pRoh->m_wx, g_pRoh->m_wz )
			   ) ) )
	{
		char param[64];
		sprintf( param , "2 %d" , slotnum );
		pCMyApp->m_pUIMgr->UseWarp( m_slot_index , param );
	}
//==============================================================
	return 2;
}
void CControlTeleportItem::SetZonePosString(int index, int zonenum, int xpos, int ypos)
{
	char strZonePos[255] = {0,};

	if( zonenum == 0 )
		//-- IDS_GATE_LOST_REALM : �ν�Ʈ���� ��
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_GATE_LOST_REALM), xpos, ypos);
	else if( zonenum == 1 )
		//-- IDS_GATE_DEKALEN : ��ī��
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_GATE_DEKALEN), xpos, ypos);
	else if( zonenum == 2 )
		//-- IDS_GATE_DEKADUNE : ��ī��
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_GATE_DEKADUNE), xpos, ypos);
	else if( zonenum == 3 )
		//-- IDS_GATE_SHAILON : ���Ϸ� �ʵ�
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_GATE_SHAILON), xpos, ypos);
	else if( zonenum == 4 )
		//-- IDS_GATE_LAGRAMIA : ��׶�̾�
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_GATE_LAGRAMIA), xpos, ypos);
	else if( zonenum == 5 )
		//-- IDS_GATE_SIRUSTE : �÷罺Ʈ
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_GATE_SIRUSTE), xpos, ypos);
	else if( zonenum == 6 )
		//-- IDS_MAP_PHAROS : �ķν�
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_MAP_PHAROS), xpos, ypos);
	else if( zonenum == 7 )
		//-- IDS_GATE_WHITEHORN : �׷���Ʈ ȭ��Ʈȥ
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_GATE_WHITEHORN), xpos, ypos);
	//wan:2004-12
	else if( zonenum == 8 )
		//-- IDS_DUNGEON4 : ������ ������Ƽ
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_DUNGEON4), xpos, ypos);
	// [4/20/2009 D.K ] : D.K ��׷��� ���� ���� Ǯ��.
	else if( zonenum == 9 )
		//-- IDS_MAP_DMITRON
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_MAP_DMITRON), xpos, ypos);
	else if( zonenum == 10 )
		//-- IDS_MAP_MATRIX
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_MATRIX), xpos, ypos);
	else if( zonenum == 13 )
		//-- IDS_WORLD_13
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_WORLD_13), xpos, ypos);
	else if( zonenum == 14 )
		//-- IDS_WORLD_14
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_WORLD_14), xpos, ypos);
	else if( zonenum == 15 )
		//-- IDS_WORLD_15
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_WORLD_15), xpos, ypos);
	else if( zonenum == 16 )
		//-- IDS_WORLD_16
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_WORLD_16), xpos, ypos);
	else if( zonenum == 17 )
		//-- IDS_WORLD_17
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_WORLD_17), xpos, ypos);
	else if( zonenum == 19 )
		//-- MOBIUS
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_WORLD_10), xpos, ypos);
	else if( zonenum == 20 )
		//-- S1
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING1829), xpos, ypos);
	else if( zonenum == 21 )
		//-- KRUMA
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING1830), xpos, ypos);
	else if( zonenum == 22 )
	{
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING1892), xpos, ypos);
	}
	else if( zonenum == 23 )
	{
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING1896), xpos, ypos);
	}
	else if( zonenum == 24 )
	{
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING1897), xpos, ypos);
	}
	else if( zonenum == 25 )
	{
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING4555), xpos, ypos);
	}
	else if( zonenum == 26 )
	{
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING4556), xpos, ypos);
	}
	else if( zonenum == 27 )
	{
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING4557), xpos, ypos);
	}
	else if( zonenum == 28 )
	{
		sprintf(strZonePos, "%s (%d, %d)", G_STRING(IDS_LHSTRING4558), xpos, ypos);
	}


	else if( zonenum == 9999 )
		//-- IDS_NOT_SET : �����ȵ�
		sprintf(strZonePos, "%s", G_STRING(IDS_NOT_SET));

	m_pCur_ZonePos[index].SetString(strZonePos);
	m_iCurZoneNum[index] = zonenum;
}
void CControlTeleportItem::Draw()
{
	if(!m_bActive)
		return;

	bool IsVisible = FALSE;

	if(	g_pNk2DFrame
			&& ( g_pNk2DFrame->IsHelpVisible()
				 || g_pNk2DFrame->IsInvenVisible()
				 || g_pNk2DFrame->IsPartyVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(SCORE)
				 || g_pNk2DFrame->IsSkillVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(MAP)
				 || g_pNk2DFrame->IsPetStatusVisible()
				 || g_pNk2DFrame->IsInterfaceVisible(QUEST)
				 || g_pNk2DFrame->IsControlGuild()
				 || g_pNk2DFrame->IsControlGuildManage()
			   ))
		IsVisible = TRUE;
	else
		IsVisible = FALSE;

	if( m_IsVisible != IsVisible )
	{
		m_IsVisible = IsVisible;
		SetDrawPos();
	}

	if( g_pDisplay && m_pBack )
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

	for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
	{
		m_SavePosBtn[i].Draw();
		m_LoadPosBtn[i].Draw();
		m_pCur_ZonePos[i].Draw(214);
	}
	m_CloseBtn.Draw();

	if(	m_IsOpenWindow == FALSE )
	{
		if ( (timeGetTime() - m_dwLastOpenTime) > 300 )
		{
			m_IsOpenWindow = TRUE;
			m_dwLastOpenTime = timeGetTime();
		}
	}
}

void CControlTeleportItem::SetDrawPos()
{
}


LRESULT CControlTeleportItem::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int result = 1;

	if(!m_bActive)
		return -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))
			return 2;

		for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
		{
			m_SavePosBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			m_LoadPosBtn[i].MsgProc(hWnd, msg, wParam, lParam);
		}

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		return 1;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 2;

		for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
		{
			m_SavePosBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			m_LoadPosBtn[i].MsgProc(hWnd, msg, wParam, lParam);
		}
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

		for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
		{
			if( m_SavePosBtn[i].m_iBtnFlg == BTN_ACTION )
			{
				SendSavePosMsg(i);
				m_SavePosBtn[i].m_iBtnFlg = BTN_NORMAL;
				return 2;
			}
			if( m_LoadPosBtn[i].m_iBtnFlg == BTN_ACTION )
			{
				result = SendLoadPosMsg(i);
				m_LoadPosBtn[i].m_iBtnFlg = BTN_NORMAL;
				return result;
			}
		}

		if(m_CloseBtn.m_iBtnFlg == BTN_ACTION)
		{
			m_CloseBtn.m_iBtnFlg = BTN_NORMAL;
			return 2;
		}

		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		for( int i = 0; i < MAX_TELEPORT_COUNT; i++ )
		{
			m_SavePosBtn[i].MsgProc(hWnd, msg, wParam, lParam);
			m_LoadPosBtn[i].MsgProc(hWnd, msg, wParam, lParam);
		}

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		return 1;
	}
	return 0;
}

