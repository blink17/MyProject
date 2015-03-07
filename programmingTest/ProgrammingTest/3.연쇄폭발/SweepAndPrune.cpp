#include "SweepAndPrune.h"
#include <algorithm>

#include "Mine.h"

SweepAndPrune::SweepAndPrune()
{
}


SweepAndPrune::~SweepAndPrune()
{
}

void SweepAndPrune::Add(Mine* mine)
{
	SPPoint start(eSPPointType::START_POINT, mine->GetMinX(), mine);
	SPPoint end(eSPPointType::END_POINT, mine->GetMaxX(), mine);
	m_pointList.push_back(start);
	m_pointList.push_back(end);
}

bool CompareSPPoint(SPPoint a, SPPoint b)
{
	return a.m_pos < b.m_pos;
}

void SweepAndPrune::Sort()
{
	std::sort(m_pointList.begin(), m_pointList.end(), CompareSPPoint);
}


SPPoint::SPPoint(eSPPointType type, int pos, Mine* baseMine) :
m_type(type), m_pos(pos), m_baseMine(baseMine) 
{
}

SPPoint::~SPPoint()
{
}