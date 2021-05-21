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
	// ���� �ֿϵ��� ����â ���� ���� �ʱ�ȭ
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

	m_Class = 0; // ������ ��ȭ Ÿ��(�⺻��0,�����1,������2)
	for( int i = 0 ; i < PET_SKILL_MAX ; ++i )
		m_SkillLevel[i] = 0; // �ڽ��� ��ų�� ������� ����.

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

	m_Class = 0; // ������ ��ȭ Ÿ��(�⺻��0,�����1,������2)
	for( int i = 0 ; i < PET_SKILL_MAX ; ++i )
		m_SkillLevel[i] = 0; // �ڽ��� ��ų�� ������� ����.

	memset(m_Wearing, 0, sizeof(m_Wearing));
}

BOOL CPetStatus::SetMode(int nMode)
{
	mode = nMode;

	if (pCMyApp->m_pMyPet)
		pCMyApp->m_pMyPet->SetMode(nMode);

	return TRUE;
}


// �ֿϵ��� ���� ���� ����
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
//-- Lyul / �����Ϸ�
//-- ���� �ٱ��ϰ� ���� ��� max_food �� 6���� ����.
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
	// [3/17/2009 D.K ] : �ڵ� ����. �����ӽ� �ٱ��ϰ� ���� �༮�ε��� �ִ� �༮���� �νĹ���.
	// �� �ƹ��� ��������..=_=;;;;;
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

void CPetStatus::SetClassType( int Class ) // ���� Ŭ����(�⺻0, �����1, ������2)
{
	m_Class = Class;
}

