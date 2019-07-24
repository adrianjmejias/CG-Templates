#include "Camera.h"

extern double deltaTime;

Camera::Camera(GameObject & go, Transform & t) : Component(go, t)
{
	Pitch = -48.05f;
	Yaw = 12.88f;
	transform.SetPosition({ 9.42f, -1.24, 5.14 });
	sensitivity = 0.10f;
	fov = 70;
	updateCameraVectors();
}

Camera::~Camera()
{
}

// Inherited via Component

void Camera::Update() {

	//PF_INFO("CAMERA POS , ROT");
	//std::cout << transform.GetPosition() << "/ " << transform.GetRotation()<<std::endl;
}

// Inherited via Component

void Camera::HandleEvent(const SDL_Event & e) {
	if (!SDL_GetRelativeMouseMode()) return;
	if (e.type == SDL_EventType::SDL_KEYDOWN) {
		//if (e.key.keysym.mod == SDL_Scancode::SDL_SCANCODE_LSHIFT)
		{
			float scaledSpeed = static_cast<float>(deltaTime) * speed;
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_W) {
				transform.Translate(Front * scaledSpeed);
			}
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_S) {
				transform.Translate(-Front * scaledSpeed);
			}
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_A) {
				transform.Translate(-Right * scaledSpeed);
			}
			if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_D) {
				transform.Translate(Right* scaledSpeed);
			}
		}
	}

	if (e.type == SDL_EventType::SDL_MOUSEMOTION) {
		float movX = sensitivity * e.motion.xrel * static_cast<float>(deltaTime);
		float movY = sensitivity * e.motion.yrel * static_cast<float>(deltaTime);

		float xoffset = e.motion.xrel* sensitivity;
		float yoffset = e.motion.yrel* sensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		//if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		
		transform.SetRotation(glm::degrees(Front));
		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, Transform::WorldUp()));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

Mat4 & Camera::GetView() {
	view = glm::lookAt(transform.GetPosition(), transform.GetPosition() + Front, Up);
	//view = transform.GetAccumulated();
	//view = Transform::GenModel(-transform.GetPosition(), -transform.GetRotation(), transform.GetScale());
	//return view;
	return view;
}

Mat4 & Camera::GetProjection(ProjectionType type, int w, int h) {
	if (type == ProjectionType::CAM_SETUP) {
		type = (isPerspective) ? ProjectionType::PERSPECTIVE : ProjectionType::ORTHO;
	}

	switch (type)
	{
	case ProjectionType::PERSPECTIVE:
		projection = glm::perspective(glm::radians(fov), static_cast<float>(w) / h, nearClippingPlane, farClippingPlane);
		break;
	case ProjectionType::ORTHO:
		projection = glm::ortho(0.f, static_cast<float>(w), 0.f, static_cast<float>(h), nearClippingPlane, farClippingPlane);
		break;
	default:
		break;
	}

	return projection;
}