#include "Editor.h"






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


	engine.assetManager.LoadModel("quad", "../assets/models/adri.obj");

	fbrowser.SetTitle("title");
	fbrowser.SetTypeFilters({ ".h", ".cpp" });

	PF::Material* defaultMat = new PF::Material();

	


	//defaultMat->AddParameter(new PF::ShaderInt());


	PF::GameObject& ts1 = engine.AddGameObject(new PF::GameObject("test subject 1"));
	PF::GameObject& ts2 = engine.AddGameObject(new PF::GameObject("test subject 2"));

	ts1.AddComponent<Rotator>();
	PF::MeshRenderer* ms = ts1.AddComponent<PF::MeshRenderer>();

	PF::Camera* cam = ts2.AddComponent<PF::Camera>();
	ts2.AddComponent<CameraController>();
	cam->Yaw = -5601.39600;
	cam->Pitch = -70.9999771;
	cam->transform->SetPosition({ 2.38617706 , 7.60761261, -1.88124359 });
	cam->updateCameraVectors();


	auto model = engine.assetManager.GetModel("quad");
	auto mesh = model->meshes[0].get();
	ms->mat = mesh->defaultMaterial;
	ms->mesh = mesh;

	json j = ts1.Serialize();

	std::ofstream o("pretty.json");
	o << j << std::endl;
	//PF::MeshRenderer& mr = ts1.AddComponent<PF::MeshRenderer>();

	//mr.mesh = &model.meshes[0];

	//ts2.AddComponent<ParticleSystem>()->Start();

	//PF_INFO("number of gameobjects {0}", scene.rootObjects.size());
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
