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

// nate 2004 - 3 : �븸 ���� ���� ��ġ
extern DWORD g_dwClientCountry;
// ���� �ε��� ( ���� �ý��� )
extern int	g_nServerIndex = 0;

// nate 2004 - 8 : �Ź��� ���� - ����Ʈ ����������
extern int g_nMutantTextureNum[];
// nate 2004 - 7 �۷ι� ShadowRect���

extern int g_nEmptyTextureNum;




extern BOOL g_bGlobalMatrix; ///���������� �� Ŭ���̾�Ʈ ���� ����.
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
extern	CSurface *g_pMatrixKor; ///���������� ���� ���� �� �̹���
extern	CSurface *g_pMatrixJpn; ///���������� ���� ���� �� �̹���
extern	CSurface *g_pMatrixBra;
extern	CSurface *g_pMatrixGer;
extern	CSurface *g_pMatrixChi;
extern	CSurface *g_pMatrixInd;
extern	CSurface *g_pMatrixTha;
extern	CSurface *g_pNvNWin; ///�������¸� ��ũ
// nate : �븸 �Ŵ� ����
extern	CSurface *g_pMedal;
extern int g_AirshipProto[];
extern int g_AirshipProto_VNUM[];
extern int g_BasicWingsProto;
extern int g_BasicWingsProto_VNUM;
extern int g_MatrixSvr;
extern BOOL	g_bNewTerrain;
extern CLolos	*g_pDisplayStandLolo[3];  // ���λ��� ��ǰ������ ������Ʈ �ε�� ///���λ���
extern CLolos	*g_pChouchin; ///�Ϻ����������׽�Ʈ ��� ������ ����Ʈ��.

CLolos	CNkCharacter::m_CubeLolo;
DWORD	CNkCharacter::m_dwAmbient;
CLolos	*g_FlyStones[];
float	g_ChaFlyHeight[CHA_FLY_FRAME];
int		g_RohFlyLevel = 0;

BOOL CheckCanNpcUpgradeItem( CItem *pItem ); // ��ȭ�� ������ ������������ üũ��. ///�Ź���ȭ�ý���

void DrawInstanceEffect( int ItemIndex, CNkCharacter *pNkChar, CCapsyong *pTargetCsy/*CItem *TargetItem*/ ); // ����Ʈ ����Ʈ�� ���� �����Ǵ°� �ƴ϶� �ٷ� ������ִ� ����Ʈ ȿ��.(�����ۿ�.) ///�Ϻ����������׽�Ʈ ������ ȿ���� �ֱ� ���� �غ�ƴ�. // �ӽ��׽�Ʈ��

extern BOOL g_bUseEventWear; ///�̺�Ʈ��������
extern bool g_bShowIndex;


#define TEST_GUILDNAME_2010


// ���� �ʱ�ȭ
void PlayerInitialLoading()
{
	g_pCapsyongTexture->SetTexturePath("data/");
	// �ȹ��ۿ� �������� ĳ���� ������ü.
	PFILE *fpLolo = NULL;
	fpLolo = pfopen("data/cube.lol", "rb");
	assert(fpLolo != NULL);
	CNkCharacter::m_CubeLolo.~CLolos();
	CNkCharacter::m_CubeLolo.SetIndexedTexture(g_pCapsyongTexture);
	CNkCharacter::m_CubeLolo.Read(fpLolo);
	pfclose(fpLolo);

	// ������� �б�
	LoadChaActList();
	// �ϴÿ��� �غذŸ��� ���̸� �̸� ����س���.
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
		m_EventWearing[i] = NULL; ///�̺�Ʈ��������
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

	m_pUserShopPopup = new CAlphaPopUp; ///���λ���
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

	m_InChaSkill = 0; //�ʱ� ��ų��.
	m_InChaAidiaSkill = 0; //�ʱ� ���̵�� ��ų��.
	m_UseBook = 0; //å ��� ȿ��
	m_ShowConditionEffect = FALSE; //����ȿ��
	m_ShowConditionEffect2 = FALSE; //����ȿ��2
	m_ShowSkillEffect = FALSE; //���ӽ�ųȿ��
	m_ShowAidiaSkillEffect = FALSE; //���̵�� ���ӽ�ųȿ��
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
	m_bMoonEventEffect = FALSE; //0212 �뺸�� �̺�Ʈ�� Effect
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

	m_bstone = false;       // ����� ������ ����߳�. Framemov���� ���� ������ ���� Ȱ��ȭ �ȴ�.
	m_bresstone = false;    // ����� ������ ������ �غ� �ƴ°�.
	m_resmobidx =0;			// ������ ���� �ε���
	m_resmobvnum =0;		// ������ ���� vnum
	m_resmobx =0;			// ������ ���� ��ǥ
	m_resmobz =0;			// ������ ���� ��ǥ
	m_bBleed = false;		// ����
	m_bhaste = 0;			// ���̽�Ʈ
	m_nBattlePoint = 0;
	m_nLaghaimPoint = 0; // [2008/5/16 Theodoric] BP -> LP �� �ٲ�
	m_nGHPoint = 0;			// �׷��� ����� ����Ʈ, ���ġ�� ������� ���� �� ���� ����Ʈ�̴�.
	m_bComboReady = false;		//mungmae-2005/04/11
	m_bComboSucceed = false;	//mungmae-2005/04/12
	m_ShowReflection = false;	//mungmae-2005/04/20
	m_bProtectionShield = false;
	m_nComboFrame = 0;			//mungmae-2005/05/03
	m_byBurningStep = 0;
	m_nSkillLevel = 0;			// nate 2005-06-17 : ��ų�� ���õ�( ��ų����Ʈ )
	m_nSkillStep = 0;			// nate 2005-06-27 : �޸�-���ΰ���, ��ĭ-����ũ�����ܰ�
	m_nFailStep = 0;
	m_nOldFrame = 0;			// nate 2005-07-21 : Burning - �޼����� ������ �Ǵ�

	m_pRegenbar = NULL;
	m_pRegenbar = new CFxRegenBar();
	m_dwRegenTime = 0;
	m_nRegenIndex = 0;
	m_bDrawRegenbar = FALSE;
	m_bAttacked = FALSE;		// mungmae 2005-07-13: ���ݹް� �ֳ�?
	m_nAttVital = 0;
	m_byShowLIfeExpantion = 0;	// nate 2005-07-24 : Life Expansion - ������ ����Ʈ ���̱�

	m_bCloaking = FALSE;
	m_bCloaking_Waring = FALSE;
	m_bPoisoning = false;		// nate 2005-11-14 : ������ - ��������� ����
	m_fFlying_frame = 0.0f;
	m_fShadow_frame = 0.0f;
	m_bShowAdrenaline = false;	//mungmae-2005/11/21 : �Ƶ巹���� ȿ�� ����
	m_fSubAttackTime = 0.0f;
	m_bYanQuest	= FALSE;

	m_bVisible = FALSE; // �������� �ø��󿡼� ���̴°��� �÷���.
	m_bSecialQuest = FALSE;

	m_UserShopLevel = 0; // ���� ������ �����°�? ///���λ���
	ZeroMemory( m_strUserShopMent, sizeof(m_strUserShopMent) );
	//m_bSwapWeapon = TRUE;

	m_HaveCash = 0; // ���� ĳ�� ///�������

	m_dwComboTick = 0; ///�����޺�

	m_MultiTargetNum = 0;  // ���̺긮�� �ظ� �޺��� Ÿ�� ������ �����س��� �ʿ䰡�־... ///�����޺�
	m_MultiTargetKind = 0; ///�����޺�
	ZeroMemory( m_MultiTargetArray, sizeof(DWORD)*MAX_MULTITARGET ); ///�����޺�
	ZeroMemory( m_MultiTargetPos, sizeof(POINT)*MAX_MULTITARGET ); ///�����޺�

	m_bSpiritModeEffect = FALSE; //mungmae-2006/09/08 ���Ǹ� �������� ȿ�� �Ѱ� ����

	m_dwReturnTime = 0;
	m_dwResurrectionTime = 0;
	m_dwResurrection_StartTime = 0;
	m_bResurrectionSkill = FALSE;
	m_nResurrection_ChaId = 0;

	m_pMyGuilddata  = NULL;

	m_NvNWin = 0; ///�������¸� Ƚ��.

	m_bNowFreePVP = FALSE; // ������� ������ ����
	m_dwPvpDestIndex = 0; // ��û���� �ε���.(�ڽ��� ������� �߿��� ����.)

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
	case 0 :		// ����
		// ������� �÷���
		m_bWanted = false;

		Siren_Off(color);
		break;
	case 1 :		// �ѱ�
		// ������� �÷���
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
	if (m_pClrAlloc)	// �Ҵ���� �ö��� �����Ѵ�.
		m_pClrAlloc->ColorFree(m_wColorIdx);

	// ���õ� ȿ���� ���ش�.
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
	SAFE_DELETE(m_pUserShopPopup); ///���λ���

	SAFE_DELETE( m_pDropStar );
	// nate 2004 - 4 : Crash_Miss
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
		SAFE_DELETE( m_pCrash_MissPopup[ i ] );

	SAFE_DELETE( m_pRegenbar );

	SAFE_DELETE( m_pGiftMessage );
	SAFE_DELETE_ARRAY( m_NkCha_Particle ); // �迭 ����Ʈ�� �ٲ�.(�迭�� new �ϰ� �׳� delete�� �ϰ��־���. �̷��� �ϸ� �޸� ���� �����ʴ°�?) (06-11-20 ����)
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
	if( m_pUserShopPopup ) ///���λ���
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

// nate 2005-11-23 : �Լ� ���� - ����Ʈ ����
// ���� : ���콺 Ŭ���� ���ÿ� ����Ʈ�� �ߵ��ϴ� ��ų�� �߰�
// ���� : ���� ���� ������ �߿� ����Ʈ �߰��Ǵ°��� bpdilst.dta���� ������������ �̿��Ұ�
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
	//�߰� ��ų �߰�	(��ĭ ��ũ����)
	else if(magicNum == SKILL_HELL_CRY)
	{
		m_MagicChaCount = 1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_HELL_CRY);
	}
	//------------------------------------------------------------------------
	// �转
	else if(magicNum == SKILLF_A_BASH)
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_BASH);
	}
	//------------------------------------------------------------------------
	// ���� ������
	else if(magicNum == SKILLF_A_DSLASH)
	{
		m_MagicChaCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_DSLASH);
	}
	//------------------------------------------------------------------------
	// ���Ʈ
	else if (magicNum == SKILLF_A_ASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_ASSAULT);
	}
	//------------------------------------------------------------------------
	// ��� ���Ʈ
	else if (magicNum == SKILLF_C_DASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = pCha->m_nCharIndex;
		m_AttackMobId = -1;
		Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_DASSAULT);
		// �÷��̾ �ƴ϶��
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
	else if( magicNum == SKILL_H_AUTOMINE )	// ���丶��
	{
		if( m_nSkillLevel > 0 )				// ��ų������ 0 �̻�
		{
//			if( g_pRoh == this ) // ���丶���� ��� �� �߻��� �ߺ������ �Ǵ°Ͷ��� ���� ��Ÿ�ӵ������ �ٲپ���. 07-03-22 ����
//				G_SK_SetCooltime( SKILL_H_AUTOMINE );	// CoolTime Setting
			m_MagicChaCount = 1;
			m_AttackChaId = pCha->m_nCharIndex;
			m_AttackMobId = -1;
			m_MagicNum = magicNum;
			Attack( pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum );
			pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pCha, NULL, EFFECT_SKILL_MINE );
		}
	}
	//mungmae-2005/07/15 ��ٿ� �߰�
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
	else if( magicNum == SKILL_F_DASHSLASH ) ///�����޺�
	{
		// �뽬 �������� ���⼱ �׳� Ÿ�� ����Ʈ�� �־��ش�.(�ڱ� �ڽŲ� ���⼭ ���� �ʴ´�.ó���� ������ �и�����)
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, pCha, NULL, EFFECT_SKILL_DSLASH );
	}
	else if( magicNum == SKILL_F_SLASHSHADOW ) ///�����޺�
	{
		this->Attack(pCha->m_wx, pCha->m_wy, pCha->m_wz, magicNum ); // ����ó���� ���ش�.
	}
	else if( magicNum == SKILL_F_STONEBASH2 ) ///�����޺�
	{
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, pCha, NULL, EFFECT_SKILL_STONE_LANCE );
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, pCha, NULL, EFFECT_SKILL_BURNINGCRASH2 );

		// Ÿ�� ����Ʈ �߰�
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_HORNBOOM;
		ef_sort.vPos = D3DVECTOR( pCha->m_wx, pCha->m_wy + 12.0f, pCha->m_wz );
		ef_sort.Index = HIT_GRYPHON;
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));


		// �ٶ󺸴� ������ ����ν����� �ǰ� �ϱ� ���ؼ�...
		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_MagicChaCount = -1;
		m_AttackMobId = this->m_resmobidx; // �ڽ��� ����ν����� ���ϵ���.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT1 ) ///�����޺�
	{
		this->SetMotionState( CHA_F_BASHREFLECT ); // ���������.
		this->SetShowCrash_Miss( 10 ); // �޺����� ����Ʈ(��) ���.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, NULL, EFFECT_SKILL_SHADOWSLASH );

		this->m_nComboFrame = 1; // ��ų�� phase�� ���.

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_MagicChaCount = -1;
		m_AttackMobId = this->m_resmobidx; // �ڽ��� ����ν����� ���ϵ���.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT2 ) ///�����޺�
	{
		// �н� ����Ʈ�� ����.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pCha, NULL, EFFECT_SKILL_BASHREFLECTION );

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_MagicChaCount = -1;
		m_AttackMobId = this->m_resmobidx; // �ڽ��� ����ν����� ���ϵ���.....
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
	//���� ����Ʈ ǥ��
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

// nate 2005-11-23 : �Լ� ���� - ����Ʈ ����
// ���� : ���콺 Ŭ���� ���ÿ� ����Ʈ�� �ߵ��ϴ� ��ų�� �߰�
// ���� : ���� ���� ������ �߿� ����Ʈ �߰��Ǵ°��� bpdilst.dta���� ������������ �̿��Ұ�
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

	// ������� �������� ��������
	if (magicNum == SKILL_DROP_ATTACK || magicNum == SKILL_ROTATE_ATTACK || magicNum == SKILL_FORCE_ATTACK )
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
	}
	//------------------------------------------------------------------------
	// ��ũ����
	else if(magicNum == SKILL_HELL_CRY)
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_HELL_CRY);
	}
	//------------------------------------------------------------------------
	// �����Ǽ�
	else if(magicNum == SKILL_SUMMON_PARTITION)
	{
		m_MagicMobCount = 1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_SUMMON_PARTITION);
	}
	//------------------------------------------------------------------------
	// �转
	else if(magicNum == SKILLF_A_BASH)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_BASH);
	}
	//------------------------------------------------------------------------
	// ���� ������
	else if(magicNum == SKILLF_A_DSLASH)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_DSLASH);
	}
	//------------------------------------------------------------------------
	// ���Ʈ
	else if (magicNum == SKILLF_A_ASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_ASSAULT);
	}
	//------------------------------------------------------------------------
	// ��� ���Ʈ
	else if (magicNum == SKILLF_C_DASSAULT)
	{
		m_MagicMobCount = -1;
		m_AttackChaId = -1;
		m_AttackMobId = pMob->m_nVirId;
		Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum);
		pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_DASSAULT);
		// �÷��̾ �ƴ϶��
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
	else if( magicNum == SKILL_H_AUTOMINE )	// ���� ����
	{
		if( m_nSkillLevel > 0 )				// ��ų������ 0 �̻�
		{
//			if( g_pRoh == this ) // ���丶���� ��� �� �߻��� �ߺ������ �Ǵ°Ͷ��� ���� ��Ÿ�ӵ������ �ٲپ���. 07-03-22 ����
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
	else if( magicNum == SKILL_F_DASHSLASH ) ///�����޺�
	{
		// �뽬 �������� ���⼱ �׳� Ÿ�� ����Ʈ�� �־��ش�.(�ڱ� �ڽŲ� ���⼭ ���� �ʴ´�.ó���� ������ �и�����)
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, pMob, EFFECT_SKILL_DSLASH );
	}
	else if( magicNum == SKILL_F_SLASHSHADOW ) ///�����޺�
	{
		this->Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz, magicNum ); // ����ó���� ���ش�.
	}
	else if( magicNum == SKILL_F_STONEBASH1 ) ///�����޺�
	{
		this->SetMotionState( CHA_F_STONEBASH ); // ��� ����.
		this->SetShowCrash_Miss( 5 ); // �޺����� ����Ʈ(��) ���.
		this->m_nComboFrame = 0; // ����转�� �ܰ踦 �ʱ�ȭ.

		if( pMob->m_nVirId == this->m_resmobidx ) // �ڱ� �ڽ��� ����ν��濡 ����转�� ����..
		{
			// �޺���ų üũ�� ƽ�� ����.
			this->m_dwComboTick = timeGetTime();
			this->m_bComboSucceed = true;
			pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // �̰� ����� �� �ִϸ��̼��� ������ �����ش�.
		}
	}
	else if( magicNum == SKILL_F_STONEBASH2 ) ///�����޺�
	{
		// ����转�� ���⼱ �׳� Ÿ�� ����Ʈ�� �־��ش�.
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, pMob, EFFECT_SKILL_STONE_LANCE );
		pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, pMob, EFFECT_SKILL_BURNINGCRASH2 );

		// Ÿ�� ����Ʈ �߰�
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_HORNBOOM;
		ef_sort.vPos = D3DVECTOR( pMob->m_wx, pMob->m_wy + 12.0f, pMob->m_wz );
		ef_sort.Index = HIT_GRYPHON;
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));


		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_AttackMobId = this->m_resmobidx; // �ڽ��� ����ν����� ���ϵ���.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT1 ) // ��� ��Ŷ ///�����޺�
	{
		this->SetMotionState( CHA_F_BASHREFLECT ); // ���������.
		this->SetShowCrash_Miss( 10 ); // �޺����� ����Ʈ(��) ���.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, NULL, EFFECT_SKILL_SHADOWSLASH );

		this->m_nComboFrame = 1; // ��ų�� phase�� ���.

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_AttackMobId = this->m_resmobidx; // �ڽ��� ����ν����� ���ϵ���.....
		m_AttackChaId = -1;
	}
	else if( magicNum == SKILL_F_BASHREFLECT2 ) // Ÿ�� ��Ŷ ///�����޺�
	{
		// �н� ����Ʈ�� ����.
		pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, pMob, EFFECT_SKILL_BASHREFLECTION );

		m_MagicMobCount = 1;
		m_MagicMobId[0] = this->m_resmobidx;
		m_AttackMobId = this->m_resmobidx; // �ڽ��� ����ν����� ���ϵ���.....
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

