#include "stdafx.h"
#include "guild_data.h"

#include "d3dutil.h"
#include "headers.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "NkCharacter.h"
#include "WindowMgr.h"

#include "s_Job_control.h"
#include "s_member_control.h"
#include "s_joinrequest_control.h"
#include "ControlGuildT.h"
#include "ControlGuildM.h"




CGuildData* CGuildData::Create()
{
	return new CGuildData;
}

CGuildData::CGuildData()
{
	m_pNkChar	= NULL;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));

	m_guild_TT_Info.reset();

	Init();
}

CGuildData::~CGuildData()
{
	Reset_guildInfo();
}

void CGuildData::Init()
{
}

int CGuildData::IsWaitlist(guild_member& g_member, int nType)
{
	list_guildmember::iterator	_iter_;

	if(nType == 1)
	{
		for(_iter_ = m_guild_TT_Info.s_guildRecom.begin(); _iter_ != m_guild_TT_Info.s_guildRecom.end(); _iter_++)
		{
			guild_member& member = *_iter_;

			if(member.s_index == g_member.s_index)
			{
				return 1;
			}
		}
	}

	if(nType == 2)
	{
		for(_iter_ = m_guild_TT_Info.s_guildGeneral.begin(); _iter_ != m_guild_TT_Info.s_guildGeneral.end(); _iter_++)
		{
			guild_member& member = *_iter_;

			if(member.s_index == g_member.s_index)
			{
				return 2;
			}
		}
	}

	return 0;

}

