#pragma once

#include "Watch.h"
#include <vector>

using namespace std;
class ParkingLots;
class CarCatalog;
class OrderList;
class ParkingInfo;

class EnterInfo
{
public:
	int				startSec;
	int				endSec;
	ParkingInfo*	parkingInfo;
};

class StayInfo
{
public:
	int				startSec;
	int				endSec;
	ParkingInfo*	parkingInfo;
};

class ParkingSimulation
{
private:
	ParkingLots*	m_parkingLots;
	CarCatalog*		m_carCatalog;
	OrderList*		m_orderList;

	Watch			m_watch;
	int				m_currOrderIndex;
	int				m_maxOrderIndex;
	ParkingInfo*	m_onTollgate;
	int				m_firstFullSec;
	int				m_totalInWaitSec;
	int				m_totalOutWaitSec;
	int				m_totalFee;

	vector<StayInfo>		m_stayQueue;
	vector<EnterInfo>		m_enterQueue;


	void SortStayQueue();
	void SortEnterQueue();

public:
	ParkingSimulation();
	~ParkingSimulation();

	void Init(ParkingLots* parkingLots, CarCatalog* carCatalog, OrderList* orderList);
	void Run();

	ParkingInfo* ReadOrder(int index);
	void Toll(int currentSec);
	void Park(ParkingInfo* parkingInfo, int startSec, int waitSec);
	void Cancel(ParkingInfo* parkingInfo, int waitSec);
	void AddStayQueue(ParkingInfo* parkingInfo, int startSec);
	bool ExistSpace(int currentSec);
	int GetClosestSpaceOccurTime(int currentSec);

	int CalcEnterWaitSec(int startSec);
	void AddEnterQueue(ParkingInfo* parkingInfo, int startSec);
	EnterInfo* GetNearEnterInfo(int startSec);

	int GetCurrOrderIndex();
	int GetFirstFullSec();
	int GetTotalInWaitSec();
	int GetTotalOutWaitSec();
	int GetTotalFee();
};

