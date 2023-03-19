#include "scene/camera_controller.h"

#include <limits>

#include "core/input/input.h"

#include "glm/glm.hpp"

#include "GLFW/glfw3.h"

namespace PhysiXal {

    void CameraController::MoveInPlaneXZ(/*GLFWwindow* window, float ts, LveGameObject& ubo*/)
    {
        glm::vec3 rotate{ 0 };
        // #### X-axis ####
        // To rotate up on x-axis (rotate right in sence)
        if (Input::IsKeyPressed(Key::Right))
        {
            PX_CORE_INFO("Right is pressed"), rotate.y += 1.f;
        }

        // To rotate down on x-axis (rotate left in sence)
        if (Input::IsKeyPressed(Key::Left))
        {
            PX_CORE_INFO("Left is pressed"), rotate.y -= 1.f;
        }

        // #### Y-axis ####
        // To rotate up on y-axis (rotate up in sence)
        if (Input::IsKeyPressed(Key::Up))
        {
            PX_CORE_INFO("Up is pressed"), rotate.x += 1.f;
        }

        // To rotate down on y-axis (rotate down in sence)
        if (Input::IsKeyPressed(Key::Down))
        {
            PX_CORE_INFO("Down is pressed"), rotate.x -= 1.f;
        }

        /*if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        {
            ubo.transform.rotation += m_LookSpeed * ts * glm::normalize(rotate);
        }

        // limit pitch values between about +/- 85ish degrees
        ubo.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        ubo.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        const glm::vec3 upDir{ 0.f, -1.f, 0.f };*/

        glm::vec3 moveDir{ 0.f };
        // #### X-axis ####
        // To move up on x-axis (right in sence)
        if (Input::IsKeyPressed(Key::D))
        {
            PX_CORE_INFO("D is pressed"), moveDir.x += 1;
        }

        // To move down on x-axis (left in sence)
        if (Input::IsKeyPressed(Key::A))
        {
            PX_CORE_INFO("A is pressed"), moveDir.x -= 1;
        }

        // #### Y-axis ####
        // To move up on y-axis (up in sence)
        if (Input::IsKeyPressed(Key::E))
        {
            PX_CORE_INFO("E is pressed"), moveDir.y += 1;
        }

        // To move down on y-axis (down in sence)
        if (Input::IsKeyPressed(Key::Q))
        {
            PX_CORE_INFO("Q is pressed"), moveDir.y -= 1;
        }

        // #### Z-axis ####
        // To move up on y-axis (forward in sence)
        if (Input::IsKeyPressed(Key::W))
        {
            PX_CORE_INFO("W is pressed"), moveDir.z += 1;
        }

        // To move up on y-axis (backwards in sence)
        if (Input::IsKeyPressed(Key::S))
        {
            PX_CORE_INFO("S is pressed"), moveDir.z -= 1;
        }
    }
}