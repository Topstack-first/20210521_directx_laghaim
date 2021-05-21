#ifndef _FX_SET_H_
#define _FX_SET_H_

#include "Config.h"

#include <vector>
#include <map>
using namespace std;

#define FX_SMALL_LIGHT		0
#define FX_PORTAL			1
#define FX_SMOKE			2
#define FX_SHADOWRING		3
#define FX_SHADOWLIGHT		4
#define FX_PARTICLE			5
#define FX_LIGHTFOG			6
#define FX_SHOWUP			7
#define FX_LIGHTNING		8
#define FX_LIGHTBALL		9
#define FX_FIREBALL			10
#define FX_FIREBALLBOOM		11
#define FX_LWLIGHT			12
#define FX_LIGHTBALLBOOM	13
#define FX_UNARMEDATTACKED	14
#define FX_METEOR			15
#define FX_PEBBLESHOT		16
#define FX_STONESHOT		17
#define FX_FSHOWER			18
#define FX_STONESHOTBOOM	19
#define FX_PEBBLESHOTBOOM	20
#define FX_SHOOT			21
#define FX_SHOOTATTACKED	22
#define FX_AIDIAFIRE		23
#define FX_AIDIABALLBOOM	24
#define FX_LIGHTHEAL		25
#define FX_STARFLARE		26
#define FX_STARFLAREBOOM	27
#define FX_SHOCKSPEAR		28
#define FX_STONERANCE		29
#define FX_REDFIRE			30
#define FX_FLY				31
#define FX_BEE				32
#define FX_BEEBOOM			33
#define FX_BORA				34
#define FX_BORABOOM			35
#define FX_MOTH				36
#define FX_SHILON			37
#define FX_HORN				38
#define FX_SILVERFLY		39
#define FX_RAVEN			40
#define FX_KINES			41
#define FX_NEO				42
#define FX_KAISEN			43
#define FX_HITEFFECTS		44
#define FX_HITBOOMEFFECTS	45
#define FX_LEVELUP			46
#define FX_SHOWUP2			47
#define FX_EATEFFECTS		48
#define FX_SHIELD			49
#define FX_SMOKEPARTICLE	50
#define FX_FIREPARTICLE		51
#define FX_HORNBOOM			52
#define FX_HEALEFFECTS		53
#define FX_WORLDCUP			54
#define FX_WORLDCUPBOOM		55
#define FX_FIREBALLBOOM2	56
#define FX_GUARDTOWER		57
#define FX_SHILONMASTER		58
#define FX_LIGHTBALLBOOMNEW	59
#define FX_CONTROL_LIGHT	60
#define FX_CATCHFLAG		61
#define FX_GREENSMOKE		62
#define FX_CONDITIONEFFECTS	63
#define FX_OPTIONSKILLEFFECTS 64
#define FX_HITPOWEREFFECTS	65
#define FX_EARTHQUAKE		66
#define FX_FROST			67
#define FX_CHAKRAM			68
#define FX_BLACKHOLE		69
#define FX_SCROLLRING		70
#define FX_SKILL_LIGHT		71
#define FX_SCROLL_LIGHT		72
#define FX_SCROLLDAMAGE		73
#define FX_WAND_CHAIN		74
#define FX_ONOFFEFFECTS		75
#define FX_SCANVIEW			76
#define FX_CHARGECANNON		77
#define FX_FLAME			78
#define FX_AUTOLASER		79
#define FX_LIFECOMMON		80
#define FX_LIFELIGHT		81
#define FX_MAGICARROW		82
#define FX_MAGICLASER		83
#define FX_FIREBOOMSHORT	84
#define FX_ROCKET			85
#define FX_METEORITE		86
#define FX_METEORITEBOOM	87
#define FX_CHAOS			88
#define FX_GATHERINGGUN		89
#define FX_DRAGONEFFECTS	90
#define FX_NUCLEARAIM		91
#define FX_NUCLEARBOOM		92
#define FX_DEATHWAVEAIM		93
#define FX_CHARWAVE			94
#define FX_PHENIX			95
#define FX_SPRAYPARTICLE	96
#define FX_TURNPARTICLE		97
#define FX_LIGHTPARTICLE	98
#define FX_HITPETEFFECTS	99
#define FX_KHANHITEFFECTS	100
#define FX_TELEPORT			101
#define FX_WEAPONEFFECTS	102
#define FX_RUNSMOKE			103
#define FX_WARGIANTMULTI	104
#define FX_WARGIANTATTACK	105
#define FX_WARGIANTDIE		106
#define FX_MOBSHOWUP		107
#define FX_POWERUP 			108
#define FX_GRYPHON 			109
#define FX_DRAGON 			110
#define FX_MOBPARTICLE 		111
#define FX_GHOSTHUNT 		112
#define FX_GHOSTRISE 		113
#define FX_SOULDUST 		114
#define FX_MAGICSWORD 		115
#define FX_SPHINX			116
#define FX_VAMPIRE			117
#define FX_VAMPIREATTACK	118
#define FX_MUSICALNOTE		119
#define FX_FALLOFFSTONE		120
#define FX_STONERANCE2		121
#define FX_DRILLSTONE		122
#define FX_CATCHFLAGDIR		123
#define FX_DROPCANDY		124 //-- Drop Candy
#define FX_DROPAPPLE		125 //-- Drop Apple
#define FX_HELLCRY			126
#define FX_AIRRAID			127
#define FX_TORNADORUIN		128
#define FX_SUMMONPARTITION	129
#define FX_DEVILRING		130
#define FX_FIREWORKS		131 //-- FireWork!!(Lyul)
#define FX_DSTONE			132
#define FX_DSLASH			133
#define FX_BASH				134
#define FX_DSTONEEFF		135
#define FX_MOB_SLAYER		136	//wan:2004-12 //몹이 쓰는 슬레이어 이펙트
#define FX_BLEED			137
#define FX_ASSAULT			138
#define FX_DEMICRUSHER		139
#define FX_FIRETOWER		140
#define FX_DSTONEBRUST		141 //mungmae-2005/04/29
#define FX_AUTOMINE			142	// nate 2005-06-15 : Auro Mine
#define FX_HELLDOWN			143 //mungmae-2005/07/04 : 헬 다운 마우스 누른 상테에서의 이펙트
#define FX_HELLDOWN2		144 //mungmae-2005/07/04 : 헬 다운 마우스 띤 상테에서의 이펙트
#define FX_LIFEEXPENSION	145	// nate 2005-07-05 : 에이디아 스킬 - Life Expension
#define FX_SUMMONSQUAD		146 // nate 2005-07-08 : 에이디아 스킬 - Summon Squad
#define FX_REFLECTION		147 // mungmae 2005/07/28: 리플렉션
#define FX_DURANT_BEAM		148 // nate 2005-09-26 : 듀란트 - 광선
#define FX_DURANT_ATTACK	149	// nate 2005-09-26 : 듀란트 - 어택
#define FX_DURANT_TARGET	150 // nate 2005-09-26 : 듀란트 - 공격 대상
#define FX_POISON			151 // nate 2005-11-03 : 포이즌
#define FX_ADRENALINE		152 // mungmae 2005/11/21 : 아드레날린
#define FX_ATTACK_MOVELOLO	153 // nate 2005-11-22 : 이펙트를 통합할 목적의 fx
#define FX_FLYINGCRASH		154 // mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
#define FX_ATTACK_FIXLOLO	155 // nate 2005-11-22 : 이펙트를 통합할 목적의 fx
#define FX_FLYINGCRASH2		156 // mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
#define FX_SHADOWSLASH		157	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
#define FX_SHADOWSLASH2		158	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
#define FX_TLVERTEX			159	// nate 2006-01-17 : 메듀사
#define FX_FOLLOW           160 // 무언가를 따라다니는 종류의 이펙트. AI를 가질 수도 있다. ///페어리
#define FX_SUMMONRING		161 // 원석 2006-05-19 : 기존 로드된 에이디아 링 이펙트중 기존 로드된 그래픽을 이용하여 찍는 녀석. ///불칸링
#define FX_SPEEDUPSMOKE		162	// mungmae 2006/06/08 : 월드컵 스피드업 연기 이펙트
#define FX_PETSKILL         163 // 진화형 펫의 스킬 이펙트
#define FX_SPIRITMODE       164 //스피릿(발동시 효과) 
#define FX_SPIRITMODE2      165 //스피릿(지속적인 효과)
#define FX_SPACESLASH		166 // 스페이스 슬래쉬
#define FX_ELECTRICHAMMER	167 // 일렉트로닉 해머
#define FX_SUMMONFIRE		168 // 서먼 파이어
#define FX_FLAMESPEAR       169 ///플레임스피어
#define FX_SUMMONFIREBOOM	170 // 서먼 파이어때 터지는 효과
#define FX_RANDOMFIRE		171 // 랜덤 파이어
#define FX_RANDOMSHOOT		172 // 랜덤 파이어 총구 이펙트
#define FX_MERCERNERYSLASH	173 // 머셔리스 슬래쉬
#define FX_ICEBLOT			174 // 아이스 볼트
#define FX_PROTECTIONSHIELD	175 // 프로텍션 쉴드
#define FX_WILDSMASH		176 // 와일드 스매쉬
#define FX_RESURRECTION		177 // 리져렉션
#define FX_TORNADO_HAMMER	178 // 토네이도 해머
#define FX_POISON_FOG		179 // 화면에 뿌려지는 독안개 ///인던1
#define FX_SHIELDBLOCK		180 // 프로텍션 쉴드 블럭 이펙트
#define FX_TYPE_PARTICLE	181 // 원석 2007-01-18 : 타입별 파티클 이펙트 ///하트로드
#define FX_LIP_PARTICLE		182 // 2007.03.14 화이트
#define FX_LOVE_PARTICLE	183
#define FX_WTRSPRINKLE		184 // [3/26/2007 Theodoric] 태국 송크란 이벤트 효과 
#define FX_WTRSPRINKLEBOOM	185 // [3/26/2007 Theodoric] 태국 송크란 이벤트 효과
#define FX_BABYSOUL			186 // [8/7/2007 Theodoric] 6주년 기념 이벤트 
#define FX_MURAMASA_GHOST	187 ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.

#define FX_DMITRON_PORTAL	188 // 드미트론 점령전 - 이동 게이트
#define FX_DMITRON_CRYSTAL	189 // 드미트론 점령전 - 크리스탈

