#pragma once

#include <vector>

using namespace std;
class Mine;
class SPPoint;

class SweepAndPrune
{
private:
	vector<SPPoint> m_pointList;

public:
	SweepAndPrune();
	~SweepAndPrune();

	void Add(Mine* mine);
	void Sort();
	vector<SPPoint>* GetPointList();
};

inline vector<SPPoint>* SweepAndPrune::GetPointList() { return &this->m_pointList; }

enum eSPPointType
{
	START_POINT,
	END_POINT,
};

class SPPoint
{
public:
	SPPoint(eSPPointType type, int pos, Mine* baseMine);
	~SPPoint();

	eSPPointType	m_type;
	int				m_pos;
	Mine*			m_baseMine;
};

