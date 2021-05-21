#include "stdafx.h"
#include "main.h"
#include "Tcpipcon.h"
#include "GUtil.h"
#include "NkCharacter.h"
#include "Item.h"
#include "ComboBox.h"
#include "ControlBottom.h"
#include "ControlTeleportItem.h"
#include "ControlWareHouse.h"
#include "LShiftMenu.h"
#include "Nk2DFrame.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "UIMgr.h"
#include "Land.h"
#include "ItemProto.h"
#include "AutoPlay_test.h"
#include <io.h>
#include <algorithm>
#include <atlcoll.h>

#include "LHUI_OpEvent.h"

#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
#include <direct.h>
#endif


#pragma warning( disable : 4800 )
#pragma warning( disable : 4172 )


extern BOOL					g_bPath;




extern BOOL g_bDungeon;
extern BOOL g_bNewTerrainDungeon;

#define FIRST_ATTK	true
#define FIRST_ITEM	false

#define AUTO_GOGO	true
#define AUTO_STOP	false

#define RANGE_MIN	50.0f
#define RANGE_MAX	250.0f

const DWORD FALIED_RETURN_BASE_TIME 	= 1000*10;
const DWORD FALIED_ATTACK_TIME			= 1000*10;
const DWORD FALIED_DECIDE_TARGET_TIME	= 1000*10;
const DWORD FALIED_PICKITEM_TIME		= 1000*10;

const DWORD FAILED_STATE_TIME			= 1000*60;

const DWORD ACTION_DELAY				= 300;
const DWORD QUICK_DELAY					= 300;
const DWORD INPEACE_DELAY				= 300;
const DWORD USING_STASH_DELAY			= 400;
const DWORD MOVE_TICK					= 400;

const float TEMP_DIST					= 15.0f;

//-----------------------------------------------------------------------------
// Name :
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnFieldProc(DWORD dt)
{
	if( _OnAvoid(dt) )
	{
		m_dwAvoidCurTime = dt;
		_ChangeState( AVOID );
		_AddChat( "귀환 : 회피" );
		_UseQuickItem( m_dwReturnKey );

#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
		{
			char log_buf[256];
			sprintf( log_buf , "[AVOID] : 귀환(%d)" , m_dwReturnKey );
			AddLog( log_buf );
		}
#endif // _NALRIM_LAGRUSH_2009_LOGSYSTEM_
		return true;
	}

	if( ITEM == m_current_state )
	{
		if( _OnFieldUsing(dt) )
			return true;

		if( !_OnPickItem(dt) )
		{
			if( dt - m_pick_item_time > FALIED_PICKITEM_TIME )
			{
				_ChangeState( ATTACK );
				_AddChat( "아이템 습득불가!?" );
			}
		}
		return true;
	}
	else if( ATTACK == m_current_state )
	{
		if( _OnFieldUsing(dt) )
			return true;
		if( _OnPet(dt) )
			return true;
		if( _OnSubArms(dt) )
			return true;
		if( _OnAdiaSummon(dt) )
			return true;
		if( _OnShield(dt) )
			return true;
		if( _OnRecovery(dt) )
			return true;
		if( _OnBloody(dt) )
			return true;


		if( _OnDecideTargetItem(dt) )
			return true;
		if( !_OnDecideTarget(dt) )
			return true;

		if( _OnVampTouch(dt) )
			return true;

		if( _OnAttack(dt) )
			return true;

		return true;
	}
	else if( RETURN_TO_BASE == m_current_state )
	{
		if( _OnFieldUsing(dt) )
			return true;

		if( _OnMoveToBase(dt) )
		{
			_ChangeState( ATTACK );
			_AddChat( "시작위치로 복귀!" );
		}

		return true;
	}

	return false;
}

