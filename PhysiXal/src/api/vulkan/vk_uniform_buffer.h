#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

namespace PhysiXal {

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class VulkanUniformBuffer
	{
	public:
		// Descriptor pool
		void CreateDescriptorSetLayout();
		void DestroyDescriptorSetLayout();

		static VkDescriptorSetLayout GetVulkanDescriptorSetLayout() { return s_DescriptorSetLayout; }

		// Uniform buffer
		void CreateUniformBuffers();
		void DestroyUnifromBuffers();

		void UpdateUniformBuffer(uint32_t currentImage);

		static std::vector<VkBuffer> GetVulkanUniformBuffers() { return s_UniformBuffers; }
	private:
		inline static VkDescriptorSetLayout s_DescriptorSetLayout;

		inline static std::vector<VkBuffer> s_UniformBuffers;
		inline static std::vector<VkDeviceMemory> s_UniformBuffersMemory;
		inline static std::vector<void*> s_UniformBuffersMapped;
	};
}
