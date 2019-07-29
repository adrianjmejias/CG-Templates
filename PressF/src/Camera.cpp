#include "Camera.h"

extern double deltaTime;




Camera::Camera(GameObject & go, Transform & t) : Component(go, t) {}

Camera::~Camera()
{
}

// Inherited via Component

void Camera::Update() {
}

// Inherited via Component

void Camera::HandleEvent(const SDL_Event & e) {

}

const Mat4 & Camera::GetProjection(ProjectionType type, int w, int h)
{
	if (type == ProjectionType::CAM_SETUP) {
		type = type;
	}


	if (isPerspective) {
		view = glm::perspective(glm::radians(45.f), w / static_cast<float>(h), 0.1f, 300.f);
	}
	else
	{
		view = glm::ortho(0.f, 100.0f, 0.f, 100.0f, 0.01f, 400.f);
	}

	return view;
}
const Mat4 & Camera::GetView()
{
	view = Transform::GenModel(transform.GetScale(), -transform.GetPosition(), transform.GetRotation());

	return view;
}

FlyingController::FlyingController(GameObject& go, Transform& t) : Component(go, t) {}

void FlyingController::Update()
{
}

void FlyingController::HandleEvent(const SDL_Event & e)
{
	if (e.type == SDL_EventType::SDL_KEYDOWN) {

		switch (e.key.keysym.scancode) {
			PF_INFO("Moving Camera");

		case SDL_SCANCODE_W:
			transform.Translate(transform.Front() * speed);
			break;
		case SDL_SCANCODE_S:
			transform.Translate(-transform.Front() * speed);
			break;
		case SDL_SCANCODE_D:
			transform.Translate(-transform.Right() * speed);
			break;
		case SDL_SCANCODE_A:
			transform.Translate(transform.Right() * speed);
			break;
		case SDL_SCANCODE_C:
			blockRotation = !blockRotation;
			break;
		default:
			return;
		}

		//transform.TryGetClean();
		//Vec3 position = transform.GetPosition();

		//PF_INFO("cam position {0}, {1}, {2}", position.x, position.y, position.z);

	}



	if (blockRotation) return;
	if (e.type == SDL_EventType::SDL_MOUSEMOTION) {
		Vec2 deltaMouse = { e.motion.xrel, e.motion.yrel };
		deltaMouse*= sensitivity;
		transform.Rotate(deltaMouse.y, deltaMouse.x, 0);

		//Vec3 rotation = transform.GetRotation();
		//Vec3 front = transform.Front();
		//Vec3 right = transform.Right();
		//Vec3 up = transform.Up();
		//transform.TryGetClean();

		//PF_INFO("cam rotation {0}, {1}, {2}", rotation.x, rotation.y, rotation.z);
		//PF_INFO("cam front {0}, {1}, {2}", front.x, front.y, front.z);
		//PF_INFO("cam right {0}, {1}, {2}", right.x, right.y, right.z);
		//PF_INFO("cam up {0}, {1}, {2}", up.x, up.y, up.z);
	}



}
