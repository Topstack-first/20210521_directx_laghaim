#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include "main.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "D3DMath.h"
#include "DirtSnd.h"
#include "Land.h"
#include "ClrAlloc.h"
#include "ChaSta.h"
#include "UIMgr.h"
#include "Frustum.h"
// nate 2004 - 4 : Crash_Miss
#include "AlphaPopUp.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h" ///점포상점 시 추가
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "FxSet.h"
#include "Skill.h"
#include "FxRedFire.h"
#include "DropStar.h"
#include "ItemProto.h"
#include "NKObjQuadNode.h"
#include "ShadowRect.h"
#include "g_stringmanager.h" ///튜토리얼 시 추가
#include "Country.h"

#include "LHGlobal_String.h"
#include "ItemOutlineMgr.h"

MobDataLoading * g_pMobDataLoading = NULL;

extern DWORD g_dwClientCountry;




extern float g_fTimeKey;
extern float g_fDSDistance;
extern DWORD g_dwBaseTime;
extern float g_ChaFlyHeight[];
extern CLolos	*g_FlyStones[];
extern CLolos	*g_pMattingLolo; ///돗자리

extern int g_MatrixSvr;
extern BOOL	g_bNewTerrain;
//서먼피션 몹크기조절
extern float g_SummonScale;
extern int g_nCurWorldIdx;

extern BOOL g_bGlobalMatrix;
extern int g_nEmptyTextureNum;

void NpcFileCollect( const char *FileName, const char *SrcPath, const char *MovePath ); // 특정 Npc Dta 에서 사용하는 파일들을 따로 모아주는 함수. ///국가대항전 전용 클라이언트 만들라고 추가.

//////////////////////////////////////////////////////////////////////
// 맙데이타 로딩.
//////////////////////////////////////////////////////////////////////

int CMobData::npc_count;

// -------------------------------------------------------------------
// 맙에 관련된 캡숑과 바이페드 로딩.
// -------------------------------------------------------------------
IndexedTexture *g_pMobTexture = NULL;

CLolos	*g_pMobLolo = NULL;

char *npc_data_file[29] =
{
	"data/Npc/npc0.dta",	"data/Npc/npc1.dta",	"data/Npc/npc2.dta",
	"data/Npc/npc3.dta",	"data/Npc/npc4.dta",	"data/Npc/npc5.dta",
	"data/Npc/npc6.dta",	"data/Npc/npc7.dta",	"data/Npc/npc8.dta",
	"data/Npc/npc9.dta",	"data/Npc/npc10.dta",	"data/Npc/npc11.dta",
	"data/Npc/npc12.dta",	"data/Npc/npc13.dta",	"data/Npc/npc14.dta",
	"data/Npc/npc15.dta",	"data/Npc/npc16.dta",	"data/Npc/npc17.dta",
	"data/Npc/npc18.dta",	"data/Npc/npc19.dta",	"data/Npc/npc20.dta",
	"data/Npc/npc21.dta",	"data/Npc/npc22.dta",	"data/Npc/npc23.dta",
	"data/Npc/npc24.dta",	"data/Npc/npc25.dta",	"data/Npc/npc26.dta",
	"data/Npc/npc27.dta",	"data/Npc/npc28.dta"
};

// 뮤턴트 색상
#define MOB_EFF_BLUE_DOT		0
#define MOB_EFF_RED				1
#define MOB_EFF_BLUE			2
#define MOB_EFF_GREEN			3
#define MOB_EFF_YELLOW			4
//====================================================
// nate 2004 - 4 : quest - 마신 다이오드 색상 변경
#define MOB_EFF_SKYBLUE			6
#define MOB_EFF_PINK			7
//====================================================
#define MOB_EFF_NUM				8

#define MOB_SHARK_STAND			67

#define SHOW_MOBCHATPOPUP_TIME	(7000)

int g_MobEffectTexture[ MOB_EFF_NUM ];
void DeletingMobLoading();

void MobInitialLoading(int world_num)
{
	DeletingMobLoading();
	// 텍스처 셋
//====================================================
	// nate 2004 - 4 : quest
	g_pMobTexture = new IndexedTexture(1000); // 06-11-22 부족해서 늘림. (원석)
//====================================================
	if( g_pMobTexture )
		g_pMobTexture->SetTexturePath("data/npc/textr/");

	// 데이타 로딩 클래스
	g_pMobDataLoading = new MobDataLoading;
	if( g_pMobDataLoading )
	{
		g_pMobDataLoading->SetBipedDir("data/Npc/bpd/");
		g_pMobDataLoading->SetCapDir("data/Npc/csy/");
		// nate 2004 - 10 : 최대 로딩 갯수 설정 - nMaxBipedNum의 갯수만 사용됨
		g_pMobDataLoading->InitSize(60, 600);

		char filename[64] = {0,};
		sprintf( filename , "data/Npc/npc%d.dta" , world_num );
		g_pMobDataLoading->ReadDataFile(filename);
	}

	// 변형몹을 위한 텍스춰 로딩...
	g_MobEffectTexture[ MOB_EFF_BLUE_DOT ] = g_pMobTexture->CreateTextureFromFile("war_arms.bmp");
	//Mutant
	g_MobEffectTexture[ MOB_EFF_RED ] = g_pMobTexture->CreateTextureFromFile("eff_red.bmp");
	//Mutant
	g_MobEffectTexture[ MOB_EFF_BLUE ] = g_pMobTexture->CreateTextureFromFile("eff_blue.bmp");
	//Mutant
	g_MobEffectTexture[ MOB_EFF_GREEN ] = g_pMobTexture->CreateTextureFromFile("eff_green.bmp");
	//Mutant
	g_MobEffectTexture[ MOB_EFF_YELLOW ] = g_pMobTexture->CreateTextureFromFile("eff_yellow.bmp");

//====================================================
	// nate 2004 - 4 : quest
	g_MobEffectTexture[ MOB_EFF_SKYBLUE ] = g_pMobTexture->CreateTextureFromFile( "eff_skyblue.bmp" );
	g_MobEffectTexture[ MOB_EFF_PINK ] = g_pMobTexture->CreateTextureFromFile( "eff_pink.bmp" );
//====================================================

	if( g_pMobTexture )
		g_pMobTexture->RestoreAllTextures(pCMyApp->GetDevice());

	if (!g_pMobLolo)
	{
		g_pMobLolo = new CLolos(g_pMobTexture);
		g_pMobLolo->Read("data/npc/lolo/j_moplight.LOL");
	}
}

void DeletingMobLoading()
{
	SAFE_DELETE(g_pMobDataLoading);
	SAFE_DELETE(g_pMobTexture);
	SAFE_DELETE(g_pMobLolo);
}

bool MobData_IsShop(int vnum)
{
	if( g_pMobDataLoading == NULL )
		return false;

	bool bResult = false;
	for( int i = 0; i < g_pMobDataLoading->m_nNpcNum; i++ )
	{
		if( g_pMobDataLoading->m_pMobData[i].nVnum == vnum )
		{
			if( g_pMobDataLoading->m_pMobData[i].flag & NPC_SHOPKEEPER )
			{
				bResult = true;
			}

			break;
		}
	}

	return bResult;
}

MobDataLoading::MobDataLoading()
	: m_nNpcNameNum(0)
{
	m_nMaxBipedNum = m_nCurBipedNum = 0;
	m_dpBiped = NULL;
	m_dpBipedName = NULL;
	m_nNpcNum = 0;
	m_pMobData = NULL;
	m_pBipedDir = NULL;
	m_pCapDir = NULL;
}

MobDataLoading::~MobDataLoading()
{
	// 하나의 바이페이드는 여러곳에서 쓰일 수 있다.
	for (int i=0; i < m_nCurBipedNum; i++)
	{
		SAFE_DELETE(m_dpBiped[i]);
		SAFE_DELETE_ARRAY(m_dpBipedName[i]);
	}

	SAFE_DELETE_ARRAY(m_dpBiped);
	SAFE_DELETE_ARRAY(m_dpBipedName);
	// 캡숑 파일 삭제.
	// Npc에 연결되어 있는 모든 갭숑을 삭제한다.
	for ( int i=0; i < m_nNpcNum; i++)
	{
		SAFE_DELETE_ARRAY(m_pMobData[i].mob_name);
		SAFE_DELETE(m_pMobData[i].pCapsyong);
		SAFE_DELETE(m_pMobData[i].pBound);
	}

	SAFE_DELETE_ARRAY( m_pMobData );

	SAFE_DELETE(m_pBipedDir);
	SAFE_DELETE(m_pCapDir);
}

void MobDataLoading::InitSize(int nMaxCapNum, int nMaxBipedNum)
{
	m_nMaxBipedNum = nMaxBipedNum;
	m_dpBiped = new CBiped *[nMaxBipedNum];
	m_dpBipedName = new char *[nMaxBipedNum];
	m_nCurBipedNum = 0;
}

void MobDataLoading::SetBipedDir(char *dir_name)
{
	SAFE_DELETE(m_pBipedDir);
	m_pBipedDir = new char[strlen(dir_name)+1];
	strcpy(m_pBipedDir, dir_name);
}

void MobDataLoading::SetCapDir(char *dir_name)
{
	SAFE_DELETE(m_pCapDir);
	m_pCapDir = new char[strlen(dir_name)+1];
	strcpy(m_pCapDir, dir_name);
}

void MobDataLoading::ReadDataFile(char *file_name)
{
//	NpcFileCollect( file_name, ".//data//npc//", ".//backup//" ); // 존별 npc 파일추출이 필요할때만 쓰면된다.

	PFILE *fp = NULL;
	PFILE *fpCap = NULL;
	FILE *fpSnd = NULL;
	char *buf = NULL;
	char path_name[255] = "";
	int npc_count = 0, len = 0;
	char fname[255] = "";

	fp = pfopen(file_name, "rb");
	if (fp == NULL)
	{
		MessageBox(NULL, "Failed to load map data", "error", MB_OK);
		return;
	}

	buf = new char[255];
	pfread(&npc_count, sizeof(int), 1, fp);
	m_nNpcNum = npc_count;
	CMobData::npc_count = npc_count;

	m_pMobData = new CMobData[npc_count];

	for (int i = 0; i < npc_count; i++)
	{
		pfread(&(m_pMobData[i].nVnum), sizeof(int), 1, fp);		// 맙 번호
		pfread(&len, sizeof(int), 1, fp);						// 화일 이름 길이.
		assert(len > 0);
		m_pMobData[i].mob_name = new char[len+1];
		pfread(m_pMobData[i].mob_name, len, 1, fp);				// 맙 이름 읽는다.
		m_pMobData[i].mob_name[len] = NULL;
		pfread(&len, sizeof(int), 1, fp);						// 캡송 화일 이름 길이.
		assert(len > 0);
		pfread(fname, len, 1, fp);								// 캡슝은 맙에 언제나 고유하게 하나만 있다.
		fname[len] = NULL;
		strcpy(path_name, m_pCapDir);
		strcat(path_name, fname);

		m_pMobData[i].pCapsyong = new CCapsyong(g_pMobTexture);
		if( VF_ERR_NONE > m_pMobData[i].pCapsyong->Read(path_name) )
		{
			SAFE_DELETE(m_pMobData[i].pCapsyong);
			m_pMobData[i].pCapsyong = NULL;
		}

		pfread(&len, sizeof(int), 1, fp);						// 텍스춰 화일 이름 길이.
		if (len > 0)
		{
			pfread(fname, len, 1, fp);
			fname[len] = NULL;
			if( g_pMobTexture && m_pMobData[i].pCapsyong != NULL)
			{
				int nObjCount = 0;
				for(nObjCount = 0; nObjCount < m_pMobData[i].pCapsyong->m_ObjectCount; nObjCount++)
				{
					m_pMobData[i].pCapsyong->m_Csy[nObjCount].m_TextureNum = g_pMobTexture->CreateTextureFromFile(fname);
				}
			}
//				m_pMobData[i].textureNum = g_pMobTexture->CreateTextureFromFile(fname);

		}
		else
			m_pMobData[i].textureNum = -1;


		pfread(&len, sizeof(int), 1, fp);						// 캡송 화일 이름 길이.
		if (len > 0)
		{
			pfread(fname, len, 1, fp);								// 캡슝은 맙에 언제나 고유하게 하나만 있다.
			fname[len] = NULL;
			strcpy(path_name, m_pCapDir);
			strcat(path_name, fname);

			m_pMobData[i].pBound = new CCapsyong(g_pMobTexture);
			if( VF_ERR_NONE > m_pMobData[i].pBound->Read(path_name) )
			{
				SAFE_DELETE(m_pMobData[i].pBound);
				m_pMobData[i].pBound = NULL;
			}
		}
		else
			m_pMobData[i].pBound = NULL;



		for (int j=0; j<7; j++) // [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5에서 7로 늘린다.
		{
			// j는 stand, move, attack, die, knock
			// 스탠드 바이페드
			pfread(&len, sizeof(int), 1, fp);						// 바이페드 화일이름 길이.
			assert(len > 0);
			pfread(fname, len, 1, fp);
			fname[len] = NULL;
			m_pMobData[i].pBpd[j] = LoadBiped(fname);				// 바이페드 포인터를 리턴한다.

			// 스탠드 사운드
			pfread(&len, sizeof(int), 1, fp);						// 사운드 파일 이름 길이.
			if ( len > 0)
			{
				// 사운드 파일이 있다.
				pfread(fname, len, 1, fp);
				fname[len] = NULL;
				// 사운드 아이디를 받아챈다.

				if (g_pDSound)
				{
					g_pDSound->SetWaveFilePath("data//Npc//wav//");
					m_pMobData[i].idxSound[j] = g_pDSound->LoadWAV(fname);
				}
				else
					m_pMobData[i].idxSound[j] = -1;
			}
			else
				m_pMobData[i].idxSound[j] = -1;

			pfread(&(m_pMobData[i].nSoundFrame[j]), sizeof(int), 1, fp);
		}

		// 맙 기타
		pfread(&(m_pMobData[i].attack_frame), sizeof(int), 1, fp);
		pfread(&(m_pMobData[i].speed), sizeof(int), 1, fp);
		pfread(&(m_pMobData[i].flag), sizeof(int), 1, fp);		//
		pfread(&(m_pMobData[i].height), sizeof(int), 1, fp);		// 높이 상/중/하
		pfread(&(m_pMobData[i].flystone), sizeof(int), 1, fp);  // 비행석
	}

	pfclose(fp);
	if( buf )
		delete [] buf;
}

CBiped *MobDataLoading::LoadBiped(char *file_name)
{
	char *path_name = NULL;
	PFILE *fp=NULL;

	// 먼저 기존에 로딩된 바이페드인지 파일이름으로 검사
	for (int idx=0; idx < m_nCurBipedNum; idx++)
	{
		if (!strcmp(m_dpBipedName[idx], file_name))
			return m_dpBiped[idx];
	}

	// 새로 바이페드 메모리를 할당한다.. 음..
	m_dpBipedName[m_nCurBipedNum] = new char[strlen(file_name)+1];
	strcpy(m_dpBipedName[m_nCurBipedNum], file_name);
	m_dpBiped[m_nCurBipedNum] = new CBiped;

	// 경로와 화일 이름을 연결한다.
	path_name = new char[strlen(m_pBipedDir)+strlen(file_name)+1];
	strcpy(path_name, m_pBipedDir);
	strcat(path_name, file_name);

	m_dpBiped[m_nCurBipedNum]->Read(path_name);

	CBiped *pBiped =  m_dpBiped[m_nCurBipedNum];
	m_nCurBipedNum++;
	// npc의 로딩 바이패드 최대 갯수를 업데이트 시켜야함
	assert(m_nCurBipedNum <= m_nMaxBipedNum);

	delete [] path_name; // 임시로 쓴 포인터 해제. (by 이원석) ///메모리누수수정

	return pBiped;
}

//////////////////////////////////////////////////////////////////////
// CMobData Construction/Destruction - 하는 일 없음.
//////////////////////////////////////////////////////////////////////
CMobData::CMobData()
	: nVnum(0)
	, mob_name(NULL)
	, attack_frame(0)
	, speed(0)
	, textureNum(0)
	, flag(0)
	, height(0)
	, flystone(0)
{
	memset(idxSound, 0, sizeof(idxSound));
	memset(nSoundFrame, 0, sizeof(nSoundFrame));

	pCapsyong = NULL;
	pBound = NULL;
	for(int i=0; i<7; i++)
		pBpd[i] = NULL;
}
CMobData::~CMobData() {}

//////////////////////////////////////////////////////////////////////
// CNkMob Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNkMob_ByPool::CNkMob_ByPool()
	: CNkMob()
{

}

CNkMob_ByPool::CNkMob_ByPool(CClrAlloc *pClrAlloc)
	: CNkMob(pClrAlloc)
{

}

CNkMob_ByPool::~CNkMob_ByPool()
{

}

