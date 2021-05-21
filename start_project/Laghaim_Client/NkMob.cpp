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
#include "ControlBottom.h" ///�������� �� �߰�
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
#include "g_stringmanager.h" ///Ʃ�丮�� �� �߰�
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
extern CLolos	*g_pMattingLolo; ///���ڸ�

extern int g_MatrixSvr;
extern BOOL	g_bNewTerrain;
//�����Ǽ� ��ũ������
extern float g_SummonScale;
extern int g_nCurWorldIdx;

extern BOOL g_bGlobalMatrix;
extern int g_nEmptyTextureNum;

void NpcFileCollect( const char *FileName, const char *SrcPath, const char *MovePath ); // Ư�� Npc Dta ���� ����ϴ� ���ϵ��� ���� ����ִ� �Լ�. ///���������� ���� Ŭ���̾�Ʈ ������ �߰�.

//////////////////////////////////////////////////////////////////////
// ������Ÿ �ε�.
//////////////////////////////////////////////////////////////////////

int CMobData::npc_count;

// -------------------------------------------------------------------
// ���� ���õ� ĸ���� ������� �ε�.
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

// ����Ʈ ����
#define MOB_EFF_BLUE_DOT		0
#define MOB_EFF_RED				1
#define MOB_EFF_BLUE			2
#define MOB_EFF_GREEN			3
#define MOB_EFF_YELLOW			4
//====================================================
// nate 2004 - 4 : quest - ���� ���̿��� ���� ����
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
	// �ؽ�ó ��
//====================================================
	// nate 2004 - 4 : quest
	g_pMobTexture = new IndexedTexture(1000); // 06-11-22 �����ؼ� �ø�. (����)
