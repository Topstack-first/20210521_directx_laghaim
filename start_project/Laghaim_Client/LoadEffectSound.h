#ifndef _LOAD_EFFECT_SOUND_H_
#define _LOAD_EFFECT_SOUND_H_

#include "Config.h"

#include "country.h"


extern int g_EffectSound[];
extern int g_EffectTextr[];
extern DWORD g_dwLangType;

#define EFF_SND_DROP		0
#define EFF_SND_L_RAIN		1
#define EFF_SND_H_RAIN		2
#define EFF_SND_L_RAIN_STOP	3
#define EFF_SND_H_RAIN_STOP	4
#define	EFF_SND_LEVELUP		5
#define EFF_SND_LIGHTNING	6
#define EFF_SND_SHOT1		7
#define EFF_SND_BOOM1		8
#define EFF_SND_SHOT2		9
#define EFF_SND_BOOM2		10
#define EFF_SND_BOOM3		11
#define EFF_SND_DROP_STONE	12
#define EFF_SND_FIRE_PILLAR	13
#define EFF_SND_STONE_LANCE	14
#define EFF_SND_STAR_FLARE	15
#define EFF_SND_SHOCK		16
#define EFF_SND_MUTE3		17
#define EFF_SND_CONGR		18
#define EFF_SND_SWORD		19
#define EFF_SND_AXE			20
#define EFF_SND_STAFF		21
#define EFF_SND_FIST		22
#define EFF_SND_EXPLO		23
#define EFF_SND_HORN		24
#define EFF_SND_BEEP1		25
#define EFF_SND_KWANG		26
#define EFF_SND_WINCHIP		27
#define EFF_SND_SILVER_FLY	28
#define EFF_SND_RAVEN		29
#define EFF_SND_KYNES		30
#define EFF_SND_NEO_SNAKE	31
#define EFF_SND_KAIZEN		32
#define EFF_SND_MANASHIELD1		33
#define EFF_SND_ELECSHIELD1		34
#define EFF_SND_SAFETYSHIELD1	35
#define EFF_SND_BEE			36
#define EFF_SND_BUTTERFLY	37
#define EFF_SND_BORA		38
#define EFF_SND_MOTH		39
#define EFF_SND_BOOM4		40
#define EFF_SND_LIGHTSWORD	41
#define EFF_SND_HAPPYNEWYEAR	42
#define EFF_SND_DROPCANDY	43
//중고랩스킬 추가 사운드
#define EFF_SND_BOMBER		44
#define EFF_SND_BOMBDRP		45
#define EFF_SND_MISSLEBOMB	46
#define EFF_SND_TONADO		47
#define EFF_SND_FISSION		48
#define EFF_SND_HELL		49
//-- 휴먼 총별 사운드 추가.
#define EFF_SND_GUN_1		50
#define EFF_SND_GUN_2		51
#define EFF_SND_GUN_3		52
#define EFF_SND_GUN_4		53
#define EFF_SND_GUN_5		54
#define EFF_SND_GUN_6		55
#define EFF_SND_GUN_7		56
//-- 불꽃 사운드 추가.
#define EFF_SND_FIREWORKS_4 57
#define EFF_SND_FIREWORKS_RAINBOW	58
#define EFF_SND_FIREWORKS_SCREW		59
#define EFF_SND_FIREWORKS_FOUNTAIN	60
//-- 하리센 사운드 추가.
#define EFF_SND_HARISEN		61
#define EFF_SND_M_ASSAULT1	62
#define EFF_SND_M_ASSAULT2	63
#define EFF_SND_W_ASSAULT1	64
#define EFF_SND_W_ASSAULT2	65
#define	EFF_SND_COUNT		66
#define	EFF_SND_WIN			67
#define	EFF_SND_LOSE		68
#define EFF_SND_BATTLESTART	69
// nate 2005-04-15
#define	EFF_SND_MINI		70	// 메트릭스 위치지정
#define EFF_SND_ENTRANCE	71	//지휘자 등장 사운드
// nate 2005-07-21 : Burning Crash - Sound
#define	EFF_SND_BURNING_1		72	// 1번 동작
#define	EFF_SND_BURNING_2		73	// 2번 동작
#define	EFF_SND_BURNING_3_M		74	// 3번 동작 남자
#define	EFF_SND_BURNING_4		75	// 4번 동작
#define	EFF_SND_BURNING_5		76	// 5번 동작
#define	EFF_SND_BURNING_6_M		77	// 6번 동작 남자
#define	EFF_SND_BURNING_3_W		78	// 3번 동작 여자
#define	EFF_SND_BURNING_6_W		79	// 6번 동작 여자
#define EFF_SND_STONE_BREAK		80	// 돌 튀기는 소리
#define EFF_SND_HELLDOWN_CAST_M 81	// 헬다운 시전 남자
#define EFF_SND_HELLDOWN_CAST_W 82	// 헬다운 시전 남자
#define EFF_SND_HELLDOWN_FINISH_M 83	// 헬다운 피니쉬 남자
#define EFF_SND_HELLDOWN_FINISH_W 84	// 헬다운 피니쉬 여자
#define EFF_SND_PIKOPIKO		85 ///반짝반짝망치 추가
#define EFF_SND_BLOCK			86  //블럭 효과음 ///페어리
#define EFF_SND_IRON_SKIN		87  //강철의 몸
#define EFF_SND_FROZEN_SHOT		88  //프로즌 샷
#define EFF_SND_RANDOM_SHOT		89  //랜덤 샷
#define EFF_SND_TONADO_HAMMER	90  //토네이도 해머 