// 이 클래스에서의 데이타 삭제는 없다.
CNkMob::CNkMob()
	: m_MobDataIdx(0)
	, m_PrevFrame(0)
	, m_dx(0)
	, m_dz(0)
	, m_tx(0)
	, m_tz(0)
	, m_dwTimeCapPrev(0)
	, m_MobSkill(0)
	, m_wColorIdx(0)
	, m_dwColorPick(0)
	, m_DropItemVnum(0)
	, m_DropItemX(0)
	, m_DropItemZ(0)
	, m_DropItemDir(0)
	, m_Flag(0)
	, m_wx(0)
	, m_wy(0)
	, m_wz(0)
	, m_dir(0)
	, m_FlyStone(0)
	, m_nAttackedTime(0)
	, m_MobClassType(0)
	, m_MobAttributeType(0)
	, m_ChaCondition(0)
	, m_CancelState(0)
	, m_ShowConditionEffect(FALSE)
	, m_ChaCondition2(0)
	, m_CancelState2(0)
	, m_ShowConditionEffect2(FALSE)
	, m_ReceiveBook(0)
	, m_fDash_Frame(0)
	, m_NowMoveSkill(0)
	, m_LastChatTime(0)
	, m_bShowMobChatPopup(FALSE)
{
	memset(m_nSoundIdx, 0, sizeof(m_nSoundIdx));
	memset(m_nSoundFrame, 0, sizeof(m_nSoundFrame));

	m_nMobNum = -1;
	m_nVirId = -10;
	m_CurFrame = 0;
	m_AniRate = 0.05f;
	m_fMoveSpeed = 0.0f;
	m_bMoving = FALSE;
	m_bAttack = FALSE;
	m_bAttacked = FALSE;
	m_pClrAlloc = NULL;
	m_AttackChaId = -1;
	m_AttackMobId = -1;
	m_AttackPetId = -1;
	m_AttackFrame = 0;
	m_AttackVital = 100;
	m_DeadFrame = 0;
	m_bHeroAttCome = FALSE;
	m_toDir = -100;
	m_Vital = 100;
//	m_DropItemIndex = -1;
	m_nMotionState = MOB_STAND;
	m_dwBaseTime = timeGetTime();
	m_LastAttackVitalTime = m_dwBaseTime;
	m_Height = MOB_HEIGHT_MIDDLE_NUM;
	m_MobAttackType = MOB_ATTACK_NORMAL;
	m_EffFrame = 0;
	m_Mutant = 0;
	m_bPet = FALSE;
	isControlLight = FALSE;
	m_pDropStar = NULL;
	m_nFlyFrame = rand() % CHA_FLY_FRAME;
	m_RunSmokeFrame = 0;
	m_bQuestMobDied = FALSE;
	m_bQuestMob = FALSE;
	m_NkMob_Particle = NULL;
	m_bDrawEffects = FALSE;
	// nate 2004 - 4 : Crash_Miss
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
		m_pCrash_MissPopup[ i ] = new CAlphaPopUp;
	m_NPC_Event = new CAlphaPopUp;
	m_byShowCrash_Miss = 0;
	m_nElapTime = 0;
	m_dwElapTime = 0;
	// nate 2004 - 4 : quest level
	m_nLevel = 0;
	m_FissionType = 0;
	m_bPoisoning = false;
	m_byInsertEffect = 0;
	m_pMobChatPopup = new CAlphaPopUp;
	if( m_pMobChatPopup )
	{
		m_pMobChatPopup->Init(12);
		m_pMobChatPopup->SetRectSoft(TRUE);
	}
	m_RegeningType = 0; // 리젠 중에 셋팅되는 리젠 연출 타임. 이 시간이 셋팅되어있으면...연출중인거다. ///인던1

	for( int i = 0 ; i < 8 ; i ++ )
		m_pBiped[i] = NULL;

	m_pCapSyong = NULL;
	m_pBound = NULL;
}

CNkMob::CNkMob(CClrAlloc *pClrAlloc)
	: m_MobDataIdx(0)
	, m_dx(0)
	, m_dz(0)
	, m_tx(0)
	, m_tz(0)
	, m_dwTimeCapPrev(0)
	, m_MobSkill(0)
	, m_DropItemVnum(0)
	, m_DropItemX(0)
	, m_DropItemZ(0)
	, m_DropItemDir(0)
	, m_Flag(0)
	, m_wx(0)
	, m_wy(0)
	, m_wz(0)
	, m_dir(0)
	, m_FlyStone(0)
	, m_nAttackedTime(0)
	, m_MobClassType(0)
	, m_MobAttributeType(0)
{
	memset(m_nSoundIdx, 0, sizeof(m_nSoundIdx));
	memset(m_nSoundFrame, 0, sizeof(m_nSoundFrame));

	m_nMobNum = -1;
	m_nVirId = -10;
	m_CurFrame = 0;
	m_PrevFrame = 0;
	m_AniRate = 0.05f;
	m_fMoveSpeed = 0.0f;
	m_bMoving = FALSE;
	m_bAttack = FALSE;
	m_bAttacked = FALSE;
	m_AttackChaId = -1;
	m_AttackMobId = -1;
	m_AttackPetId = -1;
	m_AttackFrame = 0;
	m_AttackVital = 100;
	m_Vital = 100;
	m_DeadFrame = 0;
	m_bHeroAttCome = FALSE;
	m_toDir = -100;
//	m_DropItemIndex = -1;
	m_pClrAlloc = pClrAlloc;

	if (m_pClrAlloc)
		m_pClrAlloc->ColorAlloc(&m_dwColorPick, &m_wColorIdx);
	else
	{
		m_wColorIdx = 0;
		m_dwColorPick = 0x00ffffff;
	}
	m_nMotionState = MOB_STAND;
	m_dwBaseTime = timeGetTime();
	m_LastAttackVitalTime = m_dwBaseTime;
	m_MobAttackType = MOB_ATTACK_NORMAL;
	m_Height = MOB_HEIGHT_MIDDLE_NUM;
	m_EffFrame = 0;
	m_Mutant = 0;
	m_bPet = FALSE;
	isControlLight = FALSE;
	m_ShowConditionEffect = FALSE;
	m_ChaCondition = 0;
	m_CancelState = 0;
	m_ShowConditionEffect2 = FALSE;
	m_ChaCondition2 = 0;
	m_CancelState2 = 0;
	m_ReceiveBook = 0;
	m_pDropStar = NULL;
	m_nFlyFrame = rand() % CHA_FLY_FRAME;
	m_RunSmokeFrame = 0;
	m_bQuestMobDied = FALSE;
	m_bQuestMob = FALSE;
	m_NkMob_Particle = NULL;
	m_bDrawEffects = FALSE;
	// nate 2004 - 4 : Crash_Miss
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
		m_pCrash_MissPopup[ i ] = new CAlphaPopUp;
	m_NPC_Event = new CAlphaPopUp;
	m_byShowCrash_Miss = 0;
	m_nElapTime = 0;
	m_dwElapTime = 0;
	// nate 2004 - 4 : quest level
	m_nLevel = 0;
	m_FissionType = 0;
	m_bPoisoning = false;
	m_byInsertEffect = 0;
	m_fDash_Frame = 0.0f; // 넉백을 위한 변수.
	m_NowMoveSkill = 0; // 이동형 스킬 적용 여부.(몬스터는 모션 스테이트로 구분 안하고 이걸로 구분해서 넉백등 시켜준다.)
	m_pMobChatPopup = new CAlphaPopUp;
	if( m_pMobChatPopup )
	{
		m_pMobChatPopup->Init(12,AlphaPopUp_Type__Char);
		m_pMobChatPopup->SetRectSoft(TRUE);
	}
	m_LastChatTime = timeGetTime() - SHOW_MOBCHATPOPUP_TIME;
	m_bShowMobChatPopup = FALSE;
	m_RegeningType = 0; // 리젠 중에 셋팅되는 리젠 연출 타임. 이 시간이 셋팅되어있으면...연출중인거다. ///인던1

	for( int i = 0 ; i < 8 ; i ++ )
		m_pBiped[i] = NULL;

	m_pCapSyong = NULL;
	m_pBound = NULL;

	for( int i = 0 ; i < 10 ; i ++ )
	{
		hitpoints[i][0] = 0;
		hitpoints[i][1] = 0;
	}
}

CNkMob::~CNkMob()
{
	if (m_pClrAlloc)	// 할당받은 컬라값을 해제한다.
		m_pClrAlloc->ColorFree(m_wColorIdx);

	if (m_bHeroAttCome)
		pCMyApp->SetHeroAttGoMob(NULL);

	// 관련된 효과를 없앤다.
	list<int>::iterator it;

	it = m_listEffect.begin();
	int idx;
	while (it != m_listEffect.end())
	{
		idx = *it;
		pCMyApp->m_pFxSet->DeleteEffect(idx);
		it = m_listEffect.erase(it);
	}
	SAFE_DELETE(m_pDropStar);
	SAFE_DELETE(m_NkMob_Particle);
//=======================================
	// nate 2004 - 4 : Crash_Miss
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
		SAFE_DELETE(m_pCrash_MissPopup[ i ]);

	SAFE_DELETE(m_NPC_Event);
	delete m_pMobChatPopup;
//=======================================
}

void CNkMob::SetAttackChaId(int chaId)
{
	m_AttackChaId = chaId;
	m_AttackMobId = -1;
	m_AttackPetId = -1;
}

void CNkMob::SetAttackMobId(int mobId)
{
	m_AttackMobId = mobId;
	m_AttackChaId = -1;
	m_AttackPetId = -1;
}

void CNkMob::SetAttackPetId(int petId)
{
	m_AttackPetId = petId;
	m_AttackMobId = -1;
	m_AttackChaId = -1;
}

void CNkMob::SetAttackVital(int vital)
{
	m_Vital = m_AttackVital;
	m_AttackVital = vital;
	m_LastAttackVitalTime = timeGetTime();
}

void CNkMob::Attacked()
{
	pCMyApp->m_pUIMgr->SetShowVital(m_nVirId, m_AttackVital);
	m_Vital = m_AttackVital;
}

// ---------------------------------------------------------------------
// Desc : 맙데이터 셋에서 맙의 이름을 가져온다. 검색없이....
// ---------------------------------------------------------------------
char *CNkMob::GetMobName()
{
	if( pCMyApp->m_pUIMgr->m_nNowPlayingEvent == EVENTACTION_TUTORIAL ) ///튜토리얼 중에 예외 처리.
	{
		// 튜토리얼중 '경비대장(57)'의 이름을 '카이르'로 바꿔준다.
		if( m_nMobNum == 57 ) // 경비 대장이면.
		{
			// IDS_TUTORIAL_TEACHER : 카이르
			return ( (char *)G_STRING(IDS_TUTORIAL_TEACHER) ); // 이름 임시
		}
	}

	// [3/6/2009 D.K ] : try catch. exception. this pointer is not exist, return null.
	__try
	{
		return (char*)LHGET_NPC_NAME(m_nMobNum);
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		FILE* Exceptionfp = fopen( "Exception.log", "a+" );
		char temp[128] = "";
		SYSTEMTIME stt;
		GetLocalTime( &stt );
		sprintf( temp, "Call CNkMob::GetMobName Func.\t" );
		fprintf( Exceptionfp, "%s%u%u%u : access violation( getMobName Failed )\n", temp, stt.wYear, stt.wMonth, stt.wDay );
		fclose( Exceptionfp );
		return "";
	}
}

// ---------------------------------------------------------------------
// Desc : 새로운 맙을 생성할때, 맙 번호와 서버에서 받은 아이디, 설정한다.
// ---------------------------------------------------------------------
void CNkMob::SetInfo(int mob_num, int id, int nState)
{
	int idx = -1;
	CMobData *pMobData = NULL;
	assert(g_pMobDataLoading);

	pMobData = g_pMobDataLoading->m_pMobData;
	for (idx = 0; idx < g_pMobDataLoading->m_nNpcNum; idx++)
	{
		if (mob_num == pMobData[idx].nVnum )
		{
			// 인덱스만 가지고 있어도 되는데..
			m_pBiped[0] = pMobData[idx].pBpd[0];
			m_pBiped[1] = pMobData[idx].pBpd[1];
			m_pBiped[2] = pMobData[idx].pBpd[2];
			m_pBiped[3] = pMobData[idx].pBpd[3];
			m_pBiped[4] = pMobData[idx].pBpd[4];
			m_pBiped[5] = pMobData[idx].pBpd[5];	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5개에서 7개로 늘린다.
			m_pBiped[6] = pMobData[idx].pBpd[6];	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5개에서 7개로 늘린다.

			if(mob_num == 879)
			{
				m_pBiped[7] = g_pMobDataLoading->LoadBiped("stonegolem_magic.BPD");
			}

			m_pCapSyong = pMobData[idx].pCapsyong;
			m_pBound = pMobData[idx].pBound;

			m_nSoundIdx[0] = pMobData[idx].idxSound[0];
			m_nSoundIdx[1] = pMobData[idx].idxSound[1];
			m_nSoundIdx[2] = pMobData[idx].idxSound[2];
			m_nSoundIdx[3] = pMobData[idx].idxSound[3];
			m_nSoundIdx[4] = pMobData[idx].idxSound[4];
			m_nSoundIdx[5] = pMobData[idx].idxSound[5];	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5개에서 7개로 늘린다.
			m_nSoundIdx[6] = pMobData[idx].idxSound[6];	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5개에서 7개로 늘린다.
			m_nSoundIdx[7] = -1;

			m_nSoundFrame[0] = pMobData[idx].nSoundFrame[0];
			m_nSoundFrame[1] = pMobData[idx].nSoundFrame[1];
			m_nSoundFrame[2] = pMobData[idx].nSoundFrame[2];
			m_nSoundFrame[3] = pMobData[idx].nSoundFrame[3];
			m_nSoundFrame[4] = pMobData[idx].nSoundFrame[4];
			m_nSoundFrame[5] = pMobData[idx].nSoundFrame[5];	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5개에서 7개로 늘린다.
			m_nSoundFrame[6] = pMobData[idx].nSoundFrame[6];	// [10/23/2007 Theodoric] npc 2개의 동작 추가를 위해 5개에서 7개로 늘린다.
			m_nSoundFrame[7] = 0;

			m_AttackFrame = pMobData[idx].attack_frame;
			m_fMoveSpeed = pMobData[idx].speed;
			m_Flag = pMobData[idx].flag;
			m_Height = pMobData[idx].height;
			m_FlyStone = pMobData[idx].flystone;
			m_MobDataIdx = idx;
			m_nMobNum = mob_num;

			int Rnd_Magic[3] = { SKILL_STAR_FLARE, SKILL_SHOCK_SPEAR, SKILL_STONE_LANCE };

			if (m_nMobNum == MOB_SAILON_MAGICIAN || m_nMobNum == MOB_SAILON_MAGICIAN2 ||
					m_nMobNum == MOB_SAILON_MAGICIAN3)
				m_MobSkill = Rnd_Magic[id % 3];

			break;
		}
	}

	m_nMobNum = mob_num;
	m_nVirId = id;
}

// ---------------------------------------------------------------------
// Desc : 맙의 초기 생성 위치
// ---------------------------------------------------------------------
void CNkMob::SetInitPos(float wx, float wz, float dir)
{
	m_wx = wx;
	m_wz = wz;
	m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);

	// 상어 이벤트를 위해서.
	if( m_nMobNum == 284 || m_nMobNum == 462 )
	{
		float	fSharkStand;
		if( g_bNewTerrain )
			fSharkStand = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx ) - 28.0f;
		else
			fSharkStand = MOB_SHARK_STAND;

		if( m_wy < fSharkStand )
			m_wy = fSharkStand;
	}

	m_dir = dir;
	m_toDir = dir;
}


// ---------------------------------------------------------------------
// SetInitPos()
// Desc : 맙의 초기 생성 위치
// ---------------------------------------------------------------------
void CNkMob::SetMotionState(unsigned short nState)
{
	if (m_nMotionState == 3) // 죽었는디.. 뭘 더 바래?
		return;

	if (nState == MOB_KNOCK &&
			m_nMotionState != MOB_STAND && m_nMotionState != MOB_MOVE)
		return;

	m_dwBaseTime = timeGetTime();
	m_dwTimeCapPrev = m_dwBaseTime;
	m_nMotionState = nState;

	switch(nState)
	{
	case 0:						// stand
		m_bMoving = FALSE;
		m_bAttack = FALSE;
		break;
	case 1:						// move
		m_bMoving = TRUE;
		m_bAttack = FALSE;
		break;
	case 2:						// attack
	case 5:// empty 1
	case 6:// empty 2
	case 7:
		m_bMoving = FALSE;
		m_bAttack = TRUE;
		m_PrevFrame = 0;
		m_CurFrame = 0;
		break;
	case 3:						// die
		m_bMoving = FALSE;
		m_bAttack = FALSE;
		m_PrevFrame = 0;
		m_CurFrame = 0;
		break;
	case 4:						// 워자이언트 멀티어택
		m_bMoving = FALSE;
		m_bAttack = TRUE;
		m_PrevFrame = 0;
		m_CurFrame = 0;
		break;
	}
}

void CNkMob::Kill()
{
	if( GetMotionState() == MOB_DEAD )
		return;

	if (m_bPet)
	{
		((CNkPet *) this)->Kill();
		return;
	}
	SetMotionState(MOB_DEAD);
	m_DeadFrame = 0;

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	// 워 자이언트 죽었을 때 효과  //Effect mob die Pablo
	if(m_nMobNum == 38||m_nMobNum == 331 || m_nMobNum == 352 || m_nMobNum == 1006 || m_nMobNum == 709 || m_nMobNum == 710
		|| m_nMobNum == 711 || m_nMobNum == 712 || m_nMobNum == 713 || m_nMobNum == 714 || m_nMobNum == 715 || m_nMobNum == 716 
		|| m_nMobNum == 773|| m_nMobNum == 774 || m_nMobNum == 775  || m_nMobNum == 5001 || m_nMobNum == 5002
		|| m_nMobNum == 5013 || m_nMobNum == 5014 || m_nMobNum == 5015 || m_nMobNum == 5016
		|| m_nMobNum == 5044 || m_nMobNum == 5045 || m_nMobNum == 5046 || m_nMobNum == 5047 || m_nMobNum == 5048
		|| m_nMobNum == 5069 || m_nMobNum == 5070 || m_nMobNum == 5071 || m_nMobNum == 5072 
		|| m_nMobNum == 6036 || m_nMobNum == 6038 || m_nMobNum == 648 || m_nMobNum == 6040 || m_nMobNum == 6041 
		|| m_nMobNum == 6042 || m_nMobNum == 6043 || m_nMobNum == 6044 || m_nMobNum == 6045 || m_nMobNum == 6046
		|| m_nMobNum == 6065 || m_nMobNum == 1031 || m_nMobNum == 1036 || m_nMobNum == 1041 || m_nMobNum == 1046
		 || m_nMobNum == 1051 || m_nMobNum == 1056 || m_nMobNum == 1061 || m_nMobNum == 1062)
	{
		// wan:2004-12	화이트혼 던젼 보스 뮤엘
		if(m_nMobNum == 352)
			ef_sort.num = 0;//뮤엘
		else
			ef_sort.num = 1;//워자

		ef_sort.nType = FX_WARGIANTDIE;
		ef_sort.vPos = D3DVECTOR(m_wx, m_wy, m_wz);
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
	}
	else if(m_nMobNum == 864)
	{
		ef_sort.nType = FX_BALLOON_KILL;		
		ef_sort.vPos = D3DVECTOR(m_wx, m_wy, m_wz);
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	}
}

