#include "stdafx.h"
#include "main.h"
#include "Tcpipcon.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "Item.h"
#include "ComboBox.h"
#include "ControlBottom.h"
#include "ControlTeleportItem.h"
#include "ControlWareHouse.h"
#include "LShiftMenu.h"
#include "Nk2DFrame.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "UIMgr.h"
#include "Land.h"
#include "ItemProto.h"
#include "AutoPlay_test.h"
#include "ControlParty.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include <io.h>
#include <algorithm>
#include <atlcoll.h>
#include "NKObjQuadNode.h"

#include "LHUI_OpEvent.h"
#include "g_stringmanager.h"

#include "LH_Decal.h"

#include "PacketSend.h"

#pragma warning( disable : 4800 )
#pragma warning( disable : 4172 )

extern BOOL g_bPath;
extern BOOL g_bDungeon;
extern BOOL g_bNewTerrainDungeon;
extern int	g_nCurWorldIdx;

#define FIRST_ATTK	true
#define FIRST_ITEM	false

#define AUTO_GOGO	true
#define AUTO_STOP	false

#define RANGE_MIN	50.0f
#define RANGE_MAX	250.0f

const DWORD FALIED_RETURN_BASE_TIME 	= 1000*10;
const DWORD FALIED_ATTACK_TIME			= 1000*10;
const DWORD FALIED_DECIDE_TARGET_TIME	= 1000*10;
const DWORD FALIED_PICKITEM_TIME		= 1000*10;

const DWORD FAILED_STATE_TIME			= 1000*60;

const DWORD ACTION_DELAY				= 500;
const DWORD QUICK_DELAY					= 500;
const DWORD INPEACE_DELAY				= 300;
const DWORD USING_STASH_DELAY			= 400;
const DWORD MOVE_TICK					= 400;

const float TEMP_DIST					= 15.0f;


DWORD CalcRatio( float v1, float v2 )
{
	return (DWORD)(v1 / v2 * 100.0f);
}

CAutoPlay::CAutoPlay()
	: m_initialized(false)
	, m_dwChangeLimitTime(0)
	, m_pTemporaryItem(NULL)
	, m_bSpecialMob(false)
	, m_bUsePotion(false)
	, m_perom_global_cool(0)
	, m_dwAvoidCurTime(0)
	, m_bAppleEmpty(false)
	, m_bUseApplebag(false)
	, m_bReturn(false)
	, m_bUseBuy(false)
	, m_bUsedPetWalkMode(false)
	, m_yWalkMode(0)
	, m_fEscapeX(0)
	, m_fEscapeZ(0)
	, m_dwEscapeTime(0)
{
	memset(m_dwSpecialPotionCurTime, 0, sizeof(m_dwSpecialPotionCurTime));
	memset(m_fSkillPercent, 0, sizeof(m_fSkillPercent));


	Init(false);
}

CAutoPlay::~CAutoPlay()
{
}

void CAutoPlay::Init_variable()
{
	m_dwLastestTime = timeGetTime();
	m_bAuto = AUTO_STOP;
	m_bAttk = AUTO_GOGO;
	m_bItem	= AUTO_GOGO;
	m_bMove = AUTO_STOP;
	m_bSkll = AUTO_GOGO;

	m_bFirst = FIRST_ATTK;

	m_bHealth = false;
	m_bMana = false;
	m_bStamina = false;
	m_bEPower = false;

	m_wHealthLimitTime = POTION_LIMIT_TIME;
	m_wManaLimitTime = POTION_LIMIT_TIME;
	m_wStaminaLimitTime = POTION_LIMIT_TIME;
	m_wEPowerLimitTime = POTION_LIMIT_TIME;

	m_dwHealthCurTime = timeGetTime();
	m_dwManaCurTime = timeGetTime();
	m_dwStaminaCurTime = timeGetTime();
	m_dwEPowerCurTime = timeGetTime();

	m_fHealthPercent = 50.0f;
	m_fManaPercent = 50.0f;
	m_fStaminaPercent = 50.0f;
	m_fEPowerPercent = 50.0f;

	m_dwHealthKey = 0;				// quick slot number.
	m_dwManaKey = 1;
	m_dwStaminaKey = 2;
	m_dwEPowerKey = 3;
	m_wReturn = 0;
	m_uActive = 0;
	// [3/17/2009 D.K ] : 팻 호출기 관련.
	m_bCallerEmpty = false;

	m_pSelectedMob = NULL;
	m_wCurSKill = 0;

	m_bItemRepair = false;
	m_bWalkToArea = false;
	m_bWareHouse = false;
	m_fAttkRange = 500.0f;
	m_fItemRange = 100.0f;
	m_fMoveRange = 10.0f;
	m_fStayRange = 500.0f;
	m_pTargetItem = NULL;
	m_pairPoslist.clear();
	m_vecPickItemList.clear();
	m_vecFirstTarget.clear();
	m_vecAvoidMob.clear();
	m_vecAvoidChar.clear();

	ZeroMemory( m_szFileName, sizeof(m_szFileName) );

	m_dwGameStatus = ATTACK;
	m_pSelectedMob = NULL;
	m_wWalkPosCount = 0;
	m_wTempCount = 0;
	m_dwGameMode = GAMEMODE_FULL_PACKAGE;
	m_dwGameAttkMode = ATTACKMOD_HOLD_AREA_ATTACK;
	m_pTemporaryMob = NULL;
	m_dwTemporaryTime = 0L;
	m_bOnPickItem = false;
	m_dwSubSkillTime = 0L;
	m_lastMoveTime = 0L;
	m_pSpecialMob = NULL;
	m_pCurrentMob = NULL;

	m_bUsed_Trans = false;
	m_bUseSkill = false;
	ZeroMemory( m_bSkillUsed , sizeof(DWORD)*MAX_SKILL_COUNT );

	m_perom_global_cool = 0;
}

// 상태출력용
void CAutoPlay::CheckMode()
{
	if( m_dwGameMode == GAMEMODE_FULL_PACKAGE )
		g_pNk2DFrame->AddChatStr((char *)G_STRING(IDS_USED_FULL), -1 );
	else if( m_dwGameMode == GAMEMODE_SKILL_POTION )
		g_pNk2DFrame->AddChatStr((char *)G_STRING(IDS_USED_HALF), -1 );
	else if ( m_dwGameMode == GAMEMODE_POTION_ONLY )
		g_pNk2DFrame->AddChatStr((char *)G_STRING(IDS_USED_POTION), -1 );
}

void CAutoPlay::StartLagrushMsg()
{
	char buf[256] = {0,};
	switch( m_dwGameMode )
	{
	case GAMEMODE_FULL_PACKAGE:
		sprintf( buf , G_STRING(IDS_LRUSE_FULL));
		break;
	case GAMEMODE_SKILL_POTION:
		sprintf( buf , G_STRING(IDS_LRUSE_HALF) );
		break;
	case GAMEMODE_POTION_ONLY:
		sprintf( buf , G_STRING(IDS_LRUSE_POTION) );
		break;
	}
	g_pNk2DFrame->AddChatStr(buf, -1 );
}

void CAutoPlay::EndLagrushMsg()
{
	char buf[256] = {0,};
	switch( m_dwGameMode )
	{
	case GAMEMODE_FULL_PACKAGE:
		sprintf( buf , G_STRING(IDS_LHSTRING1788) );
		break;
	case GAMEMODE_SKILL_POTION:
		sprintf( buf , G_STRING(IDS_LHSTRING1789) );
		break;
	case GAMEMODE_POTION_ONLY:
		sprintf( buf , G_STRING(IDS_LHSTRING1790) );
		break;
	}
	g_pNk2DFrame->AddChatStr(buf, -1 );
}

void CAutoPlay::Init(bool playing)
{
	m_dwLastestTime = timeGetTime();
	m_bAuto = AUTO_STOP;
	m_bAttk = AUTO_GOGO;
	m_bItem	= AUTO_GOGO;
	m_bMove = AUTO_STOP;
	m_bSkll = AUTO_GOGO;

	m_bFirst = FIRST_ATTK;

	m_bHealth = false;
	m_bMana = false;
	m_bStamina = false;
	m_bEPower = false;

	m_wHealthLimitTime = POTION_LIMIT_TIME;
	m_wManaLimitTime = POTION_LIMIT_TIME;
	m_wStaminaLimitTime = POTION_LIMIT_TIME;
	m_wEPowerLimitTime = POTION_LIMIT_TIME;

	m_dwHealthCurTime = timeGetTime();
	m_dwManaCurTime = timeGetTime();
	m_dwStaminaCurTime = timeGetTime();
	m_dwEPowerCurTime = timeGetTime();

	m_fHealthPercent = 50.0f;
	m_fManaPercent = 50.0f;
	m_fStaminaPercent = 50.0f;
	m_fEPowerPercent = 50.0f;

	m_dwHealthKey = 0;				// quick slot number.
	m_dwManaKey = 1;
	m_dwStaminaKey = 2;
	m_dwEPowerKey = 3;
	m_wReturn = 0;
	m_uActive = 0;
	// [3/17/2009 D.K ] : 팻 호출기 관련.
	m_bCallerEmpty = false;

	m_pSelectedMob = NULL;
	m_wCurSKill = 0;

	m_bItemRepair = false;
	m_bWalkToArea = false;
	m_bWareHouse = false;
	m_fAttkRange = 500.0f;
	m_fItemRange = 100.0f;
	m_fMoveRange = 10.0f;
	m_fStayRange = 500.0f;
	m_pTargetItem = NULL;
	m_pairPoslist.clear();
	m_vecPickItemList.clear();
	m_vecFirstTarget.clear();
	m_vecAvoidMob.clear();
	m_vecAvoidChar.clear();

	ZeroMemory( m_szFileName, sizeof(m_szFileName) );

	m_dwGameStatus = ATTACK;
	m_pSelectedMob = NULL;
	m_wWalkPosCount = 0;
	m_wTempCount = 0;
	m_dwGameMode = GAMEMODE_FULL_PACKAGE;
	m_dwGameAttkMode = ATTACKMOD_HOLD_AREA_ATTACK;
	m_pTemporaryMob = NULL;
	m_dwTemporaryTime = 0L;
	m_bOnPickItem = false;
	m_dwSubSkillTime = 0L;
	m_lastMoveTime = 0L;
	m_pSpecialMob = NULL;
	m_pCurrentMob = NULL;
	m_bBuyCallSign = false;

	m_bUsed_Trans = false;
	m_bUseSkill = false;
	ZeroMemory( m_bSkillUsed , sizeof(DWORD)*MAX_SKILL_COUNT );
	m_loaded_setting = false;

	if( playing )
	{
		ReLoadData();
		SetPos( g_pRoh->m_wx, g_pRoh->m_wz );
	}
}

char strTemp[128] = {0,};
char* CAutoPlay::changeSpecialWord( char* szString )
{
	memset(strTemp, 0, sizeof(strTemp));
	char temp[50];
	strcpy( strTemp, szString );
	int i=0;
	while(strTemp[i]!='\0')
	{
		switch(strTemp[i])
		{
		case '\\':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"0",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '/':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"1",1);
			strcpy(strTemp+i+2,temp);
			break;
		case ':':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"2",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '*':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"3",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '?':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"4",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '"':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"5",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '<':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"6",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '>':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"7",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '|':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"8",1);
			strcpy(strTemp+i+2,temp);
			break;
		// [4/16/2009 D.K ] : . 버그 해결.
		case '.':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"9",1);
			strcpy(strTemp+i+2,temp);
			break;
		}
		i++;
	}

	return strTemp;
}


bool CAutoPlay::SetUserName()
{
	char szPath[128] = {0,};
	strcpy( szPath, AH_CFG_DIR );
	strcat( szPath, changeSpecialWord( g_pRoh->m_pstrName ) );
	if( _access( szPath, 0 ) == -1 )
	{
		strcpy( m_szFileName, "" );
		return false;
	}
	else
	{
		strcat( szPath, AH_CFG_FILE_NAME );
	}
	strcpy( m_szFileName, szPath );
	return true;
}
void CAutoPlay::LoadWalkPosList()
{
	m_pairPoslist.clear();
	char szRootPath[128] = {0,};
	char szPath[128] = {0,};
	strcpy( szRootPath, AH_CFG_DIR );
	strcat( szRootPath, changeSpecialWord( g_pRoh->m_pstrName ) );

	int zone_index = pCMyApp->GetCurWorld();
	if( zone_index == 20 )
		zone_index = 19;


	sprintf( szPath , "%s/WalkPos%d.txt" , szRootPath, zone_index );

	FILE *fp = fopen( szPath, "rb" );
	if( fp == NULL )
		return;

	int size = filelength( fileno( fp ) );
	char* pTemp = new char[size+1];
	if( pTemp == NULL )
		return;

	ZeroMemory( pTemp, sizeof(char) * size+1 );

	fgets( pTemp, size, fp );
	char *p;
	p = strtok( pTemp, "/" );
	if( p == NULL )
	{
		delete [] pTemp;
		return;
	}

	while( p != NULL )
	{
		POSPAIR pos;
		sscanf( p, "%f#%f", &pos.first, &pos.second );

		m_pairPoslist.push_back( pos );

		p = strtok( NULL, "/" );
	}

	delete [] pTemp;
	fclose( fp );
	return;
}

void CAutoPlay::LoadWalkPosListTemp()
{
	m_pairPoslist_temp.clear();
	char szPath[128] = {0,};
	strcpy( szPath, AH_CFG_DIR );
	strcat( szPath, changeSpecialWord( g_pRoh->m_pstrName ) );
	strcat( szPath, "/WalkPos.txt" );
	FILE *fp = fopen( szPath, "rb" );
	if( fp == NULL )
		return;

	int size = filelength( fileno( fp ) );
	char* pTemp = new char[size+1];
	if( pTemp == NULL )
		return;

	ZeroMemory( pTemp, sizeof(char) * size+1 );

	fgets( pTemp, size, fp );
	char *p;
	p = strtok( pTemp, "/" );
	if( p == NULL )
	{
		delete [] pTemp;
		return;
	}

	while( p != NULL )
	{
		POSPAIR pos;
		sscanf( p, "%f#%f", &pos.first, &pos.second );
		m_pairPoslist_temp.push_back( pos );
		p = strtok( NULL, "/" );
	}

	delete [] pTemp;
	fclose( fp );
	return;
}

void CAutoPlay::SaveWalkPosList( float x, float z )
{
	char szRootPath[128] = {0,};
	char szPath[128] = {0,};
	strcpy( szRootPath, AH_CFG_DIR );
	strcat( szRootPath, changeSpecialWord( g_pRoh->m_pstrName ) );

	int zone_index = pCMyApp->GetCurWorld();
	if( zone_index == 20 )
		zone_index = 19;

	sprintf( szPath , "%s/WalkPos%d.txt" , szRootPath, zone_index );

	FILE *fp = fopen( szPath , "ab" );
	if( fp == NULL )
		return;

	fprintf( fp, "%f#%f/", x, z );

	fclose( fp );

}

void CAutoPlay::SaveWalkPosList()
{
	char szRootPath[128] = {0,};
	char szPath[128] = {0,};
	strcpy( szRootPath, AH_CFG_DIR );
	strcat( szRootPath, changeSpecialWord( g_pRoh->m_pstrName ) );


	int zone_index = pCMyApp->GetCurWorld();
	if( zone_index == 20 )
		zone_index = 19;

	sprintf( szPath , "%s/WalkPos%d.txt" , szRootPath, zone_index );
	FILE *fp = fopen( szPath, "w" );
	if( fp == NULL )
		return;

	for( int i = 0; i < m_pairPoslist.size(); i++ )
	{
		fprintf(fp, "%f#%f/", m_pairPoslist[i].first, m_pairPoslist[i].second );
	}

	fclose( fp );

}

void CAutoPlay::LoadWarningList()
{
	m_vecFirstTarget.clear();
	char szPath[128] = {0,};
	strcpy( szPath, AH_CFG_DIR );
	strcat( szPath, "Warning.txt" );
	m_szWaringMobList = "";
	FILE *fp = fopen( szPath, "rb" );
	if( fp == NULL )
	{
		return;
	}

	int size = filelength( fileno( fp ) );
	if( size == 0 )
		return;
	char* pTemp = new char[size];
	if( pTemp == NULL )
		return;

	ZeroMemory( pTemp, size );

	fgets( pTemp, size, fp );
	char *p;
	p = strtok( pTemp, "/" );
	if( p == NULL )
	{
		delete [] pTemp;
		return;
	}

	char pt[128];

	while( p != NULL )
	{
		sprintf( pt, "%s%d", p, strlen(p) );
		m_szWaringMobList += pt;
		// [5/14/2009 D.K ] : 추가
		for( int i = 0; i < g_pMobDataLoading->m_nNpcNum; i++ )
		{
			if( strcmp( (char*)GET_NPC_NAME_VNUM(g_pMobDataLoading->m_pMobData[i].nVnum), p ) == 0 )
			{
				m_vecFirstTarget.push_back( g_pMobDataLoading->m_pMobData[i].nVnum );
				break;
			}
		}
		p = strtok( NULL, "/" );
	}

	delete [] pTemp;
	fclose( fp );
	return;
}

void CAutoPlay::LoadItemList()
{
	m_szItemList = "";
	FILE* fp = fopen("./ahcfg/ItemCheck.txt", "r");
	if( fp == NULL )
	{
		LoadItemListImpl("Item.txt");
		return;
	}

	BOOL bLoad = FALSE;
	BOOL bLoad2 = FALSE;
	fscanf(fp, "%d\n%d", &bLoad, &bLoad2);

	if( bLoad == TRUE )
	{
		LoadItemListImpl("Item.txt");
	}
	if( bLoad2 == TRUE )
	{
		LoadItemListImpl("Item2.txt");
	}

	fclose(fp);
}

void CAutoPlay::LoadItemListImpl(char* filename)
{
	char buf[MAX_PATH] = {0,};
	sprintf(buf, "./ahcfg/%s", filename);

	FILE *fp = fopen( buf, "rb" );
	if( fp == NULL )
	{
		return;
	}

	int size = filelength( fileno( fp ) );
	if( size == 0 )
		return;
	char* pTemp = new char[size];
	if( pTemp == NULL )
		return;

	fgets( pTemp, size, fp );

	char *p;
	p = strtok( pTemp, "/" );
	if( p == NULL )
	{
		delete [] pTemp;
		return;
	}
	char pt[128];

	while( p != NULL )
	{
		sprintf( pt, "%s%d", p, strlen(p) );
		m_szItemList += pt;
		// [5/14/2009 D.K ] : 새로 추가.
		for( int i = 0; i < g_ItemList.m_ItemCount; i++ )
		{
			const char * name = GET_ITEM_NAME_VNUM(g_ItemList.m_ItemArray[i].m_Vnum);
			if( strcmp( GET_ITEM_NAME_VNUM(g_ItemList.m_ItemArray[i].m_Vnum), p ) == 0 )
			{
				m_vecPickItemList.push_back( g_ItemList.m_ItemArray[i].m_Vnum );
				break;
			}
		}
		p = strtok( NULL, "/" );
	}

	delete [] pTemp;
	fclose( fp );
}

void CAutoPlay::LoadAvoidUserList()
{
	m_szAvoidUserList = "";
	FILE *fp = fopen( "./ahcfg/AvoidUser.txt", "rb" );
	if( fp == NULL )
	{
		return;
	}

	int size = filelength( fileno( fp ) );
	if( size == 0 )
		return;
	char* pTemp = new char[size];
	if( pTemp == NULL )
		return;

	ZeroMemory( pTemp, size );

	fgets( pTemp, size, fp );
	char *p;
	p = strtok( pTemp, "/" );
	if( p == NULL )
	{
		delete [] pTemp;
		return;
	}

	char pt[128];

	while( p != NULL )
	{
		sprintf( pt, "%s%d", p, strlen(p) );
		m_szAvoidUserList += pt;
		// [5/14/2009 D.K ] : 추가.
		m_vecAvoidChar.push_back( STRPAIR( p, strlen( p ) ) );
		p = strtok( NULL, "/" );
	}

	delete [] pTemp;
	fclose( fp );
	return;
}

void CAutoPlay::LoadAvoidMobList()
{

	m_szAvoidMobList = "";
	FILE *fp = fopen( "./ahcfg/AvoidMob.txt", "rb" );
	if( fp == NULL )
	{
		return;
	}

	int size = filelength( fileno( fp ) );
	if( size == 0 )
		return;
	char* pTemp = new char[size];
	if( pTemp == NULL )
		return;

	ZeroMemory( pTemp, size );

	fgets( pTemp, size, fp );
	char *p;
	p = strtok( pTemp, "/" );
	if( p == NULL )
	{
		delete [] pTemp;
		return;
	}

	char pt[128];

	while( p != NULL )
	{
		sprintf( pt, "%s%d", p, strlen(p) );
		m_szAvoidMobList += pt;
		for( int i = 0; i < g_pMobDataLoading->m_nNpcNum; i++ )
		{

			if( strcmp( (char*)GET_NPC_NAME_VNUM(g_pMobDataLoading->m_pMobData[i].nVnum), p ) == 0 )
			{
				m_vecAvoidMob.push_back( g_pMobDataLoading->m_pMobData[i].nVnum );
				break;
			}
		}
		p = strtok( NULL, "/" );
	}

	delete [] pTemp;
	fclose( fp );
	return;
}

