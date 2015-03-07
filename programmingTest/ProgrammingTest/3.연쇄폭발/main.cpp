#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "QuadTree.h"
#include "ExplosionMap.h"
#include "Mine.h"
#include "SpaceBoard.h"

using namespace std;

#define MAX_INPUT_LINE_SIZE 100
#define MAX_POS_X           999999
#define MAX_POS_Y           999999
#define TREE_DEPTH			9

char inputString[MAX_INPUT_LINE_SIZE];
char buffer[MAX_INPUT_LINE_SIZE];

void ParseLine(char inputString[], int& posX, int& posY, int& radius);
bool CompareMine(Mine* a, Mine* b);

// Static Init..
ExplosionMap* ExplosionMap::s_instance = NULL;

int main(int argc, char* argv[])
{
    clock_t t = clock();

	string inputFilename = "input1.txt";
	if (argc >= 2)
	{
		inputFilename = argv[1];
	}
	cout << "File : " << inputFilename << endl;

	ifstream inputFile(inputFilename, ios::in | ios::binary);
    if (inputFile.is_open() == false)
    {
        cout << "Fail to open input file." << endl;
        inputFile.close();
        exit(EXIT_FAILURE);
    }
    int     numOfMine   = 0;
    int     posX        = 0;
    int     posY        = 0;
    int     radius      = 0;

    inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
	numOfMine = atoi(inputString);

	Mine* mineArray = new Mine[numOfMine];

	/*QuadTree		tree;
	tree.Build(TREE_DEPTH, 0, MAX_POS_X, 0, MAX_POS_Y, &explosionMap);*/

	SpaceBoard		spaceBoard;
	spaceBoard.Build(0, MAX_POS_X, 0, MAX_POS_Y);

	cout << "Build Complete. (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << ")" << endl;

	Mine*			mine	= NULL;
	size_t			size	= 0;
	int				index	= 0;

    while (!inputFile.eof())
    {
        inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
		size = strlen(inputString);
		if (size < 2)
			continue;

        ParseLine(inputString, posX, posY, radius);

		mine = &mineArray[index++];
		mine->Init(posX, posY, radius);

		spaceBoard.Add(mine);
    }
    inputFile.close();

	cout << "Parse Complete. (" << ((float)(clock() - t)) / CLOCKS_PER_SEC << ")" << endl;

	cout << "Combine Mines..." << endl;

	spaceBoard.CombineMineEachSpace();

	cout << "Result : " << ExplosionMap::Instance()->GetExplosionCount() << endl;

    t = clock() - t;
    cout << "take " << ((float)t) / CLOCKS_PER_SEC << " seconds" << endl;

	delete[] mineArray;

	cout << "Press Any Key..." << endl;
	getchar();
    return 0;
}

bool CompareMine(Mine* a, Mine* b)
{
	return a->GetX() + a->GetRadius() < b->GetX() + b->GetRadius();
}

void ParseLine(char inputString[], int& posX, int& posY, int& radius)
{
    posX = 0;
    posY = 0;
    radius = 0;

    size_t size = strlen(inputString);
    size_t i = 0;

    int j = 0;
    for (; i < size; ++i)
    {
        if (inputString[i] == ' ' || inputString[i] == '\t')
            break;

        buffer[j] = inputString[i];
        ++j;
    }
    buffer[j] = 0;
    posX = atoi(buffer);

    while (inputString[i] == ' ' || inputString[i] == '\t')
        ++i;

    j = 0;
    for (; i < size; ++i)
    {
        if (inputString[i] == ' ' || inputString[i] == '\t')
            break;

        buffer[j] = inputString[i];
        ++j;
    }
    buffer[j] = 0;
    posY = atoi(buffer);

    while (inputString[i] == ' ' || inputString[i] == '\t')
        ++i;

    j = 0;
    for (; i < size; ++i)
    {
        if (inputString[i] == ' ' || inputString[i] == '\t')
            break;

        buffer[j] = inputString[i];
        ++j;
    }
    buffer[j] = 0;
    radius = atoi(buffer);
}