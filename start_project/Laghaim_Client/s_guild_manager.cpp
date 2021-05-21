#include "stdafx.h"
#include "s_guild_manager.h"

#include "d3dutil.h"
#include "headers.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "NkCharacter.h"
#include "WindowMgr.h"
#include "guild_search.h"
#include "guild_promotion.h"
#include "guild_data.h"
#include "guild_battle.h"
#include "Land.h"
#include "UIMgr.h"
#include "g_stringmanager.h"







CGuildSystemManager* CGuildSystemManager::Create()
{
	return new CGuildSystemManager;
}

CGuildSystemManager::CGuildSystemManager()
{
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
	ZeroMemory(m_szTempBuffer1, sizeof(m_szTempBuffer1));
	ZeroMemory(m_szTempBuffer2, sizeof(m_szTempBuffer2));

	m_nSubType  = -1;
	m_nMainType = -1;

	resetNameBuffer();

	Init();
}

CGuildSystemManager::~CGuildSystemManager()
{

}

void CGuildSystemManager::resetNameBuffer()
{
	ZeroMemory(m_szguildName, sizeof(m_szguildName));
	ZeroMemory(m_szrecomName, sizeof(m_szrecomName));
}

void CGuildSystemManager::Init()
{
}

BOOL CGuildSystemManager::guilds_CheckStr(char* szMoney)
{
	return TRUE;
}

void CGuildSystemManager::guilds_SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CGuildSystemManager::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CGuildSystemManager::guild_popMsgproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, int nProcess, char* szString)
{
	switch(nProcess)
	{
	case 43 :
		{
			if(nMsg == 1)
			{
				if(guilds_CheckStr(szString))
				{
					sprintf(m_MsgBuf, "mg create c %s\n", szString);
					guilds_SendMsg();
				}
			}
		}
		break;
	case 45 :
		{
			if(nMsg == 1)
			{
				CGuildSearch* pSearch = NULL;
				if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_SEARCH))
				{
					pSearch = (CGuildSearch*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_SEARCH);
					sprintf(m_MsgBuf, "mg request %s\n", pSearch->Get_SelectGuildName());
					guilds_SendMsg();
				}

			}
		}
		break;

	case 46 :
		{
			if(nMsg == 1)
			{
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_PROMOTION);

				CGuildPromotion *pPromotion = NULL;

				if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_GUILD_PROMOTION))
				{
					pPromotion = (CGuildPromotion*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_PROMOTION);

					if(pPromotion && g_pRoh->GetMyGuildData())
					{
						pPromotion->guilds_Bt_Status(g_pRoh->GetMyGuildData()->GetMyGuildInfo().s_glevel);
					}
				}
			}

		}
		break;
	case 47 :
		{
			if(nMsg == 1)
			{
				// IDS_GUILD_DISERSE_SEND_MSG02	"정말로 길드를 해산하길 원하십니까?"
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_DISERSE_SEND_MSG02), TYPE_GUILD_CONFIRM_CANCEL, POPUP_GUILD_REALLY_DISPERSE);
			}
		}
		break;
	case 48 :
		{
			if(nMsg == 1)
			{
				sprintf(m_MsgBuf, "mg leave\n");
				guilds_SendMsg();
			}

		}
		break;

	case POPUP_GUILD_RECOMMAND : // POPUP_GUILD_RECOMMAND
		{
			if(nMsg == 1)
			{
				if(!m_szguildName || !m_szrecomName)
					return -1;

				sprintf(m_MsgBuf, "mg recom a %s %s %d\n",m_szguildName, m_szrecomName, 1);
				resetNameBuffer();
			}
			else
			{
				sprintf(m_MsgBuf, "mg recom a %s %s %d\n",m_szguildName, m_szrecomName, 0);
				resetNameBuffer();
			}

			guilds_SendMsg();
		}
		break;
	case 51 :
		{
			if(nMsg == 1)
			{
				CBattleAcceptMember* pBattle =  (CBattleAcceptMember*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_ACCEPTMEMBER);

				if(pBattle)
					pBattle->SendMsg();

				g_pNk2DFrame->GetUIWindow()->OnCloseWindow(WINDOW_GUILD_BATTLE_ACCEPTMEMBER);
			}
		}
		break;

	case 52 :	// 길드 침략전 현황에서 종결 버튼 누를시  뜨는 팝업 메시지 처리
		{
			if(nMsg == 1)
			{
				CBattleInvasionInfo* pInvasionInfo = (CBattleInvasionInfo*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_BATTLE_INVASION_INFO);

				if(pInvasionInfo)
					pInvasionInfo->SendMsg();
			}
		}
		break;
	case 53 :
		{
			sprintf(m_MsgBuf, "mg war cancel res %d %s %s \n",nMsg, m_szTempBuffer1, m_szTempBuffer2 );
			guilds_SendMsg();

		}
		break;
	/*
			case POPUP_GUILD_RANKWAR_INVITE : // 길드 랭킹전 관전 버튼 클릭시 서버 답변 받고 뜨는 팝업.
				{
					if(nMsg == 1)
						pCMyApp->m_pUIMgr->PrepareChangeWorld(TRUE, WORLD_GUILD_WAR, 0);
				}
				break;
	*/
	case POPUP_GUILD_JOIN:
		{
			if( nMsg == 1 )
			{
				CGuildSearch* guildSearch = (CGuildSearch*)g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_GUILD_SEARCH);
				sprintf( m_MsgBuf, "mg request %s\n", guildSearch->Get_SelectGuildName() );
				guilds_SendMsg();
			}
		}
	case POPUP_GUILD_WAR_MOVE:
		break;

	case POPUP_GUILD_REALLY_DISPERSE:
		if( nMsg == 1 )
		{
			sprintf(m_MsgBuf, "mg disband\n");
			guilds_SendMsg();
		}
		break;

	case POPUP_GUILD_RECOM_MEMBER:
		{
			if( nMsg == 1)
			{
				if( strlen(szString) <= 0 )
				{
					//IDS_GUILD_MSG1  [11/13/2008 parkmj] 추천인 이름을 적어주세요
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_MSG1), TYPE_GUILD_CONFIRM, 52);
					//g_pNk2DFrame->InsertPopup("추천인 이름을 적어주세요.", TYPE_GUILD_CONFIRM );
					break;
				}

				sprintf(m_MsgBuf, "mg recom r %s\n", szString);
				guilds_SendMsg();
			}
		}
		break;
	}


	return -1;
}
