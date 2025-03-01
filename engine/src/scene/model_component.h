#pragma once

#include "ecs/ecs.h"
#include <string>

namespace PhysiXal {

    // ModelComponent now derives from Component.
    class ModelComponent : public Component {
    public:
        std::string MeshPath;
        std::string VertexShaderPath;
        std::string FragmentShaderPath;
        std::string TexturePath;
    };
}