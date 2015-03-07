#pragma once

#include <string>
#include <vector>
#include "UserTree.h"

using namespace std;
class UserNode;

struct FriendInfo
{
	UserNode*	friendUser;
	int			friendScore;
};

class UserNode
{
private:
	string				m_userID;
	UserNode*			m_childLeft		= NULL;
	UserNode*			m_childRight	= NULL;
	vector<FriendInfo>	m_friendList;

public:
	UserNode(string* userID);
	~UserNode();

	void				DestroyNode();

	void				AddLeft(UserNode* left);
	void				AddRight(UserNode* right);
	void				AddFriendScore(UserNode* friendUser, int addScore);

	string*				GetUserID();
	UserNode*			GetLeft();
	UserNode*			GetRight();
	vector<FriendInfo>* GetFriendList();
};

