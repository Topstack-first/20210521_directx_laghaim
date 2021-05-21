#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>
#include <time.h>
#include "FxSet.h"
#include "ddutil.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "GUtil.h"
#include "SFXBaseShape.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Skill.h"
#include "UIMgr.h"
#include "land.h"
#include "Tcpipcon.h"
#include "TcpUtil.h"
#include "ShadowRect.h"
#include "NKObjQuadNode.h"
#include "FxFollow.h"


extern CLolos*				g_pFairyProtect; // 방어막 이펙트 롤로 ///페어리

extern BOOL					g_bNewTerrain;

char strLuckyPigCsyName[KIND_LUCKY_PIG_CSY][50] =
{
	"gold_pig.CSY",
	"pink_pig.CSY",
	"black_pig.CSY",
	"basic_pig.CSY"
};

char strLuckyPigBpdName[KIND_LUCKY_PIG_BPD][50] =
{
	"pig_stand.BPD",   // 서있기
	"pig_walkilng.BPD",    // 걷기(따라가기 포함)
	"pig_dance.BPD",     // 소셜1
	"pig_turn.BPD",     // 소셜2
	"pig_hiling.BPD",     // 스킬모션
};

// [7/31/2007 Theodoric] 6주년 기념 이벤트 베이비 소울
//char strBabySoulCsyName[KIND_BABYSOUL_CSY][50] =
//{
//	"babysoul_po.CSY",	// 포치
//	"babysoul_he.CSY",	// 히치
//	"babysoul_eh.CSY",	// 에치
//};

char strBabySoulBpdName[KIND_BABYSOUL_BPD][50] =
{
	"babysoul_stand.BPD",		// 서있기
	"babysoul_stand_up.BPD",	// 서있기 랜덤 : 날개짓 위로
	"babysoul_walk.BPD",		// 따라가기 포함
	"babysoul_critical.BPD",	// 크리티컬
};

char strRudolphCsyName[KIND_RUDOLPH_CSY][50] =
{
	"Tonakai.CSY",
	"Tonakai_K.CSY",
	"Tonakai_B.CSY",
	"Tonakai_G.CSY",
	"Tonakai_Y.CSY",
	"fawn.CSY" // [2008/11/3 Theodoric] 국내 프리미엄 상품
};

char strRudolphBpdName[KIND_RUDOLPH_BPD][50] =
{
	"SSA_stand.BPD",	// 서있기
	"SSA_walk.BPD",		// 서있기 랜덤 : 날개짓 위로
	"SSA_die.BPD",		// 소셜1
	"SSA_hit.BPD",		// 소셜2
};

char strArupakaCsyName[ARUPAKA_CSY_MAX][50] =
{
	"arupaka_pet.CSY",
	"arupaka_pet_be.CSY",
	"arupaka_pet_br.CSY"
};

char strArupakaBpdName[FX_ARUPAKA_ANI_MAX][50]=
{
	"arupaka_pet-walk.BPD",
	"arupaka_pet-down.BPD",
	"arupaka_pet-spit.BPD"
};

char strCatsCsyName[CATS_CSY_MAX][50] =
{
	"baby_cat_K.CSY",
	"baby_cat_W.CSY",
	"baby_cat_S.CSY",
};
char strCatsBpdName[FX_CATS_ANI_MAX][50] =
{
	"baby_cat_stand01.BPD",
	"baby_cat_stand02.BPD",
	"baby_cat_move.BPD",
};

char strZoroCsyName[ZORO_CSY_MAX][50] =
{
	"zorro_pet_B.CSY",
	"zorro_pet_W.CSY",
	"zorro_pet_G.CSY",
};
char strZoroBpdName[FX_ZORO_ANI_MAX][50] =
{
	"zorro_pet_idle01.BPD",
	"zorro_pet_idle02.BPD",
	"zorro_pet_move.BPD",
};

char strValoCsyName[VALO_CSY_MAX][50] =
{
	"valo_pet_B.CSY",
	"valo_pet_W.CSY",
	"valo_pet_G.CSY",
};
char strValoBpdName[FX_VALO_ANI_MAX][50] =
{
	"valo_pet_idle01.BPD",
	"valo_pet_idle02.BPD",
	"valo_pet_move.BPD",
};

char strBearCsyName[BEAR_CSY_MAX][50] =
{
	"babybear_B.CSY",
	"babybear_W.CSY",
};
char strBearBpdName[FX_BEAR_ANI_MAX][50] =
{
	"babybear_IDLE.BPD",
	"babybear_MOVE.BPD",
	"babybear_STAND.BPD",
};

char strHorseCsyName[HORSE_CSY_MAX][50] =
{
	"pet_bluehorse.csy",
	"pet_redhorse.csy"
};
char strHorseBpdName[FX_HORSE_ANI_MAX][50] =
{
	"horse_pet_stand01.BPD",
	"horse_pet_stand02.BPD",
	"horse_pet_move.BPD"	
};

char strJeguarCsyName[JEGUAR_CSY_MAX][50] =
{
	"pet_leo01.csy",
	"pet_leo02.csy",
	"pet_leo03.csy"
};
char strJeguarBpdName[FX_JEGUAR_ANI_MAX][50] =
{
	"PET_leo01_idle01.BPD",
	"PET_leo01_idle02.BPD",
	"PET_leo01_walk.BPD"	
};

char strBoniCsyName[BONI_CSY_MAX][50] =
{
	"pet_boni.csy",
	"pet_negro.csy",
	"pet_leopard.csy"
};

char strBoniBpdName[FX_BONI_ANI_MAX][50] =
{
	"PET_boni_idle.BPD",
	"PET_boni_idle.BPD",
	"PET_boni_walk.BPD"	
};

char strAZULCsyName[AZUL_CSY_MAX][50] =  //pablo test pet
{
	"Panda_Pet.CSY"
};

char strAZULBpdName[FX_AZUL_ANI_MAX][50] =
{
	"LHO_Pet_Idle.BPD",
	"LHO_Pet_S_Idle.BPD",
	"LHO_Pet_Move.BPD"
};

char strVERDECsyName[VERDE_CSY_MAX][50] =  //pablo test pet
{
	"Wolf_Pet.CSY"
};

char strVERDEBpdName[FX_VERDE_ANI_MAX][50] =
{
	"LHO_Pet_Idle.BPD",
	"LHO_Pet_S_Idle.BPD",
	"LHO_Pet_Move.BPD"
};

char strROXOCsyName[ROXO_CSY_MAX][50] =  //pablo test pet
{
	"Hamster_Pet.CSY"
};

char strROXOBpdName[FX_ROXO_ANI_MAX][50] =
{
	"LHO_Pet_Idle.BPD",
	"LHO_Pet_S_Idle.BPD",
	"LHO_Pet_Move.BPD"
};

char strAMARELOCsyName[AMARELO_CSY_MAX][50] =  //pablo test pet
{
	"Miss_A.CSY"
};

char strAMARELOBpdName[FX_AMARELO_ANI_MAX][50] =
{
	"MissA_Idle.BPD",
	"MissA_S_Idle.BPD",
	"MissA_Move.BPD"
};

char strVERMELHOCsyName[VERMELHO_CSY_MAX][50] =  //pablo test pet
{
	"Rabbit_Pet.CSY"
};

char strVERMELHOBpdName[FX_VERMELHO_ANI_MAX][50] =
{
	"Rabbit_Idle.BPD",
	"Rabbit_S_Idle.BPD",
	"Rabbit_Move.BPD"
};

char strCINZACsyName[CINZA_CSY_MAX][50] =  //pablo test pet
{
	"Altair.CSY",
	"Premium_Altair.CSY"
};

char strCINZABpdName[FX_CINZA_ANI_MAX][50] =
{
	"Altair_Idle01.BPD",
	"Altair_Idle02.BPD",
	"Altair_Move.BPD"
};

char strFESHIKCsyName[FESHIK_CSY_MAX][50] =  //pablo test pet
{
	"Feshik.CSY",
	"Premium_Feshik.CSY"
};

char strFESHIKBpdName[FX_FESHIK_ANI_MAX][50] =
{
	"Feshik_Idle01.BPD",
	"Feshik_Idle02.BPD",
	"Feshik_Move.BPD"
};

char strEZULANCsyName[EZULAN_CSY_MAX][50] =  //pablo test pet
{
	"Ezulan.CSY",
	"Ezulan_P.CSY"
};

char strEZULANBpdName[FX_EZULAN_ANI_MAX][50] =
{
	"Ezulan_Idle01.BPD",
	"Ezulan_Idle02.BPD",
	"Ezulan_Move.BPD"
};

char strTHUNDERBIRDCsyName[THUNDERBIRD_CSY_MAX][50] =  //pablo test pet
{
	"Thunderbird.CSY"
};

char strTHUNDERBIRDBpdName[FX_THUNDERBIRD_ANI_MAX][50] =
{
	"Thunderbird_Idle01.BPD",
	"Thunderbird_Idle02.BPD",
	"Thunderbird_Move.BPD"
};

char strTHUNDERBIRD_PCsyName[THUNDERBIRD_P_CSY_MAX][50] =  //pablo test pet
{
	"Premium_Thunderbird.CSY"
};

char strTHUNDERBIRD_PBpdName[FX_THUNDERBIRD_P_ANI_MAX][50] =
{
	"Premium_Thunderbird_Idle01.BPD",
	"Premium_Thunderbird_Idle02.BPD",
	"Premium_Thunderbird_Move.BPD"
};

char strPETFAME1CsyName[PETFAME1_CSY_MAX][50] =  //pablo test pet
{
	"PETFAME1.CSY"
};

char strPETFAME1BpdName[FX_PETFAME1_ANI_MAX][50] =
{
	"PETFAME1_Idle01.BPD",
	"PETFAME1_Idle02.BPD",
	"PETFAME1_Move.BPD"
};

char strPETFAME2CsyName[PETFAME2_CSY_MAX][50] =  //pablo test pet
{
	"PETFAME2.CSY"
};

char strPETFAME2BpdName[FX_PETFAME2_ANI_MAX][50] =
{
	"PETFAME2_Idle01.BPD",
	"PETFAME2_Idle02.BPD",
	"PETFAME2_Move.BPD"
};

char strPETFAME3CsyName[PETFAME3_CSY_MAX][50] =  //pablo test pet
{
	"PETFAME3.CSY"
};

char strPETFAME3BpdName[FX_PETFAME3_ANI_MAX][50] =
{
	"PETFAME3_Idle01.BPD",
	"PETFAME3_Idle02.BPD",
	"PETFAME3_Move.BPD"
};

char strPETFAME4CsyName[PETFAME4_CSY_MAX][50] =  //pablo test pet
{
	"PETFAME4.CSY"
};

char strPETFAME4BpdName[FX_PETFAME4_ANI_MAX][50] =
{
	"PETFAME4_Idle01.BPD",
	"PETFAME4_Idle02.BPD",
	"PETFAME4_Move.BPD"
};

char strPETHELPERCsyName[PETHELPER_CSY_MAX][50] =  //pablo test pet
{
	"PETHELPERXP.CSY"
};

char strPETHELPERBpdName[FX_PETHELPER_ANI_MAX][50] =
{
	"PETHELPER_Idle01.BPD",
	"PETHELPER_Idle02.BPD",
	"PETHELPER_Move.BPD"
};

char strPETHALL1CsyName[PETHALL1_CSY_MAX][50] =  //pablo test pet
{
	"PETHALL1.CSY"
};

char strPETHALL1BpdName[FX_PETHALL1_ANI_MAX][50] =
{
	"PETHALL1_Idle01.BPD",
	"PETHALL1_Idle02.BPD",
	"PETHALL1_Move.BPD"
};

char strPETHALL2CsyName[PETHALL2_CSY_MAX][50] =  //pablo test pet
{
	"PETHALL2.CSY"
};

char strPETHALL2BpdName[FX_PETHALL2_ANI_MAX][50] =
{
	"PETHALL2_Idle01.BPD",
	"PETHALL2_Idle02.BPD",
	"PETHALL2_Move.BPD"
};

char strPETHALL3CsyName[PETHALL3_CSY_MAX][50] =  //pablo test pet
{
	"PETHALL3.CSY"
};

char strPETHALL3BpdName[FX_PETHALL3_ANI_MAX][50] =
{
	"PETHALL3_Idle01.BPD",
	"PETHALL3_Idle02.BPD",
	"PETHALL3_Move.BPD"
};

char strPETNATAL1CsyName[PETNATAL1_CSY_MAX][50] =  //pablo test pet
{
	"PETNATAL1.CSY"
};

char strPETNATAL1BpdName[FX_PETNATAL1_ANI_MAX][50] =
{
	"PETNATAL1_Idle01.BPD",
	"PETNATAL1_Idle02.BPD",
	"PETNATAL1_Move.BPD"
};

char strPETNATAL2CsyName[PETNATAL2_CSY_MAX][50] =  //pablo test pet
{
	"PETNATAL2.CSY"
};

char strPETNATAL2BpdName[FX_PETNATAL2_ANI_MAX][50] =
{
	"PETNATAL2_Idle01.BPD",
	"PETNATAL2_Idle02.BPD",
	"PETNATAL2_Move.BPD"
};

char strPETNATAL3CsyName[PETNATAL3_CSY_MAX][50] =  //pablo test pet
{
	"PETNATAL3.CSY"
};

char strPETNATAL3BpdName[FX_PETNATAL3_ANI_MAX][50] =
{
	"PETNATAL3_Idle01.BPD",
	"PETNATAL3_Idle02.BPD",
	"PETNATAL3_Move.BPD"
};



CFxFollow::CFxFollow()
	: m_ring_Light_Color(0)
	, m_ring_Light_TailColor(0)
{
	m_pMainObject	= NULL;
	m_pEffect1		= NULL;
	m_pEffect2		= NULL;
	m_pLolo1		= NULL;
	m_pParticleInfoArray = NULL;

	int i = 0;

	for( i = 0 ; i < KIND_LUCKY_PIG_CSY ; i++ )
	{
		m_pCsyLuckyPig[i] = NULL;
	}

	for( i = 0 ; i < KIND_LUCKY_PIG_BPD ; i++ )
	{
		m_pBpdLuckyPig[i] = NULL;
	}

	m_pCsyBabySoul	= NULL;

	for( i = 0 ; i < KIND_BABYSOUL_BPD ; i++ )
	{
		m_pBpdBabySoul[i] = NULL;
	}


	for( i=0; i<KIND_BABYSOUL_CSY-1; i++)
	{
		m_BabySoulTextrIndex[i] = -1;
	}	

	for( i=0; i<KIND_RUDOLPH_CSY; i++ )
	{
		m_pCsyRudolph[i] = NULL;
	}

	for( i=0; i < KIND_RUDOLPH_BPD; i++ )
	{
		m_pBpdRudolph[i] = NULL;
	}


	for( i=0; i<ARUPAKA_CSY_MAX; i++ )
		m_pCsyArupaka[i] = NULL;

	for( i=0; i < FX_ARUPAKA_ANI_MAX; i++ )
		m_pBpdArupaka[i] = NULL;

	m_bSpit = false;

	m_pRectPlane = NULL;
	m_pRectPlane = new CRectPlane;	
	m_pRectPlane->Create(5.0f, 5.0f, FALSE);


	for( i=0; i<CATS_CSY_MAX; i++ )
	{
		m_pCsyCats[i] = NULL;
	}

	for( i=0; i < FX_CATS_ANI_MAX; i++ )
	{
		m_pBpdCats[i] = NULL;
	}

	for( i=0; i<ZORO_CSY_MAX; i++ )
		m_pCsyZoro[i] = NULL;
	for( i=0; i < FX_ZORO_ANI_MAX; i++ )
		m_pBpdZoro[i] = NULL;
	for( i=0; i<VALO_CSY_MAX; i++ )
		m_pCsyValo[i] = NULL;
	for( i=0; i < FX_VALO_ANI_MAX; i++ )
		m_pBpdValo[i] = NULL;
	for( i=0; i<BEAR_CSY_MAX; i++ )
		m_pCsyBear[i] = NULL;
	for( i=0; i < FX_BEAR_ANI_MAX; i++ )
		m_pBpdBear[i] = NULL;

	for( i=0; i<HORSE_CSY_MAX; i++ )
	{
		m_pCsyHorse[i] = NULL;
	}
	for( i=0; i < FX_HORSE_ANI_MAX; i++ )
	{
		m_pBpdHorse[i] = NULL;
	}

	for( i=0; i < JEGUAR_CSY_MAX; i++ )
	{
		m_pCsyJeguar[i] = NULL;
	}
	for( i=0; i < FX_JEGUAR_ANI_MAX; i++ )
	{
		m_pBpdJeguar[i] = NULL;
	}

	for( i=0; i < BONI_CSY_MAX; i++ )
	{
		m_pCsyBoni[i] = NULL;
	}
	for( i=0; i < FX_JEGUAR_ANI_MAX; i++ )
	{
		m_pBpdBoni[i] = NULL;
	}
	for( i=0; i < AZUL_CSY_MAX; i++ )	//PABLO
	{
		m_pCsyAZUL[i] = NULL;
	}
	for( i=0; i < FX_AZUL_ANI_MAX; i++ )
	{
		m_pBpdAZUL[i] = NULL;
	}
	for( i=0; i < VERDE_CSY_MAX; i++ )	//PABLO
	{
		m_pCsyVERDE[i] = NULL;
	}
	for( i=0; i < FX_VERDE_ANI_MAX; i++ )
	{
		m_pBpdVERDE[i] = NULL;
	}
		for( i=0; i < ROXO_CSY_MAX; i++ )	//PABLO
	{
		m_pCsyROXO[i] = NULL;
	}
	for( i=0; i < FX_ROXO_ANI_MAX; i++ )
	{
		m_pBpdROXO[i] = NULL;
	}
		for( i=0; i < AMARELO_CSY_MAX; i++ )	//PABLO
	{
		m_pCsyAMARELO[i] = NULL;
	}
	for( i=0; i < FX_AMARELO_ANI_MAX; i++ )
	{
		m_pBpdAMARELO[i] = NULL;
	}
		for( i=0; i < VERMELHO_CSY_MAX; i++ )	//PABLO
	{
		m_pCsyVERMELHO[i] = NULL;
	}
	for( i=0; i < FX_VERMELHO_ANI_MAX; i++ )
	{
		m_pBpdVERMELHO[i] = NULL;
	}
		for( i=0; i < CINZA_CSY_MAX; i++ )	//PABLO
	{
		m_pCsyCINZA[i] = NULL;
	}
	for( i=0; i < FX_CINZA_ANI_MAX; i++ )
	{
		m_pBpdCINZA[i] = NULL;
	}
		for( i=0; i < FESHIK_CSY_MAX; i++ )	
	{
		m_pCsyFESHIK[i] = NULL;
	}
	for( i=0; i < FX_FESHIK_ANI_MAX; i++ )
	{
		m_pBpdFESHIK[i] = NULL;
	}
	for( i=0; i < EZULAN_CSY_MAX; i++ )	
	{
		m_pCsyEZULAN[i] = NULL;
	}
	for( i=0; i < FX_EZULAN_ANI_MAX; i++ )
	{
		m_pBpdEZULAN[i] = NULL;
	}
	for( i=0; i < THUNDERBIRD_CSY_MAX; i++ )	
	{
		m_pCsyTHUNDERBIRD[i] = NULL;
	}
	for( i=0; i < FX_THUNDERBIRD_ANI_MAX; i++ )
	{
		m_pBpdTHUNDERBIRD[i] = NULL;
	}
	for( i=0; i < THUNDERBIRD_P_CSY_MAX; i++ )	
	{
		m_pCsyTHUNDERBIRD_P[i] = NULL;
	}
	for( i=0; i < FX_THUNDERBIRD_P_ANI_MAX; i++ )
	{
		m_pBpdTHUNDERBIRD_P[i] = NULL;
	}
	for( i=0; i < PETFAME1_CSY_MAX; i++ )	
	{
		m_pCsyPETFAME1[i] = NULL;
	}
	for( i=0; i < FX_PETFAME1_ANI_MAX; i++ )
	{
		m_pBpdPETFAME1[i] = NULL;
	}
	for( i=0; i < PETFAME2_CSY_MAX; i++ )	
	{
		m_pCsyPETFAME2[i] = NULL;
	}
	for( i=0; i < FX_PETFAME2_ANI_MAX; i++ )
	{
		m_pBpdPETFAME2[i] = NULL;
	}
	for( i=0; i < PETFAME3_CSY_MAX; i++ )	
	{
		m_pCsyPETFAME3[i] = NULL;
	}
	for( i=0; i < FX_PETFAME3_ANI_MAX; i++ )
	{
		m_pBpdPETFAME3[i] = NULL;
	}
	for( i=0; i < PETFAME4_CSY_MAX; i++ )	
	{
		m_pCsyPETFAME4[i] = NULL;
	}
	for( i=0; i < FX_PETFAME4_ANI_MAX; i++ )
	{
		m_pBpdPETFAME4[i] = NULL;
	}
	for( i=0; i < PETHELPER_CSY_MAX; i++ )	
	{
		m_pCsyPETHELPER[i] = NULL;
	}
	for( i=0; i < FX_PETHELPER_ANI_MAX; i++ )
	{
		m_pBpdPETHELPER[i] = NULL;
	}
	for( i=0; i < PETHALL1_CSY_MAX; i++ )	
	{
		m_pCsyPETHALL1[i] = NULL;
	}
	for( i=0; i < FX_PETHALL1_ANI_MAX; i++ )
	{
		m_pBpdPETHALL1[i] = NULL;
	}
	for( i=0; i < PETHALL2_CSY_MAX; i++ )	
	{
		m_pCsyPETHALL2[i] = NULL;
	}
	for( i=0; i < FX_PETHALL2_ANI_MAX; i++ )
	{
		m_pBpdPETHALL2[i] = NULL;
	}
	for( i=0; i < PETHALL3_CSY_MAX; i++ )	
	{
		m_pCsyPETHALL3[i] = NULL;
	}
	for( i=0; i < FX_PETHALL3_ANI_MAX; i++ )
	{
		m_pBpdPETHALL3[i] = NULL;
	}
	for( i=0; i < PETNATAL1_CSY_MAX; i++ )	
	{
		m_pCsyPETNATAL1[i] = NULL;
	}
	for( i=0; i < FX_PETNATAL1_ANI_MAX; i++ )
	{
		m_pBpdPETNATAL1[i] = NULL;
	}
	for( i=0; i < PETNATAL2_CSY_MAX; i++ )	
	{
		m_pCsyPETNATAL2[i] = NULL;
	}
	for( i=0; i < FX_PETNATAL2_ANI_MAX; i++ )
	{
		m_pBpdPETNATAL2[i] = NULL;
	}
	for( i=0; i < PETNATAL3_CSY_MAX; i++ )	
	{
		m_pCsyPETNATAL3[i] = NULL;
	}
	for( i=0; i < FX_PETNATAL3_ANI_MAX; i++ )
	{
		m_pBpdPETNATAL3[i] = NULL;
	}
}

