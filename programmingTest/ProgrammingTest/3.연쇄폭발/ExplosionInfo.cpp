#include "ExplosionInfo.h"


ExplosionInfo::ExplosionInfo(int id) : 
m_id(id)
{
}


ExplosionInfo::~ExplosionInfo()
{
}

void ExplosionInfo::Add(Mine* mine)
{
	this->m_chainingList.push_back(mine);
}

void ExplosionInfo::Merge(ExplosionInfo* other)
{
	this->m_chainingList.insert(this->m_chainingList.end(), other->m_chainingList.begin(), other->m_chainingList.end());
}

void ExplosionInfo::ClearChainingList()
{
	this->m_chainingList.clear();
}