#include "stdafx.h"
#include "headers.h"
#include "Country.h"
#include "2DResMgr.h"
#include "s_member_control.h"
#include "Tcpipcon.h"
#include "CheckButton.h"
#include "NkCharacter.h"
#include <assert.h>



extern DWORD		g_dwClientCountry;




#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

#define		CLASS_IN_NAME	CMemberControl::CBunch

#define		BOUNDARY_MEMBER_LINE_POSY_TOP	  (m_nBaseY + 180)
#define		BOUNDARY_MEMBER_LINE_POSY_BOTTOM  (m_nBaseY + 418)

CMemberControl* CMemberControl::Create(int nBaseX, int nBaseY)
{
	return new CMemberControl(nBaseX, nBaseY);
}

CMemberControl::CMemberControl(int nBaseX, int nBaseY)
{
	// 변수초기화
	m_hWnd = NULL;

	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	m_pTitle  = NULL;
	m_pBBlack = NULL;

	m_listMember.clear();

	m_nSelectNum = -1;
	m_nTotalNum =  0;

	Init();
}

CMemberControl::~CMemberControl()
{
	DeleteRes();

	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pBBlack);

	vector<CLASS_IN_NAME*>::iterator _iter;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
	{
		CLASS_IN_NAME* pTemp = (*_iter);

		SAFE_DELETE(pTemp);
	}

	m_listMember.clear();
}

void CMemberControl::Init()
{
}

void CMemberControl::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);


	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pBBlack);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTitle , "interface/guild/guild_member/admin_enter.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBBlack, "interface/guild/guild_member/admin_member_bk.bmp");

	m_Scroll.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
	m_Scroll.SetRange(1);
	SetPosition();
}

void CMemberControl::Insert_list()
{
	if(g_pRoh)	return;

	CGuildData* pData = g_pRoh->GetMyGuildData();

	if(pData == NULL)	return;

	SetTotalNum(g_pRoh->GetMyGuildGeneralMemberCnt());

	list_guildmember::iterator _iter = pData->GetMemberInfo().begin();

	for(int i = 0; _iter != pData->GetMemberInfo().end(); _iter++, i++)
	{
		CLASS_IN_NAME* pTemp  = CLASS_IN_NAME::Create(m_nBaseX, m_nBaseY);

		m_nTotalNum = i;

		pTemp->Init();
		pTemp->LoadRes();
		pTemp->SetPosition(0, 0, i);
		pTemp->SetCharName(_iter->s_name);

		m_listMember[i] = pTemp;
	}
}

void CMemberControl::Insert_list(guild_member& g_member)
{
	if(g_member.s_glevel == -1 || g_member.s_glevel == 0 || g_member.s_glevel == JOB_MASTER)
		return;

	CLASS_IN_NAME* pTemp  = CLASS_IN_NAME::Create(m_nBaseX, m_nBaseY);

	pTemp->Init();
	pTemp->LoadRes();
	pTemp->SetPosition(0, 0, m_nTotalNum);
	pTemp->SetCharName(g_member.s_name);
	pTemp->SetCharIdx(g_member.s_index);

	m_Scroll.SetRange(m_nTotalNum*28);

	m_nTotalNum++;

	m_listMember.push_back(pTemp);
}

void CMemberControl::Refresh_list(guild_member& g_member)
{

}

void CMemberControl::SetTotalNum(int nGeneral)
{
	m_listMember.resize(nGeneral);
	m_Scroll.SetRange(nGeneral*28);
}



void CMemberControl::Restore()
{

}

void CMemberControl::SetPosition()
{
	m_pBBlack->Xpos = m_nBaseX + 27;
	m_pBBlack->Ypos = m_nBaseY + 181;

	m_pTitle->Xpos = m_pBBlack->Xpos + (m_pBBlack->GetWidth() - m_pTitle->GetWidth())*0.5;
	m_pTitle->Ypos = m_nBaseY + 142;

	m_Scroll.SetDrawPos(m_nBaseX+250, m_nBaseY+161, 270);
}

