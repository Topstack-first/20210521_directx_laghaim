#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <algorithm>
using namespace std;

#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "FxSet.h"
#include "ShadowRect.h"
#include "main.h"
#include "CParticle.h"
#include "CParticleSystem.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Skill.h" ///불칸링 시 추가
#include "FxMeteor.h"
#include "FxPebbleShot.h"
#include "FxStoneShot.h"
#include "FxShoot.h"
#include "FxShootAttacked.h"
#include "FxAidiaFire.h"
#include "FxLightHeal.h"
#include "FxStarFlare.h"
#include "FxShockSpear.h"
#include "Land.h"
#include "FxStoneRance.h"
#include "FxRedFire.h"
#include "FxFly.h"
#include "FxBee.h"
#include "FxBeeBoom.h"
#include "FxBora.h"
#include "FxBoraBoom.h"
#include "FxMoth.h"
#include "FxBora.h"
#include "FxBoraBoom.h"
#include "FxMoth.h"
#include "FxShilon.h"
#include "FxHorn.h"
#include "FxHitEffects.h"
#include "FxHitBoomEffects.h"
#include "FxLevelUp.h"
#include "FxShowUp2.h"
#include "FxEatEffects.h"
#include "FxShield.h"
#include "FxSmokeParticle.h"
#include "FxFireParticle.h"
#include "FxHornBoom.h"
#include "FxHealEffects.h"
#include "FxWorldCup.h"
#include "FxWorldCupBoom.h"
#include "FxGuardTower.h"
#include "FxShilonMaster.h"
#include "FxLightBallBoomNew.h"
#include "FxControlLight.h"
#include "FxCatchFlag.h"
#include "FxGreenSmoke.h"
#include "FxConditionEffects.h"
#include "FxOptionSkillEffects.h"
#include "FxHitPowerEffects.h"
#include "FxEarthQuake.h"
#include "FxFrost.h"
#include "FxChakram.h"
#include "FxBlackHole.h"
#include "FxScrollRing.h"
#include "FxSkillLight.h"
#include "FxScrollLight.h"
#include "FxScrollDamage.h"
#include "FxChain.h"
#include "FxOnOffEffects.h"
#include "FxScanView.h"
#include "FxChargeCannon.h"
#include "FxFlame.h"
#include "FxAutoLaser.h"
#include "FxLifeCommon.h"
#include "FxLifeLight.h"
#include "FxMagicArrow.h"
#include "FxMagicLaser.h"
#include "FxFireBoomShort.h"
#include "FxRocket.h"
#include "FxMeteorite.h"
#include "FxMeteoriteBoom.h"
#include "FxChaos.h"
#include "FxGatheringGun.h"
#include "FxDragonEffects.h"
#include "FxNuclearAim.h"
#include "FxNuclearBoom.h"
#include "FxDeathWaveAim.h"
#include "FxCharWave.h"
#include "FxSprayParticle.h"
#include "FxTurnParticle.h"
#include "FxLightParticle.h"
#include "FxHitPetEffects.h"
#include "FxKhanHitEffects.h"
#include "FxTeleport.h"
#include "FxWeaponEffects.h"
#include "FxRunSmoke.h"
#include "FxWarGiantMulti.h"
#include "FxWarGiantAttack.h"
#include "FxWarGiantDie.h"
#include "FxMobShowUp.h"
#include "FxPowerUp.h"
#include "FxMobParticle.h"
#include "FxGhostHunt.h"
#include "FxGhostRise.h"
#include "FxSoulDust.h"
#include "FxMagicSword.h"
#include "FxVampire.h"
#include "FxVampireAttack.h"
#include "FxMusicalNote.h"
#include "FxFallOffStone.h"
#include "FxDrillStone.h"
#include "FxStoneRance2.h"
#include "FxCatchFlagDir.h"
#include "FxDropCandy.h"
#include "FxHellCry.h"
#include "FxAirRaid.h"
#include "FxTornadoRuin.h"
#include "FxSumPart.h"
#include "FxFireworks.h"		// FireWork!!(Lyul)
#include "FxDStone.h"
#include "Fxbash.h"
#include "FxDSlash.h"
#include "FxDStoneEff.h"
#include "FxBleed.h"
#include "FxAssault.h"
#include "FxDemiCrusher.h"
#include "FxReflection.h"
#include "FxFireTower.h"
#include "FxStoneBrust.h"
#include "HellDown_Circle.h"		//mungmae-2005/07/04
#include "FxHellDown_Circle2.h"		//mungmae-2005/07/04
#include "FxHellDown_Around.h"		//mungmae-2005/07/04
#include "FxHellDown_Lightly.h"		//mungmae-2005/07/04
#include "FxHellDown_Spark.h"		//mungmae-2005/07/04
#include "FxHellDown_Lightning.h"	//mungmae-2005/07/04
#include "FxMine.h"					// nate 2005-06-15 : 휴먼 스킬 추가
#include "FxLifeExpansion.h"		// nate 2005-07-06 : Life Expansion
#include "FxSummonSquad.h"
#include "FxDurant.h"				// nate 2005-09-26 : 듀란트 - 광선
#include "FxDurantTarget.h"			// nate 2005-09-26 : 듀란트 - 공격 대상
#include "FxPoison.h"				// nate 2005-11-03 : 포이즌
#include "FxAdrenaline.h"			// mungmae 2005/11/21 : 하이브리더 - 아드레날린
#include "FxAttackMoveLolo.h"		// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
#include "FxAttackFixLolo.h"		// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
#include "FxFlyingCrash.h"			// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
#include "FxFlyingCrash2.h"			// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
#include "FxShadowSlash.h"			// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
#include "FxShadowSlash2.h"			// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
#include "FxFollow.h"                ///페어리
#include "FxSpeedUpSmoke.h"
#include "FxSpiritMode.h"
#include "FxSpiritMode2.h"
#include "FxSpaceSlash.h"
#include "FxElectricHammer.h"
#include "FxSummonFire.h"
#include "FxSummonFireBoom.h"
#include "FxRandomFire.h"
#include "FxRandomShoot.h"
#include "FxTLVertex.h"
#include "FxPetSkill.h"
#include "FxFlameSpear.h" ///플레임스피어
#include "FxMercernerySlash.h"
#include "FxIceBlot.h"
#include "FxProtectionShield.h"
#include "FxWildSmash.h"
#include "FxResurrection.h"
#include "FxTornadoHammer.h"
#include "FxShieldBlock.h"
#include "FxPoisonFog.h" ///인던1
#include "FxParticle.h" ///하트로드
#include "FxFreePvpRing.h" ///자유대련
#include "FxWaterSprinkle.h"		// [3/26/2007 Theodoric] 태국 송크란 이벤트 
#include "FxWaterSprinkleBoom.h"	// [3/26/2007 Theodoric] 태국 송크란 이벤트
#include "FxBabySoul.h"				// [8/7/2007 Theodoric] 7주년 기념 이벤트 
#include "FxMuramasa.h" ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트 용
#include "FxDmitronEffects.h"
#include "FxBalloonKill.h"
#include "UIMgr.h"
#include "Skill.h"		// nate 2005-06-27 : Auto Mine - g_ChaSkill 참조 관련

#include "FxElectricParticle.h"
#include "FxClystalGlow.h"
#include "FxWaterfall.h"
#include "FxWaterfallParticle.h"
#include "FxPortalBOSSRAID.h"
#include "FxFireSmokeParticle.h"

#include "Fx_BossRaid_Bulkan1_02.h"
#include "Fx_BossRaid_Bulkan1_03.h"
#include "Fx_BossRaid_Bulkan2_02.h"
#include "Fx_BossRaid_Bulkan3_02.h"
#include "Fx_BossRaid_Bulkan3_03.h"

#include "Fx_BossRaid_human1_02.h"
#include "Fx_BossRaid_human1_03.h"
#include "Fx_BossRaid_Human2_02.h"
#include "Fx_BossRaid_Human2_03.h"
#include "Fx_BossRaid_Human3_02.h"	// [12/25/2007 반재승] 보스 레이드 휴먼보스3-02 스킬

#include "Fx_BossRaid_Hybri1_01.h"
#include "Fx_BossRaid_Hybri1_02.h"
#include "Fx_BossRaid_Hybri2_02.h"
#include "Fx_BossRaid_Hybri3_01.h"	// [1/16/2008 반재승] 보스 레이드 하이브리더보스3-01 노멀어택
#include "Fx_BossRaid_Hybri3_02.h"	// [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

#include "Fx_BossRaid_Kai1_01.h"
#include "Fx_BossRaid_Kai1_02.h"
#include "Fx_BossRaid_Kai2_01.h"	// [2/5/2008 반재승] 카이립톤보스2 1번 어택
#include "Fx_BossRaid_Kai2_02.h"	// [2/5/2008 반재승] 카이립톤보스2 2번 어택
#include "Fx_BossRaid_Kai2_03.h"	// [2/5/2008 반재승] 카이립톤보스2 3번 어택
#include "Fx_BossRaid_Kai3_01.h"	// [2/4/2008 반재승] 카이립톤보스3 노멀 어택
#include "Fx_BossRaid_Kai3_02.h"	// [2/4/2008 반재승] 카이립톤보스3 2번스킬
#include "Fx_BossRaid_Kai3_03.h"	// [2/11/2008 반재승] 카이립톤보스3 3번스킬

#include "Fx_BossRaid_Aidia1_01.h" // Aidia
#include "Fx_BossRaid_Aidia1_02.h" // Aidia
#include "Fx_BossRaid_Aidia1_02_shockwave.h" // Aidia

#include "Fx_BossRaid_Aidia2_01.h"
#include "Fx_BossRaid_Aidia2_02.h"
#include "Fx_BossRaid_Aidia2_03.h"

#include "Fx_BossRaid_Aidia3_01.h"	// [3/6/2008 반재승] 에이디아보스3 노멀 어택
#include "Fx_BossRaid_Aidia3_02.h"	// [3/6/2008 반재승] 에이디아보스3 2번 스킬

#include "Fx_BossRaid_LastBoss_02.h"
#include "Fx_BossRaid_LastBoss_03.h"
#include "Fx_BossRaid_LastBoss_04.h"

#include "Fx_HeroEffect.h"	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트 

#include "fxarupakaspit.h"

#include "FxLv400Effect.h"

#include "LHFX_Perom_Spin.h"
#include "LHFX_Perom_Thrust.h"
#include "LHFX_Perom_Blader.h"
#include "LHFX_Perom_Pierce.h"
#include "LHFX_Perom_Boom.h"
#include "LHFX_Perom_Shotgun.h"
#include "LHFX_Perom_Field.h"
#include "LHFX_Perom_Wheelwind.h"
#include "LHFX_Perom_Strike.h"
#include "LHFX_Perom_Cross.h"
#include "LHFX_Perom_Meta.h"
#include "LHFx_Perom_DSystem.h"
#include "LHFx_Perom_Overload.h"
#include "LHFx_Perom_Alpha.h"

#include "FxCommonLolo.h"

extern int g_EffectSound[];
extern BOOL	g_bDungeon;
extern float g_fTimeKey;
extern float g_fDSDistance;
// nate 2005-06-27 : Auto Mine - 스킬 포인트 참조
// nate 2004 - 7 글로벌 ShadowRect사용

extern BOOL g_bUseMatrixCountry; // 매트릭스(뫼비우스 아레나)가 도입된 나라인가의 플래그.(도입 안된나라는 뫼비우스 데이터를 패치 안해주기 때문에 이게 FALSE여야한다.) ///해외뫼비우스

///////////////////////////////////////////////////////////////////////////////////////////
// helper functions
bool operator<(const EffectSort &r1, const EffectSort &r2)
{
	if (r1.vSortPos.z < r2.vSortPos.z)
		return true;
	else if (r1.vSortPos.z == r2.vSortPos.z)
		return true;
	else
		return FALSE;
}

