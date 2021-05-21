#include "stdafx.h"
#include "headers.h"
#include "s_joinrequest_control.h"
#include "Country.h"
#include "2DResMgr.h"
#include "Tcpipcon.h"
#include "NkCharacter.h"
#include "guild_data.h"
#include <assert.h>
#include <time.h>
#include "g_stringmanager.h"




extern DWORD		g_dwClientCountry;






#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

CJoinRequestControl* CJoinRequestControl::Create(int nBaseX, int nBaseY)
{
	return new CJoinRequestControl(nBaseX, nBaseY);
}

CJoinRequestControl::CJoinRequestControl(int nBaseX, int nBaseY)
{
	// 변수초기화
	m_hWnd = NULL;

	m_nBaseX	=	nBaseX;
	m_nBaseY	=	nBaseY;

	m_pTitle		= NULL;
	m_pRecommanBox	= NULL;
	m_pRequestBox	= NULL;

	m_nSelectType	= -1;	// 0번 : 추천, 1번 : 일반
	m_nGSelectNum	= -1;	// 선택된것
	m_nRSelectNum   = -1;

	m_R_Waitlist.clear();
	m_G_Waitlist.clear();

	m_nRCnt = 0;
	m_nGCnt = 0;

	Init();
}

CJoinRequestControl::~CJoinRequestControl()
{
	DeleteRes();

	vector<CBunch*>::iterator _iter;

	for(_iter = m_R_Waitlist.begin(); _iter != m_R_Waitlist.end(); _iter++)
	{
		CBunch* pTemp = (*_iter);

		SAFE_DELETE(pTemp);
	}

	for(_iter = m_G_Waitlist.begin(); _iter != m_G_Waitlist.end(); _iter++)
	{
		CBunch* pTemp = (*_iter);
		SAFE_DELETE(pTemp);
	}

	m_R_Waitlist.clear();
	m_G_Waitlist.clear();

	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pRecommanBox);
	SAFE_DELETE(m_pRequestBox);

}

void CJoinRequestControl::Init()
{
}

void CJoinRequestControl::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pRecommanBox);
	SAFE_DELETE(m_pRequestBox);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTitle, "interface/guild/guild_entry/admin_enter.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pRecommanBox, "interface/guild/guild_entry/recommendlist_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pRequestBox, "interface/guild/guild_entry/requestdlist_bk.bmp");

	m_R_Scroll.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
	m_R_Scroll.SetRange(1);

	m_G_Scroll.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
	m_G_Scroll.SetRange(1);

	SetPosition();
}

void CJoinRequestControl::Insert_list()
{
	if(g_pRoh == NULL)	return;

	CGuildData* pData = g_pRoh->GetMyGuildData();

	if(pData == NULL)	return;

	SetDataReset();

	int nRecommandCnt = pData->GetGuildinfo().s_recomwaitmember;
	int nGeneralCnt   = pData->GetGuildinfo().s_generalwaitmember;

	SetTotalNum(nRecommandCnt, nGeneralCnt);

	list_guildmember::iterator _Iter_re = pData->GetRecomWaitInfo().begin();
	list_guildmember::iterator _Iter_ge = pData->GetGeneralWaitInfo().begin();

	for(int i = 0; _Iter_re != pData->GetRecomWaitInfo().end(); _Iter_re++, i++)
	{
		CBunch* pTemp  = CBunch::Create(m_nBaseX, m_nBaseY, m_nBaseY+190, m_nBaseY+295);

		pTemp->Init();
		pTemp->LoadRes();
		pTemp->SetPosition(0, 0, i);
		pTemp->SetCharName(_Iter_re->s_name);
		pTemp->SetTime(_Iter_re->s_waittime);

		m_R_Waitlist[i] = pTemp;
	}

	for(int i = 0; _Iter_ge != pData->GetGeneralWaitInfo().end(); _Iter_ge++, i++)
	{
		CBunch* pTemp  = CBunch::Create(m_nBaseX, m_nBaseY, m_nBaseY+190, m_nBaseY+295);

		pTemp->Init();
		pTemp->LoadRes();
		pTemp->SetPosition(0, 0, i);
		pTemp->SetCharName(_Iter_ge->s_name);
		pTemp->SetTime(_Iter_ge->s_waittime);

		m_G_Waitlist[i] = pTemp;
	}
}

