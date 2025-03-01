#pragma once
#include "ecs/ecs.h"
#include <glm/glm.hpp>

namespace PhysiXal {

    class ImGuizmoSystem {
    public:
        // Call this each frame in the GUI layer
        static void ManipulateSelectedEntity(const glm::mat4& view, const glm::mat4& projection, ECSManager* ecs);
    };
}