#include "PressF/pch.h"
#include "PressF/AssetsManagement/Scene.h"
#include "PressF/AssetsManagement/AssetsManager.h"
#include "PressF/Rendering/Renderer.h"
#include "PressF/ImGui/imgui_impl_sdl.h"
#include "PressF/ImGui/imgui_impl_opengl3.h"

namespace PF
{

	//class SceneManager
	//{
	//protected:
	//	std::vector<Owns<Scene>> scenesLoaded;

	//public:
	//	Scene& AddScene(Scene* scene)
	//	{
	//		scenesLoaded.emplace_back(scene);

	//		return *scene;
	//	}
	//	void Update(const ImGuiIO& io)
	//	{
	//		for (auto& s : scenesLoaded)
	//		{
	//			for (auto& go : s->rootObjects)
	//			{
	//				go->Update(io);
	//			}
	//		}
	//	}
	//};
	struct EngineConfig
	{
		bool showFPS = true;
	};

    class Engine
    {
    public:
		AssetsManager assetManager;
		Renderer* renderer;
		Scene scene;
		EngineConfig config;


		SDL_Window* win;
		SDL_GLContext glContext;
		int win_width = 800;
		int win_heigth = 600;
		bool running;
		bool captureMouse = false;
		bool show_demo_window = true;
		bool show_another_window = true;	
		Engine() = default;
		GameObject& AddGameObject(GameObject* go)
		{
			scene.AddGameObject(go);
			return *go;
		}
		
        void InitContext();
		void CleanContext();

		void Start();
		void InitRender();
		void EndRender();
		void LoopImGui();


		void LoopEvents();
		void LoopUpdate();
    };
}	