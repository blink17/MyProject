#include "ParkingLots.h"


ParkingLots::ParkingLots()
{
}


ParkingLots::~ParkingLots()
{
}

void ParkingLots::Init(int space, int tollgatePayingSec, int maxWaitingSec, int openSec, int closeSec)
{
	this->m_space = space;
	this->m_tollgatePayingSec = tollgatePayingSec;
	this->m_maxWaitingSec = maxWaitingSec; 
	this->m_openSec = openSec; 
	this->m_closeSec = closeSec;
}

int ParkingLots::GetSpace() { return this->m_space; }
int ParkingLots::GetTollgatePayingSec() { return this->m_tollgatePayingSec; }
int ParkingLots::GetMaxWaitingSec() { return this->m_maxWaitingSec; }
int ParkingLots::GetOpenSec() { return this->m_openSec; }
int ParkingLots::GetCloseSec() { return this->m_closeSec; }

bool ParkingLots::IsOpen(int currSec)
{
	return currSec >= this->m_openSec;
}

bool ParkingLots::IsClose(int currSec)
{
	return currSec >= this->m_closeSec;
}