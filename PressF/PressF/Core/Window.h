#include "PressF/pch.h"
#pragma once

namespace PF
{
	class Window
	{
		static Owns<Window> instance;
		Window() = default;
		public:
		static Window* GetInstance();
		SDL_Window* win;
		int width = 800;
		int heigth = 600;
		
		
	};
}