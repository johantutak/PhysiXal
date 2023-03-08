#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "api/vulkan/vk_buffer.h"

// Creates a hash
template<> struct std::hash<PhysiXal::Vertex>
{
	size_t operator()(PhysiXal::Vertex const& vertex) const
	{
		return ((std::hash<glm::vec3>()(vertex.pos) ^ (std::hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (std::hash<glm::vec2>()(vertex.texCoord) << 1);
	}
};