void CAutoPlay::LoadSetting()
{
	if( !SetUserName() )
	{
		// [4/23/2009 D.K ] : QA팀 요청. 이름이 같지 않다면 무조건 스톱.
// 		pCMyApp->m_pUIMgr->m_bAutoHunt = FALSE;
//		pCMyApp->m_pHeroAttGoMob = NULL;
//		m_pTargetItem = NULL;
//		m_uActive = 0;
//		m_pSelectedMob = NULL;
// 		g_pNk2DFrame->InsertPopup( "설정 내용이 없습니다. 라그러시 설정을 확인해 주세요." , TYPE_NOR_OK, 1 );
		_OnInterrupt(LagRush_Interrupt__NotSetting);
		return;
	}

	DWORD temp_race = (DWORD)GetPrivateProfileInt( AH_CFG_MAIN , AH_CFG_MAIN_RACE, 255 , m_szFileName );
	if( temp_race == 255 )
	{
// 		pCMyApp->m_pUIMgr->m_bAutoHunt = FALSE;
// 		pCMyApp->m_pHeroAttGoMob = NULL;
// 		m_pTargetItem = NULL;
// 		m_uActive = 0;
// 		m_pSelectedMob = NULL;
// 		g_pNk2DFrame->InsertPopup( "라그러쉬 버젼이 낮습니다. 라그러시 설정을 다시 해주시기 바랍니다." , TYPE_NOR_OK, 1 );

		_OnInterrupt(LagRush_Interrupt__Version_Low);

		return;
	}

	if( temp_race != g_pRoh->m_Race )
	{
		_OnInterrupt(LagRush_Interrupt__Tribe_NotMatch);
		return;
	}

	char temp[128];
	int i;
	m_bAuto		= (bool)GetPrivateProfileInt( AH_CFG_MAIN, AH_CFG_MAIN_AUTO, 0, m_szFileName );
	m_bAttk		= (bool)GetPrivateProfileInt( AH_CFG_MAIN, AH_CFG_MAIN_ATTK, 0, m_szFileName );
	m_bSkll		= (bool)GetPrivateProfileInt( AH_CFG_MAIN, AH_CFG_MAIN_SKLL, 0, m_szFileName );
	m_bItem		= (bool)GetPrivateProfileInt( AH_CFG_MAIN, AH_CFG_MAIN_ITEM, 0, m_szFileName );
	m_fStayRange= (float)GetPrivateProfileInt( AH_CFG_MAIN, AH_CFG_MAIN_ATTKRANGE, 500, m_szFileName );
	if( m_fStayRange < 30.0f )
	{
		m_fStayRange = 30.0f;
	}
	else if( m_fStayRange > 500.0f )
	{
		m_fStayRange = 500.0f;
	}
	m_fAttkRange = m_fStayRange;
	// [4/13/2009 D.K ] :
	// default is full package mode.
	m_dwGameMode= GetPrivateProfileInt( AH_MODE, AH_MODE_TYPE,	GAMEMODE_FULL_PACKAGE, m_szFileName );


	//-------------------------------------------------------------------------
	// potions.
	//-------------------------------------------------------------------------
	// m_bUsePotion= (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_USE,	0, m_szFileName );
	m_bHealth			= (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_VITAL		, 0, m_szFileName );
	m_bMana				= (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_MAGIC		, 0, m_szFileName );
	m_bStamina			= (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_STAMI		, 0, m_szFileName );
	m_bEPower			= (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_ELECT		, 0, m_szFileName );
	m_bSpecialPotion[0] = (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBPOTION1	, 0, m_szFileName );
	m_bSpecialPotion[1] = (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBPOTION2	, 0, m_szFileName );
	m_bSpecialPotion[2] = (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBPOTION3, 0, m_szFileName );
	m_bSpecialPotion[3] = (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBPOTION4, 0, m_szFileName );
	m_bSpecialPotion[4] = (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBPOTION5, 0, m_szFileName );
	m_bSpecialPotion[5] = (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBPOTION6, 0, m_szFileName );
	m_bSubArms			= (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUB		, 0, m_szFileName );
	m_bAdiaSummon		= (bool)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_ADIASUM	, 0, m_szFileName );
	m_bUsedCallSign		= (BOOL)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_CALLLIMIT	, 0, m_szFileName );


	m_fHealthPercent	= (float)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_VTLMT, 0, m_szFileName );
	m_fManaPercent		= (float)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_MGLMT, 0, m_szFileName );
	m_fStaminaPercent	= (float)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_STLMT, 0, m_szFileName );
	m_fEPowerPercent	= (float)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_ELLMT, 0, m_szFileName );
	m_wSpecialPotionLimitTime[0] = ((DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION1LMT, 600, m_szFileName )) * 1000;
	m_wSpecialPotionLimitTime[1] = ((DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION2LMT, 600, m_szFileName )) * 1000;
	m_wSpecialPotionLimitTime[2] = ((DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION3LMT, 600, m_szFileName )) * 1000;
	m_wSpecialPotionLimitTime[3] = ((DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION4LMT, 600, m_szFileName )) * 1000;
	m_wSpecialPotionLimitTime[4] = ((DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION5LMT, 600, m_szFileName )) * 1000;
	m_wSpecialPotionLimitTime[5] = ((DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION6LMT, 600, m_szFileName )) * 1000;
	m_dwSubArmsLimit	= (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBLMT,1, m_szFileName );
	m_fAdiaSummonPercent= (float)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_ADSUMLMT, 0, m_szFileName );


	m_dwHealthKey		= (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_VTKEY, 0, m_szFileName );
	m_dwManaKey			= (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_MGKEY, 0, m_szFileName );
	m_dwStaminaKey		= (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_STKEY, 0, m_szFileName );
	m_dwEPowerKey		= (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_ELKEY, 0, m_szFileName );
	m_dwSpecialPotionKey[0] = (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION1KEY, 2, m_szFileName );
	m_dwSpecialPotionKey[1] = (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION2KEY, 3, m_szFileName );
	m_dwSpecialPotionKey[2] = (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION3KEY, 2, m_szFileName );
	m_dwSpecialPotionKey[3] = (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION4KEY, 3, m_szFileName );
	m_dwSpecialPotionKey[4] = (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION5KEY, 2, m_szFileName );
	m_dwSpecialPotionKey[5] = (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SPPOTION6KEY, 3, m_szFileName );
	m_dwSubArmsKey		= (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_SUBKEY,0, m_szFileName );
	m_dwAdiaSummonKey	= (DWORD)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_ADKEY, 0, m_szFileName );
	m_uCallKeyMapping	= (UINT)GetPrivateProfileInt( AH_CFG_PTN, AH_CFG_PTN_CALLKEY,	5, m_szFileName );
	//-------------------------------------------------------------------------
	// potions.
	//-------------------------------------------------------------------------



	//-------------------------------------------------------------------------
	// skills.
	//-------------------------------------------------------------------------
	m_bRotateSkill = (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_ROTATE , 0, m_szFileName );

	ZeroMemory( m_bSkillArray, sizeof( bool )*FUNCTION_NUM);
	for( i = 0; i < FUNCTION_NUM; i ++ )
	{
		sprintf( temp, AH_CFG_SKL_NO, i+1 );
		m_bSkillArray[i] = (bool)GetPrivateProfileInt( AH_CFG_SKL, temp, 0, m_szFileName );
		sprintf( temp, AH_CFG_SKL_DIR, i+1 );
		m_bSkillDirArray[i] = GetPrivateProfileInt( AH_CFG_SKL, temp, 0, m_szFileName );
		sprintf( temp, AH_CFG_SKL_TIME, i+1 );
		m_wSkillLimitTime[i] = (DWORD)GetPrivateProfileInt( AH_CFG_SKL, temp, 0, m_szFileName ) * 1000;
		if( m_wSkillLimitTime[i] < 1000 )
		{
			m_wSkillLimitTime[i] = 1000;
		}
		sprintf( temp, AH_CFG_SKL_KEY, i+1 );
		m_dwSkillKeyArray[i] = (DWORD)GetPrivateProfileInt( AH_CFG_SKL, temp, 0, m_szFileName );
	}

	sprintf( temp, AH_CFG_SKL_NO, 0 );
	m_bLeftSkill = (bool)GetPrivateProfileInt( AH_CFG_SKL, temp, 0, m_szFileName );
	sprintf( temp, AH_CFG_SKL_KEY, 0 );
	m_dwLeftSkill = (DWORD)GetPrivateProfileInt( AH_CFG_SKL, temp, 0, m_szFileName );






	m_bShield			= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHL, 0, m_szFileName );
	m_bRecovery			= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_REC, 0, m_szFileName );
	m_bBloodyMana		= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_BLO, 0, m_szFileName );
	m_bVempTouch		= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_VMP, 0, m_szFileName );

	m_dwShieldTime		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHTIME, 0, m_szFileName )* 1000;
	m_fRecoveryPercent	= (float)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_RCLMT, 0, m_szFileName );
	m_fBloodyManaPercent= (float)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_BLLMT, 0, m_szFileName );
	m_fVempTouchPercent	= (float)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_VMLMT, 0, m_szFileName );

	m_dwShieldKey		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHKEY, 0, m_szFileName );
	m_dwRecoveryKey		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_RCKEY, 0, m_szFileName );
	m_dwBloodyManaKey	= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_BLKEY, 0, m_szFileName );
	m_dwVempTouchKey	= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_VMKEY, 0, m_szFileName );

	m_dwShieldRepeat	= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHREPEAT, 0, m_szFileName )+1;

	m_use_dsys			= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_DSYS, 0, m_szFileName );
	m_dsys_key			= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_DSYSKEY, 0, m_szFileName );

	m_bshield2			= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHL2, 0, m_szFileName );
	m_dwShieldKey2		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHKEY2, 0, m_szFileName );
	m_dwShieldTime2		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHTIME2, 0, m_szFileName ) * 1000;

	m_bshield3			= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHL3, 0, m_szFileName );
	m_dwShieldKey3		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHKEY3, 0, m_szFileName );
	m_dwShieldTime3		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHTIME3, 0, m_szFileName ) * 1000;

	m_bshield4			= (bool)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHL4, 0, m_szFileName );
	m_dwShieldKey4		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHKEY4, 0, m_szFileName );
	m_dwShieldTime4		= (DWORD)GetPrivateProfileInt( AH_CFG_SKL, AH_CFG_SKL_SHTIME4, 0, m_szFileName ) * 1000;

	//-------------------------------------------------------------------------
	// skills.
	//-------------------------------------------------------------------------


	//-------------------------------------------------------------------------
	// hunt
	//-------------------------------------------------------------------------

	m_hunt_move			= (bool)GetPrivateProfileInt( AH_CFG_HNT, AH_CFG_HNT_MOVE, 0, m_szFileName );

	m_party_confirm		= (bool)GetPrivateProfileInt( AH_CFG_HNT, AH_CFG_HNT_PARTY, 0, m_szFileName );
	m_bAvoid			= (bool)GetPrivateProfileInt( AH_CFG_HNT, AH_CFG_HNT_AVOID, 0, m_szFileName );
	m_wAvoidLimitTime	= (DWORD)GetPrivateProfileInt( AH_CFG_HNT, AH_CFG_HNT_AVDLMT, 1, m_szFileName ) * 60000;
	//-------------------------------------------------------------------------
	// hunt
	//-------------------------------------------------------------------------



	//-------------------------------------------------------------------------
	// move
	//-------------------------------------------------------------------------
	m_wReturn = 0;
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_HRETURN, 0, m_szFileName ), AH_RETURN_HEALTH );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_MRETURN, 0, m_szFileName ), AH_RETURN_MANA );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_SRETURN, 0, m_szFileName ), AH_RETURN_STAMINA );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_ERETURN, 0, m_szFileName ), AH_RETURN_EPOWER );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_ARETURN, 0, m_szFileName ), AH_RETURN_SUBARMS );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_URETURN, 0, m_szFileName ), AH_RETURN_SUMMONS );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_VRETURN, 0, m_szFileName ), AH_RETURN_HP );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_PRETURN, 0, m_szFileName ), AH_RETURN_APPLE );
	m_wReturn |= CALC_BIT( GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_CRETURN, 0, m_szFileName ), AH_RETURN_CALLER );

	m_dwReturnCount[0] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_HCOUNT, 0, m_szFileName );
	m_dwReturnCount[1] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_MCOUNT, 0, m_szFileName );
	m_dwReturnCount[2] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_SCOUNT, 0, m_szFileName );
	m_dwReturnCount[3] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_ECOUNT, 0, m_szFileName );
	m_dwReturnCount[4] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_ACOUNT, 0, m_szFileName );
	m_dwReturnCount[5] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_UCOUNT, 0, m_szFileName );
	m_dwReturnCount[6] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_VCOUNT, 0, m_szFileName );
	m_dwReturnCount[5] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_UCOUNT, 0, m_szFileName );
	m_dwReturnCount[6] = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_VCOUNT, 0, m_szFileName );
	m_dwReturnCount[7] = 0; // 사과개수
	m_dwReturnCount[8] = 0; // 라비용호출기

	m_dwReturnKey = (DWORD)GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_RETURNKEY, 5, m_szFileName );

	m_bUseTrans			=  !((BOOL)	GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_TRANSE_USE, 1, m_szFileName ));
	m_dwTransportingKey = (DWORD)	GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_TRANSE_KEY, 4, m_szFileName );
	m_dwTransportingPos = (DWORD)	GetPrivateProfileInt( AH_CFG_RTN, AH_CFG_RTN_TRANSE_POS, 0, m_szFileName );

	//-------------------------------------------------------------------------
	// move
	//-------------------------------------------------------------------------



	//-------------------------------------------------------------------------
	// buy
	//-------------------------------------------------------------------------
	m_wBuyFromShop = 0;
	for( i = 0; i < AH_QUICKSLOT_COUNT; i++ )
	{
		sprintf( temp, AH_CFG_BUY_CHECK, i+1 );
		m_wBuyFromShop |= CALC_BIT( GetPrivateProfileInt( AH_CFG_BUY, temp, 0, m_szFileName ), i );
		sprintf( temp, AH_CFG_BUY_COUNT, i+1 );
		m_dwBuyFromShopCount[i] = (DWORD)GetPrivateProfileInt( AH_CFG_BUY, temp, 0, m_szFileName );

		sprintf( temp, AH_CFG_BUY_KEEP, i+1 );
		m_dwKeepCount[i] = (DWORD)GetPrivateProfileInt( AH_CFG_BUY, temp, 0, m_szFileName );
	}
	m_bBuyApple = GetPrivateProfileInt( AH_CFG_BUY, AH_CFG_BUY_BUYAPPLE, 0, m_szFileName );
	m_bBuyCallSign = GetPrivateProfileInt( AH_CFG_BUY, AH_CFG_BUY_CALL, 0, m_szFileName );

	m_uApplesCount = GetPrivateProfileInt( AH_CFG_BUY, AH_CFG_BUY_APPLECNT, 0, m_szFileName );
	m_uCallersCount = GetPrivateProfileInt( AH_CFG_BUY, AH_CFG_BUY_CALLCNT, 0, m_szFileName );

	m_bItemRepair		= (BOOL)	GetPrivateProfileInt( AH_CFG_BUY,	 AH_CFG_BUY_REPAIR, 0, m_szFileName );
	m_bWareHouse		= (BOOL)	GetPrivateProfileInt( AH_CFG_BUY,	 AH_CFG_BUY_WARE,	0, m_szFileName );
	//-------------------------------------------------------------------------
	// buy
	//-------------------------------------------------------------------------


	//-------------------------------------------------------------------------
	// pet
	//-------------------------------------------------------------------------
	m_bPetActive		= (BOOL)GetPrivateProfileInt( AH_CFG_PET, AH_CFG_PET_ACTIVE,	0, m_szFileName );
	m_bHuntWithMaster	= (BOOL)GetPrivateProfileInt( AH_CFG_PET, AH_CFG_PET_TOGETHER,	0, m_szFileName );
	m_yHuntingType		= (BYTE)GetPrivateProfileInt( AH_CFG_PET, AH_CFG_PET_HUNTMOD,	0, m_szFileName );
	m_bUsedAvoid		= (BOOL)GetPrivateProfileInt( AH_CFG_PET, AH_CFG_PET_AVOID,		0, m_szFileName );
	m_uPetAvoidVital	= (UINT)GetPrivateProfileInt( AH_CFG_PET, AH_CFG_PET_AVOIDPER, 50, m_szFileName );
	//-------------------------------------------------------------------------
	// pet
	//-------------------------------------------------------------------------


	// 중복키 제외
// 	if( !m_bPetActive && m_bBuyApple )
// 		m_bBuyApple = false;
// 	if( !m_bPetActive && m_bUsedCallSign )
// 		m_bUsedCallSign = false;

	// 스킬셋팅 유무 처리
	m_bUseSkill = false;
	for( i = 0; i < 8; i ++ )
	{
		m_dwSkillCurTime[i] = 0;

		if( m_bSkillArray[i] )
		{
			int down_key = m_dwSkillKeyArray[i]>0?m_dwSkillKeyArray[i]-1:7;
			int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
			if( cur_skill < 0 )
			{
				m_bSkillArray[i] = false;
				continue;
			}

			m_bUseSkill = true;
		}
	}
	m_dwGameStatus = ATTACK;

	m_loaded_setting = true;
}

//-----------------------------------------------------------------------------
// Name : OnReturn
// Date : 2009.10.19
// Desc : 귀환처리
//-----------------------------------------------------------------------------
bool CAutoPlay::OnReturn( UINT status )
{
	return true;
}