void CNkCharacter::SetAttackChaId(int chaId, BOOL bPetAttack ) // �굵 ���ݽ�ų�� �÷��׸� �߰�. (�Ϻ� ��ų�� ������ �Ƚ�Ű�� �ϱ����ؼ�..) 06-01-31 ����
{
	m_AttackChaId = chaId;
	m_AttackMobId = -1;

	if (g_pRoh == this && pCMyApp->m_pMyPet && bPetAttack ) // �굵 ���ݽ�ų�� �÷��׸� �߰�. (�Ϻ� ��ų�� ������ �Ƚ�Ű�� �ϱ����ؼ�..) 06-01-31 ����
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
	//mungmae -2005/08/01 ���̵�� ���� ������ �ɷ� ������ ���� �޴� �ɷ� �ν��ϴ°� ����
	if( m_nAttVital > m_Vital )
//		&& !m_byShowLIfeExpantion) ///07-04-10 ���� ����. ���� ������ �� �Լ��� �����°� ���Ͱ� �߸��̴�. ����Ʈ���� ���Լ� ȣ���ϴ°� ���ݴ�. �̰Ŷ��� �������� �ɷ������� ��ٿ� ��Ұ� �ȵ��ڳ� -_-;
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

						if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag  ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_HELMET] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // �Ѵ� �������ϰ� �ִٸ�...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_FACE:

						if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_HELMET] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // �Ѵ� �������ϰ� �ִٸ�...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_BREAST:

						if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_ARMOR] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // �Ѵ� �������ϰ� �ִٸ�...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_ARMS:

						if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_ARMOR] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // �Ѵ� �������ϰ� �ִٸ�...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_LEGS:

						if( m_EventWearing[WEARING_PANTS] && m_bEventWearFlag ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else if( m_Wearing[WEARING_PANTS] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
						{
							if( !(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
							}
						}
						else // �Ѵ� �������ϰ� �ִٸ�...
						{
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}

						break;

					case NKCHA_FEET: // �̳༮�� ������ ���� �����ϴ�.

						if( (m_EventWearing[WEARING_PANTS] || m_EventWearing[WEARING_SHOES] ) && m_bEventWearFlag) // �̺�Ʈ ������ �ִ°��.
						{
							if( m_EventWearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								break; // �ȱ׸��� ����.
							}

							if( m_EventWearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								break; // �ȱ׸��� ����.
							}

							// �̺�Ʈ ������ �԰� �ִµ� �׸��� ������ ����.
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}
						else if( m_Wearing[WEARING_PANTS] || m_Wearing[WEARING_SHOES] ) // �̺�Ʈ ������ �Ѵ� ���� ��� �Ϲ� ������ üũ�Ѵ�.
						{
							if( m_Wearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								break; // �ȱ׸��� ����.
							}

							if( m_Wearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
							{
								break; // �ȱ׸��� ����.
							}

							// �̺�Ʈ ������ �԰� �ִµ� �׸��� ������ ����.
							m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
						}
						else // �ƹ��͵� ���԰��־ �׸���.
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
						// Animate ���Ѵ�...
					}
					else
						m_CapSyong[i]->Animate(m_Biped, m_CurFrame);
				}
			}
		}

		//------------------------------------------------------------------
		// Frame Move ���� ���� ����
		for (i = 0; i < WEARING_SHOW_NUM; i++)
		{
			// ���Ⱑ �ִٸ�
			if (m_Wearing[i]/*.m_ProtoNum >= 0*/ && GET_CSY(m_Wearing[i], m_BpdRace))
			{
				//--------------------------------------------------------------
				// �����̾�
				if (i == WEARING_CLOAK && m_nMotionState == CHA_THROW_SLAYER &&
						m_CurFrame >= THROW_SLAYER_FRAME_START && m_CurFrame <= THROW_SLAYER_FRAME_END)
				{
					// SubCsy �� Frame move �Ѵ�(� �� �� ���  �տ� �� ���� Animate )
					if (GET_SUB_CSY(m_Wearing[i]))
						GET_SUB_CSY(m_Wearing[i])->Animate(m_Biped, m_CurFrame);
				}
				// nate 2005-06-10 : Burning Crash - ĸ�� ���ϸ��̼�
				else if (i == WEARING_CLOAK && m_nMotionState == CHA_B_BURNINGCRASH )
				{
					// SubCsy �� Frame move �Ѵ�(� �� �� ���  �տ� �� ���� Animate )
					if (GET_SUB_CSY(m_Wearing[i]))
						GET_SUB_CSY(m_Wearing[i])->Animate(m_Biped, m_CurFrame);
				}
				//--------------------------------------------------------------
				// ����� ���� - Lyul_2005
				else if (i == WEARING_CLOAK && m_nMotionState == CHA_F_DSTONE)
				{
					// �տ� �� ���� �׸�
					if(m_CurFrame >= 8 && m_CurFrame <= DSTRONE_SLAYER_FRAME_END && !m_bstone)
					{
						// SubCsy �� Frame move �Ѵ�(� �� �� ���  �տ� �� ���� Render )
						if (GET_SUB_CSY(m_Wearing[i]))
							GET_SUB_CSY(m_Wearing[i])->Animate(m_Biped, m_CurFrame);

						//----------------------------------------------------------
						// ���� ������ ����.
						if(m_CurFrame > 17)
						{
							// Add mob
							if(m_bresstone && m_resmobidx)
							{
								m_bstone = true; // Item �� ���̻� �׸��� �ʰ� ����
								m_bresstone = false; // ����� �� ���� �Ϸ�
								pCMyApp->m_pUIMgr->AddMob(m_resmobvnum, m_resmobidx, m_resmobx, m_resmobz,m_resmobdir,1,0,0,9);

								// Effect
								CNkMob *pMob = pCMyApp->m_pUIMgr->FindMobById(m_resmobidx);

								if(pMob)
									pCMyApp->m_pUIMgr->InsertMagic(NULL, pMob, NULL, NULL, EFFECT_DSTONE_EFF);

								if( this == g_pRoh ) // �ڱ� �ڽ��̸� /// �����޺�
									m_dwComboTick = timeGetTime(); // �޺� ƽ�� ����.
							}
						}
					}
					// � �� ���� �׸�
					else if(!m_bstone && GET_CSY(m_Wearing[i], m_BpdRace))
						GET_CSY(m_Wearing[i], m_BpdRace)->Animate(m_Biped, m_CurFrame);
				}
				// �Ϲ� Frame Move
				else if( GET_CSY(m_Wearing[i], m_BpdRace) && !EventWearingFrameMove(i) ) ///�̺�Ʈ�������� �� �ϰ� �ִ��� ���ε� Ȯ�����ְ� �����ϰ������� �̺�Ʈ ������ �ִϸ���Ʈ �����ش�.
					GET_CSY(m_Wearing[i], m_BpdRace)->Animate(m_Biped, m_CurFrame);
			}
			else if( m_EventWearing[i] && m_bEventWearFlag ) // ���� ��� �������־ �̺�Ʈ ������ �����ϰ��ִ� ��츦 üũ�Ѵ�. ///�̺�Ʈ��������
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
			// ��ũ�� �����̴�! � ���� �׸��� ���� �տ� ��ɷ� �׷���!
			// 2005.05.11
			// Lyul / ���� �Ϸ�
			if (m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]) // NULL Check
				if (GET_SUB_CSY(m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]))
					GET_SUB_CSY(m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum])->Animate(m_Biped, m_CurFrame);
		}
	}

	///1021 ������..����~~
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
	// nate 2005-06-15 : Burning Crash - ����Ʈ
	if( m_bCombat
			&& m_Wearing[WEARING_CLOAK]
			// nate 2005-06-07 : ���� ũ���� ���� �׸���
			&& GET_SHAPE(m_Wearing[WEARING_CLOAK]) == ISUB_SLAYER
			&& m_nMotionState == CHA_B_BURNINGCRASH )
	{
		// nate 2005-06-10 : Burning Crash - �� ����Ʈ
		if( m_nMotionState == CHA_B_BURNINGCRASH
				&& GET_SUB_CSY( m_Wearing[ WEARING_CLOAK ] )
				&& (( m_CurFrame >= 72 && m_CurFrame <= 84 )
					|| m_CurFrame >= 112 ) )
			GET_SUB_CSY( m_Wearing[ WEARING_CLOAK ] )->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_BLEED
					,&m_matWorld, &(pCMyApp->m_matView));
		// nate 2005-06-10 : Burning Crash - ����
		int nSkillLevel = ( this == g_pRoh )?g_ChaSkill.m_SkillLevel[ 16 ]:m_nSkillLevel;
		if((( nSkillLevel - 1 ) / 5 ) >= 3 )	// 16 : ��ĭ 16��° ��ų( ���� ũ���� )
			SetTraceLineLVertex( TRACELINE_BURNINGCRASH );
	}
	else if (m_bCombat
			&& m_bEventWearFlag
			&& m_nMotionState != CHA_SOCIAL
			&& m_nMotionState != CHA_SWIM
			&& m_nMotionState != CHA_SWIM_STAND
			&& m_nMotionState != CHA_THROW_SLAYER
			&& m_nMotionState != CHA_THROW_CHAKRAM
			&& m_nMotionState != CHA_F_DSTONE		// Lyul_2005 ����ν����� ��쿡�� �����.						
			&& m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : �Ϸ�Ʈ�� ��
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
			 && m_nMotionState != CHA_F_DSTONE		// Lyul_2005 ����ν����� ��쿡�� �����.
			 && m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : �Ϸ�Ʈ�� ��
			 && m_Wearing[WEARING_WEAPON]
			 && GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace))
	{
		//=================================================
		// nate 2004 - 11 ; new race - �̵��� ��Į ����
		if(GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) )
		{
			// ���� ��
			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_BLEED,
						&m_matWorld, &(pCMyApp->m_matView));
			// ������ ��
			GET_SUB_CSY(m_Wearing[WEARING_WEAPON])->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_BLEED,
					&m_matWorld, &(pCMyApp->m_matView));

			// �̵����� ��/�츦 �Ǵ��Ͽ� ������ �����.
			if (m_nMotionState == CHA_ATTACK)
			{
				if(m_CurFrame < 11) // 9frame ������ ������ ����
					SetTraceLineLVertex(TRACELINE_ATTACK,0,false);
				// ���� �� ���� ���� ����Ʈ�� �������� �������� ���ϹǷ� ���������� �����Ѵ�.
				else
				{
					SetTraceLineLVertex(TRACELINE_ATTACK,0,true);

					CNkMob *pMob = NULL;
					pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
					pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_DSWORD);
				}
			}
			else if( m_nMotionState == CHA_F_DASHSLASH ) ///�����޺�
			{
				SetTraceLineLVertex( TRACELINE_DASHSLASH ); // �� �༮�� �̵������� �ȿ��� �ڵ����� ���� �˲� �������ش�.
			}
			else if( m_nMotionState == CHA_F_SLASHSHADOW ) ///�����޺�
			{
				SetTraceLineLVertex( TRACELINE_SLASHSHADOW ); // �� �༮�� �̵������� �ȿ��� �ڵ����� ���� �˲� �������ش�.
			}
		}
		else if (m_Wearing[WEARING_WEAPON]->m_Vnum==773 || m_Wearing[WEARING_WEAPON]->m_Vnum==1023)
		{
			//1008 ������
			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, TRUE, LINE_EFF_LIGHTSWORD,
						&m_matWorld, &(pCMyApp->m_matView));
		}
		// Į ������ ���� �����Ѵ�.
		// Lyul_2005 �ظ� �߰�.
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
		// ���� ��� �����ϴ� Motion
		else if (m_nMotionState == CHA_ATTACK && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN)
		{
			if (g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame <= m_CurFrame &&
					m_CurFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame + 3
					&&( m_Wearing[WEARING_WEAPON]->m_Vnum!=1238 && m_Wearing[WEARING_WEAPON]->m_Vnum!=2172 ))
			{
				//����ũ��Ÿ� ��������� �ȵ���.
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

				// ���� ������ Ȯ���Ѵ�.
				switch(m_Wearing[WEARING_WEAPON]->m_Vnum)
				{
				case 60:		// ������ �ǽ���
				case 61:		// ������ ������
					//dwGunFxColor = 0xFFEA601C;
					dwGunFxColor = 0xFFC1C1FE;
					dwGunFxAttackColor = 0xFFC1C1FE;
					break;
				case 203:		// �����ڵ��
				case 55:		// �����
				case 204:		// ����
					dwGunFxColor = 0xFFFFF8B5;
					dwGunFxAttackColor = 0xFFFFF8B5;
					break;
				default:
					dwGunFxColor = 0xFFFFFFFF;
					dwGunFxAttackColor = 0xFFFFFFFF;
					break;
				}

				//----------------------------------------------------------------
				// �ѿ� �´� Effect ( ��ǥ���� ��Ÿ��)
				// [3/28/2007 Theodoric] �±� ��ũ�� �̺�Ʈ :: �ѿ� �´� ����Ʈ ����
				if ( (pTargetMob || pTargetCha) &&
						m_Wearing[WEARING_WEAPON]->m_Vnum != 1538 && m_Wearing[WEARING_WEAPON]->m_Vnum != 1539 )
				{
					// �Ÿ��� ����߸���.
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
				// �ѿ��� ��ǥ������ ������ ��Ÿ���� Effect

				// ��Ȯ�� �������� ������~~!
				m_dir = m_toDir = (float) atan2(m_tz - m_wz, m_tx - m_wx);

				// ����� ��밡 ������, ó���� �����, tx ����� ���� ����Ѵ�.
				float fDist = DistPlToPl(m_wx, m_wz, m_tx, m_tz);
				D3DVECTOR tVector = D3DVECTOR(fDist, (m_ty-m_wy)+m_TargetHeight, 0);
				// �� ����.
				if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) )
					GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Animate(m_Biped, m_CurFrame, FALSE, LINE_EFF_NONE, &m_matWorld, &(pCMyApp->m_matView),
							&tVector);

				ZeroMemory(&ef_sort, sizeof(EffectSort));

				//----------------------------------------------------------------
				// �� �߻� Effect ( �ѱ��� ��Ÿ��)
				// [3/28/2007 Theodoric] �±� ��ũ�� �̺�Ʈ :: �� �߻� ����Ʈ�� ����
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

	//  [5/6/2009 ppmmjj83] �Ϻ� �������� �������� Į�� Į���� ���� ������� ó���� ����ϴ� �κ��� ����
	else if( m_Wearing[WEARING_WEAPON]
			 && ( m_Wearing[WEARING_WEAPON]->m_Vnum == 1590 || m_Wearing[WEARING_WEAPON]->m_Vnum == 2694 )
			 && !m_bCombat
		   ) ///�Ϻ����������׽�Ʈ ������ �� �䵵 ���󸶻縦 �����ִٸ� ��ȭ��忡�� �㸮�� �����ֵ��� ���̰�...
	{
		int ProtoNum = g_ItemList.FindItem( 1712 ); // ���󸶻� ���� ĸ����.

		if( GET_CSYC(ProtoNum, m_BpdRace) )
		{
			GET_CSYC(ProtoNum, m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
	}

	//  [5/6/2009 ppmmjj83] �Ϻ� �������� �������� Į�� Į���� ���� ������� ó���� ����ϴ� �κ��� ����
	else if( m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2168 && !m_bCombat ) ///�Ϻ����������׽�Ʈ ������ �� �䵵 ���󸶻縦 �����ִٸ� ��ȭ��忡�� �㸮�� �����ֵ��� ���̰�...
	{
		int ProtoNum = g_ItemList.FindItem( 2317 ); // ���󸶻� ���� ĸ����.

		if( GET_CSYC(ProtoNum, m_BpdRace) )
		{
			GET_CSYC(ProtoNum, m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
	}

	//  [5/6/2009 ppmmjj83] �Ϻ� �������� �������� Į�� Į���� ���� ������� ó���� ����ϴ� �κ��� ����
	else if( m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2339 && !m_bCombat ) ///�Ϻ����������׽�Ʈ ������ �� �䵵 ���󸶻縦 �����ִٸ� ��ȭ��忡�� �㸮�� �����ֵ��� ���̰�...
	{
		int ProtoNum = g_ItemList.FindItem( 2340 ); // ���󸶻� ���� ĸ����.

		if( GET_CSYC(ProtoNum, m_BpdRace) )
		{
			GET_CSYC(ProtoNum, m_BpdRace)->Animate(m_Biped, m_CurFrame);
		}
	}

	else if( m_nMotionState == CHA_H_ELECTRICSHOT )		// nate 2005-11-24 : �Ϸ�Ʈ�� �� - ���� �ٲٱ�
	{
		if( m_CurFrame > 9 && m_CurFrame < 56 )
		{
			CItem* ElectricShot_Gun = new CItem;
			ZeroMemory( ElectricShot_Gun, sizeof( CItem ) );
			ElectricShot_Gun->m_ProtoNum = g_ItemList.FindItem( 1339 );		// �߸��� ��
			if( GET_CSY(ElectricShot_Gun, m_BpdRace) )
				GET_CSY(ElectricShot_Gun, m_BpdRace)->Animate(m_Biped, m_CurFrame, FALSE, LINE_EFF_NONE, &m_matWorld, &(pCMyApp->m_matView) );
		}
	}


	//------------------------------------------------------------------------
	// ���� ����
	// 2004.07.20
	// Lyul / �����Ϸ�
	// �Ѻ��� Ʋ�� ���� ����
	if (bVisible && g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_SoundNum >= 0 && g_pRoh ) ///���λ��� �� �۾�
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
					// ���Ⱑ ���̶�� ���� �ٸ� Sound �� �����Ѵ�.
					if(m_nMotionState == CHA_ATTACK && m_Wearing[WEARING_WEAPON] &&  GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN)
					{
						switch(m_Wearing[WEARING_WEAPON]->m_Vnum)
						{
						case 204: // ����
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_1], x, y, z);
							break;
						case 55: // �����
						case 61: // �����Ͷ�����
						case 203: // ���� �ڵ��
						case 464: // �Ŀ���
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_2], x, y, z);
							break;
						case 60: // ������ �ǽ���
						case 286: // �ӽŰ�
						case 465: // ������
						case 474: // ����̵� ������
						case 644: // ��Ʈ����
						case 1066: // ũ���� ��(�Ź���)
						case 1316:	// ��-��Ʈ����
						case 1321:	// ��-ũ���� ��
						case 1707:	// ��-ũ���� �� (�븸5�ֳ�)
						case 1716:	// ��Ʈ���� ������(�븸�̺�Ʈ��)
						case 2291:	// �߽����� ��Ʈ���� ������
						case 2923:	// ��-����̵� ������
						case 3285:	// ��-ũ���� ��
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
						case 467: // ų�� ������
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_4], x, y, z);
							break;
						case 287: // ���̿� ������
						case 466: // ������ ����
						case 2264:	// �߽����� ���̿� ������
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_5], x, y, z);
							break;
						case 305: // �ö�� ������
						case 408: // ���Ϸ� ��
						case 410: // ũ���� ������
						case 983: // ���̺� ������
						case 2274:	// �߽����� ���Ϸΰ�
						case 2284:	// �߽����� ���̺� ������
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_6], x, y, z);
							break;
						case 306: // �Ľ�Į ������
						case 409: // �ñ׸� ������
						case 2269:	// ������� �Ľ�Į ������
						case 2279:	// �߽����� �ñ׸� ������
							g_pDSound->Play(g_EffectSound[EFF_SND_GUN_7], x, y, z);
							break;
						case 2052:	// �긮����
						case 2176:	// ���� �긮����
						case 2770:	// ���� �긮����(E)
						case 2746:	// �ϱ۸�
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
					// ���� �ƴ϶�� BPD �� Set �� Character ���� �����Ѵ�.
					else
						g_pDSound->Play(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_SoundNum, x, y, z);
				}
			}
			else if (m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame &&
					 m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame)
			{
				// ������ ���� ������ ������ �Ҹ��� ����.
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
							//mungmae-2005/08/29 �Ϻ� �ϸ��� ���� �߰�
							if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1125 )
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_HARISEN], x, y, z);
							}
							else if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1280 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1471 ) ///��¦��¦��ġ �߰�
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_PIKOPIKO], x, y, z);
							}
							else if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1515 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1517 ) // 070213 ��Ʈ�ε�� �������ε� ���� �ɼ� ��û������ �������� �ҵ��.
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
						// ī�� �����˿����� ������ �Ҹ�
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

	// nate 2005-06-09 : Burning Crash - �ܰ� ����
	if(	m_nMotionState == CHA_B_BURNINGCRASH )	// ��ĭ - ���� ũ����
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
		if( m_byBurningStep && !( m_nMotionState==CHA_SOCIAL&&(m_ActNum>=153&&m_ActNum<=158) ) ) // ������ ���߿� m_byBurnigStep �� �̿��ϹǷ� �ʱ�ȭ ���ϵ��� ����ó�� .
			m_byBurningStep = 0;
	}

	//mungmae-2005/04/11
	//����:��ũ���� �����߿� Ư���� ������ ������ ������ �޺� �غ�Ǿ��ٴ� ��.
	if( g_pRoh == this				// ��ĳ���� ����
			&& m_nMotionState == CHA_HELLCRY_ATTACK	// ��ũ����
			&& m_CurFrame >= 5						// 5������ �̻�
			&& m_CurFrame <= 12 )					// 12������ ����
	{
		m_bComboReady = true;					// �޺� �غ� �Ϸ�
		m_nComboFrame = m_CurFrame;				// ���� ������ ����
	}
	else
	{
		m_bComboReady = false;

		if( pCMyApp->m_pUIMgr )
			pCMyApp->m_pUIMgr->m_nClickTimeCheck = 0;
	}

	// ���̺긮�� �޺� ��ų (����ν���+ �转) ///�����޺�
	// ���� ����� ���� �������� AttackMsgSend�� �����ش�.
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
				// ����ν����� �����ش�
				CNkMob *pMob = pCMyApp->m_pUIMgr->FindMobById( m_resmobidx ); // �ش� ĳ������ ����ν����� ã�Ƽ�...
				if( pMob )
				{
					pMob->m_RunSmokeFrame = 1; // ����ν����� ���� ����� ����.
				}

				m_nComboFrame = 2;
			}
		}
		else if( m_nComboFrame == 2 )
		{
			if( m_CurFrame >= 20 )
			{
				if( g_pRoh == this ) // �ڱ��ڽ��̰� Ÿ�� ��Ŷ�� ���� ������ ���� ���¸�...
				{
					pCMyApp->AttackMsgSend( SKILL_F_STONEBASH2 ); // ����� ������ ������.
					m_nComboFrame = 3;
				}
			}
		}
	}

	// �转 ���÷�Ʈ�� ����� ������ Ÿ�� ��Ŷ�� ������. ///�����޺�
	if( m_nMotionState == CHA_F_BASHREFLECT )
	{
		if( m_nComboFrame ) // �ߵ� ��.
		{
			if( m_CurFrame > 15 )
			{
				if( this == g_pRoh && !m_bComboSucceed ) // Ÿ���� �ѹ��� ��������.
					pCMyApp->AttackMsgSend( SKILL_F_BASHREFLECT2 ); // Ÿ�� ��Ŷ. �� �ȿ��� m_bComboSucceed�� ���õȴ�.
			}
		}
	}

	// ���̺긮�� �޺� ��ų (�뽬+��������) ����
	if( m_nMotionState == CHA_F_DASHSLASH && m_CurFrame >= 6 && m_CurFrame <= 23 && m_fDash_Frame > 0 ) ///�����޺�
	{
		float pastFrame = float(m_CurFrame - m_PrevFrame); // ����Ǿ�� �� �������� ���Ѵ�.

		if(pastFrame > m_fDash_Frame)
			pastFrame = m_fDash_Frame; // �̵����� Ƚ����ŭ �̵�

		// ���� �̵� Ƚ�� ����
		m_fDash_Frame -= pastFrame;

		// �̵�
		float dx = m_dx * pastFrame;
		float dz = m_dz * pastFrame;

		// �÷��̾��� ��� ī�޶� ��ŷ �� stop �޼���
		if(this == g_pRoh)
		{
			pCMyApp->DashHero(dx , dz);
		}
		else
		{
			m_wx += dx;
			m_wz += dz;
		}

		// �̵� �Ϸ�
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
			|| (m_nMotionState == CHA_KNOCKBACK) // �˹� ó��  ///��
	  )
	{
		float pastFrame = float(m_CurFrame - m_PrevFrame); // ����Ǿ�� �� �������� ���Ѵ�.
		float* fFrame = NULL;
		if( m_nMotionState == CHA_F_DASH || m_nMotionState == CHA_KNOCKBACK ) // �˹鵵 ���� ������ ��.
			fFrame = &m_fDash_Frame;
		else if( m_nMotionState == CHA_B_FLYINGCRASH )
			fFrame = &m_fFlying_frame;
		else if( m_nMotionState == CHA_F_SHADOWSLASH )
			fFrame = &m_fShadow_frame;
		else
			return;

		if(pastFrame > *fFrame)
			pastFrame = *fFrame; // �̵����� Ƚ����ŭ �̵�

		// ���� �̵� Ƚ�� ����
		*fFrame -= pastFrame;

		// �̵�
		float dx = m_dx * pastFrame;
		float dz = m_dz * pastFrame;

		// �÷��̾��� ��� ī�޶� ��ŷ �� stop �޼���
		if(this == g_pRoh)
			pCMyApp->DashHero(dx , dz);
		else
		{
			m_wx += dx;
			m_wz += dz;
		}

		if( m_nMotionState == CHA_F_DASH )
		{
			// ������ ����
			m_CurFrame = 8;
			// �̵� �Ϸ�
			if(m_fDash_Frame == 0.0f )
			{
				m_CurFrame = 24- nInc;
				// ������ ��� Stop Msg
				if(this == g_pRoh)
				{
				}
			}
		}
		else if( m_nMotionState == CHA_KNOCKBACK ) // �˹�ó��
		{
			// ������ ����
			m_CurFrame = 0;

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));
			ef_sort.nType = FX_RUNSMOKE;
			ef_sort.vPos = D3DVECTOR(m_wx, pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz), m_wz);
			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

			// �̵� �Ϸ�
			if(m_fDash_Frame == 0.0f )
			{
				// ������ ��� Stop Msg
				if(this == g_pRoh)
				{
					char strTemp[50] = "";
					sprintf(strTemp, "stop %.0f %.0f %.0f %.0f\n", g_pRoh->m_tx, g_pRoh->m_tz,
							g_pRoh->m_wy, (g_pRoh->m_toDir) * 10000);       // m_wx,m_wz�� �ƴ϶�  �뽬�� ����� m_tx,m_tz �������� ������� �ٸ� ������� ����� ���δ�.(y���� m_wy�� �´�.m_ty�� �Ⱦ��̹Ƿ�..) (05-12-09 ����)
					g_pTcpIp->SendNetMessage(strTemp);
				}

				SetMotionState(CHA_STAND);
			}
		}
	}
	//------------------------------------------------------------------------
	// ����(snifing / ��������) ���� Ani ó��
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
	// �������� ��ŵ Delay �ƴ� ��� ��ų�� ���⼭ frame ����.
	else
	{
		m_PrevFrame = m_CurFrame;
		m_CurFrame += nInc;
	}

	// SKY �� ��� ���� �ٴѴ�.
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
	{
		m_nFlyFrame += nInc;

		if (rand() % 20 == 0) // ���� ��ġ�� ������ �ְ� �;...
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
			if (m_ActNum == 87)			// �Ӹ� �ڴ� �����̾�����.. ��� �Ӹ��ڰ�...
				SetMotionState(CHA_SOCIAL, 88);
			else if (m_ActNum == 88)	// �Ӹ� �ڰ� �ִ� �����̸�.. �� ���д�...
			{}
			else if (m_ActNum == 67)	// �ɴ� �����̾�����... ��� �ɾ��ְ�...
				SetMotionState(CHA_SOCIAL, 68);
			else if (m_ActNum == 68)
			{
				// �ɾ� ������... ���� �ٸ��� �����̰�...
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
		else if( m_nMotionState == CHA_F_DASH ) ///�뽬����
		{
			SetMotionState(CHA_STAND); // �뽬�� �������� �������� �ʰ� �Ѵ�. (�ٸ� ��� �뽬�ÿ� ����� �ѹ� �� �ع����� ��찡 �ִ�.)
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
// Desc : �� �Լ��� ���߿� �پ��� �������� ����� ĳ���͸� ���鶧 Ȯ���Ͽ� ����� �� �ִ�.
//  ����..
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
// Desc : ĳ���Ϳ� ���� �� �ʿ��� �������� �־��ش�.
//     �̸��� �������� ��� ���̵� ����.
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
// Desc : ���� ���� tx, tz�� �̵��� �Ѵ�.
//   tx�� tz�� 1���Ŀ� ĳ���Ͱ� ������ ��ġ ��ǥ�̴�.
//   ��Ʈ�� delay�� 1/20 �ʷ� �����ȴ�..
// --------------------------------------------------------------------
BOOL CNkCharacter::Move(float tx, float tz)
{
	// Ÿ������ ����
	m_tx = tx;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);
	// ��Ʈ�� �����̸� ����Ͽ� 1.1�� ���� �������� ������ �ӷ��� ���Ѵ�.
	// �� 22������.
	m_dx /= 22.0f;
	m_dz /= 22.0f;
	m_bMoving = TRUE;

	return TRUE;
}

BOOL CNkCharacter::Place(float tx, float tz, float dir, int nFrame)
{
	m_bPlace = TRUE;
	m_fPlaceDir = dir;
	// Ÿ������ ����
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
// Desc : (tx, tz)�� ���Ͽ� ���� ����� ���Ѵ�.
// --------------------------------------------------------------------
void CNkCharacter::AttackChakram(float tx, float ty, float tz)//��ũ�� ����
{
	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}

	m_RenderMultiShot = m_MultiShot = m_NextMultiShot;
	m_NextMultiShot = 0;
	// Ÿ������ ����
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);
	m_bAttack = TRUE;

	SetMotionState(CHA_THROW_CHAKRAM);// ��ũ�� ����
}

//--------------------------------------------------------------------------
// Attack
//--------------------------------------------------------------------------
// 2004.11.19
// Lyul / �߰�
// ��ų ����
void CNkCharacter::Attack(float tx, float ty, float tz, int attkType)
{
	if( m_nMotionState == CHA_KNOCKBACK ) // �˹��߿� ������ ���ý�Ų��.(�̵� ������� �ٲ㼭 ������ �ȴ�.) (07-03-06 ����)
		return;

	m_CurAttackType = attkType;

	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}

	m_RenderMultiShot = m_MultiShot = m_NextMultiShot;
	m_NextMultiShot = 0;
	// Ÿ������ ����
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
	// DROP_ATTACK ( �������� )
	if (attkType == SKILL_DROP_ATTACK)
		SetMotionState(CHA_JUMP_ATTACK);
	// ROTATE_ATTACK ( �������� )
	else if (attkType == SKILL_ROTATE_ATTACK)
		SetMotionState(CHA_ROTATE_ATTACK);
	// Hell Cry ( �� ũ����)
	else if (attkType == SKILL_HELL_CRY)
		SetMotionState(CHA_HELLCRY_ATTACK);
	// Slayer / Chakram
	else if (attkType == SKILL_THROW)
	{
		// ������ chakram �̴��� �ش� ĳ���� chakram
		if(pCMyApp->m_pUIMgr->m_bChakram || m_bChakramatt)
		{
			// CHAKRAM ( ��ũ�� )
			SetMotionState(CHA_THROW_CHAKRAM);
			pCMyApp->m_pUIMgr->m_bChakram = false;
			pCMyApp->m_pUIMgr->m_bChakramAtt = false;

			if(m_bChakramatt)
				m_bChakramatt = false;
		}
		else
		{
			// SLAYER ( �����̾� )
			//mungmae-2005/4/11(if�� �߰�)
			//����: ��ũ���̿� �����̾� �޺��� �� ����� �ι� ���� ������.
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
			// ��������
			if (GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD)
				SetMotionState(CHA_ROTATE_ATTACK);
			// ��������
			else if (GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE)
				SetMotionState(CHA_JUMP_ATTACK);
		}
		// ���� ����.
		else
		{
			SetMotionState(CHA_ATTACK);
		}
	}
	// ���� / Spell
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
	// HANDLE (���ۼ�)
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

			SetMotionState(CHA_HANDLE); // �޸� �� ����
		}
	}
	// DEEP_ATTACK
	else if ( attkType == SKILL_DEEP_ATTACK )
		SetMotionState(CHA_HUMAN_DEEP); // �޸� �� ����
	// CHAIN_ATTACK
	else if ( attkType == SKILL_CHAIN_ATTACK )
		SetMotionState(CHA_HUMAN_SHORT);
	// AIR_RAID ����̵�.
	else if (attkType == SKILL_AIR_RAID)
		SetMotionState(CHA_AIRRAID);
	//������ �ູ
	else if (attkType == SKILL_BLESSING || attkType == SKILL_JOINT_LIFE || attkType == SKILL_LIFE_LIGHT)
		SetMotionState(CHA_AIDIA_SUB/*CHA_ATTACK*/);//1007
	else if (attkType == SKILL_A_LIFE_EXPANSION )	// nate 2005-07-06 : Life Expansion - SetMOtionState()
		SetMotionState( CHA_AIDIA_SUB );
	else if (attkType == SKILL_MAGIC_MOVE)
	{
		//��������
		if (!(m_Wearing[WEARING_SHIELD]) ||
				!(GET_ATT_FLAG(m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON) ||
				(g_pRoh == this && g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0)) //���� 0�̶��,
			SetMotionState(CHA_ATTACK);
		else
			SetMotionState(CHA_MAGIC_MOVE/*CHA_ATTACK*/);//�������� ��� ���⿡...
	}
	else if (attkType == SKILL_VANPIRE_TOUCH || attkType == SKILL_HELL_TOUCH )
	{
		if (!(m_Wearing[WEARING_SHIELD]) ||
				!(GET_ATT_FLAG(m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON) ||
				(g_pRoh == this && g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0))
			SetMotionState(CHA_ATTACK);
		else
			SetMotionState(CHA_KAI_TOUCH); // ī�̸��� ��ġ
	}
	else if ( attkType == SKILL_MELODY_OF_DEF || attkType == SKILL_MELODY_OF_GUD )
	{
		if (!(m_Wearing[WEARING_SHIELD]) ||
				!(GET_ATT_FLAG(m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON) ||
				(g_pRoh == this && g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0))
			SetMotionState(CHA_ATTACK);
		else
			SetMotionState(CHA_AIDIA_PIPE); // ���̵�� �Ǹ�
	}
	//------------------------------------------------------------------------
	// ����ũ
	//------------------------------------------------------------------------
	// ���� ������
	else if ( attkType == SKILLF_A_DSLASH)
		SetMotionState(CHA_F_DSLASH);
	// ��� ���Ʈ
	else if ( attkType == SKILLF_C_DASSAULT)
		SetMotionState(CHA_F_DASSAULT);
	// ���Ʈ
	else if ( attkType == SKILLF_A_ASSAULT)
		SetMotionState(CHA_F_ASSAULT);
	// ����� ����
	else if ( attkType == SKILLF_A_DSTONE)
		SetMotionState(CHA_F_DSTONE);
	// �转
	else if ( attkType == SKILLF_A_BASH)
		SetMotionState(CHA_F_BASH);
	// Ÿ�� ���� mungmae-2005/05/10 �� ������ �ɸ��� ��ǰ� ����Ʈ�� �ϰ� ����.
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
		//��ٿ� ���� ����
		//if( ( this == g_pRoh
		//		&& pCMyApp->m_pUIMgr->m_bRClickDown == 1 )		//���콺�� ������ ���� ��
		//	|| 	( this != g_pRoh
		//			&& m_nSkillStep == 1 ) 	)
		//{
		//	if( this == g_pRoh )
		//		pCMyApp->m_pUIMgr->m_byCheckHellDown = 0;
		//	SetMotionState( CHA_C_HELLDOWN );
		//}
		//else if( ( this == g_pRoh
		//			&& pCMyApp->m_pUIMgr->m_bRClickDown == 2 ) 	//���콺�� ����� ��
		//		|| ( this != g_pRoh
		//			&& m_nSkillStep == 10 ) )
		//{
		//	if( this == g_pRoh )
		//		pCMyApp->m_pUIMgr->m_byCheckHellDown = 1;
		//	SetMotionState( CHA_C_HELLDOWN2 );
		//}
		//else
		//	SetMotionState( CHA_STAND );

		//��ٿ� ���� ����
		if( ( this == g_pRoh
				&& pCMyApp->m_pUIMgr->m_bRClickDown == 1 )		//���콺�� ������ ���� ��
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
				   && pCMyApp->m_pUIMgr->m_bRClickDown == 2 ) 	//���콺�� ����� ��
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
	else if( attkType == SKILL_K_POISON )	// nate 2005-11-08 : ������ - SetMotionState
		SetMotionState( CHA_K_POISON );
	else if( attkType == SKILL_H_ELECTRICSHOT )	// nate 2005-11-15 : �Ϸ�Ʈ���� - SetMotionState
		SetMotionState( CHA_H_ELECTRICSHOT );
	else if( attkType == SKILL_F_SLASHSHADOW )	// �뽬������+������ ������ ///�����޺�
	{
		SetMotionState( CHA_F_SLASHSHADOW );
		SetShowCrash_Miss( 10 ); // �޺����� ����Ʈ(��) ���.
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
		// �Ÿ��� ���Ѵ�.
		float dist = DistPlToPlAbs(m_wx, m_wz, m_tx, m_tz);
		// �������� ���Ѵ�.
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

		// ���̿� �Ÿ��� 3������.. �Ѥ�;
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

	// Ÿ������ ����
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

	//mungmae-2005/07/25 ��ٿ� �� �׾���  ó��
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
// �ε����� ��ũ�� �� ����
// ����ũ�� �̵��� ����
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
	// ���� ĳ�� ��ǿ� ���� ó��
	//----------------------------------------------------------------------
	switch (nState)
	{
	//--------------------------------------------------------------------
	// �ȱ�
	case CHA_WALK:
		// ���� ����� ���
		if (m_bCombat)
		{
			// ���⸦ ���� ���� ���
			if (wearing_weapon)
			{
				// �μ� ����
				if( Check_TwoHandWeapon(wearing_weapon) )
				{
					switch (GET_SHAPE(wearing_weapon))
					{
					case IWEAPON_GUN: // �� ������ ���Ŀ� ���� �ٶ�...
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
					default: // ����, ������, ����...
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
			// ���� ����
			if (wearing_weapon)
			{
				if( Check_TwoHandWeapon(wearing_weapon) )
				{
					switch (GET_SHAPE(wearing_weapon))
					{
					case IWEAPON_GUN: // �� ������ ���Ŀ� ���� �ٶ�...
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
					case IWEAPON_SPEAR:			// ITS #8192 ����� ������ġ ��� ó�� by.Sol9 [2012.06.15]
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
					default: // ����, ������, ����...
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
					// �Ѽչ���
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
				// �Ǽ��������
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
			// ��ȭ��..
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
	// ���ġ��
	case CHA_SWIM:
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SWIM_GO]].m_pBiped);
		m_ActNum = BPDIDX_SWIM_GO;
		break;
	///1002
	case CHA_AIDIA_SUB:
	case CHA_KAI_SUB:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][63]].m_pBiped); // 63�� �����?
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
	// ����
	case CHA_ATTACK:
		SetCombat(TRUE); //? ���� : ��ȭ

		// ���Ⱑ ���� ���
		if (wearing_weapon)
		{
			if( Check_TwoHandWeapon(wearing_weapon) )
			{
				switch (GET_SHAPE(wearing_weapon))
				{
				case IWEAPON_GUN: // �� ������ ���Ŀ� ���� �ٶ�...
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
				// nate 2004 - 12 : new race - ��� �ҵ�
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
				default: // ����, ������, ����...
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
					case IWEAPON_GUN: // �� ������ ���Ŀ� ���� �ٶ�...
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

					default: // Į, ����, ������, ����...
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
			offset += 53; // BPDIDX_ATT_HAND_L;	// bpdindex.dta ���� �Ǽհ��� ������
			m_ActNum = offset;
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][offset]].m_pBiped);
		}
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// �������
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
				default: //������, ����...
					break;
				}
			}
			else
			{
				// �Ѽչ���
				switch (GET_SHAPE(m_Wearing[WEARING_WEAPON]))
				{
				case IWEAPON_SWORD:
				case IWEAPON_AXE:
				case IWEAPON_STAFF:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][58]].m_pBiped);
					m_ActNum = 58;
					break;
				default: // ����, ������, ����...
					break;
				}
			}
		}
		else
		{
			// ��� ����
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][61]].m_pBiped);
			m_ActNum = 61;
		}
		FrameReset();
		break;

	//--------------------------------------------------------------------
	// ��������
	case CHA_ROTATE_ATTACK:
		SetCombat(TRUE);

		if( m_Wearing[WEARING_WEAPON] )
		{
			if( Check_TwoHandWeapon(m_Wearing[WEARING_WEAPON]) )
			{
				// �μ� ����
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
				default: //������, ����...
					break;
				}
			}
			else
			{
				// �Ѽչ���
				switch (GET_SHAPE(m_Wearing[WEARING_WEAPON]))
				{
				case IWEAPON_SWORD:
				case IWEAPON_AXE:
				case IWEAPON_STAFF:
					SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][102]].m_pBiped);
					m_ActNum = 102;
					break;
				default: // ����, ������, ����...
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
	// �����̾�
	case CHA_THROW_SLAYER:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][106]].m_pBiped);
		m_ActNum = 106;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// ��ũ��
	case CHA_THROW_CHAKRAM:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][107]].m_pBiped);
		m_ActNum = 107;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// ���ۼ�
	case CHA_HANDLE:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2GUN]].m_pBiped);
		m_ActNum = BPDIDX_STAND_2GUN;//5����...
		FrameReset();
		break;
	///////////
	//--------------------------------------------------------------------
	// ���� STAND
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
	// ���ۼ�
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
	// ����
	case CHA_MAGIC:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][act_num]].m_pBiped);
		m_ActNum = act_num;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// ��ĭ ����. �Ⱦ���.
	case CHA_BULKAN_BOW:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][116]].m_pBiped);
		m_ActNum = 116;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// ī�� ��ġ
	case CHA_KAI_TOUCH:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][117]].m_pBiped);
		m_ActNum = 117;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// ���̵�� �Ǹ�
	case CHA_AIDIA_PIPE:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][118]].m_pBiped);
		m_ActNum = 118;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// �޸� ������
	case CHA_HUMAN_DEEP:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][119]].m_pBiped);
		m_ActNum = 119;
		FrameReset();
		break;
	//--------------------------------------------------------------------
	// �޸� ü�ξ���
	case CHA_HUMAN_SHORT:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][120]].m_pBiped);
		m_ActNum = 120;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// ��ũ����
	case CHA_HELLCRY_ATTACK:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][121]].m_pBiped);
		m_ActNum = 121;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// ����̵�
	case CHA_AIRRAID:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][122]].m_pBiped);
		m_ActNum = 122;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// ���Ʈ
	case CHA_F_ASSAULT:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_ASSAULT]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_ASSAULT;
		FrameReset();
		break;

	//-----------------------------------------------------------------
	// ��� ���Ʈ // Lyul_2005
	case CHA_F_DASSAULT:

		// ��� ���Ʈ�� ���� ĳ���Ͱ� Player�� ��� �̹� ���Ʈ�� ��� ���̹Ƿ�
		// Bpd �� �ٲ۴�.
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
	// �뽬
	case CHA_F_DASH:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DASH;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	// ���� ������
	case CHA_F_DSLASH:
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DSLASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DSLASH;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	//  ���̺긮�� �޺� ��ų �뽬 + ��������
	case CHA_F_DASHSLASH: ///�����޺�
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DSLASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DSLASH;
		FrameReset();
		m_CurFrame = 5; // �ణ �ʹ��� �Ѱܾ� �� �ڿ���������.
		m_PrevFrame = 5;
		break;

	//-----------------------------------------------------------------
	//  ���̺긮�� �޺� ��ų �뽬������ + �����콽����
	case CHA_F_SLASHSHADOW: ///�����޺�
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_SHADOWSLASH1]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_SHADOWSLASH1;
		FrameReset();
		m_CurFrame = 26; // �ణ �ʹ��� �Ѱܾ� �� �ڿ���������.
		m_PrevFrame = 26;
		pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // �̰� ����� �� �ִϸ��̼��� ������ �����ش�.
		m_drawLineNum = 0; // ���� �ʱ�ȭ.

		break;

	//-----------------------------------------------------------------
	//  ���̺긮�� �޺� ��ų ����ν��� + �转
	case CHA_F_STONEBASH: ///�����޺�
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_BASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_BASH;
		FrameReset();
		pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // �̰� ����� �� �ִϸ��̼��� ������ �����ش�.
		break;

	//-----------------------------------------------------------------
	//  ���̺긮�� �޺� ��ų ����转 + ���÷���
	case CHA_F_BASHREFLECT: ///�����޺�
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_REFLECTION]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_REFLECTION;
		FrameReset();
		pCMyApp->m_pUIMgr->m_b_MotionForAttack = TRUE; // �̰� ����� �� �ִϸ��̼��� ������ �����ش�.
		break;
	//-----------------------------------------------------------------
	//  ����� ����
	case CHA_F_DSTONE:
		m_bAttack = true;
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_DSTONE]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_DSTONE;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	//  Ÿ������
	case CHA_F_TIMECHARGE:
		m_bAttack = true;
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_TIMECHARGE]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_TIMECHARGE;
		FrameReset();
		break;
	//-----------------------------------------------------------------
	//  �转
	case CHA_F_BASH:
		m_bAttack = true;
		SetCombat(TRUE);
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_SKILL_BASH]].m_pBiped);
		m_ActNum = BPDIDX_SKILL_BASH;
		FrameReset();
		break;
	//���÷��� ��� �߰�(mungmae-05/04/20)
	case CHA_F_REFLECTION:
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_REFLECTION;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_pBiped);
		FrameReset();
		break;
	// nate 2005-06-03 : Burning Crash - ���� ����
	case CHA_B_BURNINGCRASH:
		{
			SetCombat(TRUE);
			int nSkillLevel = ( this == g_pRoh )?g_ChaSkill.m_SkillLevel[ 16 ]:m_nSkillLevel;
			m_ActNum = BPDIDX_SKILL_BURNING	+ ( ( nSkillLevel - 1 ) / 5 );	// ��ų 30����Ʈ�� 6�ܰ�� ����
			SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
			FrameReset();
		}
		break;
	// nate 2005-06-03 : ī�� �ٿ��� �߰�
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
	case CHA_H_AUTOMINE:	// nate 2005-07-15 : Auto Mine - ��� ����
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_AUTOMINE;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_A_SUMMONSQUAD:	// nate 2005-07-15 : Summon Squad - ��� ����
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SKILL_SUMMONSQ;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_B_FLYINGCRASH:		// �ö��� ũ���� - SetBiped
	case CHA_K_POISON:			// nate 2005-11-08 : ������ - SetBiped
	case CHA_H_ELECTRICSHOT:	// �Ϸ�Ʈ�� �� - SetBiped
	case CHA_F_SHADOWSLASH:		// ������ ������ - SetBiped
	case CHA_F_ADRENALINE:		// �Ƶ巹���� - SetBiped
		SetCombat(TRUE);
		m_ActNum = 66 + nState;	// Biped Index : ���ӵǴ� ���ؽ���
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_WORLDCUP_KICK:	 // mungmae 2006/06/05 : ������ ű ���
		SetCombat(TRUE);
		m_ActNum = BPDIDX_WORLDCUP_KICK;
		SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][ m_ActNum ]].m_pBiped);
		FrameReset();
		break;
	case CHA_SPIRITMODE:	 // mungmae 2006/08/31 : ���Ǹ�
		SetCombat(TRUE);
		m_ActNum = BPDIDX_SPIRITMODE; ///070723 �Ϻ����׸���Ʈ ����
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

	case CHA_DIE: // �״� ����� �ƴ�  �׾� �ִ� ����
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



	case CHA_KNOCKBACK: // �и��� ��� (�ϴ� ����μ� ���ִ� ����� ���) ///��
	//-----------------------------------------------------------------
	// ������(���ֱ� CHA_STAND)
	default:
		// �������
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
					case IWEAPON_GUN: // �� ������ ���Ŀ� ���� �ٶ�...
					case IWEAPON_MISSILE:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2GUN]].m_pBiped);
						m_ActNum = BPDIDX_STAND_2GUN;
						break;
					case IWEAPON_SWORD:
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2SWORD]].m_pBiped);
						m_ActNum = BPDIDX_STAND_2SWORD;
						break;
					// nate 2004 - 12 : new race - ��� �ҵ�
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
					default: // ����, ������, ����...
						// IWEAPON_AXE, IWEAPON_STAFF, IWEAPON_BALL
						SetBiped(g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][BPDIDX_STAND_2AXE]].m_pBiped);
						m_ActNum = BPDIDX_STAND_2AXE;
						break;
					}
				}
				else
				{
					// �Ѽչ���
					///2003  �ϵ带 ������ �ν��ϴ°��� ����.
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

// lpd3dDevPick �� �ΰ��� ���ü� �ִ�.
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
	// nate 2004 - 6 : ��ī���� ���� ��Ÿ ���� �÷���
	BYTE byGuildIndex = 0;
	// LOD �Ϸ��� �Ѱǵ�.. �� �ҿ��� ���� �� ����~~~
	BOOL bLow = FALSE;
	// ���� ������ �̵�.
	dwCurtime = timeGetTime();

	//�޸� �������� ������ ������ ���߱� ���ؼ�
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

	// �޸��� ���� ������ �޸��� �� �ƴ���..
	if( g_pRoh == this && m_nMotionState != CHA_RUN )
		pCMyApp->m_bRohRunning = FALSE;

	// �޸��� ������ ���׹̳� �پ����...
	if( g_pRoh == this && pCMyApp->m_bRohRunning ) // �ɸ��� �޸��� �ִٸ�
	{
		int stamina = 0;
		while (dwCurtime >= pCMyApp->m_startRunningTime + 1000) // �޸��� �ִ�..
		{
			pCMyApp->m_startRunningTime += 1000; // ������..
			stamina += 2;
		}

		if (stamina > 0 && g_pTcpIp)
		{
			char strTemp[20] = "";
			sprintf(strTemp, "stamina %d\n", stamina);
			g_pTcpIp->SendNetMessage(strTemp);
		}
	}

	// Vital �� ���ؾ� �ϴµ�, 1�ʰ� ������ �Ⱥ��ϸ�...
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

	/// CanSwapWeapon()�� ���� by.Sol9 [2012.05.09] //////////////
	//if( m_nMotionState == CHA_MAGIC_MOVE )//mungmae-2006/06/21 �������� �߿� ���� �ٲٱ� �ȵǰ� ����.
	//	m_bSwapWeapon = FALSE;
	//else
	//	m_bSwapWeapon = TRUE;
	//////////////////////////////////////////////////////////////////////////

	if( this == g_pRoh &&
			m_nMotionState == CHA_MAGIC &&
			pCMyApp->m_pUIMgr->m_bRClickDown == 1 &&//mungmae-2008/09/01 ������ �� �߿� ��ٿ��� �����ٸ� ����.
			pCMyApp->DrawCasting )
	{
		HellDownFail( g_pRoh, NULL );
	}

	// �� �� ���ȴ�!!
	CNkMob *pMob = NULL;
	CNkCharacter *pCha = NULL;
	//���� �պ��� ��.����
	if (m_AttackMobId >= 0)
	{
		// ���̵�Ƴ� ī�� �������
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
		// ���̵�� ���̰ų� �ϵ��
		else if( ( m_Wearing[WEARING_WEAPON]
				   && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_RING
				   && m_nMotionState != CHA_A_SUMMONFIRE && m_nMotionState != CHA_A_RESURRECTION )
				 || ( m_Wearing[WEARING_SHIELD]
					  && GET_TYPE(m_Wearing[WEARING_SHIELD]) == ITYPE_SUB
					  && GET_SHAPE(m_Wearing[WEARING_SHIELD]) == ISUB_WAND
					  && m_nMotionState == CHA_MAGIC_MOVE ))
		{
			// ���� ����, �����򶧰� �Ǹ� ����!! �ϵ嵵 ����...
			if (m_PrevFrame < m_CurFrame &&	m_PrevFrame < MAGIC_FRAME && m_CurFrame >= MAGIC_FRAME)
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
				if (pMob)
				{
					//CHA_MAGIC_MOVE  // [12/3/2008 Theodoric] �̷� �����ϰ� ����Ʈ�� �ִٴ� ������ ��ȣ ��ġ�� ��Į���Ѥ�;;
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
						// [12/3/2008 Theodoric] ���� ����� ��ų��ȣ�� ������ ��ȣ�� + 1000 �̿���.
						// ���Ⱑ ������ ���� ��ų ��ȣ�� �ߺ� ���ɼ��� �־ ����
						int insertEffectNum = m_Wearing[WEARING_WEAPON]->m_Vnum;
						switch(  insertEffectNum )
						{
						case 2053:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA;
							break; // [12/3/2008 Theodoric]  �縪��
						case 2177:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA2;
							break; // ���� �縪��
						case 2265:
							insertEffectNum = EFFECT_SKILL_RING_RAVEN;
							break; // �߽����� ���̺� ��
						case 2270:
							insertEffectNum = EFFECT_SKILL_RING_NEO;
							break; // �߽����� �׿�������ũ ��
						case 2275:
							insertEffectNum = EFFECT_SKILL_RING_KAISEN;
							break; // �߽����� ī���� ��
						case 2280:
							insertEffectNum = EFFECT_SKILL_RING_SHININGEAGLE;
							break; // �߽����� �Ǵн� ��
						case 2285:
							insertEffectNum = EFFECT_SKILL_RING_SPHINX;
							break; // �߽����� ����ũ�� ��
						case 2292:
							insertEffectNum = EFFECT_SKILL_RING_DRAGON;
							break; // �߽����� �巡�� ��			// �߽��� �� ����Ʈ ���� �ε��� ��Ī ���� ���� by Sol9 [2012.06.01]
						case 2698:
							insertEffectNum = EFFECT_SKILL_RING_BULKAN;
							break;
						case 2747:
							insertEffectNum = EFFECT_SKILL_RING_GRIFFON;
							break;	// 360lv �ٸ����
						case 2924:
							insertEffectNum = EFFECT_SKILL_RING_GRYPHON;
							break;	// ��-�׸���
						case 3288:
							insertEffectNum = EFFECT_SKILL_RING_DEVIL2;
							break;	// �������� & �� ������
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
							break; // ���� ��� ��ų��
						}

						if( m_nMotionState )
							pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, insertEffectNum);
					}
				}

				m_AttackMobId = -1;
			}
		}
		// 2006 �Ž�ų
		else if( m_nMotionState == CHA_H_ELECTRICSHOT			// nate 2005-11-23 : �Ϸ�Ʈ�� �� - InsertMagic
				 || ( m_Wearing[WEARING_WEAPON]
					  && ( m_nMotionState == CHA_K_POISON				// nate 2005-11-10 : ������ - InsertMagic
						   || m_nMotionState == CHA_B_FLYINGCRASH		//mungmae 2005/11/16 : �ö��� ũ���� - ���� �����Ӷ� ����Ʈ �ֱ�
						   || m_nMotionState == CHA_F_SHADOWSLASH )))	//mungmae 2005/11/16 : ������ ������ - InsertMagic
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
			int target_count = 9;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
			float fDist = 0.0f;
			char strTemp[MAX_PATH] = {0,};

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );

				if(pMob && 120 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob ����
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
			int target_count = 6;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
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

				if(pMob && 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob ����
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
			int target_count = 9;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
			float fDist = 0.0f;
			char strTemp[128] = "";

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById( m_AttackMobId );

				if( pMob && 135 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob ����
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
			int target_count = 9;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
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
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob ����
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
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob ����
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
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob ����
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
					&& 40 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz))	// Mob ����
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
		// �̵����� �ι�° Ÿ���� �Ʒ� ��ƾ�� Ÿ�� ���Ѵ�. ���������� �Ҵ�.
		// bpdlist �� ��õ� Attack Frame �� ���Ͽ� attack effect �� �����Ѵ�.
		else if( ( m_nMotionState != CHA_ATTACK
				   && m_nMotionState != CHA_JUMP_ATTACK
				   && m_nMotionState != CHA_ROTATE_ATTACK
				   && m_nMotionState != CHA_THROW_SLAYER
				   && m_nMotionState != CHA_THROW_CHAKRAM
				   && m_nMotionState != CHA_HANDLE )
//					&& m_nMotionState != CHA_B_BURNINGCRASH )	// nate 2005-06-29 : Burning Crash - �ι�° Ÿ�� ��ŵ
				 || ( g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame <= m_CurFrame ) )
		{
			pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
			if (pMob)
			{
				// �����̾� ����Ʈ
				if (m_nMotionState == CHA_THROW_SLAYER)
				{
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_SLAYER);
					pMob->Attacked();
				}
				// ���� ĳ�� ��ũ��
				else if (this == g_pRoh && m_nMotionState == CHA_THROW_CHAKRAM && pCMyApp->m_pUIMgr
						 && pCMyApp->m_pUIMgr->m_ChakramSlotNum >= 0 && m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum] )
				{
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob,m_QuickSlot[pCMyApp->m_pUIMgr->m_ChakramSlotNum]->m_ProtoNum+2000 );//m_Wearing[WEARING_CLOAK]->m_ProtoNum+1000);
				}
				// Ÿ ĳ�� ��ũ��
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
				// �� ������ Effct �߰� (����Ʈ)
				// ���� ������� �Ѵ�.ĳ���� ������� �ϴ� �ڵ�� �Ʒ��ʿ� ����.
				// ĳ���� ���� ���� ó���� �ȴ�. �����϶�.
				else if( m_Wearing[WEARING_WEAPON] )
				{
					if(m_Wearing[WEARING_WEAPON]->m_Vnum == 1238 || m_Wearing[WEARING_WEAPON]->m_Vnum == 2172 )//����ũ��� ����Ʈ
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_DEMICRUSHER);
					// nate 2005-05-20 : Ȧ���δ�
					else if( m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1259	// Ȧ�� ������( ȣũ )
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1260		// Ȧ�� ������( ���� )
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1261		// Ȧ�� ������( ���� )
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 2170		// [2008/11/24 Theodoric] WORLD_MATRIX_2009
							 || m_Wearing[ WEARING_WEAPON ]->m_Vnum == 2171		// [2008/11/24 Theodoric] WORLD_MATRIX_2009
						   )
						pCMyApp->m_pUIMgr->InsertMagic( this, NULL, NULL, pMob, EFFECT_SKILL_HOLLEY );

					// [3/27/2007 Theodoric] �±� ��ũ�� �̺�Ʈ || ���⸦ ����� ���
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1538 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1539 )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_SONGKRAN);
					}


					///��Ʈ�ε� �߰�
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1514 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1515
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1517 ) // ��Ʈ �ε�,���귯�� �ε�
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_HEART);
					}


					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1531 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1532
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1533 ) // �Լ�(ȭ��Ʈ) : ����, ����, ��� ĵ��
					{
						// 2007.03.14 ȭ��Ʈ
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_LIP);
					}

					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1534 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1535 )
						// �Լ�(ȭ��Ʈ), LOVE : ( ���κ��� Ȧ, ���κ��� ĵ�� )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_LIP);
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_LOVE);
					}
					// �̵��� �߰�. Hit Effect
					if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD)
					{
						if( m_nMotionState == CHA_ATTACK )
							pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, NULL, pMob, HIT_DSWORD);
					}
					// �Ϲ� �� , �ܸ�
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
					// ����
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
					// ������
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

			// nate 2005-07-21 : Burning Crash - ���� �̾��
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
		//����:�ֹ��⸦ �� �������⸦ ��� ��ǰ� ȿ���� ���� �־ �������� ������ �ӵ��� ������ �ʰ�
		//	   �ֹ����� �ӵ��� ���󰡰� �Ǿ�, �ð� �����̸� �־� �ӽ������� �������� ������ �ӵ��� ����ϰ�
		//	   ���߾� ���Ҵ�.
		if(m_DoubleAttack)
		{
			if( m_Wearing[WEARING_CLOAK] )
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
				if( pMob )
				{
					switch(m_Wearing[WEARING_CLOAK]->m_Num9)
					{
					case 1://������
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_CHARGECANNON);//������
						break;
					case 2:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_FLAME);//ȭ������
						break;
					case 3:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_AUTOLASER);//������
						break;
					case 4:
						//pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_AUTOLASER);//����������.�켱 ������ ȿ��
						break;
					case 5:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SMALL_ROCKET);
						break;
					case 6:
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_MIDDLE_ROCKET);
						break;
					case 7:	//wan:2005-01	//���ǵ� ���̾�(�ٷ���)
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_SKILL_WARGIANT_MULTI);//���ǵ� ���̾�(�ٷ���)
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

		///2003 ĳ���� Ư������ ������ ȿ�� ��Ÿ����
		if(m_ChaAttackType) //ī��Ʈ
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
			// ���� ����, �����򶧰� �Ǹ� ����!!
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
						// [12/3/2008 Theodoric] ���� ����� ��ų��ȣ�� ������ ��ȣ�� + 1000 �̿���.
						// ���Ⱑ ������ ���� ��ų ��ȣ�� �ߺ� ���ɼ��� �־ ����
						int insertEffectNum = m_Wearing[WEARING_WEAPON]->m_Vnum;
						switch(  insertEffectNum )
						{
						case 2053:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA;
							break; // [12/3/2008 Theodoric]  �縪��
						case 2177:
							insertEffectNum = EFFECT_SKILL_RING_SARNGA2;
							break; // ���� �縪��
						case 2265:
							insertEffectNum = EFFECT_SKILL_RING_RAVEN;
							break; // �߽����� ���̺� ��
						case 2270:
							insertEffectNum = EFFECT_SKILL_RING_NEO;
							break; // �߽����� �׿�������ũ ��
						case 2275:
							insertEffectNum = EFFECT_SKILL_RING_KAISEN;
							break; // �߽����� ī���� ��
						case 2280:
							insertEffectNum = EFFECT_SKILL_RING_SHININGEAGLE;
							break; // �߽����� �Ǵн� ��
						case 2285:
							insertEffectNum = EFFECT_SKILL_RING_SPHINX;
							break; // �߽����� ����ũ�� ��
						case 2292:
							insertEffectNum = EFFECT_SKILL_RING_DRAGON;
							break; // �߽����� �巡�� ��			// �߽��� �� ����Ʈ ���� �ε��� ��Ī ���� ���� by Sol9 [2012.06.01]
						case 2698:
							insertEffectNum = EFFECT_SKILL_RING_BULKAN;
							break;
						case 2747:
							insertEffectNum = EFFECT_SKILL_RING_GRIFFON;
							break;	// 360lv �ٸ����
						case 2924:
							insertEffectNum = EFFECT_SKILL_RING_GRYPHON;
							break;	// ��-�׸���
						case 3288:
							insertEffectNum = EFFECT_SKILL_RING_DEVIL2;
							break;	// �������� & �� ������
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
							break; // ���� ��� ��ų��
						}

						if( m_nMotionState )
							pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, insertEffectNum);

					}
				}
				m_AttackChaId = -1;
			}
		}
		// nate 2005-11-10 : ������ - InsertMagic
		else if( m_nMotionState == CHA_H_ELECTRICSHOT			// nate 2005-11-23 : �Ϸ�Ʈ�� �� - InsertMagic
				 || m_Wearing[WEARING_WEAPON]
				 && ( m_nMotionState == CHA_K_POISON				// nate 2005-11-10 : ������ - InsertMagic
					  || m_nMotionState == CHA_F_SHADOWSLASH		//mungmae 2005/11/16 : ������ ������ - InsertMagic
					  || m_nMotionState == CHA_B_FLYINGCRASH))	//mungmae 2005/11/16 : �ö��� ũ���� - ���� �����Ӷ� ����Ʈ �ֱ�
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
			int target_count = 5;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
			float fDist = 0.0f;
			char strTemp[128] = "";

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );

				if(pCha && 120 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
			int target_count = 2;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
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

				if(pCha && 40 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
				else if(pCha && 40 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
			int target_count = 5;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
			float fDist = 0.0f;
			char strTemp[128] = "";

			if( m_PrevFrame < m_CurFrame
					&& m_PrevFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame
					&& m_CurFrame >= g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame )
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById( m_AttackChaId );

				if(pCha && 135 >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
				else if(pCha && 135 < DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
			int target_count = 5;	// �ֺ� Target ����
			int att_count = 0;		// ���� ��ų�� Target �Ѱ� ����
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
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
				else if(pCha && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
						&& 135 >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
				else if(pCha && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
						&& nSkillDistance >= DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
				else if(pCha && nSkillDistance < DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pCha->m_wx, pCha->m_wz))	// pCha ����
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
				// �� ������ Effct �߰� (����Ʈ)
				// ĳ���� ������� �Ѵ�.
				else if ( m_Wearing[WEARING_WEAPON] )
				{
					// [3/27/2007 Theodoric] �±� ��ũ�� �̺�Ʈ
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1538 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1539 )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_SONGKRAN);
					}

					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1531 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1532
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1533 ) // �Լ�(ȭ��Ʈ) : ����, ����, ��� ĵ��
					{
						// 2007.03.14 ȭ��Ʈ
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_LIP);
					}

					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1534 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1535 )
						// �Լ�(ȭ��Ʈ), LOVE : ( ���κ��� Ȧ, ���κ��� ĵ�� )
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_LIP);
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_LOVE);
					}
					///��Ʈ�ε� �߰�
					if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1514 || m_Wearing[WEARING_WEAPON]->m_Vnum == 1515
							|| m_Wearing[WEARING_WEAPON]->m_Vnum == 1517 ) // ��Ʈ �ε�,���귯�� �ε�
					{
						pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_HEART);
					}


					/* // �Ϻ����� ��û���� �����̾� �ɷ� �ߵ��ø� �ͽ��� �������� ����.
										///�Ϻ����������׽�Ʈ ������ ���󸶻� �� �ͽ� ����Ʈ.
										if( m_Wearing[WEARING_WEAPON]->m_Vnum == 1590 ) // ���󸶻�� ��������...
										{
											pCMyApp->m_pUIMgr->InsertMagic( this, NULL, pCha, NULL, EFFECT_MURAMASA_GHOST );
										}
					*/
					// �̵��� �߰�. Hit Effect
					if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD)
					{
						if( m_nMotionState == CHA_ATTACK )
							pCMyApp->m_pUIMgr->InsertHitEffects(this, NULL, pCha, NULL, HIT_DSWORD);
					}
					// �Ϲ� �� , �ܸ�
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
					// ����
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
					// ������
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
			// nate 2005-07-21 : Burning Crash - ���� �̾��
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
				case 1://������
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_CHARGECANNON);//������
					break;
				case 2:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_FLAME);//ȭ������
					break;
				case 3:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_AUTOLASER);//������
					break;
				case 4:
					//pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, EFFECT_AUTOLASER);//����������.�켱 ������ ȿ��
					break;
				case 5:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SMALL_ROCKET);//��������
					break;
				case 6:
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_MIDDLE_ROCKET);
					break;
				case 7://wan:2005-01	//���ǵ� ���̾�(�ٷ���)
					pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, EFFECT_SKILL_WARGIANT_MULTI);//���ǵ� ���̾�(�ٷ���)
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

		///2003 ĳ���Ͱ� ĳ������ Ư������ ������ ȿ�� ��Ÿ����
		if(m_ChaAttackType) //ī��Ʈ
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

	// ������ ���ǵ带 �ٲ㼭 �������Ѵ� !
	float aniRate = m_AniRate;

	if (m_Wearing[WEARING_WEAPON] ||
			(m_Wearing[WEARING_SHIELD]&& GET_SHAPE(m_Wearing[WEARING_SHIELD])==ISUB_WAND
			 && m_nMotionState == CHA_MAGIC_MOVE))
	{

		if (m_nMotionState == CHA_ATTACK
				|| m_nMotionState == CHA_ROTATE_ATTACK
				|| m_nMotionState == CHA_MAGIC_MOVE)
		{
			int AttackSpeedType = 0; //���н��Կ� �ִ� �ϵ��� ���� ������ ����...

			if(m_Wearing[WEARING_SHIELD] && m_nMotionState == CHA_MAGIC_MOVE)
				AttackSpeedType = m_Wearing[WEARING_SHIELD]->m_Speed;
			else
				AttackSpeedType = m_Wearing[WEARING_WEAPON]->m_Speed;

			//--------------------------------------------------------------------
			// ���� ���ǵ忡 ���� Frame
			// 2005.01.05 / Lyul
			// ���̽�Ʈ ��ų �������� ���
			// �̵���
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

		// ����, ���, �������� �� �Ҷ�, Į, ������ ������ ������ 26 ������¥����.
		// ����Ҷ� �Ǽյ� ������ ��������!
		//-------------------------------------------------------------------------------------
		// ���⿡ ���� �ӵ� ����
		//-------------------------------------------------------------------------------------
		// 2005.01.04 / Lyul �̵��� �߰�
		if (m_nMotionState == CHA_ATTACK
				|| m_nMotionState == CHA_JUMP_ATTACK
				|| m_nMotionState == CHA_ROTATE_ATTACK
				|| m_nMotionState == CHA_THROW_CHAKRAM)
		{
			// ��, ���� , ��ũ��
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
	if(m_CancelState==1)	//���°� �ɷ��ִٸ�,
	{
		if(m_ChaCondition==CONDITION_FREEZE)//�ð��̶��
			aniRate *= 2.0f; //�ӵ� ����.
	}

	// ������ �� �ӵ� ����
	if( m_nMotionState==CHA_SOCIAL && (m_ActNum>=153&&m_ActNum<=158) ) // ������ �� �����̽� ǥ�� üũ.
	{
		aniRate = 0.035f; // �� �� ������
//		aniRate = 0.022f; // �� �� ������
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
		// �̵� �Ÿ� ���.. ��.. ���� �ʿ�...
	}

	dwTimeCap = dwCurtime - m_dwTimeCapPrev;
	m_dwTimeCapPrev = dwCurtime;

	// �̵����̶��
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
			// ������Ʈ�� �ٲپ� �־�� �Ѵ�.
			SetMotionState(CHA_STAND);
			// place ��ɿ� ���� ���꿴���� ������ ��������� �Ѵ�.
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
		// ĳ���͸� ���� ���� �׷��ִ� ���� �����Ƿ�..
		// �װ��� Ȯ�����־�� �Ѵ�.
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
	// nate 2005-08-31 : �� ���� �ۿ��� �����Ǵ� ���׶�����
	//----------------------------------------------------------------------
	// ���� ��ǿ��� ���� ����� �����Ѵ�.
	//----------------------------------------------------------------------
	// ������ ���� �̵��ϴ� ���� ����
	// �� �̰� ���� �帨��.
	if(pCMyApp->m_pUIMgr)
	{
		// Ÿ ĳ���� �������̰ų�, �� ĳ���� �������� �Ƶ�, �뽬, ����� ����, Ÿ�������� ���

		if(( g_pRoh != this
				&& (m_bAttack
					|| m_nMotionState == CHA_F_STONEBASH		// �ٸ� ĳ���� ��ġ�޺� ��� ������ ����.. ///070723 �Ϻ����׸���Ʈ ����
					|| m_nMotionState == CHA_F_BASHREFLECT		// �ٸ� ĳ���� ��ġ�޺� ��� ������ ����.. ///070723 �Ϻ����׸���Ʈ ����
					|| m_nMotionState == CHA_A_SUMMONSQUAD))		// nate 2005-07-15 : Summon Squad - ���� ����
				|| ( g_pRoh != this
					 &&	( m_nMotionState == CHA_B_PROTECTION_SHIELD
						   || m_nMotionState == CHA_F_REFLECTION )
				   )
				|| ( g_pRoh == this
					 && (pCMyApp->m_pUIMgr->m_b_MotionForAttack
						 || m_nMotionState == CHA_F_DASH
						 || m_nMotionState == CHA_F_DSTONE
						 || m_nMotionState == CHA_F_TIMECHARGE
						 || m_nMotionState == CHA_F_REFLECTION		// mungmae-05/04/20 ���÷��� �߰�
						 || m_nMotionState == CHA_A_SUMMONSQUAD		// nate 2005-07-15 : Summon Squad - ���� ����
						 || m_nMotionState == CHA_H_AUTOMINE
						 || m_nMotionState == CHA_SPIRITMODE
						 || m_nMotionState == CHA_A_RESURRECTION // PK�� ��� �׾����� ���� ĵ�������� �������ǿ� ����ó�� �߰�. ///07-05-31 ���� �߰�
						 || m_nMotionState == CHA_B_PROTECTION_SHIELD
						))
				|| m_nMotionState == LHCha_Action__Perom_S10 // �ʵ�?
				|| m_nMotionState == LHCha_Action__Perom_S15 // �ʵ�?
		  )
		{
			if ( m_Biped && (m_CurFrame+frame_count) >= m_Biped->m_AnimCount)
			{
				// m_MultiShot : �޸� ����
				if (m_MultiShot < 0
						|| m_RenderMultiShot < 0)
					SetSnifing(FALSE);

				// ĳ���� �÷��̾��� ���� �Ǵ� �� ��� ����
				if (g_pRoh == this)
				{
					// ���� ���� �ƴϰ� ������ ���� �̵��� �ƴ϶�� STAND
					if( !pCMyApp->CompareHeroAttGo()
							&& !pCMyApp->m_pUIMgr->m_b_MovingForAttack	)
					{
						// m_b_MovingForAttack�� false �̾�� ������ ��ƾ �ƴѰ�?
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
				// ĳ���� �ٸ� ������� ������ STAND
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
				m_wy -= m_DeadFrame * (500.0f / 100); // 100 Frame(2��) ���� 1000.0f�� ���� �ɴ´�.
			else
				m_wy -= 1000.0f;
		*/
	}
	else if (m_DeadFrame > 60)
	{
		//mungmae-2005/06/23 ����콺 ���� �ð� ����
		if(g_pRoh == this && !g_pRoh->m_bNowFreePVP ) // ������� �� ������ �˾� �ȶ���. ///�������
		{
			m_dwReturnTime = timeGetTime();

			if( !pCMyApp->m_pUIMgr->m_dwRevivalAccTime )
				pCMyApp->m_pUIMgr->m_dwRevivalAccTime = timeGetTime();

			if( ( pCMyApp->m_nCurWorldIdx != WORLD_MATRIX
					&& pCMyApp->m_nCurWorldIdx != WORLD_MATRIX_2009
				) // ����콺 ���� �ƴ� ���
					|| ( (pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
						  || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
						 )
						 && g_pRoh->m_GuildLevel == 10 )	//����콺 ���̰� ������
					|| ( (pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
						  || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
						 )
						 && !pCMyApp->m_pUIMgr->m_bOnBattle) )		//����콺 ���̰� ����콺 ���ϰ� ���� ��
			{
				if (g_pNk2DFrame)
				{
					//	g_pTcpIp->SendNetMessage("revival\n");
					// IDS_30S_AFTER_REVIVAL "���� ��Ȱ �Ͻðڽ��ϱ�? 30���Ŀ� �ڵ����� ��ȯ�մϴ�."
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_30S_AFTER_REVIVAL), TYPE_NOR_OK, 35 );
				}
				else
					m_Vital = m_MaxVital;

				m_DeadFrame = -1;
				pCMyApp->m_pUIMgr->m_dwRevivalAccTime = 0; // Ȥ�� �����ٰ� �������� �� ������ �ʱ�ȭ.
				m_bDrawRegenbar = FALSE;
			}

			else if( pCMyApp->m_pUIMgr->m_dwRevivalAccTime
					 && pCMyApp->m_pUIMgr->m_bOnBattle			//����콺 �ϰ� ���� ��
					 && (pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
						 || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
						)
					 && g_pRoh->m_GuildLevel != 10 )    //�����ڰ� �ƴҶ�
			{
				m_dwRegenTime = timeGetTime() - pCMyApp->m_pUIMgr->m_dwRevivalAccTime;
				m_bDrawRegenbar = TRUE;

				if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX )
				{
					if( m_Level >= 300 ) //������ 300�� �ѳ�? �ƴϳ�?
					{
						if( MatrixTeamGrade(m_GuildIndex) == 1)		//�׾��� �� �� ������ 1���� ��
							m_nRegenIndex = 10 + 20;
						else if( MatrixTeamGrade(m_GuildIndex) == 2)//�׾��� �� �� ������ 2���� ��
							m_nRegenIndex = 10 + 10;
						else if( MatrixTeamGrade(m_GuildIndex) == 3)//�׾��� �� �� ������ 3���� ��
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
					if( m_Level >= 300 ) //������ 300�� �ѳ�? �ƴϳ�?
					{
						if( MatrixTeamGrade(m_GuildIndex) == 1)//�׾��� �� �� ������ 1���� ��
							m_nRegenIndex = 10 + 10;
						else if( MatrixTeamGrade(m_GuildIndex) == 2)//�׾��� �� �� ������ 2���� ��
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

				//�� ��(m_nRegenIndex * 1000)�� ������ �� ��Ƴ����� �Ѵ�.
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
		//		m_wy -= (m_DeadFrame-60) * (6.0f / 40); // 60 Frame(3��) �ĺ��� 40 Frame(2��) ���� 6.0f�� ���� �ɴ´�.
	}

	if( g_pTcpIp && m_dwReturnTime && timeGetTime() - m_dwReturnTime > 30000 )
	{
		g_pTcpIp->SendNetMessage("revival\n");
		m_dwReturnTime = 0;
		g_pNk2DFrame->DeletePopup();
	}

	float	swim_height, fWaterHeight;
	// ���� ����
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

	// 90.0f ���� ������ ��...
	pCMyApp->SetHeroSwimming(FALSE);

	// �̵���
	if ((m_nMotionState == CHA_WALK || m_nMotionState == CHA_RUN) && m_wy < swim_height)
		SetMotionState(CHA_SWIM);
	else if (m_nMotionState == CHA_SWIM && m_wy >= swim_height)
		SetMotionState(CHA_WALK);

	// ���� ����
	if (m_nMotionState == CHA_STAND && m_wy < swim_height)
		SetMotionState(CHA_SWIM_STAND);
	else if (m_nMotionState == CHA_SWIM_STAND && m_wy >= swim_height)
		SetMotionState(CHA_STAND);

	// �ɸ��� ���߿� ���� ��
	if (m_nMotionState == CHA_SWIM || m_nMotionState == CHA_SWIM_STAND)
	{
		if (pCMyApp->m_nCurWorldIdx != WORLD_SKY)
			m_wy = fWaterHeight;

		pCMyApp->SetHeroSwimming(TRUE);
	}

	// SKY �������� �ϴ� ������ ������Ʈ���带 ź��!
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

	// ��������� ������� ó���� �߰��� ��������.
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
					// ��Ȯ�� �������� ������~~!
					m_toDir = (float) atan2(pCha->m_wz - m_wz, pCha->m_wx - m_wx);

					if (m_nMotionState == CHA_JUMP_ATTACK
							|| m_nMotionState == CHA_ROTATE_ATTACK)
						pCha->Attacked();
					else
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, pCha, NULL, m_MagicNum); // ���� m_MagicNum �� -1�϶� ������ ��찡 �ִ�.(06-04-28 ����)
				}

				m_MagicChaId[i] = -1;
			}// End of For

			for ( int i = 0; i < m_MagicMobCount; i++)
			{
				CNkMob *pMob = pCMyApp->m_pUIMgr->FindMobById(m_MagicMobId[i]);
				if (pMob)
				{
					// ��Ȯ�� �������� ������~~!
					m_toDir = (float) atan2(pMob->m_wz - m_wz, pMob->m_wx - m_wx);

					if( m_nMotionState == CHA_JUMP_ATTACK
							|| m_nMotionState == CHA_ROTATE_ATTACK )
						pMob->Attacked();
					else if( m_MagicNum != SKILL_VANPIRE_TOUCH )
						pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, pMob, m_MagicNum); // ���� m_MagicNum �� -1�϶� ������ ��찡 �ִ�.(06-04-28 ����)
				}

				m_MagicMobId[i] = -1;
			}// End of For

			m_MagicChaCount = 0;
			m_MagicMobCount = 0;
		}
	}

	// �� ����... ���� Ʈ������ �̸� �������!
	if( bVisible
			|| ( m_nMotionState == CHA_ATTACK
				 && m_Wearing[WEARING_WEAPON]
				 && GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN
				 && g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame <= m_CurFrame
				 && m_CurFrame < g_ChaBpdList[g_ChaBpdIndex[m_BpdRace][m_ActNum]].m_AttackFrame + 3 ) )
	{
		// ���� Ʈ������. - ĳ���͸� �׸��� ����.
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
		D3DMATRIX matHideRotate;

		if (m_HideStatus == HIDE_HIDING)
		{
			if (dwCurtime < m_HideTime + HIDE_HIDING_TIME)   // 1�ʰ� ����������...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime) / HIDE_HIDING_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
			}
			else if (dwCurtime < m_HideTime + HIDE_HIDING_TIME + HIDE_DELAY_TIME)     // 2�ʰ� ����������...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime - HIDE_HIDING_TIME) / HIDE_DELAY_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
				m_wy -= hide_deep[m_Sex] * (dwCurtime - m_HideTime - HIDE_HIDING_TIME) / HIDE_DELAY_TIME;
			}
			else     // 2�ʰ� ��������...
			{
				m_HideStatus = HIDE_HIDDEN;
			}
		}
		else if (m_HideStatus == HIDE_RISING)
		{
			if (dwCurtime < m_HideTime + HIDE_RISING_TIME)   // 0.3�ʰ� ����������...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime) / HIDE_RISING_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
				m_wy -= hide_deep[m_Sex] - (hide_deep[m_Sex] * (dwCurtime - m_HideTime) / HIDE_RISING_TIME);
			}
			else if (dwCurtime < m_HideTime + HIDE_RISING_TIME + HIDE_DELAY_TIME)     // 1.3�ʰ� ����������...
			{
				Nk3DUtil_SetRotateCCWYMatrix( matHideRotate, g_2_PI * (dwCurtime - m_HideTime - HIDE_RISING_TIME) / HIDE_DELAY_TIME);
				D3DMath_MatrixMultiply( matRotate, matRotate, matHideRotate );
			}
			else     // 1.3�ʰ� ��������...
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

	// ���� ������ ���
	FrameMove(frame_count, bVisible, pMob, pCha);

	m_bVisible = bVisible; // �������� �ø��󿡼� ���̴°��� �÷���. 06-01-03 ���� �߰�(���λ��� ��ĭ ������ �ʿ�����.)

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

	// �׸��� ������...
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
						// �׸��� ���ƶ�!!
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
						if (i == WEARING_SHIELD)	// ������ ���� ������忡���� �׸���.
						{
							if( g_dwClientCountry == CTRY_JPN )
							{
								if ( ( m_bCombat
										&& m_nMotionState != CHA_SOCIAL
										&& m_nMotionState != CHA_SWIM
										&& m_nMotionState != CHA_SWIM_STAND)

										//  [6/23/2009 ppmmjj83] �Ϻ��� ��ȭ����϶� ���кκе� �׷������ ��û��
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
							// �Ѻ�����,��Ÿ������ �Ծ��� ���� �Ź��� �׸��� ���ƶ�~~~

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

	// ���°� ����
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &s_dwAlphaTest );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &s_dwAlphaBlend );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &s_dwSrcBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &s_dwDestBlend);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &m_dwAmbient);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &s_dwLighting );

	// CCapsyong�� ���
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// [3/13/2008 �����] ���⿡ ���� ���� �ɼ� ����ǰ� ���� ������Ʈ ���� ����(�Ϻ� �� ������)
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAREF, 0x0000001);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );


	// ���� �������ΰ� Ȯ���� �Ѵ�.
	if( pCMyApp->m_nCurWorldIdx != WORLD_MATRIX
			&& m_bCrystalOwner )
	{
		D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy + 30, m_wz);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
		D3DMath_MatrixMultiply( matTrans, matRotate, matTrans );	// matTrans�� temp�� ��.

		if( CommonConfig::Instance()->GetSiegeCrystalScale() == true )
		{
			D3DMATRIX matTemp;
			D3DUtil_SetScaleMatrix(matTemp, 2.0f, 2.0f, 2.0f);		// ���� ũ����Ż ũ�� ���� ��û
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
	// nate 2005-05-06 : ��Ʈ���� �Ӹ��� ������ ��ũ
	else if( ( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
			   ||  pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
			 )
			 && m_GuildGradeIndex == 10
			 && m_GuildIndex && m_bShowMatrixMark)
	{
		D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy + 25, m_wz);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, timeGetTime() * 0.0005f );
		D3DMath_MatrixMultiply( matTrans, matRotate, matTrans );	// matTrans�� temp�� ��.
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

	///2003  ĳ���� ���¿� �ɷ������� ȿ���� ��.(�ð�,�����...)���� ����.
	if(m_CancelState==1)	//���°� �ɷ��ִٸ�
	{
		if( m_ShowConditionEffect == FALSE )	// ȿ���� �׷����� �ִ���...�˻�...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_CONDITION);
			m_ShowConditionEffect = TRUE;
		}
	}

	if(m_CancelState2==1)	//���°� �ɷ��ִٸ�
	{
		if( m_ShowConditionEffect2 == FALSE )	// ȿ���� �׷����� �ִ���...�˻�...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_CONDITION2);
			m_ShowConditionEffect2 = TRUE;
		}
	}

	///2003 ,��ų�� �ɷ������� ȿ���� ��(�ɼǽ�ų)
	if(m_InChaSkill)//�ɼǽ�ų�� �ɷ��ִٸ�
	{
		if( m_ShowSkillEffect == FALSE )
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, this, NULL, EFFECT_OPTIONSKILL);

			if(m_InChaSkill != SKILL_BLESSING)
				pCMyApp->m_pUIMgr->InsertMagic(this, NULL, NULL, NULL, EFFECT_SKILL_SPELL);

			m_ShowSkillEffect = TRUE;
		}
	}

	if(m_InChaAidiaSkill)	//���̵�� �ɼǽ�ų�� �ɷ��ִٸ�
	{
		if ( m_InChaAidiaSkill == SKILL_JOINT_LIFE ||
				m_InChaAidiaSkill == SKILL_LIFE_LIGHT ||
				m_InChaAidiaSkill == SKILL_MELODY_OF_DEF ||
				m_InChaAidiaSkill == SKILL_MELODY_OF_GUD )
		{
			if( m_ShowAidiaSkillEffect == FALSE )	// ȿ���� �׷����� �ִ���...�˻�...
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


	if(m_bMoonEventEffect)	//0213 �뺸�� �̺�Ʈ ȿ��.
	{
		if(m_ShowMoonEventEffect == FALSE)
		{
			//��⿡ ȿ�� �ֱ�
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

	///2003  å������ ������ �������� ȿ���� ��.
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
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//=============================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_A_TEAM_TEX ] ), FALSE);
					//=============================================
				}
				else if (m_nTeamMemGrade == TEAM_BATTLE_BMEMBER || m_nTeamMemGrade == TEAM_BATTLE_BHEAD)
				{
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//=============================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_B_TEAM_TEX ] ), FALSE);
					//=============================================
				}
				else if (m_nTeamMemGrade == TEAM_BATTLE_AFLAG || m_nTeamMemGrade == TEAM_BATTLE_BFLAG)
				{
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//=============================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FLAG_OWN_TEX ] ), FALSE);
					//=============================================
				}
				else if(m_nTeamMemGrade == TEAM_BATTLE_GUILDPK )
				{
					if( g_pRoh->m_bGuildPK ) //���ڽ��� ������϶�
					{
						if(m_pstrGuildName)
						{
							if(strcmp(g_pRoh->m_sEnemyGName,m_pstrGuildName)==0)// �ڽ��� ����� �� ����̸��� ������
							{
								//�� ����
								// nate 2004 - 7 �۷ι� ShadowRect���
								g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
								//=============================================
								// nate 2004 - 7 : Image Manager
								// nate 2004 - 7 �۷ι� ShadowRect���
								g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_A_GUILD_TEX ] ), FALSE);
								//=============================================
							}
							else if(strcmp(g_pRoh->m_pstrGuildName,m_pstrGuildName)==0) //�ڽ��� ����̸��� ������
							{
								// ������ �Ķ���
								// nate 2004 - 7 �۷ι� ShadowRect���
								g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
								//=============================================
								// nate 2004 - 7 : Image Manager
								// nate 2004 - 7 �۷ι� ShadowRect���
								g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_B_GUILD_TEX ] ), FALSE);
								//=============================================
							}
							else
							{
								// ����� ���϶� �ٸ� ��尡 ��������̸� �����
								// nate 2004 - 7 �۷ι� ShadowRect���
								g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
								//=============================================
								// nate 2004 - 7 : Image Manager
								// nate 2004 - 7 �۷ι� ShadowRect���
								g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_C_GUILD_TEX ] ), FALSE);
								//=============================================
							}
						}
					}
					else	//�ڽ��� ��������� �ƴҶ� => ������̸� ������� ĳ����  �����
					{
						// nate 2004 - 7 �۷ι� ShadowRect���
						g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
						//========================================
						// nate 2004 - 7 : Image Manager
						// nate 2004 - 7 �۷ι� ShadowRect���
						g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_C_GUILD_TEX ] ), FALSE);
						//========================================
					}
				}
				else if ( m_bMyParty)	// ���� ��Ƽ���̸� �׸��� ����� �ٸ��� ǥ�����ش�.
				{
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//========================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_PARTY_TEX ] ), FALSE);
					//========================================
				}
				else if ( pCMyApp->m_nCurWorldIdx != WORLD_MATRIX && m_bMyGuild)
				{
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//========================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_GUILDT_TEX ] ), FALSE);
					//========================================
				}
				else if (m_bOppGuild)
				{
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 20, FALSE);
					//========================================
					// nate 2004 - 7 : Image Manager
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_OPPGUILD_TEX ] ), FALSE);
					//========================================
				}
				else
				{
					// nate 2004 - 7 �۷ι� ShadowRect���
					g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 15, FALSE);
					g_pShadowRect->Render(NULL, FALSE);
				}
			}
		}
