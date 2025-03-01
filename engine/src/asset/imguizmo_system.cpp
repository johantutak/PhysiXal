#include "px_pch.h"
#include "asset/imguizmo_system.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "scene/transform_component.h"

namespace PhysiXal {

    void ImGuizmoSystem::ManipulateSelectedEntity(const glm::mat4& view, const glm::mat4& projection, ECSManager* ecs) {
        Entity* selected = ecs->GetSelectedEntity();
        if (!selected)
            return;
        TransformComponent* transform = selected->GetComponent<TransformComponent>();
        if (!transform)
            return;

        glm::mat4 modelMatrix = transform->GetModelMatrix();

        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
            ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE | ImGuizmo::OPERATION::SCALE,
            ImGuizmo::MODE::WORLD, glm::value_ptr(modelMatrix));

        if (ImGuizmo::IsUsing()) {
            glm::vec3 scale, translation, skew;
            glm::vec4 perspective;
            glm::quat rotation;
            glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);
            transform->Position = translation;
            transform->Rotation = rotation;
            transform->Scale = scale;
        }
    }
}