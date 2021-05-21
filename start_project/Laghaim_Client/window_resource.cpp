#include "stdafx.h"
#include "window_resource.h"
#include "headers.h"
#include "IndexedTexture.h"
#include "g_stringmanager.h"
#include "main.h"





CWINDOWUI* CWINDOWUI::Create()
{
	return new CWINDOWUI;
}

CWINDOWUI::CWINDOWUI()
{
	for(int i = 0; i < WBK_TOTAL_NUM; i++)
	{
		m_pWindowUi[i] = NULL;
	}

	m_pTgaWindowUI = NULL;
}

void CWINDOWUI::LoadResource()
{
	LoadRes();
}

#define CSFBMP CreateSurfaceFromBitmap
void CWINDOWUI::LoadRes()
{
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_BATTLE],		"interface/guild/guild_battle/guild_battle_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_SEARCH],		"interface/guild/guild_search/guildlist.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_CREATE],		"interface/guild/guild_promotion/creation_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_MIX],			"interface/guild/guild_promotion/combination_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_PROMOTION],		"interface/guild/guild_promotion/guild_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_COMBINATION],	"interface/guild/guild_promotion/combination_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_NPCPAGE],		"interface/guild/guild_npc/guildNPC_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_BK],			"interface/guild/guild_common/guild_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_RANKSYSTEM],	"interface/guild/guild_battle/ranksystem.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_RANKPARICIPATE],"interface/guild/guild_battle/rankbattle_list.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_INVASIONINFO],	"interface/guild/guild_battle/invasion_status_bk.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_MEMBERNOTICE],	"interface/guild/guild_notice/guild_notice.bmp");

	for( int i=0 ; i< 12 ; i++ )
		m_pWindowUi[i]->SetColorKey(TRANS_COLOR);

	//드미트론 점령전
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_DMITRON_MENU], "interface/GuildSiege/GuildSiegeBack.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_DMITRON_INFO], "interface/GuildHunt/sub_win.bmp");
	g_pDisplay->CSFBMP(&m_pWindowUi[WBK_DMITRON_TAX], "interface/war_tax/war_tax.bmp");

	for( int i=12 ; i<15 ; i++ )
		m_pWindowUi[i]->SetColorKey(TRANS_COLOR);

	if(  g_SvrType == ST_ADULT_ONLY)
	{
		if(m_pTgaWindowUI)
		{
			m_pTgaWindowUI->InvalidateAllTextures();
			SAFE_DELETE(m_pTgaWindowUI);
		}

		m_pTgaWindowUI = new IndexedTexture(WBK_TGA_TOTAL_NUM);
		m_pTgaWindowUI->SetTexturePath("interface/wanted/");

		m_nTgaWindowNum[WBK_TGA_WANTED_PAGE]			= m_pTgaWindowUI->CreateTextureFromFile("wanted_start_page/start_page_bk.sek");
		m_nTgaWindowNum[WBK_TGA_WANTED_REGISTER]		= m_pTgaWindowUI->CreateTextureFromFile("wanted_register/regist_bk.sek");
		m_nTgaWindowNum[WBK_TGA_WANTED_MYMYNU]			= m_pTgaWindowUI->CreateTextureFromFile("wanted_mymenu/mymenu_bk.sek");
		m_nTgaWindowNum[WBK_TGA_WANTED_REGISTER_LIST]   = m_pTgaWindowUI->CreateTextureFromFile("wanted_register_list/registlist_bk.sek");

		m_pTgaWindowUI->RestoreAllTextures(pCMyApp->GetDevice());
	}

//#endif
}

void CWINDOWUI::DeleteRes()
{
	for(int i = 0; i < WBK_TOTAL_NUM; i++)
	{
		SAFE_DELETE(m_pWindowUi[i]);
	}

	if(m_pTgaWindowUI)
		m_pTgaWindowUI->InvalidateAllTextures();

	SAFE_DELETE(m_pTgaWindowUI);

}

char* CWINDOWUI::GetRaceName(int nNum)
{
	switch(nNum)
	{
	case 0 :	// 불칸
		return (char*)G_STRING(IDS_CRECHA_BULKAN);
	case 1 :	// 카이립톤
		return (char*)G_STRING(IDS_CRECHA_KAI);
	case 2 :	// 에이디아
		return (char*)G_STRING(IDS_CRECHA_ADI);
	case 3 :	// 휴먼
		return (char*)G_STRING(IDS_CRECHA_HUNMAN);
	case 4 :	// 하이브리더
		return (char*)G_STRING(IDS_SCORE_FREEK);
	case 5:
		return (char*)G_STRING(IDS_LHSTRING1893);
	default :
		return NULL;
	}

	return NULL;
}

