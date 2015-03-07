#include "ParkingSimulation.h"
#include <algorithm>

#include "ParkingLots.h"
#include "CarCatalog.h"
#include "OrderList.h"
#include "ParkingInfo.h"
#include "CarInfo.h"

using namespace std;

ParkingSimulation::ParkingSimulation()
{
}


ParkingSimulation::~ParkingSimulation()
{
}

void ParkingSimulation::Init(ParkingLots* parkingLots, CarCatalog* carCatalog, OrderList* orderList)
{
	this->m_parkingLots = parkingLots;
	this->m_carCatalog = carCatalog;
	this->m_orderList = orderList;

	this->m_currOrderIndex = 0;
	this->m_maxOrderIndex = orderList->GetOrderCount() - 1;
	this->m_onTollgate = NULL;
	this->m_firstFullSec = 0;
	this->m_totalInWaitSec = 0;
	this->m_totalOutWaitSec = 0;
	this->m_totalFee = 0;
	this->m_stayQueue.clear();
	this->m_enterQueue.clear();
}

void ParkingSimulation::Run()
{
	//this->m_watch.Init(0);

	while (this->m_currOrderIndex <= this->m_maxOrderIndex)
	{
		// 1. 데이터를 읽어온다.
		ParkingInfo* currParkingInfo = this->ReadOrder(this->m_currOrderIndex);
		//this->m_watch.SetCurrSec(currParkingInfo->GetArriveSec());

		// 2. 주차장 영업이 끝남
		if (this->m_parkingLots->IsClose(currParkingInfo->GetArriveSec()) == true)
			break;

		// 3. 톨게이트에 대기하고 있는 사람 정산
		this->Toll(currParkingInfo->GetArriveSec());

		// 4. 오픈했음
		if (this->m_parkingLots->IsOpen(currParkingInfo->GetArriveSec()) == true)
		{
			if (this->ExistSpace(currParkingInfo->GetArriveSec()) == true)
			{
				this->Park(currParkingInfo, currParkingInfo->GetArriveSec(), 0);
				continue;
			}
			else
			{
				int nextSec		    = this->GetClosestSpaceOccurTime(currParkingInfo->GetArriveSec());
				int waitSec		    = nextSec - currParkingInfo->GetArriveSec();
				int enterWaitSec	= this->CalcEnterWaitSec(nextSec);
				waitSec += enterWaitSec;
				if (waitSec > this->m_parkingLots->GetMaxWaitingSec())
				{
					waitSec = this->m_parkingLots->GetMaxWaitingSec();
					this->Cancel(currParkingInfo, waitSec);
					continue;
				}
				else
				{
					this->Park(currParkingInfo, currParkingInfo->GetArriveSec() + waitSec, waitSec);
					continue;
				}
			}
		}
		// 5. 아직 오픈전
		else
		{
			// 오픈까지 기다릴 수 있는지 검사한다. 못기다리면 바로 대기시간을 기록하고 다음으로 넘어간다.
			int waitSec		    = this->m_parkingLots->GetOpenSec() - currParkingInfo->GetArriveSec();
			int enterWaitSec	= this->CalcEnterWaitSec(this->m_parkingLots->GetOpenSec());
			waitSec += enterWaitSec;
			if (waitSec > this->m_parkingLots->GetMaxWaitingSec())
			{
				waitSec = this->m_parkingLots->GetMaxWaitingSec();
				this->Cancel(currParkingInfo, waitSec);
				continue;
			}
			// 기다릴 수 있다면 주차시킨다.
			else
			{
				if (this->ExistSpace(currParkingInfo->GetArriveSec() + waitSec) == true)
				{
					this->Park(currParkingInfo, currParkingInfo->GetArriveSec() + waitSec, waitSec);
					continue;
				}
                // 오픈까지 기다릴 수 있지만 기다린 후 들어가려할때 주차공간이 없다면 다음 기다리는 시간을 구해서 진행한다.
				else
				{
                    int openStandbySec      = currParkingInfo->GetArriveSec() + waitSec;        // 오픈까지 기다린 시각
                    int openNextSec         = this->GetClosestSpaceOccurTime(openStandbySec);   // 다음출차 시각
                    int totalWaitSec        = openNextSec - currParkingInfo->GetArriveSec();    // 전체 대기 시간
                    int openEnterWaitSec    = this->CalcEnterWaitSec(openNextSec);
                    totalWaitSec += openEnterWaitSec;

                    if (totalWaitSec > this->m_parkingLots->GetMaxWaitingSec())
                    {
                        totalWaitSec = this->m_parkingLots->GetMaxWaitingSec();
                        this->Cancel(currParkingInfo, totalWaitSec);
                        continue;
                    }
                    else
                    {
                        this->Park(currParkingInfo, currParkingInfo->GetArriveSec() + totalWaitSec, totalWaitSec);
                        continue;
                    }
				}
			}
		}
	}

	// 6. 주차장 운영 및 모든 주차 오더가 만료되었으므로 남은 차들을 정산한다.
	this->Toll(INT_MAX);
}

