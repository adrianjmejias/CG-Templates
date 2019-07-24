#pragma once
#include "core.h"

enum class ProjectionType {
	PERSPECTIVE,
	ORTHO,
	CAM_SETUP
};


class Camera : public Component{

public:
	int power = 10000;
	int isPerspective = true;
	float speed = 25.f;
	float sensitivity = 0.3f;
	float fov = 45;
	float nearClippingPlane = 0.1f;
	float farClippingPlane = 200.0f;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	float Yaw;
	float Pitch;

	Mat4 projection;
	Mat4 view;

	Camera(GameObject& go, Transform& t);
	Camera::~Camera();

	// Inherited via Component
	virtual void Update() override;

	//// Inherited via Component
	//virtual void UI() override {
	///*	nk_checkbox_label(ctx, "IsPerspective", &isPerspective);
	//	power = nk_propertyi(ctx, "Power", 0, power, 10000, 1.f, 1.f);
	//	fov = nk_propertyf(ctx, "Fov", 30.f, fov, 120.0f, 0.01f, 0.005f);
	//	nearClippingPlane = nk_propertyf(ctx, "Near", 0.001, nearClippingPlane, farClippingPlane, 1.f, 0.005f);
	//	farClippingPlane = nk_propertyf(ctx, "Far", nearClippingPlane, farClippingPlane, 6000, 1.f, 0.005f);
	//	speed = nk_propertyf(ctx, "Speed", 50, speed, 500, 1.f, 0.005f);
	//	sensitivity = nk_propertyf(ctx, "Sensitivity", 0.001f, sensitivity, 3, 00.1f, 0.005f);*/
	//}

	virtual void HandleEvent(const SDL_Event &e) override;

	void updateCameraVectors();
	virtual Mat4& GetView();

	virtual Mat4& GetProjection(ProjectionType type, int w, int h);
};