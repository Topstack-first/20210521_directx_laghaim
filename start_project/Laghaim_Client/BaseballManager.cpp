#include "stdafx.h"
#include "BaseballManager.h"
#include "LHFile.h"

BaseballManager::BaseballManager()
	: m_isBaseballOn(false)
	, m_nBallSpeed(0)
{
	memset(m_nBallCount, 0, sizeof(m_nBallCount));

	load();
}

BaseballManager::~BaseballManager()
{
}

void BaseballManager::Clear()
{
	memset(m_nBallCount, 0, sizeof(m_nBallCount));
}

void BaseballManager::load()
{
	PFILE* fp = NULL;
	fp = pfopen("data/FISH_BALLSPEED.TXT", "rt");

	if( fp == NULL )
		return;

	char strTemp[MAX_PATH] = {0,};
	pfgets(strTemp, sizeof(strTemp), fp);
	m_nBallSpeed = atoi(strTemp);

	pfclose(fp);
}

void BaseballManager::SetBallCount(int index, int count)
{
	if( index < 0 || index >= MAX_BASEBALL_GRADE )
		return;

	m_nBallCount[index] = count;
}

int BaseballManager::GetBallCount(int index)
{
	if( index < 0 || index >= MAX_BASEBALL_GRADE )
		return 0;

	return m_nBallCount[index];
}

int BaseballManager::GetBallSpeed()
{
	return m_nBallSpeed;
}