bool descending(const EffectSort &r1, const EffectSort &r2)
{
	if (r1.vSortPos.z > r2.vSortPos.z)
		return true;
	else if (r1.vSortPos.z == r2.vSortPos.z)
		return true;
	else
		return FALSE;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSet::CFxSet()
{
	m_nEfIdx = 0;					// 자동증가 인덱스.
	m_pFxLight = new CFxLight;
	m_pFxPortal = new CFxPortal;
	m_pFxSmoke = new CFxSmoke;
	m_pFxShadowRing = new CFxShadowRing;
	m_pFxShadowLight = new CFxShadowLight;
	m_pFxParticle = new CFxParticle;
	m_pFxLightFog = new CFxLightFog;
	m_pFxShowup = new CFxShowup;
	m_pFxLightning = new CFxLightning;
	m_pFxLightBall = new CFxLightBall;
	m_pFxFireBall = new CFxFireBall;
	m_pFxFireBallBoom = new CFxFireBallBoom;
	m_pFxFireBallBoom2 = new CFxFireBallBoom2;
	m_pFxLwLight = new CFxLwLight;
	m_pFxLightBallBoom = new CFxLightBallBoom;
	m_pFxUnarmedAttacked = new CFxUnarmedAttacked;
	m_pFxMeteor	= new CFxMeteor;
	m_pFxPebbleShot = new CFxPebbleShot;
	m_pFxStoneShot = new CFxStoneShot;
	m_pFxStoneShotBoom = new CFxStoneShotBoom;
	m_pFxPebbleShotBoom = new CFxPebbleShotBoom;
	m_pFxShoot	= new CFxShoot;
	m_pFxShootAttacked = new CFxShootAttacked;
	m_pFxAidiaFire = new CFxAidiaFire;
	m_pFxAidiaBallBoom = new CFxAidiaBallBoom;
	m_pFxLightHeal = new CFxLightHeal;
	m_pFxStarFlare = new CFxStarFlare;
	m_pFxStarFlareBoom = new CFxStarFlareBoom;
	m_pFxShockSpear = new CFxShockSpear;
	m_pFxStoneRance = new CFxStoneRance;
	m_pFxRedFire = new CFxRedFire;
	m_pFxFly = new CFxFly;
	m_pFxBee = new CFxBee;
	m_pFxBeeBoom = new CFxBeeBoom;
	m_pFxBora = new CFxBora;
	m_pFxBoraBoom = new CFxBoraBoom;
	m_pFxMoth = new CFxMoth;
	m_pFxShilon = new CFxShilon;
	m_pFxHorn	= new CFxHorn;
	m_pFxSilverFly = new CFxSilverFly;
	m_pFxRaven = new CFxRaven;
	m_pFxKines = new CFxKines;
	m_pFxNeo = new CFxNeo;
	m_pFxKaisen = new CFxKaisen;
	m_pFxHitEffects = new CFxHitEffects;
	m_pFxHitBoomEffects = new CFxHitBoomEffects;
	m_pFxLevelUp = new CFxLevelUp;
	m_pFxShowUp2 = new CFxShowUp2;
	m_pFxEatEffects = new CFxEatEffects;
	m_pFxShield = new CFxShield;
	m_pFxSmokeParticle = new CFxSmokeParticle;
	m_pFxFireParticle = new CFxFireParticle;
	m_pFxHornBoom = new CFxHornBoom;
	m_pFxHealEffects = new CFxHealEffects;
	m_pFxWorldCup = new CFxWorldCup;
	m_pFxWorldCupBoom = new CFxWorldCupBoom;
	m_pFxGuardTower = new CFxGuardTower;
	m_pFxShilonMaster = new CFxShilonMaster;
	m_pFxLightBallBoomNew = new CFxLightBallBoomNew;
	m_pFxControlLight = new CFxControlLight;
	m_pFxCatchFlag = new CFxCatchFlag;
	m_pFxGreenSmoke = new CFxGreenSmoke;
	m_pFxConditionEffects = new CFxConditionEffects;
	m_pFxConditionEffects2 = new CFxConditionEffects2;
	m_pFxOptionSkillEffects = new CFxOptionSkillEffects;
	m_pFxHitPowerEffects = new CFxHitPowerEffects;
	m_pFxEarthQuake = new CFxEarthQuake;
	m_pFxFrost = new CFxFrost;
	m_pFxChakram = new CFxChakram;
	m_pFxBlackHole = new CFxBlackHole;
	m_pFxScrollRing = new CFxScrollRing;
	m_pFxScrollDamage = new CFxScrollDamage;
	m_pFxSkillLight = new CFxSkillLight;
	m_pFxScrollLight = new CFxScrollLight;
	m_pFxChain = new CFxChain;
	m_pFxOnOffEffects = new CFxOnOffEffects;
	m_pFxScanView = new CFxScanView;
	m_pFxChargeCannon = new CFxChargeCannon;
	m_pFxFlame = new CFxFlame;
	m_pFxAutoLaser = new CFxAutoLaser;
	m_pFxLifeCommon = new CFxLifeCommon;
	m_pFxLifeLight = new CFxLifeLight;
	m_pFxMagicArrow = new CFxMagicArrow;
	m_pFxMagicLaser = new CFxMagicLaser;
	m_pFxFireBoomShort = new CFxFireBoomShort;
	m_pFxRocket = new CFxRocket;
	m_pFxMeteorite = new CFxMeteorite;
	m_pFxMeteoriteBoom = new CFxMeteoriteBoom;
	m_pFxChaos = new CFxChaos;
	m_pFxGatheringGun = new CFxGatheringGun;
	m_pFxDragonEffects = new CFxDragonEffects;
	m_pFxNuclearAim = new CFxNuclearAim;
	m_pFxNuclearBoom = new CFxNuclearBoom;
	m_pFxDeathWaveAim = new CFxDeathWaveAim;
	m_pFxCharWave = new CFxCharWave;
	m_pFxPhenix = new CFxPhenix;
	m_pFxSprayParticle = new CFxSprayParticle;
	m_pFxTurnParticle = new CFxTurnParticle;
	m_pFxLightParticle = new CFxLightParticle;
	m_pFxHitPetEffects = new CFxHitPetEffects;
	m_pFxKhanHitEffects = new CFxKhanHitEffects;
	m_pFxTeleport = new CFxTeleport;
	m_pFxWeaponEffects = new CFxWeaponEffects;
	m_pFxRunSmoke = new CFxRunSmoke;
	m_pFxWarGiantMulti = new CFxWarGiantMulti;
	m_pFxWarGiantAttack = new CFxWarGiantAttack;
	m_pFxWarGiantDie = new CFxWarGiantDie;
	m_pFxMobShowUp = new CFxMobShowUp;
	m_pFxPowerUp = new CFxPowerUp;
	m_pFxGryphon = new CFxGryphon;
	m_pFxDragon = new CFxDragon;
	m_pFxMobParticle = new CFxMobParticle;
	m_pFxGhostHunt = new CFxGhostHunt;
	m_pFxGhostRise = new CFxGhostRise;
	m_pFxSoulDust = new CFxSoulDust;
	m_pFxMagicSword = new CFxMagicSword;
	m_pFxSphinx = new CFxSphinx;
	m_pFxVampire = new CFxVampire;
	m_pFxVampireAttack = new CFxVampireAttack;
	m_pFxMusicalNote = new CFxMusicalNote;
	m_pFxFallOffStone = new CFxFallOffStone;
	m_pFxDrillStone = new CFxDrillStone;
	m_pFxStoneRance2 = new CFxStoneRance2;
	m_pFxCatchFlagDir = new CFxCatchFlagDir;
	m_pFxDropCandy = new CFxDropCandy; // DropCandy
	m_pFxDropApple = new CFxDropCandy; // DropApple

	m_pFxDropApplepvp = new CFxDropCandy;
	m_pFxDropCandyvip = new CFxDropCandy;
	m_pFxDropCandypvp = new CFxDropCandy;




	m_pFxDropStarCandy = new CFxDropStarCandy; // 별사탕 이벤트

	//중고렙 스킬 추가
	m_pFxHellCry	= new CFxHellCry;
	m_pFxAirRaid	= new CFxAirRaid;
	m_pFxTornadoRuin = new CFxTornadoRuin;
	m_pFxSumPart = new CFxSumPart;
	//에이디아 데빌링 플라잉데빌킹 이펙트
	m_pFxDevil = new CFxDevil;
	m_pFxSarngaMouel = new CFxSarngaMouel;	// [12/3/2008 Theodoric] 사릉가
	m_fx_yeti_ring		= new LHFX_YetiRing;	// AIDIA LEVEL 450
	m_fx_peakock_ring	= new LHFX_PeakockRing; // AIDIA LEVEL 500
	m_fx_tiger_ring		= new LHFX_TigerRing;	// AIDIA LEVEL 550
	m_fx_unicorn_ring	= new LHFX_UnicornRing; // AIDIA LEVEL 600
	m_fx_650_ring	= new LHFX_650Ring; // AIDIA LEVEL 650
	m_fx_700_ring	= new LHFX_700Ring; // AIDIA LEVEL 700
	m_fx_750_ring	= new LHFX_750Ring; // AIDIA LEVEL 750
	m_fx_800_ring	= new LHFX_800Ring; // AIDIA LEVEL 800
	m_fx_850_ring	= new LHFX_850Ring; // AIDIA LEVEL 850
	m_fx_900_ring	= new LHFX_900Ring; // AIDIA LEVEL 900
	m_fx_950_ring	= new LHFX_950Ring; // AIDIA LEVEL 950
	m_fx_1000_ring	= new LHFX_1000Ring; // AIDIA LEVEL 1000
	m_fx_CHR_ring	= new LHFX_CHRRing; // AIDIA RING CHRISTIMAS
	m_fx_CHIN_ring	= new LHFX_CHINRing; // AIDIA RING CHINESE EVENT
	m_pFxSummonRing = new CFxSummonRing;   ///불칸링
	m_pFxFireWorks = new CFxFireWorks;	// FireWork!!(Lyul)
	m_pFxDstone = new CFxDstone;				// Lyul_2005
	m_pFxDslash= new CFxDSlash;					// Lyul_2005
	m_pFxBash= new CFxBash;							// Lyul_2005
	m_pFxDstoneEff= new CFxDStoneEff;		// Lyul_2005
	m_pFxbleed = new CFxBleed;					// Lyul_2005
	m_pFxAssault = new CFxAssault;			// Lyul_2005
	m_pFxDemiCrusher = new CFxDemiCrusher;
	m_pFxFireTower= new CFxFireTower;
	m_pFxStoneBrust = new CFxStoneBrust;
	m_pFxReflection = new CFxReflection;
	m_pFxMine = new CFxMine;	// nate 2005-06-15 : Auto Mine - 메모리 할당
	m_pFxHellDown_Circle = new CFxHellDown_Circle;	   //mungmae 2005/07/04 : HellDown
	m_pFxHellDown_Circle2 = new CFxHellDown_Circle2;	   //mungmae 2005/07/04 : HellDown
	m_pFxHellDown_Around = new CFxHellDown_Around;	   //mungmae 2005/07/04 : HellDown
	m_pFxHellDown_Lightly = new CFxHellDown_Lightly;     //mungmae 2005/07/04 : HellDown
	m_pFxHellDown_Spark = new CFxHellDown_Spark;		   //mungmae 2005/07/04 : HellDown
	m_pFxHellDown_Lightning = new CFxHellDown_Lightning; //mungmae 2005/07/04 : HellDown
	m_pFxLifeExpansion = new CFxLifeExpansion;		// nate 2005-07-06 : Life Expansion
	m_pFxSummonSquad = new CFxSummonSquad;			// nate 2005-07-08 : Summon Squad
	m_pFxDurant_Beam = new CFxDurant;				// nate 2005-09-26 : 듀란트 - 광선
	m_pFxDurant_Target = new CFxDurantTarget;		// nate 2005-09-26 : 듀란트 - 공격 대상
	m_pFxPoison = new CFxPoison;	// nate 2005-11-03 : 포이즌
	m_pFxAdrenaline = new CFxAdrenaline; // mungmae 2005/11/21 : 하이브리더 - 아드레날린
	m_pFxAttack_MoveLolo = new CFxAttack_MoveLolo;	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	m_pFxFlyingCrash = new CFxFlyingCrash;			// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
	m_pFxFlyingCrash2 = new CFxFlyingCrash2;		// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
	m_pFxAttack_FixLolo = new CFxAttackFixLolo;	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	m_pFxShadowSlash = new CFxShadowSlash;		// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
	m_pFxShadowSlash2 = new CFxShadowSlash2;		// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
	m_pFxFollow = new CFxFollow;						///페어리
	m_pFxMuramasaGhost = new CFxMuramasaGhost;						///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
	m_pFxFlameSpear = new CFxFlameSpear;				// 카이 300레벨 스킬 ///플레임스피어
	m_pFxPetSkill = new CFxPetSkill;					// 진화형 펫의 스킬 이펙트
	m_pFxSpeedUpSmoke = new CFxSpeedUpSmoke;
	m_pFxSpiritMode = new CFxSpiritMode;        //스피릿
	m_pFxSpiritMode2 = new CFxSpiritMode2;        //스피릿
	m_pFxTLVertex = new CFxTLVertex;
	m_pFxSpaceSlash	= new CFxSpaceSlash;
	m_pFxElectricHammer = new CFxElectricHammer;
	m_pFxSummonFire = new CFxSummonFire;
	m_pFxSummonFireBoom = new CFxSummonFireBoom;
	m_pFxRandomFire = new CFxRandomFire;
	m_pFxRandomShoot = new CFxRandomShoot;
	m_pFxMercernerySlash = new CFxMercernerySlash;
	m_pFxIceBlot = new CFxIceBlot;
	m_pFxProtectionShield = new CFxProtectionShield;
	m_pFxShieldBlock = new CFxShieldBlock;
	m_pFxWildSmash = new CFxWildSmash;
	m_pFxResurrection = new CFxResurrection;
	m_pFxTornadoHammer = new CFxTornadoHammer;
	m_pFxPoisonFog = new CFxPoisonFog; ///인던1
	m_pFxTypeParticle = new CFxTypeParticle; ///하트로드
	m_pFxFreePvpRing = new CFxFreePvpRing;        // 자유대련의 링. ///자유대련
	m_pFxWTRSprinkle = new CFxWaterSprinkle;			// [3/26/2007 Theodoric] 태국 송크란 이벤트
	m_pFxWTRSprinkleBoom = new CFxWaterSprinkleBoom;	// [3/26/2007 Theodoric] 태국 송크란 이벤트
	m_pFxBabySoul	= new CFxBabySoul;					// [8/7/2007 Theodoric] 6주년 기념 이벤트
	m_pFxDmitronPortal = new CFxDmitronPortal; // 드미트론 점령전 - 이동 게이트
	m_pFxDmitronCrystal = new CFxDmitronCrystal; // 드미트론 점령전 - 크리스탈

	m_pFxElectricParticle	= new CFxElectricParticle;
	m_pFxClystalGlow		= new CFxClystalGlow;
	m_pFxWaterfall			= new CFxWaterfall;
	m_pFxWaterfallParticle	= new CFxWaterfallParticle;
	m_pFxLightningEx		= new CFxLightningEx;
	m_pFxPortalBossraid		= new CFxPortalBossraid;
	m_pFxFireSmokeParticle	= new CFxFireSmokeParticle;

	m_pFxBossRaidBulkan1_02 = new CFxBossRaidBulkan1_02;
	m_pFxBossRaidBulkan1_03 = new CFxBossRaidBulkan1_03;
	m_pFxBossRaidBulkan2_02 = new CFxBossRaidBulkan2_02;
	m_pFxBossRaidBulkan3_02 = new CFxBossRaidBulkan3_02;	// [11/28/2007 반재승] 보스 레이드 불칸보스3-02 스킬
	m_pFxBossRaidBulkan3_03 = new CFxBossRaidBulkan3_03;	// [11/28/2007 반재승] 보스 레이드 불칸보스3-03 스킬

	m_pFxBossRaidHuman1_02 = new CFxBossRaidHuman1_02;
	m_pFxBossRaidHuman1_03 = new CFxBossRaidHuman1_03;
	m_pFxBossRaidHuman2_02 = new CFxBossRaidHuman2_02;
	m_pFxBossRaidHuman2_03 = new CFxBossRaidHuman2_03;
	m_pFxBossRaidHuman3_02 = new CFxBossRaidHuman3_02;	// [12/13/2007 반재승] 보스 레이드 휴먼보스3-02 스킬

	m_pFxBossRaidHybri1_01 = new CFxBossRaidHybri1_01;
	m_pFxBossRaidHybri1_02 = new CFxBossRaidHybri1_02;
	m_pFxBossRaidHybri2_02 = new CFxBossRaidHybri2_02;
	m_pFxBossRaidHybri3_01 = new CFxBossRaidHybri3_01;	// [1/16/2008 반재승] 보스 레이드 하이브리더보스3-01 노멀어택
	m_pFxBossRaidHybri3_02 = new CFxBossRaidHybri3_02;	// [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

	m_pFxBossRaidKai1_01 = new CFxBossRaidKai1_01;
	m_pFxBossRaidKai1_02 = new CFxBossRaidKai1_02;
	m_pFxBossRaidKai2_01 = new CFxBossRaidKai2_01;	// [2/5/2008 반재승] 카이립톤보스2 1 어택
	m_pFxBossRaidKai2_02 = new CFxBossRaidKai2_02;	// [2/5/2008 반재승] 카이립톤보스2 2 어택
	m_pFxBossRaidKai2_03 = new CFxBossRaidKai2_03;	// [2/5/2008 반재승] 카이립톤보스2 3 어택
	m_pFxBossRaidKai3_01 = new CFxBossRaidKai3_01;	// [2/4/2008 반재승] 카이립톤보스3 노멀 어택
	m_pFxBossRaidKai3_02 = new CFxBossRaidKai3_02;	// [2/4/2008 반재승] 카이립톤보스3 2번스킬
	m_pFxBossRaidKai3_03 = new CFxBossRaidKai3_03;	// [2/11/2008 반재승] 카이립톤보스3 3번스킬

	m_pFxBossRaidAidia1_01 = new CFxBossRaidAidia1_01; // 에이디아 1_01 노말 어텍
	m_pFxBossRaidAidia1_02 = new CFxBossRaidAidia1_02; // 에이디아 1_01 노말 어텍
	m_pFxBossRaidAidia1_02_sw = new CFxBossRaidAidia1_02_Shockwave; // 에이디아 1_01 노말 어텍

	m_pFxBossRaidAidia2_01 = new CFxBossRaidAidia2_01; // 에이디아 2 1번 정상 공격
	m_pFxBossRaidAidia2_02 = new CFxBossRaidAidia2_02; // 에이디아 2 2번 특수 공격
	m_pFxBossRaidAidia2_03 = new CFxBossRaidAidia2_03; // 에이디아 2 3번 특수 공격

	m_pFxBossRaidAidia3_01 = new CFxBossRaidAidia3_01; // [3/6/2008 반재승] 에이디아보스3 노멀 어택
	m_pFxBossRaidAidia3_02 = new CFxBossRaidAidia3_02; // [3/6/2008 반재승] 에이디아보스3 2번 스킬

	m_pFxBossRaidLastBoss_02 = new CFxBossRaidLastBoss_02;
	m_pFxBossRaidLastBoss_03 = new CFxBossRaidLastBoss_03;
	m_pFxBossRaidLastBoss_04 = new CFxBossRaidLastBoss_04;

	m_pFxHeroEffect	= new CFxHeroEffect;	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트

	m_pFxArupakaSpit = new FxArupakaSpit;

	m_pFxLv400Effect = new CFxLv400Effect;

	m_fx_perom_spin			= new LHFX_Perom_Spin;
	m_fx_perom_thurst		= new LHFX_Perom_Thrust;
	m_fx_perom_blader		= new LHFX_Perom_Blader;
	m_fx_perom_pierce		= new LHFX_Perom_Pierce;
	m_fx_perom_boom			= new LHFX_Perom_Boom;
	m_fx_perom_shotgun		= new LHFX_Perom_Shotgun;
	m_fx_perom_field		= new LHFX_Perom_Field;
	m_fx_perom_wheelwind	= new LHFX_Perom_Wheelwind;
	m_fx_perom_strike		= new LHFX_Perom_Strike;
	m_fx_perom_cross		= new LHFX_Perom_Cross;
	m_fx_perom_meta			= new LHFX_Perom_Meta;
	m_fx_perom_dsystem		= new LHFx_Perom_DSystem;
	m_fx_perom_overload		= new LHFx_Perom_Overload;
	m_fx_perom_alpha		= new LHFx_Perom_Alpha;

	m_fx_griffon_ring		= new LHFX_GriffonRing;

	m_pFxBalloonKill		= new CFxBalloonKill;

	m_pFxBossAttack01		= new CFxCommonLolo;
	m_pFxBossAttack01->SetFilename("BOSS_K02_ATTACK02_EFF.LOL");
	m_pFxBossAttack02		= new CFxCommonLolo;
	m_pFxBossAttack02->SetFilename("EFF_ADIA_ATTACK02.LOL");
	m_pFxBossDebuff			= new CFxCommonLolo;
	m_pFxBossDebuff->SetFilename("BOSS_K01_ATTACK02_EFF.LOL");
	m_pFxBossMagic			= new CFxCommonLolo;
	m_pFxBossMagic->SetFilename("BOSS_K03_ATTACK02_EFF.LOL");

//========================================================================================
	m_pFrameCount = new int[FX_NUM];	// allocation memory of max count
	assert(m_pFrameCount!=NULL);

	m_pFrameCount[FX_SMALL_LIGHT] = 20;
	m_pFrameCount[FX_PORTAL] = 20;
	m_pFrameCount[FX_SMOKE] = 20;
	m_pFrameCount[FX_SHADOWRING] = 20;
	m_pFrameCount[FX_SHADOWLIGHT] = 20;
	m_pFrameCount[FX_PARTICLE] = 20;
	m_pFrameCount[FX_LIGHTFOG] = 20;
	m_pFrameCount[FX_SHOWUP] = FX_SHOWUP_FRAMECOUNT;
	m_pFrameCount[FX_LIGHTNING] = FX_LIGHTNING_FRAMECOUNT;
	m_pFrameCount[FX_LIGHTBALL] = FX_LIGHTBALL_FRAMECOUNT;
	m_pFrameCount[FX_FIREBALL] = FX_FIREBALL_FRAMECOUNT;
	m_pFrameCount[FX_FIREBALLBOOM] = FX_FIREBALLBOOM_FRAMECOUNT;
	m_pFrameCount[FX_FIREBALLBOOM2] = FX_FIREBALLBOOM2_FRAMECOUNT;
	m_pFrameCount[FX_LWLIGHT] = FX_LWLIGHT_FRAMECOUNT;
	m_pFrameCount[FX_LIGHTBALLBOOM] = FX_LIGHTBALLBOOM_FRAMECOUNT;
	m_pFrameCount[FX_UNARMEDATTACKED] = FX_UNARMEDATTACKED_FRAMECOUNT;
	m_pFrameCount[FX_METEOR] = FX_METEOR_FRAMECOUNT;
	m_pFrameCount[FX_PEBBLESHOT] = FX_PEBBLESHOT_FRAMECOUNT;
	m_pFrameCount[FX_STONESHOT] = FX_STONESHOT_FRAMECOUNT;
	m_pFrameCount[FX_FSHOWER] = FX_FSHOWER_FRAMECOUNT;
	m_pFrameCount[FX_STONESHOTBOOM] = FX_STONESHOTBOOM_FRAMECOUNT;
	m_pFrameCount[FX_PEBBLESHOTBOOM] = FX_PEBBLESHOTBOOM_FRAMECOUNT;
	m_pFrameCount[FX_SHOOT] = FX_SHOOT_FRAMECOUNT;
	m_pFrameCount[FX_SHOOTATTACKED] = FX_SHOOTATTACKED_FRAMECOUNT;
	m_pFrameCount[FX_AIDIAFIRE] = FX_AIDIAFIRE_FRAMECOUNT;
	m_pFrameCount[FX_AIDIABALLBOOM] = FX_AIDIABALLBOOM_FRAMECOUNT;
	m_pFrameCount[FX_LIGHTHEAL] = FX_LIGHTHEAL_FRAMECOUNT;
	m_pFrameCount[FX_STARFLARE] = FX_STARFLARE_FRAMECOUNT;
	m_pFrameCount[FX_STARFLAREBOOM] = FX_STARFLAREBOOM_FRAMECOUNT;
	m_pFrameCount[FX_SHOCKSPEAR] = FX_SHOCKSPEAR_FRAMECOUNT;
	m_pFrameCount[FX_STONERANCE] = FX_STONERANCE_FRAMECOUNT;
	m_pFrameCount[FX_REDFIRE] = FX_REDFIRE_FRAMECOUNT;
	m_pFrameCount[FX_FLY] = FX_FLY_FRAMECOUNT;
	m_pFrameCount[FX_BEE] = FX_BEE_FRAMECOUNT;
	m_pFrameCount[FX_BEEBOOM] = FX_BEEBOOM_FRAMECOUNT;
	m_pFrameCount[FX_BORA] = FX_BORA_FRAMECOUNT;
	m_pFrameCount[FX_BORABOOM] = FX_BORABOOM_FRAMECOUNT;
	m_pFrameCount[FX_MOTH] = FX_MOTH_FRAMECOUNT;
	m_pFrameCount[FX_SHILON] = FX_SHILON_FRAMECOUNT;
	m_pFrameCount[FX_HORN] = FX_HORN_FRAMECOUNT;
	m_pFrameCount[FX_SILVERFLY] = FX_SILVERFLY_FRAMECOUNT;
	m_pFrameCount[FX_RAVEN] = FX_RAVEN_FRAMECOUNT;
	m_pFrameCount[FX_KINES] = FX_KINES_FRAMECOUNT;
	m_pFrameCount[FX_NEO] = FX_NEO_FRAMECOUNT;
	m_pFrameCount[FX_KAISEN] = FX_KAISEN_FRAMECOUNT;
	m_pFrameCount[FX_HITEFFECTS] = FX_HITEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_HITBOOMEFFECTS] = FX_WEAPONEFF_FRAMECOUNT;
	m_pFrameCount[FX_LEVELUP] = FX_LEVELUP_FRAMECOUNT;
	m_pFrameCount[FX_SHOWUP2] = FX_SHOWUP2_FRAMECOUNT;
	m_pFrameCount[FX_EATEFFECTS] = FX_EATEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_SHIELD] = FX_SHIELD_FRAMECOUNT;
	m_pFrameCount[FX_SMOKEPARTICLE] = FX_SMOKEPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_FIREPARTICLE] = FX_FIREPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_HORNBOOM] = FX_HORNBOOM_FRAMECOUNT;
	m_pFrameCount[FX_HEALEFFECTS] = FX_HEALEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_WORLDCUP] = FX_WORLDCUP_FRAMECOUNT;
	m_pFrameCount[FX_WORLDCUPBOOM] = FX_WORLDCUPBOOM_FRAMECOUNT;
	m_pFrameCount[FX_GUARDTOWER] = FX_GUARDTOWER_FRAMECOUNT;
	m_pFrameCount[FX_SHILONMASTER] = FX_SHILONMASTER_FRAMECOUNT;
	m_pFrameCount[FX_LIGHTBALLBOOMNEW] = FX_LIGHTBALLBOOMNEW_FRAMECOUNT;
	m_pFrameCount[FX_CONTROL_LIGHT] = FX_CONTROL_LIGHT_FRAMECOUNT;
	m_pFrameCount[FX_CATCHFLAG] = FX_CATCHFLAG_FRAMECOUNT;
	m_pFrameCount[FX_GREENSMOKE] = FX_GREENSMOKE_FRAMECOUNT;
	m_pFrameCount[FX_CONDITIONEFFECTS] = FX_CONDITIONEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_CONDITIONEFFECTS2] = FX_CONDITIONEFFECTS2_FRAMECOUNT;
	m_pFrameCount[FX_OPTIONSKILLEFFECTS] = FX_OPTIONSKILLEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_HITPOWEREFFECTS] = FX_HITPOWEREFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_EARTHQUAKE] = FX_EARTHQUAKE_FRAMECOUNT;
	m_pFrameCount[FX_FROST] = FX_FROST_FRAMECOUNT;
	m_pFrameCount[FX_CHAKRAM] = FX_CHAKRAM_FRAMECOUNT;
	m_pFrameCount[FX_BLACKHOLE] = FX_BLACKHOLE_FRAMECOUNT;
	m_pFrameCount[FX_SCROLLRING] = FX_SCROLLRING_FRAMECOUNT;
	m_pFrameCount[FX_SCROLLDAMAGE] = FX_SCROLLDAMAGE_FRAMECOUNT;
	m_pFrameCount[FX_SKILL_LIGHT] = FX_SKILL_LIGHT_FRAMECOUNT;
	m_pFrameCount[FX_SCROLL_LIGHT] = FX_SCROLL_LIGHT_FRAMECOUNT;
	m_pFrameCount[FX_WAND_CHAIN] = FX_WAND_CHAIN_FRAMECOUNT;
	m_pFrameCount[FX_ONOFFEFFECTS] = FX_ONOFFEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_SCANVIEW] = FX_SCANVIEW_FRAMECOUNT;
	m_pFrameCount[FX_CHARGECANNON] = FX_CHARGECANNON_FRAMECOUNT;
	m_pFrameCount[FX_FLAME] = FX_FLAME_FRAMECOUNT;
	m_pFrameCount[FX_AUTOLASER] = FX_AUTOLASER_FRAMECOUNT;
	m_pFrameCount[FX_LIFECOMMON] = FX_LIFECOMMON_FRAMECOUNT;
	m_pFrameCount[FX_LIFELIGHT] = FX_LIFELIGHT_FRAMECOUNT;
	m_pFrameCount[FX_MAGICARROW] = FX_MAGICARROW_FRAMECOUNT;
	m_pFrameCount[FX_MAGICLASER] = FX_MAGICLASER_FRAMECOUNT;
	m_pFrameCount[FX_FIREBOOMSHORT] = FX_FIREBOOMSHORT_FRAMECOUNT;
	m_pFrameCount[FX_ROCKET] = FX_ROCKET_FRAMECOUNT;
	m_pFrameCount[FX_METEORITE] = FX_METEORITE_FRAMECOUNT;
	m_pFrameCount[FX_METEORITEBOOM] = FX_METEORITEBOOM_FRAMECOUNT;
	m_pFrameCount[FX_CHAOS] = FX_CHAOS_FRAMECOUNT;
	m_pFrameCount[FX_GATHERINGGUN] = FX_GATHERINGGUN_FRAMECOUNT;
	m_pFrameCount[FX_DRAGONEFFECTS] = FX_DRAGONEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_NUCLEARAIM] = FX_NUCLEARAIM_FRAMECOUNT;
	m_pFrameCount[FX_NUCLEARBOOM] = FX_NUCLEARBOOM_FRAMECOUNT;
	m_pFrameCount[FX_DEATHWAVEAIM] = FX_DEATHWAVEAIM_FRAMECOUNT;
	m_pFrameCount[FX_CHARWAVE] = FX_CHARWAVE_FRAMECOUNT;
	m_pFrameCount[FX_PHENIX] = FX_PHENIX_FRAMECOUNT;
	m_pFrameCount[FX_SPRAYPARTICLE] = FX_SPRAYPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_TURNPARTICLE] = FX_TURNPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_LIGHTPARTICLE] = FX_LIGHTPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_HITPETEFFECTS] = FX_HITPETEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_KHANHITEFFECTS] = FX_KHANHITEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_TELEPORT] = FX_TELEPORT_FRAMECOUNT;
	m_pFrameCount[FX_WEAPONEFFECTS] = FX_WEAPONEFFECTS_FRAMECOUNT;
	m_pFrameCount[FX_RUNSMOKE] = FX_RUNSMOKE_FRAMECOUNT;
	m_pFrameCount[FX_WARGIANTMULTI] = FX_WARGIANTMULTI_FRAMECOUNT;
	m_pFrameCount[FX_WARGIANTATTACK] = FX_WARGIANTATTACK_FRAMECOUNT;
	m_pFrameCount[FX_WARGIANTDIE] = FX_WARGIANTDIE_FRAMECOUNT;
	m_pFrameCount[FX_MOBSHOWUP] = FX_MOBSHOWUP_FRAMECOUNT;
	m_pFrameCount[FX_POWERUP] = FX_POWERUP_FRAMECOUNT;
	m_pFrameCount[FX_GRYPHON] = FX_GRYPHON_FRAMECOUNT;
	m_pFrameCount[FX_DRAGON] = FX_DRAGON_FRAMECOUNT;
	m_pFrameCount[FX_MOBPARTICLE] = FX_MOBPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_GHOSTHUNT] = FX_GHOSTHUNT_FRAMECOUNT;
	m_pFrameCount[FX_GHOSTRISE] = FX_GHOSTRISE_FRAMECOUNT;
	m_pFrameCount[FX_SOULDUST] = FX_SOULDUST_FRAMECOUNT;
	m_pFrameCount[FX_MAGICSWORD] = FX_MAGICSWORD_FRAMECOUNT;
	m_pFrameCount[FX_SPHINX] = FX_SPHINX_FRAMECOUNT;
	m_pFrameCount[FX_VAMPIRE] = FX_VAMPIRE_FRAMECOUNT;
	m_pFrameCount[FX_VAMPIREATTACK] = FX_VAMPIREATTACK_FRAMECOUNT;
	m_pFrameCount[FX_MUSICALNOTE] = FX_MUSICALNOTE_FRAMECOUNT;
	m_pFrameCount[FX_FALLOFFSTONE] = FX_FALLOFFSTONE_FRAMECOUNT;
	m_pFrameCount[FX_STONERANCE2] = FX_STONERANCE2_FRAMECOUNT;
	m_pFrameCount[FX_DRILLSTONE] = FX_DRILLSTONE_FRAMECOUNT;
	m_pFrameCount[FX_CATCHFLAGDIR] = FX_CATCHFLAGDIR_FRAMECOUNT;
	m_pFrameCount[FX_DROPCANDY] = FX_DROPCANDY_FRAMECOUNT;
	m_pFrameCount[FX_DROPAPPLE] = FX_DROPCANDY_FRAMECOUNT;

	m_pFrameCount[FX_DROPCANDYVIP] = FX_DROPCANDY_FRAMECOUNT;
	m_pFrameCount[FX_DROPCANDYPVP] = FX_DROPCANDY_FRAMECOUNT;
	m_pFrameCount[FX_DROPAPPLEPVP] = FX_DROPCANDY_FRAMECOUNT;

	//  [3/24/2009 ppmmjj83] 대야


	m_pFrameCount[FX_HELLCRY] = FX_HELLCRY_FRAMECOUNT;
	m_pFrameCount[FX_AIRRAID] = FX_AIRRAID_FRAMECOUNT;
	m_pFrameCount[FX_TORNADORUIN] = FX_TORNADORUIN_FRAMECOUNT;
	m_pFrameCount[FX_SUMMONPARTITION] = FX_SUMMONPARTITION_FRAMECOUNT;
	m_pFrameCount[FX_DEVILRING] = FX_DEVILRING_FRAMECOUNT;
	m_pFrameCount[FX_FIREWORKS] = FX_FIREWORKS_FRAMECOUNT;		// FireWork!!(Lyul)
	m_pFrameCount[FX_DSTONE] = FX_DSTONE_FRAMECOUNT;
	m_pFrameCount[FX_DSLASH] = FX_DSLASH_FRAMECOUNT;
	m_pFrameCount[FX_BASH] = FX_BASH_FRAMECOUNT;
	m_pFrameCount[FX_DSTONEEFF] = FX_DSTONEEFF_FRAMECOUNT;
	m_pFrameCount[FX_BLEED] = FX_BLEED_FRAMECOUNT;
	m_pFrameCount[FX_ASSAULT] = FX_ASSAULT_FRAMECOUNT;
	m_pFrameCount[FX_DEMICRUSHER] = FX_DEMICRUSHER_FRAMECOUNT;
	m_pFrameCount[FX_FIRETOWER] = FX_FIRETOWER_FRAMECOUNT;
	m_pFrameCount[FX_DSTONEBRUST] = FX_DSTONEBRUST_FRAMECOUNT;	//mungmae-2005/04/29
	m_pFrameCount[ FX_AUTOMINE ] = FX_MINE_FRAMECOUNT;			// nate 2005-06-15 : 휴먼 스킬 추가
	m_pFrameCount[FX_HELLDOWN] = FX_HELLDOWN_FRAMECOUNT;		// mungmae-2005/07/04 : 카이 스킬 추가
	m_pFrameCount[FX_HELLDOWN2] = FX_HELLDOWN_FRAMECOUNT2;		// mungmae-2005/07/04 : 카이 스킬 추가
	m_pFrameCount[ FX_LIFEEXPENSION ] = FX_LIFEEXPENSION_FRAMECOUNT;	// nate 2005-07-06 : Life Expansion
	m_pFrameCount[ FX_SUMMONSQUAD ] = FX_SUMMONSQUAD_FRAMECOUNT;		// nate 2006-07-08 : Summon Squad
	m_pFrameCount[ FX_REFLECTION ] = FX_REFLECTION_FRAMECOUNT;		// nate 2006-07-08 : Summon Squad
	m_pFrameCount[ FX_DURANT_BEAM ] = FX_DURANT_BEAM_FRAMECOUNT;	// nate 2005-09-26 : 듀란트 - 광선
	m_pFrameCount[ FX_DURANT_TARGET ] = FX_DURANT_TARGET_FRAMECOUNT;	// nate 2005-09-26 : 듀란트 - 공격대상
	m_pFrameCount[ FX_POISON ] = FX_POISON_FRAMECOUNT;	// nate 2005-11-03 : 포이즌
	m_pFrameCount[ FX_ADRENALINE ] = FX_ADRENALINE_FRAMECOUNT; // mungmae 2005/11/21 : 아드레날린
	m_pFrameCount[ FX_ATTACK_MOVELOLO ] = FX_ATTACKMOVELOLO_FRAMECOUNT; // nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	m_pFrameCount[ FX_FLYINGCRASH ] = FX_FLYINGCRASH_FRAMECOUNT; // mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
	m_pFrameCount[ FX_ATTACK_FIXLOLO ] = FX_ATTACKFIXLOLO_FRAMECOUNT; // nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	m_pFrameCount[ FX_FLYINGCRASH2 ] = FX_FLYINGCRASH2_FRAMECOUNT; // mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
	m_pFrameCount[ FX_SHADOWSLASH ] = FX_SHADOWSLASH_FRAMECOUNT;	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
	m_pFrameCount[ FX_SHADOWSLASH2 ] = FX_SHADOWSLASH2_FRAMECOUNT;	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
	m_pFrameCount[ FX_TLVERTEX ] = FX_TLVERTEX_FRAMECOUNT;
	m_pFrameCount[FX_SUMMONRING] = FX_SUMMONRING_FRAMECOUNT; ///불칸링
	m_pFrameCount[ FX_SPEEDUPSMOKE ] = FX_SPEEDUPSMOKE_FRAMECOUNT;
	m_pFrameCount[ FX_PETSKILL ] = FX_PETSKILL_FRAMECOUNT;
	m_pFrameCount[ FX_SPIRITMODE ] = FX_SPIRITMODE_FRAMECOUNT;  //스피릿
	m_pFrameCount[ FX_SPIRITMODE2 ] = FX_SPIRITMODE2_FRAMECOUNT;  //스피릿
	m_pFrameCount[ FX_FLAMESPEAR ] = FX_FLAMESPEAR_FRAMECOUNT; ///플레임스피어
	m_pFrameCount[ FX_SPACESLASH ] = FX_SPACESLASH_FRAMECOUNT;
	m_pFrameCount[ FX_ELECTRICHAMMER ] = FX_ELECTRICHAMMER_FRAMECOUNT;
	m_pFrameCount[ FX_SUMMONFIRE ] = FX_SUMMONFIRE_FRAMECOUNT;
	m_pFrameCount[ FX_RANDOMFIRE ] = FX_RANDOMFIRE_FRAMECOUNT;
	m_pFrameCount[ FX_RANDOMSHOOT ] = FX_RANDOMSHOOT_FRAMECOUNT;
	m_pFrameCount[ FX_MERCERNERYSLASH ] = FX_MERCERNERYSLASH_FRAMECOUNT;
	m_pFrameCount[ FX_ICEBLOT ] = FX_ICEBLOT_FRAMECOUNT;
	m_pFrameCount[ FX_PROTECTIONSHIELD ] = FX_PROTECTIONSHIELD_FRAMECOUNT;
	m_pFrameCount[ FX_SHIELDBLOCK ] = FX_SHIELDBLOCK_FRAMECOUNT;
	m_pFrameCount[ FX_WILDSMASH ] = FX_WILDSMASH_FRAMECOUNT;
	m_pFrameCount[ FX_RESURRECTION ] = FX_RESURRECTION_FRAMECOUNT;
	m_pFrameCount[ FX_TORNADO_HAMMER ] = FX_TORNADO_HAMMER_FRAMECOUNT;
	m_pFrameCount[ FX_POISON_FOG ] = FX_POISON_FOG_FRAMECOUNT; ///인던1
	m_pFrameCount[ FX_SUMMONFIREBOOM ] = FX_SUMMONFIREBOOM_FRAMECOUNT;
	m_pFrameCount[ FX_TYPE_PARTICLE ] = FX_TYPE_PARTICLE_FRAMECOUNT; ///하트로드
	m_pFrameCount[ FX_WTRSPRINKLE ] = FX_WTRSPRINKLE_FRAMECOUNT;		// [3/26/2007 Theodoric] 대국 송크란 이벤트
	m_pFrameCount[ FX_WTRSPRINKLEBOOM ] = FX_WTRSPRINKLEBOOM_FRAMECOUNT;// [3/26/2007 Theodoric] 대국 송크란 이벤트
	m_pFrameCount[ FX_BABYSOUL] = FX_BABYSOUL_FRAMECOUNT;				// [8/7/2007 Theodoric] 6주년 기념 이벤트
	m_pFrameCount[ FX_MURAMASA_GHOST ] = FX_MURAMASA_GHOST_FRAMECOUNT; ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트. // 임의로 큰수
	m_pFrameCount[ FX_DMITRON_PORTAL ] = FX_DMITRON_PORTAL_FRAMECOUNT; // 드미트론 점령전 - 이동 게이트
	m_pFrameCount[ FX_DMITRON_CRYSTAL] = FX_DMITRON_CRYSTAL_FRAMECOUNT; // 드미트론 점령전 - 크리스탈

	m_pFrameCount[FX_ELECTRICPARTICLE]	= FX_ELECTRICPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_CLYSTALGLOW]		= FX_CLYSTALGLOW_FRAMECOUNT;
	m_pFrameCount[FX_WATERFALL]			= FX_WATERFALL_FRAMECOUNT;
	m_pFrameCount[FX_WATERFALLPARTICLE] = FX_WATERFALLPARTICLE_FRAMECOUNT;
	m_pFrameCount[FX_LIGHTNINGEX]		= FX_LIGHTNINGEX_FRAMECOUNT;
	m_pFrameCount[FX_PORTAL_BOSSRAID]	= FX_PORTAL_BOSSRAID_FRAMECOUNT;
	m_pFrameCount[FX_FIRESMOKE_PARTICLE] = FX_FIRESMOKE_PARTICLE_FRAMECOUNT;

	m_pFrameCount[FX_BOSSRAID_BULKAN1_02] = FX_BOSSRAID_BULKAN1_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_BULKAN1_03] = FX_BOSSRAID_BULKAN1_03_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_BULKAN2_02] = FX_BOSSRAID_BULKAN2_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_BULKAN3_01] = FX_BOSSRAID_BULKAN3_01_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_BULKAN3_02] = FX_BOSSRAID_BULKAN3_02_FRAMECOUNT;

	m_pFrameCount[FX_BOSSRAID_HUMAN1_02] = FX_BOSSRAID_HUMAN1_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_HUMAN1_03] = FX_BOSSRAID_HUMAN1_03_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_HUMAN2_02] = FX_BOSSRAID_HUMAN2_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_HUMAN2_03] = FX_BOSSRAID_HUMAN2_03_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_HUMAN3_02] = FX_BOSSRAID_HUMAN3_02_FRAMECOUNT; // [12/25/2007 반재승] 보스레이드 휴먼보스3-02스킬

	m_pFrameCount[FX_BOSSRAID_HYBRI1_01] = FX_BOSSRAID_HYBRI1_01_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_HYBRI1_02] = FX_BOSSRAID_HYBRI1_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_HYBRI2_02] = FX_BOSSRAID_HYBRI2_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_HYBRI3_01] = FX_BOSSRAID_HAYBRI3_01_FRAMECOUNT; // [1/16/2008 반재승] 보스레이드 하이브리더보스3-01 노멀어택
	m_pFrameCount[FX_BOSSRAID_HYBRI3_02] = FX_BOSSRAID_HAYBRI3_02_FRAMECOUNT; // [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

	m_pFrameCount[FX_BOSSRAID_KAI1_01] = FX_BOSSRAID_KAI1_01_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_KAI1_02] = FX_BOSSRAID_KAI1_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_KAI2_01] = FX_BOSSRAID_KAI2_01_FRAMECOUNT; // [2/5/2008 반재승] 카이립톤보스2 노멀 어택
	m_pFrameCount[FX_BOSSRAID_KAI2_02] = FX_BOSSRAID_KAI2_02_FRAMECOUNT; // [2/5/2008 반재승] 카이립톤보스2 노멀 어택
	m_pFrameCount[FX_BOSSRAID_KAI2_03] = FX_BOSSRAID_KAI2_03_FRAMECOUNT; // [2/5/2008 반재승] 카이립톤보스2 노멀 어택
	m_pFrameCount[FX_BOSSRAID_KAI3_01] = FX_BOSSRAID_KAI3_01_FRAMECOUNT; // [2/4/2008 반재승] 카이립톤보스3 노멀 어택
	m_pFrameCount[FX_BOSSRAID_KAI3_02] = FX_BOSSRAID_KAI3_02_FRAMECOUNT; // [2/4/2008 반재승] 카이립톤보스3 2번스킬
	m_pFrameCount[FX_BOSSRAID_KAI3_03] = FX_BOSSRAID_KAI3_03_FRAMECOUNT; // [2/11/2008 반재승] 카이립톤보스3 3번스킬

	m_pFrameCount[FX_BOSSRAID_AIDIA1_01] = FX_BOSSRAID_AIDIA1_01_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_AIDIA1_02] = FX_BOSSRAID_AIDIA1_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_AIDIA1_02_SHOCKWAVE] = FX_BOSSRAID_AIDIA1_02_SHOCKWAVE_FRAMECOUNT;

	m_pFrameCount[FX_BOSSRAID_AIDIA2_01] = FX_BOSSRAID_ADIA2_01_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_AIDIA2_02] = FX_BOSSRAID_ADIA2_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_AIDIA2_03] = FX_BOSSRAID_ADIA2_03_FRAMECOUNT;

	m_pFrameCount[FX_BOSSRAID_AIDIA3_01] = FX_BOSSRAID_AIDIA3_01_FRAMECOUNT; // [3/6/2008 반재승] 에이디아보스3 노멀 어택
	m_pFrameCount[FX_BOSSRAID_AIDIA3_02] = FX_BOSSRAID_AIDIA3_02_FRAMECOUNT; // [3/6/2008 반재승] 에이디아보스3 2번 스킬
	m_pFrameCount[FX_BOSSRAID_AIDIA3_02_STONE] = FX_STONERANCE_FRAMECOUNT;	// [3/6/2008 반재승] 에이디아보스3 2번 스킬

	m_pFrameCount[FX_BOSSRAID_LASTBOSS_02] = FX_BOSSRAID_LASTBOSS_02_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_LASTBOSS_03] = FX_BOSSRAID_LASTBOSS_03_FRAMECOUNT;
	m_pFrameCount[FX_BOSSRAID_LASTBOSS_04] = FX_BOSSRAID_LASTBOSS_04_FRAMECOUNT;

	m_pFrameCount[FX_DROPSTARCANDY] = FX_DROPSTARCANDY_FRAMECOUNT; // 별사탕 이벤트

	m_pFrameCount[FX_HERO_EFFECT]	= FX_HERO_EFFECT_FRAMECOUNT;	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트

	m_pFrameCount[FX_SARNGA_MOUEL]	= FX_SARNGA_MOUEL_FRAMECOUNT;	// [12/3/2008 Theodoric] 사릉가 이펙트
	m_pFrameCount[FX_YETI_RING]		= FX_YETI_RING_FRAMECOUNT;		// AIDIA LEVEL 450
	m_pFrameCount[FX_PEAKOCK_RING]	= FX_PEAKOCK_RING_FRAMECOUNT;	// AIDIA LEVEL 500
	m_pFrameCount[FX_TIGER_RING]	= FX_TIGER_RING_FRAMECOUNT;		// AIDIA LEVEL 550
	m_pFrameCount[FX_UNICORN_RING]	= FX_UNICORN_RING_FRAMECOUNT;	// AIDIA LEVEL 600
	m_pFrameCount[FX_650_RING]	= FX_650_RING_FRAMECOUNT;	// AIDIA LEVEL 650
	m_pFrameCount[FX_700_RING]	= FX_700_RING_FRAMECOUNT;	// AIDIA LEVEL 700
	m_pFrameCount[FX_750_RING]	= FX_750_RING_FRAMECOUNT;	// AIDIA LEVEL 750
	m_pFrameCount[FX_800_RING]	= FX_800_RING_FRAMECOUNT;	// AIDIA LEVEL 800
	m_pFrameCount[FX_850_RING]	= FX_850_RING_FRAMECOUNT;	// AIDIA LEVEL 850
	m_pFrameCount[FX_900_RING]	= FX_900_RING_FRAMECOUNT;	// AIDIA LEVEL 900
	m_pFrameCount[FX_950_RING]	= FX_950_RING_FRAMECOUNT;	// AIDIA LEVEL 950
	m_pFrameCount[FX_1000_RING]	= FX_1000_RING_FRAMECOUNT;	// AIDIA LEVEL 1000
	m_pFrameCount[FX_CHR_RING]	= FX_CHR_RING_FRAMECOUNT;
	m_pFrameCount[FX_CHIN_RING]	= FX_CHIN_RING_FRAMECOUNT;	// Chinese new years
	m_pFrameCount[FX_ARUPAKA_SPIT] = FX_ARUPAKA_SPIT_FRAMECOUNT;

	m_pFrameCount[FX_LV400_EFFECT_GREEN]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV450_EFFECT_BLUE]		= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV500_EFFECT_RED]		= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV550_EFFECT_YELLOW]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV600_EFFECT_PURPLE]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV650_EFFECT_RAINBOW1]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV700_EFFECT_RAINBOW2]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV750_EFFECT_WHITE]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV800_EFFECT_BLACK]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV850_EFFECT_ORANGE]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV900_EFFECT_RAINBOW3]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV950_EFFECT_RAINBOW4]	= FX_HERO_EFFECT_FRAMECOUNT;
	m_pFrameCount[FX_LV1000_EFFECT_PINK]	= FX_HERO_EFFECT_FRAMECOUNT;

	m_pFrameCount[FX_PEROM_SKILL_S125]	= FX_PEROM_SKILL_S125_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S126]	= FX_PEROM_SKILL_S126_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S127]	= FX_PEROM_SKILL_S127_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S128]	= FX_PEROM_SKILL_S128_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S129]	= FX_PEROM_SKILL_S129_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S130]	= FX_PEROM_SKILL_S130_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S131]	= FX_PEROM_SKILL_S131_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S132]	= FX_PEROM_SKILL_S132_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S133]	= FX_PEROM_SKILL_S133_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S134]	= FX_PEROM_SKILL_S134_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S135]	= FX_PEROM_SKILL_S135_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S136]	= FX_PEROM_SKILL_S136_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S137]	= FX_PEROM_SKILL_S137_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S138]	= FX_PEROM_SKILL_S138_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S139]	= FX_PEROM_SKILL_S139_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S140]	= FX_PEROM_SKILL_S140_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S141]	= FX_PEROM_SKILL_S141_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S142]	= FX_PEROM_SKILL_S142_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S143]	= FX_PEROM_SKILL_S143_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S144]	= FX_PEROM_SKILL_S144_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S145]	= FX_PEROM_SKILL_S145_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S146]	= FX_PEROM_SKILL_S146_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S147]	= FX_PEROM_SKILL_S147_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S148]	= FX_PEROM_SKILL_S148_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S149]	= FX_PEROM_SKILL_S149_FRAME;
	m_pFrameCount[FX_PEROM_SKILL_S150]	= FX_PEROM_SKILL_S150_FRAME;


	m_pFrameCount[FX_GRIFFON_RING]	= FX_GRIFFON_RING_FRAMECOUNT;	// [12/3/2008 Theodoric] 사릉가 이펙트

	m_pFrameCount[FX_BALLOON_KILL]		= FX_BALLOON_KILL_FRAMECOUNT;
	
	m_pFrameCount[FX_BOSS_ATTACK_01]	= 40;
	m_pFrameCount[FX_BOSS_ATTACK_02]	= 71;
	m_pFrameCount[FX_BOSS_DEBUFF]		= 41;
	m_pFrameCount[FX_BOSS_MAGIC]		= 46;

	//========================================================================================
	m_EfSortVector.reserve(300); // 그와 관련 해서 튕기는 버그(재할당시 메모리주소가 바뀌어서 그런듯)가 있어서 300으로 늘림.(300을 넘어가는 경우는 거의 없었음.그리고 넘어가도 항상 튕기는 건 아님)
}

