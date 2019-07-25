#pragma once
#include "core.h"

enum class ProjectionType {
	PERSPECTIVE,
	ORTHO,
	CAM_SETUP
};
class Camera;

extern Camera* mainCamera;

class Camera : public Component{

public:
	int power = 0;
	int isPerspective = true;
	float speed = 25.f;
	float sensitivity = 0.3f;
	float fov = 45;
	float nearClippingPlane = 0.1f;
	float farClippingPlane = 400.f;

	Mat4 projection;
	Mat4 view;

	// Inherited via Component
	virtual void Update() override;

	virtual void HandleEvent(const SDL_Event &e) override;

	virtual Mat4& GetView();
	virtual Mat4& GetProjection(ProjectionType type, int w, int h);
};