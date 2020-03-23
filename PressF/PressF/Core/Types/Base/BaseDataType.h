#pragma once
#include <glm/glm.hpp>

template <typename Type>
struct BaseDataType
{
public:
	Type value;
	
	BaseDataType() {
		value = 0;
	}
	constexpr BaseDataType(const Type& a)
	{
		value = a;
	}
	constexpr operator Type&()
	{
		return value;
	}
	
	constexpr Type operator=(const Type& t)
	{
		value = t;
		return value;
	}

	constexpr Type& operator*()
	{
		return value;
	}
	//template <typename N>
	//Type constexpr operator=(const BaseDataType<N>& t)
	//{
	//	value = t.value;
	//	return value;
	//}
};