CFxSet::~CFxSet()
{
	DeleteRes();
	SAFE_DELETE( m_pFxLight );
	SAFE_DELETE( m_pFxPortal );
	SAFE_DELETE( m_pFxSmoke );
	SAFE_DELETE( m_pFxShadowRing );
	SAFE_DELETE( m_pFxShadowLight );
	SAFE_DELETE( m_pFxParticle );
	SAFE_DELETE( m_pFxLightFog );
	SAFE_DELETE( m_pFxShowup );
	SAFE_DELETE( m_pFxLightning );
	SAFE_DELETE( m_pFxLightBall );
	SAFE_DELETE( m_pFxFireBall );
	SAFE_DELETE( m_pFxFireBallBoom );
	SAFE_DELETE( m_pFxFireBallBoom2 );
	SAFE_DELETE( m_pFxLwLight );
	SAFE_DELETE( m_pFxLightBallBoom );
	SAFE_DELETE( m_pFxUnarmedAttacked );
	SAFE_DELETE( m_pFxMeteor );
	SAFE_DELETE( m_pFxPebbleShot );
	SAFE_DELETE( m_pFxStoneShot );
	SAFE_DELETE( m_pFxStoneShotBoom );
	SAFE_DELETE( m_pFxPebbleShotBoom );
	SAFE_DELETE( m_pFxShoot );
	SAFE_DELETE( m_pFxShootAttacked );
	SAFE_DELETE( m_pFxAidiaFire );
	SAFE_DELETE( m_pFxAidiaBallBoom );
	SAFE_DELETE( m_pFxLightHeal );
	SAFE_DELETE( m_pFxStarFlare );
	SAFE_DELETE( m_pFxStarFlareBoom );
	SAFE_DELETE( m_pFxShockSpear );
	SAFE_DELETE( m_pFxStoneRance );
	SAFE_DELETE_ARRAY( m_pFrameCount );
	SAFE_DELETE( m_pFxRedFire );
	SAFE_DELETE( m_pFxFly );
	SAFE_DELETE( m_pFxBee );
	SAFE_DELETE( m_pFxBeeBoom );
	SAFE_DELETE( m_pFxMoth );
	SAFE_DELETE( m_pFxBora );
	SAFE_DELETE( m_pFxBoraBoom );
	SAFE_DELETE( m_pFxShilon );
	SAFE_DELETE( m_pFxHorn );
	SAFE_DELETE( m_pFxSilverFly );
	SAFE_DELETE( m_pFxRaven );
	SAFE_DELETE( m_pFxKines );
	SAFE_DELETE( m_pFxNeo );
	SAFE_DELETE( m_pFxKaisen );
	SAFE_DELETE( m_pFxHitEffects );
	SAFE_DELETE( m_pFxHitBoomEffects );
	SAFE_DELETE( m_pFxLevelUp );
	SAFE_DELETE( m_pFxShowUp2 );
	SAFE_DELETE( m_pFxEatEffects );
	SAFE_DELETE( m_pFxShield );
	SAFE_DELETE( m_pFxSmokeParticle );
	SAFE_DELETE( m_pFxFireParticle );
	SAFE_DELETE( m_pFxHornBoom );
	SAFE_DELETE( m_pFxHealEffects );
	SAFE_DELETE( m_pFxWorldCup );
	SAFE_DELETE( m_pFxWorldCupBoom );
	SAFE_DELETE( m_pFxGuardTower );
	SAFE_DELETE( m_pFxShilonMaster );
	SAFE_DELETE( m_pFxLightBallBoomNew );
	SAFE_DELETE( m_pFxControlLight );
	SAFE_DELETE( m_pFxCatchFlag );
	SAFE_DELETE( m_pFxGreenSmoke );
	SAFE_DELETE( m_pFxConditionEffects );
	SAFE_DELETE( m_pFxConditionEffects2 );
	SAFE_DELETE( m_pFxOptionSkillEffects );
	SAFE_DELETE( m_pFxHitPowerEffects );
	SAFE_DELETE( m_pFxEarthQuake );
	SAFE_DELETE( m_pFxFrost );
	SAFE_DELETE( m_pFxChakram );
	SAFE_DELETE( m_pFxBlackHole );
	SAFE_DELETE( m_pFxScrollRing );
	SAFE_DELETE( m_pFxScrollDamage );
	SAFE_DELETE( m_pFxSkillLight );
	SAFE_DELETE( m_pFxScrollLight );
	SAFE_DELETE( m_pFxChain );
	SAFE_DELETE( m_pFxOnOffEffects );
	SAFE_DELETE( m_pFxScanView );
	SAFE_DELETE( m_pFxChargeCannon );
	SAFE_DELETE( m_pFxFlame );
	SAFE_DELETE( m_pFxAutoLaser );
	SAFE_DELETE( m_pFxLifeCommon );
	SAFE_DELETE( m_pFxLifeLight );
	SAFE_DELETE( m_pFxMagicArrow );
	SAFE_DELETE( m_pFxMagicLaser );
	SAFE_DELETE( m_pFxFireBoomShort );
	SAFE_DELETE( m_pFxRocket );
	SAFE_DELETE( m_pFxMeteorite );
	SAFE_DELETE( m_pFxMeteoriteBoom );
	SAFE_DELETE( m_pFxChaos );
	SAFE_DELETE( m_pFxGatheringGun );
	SAFE_DELETE( m_pFxDragonEffects );
	SAFE_DELETE( m_pFxNuclearAim );
	SAFE_DELETE( m_pFxNuclearBoom );
	SAFE_DELETE( m_pFxDeathWaveAim );
	SAFE_DELETE( m_pFxCharWave );
	SAFE_DELETE( m_pFxPhenix );
	SAFE_DELETE( m_pFxSprayParticle );
	SAFE_DELETE( m_pFxTurnParticle );
	SAFE_DELETE( m_pFxLightParticle );
	SAFE_DELETE( m_pFxHitPetEffects );
	SAFE_DELETE( m_pFxKhanHitEffects );
	SAFE_DELETE( m_pFxTeleport );
	SAFE_DELETE( m_pFxWeaponEffects );
	SAFE_DELETE( m_pFxRunSmoke );
	SAFE_DELETE( m_pFxWarGiantMulti );
	SAFE_DELETE( m_pFxWarGiantAttack );
	SAFE_DELETE( m_pFxWarGiantDie );
	SAFE_DELETE( m_pFxMobShowUp );
	SAFE_DELETE( m_pFxPowerUp );
	SAFE_DELETE( m_pFxGryphon );
	SAFE_DELETE( m_pFxDragon );
	SAFE_DELETE( m_pFxMobParticle );
	SAFE_DELETE( m_pFxGhostHunt );
	SAFE_DELETE( m_pFxGhostRise );
	SAFE_DELETE( m_pFxSoulDust );
	SAFE_DELETE( m_pFxMagicSword );
	SAFE_DELETE( m_pFxSphinx );
	SAFE_DELETE( m_pFxVampireAttack );
	SAFE_DELETE( m_pFxVampire );
	SAFE_DELETE( m_pFxMusicalNote );
	SAFE_DELETE( m_pFxFallOffStone );
	SAFE_DELETE( m_pFxDrillStone );
	SAFE_DELETE( m_pFxStoneRance2 );
	SAFE_DELETE( m_pFxDropCandy );
	SAFE_DELETE( m_pFxDropApple );
	SAFE_DELETE( m_pFxDropCandypvp );
	SAFE_DELETE( m_pFxDropCandyvip );
	SAFE_DELETE( m_pFxDropApplepvp );



	SAFE_DELETE( m_pFxDropStarCandy ); // 별사탕 이벤트
	SAFE_DELETE( m_pFxHellCry );
	SAFE_DELETE( m_pFxAirRaid );
	SAFE_DELETE( m_pFxTornadoRuin );
	SAFE_DELETE( m_pFxSumPart );
	SAFE_DELETE( m_pFxDevil );
	SAFE_DELETE( m_pFxSarngaMouel );	// [12/3/2008 Theodoric] 사릉가
	SAFE_DELETE( m_fx_yeti_ring);		// AIDIA LEVEL 450
	SAFE_DELETE( m_fx_peakock_ring);	// AIDIA LEVEL 500
	SAFE_DELETE( m_fx_tiger_ring);		// AIDIA LEVEL 550
	SAFE_DELETE( m_fx_unicorn_ring);	// AIDIA LEVEL 600
	SAFE_DELETE( m_fx_650_ring);	// AIDIA LEVEL 650
	SAFE_DELETE( m_fx_700_ring);	// AIDIA LEVEL 700
	SAFE_DELETE( m_fx_750_ring);	// AIDIA LEVEL 750
	SAFE_DELETE( m_fx_800_ring);	// AIDIA LEVEL 800
	SAFE_DELETE( m_fx_850_ring);	// AIDIA LEVEL 850
	SAFE_DELETE( m_fx_900_ring);	// AIDIA LEVEL 900
	SAFE_DELETE( m_fx_950_ring);	// AIDIA LEVEL 950
	SAFE_DELETE( m_fx_1000_ring);	// AIDIA LEVEL 1000
	SAFE_DELETE( m_fx_CHR_ring);
	SAFE_DELETE( m_fx_CHIN_ring);
	SAFE_DELETE( m_pFxSummonRing ); ///불칸링
	SAFE_DELETE( m_pFxFireWorks );
	SAFE_DELETE( m_pFxDstone );
	SAFE_DELETE( m_pFxDslash );
	SAFE_DELETE( m_pFxBash );
	SAFE_DELETE( m_pFxDstoneEff );
	SAFE_DELETE( m_pFxbleed );
	SAFE_DELETE( m_pFxAssault );
	SAFE_DELETE( m_pFxDemiCrusher );
	SAFE_DELETE( m_pFxFireTower );
	SAFE_DELETE( m_pFxStoneBrust );
	SAFE_DELETE( m_pFxReflection );
	SAFE_DELETE( m_pFxHellDown_Circle );
	SAFE_DELETE( m_pFxHellDown_Circle2 );
	SAFE_DELETE( m_pFxHellDown_Around );
	SAFE_DELETE( m_pFxHellDown_Lightly );
	SAFE_DELETE( m_pFxHellDown_Spark );
	SAFE_DELETE( m_pFxHellDown_Lightning );
	SAFE_DELETE( m_pFxMine );			// nate 2005-06-15 : Auto Mine - 메모리 제거
	SAFE_DELETE( m_pFxLifeExpansion );	// nate 2005-07-06 : Life Expansion
	SAFE_DELETE( m_pFxSummonSquad );	// nate 2005-07-08 : Summon Squad
	SAFE_DELETE( m_pFxCatchFlagDir);
	SAFE_DELETE( m_pFxDurant_Beam );	// nate 2005-09-26 : 듀란트 - 광선
	SAFE_DELETE( m_pFxDurant_Target );	// nate 2005-09-26 : 듀란트 - 공격대상
	SAFE_DELETE( m_pFxPoison );			// nate 2005-11-03 : 포이즌
	SAFE_DELETE( m_pFxAdrenaline );		// mungmae 2005/11/21 : 하이브리더 - 아드레날린
	SAFE_DELETE( m_pFxAttack_MoveLolo );	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	SAFE_DELETE( m_pFxFlyingCrash );	// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
	SAFE_DELETE( m_pFxFlyingCrash2 );	// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
	SAFE_DELETE( m_pFxAttack_FixLolo );	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	SAFE_DELETE( m_pFxShadowSlash );	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
	SAFE_DELETE( m_pFxShadowSlash2 );	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
	SAFE_DELETE( m_pFxMuramasaGhost );	///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
	SAFE_DELETE( m_pFxFollow );			///페어리
	SAFE_DELETE( m_pFxFlameSpear );		///플레임스피어
	SAFE_DELETE( m_pFxPetSkill );
	SAFE_DELETE( m_pFxSpeedUpSmoke );
	SAFE_DELETE( m_pFxSpiritMode );		//스피릿
	SAFE_DELETE( m_pFxSpiritMode2 );	//스피릿
	SAFE_DELETE( m_pFxSpaceSlash );
	SAFE_DELETE( m_pFxElectricHammer );
	SAFE_DELETE( m_pFxSummonFire );
	SAFE_DELETE( m_pFxSummonFireBoom );
	SAFE_DELETE( m_pFxRandomFire );
	SAFE_DELETE( m_pFxRandomShoot );
	SAFE_DELETE( m_pFxMercernerySlash );
	SAFE_DELETE( m_pFxIceBlot );
	SAFE_DELETE( m_pFxProtectionShield );
	SAFE_DELETE( m_pFxShieldBlock );
	SAFE_DELETE( m_pFxWildSmash );
	SAFE_DELETE( m_pFxResurrection );
	SAFE_DELETE( m_pFxTornadoHammer );
	SAFE_DELETE( m_pFxTLVertex );
	SAFE_DELETE( m_pFxPoisonFog );		///인던1
	SAFE_DELETE( m_pFxTypeParticle );	///하트로드
	SAFE_DELETE( m_pFxFreePvpRing );	// 자유대련의 링. ///자유대련
	SAFE_DELETE( m_pFxWTRSprinkle);
	SAFE_DELETE( m_pFxWTRSprinkleBoom);
	SAFE_DELETE( m_pFxBabySoul );		// [8/7/2007 Theodoric] 6주년 기념 이벤트
	SAFE_DELETE( m_pFxDmitronPortal );
	SAFE_DELETE( m_pFxDmitronCrystal );

	SAFE_DELETE( m_pFxElectricParticle );
	SAFE_DELETE( m_pFxClystalGlow);
	SAFE_DELETE( m_pFxWaterfall);
	SAFE_DELETE( m_pFxWaterfallParticle );
	SAFE_DELETE( m_pFxLightningEx );
	SAFE_DELETE( m_pFxPortalBossraid);
	SAFE_DELETE( m_pFxFireSmokeParticle);

	SAFE_DELETE(m_pFxBossRaidBulkan1_02);
	SAFE_DELETE(m_pFxBossRaidBulkan1_03);
	SAFE_DELETE(m_pFxBossRaidBulkan2_02);
	SAFE_DELETE(m_pFxBossRaidBulkan3_02);
	SAFE_DELETE(m_pFxBossRaidBulkan3_03);

	SAFE_DELETE(m_pFxBossRaidHuman1_02);
	SAFE_DELETE(m_pFxBossRaidHuman1_03);
	SAFE_DELETE(m_pFxBossRaidHuman2_02);
	SAFE_DELETE(m_pFxBossRaidHuman2_03);
	SAFE_DELETE(m_pFxBossRaidHuman3_02);

	SAFE_DELETE(m_pFxBossRaidHybri1_01);
	SAFE_DELETE(m_pFxBossRaidHybri1_02);
	SAFE_DELETE(m_pFxBossRaidHybri2_02);
	SAFE_DELETE(m_pFxBossRaidHybri3_01); // [1/16/2008 반재승] 보스레이드 보스 스킬 이미지 로드(하이브리더보스3-01)
	SAFE_DELETE(m_pFxBossRaidHybri3_02); // [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

	SAFE_DELETE(m_pFxBossRaidKai1_01);
	SAFE_DELETE(m_pFxBossRaidKai1_02);
	SAFE_DELETE(m_pFxBossRaidKai2_01);
	SAFE_DELETE(m_pFxBossRaidKai2_02);
	SAFE_DELETE(m_pFxBossRaidKai2_03);
	SAFE_DELETE(m_pFxBossRaidKai3_01); // [2/4/2008 반재승] 카이립톤보스3 노멀 어택
	SAFE_DELETE(m_pFxBossRaidKai3_02); // [2/4/2008 반재승] 카이립톤보스3 2번스킬
	SAFE_DELETE(m_pFxBossRaidKai3_03); // [2/11/2008 반재승] 카이립톤보스3 3번스킬

	SAFE_DELETE(m_pFxBossRaidAidia1_01); // 에이디아 1_01 노말 어텍
	SAFE_DELETE(m_pFxBossRaidAidia1_02); // 에이디아 1_01 노말 어텍
	SAFE_DELETE(m_pFxBossRaidAidia1_02_sw); // 에이디아 1_01 노말 어텍

	SAFE_DELETE(m_pFxBossRaidAidia2_01);
	SAFE_DELETE(m_pFxBossRaidAidia2_02);
	SAFE_DELETE(m_pFxBossRaidAidia2_03);

	SAFE_DELETE(m_pFxBossRaidAidia3_01); // [3/6/2008 반재승] 에이디아보스3 노멀 어택
	SAFE_DELETE(m_pFxBossRaidAidia3_02); // [3/6/2008 반재승] 에이디아보스3 2번 스킬

	SAFE_DELETE(m_pFxBossRaidLastBoss_02);
	SAFE_DELETE(m_pFxBossRaidLastBoss_03);
	SAFE_DELETE(m_pFxBossRaidLastBoss_04);

	SAFE_DELETE(m_pFxHeroEffect);	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트

	SAFE_DELETE(m_pFxArupakaSpit);

	SAFE_DELETE(m_pFxLv400Effect);

	SAFE_DELETE(m_fx_perom_spin);
	SAFE_DELETE(m_fx_perom_thurst);
	SAFE_DELETE(m_fx_perom_blader);
	SAFE_DELETE(m_fx_perom_pierce);
	SAFE_DELETE(m_fx_perom_boom);
	SAFE_DELETE(m_fx_perom_shotgun);
	SAFE_DELETE(m_fx_perom_field);
	SAFE_DELETE(m_fx_perom_wheelwind);
	SAFE_DELETE(m_fx_perom_strike);
	SAFE_DELETE(m_fx_perom_cross);
	SAFE_DELETE(m_fx_perom_meta);
	SAFE_DELETE(m_fx_perom_dsystem);
	SAFE_DELETE(m_fx_perom_overload);
	SAFE_DELETE(m_fx_perom_alpha);

	SAFE_DELETE(m_fx_griffon_ring);

	SAFE_DELETE(m_pFxBalloonKill);

	SAFE_DELETE(m_pFxBossAttack01);
	SAFE_DELETE(m_pFxBossAttack02);
	SAFE_DELETE(m_pFxBossDebuff);
	SAFE_DELETE(m_pFxBossMagic);


	vector<EffectSort*>::iterator prEffect;
	EffectSort *pEffectSort;
	prEffect = m_EfSortVector.begin();

	for ( ; prEffect != m_EfSortVector.end(); prEffect++)
	{
		pEffectSort = *prEffect;

		Tail_Info *tail_Info = NULL;
		int num = 0;

		switch (pEffectSort->nType)
		{
		case FX_VAMPIREATTACK:
		case FX_VAMPIRE:
		case FX_MAGICSWORD:
		case FX_GHOSTHUNT:
		case FX_GHOSTRISE:
			if( pEffectSort->effects )
			{
				tail_Info = (Tail_Info* )pEffectSort->effects;

				for( num = 0; num < pEffectSort->Index; num++ )
					SAFE_DELETE_ARRAY( tail_Info[num].pTailPos );

				delete [] tail_Info;
				pEffectSort->effects = tail_Info = NULL;
			}
			break;
		default:
			break;
		}

		SAFE_DELETE(pEffectSort);
	}
}

void CFxSet::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{
	if(m_pFxLightFog)			m_pFxLightFog->InitSparkles();
	if(m_pFxAidiaBallBoom)		m_pFxAidiaBallBoom->LoadRes();
	if(m_pFxLightHeal)			m_pFxLightHeal->InitSparkles();
	if(m_pFxStoneRance)			m_pFxStoneRance->LoadRes();
	if(m_pFxFly)				m_pFxFly->LoadRes();
	if(m_pFxMoth)				m_pFxMoth->LoadRes();
	if(m_pFxHorn)				m_pFxHorn->LoadRes();
	if(m_pFxSilverFly)			m_pFxSilverFly->LoadRes();
	if(m_pFxRaven)				m_pFxRaven->LoadRes();
	if(m_pFxKines)				m_pFxKines->LoadRes();
	if(m_pFxNeo)				m_pFxNeo->LoadRes();
	if(m_pFxKaisen)				m_pFxKaisen->LoadRes();
	if(m_pFxWorldCup)			m_pFxWorldCup->LoadRes();
	if(m_pFxFrost)				m_pFxFrost->LoadRes();
	if(m_pFxRocket)				m_pFxRocket->LoadRes();
	if(m_pFxPhenix)				m_pFxPhenix->LoadRes();
	if(m_pFxWarGiantMulti)		m_pFxWarGiantMulti->LoadRes();
	if(m_pFxGryphon)			m_pFxGryphon->LoadRes();
	if(m_pFxDragon)				m_pFxDragon->LoadRes();
	if(m_pFxSphinx)				m_pFxSphinx->LoadRes();
	if(m_pFxFallOffStone)		m_pFxFallOffStone->LoadRes();
	if(m_pFxDrillStone)			m_pFxDrillStone->LoadRes();
	if(m_pFxStoneRance2)		m_pFxStoneRance2->LoadRes();
	if(m_pFxDropCandy)			m_pFxDropCandy->LoadRes( 0 );	// Drop Candy
	if(m_pFxDropApple)			m_pFxDropApple->LoadRes( 1 );	// Drop Apple

	if(m_pFxDropCandyvip)			m_pFxDropCandyvip->LoadRes( 2 );
	if(m_pFxDropCandypvp)			m_pFxDropCandypvp->LoadRes( 3 );
	if(m_pFxDropApplepvp)			m_pFxDropApplepvp->LoadRes( 4 );



	if(m_pFxDropStarCandy)		m_pFxDropStarCandy->LoadRes();	// 별사탕 이벤트
	if(m_pFxHellCry)			m_pFxHellCry->LoadRes();
	if(m_pFxAirRaid)			m_pFxAirRaid->LoadRes();
	if(m_pFxTornadoRuin)		m_pFxTornadoRuin->LoadRes();
	if(m_pFxSumPart)			m_pFxSumPart->LoadRes();
	if(m_pFxDevil)				m_pFxDevil->LoadRes();
	if(m_pFxSarngaMouel)		m_pFxSarngaMouel->LoadRes(); // [12/3/2008 Theodoric] 사릉가
	if(m_fx_yeti_ring)			m_fx_yeti_ring->LoadRes();		// AIDIA LEVEL 450
	if(m_fx_peakock_ring)		m_fx_peakock_ring->LoadRes();	// AIDIA LEVEL 500
	if(m_fx_tiger_ring)			m_fx_tiger_ring->LoadRes();		// AIDIA LEVEL 550
	if(m_fx_unicorn_ring)		m_fx_unicorn_ring->LoadRes();	// AIDIA LEVEL 600
	if(m_fx_650_ring)		m_fx_650_ring->LoadRes();	// AIDIA LEVEL 650
	if(m_fx_700_ring)		m_fx_700_ring->LoadRes();	// AIDIA LEVEL 700
	if(m_fx_750_ring)		m_fx_750_ring->LoadRes();	// AIDIA LEVEL 750
	if(m_fx_800_ring)		m_fx_800_ring->LoadRes();	// AIDIA LEVEL 800
	if(m_fx_850_ring)		m_fx_850_ring->LoadRes();	// AIDIA LEVEL 850
	if(m_fx_900_ring)		m_fx_900_ring->LoadRes();	// AIDIA LEVEL 900
	if(m_fx_950_ring)		m_fx_950_ring->LoadRes();	// AIDIA LEVEL 950
	if(m_fx_1000_ring)		m_fx_1000_ring->LoadRes();	// AIDIA LEVEL 1000
	if(m_fx_CHR_ring)		m_fx_CHR_ring->LoadRes();
	if(m_fx_CHIN_ring)		m_fx_CHIN_ring->LoadRes();
	if(m_pFxBash)				m_pFxBash->LoadRes();			//wan:2005-01	배쉬 이펙트
	if(g_bUseMatrixCountry && m_pFxDemiCrusher)	m_pFxDemiCrusher->LoadRes();	// 매트릭스가 도입되는 나라만 로딩되게 처리.(by 원석) ///해외뫼비우스
	if(m_pFxMine)				m_pFxMine->LoadRes();			// nate 2005-06-15 : Auto Mine - 클레스 초기화
	if(m_pFxDurant_Beam)		m_pFxDurant_Beam->LoadRes();	// nate 2005-09-26 : 듀란트 - 공선
	if(m_pFxDurant_Target)		m_pFxDurant_Target->LoadRes();	// nate 2005-09-26 : 듀란트 - 공격 대상
	if(m_pFxPoison)				m_pFxPoison->LoadRes();			// nate 2005-11-03 : 포이즌
	if(m_pFxAttack_MoveLolo)	m_pFxAttack_MoveLolo->LoadRes();	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	if(m_pFxAdrenaline)			m_pFxAdrenaline->LoadRes();		// mungmae 2005/11/21 : 하이브리더 - 아드레날린
	if(m_pFxFlyingCrash)		m_pFxFlyingCrash->LoadRes();	// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
	if(m_pFxAttack_FixLolo)		m_pFxAttack_FixLolo->LoadRes();	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	if(m_pFxFlyingCrash2)		m_pFxFlyingCrash2->LoadRes();	// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
	if(m_pFxShadowSlash)		m_pFxShadowSlash->LoadRes();	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
	if(m_pFxShadowSlash2)		m_pFxShadowSlash2->LoadRes();	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
	if(m_pFxFlameSpear)			m_pFxFlameSpear->LoadRes();	///플레임스피어
	if(m_pFxPetSkill)			m_pFxPetSkill->LoadRes();
	if(m_pFxSpeedUpSmoke)		m_pFxSpeedUpSmoke->LoadRes();
	if(m_pFxSpiritMode)		    m_pFxSpiritMode->LoadRes();
	if(m_pFxSpiritMode2)		m_pFxSpiritMode2->LoadRes();
	if(m_pFxSpaceSlash)			m_pFxSpaceSlash->LoadRes();
	if(m_pFxElectricHammer)		m_pFxElectricHammer->LoadRes();
	if(m_pFxRandomFire)			m_pFxRandomFire->LoadRes();
	if(m_pFxSummonFire)			m_pFxSummonFire->LoadRes();
	if(m_pFxMercernerySlash)	m_pFxMercernerySlash->LoadRes();
	if(m_pFxIceBlot)			m_pFxIceBlot->LoadRes();
	if(m_pFxProtectionShield)	m_pFxProtectionShield->LoadRes();
	if(m_pFxShieldBlock)		m_pFxShieldBlock->LoadRes();
	if(m_pFxWildSmash)			m_pFxWildSmash->LoadRes();
	if(m_pFxResurrection)		m_pFxResurrection->LoadRes();
	if(m_pFxTornadoHammer)		m_pFxTornadoHammer->LoadRes();
	if(m_pFxTypeParticle)		m_pFxTypeParticle->LoadRes();	 ///하트로드
	if(m_pFxFreePvpRing)		m_pFxFreePvpRing->LoadRes();  // 자유대련의 링. ///자유대련
	if(m_pFxDmitronPortal)		m_pFxDmitronPortal->LoadRes(); // 드미트론 점령전 - 이동 게이트
	if(m_pFxDmitronCrystal)		m_pFxDmitronCrystal->LoadRes(); // 드미트론 점령전 - 크리스탈

	if(m_pFxElectricParticle)		m_pFxElectricParticle->LoadRes();
	if(m_pFxClystalGlow)			m_pFxClystalGlow->LoadRes();
	if(m_pFxWaterfall)				m_pFxWaterfall->LoadRes();
	if(m_pFxWaterfallParticle)		m_pFxWaterfallParticle->LoadRes();
	if(m_pFxLightningEx)			m_pFxLightningEx->LoadRes();
	if(m_pFxPortalBossraid)			m_pFxPortalBossraid->LoadRes();
	if(m_pFxFireSmokeParticle)		m_pFxFireSmokeParticle->LoadRes();

	if(m_pFxBossRaidBulkan1_02)		m_pFxBossRaidBulkan1_02->LoadRes();
	if(m_pFxBossRaidBulkan1_03)		m_pFxBossRaidBulkan1_03->LoadRes();
	if(m_pFxBossRaidBulkan2_02)		m_pFxBossRaidBulkan2_02->LoadRes();
	if(m_pFxBossRaidBulkan3_02)		m_pFxBossRaidBulkan3_02->LoadRes();
	if(m_pFxBossRaidBulkan3_03)		m_pFxBossRaidBulkan3_03->LoadRes(); 	// [11/21/2007 반재승] 보스레이드 보스 스킬 이미지 로드

	if(m_pFxBossRaidHuman1_02)		m_pFxBossRaidHuman1_02->LoadRes();
	if(m_pFxBossRaidHuman1_03)		m_pFxBossRaidHuman1_03->LoadRes();
	if(m_pFxBossRaidHuman2_02)		m_pFxBossRaidHuman2_02->LoadRes();
	if(m_pFxBossRaidHuman2_03)		m_pFxBossRaidHuman2_03->LoadRes();
	if(m_pFxBossRaidHuman3_02)		m_pFxBossRaidHuman3_02->LoadRes(); 		// [12/25/2007 반재승] 보스레이드 보스 스킬 이미지 로드(휴먼보스3-02)

	if(m_pFxBossRaidHybri1_01)		m_pFxBossRaidHybri1_01->LoadRes();
	if(m_pFxBossRaidHybri1_02)		m_pFxBossRaidHybri1_02->LoadRes();
	if(m_pFxBossRaidHybri2_02)		m_pFxBossRaidHybri2_02->LoadRes();
	if(m_pFxBossRaidHybri3_01)		m_pFxBossRaidHybri3_01->LoadRes(); 		// [1/16/2008 반재승] 보스레이드 보스 스킬 이미지 로드(하이브리더보스3-01)
	if(m_pFxBossRaidHybri3_02)		m_pFxBossRaidHybri3_02->LoadRes(); 		// [1/17/2008 반재승] 보스레이드 보스 스킬 로드(하이브리더보스3-02)

	if(m_pFxBossRaidKai1_01)		m_pFxBossRaidKai1_01->LoadRes();
	if(m_pFxBossRaidKai1_02)		m_pFxBossRaidKai1_02->LoadRes();
	if(m_pFxBossRaidKai2_01)		m_pFxBossRaidKai2_01->LoadRes(); 		// [2/5/2008 반재승] 카이립톤보스2 노멀 어택
	if(m_pFxBossRaidKai2_02)		m_pFxBossRaidKai2_02->LoadRes();
	if(m_pFxBossRaidKai2_03)		m_pFxBossRaidKai2_03->LoadRes();
	if(m_pFxBossRaidKai3_01)		m_pFxBossRaidKai3_01->LoadRes(); 		// [2/4/2008 반재승] 카이립톤보스3 노멀 어택
	if(m_pFxBossRaidKai3_02)		m_pFxBossRaidKai3_02->LoadRes(); 		// [2/4/2008 반재승] 카이립톤보스3 2번스킬
	if(m_pFxBossRaidKai3_03)		m_pFxBossRaidKai3_03->LoadRes(); 		// [2/11/2008 반재승] 카이립톤보스3 3번스킬

	if(m_pFxBossRaidAidia1_01)		m_pFxBossRaidAidia1_01->LoadRes(); 		// 에이디아 1_01 노말 어텍
	if(m_pFxBossRaidAidia1_02)		m_pFxBossRaidAidia1_02->LoadRes(); 		// 에이디아 1_01 노말 어텍
	if(m_pFxBossRaidAidia1_02_sw)	m_pFxBossRaidAidia1_02_sw->LoadRes(); 		// 에이디아 1_01 노말 어텍

	if(m_pFxBossRaidAidia2_01)		m_pFxBossRaidAidia2_01->LoadRes();		// 에이디아 2 1번 정상 공격
	if(m_pFxBossRaidAidia2_02)		m_pFxBossRaidAidia2_02->LoadRes();		// 에이디아 2 2번 특수 공격
	if(m_pFxBossRaidAidia2_03)		m_pFxBossRaidAidia2_03->LoadRes();		// 에이디아 2 3번 특수 공격

	if(m_pFxBossRaidAidia3_01)		m_pFxBossRaidAidia3_01->LoadRes(); 		// [3/6/2008 반재승] 에이디아보스3 노멀 어택
	if(m_pFxBossRaidAidia3_02)		m_pFxBossRaidAidia3_02->LoadRes(); 		// [3/6/2008 반재승] 에이디아보스3 2번 스킬

	if(m_pFxBossRaidLastBoss_02)	m_pFxBossRaidLastBoss_02->LoadRes();
	if(m_pFxBossRaidLastBoss_03)	m_pFxBossRaidLastBoss_03->LoadRes();
	if(m_pFxBossRaidLastBoss_04)	m_pFxBossRaidLastBoss_04->LoadRes();

	if(m_pFxHeroEffect)				m_pFxHeroEffect->LoadRes();	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트

	if(m_fx_perom_spin)			m_fx_perom_spin->LoadRes();
	if(m_fx_perom_thurst)		m_fx_perom_thurst->LoadRes();
	if(m_fx_perom_blader)		m_fx_perom_blader->LoadRes();
	if(m_fx_perom_pierce)		m_fx_perom_pierce->LoadRes();
	if(m_fx_perom_boom)			m_fx_perom_boom->LoadRes();
	if(m_fx_perom_shotgun)		m_fx_perom_shotgun->LoadRes();
	if(m_fx_perom_field)		m_fx_perom_field->LoadRes();
	if(m_fx_perom_wheelwind)	m_fx_perom_wheelwind->LoadRes();
	if(m_fx_perom_strike)		m_fx_perom_strike->LoadRes();
	if(m_fx_perom_cross)		m_fx_perom_cross->LoadRes();
	if(m_fx_perom_meta)			m_fx_perom_meta->LoadRes();
	if(m_fx_perom_dsystem)		m_fx_perom_dsystem->LoadRes();
	if(m_fx_perom_overload)		m_fx_perom_overload->LoadRes();
	if(m_fx_perom_alpha)		m_fx_perom_alpha->LoadRes();
	if(m_fx_griffon_ring)		m_fx_griffon_ring->LoadRes();
	if(m_pFxBalloonKill)		m_pFxBalloonKill->LoadRes();
	if(m_pFxBossAttack01)		m_pFxBossAttack01->LoadRes();
	if(m_pFxBossAttack02)		m_pFxBossAttack02->LoadRes();
	if(m_pFxBossDebuff)			m_pFxBossDebuff->LoadRes();
	if(m_pFxBossMagic)			m_pFxBossMagic->LoadRes();
}
// nate 2006-02-01 : 2차 로딩(해상도 조정 후)때 읽어들임
void CFxSet::LoadResAfter()
{
	if(m_pFxTLVertex)			m_pFxTLVertex->LoadRes();		// nate 2006-01-24 : TLVertex 이펙트
	if(m_pFxFollow)				m_pFxFollow->LoadRes();	///페어리 // 복돼지 할때 옮김.
	if(m_pFxMuramasaGhost)		m_pFxMuramasaGhost->LoadRes();	///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
}

