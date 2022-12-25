#include "px_pch.h"
#include "api/opengl/gl_context.h"

#include <GLFW/glfw3.h>
#ifdef PX_PLATFORM_LINUX
    #include <glad/glad.h>
#endif

namespace PhysiXal {

#ifdef PX_PLATFORM_LINUX
    
    void OpenGLContext::InitContext()
    {
         PX_CORE_INFO("Creating OpenGL context");
         
         int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
         PX_CORE_ASSERT(status, "Failed to initialize Glad!");
         
         PX_CORE_INFO("OpenGL Info:");
         PX_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
         PX_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
         PX_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef PX_ENABLE_ASSERTS
         int versionMajor;
         int versionMinor;
         glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
         glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

         PX_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 6), "PhysiXal requires at least OpenGL version 4.6!");
#endif
    }
    
     void OpenGLContext::DestroyContext()
     {
         PX_CORE_WARN("...Destroying OpenGL context (Gets destroyed alongside with the window)");
     }
#endif
}