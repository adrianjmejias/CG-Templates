#include "Editor.h"

#define BLOOM 1

using namespace PF;
namespace PF
{
	std::vector<GameObject*> GetAsSeparateGameObjects(Model& model)
	{
		std::vector<GameObject*> gos;

		gos.reserve(model.meshes.size());

		for (auto& m : model.meshes)
		{
			GameObject* go{ new GameObject(m->name) };
			MeshRenderer* mr = go->AddComponent<MeshRenderer>();
			
			mr->mesh = m.get();
			mr->mat = m->mat;
			gos.push_back(go);
		}
		return gos;
	}
}



void Editor::Init()
{
	engine.InitContext();
	////app.LoadShaders({});
	////PF::Scene scene;
	//engine.LoadModels(
	//		{
	//		"assets/models/cuboPower/simpleQuad.obj",
	//		//"assets/models/cuboPower/simpleCube.obj",
	//		//"assets/models/cuboPower/clown.obj",
	//		"assets/models/cuboPower/cuboPowerblend.obj"
	//	}
	//);
	//std::vector<PF::MeshRenderer&> mrs;

	//PF::ModelsManager& mm = engine.modelsManager;
	//PF::Model& model = mm.LoadAsset("../assets/models/cuboPower/simpleQuad.obj");

	//engine.load

	fbrowser.SetTitle("title");
	fbrowser.SetTypeFilters({ ".h", ".cpp" });
	PF::AssetsManager& am = engine.assetManager;


	Ref<PF::Model> modelQuad = am.LoadModel("quad", "../assets/models/ssao.obj");
	Ref<Texture> texHouse = am.LoadTexture("diffuseHouse", "../assets/models/house/cottage_diffuse.png");

	for (auto mat : modelQuad->materials)
	{
		mat->AddTexture("texture_diffuse", *texHouse);
	}

	{
		//engine.assetManager.LoadModel("thechosenone", "../assets/models/thechosenone.obj");
		//auto model = engine.assetManager.GetModel("thechosenone");
		//PF::GameObject& ts1 = engine.AddGameObject(new PF::GameObject("test subject 1"));
		//auto mr = ts1.AddComponent<PF::MeshRenderer>();
		//mr->SetGPUMesh(model->meshes[0].get());
		//mr->SetRenderMode(PF::MeshRenderMode::Shiny);

		//PF::ParticleSystem* ps = ts1.AddComponent<PF::ParticleSystem>();
		//ps->SetGPUMesh(model->meshes[0].get());
	}



	auto model = am.GetModel("quad");
	std::vector<PF::GameObject*> gos = PF::GetAsSeparateGameObjects(*model);
	for (auto go : gos)
	{
		engine.AddGameObject(go);
	}

	PF::GameObject& ts1 = engine.AddGameObject(new PF::GameObject("test subject 1"));
	ts1.AddComponent<Rotator>();







	PF::GameObject& ts2 = engine.AddGameObject(new PF::GameObject("test subject 2"));
	PF::Camera* cam = ts2.AddComponent<PF::Camera>();
	ts2.AddComponent<CameraController>();
	cam->Yaw = -5601.39600f;
	cam->Pitch = -70.9999771f;
	cam->transform->SetPosition({ 2.38617706f , 7.60761261f, -1.88124359f });
	cam->updateCameraVectors();
	*cam->speed = 27.f;
	*cam->MouseSensitivity = 7.f;

	//PF::Light* l = ts2.AddComponent<PF::Light>();


	//json j = ts1.Serialize();

	//std::ofstream o("pretty.json");
	//o << j << std::endl;


	PF::GameObject& ts3 = engine.AddGameObject(new PF::GameObject("Light"));
	ts3.AddComponent<PF::Light>();

	engine.Start();
	while (engine.running)
	{
		engine.LoopEvents();

		engine.InitRender();

		engine.LoopUpdate();

		engine.LoopImGui();

		LoopImGui();

		engine.EndRender();
	}

	engine.CleanContext();
}

void Editor::LoopImGui()
{
	auto io = ImGui::GetIO();


	ImGui::Begin("Editor window");

	ImGui::Text("asdasd");


	fbrowser.Display();

	if (fbrowser.HasSelected())
	{
		std::cout << "Selected filename" << fbrowser.GetSelected().string() << std::endl;
		fbrowser.ClearSelected();
	}
	ImGui::End();
}
