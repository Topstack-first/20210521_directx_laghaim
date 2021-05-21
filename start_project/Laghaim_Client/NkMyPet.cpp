#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "main.h"
#include "D3DMath.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "UIMgr.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "tcpipcon.h"
#include "TcpUtil.h"
#include "Item.h"
#include "ItemProto.h"
#include "Land.h"
#include "NKObjQuadNode.h"
#include "g_stringmanager.h"
#include "string_res.h"

#define PET_SHORT_ATT_RANGE		(30.0f)
#define PET_NEAR_RANGE			(15.0f)
#define MYPET_PICKUP_RANGE		(50.0f)

CPetStatus g_MyPetStatus;





extern void ResetItemValue(CItem *pItem);
extern BOOL	g_bNewTerrain;
extern CNkMob* g_pTargetMob[];
extern CNkCharacter* g_pTargetCha[];

CPetStatus::CPetStatus()
	: need_exp(0)
	, max_vital(0)
	, time(0)
	, need_money(0)
{
	// 내꺼 애완동물 상태창 정보 변수 초기화
	exp = 0;
	hunger = 0;
	compos_level = 0;
	level = 0;
	loyalty = 0;
	melee_att = 0;
	melee_def = 0;
	range_att = 0;
	range_def = 0;
	speed = 0;
	pick_level = 0;
	m_pBag = NULL;
	food_count = 0;
	max_food = 1;
	m_pNecklace = NULL;
	mode = MYPET_MODE_NONE;
	m_bUseMacroMode = FALSE;

	m_Class = 0; // 라비용의 진화 타입(기본형0,방어형1,공격형2)
	for( int i = 0 ; i < PET_SKILL_MAX ; ++i )
		m_SkillLevel[i] = 0; // 자신이 스킬을 배웠는지 여부.

	memset(m_Wearing, 0, sizeof(m_Wearing));
}

CPetStatus::~CPetStatus()
{
	SAFE_DELETE(m_pBag);
	SAFE_DELETE(m_pNecklace);
	
	for(int i = 0; i < PET_PIECE_NUM; ++i)
	{
		SAFE_DELETE(m_Wearing[i]);
	}	
}

void CPetStatus::Init()
{
	SAFE_DELETE(m_pBag);
	SAFE_DELETE(m_pNecklace);

	for(int i = 0; i < PET_PIECE_NUM; ++i)
	{
		SAFE_DELETE(m_Wearing[i]);
	}

	need_exp = 0;
	max_vital = 0;
	time = 0;
	need_money = 0;
	exp = 0;
	hunger = 0;
	compos_level = 0;
	level = 0;
	loyalty = 0;
	melee_att = 0;
	melee_def = 0;
	range_att = 0;
	range_def = 0;
	speed = 0;
	pick_level = 0;
	m_pBag = NULL;
	food_count = 0;
	max_food = 1;
	m_pNecklace = NULL;
	mode = MYPET_MODE_NONE;
	m_bUseMacroMode = FALSE;

	m_Class = 0; // 라비용의 진화 타입(기본형0,방어형1,공격형2)
	for( int i = 0 ; i < PET_SKILL_MAX ; ++i )
		m_SkillLevel[i] = 0; // 자신이 스킬을 배웠는지 여부.

	memset(m_Wearing, 0, sizeof(m_Wearing));
}

