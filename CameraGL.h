#pragma once
#include "core.h"


// Default camera values
extern const float YAW;
extern const float PITCH;
extern const float SPEED;
extern const float SENSITIVITY;
extern const float ZOOM;
// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CameraGL : public Component
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed; 
	float MouseSensitivity;
	float Zoom;
	CameraGL(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	CameraGL::~CameraGL();
	CameraGL(GameObject& go, Transform& t);


	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);
	virtual void Update() override;
	virtual void HandleEvent(const SDL_Event & e) override;

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

	// Inherited via Component
};