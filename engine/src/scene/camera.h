#pragma once

#include "events/event.h"
#include "events/key_event.h"
#include "events/app_event.h"

#include "api/vulkan/vk_context.h"

#include <glm/glm.hpp>

namespace PhysiXal {

	class Camera
	{
    public:
        // Camera
        static void InitCamera(float fov, float width, float height, float zNear, float zFar);

        static void OnUpdate(GLFWwindow* window, float ts);

        static glm::mat4 UpdateViewMatrix();
        static void UpdateProjection(float fov, float width, float height, float zNear, float zFar);

        static glm::mat4 GetView();
        static glm::mat4 GetProjection();

        void OnEvent(Event& e);

        bool OnKeyPressed(KeyPressedEvent& e);
	private:
        inline static glm::mat4 m_View;
        inline static glm::mat4 m_Projection;

        inline static glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 6.0f);
        inline static glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
        inline static glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        inline static float m_Yaw = -90.0f;
        inline static float m_Pitch = 0.0f;
        inline static float m_LastX = 800.0f / 2.0f;
        inline static float m_LastY = 600.0f / 2.0f;
        inline static bool m_InitMouse = true;

        inline static float m_Fov;
        inline static float m_Width;
        inline static float m_Height;
        inline static float m_ZNear;
        inline static float m_ZFar;
	};
}