// ---------------------------------------------------------------------
// Desc : 특정 좌표 공간으로 이동.
// ---------------------------------------------------------------------
BOOL CNkMob::Move(float tx, float tz, int nFrameCount)
{
	// 타겟지점 저정
	m_tx = tx;
	m_tz = tz;

	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	if( m_nMobNum != 431 )
		m_toDir = (float) atan2(m_dz, m_dx);

	if (nFrameCount == 0)
		nFrameCount = 1;

	m_dx /= nFrameCount;
	m_dz /= nFrameCount;

	if (m_Flag & NPC_FISH)
	{
		m_dx /= 2;
		m_dz /= 2;
	}

	m_bMoving = TRUE;
	SetMotionState(MOB_MOVE);

	return TRUE;
}
/*
void CNkMob::SetDropItem(int item_index, int vnum, int x, int z, float dir)
{
	m_DropItemIndex = item_index;
	m_DropItemVnum = vnum;
	m_DropItemX = x;
	m_DropItemZ = z;
	m_DropItemDir = dir;
}
*/
// ---------------------------------------------------------------------
// Desc : 몬스터 이동중 마지막에 위치를 시킴.
// ---------------------------------------------------------------------
BOOL CNkMob::Place(float tx, float tz, float dir, int nFrame)
{
	m_wx = tx;
	m_wz = tz;
	m_toDir = dir;

	// nFrame은 좀.. 생각할 문제.. 음..
	return TRUE;
}

void CNkMob::Stop(float tx, float tz, float dir)
{
	m_wx = m_tx = tx;
	m_wz = m_tz = tz;
	m_toDir = dir;

	SetMotionState(MOB_STAND);
}

// ---------------------------------------------------------------------
// Desc : 특정 좌표 공간으로 이동.
// ---------------------------------------------------------------------
BOOL CNkMob::Attack(float tx, float tz)
{
	// 공격 명령이 왔는데, 몹이 이동중이면 이동최종 목적지로 몹을 위치시킨다.
	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
		m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);

		if( g_MatrixSvr != 0 )
		{
			float fObjHeight = CNKObjQuadNode::GetHeightOnObject( m_wz, m_wx, m_wy );
			if( m_wy < fObjHeight )
				m_wy = fObjHeight;
		}

		// 상어 이벤트를 위해서.
		if( m_nMobNum == 284 || m_nMobNum == 462 )
		{
			float	fSharkStand;
			if( g_bNewTerrain )
				fSharkStand = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx ) - 28.0f;
			else
				fSharkStand = MOB_SHARK_STAND;

			if( m_wy < fSharkStand )
				m_wy = fSharkStand;
		}
	}
	//npc의 공격 모션이 두개일때 2차 공격모션
	if(pCMyApp->m_pUIMgr->m_bMultiAttack)
	{
		SetMotionState(MOB_KNOCK);
		pCMyApp->m_pUIMgr->m_bMultiAttack = FALSE;
	}
	else
		SetMotionState(MOB_ATTACK);


	if( m_nMobNum == 462 ) // 황금불량상어면...공격 모션 두개를 랜덤하게 사용.
	{
		SetMotionState(MOB_STAND); // 위에서 셋팅된 모션을 무시한다.(이걸해줘야 MOB_KNOCK 모션이 결정되도 작동한다.

		if( rand()%100 > 50 )
			SetMotionState(MOB_KNOCK);
		else
			SetMotionState(MOB_ATTACK);
	}
	
	if(  m_nMobNum != 430 && m_nMobNum != 432 )
	{
		m_tx = tx;
		m_tz = tz;
		m_dx = m_tx - m_wx;
		m_dz = m_tz - m_wz;
		m_toDir = (float) atan2(m_dz, m_dx);
	}

	if( m_nMobNum == 879 )
	{
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_BOSS_ATTACK_01;
		ef_sort.pMobFrom = this;		
		ef_sort.vSortPos.x = 6;
		ef_sort.vPos = D3DVECTOR(-230, 0, 0);
		int eff_num = pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		EffectSort* p = pCMyApp->m_pFxSet->GetEffect(eff_num);
		p->nCurFrame = 8;
		this->InsertEffectNum(eff_num);
	}

	return TRUE;
}

void CNkMob::SendMsg(int msg, int from_id, int type)
{
	switch(msg)
	{
	case MOBMSG_UNARMED_ATTACKED:
	case MOBMSG_SWORD_ATTACKED:
	case MOBMSG_MAGIC_ATTACKED:
		m_bAttacked = TRUE;
		m_nAttackedTime = g_dwBaseTime;
		break;
	}
}

BOOL CNkMob::Magic(float tx, float tz)
{
	// 공격 명령이 왔는데, 몹이 이동중이면 이동최종 목적지로 몹을 위치시킨다.
	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}
	SetMotionState(MOB_ATTACK);

	m_tx = tx;
	m_tz = tz;

	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;

	m_toDir = (float) atan2(m_dz, m_dx);

	return TRUE;
}

// ---------------------------------------------------------------------
// Desc : nInc 는 프레임 증가.
// ---------------------------------------------------------------------
void CNkMob::FrameMove(unsigned int nInc, BOOL bVisible)
{
	EffectSort ef_sort;

	if( pCMyApp && pCMyApp->m_pUIMgr )
	{
		if( pCMyApp->m_pUIMgr->IsVisibleMob(m_nMobNum) == FALSE )
			bVisible = FALSE;
	}

	int nMotionState = m_nMotionState;
	// 하늘에서는 가만히 서서 날아간당~~!
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz)
			&& m_nMotionState == MOB_MOVE)
		m_nMotionState = MOB_STAND;

	if (bVisible)
	{
		if( m_pCapSyong )
		{
			if (m_nMobNum == MOB_SAD_SHADOW || m_nMobNum == MOB_SAD_SHADOW_2)
			{
				// 슬픈 그림자는 광선검을 그려야 한다.
				D3DMATRIX matTrans, matRotate, matWorld, matView;
				D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
				Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
				D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
				pCMyApp->GetDevice()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
				m_pCapSyong->Animate(m_pBiped[m_nMotionState], m_CurFrame, TRUE, LINE_EFF_LIGHTSWORD,
									 &matWorld, &matView);
			}
			else if (m_nMobNum == MOB_SHOP_MAGIC1)
			{
				// 마법 상점 주인 공이 붕붕 거린다.
				D3DMATRIX matTrans, matRotate, matWorld, matView;
				D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
				Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
				D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
				pCMyApp->GetDevice()->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
				m_pCapSyong->Animate(m_pBiped[m_nMotionState], m_CurFrame, FALSE, LINE_EFF_BRIGHT,
									 &matWorld, &matView);
			}
			else
				m_pCapSyong->Animate(m_pBiped[m_nMotionState], m_CurFrame);
		}

		if (m_pBound && NULL != m_pBiped[m_nMotionState])
			m_pBound->Animate(m_pBiped[m_nMotionState], m_CurFrame);
		if (m_nSoundIdx[m_nMotionState] != -1)
		{
			if (m_AttackFrame <= m_CurFrame && m_nMotionState == MOB_ATTACK)
			{
				// called every attack frame
				if (m_nMobNum == MOB_DARKSOLDIER)
				{
					ZeroMemory(&ef_sort, sizeof(EffectSort));
					// m_pCapsyong
					ef_sort.nType = FX_SHOOT;
					D3DMATRIX matRot;
					D3DVECTOR vTmp;

					// shoot
					Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
					D3DMath_VectorMatrixMultiply(vTmp, m_pCapSyong->m_LineVector[0], matRot);
					ef_sort.vPos = vTmp;
					ef_sort.vPos.x += m_wx;
					ef_sort.vPos.y += m_wy;
					ef_sort.vPos.z += m_wz;
					ef_sort.dwColor = 0xFFFFF8B5;
					ef_sort.nCurFrame = m_CurFrame - m_AttackFrame;
					ef_sort.bOneFrame = TRUE;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

					// shooted
					D3DVECTOR vAdj;
					vAdj.x = m_tx - m_wx;
					//vAdj.y = m_ty - m_wy;
					vAdj.y = 0.0f;
					vAdj.z = m_tz - m_wz;
					vAdj = Normalize(vAdj);

					ef_sort.nType = FX_SHOOTATTACKED;		// has two types.
					ef_sort.nCurFrame = m_CurFrame - m_AttackFrame -1;
					ef_sort.bOneFrame = TRUE;
					ef_sort.vPos = D3DVECTOR(m_tx-vAdj.x*5, m_wy+13.0f, m_tz-vAdj.z*5);
					ef_sort.dwColor = 0xFFFFF8B5;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				}
				else if (m_nMobNum == MOB_MADSAIBOG || m_nMobNum == MOB_MADSAIBOG2||m_nMobNum == 347)  //강화 매드사이보그
				{
					ZeroMemory(&ef_sort, sizeof(EffectSort));
					// m_pCapsyong
					ef_sort.nType = FX_REDFIRE;
					D3DMATRIX matRot;
					D3DVECTOR vTmp;

					// Shoot
					Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
					D3DMath_VectorMatrixMultiply(vTmp, m_pCapSyong->m_LineVector[1], matRot);
					ef_sort.vPos = vTmp;
					ef_sort.vPos.x += m_wx;
					ef_sort.vPos.y += m_wy;
					ef_sort.vPos.z += m_wz;
					ef_sort.dwColor = 0xFFFFFFFF;
					ef_sort.nCurFrame = m_CurFrame - m_AttackFrame;
					ef_sort.bOneFrame = TRUE;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

					// Shooted
					D3DVECTOR vAdj;
					vAdj.x = m_tx - m_wx;
					vAdj.y = 0.0f;
					vAdj.z = m_tz - m_wz;
					vAdj = Normalize(vAdj);

					ef_sort.nType = FX_SHOOTATTACKED;	// has two types.
					ef_sort.bOneFrame = TRUE;
					ef_sort.nCurFrame = m_CurFrame - m_AttackFrame -1;
					ef_sort.vPos = D3DVECTOR(m_tx-vAdj.x*5, m_wy+13.0f, m_tz-vAdj.z*5);
					ef_sort.dwColor = 0xFFFFF8B5;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				}
				//wan:2004-12 샤크로드
				else if (m_nMobNum == 355 || m_nMobNum == 556)
				{
					ZeroMemory(&ef_sort, sizeof(EffectSort));
					D3DVECTOR vAdj;
					vAdj.x = m_tx - m_wx;
					vAdj.y = 0.0f;
					vAdj.z = m_tz - m_wz;
					vAdj = Normalize(vAdj);

					D3DMATRIX matRot;
					D3DVECTOR vTmp;
					Nk3DUtil_SetRotateCCWYMatrix(matRot, m_dir);
					D3DMath_VectorMatrixMultiply(vTmp, m_pCapSyong->m_LineVector[1], matRot);
					ef_sort.vPos = vTmp;
					ef_sort.vPos.x += m_wx;
					ef_sort.vPos.y += m_wy;
					ef_sort.vPos.z += m_wz;
					//--------------------------------------------------------
					// 쏘는 이펙트
					ef_sort.nType = FX_SHOOT;
					ef_sort.dwColor = 0xFFFFFFFF;
					ef_sort.nCurFrame = m_CurFrame - m_AttackFrame;
					ef_sort.bOneFrame = TRUE;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					//--------------------------------------------------------
					// 쏘는 라인
					ef_sort.nType = FX_LIGHTBALL;//FX_LWLIGHT;//FX_CHAKRAM;//FX_WAND_CHAIN;
					ef_sort.vPos.y=m_wy+20.0f;
					ef_sort.vTargetPos = D3DVECTOR(m_tx,m_wy+5.0f,m_tz);
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					//--------------------------------------------------------
					//맞는 이펙트
					ef_sort.nType = FX_SHOOTATTACKED;	// has two types.
					ef_sort.bOneFrame = TRUE;
					ef_sort.nCurFrame = m_CurFrame - m_AttackFrame -1;
					ef_sort.vPos.x=m_tx;
					ef_sort.vPos.y=m_wy+13.0f;
					ef_sort.vPos.z=m_tz;
					ef_sort.dwColor = 0xFFFFF8B5;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					//--------------------------------------------------------
				}
			}

			if (m_PrevFrame < m_nSoundFrame[m_nMotionState]
					&& m_nSoundFrame[m_nMotionState] <= m_CurFrame)
			{
				if (g_pDSound)
				{
					float x, y, z;
					x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
					y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
					z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;
					g_pDSound->Play(m_nSoundIdx[m_nMotionState], x, y, z);
				}

				if (m_nMotionState == MOB_ATTACK)
				{
					if (m_nMobNum == MOB_LIGHTENING_WORRIOR || m_nMobNum == 407)	// 뇌전사, 라이트 브레인
					{
						ZeroMemory(&ef_sort, sizeof(EffectSort));

						ef_sort.nType = FX_LWLIGHT;
						float z = tanf(m_dir);
						float x;
						while (m_dir < 0)
							m_dir += g_2_PI;

						while (m_dir >= g_2_PI)
							m_dir -= g_2_PI;

						if (z >= 0)
						{
							if (m_dir >= g_PI)
							{
								z = -z;
								x = -1;
							}
							else
								x = 1;
						}
						else
						{
							if (m_dir < g_PI)
							{
								z = -z;
								x = -1;
							}
							else
								x = 1;
						}
						D3DVECTOR vVec = D3DVECTOR(x, 0, z);
						vVec = Normalize(vVec);
						vVec *= 20;
						ef_sort.vPos = D3DVECTOR(m_wx+vVec.x, m_wy, m_wz+vVec.z);
						InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					}
				}
			}
		}
	}

	if( m_NowMoveSkill ) // 넉백등 이동 스킬 처리
	{
		float pastFrame = float(m_CurFrame - m_PrevFrame); // 진행되어야 될 프레임을 구한다.
		float* fFrame = NULL;
		if( m_NowMoveSkill == SKILL_KNOCKBACK2 )
			fFrame = &m_fDash_Frame;
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
		m_wx += dx;
		m_wz += dz;

		if( m_NowMoveSkill == SKILL_KNOCKBACK2 ) // 넉백처리
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
				SetMotionState( MOB_STAND );
				m_NowMoveSkill = 0;
			}
		}
	}

	// 하늘에서 서 있던 모션을 다시 돌려 놓는다.
	m_nMotionState = nMotionState;

	m_PrevFrame = m_CurFrame;
	m_CurFrame += nInc;
	m_EffFrame += nInc;
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
	{
		m_nFlyFrame += nInc;

		if (rand() % 20 == 0) // 가끔 위치에 변동을 주고 싶어서...
			m_nFlyFrame++;

		while (m_nFlyFrame >= CHA_FLY_FRAME)
			m_nFlyFrame -= CHA_FLY_FRAME;
	}

	// Frame 증가
	// m_CurFrame++;
	// 한동작이 다 끝난 다음에 원상태(서있는상태)로 복귀하는 코드
	// 단 죽었을때에는 동작이 끝나면 끌난상태로 계속.
	if (m_CurFrame >= m_pBiped[m_nMotionState]->m_AnimCount)
	{
		if (IsDead())
		{
			m_CurFrame = m_pBiped[m_nMotionState]->m_AnimCount-1;
			m_DeadFrame += nInc;
		}
		else
		{
			m_CurFrame = 0;

			if (m_Flag & NPC_SHOPKEEPER)
				m_nMotionState = rand() % 3;
			else if (IsAttacking() || m_nMotionState == MOB_KNOCK)
				SetMotionState(MOB_STAND);
		}
	}

	if (bVisible)
	{
		if (m_bAttacked)
		{
			DWORD dwTimeGap;
			dwTimeGap = g_dwBaseTime - m_nAttackedTime;

			if ( dwTimeGap <= 225)
			{
				// 4프레임 짜리. 오차보정도 함.. 25
				// 임시로 넣은 코드.. 수정 필요.
				EffectSort ef_sort;
				ef_sort.nType = FX_UNARMEDATTACKED;
				ef_sort.bOneFrame = TRUE;
				ef_sort.nCurFrame = dwTimeGap / 50;		// 이 효과는 객체내에서 디스플레이할 프레임 번호를 알려준다.
				ef_sort.vPos.x = m_wx;
				ef_sort.vPos.y = m_wy + 15.0f;
				ef_sort.vPos.z = m_wz;
				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}
			else
				m_bAttacked = FALSE;
		}
	}

	if ( m_AttackFrame <= m_CurFrame && m_nMotionState == MOB_ATTACK )
	{
		if( Mob_NormalAttack_EffectFrame(522, EFFECT_BOSSRAID_HYBRI3_01) )
			return;

		if( Mob_NormalAttack_EffectFrame(520, EFFECT_BOSSRAID_HYBRI1_01) )
			return;

		if( Mob_NormalAttack_EffectFrame(523, EFFECT_BOSSRAID_KAI1_01) )
			return;

		if( Mob_NormalAttack_EffectFrame(524, EFFECT_BOSSRAID_KAI2_01) )	// [2/05/2008 반재승] 카이립톤보스2 1 어택
			return;

		if( Mob_NormalAttack_EffectFrame(525, EFFECT_BOSSRAID_KAI3_01) )	// [2/04/2008 반재승] 카이립톤보스3 노멀 어택
			return ;

		if( Mob_NormalAttack_EffectFrame(517, EFFECT_BOSSRAID_AIDIA1_01) )
			return;

		if( Mob_NormalAttack_EffectFrame(519, EFFECT_BOSSRAID_AIDIA3_01) )	// [3/6/2008 반재승] 에이디아보스3 노멀 어택
			return;

		if( Mob_NormalAttack_EffectFrame(518, EFFECT_BOSSRAID_AIDIA2_01) )
			return;

		if( Mob_NormalAttack_EffectFrame(740, EFFECT_BOSSRAID_HYBRI1_01) )
			return;
	}
}

