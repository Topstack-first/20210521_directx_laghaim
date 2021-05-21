#ifndef _ENTITYFACTORY_H_
#define _ENTITYFACTORY_H_

#include "Config.h"

#include "headers.h"


#pragma warning(disable : 4786)
#pragma warning(disable : 4183)
#pragma warning(disable : 4995)


#include <map>
#include <string>

typedef int ObjectType;

/************************************************************************/
/* CreatorBase                                                          */
/************************************************************************/
template<class Base>
class CreatorBase
{
public:
	virtual ~CreatorBase() {}
	virtual Base * Create() const = 0;
};

/************************************************************************/
/* Creator                                                              */
/************************************************************************/
template<class Product, class Base>
class Creator : public CreatorBase<Base>
{
public:
	virtual Base * Create() const
	{
		return new Product;
	}
};

/************************************************************************/
/* CEntityFactory                                                       */
/************************************************************************/

template<class Base>
class CEntityFactory
{
public:
	CEntityFactory();
	~CEntityFactory();

	Base * Create(ObjectType type);
	bool Register(ObjectType type, CreatorBase<Base> * pCreator);
private:
	typedef std::map<ObjectType, CreatorBase<Base> *> CreatorMap;
	CreatorMap m_creatorMap;
};

template<class Base>
CEntityFactory<Base>::CEntityFactory()
{

}

template<class Base>
CEntityFactory<Base>::~CEntityFactory()
{
	CreatorMap::iterator beg = m_creatorMap.begin();
	CreatorMap::iterator end = m_creatorMap.end();
	for(; beg != end; beg++)
	{
		SAFE_DELETE(beg->second);
	}
	m_creatorMap.clear();
}

template<class Base>
bool CEntityFactory<Base>::Register(ObjectType type, CreatorBase<Base> * pCreator)
{
	char szTmp[256] = {0,};

	CreatorMap::iterator it = m_creatorMap.find(type);
	if (it != m_creatorMap.end())
	{
		delete pCreator;
		return false;
	}

	m_creatorMap[type] = pCreator;
	return true;
}


template<class Base>
Base * CEntityFactory<Base>::Create(ObjectType type)
{
	char szTmp[256] = {0,};

	CreatorMap::iterator it = m_creatorMap.find(type);
	if (it == m_creatorMap.end())
	{
		sprintf(szTmp, "EFactory Not Find:%d\n", type);
		OutputDebugString(szTmp);
		return NULL;
	}

	CreatorBase<Base> * pCreator = (*it).second;
	return pCreator->Create();
}

#endif // !defined(_ENTITYFACTORY_H_)
