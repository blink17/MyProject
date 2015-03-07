#pragma once
#include <vector>
#include <string>

using namespace std;
class ParkingInfo;
class CarCatalog;

class OrderList
{
private:
	vector<ParkingInfo>	m_orders;

public:
	OrderList();
	~OrderList();

	void AddOrder(CarCatalog* catalog, int listOrder, wstring type, int arriveSec, int takeJobSec);
	ParkingInfo* GetOrder(unsigned int index);
	int GetOrderCount();
	void Sort();
};

