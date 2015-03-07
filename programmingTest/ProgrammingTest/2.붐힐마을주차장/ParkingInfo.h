#pragma once

class CarInfo;

class ParkingInfo
{
private:
	int m_listOrder;
	CarInfo* m_carInfo;
	int m_arriveSec;
	int m_takeJobSec;

public:
	ParkingInfo();
	~ParkingInfo();

	void Init(int listOrder, CarInfo* carInfo, int arriveSec, int takeJobSec);

	int GetListOrder();
	CarInfo* GetCarInfo();
	int GetArriveSec();
	int GetTakeJobSec();
};

