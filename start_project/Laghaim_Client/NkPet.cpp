#include "stdafx.h"
#define D3D_OVERLOADS

#include <tchar.h>
#include <math.h>
#include <time.h>

#include <stdlib.h>
#include <assert.h>
#include <direct.h>
#include <d3d.h>
#include <assert.h>
#include "ItemProto.h"
#include "main.h"
#include "UIMgr.h"
#include "Skill.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "D3DMath.h"
#include "Land.h"
#include "Capsyong.h"
#include "DirtSnd.h"
#include "tcpipcon.h"
#include "TcpUtil.h"
#include "NKObjQuadNode.h"
#include "SFXBaseShape.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "FxPetSkill.h"

#include "g_stringmanager.h"

extern CClrAlloc			*g_ClrAlloc;
extern float				g_fTimeKey;
extern float				g_fDSDistance;
extern DWORD				g_dwBaseTime;

extern BOOL					g_bNewTerrain;
extern int					g_EffectTextr[];

int g_PetBpdCount = 0;
CPetBpd *g_PetBiped = NULL;
int g_PetBpdIndex[ PET_CLASSTYPE_NUM ][ PET_ACT_INDEX_NUM ];
CCapsyong *g_PetPickingCsy[PET_CLASSTYPE_NUM];
void DeletingPetLoading();

int g_PetSkillGroup[PET_EVOLUTION_TYPENUM][PET_SKILL_MAX] =
{
	{ 4, 5, 6, 7 } // 방어형 : 브레스, 수호, 보호, 생명
	, { 0, 1, 2, 3 } // 공격형 : 포이즌, 마비, 콜드, 블랙홀
	, { 0, 1, 6, 7 }
}; // 펫의 스킬을 진화 형태 따라 인덱싱하는 테이블( 스킬이 추가될 경우 번호가 순차적이지 않을수있기에 이렇게 씀.)

