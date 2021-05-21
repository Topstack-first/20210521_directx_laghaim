#include "stdafx.h"
// TextBoard.cpp: implementation of the CTextBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "TextBoard.h"
#include "TextList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextBoard::CTextBoard()
	: m_fHeight(0)
{
	m_fBasisX = 0.f;
	m_fBasisY = 0.f;

	m_TLineList.clear();

	m_nLineBoxNum = 0;
	m_nColumnNum = 0;

	m_nNowPage = 0;

	m_nNowSelectLine = -1;

	m_nTextSize	= 12;

	Clear();
}

CTextBoard::CTextBoard(int nTextSize)
	: m_fHeight(0)
{
	m_fBasisX = 0.f;
	m_fBasisY = 0.f;

	m_TLineList.clear();

	m_nLineBoxNum = 0;
	m_nColumnNum = 0;

	m_nNowPage = 0;

	m_nNowSelectLine = -1;

	m_nTextSize	= nTextSize;

	Clear();

}

CTextBoard::~CTextBoard()
{
	Destroy();
	Clear();
}

void CTextBoard::Destroy()
{
	AllDelLine();
}

void CTextBoard::Clear()
{
	m_nNowSelectLine = -1;
	int nCount = m_TLineList.size();
	for(int i = 0; i < nCount; i++)
	{
		m_TLineList[i]->AllRemoveText();
	}
}

void CTextBoard::Init(float fBasisX, float fBasisY, float fHeight,	int nLineNum, int nColumnNum)
{
	m_fBasisX = fBasisX;
	m_fBasisY = fBasisY;

	m_fHeight = fHeight;

	m_nLineBoxNum = nLineNum;
	m_nColumnNum = nColumnNum;
}


int CTextBoard::GetSelectLine()
{
	return m_nNowSelectLine;
}

void CTextBoard::Draw(int nPage)
{
	if(m_TLineList.empty() || nPage < 0)
		return;

	m_nNowPage = nPage;

	for(m_iter = m_TLineList.begin(); m_iter != m_TLineList.end(); m_iter++)
	{
		TEXTLINE_LIST->Draw(nPage);

	}
}

void CTextBoard::DrawLine(int nLine, int nX, int nY, BOOL bOver)
{
	if(m_TLineList.empty() || nLine < 0)
		return;

	for(m_iter = m_TLineList.begin(); m_iter != m_TLineList.end(); m_iter++)
	{
		TEXTLINE_LIST->DrawLine(nLine, nX, nY, bOver);
	}
}

void CTextBoard::DrawClipLine(int nLine, int nX, int nY, int nMin, int nMax, int nPos, BOOL bOver)
{
	if(m_TLineList.empty() || nLine < 0)
		return;

	for(m_iter = m_TLineList.begin(); m_iter != m_TLineList.end(); m_iter++)
	{
		TEXTLINE_LIST->DrawLine(nLine, nX, nY, nMin, nMax, nPos, bOver);
	}
}

void CTextBoard::AddLine(int nTextIdx, float fBoxWidth)
{
	m_iter = m_TLineList.find(nTextIdx);

	if(m_iter == m_TLineList.end())
	{
		CTextList * pTList;
		pTList = new CTextList;
		pTList->Init(m_nTextSize, TEXTLIST_RGB_NOR/*RGB(255, 255, 255)*/, m_fBasisX+(fBoxWidth*nTextIdx), m_fBasisY, fBoxWidth, m_fHeight);

		for(int i = 0; i < m_nLineBoxNum; i++)
		{
			pTList->AddTBox(i);
		}

		m_TLineList[nTextIdx] = pTList;
	}
}

void CTextBoard::AllDelLine()
{
	int nSize = m_TLineList.size();

	for(int i = 0; i < nSize; i++)
	{
		delete m_TLineList[i];		
	}

	m_TLineList.clear();
}

void CTextBoard::DelLine(int nLine)
{
	int nCount = m_TLineList.size();

	for(int i = 0; i < nCount; i++)
	{
		m_TLineList[i]->DelLine(nLine);
	}
}

