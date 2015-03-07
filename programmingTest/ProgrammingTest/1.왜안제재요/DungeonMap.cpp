#include "DungeonMap.h"

#include "UserNode.h"

DungeonMap::DungeonMap()
{
}


DungeonMap::~DungeonMap()
{
}

void DungeonMap::AddDungeonInfo(int dungeonID, UserNode* userInfo)
{
	list<UserNode*>* userList = NULL;
	unordered_map<int, list<UserNode*>*>::iterator findIter = this->m_map.find(dungeonID);

	// Add New
	if (findIter == this->m_map.end())
	{
		userList = new list<UserNode*>();
		this->m_map.insert(unordered_map<int, list<UserNode*>*>::value_type(dungeonID, userList));
	}
	else
	{
		userList = findIter->second;
	}

	string*		addedUserID = userInfo->GetUserID();
	int			ret			= 0;
	UserNode*	friendUser	= NULL;

	for (list<UserNode*>::iterator iter = userList->begin(); iter != userList->end(); ++iter)
	{
		friendUser = *iter;
		ret = addedUserID->compare(*friendUser->GetUserID());
		// Same User
		if (ret == 0)
			continue;

		userInfo->AddFriendScore(friendUser, 1);
		friendUser->AddFriendScore(userInfo, 1);
	}

	userList->push_back(userInfo);
}