ParkingInfo* ParkingSimulation::ReadOrder(int index)
{
	return this->m_orderList->GetOrder(index);
}

// 주어진 시간에 정산 할 수 있는 차들은 모두 정산한다. 큐에 있는 모든 것들을 뺀다.
void ParkingSimulation::Toll(int currentSec)
{
	// 주차한 차들중에 정산할 수 있는 차들 정리
	size_t size = this->m_stayQueue.size();
	while (size > 0)
	{
		StayInfo* stayInfo = &this->m_stayQueue.front();
		if (stayInfo->endSec <= currentSec)
		{
			this->m_totalFee += stayInfo->parkingInfo->GetCarInfo()->CalcPay(stayInfo->endSec - stayInfo->startSec);
			this->m_stayQueue.erase(this->m_stayQueue.begin());
			size = this->m_stayQueue.size();
		}
		else
		{
			break;
		}
	}

	// 입구에 들어서는 차들 정리
	size = this->m_enterQueue.size();
	while (size > 0)
	{
		EnterInfo* enterInfo = &this->m_enterQueue.front();
		if (enterInfo->endSec <= currentSec)
		{
			this->m_enterQueue.erase(this->m_enterQueue.begin());
			size = this->m_enterQueue.size();
		}
		else
		{
			break;
		}
	}
}

void ParkingSimulation::Park(ParkingInfo* parkingInfo, int startSec, int waitSec)
{
	this->AddStayQueue(parkingInfo, startSec);
	this->AddEnterQueue(parkingInfo, startSec);

	// 주차로 미뤄진 대기 시간 기록 Log
	this->m_totalInWaitSec += waitSec;
	++this->m_currOrderIndex;

	// 첫번째 만차시간
	if (this->m_firstFullSec == 0 && this->m_stayQueue.size() == this->m_parkingLots->GetSpace())
		this->m_firstFullSec = startSec;
}

void ParkingSimulation::Cancel(ParkingInfo* parkingInfo, int waitSec)
{
	// 주차로 미뤄진 대기 시간 기록 Log
	this->m_totalInWaitSec += waitSec;
	++this->m_currOrderIndex;
}

bool CompareStayInfo(StayInfo a, StayInfo b)
{
	if (a.endSec == b.endSec)
		return a.parkingInfo->GetListOrder() < b.parkingInfo->GetListOrder();

	return a.endSec < b.endSec;
}

void ParkingSimulation::SortStayQueue()
{
	std::sort(this->m_stayQueue.begin(), this->m_stayQueue.end(), CompareStayInfo);
}

