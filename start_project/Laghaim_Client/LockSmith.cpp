#include "stdafx.h"
#include <string.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "g_stringmanager.h"
#include "Nk2DFrame.h"
#include "ControlInven.h"
#include "Tcpipcon.h"
#include "TcpUtil.h"
#include "Dirtsnd.h"
#include "LoadEffectSound.h"
#include ".\locksmith.h"
#include "CommonConfig.h"

extern int					g_EffectSound[];

CLockSmith::CLockSmith(void)
	: m_hWnd(NULL)
	, m_slotbox_x(0)
	, m_slotbox_y(0)
	, m_slotkey_x(0)
	, m_slotkey_y(0)
	, m_dwMouseClickTickTime(0)
{
	int			i;

	for( i = 0; i < CUBE_ANICOUNT; i++ )
		m_pCubeAni[i]	= NULL;

	for( i = 0; i < RS_END; i++ )
		m_pResult[i]	= NULL;

	m_pBack					= NULL;
	m_pBtnCancel			= NULL;
	m_pBtnDecision			= NULL;
	m_pOverItem				= NULL;
	m_pExistSlot			= NULL;
	m_pEnableSlot			= NULL;
	m_pDisableSlot			= NULL;
	m_pItemRender			= NULL;

	m_bProcess				= false;
	m_bResultDraw			= false;
	m_bMsg					= false;

	m_dwStartTime			= 0;
	m_dwMsgTime				= 0;
	m_iAniCount				= 0;
	m_iResult				= 0;

	m_iBoxIndex[0]			= 2373;
	m_iBoxIndex[1]			= 2374;
	m_iBoxIndex[2]			= 2375;
	m_iBoxIndex[3]			= 2376;

	m_iKeyIndex[0]			= 2368;
	m_iKeyIndex[1]			= 2372;
	m_iKeyIndex[2]			= 2369;
	m_iKeyIndex[3]			= 2371;
	m_iKeyIndex[4]			= 2370;
	m_iKeyIndex[5]			= 3761;

	m_box_vnum				= -1;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CLockSmith::~CLockSmith(void)
{
}

void CLockSmith::LoadRes( CItemRender *pItemRender)
{
	int			i;
	char		buf[128];

	if( !g_pDisplay )
		return;

	m_pItemRender = pItemRender;

	m_bProcess				= false;
	m_bResultDraw			= false;
	m_bMsg					= false;

	m_dwStartTime			= 0;
	m_iAniCount				= 0;
	m_iResult				= 0;

	m_hWnd	=  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_pBtnCancel			= new CBasicButton();
	m_pBtnDecision			= new CBasicButton();

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot,	"interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot,	"interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot,		"interface/slot/slot_exist.bmp");

	g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, BACKIMAGE);				// 빽판
	m_pBack->SetColorKey( TRANS_COLOR );
	m_pBack->Xpos = m_ClientRc.right - 291 - m_pBack->GetWidth();
	m_pBack->Ypos = 0;

	for( i = 0; i < CUBE_ANICOUNT; i++ )									// 애니메이션 이미지
	{
		sprintf( buf , ANIIMAGE , i+1 );
		g_pDisplay->CreateSurfaceFromBitmap (&m_pCubeAni[i], buf);
		m_pCubeAni[i]->Xpos = m_pBack->Xpos + 16;
		m_pCubeAni[i]->Ypos = m_pBack->Ypos + 64;
	}

	for( i = 0; i < RS_END; i++ )											// 결과 이미지
	{
		sprintf( buf , RESULTIMAGE , i+1 );
		g_pDisplay->CreateSurfaceFromBitmap (&m_pResult[i], buf);
		m_pResult[i]->Xpos = m_pBack->Xpos + 16;
		m_pResult[i]->Ypos = m_pBack->Ypos + 64;
	}

	if( m_pBtnCancel )
	{
		m_pBtnCancel->SetFileName(BTN_CANCEL);
		m_pBtnCancel->LoadRes();
		m_pBtnCancel->SetPosition( m_pBack->Xpos+214, m_pBack->Ypos+6 );
	}
	if( m_pBtnDecision)
	{
		m_pBtnDecision->SetFileName(BTN_DECISION);
		m_pBtnDecision->LoadRes();
		m_pBtnDecision->SetPosition( m_pBack->Xpos+47, m_pBack->Ypos+310);
	}

	m_slotbox_x = m_pBack->Xpos+52;
	m_slotbox_y = m_pBack->Ypos+369;   // 54
	m_slotkey_x = m_pBack->Xpos+156;
	m_slotkey_y = m_pBack->Ypos+383;   // 27

	m_dwMouseClickTickTime = timeGetTime();
}

