#include "stdafx.h"
#include "headers.h"
#include "ControlGuildM.h"
#include "Country.h"
#include "2DResMgr.h"
#include "TextBoard.h"
#include "multilistbox.h"
#include "NkCharacter.h"

#include "s_joinrequest_control.h"
#include "s_grade_control.h"
#include "s_Job_control.h"
#include "s_notice_control.h"
#include "s_member_control.h"
#include "guild_data.h"



extern DWORD				g_dwClientCountry;





#define		_NONE			-1  // 처음의 상태
#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

extern LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam);
extern HHOOK g_hHook;


CControlGuildManage::CControlGuildManage()
{
	// 변수초기화
	m_hWnd = NULL;
	m_pBack = NULL;

	m_current_page = PAGE_NONE;

	m_pJoinWait   = NULL;
	m_pGradeName  = NULL;
	m_pJob		  = NULL;
	m_pNoticeInfo = NULL;
	m_pMemberlist = NULL;

	Init();
}

CControlGuildManage::~CControlGuildManage()
{
	DeleteRes();

	SAFE_DELETE(m_pJoinWait);
	SAFE_DELETE(m_pGradeName);
	SAFE_DELETE(m_pJob);
	SAFE_DELETE(m_pNoticeInfo);
	SAFE_DELETE(m_pMemberlist);

	m_pBack = NULL;
}

void CControlGuildManage::Init()
{
	m_Btclass.SetFileName("guild/admin_grade");			// 길드 계급명 변경
	m_BtNotice.SetFileName("guild/admin_notice");		// 길드 공지 관리
	m_BtJob.SetFileName("guild/admin_position");		// 길드 직책 관리
	m_BtMember.SetFileName("guild/admin_member");		// 길드 멤버 관리
	m_BtEntry.SetFileName("guild/admin_enter");			// 길드 가입 승인
	m_BtClose.SetFileName("guild/close");				// 닫기
}

void CControlGuildManage::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_pBack = NULL;

	SAFE_DELETE(m_pJoinWait);
	SAFE_DELETE(m_pGradeName);
	SAFE_DELETE(m_pJob);
	SAFE_DELETE(m_pNoticeInfo);
	SAFE_DELETE(m_pMemberlist);

	if(m_pBack == NULL)
	{
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFrom_guildback("guild_back_admin");

		if( m_pBack )
		{
			m_pBack->Xpos = m_ClientRc.right - m_pBack->GetWidth();
			m_pBack->Ypos = 29;

			m_Btclass.LoadRes();
			m_BtNotice.LoadRes();
			m_BtJob.LoadRes();
			m_BtMember.LoadRes();
			m_BtEntry.LoadRes();
			m_BtClose.LoadRes();

			m_pJoinWait = CJoinRequestControl::Create(m_pBack->Xpos, m_pBack->Ypos);
			m_pJoinWait->LoadRes();

			m_pGradeName = CGradeChangeControl::Create(m_pBack->Xpos, m_pBack->Ypos);
			m_pGradeName->LoadRes();

			m_pJob = CJobChangeControl::Create(m_pBack->Xpos, m_pBack->Ypos);
			m_pJob->LoadRes();

			m_pNoticeInfo = CNoticeControl::Create(m_pBack->Xpos, m_pBack->Ypos);
			m_pNoticeInfo->LoadRes();

			m_pMemberlist = CMemberControl::Create(m_pBack->Xpos, m_pBack->Ypos);
			m_pMemberlist->LoadRes();

			SetPosition();

		}
	}
}

void CControlGuildManage::Restore()
{

}

void CControlGuildManage::SetPosition()
{
	m_Btclass.SetPosition(m_pBack->Xpos + 36, m_pBack->Ypos + 38);
	m_BtNotice.SetPosition(m_pBack->Xpos + 150, m_pBack->Ypos + 38);
	m_BtJob.SetPosition(m_pBack->Xpos + 36, m_pBack->Ypos + 68);
	m_BtMember.SetPosition(m_pBack->Xpos + 150, m_pBack->Ypos + 68);
	m_BtEntry.SetPosition(m_pBack->Xpos + 92, m_pBack->Ypos + 99);
	m_BtClose.SetPosition(m_pBack->Xpos + 259, m_pBack->Ypos + 4);

	m_pJoinWait->SetPosition();
	m_pGradeName->SetPosition();
	m_pJob->SetPosition();
	m_pNoticeInfo->SetPosition();
	m_pMemberlist->SetPosition();
}

