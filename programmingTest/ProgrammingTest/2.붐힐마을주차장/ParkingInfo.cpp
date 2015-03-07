#include "ParkingInfo.h"
#include "CarInfo.h"


ParkingInfo::ParkingInfo()
{
}


ParkingInfo::~ParkingInfo()
{
}

void ParkingInfo::Init(int listOrder, CarInfo* carInfo, int arriveSec, int takeJobSec)
{
	this->m_listOrder = listOrder;
	this->m_carInfo = carInfo;
	this->m_arriveSec = arriveSec;
	this->m_takeJobSec = takeJobSec;
}

int ParkingInfo::GetListOrder() { return this->m_listOrder; }
CarInfo* ParkingInfo::GetCarInfo() { return this->m_carInfo; }
int ParkingInfo::GetArriveSec() { return this->m_arriveSec; }
int ParkingInfo::GetTakeJobSec() { return this->m_takeJobSec; }