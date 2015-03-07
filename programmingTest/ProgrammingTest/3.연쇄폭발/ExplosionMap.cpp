#include "ExplosionMap.h"

#include <iostream>
#include <list>
#include "ExplosionInfo.h"
#include "Mine.h"

ExplosionMap::ExplosionMap() : m_idGenerator(0)
{

}

ExplosionMap::~ExplosionMap()
{
	for (int i = 0; i < m_all.size(); ++i)
		delete m_all[i];
}

ExplosionInfo*	ExplosionMap::MakeExplosionInfo()
{
	ExplosionInfo* info = NULL;
	if (m_pool.size() > 0)
	{
		info = m_pool.back();
		m_pool.pop_back();
	}
	else
	{
		info = new ExplosionInfo(this->m_idGenerator++);
		m_all.push_back(info);
	}
	return info;
}

void ExplosionMap::ReturnExplosionInfo(ExplosionInfo* returned)
{
	if (returned == NULL)
		return;

	returned->ClearChainingList();
	m_pool.push_back(returned);
}

void ExplosionMap::Merge(ExplosionInfo* dest, ExplosionInfo* added)
{
	dest->Merge(added);

	list<Mine*>* chainingList = added->GetChainingList();
	list<Mine*>::iterator iter;
	for (iter = chainingList->begin(); iter != chainingList->end(); ++iter)
	{
		(*iter)->SetExplosionInfo(dest);
	}

	this->ReturnExplosionInfo(added);
}

int ExplosionMap::GetExplosionCount()
{
	int resultCount = 0;

	size_t size = m_all.size();
	for (size_t i = 0; i < size; ++i)
	{
		list<Mine*>* chainingList = m_all[i]->GetChainingList();
		if (chainingList->size() > 0)
			++resultCount;
	}

	return resultCount;
}