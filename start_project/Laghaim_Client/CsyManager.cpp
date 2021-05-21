#include "stdafx.h"
#include "CsyManager.h"
#include "IndexedTexture.h"
#include "CapSyong.h"
#include "main.h"

CsyManager::CsyManager()
{
}

CsyManager::~CsyManager()
{
	Clear();
}

void CsyManager::Clear()
{
	MAP_CAPSYONG::iterator itEnd = m_mapCapsyong.end();
	MAP_CAPSYONG::iterator it = m_mapCapsyong.begin();

	CCapsyong* pCsy = NULL;
	for( ; it != itEnd; it++ )
	{
		pCsy = (*it).second;
		SAFE_DELETE(pCsy);
	}

	m_mapCapsyong.clear();
}

CCapsyong* CsyManager::GetCsy(std::string filename)
{
	MAP_CAPSYONG::iterator itFind = m_mapCapsyong.find(filename);
	if( itFind != m_mapCapsyong.end() )
	{
		return (*itFind).second;
	}

	char dirName[256];
	strcpy(dirName, "data/item/csy/");
	strcat(dirName, filename.c_str());

	g_pCapsyongTexture->SetTexturePath("data/item/textr/");

	CCapsyong* pCsy = new CCapsyong(g_pCapsyongTexture);
	if( 0 > pCsy->Read(dirName) )
	{
		SAFE_DELETE(pCsy);
		return NULL;
	}

	pCsy->RestoreTextures( GET_D3DDEVICE() );

	m_mapCapsyong.insert(std::make_pair(filename, pCsy));

	return pCsy;
}

int CsyManager::GetLoloTextureNum(std::string textrFileName)
{
	g_pCapsyongTexture->SetTexturePath("data/item/textr/");

	bool bLoaded = false;
	int ret = g_pCapsyongTexture->FindTextureWXY(const_cast<char*>(textrFileName.c_str()));
	if( ret == -1 )
	{
		ret = g_pCapsyongTexture->CreateTextureFromFile(const_cast<char*>(textrFileName.c_str()));
	}
	else
	{
		bLoaded = true;
	}

	if( bLoaded == false )
	{
		g_pCapsyongTexture->RestoreByIdx(ret, GET_D3DDEVICE());
	}

	return ret;
}