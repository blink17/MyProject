#include "QuadTreeNode.h"

#include "ExplosionMap.h"
#include "ExplosionInfo.h"
#include "Mine.h"

QuadTreeNode::QuadTreeNode() :
m_parent(0), m_children(0)
{

}

QuadTreeNode::~QuadTreeNode()
{
	delete[] m_children;

	list<Mine*>::iterator iter;
	for (iter = this->m_mineList.begin(); iter != this->m_mineList.end(); ++iter)
		delete &(*iter);
}

void QuadTreeNode::Init(QuadTreeNode* parent, int minX, int maxX, int minY, int maxY)
{
	this->m_parent = parent; 
	this->m_minX = minX; 
	this->m_maxX = maxX;
	this->m_minY = minY;
	this->m_maxY = maxY;
}

QuadTreeNode* QuadTreeNode::GetParent() { return this->m_parent; }
QuadTreeNode* QuadTreeNode::GetChildren() { return this->m_children; }
int QuadTreeNode::GetChildrenCount() { return this->m_childrenCount; }
int QuadTreeNode::GetMinX() { return this->m_minX; }
int QuadTreeNode::GetMaxX() { return this->m_maxX; }
int QuadTreeNode::GetMinY() { return this->m_minY; }
int QuadTreeNode::GetMaxY() { return this->m_maxY; }
list<Mine*>* QuadTreeNode::GetMineList() { return &this->m_mineList; }


QuadTreeNode* QuadTreeNode::MakeChildren(int childrenCount)
{
	this->m_childrenCount	= childrenCount;
	this->m_children		= new QuadTreeNode[childrenCount];
	return this->m_children;
}

void QuadTreeNode::AddMine(Mine* mine, ExplosionMap* explosionMap)
{
	list<Mine*>::iterator iter;
	for (iter = this->m_mineList.begin(); iter != this->m_mineList.end(); ++iter)
	{
		Mine* existMine = *iter;
		if (existMine->IsSameExplosionInfo(mine))
			continue;

		if (existMine->IsContact(mine))
			explosionMap->Merge(existMine->GetExplosionInfo(), mine->GetExplosionInfo());
	}
	this->m_mineList.push_back(mine);
}

bool QuadTreeNode::Intersect(Mine* mine)
{
	if (this->m_minX > mine->GetX() + mine->GetRadius() || this->m_maxX < mine->GetX() - mine->GetRadius())
		return false;

	if (this->m_minY > mine->GetY() + mine->GetRadius() || this->m_maxY < mine->GetY() - mine->GetRadius())
		return false;

	return true;
}