bool CAutoPlay_EX::_OnMove(DWORD dt, float tx, float tz)
{
	static DWORD olddt = 0;
	if( olddt == 0 )
		olddt = dt;
	if( dt - olddt < MOVE_TICK )
		return false;
	olddt = dt;

	int char_motion = g_pRoh->GetMotionState();
	if( char_motion == CHA_RUN || char_motion == CHA_WALK || char_motion == CHA_STAND )
	{
		if( pCMyApp->MoveHeroT(tx, tz) )
		{
			olddt = 0;
			return true;
		}
	}
	else
	{
		if( m_current_state == ITEM || m_current_state == ATTACK || m_current_state == RETURN_TO_BASE )
		{
			g_pRoh->SetMotionState( CHA_STAND );
			return false;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name : _OnMoveTo
// Date : 2009.10.22
// Desc : walkpos.txt 참조 이용
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnMoveToWalk(DWORD dt)
{
	bool rv = false;

	if( g_pNk2DFrame->IsShopVisible() )
		g_pNk2DFrame->ToggleShopWindow();

	if( m_pairPoslist.empty() )
	{
		_OnInterrupt(LagRush_Interrupt__WalkPos_Empty);
		return false;
	}

	if( m_wWalkPosCount > m_pairPoslist.size() -1 )
	{
		m_fStayPosX = g_pRoh->m_wx;
		m_fStayPosZ = g_pRoh->m_wz;
		return true;
	}

	float dist = DistPlToPlAbs( m_pairPoslist[m_wWalkPosCount].first, m_pairPoslist[m_wWalkPosCount].second, g_pRoh->m_wx, g_pRoh->m_wz );
	if( dist > 1.0f )
	{
		_OnMove(dt, m_pairPoslist[m_wWalkPosCount].first, m_pairPoslist[m_wWalkPosCount].second );
	}
	else
	{
		m_wWalkPosCount++;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name : _OnMoveToMachine
// Date : 2009.10.22
// Desc : 지정전송장치 이용
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnMoveToMachine(DWORD dt)
{
	static DWORD olddt = 0 ;
	if( olddt == 0 )
	{
		olddt = dt;
		return false;
	}

	if( dt - olddt < 1500 )
		return false;


	if( m_bUsed_Trans && pCMyApp->m_pUIMgr->m_bIsTeleportItem )
	{
		if( pCMyApp->m_pUIMgr->m_pTeleportItem->SendLoadPosMsg( m_dwTransportingPos ) == 1 )
		{
			pCMyApp->m_pUIMgr->m_bIsTeleportItem = FALSE;
			if(pCMyApp->m_pUIMgr->m_pTeleportItem)
			{
				delete pCMyApp->m_pUIMgr->m_pTeleportItem;
				pCMyApp->m_pUIMgr->m_pTeleportItem = NULL;
			}

			_OnInterrupt(LagRush_Interrupt__Machine_Error);

			return false;
		}
		else
		{
			g_pNk2DFrame->DeleteAllPopup();
			pCMyApp->m_pUIMgr->m_bIsTeleportItem = FALSE;
			pCMyApp->m_pUIMgr->m_pTeleportItem = NULL;
			m_bUsed_Trans = false;
			return true;
		}
	}

	if( false == m_bUsed_Trans && _UseQuickItem( m_dwTransportingKey ) )
	{
		olddt = dt;
		m_bUsed_Trans = true;
	}

	return false;
}


void CAutoPlay_EX::_OnNextSkill(int next_skill)
{
	int skill_slot = next_skill;

	while(true)
	{
		if( skill_slot >= FUNCTION_NUM )
			skill_slot = 0;

		if( m_bSkillArray[skill_slot] )
		{
			m_skill_slot = skill_slot;
			break;
		}

		skill_slot++;
	}
}

void CAutoPlay_EX::_OnSetSkill(bool bNext)
{
	int skill_slot = m_skill_slot;
	bool selected_skill = false;

	if( bNext )
	{
		for( int i=0 ; i<8 ; i++ )
		{
			skill_slot++;
			if( skill_slot >= 8 )
				skill_slot = 0;

			if( m_bSkillArray[skill_slot] )
			{
				m_skill_slot = skill_slot;
				selected_skill = true;
				break;
			}
		}
	}
	else
	{
		for( int i=0 ; i<8 ; i++ )
		{
			if( m_bSkillArray[skill_slot] )
			{
				m_skill_slot = skill_slot;
				selected_skill = true;
				break;
			}

			skill_slot++;
			if( skill_slot >= 8 )
				skill_slot = 0;
		}
	}

	if( true == selected_skill )
	{
		int which_button = m_bSkillDirArray[m_skill_slot];
		int down_key = m_dwSkillKeyArray[m_skill_slot]>0?m_dwSkillKeyArray[m_skill_slot]-1:7;
		int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
		int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

		if( g_Skills[skill_index].m_bAttack )
		{
			if( which_button == 0 && g_Skills[skill_index].m_bLeft )
				g_pNk2DFrame->SetLSkill( cur_skill );
			else if( which_button == 1 && g_Skills[skill_index].m_bRight )
				g_pNk2DFrame->SetRSkill( cur_skill );
		}
		else
		{
			g_pNk2DFrame->GetControlBottom()->m_DownKey = down_key;
			g_pNk2DFrame->GetControlBottom()->ActionQuickKey( down_key );
		}
	}
}

bool CAutoPlay_EX::_OnSkillTimer(DWORD dt)
{
	if( !m_bUseSkill )
		return false;

	bool change_skill = false;

	if( 0 == m_used_skill_delay )
	{
		_OnSetSkill(false);
	}
	else if( m_used_skill_delay && dt - m_used_skill_delay > m_used_limit_delay )
	{
		_OnSetSkill(true);
	}
	else
	{
		return false;
	}

	return true;
}


bool CAutoPlay_EX::_OnAttack(DWORD dt)
{
	if( !m_pSelectedMob )
		return false;

	if( m_pSelectedMob && m_pSelectedMob->IsDead() )
	{
		if( pCMyApp->m_pUIMgr->m_b_MotionForAttack )
			return true;

		return false;
	}

	if( !_IsSpecialMob(m_pSelectedMob->m_nMobNum) )
	{
		switch( _IsWarning() )
		{
		case LR_Warning__None:
			break;
		case LR_Warning__InRange:
			_ChangeState(ATTACK);
			_AddChat( "선타겟 몬스터 발견(범위내)" );
			return false;
		case LR_Warning__OutRange:
			_ChangeState(RETURN_TO_BASE);
			_AddChat( "선타겟 몬스터 발견(범위밖)" );
			return false;
		}
	}

	float dist = DistPlToPlAbs(m_fStayPosX, m_fStayPosZ, m_pSelectedMob->m_wx, m_pSelectedMob->m_wz);
	float dist2 = DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, m_pSelectedMob->m_wx, m_pSelectedMob->m_wz);

	D3DVECTOR vWorldPos, vScreenPos;
	D3DMATRIX matPipe;
	ZeroMemory( &matPipe, sizeof( D3DMATRIX ) );

	int mx = 0, my = 0;
	vWorldPos = D3DVECTOR( m_pSelectedMob->m_wx, m_pSelectedMob->m_wy, m_pSelectedMob->m_wz );
	pCMyApp->m_pUIMgr->m_pLand->GetPipelineMatrix( matPipe );
	D3DMath_VectorMatrixMultiply( vScreenPos, vWorldPos, matPipe );

	mx = vScreenPos.x;
	my = vScreenPos.y;
	if( mx < 0 )	mx = 0;
	if( my < 0 )	my = 0;

	pCMyApp->m_pUIMgr->SetPickColor(m_pSelectedMob->m_wColorIdx);

	if( !pCMyApp->m_pUIMgr->m_b_Attack )
	{
		bool try_attack = pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );
		char buf[128];
		sprintf( buf , "공격클릭 : %d \n" , try_attack );
		_AddChat( buf );

		if( g_pRoh->GetMotionState() == CHA_STAND )
		{
			mx = 0;
			my = 0;
			switch( dt % 4 )
			{
			case 0:
				mx += 100;
				break;
			case 1:
				mx -= 100;
				break;
			case 2:
				my += 100;
				break;
			case 3:
				my -= 100;
				break;
			}

			pCMyApp->MoveHeroT( g_pRoh->m_wx + mx, g_pRoh->m_wz + my );
			// pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );
		}
		return try_attack;
	}

	if( pCMyApp->m_pUIMgr->m_b_MovingForAttack )
	{
		bool try_attack = pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , mx , my );
		char buf[128];
		sprintf( buf , "공격이동중클릭 : %d \n" , try_attack );
		_AddChat( buf );
		return try_attack;
	}

	// 공격중!!!
	if( _OnSkillTimer( dt ) )
	{
		int which_button = m_bSkillDirArray[m_skill_slot];
		int down_key = m_dwSkillKeyArray[m_skill_slot]>0?m_dwSkillKeyArray[m_skill_slot]-1:7;
		int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
		int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

		if( g_Skills[skill_index].m_bAttack )
		{
			bool try_attack = pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , mx , my );
			char buf[128];
			sprintf( buf , "스킬공격클릭 : %d \n" , try_attack );
			_AddChat( buf );
		}

		m_used_skill_delay = dt;
		m_used_limit_delay = m_wSkillLimitTime[m_skill_slot];
	}
	else
	{
		// 기본공격!!!
	}



	if( !pCMyApp->m_pUIMgr->m_b_MotionForAttack )
	{
#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
		{
			char log_buf[256];
			sprintf( log_buf , "[Attack] : 공격중인데 모션은 아니다." );
			AddLog( log_buf );
		}
#endif // _NALRIM_LAGRUSH_2009_LOGSYSTEM_
//		_ChangeState( RETURN_TO_BASE );
		return true;
	}

	return true;
}


//-----------------------------------------------------------------------------
//	Name : _OnUseSkill
//	Date : 2009.
//	Desc : 스킬사용 - F1~F8
//-----------------------------------------------------------------------------
bool CAutoPlay_EX::_OnUseSkill(DWORD dt, int x, int y, CNkMob* target)
{
	if( m_last_attack_tick == 0 )
		m_last_attack_tick = dt;

	int which_button = -1;
#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
	int log_skill = 0;
#endif // _NALRIM_LAGRUSH_2009_LOGSYSTEM_

	if( m_bUseSkill )
	{
		if( m_skill_slot >= FUNCTION_NUM )
			m_skill_slot = 0;

		bool skip = false;
		if( m_bSkillArray[m_skill_slot] )
		{
			if( m_dwSkillCurTime[m_skill_slot] != 0 )
			{
				if(  dt - m_dwSkillCurTime[m_skill_slot] < m_wSkillLimitTime[m_skill_slot] )
				{
				}
				else
				{
					m_dwSkillCurTime[m_skill_slot] = 0;
					//					m_skill_slot++;
					_OnNextSkill(m_skill_slot+1);
					return true;
				}
			}
			else
			{
				which_button = m_bSkillDirArray[m_skill_slot]; // 의미가 쫌...
				int down_key = m_dwSkillKeyArray[m_skill_slot]>0?m_dwSkillKeyArray[m_skill_slot]-1:7;
				int cur_skill = g_pNk2DFrame->GetControlBottom()->m_QuickSkill[down_key].skill;
				int skill_index = g_ChaSkill.m_SkillIndex[cur_skill];

#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
				log_skill = skill_index;
#endif // _NALRIM_LAGRUSH_2009_LOGSYSTEM_

				if( g_Skills[skill_index].m_bAttack )
				{
					if( which_button == 0 && g_Skills[skill_index].m_bLeft )
						g_pNk2DFrame->SetLSkill( cur_skill );
					else if( which_button == 1 && g_Skills[skill_index].m_bRight )
						g_pNk2DFrame->SetRSkill( cur_skill );
				}
				else
				{
					g_pNk2DFrame->GetControlBottom()->m_DownKey = down_key;
					g_pNk2DFrame->GetControlBottom()->ActionQuickKey( down_key );
					m_dwSkillCurTime[m_skill_slot] = dt;

#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
					{
						char log_buf[256];
						sprintf( log_buf , "[UseSkill] : 버튼(2) skill(%d)" , log_skill );
						AddLog( log_buf );
					}
#endif // _NALRIM_LAGRUSH_2009_LOGSYSTEM_

					return true;
				}
			}
		}
		else
		{
			//			m_skill_slot++;
			_OnNextSkill(m_skill_slot+1);
			return false;
		}
	}
	else
	{
		// 뱀파이어 터치
		if( _OnVampTouch(dt) )
			which_button = 1;
	}

	if( which_button == -1 )
	{
		if( !pCMyApp->m_pUIMgr->CmdClick( 0 , 0 , x , y ) )
			return false;
	}
	else
	{
		if( !pCMyApp->m_pUIMgr->CmdClick( which_button , 0 , x , y ) )
			return false;

		if( pCMyApp->m_pUIMgr->m_b_MovingForAttack )
			return false;

		m_dwSkillCurTime[m_skill_slot] = dt;

#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
		{
			char log_buf[256];
			sprintf( log_buf , "[UseSkill] : 버튼(%d) skill(%d)" , which_button , log_skill );
			AddLog( log_buf );
		}
#endif // _NALRIM_LAGRUSH_2009_LOGSYSTEM_
	}

	m_last_attack_tick = dt;

	return true;
}


bool CAutoPlay_EX::_OnMoveToBase(DWORD dt)
{
	if( _CheckElapsedTime( dt , m_move_to_base_tick , FALIED_RETURN_BASE_TIME ) )
	{
		// 10초 지났음
#ifdef _NALRIM_LAGRUSH_2009_LOGSYSTEM_
		{
			char log_buf[256];
			sprintf( log_buf , "[OnMoveToBase] : 실패(%d경과)" , FALIED_RETURN_BASE_TIME );
			AddLog( log_buf );
		}
#endif // _NALRIM_LAGRUSH_2009_LOGSYSTEM_
		return true;
	}

	float dist = DistPlToPlAbs( m_fStayPosX, m_fStayPosZ, g_pRoh->m_wx, g_pRoh->m_wz );
	if( dist > 1.0f )
	{
		_OnMove(dt, m_fStayPosX, m_fStayPosZ );
	}
	else
	{
		m_move_to_base_tick = 0;
		return true;
	}

	return false;
}