#define FX_BOSSRAID_BULKAN1_02		190
#define FX_BOSSRAID_BULKAN1_03		191
#define FX_BOSSRAID_BULKAN2_02		192
#define FX_BOSSRAID_BULKAN3_01		193
#define FX_BOSSRAID_BULKAN3_02		194

#define FX_BOSSRAID_HUMAN1_02		195
#define FX_BOSSRAID_HUMAN1_03		196
#define FX_BOSSRAID_HUMAN2_02		197
#define FX_BOSSRAID_HUMAN2_03		198
#define FX_BOSSRAID_HUMAN3_02		199

#define FX_BOSSRAID_AIDIA1_01		200
#define FX_BOSSRAID_AIDIA1_02		201
#define FX_BOSSRAID_AIDIA2_01		202
#define FX_BOSSRAID_AIDIA2_02		203
#define FX_BOSSRAID_AIDIA2_03		204
#define FX_BOSSRAID_AIDIA3_02		205

#define FX_BOSSRAID_HYBRI1_01		206
#define FX_BOSSRAID_HYBRI1_02		207
#define FX_BOSSRAID_HYBRI2_02		208
#define FX_BOSSRAID_HYBRI3_01		209
#define FX_BOSSRAID_HYBRI3_02		210

#define FX_BOSSRAID_KAI1_01			211
#define FX_BOSSRAID_KAI1_02			212
#define FX_BOSSRAID_KAI2_01			213	// [2/5/2008 반재승] 카이립톤보스2 1 어택
#define FX_BOSSRAID_KAI2_02			214	// [2/5/2008 반재승] 카이립톤보스2 2 어택
#define FX_BOSSRAID_KAI2_03			215 // [2/5/2008 반재승] 카이립톤보스2 3 어택
#define FX_BOSSRAID_KAI3_01			216	// [2/4/2008 반재승] 카이립톤보스3 노멀 어택
#define FX_BOSSRAID_KAI3_02			217
#define FX_BOSSRAID_KAI3_03			218

#define FX_BOSSRAID_LASTBOSS_02		219
#define FX_BOSSRAID_LASTBOSS_03		220

#define FX_ELECTRICPARTICLE			221
#define FX_CONDITIONEFFECTS2		222
#define FX_CLYSTALGLOW				223
#define FX_WATERFALL				224
#define FX_WATERFALLPARTICLE		225
#define FX_LIGHTNINGEX				226
#define FX_BOSSRAID_AIDIA1_02_SHOCKWAVE		227
#define FX_BOSSRAID_AIDIA3_01		228	// [3/6/2008 반재승] 에이디아보스3 노멀 어택
#define FX_BOSSRAID_AIDIA3_02_STONE	229	// [3/6/2008 반재승] 에이디아보스3 2번 스킬
#define FX_PORTAL_BOSSRAID			230 // 보스레이드 마지막 보스 이펙트
#define FX_FIRESMOKE_PARTICLE		231
#define FX_BOSSRAID_LASTBOSS_04     232

enum FXNUM
{
	FX_DROPSTARCANDY = 233,			// 별사탕 이벤트
	FX_HERO_EFFECT,

	FX_SARNGA_MOUEL,				// 사릉가 이펙트
	FX_YETI_RING,					// AIDIA LEVEL 450
	FX_PEAKOCK_RING,				// AIDIA LEVEL 500
	FX_TIGER_RING,					// AIDIA LEVEL 550
	FX_UNICORN_RING,				// AIDIA LEVEL 600
	FX_ARUPAKA_SPIT,
	FX_650_RING,				// AIDIA LEVEL 650
	FX_700_RING,				// AIDIA LEVEL 700
	FX_750_RING,				// AIDIA LEVEL 750
	FX_800_RING,				// AIDIA LEVEL 800
	FX_850_RING,				// AIDIA LEVEL 850
	FX_900_RING,				// AIDIA LEVEL 900
	FX_950_RING,				// AIDIA LEVEL 950
	FX_1000_RING,				// AIDIA LEVEL 1000
	FX_CHR_RING,           // AIDIA christmas
	FX_CHIN_RING,			// CHINESE NEW YEARS

	FX_LV400_EFFECT_GREEN,
	FX_LV450_EFFECT_BLUE,
	FX_LV500_EFFECT_RED,
	FX_LV550_EFFECT_YELLOW,
	FX_LV600_EFFECT_PURPLE,
	FX_LV650_EFFECT_RAINBOW1,
	FX_LV700_EFFECT_RAINBOW2,
	FX_LV750_EFFECT_WHITE,
	FX_LV800_EFFECT_BLACK,
	FX_LV850_EFFECT_ORANGE,
	FX_LV900_EFFECT_RAINBOW3,
	FX_LV950_EFFECT_RAINBOW4,
	FX_LV1000_EFFECT_PINK,

	FX_PEROM_SKILL_S125,
	FX_PEROM_SKILL_S126,
	FX_PEROM_SKILL_S127,
	FX_PEROM_SKILL_S128,
	FX_PEROM_SKILL_S129,
	FX_PEROM_SKILL_S130,
	FX_PEROM_SKILL_S131,
	FX_PEROM_SKILL_S132,
	FX_PEROM_SKILL_S133,
	FX_PEROM_SKILL_S134,
	FX_PEROM_SKILL_S135,
	FX_PEROM_SKILL_S136,
	FX_PEROM_SKILL_S137,
	FX_PEROM_SKILL_S138,
	FX_PEROM_SKILL_S139,
	FX_PEROM_SKILL_S140,
	FX_PEROM_SKILL_S141,
	FX_PEROM_SKILL_S142,
	FX_PEROM_SKILL_S143,
	FX_PEROM_SKILL_S144,
	FX_PEROM_SKILL_S145,
	FX_PEROM_SKILL_S146,
	FX_PEROM_SKILL_S147,
	FX_PEROM_SKILL_S148,
	FX_PEROM_SKILL_S149,
	FX_PEROM_SKILL_S150,
	FX_GRIFFON_RING,

	FX_ZORO_Z,
	FX_VALO_V,

	FX_BALLOON_KILL,
	FX_BOSS_ATTACK_01,
	FX_BOSS_ATTACK_02,
	FX_BOSS_DEBUFF,
	FX_BOSS_MAGIC,

	FX_DROPAPPLEPVP,
	FX_DROPCANDYPVP,
	FX_DROPCANDYVIP,

	FX_NUM							// Max 이펙트 넘버
};


