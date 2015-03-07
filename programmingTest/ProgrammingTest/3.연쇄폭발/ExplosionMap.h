#pragma once

#include <vector>

using namespace std;
class ExplosionInfo;

class ExplosionMap
{
private:
	static ExplosionMap* s_instance;

	ExplosionMap();
	~ExplosionMap();

	vector<ExplosionInfo*>	m_all;
	vector<ExplosionInfo*>	m_pool;

	int m_idGenerator;

public:
	static ExplosionMap* Instance()
	{
		if (s_instance == NULL)
		{
			s_instance = new ExplosionMap();
		}
		return s_instance;
	}

	static void Purge()
	{
		delete s_instance;
		s_instance = NULL;
	}

	ExplosionInfo*	MakeExplosionInfo();
	void			ReturnExplosionInfo(ExplosionInfo* returned);

	void					Merge(ExplosionInfo* dest, ExplosionInfo* added);
	int						GetExplosionCount();
};
