#include "Watch.h"


Watch::Watch() :
m_prevSec(0), m_currSec(0)
{
}


Watch::~Watch()
{
}

void Watch::Init(int firstSec)
{
	this->m_currSec = firstSec;
	this->m_prevSec = firstSec;
}

void Watch::SetCurrSec(int currSec)
{
	this->m_prevSec = this->m_currSec;
	this->m_currSec = currSec;
}

int Watch::GetPrevSec() { return this->m_prevSec; }
int Watch::GetCurrSec() { return this->m_currSec; }
int Watch::GetDeltaSec() { return this->m_currSec - this->m_prevSec; }
