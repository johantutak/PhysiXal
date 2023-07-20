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

		static VkDescriptorSetLayout GetVulkanUniformDescriptorSetLayout() { return s_UniformDescriptorSetLayout; }
		static VkDescriptorSetLayout GetVulkanTextureDescriptorSetLayout() { return s_TextureDescriptorSetLayout; }

		// Descriptor pool
		void CreateDescriptorPool();
		void DestroyDescriptorPool();

		// Descriptor Set
		void CreateUniformDescriptorSets();
		void CreateTextureDescriptorSet();

		void DestroyUniformDescriptorSets();
		void DestroyTextureDescriptorSet();

		static VkDescriptorPool GetVulkanDescriptorPool() { return s_DescriptorPool; }

		static std::vector<VkDescriptorSet> GetVulkanUniformDescriptorSets() { return s_UniformDescriptorSets; }
		static VkDescriptorSet GetVulkanTextureDescriptorSet() { return s_TextureDescriptorSet; }

		// Uniform buffer
		void CreateUniformBuffers();
		void DestroyUnifromBuffers();

		void UpdateUniformBuffer(U32 currentImage);

		static std::vector<VkBuffer> GetVulkanUniformBuffers() { return s_UniformBuffers; }
	private:
		inline static VkDescriptorSetLayout s_UniformDescriptorSetLayout;
		inline static VkDescriptorSetLayout s_TextureDescriptorSetLayout;

		inline static std::vector<VkBuffer> s_UniformBuffers;
		inline static std::vector<VkDeviceMemory> s_UniformBuffersMemory;
		inline static std::vector<void*> s_UniformBuffersMapped;

		inline static VkDescriptorPool s_DescriptorPool;

		inline static std::vector<VkDescriptorSet> s_UniformDescriptorSets;
		inline static VkDescriptorSet s_TextureDescriptorSet;
	};
}
