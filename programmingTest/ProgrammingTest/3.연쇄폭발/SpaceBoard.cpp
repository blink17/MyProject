#include "SpaceBoard.h"
#include <iostream>
#include <thread>

#include "Space.h"
#include "Mine.h"

SpaceBoard::SpaceBoard()
{
	m_spaceMap = new Space*[SEPARATE_COUNT];
	for (int i = 0; i < SEPARATE_COUNT; ++i)
	{
		m_spaceMap[i] = new Space[SEPARATE_COUNT];
	}
}


SpaceBoard::~SpaceBoard()
{
	for (int i = 0; i < SEPARATE_COUNT; ++i) {
		delete[] m_spaceMap[i];
	}
	delete[] m_spaceMap;
}

void SpaceBoard::Build(int minX, int maxX, int minY, int maxY)
{
	this->m_minX = minX;
	this->m_maxX = maxX;
	this->m_minY = minY;
	this->m_maxY = maxY;

	this->m_termX = (maxX - minX + 1) / SEPARATE_COUNT;
	this->m_termY = (maxY - minY + 1) / SEPARATE_COUNT;

	int spaceMinX = 0;
	int spaceMaxX = minX - 1;
	int spaceMinY = 0;
	int spaceMaxY = minY - 1;


	for (int y = 0; y < SEPARATE_COUNT; ++y)
	{
		spaceMinY = spaceMaxY + 1;
		spaceMaxY = spaceMinY + this->m_termY - 1;
		if (y == SEPARATE_COUNT - 1)
			spaceMaxX = maxY;

		spaceMinX = 0;
		spaceMaxX = minX - 1;
		for (int x = 0; x < SEPARATE_COUNT; ++x)
		{
			spaceMinX = spaceMaxX + 1;
			spaceMaxX = spaceMinX + this->m_termX - 1;
			if (x == SEPARATE_COUNT - 1)
				spaceMaxX = maxX;

			this->m_spaceMap[x][y].Init(spaceMinX, spaceMaxX, spaceMinY, spaceMaxY);

			//cout << "Space[" << x << "][" << y << "](" << spaceMinX << "," << spaceMaxX << ")(" << spaceMinY << "," << spaceMaxY << ")" << endl;
		}
	}
}

void SpaceBoard::Add(Mine* mine)
{
	int startX = mine->GetMinX() / this->m_termX;
	if (startX < 0)
		startX = 0;

	int finishX = mine->GetMaxX() / this->m_termX;
	if (finishX > SEPARATE_COUNT)
		finishX = SEPARATE_COUNT - 1;

	int startY = mine->GetMinY() / this->m_termY;
	if (startY < 0)
		startY = 0;

	int finishY = mine->GetMaxY() / this->m_termY;
	if (finishX > SEPARATE_COUNT)
		finishY = SEPARATE_COUNT - 1;

	for (int y = startY; y <= finishY; ++y)
	{
		for (int x = startX; x <= finishX; ++x)
		{
			if (this->m_spaceMap[x][y].Intersect(mine))
				this->m_spaceMap[x][y].AddMine(mine);
		}
	}
}

void CombineWork(Space** spaceMap, int startY, int endY)
{
	for (int y = startY; y < endY; ++y)
	{
		for (int x = 0; x < SEPARATE_COUNT; ++x)
		{
			spaceMap[x][y].CombineMine();
		}
	}
}

void SpaceBoard::CombineMineEachSpace()
{
	// Race������� ��Ÿ���Ƿ� ������ ������� ����.
	/*int term = SEPARATE_COUNT / 4;

	thread t1(&CombineWork, this->m_spaceMap, 0, term);
	thread t2(&CombineWork, this->m_spaceMap, term, term + term);
	thread t3(&CombineWork, this->m_spaceMap, term + term, term + term + term);
	thread t4(&CombineWork, this->m_spaceMap, term + term + term, SEPARATE_COUNT);

	t1.join();
	t2.join();
	t3.join();
	t4.join();*/


	for (int y = 0; y < SEPARATE_COUNT; ++y)
	{
		for (int x = 0; x < SEPARATE_COUNT; ++x)
		{
			this->m_spaceMap[x][y].CombineMine();
		}
	}
}