//-----------------------------------------------------------------------------
// Name : OnPotion
// Date : 2009.10.19
// Desc : 물약 사용하기
//-----------------------------------------------------------------------------
bool CAutoPlay::OnPotion()
{
	return true;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnAdiaSummon()
{
	return false;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnSubArms()
{
	return false;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnShield()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// [10/23/2008 D.K ] : 차후 통합 예정인 코드. 결국 똑같기 때문에 하나로 뭉쳐도 될듯 하다.
bool CAutoPlay::OnRecovery()
{
	return false;
}


//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnBloody()
{
	return true;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnVempTouch()
{
	return true;
}


//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
DWORD CAutoPlay::GetCurShopNo( int _CurWorld )
{
	switch ( _CurWorld )
	{
	case 0:
		return 66;						// 섹시녀.
	case 1:
	case 2:
	case 5:
		return 64;						// 던젼 상인.
	case 3:
		return 140;						// 하간 알라만다.
	case 4:
		return 93;						// 의사 아저씨.
	case 6:
		return 238;						// 파로스 잡화상인.
	case 7:
	case 8:
	case 9:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
		return 320;						// 몸짱녀.
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnSkll( int _x, int _y )
{
	return true;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnAvoid()
{
	if( pCMyApp->IsPeaceArea() )
		return false;

	char temp[128] = {0,};
	char szMessage[128] = {0,};
	CNkMob* pMob = NULL;
	CNkCharacter* pChar = NULL;
	LHMobMap_it prMob;
	LHCharacterMap_it prChar;

	if( !m_bAvoid )
		return true;

	if( g_pRoh->IsDead() || m_szAvoidMobList.size() == 0 )
		return false;

	for( prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++ )
	{
		pMob = (*prMob).second;
		sprintf( temp, "%s%d", pMob->GetMobName(), strlen( pMob->GetMobName() ) );

		float distance = DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz );

		if( strstr( m_szAvoidMobList.c_str(), temp ) && distance < m_fStayRange )
		{
			sprintf(szMessage, G_STRING(IDS_LHSTRING1782), pMob->GetMobName() );
			g_pNk2DFrame->AddChatStr( szMessage, -1 );
			OnReturn( AVOID );
			m_dwAvoidCurTime = timeGetTime();
			return false;
		}
	}

	// user search
	for( prChar = pCMyApp->m_pUIMgr->m_character_map.begin(); prChar != pCMyApp->m_pUIMgr->m_character_map.end(); prChar++ )
	{
		pChar = (*prChar).second;
		sprintf( temp, "%s%d", pChar->m_pstrName, strlen( pChar->m_pstrName ) );
		float distance = DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, pChar->m_wx, pChar->m_wz );
		if( strstr( m_szAvoidUserList.c_str(), temp ) && distance < m_fStayRange )
		{
			sprintf(szMessage, G_STRING(IDS_LHSTRING1782), pMob->GetMobName() );

			g_pNk2DFrame->AddChatStr( szMessage, -1 );
			OnReturn( AVOID );
			m_dwAvoidCurTime = timeGetTime();
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay::OnAttk()
{
	return true;
}

CNkMob*		CAutoPlay::_selectTarget(bool& _isSpecial, INT npcCode /* = -1  */)
{
	CNkMob* pMob						= NULL;
	CNkMob* selectedMob					= NULL;
	CNkMob* specialMob					= NULL;
	LHMobMap_it	pMobItr;
	float	curDist						= m_fStayRange;
	float	shortestDist				= m_fStayRange;
	_isSpecial = false;


	for( pMobItr = pCMyApp->m_pUIMgr->m_mob_map.begin(); pMobItr != pCMyApp->m_pUIMgr->m_mob_map.end(); pMobItr++ )
	{
		pMob = (*pMobItr).second;

		if( npcCode > 0 )
		{
			if( npcCode == pMob->m_nMobNum )
				return pMob;
			else
				continue;
		}

		if (pMob->IsDead() || (pMob->m_Flag & NPC_PROCEDURE) || (pMob->m_Flag & NPC_SHOPKEEPER) || (pMob->m_Flag & NPC_GUARD) ||
				(pMob->m_Flag & NPC_FISH) || (pMob->m_Flag & NPC_SUMMON) || (pMob->m_Flag & NPC_NORADER) ||
				pMob->m_nMobNum == MONSTER_CONTROL_DAMAGE || pMob->m_nMobNum == MONSTER_CONTROL_GUARD ||
				(pMob->m_Flag & NPC_QUEST) || pMob->m_nMobNum == 226
				|| !( pMob->m_nMobNum < 420 || pMob->m_nMobNum > 424 )
				|| !strcmp( "Snow Cat", pMob->GetMobName() ) )
			continue;

		if( pMob->CheckRenderHolloweenMob(pMob->m_nMobNum) == false )
		{
			continue;
		}

		if( !pCMyApp->m_pUIMgr->IsNotBlocked( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz ) )
			continue;

		curDist = abs( DistVecToVec( D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz), D3DVECTOR( pMob->m_wx, pMob->m_wy, pMob->m_wz ) ) );
		if( curDist > 160 )
			continue;

		// 선타격 몹 찾기.
		std::vector<int>::iterator itr = find( m_vecFirstTarget.begin(), m_vecFirstTarget.end(), pMob->m_nMobNum );
		if( itr != m_vecFirstTarget.end() )
		{
			_isSpecial = true;
			printf("Special mob Selected : %s\n", pMob->GetMobName() );
			return pMob;
		}

		float dist = abs( DistVecToVec( D3DVECTOR( m_fStayPosX, 0, m_fStayPosZ), D3DVECTOR( pMob->m_wx, pMob->m_wy, pMob->m_wz ) ) );

		// 몹과 시작 좌표의 거리가 최대 사냥 거리를 넘으면 몹을 선택하지 않는다.
		if( dist > m_fStayRange )
			continue;

		if( curDist <= shortestDist )
		{
			selectedMob = pMob;
		}
	}

	return selectedMob;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc : 아이템 집어먹기
//-----------------------------------------------------------------------------
bool CAutoPlay::OnItem()
{
	return false;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc : 창고 이용
//-----------------------------------------------------------------------------
bool CAutoPlay::OnNPCStash()
{
	return false;
}

void CAutoPlay::StartProc(DWORD dwGameMode)
{

}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc : 라그러쉬 종료
//-----------------------------------------------------------------------------
void CAutoPlay::StopProc(int type)
{
	if( pCMyApp->m_pUIMgr->m_bAutoHunt && g_pTcpIp )
		LH_SEND_NET_MSG( "lr end\n" );

	pCMyApp->m_pUIMgr->m_bAutoHunt = FALSE;
	pCMyApp->m_pHeroAttGoMob = NULL;
	m_pTargetItem = NULL;
	m_uActive = 0;
	m_pSelectedMob = NULL;

	EndLagrushMsg();

	g_pNk2DFrame->GetNewEvent()->SetEvent( 7 , 0 );
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc : 상점이용
//-----------------------------------------------------------------------------
bool CAutoPlay::OnNPCShop()
{
	return false;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc : 퀵슬롯으로 아이템 이동
//-----------------------------------------------------------------------------
bool CAutoPlay::SetQuickItem( UINT uType )
{
	return true;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.19
// Desc : 전송?
//-----------------------------------------------------------------------------
bool CAutoPlay::OnTransItem()
{
	return false;
}

// [12/2/2008 D.K ] : 각 중심 좌표. 나중에 창고라든지 다른 지역관련 움직임에 대한 좌표 지정.
void CAutoPlay::GetCurCenterPos ( int _CurWorld, float& x, float& z )
{
	switch ( _CurWorld )
	{
	case 0:		// 데카르디(로스트렐름)
		x = 6600.0f;
		z = 5700.0f;
		return;
	case 1:		// 데카렌 던젼
		x = 8000.0f;
		z = 1900.0f;
		return;
	case 2:		// 데카둔 던젼
		if( g_pRoh->m_wx < 1000.0f && g_pRoh->m_wz < 1000.0f )		// 1st floor.
		{
			x = 230.0f;
			z = 670.0f;
		}
		else if( g_pRoh->m_wx < 2500.0f && g_pRoh->m_wz > 2500.0f )	// 2nd floor.
		{
			x = 1735.0f;
			z = 4930.0f;
		}
		else if( g_pRoh->m_wx > 2500.0f && g_pRoh->m_wz < 2500.0f )	// 3rd floor.
		{
			x = 3020.0f;
			z = 1740.0f;
		}
		else if( g_pRoh->m_wx > 2500.0f && g_pRoh->m_wz > 2500.0f )	// 4th floor.
		{
			x = 4890.0f;
			z = 3860.0f;
		}
		return;
	case 3:		// 샤일론.
		x = 7445.0f;				// 7400 11550
		z = 11670.0f;
		return;
	case 4:		// 라그라미아(스타트존)
		x = 6348.0f;				// 6348 5213
		z = 5213.0f;
		return;
	case 5:		// 시루스트 던젼
		x = 100.0f;				// 7400 11550
		z = 220.0f;
		return;
	case 6:		// 파로스(천공의 성)
		if( g_pRoh->m_wx > 6800.0f && g_pRoh->m_wz > 7400.0f )
		{
			x = 11710.0f;				// 7400 11550
			z = 11545.0f;
		}
		else
		{
			x = 1360.0f;				// 7400 11550
			z = 1420.0f;
		}

		return;
	case 7:		// 화이트혼
		x = 8890.0f;				// 7400 11550
		z = 7945.0f;
		return;
	case 8:		// 제누스 레버너티(화이트혼 던젼)
		x = 815.0f;				// 7400 11550
		z = 460.0f;
		if( g_pRoh->m_wx > 3600.0f && g_pRoh->m_wz < 11000.0f )
		{
			x = 3990.0f;
			z = 2300.0f;
		}

		return;
	case 9:		// 드미트론
		x = 10835.0f;				// 7400 11550
		z = 6620.0f;
		return;
	case 10:	// 뫼비우스 아레나(메트릭스)
		x = 0.0f;
		z = 0.0f;
		return;
	case 11:	// 길드 랭킹존
		x = 0.0f;
		z = 0.0f;
		return;
	case 12:	// 드미트론 점령전
		x = 0.0f;
		z = 0.0f;
		return;
	case 13:	// 튜란(불칸존)	// 320
		x = 700.0f;
		z = 950.0f;
		return;
	case 14:	// 발카리야(카이립톤)	// 320
		x = 1240.0f;
		z = 11250.0f;
		return;
	case 15:	// 바르샤(에이디아)	// 320
		x = 1000.0f;
		z = 1800.0f;
		return;
	case 16:	// 퀘이즈(휴먼)	// 320
		x = 1190.0f;
		z = 11200.0f;
		return;
	case 17:	// 베어그리드(하이브리더) // 320
		x = 980.0f;
		z = 1350.0f;
		return;
	case 18:	// 마르보덴(최종보스)
		x = 0.0f;
		z = 0.0f;
		return;
	case 19:	// 없음.
		x = 0.0f;
		z = 0.0f;
		return;
	}
	return;
}

bool CAutoPlay::OnMove( bool _mode )
{
	static bool isOutofRange = false;
	float stayrange = m_fStayRange;

	if( m_wTempCount < 5 )
	{
		m_wTempCount++;
		return false;
	}

	m_wTempCount = 0;

	if( m_bSpecialMob && !_mode )
		return true;

	if( DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, m_fStayPosX, m_fStayPosZ ) < 20.0f )
	{
		isOutofRange = false;
		m_dwGameStatus = ATTACK;
		m_pTemporaryMob = NULL;
		m_pSelectedMob = NULL;
		m_wTempCount = 0;
	}

	if( m_bOnPickItem )
		stayrange += 150.0f;

	if( ( DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, m_fStayPosX, m_fStayPosZ ) >= stayrange ) || isOutofRange )
	{
		isOutofRange = true;

		m_pSelectedMob = NULL;
		float XPos = m_fStayPosX;
		float ZPos = m_fStayPosZ;
		if( !pCMyApp->m_pUIMgr->IsNotBlocked( g_pRoh->m_wx, g_pRoh->m_wz, m_fStayPosX, m_fStayPosZ ) )
		{
			int x = rand() % 200 - 100;
			int z = rand() % 200 - 100;
			XPos += x;
			ZPos += z;
		}

		_Move( XPos, ZPos );

		m_wTempCount = 0;
		return false;
	}

	return true;
}

bool	CAutoPlay::_Move(float _x, float _z )
{
	char commOut[512] = {0,};
	DWORD curTime = timeGetTime();
	if (curTime >= m_lastMoveTime + 600 && g_pRoh->IsShown() )
	{
		printf("Test Module & move to attack position.\n" );
		if ( g_bPath )
		{
			// 위치를 보간해준다. 왜 타일의 가운데에 서라고.
			int x, z;
			float fx, fz;
			fx = _x;			// 원좌표 보관.
			fz = _z;
			x = (int) (_x / 10);		// 좌표 보정.
			z = (int) (_z / 10);
			_x = (x * 10) + 5.0f;
			_z = (z * 10) + 5.0f;

			if ( (g_pRoh->m_wx == _x)
					&& (g_pRoh->m_wz == _z) )
			{
				g_pRoh->m_toDir = (float) atan2((fz-_x),(fx-_z));
				int dY = (int) g_pRoh->m_wy;
				int dir = (int) (g_pRoh->m_toDir*10000);

				sprintf(commOut, "place %d %d %d %d %d\n", (int) _x, (int) _z, dY, dir, 0);
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(commOut);

				return true;
			}
			else
			{
				pCMyApp->MoveHeroT(_x, _z);
			}
		}
		else
		{
			pCMyApp->MoveHero(_x, _z);
		}

		m_lastMoveTime = curTime;
	}
	return false;
}
//===============================================================
// [4/24/2009 D.K ] : if blocked character, called this func.
void	CAutoPlay::OnEscapeBlocked()
{
	if( timeGetTime() - m_dwEscapeTime < 1000 )
		return;

	m_dwEscapeTime = timeGetTime();

	D3DVECTOR normalVec;
	D3DVECTOR targetPos( m_fStayPosX, 0, m_fStayPosZ );
	D3DVECTOR CurrentPos( m_fEscapeX, 0, m_fEscapeZ );
	targetPos = targetPos - CurrentPos;
	// [4/24/2009 D.K ] : 역벡터.
	targetPos = -( Normalize( targetPos ) * 20.0f );

	if( ( DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, m_fEscapeX, m_fEscapeZ ) >= 20.0f ) )
	{
		targetPos = CrossProduct( targetPos, D3DVECTOR( 0, 1, 0 ) );
	}

	if( ( DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, m_fEscapeX, m_fEscapeZ ) >= 40.0f ) )
	{
		m_dwGameStatus = ATTACK;
		return;
	}

	if( g_bPath )
		pCMyApp->MoveHeroT( g_pRoh->m_wx - targetPos.x, g_pRoh->m_wz - targetPos.z );
	else
		pCMyApp->MoveHero( g_pRoh->m_wx - targetPos.x, g_pRoh->m_wz - targetPos.z );

}
//////////////////////////////////////////////////////////////////////////
// [11/18/2008 D.K ] : 펫 관련
UINT GetType( DWORD _Mode )
{
	switch ( _Mode )
	{
	case 0:
		return MYPET_MODE_COMBAT;
	case 1:
		return MYPET_MODE_FOLLOW;
	case 2:
		return MYPET_MODE_PICKUP;
	case 3:
		return MYPET_MODE_WIMPY;
	case 4:
		return MYPET_MODE_SUPP;
	case 5:
		return MYPET_MODE_RANGECOMBAT;
	case 6:
		return MYPET_MODE_GUARD;
	case 7:
		return MYPET_MODE_MACRO;
	}
	return MYPET_MODE_DEFAULT;
}

bool CAutoPlay::OnMoveOnWalk()
{
	return false;
}

int click = 0;
bool CAutoPlay::OnMoveInPeace()
{
	return false;
}

void CAutoPlay::ShowInfo()
{

}

bool CAutoPlay::OnPet()
{
	char szTemp[128] = {0,};
	CItem* pItem = NULL;
	BYTE count = 0;
	static bool charge = false;
	static bool	pickApple = false;
	static DWORD EleapsCnt = 0;
	static DWORD usedTime = timeGetTime();

	if( !m_bPetActive )
		return false;

	if( !pCMyApp->m_pMyPet )
	{
		if( timeGetTime() - usedTime > 1000 )
		{
			pCMyApp->m_pUIMgr->UseQuickItem( m_uCallKeyMapping );
			usedTime = timeGetTime();

			if( m_bCallerEmpty && g_pRoh->m_QuickCount[m_uCallKeyMapping] < 2 )	// 라비용 호출기가 없다. 갯수 지정하고 싶으면 설정하면 되나. 귀찮다..=_=
			{
				m_dwGameStatus = RETURN_TO_CITY;
				printf("Call Pet Caller.\n" );
				return false;
				//printf("Time is time %d\n", usedTime );
			}
		}
		return true;
	}

	float dist = DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, pCMyApp->m_pMyPet->m_wx, pCMyApp->m_pMyPet->m_wz );
	if( m_bUsedCallSign && dist > 1000.0f )
	{
		pCMyApp->m_pUIMgr->UseQuickItem( m_uCallKeyMapping );

		if( m_bCallerEmpty && g_pRoh->m_QuickCount[m_uCallKeyMapping] < 2 )	// 라비용 호출기가 없다. 갯수 지정하고 싶으면 설정하면 되나. 귀찮다..=_=
		{
			m_dwGameStatus = RETURN_TO_CITY;
			printf("Call Pet Caller.\n" );
			return false;
		}

		return true;
	}

	if( m_bUsedAvoid && ( pCMyApp->m_pMyPet->m_Vital / pCMyApp->m_pMyPet->m_MaxVital ) * 100 < m_uPetAvoidVital )
		ChangeMode( MYPET_MODE_WIMPY );
	else
		ChangeMode( GetType(m_yHuntingType) );

	int max_apple = 1;
	if( g_MyPetStatus.m_pBag )
		max_apple = 6;

	if( g_MyPetStatus.food_count < max_apple )
	{
		if( g_pRoh->GetCountInPack(618) )
		{
			CItem * pAppleItem = NULL;
			for( int i=0 ; i<PACK_NUM ; i++ )
			{
				pAppleItem = g_pRoh->GetInvenItem(i , 618 , true);
				if( pAppleItem )
				{
					char msg_buf[64] = {0,};
					sprintf( msg_buf , "moving_apple %d %d\n" , i , pAppleItem->m_Index );
					LH_SEND_NET_MSG( msg_buf );
					return true;
				}
			}
		}
	}

	return false;
}

void CAutoPlay::ChangeMode( DWORD _Mode )
{
	if( _Mode == g_MyPetStatus.mode )
		return;

	char szCmd[128] = {0,};
	sprintf( szCmd, "p_mode %d\n", (int)_Mode );
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage( szCmd );

	if( pCMyApp->m_pMyPet )
	{
		if( _Mode == MYPET_MODE_MACRO )
			g_pNk2DFrame->GetLShiftMenu()->m_pControlMacro->StartMacroMode();
		else
		{
			g_MyPetStatus.SetMode( _Mode );
			g_MyPetStatus.m_bUseMacroMode = FALSE;
		}
	}
}

bool CAutoPlay::OnWait()
{
	return true;
}
//////////////////////////////////////////////////////////////////////////

bool CAutoPlay::OnSubSkll()
{
	return false;
}

bool CAutoPlay::MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 오토중엔 지정되면 안되도록...
	char szMessage[128] = {0,};
	if( uMsg == WM_KEYDOWN )
	{
		/*if( g_nCurWorldIdx == WORLD_FORLORN_S3 )
		{
			return false;
		}*/

		if( wParam == VK_ADD )
		{
			LoadWalkPosList();
			POSPAIR pos;
			pos.first = g_pRoh->m_wx;
			pos.second = g_pRoh->m_wz;
			m_pairPoslist.push_back( pos );
			int num = m_pairPoslist.size();

			float _x, _z;
			pCMyApp->m_pUIMgr->ConvertWorldPos(g_pRoh->m_wx, g_pRoh->m_wz , _x, _z );
			sprintf( szMessage, G_STRING(IDS_LHSTRING1791), num, (int)_x, (int)_z );
			g_pNk2DFrame->AddChatStr( szMessage, -2 );
			SaveWalkPosList( g_pRoh->m_wx, g_pRoh->m_wz );

			return true;
		}
		else if( wParam == VK_SUBTRACT )
		{
			LoadWalkPosList();

			if( m_pairPoslist.empty() )
			{
				g_pNk2DFrame->AddChatStr( (char *)G_STRING(IDS_LHSTRING1785), -1 );
				return true;
			}
			int posInList = 0;
			std::vector<POSPAIR>::iterator itr = m_pairPoslist.begin();
			std::vector<POSPAIR>::iterator last_itr = m_pairPoslist.begin();
			for( ; itr!=m_pairPoslist.end() ; itr++ )
			{
				last_itr = itr;
			}

			if( last_itr == m_pairPoslist.end() )
				return false;


			float _x, _z;
			int num = m_pairPoslist.size();
			pCMyApp->m_pUIMgr->ConvertWorldPos( ((POSPAIR)*(last_itr)).first , ((POSPAIR)*(last_itr)).second , _x, _z );
			sprintf( szMessage, G_STRING(IDS_LHSTRING1792), num, (int)_x, (int)_z );
			g_pNk2DFrame->AddChatStr( szMessage, -1 );
			m_pairPoslist.erase( last_itr );
			SaveWalkPosList();

			return true;
		}
	}

	return false;
}

//===============================================================
// [4/13/2009 D.K ] : mode select. and execute that mode.
void CAutoPlay::AutoProc()
{
}

//===============================================================
// [4/13/2009 D.K ] : full package mode.
void CAutoPlay::FullProc()
{
}

//===============================================================
// [4/13/2009 D.K ] : potion only mode.
void	CAutoPlay::PotionProc()
{
}

//===============================================================
// [4/15/2009 D.K ] : potion and skill support mode.
void	CAutoPlay::PotionSkillProc()
{
}


bool CAutoPlay::IsFullInvenInAllPack()
{
	if( g_pRoh->IsFullInvenInAllPack() )
	{
		StopProc( AutoPlay_StopProc__FullInven );
		return true;
	}

	return false;
}

void CAutoPlay::Render(bool activate, bool edit_mode)
{
	if( edit_mode && !activate )
	{
		if( m_pairPoslist_temp.empty() )
			return;

		int walk_size = m_pairPoslist_temp.size();
		printf( "CAutoPlay::Render %d\n" , walk_size );
		for( int i=0 ; i<walk_size ; i++ )
		{
			m_pairPoslist_temp[i].first;
			m_pairPoslist_temp[i].second;
		}
	}

	// startpoint....
}

//=============================================================================
//	Name : CAutoPlay_EX 시작
//=============================================================================
CAutoPlay_EX::CAutoPlay_EX()
	:	CAutoPlay()
	,	m_change_state(false)
	,	m_prev_state(NONE_STATE)
	,	m_current_state(NONE_STATE)
	,	m_use_return(false)
	,	m_changed_delay(0)
	,	m_pick_item_time(0)
	,	m_last_quick_tick(0)
	,	m_last_action_tick(0)
	,	m_last_attack_tick(0)
	,	m_found_npc(false)
	,	m_npc_index(-1)
	,	m_buy_x(0)
	,	m_buy_z(0)
	,	m_shop_state(ShopState__Repair)
	,	m_repair_count(0)
	,	m_picked_apple(false)
	,	m_using_shield_count(0)
	,	m_using_shield_time(0)
	,	m_skill_slot(0)
	,	m_last_decide_target(0)
	,	m_move_to_base_tick(0)
	,	m_pPrevSelectedMob(NULL)
	,	m_running(false)
	,	m_attack_try_count(0)
	,	m_movetobase_try_count(0)
	,	m_last_callsign_tick(0)
	,	m_dead(false)
	,	m_stash_sendmsg(0)
	,	m_apple_sendmsg(0)
	,	m_using_surport(false)
	,	m_need_surport(-1)
	,	m_need_change(false)
	,	m_start_exp(0)
	,	m_start_laim(0)
	,	m_kill_count(0)
	,	m_using_shield_time2(0)
	,	m_using_shield_time3(0)
	,	m_using_shield_time4(0)
	, m_olddt(0)
	, m_dead_time(0)
	, m_stash_state(StashState__Pop)
	, m_success_movetobase(false)
	, m_move_to_base_dir(0)
	, m_global_attack_time(false)
	, m_recived_stash_item(false)
	, m_shop_sendmsg(0)
	, m_used_limit_delay(0)
	, m_used_skill_delay(0)
	, m_surport_sub(false)
	, m_devine_stone(false)
{
	ZeroMemory(m_temp_quick , sizeof(TEMP_QUICK)*QUICK_SLOT_NUM*QUICK_SLOT_PAGE );

	m_pDecal = NULL;
}

//-----------------------------------------------------------------------------
// Name :
//-----------------------------------------------------------------------------
CAutoPlay_EX::~CAutoPlay_EX()
{
	SAFE_DELETE(m_pDecal);
}



//-----------------------------------------------------------------------------
// Name :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_IsPeaceArea()
{
	return pCMyApp->IsPeaceArea();
}

//=============================================================================
// Name : _Polling_Potion
//=============================================================================
void CAutoPlay_EX::_Polling_Potion(DWORD dt)
{
	if( g_pRoh->IsDead() )
		return;

	if( _IsPeaceArea() )
		return;

	_OnPotion(dt);
	_OnPotion_Spacial(dt);
}


//=============================================================================
// Name : _Polling_PotionSkill
//=============================================================================
void CAutoPlay_EX::_Polling_PotionSkill(DWORD dt)
{
	if( g_pRoh->IsDead() )
		return;

	_OnPotion(dt);
	_OnPotion_Spacial(dt);

	if( _CommonSkill(dt) )
		return;


	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if( !m_pSelectedMob )
	{
		WORD pick_entity = pCMyApp->m_pUIMgr->WhatColorOnPick(point.x, point.y);
		if( pick_entity == 0 )
			return;

		// mob 인가?
		CNkMob * pMob = pCMyApp->m_pUIMgr->GetMobByColorKey(pick_entity);
		if( pMob )
		{
			if( (pMob->m_Flag & NPC_PROCEDURE) || (pMob->m_Flag & NPC_SHOPKEEPER) || (pMob->m_Flag & NPC_GUARD) ||
					(pMob->m_Flag & NPC_FISH) || (pMob->m_Flag & NPC_SUMMON) || ( (pMob->m_Flag & NPC_NORADER) && !( pMob->m_nMobNum == 678 || pMob->m_nMobNum == 679 || pMob->m_nMobNum == 834 || pMob->m_nMobNum == 835 || pMob->m_nMobNum == 836 || pMob->m_nMobNum == 837) ) ||
					pMob->m_nMobNum == MONSTER_CONTROL_DAMAGE || pMob->m_nMobNum == MONSTER_CONTROL_GUARD ||
					(pMob->m_Flag & NPC_QUEST) || pMob->m_nMobNum == 226
					|| !( pMob->m_nMobNum < 420 || pMob->m_nMobNum > 424 )
					|| !strcmp( "Snow Cat", pMob->GetMobName() ) )
				return;

			if( pMob->CheckRenderHolloweenMob(pMob->m_nMobNum) == false )
			{
				return;
			}

			_ResetAttack();
			m_pSelectedMob = pMob;
			m_attack_try_count = 0;


			D3DVECTOR vWorldPos, vScreenPos;
			D3DMATRIX matPipe;
			ZeroMemory( &matPipe, sizeof( D3DMATRIX ) );

			int mx = 0, my = 0;
			vWorldPos = D3DVECTOR( m_pSelectedMob->m_wx, m_pSelectedMob->m_wy, m_pSelectedMob->m_wz );
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipe );
			D3DMath_VectorMatrixMultiply( vScreenPos, vWorldPos, matPipe );

			mx = vScreenPos.x;
			my = vScreenPos.y;
			if( mx < 0 )	mx = 0;
			if( my < 0 )	my = 0;


			pCMyApp->m_pUIMgr->SetPickColor(m_pSelectedMob->m_wColorIdx);
			if( !pCMyApp->m_pUIMgr->m_b_Attack )
				pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );
		}
	}
	else
	{
		if( _OnVampTouch(dt) )
			return;

// 		if( pCMyApp->m_pHeroAttGoMob && m_pSelectedMob && pCMyApp->m_pHeroAttGoMob->m_nVirId != m_pSelectedMob->m_nVirId )
// 		{
// 			m_attack_try_count = 0;
// 			_ResetAttack();
// 			pCMyApp->StopHero();
// 			g_pRoh->SetMotionState( CHA_STAND );
//
// 			m_pSelectedMob = pCMyApp->m_pHeroAttGoMob;
//
// 			return;
// 		}

		if( _OnAttack_PotionSkill(dt) )
			return;

		m_attack_try_count++;

		AddLog( "[_OnFieldProc:Under attack: Attempt to attack...]" , 1 );

		if( m_pSelectedMob && m_pSelectedMob->IsDead() || m_attack_try_count > 5 )
		{
			m_attack_try_count = 0;
			_ResetAttack();
			pCMyApp->StopHero();
			g_pRoh->SetMotionState( CHA_STAND );
		}
	}
}

//=============================================================================
// Name : _Polling_Full
//=============================================================================
void CAutoPlay_EX::_Polling_Full(DWORD dt)
{
	if( _OnChangedProc(dt) )
		return;

	if( g_pRoh->IsDead() && m_current_state != REVIVAL )
	{
		AddLog( "[_Polling_Full] Player Dead : Status change request" , 1 );
		m_dead_time = 0;
		pCMyApp->StopHero();
		_ChangeState(REVIVAL);

		return;
	}

	if( m_current_state == REVIVAL )
	{
		if( m_dead_time == 0 )
			m_dead_time = dt;

		if( _CheckElapsedTime(dt , m_dead_time , 4000 ) )
		{
			AddLog( "[_Polling_Full] Player Dead : Resurrection" , 1 );
			m_dead_time = 0;

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage("revival\n");

			g_pNk2DFrame->DeleteAllPopup();
			_AddChat("Resurrected.");

			g_pRoh->m_DeadFrame = -1;
			_ChangeState( NONE_STATE );
		}
		else
		{
			AddLog( "[_Polling_Full] Player Dead :Resurrection Pending" , 1 );
		}

		return;
	}

	if( AVOID == m_current_state )
	{
		if( m_dwAvoidCurTime == 0 )
			m_dwAvoidCurTime = dt;

		if( _CheckElapsedTime( dt , m_dwAvoidCurTime , m_wAvoidLimitTime ) )
		{
			AddLog( "[_Polling_Full] Player Avoid : Dodge Time End Status Change" , 1 );
			m_dwAvoidCurTime = 0;
			_ChangeState( NONE_STATE );
		}
		else
		{
			static DWORD avoid_tick = 0;
			if(avoid_tick == 0)
				avoid_tick = dt;

			if( dt - avoid_tick < 3000 )
				return;

			avoid_tick = 0;
			AddLog( "[_Polling_Full] Player Avoid : Avoiding" , 1 );
		}
		return;
	}

	if( NONE_STATE == m_current_state )
	{
		if( _IsPeaceArea() )
		{
			_ChangeState( WAIT_IN_CITY );
		}
		else
		{
			_ChangeState( ATTACK );
		}
		return;
	}

	if( m_current_state == RETURN_TO_CITY )
	{
		AddLog( "[_Polling_Full:Try to return to town]" , 1 );
		_ChangeState( NONE_STATE );
		return;
	}

	if( GO_TO_AREA_ON_WALK == m_current_state )
	{
		if( false == _OnMoveToField(dt) )
			return;

		AddLog( "[_Polling_Full:Arrive on foot]" , 1 );
		_ChangeState( NONE_STATE );
		return;
	}

	bool rv = false;
	if( _IsPeaceArea() )
	{
		if( !_OnTownProc(dt) )
		{
			AddLog( "[_Polling_Full:_OnTownProc] failure" , 1 );
			_ChangeState( NONE_STATE );
		}
	}
	else
	{
		if( !_OnFieldProc(dt) )
		{
			AddLog( "[_Polling_Full:_OnFieldProc] failure" , 1 );
			_ChangeState( NONE_STATE );
		}
	}
}

//-----------------------------------------------------------------------------
// Name :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnChangedProc(DWORD dt)
{
	if( m_change_state )
	{
		if( m_changed_delay == 0 )
			m_changed_delay = dt;

		if( m_prev_state == ITEM && m_current_state == ATTACK )
		{
			if( dt - m_changed_delay < 300 )
				return true;

			m_changed_delay = 0;
			m_change_state = false;
		}
		else
		{
			if( dt - m_changed_delay < 500 )
				return true;

			m_changed_delay = 0;
			m_change_state = false;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnTownProc(DWORD dt)
{
	static DWORD olddt = 0;
	if( olddt == 0 )
		olddt = dt;

	if( WAIT_IN_CITY == m_current_state )
	{
		_ReturnToCity();
		AddLog( "[_OnTownProc:Changing warehouse status]" , 1 );
		_ChangeState( WARE );
		return true;
	}
	else if( WARE == m_current_state )
	{
		if( State_RV__Wait == _OnStashItem(dt) )
		{
			AddLog( "[_OnTownProc:Using warehouse]" , 1 );
			return true;
		}

		AddLog( "[_OnTownProc:Change store use]" , 1 );
		_ChangeState( SHOP_BUY );
		return true;
	}
	else if( SHOP_BUY == m_current_state )
	{
		if( State_RV__Wait == _OnShopItem(dt) )
		{
			AddLog( "[_OnTownProc:In store]" , 1 );
			return true;
		}

		if( true == _OnPotion_Town(dt) )
		{
			AddLog( "[_OnTownProc:HP Change to use Fill Shop]" , 1 );
			m_shop_state = ShopState__Buy;
			return true;
		}

		_ChangeState( PRE_ATTACK );

		SetLostQuickSlot();

		return true;
	}
	else if( PRE_ATTACK == m_current_state )
	{
		if( _OnBuffTimer(dt) )
			return true;

		if( _OnShield(dt) )
			return true;

		if( _OnDsystem(dt) )
			return true;
		if( _OnShield2(dt) )
			return true;
		if( _OnShield3(dt) )
			return true;
		if( _OnShield4(dt) )
			return true;

		_ChangeState( GO_TO_AREA_ON_WALK  );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnFieldProc(DWORD dt)
{
	if( _OnAvoid(dt) )
	{
		AddLog( "[_OnFieldProc:A return request by avoidance]" , 1 );

		m_dwAvoidCurTime = dt;
		_ChangeState( AVOID );
		_AddChat( "Return (avoid)" );
		_UseQuickItem( m_dwReturnKey );

		return true;
	}

	if( ITEM == m_current_state )
	{
		if( _OnFieldUsing(dt) )
		{
			AddLog( "[_OnFieldProc:Acquiring items: Processing basic process]" , 1 );
			return true;
		}

		// 아이템 습득
		if( !_OnPickItem(dt) )
		{
			if( _CheckElapsedTime(dt, m_pick_item_time , FALIED_PICKITEM_TIME ) )
			{
				AddLog( "[_OnFieldProc:Failed to acquire items (waiting time)]" , 1 );
				_ChangeState( NONE_STATE );
			}
			else
			{
				AddLog( "[_OnFieldProc:Failed to acquire items (on the move)]" , 1 );
			}
		}
		return true;
	}
	else if( ATTACK == m_current_state )
	{
		int rv_lost = IsLostQuickSlot(dt);

		if( rv_lost == -1 )
		{
			AddLog( "\t[_OnFieldProc] Quick Slot Error No items in inventory." );
		}
		else if( rv_lost == 0 )
		{
			// 정상
		}
		else if( rv_lost == 1 )
		{
			AddLog( "\t[_OnFieldProc] Quick Slot Error Request to move saved items." );
			return true;
		}

		if( _OnFieldUsing(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Processing main process]" , 1 );
			return true;
		}

		if( _OnPet(dt) )
		{
			AddLog( "[_OnFieldProc: Attacking: Pet-related success]" , 1 );
			return true;
		}
		if( _OnSubArms(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Successful Auxiliary Filler Handling]" , 1 );
			return true;
		}
		if( _OnAdiaSummon(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Successfully summoning Aidia]" , 1 );
			return true;
		}
		if( _OnShield(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Shielding successful]" , 1 );
			return true;
		}
		if( _OnDsystem(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Shielding successful]" , 1 );
			return true;
		}
		// 단축키 지정필요
		if( _OnShield2(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Shielding successful 2]" , 1 );
			return true;
		}
		// 단축키 지정필요
		if( _OnShield3(dt) )
		{
			AddLog( "[_OnFieldProc:ttacking: Shielding successful 3]" , 1 );
			return true;
		}
		if( _OnShield4(dt) )
		{
			AddLog( "[_OnFieldProc:공uzz: Successful shielding 3]" , 1 );
			return true;
		}
		if( _OnRecovery(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Successful recovery]" , 1 );
			return true;
		}
		if( _OnBloody(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Successful Bloody Mana]" , 1 );
			return true;
		}

		if( _OnBuffTimer(dt) )
			return true;

		if( _OnDecideTargetItem(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: item found]" , 1 );
			return true;
		}

		// _OnDecideTarget_Surport(dt);

		if( !_OnDecideTarget(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Target Decision Failed]" , 1 );
			return true;
		}

		if( _OnVampTouch(dt) )
		{
			AddLog( "[_OnFieldProc:Attacking: Vampire touch successful]" , 1 );
			return true;
		}

		if( !_OnAttack(dt) )
		{
			m_attack_try_count++;

			AddLog( "[_OnFieldProc:Attacking: Attack attempt...]" , 1 );

			if( m_attack_try_count > 5 )
			{
				AddLog( "[_OnFieldProc:Attacking: Attack attempt: 5 times failed...]" , 1 );
				_ChangeState( NONE_STATE );
				m_attack_try_count = 0;
				return false;
			}
		}

		return true;
	}
	else if( RETURN_TO_BASE == m_current_state )
	{
		if( _OnFieldUsing(dt) )
		{
			AddLog( "[_OnFieldProc:Return to attack point: Processing main process]" , 1 );
			return true;
		}

		if( _OnMoveToBase(dt) )
		{
			AddLog( "[_OnFieldProc:Return to attack point: Change to attack mode]" , 1 );
			_ChangeState( ATTACK );
		}

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name : _OnFieldUsing
//	Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnFieldUsing(DWORD dt)
{
	if( ITEM == m_current_state || ATTACK == m_current_state || RETURN_TO_BASE == m_current_state )
	{
		if( _CheckReturnToCity(dt) )
		{
			_ChangeState( RETURN_TO_CITY );
			_UseQuickItem( m_dwReturnKey );
			AddLog( "[_OnFieldUsing:Requesting return...]" , 1 );
			return true;
		}

		_OnPotion(dt);
		_OnPotion_Spacial(dt);
		_OnShield(dt);
		_OnDsystem(dt);
		_OnShield2(dt);
		_OnShield3(dt);
		_OnShield4(dt);
	}

	return false;
}


void CAutoPlay_EX::_ChangeState(int state)
{
	switch( state )
	{
	case NONE_STATE:
		break;
	case ATTACK:
		_AddChat_Debug( "ChangeState :  ATTACK" );
		break;
	case ITEM:
		_AddChat_Debug( "ChangeState : ITEM" );
		break;
	case RETURN_TO_CITY:
		_AddChat_Debug( "ChangeState : RETURN_TO_CITY" );
		break;
	case MOVE_IN_PEACE:
		_AddChat_Debug( "ChangeState : MOVE_IN_PEACE" );
		break;
	case SHOP_BUY:
		_AddChat_Debug( "ChangeState : SHOP_BUY" );
		break;
	case SHOP_SELL:
		_AddChat_Debug( "ChangeState : SHOP_SELL" );
		break;
	case REPAIR:
		_AddChat_Debug( "ChangeState : REPAIR" );
		break;
	case WARE:
		_AddChat_Debug( "ChangeState : WARE" );
		break;
	case FIELD_CHANGE:
		_AddChat_Debug( "ChangeState : FIELD_CHANGE" );
		break;
	case SET_ITEM_IN_QUICK:
		_AddChat_Debug( "ChangeState : SET_ITEM_IN_QUICK" );
		break;
	case USE_QUICK:
		_AddChat_Debug( "ChangeState : USE_QUICK" );
		break;
	case GO_TO_AREA_ON_WALK:
		_AddChat_Debug( "ChangeState : GO_TO_AREA_ON_WALK" );
		break;
	case AVOID:
		_AddChat_Debug( "ChangeState : AVOID" );
		break;
	case REVIVAL:
		_AddChat_Debug( "ChangeState : REVIVAL" );
		break;
	case DELAY_TIME:
		_AddChat_Debug( "ChangeState : DELAY_TIME" );
		break;
	case RETURN_TO_BASE:
		_AddChat_Debug( "ChangeState : RETURN_TO_BASE" );
		break;
	case WAIT_IN_CITY:
		_AddChat_Debug( "ChangeState : WAIT_IN_CITY" );
		break;
	}

	switch( state )
	{
	case ITEM:
		_ResetItem();
		break;
	case ATTACK:
		_ResetAttack();
		m_pTargetItem = NULL;
		m_last_attack_tick = GetTickCount();
		m_last_decide_target = 0;
		break;
	case RETURN_TO_BASE:
		_ResetAttack();
		m_movetobase_try_count = 0;
		m_pTargetItem = NULL;
		m_move_to_base_tick = GetTickCount();
		m_global_attack_time = 0;
		break;
	case WAIT_IN_CITY:
	case REVIVAL:
	case RETURN_TO_CITY:
		_ResetCity();
		Init();
		break;
	}

	m_need_surport = -1;

	if( m_current_state != REVIVAL )
	{
		pCMyApp->StopHero();
		g_pRoh->SetMotionState( CHA_STAND );
		g_pNk2DFrame->DeletePopup();
	}

	m_prev_state = m_current_state;
	m_current_state = state;
	m_changed_delay = timeGetTime();
	m_change_state = true;
}

bool CAutoPlay_EX::_CheckElapsedTime(DWORD dt, DWORD start_tick , DWORD delay_tick)
{
	if( start_tick == 0 )
		return false;

	if( dt - start_tick > delay_tick )
		return true;
	return false;
}

void CAutoPlay_EX::_ResetCity()
{
	m_changed_delay = GetTickCount();

	pCMyApp->SetHeroAttGoMob(NULL);
	pCMyApp->m_pUIMgr->m_b_Attack = false;
	pCMyApp->m_pUIMgr->m_b_MovingForAttack = false;
	pCMyApp->m_pUIMgr->m_b_MotionForAttack = false;

	m_pSelectedMob = NULL;
	m_pPrevSelectedMob = NULL;

	m_last_attack_tick = 0;
	m_last_decide_target = 0;
	m_move_to_base_tick = 0;
	m_attack_try_count = 0;

	m_pick_item_time = 0;
	m_pTargetItem = NULL;

	m_skill_slot = 0;
	m_last_action_tick = 0;
	m_last_attack_tick = 0;
	int i = 0;
	for( i = 0; i < AH_SPECIALPOTION; i++ )
	{
		m_dwSpecialPotionCurTime[i] = 0;
	}
//	m_dwSpecialPotionCurTime[0] = 0;
//	m_dwSpecialPotionCurTime[1] = 0;
	m_picked_apple = false;
	m_dead_time = 0;


	// 상점관련 초기화
	m_found_npc = false;
	m_buy_x = 0;
	m_buy_z = 0;
	m_pTemporaryMob = NULL;
	if( g_SvrType == ST_ADULT_ONLY )
		m_shop_state = ShopState__Repair;
	else
		m_shop_state = ShopState__Buy;

	// 이동관련 초기화
	m_wWalkPosCount = 0;
	m_bUsed_Trans = false;
	m_repair_count = 0;

	m_exception_target.clear();
}

// 아이템집기 상태?
void CAutoPlay_EX::_ResetItem()
{
	m_pick_item_time = GetTickCount();

	pCMyApp->SetHeroAttGoMob(NULL);
	pCMyApp->m_pUIMgr->m_b_Attack = false;
	pCMyApp->m_pUIMgr->m_b_MovingForAttack = false;
	pCMyApp->m_pUIMgr->m_b_MotionForAttack = false;
	pCMyApp->m_pUIMgr->m_b_SecAttack = false;

	m_pSelectedMob = NULL;
	m_pPrevSelectedMob = NULL;

	m_last_attack_tick = 0;
	m_last_decide_target = 0;
	m_move_to_base_tick = 0;
}

void CAutoPlay_EX::_ResetAttack()
{
	pCMyApp->SetHeroAttGoMob(NULL);
	pCMyApp->m_pUIMgr->m_b_Attack = false;
	pCMyApp->m_pUIMgr->m_b_MovingForAttack = false;
	pCMyApp->m_pUIMgr->m_b_MotionForAttack = false;
	pCMyApp->m_pUIMgr->m_b_SecAttack = false;

	// 공격, 타겟결정, 이동관련 시간 초기화
	m_last_attack_tick = 0;
	m_last_decide_target = 0;
	m_move_to_base_tick = 0;
	m_attack_try_count = 0;

	// 아이템
	m_pick_item_time = 0;
	m_pTargetItem = NULL;


	m_attack_try_count = 0;
	m_pSelectedMob = NULL;
}

void CAutoPlay_EX::_ResetMove()
{
	m_move_to_base_tick = GetTickCount();
}


bool CAutoPlay_EX::_UseQuickItem(DWORD dwKey)
{
	if( NULL == g_pRoh->m_QuickSlot[dwKey] )
	{
		return false;
	}

	pCMyApp->m_pUIMgr->UseQuickItem( dwKey );
	m_last_quick_tick = GetTickCount();

	return true;
}

bool CAutoPlay_EX::_MoveToQuick(int pack_no, int slot, CItem* pItem)
{
	if( NULL == pItem || NULL != g_pRoh->m_QuickSlot[slot] )
		return false;

	pCMyApp->m_pUIMgr->Send_MoveToItem( slot , pack_no , pItem->m_Index );

	return true;
}

State_ReturnValue CAutoPlay_EX::_OnFindNpc(DWORD dt, bool ware)
{
	static DWORD olddt = 0;

	if( olddt == 0 )
		olddt = 0;

	if( dt - olddt < 100 )
		return State_RV__Wait;

	olddt = dt;


	if( NULL == m_pTemporaryMob )
	{
		CNkMob* pMob = NULL;
		LHMobMap_it prMob;

		m_buy_x = 0;
		m_buy_z = 0;
		m_npc_index = -1;

		static int try_count = 0;

		for( prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++ )
		{
			pMob = (*prMob).second;
			char *p = pMob->GetMobName();
			printf("%s\n", pMob->GetMobName() );

			if( pMob->m_nMobNum == 666 && strstr(pMob->GetMobName(), "Rush" ) )
			{
				m_pTemporaryMob = pMob;

				m_buy_x = pMob->m_wx;
				m_buy_z = pMob->m_wz;
				m_npc_index = m_pTemporaryMob->m_nVirId;

				try_count = 0;

				AddLog( "[_OnFindNpc:discovery]" , 1 );

				return State_RV__Wait;
			}
		}

		{
			_AddChat_Debug( "NPC There is no mob and no carick." );

			int mx = 0;
			int my = 0;
			{
				float temp_dist = 100.0f;

				switch( dt % 8 )
				{
				case 0:
					mx += temp_dist;
					break;
				case 1:
					mx -= temp_dist;
					break;
				case 2:
					my += temp_dist;
					break;
				case 3:
					my -= temp_dist;
				case 4:
					mx += temp_dist;
					my += temp_dist;
					break;
				case 5:
					mx -= temp_dist;
					my -= temp_dist;
					break;
				case 6:
					mx += temp_dist;
					my -= temp_dist;
					break;
				case 7:
					mx -= temp_dist;
					my += temp_dist;
					break;
				}
				_OnMove(dt , g_pRoh->m_wx + mx, g_pRoh->m_wz + my );
			}

			try_count++;

			if( try_count < 5 )
				return State_RV__Wait;

			try_count = 0;
		}
	}
	else
	{
		float distance = DistPlToPl( m_pTemporaryMob->m_wx, m_pTemporaryMob->m_wz, g_pRoh->m_wx, g_pRoh->m_wz );
		if( _IsDungeon() )
			distance *= 0.5f;

		if( distance > 150 )
		{
			_OnMove(dt, m_pTemporaryMob->m_wx , m_pTemporaryMob->m_wz );
			return State_RV__Wait;
		}
		else
		{
			D3DVECTOR vWorldPos, vScreenPos;
			D3DMATRIX matPipe;
			ZeroMemory( &matPipe, sizeof( D3DMATRIX ) );

			int mx = 0, my = 0;

			pCMyApp->StopHero();
			pCMyApp->m_pUIMgr->SetPickColor(m_pTemporaryMob->m_wColorIdx);
			vWorldPos = D3DVECTOR( m_pTemporaryMob->m_wx, m_pTemporaryMob->m_wy, m_pTemporaryMob->m_wz );
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipe );

			D3DMath_VectorMatrixMultiply( vScreenPos, vWorldPos, matPipe );

			mx = vScreenPos.x;
			my = vScreenPos.y;
			if( mx < 0 )	mx = 0;
			if( my < 0 )	my = 0;

			if( ware )
				g_pNk2DFrame->ToggleWareHouseWindow();
			else
				pCMyApp->m_pUIMgr->MouseClickEvent_NPC( m_pTemporaryMob, BUTTON_LEFT );

			olddt = 0;

			AddLog( "[_OnFindNpc:Store / warehouse processing request]" , 1 );

			return State_RV__Finish;
		}
	}

	_OnInterrupt(LagRush_Interrupt__NPC_NotFound);

	return State_RV__None;
}



//-----------------------------------------------------------------------------
// Name : _OnStashItem
// Date : 2009.10.22
// Desc : 창고 이용
//-----------------------------------------------------------------------------
State_ReturnValue CAutoPlay_EX::_OnStashItem(DWORD dt)
{
	if( !m_bWareHouse )
		return State_RV__None;

	int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
	if( chaotic < 0 && chaotic > -500 )
	{
	}
	else if( chaotic <= -500 )
	{
		if( g_dev_client )
			g_pNk2DFrame->AddChatStr( "[Rag Rush] No search" , -1 );
		return State_RV__None;
	}

	if( g_pNk2DFrame->IsWareHouseVisible() )
	{
		m_found_npc = false;

		if( m_stash_sendmsg != 0 )
		{
			if( m_stash_sendmsg == 1 )
				return State_RV__Wait;

			static DWORD olddt = 0;
			if(olddt == 0)
				olddt = dt;

			if( dt - olddt < 100 )
				return State_RV__Wait;

			olddt = 0;
			m_stash_sendmsg = 0;
			return State_RV__Wait;
		}

		if( m_stash_state == StashState__Push )
		{
			CItem * inven_item = NULL;

			for( int pack=0 ; pack<PACK_NUM ; pack++ )
			{
				if( inven_item = g_pRoh->m_Inven[pack] )
				{
					while( inven_item )
					{
						bool need_put = false;
						for( int i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++ )
						{
							if( g_pRoh->m_QuickSlot[i] == NULL )
								continue;
							if( g_pRoh->m_QuickSlot[i]->m_Vnum != inven_item->m_Vnum )
								continue;

							g_pRoh->CountQuickBySlot(i);
							int quick_count = g_pRoh->m_QuickCount[i];
							if( m_wBuyFromShop & CALC_BIT( 1, i ) )
							{
								if( quick_count > m_dwBuyFromShopCount[i] )
									need_put = true;
								else
									need_put = false;
							}
							else
							{
								if( m_dwKeepCount[i] > 0 && quick_count > m_dwKeepCount[i] )
									need_put = true;
								else
									need_put = false;
							}
							break;
						}

						if( need_put )
						{
							CControlWareHouse * ware_house = g_pNk2DFrame->GetControlWareHouse();
							int ware_slot = ware_house->Push_Available(inven_item);

							if( ware_slot == -1 )
							{
								// 넣을공간이 없다.
								inven_item = inven_item->m_Next;
								continue;
							}

							char msg_buf[256] = {0,};
							sprintf( msg_buf , "stash_puts %d %d %d 1 %d %d %d\n" , inven_item->m_Vnum , ware_slot , pack , inven_item->m_Index , inven_item->m_SlotX , inven_item->m_SlotY );
							LH_SEND_NET_MSG( msg_buf );
							m_stash_sendmsg = 1;
							AddLog( "[_OnStashItem:Warehouse request]" , 1 );

							return State_RV__Wait;
						}

						inven_item = inven_item->m_Next;
					}

					char szTemp[128] = {0,};
					int push_count = 0;
					char item_buf[128] = {0, };

					inven_item = g_pRoh->m_Inven[pack];
					while( inven_item )
					{
						sprintf( szTemp, "%s%d", GET_ITEM_NAME_VNUM(inven_item->m_Vnum),
								 strlen( GET_ITEM_NAME_VNUM(inven_item->m_Vnum) ));

						if( strstr( m_szItemList.c_str(), szTemp ) )
						{

							// 퀵슬롯 아이템 제외!!!
							bool skip_item = false;
							for( int qi=0 ; qi<QUICK_SLOT_NUM*QUICK_SLOT_PAGE ; qi++ )
							{
								if( g_pRoh->m_QuickSlot[qi] && g_pRoh->m_QuickSlot[qi]->m_Vnum == inven_item->m_Vnum )
								{
									skip_item = true;
									break;
								}
							}

							if( skip_item )
							{
								inven_item = inven_item->m_Next;
								continue;
								break;
							}

							if( inven_item->m_bTimeLimit )
							{
								inven_item = inven_item->m_Next;
								continue;
								break;
							}
							if( GET_ATT_FLAG( inven_item ) & IATT_NO_STASH )
							{
								inven_item = inven_item->m_Next;
								continue;
								break;
							}
// 							if( GET_ITEM_WIDTH(inven_item) > 1 || GET_ITEM_HEIGHT(inven_item) > 1 )
// 								break;

							CControlWareHouse * ware_house = g_pNk2DFrame->GetControlWareHouse();

							int ware_slot = ware_house->Push_Available(inven_item);

							if( ware_slot == -1 )
							{
								// 넣을공간이 없다.
								inven_item = inven_item->m_Next;
								continue;
								break;
							}

							char msg_buf[256] = {0,};
							sprintf( msg_buf , "stash_puts %d %d %d 1 %d %d %d\n" , inven_item->m_Vnum , ware_slot , pack , inven_item->m_Index , inven_item->m_SlotX , inven_item->m_SlotY );
							LH_SEND_NET_MSG( msg_buf );
							m_stash_sendmsg = 1;
							AddLog( "[_OnStashItem:Warehouse request]" , 1 );
							return State_RV__Wait;

						}

						inven_item = inven_item->m_Next;
					}
				}
			}

			AddLog( "[_OnStashItem:Depositing completed: Change to find]" , 1 );

			m_stash_state = StashState__Pop;
			return State_RV__Wait;
		}
		else if( m_stash_state == StashState__Pop )
		{
			CControlWareHouse * ware_house = g_pNk2DFrame->GetControlWareHouse();

			for( int i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++ )
			{
				if( g_pRoh->m_QuickSlot[i] == NULL )
					continue;

				int quick_count = g_pRoh->m_QuickCount[i];
				int get_count = 10;
				bool need_get = false;

				if( m_wBuyFromShop & CALC_BIT( 1, i ) )
				{
					if( quick_count >= m_dwBuyFromShopCount[i] )
					{
						need_get = false;
					}
					else
					{
						need_get = true;
						get_count = min( 10 , (m_dwBuyFromShopCount[i]-quick_count) );
					}
				}
				else
				{
					if( quick_count >= m_dwKeepCount[i] )
					{
						need_get = false;
					}
					else
					{
						need_get = true;
						get_count = min( 10 , (m_dwKeepCount[i]-quick_count) );
					}
				}

				if( need_get )
				{
					int ware_slot = ware_house->FindSlot( g_pRoh->m_QuickSlot[i]->m_Vnum , false );
					if( ware_slot == -1 )
						continue;

//					ware_house->SetCurPackNum(ware_slot/20);

					CItem * ware_item = ware_house->GetWareSlotItem(ware_slot);
					int ware_count = ware_house->GetWareSlotCount(ware_slot);

					get_count = min(get_count , ware_count);

					for( int inven_pack=0 ; inven_pack<PACK_NUM ; inven_pack++ )
					{
						CItem * inven_item = g_pRoh->m_Inven[inven_pack];
						int empty_slot = g_pRoh->GetEmptyInvenInPack(inven_pack);
						if( empty_slot != 0 )
						{
//							g_pRoh->ChangePackNum( inven_pack );

							get_count = min( empty_slot , get_count );

							char msg_buf[256] = {0,};
							sprintf( msg_buf , "stash_gets %d %d %d %d\n" , ware_item->m_Vnum , ware_slot , inven_pack , get_count);
							LH_SEND_NET_MSG( msg_buf );
							m_stash_sendmsg = 1;

							char temp[128] = {0,};
							sprintf( temp , "[_OnStashItem]Find Item Count : %d %d" , ware_item->m_Vnum , get_count );
							_AddChat_Debug( temp );
							AddLog( temp , 1 );

							return State_RV__Wait;
						}
					}
				}
			}

			m_stash_state = StashState__Pop;
			return State_RV__Finish;
		}
	}
	else
	{
		if( pCMyApp->m_pUIMgr->m_mob_map.empty() )
		{

		}
		if( false == m_found_npc && State_RV__Finish == _OnFindNpc(dt,true) )
		{
			m_found_npc = true;
			m_stash_sendmsg = 1;
		}

		if( m_found_npc )
		{
			// stash_opne 을 보냈는데...창고를 못열고 있다....
		}

		return State_RV__Wait;
	}


	return State_RV__None;
}


//-----------------------------------------------------------------------------
// Name : _OnShopItem
// Date : 2009.10.22
// Desc :
//-----------------------------------------------------------------------------
State_ReturnValue CAutoPlay_EX::_OnShopItem(DWORD dt)
{
	if( g_pNk2DFrame->IsShopVisible() )
	{
		m_found_npc = false;

		if( ShopState__Repair == m_shop_state )
		{
			if( State_RV__Wait == _OnRepairItem(dt) )
				return State_RV__Wait;

			m_shop_state = ShopState__Buy;
			return State_RV__Wait;
		}
		else if( ShopState__Buy == m_shop_state )
		{
			if( State_RV__Wait == _OnBuyItem(dt) )
				return State_RV__Wait;

			m_shop_state = ShopState__MoveToQuick;
			return State_RV__Wait;
		}
		else if( m_shop_state == ShopState__MoveToQuick)
		{
			if( State_RV__Wait == _OnSetQuickItem(dt) )
				return State_RV__Wait;

			m_shop_state = ShopState__EndOfEnum;
			return State_RV__Wait;
		}
		else if( m_shop_state == ShopState__EndOfEnum)
		{
			if( g_SvrType == ST_ADULT_ONLY )
				m_shop_state = ShopState__Repair;
			else
				m_shop_state = ShopState__Buy;

			return State_RV__Finish;
		}
	}
	else
	{
		if( false == m_found_npc && State_RV__Finish == _OnFindNpc(dt,false) )
			m_found_npc = true;

		return State_RV__Wait;
	}

	return State_RV__None;
}

//-----------------------------------------------------------------------------
// Name : _OnRepairItem
// Date : 2009.10.22
// Desc : _OnShopItem - 아이템수리
//-----------------------------------------------------------------------------
State_ReturnValue CAutoPlay_EX::_OnRepairItem(DWORD dt)
{
	if( false == m_bItemRepair )
		return State_RV__None;

	static DWORD olddt = 0;

	BYTE arrRepair[6] = { 0, 1, 2, 4, 5, 9 };
	if(m_repair_count >= 6)
	{
		m_repair_count = 0;
		olddt = 0;
		return State_RV__Finish;
	}

	if( dt - olddt < 200 )
		return State_RV__Wait;

	olddt = dt;

	g_pNk2DFrame->DeleteAllPopup();
	char msg_buf[128];
	sprintf(msg_buf, "repair 99 %d 0\n", arrRepair[m_repair_count++] );
	g_pTcpIp->SendNetMessage(msg_buf);

	return State_RV__Wait;
}

//-----------------------------------------------------------------------------
// Name : _OnBuyItem
// Date : 2009.10.22
// Desc : _OnShopItem - 아이템구입
//		  구입관련 금액처리도 필요할것으로 보인다.
//-----------------------------------------------------------------------------
State_ReturnValue CAutoPlay_EX::_OnBuyItem(DWORD dt)
{
	if( IsFullInvenInAllPack() )
	{
		_OnInterrupt(LagRush_Interrupt__Inven_Full);
		return State_RV__None;
	}

	if( m_shop_sendmsg != 0 )
	{
		if( m_shop_sendmsg == 1 )
			return State_RV__Wait;

		static DWORD olddt = 0;
		if( olddt == 0 )
			olddt = dt;

		if( dt - olddt < 100 )
			return State_RV__Wait;

		olddt = 0;
		m_shop_sendmsg = 0;
		return State_RV__Wait;
	}

	static int buy_slot = 0;
	if( buy_slot > AH_QUICKSLOT_COUNT+3 )
	{
		buy_slot = 0;
		return State_RV__Finish;
	}

	if( buy_slot < AH_QUICKSLOT_COUNT )
	{
		CItem * pItem = g_pRoh->m_QuickSlot[buy_slot];
		if( NULL != pItem && (m_wBuyFromShop & CALC_BIT( 1, buy_slot )) )
		{
			int num_buy = m_dwBuyFromShopCount[buy_slot];
			int num_slot = g_pRoh->m_QuickCount[buy_slot];

			if( num_buy > num_slot )
			{
				num_buy = min( num_buy-num_slot , 10 );

				char msg_buf[256];
				sprintf(msg_buf, "lr_buy %d %d %d\n", m_npc_index, pItem->m_Vnum, num_buy);
				g_pTcpIp->SendNetMessage(msg_buf);

				m_shop_sendmsg = 1;

				char temp[128] = {0,};
				sprintf( temp , "Buy Item : %d (Have :%d)" , num_buy ,num_slot );
				_AddChat_Debug( temp );

				return State_RV__Wait;
			}
		}

		++buy_slot;
		return State_RV__Wait;
	}
	else
	{
		int pack_num = 0;
		if( g_pRoh->IsFullInvenInPack( 0 ) )
		{
			if( g_pRoh->IsFullInvenInPack( 1 ) )
			{
				if( g_pRoh->IsFullInvenInPack( 2 ) )
					pack_num = 4;
				else
					pack_num = 2;
			}
			else
			{
				pack_num = 1;
			}
		}

		g_pRoh->ChangePackNum( pack_num );
		int emptyCount = g_pRoh->GetEmptyInvenInPack( pack_num );
		int buy_count = 0;
		int item_count= 0;
		int item_index = -1;

		if( buy_slot == AH_QUICKSLOT_COUNT+1 /* 6 */ && m_bBuyApple )
		{
			buy_count = m_uApplesCount;
			item_index = 618;
			item_count = g_pRoh->GetCountInPack( item_index );
		}
		else if( buy_slot == AH_QUICKSLOT_COUNT+2 /* 7 */ && m_bBuyCallSign )
		{
			buy_count = m_uCallersCount;
			item_index = 968;

			if( g_pRoh->m_QuickSlot[m_uCallKeyMapping] && g_pRoh->m_QuickSlot[m_uCallKeyMapping]->m_Vnum == item_index )
				item_count = g_pRoh->m_QuickCount[m_uCallKeyMapping];
			else
				item_count = g_pRoh->GetCountInPack( item_index );
		}
		else if( buy_slot == AH_QUICKSLOT_COUNT+3 /* 8 */)
		{
			buy_count = 2;
			item_index = 175;
			if( g_pRoh->m_QuickSlot[m_dwReturnKey] && g_pRoh->m_QuickSlot[m_dwReturnKey]->m_Vnum == item_index )
				item_count = g_pRoh->m_QuickCount[m_dwReturnKey];
			else
				item_count = g_pRoh->GetCountInPack( item_index );
		}
		else if( buy_slot == AH_QUICKSLOT_COUNT+4 /* 9 */ && m_bUseTrans )
		{
			buy_count = 2;
			item_index = 1015;
			if( g_pRoh->m_QuickSlot[m_dwTransportingKey] && g_pRoh->m_QuickSlot[m_dwTransportingKey]->m_Vnum == item_index )
				item_count = g_pRoh->m_QuickCount[m_dwTransportingKey];
			else
				item_count = g_pRoh->GetCountInPack( item_index );
		}

		if( buy_count > item_count && item_index != -1 )
		{
			buy_count = ( buy_count - item_count ) > emptyCount ? emptyCount : ( buy_count - item_count );
			buy_count = max(buy_count , 0);

			if( buy_count != 0 )
			{
				buy_count = min( buy_count , 10 );
				char msg_buf[256];
				sprintf(msg_buf, "lr_buy %d %d %d\n", m_npc_index, item_index, buy_count);
				g_pTcpIp->SendNetMessage(msg_buf);

				char temp[128] = {0,};
				sprintf( temp , "Buy Num Count : %d" , buy_count );
				_AddChat_Debug( temp );

				m_shop_sendmsg = 1;

				return State_RV__Wait;
			}
		}

		++buy_slot;
		return State_RV__Wait;
	}

	return State_RV__None;
}

//-----------------------------------------------------------------------------
// Name : _OnSetQuickItem
// Date : 2009.10.22
// Desc : _OnShopItem - 퀵슬롯 아이템 이동처리
//-----------------------------------------------------------------------------
State_ReturnValue CAutoPlay_EX::_OnSetQuickItem(DWORD dt)
{
	static DWORD olddt = 0;
	if( olddt == 0 )
		olddt = dt;

	if( dt - olddt < 500 )
		return State_RV__Wait;


	// 귀환장치 무조건!!!
	if( m_use_return == true )
	{
		if(  NULL == g_pRoh->m_QuickSlot[m_dwReturnKey] )
		{
			CItem *pItem = NULL;
			for (int i = 0; i < PACK_NUM; i++)
			{
//				pItem = g_pRoh->PickItemInPack(i, 175, TRUE);
				pItem = g_pRoh->GetInvenItem(i, 175, TRUE);
				if (pItem)
				{
					_MoveToQuick( i , m_dwReturnKey , pItem );
					olddt = dt;
					return State_RV__Wait;
				}
			}
		}
		else
		{
			if( g_pRoh->m_QuickSlot[m_dwReturnKey]->m_Vnum != 175 )
			{
				char err_buf[256];
				sprintf( err_buf , G_STRING(IDS_LHSTRING1793), m_uCallKeyMapping + 1);
				_AddChat( err_buf );
				AddLog( err_buf , 1 );
				_OnInterrupt(LagRush_Interrupt__Slot_NotMatch);
				return State_RV__Wait;
			}
		}
	}

	if( m_bUseTrans )
	{
		if( NULL == g_pRoh->m_QuickSlot[m_dwTransportingKey] )
		{
			CItem *pItem = NULL;
			for (int i = 0; i < PACK_NUM; i++)
			{
//				pItem = g_pRoh->PickItemInPack(i, 1015, TRUE);
				pItem = g_pRoh->GetInvenItem(i, 1015, TRUE);
				if (pItem)
				{
					_MoveToQuick( i , m_dwTransportingKey , pItem );
					olddt = dt;
					return State_RV__Wait;
				}
			}
		}
		else
		{
			if( g_pRoh->m_QuickSlot[m_dwTransportingKey]->m_Vnum != 1015 )
			{
				char err_buf[256];
				sprintf( err_buf , G_STRING(IDS_LHSTRING1793) , m_dwTransportingKey + 1);
				_AddChat( err_buf );
				AddLog( err_buf , 1 );
				_OnInterrupt(LagRush_Interrupt__Slot_NotMatch);
				return State_RV__Wait;
			}
		}
	}

	if( m_bUsedCallSign )
	{
		if( NULL == g_pRoh->m_QuickSlot[m_uCallKeyMapping] )
		{
			CItem *pItem = NULL;
			for (int i = 0; i < PACK_NUM; i++)
			{
//				pItem = g_pRoh->PickItemInPack(i, 968, TRUE);
				pItem = g_pRoh->GetInvenItem(i, 968, TRUE);

				if (pItem)
				{
					_MoveToQuick( i , m_uCallKeyMapping , pItem );
					olddt = dt;
					return State_RV__Wait;
				}
			}
		}
		else
		{
			if( g_pRoh->m_QuickSlot[m_uCallKeyMapping]->m_Vnum != 968 )
			{
				char err_buf[256];
				sprintf( err_buf , (char *)G_STRING(IDS_LHSTRING1793) , m_uCallKeyMapping + 1);
				_AddChat( err_buf );
				_AddChat( (char*)G_STRING(IDS_LHSTRING1794) );
				AddLog( err_buf , 1 );
				m_bUsedCallSign = false;
				return State_RV__Wait;
			}
		}
	}

	return State_RV__Finish;
}


//-----------------------------------------------------------------------------
// Name : _OnMoveTo
// Date : 2009.10.22
// Desc : 사냥터이동
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnMoveToField(DWORD dt)
{

	if( _OnBuffTimer(dt) )
		return false;

	if( _OnShield(dt) )
		return false;

	if( _OnDsystem(dt) )
		return false;
	if( _OnShield2(dt) )
		return false;
	if( _OnShield3(dt) )
		return false;
	if( _OnShield4(dt) )
		return false;

	bool rv = false;
	if( m_bUseTrans )
		rv = _OnMoveToMachine(dt);
	else
	{

		if( _CheckReturnToCity(dt) && !_IsPeaceArea() )
		{
			_ChangeState( RETURN_TO_CITY );
			_UseQuickItem( m_dwReturnKey );
			AddLog( "[_OnMoveToField:Requesting return...]" , 1 );
			return false;
		}

		_OnPotion(dt);

		rv = _OnMoveToWalk(dt);
	}

	return rv;
}



//-----------------------------------------------------------------------------
//	Name : _OnMove
//	Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnMove(DWORD dt, float tx, float tz)
{
	static DWORD olddt = 0;
	if( olddt == 0 )
		olddt = dt;
	if( dt - olddt < MOVE_TICK )
		return false;

	if(g_pRoh->m_CancelState)
	{
		if( g_pRoh->m_ChaCondition==CONDITION_STONY ||
				g_pRoh->m_ChaCondition==CONDITION_ENTANGLE)	//석화:1
			return false;
	}

	if( g_pRoh->m_HideStatus == HIDE_HIDDEN	|| g_pRoh->m_HideStatus == HIDE_HIDING)
		return false;

	olddt = dt;

	int char_motion = g_pRoh->GetMotionState();
	if( char_motion == CHA_RUN || char_motion == CHA_WALK || char_motion == CHA_STAND || char_motion == CHA_SWIM_STAND || char_motion == CHA_SWIM )
	{
		if( pCMyApp->MoveHeroT(tx, tz) )
		{
			olddt = 0;
			return true;
		}
	}
	else
	{
		if( m_current_state == ITEM || m_current_state == ATTACK || m_current_state == RETURN_TO_BASE )
		{
			g_pRoh->SetMotionState( CHA_STAND );
			return false;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _OnMoveTo
// Date : 2009.10.22
// Desc : walkpos.txt 참조 이용
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnMoveToWalk(DWORD dt)
{
	bool rv = false;

	if( g_pNk2DFrame->IsShopVisible() )
		g_pNk2DFrame->ToggleShopWindow();

	if( m_pairPoslist.empty() )
	{
		_OnInterrupt(LagRush_Interrupt__WalkPos_Empty);
		return false;
	}

	if( m_wWalkPosCount > m_pairPoslist.size() -1 )
	{
		SetPos(g_pRoh->m_wx , g_pRoh->m_wz );

		return true;
	}

	float dist = DistPlToPl( m_pairPoslist[m_wWalkPosCount].first, m_pairPoslist[m_wWalkPosCount].second, g_pRoh->m_wx, g_pRoh->m_wz );
	if( _IsDungeon() )
		dist *= 0.5f;

	if( dist > 1.0f )
	{
		_OnMove(dt, m_pairPoslist[m_wWalkPosCount].first, m_pairPoslist[m_wWalkPosCount].second );
	}
	else
	{
		m_wWalkPosCount++;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _OnMoveToMachine
// Date : 2009.10.22
// Desc : 지정전송장치 이용
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnMoveToMachine(DWORD dt)
{
	static DWORD olddt = 0 ;
	if( olddt == 0 )
	{
		olddt = dt;
		return false;
	}

	if( dt - olddt < 1500 )
		return false;


	if( m_bUsed_Trans && pCMyApp->m_pUIMgr->m_bIsTeleportItem )
	{
		if( pCMyApp->m_pUIMgr->m_pTeleportItem->SendLoadPosMsg( m_dwTransportingPos ) == 1 )
		{
			pCMyApp->m_pUIMgr->m_bIsTeleportItem = FALSE;
			if(pCMyApp->m_pUIMgr->m_pTeleportItem)
			{
				delete pCMyApp->m_pUIMgr->m_pTeleportItem;
				pCMyApp->m_pUIMgr->m_pTeleportItem = NULL;
			}

			_OnInterrupt(LagRush_Interrupt__Machine_Error);

			return false;
		}
		else
		{
			g_pNk2DFrame->DeleteAllPopup();
			pCMyApp->m_pUIMgr->m_bIsTeleportItem = FALSE;
			pCMyApp->m_pUIMgr->m_pTeleportItem = NULL;
			m_bUsed_Trans = false;

			return true;
		}
	}

	if( false == m_bUsed_Trans && _UseQuickItem( m_dwTransportingKey ) )
	{
		olddt = dt;
		m_bUsed_Trans = true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _CheckReturnToCity
// Date : 2009.10.22
// Desc : 귀환 조건
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_CheckReturnToCity(DWORD dt)
{
	static DWORD olddt = 0;
	if( olddt == 0 )
		olddt = dt;

	if( dt - olddt < 100 )
		return false;

	olddt = 0;

	if( m_wReturn & CALC_BIT( 1, AH_RETURN_HP ) )
	{
		if( CalcRatio(g_pRoh->m_Vital , g_pRoh->m_MaxVital) <= m_dwReturnCount[AH_RETURN_HP] )
		{
			_AddChat( "Return (vitality level)" );
			return true;
		}
	}

	if( m_wReturn & CALC_BIT( 1, AH_RETURN_HEALTH ) )
	{
		if( m_bHealth && ( g_pRoh->m_QuickCount[m_dwHealthKey] <= m_dwReturnCount[AH_RETURN_HEALTH] ) )
		{
			_AddChat( "Return (Life potion count)" );
			return true;
		}
	}

	if( m_wReturn & CALC_BIT( 1, AH_RETURN_MANA ) )
	{
		if( m_bMana && ( g_pRoh->m_QuickCount[m_dwManaKey] <= m_dwReturnCount[AH_RETURN_MANA] ) )
		{
			_AddChat( "Return (number of potions)" );
			return true;
		}
	}

	if( m_wReturn & CALC_BIT( 1, AH_RETURN_STAMINA ) )
	{
		if( m_bStamina && ( g_pRoh->m_QuickCount[m_dwStaminaKey] <= m_dwReturnCount[AH_RETURN_STAMINA] ) )
		{
			_AddChat( "Return (stamina potion count)" );
			return true;
		}
	}

	if( m_wReturn & CALC_BIT( 1, AH_RETURN_EPOWER ) )
	{
		if( m_bEPower && ( g_pRoh->m_QuickCount[m_dwEPowerKey] <= m_dwReturnCount[AH_RETURN_EPOWER] ) )
		{
			_AddChat( "Return (electric power potion number)" );
			return true;
		}
	}

	if( m_wReturn & CALC_BIT( 1, AH_RETURN_SUBARMS ) )
	{
		if( m_bSubArms && ( g_pRoh->m_QuickCount[m_dwSubArmsKey] <= m_dwReturnCount[AH_RETURN_SUBARMS] ) )
		{
			_AddChat( "Return (number of auxiliary fillers)" );
			return true;
		}
	}

	if( m_wReturn & CALC_BIT( 1, AH_RETURN_SUMMONS ) )
	{
		if( m_bAdiaSummon && ( g_pRoh->m_QuickCount[m_dwAdiaSummonKey] <= m_dwReturnCount[AH_RETURN_SUMMONS] ) )
		{
			_AddChat( "Return (Number of summons)" );
			return true;
		}
	}

	if( m_bPetActive )
	{
		if( m_wReturn & CALC_BIT( 1, AH_RETURN_APPLE ) )
		{
			int num_apple = g_pRoh->GetCountInPack(618);
			if( g_MyPetStatus.m_pBag )
				num_apple += g_MyPetStatus.food_count;

			if( num_apple <= 0 )
			{
				_AddChat( "Return (number of apples)" );
				return true;
			}
		}

		if( m_wReturn & CALC_BIT( 1, AH_RETURN_CALLER ) )
		{
			if( !g_pRoh->m_QuickSlot[m_uCallKeyMapping] )
			{
				if( g_pRoh->GetCountInPack(968) > 0 )
					return false;

				_AddChat( "Return (number of paging pagers)" );
				return true;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _ReturnToCity
//-----------------------------------------------------------------------------
void CAutoPlay_EX::_ReturnToCity()
{
	if( g_pRoh->m_Race == RACE_AIDIA && m_bAdiaSummon )
	{
		if( pCMyApp->m_pUIMgr->m_pSummonMob[0] != NULL || pCMyApp->m_pUIMgr->m_pSummonMob[1] != NULL )
		{
			_UseQuickItem( m_dwAdiaSummonKey );
		}
	}
}


//-----------------------------------------------------------------------------
// Name : _OnPotion
// Date : 2009.10.22
// Desc : 물약 사용관련
//		  true - 사용
//		  false - 미사용
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnPotion(DWORD dt)
{
	if( m_last_quick_tick != 0 )
		return false;

	if( g_pRoh->IsDead() )
	{
		AddLog( "I am dead and I want to use potions." , 1);
		return true;
	}

	if( m_bHealth && CalcRatio( g_pRoh->m_Vital , g_pRoh->m_MaxVital) <= m_fHealthPercent )
	{
		if( g_pRoh->m_QuickCount[m_dwHealthKey] <= m_dwReturnCount[0] )
			return false;

		_AddChat( "Use vitality potion." );
		_UseQuickItem(m_dwHealthKey);

		return true;
	}

	if( m_bMana && CalcRatio( g_pRoh->m_Mana , g_pRoh->m_MaxMana) <= m_fManaPercent )
	{
		if( g_pRoh->m_QuickCount[m_dwManaKey] <= m_dwReturnCount[1] )
			return false;

		_AddChat( "Use magical potions." );
		_UseQuickItem(m_dwManaKey);

		return true;
	}

	if( m_bStamina && CalcRatio( g_pRoh->m_Stamina , g_pRoh->m_MaxStamina) <= m_fStaminaPercent )
	{
		if( g_pRoh->m_QuickCount[m_dwStaminaKey] <= m_dwReturnCount[2] )
			return false;

		_AddChat( "Use Stamina Potion." );
		_UseQuickItem(m_dwStaminaKey);

		return true;
	}

	if( m_bEPower && CalcRatio( g_pRoh->m_Epower , g_pRoh->m_MaxEpower ) <= m_fEPowerPercent )
	{
		if( g_pRoh->m_QuickCount[m_dwEPowerKey] <= m_dwReturnCount[3] )
			return false;

		_AddChat( "Use electric power potions." );
		_UseQuickItem(m_dwEPowerKey);

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _OnPotion_Spacial
// Date : 2009.10.22
// Desc : 특수물약사용 - 시간
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnPotion_Spacial(DWORD dt)
{
	if( m_last_quick_tick != 0 )
		return false;

	int i = 0;

	for( i = 0; i < AH_SPECIALPOTION; i++ )
	{
		if( m_bSpecialPotion[i] )
		{
			if( m_dwSpecialPotionCurTime[i] == 0 )
			{
				if( g_pRoh->m_QuickCount[ m_dwSpecialPotionKey[i] ] > 1 )
				{
					if( false == _UseQuickItem( m_dwSpecialPotionKey[i] ) )
						return false;

					m_dwSpecialPotionCurTime[i] = dt;
					return true;
				}
			}
			else
			{
				if( dt - m_dwSpecialPotionCurTime[i] > m_wSpecialPotionLimitTime[i] )
				{
					m_dwSpecialPotionCurTime[i] = 0;
					return false;
				}
			}
		}
	}

	return false;
}

bool CAutoPlay_EX::_OnPotion_Town(DWORD dt)
{
	if( m_last_quick_tick != 0 )
		return true;

	if( g_pRoh->m_Vital < g_pRoh->m_MaxVital )
	{
		if( g_pRoh->m_QuickCount[m_dwHealthKey] <= m_dwReturnCount[0] )
			return true;

		_AddChat( "Use vitality potion." );
		_UseQuickItem(m_dwHealthKey);
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _OnSubArms
// Date : 2009.10.22
// Desc : 보조충전제 - 종족마다...틀리다 ㅡㅡ;
//		  퀵슬롯 장착 아이템일경우 무시ㅡㅡ;
//		  m_last_quick_tick 검사
//		  무조건 사용한다. ㅡㅡ;;
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnSubArms(DWORD dt)
{
	if( !m_bSubArms )
		return false;

	if( m_last_quick_tick != 0 )
		return false;

	CItem * pItem = g_pRoh->m_QuickSlot[m_dwSubArmsKey];
	if( !pItem )
	{
		m_bSubArms = false;
		return false;
	}

	if( g_pRoh->m_Race == RACE_BULKAN )
	{
		// 불칸
		if( !_IsSubArms( g_pRoh->m_Wearing[3] ) )
			return false;

		if( g_pRoh->m_Wearing[3]->m_Special > (g_pRoh->m_Wearing[3]->m_Num3 * 0.1) )
			return false;

		if( pItem->m_Vnum != 739 && pItem->m_Vnum != 807 )
			return false;
	}
	else if( g_pRoh->m_Race == RACE_KAILIPTON )
	{
		// 카이
		if( !_IsSubArms(g_pRoh->m_Wearing[5] ) )
			return false;

		if( g_pRoh->m_Wearing[5]->m_PlusNum > (g_pRoh->m_Wearing[5]->m_Num3 * 0.1) )
			return false;

		if( pItem->m_Vnum != 776 && pItem->m_Vnum != 777 &&
				pItem->m_Vnum != 778 && pItem->m_Vnum != 779 )
			return false;

		if( g_pRoh->m_Wearing[5]->m_MinLevel != pItem->m_MinLevel )
			return false;
	}
	else if( g_pRoh->m_Race == RACE_AIDIA )
	{
		// 에이디아
		if( !_IsSubArms( g_pRoh->m_Wearing[WEARING_SHIELD] ) )
			return false;

		if( g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum > (g_pRoh->m_Wearing[WEARING_SHIELD]->m_Num3 * 0.1) )
			return false;

		if( pItem->m_Vnum != 1019 ) // 피리세척제
			return false;

	}
	else if( g_pRoh->m_Race == RACE_HUMAN )
	{
		// 휴먼
		if( !_IsSubArms(g_pRoh->m_Wearing[3] ) )
			return false;

		if( g_pRoh->m_Wearing[3]->m_PlusNum > (g_pRoh->m_Wearing[3]->m_Num3 * 0.1) )
			return false;

		if( g_pRoh->m_Wearing[3]->m_Vnum == 742 ||				// 레이져
				g_pRoh->m_Wearing[3]->m_Vnum == 747 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 954 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 955 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 752 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 3068 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 11066 
		  )
		{
			if( pItem->m_Vnum != 761 && pItem->m_Vnum != 762 )
				return false;
		}
		else if( g_pRoh->m_Wearing[3]->m_Vnum == 744 ||			// 로켓
				 g_pRoh->m_Wearing[3]->m_Vnum == 749 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 754 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 755 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 10374
			   )
		{
			if( pItem->m_Vnum != 765 && pItem->m_Vnum != 1050 )
				return false;
		}
		else if( g_pRoh->m_Wearing[3]->m_Vnum == 743 ||			// 지뢰
				 g_pRoh->m_Wearing[3]->m_Vnum == 748 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 753 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 10453 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 11067
			   )
		{
			if( pItem->m_Vnum != 763 && pItem->m_Vnum != 764 )
				return false;
		}
		else if( g_pRoh->m_Wearing[3]->m_Vnum == 745 ||			// 충전포
				 g_pRoh->m_Wearing[3]->m_Vnum == 750 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 950 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 951
			   )
		{
			if( pItem->m_Vnum != 757 && pItem->m_Vnum != 758 )
				return false;
		}
		else if( g_pRoh->m_Wearing[3]->m_Vnum == 741 ||			// 화염
				 g_pRoh->m_Wearing[3]->m_Vnum == 746 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 751 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 952 ||
				 g_pRoh->m_Wearing[3]->m_Vnum == 953
			   )
		{
			if( pItem->m_Vnum != 759 && pItem->m_Vnum != 760 )
				return false;
		}
		else if( g_pRoh->m_Wearing[3]->m_Vnum == 759 )
		{
			if( pItem->m_Vnum != 767 )
				return false;
		}
	}
	else if( g_pRoh->m_Race == RACE_FREAK )
	{
		if( !_IsSubArms(g_pRoh->m_Wearing[3] ) )
			return false;

		if( g_pRoh->m_Wearing[3]->m_Special > (g_pRoh->m_Wearing[3]->m_Num3 * 0.1) )
			return false;

		if( g_pRoh->m_Wearing[3]->m_Vnum == 1143 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 1155 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 3066 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 10373 || // Update Elite
				g_pRoh->m_Wearing[3]->m_Vnum == 10416 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 11064 ||
				g_pRoh->m_Wearing[3]->m_Vnum == 11085
		  )
		{
			if( pItem->m_Vnum != 1160 )
				return false;
		}
	}
	else if( g_pRoh->m_Race == RACE_PEROM )
	{
		if( !_IsSubArms(g_pRoh->m_Wearing[WEARING_CLOAK] ) )
			return false;

		if( g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special > 5 )
			return false;
	}

	_AddChat( "Use secondary filler." );
	_UseQuickItem( m_dwSubArmsKey );

	return true;
}

//-----------------------------------------------------------------------------
// Name : _IsSubArm
// Date : 2009.10.22
// Desc : 서브타입 아이템인가?
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_IsSubArms(CItem* pItem)
{
	if( !pItem )
		return false;

	if( GET_TYPE(pItem) != ITYPE_SUB )
		return false;

	int shape = GET_SHAPE(pItem);

	if( g_pRoh->m_Race == RACE_BULKAN && shape == ISUB_SLAYER )
		return true;
	if( g_pRoh->m_Race == RACE_KAILIPTON && shape == ISUB_WAND )
		return true;
	if( g_pRoh->m_Race == RACE_HUMAN && shape == ISUB_L_EQUIP )
		return true;
	if( g_pRoh->m_Race == RACE_FREAK && shape == ISUB_DSTONE )
		return true;
	if( g_pRoh->m_Race == RACE_AIDIA && shape == ISUB_WAND )
		return true;
	if( g_pRoh->m_Race == RACE_PEROM && shape == ISUB_PANEL )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Name : _OnAdiaSummon
// Date : 2009.10.22
// Desc : 에이디아 소환 - 스킬, 아이템체크
//		  m_last_quick_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnAdiaSummon(DWORD dt)
{
	if( g_pRoh->m_Race != RACE_AIDIA  )
		return false;

	if( !m_bAdiaSummon )
		return false;

	if( m_last_quick_tick != 0 )
		return false;

	if( ( pCMyApp->m_pUIMgr->m_pSummonMob[0] == NULL && pCMyApp->m_pUIMgr->m_pSummonMob[1] == NULL) &&
			( m_pSelectedMob && !m_pSelectedMob->IsDead() ) )
	{
		_AddChat( "Summons." );
		_UseQuickItem( m_dwAdiaSummonKey );

		return true;
	}
	else
	{
		if( pCMyApp->m_pUIMgr->m_pSummonMob[0] != NULL )
		{
			if( pCMyApp->m_pUIMgr->m_pSummonMob[0]->m_Vital < m_fAdiaSummonPercent ||
					m_pSelectedMob && m_pSelectedMob->IsDead() )
			{
				_AddChat( "Unsubscribe." );
				_UseQuickItem( m_dwAdiaSummonKey );

				return true;
			}
		}
		if( pCMyApp->m_pUIMgr->m_pSummonMob[1] != NULL )
		{
			if( pCMyApp->m_pUIMgr->m_pSummonMob[1]->m_Vital < m_fAdiaSummonPercent ||
					m_pSelectedMob && m_pSelectedMob->IsDead() )
			{
				_AddChat( "Unsubscribe." );
				_UseQuickItem( m_dwAdiaSummonKey );

				return true;
			}
		}
	}

	return false;
}


// 캐릭터가 공격 이후 행동들을 하기위해선 false가 되어야한다.
// 어차피 캐릭터와 팻은 따로 놀기때문에
// 설정중 캐릭터사냥안하기일경우에만 true
bool CAutoPlay_EX::_OnPet(DWORD dt)
{
	if( !m_bPetActive )
		return false;

	if( _OnCallSign(dt) )
		return true;

	if( g_MyPetStatus.mode < MYPET_MODE_HOLD )
	{
		if( !m_bHuntWithMaster )
			return true;
		return false;
	}

	if( _OnPickApple(dt) )
		return true;

	if( pCMyApp->m_pMyPet && m_bUsedAvoid && CalcRatio( pCMyApp->m_pMyPet->m_Vital , pCMyApp->m_pMyPet->m_MaxVital ) < m_uPetAvoidVital )
		ChangeMode( MYPET_MODE_WIMPY );
	else
		ChangeMode( GetType(m_yHuntingType) );

	if( !m_bHuntWithMaster )
		return true;

	return false;
}

bool CAutoPlay_EX::_OnCallSign(DWORD dt)
{
	if( !m_bUsedCallSign )
		return false;

	if( g_MyPetStatus.mode != MYPET_MODE_KEEP )
		return false;

	if( m_last_quick_tick != 0 )
		return false;

	if( m_last_callsign_tick == 0 && g_pRoh->m_QuickCount[m_uCallKeyMapping] > 0 )
	{
		_AddChat( "Use a cost pager." );
		_UseQuickItem(m_uCallKeyMapping);
		m_last_callsign_tick = dt;

		return true;
	}
	else if( _CheckElapsedTime(dt, m_last_callsign_tick , 5000) )
	{
		m_last_callsign_tick = 0;
	}

	return false;
}

bool CAutoPlay_EX::_OnPickApple(DWORD dt)
{
	if( g_MyPetStatus.mode <= MYPET_MODE_KEEP )
		return false;

	if( m_apple_sendmsg != 0 )
	{
		if( m_apple_sendmsg == 1 )
			return false;

		static DWORD olddt = 0;
		if(olddt == 0)
			olddt = dt;

		if( dt - olddt < 100 )
			return false;

		olddt = 0;
		m_apple_sendmsg = 0;
		return false;
	}

	int max_apple = 1;
	if( g_MyPetStatus.m_pBag )
		max_apple = 6;

	if( g_MyPetStatus.food_count >= max_apple )
		return false;

	for( int pack=0 ; pack<PACK_NUM ; pack++ )
	{
		CItem * pAppleItem = g_pRoh->GetInvenItem(pack , 618 , true);
		if( pAppleItem )
		{
			char msg_buf[64] = {0,};
			sprintf( msg_buf , "moving_apple %d %d\n" , pack , pAppleItem->m_Index );
			LH_SEND_NET_MSG( msg_buf );
			m_apple_sendmsg = 1;

			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name :
// Date : 2009.10.27
// Desc : 회피몹이나 캐릭이 있을경우 귀환한다.(true)
//		  대기시간!!!
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnAvoid(DWORD dt)
{
	if( false == m_bAvoid )
		return false;

	if( false == m_szAvoidMobList.empty()  )
	{
		CNkMob* pMob = NULL;
		char temp[128] = {0,};

		LHMobMap_it prMob = pCMyApp->m_pUIMgr->m_mob_map.begin();

		for( ; prMob != pCMyApp->m_pUIMgr->m_mob_map.end() ; prMob++ )
		{
			pMob = (*prMob).second;
			sprintf( temp, "%s%d", pMob->GetMobName(), strlen( pMob->GetMobName() ) );

			float distance = DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz );
			if( _IsDungeon() )
				distance *= 0.5f;

			if( strstr( m_szAvoidMobList.c_str(), temp ) && distance < m_fStayRange )
			{
				return true;
			}
		}
	}

	if( false == m_szAvoidUserList.empty() )
	{
		CNkCharacter* pChar = NULL;
		char temp[128] = {0,};

		LHCharacterMap_it prChar = pCMyApp->m_pUIMgr->m_character_map.begin();

		for( ; prChar != pCMyApp->m_pUIMgr->m_character_map.end() ; prChar++ )
		{
			pChar = (*prChar).second;
			sprintf( temp, "%s%d", pChar->m_pstrName, strlen( pChar->m_pstrName ) );

			if( strstr( m_szAvoidUserList.c_str(), temp ) )
			{
				return true;
			}
		}
	}

	return false;
}


#define _LAGRUSH_PICK_ITEM_RANGE_

bool CAutoPlay_EX::_OnDecideTargetItem(DWORD dt)
{
	if( !m_bItem || (m_pSelectedMob && !m_pSelectedMob->IsDead() ) )
		return false;

	if( g_pRoh->IsFullInvenInAllPack() )
	{
		m_bItem = false;
		return false;
	}

	char temp[128] = {0,};
	float itemLimitDistance = 500.0f;
	CItemInfo* pItemInfo = NULL;
	LHItemInfoMap_it prItem;
	float distance = 0.0f;

	float ShortestDistance = m_fStayRange;
	static DWORD time = timeGetTime();
	static u_short tryCount = 0;

	if( m_pTargetItem == NULL )
	{
		for( prItem = pCMyApp->m_pUIMgr->m_iteminfo_map.begin(); prItem != pCMyApp->m_pUIMgr->m_iteminfo_map.end(); prItem ++ )
		{
			pItemInfo = (*prItem).second;
			CItem* pItem = &pItemInfo->m_Item;

			if( pItem->m_Dropping > -1 || pItem->m_ProtoNum < 1 || pItem->m_X < 0 || pItem->m_Z < 0 )
				continue;

			if( pItem->m_Dropping < 0 )			// if m_Dropping is smaller than 0, that's dropped Item on field.
			{
				std::vector<int>::iterator itr = find( m_vecPickItemList.begin(), m_vecPickItemList.end(), pItem->m_Vnum );
				if( itr == m_vecPickItemList.end() )
					continue;

				if( pItem->m_X < 0 || pItem->m_Z < 0 )
					break;

				float stay_distance = fabs ( DistPlToPl( m_fStayPosX, m_fStayPosZ, pItem->m_X, pItem->m_Z ) );
				if( stay_distance > m_fStayRange )
					continue;

				distance = DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, pItem->m_X, pItem->m_Z );
				if( _IsDungeon() )
					distance *= 0.5f;
				if( distance > 0.1f && distance < ShortestDistance )
				{
					m_pTargetItem = pItemInfo;
					if( m_pTargetItem->m_Item.m_X < 0 || m_pTargetItem->m_Item.m_Z < 0 )
						break;
					ShortestDistance= distance;
					if( distance < 15.0f )		// if distance is smaller than 15.0f, pick up.
						break;
				}
			}
		}
	}

	if( m_pTargetItem )
	{
		_ChangeState( ITEM );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _OnPickItem
// Desc : 아이템집기 상태!
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnPickItem(DWORD dt)
{
	if( m_pTargetItem == NULL )
		return false;
	if( m_last_action_tick != 0 )
		return false;

	char commOut[512];
	sprintf( commOut, "pick %d\n", m_pTargetItem->m_Item.m_Index);

	float dist = DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, m_pTargetItem->m_Item.m_X, m_pTargetItem->m_Item.m_Z);
	if( _IsDungeon() )
		dist *= 0.5f;

	if ( dist <= 15.0f)
	{
		if( m_pTargetItem->m_Item.m_Index < 0 )
		{
			_asm nop;
			return false;
		}
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage(commOut);

		pCMyApp->m_pUIMgr->m_bPickLock = TRUE;
		_ChangeState( ATTACK );
		return true;
	}
	else
	{
		_OnMove(dt , m_pTargetItem->m_Item.m_X, m_pTargetItem->m_Item.m_Z );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CAutoPlay_EX::_OnNextSkill(int next_skill)
{
	int skill_slot = next_skill;

	while(true)
	{
		if( skill_slot >= FUNCTION_NUM )
			skill_slot = 0;

		if( m_bSkillArray[skill_slot] )
		{
			m_skill_slot = skill_slot;
			break;
		}

		skill_slot++;
	}
}

//-----------------------------------------------------------------------------
//	Name : _OnSetSkill
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::_OnSetSkill(bool bNext)
{
	int skill_slot = m_skill_slot;
	bool selected_skill = false;

	if( bNext )
	{
		for( int i=0 ; i<8 ; i++ )
		{
			skill_slot++;
			if( skill_slot >= 8 )
				skill_slot = 0;

			if( m_bSkillArray[skill_slot] )
			{
				int which_button = m_bSkillDirArray[skill_slot];
				int down_key = m_dwSkillKeyArray[skill_slot]>0?m_dwSkillKeyArray[skill_slot]-1:7;
				int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
				int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];


				if( cur_skill < 0 )
					continue;

				if( g_Skills[skill_index].m_bAttack )
				{
					if( g_Skills[skill_index].m_Kind == 1 )
					{
						if( which_button == 0 && g_Skills[skill_index].m_bLeft )
							g_pNk2DFrame->SetLSkill( cur_skill );
						else if( which_button == 1 && g_Skills[skill_index].m_bRight )
							g_pNk2DFrame->SetRSkill( cur_skill );

						m_skill_slot = skill_slot;
						break;
					}
				}
				else
				{
					if( !m_devine_stone )
						continue;

					if( skill_index == SKILLF_A_DSTONE || skill_index == SKILLF_A_TIMECHARGE )
					{
						if( which_button == 0 && g_Skills[skill_index].m_bLeft )
							g_pNk2DFrame->SetLSkill( cur_skill );
						else if( which_button == 1 && g_Skills[skill_index].m_bRight )
							g_pNk2DFrame->SetRSkill( cur_skill );

						m_skill_slot = skill_slot;
						break;
					}
				}
			}
		}
	}
	else
	{
		for( int i=0 ; i<8 ; i++ )
		{
			if( skill_slot >= 8 )
				skill_slot = 0;

			if( m_bSkillArray[skill_slot] )
			{
				int which_button = m_bSkillDirArray[skill_slot];
				int down_key = m_dwSkillKeyArray[skill_slot]>0?m_dwSkillKeyArray[skill_slot]-1:7;
				int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
				int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

				if( cur_skill < 0 )
				{
					skill_slot++;
					continue;
				}

				// 오류예상지점 2010.03.24.
				if( g_Skills[skill_index].m_bAttack )
				{
					m_skill_slot = skill_slot;
					if( which_button == 0 && g_Skills[skill_index].m_bLeft )
						g_pNk2DFrame->SetLSkill( cur_skill );
					else if( which_button == 1 && g_Skills[skill_index].m_bRight )
						g_pNk2DFrame->SetRSkill( cur_skill );

					m_skill_slot = skill_slot;
					break;
				}
				else
				{
					if( !m_devine_stone )
					{
						skill_slot++;
						continue;
					}

					if( skill_index == SKILLF_A_DSTONE || skill_index == SKILLF_A_TIMECHARGE )
					{
						if( which_button == 0 && g_Skills[skill_index].m_bLeft )
							g_pNk2DFrame->SetLSkill( cur_skill );
						else if( which_button == 1 && g_Skills[skill_index].m_bRight )
							g_pNk2DFrame->SetRSkill( cur_skill );
						m_skill_slot = skill_slot;
						break;
					}
				}
			}

			skill_slot++;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name : _OnSkillTimer
//	Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnSkillTimer(DWORD dt)
{
	if( !m_bUseSkill )
		return false;

	bool change_skill = false;

	if( 0 == m_used_skill_delay )
	{
		_OnSetSkill(false);
	}
	else if( m_used_skill_delay && dt - m_used_skill_delay > m_used_limit_delay )
	{
		_OnSetSkill(true);
	}
	else
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name : _OnAttack
//	Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnAttack(DWORD dt)
{
	if( !m_pSelectedMob )
		return false;

	if( m_pSelectedMob && m_pSelectedMob->IsDead() )
	{
		m_need_surport = -1;
		if( pCMyApp->m_pUIMgr->m_b_MotionForAttack )
			return true;

		return false;
	}

	if( !_IsSpecialMob(m_pSelectedMob->m_nMobNum) )
	{
		switch( _IsWarning() )
		{
		case LR_Warning__None:
			break;
		case LR_Warning__InRange:
			_ChangeState(ATTACK);
			_AddChat_Debug( "Found a strike monster (within range)." );
			return false;
		case LR_Warning__OutRange:
			_ChangeState(RETURN_TO_BASE);
			_AddChat_Debug( "Found a strike monster (out of range)." );
			return false;
		}
	}

	float dist = DistPlToPl(m_fStayPosX, m_fStayPosZ, m_pSelectedMob->m_wx, m_pSelectedMob->m_wz);
	float dist2 = DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, m_pSelectedMob->m_wx, m_pSelectedMob->m_wz);
	float dist3 = DistPlToPl(m_fStayPosX, m_fStayPosZ, g_pRoh->m_wx, g_pRoh->m_wz);
	float dist4 = DistPlToPl(m_fStayPosX, m_fStayPosZ, m_pSelectedMob->m_wx, m_pSelectedMob->m_wz);

	if( _IsDungeon() )
	{
		dist *= 0.5f;
		dist2 *= 0.5f;
	}

	if( dist > m_fStayRange )
	{
		_AddChat_Debug( "During the attack, the mob went out of range." );
		_ChangeState( NONE_STATE );
		return false;
	}

	if( dist3 > m_fStayRange )
	{
		_AddChat_Debug( "During the attack, the character went out of range." );
		_ChangeState( RETURN_TO_BASE );
		return false;
	}

// 	if( dist3 > m_fStayRange )
// 	{
// 		_ChangeState( NONE_STATE );
// 		return false;
// 	}

	D3DVECTOR vWorldPos, vScreenPos;
	D3DMATRIX matPipe;
	ZeroMemory( &matPipe, sizeof( D3DMATRIX ) );

	int mx = 0, my = 0;
	vWorldPos = D3DVECTOR( m_pSelectedMob->m_wx, m_pSelectedMob->m_wy, m_pSelectedMob->m_wz );
	pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipe );
	D3DMath_VectorMatrixMultiply( vScreenPos, vWorldPos, matPipe );

	mx = vScreenPos.x;
	my = vScreenPos.y;
	if( mx < 0 )	mx = 0;
	if( my < 0 )	my = 0;

	pCMyApp->m_pUIMgr->SetPickColor(m_pSelectedMob->m_wColorIdx);

	if( !pCMyApp->m_pUIMgr->m_b_Attack )
	{
		bool try_attack = pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );
		char buf[128];
		sprintf( buf , "Attack Click : %d" , try_attack );
		_AddChat_Debug( buf );
		AddLog( buf );

		if( g_pRoh->GetMotionState() == CHA_STAND )
		{
			mx = 0;
			my = 0;

			float temp_dist = 100.0f;
// 			if( _IsDungeon() )
// 				temp_dist *= 0.5f;
			switch( dt % 8 )
			{
			case 0:
				mx += temp_dist;
				break;
			case 1:
				mx -= temp_dist;
				break;
			case 2:
				my += temp_dist;
				break;
			case 3:
				my -= temp_dist;
			case 4:
				mx += temp_dist;
				my += temp_dist;
				break;
			case 5:
				mx -= temp_dist;
				my -= temp_dist;
				break;
			case 6:
				mx += temp_dist;
				my -= temp_dist;
				break;
			case 7:
				mx -= temp_dist;
				my += temp_dist;
				break;
			}

			sprintf( buf , "Attack Click: Motion Stand Let's move slightly" );
			_AddChat_Debug( buf );
			AddLog(buf);

			_OnMove(dt , g_pRoh->m_wx + mx, g_pRoh->m_wz + my );
		}
		else if( g_pRoh->GetMotionState() == CHA_SWIM || g_pRoh->GetMotionState() == CHA_SWIM_STAND )
		{
			sprintf( buf , "Attack Click : In Water...change state" );
			_AddChat_Debug( buf );
			_ChangeState( RETURN_TO_BASE );
			return false;
		}
		return try_attack;
	}

	if( pCMyApp->m_pUIMgr->m_b_MovingForAttack )
	{
		bool try_attack = pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );
		char buf[128];
		sprintf( buf , "Click during attack movement : %d , %d" , try_attack , g_pRoh->GetMotionState() );
		_AddChat_Debug( buf );
		AddLog(buf);
		return try_attack;
	}

	// 공격중!!!
	if( _OnSkillTimer( dt ) )
	{
		int which_button = m_bSkillDirArray[m_skill_slot];
		int down_key = m_dwSkillKeyArray[m_skill_slot]>0?m_dwSkillKeyArray[m_skill_slot]-1:7;
		int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
		int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

		if( cur_skill < 0 )
		{
		}
		else
		{
			if( g_Skills[skill_index].m_bAttack )
			{	
				pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , mx , my );
				m_used_skill_delay = dt;
				m_used_limit_delay = m_wSkillLimitTime[m_skill_slot];
			}
			else
			{
				if( g_pRoh->m_Race == RACE_FREAK )
				{
					if( skill_index == SKILLF_A_DSTONE || skill_index == SKILLF_A_TIMECHARGE )
					{
						{
							bool try_attack = pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , mx , my );
							char buf[128];
							sprintf( buf , "Divine Related : %d" , try_attack );
							_AddChat_Debug( buf );
							AddLog(buf);
						}

						m_used_skill_delay = dt;
						m_used_limit_delay = m_wSkillLimitTime[m_skill_slot];
					}
					else
					{
						m_used_skill_delay = 0;
						m_used_limit_delay = 0;
					}
				}
				else if( g_pRoh->m_Race == RACE_PEROM )
				{
					if( skill_index == PEROM_SKILL_S134 ||
							skill_index == PEROM_SKILL_S148 ||
							skill_index == PEROM_SKILL_S149 ||
							skill_index == PEROM_SKILL_S150 )
					{
						bool try_attack = pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , mx , my );
						m_used_skill_delay = dt;
						m_used_limit_delay = m_wSkillLimitTime[m_skill_slot];
					}
					else
					{
						m_used_skill_delay = 0;
						m_used_limit_delay = 0;
					}
				}
				else
				{
					m_used_skill_delay = 0;
					m_used_limit_delay = 0;
				}
			}
		}
	}
	else
	{
		// 기본공격!!!
	}



	if( !pCMyApp->m_pUIMgr->m_b_MotionForAttack )
	{
		return false;
	}

	return true;
}

bool CAutoPlay_EX::_OnAttack_PotionSkill(DWORD dt)
{
	if( m_pSelectedMob && m_pSelectedMob->IsDead() )
		return false;

	D3DVECTOR vWorldPos, vScreenPos;
	D3DMATRIX matPipe;
	ZeroMemory( &matPipe, sizeof( D3DMATRIX ) );

	int mx = 0, my = 0;
	vWorldPos = D3DVECTOR( m_pSelectedMob->m_wx, m_pSelectedMob->m_wy, m_pSelectedMob->m_wz );
	pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipe );
	D3DMath_VectorMatrixMultiply( vScreenPos, vWorldPos, matPipe );

	mx = vScreenPos.x;
	my = vScreenPos.y;
	if( mx < 0 )	mx = 0;
	if( my < 0 )	my = 0;

	pCMyApp->m_pUIMgr->SetPickColor(m_pSelectedMob->m_wColorIdx);

	if( !pCMyApp->m_pUIMgr->m_b_Attack )
		return pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );

	if( pCMyApp->m_pUIMgr->m_b_MovingForAttack )
		return pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );

	if( _OnSkillTimer( dt ) )
	{
		int which_button = m_bSkillDirArray[m_skill_slot];
		int down_key = m_dwSkillKeyArray[m_skill_slot]>0?m_dwSkillKeyArray[m_skill_slot]-1:7;
		int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
		int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

		if( cur_skill < 0 )
		{
		}
		else
		{
			if( g_Skills[skill_index].m_bAttack )
			{
				pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , mx , my );
				m_used_skill_delay = dt;
				m_used_limit_delay = m_wSkillLimitTime[m_skill_slot];
			}
			else
			{
				if( g_pRoh->m_Race == RACE_FREAK )
				{
					if( skill_index == SKILLF_A_DSTONE || skill_index == SKILLF_A_TIMECHARGE )
					{
						{
							bool try_attack = pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , mx , my );
							char buf[128];
							sprintf( buf , "Divine Related : %d" , try_attack );
							_AddChat_Debug( buf );
							AddLog(buf);
						}

						m_used_skill_delay = dt;
						m_used_limit_delay = m_wSkillLimitTime[m_skill_slot];
					}
					else
					{
						m_used_skill_delay = 0;
						m_used_limit_delay = 0;
					}
				}
				else if( g_pRoh->m_Race == RACE_PEROM )
				{
					if( skill_index == PEROM_SKILL_S134 ||
						skill_index == PEROM_SKILL_S148 ||
						skill_index == PEROM_SKILL_S149 ||
						skill_index == PEROM_SKILL_S150 )
					{
						bool try_attack = pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , mx , my );
						m_used_skill_delay = dt;
						m_used_limit_delay = m_wSkillLimitTime[m_skill_slot];
					}
					else
					{
						m_used_skill_delay = 0;
						m_used_limit_delay = 0;
					}
				}
				else
				{
					m_used_skill_delay = 0;
					m_used_limit_delay = 0;
				}
			}
		}
	}

	if( !pCMyApp->m_pUIMgr->m_b_MotionForAttack )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnDecideTarget(DWORD dt)
{
	if( m_pSelectedMob && !m_pSelectedMob->IsDead() )
		return true;

	// 몹이 죽고있다....딜레이...살짝 주자!!!

	bool target_special = false;
	if( !m_vecFirstTarget.empty() )
		target_special = true;

	LHMobMap & mobmap = pCMyApp->m_pUIMgr->m_mob_map;

	if( pCMyApp->m_pUIMgr->m_mob_map.empty() )
	{
		if( !m_hunt_move )
		{
			return false;
		}
		else
		{
			static DWORD olddt = 0;
			if( olddt == 0 )
				olddt = dt;

			if( dt - olddt < 3000 )
			{
				return false;
			}

			olddt = 0;

			int mx = 0;
			int my = 0;
			{
				float temp_dist = 100.0f;
				// 			if( _IsDungeon() )
				// 				temp_dist *= 0.5f;
				switch( dt % 8 )
				{
				case 0:
					mx += temp_dist;
					break;
				case 1:
					mx -= temp_dist;
					break;
				case 2:
					my += temp_dist;
					break;
				case 3:
					my -= temp_dist;
				case 4:
					mx += temp_dist;
					my += temp_dist;
					break;
				case 5:
					mx -= temp_dist;
					my -= temp_dist;
					break;
				case 6:
					mx += temp_dist;
					my -= temp_dist;
					break;
				case 7:
					mx -= temp_dist;
					my += temp_dist;
					break;
				}
				AddLog("[DecideTarget] : There are no mobs, so move around once every 3 seconds");
				_OnMove(dt , g_pRoh->m_wx + mx, g_pRoh->m_wz + my );
			}
			return false;
		}
	}

	CNkMob * pMob = NULL;
	CNkMob * pTargetMob = NULL;
	CNkMob * pTargetMob_Special = NULL;

	float shortest = m_fStayRange;
	float shortest_special = m_fStayRange;
	int positionInVec = m_vecFirstTarget.size();

	bool found_spacial = false;

	m_pSelectedMob = NULL;

	LHMobMap_it it = mobmap.begin();

	if( m_pPrevSelectedMob )
	{
		LHMobMap_it pit = mobmap.find(m_pPrevSelectedMob->m_nVirId);
		if( pit==mobmap.end() )
			m_pPrevSelectedMob = NULL;
	}

	for( ; it!= mobmap.end() ; ++it )
	{
		pMob = (*it).second;

		if( pMob->IsDead() )
		{
			continue;
		}

		if( (pMob->m_Flag & NPC_PROCEDURE) || (pMob->m_Flag & NPC_SHOPKEEPER) || (pMob->m_Flag & NPC_GUARD) ||
				(pMob->m_Flag & NPC_FISH) || (pMob->m_Flag & NPC_SUMMON) || ( (pMob->m_Flag & NPC_NORADER) && !( pMob->m_nMobNum == 678 || pMob->m_nMobNum == 679 || pMob->m_nMobNum == 834 || pMob->m_nMobNum == 835 || pMob->m_nMobNum == 836 || pMob->m_nMobNum == 837 ) ) ||
				pMob->m_nMobNum == MONSTER_CONTROL_DAMAGE || pMob->m_nMobNum == MONSTER_CONTROL_GUARD ||
				(pMob->m_Flag & NPC_QUEST) || pMob->m_nMobNum == 226
				|| !( pMob->m_nMobNum < 420 || pMob->m_nMobNum > 424 )
				|| !strcmp( "Snowcat", pMob->GetMobName() )
				|| pMob->m_nMobNum == 367 || pMob->m_nMobNum == 368	// 라이프스톤,퀘이크스톤
				|| pMob->m_nMobNum == 778 || pMob->m_nMobNum == 5080
				|| pMob->m_nMobNum == 5081 || pMob->m_nMobNum == 5085
				|| pMob->m_nMobNum == 5086// 청룡의 인장
		  )
		{
			continue;
		}

		if( pMob->CheckRenderHolloweenMob(pMob->m_nMobNum) == false )
		{
			continue;
		}

		float stay_distance = fabs ( DistPlToPl( m_fStayPosX, m_fStayPosZ, pMob->m_wx, pMob->m_wz ) );
		if( _IsDungeon() )
			stay_distance *= 0.5f;

		if( stay_distance > m_fStayRange )
		{
			continue;
		}

		float distance = fabs ( DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz ) );
		if( _IsDungeon() )
			distance *= 0.5f;

		if( target_special )
		{
			if( _IsSpecialMob(pMob->m_nMobNum) )
			{
				found_spacial = true;
				if( distance > 0.1f && distance < shortest_special )
				{
					shortest_special = distance;
					pTargetMob_Special = pMob;
				}
			}
		}

		if( false == found_spacial )
		{
			if( distance > 0.1f  && distance < shortest )
			{
				shortest = distance;
				pTargetMob = pMob;
			}
		}
	}

	if( pTargetMob_Special )
	{
		m_pSelectedMob = pTargetMob_Special;
	}
	else
	{
		if( pTargetMob )
			m_pSelectedMob = pTargetMob;
		else
			m_pSelectedMob = NULL;
	}

	if( m_pSelectedMob )
	{
		pCMyApp->StopHero();
		pCMyApp->m_pUIMgr->SetPickColor(m_pSelectedMob->m_wColorIdx);

		if( m_bUseSkill )
		{
			if( m_bRotateSkill )
			{
				m_used_skill_delay = 0;
				m_used_limit_delay = 0;
			}
			else
			{
				m_skill_slot = 0;
				m_used_skill_delay = 0;
				m_used_limit_delay = 0;
			}
		}

		return true;
	}

	m_pSelectedMob = NULL;
	pCMyApp->SetHeroAttGoMob( NULL );

	_AddChat_Debug( "There are mobs, but they're out of range ... or they can not move because of obstacles..\n" );
	AddLog( "[DecideTarget] : There are mobs, but they're out of range ... or they can not move because of obstacles." );

	float stay_distance = fabs ( DistPlToPl( m_fStayPosX, m_fStayPosZ, g_pRoh->m_wx, g_pRoh->m_wz ) );
	if( stay_distance > m_fStayRange )
	{
		_ChangeState( RETURN_TO_BASE );
		return false;
	}

	if( g_pRoh->GetMotionState() == CHA_STAND )
	{
		int mx = 0;
		int my = 0;

		if( !m_hunt_move )
		{
			_ChangeState( RETURN_TO_BASE );
			return false;
		}
		else
		{
			float temp_dist = 100.0f;

			switch( dt % 8 )
			{
			case 0:
				mx += temp_dist;
				break;
			case 1:
				mx -= temp_dist;
				break;
			case 2:
				my += temp_dist;
				break;
			case 3:
				my -= temp_dist;
			case 4:
				mx += temp_dist;
				my += temp_dist;
				break;
			case 5:
				mx -= temp_dist;
				my -= temp_dist;
				break;
			case 6:
				mx += temp_dist;
				my -= temp_dist;
				break;
			case 7:
				mx -= temp_dist;
				my += temp_dist;
				break;
			}
			_OnMove(dt , g_pRoh->m_wx + mx, g_pRoh->m_wz + my );
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_IsSpecialMob(int mobnum)
{
	if( m_vecFirstTarget.empty() )
		return false;

	std::vector<int>::iterator it_sp = find( m_vecFirstTarget.begin(), m_vecFirstTarget.end(), mobnum );
	if( it_sp == m_vecFirstTarget.end() )
		return false;

	int char_motion = g_pRoh->GetMotionState();
	if( char_motion == CHA_B_BURNINGCRASH || char_motion == CHA_B_WILDSMASH )
		return false;

	if( char_motion == CHA_HELLCRY_ATTACK ||
			char_motion == CHA_B_FLYINGCRASH ||
			char_motion == CHA_H_ELECTRICSHOT ||
			char_motion == CHA_H_AUTOMINE ||
			char_motion == CHA_H_RANDOMFIRE_ME ||
			char_motion == CHA_A_SUMMONFIRE ||
			char_motion == CHA_F_ELECTRICHAMMER ||
			char_motion == CHA_K_ICEBOLT ||
			char_motion == CHA_F_TONADOHAMMER ||
			char_motion == CHA_F_MERCERNERYSLASH ||
			char_motion == CHA_K_POISON ||
			char_motion == CHA_K_FLAMESPEAR ||
			char_motion == CHA_F_SHADOWSLASH ||
			char_motion == CHA_F_SPACESLASH ||
			char_motion == CHA_C_HELLDOWN ||
			char_motion == CHA_C_HELLDOWN2 )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LR_Warning_Type CAutoPlay_EX::_IsWarning()
{
	if( m_vecFirstTarget.empty() )
		return LR_Warning__None;

	if( _IsSpecialMob(m_pSelectedMob->m_nMobNum) )
		return LR_Warning__None;

	bool isIn = false;
	bool isOut = false;

	CNkMob * pMob = NULL;
	CNkMob * pTargetMob = NULL;
	CNkMob * pTargetMob_Special = NULL;

	LHMobMap & mobmap = pCMyApp->m_pUIMgr->m_mob_map;
	LHMobMap_it it = mobmap.begin();

	for( ; it!= mobmap.end() ; ++it )
	{
		pMob = (*it).second;

		if( pMob->IsDead() )
			continue;

		if( (pMob->m_Flag & NPC_PROCEDURE) || (pMob->m_Flag & NPC_SHOPKEEPER) || (pMob->m_Flag & NPC_GUARD) ||
				(pMob->m_Flag & NPC_FISH) || (pMob->m_Flag & NPC_SUMMON) || ( (pMob->m_Flag & NPC_NORADER) && !( pMob->m_nMobNum == 678 || pMob->m_nMobNum == 679 || pMob->m_nMobNum == 834 || pMob->m_nMobNum == 835 || pMob->m_nMobNum == 836 || pMob->m_nMobNum == 837) ) ||
				pMob->m_nMobNum == MONSTER_CONTROL_DAMAGE || pMob->m_nMobNum == MONSTER_CONTROL_GUARD ||
				(pMob->m_Flag & NPC_QUEST) || pMob->m_nMobNum == 226
				|| !( pMob->m_nMobNum < 420 || pMob->m_nMobNum > 424 )
				|| !strcmp( "스노우캣", pMob->GetMobName() ) )
			continue;

		if( pMob->CheckRenderHolloweenMob(pMob->m_nMobNum) == false )
		{
			continue;
		}

		if( !_IsSpecialMob(pMob->m_nMobNum) )
			continue;

		float stay_distance = fabs ( DistPlToPl( m_fStayPosX, m_fStayPosZ, pMob->m_wx, pMob->m_wz ) );
		float player_dist = fabs ( DistPlToPl( g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz ) );

		if( _IsDungeon() )
		{
			stay_distance *= 0.5f;
			player_dist *= 0.5f;
		}

		if( stay_distance > m_fStayRange )
		{
			if( player_dist > 200.0f )
				continue;

			isOut = true;
		}
		else
		{
			isIn = true;
			return LR_Warning__InRange;
		}
	}

	if( isOut )
	{
		return LR_Warning__OutRange;
	}

	return LR_Warning__None;
}


//-----------------------------------------------------------------------------
//	Name : _OnUseSkill
//	Date : 2009.
//	Desc : 스킬사용 - F1~F8
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnUseSkill(DWORD dt, int x, int y, CNkMob* target)
{
	if( m_last_attack_tick == 0 )
		m_last_attack_tick = dt;

	int which_button = -1;

	if( m_bUseSkill )
	{
		if( m_skill_slot >= FUNCTION_NUM )
			m_skill_slot = 0;

		bool skip = false;
		if( m_bSkillArray[m_skill_slot] )
		{
			if( m_dwSkillCurTime[m_skill_slot] != 0 )
			{
				if(  dt - m_dwSkillCurTime[m_skill_slot] < m_wSkillLimitTime[m_skill_slot] )
				{
				}
				else
				{
					m_dwSkillCurTime[m_skill_slot] = 0;
					//					m_skill_slot++;
					_OnNextSkill(m_skill_slot+1);
					return true;
				}
			}
			else
			{
				which_button = m_bSkillDirArray[m_skill_slot]; // 의미가 쫌...
				int down_key = m_dwSkillKeyArray[m_skill_slot]>0?m_dwSkillKeyArray[m_skill_slot]-1:7;
				int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
				int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

				if( cur_skill < 0 )
				{
				}
				else
				{
					// 오류예상지점 2010.03.24.
					if( g_Skills[skill_index].m_bAttack )
					{
						if( which_button == 0 && g_Skills[skill_index].m_bLeft )
							g_pNk2DFrame->SetLSkill( cur_skill );
						else if( which_button == 1 && g_Skills[skill_index].m_bRight )
							g_pNk2DFrame->SetRSkill( cur_skill );
					}
					else
					{
						g_pNk2DFrame->GetControlBottom()->m_DownKey = down_key;
						g_pNk2DFrame->GetControlBottom()->ActionQuickKey( down_key );
						m_dwSkillCurTime[m_skill_slot] = dt;
					}
					return true;
				}
			}
		}
		else
		{
			_OnNextSkill(m_skill_slot+1);
			return false;
		}
	}
	else
	{
		if( _OnVampTouch(dt) )
			which_button = 1;
	}

	if( which_button == -1 )
	{
		if( !pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , x , y ) )
			return false;
	}
	else
	{
		if( !pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , x , y ) )
			return false;

		if( pCMyApp->m_pUIMgr->m_b_MovingForAttack )
			return false;

		m_dwSkillCurTime[m_skill_slot] = dt;
	}

	m_last_attack_tick = dt;

	return true;
}

bool CAutoPlay_EX::_OnUseSkill(DWORD dt)
{
	if( !m_pSelectedMob )
		return false;

	if( m_pSelectedMob && m_pSelectedMob->IsDead() )
		return false;




	return true;
}

//-----------------------------------------------------------------------------
//	Name : _OnShield
//	Desc : 휴먼,카이 전용 : 스킬...회수?
//		   m_last_action_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnShield(DWORD dt)
{
	if( !m_bShield )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	if( m_using_shield_time !=0 && dt > m_using_shield_time + m_dwShieldTime )
	{
		m_using_shield_time = 0;
		m_using_shield_count = 0;
		return false;
	}

	if( m_dwShieldRepeat <= m_using_shield_count )
		return false;

	int down_key = m_dwShieldKey>0?m_dwShieldKey-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
	if( cur_skill < 0 )
		return false;

	int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

	if( !CheckEquipSkill(cur_skill) )
	{
		m_bShield = false;
		_AddChat( "Wrong skill or equipment required. " );
		_AddChat( "Restrict use of shield." );
		return false;
	}

	if( m_using_shield_count == 0 )
		m_using_shield_time = dt;

	if( false == pCMyApp->SkillConditionCheck(cur_skill) )
		return false;

	m_using_shield_count++;

	if( g_pRoh->m_Race != RACE_PEROM )
	{
		g_pNk2DFrame->GetControlBottom()->m_DownKey = down_key;
		g_pNk2DFrame->GetControlBottom()->ActionQuickKey( down_key );
	}
	else
	{
		g_pNk2DFrame->SetRSkill( cur_skill );

		if( pCMyApp->m_pUIMgr->CmdClick( 1 , 0 , 0, 0 ) )
		{
			m_last_action_tick = dt;
			m_perom_global_cool = dt;

			return true;
		}
		else
		{
			return false;
		}
	}

	m_last_action_tick = dt;

	return true;
}


//-----------------------------------------------------------------------------
//	Name : _OnShield
//	Desc : 필드 EX : 필드&Dsystem 작동중일때만
//		   m_last_action_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnShield2(DWORD dt)
{

	if( !m_bshield2 )
		return false;

	if( g_pRoh->m_Race != RACE_PEROM )
		return false;

	if( !g_pRoh->IsActiveDSystem() || !g_pRoh->IsActiveField() )
		return false;

	if( g_pRoh->IsActiveField_Ex() )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	if( m_using_shield_time2 !=0 && dt > m_using_shield_time2 + m_dwShieldTime )
	{
		m_using_shield_time2 = 0;
		return false;
	}

	int down_key = m_dwShieldKey2>0?m_dwShieldKey2-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
	if( cur_skill < 0 )
		return false;

	int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

	if( !CheckEquipSkill(cur_skill) )
	{
		m_bshield2 = false;
		return false;
	}

	if( false == pCMyApp->SkillConditionCheck(cur_skill) )
		return false;

	g_pNk2DFrame->SetRSkill( cur_skill );

	if( pCMyApp->m_pUIMgr->CmdClick( 1 , 0 , 0, 0 ) )
	{
		m_last_action_tick = dt;
		m_perom_global_cool = dt;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//	Name : _OnShield
//	Desc : 인게이지
//		   m_last_action_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnShield3(DWORD dt)
{

	if( !m_bshield3 )
		return false;

	if( g_pRoh->m_Race != RACE_PEROM )
		return false;

	if( g_pRoh->IsActiveEngage() )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	if( m_using_shield_time3 !=0 && dt > m_using_shield_time3 + m_dwShieldTime )
	{
		m_using_shield_time3 = 0;
		return false;
	}

	int down_key = m_dwShieldKey3>0?m_dwShieldKey3-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
	if( cur_skill < 0 )
		return false;

	int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

	if( !CheckEquipSkill(cur_skill) )
	{
		m_bshield3 = false;
		return false;
	}

	if( false == pCMyApp->SkillConditionCheck(cur_skill) )
		return false;

	g_pNk2DFrame->SetRSkill( cur_skill );

	if( pCMyApp->m_pUIMgr->CmdClick( 1 , 0 , 0, 0 ) )
	{
		m_last_action_tick = dt;
		m_perom_global_cool = dt;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//	Name : _OnShield4
//	Desc : 크리티컬스트라이크
//		   m_last_action_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnShield4(DWORD dt)
{

	if( !m_bshield4 )
		return false;

	if( g_pRoh->m_Race != RACE_PEROM )
		return false;

	if( g_pRoh->IsActiveCritical() )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	if( m_using_shield_time4 !=0 && dt > m_using_shield_time4 + m_dwShieldTime )
	{
		m_using_shield_time4 = 0;
		return false;
	}

	int down_key = m_dwShieldKey4>0?m_dwShieldKey4-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
	if( cur_skill < 0 )
		return false;

	int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

	if( !CheckEquipSkill(cur_skill) )
	{
		m_bshield4 = false;
		return false;
	}

	if( false == pCMyApp->SkillConditionCheck(cur_skill) )
		return false;

	g_pNk2DFrame->SetRSkill( cur_skill );

	if( pCMyApp->m_pUIMgr->CmdClick( 1 , 0 , 0, 0 ) )
	{
		m_last_action_tick = dt;
		m_perom_global_cool = dt;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//	Name : _OnRecovery
//	Desc : 에이디아전용 : 자힐(스킬)
//		   m_last_attack_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnRecovery(DWORD dt)
{
	if( !m_bRecovery || g_pRoh->m_Race != RACE_AIDIA )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	if( CalcRatio( g_pRoh->m_Vital , g_pRoh->m_MaxVital ) > m_fRecoveryPercent )
		return false;

	int down_key = m_dwRecoveryKey>0?m_dwRecoveryKey-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
	if( cur_skill < 0 )
		return false;

	int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

	g_pNk2DFrame->GetControlBottom()->m_DownKey = down_key;
	g_pNk2DFrame->GetControlBottom()->ActionQuickKey( down_key );
	m_last_action_tick = dt;

	return true;
}

//-----------------------------------------------------------------------------
//	Name : _OnBloody
//	Desc : 카이 전용 : 스킬 ok
//		   m_last_action_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnBloody(DWORD dt)
{
	if( !m_bBloodyMana || g_pRoh->m_Race != RACE_KAILIPTON )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	int down_key = m_dwBloodyManaKey>0?m_dwBloodyManaKey-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
	if( cur_skill < 0 )
		return false;

	if( 62 != g_ChaSkill.m_SkillIndex[cur_skill] )
		return false;

	if( CalcRatio( g_pRoh->m_Mana , g_pRoh->m_MaxMana ) > m_fBloodyManaPercent )
		return false;

	if( false == pCMyApp->SkillConditionCheck(cur_skill) )
		return false;

	g_pNk2DFrame->GetControlBottom()->m_DownKey = down_key;
	g_pNk2DFrame->GetControlBottom()->ActionQuickKey( down_key );
	m_last_action_tick = dt;

	return true;
}

//-----------------------------------------------------------------------------
//	Name : _OnBloody
//	Desc : 카이 전용 : 스킬 ok
//		   m_last_action_tick
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnVampTouch(DWORD dt)
{
	if( !m_bVempTouch )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	if( !m_pSelectedMob )
		return false;

	if( m_pSelectedMob && m_pSelectedMob->IsDead() )
		return false;

	if( CalcRatio( g_pRoh->m_Vital , g_pRoh->m_MaxVital ) > m_fVempTouchPercent )
		return false;

	int down_key = m_dwVempTouchKey>0?m_dwVempTouchKey-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
	if( cur_skill < 0 )
		return false;

	// 오류예상지점 2010.03.24.
	int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];
	g_pNk2DFrame->SetRSkill( cur_skill );

	pCMyApp->m_pUIMgr->SetPickColor(m_pSelectedMob->m_wColorIdx);

	if( !pCMyApp->m_pUIMgr->m_b_Attack || !pCMyApp->m_pUIMgr->m_b_MotionForAttack )
	{
		D3DVECTOR vWorldPos, vScreenPos;
		D3DMATRIX matPipe;
		ZeroMemory( &matPipe, sizeof( D3DMATRIX ) );

		int mx = 0, my = 0;
		vWorldPos = D3DVECTOR( m_pSelectedMob->m_wx, m_pSelectedMob->m_wy, m_pSelectedMob->m_wz );
		pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipe );
		D3DMath_VectorMatrixMultiply( vScreenPos, vWorldPos, matPipe );

		mx = vScreenPos.x;
		my = vScreenPos.y;
		if( mx < 0 )	mx = 0;
		if( my < 0 )	my = 0;

		if( !pCMyApp->m_pUIMgr->CmdClick( 1 , 0 , mx , my ) )
			return false;

		if( pCMyApp->m_pUIMgr->m_b_MovingForAttack )
			return false;

		if( !pCMyApp->m_pUIMgr->m_b_Attack )
			return false;
	}

	m_last_attack_tick = dt;

	return true;
}


bool CAutoPlay_EX::_OnDsystem(DWORD dt)
{

	if( !m_use_dsys )
		return false;

	if( g_pRoh->IsActiveDSystem() )
		return false;

	if( m_last_action_tick != 0 )
		return false;

	int down_key = m_dsys_key>0?m_dsys_key-1:7;
	int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;

	if( cur_skill < 0 )
		return false;

	int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

	if( !CheckEquipSkill(cur_skill) )
		return false;

	if( false == pCMyApp->SkillConditionCheck(cur_skill) )
		return false;

	g_pNk2DFrame->SetRSkill( cur_skill );

	if( pCMyApp->m_pUIMgr->CmdClick( 1 , 0 , 0, 0 ) )
	{
		m_last_action_tick = dt;
		m_perom_global_cool = dt;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//	Name : Init()
//	Desc : 라그러쉬 실행시 초기화
//-----------------------------------------------------------------------------
void CAutoPlay_EX::Init(bool playing)
{
	CAutoPlay::Init(playing);

	if( !m_loaded_setting )
	{
		return;
	}

	if( !_StartCheck() )
	{
		_OnInterrupt(LagRush_Interrupt__Error_Setting);
		return;
	}

	if( ( _DontUsingFullAuto() || _IsDungeon() ) && m_dwGameMode == GAMEMODE_FULL_PACKAGE )
	{
		_OnInterrupt(LagRush_Interrupt__Limited_Dungeon);
		return;
	}

	m_running			= true;

	m_change_state		= false;
	m_prev_state		= NONE_STATE;
	m_current_state		= NONE_STATE;

	m_pick_item_time	= 0;
	m_last_quick_tick	= 0;
	m_last_action_tick 	= 0;
	m_last_attack_tick 	= 0;
	m_last_callsign_tick = 0;

	if( g_SvrType == ST_ADULT_ONLY )
		m_shop_state = ShopState__Repair;
	else
		m_shop_state = ShopState__Buy;

	m_skill_slot		= 0;


	m_used_limit_delay	= 0;
	m_used_skill_delay	= 0;

	m_move_to_base_tick = 0;

	m_pPrevSelectedMob = NULL;
	m_move_to_base_dir = 1;
	m_dead_time = 0;

	m_stash_state = StashState__Push;
	int i = 0;
	for( i = 0; i < AH_SPECIALPOTION; i++ )
	{
		m_dwSpecialPotionCurTime[i] = 0;
	}
//	m_dwSpecialPotionCurTime[0] = 0;
//	m_dwSpecialPotionCurTime[1] = 0;
	m_picked_apple = false;

	m_global_attack_time = 0;
	m_shop_sendmsg = 0;
	m_stash_sendmsg = 0;
	m_apple_sendmsg = 0;
	m_devine_stone = false;

	if( pCMyApp->GetCurWorld() == 1 || pCMyApp->GetCurWorld() == 2 || pCMyApp->GetCurWorld() == 5 )
		m_fStayRange *= 0.5f;

	_ChangeState(NONE_STATE);
}

void CAutoPlay_EX::StartProc(DWORD dwGameMode)
{
	if( _IsCantZone() )
	{	
		_OnInterrupt(LagRush_Interrupt__CantZone);
		return;
	}

	if( m_running )
	{
		CheckMode();
		return;
	}

	m_pairPoslist.clear();
	m_vecPickItemList.clear();
	m_vecFirstTarget.clear();
	m_vecAvoidMob.clear();
	m_vecAvoidChar.clear();

	Init_variable();

	ReLoadData();

	if( !m_loaded_setting )
		return;

	SetPos( g_pRoh->m_wx, g_pRoh->m_wz );

	if( dwGameMode != GAMEMODE_AUTO )
		m_dwGameMode = dwGameMode;

	if( !_StartCheck() )
	{
		_OnInterrupt(LagRush_Interrupt__Error_Setting);
		return;
	}

	if( ( _DontUsingFullAuto() || _IsDungeon() ) && m_dwGameMode == GAMEMODE_FULL_PACKAGE )
	{
		_OnInterrupt(LagRush_Interrupt__Limited_Dungeon);
		return;
	}

	m_running			= true;

	m_change_state		= false;
	m_prev_state		= NONE_STATE;
	m_current_state		= NONE_STATE;

	m_pick_item_time	= 0;
	m_last_quick_tick	= 0;
	m_last_action_tick 	= 0;
	m_last_attack_tick 	= 0;
	m_last_callsign_tick = 0;

	if( g_SvrType == ST_ADULT_ONLY )
		m_shop_state = ShopState__Repair;
	else
		m_shop_state = ShopState__Buy;
	m_skill_slot		= 0;


	m_used_limit_delay	= 0;
	m_used_skill_delay	= 0;

	m_move_to_base_tick = 0;

	m_pPrevSelectedMob = NULL;
	m_move_to_base_dir = 1;
	m_dead_time = 0;

	m_stash_state = StashState__Push;
	int i = 0;
	for( i = 0; i < AH_SPECIALPOTION; i++ )
	{
		m_dwSpecialPotionCurTime[i] = 0;
	}

	m_picked_apple = false;

	m_global_attack_time = 0;
	m_shop_sendmsg = 0;
	m_stash_sendmsg = 0;
	m_apple_sendmsg = 0;

	if( pCMyApp->GetCurWorld() == 1 || pCMyApp->GetCurWorld() == 2 || pCMyApp->GetCurWorld() == 5 )
		m_fStayRange *= 0.5f;

	_ChangeState(NONE_STATE);

	if( m_running )
	{
		LH_SEND_NET_MSG( "lr start\n" );
		StartLagrushMsg();
		pCMyApp->m_pUIMgr->m_bAutoHunt = TRUE;

		// m_pDecal->Init( g_pRoh->m_wx , g_pRoh->m_wz , m_fStayRange , 20.0f );

		g_pNk2DFrame->GetNewEvent()->SetEvent( 7 , 1 );
	}
}


//-----------------------------------------------------------------------------
//	Name : StopProc
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::StopProc(int type)
{
	if( m_running && g_pTcpIp )
	{
		LH_SEND_NET_MSG( "lr end\n" );
		EndLagrushMsg();
		pCMyApp->m_pUIMgr->m_bAutoHunt = FALSE;

		g_pNk2DFrame->GetNewEvent()->SetEvent( 7 , 0 );

		m_exception_target.clear();

		// resetall

		m_running = false;
	}
	else
	{
		g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_LHSTRING1852), -1 );
	}
}


//-----------------------------------------------------------------------------
//	Name : Polling
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::Polling()
{
	DWORD dt = GetTickCount();

	if( dt - m_last_quick_tick > QUICK_DELAY )
		m_last_quick_tick = 0;
	if( dt - m_last_action_tick > ACTION_DELAY )
		m_last_action_tick = 0;

	static DWORD olddt = 0;
	if( olddt == 0 )
		olddt = dt;
	if( dt - olddt < 300 )
		return;
	olddt = 0;

	switch( m_dwGameMode )
	{
	case GAMEMODE_POTION_ONLY:
		_Polling_Potion(dt);
		break;
	case GAMEMODE_SKILL_POTION:
		_Polling_PotionSkill(dt);
		break;
	case GAMEMODE_FULL_PACKAGE:
		_Polling_Full(dt);
		break;
	}
}


//-----------------------------------------------------------------------------
//	Name : PassMsg
//	Desc :
//-----------------------------------------------------------------------------
UINT CAutoPlay_EX::PassMsg(UINT msg, UINT wparam, UINT lparam)
{
	// 작동중엔 Insert-delete 키 적용안되게..
	if( m_running && m_dwGameMode == GAMEMODE_FULL_PACKAGE )
		return 0;

	char szMessage[128] = {0,};
	if( msg == WM_KEYDOWN )
	{
		/*if( g_nCurWorldIdx == WORLD_FORLORN_S3 )
		{
			return false;
		}*/

		if( wparam == VK_ADD )
		{
			LoadWalkPosList();
			POSPAIR pos;
			pos.first = g_pRoh->m_wx;
			pos.second = g_pRoh->m_wz;
			m_pairPoslist.push_back( pos );
			int num = m_pairPoslist.size();

			float _x, _z;
			pCMyApp->m_pUIMgr->ConvertWorldPos(g_pRoh->m_wx, g_pRoh->m_wz , _x, _z );
			sprintf( szMessage, "%d Coordinate %d  %d Save.", num, (int)_x, (int)_z );
			_AddChat( szMessage );
			SaveWalkPosList( g_pRoh->m_wx, g_pRoh->m_wz );

			return true;
		}
		else if( wparam == VK_SUBTRACT )
		{
			LoadWalkPosList();

			if( m_pairPoslist.empty() )
			{
				_AddChat( "No coordinates entered." );
				return true;
			}
			int posInList = 0;// = LOWORD( wParam );
			std::vector<POSPAIR>::iterator itr = m_pairPoslist.begin();
			std::vector<POSPAIR>::iterator last_itr = m_pairPoslist.begin();
			for( ; itr!=m_pairPoslist.end() ; itr++ )
			{
				last_itr = itr;
			}

			if( last_itr == m_pairPoslist.end() )
				return false;


			float _x, _z;
			int num = m_pairPoslist.size();
			pCMyApp->m_pUIMgr->ConvertWorldPos( ((POSPAIR)*(last_itr)).first , ((POSPAIR)*(last_itr)).second , _x, _z );
			sprintf( szMessage, "%d Coordinate %d  %d Delete.", num, (int)_x, (int)_z );
			_AddChat( szMessage );
			m_pairPoslist.erase( last_itr );
			SaveWalkPosList();

			return true;
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
//	Name : LoadSetting
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::LoadSetting()
{
	CAutoPlay::LoadSetting();

	m_use_return = false;

	if( m_wReturn )
		m_use_return = true;
}

//-----------------------------------------------------------------------------
//	Name : CheckEquipSkill
//	Desc :
//-----------------------------------------------------------------------------
// rv
// 0 : 그냥 사용불가.
// 1 : 사용가능
// -1 : 장착부분
bool CAutoPlay_EX::CheckEquipSkill(int cur_skill)
{
	int li_selskill = 0 ;
	int req_STM = 0;
	int req_MNA = 0;
	int req_ELE = 0;
	int req_VAT = 0;
	int temp_point = 0;

	li_selskill = g_ChaSkill.m_SkillIndex[cur_skill];

	if(pCMyApp->m_pUIMgr->m_bChakram)
	{
		if(!g_pRoh->m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum])
			return false;
		return true;
	}

	req_STM = g_Skills[li_selskill].GetEnerge0(g_ChaSkill.m_SkillLevel[cur_skill]) ;
	req_ELE = g_Skills[li_selskill].GetEnerge0(g_ChaSkill.m_SkillLevel[cur_skill]) ;
	req_MNA = g_Skills[li_selskill].GetEnerge0(g_ChaSkill.m_SkillLevel[cur_skill]) ;

	switch(li_selskill)
	{
	case SKILL_THROW:
		if( !(g_pRoh->m_Wearing[WEARING_CLOAK]) )
			return false;
		if( !(GET_ATT_FLAG(g_pRoh->m_Wearing[WEARING_CLOAK]) & IATT_SUB_WEAPON) )
			return false;
		if(g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special ==0
				|| g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special ==1 )
			return false;

		return true;

	case SKILL_DROP_ATTACK:   // Drop Attack
	case SKILL_ROTATE_ATTACK: // Rotate Attack
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SWORD
				&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_AXE)
			return false;
		return true;

	case SKILL_FORCE_ATTACK : // Force Attack
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SWORD
				&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_AXE)
			return false;
		return true;

	case SKILL_HELL_CRY     : // Hell Cry
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SWORD
				&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_AXE)
			return false;
		return true;

	case SKILL_HELL_TOUCH :    // SKILL_TORNADO_RUIN
	case SKILL_VANPIRE_TOUCH : // SKILL_VANPIRE_TOUCH
		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_Vnum != 1021)
			return false;
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0)
			return false;
		return true;

	case SKILL_MAGIC_MOVE :
		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		if(!(GET_SHAPE(g_pRoh->m_Wearing[WEARING_SHIELD]) & ISUB_WAND))
			return false;
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_Vnum == 1021)
			return false; // 피묻은 장갑 제외
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0)
			return false;
		return true;

	case SKILLF_E_REFLECTION :
		if( !g_pRoh->m_Wearing[WEARING_WEAPON]
				|| !( GET_SHAPE( g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER))
			return false;
		if( g_pRoh->m_ShowReflection || g_pRoh->m_bShowAdrenaline )
			return false;
		return true;

	case SKILL_C_HELLDOWN :
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_STAFF)
			return false;
		return true;

	case SKILL_HANDLE :
		if(!g_pRoh->m_Wearing[WEARING_CLOAK])
			return false;
		if(!(GET_ATT_FLAG(g_pRoh->m_Wearing[WEARING_CLOAK]) & IATT_SUB_WEAPON))
			return false;
		if(g_pRoh->m_Wearing[WEARING_CLOAK]->m_PlusNum==0)
			return false;
		return true;

	case SKILL_SNIFING :
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_GUN)
			return false;
		return true;

	case SKILL_DEEP_ATTACK  : // 딥 어택
	case SKILL_CHAIN_ATTACK : // 체인 어택
		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_Vnum != 1023)
			return false;
		return true;

	case SKILL_MELODY_OF_DEF:
	case SKILL_MELODY_OF_GUD:
		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		if(!(GET_ATT_FLAG(g_pRoh->m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON))
			return false;
		if(pCMyApp->IsAcropolis() == 1 || g_pRoh->m_Mana < req_MNA)
			return false;
		if( g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum == 0 )
			return false;

		return true;

	case SKILLF_A_DSTONE: // 디바인 스톤
		if(!(g_pRoh->m_Wearing[WEARING_CLOAK]))
			return false;
		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_CLOAK]) == ISUB_DSTONE))
			return false;
		return true;

	case SKILLF_A_DSLASH: // 더블 슬래쉬
		if(!(g_pRoh->m_Wearing[WEARING_WEAPON]))
			return false;
		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD))
			return false;
		return true;

	case SKILLF_A_BASH: // 배쉬
		if(!(g_pRoh->m_Wearing[WEARING_WEAPON]))
			return false;
		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER))
			return false;
		return true;

	case SKILLF_A_ASSAULT: // 어썰트
		if(!(g_pRoh->m_Wearing[WEARING_WEAPON]))
			return false;
		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD))
			return false;
		return true;

	case SKILL_B_BURNINGCRASH:
		if( !g_pRoh->m_Wearing[ WEARING_CLOAK ] )
			return false;
		if( GET_SHAPE( g_pRoh->m_Wearing[ WEARING_CLOAK ] ) != ISUB_SLAYER )
			return false;
		return true;

	case SKILL_A_LIFE_EXPANSION:
		if( !g_pRoh->m_Wearing[ WEARING_SHIELD ] )
			return false;
		if( GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 1022 		// 1022 : 마술피리
				&& GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 3067 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 10206 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 10417 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 11065
				&& GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 11086 )	// 1022 : 마술피리 //Update Elite
			return false;
		return true;

	case SKILL_F_SHADOWSLASH://mungmae: 2005/11/18 새도우 슬래쉬
		if( !g_pRoh->m_Wearing[WEARING_WEAPON]
				|| !( GET_SHAPE( g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD))
			return false;
		return true;

	case SKILL_F_ADRENALINE ://mungmae-2005/11/21 - 아드레날린
		if( !g_pRoh->m_Wearing[WEARING_WEAPON] || !( GET_SHAPE( g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER))
			return false;
		return true;

	case SKILL_F_SPACESLASH:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_DUAL_SWORD )
			return false;
		return true;

	case SKILL_F_ELECTRICHAMMER:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_HAMMER )
			return false;
		return true;

	case SKILL_B_PROTECTION_SHIELDS :
		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		if( GET_SHAPE( g_pRoh->m_Wearing[WEARING_SHIELD]) != IARMOR_SHIELD )
			return false;
		return true;

	case SKILL_H_RANDOMFIRE:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_GUN)
			return false;
		return true;

	case SKILL_K_ICEBOLT:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_STAFF)
			return false;
		return true;
	case SKILL_F_MERCERNERY_SLASH:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_DUAL_SWORD )
			return false;
		return true;

	case SKILL_H_FROZENSHOT:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_GUN)
			return false;
		return true;

	case SKILL_F_TORNADO_HAMMER:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_HAMMER )
			return false;
		return true;

	case PEROM_SKILL_S139:
		if(!g_pRoh->m_Wearing[WEARING_CLOAK])
			return false;
		if( GET_SHAPE( g_pRoh->m_Wearing[ WEARING_CLOAK ] ) != ISUB_PANEL )
			return false;
		if( g_pRoh->m_Wearing[ WEARING_CLOAK ]->m_Special <= 5 )
			return false;
		return true;
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name : _StartCheck
//	Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_StartCheck()
{
	int pair_slot[AH_QUICKSLOT_COUNT];
	ZeroMemory(pair_slot , sizeof(int)*AH_QUICKSLOT_COUNT );
	int pair_key[FUNCTION_NUM];
	ZeroMemory(pair_key,sizeof(int)*FUNCTION_NUM);
	int temp_skill[FUNCTION_NUM];
	ZeroMemory(temp_skill,sizeof(int)*FUNCTION_NUM);


	if( m_bHealth )
		pair_slot[m_dwHealthKey]++;
	if( m_bMana )
		pair_slot[m_dwManaKey]++;
	if( m_bStamina )
		pair_slot[m_dwStaminaKey]++;
	if( m_bEPower )
		pair_slot[m_dwEPowerKey]++;
	int i = 0;
	for( i = 0; i < AH_SPECIALPOTION; i++ )
	{
		if( m_bSpecialPotion[i] )
			pair_slot[m_dwSpecialPotionKey[i]]++;
	}
//	if( m_bSpecialPotion[0] )
//		pair_slot[m_dwSpecialPotionKey[0]]++;
//	if( m_bSpecialPotion[1] )
//		pair_slot[m_dwSpecialPotionKey[1]]++;

	if( m_dwGameMode != GAMEMODE_POTION_ONLY )
	{
		if( m_dwGameMode == GAMEMODE_FULL_PACKAGE )
		{
			if( m_bUsedCallSign )
				pair_slot[m_uCallKeyMapping]++;
			if( m_wReturn )
				pair_slot[m_dwReturnKey]++;
			if( m_bUseTrans )
				pair_slot[m_dwTransportingKey]++;
		}

		if( m_bSubArms )
			pair_slot[m_dwSubArmsKey]++;
		if( m_bAdiaSummon )
			pair_slot[m_dwAdiaSummonKey]++;

		for( i = 0; i < FUNCTION_NUM ; i ++ )
		{
			if( m_bSkillArray[i] )
			{
				int down_key = m_dwSkillKeyArray[i]; //>0?m_dwSkillKeyArray[i]-1:7;
				pair_key[down_key]++;
			}
		}
		if( m_bShield )
			pair_key[m_dwShieldKey]++;
		if( m_bRecovery )
			pair_key[m_dwRecoveryKey]++;
		if( m_bBloodyMana )
			pair_key[m_dwBloodyManaKey]++;
		if( m_bVempTouch )
			pair_key[m_dwVempTouchKey]++;

		if( m_use_dsys)
			pair_key[m_dsys_key]++;
		if( m_bshield2 )
			pair_key[m_dwShieldKey2]++;
		if( m_bshield3 )
			pair_key[m_dwShieldKey3]++;
		if( m_bshield4 )
			pair_key[m_dwShieldKey4]++;
	}

	char err_buf[256];
	for( i=0 ; i<AH_QUICKSLOT_COUNT ; i++ )
	{
		if( pair_slot[i] > 2 )
		{
			sprintf(err_buf , "Key slot %d is duplicated." , i+1);
			_AddChat( err_buf );
			return false;
		}
	}


	//-------------------------------------------------------------------------
	// 스킬지정키를 어떻게 해야할까요?
	//-------------------------------------------------------------------------
	for( int i = 0; i < FUNCTION_NUM ; i ++ )
	{
		if( m_bSkillArray[i] && pair_key[i] >= 1 )
		{
			int down_key = m_dwSkillKeyArray[i]>0?m_dwSkillKeyArray[i]-1:7;
			int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
			if( cur_skill < 0 )
			{
				_AddChat( "Skill is not assigned." );
				return false;
			}
		}
	}

	if( m_bLeftSkill )
	{
		int down_key = m_dwLeftSkill>0?m_dwLeftSkill-1:7;
		int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;

		if( cur_skill < 0 )
		{
			_AddChat( "Skill is not assigned." );
			return false;
		}

		int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

		// 오류예상지점 2010.03.24.
		if( g_Skills[skill_index].m_bLeft )
			g_pNk2DFrame->SetLSkill(cur_skill);
		else
		{
			// 잘못 지정되어 있음
		}
	}

	// if shield
	if( m_bShield && pair_key[m_dwShieldKey] > 1 )
	{
		_AddChat( "Shield skill assignment keys are duplicated." );
		return false;
	}
	if( m_use_dsys && pair_key[m_dsys_key] > 1 )
	{
		_AddChat( "Shield skill assignment keys are duplicated." );
		return false;
	}
	if( m_bshield2 && pair_key[m_dwShieldKey2] > 1 )
	{
		_AddChat( "D-System Field key is duplicated." );
		return false;
	}
	if( m_bshield3 && pair_key[m_dwShieldKey3] > 1 )
	{
		_AddChat( "Duplicate Engage Key." );
		return false;
	}
	if( m_bshield4 && pair_key[m_dwShieldKey4] > 1 )
	{
		_AddChat( "Critical Strike Skill Keys are duplicated." );
		return false;
	}

	if( m_bRecovery && pair_key[m_dwRecoveryKey] > 1 )
	{
		_AddChat( "There are duplicate recovery skill assignment keys." );
		return false;
	}
	if( m_bBloodyMana && pair_key[m_dwBloodyManaKey] > 1 )
	{
		_AddChat( "Bloody mana skill assignment keys are duplicated." );
		return false;
	}
	if( m_bVempTouch && pair_key[m_dwVempTouchKey] > 1 )
	{
		_AddChat( "Vampire Touch Skill Keys are duplicated." );
		return false;
	}

	if( m_bPetActive )
	{
		if( g_MyPetStatus.mode < MYPET_MODE_KEEP )
		{
		_AddChat ("There is no charge");
		_AddChat ("I will stop using the cost");
		_AddChat ("Stop buying apples.");
		_AddChat ("Stop pager purchasing");
		_AddChat ("Stop using pager.");

			m_bPetActive	= false;
			m_bBuyApple		= false;
			m_bBuyCallSign	= false;
			m_bUsedCallSign = false;
			m_wReturn &= (~CALC_BIT( 1, AH_RETURN_APPLE ));

			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name : _GetQuickSlotKey
//	Desc :
//-----------------------------------------------------------------------------
int CAutoPlay_EX::_GetQuickSlotKey(int type)
{
	switch( type )
	{
	case AH_RETURN_HEALTH:
		return m_dwHealthKey;
	case AH_RETURN_MANA:
		return m_dwManaKey;
	case AH_RETURN_STAMINA:
		return m_dwStaminaKey;
	case AH_RETURN_EPOWER:
		return m_dwEPowerKey;
	case AH_RETURN_SUBARMS:
		return m_dwSubArmsKey;
	case AH_RETURN_SUMMONS:
		return m_dwAdiaSummonKey;
	}

	return -1;
}

//-----------------------------------------------------------------------------
//	Name : _OnMoveToBase
//	Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnMoveToBase(DWORD dt)
{
	if( _CheckElapsedTime( dt , m_move_to_base_tick , FALIED_RETURN_BASE_TIME ) )
	{
		return true;
	}

	static float prev_dist = 0;
	static int try_count = 0;
	float dist = DistPlToPl( m_fStayPosX, m_fStayPosZ, g_pRoh->m_wx, g_pRoh->m_wz );

	if( _IsDungeon() )
	{
		dist *= 0.5f;
	}
	if( dist > 1.0f )
	{
		if( !_OnMove(dt, m_fStayPosX, m_fStayPosZ ) )
		{
			if( prev_dist != 0 && prev_dist == dist )
				try_count++;

			prev_dist = dist;

			if( try_count < 3 )
				return false;

			try_count = 0;

			int mx = 0;
			int my = 0;

			float temp_dist = 100.0f;

			switch( dt % 8 )
			{
			case 0:
				mx += temp_dist;
				break;
			case 1:
				mx -= temp_dist;
				break;
			case 2:
				my += temp_dist;
				break;
			case 3:
				my -= temp_dist;
			case 4:
				mx += temp_dist;
				my += temp_dist;
				break;
			case 5:
				mx -= temp_dist;
				my -= temp_dist;
				break;
			case 6:
				mx += temp_dist;
				my -= temp_dist;
				break;
			case 7:
				mx -= temp_dist;
				my += temp_dist;
				break;
			}

			_OnMove(dt , g_pRoh->m_wx + mx, g_pRoh->m_wz + my );
		}
	}
	else
	{
		m_move_to_base_tick = 0;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//	Name : _AddChat
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::_AddChat( char * msg )
{
	if( !msg || !msg[0] )
		return;

	char buf[256] = {0,};
	sprintf( buf , "[BOT-NEW] %s" , msg );
	g_pNk2DFrame->AddChatStr( buf , -2 );
}

//-----------------------------------------------------------------------------
//	Name : _AddChat_Debug
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::_AddChat_Debug( char * msg )
{
}


//-----------------------------------------------------------------------------
//	Name : _OnInterrupt
//	Desc :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnInterrupt(LagRush_Interrupt interrupted)
{
	bool rv = true;
	char msg_buf[256] = {0,};

	switch( interrupted )
	{
	case LagRush_Interrupt__Error_Setting:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1795) );
		break;
	case LagRush_Interrupt__WalkPos_Empty:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1796) );
		break;
	case LagRush_Interrupt__NPC_NotFound:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1797) );
		break;
	case LagRush_Interrupt__Machine_Error:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1798) );
		break;
	case LagRush_Interrupt__Slot_NotMatch:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1799) );
		break;
	case LagRush_Interrupt__Inven_Full:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1800) );
		break;
	case LagRush_Interrupt__BuyItemFail:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1801) );
		break;

	case LagRush_Interrupt__Disconnect_Server:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1802) );
		break;

	case LagRush_Interrupt__Limited_Dungeon:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1803) );
		break;

	case LagRush_Interrupt__NotSetting:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1804) );
		break;
	case LagRush_Interrupt__Version_Low:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1805) );
		break;
	case LagRush_Interrupt__Tribe_NotMatch:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1806) );
		break;