//======================================================================
		// nate 2004 - 6 : ��ī���� ĳ���� �� ���� - �ٴ� �׸���
		// ��Ĵ���� plane �ڿ����� ��� ĳ���� �ٴ��� �׸���.
		if( pCMyApp->m_pUIMgr->m_pNamPlane
				&& m_pstrGuildName
				&& GetMotionState() != CHA_SWIM_STAND
				&& GetMotionState() != CHA_SWIM )
		{
			int nGuildNameLen = strlen( m_pstrGuildName );

			// ����
			if( pCMyApp->m_pUIMgr )
			{
				if( pCMyApp->m_pUIMgr->m_szDecardiGuildGamma != NULL
						|| pCMyApp->m_pUIMgr->m_szDecardiGuildDelta != NULL )
				{
					if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_szDecardiGuildGamma ) )
						byGuildIndex = 1;
					// ��Ÿ
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
		// nate 2005-03-21 : Matrix - �ٴ� ��ũ ����
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

	// nate 2005-11-14 : ������ - ��������� SetAmbient
	if( m_bPoisoning )
	{
		if( bMouseOn )
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x0044ff44 );
		else
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x00008833 );
	}
	// nate 2004 - 6 : ��ī���� ĳ���� �� ĳ���ͻ��� ����
	else if( byGuildIndex == 1)
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x008888ff );
	else if( byGuildIndex == 2 )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x00ff9999 );
	else if( g_pRoh &&
			 g_pRoh->GetMyGuildData() &&
			 (g_pRoh->GetMyGuildData()->IsInvasion_Defense() || g_pRoh->GetMyGuildData()->IsInvasion_Attack() )  &&
			 g_pRoh->GetMyGuildData()->CheckInvasionGuild( this->m_GuildIndex))
	{
		// [6/20/2007 Theodoric] ħ���� :: ħ���� ����� �������� ī��ó�� ǥ���Ѵ�.
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
	else if( m_Chaotic_Grade )	// �Ϲ����� �ƴҶ��� ����~~~
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

	// ������� ������ ǥ���ϱ� ���� ������ ȿ�� ///�������
	if( g_pRoh ) // �α��� ���� ���� ��쿡 ���ü��� �����Ƿ� üũ!
	{
		if( g_pRoh->m_bNowFreePVP && timeGetTime()-g_pRoh->m_dwRingCreateTick < 5000 && this->m_nCharIndex == g_pRoh->m_dwPvpDestIndex ) // ��� ī��Ʈ ���� ������ �����̰�.
		{
			if( (timeGetTime()/400) % 2 )
			{
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
				GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
			}
		}
		if( g_pRoh->m_bNowFreePVP && this != g_pRoh && this->m_nCharIndex != g_pRoh->m_dwPvpDestIndex ) // �ڱⰡ ������̰�, ���� �׸��� �༮�� �� 3���̸�..
		{
			// �ڽ��� ������� ���̸� �ڱ�� ��� �̿ܿ� ��� ������.
			// ������.
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		}
		if( !g_pRoh->m_bNowFreePVP && this->m_bNowFreePVP ) // �ڽ��� ����������� �ƴѵ� �̳༮�� ������� ���γ༮�̸�...
		{
			// �ڽ��� ����������� �ƴϸ� ��������� �༮�� ��� ������.
			// ������.
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		}
	}

	//----------------------------------------------------------------------
	// ĳ���� �����ϰ� �ִ� ��� �׸���.
	//----------------------------------------------------------------------
	memset(m_bEventWearRendered, 0, sizeof(m_bEventWearRendered));
	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	{
		if (m_Wearing[i])
		{
			// ����, ����
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
			// ��Ÿ
			else if (GET_CSY(m_Wearing[i], m_BpdRace))
			{
				//----------------------------------------------------------------
				// ������ ��쿡
				if (i == WEARING_SHIELD)
				{
					if( m_nMotionState != CHA_SOCIAL &&
						m_nMotionState != CHA_SWIM &&
						m_nMotionState != CHA_SWIM_STAND )
					{
						// ������ �� ������ �ϵ�
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
							// ������ ȿ��
							GET_CSY(m_Wearing[WEARING_SHIELD], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow, lpTexture, m_CurFrame);
						}
						else
						{
							// �Ϲ� ����
							if( !EventWearingRender(i) ) // �̺�Ʈ ������ �ִٸ� �̺�Ʈ ������ ����Ѵ�. ///�̺�Ʈ��������
							{
								if( IsAlphaWear(i, false) == FALSE )
								{
									GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
								}

								if( m_Wearing[i]->m_Vnum == 1588 ) ///�Ϻ����������׽�Ʈ ���Ϸ��� ���ж��..
								{
									// ����Ʈ�� ���⼭ ���� ó�� ���ش�.
									DrawInstanceEffect(  1588, this, GET_CSY(m_Wearing[i], m_BpdRace) );
								}
								else if( m_Wearing[i]->m_Vnum == 1589 ) ///�Ϻ����������׽�Ʈ ���
								{
									// ����Ʈ�� ���⼭ ���� ó�� ���ش�.
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
						if( m_EventWearing[WEARING_PANTS] && m_bEventWearFlag ) // �̺�Ʈ ������ �����ϰ� �ְ�...
						{
							if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // ���� �̺�Ʈ ������ ��� �׸��� �������.
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
				// �����̾�
				else if (i == WEARING_CLOAK
						 && ( ( m_nMotionState == CHA_THROW_SLAYER
								&& m_CurFrame >= THROW_SLAYER_FRAME_START
								&& m_CurFrame <= THROW_SLAYER_FRAME_END )							  
							  || m_nMotionState == CHA_B_BURNINGCRASH ) )
				{
					// SubCsy �� Render �Ѵ�(� �� �� ���  �տ� �� ���� Render )
					if (GET_SUB_CSY(m_Wearing[i]))
						GET_SUB_CSY(m_Wearing[i])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
				}
				//--------------------------------------------------------------
				// ����� ���� - Lyul_2005
				else if (i == WEARING_CLOAK && m_Wearing[WEARING_CLOAK] && GET_SHAPE(m_Wearing[WEARING_CLOAK]) == ISUB_DSTONE)
				{
					if(m_nMotionState == CHA_F_DSTONE)
					{
						// �տ� �� ���� �׸�
						if(m_CurFrame >= 9 && m_CurFrame <= DSTRONE_SLAYER_FRAME_END && !m_bstone)
						{
							// SubCsy �� Frame move �Ѵ�(� �� �� ���  �տ� �� ���� Render )
							if (GET_SUB_CSY(m_Wearing[i]))
								GET_SUB_CSY(m_Wearing[i])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
						}
						// � �� ���� �׸�
						else if(!m_bstone)
						{
							if( IsAlphaWear(i, false) == FALSE )
							{
								GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
							}
						}
					}
					// � �� ���� �׸�
					else if(!m_bstone)
					{
						if( !EventWearingRender(i) ) // �̺�Ʈ ������ �ִٸ� �̺�Ʈ ������ ����Ѵ�. ///�̺�Ʈ��������
						{
							if( IsAlphaWear(i, false) == FALSE )
							{
								GET_CSY(m_Wearing[i], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[i],m_BpdRace), FALSE, bLow);
							}
						}
					}
				}
				else if( !m_Chaotic_Grade && // ī���� ����Ʈ�� �Կ��ش�. �����ε� �̷������� ����Ʈ�� �̿��� ���� ������ ������ ���ܴ� ���⿡ �־�����Ѵ�.
						 !( g_bUseEventWear && m_EventWearing[i] && m_bEventWearFlag ) && // �̺�Ʈ ��� �����ϰ��ִٸ� ����Ʈ ���� üũ�� ����. ///070723 �Ϻ����׸���Ʈ ����
						 ( IS_LIGHT_OFF(m_Wearing[i]) ) // [6/19/2008 �����] �ǻ� �����۵��� �׸��� ���� ���� üũ
					   ) // ����Ʈ�� ���� �׸� ������ ���
				{
					GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

					if( !EventWearingRender(i) ) // �̺�Ʈ ������ �ִٸ� �̺�Ʈ ������ ����Ѵ�. ///�̺�Ʈ��������
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
				// �Ϲ� Render
				else
				{
					// �̺�Ʈ ������ �ִٸ� �̺�Ʈ ������ ����Ѵ�. ///�̺�Ʈ��������
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
		else if( m_EventWearing[i] && m_bEventWearFlag && ( i != WEARING_RING ) ) // ���� ��� �������־ �̺�Ʈ ������ �����ϰ��ִ� ��츦 üũ�Ѵ�. ///�̺�Ʈ��������
		{
			EventWearingRender(i); // �ٷ� �׸��� �ȴ�.
		}
		else if (m_Race == RACE_AIDIA && i == WEARING_CLOAK && g_BasicWingsProto >= 0)
		{
			if( GET_CSYC(g_BasicWingsProto, m_BpdRace) )
				GET_CSYC(g_BasicWingsProto, m_BpdRace)->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
		}
	}	

	// ������ �׸���...
	for (int i = 0; i < CAPSYONG_NUM; i++)
	{
		if (m_bShow[i] && m_CapSyong[i])//ooo
		{
			///�̺�Ʈ��������
			if( g_bUseEventWear )
			{
				// ���� �����ؼ� ũ�� ������.
				switch( i )
				{
				case NKCHA_HAIR:
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

					if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}						
					}
					else if( m_Wearing[WEARING_HELMET] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_HAIR) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}						
					}
					else // �Ѵ� �������ϰ� �ִٸ�...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

					break;

				case NKCHA_FACE:

					if( m_EventWearing[WEARING_HELMET] && m_bEventWearFlag  ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_HELMET] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_HELMET]) & ARMOR_REMOVE_FACE) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // �Ѵ� �������ϰ� �ִٸ�...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_BREAST:

					if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_ARMOR] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_BODY) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // �Ѵ� �������ϰ� �ִٸ�...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_ARMS:

					if( m_EventWearing[WEARING_ARMOR] && m_bEventWearFlag  ) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_ARMOR] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_ARMOR]) & ARMOR_REMOVE_ARMS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // �Ѵ� �������ϰ� �ִٸ�...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_LEGS:

					if( m_EventWearing[WEARING_PANTS] && m_bEventWearFlag) // �켱�� �̺�Ʈ ��� �ִ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else if( m_Wearing[WEARING_PANTS] ) // �̺�Ʈ ��� ���ٸ� �Ϲ� ��� üũ.
					{
						if( !(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_LEGS) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
						}
					}
					else // �Ѵ� �������ϰ� �ִٸ�...
					{
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}

					break;

				case NKCHA_FEET: // �̳༮�� ������ ���� �����ϴ�.

					if( (m_EventWearing[WEARING_PANTS] || m_EventWearing[WEARING_SHOES]) && m_bEventWearFlag )// �̺�Ʈ ������ �ִ°��.
					{
						if( m_EventWearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							break; // �ȱ׸��� ����.
						}

						if( m_EventWearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_EventWearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							break; // �ȱ׸��� ����.
						}

						// �̺�Ʈ ������ �԰� �ִµ� �׸��� ������ ����.
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}
					else if( m_Wearing[WEARING_PANTS] || m_Wearing[WEARING_SHOES] ) // �̺�Ʈ ������ �Ѵ� ���� ��� �Ϲ� ������ üũ�Ѵ�.
					{
						if( m_Wearing[WEARING_PANTS]&&(GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							break; // �ȱ׸��� ����.
						}

						if( m_Wearing[WEARING_SHOES]&&(GET_RENDER_FLAG(m_Wearing[WEARING_SHOES]) & ARMOR_REMOVE_FEET) ) // �׷����ϴ� �������� �����ϰ� �ִ� ���.
						{
							break; // �ȱ׸��� ����.
						}

						// �̺�Ʈ ������ �԰� �ִµ� �׸��� ������ ����.
						m_CapSyong[i]->Render(GET_D3DDEVICE(), -1, false, bLow);
					}
					else // �ƹ��͵� ���԰��־ �׸���.
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
					// �׸��� ���ƶ�!!
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
		// 567 == ��ũ��::item->m_ProtoNum
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
			// ��ũ�� �����̴�! � ���� �׸��� ���� �տ� ��ɷ� �׷���!
			// 2005.05.11
			// Lyul / ���� �Ϸ�
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
		&& m_nMotionState != CHA_F_DSTONE		// Lyul_2005 ����ν����� ��쿡�� �����.
		&& m_nMotionState != CHA_B_BURNINGCRASH	// nate 2005-06-18 : Burning Crash - ���� ũ���� ���� �ɱ�
		&& m_nMotionState != CHA_H_AUTOMINE
		&& m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : �Ϸ�Ʈ�� ��
		&& m_EventWearing[WEARING_WEAPON]
		&& GET_CSY( m_EventWearing[WEARING_WEAPON], m_BpdRace ) )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = NULL;
		if( GET_CSY(m_EventWearing[WEARING_WEAPON], m_BpdRace) && m_nMotionState != CHA_B_BURNINGCRASH )
			GET_CSY(m_EventWearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_EventWearing[WEARING_WEAPON],m_BpdRace), FALSE, bLow, lpTexture, m_CurFrame);

		// �Ϲ� �̵��� Render
		if( GET_SHAPE(m_EventWearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && GET_SUB_CSY(m_EventWearing[WEARING_WEAPON]) )
			GET_SUB_CSY(m_EventWearing[WEARING_WEAPON])->Render(GET_D3DDEVICE(), -1, FALSE, bLow);
	}
	else if (m_bCombat
			&& m_nMotionState != CHA_SOCIAL
			&& m_nMotionState != CHA_SWIM
			&& m_nMotionState != CHA_SWIM_STAND
			&& m_nMotionState != CHA_THROW_SLAYER
			&& m_nMotionState != CHA_THROW_CHAKRAM
			&& m_nMotionState != CHA_F_DSTONE		// Lyul_2005 ����ν����� ��쿡�� �����.
			&& m_nMotionState != CHA_B_BURNINGCRASH	// nate 2005-06-18 : Burning Crash - ���� ũ���� ���� �ɱ�
			&& m_nMotionState != CHA_H_AUTOMINE
			&& m_nMotionState != CHA_H_ELECTRICSHOT	// nate 2005-11-24 : �Ϸ�Ʈ�� ��
			&& m_Wearing[WEARING_WEAPON]
			&& GET_CSY( m_Wearing[WEARING_WEAPON], m_BpdRace ) )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = NULL;
		//--------------------------------------------------------------------
		// �̵��� ����(Bleed) Render
		if(GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD && m_bBleed)
		{
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_NKCHARACTER_LINE3]);
			GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, lpTexture, m_CurFrame,1);

			if( GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) )
				GET_SUB_CSY(m_Wearing[WEARING_WEAPON])->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, lpTexture, m_CurFrame,1);
		}
		// nate 2005-06-15 : Burning Crash - �����̾ �� ���̱�
		else if( m_Wearing[ WEARING_CLOAK ]
				 && m_nMotionState == CHA_B_BURNINGCRASH
				 && (( m_CurFrame >= 72 && m_CurFrame <= 84 )
					 || m_CurFrame >= 112 ) )
		{
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );
			GET_SUB_CSY( m_Wearing[ WEARING_CLOAK ] )->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, lpTexture, m_CurFrame, 1 );
		}
		// �Ϲ� ���� Render
		else
		{
			// nate 2005-06-10 : Burning Crash - ����ũ���������� �Ϲݹ���� �� �׷�
			if( GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace) && m_nMotionState != CHA_B_BURNINGCRASH )
				GET_CSY(m_Wearing[WEARING_WEAPON], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(m_Wearing[WEARING_WEAPON],m_BpdRace), FALSE, bLow, lpTexture, m_CurFrame);

			// �Ϲ� �̵��� Render
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
			// Į ���� �� �����Ѵ�. Lyul_2005 �̵��� �߰�
			if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD
					|| GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE
					|| GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER )
				m_DrawWeaponEffects = TRACELINE_ATTACK;
			// ����
			else if(m_bBleed)// && ����
				m_DrawWeaponEffects = TRACELINE_ATTACK_BLEED;
			// �̵��� �Ϲ�.
			else if(GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD )
				m_DrawWeaponEffects = TRACELINE_ATTACK;
			else
				m_DrawWeaponEffects = NONE;
		}
		else if ( m_nMotionState == CHA_JUMP_ATTACK )
			m_DrawWeaponEffects = TRACELINE_JUMPATTACK;
		else if ( m_nMotionState == CHA_ROTATE_ATTACK )
			m_DrawWeaponEffects = TRACELINE_ROTATEATTACK;
		else if( m_nMotionState == CHA_F_DASHSLASH ) ///�����޺�
			m_DrawWeaponEffects = TRACELINE_DASHSLASH;
		else if( m_nMotionState == CHA_F_SLASHSHADOW ) ///�����޺�
			m_DrawWeaponEffects = TRACELINE_SLASHSHADOW;
		else
			m_DrawWeaponEffects = NONE;

		// nate 2004 - 8
		// �Ź��� ���� - ����Ʈ ����������

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

	//  [5/6/2009 ppmmjj83] �Ϻ� �������� �������� Į�� Į���� ���� ������� ó���� ����ϴ� �κ��� ����
	else if( !m_bCombat
			 && m_Wearing[WEARING_WEAPON]
			 && (m_Wearing[WEARING_WEAPON]->m_Vnum == 1590 || m_Wearing[WEARING_WEAPON]->m_Vnum == 2694)
		   )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );;

		int ProtoNum	= g_ItemList.FindItem( 1712 ); // ���󸶻� ���� ĸ����.
		int	TextrNum	= g_ItemList.m_ItemArray[ProtoNum].m_TextrNum[m_BpdRace];

		if( GET_CSYC(ProtoNum, m_BpdRace) )
			GET_CSYC(ProtoNum, m_BpdRace)->Render(GET_D3DDEVICE(), TextrNum, FALSE, bLow, lpTexture, m_CurFrame);
	}

	//  [5/6/2009 ppmmjj83] �Ϻ� �������� �������� Į�� Į���� ���� ������� ó���� ����ϴ� �κ��� ����
	else if( !m_bCombat && m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2168 )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );;

		int ProtoNum	= g_ItemList.FindItem( 2317 ); // ���󸶻� ���� ĸ����.
		int	TextrNum	= g_ItemList.m_ItemArray[ProtoNum].m_TextrNum[m_BpdRace];

		if( GET_CSYC(ProtoNum, m_BpdRace) )
			GET_CSYC(ProtoNum, m_BpdRace)->Render(GET_D3DDEVICE(), TextrNum, FALSE, bLow, lpTexture, m_CurFrame);
	}

	//  [5/6/2009 ppmmjj83] �Ϻ� �������� �������� Į�� Į���� ���� ������� ó���� ����ϴ� �κ��� ����
	else if( !m_bCombat && m_Wearing[WEARING_WEAPON] && m_Wearing[WEARING_WEAPON]->m_Vnum == 2339 )
	{
		LPDIRECTDRAWSURFACE7 lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NKCHARACTER_LINE3 ] );;

		int ProtoNum	= g_ItemList.FindItem( 2340 ); // ���󸶻� ���� ĸ����.
		int	TextrNum	= g_ItemList.m_ItemArray[ProtoNum].m_TextrNum[m_BpdRace];

		if( GET_CSYC(ProtoNum, m_BpdRace) )
			GET_CSYC(ProtoNum, m_BpdRace)->Render(GET_D3DDEVICE(), TextrNum, FALSE, bLow, lpTexture, m_CurFrame);
	}

	else if( m_nMotionState == CHA_H_ELECTRICSHOT )		// nate 2005-11-24 : �Ϸ�Ʈ�� �� - ���� �ٲٱ�
	{
		if( m_CurFrame > 9 && m_CurFrame < 56 )
		{
			CItem* ElectricShot_Gun = new CItem;
			ZeroMemory( ElectricShot_Gun, sizeof( CItem ) );
			ElectricShot_Gun->m_ProtoNum = g_ItemList.FindItem( 1339 );	// �߸��� ��
			if( GET_CSY(ElectricShot_Gun, m_BpdRace) )
				GET_CSY(ElectricShot_Gun, m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(ElectricShot_Gun,m_BpdRace), FALSE, bLow, NULL, m_CurFrame);
		}
	}
	// nate 2005-06-10 : Burning Crash - ����
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

	// �ֺ��� �� �ǵ�����
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwAmbient);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, s_dwAlphaTest );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, s_dwAlphaBlend );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, s_dwSrcBlend);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, s_dwDestBlend);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, s_dwLighting );

	// pick buffer ����.
	if (m_HideStatus == HIDE_SHOWN
			&& lpd3dDevPick )
	{
		if( IsDrawPicking() ) // ///������� �� ���� üũ �Լ��߰�
		{
			if( IsDead() ) // �׾����� ��ŷ�ڽ� ȸ��������.
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
			// �ȹ��۸� �������Ҷ����� �ٸ� ����̽��� ����ϱ� ������
			// �ֺ����� ������ �ٸ� ����Ʈ�� ������ ��ġ�� �ʴ´�.
			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			lpd3dDevPick->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
			lpd3dDevPick->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick);
			//		lpd3dDevPick->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///��ŷ���׼���. DWORD ������ ��ȯ�Ѱɾ������� �׳� WORD���� �ٷ� �������� �ִ´�. �̷��� �ص� 32767�� ���� ���� ��ŷ�� ������ ��. ��ȯ�� ���� ���̴� ��찡 �ִµ��ؼ� �̷��� ������. (07-01-02 ����)

			m_CubeLolo.Render(lpd3dDevPick, 0, TRUE);
			lpd3dDevPick->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		}
	}


	// �߹ٴ� �� �׸���.
	if ( !IsDead()
			&& pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz) ) )
	{
		if (m_FlyLevel > 0)
		{
			GET_LOLOC(g_AirshipProto[m_FlyLevel-1])->Render(GET_D3DDEVICE());
			// �̰� ��
			D3DMATRIX matWordI;
			D3DUtil_SetIdentityMatrix(matWordI);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWordI );

			if (!m_pDropStar)
				m_pDropStar = new CDropStar;

			if( m_pDropStar )
				m_pDropStar->Render(m_wx, m_wy, m_wz);
		}
	}
	//mungmae-2005/06/23 ����콺 �����ٸ� �׸���.
	if(m_bDrawRegenbar && m_pRegenbar )
		m_pRegenbar->Render( m_dwRegenTime, m_nRegenIndex);

	if( m_pRegenbar && this == g_pRoh && m_nMotionState==CHA_SOCIAL && (m_ActNum>=153&&m_ActNum<158) ) // ������ �� �����̽� ǥ�� üũ.(�ڱ��ڽŸ�)(������ �� �ƴϴ�.)
	{
		if( m_CurFrame >= m_Biped->m_AnimCount - 20 ) // ������ �� �����ӿ� ������ ������
			m_pRegenbar->RenderAnother( POINT_DANCE_SPACE );
	}


	// ���λ����� ���� �ִ� ���� ĳ���Ͷ��... ///���λ���
	if( m_UserShopLevel && m_UserShopLevel < 4 )
	{
		// ��ǰ�����븦 �׷�����.
		if( g_pDisplayStandLolo[m_UserShopLevel-1] && GET_D3DDEVICE() )
			g_pDisplayStandLolo[m_UserShopLevel-1]->Render(GET_D3DDEVICE());
	}

	if( m_bResurrectionSkill )
	{
		char strTemp[128] = "";
		CNkCharacter* pChar = pCMyApp->m_pUIMgr->FindChaById( m_nResurrection_ChaId, FALSE ); // �������� �� ��밡 ��Ȱ�ع����� ��츦 üũ�ϱ� ���� �߰�. ///070723 �Ϻ����׸���Ʈ ����
		if( !pChar ) // ���� ���������� ���������� ��ҽ��� �ش�. (���⼭ üũ�ϴ°� ��ȿ�����̱������� �׳� ���������� ������Ȳ�����ؼ� ���⼭ ��� üũ��Ŵ.)
		{
			// ��� ��Ŷ�� �����ش�.			
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

	// ������� ���̶�� �� ����Ʈ�� �׷��ش�. ///�������
	if( this == g_pRoh && m_bNowFreePVP && m_dwRingCreateTick ) // �ڱ� �ڽ��� ������� ���϶��� ���� �׸�
	{
		// ������ ���.
		DWORD NowFrame = ( (timeGetTime()-m_dwRingCreateTick)/20 ) % 60; // 20������ 1�� �������� 40������¥���� ���.

		EffectSort ef_sort; // ������ ���߱� ���� �༮
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.vPos = m_vPvpRingPos;
		ef_sort.nCurFrame = NowFrame;
		ef_sort.subNum = 60; // ��Ż ���������� ���.
		ef_sort.dwColor = m_dwRingCreateTick; // ������ �ð�.
		pCMyApp->m_pFxSet->m_pFxFreePvpRing->Render( ef_sort ); // Insert�� �ƴ϶� �ٷ� �׷��ܿ� ����.

		// ����üũ. ���� �߽����� ���� ���� �Ÿ� �־����� �������� ģ��.
		float fDist = DistPlToPl(m_wx, m_wz, m_vPvpRingPos.x, m_vPvpRingPos.z);

		if( fDist > 150.0f ) // ������� �ӽ�.
		{
			g_pTcpIp->SendNetMessage( "pvp -4\n" ); // �� �����ƴٰ� ������ �˸�.
//			m_bNowFreePVP = FALSE; // �ѹ��� ������ ���ؼ� ���¸� ����.(�̰� ��쿡 ���� �������� ��Ŷ�� �ʰ� ���� �̻��ϰ� ���̴� ��Ȳ�� ���ü� �����Ƿ� ����.)
			m_dwRingCreateTick = 0; // �ѹ��� ������ ���ؼ� �ʱ�ȭ.
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
// ���� �⺻ ���� false
// ĳ���� ä��â ����
// ��帶ũ, PK��ũ
// �븸 ����
{
	if (m_bShowChatPopup)
	{
		if (bOnlyName && m_pChatPopup && m_pChatPopup->GetIndex() < 1)
			return;

		D3DMATRIX matPipe;
		D3DVECTOR vScr, vWorld;

		vWorld = D3DVECTOR(m_wx, m_wy+23.0f, m_wz);

		// ���ǻ� CLand�� ����� ����.
		if( pCMyApp->m_pUIMgr->m_pLand )
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
		// �������� ���� ȭ����� ������ �ٲ۴�.
		D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

		//vScr.y -= 18;	// �ڽ��� �뷫���� ũ��.
		if (vScr.y < 0)
			vScr.y = 0;

		// �˾�â�� ȭ�麸�� ������ ũ�� ����
		if (vScr.y >= g_pNk2DFrame->GetClientHeight())
			vScr.y = g_pNk2DFrame->GetClientHeight() - 20;

		if (vScr.x < 30)
			vScr.x = 30;
		if (vScr.x >= g_pNk2DFrame->GetClientWidth() - 20)
			vScr.x = g_pNk2DFrame->GetClientWidth() - 20;

		// �븸 ���� ����
		// ��ǥ ���� �ڽ��� �׸�
		if( g_dwClientCountry == CTRY_TWN )
			m_pChatPopup->Draw( vScr.x, vScr.y, 2 );
		else
			m_pChatPopup->Draw( vScr.x, vScr.y );

		int AdjustPosY = m_pChatPopup->GetYpos() - BOX_GAP;

		if ( AdjustPosY < 0 )
			AdjustPosY = 0;

		if( g_bGlobalMatrix ) ///�������¸� ��ũ �۾��� �߰�
			AdjustPosY += 17; // �Ʒ� �༮���� �� �̰� �����ִ°�? ���� �ִµ�.(���������� ���� ��ũ�� �̰� �������) // ������ ��ũ ���� ����.


		if( g_pNvNWin && m_NvNWin ) ///�������¸� Ƚ���� ������...
		{
			AdjustPosY += 1; // ��ũ�� �ʹ� �پ������Ƿ� �ణ ����ְ� ����.
			int x_start = 0;
			int x_move = 0;

			x_start = m_NvNWin * 7; // �� ǥ�õǾ��� �������� ����

			for( int i = 0 ; i < m_NvNWin ; ++i ) // �¸� Ƚ�� ��ŭ ���
			{
				g_pDisplay->Blt( vScr.x - x_start + x_move, AdjustPosY, g_pNvNWin ); // ����μ� ���� ��.
				x_move += 14;
			}
			AdjustPosY += 13; // �Ʒ� �༮���� �� �̰� �����ִ°�? ���� �ִµ�.(���������� ���� ��ũ�� �̰� �������)
		}
			
		if (m_nHaveGuildMark == true)
		{
			pCMyApp->m_pUIMgr->guildmark_render(m_GuildIndex,
												m_pChatPopup->GetXpos() + m_pChatPopup->GetGuildNameXPos(),
												m_pChatPopup->GetYpos() + m_pChatPopup->GetGuildNameYPos());
		}

		// nate
		// �븸 ���� ����
		if( m_Level >= 270 )
			g_pDisplay->Blt( vScr.x - 13, AdjustPosY, g_pMedal );

		// �븸 ���� ����
		// �ؽ�Ʈ�� ���
		if( g_dwClientCountry == CTRY_TWN )
			m_pChatPopup->Draw( vScr.x, vScr.y, 1 );

		// ����� :: g_pGuildpk
		// ��尡 �ִ� ���
		if (m_pstrGuildName)
		{
			// ��������� Ȯ���ϴ� ����
			int master_guild = -1;

			//=========================================
			// nate 2004 - 4
			// CastleWar
			// � ���ָ�ũ�� �����ٰ����� �̸� ��
			// ���� ĳ���� ȭ��Ʈȥ�� �x�°�?
			if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN)
			{
				// ��� �̸� ��
				if( !strcmp( m_pstrGuildName, pCMyApp->m_pUIMgr->m_MasterGuild1 ) )
				{
					// ���� ȭ��Ʈ�� �ְ�
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
			// ���� ĳ���� ���Ϸп� �x�°�?
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
			// ���� ĳ���� ��ī���� �x�°�?
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

				//������̸鼭 ���������� ��...
				if( m_Free_Pk == 1 )
				{
					// g_pPK : CSurface Ŭ���� ��ü <= �̹�������
					g_pDisplay->Blt( vScr.x-30, AdjustPosY, g_pPK);
					// pImgKing : ŷ �̹�������
					g_pDisplay->Blt( vScr.x+7/*-13*/, AdjustPosY, pImgKing);
				}
				//0305  Non������ ��
				else if( m_bNonPK )
				{
					g_pDisplay->Blt( vScr.x-30, AdjustPosY, g_pNonPK);
					g_pDisplay->Blt( vScr.x+7/*-13*/, AdjustPosY, pImgKing);
				}
				else	//������̸鼭 ����������...
					g_pDisplay->Blt( vScr.x-13, AdjustPosY, pImgKing);
			}
			else	////1111	// ����� �ƴϸ鼭
			{
				if(m_Free_Pk==1)
				{
					g_pDisplay->Blt( vScr.x-13, AdjustPosY, g_pPK);

					if(m_bPk_render)	//������ �����...
					{
						SetChatMsg(m_pstrName, "");
						m_bPk_render=FALSE;
					}
				}
				else if(m_bNonPK)	//0305 Non ������ ��...
				{
					g_pDisplay->Blt( vScr.x-13, AdjustPosY, g_pNonPK);

					if(m_bPk_render)	//������ �����...
					{
						SetChatMsg(m_pstrName, "");
						m_bPk_render=FALSE;
					}

				}
				else if(m_bPk_render)	//����
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

				if(m_bPk_render)	//������ �����...
				{
					SetChatMsg(m_pstrName, "");
					m_bPk_render=FALSE;
				}
			}
			else if(m_bNonPK)	//0305 Non ������ ��...
			{
				g_pDisplay->Blt( vScr.x-13, AdjustPosY, g_pNonPK);
				if(m_bPk_render)	//������ �����...
				{
					SetChatMsg(m_pstrName, "");
					m_bPk_render=FALSE;
				}
			}
			else if(m_bPk_render)	//����
			{
				SetChatMsg(m_pstrName, "");
				m_bPk_render=FALSE;
			}
		}

		//if( g_bGlobalMatrix ) ///�����������̸� �̸� �տ� ���� ��ũ ���
		//{
			int x_move = 0;
			int y_move = 0;
			int y_plus = 0; // �� ����

			HDC hdc;
			hdc = GetDC(pCMyApp->Get_hWnd());
			HFONT hFont = (HFONT)SelectObject( hdc, m_pChatPopup->GetFont() );

			x_move = 0;

			// y ���
			if( pCMyApp->Font_AddPosY == 0 ) // �� ���� ���.
				y_plus = 4;
			else
				y_plus = 1;

			SelectObject(hdc, hFont);
			ReleaseDC(pCMyApp->Get_hWnd(), hdc);

			int DrawPosX = m_pChatPopup->GetXpos()+(m_pChatPopup->GetSurWidth()/2)-x_move-10;
			int DrawPosY = m_pChatPopup->GetYpos()+y_move-25;

			if( this == g_pRoh ) // �ڱ� �ڽ��� �̸��� �������� �Ⱥپ������Ƿ� ���� ó��
			{
			/*	if( g_dwClientCountry == CTRY_KOR ) // �ѱ�
				{
					g_pDisplay->Blt( DrawPosX, DrawPosY, g_pMatrixKor);
				}
				else if( g_dwClientCountry == CTRY_JPN ) // �Ϻ�
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
				DrawRect.right =  g_pMatrixKor->GetWidth(); // �׷��� ���� 20*18�� �����޶������.
				DrawRect.bottom =  g_pMatrixKor->GetHeight();

				// �̸� �� 3���ڷ� ���� ����.
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
				else if( strncmp(m_pstrName,"GS*",3) == 0 ) // �Ϻ�
				{
					g_pDisplay->Blt( DrawPosX+DrawRect.left, DrawPosY+DrawRect.top, g_pMatrixTha, &DrawRect );
				}
			}
	//	}

	}

	if( m_UserShopLevel && g_pNk2DFrame->GetControlBottom()
			&& ( g_pNk2DFrame->GetControlBottom()->m_bSalesView || m_bShowChatPopup ) //���ä���� ��� ���콺����Ű�� �ִ³༮�� ǥ��. ///�������� �ü���
			&& m_bVisible ) // ���� ������ ������, ���ä���� ���������� ���̰�.....(���λ����� ����� ä���� �����ϰ� ���ش�.) m_bVisible �� �ø� �÷��״�. ���� ���ĵ� ������ �����ϰ� ���ϱ�� ���� ��. ///���λ���
	{
		D3DMATRIX matPipe;
		D3DVECTOR vScr, vWorld;

		vWorld = D3DVECTOR(m_wx, m_wy+15.0f, m_wz);

		// ���ǻ� CLand�� ����� ����.
		if( pCMyApp->m_pUIMgr->m_pLand )
			pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
		// �������� ���� ȭ����� ������ �ٲ۴�.
		D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

		//vScr.y -= 18;	// �ڽ��� �뷫���� ũ��.
		if (vScr.y < 0)
			vScr.y = 0;

		// �˾�â�� ȭ�麸�� ������ ũ�� ����
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
// ä�� �޼��� ������ ���� �ٸ� ������ CMultiText�� AddString()
{
	CMultiText multiText;
	char strTemp[100] ="";


	m_nGuildNameLine = 0;
	m_nHaveGuildMark = false;

	if( g_bGlobalMatrix ) ///���������� �ÿ� ���� ��ũ ǥ�ø� ���ؼ�...
	{
		multiText.AddString(" ", _FCOLOR_GREEN);
	}

	if( m_NvNWin ) ///�������¸� Ƚ���� �ִٸ�..
	{
		strcpy( strTemp, "  " );

		for( int i = 1 ; i < m_NvNWin ; ++i ) // �¸� Ƚ���� ���� ���� ��츦 ����� ������ Ƚ���� ����ؼ� �ִ´�.
			strcat( strTemp, "  " );

		multiText.AddString( strTemp, _FCOLOR_GREEN );
	}

	// ��尡 �ִ��� �˻�.
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
			// ������ ���� ���ڻ� ����
			if( m_GuildGradeIndex == 10 )			// �渶
				multiText.AddString(strTemp, RGB(255, 50, 50));
			else if( m_GuildGradeIndex == 9 )		// �α渶
				multiText.AddString(strTemp, RGB(128, 50, 128));
			else if( m_GuildGradeIndex <= 7 && m_GuildGradeIndex >= 2)		// ����
				multiText.AddString(strTemp, RGB(255, 50, 255));
			else if( m_GuildGradeIndex == 1 )		// ����
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

	// ī�����
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

	// ���ڿ��� ������� ������ ���
	// ���ڿ� ���̸� 15���� 35���� �÷� ����غ���. [2/8/2007 Theodoric]
	multiText.AddStringMulti(chat_msg, 15, TRUE);

	m_pChatPopup->MakeChatPopup(&multiText, TRUE, m_nHaveGuildMark ? m_nGuildNameLine : -1);
	m_pChatPopup->SetIndex(0);		// ���� �������� ê �ڽ�
	m_LastChatTime = timeGetTime();
}

void CNkCharacter::OnChatMsg()
{
	if (g_SvrMode == eSVR_MASTER)
		return;

	CMultiText multiText;
	char strTemp[100] = "";


	m_nGuildNameLine = 0;

	if( g_bGlobalMatrix ) ///���������� �ÿ� ���� ��ũ ǥ�ø� ���ؼ�...
	{
		multiText.AddString(" ", _FCOLOR_GREEN);
	}

	if( m_NvNWin ) ///�������¸� Ƚ���� �ִٸ�..
	{
		strcpy( strTemp, "  " );

		for( int i = 1 ; i < m_NvNWin ; ++i ) // �¸� Ƚ���� ���� ���� ��츦 ����� ������ Ƚ���� ����ؼ� �ִ´�.
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
			if( m_GuildGradeIndex == 10 )			// �渶
				multiText.AddString(strTemp, RGB(255, 50, 50));
			else if( m_GuildGradeIndex == 9 )		// �α渶
				multiText.AddString(strTemp, RGB(128, 50, 128));
			else if( m_GuildGradeIndex <= 7 && m_GuildGradeIndex >= 2)		// ����
				multiText.AddString(strTemp, RGB(255, 50, 255));
			else if( m_GuildGradeIndex == 1 )		// ����
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
	// ä�ø��� ����� �Ŀ���... �̸��� ������!
	if (m_pChatPopup && (m_LastChatTime + SHOW_CHATPOPUP_TIME > dwCurtime))
	{
		m_pChatPopup->SetIndex(1); // ä�ø��� ���� �ִµ� ä�� Ȯ�� ��忡�� �̸� Ȯ��.
		return;
	}
	else
	{
		if (m_pChatPopup->GetIndex() < 2)
			SetChatMsg(m_pstrName, "");
	}
	// �ð��� ä��â�� �� �ִ� �ð��� ��������...
	if (m_LastChatTime + (SHOW_CHATPOPUP_TIME / 2) < dwCurtime)
		m_LastChatTime = dwCurtime - (SHOW_CHATPOPUP_TIME / 2);

	if( m_pChatPopup )
		m_pChatPopup->SetIndex(2); // ���콺�� �÷����� ���� �̸� �ڽ�
}

// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
void CNkCharacter::Wear(int where, int index, int vnum, int gem_num, int special, int special2,
						int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///�Ź���ȭ�ý���
{
	if (vnum < 0)
		return;

	if( where == 3 && g_pRoh ) ///�����̾���� ���� ����Ʈ ó��.(ĳ���� ����ȭ���� �����ϱ� ���� ���ΰ� ���� ���θ� üũ.)
	{
		if( m_Wearing[3] && m_Wearing[3]->m_Vnum == 1774 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 1774) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
		if( vnum == 1774  ) // ���� �����̾� ���並 ���� ���.
		{
			if( !m_EventWearing[3] ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
				pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.
		}

		if( m_Wearing[3] && m_Wearing[3]->m_Vnum == 11273 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 11273) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
		if( vnum == 11273  ) // ���� �����̾� ���並 ���� ���.
		{
			if( !m_EventWearing[3] ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
				pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.
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
		//2003 �����ϰ� �ִ� ����������� �÷������������� �ν����� �ʰ� �ϱ�.
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

		///�Ź���ȭ�ý���
		m_Wearing[where]->m_Special2 = special2; // �߰� �ɼ�
		m_Wearing[where]->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
		m_Wearing[where]->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

		m_Wearing[where]->m_Endurance = Endur;
		m_Wearing[where]->m_MaxEndurance = MaxEndur;

		// ������ ���� �Է��غ���!
		ResetItemValue(m_Wearing[where]);
		SetSpecialValue(m_Wearing[where], GET_TYPE(m_Wearing[where]), GET_SHAPE(m_Wearing[where]), special, special2);
	}

	OnBossRaidHeroEffect();
}

void CNkCharacter::EventWear(int where, int index, int vnum, int gem_num, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///�̺�Ʈ��������
{
	if( !g_bUseEventWear ) // �̺�Ʈ ���� ���� �ý����� ���� ���ϴ� ����� �۵����ϰ�...
		return;

	if (vnum < 0)
		return;

	if( where == 3 && g_pRoh ) ///�����̾���� ���� ����Ʈ ó��.(ĳ���� ����ȭ���� �����ϱ� ���� �ε����� üũ.)
	{
		if( m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 1774 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
		if( vnum == 1774 ) // ���� �����̾� ���並 ���� ���.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.
		}

		if( m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 11273 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
		if( vnum == 11273 ) // ���� �����̾� ���並 ���� ���.
		{
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.
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
		//2003 �����ϰ� �ִ� ����������� �÷������������� �ν����� �ʰ� �ϱ�.
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

		///�Ź���ȭ�ý���
		m_EventWearing[where]->m_Special2 = special2; // �߰� �ɼ�
		m_EventWearing[where]->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
		m_EventWearing[where]->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

		m_EventWearing[where]->m_Endurance = Endur;
		m_EventWearing[where]->m_MaxEndurance = MaxEndur;

		// ������ ���� �Է��غ���!
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
		if( !m_EventWearing[3] ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.
	}
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 )
	{
		if( !m_EventWearing[3] ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
			pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.
	}

	m_Wearing[where] = m_ExtraSlot;
	m_ExtraSlot = pItem;

	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1774 )///�����̾���� ���並 �� ���.
	{
		if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 1774) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
	}
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 )///�����̾���� ���並 �� ���.
	{
		if( !(m_EventWearing[3] && m_EventWearing[3]->m_Vnum == 11273) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
			pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
	}



	if ((where == WEARING_WEAPON || where == WEARING_SHIELD) && !IsDead())
		ResetMotionState();

	return where;
}

int CNkCharacter::EventWear(int where) ///�̺�Ʈ��������
{
	if( !g_bUseEventWear ) // �̺�Ʈ ���� ���� �ý����� ���� ���ϴ� ����� �۵����ϰ�...
		return -1;

	// -1�� ���� m_ExtraSlot�� ������ �ִ� ���,,
	// where�� �ִ� ���� m_ExtraSlot�� ������ ���� ���.
	if (where >= 0)
	{
		if (!m_EventWearing[where])
			return -1;
	}
	else
	{
		// where �� -1�� ��쿡�� extra slot�� �־�� �Ѵ�.
		if (!m_ExtraSlot)
			return -1;
	}

	// ExtraSlot�� �ִ� ��쿡�� ���밡���� ����ΰ� Ȯ��.
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
		// �ϴÿ����� �Ź� �� �ٲ۴�.
		return -1;
	}

	CItem *pItem = NULL;

	if (m_EventWearing[where])
		pItem = m_EventWearing[where];



	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1774 ) ///�����̾���� ���並 �� ���.
		pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 ) ///�����̾���� ���並 �� ���.
		pCMyApp->m_pUIMgr->SkillOnOff( 1, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� �־��ش�.

	m_EventWearing[where] = m_ExtraSlot;
	m_ExtraSlot = pItem;

	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1774 )///�����̾���� ���並 �� ���.
		pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 11273 )///�����̾���� ���並 �� ���.
		pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.


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
// ����Ʈ�� ������ ���Ⱑ �ش� ĳ���� ������ �� �ִ��� �Ǵ�.
int CNkCharacter::CanWear()
{
	int where = -1;
	if (m_ExtraSlot)
	{
		if (!(GET_ITEM_RACE(m_ExtraSlot) & (1 << m_BpdRace)))
			return -11;

		if( g_bGlobalMatrix )
		{
			if( g_dwClientCountry == CTRY_KOR ) // �ѱ��� ��� ���� �� ���� ������
			{
				if( m_ExtraSlot->m_Vnum == 492 || m_ExtraSlot->m_Vnum == 493 // ����
						|| m_ExtraSlot->m_Vnum == 923 || m_ExtraSlot->m_Vnum == 924 // �� ����
						|| m_ExtraSlot->m_Vnum == 518 || m_ExtraSlot->m_Vnum == 519 || m_ExtraSlot->m_Vnum == 520 // ���ں�
						|| m_ExtraSlot->m_Vnum == 1422 || m_ExtraSlot->m_Vnum == 1423 // �Ϻ� �౸ ������
						|| m_ExtraSlot->m_Vnum == 1410 // ���� ���
				  )
					return -13;
			}
			else if( g_dwClientCountry == CTRY_JPN ) // �Ϻ��� ��� ���� �� ���� ������
			{
				if( m_ExtraSlot->m_Vnum == 301 || m_ExtraSlot->m_Vnum == 303 // �Ѻ�
						|| m_ExtraSlot->m_Vnum == 433 || m_ExtraSlot->m_Vnum == 434 // �� �Ѻ�
						|| m_ExtraSlot->m_Vnum == 1289 || m_ExtraSlot->m_Vnum == 1290 || m_ExtraSlot->m_Vnum == 920 // �±ǵ� ��(�µΰ� ����)
						|| m_ExtraSlot->m_Vnum == 1418 || m_ExtraSlot->m_Vnum == 1419 // �Ϻ� �౸ ������
						|| m_ExtraSlot->m_Vnum == 1411 // ȣũ ���
				  )
					return -13;
			}
		}

		if(GET_TYPE(m_ExtraSlot)!=ITYPE_SUB)
		{
			if (m_ExtraSlot->m_MinLevel > m_Level)
				return -2;

			if ( (GET_ITEM_MAXLEVEL(m_ExtraSlot) < m_Level )
					&& ( m_ExtraSlot->m_Vnum == 1238		// ����ũ���
						 || m_ExtraSlot->m_Vnum == 1259		// Ȧ��������( ȣũ )
						 || m_ExtraSlot->m_Vnum == 1260		// Ȧ��������( ���� )
						 || m_ExtraSlot->m_Vnum == 1261 ))	// Ȧ��������( ���� )
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
							|| !( ( m_GuildIndex == 1 && m_ExtraSlot->m_Vnum == 1259 )		// Ȧ��������( ȣũ )
								  || ( m_GuildIndex == 2 && m_ExtraSlot->m_Vnum == 1260 )		// Ȧ��������( ���� )
								  || ( m_GuildIndex == 3 && m_ExtraSlot->m_Vnum == 1261 )))	// Ȧ��������( ���� )
						return -13;
					break;
				case 2:
					if( !m_GuildIndex
							|| !( ( m_GuildIndex == 1 && m_ExtraSlot->m_Vnum == 2170 )		// ���帶��	Ȧ�� ������
								  || (m_GuildIndex == 2 && m_ExtraSlot->m_Vnum == 2171 )))	// �ȱ׸��� Ȧ�� ������
						return -13;
					break;
				}
			}
		}
		else
		{
			// ī�̸���
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
				if( GET_SHAPE(m_ExtraSlot)==ISUB_SLAYER || GET_SHAPE(m_ExtraSlot)==ISUB_CHAKRAM ) // �����̾ íũ���̶��...
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
				else if( m_ExtraSlot->m_Vnum != 829 && m_ExtraSlot->m_Vnum != 830 // ���̴� / �ϵ�踮�� ��ȯ
						 && m_ExtraSlot->m_Vnum != 831 && m_ExtraSlot->m_Vnum != 832   // ������ȯ / ��Ʈ����
						 && m_ExtraSlot->m_Vnum != 833 && m_ExtraSlot->m_Vnum != 834   // ���ɴ����� / ��ȭ����
						 && m_ExtraSlot->m_Vnum != 835 && m_ExtraSlot->m_Vnum != 842   // �ݵ����� / ��������
						 && m_ExtraSlot->m_Vnum != 844                                 // ���պ�ȯ
					   ) // ���� ���� ���� ���� ���� ������ �޸յ� ���뽺ų���� ������ ������ �Ѵ�.
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
				if(GET_SHAPE(m_ExtraSlot)==ISUB_DSTONE) // ����� ���� �̶��...
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
		// �ϴ� ���� �˻�.
		if (m_Str < m_ExtraSlot->m_Weight)
			return -3;

		///�̺�Ʈ�������� �ý��� �������� üũ ///070723 �Ϻ����׸���Ʈ ���� �� ��ġ�̵�.
		if( g_bUseEventWear )
		{
			if( g_pNk2DFrame->GetControlInven() && g_pNk2DFrame->GetControlInven()->m_bEventWearView ) // �̺�Ʈ ���� ������ �����ִ� ��츸...
			{
				if( !(GET_ATT_FLAG(m_ExtraSlot) & IATT_EVENT_WEAR) ) // �̺�Ʈ ���� ���Կ� ���� �Ұ����� �������� ���
					return -14;
			}
		}

		if(GET_ITEM_LEVEL(m_ExtraSlot) > 0 && GET_ATT_FLAG(m_ExtraSlot) & IATT_MAX_LEVEL)
		{
			if( g_pRoh->m_Level > GET_ITEM_MAXLEVEL(m_ExtraSlot ) || g_pRoh->m_Level < GET_ITEM_LEVEL(m_ExtraSlot) )
				return -2;
		}

		//----------------------------------------------------------------------
		// Extra Slot �� ��� �ִ� ��� ���� �Ǻ��� �ൿ/ �޽��� ó��
		switch (GET_TYPE(m_ExtraSlot))
		{
		case ITYPE_ARMOR:
			// ���и� �鶧, �̹� �μ� ���⸦ ����ٸ� �Ұ��� �޼���
			// �޸� ��Ÿ/���̸�Ÿ �� ��� ���������� ���� ����
			// 2004.05.12
			// Lyul / �����Ϸ�
			if (where == WEARING_SHIELD )
			{
				//  [5/18/2009 ppmmjj83] �Ϻ����������׽�Ʈ ���е��� ��Ÿ����ó�� �μչ������� ���������ϰ�.
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

				if( m_Race == RACE_FREAK )	// ���̺긮��
				{
					if( m_Wearing[WEARING_WEAPON] )
					{
						if( GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER )
						{
							return where;
						}
						else if(  GET_SHAPE(m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD )	// �̵��� �����
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
				//  [5/18/2009 ppmmjj83] �Ϻ����������׽�Ʈ ���е��� ��Ÿ����ó�� �μչ������� ���������ϰ�.
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
		///2003 ����������� ���䳪 ���� �κ��� �����Ҽ� �ִ��� ����.
		case ITYPE_SUB://����������� ���
			if ((GET_SHAPE(m_ExtraSlot) == ISUB_L_EQUIP) 		// ����������� ����.
					|| (GET_SHAPE(m_ExtraSlot) == ISUB_SLAYER) 		// �����̾����� ����.
					|| (GET_SHAPE(m_ExtraSlot) == ISUB_DSTONE)
					|| (GET_SHAPE(m_ExtraSlot) == ISUB_PANEL)
			   )		// ����� ���� // Lyul_2005
				return 3; //����Ӽ�

			if (GET_SHAPE(m_ExtraSlot) == ISUB_WAND) 		//�ϵ� ���� ����.
				//	return 9; //����Ӽ�
				return 5; //���мӼ�

			if (GET_SHAPE(m_ExtraSlot) == ISUB_S_EQUIP) 		//����������� ����.
				return 8; //����Ӽ�

			where=-20; //���� ���� �ƴ� ����������� �����ǹ� �ȵǴϱ�..
			return where;

		default:
			// ��� ���⵵ �ƴϸ�.. ���  �ȵ�¡...
			return -20;
		}
	}
	else
		return -20;
}

// �����ϰ� �ִ� ���⿡ ������ ����Ʈ�� Upgrade �� �Ѵ�.
// 2004.03.05 - �����Ϸ�
// Lyul / ����(���̾� ���� )
// ���̾� �� ��� ���� 1�� �ѹ��� Upgrade �� �õ��� �̷�� ���Ƿ�
// Plus ó���� ���Ѵ�.
int CNkCharacter::Upgrade(int where)
{
	// �� ������ �ƴϰ� ������ �������� ������ �� �������� Up grade �� �Ұ���
	// �ϴٸ� return -1
	if (m_ExtraSlot && m_Wearing[where])
	{
		if ((where = CanUpgrade(where)) < 0)
			return -1;
	}
	else
		return -1;

	// ���̾Ʊ� �̻��� ���� ���� ���׷��̵带 �Ѵ�.
	if(GET_GEMMIN_LEVEL(m_ExtraSlot) >= 181)
		return where;

	// plus���׷����ΰ� �� ���� �߰��ܰ踦 �����Ͽ� �ش�.
	// plus �ܰ��ϰ�쿡�� ��ġ�� �������� �ʴ´�.
	if (m_Wearing[where]->m_GemNum < m_Wearing[where]->m_PlusNum )
		m_Wearing[where]->m_GemNum += 1;

	return where;
}

// �����ϰ� �ִ� ���⿡ ������ ����Ʈ�� Upgrade�������� �Ǻ��Ѵ�.
// -1            : �Ұ���
// others = where: ����
// 2004.03.05 - �����Ϸ�
// Lyul / Comment
// 2004.05.12 - �����Ϸ�
// Lyul / ī�� ���̸�Ÿ , ���� ���� ���� ���� �Ұ�.
// 2004.08.13 - ������
// �Ź��� �߰��� ���� ����Ʈ ���̾� ����
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

	if( CheckCanNpcUpgradeItem(m_Wearing[where]) ) // NPC ���׷��̵� ������ �̸� �Ϲ� ���׷��̵� �Ұ� ó��. ///�Ź���ȭ�ý���
		return -1;

	if( m_Wearing[where]->m_MinLevel_Org > 261 )
		return -1;

	///�ð��������� ���׷��̵� �Ұ� ó�� (05-07-13 by ����)
	if( m_Wearing[where]->m_bTimeLimit ) // �ð��� �������̸�...
		return -1;

	// ���׷��̵� �Ұ� ������ : ��Ÿ / ���̸�Ÿ ��
	if(m_Wearing[where]->m_Vnum==875		// ��ĭ ��Ÿ ����
			|| m_Wearing[where]->m_Vnum==876	// �޸� ��Ÿ ����
			|| m_Wearing[where]->m_Vnum==573	// ī�� ��Ÿ ������
			|| m_Wearing[where]->m_Vnum==575	// ���� ���� ����
			|| m_Wearing[where]->m_Vnum==1027	// ��ĭ ���̸�Ÿ ����
			|| m_Wearing[where]->m_Vnum==1028	// �޸� ���̸�Ÿ ����
			|| m_Wearing[where]->m_Vnum==1029	// ī�� ���̸�Ÿ ����
			|| m_Wearing[where]->m_Vnum==1030	// ���� ���� ���� ����
			|| m_Wearing[where]->m_Vnum==1234	// ���̺긮�� ��Ÿ ����
			|| m_Wearing[where]->m_Vnum==1235	// ���̺긮�� ���̸�Ÿ ����
			|| m_Wearing[where]->m_Vnum==1236	// ���̺긮�� ��Ÿ ������
			|| m_Wearing[where]->m_Vnum==1237
			|| ( m_Wearing[where]->m_Vnum >= 2049 && m_Wearing[where]->m_Vnum <= 2055 )
			|| ( m_Wearing[where]->m_Vnum >= 2173 && m_Wearing[where]->m_Vnum <= 2179 )
			|| ( m_Wearing[where]->m_Vnum >= 10454 && m_Wearing[where]->m_Vnum <= 10484 ) // NEW WEAPON
			|| m_Wearing[where]->m_Vnum==10517
			|| ( m_Wearing[where]->m_Vnum >= 10522 && m_Wearing[where]->m_Vnum <= 10585 ) // NEW WEAPON
			|| ( m_Wearing[where]->m_Vnum >= 2939 && m_Wearing[where]->m_Vnum <= 2941 )
			|| ( m_Wearing[where]->m_Vnum >= 2942 && m_Wearing[where]->m_Vnum <= 2953 )
	  )	// ���̺긮�� ���̸�Ÿ ������
		return -1;



	// ����� ������ ������ ������ Ȯ��.
	int item_level = GET_ITEM_LEVEL(m_Wearing[where]);
	int min_level = GET_GEMMIN_LEVEL(m_ExtraSlot);
	int max_level = GET_GEMMAX_LEVEL(m_ExtraSlot);


	
	// ���̾Ƹ��� �̰� �Ѱ� Level �̳��� ��� (<15) Can
	// ����Ʈ ���̾Ƹ��� �̻� �̰� �Ѱ� Level �̳��� ��� (<15) Can
	if(item_level >= min_level
			&& item_level <= max_level
			&& (min_level == 181 || min_level == 261)
			&& m_Wearing[where]->m_PlusNum<15 )
		return where;

	// �Ϲ� �����̰� ��� Level �̳��� ���
	else if ( item_level >= min_level && item_level <= max_level)
	{
		// +5 �̳��� ��� Can
		if (m_Wearing[where]->m_PlusNum < 5)
			return where;
		else
			return -1;
	}
	else
		return -1;
}

// �����ϰ� �ִ� Item �� �����Ѵ�.
void CNkCharacter::Remove(int where)
{
	if (!m_Wearing[where])
		return;

	if( where == 3 ) ///�����̾���� ���� ����Ʈ ó��.(///071101 �ٸ� ĳ���Ͱ� ������ ó�� �߰�)
	{
		if( m_Wearing[where]->m_Vnum == 1774 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			if( !(m_EventWearing[where] && m_EventWearing[where]->m_Vnum == 1774) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
		if( m_Wearing[where]->m_Vnum == 11273 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			if( !(m_EventWearing[where] && m_EventWearing[where]->m_Vnum == 11273) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
	}

	delete m_Wearing[where];
	m_Wearing[where] = NULL;

	if (where == WEARING_WEAPON)
		ResetMotionState();

	OnBossRaidHeroEffect();
}

// �����ϰ� �ִ� Item �� �����Ѵ�.
void CNkCharacter::EventWearRemove(int where) ///�̺�Ʈ��������
{
	if (!m_EventWearing[where])
		return;

	if( where == 3 ) ///�����̾���� ���� ����Ʈ ó��.(///071101 �ٸ� ĳ���Ͱ� ������ ó�� �߰�)
	{
		if( m_EventWearing[where]->m_Vnum == 1774 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			if( !(m_Wearing[where] && m_Wearing[where]->m_Vnum == 1774) ) // �Ϲ� ���䰡 �����̾� ������ ���� ����...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
		if( m_EventWearing[where]->m_Vnum == 11273 ) // ���� �����̾� ���並 �԰��־����� ����.
		{
			if( !(m_Wearing[where] && m_Wearing[where]->m_Vnum == 11273) ) // �Ϲ� ���䰡 �����̾� ������ ���� ����...
				pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
		}
	}

	delete m_EventWearing[where];
	m_EventWearing[where] = NULL;

	if (where == WEARING_WEAPON)
		ResetMotionState();

	OnBossRaidHeroEffect();
}

// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
void CNkCharacter::AddToInven(int pack_num, int x, int y, int index, int vnum, int gem_num, int special, int special2,
							  int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///�Ź���ȭ�ý���
{
	if (vnum < 0)
		return;

	if (pack_num == 3)
	{
		// ��ȯâ�� �ִ� ������ ����.
		AddToExgInven(x, y, index, vnum, gem_num, special, special2, UpEndur, MaxUpdEndur, Endur, MaxEndur ); ///�Ź���ȭ�ý���
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

		// ������ ���� �Է��غ���!
		ResetItemValue(pItem);

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		//0917  �κ�â�� ����������� �÷��� ���������� �ν��ϴ°� ����.
		if(GET_TYPE(pItem)==ITYPE_SUB)
		{
			pItem->m_PlusNum = gem_num;
		}
		else if (gem_num > 0)
		{
			// 2004. 03. 22  / Lyul
			// ������ ���̾� ��� �ν� ���� Big Fix
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
					bEmptySlot = FALSE; // ������ ���¼��� �������� ������ ���Կ� ������ �ٸ� �������� �Ű� ���Ҵ�.
			}
		}

		if( !bEmptySlot && !m_ExtraSlot)
		{
			SetExtraSlot(index, vnum, gem_num, special, special2, UpEndur, MaxUpdEndur, Endur, MaxEndur); ///�Ź���ȭ�ý���

			if(pItem)
			{
				delete pItem ;
				pItem = NULL;
			}

			return;
		}
		//---------------------------------------------------------------------
		pItem->m_Special = special;

		///�Ź���ȭ�ý���
		pItem->m_Special2 = special2; // �߰� �ɼ�
		pItem->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

		pItem->m_Endurance = Endur;
		pItem->m_MaxEndurance = MaxEndur;

		SetSpecialValue(pItem, GET_TYPE(pItem), GET_SHAPE(pItem), special, special2);
		pItem->m_Next = m_Inven[pack_num];
		m_Inven[pack_num] = pItem;
		// nate 2004 - 3
		// â�� ���� Ȯ�� ����
		// ������ ī��Ʈ ���� ����
		CountQuickAll();

		for ( int x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
		{
			for (int y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
				m_Slot[pack_num][x+x_idx][y+y_idx] = index;
		}

// by evilkiki �ϴ� �ּ�ó��
//		g_pNk2DFrame->GetUpgrade()->CheckUpgradeNeed();

	}
}

void CNkCharacter::AddToInven( int nPackNum, int nItemIndex, int nSlot_X, int nSlot_Y, int nVnum )
{}

// [6/7/2007 Theodoric] ������ ���� :: �Լ� ���� �߰�
BOOL CNkCharacter::AddToExgInven(int x, int y, int index, int vnum, int gem_num, int special, int special2,
								 int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, BOOL bForce) ///�Ź���ȭ�ý���
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

		// ������ ���� �Է��غ���!
		ResetItemValue(pItem);

		pItem->m_SlotX = x;
		pItem->m_SlotY = y;
		///2003  ��ȯâ �κ��丮�� ����������� �÷��� ���������� �ν��ϴ°� ����
		if(GET_TYPE(pItem)==ITYPE_SUB)
		{
			pItem->m_PlusNum = gem_num;
		}
		else if (gem_num > 0)
		{
			// 2004. 03. 22  / Lyul
			// ������ ���̾� ��� �ν� ���� Big Fix
			if(GET_ITEM_LEVEL(pItem) >= 181)
				pItem->SetDiaGemNum(gem_num);
			else
				pItem->SetGemNum(gem_num);
		}

		pItem->m_Special = special;

		///�Ź���ȭ�ý���
		pItem->m_Special2 = special2; // �߰� �ɼ�
		pItem->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
		pItem->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

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

// by evilkiki �ϴ� �ּ�ó��
		//g_pNk2DFrame->GetUpgrade()->CheckUpgradeNeed();

	}

	// by evilkiki 2009.10.21 ������ ���� ����
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
		pack_num = m_CurPackNum;		// ���� �� �ѹ��� ����

	if (m_ExtraSlot)	 // Extra Slot �� �������� �ִ� ���
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

		// �������� ũ�� ���
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

// nate 2004 - 4 : Exchange - ��ȯâ�� �������� �ְų� ����
BOOL CNkCharacter::AddToExgInven(int x, int y, BOOL bForce)
{
	if (m_bExgLock && bForce == FALSE)
		return FALSE;

	CItem *pItem = NULL;
	int index;
	// ��ȯâ�� ������
	if (m_ExtraSlot)
	{
		index = GetItemIndexInExgSlot(x, y, GET_ITEM_WIDTH(m_ExtraSlot), GET_ITEM_HEIGHT(m_ExtraSlot));

		if (index < -1)				// �� ���� ���.
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
	// ��ȯâ���� ����
	else
	{
		index = GetItemIndexInExgSlot(x, y, 1, 1);

		if (index >= 0)
		{
			m_ExtraSlot = PickItemInExgPack(index);
		}
		else
			return FALSE;		// ��ȯâ ���Կ� �ƹ��͵� ���� ���.

		if ((m_ExtraSlot && GET_TYPE(m_ExtraSlot) == ITYPE_ETC && \
				(GET_SHAPE(m_ExtraSlot) == IETC_RECOVERY || GET_SHAPE(m_ExtraSlot) == IETC_GEMSTONE || \
				 GET_SHAPE(m_ExtraSlot) == IETC_GEM))
				|| (m_ExtraSlot && GET_TYPE(m_ExtraSlot) == ITYPE_ETC &&(GET_SHAPE(m_ExtraSlot) == IETC_SCROLL || GET_SHAPE(m_ExtraSlot) == IETC_SHELL || GET_SHAPE(m_ExtraSlot) == IETC_CHARGE)))
		{
			CountQuickAll();
		}
	}

	// by evilkiki 2009.10.21 ������ ���� ����
	if( g_pNk2DFrame->GetControlInven() )
		g_pNk2DFrame->GetControlInven()->ResetItemInfo();

	return TRUE;
}

// �� �κ��� ���������� ȣ��ȴ�.
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
	// nate 2004 - 4 : exchange - ExtraSlot�� ��������� ����
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
			// m_ExtraSlot->m_Vnum == 693 ��ũ�� �����Կ� �߰�
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
			// gem type ������ ��� ���͸� by. Sol9 [2012/12/06]
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

	// �̰� ���� �ɷ� �ִ� ���Թ�ȣ�̴�.
	if (m_bQuickSlotLock[slot_num])
		return FALSE;

	if (!m_ExtraSlot && !m_QuickSlot[slot_num])
		return FALSE;

	if (m_ExtraSlot && !(GET_ITEM_RACE(m_ExtraSlot) & (1 << m_BpdRace)))
		return FALSE;

	CItem *pItem = m_QuickSlot[slot_num];
	m_QuickSlot[slot_num] = m_ExtraSlot;
	m_ExtraSlot = pItem;

	// ���� ���� ��ġ�� �����Կ� ����ϰų� ������ ��
	// ���� ���� ��ġ UI�� �ݾ� �ش�
	if( m_ExtraSlot && m_ExtraSlot->m_Vnum == 1015 )
	{
		if( pCMyApp && pCMyApp->m_pUIMgr )
			pCMyApp->m_pUIMgr->CloseTeleportItem();
	}

	CountQuickAll();

	return TRUE;
}

// �̰� �������� ���Կ� ���� �־��ִ� ����̴�.
//2003 �Լ��� ���� ���� plus_num,special_num
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
		///2003 ����������� ���� �ޱ�(���� ������)
		m_QuickSlot[slot_num]->m_PlusNum = plus_num;
		m_QuickSlot[slot_num]->m_Special = special_num;
		m_QuickSlot[slot_num]->m_Special2 = special2_num;

		// ������ ���� �Է��غ���!
		ResetItemValue(m_QuickSlot[slot_num]);
		CountQuickBySlot(slot_num);

		// ���������� ���� �ɷ� �ִ� ������ Ǯ���ش�.
		if (m_bQuickSlotLock[slot_num])
			m_bQuickSlotLock[slot_num] = FALSE;
	}
}
//==============================================================================
// [12/9/2008 D.K ] :
// �κ��丮 ���� Ȯ�� �ڵ�.
// �� �Ѻ� �κ��丮�� �� á������ ���� ������ �Ѱ��ش�.
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
	// ���� ������ŭ ����
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
// ��ȯ�� ������ �ڵ� ä��� ���
// ��ȯ�� ���͸� ����
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

	// ���� ������ŭ ����
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
		// ��ȯ�� ������ �ڵ� ä��� ���
		// ���͸� �ּ�ó��
		if( g_pNk2DFrame->GetControlBottom() )
		{
			if( m_QuickCount[i] != count )
			{
				m_QuickCount[i] = count;
				///2003 �������� �������� å�̶�� ������ ī��Ʈ ���� �ʴ´�.
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
// nate - �κ� ���Կ��� �ε��� ����
{
	int index = -1;
	for( int x_idx = nInvenSlotX ; x_idx < nInvenSlotX + nItemWidth ; x_idx ++ )
	{
		for( int y_idx = nInvenSlotY; y_idx < nInvenSlotY + nItemHeight ; y_idx ++ )
		{
			// NkCharacter�� �κ��丮 ���Թ迭�� �����Ѵ�.
			// Inven�� Slot�� �������� ������ �ε��� ����
			if( m_Slot[ nInvenPackNum ][ x_idx ][ y_idx ] != -1 )
			{
				if( index == -1 )	// �ε����� ã���� �ٽô� ������ �ʴ´�.
					index = m_Slot[ nInvenPackNum ][ x_idx ][ y_idx ];
				// ���� �������� ũ�⿡ �°� �ε����� ��
				// �ٸ����� ����( �� ������ ���� �ٸ��� �ִٴ�.
				else if ( index != m_Slot[ nInvenPackNum ][ x_idx ][ y_idx ] )
					return -2;		// ����
			}
		}
	}

	return index;
}

// exchange slot( ��ȯâ )�� ���Ѱ�.
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
// ���� : InvenPackNum, ItemIndexNum
// ��� : �����κ��ѿ��� �������� �˻��ϰ� �����ϴ� ���
// �޸� ���� ����
// ���ŵ� �������� delete�ؾߵ�
// ��ȯ�� : ���� �ε����� ������ Ŭ���� ��ȯ
{
	// ���Ḯ��Ʈ ����
	CItem *beforeItem = NULL;		// ���� ���� �������� ����Ŵ
	CItem *pItem = m_Inven[ pack_num ];		// �������� ù��° �������� ����

	while ( pItem )
	{
		// �������� ������ �˻�
		// ���� IndexNum�Ǵ� Vnum�� ���� ������ �˻�
		if ( ( !isVnum && pItem->m_Index == index ) || ( isVnum && pItem->m_Vnum == index ) )
		{
			if ( beforeItem )
			{
				beforeItem->m_Next = pItem->m_Next;
				// nate 2004 - 3 : â�� ������ �ٷ��̵� ����
				// �߰� : �������� ī��Ʈ ����
				CountQuickAll();
			}
			else
			{
				m_Inven[pack_num] = pItem->m_Next;
				// nate 2004 - 3 : â�� ������ �ٷ��̵� ����
				// �߰� : �������� ī��Ʈ ����
				CountQuickAll();
			}

			// �������� ũ�⿡ �°� ������ �ε������� -1�� �ֱ�ȭ
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
			if( pItem->m_Vnum == 1015 // ���� ������ġ�� �ٽ� ä���ֱ� ���ϵ��� ó��(���������) (05-11-08 ����)
					|| pItem->m_Vnum == 2222 || pItem->m_Vnum == 2223 || pItem->m_Vnum == 2224) // �Ϻ��� ��Ȱ����
			{
				pItem = pItem->m_Next;

				continue;
			}

			// connie [2009/3/9] -  �Ϻ���û - �޴��� ȸ�������� ���ȵǵ���
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
	// �� �Լ��� �κ����� ������ Ŭ������ ����� ����Ҷ� ������ �Լ�.
	// ��ȯâ. ���� �������� ���������� �κ� ������ Ŭ������ ��� ���ϰ� �߰�(07-02-20 ����)
	if( g_pNk2DFrame->IsItemMultiMoveEnable() ) // ������ ����Ŭ������ �����̵���Ű�� â���� ��������...
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

	// ������ ��񿡼� �������� ã�´�.
	for (i = 0; i < WEARING_NUM; i++)
	{
		pItem = m_Wearing[i];

		if (pItem && pItem->m_Index == index)
			return pItem;
	}

	///�̺�Ʈ�������� �ý����� ���ԵǾ������� �̺�Ʈ ���� ���Ե� �˻�.
	if( g_bUseEventWear )
	{
		for (i = 0; i < WEARING_NUM; i++)
		{
			pItem = m_EventWearing[i];

			if (pItem && pItem->m_Index == index)
				return pItem;
		}
	}

	// extra���� ã�´�.
	if (m_ExtraSlot && m_ExtraSlot->m_Index == index)
		return m_ExtraSlot;

	// Inven 1,2,3���� ã�´�.
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

	// ��ȯâ���� ã�´�.
	pItem = m_ExgInven;

	while (pItem)
	{
		if (pItem->m_Index == index)
			return pItem;
		else
			pItem = pItem->m_Next;
	}

	// Quick Slot üũ
	for (i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		pItem = m_QuickSlot[i];

		if (pItem && pItem->m_Index == index)
			return pItem;
	}

	return NULL;
}

// ��� ����ǰâ������ �������� ã�´�.
// �������� �������� �����ش�.
// ���ϵ� �����ۿ� ���� ���� ����.
CItem *CNkCharacter::FindItemByIndexFromAll(int index)
{
	int temp = 0;
	return FindItemAndPackByIndexFromAll(index, temp);
}

// Vnum ���� ���� �˻��ϴ� �Լ�. ������ ������ ���� �ϳ��� �������ش�.
CItem *CNkCharacter::FindItemByVnumFromAll( int vnum ) ///�δ�1
{
	int i = 0;
	CItem *pItem = NULL;

	// ������ ��񿡼� �������� ã�´�.
	for (i = 0; i < WEARING_NUM; i++)
	{
		pItem = m_Wearing[i];

		if (pItem && pItem->m_Vnum == vnum)
			return pItem;
	}

	///�̺�Ʈ�������� �ý����� ���ԵǾ������� �̺�Ʈ ���� ���Ե� �˻�.
	if( g_bUseEventWear )
	{
		for (i = 0; i < WEARING_NUM; i++)
		{
			pItem = m_EventWearing[i];

			if (pItem && pItem->m_Vnum == vnum)
				return pItem;
		}
	}

	// extra���� ã�´�.
	if (m_ExtraSlot && m_ExtraSlot->m_Vnum == vnum)
		return m_ExtraSlot;

	// Inven 1,2,3���� ã�´�.
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

	// ��ȯâ���� ã�´�.
	pItem = m_ExgInven;

	while (pItem)
	{
		if (pItem->m_Vnum == vnum)
			return pItem;
		else
			pItem = pItem->m_Next;
	}

	// Quick Slot üũ
	for (i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		pItem = m_QuickSlot[i];

		if (pItem && pItem->m_Vnum == vnum)
			return pItem;
	}

	return NULL;
}

// �ش� �������� ��� ����ִ��� ã���ִ� �Լ�. (����� �κ�(0~2)�� ��ȯâ(3) �� ����.
int CNkCharacter::CheckWherePackItem( CItem *DestItem ) ///�δ�1
{
	int i = 0;
	CItem *pItem = NULL;

	if( !DestItem )
		return -1; // ����.
	/*
		// ������ ��񿡼� �������� ã�´�.
		for (i = 0; i < WEARING_NUM; i++) {
			pItem = m_Wearing[i];

			if (pItem && pItem == DestItem )
				return ;
		}
		// extra���� ã�´�.
		if (m_ExtraSlot && m_ExtraSlot == DestItem)
			return ;
	*/
	// Inven 1,2,3���� ã�´�.
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

	// ��ȯâ���� ã�´�.
	pItem = m_ExgInven;

	while (pItem)
	{
		if (pItem == DestItem)
			return 3;
		else
			pItem = pItem->m_Next;
	}

	return -1; // ����.
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

// [6/1/2007 Theodoric] ������ ���� :: �Լ� �Ķ���� ������ ����
void CNkCharacter::SetExtraSlot(int index, int vnum, int gem_num, int special, int special2,
								int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur) ///�Ź���ȭ�ý���
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
		////2003 ����������� �÷��� ���������� �ν��ϴ°� ����.
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

		///�Ź���ȭ�ý���
		m_ExtraSlot->m_Special2 = special2; // �߰� �ɼ�
		m_ExtraSlot->m_UpgradeEndurance = UpEndur;           // ���� ��ȭ ������
		m_ExtraSlot->m_MaxUpgradeEndurance = MaxUpdEndur;    // ��ȭ ������ �ִ�ġ

		m_ExtraSlot->m_Endurance = Endur;
		m_ExtraSlot->m_MaxEndurance = MaxEndur;

		// ������ ���� �Է��غ���!
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
	// ���� �� �� �����...
	for (int i = 0; i < WEARING_NUM; i++)
		SAFE_DELETE(m_Wearing[i]);

	// ��� ���� ������...
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

	// ��ȯ ������ ���� ������.
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

	// Extra Slot ���ְ�...
	SAFE_DELETE(m_ExtraSlot);
	// Quick Slot ���ݴ�!
	for ( int i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
	{
		SAFE_DELETE(m_QuickSlot[i]);
		m_QuickCount[i] = 0;
	}
}
// [2008/8/14 Theodoric] ��ڰ� �κ��� �ս��� ����� ���� ���� ��Ŷ
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

	// �԰� �ִ� ���߿��� ���� ����!
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

	// �κ����� ������ ������!
	for (int i = 0; i < PACK_NUM; i++)
	{
		for (int vnum = 614; vnum <= 617; vnum++)
		{
			while ( (pItem = PickItemInPack(i, vnum, TRUE) ) != NULL)
			{
				delete pItem;
			}
		}

		// ���̺긮�� �������⵵ �˻��ؼ� ����.  (by ����)
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
	// ��� �����ѵ��� ��ȯ�ϸ鼭 ��������Ŷ�� ���ڸ� ���Ѵ�.
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
	// by evilkiki 2009.09.14 : �Ѵ���?
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

void CNkCharacter::SetCash( int Cash ) ///�������
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

void CNkCharacter::DeleteEffectNum( int num ) // �ش� ĳ���Ϳ� ��ϵ� ����Ʈ�� ����Ʈ��ȣ(�����ε���)�� ã�Ƽ� �����ִ� �༮. ������ �̰� ������. ��, ���� ����Ʈ���� ������� ��� �޸𸮰� �׿����ٴ� ��... ///�� �� �۾�
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

	if (m_ExtraSlot)	// ���� ������ extre slot �� �̿��ϱ� ������, ������ ������ �ȵȴ�.
		return FALSE;
	else
	{
		m_ExtraSlot = PickWeaponInPack(SWAP_PACK_NUM, x, y); // 3�� �κ�â������ ������.
		if (!m_ExtraSlot)
			return FALSE;

		if ((where = Wear(-1)) < 0)   // ������ �� ������ �ٽ� �κ��� �ִ´�.
		{
			AddToInven(x, y, SWAP_PACK_NUM);
			return FALSE;
		}

		if (!m_ExtraSlot)
		{
			// ���� ���� �ȵ�� �־�������?
			sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x, y);
			g_pTcpIp->SendNetMessage(strTemp);
			sprintf(strTemp, "wear %d\n", where);
			g_pTcpIp->SendNetMessage(strTemp);
		}
		else if (!AddToInven(SWAP_PACK_NUM, &x2, &y2))
		{
			// ���� ���Կ� �ִ��� 3�� �κ�â�� �ִ´�.
			// �ٵ�... ��������... TT �ٽ� ���� ���Կ� �ִ� �� ������...
			// �κ��� ���� �־����....
			if (Wear(-1) >= 0)
			{
				// �ٽ� �����ϰ�...
				AddToInven(x, y, SWAP_PACK_NUM); // �κ��� �ٽ� �ְ�...
				return FALSE;
			}
			else
			{
				// �ٽ� ���� ����? �� �ַ�.. �Ѥ�;
				sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x, y);
				g_pTcpIp->SendNetMessage(strTemp);
				sprintf(strTemp, "wear %d\n", where);
				g_pTcpIp->SendNetMessage(strTemp);
				return FALSE;
			}
		}
		else
		{
			// �� �Ǿ���. ������ �޽����� ��������!
			sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x, y);
			g_pTcpIp->SendNetMessage(strTemp);
			sprintf(strTemp, "wear %d\n", where);
			g_pTcpIp->SendNetMessage(strTemp);
			sprintf(strTemp, "inven %d %d %d\n", SWAP_PACK_NUM, x2, y2);
			g_pTcpIp->SendNetMessage(strTemp);
			//mungmae-2008/08/23 ��ٿ� �߿� ���� �ٲٸ� ���.
			if( pCMyApp->m_pUIMgr->m_bRClickDown == 1 && pCMyApp->DrawCasting )
			{
				HellDownFail( g_pRoh, NULL );
			}
			// nate 2005-05-19 : 'x'��ư�� �̿��� ���� ��ȯ�� ���� �� ����
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

// Į�� ������ �����.
// Lyul_2005 ����
// �̵��� ������ left right �� �ΰ� �ʿ信 �ٶ� �¿츦 �ٲ� �����Ѵ�.
// Default : left = true
void CNkCharacter::SetTraceLineLVertex(int linetype, D3DVECTOR *pTargetVector, bool left)  // left = true
{
	CCapsyong *LineCsy = NULL;

	// �̵����� ���� �¿츦 �޸� �����Ѵ�.
	// nate 2005-06-10 : Burning Crash - ����
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

		// ������ ���ϴ� �ؽ��ķ� ������� �簢��
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
			 // nate 2005-06-10 : Burning Crash - ����
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

		// ���
		for( int twofaces = 0; twofaces < 2; twofaces++ )
		{
			if( twofaces == 0 )
			{
				// ����� �Ϲ� ������ Color
				if( m_bBleed
						// nate 2005-06-16 : Burning Crash - ���� ���� ����
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
					   )  ///�Ϻ����������׽�Ʈ ������ ���󸶻�� ������ ���������� ��������.
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
				// ����� �Ϲ� ������ Color
				if( m_bBleed
						// nate 2005-06-16 : Burning Crash - ���� ���� ����
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
					   )  ///�Ϻ����������׽�Ʈ ������ ���󸶻�� ������ ���������� ��������.
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
	else if( linetype == TRACELINE_DASHSLASH ) ///�����޺�
	{
		// �� Ÿ���� ������ �� ������ ������ �ƴ϶� ������Ų��.
		// �뽬�������� �ߵ��Ǿ����� m_drawLineNum�� 0���� �ʱ�ȭ ��Ű��
		// �뽬�������� ���������� m_DrawWeaponEffects�� 0�� �ǹǷ� ����� �ȵǰ� �ȴ�.(0�� �ƴ϶� �ٸ� ����Ʈ�� ������ �ٸ��뼱 m_TraceLine�� ó������ �ٽ� ������ �ǹǷ� �������)
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

		int MaxHalfNum = ((MAX_TRACELINE_FRAME * 4) + 4)/2; // ���ý� �迭�� ���� ũ��. ���� ������ �� ���� �κ� ���� ����.

		if( m_drawLineNum >= MaxHalfNum ) // �����̻� �׸����� �ϸ� ���´�.(������ �������� ����ؾ� �ϴϱ�.)
			return;

		frame = m_CurFrame;

		// �¿� ������ �ؼ� ó���� ���������Ѵ�.
		if( LineCsy ) // �켱 ���� ��.( ������ �⺻���� ������ ĸ���� ���õǾ��ִ�. )
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

			m_TraceLine[m_drawLineNum] = D3DLVERTEX( m_LineVector[0], 0xffffffff, 0, pattern_tu, 0.0f ); // �߰����� ���.
			m_TraceLine[m_drawLineNum+1] = D3DLVERTEX( m_LineVector[1], 0xffffffff, 0, pattern_tu, 1.0f );
		}

		if( m_Wearing[WEARING_WEAPON] && GET_SUB_CSY(m_Wearing[WEARING_WEAPON]) ) // ���� ���� ��.( �����˰��� Ʋ���� �ఢ�� ó���� �� �ʿ��ϴ�. )
		{
			LineCsy = GET_SUB_CSY(m_Wearing[WEARING_WEAPON]); // ĸ���� �������� �ٲ��ְ�...

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

			m_TraceLine[m_drawLineNum+MaxHalfNum] = D3DLVERTEX( m_LineVector[0], 0xffffffff, 0, pattern_tu, 0.0f ); // �߰����� ���.
			m_TraceLine[m_drawLineNum+MaxHalfNum+1] = D3DLVERTEX( m_LineVector[1], 0xffffffff, 0, pattern_tu, 1.0f );
		}

		m_drawLineNum += 2; // �׷��� �� ���� ����. 2�� �����Կ� ����.

		// Į�󰪰� �ؽ��� ��ǥ�� �������ش�.
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
	else if( linetype == TRACELINE_SLASHSHADOW ) ///�����޺�
	{
		// �� Ÿ���� ������ �� ������ ������ �ƴ϶� ������Ų��.
		// �뽬�������� �ߵ��Ǿ����� m_drawLineNum�� 0���� �ʱ�ȭ ��Ű��
		// �뽬�������� ���������� m_DrawWeaponEffects�� 0�� �ǹǷ� ����� �ȵǰ� �ȴ�.(0�� �ƴ϶� �ٸ� ����Ʈ�� ������ �ٸ��뼱 m_TraceLine�� ó������ �ٽ� ������ �ǹǷ� �������)
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

		totalframe = 48; // �ϴ� ȸ�� ����� ������ ������.

		if( m_CurFrame < totalframe ) // ���� ȸ�����̸�...
			totalframe = m_CurFrame; // ���� �����ӱ�����...

		stepframe = 28;

		if( m_CurFrame-15 > stepframe ) // ���� ȸ�����̸�...
			stepframe = m_CurFrame-15; // ���� �����ӱ�����...


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

			m_TraceLine[numVertex++] = D3DLVERTEX( m_LineVector[0], color, 0, pattern_tu, 0.0f ); // �߰����� ���.
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
// ��Ƽ�������� �޾ƿ´�. (��� ��Ƽ������ ������ ������� ���ٷ� ���´�.)
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

	str = AnyOneArg(str, id); // �ӽ÷� ��Ƽ�� �ƽ� ī��Ʈ�� ����.
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
// nate 2004 - 4 : Crash_Miss â �׸���
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

	//mungmae-2005/05/06 ������ ũ���� ����Ʈ ��� �޺� ����Ʈ�� �־���.
	//5~8������ �� �����ϸ� ������ �޺� ����Ʈ
	if( m_pCrash_MissPopup[ nI ] )
	{
		if( m_byShowCrash_Miss >= 5 && m_byShowCrash_Miss <= 8)
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_COMBO ] ), nElapTime );

		//9~12������ �� �����ϸ� ������ �޺� ����Ʈ
		else if( m_byShowCrash_Miss >= 9 && m_byShowCrash_Miss <= 12)
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_COMBO2 ] ), nElapTime );
		// Miss
		else if( m_byShowCrash_Miss == 2 )
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_MISS ] ), nElapTime );
		// Block ///��
		else if( m_byShowCrash_Miss == 3 )
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_BLOCK ] ), nElapTime );
		// nate 2005-05-09 : �̺�Ʈ����Ʈ(EP)ȹ��� ���� �޼����� ���� ��Ÿ��
		else if( m_byShowCrash_Miss == 20 )
			m_pCrash_MissPopup[ nI ]->Draw( vSrc.x, vSrc.y,
											g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_EVENTPOINT ] ), nElapTime );
		else if( m_byShowCrash_Miss == 21 ) ///������� ���� ǥ��
			m_pCrash_MissPopup[ nI ]->Draw2( vSrc.x, vSrc.y,
											 g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FREEPVP_WINNER ] ), nElapTime );
		else if( m_byShowCrash_Miss == 22 ) ///������� ���� ǥ��
			m_pCrash_MissPopup[ nI ]->Draw2( vSrc.x, vSrc.y,
											 g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FREEPVP_LOSER ] ), nElapTime );
	}
}

