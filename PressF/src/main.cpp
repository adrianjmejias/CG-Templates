/* hello.c */
#include "main.h"
#include <stdio.h>
#include <iostream>
#include <memory>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_GLContext gl_context;

inline void InitLog() {
	spdlog::set_pattern("[%H:%M:%S %z] [%^%L: %v %$] [t %t]");
}

inline void InitGlad() {
	// Load GL extensions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize the OpenGL context." << std::endl;
		exit(1);
	}

	// Loaded OpenGL successfully.
	std::cout << "OpenGL version lo3saded: " << GLVersion.major << "."
		<< GLVersion.minor << std::endl;
}

inline void InitSDL() {
	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		PF_ERROR("SDL2 video subsystem couldn't be initialized. Error: {0}", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("Glad Sample",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN |
		SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		std::cerr << "SDL2 Renderer couldn't be created. Error: "
			<< SDL_GetError()
			<< std::endl;
		exit(1);
	}

	// Create a OpenGL context on SDL2
	gl_context = SDL_GL_CreateContext(window);
}

inline void InitNuklear()
{
}

void clear() {
	// Create an event handler

	// Destroy everything to not leak memory.
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Init() {
	InitLog();
	//InitSDL();
	InitGlad();
	InitNuklear();
}
ShaderProgram *basic;

void InitShaders() {
	ShaderProgram* shader;

#define INIT_SHADER(TT, mvp, Lit, Shadows, ViewDependant, func)										\
	{																								\
	shader = new ShaderProgram({																	\
					Shader::FromPath("assets/shaders/defaults/"#TT".vert", GL_VERTEX_SHADER),		\
					Shader::FromPath("assets/shaders/defaults/"#TT".frag", GL_FRAGMENT_SHADER),		\
		}, func);																					\
	shaders[static_cast<int>(IllumModel::TT)] = shader;												\
	shader->MVP = mvp;																				\
	shader->lit = Lit;																				\
	shader->viewDependant = ViewDependant;															\
	shader->shadows = Shadows;																		\
	}																						\

	INIT_SHADER(COOK, true, true, false, true,
		HEADER_LAMBDA
		{
		}
	);

	INIT_SHADER(CUBEMAP, true, false, false, true,
		HEADER_LAMBDA
		{
		}
	);
}

void InitScene() {
	GameObject *go1(new GameObject("Camara"));
	Camera& cam = go1->AddComponent<Camera>();
	objects.push_back(go1);
	go1->transform.Translate(0, 0, -10);
	cam.transform.SetRotation(0, 0, 0);

	GameObject *go2(new GameObject("LUZ"));
	Light& light1 = go2->AddComponent<Light>(LightType::DIRECTIONAL);
	objects.push_back(go2);

	Mesh * cajita(new Mesh("assets/models/cuboPower/cuboPower.obj"));
	{
		meshes.push_back(cajita);
	}
	{
		GameObject *go(new GameObject("Modelo"));
		MeshRenderer& m = go->AddComponent<MeshRenderer>(*cajita);
		objects.push_back(go);
	}

	Mesh * plano(new Mesh("assets/models/plane/plane.obj"));
	{
		meshes.push_back(plano);
	}
	{
		GameObject *go(new GameObject("Plano"));
		MeshRenderer& m = go->AddComponent<MeshRenderer>(*plano);
		objects.push_back(go);
	}
	//std::cout<< Transform::WorldFront()<< Transform::RotatePoint(Transform::WorldFront(), {0,90,0}) <<std::endl;

	//GameObject *caja(new GameObject("Cajita"));
	//caja->AddComponent<MeshRenderer>(*cajita);

	sRenderer = new SystemRenderer();

	sRenderer->cubemap = new CubeMap({
		"assets/textures/mp_northlight/right.tga",
		"assets/textures/mp_northlight/left.tga",
		"assets/textures/mp_northlight/top.tga",
		"assets/textures/mp_northlight/bottom.tga",
		"assets/textures/mp_northlight/front.tga",
		"assets/textures/mp_northlight/back.tga",
		});
	sRenderer->Steal(objects);
	win_width = 600;
	win_height = 600;
}

int main(void)
{
#pragma region InitSDL

	/* SDL setup */
	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	win = SDL_CreateWindow("PressF",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	glContext = SDL_GL_CreateContext(win);
	SDL_GetWindowSize(win, &win_width, &win_height);

	InitGlad();
	/* OpenGL setup */
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	ctx = nk_sdl_init(win);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	/* Load Cursor: if you uncomment cursor loading please hide the cursor */
	{
		struct nk_font_atlas *atlas;
		nk_sdl_font_stash_begin(&atlas);
		/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
		nk_sdl_font_stash_end();
		nk_style_load_all_cursors(ctx, atlas->cursors);
		//nk_style_set_font(ctx, &roboto->handle);
	}

	/* style.c */
#ifdef INCLUDE_STYLE
	//set_style(ctx, THEME_WHITE);
	//set_style(ctx, THEME_RED);
	//set_style(ctx, THEME_BLUE);
	//set_style(ctx, THEME_DARK);
#endif

	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
#pragma endregion

	InitShaders();
	InitScene();

#pragma region Setupquad
	//basic->Compile();

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
	//float vertices[] = {
	//	0.5f,  0.5f, 0.0f,  // top right
	//	0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left
	//};
	//unsigned int indices[] = {  // note that we start from 0!
	//	0, 1, 3,  // first Triangle
	//	1, 2, 3   // second Triangle
	//};
	//unsigned int VBO, VAO, EBO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	//// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//glBindVertexArray(VAO);

	//auto m = meshes.front();
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, STL_BYTE_SIZE(m->pos, Vec3), m->pos.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m->iPos), m->iPos.data(), GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glEnableVertexAttribArray(0);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);
#pragma endregion

	bool capture = false;

	if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(capture)) == -1) {
		__debugbreak();
	}
	//if (SDL_CaptureMouse(static_cast<SDL_bool>(true)) == -1) {
	//	__debugbreak();
	//}
	while (running)
	{
#pragma region GoodShit

		SDL_GetWindowSize(win, &win_width, &win_height);
		glViewport(0, 0, win_width, win_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);
		SDL_Event evt;
		LAST = NOW;
		SDL_GetMouseState(&mouse_lastPosX, &mouse_lastPosY);

		std::queue<GameObject*> bfs; //bfs
		std::list<GameObject* > callOrder;

		for (int ii = objects.size() - 1; ii >= 0; ii--)
		{
			PF_ASSERT(objects[ii] != nullptr); // asumo que no es null
			bfs.push(objects[ii]);
		}

		while (!bfs.empty())
		{
			GameObject *ent = bfs.front();
			bfs.pop();

			callOrder.push_back(ent);

			for (Transform *child : ent->transform.children)
			{
				bfs.push(&child->gameobject);
			}
		}

		//PF_INFO("deltaTime {0}", deltaTime);

		nk_input_begin(ctx);
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT) goto cleanup;

			if (evt.type == SDL_EventType::SDL_KEYDOWN) {
				if (evt.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_C) {
					PF_INFO("capture {0}", capture);
					if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(capture)) == -1) {
						__debugbreak();
					}
					capture = !capture;
				}
			}

			for each (auto go in callOrder)
			{
				go->HandleEvent(evt);
			}

			nk_sdl_handle_event(&evt);
		}
		nk_input_end(ctx);
		//PF_INFO("UPDATE GO");

		for each (auto go in callOrder)
		{
			go->Update();
		}

