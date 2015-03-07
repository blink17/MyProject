#pragma once
class ParkingLots
{
private:
	int m_space;
	int m_tollgatePayingSec;
	int m_maxWaitingSec;
	int m_openSec;
	int m_closeSec;

public:
	ParkingLots();
	~ParkingLots();

	void Init(int space, int tollgatePayingSec, int maxWaitingSec, int openSec, int closeSec);

	int GetSpace();
	int GetTollgatePayingSec();
	int GetMaxWaitingSec();
	int GetOpenSec();
	int GetCloseSec();

	bool IsOpen(int currSec);
	bool IsClose(int currSec);
};

