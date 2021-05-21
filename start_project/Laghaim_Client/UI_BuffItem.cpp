#include "stdafx.h"
#include "UI_BuffItem.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "main.h"
#include "Compas.h"
#include "AlphaPopUp.h"
#include "string_res.h"
#include "g_stringmanager.h"

#define BUFFICON_SIZE 27
#define FLICK_START_TIME 60000 // 60√ 

UI_BuffItem::UI_BuffItem()
	: m_posX(0)
	, m_posY(0)
	, m_pPopDesc(NULL)
{
	memset(m_pSurIcon, 0, sizeof(m_pSurIcon));
	LoadRes();
}

UI_BuffItem::~UI_BuffItem()
{
	DeleteRes();
}

void UI_BuffItem::LoadRes()
{
	DeleteRes();

	char szFilename[MAX_PATH] = {0,};
	for( int i = 0; i < POTION_EVENT_NUM; i++ )
	{
		sprintf(szFilename, "INTERFACE/BUFFICON/BUFF_%02d.bmp", i);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurIcon[i], szFilename);
		if( m_pSurIcon[i] == NULL )
			continue;

		m_pSurIcon[i]->SetColorKey(RGB(0, 0, 0));
	}

	int screenWidth = g_pNk2DFrame->GetClientWidth();
	int screenHeight = g_pNk2DFrame->GetClientHeight();
	m_pPopDesc = new CAlphaPopUp;
	m_pPopDesc->LoadRes(screenWidth, screenHeight);
	m_pPopDesc->Init(14);

	ResetPosition();
}

void UI_BuffItem::DeleteRes()
{
	for( int i = 0; i < POTION_EVENT_NUM; i++ )
	{
		SAFE_DELETE(m_pSurIcon[i]);
	}

	SAFE_DELETE(m_pPopDesc);
}

void UI_BuffItem::ResetPosition()
{
	m_posX = g_pNk2DFrame->GetClientWidth();
	m_posY = COMPAS_HALF_HEIGHT_BASE * 2 + 5;
}

void UI_BuffItem::Draw()
{
	BuffItemMgr::VEC_BUFFITEM vecBuffItem = BuffItemMgr::GetInstance()->GetBuffList();
	unsigned int size = vecBuffItem.size();

	int posX = 0;
	int posY = 0;
	int type = 0;
	long remainTime = 0;
	long remainSec = 0;
	for( unsigned int i = 0; i < size; i++ )
	{
		posX = m_posX - ((i + 1) * BUFFICON_SIZE);
		posY = m_posY;
		type = vecBuffItem[i].type;

		if( type < 0 || type >= POTION_EVENT_NUM )
			continue;
		if( m_pSurIcon[type] == NULL )
			continue;

		remainTime = vecBuffItem[i].remainTime;
		remainSec  = remainTime % 1000;
		if( remainTime < FLICK_START_TIME && remainSec > 500 )
		{
		}
		else
		{
			g_pDisplay->Blt(posX, posY, m_pSurIcon[type]);
		}
	}

	drawDesc();
}

void UI_BuffItem::drawDesc()
{
	if( m_pPopDesc == NULL )
		return;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if( point.y < m_posY || point.y > m_posY + BUFFICON_SIZE )
		return;

	int screenWidth = g_pNk2DFrame->GetClientWidth();
	int buffIndex = (screenWidth - point.x) / BUFFICON_SIZE;

	BuffItemMgr::VEC_BUFFITEM vecBuffItem = BuffItemMgr::GetInstance()->GetBuffList();
	unsigned int size = vecBuffItem.size();

	if( buffIndex >= size )
		return;

	CMultiText multiText;
	multiText.AddString(const_cast<char*>(getName(buffIndex).c_str()), _FCOLOR_YELLOW);
	multiText.AddString(const_cast<char*>(getEffectDesc(buffIndex).c_str()), _FCOLOR_YELLOW);
	multiText.AddString(const_cast<char*>(getRemainTimeDesc(buffIndex).c_str()), _FCOLOR_YELLOW);

	m_pPopDesc->PutString(&multiText);
	m_pPopDesc->Draw(point.x, point.y);
}

std::string UI_BuffItem::getName( const int& index )
{
	std::string str;

	int buffType = BuffItemMgr::GetInstance()->GetBuffType(index);
	str = (char*)G_STRING(IDS_BUFF_NAME00 + buffType);

	return str;
}

std::string UI_BuffItem::getEffectDesc( const int& index )
{
	char buf[MAX_PATH] = {0,};
	std::string str;

	BuffItemMgr* buffMgr = BuffItemMgr::GetInstance();
	int buffVal = buffMgr->GetBuffValue(index);
	bool isPercent = buffMgr->GetBuffPercent(index);
	string strPercent = "";
	if( isPercent == true )
	{
		strPercent = "%";
	}
	sprintf(buf, "%d%s %s", buffVal, strPercent.c_str(), (char*)G_STRING(IDS_BUFF_INCREASE));
	str = buf;

	return str;
}

std::string UI_BuffItem::getRemainTimeDesc( const int& index )
{
	char buf[MAX_PATH] = {0,};
	std::string str;

	long remainTime = BuffItemMgr::GetInstance()->GetBuffRemainTime(index);
	sprintf(buf, (char*)G_STRING(IDS_BUFF_REMAINTIME), remainTime / 60000);
	str = buf;

	return str;
}

LRESULT UI_BuffItem::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}