//====================================================
	if( g_pMobTexture )
		g_pMobTexture->SetTexturePath("data/npc/textr/");

	// ����Ÿ �ε� Ŭ����
	g_pMobDataLoading = new MobDataLoading;
	if( g_pMobDataLoading )
	{
		g_pMobDataLoading->SetBipedDir("data/Npc/bpd/");
		g_pMobDataLoading->SetCapDir("data/Npc/csy/");
		// nate 2004 - 10 : �ִ� �ε� ���� ���� - nMaxBipedNum�� ������ ����
		g_pMobDataLoading->InitSize(60, 600);

		char filename[64] = {0,};
		sprintf( filename , "data/Npc/npc%d.dta" , world_num );
		g_pMobDataLoading->ReadDataFile(filename);
	}

	// �������� ���� �ؽ��� �ε�...
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
	// �ϳ��� �������̵�� ���������� ���� �� �ִ�.
	for (int i=0; i < m_nCurBipedNum; i++)
	{
		SAFE_DELETE(m_dpBiped[i]);
		SAFE_DELETE_ARRAY(m_dpBipedName[i]);
	}

	SAFE_DELETE_ARRAY(m_dpBiped);
	SAFE_DELETE_ARRAY(m_dpBipedName);
	// ĸ�� ���� ����.
	// Npc�� ����Ǿ� �ִ� ��� ������ �����Ѵ�.
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
//	NpcFileCollect( file_name, ".//data//npc//", ".//backup//" ); // ���� npc ���������� �ʿ��Ҷ��� ����ȴ�.

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
		pfread(&(m_pMobData[i].nVnum), sizeof(int), 1, fp);		// �� ��ȣ
		pfread(&len, sizeof(int), 1, fp);						// ȭ�� �̸� ����.
		assert(len > 0);
		m_pMobData[i].mob_name = new char[len+1];
		pfread(m_pMobData[i].mob_name, len, 1, fp);				// �� �̸� �д´�.
		m_pMobData[i].mob_name[len] = NULL;
		pfread(&len, sizeof(int), 1, fp);						// ĸ�� ȭ�� �̸� ����.
		assert(len > 0);
		pfread(fname, len, 1, fp);								// ĸ���� ���� ������ �����ϰ� �ϳ��� �ִ�.
		fname[len] = NULL;
		strcpy(path_name, m_pCapDir);
		strcat(path_name, fname);

		m_pMobData[i].pCapsyong = new CCapsyong(g_pMobTexture);
		if( VF_ERR_NONE > m_pMobData[i].pCapsyong->Read(path_name) )
		{
			SAFE_DELETE(m_pMobData[i].pCapsyong);
			m_pMobData[i].pCapsyong = NULL;
		}

		pfread(&len, sizeof(int), 1, fp);						// �ؽ��� ȭ�� �̸� ����.
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


		pfread(&len, sizeof(int), 1, fp);						// ĸ�� ȭ�� �̸� ����.
		if (len > 0)
		{
			pfread(fname, len, 1, fp);								// ĸ���� ���� ������ �����ϰ� �ϳ��� �ִ�.
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



		for (int j=0; j<7; j++) // [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5���� 7�� �ø���.
		{
			// j�� stand, move, attack, die, knock
			// ���ĵ� �������
			pfread(&len, sizeof(int), 1, fp);						// ������� ȭ���̸� ����.
			assert(len > 0);
			pfread(fname, len, 1, fp);
			fname[len] = NULL;
			m_pMobData[i].pBpd[j] = LoadBiped(fname);				// ������� �����͸� �����Ѵ�.

			// ���ĵ� ����
			pfread(&len, sizeof(int), 1, fp);						// ���� ���� �̸� ����.
			if ( len > 0)
			{
				// ���� ������ �ִ�.
				pfread(fname, len, 1, fp);
				fname[len] = NULL;
				// ���� ���̵� �޾�æ��.

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

		// �� ��Ÿ
		pfread(&(m_pMobData[i].attack_frame), sizeof(int), 1, fp);
		pfread(&(m_pMobData[i].speed), sizeof(int), 1, fp);
		pfread(&(m_pMobData[i].flag), sizeof(int), 1, fp);		//
		pfread(&(m_pMobData[i].height), sizeof(int), 1, fp);		// ���� ��/��/��
		pfread(&(m_pMobData[i].flystone), sizeof(int), 1, fp);  // ���༮
	}

	pfclose(fp);
	if( buf )
		delete [] buf;
}

CBiped *MobDataLoading::LoadBiped(char *file_name)
{
	char *path_name = NULL;
	PFILE *fp=NULL;

	// ���� ������ �ε��� ����������� �����̸����� �˻�
	for (int idx=0; idx < m_nCurBipedNum; idx++)
	{
		if (!strcmp(m_dpBipedName[idx], file_name))
			return m_dpBiped[idx];
	}

	// ���� ������� �޸𸮸� �Ҵ��Ѵ�.. ��..
	m_dpBipedName[m_nCurBipedNum] = new char[strlen(file_name)+1];
	strcpy(m_dpBipedName[m_nCurBipedNum], file_name);
	m_dpBiped[m_nCurBipedNum] = new CBiped;

	// ��ο� ȭ�� �̸��� �����Ѵ�.
	path_name = new char[strlen(m_pBipedDir)+strlen(file_name)+1];
	strcpy(path_name, m_pBipedDir);
	strcat(path_name, file_name);

	m_dpBiped[m_nCurBipedNum]->Read(path_name);

	CBiped *pBiped =  m_dpBiped[m_nCurBipedNum];
	m_nCurBipedNum++;
	// npc�� �ε� �����е� �ִ� ������ ������Ʈ ���Ѿ���
	assert(m_nCurBipedNum <= m_nMaxBipedNum);

	delete [] path_name; // �ӽ÷� �� ������ ����. (by �̿���) ///�޸𸮴�������

	return pBiped;
}

//////////////////////////////////////////////////////////////////////
// CMobData Construction/Destruction - �ϴ� �� ����.
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

// �� Ŭ���������� ����Ÿ ������ ����.
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
	m_RegeningType = 0; // ���� �߿� ���õǴ� ���� ���� Ÿ��. �� �ð��� ���õǾ�������...�������ΰŴ�. ///�δ�1

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
	m_fDash_Frame = 0.0f; // �˹��� ���� ����.
	m_NowMoveSkill = 0; // �̵��� ��ų ���� ����.(���ʹ� ��� ������Ʈ�� ���� ���ϰ� �̰ɷ� �����ؼ� �˹�� �����ش�.)
	m_pMobChatPopup = new CAlphaPopUp;
	if( m_pMobChatPopup )
	{
		m_pMobChatPopup->Init(12,AlphaPopUp_Type__Char);
		m_pMobChatPopup->SetRectSoft(TRUE);
	}
	m_LastChatTime = timeGetTime() - SHOW_MOBCHATPOPUP_TIME;
	m_bShowMobChatPopup = FALSE;
	m_RegeningType = 0; // ���� �߿� ���õǴ� ���� ���� Ÿ��. �� �ð��� ���õǾ�������...�������ΰŴ�. ///�δ�1

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
	if (m_pClrAlloc)	// �Ҵ���� �ö��� �����Ѵ�.
		m_pClrAlloc->ColorFree(m_wColorIdx);

	if (m_bHeroAttCome)
		pCMyApp->SetHeroAttGoMob(NULL);

	// ���õ� ȿ���� ���ش�.
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
// Desc : �������� �¿��� ���� �̸��� �����´�. �˻�����....
// ---------------------------------------------------------------------
char *CNkMob::GetMobName()
{
	if( pCMyApp->m_pUIMgr->m_nNowPlayingEvent == EVENTACTION_TUTORIAL ) ///Ʃ�丮�� �߿� ���� ó��.
	{
		// Ʃ�丮���� '������(57)'�� �̸��� 'ī�̸�'�� �ٲ��ش�.
		if( m_nMobNum == 57 ) // ��� �����̸�.
		{
			// IDS_TUTORIAL_TEACHER : ī�̸�
			return ( (char *)G_STRING(IDS_TUTORIAL_TEACHER) ); // �̸� �ӽ�
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
// Desc : ���ο� ���� �����Ҷ�, �� ��ȣ�� �������� ���� ���̵�, �����Ѵ�.
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
			// �ε����� ������ �־ �Ǵµ�..
			m_pBiped[0] = pMobData[idx].pBpd[0];
			m_pBiped[1] = pMobData[idx].pBpd[1];
			m_pBiped[2] = pMobData[idx].pBpd[2];
			m_pBiped[3] = pMobData[idx].pBpd[3];
			m_pBiped[4] = pMobData[idx].pBpd[4];
			m_pBiped[5] = pMobData[idx].pBpd[5];	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5������ 7���� �ø���.
			m_pBiped[6] = pMobData[idx].pBpd[6];	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5������ 7���� �ø���.

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
			m_nSoundIdx[5] = pMobData[idx].idxSound[5];	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5������ 7���� �ø���.
			m_nSoundIdx[6] = pMobData[idx].idxSound[6];	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5������ 7���� �ø���.
			m_nSoundIdx[7] = -1;

			m_nSoundFrame[0] = pMobData[idx].nSoundFrame[0];
			m_nSoundFrame[1] = pMobData[idx].nSoundFrame[1];
			m_nSoundFrame[2] = pMobData[idx].nSoundFrame[2];
			m_nSoundFrame[3] = pMobData[idx].nSoundFrame[3];
			m_nSoundFrame[4] = pMobData[idx].nSoundFrame[4];
			m_nSoundFrame[5] = pMobData[idx].nSoundFrame[5];	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5������ 7���� �ø���.
			m_nSoundFrame[6] = pMobData[idx].nSoundFrame[6];	// [10/23/2007 Theodoric] npc 2���� ���� �߰��� ���� 5������ 7���� �ø���.
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
// Desc : ���� �ʱ� ���� ��ġ
// ---------------------------------------------------------------------
void CNkMob::SetInitPos(float wx, float wz, float dir)
{
	m_wx = wx;
	m_wz = wz;
	m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);

	// ��� �̺�Ʈ�� ���ؼ�.
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
// Desc : ���� �ʱ� ���� ��ġ
// ---------------------------------------------------------------------
void CNkMob::SetMotionState(unsigned short nState)
{
	if (m_nMotionState == 3) // �׾��µ�.. �� �� �ٷ�?
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
	case 4:						// �����̾�Ʈ ��Ƽ����
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

	// �� ���̾�Ʈ �׾��� �� ȿ��  //Effect mob die Pablo
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
		// wan:2004-12	ȭ��Ʈȥ ���� ���� �¿�
		if(m_nMobNum == 352)
			ef_sort.num = 0;//�¿�
		else
			ef_sort.num = 1;//����

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
// Desc : Ư�� ��ǥ �������� �̵�.
// ---------------------------------------------------------------------
BOOL CNkMob::Move(float tx, float tz, int nFrameCount)
{
	// Ÿ������ ����
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
// Desc : ���� �̵��� �������� ��ġ�� ��Ŵ.
// ---------------------------------------------------------------------
BOOL CNkMob::Place(float tx, float tz, float dir, int nFrame)
{
	m_wx = tx;
	m_wz = tz;
	m_toDir = dir;

	// nFrame�� ��.. ������ ����.. ��..
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
// Desc : Ư�� ��ǥ �������� �̵�.
// ---------------------------------------------------------------------
BOOL CNkMob::Attack(float tx, float tz)
{
	// ���� ����� �Դµ�, ���� �̵����̸� �̵����� �������� ���� ��ġ��Ų��.
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

		// ��� �̺�Ʈ�� ���ؼ�.
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
	//npc�� ���� ����� �ΰ��϶� 2�� ���ݸ��
	if(pCMyApp->m_pUIMgr->m_bMultiAttack)
	{
		SetMotionState(MOB_KNOCK);
		pCMyApp->m_pUIMgr->m_bMultiAttack = FALSE;
	}
	else
		SetMotionState(MOB_ATTACK);


	if( m_nMobNum == 462 ) // Ȳ�ݺҷ�����...���� ��� �ΰ��� �����ϰ� ���.
	{
		SetMotionState(MOB_STAND); // ������ ���õ� ����� �����Ѵ�.(�̰������ MOB_KNOCK ����� �����ǵ� �۵��Ѵ�.

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
	// ���� ����� �Դµ�, ���� �̵����̸� �̵����� �������� ���� ��ġ��Ų��.
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
// Desc : nInc �� ������ ����.
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
	// �ϴÿ����� ������ ���� ���ư���~~!
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
				// ���� �׸��ڴ� �������� �׷��� �Ѵ�.
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
				// ���� ���� ���� ���� �غ� �Ÿ���.
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
				else if (m_nMobNum == MOB_MADSAIBOG || m_nMobNum == MOB_MADSAIBOG2||m_nMobNum == 347)  //��ȭ �ŵ���̺���
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
				//wan:2004-12 ��ũ�ε�
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
					// ��� ����Ʈ
					ef_sort.nType = FX_SHOOT;
					ef_sort.dwColor = 0xFFFFFFFF;
					ef_sort.nCurFrame = m_CurFrame - m_AttackFrame;
					ef_sort.bOneFrame = TRUE;
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					//--------------------------------------------------------
					// ��� ����
					ef_sort.nType = FX_LIGHTBALL;//FX_LWLIGHT;//FX_CHAKRAM;//FX_WAND_CHAIN;
					ef_sort.vPos.y=m_wy+20.0f;
					ef_sort.vTargetPos = D3DVECTOR(m_tx,m_wy+5.0f,m_tz);
					InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
					//--------------------------------------------------------
					//�´� ����Ʈ
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
					if (m_nMobNum == MOB_LIGHTENING_WORRIOR || m_nMobNum == 407)	// ������, ����Ʈ �극��
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

	if( m_NowMoveSkill ) // �˹�� �̵� ��ų ó��
	{
		float pastFrame = float(m_CurFrame - m_PrevFrame); // ����Ǿ�� �� �������� ���Ѵ�.
		float* fFrame = NULL;
		if( m_NowMoveSkill == SKILL_KNOCKBACK2 )
			fFrame = &m_fDash_Frame;
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
		m_wx += dx;
		m_wz += dz;

		if( m_NowMoveSkill == SKILL_KNOCKBACK2 ) // �˹�ó��
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
				SetMotionState( MOB_STAND );
				m_NowMoveSkill = 0;
			}
		}
	}

	// �ϴÿ��� �� �ִ� ����� �ٽ� ���� ���´�.
	m_nMotionState = nMotionState;

	m_PrevFrame = m_CurFrame;
	m_CurFrame += nInc;
	m_EffFrame += nInc;
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
	{
		m_nFlyFrame += nInc;

		if (rand() % 20 == 0) // ���� ��ġ�� ������ �ְ� �;...
			m_nFlyFrame++;

		while (m_nFlyFrame >= CHA_FLY_FRAME)
			m_nFlyFrame -= CHA_FLY_FRAME;
	}

	// Frame ����
	// m_CurFrame++;
	// �ѵ����� �� ���� ������ ������(���ִ»���)�� �����ϴ� �ڵ�
	// �� �׾��������� ������ ������ �������·� ���.
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
				// 4������ ¥��. ���������� ��.. 25
				// �ӽ÷� ���� �ڵ�.. ���� �ʿ�.
				EffectSort ef_sort;
				ef_sort.nType = FX_UNARMEDATTACKED;
				ef_sort.bOneFrame = TRUE;
				ef_sort.nCurFrame = dwTimeGap / 50;		// �� ȿ���� ��ü������ ���÷����� ������ ��ȣ�� �˷��ش�.
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

		if( Mob_NormalAttack_EffectFrame(524, EFFECT_BOSSRAID_KAI2_01) )	// [2/05/2008 �����] ī�̸��溸��2 1 ����
			return;

		if( Mob_NormalAttack_EffectFrame(525, EFFECT_BOSSRAID_KAI3_01) )	// [2/04/2008 �����] ī�̸��溸��3 ��� ����
			return ;

		if( Mob_NormalAttack_EffectFrame(517, EFFECT_BOSSRAID_AIDIA1_01) )
			return;

		if( Mob_NormalAttack_EffectFrame(519, EFFECT_BOSSRAID_AIDIA3_01) )	// [3/6/2008 �����] ���̵�ƺ���3 ��� ����
			return;

		if( Mob_NormalAttack_EffectFrame(518, EFFECT_BOSSRAID_AIDIA2_01) )
			return;

		if( Mob_NormalAttack_EffectFrame(740, EFFECT_BOSSRAID_HYBRI1_01) )
			return;
	}
}

// [2/21/2008 �����] �� �Ϲ� ���� ����Ʈ ���� �Լ�
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
// Desc : ������
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

	BOOL bMutantMaterial = FALSE; // ����Ʈ ġ�� �ں��Ʈ ����� ����. �ں��Ʈ ���� �����ش�. ///�±�����Ʈ�̺�Ʈ
	int nMutantAlphaOpt = 0; // ����Ʈ ġ�� ��������� ���� ���Ŀɼ� ����.

	dwCurtime = timeGetTime();

	// Vital �� ���ؾ� �ϴµ�, 1�ʰ� ������ �Ⱥ��ϸ�...
	if (m_Vital != m_AttackVital && dwCurtime > m_LastAttackVitalTime + 1000)
	{
		Attacked();
		dwCurtime = timeGetTime();
	}

	// �� �� ���ȴ�.
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
				case 218:	// ����
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, this, EFFECT_SKILL_MOB_FIRE);
					break;
				case MOB_SKELETON_MAGE: // �ذ� ������,
				case 393:	// �� �Ҽ���
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_FIRE_BALL);
					break;
				case MOB_DARK_SOUL: // 125 ��ũ�ҿ�
				case MOB_SPECTER:	// 417 ������
				//case 334: // ���� ��ũ�ҿ�
				case 542: // ��ȭ ��ũ �ҿ�
				case 596: // ��ȭ ������
				case 731:	// �����ڽ�����
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING);
					break;
				case MOB_SAILON_MAGICIAN:  // 137 ���Ϸ� ������
				case MOB_SAILON_MAGICIAN2: // 146 ���Ϸ� ������
				case MOB_SAILON_MAGICIAN3: // 164 ���Ϸ� ������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, m_MobSkill);
					break;
				case 179:
				case 180:
				case 181:	// ���̾� (Ŭ����,����,��ƿ) ��ȣž
				case 867:
				case 868:
					// ���̾� ��ƿ ��ȣž
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_GUARDTOWER_FIRE);
					break;
				case 182:
				case 183:
				case 184: // ����Ʈ�� (������,����,��ƿ) ��ȣŽ
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_GUARDTOWER_LIGHTBALL);
					break;
				case 185:
				case 186:
				case 187: // ���� (Ŭ����, ����,��ƿ) ��ȣž
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_GUARDTOWER_PEBBLESHOT);
					break;
				case MOB_KHAN_HAND: // 171
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_WORLDCUP_HAND);
					break;
				case MOB_KHAN_FOOT: // 172
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_WORLDCUP_FOOT);
					break;
				case MOB_SHILONMASTER: // 135 ���Ϸ� ������
				case 306:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_SHILONMASTER);
					break;
				case MOB_BABY_BUG: // 196 ���� ��
				case 536: // ��ȭ ���� ��
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_BABYWORM);
					break;
				case MOB_SHINING_JAKAL:	//1������ ���̴��ڰT -
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_MOB_SHINING_JAKAL);
					break;
				case 205: // ���ɴ� ����ũ ���Ӽ�2,���Ӽ�1 - �ֺ�
				case 202: // ��Ʈ ����
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_PEBBLE_SHOT);
					break;
				case 208: // ����ŷ
				case 250: // ������ ö�� �巡�� - �Ŵ�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_METEORITE);
					break;
				case 210: // �巹�� ���÷�
				case 249: // �ö��� ����ŷ -�ñ�
				case 562: // ��ȭ �巹�� ���÷�
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
				case 222: // � �巡��
				case 223: // ��ī�� �巡��
				case 265: // �г��� � �巡�� Dragon ���� �һձ�
				case 737: // ������
				case 738:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_DRAGON_FIRE);
					break;
				case 224: // ���� �巡��
				case 333: // ���� ���� �巡�� Dragon ���� �Ķ��һձ� ,��ȭ
				case 739:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_DRAGON_ICE);
					break;
				case 219: // ���̺� ������
				case 247: // ��Ʈ���̺�
				case 558: // ��ȭ ���̺� ������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_CHAIN);
					break;
				case 221: // ö�� �巡��
				case 264: // �г��� ö�� �巡��
				case 345: // ��ȭ ö�� �巡��
				case 405: // �����ĵ�
				case 560: // ��ȭ ö�� �巡��
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_FALL_STONE);
					break;
				case 133: // ĭ
				case 348: // ���� ĭ�� ��������
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
				case 258: // ����
				case 260: // ����ũ
				case 570: // ��ȭ ����ũ
				case 589: // ��ȭ ����ũ
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_SOULDUST);
					break;
				case 263: // ���÷��� ����Ʈ�� ����
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING);
					break;
				case 288: // �Ҽ��� ��
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_ARROW);
					break;
				case 290: // �� ��
				case 600: // ��ȭ �� ��
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_WAND_MAGICSWORD);
					break;
				case 297: // ���� ������
				case 350: // ��ȭ ���� ������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_FALLOFFSTONE);
					break;
				case 299: // Ŭ���� �ҿ�
				case 404: // Ŭ���� ������
				case 578: // ��ȭ Ŭ���� ������
				case 1019:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_STONERANCE2);
					break;
				case 300: // ���������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_DRILLSTONE);
					break;
				case 354: // ��ũ�� (������� ����Ʈ)
				case 555: // ��ȭ ��ũ��
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_STONE_LANCE);
					break;
				case 356: // ����� (����ŷ����Ʈ)
				case 557: // ��ȭ �����
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_RING_DEVIL);
					break;
				case 358: // ������ ÷�� �����ܰ� ����Ʈ
				case 551: // ��ȭ ������ ÷�� 
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
				case 360: // ���̽� ��ȣž
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_MOB_BABYWORM);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_ICE);
					break;
				case 361: // ���� ��ȣž
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING_BALL);
					break;
				case 1016:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_SHOCK_SPEAR);
					break;
				case 421: // nate 2005-11-22 : ��ȯ ĳũ�ͽ�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_CACTUS);
					break;
				case 423: // nate 2005-11-22 : ��ȯ �������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_DANDELION);
					break;
				case 424: // nate 2005-11-22 : ��ȯ ������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SPORE);
					break;
				case 775:
				case 5045:
				case 464: // ��ī�� ���� ���׳� Ÿ�� ����Ʈ.(�� ������ ���� ����) ///�δ�1
					pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, NULL, pMob, HIT_LEGKNOCK_ATTACK);
					break;
				default:

					if (m_nMobNum == 38||m_nMobNum == 331 || m_nMobNum == 545 || m_nMobNum == 1011)	// �� ���̾�Ʈ ���� ����, ��ȭ
						pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, NULL, pMob, HIT_WARGIANT_ATTACK);

					switch (m_MobAttackType)
					{
					case MOB_ATTACK_STARFLARE:
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_STAR_FLARE);
						break;
					case MOB_ATTACK_LIGHTNING:
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob, EFFECT_SKILL_LIGHTNING);
						break;
					case MOB_ATTACK_ACCUMULATE://����(���Ⱑ ������~)
						pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, pMob,EFFECT_SKILL_MOB_ACCUMULATE);
						break;
					case MOB_ATTACK_FIRE://�ҼӼ�(���� ��~)
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
				//case 334: //��ȭ ��ũ�ҿ�
				case 596:
				case 731:	// �����ڽ�����
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_LIGHTNING);
					break;
				case MOB_SAILON_MAGICIAN: // 137
				case MOB_SAILON_MAGICIAN2: // 146
				case MOB_SAILON_MAGICIAN3: // 164
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, m_MobSkill);
					break;
				case 179:
				case 180:
				case 181: // ���̾� ��ȣž
				case 867:
				case 868:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_GUARDTOWER_FIRE);
					break;
				case 182:
				case 183:
				case 184: // �����ú� ��ȣž
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_GUARDTOWER_LIGHTBALL);
					break;
				case 185:
				case 186:
				case 187: // ���� ��ȣž
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
				case 536: // ��������-
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_BABYWORM);
					break;
				case MOB_SHINING_JAKAL:	// 194 1������ ���̴��ڰT -
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_MOB_SHINING_JAKAL);
					break;
				case 205:
				case 202: // ���Ӽ�2,���Ӽ�1 - �ֺ�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_PEBBLE_SHOT);
					break;
				case 208:
				case 250: // ����ŷ,������ ö�� - �Ŵ�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_METEORITE);
					break;
				case 210:
				case 249:
				case 562: //���÷�,�ö��� ����ŷ -�ñ�
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
				case 265: // Dragon ���� �һձ�
				case 737: // ������
				case 738:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_DRAGON_FIRE);
					break;
				case 224:
				case 333: // Dragon ���� �Ķ��һձ� ,��ȭ
				case 739:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_DRAGON_ICE);
					break;
				case 219:
				case 247:
				case 558: // ���̺�,��Ʈ���̺�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_WAND_CHAIN);
					break;
				case 221:
				case 264:
				case 345:
				case 405:
				case 560: // ö�� �巡��,,,�����ĵ�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_FALL_STONE);
					break;
				case 133:
				case 348: // ĭ�� �������� , ��ȭĭ
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
				case 589: // ����ũ, ��ó ���� ȿ��
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_SOULDUST);
					break;
				case 263: // ���÷��� ����Ʈ�� ����
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
				case 350: // ��ȭ �������ڵ�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_FALLOFFSTONE);
					break;
				case 299:
				case 404:
				case 578: // Ŭ���� �ҿ�, �۷��� ������
				case 1019:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_STONERANCE2);
					break;
				case 300:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_DRILLSTONE);
					break;
				case 354:
				case 555: // ��ũ�� �����������Ʈ
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_STONE_LANCE);
					break;
				case 356:
				case 557: // ����� ����ŷ����Ʈ
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_RING_DEVIL);
					break;
				case 358:
				case 551: // ������ ÷�� �����ܰ� ����Ʈ
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
				case 360: // ���̽� ��ȣž
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_MOB_BABYWORM);
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_ICE);
					break;
				case 361: // ���� ��ȣž
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_LIGHTNING_BALL);
					break;
				case 1016:
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_SHOCK_SPEAR);
					break;
				case 416: // nate 2005-09-29 : ���Ʈ�� �߰����� - ���Ʈ
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_DURANT_ATTACK);
					break;
				case 421: // nate 2005-11-22 : ��ȯ ĳũ�ͽ�
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_CACTUS);
					break;
				case 423: // nate 2005-11-22 : ��ȯ �������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_DANDELION);
					break;
				case 424: // nate 2005-11-22 : ��ȯ ������
					pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, NULL, EFFECT_SPORE);
					break;
				case 438: // nate 2006-01-26 : ����Ʈ ��
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
				case 464: // ��ī�� ���� ���׳� Ÿ�� ����Ʈ.(�� ������ ���� ����) ///�δ�1
				case 775:
				case 5045:
					pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, pCha, pMob, HIT_LEGKNOCK_ATTACK);
					break;
				default:
					{
						if (m_nMobNum == 38||m_nMobNum == 331 || m_nMobNum == 545 || m_nMobNum == 1011)	// �� ���̾�Ʈ ���� ����
							pCMyApp->m_pUIMgr->InsertHitEffects(NULL, this, pCha, pMob, HIT_WARGIANT_ATTACK);

						switch (m_MobAttackType)
						{
						case MOB_ATTACK_STARFLARE:
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_STAR_FLARE);
							break;
						case MOB_ATTACK_LIGHTNING:
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_LIGHTNING);
							break;
						///2003 ������ �ҼӼ� ���� ȿ��
						case MOB_ATTACK_ACCUMULATE://����(���Ⱑ ������~)
							pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob,EFFECT_SKILL_MOB_ACCUMULATE);
							break;
						case MOB_ATTACK_FIRE://�ҼӼ�(���� ��~)
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
	// nate 2006-01-24 : Ÿ���� ���� ���� ����Ʈ ó��
	if( m_byInsertEffect )
	{
		if( m_AttackFrame <= m_CurFrame && m_nMotionState == MOB_ATTACK )	// �������̰� ���� �����ӿ� �����ϸ�
		{
			if( m_nMobNum == 432 && (m_byInsertEffect & INSERT_EFFECT_FIRE) ) // nate 2006-01-20 : ���庮
			{
				pCMyApp->m_pUIMgr->InsertMagic(NULL, this, NULL, NULL, EFFECT_FLAME);//ȭ������
				m_byInsertEffect &= ~INSERT_EFFECT_FIRE;	// ����Ʈ�� �ѹ���( ~ : �÷��� ���� ���� )
			}
		}
	}

	if( m_nMobNum == 433 ) // ������ �� Ʈ�� ���� ����ó�� ///�δ�1 �� �߰�
	{
		m_dir = m_toDir; // ���� ���� ������ ������ ���� �ٷ� ����������.
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

		if (m_bMoving == FALSE)		// ������Ʈ�� �ٲپ� �־�� �Ѵ�.
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
			// �� ���̾�Ʈ ���� ȿ��
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

	///�δ�1
	if( m_RegeningType == 2 ) // ���ӿ��� Ƣ����� �ִ� ���̸�...
	{
		float temp = pCMyApp->GetLandPtr()->GetHFHeight(m_wx,m_wz);
		if( m_wy < pCMyApp->GetLandPtr()->GetHFHeight(m_wx,m_wz) ) // ���ӿ� �����ִٸ�...
		{
			m_fDash_Frame += 0.4f;
			m_wy += (m_CurFrame-m_PrevFrame)*m_fDash_Frame; // ������ �������Ϳ� ����ؼ�...�����ӵ��� �÷��ش�.
			m_CurFrame = 0; // �������� �ʵ��� ������Ų��.

			if( m_fDash_Frame < 1.0f ) // �ʹݺο��� ���Ⱑ �ϵ���...
			{
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_RUNSMOKE;
				ef_sort.vPos = D3DVECTOR(m_wx, pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz)-5.0f, m_wz); // �ణ ���缭 ����.
				InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}

		}
		else
		{
			m_RegeningType = 0; // ������ ������ �Ϲ� ��ƾ���� ����.
			m_fDash_Frame = 0.0f; // �˹���� ������ �ӽ÷� �����Ƿ� �ʱ�ȭ ���ش�.
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

	if(m_MobClassType == 9 ) // ����� �����̰�. ///�����޺�
	{
		if( m_RunSmokeFrame ) // �� �༮�� ���õ� ����ν����� ���� ���� ���� ����.
			m_wy -= m_RunSmokeFrame;
	}

	// ��� �̺�Ʈ�� ���ؼ�.
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

	if( m_nMobNum == MOB_DARK_SOUL||m_nMobNum == 334 )		// ��ũ�ҿ��� ������ ���ƴٴѴ�. ����~~//��ȭ
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
	else if( m_nMobNum==MOB_AIREN )			//0401 ���̷� ��ȯ�� �յ�~
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

	// SKY �������� �ϴ� ������ ������Ʈ���带 ź��!
	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY)
	{
		BOOL bSky = !(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz));
		if (bSky)
			m_wy += g_ChaFlyHeight[m_nFlyFrame];
	}

	if (IsDead() && pCMyApp->m_nCurWorldIdx == WORLD_SKY &&
			!(pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_wx, m_wz)))
		m_wy -= m_DeadFrame * (700.0f / 100); // 100 Frame(2��) ���� 1000.0f�� ���� �ɴ´�.
	else if (m_DeadFrame > 0 && m_nMobNum != 438 && m_nMobNum != 465 && m_nMobNum != 466 )	// 438 : ����Ʈ ���� ������� ���� // ��ī�� �δ��� ���鵵 �Ȱ���ʰ�.(�����°� �״� �ɷ� ���) ///�δ�1
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
			// �����̾�Ʈ, ��ȭ ��������ü, �¿�
			m_wy += m_DeadFrame * (30.0f / 100);	// ������
		}
		else
		{
			if (m_DeadFrame > 60)
			{
				/*				if (m_DropItemIndex >= 0)	// nate 2006-01-24 : ���� �̻��
								{
									// �̰� ���������... �Ѷ� ������ ��ȣ�� �� �Ѱܶ�! ������ 0 �̴�.
									pCMyApp->m_pUIMgr->AddItem(m_DropItemIndex, m_DropItemVnum, m_DropItemX, m_DropItemZ, m_DropItemDir, 0, 0, 0); // 05-10-10 �ɼ��÷��� 2 �߰� (����) ///�Ź���ȭ�ý���
									m_DropItemIndex = -1;
								}
				*/				m_wy -= (m_DeadFrame-60) * (6.0f / 40); // 60 Frame(3��) �ĺ��� 40 Frame(2��) ���� 6.0f�� ���� �ɴ´�.
			}
		}
	}

	// ���� ������ ���
	FrameMove(frame_count, bVisible);

	if (bVisible == FALSE)
		return;

	if (m_LastChatTime + SHOW_MOBCHATPOPUP_TIME > dwCurtime && bVisible)
		m_bShowMobChatPopup = TRUE;
	else
		m_bShowMobChatPopup = FALSE;

	// ���� Ʈ������. - ĳ���͸� �׸��� ����.
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
		// nate 2005-08-22 : ���� ������ ����
		if( pCMyApp->m_nCurWorldIdx == WORLD_DMITRON )
		{
			if( m_nMobNum == 416
				|| m_nMobNum == MOB_SPECTER
				|| ( m_nMobNum > 391 && m_nMobNum < 413 ) )
			{
				float fScale = 1.5f;
				if( m_nMobNum == 401 || m_nMobNum == 402 )	// �����̴�, ���ν����̴�
					fScale = 2.0f;
				else if( m_nMobNum == 405 || m_nMobNum == 406 )	// ö���巡��, �ö��� ����ŷ
					fScale = 1.0f;
				else if( m_nMobNum == 416 || m_nMobNum == MOB_SPECTER )	// nate 2005-10-25 : ������ �߰�
					fScale = 1.7f;

				D3DMATRIX matS;
				D3DUtil_SetScaleMatrix(matS, fScale, fScale, fScale);
				D3DMath_MatrixMultiply(matWorld, matS, matWorld);
			}
		}
	*/
	if( m_Mutant > 10000 ) // Ư�� ����Ʈ �Ӽ� �߰�.(Ȯ��,���� ������)   ///�±�����Ʈ�̺�Ʈ
	{
		D3DMATRIX matS;

//		int PlusScale = (m_Mutant%1000) / 100; // ���� �ڸ��� Ȯ�� �����Ϸ� ���.
//		int MinusScale = (m_Mutant%10000) / 1000; // õ�� �ڸ��� ��� �����Ϸ� ���.
		int Color = m_Mutant % 100; // ���� ���ڸ��� �÷� �÷��׷� ���.
		int TransFlag = (m_Mutant%100000) / 10000; // ���� �ڸ��� 2�̻��̸� ���Ŀɼ�.

		if( TransFlag >= 2 ) // ���Ŀɼ��� ���� ���̸�..
		{
			nMutantAlphaOpt = TransFlag;
		}
		/*
				if( PlusScale ) // Ȯ�� ġ�� ������...
				{
					D3DUtil_SetScaleMatrix( matS, (float)PlusScale, (float)PlusScale, (float)PlusScale ); // �������� �����ɰ� ������ ����
					D3DMath_MatrixMultiply(matWorld, matS, matWorld);
				}

				if( MinusScale ) // Ȯ�� ġ�� ������...
				{
					D3DUtil_SetScaleMatrix( matS, 1.0f/(float)MinusScale, 1.0f/(float)MinusScale, 1.0f/(float)MinusScale ); // �������� �����ɰ� ������ ����
					D3DMath_MatrixMultiply( matWorld, matS, matWorld );
				}
		*/
		if( Color ) // ���� ���� �÷��װ� ������...
		{
			D3DMATERIAL7 mtrl;

			switch( Color )
			{
			case 1: // ����
				D3DUtil_InitMaterial( mtrl, 1.0f, 0.2f, 0.2f );
				break;
			case 2: // �Ķ�
				D3DUtil_InitMaterial( mtrl, 0.2f, 0.2f, 1.0f );
				break;
			case 3: // ����
//			    D3DUtil_InitMaterial( mtrl, 1.0f, 0.5f, 0.5f );
				D3DUtil_InitMaterial( mtrl, 1.0f, 0.2f, 1.0f );
				break;
			case 4: // ���
				D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 0.2f );
				break;
			case 5: // �ʷ�
				D3DUtil_InitMaterial( mtrl, 0.0f, 1.0f, 0.0f );
				break;
			case 6: // ����(��ħ�ϰ�..)
				D3DUtil_InitMaterial( mtrl, 0.13f, 0.1f, 0.5f );
//			    D3DUtil_InitMaterial( mtrl, 0.1f, 0.1f, 0.15f );
				break;
			default:
				D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
				break;
			}
			bMutantMaterial = TRUE; // ���͸����� �����ֱ� ���� ���.
			GET_D3DDEVICE()->SetMaterial( &mtrl );
		}

	}
	/*
		//---------------------------------------------------------------------------
		//-- �ĸ��� ���� Event
		//-- 2004.03.29
		//-- Lyul / �����Ϸ�
		//-- �� ������ �������.
		//-- �ʱ� �������� 0.5f ���� �ױ� ������ 1.5f
		//-- 20�� vital �� ������. 0.05 �� multiply �ϸ� OK
		if (m_nMobNum == 323) //-- �ĸ��� ����
		{
			float grade = m_Vital * 0.01f;
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, 1.5f - grade, 1.5f - grade, 1.5f - grade);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		//---------------------------------------------------------------------------
		//�߰� ��ų �߰�
		//�����Ǽ� �п����̸�
		if(m_FissionType>0)
		{
			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, g_SummonScale+1.0f, g_SummonScale+1.0f, g_SummonScale+1.0f);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
		//---------------------------------------------------------------------------
		//��ȭ�� ũ�� ����
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
	float ExceptionScale = GetMobExceptioncale( this ); // �������� Ŭ���̾�Ʈ �������� ����Ǵ� ��� ������ ���� �������ִ� �Լ�.
	if( ExceptionScale != 1.0f ) // 1.0f�̸� ���� ���� �ʿ������ ��ŵ.
	{
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix( matS, ExceptionScale, ExceptionScale, ExceptionScale );
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}

//--------------------------------------------------------------------------
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	// ���°� ����.
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

	// �ֺ��� ����
	DWORD dwAmbient;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	// ���� �ڽ��� ������� ���̶�� �ֺ� ������ �������ϰ� ///�������
	if( g_pRoh && g_pRoh->m_bNowFreePVP )
	{
		// �׳� �������ϰ� �����̸� ����ϰ� �ڿ� ��ƾ�� ��� ���ý�Ų��.
		// ��, ������ ���� ó�� ���� ����ó��, ����Ʈ �߰����� �� ���Ŀ� �ִ� ������
		// ������� �߿��� �Ⱥ��̰� �ȴ�.
		// ����� ��ű���۵� �Ⱦ��� ������ �ڵ����� ���� ���� ���ϰ� ���Ѵ�.
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
	if( m_bPoisoning )	// nate 2005-11-14 : ������ - npc�� ������ ������ SetAmbient
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00008833);

	if (m_nMobNum == MOB_SAD_SHADOW || m_nMobNum == MOB_SAD_SHADOW_2 )   // ���� �׸��ڴ� �������� �׷��� �Ѵ�.
	{
		if(m_MobClassType==4)	 //��ȥ -�������ϰ�...
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		}
		m_pCapSyong->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, NULL, m_CurFrame);
	}
	else if (m_nMobNum == MOB_DARK_SOUL) // ��ũ�ҿ���...
		m_pCapSyong->Render(GET_D3DDEVICE());
	else if (m_nMobNum == MOB_SPECTER)	// nate 2005-10-25 : ������ �߰�
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00008833);
		m_pCapSyong->Render(GET_D3DDEVICE());
	}
	else if (g_pMobLolo && m_nMobNum == MOB_LIGHT_BOOM) // ����ź�̿�...
		g_pMobLolo->Render(GET_D3DDEVICE(), m_CurFrame);
	////2003 �������϶��� �����Ҷ��� ��Ÿ����..
	else if(m_MobClassType==1)
	{
		if(m_bAttack	//���ݽ� �Ǵ� ���� ���̴� ���� ���������� ���̰� ��.
				|| (g_pRoh->m_Wearing[WEARING_BRACELET] && g_pRoh->m_Wearing[WEARING_BRACELET]->m_Vnum == 829)
				|| (g_pRoh->m_Wearing[WEARING_BRACELET] && g_pRoh->m_Wearing[WEARING_BRACELET]->m_Vnum == 841) //1202 ��������⵵ ���̴�ó��...
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
	else if( m_nMobNum == 38||m_nMobNum == 331||m_nMobNum==333||m_nMobNum==334||m_nMobNum==348// �� ���̾�Ʈ ,��ȭ ��ũ�ҿﵵ �ǰ� ������ ���ް�...
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
		//ȭ��Ʈȥ���� �������� �� 10%���� ����������

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
	else if( m_nMobNum == 463 ) // �±� 3�ֳ� ����ɟ�, ũ�Ⱑ Ŀ�� ����Ʈ ������ ���̻ڹǷ� ����Ʈ�� ���� ��������.
	{
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

		m_pCapSyong->Render(GET_D3DDEVICE());

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	}
	else if( nMutantAlphaOpt ) // ����Ʈ �Ӽ��� �������� ���̸�....
	{
		if( nMutantAlphaOpt == 2 ) // ������
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTCOLOR);
			m_pCapSyong->Render(GET_D3DDEVICE());
		}
		else if( nMutantAlphaOpt == 3 ) // �±� �ҷ����� ���� ����
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

	// [3/30/2007 Theodoric] ����� �� ǥ�� :: ĳ���Ͱ� �������� �� ���� ����
	if (bMouseOn || pCMyApp->m_pHeroAttGoMob == this )
	{
		if( g_dwClientCountry == CTRY_JPN )
		{
			//  [9/30/2008 parkmj] �Ϻ� ��������, �� ������ - ���콺 ������ ���󺯰� ����
			if( this->m_nMobNum == 613 || this->m_nMobNum == 646 )
				return;

			// connie [2009/3/9] -  �Ϻ� 6�ֳ��̺�Ʈ - ���콺 ������ ���󺯰� ����
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
		else if(pCMyApp->m_pHeroAttGoMob == this ) // �������� ���̶�� ������ �ٲ۴�.
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00AA6666);
		}
		else // ���콺�� ������ ��.
		{
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00888888);
		}

		// ��ٿ� �ĸ��� ������ ��� ������ ���� ������ ������ ���� �ʴ´�.
		if (m_nMobNum != 25
				&& m_nMobNum != 323
				&& m_nMobNum != 479
				&& !(m_nMobNum >= 420 && m_nMobNum <= 424) )
		{
			// ��ٴ� �� �׷���...
			if (m_nMobNum == MOB_LIGHT_BOOM)
			{
				// �� ��ź�� �ѷ�...
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

		// ���콺 on�϶� ������ �ܿ��� ���� ������
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
				// ���콺 on�϶� �������� ����������.
				GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00774545);

			// ��ٿ� �ĸ��� ������ ��� ������ ���� ������ ������ ���� �ʴ´�.
			if (m_nMobNum != 25
				&& m_nMobNum != 323
				&& !(m_nMobNum >= 420 && m_nMobNum <= 424) )
			{ // ��ٴ� �� �׷���...
				if (m_nMobNum == MOB_LIGHT_BOOM)
				{ // �� ��ź�� �ѷ�...
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

			// ���콺 on�϶� ������ �ܿ��� ���� ������
			GET_D3DDEVICE()->LightEnable(0, lightEnable);
			GET_D3DDEVICE()->LightEnable(1, lightEnable2);
		}
	*/
	// nate 2005-05-23 : ��Ʈ���� ������ ��ȯ������ üũ
	if( m_MobAttributeType > 0		// m_MobAttributeType == 100�� �ݶ��̺�Ʈ ��
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
		//���� �迭 -������
		if(m_bAttack)	//���ݽ� �Ǵ� ���� ���̴� ���� ���������� ���̰� ��.
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
		//�ı�  -�Ķ��� 	 ���߿� ����..^^;
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
		//���� -���������� ǥ��
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		DWORD alphaRef;
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000010);

		m_pCapSyong->Render(GET_D3DDEVICE(), g_MobEffectTexture[ MOB_EFF_RED ], FALSE, FALSE, NULL,0/*m_EffFrame*/);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHAREF  , alphaRef);
	}
	// �� ������ Mob �� Render(��ȯ�� Render)
	// wan:2004-12 : m_MobClassType=10 - ������� ���ͳ���Ʈ ��ȯ��
	else if(m_MobClassType==4||m_MobClassType==10)
	{
		// ���� �׷��� �� Mob�� ��ȯ�� ��� Vital �� Display
		if( pCMyApp->m_pUIMgr->m_pSummonMob[0] == this)	// (0)�Ϲ� ��ȯ���� �������� ����
			pCMyApp->DisplaySummonVital(this);
		if(m_nMobNum==352 && m_Vital <= 10 )
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xffff0000);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

		m_pCapSyong->Render(GET_D3DDEVICE());
	}
	//m_MobClassType = 9 : ����� ����
	else if(m_MobClassType == 9 )
	{
		pCMyApp->DisplayDivineVital(this);
		g_pShadowRect->SetTileCoord(D3DVECTOR(m_wx, m_wy, m_wz), 100, FALSE);

		if( !m_RunSmokeFrame ) // ����转��(����转�� ���� ���� Ÿ�ֺ̹���)�� ����Ʈ �����ϵ���.. ///�����޺�)
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
		else // ���� �������� ó��. ///�����޺�
		{
			m_RunSmokeFrame += 2; // ����ν����� ���� ���� ��.
			if( m_RunSmokeFrame > 8 )
			{
				m_RunSmokeFrame = 8;
			}
		}
	}

	if( pCMyApp->m_pUIMgr->m_pSummonMob[1] == this)	// �ڽ��� ���� �ö�� �������� ���̵���... ///07-04-10 ���� ��ũ �����־���.�Բ� ����.
		pCMyApp->DisplaySummonVital(this);

	///2003 ���� �����ֹ�(��ȭ,����)�� �ɷ����� ȿ���� ���⿡ ����
	if(m_CancelState)	//���°� �ɷ��ִٸ�
	{
		if( m_ShowConditionEffect == FALSE )	// ȿ���� �׷����� �ִ���...�˻�...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, NULL, this, EFFECT_CONDITION);
			m_ShowConditionEffect = TRUE;
		}
	}

	if(m_CancelState2)	//���°� �ɷ��ִٸ�
	{
		if( m_ShowConditionEffect2 == FALSE )	// ȿ���� �׷����� �ִ���...�˻�...
		{
			pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, NULL, this, EFFECT_CONDITION2);
			m_ShowConditionEffect2 = TRUE;
		}
	}

	//2003 å����,���� ȿ��.
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

	// nate 2004 - 8 : ���� ���̿��� ���� ���� ����
	// ������ 7 : �ϴû�, 11 : ��ũ��
	if ( m_Mutant != 5 &&
			( ( m_Mutant > 0 && m_Mutant < 8 )
			  || ( m_nMobNum > 178 && m_nMobNum < 188 ) ) )
	{
		// �������̶��~~
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

	if( bMutantMaterial ) // ���͸����� �̿��� ����Ʈ ������ ǥ���߾��ٸ�... ///�±�����Ʈ�̺�Ʈ
	{
		D3DMATERIAL7 mtrl;
		D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f ); // ���͸����� �����ش�.
		GET_D3DDEVICE()->SetMaterial( &mtrl );
	}

	//////////////////////////////////////////////////////////////////////////
	// pick buffer ����.
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
//		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///��ŷ���׼���. DWORD ������ ��ȯ�Ѱɾ������� �׳� WORD���� �ٷ� �������� �ִ´�. �̷��� �ص� 32767�� ���� ���� ��ŷ�� ������ ��. ��ȯ�� ���� ���̴� ��찡 �ִµ��ؼ� �̷��� ������. (07-01-02 ����)
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

	if( m_nMobNum == MOB_AIREN )   // �������� ����? ��Ŀ�� ��ȯ�� ȿ��
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


	if( m_nMobNum == 317 || m_nMobNum == 318 ) // �Ҿƹ���, �ҸӴϴ� �ؿ� ���ڸ��� ��Ƶ帮��. ///���ڸ�
	{
		// �Ѵ� �������� ���� ���� �ϳ����� ��� ��ġ �����ϴ� ������� ����.
		if( g_pMattingLolo && GET_D3DDEVICE() )
			g_pMattingLolo->Render( GET_D3DDEVICE() );
	}

}

