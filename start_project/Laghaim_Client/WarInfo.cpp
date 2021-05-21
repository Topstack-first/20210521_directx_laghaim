#include "stdafx.h"
// WarInfo.cpp: implementation of the CWarInfo class.
//
//////////////////////////////////////////////////////////////////////
#define D3D_OVERLOADS
#include <math.h>
#include <d3d.h>
#include "Config.h"
#include "d3dutil.h"
#include "IndexedTexture.h"
#include "LoadEffectSound.h"
#include "main.h"
#include "Land.h"
#include "WarInfo.h"
#include "g_stringmanager.h"
#include "UIMgr.h"
#include "Nk2DFrame.h"

#define WARINFO_X		230
#define WARINFO_X2		100
#define WARINFO_Y		110
#define WARINFO_GAP		15

#define WARINFO_CUR_GUILD		0
#define WARINFO_CUR_TIME		1
#define WARINFO_CUR_PLAYER		2
#define WARINFO_CUR_POSITION	3
#define WARINFO_FIRST_GUILD		4
#define WARINFO_FIRST_TIME		5
#define WARINFO_MYGUILD_POINT	6
#define WARINFO_REMAIN_TIME		7

#define MATSCOREBASEWIDTH	150
//#define MATSCOREBASE_Y	30
#define MATSCOREBASE_Y	120
#define MATSCOREGAP			32
//#define MATSCORE_Y		40
#define MATSCORE_Y		130
#define	MATSCORE_X		90






extern char g_strFontName[];

extern int		gMapWidth;							// The x-axis width of the map
extern int		gMapHeight;
extern int		gMapExtention;					// Scale Factor - Integer Type

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarInfo::CWarInfo()
{
	m_pWarInfoBase = NULL;
	m_pWarInfoBase2 = NULL;
	for(int i=0; i<4; i++)
		m_pWarScoreBase[i]= NULL;

	m_ScreenWidth = 800;
	m_ScreenHeight = 600;
	m_bTeamLose[0]=FALSE;
	m_bTeamLose[1]=FALSE;
	m_bTeamLose[2]=FALSE;

}

CWarInfo::~CWarInfo()
{
	DeleteRes();
}

void CWarInfo::FontInit()
{
	if( pCMyApp->m_nCurWorldIdx == 10	// 10 : WORLD_MATRIX,
			|| pCMyApp->m_nCurWorldIdx == 19 // 19 : WORLD_MATRIX_2009
	  )
	{
		for( int i = 0 ; i < 4 ; i ++ )
		{
			m_WarInfoBox[ i ].Init( 12, RGB( 255, 255, 255 ), 0, 0 );
		}
		//BattlePoint Yellow
		m_WarInfoBox[ 4 ].Init( 12, RGB( 231, 203, 102 ), 0, 0 );
	}
	else
	{
		for (int i = 0; i < WARINFO_NUM + WARINFO2_NUM; i++)
		{
			m_WarInfoBox[i].Init(12, RGB(255, 255, 255), 0, 0); //14font, white color
		}
	}
}