void CJoinRequestControl::Insert_list(guild_member& g_member)
{
	if(g_member.s_glevel == 0 && g_member.s_jtype == 1)	// 추천
	{
		CBunch* pTemp  = CBunch::Create(m_nBaseX, m_nBaseY, m_nBaseY+190, m_nBaseY+295);

		pTemp->Init();
		pTemp->LoadRes();
		pTemp->SetPosition(0, 0, m_nRCnt);
		pTemp->SetCharName(g_member.s_name);
		pTemp->SetTime(g_member.s_waittime);
		pTemp->SetCharIdx(g_member.s_index);

		m_R_Scroll.SetRange(m_nRCnt*55);

		m_nRCnt++;

		m_R_Waitlist.push_back(pTemp);
	}

	if(g_member.s_glevel == 0 && g_member.s_jtype == 2) // 일반
	{
		CBunch* pTemp  = CBunch::Create(m_nBaseX, m_nBaseY, m_nBaseY+325, m_nBaseY+432);

		pTemp->Init();
		pTemp->LoadRes();
		pTemp->SetPosition(0, 139, m_nGCnt);
		pTemp->SetCharName(g_member.s_name);
		pTemp->SetTime(g_member.s_waittime);
		pTemp->SetCharIdx(g_member.s_index);

		m_G_Scroll.SetRange(m_nGCnt*55);

		m_nGCnt++;

		m_G_Waitlist.push_back(pTemp);
	}
}

void CJoinRequestControl::EraseData(int charindex )
{

	bool bRePosition = false;
	int i=0;

	vector<CBunch*>::iterator _iter_;

	for(_iter_ = m_R_Waitlist.begin(); _iter_<m_R_Waitlist.end(); _iter_++)
	{
		CBunch* pTemp = (*_iter_);

		if(pTemp->m_nCharIdx == charindex )
		{
			SAFE_DELETE(pTemp);
			m_R_Waitlist.erase(_iter_);
			bRePosition = true;
			break;
		}
	}

	if(bRePosition)
	{
		for(_iter_ = m_R_Waitlist.begin(); _iter_ != m_R_Waitlist.end(); _iter_++, i++)
		{
			CBunch* pTemp  = (*_iter_);

			if(pTemp)
				pTemp->SetRePosition(i, 0);

		}

		m_R_Scroll.SetRange(m_R_Waitlist.size()*28);
		m_nRSelectNum = -1;
		m_nSelectType = -1;
		m_nRCnt--;
	}


	bRePosition = false;
	i = 0;

	for(_iter_ = m_G_Waitlist.begin(); _iter_<m_G_Waitlist.end(); _iter_++)
	{
		CBunch* pTemp = (*_iter_);

		if(pTemp->m_nCharIdx == charindex )
		{
			SAFE_DELETE(pTemp);
			m_G_Waitlist.erase(_iter_);
			bRePosition = true;
			break;
		}
	}

	if(bRePosition)
	{
		for(_iter_ = m_G_Waitlist.begin(); _iter_ != m_G_Waitlist.end(); _iter_++, i++)
		{
			CBunch* pTemp  = (*_iter_);

			if(pTemp)
				pTemp->SetRePosition(i, 135);

		}
		m_G_Scroll.SetRange(m_G_Waitlist.size()*55);
		m_nGSelectNum = -1;
		m_nSelectType = -1;
		m_nGCnt--;
	}
}


void CJoinRequestControl::EraseData()
{
	if(m_nSelectType == -1)
		return;

	if(m_nRSelectNum == -1 && m_nGSelectNum == -1)
		return;

	if(m_nSelectType == 1)		// 추천
	{
		vector<CBunch*>::iterator _iter_;

		int i = 0;

		//////////////////////////////////////////////////
		_iter_ = m_R_Waitlist.begin() + m_nRSelectNum;

		CBunch* pTemp = (*_iter_);

		SAFE_DELETE(pTemp);
		//////////////////////////////////////////////////

		m_R_Waitlist.erase(m_R_Waitlist.begin() + m_nRSelectNum);

		for(_iter_ = m_R_Waitlist.begin(); _iter_ != m_R_Waitlist.end(); _iter_++, i++)
		{
			CBunch* pTemp  = (*_iter_);

			if(pTemp)
				pTemp->SetRePosition(i, 0);

		}

		m_R_Scroll.SetRange(m_R_Waitlist.size()*28);

		m_nRSelectNum = -1;

		m_nSelectType = -1;

		m_nRCnt--;
	}
	else if(m_nSelectType == 2)		// 일반
	{
		vector<CBunch*>::iterator _iter_;

		int i = 0;

		_iter_ = m_G_Waitlist.begin() + m_nGSelectNum;

		CBunch* pTemp = (*_iter_);

		SAFE_DELETE(pTemp);

		m_G_Waitlist.erase(m_G_Waitlist.begin() + m_nGSelectNum);

		for(_iter_ = m_G_Waitlist.begin(); _iter_ != m_G_Waitlist.end(); _iter_++, i++)
		{
			CBunch* pTemp  = (*_iter_);

			if(pTemp)
				pTemp->SetRePosition(i, 135);

		}

		m_G_Scroll.SetRange(m_G_Waitlist.size()*55);

		m_nGSelectNum = -1;

		m_nSelectType = -1;

		m_nGCnt--;
	}
}