//============================================================================
// 각 효과의 프레임 카운트.
#define FX_HELLCRY_FRAMECOUNT			40
#define FX_AIRRAID_FRAMECOUNT			40
#define FX_TORNADORUIN_FRAMECOUNT		60
#define FX_SUMMONPARTITION_FRAMECOUNT	40
#define FX_SHOWUP_FRAMECOUNT			12
#define FX_LIGHTNING_FRAMECOUNT			15
#define FX_LIGHTNINGEX_FRAMECOUNT		15
#define FX_LIGHTBALL_FRAMECOUNT			100
#define FX_FIREBALL_FRAMECOUNT			50
#define FX_FIREBALLBOOM_FRAMECOUNT		20
#define FX_FIREBALLBOOM2_FRAMECOUNT		20
#define FX_LWLIGHT_FRAMECOUNT			7
#define FX_LIGHTBALLBOOM_FRAMECOUNT		10
#define FX_UNARMEDATTACKED_FRAMECOUNT	4
#define FX_METEOR_FRAMECOUNT			15
#define FX_PEBBLESHOT_FRAMECOUNT		60
#define FX_STONESHOT_FRAMECOUNT			60
#define FX_FSHOWER_FRAMECOUNT			12
#define FX_STONESHOTBOOM_FRAMECOUNT		20
#define FX_PEBBLESHOTBOOM_FRAMECOUNT	20
#define FX_SHOOT_FRAMECOUNT				3
#define FX_SHOOTATTACKED_FRAMECOUNT		4
#define FX_AIDIAFIRE_FRAMECOUNT			60
#define FX_AIDIABALLBOOM_FRAMECOUNT		20
#define FX_LIGHTHEAL_FRAMECOUNT			20
#define FX_STARFLARE_FRAMECOUNT			60
#define FX_STARFLAREBOOM_FRAMECOUNT		20
#define FX_SHOCKSPEAR_FRAMECOUNT		20
#define FX_STONERANCE_FRAMECOUNT		20
#define FX_REDFIRE_FRAMECOUNT			20
#define FX_FLY_FRAMECOUNT				40
#define FX_BEE_FRAMECOUNT				40
#define FX_BEEBOOM_FRAMECOUNT			20
#define FX_BORA_FRAMECOUNT				40
#define FX_BORABOOM_FRAMECOUNT			20
#define FX_MOTH_FRAMECOUNT				40
#define FX_SHILON_FRAMECOUNT			16
#define FX_HORN_FRAMECOUNT				20
#define FX_SILVERFLY_FRAMECOUNT			14
#define FX_RAVEN_FRAMECOUNT				20
#define FX_KINES_FRAMECOUNT				36
#define FX_NEO_FRAMECOUNT				20
#define FX_KAISEN_FRAMECOUNT			14
#define FX_HITEFFECTS_FRAMECOUNT		6
#define FX_HITBOOMEFFECTS_FRAMECOUNT	6
#define FX_LEVELUP_FRAMECOUNT			30
#define FX_SHOWUP2_FRAMECOUNT			15
#define FX_EATEFFECTS_FRAMECOUNT		30
#define FX_SHIELD_FRAMECOUNT			2400
#define FX_SMOKEPARTICLE_FRAMECOUNT		1
#define FX_FIREPARTICLE_FRAMECOUNT		1
#define FX_HORNBOOM_FRAMECOUNT			6
#define FX_HEALEFFECTS_FRAMECOUNT		20
#define FX_WORLDCUP_FRAMECOUNT			50
#define FX_WORLDCUPBOOM_FRAMECOUNT		6
#define FX_GUARDTOWER_FRAMECOUNT		50
#define FX_SHILONMASTER_FRAMECOUNT		50
#define FX_LIGHTBALLBOOMNEW_FRAMECOUNT	10
#define FX_CONTROL_LIGHT_FRAMECOUNT		2400
#define FX_CATCHFLAG_FRAMECOUNT			2400
#define FX_GREENSMOKE_FRAMECOUNT		60
#define FX_CONDITIONEFFECTS_FRAMECOUNT	2400
#define FX_CONDITIONEFFECTS2_FRAMECOUNT	2400
#define FX_OPTIONSKILLEFFECTS_FRAMECOUNT 2400
#define FX_HITPOWEREFFECTS_FRAMECOUNT	6
#define FX_EARTHQUAKE_FRAMECOUNT		15
#define FX_FROST_FRAMECOUNT				20
#define FX_CHAKRAM_FRAMECOUNT			100
#define FX_BLACKHOLE_FRAMECOUNT			30
#define FX_SCROLLRING_FRAMECOUNT		15
#define FX_SCROLLDAMAGE_FRAMECOUNT		15
#define FX_SKILL_LIGHT_FRAMECOUNT		15
#define FX_SCROLL_LIGHT_FRAMECOUNT		15
#define FX_WAND_CHAIN_FRAMECOUNT		20
#define FX_ONOFFEFFECTS_FRAMECOUNT		20
#define FX_SCANVIEW_FRAMECOUNT			20
#define FX_CHARGECANNON_FRAMECOUNT		60
#define FX_AUTOLASER_FRAMECOUNT			6
#define FX_FLAME_FRAMECOUNT				20
#define FX_LIFECOMMON_FRAMECOUNT		2400
#define FX_LIFELIGHT_FRAMECOUNT			2400
#define FX_MAGICARROW_FRAMECOUNT		60
#define FX_MAGICLASER_FRAMECOUNT		6
#define FX_FIREBOOMSHORT_FRAMECOUNT		13
#define FX_ROCKET_FRAMECOUNT			50
#define FX_METEORITE_FRAMECOUNT			60
#define FX_METEORITEBOOM_FRAMECOUNT		20
#define FX_CHAOS_FRAMECOUNT				25
#define FX_GATHERINGGUN_FRAMECOUNT		6
#define FX_DRAGONEFFECTS_FRAMECOUNT		16
#define FX_NUCLEARAIM_FRAMECOUNT		2400
#define FX_NUCLEARBOOM_FRAMECOUNT		20
#define FX_DEATHWAVEAIM_FRAMECOUNT		2400
#define FX_CHARWAVE_FRAMECOUNT			999999
#define FX_PHENIX_FRAMECOUNT			20
#define FX_SPRAYPARTICLE_FRAMECOUNT		1
#define FX_TURNPARTICLE_FRAMECOUNT		1
#define FX_LIGHTPARTICLE_FRAMECOUNT		1
#define FX_HITPETEFFECTS_FRAMECOUNT		6
#define FX_KHANHITEFFECTS_FRAMECOUNT	6
#define FX_TELEPORT_FRAMECOUNT			20
#define FX_WEAPONEFFECTS_FRAMECOUNT		1
#define FX_RUNSMOKE_FRAMECOUNT			15
#define FX_WARGIANTMULTI_FRAMECOUNT		50
#define FX_WARGIANTATTACK_FRAMECOUNT	7
#define FX_WARGIANTDIE_FRAMECOUNT		130
#define FX_MOBSHOWUP_FRAMECOUNT			20
#define FX_POWERUP_FRAMECOUNT 			999999
#define FX_GRYPHON_FRAMECOUNT			20
#define FX_DRAGON_FRAMECOUNT			18
#define FX_MOBPARTICLE_FRAMECOUNT		1
#define FX_GHOSTHUNT_FRAMECOUNT			150
#define FX_GHOSTRISE_FRAMECOUNT			150
#define FX_SOULDUST_FRAMECOUNT 			60
#define FX_MAGICSWORD_FRAMECOUNT		150
#define FX_SPHINX_FRAMECOUNT			20
#define FX_VAMPIRE_FRAMECOUNT			150
#define FX_VAMPIREATTACK_FRAMECOUNT		150
#define FX_MUSICALNOTE_FRAMECOUNT		2400
#define FX_FALLOFFSTONE_FRAMECOUNT		21
#define FX_STONERANCE2_FRAMECOUNT		21
#define FX_DRILLSTONE_FRAMECOUNT		21
#define FX_CATCHFLAGDIR_FRAMECOUNT		2400
#define FX_DROPCANDY_FRAMECOUNT			20 //-- Drop Candy(tree) Frame Count



#define FX_DEVILRING_FRAMECOUNT			18
#define FX_FIREWORKS_FRAMECOUNT			46 //-- FireWork!!(Lyul)
#define FX_DSTONE_FRAMECOUNT			46    //-- Lyul_2005
#define FX_DSLASH_FRAMECOUNT			24    //-- Lyul_2005
#define FX_BASH_FRAMECOUNT				100     //-- Lyul_2005
#define FX_DSTONEEFF_FRAMECOUNT			110 //-- Lyul_2005
#define FX_BLEED_FRAMECOUNT				15 //-- Lyul_2005
#define FX_ASSAULT_FRAMECOUNT			63 //-- Lyul_2005
#define FX_WEAPONEFF_FRAMECOUNT			20 //-- Lyul_2005 -- 다른 것과 꼬여 있어서 결국 따로 빼버림
#define FX_DEMICRUSHER_FRAMECOUNT		50
#define FX_FIRETOWER_FRAMECOUNT			50
#define FX_DSTONEBRUST_FRAMECOUNT		20 //mungmae-2005/04/29
#define FX_MINE_FRAMECOUNT				70	// nate 2005-06-15 : 휴먼 스킬 추가
#define FX_HELLDOWN_FRAMECOUNT			100  //카이 헬다운 스킬(누른 상태)
#define FX_HELLDOWN_FRAMECOUNT2			50  //카이 헬다운 스킬(누른 상태)
#define FX_LIFEEXPENSION_FRAMECOUNT		2400	// nate 2005-07-06 : Life Expansion
#define FX_SUMMONSQUAD_FRAMECOUNT		20	// nate 2005-07-08 : Summon Squad
#define FX_REFLECTION_FRAMECOUNT		2400 //mungmae 2005/07/28 :리플렉션
#define FX_DURANT_BEAM_FRAMECOUNT		100 // nate 2005-09-26 : 듀란트 - 광선
#define FX_DURANT_ATTACK_FRAMECOUNT		40	// nate 2005-09-26 : 듀란트 - 어택
#define FX_DURANT_TARGET_FRAMECOUNT		60	// nate 2005-09-26 : 듀란트 - 공격 대상
#define FX_POISON_FRAMECOUNT			60	// nate 2005-11-03 : 포이즌
#define FX_ADRENALINE_FRAMECOUNT		2400// mungmae 2005/11/21 : 아드레날린
#define FX_ATTACKMOVELOLO_FRAMECOUNT	20	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
#define FX_FLYINGCRASH_FRAMECOUNT		5	// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
#define FX_ATTACKFIXLOLO_FRAMECOUNT		999999	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx // 임의로 큰수로 바꿈 06-12-18 원석
#define FX_FLYINGCRASH2_FRAMECOUNT		100	// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
#define FX_SHADOWSLASH_FRAMECOUNT		25	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
#define FX_SHADOWSLASH2_FRAMECOUNT		15	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
#define FX_TLVERTEX_FRAMECOUNT			999999	// nate 2006-01-17 : 메듀사 // 임의로 큰수로 바꿈 06-12-18 원석
#define FX_SUMMONRING_FRAMECOUNT		1000 // 원석 2006-05-19 : 기존 로드된 에이디아 링 이펙트중 기존 로드된 그래픽을 이용하여 찍는 녀석 // 임의로 큰수(실제 프레임은 타입별로 다르게...) ///불칸링
#define FX_SPEEDUPSMOKE_FRAMECOUNT		50  //mungmae 2006/06/08 : 월드컵 스피드업 이펙트
#define FX_PETSKILL_FRAMECOUNT			1000   // 원석 2006-07-10 : 펫의 스킬 이펙트 // 임의로 큰수(실제 프레임은 타입별로 다르게...) ///불칸링
#define FX_SPIRITMODE_FRAMECOUNT        40  //스피릿
#define FX_SPIRITMODE2_FRAMECOUNT       40  //스피릿
#define FX_SPACESLASH_FRAMECOUNT		31
#define FX_ELECTRICHAMMER_FRAMECOUNT	40
#define FX_SUMMONFIRE_FRAMECOUNT		61
#define FX_FLAMESPEAR_FRAMECOUNT		999999 // 원석 2006-10-24 : ///플레임스피어 // 임의로 큰수
#define FX_RANDOMFIRE_FRAMECOUNT		130
#define FX_RANDOMSHOOT_FRAMECOUNT       3
#define FX_MERCERNERYSLASH_FRAMECOUNT   71
#define FX_ICEBLOT_FRAMECOUNT			40
#define FX_PROTECTIONSHIELD_FRAMECOUNT  300
#define FX_WILDSMASH_FRAMECOUNT			40
#define FX_RESURRECTION_FRAMECOUNT		50
#define FX_TORNADO_HAMMER_FRAMECOUNT	52
#define FX_POISON_FOG_FRAMECOUNT		999999 // 원석 2006-10-24 : 독안개 ///인던1 // 임의로 큰수
#define FX_SUMMONFIREBOOM_FRAMECOUNT    20
#define FX_SHIELDBLOCK_FRAMECOUNT		20
#define FX_TYPE_PARTICLE_FRAMECOUNT		999999 // 원석 2007-01-18 : 타입별 파티클 이펙트 ///하트로드 // 임의로 큰수
#define FX_WTRSPRINKLE_FRAMECOUNT		40	// [3/26/2007 Theodoric] 태국 송크란 이벤트  
#define FX_WTRSPRINKLEBOOM_FRAMECOUNT	20	// [3/26/2007 Theodoric] 태국 송크란 이벤트
#define FX_BABYSOUL_FRAMECOUNT			46	// [8/7/2007 Theodoric]  6주년 기념 이벤트  
#define FX_MURAMASA_GHOST_FRAMECOUNT	999999 ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트. // 임의로 큰수
#define FX_DMITRON_PORTAL_FRAMECOUNT	24 // 드미트론 점령전 - 이동 게이트
#define FX_DMITRON_CRYSTAL_FRAMECOUNT	24 // 드미트론 점령전 - 크리스탈

