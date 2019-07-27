#include "Application.h"

extern double deltaTime = 0;
extern unsigned int GLOBAL_ID = 1;
extern Camera* mainCamera = nullptr;


template <typename LambdaRender>
void RenderNormal(LambdaRender F) {

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


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Sets up shit. maybe not the best way but it's what came to me

	Traverse(rootNodes,
		[](GameObject* go) {
		PF_ASSERT(go && "GameObject is null");
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

	cubeMap = new CubeMap({
	"assets/skybox/right.jpg",
	"assets/skybox/left.jpg",
	"assets/skybox/top.jpg",
	"assets/skybox/bottom.jpg",
	"assets/skybox/front.jpg",
	"assets/skybox/back.jpg"
		});

	cubeMap->shader = shaders[6];


	{
		GameObject *go = new GameObject();
		go->AddComponent < FlyingController >();
		go->AddComponent < Camera >();
		//cam = &go->AddComponent<CameraGL>();

		go->transform.SetPosition(0, 0, -20);
		rootNodes.push_back(go);

	}

	{
		GameObject *go = new GameObject();
		go->AddComponent < Camera >();
		go->transform.SetPosition(0, 0, 10);
		rootNodes.push_back(go);
	}

	{
		GameObject *go = new GameObject();
		go->AddComponent < Light >(LightType::DIRECTIONAL);
		go->transform.SetPosition(0, 10, 0);
		rootNodes.push_back(go);
	}

	{
		GameObject *go = new GameObject();
		go->AddComponent < Light >(LightType::POINT);
		go->transform.SetPosition(0, -10, 0);
		rootNodes.push_back(go);
	}


	for (Model &model : models) {
		GameObject *papa = new GameObject();
		for (Mesh &mesh : model) {
			GameObject *go = new GameObject();
			MeshRenderer *ren = &go->AddComponent<MeshRenderer>(&mesh);
			mesh.push_back(ren);

			go->transform.SetPosition(0, 0, 0);
			go->transform.SetScale(3, 3, 3);
			go->transform.Rotate(0, 90, 0);
			go->transform.SetParent(&papa->transform);
			//go->transform.SetPosition(glm::ballRand(3.f) + 2.f);

		}
		rootNodes.push_back(papa);
	}
}

void Application::SetupModels(const std::vector<std::string>& objPaths)
{
	for (const std::string& objPath : objPaths)
	{
		objl::Loader model;

		if (!model.LoadFile(objPath)) {
			PF_ERROR("Failed to load model {0}", objPath);
			__debugbreak();
		}
		GLCreate(model);


		//cargar texturas
#define ADD_MAP(NAME)\
		if (!mat.NAME.empty() && (texturesLoaded.find(mat.NAME) == texturesLoaded.end())) {\
			texturesLoaded[mat.NAME] = Texture::TextureFromFile(mat.NAME.c_str(), basePath);\
		}\


		const std::string basePath = objPath.substr(0, objPath.find_last_of("/"));
		for (objl::Material& mat : model.LoadedMaterials) {
			ADD_MAP(map_bump);
			ADD_MAP(map_Ka);
			ADD_MAP(map_Kd);
			ADD_MAP(map_Ks);
			ADD_MAP(map_d);
		}
	}

#define ADD_MAP_TO_MATERIAL(NAME)\
	if (!mesh.mat.NAME.empty()) {\
		mesh.mat.s##NAME = texturesLoaded.at(mesh.mat.NAME);\
	}\

	for (Model& myModel : models) {

		for (Mesh& mesh : myModel) {
			ADD_MAP_TO_MATERIAL(map_bump);
			ADD_MAP_TO_MATERIAL(map_Ka);
			ADD_MAP_TO_MATERIAL(map_Kd);
			ADD_MAP_TO_MATERIAL(map_Ks);
			ADD_MAP_TO_MATERIAL(map_d);
		}
	}
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

		shaders[ii] = new ShaderProgram({ vert, frag });
	}
}


void Application::LoopMain()
{
	while (running) {
		//std::cout << "looping";
		glViewport(0, 0, win_width, win_heigth);


		HandleEvents();


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

	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	{
		ImGui::Begin("Another Window", &show_another_window);
		// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)


		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	SDL_GL_MakeCurrent(win, glContext);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	Traverse(rootNodes,
		[](GameObject* go) {},
		[](GameObject* go) {},
		[](Component* comp) {comp->Update(); }
	);
}

void Application::LoopUpdate()
{
	Traverse(rootNodes,
		[](GameObject* go) {},
		[](GameObject* go) { go->transform.TryGetClean(); },
		[](Component* comp) {comp->Update(); }
	);
}

void Application::LoopRender()
{


	const Camera& cam = *cameras[actCam];
	const Vec3& camPos = cam.transform.GetPosition();

	std::vector<const Mesh *> NonTransparentMeshes;
	std::vector<const MeshRenderer *> transparentMeshes;

	for (const Model& model : models) {

		for (const Mesh& mesh : model) {
			if (mesh.mat.illum == 1) // 1 transparent shader index
			{
				for (const MeshRenderer * mr : mesh) {
					transparentMeshes.push_back(mr);
				}
			}
			else
			{
				NonTransparentMeshes.push_back(&mesh);
			}
		}
	}


	std::sort(begin(transparentMeshes), end(transparentMeshes), [&](const MeshRenderer* a, const MeshRenderer* b) {
		float distCamToA = glm::length(a->transform.GetPosition() - camPos);
		float distCamToB = glm::length(b->transform.GetPosition() - camPos);

		return distCamToA < distCamToB;
	});




	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);









	{

		const Mat4& projection = Transform::GetProjection(cam.transform, true, win_width / static_cast<float>(win_width));
		const Mat4& view = Transform::GetView(cam.transform);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);

		DrawObjects(view, projection, NonTransparentMeshes, [&](const ShaderProgram& shader, const Material& MAT)->bool {

			if (shader.usesMaterial) {
				SET_UNIFORM(shader, MAT.kA);
				SET_UNIFORM(shader, MAT.kD);
				SET_UNIFORM(shader, MAT.kS);
				SET_UNIFORM(shader, MAT.Ns);
				SET_UNIFORM(shader, MAT.Ni);
			}

			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureID);

			if (shader.usesTextures)
			{
				if (MAT.smap_Kd)
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, MAT.smap_Kd->id);
					shader.SetUniform("tex_kD", 1);
				}

				if (MAT.smap_Ks)
				{
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, MAT.smap_Ks->id);
					shader.SetUniform("tex_kS", 2);
				}

				if (MAT.smap_bump)
				{
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_2D, MAT.smap_bump->id);
					shader.SetUniform("tex_bump", 3);
				}

				if (MAT.smap_Ka)
				{
					glActiveTexture(GL_TEXTURE4);
					glBindTexture(GL_TEXTURE_2D, MAT.smap_Ka->id);
					shader.SetUniform("tex_kA", 4);
				}
			}
			glBindTexture(GL_TEXTURE_2D, 0);


			if (shader.lit)
			{
				iVec3 lightsPlaced{ 0,0,0 };
				for (Light* light : LIGHTS) {
					PF_ASSERT(light && "Light is null");
					light->Bind(lightsPlaced, shader);
				}
			}

			if (shader.viewDependant) {
				GLCALL(shader.SetUniform("viewPos", cam.transform.GetPosition()));
			}

			if (shader.MVP) {
				SET_UNIFORM(shader, projection);
				SET_UNIFORM(shader, view);
			}

			return true;
		});

		DrawObjects(view, projection, transparentMeshes, [&](const ShaderProgram& shader, const Material& MAT, const MeshRenderer& meshRen)-> bool {

			if (MAT.smap_Kd)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MAT.smap_Kd->id);
				shader.SetUniform("tex_kD", 1);
			}
			else
			{
				PF_ASSERT("ALL TRANSPARENT MATERIALS MUST HAVE A TEXTURE");
			}

			glBindTexture(GL_TEXTURE_2D, 0);


			return true;
		});



		glDepthFunc(GL_LEQUAL);
		{
			cubeMap->shader->Use();
			ShaderProgram &shader = *cubeMap->shader;
			glm::mat4 viewSkybox = glm::mat4(glm::mat3(view));

			shader.SetUniform("view", viewSkybox);
			SET_UNIFORM(shader, projection);


			glBindVertexArray(cubeMap->skyboxVAO);
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureID);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glBindVertexArray(0);


		}
		glDepthFunc(GL_LESS);
	}

}

