#pragma once 
#include "PressF/EntityComponent/Component.h"

namespace PF
{

	enum class RenderType
	{
		Stereoscopic,
		Normal
	};

	enum class ProjectionType
	{
		Perspective,
		Projection,
	};

	enum class ClearType
	{
		Skybox,
		SolidColor,
	};


	class Camera : public Component
	{
	public:
		Vec2 clippingPlanes{0.1f, 200.f};
		Vec4 viewportRect{0, 1, 0, 1};
		Int priority{ 0 };
		Mat4 projection;

		ClearType clearType = ClearType::SolidColor;
		ProjectionType cameraType = ProjectionType::Perspective;
		RenderType renderType{ RenderType::Stereoscopic };

		Float fov{ 45 };

		std::bitset<8> cullingMask;


		// Inherited via Component
		virtual void Update(const ImGuiIO& io) override;
		virtual json Serialize() override;

		// Inherited via Component
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		// Inherited via Component
		virtual void Start() override;

		Mat4 GetProjectionMatrix(float ar =-1);

		// Inherited via Component
		virtual void ImGui() override;


        // Camera Attributes
        glm::vec3 Front;
        glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
        // Euler Angles
        float Yaw;
        float Pitch;
        // Camera options
		Float speed{ 1 };
		Float MouseSensitivity{ 1 };
        float& Zoom = fov;

		Camera() : WorldUp{ Transform::WorldUp() }, Front{ Transform::WorldFront() }, Yaw{ -90.0f }, Pitch{ 0.0f }
        {
            updateCameraVectors();
        }

        // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		static glm::mat4 GetViewMatrix(const Vec3& pos, const Vec3& front, const Vec3& up);


		Mat4 GetViewMatrix();

		std::tuple<Mat4, Mat4> GetViewMatrixStereoscopic(float IOD, float zDistance);

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset);
        void updateCameraVectors();

    private:
        // Calculates the front vector from the Camera's (updated) Euler Angles
	};
}