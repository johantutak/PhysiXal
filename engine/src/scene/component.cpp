#include "px_pch.h"
#include "scene/component.h"

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Model Transform
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Transform::InitModelMatrix()
    {
        s_ModelMatrix = glm::mat4(1.0f);
        s_ModelMatrix = glm::translate(s_ModelMatrix, s_Position);
        s_ModelMatrix = glm::rotate(s_ModelMatrix, glm::radians(s_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        s_ModelMatrix = glm::rotate(s_ModelMatrix, glm::radians(s_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        s_ModelMatrix = glm::rotate(s_ModelMatrix, glm::radians(s_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        s_ModelMatrix = glm::scale(s_ModelMatrix, s_Scale);
    }
    
    void Transform::SetPosition(const glm::vec3& position)
    {
        s_Position = position;
    }

    void Transform::SetRotation(const glm::vec3& rotation)
    {
        s_Rotation = rotation;
    }
    
    void Transform::SetScale(const glm::vec3& scale)
    {
        s_Scale = scale;
    }

    glm::mat4 Transform::GetModelMatrix()
    {
        return s_ModelMatrix;
    }
}