inline void Application::DrawObjects(const Mat4 & view, const Mat4 & projection, std::vector<const Mesh*> meshes, std::function<bool(const ShaderProgram&shader, const Material&MAT)> PreReqs)
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
			const Mat4 &model = obj->transform.GetAccumulated();
			SET_UNIFORM(shader, model);
			GLCALL(glDrawElements(GL_TRIANGLES, mesh->nElem, GL_UNSIGNED_INT, 0));
		}
	}
	GLCALL(glBindVertexArray(0));

}

void Application::GLCreate(objl::Loader &fullModel) {
	Model model;

	//data transform
	//std::vector<Vertex> vertex;
	//vertex.reserve(fullModel.LoadedVertices.size());
	//for (const objl::Vertex& oVertex : fullModel.LoadedVertices) {
	//	vertex.push_back(Vertex(oVertex));
	//}

	GLsizei totalIndices = 0;
	model.reserve(fullModel.LoadedMeshes.size());

	for (int ii = 0; ii < fullModel.LoadedMeshes.size(); ii++) {
		objl::Mesh& objlMesh = fullModel.LoadedMeshes[ii];
		Mesh myMesh;

		std::vector<Vertex> vertex;
		vertex.reserve(fullModel.LoadedVertices.size());

		for (const objl::Vertex& oVertex : objlMesh.Vertices) {
			vertex.push_back(Vertex(oVertex));
		}

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
			GLCALL(glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), vertex.data(), GL_STATIC_DRAW));

			GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.EBO));
			GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, objlMesh.Indices.size() * sizeof(unsigned int), objlMesh.Indices.data(), GL_STATIC_DRAW));

			{
				size_t off = 0;
				GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)));
				GLCALL(glEnableVertexAttribArray(0));

				off += 3 * sizeof(Vec3);
				GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
				GLCALL(glEnableVertexAttribArray(1));

				off += 3 * sizeof(Vec3);
				GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));
				GLCALL(glEnableVertexAttribArray(2));

				//GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitan)));
				//GLCALL(glEnableVertexAttribArray(2));

				//GLCALL(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tan)));
				//GLCALL(glEnableVertexAttribArray(3));

			}

		}
		GLCALL(glBindVertexArray(0));
		model.push_back(myMesh);

	}
	models.push_back(model);
}

