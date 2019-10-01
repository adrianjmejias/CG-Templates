#include "types.h"
#include "Application.h"

extern double deltaTime = 0;
extern unsigned int GLOBAL_ID = 1;
extern Camera* mainCamera = nullptr;
enum class Dirty;

static unsigned int LoadVolume(const std::string &path, int width, int heigth, int depth)
{
	unsigned int id;
	const int size = width * heigth * depth;
	FILE *pFile;
	GLubyte* pVolume = new GLubyte[size];
	errno_t err;
	if ((err = fopen_s(&pFile, path.c_str(), "rb")) != 0) {
		std::cout << "ERROR:: Unable to load texture " << path << std::endl;
		glDeleteTextures(1, &id);
	}
	else {
		fread(pVolume, sizeof(GLubyte), size, pFile);
		fclose(pFile);

		// Load data into a 3D texture
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_3D, id);

		// Set the texture parameters
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, width, heigth, depth, 0, GL_RED, GL_UNSIGNED_BYTE, pVolume);
		delete[] pVolume;
	}

	return id;
}

static void ImGuiTransform(Transform &t) {
	if (ImGui::SliderFloat3("Rotation", &t.rotation[0], -360.f, 360.f)) {
		t.SetDirty(Dirty::Model);
	}
	if (ImGui::SliderFloat3("Scale", &t.scale[0], -20, 20)) {
		t.SetDirty(Dirty::Model);
	}
	if (ImGui::SliderFloat3("Position", &t.position[0], -20, 20)) {
		t.SetDirty(Dirty::Model);
	}
	//if (ImGui::("Rotation", &t.rotation[0])) {
	//	t.SetDirty(Dirty::Model);
	//}
}

static void ImGuiGameObject(GameObject * go)
{
	if (ImGui::TreeNode(go->name.c_str())) {

		ImGuiTransform(go->transform);

		for (auto c : go->transform.children)
		{
			ImGuiGameObject(&c->gameObject);
		}

		ImGui::TreePop();
	}
}

static void Traverse(const std::vector<GameObject*> objects, std::function<void(GameObject*)> beforefgo, std::function<void(GameObject*)> afterfgo, std::function<void(Component*)> fcomp) {
	std::queue<GameObject*, std::deque<GameObject *>> q(std::deque<GameObject*>(objects.begin(), objects.end()));
	while (!q.empty()) {
		GameObject *go = q.front();
		q.pop();
		PF_ASSERT(go && "game object is null");

		beforefgo(go);
		for (Component* comp : go->components) {
			PF_ASSERT(comp && "component is null");

			fcomp(comp);
		}
		afterfgo(go);

		for (Transform* t : go->transform.children) {
			PF_ASSERT(t && "Children is null");

			q.push(&t->gameObject);
		}
	}
}

void Application::Setup(const std::vector<std::string>& objPaths, const std::vector<std::tuple<std::string, std::string>>& shaderPaths)
{
	spdlog::set_pattern("[%M:%S %z] [%^%v%$]");
	SetupShaders(shaderPaths);
	SetupModels(objPaths);
	SetupScene();



	for (Model *model : models) {
		for (Mesh &mesh : *model) {
			auto matName = mesh.mat.name;
			if (materialsLoaded.find(matName) == end(materialsLoaded)) {
				materialsLoaded[matName] = &mesh.mat;
			}
		}
	}



	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Sets up shit. maybe not the best way but it's what came to me

	Traverse(rootNodes,
		[](GameObject* go) {
	},
		[](GameObject* go) {
	},
		[&](Component* comp) {
		Steal(comp);
	}
	);

}