void PetInitialLoading()
{
	DeletingPetLoading();

	PFILE *list_fp = pfopen("data/item/petbpdlist.dta", "rt");

	if( list_fp == NULL )
	{
		// error or Log

		return;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	HANDLE				hFile;
	DWORD				dwRead;
	hFile = CreateFile("PetCreateFile.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, list_fp->_base, list_fp->_basesize, &dwRead, NULL);
	CloseHandle(hFile);

	FILE* fp;
	fp = fopen("PetFrintf.txt", "w");
	char* chtemp = list_fp->_base;
	fprintf(fp, "%s", chtemp);
	fclose(fp);
	*/
	//////////////////////////////////////////////////////////////////////////

	pfscanf(list_fp, "%d", &g_PetBpdCount);
	g_PetBiped = new CPetBpd[g_PetBpdCount];

	if (g_pDSound)
		g_pDSound->SetWaveFilePath("data/character/wav/");

	for (int i = 0; i < g_PetBpdCount; i++)
	{
		char file_name[256] = {0,}, bpd_file[100], sound_file[100];
		pfscanf(list_fp, "%s %d %d %s", bpd_file, &(g_PetBiped[i].m_AttFrame),
				&(g_PetBiped[i].m_SoundFrame), sound_file);

		printf("%s %d %d %s : %d\n", bpd_file, &(g_PetBiped[i].m_AttFrame),
			   &(g_PetBiped[i].m_SoundFrame), sound_file, i);

		sprintf(file_name, "data/item/bpd/%s", bpd_file);
		PFILE *bpd_fp = pfopen(file_name, "rb");
		g_PetBiped[i].m_Bpd.Read(bpd_fp, false);
		pfclose(bpd_fp);

		if (strcmp( sound_file, "-") )
		{
			if (g_pDSound)
				g_PetBiped[i].m_SoundNum = g_pDSound->LoadWAV( sound_file );
		}
		else
			g_PetBiped[i].m_SoundNum = -1;
	}

	pfclose(list_fp);


	list_fp = pfopen("data/item/petbpdindex.dta", "rt");

	char strTemp[256], strTemp2[256];
	int BpdIndexNum = 0;
	int ClassNum = 0;
	int Temp = 0;

	if (list_fp)
	{
		pfscanf(list_fp, "%s %d %s %d\n", strTemp, &BpdIndexNum, strTemp2, &ClassNum ); // 인덱스(행동) 갯수. / 클래스(타입) 갯수.
		assert( BpdIndexNum == PET_ACT_INDEX_NUM );
		assert( ClassNum == PET_CLASSTYPE_NUM );

		for( int i = 0 ; i < BpdIndexNum ; ++i )
		{
			pfscanf(list_fp, "%s", strTemp ); // 액트 번호 및 설명

			for( int j = 0 ; j < ClassNum ; ++j )
			{
				if( j == ClassNum-1 ) // 마지막 녀석이면 개행 해준다.
					pfscanf(list_fp, "%s\n", strTemp2 ); // bpd index
				else
					pfscanf(list_fp, "%s", strTemp2 ); // bpd index

				if( strcmp(strTemp2,"-") == 0 ) // "-"는 없는 거다.
					g_PetBpdIndex[j][i] = -1;
				else
					g_PetBpdIndex[j][i] = atoi(strTemp2);
			}
		}

		pfclose(list_fp);
	}

	g_PetPickingCsy[0] = new CCapsyong(g_pCapsyongTexture);
	if( VF_ERR_NONE > g_PetPickingCsy[0]->Read("data/item/csy/petbound.CSY") )
	{
		SAFE_DELETE(g_PetPickingCsy[0]);
		return;
	}

	g_PetPickingCsy[1] = new CCapsyong(g_pCapsyongTexture);
	if( VF_ERR_NONE > g_PetPickingCsy[1]->Read("data/item/csy/pet01_bound.CSY") )
	{
		SAFE_DELETE(g_PetPickingCsy[1]);
		return;
	}

	g_PetPickingCsy[2] = new CCapsyong(g_pCapsyongTexture);
	if( VF_ERR_NONE > g_PetPickingCsy[2]->Read("data/item/csy/pet02_bound.CSY") )
	{
		SAFE_DELETE(g_PetPickingCsy[2]);
		return;
	}

	g_PetPickingCsy[3] = new CCapsyong(g_pCapsyongTexture);
	if( VF_ERR_NONE > g_PetPickingCsy[3]->Read("data/item/csy/pet02_bound.CSY") )
	{
		SAFE_DELETE(g_PetPickingCsy[3]);
		return;
	}
}

void DeletingPetLoading()
{
	SAFE_DELETE_ARRAY(g_PetBiped);
	g_PetBpdCount= 0;
	g_PetBiped = NULL;

	for( int i = 0 ; i < PET_CLASSTYPE_NUM ; ++i )
		SAFE_DELETE( g_PetPickingCsy[i] );
}

//	by evilkiki 2008.08.04
//	return value 위험성으로 함수 수정
void GetPetSkillName( int skill_index , OUT char * rv) // 펫 스킬 인덱스로 부터 이름을 얻어오는 함수.
{
	switch( skill_index )
	{
	case 0:
		// IDS_PETSKILL0_POISON : 포이즌
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL0_POISON) );
		return;
	case 1:
		// IDS_PETSKILL1_PARALYSIS : 마비
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL1_PARALYSIS) );
		return;
	case 2:
		// IDS_PETSKILL2_COLD : 콜드
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL2_COLD) );
		return;
	case 3:
		// IDS_PETSKILL3_BLACKHOLE : 블랙홀
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL3_BLACKHOLE) );
		return;
	case 4:
		// IDS_PETSKILL4_BREATH : 브레스
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL4_BREATH) );
		return;
	case 5:
		// IDS_PETSKILL5_GUARD : 수호
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL5_GUARD) );
		return;
	case 6:
		// IDS_PETSKILL6_DEFENCE : 보호
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL6_DEFENCE) );
		return;
	case 7:
		// IDS_PETSKILL7_LIFE : 생명
		sprintf( rv, (char *)G_STRING(IDS_PETSKILL7_LIFE) );
		return;
	}

	sprintf( rv, "-" );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNkPet::CNkPet()
	: m_was_wx(0)
	, m_was_wz(0)
	, m_item_wx(0)
	, m_item_wz(0)
	, m_bMetaPet(FALSE)
{
	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		m_BodyItem[i] = -1;
		m_BodyItem_Vnum[i] = -1;
	}

	m_PetId = -1;
	m_ActNum = 0;
	m_NextActNum = 0;
	m_strName[0] = '\0';
	m_PetClass = 0;
	m_pSkillEffect1 = NULL;

	m_pSkillEffect1 = new CRectPlane;
	if( m_pSkillEffect1 )
		m_pSkillEffect1->Create( 40.0f, 40.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)

	// [3/6/2007 Theodoric] 라비용 배고품 표시 :
	m_bview = FALSE;
	m_beforTime = 0;
	eat_smile.Create( 4.0f, 4.0f, FALSE );

	m_BodyItem_Class3[PET_PIECE_HORN] = g_ItemList.FindItem(3893);
	m_BodyItem_Class3[PET_PIECE_HEAD] = g_ItemList.FindItem(3889);
	m_BodyItem_Class3[PET_PIECE_BODY] = g_ItemList.FindItem(3890);
	m_BodyItem_Class3[PET_PIECE_WINGS] = g_ItemList.FindItem(3891);
	m_BodyItem_Class3[PET_PIECE_LEGS] = g_ItemList.FindItem(3888);
	m_BodyItem_Class3[PET_PIECE_TAIL] = g_ItemList.FindItem(3892);
}

CNkPet::CNkPet(CClrAlloc *pClrAlloc)
	: CNkMob(pClrAlloc)
	, m_was_wx(0)
	, m_was_wz(0)
	, m_item_wx(0)
	, m_item_wz(0)
{
	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		m_BodyItem[i] = -1;
		m_BodyItem_Vnum[i] = -1;
	}

	m_PetId = -1;
	m_ActNum = 0;
	m_NextActNum = 0;
	m_strName[0] = '\0';
	m_bMetaPet = FALSE;
	m_PetClass = 0;
	m_pSkillEffect1 = NULL;

	m_pSkillEffect1 = new CRectPlane;
	if( m_pSkillEffect1 )
		m_pSkillEffect1->Create( 40.0f, 40.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)

	// [3/6/2007 Theodoric] 라비용 배고품 표시 : 초기화
	m_bview = FALSE;
	m_beforTime = 0;
	eat_smile.Create( 4.0f, 4.0f, FALSE );

	m_BodyItem_Class3[PET_PIECE_HORN] = g_ItemList.FindItem(3893);
	m_BodyItem_Class3[PET_PIECE_HEAD] = g_ItemList.FindItem(3889);
	m_BodyItem_Class3[PET_PIECE_BODY] = g_ItemList.FindItem(3890);
	m_BodyItem_Class3[PET_PIECE_WINGS] = g_ItemList.FindItem(3891);
	m_BodyItem_Class3[PET_PIECE_LEGS] = g_ItemList.FindItem(3888);
	m_BodyItem_Class3[PET_PIECE_TAIL] = g_ItemList.FindItem(3892);
}

