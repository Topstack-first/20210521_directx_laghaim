#include "stdafx.h"
#include "ControlPopupMenu.h"

ControlPopupMenu::ControlPopupMenu()
	: m_bVisible(false)
	, m_iPosX(0)
	, m_iPosY(0)
	, m_iGapX(0)
	, m_iGapY(0)
	, m_pSurpBack(NULL)
	, m_index(0)
{
}

ControlPopupMenu::~ControlPopupMenu()
{
}

void ControlPopupMenu::LoadRes()
{
}

void ControlPopupMenu::Draw()
{
	if( m_bVisible == false )
		return;

	if( m_pSurpBack != NULL )
		g_pDisplay->Blt(m_pSurpBack->Xpos, m_pSurpBack->Ypos, m_pSurpBack);

	unsigned int size = m_vecBtns.size();
	for( unsigned int i = 0; i < size; i++ )
	{
		m_vecBtns[i]->Draw();
	}
}

void ControlPopupMenu::DeleteRes()
{
	SAFE_DELETE(m_pSurpBack);

	unsigned int size = m_vecBtns.size();
	for( unsigned int i = 0; i < size; i++ )
	{
		SAFE_DELETE(m_vecBtns[i]);
	}
}

LRESULT ControlPopupMenu::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

void ControlPopupMenu::SetPosition(const int& posX, const int& posY, const bool& bDown)
{
	if( m_pSurpBack == NULL )
		return;

	int calcPosY = posY;
	if( bDown == false )
		calcPosY -= m_pSurpBack->GetHeight();

	m_pSurpBack->SetPos(posX, calcPosY);

	m_iPosX = posX;
	m_iPosY = posY;

	int btnPosX = 0;
	int btnPosY = 0;
	int btnHeight = m_iGapY;

	unsigned int size = m_vecBtns.size();
	for( unsigned int i = 0; i < size; i++ )
	{
		btnPosX = m_iPosX + m_iGapX;
		btnPosY = m_iPosY + btnHeight;
		m_vecBtns[i]->SetPosition(btnPosX, btnPosY);
		btnHeight += m_vecBtns[i]->Height + m_iGapY;
	}
}

bool ControlPopupMenu::isInside(const int& pointX, const int& pointY)
{
	if( pointX >= m_iPosX && pointX <= m_iPosX + m_pSurpBack->GetWidth() &&
			pointY >= m_iPosY && pointY <= m_iPosY + m_pSurpBack->GetHeight() )
		return true;

	return false;
}