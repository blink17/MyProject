#include <iostream>
#include <fstream>
#include <string>

#include "UserTree.h"
#include "DungeonMap.h"

using namespace std;

#define MAX_INPUT_LINE_SIZE 50
#define MAX_USER_ID_SIZE	20
#define MAX_DUNGEON_ID_SIZE	10

void ParseLine(char inputString[], char parseUserID[], char parseDungeonID[]);

int main(void)
{
	ifstream inputFile("INPUT.TXT", ios::in | ios::binary);
	if (inputFile.is_open() == false)
	{
		cout << "Fail to open input file." << endl;
		inputFile.close();
		exit(EXIT_FAILURE);
	}

	DungeonMap	dungeonMap;
	UserTree	tree;
	UserNode*	userNode;
	char		inputString[MAX_INPUT_LINE_SIZE];
	char		inputUserID[MAX_USER_ID_SIZE];
	char		inputDungeonID[MAX_DUNGEON_ID_SIZE];
	string*		userID;
	int			dungeonID = 0;

	while (!inputFile.eof())
	{
		memset(inputUserID, 0, MAX_USER_ID_SIZE);
		memset(inputDungeonID, 0, MAX_DUNGEON_ID_SIZE);

		inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);

		ParseLine(inputString, inputUserID, inputDungeonID);
		userID		= new string(inputUserID);
		dungeonID	= atoi(inputDungeonID);

		if (userID->compare("*") == 0 && dungeonID == 0)
			break;

		userNode	= tree.Find(userID);
		if (userNode == NULL)
			userNode = tree.AddUser(userID);

		dungeonMap.AddDungeonInfo(dungeonID, userNode);

		//cout << "UserID : " << inputUserID << ", DungeonID : " << dungeonID << endl;
	}
	inputFile.close();

    ofstream outputFile("OUTPUT.TXT", ios::out | ios::binary | ios::trunc);
    {
        //tree.VisitAll(cout);
        tree.VisitAll(outputFile);
        outputFile.close();
    }

	cout << "Press Any Key..." << endl;
	getchar();
	return 0;
}

void ParseLine(char inputString[], char parseUserID[], char parseDungeonID[])
{
	int inputLength		= strlen(inputString);
	int dungeonIdIndex	= 0;

	for (int i = 0; i < inputLength; ++i)
	{
		if (inputString[i] == ' ')
		{
			parseUserID[i] = NULL;
			dungeonIdIndex = i + 1;
			break;
		}
		parseUserID[i] = inputString[i];
	}

	for (int i = dungeonIdIndex; i < inputLength; ++i)
	{
		if (inputString[i] == ' ')
		{
			parseDungeonID[i] = NULL;
			dungeonIdIndex = i + 1;
			break;
		}
		parseDungeonID[i - dungeonIdIndex] = inputString[i];
	}
}