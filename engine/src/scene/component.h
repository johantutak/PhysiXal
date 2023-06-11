#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PhysiXal {

    class Transform
    {
    public:
        static void InitModelMatrix();

        static void SetPosition(const glm::vec3& position);
        static void SetRotation(const glm::vec3& rotation);
        static void SetScale(const glm::vec3& scale);

        static glm::mat4 GetModelMatrix();
    private:
        inline static glm::mat4 m_ModelMatrix;

        inline static glm::vec3 m_Position;
        inline static glm::vec3 m_Rotation;
        inline static glm::vec3 m_Scale;
    };
}