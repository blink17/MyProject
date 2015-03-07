#pragma once

#include <iostream>
#include <string>

using namespace std;

int DateTimeToSeconds(wchar_t dateTime[])
{
	int hours	= 0;
	int mins	= 0;
	int seconds = 0;
	wchar_t buff[3];

	size_t size = wcslen(dateTime);
	size_t i = 0;
	size_t j = 0;
	for (; i < size; ++i)
	{
		if (dateTime[i] == ':')
		{
			++i;
			break;
		}

		if (j < 2)
		{
			buff[j] = dateTime[i];
			++j;
		}
	}
	buff[j] = 0;
	hours = stoi(buff);

	j = 0;
	for (; i < size; ++i)
	{
		if (dateTime[i] == ':')
		{
			++i;
			break;
		}

		if (j < 2)
		{
			buff[j] = dateTime[i];
			++j;
		}
	}
	buff[j] = 0;
	mins = stoi(buff);

	j = 0;
	for (; i < size; ++i)
	{
		if (j < 2)
		{
			buff[j] = dateTime[i];
			++j;
		}
	}
	buff[j] = 0;
	seconds = stoi(buff);

	return hours * 3600 + mins * 60 + seconds;
}

wstring SecondsToDateTime(int allTimeSec)
{
	int hours	= 0;
	int mins	= 0;
	int seconds = 0;

	hours		= allTimeSec / 3600;
	allTimeSec	= allTimeSec % 3600;
	mins		= allTimeSec / 60;
	seconds		= allTimeSec % 60;

	wchar_t buff[10];
	swprintf(buff, 10, L"%d:%02d:%02d", hours, mins, seconds);
	return buff;

	//wstring wss;
	//wstring wss = to_wstring(hours) + L":" + to_wstring(mins) + L":" + to_wstring(seconds);
	//return wss;
}