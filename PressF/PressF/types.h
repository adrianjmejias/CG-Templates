#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <functional>
#define STL_BYTE_SIZE(cont, type) (cont.size() * sizeof(type))
//#include <OBJ_Loader.h>
using Color = glm::vec4;
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using Vec4 = glm::vec4;
using iVec3 = glm::ivec3;
using Mat4 = glm::mat4;

using Int = int;
using Float = float;
using UInt = unsigned int;

template<typename ObjType>
using Owns = std::unique_ptr<ObjType>;

template<typename ObjType>
using Shares = std::shared_ptr<ObjType>;

template<typename ObjType>
using Ref = std::weak_ptr<ObjType>;


template <typename TT>
struct Range
{
	TT min = 0;
	TT max = 10;

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




//
//
//void Assign(Vec3& a, const objl::Vector3& b);
//void Assign(Vec2& a, const objl::Vector2& b);