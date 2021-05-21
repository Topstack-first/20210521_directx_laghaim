#include "stdafx.h"
#include "Country.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "g_ImageManager.h"
#include "assert.h"
#include "CommonConfig.h"

#define G_EFFIMAGELOAD(a,b) {g_EffectTextr[ a ] = g_pCapsyongTexture->CreateTextureFromFile( b );assert(g_EffectTextr[a]!=-1);}
// 글로벌 변수
// 텍스처 번호
int g_EffectTextr[ EFF_TEXTR_NUM ];

// 텍스처 포인터

extern BOOL g_bUseMatrixCountry; // 매트릭스(뫼비우스 아레나)가 도입된 나라인가의 플래그.(도입 안된나라는 뫼비우스 데이터를 패치 안해주기 때문에 이게 FALSE여야한다.) ///해외뫼비우스


extern DWORD g_dwClientCountry;

void G_LoadEffectImage()
{
	int i = 0; ///페어리 시 추가
	char szTexName[ 100 ] = { 0, }; ///페어리 시 추가

//===============================================================================================================
	g_pCapsyongTexture->SetTexturePath("data/effect/att_textr/");
	g_EffectTextr[ EFF_FX_HITEFFECTS ] = g_pCapsyongTexture->CreateTextureFromFile( "Hit_Small.tga" );

	g_EffectTextr[ EFF_FX_HITBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "Hit_Big.tga" );
	g_EffectTextr[ EFF_FX_HITBOOMEFFECTS ] = g_pCapsyongTexture->CreateTextureFromFile( "Hit_back.tga" );

	g_EffectTextr[ EFF_FX_HORNBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "hit_horn.tga" );

	g_EffectTextr[ EFF_FX_WORLDCUPBOOM_HIT ] = g_pCapsyongTexture->CreateTextureFromFile( "ballstill.tga" );

	g_EffectTextr[ EFF_FX_HITPET ] = g_pCapsyongTexture->CreateTextureFromFile( "hit_horn.tga" );

	g_EffectTextr[ EFF_FX_WARGIANTATTACK_HIT ] = g_pCapsyongTexture->CreateTextureFromFile( "Hit_Big.tga" );
	g_EffectTextr[ EFF_FX_WARGIANTATTACK_HITBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "Hit_back.tga" );
//===============================================================================================================
	// 패스 설정
	g_pCapsyongTexture->SetTexturePath("data/effect/textr/");

	g_EffectTextr[ EFF_NKCHARACTER_LINE ] = g_pCapsyongTexture->CreateTextureFromFile("test_line.tga");//-- 궤적
	g_EffectTextr[ EFF_NKCHARACTER_LINE2 ] = g_pCapsyongTexture->CreateTextureFromFile("test_light.tga");//-- 광선검
	G_EFFIMAGELOAD(EFF_NKCHARACTER_LINE3,"bleed_line.tga"); //-- 블레드 불 효과

	g_EffectTextr[ EFF_TEXTR_BLACK ] = g_pCapsyongTexture->CreateTextureFromFile("black2.tga");
	g_EffectTextr[ EFF_FIREFLY ] = g_pCapsyongTexture->CreateTextureFromFile("firefly.tga");
	g_EffectTextr[ EFF_3DPOSPOINT_POSDIR ] = g_pCapsyongTexture->CreateTextureFromFile("posdir.tga");

	g_EffectTextr[ EFF_TEXTR_BLACK2 ] = g_pCapsyongTexture->CreateTextureFromFile("black2.tga");
	g_EffectTextr[ EFF_ENERGEBOX_00 ] = g_pCapsyongTexture->CreateTextureFromFile("energe_00.tga");
	g_EffectTextr[ EFF_CUTE_BACK ]    = g_pCapsyongTexture->CreateTextureFromFile("info_back.tga");
	g_EffectTextr[ EFF_CH_HAIR_BACK ]    = g_pCapsyongTexture->CreateTextureFromFile("ch_hair.tga");
	g_EffectTextr[ EFF_GWAR_FLAG_A ]    = g_pCapsyongTexture->CreateTextureFromFile("flag_a.tga");
	g_EffectTextr[ EFF_GWAR_FLAG_B ]    = g_pCapsyongTexture->CreateTextureFromFile("flag_b.tga");
	g_EffectTextr[ EFF_STAR_1 ]    = g_pCapsyongTexture->CreateTextureFromFile("star1.tga");
	g_EffectTextr[ EFF_TEXTR_WATER ]    = g_pCapsyongTexture->CreateTextureFromFile("water.bmp");
	g_EffectTextr[ EFF_TEXTR_WAVE ]    = g_pCapsyongTexture->CreateTextureFromFile("wave3.bmp");
	g_EffectTextr[ EFF_TEXTR_LAVA ]    = g_pCapsyongTexture->CreateTextureFromFile("lava.bmp");
	g_EffectTextr[ EFF_VITAL_BAR ]    = g_pCapsyongTexture->CreateTextureFromFile("vital_bar.tga");
	g_EffectTextr[ EFF_VITAL_BAR_BACK ]    = g_pCapsyongTexture->CreateTextureFromFile("vital_bar_back.tga");
	g_EffectTextr[ EFF_MYPARTYS ]    = g_pCapsyongTexture->CreateTextureFromFile("my_partys.tga");
	g_EffectTextr[ EFF_MYPARTS2 ]    = g_pCapsyongTexture->CreateTextureFromFile("my_partys2.tga");
	g_EffectTextr[ EFF_NUMBER_0 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_0.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_1 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_1.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_2 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_2.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_3 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_3.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_4 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_4.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_5 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_5.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_6 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_6.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_7 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_7.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_8 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_8.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_9 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_9.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_10 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_10.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_11 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_11.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_12 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_12.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_13 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_13.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_14 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_14.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_15 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_15.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_16 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_16.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_17 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_17.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_18 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_18.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
	g_EffectTextr[ EFF_NUMBER_19 ]	= g_pCapsyongTexture->CreateTextureFromFileNonePack("interface/WorldCup/the_state_of_count_19.SEK", -1, 0L, D3DTEXTR_TRANSPARENTALPHA, 0xFE);
//===================================================================================================
	// nate 2004 - 6
	// 크리스탈 소유자 좌표 출력
	g_EffectTextr[ EFF_CRISTAL_OWNER ]    = g_pCapsyongTexture->CreateTextureFromFile("Cristal_owner.bmp");
//===================================================================================================
	g_EffectTextr[ EFF_TEXTR_MATWATER ] = g_pCapsyongTexture->CreateTextureFromFile("mat_water.bmp");
	g_EffectTextr[ EFF_TEXTR_MATWAVE ]    = g_pCapsyongTexture->CreateTextureFromFile("mat_wave3.bmp");
	g_EffectTextr[ EFF_GUILD_NOTICE ]    = g_pCapsyongTexture->CreateTextureFromFile("info_guild.tga");
	g_EffectTextr[ EFF_PCBANG_USER ]    = g_pCapsyongTexture->CreateTextureFromFile("pcbang.tga");
	g_EffectTextr[ EFF_PCBANG_USER_GAGE ]	= g_pCapsyongTexture->CreateTextureFromFile("pcbangGage.tga"); // [2008/5/29 Theodoric] Pc방 30분 채크
	g_EffectTextr[ EFF_PREMIUM_USER ]		= g_pCapsyongTexture->CreateTextureFromFile("premium.tga");

	
	g_EffectTextr[ EFF_AUTOMOUSE ] = g_pCapsyongTexture->CreateTextureFromFile("dic_num.tga");
// MAIN
//==============================
	// 하늘 텍스쳐를 위한..
	g_EffectTextr[ EFF_SKY01 ] = g_pCapsyongTexture->CreateTextureFromFile("cloud.bmp"); // 낮 하늘
	g_EffectTextr[ EFF_SKY02 ] = g_pCapsyongTexture->CreateTextureFromFile("addon_cloud.bmp"); // 구름(합성을위한)
	g_EffectTextr[ EFF_SKY03 ] = g_pCapsyongTexture->CreateTextureFromFile("flow_cloud.bmp");  // 구름(스크롤을 위한)
	g_EffectTextr[ EFF_SKY04 ] = g_pCapsyongTexture->CreateTextureFromFile("noul_sky.bmp");    // 노을 하늘
	g_EffectTextr[ EFF_SKY05 ] = g_pCapsyongTexture->CreateTextureFromFile("night_sky.bmp");   // 밤   하늘
	g_EffectTextr[ EFF_SKY06 ] = g_pCapsyongTexture->CreateTextureFromFile("sebuk_sky.bmp");   // 새벽 하늘

	// 이것의 위치 중요.
//=======================================
	// nate 2004 - 6
	// 테카르디 캐릭턱 팀 구분
	// 바닦 텍스처 로딩
	g_EffectTextr[ EFF_BLUE ] = g_pCapsyongTexture->CreateTextureFromFile("blue.tga");
	g_EffectTextr[ EFF_RED ] = g_pCapsyongTexture->CreateTextureFromFile("red.tga");
//=======================================
	g_EffectTextr[ EFF_PARTY_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("start_base.tga");
	g_EffectTextr[ EFF_GUILDT_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("guild_base.tga");
	g_EffectTextr[ EFF_OPPGUILD_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("oguild_base.tga");
	g_EffectTextr[ EFF_A_TEAM_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("red_a_base.tga");
	g_EffectTextr[ EFF_B_TEAM_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("blue_a_base.tga");
	g_EffectTextr[ EFF_FLAG_OWN_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("flag_base.tga");	// 길드전에 사용

	g_EffectTextr[ EFF_A_GUILD_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("red_a_base.tga"); // 텍스쳐파일 변경 예정
	g_EffectTextr[ EFF_B_GUILD_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("blue_a_base.tga");
	g_EffectTextr[ EFF_C_GUILD_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("flag_base.tga");

	g_EffectTextr[ EFF_DIR_FLAG1_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("northFlag.tga");
	g_EffectTextr[ EFF_DIR_FLAG2_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("eastFlag.tga");
	g_EffectTextr[ EFF_DIR_FLAG3_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("westFlag.tga");
	g_EffectTextr[ EFF_DIR_FLAG4_TEX ] = g_pCapsyongTexture->CreateTextureFromFile("southFlag.tga");

	// 커튼 효과 이미지 로딩.
	g_EffectTextr[ EFF_CUTTON_IDX1 ] = g_pCapsyongTexture->CreateTextureFromFile("cutton.bmp");
	g_EffectTextr[ EFF_CUTTON_IDX2 ] = g_pCapsyongTexture->CreateTextureFromFile("cutton_alter.bmp");

	g_EffectTextr[ EFF_CLOUDSEA_SKY ] = g_pCapsyongTexture->CreateTextureFromFile("sky/cloudsea_sky.bmp");
	g_EffectTextr[ EFF_CLOUDSEA_CLOUD ] = g_pCapsyongTexture->CreateTextureFromFile("sky/cloudsea_cloud.bmp");
	//===============================================================================================
	// nate 2004 - 4
	// Crash_Miss
	g_EffectTextr[ EFF_COMBO ] = g_pCapsyongTexture->CreateTextureFromFile( "msg_combo.tga" );
	g_EffectTextr[ EFF_COMBO2 ] = g_pCapsyongTexture->CreateTextureFromFile( "msg_combo2.tga" ); //mungmae-2005/05/06
	g_EffectTextr[ EFF_MISS ] = g_pCapsyongTexture->CreateTextureFromFile( "msg_miss.tga" );
	// nate 2005-05-09 : 이벤트포인트(EP)획득시 서버 메세지에 의해 나타남
	g_EffectTextr[ EFF_EVENTPOINT ] = g_pCapsyongTexture->CreateTextureFromFile( "eff_getep.tga" );
	//===============================================================================================
	g_EffectTextr[ EFF_TEXTRSKY00 ] = g_pCapsyongTexture->CreateTextureFromFile("sky/sebuk_sky.bmp");
	g_EffectTextr[ EFF_TEXTRSKY01 ] = g_pCapsyongTexture->CreateTextureFromFile("sky/day_sky.bmp");
	g_EffectTextr[ EFF_TEXTRSKY02 ] = g_pCapsyongTexture->CreateTextureFromFile("sky/noul_sky.bmp");
	g_EffectTextr[ EFF_TEXTRSKY03 ] = g_pCapsyongTexture->CreateTextureFromFile("sky/night_sky.bmp");
	g_EffectTextr[ EFF_TEXTRSKY04 ] = g_pCapsyongTexture->CreateTextureFromFile("sky/flow_cloud3.bmp");
	g_EffectTextr[ EFF_TEXTRSKY05 ] = g_pCapsyongTexture->CreateTextureFromFile("sky/mat_sky.bmp");	// matrix

	G_EFFIMAGELOAD(EFF_NPC_EVENT ,"npc_event.tga" );					//-- NPC Event
	G_EFFIMAGELOAD(EFF_GIFT_MESSAGE ,"npc_event2.tga");       //-- NPC Event
	G_EFFIMAGELOAD(EFF_MSG_E_ENCOUNTER, "npc_encounter.tga"); //-- NPC E_Encounter

	if( CommonConfig::Instance()->GetUsingLP() == true )
	{
		G_EFFIMAGELOAD(EFF_LP_SHOP_NPC ,"npc_lpshop.tga" );
	}
	else
	{
		G_EFFIMAGELOAD(EFF_BP_SHOP_NPC ,"npc_bpshop.tga" );
	}
	G_EFFIMAGELOAD(EFF_YAN_EVENT, "npc_yanquest.tga" );		//mungmae 05/12/12 얀 퀘스트
	//------------------------------------------------------------------------
	//-- Lyul_2005 Skill
	G_EFFIMAGELOAD(EFF_SKILL_DSTONE_1,"eff_skill_dstone_1.tga");
	G_EFFIMAGELOAD(EFF_SKILL_DSTONE_2,"eff_skill_dstone_2.tga");
	G_EFFIMAGELOAD(EFF_SKILL_DSLASH_1,"eff_skill_dslash.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_1_0,"eff_dstone_cir1_0.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_1_1,"eff_dstone_cir1_1.tga");

	G_EFFIMAGELOAD(EFF_BLEED_BOOM,"eff_bleed_boom.tga"); //-- 블레드 시전 효과
	G_EFFIMAGELOAD(EFF_ASSAULT,"eff_assault.tga"); //-- 어썰트 이펙트

	G_EFFIMAGELOAD(EFF_ASSAULT_BOOM,"eff_assault_boom.tga"); //-- 어썰트 추가 이펙트
	G_EFFIMAGELOAD(EFF_DSWORD_HIT,"eff_dsword_hit.tga"); //-- 이도류 이펙트

	//-- 디바인 스톤 마법진 이펙트
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_0_0,"eff_dstone_cir0_0.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_0_1,"eff_dstone_cir0_1.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_1_0,"eff_dstone_cir1_0.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_1_1,"eff_dstone_cir1_1.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_SMOKE,"eff_dstone_smoke.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_SMOKE2,"eff_dstone_smoke2.tga");  //elite bds effect
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_2_0,"eff_dstone_cir2_0.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_3_0,"eff_dstone_cir3_0.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_0_2,"eff_dstone_cir0_2.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_4_0,"eff_dstone_cir4_0.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_5_0,"eff_dstone_cir5_0.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_0_3,"eff_dstone_cir0_3.tga");
	G_EFFIMAGELOAD(EFF_DSTONE_EFF_0_4,"eff_dstone_cir0_4.tga");
//====================================================
	// nate 2005-02-28
	// Matrix
	// 매트릭스가 도입되는 나라만 로딩되게 처리.(by 원석) ///해외뫼비우스
	if( g_bUseMatrixCountry )
	{
		G_EFFIMAGELOAD( EFF_INDICATION_POINT, "eff_indication_point.bmp" );
		G_EFFIMAGELOAD( EFF_C_MARK, "C_Mark.tga" );
		G_EFFIMAGELOAD( EFF_S_MARK, "S_Mark.tga" );
		G_EFFIMAGELOAD( EFF_H_MARK, "H_Mark.tga" );
		G_EFFIMAGELOAD( EFF_SHOP_NPC, "npc_shop.tga" );
	}

	G_EFFIMAGELOAD( EFF_FX_SUMMONSQUAD, "Summon_Squad.tga" );	// nate 2005-07-08 : Summon Squad
//====================================================
	////////////////////////////////////////////////////////////////
	///미니맵길드원찾기3D
	G_EFFIMAGELOAD( EFF_GUILDMEMBER, "mini_map/guild_member.tga"); ///미니맵길드원찾기3D
	G_EFFIMAGELOAD( EFF_GUILDMASTER, "mini_map/guild_master.tga"); ///미니맵길드원찾기3D
	G_EFFIMAGELOAD( EFF_GUILDPOS_SIDE, "mini_map/guildpos_eff.tga"); ///미니맵길드원찾기3D
	G_EFFIMAGELOAD( EFF_GUILDPOS_SIDE2, "mini_map/guildpos_eff2.tga"); ///미니맵길드원찾기3D
	////////////////////////////////////////////////////////////////
	G_EFFIMAGELOAD( EFF_SCRATCH, "eff_sctatch.tga"); // nate 2006-01-18 : 할귄자국

	G_EFFIMAGELOAD( EFF_FX_FAIRY_MOVE, "follow/fairy_any_move.tga"); // 원석 2006-03-09 : 345레벨 아이템용 이펙트 페어리 이동 애니메이션 ///페어리
	G_EFFIMAGELOAD( EFF_FX_FAIRY_ACT1, "follow/fairy_any_ACT1.tga"); // 원석 2006-03-09 : 345레벨 아이템용 이펙트 페어리 이동 애니메이션 ///페어리
	G_EFFIMAGELOAD( EFF_FX_FAIRY_LIGHT, "follow/fairy_light.tga"); // 원석 2006-03-09 : 345레벨 아이템용 이펙트 페어리 주변 광채 이펙트 ///페어리
	G_EFFIMAGELOAD( EFF_BLOCK, "follow/msg_block.tga"); // 원석 2006-03-09 : 345레벨 아이템용 이펙트 블럭 문구. ///페어리

	for( i = 0 ; i < EFF_FX_FAIRY_PROTECT_NUM ; i ++ )
	{
		sprintf( szTexName, "follow/fairy_lolo_protect_%02d.tga", i );
		G_EFFIMAGELOAD( EFF_FX_FAIRY_PROTECT1+i, szTexName ); // 원석 2006-03-09 : 345레벨 아이템용 이펙트 페어리 방어막 롤로 텍스쳐 애니메이션용 ///페어리
	}

	G_EFFIMAGELOAD( EFF_FX_PUSHSPACE_UP, "PushSpaceUp.tga"); // 원석 2006-06-03 : 스페이스바 누르라는 표시용 그림
	G_EFFIMAGELOAD( EFF_FX_PUSHSPACE_DOWN, "PushSpaceDown.tga"); // 원석 2006-06-03 : 스페이스바 누르라는 표시용 그림

	G_EFFIMAGELOAD( EFF_FX_HEART_PARTICLE, "heart.tga"); // 원석 2007-01-18 : ///하트로드 공격 이펙트
	G_EFFIMAGELOAD( EFF_KOREA_SERVICE_LEVEL_MARK, "KorServiceLevel.tga"); // 한국 심의 등급 표시용 마크. 12세용
	G_EFFIMAGELOAD( EFF_KOREA_SERVICE_LEVEL_MARK2, "KorServiceLevel2.tga"); // 한국 심의 등급 표시용 마크. 18세용
	G_EFFIMAGELOAD( EFF_FX_LUCKYPIG_PARTICLE, "heal-e_star-color.tga"); // 설날 복돼지 회복 이펙트용.

	G_EFFIMAGELOAD(	EFF_FX_LIP_PARTICLE	  , "u_lip.tga");	// 2007.03.14 화이트
	G_EFFIMAGELOAD(	EFF_FX_LOVE_L_PARTICLE, "u_love_l.tga");
	G_EFFIMAGELOAD(	EFF_FX_LOVE_O_PARTICLE, "u_love_o.tga");
	G_EFFIMAGELOAD(	EFF_FX_LOVE_V_PARTICLE, "u_love_v.tga");
	G_EFFIMAGELOAD(	EFF_FX_LOVE_E_PARTICLE, "u_love_e.tga");

	G_EFFIMAGELOAD( EFF_FX_VAMPIREBAT_STAND, "follow/VampireBat_front.tga"); ///뱀파이어망토 박쥐
	G_EFFIMAGELOAD( EFF_FX_VAMPIREBAT_MOVE, "follow/VampireBat_left.tga"); ///뱀파이어망토 박쥐
	G_EFFIMAGELOAD( EFF_FX_RING_LIGHT_EFFECT, "follow/fairy_lolo_protect_00.tga"); // [12/6/2007 반재승] 일본 링 아이템 이펙트.

	g_EffectTextr[ EFF_FX_ARUPAKA ] = g_pCapsyongTexture->CreateTextureFromFile( "saliva.tga" );
	g_EffectTextr[ EFF_FX_ARUPAKA_DROP ] = g_pCapsyongTexture->CreateTextureFromFile( "saliva01.tga" );
}


// nate 2004 - 7 - image
void G_LoadFXEffectImage()
{
	g_pCapsyongTexture->SetTexturePath( "data/effect/textr/" );

	g_EffectTextr[ EFF_FX_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "light.tga" );
	g_EffectTextr[ EFF_FX_PORTAL ] = g_pCapsyongTexture->CreateTextureFromFile( "sample1.bmp" );
	g_EffectTextr[ EFF_FX_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "smoke.tga" );
	g_EffectTextr[ EFF_FX_SHADOWRING ] = g_pCapsyongTexture->CreateTextureFromFile( "circle_shadow.tga" );
	g_EffectTextr[ EFF_FX_SHADOWLIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "lightmap.tga" );
	g_EffectTextr[ EFF_FX_PARTICLE ] = g_pCapsyongTexture->CreateTextureFromFile( "flare.tga" );
	g_EffectTextr[ EFF_FX_LIGHTFOG ] = g_pCapsyongTexture->CreateTextureFromFile( "FLARE2.BMP" );
	g_EffectTextr[ EFF_FX_SHOWUP ] = g_pCapsyongTexture->CreateTextureFromFile( "small_flare.bmp" );
	g_EffectTextr[ EFF_FX_SHOWUPSTART ] = g_pCapsyongTexture->CreateTextureFromFile( "start_base.tga" );
	g_EffectTextr[ EFF_FX_LIGHTNING ] = g_pCapsyongTexture->CreateTextureFromFile( "lightning-2.tga" );
	g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] = g_pCapsyongTexture->CreateTextureFromFile( "lightning_spark1.tga" );
	g_EffectTextr[ EFF_FX_LIGHTBALLTAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "lightning_spark.tga" );
	g_EffectTextr[ EFF_FX_LIGHTBALLHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "ihit.tga" );
	g_EffectTextr[ EFF_FX_LIGHTBALLBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "lightningball_base.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball1.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLTAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_base.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "fire_burst.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLBOOMTAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLBOOMBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_base.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLBOOM2 ] = g_pCapsyongTexture->CreateTextureFromFile( "fire_burst2.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLBOOM2TAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );
	g_EffectTextr[ EFF_FX_FIREBALLBOOM2BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_base.tga" );
	g_EffectTextr[ EFF_FX_LWLIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "lw_light.tga" );
	g_EffectTextr[ EFF_FX_LIGHTBALLBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "ihit.tga" );
	g_EffectTextr[ EFF_FX_LIGHTBALLBOOMBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "lightningball_base.tga" );
	g_EffectTextr[ EFF_FX_UNARMEDATTACKKE ] = g_pCapsyongTexture->CreateTextureFromFile( "iHit.tga" );
	g_EffectTextr[ EFF_FX_METEORLIGHTNING ] = g_pCapsyongTexture->CreateTextureFromFile( "meteor.tga" );
	g_EffectTextr[ EFF_FX_METEORSPARK ] = g_pCapsyongTexture->CreateTextureFromFile( "meteor_base.tga" );
	g_EffectTextr[ EFF_FX_PEBBLESHOTHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "stone.tga" );
	g_EffectTextr[ EFF_FX_PEBBLESHOTTAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "stone_smoke.tga" );
	g_EffectTextr[ EFF_FX_STONESHOTHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "stone.tga" );
	g_EffectTextr[ EFF_FX_STONESHOTBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "stone_smoke.tga" );
	g_EffectTextr[ EFF_FX_STONESHOTBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "stone_burst.tga" );
	g_EffectTextr[ EFF_FX_PEBBLESHOTBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "stone_burst.tga" );
	g_EffectTextr[ EFF_FX_SHOOT ] = g_pCapsyongTexture->CreateTextureFromFile( "fire.tga" );
	g_EffectTextr[ EFF_FX_SHOOTATTACKED ] = g_pCapsyongTexture->CreateTextureFromFile( "fire_attacked.tga" );
	g_EffectTextr[ EFF_FX_AIDIAFIREHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM0 ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp00.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM1 ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp01.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM2 ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp02.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM3 ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp03.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM4 ] =  g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp04.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM5 ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp05.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM6 ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp06.tga" );
	g_EffectTextr[ EFF_FX_AIDIABALLBOOM7 ] = g_pCapsyongTexture->CreateTextureFromFile( "eng_ball_exp07.tga" );
	g_EffectTextr[ EFF_FX_LIGHTHEAL ] = g_pCapsyongTexture->CreateTextureFromFile( "FLARE2.BMP" );
	g_EffectTextr[ EFF_FX_STARFLAREHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "star.tga" );
	g_EffectTextr[ EFF_FX_STARFLAREBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "star_smoke.tga" );
	g_EffectTextr[ EFF_FX_STARFLAREBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "star_fire.tga" );
	g_EffectTextr[ EFF_FX_SHOCKSPEAR ] = g_pCapsyongTexture->CreateTextureFromFile( "shock_ball.bmp" );
	g_EffectTextr[ EFF_FX_SHOCKSPEARSTART ] = g_pCapsyongTexture->CreateTextureFromFile( "shock.tga" );
	g_EffectTextr[ EFF_FX_SHOCKSPEARSPHERE ] = g_pCapsyongTexture->CreateTextureFromFile( "sphere.tga" );
	g_EffectTextr[ EFF_FX_WANTED ] = g_pCapsyongTexture->CreateTextureFromFile( "backlight.bmp" );

	int i = 0;
	char szTexName[ 100 ] = { 0, };
	for( i = 0 ; i < EFF_FX_SHOCKSPEAR_NUM ; i ++ )
	{
		sprintf( szTexName, "effect0%d.tga", i + 1 );
		g_EffectTextr[ EFF_FX_SHOCKSPEARLIGHT + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	for( i = 0 ; i < EFF_FX_SPARK_NUM ; i ++ )
	{
		sprintf( szTexName, "spark0%d.tga", i + 1 );
		g_EffectTextr[ EFF_FX_SPARK + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	g_EffectTextr[ EFF_FX_STONERANGEBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "earth.tga" );
	g_EffectTextr[ EFF_FX_STONERANGESMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );

	// nate 2005-09-30 : 듀란트 - 공격 대상
	for( i = 0 ; i < EFF_FX_ELECT_NUM ; i ++ )
	{
		sprintf( szTexName, "elect0%d.tga", i + 1 );
		g_EffectTextr[ EFF_FX_ELECT + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	g_EffectTextr[ EFF_FX_REDFIRE ] = g_pCapsyongTexture->CreateTextureFromFile( "red_fire.tga" );
	g_EffectTextr[ EFF_FX_BEEHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "bugball1.tga" );
	g_EffectTextr[ EFF_FX_BEETAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "bugball_1.tga" );
	g_EffectTextr[ EFF_FX_BEEBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "bug_burst.tga" );
	g_EffectTextr[ EFF_FX_BEEBOOMTAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "bugball_1.tga" );
	g_EffectTextr[ EFF_FX_BORAHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "bug1ball1.tga" );
	g_EffectTextr[ EFF_FX_BORATAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "bug1ball_1.tga" );
	g_EffectTextr[ EFF_FX_BORABOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "bug1_burst.tga" );
	g_EffectTextr[ EFF_FX_BORABOOMTAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "bug1ball_1.tga" );
	g_EffectTextr[ EFF_FX_SHILONBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "c_meteor.tga" );
	g_EffectTextr[ EFF_FX_SHILONBASE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_base.tga" );
	g_EffectTextr[ EFF_FX_LEVELUP_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "eff_side.tga" );
	g_EffectTextr[ EFF_FX_LEVELUP_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_LEVELUP_ROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_SHOWUP2_SHIELD ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_SHOWUP2_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_SHOWUP2_LEVELUP ] = g_pCapsyongTexture->CreateTextureFromFile( "autolaser.tga" );
	g_EffectTextr[ EFF_FX_SHOWUP2_ROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_EAT_SMILE ] = g_pCapsyongTexture->CreateTextureFromFile( "que.tga" );
	g_EffectTextr[ EFF_FX_EAT_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "meteor_base.tga" );
	g_EffectTextr[ EFF_FX_SHIELD1 ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_SHIELD2 ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_SMOKEPARTICLE ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_SPIRITMODE ] = g_pCapsyongTexture->CreateTextureFromFile( "blue_a_base.tga" );
	g_EffectTextr[ EFF_FX_SPIRITMODE2 ] = g_pCapsyongTexture->CreateTextureFromFile( "trace3.tga" );

	// FIRE PARTICLE - RED FIRE
	for( i = 0 ; i < EFF_FX_FIREPARTICLE1_NUM ; i ++ )
	{
		sprintf( szTexName, "fire/fire10%d.tga", i );
		g_EffectTextr[ EFF_FX_FIREPARTICLE1 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for( i = 0 ; i < EFF_FX_FIREPARTICLE2_NUM ; i ++ )
	{
		sprintf( szTexName, "fire/fire20%d.tga", i );
		g_EffectTextr[ EFF_FX_FIREPARTICLE2 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for( i = 0 ; i < EFF_FX_FIREPARTICLE3_NUM ; i ++ )
	{
		sprintf( szTexName, "fire/fire30%d.tga", i );
		g_EffectTextr[ EFF_FX_FIREPARTICLE3 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for( i = 0 ; i < EFF_FX_FIREPARTICLE4_NUM ; i++ )
	{
		sprintf( szTexName, "fire/fire40%d.tga", i );
		g_EffectTextr[ EFF_FX_FIREPARTICLE4 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for( i = 0 ; i < EFF_FX_FIREPARTICLE5_NUM ; i++ )
	{
		sprintf( szTexName, "fire/fire50%d.tga", i );
		g_EffectTextr[ EFF_FX_FIREPARTICLE5 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for( i = 0 ; i < EFF_FX_FIREPARTICLE6_NUM ; i++ )
	{
		sprintf( szTexName, "fire/fire60%d.tga", i );
		g_EffectTextr[ EFF_FX_FIREPARTICLE6 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	// FIRE PARTICLE - Blue Fire
	for( i = 0 ; i < EFF_FX_BFIREPARTICLE1_NUM ; i++ )
	{
		sprintf( szTexName, "fire/bfire10%d.tga", i );
		g_EffectTextr[ EFF_FX_BFIREPARTICLE1 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for( i = 0 ; i < EFF_FX_BFIREPARTICLE2_NUM ; i++ )
	{
		sprintf( szTexName, "fire/bfire20%d.tga", i );
		g_EffectTextr[ EFF_FX_BFIREPARTICLE2 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for( i = 0 ; i < EFF_FX_BFIREPARTICLE3_NUM ; i++ )
	{
		sprintf( szTexName, "fire/bfire30%d.tga", i );
		g_EffectTextr[ EFF_FX_BFIREPARTICLE3 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for(i = 0 ; i < EFF_FX_BFIREPARTICLE4_NUM ; i ++ )
	{
		sprintf( szTexName, "fire/bfire40%d.tga", i );
		g_EffectTextr[ EFF_FX_BFIREPARTICLE4 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for(i = 0 ; i < EFF_FX_BFIREPARTICLE5_NUM ; i ++ )
	{
		sprintf( szTexName, "fire/bfire50%d.tga", i );
		g_EffectTextr[ EFF_FX_BFIREPARTICLE5 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}
	for(i = 0 ; i < EFF_FX_BFIREPARTICLE6_NUM ; i ++ )
	{
		sprintf( szTexName, "fire/bfire60%d.tga", i );
		g_EffectTextr[ EFF_FX_BFIREPARTICLE6 + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	g_EffectTextr[ EFF_FX_HEAL_GUARD ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_HEAL_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_light.tga" );
	g_EffectTextr[ EFF_FX_HEAL_ROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_WORLDCUP_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );

	for( i = 0 ; i < EFF_FX_WORLDCUP_NUM ; i ++ )
	{
		sprintf( szTexName, "football0%d.tga", i + 1 );
		g_EffectTextr[ EFF_FX_WORLDCUP_FIRE + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	g_EffectTextr[ EFF_FX_GUARDTOWER_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );
	g_EffectTextr[ EFF_FX_QUARDTOWER_HEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "guardtower.tga" );
	g_EffectTextr[ EFF_FX_SHILONMASTER_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );
	g_EffectTextr[ EFF_FX_SHILONMASTER_HEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "biggogo.tga" );

	for( i = 0 ; i < EFF_FX_LIGHTBALLBOOM_NUM ; i ++ )
	{
		sprintf( szTexName, "effect0%d.tga", i + 1 );
		g_EffectTextr[ EFF_FX_LIGHTBALLBOOM_LIGHT + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	g_EffectTextr[ EFF_FX_CONTROLLIGHT_GAUGE ] = g_pCapsyongTexture->CreateTextureFromFile( "m_controler.tga" );
	g_EffectTextr[ EFF_FX_CATCHFLAG_FLAGA ] = g_pCapsyongTexture->CreateTextureFromFile( "red_a_head.tga" );
	g_EffectTextr[ EFF_FX_CATCHFLAG_FLAGB ] = g_pCapsyongTexture->CreateTextureFromFile( "blue_a_head.tga" );
	g_EffectTextr[ EFF_FX_GREENSMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "stone_smoke.tga" );
	g_EffectTextr[ EFF_FX_CONDITION_STONE ] = g_pCapsyongTexture->CreateTextureFromFile( "stony.tga" );
	g_EffectTextr[ EFF_FX_CONDITION_MAVI ] = g_pCapsyongTexture->CreateTextureFromFile( "mavi.tga" );
	g_EffectTextr[ EFF_FX_CONDITION_CHAOS ] = g_pCapsyongTexture->CreateTextureFromFile( "chaos.tga" );
	g_EffectTextr[ EFF_FX_CONDITION_COLD ] = g_pCapsyongTexture->CreateTextureFromFile( "cold.tga" );
	g_EffectTextr[ EFF_FX_CONDITION_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_OPTIONSKILL_DEFENSE ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem_sheild.tga" );
	g_EffectTextr[ EFF_FX_OPTIONSKILL_OFFENSE ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem_sword.tga" );
	g_EffectTextr[ EFF_FX_OPTIONSKILL_PEACE ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem_peace.tga" );
	g_EffectTextr[ EFF_FX_OPTIONSKILL_CARE ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem_both.tga" );
	g_EffectTextr[ EFF_FX_OPTIONSKILL_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_HITPOWER_HIT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_HITPOWER_HITBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "eff_side.tga" );
	g_EffectTextr[ EFF_FX_HITPOWER_HITCROSS ] = g_pCapsyongTexture->CreateTextureFromFile( "magiclaser.tga" );
	g_EffectTextr[ EFF_FX_EARTHQUAKE_BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "earth.tga" );
	g_EffectTextr[ EFF_FX_EARTHQUAKE_ENG ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_EARTHQUAKE_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_FROST ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem_ice.tga" );
	g_EffectTextr[ EFF_FX_BLACKHOLE ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );

	for( i = 0 ; i < EFF_FX_BLACKHOLE_EARTH_NUM ; i ++ )
	{
		sprintf( szTexName, "blackhole0%d.tga", i + 1 );
		g_EffectTextr[ EFF_FX_BLACKHOLE_EARTH + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	g_EffectTextr[ EFF_FX_SCROLLRING ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem2.tga" );
	g_EffectTextr[ EFF_FX_SCROLLDAMAGE ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem1.tga" );
	g_EffectTextr[ EFF_FX_SKILLLIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "eff_side.tga" );
	g_EffectTextr[ EFF_FX_SCROLLLIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "scroll.tga" );
	G_EFFIMAGELOAD(EFF_FX_CHAIN, "lightning_c.bmp" );       //-- 체인 라이트닝
	G_EFFIMAGELOAD(EFF_FX_WAVE_OF_DEATH,"lightning_d.bmp"); //-- 죽음의 물결 완드

	// 매트릭스가 도입되는 나라만 로딩되게 처리.(by 원석) ///해외뫼비우스
	if( g_bUseMatrixCountry )
	{
		G_EFFIMAGELOAD( EFF_FX_HOLLEY, "lightning_holley.bmp" );	// nate 2005-05-20 : 홀리부대
		G_EFFIMAGELOAD( EFF_FX_REGENBAR, "regenbar.tga" );		// mungmae 2005-06-23 : 리젠바(막대기)
		G_EFFIMAGELOAD( EFF_FX_REGENBAR2, "regenbar2.bmp" );	// mungmae 2005-06-23 : 리젠바(배경)
		G_EFFIMAGELOAD( EFF_FX_REGENSTART, "regenstart.tga" );		// mungmae 2005-06-23 : 리젠 스타트
	}

	G_EFFIMAGELOAD( EFF_FX_CASTINGBOX, "castingbox.tga" );		// mungmae 2005-07-13 : 헬다운 케스팅 박스
	g_EffectTextr[ EFF_FX_CHAIN_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "effect03.tga" );
	g_EffectTextr[ EFF_FX_ONOFF_ON ] = g_pCapsyongTexture->CreateTextureFromFile( "ON.tga" );
	g_EffectTextr[ EFF_FX_ONOFF_OFF ] = g_pCapsyongTexture->CreateTextureFromFile( "OFF.tga" );
	g_EffectTextr[ EFF_FX_ONOFF_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "meteor_base.tga" );
	g_EffectTextr[ EFF_FX_SCANVIEW_SCAN ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_SCANVIEW_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_CHARGECANNON ] = g_pCapsyongTexture->CreateTextureFromFile( "firearrow.tga" );
	G_EFFIMAGELOAD(EFF_FX_FLAME_TAIL ,"fireball_1.tga");
	G_EFFIMAGELOAD(EFF_FX_FLAME_BASE ,"lightningball_base.tga");
	G_EFFIMAGELOAD( EFF_FX_FIREWORK , "firework.tga" ); //-- !!firework(Lyul)
	g_EffectTextr[ EFF_FX_AUTOLASER ] = g_pCapsyongTexture->CreateTextureFromFile( "autolaser.tga" );
	g_EffectTextr[ EFF_FX_AUTOLASER_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "hit_power.tga" );
	g_EffectTextr[ EFF_FX_LIFECOMMONT ] = g_pCapsyongTexture->CreateTextureFromFile( "lifecommon.tga" );
	g_EffectTextr[ EFF_FX_LIFELIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "lifelight.tga" );
	g_EffectTextr[ EFF_FX_MAGICARROW ] = g_pCapsyongTexture->CreateTextureFromFile( "magicarrow.tga" );
	g_EffectTextr[ EFF_FX_MAGICLASER ] = g_pCapsyongTexture->CreateTextureFromFile( "magiclaser.tga" );
	g_EffectTextr[ EFF_FX_MAGICLASER_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "hit_power.tga" );
	g_EffectTextr[ EFF_FX_FIREBOOMSHORT ] = g_pCapsyongTexture->CreateTextureFromFile( "fire_burst.tga" );
	g_EffectTextr[ EFF_FX_FIREBOOMSHORT_BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_base.tga" );
	g_EffectTextr[ EFF_FX_ROCKET_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );
	g_EffectTextr[ EFF_FX_METEORITE_HEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "stone.tga" );
	g_EffectTextr[ EFF_FX_METEORITE_BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "STAR_SMOKE.tga" );
	g_EffectTextr[ EFF_FX_METEORITEBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "star_fire.tga" );
	g_EffectTextr[ EFF_FX_METEORITEBOOM_BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "earth.tga" );
	g_EffectTextr[ EFF_FX_CHAOS_RING ] = g_pCapsyongTexture->CreateTextureFromFile( "chaos_storm.tga" );
	g_EffectTextr[ EFF_FX_GATHERINGGUN ] = g_pCapsyongTexture->CreateTextureFromFile( "chaos_storm.tga" );
	g_EffectTextr[ EFF_FX_DRAGON_ICETAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "flame.tga" );
	g_EffectTextr[ EFF_FX_DRAGON_FIRETAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "dragon_fire.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARAIM_SIGN ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball1.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARAIM_WAVE ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARAIM_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "STAR_fire_org.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARBOOM_BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_base.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARBOOM_DUST ] = g_pCapsyongTexture->CreateTextureFromFile( "m_controler.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARBOOM_WAVE ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_NUCLEARBOOM_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_DEATHWAVEAIM_SIGN ] = g_pCapsyongTexture->CreateTextureFromFile( "m_controler.tga" );
	g_EffectTextr[ EFF_FX_DEATHWAVEAIM_WAVE ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_DEATHWAVEAIM_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_CHARWAVE_WAVE ] = g_pCapsyongTexture->CreateTextureFromFile( "charwave.tga" );
	g_EffectTextr[ EFF_FX_CHARWAVE_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_PHENIX_RING ] = g_pCapsyongTexture->CreateTextureFromFile( "chaos_storm.tga" );
	g_EffectTextr[ EFF_FX_HITPET_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_KHANHIT_RING ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_KHANHIT_WAVE ] = g_pCapsyongTexture->CreateTextureFromFile( "eff_side.tga" );
	g_EffectTextr[ EFF_FX_TELEPORT_SHIELD ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_TELEPORT_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_TELEPORT_LEVELUP ] = g_pCapsyongTexture->CreateTextureFromFile( "LevelUp.tga" );
	g_EffectTextr[ EFF_FX_TELEPORT_HEALROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_WEAPON_TRACE ] = g_pCapsyongTexture->CreateTextureFromFile( "trace.tga" );//-- 검의 궤적
	g_EffectTextr[ EFF_FX_WEAPON_TRACE2 ] = g_pCapsyongTexture->CreateTextureFromFile( "trace2.tga" );//-- Bleed의 궤적
	g_EffectTextr[ EFF_FX_RUNSMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_WARGIANTMULTI_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "fireball_1.tga" );
	g_EffectTextr[ EFF_FX_WARGIANTDIE_GROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem1.tga" );
	g_EffectTextr[ EFF_FX_WARGIANTDIE_GROUND2 ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem2.tga" );
	g_EffectTextr[ EFF_FX_WARGIANTDIE_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_WARGIANTDIE_ROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_MOBSHOWUP_LIFE ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem2.tga" );
	g_EffectTextr[ EFF_FX_MOBSHOWUP_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_MOBSHOWUP_LASER ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_MOBSHOWUP_ROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "chaos_storm.tga" );
	g_EffectTextr[ EFF_FX_POWERUP_WAVE ] = g_pCapsyongTexture->CreateTextureFromFile( "chaos_storm.tga" );
	g_EffectTextr[ EFF_FX_POWERUP_WAVE2 ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_POWERUP_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_GRIPHON_RING ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_GRIPHON_SHIELD ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_DRAGON_RING ] = g_pCapsyongTexture->CreateTextureFromFile( "chaos_storm.tga" );
	g_EffectTextr[ EFF_FX_DRAGON_CORONA ] = g_pCapsyongTexture->CreateTextureFromFile( "corona.tga" );
	g_EffectTextr[ EFF_FX_DRAGON_SHIELD ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_MOBPARTICLE_LIGHT ] = g_pCapsyongTexture->CreateTextureFromFile( "bottem_ice.tga" );
	g_EffectTextr[ EFF_FX_GHOSTHUNT ] = g_pCapsyongTexture->CreateTextureFromFile( "ghosthunt.tga" );
	g_EffectTextr[ EFF_FX_GHOSTRISE ] = g_pCapsyongTexture->CreateTextureFromFile( "ghosthunt.tga" );
	g_EffectTextr[ EFF_FX_SOULDUST ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_MAGICSWORD ] = g_pCapsyongTexture->CreateTextureFromFile( "magicsword.tga" );
	g_EffectTextr[ EFF_FX_SPHINX_RING ] = g_pCapsyongTexture->CreateTextureFromFile( "heal_round.tga" );
	g_EffectTextr[ EFF_FX_SPHINX_SHIELD ] = g_pCapsyongTexture->CreateTextureFromFile( "shield.tga" );
	g_EffectTextr[ EFF_FX_VAMPIRE ] = g_pCapsyongTexture->CreateTextureFromFile( "ghosthunt.tga" );
	g_EffectTextr[ EFF_FX_VAMPIREATTACK_BALL ] = g_pCapsyongTexture->CreateTextureFromFile( "ghosthunt.tga" );
	g_EffectTextr[ EFF_FX_MUSICALNOTE_LIFE ] = g_pCapsyongTexture->CreateTextureFromFile( "musicnote.tga" );
	g_EffectTextr[ EFF_FX_STONERANCE2 ] = g_pCapsyongTexture->CreateTextureFromFile( "earth.tga" );
	g_EffectTextr[ EFF_FX_DROPCANDY_BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "earth.tga" );
	g_EffectTextr[ EFF_FX_DROPCANDY_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_HELLCRY_BASE1 ] = g_pCapsyongTexture->CreateTextureFromFile( "hell_cry01.tga" );
	g_EffectTextr[ EFF_FX_HELLCRY_BASE2 ] = g_pCapsyongTexture->CreateTextureFromFile( "hell_cry02.tga" );
	g_EffectTextr[ EFF_FX_HELLCRY_BASE3 ] = g_pCapsyongTexture->CreateTextureFromFile( "hell_cry03.tga" );
	g_EffectTextr[ EFF_FX_HELLCRY_BASE4 ] = g_pCapsyongTexture->CreateTextureFromFile( "hell_cry04.tga" );
	g_EffectTextr[ EFF_FX_HELLCRY_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_HELLCRY_SWORD ] = g_pCapsyongTexture->CreateTextureFromFile( "biggogo.tga" );
	g_EffectTextr[ EFF_FX_TORNADORUIN_SMOKE ] = g_pCapsyongTexture->CreateTextureFromFile( "jk_smoke.tga" );
	g_EffectTextr[ EFF_FX_SUMPART_BOTTEM ] = g_pCapsyongTexture->CreateTextureFromFile( "fission_bottem.tga" );
	g_EffectTextr[ EFF_FX_SMOKE_TAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "SmokeTail.tga" );
	g_EffectTextr[ EFF_FX_FIRETOWER ] = g_pCapsyongTexture->CreateTextureFromFile( "firetower.tga" );
	g_EffectTextr[ EFF_FX_FIRETOWER2 ] = g_pCapsyongTexture->CreateTextureFromFile( "firetower2.tga" );
	g_EffectTextr[ EFF_FX_FIRETOWER3 ] = g_pCapsyongTexture->CreateTextureFromFile( "firetower3.tga" );
	g_EffectTextr[ EFF_FX_FIRETOWER4 ] = g_pCapsyongTexture->CreateTextureFromFile( "firetower4.tga" );
	//mungmae-리플렉션 추가
	g_EffectTextr[ EFF_FX_REFLECTION ] = g_pCapsyongTexture->CreateTextureFromFile( "reflection.tga" );
	g_EffectTextr[ EFF_FX_REFLECTION2 ] = g_pCapsyongTexture->CreateTextureFromFile( "reflection2.tga" );
	//mungmae-스톤버스트 추가
	g_EffectTextr[ EFF_FX_DSTONEBRUST ] = g_pCapsyongTexture->CreateTextureFromFile( "eff_dstonebrust.bmp" );
	//mungae-헬다운 추가
	g_EffectTextr[ EFF_FX_MASICCIRCLE ] = g_pCapsyongTexture->CreateTextureFromFile( "masiccircle.bmp" );
	g_EffectTextr[ EFF_FX_MASICCIRCLE2 ] = g_pCapsyongTexture->CreateTextureFromFile( "masiccircle2.bmp" );
	g_EffectTextr[ EFF_FX_AROUND ] = g_pCapsyongTexture->CreateTextureFromFile( "masicaround.bmp" );
	g_EffectTextr[ EFF_FX_HELLDOWN_LIGHTNING ] = g_pCapsyongTexture->CreateTextureFromFile( "helldownlightning.bmp" );
	G_EFFIMAGELOAD( EFF_FX_LIFEEXPANSION , "LIfe_Expansion.tga" );	// nate 2005-07-07 : LIfe Expansion
	G_EFFIMAGELOAD( EFF_FX_POISON, "LandHole.tga" );	// nate 2005-11-04 : 포이즌 - LoadImage
	g_EffectTextr[ EFF_FX_ADRENALINE ] = g_pCapsyongTexture->CreateTextureFromFile( "adrenaline.tga" );//mungmae 2005/11/21 : 아드레날린
	G_EFFIMAGELOAD( EFF_FX_ELECTRICSHOT, "Electricshot.tga" );	// nate 2005-11-04 : 포이즌 - LoadImage

	G_EFFIMAGELOAD( EFF_FX_USHOP_BOARD, "UShopBoard.tga" );	// 개인상점 말칸용 배경 ///개인상점

	G_EFFIMAGELOAD( EFF_FX_PVP_RING_SIDE0, "pvp/cylinder0.sek"); // 개인상점
	G_EFFIMAGELOAD( EFF_FX_PVP_RING_SIDE1, "pvp/cylinder1.sek");
	G_EFFIMAGELOAD( EFF_FX_PVP_RING_BOTTOM0, "pvp/circle0.sek");
	G_EFFIMAGELOAD( EFF_FX_PVP_RING_BOTTOM1, "pvp/circle1.sek");

	g_EffectTextr[ EFF_LAGRUSH_AREA ] = g_pCapsyongTexture->CreateTextureFromFile( "lagrush_area.bmp" );

	for( i = 0 ; i < 5 ; i ++ )
	{
		sprintf( szTexName, "casting%d.bmp", i + 1 );
		G_EFFIMAGELOAD( EFF_FX_HELLDOWN + i, szTexName );
	}
	// mungmae 2006-09-20 : 스피릿 점수
	for( i = 0 ; i < 10 ; i ++ )
	{
		sprintf( szTexName, "SpiritNum%d.tga", i );
		G_EFFIMAGELOAD( EFF_FX_SPIRITPOINT + i, szTexName );
	}
	for( i = 0 ; i < 10 ; i ++ )
	{
		sprintf( szTexName, "SpiritNum_R_%d.tga", i );
		G_EFFIMAGELOAD( EFF_FX_SPIRITPOINT_R + i, szTexName );
	}

	G_EFFIMAGELOAD( EFF_FX_SPIRITSLASH, "Slash.tga" );	// mungmae 2006-09-20 : 스피릿 점수에 슬래쉬 기호
	G_EFFIMAGELOAD( EFF_FX_SPACESLASH, "space_slash.tga" );
	G_EFFIMAGELOAD( EFF_FX_ELECTRIC_SPARK, "electric_spark.tga" );
	G_EFFIMAGELOAD( EFF_FX_FLAMESPEAR_MC, "FlameSpear_mc.tga" );	// 원석 2006-10-24 : 플레임스피어의 생성 마법진 ///플레임스피어
	G_EFFIMAGELOAD( EFF_FX_RESURRECTION_OUT, "Resurrection_Out.sek" );
	G_EFFIMAGELOAD( EFF_FX_RESURRECTION_SPHERE, "Resurrection_Sphere.sek" );
	G_EFFIMAGELOAD( EFF_FX_RESURRECTION_TOWER, "Resurrection_Tower.sek" );
	G_EFFIMAGELOAD( EFF_FX_RESURRECTION_MASIC, "Resurrection_Masic.sek" );

	for( i = 0 ; i < EFF_FX_POISON_FOG_NUM ; i ++ ) // 원석 2006-11-07 : 데카렌 보스방 독안개 이펙트. ///인던1
	{
		sprintf( szTexName, "poison_fog/poison_fog%04d.tga", i + 1 );
		G_EFFIMAGELOAD( EFF_FX_POISON_FOG + i, szTexName );
	}

	G_EFFIMAGELOAD( EFF_FX_LEGKNOCK_NAME, "legknock_name00.tga" );	// 원석 2006-11-07 : 레그녹 등장신의 레그녹 이름. ///인던1

	for( i = 0 ; i < EFF_FX_DEKAREN_CUT_IN_NUM ; i ++ )
	{
		sprintf( szTexName, "DekarenEvent%02d.tga", i );
		g_EffectTextr[ EFF_FX_DEKAREN_CUT_IN + i ] = g_pCapsyongTexture->CreateTextureFromFile( szTexName );
	}

	G_EFFIMAGELOAD( EFF_FX_FREEPVP_WINNER, "result_WIN.sek" ); ///자유대련
	G_EFFIMAGELOAD( EFF_FX_FREEPVP_LOSER, "result_LOSE.sek" ); ///자유대련

	G_EFFIMAGELOAD( EFF_FX_TUTORIAL_ARROW, "Tutorial_Arrow.tga" );	///튜토리얼 중 사용되는 빨간 화살표.
	/*
		G_EFFIMAGELOAD( EFF_FX_MOUSE_ZOOMIN1, "U_Arrow_Red.tga" );	///튜토리얼 중 사용되는 마우스 줌인 그림..
		G_EFFIMAGELOAD( EFF_FX_MOUSE_ZOOMIN2, "U_Arrow_Red.tga" );	///튜토리얼 중 사용되는 마우스 줌인 그림..
		G_EFFIMAGELOAD( EFF_FX_MOUSE_ZOOMOUT1, "U_Arrow_Red.tga" );	///튜토리얼 중 사용되는 마우스 줌아웃 그림..
		G_EFFIMAGELOAD( EFF_FX_MOUSE_ZOOMOUT2, "U_Arrow_Red.tga" );	///튜토리얼 중 사용되는 마우스 줌아웃 그림..

	*/

	// [3/26/2007 Theodoric] 태국 송크란 이벤트
	g_EffectTextr[ EFF_FX_WTRSPRINKLEHEAD ] = g_pCapsyongTexture->CreateTextureFromFile( "WTRSprinkle_ball1.tga" );
	g_EffectTextr[ EFF_FX_WTRSPRINKLETAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "WTRSprinkle_ball2.tga" );
	g_EffectTextr[ EFF_FX_WTRSPRINKLEBOOM ] = g_pCapsyongTexture->CreateTextureFromFile( "WTRSprinkle_burst.tga" );
	g_EffectTextr[ EFF_FX_WTRSPRINKLEBOOMTAIL ] = g_pCapsyongTexture->CreateTextureFromFile( "WTRSprinkle_ball2.tga" );

	G_EFFIMAGELOAD( EFF_FX_DMITRON_CRYSTAL1, "dmitron_crystal1.sek" ); // 드미트론 크리스탈
	G_EFFIMAGELOAD( EFF_FX_DMITRON_CRYSTAL2, "dmitron_crystal2.sek" ); // 드미트론 크리스탈
	G_EFFIMAGELOAD( EFF_FX_DMITRON_CRYSTAL3, "dmitron_crystal3.sek" ); // 드미트론 크리스탈

	G_EFFIMAGELOAD( EFF_FX_DMITRON_PORTAL1, "gate_eff01.sek" ); //드미트론 포탈
	G_EFFIMAGELOAD( EFF_FX_DMITRON_PORTAL2, "gate_eff02.sek" ); //드미트론 포탈
	G_EFFIMAGELOAD( EFF_FX_DMITRON_PORTAL3, "gate_eff03.sek" ); //드미트론 포탈

	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_BULKAN2_2_SHOCK, "shockwave.sek" );
	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_BULKAN2_2_MAGIC, "magic_square.sek" );
	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_HUMAN2_03_LASER, "spider_laser.sek" );

	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_BULKAN3_01_01, "eff_bul03_sctatch01.tga"); // [11/21/2007 반재승] 보스레이드 불칸보스3-02스킬 2D이미지1
	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_BULKAN3_01_02, "eff_bul03_sctatch02.tga"); // [11/21/2007 반재승] 보스레이드 불칸보스3-02스킬 2D이미지2
	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_BULKAN3_01_03, "eff_bul03_sctatch03.tga"); // [11/21/2007 반재승] 보스레이드 불칸보스3-02스킬 2D이미지3
	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_HUMAN3_02, "eff_ hollow.TGA"); // [12/13/2007 반재승] 보스레이드 휴먼보스3-02스킬 2D이미지
	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_HUMAN3_02_SHOCK, "corona.tga" );
	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_HYBRI3_01, "hb03_at01.tga"); // [1/16/2008 반재승] 보스레이드 하이브리더보스3-01 노멀 어택 2D이미지

	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_KAI3_03_SMOKE, "ap_map_ef_waterfallparticle_01.sek" );// [2/11/2008 반재승] 보스레이드 카이립톤보스3 - 3번 스킬 2D이미지

	G_EFFIMAGELOAD( EFF_FX_ELECTRICPARTICLE, "if_electricparticle.tga");
	G_EFFIMAGELOAD( EFF_FX_CLYSTALGLOWLIGHT_RED, "ap_glowlight_red.tga");
	G_EFFIMAGELOAD( EFF_FX_CLYSTALGLOWLIGHT_WINE, "ap_glowlight_pink.tga");
	G_EFFIMAGELOAD( EFF_FX_CLYSTALGLOWLIGHT_CYON, "ap_glowlight_green.tga");
	G_EFFIMAGELOAD( EFF_FX_WATERFALLPARTICLE, "ap_map_ef_waterfallparticle.tga");
	G_EFFIMAGELOAD( EFF_FX_WATERFALLPARTICLE_01, "ap_map_ef_waterfallparticle_01.tga");

	G_EFFIMAGELOAD( EFF_FX_BOSSRAID_AIDIA1_02_LIGHTIN, "ap_lightin.tga");

	for(i=0; i<8; i++)
	{
		sprintf( szTexName, "ap_stickfire%02d.tga", i+1 );
		G_EFFIMAGELOAD( EFF_FX_BOSSRAID_AIDIA1_02_STICKFIRE_00 + i, szTexName);
	}

	G_EFFIMAGELOAD( EFF_FX_FORTAL_BOSSRAID, "ap_finalboss_castle_p.tga");

	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트
	G_EFFIMAGELOAD( EFF_FX_HERO_EFFECT01, "ap_hero_eff01.tga");
	G_EFFIMAGELOAD( EFF_FX_HERO_EFFECT02, "ap_hero_eff02.tga");
	G_EFFIMAGELOAD( EFF_FX_HERO_EFFECT03, "ap_hero_eff03.tga");

	//  [4/6/2009 ppmmjj83] 일본 미라클 번개 이펙트



	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_G01, "AP_LV400_EFFG01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_G02, "AP_LV400_EFFG02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_G03, "AP_LV400_EFFG03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_B01, "AP_LV400_EFFB01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_B02, "AP_LV400_EFFB02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_B03, "AP_LV400_EFFB03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_R01, "AP_LV400_EFFR01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_R02, "AP_LV400_EFFR02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_R03, "AP_LV400_EFFR03.tga");	

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_Y01, "AP_LV550_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_Y02, "AP_LV550_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_Y03, "AP_LV550_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_P01, "AP_LV600_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_P02, "AP_LV600_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_P03, "AP_LV600_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_F01, "AP_LV650_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_F02, "AP_LV650_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_F03, "AP_LV650_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_M01, "AP_LV700_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_M02, "AP_LV700_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_M03, "AP_LV700_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_W01, "AP_LV750_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_W02, "AP_LV750_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_W03, "AP_LV750_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_BL01, "AP_LV800_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_BL02, "AP_LV800_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_BL03, "AP_LV800_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_N01, "AP_LV850_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_N02, "AP_LV850_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_N03, "AP_LV850_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_X01, "AP_LV900_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_X02, "AP_LV900_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_X03, "AP_LV900_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_Z01, "AP_LV950_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_Z02, "AP_LV950_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_Z03, "AP_LV950_EFF03.tga");

	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_PI01, "AP_LV1000_EFF01.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_PI02, "AP_LV1000_EFF02.tga");
	G_EFFIMAGELOAD( EFF_FX_LV400_EFFECT_PI03, "AP_LV1000_EFF03.tga");


}

// nate 2004 - 7 m_pTestTexture제거
void G_LoadEffectTestTexImage()
{
	g_pCapsyongTexture->SetTexturePath( "data/effect/testtex/" );
	g_EffectTextr[ EFF_SHADOWRECT ] = g_pCapsyongTexture->CreateTextureFromFile( "shadow.tga" );
	g_EffectTextr[ EFF_WEATHERMGR_RAIN ] = g_pCapsyongTexture->CreateTextureFromFile( "rain.tga" );
	g_EffectTextr[ EFF_WEATHERMGR_DROP ] = g_pCapsyongTexture->CreateTextureFromFile( "drop.tga" );
	g_EffectTextr[EFF_WEATHERMGR_SAKURA_DROP] = g_pCapsyongTexture->CreateTextureFromFile( "sakura.tga" );



	g_pCapsyongTexture->SetTexturePath( "interface/compas/" );
	g_EffectTextr[ EFF_COMPAS_BASE ] = g_pCapsyongTexture->CreateTextureFromFile( "base.tga" );
	g_EffectTextr[ EFF_COMPAS_TURN ] = g_pCapsyongTexture->CreateTextureFromFile( "turn.tga" );
}