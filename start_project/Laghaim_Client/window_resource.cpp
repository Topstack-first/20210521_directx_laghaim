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

	//���Ʈ�� ������
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
	case 0 :	// ��ĭ
		return (char*)G_STRING(IDS_CRECHA_BULKAN);
	case 1 :	// ī�̸���
		return (char*)G_STRING(IDS_CRECHA_KAI);
	case 2 :	// ���̵��
		return (char*)G_STRING(IDS_CRECHA_ADI);
	case 3 :	// �޸�
		return (char*)G_STRING(IDS_CRECHA_HUNMAN);
	case 4 :	// ���̺긮��
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
	case 0 :	// ��ĭ
		return (char*)G_STRING(IDS_CRECHA_BULKAN);
	case 1 :	// ī��
		return (char*)G_STRING(IDS_CRECHA_KAI_S);
	case 2 :	// ����
		return (char*)G_STRING(IDS_CRECHA_ADI_S);
	case 3 :	// �޸�
		return (char*)G_STRING(IDS_CRECHA_HUNMAN);
	case 4 :	// ����
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
	case -1 : // "������"
		return (char*)G_STRING(IDS_OFFLINE);
	case 0 : // "��ī����
		return (char*)G_STRING(IDS_WORLD_00);
	case 1 : // "��ī��"
		return (char*)G_STRING(IDS_WORLD_01);
	case 2 : // "��ī������"
		return (char*)G_STRING(IDS_WORLD_02);
	case 3 : // "���Ϸ�"
		return (char*)G_STRING(IDS_WORLD_03);
	case 4 : // "��׶�̾�"
		return (char*)G_STRING(IDS_WORLD_04);
	case 5 : // "�÷罺Ʈ"
		return (char*)G_STRING(IDS_WORLD_05);
	case 6 : // "�ķν�"
		return (char*)G_STRING(IDS_WORLD_06);
	case 7 : // "�׷���Ʈ"
		return (char*)G_STRING(IDS_WORLD_07);
	case 8 : // "������"
		return (char*)G_STRING(IDS_WORLD_08);
	case 9 : // "���Ʈ��"
		return (char*)G_STRING(IDS_WORLD_09);
	case 10: // "����콺"
		return (char*)G_STRING(IDS_MATRIX);
	case 11: // "��巩ŷ��";
		return (char*)G_STRING(IDS_WORLD_11);
	case 12: // ���Ʈ�� ������ (�̸��� �� ����严���� ǥ��
		return (char*)G_STRING(IDS_WORLD_09);
	case 13: // Ʃ��
		return (char*)G_STRING(IDS_WORLD_13);
	case 14: // ��ī����
		return (char*)G_STRING(IDS_WORLD_14);
	case 15: // �ٸ���
		return (char*)G_STRING(IDS_WORLD_15);
	case 16: // ������
		return (char*)G_STRING(IDS_WORLD_16);
	case 17: // ����׸���
		return (char*)G_STRING(IDS_WORLD_17);
	case 18: // ��������
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
	case 0 : // "��Ÿ"
		return (char*)G_STRING(IDS_GUILD_LEVEL_01);
	case 1 : // "��Ÿ"
		return (char*)G_STRING(IDS_GUILD_LEVEL_02);
	case 2 : // "��Ÿ"
		return (char*)G_STRING(IDS_GUILD_LEVEL_03);
	case 3 : // "��Ÿ"
		return (char*)G_STRING(IDS_GUILD_LEVEL_04);
	case 4 : // "�̿�Ÿ"
		return (char*)G_STRING(IDS_GUILD_LEVEL_05);
	case 5 : // "����"
		return (char*)G_STRING(IDS_GUILD_LEVEL_06);
	case 6 : // "ȭ��"
		return (char*)G_STRING(IDS_GUILD_LEVEL_07);
	case 7 : // "ī��"
		return (char*)G_STRING(IDS_GUILD_LEVEL_08);
	case 8 : // "������"
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