#define FX_BOSSRAID_BULKAN1_02_FRAMECOUNT	1
#define FX_BOSSRAID_BULKAN1_03_FRAMECOUNT	50
#define FX_BOSSRAID_BULKAN2_02_FRAMECOUNT	120
#define FX_BOSSRAID_BULKAN3_01_FRAMECOUNT	999999	// [11/21/2007 반재승] 보스레이드 불칸보스3, 2번 스킬 임의에 큰수(랜더부분에서 짤라줌)
#define FX_BOSSRAID_BULKAN3_02_FRAMECOUNT	90	    // [11/21/2007 반재승] 보스레이드 불칸보스3, 3번 스킬

#define FX_BOSSRAID_HUMAN1_02_FRAMECOUNT	50
#define FX_BOSSRAID_HUMAN1_03_FRAMECOUNT	8
#define FX_BOSSRAID_HUMAN2_02_FRAMECOUNT	120
#define FX_BOSSRAID_HUMAN2_03_FRAMECOUNT	100
#define FX_BOSSRAID_HUMAN3_02_FRAMECOUNT	90	    // [12/25/2007 반재승] 보스레이드 휴먼보스3, 2번 스킬

#define FX_BOSSRAID_HYBRI1_01_FRAMECOUNT	120
#define FX_BOSSRAID_HYBRI1_02_FRAMECOUNT	120
#define FX_BOSSRAID_HYBRI2_02_FRAMECOUNT	90
#define FX_BOSSRAID_HAYBRI3_01_FRAMECOUNT	71	    // [01/15/2008 반재승] 보스레이드 하이브리더보스3, 1번 노멀 어택
#define FX_BOSSRAID_HAYBRI3_02_FRAMECOUNT	66	    // [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

#define FX_BOSSRAID_KAI1_01_FRAMECOUNT		120
#define FX_BOSSRAID_KAI1_02_FRAMECOUNT		120
#define FX_BOSSRAID_KAI2_01_FRAMECOUNT		31		// [2/5/2008 반재승] 카이립톤보스2 1 어택
#define FX_BOSSRAID_KAI2_02_FRAMECOUNT		40		// [2/5/2008 반재승] 카이립톤보스2 2 어택
#define FX_BOSSRAID_KAI2_03_FRAMECOUNT		81		// [2/5/2008 반재승] 카이립톤보스2 3 어택
#define FX_BOSSRAID_KAI3_01_FRAMECOUNT		31		// [2/4/2008 반재승] 카이립톤보스3 노멀 어택
#define FX_BOSSRAID_KAI3_02_FRAMECOUNT		41		// [2/4/2008 반재승] 카이립톤보스3 2번스킬
#define FX_BOSSRAID_KAI3_03_FRAMECOUNT		160		// [2/11/2008 반재승] 카이립톤보스3 3번스킬, 독구름의 지속 시간 때문에 애니메이션 프레임 보다 길게 잡아줌.

#define	FX_BOSSRAID_AIDIA1_01_FRAMECOUNT	41	// 에이디아 1_01 노말 어텍
#define	FX_BOSSRAID_AIDIA1_02_FRAMECOUNT	41	// 에이디아 1_02 멀티 어텍
#define	FX_BOSSRAID_AIDIA1_02_SHOCKWAVE_FRAMECOUNT	16	// 에이디아 1_02_ 쇼크웨이브 효과

#define FX_BOSSRAID_ADIA2_01_FRAMECOUNT		120
#define FX_BOSSRAID_ADIA2_02_FRAMECOUNT		120
#define FX_BOSSRAID_ADIA2_03_FRAMECOUNT		120

#define FX_BOSSRAID_AIDIA3_01_FRAMECOUNT		41		// [3/6/2008 반재승] 에이디아보스3 노멀 어택
#define FX_BOSSRAID_AIDIA3_02_FRAMECOUNT		71		// [3/6/2008 반재승] 에이디아보스3 2번 스킬

#define FX_BOSSRAID_LASTBOSS_02_FRAMECOUNT	120
#define FX_BOSSRAID_LASTBOSS_03_FRAMECOUNT	120
#define FX_BOSSRAID_LASTBOSS_04_FRAMECOUNT	120

#define FX_ELECTRICPARTICLE_FRAMECOUNT		1
#define FX_CLYSTALGLOW_FRAMECOUNT			100
#define FX_WATERFALL_FRAMECOUNT				100
#define FX_WATERFALLPARTICLE_FRAMECOUNT		1
#define FX_PORTAL_BOSSRAID_FRAMECOUNT		1
#define FX_FIRESMOKE_PARTICLE_FRAMECOUNT	1

#define FX_DROPSTARCANDY_FRAMECOUNT			120

#define FX_HERO_EFFECT_FRAMECOUNT			1	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트 

#define FX_SARNGA_MOUEL_FRAMECOUNT			26 // [12/3/2008 Theodoric] 사릉가
#define FX_YETI_RING_FRAMECOUNT				26 // AIDIA LEVEL 450
#define FX_PEAKOCK_RING_FRAMECOUNT			26 // AIDIA LEVEL 500
#define FX_TIGER_RING_FRAMECOUNT			26 // AIDIA LEVEL 550
#define FX_UNICORN_RING_FRAMECOUNT			26 // AIDIA LEVEL 600
#define FX_650_RING_FRAMECOUNT			26 // AIDIA LEVEL 650
#define FX_700_RING_FRAMECOUNT			26 // AIDIA LEVEL 700
#define FX_750_RING_FRAMECOUNT			26 // AIDIA LEVEL 750
#define FX_800_RING_FRAMECOUNT			26 // AIDIA LEVEL 800
#define FX_850_RING_FRAMECOUNT			26 // AIDIA LEVEL 850
#define FX_900_RING_FRAMECOUNT			26 // AIDIA LEVEL 900
#define FX_950_RING_FRAMECOUNT			26 // AIDIA LEVEL 950
#define FX_1000_RING_FRAMECOUNT			26 // AIDIA LEVEL 1000
#define FX_CHR_RING_FRAMECOUNT			26
#define FX_CHIN_RING_FRAMECOUNT			26	//CHINESE NEW YEARS
#define FX_ARUPAKA_SPIT_FRAMECOUNT			60 // 알파카펫 침뱉는 이펙트

// effect's frame count
#define FX_PEROM_SKILL_S125_FRAME			0
#define FX_PEROM_SKILL_S126_FRAME			0
#define FX_PEROM_SKILL_S127_FRAME			42
#define FX_PEROM_SKILL_S128_FRAME			51
#define FX_PEROM_SKILL_S129_FRAME			56
#define FX_PEROM_SKILL_S130_FRAME			43	// alpha strike
#define FX_PEROM_SKILL_S131_FRAME			51
#define FX_PEROM_SKILL_S132_FRAME			56
#define FX_PEROM_SKILL_S133_FRAME			51
#define FX_PEROM_SKILL_S134_FRAME			43
#define FX_PEROM_SKILL_S135_FRAME			56
#define FX_PEROM_SKILL_S136_FRAME			43
#define FX_PEROM_SKILL_S137_FRAME			43
#define FX_PEROM_SKILL_S138_FRAME			43
#define FX_PEROM_SKILL_S139_FRAME			36
#define FX_PEROM_SKILL_S140_FRAME			41
#define FX_PEROM_SKILL_S141_FRAME			43
#define FX_PEROM_SKILL_S142_FRAME			0
#define FX_PEROM_SKILL_S143_FRAME			0
#define FX_PEROM_SKILL_S144_FRAME			0
#define FX_PEROM_SKILL_S145_FRAME			0
#define FX_PEROM_SKILL_S146_FRAME			0
#define FX_PEROM_SKILL_S147_FRAME			0
#define FX_PEROM_SKILL_S148_FRAME			43
#define FX_PEROM_SKILL_S149_FRAME			43
#define FX_PEROM_SKILL_S150_FRAME			43

#define FX_GRIFFON_RING_FRAMECOUNT			26

#define FX_BALLOON_KILL_FRAMECOUNT		61

//============================================================================

//============================================================================
// 특수 연출 이벤트 신 ///인던1 시 추가
#define EVENTACTION_DEKAREN_BOSS_IN			1 // 인던 데카렌 보스 등장 씬.
#define EVENTACTION_DECKAREN_CUT_IN1		2 // 데카렌 1층 돌문 앞 이벤트 컷인. 총 4장. ///인던1
#define EVENTACTION_DECKAREN_CUT_IN2		3 // 데카렌 1층 돌문 앞 이벤트 컷인. 총 4장. ///인던1
#define EVENTACTION_DECKAREN_CUT_IN3		4 // 데카렌 1층 돌문 앞 이벤트 컷인. 총 4장. 여기서 2장 연속 출력.. ///인던1
#define EVENTACTION_TUTORIAL                5 ///튜토리얼 이벤트. 스크립트를 통해 장시간 다양하게 제어된다.
//============================================================================

class CNkCharacter;
class CNkMob;
class CNkPet;

typedef enum Hit_Index
{
	HIT_HAND = 0,
	HIT_SWORD,
	HIT_SOWRD_ROTATE_ATTACK,
	HIT_AXE,
	HIT_AXE_JUMP_ATTACK,
	HIT_STAFF,
	HIT_KINES,
	HIT_RAVEN,
	HIT_NEO,
	HIT_KAISEN,
	HIT_GRYPHON,
	HIT_DRAGON,
	HIT_MOB_WORLDCUP_HAND,
	HIT_MOB_WORLDCUP_FOOT,
	HIT_WARGIANT_ATTACK,
	HIT_DSWORD,		//-- 이도류 이펙트 추가.
	HIT_LEGKNOCK_ATTACK, // 데카렌 보스 타격 ///인던1
	HIT_HEART, // 하트로드 타격 이펙트 하트가 폴폴~ ///하트로드
	HIT_LIP, // 2007.03.14 화이트
	HIT_LOVE,  // 2007.03.14 화이트
	HIT_SONGKRAN			// [3/27/2007 Theodoric] 태국 송크란 이벤트
} Hit_Index;

typedef enum Pet_Hit_Index
{
	PET_HIT_NAIL = 0,
	PET_HIT_KICK,
	PET_HIT_HEADING,
	PET_HIT_BITE,
	PET_HIT_STRIKE,
	PET_HIT_FIRE,
	PET_HIT_LIGHT,
	PET_HIT_RELICS,
	PET_HIT_GAS

} Pet_Hit_Index;

