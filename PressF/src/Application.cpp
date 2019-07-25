#include "Application.h"



Mesh Application::GLCreate(objl::Mesh &model) {
	Mesh curMesh;

	//nasty way of copying attributes
	*dynamic_cast<objl::Material*>(&curMesh.mat) = model.MeshMaterial;
	curMesh.nVertex = model.Vertices.size();


	GLCALL(glGenVertexArrays(1, &curMesh.VAO));
	GLCALL(glGenBuffers(1, &curMesh.VBO));
	GLCALL(glGenBuffers(1, &curMesh.EBO));

	GLCALL(glBindVertexArray(curMesh.VAO));
	{
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, curMesh.VBO));
		glBufferData(GL_ARRAY_BUFFER, model.Vertices.size() * sizeof(objl::Vertex), &model.Vertices[0], GL_STATIC_DRAW);
		{
			GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Position)));
			GLCALL(glEnableVertexAttribArray(0));

			GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Normal)));
			GLCALL(glEnableVertexAttribArray(1));

			GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, TextureCoordinate)));
			GLCALL(glEnableVertexAttribArray(2));
		}

		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curMesh.EBO));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.Indices.size() * sizeof(unsigned int), &model.Indices[0], GL_STATIC_DRAW);
		{
			GLCALL(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(unsigned int) * 9, (void*)0));
			GLCALL(glEnableVertexAttribArray(4));
		}
	}

	GLCALL(glBindVertexArray(0));




	return curMesh;
}

void Application::Setup(const std::vector<std::string>& objPaths, const std::vector<std::tuple<std::string, std::string>>& shaderPaths)
{
	spdlog::set_pattern("[%M:%S %z] [%^%v%$]");

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

	for each (std::string objPath in objPaths)
	{
		Model model;

		if (!model.LoadFile(objPath)) {
			PF_ERROR("Failed to load model {0}", objPath);
			__debugbreak();
		}

		for each (objl::Mesh mesh in model.LoadedMeshes)
		{
			meshes.push_back(GLCreate(mesh));
		}
	}


	DummySetup();




}

void Application::MainLoop()
{
	while (running) {
		//std::cout << "looping";
		SDL_GetWindowSize(win, &win_width, &win_heigth);
		glViewport(0, 0, win_width, win_heigth);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1, 1, 0, 1);
		LAST = NOW;
		SDL_GetMouseState(&mouse_lastPosX, &mouse_lastPosY);

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
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) {
			running = false;
			break;
		}

		if (e.type == SDL_EventType::SDL_KEYDOWN) {
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_ESCAPE) {
				running = false;
			}
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_R) {
				for (int ii = 0; ii < 15; ii++)
				{
					if (shaders[ii] != nullptr) {
						shaders[ii]->ReCompile();
					}
				}
				PF_INFO("Shaders compiled");
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

		cam->fc.HandleEvent(e);
	}
	//nk_input_end(ctx);
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
	/* process frame */
	//process_frame(ctx);

	/* render */
	//r_clear(mu_color(bg[0], bg[1], bg[2], 255));
	/*mu_Command *cmd = NULL;
	while (mu_next_command(ctx, &cmd)) {
		switch (cmd->type) {
		case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
		case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
		case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
		case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
		}
	}
	r_present();*/
}

void Application::RenderLoop()
{
	//	Camera &cam = sRenderer->GetCamera();
	//	auto view = cam.GetView();
	//	auto proj = cam.GetProjection();
	//	auto lights = sRenderer->lights;
	//	auto renderers = sRenderer->renderers;
	//	auto cubemap = sRenderer->cubemap;
	//
	//	//for each (Light* light in lights)
	//	//{
	//	//	light->ShadowPass();
	//	//}
	//
	for each (const Mesh& mesh in meshes)
	{

		iVec3 lightsPlaced{ 0,0,0 };
		const Material& MAT = mesh.mat;

		shaders[MAT.illum]->Use();
		GLCALL(glBindVertexArray(mesh.VAO));


		GLCALL(glDrawArrays(GL_TRIANGLES, 0, mesh.nVertex));

		GLCALL(glBindVertexArray(0));


		//			const size_t nElem = materialOrderForRender[ii].quantityFaces;
		//			ShaderProgram &shader = *MAT.shader;
		//			iVec3 lightsPlaced{ 0,0,0 };
		//
		//			ActShader = &shader;
		//			ActSpec = &materialOrderForRender[ii];
		//
		//			shader.use();
		//			GLCALL(glBindVertexArray(materialOrderForRender[ii].VAO));
		//
		//			const std::vector<std::string > nameMapping = {
		//				"tex_kA", /*MapType::AMBIENT*/
		//				"tex_kD", /*MapType::DIFFUSE*/
		//				"tex_kS", /*MapType::SPECULAR*/
		//				"tex_bump", /*MapType::BUMP*/
		//				"tex_cubemap", /*MapType::CUBEMAP*/
		//				"", /*MapType::SHINY*/
		//				"", /*MapType::DISPLACEMENT*/
		//				"", /*MapType::DECAL*/
		//				"", /*MapType::REFLECTION*/
		//				"", /*MapType::DISSOLVE*/
		//			};
		//
		//			// ponemos texturas
		//			for each (auto pair in MAT.maps)
		//			{
		//				Texture &tex = *pair.second;
		//				int type = static_cast<int>(tex.type);
		//				glActiveTexture(GL_TEXTURE0 + type);
		//				glBindTexture(GL_TEXTURE_2D, tex.id);
		//				shader.setInt(nameMapping[type], tex.id);
		//			}
		//
		//			// pasamos toda la data de las luces
		//			if (shader.lit) {
		//				for each (Light* light in lights)
		//				{
		//					light->Bind(lightsPlaced, shader);
		//				}
		//			}
		//
		//			// pasamos toda la data del material
		//#define SET_SHADER_PROP(XX) shader.setVec4(#XX, XX)
		//
		//			SET_SHADER_PROP(MAT.kA);
		//			SET_SHADER_PROP(MAT.kD);
		//			SET_SHADER_PROP(MAT.kS);
		//			SET_SHADER_PROP(MAT.kE);
		//
		//			shader.setFloat("MAT.IOR", MAT.refractionIndex);
		//			shader.setFloat("MAT.shiny", MAT.shiny);
		//
		//			shader.setMat4("view", view);
		//			shader.setMat4("proj", proj);
		//
		//			int nVertex = nElem * 3;
		//			for (MeshRenderer* ren : mesh.registered)
		//			{
		//				PF_ASSERT(ren && "Renderer is null");
		//				if (ren->Enabled()) {
		//					ActRenderer = ren;
		//					Transform &transform = ren->transform;
		//					shader.setMat4("model", transform.GetAccumulated());
		//
		//					shader.preReq([&]() {
		//						GLCALL(glDrawArrays(GL_TRIANGLES, 0, nVertex));
		//					});
		//				}
		//			}
		//			GLCALL(glBindVertexArray(0));
		//		}
	}
	//	//cubemap->Render();
	//
	//	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
	//


	DummyLoop();
}

void Application::DummySetup() {


	cubeMap = new CubeMap({
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
	});


	cam = new Camera();
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

	triShader = shaders[1];
}

void Application::DummyLoop() {

	auto view = Transform::GetViewExp(cam->t);
	auto projection = Transform::GetProjection(cam->t, true, win_width / static_cast<float>(win_heigth));

	ShaderProgram &shader = *triShader;

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