void Application::SetupScene()
{
	depthFB = FrameBuffer::GetGBuffer(win_width, win_heigth); //FrameBuffer::GetShadowBuffer(win_width, win_heigth);

	firstPass = shaders[0];
	lastPass = shaders[1];
	renderQuad = shaders[2];
	shaderUI = shaders[3];
	volumeId = LoadVolume("assets/models/bonsai.raw", 256, 256, 256);
	vol.Setup(volumeId);

	//shaderQuad = shaders[4];
	{
		GameObject *go = new GameObject();
		fc = go->AddComponent < FlyingController >();
		go->AddComponent < Camera >();
		//cam = &go->AddComponent<CameraGL>();

		go->transform.SetPosition(0, 0, -20);
		rootNodes.push_back(go);
	}




	for (Model *model : models) {
		GameObject *papa = new GameObject();
		for (Mesh &mesh : *model) {
			GameObject *go = new GameObject();
			MeshRenderer *ren = go->AddComponent<MeshRenderer>(&mesh);
			mesh.push_back(ren);

			/*go->transform.SetPosition(0, 0, 0);
			go->transform.SetScale(3, 3, 3);
			go->transform.Rotate(0, 90, 0);*/
			go->transform.SetParent(&papa->transform);
			//go->transform.SetPosition(glm::ballRand(3.f) + 2.f);
			
		}
		rootNodes.push_back(papa);
	}




	transferFunc = rootNodes[2]->AddComponent<TransferenceFunction >();



}

Model * Application::SetupModel(std::string objPath)
{

	objl::Loader objlModel;

	if (!objlModel.LoadFile(objPath))
	{
		PF_ERROR("Failed to load model {0}", objPath);
		__debugbreak();
	}
	Model *model = GLCreate(objlModel);

	//cargar texturas
	const std::string basePath = objPath.substr(0, objPath.find_last_of("/"));
	for (objl::Material& mat : objlModel.LoadedMaterials)
	{
		ADD_MAP(map_bump);
		ADD_MAP(map_Ka);
		ADD_MAP(map_Kd);
		ADD_MAP(map_Ks);
		ADD_MAP(map_d);
	}

	for (Mesh& mesh : *model)
	{
		ADD_MAP_TO_MATERIAL(map_bump);
		ADD_MAP_TO_MATERIAL(map_Ka);
		ADD_MAP_TO_MATERIAL(map_Kd);
		ADD_MAP_TO_MATERIAL(map_Ks);
		ADD_MAP_TO_MATERIAL(map_d);
	}
	return model;
}

void Application::SetupModels(const std::vector<std::string>& objPaths)
{
	for (const std::string& objPath : objPaths)
	{
		models.push_back(SetupModel(objPath));
	}

	lightsModel = SetupModel("assets/models/light/untitled.obj");
	modelsDebug.push_back(lightsModel);



}

void Application::SetupShaders(const std::vector<std::tuple<std::string, std::string>>& shaderPaths)
{
	const std::string baseShaderFolder = "assets/shaders/";
	for (int ii = 0; ii < shaderPaths.size(); ii++)
	{
		auto tupleName = shaderPaths[ii];

		auto vertName = std::get<0>(tupleName);
		auto fragName = std::get<1>(tupleName);

		Shader *vert = nullptr;
		Shader *frag = nullptr;

		try
		{
			vert = shadersLoaded.at(vertName);
		}
		catch (const std::exception&)
		{
			vert = Shader::FromPath(baseShaderFolder + vertName, GL_VERTEX_SHADER);
			shadersLoaded[vertName] = vert;
		}

		try
		{
			frag = shadersLoaded.at(fragName);
		}
		catch (const std::exception&)
		{
			frag = Shader::FromPath(baseShaderFolder + fragName, GL_FRAGMENT_SHADER);
			shadersLoaded[fragName] = frag;
		}

		shaders.push_back(new ShaderProgram({ vert, frag }));
	}
}

void Application::LoopMain()
{
	while (running) {
		//std::cout << "looping";
		glViewport(0, 0, win_width, win_heigth);

		LoopEvents();

		LoopUpdate();

		LoopRender();

		LoopUI();

		SDL_GL_SwapWindow(win);

		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
		SDL_GetMouseState(&mouse_deltaX, &mouse_deltaY);
		mouse_deltaX -= mouse_lastPosX;
		mouse_deltaY -= mouse_lastPosY;
		mouse_deltaY *= -1;
	}
}