int CJoinRequestControl::IsExist(int nCharIdx, int type)
{
	vector<CBunch*>::iterator _iter_;

	if(type == 1)	// 추천
	{
		for(_iter_ = m_R_Waitlist.begin() ; _iter_ != m_R_Waitlist.end(); _iter_)
		{
			CBunch* pTemp = NULL;

			pTemp = *_iter_;

			if(pTemp && pTemp->m_nCharIdx == nCharIdx)
			{
				return 1;
			}
		}
	}

	if(type == 2)	// 일반
	{
		for(_iter_ = m_G_Waitlist.begin() ; _iter_ != m_G_Waitlist.end(); _iter_)
		{
			CBunch* pTemp = NULL;

			pTemp = *_iter_;

			if(pTemp && pTemp->m_nCharIdx == nCharIdx)
			{
				return 2;
			}
		}
	}

	return 0;
}

void CJoinRequestControl::SetDataReset()
{
	vector<CBunch*>::iterator _iter;

	int nSize = m_R_Waitlist.size();

	_iter = m_R_Waitlist.begin();

	for(int i = 0; i < nSize; i++, _iter++)
	{
		CBunch* temp = (*_iter);
		SAFE_DELETE(temp);

		//	m_R_Waitlist.erase(_iter);
	}

	nSize = m_G_Waitlist.size();

	_iter = m_G_Waitlist.begin();

	for( int i = 0; i < nSize; i++, _iter++)
	{
		CBunch* temp = (*_iter);
		SAFE_DELETE(temp);
	}

	m_R_Waitlist.clear();
	m_G_Waitlist.clear();

	m_nRSelectNum = -1;
	m_nGSelectNum = -1;

	m_nSelectType = -1;

	m_nRCnt = 0;
	m_nGCnt = 0;
}

void CJoinRequestControl::SetTotalNum(int nRecommnad, int nGeneral)
{
	// 추천, 일반 수 만큼 사이즈를 잡는다.
	m_R_Waitlist.resize(nRecommnad);
	m_G_Waitlist.resize(nGeneral);

	m_R_Scroll.SetRange(nRecommnad*55);
	m_G_Scroll.SetRange(nGeneral*55);
}

void CJoinRequestControl::Restore()
{

}

void CJoinRequestControl::SetPosition()
{
	m_pTitle->Xpos = m_nBaseX + 90;
	m_pTitle->Ypos = m_nBaseY + 143;

	m_pRecommanBox->Xpos = m_nBaseX + 15;
	m_pRecommanBox->Ypos = m_nBaseY + 175;

	m_pRequestBox->Xpos = m_nBaseX + 15;
	m_pRequestBox->Ypos = m_nBaseY + 311;

	m_R_Scroll.SetDrawPos(m_nBaseX + 248, m_nBaseY + 183, 120);
	m_G_Scroll.SetDrawPos(m_nBaseX + 248, m_nBaseY + 317, 120);
}

void CJoinRequestControl::Draw()
{
	g_pDisplay->Blt(m_pTitle->Xpos		, m_pTitle->Ypos	  , m_pTitle	  );
	g_pDisplay->Blt(m_pRecommanBox->Xpos, m_pRecommanBox->Ypos, m_pRecommanBox);
	g_pDisplay->Blt(m_pRequestBox->Xpos , m_pRequestBox->Ypos , m_pRequestBox );

	m_R_Scroll.Draw();
	m_G_Scroll.Draw();

	vector<CBunch*>::iterator _iter;

	for(_iter = m_R_Waitlist.begin(); _iter != m_R_Waitlist.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp)
			pTemp->Draw(m_R_Scroll.GetPos());
	}

	for(_iter = m_G_Waitlist.begin(); _iter != m_G_Waitlist.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp)
			pTemp->Draw(m_G_Scroll.GetPos());
	}

}

void CJoinRequestControl::DeleteRes()
{
	m_R_Scroll.DeleteRes();
	m_G_Scroll.DeleteRes();

	vector<CBunch*>::iterator _iter;

	for(_iter = m_R_Waitlist.begin(); _iter != m_R_Waitlist.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp)
			pTemp->DeleteRes();
	}

	for(_iter = m_G_Waitlist.begin(); _iter != m_G_Waitlist.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp)
			pTemp->DeleteRes();
	}
}