BOOL CPetStatus::SetMode(int nMode)
{
	mode = nMode;

	if (pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetMode(nMode);

	return TRUE;
}


// 애완동물 상태 정보 저장
void CPetStatus::SetVital(int vital, int maxvital)
{
	max_vital = maxvital;
	if (pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetVital(vital, max_vital);
}

void CPetStatus::SetRangeAtt(int a)
{
	range_att = a;
	if (pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetRangeAtt(range_att);
}
void CPetStatus::SetMeleeAtt(int a)
{
	melee_att = a;
	if (pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetMeleeAtt(melee_att);
}
void CPetStatus::SetRangeDef(int a)
{
	range_def = a;
	if (pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetRangeDef(range_def);
}
void CPetStatus::SetMeleeDef(int a)
{
	melee_def = a;
	if (pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetMeleeDef(melee_def);
}
void CPetStatus::SetSpeed(int a)
{
	speed = a;
}
void CPetStatus::SetLevel_Compos(int a)
{
	compos_level = a;
}
void CPetStatus::SetLevel_Pick(int a)
{
	pick_level = a;
}
void CPetStatus::SetLoyalty(int a)
{
	loyalty = a;
}
void CPetStatus::SetHunger(int a)
{
	hunger = a;
}
void CPetStatus::SetLevel(int a)
{
	level = a;
}
void CPetStatus::SetExp(__int64 a)
{
	exp = a;
}
void CPetStatus::SetNeedMoney(__int64 a)
{
	need_money = a;
}

void CPetStatus::SetNeedExp(__int64 a)
{
	need_exp = a;
}

//-- 2005.06.14
//-- Lyul / 수정완료
//-- 라비용 바구니가 있을 경우 max_food 는 6으로 변경.
BOOL CPetStatus::SetBag(int vnum)
{
	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		SAFE_DELETE(m_pBag);
		m_pBag = new CItem;
		if( m_pBag )
		{
			m_pBag->m_Index = -1;
			m_pBag->m_Vnum = vnum;
			m_pBag->m_ProtoNum = proto_num;
			ResetItemValue(m_pBag);
		}
		else
			return FALSE;

		max_food = 6;
		return TRUE;
	}
	// [3/17/2009 D.K ] : 코드 수정. 재접속시 바구니가 없는 녀석인데도 있는 녀석으로 인식버그.
	// 왜 아무도 몰랐을까..=_=;;;;;
	else
	{
		SAFE_DELETE( m_pBag );
		max_food = 1;
	}

	return FALSE;
}

CItem* CPetStatus::SetNecklace(CItem *pItem)
{
	CItem *pTemp = m_pNecklace;
	m_pNecklace = pItem;

	return pTemp;
}

BOOL CPetStatus::SetNecklace(int vnum)
{
	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		SAFE_DELETE(m_pNecklace);
		m_pNecklace = new CItem;
		if( m_pNecklace )
		{
			m_pNecklace->m_Index = -1;
			m_pNecklace->m_Vnum = vnum;
			m_pNecklace->m_ProtoNum = proto_num;
			ResetItemValue(m_pNecklace);
		}
		else
			return FALSE;

		return TRUE;
	}
	return FALSE;
}

BOOL CPetStatus::SetFoodCount(int a)
{
	if (a < 0)
		return FALSE;

	if (a > max_food)
		return FALSE;

	food_count = a;

	return TRUE;
}

BOOL CPetStatus::IncFoodCount()
{
	return SetFoodCount(food_count+1);
}

void CPetStatus::SetClassType( int Class ) // 라비용 클래스(기본0, 방어형1, 공격형2)
{
	m_Class = Class;
}

void CPetStatus::SetSkill( int SkillNum, int Level ) // 라비용이 배운 스킬 셋팅.
{
	if( m_Class < 1 ) // 진화라비용이 아니면 스킬 셋팅은 안된다.(즉 스킬 스테이터스가 오기전에 클래스 타입 설정이 먼저와야한다.)
		return;

	// 스킬 번호를 0 ~ PET_SKILL_MAX 사이의 인덱스치로 바꿔줘야한다.
	for( int i = 0 ; i < PET_SKILL_MAX ; ++i )
	{
		if( g_PetSkillGroup[m_Class-1][i] == SkillNum ) // 맞는 스킬을 찾아서...
		{
			m_SkillLevel[i] = Level; // 자신이 스킬을 배웠는지 여부.
			return;
		}
	}
}

BOOL CPetStatus::SetEquip(int where, int vnum)
{
	if (vnum < 0)
	{
		SAFE_DELETE(m_Wearing[where]);
		return TRUE;
	}

	int proto_num = g_ItemList.FindItem(vnum);
	if (proto_num >= 0)
	{
		SAFE_DELETE(m_Wearing[where]);
		m_Wearing[where] = new CItem;
		if( m_Wearing[where] )
		{
			m_Wearing[where]->m_Index = -1;
			m_Wearing[where]->m_Vnum = vnum;
			m_Wearing[where]->m_ProtoNum = proto_num;
			ResetItemValue(m_Wearing[where]);
		}
		else
			return FALSE;

		return TRUE;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNkMyPet::CNkMyPet()
	: m_UseSkillTime(0)
{
	m_CombatMobId = -1;
	m_CombatChaId = -1;
	m_CombatPetId = -1;
	m_MasterRange = MYPET_MASTER_RANGE_DEFAULT;
	m_Vital = 100;
	m_MaxVital = 100;
	m_AttackVital = 100;
	InitSocialTime();
	m_MeleeAtt = 0;
	m_MeleeDef = 0;
	m_RangeAtt = 0;
	m_RangeDef = 0;
	m_TmpBuf[0] = '\0';
}

CNkMyPet::CNkMyPet(CClrAlloc *pClrAlloc) : CNkPet(pClrAlloc)
{
	m_CombatMobId = -1;
	m_CombatChaId = -1;
	m_CombatPetId = -1;
	m_MasterRange = MYPET_MASTER_RANGE_DEFAULT;
	m_Vital = 100;
	m_MaxVital = 100;
	m_AttackVital = 100;
	InitSocialTime();
	m_MeleeAtt = 0;
	m_MeleeDef = 0;
	m_RangeAtt = 0;
	m_RangeDef = 0;
	m_TmpBuf[0] = '\0';
	m_UseSkillTime = 0;
}

CNkMyPet::~CNkMyPet()
{}

char *CNkMyPet::GetPieceName(int type)
{
	//  [11/23/2009 ppmmjj83] 장바구니 글씨가 않나와서 추가
	if( type == 6 ) // 장바구니 버튼
		return (char*)G_STRING(IDS_ITEM_BASKET);

	if ( type < 0 || type >= PET_PIECE_NUM)
		return NULL;

	if (m_BodyItem_Vnum[type] >= 0)
	{
		return (char*)GET_ITEM_NAME_VNUM(m_BodyItem_Vnum[type]);
	}
	else
		return NULL;
}

BOOL CNkMyPet::AttackNearestMob()
{
	// 공격 명령이 왔는데, 몹이 이동중이면 이동최종 목적지로 몹을 위치시킨다.
	if (!IsStanding() || (g_MyPetStatus.mode != MYPET_MODE_COMBAT && g_MyPetStatus.mode != MYPET_MODE_RANGECOMBAT && g_MyPetStatus.mode != MYPET_MODE_GUARD))
		return FALSE;

	if (m_CombatMobId >= 0 || m_CombatChaId >= 0 || m_CombatPetId >= 0) // 가까운넘 공격하는 게 아니고, 지정한 넘 공격해야 한다.
		return FALSE;

	CNkMob *pMob, *pTargetMob = NULL;
	LHMobMap_it prMob;

	float distance = 10000.0f, dist_temp;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		// 공격 대상 찾기.
		pMob = (*prMob).second;

		if (!(pMob->IsDead()) && !(pMob->m_Flag & NPC_SHOPKEEPER) && !(pMob->m_Flag & NPC_GUARD) &&
				!(pMob->m_Flag & NPC_FISH) && !(pMob->m_Flag & NPC_SUMMON) && !(pMob->m_Flag & NPC_NORADER) &&
				pMob->m_nMobNum != MONSTER_CONTROL_DAMAGE && pMob->m_nMobNum != MONSTER_CONTROL_GUARD &&
				!(pMob->m_Flag & NPC_QUEST) && /*참회의 갑옷쥐(226) 공격 금지*/pMob->m_nMobNum != 226
				&& pMob->m_nMobNum != 420 && pMob->m_nMobNum != 421 && pMob->m_nMobNum != 422 && pMob->m_nMobNum != 423 && pMob->m_nMobNum != 424

				&& pMob->m_nMobNum != 212
				&& pMob->m_nMobNum != 213
				&& pMob->m_nMobNum != 214
				&& pMob->m_nMobNum != 215
				&& pMob->m_nMobNum != 216
				&& pMob->m_nMobNum != 225
				&& pMob->m_nMobNum != 256
				&& pMob->m_nMobNum != 369

				&& pMob->m_nMobNum != 367		// 하이브리더 디바인스톤 by.Sol9 [2012.07.19]
				&& pMob->m_nMobNum != 368
				&& pMob->m_nMobNum != 778
				&& pMob->m_nMobNum != 5080
				&& pMob->m_nMobNum != 5081
				&& pMob->m_nMobNum != 5085
				&& pMob->m_nMobNum != 5086

				&& pMob->m_nMobNum != 657
		   )
		{
			if (pCMyApp->m_pUIMgr->IsNotBlocked(m_wx, m_wz, pMob->m_wx, pMob->m_wz))
			{
				dist_temp = DistPlToPl(m_wx, m_wz, pMob->m_wx, pMob->m_wz);

				if (dist_temp < distance)
				{
					if (DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pMob->m_wx, pMob->m_wz) <= m_MasterRange)
					{
						pTargetMob = pMob;
						distance = dist_temp;
					}
				}
			}
		}
	}

	if (pTargetMob)
	{
		if (distance <= PET_SHORT_ATT_RANGE)
		{
			if( UsePetSkill(0,pTargetMob->m_nVirId) ) // 스킬 발동 확률을 체크해서 확률에 들어왔으면 스킬로 사용.
			{} // 함수 안에서 패킷 보내고 모션 바꾸고 다한다.
			else // 아니면 일반 공격.
			{
				Attack(pTargetMob->m_wx, pTargetMob->m_wz);
				m_AttackMobId = pTargetMob->m_nVirId;
				SetCombatMob(pTargetMob->m_nVirId);

				sprintf(m_TmpBuf, "p_attack npc %d\n", pTargetMob->m_nVirId);
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(m_TmpBuf);
			}
			return TRUE;
		}
		else
		{
			// 다가가야 하느니라!
			MoveToAttack(pTargetMob->m_wx, pTargetMob->m_wz);
			SetCombatMob(pTargetMob->m_nVirId);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CNkMyPet::FollowMaster()
{
	if (!IsStanding())
		return FALSE;

	if (m_CombatMobId >= 0 || m_CombatChaId >= 0 || m_CombatPetId >= 0) // 때릴넘 있으면 쳐야지 어딜 따라와?
		return FALSE;

	float dist = DistPlToPl(m_wx, m_wz, g_pRoh->m_wx, g_pRoh->m_wz );

	float FollowRange = 0.0f;

	if( m_PetClass ) // 진화형.
		FollowRange = MYPET_MASTER_RANGE_FOLLOW_CLASS1;
	else // 기본형.
		FollowRange = MYPET_MASTER_RANGE_FOLLOW;

	if( dist >= FollowRange )
	{
		if (dist < MYPET_MASTER_RANGE_MAX)
			MoveToFollow(g_pRoh->m_wx, g_pRoh->m_wz);
		else
			MoveToMaster(g_pRoh->m_wx, g_pRoh->m_wz);

		m_CombatMobId = -1;
		m_CombatChaId = -1;
		m_CombatPetId = -1;
		return TRUE;
	}
	return FALSE;
}

BOOL CNkMyPet::AttackCombatMob()
{
	CNkMob *pMob;
	CNkCharacter *pCha;
	CNkPet *pPet;

	if ( m_CombatMobId < 0 && m_CombatChaId < 0 && m_CombatPetId < 0 )
		return FALSE;

	if (!IsStanding())
		return FALSE;

	if (m_CombatMobId >= 0
			&& ( pMob = pCMyApp->m_pUIMgr->FindMobById(m_CombatMobId) )
			&& !pMob->IsDead()
			&& !(pMob->m_Flag & NPC_SHOPKEEPER)
			&& !(pMob->m_Flag & NPC_GUARD)
			&& !(pMob->m_Flag & NPC_FISH)
			&& !(pMob->m_Flag & NPC_SUMMON)
			&& !(pMob->m_Flag & NPC_NORADER)
			&& pMob->m_nMobNum != MONSTER_CONTROL_DAMAGE
			&& pMob->m_nMobNum != MONSTER_CONTROL_GUARD
			&& !(pMob->m_Flag & NPC_QUEST)
			&& pMob->m_nMobNum != 226
			&& pMob->m_nMobNum != 420
			&& pMob->m_nMobNum != 421
			&& pMob->m_nMobNum != 422
			&& pMob->m_nMobNum != 423
			&& pMob->m_nMobNum != 424
			&& pMob->m_nMobNum != 212
			&& pMob->m_nMobNum != 213
			&& pMob->m_nMobNum != 214
			&& pMob->m_nMobNum != 215
			&& pMob->m_nMobNum != 216
			&& pMob->m_nMobNum != 225
			&& pMob->m_nMobNum != 256
			&& pMob->m_nMobNum != 369
			&& pMob->m_nMobNum != 367		// 하이브리더 디바인스톤 by.Sol9 [2012.07.19]
			&& pMob->m_nMobNum != 368
			&& pMob->m_nMobNum != 778
			&& pMob->m_nMobNum != 5080
			&& pMob->m_nMobNum != 5081
			&& pMob->m_nMobNum != 5085
			&& pMob->m_nMobNum != 5086
			&& pMob->m_nMobNum != 657
	   )
	{
		if( g_bNewTerrain )
		{
			if( pCMyApp->GetLandPtr()->GetHFHeight(pMob->m_wx, pMob->m_wz) <
					CNKObjQuadNode::GetHeightOnWater( pMob->m_wz, pMob->m_wx ) )
				return FALSE;
		}
		else
		{
			if( pCMyApp->GetLandPtr()->GetHFHeight(pMob->m_wx, pMob->m_wz) < WATER_HEIGHT )
				return FALSE;
		}

		float dist = DistPlToPl(m_wx, m_wz, pMob->m_wx, pMob->m_wz);

		if(pMob->m_Flag & NPC_QUEST)
			return FALSE;

		if (dist <= PET_SHORT_ATT_RANGE)
		{
			if( UsePetSkill(0,pMob->m_nVirId) ) // 스킬 발동 확률을 체크해서 확률에 들어왔으면 스킬로 사용.
			{} // 함수 안에서 패킷 보내고 모션 바꾸고 다한다.
			else // 아니면 일반 공격.
			{
				Attack(pMob->m_wx, pMob->m_wz);
				m_AttackMobId = pMob->m_nVirId;

				sprintf(m_TmpBuf, "p_attack npc %d\n", pMob->m_nVirId);

				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(m_TmpBuf);
			}

			if (g_MyPetStatus.mode == MYPET_MODE_FOLLOW) // 따라다니는 모드일때는 한대치고 쉰다.
				m_CombatMobId = -1;

			return TRUE;
		}
		else
		{
			// 다가가야 하느니라!
			MoveToAttack(pMob->m_wx, pMob->m_wz);
			return TRUE;
		}
	}
	else if (m_CombatChaId >= 0 &&
			 (pCha = pCMyApp->m_pUIMgr->FindChaById(m_CombatChaId, FALSE)) && !(pCha->IsDead()))
	{
		if( g_bNewTerrain )
		{
			if( pCMyApp->GetLandPtr()->GetHFHeight(pCha->m_wx, pCha->m_wz) <
					CNKObjQuadNode::GetHeightOnWater( pCha->m_wz, pCha->m_wx ) )
				return FALSE;
		}
		else
		{
			if( pCMyApp->GetLandPtr()->GetHFHeight(pCha->m_wx, pCha->m_wz ) < WATER_HEIGHT )
				return FALSE;
		}

		float dist = DistPlToPl(m_wx, m_wz, pCha->m_wx, pCha->m_wz);

		if (dist <= PET_SHORT_ATT_RANGE)
		{
			if( UsePetSkill(1,pCha->m_nCharIndex) ) // 스킬 발동 확률을 체크해서 확률에 들어왔으면 스킬로 사용.
			{} // 함수 안에서 패킷 보내고 모션 바꾸고 다한다.
			else // 아니면 일반 공격.
			{
				Attack(pCha->m_wx, pCha->m_wz);
				m_AttackChaId = pCha->m_nCharIndex;

				sprintf(m_TmpBuf, "p_attack cha %d\n", pCha->m_nCharIndex);
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(m_TmpBuf);
			}

			if (g_MyPetStatus.mode == MYPET_MODE_FOLLOW) // 따라다니는 모드일때는 한대치고 쉰다.
				m_CombatChaId = -1;

			return TRUE;
		}
		else
		{
			// 다가가야 하느니라!
			MoveToAttack(pCha->m_wx, pCha->m_wz);
			return TRUE;
		}
	}
	else if (m_CombatPetId >= 0 &&
			 (pPet = pCMyApp->m_pUIMgr->FindPetById(m_CombatPetId, FALSE)) && !(pPet->IsDead()))
	{
		if( g_bNewTerrain )
		{
			if( pCMyApp->GetLandPtr()->GetHFHeight(pPet->m_wx, pPet->m_wz) <
					CNKObjQuadNode::GetHeightOnWater( pPet->m_wz, pPet->m_wx ) )
				return FALSE;
		}
		else
		{
			if( pCMyApp->GetLandPtr()->GetHFHeight(pPet->m_wx, pPet->m_wz) < WATER_HEIGHT )
				return FALSE;
		}

		float dist = DistPlToPl(m_wx, m_wz, pPet->m_wx, pPet->m_wz);

		if (dist <= PET_SHORT_ATT_RANGE)
		{
			Attack(pPet->m_wx, pPet->m_wz);
			m_AttackPetId = pPet->m_nVirId;
			sprintf(m_TmpBuf, "p_attack pet %d\n", pPet->m_nVirId);
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(m_TmpBuf);

			if (g_MyPetStatus.mode == MYPET_MODE_FOLLOW) // 따라다니는 모드일때는 한대치고 쉰다.
				m_CombatPetId = -1;

			return TRUE;
		}
		else
		{
			// 다가가야 하느니라!
			MoveToAttack(pPet->m_wx, pPet->m_wz);
			return TRUE;
		}
	}
	else
	{
		m_CombatMobId = -1;
		m_CombatChaId = -1;
		m_CombatPetId = -1;
	}

	return FALSE;
}

BOOL CNkMyPet::UsePetSkill( int DestType, int Index ) // DestType은 몹인지 캐릭터인지(펫한테는 스킬안씀)
{
	if(	timeGetTime() -	m_UseSkillTime < 5000 ) // 스킬이 일정시간안에 연속으로 나가는거 방지
		return FALSE;

	int SkillKindRate = rand()%10000; // 스킬 사용 확률.
	int UseSkillNum = -1;

	if( SkillKindRate <= 5000 ) // 일반공격. 스킬 사용 실패.
	{
		return FALSE;
	}
	else if( SkillKindRate <= 6500 ) // 마비 or 수호 15%
	{
		UseSkillNum = 1;
	}
	else if( SkillKindRate <= 8000 ) // 콜드 or 수호 15%
	{
		UseSkillNum = 2;
	}
	else if( SkillKindRate <= 9500 ) // 블랙홀 or 생명 15%
	{
		UseSkillNum = 3;
	}
	else // 포이즌 or 브레스 5%
	{
		UseSkillNum = 0;
	}

	if( UseSkillNum != -1 && g_MyPetStatus.m_SkillLevel[UseSkillNum] ) // 배운 스킬만 발동.
	{
		// 발동!
		int Temp1 = 0, Temp2 = 0, Temp3 = 0;
		CNkMob *TempMob = NULL;
		CNkCharacter *TempCha = NULL;

		int SkillIndex = 0;

		if( m_PetClass > 0 )
			SkillIndex = g_PetSkillGroup[m_PetClass-1][UseSkillNum]; // 실제 서버와 사용하는 인덱스로 전환.

		sprintf(m_TmpBuf, "p_skill %d", SkillIndex);

		//				SetMotionState( PET_USE_SKILL1 ); // 모션도 바꿔줘야한다. // 모션 여기서 바꾸지말자.

		if( DestType == 0 ) // 몬스터 타겟.
			sprintf(m_TmpBuf, "%s n", m_TmpBuf);
		else if( DestType == 1 ) // 캐릭터 타겟.
			sprintf(m_TmpBuf, "%s c", m_TmpBuf);
		else
			return FALSE; // 타겟이 이상하게 오면 안되지...

		switch( SkillIndex )
		{
		case 0: // 포이즌(진화 1단계 방어형 스킬)

			// 다중 타겟 스킬이다.(3타겟까지)
			Temp1 = 2; // 맥스 타겟.
			Temp2 = 30; // 범위.

			if( DestType == 0 ) // 몬스터 타겟.
			{
				TempMob = pCMyApp->m_pUIMgr->FindMobById( Index );
				Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempMob,  0, Temp2, true, Temp1 );
				sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index);

				for( int j = 0 ; j < Temp1 ; ++j )
				{
					sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetMob[j]->m_nVirId);
				}
			}
			else if( DestType == 1 ) // 캐릭터 타겟.
			{
				TempCha = pCMyApp->m_pUIMgr->FindChaById( Index, FALSE );
				Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempCha,  0, Temp2, false, Temp1 );

				for( int j = 0 ; j < Temp1 ; ++j )
				{
					sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetCha[j]->m_nCharIndex);
				}
			}
			else
				return FALSE; // 타겟이 이상하게 오면 안되지...

			break;

		case 1: // 마비(진화 1단계 방어형 스킬)
		case 2: // 콜드(진화 1단계 방어형 스킬)
		case 3: // 블랙홀(진화 1단계 방어형 스킬)
		case 4: // 브레스(진화 1단계 공격형 스킬)
			sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index );
			break;

		case 5: // 수호(진화 1단계 공격형 스킬)
		case 6: // 보호(진화 1단계 공격형 스킬)
		case 7: // 생명(진화 1단계 공격형 스킬)
			sprintf(m_TmpBuf, "p_skill %d", SkillIndex); // 이녀석은 스킬 번호만 보내므로 패킷을 다시 처음부터 덮어쓴다.
			break;
		}

		sprintf(m_TmpBuf, "%s\n", m_TmpBuf);

		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage(m_TmpBuf);

		// 마지막으로 스킬이 나간 시간을 기억해둔다.
		m_UseSkillTime = timeGetTime();

		return TRUE;

	}

	/*
		for( int i = 0 ; i < PET_SKILL_MAX ; ++i ) // 현재 있는 스킬들을 전부 돌면서 체크.
		{
			if( g_MyPetStatus.m_SkillLevel[i] ) // 우선은 배운 스킬이어야한다. (테스트를 위한 임시 주석처리)
			{
				if( (rand()%10000) < 500 ) // 일정 확률 체크.
				{
					// 발동!
					int Temp1 = 0, Temp2 = 0, Temp3 = 0;
					CNkMob *TempMob = NULL;
					CNkCharacter *TempCha = NULL;

					int SkillIndex = 0;

					if( m_PetClass > 0 )
						SkillIndex = g_PetSkillGroup[m_PetClass-1][i]; // 실제 서버와 사용하는 인덱스로 전환.

					sprintf(m_TmpBuf, "p_skill %d", SkillIndex);

	//				SetMotionState( PET_USE_SKILL1 ); // 모션도 바꿔줘야한다. // 모션 여기서 바꾸지말자.

					if( DestType == 0 ) // 몬스터 타겟.
						sprintf(m_TmpBuf, "%s n", m_TmpBuf);
					else if( DestType == 1 ) // 캐릭터 타겟.
						sprintf(m_TmpBuf, "%s c", m_TmpBuf);
					else
						return FALSE; // 타겟이 이상하게 오면 안되지...

					switch( SkillIndex )
					{
					case 1: // 포이즌(진화 1단계 방어형 스킬)

						// 다중 타겟 스킬이다.(3타겟까지)
						Temp1 = 2; // 맥스 타겟.
						Temp2 = 30; // 범위.

						if( DestType == 0 ) // 몬스터 타겟.
						{
							TempMob = pCMyApp->m_pUIMgr->FindMobById( Index );
							Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempMob,  0, Temp2, true, Temp1 );
							sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index);

							for( int j = 0 ; j < Temp1 ; ++j )
							{
								sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetMob[j]->m_nVirId);
							}
						}
						else if( DestType == 1 ) // 캐릭터 타겟.
						{
							TempCha = pCMyApp->m_pUIMgr->FindChaById( Index, FALSE );
							Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempCha,  0, Temp2, false, Temp1 );

							for( int j = 0 ; j < Temp1 ; ++j )
							{
								sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetCha[j]->m_nVirId);
							}
						}
						else
							return FALSE; // 타겟이 이상하게 오면 안되지...

						break;

					case 2: // 마비(진화 1단계 방어형 스킬)
					case 3: // 콜드(진화 1단계 방어형 스킬)
					case 4: // 블랙홀(진화 1단계 방어형 스킬)
					case 5: // 브레스(진화 1단계 공격형 스킬)
						sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index );
						break;

					case 6: // 수호(진화 1단계 공격형 스킬)
					case 7: // 보호(진화 1단계 공격형 스킬)
					case 8: // 생명(진화 1단계 공격형 스킬)
						sprintf(m_TmpBuf, "p_skill %d", SkillIndex); // 이녀석은 스킬 번호만 보내므로 패킷을 다시 처음부터 덮어쓴다.
						break;
					}

					sprintf(m_TmpBuf, "%s\n", m_TmpBuf);

					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage(m_TmpBuf);

					// 마지막으로 스킬이 나간 시간을 기억해둔다.
					m_UseSkillTime = timeGetTime();

					return TRUE;
				}
			}
		}
	*/
	return FALSE;
}

BOOL CNkMyPet::SetCombatMob(int nMobId, int nChaId, int nPetId)
{
	if (g_MyPetStatus.mode == MYPET_MODE_SUPP)
	{
		if (nMobId >= 0 )
		{
			m_CombatMobId = nMobId;
			return TRUE;
		}
		else if (nChaId >= 0)
		{
			m_CombatChaId = nChaId;
			return TRUE;
		}
		else if (nPetId >= 0)
		{
			m_CombatPetId = nPetId;
			return TRUE;
		}
	}
	else if (g_MyPetStatus.mode == MYPET_MODE_COMBAT || g_MyPetStatus.mode == MYPET_MODE_FOLLOW ||
			 g_MyPetStatus.mode == MYPET_MODE_RANGECOMBAT || g_MyPetStatus.mode == MYPET_MODE_GUARD )
	{
		if (nMobId >= 0 && m_CombatMobId < 0 )
		{
			m_CombatMobId = nMobId;
			return TRUE;
		}
		else if (nChaId >= 0 && m_CombatChaId < 0)
		{
			m_CombatChaId = nChaId;
			return TRUE;
		}
		else if (nPetId >= 0 && m_CombatPetId < 0)
		{
			m_CombatPetId = nPetId;
			return TRUE;
		}
	}

	return FALSE;
}

#define SOCIAL_TIME		(3000)

BOOL CNkMyPet::Social(DWORD curTime)
{
	if( !IsStanding() ) // 진화형은 동작이 길어 중간에 끊어지고 소셜액션이 들어가는 경우가 있는거 같다. 이 체크를 한번 넣어보자.
		return FALSE;

	if (curTime - m_SocialTime > SOCIAL_TIME)
	{
		if (!(rand() % 5))
		{
			int act_num = 0;

			if (g_MyPetStatus.loyalty >= 15)
				act_num = 18;
			else if (g_MyPetStatus.loyalty >= 10)
				act_num = 19;
			else if (g_MyPetStatus.loyalty >= 5)
				act_num = 20;
			else if (g_MyPetStatus.loyalty <= -15)
				act_num = 23;
			else if (g_MyPetStatus.loyalty <= -10)
				act_num = 22;
			else if (g_MyPetStatus.loyalty <= -5)
				act_num = 21;

			if (act_num != 0)
				SetMotionState(PET_SOCIAL, act_num);

			m_SocialTime = curTime;

			return TRUE;
		}
		else
			m_SocialTime += 1000;
	}
	return FALSE;
}

void CNkMyPet::InitSocialTime(DWORD curTime)
{
	m_SocialTime = curTime;
}

void CNkMyPet::InitSocialTime()
{
	InitSocialTime(timeGetTime());
}

BOOL CNkMyPet::PickItem()
{
	if (g_MyPetStatus.mode != MYPET_MODE_PICKUP || !IsStanding())
		return FALSE;

	CItemInfo *pItemInfo, *pTargetItem = NULL;
	LHItemInfoMap_it pr;

	float distance = 10000.0f, dist_temp;
	for (pr = pCMyApp->m_pUIMgr->m_iteminfo_map.begin(); pr != pCMyApp->m_pUIMgr->m_iteminfo_map.end(); pr++)
	{
		pItemInfo = (*pr).second;

		if( pItemInfo->m_Item.m_Vnum == 1051 )		// 사과 먹자 방지
			continue;

		if (pItemInfo->m_Item.m_Dropping < 0)
		{
			dist_temp = DistPlToPl(m_wx, m_wz, pItemInfo->m_Item.m_X, pItemInfo->m_Item.m_Z);
			if (dist_temp < distance)   // 일단 펫한테 가장 가까운 걸 찾는디...
			{
				// 고것이 주인 영역 안에 있어야 한댜~~!
				if (DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, pItemInfo->m_Item.m_X, pItemInfo->m_Item.m_Z) <= m_MasterRange)
				{
					pTargetItem = pItemInfo;
					distance = dist_temp;
				}
			}
		}
	}

	if (pTargetItem)
	{
		if (distance <= (g_MyPetStatus.pick_level * 10))
		{
			dist_temp = DistPlToPl(m_wx, m_wz, pItemInfo->m_Item.m_X, pItemInfo->m_Item.m_Z);
			sprintf(m_TmpBuf, "p_social %d %d %d\n", MYPET_SOCIAL_PICKUP, (int) (pTargetItem->m_Item.m_X), (int) (pTargetItem->m_Item.m_Z));
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(m_TmpBuf);
			sprintf(m_TmpBuf, "p_pick %d\n", pTargetItem->m_Item.m_Index);
			CNkPet::Pickup(pTargetItem->m_Item.m_X, pTargetItem->m_Item.m_Z);
			return TRUE;
		}
		else
		{
			MoveToAttack(pTargetItem->m_Item.m_X, pTargetItem->m_Item.m_Z);
			return TRUE;
		}
	}

	return FALSE;
}

