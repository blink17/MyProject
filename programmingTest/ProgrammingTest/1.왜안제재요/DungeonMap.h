#pragma once

#include <list>
#include <unordered_map>

using namespace std;
class UserNode;

class DungeonMap
{
private:
	unordered_map<int, list<UserNode*>*> m_map;

public:
	DungeonMap();
	~DungeonMap();

	void AddDungeonInfo(int dungeonID, UserNode* userInfo);
};

