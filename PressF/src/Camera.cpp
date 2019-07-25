#include "Camera.h"

extern double deltaTime;




Camera::~Camera()
{
}

// Inherited via Component

void Camera::Update() {
}

// Inherited via Component

void Camera::HandleEvent(const SDL_Event & e) {

}

Mat4 & Camera::GetView()
{
	// TODO: insert return statement here
}

Mat4 & Camera::GetProjection(ProjectionType type, int w, int h)
{
	// TODO: insert return statement here
}
