#pragma once 

#include "../misc/KeyCodes.hpp"
#include "../internal/Window.hpp"
//#include <// glfw/// glfw3.h>
//#include <imgui\imgui_impl_sdl.h>
#include <unordered_map>
#include <vector>
#include <functional>

//ext

class SystemInput;



class SystemInput
{
public:
	SDL_Window *window;
	SystemInput() {

	}

	virtual ~SystemInput() = default;
	std::unordered_map<KeyCode, std::vector<std::function<void()> > > keyDown;
	std::unordered_map<KeyCode, std::vector<std::function<void()> > > keyUp;
	std::unordered_map<KeyCode, std::vector<std::function<void()> > > KeyPressed;
	std::vector < std::function<void(double, double)> > mousePos;
	std::vector < std::function<void(double, double)> > mouseDelta;
	std::vector < std::function<void(double, double)>  > mouseScroll;
	std::unordered_map <MouseButton, std::vector< std::function<void(Mod)> > > mouseButtonDown;
	std::unordered_map <MouseButton, std::vector<std::function<void(Mod)> > > mouseButtonUp;

	ImGuiIO *io;
	double yAct = 0;
	double xAct = 0;
	double yLast = 0;
	double xLast = 0;


	bool Init(SDL_Window *window) {
		this->window = window;
		io = &ImGui::GetIO();

		return true;
	}

	virtual void Update() {

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				//done = true;

			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
				//done = true;
			}
		}

	}
	bool Init() {

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO();


		
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings


		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'misc/fonts/README.txt' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

		return true;
	}
	void KeyListener(KeyCode k, int event) {


		//switch (event)
		//{
		//case // glfw_PRESS:
		//	for (auto &callback : keyDown[k]) {
		//		callback();
		//	}
		//	break;
		//case // glfw_RELEASE:
		//	for (auto &callback : keyUp[k]) {
		//		callback();
		//	}
		//	break;

		//case // glfw_REPEAT:
		//	for (auto &callback : KeyPressed[k]) {
		//		callback();
		//	}
		//	break;
		//default:
		//	break;
		//}
	}
	void RegisterGetKey(KeyCode k, std::function<void()> f) {
		this->KeyPressed[k].push_back(f);
	}
	void RegisterGetKeyDown(KeyCode k, std::function<void()> f) {
		this->keyDown[k].push_back(f);
	}
	void RegisterGetKeyUp(KeyCode k, std::function<void()> f) {
		this->keyUp[k].push_back(f);
	}
	void RegisterMouseDelta(std::function<void(double, double)> f) {
		mouseDelta.push_back(f);
	}
	void RegisterMousePos(std::function<void(double, double)> f) {
		mousePos.push_back(f);
	}
	void RegisterMouseScroll(std::function<void(double, double)> f) {
		mouseScroll.push_back(f);
	}
	void RegisterMouseButtonDown(MouseButton m, std::function<void(Mod)> f) {
		mouseButtonDown[m].push_back(f);
	}
	void RegisterMouseButtonUp(MouseButton m, std::function<void(Mod)> f) {
		mouseButtonUp[m].push_back(f);
	}
	int mouseDeltaX, mouseDeltaY;
	void OnMousePosition(double xPos, double yPos) {
		xAct = xPos;
		yAct = yPos;
		for (auto f : mousePos) {
			f(xAct, yAct);
		}


		int deltaX = static_cast<int>(xAct - xLast);
		int deltaY = static_cast<int>(yAct - yLast);
		mouseDeltaX = deltaX;
		mouseDeltaY = deltaY;

		//std::cout << "DeltaX " << deltaX << " = " << xAct << "- "<<xLast<< std::endl;
		//std::cout << "DeltaY " << deltaY << " = " << yAct << "- "<<yLast<< std::endl;

		for (auto f : mouseDelta) {
			f(deltaX, deltaY);
		}



		yLast = yAct;
		xLast = xAct;
	}
	void OnMouseButton(int button, int action, int mods) {
		MouseButton a = static_cast<MouseButton>(button);
		Mod b = static_cast<Mod>(mods);

		//if (action == // glfw_PRESS) {
		//	for (auto &f : mouseButtonDown[a]) {
		//		f(b);
		//	}
		//}
		//else if (action == // glfw_RELEASE) {
		//	for (auto &f : mouseButtonUp[a]) {
		//		f(b);
		//	}
		//}

	}
	void OnMouseScroll(double xoffset, double yoffsets) {
		for (auto f : mouseScroll) {
			f(xoffset, yoffsets);
		}
	}
	void OnMouseEnter() {
		yLast = yAct;
		xLast = xAct;
	}
	void OnMouseLeave() {

	}


};


