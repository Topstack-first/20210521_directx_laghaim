#include "stdafx.h"
#include "headers.h"
#include "s_Job_control.h"
#include "Country.h"
#include "2DResMgr.h"
#include "Tcpipcon.h"
#include "NkCharacter.h"
#include "guild_data.h"
#include "g_stringmanager.h"



extern DWORD		g_dwClientCountry;





#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

#define		BOUNDARY_JOB_LINE_POSY_TOP	   (m_nBaseY + 163)
#define		BOUNDARY_JOB_LINE_POSY_BOTTOM  (m_nBaseY + 435)

CJobChangeControl* CJobChangeControl::Create(int nBaseX, int nBaseY)
{
	return new CJobChangeControl(nBaseX, nBaseY);
}

CJobChangeControl::CJobChangeControl(int nBaseX, int nBaseY)
{
	// 변수초기화
	m_hWnd = NULL;

	m_pTitle = NULL;

	for(int i = 0; i < TOTAL; i++)
		m_pName[i] = NULL;

	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	Init();
}

CJobChangeControl::~CJobChangeControl()
{
	DeleteRes();

	for(int i = 0; i < TOTAL; i++)
		SAFE_DELETE(m_pName[i]);

	SAFE_DELETE(m_pTitle);
}

void CJobChangeControl::Init()
{
	m_BtAppoint[0].SetFileName("guild/commission");
	for(int i = 1; i < TOTAL; i++)
		m_BtAppoint[i].SetFileName("guild/appoint");
}

void CJobChangeControl::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);


	g_pDisplay->CreateSurfaceFromBitmap(&m_pTitle, "interface/guild/guild_job/admin_position.bmp");

	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[MASTER], "interface/guild/guild_job/pos_master_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[ASSIST], "interface/guild/guild_job/pos_submaster_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[VULCAN], "interface/guild/guild_job/pos_bulkan_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[HUMAN] , "interface/guild/guild_job/pos_human_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[AIDA]  , "interface/guild/guild_job/pos_aidia_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[KALIP] , "interface/guild/guild_job/pos_kailipton_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[HYBRID], "interface/guild/guild_job/pos_hybrider_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pName[PEROM], "interface/guild/guild_job/POS_PEROM_BK.bmp");

//===============================================================
// [4/2/2009 D.K ] : RPS. job code error.
// 		m_BtSave.LoadRes();
// 		m_BtCancel.LoadRes();
//===============================================================
	for(int i = 0; i < TOTAL; i++)
	{
		m_BtAppoint[i].LoadRes();

		m_Text[i].Init(m_nBaseX, m_nBaseY, 20, RGB(255,255,255), RGB(255, 179, 0), FALSE, MSG_POPUP);		
		m_Text[i].SetfontSize();
	}

	m_Scroll.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
	m_Scroll.SetRange(1);

	SetPosition();
}

void CJobChangeControl::Restore()
{

}

void CJobChangeControl::SetPosition()
{
	m_pTitle->Xpos = m_nBaseX + 23;
	m_pTitle->Ypos = m_nBaseY + 142;
	for(int i = 0, nHeight = 0; i < TOTAL; i++, nHeight+=52)
	{
		m_BtAppoint[i].SetPosition(m_nBaseX + 202, m_nBaseY + 194 + nHeight);
		m_Text[i].SetPos(m_nBaseX + 46, m_nBaseY + 194 + nHeight + 8);
		m_pName[i]->Xpos = m_nBaseX + 16;
		m_pName[i]->Ypos = m_nBaseY + 174 + nHeight;
	}


	m_Scroll.SetDrawPos(m_nBaseX+250, m_nBaseY+161, 270);
}

void CJobChangeControl::Draw()
{
	if( !g_pDisplay || (m_nBaseX ==0 && m_nBaseY == 0))
		return;

	g_pDisplay->Blt(m_pTitle->Xpos, m_pTitle->Ypos, m_pTitle);

	for(int i = 0; i < TOTAL; i++)
	{
		DrawToBlt(m_pName[i]);
		m_BtAppoint[i].DrawClip(BOUNDARY_JOB_LINE_POSY_TOP, BOUNDARY_JOB_LINE_POSY_BOTTOM, m_Scroll.GetPos());

		m_Text[i].DrawToClip(BOUNDARY_JOB_LINE_POSY_TOP, BOUNDARY_JOB_LINE_POSY_BOTTOM, m_Scroll.GetPos());

	}

	m_Scroll.SetRange(7*51);
	m_Scroll.Draw();
}

