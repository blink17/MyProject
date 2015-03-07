#include "UserTree.h"
#include <iostream>
#include <fstream>
#include <list>
#include <unordered_set>
#include <algorithm>

UserTree::UserTree()
{
	
}


UserTree::~UserTree()
{
	DestroyTree(this->m_root);
}

UserNode* UserTree::Find(string* targetID)
{
	if (this->m_root == NULL)
	{
		return NULL;
	}

	UserNode*	current = this->m_root;
	int			ret = 0;

	while (current != NULL)
	{
		ret = targetID->compare(*current->GetUserID());
		// userID == currentID
		if (ret == 0)
		{
			return current;
		}
		// userID < currentID
		else if (ret < 0)
		{
			if (current->GetLeft() == NULL)
			{
				break;
			}
			else
			{
				current = current->GetLeft();
				continue;
			}
		}
		// userID > currentID
		else
		{
			if (current->GetRight() == NULL)
			{
				break;
			}
			else
			{
				current = current->GetRight();
				continue;
			}
		}
	}
	return NULL;
}

UserNode* UserTree::AddUser(string* userID)
{
	UserNode*	newNode = NULL;

	if (this->m_root == NULL)
	{
		newNode = new UserNode(userID);
		this->m_root = newNode;
		return newNode;
	}

	UserNode*	current = this->m_root;
	int			ret		= 0;

	while (current != NULL)
	{
		ret = userID->compare(*current->GetUserID());
		// userID == currentID
		if (ret == 0)
		{
			return current;
		}
		// userID < currentID
		else if (ret < 0)
		{
			if (current->GetLeft() == NULL)
			{
				newNode = new UserNode(userID);
				current->AddLeft(newNode);
				break;
			}
			else
			{
				current = current->GetLeft();
				continue;
			}
		}
		// userID > currentID
		else
		{
			if (current->GetRight() == NULL)
			{
				newNode = new UserNode(userID);
				current->AddRight(newNode);
				break;
			}
			else
			{
				current = current->GetRight();
				continue;
			}
		}
	}
	return newNode;
}

// Visit : left - root - right
void UserTree::VisitAll(ostream& outStream)
{
	if (this->m_root == NULL)
		return;

	list<UserNode*> stack;
	stack.push_front(this->m_root);

	UserNode* node;
	UserNode* left;
	UserNode* right;

	unordered_set<UserNode*> visitChecker;

	while (stack.size() > 0)
	{
		node = stack.front();
		if (node == NULL)
			break;
			
		left = node->GetLeft();
		if (left != NULL && (visitChecker.find(left) == visitChecker.end()))
		{
			stack.push_front(left);
			continue;
		}

		visitChecker.insert(node);
		stack.pop_front();

		/////
        this->PrintNode(node, outStream);
		/////

		right = node->GetRight();
		if (right != NULL && (visitChecker.find(right) == visitChecker.end()))
		{
			stack.push_front(right);
			continue;
		}
	}
}

bool CompareFriendInfo(FriendInfo a, FriendInfo b)
{
    if (a.friendScore == b.friendScore)
        return (*a.friendUser->GetUserID()).compare(*b.friendUser->GetUserID()) < 0;

    return a.friendScore > b.friendScore;
}

void UserTree::PrintNode(UserNode* node, ostream& outStream)
{
    outStream << *node->GetUserID() << ":";

	vector<FriendInfo>* friendList = node->GetFriendList();
    unsigned int		friendSize = friendList->size();
    std::sort(friendList->begin(), friendList->end(), CompareFriendInfo);

	for (unsigned int i = 0; i < friendSize; ++i)
	{
        FriendInfo current = (*friendList)[i];
        outStream << *current.friendUser->GetUserID() << "(" << current.friendScore << ")";

        if (i + 1 < friendSize)
        {
            if (i >= 2)
            {
                FriendInfo next = (*friendList)[i + 1];
                if (current.friendScore > next.friendScore)
                    break;
            }
            outStream << ",";
        }
	}
    outStream << endl;
}

void UserTree::DestroyTree(UserNode* tree)
{
	if (tree->GetRight() != NULL)
		this->DestroyTree(tree->GetRight());

	if (tree->GetLeft() != NULL)
		this->DestroyTree(tree->GetLeft());

	delete tree;
}