void CNkMyPet::SetVital(int vital, int max_vital)
{
	m_Vital = m_AttackVital = vital;
	m_MaxVital = max_vital;

	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		if( g_MyPetStatus.m_Class < 3 )
		{
			if (m_BodyItem[i] >= 0)
				m_MaxVital += g_ItemList.m_ItemArray[m_BodyItem[i]].m_PetVital;
		}
		else
		{
			if (g_MyPetStatus.m_Wearing[i] != NULL)
			{
				int protoNum = g_MyPetStatus.m_Wearing[i]->m_ProtoNum;
				m_MaxVital += g_ItemList.m_ItemArray[protoNum].m_PetVital;
			}
		}
	}
}

void CNkMyPet::SetRangeAtt(int range_att)
{
	m_RangeAtt = range_att;

	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		if( g_MyPetStatus.m_Class < 3 )
		{
			if (m_BodyItem[i] >= 0)
				m_RangeAtt += g_ItemList.m_ItemArray[m_BodyItem[i]].m_PetRangeAtt;
		}
		else
		{
			if (g_MyPetStatus.m_Wearing[i] != NULL)
			{
				int protoNum = g_MyPetStatus.m_Wearing[i]->m_ProtoNum;
				m_RangeAtt += g_ItemList.m_ItemArray[protoNum].m_PetRangeAtt;
			}
		}		
	}
}