typedef struct Deco_Particle Deco_Particle;

enum eEffectParentType
{
	eEffectParentType_None = 0,
	eEffectParentType_Char,
	eEffectParentType_Mob,
	eEffectParentType_Deco,
	eEffectParentType_LdDeco,

	eEffectParentType_End,
};

struct EffectSort
{
	INT			nType;
	int			nNum;			// Effect 고유 아이디.
	BOOL		bOneFrame;		// 매프레임마다 호출되는 효과인가?
	int			nCurFrame;
	int			nTargetType;	// 타겟이 몹인가 플레이어인가..
	D3DVECTOR	vPos;			// 시작 위치.
	D3DVECTOR	vCurPos;		// 현재의 위치.
	D3DVECTOR	vTargetPos;		// 타켓의 위치.
	D3DVECTOR	vSortPos;
	CNkCharacter *pNkChaFrom;
	CNkCharacter *pNkChaTo;
	CNkMob		 *pMobFrom;
	CNkMob       *pMobTo;
	CNkPet		*pNkPetFrom;
	CNkPet		*pNkPetTo;
	DWORD		dwColor;
	int			Index;
	int			num;
	void*		effects;
	int			subNum;			// 날아갈 아이템 번호 (일단은 차크람만 쓰인다.)
	Deco_Particle*		pParticle; // 파티클 정보를 직접 가지고 있어야할 경우 사용하기 위해 추가(위의 보이드형 포인터는 직접 해제해주기 문제가 많았다.) ///플레임스피어 시 추가.
	int			parentType;
	void*		ptrParent;
};

//sparkle
typedef struct t_sparkle
{
	int			texture;
	int			age, cur_age;	// start at age and tick down to 0
	float		scale, delta_scale;
	D3DVECTOR	position;
	D3DVECTOR	velocity;
	D3DVECTOR	color;
} Sparkle;

/////////////////////////////////////////////////////////////
// EffectSort를 정렬하기 위해서 사용하는..
bool operator<(const EffectSort &r1, const EffectSort &r2);
bool descending(const EffectSort &r1, const EffectSort &r2);

class CShadowRect;
class CFxShadowRing;
class CFxShadowLight;
class CFxLight;
class CFxPortal;
class CFxSmoke;
class CFxParticle;
class CParticleSystem;
class CFxLightFog;
class CFxShowup;
class CFxLightning;
class CFxLightBall;
class CFxFireBall;
class CFxFireBallBoom;
class CFxFireBallBoom2;
class CFxLwLight;
class CFxLightBallBoom;
class CFxUnarmedAttacked;
class IndexedTexture;
class CFxMeteor;
class CFxPebbleShot;
class CFxStoneShot;
// nate 2004 - 8 ImageManager
//class CFxFShower;
class CFxStoneShotBoom;
class CFxPebbleShotBoom;
class CFxShoot;
class CFxShootAttacked;
class CFxAidiaFire;
class CFxAidiaBallBoom;
class CFxLightHeal;
class CFxStarFlare;
class CFxStarFlareBoom;
class CFxShockSpear;
class CFxStoneRance;
class CFxRedFire;
class CFxFly;
class CFxBee;
class CFxBeeBoom;
class CFxBora;
class CFxBoraBoom;
class CFxMoth;
class CFxShilon;
class CFxHorn;
class CFxSilverFly;
class CFxRaven;
class CFxKines;
class CFxNeo;
class CFxKaisen;
class CFxHitEffects;
class CFxHitBoomEffects;
class CFxLevelUp;
class CFxShowUp2;
class CFxEatEffects;
class CFxShield;
class CFxSmokeParticle;
class CFxFireParticle;
class CFxHornBoom;
class CFxHealEffects;
class CFxWorldCup;
class CFxWorldCupBoom;
class CFxGuardTower;
class CFxShilonMaster;
class CFxLightBallBoomNew;
class CFxControlLight;
class CFxCatchFlag;
class CFxGreenSmoke;
class CFxConditionEffects;
class CFxConditionEffects2;
class CFxOptionSkillEffects;
class CFxHitPowerEffects;
class CFxEarthQuake;
class CFxFrost;
class CFxChakram;
class CFxBlackHole;
class CFxScrollRing;
class CFxScrollDamage;
class CFxSkillLight;
class CFxScrollLight;
class CFxChain;
class CFxOnOffEffects;
class CFxScanView;
class CFxChargeCannon;
class CFxFlame;
class CFxAutoLaser;
class CFxLifeCommon;
class CFxLifeLight;
class CFxMagicArrow;
class CFxMagicLaser;
class CFxFireBoomShort;
class CFxRocket;
class CFxMeteorite;
class CFxMeteoriteBoom;
class CFxChaos;
class CFxGatheringGun;
class CFxDragonEffects;
class CFxNuclearAim;
class CFxNuclearBoom;
class CFxDeathWaveAim;
class CFxCharWave;
class CFxPhenix;
class CFxSprayParticle;
class CFxTurnParticle;
class CFxLightParticle;
class CFxHitPetEffects;
class CFxKhanHitEffects;
class CFxTeleport;
class CFxWeaponEffects;
class CFxRunSmoke;
class CFxWarGiantMulti;
class CFxWarGiantAttack;
class CFxWarGiantDie;
class CFxMobShowUp;
class CFxPowerUp;
class CFxGryphon;
class CFxDragon;
class CFxMobParticle;
class CFxGhostHunt;
class CFxGhostRise;
class CFxSoulDust;
class CFxMagicSword;
class CFxSphinx;
class CFxVampire;
class CFxVampireAttack;
class CFxMusicalNote;
class CFxFallOffStone;
class CFxStoneRance2;
class CFxDrillStone;
class CFxCatchFlagDir;
class CFxDropCandy;

class CFxDropStarCandy;

//중고렙 스킬 추가
class CFxHellCry;
class CFxAirRaid;
class CFxTornadoRuin;
class CFxSumPart;

class CFxDevil;
class CFxSummonRing; ///불칸링
class CFxFireWorks;//-- FireWork!!(Lyul)
//----------------------------------------------------------------------------
//-- Lyul_2005 Skill Effect
class CFxDstone;
class CFxDSlash;
class CFxBash;
class CFxDStoneEff;
class CFxBleed;
class CFxAssault;
class CFxReflection;

class CFxDemiCrusher;
class CFxFireTower;
class CFxStoneBrust;
class CFxHellDown_Circle;
class CFxHellDown_Circle2;
class CFxHellDown_Around;
class CFxHellDown_Lightly;
class CFxHellDown_Spark;
class CFxHellDown_Lightning;
class CFxMine;			// nate 2005-06-15 : 휴먼 스킬 추가
class CFxLifeExpansion;	// nate 2005-07-06 : Life Expansion
class CFxSummonSquad;	// nate 2005-07-08 : Summon Squad
class CFxDurant;		// nate 2005-09-26 : 듀란트 - 광선
class CFxDurantTarget;	// nate 2005-09-26 : 듀란트 - 공격대상
class CFxPoison;		// nate 2005-11-03 : 포이즌
class CFxAdrenaline;	// mungmae 2005/11/21 : 하이브리더 - 아드레날
class CFxAttack_MoveLolo;	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
class CFxFlyingCrash;	// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
class CFxFlyingCrash2;	// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
class CFxAttackFixLolo;	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
class CFxShadowSlash;	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
class CFxShadowSlash2;	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
class CFxFollow;			///페어리
class CFxMuramasaGhost; ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
class CFxFlameSpear;	///플레임스피어
class CFxTLVertex;		// nate 2006-01-17 : 메듀사
class CFxSpeedUpSmoke;
class CFxPetSkill; // 라비용 스킬 이펙트
class CFxSpiritMode;
class CFxSpiritMode2;
class CFxSpaceSlash;
class CFxElectricHammer;
class CFxSummonFire;
class CFxSummonFireBoom;
class CFxRandomFire;
class CFxRandomShoot;
class CFxMercernerySlash;
class CFxIceBlot;
class CFxProtectionShield;
class CFxWildSmash;
class CFxResurrection;
class CFxTornadoHammer;
class CFxPoisonFog; ///인던1
class CFxShieldBlock;
class CFxTypeParticle; ///하트로드
class CFxFreePvpRing; ///자유대련
class CFxWaterSprinkle;		// [3/26/2007 Theodoric] 태국 송크란 이벤트
class CFxWaterSprinkleBoom; // [3/26/2007 Theodoric] 태국 송크란 이벤트
class CFxBabySoul;			// [8/7/2007 Theodoric] 6주년 기념 이벤트
class CFxDmitronPortal;
class CFxDmitronCrystal;

class CFxElectricParticle;
class CFxClystalGlow;
class CFxWaterfall;
class CFxWaterfallParticle;
class CFxLightningEx;
class CFxPortalBossraid;
class CFxFireSmokeParticle;

class CFxBossRaidBulkan1_02;
class CFxBossRaidBulkan1_03;
class CFxBossRaidBulkan2_02;
class CFxBossRaidBulkan3_02;
class CFxBossRaidBulkan3_03;

class CFxBossRaidHuman1_02;
class CFxBossRaidHuman1_03;
class CFxBossRaidHuman2_02;
class CFxBossRaidHuman2_03;
class CFxBossRaidHuman3_02;

class CFxBossRaidHybri1_01;
class CFxBossRaidHybri1_02;
class CFxBossRaidHybri2_02;
class CFxBossRaidHybri3_01; // [01/15/2008 반재승] 보스레이드 하이브리더보스3, 1번 노멀 어택
class CFxBossRaidHybri3_02; // [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

class CFxBossRaidKai1_01;
class CFxBossRaidKai1_02;
class CFxBossRaidKai2_01; // [2/5/2008 반재승] 카이립톤보스2 1 어택
class CFxBossRaidKai2_02; // [2/5/2008 반재승] 카이립톤보스2 2 어택
class CFxBossRaidKai2_03; // [2/5/2008 반재승] 카이립톤보스2 2 어택
class CFxBossRaidKai3_01; // [2/4/2008 반재승] 카이립톤보스3 노멀 어택
class CFxBossRaidKai3_02; // [2/4/2008 반재승] 카이립톤보스3 2번스킬
class CFxBossRaidKai3_03; // [2/11/2008 반재승] 카이립톤보스3 3번스킬