//----------------------------------------------------------------------
//		LOAD RESOURCE
//----------------------------------------------------------------------
void CWarInfo::LoadRes(int screenWidth, int screenHeight)
{
	DeleteRes();

	if(!g_pDisplay)
		return;
	if( pCMyApp->m_nCurWorldIdx == 10		// 10 : WORLD_MATRIX
			|| pCMyApp->m_nCurWorldIdx == 19	// 19 : WORLD_MATRIX_2009
	  )
	{
		char strTexName[100] = {0,};
		int i=0;
		for(i = 0 ; i < 5 ; i ++ )
		{
			m_WarInfoBox[ i ].SetString( "0" );
			m_WarInfoBox[ i ].Restore();
		}

		g_pDisplay->CreateSurfaceFromBitmap( &m_pWarInfoBase, "interface/warinfo/mat_warinfo.bmp" );
		m_pWarInfoBase->SetColorKey( RGB( 0, 0, 0 ) );
		for(i=0; i<4; i++)
		{
			sprintf(strTexName,"interface/warinfo/mat_warinfo%d.bmp", i);
			g_pDisplay->CreateSurfaceFromBitmap(&m_pWarScoreBase[i], strTexName);
			m_pWarScoreBase[i]->SetColorKey(RGB(0, 0, 0));
		}
	}
	else
	{
		for (int i = 0; i < WARINFO_NUM + WARINFO2_NUM; i++)
			m_WarInfoBox[i].Restore();

		g_pDisplay->CreateSurfaceFromBitmap(&m_pWarInfoBase, "interface/warinfo/warinfo.bmp");
		m_pWarInfoBase->SetColorKey(RGB(0, 0, 0));
		g_pDisplay->CreateSurfaceFromBitmap(&m_pWarInfoBase2, "interface/warinfo/warinfo2.bmp");
		m_pWarInfoBase2->SetColorKey(RGB(0, 0, 0));
	}
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
}



//----------------------------------------------------------------------
//		DELETE
//----------------------------------------------------------------------
void CWarInfo::DeleteRes()
{
	int i = 0;
	for(i=0; i<4; i++)
		SAFE_DELETE(m_pWarScoreBase[i])

		for (i = 0; i < WARINFO_NUM + WARINFO2_NUM; i++)
			m_WarInfoBox[i].DeleteRes();

	SAFE_DELETE(m_pWarInfoBase);
	SAFE_DELETE(m_pWarInfoBase2);

}

