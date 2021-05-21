#include "stdafx.h"
// WorldCupStart.cpp: implementation of the CWorldCupStart class.
//
//////////////////////////////////////////////////////////////////////


#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "D3DTypes.h"
#include "IndexedTexture.h"
#include "WorldCupStart.h"
#include "main.h"
#include "LoadEffectSound.h"
#include "DirtSnd.h"
#include "Nk2DFrame.h"





#define REMAINTIME_FONTSIZE 15
#define REMAINTIME_FONTCOLOR RGB(255, 255, 0)
#define GUILDNAME_FONTSIZE 12
#define GUILDNAME_FONTCOLOR RGB(255, 255, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorldCupStart::CWorldCupStart()
{
	m_pSurTime = NULL;
	m_pSurScoreBoard = NULL;
	m_ScreenWidth = 0;
	m_ScreenHeight = 0;
	m_nRemainTime = 0;
	m_nScoreA = 0;
	m_nScoreB = 0;
	m_nScoreA10 = 0;
	m_nScoreB10 = 0;

	m_chAGuildName = NULL;
	m_chBGuildName = NULL;

	for(int i = 0; i < 10; i++)
	{
		m_pSurScoreA[i] = NULL;
		m_pSurScoreB[i] = NULL;
		m_pSurScoreA10[i] = NULL;
		m_pSurScoreB10[i] = NULL;
	}
}

CWorldCupStart::~CWorldCupStart()
{
	DeleteRes();
}

void CWorldCupStart::LoadRes()
{
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurTime, "interface/WorldCup/Time_Table.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurScoreBoard, "interface/WorldCup/the_state_of_play.bmp");
	\

	char tmp[100];
	for(int i = 0; i < 10; i++ )
	{
		sprintf(tmp, "interface/WorldCup/the_state_of_count_%d.bmp", i);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurScoreA[i], tmp);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurScoreB[i], tmp);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurScoreA10[i], tmp);
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurScoreB10[i], tmp);
	}

	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	m_RemainTimeTex.Init(REMAINTIME_FONTSIZE, REMAINTIME_FONTCOLOR, 0, 0, -1, -1, TRUE);
	m_RemainTimeTex.SetVisible(TRUE);
	m_AGuildNameTex.Init(GUILDNAME_FONTSIZE, GUILDNAME_FONTCOLOR, 0, 0, -1, -1, TRUE);
	m_AGuildNameTex.SetVisible(TRUE);
	m_BGuildNameTex.Init(GUILDNAME_FONTSIZE, GUILDNAME_FONTCOLOR, 0, 0, -1, -1, TRUE);
	m_BGuildNameTex.SetVisible(TRUE);

	if( m_pSurTime )
		m_pSurTime->SetColorKey(RGB(0, 0, 0));
	if( m_pSurScoreBoard )
		m_pSurScoreBoard->SetColorKey(RGB(0, 0, 0));
	for( int i = 0; i < 10; i++ )
	{
		if( m_pSurScoreA[i] )
			m_pSurScoreA[i]->SetColorKey(RGB(0, 0, 0));
		if( m_pSurScoreB[i] )
			m_pSurScoreB[i]->SetColorKey(RGB(0, 0, 0));
		if( m_pSurScoreA10[i] )
			m_pSurScoreA10[i]->SetColorKey(RGB(0, 0, 0));
		if( m_pSurScoreB10[i] )
			m_pSurScoreB10[i]->SetColorKey(RGB(0, 0, 0));
	}

	m_pSurTime->Xpos = 360;
	m_pSurScoreBoard->Xpos = 572;
	m_pSurScoreBoard->Ypos = 29;
	m_RemainTimeTex.SetPos( 410, 30 );

	for(int i = 0; i < 10; i++)
	{
		m_pSurScoreA[i]->Xpos = 627;
		m_pSurScoreA[i]->Ypos = 60;
		m_pSurScoreB[i]->Xpos = 737;
		m_pSurScoreB[i]->Ypos = 60;
		m_pSurScoreA10[i]->Xpos = 603;
		m_pSurScoreA10[i]->Ypos = 60;
		m_pSurScoreB10[i]->Xpos = 713;
		m_pSurScoreB10[i]->Ypos = 60;
	}

	m_AGuildNameTex.SetPos( m_pSurScoreBoard->Xpos + 60, 43 );
	m_BGuildNameTex.SetPos( m_pSurScoreBoard->Xpos + 170, 43 );
}