class CFxBossRaidAidia1_01;
class CFxBossRaidAidia1_02;
class CFxBossRaidAidia1_02_Shockwave;

class CFxBossRaidAidia2_01;
class CFxBossRaidAidia2_02;
class CFxBossRaidAidia2_03;

class CFxBossRaidAidia3_01; // [3/6/2008 반재승] 에이디아보스3 노멀 어택
class CFxBossRaidAidia3_02; // [3/6/2008 반재승] 에이디아보스3 2번 스킬

class CFxBossRaidLastBoss_02;
class CFxBossRaidLastBoss_03;
class CFxBossRaidLastBoss_04;

class CFxHeroEffect;	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트
class CFxSarngaMouel;	// [12/3/2008 Theodoric] 사릉가 이펙트
class LHFX_YetiRing;	// AIDIA LEVEL 450
class LHFX_PeakockRing; // AIDIA LEVEL 500
class LHFX_TigerRing;	// AIDIA LEVEL 550
class LHFX_UnicornRing; // AIDIA LEVEL 600
class LHFX_650Ring; // AIDIA LEVEL 650
class LHFX_700Ring; // AIDIA LEVEL 700
class LHFX_750Ring; // AIDIA LEVEL 750
class LHFX_800Ring; // AIDIA LEVEL 800
class LHFX_850Ring; // AIDIA LEVEL 850
class LHFX_900Ring; // AIDIA LEVEL 900
class LHFX_950Ring; // AIDIA LEVEL 950
class LHFX_1000Ring; // AIDIA LEVEL 1000
class LHFX_CHRRing;
class LHFX_CHINRing;
class FxArupakaSpit;

class CFxLv400Effect;

class LHFX_Perom_Spin;
class LHFX_Perom_Thrust;
class LHFX_Perom_Blader;
class LHFX_Perom_Pierce;
class LHFX_Perom_Boom;
class LHFX_Perom_Shotgun;
class LHFX_Perom_Field;
class LHFX_Perom_Wheelwind;
class LHFX_Perom_Strike;
class LHFX_Perom_Cross;
class LHFX_Perom_Meta;
class LHFx_Perom_DSystem;
class LHFx_Perom_Overload;
class LHFx_Perom_Alpha;

class LHFX_GriffonRing;

class CFxBalloonKill;
class CFxCommonLolo;

class CFxSet
{
public:
	typedef std::vector<EffectSort*> VEC_EFFSORT;
	typedef std::map<int, EffectSort*> MAP_EFFSORT;

protected:
	int					m_nEfIdx;

public:
	CFxSet();
	virtual ~CFxSet();

	vector<EffectSort*>  m_EfSortVector;

	// Fx Classes
	CFxLight	*m_pFxLight;
	CFxPortal	*m_pFxPortal;
	CFxSmoke	*m_pFxSmoke;
	CFxShadowRing *m_pFxShadowRing;
	CFxShadowLight *m_pFxShadowLight;
	CFxParticle	   *m_pFxParticle;
	CFxLightFog	   *m_pFxLightFog;
	CFxShowup	   *m_pFxShowup;
	CFxLightning   *m_pFxLightning;
	CFxLightBall   *m_pFxLightBall;
	CFxFireBall	   *m_pFxFireBall;
	CFxFireBallBoom *m_pFxFireBallBoom;
	CFxFireBallBoom2 *m_pFxFireBallBoom2;
	CFxLwLight	   *m_pFxLwLight;
	CFxLightBallBoom *m_pFxLightBallBoom;
	CFxUnarmedAttacked *m_pFxUnarmedAttacked;
	CFxMeteor		*m_pFxMeteor;
	CFxPebbleShot	*m_pFxPebbleShot;
	CFxStoneShot	*m_pFxStoneShot;
	// nate 2004 - 8 ImageManager
//	CFxFShower		*m_pFxFShower;
	CFxStoneShotBoom *m_pFxStoneShotBoom;
	CFxPebbleShotBoom *m_pFxPebbleShotBoom;
	CFxShoot		*m_pFxShoot;
	CFxShootAttacked *m_pFxShootAttacked;
	CFxAidiaFire	*m_pFxAidiaFire;
	CFxAidiaBallBoom *m_pFxAidiaBallBoom;
	CFxLightHeal	*m_pFxLightHeal;
	CFxStarFlare	*m_pFxStarFlare;
	CFxStarFlareBoom *m_pFxStarFlareBoom;
	CFxShockSpear	*m_pFxShockSpear;
	CFxStoneRance	*m_pFxStoneRance;
	CFxRedFire		*m_pFxRedFire;
	CFxFly			*m_pFxFly;
	CFxBee			*m_pFxBee;
	CFxBeeBoom		*m_pFxBeeBoom;
	CFxBora			*m_pFxBora;
	CFxBoraBoom		*m_pFxBoraBoom;
	CFxMoth			*m_pFxMoth;
	CFxShilon		*m_pFxShilon;
	CFxHorn			*m_pFxHorn;
	CFxSilverFly	*m_pFxSilverFly;
	CFxRaven		*m_pFxRaven;
	CFxKines		*m_pFxKines;
	CFxNeo			*m_pFxNeo;
	CFxKaisen		*m_pFxKaisen;
	CFxHitEffects	*m_pFxHitEffects;
	CFxHitBoomEffects *m_pFxHitBoomEffects;
	CFxLevelUp		*m_pFxLevelUp;
	CFxShowUp2		*m_pFxShowUp2;
	CFxEatEffects	*m_pFxEatEffects;
	CFxShield		*m_pFxShield;
	CFxSmokeParticle *m_pFxSmokeParticle;
	CFxFireParticle *m_pFxFireParticle;
	CFxHornBoom		*m_pFxHornBoom;
	CFxHealEffects	*m_pFxHealEffects;
	CFxWorldCup		*m_pFxWorldCup;
	CFxWorldCupBoom	*m_pFxWorldCupBoom;
	CFxGuardTower	*m_pFxGuardTower;
	CFxShilonMaster	*m_pFxShilonMaster;
	CFxLightBallBoomNew	*m_pFxLightBallBoomNew;
	CFxControlLight *m_pFxControlLight;
	CFxCatchFlag	*m_pFxCatchFlag;
	CFxGreenSmoke	*m_pFxGreenSmoke;
	CFxConditionEffects *m_pFxConditionEffects;
	CFxConditionEffects2 *m_pFxConditionEffects2;
	CFxOptionSkillEffects *m_pFxOptionSkillEffects;
	CFxHitPowerEffects *m_pFxHitPowerEffects;
	CFxEarthQuake	*m_pFxEarthQuake;
	CFxFrost		*m_pFxFrost;
	CFxChakram		*m_pFxChakram;
	CFxBlackHole	*m_pFxBlackHole;
	CFxScrollRing	*m_pFxScrollRing;
	CFxScrollDamage	*m_pFxScrollDamage;
	CFxSkillLight	*m_pFxSkillLight;
	CFxScrollLight	*m_pFxScrollLight;
	CFxChain		*m_pFxChain;
	CFxOnOffEffects *m_pFxOnOffEffects;
	CFxScanView		*m_pFxScanView;
	CFxChargeCannon *m_pFxChargeCannon;
	CFxFlame		*m_pFxFlame;
	CFxAutoLaser	*m_pFxAutoLaser;
	CFxLifeCommon	*m_pFxLifeCommon;
	CFxLifeLight	*m_pFxLifeLight;
	CFxMagicArrow	*m_pFxMagicArrow;
	CFxMagicLaser	*m_pFxMagicLaser;
	CFxFireBoomShort *m_pFxFireBoomShort;
	CFxRocket		*m_pFxRocket;
	CFxMeteorite	*m_pFxMeteorite;
	CFxMeteoriteBoom *m_pFxMeteoriteBoom;
	CFxChaos		*m_pFxChaos;
	CFxGatheringGun *m_pFxGatheringGun;
	CFxDragonEffects *m_pFxDragonEffects;
	CFxNuclearAim	*m_pFxNuclearAim;
	CFxNuclearBoom	*m_pFxNuclearBoom;
	CFxDeathWaveAim *m_pFxDeathWaveAim;
	CFxCharWave		*m_pFxCharWave;
	CFxPhenix		*m_pFxPhenix;
	CFxSprayParticle *m_pFxSprayParticle;
	CFxTurnParticle *m_pFxTurnParticle;
	CFxLightParticle *m_pFxLightParticle;
	CFxHitPetEffects *m_pFxHitPetEffects;
	CFxKhanHitEffects *m_pFxKhanHitEffects;
	CFxTeleport		*m_pFxTeleport;
	CFxWeaponEffects *m_pFxWeaponEffects;
	CFxRunSmoke		*m_pFxRunSmoke;
	CFxWarGiantMulti *m_pFxWarGiantMulti;
	CFxWarGiantAttack *m_pFxWarGiantAttack;
	CFxWarGiantDie	*m_pFxWarGiantDie;
	CFxMobShowUp	*m_pFxMobShowUp;
	CFxPowerUp		*m_pFxPowerUp;
	CFxGryphon		*m_pFxGryphon;
	CFxDragon		*m_pFxDragon;
	CFxMobParticle	*m_pFxMobParticle;
	CFxGhostHunt	*m_pFxGhostHunt;
	CFxGhostRise	*m_pFxGhostRise;
	CFxSoulDust		*m_pFxSoulDust;
	CFxMagicSword	*m_pFxMagicSword;
	CFxSphinx		*m_pFxSphinx;
	CFxVampire		*m_pFxVampire;
	CFxVampireAttack *m_pFxVampireAttack;
	CFxMusicalNote	*m_pFxMusicalNote;
	CFxFallOffStone *m_pFxFallOffStone;
	CFxStoneRance2	*m_pFxStoneRance2;
	CFxDrillStone	*m_pFxDrillStone;
	CFxCatchFlagDir *m_pFxCatchFlagDir;
	CFxDropCandy	*m_pFxDropCandy;	//-- DropCandy
	CFxDropCandy	*m_pFxDropApple;	//-- Lyul 추가 DropApple

	CFxDropCandy	*m_pFxDropApplepvp;
	CFxDropCandy	*m_pFxDropCandyvip;
	CFxDropCandy	*m_pFxDropCandypvp;



