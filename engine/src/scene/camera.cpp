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
				if (m_InitMouse)
				{
					m_LastX = Input::GetMouseX();
					m_LastY = Input::GetMouseY();
					m_InitMouse = false;
				}

				float xoffset = Input::GetMouseX() - m_LastX;
				float yoffset = m_LastY - Input::GetMouseY();
				m_LastX = Input::GetMouseX();
				m_LastY = Input::GetMouseY();

				// Update camera angles based on mouse timestep
				float sensitivity = 0.3f;
				xoffset *= sensitivity;
				yoffset *= sensitivity;
				m_Yaw += xoffset;
				m_Pitch += yoffset;
			}

			// Update camera angles based on keyboard input
			float sensitivity = 0.3f;

			if (Input::IsKeyPressed(Key::Right))
			{
				PX_CORE_INFO("Right is pressed"), m_Yaw += sensitivity;
			}
			if (Input::IsKeyPressed(Key::Left))
			{
				PX_CORE_INFO("Left is pressed"), m_Yaw -= sensitivity;
			}
			if (Input::IsKeyPressed(Key::Up))
			{
				PX_CORE_INFO("Up is pressed"), m_Pitch += sensitivity;
			}
			if (Input::IsKeyPressed(Key::Down))
			{
				PX_CORE_INFO("Down is pressed"), m_Pitch -= sensitivity;
			}

			// #### X-axis ####
			// To move up on x-axis (right in sence)
			if (Input::IsKeyPressed(Key::D))
			{
				PX_CORE_INFO("D is pressed"), m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
			}
			// To move down on x-axis (left in sence)
			if (Input::IsKeyPressed(Key::A))
			{
				PX_CORE_INFO("A is pressed"), m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
			}

			// #### Y-axis ####
			// To move up on y-axis (up in sence)
			if (Input::IsKeyPressed(Key::Q))
			{
				PX_CORE_INFO("Q is pressed"), m_CameraPos += cameraSpeed * m_CameraUp;
			}
			// To move down on y-axis (down in sence)
			if (Input::IsKeyPressed(Key::E))
			{
				PX_CORE_INFO("E is pressed"), m_CameraPos -= cameraSpeed * m_CameraUp;
			}

			// #### Z-axis ####
			// To move up on z-axis (forward in sence)
			if (Input::IsKeyPressed(Key::W))
			{
				PX_CORE_INFO("W is pressed"), m_CameraPos += cameraSpeed * m_CameraFront;
			}
			// To move up on z-axis (backwards in sence)
			if (Input::IsKeyPressed(Key::S))
			{
				PX_CORE_INFO("S is pressed"), m_CameraPos -= cameraSpeed * m_CameraFront;
			}

			Input::SetCursorMode(false);
		}
		else
		{
			Input::SetCursorMode(true);
		}

		// constrain pitch
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		// Calculate camera direction
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraFront = glm::normalize(front);
	}

	glm::mat4 Camera::UpdateViewMatrix()
	{
		m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);

		return m_View;
	}

	void Camera::UpdateProjection(float fov, float width, float height, float zNear, float zFar)
	{
		m_Fov = fov;
		m_Width = width;
		m_Height = height;
		m_ZNear = zNear;
		m_ZFar = zFar;

		m_Projection = glm::perspective(glm::radians(m_Fov), m_Width / m_Height, m_ZNear, m_ZFar);
	}

	glm::mat4 Camera::GetView()
	{
		UpdateViewMatrix();

		return m_View;
	}

	glm::mat4 Camera::GetProjection()
	{
		return m_Projection;
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