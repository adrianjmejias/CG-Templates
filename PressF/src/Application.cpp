#include "Application.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024





void Application::Setup(const std::vector<std::string>& objPaths, const std::vector<std::string>& shaderPaths)
{

	spdlog::set_pattern("[%M:%S %z] [%n] [%^---%L---%$] %v"); 
	for each (std::string objPath in objPaths)
	{
		Model model;

		if (!model.LoadFile(objPath)) {
			PF_ERROR("Failed to load model {0}", objPath);
			__debugbreak();
		}

		models.push_back(model);
	}

	for each (std::string shaderPath in shaderPaths)
	{
		Model model;

		if (!model.LoadFile(shaderPath)) {
			PF_ERROR("Failed to load shader {0}", shaderPath);
			__debugbreak();
		}

		models.push_back(model);
	}


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
		}
		//	if (evt.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_C) {
		//		//PF_INFO("capture {0}", capture);
		//		if (SDL_SetRelativeMouseMode(static_cast<SDL_bool>(captureMouse)) == -1) {
		//			__debugbreak();
		//		}
		//		captureMouse = !captureMouse;
		//	}
		//	if (evt.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_R) {
		//	/*	for each (auto pair in meshes)
		//		{
		//			for (auto mat : pair->mtl->materials) {
		//				mat.second->shader->ReCompile();
		//			}
		//		}*/
		//	}
		//}

	/*	for each (auto go in callOrder)
		{
			go->HandleEvent(evt);
		}*/

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
	//	for each (Mesh* m in meshes)
	//	{
	//		PF_ASSERT(m && "Mesh is not null");
	//
	//		Mesh& mesh = *m;
	//		auto &materialOrderForRender = mesh.materialOrderForRender;
	//
	//		for (size_t ii = 0; ii < materialOrderForRender.size(); ii++)
	//		{
	//			const Material &MAT = materialOrderForRender[ii].mat;
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
	//	}
	//	//cubemap->Render();
	//
	//	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
	//
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
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
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
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


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
