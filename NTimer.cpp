#include "NTimer.h"
#include <Windows.h>

void NTimer::Reset()
{
	timer = 0;
	isTimeOut = false;
}

void NTimer::Update()
{
	if (timer < maxTimer)
	{
		timer++;
	}
	else
	{
		timer = maxTimer;
		isTimeOut = true;
	}
}

void NTimer::SubTimer(const float& subTimer)
{
	timer -= subTimer;
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