void CFxSet::DeleteRes()
{
	if(m_pFxLight)				m_pFxLight->DeleteRes();
	if(m_pFxPortal)				m_pFxPortal->DeleteRes();
	if(m_pFxSmoke)				m_pFxSmoke->DeleteRes();
	if(m_pFxShadowRing)			m_pFxShadowRing->DeleteRes();
	if(m_pFxShadowLight)		m_pFxShadowLight->DeleteRes();
	if(m_pFxParticle)			m_pFxParticle->DeleteRes();
	if(m_pFxLightFog)			m_pFxLightFog->DeleteRes();
	if(m_pFxShowup)				m_pFxShowup->DeleteRes();
	if(m_pFxLightning)			m_pFxLightning->DeleteRes();
	if(m_pFxLightBall)			m_pFxLightBall->DeleteRes();
	if(m_pFxFireBall)			m_pFxFireBall->DeleteRes();
	if(m_pFxFireBallBoom)		m_pFxFireBallBoom->DeleteRes();
	if(m_pFxFireBallBoom2)		m_pFxFireBallBoom2->DeleteRes();
	if(m_pFxLwLight)			m_pFxLwLight->DeleteRes();
	if(m_pFxLightBallBoom)		m_pFxLightBallBoom->DeleteRes();
	if(m_pFxUnarmedAttacked)	m_pFxUnarmedAttacked->DeleteRes();
	if(m_pFxMeteor)				m_pFxMeteor->DeleteRes();
	if(m_pFxPebbleShot)			m_pFxPebbleShot->DeleteRes();
	if(m_pFxStoneShot)			m_pFxStoneShot->DeleteRes();
	if(m_pFxStoneShotBoom)		m_pFxStoneShotBoom->DeleteRes();
	if(m_pFxPebbleShotBoom)		m_pFxPebbleShotBoom->DeleteRes();
	if(m_pFxShoot)				m_pFxShoot->DeleteRes();
	if(m_pFxShootAttacked)		m_pFxShootAttacked->DeleteRes();
	if(m_pFxAidiaFire)			m_pFxAidiaFire->DeleteRes();
	if(m_pFxAidiaBallBoom)		m_pFxAidiaBallBoom->DeleteRes();
	if(m_pFxLightHeal)			m_pFxLightHeal->DeleteRes();
	if(m_pFxStarFlare)			m_pFxStarFlare->DeleteRes();
	if(m_pFxStarFlareBoom)		m_pFxStarFlareBoom->DeleteRes();
	if(m_pFxShockSpear)			m_pFxShockSpear->DeleteRes();
	if(m_pFxStoneRance)			m_pFxStoneRance->DeleteRes();
	if(m_pFxRedFire)			m_pFxRedFire->DeleteRes();
	if(m_pFxFly)				m_pFxFly->DeleteRes();
	if(m_pFxBee)				m_pFxBee->DeleteRes();
	if(m_pFxBeeBoom)			m_pFxBeeBoom->DeleteRes();
	if(m_pFxMoth)				m_pFxMoth->DeleteRes();
	if(m_pFxBora)				m_pFxBora->DeleteRes();
	if(m_pFxBoraBoom)			m_pFxBoraBoom->DeleteRes();
	if(m_pFxShilon)				m_pFxShilon->DeleteRes();
	if(m_pFxHorn)				m_pFxHorn->DeleteRes();
	if(m_pFxSilverFly)			m_pFxSilverFly->DeleteRes();
	if(m_pFxRaven)				m_pFxRaven->DeleteRes();
	if(m_pFxKines)				m_pFxKines->DeleteRes();
	if(m_pFxNeo)				m_pFxNeo->DeleteRes();
	if(m_pFxKaisen)				m_pFxKaisen->DeleteRes();
	if(m_pFxHitEffects)			m_pFxHitEffects->DeleteRes();
	if(m_pFxHitBoomEffects)		m_pFxHitBoomEffects->DeleteRes();
	if(m_pFxLevelUp)			m_pFxLevelUp->DeleteRes();
	if(m_pFxShowUp2)			m_pFxShowUp2->DeleteRes();
	if(m_pFxEatEffects)			m_pFxEatEffects->DeleteRes();
	if(m_pFxShield)				m_pFxShield->DeleteRes();
	if(m_pFxSmokeParticle)		m_pFxSmokeParticle->DeleteRes();
	if(m_pFxFireParticle)		m_pFxFireParticle->DeleteRes();
	if(m_pFxHornBoom)			m_pFxHornBoom->DeleteRes();
	if(m_pFxHealEffects)		m_pFxHealEffects->DeleteRes();
	if(m_pFxWorldCup)			m_pFxWorldCup->DeleteRes();
	if(m_pFxWorldCupBoom)		m_pFxWorldCupBoom->DeleteRes();
	if(m_pFxGuardTower)			m_pFxGuardTower->DeleteRes();
	if(m_pFxShilonMaster)		m_pFxShilonMaster->DeleteRes();
	if(m_pFxLightBallBoomNew)	m_pFxLightBallBoomNew->DeleteRes();
	if(m_pFxControlLight)		m_pFxControlLight->DeleteRes();
	if(m_pFxCatchFlag)			m_pFxCatchFlag->DeleteRes();
	if(m_pFxGreenSmoke)			m_pFxGreenSmoke->DeleteRes();
	if(m_pFxConditionEffects)	m_pFxConditionEffects->DeleteRes();
	if(m_pFxConditionEffects2)	m_pFxConditionEffects2->DeleteRes();
	if(m_pFxOptionSkillEffects)	m_pFxOptionSkillEffects->DeleteRes();
	if(m_pFxHitPowerEffects)	m_pFxHitPowerEffects->DeleteRes();
	if(m_pFxEarthQuake)			m_pFxEarthQuake->DeleteRes();
	if(m_pFxFrost)				m_pFxFrost->DeleteRes();
	if(m_pFxChakram)			m_pFxChakram->DeleteRes();
	if(m_pFxBlackHole)			m_pFxBlackHole->DeleteRes();
	if(m_pFxScrollRing)			m_pFxScrollRing->DeleteRes();
	if(m_pFxScrollDamage)		m_pFxScrollDamage->DeleteRes();
	if(m_pFxSkillLight)			m_pFxSkillLight->DeleteRes();
	if(m_pFxScrollLight)		m_pFxScrollLight->DeleteRes();
	if(m_pFxChain)				m_pFxChain->DeleteRes();
	if(m_pFxOnOffEffects)		m_pFxOnOffEffects->DeleteRes();
	if(m_pFxScanView)			m_pFxScanView->DeleteRes();
	if(m_pFxChargeCannon)		m_pFxChargeCannon->DeleteRes();
	if(m_pFxFlame)				m_pFxFlame->DeleteRes();
	if(m_pFxAutoLaser)			m_pFxAutoLaser->DeleteRes();
	if(m_pFxLifeCommon)			m_pFxLifeCommon->DeleteRes();
	if(m_pFxLifeLight)			m_pFxLifeLight->DeleteRes();
	if(m_pFxMagicArrow)			m_pFxMagicArrow->DeleteRes();
	if(m_pFxMagicLaser)			m_pFxMagicLaser->DeleteRes();
	if(m_pFxFireBoomShort)		m_pFxFireBoomShort->DeleteRes();
	if(m_pFxRocket)				m_pFxRocket->DeleteRes();
	if(m_pFxMeteorite)			m_pFxMeteorite->DeleteRes();
	if(m_pFxMeteoriteBoom)		m_pFxMeteoriteBoom->DeleteRes();
	if(m_pFxChaos)				m_pFxChaos->DeleteRes();
	if(m_pFxGatheringGun)		m_pFxGatheringGun->DeleteRes();
	if(m_pFxDragonEffects)		m_pFxDragonEffects->DeleteRes();
	if(m_pFxNuclearAim)			m_pFxNuclearAim->DeleteRes();
	if(m_pFxNuclearBoom)		m_pFxNuclearBoom->DeleteRes();
	if(m_pFxDeathWaveAim)		m_pFxDeathWaveAim->DeleteRes();
	if(m_pFxCharWave)			m_pFxCharWave->DeleteRes();
	if(m_pFxPhenix)				m_pFxPhenix->DeleteRes();
	if(m_pFxSprayParticle)		m_pFxSprayParticle->DeleteRes();
	if(m_pFxTurnParticle)		m_pFxTurnParticle->DeleteRes();
	if(m_pFxLightParticle)		m_pFxLightParticle->DeleteRes();
	if(m_pFxHitPetEffects)		m_pFxHitPetEffects->DeleteRes();
	if(m_pFxKhanHitEffects)		m_pFxKhanHitEffects->DeleteRes();
	if(m_pFxTeleport)			m_pFxTeleport->DeleteRes();
	if(m_pFxWeaponEffects)		m_pFxWeaponEffects->DeleteRes();
	if(m_pFxRunSmoke)			m_pFxRunSmoke->DeleteRes();
	if(m_pFxWarGiantMulti)		m_pFxWarGiantMulti->DeleteRes();
	if(m_pFxWarGiantAttack)		m_pFxWarGiantAttack->DeleteRes();
	if(m_pFxWarGiantDie)		m_pFxWarGiantDie->DeleteRes();
	if(m_pFxMobShowUp)			m_pFxMobShowUp->DeleteRes();
	if(m_pFxPowerUp)			m_pFxPowerUp->DeleteRes();
	if(m_pFxGryphon)			m_pFxGryphon->DeleteRes();
	if(m_pFxDragon)				m_pFxDragon->DeleteRes();
	if(m_pFxMobParticle)		m_pFxMobParticle->DeleteRes();
	if(m_pFxGhostHunt)			m_pFxGhostHunt->DeleteRes();
	if(m_pFxGhostRise)			m_pFxGhostRise->DeleteRes();
	if(m_pFxSoulDust)			m_pFxSoulDust->DeleteRes();
	if(m_pFxMagicSword)			m_pFxMagicSword->DeleteRes();
	if(m_pFxSphinx)				m_pFxSphinx->DeleteRes();
	if(m_pFxVampire)			m_pFxVampire->DeleteRes();
	if(m_pFxVampireAttack)		m_pFxVampireAttack->DeleteRes();
	if(m_pFxMusicalNote)		m_pFxMusicalNote->DeleteRes();
	if(m_pFxFallOffStone)		m_pFxFallOffStone->DeleteRes();
	if(m_pFxDrillStone)			m_pFxDrillStone->DeleteRes();
	if(m_pFxStoneRance2)		m_pFxStoneRance2->DeleteRes();
	if(m_pFxCatchFlagDir)		m_pFxCatchFlagDir->DeleteRes();
	if(m_pFxDropCandy)			m_pFxDropCandy->DeleteRes();		// Drop Candy
	if(m_pFxDropApple)			m_pFxDropApple->DeleteRes();		// Drop Apple

	if(m_pFxDropCandyvip)			m_pFxDropCandyvip->DeleteRes();
	if(m_pFxDropCandypvp)			m_pFxDropCandypvp->DeleteRes();
	if(m_pFxDropApplepvp)			m_pFxDropApplepvp->DeleteRes();



	if(m_pFxDropStarCandy)		m_pFxDropStarCandy->DeleteRes();	// 별사탕 이벤트
	if(m_pFxHellCry)			m_pFxHellCry->DeleteRes();	//중고렙 스킬 추가
	if(m_pFxAirRaid)			m_pFxAirRaid->DeleteRes();
	if(m_pFxTornadoRuin)		m_pFxTornadoRuin->DeleteRes();
	if(m_pFxSumPart)			m_pFxSumPart->DeleteRes();
	if(m_pFxDevil)				m_pFxDevil->DeleteRes();
	if(m_pFxSarngaMouel)		m_pFxSarngaMouel->DeleteRes(); // [12/3/2008 Theodoric] 사릉가
	if(m_fx_yeti_ring)			m_fx_yeti_ring->DeleteRes();	// AIDIA LEVEL 450
	if(m_fx_peakock_ring)		m_fx_peakock_ring->DeleteRes();	// AIDIA LEVEL 500
	if(m_fx_tiger_ring)			m_fx_tiger_ring->DeleteRes();	// AIDIA LEVEL 550
	if(m_fx_unicorn_ring)		m_fx_unicorn_ring->DeleteRes();	// AIDIA LEVEL 600
	if(m_fx_650_ring)		m_fx_650_ring->DeleteRes();	// AIDIA LEVEL 650
	if(m_fx_700_ring)		m_fx_700_ring->DeleteRes();	// AIDIA LEVEL 700
	if(m_fx_750_ring)		m_fx_750_ring->DeleteRes();	// AIDIA LEVEL 750
	if(m_fx_800_ring)		m_fx_800_ring->DeleteRes();	// AIDIA LEVEL 800
	if(m_fx_850_ring)		m_fx_850_ring->DeleteRes();	// AIDIA LEVEL 850
	if(m_fx_900_ring)		m_fx_900_ring->DeleteRes();	// AIDIA LEVEL 900
	if(m_fx_950_ring)		m_fx_950_ring->DeleteRes();	// AIDIA LEVEL 950
	if(m_fx_1000_ring)		m_fx_1000_ring->DeleteRes();	// AIDIA LEVEL 1000
	if(m_fx_CHR_ring)		m_fx_CHR_ring->DeleteRes();
	if(m_fx_CHIN_ring)		m_fx_CHIN_ring->DeleteRes();
	if(m_pFxDemiCrusher)		m_pFxDemiCrusher->DeleteRes();
	if(m_pFxMine)				m_pFxMine->DeleteRes();						// nate 2005-06-15 : Auto Mine - 메모리 제거
	if(m_pFxFireTower)			m_pFxFireTower->DeleteRes();
	if(m_pFxReflection)			m_pFxReflection->DeleteRes();
	if(m_pFxHellDown_Circle)	m_pFxHellDown_Circle->DeleteRes();
	if(m_pFxHellDown_Circle2)	m_pFxHellDown_Circle2->DeleteRes();
	if(m_pFxHellDown_Around)	m_pFxHellDown_Around->DeleteRes();
	if(m_pFxHellDown_Lightly)	m_pFxHellDown_Lightly->DeleteRes();
	if(m_pFxHellDown_Spark)		m_pFxHellDown_Spark->DeleteRes();
	if(m_pFxHellDown_Lightning)m_pFxHellDown_Lightning->DeleteRes();
	if(m_pFxLifeExpansion)		m_pFxLifeExpansion->DeleteRes();
	if(m_pFxSummonSquad)		m_pFxSummonSquad->DeleteRes();
	if(m_pFxCatchFlagDir)		m_pFxCatchFlagDir->DeleteRes();
	if(m_pFxDurant_Beam)		m_pFxDurant_Beam->DeleteRes();		// nate 2005-09-26 : 듀란트 - 공선
	if(m_pFxDurant_Target)		m_pFxDurant_Target->DeleteRes();	// nate 2005-09-26 : 듀란트 - 공격 대상
	if(m_pFxPoison)				m_pFxPoison->DeleteRes();				// nate 2005-11-03 : 포이즌
	if(m_pFxAdrenaline)			m_pFxAdrenaline->DeleteRes();		// mungmae 2005/11/21 : 하이브리더 - 아드레날린
	if(m_pFxAttack_MoveLolo)	m_pFxAttack_MoveLolo->DeleteRes();		// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	if(m_pFxFlyingCrash)		m_pFxFlyingCrash->DeleteRes();		// mungmae 2005/11/22 : 불칸 플라잉 크래쉬(시작시)
	if(m_pFxAttack_FixLolo)		m_pFxAttack_FixLolo->DeleteRes();		// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
	if(m_pFxFlyingCrash2)		m_pFxFlyingCrash2->DeleteRes();		// mungmae 2005/11/24 : 불칸 플라잉 크래쉬(공격시)
	if(m_pFxShadowSlash)		m_pFxShadowSlash->DeleteRes();		// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
	if(m_pFxShadowSlash2)		m_pFxShadowSlash2->DeleteRes();		// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
	if(m_pFxMuramasaGhost)		m_pFxMuramasaGhost->DeleteRes();			///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
	if(m_pFxFollow)				m_pFxFollow->DeleteRes();			///페어리
	if(m_pFxFlameSpear)				m_pFxFlameSpear->DeleteRes();			///플레임스피어
	if(m_pFxPetSkill)			m_pFxPetSkill->DeleteRes();
	if(m_pFxSpeedUpSmoke)		m_pFxSpeedUpSmoke->DeleteRes();
	if(m_pFxSpiritMode)		    m_pFxSpiritMode->DeleteRes();
	if(m_pFxSpiritMode2)		m_pFxSpiritMode2->DeleteRes();
	if(m_pFxSpaceSlash)			m_pFxSpaceSlash->DeleteRes();
	if(m_pFxElectricHammer)		m_pFxElectricHammer->DeleteRes();
	if(m_pFxSummonFireBoom)		m_pFxSummonFireBoom->DeleteRes();
	if(m_pFxRandomFire)			m_pFxRandomFire->DeleteRes();
	if(m_pFxRandomShoot)		m_pFxRandomShoot->DeleteRes();
	if(m_pFxMercernerySlash)	m_pFxMercernerySlash->DeleteRes();
	if(m_pFxIceBlot)			m_pFxIceBlot->DeleteRes();
	if(m_pFxProtectionShield)	m_pFxProtectionShield->DeleteRes();
	if(m_pFxShieldBlock)		m_pFxShieldBlock->DeleteRes();
	if(m_pFxWildSmash)			m_pFxWildSmash->DeleteRes();
	if(m_pFxResurrection)		m_pFxResurrection->DeleteRes();
	if(m_pFxTornadoHammer)		m_pFxTornadoHammer->DeleteRes();
	if(m_pFxTLVertex)			m_pFxTLVertex->DeleteRes();
	if(m_pFxFreePvpRing)		m_pFxFreePvpRing->DeleteRes();  // 자유대련의 링. ///자유대련
	if(m_pFxDmitronPortal)		m_pFxDmitronPortal->DeleteRes(); // 드미트론 점령전 - 이동 게이트
	if(m_pFxDmitronCrystal)		m_pFxDmitronCrystal->DeleteRes(); // 드미트론 점령전 - 크리스탈

	if(m_pFxElectricParticle)	m_pFxElectricParticle->DeleteRes();
	if(m_pFxClystalGlow)		m_pFxClystalGlow->DeleteRes();
	if(m_pFxWaterfall)			m_pFxWaterfall->DeleteRes();
	if(m_pFxWaterfallParticle)	m_pFxWaterfallParticle->DeleteRes();
	if(m_pFxLightningEx)		m_pFxLightningEx->DeleteRes();
	if(m_pFxPortalBossraid)		m_pFxPortalBossraid->DeleteRes();
	if(m_pFxFireSmokeParticle)	m_pFxFireSmokeParticle->DeleteRes();

	if( m_pFxBossRaidBulkan1_02)		m_pFxBossRaidBulkan1_02->DeleteRes();
	if( m_pFxBossRaidBulkan1_03)		m_pFxBossRaidBulkan1_03->DeleteRes();
	if( m_pFxBossRaidBulkan2_02)		m_pFxBossRaidBulkan2_02->DeleteRes();
	if( m_pFxBossRaidBulkan3_02)		m_pFxBossRaidBulkan3_02->DeleteRes();
	if( m_pFxBossRaidBulkan3_03)		m_pFxBossRaidBulkan3_03->DeleteRes();

	if( m_pFxBossRaidHuman1_02)			m_pFxBossRaidHuman1_02->DeleteRes();
	if( m_pFxBossRaidHuman1_03)			m_pFxBossRaidHuman1_03->DeleteRes();
	if( m_pFxBossRaidHuman2_02)			m_pFxBossRaidHuman2_02->DeleteRes();
	if( m_pFxBossRaidHuman2_03)			m_pFxBossRaidHuman2_03->DeleteRes();
	if( m_pFxBossRaidHuman3_02)			m_pFxBossRaidHuman3_02->DeleteRes(); // [12/25/2007 반재승] 보스레이드 보스 스킬 메모리 해제(휴먼보스3-02)

	if( m_pFxBossRaidHybri1_01)			m_pFxBossRaidHybri1_01->DeleteRes();
	if( m_pFxBossRaidHybri1_02)			m_pFxBossRaidHybri1_02->DeleteRes();
	if( m_pFxBossRaidHybri2_02)			m_pFxBossRaidHybri2_02->DeleteRes();
	if( m_pFxBossRaidHybri3_01)			m_pFxBossRaidHybri3_01->DeleteRes(); // [1/16/2008 반재승] 보스레이드 보스 스킬 메모리 해제(하이브리더보스3-01)
	if( m_pFxBossRaidHybri3_02)			m_pFxBossRaidHybri3_02->DeleteRes(); // [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬

	if( m_pFxBossRaidKai1_01)			m_pFxBossRaidKai1_01->DeleteRes();
	if( m_pFxBossRaidKai1_02)			m_pFxBossRaidKai1_02->DeleteRes();
	if( m_pFxBossRaidKai2_01)			m_pFxBossRaidKai2_01->DeleteRes(); // [2/5/2008 반재승] 카이립톤보스2 노멀 어택
	if( m_pFxBossRaidKai2_02)			m_pFxBossRaidKai2_02->DeleteRes();
	if( m_pFxBossRaidKai2_03)			m_pFxBossRaidKai2_03->DeleteRes();
	if( m_pFxBossRaidKai3_01)			m_pFxBossRaidKai3_01->DeleteRes(); // [2/4/2008 반재승] 카이립톤보스3 노멀 어택
	if( m_pFxBossRaidKai3_02)			m_pFxBossRaidKai3_02->DeleteRes(); // [2/4/2008 반재승] 카이립톤보스3 2번스킬
	if( m_pFxBossRaidKai3_03)			m_pFxBossRaidKai3_03->DeleteRes(); // [2/11/2008 반재승] 카이립톤보스3 3번스킬

	if( m_pFxBossRaidAidia1_01)			m_pFxBossRaidAidia1_01->DeleteRes(); // [2/11/2008 반재승] 카이립톤보스3 3번스킬
	if( m_pFxBossRaidAidia1_02)			m_pFxBossRaidAidia1_02->DeleteRes(); // [2/11/2008 반재승] 카이립톤보스3 3번스킬
	if( m_pFxBossRaidAidia1_02_sw)		m_pFxBossRaidAidia1_02_sw->DeleteRes(); // [2/11/2008 반재승] 카이립톤보스3 3번스킬

	if( m_pFxBossRaidAidia2_01)			m_pFxBossRaidAidia2_01->DeleteRes(); // 에이디아 2 1번 정상 공격
	if( m_pFxBossRaidAidia2_02)			m_pFxBossRaidAidia2_02->DeleteRes(); // 에이디아 2 2번 특수 공격
	if( m_pFxBossRaidAidia2_03)			m_pFxBossRaidAidia2_03->DeleteRes(); // 에이디아 2 3번 특수 공격

	if( m_pFxBossRaidAidia3_01)			m_pFxBossRaidAidia3_01->DeleteRes(); // [3/6/2008 반재승] 에이디아보스3 노멀 어택
	if( m_pFxBossRaidAidia3_02)			m_pFxBossRaidAidia3_02->DeleteRes(); // [3/6/2008 반재승] 에이디아보스3 2번 스킬

	if( m_pFxBossRaidLastBoss_02)		m_pFxBossRaidLastBoss_02->DeleteRes();
	if( m_pFxBossRaidLastBoss_03)		m_pFxBossRaidLastBoss_03->DeleteRes();
	if( m_pFxBossRaidLastBoss_04)		m_pFxBossRaidLastBoss_04->DeleteRes();

	if( m_pFxHeroEffect)				m_pFxHeroEffect->DeleteRes();	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트
	// delete resource
	if(m_fx_perom_spin)			m_fx_perom_spin->DeleteRes();
	if(m_fx_perom_thurst)		m_fx_perom_thurst->DeleteRes();
	if(m_fx_perom_blader)		m_fx_perom_blader->DeleteRes();
	if(m_fx_perom_pierce)		m_fx_perom_pierce->DeleteRes();
	if(m_fx_perom_boom)			m_fx_perom_boom->DeleteRes();
	if(m_fx_perom_shotgun)		m_fx_perom_shotgun->DeleteRes();
	if(m_fx_perom_field)		m_fx_perom_field->DeleteRes();
	if(m_fx_perom_wheelwind)	m_fx_perom_wheelwind->DeleteRes();
	if(m_fx_perom_strike)		m_fx_perom_strike->DeleteRes();
	if(m_fx_perom_cross)		m_fx_perom_cross->DeleteRes();
	if(m_fx_perom_meta)			m_fx_perom_meta->DeleteRes();
	if(m_fx_perom_dsystem)		m_fx_perom_dsystem->DeleteRes();
	if(m_fx_perom_overload)		m_fx_perom_overload->DeleteRes();
	if(m_fx_perom_alpha)		m_fx_perom_alpha->DeleteRes();
	if(m_fx_griffon_ring)		m_fx_griffon_ring->DeleteRes();	
}


void CFxSet::FrameMove()
{
	D3DMATRIX matView;
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
}