	CFxDropStarCandy	*m_pFxDropStarCandy; // 별사탕 이벤트
	CFxFireWorks	*m_pFxFireWorks;	//-- FireWork!!(Lyul)
	CFxDstone		*m_pFxDstone;		//-- Lyul_2005
	CFxDSlash		*m_pFxDslash;		//-- Lyul_2005
	CFxBash			*m_pFxBash;			//-- Lyul_2005
	CFxDStoneEff	*m_pFxDstoneEff;	//-- Lyul_2005
	CFxBleed		*m_pFxbleed;		//-- Lyul_2005
	CFxAssault		*m_pFxAssault;		//-- Lyul_2005
	CFxHellCry		*m_pFxHellCry;
	CFxAirRaid		*m_pFxAirRaid;
	CFxTornadoRuin	*m_pFxTornadoRuin;
	CFxSumPart		*m_pFxSumPart;
	CFxDevil		*m_pFxDevil;
	CFxSarngaMouel	*m_pFxSarngaMouel;  // [12/3/2008 Theodoric] 사릉가
	CFxSummonRing		*m_pFxSummonRing;    ///불칸링
	LHFX_YetiRing		*m_fx_yeti_ring;		// AIDIA LEVEL 450
	LHFX_PeakockRing	*m_fx_peakock_ring;		// AIDIA LEVEL 500
	LHFX_TigerRing		*m_fx_tiger_ring;		// AIDIA LEVEL 550
	LHFX_UnicornRing	*m_fx_unicorn_ring;		// AIDIA LEVEL 600
	LHFX_650Ring	*m_fx_650_ring;		// AIDIA LEVEL 650
	LHFX_700Ring	*m_fx_700_ring;		// AIDIA LEVEL 700
	LHFX_750Ring	*m_fx_750_ring;		// AIDIA LEVEL 750
	LHFX_800Ring	*m_fx_800_ring;		// AIDIA LEVEL 800
	LHFX_850Ring	*m_fx_850_ring;		// AIDIA LEVEL 850
	LHFX_900Ring	*m_fx_900_ring;		// AIDIA LEVEL 900
	LHFX_950Ring	*m_fx_950_ring;		// AIDIA LEVEL 950
	LHFX_1000Ring	*m_fx_1000_ring;		// AIDIA LEVEL 1000
	LHFX_CHRRing	*m_fx_CHR_ring;
	LHFX_CHINRing	*m_fx_CHIN_ring;
	CFxDemiCrusher	*m_pFxDemiCrusher;
	CFxFireTower	*m_pFxFireTower;
	CFxStoneBrust	*m_pFxStoneBrust;
	CFxReflection	*m_pFxReflection;
	//mungmae-2005/07/04 헬다운 추가
	CFxHellDown_Circle		*m_pFxHellDown_Circle;
	CFxHellDown_Circle2		*m_pFxHellDown_Circle2;
	CFxHellDown_Around		*m_pFxHellDown_Around;
	CFxHellDown_Lightly		*m_pFxHellDown_Lightly;
	CFxHellDown_Spark		*m_pFxHellDown_Spark;
	CFxHellDown_Lightning	*m_pFxHellDown_Lightning;
	CFxMine					*m_pFxMine;				// nate 2005-06-15 : Auto Mine - 휴먼 스킬 추가
	CFxLifeExpansion		*m_pFxLifeExpansion;	// nate 2005-07-07 : Life Expansion - 에이디아 생명연장
	CFxSummonSquad			*m_pFxSummonSquad;		// nate 2005-07-08 : Summon Squad - 파티소환
	CFxDurant				*m_pFxDurant_Beam;		// nate 2005-09-26 : 듀란트 - 광선
	CFxDurantTarget			*m_pFxDurant_Target;	// nate 2005-09-26 : 듀란트 - 공격 대상
	CFxPoison				*m_pFxPoison;			// nate 2005-11-03 : 포이즌
	CFxAdrenaline			*m_pFxAdrenaline;		// mungmae 2005/11/21 : 하이브리더 - 아드레날린
	CFxAttack_MoveLolo		*m_pFxAttack_MoveLolo;	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	CFxFlyingCrash			*m_pFxFlyingCrash;		// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
	CFxFlyingCrash2			*m_pFxFlyingCrash2;		// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
	CFxAttackFixLolo		*m_pFxAttack_FixLolo;	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	CFxShadowSlash			*m_pFxShadowSlash;		// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
	CFxShadowSlash2			*m_pFxShadowSlash2;		// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
	CFxTLVertex				*m_pFxTLVertex;			// nate 2006-01-17 : 메듀사
	CFxFollow				*m_pFxFollow;			// 무언가를 따라다니는 종류의 이펙트. AI를 가질수도 있다. ///페어리
	CFxMuramasaGhost		*m_pFxMuramasaGhost;	///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
	CFxFlameSpear           *m_pFxFlameSpear;		///플레임스피어
	CFxPetSkill				*m_pFxPetSkill;
	CFxSpeedUpSmoke			*m_pFxSpeedUpSmoke;
	CFxSpiritMode           *m_pFxSpiritMode;       // 스피릿
	CFxSpiritMode2          *m_pFxSpiritMode2;
	CFxSpaceSlash			*m_pFxSpaceSlash;
	CFxElectricHammer		*m_pFxElectricHammer;
	CFxSummonFire			*m_pFxSummonFire;
	CFxSummonFireBoom		*m_pFxSummonFireBoom;
	CFxRandomFire			*m_pFxRandomFire;
	CFxRandomShoot			*m_pFxRandomShoot;
	CFxMercernerySlash		*m_pFxMercernerySlash;
	CFxIceBlot				*m_pFxIceBlot;
	CFxProtectionShield		*m_pFxProtectionShield;
	CFxShieldBlock			*m_pFxShieldBlock;
	CFxWildSmash			*m_pFxWildSmash;
	CFxResurrection			*m_pFxResurrection;
	CFxTornadoHammer		*m_pFxTornadoHammer;
	CFxPoisonFog            *m_pFxPoisonFog;        // 화면에 뿌려지는 독안개 ///인던1
	CFxTypeParticle         *m_pFxTypeParticle;       // 타입별로 제어하는 파티클 ///하트로드
	CFxFreePvpRing          *m_pFxFreePvpRing;      // 자유대련의 링. 이 녀석은 insert 시켜서 FxSet Render 에서 렌더 시키는 타입은 아니다.///자유대련
	CFxWaterSprinkle		*m_pFxWTRSprinkle;		// [3/26/2007 Theodoric] 태국 송크란 이벤트
	CFxWaterSprinkleBoom	*m_pFxWTRSprinkleBoom;	// [3/26/2007 Theodoric] 태국 송크란 이벤트
	CFxBabySoul				*m_pFxBabySoul;			// [8/7/2007 Theodoric] 6주년 기념 이벤트
	CFxDmitronPortal		*m_pFxDmitronPortal;	// 드미트론 점령전 - 이동 게이트
	CFxDmitronCrystal		*m_pFxDmitronCrystal;	// 드미트론 점령전 - 크리스탈

	CFxElectricParticle		*m_pFxElectricParticle;
	CFxClystalGlow			*m_pFxClystalGlow;
	CFxWaterfall			*m_pFxWaterfall;
	CFxWaterfallParticle	*m_pFxWaterfallParticle;
	CFxLightningEx			*m_pFxLightningEx;
	CFxPortalBossraid		*m_pFxPortalBossraid;
	CFxFireSmokeParticle	*m_pFxFireSmokeParticle;

	CFxBossRaidBulkan1_02	*m_pFxBossRaidBulkan1_02;
	CFxBossRaidBulkan1_03	*m_pFxBossRaidBulkan1_03;
	CFxBossRaidBulkan2_02	*m_pFxBossRaidBulkan2_02;
	CFxBossRaidBulkan3_02	*m_pFxBossRaidBulkan3_02; // [11/27/2007 반재승] 불칸 보스3-02번 스킬
	CFxBossRaidBulkan3_03	*m_pFxBossRaidBulkan3_03; // [11/27/2007 반재승] 불칸 보스3-03번 스킬

	CFxBossRaidHuman1_02	*m_pFxBossRaidHuman1_02;
	CFxBossRaidHuman1_03	*m_pFxBossRaidHuman1_03;
	CFxBossRaidHuman2_02	*m_pFxBossRaidHuman2_02;
	CFxBossRaidHuman2_03	*m_pFxBossRaidHuman2_03;
	CFxBossRaidHuman3_02	*m_pFxBossRaidHuman3_02; // [12/25/2007 반재승] 휴먼 보스3-02번 스킬

	CFxBossRaidHybri1_01	*m_pFxBossRaidHybri1_01;
	CFxBossRaidHybri1_02	*m_pFxBossRaidHybri1_02;
	CFxBossRaidHybri2_02	*m_pFxBossRaidHybri2_02;
	CFxBossRaidHybri3_01	*m_pFxBossRaidHybri3_01; // [1/16/2008 반재승] 하이브리더 보스3-노멀어택
	CFxBossRaidHybri3_02	*m_pFxBossRaidHybri3_02; // [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

	CFxBossRaidKai1_01		*m_pFxBossRaidKai1_01;
	CFxBossRaidKai1_02		*m_pFxBossRaidKai1_02;
	CFxBossRaidKai2_01		*m_pFxBossRaidKai2_01; // [2/5/2008 반재승] 카이립톤보스2 1 어택
	CFxBossRaidKai2_02		*m_pFxBossRaidKai2_02; // [2/5/2008 반재승] 카이립톤보스2 2 어택
	CFxBossRaidKai2_03		*m_pFxBossRaidKai2_03; // [2/5/2008 반재승] 카이립톤보스2 3 어택
	CFxBossRaidKai3_01		*m_pFxBossRaidKai3_01; // [2/4/2008 반재승] 카이립톤보스3 노멀 어택
	CFxBossRaidKai3_02		*m_pFxBossRaidKai3_02; // [2/4/2008 반재승] 카이립톤보스3 2번스킬 어택
	CFxBossRaidKai3_03		*m_pFxBossRaidKai3_03; // [2/11/2008 반재승] 카이립톤보스3 3번스킬 어택

	CFxBossRaidAidia1_01	*m_pFxBossRaidAidia1_01; // 에이디아 1_01 일반 공격
	CFxBossRaidAidia1_02	*m_pFxBossRaidAidia1_02; // 에이디아 1_01 일반 공격
	CFxBossRaidAidia1_02_Shockwave	*m_pFxBossRaidAidia1_02_sw; // 에이디아 1_01 일반 공격

