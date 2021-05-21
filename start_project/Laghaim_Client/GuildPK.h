#ifndef _GUILD_PK_H_
#define _GUILD_PK_H_

#include "Config.h"

class CTcpIpConnection;
class CMsgPopUp;

class CGuildPK
{
public:
	void Surrender(char *GuildName,char *GuildMasterName );
	CGuildPK();
	virtual ~CGuildPK();

	bool m_bActive;
	char m_strName[256];
	char m_sEnemyGName[100];
	bool m_bGuildPK;

	void AskGPK(char* GuildName);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
};

#endif // _GUILD_PK_H_