void CMemberControl::Draw()
{
	if( !g_pDisplay || !m_pTitle || !m_pBBlack)
		return;

	g_pDisplay->Blt(m_pTitle->Xpos , m_pTitle->Ypos , m_pTitle );

	if(m_listMember.empty() || m_nTotalNum <= 0)	return;

	g_pDisplay->Blt(m_pBBlack->Xpos, m_pBBlack->Ypos, m_pBBlack);

	vector<CLASS_IN_NAME*>::iterator _iter;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
	{
		CLASS_IN_NAME* pTemp = *_iter;

		if(pTemp)
			pTemp->Draw(m_Scroll.GetPos());
	}

	m_Scroll.Draw();

}

void CMemberControl::DeleteRes()
{
	m_Scroll.DeleteRes();

	vector<CLASS_IN_NAME*>::iterator _iter;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
	{
		CLASS_IN_NAME *pTemp = *_iter;

		if(pTemp)
			pTemp->DeleteRes();
	}
}

void CMemberControl::EraseData()
{
	if(m_nSelectNum == -1)	return;

	vector<CLASS_IN_NAME*>::iterator _iter_;

	_iter_ = m_listMember.begin() + m_nSelectNum;

	CBunch* pTemp = *_iter_;

	SAFE_DELETE(pTemp);

	m_listMember.erase(m_listMember.begin() + m_nSelectNum);

	m_nSelectNum = 0;

	for(_iter_ = m_listMember.begin(); _iter_ != m_listMember.end(); _iter_++, m_nSelectNum++)
	{
		CLASS_IN_NAME* pTemp  = (*_iter_);

		pTemp->SetRePosition(m_nSelectNum);

	}

	m_Scroll.SetRange(m_nTotalNum*28);

	m_nSelectNum = -1;

}

void CMemberControl::Delete_list(guild_member& g_member)
{
	vector<CLASS_IN_NAME*>::iterator _iter_;

	int i=0;
	for(_iter_ = m_listMember.begin(); _iter_ != m_listMember.end(); _iter_++, i++)
	{
		CLASS_IN_NAME* pTemp  = (*_iter_);

		if( pTemp->m_nCharidx == g_member.s_index)
		{
			SAFE_DELETE(pTemp);

			m_listMember.erase(_iter_);
			m_nTotalNum --;

			return;
		}
	}
}

void CMemberControl::SetListAllReset()
{
	// [6/22/2007 Theodoric] 우선 다 지운다.
	SetDataReset();

	//- 그리고 다시 하나씩 다 넣어보자.
	if(!g_pRoh)	return;

	CGuildData* pData = g_pRoh->GetMyGuildData();

	if(pData == NULL)
		return;

	list_guildmember::iterator _iter = pData->GetMemberInfo().begin();
	int GMindex = pData->GetGuildMasterChaIndex();

	for( ; _iter != pData->GetMemberInfo().end(); _iter++)
	{
		if( GMindex != _iter->s_index )
		{
			Insert_list( (*_iter) );
		}
	}
}

void CMemberControl::SetDataReset()
{
	vector<CLASS_IN_NAME*>::iterator _iter_;

	for(_iter_ = m_listMember.begin(); _iter_ != m_listMember.end(); _iter_++)
	{
		CLASS_IN_NAME* pTemp  = (*_iter_);

		SAFE_DELETE(pTemp);
	}

	m_listMember.clear();

	m_nTotalNum = 0;

	m_nSelectNum = -1;

	m_Scroll.SetRange(1);
}

LRESULT CMemberControl::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_Scroll.MsgProc(hWnd, msg, wParam, lParam);

	vector<CLASS_IN_NAME*>::iterator _iter;

	for(_iter = m_listMember.begin(); _iter != m_listMember.end(); _iter++)
	{
		CLASS_IN_NAME *pTemp = *_iter;

		if(pTemp && (pTemp->MsgProc(hWnd, msg, wParam, lParam, m_Scroll.GetPos()) != -1) )
		{
			m_nSelectNum = pTemp->m_nSequence;
		}
	}

	return 1;
}


BOOL CMemberControl::IsInside(int x, int y)
{
	return FALSE;
}