LRESULT CLockSmith::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	// 마우스가 빽판 위에 올려져있을때만 해당 버튼의 msgproc를 부른다
	if( m_bProcess )
		return TRUE;

	m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
	m_pBtnDecision->MsgProc(hWnd, msg, wParam, lParam);
	switch ( msg )
	{
	case WM_MOUSEMOVE:
		if( IsInside(x,y) )
		{
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtnDecision->MsgProc(hWnd, msg, wParam, lParam);
		}
		return TRUE;

	case WM_LBUTTONDOWN:
		if( g_pNk2DFrame->GetControlInven()->IsInside(x,y) ) return FALSE;

		else if( !IsInside(x,y) && !g_pNk2DFrame->GetControlInven()->IsInside(x,y) )
		{
			g_pNk2DFrame->ShowLockSmithWindow(FALSE);
			return TRUE;
		}
		else
		{
			if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
				return TRUE;
			m_dwMouseClickTickTime = timeGetTime();

			// 창 닫기
			if (m_pBtnCancel && m_pBtnCancel->GetState() == BTN_DOWN)
				g_pNk2DFrame->ShowLockSmithWindow(FALSE);

			// 주사위 돌리기
			else if (m_pBtnDecision && m_pBtnDecision->GetState() == BTN_DOWN)
			{
				CItem	*pItem = g_pRoh->m_ExgInven;
				int		iBox = 0;
				int		iKey = 0;
				int		iBoxIdx=0, iKeyIdx=0;

				while (pItem)
				{
					if( pItem->m_Vnum >= BOXSTARTINDEX && pItem->m_Vnum <= BOXENDINDEX )
					{
						iBox = pItem->m_Vnum;
					}
					else if( pItem->m_Vnum >= KEYSTARTINDEX && pItem->m_Vnum <= KEYENDINDEX ||
							 pItem->m_Vnum == 3761 )
					{
						iKey = pItem->m_Vnum;
					}

					pItem = pItem->m_Next;
				}

				// 아이템이 맞는지 체크
				if( iBox == 0 || iKey == 0 )
				{
					//-- IDS_GUILD_INVASION_FAIL : 조건에 맞지 않습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_INVASION_FAIL), TYPE_NOR_OK, 1 );
					return TRUE;
				}
				else
				{
					for( int i = 0; i < 4; i++ )
					{
						if( iBox == m_iBoxIndex[i] )
							iBoxIdx = i;
					}
					for( int i = 0; i < 6; i++ )
					{
						if( iKey == m_iKeyIndex[i] )
							iKeyIdx = i;
					}

					if( iBoxIdx != iKeyIdx )
					{
						if( iKeyIdx != 4 && iKeyIdx != 5 )
						{
							//-- IDS_GAMBLE_INCLUDE_NOTCHIP : 다른종류의 아이템입니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GUILD_INVASION_FAIL), TYPE_NOR_OK, 1 );
							return TRUE;
						}
					}
				}

				m_box_vnum		= m_iBoxIndex[iBoxIdx];
				m_iAniCount		= 0;
				m_iResult		= 0;
				m_bProcess		= true;
				m_bResultDraw	= false;
				m_dwStartTime	= GetTickCount();
				m_pBtnDecision->SetState(BTN_NORMAL);

				// 효과음 출력
				if( g_pDSound )
					g_pDSound->Play( g_EffectSound[EFF_SND_LOCKSMITH] );

				sprintf(m_MsgBuf, "ex_item sb\n");
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(m_MsgBuf);
			}

			// 보물상자 아이템 넣을때
			else if(x >= m_slotbox_x && x < m_slotbox_x + BOXWH &&	y >= m_slotbox_y && y < m_slotbox_y + BOXWH )
			{
				// 마우스에 아이템이 있다면
				if( g_pRoh->m_ExtraSlot )
				{
					if( g_pRoh->m_ExtraSlot->m_Vnum >= BOXSTARTINDEX && g_pRoh->m_ExtraSlot->m_Vnum <= BOXENDINDEX )
					{
						if (g_pRoh->AddToExgInven(EXTRASLOT_BOX_X, EXTRASLOT_BOX_Y))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", EXTRASLOT_BOX_X, EXTRASLOT_BOX_Y);
							if (g_pTcpIp)
								g_pTcpIp->SendNetMessage(m_MsgBuf);
						}
					}
				}

				// 아이템 빼기
				else
				{
					if (g_pRoh->AddToExgInven(EXTRASLOT_BOX_X, EXTRASLOT_BOX_Y))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", EXTRASLOT_BOX_X, EXTRASLOT_BOX_Y);
						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage(m_MsgBuf);
					}
				}
			}

			// 키 아이템 넣을때
			else if(x >= m_slotkey_x && x < m_slotkey_x + KEYWH &&	y >= m_slotkey_y && y < m_slotkey_y + KEYWH )
			{
				// 마우스에 아이템이 있다면
				if( g_pRoh->m_ExtraSlot )
				{
					if( g_pRoh->m_ExtraSlot->m_Vnum >= KEYSTARTINDEX && g_pRoh->m_ExtraSlot->m_Vnum <= KEYENDINDEX ||
							g_pRoh->m_ExtraSlot->m_Vnum == 3761 )
					{
						if (g_pRoh->AddToExgInven(EXTRASLOT_KEY_X, EXTRASLOT_KEY_Y))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", EXTRASLOT_KEY_X, EXTRASLOT_KEY_Y);
							if (g_pTcpIp)
								g_pTcpIp->SendNetMessage(m_MsgBuf);
						}
					}
				}

				// 아이템 빼기
				else
				{
					if (g_pRoh->AddToExgInven(EXTRASLOT_KEY_X, EXTRASLOT_KEY_Y))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", EXTRASLOT_KEY_X, EXTRASLOT_KEY_Y);
						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage(m_MsgBuf);
					}
				}
			}
		}
		return TRUE;

	}

	return FALSE;
}