char* CWINDOWUI::GetRaceShortName(int nNum)
{
	switch(nNum) // msg_kor
	{
	case 0 :	// 불칸
		return (char*)G_STRING(IDS_CRECHA_BULKAN);
	case 1 :	// 카이
		return (char*)G_STRING(IDS_CRECHA_KAI_S);
	case 2 :	// 에이
		return (char*)G_STRING(IDS_CRECHA_ADI_S);
	case 3 :	// 휴먼
		return (char*)G_STRING(IDS_CRECHA_HUNMAN);
	case 4 :	// 하이
		return (char*)G_STRING(IDS_SCORE_FREEK_S);
	case 5:
		return (char*)G_STRING(IDS_LHSTRING1893);
	default :
		return NULL;
	}
}

char* CWINDOWUI::GetZoneName(int nNum)
{
	switch(nNum)
	{
	case -1 : // "미접속"
		return (char*)G_STRING(IDS_OFFLINE);
	case 0 : // "데카르디
		return (char*)G_STRING(IDS_WORLD_00);
	case 1 : // "데카렌"
		return (char*)G_STRING(IDS_WORLD_01);
	case 2 : // "데카둔폐허"
		return (char*)G_STRING(IDS_WORLD_02);
	case 3 : // "샤일론"
		return (char*)G_STRING(IDS_WORLD_03);
	case 4 : // "라그라미아"
		return (char*)G_STRING(IDS_WORLD_04);
	case 5 : // "시루스트"
		return (char*)G_STRING(IDS_WORLD_05);
	case 6 : // "파로스"
		return (char*)G_STRING(IDS_WORLD_06);
	case 7 : // "그레이트"
		return (char*)G_STRING(IDS_WORLD_07);
	case 8 : // "제누스"
		return (char*)G_STRING(IDS_WORLD_08);
	case 9 : // "드미트론"
		return (char*)G_STRING(IDS_WORLD_09);
	case 10: // "뫼비우스"
		return (char*)G_STRING(IDS_MATRIX);
	case 11: // "길드랭킹존";
		return (char*)G_STRING(IDS_WORLD_11);
	case 12: // 드미트론 점령전 (이름이 길어서 드미토른오르 표시
		return (char*)G_STRING(IDS_WORLD_09);
	case 13: // 튜란
		return (char*)G_STRING(IDS_WORLD_13);
	case 14: // 발카리야
		return (char*)G_STRING(IDS_WORLD_14);
	case 15: // 바르샤
		return (char*)G_STRING(IDS_WORLD_15);
	case 16: // 퀘이즈
		return (char*)G_STRING(IDS_WORLD_16);
	case 17: // 베어그리드
		return (char*)G_STRING(IDS_WORLD_17);
	case 18: // 마르보덴
		return (char*)G_STRING(IDS_WORLD_18);
 	case 19:
		return (char*)G_STRING(IDS_WORLD_10);
	case 20:
		return (char*)G_STRING(IDS_LHSTRING1829);
	case 21:
		return (char*)G_STRING(IDS_LHSTRING1830);
	case 22:
		return (char*)G_STRING(IDS_LHSTRING1892);
	case 23:
		return (char*)G_STRING(IDS_LHSTRING1896);
	case 24:
		return (char*)G_STRING(IDS_LHSTRING1897);
	case 25:
		return (char*)G_STRING(IDS_LHSTRING4555);
	case 26:
		return (char*)G_STRING(IDS_LHSTRING4556);
	case 27:
		return (char*)G_STRING(IDS_LHSTRING4557);
	case 28:
		return (char*)G_STRING(IDS_LHSTRING4558);
	case 29:
		return (char*)G_STRING(IDS_LHSTRING4662);
	case 30:
		return (char*)G_STRING(IDS_LHSTRING4663);


	default :
		return " ";
	}

	return NULL;
}

char* CWINDOWUI::GetGuildLevel(int nNum)
{
	switch(nNum) // msg_kor
	{
	case 0 : // "베타"
		return (char*)G_STRING(IDS_GUILD_LEVEL_01);
	case 1 : // "델타"
		return (char*)G_STRING(IDS_GUILD_LEVEL_02);
	case 2 : // "제타"
		return (char*)G_STRING(IDS_GUILD_LEVEL_03);
	case 3 : // "쎄타"
		return (char*)G_STRING(IDS_GUILD_LEVEL_04);
	case 4 : // "이오타"
		return (char*)G_STRING(IDS_GUILD_LEVEL_05);
	case 5 : // "파이"
		return (char*)G_STRING(IDS_GUILD_LEVEL_06);
	case 6 : // "화이"
		return (char*)G_STRING(IDS_GUILD_LEVEL_07);
	case 7 : // "카이"
		return (char*)G_STRING(IDS_GUILD_LEVEL_08);
	case 8 : // "프사이"
		return (char*)G_STRING(IDS_GUILD_LEVEL_09);

	}

	return NULL;
}

CWINDOWUI::~CWINDOWUI()
{
	for(int i = 0; i < WBK_TOTAL_NUM; i++)
	{
		SAFE_DELETE(m_pWindowUi[i]);
	}

	SAFE_DELETE(m_pTgaWindowUI);
}