/*****************************************************************************************************************/
CLASS_IN_NAME*	CLASS_IN_NAME::Create(int nBaseX, int nBaseY)
{
	return new CBunch(nBaseX, nBaseY);
}

CLASS_IN_NAME::CBunch(int nBaseX, int nBaseY)
{
	m_hWnd		= NULL;

	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	m_nSequence   = 0;

	m_pCheckBt  = NULL;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CLASS_IN_NAME::~CBunch()
{
	DeleteRes();

	SAFE_DELETE_ARRAY(m_pCheckBt);
}

void CLASS_IN_NAME::Init()
{
	m_pCheckBt = CCheckbutton::Create(1);

	m_pCheckBt->SetFileName("guild/checkbox");

	m_BtFire.SetFileName("guild/secede_s");
}

void CLASS_IN_NAME::LoadRes()
{
	if(!g_pDisplay)	return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_BtFire.LoadRes();
	m_pCheckBt->LoadRes();

}

void CLASS_IN_NAME::Draw(int nScrollPos)
{
	if(!g_pDisplay)	return;

	m_TCharName.DrawClipText(m_TCharName.m_StringPosY, BOUNDARY_MEMBER_LINE_POSY_TOP, BOUNDARY_MEMBER_LINE_POSY_BOTTOM, nScrollPos);
	m_BtFire.DrawClip(BOUNDARY_MEMBER_LINE_POSY_TOP, BOUNDARY_MEMBER_LINE_POSY_BOTTOM, nScrollPos);

	m_pCheckBt->DrawClip(BOUNDARY_MEMBER_LINE_POSY_TOP, BOUNDARY_MEMBER_LINE_POSY_BOTTOM, nScrollPos, TRUE);

}

void CLASS_IN_NAME::DeleteRes()
{
	m_BtFire.DeleteRes();
	m_pCheckBt->DeleteRes();
	m_TCharName.DeleteRes();
}

void CLASS_IN_NAME::SetCharIdx( int chaindex)
{
	m_nCharidx = chaindex;
}

void CLASS_IN_NAME::SetCharName(char* pName)
{
	m_TCharName.SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CLASS_IN_NAME::SetPosition(int nPosX, int nPosY, int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_TCharName.Init(12, RGB(255, 255, 255), m_nBaseX + 38 , m_nBaseY + 183 + (24*nNum), 135, 25, TRUE);
	m_pCheckBt->SetPosition(m_nBaseX + 183, m_nBaseY + 183 + (24*nNum));	// 간격 24
	m_BtFire.SetPosition(m_nBaseX + 206, m_nBaseY + 182 + (24*nNum));
}

void CLASS_IN_NAME::SetRePosition(int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;
	m_TCharName.SetPos_Center(m_nBaseX + 38, m_nBaseY + 183 + (24*nNum));
	m_pCheckBt->SetPosition(m_nBaseX + 183, m_nBaseY + 183 + (24*nNum));	// 간격 24
	m_BtFire.SetPosition(m_nBaseX + 206, m_nBaseY + 182 + (24*nNum));
}

void CLASS_IN_NAME::Restore()
{

}

void CLASS_IN_NAME::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CLASS_IN_NAME::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll)
{
	m_BtFire.MsgProcClip(hWnd, msg, wParam, lParam, nScroll, BOUNDARY_MEMBER_LINE_POSY_TOP, BOUNDARY_MEMBER_LINE_POSY_BOTTOM);
	m_pCheckBt->MsgProcClip(hWnd, msg, wParam, lParam, nScroll, BOUNDARY_MEMBER_LINE_POSY_TOP, BOUNDARY_MEMBER_LINE_POSY_BOTTOM);


	switch(msg)
	{
	case WM_LBUTTONUP :
		{
			if(m_BtFire.GetState() == BTN_ACTION && m_pCheckBt->GetEnable())
			{
				sprintf(m_MsgBuf, "mg fire %s\n", m_TCharName.m_str);
				SendMsg();

				return 3;
			}
		}
		break;
	}

	return -1;
}