CNkPet::~CNkPet()
{
	SAFE_DELETE( m_pSkillEffect1 );
}

void CNkPet::SetBody(int horn, int head, int body, int wings, int legs, int tail)
{
	if (m_PetClass >= 3)
	{
		int item_idx = g_ItemList.FindItem(horn);
		if (item_idx >= 0)
			this->m_BodyItem_Class3[0] = item_idx;

		item_idx = g_ItemList.FindItem(head);
		if (item_idx >= 0)
			this->m_BodyItem_Class3[1] = item_idx;

		item_idx = g_ItemList.FindItem(body);
		if (item_idx >= 0)
			this->m_BodyItem_Class3[2] = item_idx;

		item_idx = g_ItemList.FindItem(wings);
		if (item_idx >= 0)
			this->m_BodyItem_Class3[3] = item_idx;

		item_idx = g_ItemList.FindItem(legs);
		if (item_idx >= 0)
			this->m_BodyItem_Class3[4] = item_idx;

		item_idx = g_ItemList.FindItem(tail);
		if (item_idx >= 0)
			this->m_BodyItem_Class3[5] = item_idx;


		return;		
	}
	else
	{
		SetPiece(horn, 0);
		SetPiece(head, 1);
		SetPiece(body, 2);
		SetPiece(wings, 3);
		SetPiece(legs, 4);
		SetPiece(tail, 5);
	}	
}

void CNkPet::SetPiece(int v_num, int piece_num)
{
	if (v_num >= 0)
	{
		int item_idx = g_ItemList.FindItem(v_num);

		if (item_idx < 0)
			return;

		if (g_ItemList.m_ItemArray[item_idx].m_Type != ITYPE_PET)
			return;

		if( m_PetClass != g_ItemList.m_ItemArray[item_idx].m_Dex ) // 기민성 부분에 각 파츠의 클래스를 기록한다. 클래스가 안맞는 파츠는 못낀다.
			return;

		if (piece_num < 0)
			piece_num = g_ItemList.m_ItemArray[item_idx].m_Shape;
		else if (g_ItemList.m_ItemArray[item_idx].m_Shape != piece_num)
			return;

		// 메타 피스를 보여주자!
		if (m_bMetaPet && g_ItemList.m_ItemArray[item_idx].m_Int > 0)
		{
			int item_idx2 = g_ItemList.FindItem(g_ItemList.m_ItemArray[item_idx].m_Int);

			if (item_idx2 > 0 && g_ItemList.m_ItemArray[item_idx].m_Type == ITYPE_PET)
			{
				if (g_ItemList.m_ItemArray[item_idx].m_Shape == piece_num)
					item_idx = item_idx2;
			}
		}

		m_BodyItem[piece_num] = item_idx;
		m_BodyItem_Vnum[piece_num] = v_num;
	}
	else
	{
		if (piece_num < 0 || piece_num >= PET_PIECE_NUM)
			return;

		m_BodyItem[piece_num] = -1;
		m_BodyItem_Vnum[piece_num] = -1;
	}
}

void CNkPet::Kill()
{
	SetMotionState(PET_DEAD);
	m_DeadFrame = 0;
}

BOOL CNkPet::Move(float tx, float tz, int nFrameCount)
{
	// 타겟지점 저정
	m_tx = tx;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;

	if( fabs(m_dx) < 0.1f ) // 라비용 도망 버그 수정. m_dx나 m_dz가 너무 작은 수 (내 생각으로는 소수점 3째(0.01이하)를 넘어가면 그렇게 될 확률이 높을 듯. 그냥 여유 잡고 0.1 이하면 보정하자. )
	{
		m_dx = 0.0f;
		m_wx = m_tx;
	}
	if( fabs(m_dz) < 0.1f ) // 라비용 도망 버그 수정. m_dx나 m_dz가 너무 작은 수 (내 생각으로는 소수점 3째(0.01이하)를 넘어가면 그렇게 될 확률이 높을 듯. 그냥 여유 잡고 0.1 이하면 보정하자. )
	{
		m_dz = 0.0f;
		m_wz = m_tz;
	}

	m_toDir = (float) atan2(m_dz, m_dx);

	if (nFrameCount == 0)
		nFrameCount = 1;

	m_dx /= nFrameCount;
	m_dz /= nFrameCount;

	m_bMoving = TRUE;

	SetMotionState(PET_MOVE);

	return TRUE;
}

BOOL CNkPet::Attack(float tx, float tz)
{
	// 공격 명령이 왔는데, 몹이 이동중이면 이동최종 목적지로 몹을 위치시킨다.
	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}

	SetMotionState(PET_ATTACK);

	m_tx = tx;
	m_tz = tz;
	m_dx = m_tx - m_wx;
	m_dz = m_tz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);

	return TRUE;
}

BOOL CNkPet::Pickup(float tx, float tz)
{
	if (IsMoving())
	{
		m_wx = m_tx;
		m_wz = m_tz;
	}

	SetMotionState(PET_PICKUP);

	m_was_wx = m_wx;
	m_was_wz = m_wz;
	m_item_wx = tx;
	m_item_wz = tz;
	m_dx = m_item_wx - m_wx;
	m_dz = m_item_wz - m_wz;
	m_toDir = (float) atan2(m_dz, m_dx);

	return TRUE;
}