void CNkCharacter::SetShowCrash_Miss( BYTE byCrash_Miss )
// Crash���� Miss ���� �÷��� ����
{
	// Crash�� �׸��� �ִ� �������� üũ
	// �׸��� ���� ������
	if( m_byShowCrash_Miss == 0 )
	{
		// Crash or Miss �÷��� ����
		m_byShowCrash_Miss = byCrash_Miss;
		m_dwElapTime = timeGetTime();
	}
}


void CNkCharacter::RenderCrash_Miss()
{
	m_nElapTime = ( timeGetTime() - m_dwElapTime ) / 5;

	// Ŀ���� ũ�� ����
	if( m_byShowCrash_Miss == 21 || m_byShowCrash_Miss == 22 ) ///������� ����,���� ǥ�ô� �ð��� �����.
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
		if( m_nElapTime > 100 ) // 05-02-01 / 50���� 100���� ��ġ �ø�( ������:�̿��� )
		{
			m_byShowCrash_Miss = 0;
			m_dwElapTime = 0;
			return;
		}
	}

	// ���� ũ�� ����
	if( m_byShowCrash_Miss == 21 || m_byShowCrash_Miss == 22 ) ///������� ����,���� ǥ�ô� �ϳ��� ����.
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

// nate 2005-06-17 : Burning Crash - ��ų�� ����( ���� ) ����
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
		m_nSkillStep = nSkillStep;	// nate 2005-06-27 : Auto Mine - �޸�(���ΰ���)
}