void CFxSet::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	int i;
	DWORD dwMin, dwMag;
	static bool bStart = false;
	static DWORD dwLastTime = 0;
	static float fSaveTime = 0.0f;
	int nAddFrame = 0;
	BOOL bFinished;

	FrameMove();			// 소팅을 한다.

	fSaveTime += g_fTimeKey;

	while (fSaveTime > 0.05f)
	{
		nAddFrame++;
		fSaveTime -= 0.05f;
	}

	// 필터링 설정.
	GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_MINFILTER, &dwMin);
	GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_MAGFILTER, &dwMag);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

	if (m_EfSortVector.size() > 0)
	{
		if (bStart == false)
			dwLastTime = timeGetTime();
		bStart = true;
	}

	DWORD dwZEnable, dwZWriteEnable, dwAlphaTest, dwAlphaBlend;

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZENABLE, &dwZEnable);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &dwZWriteEnable);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);

	int nLoop = m_EfSortVector.size();

	for (i=0; i < nLoop; i++)
	{
		switch (m_EfSortVector[i]->nType)
		{
		case FX_SMALL_LIGHT:
			if( m_pFxLight )
				m_pFxLight->Render(m_EfSortVector[i]->vPos);
			break;

		case FX_PORTAL:
			if( m_pFxPortal )
				m_pFxPortal->Render(m_EfSortVector[i]->vPos);
			break;

		case FX_SMOKE:
			if( m_pFxSmoke )
				m_pFxSmoke->Render(m_EfSortVector[i]->vPos);
			break;

		case FX_SHADOWRING:
			if( m_pFxShadowRing )
				m_pFxShadowRing->Render(m_EfSortVector[i]->vPos);
			break;

		case FX_SHADOWLIGHT:
			if( m_pFxShadowLight )
				m_pFxShadowLight->Render(m_EfSortVector[i]->vPos);
			break;

		case FX_PARTICLE:
			if( m_pFxParticle )
				m_pFxParticle->Render(m_EfSortVector[i]->vPos);
			break;

		case FX_LIGHTFOG:
			if( m_pFxLightFog )
				m_pFxLightFog->Render(m_EfSortVector[i]->vPos);
			break;

		case FX_SHOWUP:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxShowup )
					m_pFxShowup->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->nCurFrame);
			break;

		case FX_LIGHTNING:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxLightning )
					m_pFxLightning->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->nCurFrame);
			break;

		case FX_LIGHTBALL:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxLightBall )
					bFinished = m_pFxLightBall->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LIGHTBALL];
			}
			break;

		case FX_FIREBALL:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FIREBALL])
			{
				bFinished = FALSE;
				if( m_pFxFireBall )
					bFinished = m_pFxFireBall->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FIREBALL];
			}
			break;

		case FX_FIREBALLBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FIREBALLBOOM])
			{
				bFinished = FALSE;
				if( m_pFxFireBallBoom )
					bFinished = m_pFxFireBallBoom->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->vTargetPos, m_EfSortVector[i]->nCurFrame);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FIREBALLBOOM];
			}
			break;
		case FX_FIREBALLBOOM2:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FIREBALLBOOM2])
			{
				bFinished = FALSE;
				if( m_pFxFireBallBoom2 )
					bFinished = m_pFxFireBallBoom2->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->vTargetPos, m_EfSortVector[i]->nCurFrame);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FIREBALLBOOM2];
			}
			break;
		case FX_SUMMONFIREBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SUMMONFIREBOOM])
			{
				bFinished = FALSE;
				if( m_pFxFireBallBoom )
					bFinished = m_pFxSummonFireBoom->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->vTargetPos, m_EfSortVector[i]->nCurFrame);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SUMMONFIREBOOM];
			}
			break;
		case FX_RANDOMFIRE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_RANDOMFIRE])
			{
				bFinished = FALSE;
				if( m_pFxFireBallBoom )
					bFinished = m_pFxRandomFire->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_RANDOMFIRE];
			}
			break;

		case FX_LWLIGHT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_LWLIGHT])
			{
				bFinished = FALSE;
				if( m_pFxLwLight )
					bFinished = m_pFxLwLight->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->vTargetPos, m_EfSortVector[i]->nCurFrame);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LWLIGHT];
			}
			break;

		case FX_LIGHTBALLBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_LIGHTBALLBOOM])
			{
				bFinished = FALSE;
				if( m_pFxLightBallBoom )
					bFinished = m_pFxLightBallBoom->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->vTargetPos, m_EfSortVector[i]->nCurFrame);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LIGHTBALLBOOM];
			}
			break;

		case FX_UNARMEDATTACKED:		// 매 프레임마다 자동 삭제되는 효과.
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_UNARMEDATTACKED])
				if( m_pFxUnarmedAttacked )
					m_pFxUnarmedAttacked->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->nCurFrame);
			break;

		case FX_METEOR:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxMeteor )
					m_pFxMeteor->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->nCurFrame);
			break;

		case FX_PEBBLESHOT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_PEBBLESHOT])
			{
				bFinished = FALSE;
				if( m_pFxPebbleShot )
					bFinished = m_pFxPebbleShot->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_PEBBLESHOT];
			}
			break;

		case FX_STONESHOT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_STONESHOT])
			{
				bFinished = FALSE;
				if( m_pFxStoneShot )
					bFinished = m_pFxStoneShot->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_STONESHOT];
			}
			break;

		case FX_STONESHOTBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_STONESHOTBOOM])
			{
				bFinished = FALSE;
				if( m_pFxStoneShotBoom )
					bFinished = m_pFxStoneShotBoom->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_PEBBLESHOTBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_PEBBLESHOTBOOM])
			{
				bFinished = FALSE;
				if( m_pFxPebbleShotBoom )
					bFinished = m_pFxPebbleShotBoom->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_SHOOT:
			if (m_EfSortVector[i]->bOneFrame)
				bFinished = TRUE;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SHOOT])
			{
				bFinished = TRUE;			// 심심해서.. ^^;
				if( m_pFxShoot )
					m_pFxShoot->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_SHOOTATTACKED:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SHOOTATTACKED])
				if( m_pFxShootAttacked )
					m_pFxShootAttacked->Render(*m_EfSortVector[i]);
			break;

		case FX_AIDIAFIRE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_AIDIAFIRE])
			{
				bFinished = FALSE;
				if( m_pFxAidiaFire )
					bFinished = m_pFxAidiaFire->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_AIDIAFIRE];
			}
			break;

		case FX_AIDIABALLBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_AIDIABALLBOOM])
			{
				bFinished = FALSE;
				if( m_pFxAidiaBallBoom )
					bFinished = m_pFxAidiaBallBoom->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_LIGHTHEAL:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_LIGHTHEAL])
			{
				bFinished = FALSE;
				if( m_pFxLightHeal )
					bFinished = m_pFxLightHeal->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_STARFLARE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_STARFLARE])
			{
				bFinished = FALSE;
				if( m_pFxStarFlare )
					bFinished = m_pFxStarFlare->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_STARFLARE];
			}
			break;

		case FX_STARFLAREBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_STARFLAREBOOM])
			{
				bFinished = FALSE;
				if( m_pFxStarFlareBoom )
					bFinished = m_pFxStarFlareBoom->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_SHOCKSPEAR:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxShockSpear )
					m_pFxShockSpear->Render(*m_EfSortVector[i]);
			break;

		case FX_STONERANCE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxStoneRance )
					m_pFxStoneRance->Render(*m_EfSortVector[i]);
			break;

		case FX_REDFIRE:
			if (m_EfSortVector[i]->bOneFrame)
				bFinished = TRUE;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_REDFIRE])
			{
				bFinished = TRUE;			// 심심해서.. ^^;
				if( m_pFxRedFire )
					m_pFxRedFire->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_FLY:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FLY])
			{
				bFinished = FALSE;
				if( m_pFxFly )
					bFinished = m_pFxFly->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FLY];
			}
			break;

		case FX_BEE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_BEE])
			{
				bFinished = FALSE;
				if( m_pFxBee )
					bFinished = m_pFxBee->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BEE];
			}
			break;

		case FX_BEEBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_BEEBOOM])
			{
				bFinished = FALSE;
				if( m_pFxBeeBoom )
					bFinished = m_pFxBeeBoom->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->vTargetPos, m_EfSortVector[i]->nCurFrame);
			}
			break;

		case FX_MOTH:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_MOTH])
			{
				bFinished = FALSE;
				if( m_pFxMoth )
					bFinished = m_pFxMoth->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_MOTH];
			}
			break;

		case FX_BORA:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_BORA])
			{
				bFinished = FALSE;
				if( m_pFxBora )
					bFinished = m_pFxBora->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BORA];
			}
			break;

		case FX_BORABOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_BORABOOM])
			{
				bFinished = FALSE;
				if( m_pFxBoraBoom )
					bFinished = m_pFxBoraBoom->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->vTargetPos, m_EfSortVector[i]->nCurFrame);
			}
			break;

		case FX_SHILON:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SHILON])
			{
				bFinished = FALSE;
				if( m_pFxShilon )
					bFinished = m_pFxShilon->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_HORN:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxHorn )
					m_pFxHorn->Render(*m_EfSortVector[i]);
			break;

		case FX_SILVERFLY:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxSilverFly )
					m_pFxSilverFly->Render(*m_EfSortVector[i]);
			break;

		case FX_RAVEN:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxRaven )
					m_pFxRaven->Render(*m_EfSortVector[i]);
			break;

		case FX_KINES:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxKines )
					m_pFxKines->Render(*m_EfSortVector[i]);
			break;

		case FX_NEO:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxNeo )
					m_pFxNeo->Render(*m_EfSortVector[i]);
			break;

		case FX_KAISEN:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxKaisen )
					m_pFxKaisen->Render(*m_EfSortVector[i]);
			break;

		case FX_HITEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxHitEffects )
					bFinished = m_pFxHitEffects->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_HITBOOMEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxHitBoomEffects )
					bFinished = m_pFxHitBoomEffects->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_LEVELUP:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxLevelUp )
					bFinished = m_pFxLevelUp->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LEVELUP];
			}
			break;
		case FX_SHOWUP2:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_pFxShowUp2 && m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				bFinished = m_pFxShowUp2->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_EATEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if ( m_pFxEatEffects && m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				bFinished = m_pFxEatEffects->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_SHIELD:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxShield )
					bFinished = m_pFxShield->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_SMOKEPARTICLE:

			{
				Deco_Particle*	deco_particle;
				deco_particle = (Deco_Particle*) m_EfSortVector[i]->effects;

				D3DVECTOR ef_Pos = m_EfSortVector[i]->vPos;
				D3DVECTOR ch_Pos = D3DVECTOR(g_pRoh->m_wx, 0.0f, g_pRoh->m_wz);
				ef_Pos.y = 0.0f;

				float dist = DistVecToVec( ch_Pos, ef_Pos );

				if( dist > 1400.0f )
					break;
			}

			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxSmokeParticle )
					bFinished = m_pFxSmokeParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SMOKEPARTICLE];
			}
			break;

		case FX_ELECTRICPARTICLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxElectricParticle )
					bFinished = m_pFxElectricParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ELECTRICPARTICLE];
			}
			break;

		case FX_CLYSTALGLOW:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;

				if( m_EfSortVector[i]->nCurFrame >= 0 )
				{
					if( m_pFxClystalGlow )
						m_pFxClystalGlow->Render(*m_EfSortVector[i]);

					if( m_EfSortVector[i]->nCurFrame > m_pFrameCount[FX_CLYSTALGLOW]-1)
						m_EfSortVector[i]->nCurFrame -= m_pFrameCount[FX_CLYSTALGLOW];
				}
			}
			break;

		case FX_WATERFALL:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxWaterfall )
					bFinished = m_pFxWaterfall->Render(*m_EfSortVector[i]);

				if( m_EfSortVector[i]->nCurFrame > m_pFrameCount[FX_WATERFALL]-1)
					m_EfSortVector[i]->nCurFrame -= m_pFrameCount[FX_WATERFALL];

			}
			break;

		case FX_WATERFALLPARTICLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxWaterfallParticle )
					bFinished = m_pFxWaterfallParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_WATERFALLPARTICLE];
			}
			break;

		case FX_LIGHTNINGEX:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxLightningEx )
					m_pFxLightningEx->Render(m_EfSortVector[i]->vPos, m_EfSortVector[i]->nCurFrame);
			break;

		case FX_PORTAL_BOSSRAID:
			if( m_EfSortVector[i]->nCurFrame == 0 )
			{
				bFinished = FALSE;
				if( m_pFxPortalBossraid )
					bFinished = m_pFxPortalBossraid->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_WATERFALLPARTICLE];
			}
			break;

		case FX_FIRESMOKE_PARTICLE:
			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxFireSmokeParticle )
					bFinished = m_pFxFireSmokeParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_WATERFALLPARTICLE];
			}
			break;

		case FX_FIREPARTICLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxFireParticle )
					bFinished = m_pFxFireParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FIREPARTICLE];
			}
			break;
		case FX_HORNBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxHornBoom )
					bFinished = m_pFxHornBoom->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_HORNBOOM];
			}
			break;
		case FX_HEALEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxHealEffects )
					bFinished = m_pFxHealEffects->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_HEALEFFECTS];
			}
			break;
		case FX_WORLDCUP:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_WORLDCUP])
			{
				bFinished = FALSE;
				if( m_pFxWorldCup )
					bFinished = m_pFxWorldCup->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_WORLDCUP];
			}
			break;
		case FX_WORLDCUPBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_WORLDCUP])
			{
				bFinished = FALSE;
				if( m_pFxWorldCupBoom )
					bFinished = m_pFxWorldCupBoom->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_WORLDCUP];
			}
			break;
		case FX_GUARDTOWER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_GUARDTOWER])
			{
				bFinished = FALSE;
				if( m_pFxGuardTower )
					bFinished = m_pFxGuardTower->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_GUARDTOWER];
			}
			break;
		case FX_SHILONMASTER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SHILONMASTER])
			{
				bFinished = FALSE;
				if( m_pFxShilonMaster )
					bFinished = m_pFxShilonMaster->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SHILONMASTER];
			}
			break;
		case FX_LIGHTBALLBOOMNEW:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_LIGHTBALLBOOMNEW])
			{
				bFinished = FALSE;
				if( m_pFxLightBallBoomNew )
					bFinished = m_pFxLightBallBoomNew->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LIGHTBALLBOOMNEW];
			}
			break;
		case FX_CONTROL_LIGHT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxControlLight )
					bFinished = m_pFxControlLight->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_CONTROL_LIGHT];
			}
			break;
		case FX_CATCHFLAG:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxCatchFlag )
					bFinished = m_pFxCatchFlag->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_GREENSMOKE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_GREENSMOKE])
			{
				bFinished = FALSE;
				if( m_pFxGreenSmoke )
					bFinished = m_pFxGreenSmoke->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_GREENSMOKE];
			}
			break;
		case FX_CONDITIONEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxConditionEffects )
					bFinished = m_pFxConditionEffects->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_CONDITIONEFFECTS2:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxConditionEffects2 )
					bFinished = m_pFxConditionEffects2->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_OPTIONSKILLEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxOptionSkillEffects )
					bFinished = m_pFxOptionSkillEffects->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_HITPOWEREFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxHitPowerEffects )
					bFinished = m_pFxHitPowerEffects->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_EARTHQUAKE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_EARTHQUAKE])
			{
				bFinished = FALSE;
				if( m_pFxEarthQuake )
					bFinished = m_pFxEarthQuake->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_EARTHQUAKE];
			}
			break;
		case FX_FROST:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxFrost )
					m_pFxFrost->Render(*m_EfSortVector[i]);
			break;

		case FX_CHAKRAM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_CHAKRAM])
			{
				bFinished = FALSE;
				if( m_pFxChakram )
					bFinished = m_pFxChakram->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_CHAKRAM];
			}
			break;
		case FX_BLACKHOLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->num += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxBlackHole )
					bFinished = m_pFxBlackHole->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_SCROLLRING:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxScrollRing )
					m_pFxScrollRing->Render(*m_EfSortVector[i]);
			break;
		case FX_SCROLLDAMAGE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxScrollDamage )
					m_pFxScrollDamage->Render(*m_EfSortVector[i]);
			break;
		case FX_SKILL_LIGHT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxSkillLight )
					bFinished = m_pFxSkillLight->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SKILL_LIGHT];
			}
			break;
		case FX_SCROLL_LIGHT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxScrollLight )
					bFinished = m_pFxScrollLight->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_WAND_CHAIN:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxChain )
					m_pFxChain->Render(*m_EfSortVector[i]);
			break;
		case FX_ONOFFEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxOnOffEffects )
					m_pFxOnOffEffects->Render(*m_EfSortVector[i]);
			break;
		case FX_SCANVIEW:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxScanView )
					m_pFxScanView->Render(*m_EfSortVector[i]);
			break;
		case FX_CHARGECANNON:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxChargeCannon )
					m_pFxChargeCannon->Render(*m_EfSortVector[i]);
			break;
		case FX_FLAME:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FLAME])
			{
				bFinished = FALSE;
				if( m_pFxFlame )
					bFinished = m_pFxFlame->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FLAME];
			}
			break;
		case FX_AUTOLASER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxAutoLaser )
					m_pFxAutoLaser->Render(*m_EfSortVector[i]);
			break;
		case FX_LIFECOMMON:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxLifeCommon )
					bFinished = m_pFxLifeCommon->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_LIFELIGHT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxLifeLight )
					bFinished = m_pFxLifeLight->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_MAGICARROW:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxMagicArrow )
					m_pFxMagicArrow->Render(*m_EfSortVector[i]);
			break;
		case FX_MAGICLASER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxMagicLaser )
					m_pFxMagicLaser->Render(*m_EfSortVector[i]);
			break;
		case FX_FIREBOOMSHORT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FIREBOOMSHORT])
			{
				bFinished = FALSE;
				if( m_pFxFireBoomShort )
					bFinished = m_pFxFireBoomShort->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FIREBOOMSHORT];
			}
			break;
		case FX_ROCKET:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_ROCKET])
			{
				bFinished = FALSE;
				if( m_pFxRocket )
					bFinished = m_pFxRocket->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ROCKET];
			}
			break;

		case FX_METEORITE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_METEORITE])
			{
				bFinished = FALSE;
				if( m_pFxMeteorite )
					bFinished = m_pFxMeteorite->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_METEORITE];
			}
			break;
		case FX_METEORITEBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_METEORITEBOOM])
			{
				bFinished = FALSE;
				if( m_pFxMeteoriteBoom )
					bFinished = m_pFxMeteoriteBoom->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_CHAOS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->num += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxChaos )
					m_pFxChaos->Render(*m_EfSortVector[i]);
			break;
		case FX_GATHERINGGUN:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxGatheringGun )
					m_pFxGatheringGun->Render(*m_EfSortVector[i]);
			break;
		case FX_DRAGONEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxDragonEffects )
					m_pFxDragonEffects->Render(*m_EfSortVector[i]);
			break;
		case FX_NUCLEARAIM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_NUCLEARAIM])
			{
				bFinished = FALSE;
				if( m_pFxNuclearAim )
					bFinished = m_pFxNuclearAim->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_NUCLEARAIM];
			}
			break;
		case FX_NUCLEARBOOM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_NUCLEARBOOM])
			{
				bFinished = FALSE;
				if( m_pFxNuclearBoom )
					bFinished = m_pFxNuclearBoom->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_NUCLEARBOOM];
			}
			break;
		case FX_DEATHWAVEAIM:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_DEATHWAVEAIM])
			{
				bFinished = FALSE;
				if( m_pFxDeathWaveAim )
					bFinished = m_pFxDeathWaveAim->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_DEATHWAVEAIM];
			}
			break;
		case FX_CHARWAVE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->num == 0 )
				m_EfSortVector[i]->Index += nAddFrame;
			else if( m_EfSortVector[i]->num == 1 )
				m_EfSortVector[i]->Index -= nAddFrame;

			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_CHARWAVE])
			{
				bFinished = FALSE;
				if( m_pFxCharWave )
					bFinished = m_pFxCharWave->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_CHARWAVE];
			}
			break;
		case FX_PHENIX:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_PHENIX])
			{
				bFinished = FALSE;
				if( m_pFxPhenix )
					bFinished = m_pFxPhenix->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_PHENIX];
			}
			break;
		case FX_SPRAYPARTICLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxSprayParticle )
					bFinished = m_pFxSprayParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SPRAYPARTICLE];
			}
			break;
		case FX_TURNPARTICLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxTurnParticle )
					bFinished = m_pFxTurnParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_TURNPARTICLE];
			}
			break;
		case FX_LIGHTPARTICLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxLightParticle )
					bFinished = m_pFxLightParticle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LIGHTPARTICLE];
			}
			break;
		case FX_HITPETEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxHitPetEffects )
					bFinished = m_pFxHitPetEffects->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_KHANHITEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxKhanHitEffects )
					bFinished = m_pFxKhanHitEffects->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_TELEPORT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxTeleport )
					bFinished = m_pFxTeleport->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_WEAPONEFFECTS:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;
			if( m_pFxWeaponEffects )
				m_pFxWeaponEffects->Render(*m_EfSortVector[i]);
			break;
		case FX_RUNSMOKE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxRunSmoke )
					bFinished = m_pFxRunSmoke->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_WARGIANTMULTI:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxWarGiantMulti )
					bFinished = m_pFxWarGiantMulti->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_WARGIANTATTACK:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxWarGiantAttack )
					bFinished = m_pFxWarGiantAttack->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_WARGIANTDIE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxWarGiantDie )
					bFinished = m_pFxWarGiantDie->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_MOBSHOWUP:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxMobShowUp )
					bFinished = m_pFxMobShowUp->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_POWERUP:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->num == 0 )
				m_EfSortVector[i]->Index += nAddFrame;
			else if( m_EfSortVector[i]->num == 1 )
				m_EfSortVector[i]->Index -= nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxPowerUp )
					bFinished = m_pFxPowerUp->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_GRYPHON:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxGryphon )
					bFinished = m_pFxGryphon->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_DRAGON:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxDragon )
					bFinished = m_pFxDragon->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_MOBPARTICLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			bFinished = FALSE;
			if( m_pFxMobParticle )
				bFinished = m_pFxMobParticle->Render(*m_EfSortVector[i]);
			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];

			break;
		case FX_GHOSTHUNT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->num = nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxGhostHunt )
					bFinished = m_pFxGhostHunt->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_GHOSTRISE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->num = nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxGhostRise )
					bFinished = m_pFxGhostRise->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_SOULDUST:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxSoulDust )
					m_pFxSoulDust->Render(*m_EfSortVector[i]);
			break;
		case FX_MAGICSWORD:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->num = nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxMagicSword )
					bFinished = m_pFxMagicSword->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_SPHINX:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxSphinx )
					bFinished = m_pFxSphinx->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_VAMPIRE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->num = nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxVampire )
					bFinished = m_pFxVampire->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_VAMPIREATTACK:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->num = nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				if( m_pFxVampireAttack )
					bFinished = m_pFxVampireAttack->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			}
			break;
		case FX_MUSICALNOTE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxMusicalNote )
					bFinished = m_pFxMusicalNote->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_FALLOFFSTONE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxFallOffStone )
					bFinished = m_pFxFallOffStone->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_DRILLSTONE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxDrillStone )
					bFinished = m_pFxDrillStone->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_STONERANCE2:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
				if( m_pFxStoneRance2 )
					m_pFxStoneRance2->Render(*m_EfSortVector[i]);
			break;
		case FX_CATCHFLAGDIR:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxCatchFlagDir )
					bFinished = m_pFxCatchFlagDir->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_DROPCANDY:// Drop Candy
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxDropCandy )
					m_pFxDropCandy->Render(*m_EfSortVector[i]);
			break;

		case FX_DROPAPPLE:// Drop Apple
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxDropApple )
					m_pFxDropApple->Render(*m_EfSortVector[i]);
			break;

		case FX_DROPAPPLEPVP:// Drop Apple
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxDropApplepvp )
					m_pFxDropApplepvp->Render(*m_EfSortVector[i]);
			break;

		case FX_DROPCANDYPVP:// Drop Candy
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxDropCandypvp )
					m_pFxDropCandypvp->Render(*m_EfSortVector[i]);
			break;

		case FX_DROPCANDYVIP:// Drop Candy
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxDropCandyvip )
					m_pFxDropCandyvip->Render(*m_EfSortVector[i]);
			break;


		case FX_DROPSTARCANDY: // 별사탕 이벤트
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxDropStarCandy )
					m_pFxDropStarCandy->Render(*m_EfSortVector[i]);
			break;
		//--------------------------------------------------------------------------------
		//중고렙 스킬 추가
		case FX_HELLCRY:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxHellCry )
					m_pFxHellCry->Render(*m_EfSortVector[i]);
			break;

		case FX_AIRRAID:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxAirRaid )
					m_pFxAirRaid->Render(*m_EfSortVector[i]);
			break;
		case FX_TORNADORUIN:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if( m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType] )
			{
				bFinished = FALSE;
				if( m_pFxTornadoRuin )
					bFinished = m_pFxTornadoRuin->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_SUMMONPARTITION:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if( m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType] )
			{
				bFinished = FALSE;
				if( m_pFxSumPart )
					m_pFxSumPart->Render(*m_EfSortVector[i]);
			}
			break;
		//--------------------------------------------------------------------------------
		case FX_DEVILRING:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxDevil )
					bFinished = m_pFxDevil->Render(*m_EfSortVector[i]);
			}
			break;
		// FireWork!!(Lyul)
		case FX_FIREWORKS :
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxFireWorks )
					m_pFxFireWorks->Render(*m_EfSortVector[i]);
			}
			break;
		// Lyul_2005 디바인 스톤
		case FX_DSTONE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxDstone )
					m_pFxDstone->Render(*m_EfSortVector[i]);
			}
			break;
		// Lyul_2005 더블 슬래쉬
		case FX_DSLASH:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxDslash )
					m_pFxDslash->Render(*m_EfSortVector[i]);
			}
			break;
		// Lyul_2005 배쉬
		case FX_BASH:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxBash )
					m_pFxBash->Render(*m_EfSortVector[i]);
			}
			break;
		// Lyul_2005 블레드
		case FX_BLEED:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxbleed )
					bFinished = m_pFxbleed->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_REFLECTION:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxReflection )
					bFinished = m_pFxReflection->Render(*m_EfSortVector[i]);
				if(bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_REFLECTION];
			}
			break;
		// Lyul_2005 디바인 스톤 몹 이펙트
		case FX_DSTONEEFF:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				int a;
				a = m_EfSortVector[i]->num;

				m_EfSortVector[i]->pMobFrom = pCMyApp->m_pUIMgr->FindMobById(m_EfSortVector[i]->num);
				bFinished = FALSE;
				if( m_pFxDstoneEff )
					bFinished = m_pFxDstoneEff->Render(*m_EfSortVector[i]);
			}
			break;
		// Lyul_2005 어썰트
		case FX_ASSAULT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if(m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxAssault )
					bFinished = m_pFxAssault->Render(*m_EfSortVector[i]);
			}
			break;
		//wan:2004-12 //몹이 쓰는 슬레이어
		case FX_MOB_SLAYER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_EARTHQUAKE])
			{
				bFinished = FALSE;
				if( m_pFxEarthQuake )
					bFinished = m_pFxEarthQuake->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_EARTHQUAKE];
			}
			break;
		case FX_DEMICRUSHER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_DEMICRUSHER])
			{
				bFinished = FALSE;
				if( m_pFxDemiCrusher )
					bFinished = m_pFxDemiCrusher->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_DEMICRUSHER];
			}
			break;
		//메트릭스 그랜드 스톤 효과 처리(현기)
		case FX_FIRETOWER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= m_pFrameCount[FX_FIRETOWER] )
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FIRETOWER]-1;
			bFinished = FALSE;
			if( m_pFxFireTower )
				bFinished = m_pFxFireTower->Render(*m_EfSortVector[i]);
			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FIRETOWER];
			break;
		//mungmae-2005/04/29 스톤버스트
		case FX_DSTONEBRUST:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxStoneBrust )
					bFinished = m_pFxStoneBrust->Render(*m_EfSortVector[i]);
			}
			break;
		//mungmae-2005/07/04 헬다운
		case FX_HELLDOWN:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= m_pFrameCount[FX_HELLDOWN] )
				m_EfSortVector[i]->nCurFrame = 0;	// Loop

			if( m_EfSortVector[i]->pNkChaFrom )
			{
				if( m_EfSortVector[i]->pNkChaFrom == g_pRoh && !pCMyApp->DrawCasting )
				{
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_HELLDOWN];
					break;
				}
				else
				{
					if( m_EfSortVector[i]->pNkChaFrom->m_nSkillStep == -1
							|| m_EfSortVector[i]->pNkChaFrom->m_nSkillStep == 10
							|| m_EfSortVector[i]->pNkChaFrom->m_nFailStep == -1 )
					{
						m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_HELLDOWN];
						break;
					}
				}

				if( m_pFxHellDown_Circle2 )
					m_pFxHellDown_Circle2->Render(*m_EfSortVector[i]);
				if( m_pFxHellDown_Around )
					m_pFxHellDown_Around->Render(*m_EfSortVector[i]);
				if( m_pFxHellDown_Spark )
					m_pFxHellDown_Spark->Render(*m_EfSortVector[i]);
				if( m_pFxHellDown_Circle )
					m_pFxHellDown_Circle->Render(*m_EfSortVector[i]);
				if( m_pFxHellDown_Lightly )
					m_pFxHellDown_Lightly->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_HELLDOWN2:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame <= 30 )
			{
				if( m_pFxHellDown_Circle )
					m_pFxHellDown_Circle->Render(*m_EfSortVector[i]);
				if( m_pFxHellDown_Circle2 )
					m_pFxHellDown_Circle2->Render(*m_EfSortVector[i]);
				pCMyApp->m_bHellDownSec = TRUE;
			}
			else if( m_EfSortVector[i]->nCurFrame > 30 )
			{
				int nCurFrame = m_EfSortVector[i]->nCurFrame - 30;
				if( m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_HELLDOWN])
				{
					int nLevelGrade = g_ChaSkill.m_SkillLevel[26] / 6;	//헬다운 레벨
					if( m_pFxHellDown_Lightning )
						m_pFxHellDown_Lightning->Render(*m_EfSortVector[i], m_EfSortVector[i]->nCurFrame );
					if( nCurFrame > 3 && nLevelGrade > 0)
						if( m_pFxHellDown_Lightning )
							m_pFxHellDown_Lightning->Render2(*m_EfSortVector[i], m_EfSortVector[i]->nCurFrame );
					if( nCurFrame > 6 && nLevelGrade > 1)
						if( m_pFxHellDown_Lightning )
							m_pFxHellDown_Lightning->Render3(*m_EfSortVector[i], m_EfSortVector[i]->nCurFrame );
					if( nCurFrame > 9 && nLevelGrade > 2)
						if( m_pFxHellDown_Lightning )
							m_pFxHellDown_Lightning->Render4(*m_EfSortVector[i], m_EfSortVector[i]->nCurFrame );
					if( nCurFrame > 12 && nLevelGrade > 3)
						if( m_pFxHellDown_Lightning )
							m_pFxHellDown_Lightning->Render5(*m_EfSortVector[i], m_EfSortVector[i]->nCurFrame );
				}
			}
			break;

		// nate 2005-06-15 : Auto Mine - 렌더링 호출
		case FX_AUTOMINE:

			if( !m_pFxMine || !m_EfSortVector[i]->pNkChaFrom )
				break;

			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[ FX_AUTOMINE ])
			{
				EffectSort TempEffect = *m_EfSortVector[i];
				TempEffect.subNum = 0;
				if( TempEffect.nCurFrame > 10 )
				{
					TempEffect.nCurFrame = m_EfSortVector[i]->nCurFrame - 10;
					if( m_EfSortVector[i]->subNum < 1 )
						TempEffect.subNum = 1;

					if( m_EfSortVector[i]->num > 0 )
					{
						TempEffect.Index = 0;
						if( m_pFxMine )
							bFinished = m_pFxMine->Render(TempEffect);
					}

					if( m_EfSortVector[i]->num > 7 )
					{
						TempEffect.nCurFrame = m_EfSortVector[i]->nCurFrame - 12;
						if( m_EfSortVector[i]->subNum < 2 )
							TempEffect.subNum = 2;	// 2 마인 실패

						if( TempEffect.nCurFrame >= 0 )
						{
							TempEffect.Index = 1;
							if( m_pFxMine )
								bFinished = m_pFxMine->Render(TempEffect);
						}
					}

					if( m_EfSortVector[i]->num > 15 )
					{
						if( m_EfSortVector[i]->subNum < 3 )
							TempEffect.subNum = 3;	// 3 마인 실패

						TempEffect.nCurFrame = m_EfSortVector[i]->nCurFrame - 14;
						if( TempEffect.nCurFrame >= 0 )
						{
							TempEffect.Index = 2;
							if( m_pFxMine )
								bFinished = m_pFxMine->Render(TempEffect);
						}
					}

					if( bFinished )
					{
						m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_AUTOMINE];
						if( m_pFxMine )
							m_pFxMine->SetArrivalMine( 0 );
					}
				}
			}
			else
			{
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_AUTOMINE];
				if( m_pFxMine )
					m_pFxMine->SetArrivalMine( 0 );
			}

			break;
		case FX_LIFEEXPENSION:	// nate 2005-07-06 : Life Expansion - FX Redner
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index = nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxLifeExpansion )
					bFinished = m_pFxLifeExpansion->Render(*m_EfSortVector[i]);
				if( bFinished )
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LIFEEXPENSION];
			}
			break;
		case FX_SUMMONSQUAD:	// nate 2005-07-08 : Summon Squad - FX Render
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			bFinished = FALSE;
			if( m_EfSortVector[i]->nCurFrame >= 0
					&& m_EfSortVector[i]->nCurFrame <= m_pFrameCount[m_EfSortVector[i]->nType] )
			{
				if( m_pFxSummonSquad && !m_pFxSummonSquad->Render(*m_EfSortVector[i]) )
					bFinished = TRUE;
			}
			else
				bFinished = TRUE;

			if( bFinished )
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SUMMONSQUAD];

			break;
		case FX_DURANT_BEAM:	// nate 2005-09-26 : 듀란트 - 공선
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_DURANT_BEAM])
			{
				bFinished = FALSE;
				if( m_pFxDurant_Beam )
					bFinished = m_pFxDurant_Beam->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_DURANT_BEAM];
			}
			else
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_DURANT_BEAM];
			break;

		case FX_DURANT_TARGET:	// nate 2005-09-26 : 듀란트 - 공격대상
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_DURANT_TARGET])
				if( m_pFxDurant_Target )
					m_pFxDurant_Target->Render(*m_EfSortVector[i]);
				else
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_DURANT_TARGET];
			break;

		case FX_POISON:	// nate 2005-11-03 : 포이즌
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_POISON])
				if( m_pFxPoison )
					m_pFxPoison->Render(*m_EfSortVector[i]);
				else
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_POISON];
			break;

		case FX_ADRENALINE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxAdrenaline )
					bFinished = m_pFxAdrenaline->Render(*m_EfSortVector[i]);
				if( bFinished )
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ADRENALINE];
			}
			break;

		case FX_ATTACK_MOVELOLO:	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_ATTACK_MOVELOLO])
			{
				if( m_pFxAttack_MoveLolo )
				{
					if( m_pFxAttack_MoveLolo->Render(*m_EfSortVector[i]) )
						m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ATTACK_MOVELOLO];
				}
			}
			else
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ATTACK_MOVELOLO];
			break;

		case FX_ATTACK_FIXLOLO:	// nate 2005-11-22 : 이펙트를 통합할 목적의 fx
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_ATTACK_FIXLOLO])
			{
				if( m_pFxAttack_FixLolo )
				{
					if( m_pFxAttack_FixLolo->Render(*m_EfSortVector[i]) )
						m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ATTACK_FIXLOLO];
				}
			}
			else
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ATTACK_FIXLOLO];
			break;

		case FX_FLYINGCRASH:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FLYINGCRASH])
			{
				bFinished = FALSE;
				if( m_pFxFlyingCrash )
					bFinished = m_pFxFlyingCrash->Render(*m_EfSortVector[i]);
				if( bFinished )
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ADRENALINE];
			}
			break;

		case FX_FLYINGCRASH2:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_FLYINGCRASH2])
			{
				if( m_pFxFlyingCrash2 )
					m_pFxFlyingCrash2->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_SHADOWSLASH:	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(시작시)
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SHADOWSLASH])
				if( m_pFxShadowSlash )
					m_pFxShadowSlash->Render(*m_EfSortVector[i]);
				else
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SHADOWSLASH];
			break;

		case FX_SHADOWSLASH2:	// mungmae 2005/12/01 : 하이브리더 - 새도우 슬래쉬(공격시)
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SHADOWSLASH2])
				if( m_pFxShadowSlash2 )
					m_pFxShadowSlash2->Render(*m_EfSortVector[i]);
				else
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SHADOWSLASH2];
			break;

		case FX_FOLLOW:	// 무언가를 따라다니는 종류의 이펙트. AI를 가질 수도 있다. ///페어리
			if( m_pFxFollow )
			{
				m_pFxFollow->FrameMove( *m_EfSortVector[i], nAddFrame ); // 이녀석은 타입, 애니메이션당 프레임이 다를수 있기때문에 이 안에서 프레임 루프를 돈다.
				m_pFxFollow->Render(*m_EfSortVector[i]);
			}

			break;

		case FX_MURAMASA_GHOST:	///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
			if( m_pFxMuramasaGhost )
			{
				bFinished = FALSE;
				m_pFxMuramasaGhost->FrameMove( *m_EfSortVector[i], nAddFrame ); // 이녀석은 타입, 애니메이션당 프레임이 다를수 있기때문에 이 안에서 프레임 루프를 돈다.
				bFinished = m_pFxMuramasaGhost->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_MURAMASA_GHOST];
			}
			break;

		case FX_FLAMESPEAR: ///플레임스피어
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			bFinished = FALSE;
			bFinished = m_pFxFlameSpear->Render(*m_EfSortVector[i]);
			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_FLAMESPEAR];

			break;

		case FX_PETSKILL: // 진화형 펫의 스킬 이펙트
			if( m_pFxPetSkill )
			{
				m_pFxPetSkill->FrameMove( *m_EfSortVector[i], nAddFrame ); // 이녀석은 타입, 애니메이션당 프레임이 다를수 있기때문에 이 안에서 프레임 루프를 돈다.

				if( m_pFxPetSkill->Render(*m_EfSortVector[i]) )
				{
					// TRUE가 리턴됐다는건 제거 될 때라는 거.
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_PETSKILL];
				}
			}

			break;

		case FX_TLVERTEX:	// nate 2005-01-17 : 이펙트를 통합할 목적의 fx
			/*
						m_EfSortVector[i]->nCurFrame += nAddFrame;
						if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_TLVERTEX])
						{
							if( m_pFxTLVertex )
							{
								if( m_pFxTLVertex->Render(*m_EfSortVector[i]) )
									m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_TLVERTEX];
							}
						}
						else
							m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_ATTACK_FIXLOLO];
			*/
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			bFinished = FALSE;
			bFinished = m_pFxTLVertex->Render(*m_EfSortVector[i]);
			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_TLVERTEX];

			break;


		case FX_SUMMONRING: ///불칸링
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			bFinished = FALSE;
			bFinished = m_pFxSummonRing->Render(*m_EfSortVector[i]);
			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SUMMONRING];

			break;

		case FX_SPEEDUPSMOKE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SPEEDUPSMOKE])
			{
				bFinished = FALSE;
				if( m_pFxSpeedUpSmoke )
					bFinished = m_pFxSpeedUpSmoke->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SPEEDUPSMOKE];
			}
			break;

		case FX_SPIRITMODE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SPIRITMODE])
				if( m_pFxSpiritMode )
					m_pFxSpiritMode->Render(*m_EfSortVector[i]);
				else
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SPIRITMODE];
			break;

		case FX_SPIRITMODE2:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame >= 0)
			{
				bFinished = FALSE;
				if( m_pFxSpiritMode2 )
					bFinished = m_pFxSpiritMode2->Render(*m_EfSortVector[i]);
				if( bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SPIRITMODE2];
			}
			break;
		case FX_SPACESLASH:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SPACESLASH])
			{
				if( m_pFxSpaceSlash )
					m_pFxSpaceSlash->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_ELECTRICHAMMER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_ELECTRICHAMMER])
			{
				if( m_pFxElectricHammer )
					m_pFxElectricHammer->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_SUMMONFIRE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SUMMONFIRE])
			{
				bFinished = FALSE;
				if( m_pFxSummonFire )
					bFinished = m_pFxSummonFire->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SUMMONFIRE];
			}
			break;
		case FX_BALLOON_KILL:		
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_BALLOON_KILL])
				{
					bFinished = FALSE;
					if( m_pFxBalloonKill )
						bFinished = m_pFxBalloonKill->Render(*m_EfSortVector[i]);
					if (bFinished)
						m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_SUMMONFIRE];
				}
			}
			break;
		case FX_BOSS_ATTACK_01:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				m_pFxBossAttack01->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_BOSS_ATTACK_02:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				m_pFxBossAttack02->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_BOSS_DEBUFF:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				m_pFxBossDebuff->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_BOSS_MAGIC:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				m_pFxBossMagic->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_RANDOMSHOOT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_RANDOMSHOOT])
			{
				bFinished = TRUE;			// 심심해서.. ^^;
				if( m_pFxRandomShoot )
					m_pFxRandomShoot->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_MERCERNERYSLASH:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_MERCERNERYSLASH])
			{
				if( m_pFxMercernerySlash )
					m_pFxMercernerySlash->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_ICEBLOT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_ICEBLOT])
			{
				if( m_pFxIceBlot )
					m_pFxIceBlot->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PROTECTIONSHIELD:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if (m_EfSortVector[i]->nCurFrame >= m_pFrameCount[FX_PROTECTIONSHIELD])
				{	
					m_EfSortVector[i]->nCurFrame = 0;
				}			

				if( m_pFxProtectionShield )
				{
					m_pFxProtectionShield->Render(*m_EfSortVector[i]);
				}
			}			
			break;
		case FX_SHIELDBLOCK:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_SHIELDBLOCK])
			{
				if( m_pFxShieldBlock )
					m_pFxShieldBlock->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_WILDSMASH:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_WILDSMASH])
			{
				if( m_pFxWildSmash )
					m_pFxWildSmash->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_RESURRECTION:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_RESURRECTION])
			{
				bFinished = FALSE;
				if( m_pFxResurrection )
					bFinished = m_pFxResurrection->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_RESURRECTION];
			}
			break;
		case FX_TORNADO_HAMMER:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_TORNADO_HAMMER])
			{
				bFinished = FALSE;
				if( m_pFxTornadoHammer )
					bFinished = m_pFxTornadoHammer->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_TORNADO_HAMMER];
			}
			break;
		case FX_POISON_FOG: ///인던1
//			m_EfSortVector[i]->nCurFrame += nAddFrame;

			bFinished = FALSE;
			bFinished = m_pFxPoisonFog->Render(*m_EfSortVector[i]);
			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_POISON_FOG];

			break;

		case FX_TYPE_PARTICLE: ///하트로드

			if( m_pFxTypeParticle )
			{
				m_pFxTypeParticle->FrameMove( *m_EfSortVector[i], nAddFrame ); // 이녀석은 타입, 애니메이션당 프레임이 다를수 있기때문에 이 안에서 프레임 루프를 돈다.

				if( m_pFxTypeParticle->Render(*m_EfSortVector[i]) )
				{
					// TRUE가 리턴됐다는건 제거 될 때라는 거.
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_TYPE_PARTICLE];
				}
			}

			break;

		// [3/26/2007 Theodoric] 태국 송크란 이벤트
		case FX_WTRSPRINKLE:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[FX_WTRSPRINKLE])
			{
				bFinished = FALSE;
				if( m_pFxWTRSprinkle )
					bFinished = m_pFxWTRSprinkle->Render(*m_EfSortVector[i]);
				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_WTRSPRINKLE];
			}
			break;

		// [8/7/2007 Theodoric] 6주년 기념 이벤트
		case FX_BABYSOUL:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxBabySoul )
					m_pFxBabySoul->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_DMITRON_PORTAL:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->nCurFrame %= m_pFrameCount[m_EfSortVector[i]->nType];
			bFinished = FALSE;
			if( m_pFxDmitronPortal )
				m_pFxDmitronPortal->Render(*m_EfSortVector[i]);
			break;

		case FX_DMITRON_CRYSTAL:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->nCurFrame %= m_pFrameCount[m_EfSortVector[i]->nType];
			bFinished = FALSE;
			if( m_pFxDmitronCrystal )
				m_pFxDmitronCrystal->Render(*m_EfSortVector[i]);
			break;

		case FX_BOSSRAID_BULKAN1_02:
			if( m_pFxBossRaidBulkan1_02 )
			{
				m_pFxBossRaidBulkan1_02->FrameMove(*m_EfSortVector[i], 0 );
				m_pFxBossRaidBulkan1_02->Render(*m_EfSortVector[i] );
			}
			break;

		case FX_BOSSRAID_BULKAN1_03:
			if( m_pFxBossRaidBulkan1_03 )
			{
				m_pFxBossRaidBulkan1_03->FrameMove(*m_EfSortVector[i], nAddFrame );
				m_pFxBossRaidBulkan1_03->Render(*m_EfSortVector[i] );
			}

		case FX_BOSSRAID_BULKAN2_02:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxBossRaidBulkan2_02 )
					m_pFxBossRaidBulkan2_02->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_BOSSRAID_BULKAN3_01:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			bFinished = FALSE;
			bFinished = m_pFxBossRaidBulkan3_02->Render(*m_EfSortVector[i]);

			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_BULKAN3_01];
			break;

		case FX_BOSSRAID_BULKAN3_02:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if( m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType] )
			{
				bFinished = FALSE;
				if( m_pFxTornadoRuin )
					bFinished = m_pFxBossRaidBulkan3_03->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_BOSSRAID_HUMAN1_02:
			if( m_pFxBossRaidHuman1_02 )
				bFinished = m_pFxBossRaidHuman1_02->Render(*m_EfSortVector[i]);
			if (bFinished)
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];
			break;

		case FX_BOSSRAID_HUMAN1_03:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->nCurFrame %= m_pFrameCount[m_EfSortVector[i]->nType];

			bFinished = FALSE;
			if( m_pFxBossRaidHuman1_03 )
			{
				m_pFxBossRaidHuman1_03->FrameMove(*m_EfSortVector[i], nAddFrame );
				bFinished = m_pFxBossRaidHuman1_03->Render(*m_EfSortVector[i] );
			}

			if( bFinished )
				m_EfSortVector[i]->nCurFrame = m_pFrameCount[m_EfSortVector[i]->nType];

			break;

		case FX_BOSSRAID_HUMAN2_02:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxBossRaidHuman2_02 )
					bFinished = m_pFxBossRaidHuman2_02->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_BOSSRAID_HUMAN2_03:
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
			{
				bFinished = FALSE;
				if( m_pFxBossRaidHuman2_03 )
					bFinished = m_pFxBossRaidHuman2_03->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_BOSSRAID_HUMAN3_02:					// [12/13/2007 반재승] 보스레이드 휴먼보스3-02 스킬
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidHuman3_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_HUMAN3_02];
			}
			break;
		case FX_BOSSRAID_HYBRI1_01:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidHybri1_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_HYBRI1_01];
			}
			break;
		case FX_BOSSRAID_HYBRI1_02:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidHybri1_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_HYBRI1_02];
			}
			break;
		case FX_BOSSRAID_HYBRI2_02:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				{
					bFinished = FALSE;

					m_pFxBossRaidHybri2_02->FrameMove(*m_EfSortVector[i], 0);

					if( m_pFxBossRaidHybri2_02 )
						bFinished = m_pFxBossRaidHybri2_02->Render(*m_EfSortVector[i]);
				}
			}
			break;

		case FX_BOSSRAID_HYBRI3_01:					// [1/16/2008 반재승] 보스레이드 하이브리더보스3-01 노멀어택
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidHybri3_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_HYBRI3_01];
			}
			break;

		case FX_BOSSRAID_HYBRI3_02:					// [01/17/2008 반재승] 보스레이드 하이브리더보스3, 2번 스킬
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidHybri3_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_HYBRI3_02];
			}
			break;

		case FX_BOSSRAID_KAI1_01:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai1_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI1_01];
			}
			break;

		case FX_BOSSRAID_KAI1_02:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai1_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI1_02];
			}
			break;

		case FX_BOSSRAID_KAI2_01:					// [2/5/2008 반재승] 카이립톤보스2 노멀 어택
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai2_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI2_01];
			}
			break;

		case FX_BOSSRAID_KAI2_02:					// [2/5/2008 반재승] 카이립톤보스2 2어택
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai2_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI2_02];
			}
			break;


		case FX_BOSSRAID_KAI2_03:					// [2/5/2008 반재승] 카이립톤보스2 2어택
			{
				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai2_03->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI2_03];
			}
			break;

		case FX_BOSSRAID_KAI3_01:					// [2/4/2008 반재승] 카이립톤보스3 노멀 어택
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai3_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI3_01];
			}
			break;

		case FX_BOSSRAID_KAI3_02:					// [2/4/2008 반재승] 카이립톤보스3 2번스킬
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai3_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI3_02];
			}
			break;

		case FX_BOSSRAID_KAI3_03:					// [2/11/2008 반재승] 카이립톤보스3 3번스킬
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidKai3_03->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_KAI3_03];
			}
			break;

		case FX_BOSSRAID_AIDIA1_01:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidAidia1_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA1_01];
			}
			break;

		case FX_BOSSRAID_AIDIA1_02:
			{
				bFinished = FALSE;
				m_pFxBossRaidAidia1_02->FrameMove(*m_EfSortVector[i], 0);
				bFinished = m_pFxBossRaidAidia1_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA1_02];
			}
			break;

		case FX_BOSSRAID_AIDIA1_02_SHOCKWAVE:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidAidia1_02_sw->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA1_02_SHOCKWAVE];
			}
			break;

		case FX_BOSSRAID_AIDIA2_01:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidAidia2_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA2_01];
			}
			break;

		case FX_BOSSRAID_AIDIA2_02:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidAidia2_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA2_02];
			}
			break;

		case FX_BOSSRAID_AIDIA2_03:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidAidia2_03->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA2_03];
			}
			break;

		case FX_BOSSRAID_AIDIA3_01:					// [3/6/2008 반재승] 에이디아보스3 노멀 어택
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidAidia3_01->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA3_01];
			}
			break;

		case FX_BOSSRAID_AIDIA3_02:					// [3/6/2008 반재승] 에이디아보스3 2번 스킬
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidAidia3_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA3_02];
			}
			break;

		case FX_BOSSRAID_AIDIA3_02_STONE:		// [3/6/2008 반재승] 에이디아보스3 2번 스킬 - 주변 돌덩어리
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxStoneRance->Render_EX(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_AIDIA3_02_STONE];
			}
			break;

		case FX_BOSSRAID_LASTBOSS_02:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidLastBoss_02->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_LASTBOSS_02];
			}
			break;

		case FX_BOSSRAID_LASTBOSS_03:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidLastBoss_03->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_LASTBOSS_03];
			}
			break;

		case FX_BOSSRAID_LASTBOSS_04:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;

				bFinished = FALSE;
				bFinished = m_pFxBossRaidLastBoss_04->Render(*m_EfSortVector[i]);

				if (bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_BOSSRAID_LASTBOSS_04];
			}
			break;

		case FX_HERO_EFFECT:
			{
				m_EfSortVector[i]->nCurFrame = 0;

				bFinished = FALSE;
				bFinished = m_pFxHeroEffect->Render(*m_EfSortVector[i]);

				if(bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_HERO_EFFECT];

			}
			break;

		case FX_SARNGA_MOUEL: // [12/3/2008 Theodoric] 사릉가
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_pFxSarngaMouel)
					bFinished = m_pFxSarngaMouel->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_YETI_RING: // AIDIA LEVEL 450
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_yeti_ring)
					bFinished = m_fx_yeti_ring->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEAKOCK_RING: // AIDIA LEVEL 500
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_peakock_ring)
					bFinished = m_fx_peakock_ring->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_TIGER_RING: // AIDIA LEVEL 550
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_tiger_ring)
					bFinished = m_fx_tiger_ring->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_UNICORN_RING: // AIDIA LEVEL 600
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_unicorn_ring)
					bFinished = m_fx_unicorn_ring->Render(*m_EfSortVector[i]);
			}
			break;
			case FX_650_RING: // AIDIA LEVEL 650
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_650_ring)
					bFinished = m_fx_650_ring->Render(*m_EfSortVector[i]);
			}
			break;
			case FX_700_RING: // AIDIA LEVEL 700
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_700_ring)
					bFinished = m_fx_700_ring->Render(*m_EfSortVector[i]);
			}
			break;
			case FX_750_RING: // AIDIA LEVEL 750
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_750_ring)
					bFinished = m_fx_750_ring->Render(*m_EfSortVector[i]);
			}
			break;
			case FX_800_RING: // AIDIA LEVEL 800
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_800_ring)
					bFinished = m_fx_800_ring->Render(*m_EfSortVector[i]);
			}
			break;
			case FX_850_RING: // AIDIA LEVEL 850
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_850_ring)
					bFinished = m_fx_850_ring->Render(*m_EfSortVector[i]);
			}
			break;
			case FX_900_RING: // AIDIA LEVEL 900
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_900_ring)
					bFinished = m_fx_900_ring->Render(*m_EfSortVector[i]);
			}
			break;
			case FX_950_RING: // AIDIA LEVEL 950
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_950_ring)
					bFinished = m_fx_950_ring->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_1000_RING: // AIDIA LEVEL 1000
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_1000_ring)
					bFinished = m_fx_1000_ring->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_CHR_RING: // christmas
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_CHR_ring)
					bFinished = m_fx_CHR_ring->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_CHIN_RING: // CHINESE
			m_EfSortVector[i]->nCurFrame += nAddFrame;
			m_EfSortVector[i]->Index += nAddFrame;

			if( m_EfSortVector[i]->nCurFrame >= 0 )
			{
				bFinished = FALSE;
				if( m_fx_CHIN_ring)
					bFinished = m_fx_CHIN_ring->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_ARUPAKA_SPIT:
			m_EfSortVector[i]->nCurFrame += nAddFrame;

			if (m_EfSortVector[i]->nCurFrame < m_pFrameCount[m_EfSortVector[i]->nType])
				if( m_pFxArupakaSpit )
					m_pFxArupakaSpit->Render(*m_EfSortVector[i]);
			break;

		case FX_LV400_EFFECT_GREEN:
		case FX_LV450_EFFECT_BLUE:
		case FX_LV500_EFFECT_RED:
		case FX_LV550_EFFECT_YELLOW:
		case FX_LV600_EFFECT_PURPLE:
		case FX_LV650_EFFECT_RAINBOW1:
		case FX_LV700_EFFECT_RAINBOW2:
		case FX_LV750_EFFECT_WHITE:
		case FX_LV800_EFFECT_BLACK:
		case FX_LV850_EFFECT_ORANGE:
		case FX_LV900_EFFECT_RAINBOW3:
		case FX_LV950_EFFECT_RAINBOW4:
		case FX_LV1000_EFFECT_PINK:
			{
				m_EfSortVector[i]->nCurFrame = 0;
				bFinished = FALSE;
				bFinished = m_pFxLv400Effect->Render(*m_EfSortVector[i]);

				if(bFinished)
				{
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_LV400_EFFECT_GREEN];
				}

			}
			break;

		case FX_PEROM_SKILL_S125:
		case FX_PEROM_SKILL_S126:
		case FX_PEROM_SKILL_S142:
		case FX_PEROM_SKILL_S143:
		case FX_PEROM_SKILL_S144:
		case FX_PEROM_SKILL_S145:
		case FX_PEROM_SKILL_S146:
		case FX_PEROM_SKILL_S147:
			break;

		case FX_PEROM_SKILL_S127:
			{
				bFinished = FALSE;
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_spin )
					bFinished = m_fx_perom_spin->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S128:
			{
				bFinished = FALSE;
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_thurst )
					bFinished = m_fx_perom_thurst->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S129:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_blader )
					bFinished = m_fx_perom_blader->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S130:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_strike )
					bFinished = m_fx_perom_strike->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S131:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_pierce )
					bFinished = m_fx_perom_pierce->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S132:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_boom )
					bFinished = m_fx_perom_boom->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S133:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_shotgun )
					bFinished = m_fx_perom_shotgun->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S135:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_pierce )
					bFinished = m_fx_perom_pierce->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S136:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_wheelwind )
					bFinished = m_fx_perom_wheelwind->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S137:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_alpha )
					bFinished = m_fx_perom_alpha->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S138:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				bFinished = m_fx_perom_cross->Render(*m_EfSortVector[i]);
				if(bFinished)
					m_EfSortVector[i]->nCurFrame = m_pFrameCount[FX_PEROM_SKILL_S138];
			}
			break;
		case FX_PEROM_SKILL_S140:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_overload )
					bFinished = m_fx_perom_overload->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S141:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_field )
					bFinished = m_fx_perom_field->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_PEROM_SKILL_S134:
			{
				// 필드...지속형이어야한다.
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_field )
					bFinished = m_fx_perom_field->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S148:
		case FX_PEROM_SKILL_S149:
		case FX_PEROM_SKILL_S150:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_field )
					bFinished = m_fx_perom_field->Render(*m_EfSortVector[i]);
			}
			break;
		case FX_PEROM_SKILL_S139:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				if( m_fx_perom_field )
					bFinished = m_fx_perom_field->Render(*m_EfSortVector[i]);
			}
			break;

		case FX_GRIFFON_RING:
			{
				m_EfSortVector[i]->nCurFrame += nAddFrame;
				m_EfSortVector[i]->Index += nAddFrame;

				if( m_EfSortVector[i]->nCurFrame >= 0 )
				{
					bFinished = FALSE;
					if( m_fx_griffon_ring)
						bFinished = m_fx_griffon_ring->Render(*m_EfSortVector[i]);
				}
			}
			break;