void CNkPet::SetMotionState(unsigned short nState, int act_num) // act_num이 bpdlist의 인덱스가 아니라 bpdindex의 행동 번호임에 유의.
{
	m_dwBaseTime = timeGetTime();
	m_dwTimeCapPrev = m_dwBaseTime;
	m_nMotionState = nState;

	switch(nState)
	{
	case PET_STAND:						// stand
		m_bMoving = FALSE;
		m_bAttack = FALSE;
		m_ActNum = 0;
		break;
	case PET_MOVE:						// move
		m_bMoving = TRUE;
		m_bAttack = FALSE;
		m_ActNum = 0;
		break;
	case PET_ATTACK:						// attack
		m_bMoving = FALSE;
		m_bAttack = TRUE;
		m_PrevFrame = 0;
		m_CurFrame = 0;
		m_ActNum = (rand() % 6) + 5;
		break;
	case PET_DEAD:						// die
		m_bMoving = FALSE;
		m_bAttack = FALSE;
		m_PrevFrame = 0;
		m_CurFrame = 0;
		m_ActNum = 3;
		break;
	case PET_KNOCK:						// knock
		m_PrevFrame = 0;
		m_CurFrame = 0;
		m_ActNum = 0;
		break;
	case PET_PICKUP:					// pickup
		m_PrevFrame = 0;
		m_CurFrame = 0;
		m_ActNum = 4;
		break;
	case PET_SOCIAL:
		m_PrevFrame = 0;
		m_CurFrame = 0;
		m_ActNum = act_num;

		if( act_num == 22 || act_num == 23 ) // 일부 바닥에서 노는 종류는 업&다운 모션과 이어준다.(여기선 다운만.)
		{
			m_ActNum = 1; // 다운 모션
			m_NextActNum = act_num; // 다운 뒤로 이어질 모션 번호를 기억.
		}
		else
		{
			m_ActNum = act_num;
		}

		break;

	case PET_USE_SKILL1: // 스킬 사용 모션
		m_PrevFrame = 0;
		m_CurFrame = 0;
		m_ActNum = 24;
		break;

	default:
		break;
	}

	// m_ActNum을 라비용 클래스에 따라 변환해준다.
	m_ActNum = GetClassActNum( m_ActNum );

	if( m_ActNum == -1 ) // 혹시 없는 바이페드면... (랜덤으로 정해지는 녀석만 이게 들어와야한다. 나머진 필수적으로 있어야함.)
	{
		if( m_nMotionState == PET_ATTACK ) // 어택은 진화형은 애니메이션이 몇개 없는게 있다. 이경우 1번 공격 애니메이션으로 셋팅해주자.
			m_ActNum = g_PetBpdIndex[m_PetClass][5]; // 1번 공격 애니메이션(꼬리공격1). (이건 꼭 있게 하자.)
		else if( m_nMotionState == PET_SOCIAL ) // 소셜이면 일부 빠져있는게 있으니 한단계 높은 소셜 단계로 해준다.
			SetMotionState( m_nMotionState, act_num-1 ); // 이렇게 하면 설마 위의 것도 없어도 계속 더 찾아준다.
	}

	assert( m_ActNum != -1 ); // 이 경운 뭔가 삐꾸인거다.

}

int CNkPet::GetClassActNum( int OriActNum ) // ActNum(행동 번호)를 토대로 클래스 별로 실제 사용될 BpdIndex를 얻어온다.
{
	int ReturnAct = 0;

	if( OriActNum < 0 ) // 혹시 모르니...
		OriActNum = 0;

	// m_ActNum을 라비용 클래스에 따라 변환해준다.
	ReturnAct = g_PetBpdIndex[m_PetClass][OriActNum];

	return ReturnAct;
}

int CNkPet::GetActNumFromClassBpd( int ClassBpdIndex ) // 각 클래스의 BpdIndex ActNum을 행동 번호로 변환해주는 녀석.
{
	int ReturnAct = -1;

	for( int i = 0 ; i < PET_ACT_INDEX_NUM ; ++i ) // 전 행동 갯수를 돌면서
	{
		if( g_PetBpdIndex[m_PetClass][i] == ClassBpdIndex ) // 자기 클래스의 BpdIndex 중에서 찾는다.
		{
			ReturnAct = i;
			break;
		}
	}

	return ReturnAct;
}

#define MYPET_PICKUP_FRAME	(40)