// 	case LagRush_Interrupt__Error_Stash_Put:
// 		sprintf( msg_buf , "창고이용에 오류가 있습니다." );
// 		break;
// 	case LagRush_Interrupt__Error_Stash_Get:
// 		sprintf( msg_buf , "창고이용에 오류가 있습니다." );
// 		break;
// 	case LagRush_Interrupt__Error_Shop_Buy:
// 		sprintf( msg_buf , "상점이용에 중 오류가 있습니다." );
// 		break;

	case LagRush_Interrupt__Error_NoMsg:
		break;

	//MJ_20100325
	case LagRush_Interrupt__Wrong_Zone:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1807) );
		break;
	case LagRush_Interrupt__Wrong_OpenStash:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1808) );
		break;
	case LagRush_Interrupt__Wrong_Stash:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1809) );
		break;
	case LagRush_Interrupt__NonExist_Item:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1810) );
		break;
	case LagRush_Interrupt__NoValid_Inven:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1811) );
		break;
	case LagRush_Interrupt__NoValid_Stash:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1812) );
		break;
	case LagRush_Interrupt__Fail_FindItem:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1813) );
		break;
	case LagRush_Interrupt__OptionTime:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1814) );
		break;
	case LagRush_Interrupt__NoSame_Item:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1815) );
		break;
	case LagRush_Interrupt__Fail_Input:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1816) );
		break;

