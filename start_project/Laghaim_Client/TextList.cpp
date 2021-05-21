#include "stdafx.h"
// TextList.cpp: implementation of the CTextList class.
//
//////////////////////////////////////////////////////////////////////
#include "TextList.h"
#include "TextOutBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextList::CTextList()
	: m_fHeight(0)
{
	m_fBasisX = 0.f;
	m_fBasisY = 0.f;

	m_fBoxWidth = 0.f;

	m_BTextList.clear();

	m_bIsColLine = true;

	m_nSelectNum = -1;

	m_nfontsize = 0;

	m_color = RGB( 0, 0, 0);

}

CTextList::~CTextList()
{
	AllDelTBox();
}

void CTextList::Init(int font_size, COLORREF font_color, int pos_x, int pos_y, int width/* = -1*/, int height/* = -1*/)
{
	m_fBasisX = pos_x;
	m_fBasisY = pos_y;
	m_fBoxWidth = width;
	m_fHeight = height;

	m_color = font_color;
	m_nfontsize = font_size;

}

void CTextList::Draw(int nPage)
{
	m_iter = m_BTextList.find(nPage*10);

	int i = 0;

	for(; m_iter != m_BTextList.end(), i < 10; m_iter++, i++)
	{
		if( TBOX_LIST != NULL )
		{
			TBOX_LIST->Draw();
		}
	}
}

void CTextList::DrawLine(int nLine, int nX, int nY, BOOL bOver)
{
	m_iter = m_BTextList.find(nLine);

	if( m_iter == m_BTextList.end() )	
		return;	

	if( TBOX_LIST != NULL )
	{
		TBOX_LIST->DrawText(nY, bOver);
	}
}

void CTextList::DrawLine(int nLine, int nX, int nY, int nMin, int nMax, int nPos, BOOL bOver)
{
	m_iter = m_BTextList.find(nLine);

	if( m_iter == m_BTextList.end() )
		return;

	if( TBOX_LIST != NULL )
	{
		TBOX_LIST->DrawClipText(nY, nMin, nMax, nPos, bOver);
	}
}

void CTextList::AddTBox(int nTextIdx)
{
	m_iter = m_BTextList.find(nTextIdx);

	if(m_iter == m_BTextList.end())
	{
		CTextOutBox * pTBox;
		pTBox = new CTextOutBox;

		pTBox->Init(m_nfontsize, m_color,m_fBasisX, m_fBasisY+(m_fHeight*nTextIdx), m_fBoxWidth, m_fHeight, m_bIsColLine);

		m_BTextList[nTextIdx] = pTBox;
	}
}

void CTextList::AllDelTBox()
{
	int nSize = m_BTextList.size();
	for(int i = 0; i < nSize; i++)
	{
		delete m_BTextList[i];
		m_iter = m_BTextList.find(i);
		m_BTextList.erase(m_iter);
	}

	m_BTextList.clear();
}

void CTextList::DelLine(int nLine)
{
	delete m_BTextList[nLine];
	m_iter = m_BTextList.find(nLine);
	m_BTextList.erase(m_iter);
}

void CTextList::SetAllSelect()
{
}

void CTextList::SetAllUnSelect()
{
	m_nSelectNum = -1;
}


char* CTextList::GetText(int nTextIdx)
{
	if(nTextIdx < 0)
		return NULL;
	if(nTextIdx > (m_BTextList.size()-1))
		return NULL;

	return (m_BTextList[nTextIdx]->m_str);
}

void CTextList::SetText(int nTextIdx, char * szText)
{
	if(nTextIdx < 0)
		return;
	if(nTextIdx > (m_BTextList.size()-1))
		return;
	
	m_BTextList[nTextIdx]->SetString_Center_Arrange(szText,
			RGB(152, 212, 4),
			RGB( 97, 249, 254),
			TRUE);
}

void CTextList::RemoveText(int nTextIdx)
{
	m_BTextList[nTextIdx]->SetString("");
}

void CTextList::AllRemoveText()
{
	if(!m_BTextList.empty())
	{
		int nCount = m_BTextList.size();
		for(int i = 0; i < nCount; i++)
		{
			m_BTextList[i]->DeleteRes();
		}
	}
}

void CTextList::DeleteRes()
{
	if(!m_BTextList.empty())
	{
		int nCount = m_BTextList.size();
		for(int i = 0; i < nCount; i++)
		{
			m_BTextList[i]->DeleteRes();
		}
	}
}

int CTextList::GetSelectNum()
{
	return m_nSelectNum;
}

void CTextList::SetSelect(int nTextIdx, bool fFlag, COLORREF c_select/* = RGB(0, 0, 0)*/, COLORREF c_unselect/* = RGB(0, 0, 0)*/)
{
	m_BTextList[nTextIdx]->ChangeColor(c_select);
}

void CTextList::SetSelectOther(int nTextIdx, COLORREF color/* = RGB(0, 0, 0)*/)
{
	if(m_BTextList.size() < nTextIdx)
		return ;

	for(m_iter = m_BTextList.begin(); m_iter != m_BTextList.end(); m_iter++)
	{
		m_BTextList[nTextIdx]->ChangeColor(color);
	}
}

void CTextList::SetBasisPos(float fBasisX, float fBasisY)
{
	int nCount = 0;
	for(m_iter = m_BTextList.begin(); m_iter != m_BTextList.end(); m_iter++)
	{
		if( (nCount%10) == 0 )
			nCount = 0;

		if( TBOX_LIST != NULL )
			TBOX_LIST->SetPos_Center(fBasisX, fBasisY+(m_fHeight*nCount));

		nCount++;
	}
}

void CTextList::Restore()
{
	for(m_iter = m_BTextList.begin(); m_iter != m_BTextList.end(); m_iter++)
	{
		if( TBOX_LIST != NULL )
			TBOX_LIST->Restore();
	}
}

BOOL CTextList::InputProcess(int x, int y, int nPage)
{
	int i = 0;

	for(m_iter = m_BTextList.begin(); m_iter != m_BTextList.end(); m_iter++, i++)
	{
		if(TBOX_LIST != NULL)
		{
			if(x >= TBOX_LIST->m_StringPosX && x < TBOX_LIST->m_StringPosX + TBOX_LIST->m_Width &&
					y >= TBOX_LIST->m_StringPosY && y < TBOX_LIST->m_StringPosY + TBOX_LIST->m_Height &&
					TBOX_LIST->IsSurface())
			{
				m_nSelectNum = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}
