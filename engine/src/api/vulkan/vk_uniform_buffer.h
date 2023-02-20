#pragma once

#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

namespace PhysiXal {

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	class VulkanUniformBuffer
	{
	public:
		// Descriptor set layout
		void CreateDescriptorSetLayout();
		void DestroyDescriptorSetLayout();

		static VkDescriptorSetLayout GetVulkanDescriptorSetLayout() { return s_DescriptorSetLayout; }

		// Descriptor pool
		void CreateDescriptorPool();
		void DestroyDescriptorPool();
		void CreateDescriptorSets();

		static VkDescriptorPool GetVulkanDescriptorPool() { return s_DescriptorPool; }
		static std::vector<VkDescriptorSet> GetVulkanDescriptorSets() { return s_DescriptorSets; }

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

		inline static VkDescriptorPool s_DescriptorPool;
		inline static std::vector<VkDescriptorSet> s_DescriptorSets;
	};
}