#ifdef INCLUDE_OVERVIEW
		overview(ctx);
#endif

		//PF_INFO("UPDATE UI");
		if (nk_begin(ctx, "Hierarchy", nk_rect(0, 50, 250, 500),
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
			NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			for each (auto go in objects)
			{
				go->UI();
			}
		}
		nk_end(ctx);
#pragma endregion

		Camera &cam = sRenderer->GetCamera();
		auto view = cam.GetView();
		auto proj = cam.GetProjection();
		auto lights = sRenderer->lights;
		auto renderers = sRenderer->renderers;
		auto cubemap = sRenderer->cubemap;

		//for each (Light* light in lights)
		//{
		//	light->ShadowPass();
		//}

		for each (MeshRenderer* ren in renderers)
		{
			PF_ASSERT(ren && "Renderer is null");
			auto &materialOrderForRender = ren->mesh.materialOrderForRender;

			for (size_t ii = 0; ii < materialOrderForRender.size(); ii++)
			{
				const Material &mat = materialOrderForRender[ii].mat;
				const size_t nElem = materialOrderForRender[ii].quantityFaces;
				const ShaderProgram &shader = *mat.shader;
				iVec3 lightsPlaced{ 0,0,0 };

				shader.use();
				GLCALL(glBindVertexArray(materialOrderForRender[ii].VAO));

				const std::vector<std::string > nameMapping = {
					"tex_kA", /*MapType::AMBIENT*/
					"tex_kD", /*MapType::DIFFUSE*/
					"tex_kS", /*MapType::SPECULAR*/
					"tex_bump", /*MapType::BUMP*/
					"tex_cubemap", /*MapType::CUBEMAP*/
					"", /*MapType::SHINY*/
					"", /*MapType::DISPLACEMENT*/
					"", /*MapType::DECAL*/
					"", /*MapType::REFLECTION*/
					"", /*MapType::DISSOLVE*/
				};

				// ponemos texturas
				for each (auto pair in mat.maps)
				{
					Texture &tex = *pair.second;
					int type = static_cast<int>(tex.type);
					glActiveTexture(GL_TEXTURE0 + type);
					glBindTexture(GL_TEXTURE_2D, tex.id);
					shader.setInt(nameMapping[type], tex.id);
				}

				// pasamos toda la data de las luces
				if (shader.lit) {
					for each (Light* light in lights)
					{
						light->Bind(lightsPlaced, shader);
					}
				}

				// pasamos toda la data del material
#define SET_SHADER_PROP(XX) shader.setVec4(#XX, XX)

				SET_SHADER_PROP(mat.kA);
				SET_SHADER_PROP(mat.kD);
				SET_SHADER_PROP(mat.kS);
				SET_SHADER_PROP(mat.kE);

				shader.setFloat("mat.IOR", mat.refractionIndex);
				shader.setFloat("mat.shiny", mat.shiny);

				shader.preReq(mat);

				// pasamos toda la data del objeto
				if (shader.MVP) {
					Transform &transform = ren->transform;
					shader.setMat4("model", transform.GetAccumulated());
					shader.setMat4("view", view);
					shader.setMat4("proj", proj);
				}

				GLCALL(glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(nElem * 3)));
				GLCALL(glBindVertexArray(0));
			}
		}

		//cubemap->Render();

		nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
		SDL_GL_SwapWindow(win);
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
		SDL_GetMouseState(&mouse_deltaX, &mouse_deltaY);
		mouse_deltaX -= mouse_lastPosX;
		mouse_deltaY -= mouse_lastPosY;
		mouse_deltaY *= -1;
	}

cleanup:
	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}