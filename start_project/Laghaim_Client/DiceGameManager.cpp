#include "stdafx.h"
#include "DiceGameManager.h"

DiceGameManager::DiceGameManager()
	: m_isDiceGameOn(false)
	, m_SuccessCount(0)
{
}

DiceGameManager::~DiceGameManager()
{
}

void DiceGameManager::Clear()
{
	m_SuccessCount = 0;
}

void DiceGameManager::SetSuccessCount(int count)
{
	m_SuccessCount = count;
}

int DiceGameManager::GetSuccessCount()
{
	return m_SuccessCount;
}