void CNkCharacter::SetFailStep( int nFailStep )
{
	m_nFailStep = nFailStep;
}

// nate 2005-06-18 : ��ų ��������� üũ
BOOL CNkCharacter::IsCantMove()
{
	// ���ǻ��� : ��ų �����߿� �����ϼ� ���� ��ų-��- �߰��Ұ�
	// CHA_HELLCRY_ATTACK ���Ŀ��� ������ �������� ������ ����
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

//mungmae-2005/06/23 ����콺 �� ����(���� �ð��� ����)
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
	// Ÿ������ ����
	m_tx = tx;
	m_tz = tz;
	float dist_x = m_tx - m_wx;
	float dist_z = m_tz - m_wz;

	// ĳ���� ������ ���Ѵ�.
	if( nSkillIndex != SKILL_KNOCKBACK && nSkillIndex != SKILL_KNOCKBACK2 ) // �˹��� ���� ��ȯ ���ϰ�... ///��
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
	else if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // �˹� ///��
	{
		sc_rate = scalar / 13.0f;
		fFrame = &m_fDash_Frame;
	}
	else if( nSkillIndex == SKILLF_A_DASH )
	{
		sc_rate = scalar / 13.0f;
		fFrame = &m_fDash_Frame;
	}
	else if( nSkillIndex == SKILL_F_DASHSLASH )  ///�����޺�
	{
		sc_rate = scalar / 20.0f; // �뽬���� ���� ������.
		fFrame = &m_fDash_Frame;
	}
	else if( nSkillIndex == SKILL_F_SHADOWSLASH )
	{
		sc_rate = scalar / 130.0f;
		fFrame = &m_fShadow_frame;
	}
	else
		return;

	m_dx = dist_x / sc_rate;    // �̵��� ũ�⸦ ���Ѵ�
	m_dz = dist_z / sc_rate;	// �̵��� ũ�⸦ ���Ѵ�

	float dist = dist_x != 0.0f ? dist_x : dist_z;
	float distMod = m_dx != 0.0f ? m_dx : m_dz;

	if( nSkillIndex == SKILL_B_FLYINGCRASH )
		*fFrame = dist / distMod - 2.0f; // �̵��� Ƚ���� ���Ѵ� ( ��Ȯ�� ������ ���� float)
	else if( nSkillIndex == SKILL_F_SHADOWSLASH )
		*fFrame = dist / distMod - 0.2f;
	else if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // �˹� ///��
		*fFrame = dist / distMod - 0.2f;
	else if( nSkillIndex == SKILLF_A_DASH )
		*fFrame = dist / distMod;
	else if( nSkillIndex == SKILLF_A_DASH || nSkillIndex == SKILL_F_DASHSLASH ) ///�����޺�
		*fFrame = dist / distMod;

	if( nSkillIndex != SKILLF_A_DASH && nSkillIndex != SKILL_KNOCKBACK && nSkillIndex != SKILL_KNOCKBACK2  ) ///��
	{
		m_bAttack = TRUE;		// ���� ��ų ���� üũ
		m_CurAttackType = nSkillIndex;
	}

	if( g_pRoh == this )
		pCMyApp->StopHero( FALSE ); // ���� FALSE �߰�. ���ڸ� ���� ������ stop��Ŷ�� �ѹ� �� ������ �̶����� �ٸ� ��� ȭ�鿡�� �����ϵ��� ���̴� ���װ� �־���.(06-03-18 ����)

	// ���� ����
	if( nSkillIndex == SKILL_B_FLYINGCRASH )		// Ŭ���� ũ����
		SetMotionState(CHA_B_FLYINGCRASH);
	else if( nSkillIndex == SKILL_F_SHADOWSLASH )	// ������ ������
		SetMotionState(CHA_F_SHADOWSLASH);
	else if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // �˹� ///��
		SetMotionState( CHA_KNOCKBACK ); //�˹��� ��ä�� �и���...
	else if( nSkillIndex == SKILLF_A_DASH )			// �뽬
		SetMotionState(CHA_F_DASH);
	else if( nSkillIndex == SKILL_F_DASHSLASH ) ///�����޺�
	{
		SetMotionState( CHA_F_DASHSLASH );
		SetShowCrash_Miss( 5 ); // �޺����� ����Ʈ(��) ���.

		this->m_drawLineNum = 0; // ����Ʈ�� ���� �ʱ�ȭ.
		this->m_dwComboTick = timeGetTime(); // �޺���ų üũ�� ƽ�� ����.

		// ���ʰ��� �ٸ� �������� ������ϰ� Ÿ�����ϴ°� �ڱ��ڽ��϶����ؾ��Ѵ�.
		if( g_pRoh == this ) // �ڱ��ڽ��̸�...
			pCMyApp->AttackMsgSend( SKILL_F_DASHSLASH ); // MvSkill�� ���� �� Ÿ���� ���ؼ� ������ �����ش�.(�׷��� ���� ����ȭ�� �´´�.)
	}

	m_vMvSkillStartPos = D3DVECTOR( m_wx, m_wy, m_wz ); // ���� ��ų�� �ߵ��Ǵ� ������ ��ġ. ///�����޺�
}