void CNkMob::SetChaCondition(int Condition, int cancel)
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
// Crash_Miss â �׸���
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

void CNkMob::RenderCrash_Miss()
{
	m_nElapTime = ( timeGetTime() - m_dwElapTime ) / 5;

	// Ŀ���� ũ�� ����
	if( m_nElapTime > 100 ) // 05-02-01 / 50���� 100���� ��ġ �ø�( ������:�̿��� )
	{
		m_byShowCrash_Miss = 0;
		m_dwElapTime = 0;
		return;
	}
	// ���� ũ�� ����
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

	float Y_Plus = 23.0f; // ĳ������ ������� ���� �������� ����ġ.

	if( nEventNum == 4 ) // �ȳ��� BP����. �ȳ��� Ű�� �����Ƿ� ����ġ�� �����ش�.  ( 05-08-09 by ���� )
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

	//-- ������� ��ȯ ���

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
	case 3:	// nate 2005-03-24 : Matrix - ���� NPC �Ӹ��� ���̵� ���
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SHOP_NPC ] ), 1 );
		break;
	case 4:  // BP ����. (05-08-09 by ����)
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_BP_SHOP_NPC ] ), 1 );
		break;
	case 5: // mungmae 05/12/12 �� ����Ʈ
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_YAN_EVENT ] ), 1 );
		break;
	case 6:
		m_NPC_Event->Draw( vSrc.x, vSrc.y, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_LP_SHOP_NPC ] ), 1 );
		break;
	}	
}

