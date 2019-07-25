#include "Camera.h"



Camera::Camera() : fc(t)
{
}


Camera::~Camera()
{
}

FlyingController::FlyingController(Transform & _t) : t(_t) {}

void FlyingController::HandleEvent(SDL_Event e) {

	if (e.type == SDL_EventType::SDL_MOUSEMOTION) {
		Vec2 deltaMouse = { e.motion.xrel, e.motion.yrel };

		t.Rotate(deltaMouse.y, deltaMouse.x, 0);

		Vec3 rotation = t.GetRotation();
		Vec3 front = t.Front();
		Vec3 right = t.Right();
		Vec3 up = t.Up();

		PF_INFO("cam rotation {0}, {1}, {2}", rotation.x, rotation.y, rotation.z);
		PF_INFO("cam front {0}, {1}, {2}", front.x, front.y, front.z);
		PF_INFO("cam right {0}, {1}, {2}", right.x, right.y, right.z);
		PF_INFO("cam up {0}, {1}, {2}", up.x, up.y, up.z);
		t.TryGetClean();
	}

	if (e.type == SDL_EventType::SDL_KEYDOWN) {

		switch (e.key.keysym.scancode) {
			PF_INFO("Moving Camera");
		
		case SDL_SCANCODE_W:
			t.Translate(t.Front() * speed);
			break;
		case SDL_SCANCODE_S:
			t.Translate(-t.Front() * speed);
			break;
		case SDL_SCANCODE_D:
			t.Translate(t.Right() * speed);
			break;
		case SDL_SCANCODE_A:
			t.Translate(-t.Right() * speed);
			break;
		default:
			return;
		}
		
		t.TryGetClean();
		Vec3 position = t.GetPosition();

		PF_INFO("cam position {0}, {1}, {2}", position.x, position.y, position.z);

	}
}
