#pragma once

#include "ecs/ecs.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PhysiXal {

    // TransformComponent now derives from Component.
    class TransformComponent : public Component {
    public:
        glm::vec3 Position = glm::vec3(0.0f);
        glm::quat Rotation = glm::quat(glm::vec3(0.0f));
        glm::vec3 Scale = glm::vec3(1.0f);

        glm::mat4 GetModelMatrix() const {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), Position);
            model *= glm::mat4_cast(Rotation);
            model = glm::scale(model, Scale);
            return model;
        }
    };

}
