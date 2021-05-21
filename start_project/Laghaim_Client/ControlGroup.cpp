#include "stdafx.h"
#include "ControlGroup.h"
#include "CheckButton.h"
#include "d3dutil.h"


CCheckGroup* CCheckGroup::Create(int nbcnt/* = 2*/, char* groupname/* = NULL*/ )
{
	return new CCheckGroup(nbcnt, groupname);
}

CCheckGroup::CCheckGroup(int nbcnt /* = 2 */, char* groupname/* = NULL*/ )
{
	m_nbtnum = nbcnt;

	m_bVisible = false;

	ZeroMemory(m_szGroupName, sizeof(m_szGroupName));
	strcpy(m_szGroupName, groupname);

	m_nPosX = 0;
	m_nPosY = 0;

	m_uiwidth  = 0;
	m_uiheight = 0;

	for(int i = 0; i < 3; i++)
		m_bCheckStatus[i] = false;

	m_pbt = NULL;

	m_pbt = CCheckbutton::Create(m_nbtnum);
}

CCheckGroup::~CCheckGroup()
{
	DeleteRes();

	SAFE_DELETE_ARRAY(m_pbt);
}

void CCheckGroup::SetFileName(char* szName)
{
	if(!m_pbt || m_nbtnum == 0)	return;

	for(int i = 0; i < m_nbtnum; i++)
	{
		m_pbt[i].SetFileName(szName);
	}

}

void CCheckGroup::SetBtSet(int nNum/* = 0*/, bool bEnable)
{
	if(m_nbtnum <= nNum)	return;

	for(int i = 0; i < m_nbtnum; i++)
	{
		if(nNum == i)
			m_pbt[nNum].SetStatus(bEnable);
		else
			m_pbt[i].SetStatus(!bEnable);
	}
}

int CCheckGroup::GetBtSet()
{
	for(int i = 0; i < m_nbtnum; i++)
	{
		if(m_pbt[i].GetEnable())
			return i;
	}

	return -1;
}

void CCheckGroup::SetbuttonPos(PBTPS pbt, int nx/* = 0*/, int ny/* = 0*/, unsigned int nw/* = 0*/, unsigned int nh/* = 0*/)
{
	for(int i = 0; i < m_nbtnum; i++)
	{
		m_pbt[i].SetPosition(pbt[i].m_stPosX, pbt[i].m_stPosY);
	}

	// 그룹 영역 설정
	m_nPosX = nx;
	m_nPosY = ny;
	m_uiwidth  = nw;
	m_uiheight = nh;
}

void CCheckGroup::DeleteRes()
{
	if(!m_pbt)	return;

	for(int i = 0; i < m_nbtnum; i++)
		m_pbt[i].DeleteRes();

//	SAFE_DELETE_ARRAY(m_pbt);

}

void CCheckGroup::LoadRes(int w /* = -1 */, int h /* = -1  */)
{

	if(!m_pbt || m_nbtnum == 0)	return;

	for(int i = 0; i < m_nbtnum; i++)
	{
		m_pbt[i].LoadRes();
	}
}

bool CCheckGroup::IsInside(int x, int y)
{
	if ( ( ( x > m_nPosX) && ( x < m_nPosX + (int)m_uiwidth ) )
			&& ( ( y > m_nPosY) && ( y < (m_nPosY + (int)m_uiheight) ) ) )
		return true;
	else
		return false;
}

void CCheckGroup::DrawToBlt(bool bChecked /* = false */)
{
	if(!m_pbt || m_nbtnum == 0)	return;

	for(int i = 0; i < m_nbtnum; i++)
	{
		m_pbt[i].DrawToBlt(bChecked); // bchecked : true  ---> 그린다.
		// bchecked : false ---> 그리지 않는다.
	}

	m_bVisible = bChecked;
}

void CCheckGroup::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!m_pbt || m_nbtnum == 0)
		return;

	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return;

	int nTemp = -1;

	bool bIsDown = false;

	for(int i = 0; i < m_nbtnum; i++)
	{
		bIsDown = m_pbt[i].MsgProcGroup(hWnd, msg, wParam, lParam);

		if(bIsDown)
		{
			nTemp = i;
			m_pbt[i].SetStatus(true);
			m_bCheckStatus[nTemp] = true;
		}
	}

	// 선택된 버튼이 없다.
	if(!bIsDown && nTemp == -1)	return;

	// 상태 갱신
	for( int i = 0; i < m_nbtnum; i++)
	{
		if(i != nTemp)
		{
			m_bCheckStatus[i] = false;
			m_pbt[i].SetStatus(false);
		}
	}

}