// [2/21/2008 반재승] 몹 일반 공격 이펙트 관련 함수
BOOL CNkMob::Mob_NormalAttack_EffectFrame( int mob_Num, int effect_Num )
{
	if (m_nMobNum == mob_Num)
	{
		CNkCharacter	*pCha = NULL;

		if (m_AttackChaId >= 0)
		{
			pCha = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);
			pCMyApp->m_pUIMgr->InsertMagic( NULL, this, pCha, NULL, effect_Num );
		}
		return TRUE;
	}
	return FALSE;
}

void CNkMob::addhitpoint(DWORD num, DWORD attacker)
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
bool CNkMob::ishit()
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
#define FISH_HEIGHT	(72.0f)

// ---------------------------------------------------------------------
// Desc : 렌더링
// ---------------------------------------------------------------------
void CNkMob::Render(BOOL bMouseOn, BOOL bVisible, BOOL bDontPick)
{
	if( !GET_D3DDEVICE() || !GET_D3DPICK() || !m_pCapSyong )
		return;

	if( pCMyApp && pCMyApp->m_pUIMgr )
	{
		if( pCMyApp->m_pUIMgr->IsVisibleMob(m_nMobNum) == FALSE )
			bVisible = FALSE;
	}

	static D3DMATRIX matTrans, matRotate, matWorld;
	static DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting;

	DWORD dwCurtime;
	DWORD dwTimeCap;
	unsigned int frame_count;
	BOOL bArrived = FALSE;
	static EffectSort ef_sort;

	BOOL bMutantMaterial = FALSE; // 뮤턴트 치로 앰비언트 변경시 셋팅. 앰비언트 값을 돌려준다. ///태국뮤턴트이벤트
	int nMutantAlphaOpt = 0; // 뮤턴트 치로 반투명등의 몬스터 알파옵션 설정.

	dwCurtime = timeGetTime();

	// Vital 이 변해야 하는데, 1초가 지나도 안변하면...
	if (m_Vital != m_AttackVital && dwCurtime > m_LastAttackVitalTime + 1000)
	{
		Attacked();
		dwCurtime = timeGetTime();
	}

	// 나 너 때렸다.
	CNkMob			*pMob = NULL;
	CNkCharacter	*pCha = NULL;
	if (m_AttackMobId >= 0)
	{
		if (m_nMotionState != MOB_ATTACK ||	m_AttackFrame <= m_CurFrame)
		{
			pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
			if (pMob)
			{
				pMob->Attacked();
				switch( m_nMobNum )
				{
				case 218:	// 지뢰
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, this, EFFECT_SKILL_MOB_FIRE);
					break;
				case MOB_SKELETON_MAGE: // 해골 마법사,
				case 393:	// 본 소서러
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_FIRE_BALL);
					break;
				case MOB_DARK_SOUL: // 125 다크소울
				case MOB_SPECTER:	// 417 스펙터
				//case 334: // 보스 다크소울
				case 542: // 강화 다크 소울
				case 596: // 강화 스펙터
				case 731:	// 유피텔스펙터
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING);
					break;
				case MOB_SAILON_MAGICIAN:  // 137 샤일론 매지션
				case MOB_SAILON_MAGICIAN2: // 146 샤일론 매지션
				case MOB_SAILON_MAGICIAN3: // 164 샤일론 매지션
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, m_MobSkill);
					break;
				case 179:
				case 180:
				case 181:	// 파이어 (클레이,스톤,스틸) 수호탑
				case 867:
				case 868:
					// 파이어 스틸 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_GUARDTOWER_FIRE);
					break;
				case 182:
				case 183:
				case 184: // 라이트닝 (슬레이,스톤,스틸) 수호탐
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_GUARDTOWER_LIGHTBALL);
					break;
				case 185:
				case 186:
				case 187: // 스톤 (클레이, 스톤,스틸) 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_GUARDTOWER_PEBBLESHOT);
					break;
				case MOB_KHAN_HAND: // 171
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_WORLDCUP_HAND);
					break;
				case MOB_KHAN_FOOT: // 172
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_WORLDCUP_FOOT);
					break;
				case MOB_SHILONMASTER: // 135 샤일론 마스터
				case 306:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
					break;
				case MOB_BABY_BUG: // 196 스핏 웜
				case 536: // 강화 스핏 웜
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_BABYWORM);
					break;
				case MOB_SHINING_JAKAL:	//1층보스 샤이닝자캴 -
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_MOB_SHINING_JAKAL);
					break;
				case 205: // 볼케닉 리스크 돌속성2,돌속성1 - 팻블샷
				case 202: // 다트 피쳐
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_PEBBLE_SHOT);
					break;
				case 208: // 데빌킹
				case 250: // 세도우 철갑 드래곤 - 거대운석
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_METEORITE);
					break;
				case 210: // 드레드 템플러
				case 249: // 플라잉 데빌킹 -냉기
				case 562: // 강화 드레드 템플러
				case 5006:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_ICE);
					break;
				case 1010:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_CHAOS);
					break;
				case 5007:
				case 5014:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_TORNADO_RUIN);
					break;
				case 222: // 어스 드래곤
				case 223: // 스카이 드래곤
				case 265: // 분노한 어스 드래곤 Dragon 용의 불뿜기
				case 737: // 유피텔
				case 738:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_DRAGON_FIRE);
					break;
				case 224: // 마더 드래곤
				case 333: // 보스 마더 드래곤 Dragon 용의 파란불뿜기 ,강화
				case 739:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_DRAGON_ICE);
					break;
				case 219: // 레이븐 메이지
				case 247: // 고스트레이븐
				case 558: // 강화 레이븐 메이지
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_CHAIN);
					break;
				case 221: // 철갑 드래곤
				case 264: // 분노한 철갑 드래곤
				case 345: // 강화 철갑 드래곤
				case 405: // 데빌후드
				case 560: // 강화 철갑 드래곤
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_FALL_STONE);
					break;
				case 133: // 칸
				case 348: // 보스 칸의 지역공격
				case 1009:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_KHAN_HIT);
					break;
				case 1013:
				case 1014:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_HITPOWER);
					break;
				case 1012:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_RING_1000);
					break;
				case 1022:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_ICE_STORM);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_WAVE_OF_DEATH);
					break;
				case 1023:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_AIR_RAID);
					break;
				case 5048:
				case 5046:
				case 5047:
				case 6036:
				case 6038:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_KHAN_HIT);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_FIRE);
					break;
				case 1026:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_FIRE);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_STONERANCE2);
				case 6065:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_KHAN_HIT);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_FIRE);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_METEORITE);
					break;
				case 258: // 피쳐
				case 260: // 리스크
				case 570: // 강화 리스크
				case 589: // 강화 리스크
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_SOULDUST);
					break;
				case 263: // 템플러의 라이트닝 공격
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING);
					break;
				case 288: // 소서리 윙
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_ARROW);
					break;
				case 290: // 헬 윙
				case 600: // 강화 헬 윙
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_MAGICSWORD);
					break;
				case 297: // 헤일 위저드
				case 350: // 강화 헤일 위저드
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_FALLOFFSTONE);
					break;
				case 299: // 클레이 소울
				case 404: // 클레이 스펙터
				case 578: // 강화 클레이 스펙터
				case 1019:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_STONERANCE2);
					break;
				case 300: // 프로즌헤드
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_DRILLSTONE);
					break;
				case 354: // 시크린 (스톤랑스 이펙트)
				case 555: // 강화 시크린
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_STONE_LANCE);
					break;
				case 356: // 베라딘 (데빌킹이펙트)
				case 557: // 강화 베라딘
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_RING_DEVIL);
					break;
				case 358: // 가르폰 첨병 마법단검 이펙트
				case 551: // 강화 가르폰 첨병 
				case 5008:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_MAGICSWORD);
					break;
				case 5001:
				case 5002:
				case 716:
				case 1015:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_HELL_CRY);
					break;
				case 1017:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, SKILL_H_AUTOMINE);
					break;
				case 5013:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_RING_UNICORN);
					break;
				case 772:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_AUTOLASER);
					break;
				case 773:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_BLACKHOLE);
					break;
				case 774:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_FLAME);
					break;
				case 360: // 아이스 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_BABYWORM);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_ICE);
					break;
				case 361: // 뇌격 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING_BALL);
					break;
				case 1016:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_SHOCK_SPEAR);
					break;
				case 421: // nate 2005-11-22 : 소환 캐크터스
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_CACTUS);
					break;
				case 423: // nate 2005-11-22 : 소환 댄더리온
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_DANDELION);
					break;
				case 424: // nate 2005-11-22 : 소환 스포어
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SPORE);
					break;
				case 775:
				case 5045:
				case 464: // 데카렌 보스 레그넉 타격 이펙트.(검 궤적은 따로 넣음) ///인던1
					pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, NULL, pMob, HIT_LEGKNOCK_ATTACK);
					break;
				default:

					if (m_nMobNum == 38||m_nMobNum == 331 || m_nMobNum == 545 || m_nMobNum == 1011)	// 워 자이언트 근접 공격, 강화
						pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, NULL, pMob, HIT_WARGIANT_ATTACK);

					switch (m_MobAttackType)
					{
					case MOB_ATTACK_STARFLARE:
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_STAR_FLARE);
						break;
					case MOB_ATTACK_LIGHTNING:
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING);
						break;
					case MOB_ATTACK_ACCUMULATE://누적(전기가 지지직~)
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob,EFFECT_SKILL_MOB_ACCUMULATE);
						break;
					case MOB_ATTACK_FIRE://불속성(불이 펑~)
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_FIRE);
						break;
					default:
						pMob->Attacked();
						break;
					}
				}
			}
			m_AttackMobId = -1;
		}
	}
	else if (m_AttackChaId >= 0 || m_AttackPetId >= 0)
	{
		if ( m_AttackFrame <= m_CurFrame
				|| (m_nMotionState != MOB_KNOCK
					&& m_nMotionState != MOB_ATTACK ))
		{
			pCha = NULL;
			pMob = NULL;

			if (m_AttackChaId >= 0)
				pCha = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);
			else
			{
				if (pCMyApp->m_pMyPet && pCMyApp->m_pMyPet->m_nVirId == m_AttackPetId)
					pMob = (CNkMob *) pCMyApp->m_pMyPet;
				else
					pMob = (CNkMob *) (pCMyApp->m_pUIMgr->FindPetById(m_AttackPetId));
			}
			if (pCha || pMob)
			{
				switch( m_nMobNum )
				{
				case MOB_SKELETON_MAGE: // 61
				case 393:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_FIRE_BALL);
					break;
				case MOB_DARK_SOUL: // 125
				case MOB_SPECTER: // 417
				case 542:
				//case 334: //강화 다크소울
				case 596:
				case 731:	// 유피텔스펙터
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_LIGHTNING);
					break;
				case MOB_SAILON_MAGICIAN: // 137
				case MOB_SAILON_MAGICIAN2: // 146
				case MOB_SAILON_MAGICIAN3: // 164
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, m_MobSkill);
					break;
				case 179:
				case 180:
				case 181: // 파이어 수호탑
				case 867:
				case 868:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_GUARDTOWER_FIRE);
					break;
				case 182:
				case 183:
				case 184: // 라이팅볼 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_GUARDTOWER_LIGHTBALL);
					break;
				case 185:
				case 186:
				case 187: // 스톤 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_GUARDTOWER_PEBBLESHOT);
					break;
				case MOB_KHAN_HAND: // 171
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_WORLDCUP_HAND);
					break;
				case MOB_KHAN_FOOT: // 172
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_WORLDCUP_FOOT);
					break;
				case MOB_SHILONMASTER: // 135
				case 306:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
					break;
				case MOB_BABY_BUG: // 196
				case 536: // 새끼벌레-
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_BABYWORM);
					break;
				case MOB_SHINING_JAKAL:	// 194 1층보스 샤이닝자캴 -
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_MOB_SHINING_JAKAL);
					break;
				case 205:
				case 202: // 돌속성2,돌속성1 - 팻블샷
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_PEBBLE_SHOT);
					break;
				case 208:
				case 250: // 데빌킹,세도우 철갑 - 거대운석
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_METEORITE);
					break;
				case 210:
				case 249:
				case 562: //템플러,플라잉 데빌킹 -냉기
				case 5006:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_ICE);
					break;
				case 1010:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_CHAOS);
					break;
				case 5007:
				case 5014:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_TORNADO_RUIN);
					break;
				case 222:
				case 223:
				case 265: // Dragon 용의 불뿜기
				case 737: // 유피텔
				case 738:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_DRAGON_FIRE);
					break;
				case 224:
				case 333: // Dragon 용의 파란불뿜기 ,강화
				case 739:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_DRAGON_ICE);
					break;
				case 219:
				case 247:
				case 558: // 레이븐,고스트레이븐
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_CHAIN);
					break;
				case 221:
				case 264:
				case 345:
				case 405:
				case 560: // 철갑 드래곤,,,데빌후드
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_FALL_STONE);
					break;
				case 133:
				case 348: // 칸의 지역공격 , 강화칸
				case 1009:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_KHAN_HIT);
					break;
				case 1013:
				case 1014:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_HITPOWER);
					break;
				case 1012:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_RING_1000);
					break;
				case 1022:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_ICE_STORM);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_WAVE_OF_DEATH);
					break;
				case 1023:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_AIR_RAID);
					break;
				case 5048:
				case 5046:
				case 5047:
				case 6036:
				case 6038:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_KHAN_HIT);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_FIRE);
					break;
				case 1026:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_FIRE);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_STONERANCE2);
				case 6065:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_KHAN_HIT);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_FIRE);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_METEORITE);
				case 258:
				case 260:
				case 570:
				case 589: // 리스크, 피처 공격 효과
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_SOULDUST);
					break;
				case 263: // 템플러의 라이트닝 공격
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_LIGHTNING);
					break;
				case 288:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_ARROW);
					break;
				case 290:
				case 600:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_MAGICSWORD);
					break;
				case 297:
				case 350: // 강화 헤일위자드
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_FALLOFFSTONE);
					break;
				case 299:
				case 404:
				case 578: // 클레이 소울, 글레이 스펙터
				case 1019:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_STONERANCE2);
					break;
				case 300:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_DRILLSTONE);
					break;
				case 354:
				case 555: // 시크린 스톤랑스이펙트
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_STONE_LANCE);
					break;
				case 356:
				case 557: // 베라딘 데빌킹이펙트
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_RING_DEVIL);
					break;
				case 358:
				case 551: // 가르폰 첨병 마법단검 이펙트
				case 5008:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_MAGICSWORD);
					break;
				case 5001:
				case 5002:
				case 716:
				case 1015:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_HELL_CRY);
					break;
				case 1017:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, SKILL_H_AUTOMINE);
					break;
				case 5013:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_RING_UNICORN);
					break;
				case 772:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_AUTOLASER);
					break;
				case 773:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_BLACKHOLE);
					break;
				case 774:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_FLAME);
					break;
				case 360: // 아이스 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_BABYWORM);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_ICE);
					break;
				case 361: // 뇌격 수호탑
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_LIGHTNING_BALL);
					break;
				case 1016:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_SHOCK_SPEAR);
					break;
				case 416: // nate 2005-09-29 : 드미트론 중간보스 - 듀란트
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_DURANT_ATTACK);
					break;
				case 421: // nate 2005-11-22 : 소환 캐크터스
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_CACTUS);
					break;
				case 423: // nate 2005-11-22 : 소환 댄더리온
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_DANDELION);
					break;
				case 424: // nate 2005-11-22 : 소환 스포어
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_SPORE);
					break;
				case 438: // nate 2006-01-26 : 서펀트 퀸
				case 711:
				case 712:
				case 714:
				case 648:
				case 4013:
					{
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_SKILL_MOB_SNAKEBOSS);
						if( pCha == g_pRoh )
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_SKILL_MOB_SNAKEBOSS2);
						break;
					}
				case 464: // 데카렌 보스 레그넉 타격 이펙트.(검 궤적은 따로 넣음) ///인던1
				case 775:
				case 5045:
					pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, pCha, pMob, HIT_LEGKNOCK_ATTACK);
					break;
				default:
					{
						if (m_nMobNum == 38||m_nMobNum == 331 || m_nMobNum == 545 || m_nMobNum == 1011)	// 워 자이언트 근접 공격
							pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, pCha, pMob, HIT_WARGIANT_ATTACK);

						switch (m_MobAttackType)
						{
						case MOB_ATTACK_STARFLARE:
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_STAR_FLARE);
							break;
						case MOB_ATTACK_LIGHTNING:
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_LIGHTNING);
							break;
						///2003 누적과 불속성 공격 효과
						case MOB_ATTACK_ACCUMULATE://누적(전기가 지지직~)
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob,EFFECT_SKILL_MOB_ACCUMULATE);
							break;
						case MOB_ATTACK_FIRE://불속성(불이 펑~)
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_FIRE);
							break;
						default:
							if (pCha)
								pCha->Attacked();
							else
								pMob->Attacked();
							break;
						}
					}
					break;
				}
			}
			m_AttackChaId = -1;
			m_AttackPetId = -1;
		}
	}
///////////////////////hitpoints//////////////////////////////
	if(this->ishit())
	{
		pCMyApp->DisplayMobHitpoints(this);
	}
