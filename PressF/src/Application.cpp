#include "Application.h"

extern double deltaTime = 0;

void Application::GLCreate(objl::Loader &fullModel) {
	Model model;
	glGenVertexArrays(1, &model.VAO);
	glGenBuffers(1, &model.EBO);
	glGenBuffers(1, &model.VBO);

	//Mesh curMesh;

	////nasty way of copying attributes
	//*dynamic_cast<objl::Material*>(&curMesh.mat) = model.MeshMaterial;

	////for (size_t ii = 0; ii < model.Indices.size(); ii += 3)
	////{
	////	curMesh.indices.push_back({ model.Indices[ii],model.Indices[ii + 1], model.Indices[ii + 2] });
	////}

	////for (size_t ii = 0; ii < model.Vertices.size(); ii++)
	////{
	////	Vertex v;
	////	objl::Vertex b = model.Vertices[ii];

	////	Assign(v.pos, b.Position);
	////	Assign(v.normal, b.Normal);
	////	Assign(v.uv, b.TextureCoordinate);

	////	curMesh.vertex.push_back(v);
	////}

	//GLCALL(glGenVertexArrays(1, &curMesh.VAO));
	//GLCALL(glGenBuffers(1, &curMesh.VBO));
	//GLCALL(glGenBuffers(1, &curMesh.EBO));

	//GLCALL(glBindVertexArray(curMesh.VAO));
	//{
	//	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, curMesh.VBO));
	//	glBufferData(GL_ARRAY_BUFFER, curMesh.vertex.size() * sizeof(Vertex), &curMesh.vertex[0], GL_STATIC_DRAW);

	//	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curMesh.EBO));
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, curMesh.indices.size() * sizeof(iVec3), &curMesh.indices[0], GL_STATIC_DRAW);

	//	{
	//		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)));
	//		GLCALL(glEnableVertexAttribArray(0));

	//		GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
	//		GLCALL(glEnableVertexAttribArray(1));

	//		GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitan)));
	//		GLCALL(glEnableVertexAttribArray(2));

	//		GLCALL(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tan)));
	//		GLCALL(glEnableVertexAttribArray(3));

	//		GLCALL(glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));
	//		GLCALL(glEnableVertexAttribArray(4));
	//	}
	//}
	//GLCALL(glBindVertexArray(0));
	//return curMesh;
}

void Application::Setup(const std::vector<std::string>& objPaths, const std::vector<std::tuple<std::string, std::string>>& shaderPaths)
{
	spdlog::set_pattern("[%M:%S %z] [%^%v%$]");
	SetupShaders(shaderPaths);
	SetupModels(objPaths);
	SetupScene();

	// Sets up shit. maybe not the best way but it's what came to me
	for (auto go : objects) {
		PF_ASSERT(go && "NULL GAMEOBJECT MOSCAAAA");

		for (auto comp : go->components) {
			PF_ASSERT(comp && "NULL COMPONENT MOSCAAAA");

			Steal(comp);
		}
	}
}

void Application::SetupScene()
{
	GameObject *go = new GameObject();
	go->AddComponent < Camera >();
	objects.push_back(go);

	for(Model* model : models)
	for (Mesh &mesh : *model) {
		GameObject *go = new GameObject();
		MeshRenderer *ren = &go->AddComponent<MeshRenderer>(&mesh);
		mesh.push_back(ren);
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

void Application::MainLoop()
{
	while (running) {
		//std::cout << "looping";
		glViewport(0, 0, win_width, win_heigth);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);

		HandleEvents();

		UILoop();

		UpdateLoop();

		//std::sort(begin(callOrder), end(callorder), [])

		RenderLoop();

		SDL_GL_SwapWindow(win);

		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
		SDL_GetMouseState(&mouse_deltaX, &mouse_deltaY);
		mouse_deltaX -= mouse_lastPosX;
		mouse_deltaY -= mouse_lastPosY;
		mouse_deltaY *= -1;
	}
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
			switch (e.key.keysym.scancode)
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

		for (auto go : objects)
		{
			for (auto comp : go->components) {
				comp->HandleEvent(e);
			}
		}
	}
}

void Application::UILoop()
{
	////overview(ctx);
	//if (nk_begin(ctx, "Hierarchy", nk_rect(0, 50, 250, 500),
	//	NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
	//	NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	//{
	//	//for each (auto go in objects)
	//	//{
	//	//	go->UI();
	//	//}
	//}
	//nk_end(ctx);
}

void Application::UpdateLoop()
{
	for (GameObject* go : objects)
	{
		PF_ASSERT(go && "game object is null");
		for (Component* comp : go->components)
		{
			PF_ASSERT(comp && "component is null");
			comp->Update();
		}
	}
}

void Application::RenderLoop()
{
	Camera &cam = *cameras.top();
	Mat4 projection = cam.GetProjection(ProjectionType::CAM_SETUP, win_width, win_heigth);
	Mat4 view = cam.GetView();

	for (const Model* model : models) {
		PF_ASSERT(model && "model can't be null");
		glBindVertexArray(model->VAO);


		for (const Mesh &mesh : *model) {
			iVec3 lightsPlaced{ 0,0,0 };
			const Material &MAT = mesh.mat;
			const ShaderProgram &shader = *shaders[MAT.illum];
			shader.Use();

			if (shader.usesMaterial) {
				SET_UNIFORM(shader, MAT.kA);
				SET_UNIFORM(shader, MAT.kD);
				SET_UNIFORM(shader, MAT.kS);
				SET_UNIFORM(shader, MAT.Ns);
				SET_UNIFORM(shader, MAT.Ni);
			}

			if (shader.usesTextures) {

			}

			if (shader.lit) {
				for (Light* light : LIGHTS) {
					PF_ASSERT(light && "Light is null");
					light->Bind(lightsPlaced, shader);
				}
			}

			if (shader.viewDependant) {
				shader.SetUniform("CAM.position", cam.transform.GetPosition());
			}

			if (shader.MVP) {
				SET_UNIFORM(shader, projection);
				SET_UNIFORM(shader, view);
			}

			for (auto obj : mesh) {
				PF_ASSERT(obj && "Renderer is null");
				Mat4 &model = obj->transform.GetAccumulated();
				SET_UNIFORM(shader, model);
				glDrawElements(GL_TRIANGLES, mesh.nElem, GL_UNSIGNED_INT, (void*) mesh.offset);
			}
		}
		glBindVertexArray(0);
	}

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

void Application::Steal(Component *comp)
{
	if (Light* l = dynamic_cast<Light*>(comp)) {
		LIGHTS.push_back(l);
	}

	if (Camera * cam = dynamic_cast<Camera*>(comp)) {
		cameras.push(cam);
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