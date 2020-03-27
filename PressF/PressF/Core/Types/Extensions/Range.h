#include "../types.h"
#include "../Base/Base.h"
#pragma once

template <typename TT>
struct Range
{
	TT min{ 0 };
	TT max{ 10 };

	constexpr TT GetInterpolation(float t)
	{
		TT a{min.value * t + (max.value - min.value) * t};
		return a;
	}
	constexpr float GetPorcentage()
	{
		return max.value / static_cast<float>(min.value);
	}
};

struct RangeFloat : public Range<Float>
{
};

struct RangeInt : public Range<Int>
{
};

struct RangeVec3 : public Range<Vec3>
{
};


void ImGuiRender(RangeInt value, const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");