CFxFollow::~CFxFollow()
{
	DeleteRes();
	SAFE_DELETE(m_pRectPlane);
}

void CFxFollow::DeleteRes()
{
	int i = 0;

	///펫루돌프
	for( i = 0 ; i < KIND_RUDOLPH_CSY ; i++ )
	{
		SAFE_DELETE( m_pCsyRudolph[i] );
	}
	for( i = 0 ; i < KIND_RUDOLPH_BPD ; i++ )
	{
		SAFE_DELETE( m_pBpdRudolph[i] );
	}

	for( i = 0 ; i < KIND_LUCKY_PIG_BPD ; i++ )
	{
		SAFE_DELETE( m_pBpdLuckyPig[i] );
	}

	for( i = 0 ; i < KIND_LUCKY_PIG_CSY ; i++ )
	{
		SAFE_DELETE( m_pCsyLuckyPig[i] );
	}


	// [7/31/2007 Theodoric] 6주년 기념 이벤트 베이비 소울
	for( i = 0 ; i < KIND_BABYSOUL_BPD ; i++ )
	{
		SAFE_DELETE( m_pBpdBabySoul[i] );
	}

	SAFE_DELETE( m_pCsyBabySoul );

	for( i=0; i<KIND_BABYSOUL_CSY-1; i++)
	{
		m_BabySoulTextrIndex[i] = -1;
	}

	SAFE_DELETE( m_pMainObject );
	SAFE_DELETE( m_pEffect1 );
	SAFE_DELETE( m_pEffect2 );
	SAFE_DELETE( m_pLolo1 );

	m_ring_Light_Color = 0;
	m_ring_Light_TailColor = 0;

	for( i=0; i<ARUPAKA_CSY_MAX; i++ )
	{
		SAFE_DELETE( m_pCsyArupaka[i] );
	}

	for( i=0; i < FX_ARUPAKA_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdArupaka[i] );
	}

	for( i = 0 ; i < CATS_CSY_MAX ; i++ )
	{
		SAFE_DELETE( m_pCsyCats[i] );
	}
	for( i = 0 ; i < FX_CATS_ANI_MAX ; i++ )
	{
		SAFE_DELETE( m_pBpdCats[i] );
	}

	for( i = 0 ; i < ZORO_CSY_MAX ; i++ )
		SAFE_DELETE( m_pCsyZoro[i] );
	for( i = 0 ; i < FX_ZORO_ANI_MAX ; i++ )
		SAFE_DELETE( m_pBpdZoro[i] );
	for( i=0 ; i<VALO_CSY_MAX ; i++ )
		SAFE_DELETE( m_pCsyValo[i] );
	for( i = 0 ; i < FX_VALO_ANI_MAX ; i++ )
		SAFE_DELETE( m_pBpdValo[i] );
	for( i = 0 ; i < BEAR_CSY_MAX ; i++ )
		SAFE_DELETE( m_pCsyBear[i] );
	for( i = 0 ; i < FX_BEAR_ANI_MAX ; i++ )
		SAFE_DELETE( m_pBpdBear[i] );
	
	for( i = 0; i < HORSE_CSY_MAX; i++ )
	{
		SAFE_DELETE( m_pCsyHorse[i] );
	}	
	for( i = 0; i < FX_HORSE_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdHorse[i] );
	}

	for( i=0; i < JEGUAR_CSY_MAX; i++ )
	{
		SAFE_DELETE( m_pCsyJeguar[i] );
	}
	for( i=0; i < FX_JEGUAR_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdJeguar[i] );
	}

	for( i=0; i < BONI_CSY_MAX; i++ )
	{
		SAFE_DELETE( m_pCsyBoni[i] );
	}
	for( i=0; i < FX_BONI_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdBoni[i] );
	}
	for( i=0; i < AZUL_CSY_MAX; i++ )		//PABLO
	{
		SAFE_DELETE( m_pCsyAZUL[i] );
	}
	for( i=0; i < FX_AZUL_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdAZUL[i] );
	}
	for( i=0; i < VERDE_CSY_MAX; i++ )		//PABLO
	{
		SAFE_DELETE( m_pCsyVERDE[i] );
	}
	for( i=0; i < FX_VERDE_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdVERDE[i] );
	}
	for( i=0; i < ROXO_CSY_MAX; i++ )		//PABLO
	{
		SAFE_DELETE( m_pCsyROXO[i] );
	}
	for( i=0; i < FX_ROXO_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdROXO[i] );
	}
	for( i=0; i < AMARELO_CSY_MAX; i++ )		//PABLO
	{
		SAFE_DELETE( m_pCsyAMARELO[i] );
	}
	for( i=0; i < FX_AMARELO_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdAMARELO[i] );
	}
	for( i=0; i < VERMELHO_CSY_MAX; i++ )		//PABLO
	{
		SAFE_DELETE( m_pCsyVERMELHO[i] );
	}
	for( i=0; i < FX_VERMELHO_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdVERMELHO[i] );
	}
	for( i=0; i < CINZA_CSY_MAX; i++ )		//PABLO
	{
		SAFE_DELETE( m_pCsyCINZA[i] );
	}
	for( i=0; i < FX_CINZA_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdCINZA[i] );
	}
	for( i=0; i < FESHIK_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyFESHIK[i] );
	}
	for( i=0; i < FX_FESHIK_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdFESHIK[i] );
	}
	for( i=0; i < EZULAN_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyEZULAN[i] );
	}
	for( i=0; i < FX_EZULAN_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdEZULAN[i] );
	}
	for( i=0; i < THUNDERBIRD_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyTHUNDERBIRD[i] );
	}
	for( i=0; i < FX_THUNDERBIRD_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdTHUNDERBIRD[i] );
	}
	for( i=0; i < THUNDERBIRD_P_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyTHUNDERBIRD_P[i] );
	}
	for( i=0; i < FX_THUNDERBIRD_P_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdTHUNDERBIRD_P[i] );
	}
	for( i=0; i < PETFAME1_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETFAME1[i] );
	}
	for( i=0; i < FX_PETFAME1_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETFAME1[i] );
	}
	for( i=0; i < PETFAME2_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETFAME2[i] );
	}
	for( i=0; i < FX_PETFAME2_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETFAME2[i] );
	}
	for( i=0; i < PETFAME3_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETFAME3[i] );
	}
	for( i=0; i < FX_PETFAME3_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETFAME3[i] );
	}
	for( i=0; i < PETFAME4_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETFAME4[i] );
	}
	for( i=0; i < FX_PETFAME4_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETFAME4[i] );
	}
	for( i=0; i < PETHELPER_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETHELPER[i] );
	}
	for( i=0; i < FX_PETHELPER_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETHELPER[i] );
	}
	for( i=0; i < PETHALL1_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETHALL1[i] );
	}
	for( i=0; i < FX_PETHALL1_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETHALL1[i] );
	}
	for( i=0; i < PETHALL2_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETHALL2[i] );
	}
	for( i=0; i < FX_PETHALL2_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETHALL2[i] );
	}
	for( i=0; i < PETHALL3_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETHALL3[i] );
	}
	for( i=0; i < FX_PETHALL3_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETHALL3[i] );
	}
	for( i=0; i < PETNATAL1_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETNATAL1[i] );
	}
	for( i=0; i < FX_PETNATAL1_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETNATAL1[i] );
	}
	for( i=0; i < PETNATAL2_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETNATAL2[i] );
	}
	for( i=0; i < FX_PETNATAL2_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETNATAL2[i] );
	}
	for( i=0; i < PETNATAL3_CSY_MAX; i++ )		
	{
		SAFE_DELETE( m_pCsyPETNATAL3[i] );
	}
	for( i=0; i < FX_PETNATAL3_ANI_MAX; i++ )
	{
		SAFE_DELETE( m_pBpdPETNATAL3[i] );
	}
}

