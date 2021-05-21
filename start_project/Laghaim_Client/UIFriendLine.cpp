#include "stdafx.h"
#include "UIFriendLine.h"
#include "ddutil.h"
#include "Config.h"
#include "g_stringmanager.h"
#include "Nk2DFrame.h"
#include "window_resource.h"

#define FONT_COLOR_ONLINE	RGB(255, 255, 0)
#define FONT_COLOR_OFFLINE	RGB(128, 128, 128)
#define FONT_COLOR_COLORKEY	RGB(0, 0, 1)
#define FRIEND_FONT_SIZE 12

#define TEXT_POS_OFFSET_NAME	0
#define TEXT_POS_OFFSET_CHNL	120
#define TEXT_POS_OFFSET_ZONE	190

extern char g_strFontName[];
extern HFONT G_GetFont(int h, int w, const char* const fontname);

CUIFriendLine::CUIFriendLine()
	: m_pSurfTxtName(NULL)
	, m_pSurfTxtChannel(NULL)
	, m_pSurfTxtZone(NULL)
{
	m_info.login = false;
	m_info.channel = -1;
	m_info.zone = -1;
	m_info.index = 0;
}

CUIFriendLine::~CUIFriendLine()
{
	SAFE_DELETE(m_pSurfTxtName);
	SAFE_DELETE(m_pSurfTxtChannel);
	SAFE_DELETE(m_pSurfTxtZone);
}

void CUIFriendLine::SetInfo(stFriend info)
{
	m_info = info;

	COLORREF colorFont;
	int channel = -1;
	int zone = -1;
	if( m_info.login == true )
	{
		colorFont = FONT_COLOR_ONLINE;
		channel = m_info.channel;
		zone = m_info.zone;
	}
	else
	{
		colorFont = FONT_COLOR_OFFLINE;
		channel = -1;
		zone = -1;
	}

	bool bForceEmpty = false;
	if( m_info.deny == true )
	{
		bForceEmpty = true;
	}

	ResetNameTxt(m_info.name, colorFont);
	ResetChannelTxt(channel, colorFont, bForceEmpty);
	ResetZoneTxt(zone, colorFont, bForceEmpty);
}

stFriend CUIFriendLine::GetInfo()
{
	return m_info;
}

int CUIFriendLine::GetIndex()
{
	return m_info.index;
}

void CUIFriendLine::ChangeLogin(const bool& login)
{
	m_info.login = login;

	if( login == false )
	{
		m_info.channel = -1;
		m_info.zone = -1;
	}

	SetInfo(m_info);
}

void CUIFriendLine::ChangeChannel(const int& channel)
{
	COLORREF colorFont = FONT_COLOR_ONLINE;
	if( m_info.login == false )
	{
		colorFont = FONT_COLOR_OFFLINE;
	}
	m_info.channel = channel;

	ResetChannelTxt(channel, colorFont);
}

void CUIFriendLine::ChangeZone(const int& zone)
{
	COLORREF colorFont = FONT_COLOR_ONLINE;
	if( m_info.login == false )
	{
		colorFont = FONT_COLOR_OFFLINE;
	}
	m_info.zone = zone;

	ResetZoneTxt(zone, colorFont);
}

void CUIFriendLine::Draw(int posX, int posY)
{
	g_pDisplay->Blt(posX + TEXT_POS_OFFSET_NAME, posY, m_pSurfTxtName);
	g_pDisplay->Blt(posX + TEXT_POS_OFFSET_CHNL, posY, m_pSurfTxtChannel);
	g_pDisplay->Blt(posX + TEXT_POS_OFFSET_ZONE, posY, m_pSurfTxtZone);
}

void CUIFriendLine::ResetNameTxt(const std::string& name, COLORREF color)
{
	m_info.name = name;

	char szName[MAX_PATH] = {0,};
	sprintf(szName, "%s", name.c_str());

	HFONT font = G_GetFont(FRIEND_FONT_SIZE, 0, g_strFontName);

	SAFE_DELETE(m_pSurfTxtName);

	g_pDisplay->CreateSurfaceFromText(&m_pSurfTxtName, font, szName, FONT_COLOR_COLORKEY, color);
	if( m_pSurfTxtName )
		m_pSurfTxtName->SetColorKey(FONT_COLOR_COLORKEY);
}

void CUIFriendLine::ResetChannelTxt(const int& channel, COLORREF color, bool forceEmpty)
{
	m_info.channel = channel;

	char szChannelName[MAX_PATH] = {0,};

	if( forceEmpty == true )
		sprintf(szChannelName, " ");
	else if( channel >= 0 )
		sprintf(szChannelName, G_STRING(IDS_MSG_CHANNEL), channel+1);
	else
		sprintf(szChannelName, "None");

	HFONT font = G_GetFont(FRIEND_FONT_SIZE, 0, g_strFontName);

	SAFE_DELETE(m_pSurfTxtChannel);

	g_pDisplay->CreateSurfaceFromText(&m_pSurfTxtChannel, font, szChannelName, FONT_COLOR_COLORKEY, color);
	if( m_pSurfTxtChannel )
		m_pSurfTxtChannel->SetColorKey(FONT_COLOR_COLORKEY);
}

void CUIFriendLine::ResetZoneTxt(const int& zone, COLORREF color, bool forceEmpty)
{
	m_info.zone = zone;

	char szZoneName[MAX_PATH] = {0,};

	if( forceEmpty == true )
		sprintf(szZoneName, " ");
	else if( zone >= 0 )
		sprintf(szZoneName, "%s", g_pNk2DFrame->GetWindowRes()->GetZoneName(zone));
	else
		sprintf(szZoneName, "None");

	HFONT font = G_GetFont(FRIEND_FONT_SIZE, 0, g_strFontName);

	SAFE_DELETE(m_pSurfTxtZone);

	g_pDisplay->CreateSurfaceFromText(&m_pSurfTxtZone, font, szZoneName, FONT_COLOR_COLORKEY, color);
	if( m_pSurfTxtZone )
		m_pSurfTxtZone->SetColorKey(FONT_COLOR_COLORKEY);
}

void CUIFriendLine::RestoreSurfaces()
{
	SetInfo(m_info);
}