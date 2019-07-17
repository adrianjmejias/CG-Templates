#include "Scene.hpp"



void Scene::SetCallbacks() {
	// glfwSetWindowSizeCallback(c_input->window, CallbackWindowResize);
	// glfwSetKeyCallback(c_input->window, CallbackKey// glfw);
	// glfwSetMouseButtonCallback(c_input->window, CallbackMouseButton);
	// glfwSetCursorPosCallback(c_input->window, CallbackMousePosition);
	// glfwSetScrollCallback(c_input->window, CallbackMouseScroll);
	//// glfwSetCharCallback(c_window->window, (// glfwcharfun)TwEventChar// glfw);

}


void Scene::SetInput(SystemInput *input) {
	c_input = input;
}

bool Scene::Init() {


	c_window = new Window();
	c_ui = new UI();
	c_time = new Time();
	c_input = new SystemInput();
	c_api = new API();
	c_renderer = new SystemRenderer();



	bool isInit = c_window->Init() && c_ui->Init();
	isInit = isInit && c_time->Init();
	isInit = isInit && c_renderer->Init();
	isInit = isInit && c_input->Init();


	c_api->c_input = c_input;
	c_renderer->c_api = c_api;
	c_api->c_time = c_time;
	c_api->c_window = c_window;

	//c_window->SetSize(c_window->width, c_window->height);
	//c_camera_comp = new Camera(&c_camera);
	//c_camera.AddComponent(c_camera_comp);

	return isInit;
}

void Scene::Run() {
	c_renderer->Init();


	for (; ; ) {

		if (!c_window->EarlyUpdate()) return;
		c_time->Update([&]() {
			c_input->Update();

			std::queue<Entity*> callOrder; //bfs
			for (unsigned int ii = 0; ii < size(); ii++)
			{
				CGASSERT(at(ii).get() != nullptr); // asumo que no es null
				callOrder.push(at(ii).get());
			}

			while (!callOrder.empty())
			{
				Entity *ent = callOrder.front();
				callOrder.pop();

				CGASSERT(ent != nullptr);

				ent->Update(*c_api);

				for (Transform *child : ent->transform->children)
				{
					CGASSERT(child->ent != nullptr);
					callOrder.push(child->ent);
				}
			}
			c_renderer->Update();
			c_ui->Update();

		});
		c_window->LateUpdate();
	}
}

void Scene::AddEntity(std::vector<Entity*> entities) {

	for (auto ent : entities) {
		CGASSERT(ent != nullptr);

		std::unique_ptr<Entity> p(ent);
		this->push_back(std::move(p));

		ITweakable * tw = dynamic_cast<ITweakable*>(ent->transform);
		if (tw) {
			c_ui->AddITweakable(tw);
		}

		for (int ii = ent->components.size() - 1; ii >= 0; ii--)
		{
			Component *comp = ent->components[ii];
			CGASSERT(comp != nullptr);

			ITweakable * tw = dynamic_cast<ITweakable*>(comp);
			if (tw) {
				c_ui->AddITweakable(tw);
			}

			StartableComponent* start(dynamic_cast<StartableComponent*>(comp));

			if (start) {
				start->Start(*c_api);
			}

			ent->transform->SetDirty(Dirty::Model);

			MeshRenderer* mr = dynamic_cast<MeshRenderer*>(comp);

			if (mr) {
				c_renderer->AddRenderer(mr);
				std::swap(ent->components[ii], ent->components[ent->components.size() - 1]);
				ent->components.pop_back();
				continue;
			}

			Light* light = dynamic_cast<Light*>(comp);

			if (light) {
				c_renderer->AddLight(light);
				std::swap(ent->components[ii], ent->components[ent->components.size() - 1]);
				ent->components.pop_back();
				continue;
			}

			Camera* cam = dynamic_cast<Camera*>(comp);

			if (cam) {
				c_renderer->AddCamera(cam);
				std::swap(ent->components[ii], ent->components[ent->components.size() - 1]);
				ent->components.pop_back();
				continue;
			}
		}

	}

}

Scene::~Scene() {
	delete c_api;
	delete c_input;
	delete c_ui;
	delete c_time;
	delete c_window;
}

void Scene::KeyListener(KeyCode k, int event) {
	c_input->KeyListener(k, event);
}

void Scene::MousePosition(double xPos, double yPos) {
	//c_input->KeyListener(k, event);
	c_input->OnMousePosition(xPos, yPos);
}

void Scene::MouseButton(int button, int action, int mods) {
	//c_input->KeyListener(k, event);
	c_input->OnMouseButton(button, action, mods);
}

void Scene::MouseScroll(double xOffset, double yOffsets) {
	//c_input->KeyListener(k, event);
	c_input->OnMouseScroll(xOffset, yOffsets);
}

