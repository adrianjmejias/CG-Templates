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


void Init() {
	InitLog();
	//InitSDL();
	InitGlad();
	InitNuklear();
}

void clear() {
	// Create an event handler

	// Destroy everything to not leak memory.
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
ShaderProgram *basic;

void InitScene() {


	GameObject *go1(new GameObject("Camara"));
	Camera& cam = go1->AddComponent<Camera>();
	objects.push_back(go1);
	go1->transform.Translate(0, 0, -10);



	GameObject *go2(new GameObject("PointLight"));
	Light& light1 = go2->AddComponent<PointLight>();
	objects.push_back(go2);

	GameObject *go3(new GameObject("DirectionalLight"));
	Light& light2 = go3->AddComponent<DirectionalLight>();
	go3->transform.SetParent(&go2->transform);

	GameObject *go4(new GameObject("SpotLight"));
	Light& light3 = go4->AddComponent<SpotLight>();
	go4->transform.SetParent(&go2->transform);

	basic = new ShaderProgram({
		Shader::FromString("#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = projection*view*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0", GL_VERTEX_SHADER),
		Shader::FromString("#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0", GL_FRAGMENT_SHADER)
		},
		[](PARAMS_PREREQ) {




	});


	//Mesh * cajita(new Mesh("assets/models/fullcube/fullCube.obj"));
	//{
	//	meshes.push_back(cajita);
	//}

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
}

int main(void)
{


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
	InitScene();

	//basic->Compile();



	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
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
	unsigned int VBO, VAO, EBO;
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

	while (running)
	{
		LAST = NOW;

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

		/* Input */
		SDL_GetWindowSize(win, &win_width, &win_height);
		glViewport(0, 0, win_width, win_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);

		SDL_Event evt;
		nk_input_begin(ctx);
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT) goto cleanup;

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

		//PF_INFO("UPDATE UI");
		if (nk_begin(ctx, "Hierarchy", nk_rect(500, 50, 230, 250),
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
			NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			for each (auto go in objects)
			{
				go->UI();
		}
	}
		nk_end(ctx);
		Camera &cam = sRenderer->GetCamera();

		basic->use();
		basic->setMat4("projection", cam.GetProjection());
		basic->setMat4("view", cam.GetView());

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
								//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//PF_INFO("UPDATE RENDER");
		sRenderer->Render();

		//#define INCLUDE_OVERVIEW 

				/* -------------- EXAMPLES ---------------- */
#ifdef INCLUDE_CALCULATOR
		calculator(ctx);
#endif
#ifdef INCLUDE_OVERVIEW
		overview(ctx);
#endif
#ifdef INCLUDE_NODE_EDITOR
		node_editor(ctx);
#endif


		/* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
		* with blending, scissor, face culling, depth test and viewport and
		* defaults everything back into a default state.
		* Make sure to either a.) save and restore or b.) reset your own state after
		* rendering the UI. */
		nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
		SDL_GL_SwapWindow(win);

		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
}




cleanup:
	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