void ParkingSimulation::AddStayQueue(ParkingInfo* parkingInfo, int startSec)
{
	StayInfo stayInfo;
	stayInfo.startSec = startSec;
	stayInfo.endSec = startSec + parkingInfo->GetCarInfo()->GetDelaySec() + parkingInfo->GetTakeJobSec();
	stayInfo.parkingInfo = parkingInfo;

	this->m_stayQueue.push_back(stayInfo);
	this->SortStayQueue();

	// 새로운 큐입력으로 인한 연쇄되는 출차 시간 미루기
	bool isFoundAdd = false;

	size_t size = this->m_stayQueue.size();
	for (size_t i = 0; i < size; ++i)
	{
		StayInfo* currInfo = &this->m_stayQueue[i];

		if (isFoundAdd == false && currInfo->parkingInfo->GetListOrder() == parkingInfo->GetListOrder())
			isFoundAdd = true;

		if (isFoundAdd == true)
		{
			if (i == 0)
				continue;

			StayInfo*	prevInfo = &this->m_stayQueue[i - 1];
			int			tollPassSec = prevInfo->endSec + this->m_parkingLots->GetTollgatePayingSec();
			if (prevInfo->endSec <= currInfo->endSec && currInfo->endSec <= tollPassSec)
			{
				// 출차로 미뤄진 대기 시간 기록 Log
				this->m_totalOutWaitSec += (tollPassSec - currInfo->endSec);

				currInfo->endSec = tollPassSec;
			}
		}
	} // for
}

bool ParkingSimulation::ExistSpace(int currentSec)
{
	size_t size = this->m_stayQueue.size();

	// 오픈한 경우라면 큐에 있는 숫자만으로 검출할 수 있다.
	if (currentSec >= this->m_parkingLots->GetOpenSec())
		return size < this->m_parkingLots->GetSpace();

	// 오픈 전이라면 예상되는 남은 공간을 계산한다.
	int occupySpace = 0;
	for (size_t i = 0; i < size; ++i)
	{
		StayInfo* stayInfo = &this->m_stayQueue[i];
		if (stayInfo->startSec <= currentSec && currentSec <= stayInfo->endSec)
			++occupySpace;
	}

	return occupySpace < this->m_parkingLots->GetSpace();
}

int ParkingSimulation::GetClosestSpaceOccurTime(int currentSec)
{
	// 이미 오픈한 경우. 가장 최근에 나갈 정보만 조회하면 된다.
	if (currentSec >= this->m_parkingLots->GetOpenSec())
		return this->m_stayQueue.front().endSec;

	size_t size = this->m_stayQueue.size();
	for (size_t i = 0; i < size; ++i)
	{
		StayInfo* stayInfo = &this->m_stayQueue[i];
		if (stayInfo->startSec <= currentSec && currentSec <= stayInfo->endSec)
			return stayInfo->endSec;
	}
}

// 진입하기 위한 대기큐를 검사하여 진입큐에서 얼마나 기다려야 되나 체크한다.
int ParkingSimulation::CalcEnterWaitSec(int startSec)
{
	EnterInfo* nearInfo = this->GetNearEnterInfo(startSec);
	if (nearInfo == NULL)
		return 0;

	if (nearInfo->endSec < startSec)
		return 0;

	return nearInfo->endSec - startSec;
}

bool CompareEnterInfo(EnterInfo a, EnterInfo b)
{
	return a.endSec < b.endSec;
}

void ParkingSimulation::SortEnterQueue()
{
	std::sort(this->m_enterQueue.begin(), this->m_enterQueue.end(), CompareEnterInfo);
}

void ParkingSimulation::AddEnterQueue(ParkingInfo* parkingInfo, int startSec)
{
	EnterInfo enterInfo;
	enterInfo.startSec = startSec;
	enterInfo.endSec = startSec + parkingInfo->GetCarInfo()->GetDelaySec();
	enterInfo.parkingInfo = parkingInfo;

	this->m_enterQueue.push_back(vector<EnterInfo>::value_type(enterInfo));
	this->SortEnterQueue();
}

EnterInfo* ParkingSimulation::GetNearEnterInfo(int startSec)
{
	if (this->m_enterQueue.size() == 0)
		return NULL;

	return &this->m_enterQueue.back();
}

int ParkingSimulation::GetCurrOrderIndex() { return this->m_currOrderIndex; }
int ParkingSimulation::GetFirstFullSec() { return this->m_firstFullSec; }
int ParkingSimulation::GetTotalInWaitSec() { return this->m_totalInWaitSec; }
int ParkingSimulation::GetTotalOutWaitSec() { return this->m_totalOutWaitSec; }
int ParkingSimulation::GetTotalFee() { return this->m_totalFee; }