void Application::LoopUI()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(win);
	ImGui::NewFrame();


	ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Materials");
	for (auto pair : materialsLoaded)
	{
		const std::string& matName = pair.first;
		Material& mat = *pair.second;

		if ((ImGui::TreeNode(matName.c_str())))
		{
			//ImGui::SliderFloat4("kD", &mat.kD[0], 0.f,1.f);	
			ImGui::ColorEdit3("kD##2f", (float*)&mat.kD[0], ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("kA##2f", (float*)&mat.kS[0], ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("kS##2f", (float*)&mat.kA[0], ImGuiColorEditFlags_Float);
			ImGui::SliderFloat("Ni", &mat.Ni, 0.f, 1.f);
			ImGui::SliderFloat("Ns", &mat.Ns, 0.f, 3.f);
			ImGui::SliderInt("Shader", &mat.illum, 0, (int)(shaders.size() - 1));
			//mat.illum


			switch (mat.illum)
			{
			case 3:
				ImGui::SliderFloat("IOR Material", &mat.IOR, 0.f, 3.f);
				ImGui::SliderFloat("IOR Air", &IOR_BG, 0.f, 3.f);
				break;
			case 8:
				ImGui::SliderFloat("HeightScale", &heightScale, 0.f, 10.f);
				ImGui::SliderFloat("MinLayers", &minLayers, 6.f, maxLayers);
				ImGui::SliderFloat("MaxLayers", &maxLayers, minLayers, 400.f);

				break;
			default:
				break;
			}



			ImGui::TreePop();
		}
	}
	ImGui::End();


	ImGui::Begin("Lights");
	{
		
		Transform &camTransform = *mainCamera->transform;
		ImGui::Text("Camera transform");
		ImGuiTransform(camTransform);


		ImGui::SliderFloat4("orthoSize", &orthoSides[0], -100, 100);
		ImGui::SliderFloat2("clipping", &clippingPlane[0], 1, 600);


		for (Light* light : LIGHTS) {
			if (ImGui::TreeNode(light->gameObject->name.c_str())) {

				ImGuiTransform(*light->transform);

				ImGui::Checkbox("isOn", reinterpret_cast<bool*>(&light->enabled));
				ImGui::ColorEdit4("kD##2f", (float*)&light->kD[0], ImGuiColorEditFlags_Float);
				ImGui::ColorEdit4("kS##2f", (float*)&light->kS[0], ImGuiColorEditFlags_Float);
				ImGui::ColorEdit4("kA##2f", (float*)&light->kA[0], ImGuiColorEditFlags_Float);
				const char* types[] = { "Point","Directional", "Spotlight" };
				ImGui::Combo("Type", reinterpret_cast<int*>(&light->type), types, IM_ARRAYSIZE(types));

				switch (light->type)
				{
				case LightType::SPOTLIGHT:
					ImGui::SliderFloat("InnerAngle", &light->innerAngle, 5.f, light->outterAngle);
					ImGui::SliderFloat("OutterAngle", &light->outterAngle, light->innerAngle, 90.f);
					//falls
				case LightType::POINT:
					ImGui::SliderFloat3("attenuation", &light->attenuation[0], 0, 3);


				default:
					break;
				}



				ImGui::TreePop();
			}
		}
	}
	ImGui::End();


	ImGui::Begin("Objects");
	{
		for (auto go : rootNodes)
		{
			ImGuiGameObject(go);
		}
	}
	ImGui::End();



	bool showSelected = actGO != nullptr;
	ImGui::Begin("Selected Object", &showSelected);
	{
		//ImGui::LabelText(actGO->name.c_str(),);

	}
	ImGui::End();


	transferFunc->RenderUI();

	// Rendering
	ImGui::Render();
	SDL_GL_MakeCurrent(win, glContext);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::LoopUpdate()
{
	Traverse(rootNodes,
		[](GameObject* go) {},
		[](GameObject* go) {},
		[](Component* comp) {comp->Update(); }
	);
}

void Application::LoopRender()
{
	Camera &cam = *cameras[actCam];
	Transform camTransform = *cam.transform;
	float ar = win_width / static_cast<float>(win_heigth);
	camTransform.TryGetClean();
	const Mat4 &view = Transform::GetView(camTransform);
	const Mat4 &projection = Transform::GetProjection(camTransform, true, 45.f);
	const Mat4 &ortho = Transform::GetProjection(camTransform, false, 45.f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//vol.Render(deltaTime, view, projection, renderQuad);

	GLuint cubeVAO = vol.cubeVAO;

	GameObject *go = rootNodes[1];
	Transform& cubeTransform = go->transform;

	auto MVP = cubeTransform.GetAccumulated();
	
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFB->id);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);


		GLCALL(glBindVertexArray(cubeVAO));

		firstPass->Use();
		firstPass->SetUniform("MVP", MVP);

		GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
	}


	{

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		transferFunc->Render(shaderUI, vol.quadVAO, win_width);

		GLCALL(glBindVertexArray(cubeVAO));

		lastPass->Use();
		lastPass->SetUniform("windowSize", Vec2(win_width, win_heigth));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthFB->texture.id);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_3D, volumeId);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, transferFunc->texPreview);

		lastPass->SetUniform("bfCoords", 0);
		lastPass->SetUniform("volume", 1);
		lastPass->SetUniform("tf", 2);
		lastPass->SetUniform("MVP", MVP);
		lastPass->SetUniform("deltaTime", vol.timePassed);

		GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));

	}
	








}

