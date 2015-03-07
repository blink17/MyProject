#include <iostream>
#include "UserNode.h"


UserNode::UserNode(string* userID)
{
	this->m_userID = *userID;
}


UserNode::~UserNode()
{
}

void UserNode::DestroyNode()
{
	delete this->m_childLeft;
	delete this->m_childRight;
}

void UserNode::AddLeft(UserNode* left)
{
	if (this->m_childLeft != NULL)
	{
		delete this->m_childLeft;
	}
	this->m_childLeft = left;
}

void UserNode::AddRight(UserNode* right)
{
	if (this->m_childRight != NULL)
	{
		delete this->m_childRight;
	}
	this->m_childRight = right;
}

void UserNode::AddFriendScore(UserNode* friendUser, int addScore)
{
	unsigned	myFriendSize	= this->m_friendList.size();
	bool		isExist			= false;
	FriendInfo*	friendInfo;
	string*		friendUserID	= friendUser->GetUserID();

	for (unsigned int i = 0; i < myFriendSize; ++i)
	{
		if (this->m_friendList[i].friendUser == friendUser)
		{
			friendInfo	= &this->m_friendList[i];
			isExist		= true;
			break;
		}
	}

	if (isExist == true)
	{
		friendInfo->friendScore += addScore;
	}
	else
	{
		FriendInfo newFriendInfo;
		newFriendInfo.friendUser = friendUser;
		newFriendInfo.friendScore = addScore;
        this->m_friendList.push_back(newFriendInfo);
	}
}

string*				UserNode::GetUserID()	{ return &this->m_userID; }
UserNode*			UserNode::GetLeft()		{ return this->m_childLeft; }
UserNode*			UserNode::GetRight()	{ return this->m_childRight; }
vector<FriendInfo>* UserNode::GetFriendList() { return &this->m_friendList; }
