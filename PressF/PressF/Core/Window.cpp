#include "PressF/pch.h"
#include "Window.h"


namespace PF
{
	Owns<Window> Window::instance = nullptr;


	Window* Window::GetInstance()
	{
		if (instance.get() == nullptr)
		{
			instance.reset(new Window());
			instance->win = SDL_CreateWindow("PressF",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				instance->width, instance->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
		}

		return instance.get();
	}

}