void CNkMyPet::SetRangeDef(int range_def)
{
	m_RangeDef = range_def;

	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		if( g_MyPetStatus.m_Class < 3 )
		{
			if (m_BodyItem[i] >= 0)
				m_RangeDef += g_ItemList.m_ItemArray[m_BodyItem[i]].m_PetRangeDef;
		}
		else
		{
			if (g_MyPetStatus.m_Wearing[i] != NULL)
			{
				int protoNum = g_MyPetStatus.m_Wearing[i]->m_ProtoNum;
				m_RangeDef += g_ItemList.m_ItemArray[protoNum].m_PetRangeDef;
			}
		}		
	}
}

void CNkMyPet::SetMeleeAtt(int melee_att)
{
	m_MeleeAtt = melee_att;

	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		if( g_MyPetStatus.m_Class < 3 )
		{
			if (m_BodyItem[i] >= 0)
				m_MeleeAtt += g_ItemList.m_ItemArray[m_BodyItem[i]].m_PetMeleeAtt;
		}
		else
		{
			if (g_MyPetStatus.m_Wearing[i] != NULL)
			{
				int protoNum = g_MyPetStatus.m_Wearing[i]->m_ProtoNum;
				m_MeleeAtt += g_ItemList.m_ItemArray[protoNum].m_PetMeleeAtt;
			}
		}
	}
}

