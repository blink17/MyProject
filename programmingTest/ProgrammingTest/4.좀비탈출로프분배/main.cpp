#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <math.h>

using namespace std;

#define MAX_INPUT_LINE_SIZE 50

bool CompareRopeLength(float a, float b);
int CutRope(vector<int>* ropeList, int minLength);
int FindStandardRopeIdx(vector<int>* ropeList, int ropeNeed);
void GetOptimalRopeLength(vector<int>* ropeList, int max, int min, int ropeNeed, int &optimalLength, int &cutCount);

int main(int argc, char* argv[])
{
    clock_t t = clock();

	string inputFilename = "sample1.txt";
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

    char            inputString[MAX_INPUT_LINE_SIZE];
    char            headerBuffer[MAX_INPUT_LINE_SIZE];
    int             ropeCount = 0;
    int             ropeNeed  = 0;
    vector<int>*    ropeList = NULL;
    int             ropeIndex = 0;
    int             multipleFactor = 100;

    // Parsing Header
    if (!inputFile.eof())
    { 
        inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
        size_t size = strlen(inputString);
        size_t i    = 0;
        int j = 0;
        for (; i < size; ++i)
        {
            if (inputString[i] == ' ' || inputString[i] == '\t')
                break;

            headerBuffer[j] = inputString[i];
            ++j;
        }
        headerBuffer[j] = 0;
        ropeCount = atoi(headerBuffer);

        j = 0;
        for (; i < size; ++i)
        {
            if (inputString[i] == ' ' || inputString[i] == '\t')
                continue;

            headerBuffer[j] = inputString[i];
            ++j;
        }
        headerBuffer[j] = 0;
        ropeNeed = atoi(headerBuffer);
    }

    ropeList    = new vector<int>(ropeCount);
    ropeIndex   = 0;
    // Parsing Values
    while (!inputFile.eof() && ropeIndex < ropeCount)
    {
        inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
        (*ropeList)[ropeIndex] = (int)(atof(inputString) * multipleFactor);
        ++ropeIndex;
    }
    inputFile.close();

    // Sort Values
    std::sort(ropeList->begin(), ropeList->end(), CompareRopeLength);

    int standardIdx = FindStandardRopeIdx(ropeList, ropeNeed);
    int max = (*ropeList)[standardIdx];
    int min = 0;
    if (standardIdx > 0)
        min = (*ropeList)[standardIdx - 1];

    int optimalLength   = 0;
    int cutCount        = 0;

    GetOptimalRopeLength(ropeList, max, min, ropeNeed, optimalLength, cutCount);

    int realNum     = optimalLength / multipleFactor;   // �ڿ����κ�
    int pointNum    = optimalLength % multipleFactor;   // �Ҽ��κ�

    //cout << "Optimal Length : " << optimalLength << "(" << realNum << "." << pointNum << ")" << ", Cut Count : " << cutCount << endl;

	cout << "Result : " << realNum << "." << pointNum << endl;

    t = clock() - t;
    cout << "take " << ((float)t) / CLOCKS_PER_SEC << " seconds" << endl;

	delete ropeList;

	cout << "Press Any Key..." << endl;
    getchar();
    return 0;
}

bool CompareRopeLength(float a, float b)
{
    return a < b;
}

int CutRope(vector<int>* ropeList, int minLength)
{
    int     resultCount = 0;
    int     cache       = 0;
    size_t  size        = ropeList->size();

    for (size_t i = 0; i < size; ++i)
    {
        if (i > 0 && (*ropeList)[i] == (*ropeList)[i - 1])
        {
            resultCount += cache;
            continue;
        }

        cache = (*ropeList)[i] / minLength;
        resultCount += cache;
    }

    return resultCount;
}

int FindStandardRopeIdx(vector<int>* ropeList, int ropeNeed)
{
    size_t size = ropeList->size();
    size_t i = 0;
    int    ropeCount = 0;

    for (; i < size; ++i)
    {
        ropeCount = CutRope(ropeList, (*ropeList)[i]);
        if (ropeCount < ropeNeed)
            break;
    }

    return i;
}

void GetOptimalRopeLength(vector<int>* ropeList, int max, int min, int ropeNeed, int &optimalLength, int &cutCount)
{
    optimalLength   = 0;
    cutCount = 0;

    while (true)
    {
        optimalLength   = (min + max) / 2;
        cutCount        = CutRope(ropeList, optimalLength);

        if (optimalLength == min || optimalLength == max)
            break;

        if (cutCount >= ropeNeed)
            min = optimalLength;
        else
            max = optimalLength;
    }
}