// 	case LagRush_Interrupt__Maxrix_Leader:
// 	case LagRush_Interrupt__Maxrix_Cnt:
// 	case LagRush_Interrupt__Maxrix_OldItem_Destruction:
// 		sprintf( msg_buf , "" );
// 		break;

	case LagRush_Interrupt__InvenFull:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1817) );
		break;
	case LagRush_Interrupt__NoMoney:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1818) );
		break;
	case LagRush_Interrupt__NoBuyItem:
		sprintf( msg_buf , G_STRING(IDS_LHSTRING1819) );
		break;
	case LagRush_Interrupt__CantZone:
		sprintf( msg_buf, G_STRING(IDS_LAGRUSH_CANTZONE) );
		break;
	case LagRush_Interrupt__None:
	default:
		rv = false;
		break;
	}

	if( rv )
	{
		if( g_pNk2DFrame->IsWareHouseVisible() )
			g_pNk2DFrame->ToggleWareHouseWindow();
		if( g_pNk2DFrame->IsShopVisible() )
			g_pNk2DFrame->ToggleShopWindow();

		g_pNk2DFrame->DeleteAllPopup();

		if( msg_buf[0] != 0 )
		{
			g_pNk2DFrame->AddChatStr( msg_buf , -1 );
			g_pNk2DFrame->InsertPopup( msg_buf , TYPE_NOR_OK, 1 );
		}
		StopProc();
	}

	return rv;
}


