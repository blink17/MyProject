#pragma once
#include <string>

using namespace std;

class CarInfo
{
private:
	wstring m_type;
	int m_delaySec;
	int m_payInHour;

public:
	CarInfo(wstring type, int delaySec, int payInHour);
	~CarInfo();

	wstring* GetType();
	int GetDelaySec();
	int GetPayInHour();

	int CalcPay(int seconds);
};

