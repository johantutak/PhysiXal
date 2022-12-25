#pragma once

namespace PhysiXal {
    
#ifdef PX_PLATFORM_LINUX

    struct GLFWwindow;
    
    class OpenGLContext
    {
    public:
        static void InitContext();
        static void DestroyContext();
    private:
    };
#endif
}