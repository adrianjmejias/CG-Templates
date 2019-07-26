#pragma once
#include "core.h"

enum class ProjectionType {
	PERSPECTIVE,
	ORTHO,
	CAM_SETUP
};
class Camera;



class FlyingController : public Component
{
	float speed = 0.5f;
	float sensitivity = 0.2f;
	bool blockRotation = true;
public:
	FlyingController(GameObject& go, Transform& t);

	// Inherited via Component
	virtual void Update() override;
	virtual void HandleEvent(const SDL_Event & e) override;
};



extern Camera* mainCamera;

class Camera : public Component{

public:
	int power = 0;
	int isPerspective = true;
	ProjectionType type = ProjectionType::PERSPECTIVE;
	float speed = 25.f;
	float sensitivity = 0.3f;
	float fov = 45;
	float nearClippingPlane = 0.1f;
	float farClippingPlane = 400.f;

	Mat4 projection;
	Mat4 view;

	Camera(GameObject& go, Transform& t);;
	Camera::~Camera();

	// Inherited via Component
	virtual void Update() override;

	virtual void HandleEvent(const SDL_Event &e) override;

	virtual const Mat4& GetView();
	virtual const Mat4& GetProjection(ProjectionType type, int w, int h);
};

// 0414 305 9800