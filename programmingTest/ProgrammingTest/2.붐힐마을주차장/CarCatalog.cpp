#include "CarCatalog.h"
#include "CarInfo.h"

CarCatalog::CarCatalog()
{
}


CarCatalog::~CarCatalog()
{
}

void CarCatalog::AddCatalog(wstring type, int delaySec, int payInHour)
{
	this->m_catalog.insert(unordered_map<wstring, CarInfo>::value_type(type, CarInfo(type, delaySec, payInHour)));
}

CarInfo* CarCatalog::GetCarInfo(wstring type)
{
	unordered_map<wstring, CarInfo>::iterator findIter = this->m_catalog.find(type);
	if (findIter == this->m_catalog.end())
		return NULL;

	return &findIter->second;
}