#ifndef _GUILD_MANAGER_H_
#define _GUILD_MANAGER_H_

#include "Config.h"

#include "MsgPopUp.h"
#include "headers.h"

class CControlGuild;

class CGuildSystemManager
{

public:
	static CGuildSystemManager* Create();

	~CGuildSystemManager();

	/**************************************************************************/
	void Init();

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/**************************************************************************/
	void	guilds_SendMsg();
	BOOL	guilds_CheckStr(char* szMoney);

	void	set_guilds_name(char* szName)
	{
		strcpy(m_szguildName, szName);
	}
	void	set_guilds_recomname(char* szName)
	{
		strcpy(m_szrecomName, szName);
	}
	void	resetNameBuffer();

	void	SetSubType(int nNum)
	{
		m_nSubType = nNum;
	}
	int		GetSubType() const
	{
		return m_nSubType;
	}

	void	SetMainType(int nNum)
	{
		m_nMainType = nNum;
	}
	int		GetMainType() const
	{
		return m_nMainType;
	}

	void	set_tempbuffer1(char* szTemp)
	{
		strcpy(m_szTempBuffer1, szTemp);
	}
	void	set_tempbuffer2(char* szTemp)
	{
		strcpy(m_szTempBuffer2, szTemp);
	}

	LRESULT guild_popMsgproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, int nProcess, char* szString);

private:

	char m_MsgBuf[1024];

	char m_szguildName[128];
	char m_szrecomName[128];

	int		m_nSubType;
	int		m_nMainType;

	char m_szTempBuffer1[128];
	char m_szTempBuffer2[128];

	CGuildSystemManager();

};
#endif //_GUILD_MANAGER_H_