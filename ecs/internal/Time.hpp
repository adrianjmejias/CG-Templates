#pragma once

//#include <// glfw/// glfw3.h>
#include <functional>



 typedef double time__;
class Time{
	time__ time, dt;

    public:
    bool Init(){
		dt = 0;
		time = SDL_GetTicks();
		return true;
    }

    void Update(std::function<void()> f){
		dt = SDL_GetTicks() - time;
		if (dt <= 0) dt = 0;
		else{
			f();
		}
		time += dt;
    }

	time__ DeltaTime(){
        return dt;
    }
};