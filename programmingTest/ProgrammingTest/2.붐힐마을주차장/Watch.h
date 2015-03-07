#pragma once
class Watch
{
private:
	int m_prevSec;
	int m_currSec;

public:
	Watch();
	~Watch();

	void Init(int firstSec);
	void SetCurrSec(int currSec);

	int GetPrevSec();
	int GetCurrSec();
	int GetDeltaSec();
};

