#include "stdafx.h"
#include "BuffItemMgr.h"

BuffItemMgr::BuffItemMgr()
	: m_curTime(0)
{

}

BuffItemMgr::~BuffItemMgr()
{

}

void BuffItemMgr::Update()
{
	DWORD nowTime = timeGetTime();
	long elapsedTime = (long)(nowTime - m_curTime);
	m_curTime = nowTime;

	unsigned int size = m_vecBuffItem.size();
	for( unsigned int i = 0; i < size; i++ )
	{
		m_vecBuffItem[i].remainTime -= elapsedTime;

		if( m_vecBuffItem[i].remainTime < 0 )
			m_vecBuffItem[i].remainTime = 0;
	}
}

void BuffItemMgr::Clear()
{
	m_vecBuffItem.clear();
}

void BuffItemMgr::AddBuff( int type, unsigned long remainTime, int value )
{
	if( type < 0 || type >= POTION_EVENT_NUM )
		return;

	RemoveBuff(type);

	bool isPercent[POTION_EVENT_NUM] = {true, true, true, true, 
										true, true, false, false,
										false, false, false, true, 
										true, true, true, true};

	BuffItem buff;
	buff.type = (Buff_Type)type;
	buff.remainTime = remainTime;
	buff.value = value;
	buff.percent = isPercent[type];

	m_vecBuffItem.push_back(buff);
}

void BuffItemMgr::RemoveBuff( int type )
{
	VEC_BUFFITEM::iterator it = m_vecBuffItem.begin();
	VEC_BUFFITEM::iterator itEnd = m_vecBuffItem.end();

	for( ; it != itEnd; it++ )
	{
		if( (*it).type == type )
		{
			m_vecBuffItem.erase(it);
			return;
		}
	}
}

unsigned int BuffItemMgr::GetBuffCount()
{
	return m_vecBuffItem.size();
}

int BuffItemMgr::GetBuffType(int index)
{
	if( index < 0 || index >= m_vecBuffItem.size() )
		return -1;

	return (int)m_vecBuffItem[index].type;
}

long BuffItemMgr::GetBuffRemainTime(int index)
{
	if( index < 0 || index >= m_vecBuffItem.size() )
		return 0;

	return m_vecBuffItem[index].remainTime;
}

int BuffItemMgr::GetBuffValue(int index)
{
	if( index < 0 || index >= m_vecBuffItem.size() )
		return -1;

	return m_vecBuffItem[index].value;
}

bool BuffItemMgr::GetBuffPercent(int index)
{
	if( index < 0 || index >= m_vecBuffItem.size() )
		return false;

	return m_vecBuffItem[index].percent;
}