void CJobChangeControl::DeleteRes()
{
	for(int i = 0; i < TOTAL; i++)
	{
		m_BtAppoint[i].DeleteRes();
		m_Text[i].DeleteRes();
	}

	m_Scroll.DeleteRes();


}

BOOL CJobChangeControl::DrawToBlt(CSurface* pSurface)
{
	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = pSurface->GetWidth();
	src.bottom = pSurface->GetHeight();

	dest.left   = pSurface->Xpos;
	dest.top    = pSurface->Ypos;
	dest.right  = pSurface->Xpos + pSurface->GetWidth();
	dest.bottom = pSurface->Ypos + pSurface->GetHeight();


	if(dest.top - m_Scroll.GetPos() <= BOUNDARY_JOB_LINE_POSY_TOP &&
			dest.bottom - m_Scroll.GetPos() <= BOUNDARY_JOB_LINE_POSY_TOP)	// 위쪽  밖에 있음..
	{
		return FALSE;
	}
	else if(dest.top - m_Scroll.GetPos() <= BOUNDARY_JOB_LINE_POSY_TOP &&		// 윗 영역에 걸쳤음.
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_JOB_LINE_POSY_TOP)
	{

		src.top  = abs((dest.top - m_Scroll.GetPos()) - BOUNDARY_JOB_LINE_POSY_TOP );
		dest.top = BOUNDARY_JOB_LINE_POSY_TOP;
		dest.bottom = (dest.bottom - m_Scroll.GetPos());
	}
	else if(dest.top - m_Scroll.GetPos() >= BOUNDARY_JOB_LINE_POSY_TOP &&		// 모두 영역 안에 있음.
			dest.bottom - m_Scroll.GetPos() <= BOUNDARY_JOB_LINE_POSY_BOTTOM/*BOUNDARY_JOB_LINE_POSY_TOP*/)
	{
		dest.top    = pSurface->Ypos - m_Scroll.GetPos();
		dest.bottom = dest.top + pSurface->GetHeight();

	}
	else if(dest.top - m_Scroll.GetPos() <= BOUNDARY_JOB_LINE_POSY_BOTTOM &&
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_JOB_LINE_POSY_BOTTOM)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( BOUNDARY_JOB_LINE_POSY_BOTTOM - (dest.top - m_Scroll.GetPos()) );
		dest.top    = (dest.top- m_Scroll.GetPos());
		dest.bottom = BOUNDARY_JOB_LINE_POSY_BOTTOM;
	}
	else if(dest.top - m_Scroll.GetPos() >= BOUNDARY_JOB_LINE_POSY_BOTTOM &&
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_JOB_LINE_POSY_BOTTOM) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}

	g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, pSurface, &src);


	return TRUE;
}

void CJobChangeControl::Reset()
{
	for(int i = 0; i < TOTAL ; i++)
		m_Text[i].ClearText();
}

void CJobChangeControl::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CJobChangeControl::Insert_list()
{
	if(g_pRoh == NULL)	return;

	CGuildData* pData = g_pRoh->GetMyGuildData();

	if(pData == NULL) return;

	Reset();

	list_guildmember::iterator _iter_ = pData->GetMemberInfo().begin();

	for(; _iter_ != pData->GetMemberInfo().end(); _iter_++)
	{
		switch(_iter_->s_glevel)
		{
		case JOB_PEROM :
			{
				m_Text[PEROM].ClearText();
				m_Text[PEROM].SetText(_iter_->s_name);
			}
			break;
		case JOB_HIV :
			{
				m_Text[HYBRID].ClearText();
				m_Text[HYBRID].SetText(_iter_->s_name);
			}
			break;
		case JOB_HUMAN :
			{
				m_Text[HUMAN].ClearText();
				m_Text[HUMAN].SetText(_iter_->s_name);
			}
			break;
		case JOB_EIDA :
			{
				m_Text[AIDA].ClearText();
				m_Text[AIDA].SetText(_iter_->s_name);
			}
			break;
		case JOB_KAI:
			{
				m_Text[KALIP].ClearText();
				m_Text[KALIP].SetText(_iter_->s_name);
			}
			break;
		case JOB_VULCAN:
			{
				m_Text[VULCAN].ClearText();
				m_Text[VULCAN].SetText(_iter_->s_name);
			}
			break;
		case JOB_ASSISTANTMASTER :
			{
				m_Text[ASSIST].ClearText();
				m_Text[ASSIST].SetText(_iter_->s_name);
			}
			break;
		case JOB_MASTER :
			{
				m_Text[MASTER].ClearText();
				m_Text[MASTER].SetText(_iter_->s_name);
			}
			break;
		}
	}
}



