

//#include <glad/glad.h>
//#include <SDL.h>
#include "pch.h"
#include "Scene.h"

#include <imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
//#include "./AssetsManagement/AssetsManager.h"
//#include "AssetsManagement/ModelsManager.h"

namespace PF
{

	class SceneManager
	{
	protected:
		std::vector<Owns<Scene>> scenesLoaded;

	public:
		Scene& AddScene(Scene* scene)
		{
			scenesLoaded.emplace_back(scene);

			return *scene;
		}

		void Update(const ImGuiIO& io)
		{
			for (auto& s : scenesLoaded)
			{
				for (auto& go : s->rootObjects)
				{
					go->Update(io);
				}
			}
		}

	};
	struct EngineConfig
	{
		bool showFPS = true;
	};

    class Engine : public SceneManager
    {
    public:

		EngineConfig config;
		//ModelsManager modelsManager;
		//InputSystem input;
		SDL_Window* win;
		SDL_GLContext glContext;
		int win_width = 800;
		int win_heigth = 600;
		bool running;
		bool captureMouse = false;
		bool show_demo_window = true;
		bool show_another_window = true;	

        void InitContext();
		void CleanContext();

		void LoopMain();
		void LoopEvents();
		void LoopUpdate()
		{
			auto& io = ImGui::GetIO();
			for (auto& s : scenesLoaded)
			{
				s->Update(io);
			}
		}

    };



	
}