void CFxFollow::LoadRes()
{
	DeleteRes();

	m_pMainObject = new CRectPlane;
	if( m_pMainObject )
		m_pMainObject->Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)

	m_pEffect1 = new CRectPlane;
	if( m_pEffect1 )
		m_pEffect1->Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)

	m_pEffect2 = new CRectPlane;
	if( m_pEffect2 )
		m_pEffect2->Create( 0.5f, 0.5f, FALSE );

	char strTemp[200];
	char strTemp2[200];
	int i = 0;

	sprintf( strTemp, "./data//Npc/csy/" );
	for( i = 0 ; i < KIND_LUCKY_PIG_CSY ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strLuckyPigCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/Npc/textr/");
		m_pCsyLuckyPig[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyLuckyPig[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyLuckyPig[i]);			
		}
	}

	sprintf( strTemp, "./data/Npc//bpd/" );
	for( i = 0 ; i < KIND_LUCKY_PIG_BPD ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strLuckyPigBpdName[i] );
		m_pBpdLuckyPig[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdLuckyPig[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdLuckyPig[i]);			
		}
	}

	sprintf( strTemp, "./data/Npc/csy/" );
	sprintf( strTemp2, "%s%s", strTemp, "babysoul.CSY" );
	g_pCapsyongTexture->SetTexturePath("data/Npc/textr/");
	m_pCsyBabySoul = new CCapsyong(g_pCapsyongTexture);
	if( VF_ERR_NONE > m_pCsyBabySoul->Read(strTemp2) )
	{
		SAFE_DELETE(m_pCsyBabySoul);		
	}
	m_BabySoulTextrIndex[0] = g_pCapsyongTexture->CreateTextureFromFile("babysoul_he.bmp");
	m_BabySoulTextrIndex[1] = g_pCapsyongTexture->CreateTextureFromFile("babysoul_eh.bmp");


	sprintf( strTemp, "./data/Npc/bpd/" );
	for( i = 0 ; i < KIND_BABYSOUL_BPD ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strBabySoulBpdName[i] );
		m_pBpdBabySoul[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdBabySoul[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdBabySoul[i]);			
		}
	}

	sprintf( strTemp, "./data/Npc/csy/" );

	for( i = 0; i < KIND_RUDOLPH_CSY; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strRudolphCsyName[i] );

		g_pCapsyongTexture->SetTexturePath("data/Npc/textr/");
		m_pCsyRudolph[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyRudolph[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyRudolph[i]);			
		}
	}

	sprintf( strTemp, "./data/Npc/bpd/" );
	for( i = 0 ; i < KIND_RUDOLPH_BPD ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strRudolphBpdName[i] );
		m_pBpdRudolph[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdRudolph[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdRudolph[i]);			
		}
	}

	sprintf( strTemp, ".//data//Npc//csy//" );

	for( i = 0; i < ARUPAKA_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strArupakaCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/Npc/textr/");
		m_pCsyArupaka[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyArupaka[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyArupaka[i]);			
		}
	}

	sprintf( strTemp, ".//data//Npc//bpd//" );
	for( i = 0 ; i < FX_ARUPAKA_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strArupakaBpdName[i] );
		m_pBpdArupaka[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdArupaka[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdArupaka[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );
	for( i = 0; i < CATS_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strCatsCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyCats[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyCats[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyCats[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_CATS_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strCatsBpdName[i] );
		m_pBpdCats[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdCats[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdCats[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );
	for( i = 0; i < ZORO_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strZoroCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyZoro[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyZoro[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyZoro[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_ZORO_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strZoroBpdName[i] );
		m_pBpdZoro[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdZoro[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdZoro[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );
	for( i = 0; i < VALO_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strValoCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyValo[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyValo[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyValo[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_VALO_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strValoBpdName[i] );
		m_pBpdValo[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdValo[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdValo[i]);			
		}
	}
	sprintf( strTemp, "./data/item/csy/" );
	for( i = 0; i < BEAR_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strBearCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyBear[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyBear[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyBear[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_BEAR_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strBearBpdName[i] );
		m_pBpdBear[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdBear[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdBear[i]);			
		}
	}


	sprintf( strTemp, "./data/item/csy/" );
	for( i = 0; i < HORSE_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strHorseCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyHorse[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyHorse[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyHorse[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_HORSE_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strHorseBpdName[i] );
		m_pBpdHorse[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdHorse[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdHorse[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );
	for( i = 0; i < JEGUAR_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strJeguarCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyJeguar[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyJeguar[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyJeguar[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_JEGUAR_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strJeguarBpdName[i] );
		m_pBpdJeguar[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdJeguar[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdJeguar[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );
	for( i = 0; i < BONI_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strBoniCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyBoni[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyBoni[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyBoni[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_BONI_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strBoniBpdName[i] );
		m_pBpdBoni[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdBoni[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdBoni[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );   //PABLO TEST PET
	for( i = 0; i < AZUL_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strAZULCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyAZUL[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyAZUL[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyAZUL[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_AZUL_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strAZULBpdName[i] );
		m_pBpdAZUL[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdAZUL[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdAZUL[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );   //PABLO TEST PET
	for( i = 0; i < VERDE_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strVERDECsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyVERDE[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyVERDE[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyVERDE[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_VERDE_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strVERDEBpdName[i] );
		m_pBpdVERDE[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdVERDE[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdVERDE[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );   //PABLO TEST PET
	for( i = 0; i < ROXO_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strROXOCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyROXO[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyROXO[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyROXO[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_ROXO_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strROXOBpdName[i] );
		m_pBpdROXO[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdROXO[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdROXO[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );   //PABLO TEST PET
	for( i = 0; i < AMARELO_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strAMARELOCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyAMARELO[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyAMARELO[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyAMARELO[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_AMARELO_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strAMARELOBpdName[i] );
		m_pBpdAMARELO[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdAMARELO[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdAMARELO[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );   //PABLO TEST PET
	for( i = 0; i < VERMELHO_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strVERMELHOCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyVERMELHO[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyVERMELHO[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyVERMELHO[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_VERMELHO_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strVERMELHOBpdName[i] );
		m_pBpdVERMELHO[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdVERMELHO[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdVERMELHO[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" );   //PABLO TEST PET
	for( i = 0; i < CINZA_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strCINZACsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyCINZA[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyCINZA[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyCINZA[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_CINZA_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strCINZABpdName[i] );
		m_pBpdCINZA[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdCINZA[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdCINZA[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < FESHIK_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strFESHIKCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyFESHIK[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyFESHIK[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyFESHIK[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_FESHIK_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strFESHIKBpdName[i] );
		m_pBpdFESHIK[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdFESHIK[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdFESHIK[i]);			
		}
	}
		sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < EZULAN_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strEZULANCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyEZULAN[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyEZULAN[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyEZULAN[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_EZULAN_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strEZULANBpdName[i] );
		m_pBpdEZULAN[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdEZULAN[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdEZULAN[i]);			
		}
	}
		sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < THUNDERBIRD_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strTHUNDERBIRDCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyTHUNDERBIRD[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyTHUNDERBIRD[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyTHUNDERBIRD[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_THUNDERBIRD_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strTHUNDERBIRDBpdName[i] );
		m_pBpdTHUNDERBIRD[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdTHUNDERBIRD[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdTHUNDERBIRD[i]);			
		}
	}
		sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < THUNDERBIRD_P_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strTHUNDERBIRD_PCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyTHUNDERBIRD_P[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyTHUNDERBIRD_P[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyTHUNDERBIRD_P[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_THUNDERBIRD_P_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strTHUNDERBIRD_PBpdName[i] );
		m_pBpdTHUNDERBIRD_P[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdTHUNDERBIRD_P[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdTHUNDERBIRD_P[i]);			
		}
	}
		sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETFAME1_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME1CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETFAME1[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETFAME1[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETFAME1[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETFAME1_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME1BpdName[i] );
		m_pBpdPETFAME1[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETFAME1[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETFAME1[i]);			
		}
	}
		sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETFAME2_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME2CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETFAME2[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETFAME2[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETFAME2[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETFAME2_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME2BpdName[i] );
		m_pBpdPETFAME2[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETFAME2[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETFAME2[i]);			
		}
	}
	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETFAME3_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME3CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETFAME3[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETFAME3[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETFAME3[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETFAME3_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME3BpdName[i] );
		m_pBpdPETFAME3[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETFAME3[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETFAME3[i]);			
		}
	}
		sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETFAME4_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME4CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETFAME4[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETFAME4[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETFAME4[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETFAME4_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETFAME4BpdName[i] );
		m_pBpdPETFAME4[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETFAME4[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETFAME4[i]);			
		}
	}
		sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETHELPER_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHELPERCsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETHELPER[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETHELPER[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETHELPER[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETHELPER_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHELPERBpdName[i] );
		m_pBpdPETHELPER[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETHELPER[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETHELPER[i]);			
		}
	}
	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETHALL1_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHALL1CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETHALL1[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETHALL1[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETHALL1[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETHALL1_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHALL1BpdName[i] );
		m_pBpdPETHALL1[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETHALL1[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETHALL1[i]);			
		}
	}
	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETHALL2_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHALL2CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETHALL2[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETHALL2[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETHALL2[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETHALL2_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHALL2BpdName[i] );
		m_pBpdPETHALL2[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETHALL2[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETHALL2[i]);			
		}
	}
	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETHALL3_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHALL3CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETHALL3[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETHALL3[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETHALL3[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETHALL3_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETHALL3BpdName[i] );
		m_pBpdPETHALL3[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETHALL3[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETHALL3[i]);			
		}
	}

	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETNATAL1_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETNATAL1CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETNATAL1[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETNATAL1[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETNATAL1[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETNATAL1_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETNATAL1BpdName[i] );
		m_pBpdPETNATAL1[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETNATAL1[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETNATAL1[i]);			
		}
	}
	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETNATAL2_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETNATAL2CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETNATAL2[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETNATAL2[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETNATAL2[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETNATAL2_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETNATAL2BpdName[i] );
		m_pBpdPETNATAL2[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETNATAL2[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETNATAL2[i]);			
		}
	}
	sprintf( strTemp, "./data/item/csy/" ); 
	for( i = 0; i < PETNATAL3_CSY_MAX; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETNATAL3CsyName[i] );
		g_pCapsyongTexture->SetTexturePath("data/item/textr/");
		m_pCsyPETNATAL3[i] = new CCapsyong(g_pCapsyongTexture);
		if( VF_ERR_NONE > m_pCsyPETNATAL3[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pCsyPETNATAL3[i]);			
		}
	}

	sprintf( strTemp, "./data/item/bpd/" );
	for( i = 0 ; i < FX_PETNATAL3_ANI_MAX ; i++ )
	{
		sprintf( strTemp2, "%s%s", strTemp, strPETNATAL3BpdName[i] );
		m_pBpdPETNATAL3[i] = new CBiped();
		if( VF_ERR_NONE > m_pBpdPETNATAL3[i]->Read(strTemp2) )
		{
			SAFE_DELETE(m_pBpdPETNATAL3[i]);			
		}
	}
}

void CFxFollow::FrameMove( EffectSort &effect_sort, int nAddFrame ) // 페어리의 움직임을 구현하자.
{
	int TotalFrame = 0; // 각 타입,애니메이션 별 총 프레임.
	float speed = 0.0f; // 움직이는 속도.
	D3DVECTOR vMasterPos;  // 마스터의 위치
	D3DVECTOR vMasterPosDir;  // 마스터로 향하는 벡터
	D3DVECTOR vTempVec;
	D3DVECTOR vTempVec2;
	D3DVECTOR vTempVec3;
	float fTempFloat = 0.0f;
	float MasterDist = 0.0f; // 마스터와의 거리 값.
	D3DMATRIX TempMatrix;
	char strTemp[256];

	D3DVECTOR	vViewVec, vRightVec;
	vViewVec = pCMyApp->GetEyeDirection();
	vRightVec = CrossProduct( D3DVECTOR(0.0f,1.0f,0.0f),vViewVec ); // 뷰로 부터 라이트를 구해놓는다.
	vRightVec = Normalize( vRightVec );

	DWORD dwElapsedTick = 0; // 지나간 틱을 계산하기 위하여 사용.

	static EffectSort ef_sort;

	// 타입별로 다른 AI를 지정한다.
	switch( effect_sort.Index ) // effect_sort 의 Index를 타입으로 사용한다.
	{
	case FX_FOLLOW_TYPE_FAIRY: // 페어리 타입

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 마스터와의 거리(y는 제외하고 체크.)

		if( effect_sort.num != FX_FAIRY_STATE_BLOCKING ) // 블로킹 중엔 따라가기나 위와래 움직임을 예외적으로 막아준다.
		{
			if( effect_sort.num != FX_FAIRY_STATE_FOLLOW && MasterDist > 20.0f ) // 마스터랑 일정거리 이상 멀어지면 쫒아가도록.
			{
				effect_sort.num = FX_FAIRY_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f; // 가속도 초기화.
			}
		}

		switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.(상태값이다.)
		{
		case FX_FAIRY_STATE_STAND: // 둥둥 떠있는 상태.
			TotalFrame = FX_FAIRY_MOVE_FRAME_TOTAL;

			// 위아래로 움직인다.
			speed = 0.05f; // 이동속도의 일정 비율을 위와래 이동 속도로...

			if( effect_sort.nCurFrame > FX_FAIRY_MOVE_FRAME_TOTAL/2 )
				speed *= -1.0f;

			effect_sort.vCurPos.y += (float)(nAddFrame) * speed;

			if( effect_sort.vCurPos.y > vMasterPos.y + 19.0f ) // 일정이상 올라가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 19.0f;
			if( effect_sort.vCurPos.y < vMasterPos.y + 12.0f ) // 일정이상 내려가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 12.0f;

			// 특정확률로 방향전환
			if( (rand()%1000) < 10 )
			{
				effect_sort.subNum *= -1;
			}

			// 상태 변화.
			if( (rand()%1000) < 45 )
			{
				effect_sort.num = FX_FAIRY_STATE_SIDESTEP;

				effect_sort.vPos.y = RandomNum( -0.5f, 0.5f ); // vPos.y를 오르고 내리는 속도및 방향으로 사용.
			}
			else if( (rand()%1000) < 50 )
			{
				effect_sort.num = FX_FAIRY_STATE_TURNROUND;
				effect_sort.vSortPos.y = 0.0f; // 가속도 초기화.
				effect_sort.vSortPos.z = ( rand()%2 ? 1.0f : -1.0f ); // 도는 방향.

				effect_sort.vPos.y = RandomNum( -0.5f, 0.5f ); // vPos.y를 오르고 내리는 속도및 방향으로 사용.
			}
			break;

		case FX_FAIRY_STATE_SIDESTEP:
			TotalFrame = FX_FAIRY_MOVE_FRAME_TOTAL;

			// 업다운 말고 크게 내려가고 올라가고의 조절
			effect_sort.vCurPos.y += (float)(nAddFrame) * effect_sort.vPos.y; // vPos.y를 오르고 내리는 속도및 방향으로 사용.

			if( effect_sort.vCurPos.y > vMasterPos.y + 19.0f ) // 일정이상 올라가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 19.0f;
			if( effect_sort.vCurPos.y < vMasterPos.y + 12.0f ) // 일정이상 내려가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 12.0f;

			// 특정확률로 방향전환
			if( (rand()%1000) < 20 )
			{
				effect_sort.subNum *= -1;
			}

			// 방향에 따라 카메라 각도를 기준으로 이동.
			vTempVec.x = effect_sort.vCurPos.x;
			vTempVec.z = effect_sort.vCurPos.z;

			speed =  (float)(nAddFrame)*0.5f; // 이동속도의 일정 비율을 위와래 이동 속도로...
			vTempVec.x += vRightVec.x * (float)effect_sort.subNum * speed;
			vTempVec.z += vRightVec.z * (float)effect_sort.subNum * speed;

			fTempFloat = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
									   , D3DVECTOR(vTempVec.x,0.0f,vTempVec.z) ); // 마스터와의 거리(y는 제외하고 체크.)

			if( fTempFloat < 11.0f && fTempFloat > 5.0f ) // 일정 반경안으로 이동시에만 이동하도록...
			{
				effect_sort.vCurPos.x = vTempVec.x;
				effect_sort.vCurPos.z = vTempVec.z;
			}

			// 상태 변화.
			if( (rand()%1000) < 30 )
			{
				effect_sort.num = FX_FAIRY_STATE_STAND;
			}

			break;

		case FX_FAIRY_STATE_TURNROUND: // 주인을 빙빙 도는 상태.

			TotalFrame = FX_FAIRY_MOVE_FRAME_TOTAL;

			// 위아래로 움직인다.
			effect_sort.vCurPos.y += (float)(nAddFrame) * effect_sort.vPos.y; // vPos.y를 오르고 내리는 속도및 방향으로 사용.

			if( effect_sort.vCurPos.y > vMasterPos.y + 19.0f ) // 일정이상 올라가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 19.0f;
			if( effect_sort.vCurPos.y < vMasterPos.y + 12.0f ) // 일정이상 내려가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 12.0f;



			effect_sort.vSortPos.y += 0.01f; // 가속도를 증가. 등가속도
			if( effect_sort.vSortPos.y > 0.25f ) // 일정이상 증가 못하게.
				effect_sort.vSortPos.y = 0.25f;

			speed = effect_sort.vSortPos.y; // vSortPos.y 를 속도 값으로 사용.

			vTempVec = effect_sort.vCurPos; // 이동하기 전의 좌표를 기억...

			Nk3dMath_VectorRotateYByVertex( effect_sort.vCurPos, vMasterPos, (float)nAddFrame * effect_sort.vSortPos.z * speed );

			// 스프라이트 방향을 결정.
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( TempMatrix );
			// 공간상의 점을 화면상의 점으로 바꾼다.
			D3DMath_VectorMatrixMultiply( vTempVec2, vTempVec, TempMatrix ); // 이동하기 전 좌표.
			D3DMath_VectorMatrixMultiply( vTempVec3, effect_sort.vCurPos, TempMatrix ); // 이동한 후 좌표.

			if( vTempVec2.x < vTempVec3.x ) // 오른쪽 방향으로 이동시...
				effect_sort.subNum = 1;
			else if( vTempVec2.x > vTempVec3.x ) // 같을때는 방향을 변경하면 안됨에 유의.
				effect_sort.subNum = -1;


			if( (rand()%1000) < 30 )
			{
				effect_sort.num = FX_FAIRY_STATE_STAND;
			}

			break;


		case FX_FAIRY_STATE_FOLLOW: // 주인을 쫒아가는 상태.
			TotalFrame = FX_FAIRY_MOVE_FRAME_TOTAL;

			if( MasterDist < 7.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
			{
				effect_sort.num = FX_FAIRY_STATE_STAND;

				break;
			}

			vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			vTempVec = effect_sort.vCurPos; // 이동하기 전의 좌표를 기억...

			if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 15.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
			{
				// 여기서 계속 따라가려고 하면 움직임이 튀는 버그가 있어 그냥 임시로 붙여버리도록 땜빵.

				vMasterPosDir *= -8.0f;

				effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
				effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;
			}
			else
			{
				effect_sort.vSortPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
				if( effect_sort.vSortPos.y > 2.5f ) // 일정이상 증가 못하게.
					effect_sort.vSortPos.y = 2.5f;

				speed = effect_sort.vSortPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
				if( speed > MasterDist+5.0f )
					speed = MasterDist+5.0f;

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;
			}

			// 스프라이트 방향을 결정.
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( TempMatrix );
			// 공간상의 점을 화면상의 점으로 바꾼다.
			D3DMath_VectorMatrixMultiply( vTempVec2, vTempVec, TempMatrix ); // 이동하기 전 좌표.
			D3DMath_VectorMatrixMultiply( vTempVec3, effect_sort.vCurPos, TempMatrix ); // 이동한 후 좌표.

			if( vTempVec2.x < vTempVec3.x ) // 오른쪽 방향으로 이동시...
				effect_sort.subNum = 1;
			else if( vTempVec2.x > vTempVec3.x ) // 같을때는 방향을 변경하면 안됨에 유의.
				effect_sort.subNum = -1;

			break;

		case FX_FAIRY_STATE_BLOCKING: // 방어막치기.
			TotalFrame = FX_FAIRY_BLOCK_FRAME_TOTAL;

			// 스프라이트 방향을 결정.
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( TempMatrix );
			// 공간상의 점을 화면상의 점으로 바꾼다.
			D3DMath_VectorMatrixMultiply( vTempVec2, D3DVECTOR(effect_sort.vPos.x,effect_sort.vCurPos.y,effect_sort.vPos.z), TempMatrix );
			D3DMath_VectorMatrixMultiply( vTempVec3, effect_sort.vCurPos, TempMatrix );

			if( vTempVec2.x < vTempVec3.x ) // 오른쪽 방향으로 이동시...
				effect_sort.subNum = 1;
			else if( vTempVec2.x > vTempVec3.x ) // 같을때는 방향을 변경하면 안됨에 유의.
				effect_sort.subNum = -1;


			break;
		}

		// 프레임 갱신
		effect_sort.nCurFrame += nAddFrame;
		if( effect_sort.nCurFrame >= TotalFrame )
		{
			effect_sort.nCurFrame = 0; // 일단 이 형태의 이펙트는 전부 루프.

			if( effect_sort.num == FX_FAIRY_STATE_BLOCKING ) // 예외적으로 1회로 끝나는 애니메이션
				effect_sort.num = FX_FAIRY_STATE_STAND;
		}

		// 파티클 갱신.
		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

		if( m_pParticleInfoArray )
		{
			for( int i = 0 ; i < MAX_PARTICLE_FOR_FXFOLLOW ; ++i )
			{
				m_pParticleInfoArray[i].life_time += nAddFrame%3; // curTailPosNum을 라이프로 사용.
				m_pParticleInfoArray[i].red = 1.0f;
				m_pParticleInfoArray[i].green = 1.0f;
				m_pParticleInfoArray[i].blue = 1.0f;

				if( m_pParticleInfoArray[i].life_time > 20+(int)RandomNum(-2.0f,5.0f) ) // 수명은 임시.
				{
					// 수명이 다한 녀석이 있으면 새로 생성
					m_pParticleInfoArray[i].life_time = 0;
					m_pParticleInfoArray[i].cur_Pos.x = effect_sort.vCurPos.x+RandomNum(-0.7f,0.7f);
					m_pParticleInfoArray[i].cur_Pos.y = effect_sort.vCurPos.y;
					m_pParticleInfoArray[i].cur_Pos.z = effect_sort.vCurPos.z+RandomNum(-0.7f,0.7f);

					m_pParticleInfoArray[i].fade = 1.0f; // 투명도. 점점 줄어든다.
					m_pParticleInfoArray[i].scale = RandomNum( 0.4f, 0.6f ); // 입자크기를 랜덤.
				}

				m_pParticleInfoArray[i].fade -= 0.04f*(float)nAddFrame; // 투명도. 점점 줄어든다.
				if( m_pParticleInfoArray[i].fade < 0.0f )
					m_pParticleInfoArray[i].fade = 0.0f;

				m_pParticleInfoArray[i].cur_Pos.x += RandomNum( -0.1f, 0.1f ); // 약간 흔들리게...
				m_pParticleInfoArray[i].cur_Pos.y -= (float)nAddFrame*0.2f; // 일정 속도로 떨어지게.
				m_pParticleInfoArray[i].cur_Pos.z += RandomNum( -0.1f, 0.1f ); // 약간 흔들리게...
			}
		}

		break;

	case FX_FOLLOW_TYPE_LUCKYPIG: // 설날 복돼지.

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 마스터와의 거리(y는 제외하고 체크.)

		if( effect_sort.num != FX_LUCKYPIG_STATE_HEAL ) // 이 동작중엔 따라가는 움직임을 예외적으로 막아준다.
		{
			if( effect_sort.num != FX_LUCKYPIG_STATE_FOLLOW && MasterDist > 30.0f ) // 마스터랑 일정거리 이상 멀어지면 쫒아가도록.
			{
				effect_sort.num = FX_LUCKYPIG_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
				effect_sort.vPos.y = 0.0f; // 가속도 초기화.
			}
		}

		// 이전 프레임에서의 시간에서 진행해야할 프레임을 구한다.(돼지는 속도 세세하게 제어하려고 nAddFrame을 안쓴다.프레임 제어만 안쓰지 이동들은 nAddFrame을 씀에 유의. )
		dwElapsedTick = timeGetTime() - effect_sort.dwColor; // 이전 프레임에서 진행된 틱
		effect_sort.dwColor = timeGetTime(); // 갱신 시켜둔다.


		if( effect_sort.pNkChaFrom == g_pRoh && !g_pRoh->IsDead() ) // 자기 돼지면...// 죽었을땐 돼지 회복 안하게 처리..(07-03-06)
		{
			if( g_pRoh->m_Wearing[11] && !g_pRoh->m_Wearing[11]->m_Special ) // 옵션이 붙은 돼지는 무능력 돼지. 07-04-04 추가 ///설날돼지
			{
				effect_sort.vSortPos.z += (float)dwElapsedTick; // 틱을 누적.

				// 패킷 보내기...
				if( (DWORD)effect_sort.vSortPos.z >= 60000 ) // 회복 타이밍 체크 (현재는 1분에 한번씩.)
				{
					sprintf( strTemp, "use_effect %d 1\n", effect_sort.subNum );

					if (g_pTcpIp)
						g_pTcpIp->SendNetMessage(strTemp);

					effect_sort.vSortPos.z = 0.0f; // 다시 초기화.
				}
			}
		}

		switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
		{
		case FX_LUCKYPIG_STATE_STAND:  // 그냥 서있는 상태.
			
			// 프레임 갱신
			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdLuckyPig[FX_LUCKYPIG_ANI_STAND]->m_AnimCount ) // 루프 처리
			{
				effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
			}

			// 랜덤하게 스테이트 변경
			if( (rand()%1000) < 25 )
			{
				effect_sort.num = FX_LUCKYPIG_STATE_SOCIAL1;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}
			else if( (rand()%1000) < 20 )
			{
				effect_sort.num = FX_LUCKYPIG_STATE_SOCIAL2;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}
			else if( (rand()%1000) < 40 )
			{
				effect_sort.num = FX_LUCKYPIG_STATE_RANDOMWALK;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				effect_sort.vPos.y = 0.0f; // 가속도 초기화.

				// 목표지점을 주인을 기준으로 정한다.
				effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
				effect_sort.vTargetPos.y = 0.0f;
				effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
			}

			break;

		case FX_LUCKYPIG_STATE_RANDOMWALK:  // 아무 방향으로나 걸어감.(천천히)

			// 목표점에 일정 거리 이상 다가오면 상태 전환.
			MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
									   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

			if( MasterDist < 5.0f )
			{
				// 상태를 스탠드로 돌려준다.
				effect_sort.num = FX_LUCKYPIG_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

				break;
			}


			// 프레임 갱신
			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdLuckyPig[FX_LUCKYPIG_ANI_WALK]->m_AnimCount ) // 루프 처리
			{
				effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
			}


			// 이동
			vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
			effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


			speed = 0.008f * dwElapsedTick; // 속도 고정.

			vMasterPosDir *= speed;

			effect_sort.vCurPos.x += vMasterPosDir.x;
			effect_sort.vCurPos.z += vMasterPosDir.z;

			break;

		case FX_LUCKYPIG_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

			// 상태변화 체크
			if( MasterDist < 10.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
			{
				effect_sort.num = FX_LUCKYPIG_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

				break;
			}

			vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			// 돼지의 바라보는 방향을 주인쪽으로... vSortPos.x 를 방향값으로 사용.
			effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

			if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 20.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
			{
				// 여기서 계속 따라가려고 하면 움직임이 튀는 버그가 있어 그냥 임시로 붙여버리도록 땜빵.

				vMasterPosDir *= -15.0f;

				effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
				effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

				// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
				effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

				if( (int)effect_sort.vSortPos.y >= m_pBpdLuckyPig[FX_LUCKYPIG_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}
			}
			else
			{
				effect_sort.vPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
				if( effect_sort.vPos.y > 2.5f ) // 일정이상 증가 못하게.
					effect_sort.vPos.y = 2.5f;

				speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
				if( speed > MasterDist+5.0f )
					speed = MasterDist+5.0f;

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;

				// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
				effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.02f; // 프레임율을 거리에 반비례해서 조정

				if( (int)effect_sort.vSortPos.y >= m_pBpdLuckyPig[FX_LUCKYPIG_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}
			}

			break;

		case FX_LUCKYPIG_STATE_SOCIAL1:  // 서서 애교동작

			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdLuckyPig[FX_LUCKYPIG_ANI_SOCIAL1]->m_AnimCount ) // 루프 처리
			{
//				effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				// 이 녀석은 모션이 한번 끝나면 돌려준다.
				effect_sort.num = FX_LUCKYPIG_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}
			/*
						if( (rand()%1000) < 30 ) // 애니메이션 한번 단위가 아니라 랜덤하게 스탠드로...
						{
							effect_sort.num = FX_LUCKYPIG_STATE_STAND;
							effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						}
			*/
			break;

		case FX_LUCKYPIG_STATE_SOCIAL2:	 // 클라이언트에서 주변 돌리는 춤


			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdLuckyPig[FX_LUCKYPIG_ANI_SOCIAL2]->m_AnimCount ) // 루프 처리
			{
				effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
			}

			Nk3dMath_VectorRotateYByVertex( effect_sort.vCurPos, vMasterPos, speed ); // 얘는 도는 방향은 고정이다. // 여기선 그냥 nAddFrame을 씀에 유의.

			if( (rand()%1000) < 10 )
			{
				effect_sort.num = FX_LUCKYPIG_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}

			break;


		case FX_LUCKYPIG_STATE_HEAL:	 // 회복 모션

			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdLuckyPig[FX_LUCKYPIG_ANI_HEAL]->m_AnimCount ) // 루프 처리
			{
				// 이 녀석은 모션이 한번 끝나면 돌려준다.
				effect_sort.num = FX_LUCKYPIG_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}

			// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
			vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );
			effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


			break;
		}

		if( g_bNewTerrain ) // 워크 플레인도 올라갈수있도록.
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
			effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

			if( effect_sort.vCurPos.y < fHeight )
				effect_sort.vCurPos.y = fHeight;
		}
		else
			effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);

		break;

	// [7/31/2007 Theodoric] 6주년 기념 이벤트 베이비 소울
	case FX_FOLLOW_TYPE_BABYSOUL:

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 마스터와의 거리(y는 제외하고 체크.)

		if( effect_sort.num != (float)FX_BABYSOUL_STATE_CRITICAL ) // 이 동작중엔 따라가는 움직임을 예외적으로 막아준다.
		{
			if( effect_sort.num == (float)FX_BABYSOUL_STATE_STAND && MasterDist > 35.0f ) // 마스터랑 일정거리 이상 멀어지면 쫒아가도록.
			{
				effect_sort.num = effect_sort.num+1;
				effect_sort.vTargetPos.z	= (float)FX_BABYSOUL_STATE_WALK;
				//effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
				//effect_sort.vPos.y = 0.0f; // 가속도 초기화.
			}
		}

		// 이전 프레임에서의 시간에서 진행해야할 프레임을 구한다.(속도 세세하게 제어하려고 nAddFrame을 안쓴다.프레임 제어만 안쓰지 이동들은 nAddFrame을 씀에 유의. )
		dwElapsedTick = timeGetTime() - effect_sort.dwColor; // 이전 프레임에서 진행된 틱
		effect_sort.dwColor = timeGetTime(); // 갱신 시켜둔다.

		int randState;

		switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
		{
		case FX_BABYSOUL_STATE_STAND:
			// 프레임 갱신

			effect_sort.vTargetPos.x = 0.0f; // 스케일 값
			effect_sort.vTargetPos.y = (float)BS_TELE_SHOW; // 텔리포트 상태 값
			effect_sort.vTargetPos.z = (float)FX_BABYSOUL_STATE_NONE;

			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_STAND]->m_AnimCount ) // 루프 처리
			{
				effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y;    // 소수점만 남긴다
			}

			randState = rand()%1000;

			if( randState < 10 )
			{
				effect_sort.num = effect_sort.num+1;
				effect_sort.vTargetPos.z	= (float)FX_BABYSOUL_STATE_STAND_TELE;
			}
			else if( randState < 25 )
			{
				effect_sort.num = effect_sort.num+1;
				effect_sort.vTargetPos.z	= (float)FX_BABYSOUL_STATE_STAND_UP;
			}
			else if( randState < 70 )
			{
				// 바라볼 방향을 랜덤하게 해준다.
				int fx = (rand()%100) * ((rand()%2)? -1:1);
				int fz = (rand()%100) * ((rand()%2)? -1:1);

				//vMasterPosDir = D3DVECTOR( (float)fx, 0.0f, (float)fz) ;
				vMasterPosDir = Normalize( D3DVECTOR( (float)fx, 0.0f, (float)fz) );
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x);
			}

			break;

		case FX_BABYSOUL_STATE_STAND_TO: // 서있는 동작에서 다른 동작을 넘어갈때.

			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_STAND]->m_AnimCount )
			{
				if( effect_sort.vTargetPos.z == (float)FX_BABYSOUL_STATE_STAND_TELE )
					effect_sort.vTargetPos.y = (float)BS_TELE_FADEOUT;

				effect_sort.num = effect_sort.vTargetPos.z;
				effect_sort.vTargetPos.z		= (float)FX_BABYSOUL_STATE_NONE;

				effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
				effect_sort.vPos.y = 0.0f; // 가속도 초기화.
			}
			break;

		case FX_BABYSOUL_STATE_STAND_UP:
			// 프레임 갱신
			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_STAND_UP]->m_AnimCount ) // 루프 처리
			{
				effect_sort.num = (float)FX_BABYSOUL_STATE_STAND_UP_TO;
				effect_sort.vTargetPos.z		= (float)FX_BABYSOUL_STATE_STAND;
			} // 소수점만 남긴다

			break;

		case FX_BABYSOUL_STATE_STAND_UP_TO: // 서있는 동작에서 다른 동작을 넘어갈때.

			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_STAND_UP]->m_AnimCount )
			{
				effect_sort.num = effect_sort.vTargetPos.z;
				effect_sort.vTargetPos.z		= (float)FX_BABYSOUL_STATE_NONE;

				effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
			}
			break;

		case FX_BABYSOUL_STATE_STAND_TELE:

			switch( (int)effect_sort.vTargetPos.y )
			{
			case BS_TELE_FADEOUT:

				effect_sort.vTargetPos.x += 0.15f;

				if (effect_sort.vTargetPos.x >= 0.8f)
				{
					effect_sort.vTargetPos.x = 0.8f;
					effect_sort.vTargetPos.y = (float)BS_TELE_HIDE;
				}

				break;

			case BS_TELE_HIDE:

				if( timeGetTime()% 1000 < 10 )
				{
//					while(1)
					{
						// 등장할 위치를 랜덤하게 찍어준다.
						effect_sort.vCurPos.x = vMasterPos.x + 10.0f + RandomNum( -15.0f, 15.0f );
						effect_sort.vCurPos.z = vMasterPos.z + 10.0f + RandomNum( -15.0f, 15.0f );

						// 길드 랭킹존에서는 무한 루프가 돌기 때문에 그냥 탈출 시킨다.
						// 보이지 않는 캐릭터 정보를 보내기 대문에 무한 루프가 도는 듯.
//						if( pCMyApp->GetCurWorld() == 11 )
//							break;

						// 이동할 지점이 이동 가능 지역이면 이동한다.
//						if( pCMyApp->m_pUIMgr->m_pLand->IsMovableArea(effect_sort.vCurPos.x, effect_sort.vCurPos.z) )
//							break;
					}

					// 바라볼 방향을 랜덤하게 해준다.
					vMasterPosDir = D3DVECTOR( 0.0f, 0.0f, 0.0f ) -
									D3DVECTOR( RandomNum( -100.0f, 100.0f ), 0.0f, + 10.0f + RandomNum( -100.0f, 100.0f ) );
					vMasterPosDir = Normalize( vMasterPosDir );
					effect_sort.vSortPos.x = (float)atan2(vMasterPosDir.z, vMasterPosDir.x);

					effect_sort.vTargetPos.y = (float)BS_TELE_FADEIN;
				}
				break;

			case BS_TELE_FADEIN:

				effect_sort.vTargetPos.x -= 0.15f;

				if (effect_sort.vTargetPos.x <= 0.0f)
				{
					effect_sort.vTargetPos.x	= 0.0f;
					effect_sort.vTargetPos.y	= (float)BS_TELE_SHOW;
					effect_sort.num				= (float)FX_BABYSOUL_STATE_STAND;
					effect_sort.vTargetPos.z	= (float)FX_BABYSOUL_STATE_NONE;
				}

				break;
			}
			break;

		case FX_BABYSOUL_STATE_STAND_TELE_TO: // 서있는 동작에서 다른 동작을 넘어갈때.


			break;

		case FX_BABYSOUL_STATE_WALK:

			// 상태변화 체크
			if( MasterDist < 20.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
			{
				effect_sort.num = (float)FX_BABYSOUL_STATE_WALK_TO;
				effect_sort.vTargetPos.z		= (float)FX_BABYSOUL_STATE_STAND;
				//effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				break;
			}

			vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			// 바라보는 방향을 주인쪽으로... vSortPos.x 를 방향값으로 사용.
			effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

			if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 10.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
			{
				// 여기서 계속 따라가려고 하면 움직임이 튀는 버그가 있어 그냥 임시로 붙여버리도록 땜빵.

				vMasterPosDir *= -15.0f;

				effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
				effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

				// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
				effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

				if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y;    // 소수점만 남긴다.
				}
			}
			else
			{
				effect_sort.vPos.y += 0.03f * (float)(nAddFrame); // 가속도를 증가. 등가속도
				if( effect_sort.vPos.y > 1.0f ) // 일정이상 증가 못하게.
					effect_sort.vPos.y = 1.0f;

				speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
				if( speed > MasterDist+5.0f )
					speed = MasterDist+5.0f;

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;

				// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
				effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.02f; // 프레임율을 거리에 반비례해서 조정

				if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y;   // 소수점만 남긴다.
				}
			}

			break;


		case FX_BABYSOUL_STATE_WALK_TO:

			effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.02f; // 프레임율을 거리에 반비례해서 조정

			if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_WALK]->m_AnimCount ||
					(int)effect_sort.vSortPos.y == m_pBpdBabySoul[FX_BABYSOUL_ANI_WALK]->m_AnimCount /2 )
			{
				effect_sort.num = effect_sort.vTargetPos.z;
				effect_sort.vTargetPos.z		= (float)FX_BABYSOUL_STATE_NONE;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}
			break;

		case FX_BABYSOUL_STATE_CRITICAL:

			effect_sort.vTargetPos.x = 0.0f;
			effect_sort.vTargetPos.y = (float)BS_TELE_SHOW;

			effect_sort.vSortPos.y += dwElapsedTick * 0.02f;

			if( (int)effect_sort.vSortPos.y >= m_pBpdBabySoul[FX_BABYSOUL_ANI_CRITICAL]->m_AnimCount )
			{
				effect_sort.num = (float)FX_BABYSOUL_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}
			break;
		}

		if( g_bNewTerrain ) // 워크 플레인도 올라갈수있도록.
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
			effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

			if( effect_sort.vCurPos.y < fHeight )
				effect_sort.vCurPos.y = fHeight;
		}
		else
			effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);

		break;

	case FX_FOLLOW_TYPE_VAMPIREBAT: ///뱀파이어망토 박쥐

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 마스터와의 거리(y는 제외하고 체크.)

		if( effect_sort.num != FX_VAPIREBAT_STATE_FOLLOW && MasterDist > 15.0f ) // 마스터랑 일정거리 이상 멀어지면 쫒아가도록.
		{
			effect_sort.num = FX_VAPIREBAT_STATE_FOLLOW;
			effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
			effect_sort.vPos.y = 0.0f; // 가속도 초기화.
		}

		switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.(상태값이다.)
		{
		case FX_VAPIREBAT_STATE_STAND: // 그냥 서있는 상태. 파닥파닥, 좀 흔들린다.
			TotalFrame = FX_VAPIREBAT_STAND_FRAME_TOTAL;

			// 위아래로 움직인다.
			speed = 0.7f; // 이동속도의 일정 비율을 위와래 이동 속도로...

			effect_sort.vCurPos.x += RandomNum(-0.5f, 0.5f) * (float)(nAddFrame) * speed;
			effect_sort.vCurPos.y += RandomNum(-0.5f, 0.5f) * (float)(nAddFrame) * speed;
			effect_sort.vCurPos.z += RandomNum(-0.5f, 0.5f) * (float)(nAddFrame) * speed;
			effect_sort.vSortPos.z = RandomNum(-0.4f, 0.4f); // 서있을때 회전값.

			if( effect_sort.vCurPos.y > vMasterPos.y + 21.0f ) // 일정이상 올라가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 21.0f;
			if( effect_sort.vCurPos.y < vMasterPos.y + 15.0f ) // 일정이상 내려가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 15.0f;

			// 상태 변화.
			if( (rand()%1000) < 25 )
			{
				effect_sort.num = FX_VAPIREBAT_STATE_TURNROUND;
				effect_sort.vSortPos.y = 0.0f; // 가속도 초기화.
				effect_sort.vSortPos.z = ( rand()%2 ? 1.0f : -1.0f ); // 도는 방향.

				effect_sort.vPos.y = RandomNum( -0.5f, 0.5f ); // vPos.y를 오르고 내리는 속도및 방향으로 사용.
			}

			break;


		case FX_VAPIREBAT_STATE_TURNROUND: // 주인을 빙빙 도는 상태.

			TotalFrame = FX_VAPIREBAT_MOVE_FRAME_TOTAL;

			// 위아래로 움직인다.
			effect_sort.vCurPos.y += (float)(nAddFrame) * effect_sort.vPos.y; // vPos.y를 오르고 내리는 속도및 방향으로 사용.

			if( effect_sort.vCurPos.y > vMasterPos.y + 21.0f ) // 일정이상 올라가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 21.0f;
			if( effect_sort.vCurPos.y < vMasterPos.y + 15.0f ) // 일정이상 내려가지 못하게..
				effect_sort.vCurPos.y = vMasterPos.y + 15.0f;



			effect_sort.vSortPos.y += 0.004f; // 가속도를 증가. 등가속도
			if( effect_sort.vSortPos.y > 0.10f ) // 일정이상 증가 못하게.
				effect_sort.vSortPos.y = 0.10f;

			speed = effect_sort.vSortPos.y; // vSortPos.y 를 속도 값으로 사용.

			vTempVec = effect_sort.vCurPos; // 이동하기 전의 좌표를 기억...

			Nk3dMath_VectorRotateYByVertex( effect_sort.vCurPos, vMasterPos, (float)nAddFrame * effect_sort.vSortPos.z * speed );

			// 스프라이트 방향을 결정.
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( TempMatrix );
			// 공간상의 점을 화면상의 점으로 바꾼다.
			D3DMath_VectorMatrixMultiply( vTempVec2, vTempVec, TempMatrix ); // 이동하기 전 좌표.
			D3DMath_VectorMatrixMultiply( vTempVec3, effect_sort.vCurPos, TempMatrix ); // 이동한 후 좌표.

			if( vTempVec2.x < vTempVec3.x ) // 오른쪽 방향으로 이동시...
				effect_sort.subNum = 1;
			else if( vTempVec2.x > vTempVec3.x ) // 같을때는 방향을 변경하면 안됨에 유의.
				effect_sort.subNum = -1;


			if( (rand()%1000) < 30 )
			{
				effect_sort.num = FX_VAPIREBAT_STATE_STAND;
			}

			break;


		case FX_VAPIREBAT_STATE_FOLLOW: // 주인을 쫒아가는 상태.
			TotalFrame = FX_VAPIREBAT_MOVE_FRAME_TOTAL;

			if( MasterDist < 4.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
			{
				effect_sort.num = FX_FAIRY_STATE_STAND;

				break;
			}

			vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			vTempVec = effect_sort.vCurPos; // 이동하기 전의 좌표를 기억...

			if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 15.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
			{
				// 여기서 계속 따라가려고 하면 움직임이 튀는 버그가 있어 그냥 임시로 붙여버리도록 땜빵.

				vMasterPosDir *= -10.0f;

				effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
				effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;
			}
			else
			{
				effect_sort.vSortPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
				if( effect_sort.vSortPos.y > 2.5f ) // 일정이상 증가 못하게.
					effect_sort.vSortPos.y = 2.5f;

				speed = effect_sort.vSortPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
				if( speed > MasterDist+5.0f )
					speed = MasterDist+5.0f;

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;
			}

			// 스프라이트 방향을 결정.
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( TempMatrix );
			// 공간상의 점을 화면상의 점으로 바꾼다.
			D3DMath_VectorMatrixMultiply( vTempVec2, vTempVec, TempMatrix ); // 이동하기 전 좌표.
			D3DMath_VectorMatrixMultiply( vTempVec3, effect_sort.vCurPos, TempMatrix ); // 이동한 후 좌표.

			if( vTempVec2.x < vTempVec3.x ) // 오른쪽 방향으로 이동시...
				effect_sort.subNum = 1;
			else if( vTempVec2.x > vTempVec3.x ) // 같을때는 방향을 변경하면 안됨에 유의.
				effect_sort.subNum = -1;
		}

		// 프레임 갱신
		effect_sort.nCurFrame += nAddFrame*2;
		if( effect_sort.nCurFrame >= TotalFrame )
		{
			effect_sort.nCurFrame = 0; // 일단 이 형태의 이펙트는 전부 루프.
		}

		// 궤적 갱신.
		if( nAddFrame > 0 ) // 프레임 증가가 있었을 때만 해주자.
		{
			list<D3DVECTOR> *TraceInfoList;
			TraceInfoList = (list<D3DVECTOR> *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.
			D3DVECTOR TempVertex;

			TempVertex = D3DVECTOR( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			TraceInfoList->push_back( TempVertex ); // 점 입력.

			if( effect_sort.num == FX_VAPIREBAT_STATE_FOLLOW ) // 쫒아가는중엔 더 짧게
			{
				while( TraceInfoList->size() >= 7 ) // 이것보다 길면.
					TraceInfoList->pop_front(); // 빼준다.
			}

			if( TraceInfoList->size() >= MAX_VAMPIREBAT_TRACE ) // 일정 갯수 이상 안늘어나게.
			{
				TraceInfoList->pop_front(); // 앞에 하나 빼준다.
			}
		}

		break;

	case FX_FOLLOW_TYPE_ARUPAKAPET:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			// 캐릭터와의 거리
			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z), D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			// 처음생성시 멍떄리면 안되니까 움직임을 준다
			if(  effect_sort.num != FX_ARUPAKA_ANI_WALK && MasterDist > 30.0f )
			{
				effect_sort.num = FX_ARUPAKA_ANI_WALK;
				effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
				effect_sort.vPos.y = 0.0f; // 가속도 초기화.
			}

//			if( effect_sort.num != FX_ARUPAKA_ANI_SPIT ) m_bSpit = false;

			// 이전 프레임에서의 시간에서 진행해야할 프레임을 구한다.(돼지는 속도 세세하게 제어하려고 nAddFrame을 안쓴다.프레임 제어만 안쓰지 이동들은 nAddFrame을 씀에 유의. )
			dwElapsedTick = timeGetTime() - effect_sort.dwColor; // 이전 프레임에서 진행된 틱
			effect_sort.dwColor = timeGetTime(); // 갱신 시켜둔다.

			switch( effect_sort.num )
			{
			case FX_ARUPAKA_ANI_SPIT:
				effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

				vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				// 돼지의 바라보는 방향을 주인쪽으로... vSortPos.x 를 방향값으로 사용.
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

				// 침뱉는 이펙트
				if( (int)effect_sort.vSortPos.y == 50 )
				{
					ef_sort.nType		= FX_ARUPAKA_SPIT;
					ef_sort.vTargetPos	= vMasterPos;
					ef_sort.pNkChaTo	= effect_sort.pNkChaFrom;
					ef_sort.vPos		= effect_sort.vCurPos + (vMasterPosDir*7);

					if( ef_sort.pNkChaTo )
					{
						ef_sort.pNkChaTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					}

					m_vMasterPosDir = vMasterPos;
				}

				// 바닥 침
				if( (int)effect_sort.vSortPos.y == 55 )
					m_bSpit = true;
				if( (int)effect_sort.vSortPos.y == 61 )
					m_bSpit = false;


				if( (int)effect_sort.vSortPos.y >= m_pBpdArupaka[FX_ARUPAKA_ANI_SPIT]->m_AnimCount ) // 종료 처리
				{
					effect_sort.num = FX_ARUPAKA_ANI_STAND;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				}

				break;
			case FX_ARUPAKA_ANI_DOWN:
				effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

				if( (int)effect_sort.vSortPos.y >= m_pBpdArupaka[FX_ARUPAKA_ANI_DOWN]->m_AnimCount ) // 종료 처리
				{
					effect_sort.num = FX_ARUPAKA_ANI_STAND;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				}
				break;
			case FX_ARUPAKA_ANI_STAND:
				// 랜덤하게 스테이트 변경
				if( (rand()%500) <  20)
				{
					effect_sort.num = FX_ARUPAKA_ANI_SPIT;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					effect_sort.subNum = 0; // 끝내기 체크용.
				}
				else if( (rand()%500) < 40 )
				{
					effect_sort.num = FX_ARUPAKA_ANI_DOWN;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					effect_sort.vPos.y = 0.0f; // 가속도 초기화
				}
				else
				{
					effect_sort.num = FX_ARUPAKA_ANI_RANDOMWALK;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					effect_sort.vPos.y = 0.0f; // 가속도 초기화.

					// 목표지점을 주인을 기준으로 정한다.
					effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
					effect_sort.vTargetPos.y = 0.0f;
					effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
				}

				break;
			case FX_ARUPAKA_ANI_RANDOMWALK:
				// 목표점에 일정 거리 이상 다가오면 상태 전환.
				MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
										   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

				if( MasterDist < 5.0f )
				{
					// 상태를 스탠드로 돌려준다.
					effect_sort.num = FX_ARUPAKA_ANI_WALK;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					break;
				}


				// 프레임 갱신
				effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

				if( (int)effect_sort.vSortPos.y >= m_pBpdArupaka[FX_ARUPAKA_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}


				// 이동
				vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


				speed = 0.008f * dwElapsedTick; // 속도 고정.

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;
				break;

			case FX_ARUPAKA_ANI_WALK:

				// 상태변화 체크
				if( MasterDist < 15.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
				{
					effect_sort.num = FX_ARUPAKA_ANI_STAND;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

					break;
				}

				vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				// 돼지의 바라보는 방향을 주인쪽으로... vSortPos.x 를 방향값으로 사용.
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

				if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 20.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
				{
					// 여기서 계속 따라가려고 하면 움직임이 튀는 버그가 있어 그냥 임시로 붙여버리도록 땜빵.

					vMasterPosDir *= -15.0f;

					effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
					effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

					// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
					effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdArupaka[FX_ARUPAKA_ANI_WALK]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}
				else
				{
					effect_sort.vPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
					if( effect_sort.vPos.y > 2.5f ) // 일정이상 증가 못하게.
						effect_sort.vPos.y = 2.5f;

					speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
					if( speed > MasterDist+5.0f )
						speed = MasterDist+5.0f;

					vMasterPosDir *= speed;

					if( MasterDist < 15 ) break;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
					effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.03f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdArupaka[FX_ARUPAKA_ANI_WALK]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}

				break;
			}

			if( g_bNewTerrain ) // 워크 플레인도 올라갈수있도록.
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;

	case FX_FOLLOW_TYPE_CATS:

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

		if( effect_sort.num != CAT_STATE__FOLLOW && MasterDist > 30.0f )
		{
			effect_sort.num = CAT_STATE__FOLLOW;
			effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
			effect_sort.vPos.y = 0.0f; // 가속도 초기화.
		}

		dwElapsedTick = timeGetTime() - effect_sort.dwColor; // 이전 프레임에서 진행된 틱
		effect_sort.dwColor = timeGetTime(); // 갱신 시켜둔다.

		switch( effect_sort.num )
		{
		case CAT_STATE__STAND:
			{
				effect_sort.vSortPos.y = 0;

				if( (rand()%1000) < 50 )
				{
					effect_sort.num = CAT_STATE__RANDOMMOVE;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					effect_sort.subNum = 0; // 끝내기 체크용.

					effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
					effect_sort.vTargetPos.y = 0.0f;
					effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
				}
			}
			break;

		case CAT_STATE__SOCIAL1:
			{
				if( (int)effect_sort.vSortPos.y < 23 )
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)
				else
					effect_sort.vSortPos.y += dwElapsedTick * 0.015f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)


				if( effect_sort.subNum < 3 )
				{
					if( (int)effect_sort.vSortPos.y >= m_pBpdCats[FX_CATS_ANI_STAND2]->m_AnimCount-7 ) // 루프 처리
					{
						effect_sort.vSortPos.y = 24;

						++effect_sort.subNum;
					}
				}
				else
				{
					if( (int)effect_sort.vSortPos.y >= m_pBpdCats[FX_CATS_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = CAT_STATE__STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					}
				}
			}
			break;
		case CAT_STATE__RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

			{
				MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
										   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

				if( MasterDist < 5.0f )
				{
					// 상태를 스탠드로 돌려준다.
					effect_sort.num = CAT_STATE__STAND;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

					break;
				}

				// 프레임 갱신
				effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

				if( (int)effect_sort.vSortPos.y >= m_pBpdCats[FX_CATS_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}

				// 이동
				vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


				speed = 0.008f * dwElapsedTick; // 속도 고정.

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;
			}

			break;

		case CAT_STATE__FOLLOW:  // 주인을 쫒아가는 상태.

			{
				if( MasterDist < 10.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
				{
					effect_sort.num = 0;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					break;
				}

				vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				// 돼지의 바라보는 방향을 주인쪽으로... vSortPos.x 를 방향값으로 사용.
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

				if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 20.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
				{
					// 여기서 계속 따라가려고 하면 움직임이 튀는 버그가 있어 그냥 임시로 붙여버리도록 땜빵.

					vMasterPosDir *= -15.0f;

					effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
					effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

					// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
					effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdCats[FX_CATS_ANI_WALK]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}
				else
				{
					effect_sort.vPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
					if( effect_sort.vPos.y > 2.5f ) // 일정이상 증가 못하게.
						effect_sort.vPos.y = 2.5f;

					speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
					if( speed > MasterDist+5.0f )
						speed = MasterDist+5.0f;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.03f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdCats[FX_CATS_ANI_WALK]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}
			}

			break;
		}

		if( g_bNewTerrain )
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
			effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

			if( effect_sort.vCurPos.y < fHeight )
				effect_sort.vCurPos.y = fHeight;
		}
		else
			effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);

		break;

	case FX_FOLLOW_TYPE_VALO:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
									   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != VALO_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = VALO_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();


			if( effect_sort.pNkChaFrom == g_pRoh && !g_pRoh->IsDead() && effect_sort.subNum == SKILL_PET_VALO_BLACK )
			{
				if( g_pRoh->m_Wearing[11] && !g_pRoh->m_Wearing[11]->m_Special )
				{
					effect_sort.vSortPos.z += (float)dwElapsedTick;
					if( (DWORD)effect_sort.vSortPos.z >= 60000 )
					{
						sprintf( strTemp, "use_effect %d 1\n", effect_sort.subNum );

						if (g_pTcpIp)
							g_pTcpIp->SendNetMessage(strTemp);

						effect_sort.vSortPos.z = 0.0f;
					}
				}
			}

			switch( effect_sort.num )
			{
			case VALO_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					if( (rand()%1000) < 10 )
					{
						effect_sort.num = VALO_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}
					else if( (rand()%1000) < 50 )
					{
						effect_sort.num = VALO_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case VALO_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					{
						if( (int)effect_sort.vSortPos.y >= m_pBpdValo[FX_VALO_ANI_STAND2]->m_AnimCount ) // 종료 처리
						{
							effect_sort.num = VALO_STATE_STAND;
							effect_sort.vSortPos.y = 0;
						}
					}
				}
				break;
			case VALO_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
											   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = VALO_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdValo[FX_VALO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case VALO_STATE_FOLLOW:  // 주인을 쫒아가는 상태.
				{
					if( MasterDist < 10.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = 0;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 20.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
					{
						vMasterPosDir *= -15.0f;
						effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
						effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

						// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
						effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

						if( (int)effect_sort.vSortPos.y >= m_pBpdValo[FX_VALO_ANI_MOVE]->m_AnimCount ) // 루프 처리
						{
							effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
						}
					}
					else
					{
						effect_sort.vPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
						if( effect_sort.vPos.y > 2.5f ) // 일정이상 증가 못하게.
							effect_sort.vPos.y = 2.5f;

						speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
						if( speed > MasterDist+5.0f )
							speed = MasterDist+5.0f;

						vMasterPosDir *= speed;

						effect_sort.vCurPos.x += vMasterPosDir.x;
						effect_sort.vCurPos.z += vMasterPosDir.z;

						effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.03f; // 프레임율을 거리에 반비례해서 조정

						if( (int)effect_sort.vSortPos.y >= m_pBpdValo[FX_VALO_ANI_MOVE]->m_AnimCount ) // 루프 처리
						{
							effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
						}
					}
				}
				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
			{
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
			}
		}
		break;


	case FX_FOLLOW_TYPE_ZORO:

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

		if( effect_sort.num != ZORO_STATE_FOLLOW && MasterDist > 30.0f )
		{
			effect_sort.num = ZORO_STATE_FOLLOW;
			effect_sort.vSortPos.y = 0.0f;
			effect_sort.vPos.y = 0.0f;
		}

		dwElapsedTick = timeGetTime() - effect_sort.dwColor;
		effect_sort.dwColor = timeGetTime();


		if( effect_sort.pNkChaFrom == g_pRoh && !g_pRoh->IsDead() && effect_sort.subNum == 10053 )
		{
			if( g_pRoh->m_Wearing[11] && !g_pRoh->m_Wearing[11]->m_Special )
			{
				effect_sort.vSortPos.z += (float)dwElapsedTick;
				if( (DWORD)effect_sort.vSortPos.z >= 60000 )
				{
					sprintf( strTemp, "use_effect %d 1\n", effect_sort.subNum );

					if (g_pTcpIp)
						g_pTcpIp->SendNetMessage(strTemp);

					effect_sort.vSortPos.z = 0.0f;
				}
			}
		}

		switch( effect_sort.num )
		{
		case ZORO_STATE_STAND:
			{
				effect_sort.vSortPos.y = 0;

				if( (rand()%1000) < 10 )
				{
					effect_sort.num = ZORO_STATE_SOCIAL1;
					effect_sort.vSortPos.y = 0;
//							effect_sort.subNum = 0;
				}
				else if( (rand()%1000) < 50 )
				{
					effect_sort.num = ZORO_STATE_RANDOMMOVE;
					effect_sort.vSortPos.y = 0;
//							effect_sort.subNum = 0;

					effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
					effect_sort.vTargetPos.y = 0.0f;
					effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
				}

			}
			break;

		case ZORO_STATE_SOCIAL1:
			{
				if( (int)effect_sort.vSortPos.y < 23 )
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
				else
					effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

				if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_ZORO_ANI_STAND2]->m_AnimCount ) // 종료 처리
				{
					effect_sort.num = ZORO_STATE_STAND;
					effect_sort.vSortPos.y = 0;
				}
			}
			break;
		case ZORO_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

			{
				MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
										   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

				if( MasterDist < 5.0f )
				{
					// 상태를 스탠드로 돌려준다.
					effect_sort.num = ZORO_STATE_STAND;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

					break;
				}

				// 프레임 갱신
				effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

				if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_ZORO_ANI_MOVE]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}

				// 이동
				vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


				speed = 0.008f * dwElapsedTick; // 속도 고정.

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;
			}

			break;

		case ZORO_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

			{
				if( MasterDist < 10.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
				{
					effect_sort.num = 0;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					break;
				}

				vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

				if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 20.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
				{
					vMasterPosDir *= -15.0f;
					effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
					effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

					// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
					effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_ZORO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}
				else
				{
					effect_sort.vPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
					if( effect_sort.vPos.y > 2.5f ) // 일정이상 증가 못하게.
						effect_sort.vPos.y = 2.5f;

					speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
					if( speed > MasterDist+5.0f )
						speed = MasterDist+5.0f;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.03f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_ZORO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}
			}

			break;
		}

		if( g_bNewTerrain )
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
			effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

			if( effect_sort.vCurPos.y < fHeight )
				effect_sort.vCurPos.y = fHeight;
		}
		else
			effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);

		break;

	case FX_FOLLOW_TYPE_RUDOLPH: ///펫루돌프

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 마스터와의 거리(y는 제외하고 체크.)

		if( effect_sort.num != FX_RUDOLPH_STATE_FOLLOW && MasterDist > 30.0f ) // 마스터랑 일정거리 이상 멀어지면 쫒아가도록.
		{
			effect_sort.num = FX_RUDOLPH_STATE_FOLLOW;
			effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
			effect_sort.vPos.y = 0.0f; // 가속도 초기화.
		}

		// 이전 프레임에서의 시간에서 진행해야할 프레임을 구한다.(돼지는 속도 세세하게 제어하려고 nAddFrame을 안쓴다.프레임 제어만 안쓰지 이동들은 nAddFrame을 씀에 유의. )
		dwElapsedTick = timeGetTime() - effect_sort.dwColor; // 이전 프레임에서 진행된 틱
		effect_sort.dwColor = timeGetTime(); // 갱신 시켜둔다.


		switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
		{
		case FX_RUDOLPH_STATE_STAND:  // 그냥 서있는 상태.

			// 프레임 갱신
			/*			effect_sort.vSortPos.y += dwElapsedTick * 0.065f;

						if( (int)effect_sort.vSortPos.y >= m_pBpdRudolph[FX_RUDOLPH_ANI_STAND]->m_AnimCount ) // 루프 처리
						{
							effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
						}

						if( (rand()%1000) < 250 )
			*/
			{
				effect_sort.vSortPos.y = 0; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)
			}

			// 랜덤하게 스테이트 변경
			if( (rand()%1000) < 10 )
			{
				effect_sort.num = FX_RUDOLPH_STATE_SOCIAL1;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				effect_sort.subNum = 0; // 끝내기 체크용.
			}
			else if( (rand()%1000) < 20 )
			{
				effect_sort.num = FX_RUDOLPH_STATE_SOCIAL2;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				effect_sort.vPos.y = 0.0f; // 가속도 초기화.

				// 목표지점을 주인을 기준으로 정한다.
				effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
				effect_sort.vTargetPos.y = 0.0f;
				effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );

			}
			else if( (rand()%1000) < 50 )
			{
				effect_sort.num = FX_RUDOLPH_STATE_RANDOMWALK;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				effect_sort.vPos.y = 0.0f; // 가속도 초기화.

				// 목표지점을 주인을 기준으로 정한다.
				effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
				effect_sort.vTargetPos.y = 0.0f;
				effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
			}

			break;

		case FX_RUDOLPH_STATE_RANDOMWALK:  // 아무 방향으로나 걸어감.(천천히)

			// 목표점에 일정 거리 이상 다가오면 상태 전환.
			MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
									   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

			if( MasterDist < 5.0f )
			{
				// 상태를 스탠드로 돌려준다.
				effect_sort.num = FX_RUDOLPH_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

				break;
			}


			// 프레임 갱신
			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			if( (int)effect_sort.vSortPos.y >= m_pBpdRudolph[FX_RUDOLPH_ANI_WALK]->m_AnimCount ) // 루프 처리
			{
				effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
			}


			// 이동
			vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
			effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


			speed = 0.008f * dwElapsedTick; // 속도 고정.

			vMasterPosDir *= speed;

			effect_sort.vCurPos.x += vMasterPosDir.x;
			effect_sort.vCurPos.z += vMasterPosDir.z;

			break;

		case FX_RUDOLPH_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

			// 상태변화 체크
			if( MasterDist < 10.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
			{
				effect_sort.num = FX_RUDOLPH_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

				break;
			}

			vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			// 돼지의 바라보는 방향을 주인쪽으로... vSortPos.x 를 방향값으로 사용.
			effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

			if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 20.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
			{
				// 여기서 계속 따라가려고 하면 움직임이 튀는 버그가 있어 그냥 임시로 붙여버리도록 땜빵.

				vMasterPosDir *= -15.0f;

				effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
				effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

				// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
				effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

				if( (int)effect_sort.vSortPos.y >= m_pBpdRudolph[FX_RUDOLPH_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}
			}
			else
			{
				effect_sort.vPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
				if( effect_sort.vPos.y > 2.5f ) // 일정이상 증가 못하게.
					effect_sort.vPos.y = 2.5f;

				speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
				if( speed > MasterDist+5.0f )
					speed = MasterDist+5.0f;

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;

				// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
				effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.03f; // 프레임율을 거리에 반비례해서 조정

				if( (int)effect_sort.vSortPos.y >= m_pBpdRudolph[FX_RUDOLPH_ANI_WALK]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}
			}

			break;

		case FX_RUDOLPH_STATE_SOCIAL1:  // 서서 애교동작

			if( (int)effect_sort.vSortPos.y < 23 )
				effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)
			else
				effect_sort.vSortPos.y += dwElapsedTick * 0.015f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)


			if( effect_sort.subNum < 3 ) // 다리떨기 반복용.
			{
				if( (int)effect_sort.vSortPos.y >= m_pBpdRudolph[FX_RUDOLPH_ANI_SOCIAL1]->m_AnimCount-7 ) // 루프 처리
				{
//					// 이 녀석은 모션이 한번 끝나면 돌려준다.
					effect_sort.vSortPos.y = 24; // 바뀔때 항상 프레임 초기화.

					++effect_sort.subNum;
				}
			}
			else
			{
				if( (int)effect_sort.vSortPos.y >= m_pBpdRudolph[FX_RUDOLPH_ANI_SOCIAL1]->m_AnimCount ) // 종료 처리
				{
					effect_sort.num = FX_RUDOLPH_STATE_STAND;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
				}
			}

			break;

		case FX_RUDOLPH_STATE_SOCIAL2:	 // 클라이언트에서 주변 돌리는 춤

			effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

			vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
			vMasterPosDir = Normalize( vMasterPosDir );

			// 돼지의 바라보는 방향을 주인쪽으로... vSortPos.x 를 방향값으로 사용.
			effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

			if( (int)effect_sort.vSortPos.y >= m_pBpdRudolph[FX_RUDOLPH_ANI_SOCIAL1]->m_AnimCount ) // 종료 처리
			{
				effect_sort.num = FX_RUDOLPH_STATE_STAND;
				effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
			}

			break;
		}

		if( g_bNewTerrain ) // 워크 플레인도 올라갈수있도록.
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
			effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

			if( effect_sort.vCurPos.y < fHeight )
				effect_sort.vCurPos.y = fHeight;
		}
		else
			effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);

		break;
	// [12/4/2007 반재승] 일본 링아이템 이펙트

	case FX_FOLLOW_TYPE_BEAR:

		if( !effect_sort.pNkChaFrom )
			return;

		vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
		vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
		vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

		MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
								   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

		if( effect_sort.num != BEAR_STATE_FOLLOW && MasterDist > 30.0f )
		{
			effect_sort.num = BEAR_STATE_FOLLOW;
			effect_sort.vSortPos.y = 0.0f;
			effect_sort.vPos.y = 0.0f;
		}

		dwElapsedTick = timeGetTime() - effect_sort.dwColor;
		effect_sort.dwColor = timeGetTime();

		switch( effect_sort.num )
		{
		case BEAR_STATE_STAND:
			{
				effect_sort.vSortPos.y = 0;

				if( (rand()%1000) < 10 )
				{
					effect_sort.num = BEAR_STATE_IDLE;
					effect_sort.vSortPos.y = 0;
//							effect_sort.subNum = 0;
				}
				else if( (rand()%1000) < 50 )
				{
					effect_sort.num = BEAR_STATE_RANDOMMOVE;
					effect_sort.vSortPos.y = 0;
//							effect_sort.subNum = 0;

					effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
					effect_sort.vTargetPos.y = 0.0f;
					effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
				}

			}
			break;

		case BEAR_STATE_IDLE:
			{
				if( (int)effect_sort.vSortPos.y < 23 )
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
				else
					effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

				if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_BEAR_ANI_IDLE]->m_AnimCount ) // 종료 처리
				{
					effect_sort.num = BEAR_STATE_STAND;
					effect_sort.vSortPos.y = 0;
				}
			}
			break;
		case BEAR_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

			{
				MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
										   , D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

				if( MasterDist < 5.0f )
				{
					// 상태를 스탠드로 돌려준다.
					effect_sort.num = BEAR_STATE_STAND;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

					break;
				}

				// 프레임 갱신
				effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

				if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_BEAR_ANI_MOVE]->m_AnimCount ) // 루프 처리
				{
					effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
				}

				// 이동
				vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


				speed = 0.008f * dwElapsedTick; // 속도 고정.

				vMasterPosDir *= speed;

				effect_sort.vCurPos.x += vMasterPosDir.x;
				effect_sort.vCurPos.z += vMasterPosDir.z;
			}

			break;

		case BEAR_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

			{
				if( MasterDist < 10.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
				{
					effect_sort.num = 0;
					effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
					break;
				}

				vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
				vMasterPosDir = Normalize( vMasterPosDir );

				effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

				if( effect_sort.pNkChaFrom->GetMotionState() == CHA_RUN && MasterDist < 20.0f ) // 쫒아가는데 계속 달리고 있고 일정거리 이하면...
				{
					vMasterPosDir *= -15.0f;
					effect_sort.vCurPos.x = vMasterPos.x + vMasterPosDir.x;
					effect_sort.vCurPos.z = vMasterPos.z + vMasterPosDir.z;

					// 프레임 증가.(이 녀석은 거리에 반비례해서 프레임율 조정)
					effect_sort.vSortPos.y += dwElapsedTick * ( 1.0f / MasterDist ) * 0.7f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_BEAR_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}
				else
				{
					effect_sort.vPos.y += 0.05f * (float)(nAddFrame); // 가속도를 증가. 등가속도
					if( effect_sort.vPos.y > 2.5f ) // 일정이상 증가 못하게.
						effect_sort.vPos.y = 2.5f;

					speed = effect_sort.vPos.y * (MasterDist*0.08f) * (float)(nAddFrame); // vSortPos.y 를 속도 값으로 사용.
					if( speed > MasterDist+5.0f )
						speed = MasterDist+5.0f;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					effect_sort.vSortPos.y += dwElapsedTick * effect_sort.vPos.y * 0.03f; // 프레임율을 거리에 반비례해서 조정

					if( (int)effect_sort.vSortPos.y >= m_pBpdZoro[FX_BEAR_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
				}
			}

			break;
		}

		if( g_bNewTerrain )
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
			effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

			if( effect_sort.vCurPos.y < fHeight )
				effect_sort.vCurPos.y = fHeight;
		}
		else
			effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);

		break;

	case FX_FOLLOW_TYPE_HORSE:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != HORSE_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = HORSE_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case HORSE_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					if( (rand()%1000) < 10 )
					{
						effect_sort.num = HORSE_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}
					else if( (rand()%1000) < 50 )
					{
						effect_sort.num = HORSE_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case HORSE_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdHorse[FX_HORSE_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = HORSE_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			case HORSE_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = HORSE_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdHorse[FX_HORSE_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case HORSE_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = HORSE_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.
					
					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.025f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdHorse[FX_HORSE_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}
					
				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;

	case FX_FOLLOW_TYPE_JEGUAR:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != JEGUAR_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = JEGUAR_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case JEGUAR_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 10 )
					{
						effect_sort.num = JEGUAR_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}
					else if( seed < 30 )
					{
						effect_sort.num = JEGUAR_STATE_SOCIAL2;
						effect_sort.vSortPos.y = 0;
					}
					else if( seed < 50 )
					{
						effect_sort.num = JEGUAR_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case JEGUAR_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdJeguar[FX_JEGUAR_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = JEGUAR_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			case JEGUAR_STATE_SOCIAL2:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdJeguar[FX_JEGUAR_ANI_STAND]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = JEGUAR_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			case JEGUAR_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = JEGUAR_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdJeguar[FX_JEGUAR_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case JEGUAR_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = JEGUAR_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdJeguar[FX_JEGUAR_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;

	case FX_FOLLOW_TYPE_BONI:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != BONI_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = BONI_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case BONI_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = BONI_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = BONI_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case BONI_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdJeguar[FX_BONI_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = BONI_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case BONI_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = BONI_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdJeguar[FX_BONI_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case BONI_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = BONI_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdJeguar[FX_BONI_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;

		case FX_FOLLOW_TYPE_AZUL:		//PABLO TEST PET ######################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != AZUL_STATE_FOLLOW && MasterDist > 80.0f )
			{
				effect_sort.num = AZUL_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case AZUL_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = AZUL_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = AZUL_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case AZUL_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdAZUL[FX_AZUL_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = AZUL_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case AZUL_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = AZUL_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdAZUL[FX_AZUL_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case AZUL_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = AZUL_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdAZUL[FX_AZUL_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_VERDE:		//PABLO TEST PET  
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != VERDE_STATE_FOLLOW && MasterDist > 80.0f )
			{
				effect_sort.num = VERDE_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case VERDE_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 50 ) //25
					{
						effect_sort.num = VERDE_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 100 ) //50
					{
						effect_sort.num = VERDE_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case VERDE_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 ) //23
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f; //0.02
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f; //0.015

					if( (int)effect_sort.vSortPos.y >= m_pBpdVERDE[FX_VERDE_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = VERDE_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case VERDE_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f ) //5.0
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = VERDE_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdVERDE[FX_VERDE_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정. // 0.008

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case VERDE_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.      //23
					{
						effect_sort.num = VERDE_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdVERDE[FX_VERDE_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END  ##################################################################################################################

		case FX_FOLLOW_TYPE_ROXO:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != ROXO_STATE_FOLLOW && MasterDist > 80.0f )
			{
				effect_sort.num = ROXO_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case ROXO_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = ROXO_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = ROXO_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case ROXO_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdROXO[FX_ROXO_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = ROXO_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case ROXO_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = ROXO_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdROXO[FX_ROXO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case ROXO_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = ROXO_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdROXO[FX_ROXO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_AMARELO:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != AMARELO_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = AMARELO_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case AMARELO_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = AMARELO_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = AMARELO_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case AMARELO_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdAMARELO[FX_AMARELO_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = AMARELO_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case AMARELO_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = AMARELO_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdAMARELO[FX_AMARELO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case AMARELO_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = AMARELO_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdAMARELO[FX_AMARELO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_VERMELHO:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != VERMELHO_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = VERMELHO_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case VERMELHO_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = VERMELHO_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = VERMELHO_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -30.0f, 30.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -30.0f, 30.0f );
					}

				}
				break;

			case VERMELHO_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 23 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.02f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.015f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdVERMELHO[FX_VERMELHO_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = VERMELHO_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case VERMELHO_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = VERMELHO_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.02f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdVERMELHO[FX_VERMELHO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case VERMELHO_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = VERMELHO_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdVERMELHO[FX_VERMELHO_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_CINZA:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != CINZA_STATE_FOLLOW && MasterDist > 60.0f )
			{
				effect_sort.num = CINZA_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case CINZA_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = CINZA_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = CINZA_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );  //30
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );  //30
					}

				}
				break;

			case CINZA_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f; //0.2
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f; //0.015

					if( (int)effect_sort.vSortPos.y >= m_pBpdCINZA[FX_CINZA_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = CINZA_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case CINZA_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f ) //5.0
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = CINZA_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신							
					effect_sort.vSortPos.y += dwElapsedTick * 0.08f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdCINZA[FX_CINZA_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.040f * dwElapsedTick; // 속도 고정.  //0.008

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case CINZA_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = CINZA_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 60.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdCINZA[FX_CINZA_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_FESHIK:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != FESHIK_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = FESHIK_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case FESHIK_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = FESHIK_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = FESHIK_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case FESHIK_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdFESHIK[FX_FESHIK_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = FESHIK_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case FESHIK_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = FESHIK_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdFESHIK[FX_FESHIK_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.020f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case FESHIK_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = FESHIK_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdFESHIK[FX_FESHIK_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_EZULAN:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != EZULAN_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = EZULAN_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case EZULAN_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = EZULAN_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = EZULAN_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case EZULAN_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.03f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.020f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdEZULAN[FX_EZULAN_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = EZULAN_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case EZULAN_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = EZULAN_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.03f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdEZULAN[FX_EZULAN_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.010f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case EZULAN_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = EZULAN_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdEZULAN[FX_EZULAN_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break; 

		case FX_FOLLOW_TYPE_THUNDERBIRD:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != THUNDERBIRD_STATE_FOLLOW && MasterDist > 60.0f )
			{
				effect_sort.num = THUNDERBIRD_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case THUNDERBIRD_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = THUNDERBIRD_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = THUNDERBIRD_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case THUNDERBIRD_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = THUNDERBIRD_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case THUNDERBIRD_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = THUNDERBIRD_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.08f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.040f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case THUNDERBIRD_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = THUNDERBIRD_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 60.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break; 

		case FX_FOLLOW_TYPE_THUNDERBIRD_P:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != THUNDERBIRD_P_STATE_FOLLOW && MasterDist > 60.0f )
			{
				effect_sort.num = THUNDERBIRD_P_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case THUNDERBIRD_P_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = THUNDERBIRD_P_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = THUNDERBIRD_P_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case THUNDERBIRD_P_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = THUNDERBIRD_P_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case THUNDERBIRD_P_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = THUNDERBIRD_P_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.08f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.040f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case THUNDERBIRD_P_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = THUNDERBIRD_P_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break; 

		case FX_FOLLOW_TYPE_PETFAME1:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETFAME1_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = PETFAME1_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETFAME1_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETFAME1_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETFAME1_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETFAME1_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME1[FX_PETFAME1_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETFAME1_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETFAME1_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETFAME1_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME1[FX_PETFAME1_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETFAME1_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETFAME1_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME1[FX_PETFAME1_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_PETFAME2:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETFAME2_STATE_FOLLOW && MasterDist > 50.0f )
			{
				effect_sort.num = PETFAME2_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETFAME2_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETFAME2_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETFAME2_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETFAME2_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME2[FX_PETFAME2_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETFAME2_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETFAME2_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETFAME2_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME2[FX_PETFAME2_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETFAME2_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 35.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETFAME2_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME2[FX_PETFAME2_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################


		case FX_FOLLOW_TYPE_PETFAME3:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETFAME3_STATE_FOLLOW && MasterDist > 30.0f )
			{
				effect_sort.num = PETFAME3_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETFAME3_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETFAME3_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETFAME3_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETFAME3_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME3[FX_PETFAME3_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETFAME3_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETFAME3_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETFAME3_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME3[FX_PETFAME3_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETFAME3_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 23.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETFAME3_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME3[FX_PETFAME3_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_PETFAME4:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETFAME4_STATE_FOLLOW && MasterDist > 50.0f )
			{
				effect_sort.num = PETFAME4_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETFAME4_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETFAME4_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETFAME4_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETFAME4_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME4[FX_PETFAME4_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETFAME4_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETFAME4_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETFAME4_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME4[FX_PETFAME4_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETFAME4_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 35.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETFAME4_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETFAME4[FX_PETFAME4_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################


		case FX_FOLLOW_TYPE_PETHELPER:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETHELPER_STATE_FOLLOW && MasterDist > 50.0f )
			{
				effect_sort.num = PETHELPER_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETHELPER_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETHELPER_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETHELPER_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETHELPER_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHELPER[FX_PETHELPER_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETHELPER_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETHELPER_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETHELPER_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHELPER[FX_PETHELPER_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETHELPER_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 35.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETHELPER_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHELPER[FX_PETHELPER_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################


		case FX_FOLLOW_TYPE_PETHALL1:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETHALL1_STATE_FOLLOW && MasterDist > 25.0f )
			{
				effect_sort.num = PETHALL1_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETHALL1_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETHALL1_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETHALL1_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETHALL1_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL1[FX_PETHALL1_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETHALL1_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETHALL1_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETHALL1_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL1[FX_PETHALL1_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETHALL1_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 15.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETHALL1_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL1[FX_PETHALL1_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################


		case FX_FOLLOW_TYPE_PETHALL2:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETHALL2_STATE_FOLLOW && MasterDist > 25.0f )
			{
				effect_sort.num = PETHALL2_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETHALL2_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETHALL2_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETHALL2_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETHALL2_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL2[FX_PETHALL2_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETHALL2_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETHALL2_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETHALL2_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL2[FX_PETHALL2_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETHALL2_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 15.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETHALL2_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL2[FX_PETHALL2_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################


		case FX_FOLLOW_TYPE_PETHALL3:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETHALL3_STATE_FOLLOW && MasterDist > 25.0f )
			{
				effect_sort.num = PETHALL3_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETHALL3_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETHALL3_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETHALL3_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETHALL3_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL3[FX_PETHALL3_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETHALL3_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETHALL3_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETHALL3_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL3[FX_PETHALL3_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETHALL3_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 15.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETHALL3_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETHALL3[FX_PETHALL3_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################


		case FX_FOLLOW_TYPE_PETNATAL1:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETNATAL1_STATE_FOLLOW && MasterDist > 25.0f )
			{
				effect_sort.num = PETNATAL1_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETNATAL1_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETNATAL1_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETNATAL1_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETNATAL1_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL1[FX_PETNATAL1_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETNATAL1_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETNATAL1_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETNATAL1_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL1[FX_PETNATAL1_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETNATAL1_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 15.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETNATAL1_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL1[FX_PETNATAL1_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################


		case FX_FOLLOW_TYPE_PETNATAL2:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETNATAL2_STATE_FOLLOW && MasterDist > 25.0f )
			{
				effect_sort.num = PETNATAL2_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETNATAL2_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETNATAL2_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETNATAL2_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETNATAL2_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL2[FX_PETNATAL2_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETNATAL2_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETNATAL2_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETNATAL2_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL2[FX_PETNATAL2_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETNATAL2_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 15.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETNATAL2_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL2[FX_PETNATAL2_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################

		case FX_FOLLOW_TYPE_PETNATAL3:		//PABLO TEST PET
		{
			if( !effect_sort.pNkChaFrom )
				return;

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz;

			MasterDist = DistVecToVec( D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z)
				, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) );

			if( effect_sort.num != PETNATAL3_STATE_FOLLOW && MasterDist > 25.0f )
			{
				effect_sort.num = PETNATAL3_STATE_FOLLOW;
				effect_sort.vSortPos.y = 0.0f;
				effect_sort.vPos.y = 0.0f;
			}

			dwElapsedTick = timeGetTime() - effect_sort.dwColor;
			effect_sort.dwColor = timeGetTime();

			switch( effect_sort.num )
			{
			case PETNATAL3_STATE_STAND:
				{
					effect_sort.vSortPos.y = 0;

					int seed = rand()%1000;
					if( seed < 25 )
					{
						effect_sort.num = PETNATAL3_STATE_SOCIAL1;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;
					}					
					else if( seed < 50 )
					{
						effect_sort.num = PETNATAL3_STATE_RANDOMMOVE;
						effect_sort.vSortPos.y = 0;
						//							effect_sort.subNum = 0;

						effect_sort.vTargetPos.x = effect_sort.pNkChaFrom->m_wx + RandomNum( -120.0f, 120.0f );
						effect_sort.vTargetPos.y = 0.0f;
						effect_sort.vTargetPos.z = effect_sort.pNkChaFrom->m_wz + RandomNum( -120.0f, 120.0f );
					}

				}
				break;

			case PETNATAL3_STATE_SOCIAL1:
				{
					if( (int)effect_sort.vSortPos.y < 120 )
						effect_sort.vSortPos.y += dwElapsedTick * 0.04f;
					else
						effect_sort.vSortPos.y += dwElapsedTick * 0.030f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL3[FX_PETNATAL3_ANI_STAND2]->m_AnimCount ) // 종료 처리
					{
						effect_sort.num = PETNATAL3_STATE_STAND;
						effect_sort.vSortPos.y = 0;
					}
				}
				break;
			
			case PETNATAL3_STATE_RANDOMMOVE:  // 아무 방향으로나 걸어감.(천천히)

				{
					MasterDist = DistVecToVec( D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z)
						, D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z) ); // 이녀석은 마스터와의 거리가 아닌 목표지점까지의 거리.

					if( MasterDist < 5.0f )
					{
						// 상태를 스탠드로 돌려준다.
						effect_sort.num = PETNATAL3_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.

						break;
					}

					// 프레임 갱신
					effect_sort.vSortPos.y += dwElapsedTick * 0.04f; // 이걸 프레임 값으로 사용. (일반적으론 20프레임에 1초로 사용)

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL3[FX_PETNATAL3_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

					// 이동
					vMasterPosDir = D3DVECTOR(effect_sort.vTargetPos.x,0.0f,effect_sort.vTargetPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					// 돼지의 바라보는 방향을 목표쪽으로... vSortPos.x 를 방향값으로 사용.
					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.


					speed = 0.008f * dwElapsedTick; // 속도 고정.

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;
				}

				break;

			case PETNATAL3_STATE_FOLLOW:  // 주인을 쫒아가는 상태.

				{
					if( MasterDist < 15.0f ) // 마스터랑 일정거리 이상 가까워지면 그만 감.
					{
						effect_sort.num = PETNATAL3_STATE_STAND;
						effect_sort.vSortPos.y = 0; // 바뀔때 항상 프레임 초기화.
						break;
					}

					vMasterPosDir = D3DVECTOR(vMasterPos.x,0.0f,vMasterPos.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z); // 마스터로 향하는 벡터를 만들어두고.
					vMasterPosDir = Normalize( vMasterPosDir );

					effect_sort.vSortPos.x = (float) atan2(vMasterPosDir.z, vMasterPosDir.x); // 타이밍상 여기서 해야함에 유의.

					float fRatio = 0.05f;
					if( MasterDist < 28.0f )
						fRatio = 0.025f;
					else if( MasterDist > 45.0f )
						fRatio = 0.065f;

					speed = fRatio * dwElapsedTick;

					vMasterPosDir *= speed;

					effect_sort.vCurPos.x += vMasterPosDir.x;
					effect_sort.vCurPos.z += vMasterPosDir.z;

					if( MasterDist > 100.0f )
					{
						effect_sort.vCurPos.x = vMasterPos.x;
						effect_sort.vCurPos.z = vMasterPos.z;
					}

					effect_sort.vSortPos.y += dwElapsedTick * 0.075f;

					if( (int)effect_sort.vSortPos.y >= m_pBpdPETNATAL3[FX_PETNATAL3_ANI_MOVE]->m_AnimCount ) // 루프 처리
					{
						effect_sort.vSortPos.y -= (int)effect_sort.vSortPos.y; // 소수점만 남긴다.
					}

				}

				break;
			}

			if( g_bNewTerrain )
			{
				float fHeight = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
				effect_sort.vCurPos.y = CNKObjQuadNode::GetHeightOnObject( effect_sort.vCurPos.z, effect_sort.vCurPos.x, effect_sort.vCurPos.y );

				if( effect_sort.vCurPos.y < fHeight )
					effect_sort.vCurPos.y = fHeight;
			}
			else
				effect_sort.vCurPos.y = pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x, effect_sort.vCurPos.z);
		}
		break;   
		//END ##################################################################################################################



	case FX_FOLLOW_TYPE_RING_LIGHT:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			int actNum;

			switch( effect_sort.pNkChaFrom->m_Race  ) // 종족별로 바이패드 차이가 난다.
			{
			case 0:
			case 1:
			case 3:
			case 4:
				actNum = 24;
				break;

			case 2:
				actNum = 22;
				break;
			case 5:
				{
					if( effect_sort.pNkChaFrom->m_Sex )
						actNum = 21;
					else
						actNum = 24;

				}
				break;

			default:
				actNum = 24;
				break;
			}

			int ignoreActNum = 2;

			D3DMATRIX matTrans;
			D3DVECTOR vScr, vTemp;

			if( effect_sort.pNkChaFrom->GetBiped()->m_AnimCount == 21 )  // 서있는 동작과 그외의 동작들을 구분해서 위치를 잡아줘야 함..
			{
				vTemp = D3DVECTOR( effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()/ignoreActNum]._41 + 0.2f,
								   effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()/ignoreActNum]._42 - 0.5f,
								   effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()/ignoreActNum]._43 - 0.1f );
			}
			else
			{
				vTemp = D3DVECTOR( effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()]._41 + 0.2f,
								   effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()]._42 - 0.5f,
								   effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()]._43 - 0.1f );
			}

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetTranslateMatrix( matTrans, vTemp );
			Nk3DUtil_SetRotateCCWYMatrix( matTrans, effect_sort.pNkChaFrom->m_dir );
			D3DMath_VectorMatrixMultiply(vScr, vTemp, matTrans);

			vMasterPos.x = effect_sort.pNkChaFrom->m_wx + vScr.x;
			vMasterPos.y = effect_sort.pNkChaFrom->m_wy + vScr.y;
			vMasterPos.z = effect_sort.pNkChaFrom->m_wz + vScr.z;

			effect_sort.num = FX_RING_LIGHT_STATE_FOLLOW;
			effect_sort.vSortPos.y = 0.0f; // 프래임 초기화
			effect_sort.vPos.y = 0.0f; // 가속도 초기화.
			effect_sort.subNum = 0;

			switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.(상태값이다.)
			{
			case FX_RING_LIGHT_STATE_FOLLOW: // 주인을 쫒아가는 상태.
				{
					effect_sort.vCurPos = vMasterPos;

					// 궤적 갱신.
					if( nAddFrame > 0 ) // 프레임 증가가 있었을 때만 해주자.
					{
						list<D3DVECTOR> *TraceInfoList;
						TraceInfoList = (list<D3DVECTOR> *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.
						D3DVECTOR TempVertex;

						TempVertex = D3DVECTOR( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
						TraceInfoList->push_back( TempVertex ); // 점 입력.

						if( effect_sort.num == FX_RING_LIGHT_STATE_FOLLOW ) // 쫒아가는중엔 더 짧게
						{
							while( TraceInfoList->size() >= 7 ) // 이것보다 길면.
								TraceInfoList->pop_front(); // 빼준다.
						}

						if( TraceInfoList->size() >= MAX_RING_LIGHT_TRACE ) // 일정 갯수 이상 안늘어나게.
						{
							TraceInfoList->pop_front(); // 앞에 하나 빼준다.
						}
					}
				}

				break;
			}
		}

		break;
	}

}

void CFxFollow::Render( EffectSort &effect_sort ) // 왜곡된 평면을 드로우.(평면이 2D용 평면임에 유의. 만들당시부터 3D가 아니라 2D좌표다.)
{
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DVECTOR vViewAt = pCMyApp->GetEyeAtPos();
	D3DVECTOR vViewFrom;
	pCMyApp->GetEyePos( vViewFrom );
	float fViewDist = Magnitude( vViewFrom-vViewAt );

	DWORD dwLighting,dwAlphaBlend , dwSrc, dwDest, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	float rectwidth = 0.0f; // vSortPos를 사용안하고 있는것 같으니 그 x값을 크기값으로 사용한다.
	float FrameWidthRatio = 0.0f; // 한 프레임의 가로 비율
	float tu = 0.0f;
	float tv = 0.0f;
	float tv_plus = 0.0f; // 각 애니메이션 별 텍스쳐 좌표 보정치.
	int TotalFrame = 0; // 현재 애니메이션의 총 프레임을 받아오기 위한 변수
	int FrameWidth = 0; // 현재 애니메이션의 한 텍스쳐에서의 가로 프레임 갯수.

	int i = 0;

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

	CCapsyong *pRefDrawCsy = NULL;
	CBiped	  *pRefDrawBpd = NULL;

	switch( effect_sort.Index ) // effect_sort 의 Index를 타입으로 사용한다.
	{
	case FX_FOLLOW_TYPE_FAIRY: // 페어리 타입
		{

			if( !effect_sort.pNkChaFrom )
				return;

			// 주변 이펙트 출력
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

			// 깜빡이는 이펙트 출력.
			m_pEffect1->SetBillBoard( vViewVec, FALSE, 0.0f );
			m_pEffect1->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y+0.3f, effect_sort.vCurPos.z );
			/*      // Show 모션은 일단 보류.
			if( effect_sort.num == FX_FAIRY_STATE_SHOW1 ) // 구라 때리기 중엔 이펙트를 딴걸로 출력해준다.
			{
			rectwidth += RandomNum(1.25f,1.7f);

			int NowFrame = effect_sort.nCurFrame % 4; // 이펙트의 현재 프레임.
			TotalFrame = 4;
			FrameWidth = 2;
			FrameWidthRatio = (1.0f/(float)FrameWidth); // 한 프레임의 가로 비율
			tu = (float)(NowFrame % FrameWidth) * FrameWidthRatio;
			tv = (float)(NowFrame / FrameWidth) * FrameWidthRatio;

			if( m_pEffect1 )
			{
			m_pEffect1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu, tv+FrameWidthRatio )  );
			m_pEffect1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu, tv )  );
			m_pEffect1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv+FrameWidthRatio)  );
			m_pEffect1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv )  );
			}

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_ATTCK_EFFECT ] ) );
			//			m_pEffect1->Render( GET_D3DDEVICE() );
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_LIGHT ] ) ); // 파티클을 위해서..
			}
			else
			*/
			{
				// 깜빡이는 이펙트
				if( effect_sort.num == FX_FAIRY_STATE_BLOCKING && effect_sort.nCurFrame < 14 )
				{
					rectwidth += (float)(effect_sort.nCurFrame) * 1.8f;

					GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
				}
				else
				{
					rectwidth += RandomNum(1.25f,1.7f);
				}

				if( m_pEffect1 )
				{
					m_pEffect1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f),
												  effect_sort.dwColor,
												  0, 0.0f, 1.0f )  );
					m_pEffect1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f),
												  effect_sort.dwColor,
												  0, 0.0f, 0.0f )  );
					m_pEffect1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f),
												  effect_sort.dwColor,
												  0, 1.0f, 1.0f )  );
					m_pEffect1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f),
												  effect_sort.dwColor,
												  0, 1.0f, 0.0f )  );
				}

				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_LIGHT ] ) );
				m_pEffect1->Render();
			}

			// 파티클 출력.
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

			for( i = 0 ; i < MAX_PARTICLE_FOR_FXFOLLOW ; ++i )
			{
				if( fViewDist > 50.0f && fViewDist < 500.0f ) // 카메라와 일정거리 이상 떨어져있다면... 너무 멀어도 보정 안한다.
					rectwidth = m_pParticleInfoArray[i].scale * (fViewDist*0.02f) * 0.5f;
				else // 카메라와 가까울땐 그냥 출력
					rectwidth = m_pParticleInfoArray[i].scale * 0.5f;

				// by evilkiki 2010.07.01.페어리 떨어지는 파티클
				if( m_pEffect2 )
				{
					m_pEffect2->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f)
												  , D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].fade)
												  , 0, 0.0f, 1.0f )  );
					m_pEffect2->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f)
												  , D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].fade)
												  , 0, 0.0f, 0.0f )  );
					m_pEffect2->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f)
												  , D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].fade)
												  , 0, 1.0f, 1.0f )  );
					m_pEffect2->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f)
												  , D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].fade)
												  , 0, 1.0f, 0.0f )  );
				}

				m_pEffect2->SetBillBoard( vViewVec, FALSE, 0.0f );
				m_pEffect2->SetTranslationXYZ( m_pParticleInfoArray[i].cur_Pos.x
											   , m_pParticleInfoArray[i].cur_Pos.y, m_pParticleInfoArray[i].cur_Pos.z );

				m_pEffect2->Render();
			}

			tv_plus = 0.0f;

			// 애니메이션마다 텍스쳐가 바뀔수도 있다.(한 텍스쳐에 2개이상의 애니가 들어가 있을 수도 있다.)
			if( g_pCapsyongTexture )
			{
				switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
				{
				case FX_FAIRY_STATE_STAND: // 둥둥 떠있는 상태.
				case FX_FAIRY_STATE_SIDESTEP: // 좌우로 느리게 이동.
				case FX_FAIRY_STATE_TURNROUND: // 주인을 빙빙 도는 상태.
				case FX_FAIRY_STATE_FOLLOW: // 주인을 쫒아가는 상태.

					TotalFrame = FX_FAIRY_MOVE_FRAME_TOTAL;
					FrameWidth = FX_FAIRY_MOVE_FRAME_WIDTH;

					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_MOVE ] ) );

					if( effect_sort.num == FX_FAIRY_STATE_TURNROUND )
						tv_plus += 0.5f; // 가운데서부터 시작하는 애니메이션.

					break;

				case FX_FAIRY_STATE_BLOCKING: // 방어막치기.

					TotalFrame = FX_FAIRY_BLOCK_FRAME_TOTAL;
					FrameWidth = FX_FAIRY_BLOCK_FRAME_WIDTH;

					// 방어막 치기 모션중엔 방어막 롤로를 찍어준다.
					Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.y );
					D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos );
					D3DMath_MatrixMultiply( matRotate, matRotate, matTrans );
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matRotate );

					if( g_pFairyProtect ) // 끝 프레임에 맞춰서 시작시키기..
					{
						if( effect_sort.nCurFrame < EFF_FX_FAIRY_PROTECT_NUM )
							g_pFairyProtect->m_LoloObjs->m_TextureNum = g_EffectTextr[ EFF_FX_FAIRY_PROTECT1 + effect_sort.nCurFrame ];
						else
							g_pFairyProtect->m_LoloObjs->m_TextureNum = g_EffectTextr[ EFF_FX_FAIRY_PROTECT1 + EFF_FX_FAIRY_PROTECT_NUM-1 ];

						g_pFairyProtect->m_LoloObjs->m_bAlphablend = TRUE;

						BYTE dwAlpha = 0;

						if( effect_sort.nCurFrame < 12 ) // 투명도 증가 페이즈
						{
							dwAlpha = effect_sort.nCurFrame * 15;
						}
						else // 투명도 감소 페이즈.
						{
							dwAlpha = (TotalFrame-effect_sort.nCurFrame) * 15;
						}
						dwAlpha = 200;

						//					g_pFairyProtect->m_LoloObjs->m_TextureNum = g_EffectTextr[ EFF_FX_FAIRY_ACT1 ];

						g_pFairyProtect->Render( GET_D3DDEVICE(), 0, FALSE, FALSE, FALSE, NULL, NULL, FALSE, TRUE, (0x00ffffff|(dwAlpha<<28)) );
					}

					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_ACT1 ] ) );

					break;
					/*          // Show 모션은 일단 보류.
					case FX_FAIRY_STATE_SHOW1: // 구라 때리기 모션.

					TotalFrame = FX_FAIRY_SHOW1_FRAME_TOTAL;
					FrameWidth = FX_FAIRY_SHOW1_FRAME_WIDTH;

					if( effect_sort.vSortPos.z >= effect_sort.vPos.x ) // 이동한 거리가 목적치를 넘으면..
					{
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_MOVE ] ) );
					}
					else // 아직 목적지 까지 못간 상태.
					{
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_ACT1 ] ) );
					tv_plus += 0.5f; // 가운데서부터 시작하는 애니메이션.
					}

					break;
					*/
				}
			}

			// Follow의 타입마다 사각형 크기를 매번 조정해준다. 프레임 마다 텍스쳐 좌표도 조정해주자.
			rectwidth = effect_sort.vSortPos.x * 0.5f; // vSortPos를 사용안하고 있는것 같으니 그 x값을 크기값으로 사용한다.
			FrameWidthRatio = (1.0f/(float)FrameWidth); // 한 프레임의 가로 비율
			tu = (float)(effect_sort.nCurFrame % FrameWidth) * FrameWidthRatio;
			tv = (float)(effect_sort.nCurFrame / FrameWidth) * FrameWidthRatio;

			tv += tv_plus; // 각 애니메이션 별 텍스쳐 좌표 보정치.

			// 방향에 따라서 텍스쳐 좌표가 조절되야한다. (subNum을 방향값으로 사용.) 0은 왼쪽. 1은 오른쪽.
			if( effect_sort.subNum == -1 ) // 현재로선 0은 조정안해줘도된다.
			{
				tv += (TotalFrame/FrameWidth) * FrameWidthRatio; // 방향치만큼 세로 좌표를 증가.
			}

			if( m_pMainObject )
			{
				m_pMainObject->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu, tv+FrameWidthRatio )  );
				m_pMainObject->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu, tv )  );
				m_pMainObject->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv+FrameWidthRatio)  );
				m_pMainObject->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv )  );
			}

			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

			m_pMainObject->SetBillBoard( vViewVec, FALSE, 0.0f );
			m_pMainObject->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			m_pMainObject->Render();
		}

		break;

	case FX_FOLLOW_TYPE_LUCKYPIG: // 설날 복돼지
		{

			if( !effect_sort.pNkChaFrom )
				return;


			switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
			{
			case FX_LUCKYPIG_STATE_STAND:  // 그냥 서있는 상태.
				pRefDrawBpd = m_pBpdLuckyPig[FX_LUCKYPIG_ANI_STAND]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_LUCKYPIG_STATE_RANDOMWALK:  // 아무 방향으로나 걸어감.(천천히)
				pRefDrawBpd = m_pBpdLuckyPig[FX_LUCKYPIG_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_LUCKYPIG_STATE_FOLLOW:  // 주인을 쫒아가는 상태.
				pRefDrawBpd = m_pBpdLuckyPig[FX_LUCKYPIG_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_LUCKYPIG_STATE_SOCIAL1:  // 서서 애교동작
				pRefDrawBpd = m_pBpdLuckyPig[FX_LUCKYPIG_ANI_SOCIAL1]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_LUCKYPIG_STATE_SOCIAL2:	 // 클라이언트에서 주변 돌리는 춤
				pRefDrawBpd = m_pBpdLuckyPig[FX_LUCKYPIG_ANI_SOCIAL2]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_LUCKYPIG_STATE_HEAL:	 // 회복 모션
				pRefDrawBpd = m_pBpdLuckyPig[FX_LUCKYPIG_ANI_HEAL]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			}

			// 돼지 그림자 그리기.
			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			// 액션 스테이트로 애니메이션을 결정
			pRefDrawCsy = m_pCsyLuckyPig[effect_sort.subNum-SKILL_LUCKYPIG1]; // effect_sort의 subNum을 돼지 종류 번호로 사용.

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y ); // vSortPos.y 를 프레임 값으로 사용.

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			if( effect_sort.subNum == SKILL_LUCKYPIG1 ) // 황금 돼지는 약간 크게.
			{
				D3DUtil_SetScaleMatrix(matScale, 1.1f, 1.1f, 1.1f);
				D3DMath_MatrixMultiply( matWorld, matScale, matWorld );
			}


			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}

		break;

	case FX_FOLLOW_TYPE_BABYSOUL: // [7/31/2007 Theodoric] 6주년 기념 이벤트 베이비 소울
		{

			if( !effect_sort.pNkChaFrom || effect_sort.vTargetPos.y == (float)BS_TELE_HIDE)
				return;

			switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
			{
			case FX_BABYSOUL_STATE_STAND:  // 서있는 상태
			case FX_BABYSOUL_STATE_STAND_TO:
			case FX_BABYSOUL_STATE_STAND_TELE:
			case FX_BABYSOUL_STATE_STAND_TELE_TO:
				pRefDrawBpd = m_pBpdBabySoul[FX_BABYSOUL_ANI_STAND]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;

			case FX_BABYSOUL_STATE_STAND_UP:  // 서있는 상태 랜덤 : 날개짓 위로
			case FX_BABYSOUL_STATE_STAND_UP_TO:
				pRefDrawBpd = m_pBpdBabySoul[FX_BABYSOUL_ANI_STAND_UP]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;

			case FX_BABYSOUL_STATE_WALK:  // 걸어간다.
			case FX_BABYSOUL_STATE_WALK_TO:
				pRefDrawBpd = m_pBpdBabySoul[FX_BABYSOUL_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;

			case FX_BABYSOUL_STATE_CRITICAL:  // 크리티컬.
				pRefDrawBpd = m_pBpdBabySoul[FX_BABYSOUL_ANI_CRITICAL]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			}

			// 액션 스테이트로 애니메이션을 결정
			//		pRefDrawCsy = m_pCsyBabySoul[effect_sort.subNum-SKILL_BABYSOUL_1]; // effect_sort의 subNum을 종류 번호로 사용.

			//		pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y ); // vSortPos.y 를 프레임 값으로 사용.
			m_pCsyBabySoul->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y ); // vSortPos.y 를 프레임 값으로 사용.

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y+2.0f, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			if( effect_sort.subNum-SKILL_BABYSOUL_1 == 0
			  ) // 포치는 좀더 크게 해준다.
				D3DUtil_SetScaleMatrix(matScale, 0.9f - effect_sort.vTargetPos.x, 0.9f + effect_sort.vTargetPos.x*2.0f, 0.9f - effect_sort.vTargetPos.x);
			else
				D3DUtil_SetScaleMatrix(matScale, 0.8f - effect_sort.vTargetPos.x, 0.8f + effect_sort.vTargetPos.x*2.0f, 0.8 - effect_sort.vTargetPos.x);

			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			if( effect_sort.vTargetPos.y == (float)BS_TELE_FADEIN || effect_sort.vTargetPos.y == (float)BS_TELE_FADEOUT )
			{
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
			}
			else
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);

			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );



			//		pRefDrawCsy->Render( GET_D3DDEVICE() );
			if(effect_sort.subNum-SKILL_BABYSOUL_1 == 0
			  )
				m_pCsyBabySoul->Render( GET_D3DDEVICE() );
			else
			{
				if ( effect_sort.subNum-SKILL_BABYSOUL_2 == 0 || effect_sort.subNum-SKILL_BABYSOUL_3 == 0 )
				{
					m_pCsyBabySoul->Render( GET_D3DDEVICE(), m_BabySoulTextrIndex[effect_sort.subNum-SKILL_BABYSOUL_2] );
				}
			}
		}
		break;

	case FX_FOLLOW_TYPE_VAMPIREBAT: ///뱀파이어망토 박쥐

		{
			// 내부 변수 선언으로 인한 묶음.( ///펫루돌프 시 작업 )
			if( !effect_sort.pNkChaFrom )
				return;

			// 주변 이펙트 출력
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

			// 깜빡이는 이펙트 출력.
			m_pEffect1->SetBillBoard( vViewVec, FALSE, 0.0f );
			m_pEffect1->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );

			{
				// 깜빡이는 이펙트
				if( fViewDist > 50.0f && fViewDist < 500.0f ) // 카메라와 일정거리 이상 떨어져있다면... 너무 멀어도 보정 안한다.
					rectwidth += RandomNum(4.0f,7.0f);
				else
					rectwidth += RandomNum(2.0f,4.0f);

				if( m_pEffect1 )
				{
					DWORD color = D3DRGBA( 0.9f, 0.0f, 0.0f, /*RandomNum(0.5f,1.0f)*/0.5f );

					m_pEffect1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), color, 0, 0.0f, 1.0f )  );
					m_pEffect1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), color, 0, 0.0f, 0.0f )  );
					m_pEffect1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), color, 0, 1.0f, 1.0f )  );
					m_pEffect1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), color, 0, 1.0f, 0.0f )  );
				}

				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_LIGHT ] ) );
				m_pEffect1->Render();
			}

			list<D3DVECTOR> *TraceInfoList;
			TraceInfoList = (list<D3DVECTOR> *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

			D3DLVERTEX TempVertexArray[MAX_VAMPIREBAT_TRACE*2];
			D3DLVERTEX TempVertexArray2[MAX_VAMPIREBAT_TRACE*2];

			int k = 0;
			int l = 0;
			int j = TraceInfoList->size();
			int i = 0;
			float fLineWidth = 0.3f;

			if( fViewDist > 50.0f && fViewDist < 500.0f ) // 카메라와 일정거리 이상 떨어져있다면... 너무 멀어도 보정 안한다.
				fLineWidth = 0.5f;

			DWORD color = RGBA_MAKE(255,0,0,255);
			D3DVECTOR TempVertex;
			list<D3DVECTOR>::iterator pr;

			i = 0;

			for( pr = TraceInfoList->begin() ; pr != TraceInfoList->end() ; pr++ )
			{
				TempVertex = *pr;

				if( k < (MAX_VAMPIREBAT_TRACE*2)-2 )
				{
					// 리스트의 하나당 점 두개를 그려준다.
					TempVertexArray[k] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y-(float)((float)(k/2)/(float)j)*fLineWidth, TempVertex.z ), color, 0, (float)((float)(k/2)/(float)j), 0.0f );
					k++;

					TempVertexArray[k] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y+(float)((float)(k/2)/(float)j)*fLineWidth, TempVertex.z ), color, 0, (float)((float)(k/2)/(float)j), 1.0f );
					k++;
				}
			}

			i = 0;

			for( pr = TraceInfoList->begin() ; pr != TraceInfoList->end() ; pr++ )
			{
				TempVertex = *pr;

				if( l < (MAX_VAMPIREBAT_TRACE*2)-2 )
				{
					// 리스트의 하나당 점 두개를 그려준다.
					TempVertexArray2[l] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y, TempVertex.z-(float)((float)(l/2)/(float)j)*fLineWidth ), color, 0, (float)((float)(l/2)/(float)j), 0.0f );
					l++;
					TempVertexArray2[l] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y, TempVertex.z+(float)((float)(l/2)/(float)j)*fLineWidth ), color, 0, (float)((float)(l/2)/(float)j), 1.0f );
					l++;
				}
			}

			D3DMATRIX matWorld;
			D3DUtil_SetIdentityMatrix(matWorld);
			matWorld._41 = 0.0f;
			matWorld._42 = 0.0f;
			matWorld._43 = 0.0f;

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
			//		GET_D3DDEVICE()->SetTexture(0,g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WEAPON_TRACE2 ] ) );
			GET_D3DDEVICE()->SetTexture(0,NULL );
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &TempVertexArray, k, 0 );
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &TempVertexArray2, l, 0 );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);


			tv_plus = 0.0f;

			// 애니메이션마다 텍스쳐가 바뀔수도 있다.(한 텍스쳐에 2개이상의 애니가 들어가 있을 수도 있다.)
			if( g_pCapsyongTexture )
			{
				switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
				{
				case FX_VAPIREBAT_STATE_STAND: // 그냥 서있는 상태. 파닥파닥, 좀 흔들린다.

					TotalFrame = FX_VAPIREBAT_STAND_FRAME_TOTAL;
					FrameWidth = FX_VAPIREBAT_STAND_FRAME_WIDTH;

					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_VAMPIREBAT_STAND ] ) );

					break;

				case FX_VAPIREBAT_STATE_TURNROUND: // 주인을 빙빙 도는 상태.
				case FX_VAPIREBAT_STATE_FOLLOW: // 주인을 쫒아가는 상태.

					TotalFrame = FX_VAPIREBAT_MOVE_FRAME_TOTAL;
					FrameWidth = FX_VAPIREBAT_MOVE_FRAME_WIDTH;

					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_VAMPIREBAT_MOVE ] ) );

					//				if( effect_sort.num == FX_VAPIREBAT_STATE_TURNROUND )
					//					tv_plus += 0.5f; // 가운데서부터 시작하는 애니메이션.

					break;
				}
			}

			// Follow의 타입마다 사각형 크기를 매번 조정해준다. 프레임 마다 텍스쳐 좌표도 조정해주자.
			rectwidth = effect_sort.vSortPos.x * 0.5f; // vSortPos를 사용안하고 있는것 같으니 그 x값을 크기값으로 사용한다.
			FrameWidthRatio = (1.0f/(float)FrameWidth); // 한 프레임의 가로 비율
			tu = (float)(effect_sort.nCurFrame % FrameWidth) * FrameWidthRatio;
			tv = (float)(effect_sort.nCurFrame / FrameWidth) * FrameWidthRatio;

			tv += tv_plus; // 각 애니메이션 별 텍스쳐 좌표 보정치.

			// 방향에 따라서 텍스쳐 좌표가 조절되야한다. (subNum을 방향값으로 사용.) 0은 왼쪽. 1은 오른쪽.
			if( m_pMainObject )
			{
				if( effect_sort.num == FX_VAPIREBAT_STATE_STAND || effect_sort.subNum == 1 ) // 현재로선 0은 조정안해줘도된다.
				{
					m_pMainObject->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu, tv+FrameWidthRatio )  );
					m_pMainObject->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu, tv )  );
					m_pMainObject->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv+FrameWidthRatio)  );
					m_pMainObject->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv )  );
				}
				else
				{
					m_pMainObject->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv+FrameWidthRatio )  );
					m_pMainObject->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu+FrameWidthRatio, tv )  );
					m_pMainObject->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, tu, tv+FrameWidthRatio)  );
					m_pMainObject->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, tu, tv )  );
				}

			}


			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

			m_pMainObject->SetBillBoard( vViewVec, FALSE, 0.0f );
			m_pMainObject->RotateZ( effect_sort.vSortPos.z );
			m_pMainObject->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			m_pMainObject->Render();
		}
		break;

	case FX_FOLLOW_TYPE_ARUPAKAPET:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
			{
			case FX_ARUPAKA_ANI_STAND:
				pRefDrawBpd = m_pBpdArupaka[FX_ARUPAKA_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_ARUPAKA_ANI_WALK:
				pRefDrawBpd = m_pBpdArupaka[FX_ARUPAKA_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_ARUPAKA_ANI_DOWN:
				pRefDrawBpd = m_pBpdArupaka[FX_ARUPAKA_ANI_DOWN]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_ARUPAKA_ANI_SPIT:
				pRefDrawBpd = m_pBpdArupaka[FX_ARUPAKA_ANI_SPIT]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_ARUPAKA_ANI_RANDOMWALK:
				pRefDrawBpd = m_pBpdArupaka[FX_ARUPAKA_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			}

			pRefDrawCsy = m_pCsyArupaka[effect_sort.nTargetType];										// effect_sort의 subNum을 돼지 종류 번호로 사용.

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y );	// vSortPos.y 를 프레임 값으로 사용.

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			D3DUtil_SetScaleMatrix(matScale, 0.7f, 0.7f, 0.7f); // 약간 크기를 줄인다.5
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );

			SpitRender(effect_sort);
		}
		break;

	case FX_FOLLOW_TYPE_CATS:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case CAT_STATE__STAND:
				pRefDrawBpd = m_pBpdCats[FX_CATS_ANI_STAND];
				break;
			case CAT_STATE__SOCIAL1:
				pRefDrawBpd = m_pBpdCats[FX_CATS_ANI_STAND2];
				break;
			case CAT_STATE__RANDOMMOVE:
				pRefDrawBpd = m_pBpdCats[FX_CATS_ANI_WALK];
				break;
			case CAT_STATE__FOLLOW:
				pRefDrawBpd = m_pBpdCats[FX_CATS_ANI_WALK];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(NULL, FALSE);

			pRefDrawCsy = m_pCsyCats[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			D3DUtil_SetScaleMatrix(matScale, 1.2f, 1.2f, 1.2f); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

	case FX_FOLLOW_TYPE_ZORO:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case ZORO_STATE_STAND:
				pRefDrawBpd = m_pBpdZoro[FX_ZORO_ANI_STAND];
				break;
			case ZORO_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdZoro[FX_ZORO_ANI_STAND2];
				break;
			case ZORO_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdZoro[FX_ZORO_ANI_MOVE];
				break;
			case ZORO_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdZoro[FX_ZORO_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(NULL, FALSE);

			pRefDrawCsy = m_pCsyZoro[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			D3DUtil_SetScaleMatrix(matScale, 1.2f, 1.2f, 1.2f); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

	case FX_FOLLOW_TYPE_VALO:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case VALO_STATE_STAND:
				pRefDrawBpd = m_pBpdValo[FX_VALO_ANI_STAND];
				break;
			case VALO_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdValo[FX_VALO_ANI_STAND2];
				break;
			case VALO_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdValo[FX_VALO_ANI_MOVE];
				break;
			case VALO_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdValo[FX_VALO_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyValo[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			D3DUtil_SetScaleMatrix(matScale, 1.2f, 1.2f, 1.2f); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

	case FX_FOLLOW_TYPE_RUDOLPH: ///펫루돌프
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num ) // effect_sort의 num을 애니메이션 번호로 사용.
			{
			case FX_RUDOLPH_STATE_STAND:  // 그냥 서있는 상태.
				pRefDrawBpd = m_pBpdRudolph[FX_RUDOLPH_ANI_STAND]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_RUDOLPH_STATE_RANDOMWALK:  // 아무 방향으로나 걸어감.(천천히)
				pRefDrawBpd = m_pBpdRudolph[FX_RUDOLPH_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_RUDOLPH_STATE_FOLLOW:  // 주인을 쫒아가는 상태.
				pRefDrawBpd = m_pBpdRudolph[FX_RUDOLPH_ANI_WALK]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_RUDOLPH_STATE_SOCIAL1:  // 서서 애교동작
				pRefDrawBpd = m_pBpdRudolph[FX_RUDOLPH_ANI_SOCIAL1]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			case FX_RUDOLPH_STATE_SOCIAL2:	 // 클라이언트에서 주변 돌리는 춤
				pRefDrawBpd = m_pBpdRudolph[FX_RUDOLPH_ANI_SOCIAL2]; // effect_sort의 num을 애니메이션 번호(액션 스테이트)로 사용. (현재는 1:1 대응이므로 그냥 사용해도 된다.)
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyRudolph[effect_sort.nTargetType]; // effect_sort의 subNum을 돼지 종류 번호로 사용.

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y ); // vSortPos.y 를 프레임 값으로 사용.

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			D3DUtil_SetScaleMatrix(matScale, 0.8f, 0.8f, 0.8f); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

	case FX_FOLLOW_TYPE_BEAR:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case BEAR_STATE_STAND:
				pRefDrawBpd = m_pBpdBear[FX_BEAR_ANI_STAND];
				break;
			case BEAR_STATE_IDLE:
				pRefDrawBpd = m_pBpdBear[FX_BEAR_ANI_IDLE];
				break;
			case BEAR_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdBear[FX_BEAR_ANI_MOVE];
				break;
			case BEAR_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdBear[FX_BEAR_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyBear[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			D3DUtil_SetScaleMatrix(matScale, 1.2f, 1.2f, 1.2f); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

	case FX_FOLLOW_TYPE_HORSE:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case HORSE_STATE_STAND:
				pRefDrawBpd = m_pBpdHorse[FX_HORSE_ANI_STAND];
				break;
			case HORSE_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdHorse[FX_HORSE_ANI_STAND2];
				break;
			case HORSE_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdHorse[FX_HORSE_ANI_MOVE];
				break;
			case HORSE_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdHorse[FX_HORSE_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyHorse[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.9f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

	case FX_FOLLOW_TYPE_JEGUAR:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case JEGUAR_STATE_STAND:
			case JEGUAR_STATE_SOCIAL2:
				pRefDrawBpd = m_pBpdJeguar[FX_JEGUAR_ANI_STAND];
				break;
			case JEGUAR_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdJeguar[FX_JEGUAR_ANI_STAND2];
				break;
			case JEGUAR_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdJeguar[FX_JEGUAR_ANI_MOVE];
				break;
			case JEGUAR_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdJeguar[FX_JEGUAR_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyJeguar[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.2f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

	case FX_FOLLOW_TYPE_BONI:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case BONI_STATE_STAND:			
				pRefDrawBpd = m_pBpdBoni[FX_BONI_ANI_STAND];
				break;
			case BONI_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdBoni[FX_BONI_ANI_STAND2];
				break;
			case BONI_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdBoni[FX_BONI_ANI_MOVE];
				break;
			case BONI_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdBoni[FX_BONI_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyBoni[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.1f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

			case FX_FOLLOW_TYPE_AZUL:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case AZUL_STATE_STAND:			
				pRefDrawBpd = m_pBpdAZUL[FX_AZUL_ANI_STAND];
				break;
			case AZUL_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdAZUL[FX_AZUL_ANI_STAND2];
				break;
			case AZUL_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdAZUL[FX_AZUL_ANI_MOVE];
				break;
			case AZUL_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdAZUL[FX_AZUL_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyAZUL[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;           // ###########################################################################################################################

		case FX_FOLLOW_TYPE_VERDE:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case VERDE_STATE_STAND:			
				pRefDrawBpd = m_pBpdVERDE[FX_VERDE_ANI_STAND];
				break;
			case VERDE_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdVERDE[FX_VERDE_ANI_STAND2];
				break;
			case VERDE_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdVERDE[FX_VERDE_ANI_MOVE];
				break;
			case VERDE_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdVERDE[FX_VERDE_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyVERDE[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;           // ###########################################################################################################################

		case FX_FOLLOW_TYPE_ROXO:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case ROXO_STATE_STAND:			
				pRefDrawBpd = m_pBpdROXO[FX_ROXO_ANI_STAND];
				break;
			case ROXO_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdROXO[FX_ROXO_ANI_STAND2];
				break;
			case ROXO_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdROXO[FX_ROXO_ANI_MOVE];
				break;
			case ROXO_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdROXO[FX_ROXO_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyROXO[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;           // ###########################################################################################################################

		case FX_FOLLOW_TYPE_AMARELO:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case AMARELO_STATE_STAND:			
				pRefDrawBpd = m_pBpdAMARELO[FX_AMARELO_ANI_STAND];
				break;
			case AMARELO_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdAMARELO[FX_AMARELO_ANI_STAND2];
				break;
			case AMARELO_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdAMARELO[FX_AMARELO_ANI_MOVE];
				break;
			case AMARELO_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdAMARELO[FX_AMARELO_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyAMARELO[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;           // ###########################################################################################################################

		case FX_FOLLOW_TYPE_VERMELHO:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case VERMELHO_STATE_STAND:			
				pRefDrawBpd = m_pBpdVERMELHO[FX_VERMELHO_ANI_STAND];
				break;
			case VERMELHO_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdVERMELHO[FX_VERMELHO_ANI_STAND2];
				break;
			case VERMELHO_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdVERMELHO[FX_VERMELHO_ANI_MOVE];
				break;
			case VERMELHO_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdVERMELHO[FX_VERMELHO_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyVERMELHO[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.055f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;           // ###########################################################################################################################

		case FX_FOLLOW_TYPE_CINZA:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case CINZA_STATE_STAND:			
				pRefDrawBpd = m_pBpdCINZA[FX_CINZA_ANI_STAND];
				break;
			case CINZA_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdCINZA[FX_CINZA_ANI_STAND2];
				break;
			case CINZA_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdCINZA[FX_CINZA_ANI_MOVE];
				break;
			case CINZA_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdCINZA[FX_CINZA_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyCINZA[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.055f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;           // ###########################################################################################################################

		case FX_FOLLOW_TYPE_FESHIK:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case FESHIK_STATE_STAND:			
				pRefDrawBpd = m_pBpdFESHIK[FX_FESHIK_ANI_STAND];
				break;
			case FESHIK_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdFESHIK[FX_FESHIK_ANI_STAND2];
				break;
			case FESHIK_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdFESHIK[FX_FESHIK_ANI_MOVE];
				break;
			case FESHIK_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdFESHIK[FX_FESHIK_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyFESHIK[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.055f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;           // ###########################################################################################################################

		case FX_FOLLOW_TYPE_THUNDERBIRD:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case THUNDERBIRD_STATE_STAND:			
				pRefDrawBpd = m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_STAND];
				break;
			case THUNDERBIRD_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_STAND2];
				break;
			case THUNDERBIRD_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_MOVE];
				break;
			case THUNDERBIRD_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyTHUNDERBIRD[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.055f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_THUNDERBIRD_P:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case THUNDERBIRD_P_STATE_STAND:			
				pRefDrawBpd = m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_STAND];
				break;
			case THUNDERBIRD_P_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_STAND2];
				break;
			case THUNDERBIRD_P_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_MOVE];
				break;
			case THUNDERBIRD_P_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyTHUNDERBIRD_P[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.055f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_EZULAN:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case EZULAN_STATE_STAND:			
				pRefDrawBpd = m_pBpdEZULAN[FX_EZULAN_ANI_STAND];
				break;
			case EZULAN_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdEZULAN[FX_EZULAN_ANI_STAND2];
				break;
			case EZULAN_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdEZULAN[FX_EZULAN_ANI_MOVE];
				break;
			case EZULAN_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdEZULAN[FX_EZULAN_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyEZULAN[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETFAME1:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETFAME1_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETFAME1[FX_PETFAME1_ANI_STAND];
				break;
			case PETFAME1_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETFAME1[FX_PETFAME1_ANI_STAND2];
				break;
			case PETFAME1_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETFAME1[FX_PETFAME1_ANI_MOVE];
				break;
			case PETFAME1_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETFAME1[FX_PETFAME1_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETFAME1[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.060f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETFAME2:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETFAME2_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETFAME2[FX_PETFAME2_ANI_STAND];
				break;
			case PETFAME2_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETFAME2[FX_PETFAME2_ANI_STAND2];
				break;
			case PETFAME2_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETFAME2[FX_PETFAME2_ANI_MOVE];
				break;
			case PETFAME2_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETFAME2[FX_PETFAME2_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETFAME2[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.07f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;


		case FX_FOLLOW_TYPE_PETFAME3:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETFAME3_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETFAME3[FX_PETFAME3_ANI_STAND];
				break;
			case PETFAME3_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETFAME3[FX_PETFAME3_ANI_STAND2];
				break;
			case PETFAME3_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETFAME3[FX_PETFAME3_ANI_MOVE];
				break;
			case PETFAME3_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETFAME3[FX_PETFAME3_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETFAME3[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 1.1f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;


		case FX_FOLLOW_TYPE_PETFAME4:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETFAME4_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETFAME4[FX_PETFAME4_ANI_STAND];
				break;
			case PETFAME4_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETFAME4[FX_PETFAME4_ANI_STAND2];
				break;
			case PETFAME4_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETFAME4[FX_PETFAME4_ANI_MOVE];
				break;
			case PETFAME4_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETFAME4[FX_PETFAME4_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETFAME4[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.25f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETHELPER:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETHELPER_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETHELPER[FX_PETHELPER_ANI_STAND];
				break;
			case PETHELPER_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETHELPER[FX_PETHELPER_ANI_STAND2];
				break;
			case PETHELPER_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETHELPER[FX_PETHELPER_ANI_MOVE];
				break;
			case PETHELPER_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETHELPER[FX_PETHELPER_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETHELPER[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 4.0f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETHALL1:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETHALL1_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETHALL1[FX_PETHALL1_ANI_STAND];
				break;
			case PETHALL1_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETHALL1[FX_PETHALL1_ANI_STAND2];
				break;
			case PETHALL1_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETHALL1[FX_PETHALL1_ANI_MOVE];
				break;
			case PETHALL1_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETHALL1[FX_PETHALL1_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETHALL1[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETHALL2:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETHALL2_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETHALL2[FX_PETHALL2_ANI_STAND];
				break;
			case PETHALL2_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETHALL2[FX_PETHALL2_ANI_STAND2];
				break;
			case PETHALL2_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETHALL2[FX_PETHALL2_ANI_MOVE];
				break;
			case PETHALL2_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETHALL2[FX_PETHALL2_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETHALL2[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETHALL3:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETHALL3_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETHALL3[FX_PETHALL3_ANI_STAND];
				break;
			case PETHALL3_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETHALL3[FX_PETHALL3_ANI_STAND2];
				break;
			case PETHALL3_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETHALL3[FX_PETHALL3_ANI_MOVE];
				break;
			case PETHALL3_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETHALL3[FX_PETHALL3_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETHALL3[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETNATAL1:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETNATAL1_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETNATAL1[FX_PETNATAL1_ANI_STAND];
				break;
			case PETNATAL1_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETNATAL1[FX_PETNATAL1_ANI_STAND2];
				break;
			case PETNATAL1_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETNATAL1[FX_PETNATAL1_ANI_MOVE];
				break;
			case PETNATAL1_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETNATAL1[FX_PETNATAL1_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETNATAL1[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;

		case FX_FOLLOW_TYPE_PETNATAL2:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETNATAL2_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETNATAL2[FX_PETNATAL2_ANI_STAND];
				break;
			case PETNATAL2_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETNATAL2[FX_PETNATAL2_ANI_STAND2];
				break;
			case PETNATAL2_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETNATAL2[FX_PETNATAL2_ANI_MOVE];
				break;
			case PETNATAL2_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETNATAL2[FX_PETNATAL2_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETNATAL2[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;


		case FX_FOLLOW_TYPE_PETNATAL3:			//PABLO TEST PET   ##################################################################################
		{
			if( !effect_sort.pNkChaFrom )
				return;

			switch( effect_sort.num )
			{
			case PETNATAL3_STATE_STAND:			
				pRefDrawBpd = m_pBpdPETNATAL3[FX_PETNATAL3_ANI_STAND];
				break;
			case PETNATAL3_STATE_SOCIAL1:
				pRefDrawBpd = m_pBpdPETNATAL3[FX_PETNATAL3_ANI_STAND2];
				break;
			case PETNATAL3_STATE_RANDOMMOVE:
				pRefDrawBpd = m_pBpdPETNATAL3[FX_PETNATAL3_ANI_MOVE];
				break;
			case PETNATAL3_STATE_FOLLOW:
				pRefDrawBpd = m_pBpdPETNATAL3[FX_PETNATAL3_ANI_MOVE];
				break;
			}

			g_pShadowRect->SetTileCoord(D3DVECTOR(effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z), 7, FALSE);
			g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHADOWLIGHT ] ), FALSE);

			pRefDrawCsy = m_pCsyPETNATAL3[effect_sort.nTargetType];

			if( pRefDrawCsy == NULL )
				break;

			pRefDrawCsy->Animate( pRefDrawBpd, (int)effect_sort.vSortPos.y+1 );

			D3DUtil_SetIdentityMatrix(matTrans);
			D3DUtil_SetIdentityMatrix(matRotate);
			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x ); // vSortPos.x를 방향값으로 사용.

			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

			float scale = 0.06f;
			D3DUtil_SetScaleMatrix(matScale, scale, scale, scale); // 약간 크기를 줄인다.
			D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			pRefDrawCsy->Render( GET_D3DDEVICE() );
		}
		break;
	// [12/4/2007 반재승] 일본 링아이템 이펙트
	case FX_FOLLOW_TYPE_RING_LIGHT:
		{
			if( !effect_sort.pNkChaFrom )
				return;

			// 주변 이펙트 출력
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

			// 깜빡이는 이펙트 출력.
			m_pEffect1->SetBillBoard( vViewVec, FALSE, 0.0f );
			m_pEffect1->RotateZ(timeGetTime()/3000.0f);
			m_pEffect1->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );

			// 깜빡이는 이펙트
			rectwidth += RandomNum(2.0f, 5.0f) ;

			SetRing_Light_Color( effect_sort.nTargetType );

			if( m_ring_Light_Color < 0 )
				return;

			if( m_pEffect1 )
			{
				m_pEffect1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), m_ring_Light_Color, 0, 0.0f, 1.0f )  );
				m_pEffect1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), m_ring_Light_Color, 0, 0.0f, 0.0f )  );
				m_pEffect1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), m_ring_Light_Color, 0, 1.0f, 1.0f )  );
				m_pEffect1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), m_ring_Light_Color, 0, 1.0f, 0.0f )  );
			}

			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_RING_LIGHT_EFFECT ] ) );
			m_pEffect1->Render();

			list<D3DVECTOR> *TraceInfoList;
			TraceInfoList = (list<D3DVECTOR> *)effect_sort.effects; // 저장은 effect_sort 단위로 되어있으니 포인터를 받아와서 처리.

			D3DLVERTEX TempVertexArray[MAX_RING_LIGHT_TRACE*2];
			D3DLVERTEX TempVertexArray2[MAX_RING_LIGHT_TRACE*2];

			int k = 0;
			int l = 0;
			int j = TraceInfoList->size();
			int i = 0;
			float fLineWidth = 0.3f;

			D3DVECTOR TempVertex;
			list<D3DVECTOR>::iterator pr;

			i = 0;

			for( pr = TraceInfoList->begin() ; pr != TraceInfoList->end() ; pr++ )
			{
				TempVertex = *pr;

				if( k < (MAX_RING_LIGHT_TRACE*2)-2 )
				{
					// 리스트의 하나당 점 두개를 그려준다.
					TempVertexArray[k] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y-(float)((float)(k/2)/(float)j)*fLineWidth, TempVertex.z ), m_ring_Light_TailColor, 0, (float)((float)(k/2)/(float)j), 0.0f );
					k++;

					TempVertexArray[k] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y+(float)((float)(k/2)/(float)j)*fLineWidth, TempVertex.z ), m_ring_Light_TailColor, 0, (float)((float)(k/2)/(float)j), 1.0f );
					k++;
				}
			}

			i = 0;

			for( pr = TraceInfoList->begin() ; pr != TraceInfoList->end() ; pr++ )
			{
				TempVertex = *pr;

				if( l < (MAX_RING_LIGHT_TRACE*2)-2 )
				{
					// 리스트의 하나당 점 두개를 그려준다.
					TempVertexArray2[l] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y, TempVertex.z-(float)((float)(l/2)/(float)j)*fLineWidth ), m_ring_Light_TailColor, 0, (float)((float)(l/2)/(float)j), 0.0f );
					l++;
					TempVertexArray2[l] = D3DLVERTEX( D3DVECTOR( TempVertex.x, TempVertex.y, TempVertex.z+(float)((float)(l/2)/(float)j)*fLineWidth ), m_ring_Light_TailColor, 0, (float)((float)(l/2)/(float)j), 1.0f );
					l++;
				}
			}

			D3DUtil_SetIdentityMatrix(matWorld);
			matWorld._41 = 0.0f;
			matWorld._42 = 0.0f;
			matWorld._43 = 0.0f;

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
			GET_D3DDEVICE()->SetTexture(0,NULL );
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &TempVertexArray, k, 0 );
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &TempVertexArray2, l, 0 );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
		}

		break;
	}


	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );
}

void CFxFollow::SpitRender(EffectSort &effect_sort)
{
	if( !m_bSpit ) return;

	DWORD alphaTest, alphaBlend;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCCOLOR );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_SRCALPHA );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


//////////////////////////////////////////////////////////////////////////
	D3DVECTOR	vViewVec;

	vViewVec = pCMyApp->GetEyeDirection();

	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ARUPAKA_DROP ] ) );


	m_pRectPlane->SetBillBoard(vViewVec, true, 0.0f);       //-- Make Billboard
	m_pRectPlane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pRectPlane->RotateX(float(3.14f/2.0f));
	//m_pRectPlane->Scale(m_fScale, m_fScale, 0.0f);           //-- Scale

	m_pRectPlane->SetTranslationXYZ(m_vMasterPosDir.x, m_vMasterPosDir.y+0.2f, m_vMasterPosDir.z);
	m_pRectPlane->Render();

//////////////////////////////////////////////////////////////////////////



	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

}

// [12/6/2007 반재승] 일본 링아이템 이펙트 관련 함수.
void	CFxFollow::SetRing_Light_Color( int ringNum )
{
	float alpah = RandomNum(0.6f,1.0f);

	switch( ringNum )
	{
	case SKILL_RING_LIGHT_WHITE:
		m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 1.0f, alpah );
		m_ring_Light_TailColor = RGBA_MAKE(255,255,255,255);
		break;

	case SKILL_RING_LIGHT_DARK:
		m_ring_Light_Color = D3DRGBA( 0.53f, 0.0f, 0.53f, 1.0f );
		m_ring_Light_TailColor = RGBA_MAKE(160,0,160,255);
		break;

	case SKILL_RING_LIGHT_RED:
		m_ring_Light_Color = D3DRGBA( 1.0f, 0.0f, 0.0f, alpah );
		m_ring_Light_TailColor = RGBA_MAKE(255,0,0,255);
		break;

	case SKILL_RING_LIGHT_BLUE:
		m_ring_Light_Color = D3DRGBA( 0.2f, 0.2f, 1.0f, 1.0f );
		m_ring_Light_TailColor = RGBA_MAKE(10,10,255,255);
		break;

	case SKILL_RING_LIGHT_GREEN:
		m_ring_Light_Color = D3DRGBA( 0.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_TailColor = RGBA_MAKE(0,255,0,255);
		break;

	case SKILL_RING_LIGHT_GOLD: // 추가로 선언만 되어있음
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,215,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(255,215,0,(int)(255*alpah));
		break;

	case SKILL_ROYALE_1: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,255,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(255,0,0,(int)(255*alpah));
		break;
	case SKILL_ROYALE_2: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,0,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(0,255,0,(int)(255*alpah));
		break;
	case SKILL_ROYALE_3: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(0,255,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(0,0,255,(int)(255*alpah));
		break;
	case SKILL_ROYALE_4: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(0,0,255,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(255,0,255,(int)(255*alpah));
		break;
	case SKILL_ROYALE_5: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,0,255,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(0,0,0,(int)(255*alpah));
		break;
	case SKILL_ROYALE_6: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(0,0,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(255,255,255,(int)(255*alpah));
		break;
	case SKILL_ROYALE_7: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,255,255,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(153,0,153,(int)(255*alpah));
		break;
	case SKILL_ROYALE_8: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(153,0,153,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(0,255,255,(int)(255*alpah));
		break;
	case SKILL_ROYALE_9: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(0,255,255,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(0,255,0,(int)(255*alpah));
		break;
	case SKILL_ROYALE_10: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(0,255,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(128,128,128,(int)(255*alpah));
		break;
	case SKILL_ROYALE_11: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(128,128,128,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(255,255,0,(int)(255*alpah));
		break;
	case SKILL_ROYALE_12: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,255,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(0,0,153,(int)(255*alpah));
		break;
	case SKILL_ROYALE_13: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(0,0,153,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(0,102,102,(int)(255*alpah));
		break;
	case SKILL_ROYALE_14: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,215,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(255,215,0,(int)(255*alpah));
		break;
	case SKILL_ROYALE_15: 
		// m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 0.0f, alpah );
		m_ring_Light_Color = RGBA_MAKE(255,215,0,(int)(255*alpah));
		m_ring_Light_TailColor = RGBA_MAKE(255,215,0,(int)(255*alpah));
		break;

	default:
		m_ring_Light_Color = D3DRGBA( 1.0f, 1.0f, 1.0f, alpah );
		m_ring_Light_TailColor = RGBA_MAKE(255,255,255,255);
		break;
	}
}




