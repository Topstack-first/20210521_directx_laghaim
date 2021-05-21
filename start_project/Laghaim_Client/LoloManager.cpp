#include "stdafx.h"
#include "LoloManager.h"
#include "IndexedTexture.h"
#include "CapSyong.h"
#include "main.h"

LoloManager::LoloManager()
{

}

LoloManager::~LoloManager()
{
	Clear();
}

void LoloManager::Clear()
{
	MAP_LOLO::iterator itEnd = m_mapLolo.end();
	MAP_LOLO::iterator it = m_mapLolo.begin();

	CLolos* pLolo = NULL;
	for( ; it != itEnd; it++ )
	{
		pLolo = (*it).second;
		SAFE_DELETE(pLolo);
	}

	m_mapLolo.clear();
}

CLolos* LoloManager::GetLolo(std::string filename)
{
	MAP_LOLO::iterator itFind = m_mapLolo.find(filename);
	if( itFind != m_mapLolo.end() )
	{
		return (*itFind).second;
	}

	char dirName[256];
	strcpy(dirName, "data/item/lolo/");
	strcat(dirName, filename.c_str());

	g_pCapsyongTexture->SetTexturePath("data/item/textr/");

	CLolos* pLolo = new CLolos(g_pCapsyongTexture);
	if( 0 > pLolo->Read(dirName) )
	{
		SAFE_DELETE(pLolo);
		return NULL;
	}

	pLolo->RestoreTextures( GET_D3DDEVICE() );

	m_mapLolo.insert(std::make_pair(filename, pLolo));

	return pLolo;
}

int LoloManager::GetLoloTextureNum(std::string textrFileName)
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