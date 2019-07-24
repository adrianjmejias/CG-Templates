#pragma once
#include "types.h"
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

bool GLLogCall(const char* function, const char* file, int line);

#define GLASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	GLASSERT(GLLogCall(#x, __FILE__, __LINE__))\

void GLClearError();

std::ostream& operator<<(std::ostream& os, const Vec3& v);
std::istream& operator>> (std::istream& is, Vec3& v);


// Macros cant be at eof
