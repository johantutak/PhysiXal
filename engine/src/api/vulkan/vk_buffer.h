#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

#include <array>

#include <glm/glm.hpp>

namespace PhysiXal {

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription GetBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	};

	class VulkanBuffer
	{
	public:
		// Buffer creation (staging buffer)
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		// Vertex buffer
		void CreateVertexBuffer(VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory, std::vector<Vertex>& vertices);
		void DestroyVertexBuffer(VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
		U32 FindMemoryType(U32 typeFilter, VkMemoryPropertyFlags properties);

		// Index buffer
		void CreateIndexBuffer(VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory, std::vector<U32>& indices);
		void DestroyIndexBuffer(VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
	private:
		
	};
}
