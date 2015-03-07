#pragma once

class ExplosionInfo;
class ExplosionMap;

class Mine
{
private:
	int m_x;
	int m_y;
	int m_radius;
	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;

	ExplosionInfo* m_explosionInfo;

public:
	Mine();
	~Mine();

	void Init(int x, int y, int radius);

	int GetX();
	int GetY();
	int GetRadius();
	int GetMinX();
	int GetMaxX();
	int GetMinY();
	int GetMaxY();
	ExplosionInfo* GetExplosionInfo();

	void SetExplosionInfo(ExplosionInfo* newExposionInfo);
	bool IsSameExplosionInfo(Mine* other);
	bool IsContact(Mine* other);
};

inline int Mine::GetX() { return this->m_x; }
inline int Mine::GetY() { return this->m_y; }
inline int Mine::GetRadius() { return this->m_radius; }
inline int Mine::GetMinX() { return this->m_minX; }
inline int Mine::GetMaxX() { return this->m_maxX; }
inline int Mine::GetMinY() { return this->m_minY; }
inline int Mine::GetMaxY() { return this->m_maxY; }
inline ExplosionInfo* Mine::GetExplosionInfo() { return this->m_explosionInfo; }