void Application::HandleEvents()
{
	SDL_GetWindowSize(win, &win_width, &win_heigth);
	LAST = NOW;
	SDL_GetMouseState(&mouse_lastPosX, &mouse_lastPosY);
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
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
				for (size_t ii = 0; ii < 15; ii++)
				{
					auto shader = shaders[ii];
					if (shader) {
						shader->ReCompile();
					}
				}
				break;
			case SDL_Scancode::SDL_SCANCODE_C:
				if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(captureMouse)) == -1) {
					__debugbreak();
				}
				captureMouse = !captureMouse;
				break;

			case SDL_Scancode::SDL_SCANCODE_1:
			case SDL_Scancode::SDL_SCANCODE_2:
			case SDL_Scancode::SDL_SCANCODE_3:
			case SDL_Scancode::SDL_SCANCODE_4:
			case SDL_Scancode::SDL_SCANCODE_5:
			{
				int newCam = static_cast<int>(keyPressed) - static_cast<int>(SDL_Scancode::SDL_SCANCODE_1);
				if (actCam != newCam) {
					PF_INFO("Swapping Camera to {0}", newCam);
					if (newCam < cameras.size()) {
						std::swap(cameras[actCam]->power, cameras[newCam]->power); // explota si no está y no hay side effects
						actCam = newCam;
						mainCamera = cameras[actCam];
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
	}
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

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(win, glContext);
	ImGui_ImplOpenGL3_Init();

}

Application::~Application()
{
	for (int ii = 0; ii < 30; ii++) {
		if (shaders[ii]) {
			delete shaders[ii];
		}
	}

	std::for_each(begin(shadersLoaded), end(shadersLoaded), [](std::pair<std::string, Shader*> p) {delete p.second; });
	std::for_each(begin(texturesLoaded), end(texturesLoaded), [](std::pair<std::string, Texture*> p) {delete p.second; });

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