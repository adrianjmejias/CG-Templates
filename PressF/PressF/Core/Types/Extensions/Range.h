#include "../types.h"
#include "../Base/Base.h"
#pragma once

template <typename TT>
struct Range
{
	TT min{ 0 };
	TT max{ 10 };

	TT GetInterpolation(float t)
	{
		return min * t + (max - min) * t;
	}
	float GetPorcentage()
	{
		return max / static_cast<float>(min);
	}
};

struct RangeFloat : public Range<Float>
{
};

struct RangeInt : public Range<Int>
{
};