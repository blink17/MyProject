#include "CarInfo.h"


CarInfo::CarInfo(wstring type, int delaySec, int payInHour) :
m_type(type), m_delaySec(delaySec), m_payInHour(payInHour)
{
}


CarInfo::~CarInfo()
{
}

wstring* CarInfo::GetType() { return &this->m_type; }
int CarInfo::GetDelaySec() { return this->m_delaySec; }
int CarInfo::GetPayInHour() { return this->m_payInHour; }

int CarInfo::CalcPay(int seconds)
{
	int hours = seconds / 3600;
	if (seconds % 3600 > 0)
		++hours;

	return this->m_payInHour * hours;
}