void Application::DrawObjects(std::vector<const Mesh*> meshes, std::function<bool(const ShaderProgram&shader, const Material&MAT)> PreReqs)
{

	auto meshesToRender = meshes;
	for (const Mesh * const mesh : meshesToRender) {
		const Material &MAT = mesh->mat;
		const ShaderProgram &shader = *shaders[MAT.illum];

		shader.Use();
		GLCALL(glBindVertexArray(mesh->VAO));

		PreReqs(shader, MAT);

		for (auto obj : *mesh) {
			PF_ASSERT(obj && "Renderer is null");
			const Mat4 &MVP = obj->transform->MVP;
			const glm::mat3 &normalMatrix = obj->transform->normalMatrix;

			SET_UNIFORM(shader, MVP);
			SET_UNIFORM(shader, normalMatrix);

			GLCALL(glDrawElements(GL_TRIANGLES, mesh->nElem, GL_UNSIGNED_INT, 0));
		}
	}
	GLCALL(glBindVertexArray(0));

}

void Application::LoopEvents()
{
	SDL_GetWindowSize(win, &win_width, &win_heigth);
	LAST = NOW;
	SDL_GetMouseState(&mouse_lastPosX, &mouse_lastPosY);
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
				for (size_t ii = 0; ii < shaders.size(); ii++)
				{
					auto shader = shaders[ii];
					shader->ReCompile();
				}
				break;
			case SDL_Scancode::SDL_SCANCODE_C:
				if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(captureMouse)) == -1) {
					__debugbreak();
				}
				captureMouse = !captureMouse;
				break;

			case SDL_Scancode::SDL_SCANCODE_0:
				renderPlane = !renderPlane;
				break;
			case SDL_Scancode::SDL_SCANCODE_9:
				renderScene = !renderScene;
				break;
			case SDL_Scancode::SDL_SCANCODE_1:
			case SDL_Scancode::SDL_SCANCODE_2:
			case SDL_Scancode::SDL_SCANCODE_3:
			case SDL_Scancode::SDL_SCANCODE_4:
			case SDL_Scancode::SDL_SCANCODE_5:
			{
				int newCam = static_cast<int>(keyPressed) - static_cast<int>(SDL_Scancode::SDL_SCANCODE_1);
				if (actCam != newCam) {
					PF_INFO("Swapped to Camera {0}", newCam);
					if (newCam < cameras.size()) {
						actCam = newCam;
						mainCamera = cameras[actCam];
						mainCamera->gameObject->AddComponent(fc);
						mainCamera->transform->SetDirty(Dirty::Model);
					}
					else {
						PF_WARN("Camera {0} not available", newCam);
					}
				}
			}
			break;
			default:
				break;
			}
		}

		if (e.window.type == SDL_EventType::SDL_WINDOWEVENT) {
			switch (e.window.event) {
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				win_width = e.window.data1;
				win_heigth = e.window.data2;




				SDL_Log("Window %d size changed to %dx%d",
					e.window.windowID,
					e.window.data1,
					e.window.data2);
				break;
			}
		}

		for (auto go : rootNodes)
		{
			for (auto comp : go->components) {
				comp->HandleEvent(e);
			}
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
}