LRESULT CJoinRequestControl::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_R_Scroll.MsgProc(hWnd, msg, wParam, lParam);
	m_G_Scroll.MsgProc(hWnd, msg, wParam, lParam);

	vector<CBunch*>::iterator _iter;

	for(_iter = m_R_Waitlist.begin(); _iter != m_R_Waitlist.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp && (pTemp->MsgProc(hWnd, msg, wParam, lParam, m_R_Scroll.GetPos()) != -1))
		{
			m_nRSelectNum = pTemp->m_nSequence;

			m_nSelectType = 1;
		}
	}

	for(_iter = m_G_Waitlist.begin(); _iter != m_G_Waitlist.end(); _iter++)
	{
		CBunch *pTemp = *_iter;

		if(pTemp && (pTemp->MsgProc(hWnd, msg, wParam, lParam, m_G_Scroll.GetPos()) != -1))
		{
			m_nGSelectNum = pTemp->m_nSequence;

			m_nSelectType = 2;

			//EraseData();
		}
	}

	return 1;
}


BOOL CJoinRequestControl::IsInside(int x, int y)
{

	return FALSE;
}

/******************************************************************************************/

CBunch*	CBunch::Create(int nBaseX, int nBaseY, int nTopLine, int nBottomLine)
{
	return new CBunch(nBaseX, nBaseY, nTopLine, nBottomLine);
}

CBunch::CBunch(int nBaseX, int nBaseY, int nTopLine, int nBottomLine)
{
	m_hWnd		= NULL;

	m_nBaseX = nBaseX;
	m_nBaseY = nBaseY;

	m_nTopLimit		= nTopLine;
	m_nBottomLimit	= nBottomLine;

	m_nRequestNum = 0;
	m_nSequence   = 0;

	m_pInputBox = NULL;

	m_nCharIdx	= -1;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CBunch::~CBunch()
{
	DeleteRes();
	SAFE_DELETE(m_pInputBox);
}

void CBunch::Init()
{
	btOk.SetFileName("guild/s_accept");
	btCancel.SetFileName("guild/refuse");
}

void CBunch::LoadRes()
{
	if(!g_pDisplay)	return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	SAFE_DELETE(m_pInputBox);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pInputBox, "interface/guild/guild_entry/enter_box.bmp");

	btOk.LoadRes();
	btCancel.LoadRes();

}

void CBunch::Draw(int nScrollPos)
{
	if(!g_pDisplay )// ||
		return;

	DrawToBlt(m_pInputBox, nScrollPos);

	btOk.DrawClip(m_nTopLimit, m_nBottomLimit, nScrollPos);

	btCancel.DrawClip(m_nTopLimit, m_nBottomLimit, nScrollPos);

	nameBox.DrawClipText(nameBox.m_StringPosY, m_nTopLimit, m_nBottomLimit, nScrollPos);
	dateBox.DrawClipText(dateBox.m_StringPosY, m_nTopLimit, m_nBottomLimit, nScrollPos);

// 	btOk.DrawClip(m_nTopLimit, m_nBottomLimit, nScrollPos);
//
// 	btCancel.DrawClip(m_nTopLimit, m_nBottomLimit, nScrollPos);
//
// 	nameBox.DrawClipText(nameBox.m_StringPosY, m_nTopLimit, m_nBottomLimit, nScrollPos);
// 	dateBox.DrawClipText(dateBox.m_StringPosY, m_nTopLimit, m_nBottomLimit, nScrollPos);

}

BOOL CBunch::DrawToBlt(CSurface* pSurface, int nScrollPos)
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


	if(dest.top - nScrollPos <= m_nTopLimit &&
			dest.bottom - nScrollPos <= m_nTopLimit)	// 위쪽  밖에 있음..
	{
		return FALSE;
	}
	else if(dest.top - nScrollPos <= m_nTopLimit &&		// 윗 영역에 걸쳤음.
			dest.bottom - nScrollPos >= m_nTopLimit)
	{

		src.top  = abs((dest.top - nScrollPos) - m_nTopLimit );
		dest.top = m_nTopLimit;
		dest.bottom = (dest.bottom - nScrollPos);
	}
	else if(dest.top - nScrollPos >= m_nTopLimit &&		// 모두 영역 안에 있음.
			dest.bottom - nScrollPos <= m_nBottomLimit)
	{
		dest.top    = pSurface->Ypos - nScrollPos;
		dest.bottom = dest.top + pSurface->GetHeight();

	}
	else if(dest.top - nScrollPos <= m_nBottomLimit &&
			dest.bottom - nScrollPos >= m_nBottomLimit)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( m_nBottomLimit - (dest.top - nScrollPos) );
		dest.top    = (dest.top- nScrollPos);
		dest.bottom = m_nBottomLimit;
	}
	else if(dest.top - nScrollPos >= m_nBottomLimit &&
			dest.bottom - nScrollPos >= m_nBottomLimit) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}

	g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, pSurface, &src);


	return TRUE;
}