EffectSort* CNkCharacter::FindMyFairy( int Type ) //�ڱ� �ڽ��� �� ����Ʈ(����ü)�� ã���ִ� �Լ� ///�� // 07-01-30 Ÿ�� ���ڸ� �߰��Ͽ� ����
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

		if( pEffectSort->nNum == idx ) // �ڱ� �ڽ��� ������ ����Ʈ �� �ϳ����..
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
			  ) // �� ����Ʈ ã��
			{
				// ������ üũ
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_FAIRY ) // ����...(���� �� �̻��̸� �����ϴ�.)
				{
					return pEffectSort; // ���� �׳� ����.
				}
			}
			else if( Type >= SKILL_LUCKYPIG1 && Type <= SKILL_LUCKYPIG4 ) // ������ ã��
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_LUCKYPIG && pEffectSort->subNum == Type ) // ������ �߿��� �������� �´��� üũ�ؼ�
				{
					return pEffectSort; // ���ϴ� �������� ����.
				}
			}
			else if( Type >= SKILL_BABYSOUL_1 && Type <= SKILL_BABYSOUL_3 ) // [7/31/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ ���̺� �ҿ�
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_BABYSOUL && pEffectSort->subNum == Type ) // �������� �´��� üũ�ؼ�
				{
					return pEffectSort;
				}
			}
			else if( Type == SKILL_VAMPIREBAT ) ///�����̾���� ����
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_VAMPIREBAT )
				{
					return pEffectSort;
				}
			}
			else if( Type == SKILL_PETRUDOLPH             ///��絹��_����   // [12/10/2007 parkmj] �̴Ϸ絹�� 4�� �߰�(����, �Ķ�, ���, ���)
					 ||	Type == SKILL_PETRUDOLPH_BLACK    ///��絹��_����
					 ||	Type == SKILL_PETRUDOLPH_BLUE     ///��絹��_�Ķ�
					 ||	Type == SKILL_PETRUDOLPH_GREEN    ///��絹��_���
					 ||	Type == SKILL_PETRUDOLPH_YELLOW
					 || Type == SKILL_PETRUDOLPH_KOR_PREMIUM) ///��絹��_���
			{
				if( pEffectSort->nType == FX_FOLLOW && pEffectSort->Index == FX_FOLLOW_TYPE_RUDOLPH )
				{
					return pEffectSort;
				}
			}
			// [12/4/2007 parkmj] �Ϻ� �������� ����Ʈ
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