//////////////////////////////////////////////////////////////////////////
		}
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, dwZEnable);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, dwZWriteEnable);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MINFILTER, dwMin);
	GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, dwMag);

	deleteTimedEffect();
}

void CFxSet::deleteTimedEffect()
{
	// 프레임 수명이 다한 이펙트는 제거한다.
	vector<EffectSort*>::iterator prEffect;
	vector<EffectSort*>::iterator prEnd;
	EffectSort *pEffectSort;

	prEffect = m_EfSortVector.begin();

	prEnd = m_EfSortVector.end();
	for ( ; prEffect != m_EfSortVector.end(); prEffect++)
	{
		pEffectSort = *prEffect;
		if( !pEffectSort )
			continue;

		while (pEffectSort->bOneFrame)
		{
			// 매 프레임마다 호출되는 효고.
			prEffect = eraseEffect(prEffect);
			if (prEffect == m_EfSortVector.end())
				break;
			pEffectSort = *prEffect;
		}

		if(prEffect == m_EfSortVector.end())
			break;

		if (pEffectSort->nType == FX_CONDITIONEFFECTS )
		{
			if (pEffectSort->pNkChaTo != NULL &&
					pEffectSort->pNkChaTo->m_CancelState == FALSE )
			{
				pEffectSort->pNkChaTo->m_ShowConditionEffect = FALSE;
				prEffect = eraseEffect(prEffect);
				if( prEffect == m_EfSortVector.end() )
					break;
				pEffectSort = *prEffect;
			}
			else if (pEffectSort->pMobTo != NULL &&
					 pEffectSort->pMobTo->m_CancelState == FALSE )
			{
				pEffectSort->pMobTo->m_ShowConditionEffect = FALSE;
				prEffect = eraseEffect(prEffect);
				if( prEffect == m_EfSortVector.end() )
					break;
				pEffectSort = *prEffect;
			}
		}

		if (pEffectSort->nType == FX_CONDITIONEFFECTS2 )
		{
			if (pEffectSort->pNkChaTo != NULL &&
					pEffectSort->pNkChaTo->m_CancelState2 == FALSE )
			{
				pEffectSort->pNkChaTo->m_ShowConditionEffect2 = FALSE;
				prEffect = eraseEffect(prEffect);
				if( prEffect == m_EfSortVector.end() )
					break;
				pEffectSort = *prEffect;
			}
			else if (pEffectSort->pMobTo != NULL &&
					 pEffectSort->pMobTo ->m_CancelState2 == FALSE )
			{
				pEffectSort->pMobTo->m_ShowConditionEffect2 = FALSE;
				prEffect = eraseEffect(prEffect);
				if( prEffect == m_EfSortVector.end() )
					break;
				pEffectSort = *prEffect;
			}
		}
		else if (pEffectSort->nCurFrame >= m_pFrameCount[pEffectSort->nType])
		{
			// 디스플레이할 프레임 번호가 다 지나간 경우.
			Tail_Info *tail_Info = NULL;
			int num = 0;


			switch (pEffectSort->nType)
			{
			case FX_FOLLOW: // 페어리는 여기서 지우지 않는다.(페어리는 프레임은 무한 반복이다.) ///페어리.
				continue;

			case FX_PEROM_SKILL_S139:
				continue;
			case FX_VAMPIREATTACK:
			case FX_VAMPIRE:
			case FX_MAGICSWORD:
			case FX_GHOSTHUNT:
			case FX_GHOSTRISE:
				if( pEffectSort->effects )
				{
					tail_Info = (Tail_Info* )pEffectSort->effects;

					for( num = 0; num < pEffectSort->Index; num++ )
						SAFE_DELETE_ARRAY( tail_Info[num].pTailPos );

					delete [] tail_Info;
					pEffectSort->effects = tail_Info = NULL;
				}
				break;

			case FX_FIREPARTICLE: // 이 종류라도 순간나오고 사라지는 타입만 여기로 들어온다.
			case FX_TYPE_PARTICLE: ///하트로드 // 이 녀석은 이펙트 내에서 지우게 해봤다.

				if( pEffectSort->effects )
				{
					Deco_Particle *Fire_Particle = NULL; ///플레임스피어 때 추가.

					Fire_Particle = (Deco_Particle* )pEffectSort->effects;
					delete [] Fire_Particle;
					pEffectSort->effects = Fire_Particle = NULL;
				}
				break;

			case FX_BOSSRAID_BULKAN1_02:
				{

				}
				break;

			case FX_BOSSRAID_BULKAN2_02:
				if ( pEffectSort->pParticle )
				{
					delete [] pEffectSort->pParticle;
					pEffectSort->effects = NULL;
				}
				break;

			case FX_BOSSRAID_HUMAN1_02:

				break;

			case FX_BOSSRAID_HUMAN1_03:
				if( pEffectSort->pParticle )
				{
					delete [] pEffectSort->pParticle;
					pEffectSort->effects = NULL;
				}
				break;

			case FX_BOSSRAID_HUMAN2_02:
				break;

			case FX_BOSSRAID_HUMAN2_03:
				if ( pEffectSort->pParticle )
				{
					delete [] pEffectSort->pParticle;
					pEffectSort->effects = NULL;
				}
				break;

			case FX_BOSSRAID_HYBRI2_02:
				{
					if( pEffectSort->pParticle )
					{
						Deco_Particle* pTempParticle = NULL;
						pTempParticle = pEffectSort->pParticle;
						delete [] pTempParticle;
						pEffectSort->pParticle = pTempParticle = NULL;
					}
				}
				break;

			case FX_BOSSRAID_AIDIA1_02:
				{
					if( pEffectSort->pParticle )
					{
						Deco_Particle* pTempParticle = NULL;
						pTempParticle = pEffectSort->pParticle;
						delete [] pTempParticle;
						pEffectSort->pParticle = pTempParticle = NULL;
					}
				}
				break;

			case FX_BOSSRAID_AIDIA2_03:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_BOSSRAID_LASTBOSS_02:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_BOSSRAID_LASTBOSS_03:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_BOSSRAID_LASTBOSS_04:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_DROPSTARCANDY: // 별사탕 이벤트
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_HERO_EFFECT: // [2008/4/28 Theodoric] 보스레이드 영웅 이펙트
				{
					if( pEffectSort->effects )
					{
						delete [] pEffectSort->effects;
						pEffectSort =  NULL;
					}

				}
				break;
			case FX_LV400_EFFECT_GREEN:
			case FX_LV450_EFFECT_BLUE:
			case FX_LV500_EFFECT_RED:
			case FX_LV550_EFFECT_YELLOW:
			case FX_LV600_EFFECT_PURPLE:
			case FX_LV650_EFFECT_RAINBOW1:
			case FX_LV700_EFFECT_RAINBOW2:
			case FX_LV750_EFFECT_WHITE:
			case FX_LV800_EFFECT_BLACK:
			case FX_LV850_EFFECT_ORANGE:
			case FX_LV900_EFFECT_RAINBOW3:
			case FX_LV950_EFFECT_RAINBOW4:
			case FX_LV1000_EFFECT_PINK:
				{
					if( pEffectSort->effects )
					{
						delete [] pEffectSort->effects;
						pEffectSort =  NULL;
					}

				}
				break;

			default:
				break;
			}

			prEffect = eraseEffect(prEffect);
			if( prEffect == m_EfSortVector.end() )
				break;
			pEffectSort = *prEffect;
		}
	}
}

CFxSet::VEC_EFFSORT::iterator CFxSet::eraseEffect(VEC_EFFSORT::iterator it, BOOL bRemoveParentsList)
{
	MAP_EFFSORT::iterator itFind = m_mapEfSort.find((*it)->nNum);
	if( itFind != m_mapEfSort.end() )
	{
		m_mapEfSort.erase(itFind);
	}

	EffectSort* pES = *it;

	if( bRemoveParentsList == TRUE )
	{
		if( pES->parentType == eEffectParentType_Char )
		{
			CNkCharacter* pCh = (CNkCharacter*) pES->ptrParent;
			if( pCh )
			{
				pCh->DeleteEffectNum(pES->nNum);
			}
		}
	}

	SAFE_DELETE(pES);

	return m_EfSortVector.erase(it);
}

int CFxSet::InsertFxEffect(EffectSort ef_sort)
{
	ef_sort.parentType = 0;
	ef_sort.ptrParent = NULL;

	switch (ef_sort.nType)
	{
	case FX_UNARMEDATTACKED:					// 호출하는 객체내에서 프레임 관리를 한다.
	case FX_SHOOT:
	case FX_REDFIRE:
		ef_sort.bOneFrame = TRUE;
		break;

	case FX_SHOOTATTACKED:				// has two types of attacked.
		break;

	case FX_MOBSHOWUP:
	case FX_DRAGONEFFECTS:
	case FX_FLAME:
	case FX_SKILL_LIGHT:
	case FX_SCROLL_LIGHT:
	case FX_LEVELUP:
	case FX_SHOWUP2:
		ef_sort.bOneFrame = FALSE;
		break;

	case FX_SUMMONRING: ///불칸링

		if( ef_sort.num == EFFECT_SKILL_RING_BULKAN ) // 불칸링은 프레임을 좀 중간부터 시작하게 해준다.
			ef_sort.nCurFrame = 5;

		break;

	case FX_PETSKILL:  // 프레임번호를 -로 줘서 딜레이를 주고싶은 녀석은 default에 안들어가게 해준다.
		ef_sort.bOneFrame = FALSE;
		break;

	case FX_WARGIANTATTACK: // 이녀석은 프레임 초기화를 안한다. 프레임을 직접 넣어줘야한다.(프레임가지고 타이밍 장난을 좀 치기때문) ///인던1시 추가
		break;

	case FX_DMITRON_PORTAL:
		ef_sort.bOneFrame = FALSE;
		ef_sort.nCurFrame = 0;							// 효과내에서 프레임 관리를 한다.
		break;

	case FX_DMITRON_CRYSTAL:
		ef_sort.bOneFrame = FALSE;
		ef_sort.nCurFrame = 0;							// 효과내에서 프레임 관리를 한다.
		break;

	case FX_BOSSRAID_BULKAN1_02:
		{
			ef_sort.nCurFrame	= 0;
			ef_sort.num			= 0;			// 바로 이펙트가 방출되는 것이 아니라 지정된 프레임에서 이펙트가 방출된다.

			CNkMob* pMob = ef_sort.pMobFrom;

			ef_sort.vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, 0.0f,ef_sort.pNkChaTo->m_wz);	// 목표 지점
		}
		break;

	case FX_BOSSRAID_BULKAN2_02:
		{
			ef_sort.bOneFrame = FALSE;
			ef_sort.nCurFrame = 0;
		}
		break;

	case FX_BOSSRAID_BULKAN1_03:
		{
			ef_sort.nCurFrame	= 0;
			ef_sort.num			= 0;			// 바로 이펙트가 방출되는 것이 아니라 지정된 프레임에서 이펙트가 방출된다.

			CNkMob* pMob = ef_sort.pMobFrom;

			ef_sort.vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, 0.0f,ef_sort.pNkChaTo->m_wz);	// 목표 지점
		}
		break;

	case FX_BOSSRAID_HUMAN1_02:
		{
			ef_sort.nCurFrame	= 0;
			ef_sort.num			= 0;			// 바로 이펙트가 방출되는 것이 아니라 지정된 프레임에서 이펙트가 방출된다.

			CNkMob* pMob = ef_sort.pMobFrom;

			ef_sort.vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, 0.0f,ef_sort.pNkChaTo->m_wz);	// 목표 지점
			ef_sort.vCurPos	   = D3DVECTOR( pMob->m_wx, 0.0f, pMob->m_wz);							// 시작 지점
		}
		break;

	case FX_BOSSRAID_HUMAN1_03:
		{
			ef_sort.nCurFrame	= 0;
			ef_sort.num			= 0;			// 바로 이펙트가 방출되는 것이 아니라 지정된 프레임에서 이펙트가 방출된다.

			// 파티클과 앞으로 나아가는 lolo 파일의 속성을 가질 메모리르 생성하자.
			EffectSort*	pTempEffectSort	= new EffectSort[FX_BOSSRAID_HUMAN1_03_MAX_COUNT];
			ZeroMemory( pTempEffectSort, sizeof(EffectSort) * FX_BOSSRAID_HUMAN1_03_MAX_COUNT);
			ef_sort.effects	= pTempEffectSort;

			CNkMob* pMob = ef_sort.pMobFrom;

			EffectSort* pEfsort	= (EffectSort*)ef_sort.effects;
			ef_sort.vTargetPos	= D3DVECTOR( ef_sort.pNkChaTo->m_wx, ef_sort.pNkChaTo->m_wy ,ef_sort.pNkChaTo->m_wz);		// 목표 지점
			pEfsort->vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, ef_sort.pNkChaTo->m_wy ,ef_sort.pNkChaTo->m_wz);		// 목표 지점
		}
		break;

	case FX_BOSSRAID_HUMAN2_02:
		{
			ef_sort.bOneFrame = FALSE;
			ef_sort.nCurFrame = 0;

			CNkMob* pMob = ef_sort.pMobFrom;
			ef_sort.vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, 0.0f,ef_sort.pNkChaTo->m_wz);	// 목표 지점
		}
		break;

	case FX_BOSSRAID_HUMAN2_03:
		{
			ef_sort.bOneFrame = FALSE;
			ef_sort.nCurFrame = 0;

			CNkMob* pMob = ef_sort.pMobFrom;
			ef_sort.vTargetPos = D3DVECTOR( ef_sort.pNkChaTo->m_wx, 0.0f,ef_sort.pNkChaTo->m_wz);	// 목표 지점
		}
		break;

	case FX_BOSSRAID_HYBRI2_02:
		{
			Deco_Particle * pTempDecoParticle	= new Deco_Particle[FX_BOSSRAID_HYBRIDER2_02_MAX_PARTICLE];
			ef_sort.pParticle	= pTempDecoParticle;
			CNkMob* pMob = ef_sort.pMobFrom;
		}
		break;

	case FX_BOSSRAID_KAI2_03:
		{
			ef_sort.nCurFrame	= 0;
			ef_sort.num			= 0;			// 바로 이펙트가 방출되는 것이 아니라 지정된 프레임에서 이펙트가 방출된다.
		}
		break;

	case FX_BOSSRAID_AIDIA1_02:
		{
			// 파티클에 쓰일 구조체 메모리를 생성하자.
			int memorysize = FX_BOSSRAID_AIDIA1_02_MAXCOUNT_1 + FX_BOSSRAID_AIDIA1_02_MAXCOUNT_2;
			Deco_Particle *pTempDecoParticle = new Deco_Particle[memorysize];
			ZeroMemory( pTempDecoParticle, sizeof(Deco_Particle) * memorysize);
			ef_sort.pParticle = pTempDecoParticle;

			ef_sort.nCurFrame	= 0;
			ef_sort.num			= 0;
		}
		break;

	case FX_BOSSRAID_AIDIA3_02:					// [3/6/2008 반재승] 에이디아보스3 2번 스킬
		{
			ef_sort.bOneFrame = FALSE;
			ef_sort.nCurFrame	= 0;
			ef_sort.num			= 0;
		}

		break;

	case FX_BOSSRAID_LASTBOSS_02:
		{
			ef_sort.bOneFrame = FALSE;
			ef_sort.nCurFrame = 0;
		}
		break;

	case FX_BOSSRAID_LASTBOSS_03:
		{
			ef_sort.bOneFrame = FALSE;
			ef_sort.nCurFrame = 0;
		}
		break;

	case FX_BOSSRAID_LASTBOSS_04:
		{
			ef_sort.bOneFrame = FALSE;
			ef_sort.nCurFrame = 0;
		}
		break;

	case FX_HERO_EFFECT:	// [2008/4/28 Theodoric] 보스레이드 영웅 이펙트
		{
			// 파티클과 앞으로 나아가는 lolo 파일의 속성을 가질 메모리르 생성하자.
			Deco_Particle * pTempDecoParticle	= new Deco_Particle[MAX_HEROEFFECT_PLANE];
			ZeroMemory( pTempDecoParticle, sizeof(Deco_Particle) * MAX_HEROEFFECT_PLANE);
			ef_sort.effects	= pTempDecoParticle;

			ef_sort.num = 0;
		}
		break;
	case FX_LV400_EFFECT_GREEN:
	case FX_LV450_EFFECT_BLUE:
	case FX_LV500_EFFECT_RED:
	case FX_LV550_EFFECT_YELLOW:
	case FX_LV600_EFFECT_PURPLE:
	case FX_LV650_EFFECT_RAINBOW1:
	case FX_LV700_EFFECT_RAINBOW2:
	case FX_LV750_EFFECT_WHITE:
	case FX_LV800_EFFECT_BLACK:
	case FX_LV850_EFFECT_ORANGE:
	case FX_LV900_EFFECT_RAINBOW3:
	case FX_LV950_EFFECT_RAINBOW4:
	case FX_LV1000_EFFECT_PINK:
		{
			Deco_Particle * pTempDecoParticle	= new Deco_Particle[MAX_EFFECT_PLANE];
			ZeroMemory( pTempDecoParticle, sizeof(Deco_Particle) * MAX_EFFECT_PLANE);
			ef_sort.effects	= pTempDecoParticle;
			ef_sort.num = 0;
		}
		break;

	case FX_PEROM_SKILL_S139:
		{
			_asm nop;
		}
		break;
	// 몬가 넣어야겠지?
//////////////////////////////////////////////////////////////////////////
	default:
		ef_sort.bOneFrame = FALSE;
		ef_sort.nCurFrame = 0;							// 효과내에서 프레임 관리를 한다.
		break;
	}

	m_nEfIdx++;									// 특수효과의 고유아이디.
	ef_sort.nNum = m_nEfIdx;

	EffectSort* pES = new EffectSort(ef_sort);
	m_EfSortVector.push_back(pES);
	m_mapEfSort.insert( std::make_pair( m_nEfIdx, *m_EfSortVector.rbegin() ) );

	return m_nEfIdx;
}

void CFxSet::InsertTestEffect(int test_num)
{
	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	switch (test_num)
	{
	case 1:
		ef_sort.nType = FX_CATCHFLAG;
		ef_sort.pNkChaTo = g_pRoh;		// 맞는 놈.

		ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		break;

	case 2:
		ef_sort.nType = FX_SHOCKSPEAR;
		ef_sort.vTargetPos = D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		ef_sort.pNkChaTo = g_pRoh;		// 맞는 놈.

		ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx, pCMyApp->GetLandPtr()->GetHFHeight(g_pRoh->m_wx, g_pRoh->m_wz), g_pRoh->m_wz);
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		break;

	case 3:
		ef_sort.nType = FX_STONERANCE;
		ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		break;

	case 4:
		ef_sort.nType = FX_PEBBLESHOT;
		ef_sort.vTargetPos = D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		ef_sort.pNkChaTo = g_pRoh;		// 맞는 놈.
		ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx-50, g_pRoh->m_wy+20, g_pRoh->m_wz-50);
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		break;

	case 5:
		ef_sort.nType = FX_FSHOWER;
		ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx+20.0f, g_pRoh->m_wy+40.0f, g_pRoh->m_wz+20.0f);
		ef_sort.vTargetPos = D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		InsertFxEffect(ef_sort);
		break;

	default:
		break;
	}
}

void CFxSet::DeleteEffect(int num, BOOL bRemoveParentsList)
{
	// 프레임 수명이 다한 이펙트는 제거한다.
	vector<EffectSort*>::iterator prEffect;
	EffectSort *pEffectSort;

	prEffect = m_EfSortVector.begin();

	for ( ; prEffect != m_EfSortVector.end(); prEffect++)
	{
		pEffectSort = *prEffect;

		if (pEffectSort->nNum == num)
		{
			Tail_Info *tail_Info = NULL;
			int num = 0;

			switch (pEffectSort->nType)
			{
			case FX_VAMPIREATTACK:
			case FX_VAMPIRE:
			case FX_MAGICSWORD:
			case FX_GHOSTHUNT:
			case FX_GHOSTRISE:
				if( pEffectSort->effects )
				{
					tail_Info = (Tail_Info* )pEffectSort->effects;
					for( num = 0; num < pEffectSort->Index; num++ )
						SAFE_DELETE_ARRAY( tail_Info[num].pTailPos );

					delete [] tail_Info;
					pEffectSort->effects = tail_Info = NULL;
				}
				break;

			case FX_FOLLOW: ///뱀파이어망토 // 이 녀석은 렌더에선 지울 필요가 없다.

				if( pEffectSort->effects )
				{
					if( pEffectSort->Index == FX_FOLLOW_TYPE_FAIRY ) // 페어리
					{
						Deco_Particle *effect_temp = NULL;

						effect_temp = (Deco_Particle* )pEffectSort->effects;
						delete [] effect_temp;
						pEffectSort->effects = effect_temp = NULL;
					}
					else if( pEffectSort->Index == FX_FOLLOW_TYPE_VAMPIREBAT ) // 박쥐
					{
						list<D3DVECTOR> *effect_temp = NULL;

						effect_temp = (list<D3DVECTOR>* )pEffectSort->effects;
						delete effect_temp;
						pEffectSort->effects = effect_temp = NULL;
					}
					else if( pEffectSort->Index == FX_FOLLOW_TYPE_RING_LIGHT ) // [12/6/2007 반재승] 일본 링아이템 이펙트
					{
						list<D3DVECTOR> *effect_temp = NULL;

						effect_temp = (list<D3DVECTOR>* )pEffectSort->effects;
						delete effect_temp;
						pEffectSort->effects = effect_temp = NULL;

						m_pFxFollow->SetRing_Light_Color( -1 );
					}
				}
				break;

			case FX_BOSSRAID_BULKAN1_02:
				{
				}
				break;

			case FX_BOSSRAID_BULKAN2_02:
				{
				}
				break;

			case FX_BOSSRAID_HUMAN1_02:
				break;

			case FX_BOSSRAID_HUMAN1_03:
				if( pEffectSort->effects )
				{
					EffectSort* pTempEffectSort = NULL;
					pTempEffectSort = (EffectSort*)pEffectSort->effects;
					delete [] pTempEffectSort;
					pEffectSort->effects = pTempEffectSort = NULL;
				}
				break;

			case FX_BOSSRAID_HYBRI2_02:
				{
					if( pEffectSort->pParticle )
					{
						Deco_Particle* pTempParticle = NULL;
						pTempParticle = pEffectSort->pParticle;
						delete [] pTempParticle;
						pEffectSort->pParticle = pTempParticle = NULL;
					}
				}
				break;

			case FX_BOSSRAID_AIDIA1_02:
				{
					if( pEffectSort->pParticle )
					{
						Deco_Particle* pTempParticle = NULL;
						pTempParticle = pEffectSort->pParticle;
						delete [] pTempParticle;
						pEffectSort->pParticle = pTempParticle = NULL;
					}
				}
				break;

			case FX_BOSSRAID_AIDIA2_03:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_BOSSRAID_LASTBOSS_02:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_BOSSRAID_LASTBOSS_03:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_BOSSRAID_LASTBOSS_04:
				{
					if ( pEffectSort->pParticle )
					{
						delete [] pEffectSort->pParticle;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_HERO_EFFECT:
				{
					if(pEffectSort->effects)
					{
						delete [] pEffectSort->effects;
						pEffectSort->effects = NULL;
					}
				}
				break;

			case FX_LV400_EFFECT_GREEN:
			case FX_LV450_EFFECT_BLUE:
			case FX_LV500_EFFECT_RED:
			case FX_LV550_EFFECT_YELLOW:
			case FX_LV600_EFFECT_PURPLE:
			case FX_LV650_EFFECT_RAINBOW1:
			case FX_LV700_EFFECT_RAINBOW2:
			case FX_LV750_EFFECT_WHITE:
			case FX_LV800_EFFECT_BLACK:
			case FX_LV850_EFFECT_ORANGE:
			case FX_LV900_EFFECT_RAINBOW3:
			case FX_LV950_EFFECT_RAINBOW4:
			case FX_LV1000_EFFECT_PINK:
				{
					if(pEffectSort->effects)
					{
						delete [] pEffectSort->effects;
						pEffectSort->effects = NULL;
					}
				}
				break;


			default:


				break;
			}

			prEffect = eraseEffect(prEffect, bRemoveParentsList);
			break;
		}
	}
}

EffectSort* CFxSet::GetEffect(int num)
{
	MAP_EFFSORT::iterator itFind = m_mapEfSort.find(num);
	if( itFind != m_mapEfSort.end() )
	{
		return (*itFind).second;
	}

	return NULL;
}

///튜토리얼 시 추가
void CFxSet::RenderFadeScreen( D3DCOLOR Fade_Color ) // 페이드용 화면 전체 덮는 판때기를 그려주는 함수. 원래 이 클래스에 있을 만한 녀석은 아니지만 별도로 빼기 애매해서 여기다 넣는다.
{
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	D3DTLVERTEX TLVec[4];

	TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,1024,0), 1, Fade_Color, 0, 0, 1.0f );
	TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0), 1, Fade_Color, 0, 0, 0 );
	TLVec[2] = D3DTLVERTEX( D3DVECTOR(1024,0,0), 1, Fade_Color, 0, 1.0f, 0 );
	TLVec[3] = D3DTLVERTEX( D3DVECTOR(1024,1024,0), 1, Fade_Color, 0, 1.0f, 1.0f );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);

	GET_D3DDEVICE()->SetTexture(0, NULL );
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
}

///////////////////////////////////////////////////////////////////////////////////////
// FxLight Classes
// nate 2004 - 8 ImageManager
#define FX_LIGHT_SIZE 10.0f

CFxLight::CFxLight()
{
	m_nTotalFrame = 20;

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-FX_LIGHT_SIZE, -FX_LIGHT_SIZE, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_LIGHT_SIZE, FX_LIGHT_SIZE, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( FX_LIGHT_SIZE, -FX_LIGHT_SIZE, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_LIGHT_SIZE, FX_LIGHT_SIZE, 0), 0xffffffff, 0, 1.0f, 0.0f );
}

CFxLight::~CFxLight()
{}

void CFxLight::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxLight::DeleteRes()
{}

void CFxLight::FrameMove()
{}

void CFxLight::Render(D3DVECTOR &vPos)
{
	static int nFrameCount = 0;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matWorld;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	// 여기서 부터
	D3DMATRIX ViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( ViewMatrix );
	// get the ViewMatrix from D3D
	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &ViewMatrix );

	D3DMATRIX InvViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( InvViewMatrix );
	// get the Inverse of the ViewMatrix, kinda like your reflection looking at you in the mirror
	D3DMath_MatrixInvert( InvViewMatrix, ViewMatrix );

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	InvViewMatrix._41 = vPos.x;
	InvViewMatrix._42 = vPos.y;
	InvViewMatrix._43 = vPos.z;

	D3DUtil_SetRotateZMatrix(matRz, g_2_PI * (float) nFrameCount / m_nTotalFrame);
	nFrameCount++;
	if (nFrameCount >= m_nTotalFrame)
		nFrameCount = 0;

	D3DMath_MatrixMultiply(InvViewMatrix, matRz, InvViewMatrix);

	// Set the new world transform and render the particle
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &InvViewMatrix );
	// 여기까지

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHT ] );
	//	lpSurface = m_pIndexedTexture->GetSurfaceByIdx(m_wTexIdx);
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////
// FxLight Classes
CFxPortal::CFxPortal()
{
	m_fPortalSizeX[0] = 10.0f;
	m_fPortalSizeY[0] = 10.0f;
	m_fPortalSizeX[1] = 15.0f;
	m_fPortalSizeY[1] = 15.0f;
	m_fPortalSizeX[2] = 10.0f;
	m_fPortalSizeY[2] = 10.0f;

	// 1번째 그림 네 개의 텍스처.
	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.0f, 0.5f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 0.5f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 0.0f );

	m_lverRect[4] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 0.5f );
	m_lverRect[5] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 0.0f );
	m_lverRect[6] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 1.0f, 0.5f );
	m_lverRect[7] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 1.0f, 0.0f );

	m_lverRect[8] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[9] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.0f, 0.5f );
	m_lverRect[10] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[11] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 0.5f );

	m_lverRect[12] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[13] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.5f, 0.5f );
	m_lverRect[14] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], -m_fPortalSizeY[0], 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[15] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], m_fPortalSizeY[0], 0), 0xffffffff, 0, 1.0f, 0.5f );

	// 2번째 그림.
	m_lverRect[16] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[1], -m_fPortalSizeY[1], 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[17] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[1], m_fPortalSizeY[1], 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[18] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[1], -m_fPortalSizeY[1], 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[19] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[1], m_fPortalSizeY[1], 0), 0xffffffff, 0, 1.0f, 0.0f );

	// 3번째 그림.
	m_lverRect[20] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[2], -m_fPortalSizeY[2], 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[21] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[2], m_fPortalSizeY[2], 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[22] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[2], -m_fPortalSizeY[2], 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[23] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[2], m_fPortalSizeY[2], 0), 0xffffffff, 0, 1.0f, 0.0f );
}

CFxPortal::~CFxPortal()
{}

void CFxPortal::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxPortal::DeleteRes()
{}

void CFxPortal::FrameMove()
{}

void CFxPortal::Render(D3DVECTOR &vPos)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	static int offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ZERO );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_SRCCOLOR );

	RenderRect(vPos, 0.0f);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxPortal::RenderRect(D3DVECTOR &vPos, float fRad)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;

	vViewVec = pCMyApp->GetEyeDirection();
	// 큰 사각형 그리기.
	float ry = atan2f(vViewVec.x, vViewVec.z) +fRad;
	float rx = -0.1745f * 3;							// 10도 기울기.

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y;
	matWorld._43 = vPos.z;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PORTAL ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], 2*m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], 2*m_fPortalSizeY[0], 0), 0xffffffff, 0, 1.0f, 0.0f );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

	// 작은 사각형 그리기.
	ry = atan2f(vViewVec.x, vViewVec.z) +fRad;
	rx = -0.1745f * 3 - 0.0872f;							// 10도 기울기.

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y;
	matWorld._43 = vPos.z;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fPortalSizeX[0], 2*m_fPortalSizeY[0], 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fPortalSizeX[0], 2*m_fPortalSizeY[0], 0), 0xffffffff, 0, 1.0f, 0.0f );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
}

///////////////////////////////////////////////////////////////////////////////////////
// FxSmoke Classes
CFxSmoke::CFxSmoke()
{
	int i;
	m_fSmokeSizeX[0] = 7.0f;
	m_fSmokeSizeX[1] = 6.0f;
	m_fSmokeSizeX[2] = 7.0f;
	m_fSmokeSizeX[3] = 6.0f;
	m_fSmokeSizeX[4] = 5.0f;
	m_fSmokeSizeX[5] = 6.0f;
	m_fSmokeSizeX[6] = 7.0f;
	m_fSmokeSizeX[7] = 6.0f;

	for (i=0; i < 8; i++)
		m_fSmokeSizeY[i] = m_fSmokeSizeX[i];

	m_ScaleVector[0].x = 1.0f;
	m_ScaleVector[0].y = 1.0f;
	m_ScaleVector[0].z = 0.0f;

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-m_ScaleVector[0].x, -m_ScaleVector[0].y, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_ScaleVector[0].x, m_ScaleVector[0].y, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( m_ScaleVector[0].x, -m_ScaleVector[0].y, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_ScaleVector[0].x, m_ScaleVector[0].y, 0), 0xffffffff, 0, 1.0f, 0.0f );

	for (i=0; i < 4; i++)
	{
		m_ScaleVector[i].x = m_lverRect[i].x;
		m_ScaleVector[i].y = m_lverRect[i].y;
	}

	m_nTotalFrame = 8;
}