//////////////////////////////////////////////////////////////
	// nate 2006-01-24 : 타겟이 없는 몹의 이펙트 처리
	if( m_byInsertEffect )
	{
		if( m_AttackFrame <= m_CurFrame && m_nMotionState == MOB_ATTACK )	// 공격중이고 어택 프레임에 도달하면
		{
			if( m_nMobNum == 432 && (m_byInsertEffect & INSERT_EFFECT_FIRE) ) // nate 2006-01-20 : 불장벽
			{
				pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_FLAME);//화염방사기
				m_byInsertEffect &= ~INSERT_EFFECT_FIRE;	// 이펙트는 한번만( ~ : 플레그 값의 보수 )
			}
		}
	}

	if( m_nMobNum == 433 ) // 구르는 돌 트랩 방향 예외처리 ///인던1 시 추가
	{
		m_dir = m_toDir; // 방향 도는 과정을 보이지 말고 바로 돌려버린다.
	}
	else if (m_toDir > -100 && m_toDir != m_dir)
	{
		if (m_dir - m_toDir > g_PI)
			m_dir -= g_2_PI;
		else if (m_toDir - m_dir > g_PI)
			m_dir += g_2_PI;

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

	frame_count = 0;
	if ( ((dwCurtime - m_dwBaseTime) * 0.001f) >= m_AniRate)
	{
		m_dwBaseTime += (DWORD) (m_AniRate*1000);
		frame_count = 1;
		while ( ((dwCurtime - m_dwBaseTime)*0.001f) >= m_AniRate )
		{
			frame_count++;
			m_dwBaseTime += (DWORD) (m_AniRate*1000);
		}
	}

	dwTimeCap = dwCurtime - m_dwTimeCapPrev;
	m_dwTimeCapPrev = dwCurtime;

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
				bArrived = TRUE;
		} // 2 / 4
		else if ( m_dx < 0 && m_dz >= 0)
		{
			if (m_wx <= m_tx && m_wz >= m_tz)
				bArrived = TRUE;
		} // 3/4
		else if ( m_dx <= 0 && m_dz <= 0)
		{
			if (m_wx <= m_tx && m_wz <= m_tz)
				bArrived = TRUE;
		} // 4/4
		else if ( m_dx >= 0 && m_dz <= 0)
		{
			if (m_wx >= m_tx && m_wz <= m_tz)
				bArrived = TRUE;
		}

		if (bArrived)
		{
			m_wx = m_tx;
			m_wz = m_tz;
			m_bMoving = FALSE;
		}

		if (m_bMoving == FALSE)		// 스테이트를 바꾸어 주어야 한다.
			SetMotionState(MOB_STAND);
		//MOB EFFECT SMOKE WHEN RUN PABLO
		if( m_nMobNum == 38 || m_nMobNum == 331 || m_nMobNum == 1006 || m_nMobNum == 709 || m_nMobNum == 710
		|| m_nMobNum == 711 || m_nMobNum == 712 || m_nMobNum == 713 || m_nMobNum == 714 || m_nMobNum == 715 || m_nMobNum == 716 
		|| m_nMobNum == 773|| m_nMobNum == 774 || m_nMobNum == 775  || m_nMobNum == 5001 || m_nMobNum == 5002
		|| m_nMobNum == 5013 || m_nMobNum == 5014 || m_nMobNum == 5015 || m_nMobNum == 5016
		|| m_nMobNum == 5044 || m_nMobNum == 5045 || m_nMobNum == 5046 || m_nMobNum == 5047 || m_nMobNum == 5048
		|| m_nMobNum == 5069 || m_nMobNum == 5070 || m_nMobNum == 5071 || m_nMobNum == 5072 
		|| m_nMobNum == 6036 || m_nMobNum == 6038 || m_nMobNum == 648 || m_nMobNum == 6040 || m_nMobNum == 6041 
		|| m_nMobNum == 6042 || m_nMobNum == 6043 || m_nMobNum == 6044 || m_nMobNum == 6045 || m_nMobNum == 6046
		|| m_nMobNum == 6065 || m_nMobNum == 1031 || m_nMobNum == 1036 || m_nMobNum == 1041 || m_nMobNum == 1046
		 || m_nMobNum == 1051 || m_nMobNum == 1056 || m_nMobNum == 1061 || m_nMobNum == 1062)
		{
			// 워 자이언트 먼지 효과
			m_RunSmokeFrame += (int)frame_count;

			if( m_RunSmokeFrame > 6 )
			{
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_RUNSMOKE;
				ef_sort.pMobFrom = this;
				ef_sort.vPos = D3DVECTOR(m_wx, pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz), m_wz);
				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

				m_RunSmokeFrame = 0;
			}
		}
	}

	///인던1
	if( m_RegeningType == 2 ) // 땅속에서 튀어나오고 있는 중이면...
	{
		float temp = pCMyApp->GetLandPtr()->GetHFHeight(m_wx,m_wz);
		if( m_wy < pCMyApp->GetLandPtr()->GetHFHeight(m_wx,m_wz) ) // 땅속에 박혀있다면...
		{
			m_fDash_Frame += 0.4f;
			m_wy += (m_CurFrame-m_PrevFrame)*m_fDash_Frame; // 프레임 지나간것에 기반해서...일정속도로 올려준다.
			m_CurFrame = 0; // 움직이지 않도록 고정시킨다.

			if( m_fDash_Frame < 1.0f ) // 초반부에만 연기가 일도록...
			{
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_RUNSMOKE;
				ef_sort.vPos = D3DVECTOR(m_wx, pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz)-5.0f, m_wz); // 약간 낮춰서 생성.
				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}

		}
		else
		{
			m_RegeningType = 0; // 연출을 끝내고 일반 루틴으로 들어간다.
			m_fDash_Frame = 0.0f; // 넉백시의 변수를 임시로 썼으므로 초기화 해준다.
		}
	}
	else
		m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);

	//=================================================
	// nate 2004 - 4 : CastleWar
	if( pCMyApp->m_pUIMgr->m_bOnBattle )
	{
		if( m_nMobNum == 327 || m_nMobNum == 328 || m_nMobNum == 329 )
			m_Mutant = 2;
	}
	else
	{
		if( m_nMobNum == 327 || m_nMobNum == 328 || m_nMobNum == 329 )
			m_Mutant = 0;
	}
	//=================================================

	if(m_MobClassType == 9 ) // 디바인 스톤이고. ///하이콤보
	{
		if( m_RunSmokeFrame ) // 이 녀석이 셋팅된 디바인스톤은 땅에 박혀 들어가게 셋팅.
			m_wy -= m_RunSmokeFrame;
	}

	// 상어 이벤트를 위해서.
	if( m_nMobNum == 284 || m_nMobNum == 462 )
	{
		float fSharkStand = 0.0f;
		if( g_bNewTerrain )
			fSharkStand = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx ) - 28.0f;
		else
			fSharkStand = MOB_SHARK_STAND;

		if( m_wy < fSharkStand )
			m_wy = fSharkStand;
	}

	if( m_nMobNum == MOB_DARK_SOUL||m_nMobNum == 334 )		// 다크소울은 물위를 날아다닌다. 우휘~~//강화
	{
		float	fWaterHeight;
		if( g_bNewTerrain )
			fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx );
		else
			fWaterHeight = WATER_HEIGHT;

		if( m_wy < fWaterHeight )
			m_wy = fWaterHeight;
	}
	else if( m_Flag & NPC_FISH )
	{
		if( g_bNewTerrain )
			m_wy = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx );
		else
			m_wy = WATER_HEIGHT;
	}
	else if( m_nMobNum==MOB_AIREN )			//0401 에이런 소환은 둥둥~
	{
		float	fWaterHeight;
		if( g_bNewTerrain )
			fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx ) + 18.0f;
		else
			fWaterHeight = WATER_HEIGHT + 18;

		m_wy += 18;
		if( m_wy < fWaterHeight )
			m_wy = fWaterHeight;
	}

	// SKY 존에서는 하늘 위에서 스케이트보드를 탄닷!
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
	{
		BOOL bSky = !(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz));
		if (bSky)
			m_wy += g_ChaFlyHeight[m_nFlyFrame];
	}

	if (IsDead() && pCMyApp->m_nCurWorldIdx == WORLD_SKY &&
			!(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz)))
		m_wy -= m_DeadFrame * (700.0f / 100); // 100 Frame(2초) 동안 1000.0f을 가라 앉는다.
	else if (m_DeadFrame > 0 && m_nMobNum != 438 && m_nMobNum != 465 && m_nMobNum != 466 )	// 438 : 서펀트 퀸은 가라않지 말자 // 데카렌 인던의 문들도 안가라않게.(열리는걸 죽는 걸로 사용) ///인던1
	{
		if(m_nMobNum == 38||m_nMobNum ==331 || m_nMobNum==333 || m_nMobNum==334 || m_nMobNum==348||m_nMobNum == 352 || m_nMobNum == 545 
			|| m_nMobNum == 1006 || m_nMobNum == 709 || m_nMobNum == 710
		|| m_nMobNum == 711 || m_nMobNum == 712 || m_nMobNum == 713 || m_nMobNum == 714 || m_nMobNum == 715 || m_nMobNum == 716 
		|| m_nMobNum == 773|| m_nMobNum == 774 || m_nMobNum == 775  || m_nMobNum == 5001 || m_nMobNum == 5002
		|| m_nMobNum == 5013 || m_nMobNum == 5014 || m_nMobNum == 5015 || m_nMobNum == 5016
		|| m_nMobNum == 5044 || m_nMobNum == 5045 || m_nMobNum == 5046 || m_nMobNum == 5047 || m_nMobNum == 5048
		|| m_nMobNum == 5069 || m_nMobNum == 5070 || m_nMobNum == 5071 || m_nMobNum == 5072 
		|| m_nMobNum == 6036 || m_nMobNum == 6038 || m_nMobNum == 648 || m_nMobNum == 6040 || m_nMobNum == 6041 
		|| m_nMobNum == 6042 || m_nMobNum == 6043 || m_nMobNum == 6044 || m_nMobNum == 6045 || m_nMobNum == 6046 || m_nMobNum == 4013 || m_nMobNum == 4011
		|| m_nMobNum == 879 || m_nMobNum == 6065 || m_nMobNum == 6065 || m_nMobNum == 1031 || m_nMobNum == 1036 || m_nMobNum == 1041 || m_nMobNum == 1046
		 || m_nMobNum == 1051 || m_nMobNum == 1056 || m_nMobNum == 1061 || m_nMobNum == 1062)
		{
			// 워자이언트, 강화 보스몹전체, 뮤엘
			m_wy += m_DeadFrame * (30.0f / 100);	// 떠오름
		}
		else
		{
			if (m_DeadFrame > 60)
			{
				/*				if (m_DropItemIndex >= 0)	// nate 2006-01-24 : 현재 미사용
								{
									// 이거 써먹으려면... 뿌라쓰 아이템 번호도 잘 넘겨라! 지금은 0 이다.
									pCMyApp->m_pUIMgr->AddItem(m_DropItemIndex, m_DropItemVnum, m_DropItemX, m_DropItemZ, m_DropItemDir, 0, 0, 0); // 05-10-10 옵션플래그 2 추가 (원석) ///신방어구강화시스템
									m_DropItemIndex = -1;
								}
				*/				m_wy -= (m_DeadFrame-60) * (6.0f / 40); // 60 Frame(3초) 후부터 40 Frame(2초) 동안 6.0f을 가라 앉는다.
			}
		}
	}

	// 다음 프레임 계산
	FrameMove(frame_count, bVisible);

	if (bVisible == FALSE)
		return;

	if (m_LastChatTime + SHOW_MOBCHATPOPUP_TIME > dwCurtime && bVisible)
		m_bShowMobChatPopup = TRUE;
	else
		m_bShowMobChatPopup = FALSE;

	// 월드 트랜스폼. - 캐릭터를 그리기 위한.
	D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
	/*
		if (m_Mutant == 5 || m_bQuestMob != FALSE)
		{
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 1.5f, 1.5f, 1.5f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		// nate 2005-08-22 : 몬스터 스케일 조정
		if( pCMyApp->m_nCurWorldIdx == WORLD_DMITRON )
		{
			if( m_nMobNum == 416
				|| m_nMobNum == MOB_SPECTER
				|| ( m_nMobNum > 391 && m_nMobNum < 413 ) )
			{
				float fScale = 1.5f;
				if( m_nMobNum == 401 || m_nMobNum == 402 )	// 스파이더, 레인스파이더
					fScale = 2.0f;
				else if( m_nMobNum == 405 || m_nMobNum == 406 )	// 철갑드래곤, 플라잉 데빌킹
					fScale = 1.0f;
				else if( m_nMobNum == 416 || m_nMobNum == MOB_SPECTER )	// nate 2005-10-25 : 스펙터 추가
					fScale = 1.7f;

				D3DMATRIX matS;
				D3DUtil_SetScaleMatrix(matS, fScale, fScale, fScale);
				D3DMath_MatrixMultiply(matWorld, matS, matWorld);
			}
		}
	*/
	if( m_Mutant > 10000 ) // 특수 뮤턴트 속성 추가.(확대,색상 입히기)   ///태국뮤턴트이벤트
	{
		D3DMATRIX matS;

//		int PlusScale = (m_Mutant%1000) / 100; // 백의 자리를 확대 스케일로 사용.
//		int MinusScale = (m_Mutant%10000) / 1000; // 천의 자리를 축소 스케일로 사용.
		int Color = m_Mutant % 100; // 끝의 두자리를 컬러 플래그로 사용.
		int TransFlag = (m_Mutant%100000) / 10000; // 만의 자리가 2이상이면 알파옵션.

		if( TransFlag >= 2 ) // 알파옵션이 붙은 몹이면..
		{
			nMutantAlphaOpt = TransFlag;
		}
		/*
				if( PlusScale ) // 확대 치가 있으면...
				{
					D3DUtil_SetScaleMatrix( matS, (float)PlusScale, (float)PlusScale, (float)PlusScale ); // 스케일이 누적될것 같음에 유의
					D3DMath_MatrixMultiply(matWorld, matS, matWorld);
				}

				if( MinusScale ) // 확대 치가 있으면...
				{
					D3DUtil_SetScaleMatrix( matS, 1.0f/(float)MinusScale, 1.0f/(float)MinusScale, 1.0f/(float)MinusScale ); // 스케일이 누적될것 같음에 유의
					D3DMath_MatrixMultiply( matWorld, matS, matWorld );
				}
		*/
		if( Color ) // 색깔 변경 플래그가 있으면...
		{
			D3DMATERIAL7 mtrl;

			switch( Color )
			{
			case 1: // 빨강
				D3DUtil_InitMaterial( mtrl, 1.0f, 0.2f, 0.2f );
				break;
			case 2: // 파랑
				D3DUtil_InitMaterial( mtrl, 0.2f, 0.2f, 1.0f );
				break;
			case 3: // 보라
//			    D3DUtil_InitMaterial( mtrl, 1.0f, 0.5f, 0.5f );
				D3DUtil_InitMaterial( mtrl, 1.0f, 0.2f, 1.0f );
				break;
			case 4: // 노랑
				D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 0.2f );
				break;
			case 5: // 초록
				D3DUtil_InitMaterial( mtrl, 0.0f, 1.0f, 0.0f );
				break;
			case 6: // 남색(음침하게..)
				D3DUtil_InitMaterial( mtrl, 0.13f, 0.1f, 0.5f );
//			    D3DUtil_InitMaterial( mtrl, 0.1f, 0.1f, 0.15f );
				break;
			default:
				D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
				break;
			}
			bMutantMaterial = TRUE; // 매터리얼을 돌려주기 위해 기억.
			GET_D3DDEVICE()->SetMaterial( &mtrl );
		}

	}
	/*
		//---------------------------------------------------------------------------
		//-- 식목일 나무 Event
		//-- 2004.03.29
		//-- Lyul / 수정완료
		//-- 다 때리모 사라진다.
		//-- 초기 스케일은 0.5f 에서 죽기 직전은 1.5f
		//-- 20의 vital 을 가진다. 0.05 씩 multiply 하면 OK
		if (m_nMobNum == 323) //-- 식목일 나무
		{
			float grade = m_Vital * 0.01f;
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 1.5f - grade, 1.5f - grade, 1.5f - grade);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		//---------------------------------------------------------------------------
		//중고렙 스킬 추가
		//서먼피션 분열몹이면
		if(m_FissionType>0)
		{
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, g_SummonScale+1.0f, g_SummonScale+1.0f, g_SummonScale+1.0f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		//---------------------------------------------------------------------------
		//강화몹 크기 조절
		//---------------------------------------------------------------------------
		if(m_nMobNum==334){
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 3.5f, 3.5f, 3.5f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		else if(m_nMobNum==331||m_nMobNum==348){
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 1.7f, 1.7f, 1.7f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		else if(m_nMobNum==333||m_nMobNum==337||m_nMobNum==347){
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 1.5f, 1.5f, 1.5f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		else if(m_nMobNum==338||m_nMobNum==339||m_nMobNum==340||m_nMobNum==341||
			m_nMobNum==342||m_nMobNum==343||m_nMobNum==344||m_nMobNum==345||m_nMobNum==346||m_nMobNum==349||m_nMobNum==350){
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 1.2f, 1.2f, 1.2f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		else if(m_nMobNum==413 || m_nMobNum==414)
		{
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 2.0f, 2.0f, 2.0f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
	*/
	float ExceptionScale = GetMobExceptioncale( this ); // 예외적인 클라이언트 스케일이 적용되는 경우 스케일 값을 리턴해주는 함수.
	if( ExceptionScale != 1.0f ) // 1.0f이면 굳이 해줄 필요없으니 스킵.
	{
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix( matS, ExceptionScale, ExceptionScale, ExceptionScale );
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}