void CNkPet::Render(BOOL bMouseOn, BOOL bVisible, BOOL bDontPick)
{
	if( !GET_D3DDEVICE() )
		return;

	static D3DMATRIX matScale, matTrans, matRotate, matWorld;
	static DWORD s_AlphaTest, s_dwAmbient, s_dwZWriteEnable;
	DWORD dwCurtime;
	DWORD dwTimeCap;
	unsigned int frame_count;
	BOOL bArrived = FALSE;

	dwCurtime = timeGetTime();

	// Vital 이 변해야 하는데, 1초가 지나도 안변하면...
	if (m_Vital != m_AttackVital && dwCurtime > m_LastAttackVitalTime + 1000)
	{
		Attacked();
		dwCurtime = timeGetTime();
	}

	// 나 너 때렸다.
	CNkMob *pMob = NULL;
	CNkCharacter *pCha = NULL;
	if (m_AttackMobId >= 0 || m_AttackChaId >= 0 || m_AttackPetId >=0)
	{
		if (m_nMotionState != PET_ATTACK ||	g_PetBiped[m_ActNum].m_AttFrame <= m_CurFrame)
		{
			if (m_AttackMobId >= 0)
			{
				pMob = pCMyApp->m_pUIMgr->FindMobById(m_AttackMobId);
				if (pMob)
					pMob->Attacked();
			}
			else if (m_AttackChaId >= 0)
			{
				pCha = pCMyApp->m_pUIMgr->FindChaById(m_AttackChaId);
				if (pCha)
					pCha->Attacked();
			}
			else
			{
				pMob = (CNkMob *) pCMyApp->m_pUIMgr->FindPetById(m_AttackPetId);
				if (pMob)
					pMob->Attacked();
			}


			if( m_ActNum >= 5 && m_ActNum <= 6 )
			{
				if (pMob)
					pCMyApp->m_pUIMgr->InsertPetHitEffects(this, NULL, NULL, NULL, pMob, PET_HIT_NAIL);
				else if (pCha)
					pCMyApp->m_pUIMgr->InsertPetHitEffects(this, NULL, pCha, NULL, NULL, PET_HIT_NAIL);
			}
			else if( m_nMotionState != PET_USE_SKILL1 ) // df
			{
				if (pMob)
					pCMyApp->m_pUIMgr->InsertPetHitEffects(this, NULL, NULL, NULL, pMob, PET_HIT_NAIL);
				else if (pCha)
					pCMyApp->m_pUIMgr->InsertPetHitEffects(this, NULL, pCha, NULL, NULL, PET_HIT_NAIL);
			}

			m_AttackMobId = -1;
			m_AttackChaId = -1;
			m_AttackPetId = -1;
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
		else if (m_dx > 0.001f && m_wx >= m_tx)     // 여기부터는 추가 검사...
		{
			bArrived = TRUE;
		}
		else if (m_dx < -0.001f && m_wx <= m_tx)     // 애완동물 도망가는 거 막아보려구...
		{
			bArrived = TRUE;
		}
		else if (m_dz > 0.001f && m_wx >= m_tz)
		{
			bArrived = TRUE;
		}
		else if (m_dz < -0.001f && m_wx <= m_tz)
		{
			bArrived = TRUE;
		}

		if (bArrived)
		{
			m_wx = m_tx;
			m_wz = m_tz;
			m_bMoving = FALSE;
		}

		if (m_bMoving == FALSE )	// 스테이트를 바꾸어 주어야 한다.
			SetMotionState(PET_STAND);

//=======================================================
		// nate 2004 - 6 : 라비용 높이 버그 수정
		if( g_bNewTerrain )
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight( m_wx, m_wz );
			m_wy = CNKObjQuadNode::GetHeightOnObject( m_wz, m_wx, m_wy );
			if( m_wy < fHeight )
				m_wy = fHeight;
		}
		else
			m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);
//=======================================================
	}

	if( m_nMotionState == PET_USE_SKILL1 ) // 스킬 사용중엔...
	{
	}

	if (m_nMotionState == PET_PICKUP)
	{
		if (m_PrevFrame < (MYPET_PICKUP_FRAME/2) && (MYPET_PICKUP_FRAME/2) <= m_CurFrame &&
				pCMyApp->m_pMyPet && pCMyApp->m_pMyPet == (CNkMyPet *) this)
		{
			g_pTcpIp->SendNetMessage(pCMyApp->m_pMyPet->m_TmpBuf);
			m_toDir = (float) atan2(m_wz - m_was_wz, m_wx - m_was_wx);
		}

		if (m_CurFrame < (MYPET_PICKUP_FRAME/2))
		{
			m_wx = m_was_wx + (m_item_wx - m_was_wx) / (MYPET_PICKUP_FRAME/2) * m_CurFrame;
			m_wz = m_was_wz + (m_item_wz - m_was_wz) / (MYPET_PICKUP_FRAME/2) * m_CurFrame;
		}
		else
		{
			m_wx = m_was_wx + (m_item_wx - m_was_wx) / (MYPET_PICKUP_FRAME/2) * (MYPET_PICKUP_FRAME - m_CurFrame);
			m_wz = m_was_wz + (m_item_wz - m_was_wz) / (MYPET_PICKUP_FRAME/2) * (MYPET_PICKUP_FRAME - m_CurFrame);
		}
//=======================================================
		// nate 2004 - 6
		// 라비용 높이 버그 수정
		if( g_bNewTerrain )
		{
			float fHeight = pCMyApp->GetLandPtr()->GetHFHeight( m_wx, m_wz );
			m_wy = CNKObjQuadNode::GetHeightOnObject( m_wz, m_wx, m_wy );
			if( m_wy < fHeight )
				m_wy = fHeight;
		}
		else
			m_wy = pCMyApp->GetLandPtr()->GetHFHeight(m_wx, m_wz);
//=======================================================
	}

	float	fWaterHeight = 0.0f;

	if( g_bNewTerrain )
		fWaterHeight = CNKObjQuadNode::GetHeightOnWater( m_wz, m_wx );
	else
		fWaterHeight = WATER_HEIGHT;

	if( m_wy < fWaterHeight )
		m_wy = fWaterHeight;

	if (m_DeadFrame > 60)
		m_wy -= (m_DeadFrame-60) * (2.0f / 40); // 60 Frame(3초) 후부터 40 Frame(2초) 동안 6.0f을 가라 앉는다.

	// 다음 프레임 계산
	FrameMove(frame_count, bVisible);

	if (bVisible == FALSE)
		return;

	// 월드 트랜스폼. - 캐릭터를 그리기 위한.
	float fScale = 1.0f;
	if( g_MyPetStatus.m_Class == 3 )
	{
		fScale = 1.5f;
	}
	D3DUtil_SetScaleMatrix(matScale, fScale, fScale, fScale);
	D3DUtil_SetTranslateMatrix( matTrans, m_wx, m_wy, m_wz);
	Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);		
	D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
	D3DMath_MatrixMultiply( matWorld, matScale, matWorld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	// 상태값 보존.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &s_AlphaTest );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);

	// 주변광 저장
	if (bMouseOn)
	{
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &s_dwAmbient);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x0072ff84);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &s_dwZWriteEnable);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	}

	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		if( m_PetClass == 3 )
		{
			if( m_BodyItem_Class3[i] >= 0 )
			{
				if( GET_CSYC(m_BodyItem_Class3[i], 0) )
					GET_CSYC(m_BodyItem_Class3[i], 0)->Render(GET_D3DDEVICE(), GET_TEXTR_NUM_PROTO(m_BodyItem_Class3[i], 0));
			}
		}
		else
		{
			if (m_BodyItem[i] >= 0)
			{
				if( GET_CSYC(m_BodyItem[i], 0) )
					GET_CSYC(m_BodyItem[i], 0)->Render(GET_D3DDEVICE());
			}
		}		
	}

	if (bMouseOn)
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, s_dwAmbient);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, s_dwZWriteEnable);
	}
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, s_AlphaTest );
	///////////////////////////////////////
	// pick buffer 쓰기.
	if (!IsDead() && GET_D3DPICK() && bDontPick == FALSE)
	{
		GET_D3DPICK()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );

		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
