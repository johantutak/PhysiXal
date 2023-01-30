#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

#include <array>

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
	};

	class VulkanBuffer
	{
	public:
		// Buffer creation (staging buffer)
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		// Vertex buffer
		void CreateVertexBuffer();
		void DestroyVertexBuffer();
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static VkBuffer GetVulkanVertexBuffer() { return s_VertexBuffer; }

		// Index buffer
		void CreateIndexBuffer();
		void DestroyIndexBuffer();

		static VkBuffer GetVulkanIndexBuffer() { return s_IndexBuffer; }
	private:
		static inline VkBuffer s_VertexBuffer;
		static inline VkDeviceMemory s_VertexBufferMemory;

		static inline VkBuffer s_IndexBuffer;
		static inline VkDeviceMemory s_IndexBufferMemory;
	};
}