void CNkMyPet::SetMeleeDef(int melee_def)
{
	m_MeleeDef = melee_def;

	for (int i = 0; i < PET_PIECE_NUM; i++)
	{
		if( g_MyPetStatus.m_Class < 3 )
		{
			if (m_BodyItem[i] >= 0)
				m_MeleeDef += g_ItemList.m_ItemArray[m_BodyItem[i]].m_PetMeleeDef;
		}
		else
		{
			if (g_MyPetStatus.m_Wearing[i] != NULL)
			{
				int protoNum = g_MyPetStatus.m_Wearing[i]->m_ProtoNum;
				m_MeleeDef += g_ItemList.m_ItemArray[protoNum].m_PetMeleeDef;
			}
		}
	}
}

BOOL CNkMyPet::SetMode(int nMode)
{
	m_CombatMobId = -1;
	m_CombatChaId = -1;
	m_CombatPetId = -1;

	switch (nMode)
	{
	case MYPET_MODE_COMBAT:
	case MYPET_MODE_RANGECOMBAT:
	case MYPET_MODE_GUARD: // 경계모드.
		m_MasterRange = MYPET_MASTER_RANGE_COMBAT;
		break;
	case MYPET_MODE_PICKUP:
		m_MasterRange = MYPET_MASTER_RANGE_PICKUP;
		break;
	default: // case MYPET_MODE_FOLLOW: case MYPET_MODE_SUPP:
		m_MasterRange = MYPET_MASTER_RANGE_FOLLOW;
		break;
	}

	return TRUE;
}

