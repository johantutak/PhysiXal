#include "px_pch.h"
#include "scene/component.h"

namespace PhysiXal {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Model Transform
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Transform::InitModelMatrix()
    {
        m_ModelMatrix = glm::mat4(1.0f);
        m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
    }
    
    void Transform::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
    }

    void Transform::SetRotation(const glm::vec3& rotation)
    {
        m_Rotation = rotation;
    }
    
    void Transform::SetScale(const glm::vec3& scale)
    {
        m_Scale = scale;
    }

    glm::mat4 Transform::GetModelMatrix()
    {
        return m_ModelMatrix;
    }
}