//		GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///피킹버그수정. DWORD 형으로 변환한걸쓰지말고 그냥 WORD형을 바로 색상으로 넣는다. 이렇게 해도 32767개 까지 동시 피킹이 가능할 터. 변환중 뭔가 꼬이는 경우가 있는듯해서 이렇게 변경함. (07-01-02 원석)
		if( g_PetPickingCsy[m_PetClass] )
			g_PetPickingCsy[m_PetClass]->Render(GET_D3DPICK(), -1, TRUE);

		GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	}


	// [3/6/2007 Theodoric] 라비용 배고품 표시 : 사과가 없으면 머리위에 사과를 그려주자.
	if( g_MyPetStatus.food_count <= 0 && this == pCMyApp->m_pMyPet )
	{
		D3DVECTOR vTemp = D3DVECTOR(g_PetBiped[m_ActNum].m_Bpd.m_AniMat[10].m_Mats[m_CurFrame]._41,
									g_PetBiped[m_ActNum].m_Bpd.m_AniMat[10].m_Mats[m_CurFrame]._42,
									g_PetBiped[m_ActNum].m_Bpd.m_AniMat[10].m_Mats[m_CurFrame]._43 );

		D3DMath_VectorMatrixMultiply( vTemp, vTemp, matWorld );

		float Temphigh = 0.0f;
		switch( g_MyPetStatus.m_Class )
		{
		case 0:
			Temphigh =  7.0f;
			break; // 일반형
		case 1:
			Temphigh = 10.0f;
			break; // 방어형
		case 2:
			Temphigh =  7.0f;
			break; // 공격형
		case 3:
			Temphigh = 7.0f;
			break;
		}

		RenderApple( vTemp.x, vTemp.y+Temphigh, vTemp.z );
	}
}


void CNkPet::FrameMove(unsigned int nInc, BOOL bVisible)
{
	EffectSort ef_sort;

	if (bVisible)
	{
		for (int i = 0; i < PET_PIECE_NUM; i++)
		{
			if( m_PetClass == 3 )
			{
				if (m_BodyItem_Class3[i] >= 0)
				{
					if( GET_CSYC(m_BodyItem_Class3[i], 0) )
						GET_CSYC(m_BodyItem_Class3[i], 0)->Animate(&g_PetBiped[m_ActNum].m_Bpd, m_CurFrame);
				}
			}
			else
			{
				if (m_BodyItem[i] >= 0)
				{
					if( GET_CSYC(m_BodyItem[i], 0) )
						GET_CSYC(m_BodyItem[i], 0)->Animate(&g_PetBiped[m_ActNum].m_Bpd, m_CurFrame);
				}
			}			
		}


		if( g_PetPickingCsy[m_PetClass] )
			g_PetPickingCsy[m_PetClass]->Animate(&g_PetBiped[m_ActNum].m_Bpd, m_CurFrame);

		if( g_PetBiped[m_ActNum].m_SoundNum >= 0 )
		{
			if( m_PrevFrame < m_CurFrame )
			{
				if( m_PrevFrame < g_PetBiped[m_ActNum].m_SoundFrame &&
						m_CurFrame >= g_PetBiped[m_ActNum].m_SoundFrame)
				{
					float x, y, z;
					x = (m_wx - g_pRoh->m_wx) / g_fDSDistance;
					y = (m_wy - g_pRoh->m_wy) / g_fDSDistance;
					z = (m_wz - g_pRoh->m_wz) / g_fDSDistance;

					if( g_pDSound )
						g_pDSound->Play( g_PetBiped[m_ActNum].m_SoundNum, x, y, z );
				}
			}
		}
	}

	m_PrevFrame = m_CurFrame;
	m_CurFrame += nInc;
	m_EffFrame += nInc;

	// Frame 증가
	//m_CurFrame++;
	// 한동작이 다 끝난 다음에 원상태(서있는상태)로 복귀하는 코드
	// 단 죽었을때에는 동작이 끝나면 끌난상태로 계속.
	// 일부 동작들은 업 & 다운과 이어져야한다.
	if (m_CurFrame >= g_PetBiped[m_ActNum].m_Bpd.m_AnimCount)
	{
		if (IsDead())
		{
			m_CurFrame = g_PetBiped[m_ActNum].m_Bpd.m_AnimCount-1;
			m_DeadFrame += nInc;
		}
		else if( GetActNumFromClassBpd(m_ActNum) == 1 ) // 다운 모션 중이었다면 다음 액션을 이어준다.
		{
			m_ActNum = GetClassActNum( m_NextActNum ); // 미리 지정해둔 다음 모션으로...

			m_PrevFrame = 0;
			m_CurFrame = 0;
			m_dwTimeCapPrev = timeGetTime(); // 이건 꼭 해줘야하는건진 모르겠다.
		}
		else if( GetActNumFromClassBpd(m_ActNum) == 22 || GetActNumFromClassBpd(m_ActNum) == 23 ) // 일부 모션들은 업모션으로 이어져야한다.(현재로선 땅에서 노는 devolt 4와 6만 해당)
		{
			m_ActNum = GetClassActNum( 2 ); // 자신의 클래스에 맞는 업 애니메이션 번호를 얻어온다.

			m_PrevFrame = 0;
			m_CurFrame = 0;
			m_dwTimeCapPrev = timeGetTime(); // 이건 꼭 해줘야하는건진 모르겠다.
		}
		else
		{
			m_CurFrame = 0;
			if (!IsStanding() && !IsMoving())
				SetMotionState(PET_STAND);
		}
	}

	if (bVisible)
	{
		if (m_bAttacked)
		{
			DWORD dwTimeGap;
			dwTimeGap = g_dwBaseTime - m_nAttackedTime ;
			if ( dwTimeGap <= 225)  		// 4프레임 짜리. 오차보정도 함.. 25
			{
				// 임시로 넣은 코드.. 수정 필요.
				EffectSort ef_sort;
				ef_sort.nType = FX_UNARMEDATTACKED;
				ef_sort.bOneFrame = TRUE;
				ef_sort.nCurFrame = dwTimeGap / 50;		// 이 효과는 객체내에서 디스플레이할 프레임 번호를 알려준다.
				ef_sort.vPos.x = m_wx;
				ef_sort.vPos.y = m_wy + 15.0f;
				ef_sort.vPos.z = m_wz;

				pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			}
			else
				m_bAttacked = FALSE;
		}
	}
}

