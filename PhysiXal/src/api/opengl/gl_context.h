#pragma once

#ifdef PX_PLATFORM_LINUX
struct GLFWwindow;
#endif

namespace PhysiXal {
    
#ifdef PX_PLATFORM_LINUX
    
    class OpenGLContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        
        void CreateContext();
        void DestroyContext();
        
        void SwapBuffers();
    private:
        GLFWwindow* m_WindowHandle;
    };
#endif
}