//-----------------------------------------------------------------------------
//	Name : AddLog
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::AddLog(char * log , int phase_state)
{
}

//-----------------------------------------------------------------------------
//	Name : LogStart()
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::LogStart()
{
}


void CAutoPlay_EX::Lagrush_Profile()
{
}

//-----------------------------------------------------------------------------
//	Name : LogEnd()
//	Desc :
//-----------------------------------------------------------------------------
void CAutoPlay_EX::LogEnd(bool force)
{
}

void CAutoPlay_EX::SetSurport(int source, int target)
{
	if( !m_using_surport )
		return;

	if( !g_pNk2DFrame->GetControlParty()->IsMyParty(source) )
		return;

	if( m_pSelectedMob && m_pSelectedMob->m_nVirId == target )
	{
		m_need_surport = target;
		return;
	}
	else
	{
		m_need_surport = target;
		m_need_change = true;
	}
}

bool CAutoPlay_EX::_OnDecideTarget_Surport(DWORD dt)
{
	if( !m_using_surport )
		return false;

	if( !g_pNk2DFrame->GetControlParty() )
	{
//		m_using_surport = false;
		return false;
	}

	if( !g_pNk2DFrame->GetControlParty()->m_MemberCount )
	{
//		m_using_surport = false;
		return false;
	}

	if( m_need_surport == -1 )
		return false;

	if( m_pSelectedMob && m_pSelectedMob->m_nVirId == m_need_surport )
		return true;


	CNkMob * mob = pCMyApp->m_pUIMgr->GetMobByID( m_need_surport );
	if( !mob )
		return false;
	if( mob && mob->IsDead() )
		return false;

	m_pSelectedMob = mob;
	pCMyApp->StopHero();
	pCMyApp->m_pUIMgr->SetPickColor(m_pSelectedMob->m_wColorIdx);

	return false;
}