EffectSort* CNkCharacter::FindMySpirit() //�ڱ� �ڽ��� �� ����Ʈ(����ü)�� ã���ִ� �Լ� ///��
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

		if( pEffectSort->nNum == idx ) // �ڱ� �ڽ��� ������ ����Ʈ �� �ϳ����..
		{
			// ������ üũ
			if( pEffectSort->nType == FX_SPIRITMODE2 && pEffectSort->Index == 0 ) // ����...(���� �� �̻��̸� �����ϴ�.)
			{
				return pEffectSort; // ���� �׳� ����.
			}
		}
		it++;
	}

	return NULL;
}

BOOL CNkCharacter::IsDrawPicking() ///�������
{
	if( g_pRoh )
	{
		if( g_pRoh->m_bNowFreePVP ) // �ڱⰡ ������� ���� ���
		{
			if( timeGetTime()-g_pRoh->m_dwRingCreateTick < 5000 ) // ī��Ʈ �߿� ������ ��������..
				return FALSE;
			else if( this->m_nCharIndex != g_pRoh->m_dwPvpDestIndex ) // ������ �ƴ� ���
				return FALSE;
		}
		else // ������� ���� �ƴѰ��
		{
			if( this->m_bNowFreePVP ) // ������� ���� �༮�� ��������.
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CNkCharacter::EventWearingRender( int where ) // Ư�� ��ġ�� �̺�Ʈ ������ �׸���. ///�̺�Ʈ��������
{
	if( !g_bUseEventWear  ) // �̺�Ʈ ���� ���� �ý����� ���� ���ϴ� ����� �۵����ϰ�...
		return FALSE;

	if( !m_bEventWearFlag ) //g_dwLangType == LANG_KOR )
		return FALSE;

	//	if( this != g_pRoh ) // �ڱ� �ڽŸ� �̺�Ʈ ������ �̰ɷ� �׸���.(�ٸ� �̴� �������� ó�����ش�.)
	//		return FALSE; // �̷��� �ϸ� ���� ������ ������ش�.

	//  [6/19/2009 ppmmjj83] �Ϻ� �������� �����Ǵ� ������ �ϵ��ڵ�
	if( m_EventWearing[where] )
	{
		if( m_EventWearing[where]->m_Vnum >= 2320 && m_EventWearing[where]->m_Vnum <= 2323 ) return FALSE;
	}

	if( where == WEARING_SHOES ) // ����� ���� �� �Ӽ��� üũ������Ѵ�.(���� ����üũ���� �����ؾ��Կ� ����) ///070723 �Ϻ����׸���Ʈ ���� �� ����
	{
		if( m_EventWearing[WEARING_PANTS] )
		{
			if( (GET_RENDER_FLAG(m_EventWearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // �̰�� ��� �׸��� �ʴ´�.
			{
				m_bEventWearRendered[where] = true;
				return TRUE; // ���� ���嵵 ��� ���Ѵ�.
			}
		}
		else if( m_Wearing[WEARING_PANTS] )
		{
			if( (GET_RENDER_FLAG(m_Wearing[WEARING_PANTS]) & ARMOR_REMOVE_FEET) ) // �̰�� ��� �׸��� �ʴ´�.
			{
				m_bEventWearRendered[where] = true;
				return TRUE; // ���� ���嵵 ��� ���Ѵ�.
			}
		}
	}

	CItem* wearItem = m_EventWearing[where];

	// �켱�� �ش� ��ġ�� �̺�Ʈ ������ �����ϰ� �ִ��� üũ�Ѵ�.
	if( !wearItem )
		return FALSE; // �̷��� �ϸ� ���� ������ ������ش�.

	m_bEventWearRendered[where] = true;

	if( IsAlphaWear(where, true) )
	{
		return TRUE;
	}

	// �Ϻ� �̺�Ʈ ���� ����ó��.
	if( !m_Chaotic_Grade // ī���ϰ��� ����
			&& ( IS_LIGHT_OFF(m_EventWearing[where]) ) // [6/19/2008 �����] �ǻ� �����۵��� �׸��� ���� ���� üũ
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
		GET_CSY(m_EventWearing[where], m_BpdRace)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM(wearItem,m_BpdRace), FALSE ); // �̺�Ʈ ������ ����Ѵ�.

	if( !m_Chaotic_Grade // ī���ϰ��� ����
			&& ( IS_LIGHT_OFF(m_EventWearing[where]) ) // [6/19/2008 �����] �ǻ� �����۵��� �׸��� ���� ���� üũ
	  )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );	

	if( wearItem->m_Vnum == 1588 ) ///�Ϻ����������׽�Ʈ ���Ϸ��� ���ж��..
	{
		// ����Ʈ�� ���⼭ ���� ó�� ���ش�.
		DrawInstanceEffect(  1588, this, GET_CSY(wearItem, m_BpdRace) );
	}
	else if( m_EventWearing[where]->m_Vnum == 1589 ) ///�Ϻ����������׽�Ʈ ���
	{
		// ����Ʈ�� ���⼭ ���� ó�� ���ش�.
		DrawInstanceEffect(  1589, this, GET_CSY(wearItem, m_BpdRace) );
	}
	
	return TRUE;
}

BOOL CNkCharacter::EventWearingFrameMove( int where ) // Ư�� ��ġ�� �̺�Ʈ ������ �׸���. ///�̺�Ʈ��������
{
	if( !g_bUseEventWear )
		return FALSE;

	if( !m_bEventWearFlag )
		return FALSE;

	//	if( this != g_pRoh ) // �ڱ� �ڽŸ� �̺�Ʈ ������ �̰ɷ� �׸���.(�ٸ� �̴� �������� ó�����ش�.)
	//		return FALSE; // �̷��� �ϸ� ���� ������ ������ش�.

	// �켱�� �ش� ��ġ�� �̺�Ʈ ������ �����ϰ� �ִ��� üũ�Ѵ�.
	if( !m_EventWearing[where] )
		return FALSE; // �̷��� �ϸ� ���� ������ ������ش�.

	// [2/20/2008 �����] �̺�Ʈ �κ��丮�� �� ���� �߰�
	if( where != WEARING_RING )
	{
		if( GET_CSY(m_EventWearing[where], m_BpdRace) )
			GET_CSY(m_EventWearing[where], m_BpdRace)->Animate(m_Biped, m_CurFrame);
	}

	return TRUE;
}

void DrawInstanceEffect( int ItemIndex, CNkCharacter *pNkChar, CCapsyong *pTargetCsy/*CItem *TargetItem*/ ) // ����Ʈ ����Ʈ�� ���� �����Ǵ°� �ƴ϶� �ٷ� ������ִ� ����Ʈ ȿ��.(�����ۿ�.) ///�Ϻ����������׽�Ʈ ������ ȿ���� �ֱ� ���� �غ�ƴ�.
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

	if( ItemIndex == 1588 || ItemIndex == 3262 ) // ���Ϸ��� ����
	{
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTALPHA );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		float rectwidth = 5.0f; // vSortPos�� �����ϰ� �ִ°� ������ �� x���� ũ�Ⱚ���� ����Ѵ�.
		rectwidth += (rand()%10)*0.07f;
		// ������ �߰����� ���Ѵ�.

		lineCenter.x = (pTargetCsy->m_LineVector[0].x + pTargetCsy->m_LineVector[1].x) / 2;
		lineCenter.y = (pTargetCsy->m_LineVector[0].y + pTargetCsy->m_LineVector[1].y) / 2;
		lineCenter.z = (pTargetCsy->m_LineVector[0].z + pTargetCsy->m_LineVector[1].z) / 2;

		CRectPlane MainObject;    // ������� �� ���� �ִ� Ŭ����.
		color = 0x33ffffff;
		MainObject.Create( 5.0f, 5.0f, FALSE ); // ũ�⸦ ���Ƿ� �����س��´�.(�� ����Ʈ���� �������� ũ�⸦ �������ش�.)
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
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &pNkChar->m_matWorld ); // �̰� ��������Ѵ�.
	}
	if( ItemIndex == 1589 || ItemIndex == 3263 ) // ���
	{
		// ��� �κ�.
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
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &pNkChar->m_matWorld ); // �̰� ��������Ѵ�.


		// ��� �Һ�.
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTALPHA );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		float rectwidth = 5.0f; // vSortPos�� �����ϰ� �ִ°� ������ �� x���� ũ�Ⱚ���� ����Ѵ�.
		rectwidth += (rand()%10)*0.07f;
		/*
		// ������ �߰����� ���Ѵ�.
		lineCenter.x = (pTargetCsy->m_LineVector[0].x + pTargetCsy->m_LineVector[1].x) / 2;
		lineCenter.y = (pTargetCsy->m_LineVector[0].y + pTargetCsy->m_LineVector[1].y) / 2;
		lineCenter.z = (pTargetCsy->m_LineVector[0].z + pTargetCsy->m_LineVector[1].z) / 2;
		*/
		CRectPlane MainObject;    // ������� �� ���� �ִ� Ŭ����.
		color = 0x55ffff00;
		MainObject.Create( 5.0f, 5.0f, FALSE ); // ũ�⸦ ���Ƿ� �����س��´�.(�� ����Ʈ���� �������� ũ�⸦ �������ش�.)
		MainObject.ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), color, 0, 0.0f, 1.0f )  );
		MainObject.ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), color, 0, 0.0f, 0.0f )  );
		MainObject.ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), color, 0, 1.0f, 1.0f)  );
		MainObject.ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), color, 0, 1.0f, 0.0f )  );

		D3DVECTOR vLineDir = pTargetCsy->m_LineVector[1] - pTargetCsy->m_LineVector[0];
		MainObject.SetBillBoard( vViewVec, TRUE, 0.0f );

		vTmp.y -= 3.7f; // y�� ������ ��ġ.

		MainObject.SetTranslationXYZ( vTmp.x, vTmp.y, vTmp.z ); // �̳༮�� �̹� ĳ���� ��ġ�� ���Ե� �༮�̴�.

		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[EFF_FX_LIGHTFOG] );
		GET_D3DDEVICE()->SetTexture( 0, lpTexture );
		MainObject.Render();
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &pNkChar->m_matWorld ); // �̰� ��������Ѵ�.
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
		return LV300_BULKAN;	// ũ���� ��
	case 1576:
	case 1577:
	case 1578:
	case 1579:
		return LV300_KAILIPTON;	// ������
	case 1560:
	case 1561:
	case 1562:
	case 1563:
		return LV300_AIDIA;		// �̽ý�
	case 1572:
	case 1573:
	case 1574:
	case 1575:
		return LV300_HUMAN;		// ������
	case 1568:
	case 1569:
	case 1570:
	case 1571:
		return LV300_HYBRIDER;	// �÷�Ÿ		
	case 1797:
	case 1798:
	case 1799:
	case 1800:
		return RARE_BULKAN;		// ���丣�Ͽ�
	case 1805:
	case 1806:
	case 1807:
	case 1808:
		return RARE_KAILIPTON;	// Ŀ���
	case 1801:
	case 1802:
	case 1803:
	case 1804:
		return RARE_AIDIA;		// �����
	case 1813:
	case 1814:
	case 1815:
	case 1816:
		return RARE_HUMAN;		// ��īƮ
	case 1809:
	case 1810:
	case 1811:
	case 1812:
		return RARE_HYBRIDER;	// �帮�Ƶ�		
	case 1817:
	case 1818:
	case 1819:
	case 1820:
		return HEROSET;			// ���� ���	
	case 2180:
	case 2181:
	case 2182:
	case 2183:
		return T_RARE_BULKAN;		// ���丣�Ͽ�
	case 2188:
	case 2189:
	case 2190:
	case 2191:
		return T_RARE_KAILIPTON;	// Ŀ���
	case 2184:
	case 2185:
	case 2186:
	case 2187:
		return T_RARE_AIDIA;		// �����
	case 2196:
	case 2197:
	case 2198:
	case 2199:
		return T_RARE_HUMAN;		// ��īƮ
	case 2192:
	case 2193:
	case 2194:
	case 2195:
		return T_RARE_HYBRIDER;	// �帮�Ƶ�
		// ���� �� // GM��
	case 2200:
	case 2201:
	case 2202:
	case 2203:
		return GM_HEROSET;			// ���� ���	

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
		return LV300_BULKAN;	// ũ���� ��
	case 1576:
	case 1577:
	case 1578:
	case 1579:
		return LV300_KAILIPTON;	// ������
	case 1560:
	case 1561:
	case 1562:
	case 1563:
		return LV300_AIDIA;		// �̽ý�
	case 1572:
	case 1573:
	case 1574:
	case 1575:
		return LV300_HUMAN;		// ������
	case 1568:
	case 1569:
	case 1570:
	case 1571:
		return LV300_HYBRIDER;	// �÷�Ÿ		
	case 1797:
	case 1798:
	case 1799:
	case 1800:
		return RARE_BULKAN;		// ���丣�Ͽ�
	case 1805:
	case 1806:
	case 1807:
	case 1808:
		return RARE_KAILIPTON;	// Ŀ���
	case 1801:
	case 1802:
	case 1803:
	case 1804:
		return RARE_AIDIA;		// �����
	case 1813:
	case 1814:
	case 1815:
	case 1816:
		return RARE_HUMAN;		// ��īƮ
	case 1809:
	case 1810:
	case 1811:
	case 1812:
		return RARE_HYBRIDER;	// �帮�Ƶ�		
	case 1817:
	case 1818:
	case 1819:
	case 1820:
		return HEROSET;			// ���� ���	
	case 2180:
	case 2181:
	case 2182:
	case 2183:
		return T_RARE_BULKAN;		// ���丣�Ͽ�
	case 2188:
	case 2189:
	case 2190:
	case 2191:
		return T_RARE_KAILIPTON;	// Ŀ���
	case 2184:
	case 2185:
	case 2186:
	case 2187:
		return T_RARE_AIDIA;		// �����
	case 2196:
	case 2197:
	case 2198:
	case 2199:
		return T_RARE_HUMAN;		// ��īƮ
	case 2192:
	case 2193:
	case 2194:
	case 2195:
		return T_RARE_HYBRIDER;	// �帮�Ƶ�
		// ���� �� // GM��
	case 2200:
	case 2201:
	case 2202:
	case 2203:
		return GM_HEROSET;			// ���� ���	

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
	if(m_EventWearing[3] != NULL && m_EventWearing[3]->m_Vnum == 2048 ) // �̺�Ʈ ���忡 ���ڴ� ��� ���� �ߴٸ�
		return TRUE;

	if(m_Wearing[3] != NULL && m_Wearing[3]->m_Vnum == 2048 )
		return TRUE;

	return FALSE;
}