// �������� Crash / Miss �� ���� ���� �Լ�
// NPC Event �� ���⼭ �ʱ�ȭ
// Lyul / ������
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
		m_NPC_Event->LoadRes( width, height ); //-- Lyul �߰�
	}
}
//=====================================================================
//���̵�� �����Ǽ� ���� Ŀ���� ��ȯ��Ÿ�� ����
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
	// Ÿ������ ����
	m_tx = tx;
	m_tz = tz;
	float dist_x = m_tx - m_wx;
	float dist_z = m_tz - m_wz;

	// ĳ���� ������ ���Ѵ�.
	if( nSkillIndex != SKILL_KNOCKBACK && nSkillIndex != SKILL_KNOCKBACK2 ) // �˹��� ���� ��ȯ ���ϰ�...
	{
		m_toDir = (float) atan2(dist_z, dist_x);
	}

	float scalar= sqrt(pow(dist_x,2) + pow(dist_z,2));
	float sc_rate = 0.0f;

	float* fFrame = NULL;
	if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // �˹� ///��
	{
		sc_rate = scalar / 13.0f;
		fFrame = &m_fDash_Frame;
	}
	else
		return;

	m_dx = dist_x / sc_rate;    // �̵��� ũ�⸦ ���Ѵ�
	m_dz = dist_z / sc_rate;	// �̵��� ũ�⸦ ���Ѵ�

	float dist = dist_x != 0.0f ? dist_x : dist_z;
	float distMod = m_dx != 0.0f ? m_dx : m_dz;

	if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // �˹� ///��
		*fFrame = dist / distMod - 0.2f;

	// ���� ����
	if( nSkillIndex == SKILL_KNOCKBACK || nSkillIndex == SKILL_KNOCKBACK2 ) // �˹� ///��
		SetMotionState( MOB_KNOCK ); //�˹��� ��ä�� �и���...

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

		m_pMobChatPopup->Draw( vScr.x, vScr.y);
	}
}

