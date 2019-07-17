#pragma once
#include <memory>
//#include <// glfw/// glfw3.h>
#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
//#include <imgui/imgui_impl_// glfw.h>
//#include <imgui/imgui_impl_opengl3.h>
using WindowType = SDL_Window;
using GLContext = SDL_GLContext;
//template< class T > using MyType = AnotherType< T, MyAllocatorType >;


class Window {
public:
	Window() {

	}
	bool gameMode = false;
	//unsigned int width = 800;
	//// Window current height
	//unsigned int height = 600;
	// Window title
	const char *windowTitle = "Adrian Mejias";
	// Window pointer
	SDL_Window* window;
	SDL_GLContext gl_context;

	bool isOpen = true;
	ImGuiIO *io;
	glm::vec4 clear_color{ 1,0,0,1 };

	friend class Scene;
	void SetSize(int width, int height) {

		//TwWindowSize(width, height);
		glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);

	}

	

	void SetGameMode(bool mode) {
		if (mode) {
			// glfwSetInputMode(window, // glfw_CURSOR, // glfw_CURSOR_DISABLED);
		}
		else {
			// glfwSetInputMode(window, // glfw_CURSOR, // glfw_CURSOR_NORMAL);

		}
	}
	unsigned int GetWidth() {
		return static_cast<int>(io->DisplaySize.x);
	}
	unsigned int GetHeight() {
		return static_cast<int>(io->DisplaySize.y);
	}

	bool Init() {

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
		{
			printf("Error: %s\n", SDL_GetError());

			__debugbreak();
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, window_flags);
		gl_context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, gl_context);
		SDL_GL_SetSwapInterval(1); // Enable vsync

		bool err = gladLoadGL() == 0;
		printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
		if (err)
		{
			fprintf(stderr, "Failed to initialize OpenGL loader!\n");
			__debugbreak();
			return false;
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init();



		return true;
	}
	
	void Start() {

	}
	
	~Window() {
		std::cout << "Bye " << __FILE__ << std::endl;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}


 
	void Close() {
		// glfwSetWindowShouldClose(window, true);
	}
	
	bool EarlyUpdate() {
		
		if (!isOpen) {
			__debugbreak();
			return false;
		}



		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		return true;
	}
	void LateUpdate() {

		ImGui::Render();
		glViewport(0, 0, (int) io->DisplaySize.x, (int)io->DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}
	

};

