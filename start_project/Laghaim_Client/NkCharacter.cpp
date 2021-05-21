#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include <time.h>
#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "main.h"
#include "ShadowRect.h"
#include "D3DMath.h"
#include "Land.h"
#include "ClrAlloc.h"
#include "GoodShadow.h"
#include "ItemProto.h"
#include "ChaAct.h"
#include "DirtSnd.h"
#include "ControlScore.h"
#include "ControlShop.h"
#include "ControlSkill.h"
#include "controlBottom.h"
#include "ControlInven.h"
#include "ControlUpgrade.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "UIMgr.h"
#include "Tcpipcon.h"
#include "GUtil.h"
#include "LoadEffectSound.h"
#include "d3dmath.h"
#include "FxSet.h"
#include "AlphaPopup.h"
#include "SpecialItem.h"
#include "headers.h"
#include "Path.h"
#include "ChaSta.h"
#include "controlpeiceshop.h"
#include "DropStar.h"
#include "tcputil.h"
#include "Nk2DFrame.h"
#include "GuildPK.h"
#include "NKObjQuadNode.h"
#include "g_stringmanager.h"
#include "sfx.h"
#include "ImageDefine.h"
#include "FxFreePvpRing.h"
#include "guild_data.h"
#include "ControlGuildT.h"
#include "ControlGuildM.h"
#include "FxSiren.h"
#include "AutoPlay_test.h"
#include "FxFollow.h"
#include "ItemOutlineMgr.h"
#include "CommonConfig.h"
#include "DecardiBattleManager.h"
#include "PacketSend.h"

CNkCharacter * g_pRoh = NULL;


#define SHOW_CHATPOPUP_TIME	(5000)

// nate 2004 - 3 : 대만 훈장 관련 패치
extern DWORD g_dwClientCountry;
// 서버 인덱스 ( 쪽지 시스템 )
extern int	g_nServerIndex = 0;

// nate 2004 - 8 : 신무기 관련 - 뮤턴트 색상입히기
extern int g_nMutantTextureNum[];
// nate 2004 - 7 글로벌 ShadowRect사용

extern int g_nEmptyTextureNum;




extern BOOL g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.
extern float g_fDSDistance;

extern CNkMob* g_pTargetMob[];
extern CNkCharacter* g_pTargetCha[];
extern void LoadChaActList();
extern void DeleteChaActList();
extern void DeleteItems();
//extern CChaAct	g_ChaBpdList[MAX_CHA_ACT_NUM];
// nate 2004 - 11 : new race
//extern int g_ChaBpdIndex[ NUM_RACE*NUM_SEX ][ CHA_ACT_INDEX_NUM ];

extern int g_EffectTextr[];
// nate 2004 - 4 : CastleWar
extern CLolos *g_pWhangLolo1;
extern CLolos *g_pWhangLolo2;
extern CLolos *g_pWhangLolo3;
extern CLolos *g_pWhangLolo4;
extern BOOL g_bPath;
//1002
extern char commOutBuf[];
// nate 2004 - 4 : CastleWar
extern CSurface *g_pImgKing1;
extern CSurface *g_pImgKing2;
extern CSurface *g_pImgKing3;
extern CSurface *g_pImgKing4;
extern	CSurface *g_pPK;
extern	CSurface *g_pNonPK;
extern	CSurface *g_pGuildpk;
extern	CSurface *g_pMatrixKor; ///국가대항전 시의 국가 명 이미지
extern	CSurface *g_pMatrixJpn; ///국가대항전 시의 국가 명 이미지
extern	CSurface *g_pMatrixBra;
extern	CSurface *g_pMatrixGer;
extern	CSurface *g_pMatrixChi;
extern	CSurface *g_pMatrixInd;
extern	CSurface *g_pMatrixTha;
extern	CSurface *g_pNvNWin; ///국가전승리 마크
// nate : 대만 매달 관련
extern	CSurface *g_pMedal;
extern int g_AirshipProto[];
extern int g_AirshipProto_VNUM[];
extern int g_BasicWingsProto;
extern int g_BasicWingsProto_VNUM;
extern int g_MatrixSvr;
extern BOOL	g_bNewTerrain;
extern CLolos	*g_pDisplayStandLolo[3];  // 개인상점 상품진열대 오브젝트 로드용 ///개인상점
extern CLolos	*g_pChouchin; ///일본디자인컨테스트 등롱 아이템 이펙트용.

CLolos	CNkCharacter::m_CubeLolo;
DWORD	CNkCharacter::m_dwAmbient;
CLolos	*g_FlyStones[];
float	g_ChaFlyHeight[CHA_FLY_FRAME];
int		g_RohFlyLevel = 0;

BOOL CheckCanNpcUpgradeItem( CItem *pItem ); // 강화가 가능한 아이템인지의 체크용. ///신방어구강화시스템

void DrawInstanceEffect( int ItemIndex, CNkCharacter *pNkChar, CCapsyong *pTargetCsy/*CItem *TargetItem*/ ); // 이펙트 리스트에 들어가서 관리되는게 아니라 바로 출력해주는 이펙트 효과.(아이템용.) ///일본디자인컨테스트 아이템 효과를 넣기 위해 준비됐다. // 임시테스트용

extern BOOL g_bUseEventWear; ///이벤트복장착용
extern bool g_bShowIndex;


#define TEST_GUILDNAME_2010


// 전역 초기화
void PlayerInitialLoading()
{
	g_pCapsyongTexture->SetTexturePath("data/");
	// 픽버퍼에 렌더링할 캐릭터 정육면체.
	PFILE *fpLolo = NULL;
	fpLolo = pfopen("data/cube.lol", "rb");
	assert(fpLolo != NULL);
	CNkCharacter::m_CubeLolo.~CLolos();
	CNkCharacter::m_CubeLolo.SetIndexedTexture(g_pCapsyongTexture);
	CNkCharacter::m_CubeLolo.Read(fpLolo);
	pfclose(fpLolo);

	// 바이페드 읽기
	LoadChaActList();
	// 하늘에서 붕붕거리는 높이를 미리 계산해놓자.
	for (int i = 0; i < CHA_FLY_FRAME; i++)
		g_ChaFlyHeight[i] = sinf(g_2_PI * i / CHA_FLY_FRAME) * 2 + 2;
}

void DeletePlayerData()
{
	DeleteChaActList();
	DeleteItems();
	SAFE_DELETE( g_pCapsyongTexture );
}

// static member variables
float CNkCharacter::m_fRunSpeed = 8.0f;
int	  CNkCharacter::m_nCurHour = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNkCharacter::CNkCharacter(CShadowRect *pShadowRect, CGoodShadow *pGoodShadow, CClrAlloc *pClrAlloc)
	: m_wx(0)
	, m_wy(0)
	, m_wz(0)
	, m_dir(0)
	, m_dx(0)
	, m_dz(0)
	, m_dwTimeCapPrev(0)
	, m_tx(0)
	, m_ty(0)
	, m_tz(0)
	, m_Race(0)
	, m_Hair(0)
	, m_BpdRace(0)
	, m_dwOwnAmbient(0)
	, m_LastAttackVitalTime(0)
	, m_bPlace(FALSE)
	, m_fPlaceDir(0)
	, m_ChaCondition(0)
	, m_ChaCondition2(0)
	, m_CancelState2(0)
	, m_iMaxPartysNum(0)
	, m_fDash_Frame(0)
	, m_resmobdir(0)
	, m_bSwapWeapon(FALSE)
	, m_nGuildNameLine(0)
	, m_nHaveGuildMark(false)
	, m_indexSummonNpc(0)
{
	memset(m_iPartysListId, 0, sizeof(m_iPartysListId));
	memset(m_fPartysVitalList, 0, sizeof(m_fPartysVitalList));
	memset(m_iPartysListXpos, 0, sizeof(m_iPartysListXpos));
	memset(m_iPartysListYpos, 0, sizeof(m_iPartysListYpos));

	m_Biped = NULL;
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
		m_pCrash_MissPopup[ i ] = new CAlphaPopUp;

	m_pGiftMessage = new CAlphaPopUp;
	m_byShowGiftMessage = 0;
	m_byShowCrash_Miss = 0;
	m_nElapTime = 0;
	m_dwElapTime = 0;
	m_byShowGiftMessage = 0;
	m_bGuildPK = false;
	m_bChakramatt = false;
	ZeroMemory(m_sEnemyGName,sizeof(m_sEnemyGName));

	for ( int i = 0; i < CAPSYONG_NUM; i++)
		m_CapSyong[i] = NULL;

	m_CurFrame = 0;
	m_PrevFrame = 0;

	for ( int i = 0; i < CAPSYONG_NUM + 2; i++)
		m_bShow[i] = TRUE;

	for ( int i = 0; i < WEARING_NUM; i++)
	{
		m_Wearing[i] = NULL;
		m_EventWearing[i] = NULL; ///이벤트복장착용
	}

	m_ExtraSlot = NULL;

	for ( int i = 0; i < PACK_NUM; i++)
	{
		m_Inven[i] = NULL;

		for (int x = 0; x < SLOT_X_NUM; x++)
			for (int y = 0; y < SLOT_Y_NUM; y++)
				m_Slot[i][x][y] = -1;
	}

	m_ExgInven = NULL;
	m_bExgLock = FALSE;

	for ( int i=0; i < EXG_SLOT_X_NUM; i++)
		for ( int j = 0; j < EXG_SLOT_Y_NUM; j++)
			m_ExgSlot[i][j] = -1;

	for ( int i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		m_QuickSlot[i] = NULL;
		m_bQuickSlotLock[i] = FALSE;
		m_QuickCount[i] = 0;
	}

	m_CurPackNum = 0;
	m_pGoodShadow = pGoodShadow;
	m_dwBaseTime = timeGetTime();
	m_AniRate = 0.05f;
	m_bMoving = FALSE;
	m_bAttack = FALSE;
	m_bRun = FALSE;
	m_nUserIndex = 0;
	m_nCharIndex = -1;
	m_pstrName = NULL;
	m_pstrGuildName = NULL;
	m_pstrGuildGradeName = NULL;
	m_GuildIndex = 0;
	m_GuildGradeIndex = 0;
	m_pClrAlloc = pClrAlloc;

	m_bShowMatrixMark = TRUE;

	if (m_pClrAlloc)
		m_pClrAlloc->ColorAlloc(&m_dwColorPick, &m_wColorIdx);
	else
	{
		m_wColorIdx = 0;
		m_dwColorPick = 0x00ffffff;
	}

	m_ActNum = 0;
	m_Sex = 0;

	m_Level = 1;
	m_Vital = 1;
	m_MaxVital = 1;
	m_Mana = 1;
	m_MaxMana = 1;
	m_Stamina = 1;
	m_MaxStamina = 1;
	m_Epower = 1;
	m_MaxEpower = 1;
	m_Str = 1;
	m_Int = 1;
	m_Dex = 1;
	m_Con = 1;
	m_Cha = 1;
	m_Exp = 0;
	m_NeedExp = 0;
	m_LevelupPoint = 0;
	m_Money = 0;
	m_Fame = 0;
	m_pScoreWnd = NULL;
	m_AttackChaId = -1;
	m_AttackMobId = -1;
	m_AttackVital = m_Vital;
	m_DeadFrame = -1;
	m_bCombat = FALSE;
	m_toDir = -100;
	m_bHeroAttCome = FALSE;
	m_MagicChaId[0] = -1;
	m_MagicMobId[0] = -1;
	m_MagicChaCount = 0;
	m_MagicMobCount = 0;
	m_MagicNum = -1;

	m_pSiren = NULL;

	m_pChatPopup = new CAlphaPopUp;
	if( m_pChatPopup )
	{
		m_pChatPopup->Init(12,AlphaPopUp_Type__Char);
		m_pChatPopup->SetRectSoft(TRUE);
	}

	m_pUserShopPopup = new CAlphaPopUp; ///개인상점
	if( m_pUserShopPopup )
	{
		m_pUserShopPopup->Init(12,AlphaPopUp_Type__Char);
		m_pUserShopPopup->SetRectSoft(TRUE);
	}

	m_LastChatTime = timeGetTime() - SHOW_CHATPOPUP_TIME;
	m_bShowChatPopup = FALSE;
	m_nMotionState = CHA_STAND;
	m_TargetHeight = MOB_HEIGHT_MIDDLE_VAL;
	m_nManaShield = 0;
	m_nElectricShield = 0;
	m_nSafetyShield = 0;
	isShieldLoad = FALSE;
	m_bMyParty = FALSE;
	m_bMyGuild = FALSE;
	m_bOppGuild = FALSE;
	m_bCrystalOwner = FALSE;
	m_bCrystalGuild = FALSE;
	m_nTeamMemGrade = 0;
	m_bCatchFlagShow = FALSE;
	m_MultiShot = 0;
	m_NextMultiShot = 0;
	m_RenderMultiShot = 0;
	m_HideStatus = HIDE_SHOWN;
	m_HideTime = timeGetTime();
	m_GuildLevel = 0;

	m_InChaSkill = 0; //초기 스킬값.
	m_InChaAidiaSkill = 0; //초기 에이디아 스킬값.
	m_UseBook = 0; //책 사용 효과
	m_ShowConditionEffect = FALSE; //상태효과
	m_ShowConditionEffect2 = FALSE; //상태효과2
	m_ShowSkillEffect = FALSE; //지속스킬효과
	m_ShowAidiaSkillEffect = FALSE; //에이디아 지속스킬효과
	m_CancelState = 0;
	m_ChaAttackType = 0;
	m_Accumulate = FALSE;
	m_Chaotic_Grade = 0;
	m_Free_Pk = 0;
	m_Chaotic_Num = 0;
	m_bPk_render = FALSE;
	m_bNonPK = FALSE;
	m_bIsParty = FALSE;
	m_DoubleAttack = FALSE;
	m_pDropStar = NULL;
	m_nFlyFrame = rand() % CHA_FLY_FRAME;
	m_FlyLevel = 0;

	m_bOwnAmbient = FALSE;
	m_NkCha_Particle = new Deco_Particle[MAX_NKCHA_PARTICLE];
	Initial_Particle();
	m_drawLineNum = 0;
	m_DrawWeaponEffects = 0;
	m_bMoonEventEffect = FALSE; //0212 대보름 이벤트때 Effect
	m_ShowMoonEventEffect = FALSE;
	m_bShowPartysVital = FALSE;
	m_bRecivedPartysInfo = FALSE;
	m_bIsParty = FALSE;
	m_CurAttackType = 0;

	for( int i = 0; i < 4; i++ )
	{
		m_bFlagOwner[i] = FALSE;
		m_bFlagGuild[i] = FALSE;
	}

	mi_Snifing_GAP_Frame = 0;
	mb_Snifing = false;

	m_bstone = false;       // 디바인 스톤을 사용했나. Framemov에서 땅에 박히는 순간 활성화 된다.
	m_bresstone = false;    // 디바인 스톤이 생성될 준비가 됐는가.
	m_resmobidx =0;			// 생성될 몹의 인덱스
	m_resmobvnum =0;		// 생성될 몹의 vnum
	m_resmobx =0;			// 생성될 몹의 좌표
	m_resmobz =0;			// 생성될 몹의 좌표
	m_bBleed = false;		// 블레드
	m_bhaste = 0;			// 헤이스트
	m_nBattlePoint = 0;
	m_nLaghaimPoint = 0; // [2008/5/16 Theodoric] BP -> LP 로 바뀜
	m_nGHPoint = 0;			// 그랜드 히어로 포인트, 사용치를 고려하지 않은 총 누적 포인트이다.
	m_bComboReady = false;		//mungmae-2005/04/11
	m_bComboSucceed = false;	//mungmae-2005/04/12
	m_ShowReflection = false;	//mungmae-2005/04/20
	m_bProtectionShield = false;
	m_nComboFrame = 0;			//mungmae-2005/05/03
	m_byBurningStep = 0;
	m_nSkillLevel = 0;			// nate 2005-06-17 : 스킬의 숙련도( 스킬포인트 )
	m_nSkillStep = 0;			// nate 2005-06-27 : 휴먼-마인갯수, 불칸-버닝크래쉬단계
	m_nFailStep = 0;
	m_nOldFrame = 0;			// nate 2005-07-21 : Burning - 메세지를 보낼지 판단

	m_pRegenbar = NULL;
	m_pRegenbar = new CFxRegenBar();
	m_dwRegenTime = 0;
	m_nRegenIndex = 0;
	m_bDrawRegenbar = FALSE;
	m_bAttacked = FALSE;		// mungmae 2005-07-13: 공격받고 있나?
	m_nAttVital = 0;
	m_byShowLIfeExpantion = 0;	// nate 2005-07-24 : Life Expansion - 생명연장 이펙트 보이기

	m_bCloaking = FALSE;
	m_bCloaking_Waring = FALSE;
	m_bPoisoning = false;		// nate 2005-11-14 : 포이즌 - 포이즌상태 저장
	m_fFlying_frame = 0.0f;
	m_fShadow_frame = 0.0f;
	m_bShowAdrenaline = false;	//mungmae-2005/11/21 : 아드레날린 효과 유무
	m_fSubAttackTime = 0.0f;
	m_bYanQuest	= FALSE;

	m_bVisible = FALSE; // 프러스텀 컬링상에서 보이는가의 플래그.
	m_bSecialQuest = FALSE;

	m_UserShopLevel = 0; // 개인 상점을 열었는가? ///개인상점
	ZeroMemory( m_strUserShopMent, sizeof(m_strUserShopMent) );
	//m_bSwapWeapon = TRUE;

	m_HaveCash = 0; // 보유 캐쉬 ///유료상점

	m_dwComboTick = 0; ///하이콤보

	m_MultiTargetNum = 0;  // 하이브리더 해머 콤보시 타겟 갯수를 저장해놓을 필요가있어서... ///하이콤보
	m_MultiTargetKind = 0; ///하이콤보
	ZeroMemory( m_MultiTargetArray, sizeof(DWORD)*MAX_MULTITARGET ); ///하이콤보
	ZeroMemory( m_MultiTargetPos, sizeof(POINT)*MAX_MULTITARGET ); ///하이콤보

	m_bSpiritModeEffect = FALSE; //mungmae-2006/09/08 스피릿 지속적인 효과 켜고 끄기

	m_dwReturnTime = 0;
	m_dwResurrectionTime = 0;
	m_dwResurrection_StartTime = 0;
	m_bResurrectionSkill = FALSE;
	m_nResurrection_ChaId = 0;

	m_pMyGuilddata  = NULL;

	m_NvNWin = 0; ///국가전승리 횟수.

	m_bNowFreePVP = FALSE; // 자유대련 중인지 여부
	m_dwPvpDestIndex = 0; // 대련상대의 인덱스.(자신이 자유대련 중에만 셋팅.)

	m_vPvpRingPos = D3DVECTOR(0.0f,0.0f,0.0f);
	m_dwRingCreateTick = 0;
	m_bchaDontAttackEffect = FALSE;

	m_SirenColorFlag = 0x00;// Siren

	m_BossRaidHeroEffectIndex = -1;
	m_SetItem = SETITEM_NONE;
	m_SetItemEventWear = SETITEM_NONE;

	m_SvrChannelNum = 0;

	m_bEventWearFlag = FALSE;

	m_lv400_effect_index = -1;

	m_bWanted = false;

	m_send_rivial = false;


	m_d_system = false;
	m_activate_field = false;
	m_activate_engage = false;
	m_activate_critical = false;
	m_activate_field_ex = false;
	m_activate_buff_skill = 0;

	m_bpexp = 0;
	m_bpmax = 0;
	m_bBrLv =0;


	m_bHellDowning = false;

	m_fScaleSize = 1.0f;

	memset(m_bEventWearRendered, 0, sizeof(m_bEventWearRendered));

	for( int i = 0 ; i < 10 ; i ++ )
	{
		hitpoints[i][0] = 0;
		hitpoints[i][1] = 0;
	}
}

void CNkCharacter::CreateSiren(int color)
{
	SAFE_DELETE(m_pSiren);

	SetSirenOn(color);
	int SirenColor = GetSirenColor();

	if( SirenColor != -1)
	{
		m_pSiren = new CFxSiren;
		m_pSiren->Create(4, (int)m_Race, SirenColor);
		m_pSiren->LoadRes();
	}
}

void CNkCharacter::Siren_On(int color)
{
	CreateSiren(color);
}

void CNkCharacter::Siren_Off(int color)
{
	SAFE_DELETE(m_pSiren);

	SetSirenOff( color );
	int sirenColor = GetSirenColor();
	if( sirenColor != -1 )
		CreateSiren(sirenColor);
}

void CNkCharacter::Siren_Switch(int switch_flag, int color)
{
	switch(switch_flag)
	{
	case 0 :		// 끄기
		// 현상수배 플레그
		m_bWanted = false;

		Siren_Off(color);
		break;
	case 1 :		// 켜기
		// 현상수배 플레그
		m_bWanted = true;

		Siren_On(color);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////


void CNkCharacter::SetSirenOn(int color)
{
	switch( color )
	{
	case 0: //red
		m_SirenColorFlag |= 0x02;
		break;
	case 1: // blue
		m_SirenColorFlag |= 0x08;
		break;
	case 2: //Green
		m_SirenColorFlag |= 0x10;
		break;
	case 3: //Yellow
		m_SirenColorFlag |= 0x20;
		break;
	}
}

void CNkCharacter::SetSirenOff(int color)
{
	switch( color )
	{
	case 0: //red
		if( IsSirenRed() )		m_SirenColorFlag ^= 0x02;
		break;
	case 1: // blue
		if( IsSirenBlue() )		m_SirenColorFlag ^= 0x08;
		break;
	case 2: //Green
		if( IsSirenGreen() )	m_SirenColorFlag ^= 0x10;
		break;
	case 3: //Yellow
		if( IsSirenYellow() )	m_SirenColorFlag ^= 0x20;
		break;
	}
}

BOOL CNkCharacter::IsSirenRed()
{
	if( m_SirenColorFlag & 0x02 )
		return TRUE;

	return FALSE;
}
BOOL CNkCharacter::IsSirenBlue()
{
	if( m_SirenColorFlag & 0x08 )
		return TRUE;

	return FALSE;
}
BOOL CNkCharacter::IsSirenGreen()
{
	if( m_SirenColorFlag & 0x10 )
		return TRUE;

	return FALSE;
}
BOOL CNkCharacter::IsSirenYellow()
{
	if( m_SirenColorFlag & 0x20 )
		return TRUE;

	return FALSE;
}

int CNkCharacter::GetSirenColor()
{
	if( IsSirenRed() )
		return 0;

	if( IsSirenBlue() )
		return 1;

	if( IsSirenGreen() )
		return 2;

	if( IsSirenYellow() )
		return 3;

	return -1;
}
//////////////////////////////////////////////////////////////////////////

CNkCharacter::~CNkCharacter()
{
	if (m_pClrAlloc)	// 할당받은 컬라값을 해제한다.
		m_pClrAlloc->ColorFree(m_wColorIdx);

	// 관련된 효과를 없앤다.
	set<int>::iterator it;
	it = m_listEffect.begin();
	int idx;
	while (it != m_listEffect.end())
	{
		idx = *it;
		pCMyApp->m_pFxSet->DeleteEffect(idx, FALSE);
		it++;
	}

	m_pGoodShadow = NULL;
	RemoveAll();
	SAFE_DELETE_ARRAY( m_pstrName );
	SAFE_DELETE_ARRAY( m_pstrGuildName );

	SAFE_DELETE(m_pSiren);

	if (m_bHeroAttCome)
		pCMyApp->SetHeroAttGoCha(NULL);

	SAFE_DELETE(m_pChatPopup);
	SAFE_DELETE(m_pUserShopPopup); ///개인상점

	SAFE_DELETE( m_pDropStar );
	// nate 2004 - 4 : Crash_Miss
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
		SAFE_DELETE( m_pCrash_MissPopup[ i ] );

	SAFE_DELETE( m_pRegenbar );

	SAFE_DELETE( m_pGiftMessage );
	SAFE_DELETE_ARRAY( m_NkCha_Particle ); // 배열 딜리트로 바꿈.(배열로 new 하고 그냥 delete만 하고있었다. 이렇게 하면 메모리 누수 나지않는가?) (06-11-20 원석)
	SAFE_DELETE_ARRAY( m_pstrGuildGradeName );
	DeleteMyGuildData();

	for( int i = 0; i < WEARING_NUM; i++ )
		SAFE_DELETE(m_EventWearing[i]);
}

void CNkCharacter::InitChatPopup(int width, int height)
{
	if( !g_pDisplay || !GET_D3DDEVICE() )
		return;

	if( m_pChatPopup )
		m_pChatPopup->LoadRes(width, height);
	if( m_pUserShopPopup ) ///개인상점
		m_pUserShopPopup->LoadRes(width, height);


	// nate 2004 - 4
	// Crash_Miss
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
	{
		if( m_pCrash_MissPopup[ i ] )
			m_pCrash_MissPopup[ i ]->LoadRes(width, height );
	}
	// nate 2004 - 5 : CastleWar
	if( m_pGiftMessage )
		m_pGiftMessage->LoadRes( width, height );
}

// nate 2005-11-23 : 함수 설명 - 이펙트 셋팅
// 주의 : 마우스 클릭과 동시에 이펙트가 발동하는 스킬만 추가
// 참고 : 일정 동작 프레임 중에 이펙트 추가되는것은 bpdilst.dta에서 공격프레임을 이용할것
void CNkCharacter::SetMagicCha(CNkCharacter *pCha, int magicNum)
{
	if( !pCha )
		return;

	if (magicNum != m_MagicNum || m_MagicChaCount > 5)
		m_MagicChaCount = 0;

	m_MagicChaId[m_MagicChaCount++] = pCha->m_nCharIndex;
	m_MagicMobId[0] =  -1;
	m_MagicMobCount = 0;

	m_MagicNum = -1;

	if (magicNum == SKILL_DROP_ATTACK
			|| magicNum == SKILL_ROTATE_ATTACK
			|| magicNum == SKILL_FORCE_ATTACK )
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
	}
	//------------------------------------------------------------------------
	//중고렙 스킬 추가	(불칸 헬크라이)
	else if(magicNum == SKILL_HELL_CRY)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_HELL_CRY);
	}
	//------------------------------------------------------------------------
	// 배쉬
	else if(magicNum == SKILLF_A_BASH)
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_BASH);
	}
	//------------------------------------------------------------------------
	// 더블 슬래쉬
	else if(magicNum == SKILLF_A_DSLASH)
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_DSLASH);
	}
	//------------------------------------------------------------------------
	// 어썰트
	else if (magicNum == SKILLF_A_ASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_ASSAULT);
	}
	//------------------------------------------------------------------------
	// 듀얼 어썰트
	else if (magicNum == SKILLF_C_DASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_DASSAULT);
		// 플레이어가 아니라면
		if(this!=g_pRoh )
		{
			m_MagicMobCount = -1;
			m_AttackChaId = pCha->m_nCharIndex;
			m_AttackMobId = -1;
			Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_ASSAULT);
		}
	}
	//------------------------------------------------------------------------
	// nate 2005-06-21 : Burning Crash - Set Attack
	else if( magicNum == SKILL_B_BURNINGCRASH )
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack( pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum );
	}
	// nate 2005-06-21 : Auto Mine - attack, insertMagic
	else if( magicNum == SKILL_H_AUTOMINE )	// 오토마인
	{
		if( m_nSkillLevel > 0 )				// 스킬레벨이 0 이상
		{
//			if( g_pRoh == this ) // 오토마인의 경우 렉 발생시 중복사용이 되는것땜에 사용시 쿨타임돌리기로 바꾸었다. 07-03-22 원석
//				G_SK_SetCooltime( SKILL_H_AUTOMINE );	// CoolTime Setting
			m_MagicChaCount = 1;
			m_AttackChaId = pCha->m_nCharIndex;
			m_AttackMobId = -1;
			m_MagicNum = magicNum;
			Attack( pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum );
			pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pCha, NULL, EFFECT_SKILL_MINE );
		}
	}
	//mungmae-2005/07/15 헬다운 추가
	else if ( magicNum == SKILL_C_HELLDOWN )
	{
		if( g_pRoh != this && m_nSkillStep == -1 )
			SetMotionState( CHA_STAND );
		else
		{
			Attack( pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum );
			pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pCha, NULL, EFFECT_SKILL_HELLDOWN );
		}
		m_MagicMobCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_MagicNum = magicNum;
		m_bAttacked = FALSE;
		m_AttackMobId = -1;
	}
	else if( magicNum == SKILL_F_DASHSLASH ) ///하이콤보
	{
		// 대쉬 슬래쉬는 여기선 그냥 타격 이펙트만 넣어준다.(자기 자신껀 여기서 넣지 않는다.처리상 이유로 분리했음)
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, pCha, NULL, EFFECT_SKILL_DSLASH );
	}
	else if( magicNum == SKILL_F_SLASHSHADOW ) ///하이콤보
	{
		this->Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum ); // 어택처리를 해준다.
	}
	else if( magicNum == SKILL_F_STONEBASH2 ) ///하이콤보
	{
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, pCha, NULL, EFFECT_SKILL_STONE_LANCE );
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, pCha, NULL, EFFECT_SKILL_BURNINGCRASH2 );

		// 타격 이펙트 추가
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_HORNBOOM;
		ef_sort.vPos = D3DVECTOR( pCha->m_wx, pCha->m_wy + 12.0f, pCha->m_wz );
		ef_sort.Index = HIT_GRYPHON;
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));


		// 바라보는 방향을 디바인스톤이 되게 하기 위해서...
		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_MagicChaCount = -1;
		m_AttackMobId = this->m_resmobidx; // 자신의 디바인스톤을 향하도록.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT1 ) ///하이콤보
	{
		this->SetMotionState( CHA_F_BASHREFLECT ); // 모션을변경.
		this->SetShowCrash_Miss( 10 ); // 콤보성공 이펙트(강) 출력.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, NULL, EFFECT_SKILL_SHADOWSLASH );

		this->m_nComboFrame = 1; // 스킬의 phase에 사용.

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_MagicChaCount = -1;
		m_AttackMobId = this->m_resmobidx; // 자신의 디바인스톤을 향하도록.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT2 ) ///하이콤보
	{
		// 분신 이펙트를 삽입.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pCha, NULL, EFFECT_SKILL_BASHREFLECTION );

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_MagicChaCount = -1;
		m_AttackMobId = this->m_resmobidx; // 자신의 디바인스톤을 향하도록.....
		m_AttackChaId = -1;
	}
	else if(magicNum == SKILL_F_SPACESLASH)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SPACE_SLASH);
	}
	else if(magicNum == SKILL_F_ELECTRICHAMMER)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_ELECTRIC_HAMMER);
	}
	else if(magicNum == SKILL_K_FLAMESPEAR )
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_FLAMESPEAR);
	}
	else if(magicNum == SKILL_H_FROZENSHOT )
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_FROZEN_SHOT);
	}
	else if(magicNum == SKILL_A_SUMMONFIRE )
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_SUMMONFIRE );
	}
	else if(magicNum == SKILL_H_RANDOMFIRE )
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_RANDOMFIRE );
	}
	else if(magicNum == SKILL_F_MERCERNERY_SLASH )
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_MERCERNERYSLASH );
	}
	else if( magicNum == SKILL_K_ICEBOLT )
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_ICEBOLT );
	}
	else if( magicNum == SKILL_B_WILDSMASH )
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_WILDSMASH );
	}
	else if( magicNum == SKILL_A_RESURRECTION )
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
	}
	else if( magicNum == SKILL_F_TORNADO_HAMMER )
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_TORNADO_HAMMER );
	}
	else if( magicNum == SKILL_AIR_RAID )
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_AIR_RAID );
	}
	else if(magicNum == PEROM_SKILL_S127)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S127);
	}
	else if(magicNum == PEROM_SKILL_S128)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S128);
	}
	else if(magicNum == PEROM_SKILL_S129)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S129);
	}
	else if(magicNum == PEROM_SKILL_S130)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S130);
	}
	else if(magicNum == PEROM_SKILL_S131)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S131);
	}
	else if(magicNum == PEROM_SKILL_S132)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S132);
	}
	else if(magicNum == PEROM_SKILL_S133)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S133);
	}
	else if(magicNum == PEROM_SKILL_S134)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S134);
	}
	else if(magicNum == PEROM_SKILL_S135)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S135);
	}
	else if(magicNum == PEROM_SKILL_S136)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S136);
	}
	else if(magicNum == PEROM_SKILL_S137)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S137);
	}
	else if(magicNum == PEROM_SKILL_S138)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S138);
	}
	else if(magicNum == PEROM_SKILL_S139)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S139);
	}
	else if(magicNum == PEROM_SKILL_S140)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S140);
	}
	else if(magicNum == PEROM_SKILL_S141)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S141);
	}
	else if(magicNum == PEROM_SKILL_S148)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S148);
	}
	else if(magicNum == PEROM_SKILL_S149)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S149);
	}
	else if(magicNum == PEROM_SKILL_S150)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_PEROM_SKILL_S150);
	}
	else
	{
		m_MagicNum = magicNum;
		Magic(pCha->m_wx, pCha->m_wy, pCha->m_wz, CHA_MAGIC_SOFT);
	}
}

void CNkCharacter::AddMagicCha(CNkCharacter *pCha)
{
	if( pCha )
		m_MagicChaId[m_MagicChaCount++] = pCha->m_nCharIndex;
}

void CNkCharacter::SetChaCondition(int Condition, int cancel)
{
	//후위 이펙트 표현
	if (m_CancelState != FALSE &&
			m_ChaCondition != 0 )
	{
		m_ChaCondition2 = m_ChaCondition;
		m_CancelState2 = TRUE;
	}
	m_ChaCondition = Condition;
	m_CancelState = cancel;
}

void CNkCharacter::SetChaCondition2(int Condition, int cancel)
{
	m_ChaCondition2 = Condition;
	m_CancelState2 = cancel;
}

// nate 2005-11-23 : 함수 설명 - 이펙트 셋팅
// 주의 : 마우스 클릭과 동시에 이펙트가 발동하는 스킬만 추가
// 참고 : 일정 동작 프레임 중에 이펙트 추가되는것은 bpdilst.dta에서 공격프레임을 이용할것
void CNkCharacter::SetMagicMob(CNkMob *pMob, int magicNum)
{
	if( !pMob )
		return;

	if (magicNum != m_MagicNum || m_MagicMobCount > 5)
		m_MagicMobCount = 0;

	m_MagicMobId[m_MagicMobCount++] = pMob->m_nVirId;
	m_MagicChaId[0] = -1;
	m_MagicChaCount = 0;

	m_MagicNum = -1;

	// 내려찍기 돌려베기 포스어택
	if (magicNum == SKILL_DROP_ATTACK || magicNum == SKILL_ROTATE_ATTACK || magicNum == SKILL_FORCE_ATTACK )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
	}
	//------------------------------------------------------------------------
	// 헬크라이
	else if(magicNum == SKILL_HELL_CRY)
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_HELL_CRY);
	}
	//------------------------------------------------------------------------
	// 서먼피션
	else if(magicNum == SKILL_SUMMON_PARTITION)
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_SUMMON_PARTITION);
	}
	//------------------------------------------------------------------------
	// 배쉬
	else if(magicNum == SKILLF_A_BASH)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_BASH);
	}
	//------------------------------------------------------------------------
	// 더블 슬래쉬
	else if(magicNum == SKILLF_A_DSLASH)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_DSLASH);
	}
	//------------------------------------------------------------------------
	// 어썰트
	else if (magicNum == SKILLF_A_ASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_ASSAULT);
	}
	//------------------------------------------------------------------------
	// 듀얼 어썰트
	else if (magicNum == SKILLF_C_DASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_DASSAULT);
		// 플레이어가 아니라면
		if(this!=g_pRoh )
		{
			m_MagicMobCount = -1;
			m_AttackChaId = -1;
			m_AttackMobId = pMob->m_nVirId;
			Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_ASSAULT);
		}
	}
	// nate 2005-06-21 : Burning Crash - Set Attack
	else if( magicNum == SKILL_B_BURNINGCRASH )
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
	}
	// nate 2005-06-21 : Auto Mine - attack, insertMagic
	else if( magicNum == SKILL_H_AUTOMINE )	// 오토 마인
	{
		if( m_nSkillLevel > 0 )				// 스킬레벨이 0 이상
		{
//			if( g_pRoh == this ) // 오토마인의 경우 렉 발생시 중복사용이 되는것땜에 사용시 쿨타임돌리기로 바꾸었다. 07-03-22 원석
//				G_SK_SetCooltime( SKILL_H_AUTOMINE );	// CoolTime Setting

			m_MagicMobCount = 1;
			m_AttackChaId = pMob->m_nVirId;
			m_AttackMobId = -1;
			m_MagicNum = magicNum;
			Attack( pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum );
			pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, pMob, EFFECT_SKILL_MINE );
		}
	}
	//mungmae-2005/07/13 :
	else if ( magicNum == SKILL_C_HELLDOWN )
	{
		//if( g_pRoh != this && m_nSkillStep == -1 )
		//	SetMotionState( CHA_STAND );
		//else
		{
			Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_HELLDOWN );
		}
		m_MagicMobCount = -1;
		m_AttackMobId = pMob->m_nVirId;
		m_MagicNum = magicNum;
		m_bAttacked = FALSE;
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_DASHSLASH ) ///하이콤보
	{
		// 대쉬 슬래쉬는 여기선 그냥 타격 이펙트만 넣어준다.(자기 자신껀 여기서 넣지 않는다.처리상 이유로 분리했음)
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, pMob, EFFECT_SKILL_DSLASH );
	}
	else if( magicNum == SKILL_F_SLASHSHADOW ) ///하이콤보
	{
		this->Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum ); // 어택처리를 해준다.
	}
	else if( magicNum == SKILL_F_STONEBASH1 ) ///하이콤보
	{
		this->SetMotionState( CHA_F_STONEBASH ); // 모션 셋팅.
		this->SetShowCrash_Miss( 5 ); // 콤보성공 이펙트(약) 출력.
		this->m_nComboFrame = 0; // 스톤배쉬의 단계를 초기화.

		if( pMob->m_nVirId == this->m_resmobidx ) // 자기 자신의 디바인스톤에 스톤배쉬로 오면..
		{
			// 콤보스킬 체크용 틱을 설정.
			this->m_dwComboTick = timeGetTime();
			this->m_bComboSucceed = true;
			pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // 이걸 해줘야 한 애니메이션이 끝나후 멈춰준다.
		}
	}
	else if( magicNum == SKILL_F_STONEBASH2 ) ///하이콤보
	{
		// 스톤배쉬는 여기선 그냥 타격 이펙트만 넣어준다.
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, pMob, EFFECT_SKILL_STONE_LANCE );
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, pMob, EFFECT_SKILL_BURNINGCRASH2 );

		// 타격 이펙트 추가
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_HORNBOOM;
		ef_sort.vPos = D3DVECTOR( pMob->m_wx, pMob->m_wy + 12.0f, pMob->m_wz );
		ef_sort.Index = HIT_GRYPHON;
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));


		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_AttackMobId = this->m_resmobidx; // 자신의 디바인스톤을 향하도록.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT1 ) // 모션 패킷 ///하이콤보
	{
		this->SetMotionState( CHA_F_BASHREFLECT ); // 모션을변경.
		this->SetShowCrash_Miss( 10 ); // 콤보성공 이펙트(강) 출력.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, NULL, EFFECT_SKILL_SHADOWSLASH );

		this->m_nComboFrame = 1; // 스킬의 phase에 사용.

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_AttackMobId = this->m_resmobidx; // 자신의 디바인스톤을 향하도록.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT2 ) // 타겟 패킷 ///하이콤보
	{
		// 분신 이펙트를 삽입.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, pMob, EFFECT_SKILL_BASHREFLECTION );

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_AttackMobId = this->m_resmobidx; // 자신의 디바인스톤을 향하도록.....
		m_AttackChaId = -1;
	}
	else if(magicNum == SKILL_F_SPACESLASH )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SPACE_SLASH);
	}
	else if(magicNum == SKILL_F_ELECTRICHAMMER)
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_ELECTRIC_HAMMER);
	}
	else if(magicNum == SKILL_K_FLAMESPEAR )
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_FLAMESPEAR);
	}
	else if(magicNum == SKILL_H_FROZENSHOT )
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_FROZEN_SHOT);
	}
	else if(magicNum == SKILL_A_SUMMONFIRE )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_SUMMONFIRE );
	}
	else if(magicNum == SKILL_H_RANDOMFIRE )
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_RANDOMFIRE );
	}
	else if(magicNum == SKILL_F_MERCERNERY_SLASH )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_MERCERNERYSLASH );
	}
	else if(magicNum == SKILL_K_ICEBOLT )
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_ICEBOLT );
	}
	else if( magicNum == SKILL_B_WILDSMASH )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_WILDSMASH );
	}
	else if( magicNum == SKILL_A_RESURRECTION )
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
	}
	else if( magicNum ==SKILL_F_TORNADO_HAMMER )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_TORNADO_HAMMER );
	}
	else if(magicNum == SKILL_AIR_RAID )
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_AIR_RAID);
	}
	else if(magicNum == PEROM_SKILL_S127 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S127 );
	}
	else if(magicNum == PEROM_SKILL_S128 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S128 );
	}
	else if(magicNum == PEROM_SKILL_S129 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S129 );
	}
	else if(magicNum == PEROM_SKILL_S130 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S130 );
	}
	else if(magicNum == PEROM_SKILL_S131 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S131 );
	}
	else if(magicNum == PEROM_SKILL_S132 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S132 );
	}
	else if(magicNum == PEROM_SKILL_S133 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S133 );
	}
	else if(magicNum == PEROM_SKILL_S134 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S134 );
	}
	else if(magicNum == PEROM_SKILL_S135 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S135 );
	}
	else if(magicNum == PEROM_SKILL_S136 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S136 );
	}
	else if(magicNum == PEROM_SKILL_S137 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S137 );
	}
	else if(magicNum == PEROM_SKILL_S138 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S138 );
	}
	else if(magicNum == PEROM_SKILL_S140 )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_PEROM_SKILL_S140 );
	}
	else
	{
		m_MagicNum = magicNum;
		Magic(pMob->m_wx, pMob->m_wy, pMob->m_wz, CHA_MAGIC_SOFT);
	}

	if (g_pRoh == this && pCMyApp->m_pMyPet)
		if( pMob->m_nMobNum != 226 )
			pCMyApp->m_pMyPet->SetCombatMob(pMob->m_nVirId);
}

void CNkCharacter::AddMagicMob(CNkMob *pMob)
{
	if( pMob )
		m_MagicMobId[m_MagicMobCount++] = pMob->m_nVirId;
}

void CNkCharacter::SetAttackChaId(int chaId, BOOL bPetAttack ) // 펫도 공격시킬지 플래그를 추가. (일부 스킬은 공격을 안시키게 하기위해서..) 06-01-31 원석
{
	m_AttackChaId = chaId;
	m_AttackMobId = -1;

	if (g_pRoh == this && pCMyApp->m_pMyPet && bPetAttack ) // 펫도 공격시킬지 플래그를 추가. (일부 스킬은 공격을 안시키게 하기위해서..) 06-01-31 원석
		pCMyApp->m_pMyPet->SetCombatMob(-1, m_AttackChaId);
}

void CNkCharacter::SetAttackMobId(int mobId)
{
	m_AttackMobId = mobId;
	m_AttackChaId = -1;

	if (g_pRoh == this && pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetCombatMob(m_AttackMobId);
}

void CNkCharacter::SetAttackVital(int vital)
{
	m_Vital = m_AttackVital;

	m_AttackVital = vital;
	m_LastAttackVitalTime = timeGetTime();
}

void CNkCharacter::Attacked()
{
	//mungmae -2005/08/01 에이디아 생명 증폭이 걸려 있으면 공격 받는 걸로 인식하는거 수정
	if( m_nAttVital > m_Vital )
//		&& !m_byShowLIfeExpantion) ///07-04-10 원석 수정. 생명 증폭이 이 함수에 들어오는거 부터가 잘못이다. 이펙트에서 이함수 호출하는거 없앴다. 이거땜에 생명증폭 걸려있으면 헬다운 취소가 안되자나 -_-;
		m_bAttacked = TRUE;
	else
		m_bAttacked = FALSE;


	m_nAttVital = m_Vital;
	m_Vital = m_AttackVital;

	if (m_Vital <= 0)
		Kill();
}

#define THROW_SLAYER_FRAME_START	4
#define THROW_SLAYER_FRAME_END		26
#define THROW_CHAKRAM_FRAME_START	5
#define THROW_CHAKRAM_FRAME_END		27
#define DSTRONE_FRAME_START			4	//- Lyul 2005
#define DSTRONE_SLAYER_FRAME_END	26  //- Lyul 2005

//--------------------------------------------------------------------------
// Framemove
//--------------------------------------------------------------------------
void CNkCharacter::FrameMove(unsigned int nInc, BOOL bVisible, CNkMob *pTargetMob, CNkCharacter *pTargetCha)
{
	int i = 0;
	static EffectSort ef_sort;
	static D3DVECTOR vTmp;
	static D3DMATRIX matRot;
	static DWORD	dwGunFxColor, dwGunFxAttackColor;

	ZeroMemory(&ef_sort, sizeof(EffectSort));

	m_DrawWeaponEffects = NONE;

	if (bVisible)
	{
		for (i=0; i < CAPSYONG_NUM ; i++)
		{
			if (m_bShow[i] && m_CapSyong[i])
			{
				if( g_bUseEventWear )
				{
					switch( i )
					{
					case NKCHA_HAIR:

						if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag  ) // 우선은 이벤트 장비가 있는지 체크.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_HELMET] ) // 이벤트 장비가 없다면 일반 장비를 체크.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // 둘다 안착용하고 있다면...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_FACE:

						if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag ) // 우선은 이벤트 장비가 있는지 체크.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_HELMET] ) // 이벤트 장비가 없다면 일반 장비를 체크.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // 둘다 안착용하고 있다면...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_BREAST:

						if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag ) // 우선은 이벤트 장비가 있는지 체크.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_ARMOR] ) // 이벤트 장비가 없다면 일반 장비를 체크.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // 둘다 안착용하고 있다면...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_ARMS:

						if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag ) // 우선은 이벤트 장비가 있는지 체크.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_ARMOR] ) // 이벤트 장비가 없다면 일반 장비를 체크.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // 둘다 안착용하고 있다면...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_LEGS:

						if( m_EventWearing[WEARING_PANTS] && m_bEventWearFlag ) // 우선은 이벤트 장비가 있는지 체크.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_PANTS] ) // 이벤트 장비가 없다면 일반 장비를 체크.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // 둘다 안착용하고 있다면...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_FEET: // 이녀석은 조건이 좀더 복잡하다.

						if( (m_EventWearing[WEARING_PANTS] || m_EventWearing[WEARING_SHOES] ) && m_bEventWearFlag) // 이벤트 복장이 있는경우.
						{
							if( m_EventWearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								break; // 안그리는 조건.
							}

							if( m_EventWearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								break; // 안그리는 조건.
							}

							// 이벤트 복장을 입고 있는데 그리는 조건인 경우다.
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}
						else if( m_Wearing[WEARING_PANTS] || m_Wearing[WEARING_SHOES] ) // 이벤트 복장이 둘다 없는 경우 일반 복장을 체크한다.
						{
							if( m_Wearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								break; // 안그리는 조건.
							}

							if( m_Wearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
							{
								break; // 안그리는 조건.
							}

							// 이벤트 복장을 입고 있는데 그리는 조건인 경우다.
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}
						else // 아무것도 안입고있어도 그린다.
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					}
				}
				else
				{

					if ((i == NKCHA_HAIR && (m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR)) ||
							(i == NKCHA_FACE && (m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE)) ||
							(i == NKCHA_BREAST && (m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY)) ||
							(i == NKCHA_ARMS && (m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS)) ||
							(i == NKCHA_LEGS && (m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS)) ||
							(i == NKCHA_FEET && (m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET)) ||
							(i == NKCHA_FEET && (m_Wearing[WEARING_SHOES] && GET_RENDER_FLAG(m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET)) )
					{
						// Animate 안한다...
					}
					else
						m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
				}
			}
		}

		//------------------------------------------------------------------
		// Frame Move 에서 무기 제거
		for (i = 0; i < WEARING_SHOW_NUM; i++)
		{
			// 무기가 있다면
			if (m_Wearing[i]/*.m_ProtoNum >= 0*/ && GET_CSY(m_Wearing[i], m_BpdRace))
			{
				//--------------------------------------------------------------
				// 슬레이어
				if (i == WEARING_CLOAK && m_nMotionState == CHA_THROW_SLAYER &&
						m_CurFrame >= THROW_SLAYER_FRAME_START && m_CurFrame <= THROW_SLAYER_FRAME_END)
				{
					// SubCsy 를 Frame move 한다(등에 찬 것 대신  손에 듣 것을 Animate )
					if (GET_SUB_CSY(m_Wearing[i]))
						GET_SUB_CSY(m_Wearing[i])->Animate(m_Biped, m_CurFrame);
				}
				// nate 2005-06-10 : Burning Crash - 캡숑 에니메이션
				else if (i == WEARING_CLOAK && m_nMotionState == CHA_B_BURNINGCRASH )
				{
					// SubCsy 를 Frame move 한다(등에 찬 것 대신  손에 듣 것을 Animate )
					if (GET_SUB_CSY(m_Wearing[i]))
						GET_SUB_CSY(m_Wearing[i])->Animate(m_Biped, m_CurFrame);
				}
				//--------------------------------------------------------------
				// 디바인 스톤 - Lyul_2005
				else if (i == WEARING_CLOAK && m_nMotionState == CHA_F_DSTONE)
				{
					// 손에 든 것을 그림
					if(m_CurFrame >= 8 && m_CurFrame <= DSTRONE_SLAYER_FRAME_END && !m_bstone)
					{
						// SubCsy 를 Frame move 한다(등에 찬 것 대신  손에 듣 것을 Render )
						if (GET_SUB_CSY(m_Wearing[i]))
							GET_SUB_CSY(m_Wearing[i])->Animate(m_Biped, m_CurFrame);

						//----------------------------------------------------------
						// 땅에 박히는 순간.
						if(m_CurFrame > 17)
						{
							// Add mob
							if(m_bresstone && m_resmobidx)
							{
								m_bstone = true; // Item 은 더이상 그리지 않게 설정
								m_bresstone = false; // 예약된 몹 생성 완료
								pCMyApp->m_pUIMgr->AddMob(m_resmobvnum, m_resmobidx, m_resmobx, m_resmobz,m_resmobdir,1,0,0,9);

								// Effect
								CNkMob *pMob = pCMyApp->m_pUIMgr->FindMobById(m_resmobidx);

								if(pMob)
									pCMyApp->m_pUIMgr->InsertMagic(NULL, pMob, NULL, NULL, EFFECT_DSTONE_EFF);

								if( this == g_pRoh ) // 자기 자신이면 /// 하이콤보
									m_dwComboTick = timeGetTime(); // 콤보 틱을 설정.
							}
						}
					}
					// 등에 찬 것을 그림
					else if(!m_bstone && GET_CSY(m_Wearing[i], m_BpdRace))
						GET_CSY(m_Wearing[i], m_BpdRace)->Animate(m_Biped, m_CurFrame);
				}
				// 일반 Frame Move
				else if( GET_CSY(m_Wearing[i], m_BpdRace) && !EventWearingFrameMove(i) ) ///이벤트복장착용 을 하고 있는지 여부도 확인해주고 착용하고있으면 이벤트 복장을 애니메이트 시켜준다.
					GET_CSY(m_Wearing[i], m_BpdRace)->Animate(m_Biped, m_CurFrame);
			}
			else if( m_EventWearing[i] && m_bEventWearFlag ) // 메인 장비를 안차고있어도 이벤트 복장을 착용하고있는 경우를 체크한다. ///이벤트복장착용
			{
				EventWearingFrameMove(i);
			}
			else if (m_Race == RACE_AIDIA && i == WEARING_CLOAK && g_BasicWingsProto >= 0)
			{
				if( GET_CSYC(g_BasicWingsProto,m_BpdRace) )
					GET_CSYC(g_BasicWingsProto,m_BpdRace)->Animate(m_Biped, m_CurFrame);
			}
		}
	}

	if(m_nMotionState == CHA_THROW_CHAKRAM )
	{
		if(this !=g_pRoh && m_CurFrame >= THROW_CHAKRAM_FRAME_START && m_CurFrame <= THROW_CHAKRAM_FRAME_END)
		{
			int proto_num = g_ItemList.FindItem(693);

			if (GET_SUB_CSYC(proto_num) && (proto_num >= 0))
				GET_SUB_CSYC(proto_num)->Animate(m_Biped, m_CurFrame);
		}
		else if(pCMyApp->m_pUIMgr && pCMyApp->m_pUIMgr->m_ChakramSlotNum >= 0 &&
				m_CurFrame >= THROW_CHAKRAM_FRAME_START && m_CurFrame <= THROW_CHAKRAM_FRAME_END)
		{
			// 차크람 공격이닷! 등에 찬것 그리지 말고 손에 든걸로 그려랏!
			// 2005.05.11
			// Lyul / 수정 완료
			if (m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]) // NULL Check
				if (GET_SUB_CSY(m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]))
					GET_SUB_CSY(m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum])->Animate(m_Biped, m_CurFrame);
		}
	}

	///1021 광선검..지잉~~
	if(m_bCombat && m_nMotionState != CHA_SOCIAL &&
			m_nMotionState != CHA_SWIM && m_nMotionState != CHA_SWIM_STAND &&
			m_nMotionState != CHA_THROW_SLAYER && m_nMotionState != CHA_THROW_CHAKRAM &&
			m_Wearing[WEARING_SHIELD] &&
			GET_CSY(m_Wearing[WEARING_SHIELD], m_BpdRace))
	{
		if (GET_CSY(m_Wearing[WEARING_SHIELD], m_BpdRace)
				&& (m_Wearing[WEARING_SHIELD]->m_Vnum==773 || m_Wearing[WEARING_SHIELD]->m_Vnum==1023))
			GET_CSY(m_Wearing[WEARING_SHIELD], m_BpdRace)->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_LIGHTSWORD,
					&m_matWorld, &(pCMyApp->m_matView));
	}

	//----------------------------------------------------------------------
	// WEARING_WEAPON Frame Move
	//----------------------------------------------------------------------
	// nate 2005-06-15 : Burning Crash - 이펙트
	if( m_bCombat
			&& m_Wearing[WEARING_CLOAK]
			// nate 2005-06-07 : 버닝 크래쉬 궤적 그리기
			&& GET_SHAPE(m_Wearing[WEARING_CLOAK]) == ISUB_SLAYER
			&& m_nMotionState == CHA_B_BURNINGCRASH )
	{
		// nate 2005-06-10 : Burning Crash - 불 이펙트
		if( m_nMotionState == CHA_B_BURNINGCRASH
				&& GET_SUB_CSY( m_Wearing[ WEARING_CLOAK ] )
				&& (( m_CurFrame >= 72 && m_CurFrame <= 84 )
					|| m_CurFrame >= 112 ) )
			GET_SUB_CSY( m_Wearing[ WEARING_CLOAK ] )->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_BLEED
					,&m_matWorld, &(pCMyApp->m_matView));
		// nate 2005-06-10 : Burning Crash - 궤적
		int nSkillLevel = ( this == g_pRoh )?g_ChaSkill.m_SkillLevel[ 16 ]:m_nSkillLevel;
		if((( nSkillLevel - 1 ) / 5 ) >= 3 )	// 16 : 불칸 16번째 스킬( 버닝 크래쉬 )
			SetTraceLineLVertex( TRACELINE_BURNINGCRASH );
	}
	else if (m_bCombat
			&& m_bEventWearFlag
			&& m_nMotionState != CHA_SOCIAL
			&& m_nMotionState != CHA_SWIM
			&& m_nMotionState != CHA_SWIM_STAND
			&& m_nMotionState != CHA_THROW_SLAYER
			&& m_nMotionState != CHA_THROW_CHAKRAM
			&& m_nMotionState != CHA_F_DSTONE		// Lyul_2005 디바인스톤일 경우에도 숨긴다.						
			&& m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : 일렉트릭 샷
			&& m_EventWearing[WEARING_WEAPON]
			&& GET_CSY( m_EventWearing[WEARING_WEAPON], m_BpdRace ) )
	{
		if (GET_SHAPE(m_EventWearing[WEARING_WEAPON]) == IWEAPON_STAFF)
			GET_CSY(m_EventWearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, FALSE, LINE_EFF_BRIGHT, &m_matWorld, &(pCMyApp->m_matView));
		else
			GET_CSY(m_EventWearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame);
	}
	else if (m_bCombat
			 && m_nMotionState != CHA_SOCIAL
			 && m_nMotionState != CHA_SWIM
			 && m_nMotionState != CHA_SWIM_STAND
			 && m_nMotionState != CHA_THROW_SLAYER
			 && m_nMotionState != CHA_THROW_CHAKRAM
			 && m_nMotionState != CHA_F_DSTONE		// Lyul_2005 디바인스톤일 경우에도 숨긴다.
			 && m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : 일렉트릭 샷
			 && m_Wearing[WEARING_WEAPON]
			 && GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace))
	{
		//=================================================
		// nate 2004 - 11 ; new race - 이도류 쌍칼 관련
		if(GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) )
		{
			// 왼쪽 검
			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_BLEED,
						&m_matWorld, &(pCMyApp->m_matView));
			// 오른쪽 검
			GET_SUB_CSY(m_Wearing[WEARING_WEAPON])->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_BLEED,
					&m_matWorld, &(pCMyApp->m_matView));

			// 이도류의 좌/우를 판단하여 궤적을 남긴다.
			if (m_nMotionState == CHA_ATTACK)
			{
				if(m_CurFrame < 11) // 9frame 까지는 오른쪽 무기
					SetTraceLineLVertex(TRACELINE_ATTACK,0,false);
				// 오른 쪽 검의 공격 이펙트는 엔진에서 지원하지 못하므로 강제적으로 설정한다.
				else
				{
					SetTraceLineLVertex(TRACELINE_ATTACK,0,true);

					CNkMob *pMob = NULL;
					pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
					pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_DSWORD);
				}
			}
			else if( m_nMotionState == CHA_F_DASHSLASH ) ///하이콤보
			{
				SetTraceLineLVertex( TRACELINE_DASHSLASH ); // 이 녀석은 이도류지만 안에서 자동으로 양쪽 검껄 생성해준다.
			}
			else if( m_nMotionState == CHA_F_SLASHSHADOW ) ///하이콤보
			{
				SetTraceLineLVertex( TRACELINE_SLASHSHADOW ); // 이 녀석은 이도류지만 안에서 자동으로 양쪽 검껄 생성해준다.
			}
		}
		else if (m_Wearing[WEARING_WEAPON]->m_Vnum==773 || m_Wearing[WEARING_WEAPON]->m_Vnum==1023)
		{
			//1008 광선검
			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_LIGHTSWORD,
						&m_matWorld, &(pCMyApp->m_matView));
		}
		// 칼 궤적을 동적 생성한다.
		// Lyul_2005 해머 추가.
		else if ( bVisible
				  && ( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD
					   || GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE
					   || GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD
					   || GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER ) )
		{
			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, FALSE, LINE_EFF_BRIGHT
						,&m_matWorld, &(pCMyApp->m_matView));
			if (m_nMotionState == CHA_ATTACK)
				SetTraceLineLVertex( TRACELINE_ATTACK );
			else if ( m_nMotionState == CHA_JUMP_ATTACK
					  || m_nMotionState == CHA_ROTATE_ATTACK )
				SetTraceLineLVertex( TRACELINE_JUMPATTACK );
		}
		//--------------------------------------------------------------------
		// 총을 들고 공격하는 Motion
		else if (m_nMotionState == CHA_ATTACK && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN)
		{
			if (g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame <= m_CurFrame &&
					m_CurFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame + 3
					&&( m_Wearing[WEARING_WEAPON]->m_Vnum!=1238 && m_Wearing[WEARING_WEAPON]->m_Vnum!=2172 ))
			{
				//데미크루셔를 들고있을땐 안들어간다.
				if (pTargetMob)
				{
					m_tx = pTargetMob->m_wx, m_ty = pTargetMob->m_wy, m_tz = pTargetMob->m_wz;

					switch (pTargetMob->m_Height)
					{
					case MOB_HEIGHT_HIGH_NUM:
						m_TargetHeight = MOB_HEIGHT_HIGH_VAL;
						break;
					case MOB_HEIGHT_LOW_NUM:
						m_TargetHeight = MOB_HEIGHT_LOW_VAL;
						break;
					default: // MOB_HEIGHT_MIDDLE_NUM:
						m_TargetHeight = MOB_HEIGHT_MIDDLE_VAL;
						break;
					}
				}
				else if (pTargetCha)
				{
					m_tx = pTargetCha->m_wx, m_ty = pTargetCha->m_wy, m_tz = pTargetCha->m_wz;
					m_TargetHeight = MOB_HEIGHT_MIDDLE_VAL;
				}

				// 총의 종류를 확인한다.
				switch(m_Wearing[WEARING_WEAPON]->m_Vnum)
				{
				case 60:		// 블래스터 피스톨
				case 61:		// 블래스터 라이플
					//dwGunFxColor = 0xFFEA601C;
					dwGunFxColor = 0xFFC1C1FE;
					dwGunFxAttackColor = 0xFFC1C1FE;
					break;
				case 203:		// 에어핸드건
				case 55:		// 에어건
				case 204:		// 권총
					dwGunFxColor = 0xFFFFF8B5;
					dwGunFxAttackColor = 0xFFFFF8B5;
					break;
				default:
					dwGunFxColor = 0xFFFFFFFF;
					dwGunFxAttackColor = 0xFFFFFFFF;
					break;
				}

				//----------------------------------------------------------------
				// 총에 맞는 Effect ( 목표물에 나타남)
				// [3/28/2007 Theodoric] 태국 송크란 이벤트 :: 총에 맞는 이펙트 끄기
				if ( (pTargetMob || pTargetCha) &&
						m_Wearing[WEARING_WEAPON]->m_Vnum != 1538 && m_Wearing[WEARING_WEAPON]->m_Vnum != 1539 )
				{
					// 거리를 떨어뜨린다.
					D3DVECTOR vAdj;
					vAdj.x = m_tx - m_wx;
					vAdj.y = m_ty - m_wy;
					vAdj.z = m_tz - m_wz;

					vAdj = Normalize(vAdj);

					ef_sort.nType = FX_SHOOTATTACKED;
					ef_sort.bOneFrame = FALSE;
					ef_sort.nCurFrame = -1;
					ef_sort.vPos = D3DVECTOR(m_tx-vAdj.x*5, m_ty+m_TargetHeight, m_tz-vAdj.z*5);
					ef_sort.dwColor = dwGunFxAttackColor;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				}
				//----------------------------------------------------------------
				// 총에서 목표까지의 괘적을 나타내는 Effect

				// 정확한 방향으로 쏴야쥐~~!
				m_dir = m_toDir = (float) atan2(m_tz - m_wz, m_tx - m_wx);

				// 쏘려는 상대가 없으면, 처음에 저장된, tx 등등의 값을 사용한다.
				float fDist = DistPlToPl(m_wx, m_wz, m_tx, m_tz);
				D3DVECTOR tVector = D3DVECTOR(fDist, (m_ty-m_wy)+m_TargetHeight, 0);
				// 총 괘적.
				if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
					GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, FALSE, LINE_EFF_NONE, &m_matWorld, &(pCMyApp->m_matView),
							&tVector);

				ZeroMemory(&ef_sort, sizeof(EffectSort));

				//----------------------------------------------------------------
				// 총 발사 Effect ( 총구에 나타남)
				// [3/28/2007 Theodoric] 태국 송크란 이벤트 :: 총 발사 이펙트를 끄기
				if( m_Wearing[WEARING_WEAPON]->m_Vnum != 1538 && m_Wearing[WEARING_WEAPON]->m_Vnum != 1539 &&
						GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				{
					ef_sort.nType = FX_SHOOT;

					Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
					D3DMath_VectorMatrixMultiply(vTmp, GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->m_LineVector[0], matRot);
					ef_sort.vPos = vTmp;
					ef_sort.vPos.x += m_wx;
					ef_sort.vPos.y += m_wy;
					ef_sort.vPos.z += m_wz;
					ef_sort.dwColor = dwGunFxColor;
					ef_sort.nCurFrame = m_CurFrame - g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;
					ef_sort.bOneFrame = TRUE;

					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				}
			}
			else if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
		else if (bVisible && GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace))
		{
			if (GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_STAFF)
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, FALSE, LINE_EFF_BRIGHT, &m_matWorld, &(pCMyApp->m_matView));
			else
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
	}

	//  [5/6/2009 ppmmjj83] 일본 아이템중 비전투시 칼을 칼집에 넣은 모양으로 처리해 줘야하는 부분이 있음
	else if( m_Wearing[WEARING_WEAPON]
			 && ( m_Wearing[WEARING_WEAPON]->m_Vnum == 1590 || m_Wearing[WEARING_WEAPON]->m_Vnum == 2694 )
			 && !m_bCombat
		   ) ///일본디자인컨테스트 아이템 중 요도 무라마사를 차고있다면 평화모드에서 허리에 차고있듯이 보이게...
	{
		int ProtoNum = g_ItemList.FindItem( 1712 ); // 무라마사 보조 캡숑용.

		if( GET_CSYC(ProtoNum, m_BpdRace) )
		{
			GET_CSYC(ProtoNum, m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
	}

	//  [5/6/2009 ppmmjj83] 일본 아이템중 비전투시 칼을 칼집에 넣은 모양으로 처리해 줘야하는 부분이 있음
	else if( m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2168 && !m_bCombat ) ///일본디자인컨테스트 아이템 중 요도 무라마사를 차고있다면 평화모드에서 허리에 차고있듯이 보이게...
	{
		int ProtoNum = g_ItemList.FindItem( 2317 ); // 무라마사 보조 캡숑용.

		if( GET_CSYC(ProtoNum, m_BpdRace) )
		{
			GET_CSYC(ProtoNum, m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
	}

	//  [5/6/2009 ppmmjj83] 일본 아이템중 비전투시 칼을 칼집에 넣은 모양으로 처리해 줘야하는 부분이 있음
	else if( m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2339 && !m_bCombat ) ///일본디자인컨테스트 아이템 중 요도 무라마사를 차고있다면 평화모드에서 허리에 차고있듯이 보이게...
	{
		int ProtoNum = g_ItemList.FindItem( 2340 ); // 무라마사 보조 캡숑용.

		if( GET_CSYC(ProtoNum, m_BpdRace) )
		{
			GET_CSYC(ProtoNum, m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
	}

	else if( m_nMotionState == CHA_H_ELECTRICSHOT )		// nate 2005-11-24 : 일렉트릭 샷 - 무기 바꾸기
	{
		if( m_CurFrame > 9 && m_CurFrame < 56 )
		{
			CItem* ElectricShot_Gun = new CItem;
			ZeroMemory( ElectricShot_Gun, sizeof( CItem ) );
			ElectricShot_Gun->m_ProtoNum = g_ItemList.FindItem( 1339 );		// 야마토 건
			if( GET_CSY(ElectricShot_Gun, m_BpdRace) )
				GET_CSY(ElectricShot_Gun, m_BpdRace)->Animate(m_Biped, m_CurFrame, FALSE, LINE_EFF_NONE, &m_matWorld, &(pCMyApp->m_matView) );
		}
	}


	//------------------------------------------------------------------------
	// 사운드 연주
	// 2004.07.20
	// Lyul / 수정완료
	// 총별로 틀린 사운드 연출
	if (bVisible && g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_SoundNum >= 0 && g_pRoh ) ///개인상점 시 작업
	{
		if (m_PrevFrame < m_CurFrame)
		{
			if (m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_SoundFrame &&
					m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_SoundFrame)
			{
				//------------------------------------------------------------------
				// Sound Set
				if (g_pDSound)
				{
					float x, y, z;
					x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
					y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
					z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;

					//----------------------------------------------------------------
					// 무기가 총이라면 각기 다른 Sound 를 적용한다.
					if(m_nMotionState == CHA_ATTACK && m_Wearing[WEARING_WEAPON] &&  GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN)
					{
						switch(m_Wearing[WEARING_WEAPON]->m_Vnum)
						{
						case 204: // 숏건
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_1], x, y, z);
							break;
						case 55: // 에어건
						case 61: // 블래스터라이플
						case 203: // 에어 핸드건
						case 464: // 파워건
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_2], x, y, z);
							break;
						case 60: // 블래스터 피스톨
						case 286: // 머신건
						case 465: // 레인져
						case 474: // 토네이도 라이플
						case 644: // 메트릭스
						case 1066: // 크럼블 건(신무기)
						case 1316:	// 진-매트릭스
						case 1321:	// 진-크럼블 건
						case 1707:	// 진-크럼블 건 (대만5주년)
						case 1716:	// 매트릭스 라이플(대만이벤트용)
						case 2291:	// 견습생의 매트릭스 라이플
						case 2923:	// 사-토네이도 라이플
						case 3285:	// 사-크럼블 건
						case 10457:
						case 10472:
						case 10525:
						case 10541:
						case 10557:
						case 10573:
						case 10706:
						case 10726:

							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_3], x, y, z);
							break;
						case 407: // M-99
						case 467: // 킬러 라이플
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_4], x, y, z);
							break;
						case 287: // 바이오 라이플
						case 466: // 레이져 샷건
						case 2264:	// 견습생의 바이오 라이플
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_5], x, y, z);
							break;
						case 305: // 플라즈마 라이플
						case 408: // 자일로 건
						case 410: // 크립토 라이플
						case 983: // 웨이브 라이플
						case 2274:	// 견습생의 자일로건
						case 2284:	// 견습생의 웨이브 라이플
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_6], x, y, z);
							break;
						case 306: // 파스칼 라이플
						case 409: // 시그마 라이플
						case 2269:	// 겹습생의 파스칼 라이플
						case 2279:	// 견습생의 시그마 라이플
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_7], x, y, z);
							break;
						case 2052:	// 브리오낙
						case 2176:	// 신의 브리오낙
						case 2770:	// 신의 브리오낙(E)
						case 2746:	// 니글링
						case 10465:
						case 10480:
						case 10533:
						case 10549:
						case 10565:
						case 10581:
						case 10842:
						case 11229:
						case 11268:

							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_8], x, y, z);
							break;
						}
					}
					// 총이 아니라면 BPD 에 Set 된 Character 음을 적용한다.
					else
						g_pDSound->Play(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_SoundNum, x, y, z);
				}
			}
			else if (m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame &&
					 m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame)
			{
				// 때리는 순간 때리는 무기의 소리를 낸다.
				if (g_pDSound)
				{
					if (m_nMotionState == CHA_ATTACK || m_nMotionState == CHA_JUMP_ATTACK || m_nMotionState == CHA_ROTATE_ATTACK)
					{
						float x, y, z;
						x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
						y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
						z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;
						if (m_Wearing[WEARING_WEAPON])
						{
							//mungmae-2005/08/29 일본 하리센 사운드 추가
							if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1125 )
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_HARISEN], x, y, z);
							}
							else if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1280 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1471 ) ///반짝반짝망치 추가
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_PIKOPIKO], x, y, z);
							}
							else if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1515 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1517 ) // 070213 하트로드는 스태프인데 마석 옵션 요청때문에 셰이프가 소드다.
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_STAFF], x, y, z);
							}
							else
							{
								switch (GET_SHAPE(m_Wearing[WEARING_WEAPON]))
								{
								case IWEAPON_SWORD:
									g_pDSound->Play(g_EffectSound[EFF_SND_SWORD], x, y, z);
									break;
								case IWEAPON_AXE:
									g_pDSound->Play(g_EffectSound[EFF_SND_AXE], x, y, z);
									break;
								case IWEAPON_GUN:
									if (m_nMotionState != CHA_ATTACK)
										g_pDSound->Play(g_EffectSound[EFF_SND_STAFF], x, y, z);
									break;
								// Lyul_2005
								case IWEAPON_HAMMER:
								case IWEAPON_DUAL_SWORD:
									break;
								default:
									g_pDSound->Play(g_EffectSound[EFF_SND_STAFF], x, y, z);
									break;
								}
							}
						}
						else
						{
							g_pDSound->Play(g_EffectSound[EFF_SND_FIST], x, y, z);
						}
					}
					else if (m_nMotionState == CHA_KAI_SUB &&
							 (m_Wearing[WEARING_SHIELD] && m_Wearing[WEARING_SHIELD]->m_Vnum==773 ||
							  m_Wearing[WEARING_SHIELD] && m_Wearing[WEARING_SHIELD]->m_Vnum==1023) )
					{
						// 카이 광선검에서만 광선검 소리
						float x, y, z;
						x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
						y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
						z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;

						g_pDSound->Play(g_EffectSound[EFF_SND_LIGHTSWORD], x, y, z);
					}
				}
			}
		}
	}

	// nate 2005-06-09 : Burning Crash - 단계 구분
	if(	m_nMotionState == CHA_B_BURNINGCRASH )	// 불칸 - 버닝 크래쉬
	{
		int nSkillLevel = ( this == g_pRoh )?g_ChaSkill.m_SkillLevel[ 16 ]:m_nSkillLevel;
		nSkillLevel = ((nSkillLevel - 1) / 5) + 1;

		if( m_CurFrame > 112 && nSkillLevel >= 6 )
		{
			if( m_CurFrame > 124 && m_nOldFrame < 7 )
			{
				if( m_AttackChaId >= 0 )
				{
					CNkCharacter* pChar = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
					pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pChar, NULL, EFFECT_SKILL_BURNINGCRASH );
					pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pChar, NULL, EFFECT_SKILL_BURNINGCRASH2 );
				}
				else if( m_AttackMobId >= 0 )
				{
					CNkMob* pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
					pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, pMob, EFFECT_SKILL_BURNINGCRASH );
					pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, pMob, EFFECT_SKILL_BURNINGCRASH2 );
				}
				m_nOldFrame = 7;
			}
			m_byBurningStep = 6;
		}
		else if( m_CurFrame > 85 && nSkillLevel >= 5 )
			m_byBurningStep = 5;
		else if( m_CurFrame > 72 && nSkillLevel >= 4 )
			m_byBurningStep = 4;
		else if( m_CurFrame > 45 && nSkillLevel >= 3 )
			m_byBurningStep = 3;
		else if( m_CurFrame > 25 && nSkillLevel >= 2 )
			m_byBurningStep = 2;
		else
			m_byBurningStep = 1;

		if( m_AttackMobId < 0
				&& m_AttackChaId < 0 )
		{
			SetMotionState( CHA_STAND );
			m_byBurningStep = -1;
			if( this == g_pRoh )
			{
				pCMyApp->m_pUIMgr->m_b_Attack = FALSE;
				pCMyApp->m_pUIMgr->m_b_MovingForAttack = FALSE;
				pCMyApp->m_pUIMgr->m_b_MotionForAttack = 0;
				pCMyApp->AttackMsgSend( 16 );
			}
		}
		else if( m_nOldFrame < m_byBurningStep )
		{
			m_nOldFrame = m_byBurningStep;
			if( g_pDSound )	// nate 2005-07-21 : Burning Crash Sound
			{
				int nGap = (m_nOldFrame - 1);
				if( m_Sex && m_nOldFrame == 3)
					nGap += 4;
				else if( m_Sex && m_nOldFrame == 6 )
					nGap += 2;
				g_pDSound->Play(g_EffectSound[EFF_SND_BURNING_1 + nGap]);
			}
			if( this == g_pRoh )
				pCMyApp->AttackMsgSend( 16 );
		}
	}
	else
	{
		if( m_nOldFrame )
			m_nOldFrame = 0;
		if( m_byBurningStep && !( m_nMotionState==CHA_SOCIAL&&(m_ActNum>=153&&m_ActNum<=158) ) ) // 꼭지점 댄스중엔 m_byBurnigStep 을 이용하므로 초기화 안하도록 예외처리 .
			m_byBurningStep = 0;
	}

	//mungmae-2005/04/11
	//설명:헬크라이 어택중에 특정한 프레임 조건이 맞으면 콤보 준비되었다는 것.
	if( g_pRoh == this				// 주캐릭에 한함
			&& m_nMotionState == CHA_HELLCRY_ATTACK	// 헬크라이
			&& m_CurFrame >= 5						// 5프레임 이상
			&& m_CurFrame <= 12 )					// 12프레임 이하
	{
		m_bComboReady = true;					// 콤보 준비 완료
		m_nComboFrame = m_CurFrame;				// 현재 프레임 저장
	}
	else
	{
		m_bComboReady = false;

		if( pCMyApp->m_pUIMgr )
			pCMyApp->m_pUIMgr->m_nClickTimeCheck = 0;
	}

	// 하이브리더 콤보 스킬 (디바인스톤+ 배쉬) ///하이콤보
	// 공격 모션이 거의 끝날때쯤 AttackMsgSend를 보내준다.
	if( m_nMotionState == CHA_F_STONEBASH )
	{
		if( m_nComboFrame == 0 )
		{
			if( m_CurFrame >= 14 )
			{
				m_nComboFrame = 1;
			}
		}
		else if( m_nComboFrame == 1 )
		{
			if( m_CurFrame >= 17 )
			{
				// 디바인스톤을 내려준다
				CNkMob *pMob = pCMyApp->m_pUIMgr->FindMobById( m_resmobidx ); // 해당 캐릭터의 디바인스톤을 찾아서...
				if( pMob )
				{
					pMob->m_RunSmokeFrame = 1; // 디바인스톤이 땅에 들어가라고 셋팅.
				}

				m_nComboFrame = 2;
			}
		}
		else if( m_nComboFrame == 2 )
		{
			if( m_CurFrame >= 20 )
			{
				if( g_pRoh == this ) // 자기자신이고 타겟 패킷을 아직 보내지 않은 상태면...
				{
					pCMyApp->AttackMsgSend( SKILL_F_STONEBASH2 ); // 모션이 끝날때 보내자.
					m_nComboFrame = 3;
				}
			}
		}
	}

	// 배쉬 리플렉트가 모션이 끝날때 타겟 패킷을 보낸다. ///하이콤보
	if( m_nMotionState == CHA_F_BASHREFLECT )
	{
		if( m_nComboFrame ) // 발동 전.
		{
			if( m_CurFrame > 15 )
			{
				if( this == g_pRoh && !m_bComboSucceed ) // 타겟은 한번만 보내도록.
					pCMyApp->AttackMsgSend( SKILL_F_BASHREFLECT2 ); // 타겟 패킷. 이 안에서 m_bComboSucceed가 셋팅된다.
			}
		}
	}

	// 하이브리더 콤보 스킬 (대쉬+더블슬래쉬) 구현
	if( m_nMotionState == CHA_F_DASHSLASH && m_CurFrame >= 6 && m_CurFrame <= 23 && m_fDash_Frame > 0 ) ///하이콤보
	{
		float pastFrame = float(m_CurFrame - m_PrevFrame); // 진행되어야 될 프레임을 구한다.

		if(pastFrame > m_fDash_Frame)
			pastFrame = m_fDash_Frame; // 이동가능 횟수만큼 이동

		// 남은 이동 횟수 감소
		m_fDash_Frame -= pastFrame;

		// 이동
		float dx = m_dx * pastFrame;
		float dz = m_dz * pastFrame;

		// 플레이어일 경우 카메라 워킹 및 stop 메세지
		if(this == g_pRoh)
		{
			pCMyApp->DashHero(dx , dz);
		}
		else
		{
			m_wx += dx;
			m_wz += dz;
		}

		// 이동 완료
		if(m_fDash_Frame == 0.0f )
		{
			if(this == g_pRoh)
			{
			}
		}
	}

	if( ( m_nMotionState == CHA_F_DASH && m_CurFrame >= 6 && m_CurFrame <= 23 && m_fDash_Frame>0 )
			|| (m_nMotionState == CHA_B_FLYINGCRASH && m_CurFrame >= 6 && m_CurFrame <= 22 && m_fFlying_frame > 0)
			|| (m_nMotionState == CHA_F_SHADOWSLASH && m_CurFrame >= 24 && m_CurFrame <= 40 && m_fShadow_frame > 0)
			|| (m_nMotionState == CHA_KNOCKBACK) // 넉백 처리  ///페어리
	  )
	{
		float pastFrame = float(m_CurFrame - m_PrevFrame); // 진행되어야 될 프레임을 구한다.
		float* fFrame = NULL;
		if( m_nMotionState == CHA_F_DASH || m_nMotionState == CHA_KNOCKBACK ) // 넉백도 같은 변수를 씀.
			fFrame = &m_fDash_Frame;
		else if( m_nMotionState == CHA_B_FLYINGCRASH )
			fFrame = &m_fFlying_frame;
		else if( m_nMotionState == CHA_F_SHADOWSLASH )
			fFrame = &m_fShadow_frame;
		else
			return;

		if(pastFrame > *fFrame)
			pastFrame = *fFrame; // 이동가능 횟수만큼 이동

		// 남은 이동 횟수 감소
		*fFrame -= pastFrame;

		// 이동
		float dx = m_dx * pastFrame;
		float dz = m_dz * pastFrame;

		// 플레이어일 경우 카메라 워킹 및 stop 메세지
		if(this == g_pRoh)
			pCMyApp->DashHero(dx , dz);
		else
		{
			m_wx += dx;
			m_wz += dz;
		}

		if( m_nMotionState == CHA_F_DASH )
		{
			// 프레임 고정
			m_CurFrame = 8;
			// 이동 완료
			if(m_fDash_Frame == 0.0f )
			{
				m_CurFrame = 24- nInc;
				// 본인일 경우 Stop Msg
				if(this == g_pRoh)
				{
				}
			}
		}
		else if( m_nMotionState == CHA_KNOCKBACK ) // 넉백처리
		{
			// 프레임 고정
			m_CurFrame = 0;

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));
			ef_sort.nType = FX_RUNSMOKE;
			ef_sort.vPos = D3DVECTOR(m_wx, pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz), m_wz);
			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

			// 이동 완료
			if(m_fDash_Frame == 0.0f )
			{
				// 본인일 경우 Stop Msg
				if(this == g_pRoh)
				{
					char strTemp[50] = "";
					sprintf(strTemp, "stop %.0f %.0f %.0f %.0f\n", g_pRoh->m_tx, g_pRoh->m_tz,
							g_pRoh->m_wy, (g_pRoh->m_toDir) * 10000);       // m_wx,m_wz가 아니라  대쉬의 찍었던 m_tx,m_tz 기준으로 보내줘야 다른 사람한테 제대로 보인다.(y쪽은 m_wy가 맞다.m_ty가 안쓰이므로..) (05-12-09 원석)
					g_pTcpIp->SendNetMessage(strTemp);
				}

				SetMotionState(CHA_STAND);
			}
		}
	}
	//------------------------------------------------------------------------
	// 저격(snifing / 스나이핑) 관련 Ani 처리
	if(m_CurFrame > 9 && mb_Snifing )
	{
		if(mi_Snifing_GAP_Frame < 7)
		{
			mi_Snifing_GAP_Frame += nInc;
			nInc = 0;
		}
		else
		{
			mi_Snifing_GAP_Frame = 0;
			mb_Snifing = false;
		}
	}
	// 스나이핑 스킵 Delay 아닌 모든 스킬은 여기서 frame 증가.
	else
	{
		m_PrevFrame = m_CurFrame;
		m_CurFrame += nInc;
	}

	// SKY 일 경우 날라 다닌다.
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
	{
		m_nFlyFrame += nInc;

		if (rand() % 20 == 0) // 가끔 위치에 변동을 주고 싶어서...
			m_nFlyFrame++;

		while (m_nFlyFrame >= CHA_FLY_FRAME)
			m_nFlyFrame -= CHA_FLY_FRAME;
	}

	// Dead Frame
	if (IsDead() && m_DeadFrame >= 0)
		m_DeadFrame += nInc;


	if (m_Biped && m_CurFrame >= m_Biped->m_AnimCount)
	{
		m_CurFrame = 0;
		m_PrevFrame = 0;

		if (m_nMotionState == CHA_SOCIAL)
		{
			if (m_ActNum == 87)			// 머리 박는 동작이었으면.. 계속 머리박게...
				SetMotionState(CHA_SOCIAL, 88);
			else if (m_ActNum == 88)	// 머리 박고 있는 동작이면.. 걍 놔둔다...
			{}
			else if (m_ActNum == 67)	// 앉는 동작이었으면... 계속 앉아있게...
				SetMotionState(CHA_SOCIAL, 68);
			else if (m_ActNum == 68)
			{
				// 앉아 있으면... 가끔 다르게 움직이게...
				if (!(rand() % 5))
					SetMotionState(CHA_SOCIAL, 69);
			}
			else if (m_ActNum == 69)
				SetMotionState(CHA_SOCIAL, 68);
			else
				SetMotionState(CHA_STAND);
		}
		else if (IsDead())
			m_PrevFrame = m_CurFrame = m_Biped->m_AnimCount-1;
		else if( m_nMotionState == CHA_F_DASH ) ///대쉬수정
		{
			SetMotionState(CHA_STAND); // 대쉬는 프레임이 루프되지 않게 한다. (다른 사람 대쉬시에 모션을 한번 더 해버리는 경우가 있다.)
		}
	}
}
void CNkCharacter::addhitpoint(DWORD num, DWORD attacker)
{
	DWORD oldesttime = this->hitpoints[0][1];
	char tcp = 0;
	for(int i = 0; i<10; i++)
	{
		if(hitpoints[i][0] == 0)
		{
			this->hitpoints[i][0] = num;
			this->hitpoints[i][1] = GetTickCount();
			this->hitpoints[i][2] = attacker;
			return;
		}
		else if(this->hitpoints[i][1] < oldesttime)
		{
			oldesttime = this->hitpoints[i][1];
			tcp = i;
		}
	}
	this->hitpoints[tcp][0] = num;
	this->hitpoints[tcp][1] = GetTickCount();
	this->hitpoints[tcp][2] = attacker;
}
bool CNkCharacter::ishit()
{
	DWORD timenow = GetTickCount();
	for(int i = 0; i<10; i++)
	{
		if(hitpoints[i][0] > 0)
		{
			if(timenow - hitpoints[i][1] > 1000)
			{
				hitpoints[i][0] = 0;
				hitpoints[i][1] = 0;
			}
		}
	}
	for(int i = 0; i<10; i++)
	{
		if(hitpoints[i][0] > 0)
			return true;
	}
	return false;
}
// --------------------------------------------------------------------------
// MakeBasicBody()
// Desc : 이 함수는 나중에 다양한 아이템을 장식한 캐릭터를 만들때 확장하여 사용할 수 있다.
//  으흠..
// --------------------------------------------------------------------------
void CNkCharacter::MakeBasicBody(int nRace, int nSex, int nHair)
{
	m_Race		= (char)nRace;
	m_Sex		= (char)nSex;
	m_BpdRace	= nRace*2 + nSex;
	if (nHair >= 10)
		nHair = (nHair % 10) + 5;

	if (nHair < 0 || nHair >= 10)
		nHair = 0;

	m_Hair = (char) nHair;

	CSYMAP_it it = pCMyApp->m_race_csy_map.begin();
	for( int i=0 ; i<NKCHA_HAIR ; i++ )
	{
		it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(i&0xFF)<<8);
		if( it != pCMyApp->m_race_csy_map.end() )
			SetCapsyong((*it).second,i);
		else
		{
			_asm nop;
		}
	}

	it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(5&0xFF)<<8|(nHair&0xFF));
	if( it != pCMyApp->m_race_csy_map.end() )
		SetCapsyong((*it).second,NKCHA_HAIR);
	else
	{
		_asm nop;
	}
}

void CNkCharacter::MakeBasicBody()
{
	MakeBasicBody(m_Race, m_Sex, m_Hair);
}

void CNkCharacter::SetBiped(CBiped *pBiped)
{
	if( !pBiped )
		return;

	m_dwTimeCapPrev = m_dwBaseTime;
	m_Biped = pBiped;
}

void CNkCharacter::SetCapsyong(CCapsyong *pCapsyong, int capsyong_num)
{
	m_CapSyong[capsyong_num] = pCapsyong;
	m_dwBaseTime = timeGetTime();
}

void CNkCharacter::SetInitPos(float wx, float wz)
{
	m_wx = wx;
	m_wz = wz;
	m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);
}

void CNkCharacter::SetInitPos(float wx, float wy, float wz)
{
	m_wx = wx;
	m_wy = wy;
	m_wz = wz;
}

void CNkCharacter::SetCharName(const char* name)
{
	int len = strlen(name);
	SAFE_DELETE_ARRAY( m_pstrName );

	m_pstrName = new char[len+1];
	strcpy(m_pstrName, name);
}

// --------------------------------------------------------------------------
// SetInfo()
// Desc : 캐릭터에 대한 꼭 필요한 정보만을 넣어준다.
//     이름과 서버와의 통신 아이디 정도.
// --------------------------------------------------------------------------
void CNkCharacter::SetInfo(int id, const char *name, char *guild_name, char* guild_grade_name)
{
	m_nCharIndex = id;
	int len = strlen(name);
	SAFE_DELETE_ARRAY( m_pstrName );

	m_pstrName = new char[len+1];
	strcpy(m_pstrName, name);

	SetGuildName(guild_name);
	SetGuildGrade(guild_grade_name);
}

void CNkCharacter::SetGuildName(char *guild_name)
{
	SAFE_DELETE_ARRAY( m_pstrGuildName );

	if (guild_name)
	{
		int len = strlen(guild_name);
		if (len > 0)
		{
			m_pstrGuildName = new char[len+1];
			strcpy(m_pstrGuildName, guild_name);
		}
	}
}
void CNkCharacter::SetGuildGrade(char *guild_grade_name)
{
	SAFE_DELETE_ARRAY( m_pstrGuildGradeName );

	if (guild_grade_name)
	{
		int len = strlen(guild_grade_name);

		if (len > 0)
		{
			m_pstrGuildGradeName = new char[len+1];
			strcpy(m_pstrGuildGradeName, guild_grade_name);
		}
	}
}


// --------------------------------------------------------------------
// Move()
// Desc : 월드 공간 tx, tz로 이동을 한다.
//   tx와 tz는 1초후에 캐릭터가 도달할 위치 좌표이다.
//   네트웍 delay가 1/20 초로 가정된다..
// --------------------------------------------------------------------
BOOL CNkCharacter::Move(float tx, float tz)
{
	// 타겟지점 저정
	m_tx = tx;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);
	// 네트웍 딜레이를 고려하여 1.1초 동안 목적지에 도착할 속력을 구한다.
	// 즉 22프레임.
	m_dx /= 22.0f;
	m_dz /= 22.0f;
	m_bMoving = TRUE;

	return TRUE;
}

BOOL CNkCharacter::Place(float tx, float tz, float dir, int nFrame)
{
	m_bPlace = TRUE;
	m_fPlaceDir = dir;
	// 타겟지점 저정
	m_tx = tx;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;

	m_toDir = (float) atan2(m_dz, m_dx);

	if (nFrame <= 0 )
		nFrame = 1;

	m_dx /= nFrame;
	m_dz /= nFrame;
	m_bMoving = TRUE;

	return m_bMoving;
}

void CNkCharacter::Stop(float tx, float tz, float dir)
{
	m_bMoving = FALSE;

	m_wx = m_tx = tx;
	m_wz = m_tz = tz;
	m_toDir = dir;

	SetMotionState(CHA_STAND);
}

// --------------------------------------------------------------------
// Attack()
// Desc : (tx, tz)를 향하여 공격 모션을 취한다.
// --------------------------------------------------------------------
void CNkCharacter::AttackChakram(float tx, float ty, float tz)//차크람 공격
{
	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}

	m_RenderMultiShot = m_MultiShot = m_NextMultiShot;
	m_NextMultiShot = 0;
	// 타겟지점 저정
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);
	m_bAttack = TRUE;

	SetMotionState(CHA_THROW_CHAKRAM);// 차크람 공격
}

//--------------------------------------------------------------------------
// Attack
//--------------------------------------------------------------------------
// 2004.11.19
// Lyul / 추가
// 스킬 정리
void CNkCharacter::Attack(float tx, float ty, float tz, int attkType)
{
	if( m_nMotionState == CHA_KNOCKBACK ) // 넉백중엔 어택을 무시시킨다.(이동 방향등을 바꿔서 문제가 된다.) (07-03-06 원석)
		return;

	m_CurAttackType = attkType;

	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}

	m_RenderMultiShot = m_MultiShot = m_NextMultiShot;
	m_NextMultiShot = 0;
	// 타겟지점 저정
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);
	m_bAttack = TRUE;
	//------------------------------------------------------------------------
	// BULKAN
	//------------------------------------------------------------------------
	// DROP_ATTACK ( 점프어택 )
	if (attkType == SKILL_DROP_ATTACK)
		SetMotionState(CHA_JUMP_ATTACK);
	// ROTATE_ATTACK ( 돌려베기 )
	else if (attkType == SKILL_ROTATE_ATTACK)
		SetMotionState(CHA_ROTATE_ATTACK);
	// Hell Cry ( 헬 크라이)
	else if (attkType == SKILL_HELL_CRY)
		SetMotionState(CHA_HELLCRY_ATTACK);
	// Slayer / Chakram
	else if (attkType == SKILL_THROW)
	{
		// 본인의 chakram 이던가 해당 캐릭의 chakram
		if(pCMyApp->m_pUIMgr->m_bChakram || m_bChakramatt)
		{
			// CHAKRAM ( 차크람 )
			SetMotionState(CHA_THROW_CHAKRAM);
			pCMyApp->m_pUIMgr->m_bChakram = false;
			pCMyApp->m_pUIMgr->m_bChakramAtt = false;

			if(m_bChakramatt)
				m_bChakramatt = false;
		}
		else
		{
			// SLAYER ( 슬레이어 )
			//mungmae-2005/4/11(if문 추가)
			//설명: 헬크라이와 슬레이어 콤보쓸 때 모션을 두번 하지 말도록.
			if(!m_bComboSucceed)
			{
				SetMotionState(CHA_THROW_SLAYER);
				m_bComboSucceed = false;
			}
		}
	}
	// Force Attack
	else if (attkType == SKILL_FORCE_ATTACK)
	{
		if (m_Wearing[WEARING_WEAPON])
		{
			// 돌려베기
			if (GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD)
				SetMotionState(CHA_ROTATE_ATTACK);
			// 점프어택
			else if (GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE)
				SetMotionState(CHA_JUMP_ATTACK);
		}
		// 무기 없음.
		else
		{
			SetMotionState(CHA_ATTACK);
		}
	}
	// 해제 / Spell
	else if (attkType == SKILL_REMOVE || attkType == SKILL_BOOK_READ)
	{
		SetMotionState(CHA_KAI_SUB);
		pCMyApp->m_pUIMgr->m_bBookSpell = false;
		pCMyApp->m_pUIMgr->m_bBookSpellAtt = false;
	}
	//------------------------------------------------------------------------
	// HUMAN
	//------------------------------------------------------------------------
	// Snifing
	else if(attkType == SKILL_SNIFING)
	{
		mb_Snifing = true;
		SetMotionState(CHA_ATTACK);
	}
	// HANDLE (조작술)
	else if (attkType == SKILL_HANDLE)
	{
		m_DoubleAttack = true;

		if(g_pRoh == this)
		{
			if( pCMyApp->m_pUIMgr->m_b_Attack )
			{
				if(m_nMotionState != CHA_ATTACK)
					SetMotionState(CHA_ATTACK);
				return;
			}

			SetMotionState(CHA_HANDLE); // 휴먼 딥 공격
		}
	}
	// DEEP_ATTACK
	else if ( attkType == SKILL_DEEP_ATTACK )
		SetMotionState(CHA_HUMAN_DEEP); // 휴먼 딥 공격
	// CHAIN_ATTACK
	else if ( attkType == SKILL_CHAIN_ATTACK )
		SetMotionState(CHA_HUMAN_SHORT);
	// AIR_RAID 에어레이드.
	else if (attkType == SKILL_AIR_RAID)
		SetMotionState(CHA_AIRRAID);
	//대지의 축복
	else if (attkType == SKILL_BLESSING || attkType == SKILL_JOINT_LIFE || attkType == SKILL_LIFE_LIGHT)
		SetMotionState(CHA_AIDIA_SUB/*CHA_ATTACK*/);//1007
	else if (attkType == SKILL_A_LIFE_EXPANSION )	// nate 2005-07-06 : Life Expansion - SetMOtionState()
		SetMotionState( CHA_AIDIA_SUB );
	else if (attkType == SKILL_MAGIC_MOVE)
	{
		//마법이전
		if (!(m_Wearing[WEARING_SHIELD]) ||
				!(GET_ATT_FLAG(m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON) ||
				(g_pRoh == this && g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0)) //충전 0이라면,
			SetMotionState(CHA_ATTACK);
		else
			SetMotionState(CHA_MAGIC_MOVE/*CHA_ATTACK*/);//마법이전 모션 여기에...
	}
	else if (attkType == SKILL_VANPIRE_TOUCH || attkType == SKILL_HELL_TOUCH )
	{
		if (!(m_Wearing[WEARING_SHIELD]) ||
				!(GET_ATT_FLAG(m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON) ||
				(g_pRoh == this && g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0))
			SetMotionState(CHA_ATTACK);
		else
			SetMotionState(CHA_KAI_TOUCH); // 카이립톤 터치
	}
	else if ( attkType == SKILL_MELODY_OF_DEF || attkType == SKILL_MELODY_OF_GUD )
	{
		if (!(m_Wearing[WEARING_SHIELD]) ||
				!(GET_ATT_FLAG(m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON) ||
				(g_pRoh == this && g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0))
			SetMotionState(CHA_ATTACK);
		else
			SetMotionState(CHA_AIDIA_PIPE); // 에이디아 피리
	}
	//------------------------------------------------------------------------
	// 프리크
	//------------------------------------------------------------------------
	// 더블 슬래쉬
	else if ( attkType == SKILLF_A_DSLASH)
		SetMotionState(CHA_F_DSLASH);
	// 듀얼 어썰트
	else if ( attkType == SKILLF_C_DASSAULT)
		SetMotionState(CHA_F_DASSAULT);
	// 어썰트
	else if ( attkType == SKILLF_A_ASSAULT)
		SetMotionState(CHA_F_ASSAULT);
	// 디바인 스톤
	else if ( attkType == SKILLF_A_DSTONE)
		SetMotionState(CHA_F_DSTONE);
	// 배쉬
	else if ( attkType == SKILLF_A_BASH)
		SetMotionState(CHA_F_BASH);
	// 타임 차지 mungmae-2005/05/10 딴 유저의 케릭이 모션과 이펙트를 하게 하자.
	else if ( attkType == SKILLF_A_TIMECHARGE)
	{
		SetMotionState(CHA_F_TIMECHARGE);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL
									   ,pCMyApp->m_pUIMgr->FindMobById(m_resmobidx)
									   ,EFFECT_SKILL_TIMECHG);
	}
	// nate 2005-06-10 : Burning Crash - Motion Setting
	else if( attkType == SKILL_B_BURNINGCRASH )
	{
		// Cool Time
		if( g_pRoh == this )
			G_SK_SetCooltime( SKILL_B_BURNINGCRASH );
		SetMotionState( CHA_B_BURNINGCRASH );
	}

	// nate 2005-06-21 : Auto Mine - SetMotionState
	else if( attkType == SKILL_H_AUTOMINE )
		SetMotionState( CHA_H_AUTOMINE );
	else if ( attkType == SKILL_C_HELLDOWN )
	{
		//헬다운 동작 관련
		//if( ( this == g_pRoh
		//		&& pCMyApp->m_pUIMgr->m_bRClickDown == 1 )		//마우스를 누르고 있을 때
		//	|| 	( this != g_pRoh
		//			&& m_nSkillStep == 1 ) 	)
		//{
		//	if( this == g_pRoh )
		//		pCMyApp->m_pUIMgr->m_byCheckHellDown = 0;
		//	SetMotionState( CHA_C_HELLDOWN );
		//}
		//else if( ( this == g_pRoh
		//			&& pCMyApp->m_pUIMgr->m_bRClickDown == 2 ) 	//마우스를 띠었을 때
		//		|| ( this != g_pRoh
		//			&& m_nSkillStep == 10 ) )
		//{
		//	if( this == g_pRoh )
		//		pCMyApp->m_pUIMgr->m_byCheckHellDown = 1;
		//	SetMotionState( CHA_C_HELLDOWN2 );
		//}
		//else
		//	SetMotionState( CHA_STAND );

		//헬다운 동작 관련
		if( ( this == g_pRoh
				&& pCMyApp->m_pUIMgr->m_bRClickDown == 1 )		//마우스를 누르고 있을 때
				|| 	( this != g_pRoh && m_nSkillStep <= 1 )
		  )
		{
			if( this == g_pRoh )
				pCMyApp->m_pUIMgr->m_byCheckHellDown = 0;

			//if ( this != g_pRoh && true == m_bHellDowning )
			//	SetMotionState( CHA_STAND );
			//else
			SetMotionState( CHA_C_HELLDOWN );
		}
		else if( ( this == g_pRoh
				   && pCMyApp->m_pUIMgr->m_bRClickDown == 2 ) 	//마우스를 띠었을 때
				 || ( this != g_pRoh
					  && m_nSkillStep == 10 ) )
		{
			if( this == g_pRoh )
				pCMyApp->m_pUIMgr->m_byCheckHellDown = 1;
			SetMotionState( CHA_C_HELLDOWN2 );
		}
		else
			SetMotionState( CHA_STAND );

		m_bHellDowning = !m_bHellDowning;
	}
	else if( attkType == SKILL_K_POISON )	// nate 2005-11-08 : 포이즌 - SetMotionState
		SetMotionState( CHA_K_POISON );
	else if( attkType == SKILL_H_ELECTRICSHOT )	// nate 2005-11-15 : 일렉트릭샷 - SetMotionState
		SetMotionState( CHA_H_ELECTRICSHOT );
	else if( attkType == SKILL_F_SLASHSHADOW )	// 대쉬슬래쉬+섀도우 슬래쉬 ///하이콤보
	{
		SetMotionState( CHA_F_SLASHSHADOW );
		SetShowCrash_Miss( 10 ); // 콤보성공 이펙트(강) 출력.
	}
	else if( attkType == SKILL_F_SPACESLASH )
		SetMotionState( CHA_F_SPACESLASH );
	else if( attkType == SKILL_F_ELECTRICHAMMER )
		SetMotionState( CHA_F_ELECTRICHAMMER );
	else if( attkType == SKILL_K_FLAMESPEAR )
		SetMotionState( CHA_K_FLAMESPEAR );
	else if( attkType == SKILL_A_SUMMONFIRE )
		SetMotionState( CHA_A_SUMMONFIRE );
	else if( attkType == SKILL_H_RANDOMFIRE )
	{
		if( this == g_pRoh )
			SetMotionState( CHA_H_RANDOMFIRE_ME );
		else if ( this != g_pRoh && m_nSkillStep > 0)
			SetMotionState( CHA_H_RANDOMFIRE_YOU );
	}
	else if( attkType == SKILL_F_MERCERNERY_SLASH )
		SetMotionState( CHA_F_MERCERNERYSLASH );
	else if( attkType == SKILL_K_ICEBOLT )
		SetMotionState( CHA_K_ICEBOLT );
	else if( attkType == SKILL_B_WILDSMASH )
		SetMotionState( CHA_B_WILDSMASH );
	else if( attkType == SKILL_A_RESURRECTION )
	{
		if( this == g_pRoh )
		{
			m_bResurrectionSkill = TRUE;
			m_dwResurrection_StartTime = timeGetTime();
		}
		SetMotionState( CHA_A_RESURRECTION );
	}
	else if( attkType == SKILL_F_TORNADO_HAMMER )
		SetMotionState( CHA_F_TONADOHAMMER );
	//=====================================================
// 	else if( attkType == PEROM_SKILL_S125 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S01);
// 	}
// 	else if( attkType == PEROM_SKILL_S126 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S02);
// 	}
	else if( attkType == PEROM_SKILL_S127 )
	{
		SetMotionState(LHCha_Action__Perom_S03);
	}
	else if( attkType == PEROM_SKILL_S128 )
	{
		SetMotionState(LHCha_Action__Perom_S04);
	}
	else if( attkType == PEROM_SKILL_S129 )
	{
		SetMotionState(LHCha_Action__Perom_S05);
	}
	else if( attkType == PEROM_SKILL_S130 )
	{
		SetMotionState(LHCha_Action__Perom_S06);
	}
	else if( attkType == PEROM_SKILL_S131 )
	{
		SetMotionState(LHCha_Action__Perom_S07);
	}
	else if( attkType == PEROM_SKILL_S132 )
	{
		SetMotionState(LHCha_Action__Perom_S08);
	}
	else if( attkType == PEROM_SKILL_S133 )
	{
		SetMotionState(LHCha_Action__Perom_S09);
	}
	else if( attkType == PEROM_SKILL_S134 )
	{
		SetMotionState(LHCha_Action__Perom_S10);
	}
	else if( attkType == PEROM_SKILL_S135 )
	{
		SetMotionState(LHCha_Action__Perom_S11);
	}
	else if( attkType == PEROM_SKILL_S136 )
	{
		SetMotionState(LHCha_Action__Perom_S12);
	}
	else if( attkType == PEROM_SKILL_S137 )
	{
		SetMotionState(LHCha_Action__Perom_S13);
	}
	else if( attkType == PEROM_SKILL_S138 )
	{
		SetMotionState(LHCha_Action__Perom_S14);
	}
	else if( attkType == PEROM_SKILL_S139 )
	{
		SetMotionState(LHCha_Action__Perom_S15);
	}
	else if( attkType == PEROM_SKILL_S140 )
	{
		SetMotionState(LHCha_Action__Perom_S16);
	}
	else if( attkType == PEROM_SKILL_S141 )
	{
		SetMotionState(LHCha_Action__Perom_S17);
	}
// 	else if( attkType == PEROM_SKILL_S142 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S18);
// 	}
// 	else if( attkType == PEROM_SKILL_S143 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S19);
// 	}
// 	else if( attkType == PEROM_SKILL_S144 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S20);
// 	}
// 	else if( attkType == PEROM_SKILL_S145 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S21);
// 	}
// 	else if( attkType == PEROM_SKILL_S146 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S22);
// 	}
// 	else if( attkType == PEROM_SKILL_S147 )
// 	{
// 		SetMotionState(LHCha_Action__Perom_S23);
// 	}
	else if( attkType == PEROM_SKILL_S148 )
	{
		SetMotionState(LHCha_Action__Perom_S24);
	}
	else if( attkType == PEROM_SKILL_S149 )
	{
		SetMotionState(LHCha_Action__Perom_S25);
	}
	else if( attkType == PEROM_SKILL_S150 )
	{
		SetMotionState(LHCha_Action__Perom_S26);
	}
	else if (attkType > 0)
	{
		// 거리를 구한다.
		float dist = DistPlToPlAbs(m_wx, m_wz, m_tx, m_tz);
		// 높이차를 구한다.
		float height_diff = 0;

		switch (attkType)
		{
		case MOB_HEIGHT_HIGH_NUM:
			height_diff = (m_wy+MOB_HEIGHT_MIDDLE_VAL)-(m_ty+MOB_HEIGHT_HIGH_VAL);
			break;
		case MOB_HEIGHT_LOW_NUM:
			height_diff = (m_wy+MOB_HEIGHT_MIDDLE_VAL)-(m_ty+MOB_HEIGHT_LOW_VAL);
			break;
		default: //case MOB_HEIGHT_MIDDLE_NUM:
			height_diff = m_wy-m_ty;
			break;
		}

		// 높이와 거리를 3배정도.. ㅡㅡ;
		if (dist < height_diff * 10)
			SetMotionState(CHA_ATTACK, 2);
		else if (-dist > height_diff * 10)
			SetMotionState(CHA_ATTACK, 1);
		else
		{
			if(pCMyApp->IsSoccerZone())
				SetMotionState(CHA_WORLDCUP_KICK);
			else
				SetMotionState(CHA_ATTACK, 0);
		}
	}
	else
	{
		if(pCMyApp->IsSoccerZone())
			SetMotionState(CHA_WORLDCUP_KICK);
		else
			SetMotionState(CHA_ATTACK);
	}
}

void CNkCharacter::Magic(float tx, float ty, float tz, int act_num)
{
	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}

	// 타겟지점 저정
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);
	m_bAttack = TRUE;

	SetMotionState(CHA_MAGIC, act_num);
}

void CNkCharacter::Kill()
{
	if( GetMotionState() == CHA_DEAD )
		return;

	//mungmae-2005/07/25 헬다운 중 죽었을  처리
	if( m_nMotionState == CHA_C_HELLDOWN )
	{
		if( g_pRoh == this )
		{
			CNkCharacter* pCha = NULL;
			CNkMob* pMob = NULL;

			if( m_AttackChaId != -1 )
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
			else if( m_AttackMobId != -1 )
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );

			HellDownFail( pCha, pMob );
		}
		else
			m_nSkillStep = -1;
	}

	SetMotionState(CHA_DEAD);
	m_DeadFrame = 0;
}

//==============================================
// nate 2004 - 11
// new race
// 인덱스의 메크로 값 도입
// 프리크의 이도류 삽입
void CNkCharacter::SetMotionState(unsigned short nState, int act_num)
{
	if (m_Vital <= 0 && nState != CHA_DEAD)
		return;

	m_nMotionState = nState;
	int offset = 0;

	CItem* wearing_weapon = m_Wearing[WEARING_WEAPON];
	if( m_bEventWearFlag && m_EventWearing[WEARING_WEAPON] )
	{
		wearing_weapon = m_EventWearing[WEARING_WEAPON];
	}

	//----------------------------------------------------------------------
	// 현재 캐릭 모션에 따른 처리
	//----------------------------------------------------------------------
	switch (nState)
	{
	//--------------------------------------------------------------------
	// 걷기
	case CHA_WALK:
		// 전투 모드일 경우
		if (m_bCombat)
		{
			// 무기를 장착 했을 경우
			if (wearing_weapon)
			{
				// 두손 무기
				if( Check_TwoHandWeapon(wearing_weapon) )
				{
					switch (GET_SHAPE(wearing_weapon))
					{
					case IWEAPON_GUN: // 총 동작은 추후에 수정 바람...
					case IWEAPON_MISSILE:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_WALK_2GUN]].m_pBiped);
						m_ActNum = BPDIDX_WALK_2GUN;
						break;
					case IWEAPON_SWORD:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_WALK_2SWORD]].m_pBiped);
						m_ActNum = BPDIDX_WALK_2SWORD;
						break;
					// nate 2004 - 12 : new race
					case IWEAPON_DUAL_SWORD:
						SetBiped( g_ChaBpdList[ g_ChaBpdIndex[ m_BpdRace ][ BPDIDX_WALK_DSWORD ] ].m_pBiped );
						m_ActNum = BPDIDX_WALK_DSWORD;
						break;
					case IWEAPON_SPEAR:
						SetBiped( g_ChaBpdList[ g_ChaBpdIndex[ m_BpdRace ][ BPDIDX_WALK_2SWORD ] ].m_pBiped );
						m_ActNum = BPDIDX_WALK_2SWORD;
						break;
					default: // 도끼, 지팡이, 구슬...
						// IWEAPON_HAMMER, IWEAPON_AXE, IWEAPON_STAFF, IWEAPON_BALL:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_WALK_2AXE]].m_pBiped);
						m_ActNum = BPDIDX_WALK_2AXE;
						break;
					}
				}
				else
				{
					switch (GET_SHAPE(wearing_weapon))
					{
					case IWEAPON_GUN:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_WALK_1GUN]].m_pBiped);
						m_ActNum = BPDIDX_WALK_1GUN;
						break;
					default:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_WALK_1HAND]].m_pBiped);
						m_ActNum = BPDIDX_WALK_1HAND;
						break;
					}
				}
			}
			else
			{
				SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_WALK_HAND]].m_pBiped);
				m_ActNum = BPDIDX_WALK_HAND;
			}
		}
		else
		{
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_WALK_PEACE]].m_pBiped);
			m_ActNum = BPDIDX_WALK_PEACE;
		}
		break;

	case CHA_RUN:
		m_bRun = TRUE;
		if (m_bCombat)
		{
			// 무기 장착
			if (wearing_weapon)
			{
				if( Check_TwoHandWeapon(wearing_weapon) )
				{
					switch (GET_SHAPE(wearing_weapon))
					{
					case IWEAPON_GUN: // 총 동작은 추후에 수정 바람...
					case IWEAPON_MISSILE:
						if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_2GUN]].m_pBiped);
							m_ActNum = BPDIDX_FLY_2GUN;
						}
						else
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_2GUN]].m_pBiped);
							m_ActNum = BPDIDX_RUN_2GUN;
						}
						break;
					case IWEAPON_SWORD:
						if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_2SWORD]].m_pBiped);
							m_ActNum = BPDIDX_FLY_2SWORD;
						}
						else
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_2SWORD]].m_pBiped);
							m_ActNum = BPDIDX_RUN_2SWORD;
						}
						break;
					case IWEAPON_DUAL_SWORD:
						SetBiped( g_ChaBpdList[ g_ChaBpdIndex[ m_BpdRace ][ BPDIDX_RUN_DSWORD ]].m_pBiped );
						m_ActNum = BPDIDX_RUN_DSWORD;
						break;
					case IWEAPON_SPEAR:			// ITS #8192 아토믹 비행장치 모션 처리 by.Sol9 [2012.06.15]
						if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_2SWORD]].m_pBiped);
							m_ActNum = BPDIDX_FLY_2SWORD;
						}
						else
						{
							SetBiped( g_ChaBpdList[ g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_2SWORD]].m_pBiped );
							m_ActNum = BPDIDX_RUN_2SWORD;
						}
						break;
					default: // 도끼, 지팡이, 구슬...
						// IWEAPON_HAMMER, IWEAPON_AXE, IWEAPON_STAFF, case IWEAPON_BALL
						if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_2AXE]].m_pBiped);
							m_ActNum = BPDIDX_FLY_2AXE;
						}
						else
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_2AXE]].m_pBiped);
							m_ActNum = BPDIDX_RUN_2AXE;
						}
						break;
					}
				}
				else
				{
					// 한손무기
					if(GET_ATT_FLAG(wearing_weapon) & IATT_SUB_WEAPON)
					{
						if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_1HAND]].m_pBiped);
							m_ActNum = BPDIDX_FLY_1HAND;
						}
						else
						{
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_1HAND]].m_pBiped);
							m_ActNum = BPDIDX_RUN_1HAND;
						}
					}
					else
					{
						switch (GET_SHAPE(wearing_weapon))
						{
						case IWEAPON_GUN:
							if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
							{
								SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_1GUN]].m_pBiped);
								m_ActNum = BPDIDX_FLY_1GUN;
							}
							else
							{
								SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_1GUN]].m_pBiped);
								m_ActNum = BPDIDX_RUN_1GUN;
							}
							break;
						default:
							if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
							{
								SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_1HAND]].m_pBiped);
								m_ActNum = BPDIDX_FLY_1HAND;
							}
							else
							{
								SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_1HAND]].m_pBiped);
								m_ActNum = BPDIDX_RUN_1HAND;
							}
							break;
						}
					}
				}
			}
			else
			{
				// 맨손전투모드
				if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
				{
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_HAND]].m_pBiped);
					m_ActNum = BPDIDX_FLY_HAND;
				}
				else
				{
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_HAND]].m_pBiped);
					m_ActNum = BPDIDX_RUN_HAND;
				}
			}
		}
		else
		{
			// 평화시..
			if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
			{
				SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_FLY_PEACE]].m_pBiped);
				m_ActNum = BPDIDX_FLY_PEACE;
			}
			else
			{
				SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_RUN_PEACE]].m_pBiped);
				m_ActNum = BPDIDX_RUN_PEACE;
			}
		}
		break;

	//--------------------------------------------------------------------
	// 헤엄치기
	case CHA_SWIM:
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SWIM_GO]].m_pBiped);
		m_ActNum = BPDIDX_SWIM_GO;
		break;
	///1002
	case CHA_AIDIA_SUB:
	case CHA_KAI_SUB:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][63]].m_pBiped); // 63은 뭘까요?
		m_ActNum = 63;
		FrameReset();
		break;
	case CHA_MAGIC_MOVE:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][115]].m_pBiped);//29
		m_ActNum = 115;
		FrameReset();
		break;

	//--------------------------------------------------------------------
	// 공격
	case CHA_ATTACK:
		SetCombat(TRUE); //? 전투 : 평화

		// 무기가 있을 경우
		if (wearing_weapon)
		{
			if( Check_TwoHandWeapon(wearing_weapon) )
			{
				switch (GET_SHAPE(wearing_weapon))
				{
				case IWEAPON_GUN: // 총 동작은 추후에 수정 바람...
					if (act_num < 0 || act_num > 2)
						act_num = 0;
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_ATT_2GUN+act_num]].m_pBiped);
					m_ActNum = BPDIDX_ATT_2GUN;
					break;
				case IWEAPON_MISSILE:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_ATT_1GUN_NO]].m_pBiped);
					m_ActNum = BPDIDX_ATT_1GUN_NO;
					break;
				case IWEAPON_SWORD:
					offset = rand() % 3;
					offset += BPDIDX_ATT_2SWORD;
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][offset]].m_pBiped);
					m_ActNum = offset;
					break;
				case IWEAPON_RING:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][63]].m_pBiped);
					m_ActNum = 63;
					break;
				// nate 2004 - 12 : new race - 듀얼 소드
				case IWEAPON_DUAL_SWORD:
					offset = rand() % 3;
					offset += BPDIDX_ATT_DSWORD;
					SetBiped( g_ChaBpdList[ g_ChaBpdIndex[ m_BpdRace ][ offset ]].m_pBiped );
					m_ActNum = offset;
					break;
				case IWEAPON_SPEAR:
					offset = rand() % 3;
					offset += 32; // BPDIDX_ATT_2SWORD;
					SetBiped( g_ChaBpdList[ g_ChaBpdIndex[ m_BpdRace ][ offset ]].m_pBiped );
					m_ActNum = offset;
					break;
				default: // 도끼, 지팡이, 구슬...
					// IWEAPON_HAMMER, IWEAPON_AXE, IWEAPON_STAFF, IWEAPON_BALL
					offset = rand() % 3;
					offset += BPDIDX_ATT_2AXE;
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][offset]].m_pBiped);
					m_ActNum = offset;
					break;
				}
			}
			else
			{
				if(GET_ATT_FLAG(wearing_weapon) & IATT_SUB_WEAPON)
				{
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_ATT_1HAND]].m_pBiped);
					m_ActNum = BPDIDX_ATT_1HAND;
				}
				else
				{
					switch (GET_SHAPE(wearing_weapon))
					{
					case IWEAPON_GUN: // 총 동작은 추후에 수정 바람...
						if (act_num < 0 || act_num > 2)
							act_num = 0;
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_ATT_1GUN+act_num]].m_pBiped);
						m_ActNum = BPDIDX_ATT_1GUN;
						break;
					case IWEAPON_MISSILE:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_ATT_1GUN_NO]].m_pBiped);
						m_ActNum = BPDIDX_ATT_1GUN_NO;
						break;
					case IWEAPON_RING:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][63]].m_pBiped);
						m_ActNum = 63;
						break;

					default: // 칼, 도끼, 지팡이, 구슬...
						// IWEAPON_SWORD, IWEAPON_AXE, IWEAPON_STAFF, IWEAPON_BALL:
						if( wearing_weapon->m_Vnum == 1259
								|| wearing_weapon->m_Vnum == 1260
								|| wearing_weapon->m_Vnum == 1261
								|| wearing_weapon->m_Vnum == 2170	// [2008/11/24 Theodoric] WORLD_MATRIX_2009
								|| wearing_weapon->m_Vnum == 2171	// [2008/11/24 Theodoric] WORLD_MATRIX_2009
						  )
						{
							SetCombat(TRUE);
							m_ActNum = BPDIDX_HOLLYSTAFF;
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);//29
							FrameReset();
						}
						else
						{
							offset = rand() % 3;
							offset += BPDIDX_ATT_1HAND;
							m_ActNum = offset;
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][offset]].m_pBiped);
						}
						break;
					}
				}
			}
		}
		else
		{
			offset = rand() % 4;
			offset += 53; // BPDIDX_ATT_HAND_L;	// bpdindex.dta 에서 맨손공격 오프셋
			m_ActNum = offset;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][offset]].m_pBiped);
		}
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 내려찍기
	case CHA_JUMP_ATTACK:
		SetCombat(TRUE);
		if( m_Wearing[WEARING_WEAPON] )
		{
			if( Check_TwoHandWeapon(m_Wearing[WEARING_WEAPON]) )
			{
				switch (GET_SHAPE(m_Wearing[WEARING_WEAPON]))
				{
				case IWEAPON_SWORD:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][59]].m_pBiped);
					m_ActNum = 59;
					break;
				case IWEAPON_AXE:
				case IWEAPON_STAFF:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][60]].m_pBiped);
					m_ActNum = 60;
					break;
				default: //지팡이, 구슬...
					break;
				}
			}
			else
			{
				// 한손무기
				switch (GET_SHAPE(m_Wearing[WEARING_WEAPON]))
				{
				case IWEAPON_SWORD:
				case IWEAPON_AXE:
				case IWEAPON_STAFF:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][58]].m_pBiped);
					m_ActNum = 58;
					break;
				default: // 도끼, 지팡이, 구슬...
					break;
				}
			}
		}
		else
		{
			// 장비 없이
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][61]].m_pBiped);
			m_ActNum = 61;
		}
		FrameReset();
		break;

	//--------------------------------------------------------------------
	// 돌려베기
	case CHA_ROTATE_ATTACK:
		SetCombat(TRUE);

		if( m_Wearing[WEARING_WEAPON] )
		{
			if( Check_TwoHandWeapon(m_Wearing[WEARING_WEAPON]) )
			{
				// 두손 무기
				switch (GET_SHAPE(m_Wearing[WEARING_WEAPON]))
				{
				case IWEAPON_SWORD:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][103]].m_pBiped);
					m_ActNum = 103;
					break;
				case IWEAPON_AXE:
				case IWEAPON_STAFF:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][104]].m_pBiped);
					m_ActNum = 104;
					break;
				default: //지팡이, 구슬...
					break;
				}
			}
			else
			{
				// 한손무기
				switch (GET_SHAPE(m_Wearing[WEARING_WEAPON]))
				{
				case IWEAPON_SWORD:
				case IWEAPON_AXE:
				case IWEAPON_STAFF:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][102]].m_pBiped);
					m_ActNum = 102;
					break;
				default: // 도끼, 지팡이, 구슬...
					break;
				}
			}
		}
		else
		{
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][102]].m_pBiped);
			m_ActNum = 102;
		}
		FrameReset();
		break;

	//--------------------------------------------------------------------
	// 슬레이어
	case CHA_THROW_SLAYER:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][106]].m_pBiped);
		m_ActNum = 106;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 차크람
	case CHA_THROW_CHAKRAM:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][107]].m_pBiped);
		m_ActNum = 107;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 조작술
	case CHA_HANDLE:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2GUN]].m_pBiped);
		m_ActNum = BPDIDX_STAND_2GUN;//5에서...
		FrameReset();
		break;
	///////////
	//--------------------------------------------------------------------
	// 수영 STAND
	case CHA_SWIM_STAND:
		if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
		{
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][107]].m_pBiped);
			m_ActNum = 107;
		}
		else
		{
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SWIM_STOP]].m_pBiped);
			m_ActNum = BPDIDX_SWIM_STOP;
		}
		break;
	//--------------------------------------------------------------------
	// 조작술
	case CHA_SOCIAL:
		m_bCombat = FALSE;
		m_ActNum = act_num;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// Dead
	case CHA_DEAD:
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_DIE]].m_pBiped);
		m_ActNum = BPDIDX_DIE;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 매직
	case CHA_MAGIC:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][act_num]].m_pBiped);
		m_ActNum = act_num;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 불칸 보우. 안쓸듯.
	case CHA_BULKAN_BOW:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][116]].m_pBiped);
		m_ActNum = 116;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 카이 터치
	case CHA_KAI_TOUCH:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][117]].m_pBiped);
		m_ActNum = 117;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 에이디야 피리
	case CHA_AIDIA_PIPE:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][118]].m_pBiped);
		m_ActNum = 118;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 휴먼 딥어택
	case CHA_HUMAN_DEEP:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][119]].m_pBiped);
		m_ActNum = 119;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// 휴먼 체인어텍
	case CHA_HUMAN_SHORT:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][120]].m_pBiped);
		m_ActNum = 120;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// 헬크라이
	case CHA_HELLCRY_ATTACK:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][121]].m_pBiped);
		m_ActNum = 121;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// 에어레이드
	case CHA_AIRRAID:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][122]].m_pBiped);
		m_ActNum = 122;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// 어썰트
	case CHA_F_ASSAULT:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_ASSAULT]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_ASSAULT;
		FrameReset();
		break;

	//-----------------------------------------------------------------
	// 듀얼 어썰트 // Lyul_2005
	case CHA_F_DASSAULT:

		// 듀얼 어썰트를 쓰는 캐릭터가 Player일 경우 이미 어썰트가 사용 중이므로
		// Bpd 만 바꾼다.
		if(this == g_pRoh)
		{
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DASSAULT]].m_pBiped);
			m_ActNum = BPDIDX_SKILL_DASSAULT;
		}
		else
		{
			SetCombat(TRUE);
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DASSAULT]].m_pBiped);
			m_ActNum = BPDIDX_SKILL_DASSAULT;
			FrameReset();
		}
		break;
	//-----------------------------------------------------------------
	// 대쉬
	case CHA_F_DASH:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DASH;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// 더블 슬래쉬
	case CHA_F_DSLASH:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DSLASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DSLASH;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	//  하이브리더 콤보 스킬 대쉬 + 더블슬래쉬
	case CHA_F_DASHSLASH: ///하이콤보
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DSLASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DSLASH;
		FrameReset();
		m_CurFrame = 5; // 약간 초반을 넘겨야 더 자연스럽더라.
		m_PrevFrame = 5;
		break;

	//-----------------------------------------------------------------
	//  하이브리더 콤보 스킬 대쉬슬래쉬 + 섀도우슬래쉬
	case CHA_F_SLASHSHADOW: ///하이콤보
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_SHADOWSLASH1]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_SHADOWSLASH1;
		FrameReset();
		m_CurFrame = 26; // 약간 초반을 넘겨야 더 자연스럽더라.
		m_PrevFrame = 26;
		pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // 이걸 해줘야 한 애니메이션이 끝나후 멈춰준다.
		m_drawLineNum = 0; // 궤적 초기화.

		break;

	//-----------------------------------------------------------------
	//  하이브리더 콤보 스킬 디바인스톤 + 배쉬
	case CHA_F_STONEBASH: ///하이콤보
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_BASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_BASH;
		FrameReset();
		pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // 이걸 해줘야 한 애니메이션이 끝나후 멈춰준다.
		break;

	//-----------------------------------------------------------------
	//  하이브리더 콤보 스킬 스톤배쉬 + 리플렉션
	case CHA_F_BASHREFLECT: ///하이콤보
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_REFLECTION]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_REFLECTION;
		FrameReset();
		pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // 이걸 해줘야 한 애니메이션이 끝나후 멈춰준다.
		break;
	//-----------------------------------------------------------------
	//  디바인 스톤
	case CHA_F_DSTONE:
		m_bAttack = true;
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DSTONE]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DSTONE;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	//  타임차지
	case CHA_F_TIMECHARGE:
		m_bAttack = true;
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_TIMECHARGE]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_TIMECHARGE;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	//  배쉬
	case CHA_F_BASH:
		m_bAttack = true;
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_BASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_BASH;
		FrameReset();
		break;
	//리플렉션 모션 추가(mungmae-05/04/20)
	case CHA_F_REFLECTION:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_REFLECTION;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
		FrameReset();
		break;
	// nate 2005-06-03 : Burning Crash - 동작 셋팅
	case CHA_B_BURNINGCRASH:
		{
			SetCombat(TRUE);
			int nSkillLevel = ( this == g_pRoh )?g_ChaSkill.m_SkillLevel[ 16 ]:m_nSkillLevel;
			m_ActNum = BPDIDX_SKILL_BURNING	+ ( ( nSkillLevel - 1 ) / 5 );	// 스킬 30포인트를 6단계로 구분
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
			FrameReset();
		}
		break;
	// nate 2005-06-03 : 카이 다운헬 추가
	case CHA_C_HELLDOWN:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_DOWNHELL1;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_C_HELLDOWN2:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_DOWNHELL2;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_H_AUTOMINE:	// nate 2005-07-15 : Auto Mine - 모션 셋팅
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_AUTOMINE;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_A_SUMMONSQUAD:	// nate 2005-07-15 : Summon Squad - 모션 셋팅
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_SUMMONSQ;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_B_FLYINGCRASH:		// 플라잉 크래쉬 - SetBiped
	case CHA_K_POISON:			// nate 2005-11-08 : 포이즌 - SetBiped
	case CHA_H_ELECTRICSHOT:	// 일렉트릭 샷 - SetBiped
	case CHA_F_SHADOWSLASH:		// 쉐도우 슬래쉬 - SetBiped
	case CHA_F_ADRENALINE:		// 아드레날린 - SetBiped
		SetCombat(TRUE);
		m_ActNum = 66 + nState;	// Biped Index : 연속되는 인텍스값
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_WORLDCUP_KICK:	 // mungmae 2006/06/05 : 월드컵 킥 모션
		SetCombat(TRUE);
		m_ActNum = BPDIDX_WORLDCUP_KICK;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_SPIRITMODE:	 // mungmae 2006/08/31 : 스피릿
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SPIRITMODE; ///070723 일본버그리포트 수정
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_B_WILDSMASH:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_WILDSMASH;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_K_FLAMESPEAR:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_FLAMESPEAR;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_H_RANDOMFIRE_ME:
		{
			SetCombat(TRUE);
			int nSkillLevel = g_ChaSkill.m_SkillLevel[ 18 ];
			if( nSkillLevel <= 10 )
			{
				m_ActNum = BPDIDX_SKILL_RANDOMFIRE2;
				m_CurFrame = 66;
			}
			else if( nSkillLevel <= 20 )
			{
				m_ActNum = BPDIDX_SKILL_RANDOMFIRE3;
				m_CurFrame = 88;
			}
			else
			{
				m_ActNum = BPDIDX_SKILL_RANDOMFIRE4;
				m_CurFrame = 109;
			}
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
			FrameReset();
		}
		break;
	case CHA_H_RANDOMFIRE_YOU:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_RANDOMFIRE_YOU;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_A_SUMMONFIRE:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_SUMMONFIRE;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_F_SPACESLASH:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_SPACESLASH;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_F_ELECTRICHAMMER:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_ELECTRICHAMMER;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_B_PROTECTION_SHIELD:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_PROTECTION_SHIELD;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE;
		break;
	case CHA_K_ICEBOLT:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_ICEBOLT;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_A_RESURRECTION:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_RESURRECTION;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_F_TONADOHAMMER:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_TONADOHAMMER;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_F_MERCERNERYSLASH:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_MERCERNERY_SLASH;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;

	case CHA_DIE: // 죽는 모션이 아닌  죽어 있는 상태
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_DIE]].m_pBiped);
		m_ActNum = BPDIDX_DIE;
		m_nMotionState = CHA_DEAD;
		m_Vital = 0;
		m_DeadFrame = m_PrevFrame = m_CurFrame = m_Biped->m_AnimCount;
		break;

	case LHCha_Action__Perom_S03:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No03;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S04:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No04;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S05:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No05;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S06:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No06;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S07:
		{
			m_ActNum = PEROM_BPDIDX_SKILL_No07;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S08:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No08;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S09:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No09;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S10:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No10;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
			pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE;
		}
		break;
	case LHCha_Action__Perom_S11:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No11;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S12:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No12;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S13:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No13;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S14:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No14;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S15:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No15;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S16:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No16;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S17:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No17;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;
	case LHCha_Action__Perom_S24:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No24;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;

	case LHCha_Action__Perom_S25:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No25;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;

	case LHCha_Action__Perom_S26:
		{
			SetCombat(TRUE);
			m_ActNum = PEROM_BPDIDX_SKILL_No26;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
			FrameReset();
		}
		break;



	case CHA_KNOCKBACK: // 밀리는 모션 (일단 현재로선 서있는 모션을 사용) ///페어리
	//-----------------------------------------------------------------
	// 숨쉬기(서있기 CHA_STAND)
	default:
		// 전투모드
		m_bAttack = false;
		char aa[1024];
		sprintf(aa,"CHA_STAND %d\n", m_bCombat);
		OutputDebugString(aa);

		if( m_bCombat )
		{
			if( wearing_weapon )
			{
				if( Check_TwoHandWeapon(wearing_weapon) )
				{
					switch (GET_SHAPE(wearing_weapon))
					{
					case IWEAPON_GUN: // 총 동작은 추후에 수정 바람...
					case IWEAPON_MISSILE:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2GUN]].m_pBiped);
						m_ActNum = BPDIDX_STAND_2GUN;
						break;
					case IWEAPON_SWORD:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2SWORD]].m_pBiped);
						m_ActNum = BPDIDX_STAND_2SWORD;
						break;
					// nate 2004 - 12 : new race - 듀얼 소드
					case IWEAPON_DUAL_SWORD:
						SetBiped( g_ChaBpdList[ g_ChaBpdIndex[ m_BpdRace ][ BPDIDX_STAND_DSWORD ] ].m_pBiped );
						m_ActNum = BPDIDX_STAND_DSWORD;
						break;

					case IWEAPON_SPEAR:
						{
							SetBiped( g_ChaBpdList[ g_ChaBpdIndex[ m_BpdRace ][ BPDIDX_STAND_2SWORD ] ].m_pBiped );
							m_ActNum = BPDIDX_STAND_2SWORD;
						}
						break;
					default: // 도끼, 지팡이, 구슬...
						// IWEAPON_AXE, IWEAPON_STAFF, IWEAPON_BALL
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2AXE]].m_pBiped);
						m_ActNum = BPDIDX_STAND_2AXE;
						break;
					}
				}
				else
				{
					// 한손무기
					///2003  완드를 총으로 인식하는것을 막음.
					if( GET_ATT_FLAG(wearing_weapon) & IATT_SUB_WEAPON )
					{
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_1HAND]].m_pBiped);
						m_ActNum = BPDIDX_STAND_1HAND;
					}
					else
					{
						switch(GET_SHAPE(wearing_weapon))
						{
						case IWEAPON_GUN:
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_1GUN]].m_pBiped);
							m_ActNum = BPDIDX_STAND_1GUN;
							break;
						default:
							SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_1HAND]].m_pBiped);
							m_ActNum = BPDIDX_STAND_1HAND;
							break;
						}
					}
				}
			}
			else
			{
				SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_HAND]].m_pBiped);
				m_ActNum = BPDIDX_STAND_HAND;
			}

		}
		else
		{
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_PEACE]].m_pBiped);
			m_ActNum = BPDIDX_STAND_PEACE;
		}
		break;
	}

}
//==============================================

#define MOUSE_ON_AMBIENT	0x00FFFFFF
#define MOUSE_OFF_AMBIENT	0x008f96b3
#define MANASHIELD_AMBIENT	0xFF6666FF
#define ELECTRICSHIELD_AMBIENT	0xFF678A6D

#define MAGIC_FRAME (13)
#define ABS(a)	((a) < 0 ? -(a) : (a))
#define LOW_DISTANCE	(100.0f)

#define HIDE_HIDING_TIME	(1000) //Human delay skill PABLO
#define HIDE_RISING_TIME	(300)
#define HIDE_DELAY_TIME		(1000)

float hide_deep[2] = { 18.0f, 16.5f };

BOOL CNkCharacter::IsAlphaWear( int w, bool bEvent )
{
	CItem* pItem = NULL;
	if( bEvent == false )
	{
		if( m_Wearing[w] == NULL )
			return FALSE;

		pItem = m_Wearing[w];
	}
	else
	{
		if( m_EventWearing[w] == NULL )
			return FALSE;

		pItem = m_EventWearing[w];
	}

	if( pItem->m_Vnum >= 2320 && pItem->m_Vnum <= 2323 )
	{
		return TRUE;
	}

	if( pItem->m_Vnum >= 3826 && pItem->m_Vnum <= 3834 )
	{
		return TRUE;
	}

	if( pItem->m_Vnum >= 3864 && pItem->m_Vnum <= 3872 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CNkCharacter::RenderAlphaWear(CItem* pItem, BOOL bLow)
{
	if( GET_CSY(pItem, m_BpdRace) )
	{
		DWORD alphaBlend, alphaTest, alphaRef, alphaFunc;

		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHAREF, &alphaRef );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHAFUNC, &alphaFunc );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAREF, 0xAAAAAAAA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

		GET_CSY(pItem, m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(pItem,m_BpdRace), FALSE, bLow);

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAREF, alphaRef );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, alphaFunc );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, alphaTest);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend);	
	}

	return FALSE;
}

// lpd3dDevPick 이 널값이 들어올수 있다.
BOOL CNkCharacter::Render(LPDIRECT3DDEVICE7 lpd3dDevPick, BOOL bMouseOn, BOOL bVisible)
{
	if( !GET_D3DDEVICE() )
		return FALSE;

	static D3DMATRIX matTrans, matRotate, matWorld;
	static DWORD s_dwAlphaTest, s_dwAlphaBlend, s_dwSrcBlend, s_dwDestBlend, s_dwLighting;
	DWORD dwCurtime;
	DWORD dwTimeCap;
	unsigned int frame_count;
	BOOL bArrived = FALSE;
	// nate 2004 - 6 : 데카르디 감마 델타 구분 플레그
	BYTE byGuildIndex = 0;
	// LOD 하려구 한건데.. 별 소용이 없을 것 같당~~~
	BOOL bLow = FALSE;
	// 다음 프레임 이동.
	dwCurtime = timeGetTime();

	//휴먼 보조무기 공속을 강제로 맞추기 위해서
	FLOAT fTime = timeGetTime() / 1000 ;

	if( m_MultiShot > 0 )
	{
		if( m_nMotionState == CHA_ATTACK
				&& m_Wearing[WEARING_WEAPON]
				&& GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN
				&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame + 3 )
		{
			m_CurFrame = g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;
			m_PrevFrame = m_CurFrame-1;
			m_MultiShot--;
		}
	}

	// 달리고 있지 않으면 달리는 게 아니지..
	if( g_pRoh == this && m_nMotionState != CHA_RUN )
		pCMyApp->m_bRohRunning = FALSE;

	// 달리고 있으면 스테미너 줄어야쥐...
	if( g_pRoh == this && pCMyApp->m_bRohRunning ) // 케릭이 달리고 있다면
	{
		int stamina = 0;
		while (dwCurtime >= pCMyApp->m_startRunningTime + 1000) // 달리고 있다..
		{
			pCMyApp->m_startRunningTime += 1000; // 아직도..
			stamina += 2;
		}

		if (stamina > 0 && g_pTcpIp)
		{
			char strTemp[20] = "";
			sprintf(strTemp, "stamina %d\n", stamina);
			g_pTcpIp->SendNetMessage(strTemp);
		}
	}

	// Vital 이 변해야 하는데, 1초가 지나도 안변하면...
	if (m_Vital != m_AttackVital && dwCurtime > m_LastAttackVitalTime + 1000)
	{
		Attacked();
		dwCurtime = timeGetTime();
	}
	else if (m_nMotionState != CHA_DEAD && m_Vital <= 0 && dwCurtime > m_LastAttackVitalTime + 1000)
	{
		Kill();//Death!
		dwCurtime = timeGetTime();
	}

	/// CanSwapWeapon()로 통합 by.Sol9 [2012.05.09] //////////////
	//if( m_nMotionState == CHA_MAGIC_MOVE )//mungmae-2006/06/21 마법이전 중에 무기 바꾸기 안되게 막음.
	//	m_bSwapWeapon = FALSE;
	//else
	//	m_bSwapWeapon = TRUE;
	//////////////////////////////////////////////////////////////////////////

	if( this == g_pRoh &&
			m_nMotionState == CHA_MAGIC &&
			pCMyApp->m_pUIMgr->m_bRClickDown == 1 &&//mungmae-2008/09/01 오른쪽 ㅅ 중에 헬다운이 나간다면 실패.
			pCMyApp->DrawCasting )
	{
		HellDownFail( g_pRoh, NULL );
	}

	// 나 너 때렸다!!
	CNkMob *pMob = NULL;
	CNkCharacter *pCha = NULL;
	//아직 손봐야 함.오혁
	if (m_AttackMobId >= 0)
	{
		// 에이디아나 카이 서브어택
		if(m_nMotionState == CHA_AIDIA_SUB || m_nMotionState == CHA_KAI_SUB	)
		{
			//1007
			if (m_PrevFrame < m_CurFrame &&	m_PrevFrame < MAGIC_FRAME && m_CurFrame >= MAGIC_FRAME)
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
				if (pMob)pMob->Attacked();
				m_AttackMobId = -1;
			}
		}
		// 에이디아 링이거나 완드류
		else if( ( m_Wearing[WEARING_WEAPON]
				   && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_RING
				   && m_nMotionState != CHA_A_SUMMONFIRE && m_nMotionState != CHA_A_RESURRECTION )
				 || ( m_Wearing[WEARING_SHIELD]
					  && GET_TYPE(m_Wearing[WEARING_SHIELD]) == ITYPE_SUB
					  && GET_SHAPE(m_Wearing[WEARING_SHIELD]) == ISUB_WAND
					  && m_nMotionState == CHA_MAGIC_MOVE ))
		{
			// 링을 쓰고, 마법쏠때가 되면 쏘자!! 완드도 여그...
			if (m_PrevFrame < m_CurFrame &&	m_PrevFrame < MAGIC_FRAME && m_CurFrame >= MAGIC_FRAME)
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
				if (pMob)
				{
					//CHA_MAGIC_MOVE  // [12/3/2008 Theodoric] 이런 무식하게 이펙트를 넣다니 디파인 번호 겹치면 어칼라고ㅡㅡ;;
					if(m_nMotionState == CHA_MAGIC_MOVE)
					{
						int insertEffectNum = m_Wearing[WEARING_SHIELD]->m_Vnum;
						switch(insertEffectNum)
						{
						case 3065:
							insertEffectNum = EFFECT_SKILL_WAND_WAVE_OF_DEATH;
							break;
						case 10372:
							insertEffectNum = EFFECT_SKILL_WAND_ARROW;
							break;
						case 10415:
							insertEffectNum = EFFECT_SKILL_WAND_METEORITE;
							break;
						case 11063:
							insertEffectNum = EFFECT_SKILL_WAND_ICE_STORM;
							break;
						case 11084:
							insertEffectNum = EFFECT_SKILL_WAND_CHAOS;
							break;
						default:
							insertEffectNum = m_Wearing[WEARING_SHIELD]->m_Vnum+1000;
							break;
						}

						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, insertEffectNum);
					}
					else
					{
						// [12/3/2008 Theodoric] 이전 방법은 스킬번호가 아이템 번호에 + 1000 이였다.
						// 무기가 증가에 따라 스킬 번호가 중복 가능성이 있어서 수정
						int insertEffectNum = m_Wearing[WEARING_WEAPON]->m_Vnum;
						switch(  insertEffectNum )
						{
						case 2053:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA;
							break; // [12/3/2008 Theodoric]  사릉가
						case 2177:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA2;
							break; // 신의 사릉가
						case 2265:
							insertEffectNum = EFFECT_SKILL_RING_RAVEN;
							break; // 견습생의 레이븐 링
						case 2270:
							insertEffectNum = EFFECT_SKILL_RING_NEO;
							break; // 견습생의 네오스네이크 링
						case 2275:
							insertEffectNum = EFFECT_SKILL_RING_KAISEN;
							break; // 견습생의 카이젠 링
						case 2280:
							insertEffectNum = EFFECT_SKILL_RING_SHININGEAGLE;
							break; // 견습생의 피닉스 링
						case 2285:
							insertEffectNum = EFFECT_SKILL_RING_SPHINX;
							break; // 견습생의 스핑크스 링
						case 2292:
							insertEffectNum = EFFECT_SKILL_RING_DRAGON;
							break; // 견습생의 드래곤 링			// 견습생 링 이펙트 관련 인덱스 매칭 문제 수정 by Sol9 [2012.06.01]
						case 2698:
							insertEffectNum = EFFECT_SKILL_RING_BULKAN;
							break;
						case 2747:
							insertEffectNum = EFFECT_SKILL_RING_GRIFFON;
							break;	// 360lv 바리사다
						case 2924:
							insertEffectNum = EFFECT_SKILL_RING_GRYPHON;
							break;	// 사-그리폰
						case 3288:
							insertEffectNum = EFFECT_SKILL_RING_DEVIL2;
							break;	// 진데빌링 & 사 데빌링
						case 10458: // AIDIA LEVEL 450
							insertEffectNum = EFFECT_SKILL_RING_YETI;
							break;
						case 10466: // AIDIA LEVEL 500
							insertEffectNum = EFFECT_SKILL_RING_PEAKOCK;
							break;
						case 10473: // AIDIA LEVEL 550
							insertEffectNum = EFFECT_SKILL_RING_TIGER;
							break;
						case 10481: // AIDIA LEVEL 600
							insertEffectNum = EFFECT_SKILL_RING_UNICORN;
							break;	
						case 10526: // AIDIA LEVEL 650
							insertEffectNum = EFFECT_SKILL_RING_650;
							break;	
						case 10534: // AIDIA LEVEL 700
							insertEffectNum = EFFECT_SKILL_RING_700;
							break;	
					    case 10542: // AIDIA LEVEL 750
							insertEffectNum = EFFECT_SKILL_RING_750;
							break;	
						case 10550: // AIDIA LEVEL 800
							insertEffectNum = EFFECT_SKILL_RING_800;
							break;	
						case 10558: // AIDIA LEVEL 850
							insertEffectNum = EFFECT_SKILL_RING_850;
							break;	
						case 10566: // AIDIA LEVEL 870
							insertEffectNum = EFFECT_SKILL_RING_900;
							break;	
						case 10574: // AIDIA LEVEL 890
							insertEffectNum = EFFECT_SKILL_RING_950;
							break;	
						case 10727: // AIDIA LEVEL 910
							insertEffectNum = EFFECT_SKILL_RING_1000;
							break;	
						case 10707: // AIDIA CHRISTMAS
							insertEffectNum = EFFECT_SKILL_RING_CHR;
							break;
						case 10843: // AIDIA CHINESE
							insertEffectNum = EFFECT_SKILL_RING_CHIN;
							break;
						default:
							insertEffectNum += 1000;
							break; // 이전 사용 스킬들
						}

						if( m_nMotionState )
							pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, insertEffectNum);
					}
				}

				m_AttackMobId = -1;
			}
		}
		// 2006 신스킬
		else if( m_nMotionState == CHA_H_ELECTRICSHOT			// nate 2005-11-23 : 일렉트릭 샷 - InsertMagic
				 || ( m_Wearing[WEARING_WEAPON]
					  && ( m_nMotionState == CHA_K_POISON				// nate 2005-11-10 : 포이즌 - InsertMagic
						   || m_nMotionState == CHA_B_FLYINGCRASH		//mungmae 2005/11/16 : 플라잉 크래쉬 - 어택 프레임때 이펙트 넣기
						   || m_nMotionState == CHA_F_SHADOWSLASH )))	//mungmae 2005/11/16 : 쉐도우 슬래쉬 - InsertMagic
		{
			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
				if( pMob )
				{
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, m_nMotionState - 1 );
					if( g_pRoh == this
							&& (m_nMotionState == CHA_H_ELECTRICSHOT
								|| m_nMotionState == CHA_F_SHADOWSLASH ))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 1, 0);
					}
				}
				m_AttackMobId = -1;
			}
		}
		else if( m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER
				 && m_nMotionState == CHA_F_ELECTRICHAMMER )
		{
			int target_count = 9;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[MAX_PATH] = {0,};

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );

				if(pMob && 120 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pMob->m_nVirId);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pMob,  fDist, 30, true, target_count);
												
						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicMob(g_pTargetMob[i]);
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 1, vecMobIndex);
					}
				}
				
				m_AttackMobId = -1;
			}
		}
		else if( m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER
				 && m_nMotionState == CHA_F_TONADOHAMMER )
		{
			int target_count = 6;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[128] = "";
			float x, y, z;
			x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
			y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
			z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;

			if( m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 17 && m_CurFrame >= 17 ) || ( m_PrevFrame < 25 && m_CurFrame >= 25 )
											  || ( m_PrevFrame < 33 && m_CurFrame >= 33 ) ) )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
				if( g_pDSound )
					g_pDSound->Play(g_EffectSound[EFF_SND_TONADO_HAMMER], x, y, z);

				if(pMob && 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pMob->m_nVirId);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pMob,  fDist, 30, true, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicMob(g_pTargetMob[i]);
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
							
							if( g_pDSound )
								g_pDSound->Play(g_EffectSound[EFF_SND_TONADO_HAMMER], x, y, z);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 1, vecMobIndex);
					}
				}
				else if( pMob && 40 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
				{	
					m_AttackMobId = -1;

					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, -1, 0);
					}
				}				
			}
		}
		else if( m_nMotionState == CHA_A_SUMMONFIRE )
		{
			int target_count = 9;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[128] = "";

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );

				if( pMob && 135 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pMob->m_nVirId);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pMob,  fDist, 30, true, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicMob(g_pTargetMob[i]);
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 1, vecMobIndex);
					}
				}
				else if( pMob && 135 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
				{	
					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, -1, 0);
					}
				}
				
				m_AttackMobId = -1;
				
				if( g_pRoh == this )
				{
					G_SK_SetCooltime(m_CurAttackType);					
				}
			}
		}
		else if( m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN
				 && m_nMotionState == CHA_H_RANDOMFIRE_ME )
		{
			int nSkillLevel = g_ChaSkill.m_SkillLevel[ 18 ];
			int target_count = 9;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[128] = "";

			int nSkillDistance = pCMyApp->GetSkillDistance(SKILL_H_RANDOMFIRE);

			if( g_pDSound )
			{
				float x, y, z;
				x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
				y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
				z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_RANDOM_SHOT], x, y, z);
			}

			pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );

			if( nSkillLevel <= 10 && m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 22 && m_CurFrame >= 22 ) || ( m_PrevFrame < 38 && m_CurFrame >= 38 )
					|| ( m_PrevFrame < 54 && m_CurFrame >= 54 ) ) )
			{
				if( pMob && !pMob->IsDead()
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pMob->m_nVirId);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pMob,  fDist, 120, true, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicMob(g_pTargetMob[i]);
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 1, vecMobIndex);
					}
				}
				else if( pMob && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
				{
					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, -1, 0);
					}
				}
			}

			if( nSkillLevel > 10 && nSkillLevel <= 20 && m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 22 && m_CurFrame >= 22 ) || ( m_PrevFrame < 38 && m_CurFrame >= 38 )
					|| ( m_PrevFrame < 54 && m_CurFrame >= 54 ) || ( m_PrevFrame < 70 && m_CurFrame >= 70 ) ) )
			{
				if( pMob && !pMob->IsDead()
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pMob->m_nVirId);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pMob,  fDist, 120, true, target_count);
						
						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicMob(g_pTargetMob[i]);
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 1, vecMobIndex);
					}
				}
				else if( pMob && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
				{	
					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, -1, 0);
					}					
				}				
			}

			if( nSkillLevel > 20 && nSkillLevel <= 35 && m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 22 && m_CurFrame >= 22 ) || ( m_PrevFrame < 38 && m_CurFrame >= 38 )
					|| ( m_PrevFrame < 54 && m_CurFrame >= 54 ) || ( m_PrevFrame < 70 && m_CurFrame >= 70 ) || ( m_PrevFrame < 86 && m_CurFrame >= 86 ) ) )
			{
				if( pMob && !pMob->IsDead()
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pMob->m_nVirId);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pMob,  fDist, 120, true, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicMob(g_pTargetMob[i]);
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 1, vecMobIndex);
					}
				}
				else if( pMob && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
				{
					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, -1, 0);
					}
				}				
			}
			if( ( 17 <= m_CurFrame && m_CurFrame >= 17 + 5 ) &&
					GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
			{
				D3DMATRIX	matRot;
				D3DVECTOR	vTmp;
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_RANDOMSHOOT;

				m_dir = m_toDir = (float) atan2(m_tz - m_wz, m_tx - m_wx);

				Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->m_LineVector[0], matRot);
				ef_sort.vPos = vTmp;
				ef_sort.vPos.x += m_wx;
				ef_sort.vPos.y += m_wy;
				ef_sort.vPos.z += m_wz;
				ef_sort.dwColor = 0xFFFFFFFF;
				ef_sort.nCurFrame = m_CurFrame - g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;
				ef_sort.bOneFrame = TRUE;

				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				//		m_AttackMobId = -1;
			}
			if( pMob && pMob->IsDead() )
				SetMotionState(CHA_STAND);
		}
		else if( m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN &&
				 m_nMotionState == CHA_H_RANDOMFIRE_YOU )
		{
			if( g_pDSound )
			{
				float x, y, z;
				x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
				y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
				z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_RANDOM_SHOT], x, y, z);
			}

			if( ( 0 <= m_CurFrame && m_CurFrame >= 5 ) &&
					GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
			{
				D3DMATRIX	matRot;
				D3DVECTOR	vTmp;
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_RANDOMSHOOT;

				m_dir = m_toDir = (float) atan2(m_tz - m_wz, m_tx - m_wx);

				Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->m_LineVector[0], matRot);
				ef_sort.vPos = vTmp;
				ef_sort.vPos.x += m_wx;
				ef_sort.vPos.y += m_wy;
				ef_sort.vPos.z += m_wz;
				ef_sort.dwColor = 0xFFFFFFFF;
				ef_sort.nCurFrame = m_CurFrame - g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;
				ef_sort.bOneFrame = TRUE;

				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}
		}
		else if( m_nMotionState == CHA_F_MERCERNERYSLASH
				 && m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD )
		{
			if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 12 && m_CurFrame >= 12 ) )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
				if( pMob )
				{
					if( g_pRoh == this && pMob
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 2, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 25 && m_CurFrame >= 25 ) )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
				if( pMob )
				{
					if( g_pRoh == this && pMob
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz ))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 3, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 32 && m_CurFrame >= 32 ) )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
				if( pMob )
				{
					if( g_pRoh == this && pMob
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 4, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 42 && m_CurFrame >= 42 ) )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
				if( pMob )
				{
					if( g_pRoh == this && pMob
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 5, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 58 && m_CurFrame >= 58 ) )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );
				if( pMob )
				{
					if( g_pRoh == this && pMob
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, 6, 10);
					}
				}
			}
			if( pMob
					&& 40 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob 공격
			{
				if( g_pRoh == this )
				{	
					SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Npc, pMob->m_nVirId, -1, 0);
				}
				m_AttackMobId = -1;
			}
		}
		//----------------------------------------------------------------------
		// Hit Effect
		//----------------------------------------------------------------------
		// 이도류의 두번째 타격은 아래 루틴을 타지 못한다. 강제적으로 할당.
		// bpdlist 에 명시된 Attack Frame 과 비교하여 attack effect 를 적용한다.
		else if( ( m_nMotionState != CHA_ATTACK
				   && m_nMotionState != CHA_JUMP_ATTACK
				   && m_nMotionState != CHA_ROTATE_ATTACK
				   && m_nMotionState != CHA_THROW_SLAYER
				   && m_nMotionState != CHA_THROW_CHAKRAM
				   && m_nMotionState != CHA_HANDLE )
//					&& m_nMotionState != CHA_B_BURNINGCRASH )	// nate 2005-06-29 : Burning Crash - 두번째 타격 스킵
				 || ( g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame <= m_CurFrame ) )
		{
			pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
			if (pMob)
			{
				// 슬래이어 이펙트
				if (m_nMotionState == CHA_THROW_SLAYER)
				{
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_SLAYER);
					pMob->Attacked();
				}
				// 본인 캐릭 차크람
				else if (this == g_pRoh && m_nMotionState == CHA_THROW_CHAKRAM && pCMyApp->m_pUIMgr
						 && pCMyApp->m_pUIMgr->m_ChakramSlotNum >= 0 && m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum] )
				{
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob,m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]->m_ProtoNum+2000 );//m_Wearing[WEARING_CLOAK]->m_ProtoNum+1000);
				}
				// 타 캐릭 차크람
				else if( this != g_pRoh && m_nMotionState == CHA_THROW_CHAKRAM )//this !=g_pRoh && m_CurFrame >= THROW_CHAKRAM_FRAME_START && m_CurFrame <= THROW_CHAKRAM_FRAME_END
				{
					int proto_num = g_ItemList.FindItem(693);
					if (proto_num >= 0)
					{
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob,proto_num+2000 );
					}
				}
				else if (m_nMotionState == CHA_KAI_TOUCH && m_Wearing[WEARING_SHIELD])
				{
					if( m_CurAttackType == SKILL_VANPIRE_TOUCH )
						pCMyApp->m_pUIMgr->InsertMagic(NULL, pMob, this, NULL, EFFECT_SKILL_VANPIRE_TOUCH);
					else if( m_CurAttackType == SKILL_HELL_TOUCH )
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_HELL_TOUCH);

					pMob->Attacked();
				}
				else if( (m_nMotionState == CHA_HUMAN_DEEP || m_nMotionState == CHA_HUMAN_SHORT)
						 && m_Wearing[WEARING_SHIELD] )
				{}
				//------------------------------------------------------------------
				// 각 무기의 Effct 추가 (이펙트)
				// 몹을 대상으로 한다.캐릭을 대상으로 하는 코드는 아랫쪽에 존재.
				// 캐릭과 몹은 따로 처리가 된다. 주의하라.
				else if( m_Wearing[WEARING_WEAPON] )
				{
					if(m_Wearing[WEARING_WEAPON]->m_Vnum == 1238 || m_Wearing[WEARING_WEAPON]->m_Vnum == 2172 )//데미크루셔 이펙트
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_DEMICRUSHER);
					// nate 2005-05-20 : 홀리부대
					else if( m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1259	// 홀리 스테프( 호크 )
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1260		// 홀리 스테프( 쿨레인 )
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1261		// 홀리 스테프( 쉬버 )
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 2170		// [2008/11/24 Theodoric] WORLD_MATRIX_2009
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 2171		// [2008/11/24 Theodoric] WORLD_MATRIX_2009
						   )
						pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, pMob, EFFECT_SKILL_HOLLEY );

					// [3/27/2007 Theodoric] 태국 송크란 이벤트 || 무기를 들었을 경우
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1538 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1539 )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_SONGKRAN);
					}


					///하트로드 추가
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1514 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1515
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1517 ) // 하트 로드,러브러브 로드
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_HEART);
					}


					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1531 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1532
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1533 ) // 입술(화이트) : 딸기, 레몬, 멜론 캔디
					{
						// 2007.03.14 화이트
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_LIP);
					}

					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1534 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1535 )
						// 입술(화이트), LOVE : ( 레인보우 홀, 레인보우 캔디 )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_LIP);
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_LOVE);
					}
					// 이도류 추가. Hit Effect
					if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD)
					{
						if( m_nMotionState == CHA_ATTACK )
							pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_DSWORD);
					}
					// 일반 검 , 햄머
					else if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD
							 || GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER )
					{
						if( m_nMotionState == CHA_ATTACK )
						{
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_SWORD);
						}
						else if( m_nMotionState == CHA_ROTATE_ATTACK || m_nMotionState == CHA_JUMP_ATTACK)
						{
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_SOWRD_ROTATE_ATTACK);
						}
					}
					// 도끼
					else if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE )
					{
						if( m_nMotionState == CHA_ATTACK )
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_AXE);
						else if( m_nMotionState == CHA_ROTATE_ATTACK || m_nMotionState == CHA_JUMP_ATTACK)
						{
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_AXE_JUMP_ATTACK);
						}
					}
					// 지팡이
					else if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_STAFF )
					{
						if( m_nMotionState == CHA_ATTACK )
							pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_STAFF);
					}
					pMob->Attacked();
				}
				else
				{
					if( m_nMotionState == CHA_ATTACK )
						if( m_CurAttackType == SKILL_FORCE_ATTACK )
							pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
						else
							pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_HAND);
					else if( (m_nMotionState == CHA_ROTATE_ATTACK || m_nMotionState == CHA_JUMP_ATTACK)
							 && m_CurAttackType == SKILL_FORCE_ATTACK )
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
					else if( m_nMotionState == CHA_JUMP_ATTACK )
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_AXE_JUMP_ATTACK);
					else if( m_nMotionState == CHA_ROTATE_ATTACK )
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_SOWRD_ROTATE_ATTACK);

					pMob->Attacked();
				}
			}

			// nate 2005-07-21 : Burning Crash - 동작 이어가기
			if( m_nMotionState == CHA_B_BURNINGCRASH)
			{
				if(BurningCrashEndFrame(m_byBurningStep) <= m_CurFrame)
				{
					if(pMob && (pMob->IsDead()
								|| (g_pRoh == this
									&& DistPlToPlAbs( m_wx,  m_wz, pMob->m_wx, pMob->m_wz) > 40 )))
						m_AttackMobId = -1;
				}
			}
			else if( m_nMotionState == CHA_C_HELLDOWN )
			{
				if( pMob
						&& (pMob->IsDead()
							|| (g_pRoh == this
								&& (DistPlToPlAbs(m_wx,  m_wz, pMob->m_wx, pMob->m_wz) > 215
									|| m_bAttacked ))))
					HellDownFail( NULL, pMob );

				if( pMob
						&& g_pRoh == this
						&& DistPlToPlAbs(m_wx,  m_wz, pMob->m_wx, pMob->m_wz) > 215 )
					g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_SKILL_RANGEOUT));

				if( m_nFailStep == -1)
					HellDownFail( NULL, pMob );
			}
			else if( !m_DoubleAttack )
				m_AttackMobId = -1;
		}

		//mungmae-2005/05/13
		//설명:주무기를 쏠때 보조무기를 쏘는 모션과 효과를 따로 넣어서 보조무기 고유의 속도를 따라가지 않고
		//	   주무기의 속도를 따라가게 되어, 시간 딜레이를 주어 임시적으로 보조무기 고유의 속도를 비슷하게
		//	   맞추어 놓았다.
		if(m_DoubleAttack)
		{
			if( m_Wearing[WEARING_CLOAK] )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
				if( pMob )
				{
					switch(m_Wearing[WEARING_CLOAK]->m_Num9)
					{
					case 1://충전포
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_CHARGECANNON);//충전포
						break;
					case 2:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_FLAME);//화염방사기
						break;
					case 3:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_AUTOLASER);//레이저
						break;
					case 4:
						//pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_AUTOLASER);//지뢰투석기.우선 레이저 효과
						break;
					case 5:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SMALL_ROCKET);
						break;
					case 6:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_MIDDLE_ROCKET);
						break;
					case 7:	//wan:2005-01	//레피드 파이어(핵런쳐)
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_WARGIANT_MULTI);//레피드 파이어(핵런쳐)
						break;
					case 0:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_GATHERING);
						break;
					default:
						break;
					}
				}
			}
			m_DoubleAttack=FALSE;
			m_fSubAttackTime = fTime;
		}

		///2003 캐릭이 특수공격 성공시 효과 나타내기
		if(m_ChaAttackType) //카운트
		{
			if (pMob)
			{
				pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_HITPOWER);
				m_ChaAttackType = 0;
			}
		}

	}

	else if (m_AttackChaId >= 0)
	{
		CNkCharacter * pCharAttacked = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);

		if( m_nMotionState == CHA_AIDIA_SUB
				|| m_nMotionState == CHA_KAI_SUB
				|| m_nMotionState == CHA_AIDIA_PIPE )
		{
			if (m_PrevFrame < m_CurFrame &&	m_PrevFrame < MAGIC_FRAME && m_CurFrame >= MAGIC_FRAME)
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);
				pCharAttacked = pCha;
				if (pCha)
					pCha->Attacked();
				m_AttackChaId = -1;
			}
		}
		else if( ( m_Wearing[WEARING_WEAPON]
				   && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_RING
				   && m_nMotionState != CHA_A_SUMMONFIRE && m_nMotionState != CHA_A_RESURRECTION )
				 || ( m_Wearing[WEARING_SHIELD]
					  && GET_TYPE(m_Wearing[WEARING_SHIELD]) == ITYPE_SUB
					  && GET_SHAPE(m_Wearing[WEARING_SHIELD]) == ISUB_WAND
					  && m_nMotionState == CHA_MAGIC_MOVE ))
		{
			// 링을 쓰고, 마법쏠때가 되면 쏘자!!
			if (m_PrevFrame < m_CurFrame &&	m_PrevFrame < MAGIC_FRAME && m_CurFrame >= MAGIC_FRAME)
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);
				if (pCha)
				{
					//CHA_MAGIC_MOVE
					if(m_nMotionState == CHA_MAGIC_MOVE)
					{
						int insertEffectNum = m_Wearing[WEARING_SHIELD]->m_Vnum;
						switch(insertEffectNum)
						{
						case 3065:
							insertEffectNum = EFFECT_SKILL_WAND_WAVE_OF_DEATH;
							break;
						case 10372:
							insertEffectNum = EFFECT_SKILL_WAND_ARROW;
							break;
						case 10415:
							insertEffectNum = EFFECT_SKILL_WAND_METEORITE;
							break;
						case 11063:
							insertEffectNum = EFFECT_SKILL_WAND_ICE_STORM;
							break;
						case 11084:
							insertEffectNum = EFFECT_SKILL_WAND_CHAOS;
							break;
						default:
							insertEffectNum = m_Wearing[WEARING_SHIELD]->m_Vnum+1000;
							break;
						}

						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, insertEffectNum);
					}
					else
					{
						// [12/3/2008 Theodoric] 이전 방법은 스킬번호가 아이템 번호에 + 1000 이였다.
						// 무기가 증가에 따라 스킬 번호가 중복 가능성이 있어서 수정
						int insertEffectNum = m_Wearing[WEARING_WEAPON]->m_Vnum;
						switch(  insertEffectNum )
						{
						case 2053:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA;
							break; // [12/3/2008 Theodoric]  사릉가
						case 2177:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA2;
							break; // 신의 사릉가
						case 2265:
							insertEffectNum = EFFECT_SKILL_RING_RAVEN;
							break; // 견습생의 레이븐 링
						case 2270:
							insertEffectNum = EFFECT_SKILL_RING_NEO;
							break; // 견습생의 네오스네이크 링
						case 2275:
							insertEffectNum = EFFECT_SKILL_RING_KAISEN;
							break; // 견습생의 카이젠 링
						case 2280:
							insertEffectNum = EFFECT_SKILL_RING_SHININGEAGLE;
							break; // 견습생의 피닉스 링
						case 2285:
							insertEffectNum = EFFECT_SKILL_RING_SPHINX;
							break; // 견습생의 스핑크스 링
						case 2292:
							insertEffectNum = EFFECT_SKILL_RING_DRAGON;
							break; // 견습생의 드래곤 링			// 견습생 링 이펙트 관련 인덱스 매칭 문제 수정 by Sol9 [2012.06.01]
						case 2698:
							insertEffectNum = EFFECT_SKILL_RING_BULKAN;
							break;
						case 2747:
							insertEffectNum = EFFECT_SKILL_RING_GRIFFON;
							break;	// 360lv 바리사다
						case 2924:
							insertEffectNum = EFFECT_SKILL_RING_GRYPHON;
							break;	// 사-그리폰
						case 3288:
							insertEffectNum = EFFECT_SKILL_RING_DEVIL2;
							break;	// 진데빌링 & 사 데빌링
						case 10458: // AIDIA LEVEL 450
							insertEffectNum = EFFECT_SKILL_RING_YETI;
							break;
						case 10466: // AIDIA LEVEL 500
							insertEffectNum = EFFECT_SKILL_RING_PEAKOCK;
							break;
						case 10473: // AIDIA LEVEL 550
							insertEffectNum = EFFECT_SKILL_RING_TIGER;
							break;
						case 10481: // AIDIA LEVEL 600
							insertEffectNum = EFFECT_SKILL_RING_UNICORN;
							break;	
						case 10526: // AIDIA LEVEL 650
							insertEffectNum = EFFECT_SKILL_RING_650;
							break;	
						case 10534: // AIDIA LEVEL 700
							insertEffectNum = EFFECT_SKILL_RING_700;
							break;	
					    case 10542: // AIDIA LEVEL 750
							insertEffectNum = EFFECT_SKILL_RING_750;
							break;	
						case 10550: // AIDIA LEVEL 800
							insertEffectNum = EFFECT_SKILL_RING_800;
							break;	
						case 10558: // AIDIA LEVEL 850
							insertEffectNum = EFFECT_SKILL_RING_850;
							break;	
						case 10566: // AIDIA LEVEL 870
							insertEffectNum = EFFECT_SKILL_RING_900;
							break;	
						case 10574: // AIDIA LEVEL 890
							insertEffectNum = EFFECT_SKILL_RING_950;
							break;	
						case 10727: // AIDIA LEVEL 910
							insertEffectNum = EFFECT_SKILL_RING_1000;
							break;	
						case 10707: // AIDIA CHRISTMAS
							insertEffectNum = EFFECT_SKILL_RING_CHR;
							break;
						case 10843: // AIDIA CHINESE
							insertEffectNum = EFFECT_SKILL_RING_CHIN;
							break;
						default:
							insertEffectNum += 1000;
							break; // 이전 사용 스킬들
						}

						if( m_nMotionState )
							pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, insertEffectNum);

					}
				}
				m_AttackChaId = -1;
			}
		}
		// nate 2005-11-10 : 포이즌 - InsertMagic
		else if( m_nMotionState == CHA_H_ELECTRICSHOT			// nate 2005-11-23 : 일렉트릭 샷 - InsertMagic
				 || m_Wearing[WEARING_WEAPON]
				 && ( m_nMotionState == CHA_K_POISON				// nate 2005-11-10 : 포이즌 - InsertMagic
					  || m_nMotionState == CHA_F_SHADOWSLASH		//mungmae 2005/11/16 : 쉐도우 슬래쉬 - InsertMagic
					  || m_nMotionState == CHA_B_FLYINGCRASH))	//mungmae 2005/11/16 : 플라잉 크래쉬 - 어택 프레임때 이펙트 넣기
		{
			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
				if( pCha )
				{
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, m_nMotionState - 1);
					if( g_pRoh == this
							&& ( m_nMotionState == CHA_H_ELECTRICSHOT
								 || m_nMotionState == CHA_F_SHADOWSLASH ))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 1, 0);
					}
				}

				m_AttackChaId = -1;
			}
		}
		else if( m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER
				 && m_nMotionState == CHA_F_ELECTRICHAMMER )
		{
			int target_count = 5;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[128] = "";

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );

				if(pCha && 120 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pCha->m_nCharIndex);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pCha,  fDist, 30, false, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicCha(g_pTargetCha[i]);
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 1, vecMobIndex);
					}
				}
				
				if ( g_pRoh == this )
				{
					G_SK_SetCooltime(m_CurAttackType);					
				}

				m_AttackChaId =-1;
			}
		}
		else if( m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER
				 && m_nMotionState == CHA_F_TONADOHAMMER )
		{
			int target_count = 2;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[128] = "";
			float x, y, z;
			x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
			y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
			z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;

			if( m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 17 && m_CurFrame >= 17 ) || ( m_PrevFrame < 25 && m_CurFrame >= 25 )
											  || ( m_PrevFrame < 33 && m_CurFrame >= 33 ) ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );

				if( g_pDSound )
					g_pDSound->Play(g_EffectSound[EFF_SND_TONADO_HAMMER], x, y, z);

				if(pCha && 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{
					std::vector<int> vecMobIndex;
					
					vecMobIndex.push_back(pCha->m_nCharIndex);

					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pCha,  fDist, 30, false, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicCha(g_pTargetCha[i]);
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
							
							if( g_pDSound )
								g_pDSound->Play(g_EffectSound[EFF_SND_TONADO_HAMMER], x, y, z);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 1, vecMobIndex);
					}
				}
				else if(pCha && 40 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{	
					m_AttackChaId =-1;

					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, -1, 0);
					}
				}
				
				if ( g_pRoh == this )
				{
					G_SK_SetCooltime(m_CurAttackType);					
				}
			}
		}
		else if( m_nMotionState == CHA_A_SUMMONFIRE )
		{
			int target_count = 5;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[128] = "";

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );

				if(pCha && 135 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{	
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pCha->m_nCharIndex);

					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pCha,  fDist, 30, false, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicCha(g_pTargetCha[i]);
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 1, vecMobIndex);
					}
				}
				else if(pCha && 135 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{	
					m_AttackChaId =-1;

					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, -1, 0);
					}					
				}
				
				if ( g_pRoh == this )
				{
					G_SK_SetCooltime(m_CurAttackType);					
				}
			}
		}		
		else if( m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN
				 && m_nMotionState == CHA_H_RANDOMFIRE_ME )
		{
			int nSkillLevel = g_ChaSkill.m_SkillLevel[ 18 ];
			int target_count = 5;	// 주변 Target 개수
			int att_count = 0;		// 다중 스킬의 Target 한계 개수
			float fDist = 0.0f;
			char strTemp[128] = "";

			int nSkillDistance = pCMyApp->GetSkillDistance(SKILL_H_RANDOMFIRE);

			if( g_pDSound )
			{
				float x, y, z;
				x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
				y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
				z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_RANDOM_SHOT], x, y, z);
			}

			pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );

			if( nSkillLevel <= 10 && m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 22 && m_CurFrame >= 22 ) || ( m_PrevFrame < 38 && m_CurFrame >= 38 )
					|| ( m_PrevFrame < 54 && m_CurFrame >= 54 ) ) )
			{
				if(pCha && !pCha->IsDead()
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pCha->m_nCharIndex);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pCha,  fDist, 60, false, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicCha(g_pTargetCha[i]);
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 1, vecMobIndex);
					}
				}
				else if(pCha && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{	
					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, -1, 0);
					}
				}
			}
			if( nSkillLevel > 10 && nSkillLevel <= 20 && m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 22 && m_CurFrame >= 22 ) || ( m_PrevFrame < 38 && m_CurFrame >= 38 )
					|| ( m_PrevFrame < 54 && m_CurFrame >= 54 ) || ( m_PrevFrame < 70 && m_CurFrame >= 70 ) ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );

				if(pCha && !pCha->IsDead()
						&& 135 >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pCha->m_nCharIndex);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pCha,  fDist, 60, false, target_count);
												
						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicCha(g_pTargetCha[i]);
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 1, vecMobIndex);
					}
				}
				else if(pCha && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{
					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, -1, 0);
					}
				}				
			}

			if( nSkillLevel > 20 && nSkillLevel <= 35 && m_PrevFrame < m_CurFrame && ( ( m_PrevFrame < 22 && m_CurFrame >= 22 ) || ( m_PrevFrame < 38 && m_CurFrame >= 38 )
					|| ( m_PrevFrame < 54 && m_CurFrame >= 54 ) || ( m_PrevFrame < 70 && m_CurFrame >= 70 ) || ( m_PrevFrame < 86 && m_CurFrame >= 86 ) ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );				

				if(pCha && !pCha->IsDead()
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{	
					std::vector<int> vecMobIndex;

					vecMobIndex.push_back(pCha->m_nCharIndex);
					
					if(target_count > 0)
					{
						int nCount = pCMyApp->m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, pCha,  fDist, 60, false, target_count);

						for (int i = 0; i < nCount; i++)
						{
							g_pRoh->AddMagicCha(g_pTargetCha[i]);
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
						}
					}

					if( g_pRoh == this )
					{
						SendCharUseSkill_MultiTargetStep(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 1, vecMobIndex);
					}
				}
				else if(pCha && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha 공격
				{
					if( g_pRoh == this )
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, -1, 0);
					}
				}

				if ( g_pRoh == this )
				{
					G_SK_SetCooltime(m_CurAttackType);					
				}
			}
			if( ( 17 <= m_CurFrame && m_CurFrame >= 17 + 5 ) &&
					GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
			{
				D3DMATRIX	matRot;
				D3DVECTOR	vTmp;
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_RANDOMSHOOT;

				m_dir = m_toDir = (float) atan2(m_tz - m_wz, m_tx - m_wx);

				Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->m_LineVector[0], matRot);
				ef_sort.vPos = vTmp;
				ef_sort.vPos.x += m_wx;
				ef_sort.vPos.y += m_wy;
				ef_sort.vPos.z += m_wz;
				ef_sort.dwColor = 0xFFFFFFFF;
				ef_sort.nCurFrame = m_CurFrame - g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;
				ef_sort.bOneFrame = TRUE;

				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}
			if( pCha && pCha->IsDead() )
				SetMotionState(CHA_STAND);
		}
		else if( m_nMotionState == CHA_H_RANDOMFIRE_YOU )
		{
			if( g_pDSound )
			{
				float x, y, z;
				x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
				y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
				z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;
				g_pDSound->Play(g_EffectSound[EFF_SND_RANDOM_SHOT], x, y, z);
			}

			//
			if( m_Wearing[WEARING_WEAPON] != NULL )
			{
				if( ( 0 <= m_CurFrame && m_CurFrame >= 5 ) &&
						GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				{
					D3DMATRIX	matRot;
					D3DVECTOR	vTmp;
					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));

					ef_sort.nType = FX_RANDOMSHOOT;

					m_dir = m_toDir = (float) atan2(m_tz - m_wz, m_tx - m_wx);

					Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
					D3DMath_VectorMatrixMultiply(vTmp, GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->m_LineVector[0], matRot);
					ef_sort.vPos = vTmp;
					ef_sort.vPos.x += m_wx;
					ef_sort.vPos.y += m_wy;
					ef_sort.vPos.z += m_wz;
					ef_sort.dwColor = 0xFFFFFFFF;
					ef_sort.nCurFrame = m_CurFrame - g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;
					ef_sort.bOneFrame = TRUE;

					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					//		m_AttackChaId =-1;
				}
			}
		}
		else if( m_nMotionState == CHA_F_MERCERNERYSLASH
				 && m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD )
		{
			if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 12 && m_CurFrame >= 12 ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
				if( pCha )
				{
					if( g_pRoh == this && pCha
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))
					{	
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 2, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 25 && m_CurFrame >= 25 ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
				if( pCha )
				{
					if( g_pRoh == this && pCha
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 3, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 32 && m_CurFrame >= 32 ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
				if( pCha )
				{
					if( g_pRoh == this && pCha
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 4, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 42 && m_CurFrame >= 42 ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
				if( pCha )
				{
					if( g_pRoh == this && pCha
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 5, 10);
					}
				}
			}
			else if( m_PrevFrame < m_CurFrame && ( m_PrevFrame < 58 && m_CurFrame >= 58 ) )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );
				if( pCha )
				{
					if( g_pRoh == this && pCha
							&& 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))
					{
						SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, 6, 10);
					}
				}
			}
			if( pCha
					&& 40 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))
			{
				if( g_pRoh == this )
				{
					SendCharUseSkill_TargetValue2(m_CurAttackType, eCharType_Char, pCha->m_nCharIndex, -1, 0);
				}
				m_AttackChaId =-1;
			}
		}
		else if ( (m_nMotionState != CHA_ATTACK
				   && m_nMotionState != CHA_JUMP_ATTACK && m_nMotionState != CHA_ROTATE_ATTACK
				   && m_nMotionState != CHA_THROW_SLAYER && m_nMotionState != CHA_THROW_CHAKRAM
				   && m_nMotionState != CHA_HANDLE )
				  || (g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame <= m_CurFrame) )
		{
			pCha = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);

			if (pCha)
			{
				if (m_nMotionState == CHA_THROW_SLAYER)
				{
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_SLAYER);
					pCha->Attacked();
				}
				else if (this == g_pRoh && m_nMotionState == CHA_THROW_CHAKRAM && pCMyApp->m_pUIMgr
						 && pCMyApp->m_pUIMgr->m_ChakramSlotNum >= 0 && m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum] )
				{
					int proto_num = g_ItemList.FindItem(693);
					if (proto_num >= 0)
					{
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL,proto_num+2000 );
					}
				}
				else if(this != g_pRoh && m_nMotionState == CHA_THROW_CHAKRAM)
				{
					int proto_num = g_ItemList.FindItem(693);
					if (proto_num >= 0)
					{
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL,proto_num+2000 );
					}
				}
				else if (m_nMotionState == CHA_KAI_TOUCH && m_Wearing[WEARING_SHIELD])
				{
					if( m_CurAttackType == SKILL_VANPIRE_TOUCH )
						pCMyApp->m_pUIMgr->InsertMagic(pCha, NULL, this, NULL, EFFECT_SKILL_VANPIRE_TOUCH);
					else if( m_CurAttackType == SKILL_HELL_TOUCH )
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_HELL_TOUCH);

					pCha->Attacked();
				}
				else if( (m_nMotionState == CHA_HUMAN_DEEP || m_nMotionState == CHA_HUMAN_SHORT)
						 && m_Wearing[WEARING_SHIELD])
				{}
				//------------------------------------------------------------------
				// 각 무기의 Effct 추가 (이펙트)
				// 캐릭을 대상으로 한다.
				else if ( m_Wearing[WEARING_WEAPON] )
				{
					// [3/27/2007 Theodoric] 태국 송크란 이벤트
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1538 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1539 )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_SONGKRAN);
					}

					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1531 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1532
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1533 ) // 입술(화이트) : 딸기, 레몬, 멜론 캔디
					{
						// 2007.03.14 화이트
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_LIP);
					}

					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1534 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1535 )
						// 입술(화이트), LOVE : ( 레인보우 홀, 레인보우 캔디 )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_LIP);
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_LOVE);
					}
					///하트로드 추가
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1514 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1515
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1517 ) // 하트 로드,러브러브 로드
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_HEART);
					}


					/* // 일본측의 요청으로 뱀파이어 능력 발동시만 귀신이 나오도록 수정.
										///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
										if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1590 ) // 무라마사로 때렸으면...
										{
											pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pCha, NULL, EFFECT_MURAMASA_GHOST );
										}
					*/
					// 이도류 추가. Hit Effect
					if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD)
					{
						if( m_nMotionState == CHA_ATTACK )
							pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_DSWORD);
					}
					// 일반 검 , 햄머
					else if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD
							 || GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER )
					{
						if( m_nMotionState == CHA_ATTACK )
						{
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_SWORD);
						}
						else if( m_nMotionState == CHA_ROTATE_ATTACK || m_nMotionState == CHA_JUMP_ATTACK)
						{
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_SOWRD_ROTATE_ATTACK);
						}
					}
					// 도끼
					else if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE )
					{
						if( m_nMotionState == CHA_ATTACK )
						{
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_AXE);
						}
						else if( m_nMotionState == CHA_ROTATE_ATTACK || m_nMotionState == CHA_JUMP_ATTACK)
						{
							if( m_CurAttackType == SKILL_FORCE_ATTACK )
								pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_MOB_SHILONMASTER);
							else
								pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_AXE_JUMP_ATTACK);
						}
					}
					// 지팡이
					else if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_STAFF )
					{
						if( m_nMotionState == CHA_ATTACK )
							pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_STAFF);
					}
				}
				else
				{
					if( m_nMotionState == CHA_ATTACK )
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_HAND);
				}

				pCha->Attacked();
			}
			// nate 2005-07-21 : Burning Crash - 동작 이어가기
			if(m_nMotionState == CHA_B_BURNINGCRASH)
			{
				if(BurningCrashEndFrame(m_byBurningStep) <= m_CurFrame)
				{
					if(pCha
							&& (pCha->IsDead()
								|| (g_pRoh == this
									&& DistPlToPlAbs( m_wx,  m_wz, pCha->m_wx, pCha->m_wz) > 40 )))
						m_AttackChaId = -1;
				}
			}
			else if( m_nMotionState == CHA_C_HELLDOWN )
			{
				if(pCha
						&& (pCha->IsDead()
							|| (g_pRoh == this
								&& (DistPlToPlAbs(m_wx, m_wz, pCha->m_wx, pCha->m_wz) > 215
									|| m_bAttacked ))))
					HellDownFail( pCha, NULL );

				if( pCha
						&& g_pRoh == this
						&& DistPlToPlAbs(m_wx, m_wz, pCha->m_wx, pCha->m_wz) > 215 )
					g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_SKILL_RANGEOUT));
			}
			else if( !m_DoubleAttack )
				m_AttackChaId = -1;

			if( m_nFailStep == -1 )
				HellDownFail( pCha, NULL, FALSE );
		}

		if(m_DoubleAttack)
		{
			pCha = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);
			if( pCha )
			{
				switch(m_Wearing[WEARING_CLOAK]->m_Num9)
				{
				case 1://충전포
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_CHARGECANNON);//충전포
					break;
				case 2:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_FLAME);//화염방사기
					break;
				case 3:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_AUTOLASER);//레이저
					break;
				case 4:
					//pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_AUTOLASER);//지뢰투석기.우선 레이저 효과
					break;
				case 5:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SMALL_ROCKET);//소형로켓
					break;
				case 6:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_MIDDLE_ROCKET);
					break;
				case 7://wan:2005-01	//레피드 파이어(핵런쳐)
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_WARGIANT_MULTI);//레피드 파이어(핵런쳐)
					break;
				case 0:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_GATHERING);
					break;
				default:
					break;
				}
			}
			m_DoubleAttack=FALSE;
			m_fSubAttackTime = fTime;
		}

		///2003 캐릭터가 캐릭에게 특수공격 성공시 효과 나타내기
		if(m_ChaAttackType) //카운트
		{
			if (pCharAttacked)
			{
				pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCharAttacked, NULL, EFFECT_HITPOWER);
			}
			m_ChaAttackType = 0;
		}
	}

	if (m_toDir > -100 && m_toDir != m_dir)
	{
		if (m_dir - m_toDir > g_PI)
		{
			m_dir -= g_2_PI;
		}
		else if (m_toDir - m_dir > g_PI)
		{
			m_dir += g_2_PI;
		}

		if (m_dir > m_toDir)
		{
			m_dir -= (dwCurtime-m_dwBaseTime) * (g_PI / 1000);

			if (m_dir < m_toDir)
				m_dir = m_toDir;
		}
		else
		{
			m_dir += (dwCurtime-m_dwBaseTime) * (g_PI / 1000);
			if (m_dir > m_toDir)
				m_dir = m_toDir;
		}
	}

	// 무기의 스피드를 바꿔서 렌더링한다 !
	float aniRate = m_AniRate;

	if (m_Wearing[WEARING_WEAPON] ||
			(m_Wearing[WEARING_SHIELD]&& GET_SHAPE(m_Wearing[WEARING_SHIELD])==ISUB_WAND
			 && m_nMotionState == CHA_MAGIC_MOVE))
	{

		if (m_nMotionState == CHA_ATTACK
				|| m_nMotionState == CHA_ROTATE_ATTACK
				|| m_nMotionState == CHA_MAGIC_MOVE)
		{
			int AttackSpeedType = 0; //방패슬롯에 있는 완드의 공속 적용을 위해...

			if(m_Wearing[WEARING_SHIELD] && m_nMotionState == CHA_MAGIC_MOVE)
				AttackSpeedType = m_Wearing[WEARING_SHIELD]->m_Speed;
			else
				AttackSpeedType = m_Wearing[WEARING_WEAPON]->m_Speed;

			//--------------------------------------------------------------------
			// 무기 스피드에 따른 Frame
			// 2005.01.05 / Lyul
			// 헤이스트 스킬 마스터일 경우
			// 이도류
			if(m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && AttackSpeedType>0)
			{
				if(m_bhaste==1 && m_Race ==RACE_FREAK)
					AttackSpeedType--;
			}

			switch (AttackSpeedType)
			{
			case ISPEED_VERY_FAST:
				if (m_RenderMultiShot >= 2)
				{
					aniRate = 0.0231f;
				}
				else if (m_RenderMultiShot >= 1)
				{
					aniRate = 0.0261f;
				}
				else
				{
					aniRate = 0.03f;
				}

				if (m_RenderMultiShot < 0)
				{
					aniRate = 0.05f;
				}
				break;
			case ISPEED_FAST:
				if (m_RenderMultiShot >= 2)
				{
					aniRate = 0.0308f;
				}
				else if (m_RenderMultiShot >= 1)
				{
					aniRate = 0.0348f;
				}
				else
				{
					aniRate = 0.04f;
				}

				if (m_RenderMultiShot < 0)
				{
					aniRate = 0.06f;
				}
				break;
			case ISPEED_SLOW:
				if (m_RenderMultiShot >= 2)
				{
					aniRate = 0.0462f;
				}
				else if (m_RenderMultiShot >= 1)
				{
					aniRate = 0.0522f;
				}
				else
				{
					aniRate = 0.06f;
				}

				if (m_RenderMultiShot < 0)
				{
					aniRate = 0.07f;
				}
				break;
			case ISPEED_VERY_SLOW:
				if (m_RenderMultiShot >= 2)
				{
					aniRate = 0.0538f;
				}
				else if (m_RenderMultiShot >= 1)
				{
					aniRate = 0.0609f;
				}
				else
				{
					aniRate = 0.07f;
				}

				if (m_RenderMultiShot < 0)
				{
					aniRate = 0.07f;
				}
				break;
			default:
				if (m_RenderMultiShot >= 2)
				{
					aniRate = 0.0385f;
				}
				else if (m_RenderMultiShot >= 1)
				{
					aniRate = 0.0435f;
				}
				else
				{
					aniRate = 0.05f;
				}

				if (m_RenderMultiShot < 0)
				{
					aniRate = 0.07f;
				}
				break;
			}
		}

		// 공격, 찍기, 돌려베기 를 할때, 칼, 도끼를 가지고 있으면 26 프레임짜리다.
		// 찍기할때 맨손도 있지만 무시하자!
		//-------------------------------------------------------------------------------------
		// 무기에 대한 속도 변경
		//-------------------------------------------------------------------------------------
		// 2005.01.04 / Lyul 이도류 추가
		if (m_nMotionState == CHA_ATTACK
				|| m_nMotionState == CHA_JUMP_ATTACK
				|| m_nMotionState == CHA_ROTATE_ATTACK
				|| m_nMotionState == CHA_THROW_CHAKRAM)
		{
			// 검, 도끼 , 차크람
			if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD
					|| GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE
					|| ( m_Wearing[WEARING_CLOAK]
						 && GET_SHAPE(m_Wearing[WEARING_CLOAK]) == ISUB_CHAKRAM ) )
			{
				aniRate /= (26.0f/20.0f);
			}
		}
	}

	if (m_nMotionState == CHA_MAGIC)
	{
		if (m_Wearing[WEARING_WEAPON] && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_STAFF)
		{
			aniRate = 0.05f - (0.0333333f * m_Wearing[WEARING_WEAPON]->m_MagicAttackSpeed / 100);

			if (aniRate < 0.03f)
				aniRate = 0.03f;
		}
		switch (m_MagicNum)
		{
		case SKILL_FIRE_BALL:
			aniRate *= 1.5f;
			break;
		case SKILL_FIRE_PILLAR:
			aniRate *= 1.6f;
			break;
		case SKILL_STAR_FLARE:
			aniRate *= 1.7f;
			break;
		}
	}

	/////2003 0924
	if(m_CancelState==1)	//상태가 걸려있다면,
	{
		if(m_ChaCondition==CONDITION_FREEZE)//냉각이라면
			aniRate *= 2.0f; //속도 저하.
	}

	// 꼭지점 댄스 속도 조절
	if( m_nMotionState==CHA_SOCIAL && (m_ActNum>=153&&m_ActNum<=158) ) // 꼭지점 댄스 스페이스 표시 체크.
	{
		aniRate = 0.035f; // 좀 더 빠르게
//		aniRate = 0.022f; // 좀 더 빠르게
	}


	frame_count = 0;
	if ( ((dwCurtime - m_dwBaseTime) * 0.001f) >= aniRate)
	{
		m_dwBaseTime += (DWORD) (aniRate*1000);
		frame_count = 1;
		while ( ((dwCurtime - m_dwBaseTime)*0.001f) >= aniRate )
		{
			frame_count++;
			m_dwBaseTime += (DWORD) (aniRate*1000);
		}
		// 이동 거리 계산.. 음.. 수정 필요...
	}

	dwTimeCap = dwCurtime - m_dwTimeCapPrev;
	m_dwTimeCapPrev = dwCurtime;

	// 이동중이라면
	if (m_bMoving)////////
	{
		float tx = (m_dx * ((float)dwTimeCap / 50.0f));
		float tz = (m_dz * ((float)dwTimeCap / 50.0f));
		m_wx += tx;
		m_wz += tz;

		// 1/4
		if ( m_dx >= 0 && m_dz >= 0)
		{
			if (m_wx >= m_tx && m_wz >= m_tz)
			{
				bArrived = TRUE;
			}
		} // 2 / 4
		else if ( m_dx < 0 && m_dz >= 0)
		{
			if (m_wx <= m_tx && m_wz >= m_tz)
			{
				bArrived = TRUE;
			}
		} // 3/4
		else if ( m_dx <= 0 && m_dz <= 0)
		{
			if (m_wx <= m_tx && m_wz <= m_tz)
			{
				bArrived = TRUE;
			}
		} // 4/4
		else if ( m_dx >= 0 && m_dz <= 0)
		{
			if (m_wx >= m_tx && m_wz <= m_tz)
			{
				bArrived = TRUE;
			}
		}

		if (bArrived)
		{
			m_wx = m_tx;
			m_wz = m_tz;

			m_bMoving = FALSE;
		}

		if (m_bMoving == FALSE)
		{
			// 스테이트를 바꾸어 주어야 한다.
			SetMotionState(CHA_STAND);
			// place 명령에 의한 무브였으면 방향을 보정해줘야 한다.
			if (m_bPlace)
			{
				m_bPlace = FALSE;
				m_toDir = m_fPlaceDir;
			}
		}
		// 2004 check later
		if( g_bNewTerrain )
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight( m_wx, m_wz );
			m_wy = CNKObjQuadNode::GetHeightOnObject( m_wz, m_wx, m_wy );

			if( m_wy < fHeight )
				m_wy = fHeight;
		}
		else
			m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);

	}
	else
	{
		// 캐력터를 게임 전에 그려주는 것이 있으므로..
		// 그것을 확인해주어야 한다.
		if (pCMyApp->GetLandPtr())
		{
			if( g_bNewTerrain )
			{
				float	fHeight = pCMyApp->GetLandPtr()->GetHFHeight( m_wx, m_wz );
				m_wy = CNKObjQuadNode::GetHeightOnObject( m_wz, m_wx, m_wy );
				if( m_wy < fHeight )
					m_wy = fHeight;
			}
			else
				m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);
		}
	}
	// nate 2005-08-31 : 맵 영역 밖에서 생성되는 버그때문에
	//----------------------------------------------------------------------
	// 종료 모션에서 다음 모션을 결정한다.
	//----------------------------------------------------------------------
	// 공격을 위해 이동하는 것은 제외
	// 아 이거 로직 드릅네.
	if(pCMyApp->m_pUIMgr)
	{
		// 타 캐릭이 공격중이거나, 자 캐릭이 공격위한 아동, 대쉬, 디바인 스톤, 타임차지일 경우

		if(( g_pRoh != this
				&& (m_bAttack
					|| m_nMotionState == CHA_F_STONEBASH		// 다른 캐릭터 망치콤보 모션 정지를 위해.. ///070723 일본버그리포트 수정
					|| m_nMotionState == CHA_F_BASHREFLECT		// 다른 캐릭터 망치콤보 모션 정지를 위해.. ///070723 일본버그리포트 수정
					|| m_nMotionState == CHA_A_SUMMONSQUAD))		// nate 2005-07-15 : Summon Squad - 동작 멈춤
				|| ( g_pRoh != this
					 &&	( m_nMotionState == CHA_B_PROTECTION_SHIELD
						   || m_nMotionState == CHA_F_REFLECTION )
				   )
				|| ( g_pRoh == this
					 && (pCMyApp->m_pUIMgr->m_b_MotionForAttack
						 || m_nMotionState == CHA_F_DASH
						 || m_nMotionState == CHA_F_DSTONE
						 || m_nMotionState == CHA_F_TIMECHARGE
						 || m_nMotionState == CHA_F_REFLECTION		// mungmae-05/04/20 리플렉션 추가
						 || m_nMotionState == CHA_A_SUMMONSQUAD		// nate 2005-07-15 : Summon Squad - 동작 멈춤
						 || m_nMotionState == CHA_H_AUTOMINE
						 || m_nMotionState == CHA_SPIRITMODE
						 || m_nMotionState == CHA_A_RESURRECTION // PK시 상대 죽었을때 공격 캔슬때문에 리저렉션에 예외처리 추가. ///07-05-31 원석 추가
						 || m_nMotionState == CHA_B_PROTECTION_SHIELD
						))
				|| m_nMotionState == LHCha_Action__Perom_S10 // 필드?
				|| m_nMotionState == LHCha_Action__Perom_S15 // 필드?
		  )
		{
			if ( m_Biped && (m_CurFrame+frame_count) >= m_Biped->m_AnimCount)
			{
				// m_MultiShot : 휴먼 연사
				if (m_MultiShot < 0
						|| m_RenderMultiShot < 0)
					SetSnifing(FALSE);

				// 캐릭이 플레이어라면 조건 판단 후 모션 결정
				if (g_pRoh == this)
				{
					// 공격 중이 아니고 공격을 위한 이동도 아니라면 STAND
					if( !pCMyApp->CompareHeroAttGo()
							&& !pCMyApp->m_pUIMgr->m_b_MovingForAttack	)
					{
						// m_b_MovingForAttack이 false 이어야 들어오는 루틴 아닌가?
						pCMyApp->m_pUIMgr->m_b_MotionForAttack = false;

						if( m_nMotionState == CHA_C_HELLDOWN2
								|| m_nMotionState == CHA_C_HELLDOWN )
							pCMyApp->m_pUIMgr->m_byCheckHellDown = 1;	//  0 : Useing HellDown

						if( !( m_nMotionState == CHA_C_HELLDOWN
								&& pCMyApp->m_pUIMgr->m_bRClickDown == 2
								&& pCMyApp->m_bHellDownReady ) )
							SetMotionState(CHA_STAND);
					}
				}
				// 캐릭이 다른 유저라면 무조건 STAND
				else
				{
					if( m_nMotionState != CHA_C_HELLDOWN )
						SetMotionState(CHA_STAND);
				}
			}
		}
	}
	///////////////////////hitpoints//////////////////////////////
	if(this->ishit())
	{
		pCMyApp->DisplayChaHitpoints(this);
	}
//////////////////////////////////////////////////////////////
	if ( IsDead()
			&& pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !( pCMyApp->m_pUIMgr->m_pLand->IsDrawTile( m_wx, m_wz ) ) )
	{
		if (g_pRoh == this && m_DeadFrame > 40)
		{
			if (g_pTcpIp)
			{
				g_pTcpIp->SendNetMessage("revival\n");
			}
			else
			{
				m_Vital = m_MaxVital;
			}

			m_DeadFrame = -1;
		}
		/*	else if (m_DeadFrame > 0)
				m_wy -= m_DeadFrame * (500.0f / 100); // 100 Frame(2초) 동안 1000.0f을 가라 앉는다.
			else
				m_wy -= 1000.0f;
		*/
	}
	else if (m_DeadFrame > 60)
	{
		//mungmae-2005/06/23 뫼비우스 리젠 시간 설정
		if(g_pRoh == this && !g_pRoh->m_bNowFreePVP ) // 자유대련 중 죽은건 팝업 안띄운다. ///자유대련
		{
			m_dwReturnTime = timeGetTime();

			if( !pCMyApp->m_pUIMgr->m_dwRevivalAccTime )
				pCMyApp->m_pUIMgr->m_dwRevivalAccTime = timeGetTime();

			if( ( pCMyApp->m_nCurWorldIdx != WORLD_MATRIX
					&& pCMyApp->m_nCurWorldIdx != WORLD_MATRIX_2009
				) // 뫼비우스 맵이 아닐 경우
					|| ( (pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
						  || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
						 )
						 && g_pRoh->m_GuildLevel == 10 )	//뫼비우스 맵이고 지휘자
					|| ( (pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
						  || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
						 )
						 && !pCMyApp->m_pUIMgr->m_bOnBattle) )		//뫼비우스 맵이고 뫼비우스 않하고 있을 때
			{
				if (g_pNk2DFrame)
				{
					//	g_pTcpIp->SendNetMessage("revival\n");
					// IDS_30S_AFTER_REVIVAL "지금 부활 하시겠습니까? 30초후에 자동으로 귀환합니다."
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_30S_AFTER_REVIVAL), TYPE_NOR_OK, 35 );
				}
				else
					m_Vital = m_MaxVital;

				m_DeadFrame = -1;
				pCMyApp->m_pUIMgr->m_dwRevivalAccTime = 0; // 혹시 리젠바가 남아있을 수 있으니 초기화.
				m_bDrawRegenbar = FALSE;
			}

			else if( pCMyApp->m_pUIMgr->m_dwRevivalAccTime
					 && pCMyApp->m_pUIMgr->m_bOnBattle			//뫼비우스 하고 있을 때
					 && (pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
						 || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
						)
					 && g_pRoh->m_GuildLevel != 10 )    //지휘자가 아닐때
			{
				m_dwRegenTime = timeGetTime() - pCMyApp->m_pUIMgr->m_dwRevivalAccTime;
				m_bDrawRegenbar = TRUE;

				if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX )
				{
					if( m_Level >= 300 ) //레벨이 300이 넘냐? 아니냐?
					{
						if( MatrixTeamGrade(m_GuildIndex) == 1)		//죽었을 때 팀 순위가 1등일 때
							m_nRegenIndex = 10 + 20;
						else if( MatrixTeamGrade(m_GuildIndex) == 2)//죽었을 때 팀 순위가 2등일 때
							m_nRegenIndex = 10 + 10;
						else if( MatrixTeamGrade(m_GuildIndex) == 3)//죽었을 때 팀 순위가 3등일 때
							m_nRegenIndex = 10;
					}
					else
					{
						if( MatrixTeamGrade(m_GuildIndex) == 1)
							m_nRegenIndex = 5 + 20;
						else if( MatrixTeamGrade(m_GuildIndex) == 2)
							m_nRegenIndex = 5 + 10;
						else if( MatrixTeamGrade(m_GuildIndex) == 3)
							m_nRegenIndex = 5;
					}
				}
				else if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009 )
				{
					if( m_Level >= 300 ) //레벨이 300이 넘냐? 아니냐?
					{
						if( MatrixTeamGrade(m_GuildIndex) == 1)//죽었을 때 팀 순위가 1등일 때
							m_nRegenIndex = 10 + 10;
						else if( MatrixTeamGrade(m_GuildIndex) == 2)//죽었을 때 팀 순위가 2등일 때
							m_nRegenIndex = 10;
						else
							m_nRegenIndex = 20;

					}
					else
					{
						if( MatrixTeamGrade(m_GuildIndex) == 1)
							m_nRegenIndex = 5 + 10;
						else if( MatrixTeamGrade(m_GuildIndex) == 2)
							m_nRegenIndex = 5;
						else
							m_nRegenIndex = 15;
					}
				}

				//몇 초(m_nRegenIndex * 1000)가 지났을 때 살아나도록 한다.
				if( m_dwRegenTime > m_nRegenIndex * 1000 )
				{
					if( IsDead() )
					{
						if ( g_pTcpIp )
						{
							g_pTcpIp->SendNetMessage("revival\n");
							pCMyApp->m_bRegenStart = TRUE;
						}
						else
							m_Vital = m_MaxVital;

						m_DeadFrame = -1;
						pCMyApp->m_pUIMgr->m_dwRevivalAccTime = 0;
						m_bDrawRegenbar = FALSE;
					}
				}
			}
		}

		//	else
		//		m_wy -= (m_DeadFrame-60) * (6.0f / 40); // 60 Frame(3초) 후부터 40 Frame(2초) 동안 6.0f을 가라 앉는다.
	}

	if( g_pTcpIp && m_dwReturnTime && timeGetTime() - m_dwReturnTime > 30000 )
	{
		g_pTcpIp->SendNetMessage("revival\n");
		m_dwReturnTime = 0;
		g_pNk2DFrame->DeletePopup();
	}

	float	swim_height, fWaterHeight;
	// 수영 관련
	if( g_bNewTerrain )
	{
		fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx );

		if( m_Race == RACE_AIDIA )
			swim_height = fWaterHeight;
		else
			swim_height = fWaterHeight - 11.0f;
	}
	else
	{
		fWaterHeight = WATER_HEIGHT;

		if( m_Race == RACE_AIDIA )
			swim_height = WATER_HEIGHT;
		else
			swim_height = SWIM_HEIGHT;
	}

	// 90.0f 보다 낮으면 물...
	pCMyApp->SetHeroSwimming(FALSE);

	// 이동중
	if ((m_nMotionState == CHA_WALK || m_nMotionState == CHA_RUN) && m_wy < swim_height)
		SetMotionState(CHA_SWIM);
	else if (m_nMotionState == CHA_SWIM && m_wy >= swim_height)
		SetMotionState(CHA_WALK);

	// 정지 상태
	if (m_nMotionState == CHA_STAND && m_wy < swim_height)
		SetMotionState(CHA_SWIM_STAND);
	else if (m_nMotionState == CHA_SWIM_STAND && m_wy >= swim_height)
		SetMotionState(CHA_STAND);

	// 케릭이 수중에 있을 때
	if (m_nMotionState == CHA_SWIM || m_nMotionState == CHA_SWIM_STAND)
	{
		if (pCMyApp->m_nCurWorldIdx != WORLD_SKY)
			m_wy = fWaterHeight;

		pCMyApp->SetHeroSwimming(TRUE);
	}

	// SKY 존에서는 하늘 위에서 스케이트보드를 탄닷!
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
	{
		BOOL bSky = !(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz));

		if (m_nMotionState == CHA_WALK && bSky)
		{
			m_bRun = TRUE;
			SetMotionState(CHA_RUN);
		}
		if (bSky)
			m_wy += g_ChaFlyHeight[m_nFlyFrame];
	}

	// 돌려베기와 내려찍기 처리를 중간에 끼워넣음.
	if (m_nMotionState != CHA_C_HELLDOWN
			&& (m_nMotionState == CHA_MAGIC
				|| m_MagicChaCount > 0
				|| m_MagicMobCount > 0 ))
	{
		if( m_nMotionState != CHA_MAGIC
				|| ( m_PrevFrame < m_CurFrame
					 && m_PrevFrame < MAGIC_FRAME
					 && m_CurFrame >= MAGIC_FRAME) )
		{
			for (int i = 0; i < m_MagicChaCount; i++)
			{
				CNkCharacter *pCha = pCMyApp->m_pUIMgr->FindChaById(m_MagicChaId[i]);

				if( pCha )
				{
					// 정확한 방향으로 쏴야쥐~~!
					m_toDir = (float) atan2(pCha->m_wz - m_wz, pCha->m_wx - m_wx);

					if (m_nMotionState == CHA_JUMP_ATTACK
							|| m_nMotionState == CHA_ROTATE_ATTACK)
						pCha->Attacked();
					else
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, m_MagicNum); // 여기 m_MagicNum 이 -1일때 들어오는 경우가 있다.(06-04-28 원석)
				}

				m_MagicChaId[i] = -1;
			}// End of For

			for ( int i = 0; i < m_MagicMobCount; i++)
			{
				CNkMob *pMob = pCMyApp->m_pUIMgr->FindMobById(m_MagicMobId[i]);
				if (pMob)
				{
					// 정확한 방향으로 쏴야쥐~~!
					m_toDir = (float) atan2(pMob->m_wz - m_wz, pMob->m_wx - m_wx);

					if( m_nMotionState == CHA_JUMP_ATTACK
							|| m_nMotionState == CHA_ROTATE_ATTACK )
						pMob->Attacked();
					else if( m_MagicNum != SKILL_VANPIRE_TOUCH )
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, m_MagicNum); // 여기 m_MagicNum 이 -1일때 들어오는 경우가 있다.(06-04-28 원석)
				}

				m_MagicMobId[i] = -1;
			}// End of For

			m_MagicChaCount = 0;
			m_MagicMobCount = 0;
		}
	}

	// 총 때메... 월드 트랜스폼 미리 계산하자!
	if( bVisible
			|| ( m_nMotionState == CHA_ATTACK
				 && m_Wearing[WEARING_WEAPON]
				 && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN
				 && g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame <= m_CurFrame
				 && m_CurFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame + 3 ) )
	{
		// 월드 트랜스폼. - 캐릭터를 그리기 위한.
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
		D3DMATRIX matHideRotate;

		if (m_HideStatus == HIDE_HIDING)
		{
			if (dwCurtime < m_HideTime + HIDE_HIDING_TIME)   // 1초가 안지났으면...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime) / HIDE_HIDING_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
			}
			else if (dwCurtime < m_HideTime + HIDE_HIDING_TIME + HIDE_DELAY_TIME)     // 2초가 안지났으면...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime - HIDE_HIDING_TIME) / HIDE_DELAY_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
				m_wy -= hide_deep[m_Sex] * (dwCurtime - m_HideTime - HIDE_HIDING_TIME) / HIDE_DELAY_TIME;
			}
			else     // 2초가 지났으면...
			{
				m_HideStatus = HIDE_HIDDEN;
			}
		}
		else if (m_HideStatus == HIDE_RISING)
		{
			if (dwCurtime < m_HideTime + HIDE_RISING_TIME)   // 0.3초가 안지났으면...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime) / HIDE_RISING_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
				m_wy -= hide_deep[m_Sex] - (hide_deep[m_Sex] * (dwCurtime - m_HideTime) / HIDE_RISING_TIME);
			}
			else if (dwCurtime < m_HideTime + HIDE_RISING_TIME + HIDE_DELAY_TIME)     // 1.3초가 안지났으면...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime - HIDE_RISING_TIME) / HIDE_DELAY_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
			}
			else     // 1.3초가 지났으면...
			{
				m_HideStatus = HIDE_SHOWN;
			}
		}

		if (m_HideStatus == HIDE_HIDDEN)
			m_wy -= hide_deep[m_Sex];

		D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
		D3DMath_MatrixMultiply( m_matWorld, matRotate, matTrans );

		D3DMATRIX matTemp;
		D3DUtil_SetScaleMatrix(matTemp, m_fScaleSize, m_fScaleSize, m_fScaleSize);
		D3DMath_MatrixMultiply(m_matWorld, matTemp, m_matWorld);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
	}

	// 다음 프레임 계산
	FrameMove(frame_count, bVisible, pMob, pCha);

	m_bVisible = bVisible; // 프러스텀 컬링상에서 보이는가의 플래그. 06-01-03 원석 추가(개인상점 말칸 때문에 필요했음.)

	if (m_LastChatTime + SHOW_CHATPOPUP_TIME > dwCurtime && bVisible)
	{
		m_bShowChatPopup = TRUE;
	}
	else
	{
		m_bShowChatPopup = FALSE;
	}

	if (!bVisible)
		return TRUE;

	// 그림자 렌더링...
	if (m_pGoodShadow)
	{
		if (m_pGoodShadow->Start(&matRotate, m_nCurHour))
		{
			for (int i = 0; i < CAPSYONG_NUM; i++)
			{
				if (m_bShow[i] && m_CapSyong[i])
				{
					if ((i == NKCHA_HAIR && (m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR)) ||
							(i == NKCHA_FACE && (m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE)) ||
							(i == NKCHA_BREAST && (m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY)) ||
							(i == NKCHA_ARMS && (m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS)) ||
							(i == NKCHA_LEGS && (m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS)) ||
							(i == NKCHA_FEET && (m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET)) ||
							(i == NKCHA_FEET && ((m_Wearing[WEARING_SHOES]  && GET_RENDER_FLAG(m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET)
												 || (m_Wearing[WEARING_PANTS] && (m_Wearing[WEARING_PANTS]->m_Vnum == 384 || m_Wearing[WEARING_PANTS]->m_Vnum == 303)))) )
					{
						// 그리지 말아라!!
					}
					else
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, TRUE, bLow);
				}
			}

			for ( int i = 0; i < WEARING_SHOW_NUM; i++)
			{
				if (m_Wearing[i] )// .m_ProtoNum >= 0)
				{
					if (GET_CSY(m_Wearing[i], m_BpdRace))
					{
						if (i == WEARING_SHIELD)	// 방패일 때는 전투모드에서만 그린다.
						{
							if( g_dwClientCountry == CTRY_JPN )
							{
								if ( ( m_bCombat
										&& m_nMotionState != CHA_SOCIAL
										&& m_nMotionState != CHA_SWIM
										&& m_nMotionState != CHA_SWIM_STAND)

										//  [6/23/2009 ppmmjj83] 일본은 평화모드일때 방패부분도 그려돌라고 요청함
										|| ( g_dwClientCountry == CTRY_JPN
											 && !( m_Wearing[WEARING_SHIELD]->m_Vnum == 773 || m_Wearing[WEARING_SHIELD]->m_Vnum == 1023 ) )
								   )
									GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), -1, TRUE, bLow);
							}
							else
							{
								if ( m_nMotionState != CHA_SOCIAL &&
										m_nMotionState != CHA_SWIM &&
										m_nMotionState != CHA_SWIM_STAND )
								{
									GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), -1, TRUE, bLow);
								}
							}
						}
						else if ( i == WEARING_SHOES
								  && m_Wearing[WEARING_PANTS]
								  && ( GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET ) )
						{
							//(m_Wearing[WEARING_PANTS]->m_Vnum == 303 || m_Wearing[WEARING_PANTS]->m_Vnum == 311 || m_Wearing[WEARING_PANTS]->m_Vnum == 384 || m_Wearing[WEARING_PANTS]->m_Vnum == 434
							//|| m_Wearing[WEARING_PANTS]->m_Vnum == 493 || m_Wearing[WEARING_PANTS]->m_Vnum == 520))
							// 한복바지,산타바지를 입었을 때는 신발을 그리지 말아랑~~~

						}
						else
							GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), -1, TRUE, bLow);
					}
				}
				else if (m_Race == RACE_AIDIA && i == WEARING_CLOAK && g_BasicWingsProto >= 0)
				{
					if( GET_CSYC(g_BasicWingsProto, m_BpdRace) )
						GET_CSYC(g_BasicWingsProto, m_BpdRace)->Render(GET_D3DDEVICE(), -1, TRUE, bLow);
				}
			}
			m_pGoodShadow->End(TRUE);
		}
	}

	// 상태값 보존
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &s_dwAlphaTest );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &s_dwAlphaBlend );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &s_dwSrcBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &s_dwDestBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &m_dwAmbient);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &s_dwLighting );

	// CCapsyong들 출력
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// [3/13/2008 반재승] 무기에 알파 블렌딩 옵션 적용되게 랜더 스테이트 수정 해줌(일본 낫 아이템)
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAREF, 0x0000001);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );


	// 수정 소유자인가 확인을 한다.
	if( pCMyApp->m_nCurWorldIdx != WORLD_MATRIX
			&& m_bCrystalOwner )
	{
		D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy + 30, m_wz);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
		D3DMath_MatrixMultiply( matTrans, matRotate, matTrans );	// matTrans를 temp로 씀.

		if( CommonConfig::Instance()->GetSiegeCrystalScale() == true )
		{
			D3DMATRIX matTemp;
			D3DUtil_SetScaleMatrix(matTemp, 2.0f, 2.0f, 2.0f);		// 공성 크리스탈 크기 조절 요청
			D3DMath_MatrixMultiply(matTrans, matTemp, matTrans);
		}		

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFFFF);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

		//=================================================
		// nate 2004 - 4 : CastleWar
		if( pCMyApp->m_nCurWorldIdx == WORLD_GUILD && g_pWhangLolo1 )
			g_pWhangLolo1->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
		else if( pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN && g_pWhangLolo2 )
			g_pWhangLolo2->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
		else if( pCMyApp->m_nCurWorldIdx == WORLD_START && g_pWhangLolo4 )
			g_pWhangLolo4->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
		else if( pCMyApp->m_nCurWorldIdx == WORLD_FIELD && g_pWhangLolo3 )
			g_pWhangLolo3->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
		//=================================================

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwAmbient);
	}
	// nate 2005-05-06 : 메트릭스 머리위 지휘자 마크
	else if( ( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
			   ||  pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
			 )
			 && m_GuildGradeIndex == 10
			 && m_GuildIndex && m_bShowMatrixMark)
	{
		D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy + 25, m_wz);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, timeGetTime() * 0.0005f );
		D3DMath_MatrixMultiply( matTrans, matRotate, matTrans );	// matTrans를 temp로 씀.
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFFFF);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

		switch(pCMyApp->m_nCurWorldIdx)
		{
		case WORLD_MATRIX:
			switch(m_GuildIndex)
			{
			case 1:
				if( g_pWhangLolo1 ) g_pWhangLolo1->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
				break;
			case 2:
				if( g_pWhangLolo2 ) g_pWhangLolo2->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
				break;
			case 3:
				if( g_pWhangLolo3 ) g_pWhangLolo3->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
				break;
			}
			break;
		case WORLD_MATRIX_2009:
			switch(m_GuildIndex)
			{
			case 1:
				if( g_pWhangLolo2 ) g_pWhangLolo2->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
				break;
			case 2:
				if( g_pWhangLolo3 ) g_pWhangLolo3->Render( GET_D3DDEVICE(), 0, FALSE, FALSE );
				break;
			}
			break;
		}

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwAmbient);
	}

	///2003  캐릭이 상태에 걸려있을때 효과를 줌.(냉각,마비등...)누적 제외.
	if(m_CancelState==1)	//상태가 걸려있다면
	{
		if( m_ShowConditionEffect == FALSE )	// 효과가 그려지고 있는지...검사...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_CONDITION);
			m_ShowConditionEffect = TRUE;
		}
	}

	if(m_CancelState2==1)	//상태가 걸려있다면
	{
		if( m_ShowConditionEffect2 == FALSE )	// 효과가 그려지고 있는지...검사...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_CONDITION2);
			m_ShowConditionEffect2 = TRUE;
		}
	}

	///2003 ,스킬이 걸려있을때 효과를 줌(옵션스킬)
	if(m_InChaSkill)//옵션스킬이 걸려있다면
	{
		if( m_ShowSkillEffect == FALSE )
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_OPTIONSKILL);

			if(m_InChaSkill != SKILL_BLESSING)
				pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SKILL_SPELL);

			m_ShowSkillEffect = TRUE;
		}
	}

	if(m_InChaAidiaSkill)	//에이디아 옵션스킬이 걸려있다면
	{
		if ( m_InChaAidiaSkill == SKILL_JOINT_LIFE ||
				m_InChaAidiaSkill == SKILL_LIFE_LIGHT ||
				m_InChaAidiaSkill == SKILL_MELODY_OF_DEF ||
				m_InChaAidiaSkill == SKILL_MELODY_OF_GUD )
		{
			if( m_ShowAidiaSkillEffect == FALSE )	// 효과가 그려지고 있는지...검사...
			{
				if(m_InChaAidiaSkill==SKILL_JOINT_LIFE)
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SKILL_JOINT_LIFE);
				else if(m_InChaAidiaSkill==SKILL_LIFE_LIGHT)
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SKILL_LIFE_LIGHT);
				else if(m_InChaAidiaSkill==SKILL_MELODY_OF_DEF || m_InChaAidiaSkill==SKILL_MELODY_OF_GUD)
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SKILL_MELODY_OF_DEF);

				m_ShowAidiaSkillEffect = TRUE;
			}
		}
		else
		{
			m_InChaAidiaSkill = 0;
		}
	}


	if( m_Race == RACE_PEROM )
	{
//		if(  )
		{
			int buff_skill = 0;
			if( IsActiveField() )
			{
				buff_skill = EFFECT_PEROM_OPTIONSKILL_S134;
			}
			if( IsActiveEngage() )
			{
				buff_skill = EFFECT_PEROM_OPTIONSKILL_S149;
			}
			if( IsActiveCritical() )
			{
				buff_skill = EFFECT_PEROM_OPTIONSKILL_S148;
			}
			if( IsActiveField_Ex() )
			{
				buff_skill = EFFECT_PEROM_OPTIONSKILL_S150;
			}
			if( m_activate_buff_skill != buff_skill )
			{
				m_activate_buff_skill = buff_skill;
			}
			if( m_activate_buff_skill == 0 && m_ShowSkillEffect == TRUE )
			{
				m_ShowSkillEffect = FALSE;
			}
			if( m_activate_buff_skill != 0 && m_ShowSkillEffect == FALSE )
			{
				pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_OPTIONSKILL);
				m_activate_buff_skill = buff_skill;
				m_ShowSkillEffect = TRUE;
			}
		}
	}


	if(m_bMoonEventEffect)	//0213 대보름 이벤트 효과.
	{
		if(m_ShowMoonEventEffect == FALSE)
		{
			//요기에 효과 넣기
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));
			ef_sort.nType = FX_POWERUP;
			ef_sort.num = 0;
			ef_sort.pNkChaFrom = this;
			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

			m_ShowMoonEventEffect = TRUE;
		}
	}
	else
		m_ShowMoonEventEffect = FALSE;

	///2003  책마법을 쓸때나 당했을때 효과를 줌.
	if(m_UseBook!=0)
	{
		switch (m_UseBook)
		{
		case EFFECT_BLACKHOLE :
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_BLACKHOLE);
			break;
		case EFFECT_BOOK 	 :
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_BOOK);
			break;
		case EFFECT_USE_BOOK  :
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_USE_BOOK );
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_USE_BOOK_SPELL);
			break;
		case EFFECT_REMOVE 	 :
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_USE_BOOK );
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SKILL_SPELL);
			break;
		case EFFECT_SCANVIEW :
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SCANVIEW );
			break;
		case EFFECT_LIFE_LIGHT:
			pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SKILL_SPELL);
			break;
		}
		m_UseBook=0;
	}

	if ( pCMyApp->m_pUIMgr && g_pShadowRect && IsShown()
			&& !(m_nMotionState==CHA_SOCIAL&&(m_ActNum>=153&&m_ActNum<=158)) )
	{
		if(pCMyApp->GetCurWorld() != 3 && m_nTeamMemGrade == TEAM_BATTLE_GUILDPK )// )
		{
			ZeroMemory(g_pRoh->m_sEnemyGName,sizeof(g_pRoh->m_sEnemyGName));
			m_nTeamMemGrade = 0;
		}
		if( m_pGoodShadow
				&& this == g_pRoh
				&& this->m_nTeamMemGrade != TEAM_BATTLE_AFLAG
				&& this->m_nTeamMemGrade == TEAM_BATTLE_NONE
				&& !this->m_bGuildPK
				&& ( !pCMyApp->m_pUIMgr->m_bOnBattle && pCMyApp->GetCurWorld() == 7 ) )
		{
			if (m_pGoodShadow->GetSurface())
			{
				g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), m_nCurHour, TRUE);
				g_pShadowRect->Render(m_pGoodShadow->GetSurface(), TRUE);
			}
		}
		else
		{
			if( pCMyApp->m_pUIMgr->m_bOnBattle && pCMyApp->GetCurWorld() == 7 )
			{
				if( m_bCrystalGuild )
				{
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_PARTY_TEX ] ), FALSE);
				}
				else if( m_bOppGuild )
				{
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_OPPGUILD_TEX ] ), FALSE);
				}
				else
				{
					BOOL bHasFlag = FALSE;
					for( int num = 0; num < 4; num++ )
					{
						if ( m_bFlagGuild[num] )
						{
							g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
							g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ num + EFF_DIR_FLAG1_TEX ] ), FALSE); // 43 : EFF_DIR_FLAG1_TEX
							bHasFlag = TRUE;
							break;
						}
						else if( m_bFlagOwner[num] )
						{
							g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
							g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ num + EFF_DIR_FLAG1_TEX ] ), FALSE); // 43 : EFF_DIR_FLAG1_TEX
							bHasFlag = TRUE;
							break;
						}
					}

					if( !bHasFlag )
					{
						if (m_bMyGuild)
						{
							g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
							g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_GUILDT_TEX ] ), FALSE);
						}
					}
				}
			}
			else
			{
				if (m_nTeamMemGrade == TEAM_BATTLE_AMEMBER || m_nTeamMemGrade == TEAM_BATTLE_AHEAD)
				{
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//=============================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_A_TEAM_TEX ] ), FALSE);
					//=============================================
				}
				else if (m_nTeamMemGrade == TEAM_BATTLE_BMEMBER || m_nTeamMemGrade == TEAM_BATTLE_BHEAD)
				{
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//=============================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_B_TEAM_TEX ] ), FALSE);
					//=============================================
				}
				else if (m_nTeamMemGrade == TEAM_BATTLE_AFLAG || m_nTeamMemGrade == TEAM_BATTLE_BFLAG)
				{
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//=============================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FLAG_OWN_TEX ] ), FALSE);
					//=============================================
				}
				else if(m_nTeamMemGrade == TEAM_BATTLE_GUILDPK )
				{
					if( g_pRoh->m_bGuildPK ) //나자신이 길드중일때
					{
						if(m_pstrGuildName)
						{
							if(strcmp(g_pRoh->m_sEnemyGName,m_pstrGuildName)==0)// 자신의 적길드 와 길드이름이 같을때
							{
								//적 빨강
								// nate 2004 - 7 글로벌 ShadowRect사용
								g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
								//=============================================
								// nate 2004 - 7 : Image Manager
								// nate 2004 - 7 글로벌 ShadowRect사용
								g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_A_GUILD_TEX ] ), FALSE);
								//=============================================
							}
							else if(strcmp(g_pRoh->m_pstrGuildName,m_pstrGuildName)==0) //자신의 길드이름과 같을때
							{
								// 같은편 파란색
								// nate 2004 - 7 글로벌 ShadowRect사용
								g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
								//=============================================
								// nate 2004 - 7 : Image Manager
								// nate 2004 - 7 글로벌 ShadowRect사용
								g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_B_GUILD_TEX ] ), FALSE);
								//=============================================
							}
							else
							{
								// 길드전 중일때 다른 길드가 길드전중이면 노란색
								// nate 2004 - 7 글로벌 ShadowRect사용
								g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
								//=============================================
								// nate 2004 - 7 : Image Manager
								// nate 2004 - 7 글로벌 ShadowRect사용
								g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_C_GUILD_TEX ] ), FALSE);
								//=============================================
							}
						}
					}
					else	//자신이 길드전중이 아닐때 => 구경꾼이면 길드전인 캐릭은  노란색
					{
						// nate 2004 - 7 글로벌 ShadowRect사용
						g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
						//========================================
						// nate 2004 - 7 : Image Manager
						// nate 2004 - 7 글로벌 ShadowRect사용
						g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_C_GUILD_TEX ] ), FALSE);
						//========================================
					}
				}
				else if ( m_bMyParty)	// 나의 파티원이면 그림자 모양을 다르게 표현해준다.
				{
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//========================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_PARTY_TEX ] ), FALSE);
					//========================================
				}
				else if ( pCMyApp->m_nCurWorldIdx != WORLD_MATRIX && m_bMyGuild)
				{
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//========================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_GUILDT_TEX ] ), FALSE);
					//========================================
				}
				else if (m_bOppGuild)
				{
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//========================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_OPPGUILD_TEX ] ), FALSE);
					//========================================
				}
				else
				{
					// nate 2004 - 7 글로벌 ShadowRect사용
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 15, FALSE);
					g_pShadowRect->Render(NULL, FALSE);
				}
			}
		}
//======================================================================
		// nate 2004 - 6 : 데카르디 캐릭터 팀 구분 - 바닦 그리기
		// 주캑릭의 plane 자원으로 모든 캐릭의 바닦을 그린다.
		if( pCMyApp->m_pUIMgr->m_pNamPlane
				&& m_pstrGuildName
				&& GetMotionState() != CHA_SWIM_STAND
				&& GetMotionState() != CHA_SWIM )
		{
			int nGuildNameLen = strlen( m_pstrGuildName );

			// 감마
			if( pCMyApp->m_pUIMgr )
			{
				if( pCMyApp->m_pUIMgr->m_szDecardiGuildGamma != NULL
						|| pCMyApp->m_pUIMgr->m_szDecardiGuildDelta != NULL )
				{
					if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_szDecardiGuildGamma ) )
						byGuildIndex = 1;
					// 델타
					else if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_szDecardiGuildDelta ) )
						byGuildIndex = 2;
					else
						byGuildIndex = 0;
				}
			}

			if( byGuildIndex != 0 )
			{
				D3DMATRIX matWorld;
				GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

				if( byGuildIndex == 1 )
				{
					// nate 2004 - 7 : Image Manager
					GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_BLUE ] ) );
					pCMyApp->m_pUIMgr->m_pNamPlane->SetColor( 255, 255, 255, 255 );
					pCMyApp->m_pUIMgr->m_pNamPlane->SetRotationX( g_PI * 0.5f );
					pCMyApp->m_pUIMgr->m_pNamPlane->RotateZ( -( timeGetTime() / 500.0f ) );
					pCMyApp->m_pUIMgr->m_pNamPlane->Scale( 0.6f, 0.6f, 0.6f );
					pCMyApp->m_pUIMgr->m_pNamPlane->SetTranslationXYZ( m_wx, m_wy + 0.5f, m_wz );
					pCMyApp->m_pUIMgr->m_pNamPlane->Render();
				}
				else if( byGuildIndex == 2 )
				{
					// nate 2004 - 7 : Image Manager
					GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_RED ] ) );
					pCMyApp->m_pUIMgr->m_pNamPlane->SetColor( 255, 255, 255, 255 );
					pCMyApp->m_pUIMgr->m_pNamPlane->SetRotationX( g_PI * 0.5f );
					pCMyApp->m_pUIMgr->m_pNamPlane->RotateZ( -( timeGetTime() / 500.0f ) );
					pCMyApp->m_pUIMgr->m_pNamPlane->Scale( 0.6f, 0.6f, 0.6f );
					pCMyApp->m_pUIMgr->m_pNamPlane->SetTranslationXYZ( m_wx, m_wy + 0.5f, m_wz );
					pCMyApp->m_pUIMgr->m_pNamPlane->Render();
				}
				GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, s_dwLighting );
			}
		}
		// nate 2005-03-21 : Matrix - 바닦 마크 생성
		if( m_GuildIndex
				&& ( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
					 || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
				   )
				&& pCMyApp->m_pUIMgr->m_pMatrixMark
				&& GetMotionState() != CHA_SWIM_STAND
				&& GetMotionState() != CHA_SWIM
				&& m_bShowMatrixMark == TRUE
		  )
		{
			D3DMATRIX matWorld;
			GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffaaaabb );

			switch(pCMyApp->m_nCurWorldIdx )
			{
			case WORLD_MATRIX:
				switch( m_GuildIndex )
				{
				case 1:
					GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_H_MARK ] ) );
					break;
				case 2:
					GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_C_MARK ] ) );
					break;
				case 3:
					GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_S_MARK ] ) );
					break;
				}
				break;
			case WORLD_MATRIX_2009:
				switch( m_GuildIndex )
				{
				case 1:
					GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_C_MARK ] ) );
					break;
				case 2:
					GET_D3DDEVICE()->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_S_MARK ] ) );
					break;
				}
				break;
			}

			pCMyApp->m_pUIMgr->m_pMatrixMark->SetRotationX( g_PI );
			pCMyApp->m_pUIMgr->m_pMatrixMark->SetTranslationXYZ( m_wx, m_wy + 1.0f, m_wz );
			pCMyApp->m_pUIMgr->m_pMatrixMark->RotateY( -( timeGetTime() / 10000.0f ) );
			pCMyApp->m_pUIMgr->m_pMatrixMark->Render();

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, m_dwAmbient);
		}
//======================================================================
	}

	LPDIRECTDRAWSURFACE7 traceTexture = NULL, lightswordTexture = NULL;

	if (this == g_pRoh)
	{
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_SHIELD;
		ef_sort.pNkChaTo = g_pRoh;
		ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx, pCMyApp->GetLandPtr()->GetHFHeight(g_pRoh->m_wx, g_pRoh->m_wz), g_pRoh->m_wz);

		if (m_nManaShield > 0)
		{
			if( ! isShieldLoad )
			{
				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				isShieldLoad = TRUE;
			}

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, MANASHIELD_AMBIENT);
		}
		else if (m_nElectricShield > 0)
		{
			if( ! isShieldLoad )
			{
				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				isShieldLoad = TRUE;
			}

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, ELECTRICSHIELD_AMBIENT);
		}
		else if (m_nSafetyShield > 0)
		{
			if( ! isShieldLoad )
			{
				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				isShieldLoad = TRUE;
			}

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, ELECTRICSHIELD_AMBIENT);
		}
		else
		{
			isShieldLoad = FALSE;
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, MOUSE_OFF_AMBIENT);
		}
	}
	else if (m_bOwnAmbient)
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwOwnAmbient);
	else if (bMouseOn)
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, MOUSE_ON_AMBIENT);
	else
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, MOUSE_OFF_AMBIENT);


// 	if( IsActiveDSystem() )
// 	{
// 		EffectSort ef_sort;
// 		ZeroMemory(&ef_sort, sizeof(EffectSort));
//
// 		ef_sort.nType = FX_PEROM_SKILL_S139;
// 		ef_sort.pNkChaTo = this;
// 		ef_sort.vPos = D3DVECTOR(m_wx, pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz), m_wz);
//
// 		InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
// 	}

	// nate 2005-11-14 : 포이즌 - 포이즌상태 SetAmbient
	if( m_bPoisoning )
	{
		if( bMouseOn )
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x0044ff44 );
		else
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x00008833 );
	}
	// nate 2004 - 6 : 데카르디 캐릭터 팀 캐릭터색상 조정
	else if( byGuildIndex == 1)
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x008888ff );
	else if( byGuildIndex == 2 )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x00ff9999 );
	else if( g_pRoh &&
			 g_pRoh->GetMyGuildData() &&
			 (g_pRoh->GetMyGuildData()->IsInvasion_Defense() || g_pRoh->GetMyGuildData()->IsInvasion_Attack() )  &&
			 g_pRoh->GetMyGuildData()->CheckInvasionGuild( this->m_GuildIndex))
	{
		// [6/20/2007 Theodoric] 침략전 :: 침략과 관계된 길드원들은 카오처럼 표시한다.
		DWORD chao_ambient;
		chao_ambient = ( (150 << 24) | (255 << 16) | (100 << 8) | 100 );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, chao_ambient);
	}
	/*	else if( pCMyApp->GetCurWorld() == WORLD_GUILD_WAR &&
				 this->m_GuildIndex != g_pRoh->m_GuildIndex )
		{
			DWORD chao_ambient;
			chao_ambient = ( (150 << 24) | (255 << 16) | (100 << 8) | 100 );
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, chao_ambient);
		}
	*/
	else if( m_Chaotic_Grade )	// 일반인이 아닐때만 들어와~~~
	{
		DWORD chao_ambient;

		if( m_Chaotic_Grade == 1 )
			chao_ambient = ( (150 << 24) | (255<< 16) | (150<< 8) | 60 );
		else if( m_Chaotic_Grade == 2 )
			chao_ambient = ( (150 << 24) | (255 << 16) | (0 << 8) | 0 );
		else if( m_Chaotic_Grade == 10 || m_Chaotic_Grade == 11 )
			chao_ambient = ( (150 << 24) | (150 << 16) | (15 << 8) | 150 );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, chao_ambient);
	}

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	if( dcbMgr->IsStart() == true )
	{
		int team = dcbMgr->GetTeam(m_nCharIndex);
		if( team == 0 )
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xffff0000);
		}
		else if( team == 1 )
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xff0000ff);
		}
	}

	if( m_Race == RACE_FREAK )
	{
		if( m_bCloaking
				|| ((( timeGetTime() / 400 ) % 2)
					&& m_bCloaking_Waring ) )
		{
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		}
	}

	if( m_bchaDontAttackEffect )
	{
		if( (timeGetTime()/400) % 2 )
		{
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		}
	}

	// 자유대련 중임을 표시하기 위한 반투명 효과 ///자유대련
	if( g_pRoh ) // 로그인 전엔 없는 경우에 들어올수도 잇으므로 체크!
	{
		if( g_pRoh->m_bNowFreePVP && timeGetTime()-g_pRoh->m_dwRingCreateTick < 5000 && this->m_nCharIndex == g_pRoh->m_dwPvpDestIndex ) // 대기 카운트 중의 상대방은 깜빡이게.
		{
			if( (timeGetTime()/400) % 2 )
			{
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
			}
		}
		if( g_pRoh->m_bNowFreePVP && this != g_pRoh && this->m_nCharIndex != g_pRoh->m_dwPvpDestIndex ) // 자기가 대련중이고, 지금 그리는 녀석이 제 3자이면..
		{
			// 자신이 자유대련 중이면 자기랑 상대 이외엔 모두 반투명.
			// 반투명.
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		}
		if( !g_pRoh->m_bNowFreePVP && this->m_bNowFreePVP ) // 자신은 자유대련중이 아닌데 이녀석이 자유대련 중인녀석이면...
		{
			// 자신이 자유대련중이 아니면 자유대련인 녀석들 모두 반투명.
			// 반투명.
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		}
	}

	//----------------------------------------------------------------------
	// 캐릭이 장착하고 있는 장비를 그린다.
	//----------------------------------------------------------------------
	memset(m_bEventWearRendered, 0, sizeof(m_bEventWearRendered));
	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	{
		if (m_Wearing[i])
		{
			// 갑옷, 바지
			if ((i == WEARING_ARMOR || i == WEARING_PANTS ) && (GET_RENDER_FLAG(m_Wearing[i]) & ARMOR_COUSE_SKIN))
			{
				if (i == WEARING_ARMOR)
				{
					if (m_CapSyong[NKCHA_BREAST])
						m_CapSyong[NKCHA_BREAST]->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
					if (m_CapSyong[NKCHA_ARMS])
						m_CapSyong[NKCHA_ARMS]->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
				}
				else
				{
					if (m_CapSyong[NKCHA_LEGS])
						m_CapSyong[NKCHA_LEGS]->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
				}
			}
			// 기타
			else if (GET_CSY(m_Wearing[i], m_BpdRace))
			{
				//----------------------------------------------------------------
				// 방패일 경우에
				if (i == WEARING_SHIELD)
				{
					if( m_nMotionState != CHA_SOCIAL &&
						m_nMotionState != CHA_SWIM &&
						m_nMotionState != CHA_SWIM_STAND )
					{
						// 광선검 및 마법검 완드
						if( m_bCombat && (m_Wearing[i]->m_Vnum==773 || m_Wearing[i]->m_Vnum==1023) )//
						{
							LPDIRECTDRAWSURFACE7 lpTexture;
							if (m_nMotionState == CHA_ATTACK
								&& GET_SHAPE(m_Wearing[WEARING_SHIELD]) != IWEAPON_STAFF
								&& GET_SHAPE(m_Wearing[WEARING_SHIELD]) != IWEAPON_GUN)
								lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_NKCHARACTER_LINE]);
							else
							{
								if( GET_ATT_FLAG(m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON )
									lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_NKCHARACTER_LINE2]);
								else
									lpTexture = NULL;
							}
							// 광선검 효과
							GET_CSY(m_Wearing[WEARING_SHIELD], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow, lpTexture, m_CurFrame);
						}
						else
						{
							// 일반 방패
							if( !EventWearingRender(i) ) // 이벤트 복장이 있다면 이벤트 복장을 출력한다. ///이벤트복장착용
							{
								if( IsAlphaWear(i, false) == FALSE )
								{
									GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
								}

								if( m_Wearing[i]->m_Vnum == 1588 ) ///일본디자인컨테스트 헤일로의 방패라면..
								{
									// 이펙트를 여기서 예외 처리 해준다.
									DrawInstanceEffect(  1588, this, GET_CSY(m_Wearing[i], m_BpdRace) );
								}
								else if( m_Wearing[i]->m_Vnum == 1589 ) ///일본디자인컨테스트 등롱
								{
									// 이펙트를 여기서 예외 처리 해준다.
									DrawInstanceEffect(  1589, this, GET_CSY(m_Wearing[i], m_BpdRace) );
								}
							}
						}
					}					
				}
				else if (i == WEARING_SHOES && m_Wearing[WEARING_PANTS] &&
						 (GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET))
				{	
					if( g_bUseEventWear )
					{
						if( m_EventWearing[WEARING_PANTS] && m_bEventWearFlag ) // 이벤트 바지를 착용하고 있고...
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // 만약 이벤트 바지는 슈즈를 그리는 종류라면.
							{	
								if( !EventWearingRender(i) )
								{
									if( IsAlphaWear(i, false) == FALSE )
									{
										GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
									}
								}
							}
						}

					}
				}
				//--------------------------------------------------------------
				// 슬레이어
				else if (i == WEARING_CLOAK
						 && ( ( m_nMotionState == CHA_THROW_SLAYER
								&& m_CurFrame >= THROW_SLAYER_FRAME_START
								&& m_CurFrame <= THROW_SLAYER_FRAME_END )							  
							  || m_nMotionState == CHA_B_BURNINGCRASH ) )
				{
					// SubCsy 를 Render 한다(등에 찬 것 대신  손에 듣 것을 Render )
					if (GET_SUB_CSY(m_Wearing[i]))
						GET_SUB_CSY(m_Wearing[i])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
				}
				//--------------------------------------------------------------
				// 디바인 스톤 - Lyul_2005
				else if (i == WEARING_CLOAK && m_Wearing[WEARING_CLOAK] && GET_SHAPE(m_Wearing[WEARING_CLOAK]) == ISUB_DSTONE)
				{
					if(m_nMotionState == CHA_F_DSTONE)
					{
						// 손에 든 것을 그림
						if(m_CurFrame >= 9 && m_CurFrame <= DSTRONE_SLAYER_FRAME_END && !m_bstone)
						{
							// SubCsy 를 Frame move 한다(등에 찬 것 대신  손에 듣 것을 Render )
							if (GET_SUB_CSY(m_Wearing[i]))
								GET_SUB_CSY(m_Wearing[i])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
						}
						// 등에 찬 것을 그림
						else if(!m_bstone)
						{
							if( IsAlphaWear(i, false) == FALSE )
							{
								GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
							}
						}
					}
					// 등에 찬 것을 그림
					else if(!m_bstone)
					{
						if( !EventWearingRender(i) ) // 이벤트 복장이 있다면 이벤트 복장을 출력한다. ///이벤트복장착용
						{
							if( IsAlphaWear(i, false) == FALSE )
							{
								GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
							}
						}
					}
				}
				else if( !m_Chaotic_Grade && // 카오면 라이트를 먹여준다. 앞으로도 이런식으로 라이트를 이용해 색을 입히는 종류의 예외는 여기에 넣어줘야한다.
						 !( g_bUseEventWear && m_EventWearing[i] && m_bEventWearFlag ) && // 이벤트 장비를 착용하고있다면 라이트 예외 체크를 안함. ///070723 일본버그리포트 수정
						 ( IS_LIGHT_OFF(m_Wearing[i]) ) // [6/19/2008 반재승] 의상 아이템들의 그림자 끄기 여부 체크
					   ) // 라이트를 끄고 그릴 종류의 장비
				{
					GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

					if( !EventWearingRender(i) ) // 이벤트 복장이 있다면 이벤트 복장을 출력한다. ///이벤트복장착용
					{
						DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
						if( dcbMgr->IsStart() == true )
						{
							int team = dcbMgr->GetTeam(m_nCharIndex);
							if( team != -1 )
							{
								GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
							}
						}

						if( IsAlphaWear(i, false) == FALSE )
						{
							GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
						}
					}
					GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
				}
				// 일반 Render
				else
				{
					// 이벤트 복장이 있다면 이벤트 복장을 출력한다. ///이벤트복장착용
					if( !EventWearingRender(i) )
					{
						if( IsAlphaWear(i, false) == FALSE )
						{
							GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
						}
					}
				}
			}
		}
		else if( m_EventWearing[i] && m_bEventWearFlag && ( i != WEARING_RING ) ) // 메인 장비를 안차고있어도 이벤트 복장을 착용하고있는 경우를 체크한다. ///이벤트복장착용
		{
			EventWearingRender(i); // 바로 그리면 된다.
		}
		else if (m_Race == RACE_AIDIA && i == WEARING_CLOAK && g_BasicWingsProto >= 0)
		{
			if( GET_CSYC(g_BasicWingsProto, m_BpdRace) )
				GET_CSYC(g_BasicWingsProto, m_BpdRace)->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
		}
	}	

	// 몸뚱이 그리기...
	for (int i = 0; i < CAPSYONG_NUM; i++)
	{
		if (m_bShow[i] && m_CapSyong[i])//ooo
		{
			///이벤트복장착용
			if( g_bUseEventWear )
			{
				// 조금 복잡해서 크게 나눴다.
				switch( i )
				{
				case NKCHA_HAIR:
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

					if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag) // 우선은 이벤트 장비가 있는지 체크.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}						
					}
					else if( m_Wearing[WEARING_HELMET] ) // 이벤트 장비가 없다면 일반 장비를 체크.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}						
					}
					else // 둘다 안착용하고 있다면...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

					break;

				case NKCHA_FACE:

					if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag  ) // 우선은 이벤트 장비가 있는지 체크.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_HELMET] ) // 이벤트 장비가 없다면 일반 장비를 체크.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // 둘다 안착용하고 있다면...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_BREAST:

					if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag ) // 우선은 이벤트 장비가 있는지 체크.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_ARMOR] ) // 이벤트 장비가 없다면 일반 장비를 체크.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // 둘다 안착용하고 있다면...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_ARMS:

					if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag  ) // 우선은 이벤트 장비가 있는지 체크.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_ARMOR] ) // 이벤트 장비가 없다면 일반 장비를 체크.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // 둘다 안착용하고 있다면...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_LEGS:

					if( m_EventWearing[WEARING_PANTS] && m_bEventWearFlag) // 우선은 이벤트 장비가 있는지 체크.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_PANTS] ) // 이벤트 장비가 없다면 일반 장비를 체크.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // 둘다 안착용하고 있다면...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_FEET: // 이녀석은 조건이 좀더 복잡하다.

					if( (m_EventWearing[WEARING_PANTS] || m_EventWearing[WEARING_SHOES]) && m_bEventWearFlag )// 이벤트 복장이 있는경우.
					{
						if( m_EventWearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							break; // 안그리는 조건.
						}

						if( m_EventWearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							break; // 안그리는 조건.
						}

						// 이벤트 복장을 입고 있는데 그리는 조건인 경우다.
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}
					else if( m_Wearing[WEARING_PANTS] || m_Wearing[WEARING_SHOES] ) // 이벤트 복장이 둘다 없는 경우 일반 복장을 체크한다.
					{
						if( m_Wearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							break; // 안그리는 조건.
						}

						if( m_Wearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // 그려야하는 아이템을 착용하고 있는 경우.
						{
							break; // 안그리는 조건.
						}

						// 이벤트 복장을 입고 있는데 그리는 조건인 경우다.
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}
					else // 아무것도 안입고있어도 그린다.
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				}
			}
			else
			{
				if ((i == NKCHA_HAIR && (m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR)) ||
						(i == NKCHA_FACE && (m_Wearing[WEARING_HELMET] && GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE)) ||
						(i == NKCHA_BREAST && (m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY)) ||
						(i == NKCHA_ARMS && (m_Wearing[WEARING_ARMOR] && GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS)) ||
						(i == NKCHA_LEGS && (m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS)) ||
						(i == NKCHA_FEET && (m_Wearing[WEARING_PANTS] && GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET)) ||
						(i == NKCHA_FEET && ((m_Wearing[WEARING_SHOES]  && GET_RENDER_FLAG(m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET)
											 || (m_Wearing[WEARING_PANTS] && (m_Wearing[WEARING_PANTS]->m_Vnum == 384 || m_Wearing[WEARING_PANTS]->m_Vnum == 303)))) )
				{
					// 그리지 말아라!!
				}
				else
				{
					m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);

				}
			}
		}
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	for (int i = WEARING_SHOW_NUM; i >= 0; i--)
	{
		if( m_EventWearing[i] && m_bEventWearFlag && ( i != WEARING_RING ) )
		{
			if( IsAlphaWear(i, true) )
			{
				RenderAlphaWear(m_EventWearing[i], bLow);
			}
		}
		else if( m_Wearing[i] )
		{
			if( IsAlphaWear(i, false) )
			{
				RenderAlphaWear(m_Wearing[i], bLow);
			}
		}
	}
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

	if (m_nMotionState == CHA_THROW_CHAKRAM )
	{
		// 567 == 차크람::item->m_ProtoNum
		if(this !=g_pRoh
				&& m_CurFrame >= THROW_CHAKRAM_FRAME_START
				&& m_CurFrame <= THROW_CHAKRAM_FRAME_END)
		{
			int proto_num = g_ItemList.FindItem(693);

			if (GET_SUB_CSYC(proto_num) && (proto_num >= 0))
				GET_SUB_CSYC(proto_num)->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
		}
		else if( pCMyApp->m_pUIMgr && pCMyApp->m_pUIMgr->m_ChakramSlotNum >= 0
				 && m_CurFrame >= THROW_CHAKRAM_FRAME_START && m_CurFrame <= THROW_CHAKRAM_FRAME_END)
		{
			// 차크람 공격이닷! 등에 찬것 그리지 말고 손에 든걸로 그려랏!
			// 2005.05.11
			// Lyul / 수정 완료
			if (m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]) // NULL Check
				if (GET_SUB_CSY(m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]))
					GET_SUB_CSY(m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
		}
	}



	ItemOutlineMgr* outlineMgr = ItemOutlineMgr::GetInstance();
	float fWidth = outlineMgr->GetWidth(eOutlineType_DSystem);
	DWORD dwColor = outlineMgr->GetColor(eOutlineType_DSystem, 0);
	if( m_Race == RACE_PEROM && IsActiveDSystem() )
	{
		for( int e=0 ; e<WEARING_SHOW_NUM ; e++ )
		{
			if( m_EventWearing[e] && GET_CSY(m_EventWearing[e], m_BpdRace) && (m_bEventWearRendered[e] == true) )
			{
				GET_CSY(m_EventWearing[e], m_BpdRace)->RenderOutline(GET_D3DDEVICE(), g_nEmptyTextureNum, fWidth, dwColor);
			}
			else if( m_Wearing[e] && GET_CSY(m_Wearing[e], m_BpdRace) && (m_bEventWearRendered[e] == false) )
			{
				GET_CSY(m_Wearing[e], m_BpdRace)->RenderOutline(GET_D3DDEVICE(), g_nEmptyTextureNum, fWidth, dwColor);
			}						
		}

		int w[CAPSYONG_NUM] = {0, 1, 1, 2, 4, 0};
		bool d[CAPSYONG_NUM] = {true, true, true, true, true, true};
		for(int i = 0; i < CAPSYONG_NUM; i++)
		{
			if( m_Wearing[w[i]] && (m_bEventWearRendered[w[i]] == false) )
			{
				d[i] = false;				

				continue;
			}
			else if( m_EventWearing[w[i]] && (m_bEventWearRendered[w[i]] == true) )
			{
				d[i] = false;				

				continue;
			}			
		}

		for(int i = 0; i < CAPSYONG_NUM; i++)
		{
			if( m_Wearing[i] && (m_bEventWearRendered[i] == false) )
			{
				if( (GET_RENDER_FLAG(m_Wearing[i]) & ARMOR_REMOVE_HAIR) )
				{
					d[NKCHA_HAIR] = false;
				}
				if( (GET_RENDER_FLAG(m_Wearing[i]) & ARMOR_REMOVE_FACE) )
				{
					d[NKCHA_FACE] = false;
				}
				if( (GET_RENDER_FLAG(m_Wearing[i]) & ARMOR_REMOVE_BODY) )
				{
					d[NKCHA_BREAST] = false;
				}
				if( (GET_RENDER_FLAG(m_Wearing[i]) & ARMOR_REMOVE_ARMS) )
				{
					d[NKCHA_ARMS] = false;
				}
				if( (GET_RENDER_FLAG(m_Wearing[i]) & ARMOR_REMOVE_LEGS) )
				{
					d[NKCHA_LEGS] = false;
				}
				if( (GET_RENDER_FLAG(m_Wearing[i]) & ARMOR_REMOVE_FEET) )
				{
					d[NKCHA_FEET] = false;
				}
			}
			else if( m_EventWearing[i] && (m_bEventWearRendered[i] == true) )
			{
				if( (GET_RENDER_FLAG(m_EventWearing[i]) & ARMOR_REMOVE_HAIR) )
				{
					d[NKCHA_HAIR] = false;
				}
				if( (GET_RENDER_FLAG(m_EventWearing[i]) & ARMOR_REMOVE_FACE) )
				{
					d[NKCHA_FACE] = false;
				}
				if( (GET_RENDER_FLAG(m_EventWearing[i]) & ARMOR_REMOVE_BODY) )
				{
					d[NKCHA_BREAST] = false;
				}
				if( (GET_RENDER_FLAG(m_EventWearing[i]) & ARMOR_REMOVE_ARMS) )
				{
					d[NKCHA_ARMS] = false;
				}
				if( (GET_RENDER_FLAG(m_EventWearing[i]) & ARMOR_REMOVE_LEGS) )
				{
					d[NKCHA_LEGS] = false;
				}
				if( (GET_RENDER_FLAG(m_EventWearing[i]) & ARMOR_REMOVE_FEET) )
				{
					d[NKCHA_FEET] = false;
				}
			}			
		}

		for(int i = 0; i < CAPSYONG_NUM; i++)
		{
			if( d[i] == false )
				continue;

			m_CapSyong[i]->RenderOutline(GET_D3DDEVICE(), g_nEmptyTextureNum, fWidth, dwColor);
		}
	}



		/*if(m_Vital >= m_MaxVital) m_MaxVital = m_Vital;
		pCMyApp->DisplayPlayerVital(this, (this->m_Vital/this->m_MaxVital)*100);*/

	//----------------------------------------------------------------------
	// WEARING_WEAPON Render
	//----------------------------------------------------------------------
	if (m_bCombat
		&& m_bEventWearFlag
		&& m_nMotionState != CHA_SOCIAL
		&& m_nMotionState != CHA_SWIM
		&& m_nMotionState != CHA_SWIM_STAND
		&& m_nMotionState != CHA_THROW_SLAYER
		&& m_nMotionState != CHA_THROW_CHAKRAM
		&& m_nMotionState != CHA_F_DSTONE		// Lyul_2005 디바인스톤일 경우에도 숨긴다.
		&& m_nMotionState != CHA_B_BURNINGCRASH	// nate 2005-06-18 : Burning Crash - 버닝 크래쉬 제한 걸기
		&& m_nMotionState != CHA_H_AUTOMINE
		&& m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : 일렉트릭 샷
		&& m_EventWearing[WEARING_WEAPON]
		&& GET_CSY( m_EventWearing[WEARING_WEAPON], m_BpdRace ) )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = NULL;
		if( GET_CSY(m_EventWearing[WEARING_WEAPON], m_BpdRace) && m_nMotionState != CHA_B_BURNINGCRASH )
			GET_CSY(m_EventWearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_EventWearing[WEARING_WEAPON],m_BpdRace), FALSE, bLow, lpTexture, m_CurFrame);

		// 일반 이도류 Render
		if( GET_SHAPE(m_EventWearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && GET_SUB_CSY(m_EventWearing[WEARING_WEAPON]) )
			GET_SUB_CSY(m_EventWearing[WEARING_WEAPON])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
	}
	else if (m_bCombat
			&& m_nMotionState != CHA_SOCIAL
			&& m_nMotionState != CHA_SWIM
			&& m_nMotionState != CHA_SWIM_STAND
			&& m_nMotionState != CHA_THROW_SLAYER
			&& m_nMotionState != CHA_THROW_CHAKRAM
			&& m_nMotionState != CHA_F_DSTONE		// Lyul_2005 디바인스톤일 경우에도 숨긴다.
			&& m_nMotionState != CHA_B_BURNINGCRASH	// nate 2005-06-18 : Burning Crash - 버닝 크래쉬 제한 걸기
			&& m_nMotionState != CHA_H_AUTOMINE
			&& m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : 일렉트릭 샷
			&& m_Wearing[WEARING_WEAPON]
			&& GET_CSY( m_Wearing[WEARING_WEAPON], m_BpdRace ) )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = NULL;
		//--------------------------------------------------------------------
		// 이도류 블레드(Bleed) Render
		if(GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && m_bBleed)
		{
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_NKCHARACTER_LINE3]);
			GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, lpTexture, m_CurFrame,1);

			if( GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) )
				GET_SUB_CSY(m_Wearing[WEARING_WEAPON])->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, lpTexture, m_CurFrame,1);
		}
		// nate 2005-06-15 : Burning Crash - 슬레이어에 불 붙이기
		else if( m_Wearing[ WEARING_CLOAK ]
				 && m_nMotionState == CHA_B_BURNINGCRASH
				 && (( m_CurFrame >= 72 && m_CurFrame <= 84 )
					 || m_CurFrame >= 112 ) )
		{
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );
			GET_SUB_CSY( m_Wearing[ WEARING_CLOAK ] )->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, lpTexture, m_CurFrame, 1 );
		}
		// 일반 무기 Render
		else
		{
			// nate 2005-06-10 : Burning Crash - 버닝크래쉬에서는 일반무기는 안 그램
			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) && m_nMotionState != CHA_B_BURNINGCRASH )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[WEARING_WEAPON],m_BpdRace), FALSE, bLow, lpTexture, m_CurFrame);

			// 일반 이도류 Render
			if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) )
				GET_SUB_CSY(m_Wearing[WEARING_WEAPON])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
		}

		// outline
		if( m_Wearing[WEARING_WEAPON]->m_MinLevel_Org >= 320 )
		{
			fWidth = outlineMgr->GetWidth(WEARING_WEAPON);
			dwColor = outlineMgr->GetColor(WEARING_WEAPON, m_Wearing[WEARING_WEAPON]->m_PlusNum);

			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) && (m_bEventWearRendered[WEARING_WEAPON] == false) )
			{
				if( m_nMotionState != CHA_B_BURNINGCRASH )
				{
					GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->RenderOutline(GET_D3DDEVICE(), g_nEmptyTextureNum, fWidth, dwColor);
				}

				if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) )
				{
					GET_SUB_CSY(m_Wearing[WEARING_WEAPON])->RenderOutline(GET_D3DDEVICE(), g_nEmptyTextureNum, fWidth, dwColor);
				}
			}
		}

		if ( m_nMotionState == CHA_ATTACK)
		{
			// 칼 궤적 을 지정한다. Lyul_2005 이도류 추가
			if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD
					|| GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE
					|| GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER )
				m_DrawWeaponEffects = TRACELINE_ATTACK;
			// 블레드
			else if(m_bBleed)// && 블레드
				m_DrawWeaponEffects = TRACELINE_ATTACK_BLEED;
			// 이도류 일반.
			else if(GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD )
				m_DrawWeaponEffects = TRACELINE_ATTACK;
			else
				m_DrawWeaponEffects = NONE;
		}
		else if ( m_nMotionState == CHA_JUMP_ATTACK )
			m_DrawWeaponEffects = TRACELINE_JUMPATTACK;
		else if ( m_nMotionState == CHA_ROTATE_ATTACK )
			m_DrawWeaponEffects = TRACELINE_ROTATEATTACK;
		else if( m_nMotionState == CHA_F_DASHSLASH ) ///하이콤보
			m_DrawWeaponEffects = TRACELINE_DASHSLASH;
		else if( m_nMotionState == CHA_F_SLASHSHADOW ) ///하이콤보
			m_DrawWeaponEffects = TRACELINE_SLASHSHADOW;
		else
			m_DrawWeaponEffects = NONE;

		// nate 2004 - 8
		// 신무기 관련 - 뮤턴트 색상입히기

		// 870 weapon effect
		if(	m_Wearing[ WEARING_WEAPON ]->m_Vnum > 10561
				&& m_Wearing[ WEARING_WEAPON ]->m_Vnum < 10570
				&& m_nMotionState != CHA_B_BURNINGCRASH
				&& GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace))
		{
			if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 9 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 7 ]/*skyblue*/, FALSE, FALSE, NULL, m_CurFrame);
			else if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 6 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 6 ]/*yellow*/, FALSE, FALSE, NULL, m_CurFrame);
			else if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 4 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 8 ]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
		}

		// 890 weapon effect
		else if(	m_Wearing[ WEARING_WEAPON ]->m_Vnum > 10569
				&& m_Wearing[ WEARING_WEAPON ]->m_Vnum < 10578
				&& m_nMotionState != CHA_SPIRITMODE
				&& GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace))
		{
			if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 9 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 7 ]/*skyblue*/, FALSE, FALSE, NULL, m_CurFrame);
			else if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 6 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 6 ]/*yellow*/, FALSE, FALSE, NULL, m_CurFrame);
			else if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 4 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 8 ]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
		}

		// 910 weapon effect
		else if(	m_Wearing[ WEARING_WEAPON ]->m_Vnum > 10722
				&& m_Wearing[ WEARING_WEAPON ]->m_Vnum < 10731
				&& m_nMotionState != CHA_SPIRITMODE
				&& GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace))
		{
			if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 9 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 7 ]/*skyblue*/, FALSE, FALSE, NULL, m_CurFrame);
			else if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 6 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 6 ]/*yellow*/, FALSE, FALSE, NULL, m_CurFrame);
			else if( m_Wearing[ WEARING_WEAPON ]->m_PlusNum > 4 )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[ 8 ]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
		}

		
		/*
		for( int e=0 ; e<WEARING_SHOW_NUM ; e++ )
		{
			fWidth = outlineMgr->GetWidth(e);
			dwColor = outlineMgr->GetColor(e, m_Wearing[e]->m_PlusNum);
			if (GET_CSY(m_Wearing[e], m_BpdRace) &&
				m_Wearing[e]->m_Vnum > 1805 &&
				m_Wearing[e]->m_Vnum < 1808)
			{
				GET_CSY(m_Wearing[e], m_BpdRace)->RenderOutline(GET_D3DDEVICE(), g_nEmptyTextureNum, fWidth, dwColor);
				
			}
		}*/
	}//End Combat and no social

	//  [5/6/2009 ppmmjj83] 일본 아이템중 비전투시 칼을 칼집에 넣은 모양으로 처리해 줘야하는 부분이 있음
	else if( !m_bCombat
			 && m_Wearing[WEARING_WEAPON]
			 && (m_Wearing[WEARING_WEAPON]->m_Vnum == 1590 || m_Wearing[WEARING_WEAPON]->m_Vnum == 2694)
		   )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );;

		int ProtoNum	= g_ItemList.FindItem( 1712 ); // 무라마사 보조 캡숑용.
		int	TextrNum	= g_ItemList.m_ItemArray[ProtoNum].m_TextrNum[m_BpdRace];

		if( GET_CSYC(ProtoNum, m_BpdRace) )
			GET_CSYC(ProtoNum, m_BpdRace)->Render(GET_D3DDEVICE(), TextrNum, FALSE, bLow, lpTexture, m_CurFrame);
	}

	//  [5/6/2009 ppmmjj83] 일본 아이템중 비전투시 칼을 칼집에 넣은 모양으로 처리해 줘야하는 부분이 있음
	else if( !m_bCombat && m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2168 )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );;

		int ProtoNum	= g_ItemList.FindItem( 2317 ); // 무라마사 보조 캡숑용.
		int	TextrNum	= g_ItemList.m_ItemArray[ProtoNum].m_TextrNum[m_BpdRace];

		if( GET_CSYC(ProtoNum, m_BpdRace) )
			GET_CSYC(ProtoNum, m_BpdRace)->Render(GET_D3DDEVICE(), TextrNum, FALSE, bLow, lpTexture, m_CurFrame);
	}

	//  [5/6/2009 ppmmjj83] 일본 아이템중 비전투시 칼을 칼집에 넣은 모양으로 처리해 줘야하는 부분이 있음
	else if( !m_bCombat && m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2339 )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );;

		int ProtoNum	= g_ItemList.FindItem( 2340 ); // 무라마사 보조 캡숑용.
		int	TextrNum	= g_ItemList.m_ItemArray[ProtoNum].m_TextrNum[m_BpdRace];

		if( GET_CSYC(ProtoNum, m_BpdRace) )
			GET_CSYC(ProtoNum, m_BpdRace)->Render(GET_D3DDEVICE(), TextrNum, FALSE, bLow, lpTexture, m_CurFrame);
	}

	else if( m_nMotionState == CHA_H_ELECTRICSHOT )		// nate 2005-11-24 : 일렉트릭 샷 - 무기 바꾸기
	{
		if( m_CurFrame > 9 && m_CurFrame < 56 )
		{
			CItem* ElectricShot_Gun = new CItem;
			ZeroMemory( ElectricShot_Gun, sizeof( CItem ) );
			ElectricShot_Gun->m_ProtoNum = g_ItemList.FindItem( 1339 );	// 야마토 건
			if( GET_CSY(ElectricShot_Gun, m_BpdRace) )
				GET_CSY(ElectricShot_Gun, m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(ElectricShot_Gun,m_BpdRace), FALSE, bLow, NULL, m_CurFrame);
		}
	}
	// nate 2005-06-10 : Burning Crash - 궤적
	else if( m_nMotionState == CHA_B_BURNINGCRASH
			 && m_Wearing[ WEARING_CLOAK ] )
	{
		int nSkillLevel = ( this == g_pRoh )?g_ChaSkill.m_SkillLevel[ 16 ]:m_nSkillLevel;
		if( ( ( nSkillLevel - 1 ) / 5 ) >= 3 )
			m_DrawWeaponEffects = TRACELINE_ATTACK_BLEED;
	}
	else
		m_DrawWeaponEffects = NONE;


	//We'll do it here out of the combat state and social state

	if (!m_bEventWearFlag)//Not showing Event wear so render the effect dumbass
	{	
		for (int i = 0; i < WEARING_SHOW_NUM; i++)
		{
			if (m_Wearing[i] &&
				m_Wearing[i]->m_Vnum > 1816 &&
				m_Wearing[i]->m_Vnum < 1821 &&
				GET_CSY(m_Wearing[i], m_BpdRace))
			{
				int textNum = 0;
				textNum = m_Wearing[i]->m_PlusNum >= 15 ? 2 :
					m_Wearing[i]->m_PlusNum >= 10 ? 0:
					1;
				GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
			}
			
		}
	}
	else
	{
		for (int i = 0; i < WEARING_SHOW_NUM; i++)
		{
			if (m_EventWearing[i] &&
				m_EventWearing[i]->m_Vnum > 1816 &&
				m_EventWearing[i]->m_Vnum < 1821 &&
				GET_CSY(m_EventWearing[i], m_BpdRace))
			{
				int textNum = 0;
				textNum = m_EventWearing[i]->m_PlusNum >= 15 ? 2 :
					m_EventWearing[i]->m_PlusNum >= 10 ? 0:
					1;
				GET_CSY(m_EventWearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
			}
		}
	}
	
	//if (!m_bEventWearFlag)//Not showing Event wear so render the effect dumbass
	//{	
	//	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	//	{
	//		if (m_Wearing[i] &&
	//			m_Wearing[i]->m_Vnum > 10165 &&
	//			m_Wearing[i]->m_Vnum < 10190 &&
	//			GET_CSY(m_Wearing[i], m_BpdRace))
	//		{
	//			int textNum = 0;
	//			textNum = m_Wearing[i]->m_PlusNum >= 15 ? 1 :
	//				m_Wearing[i]->m_PlusNum >= 10 ? 0:
	//				m_Wearing[i]->m_PlusNum >= 5 ? 2:
	//				-1;
	//			GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
	//		}
	//		
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	//	{
	//		if (m_EventWearing[i] &&
	//			m_EventWearing[i]->m_Vnum > 10165 &&
	//			m_EventWearing[i]->m_Vnum < 10190 &&
	//			GET_CSY(m_EventWearing[i], m_BpdRace))
	//		{
	//			int textNum = 0;
	//			textNum = m_EventWearing[i]->m_PlusNum >= 15 ? 1 :
	//				m_EventWearing[i]->m_PlusNum >= 10 ? 0:
	//				m_EventWearing[i]->m_PlusNum >= 5 ? 2:
	//				-1;
	//			GET_CSY(m_EventWearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
	//		}
	//	}
	//}


	//	if (!m_bEventWearFlag)//Not showing Event wear so render the effect dumbass
	//{	
	//	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	//	{
	//		if (m_Wearing[i] &&
	//			m_Wearing[i]->m_Vnum > 10864 &&
	//			m_Wearing[i]->m_Vnum < 10889 &&
	//			GET_CSY(m_Wearing[i], m_BpdRace))
	//		{
	//			int textNum = 0;
	//			textNum = m_Wearing[i]->m_PlusNum >= 15 ? 4 :
	//				m_Wearing[i]->m_PlusNum >= 10 ? 3:
	//				m_Wearing[i]->m_PlusNum >= 5 ? 5:
	//				-1;
	//			GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
	//		}
	//		
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	//	{
	//		if (m_EventWearing[i] &&
	//			m_EventWearing[i]->m_Vnum > 10864 &&
	//			m_EventWearing[i]->m_Vnum < 10889 &&
	//			GET_CSY(m_EventWearing[i], m_BpdRace))
	//		{
	//			int textNum = 0;
	//			textNum = m_EventWearing[i]->m_PlusNum >= 15 ? 4 :
	//				m_EventWearing[i]->m_PlusNum >= 10 ? 3:
	//				m_EventWearing[i]->m_PlusNum >= 5 ? 5:
	//				-1;
	//			GET_CSY(m_EventWearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
	//		}
	//	}
	//}


	//	if (!m_bEventWearFlag)//Not showing Event wear so render the effect dumbass
	//{	
	//	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	//	{
	//		if (m_Wearing[i] &&
	//			m_Wearing[i]->m_Vnum > 10744 &&
	//			m_Wearing[i]->m_Vnum < 10769 &&
	//			GET_CSY(m_Wearing[i], m_BpdRace))
	//		{
	//			int textNum = 0;
	//			textNum = m_Wearing[i]->m_PlusNum >= 15 ? 7 :
	//				m_Wearing[i]->m_PlusNum >= 10 ? 6:
	//				m_Wearing[i]->m_PlusNum >= 5 ? 8:
	//				-1;
	//			GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
	//		}
	//		
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	//	{
	//		if (m_EventWearing[i] &&
	//			m_EventWearing[i]->m_Vnum > 10744 &&
	//			m_EventWearing[i]->m_Vnum < 10769 &&
	//			GET_CSY(m_EventWearing[i], m_BpdRace))
	//		{
	//			int textNum = 0;
	//			textNum = m_EventWearing[i]->m_PlusNum >= 15 ? 7 :
	//				m_EventWearing[i]->m_PlusNum >= 10 ? 6:
	//				m_EventWearing[i]->m_PlusNum >= 5 ? 8:
	//				-1;
	//			GET_CSY(m_EventWearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), g_nMutantTextureNum[textNum]/*red*/, FALSE, FALSE, NULL, m_CurFrame);
	//		}
	//	}
	//}

	////////

	////////
	if( ( m_nTeamMemGrade == TEAM_BATTLE_AFLAG
			|| m_nTeamMemGrade == TEAM_BATTLE_BFLAG ) )
	{
		if( m_bCatchFlagShow == FALSE )
		{
			m_bCatchFlagShow = TRUE;

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));
			ef_sort.nType = FX_CATCHFLAG;
			ef_sort.pNkChaTo = this;
			ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
	}
	else if( pCMyApp->m_pUIMgr
			 && pCMyApp->m_pUIMgr->m_bOnBattle
			 && ( m_bFlagOwner[0]
				  || m_bFlagOwner[1]
				  || m_bFlagOwner[2]
				  || m_bFlagOwner[3] ) )
	{
		if( m_bCatchFlagShow == FALSE )
		{
			m_bCatchFlagShow = TRUE;

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));
			ef_sort.nType = FX_CATCHFLAGDIR;
			ef_sort.pNkChaTo = this;
			ef_sort.vPos = D3DVECTOR(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);

			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
	}

	// 주변광 값 되돌리기
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwAmbient);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, s_dwAlphaTest );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, s_dwAlphaBlend );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, s_dwSrcBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, s_dwDestBlend);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, s_dwLighting );

	// pick buffer 쓰기.
	if (m_HideStatus == HIDE_SHOWN
			&& lpd3dDevPick )
	{
		if( IsDrawPicking() ) // ///자유대련 시 조건 체크 함수추가
		{
			if( IsDead() ) // 죽었으면 피킹박스 회전시켜줌.
			{
				D3DMATRIX		matRotate;
				D3DXQUATERNION TempQuat;
				D3DMath_QuaternionFromAngles( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
											  , -1.57f, m_dir+1.57f, 0.0f );
				D3DMath_MatrixFromQuaternion( matRotate, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w );

				D3DMath_MatrixMultiply( m_matWorld, matRotate, m_matWorld );
				D3DMATRIX		matScale;
				D3DUtil_SetScaleMatrix(matScale, 3.0f, 1.0f, 3.0f);
				D3DMath_MatrixMultiply( m_matWorld, matScale, m_matWorld );
			}
			// 픽버퍼를 렌더링할때에는 다른 디바이스를 사용하기 때문에
			// 주변광을 제외한 다른 라이트는 영향을 미치지 않는다.
			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			lpd3dDevPick->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
			lpd3dDevPick->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick);
			//		lpd3dDevPick->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///피킹버그수정. DWORD 형으로 변환한걸쓰지말고 그냥 WORD형을 바로 색상으로 넣는다. 이렇게 해도 32767개 까지 동시 피킹이 가능할 터. 변환중 뭔가 꼬이는 경우가 있는듯해서 이렇게 변경함. (07-01-02 원석)

			m_CubeLolo.Render(lpd3dDevPick, 0, TRUE);
			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		}
	}


	// 발바닥 돌 그리기.
	if ( !IsDead()
			&& pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz) ) )
	{
		if (m_FlyLevel > 0)
		{
			GET_LOLOC(g_AirshipProto[m_FlyLevel-1])->Render(GET_D3DDEVICE());
			// 이건 별
			D3DMATRIX matWordI;
			D3DUtil_SetIdentityMatrix(matWordI);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWordI );

			if (!m_pDropStar)
				m_pDropStar = new CDropStar;

			if( m_pDropStar )
				m_pDropStar->Render(m_wx, m_wy, m_wz);
		}
	}
	//mungmae-2005/06/23 뫼비우스 리젠바를 그린다.
	if(m_bDrawRegenbar && m_pRegenbar )
		m_pRegenbar->Render( m_dwRegenTime, m_nRegenIndex);

	if( m_pRegenbar && this == g_pRoh && m_nMotionState==CHA_SOCIAL && (m_ActNum>=153&&m_ActNum<158) ) // 꼭지점 댄스 스페이스 표시 체크.(자기자신만)(마지막 껀 아니다.)
	{
		if( m_CurFrame >= m_Biped->m_AnimCount - 20 ) // 끝나기 전 프레임에 눌르면 오케이
			m_pRegenbar->RenderAnother( POINT_DANCE_SPACE );
	}


	// 개인상점을 열고 있는 중의 캐릭터라면... ///개인상점
	if( m_UserShopLevel && m_UserShopLevel < 4 )
	{
		// 상품진열대를 그려주자.
		if( g_pDisplayStandLolo[m_UserShopLevel-1] && GET_D3DDEVICE() )
			g_pDisplayStandLolo[m_UserShopLevel-1]->Render(GET_D3DDEVICE());
	}

	if( m_bResurrectionSkill )
	{
		char strTemp[128] = "";
		CNkCharacter* pChar = pCMyApp->m_pUIMgr->FindChaById( m_nResurrection_ChaId, FALSE ); // 리저렉션 중 상대가 부활해버리는 경우를 체크하기 위해 추가. ///070723 일본버그리포트 수정
		if( !pChar ) // 만약 없어졌으면 리저렉션을 취소시켜 준다. (여기서 체크하는게 비효율적이긴하지만 그냥 직관적으로 여러상황생각해서 여기서 계속 체크시킴.)
		{
			// 취소 패킷도 날려준다.			
			SendCharUseSkill_TargetValue2(SKILL_A_RESURRECTION, eCharType_Char, m_nResurrection_ChaId, 1, -1);

			m_dwResurrectionTime = 0;
			m_bResurrectionSkill = FALSE;
		}
		else
		{
			m_dwResurrectionTime = timeGetTime() - m_dwResurrection_StartTime;

			int nResurrectionIndex = 0;
			if( g_ChaSkill.m_SkillLevel[22] <= 5 )
				nResurrectionIndex = 25;
			else if( g_ChaSkill.m_SkillLevel[22] <= 10 )
				nResurrectionIndex = 20;
			else if( g_ChaSkill.m_SkillLevel[22] <= 15 )
				nResurrectionIndex = 15;
			else if( g_ChaSkill.m_SkillLevel[22] <= 20 )
				nResurrectionIndex = 10;
			else if( g_ChaSkill.m_SkillLevel[22] <= 25 )
				nResurrectionIndex = 5;

			if( m_dwResurrectionTime > nResurrectionIndex * 1000 )
			{	
				SendCharUseSkill_TargetValue2(SKILL_A_RESURRECTION, eCharType_Char, m_nResurrection_ChaId, 1, 10);

				m_dwResurrectionTime = 0;
				m_bResurrectionSkill = FALSE;
			}
			m_pRegenbar->Render( m_dwResurrectionTime, nResurrectionIndex);
		}
	}

	// 자유대련 중이라면 링 이펙트를 그려준다. ///자유대련
	if( this == g_pRoh && m_bNowFreePVP && m_dwRingCreateTick ) // 자기 자신이 자유대련 중일때만 링을 그림
	{
		// 프레임 계산.
		DWORD NowFrame = ( (timeGetTime()-m_dwRingCreateTick)/20 ) % 60; // 20프레임 1초 기준으로 40프레임짜리로 계산.

		EffectSort ef_sort; // 형식을 맞추기 위한 녀석
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.vPos = m_vPvpRingPos;
		ef_sort.nCurFrame = NowFrame;
		ef_sort.subNum = 60; // 토탈 프레임으로 사용.
		ef_sort.dwColor = m_dwRingCreateTick; // 생성된 시간.
		pCMyApp->m_pFxSet->m_pFxFreePvpRing->Render( ef_sort ); // Insert가 아니라 바로 그려줌에 유의.

		// 도망체크. 링의 중심으로 부터 일정 거리 멀어지면 도망으로 친다.
		float fDist = DistPlToPl(m_wx, m_wz, m_vPvpRingPos.x, m_vPvpRingPos.z);

		if( fDist > 150.0f ) // 사이즈는 임시.
		{
			g_pTcpIp->SendNetMessage( "pvp -4\n" ); // 나 도망쳤다고 서버에 알림.
//			m_bNowFreePVP = FALSE; // 한번만 보내기 위해서 상태를 해제.(이거 경우에 따라서 서버에서 패킷이 늦게 오면 이상하게 보이는 상황이 나올수 있으므로 유의.)
			m_dwRingCreateTick = 0; // 한번만 보내기 위해서 초기화.
		}
	}

	if(m_pSiren && g_SvrType == ST_12_OVER ) // && !pCMyApp->IsPeaceArea(NULL, this->m_tx, this->m_tz)
	{
		EffectSort eff;
		m_pSiren->Render(eff, this);

	}

	return TRUE;
}

// nate
void CNkCharacter::DrawChatPopup(BOOL bOnlyName)
// 인자 기본 리셋 false
// 캐릭터 채팅창 오픈
// 길드마크, PK마크
// 대만 훈장
{
	if (m_bShowChatPopup)
	{
		if (bOnlyName && m_pChatPopup && m_pChatPopup->GetIndex() < 1)
			return;

		D3DMATRIX matPipe;
		D3DVECTOR vScr, vWorld;

		vWorld = D3DVECTOR(m_wx, m_wy+23.0f, m_wz);

		// 편의상 CLand의 기능을 쓴다.
		if( pCMyApp->m_pUIMgr->m_pLand )
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
		// 공간상의 점을 화면상의 점으로 바꾼다.
		D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

		//vScr.y -= 18;	// 박스의 대략적인 크기.
		if (vScr.y < 0)
			vScr.y = 0;

		// 팝업창이 화면보가 높으면 크기 조절
		if (vScr.y >= g_pNk2DFrame->GetClientHeight())
			vScr.y = g_pNk2DFrame->GetClientHeight() - 20;

		if (vScr.x < 30)
			vScr.x = 30;
		if (vScr.x >= g_pNk2DFrame->GetClientWidth() - 20)
			vScr.x = g_pNk2DFrame->GetClientWidth() - 20;

		// 대만 훈장 관련
		// 좌표 계산과 박스만 그림
		if( g_dwClientCountry == CTRY_TWN )
			m_pChatPopup->Draw( vScr.x, vScr.y, 2 );
		else
			m_pChatPopup->Draw( vScr.x, vScr.y );

		int AdjustPosY = m_pChatPopup->GetYpos() - BOX_GAP;

		if ( AdjustPosY < 0 )
			AdjustPosY = 0;

		if( g_bGlobalMatrix ) ///국가전승리 마크 작업시 추가
			AdjustPosY += 17; // 아래 녀석들은 왜 이거 안해주는가? 문제 있는듯.(국가대항전 국가 마크도 이거 해줘야함) // 국가전 마크 길이 개행.


		if( g_pNvNWin && m_NvNWin ) ///국가전승리 횟수가 있으면...
		{
			AdjustPosY += 1; // 마크가 너무 붙어있으므로 약간 띄어주고 시작.
			int x_start = 0;
			int x_move = 0;

			x_start = m_NvNWin * 7; // 총 표시되야할 사이즈의 절반

			for( int i = 0 ; i < m_NvNWin ; ++i ) // 승리 횟수 만큼 출력
			{
				g_pDisplay->Blt( vScr.x - x_start + x_move, AdjustPosY, g_pNvNWin ); // 현재로선 가장 위.
				x_move += 14;
			}
			AdjustPosY += 13; // 아래 녀석들은 왜 이거 안해주는가? 문제 있는듯.(국가대항전 국가 마크도 이거 해줘야함)
		}
			
		if (m_nHaveGuildMark == true)
		{
			pCMyApp->m_pUIMgr->guildmark_render(m_GuildIndex,
												m_pChatPopup->GetXpos() + m_pChatPopup->GetGuildNameXPos(),
												m_pChatPopup->GetYpos() + m_pChatPopup->GetGuildNameYPos());
		}

		// nate
		// 대만 훈장 관련
		if( m_Level >= 270 )
			g_pDisplay->Blt( vScr.x - 13, AdjustPosY, g_pMedal );

		// 대만 훈장 관련
		// 텍스트만 출력
		if( g_dwClientCountry == CTRY_TWN )
			m_pChatPopup->Draw( vScr.x, vScr.y, 1 );

		// 길드전 :: g_pGuildpk
		// 길드가 있는 경우
		if (m_pstrGuildName)
		{
			// 어떤성주인지 확인하는 변수
			int master_guild = -1;

			//=========================================
			// nate 2004 - 4
			// CastleWar
			// 어떤 성주마크를 보여줄것인지 미리 비교
			// 현재 캐릭이 화이트혼에 엤는가?
			if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN)
			{
				// 길드 이름 비교
				if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild1 ) )
				{
					// 내가 화이트에 있고
					if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
					else if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
				}
				else if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild2 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
					else if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
				}
				else if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild3 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
					else if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
				}
			}
			// 현재 캐릭이 샤일론에 엤는가?
			else if( pCMyApp->m_nCurWorldIdx == WORLD_GUILD )
			{
				if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild1 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
					else if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
				}
				else if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild2 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
					else if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
				}
				else if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild3 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
					else if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
				}
			}
			// 현재 캐릭이 데카르디에 엤는가?
			else// if( pCMyApp->m_nCurWorldIdx == WORLD_FIELD )
			{
				if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild1 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone1 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
				}
				else if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild2 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone2 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
				}
				else if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild3 ) )
				{
					if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_FIELD )
						master_guild = WORLD_FIELD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_GUILD )
						master_guild = WORLD_GUILD;
					else if( pCMyApp->m_pUIMgr->m_MasterZone3 == WORLD_WHITEHORN )
						master_guild = WORLD_WHITEHORN;
				}
			}
			//=========================================

			if( master_guild < 0 )
			{
				if (!strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[0])
						|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[1])
						|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[2])
						|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[3]) )
					master_guild = 100;
			}

			if( master_guild >= 0 )
			{
				//===========================================
				// nate 2004 - 4 : CastleWar
				CSurface *pImgKing;

				if( master_guild == WORLD_WHITEHORN )
					pImgKing = g_pImgKing2;
				else if( master_guild == WORLD_GUILD )
					pImgKing = g_pImgKing1;
				else if( master_guild == WORLD_FIELD )
					pImgKing = g_pImgKing4;
				else
					pImgKing = g_pImgKing3;
				//===========================================

				//성길드이면서 프리피케이 중...
				if( m_Free_Pk == 1 )
				{
					// g_pPK : CSurface 클래스 객체 <= 이미지파일
					g_pDisplay->Blt( vScr.x-30, AdjustPosY, g_pPK);
					// pImgKing : 킹 이미지파일
					g_pDisplay->Blt( vScr.x+7/*-13*/, AdjustPosY, pImgKing);
				}
				//0305  Non피케이 중
				else if( m_bNonPK )
				{
					g_pDisplay->Blt( vScr.x-30, AdjustPosY, g_pNonPK);
					g_pDisplay->Blt( vScr.x+7/*-13*/, AdjustPosY, pImgKing);
				}
				else	//성길드이면서 논피케이중...
					g_pDisplay->Blt( vScr.x-13, AdjustPosY, pImgKing);
			}
			else	////1111	// 성길드 아니면서
			{
				if(m_Free_Pk==1)
				{
					g_pDisplay->Blt( vScr.x-13, AdjustPosY, g_pPK);

					if(m_bPk_render)	//피케이 선언시...
					{
						SetChatMsg(m_pstrName, "");
						m_bPk_render=FALSE;
					}
				}
				else if(m_bNonPK)	//0305 Non 피케이 중...
				{
					g_pDisplay->Blt( vScr.x-13, AdjustPosY, g_pNonPK);

					if(m_bPk_render)	//피케이 선언시...
					{
						SetChatMsg(m_pstrName, "");
						m_bPk_render=FALSE;
					}

				}
				else if(m_bPk_render)	//해제
				{
					SetChatMsg(m_pstrName, "");
					m_bPk_render=FALSE;
				}
			}
		}
		else
		{
			if(m_Free_Pk==1)
			{
				g_pDisplay->Blt( vScr.x-13, AdjustPosY, g_pPK);

				if(m_bPk_render)	//피케이 선언시...
				{
					SetChatMsg(m_pstrName, "");
					m_bPk_render=FALSE;
				}
			}
			else if(m_bNonPK)	//0305 Non 피케이 중...
			{
				g_pDisplay->Blt( vScr.x-13, AdjustPosY, g_pNonPK);
				if(m_bPk_render)	//피케이 선언시...
				{
					SetChatMsg(m_pstrName, "");
					m_bPk_render=FALSE;
				}
			}
			else if(m_bPk_render)	//해제
			{
				SetChatMsg(m_pstrName, "");
				m_bPk_render=FALSE;
			}
		}

		//if( g_bGlobalMatrix ) ///국가대항전이면 이름 앞에 나라 마크 출력
		//{
			int x_move = 0;
			int y_move = 0;
			int y_plus = 0; // 줄 간격

			HDC hdc;
			hdc = GetDC(pCMyApp->Get_hWnd());
			HFONT hFont = (HFONT)SelectObject( hdc, m_pChatPopup->GetFont() );

			x_move = 0;

			// y 계산
			if( pCMyApp->Font_AddPosY == 0 ) // 줄 간격 계산.
				y_plus = 4;
			else
				y_plus = 1;

			SelectObject(hdc, hFont);
			ReleaseDC(pCMyApp->Get_hWnd(), hdc);

			int DrawPosX = m_pChatPopup->GetXpos()+(m_pChatPopup->GetSurWidth()/2)-x_move-10;
			int DrawPosY = m_pChatPopup->GetYpos()+y_move-25;

			if( this == g_pRoh ) // 자기 자신은 이름에 국가명이 안붙어있으므로 별개 처리
			{
			/*	if( g_dwClientCountry == CTRY_KOR ) // 한국
				{
					g_pDisplay->Blt( DrawPosX, DrawPosY, g_pMatrixKor);
				}
				else if( g_dwClientCountry == CTRY_JPN ) // 일본
				{
					g_pDisplay->Blt( DrawPosX, DrawPosY, g_pMatrixJpn);
				}*/
			}
			else
			{

				RECT DrawRect;

				if( DrawPosX < 0 )
					DrawRect.left = -DrawPosX;
				else
					DrawRect.left = 0;

				if( DrawPosY < 0 )
					DrawRect.top =  -DrawPosY;
				else
					DrawRect.top = 0;
				DrawRect.right =  g_pMatrixKor->GetWidth(); // 그래픽 팀에 20*18로 만들어달라고하자.
				DrawRect.bottom =  g_pMatrixKor->GetHeight();

				// 이름 앞 3글자로 국가 구분.
				if( strncmp(m_pstrName,"GS.",3) == 0 ) // KOREA
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixKor, &DrawRect);
				}
				else if( strncmp(m_pstrName,"GS-",3) == 0 ) // JAPAO
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixJpn, &DrawRect );
				}
				else if( strncmp(m_pstrName,"GS_",3) == 0 ) // BRASIL
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixBra, &DrawRect );
				}
				else if( strncmp(m_pstrName,"GS@",3) == 0 ) // ALEMANHA
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixGer, &DrawRect );
			}
				else if( strncmp(m_pstrName,"GS=",3) == 0 ) // CHINA
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixChi, &DrawRect );
				}
				else if( strncmp(m_pstrName,"GS/",3) == 0 ) // INDONESIA
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixInd, &DrawRect );
		}
				else if( strncmp(m_pstrName,"GS*",3) == 0 ) // 일본
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixTha, &DrawRect );
				}
			}
	//	}

	}

	if( m_UserShopLevel && g_pNk2DFrame->GetControlBottom()
			&& ( g_pNk2DFrame->GetControlBottom()->m_bSalesView || m_bShowChatPopup ) //장사채널을 껏어도 마우스가리키고 있는녀석은 표기. ///점포상점 시수정
			&& m_bVisible ) // 개인 상점을 열었고, 장사채널이 켜져있으면 보이게.....(개인상점을 열었어도 채팅은 가능하게 해준다.) m_bVisible 는 컬링 플래그다. 위에 함쳐도 되지만 복잡하게 보일까봐 따로 뺌. ///개인상점
	{
		D3DMATRIX matPipe;
		D3DVECTOR vScr, vWorld;

		vWorld = D3DVECTOR(m_wx, m_wy+15.0f, m_wz);

		// 편의상 CLand의 기능을 쓴다.
		if( pCMyApp->m_pUIMgr->m_pLand )
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
		// 공간상의 점을 화면상의 점으로 바꾼다.
		D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

		//vScr.y -= 18;	// 박스의 대략적인 크기.
		if (vScr.y < 0)
			vScr.y = 0;

		// 팝업창이 화면보가 높으면 크기 조절
		if (vScr.y >= g_pNk2DFrame->GetClientHeight())
			vScr.y = g_pNk2DFrame->GetClientHeight() - 20;

		if (vScr.x < 30)
			vScr.x = 30;

		if (vScr.x >= g_pNk2DFrame->GetClientWidth() - 20)
			vScr.x = g_pNk2DFrame->GetClientWidth() - 20;

		m_pUserShopPopup->Draw( vScr.x, vScr.y, 3 );
	}
}

// nate
void CNkCharacter::SetChatMsg(char *name, char *chat_msg)
// 채팅 메세지 길드원에 따라 다른 색으로 CMultiText에 AddString()
{
	CMultiText multiText;
	char strTemp[100] ="";


	m_nGuildNameLine = 0;
	m_nHaveGuildMark = false;

	if( g_bGlobalMatrix ) ///국가대항전 시에 국가 마크 표시를 위해서...
	{
		multiText.AddString(" ", _FCOLOR_GREEN);
	}

	if( m_NvNWin ) ///국가전승리 횟수가 있다면..
	{
		strcpy( strTemp, "  " );

		for( int i = 1 ; i < m_NvNWin ; ++i ) // 승리 횟수가 졸라 많을 경우를 대비해 공백을 횟수에 비례해서 넣는다.
			strcat( strTemp, "  " );

		multiText.AddString( strTemp, _FCOLOR_GREEN );
	}

	// 길드가 있는지 검사.
	if (m_pstrGuildName)
	{
		if( m_Free_Pk
				|| m_bNonPK
				// nate 2004 - 4 : CastleWar
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild1)
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild2)
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[0])
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[1])
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[2])
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[3])
				|| !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild3 )
				|| !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild3 ) )
		{
			multiText.AddString(" ", _FCOLOR_GREEN);
			m_nGuildNameLine += 1;
		}

		if (pCMyApp->m_pUIMgr->guildmark_exist(m_GuildIndex) == true)
		{
			m_nHaveGuildMark = true;			
		}
		else
		{
			m_nHaveGuildMark = false;			
		}

		sprintf(strTemp, "%s", m_pstrGuildName);

		multiText.AddString(strTemp, _FCOLOR_GREEN/*RGB(127, 255, 127)*/);

		if( m_pstrGuildGradeName != NULL )
		{
			sprintf(strTemp, "%s", m_pstrGuildGradeName);
			// 길드원에 따른 글자색 설정
			if( m_GuildGradeIndex == 10 )			// 길마
				multiText.AddString(strTemp, RGB(255, 50, 50));
			else if( m_GuildGradeIndex == 9 )		// 부길마
				multiText.AddString(strTemp, RGB(128, 50, 128));
			else if( m_GuildGradeIndex <= 7 && m_GuildGradeIndex >= 2)		// 부장
				multiText.AddString(strTemp, RGB(255, 50, 255));
			else if( m_GuildGradeIndex == 1 )		// 길드원
				multiText.AddString(strTemp, RGB(255, 255, 255));
		}
	}
	else if(m_Free_Pk || m_bNonPK)
		multiText.AddString(" ", _FCOLOR_GREEN);

	if( g_bShowIndex )
	{
		char index_str[20] = "";
		sprintf(index_str, "(%d)", m_nCharIndex);
		multiText.AddString(index_str, _FCOLOR_YELLOW);
	}

	// 카오등급
	if( m_Chaotic_Grade == 1 )
		multiText.AddString(name, RGB(255, 150, 64));
	else if( m_Chaotic_Grade == 2 )
		multiText.AddString(name, RGB(255, 60, 60));
	else if( m_Chaotic_Grade == 10 || m_Chaotic_Grade == 11 )
		multiText.AddString(name, RGB(180, 70, 180));
	else
		multiText.AddString(name, _FCOLOR_YELLOW);
	//char playerLevel[20] = "";
	//sprintf(playerLevel, "Lv:%d Fame:%lld", m_Level,(__int64)m_Fame);
	//multiText.AddString(playerLevel, RGB(0, 255, 0));

	char playerLevel[20] = "";
	sprintf(playerLevel, "Lv : %d", m_Level);
	multiText.AddString(playerLevel, RGB(0, 255, 0));

	char FamePoint[20] = "";
	sprintf(FamePoint, "Fame : %lld", (__int64)m_Fame);
	multiText.AddString(FamePoint, RGB(255, 255, 0));

	// 문자열이 길어지면 여러줄 출력
	// 문자열 길이를 15에서 35으로 늘려 출력해보자. [2/8/2007 Theodoric]
	multiText.AddStringMulti(chat_msg, 15, TRUE);

	m_pChatPopup->MakeChatPopup(&multiText, TRUE, m_nHaveGuildMark ? m_nGuildNameLine : -1);
	m_pChatPopup->SetIndex(0);		// 말을 했을때의 챗 박스
	m_LastChatTime = timeGetTime();
}

void CNkCharacter::OnChatMsg()
{
	if (g_SvrMode == eSVR_MASTER)
		return;

	CMultiText multiText;
	char strTemp[100] = "";


	m_nGuildNameLine = 0;

	if( g_bGlobalMatrix ) ///국가대항전 시에 국가 마크 표시를 위해서...
	{
		multiText.AddString(" ", _FCOLOR_GREEN);
	}

	if( m_NvNWin ) ///국가전승리 횟수가 있다면..
	{
		strcpy( strTemp, "  " );

		for( int i = 1 ; i < m_NvNWin ; ++i ) // 승리 횟수가 졸라 많을 경우를 대비해 공백을 횟수에 비례해서 넣는다.
			strcat( strTemp, "  " );

		multiText.AddString( strTemp, _FCOLOR_GREEN );
	}

	if (m_pstrGuildName)
	{
		if( m_Free_Pk
				|| m_bNonPK
				// nate 2004 - 4 : CastleWar
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild1)
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild2)
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[0])
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[1])
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[2])
				|| !strcmp(m_pstrGuildName, pCMyApp->m_pUIMgr->m_SubGuild[3]) )
		{
			multiText.AddString(" ", _FCOLOR_GREEN);
			m_nGuildNameLine += 1;
		}

		if (pCMyApp->m_pUIMgr->guildmark_exist(m_GuildIndex) == true)
		{
			m_nHaveGuildMark = true;			
		}
		else
		{
			m_nHaveGuildMark = false;			
		}

		sprintf(strTemp, "%s", m_pstrGuildName);
		multiText.AddString(strTemp, _FCOLOR_GREEN/*RGB(127, 255, 127)*/);

		if( m_pstrGuildGradeName != NULL )
		{
			sprintf(strTemp, "%s", m_pstrGuildGradeName);
			if( m_GuildGradeIndex == 10 )			// 길마
				multiText.AddString(strTemp, RGB(255, 50, 50));
			else if( m_GuildGradeIndex == 9 )		// 부길마
				multiText.AddString(strTemp, RGB(128, 50, 128));
			else if( m_GuildGradeIndex <= 7 && m_GuildGradeIndex >= 2)		// 부장
				multiText.AddString(strTemp, RGB(255, 50, 255));
			else if( m_GuildGradeIndex == 1 )		// 길드원
				multiText.AddString(strTemp, RGB(255, 255, 255));
		}
	}
	else if(m_Free_Pk || m_bNonPK)
		multiText.AddString(" ", _FCOLOR_GREEN);

	if( g_bShowIndex )
	{
		char index_str[20] = "";
		sprintf(index_str, "(%d)", m_nCharIndex);
		multiText.AddString(index_str, _FCOLOR_YELLOW);
	}

	if( m_Chaotic_Grade == 1 )
		multiText.AddString(m_pstrName, RGB(255, 150, 64));
	else if( m_Chaotic_Grade == 2 )
		multiText.AddString(m_pstrName, RGB(255, 60, 60));
	else if( m_Chaotic_Grade == 10 || m_Chaotic_Grade == 11 )
		multiText.AddString(m_pstrName, RGB(180, 70, 180));
	else
		multiText.AddString(m_pstrName, _FCOLOR_YELLOW);
	
	multiText.AddStringMulti("", 15, FALSE);

	//char playerLevel[20] = "";
	//sprintf(playerLevel, "Lv : %d;Fame : %lld", m_Level,(__int64)m_Fame);
	//multiText.AddString(playerLevel, RGB(0, 255, 0));

	char playerLevel[20] = "";
	sprintf(playerLevel, "Lv : %d", m_Level);
	multiText.AddString(playerLevel, RGB(0, 255, 0));

	char FamePoint[20] = "";
	sprintf(FamePoint, "Fame : %lld", (__int64)m_Fame);
	multiText.AddString(FamePoint, RGB(255, 255, 0));

	m_pChatPopup->MakeChatPopup(&multiText, TRUE, m_nHaveGuildMark ? m_nGuildNameLine : -1);

	DWORD dwCurtime = timeGetTime();
	// 채팅말이 사라진 후에는... 이름만 남기자!
	if (m_pChatPopup && (m_LastChatTime + SHOW_CHATPOPUP_TIME > dwCurtime))
	{
		m_pChatPopup->SetIndex(1); // 채팅말이 남아 있는데 채팅 확장 모드에서 이름 확인.
		return;
	}
	else
	{
		if (m_pChatPopup->GetIndex() < 2)
			SetChatMsg(m_pstrName, "");
	}
	// 시간은 채팅창이 떠 있는 시간의 절반으로...
	if (m_LastChatTime + (SHOW_CHATPOPUP_TIME / 2) < dwCurtime)
		m_LastChatTime = dwCurtime - (SHOW_CHATPOPUP_TIME / 2);

	if( m_pChatPopup )
		m_pChatPopup->SetIndex(2); // 마우스를 올려놨을 때의 이름 박스
}

// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
void CNkCharacter::Wear(int where, int index, int vnum, int gem_num, int special, int special2,
						int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///신방어구강화시스템
{
	if (vnum < 0)
		return;

	if( where == 3 && g_pRoh ) ///뱀파이어망토 박쥐 이펙트 처리.(캐릭터 선택화면을 구분하기 위해 주인공 존재 여부를 체크.)
	{
		if( m_Wearing[3] && m_Wearing[3]->m_Vnum == 1774 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 1774) ) // 이벤트 망토를 안 끼고있는 경우만...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
		if( vnum == 1774  ) // 새로 뱀파이어 망토를 끼는 경우.
		{
			if( !m_EventWearing[3] ) // 이벤트 망토를 안 끼고있는 경우만...
				pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.
		}

		if( m_Wearing[3] && m_Wearing[3]->m_Vnum == 11273 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 11273) ) // 이벤트 망토를 안 끼고있는 경우만...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
		if( vnum == 11273  ) // 새로 뱀파이어 망토를 끼는 경우.
		{
			if( !m_EventWearing[3] ) // 이벤트 망토를 안 끼고있는 경우만...
				pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.
		}
	}

	int proto_num = g_ItemList.FindItem(vnum);

	if (proto_num >= 0)
	{
		SAFE_DELETE(m_Wearing[where]);
		m_Wearing[where] = new CItem;
		if( !m_Wearing[where] )
			return;
		m_Wearing[where]->m_Index = index;
		m_Wearing[where]->m_Vnum = vnum;
		m_Wearing[where]->m_ProtoNum = proto_num;
		//2003 착용하고 있는 서브아이템을 플러스아이템으로 인식하지 않게 하기.
		if(GET_TYPE(m_Wearing[where])==ITYPE_SUB)
			m_Wearing[where]->m_PlusNum = gem_num;
		else if (gem_num > 0)
		{
			if(g_ItemList.m_ItemArray[proto_num].m_MinLevel>=181)
				m_Wearing[where]->SetDiaGemNum(gem_num);
			else
				m_Wearing[where]->SetGemNum(gem_num);
		}

		m_Wearing[where]->m_Special = special;

		///신방어구강화시스템
		m_Wearing[where]->m_Special2 = special2; // 추가 옵션
		m_Wearing[where]->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		m_Wearing[where]->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

		m_Wearing[where]->m_Endurance = Endur;
		m_Wearing[where]->m_MaxEndurance = MaxEndur;

		// 아이템 값을 입력해보자!
		ResetItemValue(m_Wearing[where]);
		SetSpecialValue(m_Wearing[where], GET_TYPE(m_Wearing[where]), GET_SHAPE(m_Wearing[where]), special, special2);
	}

	OnBossRaidHeroEffect();
}

void CNkCharacter::EventWear(int where, int index, int vnum, int gem_num, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///이벤트복장착용
{
	if( !g_bUseEventWear ) // 이벤트 복장 착용 시스템을 지원 안하는 나라는 작동안하게...
		return;

	if (vnum < 0)
		return;

	if( where == 3 && g_pRoh ) ///뱀파이어망토 박쥐 이펙트 처리.(캐릭터 선택화면을 구분하기 위해 인덱스를 체크.)
	{
		if( m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 1774 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
		if( vnum == 1774 ) // 새로 뱀파이어 망토를 끼는 경우.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.
		}

		if( m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 11273 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
		if( vnum == 11273 ) // 새로 뱀파이어 망토를 끼는 경우.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.
		}
	}

	int proto_num = g_ItemList.FindItem(vnum);

	if (proto_num >= 0)
	{
		SAFE_DELETE(m_EventWearing[where]);
		m_EventWearing[where] = new CItem;
		if( !m_EventWearing[where] )
			return;
		m_EventWearing[where]->m_Index = index;
		m_EventWearing[where]->m_Vnum = vnum;
		m_EventWearing[where]->m_ProtoNum = proto_num;
		//2003 착용하고 있는 서브아이템을 플러스아이템으로 인식하지 않게 하기.
		if(GET_TYPE(m_EventWearing[where])==ITYPE_SUB)
			m_EventWearing[where]->m_PlusNum = gem_num;
		else if (gem_num > 0)
		{
			if(g_ItemList.m_ItemArray[proto_num].m_MinLevel>=181)
				m_EventWearing[where]->SetDiaGemNum(gem_num);
			else
				m_EventWearing[where]->SetGemNum(gem_num);
		}

		m_EventWearing[where]->m_Special = special;

		///신방어구강화시스템
		m_EventWearing[where]->m_Special2 = special2; // 추가 옵션
		m_EventWearing[where]->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		m_EventWearing[where]->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

		m_EventWearing[where]->m_Endurance = Endur;
		m_EventWearing[where]->m_MaxEndurance = MaxEndur;

		// 아이템 값을 입력해보자!
		ResetItemValue(m_EventWearing[where]);
		SetSpecialValue(m_EventWearing[where], GET_TYPE(m_EventWearing[where]), GET_SHAPE(m_EventWearing[where]), special, special2);
	}

	OnBossRaidHeroEffect();
}

int CNkCharacter::Wear(int where)
{
	if (where >= 0)
	{
		if (!m_Wearing[where])
			return -1;
	}
	else
	{
		if (!m_ExtraSlot)
			return -1;
	}

	if (m_ExtraSlot)
	{
		if ((where = CanWear()) < 0)
			return -1;
	}

	if (where == WEARING_SHOES
			&& pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
	{
		return -1;
	}

	CItem *pItem = NULL;

	if (m_Wearing[where])
		pItem = m_Wearing[where];

	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1774 )
	{
		if( !m_EventWearing[3] ) // 이벤트 망토를 안 끼고있는 경우만...
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.
	}
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 )
	{
		if( !m_EventWearing[3] ) // 이벤트 망토를 안 끼고있는 경우만...
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.
	}

	m_Wearing[where] = m_ExtraSlot;
	m_ExtraSlot = pItem;

	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1774 )///뱀파이어망토 망토를 뺀 경우.
	{
		if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 1774) ) // 이벤트 망토를 안 끼고있는 경우만...
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
	}
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 )///뱀파이어망토 망토를 뺀 경우.
	{
		if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 11273) ) // 이벤트 망토를 안 끼고있는 경우만...
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
	}



	if ((where == WEARING_WEAPON || where == WEARING_SHIELD) && !IsDead())
		ResetMotionState();

	return where;
}

int CNkCharacter::EventWear(int where) ///이벤트복장착용
{
	if( !g_bUseEventWear ) // 이벤트 복장 착용 시스템을 지원 안하는 나라는 작동안하게...
		return -1;

	// -1인 경우는 m_ExtraSlot에 아템이 있는 경우,,
	// where가 있는 경우는 m_ExtraSlot에 아템이 없는 경우.
	if (where >= 0)
	{
		if (!m_EventWearing[where])
			return -1;
	}
	else
	{
		// where 가 -1인 경우에는 extra slot에 있어야 한다.
		if (!m_ExtraSlot)
			return -1;
	}

	// ExtraSlot에 있는 경우에는 착용가능한 장비인가 확인.
	if (m_ExtraSlot)
	{
		if ((where = CanWear()) < 0)
			return -1;
	}

	//if (where == WEARING_SHIELD)
	//{
	//	return -1;
	//}

	if (where == WEARING_SHOES
			&& pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz))
	{
		// 하늘에서는 신발 못 바꾼다.
		return -1;
	}

	CItem *pItem = NULL;

	if (m_EventWearing[where])
		pItem = m_EventWearing[where];



	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1774 ) ///뱀파이어망토 망토를 낄 경우.
		pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 ) ///뱀파이어망토 망토를 낄 경우.
		pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 넣어준다.

	m_EventWearing[where] = m_ExtraSlot;
	m_ExtraSlot = pItem;

	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1774 )///뱀파이어망토 망토를 뺀 경우.
		pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 )///뱀파이어망토 망토를 뺀 경우.
		pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.


	if ((where == WEARING_WEAPON || where == WEARING_SHIELD) && !IsDead())
		ResetMotionState();

	return where;
}

int	CNkCharacter::CanPetWear()
{
	int where = -1;
	if( !m_ExtraSlot )
		return -20;

	// if( m_ExtraSlot->m_Vnum
	// 1709,1710,1711
	//	1521 1522 1523 1524



	return where;
}
// 엑스트라 슬롯의 무기가 해당 캐릭이 장착할 수 있는지 판단.
int CNkCharacter::CanWear()
{
	int where = -1;
	if (m_ExtraSlot)
	{
		if (!(GET_ITEM_RACE(m_ExtraSlot) & (1 << m_BpdRace)))
			return -11;

		if( g_bGlobalMatrix )
		{
			if( g_dwClientCountry == CTRY_KOR ) // 한국의 경우 입을 수 없는 아이템
			{
				if( m_ExtraSlot->m_Vnum == 492 || m_ExtraSlot->m_Vnum == 493 // 기모노
						|| m_ExtraSlot->m_Vnum == 923 || m_ExtraSlot->m_Vnum == 924 // 새 기모노
						|| m_ExtraSlot->m_Vnum == 518 || m_ExtraSlot->m_Vnum == 519 || m_ExtraSlot->m_Vnum == 520 // 닌자복
						|| m_ExtraSlot->m_Vnum == 1422 || m_ExtraSlot->m_Vnum == 1423 // 일본 축구 유니폼
						|| m_ExtraSlot->m_Vnum == 1410 // 쿨레인 깃발
				  )
					return -13;
			}
			else if( g_dwClientCountry == CTRY_JPN ) // 일본의 경우 입을 수 없는 아이템
			{
				if( m_ExtraSlot->m_Vnum == 301 || m_ExtraSlot->m_Vnum == 303 // 한복
						|| m_ExtraSlot->m_Vnum == 433 || m_ExtraSlot->m_Vnum == 434 // 새 한복
						|| m_ExtraSlot->m_Vnum == 1289 || m_ExtraSlot->m_Vnum == 1290 || m_ExtraSlot->m_Vnum == 920 // 태권도 복(태두건 포함)
						|| m_ExtraSlot->m_Vnum == 1418 || m_ExtraSlot->m_Vnum == 1419 // 일본 축구 유니폼
						|| m_ExtraSlot->m_Vnum == 1411 // 호크 깃발
				  )
					return -13;
			}
		}

		if(GET_TYPE(m_ExtraSlot)!=ITYPE_SUB)
		{
			if (m_ExtraSlot->m_MinLevel > m_Level)
				return -2;

			if ( (GET_ITEM_MAXLEVEL(m_ExtraSlot) < m_Level )
					&& ( m_ExtraSlot->m_Vnum == 1238		// 데미크루셔
						 || m_ExtraSlot->m_Vnum == 1259		// 홀리스테프( 호크 )
						 || m_ExtraSlot->m_Vnum == 1260		// 홀리스테프( 쿨레인 )
						 || m_ExtraSlot->m_Vnum == 1261 ))	// 홀리스테프( 쉬버 )
				return -2;

			if( g_MatrixSvr != 0
					&& ( m_ExtraSlot->m_Vnum >= 1259
						 && m_ExtraSlot->m_Vnum <= 1261)
					|| (m_ExtraSlot->m_Vnum >= 2170
						&&m_ExtraSlot->m_Vnum <= 2171))
			{
				switch( g_MatrixSvr ) // [2008/11/24 Theodoric] WORLD_MATRIX_2009
				{
				case 1:
					if( !m_GuildIndex
							|| !( ( m_GuildIndex == 1 && m_ExtraSlot->m_Vnum == 1259 )		// 홀리스테프( 호크 )
								  || ( m_GuildIndex == 2 && m_ExtraSlot->m_Vnum == 1260 )		// 홀리스테프( 쿨레인 )
								  || ( m_GuildIndex == 3 && m_ExtraSlot->m_Vnum == 1261 )))	// 홀리스테프( 쉬버 )
						return -13;
					break;
				case 2:
					if( !m_GuildIndex
							|| !( ( m_GuildIndex == 1 && m_ExtraSlot->m_Vnum == 2170 )		// 툰드마르	홀리 스테프
								  || (m_GuildIndex == 2 && m_ExtraSlot->m_Vnum == 2171 )))	// 안그마르 홀리 스테프
						return -13;
					break;
				}
			}
		}
		else
		{
			// 카이립톤
			if(g_pRoh->m_Race==RACE_KAILIPTON)
			{
				if(GET_SHAPE(m_ExtraSlot)==IETC_SCROLL)
				{
					if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILL_BOOK_READ))
						return -2;
				}
				else if(GET_SHAPE(m_ExtraSlot)==ISUB_WAND)
				{
					if( m_ExtraSlot->m_Vnum == 1021 )
					{
						if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILL_VANPIRE_TOUCH))
							return -2;
					}
					else
					{
						if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILL_MAGIC_MOVE))
							return -2;
					}
				}
			}
			else if(g_pRoh->m_Race==RACE_BULKAN)
			{
				if( GET_SHAPE(m_ExtraSlot)==ISUB_SLAYER || GET_SHAPE(m_ExtraSlot)==ISUB_CHAKRAM ) // 슬레이어나 챠크람이라면...
				{
					if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILL_THROW))
						return -2;
				}
			}
			else if(g_pRoh->m_Race==RACE_HUMAN)
			{
				if( m_ExtraSlot->m_Vnum == 1023 )
				{
					if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILL_DEEP_ATTACK))
						return -2;
				}
				else if( m_ExtraSlot->m_Vnum != 829 && m_ExtraSlot->m_Vnum != 830 // 레이다 / 하드배리어 변환
						 && m_ExtraSlot->m_Vnum != 831 && m_ExtraSlot->m_Vnum != 832   // 퍼지변환 / 볼트저항
						 && m_ExtraSlot->m_Vnum != 833 && m_ExtraSlot->m_Vnum != 834   // 볼케닉저항 / 석화저항
						 && m_ExtraSlot->m_Vnum != 835 && m_ExtraSlot->m_Vnum != 842   // 콜드저항 / 통합저항
						 && m_ExtraSlot->m_Vnum != 844                                 // 통합변환
					   ) // 종족 공통 착용 가능 소형 장비들은 휴먼도 착용스킬레벨 제한이 없도록 한다.
				{
					if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILL_HANDLE))
						return -2;
				}
			}
			else if(g_pRoh->m_Race==RACE_AIDIA)
			{
				if( m_ExtraSlot->m_Vnum == 1022
						|| m_ExtraSlot->m_Vnum == 3067 || m_ExtraSlot->m_Vnum == 10206 || m_ExtraSlot->m_Vnum == 10417 || m_ExtraSlot->m_Vnum == 11065 || m_ExtraSlot->m_Vnum == 11086) // Update Elite
				{
					if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILL_MELODY_OF_DEF))
						return -2;
				}
			}
			else if(g_pRoh->m_Race==RACE_FREAK)
			{
				if(GET_SHAPE(m_ExtraSlot)==ISUB_DSTONE) // 디바인 스톤 이라면...
				{
					if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(SKILLF_A_DSTONE))
						return -2;
				}
			}
			else if(g_pRoh->m_Race==RACE_PEROM)
			{
				if(GET_SHAPE(m_ExtraSlot)==ISUB_PANEL)
				{
					if(m_ExtraSlot->m_MinLevel > g_ChaSkill.GetLevelByIndex(PEROM_SKILL_S139))
						return -2;
				}
			}
		}

		if (m_ExtraSlot->m_Int > m_Int)
			return -7;

		if (m_ExtraSlot->m_Dex > m_Dex)
			return -8;

		int where_weight = 0;
		switch (GET_TYPE(m_ExtraSlot))
		{
		case ITYPE_SUB:
			break;
		case ITYPE_ARMOR:
			where = GET_SHAPE(m_ExtraSlot);
			break;
		case ITYPE_WEAPON:
			where = WEARING_WEAPON;
			break;
		case ITYPE_ETC:
			{
				if( GET_SHAPE(m_ExtraSlot) == IETC_EFFECT )
					return m_ExtraSlot->m_Num0;
				return -20;
			}
			break;
		default:
			return -20;
		}
		// 일단 무게 검사.
		if (m_Str < m_ExtraSlot->m_Weight)
			return -3;

		///이벤트복장착용 시스템 장착가능 체크 ///070723 일본버그리포트 수정 때 위치이동.
		if( g_bUseEventWear )
		{
			if( g_pNk2DFrame->GetControlInven() && g_pNk2DFrame->GetControlInven()->m_bEventWearView ) // 이벤트 복장 슬롯을 보고있는 경우만...
			{
				if( !(GET_ATT_FLAG(m_ExtraSlot) & IATT_EVENT_WEAR) ) // 이벤트 복장 슬롯에 착용 불가능한 아이템인 경우
					return -14;
			}
		}

		if(GET_ITEM_LEVEL(m_ExtraSlot) > 0 && GET_ATT_FLAG(m_ExtraSlot) & IATT_MAX_LEVEL)
		{
			if( g_pRoh->m_Level > GET_ITEM_MAXLEVEL(m_ExtraSlot ) || g_pRoh->m_Level < GET_ITEM_LEVEL(m_ExtraSlot) )
				return -2;
		}

		//----------------------------------------------------------------------
		// Extra Slot 에 들고 있는 장비 종류 판별후 행동/ 메시지 처리
		switch (GET_TYPE(m_ExtraSlot))
		{
		case ITYPE_ARMOR:
			// 방패를 들때, 이미 두손 무기를 들었다면 불가능 메세지
			// 휴먼 메타/하이메타 의 경우 예외적으로 장착 가능
			// 2004.05.12
			// Lyul / 수정완료
			if (where == WEARING_SHIELD )
			{
				//  [5/18/2009 ppmmjj83] 일본디자인컨테스트 방패들은 메타방패처럼 두손무기차도 장착가능하게.
				if( m_ExtraSlot->m_Vnum == 1588 || m_ExtraSlot->m_Vnum == 1589
						|| ( m_ExtraSlot->m_Vnum >= 2219 && m_ExtraSlot->m_Vnum <= 2221 )
						|| ( m_ExtraSlot->m_Vnum >= 2245 && m_ExtraSlot->m_Vnum <= 2250 )
						|| m_ExtraSlot->m_Vnum == 10712 || m_ExtraSlot->m_Vnum == 1127
						|| m_ExtraSlot->m_Vnum == 1037 || m_ExtraSlot->m_Vnum == 1038
						|| m_ExtraSlot->m_Vnum == 1288 || m_ExtraSlot->m_Vnum == 10852
						|| m_ExtraSlot->m_Vnum == 11235 || m_ExtraSlot->m_Vnum == 11274
						|| m_ExtraSlot->m_Vnum == 4634
				  )
					return where;

				if( m_Race == RACE_HUMAN )
				{
					return where;
				}

				if( m_Race == RACE_FREAK )	// 하이브리더
				{
					if( m_Wearing[WEARING_WEAPON] )
					{
						if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER )
						{
							return where;
						}
						else if(  GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD )	// 이도류 착용시
							return -12;
					}

					if( m_Wearing[ WEARING_CLOAK ] &&
							GET_TYPE(m_Wearing[WEARING_CLOAK]) == ITYPE_ARMOR &&
							GET_SHAPE(m_Wearing[WEARING_CLOAK]) == IARMOR_CLOAK )
					{
						return -12;
					}
				}

				if( m_Wearing[WEARING_WEAPON] )
				{
					if (GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN)
					{
						return -9;
					}
					else if (GET_RENDER_FLAG(m_Wearing[WEARING_WEAPON]) > 0 && m_Race != RACE_BULKAN)
					{
						return -5;
					}
				}
			}
			else if( where == WEARING_CLOAK )
			{
				if( m_Race == RACE_FREAK )
				{
					if( m_Wearing[ WEARING_WEAPON ] )
					{
						if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER )
						{
							return -12;
						}
						else if(  GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD )
						{
							return where;
						}
					}

					if( m_Wearing[ WEARING_SHIELD ] )
					{
						return -12;
					}
				}
			}
			return where;

		case ITYPE_WEAPON:
			if( where != WEARING_WEAPON )
			{
				return FALSE;
			}

			if( m_Race == RACE_FREAK )
			{
				if( m_Wearing[ WEARING_CLOAK ] &&
						GET_TYPE(m_Wearing[WEARING_CLOAK]) == ITYPE_ARMOR &&
						GET_SHAPE(m_Wearing[WEARING_CLOAK]) == IARMOR_CLOAK )
				{
					if( GET_SHAPE(m_ExtraSlot) == IWEAPON_HAMMER )
					{
						return -12;
					}
					else if( GET_SHAPE(m_ExtraSlot) == IWEAPON_DUAL_SWORD )
					{
						if( m_Wearing[WEARING_SHIELD] )
						{
							return -12;
						}
						else
						{
							return where;
						}
					}
				}
			}

			if( m_Wearing[WEARING_SHIELD] )
			{
				//  [5/18/2009 ppmmjj83] 일본디자인컨테스트 방패들은 메타방패처럼 두손무기차도 장착가능하게.
				if( m_Wearing[WEARING_SHIELD]->m_Vnum == 1588 || m_Wearing[WEARING_SHIELD]->m_Vnum == 1589
						|| ( m_Wearing[WEARING_SHIELD]->m_Vnum >= 2219 && m_Wearing[WEARING_SHIELD]->m_Vnum <= 2221 )
						|| ( m_Wearing[WEARING_SHIELD]->m_Vnum >= 2245 && m_Wearing[WEARING_SHIELD]->m_Vnum <= 2250 )
						|| m_Wearing[WEARING_SHIELD]->m_Vnum == 10712 || m_Wearing[WEARING_SHIELD]->m_Vnum == 1127
						|| m_Wearing[WEARING_SHIELD]->m_Vnum == 1037 || m_Wearing[WEARING_SHIELD]->m_Vnum == 1038
						|| m_Wearing[WEARING_SHIELD]->m_Vnum == 1288 || m_Wearing[WEARING_SHIELD]->m_Vnum == 10852
						|| m_Wearing[WEARING_SHIELD]->m_Vnum == 11235 || m_Wearing[WEARING_SHIELD]->m_Vnum == 11274
						|| m_Wearing[WEARING_SHIELD]->m_Vnum == 4634

				  )
					return where;

				if( m_Race == RACE_HUMAN )
				{
					return where;
				}

				if( m_Race == RACE_FREAK )
				{
					if( m_Wearing[WEARING_SHIELD] )
					{
						if( GET_SHAPE(m_ExtraSlot) == IWEAPON_DUAL_SWORD )
						{
							return -12;
						}
						else if( GET_SHAPE(m_ExtraSlot) == IWEAPON_HAMMER )
						{
							return where;
						}
					}
				}
				if(GET_SHAPE(m_Wearing[WEARING_SHIELD])==ISUB_WAND)
				{
					return where;
				}

				if (GET_SHAPE(m_ExtraSlot) == IWEAPON_GUN)
				{
					return -10;
				}
				else if (GET_RENDER_FLAG(m_ExtraSlot) > 0 && m_Race != RACE_BULKAN)
				{
					return -6;
				}
			}
			return where;
		///2003 서브아이템을 망토나 무기 인벤에 착용할수 있는지 구분.
		case ITYPE_SUB://서브아이템인 경우
			if ((GET_SHAPE(m_ExtraSlot) == ISUB_L_EQUIP) 		// 대형장비인지 구분.
					|| (GET_SHAPE(m_ExtraSlot) == ISUB_SLAYER) 		// 슬레이어인지 구분.
					|| (GET_SHAPE(m_ExtraSlot) == ISUB_DSTONE)
					|| (GET_SHAPE(m_ExtraSlot) == ISUB_PANEL)
			   )		// 디바인 스톤 // Lyul_2005
				return 3; //망토속성

			if (GET_SHAPE(m_ExtraSlot) == ISUB_WAND) 		//완드 인지 구분.
				//	return 9; //무기속성
				return 5; //방패속성

			if (GET_SHAPE(m_ExtraSlot) == ISUB_S_EQUIP) 		//소형장비인지 구분.
				return 8; //팔찌속성

			where=-20; //서브 무기 아닌 서브아이템이 장착되믄 안되니까..
			return where;

		default:
			// 장비도 무기도 아니면.. 당근  안되징...
			return -20;
		}
	}
	else
		return -20;
}

// 장착하고 있는 무기에 보석을 떨어트려 Upgrade 를 한다.
// 2004.03.05 - 수정완료
// Lyul / 수정(다이아 버그 )
// 다이아 일 경우 보석 1당 한번의 Upgrade 의 시도가 이루어 지므로
// Plus 처리를 피한다.
int CNkCharacter::Upgrade(int where)
{
	// 빈 슬롯이 아니고 장착한 아이템이 있을때 그 아이템이 Up grade 가 불가능
	// 하다면 return -1
	if (m_ExtraSlot && m_Wearing[where])
	{
		if ((where = CanUpgrade(where)) < 0)
			return -1;
	}
	else
		return -1;

	// 다이아급 이상일 경우는 원샷 업그레이드를 한다.
	if(GET_GEMMIN_LEVEL(m_ExtraSlot) >= 181)
		return where;

	// plus업그레이인가 와 보석 추가단계를 구별하여 준다.
	// plus 단계일경우에는 수치를 조절하지 않는다.
	if (m_Wearing[where]->m_GemNum < m_Wearing[where]->m_PlusNum )
		m_Wearing[where]->m_GemNum += 1;

	return where;
}

// 장착하고 있는 무기에 보석을 떨어트려 Upgrade가능한지 판별한다.
// -1            : 불가능
// others = where: 가능
// 2004.03.05 - 수정완료
// Lyul / Comment
// 2004.05.12 - 수정완료
// Lyul / 카이 하이메타 , 에댜 빛의 망토 업글 불가.
// 2004.08.13 - 수정중
// 신무기 추가로 인한 리젠트 다이아 업글
int CNkCharacter::CanUpgrade(int where)
{
	if (!m_ExtraSlot || !m_Wearing[where])
		return -1;

	if( where == WEARING_PET1 || where == WEARING_PET2 || where == WEARING_PET3 || where == WEARING_PET4 )
		return -1;

	if (GET_TYPE(m_ExtraSlot) != ITYPE_ETC || GET_SHAPE(m_ExtraSlot) != IETC_GEM)
		return -1;
	
	if(GET_ATT_FLAG(m_Wearing[where]) & (1 << 25))
	{
		//check fullopt
	int value = 0;
	switch(m_ExtraSlot->m_Vnum)
	{
	case 5400:
		value = (m_Wearing[where]->m_Special2 >> 0) & 7;
		break;
	case 5401:
		value = (m_Wearing[where]->m_Special2 >> 3) & 7;
		break;
	case 5402:
		value = (m_Wearing[where]->m_Special2 >> 6) & 7;
		break;
	case 5403:
		value = (m_Wearing[where]->m_Special2 >> 25) & 7;
		break;
	case 5404:
		if(where == 9 || where == 3 || where == 5)return -1;


		value = (m_Wearing[where]->m_Special2 >> 9) & 1;
		break;
	case 5405:
		if(where == 9 || where == 3 || where == 5)return -1;

		value = (m_Wearing[where]->m_Special2 >> 28) & 1;
		break;
	}
	if(m_ExtraSlot->m_Vnum == 5404 || m_ExtraSlot->m_Vnum == 5405)
	{
		if(((m_Wearing[where]->m_Special2 >> 9) & 1 ) > 0 ||  ((m_Wearing[where]->m_Special2 >> 28) & 1) > 0)
			return -1;
		else
			return where;

	}
	else
	{
		if(value >= 7)
		{
			return -1;
		}
		else
		{
			return where;
		}
	}
	}


	if (GET_ATT_FLAG(m_Wearing[where]) & IATT_NO_GEM)
		return -1;

	if( CheckCanNpcUpgradeItem(m_Wearing[where]) ) // NPC 업그레이드 아이템 이면 일반 업그레이드 불가 처리. ///신방어구강화시스템
		return -1;

	if( m_Wearing[where]->m_MinLevel_Org > 261 )
		return -1;

	///시간제아이템 업그레이드 불가 처리 (05-07-13 by 원석)
	if( m_Wearing[where]->m_bTimeLimit ) // 시간제 아이템이면...
		return -1;

	// 업그레이드 불가 아이템 : 메타 / 하이메타 방어구
	if(m_Wearing[where]->m_Vnum==875		// 불칸 메타 방패
			|| m_Wearing[where]->m_Vnum==876	// 휴먼 메타 방패
			|| m_Wearing[where]->m_Vnum==573	// 카이 메타 케이프
			|| m_Wearing[where]->m_Vnum==575	// 에댜 빛의 날개
			|| m_Wearing[where]->m_Vnum==1027	// 불칸 하이메타 방패
			|| m_Wearing[where]->m_Vnum==1028	// 휴먼 하이메타 방패
			|| m_Wearing[where]->m_Vnum==1029	// 카이 하이메타 방패
			|| m_Wearing[where]->m_Vnum==1030	// 에댜 하이 빛의 날개
			|| m_Wearing[where]->m_Vnum==1234	// 하이브리더 메타 방패
			|| m_Wearing[where]->m_Vnum==1235	// 하이브리더 하이메타 방패
			|| m_Wearing[where]->m_Vnum==1236	// 하이브리더 메타 케이프
			|| m_Wearing[where]->m_Vnum==1237
			|| ( m_Wearing[where]->m_Vnum >= 2049 && m_Wearing[where]->m_Vnum <= 2055 )
			|| ( m_Wearing[where]->m_Vnum >= 2173 && m_Wearing[where]->m_Vnum <= 2179 )
			|| ( m_Wearing[where]->m_Vnum >= 10454 && m_Wearing[where]->m_Vnum <= 10484 ) // NEW WEAPON
			|| m_Wearing[where]->m_Vnum==10517
			|| ( m_Wearing[where]->m_Vnum >= 10522 && m_Wearing[where]->m_Vnum <= 10585 ) // NEW WEAPON
			|| ( m_Wearing[where]->m_Vnum >= 2939 && m_Wearing[where]->m_Vnum <= 2941 )
			|| ( m_Wearing[where]->m_Vnum >= 2942 && m_Wearing[where]->m_Vnum <= 2953 )
	  )	// 하이브리더 하이메타 케이프
		return -1;



	// 장비의 랩제와 보석의 랩제를 확인.
	int item_level = GET_ITEM_LEVEL(m_Wearing[where]);
	int min_level = GET_GEMMIN_LEVEL(m_ExtraSlot);
	int max_level = GET_GEMMAX_LEVEL(m_ExtraSlot);


	
	// 다이아몬드급 이고 한계 Level 이내일 경우 (<15) Can
	// 리젠트 다이아몬드급 이상 이고 한계 Level 이내일 경우 (<15) Can
	if(item_level >= min_level
			&& item_level <= max_level
			&& (min_level == 181 || min_level == 261)
			&& m_Wearing[where]->m_PlusNum<15 )
		return where;

	// 일반 보석이고 허용 Level 이내일 경우
	else if ( item_level >= min_level && item_level <= max_level)
	{
		// +5 이내일 경우 Can
		if (m_Wearing[where]->m_PlusNum < 5)
			return where;
		else
			return -1;
	}
	else
		return -1;
}

// 장착하고 있는 Item 을 제거한다.
void CNkCharacter::Remove(int where)
{
	if (!m_Wearing[where])
		return;

	if( where == 3 ) ///뱀파이어망토 박쥐 이펙트 처리.(///071101 다른 캐릭터가 벗을때 처리 추가)
	{
		if( m_Wearing[where]->m_Vnum == 1774 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			if( !(m_EventWearing[where] && m_EventWearing[where]->m_Vnum == 1774) ) // 이벤트 망토를 안 끼고있는 경우만...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
		if( m_Wearing[where]->m_Vnum == 11273 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			if( !(m_EventWearing[where] && m_EventWearing[where]->m_Vnum == 11273) ) // 이벤트 망토를 안 끼고있는 경우만...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
	}

	delete m_Wearing[where];
	m_Wearing[where] = NULL;

	if (where == WEARING_WEAPON)
		ResetMotionState();

	OnBossRaidHeroEffect();
}

// 장착하고 있는 Item 을 제거한다.
void CNkCharacter::EventWearRemove(int where) ///이벤트복장착용
{
	if (!m_EventWearing[where])
		return;

	if( where == 3 ) ///뱀파이어망토 박쥐 이펙트 처리.(///071101 다른 캐릭터가 벗을때 처리 추가)
	{
		if( m_EventWearing[where]->m_Vnum == 1774 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			if( !(m_Wearing[where] && m_Wearing[where]->m_Vnum == 1774) ) // 일반 망토가 뱀파이어 망토인 경우는 빼고...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
		if( m_EventWearing[where]->m_Vnum == 11273 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
		{
			if( !(m_Wearing[where] && m_Wearing[where]->m_Vnum == 11273) ) // 일반 망토가 뱀파이어 망토인 경우는 빼고...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
		}
	}

	delete m_EventWearing[where];
	m_EventWearing[where] = NULL;

	if (where == WEARING_WEAPON)
		ResetMotionState();

	OnBossRaidHeroEffect();
}

// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
void CNkCharacter::AddToInven(int pack_num, int x, int y, int index, int vnum, int gem_num, int special, int special2,
							  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///신방어구강화시스템
{
	if (vnum < 0)
		return;

	if (pack_num == 3)
	{
		// 교환창에 있는 아이템 슬롯.
		AddToExgInven(x, y, index, vnum, gem_num, special, special2, UpEndur, MaxUpdEndur, Endur, MaxEndur ); ///신방어구강화시스템
		return;
	}

	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		CItem *pItem = NULL;
		pItem = new CItem;
		if( !pItem )
			return;

		pItem->m_Index = index;
		pItem->m_Vnum = vnum;
		pItem->m_ProtoNum = proto_num;

		// 아이템 값을 입력해보자!
		ResetItemValue(pItem);

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		//0917  인벤창의 서브아이템을 플러스 아이템으로 인식하는거 막음.
		if(GET_TYPE(pItem)==ITYPE_SUB)
		{
			pItem->m_PlusNum = gem_num;
		}
		else if (gem_num > 0)
		{
			// 2004. 03. 22  / Lyul
			// 랩프리 다이아 장비 인식 오류 Big Fix
			if(GET_ITEM_LEVEL(pItem)>=181)
				pItem->SetDiaGemNum(gem_num);
			else
				pItem->SetGemNum(gem_num);
		}
		//---------------------------------------------------------------------
		BOOL bEmptySlot = TRUE;
		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
			{
				if(m_Slot[pack_num][x+x_idx][y+y_idx] > -1)
					bEmptySlot = FALSE; // 아이템 집는순간 아이템이 놓여질 슬롯에 유저가 다른 아이템을 옮겨 놓았다.
			}
		}

		if( !bEmptySlot && !m_ExtraSlot)
		{
			SetExtraSlot(index, vnum, gem_num, special, special2, UpEndur, MaxUpdEndur, Endur, MaxEndur); ///신방어구강화시스템

			if(pItem)
			{
				delete pItem ;
				pItem = NULL;
			}

			return;
		}
		//---------------------------------------------------------------------
		pItem->m_Special = special;

		///신방어구강화시스템
		pItem->m_Special2 = special2; // 추가 옵션
		pItem->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

		pItem->m_Endurance = Endur;
		pItem->m_MaxEndurance = MaxEndur;

		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, special2);
		pItem->m_Next = m_Inven[pack_num];
		m_Inven[pack_num] = pItem;
		// nate 2004 - 3
		// 창고 슬롯 확장 관련
		// 퀵슬롯 카운트 버그 수정
		CountQuickAll();

		for ( int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				m_Slot[pack_num][x+x_idx][y+y_idx] = index;
		}

// by evilkiki 일단 주석처리
//		g_pNk2DFrame->GetUpgrade()->CheckUpgradeNeed();

	}
}

void CNkCharacter::AddToInven( int nPackNum, int nItemIndex, int nSlot_X, int nSlot_Y, int nVnum )
{}

// [6/7/2007 Theodoric] 내구도 수리 :: 함수 인자 추가
BOOL CNkCharacter::AddToExgInven(int x, int y, int index, int vnum, int gem_num, int special, int special2,
								 int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, BOOL bForce) ///신방어구강화시스템
{
	if (vnum < 0)
		return FALSE;

	if (m_bExgLock && bForce == FALSE)
		return FALSE;

	int proto_num = g_ItemList.FindItem(vnum);

	if (proto_num >= 0)
	{
		CItem *pItem = NULL;
		pItem = new CItem;
		if( !pItem )
			return FALSE;

		pItem->m_Index = index;
		pItem->m_Vnum = vnum;
		pItem->m_ProtoNum = proto_num;

		// 아이템 값을 입력해보자!
		ResetItemValue(pItem);

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		///2003  교환창 인벤토리의 서브아이템을 플러스 아이템으로 인식하는거 막기
		if(GET_TYPE(pItem)==ITYPE_SUB)
		{
			pItem->m_PlusNum = gem_num;
		}
		else if (gem_num > 0)
		{
			// 2004. 03. 22  / Lyul
			// 랩프리 다이아 장비 인식 오류 Big Fix
			if(GET_ITEM_LEVEL(pItem) >= 181)
				pItem->SetDiaGemNum(gem_num);
			else
				pItem->SetGemNum(gem_num);
		}

		pItem->m_Special = special;

		///신방어구강화시스템
		pItem->m_Special2 = special2; // 추가 옵션
		pItem->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

		pItem->m_Endurance = Endur;
		pItem->m_MaxEndurance = MaxEndur;

		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, special2);
		pItem->m_Next = m_ExgInven;
		m_ExgInven = pItem;

		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				m_ExgSlot[x+x_idx][y+y_idx] = index;
		}

// by evilkiki 일단 주석처리
		//g_pNk2DFrame->GetUpgrade()->CheckUpgradeNeed();

	}

	// by evilkiki 2009.10.21 아이템 정보 갱신
	if( g_pNk2DFrame->GetControlInven() )
		g_pNk2DFrame->GetControlInven()->ResetItemInfo();

	return TRUE;
}

BOOL CNkCharacter::AddToInven(int x, int y, int pack_num)
{
	CItem *pItem = NULL;
	int index;
	int nExtraShape, nInvenShape;

	if (pack_num < 0)
		pack_num = m_CurPackNum;		// 현재 팩 넘버로 셋팅

	if (m_ExtraSlot)	 // Extra Slot 에 아이템이 있는 경우
	{
		index = GetItemIndexInSlot(pack_num, x, y, GET_ITEM_WIDTH(m_ExtraSlot), GET_ITEM_HEIGHT(m_ExtraSlot));

		if (index < -1)
			return FALSE;

		if (index >= 0)
			pItem = PickItemInPack(pack_num, index);

		m_ExtraSlot->m_SlotX = x;
		m_ExtraSlot->m_SlotY = y;
		m_ExtraSlot->m_Next = m_Inven[pack_num];
		m_Inven[pack_num] = m_ExtraSlot;

		// 아이템의 크기 계산
		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(m_ExtraSlot); x_idx++)
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(m_ExtraSlot); y_idx++)
				m_Slot[pack_num][x+x_idx][y+y_idx] = m_ExtraSlot->m_Index;

		m_ExtraSlot = pItem;
		pItem = NULL;

		BOOL bRecount = FALSE;
		if (m_ExtraSlot)
		{
			nExtraShape = GET_SHAPE(m_ExtraSlot);

			if ( GET_TYPE(m_ExtraSlot) == ITYPE_ETC )
				bRecount = TRUE;
		}
		else if (m_Inven[pack_num])
		{
			nInvenShape = GET_SHAPE(m_Inven[pack_num]);

			if ( GET_TYPE(m_Inven[pack_num]) == ITYPE_ETC )
				bRecount = TRUE;
		}

		if (bRecount)
			CountQuickAll();
	}
	else
	{
		index = GetItemIndexInSlot(pack_num, x, y, 1, 1);

		if (index >= 0)
		{
			m_ExtraSlot = PickItemInPack(pack_num, index);
		}
		else
			return FALSE;

		if (m_ExtraSlot)
		{
			nExtraShape = GET_SHAPE(m_ExtraSlot);

			if ( GET_TYPE(m_ExtraSlot) == ITYPE_ETC )
				CountQuickAll();
		}
	}

	return TRUE;
}

// nate 2004 - 4 : Exchange - 교환창에 아이템을 넣거나 뺄때
BOOL CNkCharacter::AddToExgInven(int x, int y, BOOL bForce)
{
	if (m_bExgLock && bForce == FALSE)
		return FALSE;

	CItem *pItem = NULL;
	int index;
	// 교환창에 넣을때
	if (m_ExtraSlot)
	{
		index = GetItemIndexInExgSlot(x, y, GET_ITEM_WIDTH(m_ExtraSlot), GET_ITEM_HEIGHT(m_ExtraSlot));

		if (index < -1)				// 못 놓는 경우.
			return FALSE;
		if (index >= 0)
			pItem = PickItemInExgPack(index);

		m_ExtraSlot->m_SlotX = x;
		m_ExtraSlot->m_SlotY = y;
		m_ExtraSlot->m_Next = m_ExgInven;
		m_ExgInven = m_ExtraSlot;

		for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(m_ExtraSlot); x_idx++)
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(m_ExtraSlot); y_idx++)
				m_ExgSlot[x+x_idx][y+y_idx] = m_ExtraSlot->m_Index;

		m_ExtraSlot = pItem;

		if ((m_ExtraSlot &&
				GET_TYPE(m_ExtraSlot) == ITYPE_ETC && GET_SHAPE(m_ExtraSlot) == IETC_RECOVERY) ||
				(GET_TYPE(m_ExgInven) == ITYPE_ETC && GET_SHAPE(m_ExgInven) == IETC_RECOVERY))
		{
			CountQuickAll();
		}
	}
	// 교환창에서 뺄때
	else
	{
		index = GetItemIndexInExgSlot(x, y, 1, 1);

		if (index >= 0)
		{
			m_ExtraSlot = PickItemInExgPack(index);
		}
		else
			return FALSE;		// 교환창 슬롯에 아무것도 없는 경우.

		if ((m_ExtraSlot && GET_TYPE(m_ExtraSlot) == ITYPE_ETC && \
				(GET_SHAPE(m_ExtraSlot) == IETC_RECOVERY || GET_SHAPE(m_ExtraSlot) == IETC_GEMSTONE || \
				 GET_SHAPE(m_ExtraSlot) == IETC_GEM))
				|| (m_ExtraSlot && GET_TYPE(m_ExtraSlot) == ITYPE_ETC &&(GET_SHAPE(m_ExtraSlot) == IETC_SCROLL || GET_SHAPE(m_ExtraSlot) == IETC_SHELL || GET_SHAPE(m_ExtraSlot) == IETC_CHARGE)))
		{
			CountQuickAll();
		}
	}

	// by evilkiki 2009.10.21 아이템 정보 갱신
	if( g_pNk2DFrame->GetControlInven() )
		g_pNk2DFrame->GetControlInven()->ResetItemInfo();

	return TRUE;
}

// 이 부분은 서버에서만 호출된다.
BOOL CNkCharacter::DelFromExgInven(int x, int y)
{
	int index;
	CItem *pItem = NULL;
	index = GetItemIndexInExgSlot(x, y, 1, 1);

	if (index >= 0)
		pItem = PickItemInExgPack(index);
	else
		return FALSE;

	//========================================================================
	// nate 2004 - 4 : exchange - ExtraSlot이 비어있을때 뻑남
	if ( ( pItem
			&& GET_TYPE( pItem ) == ITYPE_ETC
			&& ( GET_SHAPE( pItem ) == IETC_RECOVERY
				 || GET_SHAPE( pItem ) == IETC_GEMSTONE
				 || GET_SHAPE( pItem ) == IETC_GEM ) )
			|| ( m_ExtraSlot
				 && ( GET_TYPE( m_ExtraSlot ) == ITYPE_ETC
					  && ( GET_SHAPE( m_ExtraSlot ) == IETC_SCROLL
						   || GET_SHAPE( m_ExtraSlot ) == IETC_SHELL
						   || GET_SHAPE( m_ExtraSlot ) == IETC_CHARGE ) ) ) )

		CountQuickAll();
	//========================================================================

	if (pItem)
		delete pItem;

	return TRUE;
}

BOOL CNkCharacter::AddToInven(int pack_num, int *x, int *y)
{
	if (!m_ExtraSlot || pack_num < 0 || pack_num >= PACK_NUM)
		return FALSE;

	for (int x_idx = 0; x_idx <= SLOT_X_NUM - GET_ITEM_WIDTH(m_ExtraSlot); x_idx++)
	{
		for (int y_idx = 0; y_idx <= SLOT_Y_NUM - GET_ITEM_HEIGHT(m_ExtraSlot); y_idx++)
		{
			BOOL found = TRUE;
			for (int x_idx2 = 0; x_idx2 < GET_ITEM_WIDTH(m_ExtraSlot) && found; x_idx2++)
			{
				for (int y_idx2 = 0; y_idx2 < GET_ITEM_HEIGHT(m_ExtraSlot); y_idx2++)
				{
					if (m_Slot[pack_num][x_idx+x_idx2][y_idx+y_idx2] != -1)
					{
						found = FALSE;
						break;
					}
				}
			}

			if (found)
			{
				*x = x_idx;
				*y = y_idx;
				return AddToInven(x_idx, y_idx, pack_num);
			}
		}
	}

	return FALSE;
}

int cntChakram = 0;

BOOL CNkCharacter::ToQuickSlot(int slot_num)
{
	if( m_ExtraSlot && g_dwLangType == LANG_KOR )
	{
		if( m_ExtraSlot->m_Vnum >= 1960 && m_ExtraSlot->m_Vnum <= 1967 )
		{
			return FALSE;
		}
	}

	if (m_ExtraSlot)
	{
		switch( m_ExtraSlot->m_Vnum )
		{
		case 2430:
		case 2431:
		case 2432:
			if( slot_num != 3 )
				return FALSE;
		}

		if(GET_ATT_FLAG(m_ExtraSlot) & IATT_SUB_QUICK )
		{
			// m_ExtraSlot->m_Vnum == 693 차크람 퀵슬롯에 추가
		}
		else if( GET_TYPE( m_ExtraSlot ) == ITYPE_SUB && GET_SHAPE( m_ExtraSlot ) == ISUB_CHAKRAM )
		{
			int slot = -1;
			BOOL bChakram = FALSE;

			for(int i = 0; i<QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
			{
				if(m_QuickSlot[i] && (GET_SHAPE(m_QuickSlot[i])==ISUB_CHAKRAM))
				{
					bChakram = TRUE;
					slot = i;
				}
			}

			if(bChakram)
			{
				if(slot_num != slot)
					return FALSE;
			}
		}
		else if (GET_TYPE(m_ExtraSlot) == ITYPE_ETC )
		{
			// gem type 퀵슬롯 등록 필터링 by. Sol9 [2012/12/06]
			if( GET_SHAPE(m_ExtraSlot) != IETC_RECOVERY && GET_SHAPE(m_ExtraSlot) != IETC_GEMSTONE &&
					/*GET_SHAPE(m_ExtraSlot) != IETC_GEM &&*/ GET_SHAPE(m_ExtraSlot) != IETC_QUICK			&&
					GET_SHAPE(m_ExtraSlot) != IETC_WARP && GET_SHAPE(m_ExtraSlot) != IETC_BUFF &&
					GET_SHAPE(m_ExtraSlot) != IETC_QUEST && GET_SHAPE(m_ExtraSlot) != IETC_PET &&
					GET_SHAPE(m_ExtraSlot) != IETC_PREMIUM && GET_SHAPE(m_ExtraSlot) != IETC_SKILL &&
					GET_SHAPE(m_ExtraSlot) != IETC_SCROLL && GET_SHAPE(m_ExtraSlot) != IETC_SUMMON &&
					GET_SHAPE(m_ExtraSlot) != IETC_SHELL && GET_SHAPE(m_ExtraSlot) != IETC_CHARGE &&
					GET_SHAPE(m_ExtraSlot) != IECT_MOBSUMMON && GET_SHAPE(m_ExtraSlot) != IETC_USING &&
					GET_SHAPE(m_ExtraSlot) != IETC_SUMMONNPC )

				return FALSE;
		}
		else
		{
			return FALSE;
		}
	}

	if (slot_num < 0 || slot_num >= QUICK_SLOT_NUM*QUICK_SLOT_PAGE)
		return FALSE;

	// 이건 락이 걸려 있는 슬롯번호이다.
	if (m_bQuickSlotLock[slot_num])
		return FALSE;

	if (!m_ExtraSlot && !m_QuickSlot[slot_num])
		return FALSE;

	if (m_ExtraSlot && !(GET_ITEM_RACE(m_ExtraSlot) & (1 << m_BpdRace)))
		return FALSE;

	CItem *pItem = m_QuickSlot[slot_num];
	m_QuickSlot[slot_num] = m_ExtraSlot;
	m_ExtraSlot = pItem;

	// 지정 전송 장치를 퀵슬롯에 등록하거나 해제할 때
	// 지정 전송 장치 UI를 닫아 준다
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1015 )
	{
		if( pCMyApp && pCMyApp->m_pUIMgr )
			pCMyApp->m_pUIMgr->CloseTeleportItem();
	}

	CountQuickAll();

	return TRUE;
}

// 이건 서버에서 슬롯에 집어 넣어주는 명령이다.
//2003 함수의 인자 증가 plus_num,special_num
void CNkCharacter::ToQuickSlot(int slot_num, int index, int vnum, int plus_num, int special_num, int special2_num )
{
	if (slot_num < 0 || slot_num >= QUICK_SLOT_NUM*QUICK_SLOT_PAGE || vnum < 0)
		return;

	int proto_num = g_ItemList.FindItem(vnum);

	if (proto_num >= 0)
	{
		if (m_QuickSlot[slot_num])
			delete m_QuickSlot[slot_num];

		m_QuickSlot[slot_num] = new CItem;
		if( !m_QuickSlot[slot_num] )
			return;
		m_QuickSlot[slot_num]->m_Index = index;
		m_QuickSlot[slot_num]->m_Vnum = vnum;
		m_QuickSlot[slot_num]->m_ProtoNum = proto_num;
		///2003 서브아이템의 정보 받기(갯수 같은거)
		m_QuickSlot[slot_num]->m_PlusNum = plus_num;
		m_QuickSlot[slot_num]->m_Special = special_num;
		m_QuickSlot[slot_num]->m_Special2 = special2_num;

		// 아이템 값을 입력해보자!
		ResetItemValue(m_QuickSlot[slot_num]);
		CountQuickBySlot(slot_num);

		// 보석생성시 락이 걸려 있는 슬롯을 풀어준다.
		if (m_bQuickSlotLock[slot_num])
			m_bQuickSlotLock[slot_num] = FALSE;
	}
}
//==============================================================================
// [12/9/2008 D.K ] :
// 인벤토리 정보 확인 코드.
// 각 팩별 인벤토리가 꽉 찼는지에 대한 정보를 넘겨준다.
//==============================================================================
// [12/9/2008 D.K ] : BOOL IsFullInvenInPack( int _packNo )
// return value : if inventory is full in pack no, return true.
BOOL CNkCharacter::IsFullInvenInPack( int _packNo )
{
	int *pSlot = &m_Slot[_packNo][0][0];
	for( int i = 0; i < SLOT_X_NUM * SLOT_Y_NUM; i++ )
	{
		if( *( pSlot + i ) <= 0 )
			return FALSE;
	}
	return TRUE;
}
//==============================================================================
// [12/9/2008 D.K ] : BOOL IsFullInvenInAllPack()
// return value : if inventory is full in all pack, return true.
BOOL CNkCharacter::IsFullInvenInAllPack()
{
	return (IsFullInvenInPack( 0 ) && IsFullInvenInPack( 1 ) && IsFullInvenInPack( 2 ) && IsFullInvenInPack( 4 ));
}

//===============================================================
// [3/5/2009 D.K ] : int GetEmptyInvenInPack();
// return value : if inventory is not full, return emplty count.
int CNkCharacter::GetEmptyInvenInPack( int _packNo )
{
	if( _packNo == 3 )
		return 0;

	int emptyCount = 0;
	int *pSlot = &m_Slot[_packNo][0][0];
	for( int i = 0; i < SLOT_X_NUM * SLOT_Y_NUM; i++ )
	{
		if( *( pSlot + i ) <= 0 )
			emptyCount++;
	}

	return emptyCount;
}

int	CNkCharacter::GetCountInExch( int nVnum )
{
	int count = 0;

	CItem *pItem = m_ExgInven;
	while( pItem )
	{
		if ( pItem->m_Vnum == nVnum )
			count++;

		pItem = pItem->m_Next;
	}

	return count;
}

int CNkCharacter::GetCountInPack( int nVnum )
{
	int count = 0;
	// 슬롯 갯수만큼 루프
	for ( int pack_num = 0 ; pack_num < PACK_NUM ; pack_num ++ )
	{
		CItem *pItem = m_Inven[ pack_num ];

		while( pItem )
		{
			if ( pItem->m_Vnum == nVnum )
				count++;

			pItem = pItem->m_Next;
		}
	}
	return count;
}

int	CNkCharacter::GetCountInPack( int pack ,int vnum )
{
	int count = 0;

	CItem *pItem = m_Inven[ pack ];
	while( pItem )
	{
		if ( pItem->m_Vnum == vnum )
			count++;

		pItem = pItem->m_Next;
	}

	return count;
}

int CNkCharacter::GetCountStrInPack(int pack , int vnum , int need_count, char * param)
{
	if( !param )
		return -1;

	param[0] = 0;

	int count = 0;

	CItem *pItem = m_Inven[ pack ];
	while( pItem )
	{
		if( pItem->m_Vnum == vnum )
		{
			char buf[32];
			buf[0] = 0;
			sprintf( buf , "%d %d %d" , pItem->m_Index , pItem->m_SlotX , pItem->m_SlotY );
			strcat( param , buf );
			count++;

			if( need_count == count )
				break;

			strcat( param , " " );
		}

		pItem = pItem->m_Next;
	}

	return count;
}

//==============================================================================
// nate 2004 - 4
// 소환석 퀵슬롯 자동 채우기 기능
// 소환석 필터링 제저
void CNkCharacter::CountQuickAll()
{
	for ( int slot_num = 0 ; slot_num < QUICK_SLOT_NUM*QUICK_SLOT_PAGE ; slot_num ++ )
	{
		if( !m_QuickSlot[ slot_num ] )
			continue;

		int count = 1;

		for( int pack_num = 0 ; pack_num < PACK_NUM ; pack_num ++ )
		{
			CItem *pItem = m_Inven[ pack_num ];

			while( pItem )
			{
				if( pItem->m_Vnum == m_QuickSlot[ slot_num ]->m_Vnum )
				{
					count ++;
				}

				pItem = pItem->m_Next;
			}
		}

		if( g_pNk2DFrame->GetControlBottom() )
		{
			if( ( ( GET_TYPE( m_QuickSlot[ slot_num ] ) == ITYPE_SUB )
					&& ( GET_SHAPE( m_QuickSlot[ slot_num ] ) == ISUB_BOOK ) ) )
				g_pNk2DFrame->GetControlBottom()->SetQuickNoCount(slot_num);

			if( m_QuickCount[ slot_num ] != count )
			{
				m_QuickCount[ slot_num ] = count;
				if( !( ( GET_TYPE( m_QuickSlot[ slot_num ] ) == ITYPE_SUB )
						&& ( GET_SHAPE( m_QuickSlot[ slot_num ] ) == ISUB_BOOK ) )
						&& m_QuickSlot[ slot_num ]->m_Vnum !=926 && m_QuickSlot[ slot_num ]->m_Vnum !=936
						&& ( GET_SHAPE( m_QuickSlot[ slot_num ] ) != ISUB_CHAKRAM ) )
					g_pNk2DFrame->GetControlBottom()->SetQuickCount( slot_num );
				else
					g_pNk2DFrame->GetControlBottom()->SetQuickNoCount( slot_num );
			}
		}
	}
}
//==============================================================================

void CNkCharacter::CountQuickBySlot(int slot_num)
{
	if ( !m_QuickSlot[ slot_num ] )
		return;

	if( !g_pNk2DFrame->GetControlBottom() )
		return;

	int vnum = m_QuickSlot[ slot_num ]->m_Vnum;

	// 슬롯 갯수만큼 루프
	for ( int i = 0 ; i < QUICK_SLOT_NUM ; i ++ )
	{
		if ( !m_QuickSlot[ i ] || m_QuickSlot[ i ]->m_Vnum != vnum )
			continue;

		int count = 1;

		for ( int pack_num = 0 ; pack_num < PACK_NUM ; pack_num ++ )
		{
			CItem *pItem = m_Inven[ pack_num ];

			while( pItem )
			{
				if ( pItem->m_Vnum == m_QuickSlot[ i ]->m_Vnum )
					count++;

				pItem = pItem->m_Next;
			}
		}
		//==============================================================================
		// nate 2004 - 4
		// 소환석 퀵슬롯 자동 채우기 기능
		// 필터링 주석처리
		if( g_pNk2DFrame->GetControlBottom() )
		{
			if( m_QuickCount[i] != count )
			{
				m_QuickCount[i] = count;
				///2003 퀵슬롯의 아이템이 책이라면 갯수를 카운트 하지 않는다.
				if( !( ( GET_TYPE( m_QuickSlot[ i ] ) == ITYPE_SUB )
						&& ( GET_SHAPE( m_QuickSlot[ i ] ) == ISUB_BOOK ) )
						&& m_QuickSlot[ i ]->m_Vnum != 926
						&& m_QuickSlot[ i ]->m_Vnum != 936
						&& ( GET_SHAPE( m_QuickSlot[ i ] )!=ISUB_CHAKRAM ) )
					g_pNk2DFrame->GetControlBottom()->SetQuickCount( i );
				else
					g_pNk2DFrame->GetControlBottom()->SetQuickNoCount( i );
			}

			if( ( ( GET_TYPE( m_QuickSlot[ i ] )== ITYPE_SUB )
					&& ( GET_SHAPE( m_QuickSlot[ i ] ) == ISUB_BOOK ) )
					|| m_QuickSlot[ i ]->m_Vnum == 926
					|| m_QuickSlot[ i ]->m_Vnum == 936 )
				g_pNk2DFrame->GetControlBottom()->SetQuickNoCount( i );
		}
		//==============================================================================
	}
}

int CNkCharacter::GetItemIndexInSlot(int nInvenPackNum, int nInvenSlotX, int nInvenSlotY, int nItemWidth, int nItemHeight)
// nate - 인벤 슬롯에서 인덱스 추출
{
	int index = -1;
	for( int x_idx = nInvenSlotX ; x_idx < nInvenSlotX + nItemWidth ; x_idx ++ )
	{
		for( int y_idx = nInvenSlotY; y_idx < nInvenSlotY + nItemHeight ; y_idx ++ )
		{
			// NkCharacter가 인벤토리 슬롯배열을 관리한다.
			// Inven의 Slot에 아이템이 있으면 인덱스 저장
			if( m_Slot[ nInvenPackNum ][ x_idx ][ y_idx ] != -1 )
			{
				if( index == -1 )	// 인덱스를 찾으면 다시는 들어오지 않는다.
					index = m_Slot[ nInvenPackNum ][ x_idx ][ y_idx ];
				// 현재 아이템의 크기에 맞게 인덱스값 비교
				// 다를경우는 에러( 현 아이템 위에 다른게 있다는.
				else if ( index != m_Slot[ nInvenPackNum ][ x_idx ][ y_idx ] )
					return -2;		// 에러
			}
		}
	}

	return index;
}

// exchange slot( 교환창 )에 대한것.
int CNkCharacter::GetItemIndexInExgSlot(int x, int y, int w, int h)
{
	int index = -1;

	for (int x_idx = x; x_idx < x+w; x_idx++)
	{
		for (int y_idx = y; y_idx < y+h; y_idx++)
		{
			if (m_ExgSlot[x_idx][y_idx] != -1)
			{
				if (index == -1)
					index = m_ExgSlot[x_idx][y_idx];
				else if (index != m_ExgSlot[x_idx][y_idx])
					return -2;
			}
		}
	}

	return index;
}

// nate
CItem* CNkCharacter::PickItemInPack(int pack_num, int index, BOOL isVnum)
// 인자 : InvenPackNum, ItemIndexNum
// 기능 : 현재인벤팩에서 아이템을 검사하고 제거하는 기능
// 메모리 누적 유발
// 제거된 아이템은 delete해야됨
// 반환값 : 현재 인덱스의 아이템 클래스 반환
{
	// 연결리스트 구조
	CItem *beforeItem = NULL;		// 현재 앞의 아이템을 가르킴
	CItem *pItem = m_Inven[ pack_num ];		// 현재팩의 첫번째 아이템이 나옴

	while ( pItem )
	{
		// 현재팩의 아이템 검색
		// 인자 IndexNum또는 Vnum과 같은 아이템 검색
		if ( ( !isVnum && pItem->m_Index == index ) || ( isVnum && pItem->m_Vnum == index ) )
		{
			if ( beforeItem )
			{
				beforeItem->m_Next = pItem->m_Next;
				// nate 2004 - 3 : 창고 아이템 다량이동 관련
				// 추가 : 퀵슬롯의 카운트 감소
				CountQuickAll();
			}
			else
			{
				m_Inven[pack_num] = pItem->m_Next;
				// nate 2004 - 3 : 창고 아이템 다량이동 관련
				// 추가 : 퀵슬롯의 카운트 감소
				CountQuickAll();
			}

			// 아이템의 크기에 맞게 슬롯의 인덱스값을 -1로 최기화
			for( int x_idx = 0 ; x_idx < GET_ITEM_WIDTH( pItem ) ; x_idx++ )
				for (int y_idx = 0 ; y_idx < GET_ITEM_HEIGHT( pItem ) ; y_idx++ )
					m_Slot[ pack_num ][ pItem->m_SlotX + x_idx ][ pItem->m_SlotY + y_idx ] = -1;

			pItem->m_Next = NULL;
			break;
		}
		beforeItem = pItem;
		pItem = pItem->m_Next;
	}

	return pItem;
}

CItem* CNkCharacter::PickPowerInPack(int pack_num, int type_idx, int shape_idx, int app_to)
{
	CItem *beforeItem = NULL;
	CItem *pItem = m_Inven[pack_num];

	while (pItem)
	{
		if (GET_TYPE(pItem) == type_idx && GET_SHAPE(pItem) == shape_idx &&	GET_APPTO(pItem) == app_to)
		{
			if( pItem->m_Vnum == 1015 // 지정 전송장치는 다시 채워주기 안하도록 처리(오토방지용) (05-11-08 원석)
					|| pItem->m_Vnum == 2222 || pItem->m_Vnum == 2223 || pItem->m_Vnum == 2224) // 일본용 부활스톤
			{
				pItem = pItem->m_Next;

				continue;
			}

			// connie [2009/3/9] -  일본요청 - 메달은 회복템으로 사용안되도록
			if( g_dwClientCountry == CTRY_JPN && (pItem->m_Vnum == 2118 || pItem->m_Vnum == 2119 ||pItem->m_Vnum == 2120) )
			{
				pItem = pItem->m_Next;

				continue;
			}


			if (beforeItem)
				beforeItem->m_Next = pItem->m_Next;
			else
				m_Inven[pack_num] = pItem->m_Next;

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					m_Slot[pack_num][pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;

			pItem->m_Next = NULL;
			break;
		}
		beforeItem = pItem;
		pItem = pItem->m_Next;
	}

	return pItem;
}

BOOL CNkCharacter::UseItemInPack(int index, BOOL isVnum)
{
	//return FALSE;
	// 이 함수는 인벤에서 오른쪽 클릭으로 물약등 사용할때 들어오는 함수.
	// 교환창. 개인 상점등이 열려있을땐 인벤 오른쪽 클릭으로 사용 못하게 추가(07-02-20 원석)
	if( g_pNk2DFrame->IsItemMultiMoveEnable() ) // 오른쪽 더블클릭으로 다중이동시키는 창들이 떠잇으면...
		return FALSE;


	CItem *beforeItem = NULL;
	CItem *pItem = m_Inven[m_CurPackNum];

	while (pItem)
	{
		if ((!isVnum && pItem->m_Index == index) || (isVnum && pItem->m_Vnum == index))
		{
			if (GET_TYPE(pItem) == ITYPE_ETC
					&& (GET_SHAPE(pItem) == IETC_RECOVERY /*|| GET_SHAPE(pItem) == IETC_GEMSTONE*/))
			{
				if (beforeItem)
					beforeItem->m_Next = pItem->m_Next;
				else
					m_Inven[m_CurPackNum] = pItem->m_Next;

				for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
					for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
						m_Slot[m_CurPackNum][pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;

				pItem->m_Next = NULL;
				delete pItem;
				return TRUE;
			}
		}
		beforeItem = pItem;
		pItem = pItem->m_Next;
	}

	return FALSE;
}

CItem* CNkCharacter::PickItemInExgPack(int index, BOOL isVnum)
{
	CItem *beforeItem = NULL;
	CItem *pItem = m_ExgInven;

	while (pItem)
	{

		if ((!isVnum && pItem->m_Index == index) || (isVnum && pItem->m_Vnum == index))
		{
			if (beforeItem)
				beforeItem->m_Next = pItem->m_Next;
			else
				m_ExgInven = pItem->m_Next;

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					m_ExgSlot[pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;

			pItem->m_Next = NULL;
			break;
		}
		beforeItem = pItem;
		pItem = pItem->m_Next;
	}

	return pItem;
}

CItem* CNkCharacter::FindItemAndPackByIndexFromAll(int index, int& pack)
{
	pack = -1;
	int i = 0;
	CItem *pItem = NULL;

	// 착용한 장비에서 아이템을 찾는다.
	for (i = 0; i < WEARING_NUM; i++)
	{
		pItem = m_Wearing[i];

		if (pItem && pItem->m_Index == index)
			return pItem;
	}

	///이벤트복장착용 시스템이 도입되어있으면 이벤트 복장 슬롯도 검사.
	if( g_bUseEventWear )
	{
		for (i = 0; i < WEARING_NUM; i++)
		{
			pItem = m_EventWearing[i];

			if (pItem && pItem->m_Index == index)
				return pItem;
		}
	}

	// extra에서 찾는다.
	if (m_ExtraSlot && m_ExtraSlot->m_Index == index)
		return m_ExtraSlot;

	// Inven 1,2,3에서 찾는다.
	for (i=0; i < PACK_NUM; i++)
	{
		pItem = m_Inven[i];

		while(pItem)
		{
			if (pItem->m_Index == index)
			{
				pack = i;
				return pItem;
			}
			else
			{
				pItem = pItem->m_Next;
			}
		}
	}

	// 교환창에서 찾는다.
	pItem = m_ExgInven;

	while (pItem)
	{
		if (pItem->m_Index == index)
			return pItem;
		else
			pItem = pItem->m_Next;
	}

	// Quick Slot 체크
	for (i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		pItem = m_QuickSlot[i];

		if (pItem && pItem->m_Index == index)
			return pItem;
	}

	return NULL;
}

// 모든 소지품창내에서 아이템을 찾는다.
// 아이템의 참조만을 돌려준다.
// 리턴된 아이템에 대한 삭제 금지.
CItem *CNkCharacter::FindItemByIndexFromAll(int index)
{
	int temp = 0;
	return FindItemAndPackByIndexFromAll(index, temp);
}

// Vnum 으로 부터 검색하는 함수. 여러개 있으면 그중 하나만 리턴해준다.
CItem *CNkCharacter::FindItemByVnumFromAll( int vnum ) ///인던1
{
	int i = 0;
	CItem *pItem = NULL;

	// 착용한 장비에서 아이템을 찾는다.
	for (i = 0; i < WEARING_NUM; i++)
	{
		pItem = m_Wearing[i];

		if (pItem && pItem->m_Vnum == vnum)
			return pItem;
	}

	///이벤트복장착용 시스템이 도입되어있으면 이벤트 복장 슬롯도 검사.
	if( g_bUseEventWear )
	{
		for (i = 0; i < WEARING_NUM; i++)
		{
			pItem = m_EventWearing[i];

			if (pItem && pItem->m_Vnum == vnum)
				return pItem;
		}
	}

	// extra에서 찾는다.
	if (m_ExtraSlot && m_ExtraSlot->m_Vnum == vnum)
		return m_ExtraSlot;

	// Inven 1,2,3에서 찾는다.
	for (i=0; i < PACK_NUM; i++)
	{
		pItem = m_Inven[i];

		while(pItem)
		{
			if (pItem->m_Vnum == vnum)
				return pItem;
			else
				pItem = pItem->m_Next;
		}
	}

	// 교환창에서 찾는다.
	pItem = m_ExgInven;

	while (pItem)
	{
		if (pItem->m_Vnum == vnum)
			return pItem;
		else
			pItem = pItem->m_Next;
	}

	// Quick Slot 체크
	for (i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		pItem = m_QuickSlot[i];

		if (pItem && pItem->m_Vnum == vnum)
			return pItem;
	}

	return NULL;
}

// 해당 아이템이 어디 들어있는지 찾아주는 함수. (현재는 인벤(0~2)과 교환창(3) 만 지원.
int CNkCharacter::CheckWherePackItem( CItem *DestItem ) ///인던1
{
	int i = 0;
	CItem *pItem = NULL;

	if( !DestItem )
		return -1; // 없음.
	/*
		// 착용한 장비에서 아이템을 찾는다.
		for (i = 0; i < WEARING_NUM; i++) {
			pItem = m_Wearing[i];

			if (pItem && pItem == DestItem )
				return ;
		}
		// extra에서 찾는다.
		if (m_ExtraSlot && m_ExtraSlot == DestItem)
			return ;
	*/
	// Inven 1,2,3에서 찾는다.
	for (i=0; i < PACK_NUM; i++)
	{
		pItem = m_Inven[i];

		while(pItem)
		{
			if (pItem == DestItem)
				return i;
			else
				pItem = pItem->m_Next;
		}
	}

	// 교환창에서 찾는다.
	pItem = m_ExgInven;

	while (pItem)
	{
		if (pItem == DestItem)
			return 3;
		else
			pItem = pItem->m_Next;
	}

	return -1; // 없음.
}

int CNkCharacter::GetSlotIndex(int x, int y)
{
	if (x < 0 || x >= SLOT_X_NUM || y < 0 || y >= SLOT_Y_NUM)
		return -1;

	return m_Slot[m_CurPackNum][x][y];
}

int CNkCharacter::GetExgSlotIndex (int x, int y)
{
	if (x < 0 || x >= EXG_SLOT_X_NUM || y < 0 || y >= EXG_SLOT_Y_NUM)
		return -1;

	return m_ExgSlot[x][y];
}

// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
void CNkCharacter::SetExtraSlot(int index, int vnum, int gem_num, int special, int special2,
								int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///신방어구강화시스템
{
	int proto_num = g_ItemList.FindItem(vnum);

	if (proto_num >= 0)
	{
		SAFE_DELETE(m_ExtraSlot);
		m_ExtraSlot = new CItem;
		if( !m_ExtraSlot )
			return;
		m_ExtraSlot->m_Index = index;
		m_ExtraSlot->m_Vnum = vnum;
		m_ExtraSlot->m_ProtoNum = proto_num;
		////2003 서브아이템을 플러스 아이템으로 인식하는거 막기.
		if(GET_TYPE(m_ExtraSlot)==ITYPE_SUB)
			m_ExtraSlot->m_PlusNum = gem_num;
		else if (gem_num > 0)
		{
			if(g_ItemList.m_ItemArray[proto_num].m_MinLevel>=181)
				m_ExtraSlot->SetDiaGemNum(gem_num);
			else
				m_ExtraSlot->SetGemNum(gem_num);
		}
		m_ExtraSlot->m_Special = special;

		///신방어구강화시스템
		m_ExtraSlot->m_Special2 = special2; // 추가 옵션
		m_ExtraSlot->m_UpgradeEndurance = UpEndur;           // 현재 강화 내구도
		m_ExtraSlot->m_MaxUpgradeEndurance = MaxUpdEndur;    // 강화 내구도 최대치

		m_ExtraSlot->m_Endurance = Endur;
		m_ExtraSlot->m_MaxEndurance = MaxEndur;

		// 아이템 값을 입력해보자!
		ResetItemValue(m_ExtraSlot);
		SetSpecialValue(m_ExtraSlot, GET_TYPE(m_ExtraSlot), GET_SHAPE(m_ExtraSlot), special, special2);
	}
}

void CNkCharacter::RemoveByIndex( int index )
{
	if( RemoveEquipItem(index) )
		return;
	if( RemoveEventItem(index) )
		return;
	if( RemoveInvenItem( index ) )
		return;
	if( RemoveExchageItem( index ) )
		return;
	if( RemoveExtraItem( index ) )
		return;
	if( RemoveQuickItem( index ) )
		return;
}


void CNkCharacter::RemoveAll()
{
	// 입은 것 다 벗기고...
	for (int i = 0; i < WEARING_NUM; i++)
		SAFE_DELETE(m_Wearing[i]);

	// 장비 몽땅 날리고...
	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	for (int i = 0; i < PACK_NUM; i++)
	{
		pItem = m_Inven[i];
		while (pItem)
		{
			nextItem = pItem->m_Next;
			delete pItem;
			pItem = nextItem;
		}
		m_Inven[i] = NULL;

		for (int x = 0; x < SLOT_X_NUM; x++)
			for (int y = 0; y < SLOT_Y_NUM; y++)
				m_Slot[i][x][y] = -1;
	}

	// 교환 아이템 몽땅 날린다.
	pItem = m_ExgInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	m_ExgInven = NULL;

	for (int i = 0 ; i < EXG_SLOT_X_NUM ; i++)
		for (int j = 0; j < EXG_SLOT_Y_NUM ; j++)
			m_ExgSlot[i][j] = -1;

	// Extra Slot 없애고...
	SAFE_DELETE(m_ExtraSlot);
	// Quick Slot 없앴다!
	for ( int i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		SAFE_DELETE(m_QuickSlot[i]);
		m_QuickCount[i] = 0;
	}
}
// [2008/8/14 Theodoric] 운영자가 인벤을 손쉽게 지우기 위해 만든 패킷
void CNkCharacter::RemoveAllInven(int slot)
{
	int i;
	int sSlot = 0;
	int eSlot = 0;
	switch( slot )
	{
	case 0:
		sSlot = 0;
		eSlot = 1;
		break;
	case 1:
		sSlot = 1;
		eSlot = 2;
		break;
	case 2:
		sSlot = 2;
		eSlot = 3;
		break;
	case 4:
		sSlot = 4;
		eSlot = 5;
		break;
	case 99:
		sSlot = 0;
		eSlot = 5;
		break;
	default:
		return;
	}

	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	for(i=sSlot; i<eSlot; i++)
	{
		pItem = m_Inven[i];
		while (pItem)
		{
			nextItem = pItem->m_Next;
			delete pItem;
			pItem = nextItem;
		}
		m_Inven[i] = NULL;

		for (int x = 0; x < SLOT_X_NUM; x++)
			for (int y = 0; y < SLOT_Y_NUM; y++)
				m_Slot[i][x][y] = -1;
	}
}

void CNkCharacter::RemoveAllEggs()
{
	CItem *pItem;

	// 입고 있는 것중에서 알을 빼자!
	if( m_Wearing[WEARING_CLOAK] )
	{
		if( m_Wearing[WEARING_CLOAK]->m_Vnum == 1161
				|| ( m_Wearing[WEARING_CLOAK]->m_Vnum >= 614
					 && m_Wearing[WEARING_CLOAK]->m_Vnum <= 617)
				|| m_Wearing[WEARING_CLOAK]->m_Vnum == 2319
				|| m_Wearing[WEARING_CLOAK]->m_Vnum == 3158
		  )
			SAFE_DELETE(m_Wearing[WEARING_CLOAK]);
	}

	// 인벤에도 있으면 없애자!
	for (int i = 0; i < PACK_NUM; i++)
	{
		for (int vnum = 614; vnum <= 617; vnum++)
		{
			while ( (pItem = PickItemInPack(i, vnum, TRUE) ) != NULL)
			{
				delete pItem;
			}
		}

		// 하이브리더 알포데기도 검사해서 삭제.  (by 원석)
		while ((pItem = PickItemInPack(i, 1161, TRUE)) != NULL)
			delete pItem;
		while ((pItem = PickItemInPack(i, 2319, TRUE)) != NULL)
			delete pItem;
		while ((pItem = PickItemInPack(i, 3158, TRUE)) != NULL)
			delete pItem;
	}
}

int CNkCharacter::GetKitsCount()
{
	int count = 0;
	// 모든 슬롯팩들을 순환하면서 보석생산킷의 숫자를 구한다.
	for (int pack_num = 0; pack_num < PACK_NUM; pack_num++)
	{
		CItem *pItem = m_Inven[pack_num];
		while (pItem)
		{
			if (pItem->m_Vnum == 229)
				count++;
			pItem = pItem->m_Next;
		}
	}

	return count;
}

CGuildData*  CNkCharacter::GetMyGuildData()
{
	return m_pMyGuilddata;
}

t_Info CNkCharacter::GetMyGuildRevisionTime()
{
	return m_pMyGuilddata->GetMyGuildInfo().s_revisiontime;
}

int CNkCharacter::GetMyGuildLevel()
{
	return m_pMyGuilddata->GetMyInfo().s_glevel;
}

int CNkCharacter::GetGuildLevel()
{
	return m_pMyGuilddata->GetMyGuildInfo().s_glevel;
}

int	CNkCharacter::GetMyGuildIndex()
{
	return m_pMyGuilddata->GetMyGuildInfo().s_guildIndex;
}

int CNkCharacter::GetMyGuildTotalMemberCnt()
{
	return m_pMyGuilddata->GetGuildinfo().s_totalmember;
}

int	CNkCharacter::GetMyGuildGeneralMemberCnt()
{
	return m_pMyGuilddata->GetGuildinfo().s_genericmember;
}
int	CNkCharacter::GetMyGuildRecomWaitCnt()
{
	return m_pMyGuilddata->GetGuildinfo().s_recomwaitmember;
}
int	CNkCharacter::GetMyGuildGeneralWatiCnt()
{
	return m_pMyGuilddata->GetGuildinfo().s_generalwaitmember;
}

char* CNkCharacter::GetMyGuildName()
{
	return m_pMyGuilddata->GetMyGuildInfo().s_guildName;
}

char* CNkCharacter::GetInvasionAttackGuild()
{
	return m_pMyGuilddata->GetInvasionAttackName();
}

t_Info CNkCharacter::GetinvasionRemainTime()
{
	return m_pMyGuilddata->GetInvasionRemainTime();
}

char* CNkCharacter::GetInvasionDefenseGuild()
{
	return NULL;
}

BOOL CNkCharacter::MakeMyGuildData()
{
	DeleteMyGuildData();

	if(m_pMyGuilddata == NULL)
	{
		m_pMyGuilddata = CGuildData::Create();

		return TRUE;
	}

	return FALSE;
}

void CNkCharacter::DeleteMyGuildData()
{
	if(m_pMyGuilddata && g_pNk2DFrame && g_pNk2DFrame->GetControlGuild())
		g_pNk2DFrame->GetControlGuild()->all_reset();

	if(m_pMyGuilddata && g_pNk2DFrame && g_pNk2DFrame->GetControlGuildManage())
		g_pNk2DFrame->GetControlGuildManage()->all_data_Reset();

	SAFE_DELETE(m_pMyGuilddata);

}

int CNkCharacter::CheckInvasionGNameType(char* guildname)
{
	INVASION_CHECK invasion_check = m_pMyGuilddata->CheckInvasionGNameType(guildname);

	switch(invasion_check)
	{
	case INVASION_ATTACK :
		return INVASION_ATTACK;
	case INVASION_DEFENSE :
		return INVASION_DEFENSE;
	case INVASION_NONE :
		return INVASION_NONE;
	}

	return INVASION_NONE;
}


void CNkCharacter::SetLevel(int level)
{
	// by evilkiki 2009.09.14 : ???
	m_Level = level;

	if (m_pScoreWnd)
		m_pScoreWnd->SetLevel();

	if (pCMyApp && pCMyApp->m_pUIMgr && g_pNk2DFrame->GetControlSkill() )
		g_pNk2DFrame->GetControlSkill()->SetSkillEnable();
}

void CNkCharacter::SetVital(int vital, int max_vital)
{

	m_Vital = (float)vital;
	m_AttackVital = vital;
	m_MaxVital = max_vital;
	m_LastAttackVitalTime = timeGetTime();

	if (m_pScoreWnd)
		m_pScoreWnd->SetVital();

	if (pCMyApp && pCMyApp->m_pUIMgr && g_pNk2DFrame->GetControlBottom())
		g_pNk2DFrame->GetControlBottom()->ResetInfo(-100);
}

void CNkCharacter::SetMana(int mana, int max_mana)
{
	m_Mana = (float)mana;
	m_MaxMana = max_mana;
	if (m_pScoreWnd)
		m_pScoreWnd->SetMana();

	if (pCMyApp && pCMyApp->m_pUIMgr && g_pNk2DFrame->GetControlBottom() )
		g_pNk2DFrame->GetControlBottom()->ResetInfo(-101);
}

void CNkCharacter::SetStamina(int stamina, int max_stamina)
{
	m_Stamina = (double)stamina;
	m_MaxStamina = max_stamina;
	if (m_pScoreWnd)
		m_pScoreWnd->SetStamina();

	if (pCMyApp && pCMyApp->m_pUIMgr && g_pNk2DFrame->GetControlBottom())
		g_pNk2DFrame->GetControlBottom()->ResetInfo(-102);
}

void CNkCharacter::SetEpower(int epower, int max_epower)
{
	m_Epower = (float)epower;
	m_MaxEpower = max_epower;
	if (m_pScoreWnd)
		m_pScoreWnd->SetEpower();

	if (pCMyApp && pCMyApp->m_pUIMgr&& g_pNk2DFrame->GetControlBottom())
		g_pNk2DFrame->GetControlBottom()->ResetInfo(-103);
}

void CNkCharacter::SetAbil(int str, int intel, int dex, int con, int cha)
{
	m_Str = str;
	m_Int = intel;
	m_Dex = dex;
	m_Con = con;
	m_Cha = cha;
	if (m_pScoreWnd)
		m_pScoreWnd->SetAbil();
}


void CNkCharacter::SetStr(int str)
{
	m_Str = str;
	if (m_pScoreWnd)
		m_pScoreWnd->SetStr();
}

void CNkCharacter::SetInt(int intel)
{
	m_Int = intel;
	if (m_pScoreWnd)
		m_pScoreWnd->SetInt();
}
void CNkCharacter::SetDex(int dex)
{
	m_Dex = dex;
	if (m_pScoreWnd)
		m_pScoreWnd->SetDex();
}
void CNkCharacter::SetCon(int con)
{
	m_Con = con;
	if (m_pScoreWnd)
		m_pScoreWnd->SetCon();
}
void CNkCharacter::SetCha(int cha)
{
	m_Cha = cha;
	if (m_pScoreWnd)
		m_pScoreWnd->SetCha();
}


void CNkCharacter::SetExp(__int64 exp)
{
	// by evilkiki 2009.09.14 : 총누적?
	m_Exp = exp;
	if (m_pScoreWnd)
		m_pScoreWnd->SetExp();
}

void CNkCharacter::SetNeedExp(__int64 needExp)
{
	// by evilkiki 2009.09.14  : ???
	m_NeedExp = needExp;
	if (m_pScoreWnd)
		m_pScoreWnd->Set_NeedExp();
}

void CNkCharacter::SetPenalty()
{
	if (m_pScoreWnd)
		m_pScoreWnd->SetPenalty();
}

void CNkCharacter::SetDefense()
{
	if (m_pScoreWnd)
		m_pScoreWnd->SetDefense();
}

void CNkCharacter::SetLevelupPoint(int point)
{
	m_LevelupPoint = point;
	if (m_pScoreWnd)
		m_pScoreWnd->SetLevelupPoint();

	if( g_pNk2DFrame->GetControlSkill() )
		g_pNk2DFrame->GetControlSkill()->SetLevelupPoint();
}

void CNkCharacter::SetMoney(__int64 money)
{
	m_Money = money;

	g_pNk2DFrame->SetMyMoney(money);


	if (m_pScoreWnd)
		m_pScoreWnd->SetMoney();

// 	g_pNk2DFrame->GetControlShop()->SetMoney();
//
// 	if( g_pNk2DFrame->GetPetLevel() )
// 		g_pNk2DFrame->GetPetLevel()->SetMoney(m_Money)
}

void CNkCharacter::SetFame(int fame)
{
	m_Fame = fame;
	if (m_pScoreWnd)
		m_pScoreWnd->SetFame();
	if( g_pNk2DFrame && g_pNk2DFrame->IsShopVisible() )
		g_pNk2DFrame->GetControlShop()->SetMoney( true, fame );
}

void CNkCharacter::SetCash( int Cash ) ///유료상점
{
	m_HaveCash = Cash;
	if( m_pScoreWnd )
		m_pScoreWnd->SetCash();
}

void CNkCharacter::InsertEffectNum(int num)
{
	m_listEffect.insert(num);

	EffectSort *pEffectSort = pCMyApp->m_pFxSet->GetEffect(num);
	if( pEffectSort )
	{
		pEffectSort->parentType = eEffectParentType_Char;
		pEffectSort->ptrParent = this;
	}
}

void CNkCharacter::DeleteEffectNum( int num ) // 해당 캐릭터에 등록된 이펙트중 이펙트번호(고유인덱스)를 찾아서 지워주는 녀석. 기존엔 이게 없었다. 즉, 기존 이펙트들은 사라져도 계속 메모리가 쌓여간다는 뜻... ///페어리 시 작업
{
	std::set<int>::iterator itFind = m_listEffect.find(num);
	if( itFind != m_listEffect.end() )
	{
		m_listEffect.erase(itFind);
	}
}

void CNkCharacter::SetHide(int hideStatus)
{
	switch (hideStatus)
	{
	case HIDE_HIDING:
		{
			if (m_HideStatus == HIDE_HIDING || m_HideStatus == HIDE_HIDDEN)
			{
				return;
			}
		}
		break;
	case HIDE_RISING:
		{
			if (m_HideStatus == HIDE_RISING || m_HideStatus == HIDE_SHOWN)
			{
				return;
			}
		}
		break;
	default:
		return;
	}

	SetCombat(FALSE);
	m_HideStatus = hideStatus;
	m_HideTime = timeGetTime();
}

BOOL CNkCharacter::ToggleHide()
{
	switch (m_HideStatus)
	{
	case HIDE_SHOWN:
		SetHide(HIDE_HIDING);
		break;
	case HIDE_HIDDEN:
		SetHide(HIDE_RISING);
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

void CNkCharacter::SetSnifing(BOOL bSnifing)
{
	if (bSnifing)
		m_RenderMultiShot = m_MultiShot = m_NextMultiShot = -1;
	else
		m_RenderMultiShot = m_MultiShot = m_NextMultiShot = 0;
}

BOOL CNkCharacter::IsMyTeam()
{
	int a, b;

	a = WhichTeam(g_pRoh);
	b = WhichTeam(this);

	if (a == b)
		return TRUE;
	else
		return FALSE;
}

int CNkCharacter::WhichTeam(CNkCharacter *pNkCha)
{
	if (pNkCha->m_nTeamMemGrade == TEAM_BATTLE_AHEAD
			|| pNkCha->m_nTeamMemGrade == TEAM_BATTLE_AMEMBER
			|| pNkCha->m_nTeamMemGrade == TEAM_BATTLE_AFLAG)
		return 0;

	if (pNkCha->m_nTeamMemGrade == TEAM_BATTLE_AHEAD
			|| pNkCha->m_nTeamMemGrade == TEAM_BATTLE_AMEMBER
			|| pNkCha->m_nTeamMemGrade == TEAM_BATTLE_AFLAG)
		return 1;

	return 2;
}

#define SWAP_PACK_NUM	2
BOOL CNkCharacter::SwapWeapon()
{
	int x, y, x2, y2, where;
	char strTemp[100] = "";

	if (m_ExtraSlot)	// 무기 스왑은 extre slot 을 이용하기 때문에, 뭔가가 있으면 안된다.
		return FALSE;
	else
	{
		m_ExtraSlot = PickWeaponInPack(SWAP_PACK_NUM, x, y); // 3번 인벤창에서만 꺼낸다.
		if (!m_ExtraSlot)
			return FALSE;

		if ((where = Wear(-1)) < 0)   // 착용할 수 없으면 다시 인벤에 넣는다.
		{
			AddToInven(x, y, SWAP_PACK_NUM);
			return FALSE;
		}

		if (!m_ExtraSlot)
		{
			// 원래 무기 안들고 있었나보네?
			sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x, y);
			g_pTcpIp->SendNetMessage(strTemp);
			sprintf(strTemp, "wear %d\n", where);
			g_pTcpIp->SendNetMessage(strTemp);
		}
		else if (!AddToInven(SWAP_PACK_NUM, &x2, &y2))
		{
			// 무기 슬롯에 있던걸 3번 인벤창에 넣는다.
			// 근디... 못넣으면... TT 다시 무기 슬롯에 있던 걸 꺼내서...
			// 인벤에 도로 넣어야쥐....
			if (Wear(-1) >= 0)
			{
				// 다시 착용하고...
				AddToInven(x, y, SWAP_PACK_NUM); // 인벤에 다시 넣고...
				return FALSE;
			}
			else
			{
				// 다시 착용 못해? 난 멀러.. ㅡㅡ;
				sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x, y);
				g_pTcpIp->SendNetMessage(strTemp);
				sprintf(strTemp, "wear %d\n", where);
				g_pTcpIp->SendNetMessage(strTemp);
				return FALSE;
			}
		}
		else
		{
			// 다 되었다. 서버로 메시지를 보내보자!
			sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x, y);
			g_pTcpIp->SendNetMessage(strTemp);
			sprintf(strTemp, "wear %d\n", where);
			g_pTcpIp->SendNetMessage(strTemp);
			sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x2, y2);
			g_pTcpIp->SendNetMessage(strTemp);
			//mungmae-2008/08/23 헬다운 중에 무기 바꾸면 취소.
			if( pCMyApp->m_pUIMgr->m_bRClickDown == 1 && pCMyApp->DrawCasting )
			{
				HellDownFail( g_pRoh, NULL );
			}
			// nate 2005-05-19 : 'x'버튼을 이용한 무기 교환시 방어력 제 측정
			SetDefense();
		}
	}

	return TRUE;
}

CItem* CNkCharacter::PickWeaponInPack(int pack_num, int &x, int &y)
{
	CItem *beforeItem = NULL;
	CItem *pItem = m_Inven[pack_num];

	while (pItem)
	{
		if (GET_TYPE(pItem) == ITYPE_WEAPON ||
				(GET_TYPE(pItem) == ITYPE_SUB &&
				 GET_SHAPE(pItem) == ISUB_WAND))
		{
			if (beforeItem)
				beforeItem->m_Next = pItem->m_Next;
			else
				m_Inven[pack_num] = pItem->m_Next;

			for (int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
				for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
					m_Slot[pack_num][pItem->m_SlotX+x_idx][pItem->m_SlotY+y_idx] = -1;

			pItem->m_Next = NULL;
			x = pItem->m_SlotX;
			y = pItem->m_SlotY;
			break;
		}
		beforeItem = pItem;
		pItem = pItem->m_Next;
	}

	return pItem;
}

BOOL CNkCharacter::CanSwapWeapon()
{
	switch (m_nMotionState)
	{
	case CHA_STAND :
	case CHA_WALK :
	case CHA_RUN :
	case CHA_SWIM :
	case CHA_SWIM_STAND :
		return TRUE;

	default :
		return FALSE;
	}
}

void CNkCharacter::SetFlyLevel(int flyLevel)
{
	m_FlyLevel = flyLevel;
	if (this == g_pRoh)
		g_RohFlyLevel = flyLevel;
}

void CNkCharacter::GetHalfMat(D3DMATRIX &mat, CBiped *pBiped, int bpd_idx, int CurFrame)
{
	mat._11 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._11 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._11) / 2;
	mat._12 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._12 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._12) / 2;
	mat._13 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._13 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._13) / 2;
	mat._14 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._14 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._14) / 2;
	mat._21 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._21 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._21) / 2;
	mat._22 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._22 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._22) / 2;
	mat._23 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._23 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._23) / 2;
	mat._24 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._24 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._24) / 2;
	mat._31 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._31 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._31) / 2;
	mat._32 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._32 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._32) / 2;
	mat._33 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._33 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._33) / 2;
	mat._34 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._34 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._34) / 2;
	mat._41 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._41 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._41) / 2;
	mat._42 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._42 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._42) / 2;
	mat._43 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._43 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._43) / 2;
	mat._44 = 1.0f;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._44 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._44) / 2;

	return;
}

void CNkCharacter::Initial_Particle()
{
	int spray_particle_start_time[MAX_NKCHA_PARTICLE] = { 0, };

	for( int i = 0; i < MAX_NKCHA_PARTICLE; i++ )
	{
		m_NkCha_Particle[i].start_time = spray_particle_start_time[i];
		m_NkCha_Particle[i].life_time = 0;
		m_NkCha_Particle[i].cur_time = 0;
		m_NkCha_Particle[i].scale = 1.0f;
		m_NkCha_Particle[i].fade = 0.5f;

		m_NkCha_Particle[i].red = 1.0f;
		m_NkCha_Particle[i].green = 1.0f;
		m_NkCha_Particle[i].blue = 1.0f;

		m_NkCha_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		m_NkCha_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		m_NkCha_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		m_NkCha_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

		m_NkCha_Particle[i].texIndex = 0;
		m_NkCha_Particle[i].texFrame = 0;
	}
	return;
}

// 칼의 궤적을 만든다.
// Lyul_2005 수정
// 이도류 때문에 left right 를 두고 필요에 다라 좌우를 바꿔 생성한다.
// Default : left = true
void CNkCharacter::SetTraceLineLVertex(int linetype, D3DVECTOR *pTargetVector, bool left)  // left = true
{
	CCapsyong *LineCsy = NULL;

	// 이도류를 위해 좌우를 달리 설정한다.
	// nate 2005-06-10 : Burning Crash - 궤적
	if( linetype == TRACELINE_BURNINGCRASH
			&& m_Wearing[WEARING_CLOAK] )
		LineCsy = GET_SUB_CSY( m_Wearing[ WEARING_CLOAK ] );
	else if(!left && m_Wearing[WEARING_WEAPON])
		LineCsy = GET_SUB_CSY(m_Wearing[WEARING_WEAPON]);
	else if( m_Wearing[WEARING_WEAPON] )
		LineCsy = GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace);

	if( !m_Biped )
		return;

	if( !LineCsy )
		return;

	if( !LineCsy->m_LineVector0 )
		return;

	D3DMATRIX g_TempMat[2];
	D3DMATRIX *pLineMat, *pLineMat2;
	D3DVECTOR m_LineVector[2];

	float pattern_tu = 0.0f;
	DWORD color;
	int oldCurFrame = 0;
	int numVertex = 0;
	float tu, tv;
	float org_red, org_green, org_blue;
	float red, green, blue, alpha;
	int traceframe = 5;
	int stepframe = 0;
	int endframe = m_CurFrame - traceframe;
	int totalframe = 0;
	int frame = 0;
	D3DVECTOR dirVec;
	float scale = 0.0f;
	oldCurFrame = m_CurFrame;

	if( pTargetVector && (linetype == TRACELINE_SHOOTATTACK) )
	{
		pattern_tu = 3.0f;
		m_drawLineNum = 4;
		scale = 0.5f;
		oldCurFrame = g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;
		D3DVECTOR	vChaVec, vMobVec;

		if (m_Biped->m_bHalfFrame)
		{
			if (frame % 2)
			{
				GetHalfMat(g_TempMat[0], m_Biped, LineCsy->m_LineBiped[0], oldCurFrame / 2);
				pLineMat = &g_TempMat[0];
			}
			else
				pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[oldCurFrame / 2]);
		}
		else
			pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[oldCurFrame]);

		NKMath_VectorMatrixMultiply(m_LineVector[0],
									LineCsy->m_LineVector0[0], *pLineMat);

		D3DMATRIX	matRot;
		D3DVECTOR	vTmp;
		Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
		D3DMath_VectorMatrixMultiply(vTmp, m_LineVector[0], matRot);
		vTmp.x += m_wx;
		vTmp.y += m_wy;
		vTmp.z += m_wz;

		vChaVec = vTmp;
		vMobVec = D3DVECTOR(pTargetVector->x, pTargetVector->y, pTargetVector->z);

		D3DVECTOR	vDirVec;
		vDirVec = vMobVec - vChaVec;
		vDirVec = Normalize(vDirVec);

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		D3DVECTOR	vRight = CrossProduct( vViewVec, vDirVec );
		vRight = Normalize(vRight);
		vRight *= scale;

		D3DVECTOR	vDir;
		vDir = vMobVec - vChaVec;

		int curFrame = m_CurFrame - g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame;

		if( curFrame == 0 )
			vChaVec += (vDir*0.0f);
		else if( curFrame == 1 )
			vChaVec += (vDir*0.4f);
		else if( curFrame == 2 )
			vChaVec += (vDir*0.7f);

		red = 0.38f;
		green = 0.38f;
		blue = 1.0f;

		float tu[4];
		tu[0] = 0.0f + oldCurFrame * 0.35f;
		tu[1] = 0.0f + oldCurFrame * 0.35f;
		tu[2] = 1.0f + oldCurFrame * 0.35f;
		tu[3] = 1.0f + oldCurFrame * 0.35f;

		for( int i = 0; i < 4; i++ )
		{
			if( tu[i] > 1.0f )
				tu[i] = 1.0f - tu[i];
		}

		// 옆으로 향하는 텍스쳐로 빌보드된 사각형
		color = D3DRGBA(red * 0.15f, green * 0.15f, blue * 0.15f, 1.0f * 0.15f);
		m_TraceLine[0] = D3DLVERTEX( vChaVec - vRight, color, 0, tu[2], 0.0f );
		m_TraceLine[1] = D3DLVERTEX( vChaVec + vRight, color, 0, tu[3], 1.0f );
		color = D3DRGBA(red, green, blue, 1.0f);
		m_TraceLine[2] = D3DLVERTEX( vMobVec - vRight, color, 0, tu[0], 0.0f );
		m_TraceLine[3] = D3DLVERTEX( vMobVec + vRight, color, 0, tu[1], 1.0f );
	}
	else if( linetype == TRACELINE_ATTACK
			 || linetype == TRACELINE_JUMPATTACK
			 || linetype == TRACELINE_ROTATEATTACK
			 // nate 2005-06-10 : Burning Crash - 궤적
			 || linetype == TRACELINE_BURNINGCRASH )
	{
		pattern_tu = 1.0f;
		scale = 0.0f;

		if( endframe <= 0 )
		{
			endframe = 0;
			totalframe = m_CurFrame;
		}
		else
			totalframe = traceframe;

		m_drawLineNum = ( (m_CurFrame - endframe) * 4) + 4;

		// 양면
		for( int twofaces = 0; twofaces < 2; twofaces++ )
		{
			if( twofaces == 0 )
			{
				// 블레드와 일반 궤적의 Color
				if( m_bBleed
						// nate 2005-06-16 : Burning Crash - 궤적 색상 조절
						|| ( linetype == TRACELINE_BURNINGCRASH
							 && ( m_CurFrame >= 72 && m_CurFrame <= 84 )
							 || m_CurFrame >= 112 ) )
				{
					org_red = 1.0f;
					org_green = 1.0f;
					org_blue = 1.0f;
				}
				else if( m_Wearing[WEARING_WEAPON]
						 && ( m_Wearing[WEARING_WEAPON]->m_Vnum==1590 || m_Wearing[WEARING_WEAPON]->m_Vnum == 2694 )
					   )  ///일본디자인컨테스트 아이템 무라마사는 궤적이 빨간색으로 나오도록.
				{
					org_red = 1.0f;
					org_green = 0.0f;
					org_blue = 0.0f;
				}
				else
				{
					org_red = 0.38f;
					org_green = 0.38f;
					org_blue = 1.0f;
				}

				for( frame = m_CurFrame, stepframe = 0; frame >= endframe; frame--, stepframe++ )
				{
					if (m_Biped->m_bHalfFrame)
					{
						if (frame % 2)
						{
							GetHalfMat(g_TempMat[0], m_Biped, LineCsy->m_LineBiped[0], frame / 2);
							GetHalfMat(g_TempMat[1], m_Biped, LineCsy->m_LineBiped[1], frame / 2);

							pLineMat = &g_TempMat[0];
							pLineMat2 = &g_TempMat[1];
						}
						else
						{
							pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame / 2]);
							pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame / 2]);
						}
					}
					else
					{
						pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame]);
						pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame]);
					}

					NKMath_VectorMatrixMultiply(m_LineVector[0],
												LineCsy->m_LineVector0[0], *pLineMat);
					NKMath_VectorMatrixMultiply(m_LineVector[1],
												LineCsy->m_LineVector0[1], *pLineMat2);

					D3DMATRIX	matRot;
					Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
					D3DMath_VectorMatrixMultiply(m_LineVector[0], m_LineVector[0], matRot);
					m_LineVector[0].x += m_wx;
					m_LineVector[0].y += m_wy;
					m_LineVector[0].z += m_wz;
					D3DMath_VectorMatrixMultiply(m_LineVector[1], m_LineVector[1], matRot);
					m_LineVector[1].x += m_wx;
					m_LineVector[1].y += m_wy;
					m_LineVector[1].z += m_wz;

					dirVec = m_LineVector[0] - m_LineVector[1];
					dirVec = Normalize(dirVec);
					m_LineVector[0] = m_LineVector[0] + (dirVec * scale);
					m_LineVector[1] = m_LineVector[1] - (dirVec * scale);

					alpha = ((1.0f / totalframe) * stepframe);
					red = org_red - (org_red * alpha);
					green = org_green - (org_green * alpha);
					blue = org_blue - (org_blue * alpha);
					color = D3DRGBA(red * 0.7f, green * 0.7f, blue * 0.7f, (1.0f - alpha) * 0.7f);
					tu = ( (1.0f * pattern_tu) / traceframe) * stepframe;
					tv = 0.0f;
					m_TraceLine[numVertex++] = D3DLVERTEX( m_LineVector[0], color, 0, tu, tv );
					tu = ( (1.0f * pattern_tu) / traceframe) * stepframe;
					tv = 1.0f;
					m_TraceLine[numVertex++] = D3DLVERTEX( m_LineVector[1], color, 0, tu, tv );
				}
			}
			else
			{
				// 블레드와 일반 궤적의 Color
				if( m_bBleed
						// nate 2005-06-16 : Burning Crash - 궤적 색상 조절
						|| ( linetype == TRACELINE_BURNINGCRASH
							 && ( m_CurFrame >= 72 && m_CurFrame <= 84 )
							 || m_CurFrame >= 112 ) )
				{
					org_red = 1.0f;
					org_green = 1.0f;
					org_blue = 1.0f;
				}
				else if( m_Wearing[WEARING_WEAPON]
						 && ( m_Wearing[WEARING_WEAPON]->m_Vnum==1590 || m_Wearing[WEARING_WEAPON]->m_Vnum==2694 )
					   )  ///일본디자인컨테스트 아이템 무라마사는 궤적이 빨간색으로 나오도록.
				{
					org_red = 1.0f;
					org_green = 0.0f;
					org_blue = 0.0f;
				}
				else
				{
					org_red = 0.48f;
					org_green = 0.48f;
					org_blue = 1.0f;
				}

				for( frame = endframe, stepframe = totalframe; frame <= m_CurFrame; frame++, stepframe-- )
				{
					if (m_Biped->m_bHalfFrame)
					{
						if (frame % 2)
						{
							GetHalfMat(g_TempMat[0], m_Biped, LineCsy->m_LineBiped[0], frame / 2);
							GetHalfMat(g_TempMat[1], m_Biped, LineCsy->m_LineBiped[1], frame / 2);

							pLineMat = &g_TempMat[0];
							pLineMat2 = &g_TempMat[1];
						}
						else
						{
							pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame / 2]);
							pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame / 2]);
						}
					}
					else
					{
						pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame]);
						pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame]);
					}

					NKMath_VectorMatrixMultiply(m_LineVector[0],
												LineCsy->m_LineVector0[0], *pLineMat);
					NKMath_VectorMatrixMultiply(m_LineVector[1],
												LineCsy->m_LineVector0[1], *pLineMat2);

					D3DMATRIX	matRot;

					Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
					D3DMath_VectorMatrixMultiply(m_LineVector[0], m_LineVector[0], matRot);
					m_LineVector[0].x += m_wx;
					m_LineVector[0].y += m_wy;
					m_LineVector[0].z += m_wz;
					D3DMath_VectorMatrixMultiply(m_LineVector[1], m_LineVector[1], matRot);
					m_LineVector[1].x += m_wx;
					m_LineVector[1].y += m_wy;
					m_LineVector[1].z += m_wz;

					dirVec = m_LineVector[0] - m_LineVector[1];
					dirVec = Normalize(dirVec);
					m_LineVector[0] = m_LineVector[0] + (dirVec * scale);
					m_LineVector[1] = m_LineVector[1] - (dirVec * scale);

					alpha = ((1.0f / totalframe) * stepframe);
					red = org_red - (org_red * alpha);
					green = org_green - (org_green * alpha);
					blue = org_blue - (org_blue * alpha);
					color = D3DRGBA(red * 0.7f, green * 0.7f, blue * 0.7f, (1.0f - alpha) * 0.7f);

					tu = ( (1.0f * pattern_tu) / traceframe) * stepframe;
					tv = 0.0f;
					m_TraceLine[numVertex++] = D3DLVERTEX( m_LineVector[0], color, 0, tu, tv );
					tu = ( (1.0f * pattern_tu) / traceframe) * stepframe;
					tv = 1.0f;
					m_TraceLine[numVertex++] = D3DLVERTEX( m_LineVector[1], color, 0, tu, tv );
				}
			}
		}
	}
	else if( linetype == TRACELINE_DASHSLASH ) ///하이콤보
	{
		// 이 타입의 궤적은 매 프레임 생성이 아니라 누적시킨다.
		// 대쉬슬래쉬가 발동되었을때 m_drawLineNum을 0으로 초기화 시키고
		// 대쉬슬래쉬가 끝났을때는 m_DrawWeaponEffects가 0이 되므로 출력이 안되게 된다.(0이 아니라 다른 이펙트로 들어오면 다른대선 m_TraceLine이 처음부터 다시 써지게 되므로 상관없음)
		pattern_tu = RandomNum(0.0f,1.0f);
		if( linetype == TRACELINE_DASHSLASH )
			scale = 7.0f;
		else if( linetype == TRACELINE_SLASHSHADOW )
			scale = 17.0f;

		if( m_bBleed )
		{
			org_red = 0.65f;
			org_green = 0.65f;
			org_blue = 0.65f;
		}
		else
		{
			org_red = 0.1f;
			org_green = 0.1f;
			org_blue = 0.2f;
		}

		int MaxHalfNum = ((MAX_TRACELINE_FRAME * 4) + 4)/2; // 버택스 배열의 절반 크기. 우측 궤적은 이 절반 부분 부터 써짐.

		if( m_drawLineNum >= MaxHalfNum ) // 절반이상 그리려고 하면 막는다.(절반은 우측에서 사용해야 하니까.)
			return;

		frame = m_CurFrame;

		// 좌우 구분을 해서 처리가 갈라져야한다.
		if( LineCsy ) // 우선 좌측 검.( 위에서 기본으로 좌측검 캡숑이 선택되어있다. )
		{
			if( m_Biped->m_bHalfFrame )
			{
				if( frame % 2 )
				{
					GetHalfMat(g_TempMat[0], m_Biped, LineCsy->m_LineBiped[0], frame / 2);
					GetHalfMat(g_TempMat[1], m_Biped, LineCsy->m_LineBiped[1], frame / 2);

					pLineMat = &g_TempMat[0];
					pLineMat2 = &g_TempMat[1];
				}
				else
				{
					pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame / 2]);
					pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame / 2]);
				}
			}
			else
			{
				pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame]);
				pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame]);
			}

			NKMath_VectorMatrixMultiply(m_LineVector[0], LineCsy->m_LineVector0[0], *pLineMat);
			NKMath_VectorMatrixMultiply(m_LineVector[1], LineCsy->m_LineVector0[1], *pLineMat2);

			D3DMATRIX	matRot;
			Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
			D3DMath_VectorMatrixMultiply(m_LineVector[0], m_LineVector[0], matRot);
			m_LineVector[0].x += m_wx;
			m_LineVector[0].y += m_wy;
			m_LineVector[0].z += m_wz;
			D3DMath_VectorMatrixMultiply(m_LineVector[1], m_LineVector[1], matRot);
			m_LineVector[1].x += m_wx;
			m_LineVector[1].y += m_wy;
			m_LineVector[1].z += m_wz;

			dirVec = m_LineVector[0] - m_LineVector[1];
			dirVec = Normalize(dirVec);
			m_LineVector[0] = m_LineVector[0] + (dirVec * scale);
			m_LineVector[1] = m_LineVector[1] - (dirVec * scale);

			m_TraceLine[m_drawLineNum] = D3DLVERTEX( m_LineVector[0], 0xffffffff, 0, pattern_tu, 0.0f ); // 중간에다 기억.
			m_TraceLine[m_drawLineNum+1] = D3DLVERTEX( m_LineVector[1], 0xffffffff, 0, pattern_tu, 1.0f );
		}

		if( m_Wearing[WEARING_WEAPON] && GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) ) // 다음 우측 검.( 좌측검과는 틀리게 약각의 처리가 더 필요하다. )
		{
			LineCsy = GET_SUB_CSY(m_Wearing[WEARING_WEAPON]); // 캡숑을 우측껄로 바꿔주고...

			if( m_Biped->m_bHalfFrame )
			{
				if( frame % 2 )
				{
					GetHalfMat(g_TempMat[0], m_Biped, LineCsy->m_LineBiped[0], frame / 2);
					GetHalfMat(g_TempMat[1], m_Biped, LineCsy->m_LineBiped[1], frame / 2);

					pLineMat = &g_TempMat[0];
					pLineMat2 = &g_TempMat[1];
				}
				else
				{
					pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame / 2]);
					pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame / 2]);
				}
			}
			else
			{
				pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame]);
				pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame]);
			}

			NKMath_VectorMatrixMultiply(m_LineVector[0], LineCsy->m_LineVector0[0], *pLineMat);
			NKMath_VectorMatrixMultiply(m_LineVector[1], LineCsy->m_LineVector0[1], *pLineMat2);

			D3DMATRIX	matRot;
			Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
			D3DMath_VectorMatrixMultiply(m_LineVector[0], m_LineVector[0], matRot);
			m_LineVector[0].x += m_wx;
			m_LineVector[0].y += m_wy;
			m_LineVector[0].z += m_wz;
			D3DMath_VectorMatrixMultiply(m_LineVector[1], m_LineVector[1], matRot);
			m_LineVector[1].x += m_wx;
			m_LineVector[1].y += m_wy;
			m_LineVector[1].z += m_wz;

			dirVec = m_LineVector[0] - m_LineVector[1];
			dirVec = Normalize(dirVec);
			m_LineVector[0] = m_LineVector[0] + (dirVec * scale);
			m_LineVector[1] = m_LineVector[1] - (dirVec * scale);

			m_TraceLine[m_drawLineNum+MaxHalfNum] = D3DLVERTEX( m_LineVector[0], 0xffffffff, 0, pattern_tu, 0.0f ); // 중간에다 기억.
			m_TraceLine[m_drawLineNum+MaxHalfNum+1] = D3DLVERTEX( m_LineVector[1], 0xffffffff, 0, pattern_tu, 1.0f );
		}

		m_drawLineNum += 2; // 그려야 할 양을 누적. 2씩 증가함에 유의.

		// 칼라값과 텍스쳐 좌표를 조정해준다.
		for( int i = 0 ; i < m_drawLineNum; ++i )
		{
			red = org_red;
			green = org_green;
			blue = org_blue;

			alpha = (timeGetTime()-m_dwComboTick) * ((float)(m_drawLineNum-i)*0.025f) * 0.001f;
			if( alpha > 1.0f )
				alpha = 1.0f;

			alpha = 1.0f - alpha;

			color = D3DRGBA( red*alpha, green*alpha, blue*alpha, 1.0f );

			m_TraceLine[i].color = color;
			m_TraceLine[i+1].color = color;
			m_TraceLine[i+MaxHalfNum].color = color;
			m_TraceLine[i+MaxHalfNum+1].color = color;
		}
	}
	else if( linetype == TRACELINE_SLASHSHADOW ) ///하이콤보
	{
		// 이 타입의 궤적은 매 프레임 생성이 아니라 누적시킨다.
		// 대쉬슬래쉬가 발동되었을때 m_drawLineNum을 0으로 초기화 시키고
		// 대쉬슬래쉬가 끝났을때는 m_DrawWeaponEffects가 0이 되므로 출력이 안되게 된다.(0이 아니라 다른 이펙트로 들어오면 다른대선 m_TraceLine이 처음부터 다시 써지게 되므로 상관없음)
		//		pattern_tu = RandomNum(0.0f,1.0f);
		scale = 17.0f;

		if( m_bBleed )
		{
			org_red = 0.65f;
			org_green = 0.65f;
			org_blue = 0.65f;
		}
		else
		{
			org_red = 0.1f;
			org_green = 0.1f;
			org_blue = 0.2f;
		}

		totalframe = 48; // 하단 회전 모션이 끝나는 프레임.

		if( m_CurFrame < totalframe ) // 아직 회전중이면...
			totalframe = m_CurFrame; // 현재 프레임까지만...

		stepframe = 28;

		if( m_CurFrame-15 > stepframe ) // 아직 회전중이면...
			stepframe = m_CurFrame-15; // 현재 프레임까지만...


		for( frame = stepframe ; frame < totalframe ; ++frame )
		{
			if( m_Biped->m_bHalfFrame )
			{
				if( frame % 2 )
				{
					GetHalfMat(g_TempMat[0], m_Biped, LineCsy->m_LineBiped[0], frame / 2);
					GetHalfMat(g_TempMat[1], m_Biped, LineCsy->m_LineBiped[1], frame / 2);

					pLineMat = &g_TempMat[0];
					pLineMat2 = &g_TempMat[1];
				}
				else
				{
					pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame / 2]);
					pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame / 2]);
				}
			}
			else
			{
				pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[frame]);
				pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[frame]);
			}

			NKMath_VectorMatrixMultiply(m_LineVector[0], LineCsy->m_LineVector0[0], *pLineMat);
			NKMath_VectorMatrixMultiply(m_LineVector[1], LineCsy->m_LineVector0[1], *pLineMat2);

			D3DMATRIX	matRot;
			Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
			D3DMath_VectorMatrixMultiply(m_LineVector[0], m_LineVector[0], matRot);
			m_LineVector[0].x += m_wx;
			m_LineVector[0].y += m_wy;
			m_LineVector[0].z += m_wz;
			D3DMath_VectorMatrixMultiply(m_LineVector[1], m_LineVector[1], matRot);
			m_LineVector[1].x += m_wx;
			m_LineVector[1].y += m_wy;
			m_LineVector[1].z += m_wz;

			dirVec = m_LineVector[0] - m_LineVector[1];
			dirVec = Normalize(dirVec);
			m_LineVector[0] = m_LineVector[0] + (dirVec * scale);
			m_LineVector[1] = m_LineVector[1] - (dirVec * scale);

			pattern_tu = (float)frame*0.125f;

			color = D3DRGBA( org_red, org_green, org_blue, 1.0f );

			m_TraceLine[numVertex++] = D3DLVERTEX( m_LineVector[0], color, 0, pattern_tu, 0.0f ); // 중간에다 기억.
			m_TraceLine[numVertex].color = color;
			m_TraceLine[numVertex++] = D3DLVERTEX( m_LineVector[1], color, 0, pattern_tu, 1.0f );
		}

		if( m_CurFrame > 40 && m_dwComboTick )
		{
			pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, NULL, EFFECT_SKILL_SHADOWSLASH2 );
			m_dwComboTick = 0;
		}		

		m_drawLineNum = numVertex-1;

		if( m_drawLineNum < 0 )
		{
			m_drawLineNum = 0;
		}
	}
	else if ( linetype == TRACELINE_NORMAL )
	{
		if (m_Biped->m_bHalfFrame)
		{
			if (frame % 2)
			{
				GetHalfMat(g_TempMat[0], m_Biped, LineCsy->m_LineBiped[0], oldCurFrame / 2);
				GetHalfMat(g_TempMat[1], m_Biped, LineCsy->m_LineBiped[1], oldCurFrame / 2);

				pLineMat = &g_TempMat[0];
				pLineMat2 = &g_TempMat[1];
			}
			else
			{
				pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[oldCurFrame / 2]);
				pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[oldCurFrame / 2]);
			}
		}
		else
		{
			pLineMat = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[0]].m_Mats[oldCurFrame]);
			pLineMat2 = &(m_Biped->m_AniMat[LineCsy->m_LineBiped[1]].m_Mats[oldCurFrame]);
		}

		NKMath_VectorMatrixMultiply(m_LineVector[0],
									LineCsy->m_LineVector0[0], *pLineMat);
		NKMath_VectorMatrixMultiply(m_LineVector[1],
									LineCsy->m_LineVector0[1], *pLineMat2);

		D3DMATRIX	matRot;
		D3DVECTOR	vTmp;

		Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
		D3DMath_VectorMatrixMultiply(vTmp, m_LineVector[0], matRot);
		vTmp.x += m_wx;
		vTmp.y += m_wy;
		vTmp.z += m_wz;
		m_TraceLine[numVertex++] = D3DLVERTEX( vTmp, 0x00000000, 0, 0.0f, 0.0f );

		D3DMath_VectorMatrixMultiply(vTmp, m_LineVector[1], matRot);
		vTmp.x += m_wx;
		vTmp.y += m_wy;
		vTmp.z += m_wz;
		m_TraceLine[numVertex++] = D3DLVERTEX( vTmp, 0x00000000, 0, 0.0f, 0.0f );
	}

	return;
}

//-----------------------------------------------------------------
// 파티원정보를 받아온다. (모든 파티원들의 정보가 순서대로 한줄로 들어온다.)
//-----------------------------------------------------------------

void CNkCharacter::GetPvInfo(char* str)
{
	if(!str)
		return;

	m_iMaxPartysNum = 0;
	char id[80], vital[80];
	char pos_x[80],  pos_y[80];

	while(*str != 0x00)
	{
		str = AnyTwoArg(str, id, vital);

		m_iPartysListId[m_iMaxPartysNum] = atoi(id);
		m_fPartysVitalList[m_iMaxPartysNum] = atof(vital);

		str = AnyTwoArg(str, pos_x, pos_y);
		m_iPartysListXpos[m_iMaxPartysNum] = atoi(pos_x);
		m_iPartysListYpos[m_iMaxPartysNum] = atoi(pos_y);
		m_iMaxPartysNum++;
	}
	m_bRecivedPartysInfo = TRUE;
}
/*
void CNkCharacter::GetPvInfo(char* str)
{
	if(!str)
		return;

	char id[80], vital[80];
	char pos_x[80],  pos_y[80];

	str = AnyOneArg(str, id); // 임시로 파티원 맥스 카운트를 받자.
	m_iMaxPartysNum = atoi(id);

	//while(*str != 0x00)
	for( int i=0 ;i<m_iMaxPartysNum; i++)
	{
		str = AnyTwoArg(str, id, vital);

		m_iPartysListId[m_iMaxPartysNum] = atoi(id);
		m_fPartysVitalList[m_iMaxPartysNum] = atof(vital);

		str = AnyTwoArg(str, pos_x, pos_y);
		m_iPartysListXpos[m_iMaxPartysNum] = atoi(pos_x);
		m_iPartysListYpos[m_iMaxPartysNum] = atoi(pos_y);
	}
	m_bRecivedPartysInfo = TRUE;
}
*/


//=====================================================================
// nate 2004 - 4 : Crash_Miss 창 그리기
void CNkCharacter::DrawCrash_Miss( int nI, int nElapTime )
{
	if( nElapTime < 7 )
		nElapTime = 7;

	D3DMATRIX matPipeline;
	D3DVECTOR vSrc, vWorld;

	vWorld = D3DVECTOR( m_wx, m_wy + 23.0f, m_wz );

	if( pCMyApp->m_pUIMgr->m_pLand )
		pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipeline );

	D3DMath_VectorMatrixMultiply( vSrc, vWorld, matPipeline );

	if( vSrc.y < 0 )
		vSrc.y = 0;

	if( vSrc.x < 30 )
		vSrc.x = 30;

	if( vSrc.y >= g_pNk2DFrame->GetClientHeight() )
		vSrc.y = g_pNk2DFrame->GetClientHeight() - 20;

	if( vSrc.x >= g_pNk2DFrame->GetClientWidth() - 20 )
		vSrc.x = g_pNk2DFrame->GetClientWidth() - 20;

	//mungmae-2005/05/06 기존의 크래쉬 이펙트 대신 콤보 이펙트를 넣었다.
	//5~8프레임 때 성공하면 나오는 콤보 이펙트
	if( m_pCrash_MissPopup[ nI ] )
	{
		if( m_byShowCrash_Miss >= 5 && m_byShowCrash_Miss <= 8)
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_COMBO ] ), nElapTime );

		//9~12프레임 때 성공하면 나오는 콤보 이펙트
		else if( m_byShowCrash_Miss >= 9 && m_byShowCrash_Miss <= 12)
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_COMBO2 ] ), nElapTime );
		// Miss
		else if( m_byShowCrash_Miss == 2 )
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_MISS ] ), nElapTime );
		// Block ///페어리
		else if( m_byShowCrash_Miss == 3 )
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_BLOCK ] ), nElapTime );
		// nate 2005-05-09 : 이벤트포인트(EP)획득시 서버 메세지에 의해 나타남
		else if( m_byShowCrash_Miss == 20 )
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_EVENTPOINT ] ), nElapTime );
		else if( m_byShowCrash_Miss == 21 ) ///자유대련 승자 표시
			m_pCrash_MissPopup[ nI ]->Draw2( vSrc.x, vSrc.y,
											 g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FREEPVP_WINNER ] ), nElapTime );
		else if( m_byShowCrash_Miss == 22 ) ///자유대련 패자 표시
			m_pCrash_MissPopup[ nI ]->Draw2( vSrc.x, vSrc.y,
											 g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FREEPVP_LOSER ] ), nElapTime );
	}
}

void CNkCharacter::SetShowCrash_Miss( BYTE byCrash_Miss )
// Crash인지 Miss 인지 플레그 셋팅
{
	// Crash를 그리고 있는 상태인지 체크
	// 그리고 있지 않으면
	if( m_byShowCrash_Miss == 0 )
	{
		// Crash or Miss 플레그 쎗팅
		m_byShowCrash_Miss = byCrash_Miss;
		m_dwElapTime = timeGetTime();
	}
}


void CNkCharacter::RenderCrash_Miss()
{
	m_nElapTime = ( timeGetTime() - m_dwElapTime ) / 5;

	// 커지는 크기 조절
	if( m_byShowCrash_Miss == 21 || m_byShowCrash_Miss == 22 ) ///자유대련 승자,패자 표시는 시간을 더길게.
	{
		if( m_nElapTime > 500 )
		{
			m_byShowCrash_Miss = 0;
			m_dwElapTime = 0;
			return;
		}
	}
	else
	{
		if( m_nElapTime > 100 ) // 05-02-01 / 50에서 100으로 수치 늘림( 수정자:이원석 )
		{
			m_byShowCrash_Miss = 0;
			m_dwElapTime = 0;
			return;
		}
	}

	// 정지 크기 조정
	if( m_byShowCrash_Miss == 21 || m_byShowCrash_Miss == 22 ) ///자유대련 승자,패자 표시는 하나만 찍음.
	{
		DrawCrash_Miss( 0, m_nElapTime );
	}
	else
	{
		for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
			DrawCrash_Miss( i, m_nElapTime - 10 * i );
	}
}
//=====================================================================


//=====================================================================
// nate 2004 - 5
// CastleWAr
void CNkCharacter::RenderGiftMessage()
{
	if( !m_pGiftMessage )
		return;

	D3DMATRIX matPipeline;
	D3DVECTOR vSrc, vWorld;
	vWorld = D3DVECTOR( m_wx, m_wy + 23.0f, m_wz );

	if( pCMyApp->m_pUIMgr->m_pLand )
		pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipeline );

	D3DMath_VectorMatrixMultiply( vSrc, vWorld, matPipeline );

	if( vSrc.y < 0 )
		return;

	if( vSrc.x < 0 )
		return;

	if( vSrc.y >= g_pNk2DFrame->GetClientHeight() -20 )
		return;

	if( vSrc.x >= g_pNk2DFrame->GetClientWidth() - 20 )
		return;

	m_pGiftMessage->Draw( vSrc.x, vSrc.y,
						  g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_GIFT_MESSAGE ] ), 1 );
}

void CNkCharacter::SetBattlePoint(int BattlePoint)
{
	m_nBattlePoint = BattlePoint;

	if (m_pScoreWnd)
		m_pScoreWnd->SetBP();
}

void CNkCharacter::SetLaghaimPoint(int LaghaimPoint )
{
	m_nLaghaimPoint = LaghaimPoint;

	if (m_pScoreWnd)
		m_pScoreWnd->SetLP();
}

void CNkCharacter::SetGHPoint(int GHPoint)
{
	m_nGHPoint = GHPoint;

	if (m_pScoreWnd)
		m_pScoreWnd->SetGHP();
}

// nate 2005-06-17 : Burning Crash - 스킬의 스텝( 동작 ) 구분
void CNkCharacter::SetSkillStep( int nSkillStep )
{
	if( m_Race == RACE_BULKAN )
	{
		if( nSkillStep == 1 )
			m_CurFrame = 0;
		else if( nSkillStep == 2 )
			m_CurFrame = 26;
		else if( nSkillStep == 3 )
			m_CurFrame = 45;
		else if( nSkillStep == 4 )
			m_CurFrame = 72;
		else if( nSkillStep == 5 )
			m_CurFrame = 85;
		else if( nSkillStep == 6 )
			m_CurFrame = 112;
		else
			SetMotionState( CHA_STAND );
	}
	else
		m_nSkillStep = nSkillStep;	// nate 2005-06-27 : Auto Mine - 휴먼(마인갯수)
}

void CNkCharacter::SetFailStep( int nFailStep )
{
	m_nFailStep = nFailStep;
}

// nate 2005-06-18 : 스킬 사용중인지 체크
BOOL CNkCharacter::IsCantMove()
{
	// 주의사항 : 스킬 동작중에 움직일수 없는 스킬-만- 추가할것
	// CHA_HELLCRY_ATTACK 이후에는 일정한 순서값을 가지고 있음
	if(m_nMotionState == CHA_ATTACK
			|| m_nMotionState == CHA_MAGIC
			|| m_nMotionState == CHA_SOCIAL
			|| m_nMotionState == CHA_JUMP_ATTACK
			|| m_nMotionState == CHA_ROTATE_ATTACK
			|| m_nMotionState == CHA_KAI_TOUCH
			|| m_nMotionState == CHA_AIDIA_PIPE
			|| m_nMotionState == CHA_HUMAN_DEEP
			|| m_nMotionState == CHA_HUMAN_SHORT
			|| m_nMotionState == CHA_THROW_SLAYER
			|| m_nMotionState == CHA_THROW_CHAKRAM
			|| m_nMotionState == CHA_HANDLE
			|| m_nMotionState == CHA_AIDIA_SUB
			|| m_nMotionState == CHA_KAI_SUB
			|| m_nMotionState == CHA_MAGIC_MOVE
			|| m_nMotionState > CHA_HELLCRY_ATTACK )
		return TRUE;
	return FALSE;
}

int	CNkCharacter::BurningCrashEndFrame( int nComboStep )
{
	int nEndFrame = 0;
	switch( nComboStep )
	{
	case 0:
		nEndFrame = 1;
		break;
	case 1:
		nEndFrame = 25;
		break;
	case 2:
		nEndFrame = 44;
		break;
	case 3:
		nEndFrame = 71;
		break;
	case 4:
		nEndFrame = 84;
		break;
	case 5:
		nEndFrame = 111;
		break;
	case 6:
		nEndFrame = 130;
		break;
	}
	return nEndFrame;
}

//mungmae-2005/06/23 뫼비우스 팀 순위(리젠 시간을 위해)
int CNkCharacter::MatrixTeamGrade(int nTeamindex)
{
	int nIndex = nTeamindex - 1;
	int temp[ 3 ] = {0,};
	int nGrade = 2;

	if( pCMyApp->GetCurWorld() == 19 ) //WORLD_MATRIX_2009 )
	{
		if( nTeamindex == 1 )
		{
			temp[ 0 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ nIndex ];
			temp[ 1 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 1 ];
		}

		else if( nTeamindex == 2 )
		{
			temp[ 0 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ nIndex ];
			temp[ 1 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 0 ];
		}

		nGrade = 2;
		if( temp[0] >= temp[1] )
			nGrade --;
	}
	else
	{
		if( nTeamindex == 1 )
		{
			temp[ 0 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ nIndex ];
			temp[ 1 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 1 ];
			temp[ 2 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 2 ];
		}

		else if( nTeamindex == 2 )
		{
			temp[ 0 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ nIndex ];
			temp[ 1 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 0 ];
			temp[ 2 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 2 ];
		}

		else if( nTeamindex == 3 )
		{
			temp[ 0 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ nIndex ];
			temp[ 1 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 0 ];
			temp[ 2 ] = pCMyApp->m_pUIMgr->m_nGuildPoint[ 1 ];
		}

		nGrade = 3;

		if( temp[ 0 ] > temp[ 1 ] )
			nGrade--;

		if( temp[ 0 ] > temp[ 2 ] )
			nGrade--;
	}


	return nGrade;

}

void CNkCharacter::HellDownFail( CNkCharacter* pCha, CNkMob* pMob, BOOL bPacket )
{
	if( !pCha && !pMob )
		return;

	{
		pCMyApp->m_pUIMgr->m_bRClickDown = 0;
		pCMyApp->m_pUIMgr->m_byCheckHellDown = 1;
		pCMyApp->m_bHellDownReady = FALSE;
		pCMyApp->DrawCasting = FALSE;
		pCMyApp->m_dPressTime = 0L;
		m_nSkillStep = 0;
		m_nFailStep = 0;

		SetMotionState( CHA_STAND );
		if( g_pRoh == this )
			G_SK_SetCooltime( SKILL_C_HELLDOWN );
	}

	if( pCha )
		m_AttackChaId = -1;
	else
		m_AttackMobId = -1;

	if( bPacket )
	{
		char commOut[512] = "";
		if( pCha && (this == g_pRoh))
		{	
			SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Char, pCha->m_nCharIndex, -1, 0);
		}
		else if( pMob && (this == g_pRoh))
		{	
			SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Npc, pMob->m_nVirId, -1, 0);
		}
		else
		{
			return;
		}		
	}

	if (g_pDSound)
	{
		if( g_pRoh->m_Sex == 0)
			g_pDSound->Stop(g_EffectSound[EFF_SND_HELLDOWN_CAST_M]);
		else
			g_pDSound->Stop(g_EffectSound[EFF_SND_HELLDOWN_CAST_W]);
	}
}

void CNkCharacter::MoveSkill(float tx, float tz, int nSkillIndex)
{
	// 타겟지점 저정
	m_tx = tx;
	m_tz = tz;
	float dist_x = m_tx - m_wx;
	float dist_z = m_tz - m_wz;

	// 캐릭의 방향을 구한다.
	if( nSkillIndex != SKILL_KNOCKBACK && nSkillIndex != SKILL_KNOCKBACK2 ) // 넉백은 방향 전환 안하게... ///페어리
	{
		m_toDir = (float) atan2(dist_z, dist_x);
	}

	float scalar= sqrt(pow(dist_x,2) + pow(dist_z,2));
	float sc_rate = 0.0f;

	float* fFrame = NULL;
	if( nSkillIndex == SKILL_B_FLYINGCRASH )
	{
		sc_rate = scalar / 13.0f;
		fFrame = &m_fFlying_frame;
	}
	else if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // 넉백 ///페어리
	{
		sc_rate = scalar / 13.0f;
		fFrame = &m_fDash_Frame;
	}
	else if( nSkillIndex == SKILLF_A_DASH )
	{
		sc_rate = scalar / 13.0f;
		fFrame = &m_fDash_Frame;
	}
	else if( nSkillIndex == SKILL_F_DASHSLASH )  ///하이콤보
	{
		sc_rate = scalar / 20.0f; // 대쉬보다 좀더 빠르게.
		fFrame = &m_fDash_Frame;
	}
	else if( nSkillIndex == SKILL_F_SHADOWSLASH )
	{
		sc_rate = scalar / 130.0f;
		fFrame = &m_fShadow_frame;
	}
	else
		return;

	m_dx = dist_x / sc_rate;    // 이동할 크기를 구한다
	m_dz = dist_z / sc_rate;	// 이동할 크기를 구한다

	float dist = dist_x != 0.0f ? dist_x : dist_z;
	float distMod = m_dx != 0.0f ? m_dx : m_dz;

	if( nSkillIndex == SKILL_B_FLYINGCRASH )
		*fFrame = dist / distMod - 2.0f; // 이동할 횟수를 구한다 ( 정확한 보정을 위해 float)
	else if( nSkillIndex == SKILL_F_SHADOWSLASH )
		*fFrame = dist / distMod - 0.2f;
	else if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // 넉백 ///페어리
		*fFrame = dist / distMod - 0.2f;
	else if( nSkillIndex == SKILLF_A_DASH )
		*fFrame = dist / distMod;
	else if( nSkillIndex == SKILLF_A_DASH || nSkillIndex == SKILL_F_DASHSLASH ) ///하이콤보
		*fFrame = dist / distMod;

	if( nSkillIndex != SKILLF_A_DASH && nSkillIndex != SKILL_KNOCKBACK && nSkillIndex != SKILL_KNOCKBACK2  ) ///페어리
	{
		m_bAttack = TRUE;		// 공격 스킬 여부 체크
		m_CurAttackType = nSkillIndex;
	}

	if( g_pRoh == this )
		pCMyApp->StopHero( FALSE ); // 인자 FALSE 추가. 인자를 넣지 않으면 stop패킷을 한번 더 보내고 이때문에 다른 사람 화면에서 워프하듯이 보이는 버그가 있었음.(06-03-18 원석)

	// 동작 구분
	if( nSkillIndex == SKILL_B_FLYINGCRASH )		// 클라잉 크래쉬
		SetMotionState(CHA_B_FLYINGCRASH);
	else if( nSkillIndex == SKILL_F_SHADOWSLASH )	// 쉐도우 슬레쉬
		SetMotionState(CHA_F_SHADOWSLASH);
	else if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // 넉백 ///페어리
		SetMotionState( CHA_KNOCKBACK ); //넉백은 선채로 밀리게...
	else if( nSkillIndex == SKILLF_A_DASH )			// 대쉬
		SetMotionState(CHA_F_DASH);
	else if( nSkillIndex == SKILL_F_DASHSLASH ) ///하이콤보
	{
		SetMotionState( CHA_F_DASHSLASH );
		SetShowCrash_Miss( 5 ); // 콤보성공 이펙트(약) 출력.

		this->m_drawLineNum = 0; // 이펙트용 궤적 초기화.
		this->m_dwComboTick = timeGetTime(); // 콤보스킬 체크용 틱을 설정.

		// 위쪽것은 다른 유저껏도 해줘야하고 타겟정하는건 자기자신일때만해야한다.
		if( g_pRoh == this ) // 자기자신이면...
			pCMyApp->AttackMsgSend( SKILL_F_DASHSLASH ); // MvSkill을 받은 후 타겟을 정해서 서버로 보내준다.(그래야 라인 동기화가 맞는다.)
	}

	m_vMvSkillStartPos = D3DVECTOR( m_wx, m_wy, m_wz ); // 무브 스킬이 발동되는 시점의 위치. ///하이콤보
}

EffectSort* CNkCharacter::FindMyFairy( int Type ) //자기 자신의 페어리 이펙트(구조체)를 찾아주는 함수 ///페어리 // 07-01-30 타입 인자를 추가하여 응용
{
	set<int>::iterator it = m_listEffect.begin();
	set<int>::iterator itEnd = m_listEffect.end();
	int idx;

	while (it != itEnd)
	{
		idx = *it;

		EffectSort *pEffectSort = pCMyApp->m_pFxSet->GetEffect(idx);
		if( pEffectSort == NULL )
		{
			it++;
			continue;
		}

		if( pEffectSort->nNum == idx ) // 자기 자신이 소유한 이펙트 중 하나라면..
		{
			if( Type == SKILL_FAIRY
					|| ( Type == SKILL_FAIRY_HUMAN_SILVER )
					|| ( Type == SKILL_FAIRY_HUMAN_BLACK	 )
					|| ( Type == SKILL_FAIRY_KAI_BLUE	 )
					|| ( Type == SKILL_FAIRY_KAI_RED		 )
					|| ( Type == SKILL_FAIRY_HYB_SILVER	 )
					|| ( Type == SKILL_FAIRY_HYB_GOLD	 )
					|| ( Type == SKILL_FAIRY_AID_BLUE	 )
					|| ( Type == SKILL_FAIRY_AID_GREEN	 )
					|| ( Type == SKILL_FAIRY_BUL_SILVER	 )
					|| ( Type == SKILL_FAIRY_BUL_VIOLET )
					|| ( Type == SKILL_FAIRY_NECK1 )
					|| ( Type == SKILL_FAIRY_NECK2 )
					|| ( Type == SKILL_FAIRY_NECK3 )
					|| ( Type == SKILL_FAIRY_NECK4 )
					|| ( Type == SKILL_FAIRY_NECK5 )
			  ) // 페어리 이펙트 찾기
			{
				// 페어리인지 체크
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_FAIRY ) // 페어리면...(페어리가 둘 이상이면 난감하다.)
				{
					return pEffectSort; // 페어리면 그냥 리턴.
				}
			}
			else if( Type >= SKILL_LUCKYPIG1 && Type <= SKILL_LUCKYPIG4 ) // 복돼지 찾기
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_LUCKYPIG && pEffectSort->subNum == Type ) // 복돼지 중에서 종류까지 맞는지 체크해서
				{
					return pEffectSort; // 원하던 복돼지로 리턴.
				}
			}
			else if( Type >= SKILL_BABYSOUL_1 && Type <= SKILL_BABYSOUL_3 ) // [7/31/2007 Theodoric] 6주년 기념 이벤트 베이비 소울
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_BABYSOUL && pEffectSort->subNum == Type ) // 종류까지 맞는지 체크해서
				{
					return pEffectSort;
				}
			}
			else if( Type == SKILL_VAMPIREBAT ) ///뱀파이어망토 박쥐
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_VAMPIREBAT )
				{
					return pEffectSort;
				}
			}
			else if( Type == SKILL_PETRUDOLPH             ///펫루돌프_빨강   // [12/10/2007 parkmj] 미니루돌프 4종 추가(검정, 파랑, 녹색, 노랑)
					 ||	Type == SKILL_PETRUDOLPH_BLACK    ///펫루돌프_검정
					 ||	Type == SKILL_PETRUDOLPH_BLUE     ///펫루돌프_파랑
					 ||	Type == SKILL_PETRUDOLPH_GREEN    ///펫루돌프_녹색
					 ||	Type == SKILL_PETRUDOLPH_YELLOW
					 || Type == SKILL_PETRUDOLPH_KOR_PREMIUM) ///펫루돌프_노랑
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_RUDOLPH )
				{
					return pEffectSort;
				}
			}
			// [12/4/2007 parkmj] 일본 링아이템 이펙트
			else if( ( Type == SKILL_RING_LIGHT_WHITE )
					 || ( Type == SKILL_RING_LIGHT_DARK )
					 || ( Type == SKILL_RING_LIGHT_RED )
					 || ( Type == SKILL_RING_LIGHT_BLUE )
					 || ( Type == SKILL_RING_LIGHT_GREEN )
					 || ( Type == SKILL_RING_LIGHT_GOLD )
					 || ( Type == SKILL_ROYALE_1 )
					 || ( Type == SKILL_ROYALE_2 )
					 || ( Type == SKILL_ROYALE_3 )
					 || ( Type == SKILL_ROYALE_4 )
					 || ( Type == SKILL_ROYALE_5 )
					 || ( Type == SKILL_ROYALE_6 )
					 || ( Type == SKILL_ROYALE_7 )
					 || ( Type == SKILL_ROYALE_8 )
					 || ( Type == SKILL_ROYALE_9 )
					 || ( Type == SKILL_ROYALE_10 )
					 || ( Type == SKILL_ROYALE_11 )
					 || ( Type == SKILL_ROYALE_12 )
					 || ( Type == SKILL_ROYALE_13 )
					 || ( Type == SKILL_ROYALE_14 )
					 || ( Type == SKILL_ROYALE_15 )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_RING_LIGHT )
				{
					return pEffectSort;
				}
			}
			else if( ( Type == SKILL_ARUPAKA_PET )
					 || ( Type == SKILL_ARUPAKA_PET_BE )
					 || ( Type == SKILL_ARUPAKA_PET_BR )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_ARUPAKAPET )
				{
					return pEffectSort;
				}
			}
			else if( ( Type == SKILL_PET_CATS_BLACK )
					 || ( Type == SKILL_PET_CATS_WHITE )
					 || ( Type == SKILL_PET_CATS_MIX )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_CATS )
				{
					return pEffectSort;
				}
			}

			else if( ( Type == SKILL_PET_ZORO_BLACK )
					 || ( Type == SKILL_PET_ZORO_WHITE )
					 || ( Type == SKILL_PET_ZORO_GREEN )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_ZORO )
				{
					return pEffectSort;
				}
			}

			else if( ( Type == SKILL_PET_VALO_BLACK )
					 || ( Type == SKILL_PET_VALO_WHITE )
					 || ( Type == SKILL_PET_VALO_GREEN )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_VALO )
				{
					return pEffectSort;
				}
			}
			else if( ( Type == SKILL_PET_BEAR_BLACK )
					 || ( Type == SKILL_PET_BEAR_WHITE )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_BEAR )
				{
					return pEffectSort;
				}
			}
			else if( Type == SKILL_PET_HORSE_BLUE || Type == SKILL_PET_HORSE_RED )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_HORSE )
				{
					return pEffectSort;
				}
			}
			else if( Type == SKILL_PET_JEGUAR_1 || Type == SKILL_PET_JEGUAR_2 || Type == SKILL_PET_JEGUAR_3 )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_JEGUAR )
				{
					return pEffectSort;
				}
			}
			else if( Type == SKILL_PET_BONI_1 || Type == SKILL_PET_BONI_2 || Type == SKILL_PET_BONI_3 )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_BONI )
				{
					return pEffectSort;
				}
			}   //PABLO TEST PET ##########################################################################
			else if( Type ==SKILL_PET_AZUL )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_AZUL )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_VERDE )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_VERDE  )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_ROXO )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_ROXO )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_AMARELO )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_AMARELO )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_VERMELHO )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_VERMELHO )
				{
					return pEffectSort;
				}
			}
			else if( ( Type == SKILL_PET_FESHIK )
					 || ( Type == SKILL_PET_FESHIK_P )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_FESHIK)
				{
					return pEffectSort;
				}
			}
			else if( ( Type == SKILL_PET_CINZA )
					 || ( Type == SKILL_PET_ALTAIR_P )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_CINZA )
				{
					return pEffectSort;
				}
			}
			else if( ( Type == SKILL_PET_EZULAN )
					 || ( Type == SKILL_PET_EZULAN_P )
				   )
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_EZULAN)
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_THUNDERBIRD )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_THUNDERBIRD )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_THUNDERBIRD_P )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_THUNDERBIRD_P )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_PETFAME1 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETFAME1 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_PETFAME2 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETFAME2 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_PETFAME3 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETFAME3 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_PETFAME4 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETFAME4 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_HELPER )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETHELPER )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_HALL1 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETHALL1 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_HALL2 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETHALL2 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_HALL3 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETHALL3 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_NATAL1 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETNATAL1 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_NATAL2 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETNATAL2 )
				{
					return pEffectSort;
				}
			}
			else if( Type ==SKILL_PET_NATAL3 )
			{
			if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_PETNATAL3 )
				{
					return pEffectSort;
				}
			}
		}
		it++;
	}

	return NULL;
}

EffectSort* CNkCharacter::FindMySpirit() //자기 자신의 페어리 이펙트(구조체)를 찾아주는 함수 ///페어리
{
	set<int>::iterator it = m_listEffect.begin();
	set<int>::iterator itEnd = m_listEffect.end();
	int idx;

	while (it != itEnd)
	{
		idx = *it;

		EffectSort *pEffectSort = pCMyApp->m_pFxSet->GetEffect(idx);
		if( pEffectSort == NULL )
		{
			it++;
			continue;
		}

		if( pEffectSort->nNum == idx ) // 자기 자신이 소유한 이펙트 중 하나라면..
		{
			// 페어리인지 체크
			if( pEffectSort->nType == FX_SPIRITMODE2 && pEffectSort->Index == 0 ) // 페어리면...(페어리가 둘 이상이면 난감하다.)
			{
				return pEffectSort; // 페어리면 그냥 리턴.
			}
		}
		it++;
	}

	return NULL;
}

BOOL CNkCharacter::IsDrawPicking() ///자유대련
{
	if( g_pRoh )
	{
		if( g_pRoh->m_bNowFreePVP ) // 자기가 자유대련 중의 경우
		{
			if( timeGetTime()-g_pRoh->m_dwRingCreateTick < 5000 ) // 카운트 중엔 누구도 못때리게..
				return FALSE;
			else if( this->m_nCharIndex != g_pRoh->m_dwPvpDestIndex ) // 상대방이 아닌 경우
				return FALSE;
		}
		else // 자유대련 중이 아닌경우
		{
			if( this->m_bNowFreePVP ) // 자유대련 중인 녀석을 못때리게.
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CNkCharacter::EventWearingRender( int where ) // 특정 위치의 이벤트 복장을 그린다. ///이벤트복장착용
{
	if( !g_bUseEventWear  ) // 이벤트 복장 착용 시스템을 지원 안하는 나라는 작동안하게...
		return FALSE;

	if( !m_bEventWearFlag ) //g_dwLangType == LANG_KOR )
		return FALSE;

	//	if( this != g_pRoh ) // 자기 자신만 이벤트 복장을 이걸로 그린다.(다른 이는 서버에서 처리해준다.)
	//		return FALSE; // 이렇게 하면 원래 복장을 출력해준다.

	//  [6/19/2009 ppmmjj83] 일본 수영모자 투영되는 문제로 하드코딩
	if( m_EventWearing[where] )
	{
		if( m_EventWearing[where]->m_Vnum >= 2320 && m_EventWearing[where]->m_Vnum <= 2323 ) return FALSE;
	}

	if( where == WEARING_SHOES ) // 슈즈는 바지 쪽 속성을 체크해줘야한다.(슈즈 착용체크보다 먼저해야함에 유의) ///070723 일본버그리포트 수정 때 수정
	{
		if( m_EventWearing[WEARING_PANTS] )
		{
			if( (GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // 이경우 슈즈를 그리지 않는다.
			{
				m_bEventWearRendered[where] = true;
				return TRUE; // 원래 복장도 출력 안한다.
			}
		}
		else if( m_Wearing[WEARING_PANTS] )
		{
			if( (GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // 이경우 슈즈를 그리지 않는다.
			{
				m_bEventWearRendered[where] = true;
				return TRUE; // 원래 복장도 출력 안한다.
			}
		}
	}

	CItem* wearItem = m_EventWearing[where];

	// 우선은 해당 위치에 이벤트 복장을 착용하고 있는지 체크한다.
	if( !wearItem )
		return FALSE; // 이렇게 하면 원래 복장을 출력해준다.

	m_bEventWearRendered[where] = true;

	if( IsAlphaWear(where, true) )
	{
		return TRUE;
	}

	// 일부 이벤트 복장 예외처리.
	if( !m_Chaotic_Grade // 카오일경우는 예외
			&& ( IS_LIGHT_OFF(m_EventWearing[where]) ) // [6/19/2008 반재승] 의상 아이템들의 그림자 끄기 여부 체크
	  )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	if( dcbMgr->IsStart() == true )
	{
		int team = dcbMgr->GetTeam(m_nCharIndex);
		if( team != -1 )
		{
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		}
	}

	if( GET_CSY(m_EventWearing[where], m_BpdRace) )
		GET_CSY(m_EventWearing[where], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(wearItem,m_BpdRace), FALSE ); // 이벤트 복장을 출력한다.

	if( !m_Chaotic_Grade // 카오일경우는 예외
			&& ( IS_LIGHT_OFF(m_EventWearing[where]) ) // [6/19/2008 반재승] 의상 아이템들의 그림자 끄기 여부 체크
	  )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );	

	if( wearItem->m_Vnum == 1588 ) ///일본디자인컨테스트 헤일로의 방패라면..
	{
		// 이펙트를 여기서 예외 처리 해준다.
		DrawInstanceEffect(  1588, this, GET_CSY(wearItem, m_BpdRace) );
	}
	else if( m_EventWearing[where]->m_Vnum == 1589 ) ///일본디자인컨테스트 등롱
	{
		// 이펙트를 여기서 예외 처리 해준다.
		DrawInstanceEffect(  1589, this, GET_CSY(wearItem, m_BpdRace) );
	}
	
	return TRUE;
}

BOOL CNkCharacter::EventWearingFrameMove( int where ) // 특정 위치의 이벤트 복장을 그린다. ///이벤트복장착용
{
	if( !g_bUseEventWear )
		return FALSE;

	if( !m_bEventWearFlag )
		return FALSE;

	//	if( this != g_pRoh ) // 자기 자신만 이벤트 복장을 이걸로 그린다.(다른 이는 서버에서 처리해준다.)
	//		return FALSE; // 이렇게 하면 원래 복장을 출력해준다.

	// 우선은 해당 위치에 이벤트 복장을 착용하고 있는지 체크한다.
	if( !m_EventWearing[where] )
		return FALSE; // 이렇게 하면 원래 복장을 출력해준다.

	// [2/20/2008 반재승] 이벤트 인벤토리에 링 슬롯 추가
	if( where != WEARING_RING )
	{
		if( GET_CSY(m_EventWearing[where], m_BpdRace) )
			GET_CSY(m_EventWearing[where], m_BpdRace)->Animate(m_Biped, m_CurFrame);
	}

	return TRUE;
}

void DrawInstanceEffect( int ItemIndex, CNkCharacter *pNkChar, CCapsyong *pTargetCsy/*CItem *TargetItem*/ ) // 이펙트 리스트에 들어가서 관리되는게 아니라 바로 출력해주는 이펙트 효과.(아이템용.) ///일본디자인컨테스트 아이템 효과를 넣기 위해 준비됐다.
{
	if( !pNkChar )
		return;

	if( !pTargetCsy )
		return;

	DWORD srcBlend, destBlend;
	DWORD alphaBlend, alphaTest, alphaRef;
	DWORD dwTextureState[3];
	DWORD lightingOld, s_dwZWriteEnable;
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHAREF, &alphaRef );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlend );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &lightingOld);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &s_dwZWriteEnable);

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );


	LPDIRECTDRAWSURFACE7 lpTexture;
	D3DMATRIX	matWorld, matRotate, matTrans;
	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_SetIdentityMatrix(matRotate);
	D3DUtil_SetIdentityMatrix(matTrans);
	D3DVECTOR lineCenter;
	DWORD color;

	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DVECTOR vViewAt = pCMyApp->GetEyeAtPos();
	D3DVECTOR vViewFrom;
	pCMyApp->GetEyePos( vViewFrom );
	float fViewDist = Magnitude( vViewFrom-vViewAt );

	if( ItemIndex == 1588 || ItemIndex == 3262 ) // 헤일로의 방패
	{
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTALPHA );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		float rectwidth = 5.0f; // vSortPos를 사용안하고 있는것 같으니 그 x값을 크기값으로 사용한다.
		rectwidth += (rand()%10)*0.07f;
		// 라인의 중간값을 구한다.

		lineCenter.x = (pTargetCsy->m_LineVector[0].x + pTargetCsy->m_LineVector[1].x) / 2;
		lineCenter.y = (pTargetCsy->m_LineVector[0].y + pTargetCsy->m_LineVector[1].y) / 2;
		lineCenter.z = (pTargetCsy->m_LineVector[0].z + pTargetCsy->m_LineVector[1].z) / 2;

		CRectPlane MainObject;    // 빌보드로 쓸 수도 있는 클래스.
		color = 0x33ffffff;
		MainObject.Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)
		MainObject.ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), color, 0, 0.0f, 1.0f )  );
		MainObject.ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), color, 0, 0.0f, 0.0f )  );
		MainObject.ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), color, 0, 1.0f, 1.0f)  );
		MainObject.ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), color, 0, 1.0f, 0.0f )  );

		D3DVECTOR vLineDir = pTargetCsy->m_LineVector[1] - pTargetCsy->m_LineVector[0];
		MainObject.SetBillBoard( vViewVec, TRUE, 0.0f );

		D3DVECTOR vTmp = D3DVECTOR( lineCenter.x, lineCenter.y, lineCenter.z );
		vTmp.x *= 1.3f;
		vTmp.z *= 1.3f;
		Nk3DUtil_SetRotateCCWYMatrix(matRotate, pNkChar->m_dir);
		D3DMath_VectorMatrixMultiply(vTmp, vTmp, matRotate);

		MainObject.SetTranslationXYZ( pNkChar->m_matWorld._41+vTmp.x, pNkChar->m_matWorld._42+vTmp.y, pNkChar->m_matWorld._43+vTmp.z );

		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[EFF_FX_LIGHTFOG] );
		GET_D3DDEVICE()->SetTexture( 0, lpTexture );
		MainObject.Render();
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &pNkChar->m_matWorld ); // 이걸 돌려줘야한다.
	}
	if( ItemIndex == 1589 || ItemIndex == 3263 ) // 등롱
	{
		// 등롱 부분.
		lineCenter.x = (pTargetCsy->m_LineVector[0].x + pTargetCsy->m_LineVector[1].x) / 2;
		lineCenter.y = (pTargetCsy->m_LineVector[0].y + pTargetCsy->m_LineVector[1].y) / 2;
		lineCenter.z = (pTargetCsy->m_LineVector[0].z + pTargetCsy->m_LineVector[1].z) / 2;

		D3DVECTOR vTmp = D3DVECTOR( lineCenter.x, lineCenter.y, lineCenter.z );
		Nk3DUtil_SetRotateCCWYMatrix(matRotate, pNkChar->m_dir);
		D3DMath_VectorMatrixMultiply(vTmp, vTmp, matRotate);

		vTmp.x += pNkChar->m_matWorld._41;
		vTmp.y += pNkChar->m_matWorld._42;
		vTmp.z += pNkChar->m_matWorld._43;

		D3DUtil_SetTranslateMatrix( matWorld, vTmp );

		Nk3DUtil_SetRotateCCWYMatrix( matRotate, pNkChar->m_dir );
		D3DMath_MatrixMultiply( matWorld, matRotate, matWorld );

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		g_pChouchin->Render( GET_D3DDEVICE(), 0, FALSE, FALSE, FALSE, NULL, NULL, FALSE, TRUE, 0xffffffff );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &pNkChar->m_matWorld ); // 이걸 돌려줘야한다.


		// 등속 불빛.
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTALPHA );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		float rectwidth = 5.0f; // vSortPos를 사용안하고 있는것 같으니 그 x값을 크기값으로 사용한다.
		rectwidth += (rand()%10)*0.07f;
		/*
		// 라인의 중간값을 구한다.
		lineCenter.x = (pTargetCsy->m_LineVector[0].x + pTargetCsy->m_LineVector[1].x) / 2;
		lineCenter.y = (pTargetCsy->m_LineVector[0].y + pTargetCsy->m_LineVector[1].y) / 2;
		lineCenter.z = (pTargetCsy->m_LineVector[0].z + pTargetCsy->m_LineVector[1].z) / 2;
		*/
		CRectPlane MainObject;    // 빌보드로 쓸 수도 있는 클래스.
		color = 0x55ffff00;
		MainObject.Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)
		MainObject.ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), color, 0, 0.0f, 1.0f )  );
		MainObject.ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), color, 0, 0.0f, 0.0f )  );
		MainObject.ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), color, 0, 1.0f, 1.0f)  );
		MainObject.ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), color, 0, 1.0f, 0.0f )  );

		D3DVECTOR vLineDir = pTargetCsy->m_LineVector[1] - pTargetCsy->m_LineVector[0];
		MainObject.SetBillBoard( vViewVec, TRUE, 0.0f );

		vTmp.y -= 3.7f; // y만 내려준 위치.

		MainObject.SetTranslationXYZ( vTmp.x, vTmp.y, vTmp.z ); // 이녀석은 이미 캐릭터 위치가 포함된 녀석이다.

		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[EFF_FX_LIGHTFOG] );
		GET_D3DDEVICE()->SetTexture( 0, lpTexture );
		MainObject.Render();
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &pNkChar->m_matWorld ); // 이걸 돌려줘야한다.
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, s_dwZWriteEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAREF   , alphaRef);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, lightingOld);
}

bool CNkCharacter::IsWeared(CItem* pItem)
{
	for(int i = 0; i < WEARING_NUM; i++)
	{
		if( m_Wearing[i] == NULL )
			continue;

		if( m_Wearing[i] == pItem )
			return true;
	}

	return false;
}

bool CNkCharacter::IsEventWeared(CItem* pItem)
{
	for(int i = 0; i < WEARING_NUM; i++)
	{
		if( m_EventWearing[i] == NULL )
			continue;

		if( m_EventWearing[i] == pItem )
			return true;
	}

	return false;	
}

BOOL CNkCharacter::GetSetItemEffectString(char * str, bool bEventWear)
{
	int setType = bEventWear == true ? m_SetItemEventWear : m_SetItem;
	switch( setType )
	{
	case LV300_BULKAN:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +600, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_STAMINA));
		strcat(str," +100");
		return TRUE;
	case LV300_KAILIPTON:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +100, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +600");
		return TRUE;
	case LV300_AIDIA:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +200, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +500");
		return TRUE;
	case LV300_HUMAN:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +200, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_EPOWER));
		strcat(str," +500");
		return TRUE;
	case LV300_HYBRIDER:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +300, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +150, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_STAMINA));
		strcat(str," +150");
		return TRUE;
	case RARE_BULKAN:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +1200, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_STAMINA));
		strcat(str," +200");
		return TRUE;
	case RARE_KAILIPTON:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +200, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +1200");
		return TRUE;
	case RARE_AIDIA:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +400, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +1000");
		return TRUE;
	case RARE_HUMAN:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +400, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_EPOWER));
		strcat(str," +1000");
		return TRUE;
	case RARE_HYBRIDER:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +800, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +300, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_STAMINA));
		strcat(str," +300");
		return TRUE;
	/*case HEROSET:
		sprintf(str, "%s +1500", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;*/	
	case T_RARE_BULKAN:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +1200, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_STAMINA));
		strcat(str," +200");
		return TRUE;
	case T_RARE_KAILIPTON:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +200, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +1200");
		return TRUE;
	case T_RARE_AIDIA:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +400, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +1000");
		return TRUE;
	case T_RARE_HUMAN:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +400, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_EPOWER));
		strcat(str," +1000");
		return TRUE;
	case T_RARE_HYBRIDER:
		strcpy(str,(char*)G_STRING(IDS_BOTTOM_VATAL));
		strcat(str," +800, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_MANA));
		strcat(str," +300, ");
		strcat(str,(char*)G_STRING(IDS_BOTTOM_STAMINA));
		strcat(str," +300");
		return TRUE;
	/*case GM_HEROSET:
		sprintf(str, "%s +15000", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;*/

	case RARE_BULKAN1:
		sprintf(str, "%s +1500", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;
	case RARE_KAILIPTON1:
		sprintf(str, "%s +1500", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;
	case RARE_PEROM1:
		sprintf(str, "%s +1500", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;
	case RARE_AIDIA1:
		sprintf(str, "%s +1500", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;
	case RARE_HUMAN1:
		sprintf(str, "%s +1500", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;
	case RARE_HYBRIDER1:
		sprintf(str, "%s +1500", (char*)G_STRING(IDS_ALL_STATE));
		return TRUE;

	case SETITEM_EVENT1:
		{
			strcpy(str,(char*)G_STRING(IDS_MAX_VITAL));
			strcat(str," +500, ");
			strcat(str,(char*)G_STRING(IDS_MAX_MANA));
			strcat(str," +500, ");
			strcat(str,(char*)G_STRING(IDS_MAX_STAMINA));
			strcat(str," +500, ");
			strcat(str,(char*)G_STRING(IDS_MAX_EPOWER));
			strcat(str," +500");
		}
		return TRUE;
	case SETITEM_EVENT2:
		{
			strcpy(str,(char*)G_STRING(IDS_MAX_VITAL));
			strcat(str," +800");
		}
		return TRUE;
	case SETITEM_EVENT3:
		{
			strcpy(str,(char*)G_STRING(IDS_MAX_VITAL));
			strcat(str," +1300");
		}
		return TRUE;
	}
	return FALSE;
}

SETITEM CNkCharacter::IsSetItem(int vNum)
{	
	switch( vNum )
	{	
	case 1564:
	case 1565:
	case 1566:
	case 1567:
		return LV300_BULKAN;	// 크리거 셋
	case 1576:
	case 1577:
	case 1578:
	case 1579:
		return LV300_KAILIPTON;	// 블레이즈
	case 1560:
	case 1561:
	case 1562:
	case 1563:
		return LV300_AIDIA;		// 이시스
	case 1572:
	case 1573:
	case 1574:
	case 1575:
		return LV300_HUMAN;		// 뱅퀴쉬
	case 1568:
	case 1569:
	case 1570:
	case 1571:
		return LV300_HYBRIDER;	// 플레타		
	case 1797:
	case 1798:
	case 1799:
	case 1800:
		return RARE_BULKAN;		// 인페르니움
	case 1805:
	case 1806:
	case 1807:
	case 1808:
		return RARE_KAILIPTON;	// 커쥬어
	case 1801:
	case 1802:
	case 1803:
	case 1804:
		return RARE_AIDIA;		// 프레어리
	case 1813:
	case 1814:
	case 1815:
	case 1816:
		return RARE_HUMAN;		// 아카트
	case 1809:
	case 1810:
	case 1811:
	case 1812:
		return RARE_HYBRIDER;	// 드리아드		
	case 1817:
	case 1818:
	case 1819:
	case 1820:
		return HEROSET;			// 영웅 방어	
	case 2180:
	case 2181:
	case 2182:
	case 2183:
		return T_RARE_BULKAN;		// 인페르니움
	case 2188:
	case 2189:
	case 2190:
	case 2191:
		return T_RARE_KAILIPTON;	// 커쥬어
	case 2184:
	case 2185:
	case 2186:
	case 2187:
		return T_RARE_AIDIA;		// 프레어리
	case 2196:
	case 2197:
	case 2198:
	case 2199:
		return T_RARE_HUMAN;		// 아카트
	case 2192:
	case 2193:
	case 2194:
	case 2195:
		return T_RARE_HYBRIDER;	// 드리아드
		// 영웅 방어구 // GM용
	case 2200:
	case 2201:
	case 2202:
	case 2203:
		return GM_HEROSET;			// 영웅 방어	

	case 2295:
	case 2296:
	case 2297:
	case 2298:
		return RARE_HYBRIDER1;

	case 2300:
	case 2301:
	case 2302:
	case 2303:
		return RARE_HUMAN1;

	case 2305:
	case 2306:
	case 2307:
	case 2308:
		return RARE_BULKAN1;

	case 2309:
	case 2310:
	case 2311:
	case 2312:
		return RARE_AIDIA1;

	case 2313:
	case 2314:
	case 2315:
	case 2316:
		return RARE_KAILIPTON1;

	case 3227:
	case 3228:
	case 3229:
	case 3240:
		return RARE_PEROM1;
	}
	
	return SETITEM_NONE;
}

void CNkCharacter::SetSetItem()
{
	int nSetItem[SETITEM_MAX] = {0,};
	int nSetItemCount[SETITEM_MAX] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
	m_SetItem = SETITEM_NONE;		

	for(int i= 0; i<WEARING_NUM; i++ )
	{
		if( m_Wearing[i] == NULL )
			continue;

		SETITEM setItem = IsSetItem(m_Wearing[i]->m_Vnum);

		if( setItem != SETITEM_NONE)
		{
			nSetItem[setItem]++;
			if( nSetItem[setItem] == nSetItemCount[setItem] )
			{
				SetSetItem( setItem );
				break;
			}
		}
	}
}

void CNkCharacter::SetSetItemEventWear()
{
	int nSetItem[SETITEM_MAX] = {0,};
	int nSetItemCount[SETITEM_MAX] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
	m_SetItemEventWear = SETITEM_NONE;

	for(int i= 0; i<WEARING_NUM; i++ )
	{
		if( m_EventWearing[i] == NULL )
			continue;

		SETITEM setItem = IsSetItemEventWear(m_EventWearing[i]->m_Vnum);

		if( setItem != SETITEM_NONE)
		{
			nSetItem[setItem]++;
			if( nSetItem[setItem] == nSetItemCount[setItem] )
			{
				SetSetItemEventWear( setItem );
				break;
			}
		}
	}
}



SETITEM	CNkCharacter::IsSetItemEventWear(int vNum)
{
	switch( vNum )
	{	
	case 1564:
	case 1565:
	case 1566:
	case 1567:
		return LV300_BULKAN;	// 크리거 셋
	case 1576:
	case 1577:
	case 1578:
	case 1579:
		return LV300_KAILIPTON;	// 블레이즈
	case 1560:
	case 1561:
	case 1562:
	case 1563:
		return LV300_AIDIA;		// 이시스
	case 1572:
	case 1573:
	case 1574:
	case 1575:
		return LV300_HUMAN;		// 뱅퀴쉬
	case 1568:
	case 1569:
	case 1570:
	case 1571:
		return LV300_HYBRIDER;	// 플레타		
	case 1797:
	case 1798:
	case 1799:
	case 1800:
		return RARE_BULKAN;		// 인페르니움
	case 1805:
	case 1806:
	case 1807:
	case 1808:
		return RARE_KAILIPTON;	// 커쥬어
	case 1801:
	case 1802:
	case 1803:
	case 1804:
		return RARE_AIDIA;		// 프레어리
	case 1813:
	case 1814:
	case 1815:
	case 1816:
		return RARE_HUMAN;		// 아카트
	case 1809:
	case 1810:
	case 1811:
	case 1812:
		return RARE_HYBRIDER;	// 드리아드		
	case 1817:
	case 1818:
	case 1819:
	case 1820:
		return HEROSET;			// 영웅 방어	
	case 2180:
	case 2181:
	case 2182:
	case 2183:
		return T_RARE_BULKAN;		// 인페르니움
	case 2188:
	case 2189:
	case 2190:
	case 2191:
		return T_RARE_KAILIPTON;	// 커쥬어
	case 2184:
	case 2185:
	case 2186:
	case 2187:
		return T_RARE_AIDIA;		// 프레어리
	case 2196:
	case 2197:
	case 2198:
	case 2199:
		return T_RARE_HUMAN;		// 아카트
	case 2192:
	case 2193:
	case 2194:
	case 2195:
		return T_RARE_HYBRIDER;	// 드리아드
		// 영웅 방어구 // GM용
	case 2200:
	case 2201:
	case 2202:
	case 2203:
		return GM_HEROSET;			// 영웅 방어	

	case 2295:
	case 2296:
	case 2297:
	case 2298:
		return RARE_HYBRIDER1;

	case 2300:
	case 2301:
	case 2302:
	case 2303:
		return RARE_HUMAN1;

	case 2305:
	case 2306:
	case 2307:
	case 2308:
		return RARE_BULKAN1;

	case 2309:
	case 2310:
	case 2311:
	case 2312:
		return RARE_AIDIA1;

	case 2313:
	case 2314:
	case 2315:
	case 2316:
		return RARE_KAILIPTON1;

	case 3227:
	case 3228:
	case 3229:
	case 3240:
		return RARE_PEROM1;
	}
	
	return SETITEM_NONE;
}

void CNkCharacter::OnBossRaidHeroEffect()
{
	SetSetItem();
	SetSetItemEventWear();

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
	{
		g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE;
	}
	
	//Event Wear
	if (GetSetItemEventWear() == HEROSET || GetSetItemEventWear() == GM_HEROSET)
	{
		if (!m_bEventWearFlag)//Not Displaying it, try remove the effect
		{	
			if( GetBossRaidHeroEffectIndex() == -1 )
				return;

			pCMyApp->m_pFxSet->DeleteEffect( GetBossRaidHeroEffectIndex() );
			DeleteEffectNum( GetBossRaidHeroEffectIndex() );

			SetBossRaidHeroEffectIndex( -1 );
			return;
		}
		//Displaying, show effect
		if( GetBossRaidHeroEffectIndex() != -1 )
			return;

		EffectSort ef;

		ZeroMemory( &ef, sizeof(EffectSort));

		ef.nType		= FX_HERO_EFFECT;
		ef.num			= 0;
		ef.nCurFrame	= 0;
		ef.pNkChaTo		= this;

		int index = pCMyApp->m_pFxSet->InsertFxEffect(ef);
		InsertEffectNum( index );
		SetBossRaidHeroEffectIndex( index );
		return;
	}
	else//No Event Equip
	{
		if( GetBossRaidHeroEffectIndex() == -1 )
			return;

		pCMyApp->m_pFxSet->DeleteEffect( GetBossRaidHeroEffectIndex() );
		DeleteEffectNum( GetBossRaidHeroEffectIndex() );

		SetBossRaidHeroEffectIndex( -1 );
	}
	//Normal Armour slots Equipped
	if(GetSetItem() == HEROSET || GetSetItem() == GM_HEROSET)
	{
		if( GetBossRaidHeroEffectIndex() != -1 )
			return;

		EffectSort ef;

		ZeroMemory( &ef, sizeof(EffectSort));

		ef.nType		= FX_HERO_EFFECT;
		ef.num			= 0;
		ef.nCurFrame	= 0;
		ef.pNkChaTo		= this;

		int index = pCMyApp->m_pFxSet->InsertFxEffect(ef);
		InsertEffectNum( index );
		SetBossRaidHeroEffectIndex( index );
	}
	//Normal Armour slots not Equipped
	else
	{
		if( GetBossRaidHeroEffectIndex() == -1 )
			return;

		pCMyApp->m_pFxSet->DeleteEffect( GetBossRaidHeroEffectIndex() );
		DeleteEffectNum( GetBossRaidHeroEffectIndex() );

		SetBossRaidHeroEffectIndex( -1 );
	}	
}




BOOL CNkCharacter::IsPressMan()
{
	if(m_EventWearing[3] != NULL && m_EventWearing[3]->m_Vnum == 2048 ) // 이벤트 복장에 기자단 깃발 착용 했다면
		return TRUE;

	if(m_Wearing[3] != NULL && m_Wearing[3]->m_Vnum == 2048 )
		return TRUE;

	return FALSE;
}

//  [8/16/2008 반재승]
void CNkCharacter::SetRunState( BOOL state )
{
	m_bRun = state;
}

BOOL CNkCharacter::IsWearing()
{
	for(int i=0; i<WEARING_NUM; i++)
	{
		if( m_Wearing[i] || m_EventWearing[i] )
			return TRUE;
	}

	return FALSE;
}



BOOL CNkCharacter::FindExgSlotBlankSlot(int &x, int &y, int w, int h)
{
	BOOL success = FALSE;

	for (int x_idx = 0; !success && x_idx+w-1 < EXG_SLOT_X_NUM; x_idx++)
	{
		for (int y_idx = 0; y_idx+h-1 < EXG_SLOT_Y_NUM; y_idx++)
		{
			BOOL found = TRUE;
			for (int x_w = x_idx; found && x_w < x_idx + w; x_w++)
			{
				for (int y_h = y_idx; y_h < y_idx + h; y_h++)
				{
					if (m_ExgSlot[x_w][y_h] >= 0)
					{
						found = FALSE;
						break;
					}
				}
			}

			if (found)
			{
				x = x_idx;
				y = y_idx;
				success = TRUE;
				break;
			}
		}
	}
	return success;
}

void CNkCharacter::InitExpTable()
{
	if( this != g_pRoh )
		return;
}


UINT64 CNkCharacter::GetTotalExp()
{
	if( this != g_pRoh )
		return 0;

	m_Exp;
	m_NeedExp;

	return 0;
}


UINT64 CNkCharacter::GetNeedExp()
{
	if( this != g_pRoh )
		return 0;

	m_Exp;
	m_NeedExp;

	return 0;
}

UINT64 CNkCharacter::GetLevelExp(int level)
{
	if( this != g_pRoh )
		return 0;

	m_Exp;
	m_NeedExp;

	return 0;
}

bool CNkCharacter::RemoveEquipItem(int index)
{
	for( int i = 0 ; i < WEARING_NUM ; i++ )
	{
		if( m_Wearing[i] && m_Wearing[i]->m_Index == index )
		{
			if( i == 3 )
			{
				if( m_Wearing[i]->m_Vnum == 1774 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
				{
					if( !(m_EventWearing[i] && m_EventWearing[i]->m_Vnum == 1774) ) // 이벤트 망토를 안 끼고있는 경우만...
						pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
				}
				if( m_Wearing[i]->m_Vnum == 11273 ) // 기존 뱀파이어 망토를 입고있었는지 여부.
				{
					if( !(m_EventWearing[i] && m_EventWearing[i]->m_Vnum == 11273) ) // 이벤트 망토를 안 끼고있는 경우만...
						pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // 박쥐 이펙트를 빼준다.
				}
			}

			SAFE_DELETE(m_Wearing[i]);
			if (i == WEARING_WEAPON)
				ResetMotionState();

			OnBossRaidHeroEffect();
			return true;
		}
	}

	return false;
}

bool CNkCharacter::RemoveEventItem(int index)
{
	if( false == g_bUseEventWear )
		return false;

	for ( int i = 0; i < WEARING_NUM; i++ )
	{
		if( m_EventWearing[i] && m_EventWearing[i]->m_Index == index )
		{
			if( i == 3 )
			{
				if( m_EventWearing[i]->m_Vnum == 1774 )
				{
					if( !(m_Wearing[i] && m_Wearing[i]->m_Vnum == 1774) )
						pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT );
				}
				if( m_EventWearing[i]->m_Vnum == 11273 )
				{
					if( !(m_Wearing[i] && m_Wearing[i]->m_Vnum == 11273) )
						pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT );
				}
			}

			SAFE_DELETE( m_EventWearing[i] );
			if (i == WEARING_WEAPON)
				ResetMotionState();

			OnBossRaidHeroEffect();
			return true;
		}
	}

	return false;
}

bool CNkCharacter::RemoveInvenItem(int index)
{
	CItem *pItem = NULL;
	CItem *pPrevItem = NULL;

	for( int i = 0; i < PACK_NUM; i++ )
	{
		pItem = m_Inven[i];
		pPrevItem = NULL;

		while( pItem )
		{
			if( pItem->m_Index == index )
			{
				if( pPrevItem )
					pPrevItem->m_Next = pItem->m_Next;
				else
					m_Inven[i] = pItem->m_Next;

				for( int x = pItem->m_SlotX ; x < pItem->m_SlotX+GET_ITEM_WIDTH(pItem) ; ++x )
					for( int y = pItem->m_SlotY ; y < pItem->m_SlotY+GET_ITEM_HEIGHT(pItem) ; ++ y )
						m_Slot[i][x][y] = -1;

				SAFE_DELETE( pItem );
				return true;
			}

			pPrevItem = pItem;
			pItem = pPrevItem->m_Next;
		}
	}

	return false;
}

bool CNkCharacter::RemoveExchageItem(int index)
{
	CItem *pItem = m_ExgInven;
	CItem *pPrevItem = NULL;

	while (pItem)
	{
		if( pItem->m_Index == index )
		{
			if( pPrevItem )
				pPrevItem->m_Next = pItem->m_Next;
			else
				m_ExgInven = pItem->m_Next;

			for( int x = pItem->m_SlotX ; x < pItem->m_SlotX+GET_ITEM_WIDTH(pItem) ; ++x )
				for( int y = pItem->m_SlotY ; y < pItem->m_SlotY+GET_ITEM_HEIGHT(pItem) ; ++ y ) // 아이템마다의 범위를 검사.
					m_ExgSlot[x][y] = -1;

			SAFE_DELETE( pItem );
			return true;
		}

		pPrevItem = pItem;
		pItem = pPrevItem->m_Next;
	}

	return false;
}

bool CNkCharacter::RemoveExtraItem(int index)
{
	if( m_ExtraSlot && m_ExtraSlot->m_Index == index )
	{
		SAFE_DELETE( m_ExtraSlot );
		return true;
	}

	return false;
}

bool CNkCharacter::RemoveQuickItem(int index)
{
	int slot = -1;
	for ( int i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++ )
	{
		if( m_QuickSlot[i] && m_QuickSlot[i]->m_Index == index )
		{
			slot = i;

			CItem* pItem = NULL;
			CItem* pItem2 = NULL;
			int pack_num = -1;
			for (int p = 0; p < PACK_NUM; p++)
			{
				pItem = PickItemInPack(p, m_QuickSlot[i]->m_Vnum , TRUE);
				if (pItem)
				{
					pack_num = p;
					break;
				}
			}

			if (!pItem && m_QuickSlot[i]->m_Vnum != 175
					&& m_QuickSlot[i]->m_Vnum != 2222 && m_QuickSlot[i]->m_Vnum != 2223 && m_QuickSlot[i]->m_Vnum != 2224
			   )
			{
				for (int p = 0; p < PACK_NUM; p++)
				{
					pItem = PickPowerInPack(p, GET_TYPE(m_QuickSlot[i]), GET_SHAPE(m_QuickSlot[i]),
											GET_APPTO(m_QuickSlot[i]));
					if (pItem)
					{
						pack_num = p;
						break;
					}
				}
				if (!pItem && m_QuickSlot[i]->m_AppTo == 5)
				{
					for (int p = 0; p < PACK_NUM; p++)
					{
						pItem = PickPowerInPack(p, GET_TYPE(m_QuickSlot[i]), GET_SHAPE(m_QuickSlot[i]), 0);
						if (pItem)
						{
							pack_num = p;
							break;
						}
					}
				}
			}

			SAFE_DELETE( m_QuickSlot[i] );

			if( pItem )
			{
				pCMyApp->m_pUIMgr->Send_MoveToItem(slot , pack_num , pItem->m_Index);
			}

			return true;
		}
	}

	return false;
}

bool CNkCharacter::RemoveQuickItem(int slot, int index, bool moveto ,bool similar)
{
	if( slot < 0 )
		return false;

	if( m_QuickSlot[slot] && m_QuickSlot[slot]->m_Index != index )
		return false;

	if( moveto == true && CheckMoveToQuick(m_QuickSlot[slot]->m_Vnum) )
	{
		int pack_num = -1;
		CItem* pItem = NULL;

		for (int p = 0; p < PACK_NUM; p++)
		{
			pItem = GetInvenItem(p, m_QuickSlot[slot]->m_Vnum , TRUE);
			if (pItem)
			{
				pack_num = p;
				break;
			}
		}

		if( similar )
		{
			if( !pItem )
			{
				for (int p = 0; p < PACK_NUM; p++)
				{
					pItem = GetPowerInPack(p, GET_TYPE(m_QuickSlot[slot]), GET_SHAPE(m_QuickSlot[slot]),
										   GET_APPTO(m_QuickSlot[slot]));
					if (pItem)
					{
						pack_num = p;
						break;
					}
				}
			}

			if (!pItem && m_QuickSlot[slot]->m_AppTo == 5)
			{
				for (int p = 0; p < PACK_NUM; p++)
				{
					pItem = GetPowerInPack(p, GET_TYPE(m_QuickSlot[slot]), GET_SHAPE(m_QuickSlot[slot]), 0);
					if (pItem)
					{
						pack_num = p;
						break;
					}
				}
			}
		}

		if( pItem )
		{
			if( pCMyApp->m_pUIMgr->Send_MoveToItem(slot , pack_num , pItem->m_Index) )
			{
			}
		}
		else
		{
			SAFE_DELETE( m_QuickSlot[slot] );
		}
	}
	else
	{
		SAFE_DELETE( m_QuickSlot[slot] );
	}

	return true;
}

bool CNkCharacter::RemoveInvenItem(int pack, int index)
{
	CItem* pItem = PickItemInPack(pack , index , FALSE);

	if( pItem )
		SAFE_DELETE(pItem);

	return true;
}

bool CNkCharacter::InvenToQuick(int slot, int index)
{
	if( slot < 0 )
		return false;

	SAFE_DELETE( m_QuickSlot[slot] );

	CItem * pInvenItem = NULL;
	for( int p=0; p<PACK_NUM ; p++ )
	{
		pInvenItem = PickItemInPack(p , index , FALSE);
		if( pInvenItem )
			break;
	}

	if( !pInvenItem )
		return false;

	ToQuickSlot( slot , index , pInvenItem->m_Vnum , pInvenItem->m_PlusNum , pInvenItem->m_Special , pInvenItem->m_Special2 );
	SAFE_DELETE(pInvenItem);
	return true;
}


CItem * CNkCharacter::GetInvenItem(int pack , int index, bool vnum)
{
	CItem *pItem = m_Inven[ pack ];
	while(pItem)
	{
		if( vnum )
		{
			if( pItem->m_Vnum == index )
				return pItem;
		}
		else
		{
			if( pItem->m_Index == index )
				return pItem;
		}

		pItem = pItem->m_Next;
	}

	return NULL;
}

CItem * CNkCharacter::GetPowerInPack(int pack_num, int type_idx, int shape_idx, int app_to)
{
	CItem *pItem = m_Inven[pack_num];
	while (pItem)
	{
		if (GET_TYPE(pItem) == type_idx && GET_SHAPE(pItem) == shape_idx &&	GET_APPTO(pItem) == app_to)
		{
			return pItem;
		}

		pItem = pItem->m_Next;
	}

	return NULL;
}

bool CNkCharacter::CheckMoveToQuick(int vnum)
{
	return true;
}

bool CNkCharacter::UpdateQuickItem(int plus_num, int special_num, int slot_num)
{
	// 차크람,북
	if( !m_QuickSlot[slot_num] )
		return false;

	m_QuickSlot[slot_num]->m_PlusNum = plus_num;
	m_QuickSlot[slot_num]->m_Special = special_num;

	g_pNk2DFrame->GetControlInven()->ResetItemInfo();

	return true;
}

bool CNkCharacter::UpdateEquipItem(int plus_num, int special_num, int equip_where)
{
	if( equip_where < 0 || !m_Wearing[equip_where] )
		return false;

	m_Wearing[equip_where]->m_PlusNum = plus_num;
	m_Wearing[equip_where]->m_Special = special_num;

	g_pNk2DFrame->GetControlInven()->ResetItemInfo();

	return true;
}

bool CNkCharacter::IsEquiped()
{
	for( int i=0 ; i<WEARING_NUM ; i++ )
	{
		if( m_Wearing[i] )
			return true;
		if( m_EventWearing[i] )
			return true;
	}

	return false;
}

bool CNkCharacter::Check_TwoHandWeapon(CItem * pItem)
{
	if( GET_RENDER_FLAG(pItem) > 0
			&& ( !m_Wearing[WEARING_SHIELD]
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==875
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==876
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1588 ///일본디자인컨테스트 아이템 예외처리.
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1589 ///일본디자인컨테스트 아이템 예외처리.
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1027
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1028
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1037
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1038
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1127
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1288
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==2219
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==2220
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==2221
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==3648
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==3654
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==3650
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==3656
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10190
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10196
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10192
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10198
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10586
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10588
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10592
			     || m_Wearing[WEARING_SHIELD]->m_Vnum==10594
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10598
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10600
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10712
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10852
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==11235
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==11274
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==4634
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10713
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10714
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==10716
				 || GET_SHAPE(pItem) == IWEAPON_DUAL_SWORD
				 || GET_SHAPE(pItem) == IWEAPON_HAMMER )
	  )
	{
		return true;
	}


	return false;
}

void CNkCharacter::scaleCharacter(float ratio)
{
	m_fScaleSize = max(ratio, 0.0f);			// 1.0f 이하는 1.0f로
}

void CNkCharacter::SetSummonNpcIndex(const int& index)
{
	m_indexSummonNpc = index;
}

const int& CNkCharacter::GetSummonNpcIndex()
{
	return m_indexSummonNpc;
}

void CNkCharacter::SetTargetPos(float tx, float ty, float tz)
{
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
}
