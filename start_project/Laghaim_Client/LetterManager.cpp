#include "stdafx.h"
#include "LetterManager.h"

LetterManager::LetterManager()
	: m_nMaxPage(1)
	, m_nCurPage(0)
{
}

LetterManager::~LetterManager()
{
}

void LetterManager::Clear()
{
	m_vecList.clear();
	for(int i = 0; i < LETTER_TYPE_MAX; i++)
	{
		m_mapContent[i].clear();
	}	
}

void LetterManager::SetCurPage(const int nCurPage)
{
	m_nCurPage = nCurPage;
}

const int LetterManager::GetCurPage()
{
	return m_nCurPage;
}

void LetterManager::SetMaxPage(const int nMaxPage)
{
	m_nMaxPage = nMaxPage;
}

const int LetterManager::GetMaxPage()
{
	return m_nMaxPage;
}

void LetterManager::ClearList()
{
	m_vecList.clear();
}

void LetterManager::AddListData(int type, const NoteMessage& data)
{
	// vec
	LetterListData listData;
	listData.index = data.index;
	listData.state = data.state;
	listData.name = data.char_name;
	listData.title = data.title;
	listData.send_time = data.send_time;
	listData.keep_end_time = data.keep_end_time;

	m_vecList.push_back(listData);

	// map
	auto it = m_mapContent[type].find(data.index);
	if( it != m_mapContent[type].end() )
		return;

	LetterContent ContentData;
	ContentData.index = data.index;
	ContentData.name = data.char_name;
	ContentData.title = data.title;
	ContentData.content = "";
	ContentData.hasContent = false;

	m_mapContent[type].insert(std::make_pair(data.index, ContentData));
}

const LetterManager::VEC_LETTER_LIST& LetterManager::GetList()
{
	return m_vecList;
}

void LetterManager::UpdateContent(int type, int index, const std::string& content)
{
	int size = m_vecList.size();
	for(int i = 0; i < size; i++)
	{
		if( m_vecList[i].index == index )
		{
			m_vecList[i].state = 0;
		}
	}

	auto it = m_mapContent[type].find(index);
	if( it == m_mapContent[type].end() )
		return;
	
	m_mapContent[type][index].content = content;
	m_mapContent[type][index].hasContent = true;
}

const int LetterManager::GetLetterIndex(int arrIdx)
{
	if( arrIdx < 0 || arrIdx >= m_vecList.size() )
		return -1;

	return m_vecList[arrIdx].index;
}

LetterContent LetterManager::GetContent(int type, int index)
{
	LetterContent data;
	data.index = -1;

	auto it = m_mapContent[type].find(index);
	if( it == m_mapContent[type].end() )
		return data;

	if( it->second.hasContent == false )
		return data;

	return it->second;
}

LetterContent LetterManager::GetContentByArrayIndex(int type, int arrIdx)
{
	if( arrIdx < 0 || arrIdx >= m_vecList.size() )
		return GetContent(type, -1);

	return GetContent(type, m_vecList[arrIdx].index);
}
