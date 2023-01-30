#pragma once

#include <vulkan/vulkan.h>

namespace PhysiXal {

	class VulkanModel
    {
	public:
		// Model
		void LoadModel();

        static std::vector<Vertex> GetVulkanVertices() { return s_Vertices; }
		static std::vector<uint32_t> GetVulkanIndices() { return s_Indices; }
	private:
		static inline std::vector<Vertex> s_Vertices;
    	static inline std::vector<uint32_t> s_Indices;
	};
}