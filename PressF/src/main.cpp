/* hello.c */
#include "main.h"
#include <stdio.h>
#include <iostream>
#include <memory>


inline void InitLog() {
	spdlog::set_pattern("[%H:%M:%S %z] [%^%L: %v %$] [t %t]");

}

inline void InitGlad() {

}

inline void InitSDL() {
	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		PF_ASSERT("SDL2 video subsystem couldn't be initialized. Error: {0}", SDL_GetError());
		exit(1);
	}

	SDL_Window* window = SDL_CreateWindow("Glad Sample",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN |
		SDL_WINDOW_OPENGL);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		std::cerr << "SDL2 Renderer couldn't be created. Error: "
			<< SDL_GetError()
			<< std::endl;
		exit(1);
	}

	// Create a OpenGL context on SDL2
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	// Load GL extensions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize the OpenGL context." << std::endl;
		exit(1);
	}

	// Loaded OpenGL successfully.
	std::cout << "OpenGL version loaded: " << GLVersion.major << "."
		<< GLVersion.minor << std::endl;

	// Create an event handler
	SDL_Event event;
	// Loop condition
	bool running = true;

	while (running) {
		SDL_PollEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				running = false;
				break;
			}
		}

		glClearColor(0, 0, 0, 1);

		// You'd want to use modern OpenGL here
		glColor3d(0, 1, 0);
		glBegin(GL_TRIANGLES);
		glVertex2f(.2f, 0);
		glVertex2f(.01f, .2f);
		glVertex2f(-.2f, 0);
		glEnd();

		SDL_GL_SwapWindow(window);
	}

	// Destroy everything to not leak memory.
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

inline void InitNuklear()
{

}

void Init() {

}



int main() {
	InitLog();

	InitSDL();
}