//--------------------------------------------------------------------------
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	// 주변광 저장
	DWORD dwAmbient;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	// 만약 자신이 자유대련 중이라면 주변 몹들은 반투명하게 ///자유대련
	if( g_pRoh && g_pRoh->m_bNowFreePVP )
	{
		// 그냥 반투명하게 몸뚱이를 출력하고 뒤에 루틴을 모두 무시시킨다.
		// 즉, 별도의 색상 처리 등의 예외처리, 이펙트 추가등이 이 이후에 있던 종류는
		// 자유대련 중에는 안보이게 된다.
		// 참고로 피킥버퍼도 안쓰기 때문에 자동으로 몹을 공격 못하게 변한다.
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		m_pCapSyong->Render(GET_D3DDEVICE());
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	}
	if( m_bPoisoning )	// nate 2005-11-14 : 포이즌 - npc에 독성분 입히기 SetAmbient
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00008833);

	if (m_nMobNum == MOB_SAD_SHADOW || m_nMobNum == MOB_SAD_SHADOW_2 )   // 슬픈 그림자는 광선검을 그려야 한다.
	{
		if(m_MobClassType==4)	 //영혼 -반투명하게...
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		}
		m_pCapSyong->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, NULL, m_CurFrame);
	}
	else if (m_nMobNum == MOB_DARK_SOUL) // 다크소울은...
		m_pCapSyong->Render(GET_D3DDEVICE());
	else if (m_nMobNum == MOB_SPECTER)	// nate 2005-10-25 : 스펙터 추가
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00008833);
		m_pCapSyong->Render(GET_D3DDEVICE());
	}
	else if (g_pMobLolo && m_nMobNum == MOB_LIGHT_BOOM) // 빛폭탄이여...
		g_pMobLolo->Render(GET_D3DDEVICE(), m_CurFrame);
	////2003 공간몹일때는 공격할때만 나타나게..
	else if(m_MobClassType==1)
	{
		if(m_bAttack	//공격시 또는 내가 레이다 차고 있을때에만 보이게 함.
				|| (g_pRoh->m_Wearing[WEARING_BRACELET] && g_pRoh->m_Wearing[WEARING_BRACELET]->m_Vnum == 829)
				|| (g_pRoh->m_Wearing[WEARING_BRACELET] && g_pRoh->m_Wearing[WEARING_BRACELET]->m_Vnum == 841) //1202 공간제어기도 레이다처럼...
				|| (g_pRoh->m_Wearing[WEARING_CLOAK]
					&& ( (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum>=695 && g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum<=756)
						 || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum>=950 && g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum<=955)
						 || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==3068) || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==10374)
						 || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==10453)  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==11066)  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==11067)
						  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==11087)) // its #8641 by.Sol9 [2012.05.14] // Update Elite
					&& (0x1 & g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special)))
		{
			m_pCapSyong->Render(GET_D3DDEVICE());
		}
	}   //Effect 10% ho red mobs
	else if( m_nMobNum == 38||m_nMobNum == 331||m_nMobNum==333||m_nMobNum==334||m_nMobNum==348// 워 자이언트 ,강화 다크소울도 피가 딸리면 열받게...
			 ||(m_nMobNum==352&&m_MobClassType!=4)||m_nMobNum==353||m_nMobNum==354||m_nMobNum==355||m_nMobNum==356
			 ||m_nMobNum==555||m_nMobNum==556||m_nMobNum==557||m_nMobNum==563 || m_nMobNum == 1006 || m_nMobNum == 709 || m_nMobNum == 710
		|| m_nMobNum == 711 || m_nMobNum == 712 || m_nMobNum == 713 || m_nMobNum == 714 || m_nMobNum == 715 || m_nMobNum == 716 
		|| m_nMobNum == 773|| m_nMobNum == 774 || m_nMobNum == 775  || m_nMobNum == 5001 || m_nMobNum == 5002
		|| m_nMobNum == 5013 || m_nMobNum == 5014 || m_nMobNum == 5015 || m_nMobNum == 5016
		|| m_nMobNum == 5044 || m_nMobNum == 5045 || m_nMobNum == 5046 || m_nMobNum == 5047 || m_nMobNum == 5048
		|| m_nMobNum == 5069 || m_nMobNum == 5070 || m_nMobNum == 5071 || m_nMobNum == 5072 
		|| m_nMobNum == 6036 || m_nMobNum == 6038 || m_nMobNum == 648 || m_nMobNum == 6040 || m_nMobNum == 6041 
		|| m_nMobNum == 6042 || m_nMobNum == 6043 || m_nMobNum == 6044 || m_nMobNum == 6045 || m_nMobNum == 6046
		|| m_nMobNum == 6065 || (m_nMobNum >= 1009 && m_nMobNum <= 1026) || m_nMobNum == 1031 || m_nMobNum == 1036 || m_nMobNum == 1041 || m_nMobNum == 1046
		 || m_nMobNum == 1051 || m_nMobNum == 1056 || m_nMobNum == 1061 || m_nMobNum == 1062)
	{
		//화이트혼던젼 보스몹들 피 10%부터 붉은색으로

		if( m_Vital <= 10 )
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xffff0000);

		if (IsDead())
		{
			float csy_alpha = 1.0f - ((float)m_DeadFrame/110);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

			DWORD alphaArg1, alphaArg2, alphaOp;
			GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG1, &alphaArg1);
			GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG2, &alphaArg2);
			GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAOP,  &alphaOp);
			GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);

			for( int obcnt = 0; obcnt < m_pCapSyong->m_ObjectCount; obcnt++ )
				for( int vtxcnt = 0; vtxcnt < m_pCapSyong->m_Csy[obcnt].m_VertexCount; vtxcnt++ )
					m_pCapSyong->m_Csy[obcnt].m_Vertices[vtxcnt].diffuse = D3DRGBA(0.75f, 1.0f, 0.75f, csy_alpha);

			m_pCapSyong->Render(GET_D3DDEVICE());

			GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG1, alphaArg1);
			GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAARG2, alphaArg2);
			GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_ALPHAOP, alphaOp);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
		}
		else
			m_pCapSyong->Render(GET_D3DDEVICE());
	}
	else if( m_nMobNum == 463 ) // 태국 3주년 기념케잌, 크기가 커서 라이트 받으면 안이쁘므로 라이트를 몽땅 꺼버린다.
	{
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

		m_pCapSyong->Render(GET_D3DDEVICE());

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	}
	else if( nMutantAlphaOpt ) // 뮤턴트 속성이 반투명인 몹이면....
	{
		if( nMutantAlphaOpt == 2 ) // 반투명
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTCOLOR);
			m_pCapSyong->Render(GET_D3DDEVICE());
		}
		else if( nMutantAlphaOpt == 3 ) // 태국 할로윈용 음영 반전
		{
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_INVSRCCOLOR);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ZERO);
			m_pCapSyong->Render(GET_D3DDEVICE());
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
		}
	}
	else if( m_nMobNum== 678 || m_nMobNum == 679 || m_nMobNum == 834 || m_nMobNum == 835 || m_nMobNum == 836 || m_nMobNum == 837 )
	{
		if( CheckRenderHolloweenMob(m_nMobNum) == true )
		{
			m_pCapSyong->Render(GET_D3DDEVICE());
		}
		else
		{
			return;
		}
	}
	else
	{
		if(m_MobClassType!=4&&m_MobClassType!=10)
			m_pCapSyong->Render(GET_D3DDEVICE());
	}

	// [3/30/2007 Theodoric] 생명력 바 표시 :: 캐릭터가 공격중인 몹 색상 변경
	if (bMouseOn || pCMyApp->m_pHeroAttGoMob == this )
	{
		if( g_dwClientCountry == CTRY_JPN )
		{
			//  [9/30/2008 parkmj] 일본 영웅석상, 빅 스노우맨 - 마우스 오버시 색상변경 막음
			if( this->m_nMobNum == 613 || this->m_nMobNum == 646 )
				return;

			// connie [2009/3/9] -  일본 6주년이벤트 - 마우스 오버시 색상변경 막음
			if( this->m_nMobNum == 616 )
				return;
		}


		ItemOutlineMgr* outlineMgr = ItemOutlineMgr::GetInstance();
		float fWidth = outlineMgr->GetWidth(eOutlineType_NpcSelect);
		DWORD dwColor = outlineMgr->GetColor(eOutlineType_NpcSelect, 0);

		if( (m_Flag & NPC_SUMMON) == false )
		{
			m_pCapSyong->RenderOutline(GET_D3DDEVICE(), g_nEmptyTextureNum, fWidth, dwColor, g_pCapsyongTexture);
		}

		BOOL lightEnable, lightEnable2;
		GET_D3DDEVICE()->GetLightEnable(0, &lightEnable);
		GET_D3DDEVICE()->GetLightEnable(1, &lightEnable2);
		GET_D3DDEVICE()->LightEnable(0, FALSE);
		GET_D3DDEVICE()->LightEnable(1, FALSE);

		if ((m_Flag & NPC_SHOPKEEPER) || (m_Flag & NPC_GUARD))
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00008010);
		}
		else if(pCMyApp->m_pHeroAttGoMob == this ) // 공격중인 몹이라면 색상을 바꾼다.
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00AA6666);
		}
		else // 마우스가 오버된 몹.
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00888888);
		}

		// 라다와 식목일 나무일 경우 어택을 위한 붉은색 블랜딩을 하지 않는다.
		if (m_nMobNum != 25
				&& m_nMobNum != 323
				&& m_nMobNum != 479
				&& !(m_nMobNum >= 420 && m_nMobNum <= 424) )
		{
			// 라다는 좀 그렇군...
			if (m_nMobNum == MOB_LIGHT_BOOM)
			{
				// 빛 폭탄은 롤로...
				if( g_pMobLolo )
					g_pMobLolo->Render(GET_D3DDEVICE(), m_CurFrame);
			}
			else
			{
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
				m_pCapSyong->Render(GET_D3DDEVICE(), -1, TRUE);
			}
		}

		// 마우스 on일때 윤곽선 외에는 밝은 색으로
		GET_D3DDEVICE()->LightEnable(0, lightEnable);
		GET_D3DDEVICE()->LightEnable(1, lightEnable2);
	}
	/*
		if (bMouseOn)
		{
			BOOL lightEnable, lightEnable2;
			GET_D3DDEVICE()->GetLightEnable(0, &lightEnable);
			GET_D3DDEVICE()->GetLightEnable(1, &lightEnable2);
			GET_D3DDEVICE()->LightEnable(0, FALSE);
			GET_D3DDEVICE()->LightEnable(1, FALSE);

			if ((m_Flag & NPC_SHOPKEEPER) || (m_Flag & NPC_GUARD))
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00008010);
			else
				// 마우스 on일때 윤곽선은 빨간색으로.
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00774545);

			// 라다와 식목일 나무일 경우 어택을 위한 붉은색 블랜딩을 하지 않는다.
			if (m_nMobNum != 25
				&& m_nMobNum != 323
				&& !(m_nMobNum >= 420 && m_nMobNum <= 424) )
			{ // 라다는 좀 그렇군...
				if (m_nMobNum == MOB_LIGHT_BOOM)
				{ // 빛 폭탄은 롤로...
					if( g_pMobLolo )
						g_pMobLolo->Render(GET_D3DDEVICE(), m_CurFrame);
				}
				else
				{
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
					m_pCapSyong->Render(GET_D3DDEVICE(), -1, TRUE);
				}
			}

			// 마우스 on일때 윤곽선 외에는 밝은 색으로
			GET_D3DDEVICE()->LightEnable(0, lightEnable);
			GET_D3DDEVICE()->LightEnable(1, lightEnable2);
		}
	*/
	// nate 2005-05-23 : 매트릭스 지휘자 소환몹인지 체크
	if( m_MobAttributeType > 0		// m_MobAttributeType == 100은 반란이벤트 용
			&& m_MobAttributeType < 4
			&& ( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX
				 || pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009
			   )
			&& pCMyApp->m_pUIMgr->m_pMatrixMark )
	{
		D3DMATRIX matWorld;
		GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, 0xffaaaabb );

		switch(pCMyApp->m_nCurWorldIdx)
		{
		case WORLD_MATRIX:
			switch(m_MobAttributeType)
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
			switch(m_MobAttributeType)
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
	}

	if(m_MobClassType==1)
	{
		//공간 계열 -검은색
		if(m_bAttack)	//공격시 또는 내가 레이다 차고 있을때에만 보이게 함.
		{
			if( !(g_pRoh->m_Wearing[WEARING_BRACELET] && g_pRoh->m_Wearing[WEARING_BRACELET]->m_Vnum == 829)
					&& !(g_pRoh->m_Wearing[WEARING_BRACELET] && g_pRoh->m_Wearing[WEARING_BRACELET]->m_Vnum == 841) //1202
					&& !(g_pRoh->m_Wearing[WEARING_CLOAK]
						 && ( (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum>=695 && g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum<=756)
							  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum>=950 && g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum<=955)
							  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==3068) || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==10374)
							  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==10453)
							  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==11066)
							  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==11067)
							  || (g_pRoh->m_Wearing[WEARING_CLOAK]->m_Vnum==11087)) // its #8641 by.Sol9 [2012.05.14] // Update Elite
						 && (0x1 & g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special)))
			{
				GET_D3DDEVICE()->LightEnable(0, FALSE);
				GET_D3DDEVICE()->LightEnable(1, FALSE);
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00000000);

				m_pCapSyong->Render(GET_D3DDEVICE());

				GET_D3DDEVICE()->LightEnable(0, TRUE);
				GET_D3DDEVICE()->LightEnable(1, TRUE);
			}
		}
	}
	else if(m_MobClassType==2)
	{
		//파괴  -파란색 	 나중에 정리..^^;
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		DWORD alphaRef;
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000010);

		m_pCapSyong->Render(GET_D3DDEVICE(), g_MobEffectTexture[ MOB_EFF_BLUE ], FALSE, FALSE, NULL,0/*m_EffFrame*/);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF  , alphaRef);
	}
	else if(m_MobClassType==3)
	{
		//저주 -붉은색으로 표시
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		DWORD alphaRef;
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000010);

		m_pCapSyong->Render(GET_D3DDEVICE(), g_MobEffectTexture[ MOB_EFF_RED ], FALSE, FALSE, NULL,0/*m_EffFrame*/);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF  , alphaRef);
	}
	// 반 투명한 Mob 을 Render(소환수 Render)
	// wan:2004-12 : m_MobClassType=10 - 베라딘의 윈터나이트 소환수
	else if(m_MobClassType==4||m_MobClassType==10)
	{
		// 현재 그려야 할 Mob이 소환수 라면 Vital 을 Display
		if( pCMyApp->m_pUIMgr->m_pSummonMob[0] == this)	// (0)일반 소환수만 에너지바 보임
			pCMyApp->DisplaySummonVital(this);
		if(m_nMobNum==352 && m_Vital <= 10 )
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xffff0000);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

		m_pCapSyong->Render(GET_D3DDEVICE());
	}
	//m_MobClassType = 9 : 디바인 스톤
	else if(m_MobClassType == 9 )
	{
		pCMyApp->DisplayDivineVital(this);
		g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 100, FALSE);

		if( !m_RunSmokeFrame ) // 스톤배쉬시(스톤배쉬중 스톤 때린 타이밍부터)엔 이펙트 제거하도록.. ///하이콤보)
		{
			if(m_nMobNum == 5086)
				g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_5_0 ] ), FALSE);
			if(m_nMobNum == 5085)
				g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_4_0 ] ), FALSE);
			if(m_nMobNum == 5081)
				g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_3_0 ] ), FALSE);
			if(m_nMobNum == 5080)
				g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_2_0 ] ), FALSE);
			if(m_nMobNum == 367 || m_nMobNum == 778 )
				g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_1_0 ] ), FALSE);
			if(m_nMobNum == 368)
				g_pShadowRect->Render(g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_0_0 ] ), FALSE);
		}
		else // 높이 내려가기 처리. ///하이콤보
		{
			m_RunSmokeFrame += 2; // 디바인스톤이 땅에 들어가는 양.
			if( m_RunSmokeFrame > 8 )
			{
				m_RunSmokeFrame = 8;
			}
		}
	}

	if( pCMyApp->m_pUIMgr->m_pSummonMob[1] == this)	// 자신의 서먼 플라워 에너지바 보이도록... ///07-04-10 버젼 싱크 빠져있었음.함께 넣자.
		pCMyApp->DisplaySummonVital(this);

	///2003 몹이 상태주문(석화,마비)에 걸렸을때 효과를 여기에 넣음
	if(m_CancelState)	//상태가 걸려있다면
	{
		if( m_ShowConditionEffect == FALSE )	// 효과가 그려지고 있는지...검사...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, NULL, this, EFFECT_CONDITION);
			m_ShowConditionEffect = TRUE;
		}
	}

	if(m_CancelState2)	//상태가 걸려있다면
	{
		if( m_ShowConditionEffect2 == FALSE )	// 효과가 그려지고 있는지...검사...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, NULL, this, EFFECT_CONDITION2);
			m_ShowConditionEffect2 = TRUE;
		}
	}

	//2003 책마법,해제 효과.
	if(m_ReceiveBook!=0)
	{
		switch (m_ReceiveBook)
		{
		case EFFECT_BLACKHOLE :
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, NULL, this, EFFECT_BLACKHOLE);
			break;
		case EFFECT_BOOK 	 :
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, NULL, this, EFFECT_BOOK);
			break;
		case EFFECT_REMOVE 	 :
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, NULL, this, EFFECT_REMOVE );
			break;
		}
		m_ReceiveBook=0;
	}

	// nate 2004 - 8 : 마신 다이오르 색상 변경 관련
	// 서버값 7 : 하늘색, 11 : 핑크색
	if ( m_Mutant != 5 &&
			( ( m_Mutant > 0 && m_Mutant < 8 )
			  || ( m_nMobNum > 178 && m_nMobNum < 188 ) ) )
	{
		// 변형몹이라우~~
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		DWORD alphaRef;
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000010);

		m_pCapSyong->Render( GET_D3DDEVICE(),
							 g_MobEffectTexture[ m_Mutant ],
							 FALSE, FALSE, NULL,
							 m_EffFrame );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHAREF  , alphaRef );
	}

	if (m_nMobNum == MONSTER_CONTROL_GUARD )
	{
		if( !isControlLight )
		{
			CNkMob *pMob;
			pMob = pCMyApp->m_pUIMgr->FindMobById(m_nVirId);
			pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_CONTROL_LIGHT_GUARD);
			isControlLight = TRUE;
		}
	}
	else if (m_nMobNum == MONSTER_CONTROL_DAMAGE )
	{
		if( !isControlLight )
		{
			CNkMob *pMob;
			pMob = pCMyApp->m_pUIMgr->FindMobById(m_nVirId);
			pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_CONTROL_LIGHT_DAMAGE);
			isControlLight = TRUE;
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	if( bMutantMaterial ) // 매터리얼을 이용해 뮤턴트 색상을 표현했었다면... ///태국뮤턴트이벤트
	{
		D3DMATERIAL7 mtrl;
		D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f ); // 매터리얼을 돌려준다.
		GET_D3DDEVICE()->SetMaterial( &mtrl );
	}

	//////////////////////////////////////////////////////////////////////////
	// pick buffer 쓰기.
	if ( bDontPick == FALSE
			&& !IsDead()
			&& !IsTrap()
			&& GET_D3DPICK()
			&& pCMyApp->m_pUIMgr->m_pSummonMob[ 1 ] != this
			&& !(m_Flag & NPC_FISH)
			&& (!(m_Flag & NPC_SUMMON)
				|| ((m_Flag & NPC_SUMMON)
					&& pCMyApp->m_pUIMgr->m_SummonMobControl) ) )
	{
		GET_D3DPICK()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
//		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///피킹버그수정. DWORD 형으로 변환한걸쓰지말고 그냥 WORD형을 바로 색상으로 넣는다. 이렇게 해도 32767개 까지 동시 피킹이 가능할 터. 변환중 뭔가 꼬이는 경우가 있는듯해서 이렇게 변경함. (07-01-02 원석)
		GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );

		if (m_pBound)
			m_pBound->Render(GET_D3DPICK(), -1, TRUE);
		else if (m_pCapSyong->m_BoundNum > 0)
			m_pCapSyong->RenderBound(GET_D3DPICK());
		else
			m_pCapSyong->Render(GET_D3DPICK(), -1, TRUE);
		GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	}


	if (!IsDead() && pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz)))
	{
		if (m_FlyStone >= 0 && m_FlyStone < FLY_STONE_NUM)
		{
			if( g_FlyStones[m_FlyStone] )
				g_FlyStones[m_FlyStone]->Render(GET_D3DDEVICE());

			D3DMATRIX matWordI;
			D3DUtil_SetIdentityMatrix(matWordI);
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWordI );
			SAFE_DELETE(m_pDropStar);

			if( m_pDropStar )
				m_pDropStar->Render(m_wx, m_wy, m_wz);
		}
	}

	if( m_nMobNum == MOB_AIREN )   // 떨어지는 별들? 팅커벨 소환물 효과
	{

		if( !m_NkMob_Particle )
		{
			m_NkMob_Particle = new Deco_Particle[MAX_NKMOB_PARTICLE];
			Initial_Particle();
			m_bDrawEffects = TRUE;

			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_MOBPARTICLE;
			ef_sort.num = m_nMobNum;
			ef_sort.pMobTo = this;
			InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
	}


	if( m_nMobNum == 317 || m_nMobNum == 318 ) // 할아버지, 할머니는 밑에 돗자리를 깔아드리자. ///돗자리
	{
		// 둘다 찍혀야할 때는 둘중 하나에만 찍고 위치 보정하는 방식으로 하자.
		if( g_pMattingLolo && GET_D3DDEVICE() )
			g_pMattingLolo->Render( GET_D3DDEVICE() );
	}

}