//  [8/16/2008 �����]
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
				if( m_Wearing[i]->m_Vnum == 1774 ) // ���� �����̾� ���並 �԰��־����� ����.
				{
					if( !(m_EventWearing[i] && m_EventWearing[i]->m_Vnum == 1774) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
						pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
				}
				if( m_Wearing[i]->m_Vnum == 11273 ) // ���� �����̾� ���並 �԰��־����� ����.
				{
					if( !(m_EventWearing[i] && m_EventWearing[i]->m_Vnum == 11273) ) // �̺�Ʈ ���並 �� �����ִ� ��츸...
						pCMyApp->m_pUIMgr->SkillOnOff( 0, TRUE, m_nCharIndex, SKILL_VAMPIREBAT ); // ���� ����Ʈ�� ���ش�.
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
				for( int y = pItem->m_SlotY ; y < pItem->m_SlotY+GET_ITEM_HEIGHT(pItem) ; ++ y ) // �����۸����� ������ �˻�.
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
	// ��ũ��,��
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
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1588 ///�Ϻ����������׽�Ʈ ������ ����ó��.
				 || m_Wearing[WEARING_SHIELD]->m_Vnum==1589 ///�Ϻ����������׽�Ʈ ������ ����ó��.
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
	m_fScaleSize = max(ratio, 0.0f);			// 1.0f ���ϴ� 1.0f��
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
