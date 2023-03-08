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

#include "core/platform_detetction.h"

#include <memory>

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

#include "core/scope.h"
#include "core/ref.h"

#include "core/assert.h"
#include "log/log.h"

#include "core/hash.h"

namespace PhysiXal {

	class Core
	{
	public:
		// Core
		static void InitializeCore();
		static void ShutdownCore();
	private:

	};
}