void CNkMob::SetMobChatMsg(char *chat_msg)
{
	CMultiText multiText;
	multiText.AddString( GetMobName(), RGB(252, 180, 180) );
	multiText.AddStringMulti(chat_msg, 15, TRUE, RGB(255, 251, 115));
	m_pMobChatPopup->PutString(&multiText, TRUE);
	m_pMobChatPopup->SetIndex(0);		// ���� �������� ê �ڽ�
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

float GetMobExceptioncale( CNkMob *DestMob ) // �������� Ŭ���̾�Ʈ �������� ����Ǵ� ��� ������ ���� �������ִ� �Լ�.
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
			if( DestMob->m_nMobNum == 401 || DestMob->m_nMobNum == 402 )	// �����̴�, ���ν����̴�
				fScale = 2.0f;
			else if( DestMob->m_nMobNum == 405 || DestMob->m_nMobNum == 406 )	// ö���巡��, �ö��� ����ŷ
				fScale = 1.0f;
			else if( DestMob->m_nMobNum == 416 || DestMob->m_nMobNum == MOB_SPECTER )	// nate 2005-10-25 : ������ �߰�
				fScale = 1.7f;

			return fScale;
			*/
		}
	}

	if( DestMob->m_Mutant > 10000 ) // Ư�� ����Ʈ �Ӽ� �߰�.(Ȯ��,���� ������)   ///�±�����Ʈ�̺�Ʈ
	{
		D3DMATRIX matS;

		int PlusScale = (DestMob->m_Mutant%1000) / 100; // ���� �ڸ��� Ȯ�� �����Ϸ� ���.
		int MinusScale = (DestMob->m_Mutant%10000) / 1000; // õ�� �ڸ��� ��� �����Ϸ� ���.

		if( PlusScale ) // Ȯ�� ġ�� ������...
		{
			return ((float)PlusScale);
		}

		if( MinusScale ) // Ȯ�� ġ�� ������...
		{
			return (1.0f/(float)MinusScale);
		}
	}

	//---------------------------------------------------------------------------
	//-- �ĸ��� ���� Event
	//-- 2004.03.29
	//-- Lyul / �����Ϸ�
	//-- �� ������ �������.
	//-- �ʱ� �������� 0.5f ���� �ױ� ������ 1.5f
	//-- 20�� vital �� ������. 0.05 �� multiply �ϸ� OK
	if( DestMob->m_nMobNum == 323 || DestMob->m_nMobNum == 479) //-- �ĸ��� ����
	{
		float grade = DestMob->m_Vital * 0.01f;

		return 1.5f-grade;
	}
	//---------------------------------------------------------------------------
	//�߰� ��ų �߰�
	//�����Ǽ� �п����̸�
	if( DestMob->m_FissionType > 0 )
	{
		return (g_SummonScale+1.0f);
	}
	//---------------------------------------------------------------------------
	//��ȭ�� ũ�� ����
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
		case 535: // ��ȭ �̸� �ī��Ʈ
		case 575: // ��ȭ ���̿� Ŀ��
			return 1.5f;

		case 577: // ��ȭ ���ν����̴�
		case 597: // ��ȭ ���� ������
		case 601: // ��ȭ ���̰���
			return 2.0f;

		}
	}

	float fResult = MobScaleData::GetInstance()->GetScaleValue(DestMob->m_nMobNum);

	return fResult;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������� NPC Ŭ����( NKMob Ŭ������ ��ӹ޾ƾ���. ��ƾ�� ���� ������, ��� �������� �����پ������� ����Ѵ�. ) ///��������
