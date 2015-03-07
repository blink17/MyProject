#pragma once

#include <vector>

using namespace std;
class Mine;
class ExplosionMap;

class Space
{
private:
	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;

	vector<Mine*> m_mineVector;

public:
	Space();
	~Space();

	void Init(int minX, int maxX, int minY, int maxY);

	int GetMinX();
	int GetMaxX();
	int GetMinY();
	int GetMaxY();

	void AddMine(Mine* mine);
	bool Intersect(Mine* mine);

	void CombineMine();
};

inline int Space::GetMinX() { return this->m_minX; }
inline int Space::GetMaxX() { return this->m_maxX; }
inline int Space::GetMinY() { return this->m_minY; }
inline int Space::GetMaxY() { return this->m_maxY; }