CItem* CLockSmith::Draw(void)
{
	m_pOverItem = NULL;

	g_pDisplay->Blt(m_pBack->Xpos , m_pBack->Ypos, m_pBack);
	m_pBtnCancel->Draw();
	m_pBtnDecision->Draw();

	DWORD dwTemp	= GetTickCount();

	if( m_bProcess && !m_bResultDraw )
	{
		if( m_dwStartTime != 0 && m_dwStartTime + 95 < dwTemp )
		{
			m_iAniCount++;

			m_dwStartTime = dwTemp;

			if( m_iAniCount >= CUBE_ANICOUNT )
			{
				char	g_Buf[52];

				int nlen = strlen(m_MsgBuf);
				m_MsgBuf[nlen+1] = '\0';
				char* orgPtr = NULL;
				char* pszTemp = new char[nlen+1];
				strncpy( pszTemp, m_MsgBuf, nlen+1);

				// input original pointer.
				orgPtr = pszTemp;

				pszTemp		= AnyOneArg(pszTemp, g_Buf);
				m_iResult	= atoi( g_Buf );

				SAFE_DELETE_ARRAY(orgPtr);

// 				m_bProcess		= false;
				m_bResultDraw	= true;
				m_bMsg			= true;

				m_dwMsgTime		= dwTemp;
			}
		}

		g_pDisplay->Blt(m_pCubeAni[m_iAniCount]->Xpos , m_pCubeAni[m_iAniCount]->Ypos, m_pCubeAni[m_iAniCount]);
	}

	if( m_dwMsgTime !=0 && m_dwMsgTime + 1000 < dwTemp && m_bMsg )
	{
		// IDS_EMCEE_ITEM_CHECK
		m_bProcess		= false;
		m_bMsg = false;

		char msg_buf[256];
		int idx = g_ItemList.FindItem( m_box_vnum );
		if( idx == -1 )
		{
			// error
		}
		else
		{

			g_pNk2DFrame->InsertPopup( ( char *)G_STRING(IDS_LHSTRING1773) ,TYPE_NOR_OK, 1);

			sprintf( msg_buf , ( char *)G_STRING(IDS_LHSTRING1774), GET_ITEM_NAME_VNUM(m_box_vnum) , GetResultNum(m_iResult) );
			g_pNk2DFrame->AddChatStr( msg_buf ,-1 );
			g_pNk2DFrame->AddChatStr( ( char *)G_STRING(IDS_LHSTRING1773) ,-1 );

			m_box_vnum = -1;
		}
	}


	if( m_bResultDraw )
		g_pDisplay->Blt(m_pResult[m_iResult]->Xpos , m_pResult[m_iResult]->Ypos, m_pResult[m_iResult]);


	DrawMySlot();

	return m_pOverItem;
}