void CNkMob::SetChaCondition(int Condition, int cancel)
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

void CNkMob::SetChaCondition2(int Condition, int cancel)
{
	m_ChaCondition2 = Condition;
	m_CancelState2 = cancel;
}

void CNkMob::InsertEffectNum(int num)
{
	m_listEffect.insert(m_listEffect.begin(), num);
}

void CNkMob::Initial_Particle()
{
	int GoldDust_particle_start_time[MAX_NKCHA_PARTICLE] = { 0, };//0, 10, 15, 20, 25, 30, 35, 40, 45 };

	for( int i = 0; i < MAX_NKCHA_PARTICLE; i++ )
	{
		m_NkMob_Particle[i].start_time = GoldDust_particle_start_time[i];
		m_NkMob_Particle[i].life_time = 0;
		m_NkMob_Particle[i].cur_time = 0;
		m_NkMob_Particle[i].scale = 1.0f;
		m_NkMob_Particle[i].fade = 0.5f;

		m_NkMob_Particle[i].red = 1.0f;
		m_NkMob_Particle[i].green = 1.0f;
		m_NkMob_Particle[i].blue = 1.0f;

		m_NkMob_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		m_NkMob_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		m_NkMob_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		m_NkMob_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

		m_NkMob_Particle[i].texIndex = 0;
		m_NkMob_Particle[i].texFrame = 0;
	}
	return;
}

//=====================================================================
// nate 2004 - 4 : Crash_Miss
void CNkMob::DrawCrash_Miss( int nI, int nElapTime )
// Crash_Miss 창 그리기
{
	if( nElapTime < 7 )
		nElapTime = 7;

	D3DMATRIX matPipeline;
	D3DVECTOR vSrc, vWorld;

	vWorld = D3DVECTOR( m_wx, m_wy + 23.0f, m_wz );

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

	// Miss
	if( m_byShowCrash_Miss == 2 )
		m_pCrash_MissPopup[ nI ]->Draw(
			vSrc.x,	vSrc.y,
			g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_MISS ] ),	// nate 2004 - 7  Image Manager
			nElapTime );
}

void CNkMob::SetShowCrash_Miss( BYTE byCrash_Miss )
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

void CNkMob::RenderCrash_Miss()
{
	m_nElapTime = ( timeGetTime() - m_dwElapTime ) / 5;

	// 커지는 크기 조절
	if( m_nElapTime > 100 ) // 05-02-01 / 50에서 100으로 수치 늘림( 수정자:이원석 )
	{
		m_byShowCrash_Miss = 0;
		m_dwElapTime = 0;
		return;
	}
	// 정지 크기 조정
	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
		DrawCrash_Miss( i, m_nElapTime - 10 * i );
}
//----------------------------------------------------------------------------
//-- InitCrash_Misspopuop
//----------------------------------------------------------------------------
void CNkMob::RenderNPC_Event( int nEventNum )
{
	D3DMATRIX matPipeline;
	D3DVECTOR vSrc, vWorld;

	float Y_Plus = 23.0f; // 캐릭터의 어느정도 위에 찍을지의 보정치.

	if( nEventNum == 4 ) // 안나의 BP상점. 안나는 키가 작으므로 보정치를 내려준다.  ( 05-08-09 by 원석 )
		Y_Plus = 17.0f;

	if( g_bGlobalMatrix )
		Y_Plus += 10.0f;

	vWorld = D3DVECTOR( m_wx, m_wy + Y_Plus, m_wz );

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

	//-- 빌보드로 변환 출력

	switch( nEventNum )
	{
	case 0:
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_NPC_EVENT ] ),1 );
		break;
	case 1:
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_GIFT_MESSAGE ] ),1 );
		break;
	case 2:
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_MSG_E_ENCOUNTER ] ),1 );
		break;
	case 3:	// nate 2005-03-24 : Matrix - 상점 NPC 머리에 가이드 출력
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SHOP_NPC ] ), 1 );
		break;
	case 4:  // BP 상점. (05-08-09 by 원석)
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_BP_SHOP_NPC ] ), 1 );
		break;
	case 5: // mungmae 05/12/12 얀 퀘스트
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_YAN_EVENT ] ), 1 );
		break;
	case 6:
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_LP_SHOP_NPC ] ), 1 );
		break;
	}	
}

// 우현씨가 Crash / Miss 를 위해 만든 함수
// NPC Event 도 여기서 초기화
// Lyul / 수정중
void CNkMob::InitCrash_MissPopup(int width, int height)
{
	if( !GET_D3DDEVICE() )
		return;

	for( int i = 0 ; i < CRASH_MISS_COUNT ; i ++ )
	{
		if( m_pCrash_MissPopup[ i ] )
		{
			m_pCrash_MissPopup[ i ]->SetAlphaPopup_Type(AlphaPopUp_Type__Char);
			m_pCrash_MissPopup[ i ]->LoadRes( width, height );
		}
	}

	if( m_NPC_Event )
	{
		m_NPC_Event->SetAlphaPopup_Type(AlphaPopUp_Type__Char);
		m_NPC_Event->LoadRes( width, height ); //-- Lyul 추가
	}
}
//=====================================================================
//에이디아 서먼피션 사용시 커지는 소환몹타입 지정
void CNkMob::SetFissionType(int FissionType)
{
	m_FissionType = FissionType;
}

BOOL CNkMob::IsTrap()
{
	if( m_nMobNum >= 430 && m_nMobNum <= 434 )
		return true;

	return false;
}

void CNkMob::MoveSkill(float tx, float tz, int nSkillIndex)
{
	// 타겟지점 저정
	m_tx = tx;
	m_tz = tz;
	float dist_x = m_tx - m_wx;
	float dist_z = m_tz - m_wz;

	// 캐릭의 방향을 구한다.
	if( nSkillIndex != SKILL_KNOCKBACK && nSkillIndex != SKILL_KNOCKBACK2 ) // 넉백은 방향 전환 안하게...
	{
		m_toDir = (float) atan2(dist_z, dist_x);
	}

	float scalar= sqrt(pow(dist_x,2) + pow(dist_z,2));
	float sc_rate = 0.0f;

	float* fFrame = NULL;
	if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // 넉백 ///페어리
	{
		sc_rate = scalar / 13.0f;
		fFrame = &m_fDash_Frame;
	}
	else
		return;

	m_dx = dist_x / sc_rate;    // 이동할 크기를 구한다
	m_dz = dist_z / sc_rate;	// 이동할 크기를 구한다

	float dist = dist_x != 0.0f ? dist_x : dist_z;
	float distMod = m_dx != 0.0f ? m_dx : m_dz;

	if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // 넉백 ///페어리
		*fFrame = dist / distMod - 0.2f;

	// 동작 구분
	if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // 넉백 ///페어리
		SetMotionState( MOB_KNOCK ); //넉백은 선채로 밀리게...

	m_NowMoveSkill = SKILL_KNOCKBACK2;
}

void CNkMob::InitMobChatPopup(int width, int height)
{
	if( !GET_D3DDEVICE() )
		return;

	if( m_pMobChatPopup )
		m_pMobChatPopup->LoadRes(width, height);
}

void CNkMob::DrawMobChatPopup()
{
	if( m_bShowMobChatPopup )
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

		m_pMobChatPopup->Draw( vScr.x, vScr.y);
	}
}

void CNkMob::SetMobChatMsg(char *chat_msg)
{
	CMultiText multiText;
	multiText.AddString( GetMobName(), RGB(252, 180, 180) );
	multiText.AddStringMulti(chat_msg, 15, TRUE, RGB(255, 251, 115));
	m_pMobChatPopup->PutString(&multiText, TRUE);
	m_pMobChatPopup->SetIndex(0);		// 말을 했을때의 챗 박스
	m_LastChatTime = timeGetTime();
}

bool CNkMob::CheckRenderHolloweenMob(const int& mobNum)
{
	if (mobNum== 678 || mobNum == 679 || mobNum == 834 || mobNum == 835 || mobNum == 836 || mobNum == 837)
	{
		if (g_pRoh->m_Wearing[WEARING_HELMET] &&
				(g_pRoh->m_Wearing[WEARING_HELMET]->m_Vnum == 1945 ||
				 g_pRoh->m_Wearing[WEARING_HELMET]->m_Vnum == 1946))
			return true;
		else if (g_pRoh->m_EventWearing[WEARING_HELMET] &&
				 (g_pRoh->m_EventWearing[WEARING_HELMET]->m_Vnum == 1945 ||
				  g_pRoh->m_EventWearing[WEARING_HELMET]->m_Vnum == 1946))
			return true;
		else
			return false;
	}

	return true;
}

void CNkMob::ExecuteBossSkill(int type)
{	
	int state = 0;
	int eff_type = 0;
	int start_frame = 0;
	float scale = 0;
	D3DVECTOR vpos;
	if( type == 0 )
	{
		state = MOB_EMPTY1;
		eff_type = FX_BOSS_ATTACK_02;
		start_frame = -32;
		vpos = D3DVECTOR(0, 0, 0);
		scale = 2;
	}
	else if( type == 1 )
	{
		state = MOB_EMPTY2;
		eff_type = FX_BOSS_DEBUFF;
		start_frame = -5;
		vpos = D3DVECTOR(25, 0, 0);
		scale = 2;
	}
	else if( type == 2 )
	{
		state = MOB_EMPTY3;
		eff_type = FX_BOSS_MAGIC;
		start_frame = -20;
		vpos = D3DVECTOR(50, -60, 0);
		scale = 3;
	}

	SetMotionState(state);

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	ef_sort.nType = eff_type;
	ef_sort.pMobFrom = this;
	ef_sort.nCurFrame = start_frame;
	ef_sort.vSortPos.x = scale;
	ef_sort.vPos = vpos;
	int eff_num = pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	EffectSort* p = pCMyApp->m_pFxSet->GetEffect(eff_num);
	p->nCurFrame = start_frame;
	InsertEffectNum(eff_num);	
}

