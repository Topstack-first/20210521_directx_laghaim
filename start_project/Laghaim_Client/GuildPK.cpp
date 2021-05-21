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

	if( nMsg == 1) // Ȯ�ι�ư�� �������
	{
		// ��û�� �³��ϴ� �޽����� ������.
		sprintf(buf,"gpk accept %s\n",m_strName);
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(buf);
		m_bActive = false;
	}
	else if(nMsg == -1)// ��ҹ�ư�� �������
	{
		// ��û�� �����ϴ� �޽����� ������.
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
	//-- IDS_GUILD_REQUEST : %s ���κ��� ����� ��û�� �޾ҽ��ϴ�
	sprintf(strTemp,G_STRING(IDS_GUILD_REQUEST), GuildName);
	g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 8);
	m_bActive = true;
	strcpy(m_strName,GuildName);
}

void CGuildPK::Surrender(char *GuildName,char *GuildMasterName )
{
	char strTemp[200] = "";
	//-- IDS_GUILD_SURRENDER : %s ��� %s �渶�κ��� �׺� ��û�� �޾ҽ��ϴ�
	sprintf(strTemp,G_STRING(IDS_GUILD_SURRENDER),GuildName,GuildMasterName);
	g_pNk2DFrame->InsertPopup( strTemp, TYPE_NOR_OK, 1 );
	m_bActive = true;
}
