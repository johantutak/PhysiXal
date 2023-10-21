#pragma once

#include "api/vulkan/vk_buffer.h"

#include <vulkan/vulkan.h>

namespace PhysiXal {

	struct Mesh {
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		std::vector<Vertex> vertices;
		std::vector<U32> indices;
	};

	class VulkanMesh
    {
	public:
		// Model
		void LoadMesh(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<U32>& indices);
		void UnloadMesh(std::vector<Vertex>& vertices, std::vector<U32>& indices);

		static Mesh& GetVulkanMesh() { return mesh; }
	private:
		inline static Mesh mesh;
	};
}