CFxSmoke::~CFxSmoke()
{}

void CFxSmoke::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxSmoke::DeleteRes()
{}

void CFxSmoke::FrameMove()
{}

void CFxSmoke::Render(D3DVECTOR &vPos)
{
	static int nFrameCount = 0;
	static DWORD dwLastTime = 0;
	DWORD dwCurTime;

	//FrameMove(nFrameCount);
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matWorld;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	// 현재 프레임 수를 결정한다.
	dwCurTime = timeGetTime();
	if ( (dwCurTime - dwLastTime) > 125)
	{
		dwLastTime = dwCurTime;
		nFrameCount++;
		if (nFrameCount >= m_nTotalFrame)
			nFrameCount = 0;
	}

	vViewVec = pCMyApp->GetEyeDirection();
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);

	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y+11.0f;
	matWorld._43 = vPos.z;

	// 데이타 값 변형.
	for (int i=0; i < 4; i++)
	{
		m_lverRect[i].x = m_ScaleVector[i].x * m_fSmokeSizeX[nFrameCount] ;
		m_lverRect[i].y = m_ScaleVector[i].y * m_fSmokeSizeY[nFrameCount] ;
	}
	m_lverRect[0].tu = nFrameCount*0.125f;
	m_lverRect[1].tu = nFrameCount*0.125f;
	m_lverRect[2].tu = (nFrameCount+1)*0.125f;
	m_lverRect[3].tu = (nFrameCount+1)*0.125f;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SMOKE ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////
// CFxShadowRing
CFxShadowRing::CFxShadowRing()
{
	m_nShadowRingSize[0] = 20;
	m_nShadowRingSize[1] = 30;
	m_nShadowRingSize[2] = 40;
	m_nShadowRingSize[3] = 50;
	m_nShadowRingSize[4] = 50;
	m_nShadowRingSize[5] = 40;
	m_nShadowRingSize[6] = 30;
	m_nShadowRingSize[7] = 20;

	m_nTotalFrame = 8;
}

CFxShadowRing::~CFxShadowRing()
{}

void CFxShadowRing::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxShadowRing::DeleteRes()
{}

void CFxShadowRing::FrameMove()
{}

void CFxShadowRing::Render(D3DVECTOR &vPos)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	static int nFrameCount = 0;
	static DWORD dwLastTime = 0;
	DWORD dwCurTime;
	static int i = 0;

	dwCurTime = timeGetTime();
	if ( (dwCurTime - dwLastTime) > 200)
	{
		dwLastTime = dwCurTime;
		nFrameCount++;
		if (nFrameCount >= m_nTotalFrame)
			nFrameCount = 0;
	}

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWRING ] );
	// nate 2004 - 7 글로벌 ShadowRect사용
	g_pShadowRect->SetTileCoord(vPos, m_nShadowRingSize[nFrameCount], FALSE);		// 부울값은 시간인가 아닌가
	g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
}


///////////////////////////////////////////////////////////////////////////////////////
// CFxShadowLight
CFxShadowLight::CFxShadowLight()
{
	m_nShadowLightSize[0] = 100;
	m_nShadowLightSize[1] = 30;
	m_nShadowLightSize[2] = 40;
	m_nShadowLightSize[3] = 50;
	m_nShadowLightSize[4] = 80;
	m_nShadowLightSize[5] = 40;
	m_nShadowLightSize[6] = 30;
	m_nShadowLightSize[7] = 150;

	m_nTotalFrame = 8;
}

CFxShadowLight::~CFxShadowLight()
{}

void CFxShadowLight::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxShadowLight::DeleteRes()
{}

void CFxShadowLight::FrameMove()
{}

void CFxShadowLight::Render(D3DVECTOR &vPos)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	static int nFrameCount = 0;
	static DWORD dwLastTime = 0;
	DWORD dwCurTime;
	static int i = 0;

	dwCurTime = timeGetTime();
	if ( (dwCurTime - dwLastTime) > 200)
	{
		dwLastTime = dwCurTime;
		nFrameCount++;
		if (nFrameCount >= m_nTotalFrame)
			nFrameCount = 0;
	}

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] );
	// nate 2004 - 7 글로벌 ShadowRect사용
	g_pShadowRect->SetTileCoord(vPos, m_nShadowLightSize[nFrameCount], FALSE);		// 부울값은 시간인가 아닌가
	g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
}

///////////////////////////////////////////////////////////////////////////////////////
// FxParticle Classes
CFxParticle::CFxParticle()
{
	m_ParticleSystem  = NULL;
	m_ParticleSystem = new CParticleSystem();
	m_ParticleSystem->m_bIsSuppressed = false;
	LoadPreset(0);
}

CFxParticle::~CFxParticle()
{
	DeleteRes();
	SAFE_DELETE(m_ParticleSystem);
}

void CFxParticle::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxParticle::DeleteRes()
{}

void CFxParticle::LoadPreset(int iPreset)
{
	switch ( iPreset )
	{
	case 0:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r	= 1.0f;
		m_ParticleSystem->m_d3dcColorStart.g	= 0.8f;
		m_ParticleSystem->m_d3dcColorStart.b	= 0.5f;

		m_ParticleSystem->m_d3dcColorVar.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorEnd.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.b	= 1.0f;

		m_ParticleSystem->m_fAlphaStart		= 0.5f;
		m_ParticleSystem->m_fAlphaVar		= 0.0f;
		m_ParticleSystem->m_fAlphaEnd		= 0.0f;

		m_ParticleSystem->m_fSizeStart		= 4.0f;
		m_ParticleSystem->m_fSizeVar		= 0.0f;
		m_ParticleSystem->m_fSizeEnd		= 8.0f;

		m_ParticleSystem->m_fSpeed			= 15.0f;
		m_ParticleSystem->m_fSpeedVar		= 0.0f;

		m_ParticleSystem->m_fTheta			= 180.0f;

		m_ParticleSystem->m_fLife		= 1.0f;
		m_ParticleSystem->m_fLifeVar	= 0.5f;

		m_ParticleSystem->m_fGravityStart	= 0.0f;
		m_ParticleSystem->m_fGravityVar		= 0.0f;
		m_ParticleSystem->m_fGravityEnd		= 0.0f;

		m_ParticleSystem->m_uParticlesPerSec	= 80;

		m_ParticleSystem->m_bIsMoving		= FALSE;
		m_ParticleSystem->m_bIsAttractive	= FALSE;
		m_ParticleSystem->m_bIsColliding	= FALSE;

		m_ParticleSystem->m_uTextureID = 0;
		break;

	case 1:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r = 1.00f;
		m_ParticleSystem->m_d3dcColorStart.g = 1.00f;
		m_ParticleSystem->m_d3dcColorStart.b = 0.30f;

		m_ParticleSystem->m_d3dcColorVar.r = 0.00f;
		m_ParticleSystem->m_d3dcColorVar.g = 0.00f;
		m_ParticleSystem->m_d3dcColorVar.b = 0.00f;

		m_ParticleSystem->m_d3dcColorEnd.r = 0.00f;
		m_ParticleSystem->m_d3dcColorEnd.g = 1.00f;
		m_ParticleSystem->m_d3dcColorEnd.b = 0.00f;

		m_ParticleSystem->m_fAlphaStart = 0.96f;
		m_ParticleSystem->m_fAlphaVar   = 1.00f;
		m_ParticleSystem->m_fAlphaEnd   = 0.16f;

		m_ParticleSystem->m_fSizeStart  = 2.74f;
		m_ParticleSystem->m_fSizeVar    = 2.00f;
		m_ParticleSystem->m_fSizeEnd    = 6.09f;

		m_ParticleSystem->m_fSpeed      = 237.75f;
		m_ParticleSystem->m_fSpeedVar   = 50.00f;
		m_ParticleSystem->m_fTheta      = 44.13f;

		m_ParticleSystem->m_fLife       = 0.97f;
		m_ParticleSystem->m_fLifeVar    = 0.50f;

		m_ParticleSystem->m_fGravityStart  = -0.69f;
		m_ParticleSystem->m_fGravityVar    = 1.00f;
		m_ParticleSystem->m_fGravityEnd    = -0.49f;

		m_ParticleSystem->m_uParticlesPerSec = 206;

		m_ParticleSystem->m_bIsMoving     = 0;
		m_ParticleSystem->m_bIsAttractive = 0;
		m_ParticleSystem->m_bIsColliding  = 0;

		m_ParticleSystem->m_uTextureID = 0;

		break;

	case 2:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r	= 1.0f;
		m_ParticleSystem->m_d3dcColorStart.g	= 1.0f;
		m_ParticleSystem->m_d3dcColorStart.b	= 1.0f;

		m_ParticleSystem->m_d3dcColorVar.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorEnd.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.b	= 1.0f;

		m_ParticleSystem->m_fAlphaStart		= 1.0f;
		m_ParticleSystem->m_fAlphaVar		= 0.0f;
		m_ParticleSystem->m_fAlphaEnd		= 0.0f;

		m_ParticleSystem->m_fSizeStart		= 4.0f;
		m_ParticleSystem->m_fSizeVar		= 0.0f;
		m_ParticleSystem->m_fSizeEnd		= 0.5f;

		m_ParticleSystem->m_fSpeed			= 100.0f;
		m_ParticleSystem->m_fSpeedVar		= 10.0f;

		m_ParticleSystem->m_fTheta			= 45.0f;

		m_ParticleSystem->m_fLife		= 3.0f;
		m_ParticleSystem->m_fLifeVar	= 0.0f;

		m_ParticleSystem->m_fGravityStart	= 4.0f;
		m_ParticleSystem->m_fGravityVar		= 0.0f;
		m_ParticleSystem->m_fGravityEnd		= 4.0f;

		m_ParticleSystem->m_uParticlesPerSec	= 100;

		m_ParticleSystem->m_bIsMoving		= FALSE;
		m_ParticleSystem->m_bIsAttractive	= FALSE;
		m_ParticleSystem->m_bIsColliding	= TRUE;

		m_ParticleSystem->m_uTextureID = 0;
		break;

	case 3:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorStart.g	= 1.0f;
		m_ParticleSystem->m_d3dcColorStart.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorVar.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorEnd.r	= 1.0f;
		m_ParticleSystem->m_d3dcColorEnd.g	= 1.0f;
		m_ParticleSystem->m_d3dcColorEnd.b	= 1.0f;

		m_ParticleSystem->m_fAlphaStart		= 1.0f;
		m_ParticleSystem->m_fAlphaVar		= 0.0f;
		m_ParticleSystem->m_fAlphaEnd		= 1.0f;

		m_ParticleSystem->m_fSizeStart		= 10.0f;
		m_ParticleSystem->m_fSizeVar		= 0.0f;
		m_ParticleSystem->m_fSizeEnd		= 5.0f;

		m_ParticleSystem->m_fSpeed			= 50.0f;
		m_ParticleSystem->m_fSpeedVar		= 0.0f;

		m_ParticleSystem->m_fTheta			= 180.0f;

		m_ParticleSystem->m_fLife		= 4.0f;
		m_ParticleSystem->m_fLifeVar	= 0.0f;

		m_ParticleSystem->m_fGravityStart	= 0.0f;
		m_ParticleSystem->m_fGravityVar		= 0.0f;
		m_ParticleSystem->m_fGravityEnd		= 0.0f;

		m_ParticleSystem->m_uParticlesPerSec	= 50;

		m_ParticleSystem->m_bIsMoving		= FALSE;
		m_ParticleSystem->m_bIsAttractive	= TRUE;
		m_ParticleSystem->m_bIsColliding	= FALSE;

		m_ParticleSystem->m_uTextureID = 0;
		break;

	case 4:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r	= 1.0f;
		m_ParticleSystem->m_d3dcColorStart.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorStart.b	= 1.0f;

		m_ParticleSystem->m_d3dcColorVar.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorEnd.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.b	= 1.0f;

		m_ParticleSystem->m_fAlphaStart		= 1.0f;
		m_ParticleSystem->m_fAlphaVar		= 0.0f;
		m_ParticleSystem->m_fAlphaEnd		= 0.0f;

		m_ParticleSystem->m_fSizeStart		= 1.0f;
		m_ParticleSystem->m_fSizeVar		= 0.0f;
		m_ParticleSystem->m_fSizeEnd		= 10.0f;

		m_ParticleSystem->m_fSpeed			= 5.0f;
		m_ParticleSystem->m_fSpeedVar		= 0.0f;

		m_ParticleSystem->m_fTheta			= 180.0f;

		m_ParticleSystem->m_fLife		= 2.0f;
		m_ParticleSystem->m_fLifeVar	= 0.0f;

		m_ParticleSystem->m_fGravityStart	= 0.0f;
		m_ParticleSystem->m_fGravityVar		= 0.0f;
		m_ParticleSystem->m_fGravityEnd		= -1.0f;

		m_ParticleSystem->m_uParticlesPerSec	= 25;

		m_ParticleSystem->m_bIsMoving		= FALSE;
		m_ParticleSystem->m_bIsAttractive	= FALSE;
		m_ParticleSystem->m_bIsColliding	= FALSE;

		m_ParticleSystem->m_uTextureID = 1;
		break;

	case 5:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r	= 1.0f;
		m_ParticleSystem->m_d3dcColorStart.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorStart.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorVar.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorEnd.r	= 1.0f;
		m_ParticleSystem->m_d3dcColorEnd.g	= 1.0f;
		m_ParticleSystem->m_d3dcColorEnd.b	= 1.0f;

		m_ParticleSystem->m_fAlphaStart		= 0.25f;
		m_ParticleSystem->m_fAlphaVar		= 0.0f;
		m_ParticleSystem->m_fAlphaEnd		= 1.0f;

		m_ParticleSystem->m_fSizeStart		= 0.5f;
		m_ParticleSystem->m_fSizeVar		= 0.0f;
		m_ParticleSystem->m_fSizeEnd		= 2.0f;

		m_ParticleSystem->m_fSpeed			= 100.0f;
		m_ParticleSystem->m_fSpeedVar		=  50.0f;

		m_ParticleSystem->m_fTheta			= 45.0f;

		m_ParticleSystem->m_fLife		= 3.0f;
		m_ParticleSystem->m_fLifeVar	= 0.0f;

		m_ParticleSystem->m_fGravityStart	= 0.0f;
		m_ParticleSystem->m_fGravityVar		= 0.0f;
		m_ParticleSystem->m_fGravityEnd		= 0.0f;

		m_ParticleSystem->m_uParticlesPerSec	= 150;

		m_ParticleSystem->m_bIsMoving			= 0;
		m_ParticleSystem->m_bIsAttractive		= 0;
		m_ParticleSystem->m_bIsColliding		= 0;

		m_ParticleSystem->m_uTextureID = 4;
		break;

	case 6:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r = 1.00f;
		m_ParticleSystem->m_d3dcColorStart.g = 1.00f;
		m_ParticleSystem->m_d3dcColorStart.b = 0.30f;

		m_ParticleSystem->m_d3dcColorVar.r = 0.00f;
		m_ParticleSystem->m_d3dcColorVar.g = 0.00f;
		m_ParticleSystem->m_d3dcColorVar.b = 0.00f;

		m_ParticleSystem->m_d3dcColorEnd.r = 0.00f;
		m_ParticleSystem->m_d3dcColorEnd.g = 1.00f;
		m_ParticleSystem->m_d3dcColorEnd.b = 0.00f;

		m_ParticleSystem->m_fAlphaStart = 0.96f;
		m_ParticleSystem->m_fAlphaVar   = 1.00f;
		m_ParticleSystem->m_fAlphaEnd   = 0.16f;

		m_ParticleSystem->m_fSizeStart  = 2.74f;
		m_ParticleSystem->m_fSizeVar    = 2.00f;
		m_ParticleSystem->m_fSizeEnd    = 6.09f;

		m_ParticleSystem->m_fSpeed      = 14.71f;
		m_ParticleSystem->m_fSpeedVar   = 50.00f;

		m_ParticleSystem->m_fTheta     	= 74.12f;

		m_ParticleSystem->m_fLife		= 0.97f;
		m_ParticleSystem->m_fLifeVar	= 0.50f;

		m_ParticleSystem->m_fGravityStart  = -0.69f;
		m_ParticleSystem->m_fGravityVar    = 1.00f;
		m_ParticleSystem->m_fGravityEnd    = -0.49f;

		m_ParticleSystem->m_uParticlesPerSec = 441;

		m_ParticleSystem->m_bIsMoving     = 0;
		m_ParticleSystem->m_bIsAttractive = 0;
		m_ParticleSystem->m_bIsColliding  = 0;

		m_ParticleSystem->m_uTextureID = 0;
		break;

	case 7:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r = 0.00f;
		m_ParticleSystem->m_d3dcColorStart.g = 0.00f;
		m_ParticleSystem->m_d3dcColorStart.b = 0.00f;

		m_ParticleSystem->m_d3dcColorVar.r = 1.00f;
		m_ParticleSystem->m_d3dcColorVar.g = 1.00f;
		m_ParticleSystem->m_d3dcColorVar.b = 1.00f;

		m_ParticleSystem->m_d3dcColorEnd.r = 0.00f;
		m_ParticleSystem->m_d3dcColorEnd.g = 0.00f;
		m_ParticleSystem->m_d3dcColorEnd.b = 0.00f;

		m_ParticleSystem->m_fAlphaStart = 0.87f;
		m_ParticleSystem->m_fAlphaVar   = 1.00f;
		m_ParticleSystem->m_fAlphaEnd   = 0.89f;

		m_ParticleSystem->m_fSizeStart  = 0.59f;
		m_ParticleSystem->m_fSizeVar    = 0.00f;
		m_ParticleSystem->m_fSizeEnd    = 9.35f;

		m_ParticleSystem->m_fSpeed      = 188.73f;
		m_ParticleSystem->m_fSpeedVar   = 10.00f;
		m_ParticleSystem->m_fTheta      = 54.71f;

		m_ParticleSystem->m_fLife       = 0.78f;
		m_ParticleSystem->m_fLifeVar    = 0.00f;

		m_ParticleSystem->m_fGravityStart  = -0.20f;
		m_ParticleSystem->m_fGravityVar    = 0.00f;
		m_ParticleSystem->m_fGravityEnd    = -0.20f;

		m_ParticleSystem->m_uParticlesPerSec = 676;

		m_ParticleSystem->m_bIsMoving     = 0;
		m_ParticleSystem->m_bIsAttractive = 0;
		m_ParticleSystem->m_bIsColliding  = 0;

		m_ParticleSystem->m_uTextureID = 3;
		break;

	case 8:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r = 0.40f;
		m_ParticleSystem->m_d3dcColorStart.g = 0.40f;
		m_ParticleSystem->m_d3dcColorStart.b = 0.35f;

		m_ParticleSystem->m_d3dcColorVar.r = 0.00f;
		m_ParticleSystem->m_d3dcColorVar.g = 0.00f;
		m_ParticleSystem->m_d3dcColorVar.b = 0.00f;

		m_ParticleSystem->m_d3dcColorEnd.r = 0.75f;
		m_ParticleSystem->m_d3dcColorEnd.g = 1.00f;
		m_ParticleSystem->m_d3dcColorEnd.b = 1.00f;

		m_ParticleSystem->m_fAlphaStart = 0.78f;
		m_ParticleSystem->m_fAlphaVar   = 0.00f;
		m_ParticleSystem->m_fAlphaEnd   = 0.31f;

		m_ParticleSystem->m_fSizeStart  = 0.50f;
		m_ParticleSystem->m_fSizeVar    = 0.00f;
		m_ParticleSystem->m_fSizeEnd    = 5.53f;

		m_ParticleSystem->m_fSpeed      = 30.00f;
		m_ParticleSystem->m_fSpeedVar   = 0.00f;

		m_ParticleSystem->m_fTheta      = 90.00f;

		m_ParticleSystem->m_fLife       = 3.21f;
		m_ParticleSystem->m_fLifeVar    = 0.50f;

		m_ParticleSystem->m_fGravityStart  = -0.29f;
		m_ParticleSystem->m_fGravityVar    = 0.00f;
		m_ParticleSystem->m_fGravityEnd    = 0.10f;

		m_ParticleSystem->m_uParticlesPerSec = 59;

		m_ParticleSystem->m_bIsMoving     = 1;
		m_ParticleSystem->m_bIsAttractive = 0;
		m_ParticleSystem->m_bIsColliding  = 0;

		m_ParticleSystem->m_uTextureID = 2;
		break;

	default:
		//-- Particle System Settings ----
		m_ParticleSystem->m_d3dcColorStart.r	= 1.0f;
		m_ParticleSystem->m_d3dcColorStart.g	= 0.8f;
		m_ParticleSystem->m_d3dcColorStart.b	= 0.5f;

		m_ParticleSystem->m_d3dcColorVar.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorVar.b	= 0.0f;

		m_ParticleSystem->m_d3dcColorEnd.r	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.g	= 0.0f;
		m_ParticleSystem->m_d3dcColorEnd.b	= 1.0f;

		m_ParticleSystem->m_fAlphaStart		= 0.5f;
		m_ParticleSystem->m_fAlphaVar			= 0.0f;
		m_ParticleSystem->m_fAlphaEnd			= 0.0f;

		m_ParticleSystem->m_fSizeStart		= 4.0f;
		m_ParticleSystem->m_fSizeVar			= 0.0f;
		m_ParticleSystem->m_fSizeEnd			= 8.0f;

		m_ParticleSystem->m_fSpeed			= 15.0f;
		m_ParticleSystem->m_fSpeedVar			= 0.0f;

		m_ParticleSystem->m_fTheta			= 180.0f;

		m_ParticleSystem->m_fLife		= 1.0f;
		m_ParticleSystem->m_fLifeVar	= 0.5f;

		m_ParticleSystem->m_fGravityStart		= 0.0f;
		m_ParticleSystem->m_fGravityVar		= 0.0f;
		m_ParticleSystem->m_fGravityEnd		= 0.0f;

		m_ParticleSystem->m_uParticlesPerSec	= 80;

		m_ParticleSystem->m_bIsMoving = FALSE;
		m_ParticleSystem->m_bIsAttractive = FALSE;
		m_ParticleSystem->m_bIsColliding = FALSE;

		m_ParticleSystem->m_uTextureID = 0;
		break;
	}
}


void CFxParticle::FrameMove(float fTimeKey)
{
	static float fLastUpdate;
	m_ParticleSystem->Update(fTimeKey);
	fLastUpdate = fTimeKey;
}

void CFxParticle::Render(D3DVECTOR &vPos)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	// 파티클 업데이트를 위한 프레임 무브.
	static DWORD dwLastTime = 0;

	FLOAT fTimeKey = ( timeGetTime() - dwLastTime ) * 0.001f;
	dwLastTime = timeGetTime();
	FrameMove(fTimeKey);

	// setup our alpha blending mode, which will use the alpha values we assign to the vertices of the quad
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

	// render our particle system
	// 텍스처 설정.
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PARTICLE ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// 라이팅 조정.
	DWORD lightingOld;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &lightingOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	// setup our texture stages knowing that we want to do some alpha blending
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	m_ParticleSystem->Draw(vPos); //fix me

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, lightingOld);
}

///////////////////////////////////////////////////////////////////////////////////////
// FxLightFog Classes
// nate 2004 - 8 ImageManager
#define FX_LIGHTFOG_LIGHTSIZE 3.0f

CFxLightFog::CFxLightFog()
{
	m_nTotalFrame = 20;

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(FX_LIGHTFOG_LIGHTSIZE, -FX_LIGHTFOG_LIGHTSIZE, 0), 0x00ffffff, 0, 1.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTFOG_LIGHTSIZE, -FX_LIGHTFOG_LIGHTSIZE, 0), 0x00ffffff, 0, 0.0f, 1.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTFOG_LIGHTSIZE, FX_LIGHTFOG_LIGHTSIZE, 0), 0x00ffffff, 0, 0.0f, 0.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_LIGHTFOG_LIGHTSIZE, FX_LIGHTFOG_LIGHTSIZE, 0), 0x00ffffff, 0, 1.0f, 0.0f );

	m_lverRot[0] = m_lverRect[0];
	m_lverRot[1] = m_lverRect[1];
	m_lverRot[2] = m_lverRect[2];
	m_lverRot[3] = m_lverRect[3];

	m_nCurSparkles = CUR_SPARKLES;

	m_pSparkles = new Sparkle[MAX_SPARKLES];
	m_pIndices = new WORD[MAX_SPARKLES*6];
	m_pMesh = new D3DLVERTEX[MAX_SPARKLES*4];

	srand(time(0));

	// 입자가 생기는 바운드 영역 맥스값.(축의 반절값만..)
	m_vecBound = D3DVECTOR(12.0f, 13.0f, 12.0f);
}

CFxLightFog::~CFxLightFog()
{
	DeleteRes();

	SAFE_DELETE_ARRAY(m_pSparkles);
	SAFE_DELETE_ARRAY(m_pIndices);
	SAFE_DELETE_ARRAY(m_pMesh);
}

void CFxLightFog::LoadRes(IndexedTexture *pIndexedTexture)
{
	InitSparkles();
}

void CFxLightFog::InitSparkles()
{
	int i = 0;

	for (i=0; i < MAX_SPARKLES; i++)
		m_pSparkles[i] = RandomSparkle();

	for (i=0; i < MAX_SPARKLES; i++)
	{
		m_pIndices[i*6+0] = 4*i + 0;
		m_pIndices[i*6+1] = 4*i + 1;
		m_pIndices[i*6+2] = 4*i + 2;
		m_pIndices[i*6+3] = 4*i + 0;
		m_pIndices[i*6+4] = 4*i + 2;
		m_pIndices[i*6+5] = 4*i + 3;
	}
}

Sparkle CFxLightFog::RandomSparkle()
{
	Sparkle ret;

	ret.position = D3DVECTOR(m_vecBound.x * (rnd()-rnd()), m_vecBound.y * (rnd()-rnd()),  m_vecBound.z * (rnd()-rnd()));
	ret.color = D3DVECTOR(rnd(), rnd(), rnd());
	ret.velocity = D3DVECTOR(0,0,0);

	return ret;
}

void CFxLightFog::DeleteRes()
{}

void CFxLightFog::FrameMove(float fTimeKey)
{}

void CFxLightFog::Render(D3DVECTOR &vPos)
{
	static int nFrameCount = 0;
	static float g_fSaveTime = 0.0f;
	int i = 0;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRyRev, matRySet, matRz, matWorld, matRot;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	// 회전 및 월드 트랜스폼.
	D3DUtil_SetRotateYMatrix(matRySet, g_2_PI * (float) nFrameCount / m_nTotalFrame);
	D3DUtil_SetRotateYMatrix(matRyRev, -g_2_PI * (float) nFrameCount / m_nTotalFrame);

	g_fSaveTime += g_fTimeKey;

	if ( g_fSaveTime > 0.05f)
	{
		nFrameCount++;
		if (nFrameCount < m_nTotalFrame/2)
			m_nCurSparkles = m_nCurSparkles + nFrameCount*2;
		else
			m_nCurSparkles = m_nCurSparkles - nFrameCount*2;

		if (m_nCurSparkles < 0)
			m_nCurSparkles = 1;

		g_fSaveTime = 0.0f;
	}

	assert(m_nCurSparkles < MAX_SPARKLES);

	if (nFrameCount >= m_nTotalFrame)
	{
		nFrameCount = 0;
		m_nCurSparkles = CUR_SPARKLES;
	}

	D3DUtil_SetIdentityMatrix(matWorld);

	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y+m_vecBound.y;		// y축 보정.
	matWorld._43 = vPos.z;

	D3DMath_MatrixMultiply(matWorld, matRySet, matWorld);

	vViewVec = pCMyApp->GetEyeDirection();
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DMath_MatrixMultiply(matRx, matRx, matRyRev);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matRot, matRx, matRy);

	// 기본 사각형을 카메라를 향하게 회전.
	NKMath_LVertexMatrixMultiply(m_lverRot[0], m_lverRect[0], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[1], m_lverRect[1], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[2], m_lverRect[2], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[3], m_lverRect[3], matRot);

	// 렌더링 단계 시작.
	int num = 0;
	for (i=0; i<m_nCurSparkles; i++)
	{
		D3DCOLOR	color = D3DRGB(m_pSparkles[i].color.x, m_pSparkles[i].color.y, m_pSparkles[i].color.z);
		D3DVECTOR position = m_pSparkles[i].position;

		for (int j=0; j < 4; j++)
		{
			m_pMesh[num*4+j].x = position.x + m_lverRot[j].x;
			m_pMesh[num*4+j].y = position.y + m_lverRot[j].y;
			m_pMesh[num*4+j].z = position.z + m_lverRot[j].z;

			m_pMesh[num*4+j].color = color;
			m_pMesh[num*4+j].specular = 0;
			m_pMesh[num*4+j].tu = m_lverRot[j].tu;
			m_pMesh[num*4+j].tv = m_lverRot[j].tv;
		}
		num++;
	}

	// 월드 트랜스폼.
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTFOG ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	GET_D3DDEVICE()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DFVF_LVERTEX, (LPVOID)m_pMesh, m_nCurSparkles*4 , m_pIndices, m_nCurSparkles*6, 0);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////
// CFxShowup Class
CFxShowup::CFxShowup()
{
	m_nTotalFrame = FX_SHOWUP_FRAMECOUNT;
	m_fRadius = 3.0f;				// 빛덩어리의 크기.
	m_fCycleRadius = 5.5f;
	m_fHeight = m_fRadius;			// 지면에서 원의 크기 높이부터 시작.
	m_pMesh = new D3DLVERTEX[4*FX_SHOWUPRINGNUM];
	m_pIndices = new WORD[6*FX_SHOWUPRINGNUM];

	m_nStartRingSize[0] = 20;
	m_nStartRingSize[1] = 30;
	m_nStartRingSize[2] = 47;
	m_nStartRingSize[3] = 50;
	m_nStartRingSize[4] = 50;
	m_nStartRingSize[5] = 45;
	m_nStartRingSize[6] = 35;
	m_nStartRingSize[7] = 20;
	m_nStartRingSize[8] = 5;
	m_nStartRingSize[9] = 0;

	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(m_fRadius, -m_fRadius, 0), 0x00ffffff, 0, 1.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fRadius, -m_fRadius, 0), 0x00ffffff, 0, 0.0f, 1.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR(-m_fRadius, m_fRadius, 0), 0x00ffffff, 0, 0.0f, 0.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fRadius, m_fRadius, 0), 0x00ffffff, 0, 1.0f, 0.0f );

	float x = 0.0f, z = 0.0f;
	int i = 0;

	for (i=0; i < FX_SHOWUPRINGNUM; i++)
	{
		x = m_fCycleRadius*cosf(i*1.0471f);		// 1.0471f == 60도.
		z = m_fCycleRadius*sinf(i*1.0471f);

		m_ShowupRing[i].position.x = x;
		m_ShowupRing[i].position.y = 0.0f;
		m_ShowupRing[i].position.z = z;
		m_ShowupRing[i].color = D3DVECTOR(rnd(), rnd(), rnd());
	}

	for (i=0; i < FX_SHOWUPRINGNUM; i++)
	{
		m_pIndices[i*6+0] = 4*i + 0;
		m_pIndices[i*6+1] = 4*i + 1;
		m_pIndices[i*6+2] = 4*i + 2;
		m_pIndices[i*6+3] = 4*i + 0;
		m_pIndices[i*6+4] = 4*i + 2;
		m_pIndices[i*6+5] = 4*i + 3;
	}

	D3DVECTOR vColor = D3DVECTOR(rnd()/2+0.5f, rnd()/2+0.5f, rnd()/2+0.5f);
	m_lverRect[0].color = D3DRGB(vColor.x, vColor.y, vColor.z);
	m_lverRect[1].color = m_lverRect[2].color = m_lverRect[3].color = m_lverRect[0].color;

	memcpy(m_lverRot, m_lverRect, sizeof(m_lverRect));
}

CFxShowup::~CFxShowup()
{
	DeleteRes();
	SAFE_DELETE_ARRAY(m_pMesh);
	SAFE_DELETE_ARRAY(m_pIndices);
}

void CFxShowup::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxShowup::DeleteRes()
{}

// 해당 프레임을 그려준다.
void CFxShowup::Render(D3DVECTOR &vPos, int nFrame)
{
	int i = 0, j = 0;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRyCycle, matRyRev, matWorld, matRot;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	if (nFrame >= m_nTotalFrame)
	{
		MessageBox(NULL, "The frame count is over.", "Test", MB_OK);
		return;
	}

	// z-buffer를 enable시키고 write는 하지 못하게 한다.
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	// 원환을 그려주는 위치.
	m_fHeight = m_fRadius + 1.5f * nFrame;

	// 먼저 바닥부터 그림.
	if (nFrame < 10)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUPSTART ] );

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->SetTileCoord(vPos, m_nStartRingSize[nFrame], FALSE);
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	// 바닥 그리기 끝.

	// 싸이클 회전각 계산과 빌보드 회전 보정각 계산.
	D3DUtil_SetRotateYMatrix(matRyCycle, g_2_PI * (float) nFrame / 10);
	D3DUtil_SetRotateYMatrix(matRyRev, -g_2_PI * (float) nFrame / 10);

	// 빌보드 회전.
	vViewVec = pCMyApp->GetEyeDirection();
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DMath_MatrixMultiply(matRx, matRx, matRyRev);		// 싸이클 회전에 의한 보정.
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matRot, matRx, matRy);

	// 기본 사각형을 카메라를 향하게 회전.
	NKMath_LVertexMatrixMultiply(m_lverRot[0], m_lverRect[0], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[1], m_lverRect[1], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[2], m_lverRect[2], matRot);
	NKMath_LVertexMatrixMultiply(m_lverRot[3], m_lverRect[3], matRot);

	// 그릴 메쉬 데이타
	for (i=0; i < FX_SHOWUPRINGNUM; i++)
	{
		D3DCOLOR fColor = D3DRGB(m_ShowupRing[i].color.x, m_ShowupRing[i].color.y, m_ShowupRing[i].color.z);
		float x, z;
		x = m_fCycleRadius*cosf(i*1.0471f);		// 1.0471f == 60도.
		z = m_fCycleRadius*sinf(i*1.0471f);

		m_ShowupRing[i].position.x = x;
		m_ShowupRing[i].position.y = 0.0f;
		m_ShowupRing[i].position.z = z;

		for (j=0; j < 4; j++)
		{
			m_pMesh[i*4+j].x = m_ShowupRing[i].position.x + m_lverRot[j].x;
			m_pMesh[i*4+j].y = m_ShowupRing[i].position.y + m_lverRot[j].y;
			m_pMesh[i*4+j].z = m_ShowupRing[i].position.z + m_lverRot[j].z;

			m_pMesh[i*4+j].color = fColor;
			m_pMesh[i*4+j].specular = 0.0f;
			m_pMesh[i*4+j].tu = m_lverRot[j].tu;
			m_pMesh[i*4+j].tv = m_lverRot[j].tv;
		}
	}

	// 월드공간 이동 좌표.
	D3DUtil_SetIdentityMatrix(matWorld);
	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y+m_fHeight+1.0f;		// y축 보정.
	matWorld._43 = vPos.z;

	D3DMath_MatrixMultiply(matWorld, matRyCycle, matWorld);

	// 월드 트랜스폼.
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOWUP ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,D3DFVF_LVERTEX, (LPVOID)m_pMesh, FX_SHOWUPRINGNUM*4 , m_pIndices, FX_SHOWUPRINGNUM*6, 0);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////
// CFxLighting Class
// nate 2004 - 8 ImageManager
#define FX_LIGHTNING_LWIDTH 8.0f
#define FX_LIGHTNING_LHEIGHT 80.0f