float GetMobExceptioncale( CNkMob *DestMob ) // 예외적인 클라이언트 스케일이 적용되는 경우 스케일 값을 리턴해주는 함수.
{
	if( !DestMob )
		return 1.0f;

	if( DestMob->m_Mutant == 5 || DestMob->m_bQuestMob != FALSE )
	{
		return 1.5f;
	}

	if( pCMyApp->m_nCurWorldIdx == WORLD_DMITRON )
	{
		if( DestMob->m_nMobNum == 416
				|| DestMob->m_nMobNum == MOB_SPECTER
				|| ( DestMob->m_nMobNum > 391 && DestMob->m_nMobNum < 413 )
		  )
		{
			switch( DestMob->m_nMobNum )
			{
			case 401:
			case 402:
				return 2.0f;

			case 405:
			case 406:
				return 1.0f;

			case 416:
			case MOB_SPECTER:
				return 1.7f;
			}

			return 1.5f;

			/*
			float fScale = 1.5f;
			if( DestMob->m_nMobNum == 401 || DestMob->m_nMobNum == 402 )	// 스파이더, 레인스파이더
				fScale = 2.0f;
			else if( DestMob->m_nMobNum == 405 || DestMob->m_nMobNum == 406 )	// 철갑드래곤, 플라잉 데빌킹
				fScale = 1.0f;
			else if( DestMob->m_nMobNum == 416 || DestMob->m_nMobNum == MOB_SPECTER )	// nate 2005-10-25 : 스펙터 추가
				fScale = 1.7f;

			return fScale;
			*/
		}
	}

	if( DestMob->m_Mutant > 10000 ) // 특수 뮤턴트 속성 추가.(확대,색상 입히기)   ///태국뮤턴트이벤트
	{
		D3DMATRIX matS;

		int PlusScale = (DestMob->m_Mutant%1000) / 100; // 백의 자리를 확대 스케일로 사용.
		int MinusScale = (DestMob->m_Mutant%10000) / 1000; // 천의 자리를 축소 스케일로 사용.

		if( PlusScale ) // 확대 치가 있으면...
		{
			return ((float)PlusScale);
		}

		if( MinusScale ) // 확대 치가 있으면...
		{
			return (1.0f/(float)MinusScale);
		}
	}

	//---------------------------------------------------------------------------
	//-- 식목일 나무 Event
	//-- 2004.03.29
	//-- Lyul / 수정완료
	//-- 다 때리모 사라진다.
	//-- 초기 스케일은 0.5f 에서 죽기 직전은 1.5f
	//-- 20의 vital 을 가진다. 0.05 씩 multiply 하면 OK
	if( DestMob->m_nMobNum == 323 || DestMob->m_nMobNum == 479) //-- 식목일 나무
	{
		float grade = DestMob->m_Vital * 0.01f;

		return 1.5f-grade;
	}
	//---------------------------------------------------------------------------
	//중고렙 스킬 추가
	//서먼피션 분열몹이면
	if( DestMob->m_FissionType > 0 )
	{
		return (g_SummonScale+1.0f);
	}
	//---------------------------------------------------------------------------
	//강화몹 크기 조절
	//---------------------------------------------------------------------------
	switch( DestMob->m_nMobNum )
	{
	case 334:
		return 3.5f;
	case 331:
	case 348:
		return 1.7f;
	case 333:
	case 337:
	case 347:
	case 464:
		return 1.5f;
	case 338:
	case 339:
	case 340:
	case 341:
	case 342:
	case 343:
	case 344:
	case 345:
	case 346:
	case 349:
	case 350:
		return 1.2f;
	case 413:
	case 414:
	case 479:
		return 2.0f;
	}

	if( pCMyApp->IsCurWorldID_BossraidZone() )
	{
		float fScale = 1.0f;
		switch( DestMob->m_nMobNum )
		{
		case 535: // 강화 이른 어스카이트
		case 575: // 강화 라이온 커터
			return 1.5f;

		case 577: // 강화 레인스파이더
		case 597: // 강화 레인 스파이
		case 601: // 강화 데미가드
			return 2.0f;

		}
	}

	float fResult = MobScaleData::GetInstance()->GetScaleValue(DestMob->m_nMobNum);

	return fResult;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 점포상점 NPC 클래스( NKMob 클래스를 상속받아쓴다. 루틴은 완전 독립적, 멤버 변수들을 가져다쓰기위해 상속한다. ) ///점포상점
CUserShopNpc::CUserShopNpc() : CNkMob()
{
	m_MasterIndex = 0; // 상점 주인의 인덱스. 패킷을 보낼땐 이걸로 보내야 한다.
	ZeroMemory( m_strMasterName, sizeof(m_strMasterName) ); // 상점 주인 이름.
	ZeroMemory( m_strUserShopMent, sizeof(m_strUserShopMent) ); // 상점 멘트 스트링. 머리위에 표시해줘야한다.

	if( m_pCrash_MissPopup[0] ) // m_pCrash_MissPopup[0]을 멘트 팝업출력용으로 쓰기위해선 약간의 초기화가 더 필요하다.(폰트셋팅등)
	{
		m_pCrash_MissPopup[0]->Init(12,AlphaPopUp_Type__Char);
		m_pCrash_MissPopup[0]->SetRectSoft(TRUE);
	}

	m_bRemoveHair = FALSE; // 머리없앰 옵션 체크
	m_bRemoveFace = FALSE; // 얼굴없앰 옵션 체크
	m_bRemoveBody = FALSE; // 몸통없앰 옵션 체크
	m_bRemoveArms = FALSE; // 팔없앰 옵션 체크
	m_bRemoveLegs = FALSE; // 다리없앰 옵션 체크
	m_bRemoveFoot = FALSE; // 발없앰 옵션 체크
}

CUserShopNpc::CUserShopNpc( CClrAlloc *pClrAlloc ):CNkMob( pClrAlloc ) // 부모 클래스의 생성자를 이렇게 해야 제대로 호출되더라.
{
	m_MasterIndex = 0; // 상점 주인의 인덱스. 패킷을 보낼땐 이걸로 보내야 한다.
	ZeroMemory( m_strMasterName, sizeof(m_strMasterName) ); // 상점 주인 이름.
	ZeroMemory( m_strUserShopMent, sizeof(m_strUserShopMent) ); // 상점 멘트 스트링. 머리위에 표시해줘야한다.

	if( m_pCrash_MissPopup[0] ) // m_pCrash_MissPopup[0]을 멘트 팝업출력용으로 쓰기위해선 약간의 초기화가 더 필요하다.(폰트셋팅등)
	{
		m_pCrash_MissPopup[0]->Init(12,AlphaPopUp_Type__Char);
		m_pCrash_MissPopup[0]->SetRectSoft(TRUE);
	}

	m_bRemoveHair = FALSE; // 머리없앰 옵션 체크
	m_bRemoveFace = FALSE; // 얼굴없앰 옵션 체크
	m_bRemoveBody = FALSE; // 몸통없앰 옵션 체크
	m_bRemoveArms = FALSE; // 팔없앰 옵션 체크
	m_bRemoveLegs = FALSE; // 다리없앰 옵션 체크
	m_bRemoveFoot = FALSE; // 발없앰 옵션 체크
}

CUserShopNpc::~CUserShopNpc()
{
	// 부모 클래스의 소멸자가 호출된다.
}

void CUserShopNpc::FrameMove( BOOL bVisible )
{
	DWORD dwCurtime;
	DWORD dwTimeCap;
	unsigned int frame_count;

	dwCurtime = timeGetTime();

	frame_count = 0;
	if ( ((dwCurtime - m_dwBaseTime) * 0.001f) >= m_AniRate)
	{
		m_dwBaseTime += (DWORD) (m_AniRate*1000);
		frame_count = 1;
		while ( ((dwCurtime - m_dwBaseTime)*0.001f) >= m_AniRate )
		{
			frame_count++;
			m_dwBaseTime += (DWORD) (m_AniRate*1000);
		}
	}

	dwTimeCap = dwCurtime - m_dwTimeCapPrev;
	m_dwTimeCapPrev = dwCurtime;

	m_PrevFrame = m_CurFrame;
	m_CurFrame += frame_count;
//	m_EffFrame += frame_count;

	if (bVisible)
	{
		if( m_pCapSyong )
		{
			m_pCapSyong->Animate(m_pBiped[m_nMotionState], m_CurFrame);
		}

		if (m_pBound)
			m_pBound->Animate(m_pBiped[m_nMotionState], m_CurFrame);

	}

	// 한동작이 다 끝난 다음에 원상태(서있는상태)로 복귀하는 코드
	// 단 죽었을때에는 동작이 끝나면 끌난상태로 계속.
	if( m_CurFrame >= m_pBiped[m_nMotionState]->m_AnimCount )
	{
		m_CurFrame = 0;

		m_nMotionState = rand() % 5; // 5개 동작을 랜덤으로 돌려준다.
	}
}

void CUserShopNpc::Render( BOOL bMouseOn, BOOL bVisible, BOOL bDontPick )
{
	if( !GET_D3DDEVICE() || !GET_D3DPICK() || !m_pCapSyong )
		return;

	static D3DMATRIX matTrans, matRotate, matWorld;
	static DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting;

	FrameMove( bVisible );

	D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	// 주변광 저장
	DWORD dwAmbient;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	switch( m_nMobNum ) // Npc따라 관리하여 출력한다.
	{
	case 441: // 아르바이트 걸. // 이 녀석은 오브젝트가 9개로 나누어져있다.

		if( !m_bRemoveBody )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 0 ); // 몸통(옷)

		if( !m_bRemoveBody ) // 몸이랑 팔이 붙어있다.
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 1 ); // 벨트

		if( !m_bRemoveFoot )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 3 ); // 신발

		if( !m_bRemoveArms )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 4 ); // 팔

		if( !m_bRemoveHair ) // 머리카락 제거 옵션이 없을때만 출력.
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 2 ); // 머리카락

		if( !m_bRemoveFace ) // 얼굴 제거 옵션이 없을때만 출력.
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 5 ); // 얼굴

		if( !m_bRemoveLegs )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 6 ); // 다리

		if( !m_bRemoveArms )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 7 ); // 손

		m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 8 ); // 책

		break;

	default: // 따로 관리에 등록되어있지 않으면...
		m_pCapSyong->Render(GET_D3DDEVICE()); // 통짜 출력.
		break;
	}

	if( m_WearList.size() != 0 ) // 착용한 장비가 있다면... // 이 리스트엔 서버로 부터 Vnum을 받아서 아이템 배열의 ProtoNum(index)으로 바꾼 값을 가지고 있다.
	{
		CCapsyong *TempWearCsy = NULL;

		for( vector<int>::iterator prWear = m_WearList.begin() ; prWear != m_WearList.end() ; ++prWear )
		{
			TempWearCsy = GET_CSYC((*prWear), 1);

			if( TempWearCsy )
			{
				TempWearCsy->Animate( m_pBiped[m_nMotionState], m_CurFrame );	 // 부득이 하게 여기서 애니메이트.
				TempWearCsy->Render( GET_D3DDEVICE() );
			}
		}
	}

	if (bMouseOn)
	{
		BOOL lightEnable, lightEnable2;
		GET_D3DDEVICE()->GetLightEnable(0, &lightEnable);
		GET_D3DDEVICE()->GetLightEnable(1, &lightEnable2);
		GET_D3DDEVICE()->LightEnable(0, FALSE);
		GET_D3DDEVICE()->LightEnable(1, FALSE);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00008010);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

		m_pCapSyong->Render(GET_D3DDEVICE(), -1, TRUE );

		// 마우스 on일때 윤곽선 외에는 밝은 색으로
		GET_D3DDEVICE()->LightEnable(0, lightEnable);
		GET_D3DDEVICE()->LightEnable(1, lightEnable2);
	}

	// 멘트 팝업 그리기. ( m_pCrash_MissPopup[0]을 사용한다. )
	if( bVisible && (g_pNk2DFrame->GetControlBottom()->m_bSalesView||bMouseOn) ) // 장사채널이 꺼져있으면 안보이게... ///점포상점시 추가
	{
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

		m_pCrash_MissPopup[0]->Draw( vScr.x, vScr.y, 3 );
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	//////////////////////////////////////////////////////////////////////////
	// pick buffer 쓰기.
	if( GET_D3DPICK() && bDontPick == FALSE )
	{
		GET_D3DPICK()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
//		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///피킹버그수정. DWORD 형으로 변환한걸쓰지말고 그냥 WORD형을 바로 색상으로 넣는다. 이렇게 해도 32767개 까지 동시 피킹이 가능할 터. 변환중 뭔가 꼬이는 경우가 있는듯해서 이렇게 변경함. (07-01-02 원석)
		GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );

		if (m_pBound)
			m_pBound->Render(GET_D3DPICK(), -1, TRUE);
		else if (m_pCapSyong->m_BoundNum > 0)
			m_pCapSyong->RenderBound(GET_D3DPICK());
		else
			m_pCapSyong->Render(GET_D3DPICK(), -1, TRUE);

//		GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	}
	//////////////////////////////////////////////////////////////////////////
}


// 점포상점 NPC 클래스( NKMob 클래스를 상속받아쓴다. 루틴은 완전 독립적, 멤버 변수들을 가져다쓰기위해 상속한다. )
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void NpcFileCollect( const char *FileName, const char *SrcPath, const char *MovePath ) // 특정 Npc Dta 에서 사용하는 파일들을 따로 모아주는 함수. ///국가대항전 전용 클라이언트 만들라고 추가.
{
	PFILE *fp = NULL;
	PFILE *fpCap = NULL;
	PFILE *fpSnd = NULL;
	char *buf = NULL;
	char path_name[255] = "";
	char path_name2[255] = "";
	int npc_count = 0, len = 0;
	char fname[255] = "";
	BYTE TempByte = 0;
	int TempInt = 0;
	float TempFloat = 0.0f;
	WORD TempWord = 0;
	char TempString[255] = "";

	fp = pfopen( FileName, "rb" );
	if (fp == NULL)
	{
		MessageBox(NULL, "Failed to load map data", "error!", MB_OK);
		return;
	}

	buf = new char[255];
	pfread(&npc_count, sizeof(int), 1, fp);

	// 순차적으로 읽어나간다.
	for (int i = 0; i < npc_count; i++)
	{
		pfread(&TempInt, sizeof(int), 1, fp);		// 맙 번호
		pfread(&TempInt, sizeof(int), 1, fp);						// 화일 이름 길이.
		pfread(TempString, TempInt, 1, fp);				// 맙 이름 읽는다.
		TempString[TempInt] = NULL;

		// 캡숑 화일 읽기.
		pfread(&TempInt, sizeof(int), 1, fp);						// 캡송 화일 이름 길이.
		assert(TempInt > 0);
		pfread(fname, TempInt, 1, fp);								// 캡슝은 맙에 언제나 고유하게 하나만 있다.
		fname[TempInt] = NULL;
		strcpy(path_name, SrcPath );
		strcat(path_name, "csy/" );
		strcat(path_name, fname);

		strcpy(path_name2, MovePath);
		strcat(path_name2, "csy/" );
		strcat(path_name2, fname);

		// 캡숑 파일 복사.
		CopyFile( path_name, path_name2, TRUE ); // 뒤에 TRUE는 파일이 존재할경우 스킵하라는 플래그.


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// 캡숑안의 텍스쳐 읽기.
		{
			fpCap = pfopen(path_name, "rb");
			//		m_pMobData[i].pCapsyong = new CCapsyong(g_pMobTexture);
			//		m_pMobData[i].pCapsyong->Read(fpCap);
			int i;

			char name[100];
			int name_len;

			int object_num;
			int all_face_count = 0;
			int low_face_count = 0;
			int m_bCsy2 = 0;

			pfread(&object_num, sizeof(int), 1, fpCap);

			if (object_num == 181818)
			{
				int i_temp;
				for (i = 0; i < 8; i++)
				{
					pfread(&i_temp, sizeof(int), 1, fpCap);
				}
				pfread(&object_num, sizeof(int), 1, fpCap);
			}
			else if( object_num == 282828 ) ///CSY2
			{
				m_bCsy2 = TRUE;

				int i_temp;
				for (i = 0; i < 8; i++)
				{
					pfread(&i_temp, sizeof(int), 1, fpCap);
				}
				pfread(&object_num, sizeof(int), 1, fpCap);
			}

			// Object의 갯수만큼 읽어온다.
			for (int obj_idx = 0; obj_idx < object_num; obj_idx++)
			{
				// Material의 Texture파일 이름을 읽는다.
				pfread(&name_len, sizeof(int), 1, fpCap);
				if (name_len)
				{
					pfread(name, name_len, 1, fpCap);
					name[name_len] = '\0';

					// 텍스쳐 파일 복사. 텍스쳐는 bmp나 tga확장자로 컨버팅된 상태로 폴더에 들어있어야함에 유의.
					strcpy(path_name, SrcPath);
					strcat(path_name, "textr//" );
					strcat(path_name, name);
					strcpy(path_name2, MovePath);
					strcat(path_name2, "textr//" );
					strcat(path_name2, name);
					CopyFile( path_name, path_name2, TRUE ); // 뒤에 TRUE는 파일이 존재할경우 스킵하라는 플래그.
				}

				// Read Vertices
				int vertex_num;

				pfread(&vertex_num, sizeof(int), 1, fpCap);

				for (i = 0; i < vertex_num; i++)
				{
					if( m_bCsy2 ) ///CSY2
					{
						pfread(&TempByte, sizeof(BYTE), 1, fpCap);

						if( TempByte > MAX_BPD_WEIGHT_NUM )
							TempByte = MAX_BPD_WEIGHT_NUM;

						for( int j = 0 ; j < TempByte ; ++j )
						{
							pfread(&TempFloat, sizeof(float), 1, fpCap);
							pfread(&TempFloat, sizeof(float), 1, fpCap);
							pfread(&TempFloat, sizeof(float), 1, fpCap);

							pfread(&TempFloat, sizeof(float), 1, fpCap);
							pfread(&TempFloat, sizeof(float), 1, fpCap);
							pfread(&TempFloat, sizeof(float), 1, fpCap);


							pfread(&TempWord, sizeof(short), 1, fpCap);
							pfread(&TempFloat, sizeof(float), 1, fpCap);
						}

						pfread(&TempFloat, sizeof(float), 1, fpCap);
						pfread(&TempFloat, sizeof(float), 1, fpCap);
					}
					else
					{
						pfread(&TempFloat, sizeof(float), 1, fpCap);
						pfread(&TempFloat, sizeof(float), 1, fpCap);
						pfread(&TempFloat, sizeof(float), 1, fpCap);

						pfread(&TempFloat, sizeof(float), 1, fpCap);
						pfread(&TempFloat, sizeof(float), 1, fpCap);
						pfread(&TempFloat, sizeof(float), 1, fpCap);

						pfread(&TempFloat, sizeof(float), 1, fpCap);
						pfread(&TempFloat, sizeof(float), 1, fpCap);

						pfread(&TempWord, sizeof(short), 1, fpCap);
					}
				}

				// Read Faces
				int face_num;
				pfread(&(face_num), sizeof(int), 1, fpCap);
				int vert_idx;

				for (i = 0; i < face_num * 3; i++)
				{
					pfread(&vert_idx, sizeof(int), 1, fpCap);
				}

				// Read Faces Low
				pfread(&(face_num), sizeof(int), 1, fpCap);

				if (face_num > 0)
				{
					for (i = 0; i < face_num * 3; i++)
					{
						pfread(&vert_idx, sizeof(int), 1, fpCap);
					}
				}
			}

			pfclose(fpCap);
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////



		// 텍스춰 로딩...
		pfread(&len, sizeof(int), 1, fp);						// 텍스춰 화일 이름 길이.
		if (len > 0)
		{
			pfread(fname, len, 1, fp);
			fname[len] = NULL;

			// 텍스쳐 파일 복사.
			strcpy(path_name, SrcPath);
			strcat(path_name, "textr//" );
			strcat(path_name, fname);
			strcpy(path_name2, MovePath);
			strcat(path_name2, "textr//" );
			strcat(path_name2, fname);
			CopyFile( path_name, path_name2, TRUE ); // 뒤에 TRUE는 파일이 존재할경우 스킵하라는 플래그.
		}

		// 바운드 화일 읽기.
		pfread(&len, sizeof(int), 1, fp);						// 캡송 화일 이름 길이.
		if (len > 0)
		{
			pfread(fname, len, 1, fp);								// 캡슝은 맙에 언제나 고유하게 하나만 있다.
			fname[len] = NULL;

			strcpy(path_name, SrcPath);
			strcat(path_name, "csy//" );
			strcat(path_name, fname);
			strcpy(path_name2, MovePath);
			strcat(path_name2, "csy//" );
			strcat(path_name2, fname);
			CopyFile( path_name, path_name2, TRUE ); // 뒤에 TRUE는 파일이 존재할경우 스킵하라는 플래그.
		}

		for (int j=0; j < 5; j++)
		{
			// j는 stand, move, attack, die, knock
			// 스탠드 바이페드
			pfread(&len, sizeof(int), 1, fp);						// 바이페드 화일이름 길이.
			assert(len > 0);
			pfread(fname, len, 1, fp);
			fname[len] = NULL;

			strcpy(path_name, SrcPath);
			strcat(path_name, "bpd//" );
			strcat(path_name, fname);
			strcpy(path_name2, MovePath);
			strcat(path_name2, "bpd//" );
			strcat(path_name2, fname);
			CopyFile( path_name, path_name2, TRUE ); // 뒤에 TRUE는 파일이 존재할경우 스킵하라는 플래그.


			// 스탠드 사운드
			pfread(&len, sizeof(int), 1, fp);						// 사운드 파일 이름 길이.
			if ( len > 0)
			{
				// 사운드 파일이 있다.
				pfread(fname, len, 1, fp);
				fname[len] = NULL;

				strcpy(path_name, SrcPath);
				strcat(path_name, "wav//" );
				strcat(path_name, fname);
				strcpy(path_name2, MovePath);
				strcat(path_name2, "wav//" );
				strcat(path_name2, fname);
				CopyFile( path_name, path_name2, TRUE ); // 뒤에 TRUE는 파일이 존재할경우 스킵하라는 플래그.
			}

			pfread(&TempInt, sizeof(int), 1, fp);
		}

		// 맙 기타
		pfread(&TempInt, sizeof(int), 1, fp);
		pfread(&TempInt, sizeof(int), 1, fp);
		pfread(&TempInt, sizeof(int), 1, fp);
		pfread(&TempInt, sizeof(int), 1, fp);
		pfread(&TempInt, sizeof(int), 1, fp);
	}
	pfclose(fp);

	if( buf )
		delete [] buf;
}

//////////////////////////////////////////////////////////////////////////
#include "LHExcelReader.h"
MobScaleData::MobScaleData()
{
	LoadScript();
}

MobScaleData::~MobScaleData()
{

}

float MobScaleData::GetScaleValue(const int& mobIndex)
{
	MAP_SCALE::iterator itFind = m_mapScale.find(mobIndex);
	if( itFind != m_mapScale.end() )
	{
		return itFind->second;
	}

	return 1.0f;
}

void MobScaleData::LoadScript()
{
	m_mapScale.clear();

	LHExcelReader er;
	if( !er.Open("data/npc/npc_scale.txt") )
		return;

	int size = er.GetRowSize();

	int mobIndex = 0;
	float mobScale = 0;
	for(int row = 0; row < size; row++)
	{
		mobIndex = er.GetInt(row, 0);
		mobScale = er.GetFloat(row, 1);

		m_mapScale.insert(make_pair(mobIndex, mobScale));
	}
}