	CFxBossRaidAidia2_01	*m_pFxBossRaidAidia2_01; // 에이디아 2 1번 정상 공격
	CFxBossRaidAidia2_02	*m_pFxBossRaidAidia2_02; // 에이디아 2 2번 특수 공격
	CFxBossRaidAidia2_03	*m_pFxBossRaidAidia2_03; // 에이디아 2 3번 특수 공격

	CFxBossRaidAidia3_01	*m_pFxBossRaidAidia3_01; // [3/6/2008 반재승] 에이디아보스3 노멀 어택
	CFxBossRaidAidia3_02	*m_pFxBossRaidAidia3_02; // [3/6/2008 반재승] 에이디아보스3 2번 스킬

	CFxBossRaidLastBoss_02	*m_pFxBossRaidLastBoss_02;
	CFxBossRaidLastBoss_03	*m_pFxBossRaidLastBoss_03;
	CFxBossRaidLastBoss_04	*m_pFxBossRaidLastBoss_04;
	CFxHeroEffect			*m_pFxHeroEffect;	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트

	FxArupakaSpit			*m_pFxArupakaSpit;

	CFxLv400Effect * m_pFxLv400Effect;


	LHFX_Perom_Spin			*		m_fx_perom_spin;
	LHFX_Perom_Thrust		*		m_fx_perom_thurst;
	LHFX_Perom_Blader		*		m_fx_perom_blader;
	LHFX_Perom_Pierce		*		m_fx_perom_pierce;
	LHFX_Perom_Boom			*		m_fx_perom_boom;
	LHFX_Perom_Shotgun		*		m_fx_perom_shotgun;
	LHFX_Perom_Field		*		m_fx_perom_field;
	LHFX_Perom_Wheelwind	*		m_fx_perom_wheelwind;
	LHFX_Perom_Strike		*		m_fx_perom_strike;
	LHFX_Perom_Cross		*		m_fx_perom_cross;
	LHFX_Perom_Meta			*		m_fx_perom_meta;
	LHFx_Perom_DSystem		*		m_fx_perom_dsystem;
	LHFx_Perom_Overload		*		m_fx_perom_overload;
	LHFx_Perom_Alpha		*		m_fx_perom_alpha;

	LHFX_GriffonRing		*		m_fx_griffon_ring;

	CFxBalloonKill*			m_pFxBalloonKill;

	CFxCommonLolo*			m_pFxBossAttack01;
	CFxCommonLolo*			m_pFxBossAttack02;
	CFxCommonLolo*			m_pFxBossDebuff;
	CFxCommonLolo*			m_pFxBossMagic;

	//////////////////////////////////////////////////////////////////////////

	int			*m_pFrameCount;
	// 데이타 초기화 및 해제.
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadResAfter();
	void		SetDevice() {};
	void		DeleteRes();

	int			InsertFxEffect(EffectSort ef_sort);
	void		DeleteEffect(int num, BOOL bRemoveParentsList = TRUE);
	EffectSort* GetEffect(int num);

	void		InsertTestEffect(int test_num);

	// 렌더링 제어.
	void		FrameMove();
	void		Render();

	///튜토리얼 시 추가
	void        RenderFadeScreen( D3DCOLOR Fade_Color ); // 페이드용 화면 전체 덮는 판때기를 그려주는 함수. 원래 이 클래스에 있을 만한 녀석은 아니지만 별도로 빼기 애매해서 여기다 넣는다.

private:
	void		deleteTimedEffect();
	VEC_EFFSORT::iterator eraseEffect(VEC_EFFSORT::iterator it, BOOL bRemoveParentsList = TRUE);

private:
	MAP_EFFSORT m_mapEfSort;
};

class CFxLight
{
public:
	CFxLight();
	~CFxLight();

	D3DLVERTEX	m_lverRect[4];
	int			m_nTotalFrame;

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		FrameMove();
	void		Render(D3DVECTOR &vPos);
};

class CFxPortal
{
public:
	CFxPortal();
	~CFxPortal();

	D3DLVERTEX	m_lverRect[24];
	float		m_fPortalSizeX[3], m_fPortalSizeY[3];

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		FrameMove();
	void		Render(D3DVECTOR &vPos);
	void		RenderRect(D3DVECTOR &vPos, float fRad);
};

class CFxSmoke
{
public:
	CFxSmoke();
	~CFxSmoke();

	D3DLVERTEX	m_lverRect[4];
	D3DVECTOR	m_ScaleVector[4];
	float		m_fSmokeSizeX[8], m_fSmokeSizeY[8];
	int			m_nTotalFrame;

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();

	void		FrameMove();
	void		Render(D3DVECTOR &vPos);
};


class CFxShadowRing
{
public:
	CFxShadowRing();
	~CFxShadowRing();

	int			m_nTotalFrame;
	int			m_nShadowRingSize[8];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	void		FrameMove();
	void		Render(D3DVECTOR &vPos);
};

class CFxShadowLight
{
public:
	CFxShadowLight();
	~CFxShadowLight();

	int			m_nTotalFrame;
	int			m_nShadowLightSize[8];

	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();

	void		FrameMove();
	void		Render(D3DVECTOR &vPos);
};

class CFxParticle
{
	CParticleSystem* m_ParticleSystem;

public:
	CFxParticle();
	~CFxParticle();

	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();
	void		LoadPreset( int iPreset );
	void		FrameMove( float fTimeKey );
	void		Render( D3DVECTOR &vPos );
};

#define MAX_SPARKLES	150
#define CUR_SPARKLES	20

//dynamics
#define MAX_FORCE		0.02f
#define MAX_VELOCITY	0.8
#define DAMP			0.998f
#define MAX_DIST	   60.0f
#define rnd() float(rand())/RAND_MAX

class CFxLightFog
{
public:
	CFxLightFog();
	~CFxLightFog();

	D3DLVERTEX	m_lverRect[4], m_lverRot[4];
	int			m_nTotalFrame;

	void		InitSparkles();
	Sparkle		RandomSparkle();
	int			m_nCurSparkles;

	Sparkle		*m_pSparkles;
	D3DLVERTEX  *m_pMesh;
	WORD		*m_pIndices;
	D3DVECTOR	m_vecBound;
	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();
	void		FrameMove(float fTimeKey);
	void		Render(D3DVECTOR &vPos);
};

typedef struct t_showupring
{
	D3DVECTOR	position;
	D3DVECTOR	color;
	float		fSpeed;
} ShowupRing;

#define FX_SHOWUPRINGNUM 6

class CFxShowup
{
public:
	CFxShowup();
	~CFxShowup();

	float		m_fRadius, m_fCycleRadius;	// 빛덩어리의 크기와 순환궤도의 크기.
	ShowupRing  m_ShowupRing[FX_SHOWUPRINGNUM];			// 6개의 빛덩어리.

	float		m_fHeight;
	D3DLVERTEX	m_lverRect[4], m_lverRot[4];
	D3DLVERTEX  *m_pMesh;
	WORD		*m_pIndices;
	int			m_nStartRingSize[10];
	int			m_nTotalFrame;
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();
	void		Render(D3DVECTOR &vPos, int nFrame);
};

class CFxLightning
{
public:
	CFxLightning();
	~CFxLightning();

	D3DLVERTEX			m_lverRect[16];
	D3DLVERTEX			m_lverFrag[4];
	int					m_nSparkSize[10];
	int					m_nTotalFrame;
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();
	void		Render(D3DVECTOR &vPos, int nFrame, int nType = 0);
	void		RenderFrag(D3DVECTOR &vPos, int nFrame);
	void		RenderFrag2(D3DVECTOR &vPos, int nFrame);
};

#define FX_LIGHTBALL_HEIGHT 10

class CFxLightBall
{
public:
	CFxLightBall();
	~CFxLightBall();

	float		m_fSize, m_fSpeed;
	D3DLVERTEX  m_lverRect[16];
	D3DLVERTEX	m_lverBoom[4];
	D3DLVERTEX	m_lverTail[4];
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

#define FX_FIREBALL_HEIGHT 10

class CFxFireBall
{
public:
	CFxFireBall();
	~CFxFireBall();

	float		m_fSize, m_fSpeed;
	D3DLVERTEX	m_lverHead[4];
	D3DLVERTEX	m_lverTail[4];
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

typedef struct t_texcode
{
	float tu, tv;
} Texcode;

class CFxFireBallBoom
{
public:
	CFxFireBallBoom();
	~CFxFireBallBoom();

	float		m_fSpeed, m_fBoomSize;
	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수.
	int			m_nFrameIdx[FX_FIREBALLBOOM_FRAMECOUNT];
	float		m_fBaseSize[FX_FIREBALLBOOM_FRAMECOUNT];
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();
	BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
};

class CFxFireBallBoom2
{
public:
	CFxFireBallBoom2();
	~CFxFireBallBoom2();

	float		m_fSpeed, m_fBoomSize;
	D3DLVERTEX	m_lverBoom[4];
	Texcode		m_Texcode[12];		// 한 이미지에 있는 구분되는 텍스쳐 수.
	int			m_nFrameIdx[FX_FIREBALLBOOM2_FRAMECOUNT];
	float		m_fBaseSize[FX_FIREBALLBOOM2_FRAMECOUNT];
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();
	BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
};

#define FX_LWLIGHT_HEIGHT 20

class CFxLwLight
{
public:
	CFxLwLight();
	~CFxLwLight();

	int			m_nTexFrame;
	D3DLVERTEX  m_lverRect[4];
	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();
	BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
};

class CFxLightBallBoom
{
public:
	CFxLightBallBoom();
	~CFxLightBallBoom();

	float		m_fBaseSize[FX_LIGHTBALLBOOM_FRAMECOUNT];
	float		m_fBoomSize[FX_LIGHTBALLBOOM_FRAMECOUNT];
	D3DLVERTEX			m_lverBoom[4];
	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect = NULL);
	void		DeleteRes();
	BOOL		Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame);
};

class CFxUnarmedAttacked
{
public:
	CFxUnarmedAttacked();
	~CFxUnarmedAttacked();

	float		m_fTexSize;
	D3DLVERTEX			m_lverRect[4];
	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		DeleteRes();
	BOOL		Render(D3DVECTOR &vPosTo, int nFrame);
};
#endif // _FX_SET_H_