BOOL CNkMyPet::MoveToFollow(float x, float z)
{
	float fDx = x - m_wx;
	float fDz = z - m_wz;

	float mag = (float) sqrt( (double) (fDx*fDx + fDz*fDz));

	if (mag < g_EPSILON) // 라비용 도망 버그 추정 (mag < g_EPSILON)=> mag값이 1.0f보다 작을때 문제 발생 <= 10.0f
		return FALSE;

	if( g_MyPetStatus.speed == 0.0f ) // 라비용 파츠 삐꾸 버그 땜빵. 파츠가 삐꾸라서 스피드가 0으로 와서 워크패킷이 무수히 가는 문제를 막는다. ///070723 일본버그리포트 수정 때 같이 수정.
		return FALSE;

	fDx -= fDx / mag * 10.0f;
	fDz -= fDz / mag * 10.0f;


	float fIx = fDx / mag * g_MyPetStatus.speed * 10;
	float fIz = fDz / mag * g_MyPetStatus.speed * 10;

	if (fDx > 0)
	{
		if (fIx > fDx)
			fIx = fDx;
	}
	else
	{
		if (fIx < fDx)
			fIx = fDx;
	}
	if (fDz > 0)
	{
		if (fIz > fDz)
			fIz = fDz;
	}
	else
	{
		if (fIz < fDz)
			fIz = fDz;
	}

	float tofX = m_wx + fIx;
	float tofZ = m_wz + fIz;
	Move(tofX, tofZ);
	sprintf(m_TmpBuf, "p_walk %d %d 100 0\n",
			(int) (tofX), (int) (tofZ));
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(m_TmpBuf);

	return TRUE;
}

