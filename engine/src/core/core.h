#pragma once

/**								#### PhysiXal Engine ⟨™⟩ - ©2023 - by Johan Tutak ####								**/
/**
* 
* Notable mentions:
* 
* - This engine:
* 
* - is inspiered by Hazel 2D/3D and Walnut made by TheCherno at GitHub.com and got some of it's code base that is simillar
* - or modified from it.
* 
* - is inspiered by Little Vulkan Engine made by blurrypiano at GitHub.com and got some of it's code base that is simillar
* - or modified from it.
* 
* - is inspiered by Vulkan Tutorial made by Alexander Overvoorde at vulkan-tutorial.com and got some of it's code base that is simillar
* - or modified from it.
* 
* - is inspiered by Pelican Engine made by SeppahBaws at GitHub.com and got some of it's code base that is simillar
* - or modified from it.
* 
**/

#include <memory>

namespace PhysiXal {

	// Core
	void InitializeCore();
	void ShutdownCore();

}

#include "core/platform_detetction.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "api/vulkan/vk_buffer.h"

// #### Macros ####

#ifdef PX_DEBUG
	#if defined(PX_PLATFORM_WINDOWS)
		#define PX_DEBUGBREAK() __debugbreak()
	#elif defined(PX_PLATFORM_LINUX)
		#include <signal.h>
		#define PX_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define PX_ENABLE_ASSERTS
#else
	#define PX_DEBUGBREAK()
#endif

#define PX_EXPAND_MACRO(x) x
#define PX_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define PX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

// #### Pointer wrappers ####

namespace PhysiXal {

	// Creates a scope
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// #### TO DO #### integrate into it's own ref class
	// Creates a ref
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "core/assert.h"
#include "log/log.h"

namespace std {

	// Creates a hash
	template<> struct hash<PhysiXal::Vertex> 
	{
		size_t operator()(PhysiXal::Vertex const& vertex) const 
		{
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}