#include "Space.h"
#include <iostream>
#include <list>
#include <time.h>
#include <math.h>

#include "Mine.h"
#include "ExplosionMap.h"
#include "ExplosionInfo.h"
#include "SweepAndPrune.h"

Space::Space()
{
}


Space::~Space()
{
}

void Space::Init(int minX, int maxX, int minY, int maxY)
{
	this->m_minX = minX;
	this->m_maxX = maxX;
	this->m_minY = minY;
	this->m_maxY = maxY;
}

void Space::AddMine(Mine* mine)
{
	m_mineVector.push_back(mine);
}

bool Space::Intersect(Mine* mine)
{
	if (this->m_minX > mine->GetMaxX() || this->m_maxX < mine->GetMinX())
		return false;

	if (this->m_minY > mine->GetMaxY() || this->m_maxY < mine->GetMinY())
		return false;

	return true;
}

//int existCount = 0;

void Space::CombineMine()
{
	size_t size = m_mineVector.size();
	if (size == 0)
		return;

	//cout << size << "(" << ++existCount << ")" << endl;
	//return;

	clock_t t = clock();

	SweepAndPrune sweepAndPrune;
	for (size_t i = 0; i < size; ++i)
	{
		sweepAndPrune.Add(m_mineVector[i]);
	}
	sweepAndPrune.Sort();

	//cout << "CombineMine::Sort Complete. (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << ")" << endl;
	//return;

	vector<SPPoint>*	spList	= sweepAndPrune.GetPointList();
	size_t				spSize	= spList->size();
	SPPoint*			spPoint	= NULL;
	list<Mine*>				compareList;
	list<Mine*>::iterator	compareIter;
	Mine*					compareMine = NULL;
			
	for (size_t sp_idx = 0; sp_idx < spSize; ++sp_idx)
	{
		spPoint = &(*spList)[sp_idx];
		// Start Point
		if (spPoint->m_type == eSPPointType::START_POINT)
		{
			compareIter = compareList.begin();
			for (; compareIter != compareList.end(); ++compareIter)
			{
				compareMine = *compareIter;
				if (spPoint->m_baseMine->GetExplosionInfo() != NULL && compareMine->GetExplosionInfo() != NULL)
				{
					if (spPoint->m_baseMine->GetExplosionInfo() == compareMine->GetExplosionInfo())
						continue;
				}

				if (spPoint->m_baseMine->IsContact(compareMine) == true)
				{
					// Collision Occur
					ExplosionInfo* explosionInfo = NULL;
					if (spPoint->m_baseMine->GetExplosionInfo() == NULL)
					{
						// Add new
						if (compareMine->GetExplosionInfo() == NULL)
						{
							explosionInfo = ExplosionMap::Instance()->MakeExplosionInfo();
							explosionInfo->Add(spPoint->m_baseMine);
							explosionInfo->Add(compareMine);

							spPoint->m_baseMine->SetExplosionInfo(explosionInfo);
							compareMine->SetExplosionInfo(explosionInfo);
						}
						// Add base to compare
						else
						{
							explosionInfo = compareMine->GetExplosionInfo();
							explosionInfo->Add(spPoint->m_baseMine);

							spPoint->m_baseMine->SetExplosionInfo(explosionInfo);
						}
					}
					// baseMine has ExplosionInfo
					else
					{
						// Add compare to base
						if (compareMine->GetExplosionInfo() == NULL)
						{
							explosionInfo = spPoint->m_baseMine->GetExplosionInfo();
							explosionInfo->Add(compareMine);

							compareMine->SetExplosionInfo(explosionInfo);
						}
						// Merge
						else
						{
							explosionInfo = compareMine->GetExplosionInfo();
							if (explosionInfo->GetChainingList()->size() > spPoint->m_baseMine->GetExplosionInfo()->GetChainingList()->size())
								ExplosionMap::Instance()->Merge(explosionInfo, spPoint->m_baseMine->GetExplosionInfo());
							else
								ExplosionMap::Instance()->Merge(spPoint->m_baseMine->GetExplosionInfo(), explosionInfo);
						}
					}
				}
			}
			compareList.push_back(spPoint->m_baseMine);
		}
		// End Point
		else
		{
			if (spPoint->m_baseMine->GetExplosionInfo() == NULL)
			{
				ExplosionInfo* explosionInfo = ExplosionMap::Instance()->MakeExplosionInfo();
				explosionInfo->Add(spPoint->m_baseMine);
				spPoint->m_baseMine->SetExplosionInfo(explosionInfo);
			}
			compareList.remove(spPoint->m_baseMine);
		}
	}

	//cout << "CombineMine::CollisionCheck Complete. (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << ")" << endl;
}