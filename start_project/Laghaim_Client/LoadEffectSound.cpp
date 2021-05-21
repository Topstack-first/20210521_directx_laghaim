#include "stdafx.h"

#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "IndexedTexture.h"
#include "country.h"

extern DWORD	g_dwClientCountry;
extern BOOL g_ThaiClient;

extern BOOL g_bUseMatrixCountry; // ��Ʈ����(����콺 �Ʒ���)�� ���Ե� �����ΰ��� �÷���.(���� �ȵȳ���� ����콺 �����͸� ��ġ �����ֱ� ������ �̰� FALSE�����Ѵ�.) ///�ؿܸ���콺

int g_EffectSound[EFF_SND_NUM];
//-- Global font .. Lyul
HFONT g_hFont[G_FONT_NUM];
//-- Flag Font loaded .. Font �� Load ���� �ʾҴٸ� NULL �� RETURN
bool test_flag_ready_font = false;

//-- 04.03.12 ���� Setfont�Լ��� ���� ������ ������ Createfont �� �����ϴ� Class
//--
//--
//-- 13 ���� Class
//-- ProgressBox::SetFont (int f_w, int f_h, char *fontname); == �����ϰ�f_w �� ���
//-- AlphaPopUp.h::SetFont (int f_w, int f_h, char *fontname);
//-- ControlParty::SetFont (int f_w, int f_h, char *fontname);
//-- ControlScore::SetFont (int f_w, int f_h, char *fontname);
//-- ListBox::SetFont (int f_w, int f_h, char *fontname);
//-- im\MsgPopUp.h::SetFont (int f_w, int f_h, char *fontname);
//-- NewListBox.h::SetFont (int f_w, int f_h, char *fontname);
//-- OneSelfInfo::SetFont (int f_w, int f_h, char *fontname);
//-- QlistBox::SetFont (int f_w, int f_h, char *fontname);
//-- QuestData::SetFont (int f_w, int f_h, char *fontname);
//-- SpinBox::SetFont (int f_w, int f_h, char *fontname);
//-- TextBox::SetFont (int f_w, int f_h, char *fontname);
//-- TextOutBox::SetFont (int f_w, int f_h, char *fontname);
//----------------------------------------------------------------------------
//-- Get Gloabal Font
//----------------------------------------------------------------------------
//-- �� �Լ����� Set Font���� ȣ�� �ȴ�.
//-- �̸� ������ global Font �� return �Ѵ�.
//-- test_flag_ready_font �� False �� ��� NULL �� RETURN
//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//-- ���� ���� If �ʻ�� ���� ���� �� �˾Ҵ�..
//-- !!!! 12, 4 ũ���� FONT �� ȣ���ϴ� ���� ������. ���� ���� ��� ������
//-- !!!! �ϴ��� ��� ������ ���¿��� 12,0 ���� ���ǰ� ���Ƶ�
HFONT G_GetFont(int h, int w,const char * const fontname)
{
	if(!test_flag_ready_font)
		return NULL;

	//-- ���̴� ���밪����
	h = abs(h);
	//-- Ư���� ������ ���� FONT -- ������ ����� ���� �ʴ´�. ??
	if(w!=0 && h/2!=w)
	{
		//-- G_FONT_124
		if(h == 12  && w == 4)
			return g_hFont[G_FONT_124];
		else
		{
			return g_hFont[G_FONT_160];
		}
	}
	//-- G_FONT_80
	if(h == 8  ) return 	g_hFont[G_FONT_80];
	//-- G_FONT_90
	else if(h == 9) return 	g_hFont[G_FONT_90];
	//-- G_FONT_100
	else if(h == 10 ) return 	g_hFont[G_FONT_100];
	//-- G_FONT_120
	else if(h == 12  ) return 	g_hFont[G_FONT_120];
	//-- G_FONT_130
	else if(h == 13  ) return 	g_hFont[G_FONT_130];
	//-- G_FONT_140
	else if(h == 14  ) return 	g_hFont[G_FONT_140];
	//-- G_FONT_150
	else if(h == 15  ) return 	g_hFont[G_FONT_150];
	//-- G_FONT_160
	else if(h == 16  ) return 	g_hFont[G_FONT_160];
	//-- G_FONT_200
	else if(h == 20  ) return 	g_hFont[G_FONT_200];
	else
	{
		// nate 2004 - 4 : CastleWar - ������ �޼��� �ڽ��� �� �ʿ䰡 ����.
		return g_hFont[G_FONT_160];
	}
}
//----------------------------------------------------------------------------
//-- Create Gloabal Font
//----------------------------------------------------------------------------
//-- Global ������ Font �� Create �Ѵ�.
//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//--
void Create_Global_Font(int font_idx,int h, int w,char *fontname)
{
	g_hFont[font_idx] = CreateFont (h, w,						// ����, ���� ũ��
									0, 0,					// ������ ����
									FW_NORMAL,				// ��Ʈ�� ����
									FALSE,					// ���Ÿ� �÷���
									FALSE,					// ������� �÷���
									FALSE,					// Strikeout �÷���
									DEFAULT_CHARSET,		// Char SET
									OUT_DEFAULT_PRECIS,
									CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY,
									DEFAULT_PITCH,
									fontname);
}
//----------------------------------------------------------------------------
//-- Load Gloabal Font
//----------------------------------------------------------------------------
//-- WM CREATE ���� ȣ��
//-- ���� �� ���� ��� Font �� Create �Ѵ�.
//-- g_dwLangType �� �������� �� ���� ������ �ϹǷ� g_dwLangtype�� �ʼ� ������
//-- ���� ��Ȯ�� ���� ������ �Ѵ�.
//-- 2004.03.15 - �����Ϸ�
//-- Lyul
void Load_Global_Font()
{
	test_flag_ready_font = true;

	switch (g_dwLangType)
	{
		//-- KOREA
	case LANG_KOR:
		if(!g_ThaiClient)
		{
			Create_Global_Font(G_FONT_80,8,0,"����ü");
			Create_Global_Font(G_FONT_90,9,0,"����ü");
			Create_Global_Font(G_FONT_100,10,0,"����ü");
			//-- 5 Font �� ���� ��,.��
			Create_Global_Font(G_FONT_120,12,0,"����ü");
			Create_Global_Font(G_FONT_124,12,0,"����ü");
			Create_Global_Font(G_FONT_130,13,0,"����ü");
			Create_Global_Font(G_FONT_140,14,0,"����ü");
			Create_Global_Font(G_FONT_150,15,0,"����ü");
			Create_Global_Font(G_FONT_160,16,0,"����ü");
			Create_Global_Font(G_FONT_200,20,0,"����ü");
			break;
		}
		else
		{
			Create_Global_Font(G_FONT_80,8,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_90,9,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_100,10,0,"MS Sans Serif");
			//-- 5 Font �� ���� ��,.��
			Create_Global_Font(G_FONT_120,12,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_124,12,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_130,13,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_140,14,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_150,15,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_160,16,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_200,20,0,"MS Sans Serif");
			break;
		}

		//-- JAPAN
	case LANG_JPN:
		Create_Global_Font(G_FONT_80,8,0,"�l�r ����");
		Create_Global_Font(G_FONT_90,9,0,"�l�r ����");
		Create_Global_Font(G_FONT_100,10,0,"�l�r ����");
		Create_Global_Font(G_FONT_120,12,0,"�l�r ����");
		Create_Global_Font(G_FONT_124,12,0,"�l�r ����");
		Create_Global_Font(G_FONT_130,13,0,"�l�r ����");
		Create_Global_Font(G_FONT_140,14,0,"�l�r ����");
		Create_Global_Font(G_FONT_150,15,0,"�l�r ����");
		Create_Global_Font(G_FONT_160,16,0,"�l�r ����");
		Create_Global_Font(G_FONT_200,20,0,"�l�r ����");
		break;

	//-- �ε� ���� ���� ??
	case LANG_IDN:
	case LANG_DEU:
		Create_Global_Font(G_FONT_80,8,0,"MS Gothic");
		Create_Global_Font(G_FONT_90,9,0,"MS Gothic");
		Create_Global_Font(G_FONT_100,10,0,"MS Gothic");
		Create_Global_Font(G_FONT_120,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_124,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_130,13,0,"MS Gothic");
		Create_Global_Font(G_FONT_140,14,0,"MS Gothic");
		Create_Global_Font(G_FONT_150,15,0,"MS Gothic");
		Create_Global_Font(G_FONT_160,16,0,"MS Gothic");
		Create_Global_Font(G_FONT_200,20,0,"MS Gothic");
		break;

	case LANG_TH:
		Create_Global_Font(G_FONT_80,8,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_90,9,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_100,10,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_120,12,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_124,12,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_130,13,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_140,14,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_150,15,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_160,16,0,"MS Sans Serif");
		Create_Global_Font(G_FONT_200,20,0,"MS Sans Serif");
		break;

	case LANG_CN:
		Create_Global_Font(G_FONT_80,8,0,"����");
		Create_Global_Font(G_FONT_90,9,0,"����");
		Create_Global_Font(G_FONT_100,10,0,"����");
		Create_Global_Font(G_FONT_120,12,0,"����");
		Create_Global_Font(G_FONT_124,12,0,"����");
		Create_Global_Font(G_FONT_130,13,0,"����");
		Create_Global_Font(G_FONT_140,14,0,"����");
		Create_Global_Font(G_FONT_150,15,0,"����");
		Create_Global_Font(G_FONT_160,16,0,"����");
		Create_Global_Font(G_FONT_200,20,0,"����");
		break;

	case LANG_TWN:
		Create_Global_Font(G_FONT_80,8,0,"�ө���");
		Create_Global_Font(G_FONT_90,9,0,"�ө���");
		Create_Global_Font(G_FONT_100,10,0,"�ө���");
		Create_Global_Font(G_FONT_120,12,0,"�ө���");
		Create_Global_Font(G_FONT_124,12,0,"�ө���");
		Create_Global_Font(G_FONT_130,13,0,"�ө���");
		Create_Global_Font(G_FONT_140,14,0,"�ө���");
		Create_Global_Font(G_FONT_150,15,0,"�ө���");
		Create_Global_Font(G_FONT_160,16,0,"�ө���");
		Create_Global_Font(G_FONT_200,20,0,"�ө���");
		break;

	case LANG_ENG:
		Create_Global_Font(G_FONT_80,8,0,"MS Gothic");
		Create_Global_Font(G_FONT_90,9,0,"MS Gothic");
		Create_Global_Font(G_FONT_100,10,0,"MS Gothic");
		Create_Global_Font(G_FONT_120,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_124,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_130,13,0,"MS Gothic");
		Create_Global_Font(G_FONT_140,14,0,"MS Gothic");
		Create_Global_Font(G_FONT_150,15,0,"MS Gothic");
		Create_Global_Font(G_FONT_160,16,0,"MS Gothic");
		Create_Global_Font(G_FONT_200,20,0,"MS Gothic");
		break;

	case LANG_FR:
		Create_Global_Font(G_FONT_80,8,0,"MS Gothic");
		Create_Global_Font(G_FONT_90,9,0,"MS Gothic");
		Create_Global_Font(G_FONT_100,10,0,"MS Gothic");
		Create_Global_Font(G_FONT_120,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_124,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_130,13,0,"MS Gothic");
		Create_Global_Font(G_FONT_140,14,0,"MS Gothic");
		Create_Global_Font(G_FONT_150,15,0,"MS Gothic");
		Create_Global_Font(G_FONT_160,16,0,"MS Gothic");
		Create_Global_Font(G_FONT_200,20,0,"MS Gothic");
		break;

	case LANG_BRA: ///�����
		Create_Global_Font(G_FONT_80,8,0,"MS Gothic");
		Create_Global_Font(G_FONT_90,9,0,"MS Gothic");
		Create_Global_Font(G_FONT_100,10,0,"MS Gothic");
		Create_Global_Font(G_FONT_120,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_124,12,0,"MS Gothic");
		Create_Global_Font(G_FONT_130,13,0,"MS Gothic");
		Create_Global_Font(G_FONT_140,14,0,"MS Gothic");
		Create_Global_Font(G_FONT_150,15,0,"MS Gothic");
		Create_Global_Font(G_FONT_160,16,0,"MS Gothic");
		Create_Global_Font(G_FONT_200,20,0,"MS Gothic");
		break;

	default:
		MessageBox(NULL,"LoadEffectSount => Load_Global_Font() Unknown Language Font : Ask to Lyul",0,0); //--
	}
}