void CLockSmith::DrawMySlot(void)
{

	RECT rcRect;
	POINT point;
	//int slot_x_num, slot_y_num;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = g_pRoh->m_ExgInven;
	while (pItem)
	{
		if( pItem->m_Vnum >= BOXSTARTINDEX && pItem->m_Vnum <= BOXENDINDEX )
		{
			rcRect.left = 0;
			rcRect.top = 0;
			rcRect.right = BOXWH;
			rcRect.bottom = BOXWH;

			g_pDisplay->Blt( m_slotbox_x, m_slotbox_y, m_pExistSlot, &rcRect);
		}

		if( pItem->m_Vnum >= KEYSTARTINDEX && pItem->m_Vnum <= KEYENDINDEX ||
				pItem->m_Vnum == 3761 )
		{
			rcRect.left = 0;
			rcRect.top = 0;
			rcRect.right = KEYWH;
			rcRect.bottom = KEYWH;

			g_pDisplay->Blt( m_slotkey_x, m_slotkey_y, m_pExistSlot, &rcRect);
		}
		pItem = pItem->m_Next;
	}


	// 내 교환품 그리기..
	int index = 0;

	if ( (point.x >= m_slotbox_x && point.x <= m_slotbox_x+BOXWH) && (point.y >= m_slotbox_y && point.y <= m_slotbox_y+BOXWH))
		index = g_pRoh->GetExgSlotIndex(EXTRASLOT_BOX_X, EXTRASLOT_BOX_Y);
	else if ( (point.x >= m_slotkey_x && point.x <= m_slotkey_x+KEYWH) && (point.y >= m_slotkey_y && point.y <= m_slotkey_y+KEYWH))
		index = g_pRoh->GetExgSlotIndex(EXTRASLOT_KEY_X, EXTRASLOT_KEY_Y);
	else
		index = g_pRoh->GetExgSlotIndex(-1, -1);

	pItem = g_pRoh->m_ExgInven;

	if( !m_pItemRender )
		return;

	m_pItemRender->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.

	while (pItem)
	{
		if (pItem->m_Index == index)
		{
			if( pItem->m_Vnum >= BOXSTARTINDEX && pItem->m_Vnum <= BOXENDINDEX )
			{
				g_pNk2DFrame->RenderItemInUI( m_slotbox_x, m_slotbox_y - SLOT_HEIGHT, pItem , TRUE , FALSE , TRUE , TRUE );
				m_pOverItem = pItem;
			}
			else if( pItem->m_Vnum >= KEYSTARTINDEX && pItem->m_Vnum <= KEYENDINDEX ||
					 pItem->m_Vnum == 3761 )
			{
				g_pNk2DFrame->RenderItemInUI( m_slotkey_x + (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2,
											  m_slotkey_y  + (GET_ITEM_HEIGHT(pItem) * SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
											  pItem , TRUE , FALSE , TRUE , TRUE );

				m_pOverItem = pItem;
			}

		}
		else
		{
			DWORD ambient = 0x00555555;
			if(g_pNk2DFrame->IsScroll(pItem))
				ambient = 0x00cccccc;
			else if (pItem->m_PlusNum > 0)
				ambient = UPGRADE_ITEM_COLOR;

			if( pItem->m_Vnum >= BOXSTARTINDEX && pItem->m_Vnum <= BOXENDINDEX )
			{
				g_pNk2DFrame->RenderItemInUI( m_slotbox_x, m_slotbox_y - SLOT_HEIGHT, pItem , FALSE , FALSE , TRUE , TRUE );
				//m_pOverItem = NULL;
			}
			else if( pItem->m_Vnum >= KEYSTARTINDEX && pItem->m_Vnum <= KEYENDINDEX ||
					 pItem->m_Vnum == 3761 )
			{
				g_pNk2DFrame->RenderItemInUI( m_slotkey_x + (GET_ITEM_WIDTH(pItem) * SLOT_WIDTH - ITEM_RENDER_WIDTH) /2,
											  m_slotkey_y  + (GET_ITEM_HEIGHT(pItem) * SLOT_HEIGHT - ITEM_RENDER_HEIGHT) /2,
											  pItem , FALSE , FALSE , TRUE , TRUE );

				//m_pOverItem = NULL;
			}
		}
		pItem = pItem->m_Next;
	}
	m_pItemRender->EndRenderLolo();

}

void CLockSmith::DeleteRes(void)
{
	int		i;

	SAFE_DELETE(m_pBack);
	for( i = 0; i < CUBE_ANICOUNT; i++ )
		SAFE_DELETE(m_pCubeAni[i]);
	for( i = 0; i < RS_END; i++ )
		SAFE_DELETE(m_pResult[i]);

	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnDecision);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
}

BOOL CLockSmith::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y);
}


int CLockSmith::GetResultNum(int rv)
{
	int total_num = 0;

	if( CommonConfig::Instance()->GetKorFlag() == false )
		return total_num;

	switch( rv )
	{
	case RS_11:
		total_num = 2;
		break;
	case RS_12:
		total_num = 3;
		break;
	case RS_13:
	case RS_22:
		total_num = 4;
		break;
	case RS_14:
	case RS_23:
		total_num = 5;
		break;
	case RS_15:
	case RS_24:
	case RS_33:
		total_num = 6;
		break;
	case RS_16:
	case RS_25:
	case RS_34:
		total_num = 7;
		break;
	case RS_26:
	case RS_35:
	case RS_44:
		total_num = 8;
		break;
	case RS_36:
	case RS_45:
		total_num = 9;
		break;
	case RS_46:
	case RS_55:
		total_num = 10;
		break;
	case RS_56:
		total_num = 11;
		break;
	case RS_66:
		total_num = 12;
		break;
	}

	return total_num;
}
