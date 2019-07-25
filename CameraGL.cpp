#include "CameraGL.h"


const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.2f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


extern double deltaTime;

CameraGL::~CameraGL()
{
}


// Calculates the front vector from the Camera's (updated) Euler Angles


// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis


// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.


// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)


// Returns the view matrix calculated using Euler Angles and the LookAt Matrix


// Constructor with scalar values


// Constructor with vectors





CameraGL::CameraGL(GameObject & go, Transform & t) : Component(go, t)
{
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = YAW;
	Pitch = PITCH;
	updateCameraVectors();
}


glm::mat4 CameraGL::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}


void CameraGL::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void CameraGL::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void CameraGL::Update()
{
}

void CameraGL::HandleEvent(const SDL_Event & e)
{
	PF_INFO("Moving glcamera");
	if (e.type == SDL_EventType::SDL_KEYDOWN) {
		//if (e.key.keysym.mod == SDL_Scancode::SDL_SCANCODE_LSHIFT)
		{
			float scaledSpeed = static_cast<float>(deltaTime) * MovementSpeed;

			{
				float velocity = MovementSpeed * deltaTime;
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_W) {
					Position += Front * velocity;


					transform.Translate(Front * scaledSpeed);
				}
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_S) {
					Position -= Front * velocity;

					transform.Translate(-Front * scaledSpeed);
				}
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_A) {
					Position -= Right * velocity;
					transform.Translate(-Right * scaledSpeed);
				}
				if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_D) {
					Position += Right * velocity;
					transform.Translate(Right* scaledSpeed);
				}
			}
		}
		if (e.type == SDL_EventType::SDL_MOUSEMOTION) {
			float movX = MouseSensitivity * e.motion.xrel * static_cast<float>(deltaTime);
			float movY = MouseSensitivity * e.motion.yrel * static_cast<float>(deltaTime);

			float xoffset = e.motion.xrel* MouseSensitivity;
			float yoffset = e.motion.yrel* MouseSensitivity;

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
}
