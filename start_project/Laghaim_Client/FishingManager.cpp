#include "stdafx.h"
#include "FishingManager.h"
#include "LHExcelReader.h"

FishingManager::FishingManager()
	: m_isFishingOn(false)
	, m_nBallSpeed(0)
{
	memset(m_nFishCount, 0, sizeof(m_nFishCount));

	load();
}

FishingManager::~FishingManager()
{
}

void FishingManager::Clear()
{
	memset(m_nFishCount, 0, sizeof(m_nFishCount));
}

void FishingManager::load()
{
	LHExcelReader er;
	if( er.Open("data/FISH_OFFSET.TXT") == false )
		return;

	stFishOffset offset;
	int col = 0;
	int rowCount = er.GetRowSize();
	for(int i = 0; i < rowCount; i++)
	{
		col = 0;
		offset.filename = er.GetData(i, col++);
		offset.offset.x = er.GetInt(i, col++);
		offset.offset.y = er.GetInt(i, col++);

		m_mapOffset.insert(std::make_pair(i, offset));
	}

	PFILE* fp = NULL;
	fp = pfopen("data/FISH_BALLSPEED.TXT", "rt");

	if( fp == NULL )
		return;

	char strTemp[MAX_PATH] = {0,};
	pfgets(strTemp, sizeof(strTemp), fp);
	m_nBallSpeed = atoi(strTemp);

	pfclose(fp);
}

std::string FishingManager::GetFileName(int index)
{
	std::string filename("");

	MAP_FISHOFFSET::iterator itFind = m_mapOffset.find(index);
	if( itFind == m_mapOffset.end() )
		return filename;

	filename = itFind->second.filename;

	return filename;
}

POINT FishingManager::GetOffset(int index)
{
	POINT pt;
	pt.x = 0;
	pt.y = 0;

	MAP_FISHOFFSET::iterator itFind = m_mapOffset.find(index);
	if( itFind == m_mapOffset.end() )
		return pt;

	pt = itFind->second.offset;

	return pt;
}

void FishingManager::SetFishCount(int index, int count)
{
	if( index < 0 || index >= MAX_FISHTYPE )
		return;

	m_nFishCount[index] = count;
}

int FishingManager::GetFishCount(int index)
{
	if( index < 0 || index >= MAX_FISHTYPE )
		return 0;

	return m_nFishCount[index];
}

int FishingManager::GetBallSpeed()
{
	return m_nBallSpeed;
}
