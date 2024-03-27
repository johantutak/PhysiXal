#include "px_pch.h"
#include "scene/component.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include <glm/gtx/matrix_decompose.hpp>

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

    void Transform::ManipulateModelMatrix(const glm::mat4& view, const glm::mat4& projection, glm::mat4& modelMatrix)
    {
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(modelMatrix));

        if (ImGuizmo::IsUsing())
        {
            // If you want to update the static members after manipulation
            glm::vec3 scale, translation, skew;
            glm::vec4 perspective;
            glm::quat rotation;

            // Decompose the model matrix into its components
            glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

            // Update the static member variables with the decomposed values
            s_Position = translation;
            s_Rotation = glm::eulerAngles(rotation); // Convert quaternion to euler angles
            s_Scale = scale;

            // If you want the rotation in degrees, convert from radians
            s_Rotation = glm::degrees(s_Rotation);

            // Convert Euler angles (in degrees) back to a quaternion for rotation
            glm::quat rotationQuat = glm::quat(glm::radians(s_Rotation));

            // Recompose the matrix manually
            s_ModelMatrix = glm::translate(glm::mat4(1.0f), s_Position) * glm::mat4(rotationQuat) * glm::scale(glm::mat4(1.0f), s_Scale);
        }
    }
}