#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>

#include "Utils.h"
#include "CarCatalog.h"
#include "ParkingLots.h"
#include "OrderList.h"
#include "ParkingSimulation.h"

using namespace std;

#define MAX_INPUT_LINE_SIZE 100

void ReadSettings(string fileName, ParkingLots* parkingLots, CarCatalog* carCatalog);
void ReadDatas(string fileName, OrderList* orderList, CarCatalog* carCatalog);

int main(int argc, char* argv[])
{
    std::locale::global(std::locale("Korean"));

	ParkingLots parkingLots;
	CarCatalog carCatalog;
	OrderList orderList;

	string settingFilename	= "SettingS.ini";
	string DataFilename		= "DataS.txt";
	if (argc >= 3)
	{
		settingFilename = argv[1];
		DataFilename = argv[2];
	}
	cout << "Setting File : " << settingFilename << endl;
	cout << "Data File : " << DataFilename << endl;

	ReadSettings(settingFilename, &parkingLots, &carCatalog);
	ReadDatas(DataFilename, &orderList, &carCatalog);
	orderList.Sort();

	ParkingSimulation parkingSimulation;
	parkingSimulation.Init(&parkingLots, &carCatalog, &orderList);
	parkingSimulation.Run();

	// 1)
	wcout << "1) " << SecondsToDateTime(parkingSimulation.GetFirstFullSec()) << endl;
	
	// 2)
	int inWait = (int)((float)parkingSimulation.GetTotalInWaitSec() / (float)parkingSimulation.GetCurrOrderIndex() + 0.5f);
	int outWait = (int)((float)parkingSimulation.GetTotalOutWaitSec() / (float)parkingSimulation.GetCurrOrderIndex() + 0.5f);
	cout << "2) " << inWait << ", " << outWait << endl;

	// 3)
	cout << "3) " << parkingSimulation.GetTotalFee() << endl;

	cout << "Press Any Key..." << endl;
    getchar();
    return 0;
}

void ReadSettingValue(wchar_t line[], wchar_t value[])
{
	size_t lineSize = wcslen(line);
	size_t i = 0;
	for (; i < lineSize; ++i)
	{
		if (line[i] == '=')
		{
			++i;
			break;
		}
	}

	int j = 0;
	for (; i < lineSize; ++i)
	{
		if (line[i] == ' ' || line[i] == '\t')
			continue;

		value[j] = line[i];
		++j;
	}
	value[j] = NULL;
}

void ReadSettings(string fileName, ParkingLots* parkingLots, CarCatalog* carCatalog)
{
	wifstream inputFile(fileName, ios::in | ios::binary);
	if (inputFile.is_open() == false)
	{
		cout << "Fail to open input file." << endl;
		inputFile.close();
		exit(EXIT_FAILURE);
	}
	inputFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	wchar_t	inputString[MAX_INPUT_LINE_SIZE];
	wchar_t valueString[MAX_INPUT_LINE_SIZE];
	bool	isReadParkingLots = false;
	bool	isReadType = false;

	while (!inputFile.eof())
	{
		if (isReadParkingLots == false)
		{
			int space = 0;
			int tollgatePayingSec = 0;
			int maxWaitingSec = 0;
			int openSec = 0;
			int closeSec = 0;

			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			ReadSettingValue(inputString, valueString);
			space = stoi(valueString);

			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			ReadSettingValue(inputString, valueString);
			tollgatePayingSec = stoi(valueString);

			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			ReadSettingValue(inputString, valueString);
			maxWaitingSec = stoi(valueString);

			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			ReadSettingValue(inputString, valueString);
			openSec = DateTimeToSeconds(valueString);

			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			ReadSettingValue(inputString, valueString);
			closeSec = DateTimeToSeconds(valueString);

			parkingLots->Init(space, tollgatePayingSec, maxWaitingSec, openSec, closeSec);
			isReadParkingLots = true;
		}
		else
		{
			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			size_t inputSize = wcslen(inputString);
			if (inputSize < 2)
				continue;

			wstring type;
			int delaySec = 0;
			int payInHour = 0;

			// Type
			int j = 0;
			for (size_t i = 0; i < inputSize; ++i)
			{
				if (inputString[i] == '[')
					continue;

				if (inputString[i] == ']')
					break;

				valueString[j] = inputString[i];
				++j;
			}
			valueString[j] = 0;
			type = valueString;

			// Comment
			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);

			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			ReadSettingValue(inputString, valueString);
			delaySec = stoi(valueString);

			inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
			ReadSettingValue(inputString, valueString);
			payInHour = stoi(valueString);

			carCatalog->AddCatalog(type, delaySec, payInHour);
		}
	}
	inputFile.close();
}

void ReadDatas(string fileName, OrderList* orderList, CarCatalog* carCatalog)
{
	wifstream inputFile(fileName, ios::in | ios::binary);
	if (inputFile.is_open() == false)
	{
		cout << "Fail to open input file." << endl;
		inputFile.close();
		exit(EXIT_FAILURE);
	}
	inputFile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	wchar_t	inputString[MAX_INPUT_LINE_SIZE];
	wchar_t valueString[MAX_INPUT_LINE_SIZE];
	int     orderNum = 0;

	inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);

	while (!inputFile.eof())
	{
		inputFile.getline(inputString, MAX_INPUT_LINE_SIZE);
		size_t inputSize = wcslen(inputString);
		if (inputSize < 2)
			continue;

		wstring type;
		int arriveSec = 0;
		int takeJobSec = 0;

		// Type
		size_t	i = 0;
		int		j = 0;
		for (; i < inputSize; ++i)
		{
			if (inputString[i] == ' ' || inputString[i] == '\t')
				break;

			valueString[j] = inputString[i];
			++j;
		}
		valueString[j] = 0;
		type = valueString;

		while (inputString[i] == ' ' || inputString[i] == '\t')
			++i;

		// ArriveSec
		j = 0;
		for (; i < inputSize; ++i)
		{
			if (inputString[i] == ' ' || inputString[i] == '\t')
				break;

			valueString[j] = inputString[i];
			++j;
		}
		valueString[j] = 0;
		arriveSec = DateTimeToSeconds(valueString);

		while (inputString[i] == ' ' || inputString[i] == '\t')
			++i;

		// TakeJobSec
		j = 0;
		for (; i < inputSize; ++i)
		{
			if (inputString[i] == ' ' || inputString[i] == '\t')
				break;

			valueString[j] = inputString[i];
			++j;
		}
		valueString[j] = 0;
		takeJobSec = stoi(valueString);

		orderList->AddOrder(carCatalog, orderNum, type, arriveSec, takeJobSec);

		++orderNum;
	}
	inputFile.close();
}