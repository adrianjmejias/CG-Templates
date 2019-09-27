
#include "types.h"
#include "Camera.h"

extern double deltaTime;




Camera::Camera(COMP_PARAMS) COMP_INIT {}

Camera::~Camera()
{
}

// Inherited via Component

void Camera::Update() {
}

// Inherited via Component

void Camera::HandleEvent(const SDL_Event & e) {

}

FlyingController::FlyingController(COMP_PARAMS) COMP_INIT {}

void FlyingController::Update()
{
}

void FlyingController::HandleEvent(const SDL_Event & e)
{
	Transform& transform = *this->transform;
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
		transform.Rotate(deltaMouse.y, -deltaMouse.x, 0);

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
