#include "PressF/pch.h"
#include "Engine.h"


namespace PF
{
	void Engine::InitContext()
	{
		/* SDL setup */
		SDL_SetMainReady();
		SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		win = SDL_CreateWindow("PressF",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			win_width, win_heigth, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI );
		glContext = SDL_GL_CreateContext(win);
		//SDL_GetWindowSize(win, &win_width, &win_heigth);
		SDL_SetWindowResizable(win, (SDL_bool)true);
		//SDL_SetWindowBordered(win, SDL_FALSE);



		if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(captureMouse)) == -1) {
			__debugbreak();
		}
		// Load GL extensions using glad
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			std::cerr << "Failed to initialize the OpenGL context." << std::endl;
			__debugbreak();
		}

		// Loaded OpenGL successfully.
		std::cout << "OpenGL version loaded: " << GLVersion.major << "."
			<< GLVersion.minor << std::endl;
		/* OpenGL setup */
		glViewport(0, 0, win_width, win_heigth);


		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.WantCaptureKeyboard = false;
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL(win, glContext);
		ImGui_ImplOpenGL3_Init();
	}
	
	void Engine::CleanContext()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		//nk_sdl_shutdown();
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(win);
		SDL_Quit();
	}

	void Engine::InitRender()
	{
			glViewport(0, 0, win_width, win_heigth);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(win);
			ImGui::NewFrame();

			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0,0,0, 1);
	}

	void Engine::LoopUpdate()
	{
		auto& io = ImGui::GetIO();
		for (auto& s : scenesLoaded)
		{
			s->Update(io);
		}
	}

	void Engine::EndRender()
	{
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(win);
	}

	void Engine::LoopImGui()
	{
	
		auto io = ImGui::GetIO();

		if (ImGui::Begin("Engine"))
		{

			if (ImGui::Checkbox("Show FPS", &config.showFPS))
			{
				ImGui::Text("FPS: %f", io.Framerate);
			}

			ImGui::End();
		}

		ImGui::ShowDemoWindow(&show_demo_window);
	}

	void Engine::LoopEvents()
	{
		SDL_GetWindowSize(win, &win_width, &win_heigth);
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {
				running = false;
				break;
			}
			if (e.type == SDL_EventType::SDL_KEYDOWN) {

				auto keyPressed = e.key.keysym.scancode;
				switch (keyPressed)
				{
				case SDL_Scancode::SDL_SCANCODE_ESCAPE:
					running = false;
					break;
				case SDL_Scancode::SDL_SCANCODE_R:
					PF_INFO("Recompiling");

					for (auto& [key, val] : shaders)
					{
						val->ReCompile();
					}

					/*for (size_t ii = 0; ii < shaders.size(); ii++)
					{
					auto shader = shaders[ii];
					shader->ReCompile();
					}*/
					break;
				case SDL_Scancode::SDL_SCANCODE_C:
					if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(captureMouse)) == -1) {
						__debugbreak();
					}
					captureMouse = !captureMouse;
					break;

				case SDL_Scancode::SDL_SCANCODE_0:
					//renderPlane = !renderPlane;
					break;
				case SDL_Scancode::SDL_SCANCODE_9:
					//renderScene = !renderScene;
					break;
				case SDL_Scancode::SDL_SCANCODE_1:
				case SDL_Scancode::SDL_SCANCODE_2:
				case SDL_Scancode::SDL_SCANCODE_3:
				case SDL_Scancode::SDL_SCANCODE_4:
				case SDL_Scancode::SDL_SCANCODE_5:
				{
					int newCam = static_cast<int>(keyPressed) - static_cast<int>(SDL_Scancode::SDL_SCANCODE_1);
					//if (actCam != newCam) {
					//	PF_INFO("Swapped to Camera {0}", newCam);
					//	/*if (newCam < cameras.size()) {
					//		actCam = newCam;
					//		mainCamera = cameras[actCam];
					//		mainCamera->gameObject->AddComponent(fc);
					//		mainCamera->transform->SetDirty(Dirty::Model);
					//	}
					//	else {
					//		PF_WARN("Camera {0} not available", newCam);
					//	}*/
					//}
				}
				break;
				default:
					break;
				}
			}

			if (e.window.type == SDL_EventType::SDL_WINDOWEVENT) {
				switch (e.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					SDL_Log("Window %d size changed to %dx%d",
						e.window.windowID,
						e.window.data1,
						e.window.data2);
					break;
				}
			}

			/*for (auto go : rootNodes)
			{
			for (auto comp : go->components) {
			comp->HandleEvent(e);
			}
			}*/


			ImGui_ImplSDL2_ProcessEvent(&e);
		}
	}
}