char * CTextBoard::GetText(int nLineIdx, int nTextIdx)
{
	if(nTextIdx < 0)
		return NULL;
	if(nTextIdx > (m_TLineList.size()-1))
		return NULL;

	return (m_TLineList[nTextIdx]->GetText(nLineIdx));
}

void CTextBoard::SetText(int nLineIdx, int nTextIdx, char *szText)
{
	if(nTextIdx < 0)
		return;
	if(nTextIdx > (m_TLineList.size()-1))
		return;

	///071102 원석 수정 : 리스트가 기본라인 수를 넘으면 추가로 확장시켜준다.
	if( nLineIdx >= m_nLineBoxNum ) // 리스트 한계에 들어섰다면...
		ExtendedBoardLine( 200 ); // 200라인 단위로 확장 시킨다.

	m_TLineList[nTextIdx]->SetText(nLineIdx, szText);
}

void CTextBoard::RemoveLine(int nLineIdx)
{
	int nCount = m_TLineList.size();
	for(int i = 0; i < nCount; i++)
	{
		m_TLineList[i]->RemoveText(nLineIdx);
	}
}

void CTextBoard::AllRemoveLine()
{
	if(!m_TLineList.empty())
	{
		int nCount = m_TLineList.size();
		for(int i = 0; i < nCount; i++)
		{
			m_TLineList[i]->AllRemoveText();
		}
	}
}

void CTextBoard::SetBasisPos(float fBasisX, float fBasisY)
{
	float fTLinePosition = 0.f;
	m_fBasisX = fBasisX; ///071102 원석 수정 : 이걸 기억해둬야 라인 추가 시에 좌표를 셋팅한다.
	m_fBasisY = fBasisY; ///071102 원석 수정 : 이걸 기억해둬야 라인 추가 시에 좌표를 셋팅한다.

	for(m_iter = m_TLineList.begin(); m_iter != m_TLineList.end(); m_iter++)
	{
		TEXTLINE_LIST->SetBasisPos(fBasisX+fTLinePosition, fBasisY);
		fTLinePosition += TEXTLINE_LIST->GetBoxWidth();
	}
}

void CTextBoard::DeleteRes()
{
	int nCount = m_TLineList.size();

	for(int i = 0; i < nCount; i++)
	{
		m_TLineList[i]->DeleteRes();
	}
}

void CTextBoard::Restore()
{
	int nCount = m_TLineList.size();

	for(int i = 0; i < nCount; i++)
	{
		m_TLineList[i]->Restore();
	}
}

void CTextBoard::SelectLine(int nSelectNum, bool bRelease)
{
	int nCount;
	if(nSelectNum < 0 || nSelectNum >= m_nLineBoxNum)
	{

	}
	else if(nSelectNum >= 0 && bRelease)
	{
		nCount = m_TLineList.size();
		for(int i = 0; i < nCount; i++)
		{
			m_TLineList[i]->SetSelect(nSelectNum, true, TEXTLIST_RGB_CLK/*RGB(255, 0, 0)*/);	//클릭
		}
	}
	else if(nSelectNum >= 0 && !bRelease)
	{
		nCount = m_TLineList.size();
		for(int i = 0; i < nCount; i++)
		{
			m_TLineList[i]->SetSelect(nSelectNum, true, TEXTLIST_RGB_NOR/*RGB(0, 255, 64)*/);	 //노말로 전환
		}
	}
}

HRESULT CTextBoard::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return 0;
}

void CTextBoard::ExtendedBoardLine( int Plus ) ///071102 원석 수정 : 라인수가 엄청 많이 와서 기존 초기 수치를 넘을때 확장해주는 함수.
{
	int nCount = m_TLineList.size();

	for(int k = 0; k < nCount; k++) // 전 세로 단위의 라인수를 확장해줘야한다.
	{
		for(int i = m_nLineBoxNum ; i < m_nLineBoxNum+Plus ; i++)
		{
			m_TLineList[k]->AddTBox(i);
		}
	}

	SetBasisPos( m_fBasisX, m_fBasisY );

	m_nLineBoxNum += Plus; // 순서상 여기서 늘린다.
}