Model * Application::GLCreate(objl::Loader & fullModel) {
	Model * m = new Model();
	Model &model = *m;
	//data transform
	//std::vector<Vertex> vertex;
	//vertex.reserve(fullModel.LoadedVertices.size());
	//for (const objl::Vertex& oVertex : fullModel.LoadedVertices) {
	//	vertex.push_back(Vertex(oVertex));
	//}

	GLsizei totalIndices = 0;
	model.reserve(fullModel.LoadedMeshes.size());





	std::vector<Vertex> vertex{begin(fullModel.LoadedVertices), end(fullModel.LoadedVertices)};

	std::vector<Vec3> tanPerFace;
	tanPerFace.reserve(fullModel.LoadedIndices.size() / 3);

	std::vector<std::vector<size_t> > vertexContrib{ vertex.size(), std::vector<size_t>() };

	for (size_t iiFace = 0, iiReal = 0; iiReal < fullModel.LoadedIndices.size() - 2; iiFace++, iiReal += 3)
	{
		const int idx_a = fullModel.LoadedIndices[iiReal];
		const int idx_b = fullModel.LoadedIndices[iiReal + 1];
		const int idx_c = fullModel.LoadedIndices[iiReal + 2];

		const Vertex &v_1 = vertex[idx_a];
		const Vertex &v_2 = vertex[idx_b];
		const Vertex &v_3 = vertex[idx_c];


		// hago esto por legibilidad
		const Vec3 &u{ v_1.uv[0],v_2.uv[0],v_3.uv[0] };
		const Vec3 &v{ v_1.uv[1],v_2.uv[1],v_3.uv[1] };
		const Vec3 p[3]{ v_1.pos,v_2.pos ,v_3.pos };


		const float v20 = v[2] - v[0];
		const float v10 = v[1] - v[0];

		const float u10 = u[1] - u[0];
		const float u20 = u[2] - u[0];

		const Vec3 p10 = p[1] - p[0];
		const Vec3 p20 = p[2] - p[0];


		const Vec3 tan(((v20*p10) - (v10 * p20)) / ((u10*v20) - (v10*u20)));


		tanPerFace.push_back(tan);

		vertexContrib[idx_a].push_back(iiFace);
		vertexContrib[idx_b].push_back(iiFace);
		vertexContrib[idx_c].push_back(iiFace);
	}

	for (size_t ii = 0; ii < vertexContrib.size(); ii++)
	{
		Vec3 promTan{ 0,0,0 };


		for (size_t indexFace : vertexContrib[ii])
		{
			promTan += tanPerFace[indexFace];
		}

		vertex[ii].tan = glm::normalize(promTan / static_cast<float>(vertexContrib[ii].size()));
	}

	for (int ii = 0; ii < fullModel.LoadedMeshes.size(); ii++) {
		objl::Mesh& objlMesh = fullModel.LoadedMeshes[ii];
		Mesh myMesh;
		myMesh.name = objlMesh.MeshName;

		std::vector<Vertex> meshVertex{begin(objlMesh.Vertices), end(objlMesh.Vertices) };
		

		myMesh.mat = objlMesh.MeshMaterial;
		myMesh.nElem = static_cast<GLsizei>(objlMesh.Indices.size());
		myMesh.offset = totalIndices;

		totalIndices += myMesh.nElem;

		GLCALL(glGenVertexArrays(1, &myMesh.VAO));
		GLCALL(glGenBuffers(1, &myMesh.EBO));
		GLCALL(glGenBuffers(1, &myMesh.VBO));

		GLCALL(glBindVertexArray(myMesh.VAO));
		{
			GLCALL(glBindBuffer(GL_ARRAY_BUFFER, myMesh.VBO));
			GLCALL(glBufferData(GL_ARRAY_BUFFER, meshVertex.size() * sizeof(Vertex), meshVertex.data(), GL_STATIC_DRAW));

			GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.EBO));
			GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, objlMesh.Indices.size() * sizeof(unsigned int), objlMesh.Indices.data(), GL_STATIC_DRAW));

			{
				GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)));
				GLCALL(glEnableVertexAttribArray(0));

				GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
				GLCALL(glEnableVertexAttribArray(1));

				GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));
				GLCALL(glEnableVertexAttribArray(2));

				GLCALL(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tan)));
				GLCALL(glEnableVertexAttribArray(3));

			}

		}
		GLCALL(glBindVertexArray(0));
		model.push_back(myMesh);

	}
	
	return m;
}

