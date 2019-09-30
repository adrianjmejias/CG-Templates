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
	float speed = 10.f;
	float sensitivity = 0.2f;
	bool blockRotation = true;
	bool gimballProtection = true;
public:
	FlyingController(COMP_PARAMS);

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
	float fov = 45.f;
	float speed = 25.f;
	float sensitivity = 0.3f;
	float nearClippingPlane = 0.1f;
	float farClippingPlane = 800.f;
	Vec4 screenOrtho = {};
	Mat4 projection;
	Mat4 view;

	Camera(COMP_PARAMS);
	Camera::~Camera();

	// Inherited via Component
	virtual void Update() override;

	virtual void HandleEvent(const SDL_Event &e) override;

	const Mat4& GetView()
	{
		Transform &transform = *this->transform;
		const Vec3 &position = transform.GetPosition();

		view = glm::lookAt(position, position +transform.Front(), transform.Up());

		return view;
	}
	const Mat4& GetProjection(float ar)
	{
		switch (type)
		{
		case ProjectionType::PERSPECTIVE:
			view = Camera::GetPerspective(fov, ar, nearClippingPlane, farClippingPlane);
			break;
		case ProjectionType::ORTHO:
			view = Camera::GetOrtho(screenOrtho, nearClippingPlane, farClippingPlane);
			break;
		case ProjectionType::CAM_SETUP:
			break;
		default:
			break;
		}

		return view;
	}

	static Mat4 GetPerspective(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane)
	{
		return glm::perspective(glm::radians(fov), aspectRatio, nearClippingPlane, farClippingPlane);
	}
	static Mat4 GetOrtho(Vec4 screenOrtho, float nearClippingPlane, float farClippingPlane)
	{
		return glm::ortho(screenOrtho[3], screenOrtho[1], screenOrtho[2], screenOrtho[0], nearClippingPlane, farClippingPlane);
	}
};

// 0414 305 9800