CFxLightning::CFxLightning()
{
	m_nTotalFrame = FX_LIGHTNING_FRAMECOUNT;
	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );

	m_lverRect[4] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[5] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );
	m_lverRect[6] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[7] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );

	m_lverRect[8] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[9] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );
	m_lverRect[10] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[11] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );

	m_lverRect[12] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[13] = D3DLVERTEX( D3DVECTOR(-FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );
	m_lverRect[14] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[15] = D3DLVERTEX( D3DVECTOR( FX_LIGHTNING_LWIDTH, FX_LIGHTNING_LHEIGHT, 0), 0xffffffff, 0, 1.0f, 0.0f );

	// 바닥에 그려지는 폭파
	m_nSparkSize[0] = 30;
	m_nSparkSize[1] = 40;
	m_nSparkSize[2] = 57;
	m_nSparkSize[3] = 60;
	m_nSparkSize[4] = 60;
	m_nSparkSize[5] = 55;
	m_nSparkSize[6] = 45;
	m_nSparkSize[7] = 30;
	m_nSparkSize[8] = 15;
	m_nSparkSize[9] = 10;
}

CFxLightning::~CFxLightning()
{
	DeleteRes();
}

void CFxLightning::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxLightning::DeleteRes()
{}

void CFxLightning::Render(D3DVECTOR &vPos, int nFrame, int nType )
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	static int offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	if (nFrame < 10)
	{
		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = 0.0f;

		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// 텍스처 택하기.
		// nate 2004 - 7 - image
		if( nType == 0 )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNING ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float scale, fade;

		scale = (float)nFrame / FX_LIGHTNING_FRAMECOUNT;
		fade = 1.0f - (scale * 0.75f);

		float red, green, blue;
		red = green = blue = 0.0f;

		red = 0.85f - (0.85f * scale * 0.55f);
		green = 0.85f - (0.85f * scale * 0.55f);
		blue = 1.0f - (1.0f * scale * 0.55f);

		m_lverRect[offset*4].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 1].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 2].color =D3DRGBA(red, green, blue, fade);
		m_lverRect[offset*4 + 3].color =D3DRGBA(red, green, blue, fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRect[offset*4], 4, 0 );
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	// 먼저 바닥부터 그림.
	if (nFrame < 10)
	{
		D3DUtil_SetIdentityMatrix(matWorld);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;		// y축 보정.
		matWorld._43 = vPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNINGSPARK ] );

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame]*0.6, FALSE);
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPos, m_nSparkSize[nFrame], FALSE);
	}

	// 번개 잔상 그리기.
	int i;
	if (nFrame < 10)
	{
		for (i=0; i < 10; i++)
			RenderFrag2(vPos, nFrame);
	}
	else
	{
		for (i=m_nTotalFrame-nFrame+5; i > 0; i-=2)
			RenderFrag2(vPos, nFrame);
	}

	offset++;
	if (offset > 3)
		offset = 0;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
}

void CFxLightning::RenderFrag(D3DVECTOR &vPos, int nFrame)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matWorld;

	// 임시 변수
	D3DVECTOR vRand, vInvRand, vOri;
	float fWidth = 4.0f;

	vRand = D3DVECTOR(4.0f, 10.0f, 0.0f);
	vOri = D3DVECTOR(0.0f, 0.0f, 0.0f);

	vInvRand = vOri - vRand;

	vInvRand.x = vInvRand.y;
	vInvRand.y = vInvRand.x;
	vInvRand.z = 0.0f;

	vInvRand = Normalize(vInvRand);
	vInvRand *= fWidth;
	// 임시 변수 끝.

	// 몇번째 사각형을 쓸 것인가를 결정.
	int nWhich = 0;
	nWhich = (int) (rnd()*3.0f + 0.5f);
	vViewVec = pCMyApp->GetEyeDirection();
	// 큰 사각형 그리기.
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = 0.0f;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y;
	matWorld._43 = vPos.z;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNING ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float scale, fade;

	scale = (float)nFrame / FX_LIGHTNING_FRAMECOUNT;
	fade = 1.0f - (scale * 0.75f);

	float red, green, blue;
	red = green = blue = 0.0f;

	red = 0.45f - (0.45f * scale * 0.95f);
	green = 0.45f - (0.45f * scale * 0.95f);
	blue = 0.9f - (0.9f * scale * 0.95f);

	m_lverFrag[0] = D3DLVERTEX( D3DVECTOR(vOri.x + vInvRand.x, vOri.y - vInvRand.y, vOri.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 1.0f );
	m_lverFrag[1] = D3DLVERTEX( D3DVECTOR(vRand.x + vInvRand.x, vRand.y - vInvRand.y, vRand.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 0.0f );
	m_lverFrag[2] = D3DLVERTEX( D3DVECTOR( vOri.x - vInvRand.x, vOri.y + vInvRand.y, vOri.z ),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 1.0f );
	m_lverFrag[3] = D3DLVERTEX( D3DVECTOR( vRand.x - vInvRand.x, vRand.y + vInvRand.y, vRand.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 0.0f );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverFrag, 4, 0 );
}

void CFxLightning::RenderFrag2(D3DVECTOR &vPos, int nFrame)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matId, matWorld, matView, matViewOld;

	// 임시 변수
	D3DVECTOR vRand, vInvVec, vOri, vVec, vVec2;
	float fThetha = 0.0f;
	float fWidth = 10.0f;

	fThetha = rnd() * g_2_PI;
	vRand.x = cosf(fThetha)*fWidth*(rnd()+0.2f);
	vRand.z = sinf(fThetha)*fWidth*(rnd()+0.2f);
	vRand.y = rnd() * 13.0f;
	vOri = D3DVECTOR(0.0f, 0.0f, 0.0f);

	D3DUtil_SetIdentityMatrix(matWorld);
	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y;
	matWorld._43 = vPos.z;

	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matViewOld);

	D3DMath_MatrixMultiply(matView, matWorld, matViewOld);

	D3DMath_VectorMatrixMultiply(vVec, vOri, matView);
	D3DMath_VectorMatrixMultiply(vVec2, vRand, matView);

	vInvVec = vVec;
	vInvVec -= vVec2;

	float x_temp = vInvVec.x;
	vInvVec.x = vInvVec.y;
	vInvVec.y = x_temp;
	vInvVec.z = 0.0f;

	vInvVec = Normalize(vInvVec);
	vInvVec *= fWidth;
	// 임시 변수 끝.

	// 몇번째 사각형을 쓸 것인가를 결정.
	int nWhich = 0;
	nWhich = (int) (rnd()*3.0f + 0.5f);

	D3DUtil_SetIdentityMatrix(matId);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matId );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matId );
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTNING ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float scale, fade;

	scale = (float)nFrame / FX_LIGHTNING_FRAMECOUNT;
	fade = 1.0f - (scale * 0.75f);

	float red, green, blue;
	red = green = blue = 0.0f;

	red = 0.35f - (0.35f * scale * 0.85f);
	green = 0.35f - (0.35f * scale * 0.85f);
	blue = 0.8f - (0.8f * scale * 0.85f);

	m_lverFrag[0] = D3DLVERTEX( D3DVECTOR(vVec.x + vInvVec.x, vVec.y - vInvVec.y, vVec.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 1.0f );
	m_lverFrag[1] = D3DLVERTEX( D3DVECTOR(vVec2.x + vInvVec.x, vVec2.y - vInvVec.y, vVec2.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*nWhich, 0.0f );
	m_lverFrag[2] = D3DLVERTEX( D3DVECTOR( vVec.x - vInvVec.x, vVec.y + vInvVec.y, vVec.z ),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 1.0f );
	m_lverFrag[3] = D3DLVERTEX( D3DVECTOR( vVec2.x - vInvVec.x, vVec2.y + vInvVec.y, vVec2.z),
								D3DRGBA(red, green, blue, fade), 0, 0.25f*(nWhich+1), 0.0f );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverFrag, 4, 0 );

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_VIEW, &matViewOld );
}

///////////////////////////////////////////////////////////////////////////////////////
// CFxLightBall Class
CFxLightBall::CFxLightBall()
{
	m_fSize = 3.5f;
	m_fSpeed = 7.0f;
}

CFxLightBall::~CFxLightBall()
{
	DeleteRes();
}

void CFxLightBall::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxLightBall::DeleteRes()
{}

BOOL CFxLightBall::Render(EffectSort &effect_sort)
{
	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	unsigned int offset, i;
	BOOL bFinished = FALSE;

	float ry = 0.0f, rx = 0.0f;

	vViewVec = pCMyApp->GetEyeDirection();

	ry = atan2f(vViewVec.x, vViewVec.z);
	rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	int nFrame;
	nFrame = effect_sort.nCurFrame;

	if (nFrame == 0)
		offset = 0;
	else
		offset = nFrame % 4;

	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// 라이트볼 마법은 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= m_fSpeed;

	// 현재 프레임에서 7프레임 전까지 그린다.
	int start_frame, end_frame;

	start_frame = nFrame - 6;		// 총 7프레임.

	if (start_frame < 0)
		start_frame = 0;

	end_frame = nFrame;

	// 텍스처 택하기.
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLTAIL ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	//	int nTex = 0;
	float light_scale = 4.5f;
	float light_dir = 0.5f;
	float light_fade = 0.5f;
	float distance = 0.0f;
	//wan:2004-12
	if(effect_sort.pMobFrom&&effect_sort.pMobFrom->m_nMobNum==361)	//뇌격 수호탑
		light_scale = 7.0f;

	for (i= start_frame; i <= end_frame; i++, distance+=0.7f)
	{
		if( i == (end_frame - 1) )
		{
			vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
			vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
			vDrawPos.z = vPosFrom.z + vDir.z * (i+1);
		}
		else
		{
			vDrawPos.x = vPosFrom.x + vDir.x * (end_frame - light_dir);
			vDrawPos.y = vPosFrom.y + vDir.y * (end_frame - light_dir);
			vDrawPos.z = vPosFrom.z + vDir.z * (end_frame - light_dir);
		}

		if( end_frame <= 5 )
			light_dir *= 1.25f;
		else
			light_dir += 0.75f;

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_LIGHTBALL_HEIGHT;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_lverTail[0] = D3DLVERTEX( D3DVECTOR(-light_scale, -light_scale, 0), 0xffffffff, 0, 0.0f, 1.0f );
		m_lverTail[1] = D3DLVERTEX( D3DVECTOR(-light_scale, light_scale, 0), 0xffffffff, 0, 0.0f, 0.0f );
		m_lverTail[2] = D3DLVERTEX( D3DVECTOR( light_scale, -light_scale, 0), 0xffffffff, 0, 1.0f, 1.0f );
		m_lverTail[3] = D3DLVERTEX( D3DVECTOR( light_scale, light_scale, 0), 0xffffffff, 0, 1.0f, 0.0f );

		m_lverTail[0].color = D3DRGBA(light_fade, light_fade, light_fade, light_fade);
		m_lverTail[1].color = D3DRGBA(light_fade, light_fade, light_fade, light_fade);
		m_lverTail[2].color = D3DRGBA(light_fade, light_fade, light_fade, light_fade);
		m_lverTail[3].color = D3DRGBA(light_fade, light_fade, light_fade, light_fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverTail, 4, 0 );
		light_scale -= 0.375f;
		light_fade -= 0.04f;
	}


	// 마지막 프레임이 목적지에 도착했는가 확인? - 거리를 측정해서 한다.
	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) - DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		// 맞은 놈한테 너 맞았다고 알려준다.
		// 그럼으로써 맞은 놈이 가지고 있는 effect_num이 삭제된다.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		vDrawPos = vPosTo;
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y +FX_LIGHTBALL_HEIGHT;
		matWorld._43 = vDrawPos.z;

		// 데이타 정의.
		m_lverBoom[0] = D3DLVERTEX( D3DVECTOR(-m_fSize*2.0f, -m_fSize*2.0f, 0), 0xffffffff, 0, 0.0f, 1.0f );
		m_lverBoom[1] = D3DLVERTEX( D3DVECTOR(-m_fSize*2.0f, m_fSize*2.0f, 0), 0xffffffff, 0, 0.0f, 0.0f );
		m_lverBoom[2] = D3DLVERTEX( D3DVECTOR( m_fSize*2.0f, -m_fSize*2.0f, 0), 0xffffffff, 0, 1.0f, 1.0f );
		m_lverBoom[3] = D3DLVERTEX( D3DVECTOR( m_fSize*2.0f, m_fSize*2.0f, 0), 0xffffffff, 0, 1.0f, 0.0f );

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverBoom, 4, 0 );
		bFinished = TRUE;

		// 폭파 효과를 호출한다.
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_LIGHTBALLBOOMNEW;
		if (effect_sort.pMobTo)
		{
			ef_sort.pMobTo = effect_sort.pMobTo;
			effect_sort.pMobTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		else if (effect_sort.pNkChaTo)
		{
			ef_sort.pNkChaTo = effect_sort.pNkChaTo;
			effect_sort.pNkChaTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}

		if (g_pDSound)
		{
			float x, y, z;
			x = (vPosTo.x - g_pRoh->m_wx) / g_fDSDistance;
			y = (vPosTo.y - g_pRoh->m_wy) / g_fDSDistance;
			z = (vPosTo.z - g_pRoh->m_wz) / g_fDSDistance;
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM2], x, y, z);
		}
	}
	else
	{
		// 선두.
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_LIGHTBALL_HEIGHT;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.2f, -m_fSize*1.2f, 0), 0xffffffff, 0, 0.0f, 1.0f );
		m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.2f, m_fSize*1.2f, 0), 0xffffffff, 0, 0.0f, 0.0f );
		m_lverRect[2] = D3DLVERTEX( D3DVECTOR( m_fSize*1.2f, -m_fSize*1.2f, 0), 0xffffffff, 0, 1.0f, 1.0f );
		m_lverRect[3] = D3DLVERTEX( D3DVECTOR( m_fSize*1.2f, m_fSize*1.2f, 0), 0xffffffff, 0, 1.0f, 0.0f );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );
	}

	// 바닥의 빛.
	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLBASE ] );

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 55, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 70, FALSE);		// 부울값은 시간인가 아닌가
		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CFxFireBall Class
CFxFireBall::CFxFireBall()
{
	m_fSize = 3.0f;
	m_fSpeed = 6.0f;
}

CFxFireBall::~CFxFireBall()
{
	DeleteRes();
}

void CFxFireBall::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxFireBall::DeleteRes()
{}

BOOL CFxFireBall::Render(EffectSort &effect_sort)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	unsigned int offset;
	int i;
	BOOL bFinished = FALSE;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	int nFrame;
	nFrame = effect_sort.nCurFrame;

	if (nFrame == 0)
		offset = 0;
	else
		offset = nFrame % 11;

	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;

	// 화이어볼 마법은 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;

	// 타겟 좌표를 구한다.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= m_fSpeed;

	// 현재 프레임에서 8프레임 전까지 그린다.
	int start_frame, end_frame;

	start_frame = nFrame - 8;		// 총 8프레임.
	if (start_frame < 0)
		start_frame = 0;
	end_frame = nFrame;

	// 텍스처 택하기.
	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLTAIL ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	// 마지막 프레임 전까지 그림을 그린다.
	// 여기서 start_frame은 화이어볼의 꼬리쪽.
	int nTex = 0;
	float fire_scale = 4.5f;
	float fire_dir = 0.5f;
	float fire_fade = 0.5f;

	for (i= end_frame - 1; i >= start_frame; i--)
	{
		if( i == (end_frame - 1) )
		{
			vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
			vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
			vDrawPos.z = vPosFrom.z + vDir.z * (i+1);
		}
		else
		{
			vDrawPos.x = vPosFrom.x + vDir.x * (end_frame - fire_dir);
			vDrawPos.y = vPosFrom.y + vDir.y * (end_frame - fire_dir);
			vDrawPos.z = vPosFrom.z + vDir.z * (end_frame - fire_dir);
		}

		if( end_frame <= 5 )
			fire_dir *= 1.25f;
		else
			fire_dir *= 1.5f;

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_FIREBALL_HEIGHT;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		MakeLVertexRect(m_lverTail, fire_scale, fire_scale, TRUE);

		m_lverTail[0].tu = nTex * 0.125f;
		m_lverTail[1].tu = nTex * 0.125f;
		m_lverTail[2].tu = (nTex+1) * 0.125f;
		m_lverTail[3].tu = (nTex+1) * 0.125f;

		m_lverTail[0].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lverTail[1].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lverTail[2].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);
		m_lverTail[3].color = D3DRGBA(fire_fade, fire_fade, fire_fade, fire_fade);

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverTail, 4, 0 );
		nTex++;
		fire_scale -= 0.375f;
		fire_fade -= 0.04f;
	}

	// 마지막 프레임이 목적지에 도착했는가 확인? - 거리를 측정해서 한다.
	vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
	vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
	vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) - DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
	{
		// 맞은 놈한테 너 맞았다고 알려준다.
		// 그럼으로써 맞은 놈이 가지고 있는 effect_num이 삭제된다.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		vDrawPos = vPosTo;
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_FIREBALL_HEIGHT;
		matWorld._43 = vDrawPos.z;

		// 데이타 정의.
		MakeLVertexRect(m_lverHead, m_fSize*2.5f, m_fSize*2.5f, TRUE);

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
										m_lverHead, 4, 0 );
		bFinished = TRUE;

		// 폭파 효과를 호출한다.
		EffectSort ef_sort;
		ef_sort.nType = FX_FIREBALLBOOM;
		ef_sort.vPos = vPosFrom;
		ef_sort.vTargetPos = vPosTo;
		//ef_sort.vTargetPos = vDrawPos;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		if (g_pDSound)
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM1], (vPosTo.x- g_pRoh->m_wx)/g_fDSDistance, (vPosTo.y- g_pRoh->m_wy)/g_fDSDistance, (vPosTo.z-g_pRoh->m_wz)/g_fDSDistance);
	}
	else
	{
		// 선두.
		matWorld._41 = vDrawPos.x;
		matWorld._42 = vDrawPos.y + FX_FIREBALL_HEIGHT;
		matWorld._43 = vDrawPos.z;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_lverHead[0] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.5f, -m_fSize*1.5f, 0), 0xffffffff, 0, 0.0f, 1.0f );
		m_lverHead[1] = D3DLVERTEX( D3DVECTOR(-m_fSize*1.5f, m_fSize*1.5f, 0), 0xffffffff, 0, 0.0f, 0.0f );
		m_lverHead[2] = D3DLVERTEX( D3DVECTOR( m_fSize*1.5f, -m_fSize*1.5f, 0), 0xffffffff, 0, 1.0f, 1.0f );
		m_lverHead[3] = D3DLVERTEX( D3DVECTOR( m_fSize*1.5f, m_fSize*1.5f, 0), 0xffffffff, 0, 1.0f, 0.0f );

		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLHEAD ] );
		GET_D3DDEVICE()->SetTexture(0, lpSurface);
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverHead, 4, 0 );
	}

	// 바닥의 빛.
	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLBASE ] );

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 55, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vDrawPos, 70, FALSE);		// 부울값은 시간인가 아닌가
		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}


////////////////////////////////////////////////////////////////////////////////////////////
// CFxFireBallBoom
CFxFireBallBoom::CFxFireBallBoom()
{
	// nate 2004 - 8 ImageManager
	m_fSpeed = 8.0f;
	m_fBoomSize = 25.0f;

	m_Texcode[0].tu = 0.0f;
	m_Texcode[0].tv = 0.0f;
	m_Texcode[1].tu = 0.25f;
	m_Texcode[1].tv = 0.0f;
	m_Texcode[2].tu = 0.5f;
	m_Texcode[2].tv = 0.0f;
	m_Texcode[3].tu = 0.75f;
	m_Texcode[3].tv = 0.0f;

	m_Texcode[4].tu = 0.0f;
	m_Texcode[4].tv = 0.25f;
	m_Texcode[5].tu = 0.25f;
	m_Texcode[5].tv = 0.25f;
	m_Texcode[6].tu = 0.5f;
	m_Texcode[6].tv = 0.25f;
	m_Texcode[7].tu = 0.75f;
	m_Texcode[7].tv = 0.25f;

	m_Texcode[8].tu = 0.0f;
	m_Texcode[8].tv = 0.5f;
	m_Texcode[9].tu = 0.25f;
	m_Texcode[9].tv = 0.5f;
	m_Texcode[10].tu = 0.5f;
	m_Texcode[10].tv = 0.5f;
	m_Texcode[11].tu = 0.75f;
	m_Texcode[11].tv = 0.5f;

	m_nFrameIdx[0] = 0;
	m_nFrameIdx[1] = 0;
	m_nFrameIdx[2] = 1;
	m_nFrameIdx[3] = 2;
	m_nFrameIdx[4] = 3;

	m_nFrameIdx[5] = 4;
	m_nFrameIdx[6] = 5;
	m_nFrameIdx[7] = 5;
	m_nFrameIdx[8] = 4;
	m_nFrameIdx[9] = 4;

	m_nFrameIdx[10] = 5;
	m_nFrameIdx[11] = 6;
	m_nFrameIdx[12] = 7;
	m_nFrameIdx[13] = 7;

	m_nFrameIdx[14] = 8;

	m_nFrameIdx[15] = 8;
	m_nFrameIdx[16] = 9;
	m_nFrameIdx[17] = 9;
	m_nFrameIdx[18] = 11;
	m_nFrameIdx[19] = 11;

	m_fBaseSize[0] = 70.0f;
	m_fBaseSize[1] = 70.0f;
	m_fBaseSize[2] = 75.0f;
	m_fBaseSize[3] = 75.0f;
	m_fBaseSize[4] = 75.0f;

	m_fBaseSize[5] = 87.0f;
	m_fBaseSize[6] = 87.0f;
	m_fBaseSize[7] = 87.0f;
	m_fBaseSize[8] = 85.0f;
	m_fBaseSize[9] = 85.0f;

	m_fBaseSize[10] = 90.0f;
	m_fBaseSize[11] = 90.0f;
	m_fBaseSize[12] = 90.0f;
	m_fBaseSize[13] = 87.0f;
	m_fBaseSize[14] = 87.0f;

	m_fBaseSize[15] = 75.0f;
	m_fBaseSize[16] = 60.0f;
	m_fBaseSize[17] = 50.0f;
	m_fBaseSize[18] = 40.0f;
	m_fBaseSize[19] = 30.0f;
}

CFxFireBallBoom::~CFxFireBallBoom()
{
	DeleteRes();
}

void CFxFireBallBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxFireBallBoom::DeleteRes()
{}

BOOL CFxFireBallBoom::Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	BOOL bFinished = FALSE;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	///////////////////////////////////////////////////////////////////////
	// 잔상 그리기.

	// 방향벡터를 구한다.
	D3DVECTOR vDir;
	D3DVECTOR vDrawPos;
	BOOL bDrawTail = TRUE;
	int start_frame, end_frame;
	int nFrameDist;
	float fDist;

	// 출발 점과 타겟지점과의 거리가 7프레임 이상이 되는가 확인한다.
	// 왜 꼬리잔상 그림이 소스지점보다 더 그려지지 않을려고..
	fDist = DistVecToVec(vPosFrom, vPosTo);

	// 몇 프레임 거리인가 확인을 한다.
	assert(m_fSpeed != 0.0f);
	nFrameDist = (int) (fDist / m_fSpeed);		// src와 dest와의 프레임간 거리.

	start_frame = 0;
	end_frame = 7 - nFrame;	// 7은 총잔상의 수에서 1을 뺀 것.
	if (end_frame < 0)
		bDrawTail = FALSE;

	if (nFrameDist < end_frame)
		end_frame = nFrameDist;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= m_fSpeed;
	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.
	matWorld._41 = vPosTo.x;
	matWorld._42 = vPosTo.y + 30;
	matWorld._43 = vPosTo.z;

	MakeLVertexRect(m_lverBoom, m_fBoomSize, m_fBoomSize, TRUE);

	if (nFrame < FX_FIREBALLBOOM_FRAMECOUNT)
	{
		m_lverBoom[0].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
		m_lverBoom[0].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
		m_lverBoom[1].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
		m_lverBoom[1].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;
		m_lverBoom[2].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
		m_lverBoom[2].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
		m_lverBoom[3].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
		m_lverBoom[3].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;
	}

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLBOOM ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverBoom, 4, 0 );

	if (nFrame >= FX_FIREBALLBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLBOOMBASE ] );

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame]*0.6, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame], FALSE);		// 부울값은 시간인가 아닌가

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}


////////////////////////////////////////////////////////////////////////////////////////////
// CFxFireBallBoom
CFxFireBallBoom2::CFxFireBallBoom2()
{
	m_fSpeed = 8.0f;
	m_fBoomSize = 25.0f;

	m_Texcode[0].tu = 0.0f;
	m_Texcode[0].tv = 0.0f;
	m_Texcode[1].tu = 0.25f;
	m_Texcode[1].tv = 0.0f;
	m_Texcode[2].tu = 0.5f;
	m_Texcode[2].tv = 0.0f;
	m_Texcode[3].tu = 0.75f;
	m_Texcode[3].tv = 0.0f;

	m_Texcode[4].tu = 0.0f;
	m_Texcode[4].tv = 0.25f;
	m_Texcode[5].tu = 0.25f;
	m_Texcode[5].tv = 0.25f;
	m_Texcode[6].tu = 0.5f;
	m_Texcode[6].tv = 0.25f;
	m_Texcode[7].tu = 0.75f;
	m_Texcode[7].tv = 0.25f;

	m_Texcode[8].tu = 0.0f;
	m_Texcode[8].tv = 0.5f;
	m_Texcode[9].tu = 0.25f;
	m_Texcode[9].tv = 0.5f;
	m_Texcode[10].tu = 0.5f;
	m_Texcode[10].tv = 0.5f;
	m_Texcode[11].tu = 0.75f;
	m_Texcode[11].tv = 0.5f;

	m_nFrameIdx[0] = 0;
	m_nFrameIdx[1] = 0;
	m_nFrameIdx[2] = 1;
	m_nFrameIdx[3] = 2;
	m_nFrameIdx[4] = 3;

	m_nFrameIdx[5] = 4;
	m_nFrameIdx[6] = 5;
	m_nFrameIdx[7] = 5;
	m_nFrameIdx[8] = 4;
	m_nFrameIdx[9] = 4;

	m_nFrameIdx[10] = 5;
	m_nFrameIdx[11] = 6;
	m_nFrameIdx[12] = 7;
	m_nFrameIdx[13] = 7;

	m_nFrameIdx[14] = 8;

	m_nFrameIdx[15] = 8;
	m_nFrameIdx[16] = 9;
	m_nFrameIdx[17] = 9;
	m_nFrameIdx[18] = 11;
	m_nFrameIdx[19] = 11;

	m_fBaseSize[0] = 70.0f;
	m_fBaseSize[1] = 70.0f;
	m_fBaseSize[2] = 75.0f;
	m_fBaseSize[3] = 75.0f;
	m_fBaseSize[4] = 75.0f;

	m_fBaseSize[5] = 87.0f;
	m_fBaseSize[6] = 87.0f;
	m_fBaseSize[7] = 87.0f;
	m_fBaseSize[8] = 85.0f;
	m_fBaseSize[9] = 85.0f;

	m_fBaseSize[10] = 90.0f;
	m_fBaseSize[11] = 90.0f;
	m_fBaseSize[12] = 90.0f;
	m_fBaseSize[13] = 87.0f;
	m_fBaseSize[14] = 87.0f;

	m_fBaseSize[15] = 75.0f;
	m_fBaseSize[16] = 60.0f;
	m_fBaseSize[17] = 50.0f;
	m_fBaseSize[18] = 40.0f;
	m_fBaseSize[19] = 30.0f;
}

CFxFireBallBoom2::~CFxFireBallBoom2()
{
	DeleteRes();
}

void CFxFireBallBoom2::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxFireBallBoom2::DeleteRes()
{}

BOOL CFxFireBallBoom2::Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	BOOL bFinished = FALSE;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	///////////////////////////////////////////////////////////////////////
	// 잔상 그리기.
	// 방향벡터를 구한다.
	D3DVECTOR vDir;
	D3DVECTOR vDrawPos;
	BOOL bDrawTail = TRUE;
	int start_frame, end_frame;
	int nFrameDist;
	float fDist;

	// 출발 점과 타겟지점과의 거리가 7프레임 이상이 되는가 확인한다.
	// 왜 꼬리잔상 그림이 소스지점보다 더 그려지지 않을려고..
	fDist = DistVecToVec(vPosFrom, vPosTo);

	// 몇 프레임 거리인가 확인을 한다.
	assert(m_fSpeed != 0.0f);
	nFrameDist = (int) (fDist / m_fSpeed);		// src와 dest와의 프레임간 거리.

	start_frame = 0;
	end_frame = 7 - nFrame;	// 7은 총잔상의 수에서 1을 뺀 것.

	if (end_frame < 0)
		bDrawTail = FALSE;

	if (nFrameDist < end_frame)
		end_frame = nFrameDist;

	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	vDir *= m_fSpeed;

	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.
	matWorld._41 = vPosTo.x;
	matWorld._42 = vPosTo.y + FX_FIREBALL_HEIGHT;
	matWorld._43 = vPosTo.z;

	MakeLVertexRect(m_lverBoom, m_fBoomSize, m_fBoomSize, TRUE);

	if (nFrame < FX_FIREBALLBOOM_FRAMECOUNT)
	{
		m_lverBoom[0].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
		m_lverBoom[0].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
		m_lverBoom[1].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
		m_lverBoom[1].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;
		m_lverBoom[2].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
		m_lverBoom[2].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
		m_lverBoom[3].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
		m_lverBoom[3].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;
	}

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLBOOM2 ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverBoom, 4, 0 );

	if (nFrame >= FX_FIREBALLBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREBALLBOOM2BASE ] );

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame]*0.6, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame], FALSE);		// 부울값은 시간인가 아닌가

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

/////////////////////////////////////////////////////////////////////////////////////
// CFxLwLight Class
// nate 2004 - 8 ImageManager
#define FX_LWLIGHT_TEXSIZE 7.0f

CFxLwLight::CFxLwLight()
{
	m_nTexFrame = 4;
}

CFxLwLight::~CFxLwLight()
{
	DeleteRes();
}

void CFxLwLight::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxLwLight::DeleteRes()
{}

BOOL CFxLwLight::Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame)
{
	static int nFrameCount = 0;
	BOOL bFinished = FALSE;

	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matRz, matWorld;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	// 여기서 부터
	D3DMATRIX ViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( ViewMatrix );
	// get the ViewMatrix from D3D
	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &ViewMatrix );

	D3DMATRIX InvViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( InvViewMatrix );
	// get the Inverse of the ViewMatrix, kinda like your reflection looking at you in the mirror
	D3DMath_MatrixInvert( InvViewMatrix, ViewMatrix );

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	InvViewMatrix._41 = vPosFrom.x;
	InvViewMatrix._42 = vPosFrom.y + FX_LWLIGHT_HEIGHT;
	InvViewMatrix._43 = vPosFrom.z;

	// Set the new world transform and render the particle
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &InvViewMatrix );

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LWLIGHT ] );
	//	lpSurface = m_pIndexedTexture->GetSurfaceByIdx(m_nTexIdx);
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	int nTex;
	nTex = nFrame % m_nTexFrame;

	MakeLVertexRect(m_lverRect, FX_LWLIGHT_TEXSIZE, FX_LWLIGHT_TEXSIZE, TRUE);

	m_lverRect[0].tu = 0.0f;
	m_lverRect[0].tv = 0.25f * (nTex+1) ;

	m_lverRect[1].tu = 0.0f;
	m_lverRect[1].tv = 0.25f * nTex;

	m_lverRect[2].tu = 1.0f ;
	m_lverRect[2].tv = 0.25f * (nTex+1);

	m_lverRect[3].tu = 1.0f;
	m_lverRect[3].tv = 0.25f * nTex;

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////
// CFxLightBallBoom Class
CFxLightBallBoom::CFxLightBallBoom()
{
	m_fBaseSize[0] = 70.0f;
	m_fBaseSize[1] = 75.0f;
	m_fBaseSize[2] = 80.0f;
	m_fBaseSize[3] = 87.0f;
	m_fBaseSize[4] = 90.0f;

	m_fBaseSize[5] = 80.0f;
	m_fBaseSize[6] = 70.0f;
	m_fBaseSize[7] = 60.0f;
	m_fBaseSize[8] = 45.0f;
	m_fBaseSize[9] = 30.0f;

	m_fBoomSize[0] = 12.0f;
	m_fBoomSize[1] = 13.0f;
	m_fBoomSize[2] = 14.0f;
	m_fBoomSize[3] = 15.0f;
	m_fBoomSize[4] = 16.0f;

	m_fBoomSize[5] = 15.0f;
	m_fBoomSize[6] = 13.0f;
	m_fBoomSize[7] = 10.0f;
	m_fBoomSize[8] = 8.0f;
	m_fBoomSize[9] = 5.0f;
}

CFxLightBallBoom::~CFxLightBallBoom()
{
	DeleteRes();
}

void CFxLightBallBoom::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxLightBallBoom::DeleteRes()
{}

BOOL CFxLightBallBoom::Render(D3DVECTOR &vPosFrom, D3DVECTOR &vPosTo, int nFrame)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;
	BOOL bFinished = FALSE;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.

	if (nFrame >= FX_LIGHTBALLBOOM_FRAMECOUNT - 1)
		bFinished = TRUE;

	matWorld._41 = vPosTo.x;
	matWorld._42 = vPosTo.y + FX_LIGHTBALL_HEIGHT;
	matWorld._43 = vPosTo.z;

	MakeLVertexRect(m_lverBoom, m_fBoomSize[nFrame], m_fBoomSize[nFrame], TRUE);

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLBOOM ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverBoom, 4, 0 );

	// nate 2004 - 7 글로벌 ShadowRect사용
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIGHTBALLBOOMBASE ] );

		if (g_bDungeon)
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame]*0.6f, FALSE);		// 부울값은 시간인가 아닌가
		else
			// nate 2004 - 7 글로벌 ShadowRect사용
			g_pShadowRect->SetTileCoord(vPosTo, m_fBaseSize[nFrame], FALSE);		// 부울값은 시간인가 아닌가

		// nate 2004 - 7 글로벌 ShadowRect사용
		g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Class CFxUnarmedAttacked
CFxUnarmedAttacked::CFxUnarmedAttacked()
{
	m_fTexSize = 5;
	MakeLVertexRect(m_lverRect, m_fTexSize, m_fTexSize, TRUE);
}

CFxUnarmedAttacked::~CFxUnarmedAttacked()
{
	DeleteRes();
}

void CFxUnarmedAttacked::LoadRes(IndexedTexture *pIndexedTexture)
{}

void CFxUnarmedAttacked::DeleteRes()
{}

BOOL CFxUnarmedAttacked::Render(D3DVECTOR &vPosTo, int nFrame)
{
	static int nFrameCount = 0;
	D3DVECTOR vViewVec;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;

	// 여기서 부터
	D3DMATRIX ViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( ViewMatrix );
	// get the ViewMatrix from D3D
	GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &ViewMatrix );

	D3DMATRIX InvViewMatrix;
	// this guarantees a clean Matrix before doing operations with it
	D3DUtil_SetIdentityMatrix( InvViewMatrix );
	// get the Inverse of the ViewMatrix, kinda like your reflection looking at you in the mirror
	D3DMath_MatrixInvert( InvViewMatrix, ViewMatrix );

	// set the translation portion of the InvViewMatrix to the location of the particles in world space
	InvViewMatrix._41 = vPosTo.x;
	InvViewMatrix._42 = vPosTo.y;
	InvViewMatrix._43 = vPosTo.z;

	// Set the new world transform and render the particle
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &InvViewMatrix );

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_UNARMEDATTACKKE ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	DWORD dwZenable;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZENABLE, &dwZenable );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverRect, 4, 0 );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, dwZenable);

	return TRUE;
}
