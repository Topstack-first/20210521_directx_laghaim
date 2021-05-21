#include "stdafx.h"
#include <algorithm>
#include "BingoManager.h"

BingoManager::BingoManager()
	: m_isBingoOn(false)
{
	Clear();
}

BingoManager::~BingoManager()
{
}

void BingoManager::Clear()
{
	m_SuccessCount = 0;
	memset(m_Previews, 0, sizeof(m_Previews));
	memset(m_PresentState, 0, sizeof(m_PresentState));
	memset(m_isChecked, 0, sizeof(m_isChecked));
}

void BingoManager::SetSuccessCount(const int successCount)
{
	m_SuccessCount = successCount;
}

const int BingoManager::GetSuccessCount()
{
	return m_SuccessCount;
}

void BingoManager::SetPreview(int* previews)
{
	memcpy(m_Previews, previews, sizeof(m_Previews));	
}

const int* BingoManager::GetPreview()
{
	return m_Previews;
}

void BingoManager::SetPresentState(int* presentStates)
{
	memcpy(m_PresentState, presentStates, sizeof(m_PresentState));
}

const int* BingoManager::GetPresentState()
{
	return m_PresentState;
}

void BingoManager::SetBingoNums(const int count, int* bingoNums)
{
	memset(m_isChecked, 0, sizeof(m_isChecked));

	int num = 0;
	for(int i = 0; i < count; i++)
	{
		num = bingoNums[i];
		
		if( num < 1 || num > BINGO_NUMS_MAX )
			continue;

		m_isChecked[num - 1] = true;
	}	
}

const bool* BingoManager::GetBingoNums()
{
	return m_isChecked;
}
