#pragma once


#define CGDEBUG
#ifdef  CGDEBUG
#define CGASSERT(x) if(!(x)){\
	std::cout<< "CGASSERT FAILED : " << #x <<std::endl;\
	std::cout<< "LINE : " <<__LINE__<<std::endl;\
	std::cout<< "FILE : " <<__FILE__<<std::endl;\
		__debugbreak();\
	}\

//TODO: PONER ELSE PARA PROD BUILD

#endif //  CGDEBUG




//#define forn()