BOOL CNkMyPet::MoveToMaster(float x, float z)
{
	float fDx = x - m_wx;
	float fDz = z - m_wz;

	float tofX = x - fDx/5.0f;
	float tofZ = z - fDz/5.0f;

	Move(tofX, tofZ);
	sprintf(m_TmpBuf, "p_walk %d %d 100 0\n",
			(int) (tofX), (int) (tofZ));
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(m_TmpBuf);

	return TRUE;
}

BOOL CNkMyPet::MoveToAttack(float x, float z)
{
	float fDx = x - m_wx;
	float fDz = z - m_wz;

	float mag = (float) sqrt( (double) (fDx*fDx + fDz*fDz));

	if (mag < g_EPSILON)
		return FALSE;

	if( g_MyPetStatus.speed == 0.0f ) // 라비용 파츠 삐꾸 버그 땜빵. 파츠가 삐꾸라서 스피드가 0으로 와서 워크패킷이 무수히 가는 문제를 막는다. ///070723 일본버그리포트 수정 때 같이 수정.
		return FALSE;

	fDx -= fDx / mag * 19.0f;
	fDz -= fDz / mag * 19.0f;

	float fIx = fDx / mag * g_MyPetStatus.speed * 10;
	float fIz = fDz / mag * g_MyPetStatus.speed * 10;
	if (fDx > 0)
	{
		if (fIx > fDx)
			fIx = fDx;
	}
	else
	{
		if (fIx < fDx)
			fIx = fDx;
	}
	if (fDz > 0)
	{
		if (fIz > fDz)
			fIz = fDz;
	}
	else
	{
		if (fIz < fDz)
			fIz = fDz;
	}

	float tofX = m_wx + fIx;
	float tofZ = m_wz + fIz;
	Move(tofX, tofZ);

	sprintf(m_TmpBuf, "p_walk %d %d 100 0\n",
			(int) (tofX), (int) (tofZ));
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(m_TmpBuf);

	return TRUE;
}