void CWarInfo::Draw()
{
	int warinfo_x = m_ScreenWidth - WARINFO_X;
	int warinfo_x2 = m_ScreenWidth - WARINFO_X + WARINFO_X2;
	int warinfo_y = WARINFO_Y + 5;
	int matScore_y = MATSCORE_Y;
	int i = 0;

	int iClintWidth = g_pNk2DFrame->GetClientWidth();

	// nate 2005-03-11
	// Matrix
	// 메트릭스 서버 배틀 정보 출력
	if( pCMyApp->m_nCurWorldIdx == 10		// 10 : WORLD_MATRIX
			|| pCMyApp->m_nCurWorldIdx == 19	// 19 : WORLD_MATRIX_2009
	  )
	{
		if( g_pDisplay && m_pWarInfoBase && m_pWarScoreBase)
		{
			if( m_pWarInfoBase->GetDDrawSurface()->IsLost() )
			{
				delete m_pWarInfoBase;
				m_pWarInfoBase = NULL;
				return;
			}

			//g_pDisplay->Blt( MATSCOREBASEWIDTH, 0, m_pWarInfoBase );
			//g_pDisplay->Blt( (iClintWidth/2) - (m_pWarInfoBase->GetWidth()/2), 0, m_pWarInfoBase );
			g_pDisplay->Blt( WARINFO_BASE_X, WARINFO_BASE_Y, m_pWarInfoBase );

			if( pCMyApp->m_nCurWorldIdx == 10 )
			{
				g_pDisplay->Blt( m_ScreenWidth-MATSCOREBASEWIDTH, MATSCOREBASE_Y, m_pWarScoreBase[0] );
			}
			else
			{
				RECT rc;
				rc.top		= 30;
				rc.bottom	= 93;
				rc.left		= 0;
				rc.right	= 146;

				g_pDisplay->Blt( m_ScreenWidth-MATSCOREBASEWIDTH, MATSCOREBASE_Y, m_pWarScoreBase[0], &rc );
			}
		}
		//m_WarInfoBox[ 0 ].Draw( (iClintWidth/2) - (m_pWarInfoBase->GetWidth()/2)+20, 45 ); // Time remaining
		//m_WarInfoBox[ 4 ].Draw( (iClintWidth/2) - (m_pWarInfoBase->GetWidth()/2)+25, 10 ); // Show personal score
		//m_WarInfoBox[ 0 ].Draw( MATSCOREBASEWIDTH+20, 45 ); // Time remaining
		//m_WarInfoBox[ 4 ].Draw( MATSCOREBASEWIDTH+25, 10 ); // Show personal score
		m_WarInfoBox[ 0 ].Draw( WARINFO_BASE_X+20, WARINFO_BASE_Y+45 ); // Time remaining
		m_WarInfoBox[ 4 ].Draw( WARINFO_BASE_X+25, WARINFO_BASE_Y+10 ); // Show personal score

		if( pCMyApp->m_nCurWorldIdx == 10 )
		{
			// The score of the alliance to which he belongs is displayed in yellow
			if(g_pRoh->m_GuildIndex > 0 && g_pRoh->m_GuildIndex < 4)
				m_WarInfoBox[g_pRoh->m_GuildIndex].m_FontColor= RGB( 231, 203, 102 );

			for( i = 1 ; i < 4 ; i ++ )
			{
				m_WarInfoBox[ i ].Draw( m_ScreenWidth - MATSCORE_X, matScore_y );
				matScore_y += MATSCOREGAP;
				if(m_bTeamLose[i-1])
					g_pDisplay->Blt( m_ScreenWidth-MATSCOREBASEWIDTH + 15, MATSCOREBASE_Y+ 2 + MATSCOREGAP*(i-1), m_pWarScoreBase[i] );
			}
		}
		else if( pCMyApp->m_nCurWorldIdx == 19 )
		{
			// The score of the alliance to which he belongs is displayed in yellow
			if(g_pRoh->m_GuildIndex > 0 && g_pRoh->m_GuildIndex < 3)
				m_WarInfoBox[ g_pRoh->m_GuildIndex ].m_FontColor= RGB( 231, 203, 102 );

			for( i = 1 ; i < 3 ; i ++ )
			{
				m_WarInfoBox[ i ].Draw( m_ScreenWidth - MATSCORE_X, matScore_y );
				matScore_y += MATSCOREGAP;
				if( m_bTeamLose[i-1] ||
						( pCMyApp->m_pUIMgr->m_nStoneState[i+3] == 2 && timeGetTime()%800 > 400 ))
					g_pDisplay->Blt( m_ScreenWidth-MATSCOREBASEWIDTH + 15, MATSCOREBASE_Y+ 4 + MATSCOREGAP*(i-1), m_pWarScoreBase[i+1] );
			}
		}
	}
	else
	{
		for (int i = 0; i < WARINFO_NUM; i++)
		{
			m_WarInfoBox[i].Draw(warinfo_x2 ,warinfo_y);
			warinfo_y += WARINFO_GAP;
		}
		if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN)
		{
			for (int i = WARINFO_NUM; i < WARINFO_NUM + WARINFO2_NUM; i++)
			{
				m_WarInfoBox[i].Draw(warinfo_x2 ,warinfo_y);
				warinfo_y += WARINFO_GAP;
			}
		}

		// 제목
		if(g_pDisplay && m_pWarInfoBase && m_pWarInfoBase2)
		{
			if (m_pWarInfoBase->GetDDrawSurface()->IsLost())
			{
				delete m_pWarInfoBase;
				m_pWarInfoBase = NULL;
				return;
			}
			if (m_pWarInfoBase2->GetDDrawSurface()->IsLost())
			{
				delete m_pWarInfoBase2;
				m_pWarInfoBase2 = NULL;
				return;
			}
			g_pDisplay->Blt(warinfo_x, WARINFO_Y ,m_pWarInfoBase);
			if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN)
				g_pDisplay->Blt(warinfo_x, WARINFO_Y+120 ,m_pWarInfoBase2);
		}
	}
}

