#include "stdafx.h"
#include "ControlLetterRecv.h"
#include "Nk2DFrame.h"
#include "PacketSend.h"
#include "LetterManager.h"
#include "ControlLetterWindow.h"
#include "ControlLetterSend.h"

ControlLetterRecv::ControlLetterRecv()
	: m_pLetterBox(NULL)
	, m_isLock(false)
	, m_index(-1)
	, m_type(0)
{
}

ControlLetterRecv::~ControlLetterRecv()
{
	DeleteRes();
}

HRESULT ControlLetterRecv::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

void ControlLetterRecv::LoadRes()
{
	m_background.Init("LETTER/BG_RECV_MSG");
	m_btnClose.Init("common/btn_close_01");
	m_btnOK.Init("LETTER/BTN_OK");
	m_btnDel.Init("LETTER/BTN_DEL");
	m_btnRe.Init("LETTER/BTN_RE");

	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btnClose.SetPosition(bgx + 346, bgy + 6);
	m_btnOK.SetPosition(bgx + 45, bgy + 343);
	m_btnDel.SetPosition(bgx + 235, bgy + 343);
	m_btnRe.SetPosition(bgx + 140, bgy + 343);
	m_tbTitle.Init(bgx + 84, bgy + 46, 36, RGB(175,140,93), RGB(255,255,255));
	m_tbTitle.SetfontSize();
	m_tbName.Init(bgx + 84, bgy + 121, 40, RGB(175,140,93), RGB(255,255,255));
	m_tbName.SetfontSize();

	m_pLetterBox = new CLetterBox();
	m_pLetterBox->Init_MEBox(false, 9, 34);
	m_pLetterBox->Init(0, 0, 30, RGB(175,140,93), RGB(255,255,255));
	m_pLetterBox->SetPos(bgx + 84, bgy + 162);
	m_pLetterBox->SetMaxLen(400);
	m_pLetterBox->SetFontSize(12);
	m_pLetterBox->SetEnable(false);
}

void ControlLetterRecv::DeleteRes()
{
	m_background.DeleteRes();
	m_btnClose.DeleteRes();
	m_btnOK.DeleteRes();
	m_btnDel.DeleteRes();
	m_btnRe.DeleteRes();
	m_tbTitle.DeleteRes();
	m_tbName.DeleteRes();

	if( m_pLetterBox )
		m_pLetterBox->DeleteRes();
	SAFE_DELETE(m_pLetterBox);
}

float ControlLetterRecv::Update()
{
	DWORD timeCur = GetTickCount();
	DWORD timeElapsed = timeCur - m_time;
	m_time = timeCur;

	float fElapsed = (float)(timeElapsed) / 1000.0f;
	m_timeAccum += fElapsed;

	return 0;
}

void ControlLetterRecv::Draw()
{
	m_background.Draw();
	m_btnClose.Draw();
	m_btnOK.Draw();
	m_btnDel.Draw();
	m_btnRe.Draw();
	m_tbTitle.Draw();
	m_tbName.Draw();

	if( m_pLetterBox )
		m_pLetterBox->Draw();
}

void ControlLetterRecv::Refresh(int index, int type)
{
	m_index = index;
	m_type = type;

	LetterContent cont = LetterManager::GetInstance()->GetContent(m_type, index);

	m_tbTitle.ClearText();
	m_tbName.ClearText();	

	m_tbTitle.SetText((char*)cont.title.c_str());
	m_tbName.SetText((char*)cont.name.c_str());
	m_pLetterBox->m_MEBox.Set_CoreString((char*)cont.content.c_str());
}

void ControlLetterRecv::ActionClose()
{
	ActionOK();
}

void ControlLetterRecv::ActionOK()
{
	SetLock(false);

	g_pNk2DFrame->SetVisibleInterface(false, LETTER_RECV);
	((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->SetLock(false);
}

void ControlLetterRecv::ActionDel()
{
	if( m_index == -1 )
		return;

	SetLock(true);

	SendNoteMessageDel(m_type, m_index);
}

void ControlLetterRecv::ActionRe()
{
	SetLock(false);

	g_pNk2DFrame->SetVisibleInterface(false, LETTER_RECV);
	((ControlLetterSend*)g_pNk2DFrame->GetInterface(LETTER_SEND))->ClearText();
	((ControlLetterSend*)g_pNk2DFrame->GetInterface(LETTER_SEND))->SetName(m_tbName.GetString());
	((ControlLetterSend*)g_pNk2DFrame->GetInterface(LETTER_SEND))->SetLock(false);
	g_pNk2DFrame->SetVisibleInterface(true, LETTER_SEND);
}

LRESULT ControlLetterRecv::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			int x = LOWORD (lParam);
			int y = HIWORD (lParam);

			if( m_background.IsIn(x, y) == false )
				return 0;

			m_btnClose.MsgProc(hWnd,msg, wParam, lParam);
			m_btnOK.MsgProc(hWnd, msg, wParam, lParam);
			m_btnDel.MsgProc(hWnd, msg, wParam, lParam);
			m_btnRe.MsgProc(hWnd, msg, wParam, lParam);

			if( m_btnClose.GetState() == BTN_ACTION )
			{
				m_btnClose.SetState(BTN_NORMAL);
				ActionClose();

				return 1;
			}

			if( m_btnOK.GetState() == BTN_ACTION )
			{
				m_btnOK.SetState(BTN_NORMAL);
				ActionOK();

				return 1;
			}
			
			if( m_btnDel.GetState() == BTN_ACTION )
			{
				m_btnDel.SetState(BTN_NORMAL);
				ActionDel();

				return 1;
			}

			if( m_btnRe.GetState() == BTN_ACTION )
			{
				m_btnRe.SetState(BTN_NORMAL);
				ActionRe();

				return 1;
			}

			return 1;
		}
		break;
	}
	return 0;
}