void CNkMyPet::SetBody(int horn, int head, int body, int wings, int legs, int tail)
{
	CNkPet::SetBody(horn, head, body, wings, legs, tail);

	if (m_PetClass >= 3)
	{	
		int item_idx = g_ItemList.FindItem(horn);
		if (item_idx >= 0)
			m_BodyItem_Class3[PET_PIECE_HORN] = item_idx;

		item_idx = g_ItemList.FindItem(head);
		if (item_idx >= 0)
			m_BodyItem_Class3[PET_PIECE_HEAD] = item_idx;

		item_idx = g_ItemList.FindItem(body);
		if (item_idx >= 0)
			m_BodyItem_Class3[PET_PIECE_BODY] = item_idx;

		item_idx = g_ItemList.FindItem(wings);
		if (item_idx >= 0)
			m_BodyItem_Class3[PET_PIECE_WINGS] = item_idx;


		item_idx = g_ItemList.FindItem(legs);
		if (item_idx >= 0)
			m_BodyItem_Class3[PET_PIECE_LEGS] = item_idx;

		item_idx = g_ItemList.FindItem(tail);
		if (item_idx >= 0)
			m_BodyItem_Class3[PET_PIECE_TAIL] = item_idx;




		g_MyPetStatus.SetEquip(0, horn);
		g_MyPetStatus.SetEquip(1, head);
		g_MyPetStatus.SetEquip(2, body);
		g_MyPetStatus.SetEquip(3, wings);
		g_MyPetStatus.SetEquip(4, legs);
		g_MyPetStatus.SetEquip(5, tail);
	}
}