//----------------------------------------------------------------------------
//-- Destory Gloabal Font
//----------------------------------------------------------------------------
//-- ������ ��� ��Ʈ�� �ı��Ѵ�.
//-- !!!! Font �� ����� �༮�� Old_font�� ����� Font �� �����ٱ� ��,.��?
//-- WM_CLOSE ���� ȣ�� �ȴ�.
//-- 2004.03.15 - �����Ϸ�
//-- Lyul
void Destroy_Global_Font()
{
	int i = 0;
	for(i=0; i<G_FONT_NUM; i++)
		DeleteObject(g_hFont[i]);
}


//----------------------------------------------------------------------------
//-- Load Effect_Sound / Texture
//----------------------------------------------------------------------------
//-- Effect Sound �� Texture �� Load �Ѵ�.

void LoadEffectSound()
{
	if (g_pDSound)
	{
		g_pDSound->SetWaveFilePath("data/sound/");
		g_EffectSound[EFF_SND_DROP] = g_pDSound->LoadWAV("drop.wav");
		g_EffectSound[EFF_SND_L_RAIN] = g_pDSound->LoadWAV("l_rain.wav");
		g_EffectSound[EFF_SND_H_RAIN] = g_pDSound->LoadWAV("h_rain.wav");
		g_EffectSound[EFF_SND_L_RAIN_STOP] = g_pDSound->LoadWAV("l_rain_stop.wav");
		g_EffectSound[EFF_SND_H_RAIN_STOP] = g_pDSound->LoadWAV("h_rain_stop.wav");
		g_EffectSound[EFF_SND_LEVELUP] = g_pDSound->LoadWAV("levelup.wav");
		g_EffectSound[EFF_SND_LIGHTNING] = g_pDSound->LoadWAV("lightning.wav");
		g_EffectSound[EFF_SND_SHOT1] = g_pDSound->LoadWAV("shot1.wav");
		g_EffectSound[EFF_SND_BOOM1] = g_pDSound->LoadWAV("boom1.wav");
		g_EffectSound[EFF_SND_SHOT2] = g_pDSound->LoadWAV("shot2.wav");
		g_EffectSound[EFF_SND_BOOM2] = g_pDSound->LoadWAV("boom2.wav");
		g_EffectSound[EFF_SND_BOOM3] = g_pDSound->LoadWAV("boom3.wav");
		g_EffectSound[EFF_SND_DROP_STONE] = g_pDSound->LoadWAV("drop_stone.wav");
		g_EffectSound[EFF_SND_FIRE_PILLAR] = g_pDSound->LoadWAV("fire_pillar.wav");
		g_EffectSound[EFF_SND_STONE_LANCE] = g_pDSound->LoadWAV("stone_lan.wav");
		g_EffectSound[EFF_SND_STAR_FLARE] = g_pDSound->LoadWAV("star_flare.wav");
		g_EffectSound[EFF_SND_SHOCK] = g_pDSound->LoadWAV("shock.wav");
		g_EffectSound[EFF_SND_MUTE3] = g_pDSound->LoadWAV("mute3.wav");
		g_EffectSound[EFF_SND_CONGR] = g_pDSound->LoadWAV("congr2.wav");
		g_EffectSound[EFF_SND_SWORD] = g_pDSound->LoadWAV("sword.wav");
		g_EffectSound[EFF_SND_AXE] = g_pDSound->LoadWAV("axe.wav");
		g_EffectSound[EFF_SND_STAFF] = g_pDSound->LoadWAV("staff.wav");
		g_EffectSound[EFF_SND_FIST] = g_pDSound->LoadWAV("att_unarm.wav");
		g_EffectSound[EFF_SND_EXPLO] = g_pDSound->LoadWAV("explo.wav");
		g_EffectSound[EFF_SND_HORN] = g_pDSound->LoadWAV("horn.wav");
		g_EffectSound[EFF_SND_BEEP1] = g_pDSound->LoadWAV("beep1.wav");
		g_EffectSound[EFF_SND_KWANG] = g_pDSound->LoadWAV("kwang.wav");
		g_EffectSound[EFF_SND_WINCHIP] = g_pDSound->LoadWAV("win_chip.wav");
		g_EffectSound[EFF_SND_SILVER_FLY] = g_pDSound->LoadWAV("silverfly.wav");
		g_EffectSound[EFF_SND_RAVEN] = g_pDSound->LoadWAV("raven.wav");
		g_EffectSound[EFF_SND_KYNES] = g_pDSound->LoadWAV("kynes.wav");
		g_EffectSound[EFF_SND_NEO_SNAKE] = g_pDSound->LoadWAV("neosnake.wav");
		g_EffectSound[EFF_SND_KAIZEN] = g_pDSound->LoadWAV("kaizen.wav");
		g_EffectSound[EFF_SND_MANASHIELD1] = g_pDSound->LoadWAV("mana_shield.wav");
		g_EffectSound[EFF_SND_ELECSHIELD1] = g_pDSound->LoadWAV("elec_shield.wav");
		g_EffectSound[EFF_SND_SAFETYSHIELD1] = g_pDSound->LoadWAV("safety_shield.wav");
		g_EffectSound[EFF_SND_BEE] = g_pDSound->LoadWAV("bee.wav");
		g_EffectSound[EFF_SND_BUTTERFLY] = g_pDSound->LoadWAV("butterfly.wav");
		g_EffectSound[EFF_SND_BORA] = g_pDSound->LoadWAV("bora.wav");
		g_EffectSound[EFF_SND_MOTH] = g_pDSound->LoadWAV("moth.wav");
		g_EffectSound[EFF_SND_BOOM4] = g_pDSound->LoadWAV("boom4.wav");
		g_EffectSound[EFF_SND_LIGHTSWORD] = g_pDSound->LoadWAV("light_sword.wav");
		g_EffectSound[EFF_SND_HAPPYNEWYEAR] = g_pDSound->LoadWAV("bell.wav");
		g_EffectSound[EFF_SND_DROPCANDY] = g_pDSound->LoadWAV("candy.wav");
		//�߰� ��ų �߰� ���� ��ų�߰��� �ּ� ��������
		g_EffectSound[EFF_SND_BOMBER] = g_pDSound->LoadWAV("bomber.wav");
		g_EffectSound[EFF_SND_BOMBDRP] = g_pDSound->LoadWAV("bombdrp.wav");
		g_EffectSound[EFF_SND_MISSLEBOMB] = g_pDSound->LoadWAV("missle_bomb.wav");
		g_EffectSound[EFF_SND_TONADO] = g_pDSound->LoadWAV("tonado.WAV");
		g_EffectSound[EFF_SND_FISSION] = g_pDSound->LoadWAV("fission.WAV");
		g_EffectSound[EFF_SND_HELL] = g_pDSound->LoadWAV("jump_att.wav");
		//-- �޸� �� Sound �پ�ȭ
		g_EffectSound[EFF_SND_GUN_1] = g_pDSound->LoadWAV("gun_1.WAV");
		g_EffectSound[EFF_SND_GUN_2] = g_pDSound->LoadWAV("gun_2.WAV");
		g_EffectSound[EFF_SND_GUN_3] = g_pDSound->LoadWAV("gun_3.WAV");
		g_EffectSound[EFF_SND_GUN_4] = g_pDSound->LoadWAV("gun_4.WAV");
		g_EffectSound[EFF_SND_GUN_5] = g_pDSound->LoadWAV("gun_5.WAV");
		g_EffectSound[EFF_SND_GUN_6] = g_pDSound->LoadWAV("gun_6.WAV");
		g_EffectSound[EFF_SND_GUN_7] = g_pDSound->LoadWAV("gun_7.WAV");
		g_EffectSound[EFF_SND_GUN_8] = g_pDSound->LoadWAV("gun_8.WAV");

		g_EffectSound[EFF_SND_FIREWORKS_4] = g_pDSound->LoadWAV("fw_four.wav");
		g_EffectSound[EFF_SND_FIREWORKS_RAINBOW] = g_pDSound->LoadWAV("fw_rainbow.wav");
		g_EffectSound[EFF_SND_FIREWORKS_SCREW] = g_pDSound->LoadWAV("fw_screw.wav");
		g_EffectSound[EFF_SND_FIREWORKS_FOUNTAIN] = g_pDSound->LoadWAV("fw_fountain.wav");
		//-- �ϸ��� ����
		if( g_dwClientCountry == CTRY_JPN ) // �ϸ��� ����� �Ϻ��� �ε��ϰ�.(�ٸ� ����� ���̺������� ��ġ�� �ȵǾ��ִµ�.)(���� ����(�ε��� �ȵ�)�� ��� �˾Ƽ� ���ɷ� ��ġ�Ǵµ�.)
		{
			g_EffectSound[EFF_SND_HARISEN  ] = g_pDSound->LoadWAV("Harisen.wav");
			g_EffectSound[EFF_SND_PIKOPIKO ] = g_pDSound->LoadWAV("pikopiko.wav"); ///��¦��¦��ġ �߰�
		}

		//-- Assault Sound
		g_EffectSound[EFF_SND_M_ASSAULT1] = g_pDSound->LoadWAV("fm_skill_Dassault1.wav");
		g_EffectSound[EFF_SND_M_ASSAULT2] = g_pDSound->LoadWAV("fm_skill_Dassault2.wav");
		g_EffectSound[EFF_SND_W_ASSAULT1] = g_pDSound->LoadWAV("fw_skill_Dassault1.wav");
		g_EffectSound[EFF_SND_W_ASSAULT2] = g_pDSound->LoadWAV("fw_skill_Dassault2.wav");

		//-- BattleStart Sound
		if( g_bUseMatrixCountry ) ///�ؿܸ���콺
		{
			g_EffectSound[EFF_SND_COUNT] = g_pDSound->LoadWAV("count.wav");
			g_EffectSound[EFF_SND_WIN] = g_pDSound->LoadWAV("Win.wav");
			g_EffectSound[EFF_SND_LOSE] = g_pDSound->LoadWAV("Lose.wav");
			g_EffectSound[EFF_SND_BATTLESTART] = g_pDSound->LoadWAV("app.wav");
			g_EffectSound[ EFF_SND_MINI ] = g_pDSound->LoadWAV("mini.wav");
			g_EffectSound[ EFF_SND_ENTRANCE] = g_pDSound->LoadWAV("entrance.wav");
		}

		// nate 2005-07-21 : Burning Crash - Sound
		g_EffectSound[ EFF_SND_BURNING_1 ] = g_pDSound->LoadWAV("Burning_1.wav");
		g_EffectSound[ EFF_SND_BURNING_2 ] = g_pDSound->LoadWAV("Burning_2.wav");
		g_EffectSound[ EFF_SND_BURNING_3_M ] = g_pDSound->LoadWAV("Burning_3_m.wav");
		g_EffectSound[ EFF_SND_BURNING_3_W ] = g_pDSound->LoadWAV("Burning_3_w.wav");
		g_EffectSound[ EFF_SND_BURNING_4 ] = g_pDSound->LoadWAV("Burning_4.wav");
		g_EffectSound[ EFF_SND_BURNING_5 ] = g_pDSound->LoadWAV("Burning_5.wav");
		g_EffectSound[ EFF_SND_BURNING_6_M ] = g_pDSound->LoadWAV("Burning_6_m.wav");
		g_EffectSound[ EFF_SND_BURNING_6_W ] = g_pDSound->LoadWAV("Burning_6_w.wav");
		g_EffectSound[ EFF_SND_STONE_BREAK ] = g_pDSound->LoadWAV("break_stone.wav");
		g_EffectSound[ EFF_SND_HELLDOWN_CAST_M ] = g_pDSound->LoadWAV("helldown_cast_m.wav");
		g_EffectSound[ EFF_SND_HELLDOWN_CAST_W ] = g_pDSound->LoadWAV("helldown_cast_w.wav");
		g_EffectSound[ EFF_SND_HELLDOWN_FINISH_M ] = g_pDSound->LoadWAV("helldown_finish_m.wav");
		g_EffectSound[ EFF_SND_HELLDOWN_FINISH_W ] = g_pDSound->LoadWAV("helldown_finish_w.wav");
		g_EffectSound[ EFF_SND_BLOCK ] = g_pDSound->LoadWAV("Block.wav"); //�� ȿ���� ///��
		g_EffectSound[ EFF_SND_IRON_SKIN ] = g_pDSound->LoadWAV("iron_skin.wav");
		g_EffectSound[ EFF_SND_FROZEN_SHOT ] = g_pDSound->LoadWAV("frozen_shot.wav");
		g_EffectSound[ EFF_SND_RANDOM_SHOT ] = g_pDSound->LoadWAV("random_shot.wav");
		g_EffectSound[ EFF_SND_TONADO_HAMMER ] = g_pDSound->LoadWAV("tonado_hammer_hit.wav");
		g_EffectSound[ EFF_SND_REVIVAL ] = g_pDSound->LoadWAV("revival.wav");
		g_EffectSound[ EFF_SND_LOCKSMITH ] = g_pDSound->LoadWAV("dice_roll.wav");
		g_EffectSound[ EFF_SND_ROLLET_ONEMORE ] = g_pDSound->LoadWAV("rullet_end_1more.wav");
		g_EffectSound[ EFF_SND_ROLLET_BAD ] = g_pDSound->LoadWAV("rullet_end_bad.wav");
		g_EffectSound[ EFF_SND_ROLLET_DOWN ] = g_pDSound->LoadWAV("rullet_end_down.wav");
		g_EffectSound[ EFF_SND_ROLLET_GOOD ] = g_pDSound->LoadWAV("rullet_end_good.wav");
		g_EffectSound[ EFF_SND_ROLLET_ROLL ] = g_pDSound->LoadWAV("rullet_roll.wav");
	}
}
