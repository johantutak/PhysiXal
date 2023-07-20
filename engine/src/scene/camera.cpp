#include "px_pch.h"
#include "scene/camera.h"

#include "events/event.h"
#include "events/app_event.h"
#include "events/key_event.h"

#include "api/vulkan/vk_context.h"

#include "core/input/input.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Camera
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Camera::InitCamera(float fov, float width, float height, float zNear, float zFar)
	{
		UpdateProjection(fov, width, height, zNear, zFar);
	}

	void Camera::OnUpdate(GLFWwindow* window, float ts)
	{
		// #### This function handles camera controls  ####

		if (Input::IsMouseButtonPressed(MouseCode::ButtonRight))
		{
			// Calculate camera position
			float cameraSpeed = 10.0f * ts;

			PX_CORE_INFO("Mouse button (right) is pressed");

			if (Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
			{
				PX_CORE_INFO("Mouse button (left) is pressed");
				// Calculate mouse timestep
				if (s_InitMouse)
				{
					s_LastX = Input::GetMouseX();
					s_LastY = Input::GetMouseY();
					s_InitMouse = false;
				}

				float xoffset = Input::GetMouseX() - s_LastX;
				float yoffset = s_LastY - Input::GetMouseY();
				s_LastX = Input::GetMouseX();
				s_LastY = Input::GetMouseY();

				// Update camera angles based on mouse timestep
				float sensitivity = 0.3f;
				xoffset *= sensitivity;
				yoffset *= sensitivity;
				s_Yaw += xoffset;
				s_Pitch += yoffset;
			}

			// Update camera angles based on keyboard input
			float sensitivity = 0.3f;

			if (Input::IsKeyPressed(Key::Right))
			{
				PX_CORE_INFO("Right is pressed"), s_Yaw += sensitivity;
			}
			if (Input::IsKeyPressed(Key::Left))
			{
				PX_CORE_INFO("Left is pressed"), s_Yaw -= sensitivity;
			}
			if (Input::IsKeyPressed(Key::Up))
			{
				PX_CORE_INFO("Up is pressed"), s_Pitch += sensitivity;
			}
			if (Input::IsKeyPressed(Key::Down))
			{
				PX_CORE_INFO("Down is pressed"), s_Pitch -= sensitivity;
			}

			// #### X-axis ####
			// To move up on x-axis (right in sence)
			if (Input::IsKeyPressed(Key::D))
			{
				PX_CORE_INFO("D is pressed"), s_CameraPos += glm::normalize(glm::cross(s_CameraFront, s_CameraUp)) * cameraSpeed;
			}
			// To move down on x-axis (left in sence)
			if (Input::IsKeyPressed(Key::A))
			{
				PX_CORE_INFO("A is pressed"), s_CameraPos -= glm::normalize(glm::cross(s_CameraFront, s_CameraUp)) * cameraSpeed;
			}

			// #### Y-axis ####
			// To move up on y-axis (up in sence)
			if (Input::IsKeyPressed(Key::Q))
			{
				PX_CORE_INFO("Q is pressed"), s_CameraPos += cameraSpeed * s_CameraUp;
			}
			// To move down on y-axis (down in sence)
			if (Input::IsKeyPressed(Key::E))
			{
				PX_CORE_INFO("E is pressed"), s_CameraPos -= cameraSpeed * s_CameraUp;
			}

			// #### Z-axis ####
			// To move up on z-axis (forward in sence)
			if (Input::IsKeyPressed(Key::W))
			{
				PX_CORE_INFO("W is pressed"), s_CameraPos += cameraSpeed * s_CameraFront;
			}
			// To move up on z-axis (backwards in sence)
			if (Input::IsKeyPressed(Key::S))
			{
				PX_CORE_INFO("S is pressed"), s_CameraPos -= cameraSpeed * s_CameraFront;
			}

			Input::SetCursorMode(false);
		}
		else
		{
			Input::SetCursorMode(true);
		}

		// constrain pitch
		if (s_Pitch > 89.0f)
			s_Pitch = 89.0f;
		if (s_Pitch < -89.0f)
			s_Pitch = -89.0f;

		// Calculate camera direction
		glm::vec3 front;
		front.x = cos(glm::radians(s_Yaw)) * cos(glm::radians(s_Pitch));
		front.y = sin(glm::radians(s_Pitch));
		front.z = sin(glm::radians(s_Yaw)) * cos(glm::radians(s_Pitch));
		s_CameraFront = glm::normalize(front);
	}

	glm::mat4 Camera::UpdateViewMatrix()
	{
		s_View = glm::lookAt(s_CameraPos, s_CameraPos + s_CameraFront, s_CameraUp);

		return s_View;
	}

	void Camera::UpdateProjection(float fov, float width, float height, float zNear, float zFar)
	{
		s_Fov = fov;
		s_Width = width;
		s_Height = height;
		s_ZNear = zNear;
		s_ZFar = zFar;

		s_Projection = glm::perspective(glm::radians(s_Fov), s_Width / s_Height, s_ZNear, s_ZFar);
	}

	glm::mat4 Camera::GetView()
	{
		UpdateViewMatrix();

		return s_View;
	}

	glm::mat4 Camera::GetProjection()
	{
		return s_Projection;
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(PX_BIND_EVENT_FN(Camera::OnKeyPressed));
	}

	bool Camera::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}
}