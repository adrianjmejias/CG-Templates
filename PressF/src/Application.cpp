#include "Application.h"

extern double deltaTime = 0;
extern unsigned int GLOBAL_ID = 1;
extern Camera* mainCamera = nullptr;


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
	SetupDummy();


	glEnable(GL_DEPTH_TEST);

	// Sets up shit. maybe not the best way but it's what came to me

	Traverse(rootNodes,
		[](GameObject* go) {
		PF_ASSERT(go && "GameObject is null");
	},
		[&](GameObject* go) {
	},
		[&](Component* comp) {
		Steal(comp);
	}
	);

}

void Application::SetupScene()
{
	{
		GameObject *go = new GameObject();
		go->AddComponent < FlyingController >();
		go->AddComponent < Camera >();
		//cam = &go->AddComponent<CameraGL>();

		go->transform.SetPosition(0, 0, -10);
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


	for (size_t ii = 0; ii < 3; ii++)
		for (Model &model : models) {
			GameObject *papa = new GameObject();
			for (Mesh &mesh : model) {
				GameObject *go = new GameObject();
				MeshRenderer *ren = &go->AddComponent<MeshRenderer>(&mesh);
				mesh.push_back(ren);

				go->transform.SetPosition(0, 0, 0);
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
			PF_DEBUG("", mesh.EBO);
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
	const std::string baseShaderFolder = "assets/shaders/defaults/";
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


void Application::SetupDummy() {
	cubeMap = new CubeMap({
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
		});


	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	cubeMap->shader = shaders[6];

	shaderTri = shaders[1];
}

void Application::DummyLoop() {

	const Camera& cam = *cameras[actCam];

	auto view = Transform::GetView(cam.transform);
	auto projection = Transform::GetProjection(cam.transform, true, win_width / static_cast<float>(win_heigth));

	ShaderProgram &shader = *shaderTri;

	shader.Use();
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	SET_UNIFORM(shader, view);
	SET_UNIFORM(shader, projection);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



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


	// glBindVertexArray(0); // no need to unbind it every time
}

void Application::LoopMain()
{
	while (running) {
		//std::cout << "looping";
		glViewport(0, 0, win_width, win_heigth);


		HandleEvents();

		LoopUI();

		LoopUpdate();



		// no need to order the cameras over power. we select the one we want
		//std::sort(begin(orderedCameras), end(orderedCameras), [](Camera* a, Camera* b)
		//{
		//	return a->power > b->power;
		//}); // keep cameras ordered

		const Camera& cam = *cameras[actCam];
		const Vec3& camPos = cam.transform.GetPosition();

		std::sort(begin(renderers), end(renderers), [&](const MeshRenderer* a, const MeshRenderer* b) {
			float distCamToA = glm::length(a->transform.GetPosition() - camPos);
			float distCamToB = glm::length(b->transform.GetPosition() - camPos);

			return distCamToA > distCamToB;
		});

		LoopRender();



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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);
	const Camera &cam = *cameras[actCam];

	Mat4 projection = Transform::GetProjection(cam.transform, true, win_width / static_cast<float>(win_width));
	const Mat4 view = Transform::GetView(cam.transform);

	for (const Model& model : models) {
		for (const Mesh &mesh : model) {
			iVec3 lightsPlaced{ 0,0,0 };
			const Material &MAT = mesh.mat;
			const ShaderProgram &shader = *shaders[MAT.illum];
			shader.Use();
			GLCALL(glBindVertexArray(mesh.VAO));

			if (shader.usesMaterial) {
				SET_UNIFORM(shader, MAT.kA);
				SET_UNIFORM(shader, MAT.kD);
				SET_UNIFORM(shader, MAT.kS);
				SET_UNIFORM(shader, MAT.Ns);
				SET_UNIFORM(shader, MAT.Ni);
			}

			if (shader.usesTextures) {
				if (MAT.smap_Ka)
				{
					shader.SetUniform("map_kD", static_cast<int>(MAT.smap_Kd->id));
				}

				if (MAT.smap_bump)
				{
					shader.SetUniform("map_bump", static_cast<int>(MAT.smap_bump->id));
				}
			}

			if (shader.lit) {
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

			for (auto obj : mesh) {
				PF_ASSERT(obj && "Renderer is null");
				const Mat4 &model = obj->transform.GetAccumulated();
				SET_UNIFORM(shader, model);
				GLCALL(glDrawElements(GL_TRIANGLES, mesh.nElem, GL_UNSIGNED_INT, 0));
			}
		}
		GLCALL(glBindVertexArray(0));
	}


	DummyLoop();




	//	//			const size_t nElem = materialOrderForRender[ii].quantityFaces;
	//	//			ShaderProgram &shader = *MAT.shader;
	//	//			iVec3 lightsPlaced{ 0,0,0 };
	//	//
	//	//			ActShader = &shader;
	//	//			ActSpec = &materialOrderForRender[ii];
	//	//
	//	//			shader.use();
	//	//			GLCALL(glBindVertexArray(materialOrderForRender[ii].VAO));
	//	//
	//	//			const std::vector<std::string > nameMapping = {
	//	//				"tex_kA", /*MapType::AMBIENT*/
	//	//				"tex_kD", /*MapType::DIFFUSE*/
	//	//				"tex_kS", /*MapType::SPECULAR*/
	//	//				"tex_bump", /*MapType::BUMP*/
	//	//				"tex_cubemap", /*MapType::CUBEMAP*/
	//	//				"", /*MapType::SHINY*/
	//	//				"", /*MapType::DISPLACEMENT*/
	//	//				"", /*MapType::DECAL*/
	//	//				"", /*MapType::REFLECTION*/
	//	//				"", /*MapType::DISSOLVE*/
	//	//			};
	//	//
	//	//			// ponemos texturas
	//	//			for each (auto pair in MAT.maps)
	//	//			{
	//	//				Texture &tex = *pair.second;
	//	//				int type = static_cast<int>(tex.type);
	//	//				glActiveTexture(GL_TEXTURE0 + type);
	//	//				glBindTexture(GL_TEXTURE_2D, tex.id);
	//	//				shader.setInt(nameMapping[type], tex.id);
	//	//			}
	//	//
	//	//			// pasamos toda la data de las luces
	//	//			if (shader.lit) {
	//	//				for each (Light* light in lights)
	//	//				{
	//	//					light->Bind(lightsPlaced, shader);
	//	//				}
	//	//			}
	//	//
	//	//			// pasamos toda la data del material
	//	//#define SET_SHADER_PROP(XX) shader.setVec4(#XX, XX)
	//	//
	//	//			SET_SHADER_PROP(MAT.kA);
	//	//			SET_SHADER_PROP(MAT.kD);
	//	//			SET_SHADER_PROP(MAT.kS);
	//	//			SET_SHADER_PROP(MAT.kE);
	//	//
	//	//			shader.setFloat("MAT.IOR", MAT.refractionIndex);
	//	//			shader.setFloat("MAT.shiny", MAT.shiny);
	//	//
	//	//			shader.setMat4("view", view);
	//	//			shader.setMat4("proj", proj);
	//	//
	//	//			int nVertex = nElem * 3;
	//	//			for (MeshRenderer* ren : mesh.registered)
	//	//			{
	//	//				PF_ASSERT(ren && "Renderer is null");
	//	//				if (ren->Enabled()) {
	//	//					ActRenderer = ren;
	//	//					Transform &transform = ren->transform;
	//	//					shader.setMat4("model", transform.GetAccumulated());
	//	//
	//	//					shader.preReq([&]() {
	//	//						GLCALL(glDrawArrays(GL_TRIANGLES, 0, nVertex));
	//	//					});
	//	//				}
	//	//			}
	//	//			GLCALL(glBindVertexArray(0));
	//	//		}
	//}
	//	//cubemap->Render();
	//
	//	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
	//
}

void Application::DrawFrame(Transform t)
{
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
				GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)off));
				GLCALL(glEnableVertexAttribArray(0));

				off += 3 * sizeof(Vec3);
				GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)off));
				GLCALL(glEnableVertexAttribArray(1));

				off += 3 * sizeof(Vec3);
				GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)off));
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
	// Load GL extensions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize the OpenGL context." << std::endl;
		exit(1);
	}

	// Loaded OpenGL successfully.
	std::cout << "OpenGL version loaded: " << GLVersion.major << "."
		<< GLVersion.minor << std::endl;
	/* OpenGL setup */
	glViewport(0, 0, win_width, win_heigth);

	//ctx = malloc(sizeof(mu_Context));
	//mu_init(ctx);

	//ctx->text_width = win_width;
	//ctx->text_height = win_heigth;

	//ctx = nk_sdl_init(win);
	///* Load Fonts: if none of these are loaded a default font will be used  */
	///* Load Cursor: if you uncomment cursor loading please hide the cursor */
	//{
	//	struct nk_font_atlas *atlas;
	//	nk_sdl_font_stash_begin(&atlas);
	//	/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
	//	nk_sdl_font_stash_end();
	//	nk_style_load_all_cursors(ctx, atlas->cursors);
	//	//nk_style_set_font(ctx, &roboto->handle);
	//}
}

Application::~Application()
{
	//nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
}