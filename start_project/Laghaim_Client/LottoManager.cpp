#include "stdafx.h"
#include "LottoManager.h"

LottoManager::LottoManager()
	: m_isLottoOn(false)
	, m_cntTicket(0)
{
}

LottoManager::~LottoManager()
{
}

void LottoManager::Clear()
{
	m_vecPack.clear();
	m_cntTicket = 0;
}

void LottoManager::ClearList()
{
	m_vecPack.clear();
}

void LottoManager::AddList(const std::vector<int>& numbers)
{
	if( numbers.size() != MAX_LOTTO_INPUT )
	{
		return;
	}

	LottoPack pack;
	for(int i = 0; i < MAX_LOTTO_INPUT; i++)
	{
		pack.num[i] = numbers[i];
	}

	m_vecPack.push_back(pack);
}

const int LottoManager::GetListCount()
{
	int size = (int)m_vecPack.size();
	if( size > MAX_LOTTO_REGIST_LIST )
	{
		size = MAX_LOTTO_REGIST_LIST;
	}

	return size;
}

LottoPack LottoManager::GetPack(const int index, bool& bFail)
{
	bFail = false;

	LottoPack pack;
	pack.num[0]=0;        //Arnold
	pack.num[1]=0;
	pack.num[2]=0;
	pack.num[3]=0;
	pack.num[4]=0;
	pack.num[5]=0;
	
	if( index >= MAX_LOTTO_REGIST_LIST )
	{
		bFail = true;
		return pack;
	}

	if( index < 0 || index >= m_vecPack.size() )
	{
		bFail = true;
		return pack;
	}

	return m_vecPack[index];
}