void CPetStatus::SetSkill( int SkillNum, int Level ) // ������ ��� ��ų ����.
{
	if( m_Class < 1 ) // ��ȭ������ �ƴϸ� ��ų ������ �ȵȴ�.(�� ��ų �������ͽ��� �������� Ŭ���� Ÿ�� ������ �����;��Ѵ�.)
		return;

	// ��ų ��ȣ�� 0 ~ PET_SKILL_MAX ������ �ε���ġ�� �ٲ�����Ѵ�.
	for( int i = 0 ; i < PET_SKILL_MAX ; ++i )
	{
		if( g_PetSkillGroup[m_Class-1][i] == SkillNum ) // �´� ��ų�� ã�Ƽ�...
		{
			m_SkillLevel[i] = Level; // �ڽ��� ��ų�� ������� ����.
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
	//  [11/23/2009 ppmmjj83] ��ٱ��� �۾��� �ʳ��ͼ� �߰�
	if( type == 6 ) // ��ٱ��� ��ư
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
	// ���� ����� �Դµ�, ���� �̵����̸� �̵����� �������� ���� ��ġ��Ų��.
	if (!IsStanding() || (g_MyPetStatus.mode != MYPET_MODE_COMBAT && g_MyPetStatus.mode != MYPET_MODE_RANGECOMBAT && g_MyPetStatus.mode != MYPET_MODE_GUARD))
		return FALSE;

	if (m_CombatMobId >= 0 || m_CombatChaId >= 0 || m_CombatPetId >= 0) // ������ �����ϴ� �� �ƴϰ�, ������ �� �����ؾ� �Ѵ�.
		return FALSE;

	CNkMob *pMob, *pTargetMob = NULL;
	LHMobMap_it prMob;

	float distance = 10000.0f, dist_temp;
	for (prMob = pCMyApp->m_pUIMgr->m_mob_map.begin(); prMob != pCMyApp->m_pUIMgr->m_mob_map.end(); prMob++)
	{
		// ���� ��� ã��.
		pMob = (*prMob).second;

		if (!(pMob->IsDead()) && !(pMob->m_Flag & NPC_SHOPKEEPER) && !(pMob->m_Flag & NPC_GUARD) &&
				!(pMob->m_Flag & NPC_FISH) && !(pMob->m_Flag & NPC_SUMMON) && !(pMob->m_Flag & NPC_NORADER) &&
				pMob->m_nMobNum != MONSTER_CONTROL_DAMAGE && pMob->m_nMobNum != MONSTER_CONTROL_GUARD &&
				!(pMob->m_Flag & NPC_QUEST) && /*��ȸ�� ������(226) ���� ����*/pMob->m_nMobNum != 226
				&& pMob->m_nMobNum != 420 && pMob->m_nMobNum != 421 && pMob->m_nMobNum != 422 && pMob->m_nMobNum != 423 && pMob->m_nMobNum != 424

				&& pMob->m_nMobNum != 212
				&& pMob->m_nMobNum != 213
				&& pMob->m_nMobNum != 214
				&& pMob->m_nMobNum != 215
				&& pMob->m_nMobNum != 216
				&& pMob->m_nMobNum != 225
				&& pMob->m_nMobNum != 256
				&& pMob->m_nMobNum != 369

				&& pMob->m_nMobNum != 367		// ���̺긮�� ����ν��� by.Sol9 [2012.07.19]
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
			if( UsePetSkill(0,pTargetMob->m_nVirId) ) // ��ų �ߵ� Ȯ���� üũ�ؼ� Ȯ���� �������� ��ų�� ���.
			{} // �Լ� �ȿ��� ��Ŷ ������ ��� �ٲٰ� ���Ѵ�.
			else // �ƴϸ� �Ϲ� ����.
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
			// �ٰ����� �ϴ��϶�!
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

	if (m_CombatMobId >= 0 || m_CombatChaId >= 0 || m_CombatPetId >= 0) // ������ ������ �ľ��� ��� �����?
		return FALSE;

	float dist = DistPlToPl(m_wx, m_wz, g_pRoh->m_wx, g_pRoh->m_wz );

	float FollowRange = 0.0f;

	if( m_PetClass ) // ��ȭ��.
		FollowRange = MYPET_MASTER_RANGE_FOLLOW_CLASS1;
	else // �⺻��.
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
			&& pMob->m_nMobNum != 367		// ���̺긮�� ����ν��� by.Sol9 [2012.07.19]
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
			if( UsePetSkill(0,pMob->m_nVirId) ) // ��ų �ߵ� Ȯ���� üũ�ؼ� Ȯ���� �������� ��ų�� ���.
			{} // �Լ� �ȿ��� ��Ŷ ������ ��� �ٲٰ� ���Ѵ�.
			else // �ƴϸ� �Ϲ� ����.
			{
				Attack(pMob->m_wx, pMob->m_wz);
				m_AttackMobId = pMob->m_nVirId;

				sprintf(m_TmpBuf, "p_attack npc %d\n", pMob->m_nVirId);

				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(m_TmpBuf);
			}

			if (g_MyPetStatus.mode == MYPET_MODE_FOLLOW) // ����ٴϴ� ����϶��� �Ѵ�ġ�� ����.
				m_CombatMobId = -1;

			return TRUE;
		}
		else
		{
			// �ٰ����� �ϴ��϶�!
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
			if( UsePetSkill(1,pCha->m_nCharIndex) ) // ��ų �ߵ� Ȯ���� üũ�ؼ� Ȯ���� �������� ��ų�� ���.
			{} // �Լ� �ȿ��� ��Ŷ ������ ��� �ٲٰ� ���Ѵ�.
			else // �ƴϸ� �Ϲ� ����.
			{
				Attack(pCha->m_wx, pCha->m_wz);
				m_AttackChaId = pCha->m_nCharIndex;

				sprintf(m_TmpBuf, "p_attack cha %d\n", pCha->m_nCharIndex);
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(m_TmpBuf);
			}

			if (g_MyPetStatus.mode == MYPET_MODE_FOLLOW) // ����ٴϴ� ����϶��� �Ѵ�ġ�� ����.
				m_CombatChaId = -1;

			return TRUE;
		}
		else
		{
			// �ٰ����� �ϴ��϶�!
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

			if (g_MyPetStatus.mode == MYPET_MODE_FOLLOW) // ����ٴϴ� ����϶��� �Ѵ�ġ�� ����.
				m_CombatPetId = -1;

			return TRUE;
		}
		else
		{
			// �ٰ����� �ϴ��϶�!
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

BOOL CNkMyPet::UsePetSkill( int DestType, int Index ) // DestType�� ������ ĳ��������(�����״� ��ų�Ⱦ�)
{
	if(	timeGetTime() -	m_UseSkillTime < 5000 ) // ��ų�� �����ð��ȿ� �������� �����°� ����
		return FALSE;

	int SkillKindRate = rand()%10000; // ��ų ��� Ȯ��.
	int UseSkillNum = -1;

	if( SkillKindRate <= 5000 ) // �Ϲݰ���. ��ų ��� ����.
	{
		return FALSE;
	}
	else if( SkillKindRate <= 6500 ) // ���� or ��ȣ 15%
	{
		UseSkillNum = 1;
	}
	else if( SkillKindRate <= 8000 ) // �ݵ� or ��ȣ 15%
	{
		UseSkillNum = 2;
	}
	else if( SkillKindRate <= 9500 ) // ��Ȧ or ���� 15%
	{
		UseSkillNum = 3;
	}
	else // ������ or �극�� 5%
	{
		UseSkillNum = 0;
	}

	if( UseSkillNum != -1 && g_MyPetStatus.m_SkillLevel[UseSkillNum] ) // ��� ��ų�� �ߵ�.
	{
		// �ߵ�!
		int Temp1 = 0, Temp2 = 0, Temp3 = 0;
		CNkMob *TempMob = NULL;
		CNkCharacter *TempCha = NULL;

		int SkillIndex = 0;

		if( m_PetClass > 0 )
			SkillIndex = g_PetSkillGroup[m_PetClass-1][UseSkillNum]; // ���� ������ ����ϴ� �ε����� ��ȯ.

		sprintf(m_TmpBuf, "p_skill %d", SkillIndex);

		//				SetMotionState( PET_USE_SKILL1 ); // ��ǵ� �ٲ�����Ѵ�. // ��� ���⼭ �ٲ�������.

		if( DestType == 0 ) // ���� Ÿ��.
			sprintf(m_TmpBuf, "%s n", m_TmpBuf);
		else if( DestType == 1 ) // ĳ���� Ÿ��.
			sprintf(m_TmpBuf, "%s c", m_TmpBuf);
		else
			return FALSE; // Ÿ���� �̻��ϰ� ���� �ȵ���...

		switch( SkillIndex )
		{
		case 0: // ������(��ȭ 1�ܰ� ����� ��ų)

			// ���� Ÿ�� ��ų�̴�.(3Ÿ�ٱ���)
			Temp1 = 2; // �ƽ� Ÿ��.
			Temp2 = 30; // ����.

			if( DestType == 0 ) // ���� Ÿ��.
			{
				TempMob = pCMyApp->m_pUIMgr->FindMobById( Index );
				Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempMob,  0, Temp2, true, Temp1 );
				sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index);

				for( int j = 0 ; j < Temp1 ; ++j )
				{
					sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetMob[j]->m_nVirId);
				}
			}
			else if( DestType == 1 ) // ĳ���� Ÿ��.
			{
				TempCha = pCMyApp->m_pUIMgr->FindChaById( Index, FALSE );
				Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempCha,  0, Temp2, false, Temp1 );

				for( int j = 0 ; j < Temp1 ; ++j )
				{
					sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetCha[j]->m_nCharIndex);
				}
			}
			else
				return FALSE; // Ÿ���� �̻��ϰ� ���� �ȵ���...

			break;

		case 1: // ����(��ȭ 1�ܰ� ����� ��ų)
		case 2: // �ݵ�(��ȭ 1�ܰ� ����� ��ų)
		case 3: // ��Ȧ(��ȭ 1�ܰ� ����� ��ų)
		case 4: // �극��(��ȭ 1�ܰ� ������ ��ų)
			sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index );
			break;

		case 5: // ��ȣ(��ȭ 1�ܰ� ������ ��ų)
		case 6: // ��ȣ(��ȭ 1�ܰ� ������ ��ų)
		case 7: // ����(��ȭ 1�ܰ� ������ ��ų)
			sprintf(m_TmpBuf, "p_skill %d", SkillIndex); // �̳༮�� ��ų ��ȣ�� �����Ƿ� ��Ŷ�� �ٽ� ó������ �����.
			break;
		}

		sprintf(m_TmpBuf, "%s\n", m_TmpBuf);

		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage(m_TmpBuf);

		// ���������� ��ų�� ���� �ð��� ����صд�.
		m_UseSkillTime = timeGetTime();

		return TRUE;

	}

	/*
		for( int i = 0 ; i < PET_SKILL_MAX ; ++i ) // ���� �ִ� ��ų���� ���� ���鼭 üũ.
		{
			if( g_MyPetStatus.m_SkillLevel[i] ) // �켱�� ��� ��ų�̾���Ѵ�. (�׽�Ʈ�� ���� �ӽ� �ּ�ó��)
			{
				if( (rand()%10000) < 500 ) // ���� Ȯ�� üũ.
				{
					// �ߵ�!
					int Temp1 = 0, Temp2 = 0, Temp3 = 0;
					CNkMob *TempMob = NULL;
					CNkCharacter *TempCha = NULL;

					int SkillIndex = 0;

					if( m_PetClass > 0 )
						SkillIndex = g_PetSkillGroup[m_PetClass-1][i]; // ���� ������ ����ϴ� �ε����� ��ȯ.

					sprintf(m_TmpBuf, "p_skill %d", SkillIndex);

	//				SetMotionState( PET_USE_SKILL1 ); // ��ǵ� �ٲ�����Ѵ�. // ��� ���⼭ �ٲ�������.

					if( DestType == 0 ) // ���� Ÿ��.
						sprintf(m_TmpBuf, "%s n", m_TmpBuf);
					else if( DestType == 1 ) // ĳ���� Ÿ��.
						sprintf(m_TmpBuf, "%s c", m_TmpBuf);
					else
						return FALSE; // Ÿ���� �̻��ϰ� ���� �ȵ���...

					switch( SkillIndex )
					{
					case 1: // ������(��ȭ 1�ܰ� ����� ��ų)

						// ���� Ÿ�� ��ų�̴�.(3Ÿ�ٱ���)
						Temp1 = 2; // �ƽ� Ÿ��.
						Temp2 = 30; // ����.

						if( DestType == 0 ) // ���� Ÿ��.
						{
							TempMob = pCMyApp->m_pUIMgr->FindMobById( Index );
							Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempMob,  0, Temp2, true, Temp1 );
							sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index);

							for( int j = 0 ; j < Temp1 ; ++j )
							{
								sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetMob[j]->m_nVirId);
							}
						}
						else if( DestType == 1 ) // ĳ���� Ÿ��.
						{
							TempCha = pCMyApp->m_pUIMgr->FindChaById( Index, FALSE );
							Temp1 = pCMyApp->m_pUIMgr->FindNearTarget( m_wx, m_wz, TempCha,  0, Temp2, false, Temp1 );

							for( int j = 0 ; j < Temp1 ; ++j )
							{
								sprintf( m_TmpBuf, "%s %d", m_TmpBuf, g_pTargetCha[j]->m_nVirId);
							}
						}
						else
							return FALSE; // Ÿ���� �̻��ϰ� ���� �ȵ���...

						break;

					case 2: // ����(��ȭ 1�ܰ� ����� ��ų)
					case 3: // �ݵ�(��ȭ 1�ܰ� ����� ��ų)
					case 4: // ��Ȧ(��ȭ 1�ܰ� ����� ��ų)
					case 5: // �극��(��ȭ 1�ܰ� ������ ��ų)
						sprintf( m_TmpBuf, "%s %d", m_TmpBuf, Index );
						break;

					case 6: // ��ȣ(��ȭ 1�ܰ� ������ ��ų)
					case 7: // ��ȣ(��ȭ 1�ܰ� ������ ��ų)
					case 8: // ����(��ȭ 1�ܰ� ������ ��ų)
						sprintf(m_TmpBuf, "p_skill %d", SkillIndex); // �̳༮�� ��ų ��ȣ�� �����Ƿ� ��Ŷ�� �ٽ� ó������ �����.
						break;
					}

					sprintf(m_TmpBuf, "%s\n", m_TmpBuf);

					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage(m_TmpBuf);

					// ���������� ��ų�� ���� �ð��� ����صд�.
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
	if( !IsStanding() ) // ��ȭ���� ������ ��� �߰��� �������� �ҼȾ׼��� ���� ��찡 �ִ°� ����. �� üũ�� �ѹ� �־��.
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

		if( pItemInfo->m_Item.m_Vnum == 1051 )		// ��� ���� ����
			continue;

		if (pItemInfo->m_Item.m_Dropping < 0)
		{
			dist_temp = DistPlToPl(m_wx, m_wz, pItemInfo->m_Item.m_X, pItemInfo->m_Item.m_Z);
			if (dist_temp < distance)   // �ϴ� ������ ���� ����� �� ã�µ�...
			{
				// ����� ���� ���� �ȿ� �־�� �Ѵ�~~!
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
	case MYPET_MODE_GUARD: // �����.
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

	if (mag < g_EPSILON) // ���� ���� ���� ���� (mag < g_EPSILON)=> mag���� 1.0f���� ������ ���� �߻� <= 10.0f
		return FALSE;

	if( g_MyPetStatus.speed == 0.0f ) // ���� ���� �߲� ���� ����. ������ �߲ٶ� ���ǵ尡 0���� �ͼ� ��ũ��Ŷ�� ������ ���� ������ ���´�. ///070723 �Ϻ����׸���Ʈ ���� �� ���� ����.
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

	if( g_MyPetStatus.speed == 0.0f ) // ���� ���� �߲� ���� ����. ������ �߲ٶ� ���ǵ尡 0���� �ͼ� ��ũ��Ŷ�� ������ ���� ������ ���´�. ///070723 �Ϻ����׸���Ʈ ���� �� ���� ����.
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
