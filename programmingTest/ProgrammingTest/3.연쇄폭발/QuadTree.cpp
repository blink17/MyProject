#include "QuadTree.h"

#include <iostream>
#include "QuadTreeNode.h"
#include "ExplosionMap.h"
#include "Mine.h"

QuadTree::QuadTree()
{
}


QuadTree::~QuadTree()
{
}

void QuadTree::Build(int depth, int minX, int maxX, int minY, int maxY)
{
	this->m_depth = depth;
	this->m_minX = minX;
	this->m_maxX = maxX;
	this->m_minY = minY;
	this->m_maxY = maxY;

	this->m_root = new QuadTreeNode();
	this->m_root->Init(NULL, minX, maxX, minY, maxY);
	this->BuildTree(this->m_root, depth - 1);
}

void QuadTree::BuildTree(QuadTreeNode* parent, int depth)
{
	// if leaf node
	if (depth < 0)
		return;

	int midX = (parent->GetMinX() + parent->GetMaxX()) / 2;
	int midY = (parent->GetMinY() + parent->GetMaxY()) / 2;

	// build chlid
	// y x
	// 0 0 = 0
	// 0 1 = 1
	// 1 0 = 2
	// 1 1 = 3
	QuadTreeNode* children = parent->MakeChildren(QuadTree::CHILD_COUNT);

	children[0].Init(parent, parent->GetMinX(), midX, parent->GetMinY(), midY);
	this->BuildTree(&children[0], depth - 1);

	children[1].Init(parent, midX + 1, parent->GetMaxY(), parent->GetMinY(), midY);
	this->BuildTree(&children[1], depth - 1);

	children[2].Init(parent, parent->GetMinX(), midX, midY + 1, parent->GetMaxY());
	this->BuildTree(&children[2], depth - 1);

	children[3].Init(parent, midX + 1, parent->GetMaxY(), midY + 1, parent->GetMaxY());
	this->BuildTree(&children[3], depth - 1);
}

void QuadTree::Add(Mine* mine)
{
	this->AddLeaf(mine, this->m_root);
}

void QuadTree::AddLeaf(Mine* mine, QuadTreeNode* current)
{
	// leaf node
	QuadTreeNode* children = current->GetChildren();
	if (children == NULL)
	{
		//cout << "Leaf Size : " << current->GetMaxX() - current->GetMinX() << endl;
		current->AddMine(mine, this->m_explosionMap);
		return;
	}

	int childCount = current->GetChildrenCount();
	QuadTreeNode* childNode = NULL;
	for (int i = 0; i < childCount; ++i)
	{
		if (children[i].Intersect(mine))
		{
			this->AddLeaf(mine, &children[i]);
		}
	}
}