CUserShopNpc::CUserShopNpc() : CNkMob()
{
	m_MasterIndex = 0; // ���� ������ �ε���. ��Ŷ�� ������ �̰ɷ� ������ �Ѵ�.
	ZeroMemory( m_strMasterName, sizeof(m_strMasterName) ); // ���� ���� �̸�.
	ZeroMemory( m_strUserShopMent, sizeof(m_strUserShopMent) ); // ���� ��Ʈ ��Ʈ��. �Ӹ����� ǥ��������Ѵ�.

	if( m_pCrash_MissPopup[0] ) // m_pCrash_MissPopup[0]�� ��Ʈ �˾���¿����� �������ؼ� �ణ�� �ʱ�ȭ�� �� �ʿ��ϴ�.(��Ʈ���õ�)
	{
		m_pCrash_MissPopup[0]->Init(12,AlphaPopUp_Type__Char);
		m_pCrash_MissPopup[0]->SetRectSoft(TRUE);
	}

	m_bRemoveHair = FALSE; // �Ӹ����� �ɼ� üũ
	m_bRemoveFace = FALSE; // �󱼾��� �ɼ� üũ
	m_bRemoveBody = FALSE; // ������� �ɼ� üũ
	m_bRemoveArms = FALSE; // �Ⱦ��� �ɼ� üũ
	m_bRemoveLegs = FALSE; // �ٸ����� �ɼ� üũ
	m_bRemoveFoot = FALSE; // �߾��� �ɼ� üũ
}

