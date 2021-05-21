#ifndef _CUIFRIENDLINE_H_
#define _CUIFRIENDLINE_H_

#include <Windows.h>
#include "DataFriend.h"

class CSurface;
class CUIFriendLine
{
public:
	CUIFriendLine();
	~CUIFriendLine();

public:
	void SetInfo(stFriend info);
	stFriend GetInfo();
	int GetIndex();
	void ChangeLogin(const bool& login);
	void ChangeChannel(const int& channel);
	void ChangeZone(const int& zone);

public:
	void Draw(int posX, int posY);

public:
	bool GetLogin()
	{
		return m_info.login;
	}

public:
	void ResetNameTxt(const std::string& name, COLORREF color);
	void ResetChannelTxt(const int& channel, COLORREF color, bool forceEmpty = false);
	void ResetZoneTxt(const int& zone, COLORREF color, bool forceEmpty = false);

	void RestoreSurfaces();

private:
	stFriend	m_info;
	CSurface*	m_pSurfTxtName;
	CSurface*	m_pSurfTxtChannel;
	CSurface*	m_pSurfTxtZone;
};

#endif