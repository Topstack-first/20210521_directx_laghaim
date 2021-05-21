#include "stdafx.h"
#include "ControlYutboard.h"
#include "Nk2DFrame.h"
#include "Tcpipcon.h"

const int YUT_FRAME_TIME = 500;
const int UNIT_MOVE_TIME = 500;

ControlYutboard::ControlYutboard()
	: m_result(0)
	, m_aniYut(false)
	, m_aniUnitMove(false)
	, m_timeAniYutStart(0)
	, m_timeAniUnitMoveStart(0)
	, m_surYut(NULL)
	, m_surResult(NULL)
	, m_curPos(-1)
	, m_CompleteCount(0)
{
}

ControlYutboard::~ControlYutboard()
{
	DeleteRes();
}

HRESULT ControlYutboard::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlYutboard::LoadRes()
{
	if( YutManager::GetInstance()->GetYutnoriOn() == false )
	{
		return;
	}

	m_background.Init("yutnori/yut_board");
	m_btn_start.Init("yutnori/BTN_YUT_START");
	m_btn_close.Init("common/btn_close_01");

	char szFilePath[MAX_PATH] = {0,};
	for(int i = 0; i < MAX_UNIT_TYPE; i++)
	{
		sprintf(szFilePath, "yutnori/unit_0%d", i + 1);
		m_img_unit[i].Init(szFilePath);
	}

	m_background.Align( 6 , g_pNk2DFrame->GetClientWidth() , g_pNk2DFrame->GetClientHeight() );
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btn_start.SetPosition(bgx + 548, bgy + 380);
	m_btn_close.SetPosition(bgx + 738, bgy + 22);

	DWORD textColor = RGB(255, 255, 255);
	m_text_CompleteCount.Init(12, textColor, bgx + 630, bgy + 308, 82, 21, TRUE);
	m_text_ItemCount.Init(12, textColor, bgx + 630, bgy + 336, 82, 21, TRUE);

	m_text_CompleteCount.SetString(100);
	m_text_ItemCount.SetString(100);

	g_pDisplay->CreateSurfaceFromBitmap(&m_surYut, "INTERFACE/YUTNORI/YUT_SEQ01.BMP");
	if( m_surYut )
	{
		m_surYut->SetColorKey(RGB(0, 0, 0));
	}	
	g_pDisplay->CreateSurfaceFromBitmap(&m_surResult, "INTERFACE/YUTNORI/YUT_SEQ02.BMP");
	if( m_surResult )
	{
		m_surResult->SetColorKey(RGB(0, 0, 0));
	}	

	int index = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			index = (i * 4) + j;
			if( index >= MAX_YUT_FRAME )
				break;

			m_rcYut[index].left = j * 256;
			m_rcYut[index].top = i * 256;
			m_rcYut[index].right = j * 256 + 256;
			m_rcYut[index].bottom = i * 256 + 256;
		}
	}

	index = 0;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			index = (i * 4) + j;
			if( index >= eYutResult_MAX )
				break;

			m_rcResult[index].left = j * 256;
			m_rcResult[index].top = i * 256;
			m_rcResult[index].right = j * 256 + 256;
			m_rcResult[index].bottom = i * 256 + 256;
		}
	}
}

void ControlYutboard::DeleteRes()
{
	if( YutManager::GetInstance()->GetYutnoriOn() == false  )
	{
		return;
	}

	m_background.DeleteRes();
	m_btn_close.DeleteRes();
	m_btn_start.DeleteRes();
	for(int i = 0; i < MAX_UNIT_TYPE; i++)
	{
		m_img_unit[i].DeleteRes();
	}

	SAFE_DELETE(m_surYut);
	SAFE_DELETE(m_surResult);
}

void ControlYutboard::Draw()
{
	if( !g_pDisplay )
		return;

	m_background.Draw();
	m_btn_start.Draw();
	m_btn_close.Draw();

	m_text_CompleteCount.Draw();
	m_text_ItemCount.Draw();

	updateAniYut();
	updateAniUnitMove();
	updateText();
}

