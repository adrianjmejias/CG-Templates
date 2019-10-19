#include "types.h"
#include "Application.h"

extern double deltaTime = 0;
extern unsigned int GLOBAL_ID = 1;
enum class Dirty;


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
	SetupShaders(shaderPaths);
	SetupScene();

	rayCastShader = shaders[0];



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
	vol.Setup(volumeId);
	camera.reset (new OGLCamera(Vec3(0,0,0)));
}

void Application::SetupShaders(const std::vector<std::tuple<std::string, std::string>>& shaderPaths)
{
	const std::string baseShaderFolder = "";//"assets/shaders/";
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


	ImGui::Begin("Lights");
	{
		
		//Transform &camTransform = *camera->transform;
		//ImGui::Text("Camera transform");
		//ImGuiTransform(camTransform);



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

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) {
		camera->ProcessKeyboard(OGLCamera_Movement::FORWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_S]) {
		camera->ProcessKeyboard(OGLCamera_Movement::BACKWARD, deltaTime);
	}
	if (state[SDL_SCANCODE_A]) {
		camera->ProcessKeyboard(OGLCamera_Movement::LEFT, deltaTime);
	}
	if (state[SDL_SCANCODE_D]) {
		camera->ProcessKeyboard(OGLCamera_Movement::RIGHT, deltaTime);
	}
















	
	
	float ar = win_width / static_cast<float>(win_heigth);
	
	const Mat4 &view = camera->GetViewMatrix();
	const Mat4 &projection = glm::perspective(glm::radians(camera->Zoom), (float)win_width / (float)win_heigth, 0.1f, 100.0f);
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//vol.Render(deltaTime, view, projection, renderQuad);

	{

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);

		GLCALL(glBindVertexArray(vol.quadVAO));

		auto VP = projection * view;
		
		rayCastShader->Use();
		rayCastShader->SetUniform("VP", VP);
		rayCastShader->SetUniform("inverseVP", glm::inverse(VP));
		rayCastShader->SetUniform("camPos", camera->Position);

		//lastPass->SetUniform("deltaTime", vol.timePassed);

		GLCALL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

	}
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

		if (e.type == SDL_EventType::SDL_MOUSEMOTION)
		{

			if (!captureMouse)
			{
				camera->ProcessMouseMovement(e.motion.xrel, e.motion.yrel, true);
			}
		}
		if (e.type == SDL_EventType::SDL_MOUSEWHEEL)
		{

				camera->ProcessMouseScroll(e.wheel.y);
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

void Application::Steal(Component *comp)
{
	if (Light* l = dynamic_cast<Light*>(comp)) {
		LIGHTS.push_back(l);
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