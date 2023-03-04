#pragma once

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

#include "core/platform_detetction.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "api/vulkan/vk_buffer.h"

// Core

// #### Variable notations ####

//	*	m for members
//	*	c for constants/readonlys
//	*	p for pointer (and pp for pointer to pointer)
//	*	v for volatile
//	*	s for static
//	*	i for indexes and iterators or integers
//	*	e for events
//	*	g for global
//  *   u for unsigned integer

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

#ifdef PX_ENABLE_ASSERTS
	#define PX_ASSERT(x, ...) { if(!(x)) { PX_ERROR("Assertion Failed: {0}", __VA_ARGS__); PX_DEBUGBREAK(); } }
	#define PX_CORE_ASSERT(x, ...) { if(!(x)) { PX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); PX_DEBUGBREAK(); } }
#else
	#define PX_ASSERT(x, ...)
	#define PX_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define PX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace PhysiXal {

	// Creates a scope
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// Creates a ref #### TO DO #### integrate into the real Ref Class
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}


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