void CControlGuildManage::Draw()
{
	if( !g_pDisplay )
		return;

	if( m_pBack)
	{
		g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);

		m_Btclass.Draw();
		m_BtNotice.Draw();
		m_BtJob.Draw();
		m_BtMember.Draw();
		m_BtEntry.Draw(m_pBack->Xpos + (m_pBack->GetWidth()-m_BtEntry.Width)*0.5 ,m_pBack->Ypos + 99) ;

		m_BtClose.Draw();

		switch(m_current_page)
		{
		case PAGE_GRADE_CHANGE :
			if(m_pGradeName)
				m_pGradeName->Draw();
			break;
		case PAGE_NOTICE	   :
			if(m_pNoticeInfo)
				m_pNoticeInfo->Draw();
			break;
		case PAGE_JOB		   :
			if(m_pJob)
				m_pJob->Draw();
			break;
		case PAGE_MEMBER	   :
			if(m_pMemberlist)
				m_pMemberlist->Draw();
			break;
		case PAGE_REQUEST	   :
			if(m_pJoinWait)
				m_pJoinWait->Draw();
			break;
		case PAGE_NONE		   :
			break;
		}

	}

}

void CControlGuildManage::DeleteRes()
{
	m_Btclass.DeleteRes();
	m_BtNotice.DeleteRes();
	m_BtJob.DeleteRes();
	m_BtMember.DeleteRes();
	m_BtEntry.DeleteRes();
	m_BtClose.DeleteRes();

	if(m_pJoinWait)
		m_pJoinWait->DeleteRes();

	if(m_pGradeName)
		m_pGradeName->DeleteRes();

	if(m_pJob)
		m_pJob->DeleteRes();

	if(m_pNoticeInfo)
		m_pNoticeInfo->DeleteRes();

	if(m_pMemberlist)
		m_pMemberlist->DeleteRes();

}

BOOL CControlGuildManage::IsCheckBtnStatus()
{
	_eDrawPage tmpPage = PAGE_NONE;

	if(m_Btclass.GetState() == BTN_ACTION)
	{
		tmpPage = PAGE_GRADE_CHANGE;
	}
	else if(m_BtNotice.GetState() == BTN_ACTION)
	{
		tmpPage = PAGE_NOTICE;
	}
	else if(m_BtJob.GetState() == BTN_ACTION)
	{
		tmpPage = PAGE_JOB;
	}
	else if(m_BtMember.GetState() == BTN_ACTION)
	{
		tmpPage = PAGE_MEMBER;
	}
	else if(m_BtEntry.GetState() == BTN_ACTION)
	{
		tmpPage = PAGE_REQUEST;
	}
	else if(m_BtClose.GetState() == BTN_ACTION)
		return TRUE;

	if(m_current_page == tmpPage)
		return FALSE;

	return TRUE;
}

void CControlGuildManage::set_btn_status(BOOL bEnable)
{
	if(bEnable)
	{
		m_BtJob.SetDisable( false );
		m_BtJob.SetState( BTN_NORMAL);

		m_BtNotice.SetDisable( false );
		m_BtNotice.SetState( BTN_NORMAL );

		m_BtJob.SetDisable( false );
		m_BtJob.SetState( BTN_NORMAL );

		m_current_page = PAGE_MEMBER;
	}
	else
	{
		m_BtJob.SetDisable( true );
		m_BtJob.SetState( BTN_DISABLE );

		m_BtNotice.SetDisable( true );
		m_BtNotice.SetState( BTN_DISABLE );

		m_BtJob.SetDisable( true );
		m_BtJob.SetState( BTN_DISABLE );
	}
}

void CControlGuildManage::ResetNotice(char *szNotice)
{
	m_pNoticeInfo->SetNotice(szNotice);
}

void CControlGuildManage::ResetGrade(char *szgrade, int nLevel)
{
	m_pGradeName->SetGradeName(szgrade, nLevel);
}

void CControlGuildManage::ResetJoinRequest(int charindex)
{
	m_pJoinWait->EraseData(charindex);
}

void CControlGuildManage::ResetMember()
{
	m_pMemberlist->EraseData();
}

