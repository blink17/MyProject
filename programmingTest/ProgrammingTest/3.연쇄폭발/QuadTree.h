#pragma once

using namespace std;
class ExplosionMap;
class QuadTreeNode;
class Mine;

class QuadTree
{
private:
	QuadTreeNode* m_root;
	int m_depth;
	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;
	ExplosionMap* m_explosionMap;

	void BuildTree(QuadTreeNode* parent, int depth);
	void AddLeaf(Mine* mine, QuadTreeNode* current);

public:
	QuadTree();
	~QuadTree();

	static const int CHILD_COUNT = 4;

	void Build(int depth, int minX, int maxX, int minY, int maxY);
	void Add(Mine* mine);
};

