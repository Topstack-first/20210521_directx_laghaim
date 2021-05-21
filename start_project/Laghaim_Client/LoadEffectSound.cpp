#include "stdafx.h"

#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "IndexedTexture.h"
#include "country.h"

extern DWORD	g_dwClientCountry;
extern BOOL g_ThaiClient;

extern BOOL g_bUseMatrixCountry; // 매트릭스(뫼비우스 아레나)가 도입된 나라인가의 플래그.(도입 안된나라는 뫼비우스 데이터를 패치 안해주기 때문에 이게 FALSE여야한다.) ///해외뫼비우스

int g_EffectSound[EFF_SND_NUM];
//-- Global font .. Lyul
HFONT g_hFont[G_FONT_NUM];
//-- Flag Font loaded .. Font 가 Load 되지 않았다면 NULL 이 RETURN
bool test_flag_ready_font = false;

//-- 04.03.12 현재 Setfont함수를 각자 가지고 스스로 Createfont 를 수행하는 Class
//--
//--
//-- 13 개의 Class
//-- ProgressBox::SetFont (int f_w, int f_h, char *fontname); == 유일하게f_w 를 사용
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
//-- 각 함수들의 Set Font에서 호출 된다.
//-- 미리 생성된 global Font 를 return 한다.
//-- test_flag_ready_font 가 False 일 경우 NULL 을 RETURN
//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 정녕 나는 If 필살기 쓰지 않을 줄 알았다..
//-- !!!! 12, 4 크기의 FONT 를 호출하는 곳이 있으나. 정작 추적 결과 사용안함
//-- !!!! 일단은 사용 가능한 상태에서 12,0 으로 사용되게 막아둠
HFONT G_GetFont(int h, int w,const char * const fontname)
{
	if(!test_flag_ready_font)
		return NULL;

	//-- 높이는 절대값으로
	h = abs(h);
	//-- 특별한 비율을 가진 FONT -- 하지만 사용은 하지 않는다. ??
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
		// nate 2004 - 4 : CastleWar - 유저가 메세지 박스를 볼 필요가 없다.
		return g_hFont[G_FONT_160];
	}
}
//----------------------------------------------------------------------------
//-- Create Gloabal Font
//----------------------------------------------------------------------------
//-- Global 변수에 Font 를 Create 한다.
//-- 2004.03.15 - 수정완료
//-- Lyul
//--
void Create_Global_Font(int font_idx,int h, int w,char *fontname)
{
	g_hFont[font_idx] = CreateFont (h, w,						// 세로, 가로 크기
									0, 0,					// 기울어짐 각도
									FW_NORMAL,				// 폰트의 굵기
									FALSE,					// 이탤릭 플래그
									FALSE,					// 언더라인 플래그
									FALSE,					// Strikeout 플래그
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
//-- WM CREATE 에서 호출
//-- 게임 상에 사용될 모든 Font 를 Create 한다.
//-- g_dwLangType 를 기준으로 각 국별 생성을 하므로 g_dwLangtype는 필수 적으로
//-- 먼저 정확한 값을 가져야 한다.
//-- 2004.03.15 - 수정완료
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
			Create_Global_Font(G_FONT_80,8,0,"굴림체");
			Create_Global_Font(G_FONT_90,9,0,"굴림체");
			Create_Global_Font(G_FONT_100,10,0,"굴림체");
			//-- 5 Font 로 나옴 ㅡ,.ㅡ
			Create_Global_Font(G_FONT_120,12,0,"굴림체");
			Create_Global_Font(G_FONT_124,12,0,"굴림체");
			Create_Global_Font(G_FONT_130,13,0,"굴림체");
			Create_Global_Font(G_FONT_140,14,0,"굴림체");
			Create_Global_Font(G_FONT_150,15,0,"굴림체");
			Create_Global_Font(G_FONT_160,16,0,"굴림체");
			Create_Global_Font(G_FONT_200,20,0,"굴림체");
			break;
		}
		else
		{
			Create_Global_Font(G_FONT_80,8,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_90,9,0,"MS Sans Serif");
			Create_Global_Font(G_FONT_100,10,0,"MS Sans Serif");
			//-- 5 Font 로 나옴 ㅡ,.ㅡ
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
		Create_Global_Font(G_FONT_80,8,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_90,9,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_100,10,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_120,12,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_124,12,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_130,13,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_140,14,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_150,15,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_160,16,0,"굃굍 뼻뮝");
		Create_Global_Font(G_FONT_200,20,0,"굃굍 뼻뮝");
		break;

	//-- 인도 독일 동일 ??
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
		Create_Global_Font(G_FONT_80,8,0,"芥竟");
		Create_Global_Font(G_FONT_90,9,0,"芥竟");
		Create_Global_Font(G_FONT_100,10,0,"芥竟");
		Create_Global_Font(G_FONT_120,12,0,"芥竟");
		Create_Global_Font(G_FONT_124,12,0,"芥竟");
		Create_Global_Font(G_FONT_130,13,0,"芥竟");
		Create_Global_Font(G_FONT_140,14,0,"芥竟");
		Create_Global_Font(G_FONT_150,15,0,"芥竟");
		Create_Global_Font(G_FONT_160,16,0,"芥竟");
		Create_Global_Font(G_FONT_200,20,0,"芥竟");
		break;

	case LANG_TWN:
		Create_Global_Font(G_FONT_80,8,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_90,9,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_100,10,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_120,12,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_124,12,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_130,13,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_140,14,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_150,15,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_160,16,0,"꾄ⁿ톱");
		Create_Global_Font(G_FONT_200,20,0,"꾄ⁿ톱");
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

	case LANG_BRA: ///브라질
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
//-- 생성된 모든 폰트를 파괴한다.
//-- !!!! Font 를 사용한 녀석이 Old_font를 사용해 Font 를 놓아줄까 ㅡ,.ㅡ?
//-- WM_CLOSE 에서 호출 된다.
//-- 2004.03.15 - 수정완료
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
//-- Effect Sound 와 Texture 를 Load 한다.

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
		//중고렙 스킬 추가 사운드 스킬추가시 주석 지워주장
		g_EffectSound[EFF_SND_BOMBER] = g_pDSound->LoadWAV("bomber.wav");
		g_EffectSound[EFF_SND_BOMBDRP] = g_pDSound->LoadWAV("bombdrp.wav");
		g_EffectSound[EFF_SND_MISSLEBOMB] = g_pDSound->LoadWAV("missle_bomb.wav");
		g_EffectSound[EFF_SND_TONADO] = g_pDSound->LoadWAV("tonado.WAV");
		g_EffectSound[EFF_SND_FISSION] = g_pDSound->LoadWAV("fission.WAV");
		g_EffectSound[EFF_SND_HELL] = g_pDSound->LoadWAV("jump_att.wav");
		//-- 휴먼 총 Sound 다양화
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
		//-- 하리센 사운드
		if( g_dwClientCountry == CTRY_JPN ) // 하리센 사운드는 일본만 로딩하게.(다른 나라는 웨이브파일이 패치가 안되어있는듯.)(없는 사운드(로딩이 안된)의 경우 알아서 딴걸로 대치되는듯.)
		{
			g_EffectSound[EFF_SND_HARISEN  ] = g_pDSound->LoadWAV("Harisen.wav");
			g_EffectSound[EFF_SND_PIKOPIKO ] = g_pDSound->LoadWAV("pikopiko.wav"); ///반짝반짝망치 추가
		}

		//-- Assault Sound
		g_EffectSound[EFF_SND_M_ASSAULT1] = g_pDSound->LoadWAV("fm_skill_Dassault1.wav");
		g_EffectSound[EFF_SND_M_ASSAULT2] = g_pDSound->LoadWAV("fm_skill_Dassault2.wav");
		g_EffectSound[EFF_SND_W_ASSAULT1] = g_pDSound->LoadWAV("fw_skill_Dassault1.wav");
		g_EffectSound[EFF_SND_W_ASSAULT2] = g_pDSound->LoadWAV("fw_skill_Dassault2.wav");

		//-- BattleStart Sound
		if( g_bUseMatrixCountry ) ///해외뫼비우스
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
		g_EffectSound[ EFF_SND_BLOCK ] = g_pDSound->LoadWAV("Block.wav"); //블럭 효과음 ///페어리
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