void CControlGuildManage::all_data_Reset()
{
	if(m_pNoticeInfo)
		m_pNoticeInfo->cleartext();

	if(m_pJoinWait)
		m_pJoinWait->SetDataReset();

	if(m_pMemberlist)
		m_pMemberlist->SetDataReset();
	if(m_pJob)
		m_pJob->Reset();

	set_btn_status(false);
}

LRESULT CControlGuildManage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(m_current_page)
	{
	case PAGE_GRADE_CHANGE :
		if(m_pGradeName)
		{
			if(m_pGradeName->MsgProc(hWnd, msg, wParam, lParam) == 3)
				return 1;
		}
		break;
	case PAGE_NOTICE	   :
		if(m_pNoticeInfo)
		{
			if(m_pNoticeInfo->MsgProc(hWnd, msg, wParam, lParam) == 3)
				return 1;
		}
		break;
	case PAGE_JOB		   :
		if(m_pJob)
		{
			if(m_pJob->MsgProc(hWnd, msg, wParam, lParam) == 3)
				return 1;
		}
		break;
	case PAGE_MEMBER	   :
		if(m_pMemberlist)
			m_pMemberlist->MsgProc(hWnd, msg, wParam, lParam);
		break;
	case PAGE_REQUEST	   :
		if(m_pJoinWait)
			m_pJoinWait->MsgProc(hWnd, msg, wParam, lParam);
		break;
	case PAGE_NONE		   :
		break;
	}

	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return 0;


	m_Btclass.MsgProc(hWnd, msg, wParam, lParam);
	m_BtNotice.MsgProc(hWnd, msg, wParam, lParam);
	m_BtJob.MsgProc(hWnd, msg, wParam, lParam);
	m_BtMember.MsgProc(hWnd, msg, wParam, lParam);
	m_BtEntry.MsgProc(hWnd, msg, wParam, lParam);
	m_BtClose.MsgProc(hWnd, msg, wParam, lParam);

	switch ( msg )
	{
	case WM_LBUTTONUP:
		{
			if(!IsCheckBtnStatus())
				return 1;

			g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, NULL, GetCurrentThreadId());

			if(m_Btclass.GetState() == BTN_ACTION)
			{
				CGuildData* pData = g_pRoh->GetMyGuildData();

				m_pGradeName->SetGradeName(pData->GetMyGuildInfo().s_guildjobname[JOB_MASTER], JOB_MASTER);
				m_pGradeName->SetGradeName(pData->GetMyGuildInfo().s_guildjobname[JOB_ASSISTANTMASTER], JOB_ASSISTANTMASTER);

				m_pGradeName->SetGradeName(pData->GetMyGuildInfo().s_guildjobname[JOB_HIV], JOB_HIV);

				m_pGradeName->SetGradeName(pData->GetMyGuildInfo().s_guildjobname[JOB_GENERIC], JOB_GENERIC);

				m_current_page = PAGE_GRADE_CHANGE;
			}
			else if(m_BtNotice.GetState() == BTN_ACTION)
			{
				CGuildData* pData = g_pRoh->GetMyGuildData();

				if( pData )
				{
					m_pNoticeInfo->SetNotice(pData->GetMyNoticeInfo().s_guildnotic);
					m_current_page = PAGE_NOTICE;
				}
			}
			else if(m_BtJob.GetState() == BTN_ACTION)
			{
				m_pJob->Insert_list();

				m_current_page = PAGE_JOB;
			}
			else if(m_BtMember.GetState() == BTN_ACTION)
			{
				m_current_page = PAGE_MEMBER;
			}
			else if(m_BtEntry.GetState() == BTN_ACTION)
			{
				m_current_page = PAGE_REQUEST;
			}
			else if(m_BtClose.GetState() == BTN_ACTION)
			{
				m_current_page = PAGE_NONE;

				g_pNk2DFrame->ShowControlGuildManage(FALSE);
				m_BtClose.SetState(BTN_NORMAL);
			}
		}
		return 1;
	}


	return 1;
}

void CControlGuildManage::HookClose()
{
	UnhookWindowsHookEx(g_hHook);
}

BOOL CControlGuildManage::IsInside(int x, int y)
{
	if(!m_pBack)
		return FALSE;

	if( x >= m_pBack->Xpos
			&& x < m_pBack->Xpos + m_pBack->GetWidth()
			&& y >= m_pBack->Ypos
			&& y < m_pBack->Ypos + m_pBack->GetHeight() )
		return TRUE;

	return FALSE;
}
