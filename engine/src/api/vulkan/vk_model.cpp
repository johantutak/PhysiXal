#include "px_pch.h"
#include "api/vulkan/vk_model.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <tiny_obj_loader.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Model
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanModel::LoadModel()
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Loading model");

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        PX_PROFILE_SCOPE("tinyobj::LoadObj - VulkanModel::LoadModel()");

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) 
        {
            PX_CORE_ERROR(warn + err);
        }

        std::unordered_map<Vertex, U32> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) 
            {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<U32>(s_Vertices.size());
                    s_Vertices.push_back(vertex);
                }

                s_Indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}