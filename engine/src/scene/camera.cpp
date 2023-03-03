#include "px_pch.h"
#include "scene/camera.h"

#include "events/event.h"
#include "events/key_event.h"

#include "core/input/input.h"

#include <glm/gtc/matrix_transform.hpp>

#include "events/app_event.h"

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Camera
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Camera::Camera(float fov, float width, float height, float zNear, float zFar)
		: m_Position(-20.0f, 20.0f, 20.0f)
		, m_Forward(0.0f, 0.0f, 1.0f)
		, m_Yaw(0.0f)
		, m_Pitch(0.0f)
	{
		UpdateProjection(fov, width, height, zNear, zFar);
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(PX_BIND_EVENT_FN(Camera::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(PX_BIND_EVENT_FN(Camera::OnKeyPressed));
	}

	void Camera::OnUpdate()
	{
		glm::vec3 movement{};

		// Movement

		// To move up and down on x-axis (left or right in sence)
		if (Input::IsKeyPressed(Key::A))
			PX_CORE_INFO("A is pressed"),
			movement.x -= 1;
		if (Input::IsKeyPressed(Key::D))
			PX_CORE_INFO("D is pressed"),
			movement.x += 1;

		// To move up and down on y-axis (up or down in sence)
		if (Input::IsKeyPressed(Key::W))
			PX_CORE_INFO("W is pressed"),
			movement.y += 1;
		if (Input::IsKeyPressed(Key::S))
			PX_CORE_INFO("S is pressed"),
			movement.y -= 1;

		// To move up and down on z-axis (forward or backwards in sence)
		if (Input::IsKeyPressed(Key::Q))
			PX_CORE_INFO("Q is pressed"),
			movement.z -= 1;
		if (Input::IsKeyPressed(Key::E))
			PX_CORE_INFO("E is pressed"),
			movement.z += 1;

		// constrain pitch
		if (m_Pitch > 89.9f)
			m_Pitch = 89.9f;
		if (m_Pitch < -89.9f)
			m_Pitch = -89.9f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(-m_Pitch));
		front = glm::normalize(front);
		m_Forward = front;

		if (glm::length(movement) > 0)
		{
			const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			const glm::vec3 forward = glm::normalize(m_Forward);
			const glm::vec3 right = glm::normalize(glm::cross(m_Forward, up));

			movement = glm::normalize(movement);
			glm::vec3 mov{};

			mov += movement.x * right;
			mov += movement.y * forward;
			mov += movement.z * up;

			// #### TO Do #### set movement speed to timestep/v-sync
			// Do it like example under
			//m_Position += mov * Time::GetDeltaTime() * (m_IsSlowMovement ? m_MoveSpeedSlow : m_MoveSpeed);
		}
	}

	glm::mat4 Camera::GetView()
	{
		UpdateViewMatrix();

		return m_View;
	}

	glm::mat4 Camera::GetProjection() const
	{
		return m_Projection;
	}

	glm::vec3 Camera::GetPosition() const
	{
		return m_Position;
	}

	bool Camera::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() <= 0 || e.GetHeight() <= 0)
			return false;

		UpdateProjection(m_Fov, static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()), m_ZNear, m_ZFar);
		return false;
	}

	bool Camera::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == KeyCode::LeftShift)
			m_IsSlowMovement = !m_IsSlowMovement;

		return false;
	}

	void Camera::UpdateViewMatrix()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Forward, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Camera::UpdateProjection(float fov, float width, float height, float zNear, float zFar)
	{
		m_Fov = fov;
		m_Width = width;
		m_Height = height;
		m_ZNear = zNear;
		m_ZFar = zFar;

		m_Projection = glm::perspective(glm::radians(m_Fov / 2), m_Width / m_Height, m_ZNear, m_ZFar);
	}
}