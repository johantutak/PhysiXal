#pragma once

// ---- For use by PhysiXal applications ---------------------

// From Hazel Engine

#include "core/core.h"

#include "core/application.h"
#include "core/core_layer.h"
#include "log/log.h"

// ---- UI ---------------------------------------------------


// ---- Renderer ---------------------------------------------

#ifdef PX_PLATFORM_WINDOWS
#include "api/vulkan/vk_renderer.h"
#endif
#ifdef PX_PLATFORM_LINUX
#include "api/opengl/gl_renderer.h"
#include "api/opengl/gl_shader.h"
#endif

// -----------------------------------------------------------