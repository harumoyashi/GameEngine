#include "NTimer.h"
#include <Windows.h>

void NTimer::Reset()
{
	timer_ = 0;
	isTimeOut_ = false;
}

void NTimer::Update(const bool isRoop,const float elapseTimer)
{
	if (timer_ < maxTimer_)
	{
		timer_ += 1.0f * elapseTimer;
	}
	else
	{
		timer_ = maxTimer_;
		isTimeOut_ = true;
		if (isRoop == true)
		{
			Reset();
		}
	}
}

void NTimer::SubTimer(const float subTimer, const float elapseTimer)
{
	timer_ -= subTimer * elapseTimer;
}

float NTimer::GetNowTime(const TimeType& timeType)
{
	switch (timeType)
	{
	case TimeType::MilliSecond:
		return (float)GetTickCount64();
		break;
	case TimeType::Second:
		return (float)((GetTickCount64() / 1000) % 60);
		break;
	case TimeType::Minute:
		return (float)((GetTickCount64() / 60000) % 60);
		break;
	}

	return -1;
}