enum EFFSOUND { EFF_SND_REVIVAL = 91,

				EFF_SND_GUN_8,				// 브리오낙, 신의브리오낙 신사운드 추가

				EFF_SND_LOCKSMITH,		    // [일본] : 주사위 소리
				
				EFF_SND_ROLLET_ONEMORE,
				EFF_SND_ROLLET_BAD,
				EFF_SND_ROLLET_DOWN,
				EFF_SND_ROLLET_GOOD,
				EFF_SND_ROLLET_ROLL,

				EFF_SND_NUM					// Effect Sound Total Count
			  };

//----------------------------------------------------------------------------
//-- Gloabal Font
//----------------------------------------------------------------------------
//-- 모든 소스에서 사용될 Font 를 Global로
//-- void LoadEffectSound(); 에서 호출..켁.. 사운드 아닌데 ㅡ,.ㅡ

//-- 2004.03.15 - 수정완료
//-- Lyul

//-- 0000.00.00 - ??????
//-- name / description
//-- Global Font Valiable


#define G_FONT_NUM 10                //--  임시지정, 폰트의 개수
//-- 굴림체
#define G_FONT_80   0        //--     h:8  w:0
#define G_FONT_90   1        //--     h:9  w:0
#define G_FONT_100  2        //--     h:10  w:0
#define G_FONT_120  3        //--     h:12  w:0
#define G_FONT_124  4        //--     h:12  w:4
#define G_FONT_130  5        //--     h:13  w:0
#define G_FONT_140  6        //--     h:14  w:0
#define G_FONT_150  7        //--     h:15  w:0
#define G_FONT_160  8        //--    h:16  w:0
#define G_FONT_200  9        //--    h:20  w:0


//-- Global Font 를 return 한다.
HFONT G_GetFont(int h, int w,const char  * const fontname);

//-- Global Font 를 Load 한다. Winmain 에서
void Destroy_Global_Font();

//-- Global Font 를 해제 한다. WM_CLOSE 호출시
void Load_Global_Font();

//-- 속지 말자 Sound 뿐 아니라 texture 도 Load 한다.
void LoadEffectSound();

#endif // _LOAD_EFFECT_SOUND_H_
