#include "PressF/pch.h"
#include "Camera.h"
#include "PressF/Rendering/Renderer.h"
#include "PressF/Core/Window.h"

namespace PF
{

	
	void Camera::Update(const ImGuiIO& io)
	{	
		if (io.KeyCtrl)
		{
			Vec3 movement{ 0 };
			float camSpeed = io.DeltaTime * speed;

			if (ImGui::IsKeyPressed(SDL_SCANCODE_A))
			{
				movement -= Right * camSpeed;
			}

			if (ImGui::IsKeyPressed(SDL_SCANCODE_S))
			{
				movement -= Front * camSpeed;
			}

			if (ImGui::IsKeyPressed(SDL_SCANCODE_D))
			{
				movement += Right * camSpeed;
			}

			if (ImGui::IsKeyPressed(SDL_SCANCODE_W))
			{
				movement += Front * camSpeed;
			}
			transform->Translate(movement);



			Vec3 mDelta{ io.MouseDelta.x * MouseSensitivity, io.MouseDelta.y * MouseSensitivity, 0 };
			ProcessMouseMovement(mDelta.x, mDelta.y, true);
			ProcessMouseScroll(io.MouseWheel);
		}


	}

	void Camera::HandleEvent(const SDL_Event& e)
	{
	}

	json Camera::Serialize()
	{
		return json();
	}

	void Camera::OnEnable()
	{
		PF::Renderer::GetInstance()->RegisterCamera(this);
	}

	void Camera::OnDisable()
	{
		PF::Renderer::GetInstance()->UnRegisterCamera(this);
	}

	void Camera::Start()
	{
		//transform->Translate(0, 0, -4);
	}

	Mat4 Camera::GetProjectionMatrix()
	{
		//if(transform->)
		
		Window* win = Window::GetInstance();

		if (cameraType == ProjectionType::Perspective)
		{
			projection = glm::perspective(glm::radians(*fov), win->width / float(win->heigth), clippingPlanes.x, clippingPlanes.y);
		}
		else
		{
			projection = glm::ortho(0, win->width, 0, win->heigth);
		}

		return projection;
	}

	void Camera::ImGui()
	{
		ImGui::Text("Camera");

		ImGui::DragFloat4("ViewportRect", &viewportRect.x);
		ImGui::DragFloat2("ClippingPlanes", &clippingPlanes.x);
		ImGuiRender(priority, "Priority");
		ImGuiRender(fov, "FOV", 0.2f);
		ImGuiRender(speed, "Camera Speed", 0.2f);
		ImGuiRender(MouseSensitivity, "Mouse Sensitivity", 0.2f);
	}

	glm::mat4 Camera::GetViewMatrix(const Vec3& pos, const Vec3& front, const Vec3& up)
	{
		return glm::lookAt(pos, pos + front, up);
	}

	Mat4 Camera::GetViewMatrix()
	{
		return GetViewMatrix(transform->GetPosition(), Front, Up);
	}

	void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
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
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
	
}
