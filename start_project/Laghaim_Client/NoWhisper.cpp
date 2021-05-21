#include "stdafx.h"

#include <list>
#include "NoWhisper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoWhisper::CNoWhisper()
{}

CNoWhisper::~CNoWhisper()
{
	char *pName = NULL;
	list<char *>::iterator pr;

	pr = m_listName.begin();
	for ( ; pr != m_listName.end(); pr++)
	{
		pName = *pr;
		SAFE_DELETE(pName);
	}
}

void CNoWhisper::AddToList(char *player_name)
{
	if( player_name == NULL )
		return;

	char *pName = NULL;
	list<char *>::iterator pr = m_listName.begin();

	if (player_name)
	{
		// 우선 리스트에 이미 존재하는지 확인한다.
		BOOL bExist;

		bExist = IsIntheList(player_name);

		if (!bExist)
		{
			pName = new char[strlen(player_name)+1];
			strcpy(pName, player_name);
			m_listName.insert(pr, pName);
		}
		else
			DeleteFromList(player_name);
	}
}

void CNoWhisper::DeleteFromList(char *player_name)
{
	if( player_name == NULL )
		return;

	char *pName = NULL;
	list<char *>::iterator pr = m_listName.begin();

	for( ; pr != m_listName.end(); pr++)
	{
		pName = *pr;

		if (strcmp(pName, player_name) == 0)
		{
			delete pName;
			m_listName.erase(pr);
			break;
		}
	}
}

BOOL CNoWhisper::IsIntheList(char *player_name)
{
	if( player_name == NULL )
		return FALSE;

	char *pName = NULL;
	list<char *>::iterator pr = m_listName.begin();

	for( ; pr != m_listName.end(); pr++)
	{
		pName = *pr;

		if (strcmp(pName, player_name) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}


