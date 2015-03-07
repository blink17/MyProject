#include "OrderList.h"
#include <algorithm>

#include "CarCatalog.h"
#include "ParkingInfo.h"

OrderList::OrderList()
{
}


OrderList::~OrderList()
{
}

void OrderList::AddOrder(CarCatalog* catalog, int listOrder, wstring type, int arriveSec, int takeJobSec)
{
	CarInfo* carInfo = catalog->GetCarInfo(type);

	ParkingInfo parkingInfo;
	parkingInfo.Init(listOrder, carInfo, arriveSec, takeJobSec);

	this->m_orders.push_back(parkingInfo);
}

ParkingInfo* OrderList::GetOrder(unsigned int index)
{
	if (index >= this->m_orders.size())
		return NULL;

	return &this->m_orders[index];
}

int OrderList::GetOrderCount()
{
	return this->m_orders.size();
}

bool CompareParkingInfo(ParkingInfo a, ParkingInfo b)
{
	if (a.GetArriveSec() == b.GetArriveSec())
		return a.GetListOrder() < b.GetListOrder();

	return a.GetArriveSec() < b.GetArriveSec();
}

void OrderList::Sort()
{
	size_t size = this->m_orders.size();
	std::sort(this->m_orders.begin(), this->m_orders.end(), CompareParkingInfo);
}
