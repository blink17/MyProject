#pragma once

#include <list>

using namespace std;
class Mine;
class ExplosionMap;

class ExplosionInfo
{
private:
	int				m_id;
	list<Mine*>		m_chainingList;

public:
	ExplosionInfo(int id);
	~ExplosionInfo();

	void Add(Mine* mine);
	void Merge(ExplosionInfo* other);

	int GetID();
	list<Mine*>* GetChainingList();
	void ClearChainingList();
};

inline int ExplosionInfo::GetID() { return this->m_id; }
inline list<Mine*>* ExplosionInfo::GetChainingList() { return &this->m_chainingList; }

