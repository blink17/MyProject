#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class CarInfo;

class CarCatalog
{
private:
	unordered_map<wstring, CarInfo> m_catalog;

public:
	CarCatalog();
	~CarCatalog();

	void AddCatalog(wstring type, int delaySec, int payInHour);
	CarInfo* GetCarInfo(wstring type);
};