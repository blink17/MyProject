#pragma once

#include "Space.h"

#define SEPARATE_COUNT	450

using namespace std;
class Mine;

class SpaceBoard
{
private:
	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;
	//Space m_spaceMap[SEPARATE_COUNT][SEPARATE_COUNT];
	Space** m_spaceMap;
	int m_termX;
	int m_termY;

public:
	SpaceBoard();
	~SpaceBoard();

	void Build(int minX, int maxX, int minY, int maxY);
	void Add(Mine* mine);
	void CombineMineEachSpace();
};

