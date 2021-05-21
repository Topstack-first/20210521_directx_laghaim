#include "stdafx.h"
#include "DecardiBattleManager.h"

DecardiBattleManager::DecardiBattleManager()
	: m_bStart(false)
	, m_bGate(true)
	, m_dwStartTime(0)
{	
	memset(m_nPoint, 0, sizeof(m_nPoint));
	m_attackable[0].setData(6580, 5630, 6620, 5670);
	m_attackable[1].setData(6580, 6930, 6620, 6970);
	m_attackable[2].setData(5729, 6280, 5769, 6320);
	m_attackable[3].setData(7480, 6280, 7520, 6320);
	m_attackable[4].setData(6585, 6275, 6620, 6320);
}

DecardiBattleManager::~DecardiBattleManager()
{
}

void DecardiBattleManager::Clear()
{
	m_bGate = true;
	ClearList();
}

void DecardiBattleManager::ClearList()
{
	m_mapChar.clear();
	m_bStart = false;	
	m_nPoint[0] = 0;
	m_nPoint[1] = 0;
}

void DecardiBattleManager::AddTeam(const int team, const int charIndex)
{
	MAP_DCB::iterator itFind = m_mapChar.find(charIndex);
	if( itFind != m_mapChar.end() )
		return;

	m_mapChar.insert(std::make_pair(charIndex, team));
}

const int DecardiBattleManager::GetTeam(const int charIndex)
{
	MAP_DCB::iterator itFind = m_mapChar.find(charIndex);
	if( itFind == m_mapChar.end() )
		return -1;
	
	return itFind->second;
}

const bool DecardiBattleManager::IsSameTeam(const int idx1, const int idx2)
{
	int team1 = GetTeam(idx1);
	int team2 = GetTeam(idx2);	

	if( team1 == -1 || team2 == -1 )
		return true;

	return team1 == team2;
}

void DecardiBattleManager::SetPoint(const int team, const int point)
{
	if( team < 0 || team >= DCB_TEAM_COUNT )
		return;

	m_nPoint[team] = point;
}

const int DecardiBattleManager::GetPoint(const int team)
{
	if( team < 0 || team >= DCB_TEAM_COUNT )
		return 0;

	return m_nPoint[team];
}

void DecardiBattleManager::GetTime(int& min, int& sec)
{
	DWORD dwCurTime = GetTickCount();

	int nElapsedTime = (int)(m_dwStartTime - dwCurTime);
	nElapsedTime *= -1;

	nElapsedTime /= 1000;
	nElapsedTime = 1800 - nElapsedTime;

	if( nElapsedTime < 0 )
	{
		nElapsedTime = 0;
	}

	min = nElapsedTime / 60;
	sec = nElapsedTime % 60;
}

void DecardiBattleManager::Start()
{
	m_bStart = true;

	m_dwStartTime = GetTickCount();
}

void DecardiBattleManager::End()
{
	m_bStart = false;
}

const bool DecardiBattleManager::IsStart()
{
	return m_bStart;
}

void DecardiBattleManager::SetGate(const bool bDraw)
{
	m_bGate = bDraw;
}

const bool DecardiBattleManager::GetGate()
{
	return m_bGate;
}

const bool DecardiBattleManager::IsAttackable(const int x, const int z)
{
	if( m_bStart == false )
		return false;

	for(int i = 0; i < 5; i++)
	{
		if( m_attackable[i].isInside(x, z) == true )
			return true;
	}

	return false;
}
