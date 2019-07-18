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

void InitScene() {

	{
		GameObject *go(new GameObject("Camara"));
		Camera& cam = go->AddComponent<Camera>();
		objects.push_back(go);
	}


	{
		GameObject *go(new GameObject("PointLight"));
		Light& light = go->AddComponent<PointLight>();
		objects.push_back(go);
	}

	{
		GameObject *go(new GameObject("DirectionalLight"));
		Light& light = go->AddComponent<DirectionalLight>();
		objects.push_back(go);
	}

	{
		GameObject *go(new GameObject("SpotLight"));
		Light& light = go->AddComponent<SpotLight>();
		objects.push_back(go);
	}

	{
		//Mesh * mesh = loadObj("C:/Users/adrianjmejias/Desktop/CG-Templates/assets/models/parenting/normal.obj");
		Material::ReadMTLLIB("C:/Users/adrianjmejias/Desktop/CG-Templates/assets/models/parenting/normal.mtl");

	}


	sRenderer = new SystemRenderer();

	sRenderer->Steal(objects);
}



/* ===============================================================
*
*                          DEMO
*
* ===============================================================*/
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
	win = SDL_CreateWindow("Demo",
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
		//struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../assets/fonts/Roboto-Regular.ttf", 16, 0);
		/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
		/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
		/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
		/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
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




	



	while (running)
	{
		/* Input */
		SDL_Event evt;
		nk_input_begin(ctx);
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) goto cleanup;
			nk_sdl_handle_event(&evt);
		} nk_input_end(ctx);

		PF_INFO("UPDATE GO");

		for each (auto go in objects)
		{
			go->Update();
		}


		PF_INFO("UPDATE UI");
		if (nk_begin(ctx, "Demo", nk_rect(500, 50, 230, 250),
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
			NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			for each (auto go in objects)
			{
				go->UI();
			}
		}
		nk_end(ctx);

		PF_INFO("UPDATE RENDER");
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
		
		SDL_GetWindowSize(win, &win_width, &win_height);
		glViewport(0, 0, win_width, win_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);
		/* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
		* with blending, scissor, face culling, depth test and viewport and
		* defaults everything back into a default state.
		* Make sure to either a.) save and restore or b.) reset your own state after
		* rendering the UI. */
		nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
		SDL_GL_SwapWindow(win);
	}




cleanup:
	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
