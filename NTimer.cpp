#include "NTimer.h"

void NTimer::Reset()
{
	timer = maxTimer;
	isTimeOut = true;
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
