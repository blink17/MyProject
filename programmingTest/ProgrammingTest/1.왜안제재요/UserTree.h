#pragma once

#include <string>
#include "UserNode.h"

using namespace std;
class UserNode;

class UserTree
{
private:
	UserNode* m_root = NULL;
	
public:
	UserTree();
	~UserTree();

	UserNode* Find(string* targetID);
    void VisitAll(ostream& outStream);
    void PrintNode(UserNode* node, ostream& outStream);
	UserNode* AddUser(string* userID);
	void DestroyTree(UserNode* tree);
};

