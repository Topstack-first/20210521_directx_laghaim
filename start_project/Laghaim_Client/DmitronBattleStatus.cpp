#include "stdafx.h"
#include "Land.h"
#include "TextOutBox.h"
#include "DmitronBattleStatus.h"
#include "Nk2DFrame.h"
#include "NKCharacter.h"
#include "g_stringmanager.h"
#include "CommonConfig.h"

extern int		g_nCurWorldIdx;				// Current World Index ///인던1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDmitronBattleStatus::CDmitronBattleStatus()
	: ZoneId(0)
	, OwnerPoint(0)
	, ChalengerPoint(0)
	, LastTime(0)
	, OccpTime(0)
	, m_nPosX(0)
	, m_nPosY(0)
{
	memset(OwnerName, 0, sizeof(OwnerName));
	memset(ChalengerName, 0, sizeof(ChalengerName));
	memset(m_ZoneStatus, 0, sizeof(m_ZoneStatus));
}

CDmitronBattleStatus::~CDmitronBattleStatus()
{
	DeleteRes();
}

void CDmitronBattleStatus::LoadRes( )
{
	m_StatusBox1.Init( 12, RGB(255,255,255), 0, 0 );
	m_StatusBox2.Init( 12, RGB(255,255,0), 0, 0 );
}

void CDmitronBattleStatus::DeleteRes()
{
	m_StatusBox1.DeleteRes();
	m_StatusBox2.DeleteRes();
}

int CDmitronBattleStatus::Draw()
{
	if (g_nCurWorldIdx == WORLD_DMITRON_BATTLE)
	{
		int i = 0;
		// 인터페이스 윈도우가 열린 상태인지 체크
		if( g_pNk2DFrame->IsHelpVisible()			|| g_pNk2DFrame->IsInvenVisible()
				|| g_pNk2DFrame->IsPartyVisible()		|| g_pNk2DFrame->IsInterfaceVisible(SCORE)
				|| g_pNk2DFrame->IsSkillVisible()		|| g_pNk2DFrame->IsInterfaceVisible(MAP)
				|| g_pNk2DFrame->IsPetStatusVisible()	|| g_pNk2DFrame->IsInterfaceVisible(QUEST) )
		{
			m_nPosX = g_pNk2DFrame->GetClientWidth() - 274 - 200;
		}
		else
		{
			m_nPosX = g_pNk2DFrame->GetClientWidth() - 200;
		}

		if( CommonConfig::Instance()->GetThaFlag() == true )
		{
			m_nPosX -= 50;
		}

		m_nPosY = 35;

		m_StatusBox1.SetPos(m_nPosX, m_nPosY);
		m_StatusBox1.Draw();

		m_StatusBox2.SetPos(m_nPosX, m_nPosY);
		m_StatusBox2.Draw();


	}
	return -1;
}

void CDmitronBattleStatus::Init()
{
	m_ZoneStatus[0] = 0;
	m_ZoneStatus[1] = 0;
	m_ZoneStatus[2] = 0;
	m_ZoneStatus[3] = 0;
	m_ZoneStatus[4] = 0;

	ZoneId = 0;
	ZeroMemory(OwnerName, sizeof(OwnerName)); // 드미트론 길드 이름
	OwnerPoint = 0;
	ZeroMemory(ChalengerName, sizeof(ChalengerName)); // 도전자(최고 점수)
	ChalengerPoint = 0;	// 도전자가 입힌 데미지
	LastTime = 0;	// 점령 시간
	OccpTime = 0;   // 점령 시간
}

void CDmitronBattleStatus::SetDmitronInfo1(int index, int status)
{
	m_ZoneStatus[index] = status;
}

void CDmitronBattleStatus::SetDmitronInfo2(int index, int status, char* myGuildName, int myGuildPoint,
		char* topGuildName, int topGuildPoint, int lastTime, int elapsedTime)
{
	m_ZoneStatus[index] = status;

	ZoneId = index;
	strcpy(OwnerName, myGuildName);
	OwnerPoint = myGuildPoint;
	strcpy(ChalengerName, topGuildName);
	ChalengerPoint = topGuildPoint;
	LastTime = lastTime;
	OccpTime = elapsedTime;
}


void CDmitronBattleStatus::MakeDrawPopup()
{
	char buffer[256], format[256];
	CMultiText status1;
	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_MYGUILD) );
	strcat(buffer, OwnerName);
	status1.AddString(buffer);

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_MYPOINT) );
	strcat(buffer, itoa(OwnerPoint, format, 10));
	status1.AddString(buffer);

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_TOPGUILD) );
	strcat(buffer, ChalengerName);
	status1.AddString(buffer);

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_TOPPOINT) );
	strcat(buffer, itoa(ChalengerPoint, format, 10));
	status1.AddString(buffer);

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_LASTTIME) );
	sprintf(format, (char*)G_STRING(IDS_GUILD_DAT_SECOND), LastTime);
	strcat(buffer, format);
	status1.AddString(buffer);

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_OCCPTIME) );
	sprintf(format, (char*)G_STRING(IDS_GUILD_DAT_SECOND), OccpTime);
	strcat(buffer, format);
	status1.AddString(buffer);
	for(int i = 0; i < DMITRON_BATTLE_ZONE; ++i)
	{
		if (i != ZoneId)
		{
			strcpy(buffer, CDmitronString::Zone(i) );
			strcat(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_PREFIX1) );
			strcat(buffer, CDmitronString::Battle(m_ZoneStatus[i]) );
			status1.AddString(buffer);
		}
	}
	m_StatusBox1.SetString(&status1);

	CMultiText status2;
	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_MYGUILD) );
	status2.AddString(buffer, RGB(255, 255, 0));

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_MYPOINT) );
	status2.AddString(buffer, RGB(255, 255, 0));

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_TOPGUILD) );
	status2.AddString(buffer, RGB(255, 255, 0));

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_TOPPOINT) );
	status2.AddString(buffer, RGB(255, 255, 0));

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_LASTTIME) );
	status2.AddString(buffer, RGB(255, 255, 0));

	strcpy(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_OCCPTIME) );
	status2.AddString(buffer, RGB(255, 255, 0));

	for(int i = 0; i < DMITRON_BATTLE_ZONE; ++i)
	{
		if (i != ZoneId)
		{
			strcpy(buffer, CDmitronString::Zone(i) );
			strcat(buffer, (char*)G_STRING(IDS_DMITRON_BATTLE_PREFIX1) );
			status2.AddString(buffer, RGB(255, 255, 0));
		}
	}

	m_StatusBox2.SetString(&status2);
}