BOOL CGuildData::IsInvasion_Attack()
{
	list_guildInvasion::iterator _iter_;

	if( m_guild_TT_Info.s_guildInvasion.empty() )
		return FALSE;

	for(_iter_ = m_guild_TT_Info.s_guildInvasion.begin(); _iter_ != m_guild_TT_Info.s_guildInvasion.end(); _iter_++)
	{
		guild_Invasion& invasion = *_iter_;

		if(invasion.s_Invasion_flag == INVASION_ATTACK)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CGuildData::IsInvasion_Defense()
{
	list_guildInvasion::iterator _iter_;

	if( m_guild_TT_Info.s_guildInvasion.empty() )
		return FALSE;

	for(_iter_ = m_guild_TT_Info.s_guildInvasion.begin(); _iter_ != m_guild_TT_Info.s_guildInvasion.end(); _iter_++)
	{
		guild_Invasion& invasion = *_iter_;

		if(invasion.s_Invasion_flag == INVASION_DEFENSE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int	CGuildData::GetGLevel(int charindex)
{
	list_guildmember::iterator _iter_;
	for(_iter_ = m_guild_TT_Info.s_guildRecom.begin(); _iter_ != m_guild_TT_Info.s_guildRecom.end(); _iter_++)
	{
		guild_member* mem_user = &(*_iter_);

		if(mem_user->s_index == charindex) // 일반길원이다
			return mem_user->s_glevel;
	}

	for(_iter_ = m_guild_TT_Info.s_guildGeneral.begin(); _iter_ != m_guild_TT_Info.s_guildGeneral.end(); _iter_++)
	{
		guild_member* mem_user = &(*_iter_);

		if(mem_user->s_index == charindex) // 일반길원이다
			return mem_user->s_glevel;
	}
	return -1;
}

BOOL CGuildData::CheckInvasionGuild(int guildIndex )
{
	if( guildIndex  == 0)
		return FALSE;

	list_guildInvasion::iterator _iter_;

	if( m_guild_TT_Info.s_guildInvasion.empty() )
		return FALSE;

	for(_iter_ = m_guild_TT_Info.s_guildInvasion.begin(); _iter_ != m_guild_TT_Info.s_guildInvasion.end(); _iter_++)
	{
		guild_Invasion& invasion = *_iter_;

		if(invasion.s_guild_Index == guildIndex)
			return TRUE;
	}

	return FALSE;
}

void CGuildData::guilds_SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CGuildData::Insert_guildInfo(guild_Info& info)
{
	m_guild_TT_Info.s_guildInfo = info;
}

char* CGuildData::GetInvasionAttackName()
{
	list_guildInvasion::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildInvasion.begin(); _iter_ != m_guild_TT_Info.s_guildInvasion.end(); _iter_++)
	{
		guild_Invasion& invasion = *_iter_;

		if(invasion.s_Invasion_flag == INVASION_ATTACK)
		{
			return invasion.s_guild_Name;
		}
	}
	return NULL;
}

t_Info CGuildData::GetInvasionRemainTime()
{
	list_guildInvasion::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildInvasion.begin(); _iter_ != m_guild_TT_Info.s_guildInvasion.end(); _iter_++)
	{
		guild_Invasion& invasion = *_iter_;

		if(invasion.s_Invasion_flag == INVASION_ATTACK)
		{
			return invasion.s_time;
		}
	}
	return t_Info(-1, -1, -1, -1, -1, -1);
}

void CGuildData::Insert_guilddata(guild_member& member)
{
	if( g_pNk2DFrame->GetControlGuild() && g_pNk2DFrame->GetControlGuildManage() && g_pNk2DFrame->GetControlGuildManage() )
	{
		CControlGuild*			pControl    = g_pNk2DFrame->GetControlGuild();
		CJoinRequestControl*	pJoinwait   = g_pNk2DFrame->GetControlGuildManage()->m_pJoinWait;
		CMemberControl*			pMemberlist = g_pNk2DFrame->GetControlGuildManage()->m_pMemberlist;

		switch(member.s_check)
		{
		case CHECK_UPDATE :
			Insert_guildupdate(member);
			pControl->UpdateData(member);
			g_pNk2DFrame->GetControlGuildManage()->m_pJob->Insert_list();
			pMemberlist->SetListAllReset();
			break;
		case CHECK_ADD :
			Insert_guildMember(member);
			pControl->UpdateData(member);
			pMemberlist->Insert_list(member);
			break;
		case CHECK_GENERAL_WAIT_ADD :
			Insert_guildGeneralWait(member);
			pJoinwait->Insert_list(member);
			break;
		case CHECK_RECOMMAND_WAIT_ADD :
			Insert_guildRecomWait(member);
			pJoinwait->Insert_list(member);
			break;
		case CHECK_DELETE :
			{
				Delete_Member(member);
				pControl->Delete_list(member);
				Delete_GeneralWait(member);
				Delete_RecommandWait(member);
				pJoinwait->EraseData(member.s_index);
				//pJoinwait->EraseData();
				//pMemberlist->Delete_list(member);
				pMemberlist->SetListAllReset();
			}
			break;
		}
	}
}


void CGuildData::Insert_guildupdate(guild_member& member)
{
	list_guildmember::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildmember.begin(); _iter_ != m_guild_TT_Info.s_guildmember.end(); _iter_++)
	{
		guild_member* mem_user = &(*_iter_);

		if(mem_user->s_index == member.s_index)
		{
			mem_user->s_connect  = member.s_connect;
			mem_user->s_glevel   = member.s_glevel;
			mem_user->s_index	 = member.s_index;
			mem_user->s_jtype    = member.s_jtype;
			mem_user->s_race	 = member.s_race;
			mem_user->s_waittime = member.s_waittime;
			mem_user->s_check    = member.s_check;

			ZeroMemory(mem_user->s_name, sizeof(mem_user->s_name));
			strcpy(mem_user->s_name, member.s_name);
		}
	}
}



void CGuildData::Member_list_check()
{
	// 포인터 임다..조심..
	m_GeneralMember.clear();
	m_CaptainMember.clear();

	list_guildmember::iterator _iter_;
	for(_iter_ = m_guild_TT_Info.s_guildmember.begin(); _iter_ != m_guild_TT_Info.s_guildmember.end(); _iter_++)
	{
		guild_member* mem_user = &(*_iter_);

		if(mem_user->s_glevel == 1) // 일반길원이다
		{
			m_GeneralMember.push_back(mem_user);
		}
		else if(mem_user->s_glevel == 10 || mem_user->s_glevel == 9 || mem_user->s_glevel == 7 || mem_user->s_glevel == 6 ||
				mem_user->s_glevel == 5  || mem_user->s_glevel == 4 || mem_user->s_glevel == 3 || mem_user->s_glevel == 2 )
		{
			m_CaptainMember.push_back(mem_user);
		}
	}
}

void CGuildData::Delete_RecommandWait(guild_member& member)
{
	list_guildmember::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildRecom.begin(); _iter_ != m_guild_TT_Info.s_guildRecom.end(); _iter_++)
	{
		guild_member* pTemp = &(*_iter_);

		if(pTemp)
		{
			if(pTemp->s_index == member.s_index)
			{
				m_guild_TT_Info.s_guildRecom.erase(_iter_);
				return;
			}
		}
	}
}

void CGuildData::Delete_GeneralWait(guild_member& member)
{
	list_guildmember::iterator _iter_;

	int nSize = m_guild_TT_Info.s_guildGeneral.size();

	for(_iter_ = m_guild_TT_Info.s_guildGeneral.begin(); _iter_ != m_guild_TT_Info.s_guildGeneral.end(); _iter_++)
	{
		guild_member& pTemp = *_iter_;

		if(pTemp.s_index == member.s_index)
		{
			m_guild_TT_Info.s_guildGeneral.erase(_iter_);
			return;
		}
	}

	nSize = m_guild_TT_Info.s_guildGeneral.size();
}

void CGuildData::Delete_Member(guild_member& member)
{
	list_guildmember::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildmember.begin(); _iter_ != m_guild_TT_Info.s_guildmember.end(); _iter_++)
	{
		guild_member& pTemp = *_iter_;

		if(pTemp.s_index == member.s_index)
		{
			m_guild_TT_Info.s_guildmember.erase(_iter_);
			return;
		}
	}
}