void CNkPet::SetName(char *name)
{
	if (!name)
		return;

	if (strlen(name) > MAX_PET_NAME)
		return;

	strcpy(m_strName, name);
}

void CNkPet::SetPetEffect( int skill_idx, CNkMob *pMob, CNkCharacter *pCha ) // CNkCharater 의 SetMagicMob,SetMagicCha 기능의 함수. 스킬을 사용한 펫과 대상에대한 이펙트,모션등을 셋팅한다.
{
	EffectSort ef_sort;
	ZeroMemory( &ef_sort, sizeof(EffectSort) );
	D3DVECTOR vTempVec;
	float TempFloat;

	switch( skill_idx ) // 스킬에 따라 다르게 처리한다.
	{
	case 0: // 포이즌(진화 1단계 방어형 스킬)
		// 다중 타겟 스킬이다.(3타겟까지)
		// 포이즌은 좀 예외적으로 처리하자. p_effect 를 하나만 받고, 이건 펫의 모션 및 독구름 이펙트 추가에만 쓴다.
		// 공격 타겟에 대한 패킷은 상태 변화만 다른 패킷 통해서 넣도록...
		SetMotionState( PET_USE_SKILL1 );

		ef_sort.nType = FX_PETSKILL; // 따라다니는 이펙트의
		ef_sort.Index = 1; // 이펙트 타입. 1은 포이즌 구(가스 아님) 이펙트.

		// 초기화.
		ef_sort.nCurFrame = 0; // 프레임은 일정 시간에 이동시키기 위해 사용.
		ef_sort.subNum = 0; // 상태값(또는 애니메이션)으로 사용.(0은 가스 나오기 전, 1은 가스 나온 후)

		ef_sort.vPos.x = m_wx; // 시작 위치
		ef_sort.vPos.y = m_wy;
		ef_sort.vPos.z = m_wz;

		ef_sort.pMobFrom = (CNkMob *)this;

		this->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

		break;

	case 4: // 브레스(진화 1단계 공격형 스킬)
		SetMotionState( PET_USE_SKILL1 );
		pCMyApp->m_pUIMgr->InsertMagic(NULL, this, pCha, pMob, EFFECT_SKILL_PET_BLESS );
		break;

	case 1: // 마비(진화 1단계 방어형 스킬)
	case 2: // 콜드(진화 1단계 방어형 스킬)
	case 3: // 블랙홀(진화 1단계 방어형 스킬)
	case 5: // 수호(진화 1단계 공격형 스킬)
	case 6: // 보호(진화 1단계 공격형 스킬)
	case 7: // 생명(진화 1단계 공격형 스킬)

		SetMotionState( PET_USE_SKILL1 ); // 현재로선 이 스킬들은 모션만 한번 취해준다.

		// 상태이상(또는 버프) 스킬들은 광자를 대상에게 쏘는 이펙트를 넣어준다.
		ef_sort.nType = FX_PETSKILL; // 따라다니는 이펙트의
		ef_sort.Index = 0; // 이펙트 타입. 0은 광자 이펙트.

		// 초기화.
		ef_sort.nCurFrame = -27; // 프레임은 일정 시간에 이동시키기 위해 사용.
//		ef_sort.nCurFrame = 0; // 프레임은 일정 시간에 이동시키기 위해 사용.
		ef_sort.subNum = 0; // 상태값(또는 애니메이션)으로 사용.(0은 날라가는중, 1은 터지는 중)
		ef_sort.vSortPos.x = 2.3f; // 크기값으로 사용.

		D3DMATRIX	matRot;
		D3DVECTOR	vTmp;

		CCapsyong *pCapSyong = NULL;
		
		if( m_PetClass == 3 )
		{
			pCapSyong = GET_CSYC(this->m_BodyItem_Class3[PET_PIECE_HEAD],0);
		}
		else
		{
			pCapSyong = GET_CSYC(this->m_BodyItem[PET_PIECE_HEAD],0);
		}

		if( !pCapSyong || !pCapSyong->m_LineVector || !pCapSyong->m_LineVector0 )
			return;

		NKMath_VectorMatrixMultiply(   pCapSyong->m_LineVector[0], pCapSyong->m_LineVector0[0],
									   g_PetBiped[this->m_ActNum].m_Bpd.m_AniMat[ pCapSyong->m_LineBiped[0] ].m_Mats[ 27 ]    ); // 특정 프레임의 위치를 찾아온다.

		Nk3DUtil_SetRotateCCWYMatrix( matRot, this->m_dir );
		D3DMath_VectorMatrixMultiply( vTmp, pCapSyong->m_LineVector[0], matRot );
		ef_sort.vPos = D3DVECTOR(	vTmp.x + this->m_wx,
									vTmp.y + this->m_wy,
									vTmp.z + this->m_wz );

		ef_sort.vCurPos = ef_sort.vPos;


		if( pCha )
		{
			ef_sort.pNkChaTo = pCha;
			ef_sort.vTargetPos.x = pCha->m_wx; // 대상 위치
			ef_sort.vTargetPos.y = pCha->m_wy+13.0f;
			ef_sort.vTargetPos.z = pCha->m_wz;
		}
		else if( pMob )
		{
			ef_sort.pMobTo = pMob;
			ef_sort.vTargetPos.x = pMob->m_wx; // 대상 위치
			ef_sort.vTargetPos.y = pMob->m_wy+13.0f;
			ef_sort.vTargetPos.z = pMob->m_wz;
		}

		// 프레임당 이동량을 구해둔다.
		vTempVec = ef_sort.vTargetPos - ef_sort.vPos;
		TempFloat = Magnitude( vTempVec ); // 점과 직선의 거리.
		ef_sort.vSortPos.y = TempFloat / PET_SKILL_EFFECT_TYPE1_FRAME; // 일정 프레임안에 이동할수있도록 이동할 수 있게...

		// 파티클 셋팅
		Deco_Particle *ParticleInfoArray;
		ParticleInfoArray = new Deco_Particle[MAX_FXPETSKILL_PARTICLE];
		ZeroMemory( ParticleInfoArray, sizeof(Deco_Particle)*MAX_FXPETSKILL_PARTICLE );
		for( int i = 0 ; i < MAX_FXPETSKILL_PARTICLE ; ++i )
		{
			ParticleInfoArray[i].life_time = i*10+20; // 라이프 타임은 따로 초기화를 해줘야한다.
		}
		ef_sort.effects = ParticleInfoArray; // 각 이펙트 별로 파티클 메모리주소를 기억.

		int EffectNum = pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );

		this->InsertEffectNum( EffectNum );

		if( pMob )
			pMob->InsertEffectNum( EffectNum );
		else if( pCha )
			pCha->InsertEffectNum( EffectNum );

//		pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );

		break;
	}
}