CUserShopNpc::CUserShopNpc( CClrAlloc *pClrAlloc ):CNkMob( pClrAlloc ) // �θ� Ŭ������ �����ڸ� �̷��� �ؾ� ����� ȣ��Ǵ���.
{
	m_MasterIndex = 0; // ���� ������ �ε���. ��Ŷ�� ������ �̰ɷ� ������ �Ѵ�.
	ZeroMemory( m_strMasterName, sizeof(m_strMasterName) ); // ���� ���� �̸�.
	ZeroMemory( m_strUserShopMent, sizeof(m_strUserShopMent) ); // ���� ��Ʈ ��Ʈ��. �Ӹ����� ǥ��������Ѵ�.

	if( m_pCrash_MissPopup[0] ) // m_pCrash_MissPopup[0]�� ��Ʈ �˾���¿����� �������ؼ� �ణ�� �ʱ�ȭ�� �� �ʿ��ϴ�.(��Ʈ���õ�)
	{
		m_pCrash_MissPopup[0]->Init(12,AlphaPopUp_Type__Char);
		m_pCrash_MissPopup[0]->SetRectSoft(TRUE);
	}

	m_bRemoveHair = FALSE; // �Ӹ����� �ɼ� üũ
	m_bRemoveFace = FALSE; // �󱼾��� �ɼ� üũ
	m_bRemoveBody = FALSE; // ������� �ɼ� üũ
	m_bRemoveArms = FALSE; // �Ⱦ��� �ɼ� üũ
	m_bRemoveLegs = FALSE; // �ٸ����� �ɼ� üũ
	m_bRemoveFoot = FALSE; // �߾��� �ɼ� üũ
}

