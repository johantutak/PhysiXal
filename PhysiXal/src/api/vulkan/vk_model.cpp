#include "px_pch.h"
#include "api/vulkan/vk_model.h"

#include "api/vulkan/vk_utilities.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Model
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VulkanModel::LoadModel()
	{
		PX_CORE_INFO("Loading model");

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.s_Indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.s_Vertices[3 * index.vertex_index + 0],
                    attrib.s_Vertices[3 * index.vertex_index + 1],
                    attrib.s_Vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(s_Vertices.size());
                    s_Vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
	}
}