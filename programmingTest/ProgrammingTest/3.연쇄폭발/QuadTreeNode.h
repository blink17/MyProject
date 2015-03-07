#pragma once

#include <list>

using namespace std;
class Mine;
class ExplosionMap;

class QuadTreeNode
{
private:
	QuadTreeNode* m_parent;
	QuadTreeNode* m_children;
	int m_childrenCount;
	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;

	list<Mine*> m_mineList;

public:
	QuadTreeNode();
	~QuadTreeNode();

	void Init(QuadTreeNode* parent, int minX, int maxX, int minY, int maxY);

	QuadTreeNode* GetParent();
	QuadTreeNode* GetChildren();
	int GetChildrenCount();
	int GetMinX();
	int GetMaxX();
	int GetMinY();
	int GetMaxY();

	list<Mine*>*	GetMineList();

	QuadTreeNode*	MakeChildren(int childrenCount);
	void			AddMine(Mine* mine, ExplosionMap* explosionMap);
	bool			Intersect(Mine* mine);
};

