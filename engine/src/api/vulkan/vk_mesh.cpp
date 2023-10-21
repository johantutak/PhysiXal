#include "px_pch.h"
#include "api/vulkan/vk_mesh.h"

#include "api/vulkan/vk_utilities.h"
#include "api/vulkan/vk_initializers.h"

#include "asset/asset_manager.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <tiny_obj_loader.h>

namespace PhysiXal {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Model
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VulkanMesh::LoadMesh(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<U32>& indices)
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_INFO("Loading model");

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        PX_PROFILE_SCOPE("tinyobj::LoadObj - VulkanModel::LoadModel()");

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
        {
            PX_CORE_ERROR(warn + err);
        }

        std::unordered_map<Vertex, U32> uniqueVertices{};

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.pos =
                {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord =
                {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0) 
                {
                    uniqueVertices[vertex] = static_cast<U32>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    void VulkanMesh::UnloadMesh(std::vector<Vertex>& vertices, std::vector<U32>& indices)
    {
        PX_PROFILE_FUNCTION();

        PX_CORE_WARN("...Unloading mesh data (vertices & indices)");

        mesh.indices.clear();
        mesh.vertices.clear();
    }

}