// [3/6/2007 Theodoric] 라비용 배고품 표시 : 라비용의 머리위 사과를 그려준다.
void CNkPet::RenderApple( float mX, float mY, float mZ )
{
	DWORD checktime;
	DWORD currentTime = timeGetTime();
	DWORD tempTime;

	if( m_bview )
	{
		checktime = 1000;
	}
	else
	{
		checktime =  300;
	}

	tempTime = currentTime - m_beforTime;

	if( tempTime >= checktime )
	{
		m_beforTime = currentTime;
		m_bview = !m_bview;
		tempTime = 0;
	}

	if( m_bview )
	{
		DWORD dwMin, dwMag;
		DWORD dwLight, dwZEnable, dwZWriteEnable, dwAlphaTest, dwAlphaBlend;

		float Fade = 1.0f - ((float)tempTime/(float)checktime) * 0.5f;

		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_MINFILTER, &dwMin);
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_MAGFILTER, &dwMag);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwLight);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZENABLE, &dwZEnable);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &dwZWriteEnable);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
		// Set any appropiate state
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

		DWORD  dwTextureState[3];
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState(0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		// [3/6/2007 Theodoric] 바탕 그리기
		if( g_pCapsyongTexture )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_EAT_LIGHT ] ) );

		D3DVECTOR	vViewVec;
		vViewVec = pCMyApp->GetEyeDirection();

		//eat_smile.SetColor(1.0f, 1.0f, 1.0f,1.0f);
		eat_smile.SetColor(1.0f, 1.0f, 1.0f, Fade );
		eat_smile.SetBillBoard( vViewVec, FALSE, 0.0f );
		eat_smile.Scale( 1.6f, 1.6f, 1.6f );
		eat_smile.SetTranslationXYZ(mX, mY+1.0f , mZ);
		eat_smile.Render();

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );


		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwLight);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, dwZEnable);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, dwZWriteEnable);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MINFILTER, dwMin);
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MAGFILTER, dwMag);
	}


	if( m_bview )
	{
		D3DMATRIX matTrans, matRotate, matWorld, matDrop; //, matScale;

		D3DUtil_SetTranslateMatrix( matTrans, mX, mY, mZ);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, 0.0f);

		int AppleProtoNum = g_ItemList.FindItem(1051) ; // 1051 왕사과
		CLolos *pLolo = GET_LOLOC(AppleProtoNum);

		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		//GET_D3DDEVICE()->LightEnable( 1, TRUE);

		DWORD dwLight, dwAmbient;
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwLight);
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xffffffff);
		pLolo->Render(GET_D3DDEVICE(), 0, FALSE, FALSE, FALSE, &matWorld, NULL, FALSE );

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwLight);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbient);
	}

}


