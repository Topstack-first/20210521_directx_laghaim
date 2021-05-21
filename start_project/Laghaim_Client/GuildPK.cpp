#include "stdafx.h"
#include "tcpipcon.h"
#include "msgpopup.h"
#include "uimgr.h"
#include "GuildPK.h"
#include "headers.h"
#include "g_stringmanager.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildPK::CGuildPK()
{
	m_bActive = m_bGuildPK = false;
	ZeroMemory(m_strName,sizeof(m_strName));
	ZeroMemory(m_sEnemyGName,sizeof(m_sEnemyGName));
}

CGuildPK::~CGuildPK()
{
}

LRESULT CGuildPK::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	char buf[256] = "";

	if( nMsg == 1) // 확인버튼이 눌린경우
	{
		// 요청을 승낙하는 메시지를 보낸다.
		sprintf(buf,"gpk accept %s\n",m_strName);
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(buf);
		m_bActive = false;
	}
	else if(nMsg == -1)// 취소버튼이 눌린경우
	{
		// 요청을 거절하는 메시지를 보낸다.
		sprintf(buf,"gpk cancel %s\n",m_strName);
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(buf);
		m_bActive = false;
	}

	return 0;
}

void CGuildPK::AskGPK(char *GuildName)
{
	char strTemp[200] = "";
	//-- IDS_GUILD_REQUEST : %s 길드로부터 길드전 신청을 받았습니다
	sprintf(strTemp,G_STRING(IDS_GUILD_REQUEST), GuildName);
	g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 8);
	m_bActive = true;
	strcpy(m_strName,GuildName);
}

void CGuildPK::Surrender(char *GuildName,char *GuildMasterName )
{
	char strTemp[200] = "";
	//-- IDS_GUILD_SURRENDER : %s 길드 %s 길마로부터 항복 신청을 받았습니다
	sprintf(strTemp,G_STRING(IDS_GUILD_SURRENDER),GuildName,GuildMasterName);
	g_pNk2DFrame->InsertPopup( strTemp, TYPE_NOR_OK, 1 );
	m_bActive = true;
}