void CAutoPlay_EX::Render(bool activate, bool edit_mode)
{
	if( !IsRun() )
		return;

	if( !GET_D3DDEVICE() )
		return;

	if( _IsPeaceArea() )
	{
	}
	else
	{
		if( _IsDungeon() || m_dwGameMode != GAMEMODE_FULL_PACKAGE )
			return;

		if( m_current_state == GO_TO_AREA_ON_WALK )
			return;

		if( m_pDecal )
			m_pDecal->Render(0);

// 		D3DMATRIX matWorld;
// 		LPDIRECTDRAWSURFACE7 lpTexture = NULL;
//
// 		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_MASICCIRCLE]);
//
// 		D3DUtil_SetIdentityMatrix(matWorld);
//
// 		matWorld._41 = m_fStayPosX;
// 		matWorld._42 = g_pRoh->m_wy+0.2f;
// 		matWorld._43 = m_fStayPosZ;
//
// 		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
//
//
// 		DWORD dwTextureState[3];
//
// 		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
// 		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
// 		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );
//
// 		// D3DFVF_VERTEX
// 		D3DVERTEX test[4];
//
// 		test[0] = D3DVERTEX( D3DVECTOR(-m_fStayRange,0,-m_fStayRange), D3DVECTOR(0, 0, -1), 0.0f, 0.0f );
// 		test[1] = D3DVERTEX( D3DVECTOR(-m_fStayRange,0,m_fStayRange), D3DVECTOR(0, 0, -1), 0.0f, 1.0f );
// 		test[2] = D3DVERTEX( D3DVECTOR(m_fStayRange,0,-m_fStayRange), D3DVECTOR(0, 0, -1), 1.0f, 0.0f );
// 		test[3] = D3DVERTEX( D3DVECTOR(m_fStayRange,0,m_fStayRange), D3DVECTOR(0, 0, -1), 1.0f, 1.0f );
//
// 		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
// 		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
// 		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );
//
// 		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
//
// 		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
// 		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
// 		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
//
// 		GET_D3DDEVICE()->SetTexture(0, lpTexture);
// 		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_VERTEX , &test , 4 , 0 );
//
// 		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
// 		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
//
// 		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
// 		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
// 		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	}
}

void CAutoPlay_EX::KillMob(int mob_index)
{
	// 무조건... ++
	m_kill_count++;
}

bool CAutoPlay_EX::_IsDungeon()
{
	if( pCMyApp->GetCurWorld() == 1 ||
			pCMyApp->GetCurWorld() == 2 ||
			pCMyApp->GetCurWorld() == 5 )
		return true;
	return false;
}

bool CAutoPlay_EX::_IsCantZone()
{
	if( pCMyApp->GetCurWorld() == 26 ) //TRITIAN CANT USE MACRO BOT
		return true;
	else if( pCMyApp->GetCurWorld() == 29 ) //TRITIAN CANT USE MACRO BOT
		return true;

	return false;
}

void CAutoPlay_EX::ResetAll()
{
	m_dwGameStatus = NONE_STATE;
	m_pSelectedMob = NULL;
	m_pTemporaryMob = NULL;
	m_pTemporaryItem = NULL;

	m_szWaringMobList.clear();
	m_szEscapeList.clear();
	m_szItemList.clear();
	m_szAvoidUserList.clear();
	m_szAvoidMobList.clear();
	m_vecPickItemList.clear();
	m_vecFirstTarget.clear();
	m_vecAvoidMob.clear();
	m_vecAvoidChar.clear();
	m_pairPoslist.clear();
	m_pairPoslist_temp.clear();
}

void CAutoPlay_EX::SetPos(float x, float z)
{
	if( false == _IsPeaceArea() )
	{
		m_devine_stone = true;
	}
	else
	{
		m_devine_stone = false;
	}

	m_fStayPosX = x;
	m_fStayPosZ = z;

	if( m_pDecal )
		m_pDecal->ReInit(m_fStayPosX,m_fStayPosZ,m_fStayRange,10);
	else
		m_pDecal = new LH_Decal(m_fStayPosX,m_fStayPosZ,m_fStayRange,10);
}

void CAutoPlay_EX::Init_variable()
{
	CAutoPlay::Init_variable();

	m_change_state		= false;
	m_prev_state		= NONE_STATE;
	m_current_state		= NONE_STATE;

	m_pick_item_time	= 0;
	m_last_quick_tick	= 0;
	m_last_action_tick 	= 0;
	m_last_attack_tick 	= 0;
	m_last_callsign_tick = 0;

	if( g_SvrType == ST_ADULT_ONLY )
		m_shop_state = ShopState__Repair;
	else
		m_shop_state = ShopState__Buy;
	m_skill_slot		= 0;

	m_used_limit_delay	= 0;
	m_used_skill_delay	= 0;

	m_move_to_base_tick = 0;

	m_pPrevSelectedMob = NULL;
	m_move_to_base_dir = 1;
	m_dead_time = 0;

	m_stash_state = StashState__Push;
	int i = 0;
	for( i = 0; i < AH_SPECIALPOTION; i++ )
	{
		m_dwSpecialPotionCurTime[i] = 0;
	}
//	m_dwSpecialPotionCurTime[0] = 0;
//	m_dwSpecialPotionCurTime[1] = 0;
	m_picked_apple = false;

	m_global_attack_time = 0;
	m_shop_sendmsg = 0;
	m_stash_sendmsg = 0;
	m_apple_sendmsg = 0;
	m_devine_stone = false;
}

void CAutoPlay_EX::SetLostQuickSlot()
{
	for( int i=0 ; i<QUICK_SLOT_NUM*QUICK_SLOT_PAGE ; i++ )
	{
		if( g_pRoh->m_QuickSlot[i] )
		{
			m_temp_quick[i].quick_slot_vnum = g_pRoh->m_QuickSlot[i]->m_Vnum;
			m_temp_quick[i].tick = 0;
		}
		else
		{
			m_temp_quick[i].quick_slot_vnum = -1;
			m_temp_quick[i].tick = 0;
		}
	}
}

// 1 : 이동성공
// 0 : 이동필요없음
// -1 : 이동할아이템 없음
int CAutoPlay_EX::IsLostQuickSlot(DWORD dt)
{
	for( int i=0 ; i<QUICK_SLOT_NUM*QUICK_SLOT_PAGE ; i++ )
	{
		if( m_temp_quick[i].quick_slot_vnum != 0 && m_temp_quick[i].tick !=0 )
		{
			if( NULL == g_pRoh->m_QuickSlot[i] )
			{
				if( dt - m_temp_quick[i].tick > 1000 )
				{
					m_temp_quick[i].tick = 0;

					if( FillLostQuickSlot(i , m_temp_quick[i].quick_slot_vnum) )
					{
						return 1;
					}
					else
					{
						return -1;
					}
				}
			}
			else
			{
				m_temp_quick[i].tick = 0;
			}
		}
	}

	return 0;
}

// 로스트된 아이템을 이동시켜주자
bool CAutoPlay_EX::FillLostQuickSlot(int slot , int vnum)
{
	CItem* pItem = NULL;

	for( int p=0 ; p<PACK_NUM ; p++ )
	{
		pItem = g_pRoh->GetInvenItem( p , vnum , true);
		if( pItem )
		{
			pCMyApp->m_pUIMgr->Send_MoveToItem(slot , p , pItem->m_Index);
			return true;
		}
	}

	return false;
}

void CAutoPlay_EX::SetSendMove(int slot)
{
	if( m_temp_quick[slot].quick_slot_vnum != 0 && m_temp_quick[slot].tick == 0 )
		m_temp_quick[slot].tick = GetTickCount();
}



bool CAutoPlay_EX::_OnBuffTimer(DWORD dt)
{
	for( int i=0 ; i<8 ; i++ )
	{
		if( m_bSkillArray[i] )
		{
			int down_key = m_dwSkillKeyArray[i]>0?m_dwSkillKeyArray[i]-1:7;
			int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
			int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

			if( cur_skill < 0 )
				continue;

			if( g_Skills[skill_index].m_Kind == SKILL_KIND_ME )
			{

				if( g_Skills[skill_index].m_bAttack )
				{
					if( skill_index != 48 && skill_index != 88 && skill_index != 119
							&& skill_index != PEROM_SKILL_S134 && skill_index != PEROM_SKILL_S139
							&& skill_index != PEROM_SKILL_S148 && skill_index != PEROM_SKILL_S149
							&& skill_index != PEROM_SKILL_S150 && skill_index != PEROM_SKILL_S141
					  )
						continue;
				}

				if( skill_index == SKILLF_A_DSTONE || skill_index == SKILLF_A_TIMECHARGE )
					continue;

				if( m_dwSkillCurTime[i] && dt - m_dwSkillCurTime[i] < m_wSkillLimitTime[i] )
					continue;

				g_pNk2DFrame->GetControlBottom()->m_DownKey = down_key;
				g_pNk2DFrame->GetControlBottom()->ActionQuickKey( down_key );
				m_dwSkillCurTime[i] = GetTickCount();


				return true;
			}
			else if( g_Skills[skill_index].m_Kind == 4 ) // 캐릭터 타겟 필요
			{
				if( m_dwSkillCurTime[i] && dt - m_dwSkillCurTime[i] < m_wSkillLimitTime[i] )
					continue;
				
				SendCharUseSkill_Target(skill_index, eCharType_Char, g_pRoh->m_nCharIndex);
				m_dwSkillCurTime[i] = GetTickCount();

				return true;
			}
		}
	}

	return false;
}

bool CAutoPlay_EX::_CommonSkill(DWORD dt)
{
	if( _OnBuffTimer(dt) )
		return true;

	if( _OnSubArms(dt) )
	{
		AddLog( "[_OnFieldProc:Attacking: Successful Auxiliary Filler Handling]" , 1 );
		return true;
	}
	if( _OnAdiaSummon(dt) )
	{
		AddLog( "[_OnFieldProc:Attacking: Successfully summoning Aidia]" , 1 );
		return true;
	}
	if( _OnShield(dt) )
	{
		AddLog( "[_OnFieldProc:Attacking: Shielding successful]" , 1 );
		return true;
	}
	if( _OnDsystem(dt) )
	{
		return true;
	}
	if( _OnShield2(dt) )
	{
		return true;
	}
	if( _OnShield3(dt) )
	{
		return true;
	}
	if( _OnShield4(dt) )
	{
		return true;
	}
	if( _OnRecovery(dt) )
	{
		return true;
	}
	if( _OnBloody(dt) )
	{
		return true;
	}

	return false;
}


bool CAutoPlay_EX::_IsStashPush(CItem* pItem)
{
	if( !pItem )
		return false;

	if( GET_ATT_FLAG( pItem ) & IATT_NO_STASH )
		return false;
	if( pItem->m_bTimeLimit )
		return false;

	CControlWareHouse * ware_house = g_pNk2DFrame->GetControlWareHouse();

	if( !ware_house )
		return false;

	if( -1 == ware_house->Push_Available(pItem) )
		return false;

	return true;
}



bool CAutoPlay_EX::_DontUsingFullAuto()
{
	if( pCMyApp->GetCurWorld() == 10 ||
			pCMyApp->GetCurWorld() == 11 ||
			pCMyApp->GetCurWorld() == 12 ||
			pCMyApp->GetCurWorld() == 18 /*||
			pCMyApp->GetCurWorld() == 19*/ )
		return true;

	if( pCMyApp->IsBossRoomArea(0,0) )
		return true;
	
	return false;
}

