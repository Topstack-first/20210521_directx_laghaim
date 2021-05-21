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
//�߰���ų �߰� ����
#define EFF_SND_BOMBER		44
#define EFF_SND_BOMBDRP		45
#define EFF_SND_MISSLEBOMB	46
#define EFF_SND_TONADO		47
#define EFF_SND_FISSION		48
#define EFF_SND_HELL		49
//-- �޸� �Ѻ� ���� �߰�.
#define EFF_SND_GUN_1		50
#define EFF_SND_GUN_2		51
#define EFF_SND_GUN_3		52
#define EFF_SND_GUN_4		53
#define EFF_SND_GUN_5		54
#define EFF_SND_GUN_6		55
#define EFF_SND_GUN_7		56
//-- �Ҳ� ���� �߰�.
#define EFF_SND_FIREWORKS_4 57
#define EFF_SND_FIREWORKS_RAINBOW	58
#define EFF_SND_FIREWORKS_SCREW		59
#define EFF_SND_FIREWORKS_FOUNTAIN	60
//-- �ϸ��� ���� �߰�.
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
#define	EFF_SND_MINI		70	// ��Ʈ���� ��ġ����
#define EFF_SND_ENTRANCE	71	//������ ���� ����
// nate 2005-07-21 : Burning Crash - Sound
#define	EFF_SND_BURNING_1		72	// 1�� ����
#define	EFF_SND_BURNING_2		73	// 2�� ����
#define	EFF_SND_BURNING_3_M		74	// 3�� ���� ����
#define	EFF_SND_BURNING_4		75	// 4�� ����
#define	EFF_SND_BURNING_5		76	// 5�� ����
#define	EFF_SND_BURNING_6_M		77	// 6�� ���� ����
#define	EFF_SND_BURNING_3_W		78	// 3�� ���� ����
#define	EFF_SND_BURNING_6_W		79	// 6�� ���� ����
#define EFF_SND_STONE_BREAK		80	// �� Ƣ��� �Ҹ�
#define EFF_SND_HELLDOWN_CAST_M 81	// ��ٿ� ���� ����
#define EFF_SND_HELLDOWN_CAST_W 82	// ��ٿ� ���� ����
#define EFF_SND_HELLDOWN_FINISH_M 83	// ��ٿ� �ǴϽ� ����
#define EFF_SND_HELLDOWN_FINISH_W 84	// ��ٿ� �ǴϽ� ����
#define EFF_SND_PIKOPIKO		85 ///��¦��¦��ġ �߰�
#define EFF_SND_BLOCK			86  //�� ȿ���� ///��
#define EFF_SND_IRON_SKIN		87  //��ö�� ��
#define EFF_SND_FROZEN_SHOT		88  //������ ��
#define EFF_SND_RANDOM_SHOT		89  //���� ��
#define EFF_SND_TONADO_HAMMER	90  //����̵� �ظ� 

enum EFFSOUND { EFF_SND_REVIVAL = 91,

				EFF_SND_GUN_8,				// �긮����, ���Ǻ긮���� �Ż��� �߰�

				EFF_SND_LOCKSMITH,		    // [�Ϻ�] : �ֻ��� �Ҹ�
				
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
//-- ��� �ҽ����� ���� Font �� Global��
//-- void LoadEffectSound(); ���� ȣ��..��.. ���� �ƴѵ� ��,.��

//-- 2004.03.15 - �����Ϸ�
//-- Lyul

//-- 0000.00.00 - ??????
//-- name / description
//-- Global Font Valiable


#define G_FONT_NUM 10                //--  �ӽ�����, ��Ʈ�� ����
//-- ����ü
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


//-- Global Font �� return �Ѵ�.
HFONT G_GetFont(int h, int w,const char  * const fontname);

//-- Global Font �� Load �Ѵ�. Winmain ����
void Destroy_Global_Font();

//-- Global Font �� ���� �Ѵ�. WM_CLOSE ȣ���
void Load_Global_Font();

//-- ���� ���� Sound �� �ƴ϶� texture �� Load �Ѵ�.
void LoadEffectSound();

#endif // _LOAD_EFFECT_SOUND_H_