void CBunch::DeleteRes()
{
	nameBox.DeleteRes();
	dateBox.DeleteRes();

	btOk.DeleteRes();
	btCancel.DeleteRes();

}

void CBunch::SetCharName(char* pName)
{
	nameBox.SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CBunch::SetRePosition(int nNum, int nPosY)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;


	nameBox.SetPos_Center(m_nBaseX + 25 , m_nBaseY + 205 + (55*m_nSequence) + nPosY);
	dateBox.SetPos_Center(m_nBaseX + 19 , m_nBaseY + 227 + (55*m_nSequence) + nPosY);

	btOk.SetPosition(m_nBaseX + 160	   , m_nBaseY + 195 + (55*m_nSequence) + nPosY);
	btCancel.SetPosition(m_nBaseX + 204, m_nBaseY + 195 + (55*m_nSequence) + nPosY);

	m_pInputBox->Xpos = m_nBaseX + 19;
	m_pInputBox->Ypos = m_nBaseY + 195 + (55*m_nSequence) + nPosY;
}

void CBunch::SetTime(t_Info time)
{
	if( time.nYear	== -1 &&
			time.nMon	== -1 &&
			time.nDay	== -1 &&
			time.nHour	== -1 &&
			time.nMin	== -1 &&
			time.nSec	== -1)
		return;

	char szTmp[256] = {0,};

	if( time.nYear != -1 )
	{
		// IDS_GUILD_JOIN_CANCEL_D_DAY_YEAR "%02d/%02d/%02d %02d:%02d 에 요청이 취소 됩니다."
		sprintf(szTmp, (char*)G_STRING(IDS_GUILD_JOIN_CANCEL_D_DAY_YEAR),
				time.nYear, time.nMon, time.nDay, time.nHour, time.nMin);
	}
	else if( time.nMon != -1 )
	{
		// IDS_GUILD_JOIN_CANCEL_D_DAY_MONTH "%02d/%02d %02d:%02d 에 요청이 취소됩니다."
		sprintf(szTmp, (char*)G_STRING(IDS_GUILD_JOIN_CANCEL_D_DAY_MONTH),
				time.nMon, time.nDay, time.nHour, time.nMin);

	}

	dateBox.SetString(szTmp, RGB(255, 0, 0));
}

void CBunch::SetPosition(int nPosX, int nPosY, int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;


	nameBox.Init(12, RGB(255, 255, 255), m_nBaseX + 25 , m_nBaseY + 205 + (55*m_nSequence) + nPosY, m_pInputBox->GetWidth(), m_pInputBox->GetHeight(), TRUE);
	dateBox.Init(12, RGB(255, 255, 255), m_nBaseX + 19 , m_nBaseY + 227 + (55*m_nSequence) + nPosY);

	btOk.SetPosition(m_nBaseX + 160	   , m_nBaseY + 195 + (55*m_nSequence) + nPosY);
	btCancel.SetPosition(m_nBaseX + 204, m_nBaseY + 195 + (55*m_nSequence) + nPosY);

	m_pInputBox->Xpos = m_nBaseX + 19;
	m_pInputBox->Ypos = m_nBaseY + 195 + (55*m_nSequence) + nPosY;

}

void CBunch::Restore()
{

}

void CBunch::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CBunch::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll)
{
	btOk.MsgProcClip(hWnd, msg, wParam, lParam,nScroll, m_nTopLimit, m_nBottomLimit );
	btCancel.MsgProcClip(hWnd, msg, wParam, lParam,nScroll, m_nTopLimit, m_nBottomLimit );

	switch(msg)
	{
	case WM_LBUTTONUP :
		{
			if(btOk.GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg allow %s %d\n", nameBox.m_str, 1);
				SendMsg();

				return m_nSequence;
			}
			else if(btCancel.GetState() == BTN_ACTION)
			{
				sprintf(m_MsgBuf, "mg allow %s %d\n", nameBox.m_str, 0);
				SendMsg();


				return m_nSequence;
			}
		}
		break;
	}
	return -1;
}