void Application::Steal(Component *comp)
{
	if (Light* l = dynamic_cast<Light*>(comp)) {
		LIGHTS.push_back(l);
	}

	if (Camera * cam = dynamic_cast<Camera*>(comp)) {
		if (cameras.size() < 1) {
			mainCamera = cam;
			mainCamera->power = 1;
		}
		cameras.push_back(cam);
	}

	if (MeshRenderer* ren = dynamic_cast<MeshRenderer*>(comp)) {
		renderers.push_back(ren);
	}
}

void Application::DrawObjects(std::vector<const MeshRenderer*> meshes, std::function<bool(const ShaderProgram&shader, const Material&MAT, const MeshRenderer&mesh)> PreReqs)
{
	auto meshesToRender = meshes;
	for (const MeshRenderer * const meshRen : meshesToRender) {

		const Mesh * mesh = meshRen->mesh;
		const Material &MAT = mesh->mat;
		const ShaderProgram &shader = *shaders[MAT.illum];

		shader.Use();
		GLCALL(glBindVertexArray(mesh->VAO));

		if (PreReqs(shader, MAT, *meshRen)) {

			const Mat4 &MVP = meshRen->transform->MVP;
			SET_UNIFORM(shader, MVP);
			GLCALL(glDrawElements(GL_TRIANGLES, mesh->nElem, GL_UNSIGNED_INT, 0));
		}

	}
	GLCALL(glBindVertexArray(0));

}

Application::Application()
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
		win_width, win_heigth, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	glContext = SDL_GL_CreateContext(win);
	SDL_GetWindowSize(win, &win_width, &win_heigth);
	SDL_SetWindowResizable(win, (SDL_bool)true);


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

Application::~Application()
{

	for (int ii = 0; ii < shaders.size(); ii++) {
		delete shaders[ii];
	}

	std::for_each(begin(shadersLoaded), end(shadersLoaded), [](std::pair<std::string, Shader*> p) {delete p.second; });
	std::for_each(begin(texturesLoaded), end(texturesLoaded), [](std::pair<std::string, Texture*> p) {delete p.second; });
	std::for_each(begin(models), end(models), [](Model* p) {delete p; });
	std::for_each(begin(modelsDebug), end(modelsDebug), [](Model* p) {delete p; });
	//std::for_each(begin(shaders), end(shaders), [](ShaderProgram* p) {delete p; });
	
	glDeleteTextures(1, &volumeId);
	glDeleteVertexArrays(1, &VAO_PLANE);
	glDeleteBuffers(1, &VBO_PLANE);

	//delete cubeMap;
	
	Traverse(rootNodes,
		[](GameObject* go) {},
		[](GameObject* go) {delete go; },
		[](Component* comp) {delete comp; }
	);


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	//nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
}