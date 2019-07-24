#pragma once
#include <spdlog\spdlog.h>
#include <ostream>
class Logger
{
public:
	Logger();
	~Logger();
};
#define PF_INFO(...)      spdlog::info(__VA_ARGS__)
#define PF_ERROR(...)     spdlog::error(__VA_ARGS__)
#define PF_DEBUG(...)     spdlog::debug(__VA_ARGS__)
#define PF_WARN(...)      spdlog::warn(__VA_ARGS__)
#define PF_CRITICAL(...)  spdlog::critical(__VA_ARGS__)

#define PF_ASSERT(x) if(!(x)){\
	PF_ERROR("PF_ASSERT FAILED : {0}" , #x);\
	PF_ERROR("LINE : {0}", __LINE__);\
	PF_ERROR("FILE : {0}", __FILE__);\
		__debugbreak();\
	}\

//std::ostream& operator<<(std::ostream& os, const Vec3& v)
//{
//	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
//	return os;
//}
//std::istream& operator>> (std::istream& is, Vec3& v)
//{
//	is >> v.x >> v.y >> v.z;
//	return is;
//}



// Macros cant be at eof
