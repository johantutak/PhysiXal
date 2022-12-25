#include "px_pch.h"
#include "api/opengl/gl_renderer.h"

#include "api/opengl/gl_context.h"
#include "api/opengl/gl_shader.h"

#include <GLFW/glfw3.h>

namespace PhysiXal {

#ifdef PX_PLATFORM_LINUX

    void OpenGLRenderer::Init()
    {
        PX_CORE_INFO("Initializing the renderer");
        OpenGLContext::InitContext();
    }
	
    void OpenGLRenderer::Shutdown()
    {
        PX_CORE_WARN("...Shutting down the renderer");
        OpenGLContext::DestroyContext();
    }

    void OpenGLRenderer::BeginFrame()
    {
       
    }
#endif
}