CUserShopNpc::~CUserShopNpc()
{
	// �θ� Ŭ������ �Ҹ��ڰ� ȣ��ȴ�.
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

	// �ѵ����� �� ���� ������ ������(���ִ»���)�� �����ϴ� �ڵ�
	// �� �׾��������� ������ ������ �������·� ���.
	if( m_CurFrame >= m_pBiped[m_nMotionState]->m_AnimCount )
	{
		m_CurFrame = 0;

		m_nMotionState = rand() % 5; // 5�� ������ �������� �����ش�.
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

	// ���°� ����.
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

	// �ֺ��� ����
	DWORD dwAmbient;
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	switch( m_nMobNum ) // Npc���� �����Ͽ� ����Ѵ�.
	{
	case 441: // �Ƹ�����Ʈ ��. // �� �༮�� ������Ʈ�� 9���� ���������ִ�.

		if( !m_bRemoveBody )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 0 ); // ����(��)

		if( !m_bRemoveBody ) // ���̶� ���� �پ��ִ�.
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 1 ); // ��Ʈ

		if( !m_bRemoveFoot )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 3 ); // �Ź�

		if( !m_bRemoveArms )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 4 ); // ��

		if( !m_bRemoveHair ) // �Ӹ�ī�� ���� �ɼ��� �������� ���.
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 2 ); // �Ӹ�ī��

		if( !m_bRemoveFace ) // �� ���� �ɼ��� �������� ���.
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 5 ); // ��

		if( !m_bRemoveLegs )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 6 ); // �ٸ�

		if( !m_bRemoveArms )
			m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 7 ); // ��

		m_pCapSyong->RenderUnit( GET_D3DDEVICE(), 8 ); // å

		break;

	default: // ���� ������ ��ϵǾ����� ������...
		m_pCapSyong->Render(GET_D3DDEVICE()); // ��¥ ���.
		break;
	}

	if( m_WearList.size() != 0 ) // ������ ��� �ִٸ�... // �� ����Ʈ�� ������ ���� Vnum�� �޾Ƽ� ������ �迭�� ProtoNum(index)���� �ٲ� ���� ������ �ִ�.
	{
		CCapsyong *TempWearCsy = NULL;

		for( vector<int>::iterator prWear = m_WearList.begin() ; prWear != m_WearList.end() ; ++prWear )
		{
			TempWearCsy = GET_CSYC((*prWear), 1);

			if( TempWearCsy )
			{
				TempWearCsy->Animate( m_pBiped[m_nMotionState], m_CurFrame );	 // �ε��� �ϰ� ���⼭ �ִϸ���Ʈ.
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

		// ���콺 on�϶� ������ �ܿ��� ���� ������
		GET_D3DDEVICE()->LightEnable(0, lightEnable);
		GET_D3DDEVICE()->LightEnable(1, lightEnable2);
	}

	// ��Ʈ �˾� �׸���. ( m_pCrash_MissPopup[0]�� ����Ѵ�. )
	if( bVisible && (g_pNk2DFrame->GetControlBottom()->m_bSalesView||bMouseOn) ) // ���ä���� ���������� �Ⱥ��̰�... ///���������� �߰�
	{
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

		m_pCrash_MissPopup[0]->Draw( vScr.x, vScr.y, 3 );
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	//////////////////////////////////////////////////////////////////////////
	// pick buffer ����.
	if( GET_D3DPICK() && bDontPick == FALSE )
	{
		GET_D3DPICK()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
//		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///��ŷ���׼���. DWORD ������ ��ȯ�Ѱɾ������� �׳� WORD���� �ٷ� �������� �ִ´�. �̷��� �ص� 32767�� ���� ���� ��ŷ�� ������ ��. ��ȯ�� ���� ���̴� ��찡 �ִµ��ؼ� �̷��� ������. (07-01-02 ����)
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


// �������� NPC Ŭ����( NKMob Ŭ������ ��ӹ޾ƾ���. ��ƾ�� ���� ������, ��� �������� �����پ������� ����Ѵ�. )
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void NpcFileCollect( const char *FileName, const char *SrcPath, const char *MovePath ) // Ư�� Npc Dta ���� ����ϴ� ���ϵ��� ���� ����ִ� �Լ�. ///���������� ���� Ŭ���̾�Ʈ ������ �߰�.
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

	// ���������� �о����.
	for (int i = 0; i < npc_count; i++)
	{
		pfread(&TempInt, sizeof(int), 1, fp);		// �� ��ȣ
		pfread(&TempInt, sizeof(int), 1, fp);						// ȭ�� �̸� ����.
		pfread(TempString, TempInt, 1, fp);				// �� �̸� �д´�.
		TempString[TempInt] = NULL;

		// ĸ�� ȭ�� �б�.
		pfread(&TempInt, sizeof(int), 1, fp);						// ĸ�� ȭ�� �̸� ����.
		assert(TempInt > 0);
		pfread(fname, TempInt, 1, fp);								// ĸ���� ���� ������ �����ϰ� �ϳ��� �ִ�.
		fname[TempInt] = NULL;
		strcpy(path_name, SrcPath );
		strcat(path_name, "csy/" );
		strcat(path_name, fname);

		strcpy(path_name2, MovePath);
		strcat(path_name2, "csy/" );
		strcat(path_name2, fname);

		// ĸ�� ���� ����.
		CopyFile( path_name, path_name2, TRUE ); // �ڿ� TRUE�� ������ �����Ұ�� ��ŵ�϶�� �÷���.


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// ĸ������ �ؽ��� �б�.
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

			// Object�� ������ŭ �о�´�.
			for (int obj_idx = 0; obj_idx < object_num; obj_idx++)
			{
				// Material�� Texture���� �̸��� �д´�.
				pfread(&name_len, sizeof(int), 1, fpCap);
				if (name_len)
				{
					pfread(name, name_len, 1, fpCap);
					name[name_len] = '\0';

					// �ؽ��� ���� ����. �ؽ��Ĵ� bmp�� tgaȮ���ڷ� �����õ� ���·� ������ ����־���Կ� ����.
					strcpy(path_name, SrcPath);
					strcat(path_name, "textr//" );
					strcat(path_name, name);
					strcpy(path_name2, MovePath);
					strcat(path_name2, "textr//" );
					strcat(path_name2, name);
					CopyFile( path_name, path_name2, TRUE ); // �ڿ� TRUE�� ������ �����Ұ�� ��ŵ�϶�� �÷���.
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



		// �ؽ��� �ε�...
		pfread(&len, sizeof(int), 1, fp);						// �ؽ��� ȭ�� �̸� ����.
		if (len > 0)
		{
			pfread(fname, len, 1, fp);
			fname[len] = NULL;

			// �ؽ��� ���� ����.
			strcpy(path_name, SrcPath);
			strcat(path_name, "textr//" );
			strcat(path_name, fname);
			strcpy(path_name2, MovePath);
			strcat(path_name2, "textr//" );
			strcat(path_name2, fname);
			CopyFile( path_name, path_name2, TRUE ); // �ڿ� TRUE�� ������ �����Ұ�� ��ŵ�϶�� �÷���.
		}

		// �ٿ�� ȭ�� �б�.
		pfread(&len, sizeof(int), 1, fp);						// ĸ�� ȭ�� �̸� ����.
		if (len > 0)
		{
			pfread(fname, len, 1, fp);								// ĸ���� ���� ������ �����ϰ� �ϳ��� �ִ�.
			fname[len] = NULL;

			strcpy(path_name, SrcPath);
			strcat(path_name, "csy//" );
			strcat(path_name, fname);
			strcpy(path_name2, MovePath);
			strcat(path_name2, "csy//" );
			strcat(path_name2, fname);
			CopyFile( path_name, path_name2, TRUE ); // �ڿ� TRUE�� ������ �����Ұ�� ��ŵ�϶�� �÷���.
		}

		for (int j=0; j < 5; j++)
		{
			// j�� stand, move, attack, die, knock
			// ���ĵ� �������
			pfread(&len, sizeof(int), 1, fp);						// ������� ȭ���̸� ����.
			assert(len > 0);
			pfread(fname, len, 1, fp);
			fname[len] = NULL;

			strcpy(path_name, SrcPath);
			strcat(path_name, "bpd//" );
			strcat(path_name, fname);
			strcpy(path_name2, MovePath);
			strcat(path_name2, "bpd//" );
			strcat(path_name2, fname);
			CopyFile( path_name, path_name2, TRUE ); // �ڿ� TRUE�� ������ �����Ұ�� ��ŵ�϶�� �÷���.


			// ���ĵ� ����
			pfread(&len, sizeof(int), 1, fp);						// ���� ���� �̸� ����.
			if ( len > 0)
			{
				// ���� ������ �ִ�.
				pfread(fname, len, 1, fp);
				fname[len] = NULL;

				strcpy(path_name, SrcPath);
				strcat(path_name, "wav//" );
				strcat(path_name, fname);
				strcpy(path_name2, MovePath);
				strcat(path_name2, "wav//" );
				strcat(path_name2, fname);
				CopyFile( path_name, path_name2, TRUE ); // �ڿ� TRUE�� ������ �����Ұ�� ��ŵ�϶�� �÷���.
			}

			pfread(&TempInt, sizeof(int), 1, fp);
		}

		// �� ��Ÿ
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