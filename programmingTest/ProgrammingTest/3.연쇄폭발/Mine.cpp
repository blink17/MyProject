#include "Mine.h"

#include "ExplosionMap.h"
#include "ExplosionInfo.h"

Mine::Mine() : m_explosionInfo(0)
{

}

Mine::~Mine()
{
}

void Mine::Init(int x, int y, int radius)
{
	this->m_x = x;
	this->m_y = y;
	this->m_radius = radius;
	this->m_minX = x - radius;
	this->m_maxX = x + radius;
	this->m_minY = y - radius;
	this->m_maxY = y + radius;
}

void Mine::SetExplosionInfo(ExplosionInfo* newExposionInfo)
{
	this->m_explosionInfo = newExposionInfo;
}

bool Mine::IsSameExplosionInfo(Mine* other)
{
	return this->m_explosionInfo->GetID() == other->m_explosionInfo->GetID();
}

bool Mine::IsContact(Mine* other)
{
	// ���� �ڽ��� ���� �˻�
	if (this->m_minX > other->GetMaxX() || this->m_maxX < other->GetMinX())
		return false;

	if (this->m_minY > other->GetMaxY() || this->m_maxY < other->GetMinY())
		return false;

	// ������ �˻�
	int radiusSum = this->m_radius + other->m_radius;
	int distX = this->m_x - other->m_x;
	int distY = this->m_y - other->m_y;

	return (radiusSum * radiusSum) >= ((distX * distX) + (distY * distY));
}