void CWorldCupStart::DeleteRes()
{
	SAFE_DELETE(m_pSurTime);
	SAFE_DELETE(m_pSurScoreBoard);
	for(int i = 0; i < 10; i++)
	{
		SAFE_DELETE(m_pSurScoreA[i]);
		SAFE_DELETE(m_pSurScoreB[i]);
		SAFE_DELETE(m_pSurScoreA10[i]);
		SAFE_DELETE(m_pSurScoreB10[i]);
	}
	m_RemainTimeTex.DeleteRes();
	m_AGuildNameTex.DeleteRes();
	m_BGuildNameTex.DeleteRes();
}

void CWorldCupStart::Draw()
{
	char strLoc[30] = {0,}, strLoc2[30] = {0,}, strLoc3[30] = {0,};

	sprintf(strLoc, "%dºÐ", m_nRemainTime);
	if( m_chAGuildName )
		sprintf(strLoc2, "%s", m_chAGuildName);
	if( m_chBGuildName )
		sprintf(strLoc3, "%s", m_chBGuildName);

	if (g_pDisplay && m_pSurTime)
		g_pDisplay->Blt(m_pSurTime->Xpos, 0, m_pSurTime);
	if (g_pDisplay && m_pSurScoreBoard)
		g_pDisplay->Blt(m_pSurScoreBoard->Xpos, m_pSurScoreBoard->Ypos, m_pSurScoreBoard);

	m_RemainTimeTex.SetString(strLoc,RGB(231, 203, 102),RGB(0,0,0));
	m_RemainTimeTex.Draw();
	m_AGuildNameTex.SetString(strLoc2,RGB(231, 203, 102),RGB(0,0,0));
	m_AGuildNameTex.Draw();
	m_BGuildNameTex.SetString(strLoc3,RGB(231, 203, 102),RGB(0,0,0));
	m_BGuildNameTex.Draw();
}

void CWorldCupStart::DrawScore()
{
	if ( g_pDisplay )
	{
		g_pDisplay->Blt(m_pSurScoreA[m_nScoreA]->Xpos, m_pSurScoreA[m_nScoreA]->Ypos, m_pSurScoreA[m_nScoreA]);
		g_pDisplay->Blt(m_pSurScoreB[m_nScoreB]->Xpos, m_pSurScoreB[m_nScoreB]->Ypos, m_pSurScoreB[m_nScoreB]);
		g_pDisplay->Blt(m_pSurScoreA10[m_nScoreA10]->Xpos, m_pSurScoreA10[m_nScoreA10]->Ypos, m_pSurScoreA10[m_nScoreA10]);
		g_pDisplay->Blt(m_pSurScoreB10[m_nScoreB10]->Xpos, m_pSurScoreB10[m_nScoreB10]->Ypos, m_pSurScoreB10[m_nScoreB10]);
	}
}

void CWorldCupStart::SetRemainTime(int ntime)
{
	m_nRemainTime = ntime;
}

void CWorldCupStart::SetScore(int nScoreA, int nScoreB)
{
	m_nScoreA = nScoreA % 10;
	m_nScoreB = nScoreB % 10;
	m_nScoreA10 = nScoreA / 10;
	m_nScoreB10 = nScoreB / 10;
}

void CWorldCupStart::SetGuildName(char *ch_AGuild, char *ch_BGuild)
{
	SAFE_DELETE_ARRAY( m_chAGuildName );
	SAFE_DELETE_ARRAY( m_chBGuildName );

	if (ch_AGuild && ch_BGuild)
	{
		int len = strlen(ch_AGuild);
		int len2 = strlen(ch_BGuild);
		if (len > 0 && len2 > 0)
		{
			m_chAGuildName = new char[len+1];
			m_chBGuildName = new char[len2+1];
			strcpy(m_chAGuildName, ch_AGuild);
			strcpy(m_chBGuildName, ch_BGuild);
		}
	}
}