LRESULT CJobChangeControl::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_Scroll.MsgProc(hWnd, msg, wParam, lParam);

	int res = 0;

	for(int i = 0; i < TOTAL; i++)
	{
		m_BtAppoint[i].MsgProcClip(hWnd, msg, wParam, lParam, m_Scroll.GetPos(), BOUNDARY_JOB_LINE_POSY_TOP, BOUNDARY_JOB_LINE_POSY_BOTTOM);
		if(m_Text[i].MsgProcClip(hWnd, msg, wParam, lParam, m_Scroll.GetPos(), BOUNDARY_JOB_LINE_POSY_TOP, BOUNDARY_JOB_LINE_POSY_BOTTOM) != E_FAIL)
			res = 3;
	}

	if( res == 3 )
		return res;

	switch(msg)
	{
	case WM_LBUTTONUP :
		{
			if(m_BtAppoint[MASTER].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[MASTER].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_MASTER, m_Text[MASTER].GetStrBuf());
					SendMsg();
				}
				else // IDS_GUILD_JOBNAME_FAIL_STRLEN	"글자수가 초과 되었습니다. (한글 8자 - 영문 16)"
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_JOBNAME_FAIL_STRLEN), TYPE_GUILD_CONFIRM, 42);

			}

			if(m_BtAppoint[ASSIST].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[ASSIST].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_ASSISTANTMASTER, m_Text[ASSIST].GetStrBuf());
					SendMsg();
				}
				else
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_GENERIC, m_Text[ASSIST].GetStrBuf());
					SendMsg();
				}
			}

			if(m_BtAppoint[VULCAN].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[VULCAN].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_VULCAN, m_Text[VULCAN].GetStrBuf());
					SendMsg();
				}
				else
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_GENERIC, m_Text[VULCAN].GetStrBuf());
					SendMsg();
				}

			}

			if(m_BtAppoint[HUMAN].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[HUMAN].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_HUMAN, m_Text[HUMAN].GetStrBuf());
					SendMsg();
				}
				else
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_GENERIC, m_Text[HUMAN].GetStrBuf());
					SendMsg();
				}

			}

			if(m_BtAppoint[AIDA].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[AIDA].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_EIDA, m_Text[AIDA].GetStrBuf());
					SendMsg();
				}
				else
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_GENERIC, m_Text[AIDA].GetStrBuf());
					SendMsg();
				}

			}

			if(m_BtAppoint[KALIP].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[KALIP].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_KAI, m_Text[KALIP].GetStrBuf());
					SendMsg();
				}
				else
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_GENERIC, m_Text[KALIP].GetStrBuf());
					SendMsg();
				}

			}

			if(m_BtAppoint[HYBRID].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[HYBRID].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_HIV, m_Text[HYBRID].GetStrBuf());
					SendMsg();
				}
				else
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_GENERIC, m_Text[HYBRID].GetStrBuf());
					SendMsg();
				}
			}

			if(m_BtAppoint[PEROM].GetState() == BTN_ACTION)
			{
				if(strlen(m_Text[PEROM].GetStrBuf()) <= 16)
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_PEROM, m_Text[PEROM].GetStrBuf());
					SendMsg();
				}
				else
				{
					sprintf(m_MsgBuf,"mg appoint %d %s\n", JOB_GENERIC, m_Text[PEROM].GetStrBuf());
					SendMsg();
				}
			}
		}
		break;
	}

	return 1;
}


BOOL CJobChangeControl::IsInside(int x, int y)
{

	return FALSE;
}


BOOL CJobChangeControl::IsFocus()
{
	for(int i=0 ; i<TOTAL; i++)
	{
		if(m_Text[i].GetEnable() )
			return true;
	}

	return false;
}
