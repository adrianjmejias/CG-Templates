#pragma once
#include "Range.h"

struct Clock
{
	using TickFunction = std::function<void()>;
	TickFunction onTimePassed;
	RangeFloat time;
	void Update(float deltaTime)
	{
		time.min += deltaTime;

		if (time.min >= time.max)
		{
			onTimePassed();
			time.min -= time.max;
		}

	}
};