// nate 2005-03-11
// Matrix
// 메트릭스 서버 배틀 포인트 저장
void CWarInfo::SetMatWarInfo( int nHawklawPoint, int nCoolrainPoint, int nShiberPoint,int nBattleTime )
{
	m_bTeamLose[0]=FALSE;
	m_bTeamLose[1]=FALSE;
	m_bTeamLose[2]=FALSE;
	char szInfoTemp[ 100 ];
	// IDS_WARINFO_MIN : "%d 분"
	sprintf( szInfoTemp, G_STRING(IDS_WARINFO_MIN), nBattleTime/60 );
	m_WarInfoBox[ 0 ].SetString( szInfoTemp );

	if(nHawklawPoint<0)
		m_bTeamLose[0] =TRUE;
	else
	{
		sprintf(szInfoTemp, G_STRING(IDS_WARINFO_POINT1), nHawklawPoint/100);//"%d 점"
		m_WarInfoBox[ 1 ].SetString( szInfoTemp );
	}

	if(nCoolrainPoint<0)
		m_bTeamLose[1] =TRUE;
	else
	{
		sprintf(szInfoTemp, G_STRING(IDS_WARINFO_POINT1), nCoolrainPoint/100);//"%d 점"
		m_WarInfoBox[ 2 ].SetString( szInfoTemp );
	}

	if(nShiberPoint<0)
		m_bTeamLose[2] =TRUE;
	else
	{
		sprintf(szInfoTemp, G_STRING(IDS_WARINFO_POINT1), nShiberPoint/100);//"%d 점"
		m_WarInfoBox[ 3 ].SetString( szInfoTemp );
	}
}


void CWarInfo::SetMatWar2009Info(int nGuild1, int nGuild2,int nBattleTime )
{
	m_bTeamLose[0]=FALSE;
	m_bTeamLose[1]=FALSE;
	m_bTeamLose[2]=FALSE;
	char szInfoTemp[ 100 ];
	// IDS_WARINFO_MIN : "%d 분"
	sprintf( szInfoTemp, G_STRING(IDS_WARINFO_MIN), nBattleTime/60 );
	m_WarInfoBox[ 0 ].SetString( szInfoTemp );

	if(nGuild1<0)
		m_bTeamLose[0] =TRUE;
	else
	{
		sprintf(szInfoTemp, G_STRING(IDS_WARINFO_POINT1), nGuild1/100);//"%d 점"
		m_WarInfoBox[ 1 ].SetString( szInfoTemp );
	}

	if(nGuild2<0)
		m_bTeamLose[2] =TRUE;
	else
	{
		sprintf(szInfoTemp, G_STRING(IDS_WARINFO_POINT1), nGuild2/100);//"%d 점"
		m_WarInfoBox[ 2 ].SetString( szInfoTemp );
	}
}

// nate 2005-03-11
// Matrix
// 메트릭스 서버에서 개인 배틀 포인트 셋팅
void CWarInfo::SetMatBattlePoint( int nBattlePoint )
{
	char szInfoTemp[ 50 ];
	char szInfoTemp1[ 50 ];
	int bp = nBattlePoint%100;
	if(bp<10)
		sprintf(szInfoTemp, "%d.0", nBattlePoint/100);
	else
		sprintf(szInfoTemp, "%d.", nBattlePoint/100);
	sprintf(szInfoTemp1, G_STRING(IDS_WARINFO_POINT1), nBattlePoint%100);//"%d 점"
	strcat(szInfoTemp,szInfoTemp1);
	m_WarInfoBox[4].SetString( szInfoTemp );

}