void CGuildData::Delete_Invasion(guild_Invasion& Invasion)
{
	list_guildInvasion::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildInvasion.begin(); _iter_ != m_guild_TT_Info.s_guildInvasion.end(); _iter_++)
	{
		guild_Invasion& pTemp = *_iter_;

		if(pTemp.s_guild_Index == Invasion.s_guild_Index)
		{
			m_guild_TT_Info.s_guildInvasion.erase(_iter_);
			return;
		}
	}
}

void CGuildData::Insert_guildMember(guild_member& member)
{
	m_guild_TT_Info.s_guildmember.push_back(member);
	m_guild_TT_Info.s_genericmember++;
}

void CGuildData::Insert_guildInvasion(guild_Invasion& invasion)
{
	m_guild_TT_Info.s_guildInvasion.push_back(invasion);
	m_guild_TT_Info.s_InvasionNum++;
}

void CGuildData::Insert_guildRecomWait(guild_member& member)
{
	m_guild_TT_Info.s_guildRecom.push_back(member);
	m_guild_TT_Info.s_recomwaitmember++;
}

void CGuildData::Insert_guildGeneralWait(guild_member& member)
{
	m_guild_TT_Info.s_guildGeneral.push_back(member);

	int nSize = m_guild_TT_Info.s_guildGeneral.size();
	m_guild_TT_Info.s_generalwaitmember++;
}

void CGuildData::Insert_guildMyInfo(guild_member& member)
{
	m_guild_TT_Info.s_guildMyinfo = member;
}

void CGuildData::Insert_guildnotice(guild_alert& alert)
{
	strcpy(m_guild_TT_Info.s_guildnotice.s_guildnotic, alert.s_guildnotic);
}

void CGuildData::Change_guildnotice(char* szNotice)
{
	strcpy(m_guild_TT_Info.s_guildnotice.s_guildnotic, szNotice);
}

void CGuildData::Change_guildgradename(char *pszname, int nLevel)
{
	strcpy(m_guild_TT_Info.s_guildInfo.s_guildjobname[nLevel], pszname);
}

void CGuildData::Change_guildLevel(int nLevel)
{
	m_guild_TT_Info.s_guildInfo.s_glevel = nLevel;
}

void CGuildData::Reset_guildInfo()
{
	m_guild_TT_Info.s_guildInfo.reset();
}

void CGuildData::Reset_guildMember()
{
	m_guild_TT_Info.s_guildmember.clear();
}

void CGuildData::Reset_guildRecomWait()
{
	m_guild_TT_Info.s_guildRecom.clear();
}

void CGuildData::Reset_guildGeneralWait()
{
	m_guild_TT_Info.s_guildGeneral.clear();
}

void CGuildData::Reset_guildMyInfo()
{
	m_guild_TT_Info.s_guildMyinfo.reset();
}

void CGuildData::Reset_guildNotice()
{
	m_guild_TT_Info.s_guildnotice.reset();
}

void CGuildData::Reset_guildInvasion()
{
	m_guild_TT_Info.s_guildInvasion.clear();
	m_guild_TT_Info.s_InvasionNum = 0;
}

void CGuildData::Reset_guildDataAll()
{
	m_guild_TT_Info.reset();
}

guild_member* CGuildData::SearchMember_charIdx( int nCharIdx )
{
	list_guildmember::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildmember.begin(); _iter_ != m_guild_TT_Info.s_guildmember.end(); _iter_++)
	{
		if(_iter_->s_index == nCharIdx)
			return &(*_iter_);
	}

	return NULL;
}

INVASION_CHECK CGuildData::CheckInvasionGNameType(char* guildname)	 // 길드명으로 검색
{
	list_guildInvasion::iterator _iter_;

	for(_iter_ = m_guild_TT_Info.s_guildInvasion.begin(); _iter_ != m_guild_TT_Info.s_guildInvasion.end(); _iter_++)
	{
		if(!strcmp((*_iter_).s_guild_Name, guildname))
		{
			return (*_iter_).s_Invasion_flag;
		}
	}

	return INVASION_NONE;
}


int CGuildData::GetGuildMasterChaIndex()
{
	list_guildmember::iterator _iter_ = m_guild_TT_Info.s_guildmember.begin();
	for( ; _iter_ != m_guild_TT_Info.s_guildmember.end(); _iter_++ )
	{
		if( _iter_->s_glevel == 10 )
			return _iter_->s_index;
	}

	return -1;
}