LRESULT ControlYutboard::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_background.IsIn(x, y) == false )
				return 0;

			m_btn_close.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_start.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btn_close.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE, YUTBOARD);
				m_btn_close.SetState(BTN_NORMAL);
			}

			if( m_btn_start.GetState() == BTN_ACTION )
			{
				g_pTcpIp->SendNetMessage("yut_throw\n");
				m_btn_start.SetState(BTN_NORMAL);
				SetEnableStartBtn(false);
			}

			return 1;
		}
	}

	return 0;
}

void ControlYutboard::SetAniYutStart(int result)
{
	if( result < 0 || result >= eYutResult_MAX )
		return;

	m_aniYut = true;
	m_result = result;
	m_timeAniYutStart = GetTickCount();
}

void ControlYutboard::updateAniYut()
{
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	if( m_aniYut == false )
	{
		if( m_result != -1 )
		{
			g_pDisplay->Blt(bgx + 460, bgy, m_surResult, &m_rcResult[m_result]);
		}
		return;
	}

	DWORD dwSub = GetTickCount() - m_timeAniYutStart;
	if( dwSub > YUT_FRAME_TIME )
	{
		m_aniYut = false;
		SetAniUnitMoveStart();

		return;
	}
	int curFrame = MAX_YUT_FRAME * dwSub / YUT_FRAME_TIME;

	if( curFrame < 0 )
	{
		curFrame = 0;
	}
	else if( curFrame >= MAX_YUT_FRAME )
	{
		curFrame = MAX_YUT_FRAME - 1;
	}

	g_pDisplay->Blt(bgx + 460, bgy, m_surYut, &m_rcYut[curFrame]);
}

void ControlYutboard::SetAniUnitMoveStart()
{
	if( m_curPos == -1 )
		m_curPos = 0;

	YutManager* yutMgr = YutManager::GetInstance();
	int route = 0;
	if( m_result == eYutResult_Back )
	{
		if( m_curPos != 0 )
		{
			route = yutMgr->GetRoute(m_curPos, m_result);
			m_queRoute.push(route);
		}
		else
		{
			m_curPos = -1;
		}
	}
	else
	{
		for(int i = 0; i < m_result + 1; i++)
		{
			route = yutMgr->GetRoute(m_curPos, i);
			m_queRoute.push(route);
		}
	}

	m_aniUnitMove = true;
	m_timeAniUnitMoveStart = GetTickCount();
}

void ControlYutboard::updateAniUnitMove()
{
	YutManager* yutMgr = YutManager::GetInstance();
	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	if( m_aniUnitMove == true )
	{
		DWORD dwSub = GetTickCount() - m_timeAniUnitMoveStart;
		if( dwSub > UNIT_MOVE_TIME )
		{
			if( m_queRoute.empty() == true )
			{
				m_aniUnitMove = false;
				m_CompleteCount = yutMgr->GetCompleteCount();
				SetEnableStartBtn(true);
			}
			else
			{
				int route = m_queRoute.front();
				m_queRoute.pop();

				m_curPos = route;

				m_timeAniUnitMoveStart = GetTickCount();
			}
		}
	}

	if( m_curPos != -1 )
	{
		int complete = m_CompleteCount;
		complete %= 60;
		complete /= 10;

		POINT pt = yutMgr->GetUIPos(m_curPos);
		m_img_unit[complete].Draw(bgx + pt.x - 29, bgy + pt.y - 29);
	}
}

void ControlYutboard::updateText()
{
	const int yutItemVnum = 3732;
	int count = g_pRoh->GetCountInPack(yutItemVnum);
	m_text_ItemCount.SetString(count);

	m_text_CompleteCount.SetString(m_CompleteCount);
}

void ControlYutboard::SetCurPos(int pos)
{
	m_curPos = pos;
}

void ControlYutboard::SetEnableStartBtn(bool bEnable)
{
	m_btn_start.SetDisable2(!bEnable);
}

void ControlYutboard::SetCompleteCount(int completeCount)
{
	m_CompleteCount = completeCount;
}