void CWarInfo::SetWarInfo(char *guild_name, char *owner_name, int nOwnerSec,
						  char *first_guild_name, int nFirstSec, float owner_x, float owner_z, int point)
{
	char info_temp[100];
	m_WarInfoBox[WARINFO_CUR_GUILD].SetString(guild_name);

	//-- IDS_WARINFO_POINT1 : %d 점
	sprintf(info_temp, G_STRING(IDS_WARINFO_POINT1), nOwnerSec);//"%d 점"
	m_WarInfoBox[WARINFO_CUR_TIME].SetString(info_temp);

	m_WarInfoBox[WARINFO_CUR_PLAYER].SetString(owner_name);

	int ownerPosX, ownerPosY;
	if( pCMyApp->GetCurWorld() == 1 || pCMyApp->GetCurWorld() == 2 || pCMyApp->GetCurWorld() == 5 )
	{
		ownerPosX = (owner_x/(gMapExtention/4));
		ownerPosY = (int)(gMapHeight*4-(owner_z/(gMapExtention/4)));
	}
//==============================================
	// nate 2004 - 4
	// CastleWar
	// 크리스탈 소유자 좌표 기존 버그 수정
	else
	{
		if( owner_x == 0 && owner_z == 0 )
		{
			ownerPosX = 0;
			ownerPosY = 0;
		}
		else
		{
			ownerPosX = (owner_x/(gMapExtention/5));
			ownerPosY = (int)(gMapHeight*5-(owner_z/(gMapExtention/5)));
		}
	}
//==============================================

//	sprintf(info_temp, "(%d, %d)", (int) (owner_x/gMapExtention), (int) (gMapHeight-owner_z/gMapExtention));
	sprintf(info_temp, "(%d, %d)", ownerPosX, ownerPosY);
	m_WarInfoBox[WARINFO_CUR_POSITION].SetString(info_temp);

	m_WarInfoBox[WARINFO_FIRST_GUILD].SetString(first_guild_name);

	//-- IDS_WARINFO_POINT2 : %d 점
	sprintf(info_temp, G_STRING(IDS_WARINFO_POINT2), nFirstSec);//"%d 점"
	m_WarInfoBox[WARINFO_FIRST_TIME].SetString(info_temp);

	//-- IDS_WARINFO_POINT3 : %d 점
	sprintf(info_temp, G_STRING(IDS_WARINFO_POINT3), point);//"%d 점"
	m_WarInfoBox[WARINFO_MYGUILD_POINT].SetString(info_temp);
}

void CWarInfo::SetWarInfo2(int flag_num, char *guild_name, char *owner_name)
{
	char info_temp[100];

	if (flag_num < 0 || flag_num >= WARINFO2_NUM)
		return;

	//sprintf(info_temp, "%s(%s)", owner_name, guild_name);
	sprintf(info_temp, "%s", guild_name);
	m_WarInfoBox[WARINFO_NUM+flag_num].SetString(info_temp);
}

void CWarInfo::SetBattleCount(int nSec)
{
	char info_temp[100];

	//-- IDS_WARINFO_SEC : %d 초
	sprintf(info_temp, G_STRING(IDS_WARINFO_SEC), nSec);//"%d초"
	m_WarInfoBox[WARINFO_REMAIN_TIME].SetString(info_temp);
}





CMatrixWarMessage::CMatrixWarMessage()
	: m_dwCurTime(0)
	, m_Second(0)
{
	m_OldCount = -1;
	m_NewCount = -1;
	m_dwBefTime = 0;
}

CMatrixWarMessage::~CMatrixWarMessage()
{}


void CMatrixWarMessage::Init()
{
	m_Second = 0;
	m_OldCount = -1;
	m_NewCount = -1;
	m_dwBefTime = 0;
}

void CMatrixWarMessage::MatrixMsgSet(int sec)
{
	if( sec == 0)
		Init();
	else
	{
		m_Second = sec;
		m_dwBefTime = timeGetTime();
	}

}

void CMatrixWarMessage::MsgProc()
{
	if( m_dwBefTime != 0)
	{
		DWORD temp = timeGetTime() - m_dwBefTime;
		m_NewCount = (int)(temp/1000);

		if( m_NewCount > m_OldCount )
		{
			char temp[512] = {0,};
			sprintf( temp, (char*)G_STRING(IDS_MATRIX_2009_18), m_Second-m_NewCount);
			g_pNk2DFrame->AddChatStr(temp, 4);

			m_OldCount = m_NewCount;
			if( (m_Second - m_NewCount) == 1)
				Init();
		}
	}
}

